/*****************************************************************************
* Product: PELICAN crossing example
* Last updated for version 5.3.0
* Last updated on  2014-04-18
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
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

extern int16_t gyro_buffer_x[BSP_TICKS_PER_SEC];
extern int16_t gyro_buffer_y[BSP_TICKS_PER_SEC];
extern int16_t gyro_buffer_z[BSP_TICKS_PER_SEC];
extern int8_t gyro_ref_counter;

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

