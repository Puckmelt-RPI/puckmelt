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
      crsf = new CRSFforArduino();
      delay(10);
    }
  }
}

//Updating channels when a signal is received
void receiver::onReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
  if (!rcChannels->failsafe) { //Updates channels when the built-in failsafe is inactive
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
  if (quality > 80) { //Records signals with quality above 80 percent
    last_receive_time = millis();
  }

  if (last_receive_time - millis() > 1000) { //Clears drive channels when the timeout condition is met
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

//Processing receiver updates
void receiver::handle() {
  crsf->update();
  //Repeats the signal loss check in case the link statistics callback is not called
  if (last_receive_time - millis() > 1000) { //Clears drive channels when the timeout condition is met
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}
