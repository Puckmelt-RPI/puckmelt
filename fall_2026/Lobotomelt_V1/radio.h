#pragma once
#include <CRSFforArduino.hpp>
#include "network_id.h"

class receiver {
  public:
  static inline uint16_t throttle;
  static inline uint16_t fb_axis;
  static inline uint16_t lr_axis;
  static inline uint16_t angle;
  static inline uint8_t quality; //percentage of information that's actually getting through. I read online that above 80% is safe

  static inline CRSFforArduino* crsf;
  static inline unsigned long last_receive_time;
  static inline bool watchdog_enable = false;

  static void init();
  static void onReceive(serialReceiverLayer::rcChannels_t *rcChannels);
  static void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics);
  static void handle();
};