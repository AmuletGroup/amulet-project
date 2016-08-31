/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#ifndef AMULET_BOARD_H
#define AMULET_BOARD_H

// Amulet models
#define SNAIL_KITE 	10
#define WHITE_OWL 	11

#ifndef AMULET_DEVICE_NAME
#define AMULET_DEVICE_NAME SNAIL_KITE
#define AMULET_DEVICE_REV 1
#endif


#if AMULET_DEVICE_NAME == SNAIL_KITE

#define UART_RX_PIN 28
#define UART_TX_PIN 29
#define RTS_PIN_NUMBER 30
#define CTS_PIN_NUMBER 2

#define SPIS_SCLK 30
#define SPIS_SIMO 28
#define SPIS_SOMI 29
#define SPIS_CS 2
#define BLE_RDY 4

#endif

#if AMULET_DEVICE_NAME == WHITE_OWL

#define UART_RX_PIN 28
#define UART_TX_PIN 29
#define RTS_PIN_NUMBER 30
#define CTS_PIN_NUMBER 2

#define SPIS_SCLK 30
#define SPIS_SIMO 28
#define SPIS_SOMI 29
#define SPIS_CS 2

#endif


#endif // AMULET_BOARD_H