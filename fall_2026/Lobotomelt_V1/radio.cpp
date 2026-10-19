#include "radio.h"
#include "network_id.h"

void onReceive(serialReceiverLayer::rcChannels_t *rcChannels);
void printOnReceive(serialReceiverLayer::rcChannels_t *rcChannels);

void receiver::init() {
  crsf = new CRSFforArduino(&Serial0,RX,TX);
  while(true) { //hold program hostage if crsf fails
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

void receiver::onReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
  if (!rcChannels->failsafe) { //if the builtin failsafe isn't active, update channels, otherwise set to 0
    angle = crsf->rcToUs(crsf->getChannel(1));
    throttle = crsf->rcToUs(crsf->getChannel(2));
    fb_axis = crsf->rcToUs(crsf->getChannel(4));
    lr_axis = crsf->rcToUs(crsf->getChannel(3));
  } else {
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

void receiver::onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics) {
  quality = linkStatistics.lqi;
  if (quality > 80) { //only consider quality signals to be a received signal
    last_receive_time = millis();
  }

  if (last_receive_time - millis() > 1000) { //if a quality signal hasn't been received for over 1 second, set channels to 0
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

void receiver::handle() {
  crsf->update();
  //redundant protocol to ensure the check is being run in the loop (crsf should be running onLinkStatisticsUpdate but just in case)
  if (last_receive_time - millis() > 1000) { //if a quality signal hasn't been received for over 1 second, set channels to 0
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}