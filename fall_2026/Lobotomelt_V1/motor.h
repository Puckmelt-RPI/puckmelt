#ifndef motor_H
#define motor_H

//Importing required libraries & files
#include <Arduino.h>
#include <DShotRMT.h>

class puckMotor{
    
    private:

        DShotRMT motor;
        float throttlePercent; //Throttle percent
        gpio_num_t motorPin;

        bool reversed;
    
    public:

        //Constructor
        puckMotor(const int GPIO_Pin); //Note: GPIO_Pin is an integer, not a gpio_num_t. Gets converted to that inside constructor

        void init(); //Initializes & begins the motor

        //Setters
        void throttle(float newThrottle);

        void defaultRotation(); //Motor rotates in its default direction
        void reverseRotation(); //Motor rotates in reverse direction

        void shutdown(); //Terminates rotation 

        //Destructor
        ~puckMotor();

};

#endif
