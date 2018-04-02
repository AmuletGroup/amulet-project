/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#ifndef bsp_h
#define bsp_h

#include <stdint.h>
#include <msp430.h>

#include "bsp_sensors.h"
#include "SharpDisplay.h"

//uint8_t status = 17;
#define BSP_DEBUG

/* system clock ticks per second ...........................................*/
#define BSP_TICKS_PER_SEC    20U

extern int16_t acc_buffer_x[BSP_TICKS_PER_SEC];
extern int16_t acc_buffer_y[BSP_TICKS_PER_SEC];
extern int16_t acc_buffer_z[BSP_TICKS_PER_SEC];
//extern int16_t acc_buffer[120];
extern int8_t acc_ref_counter;
extern int8_t ble_ref_counter;

/**
 * This is where everything being set up, you can enable/disable individual function 
 * to fit you need
 *
 */
void BSP_init(void);

void BSP_terminate(int16_t result);
int setupIPC();

void toggle_on();
void toggle_off();

void toggle2_on();

void toggle2_off();

#endif /* bsp_h */

