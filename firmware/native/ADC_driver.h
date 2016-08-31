/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 * ADC_driver.h
 *
 * written by gunnar pope 4/11/2015
 * charles.g.pope.th@dartmouth.edu
 * modified by Steven Hearndon 3/9/2016
 * shearnd@clemson.edu
 *
 */

#ifndef ADC_driver_h
#define ADC_driver_h

#include <msp430fr5989.h>
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#include <intrinsics.h>
#elif defined(__GNUC__)
#include <in430.h>
#endif
#include <stdint.h>
#include "bsp_init.h"

void adc_init();
void enable_ADC();
void disable_ADC();
uint16_t read_ADC(uint32_t input_channel);
#endif
