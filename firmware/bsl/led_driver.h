/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

// Gunnar Pope
// 2/28/2015

#ifndef led_driver_h
#define led_driver_h

#include "core_ui.h"

/*
 * LED related pin definitions (see: bsp_devboard_init.h & bsp_bracelet_init.h).
 */

/**
 * LED (1) initialization.
 *
 * @note LEDs initialization is invoked in bsp_setup.c.
 */
void LED1init();

/**
 * LED (2) initialization.
 *
 * @note LEDs initialization is invoked in bsp_setup.c.
 */
void LED2init();

// the leds are turned on/off in the bsp_actuators.c file, with the functions
// below
void turn_LED1_On();
void turn_LED2_On();
void turn_LED1_Off();
void turn_LED2_Off();
void turn_LED_on(uint8_t);
void turn_LED_off(uint8_t);

#endif