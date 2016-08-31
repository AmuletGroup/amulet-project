/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 * 2012-04-29 rick@kimballsoftware.com - added msp430 support.
 *
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <msp430.h>
#include <inttypes.h>
#include "spi_430.h"


#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 4

#define LSBFIRST 0
#define MSBFIRST 1

#define HIGH 0x1
#define LOW  0x0
 
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

// SPI data transfer function
inline static uint8_t SPItransfer(uint8_t _data);

// SPI Configuration functions

inline static void SPIbegin(); // Default
inline static void SPIend();

inline static void SPIsetBitOrder(uint8_t);
inline static void SPIsetDataMode(uint8_t);
inline static void SPIsetClockDivider(uint8_t);

uint8_t SPItransfer(uint8_t _data) {
    return spi_send(_data);
}

void SPIbegin()
{
    spi_initialize();
}

void SPIend()
{
    spi_disable();
}

void SPIsetBitOrder(uint8_t bitOrder)
{
    spi_set_bitorder(bitOrder);
}

void SPIsetDataMode(uint8_t mode)
{
    spi_set_datamode(mode);
}

void SPIsetClockDivider(uint8_t rate)
{
    spi_set_divisor(rate);
}

#endif
