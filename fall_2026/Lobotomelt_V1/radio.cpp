//radio.cpp
//CRSF communication implementation
//Designer: Grant Williams

//Import required libraries & files
#include "radio.h"
#include "network_id.h"

//Receive callback declarations
void onReceive(serialReceiverLayer::rcChannels_t *rcChannels);
void printOnReceive(serialReceiverLayer::rcChannels_t *rcChannels);

//Initializing receiver
void receiver::init() {
  crsf = new CRSFforArduino(&Serial0,RX,TX);
  while(true) { //Retries until the receiver initializes successfully
    if(crsf->begin()){
      crsf->setRcChannelsCallback(receiver::onReceive);
      break;
    } else {
      Serial.println("crsf failed");
      crsf->end();
      delete crsf;
      crsf = new CRSFforArduino(&Serial0,RX,TX);
      delay(10);
    }
  }
}

//Converting a controller axis into a signed percentage
float receiver::axisPercent(int value) {
  const int center = 1500;
  const int deadzone = 25;
  const int halfRange = 500; //Assumes controller endpoints of 1000 & 2000

  if (value == 0) { //Zero marks startup, failsafe or signal timeout
    return 0;
  }

  int offset = value - center;
  int magnitude = abs(offset);
  if (magnitude <= deadzone) {
    return 0;
  }

  //Removes the dead zone & scales the remaining stick travel
  float percent = (magnitude - deadzone) * 100.0f / (halfRange - deadzone);
  percent = constrain(percent, 0.0f, 100.0f);
  return offset < 0 ? -percent : percent;
}

//Updating channels when a signal is received
void receiver::onReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
  if (!rcChannels->failsafe) { //Updates channels when the built-in failsafe is inactive
    last_receive_time = millis(); //Records the last valid control packet
    angle = crsf->rcToUs(crsf->getChannel(1));
    throttle = crsf->rcToUs(crsf->getChannel(2));
    fb_axis = crsf->rcToUs(crsf->getChannel(4));
    lr_axis = crsf->rcToUs(crsf->getChannel(3));
  } else { //Clears drive channels when the failsafe is active
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

//Updating signal quality & checking for signal loss
void receiver::onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics) {
  quality = linkStatistics.lqi;
  if (millis() - last_receive_time > 1000) { //Clears drive channels after one second without a valid control packet
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

//Processing receiver updates
void receiver::handle() {
  crsf->update();
  //Checks for signal loss on every loop, even when no callbacks arrive
  if (millis() - last_receive_time > 1000) { //Clears drive channels after one second without a valid control packet
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}
