/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#ifndef ANALYTICS_H_
#define ANALYTICS_H_

/*
 * These functions provide a form of application and system level analytics for Amulet.
 * They store all information in high FRAM, and have a static allocation designated for this purpose.
 * Use Cases:
 * 		- keeping track of crashes (frequency, what app caused it, what state),
 * 		- logging battery voltage over time
 * 		- Logging user interaction, i.e. classical analytics (ala flurry)
 * 
 * IN the future it would be good to abstract this to support any type of counter, or log, as defined
 * by the programmer or a configuration file. 
 * View this as a working template for creating this functionality.
 *
 * @author Josiah Hester
 * @created 2/1/16
 */

#include <stdint.h>
#include <msp430.h>
#include <stdbool.h>
#define FRAM_LOG_LENGTH 64

/**
 * BLE event types to log
 */
typedef enum analyticsevents {
  BLE_EVENT_DISCONNECT = 0x00,
  BLE_EVENT_CONNECT = 0x01,
  UI_EVENT_BUTTON0_TAP = 0x02,
  UI_EVENT_BUTTON0_HOLD = 0x03,
  UI_EVENT_BUTTON1_TAP = 0x04,
  UI_EVENT_BUTTON1_HOLD = 0x05,
  UI_EVENT_BOTH_HOLD = 0x06,
  UI_EVENT_CAPTOUCH = 0x07
} AnalyticsEvent;

// Init the analytics logs
void init_analytic_logs();

//	"Logs" a crash to a counter stored in FRAM
void increment_crash_counter();

// Call this every time BLE disconnected or connected
void log_ble_event(uint16_t event_type);

// Logs battery voltage adnt the hour (0-23), and minute (0-59)
// RTC must be enabled in Calendar Mode With Hexadecimal Format for the
// timestamp to work
// Actually logs the ADC value for a 2.0V reference on a 1/2 voltage divider
// from battery.
void log_batt_voltage(uint16_t batt_adc);

// Log user interactison like button presses and CT touches
void log_user_interaction(uint16_t event_type);

// Gets the number of BLE connections / disconnections since this function was
// called last
uint16_t most_recent_ble_events;

// Gets the number of UI events since this function was called last
uint16_t most_recent_ui_events;

bool is_ble_peripheral_paired();

#endif  // ANALYTICS_H_
