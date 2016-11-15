/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#ifndef SDCARD_DRIVER_H
#define SDCARD_DRIVER_H

// Amulet specific SD card configuration in the BSP layer
// Just turns the state of the MOSFET so that power is supplied
// Since the sleep of the SD card can be from 100 microamps to a milliamp
void sd_card_power_on();
void sd_card_power_off();
void SDinit(void);
void sd_enable();
void sd_disable();

#endif