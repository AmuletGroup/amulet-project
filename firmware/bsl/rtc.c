/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

// tianlong(refactor)
// 5/3/2016

#include <msp430.h>
#include <msp430fr5989.h>
#include "rtc.h"
#include "inc/hw_memmap.h"
#include "driverlibHeaders.h"

void pause_rtc() {
  // Enable RTC(real time clock)
  RTC_C_disableInterrupt(__MSP430_BASEADDRESS_RTC_C__,
                         RTC_C_TIME_EVENT_INTERRUPT);
  RTC_C_holdClock(__MSP430_BASEADDRESS_RTC_C__);
}

void resume_rtc() {
  RTC_C_enableInterrupt(__MSP430_BASEADDRESS_RTC_C__,
                        RTC_C_TIME_EVENT_INTERRUPT);
  RTC_C_setCalendarEvent(__MSP430_BASEADDRESS_RTC_C__,
                         RTC_C_CALENDAREVENT_MINUTECHANGE);
  RTC_C_startClock(__MSP430_BASEADDRESS_RTC_C__);
}