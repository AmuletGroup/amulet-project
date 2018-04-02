/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 * buttons_driver.h
 *
 * written by Gunnar Pope 02/05/2015
 * charles.g.pope.th@dartmouth.edu
 *
 * This file is used to configure the GPIO pins used for the buttons
 * If you #define DEVBOARD_BUTTONS (~line 45), this will enable
 * the two push buttons on the dev boards and light up the green LED
 * with each button press. Holding button for ~1 sec will turn off
 * the light and generate a button hold signal.
 *
 * If #define AMULET_BRACELET is defined, the button ports will
 * be configured for P4.2 and P4.3 on Joe's Amulet App boards
 *
 */
#ifndef buttons_driver_h
#define buttons_driver_h

#include "core_ui.h"

/*
 * Button related pin definitions (see: bsp_devboard_init.h &
 * bsp_bracelet_init.h).
 */

typedef enum { waiting, button_pressed, button_disabled } state_t;

void checkIfDebounced(uint8_t button);
void button_set_state(uint8_t button, state_t state);
void button_init(uint8_t button);
void button_set_state(uint8_t button, state_t state);
void debounce(uint8_t button);
void generateButtonHoldSig(uint8_t button);
void generateButtonTapSig(uint8_t button);
void generateBothHoldSig();

uint8_t is_captouch_enabled();
void turnOnCapTouch();
void turnOffCapTouch();

state_t getState(uint8_t button);

#endif /* buttons_driver_h */
