//Lobotomelt_V1
//Basic Meltybrain tank drive software
//Main program switches to standard tank drive when the controller sends a command
//Other modules temporarily use spring 2026 versions

#include "radio.h"
#include "motor.h"

Receiver radio; //Represents actual receiver on the robot. Read user input off this object
puckMotor leftMotor(4);
puckMotor rightMotor(5);

//Initializing the program
void setup() {
  //Setup code runs once on startup



}

//Running the main program
void loop() {
  //Main code runs repeatedly

}
