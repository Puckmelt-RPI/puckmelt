#pragma once

//Importing required libraries & files
#include <CRSFforArduino.hpp>
#include "network_id.h"

class receiver {
  public:
  static inline int16_t throttle;
  static inline int16_t fb_axis;
  static inline int16_t lr_axis;
  static inline int16_t angle;
  static inline int8_t quality; //Signal quality percentage

  static inline CRSFforArduino* crsf;
  static inline unsigned long last_receive_time;
  static inline bool watchdog_enable = false;

  static void init(); //Initializes & begins the receiver

  //Callbacks
  static void onReceive(serialReceiverLayer::rcChannels_t *rcChannels); //Updates received channels
  static void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics); //Updates signal quality & checks for signal loss

  static void handle(); //Processes receiver updates & checks for signal loss
};
