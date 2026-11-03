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

//Throttle Setter (this takes throttlePercent values)
void puckMotor::throttle(float newThrottle){

  throttlePercent= newThrottle;
  motor.sendThrottlePercent(throttlePercent);
  
  Serial.print(motorPin);
  Serial.print(" throttle: ");
  Serial.print(throttlePercent);
  Serial.println("%");
}

//Default motor direction
void puckMotor::defaultRotation(){ 
  
  motor.setMotorSpinDirection(false);

  Serial.print(motorPin);
  Serial.println(" rotation direction: DEFAULT");
}

//Reverse motor direction
void puckMotor::reverseRotation(){ 
  
  motor.setMotorSpinDirection(true);

  Serial.print(motorPin);
  Serial.println(" rotation direction: REVERSED");
}

//Emergency shutdown, terminates all rotation
void puckMotor::shutdown(){
  
  motor.sendThrottlePercent(0);
  throttlePercent=0;
  
  Serial.print("EMERGENCY SHUTDOWN: ");
  Serial.println(motorPin);
}
