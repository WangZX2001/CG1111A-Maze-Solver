void forward(){
  rightMotor.run(speed_right);
  leftMotor.run(-speed_left);
}
//Turn Functions for Obstacle Avoidance
void adjustRight(){
  leftMotor.run(-200);
  rightMotor.run(turnSpeed);
  delay(165); //200 175
}
void adjustLeft(){
  rightMotor.run(200);
  leftMotor.run(-turnSpeed+20);
  delay(120); //200 175 165 150,145
}
void stop(int time) {
  leftMotor.stop();
  rightMotor.stop();
  delay(time);
}
void moveOneBoxblue() {
  stop(FORWARD_DELAY);
  leftMotor.run(-SPEED);
  rightMotor.run(SPEED);
  delay(ONEBOX_DELAY_blue);
  stop(FORWARD_DELAY);
}

void moveOneBoxpink() {
  stop(FORWARD_DELAY);
  leftMotor.run(-SPEED);
  rightMotor.run(SPEED);
  delay(ONEBOX_DELAY_pink);
  stop(FORWARD_DELAY);
}

void turnRight() {
  leftMotor.run(-SPEED); // Left wheel goes anti-clockwise
  rightMotor.run(-SPEED); //right wheel goes anti-clockwise
  delay(TURN_DELAY_right); 
}
void turnLeft() {
  leftMotor.run(SPEED);
  rightMotor.run(SPEED);
  delay(TURN_DELAY_left);
}
void uTurn() {
  turnLeft();
  delay(UTURN_DELAY);
}
void twoLeftTurn() {
  turnLeft();
  moveOneBoxpink();
  turnLeft();
}
void twoRightTurn() {
  turnRight();
  moveOneBoxblue();
  turnRight();
}

//Indefinite loop
void stop_indef()
{
  leftMotor.stop();
  rightMotor.stop();
  while(true)
  {
    delay(10);
  }
}
