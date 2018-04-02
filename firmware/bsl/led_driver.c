/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

// Gunnar Pope
// 2/28/15

#include <msp430.h>
#include <msp430fr5989.h>
#include "led_driver.h"
#include "bsp_init.h"

void LED1init() {
  LED1_PxDIR |= LED1_BITx;
  turn_LED1_Off();
}

void LED2init() {
  LED2_PxDIR |= LED2_BITx;
  turn_LED2_Off();
}

void turn_LED1_On() { LED1_PxOUT |= LED1_BITx; }

void turn_LED2_On() { LED2_PxOUT |= LED2_BITx; }

void turn_LED1_Off() { LED1_PxOUT &= ~LED1_BITx; }

void turn_LED2_Off() { LED2_PxOUT &= ~LED2_BITx; }

void turn_LED_on(uint8_t led) {
  switch (led) {
    case EXTERNAL_GREEN:
      turn_LED1_On();
      break;
    case EXTERNAL_RED:
      turn_LED2_On();
      break;
    default:
      break;
  }
}

void turn_LED_off(uint8_t led) {
  switch (led) {
    case EXTERNAL_GREEN:
      turn_LED1_Off();
      break;
    case EXTERNAL_RED:
      turn_LED2_Off();
      break;
    default:
      break;
  }
}
