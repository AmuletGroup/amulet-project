#include <msp430fr5989.h>
#include <stdint.h>
#include <stdbool.h>
#include "nrf51822.h"
#include "bsp_init.h"

#define SPI_CLOCK_DIV4 4
bool is_bluetooth_enabled = false;

bool is_ble_enabled() { return is_bluetooth_enabled; }

void nrf51822_turn_on() {
  IPC_CS_PxDIR |= IPC_CS_BITx;
  IPC_CS_PxOUT |= IPC_CS_BITx;
  IPC_SPI_DIR |= IPC_SPI_CLK + IPC_SPI_SOMI + IPC_SPI_SIMO;
  IPC_SPI_OUT |= IPC_SPI_CLK + IPC_SPI_SOMI + IPC_SPI_SIMO;
  __delay_cycles(1000000);
  // BLE turn on
  IPC_PWR_PxDIR |= IPC_PWR_BITx;
  IPC_PWR_PxOUT &= ~IPC_PWR_BITx;
  __delay_cycles(100000);
  is_bluetooth_enabled = true;
}

void nrf51822_turn_off() {
  // All connected pins btw msp430 and nrf51822 (SPI pins) to input and pulldown
  IPC_CS_PxDIR &= ~IPC_CS_BITx;
  IPC_CS_PxOUT &= ~IPC_CS_BITx;
  IPC_SPI_REN |= IPC_CS_BITx;

  IPC_SPI_DIR &= ~(IPC_SPI_CLK | IPC_SPI_SOMI | IPC_SPI_SIMO);
  IPC_SPI_OUT &= ~(IPC_SPI_CLK | IPC_SPI_SOMI | IPC_SPI_SIMO);
  P2REN |= (IPC_SPI_CLK | IPC_SPI_SOMI | IPC_SPI_SIMO);

  // Close power FET
  IPC_PWR_PxDIR |= IPC_PWR_BITx;
  IPC_PWR_PxOUT |= IPC_PWR_BITx;
  is_bluetooth_enabled = false;
}

/**
 * spi_initialize() - Configure USCI UCB0 for SPI mode
 *
 * P3.7 - CS (active low)
 * P2.2 - SCLK
 * P2.1 - MISO aka SOMI
 * P2.0 - MOSI aka SIMO
 *
 */
void nrf51822_init_spi() {
  /* Put USCI in reset mode, source USCI clock from SMCLK. */
  UCxxCTLW0_IPC = UCSWRST;
  UCxxCTLW0_IPC |= UCSSEL_2;

  /* SPI in master MODE 0 - CPOL=0 SPHA=0. */
  UCxxCTLW0_IPC |= UCCKPH | UCSYNC | UCMST | UCMSB;

  /* Set pins to SPI mode. */
  IPC_SPI_SEL0 |= IPC_SPI_CLK;
  IPC_SPI_SEL0 |= IPC_SPI_SOMI;
  IPC_SPI_SEL0 |= IPC_SPI_SIMO;
  IPC_SPI_SEL1 &= ~IPC_SPI_CLK;
  IPC_SPI_SEL1 &= ~IPC_SPI_SOMI;
  IPC_SPI_SEL1 &= ~IPC_SPI_SIMO;

  /* Set initial speed to 2MHz which is the max */
  // UCA0BRW = 1024;
  UCxxBR0_IPC = SPI_CLOCK_DIV4 & 0xFF;
  UCxxBR1_IPC = (SPI_CLOCK_DIV4 >> 8) & 0xFF;

  /* Release USCI for operation. */
  UCxxCTLW0_IPC &= ~UCSWRST;
  // Settle, and set CS direction
  __delay_cycles(100000);
  IPC_CS_PxDIR |= IPC_CS_BITx;
}

void nrf51822_select() {
  IPC_CS_PxOUT &= ~IPC_CS_BITx;
  __delay_cycles(80);
}

void nrf51822_deselect() {
  __delay_cycles(80);
  IPC_CS_PxOUT |= IPC_CS_BITx;
}

/**
 * spi_send() - send a byte and recv response.
 */
uint8_t nrf51822_spi_transfer(const uint8_t _data) {
  /* Wait for previous tx to complete. */
  while (!(UCxxIFG_IPC & UCTXIFG))
    ;

  /* Setting TXBUF clears the TXIFG flag. */
  UCxxTXBUF_IPC = _data;

  /* Wait for a rx character? */
  while (!(UCxxIFG_IPC & UCRXIFG))
    ;

  /* Reading clears RXIFG flag. */
  return UCxxRXBUF_IPC;
}
