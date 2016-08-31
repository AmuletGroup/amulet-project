/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 *timer_A1.c
 *
 * written by Gunnar Pope 02/05/2015
 * charles.g.pope.th@dartmouth.edu
 *
 */

#include <msp430.h>
#include <msp430fr5989.h>
#include <stdint.h>

#include "buttons_driver.h"
#include "timer_A1.h"
#include "bsp_init.h"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#include <intrinsics.h>
#elif defined(__GNUC__)
#include <in430.h>
#endif

extern void button_set_state(uint8_t button, state_t state);
extern void generateButtonHoldSig(uint8_t button);

static uint16_t count;

void timerA1_resetCount() { count = 0; }

void timerA1_init() {
  // set up timer, but don't enable it with MC field
  // clock is enabled in button interrupt

  TA1CTL &= ~0x30;  // disable the timer
  TA1CTL |= MC_0;

  // clock source is smclk @ 16Mhz, divisor = 8, enable interrupts,clear
  // register on enterance
  TA1CTL = (TASSEL_2 | ID_3 | TAIE | TACLR);

  TA1CCR0 = DELAY5ms * 3;  // set clock delay
  TA1CTL |= MC_1;  // start counting up
}

void timerA1_addCount() { count++; }

uint16_t timerA1_getCount() { return (count); }

void timerA1_disable() {
  TA1CTL &= ~0x30;  // disable the timer
  TA1CTL |= MC_0;
}

//ADDED BY GUNNAR 2/14/2015
//used to debounce button1
//ISR initialized in timer_A1.c
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TimerA1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TimerA1 (void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif

  //upon entrance, initial debounce timeframe has passed
    if (TA1IV & TA1IV_TACCR1){

    switch(getState(BUTTON0)){
    case waiting:
      //this one turns on the led
      checkIfDebounced(BUTTON0);
      break;
    case button_pressed:
      timerA1_addCount();
      break;

    default:
      break;
    }//end switch
   }//end if
}//end interrupt
