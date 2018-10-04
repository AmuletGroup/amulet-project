/*
 * core_actuators.h
 *
 *  Created on: Oct 11, 2013
 *      Author: Andres
 */

#ifndef CORE_ACTUATORS_H_
#define CORE_ACTUATORS_H_

#include <stdint.h>

//Internal Actuators

#ifndef TIVA_BOARD
#define ACTLED1      0x01
#define ACTLED2      0x02
#define ACTLED3      0x04
#else
#define ACTLED1      0x02
#define ACTLED2      0x04
#define ACTLED3      0x08
#endif

/**
 * Action: Turn LED with led_id ON
Response event:none
 *
 */
void LedOn(uint8_t led_id);

/**
 * Action: Turn LED with led_id OFF
Response event:none
 */
void LedOff(uint8_t led_id);

void DisplayMessage(uint8_t *message, uint32_t len);


#endif /* CORE_ACTUATORS_H_ */
