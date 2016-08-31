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

void sd_card_power_on() {
  SD_POWER_DIR |= SD_POWER;
  SD_POWER_OUT &= ~SD_POWER;
}

void sd_card_power_off() {
  SD_POWER_DIR |= SD_POWER;
  SD_POWER_OUT |= SD_POWER;
}

void SDinit(void) {
  SD_CS_SEL &= ~SD_CS;
  SD_CS_DIR |= SD_CS;
  SD_CS_OUT &= ~SD_CS;

  SD_POWER_DIR |= SD_POWER;
  SD_POWER_OUT |= SD_POWER;

  // Port initialization for SD Card operation
  SPI_SEL |= SPI_CLK | SPI_SOMI | SPI_SIMO;
  SPI_DIR |= SPI_CLK | SPI_SIMO;
  SD_CS_SEL &= ~SD_CS;
  SD_CS_OUT |= SD_CS;
  SD_CS_DIR |= SD_CS;
}
