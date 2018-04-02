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

void get_rtc_values(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour,
                    uint8_t* min, uint8_t* sec, uint8_t* weekday) {
  *year = RTCYEAR;
  *month = RTCMON;
  *day = RTCDAY;
  *hour = RTCHOUR;
  *min = RTCMIN;
  *sec = RTCSEC;
  *weekday = RTCDOW;
}

void set_rtc_values(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                  uint8_t min, uint8_t sec, uint8_t weekday) {
  pause_rtc();
  RTCYEAR = year;
  RTCMON = month;
  RTCDAY = day;
  RTCHOUR = hour;
  RTCMIN = min;
  RTCSEC = sec;
  RTCDOW = weekday;
  resume_rtc();
}
