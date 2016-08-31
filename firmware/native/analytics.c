/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#include "analytics.h"


// Crash logs, in data section, this is init to all zeros on program load
uint16_t __analytics_crash_counter __attribute__ ((section (".upper.data")));

// Battery logs
// noinit section could be any value before use, so use counters to only write data
uint16_t __analytics_batt_log_counter __attribute__ ((section (".noinit")));
uint32_t __analytics_batt_adc[FRAM_LOG_LENGTH] __attribute__ ((section (".noinit")));

// BLE connect / disconnect logs
uint16_t __analytics_ble_counter __attribute__ ((section (".noinit")));
uint32_t __analytics_ble_disconnects[FRAM_LOG_LENGTH] __attribute__ ((section (".noinit")));
bool is_ble_paired = false;

// User interactions
uint16_t __analytics_ui_counter __attribute__ ((section (".noinit")));
uint32_t __analytics_user_interacts[FRAM_LOG_LENGTH]  __attribute__ ((section (".noinit")));

void init_analytic_logs() {
  __analytics_batt_log_counter = 0;
  __analytics_ble_counter = 0;
  __analytics_ui_counter = 0;
  most_recent_ble_events = 0;
  most_recent_ui_events = 0;
}

void increment_crash_counter() { __analytics_crash_counter++; }

void log_ble_event(uint16_t event_type) {
  uint32_t minutes = RTCMIN;
  uint32_t hours = RTCHOUR;
  __analytics_batt_adc[__analytics_ble_counter] =
      (hours << 24) + (minutes << 16) + event_type;
  __analytics_ble_counter = (__analytics_ble_counter + 1) % FRAM_LOG_LENGTH;
  most_recent_ble_events++;
  if (event_type == BLE_EVENT_DISCONNECT) {
    is_ble_paired = false;
  } else {
    is_ble_paired = true;
  }
}

void log_batt_voltage(uint16_t batt_adc) {
  // Put the HOUR, and MINUTE from RTC, as well as the batt ADC value
  uint32_t minutes = RTCMIN;
  uint32_t hours = RTCHOUR;
  __analytics_batt_adc[__analytics_batt_log_counter] =
      (hours << 24) + (minutes << 16) + batt_adc;
  __analytics_batt_log_counter =
      (__analytics_batt_log_counter + 1) % FRAM_LOG_LENGTH;
}

void log_user_interaction(uint16_t event_type) {
  uint32_t minutes = RTCMIN;
  uint32_t hours = RTCHOUR;
  __analytics_batt_adc[__analytics_ui_counter] =
      (hours << 24) + (minutes << 16) + event_type;
  __analytics_ui_counter = (__analytics_ui_counter + 1) % FRAM_LOG_LENGTH;
  // Increment one off counter
  most_recent_ui_events++;
}

bool is_ble_peripheral_paired() { return is_ble_paired; }
