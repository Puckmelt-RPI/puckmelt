//Lobotomelt_V1
//Basic Meltybrain tank drive software
//Main program switches to standard tank drive when the controller sends a command
//Other modules temporarily use spring 2026 versions

//module inclusions
#include "radio.h"
#include "motor.h"

receiver radio; //Represents actual receiver on the robot. Read user input off this object
puckMotor leftMotor(4);
puckMotor rightMotor(5);

void setup() {
  //Setup code runs once on startup
  radio.init(); //Initializes CRSF communication
  leftMotor.init();
  rightMotor.init();
}

//Tank drive
void lobotoMelt(){
  radio.handle(); //Processes incoming CRSF data

  //Stops both motors when either drive channel has no valid input
  if (radio.fb_axis == 0 || radio.lr_axis == 0) {
    leftMotor.throttle(0);
    rightMotor.throttle(0);
    return;
  }

  float front_back = radio.axisPercent(radio.fb_axis);
  float left_right = radio.axisPercent(radio.lr_axis);

  float left = front_back + left_right;
  float right = front_back - left_right;

  //Scales both commands together to preserve steering proportions
  float largest = max(abs(left), abs(right));
  if (largest > 100.0f) {
    left *= 100.0f / largest;
    right *= 100.0f / largest;
  }

  leftMotor.throttle(left);
  rightMotor.throttle(right);
}


void loop() {
  //Main code runs repeatedly
  lobotoMelt();
}
