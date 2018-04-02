// Ron Peterson
// 3/1/2016

#ifndef l3gd20h_h
#define l3gd20h_h

#include "core_ui.h"
#include "bsp_init.h" // Bracelet or dev kit pin definitions

/*
 * Gyro related pin definitions (see: bsp_devboard_init.h & bsp_bracelet_init.h).
 */


/**
 * LED (1) initialization. 
 *
 * @note Gyro initialization is invoked in bsp_setup.c.
 */
void gyroInit();

// Used to power/unpower the gyro
void turn_gyro_On();
void turn_gyro_Off();

#endif
