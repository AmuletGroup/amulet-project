/*******************************************************************************/
/* msp430fr59xx_i2c.h - a general purpose I2C library for the family of        */
/*                      msp430fr(58/59/68/69)xx MCUs.                          */
/*                                                                             */
/* Written by Taylor Hardin 09/12/2016                                         */
/* Taylor.A.Hardin.GR@dartmouth.edu                                            */
/*******************************************************************************/
#ifndef MSP430FR59XX_I2C_H
#define MSP430FR59XX_I2C_H


/*******************************************************************************/
/* Included Libraries                                                          */
/*******************************************************************************/
#include <stdint.h>
#include "bsp_init.h"


/*******************************************************************************/
/* Function Definitions                                                        */
/*******************************************************************************/
void I2C_Init(uint16_t clk_source, uint8_t clk_divider);
void I2C_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t length);
void I2C_Write(uint8_t slave_addr, uint8_t *data, uint8_t length);
#endif