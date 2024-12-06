/** This is the main code of our mBot used for Test Run. It has two other ino --> LDR.ino and motor.ino. 
Remember to include these two INO in the correct sequence. 
Purpose of the LDR.ino is to callibrate the LDR and get the analog readings from LDR
Purpose of the Motor.ino is to declare the actions carried out by the motors
**/
#include <MeMCore.h> 

//Define time delays for LDR, values are used in LDR.ino
#define RGBWait 200 //in ms
#define LDRWait 10  //in ms 

//Define time delays for Motor, values are used in motor.ino
#define SPEED 255 //speed between 0 - 255
#define FORWARD_DELAY 100 //in ms
#define TURN_DELAY_right 320//in ms //Before callibration: 335
#define TURN_DELAY_left 310 //in ms //Before callibration：300, 320
#define ONEBOX_DELAY_blue 640//in ms //Before callibration: 610
#define ONEBOX_DELAY_pink 660 //in ms
#define UTURN_DELAY 290 //in ms

//Define pins
#define DECODER_PIN_A A0  //Decoder control pin A
#define DECODER_PIN_B A1 //Decoder control pin B
#define IR A2 //IR input PIN at A2
#define LDR A3 //LDR sensor pin at A3
#define PUSH_BUTTON A7

//Define some global variables, variables are used inside the main loop
int ir_value; 
float ultradistance;

//Define some motor speed because the mBot doesn't move in a straightline
uint8_t speed_right = 215; //Before callibration: 215, 245,255 //235, 225, 215
uint8_t speed_left = 235;  //Before callibration: 200 original 235
uint8_t turnSpeed = 100; //Before callibration: 100, 80,75

//Default mBot pin declaration
MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
MeRGBLed led(0, 30); // set up LED pin
MeLineFollower lineFinder(PORT_1); // assigning Line Follower to port 1
MeUltrasonicSensor ultraSensor(PORT_2); // assigning UR to port 2
MeBuzzer buzzer;

// Placeholders for color detection (Final Test Run values)
float colourArray[] = {0, 0, 0}; // Detected color intensities
float whiteArray[] = {907, 906, 768}; //{887, 915, 763};  // White calibration values
float blackArray[] = {771,672,430};//{791, 729, 377};  // Black calibration values
float greyDiff[] = {136, 234, 338} ; //{96, 186, 386};    // Range (white - black)

// check calibration on the test day (Final Test Run values)
// Manhattan Distance
int calibratedColors[6][3] = { 
  {132, 82, 77},//Before Callibration: {155, 84, 103},  // Red
  {55, 198, 136},//{45, 196, 151},   // Green
  {150, 152, 95}, //{168, 145, 108},  // Orange
  {152, 209, 198}, //{168, 209, 220},  //{360, 235, 236},  // Pink
  {51, 191, 201}, //{45, 191, 218}, //Blue
  {162, 245, 245}, //white
};

/*Euclidean Distance
//Did not Implement this for Final Test Run
double squaredColors[6] = { 
   195.369,//{155, 84, 103},  // Red
   266.207,//{45, 196, 151},   // Green
   256.144, //{168, 145, 108},  // Orange
   359.369, //{168, 209, 220},  //{360, 235, 236},  // Pink
   308.146, //{45, 191, 218}, //Blue
   382.484, //white
};*/

// Display colors for RGB LED, for debugging purposes
int displayLED[6][3] = { 
    {255, 0, 0},     // Red
    {0, 255, 0},     // Green
    {255, 50, 0},    // Orange
    {128, 0, 128},   // Pink
    {0, 0, 255},      // Blue 
    {255,255,255},  //white
};

// Color names for serial output for debug
char* colorNames[] = {"Red", "Green", "Orange", "Pink", "Blue","white"};

//Action executed after LDR decodes the colour of the paper, function inside motor.ino
void action() {
  int color = detectColor(); // Each LDR number corresponds to a specific colour detected.
  if (color == 0) { // Red
    turnLeft();
  } else if (color == 1) { // Green
    turnRight();
  } else if (color == 2) { // Orange 
    uTurn();
  } else if (color == 3) { // Purple 
    twoLeftTurn();
  } else if (color == 4) { // Blue
    twoRightTurn();
  }
   else { //White
    celebrate();
    stop_indef();
  } 
}

void setup() {
    pinMode(PUSH_BUTTON, INPUT);
    pinMode(DECODER_PIN_A, OUTPUT);
    pinMode(DECODER_PIN_B, OUTPUT);
    led.setpin(13); // Set the LED pin
    pinMode(A2, INPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    Serial.begin(9600); // Begin serial communication
    //setBalance(); // Calibrate LDR with black and white(Disabled after callibration)
    digitalWrite(A0,LOW);
    digitalWrite(A1,LOW);
}

void celebrate()
{
 // Star Wars Main Theme (simplified)
  led.setColor(displayLED[0][0], displayLED[0][1], displayLED[0][2]);
  led.show();
  // Note: G
  buzzer.tone(392, 500);  // G4 for 500 ms
  delay(100);
  led.setColor(displayLED[0][0], displayLED[0][1], displayLED[0][2]);
  led.show();
  
  // Note: G
  buzzer.tone(392, 500);  // G4 for 500 ms
  delay(100);
  led.setColor(displayLED[0][0], displayLED[0][1], displayLED[0][2]);
  led.show();  
  
  // Note: G
  buzzer.tone(392, 500);  // G4 for 500 ms
  delay(100);
  led.setColor(displayLED[0][0], displayLED[0][1], displayLED[0][2]);
  led.show();  
 
  // Note: Eb
  buzzer.tone(311, 350);  // Eb4 for 350 ms
  led.setColor(displayLED[1][0], displayLED[1][1], displayLED[1][2]);
  led.show(); 
  
  // Note: Bb
  buzzer.tone(466, 150);  // Bb4 for 150 ms
  led.setColor(displayLED[2][0], displayLED[2][1], displayLED[2][2]);
  led.show();
  
  // Note: G
  buzzer.tone(392, 500);  // G4 for 500 ms
  led.setColor(displayLED[0][0], displayLED[0][1], displayLED[0][2]);
  led.show();
  
  // Note: Eb
  buzzer.tone(311, 350);  // Eb4 for 350 ms
  led.setColor(displayLED[1][0], displayLED[1][1], displayLED[1][2]);
  led.show();
  
  // Note: Bb
  buzzer.tone(466, 150);  // Bb4 for 150 ms
  led.setColor(displayLED[2][0], displayLED[2][1], displayLED[2][2]);
  led.show();
  
  // Note: G
  buzzer.tone(392, 1000); // G4 for 1000 ms
  led.setColor(displayLED[0][0], displayLED[0][1], displayLED[0][2]);
  led.show();
  delay(1100);
}


void loop() {
  ir_value = analogRead(A2);
  ultradistance = ultraSensor.distanceCm();
  forward();
  Serial.println(ir_value);
  if (ultradistance <= 8) // too close to the left wall. Before Callibration: 10
  {
    adjustRight();
  }
  if (ir_value <= 225) // too close to the right wall. Before Callibration: 150, 170,180,200,250,275,
  {
    adjustLeft();
  }
  else{
    forward();
  }
  int sensorState = lineFinder.readSensors();
  if (sensorState == S1_IN_S2_IN) { //inside black line, LDR operates and starts action
  stop(10);
  action();
  } 
}

