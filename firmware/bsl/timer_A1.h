/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

// Gunnar Pope
// timer_A2.h

#define DELAY20ms 0x9C40  // 20ms delay for debounce
#define DELAY10ms 0x4E20
#define DELAY5ms 0x2710
#define MAXCOUNT 0XF000

// global functions for the timer
void timerA1_init();
void timerA1_disable();
void timerA1_addCount();
void timerA1_resetCount();
uint16_t timerA1_getCount();
