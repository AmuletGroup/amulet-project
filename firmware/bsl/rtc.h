// tianlong(refactor)
// 5/3/2016

#ifndef rtc_driver_h
#define rtc_driver_h

#include <stdint.h>

void pause_rtc();

void resume_rtc();

void get_rtc_values(uint16_t* year, uint8_t* month, uint8_t* day,
                    uint8_t* hour, uint8_t* min, uint8_t* sec,
                    uint8_t* weekday);

void set_rtc_values(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                  uint8_t min, uint8_t sec, uint8_t weekday);

#endif
