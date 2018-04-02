/*******************************************************************************/
/* mpr121.c - a library for using the mpr121 capacitive touch sensor with the  */
/*            family of msp430fr(58/59/68/69)xx MCUs.                          */
/*                                                                             */
/* datasheet - https://www.sparkfun.com/datasheets/Components/MPR121.pdf       */
/*                                                                             */
/* Written by Taylor Hardin 09/12/2016                                         */
/* Taylor.A.Hardin.GR@dartmouth.edu                                            */
/*******************************************************************************/
#include "MPR121.h"


/*******************************************************************************/
/* MPR121_Init - Initializes filtering levels and sets all 12 pins to          */
/*               capacitive touch mode. The capacitive touch sensitivity can   */
/*               be adjusted by modifying the TOU_THRESH and REL_THRESH vars.  */
/*               These macros can be found in MPR121.h. NOTE: This library     */
/*               does NOT initialize the I2C library. You must do so outside.  */
/*******************************************************************************/
void MPR121_Init(uint8_t slave_addr) {
    /* 1. Initialize all of the registers for the mpr121 chip. */
    MPR121_Set_Register(slave_addr, ELE_CFG, 0x00); 

    /* 2. Section A - Controls filtering when data is > baseline. */
    MPR121_Set_Register(slave_addr, MHD_R, 0x01);
    MPR121_Set_Register(slave_addr, NHD_R, 0x01);
    MPR121_Set_Register(slave_addr, NCL_R, 0x00);
    MPR121_Set_Register(slave_addr, FDL_R, 0x00);

    /* 3. Section B - Controls filtering when data is < baseline. */
    MPR121_Set_Register(slave_addr, MHD_F, 0x01);
    MPR121_Set_Register(slave_addr, NHD_F, 0x01);
    MPR121_Set_Register(slave_addr, NCL_F, 0xFF);
    MPR121_Set_Register(slave_addr, FDL_F, 0x02);

    /* 4. Section C - Sets touch and release thresholds for each electrode */
    MPR121_Set_Register(slave_addr, ELE0_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE0_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE1_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE1_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE2_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE2_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE3_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE3_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE4_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE4_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE5_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE5_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE6_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE6_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE7_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE7_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE8_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE8_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE9_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE9_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE10_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE10_R, REL_THRESH);

    MPR121_Set_Register(slave_addr, ELE11_T, TOU_THRESH);
    MPR121_Set_Register(slave_addr, ELE11_R, REL_THRESH);

    /* 5. Section D Set the Filter Configuration */
    MPR121_Set_Register(slave_addr, FIL_CFG, 0x04);

    /* 6. Section E Electrode Configuration. Set ELE_CFG to 0x00 *
     *    to return to standby mode.                             */
    MPR121_Set_Register(slave_addr, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
}


/*******************************************************************************/
/* MPR121_Set_Register - Uses functions of msp430fr59xx_i2c library to set the */
/*                       values of a specified mpr121 register.                */
/*******************************************************************************/
void MPR121_Set_Register(uint8_t slave_addr, uint8_t reg_addr, uint8_t value) {
    uint8_t data[2];
    data[0] = reg_addr;
    data[1] = value;
	I2C_Write(slave_addr, data, 2);
}


/*******************************************************************************/
/* MPR121_Read_Touch_States - Uses functions provided by msp430fr59xx_i2c      */
/*                            library to read the states of the capacitive     */
/*                            touch pins. There are 12 capacitive touch pins   */
/*                            and their statuses are located at 0x00-0x01.     */ 
/*                            0x00 = E7 E6 E5 E4 E3 E2 E1 E0                   */
/*                            0x01 = xx xx xx xx EB EA E9 E8                   */
/*                            The mpr121 automatically increments the read     */
/*                            read address, so we can send 0x00 and read twice */
/*                            to get bytes 0x00 and 0x01.                      */
/*******************************************************************************/
void MPR121_Read_Touch_States(uint8_t slave_addr, uint8_t *touch_states) {
	if(MPR121_Have_Touch_States_Changed()) {
		I2C_Read(slave_addr, 0x00, touch_states, 2);   // MAKE SURE touch_states size >= 2
	}
}


/*******************************************************************************/
/* MPR121_Have_Touch_States_Changed - The IRQ pin has a pull-up res because it */
/*                                    is active low. If low, return true.      */
/*******************************************************************************/
uint8_t MPR121_Have_Touch_States_Changed() {
	if(!(MPR121_IRQ_PxIN & MPR121_IRQ)) {
		return 1;
	} else {
		return 0;
	}
}