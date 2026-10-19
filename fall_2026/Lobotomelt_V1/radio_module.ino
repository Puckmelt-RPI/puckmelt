//Import required libraries & files
#include <Arduino.h>
#include "wireless.h"
#include "radio.h"

//Communication objects
Wireless laptop = Wireless();
receiver rc = receiver();

//Initializing serial communication, wireless connection & receiver
void setup() {
  Serial.begin(115200);
  laptop.init();
  rc.init();
}

//Processing communication updates & printing the forward/backward axis
void loop() {
  laptop.handle();
  rc.handle();
  Serial.println(String(rc.fb_axis));
}
