#include <Arduino.h>
#include "wireless.h"
#include "radio.h"

Wireless laptop = Wireless();
receiver rc = receiver();

void setup() {
  Serial.begin(115200);
  laptop.init();
  rc.init();
}

void loop() {
  laptop.handle();
  rc.handle();
  Serial.println(String(rc.fb_axis));
}