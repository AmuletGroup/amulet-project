/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

 
#include "sd_driver.h"
#include "msp430fr5989.h"
#include "bsp_init.h"
#include "qp_port.h"

void sd_card_power_on() {
  // Set the power pin LOW to turn the mosfet on
  SD_POWER_DIR |= SD_POWER;
  SD_POWER_OUT &= ~SD_POWER;
}

void sd_card_power_off() {
  // Set the power pin HIGH to turn the mosfet off
  SD_POWER_DIR |= SD_POWER;
  SD_POWER_OUT |= SD_POWER;
}

void SDinit(void) {
  SD_CS_SEL &= ~SD_CS;
  SD_CS_DIR |= SD_CS;
  SD_CS_OUT &= ~SD_CS;

  sd_card_power_off();

  // Port initialization for SD Card operation
  SPI_SEL |= SPI_CLK | SPI_SOMI | SPI_SIMO;
  SPI_DIR |= SPI_CLK | SPI_SIMO;
  SD_CS_SEL &= ~SD_CS;
  SD_CS_OUT |= SD_CS;
  SD_CS_DIR |= SD_CS;
}

// ** These were moved from core_storage.c, since they do not belong there.
//    (Steven Hearndon 9/16/2016)
void sd_enable() {
  QF_INT_DISABLE();
  __no_operation();
  SD_CS_OUT &= ~SD_CS;
  sd_card_power_on();
  __delay_cycles(10);
}

void sd_disable() {
  // Turn off SD card
  sd_card_power_off();
  SD_CS_OUT |= SD_CS;

  // UCxxCTLW0 = UCSWRST | UCSSEL_2;
  // UCxxCTLW0 |= UCCKPH | UCMSB | UCSYNC | UCMST;
  // UCxxBR0 = 2 & 0xFF;
  // UCxxBR1 = (2 >> 8 ) & 0xFF;
  // //UCB0CTLW0 &= ~UCCKPL;
  // UCxxCTLW0 &= ~UCSWRST;
  QF_INT_ENABLE();
  __no_operation();
}