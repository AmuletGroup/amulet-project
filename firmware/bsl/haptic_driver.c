/*
 *timer_A2.c
 *
 * written by Gunnar Pope 02/05/2015
 * charles.g.pope.th@dartmouth.edu
 *
 * This timer is used by button1 for debouncing and for
 * generating button tap and hold events
 *
 * The associated interrupt vector is
 *
 */

#include <msp430.h>
#include <msp430fr5989.h>
#include <stdint.h>

#include "haptic_driver.h"
#include "bsp_init.h"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#include <intrinsics.h>
#elif defined(__GNUC__)
#include <in430.h>
#endif

static uint8_t buzzCount = 0;
static uint8_t cycleCount = 0;

void haptic_init() {
  // set up timer, but don't enable it with MC field
  // clock is enabled in button interrupt

  PM5CTL0 &=
      ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode
  haptic_PxDIR |= haptic_BITx;  // set the pin as an output
  haptic_PxOUT &= ~haptic_BITx;  // initialize the pin as low

  TA3CTL &= ~0x30;  // disable the timer
  TA3CTL |= MC_0;

  // clock source is smclk @ 16Mhz, divisor = 8, enable interrupts,clear
  // register on enterance
  TA3CTL = (TASSEL_1 | ID_3 | TAIE | TACLR);
  // set the haptic buzz period to 10ms. (0x4E20/(16Mhz/8))=0.1 sec
  TA3CCR0 = 0x420;  // set clock delay
}

void timerA3_disable() {
  TA3CTL &= ~0x30;  // disable the timer
  TA3CTL |= MC_0;
}

void haptic() {
  cycleCount++;

  switch (buzzCount) {
    case 1:
      if (cycleCount == 1) {
        haptic_PxOUT &= ~haptic_BITx;
        timerA3_disable();
        cycleCount = 0;
      }
      break;
    case 2:
      if (cycleCount == 3) {
        haptic_PxOUT &= ~haptic_BITx;
        timerA3_disable();
        cycleCount = 0;
      } else {
        haptic_PxOUT ^= haptic_BITx;
      }
      break;
    default:
      break;

  }  // end switch
}  // end haptic

void singleBuzz() {
  buzzCount = 1;
  haptic_PxOUT |= haptic_BITx;
  TA3CTL |= MC_1;  // start counting up
}
void doubleBuzz() {
  buzzCount = 2;
  haptic_PxOUT |= haptic_BITx;
  TA3CTL |= MC_1;  // start counting up
}
