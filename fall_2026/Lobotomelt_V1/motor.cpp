//Import required libraries & files
#include <Arduino.h>
#include <DShotRMT.h>

#include "motor.h"

//Constructor
puckMotor::puckMotor(const int GPIO_Pin)
  : motor((gpio_num_t)GPIO_Pin, DSHOT150, false){
  
  throttlePercent = 0; //Ensures throttle is zero on startup
  motorPin= (gpio_num_t)GPIO_Pin;
  reversed = false;
}

//Initializing motor
void puckMotor::init(){
  motor.begin();
  motor.setMotorSpinDirection(false); //Ensures motor rotates in default direction on startup
  motor.sendThrottlePercent(0); //Ensures no rotation on startup
  
  Serial.print("Motor initialized: ");
  Serial.println(motorPin);
}

//Destructor
puckMotor::~puckMotor(){}

//Default motor direction (forwards)
void puckMotor::defaultRotation(){ 
  
  motor.setMotorSpinDirection(false);
  reversed = false;
}

//Reverse motor direction (backwards)
void puckMotor::reverseRotation(){ 
  
  motor.setMotorSpinDirection(true);
  reversed = true;
}

//Throttle Setter (this takes throttlePercent values)
void puckMotor::throttle(float newThrottle){

  throttlePercent= newThrottle;

  //Negative throttle values mean motor rotates backwards
  if (throttlePercent<0){
    reverseRotation();
    motor.sendThrottlePercent(-throttlePercent); //Sends the absolute value instead

  } 
  else{
    defaultRotation();
    motor.sendThrottlePercent(throttlePercent);
  }
  
  Serial.println(String(motorPin) + " throttle: " + String(throttlePercent) + "% " + String(reversed));
}

//Emergency shutdown, terminates all rotation
void puckMotor::shutdown(){
  
  motor.sendThrottlePercent(0);
  throttlePercent=0;
  
  Serial.print("EMERGENCY SHUTDOWN: ");
  Serial.println(motorPin);
}
