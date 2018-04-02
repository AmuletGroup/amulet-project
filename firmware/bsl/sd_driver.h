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