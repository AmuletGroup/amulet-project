/**
 * File: eusci_spi.c - msp430 USCI SPI implementation
 *
 * EUSCI flavor implementation by Robert Wessels <robertinant@yahoo.com>
 * Copyright (c) 2012 by Rick Kimball <rick@kimballsoftware.com>
 * spi abstraction api for msp430
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "spi_430.h"
#include "bsp_init.h"

#ifdef __MSP430_HAS_EUSCI_B0__

/**
 * USCI flags for various the SPI MODEs
 *
 * Note: The msp430 UCCKPL tracks the CPOL value. However,
 * the UCCKPH flag is inverted when compared to the CPHA
 * value described in Motorola documentation.
 */

#define SPI_MODE_0 (UCCKPH)		/* CPOL=0 CPHA=0 */
#define SPI_MODE_1 (0)			/* CPOL=0 CPHA=1 */
#define SPI_MODE_2 (UCCKPL | UCCKPH)	/* CPOL=1 CPHA=0 */
#define SPI_MODE_3 (UCCKPL)		/* CPOL=1 CPHA=1 */

#define SPI_MODE_MASK (UCCKPL | UCCKPH)

/**
 * spi_initialize() - Configure USCI UCB0 for SPI mode
 *
 * P2.7 - CS (active low)
 * P1.4 - SCLK
 * P1.6 - MOSI aka SIMO
 * P1.7 - MISO aka SOMI
 *
 */
void spi_initialize(void)
{
	/* Put USCI in reset mode, source USCI clock from SMCLK. */
	UCxxCTLW0_ACCEL = UCSWRST | UCSSEL_2;

	/* SPI in master MODE 0 - CPOL=0 SPHA=0. */
	UCxxCTLW0_ACCEL |= SPI_MODE_0 | UCMSB | UCSYNC | UCMST;

	/* Set pins to SPI mode. */
	ACCEL_SPI_CLK_SEL0 |= ACCEL_SPI_CLK;
	ACCEL_SPI_SEL0 |= ACCEL_SPI_SIMO;
	ACCEL_SPI_SEL0 |= ACCEL_SPI_SOMI;
	ACCEL_SPI_CLK_SEL1 &= ~ACCEL_SPI_CLK;
	ACCEL_SPI_SEL1 &= ~ACCEL_SPI_SIMO;
	ACCEL_SPI_SEL1 &= ~ACCEL_SPI_SOMI;
	//pinMode_int(SCK, SPISCK_SET_MODE);
	//pinMode_int(MOSI, SPIMOSI_SET_MODE);
	//pinMode_int(MISO, SPIMISO_SET_MODE);


	// Set initial speed for SPI comms
        // Recommended range for SPI clock on ADX362 is 1MHz to 8MHz.
        // Since our system clock is 16MHz, 4MHz SPI is probably the max.
        // System clock is 16MHz so:
        //   SPI_CLOCK_DIV4 would be 4MHz
        //   SPI_CLOCK_DIV8 would be 2MHz
        //   SPI_CLOCK_DIV16 would be 1MHz
        //   SPI_CLOCK_DIV32 would be 500KHz
        //   SPI_CLOCK_DIV64 would be 250KHz
        //   SPI_CLOCK_DIV128 would be 125KHz

        // Uncomment for 4MHz SPI clock
	//UCB0BR0 = SPI_CLOCK_DIV4 & 0xFF;
	//UCB0BR1 = (SPI_CLOCK_DIV4 >> 8 ) & 0xFF;

        // Uncomment for 1MHz SPI clock
	UCxxBR0_ACCEL = SPI_CLOCK_DIV2 & 0xFF;
	UCxxBR1_ACCEL = (SPI_CLOCK_DIV2 >> 8 ) & 0xFF;

	/* Release USCI for operation. */
	UCxxCTLW0_ACCEL &= ~UCSWRST;
}

/**
 * spi_disable() - put USCI into reset mode.
 */
void spi_disable(void)
{
	/* Put USCI in reset mode. */
	UCxxCTLW0_ACCEL |= UCSWRST;
}

/**
 * spi_send() - send a byte and recv response.
 */
uint8_t spi_send(const uint8_t _data)
{
	/* Wait for previous tx to complete. */
	while (!(UCxxIFG_ACCEL & UCTXIFG))
		;

	/* Setting TXBUF clears the TXIFG flag. */
	UCxxTXBUF_ACCEL = _data;

	/* Wait for a rx character? */
	while (!(UCxxIFG_ACCEL & UCRXIFG))
		;

	/* Reading clears RXIFG flag. */
	return UCxxRXBUF_ACCEL;
}

/***SPI_MODE_0
 * spi_set_divisor() - set new clock divider for USCI.
 *
 * USCI speed is based on the SMCLK divided by BR0 and BR1.
 *
 */
void spi_set_divisor(const uint16_t clkdiv)
{
	/* Hold UCB0 in reset. */
	UCxxCTLW0_ACCEL |= UCSWRST;

	UCxxBR0_ACCEL = clkdiv & 0xFF;
	UCxxBR1_ACCEL = (clkdiv >> 8 ) & 0xFF;

	/* Release for operation. */
	UCxxCTLW0_ACCEL &= ~UCSWRST;
}

/**
 * spi_set_bitorder(LSBFIRST=0 | MSBFIRST=1).
 */
void spi_set_bitorder(const uint8_t order)
{
	/* Hold UCB0 in reset. */
	UCxxCTLW0_ACCEL |= UCSWRST;

	UCxxCTLW0_ACCEL = (UCxxCTLW0_ACCEL & ~UCMSB) | ((order == 1 /*MSBFIRST*/) ? UCMSB : 0); /* MSBFIRST = 1 */

	/* Release for operation. */
	UCxxCTLW0_ACCEL &= ~UCSWRST;
}

/**
 * spi_set_datamode() - mode 0 - 3.
 */
void spi_set_datamode(const uint8_t mode)
{
	/* Hold UCB0 in reset. */
	UCxxCTL1_ACCEL |= UCSWRST;
	switch(mode) {
	case 0: /* SPI_MODE0 */
		UCxxCTLW0_ACCEL = (UCxxCTLW0_ACCEL & ~SPI_MODE_MASK) | SPI_MODE_0;
		break;
	case 1: /* SPI_MODE1 */
		UCxxCTLW0_ACCEL = (UCxxCTLW0_ACCEL & ~SPI_MODE_MASK) | SPI_MODE_1;
		break;
	case 2: /* SPI_MODE2 */
		UCxxCTLW0_ACCEL = (UCxxCTLW0_ACCEL & ~SPI_MODE_MASK) | SPI_MODE_2;
		break;
	case 4: /* SPI_MODE3 */
		UCxxCTLW0_ACCEL = (UCxxCTLW0_ACCEL & ~SPI_MODE_MASK) | SPI_MODE_3;
		break;
	default:
		break;
	}

	/* Release for operation. */
	UCxxCTL1_ACCEL &= ~UCSWRST;
}
#endif
