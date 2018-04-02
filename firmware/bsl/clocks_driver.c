/*
 * clocks_driver.c
 * written by: Gunnar Pope
 * gunnarpope@gmail.com
 * 6/26/2015
 * Modified by Josiah Hester
 *
 * The clocks driver uses the following settings
 *
 * MCLK 	= 8  Mhz (default setting)	//used for MPU processing
 * SMCLK 	= 8 MHz						//used for peripherals that require precise timing (SPI, UART, etc)
 * ACLK  	= 32 kHz					//used to keep time during low-power mode
 */

#include "clocks_driver.h"
#include "msp430fr5989.h"
#include "bsp_init.h"

// ACLK=LFXT1/1=32,768Hz; SMCLK=DCOCLK/1=8MHz; and MCLK=DCOCLK/1=8MHz.msp
void clocks_init(void) {
  // enable the PJ.4 and PJ.5 registers to enable the LFXIN/LFXOUT
  PJREN &= ~(BIT4 | BIT5);
  PJSEL0 |= BIT4 | BIT5;  // For XT1
  PJSEL1 &= ~(BIT5 | BIT5);

  CSCTL0_H = CSKEY >> 8;  // Unlock CS registers
  CSCTL1 = DCOFSEL_6;     // Set DCO to 8MHz
  CSCTL2 =
      SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set SMCLK = MCLK = DCO
                                                    // ACLK = LXFT
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;             // Set all dividers to 1
  CSCTL4 &= ~LFXTOFF;
  do {
    CSCTL5 &= ~LFXTOFFG;  // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);  // Test oscillator fault flag

  // clear the cskey, lock cs registers
  CSCTL0_H = 0;
}
