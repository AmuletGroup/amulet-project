/*******************************************************************************/
/* mpr121.h - a library for using the mpr121 capacitive touch sensor with the  */
/*            family of msp430fr(58/59/68/69)xx MCUs.                          */
/*                                                                             */
/* datasheet - https://www.sparkfun.com/datasheets/Components/MPR121.pdf       */
/*                                                                             */
/* Written by Taylor Hardin 09/12/2016                                         */
/* Taylor.A.Hardin.GR@dartmouth.edu                                            */
/*******************************************************************************/
#ifndef MPR121_H
#define MPR121_H


/*******************************************************************************/
/* Included Libraries                                                          */
/*******************************************************************************/
#include "bsp_init.h"
#include "msp430fr59xx_i2c.h"


/*******************************************************************************/
/* MPR121 Register Address Definitions                                         */
/*******************************************************************************/
#define MHD_R	0x2B
#define NHD_R	0x2C
#define	NCL_R 	0x2D
#define	FDL_R	0x2E
#define	MHD_F	0x2F
#define	NHD_F	0x30
#define	NCL_F	0x31
#define	FDL_F	0x32
#define	ELE0_T	0x41
#define	ELE0_R	0x42
#define	ELE1_T	0x43
#define	ELE1_R	0x44
#define	ELE2_T	0x45
#define	ELE2_R	0x46
#define	ELE3_T	0x47
#define	ELE3_R	0x48
#define	ELE4_T	0x49
#define	ELE4_R	0x4A
#define	ELE5_T	0x4B
#define	ELE5_R	0x4C
#define	ELE6_T	0x4D
#define	ELE6_R	0x4E
#define	ELE7_T	0x4F
#define	ELE7_R	0x50
#define	ELE8_T	0x51
#define	ELE8_R	0x52
#define	ELE9_T	0x53
#define	ELE9_R	0x54
#define	ELE10_T	0x55
#define	ELE10_R	0x56
#define	ELE11_T	0x57
#define	ELE11_R	0x58
#define	FIL_CFG	0x5D
#define	ELE_CFG	0x5E
#define GPIO_CTRL0	0x73
#define	GPIO_CTRL1	0x74
#define GPIO_DATA	0x75
#define	GPIO_DIR	0x76
#define	GPIO_EN		0x77
#define	GPIO_SET	0x78
#define	GPIO_CLEAR	0x79
#define	GPIO_TOGGLE	0x7A
#define	ATO_CFG0	0x7B
#define	ATO_CFGU	0x7D
#define	ATO_CFGL	0x7E
#define	ATO_CFGT	0x7F


/*******************************************************************************/
/* Capacitive Pin Touch/Release Thresholds                                     */
/*******************************************************************************/
#define TOU_THRESH	0x0F         // Modify these values to adjust the touch
#define	REL_THRESH	0x0A         // and release sensitivity of the cap pins


/*******************************************************************************/
/* Function Definitions                                                        */
/*******************************************************************************/
void MPR121_Init(uint8_t slave_addr);
void MPR121_Set_Register(uint8_t slave_addr, uint8_t reg_addr, uint8_t value);
void MPR121_Read_Touch_States(uint8_t slave_addr, uint8_t *touch_states);
uint8_t MPR121_Have_Touch_States_Changed();
#endif