//for calibration of LDR at the start(Disabled for Final Run)
/*void setBalance() {
    Serial.println("Place White Sample for Calibration...");
    delay(5000); // Wait for white sample

    // Scan white sample
    for (int i = 0; i <= 2; i++) {
        selectLED(i);
        delay(RGBWait);
        whiteArray[i] = getAvgReading(5);
        Serial.println(whiteArray[i]);
        delay(RGBWait);
    }
    offLED();

    Serial.println("Place Black Sample for Calibration...");
    delay(5000); // Wait for black sample

    // Scan black sample
    for (int i = 0; i <= 2; i++) {
        selectLED(i);
        delay(RGBWait);
        blackArray[i] = getAvgReading(5);
        Serial.println(blackArray[i]);
        greyDiff[i] = whiteArray[i] - blackArray[i];
        delay(RGBWait);
    }
    offLED();
    Serial.println("Calibration Completed. Ready for Color Detection.");
    delay(5000);
}*/

//LDR gets analog reading of the amount of light reflected
int getAvgReading(int times) {
    int total = 0;
    for (int i = 0; i < times; i++) {
        total += analogRead(LDR);
        delay(LDRWait);
    }
    return total / times;
}

//function that turns on selected LED color
void selectLED(int color) {
    switch (color) {
        case 0: // Red
            digitalWrite(DECODER_PIN_A, HIGH);
            digitalWrite(DECODER_PIN_B, HIGH);
            break;
        case 1: // Green
            digitalWrite(DECODER_PIN_A, LOW);
            digitalWrite(DECODER_PIN_B, HIGH);
            break;
        case 2: // Blue
            digitalWrite(DECODER_PIN_A, HIGH);
            digitalWrite(DECODER_PIN_B, LOW);
            break;
    }
}

//turn off LED and on IR
void offLED() {
  digitalWrite(DECODER_PIN_A, LOW);
  digitalWrite(DECODER_PIN_B, LOW);
}

//detect color of paper and return index of color detected
int detectColor() {
    for (int c = 0; c <= 2; c++) {
        selectLED(c); // on the selected LED
        delay(RGBWait); // Stabilize LDR
        // Average reading for current LED color
        colourArray[c] = getAvgReading(5);
        colourArray[c] = (colourArray[c] - blackArray[c]) / greyDiff[c] * 255;
        //debugging purposes
        Serial.print(blackArray[c]);
        Serial.print(whiteArray[c]);
        Serial.print(greyDiff[c]);
        Serial.print("Intensity for ");
        Serial.print(colorNames[c]);
        Serial.print(": ");
        Serial.println(int(colourArray[c]));
        delay(RGBWait);
        offLED();
    }

    int minDifference = 1000;  // Arbitrary large number
    int colorIndex = -1;
   // Euclidean Distance: double squaredDetectColor = sqrt(colourArray[0]*colourArray[0] + colourArray[1]*colourArray[1] + colourArray[2]*colourArray[2]);
    
    // Find closest match using Manhattan Distance
    for (int i = 0; i < 6; i++) {
        //Euclidean Distance: double diff = fabs(squaredDetectColor - squaredColors[i]);
        int diff = abs(colourArray[0] - calibratedColors[i][0]) +
                   abs(colourArray[1] - calibratedColors[i][1]) +
                   abs(colourArray[2] - calibratedColors[i][2]); 
        
        if (diff < minDifference) {
            minDifference = diff;
            colorIndex = i;
        }
    }
    //Index of the closest matching color
    if (colorIndex != -1) {
        Serial.print("Detected Color: ");//Debug on Serial Monitor
        Serial.println(int(colorIndex));
        Serial.println(colorNames[colorIndex]);
        led.setColor(displayLED[colorIndex][0], displayLED[colorIndex][1], displayLED[colorIndex][2]);//Visual debug
        led.show();
    }
    return colorIndex;
    //Return the index of the color detected
}