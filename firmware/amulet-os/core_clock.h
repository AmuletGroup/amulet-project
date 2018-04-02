/*
 * core_clock.h
 *
 *  Created on: 7/5/2016
 *      Author: Steven Hearndon
 */

#ifndef CORE_CLOCK_H_
#define CORE_CLOCK_H_

#include "amulet.h"

/**
 * Seconds since the amulet started up
 */
uint32_t CoreSeconds(uint8_t requestor);

/**
 * Get current date and time all the way from the BSP layer.
 */
void CoreClock(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour,
               uint8_t *min, uint8_t *sec, uint8_t *weekday);

/**
 * Set current date and time all the way to the BSP layer.
 */
void CoreSetClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                  uint8_t min, uint8_t sec, uint8_t weekday);

/**
 * Binary Timestamp
 *
 * Sets the passed timestamp char array to a binary timestamp.
 * Each byte represents a value, in the format YMDhms
 */
void CoreTimeStamp(char *timestamp);

/**
 * Filename Timestamp
 *
 * Sets the passed timestamp char array to a condensed but
 * readable timestamp. Each char represents a value for YMDh,
 * with values 1 to 9 then A+ for 10+. Minutes and Seconds
 * still use two chars for two digits
 */
void CoreTimeStampFile(char *timestamp);

/**
 * Human readable Timestamp
 *
 * Sets the passed timestamp char array to a readable timestamp
 * in the format YYYY-MM-DD hh:mm:ss
 */
void CoreTimeStampText(char *timestamp);

#endif /* CORE_CLOCK_H_ */
