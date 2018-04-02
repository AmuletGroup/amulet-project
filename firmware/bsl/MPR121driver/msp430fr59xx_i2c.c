/*******************************************************************************/
/* msp430fr59xx_i2c.h - a general purpose I2C library for the family of        */
/*                      msp430fr(58/59/68/69)xx MCUs.                          */
/*                                                                             */
/* Written by Taylor Hardin 09/12/2016                                         */
/* Taylor.A.Hardin.GR@dartmouth.edu                                            */
/*******************************************************************************/
#include "msp430fr59xx_i2c.h"


/*******************************************************************************/
/* I2C_Init - Configures UCB0 for I2C Master Sync mode with tx, rx, start, and */
/*            stop interrupts enabled. If you change the interrupts, make sure */
/*            to update the interrupt vector at the end of this document. To   */
/*            choose the clock source, use the UCSSEL_UCLK, UCSSEL__SMCLK,     */
/*            or UCSSEL__ACLK macros for main, sub-main, or auxillary clock.   */
/*******************************************************************************/
void I2C_Init(uint16_t clk_source, uint8_t clk_divider) {
    UCB0CTLW0 |=  UCSWRST;                                  // Software reset enabled
    UCB0CTLW0 |=  UCMODE_3 | UCMST | UCSYNC;                // I2C mode, Master mode, sync
    UCB0CTLW0 |=  clk_source;                               // Use UCSSEL__UCLK, UCSSEL__SMCLK, or UCSSEL__ACLK macros
    UCB0BRW    =  clk_divider;                              // Baudrate = clk_source / clk_divider
    UCB0CTL1  &= ~UCSWRST;                                  // Exit software reset
    UCB0IE    |=  UCTXIE0 | UCRXIE0 | UCSTPIE | UCSTTIE;    // Enable Tx/Rx, start/stop interrupts
}


/*******************************************************************************/
/* I2C_Read - Reads length bytes starting from the specified slaves reg_addr.  */
/*            NOTE: I wrote this library to use with the mpr121, which auto-   */
/*            matically increments the reg_addr after every successive read.   */
/*            I don't know if all devices do this, so you should check to make */
/*            sure that the device is incrementing reg_addr, otherwise you're  */
/*            just going to read from the same address length times.           */
/*******************************************************************************/
void I2C_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t length) {
    /* 1. Configure slave address, set to write mode, and write reg_addr to *
    *     tell the slave what register you want to start reading from.      */
    UCB0CTLW0 |= UCSWRST;                       // Remember, you must enable sofware reset
    UCB0I2CSA  = slave_addr;                    // to reset the slave address
    UCB0CTL1 &= ~UCSWRST;                       // Exit software reset

    while(UCB0STAT & UCBBUSY);                  // Poll while waiting for clear SDA bus
    UCB0CTL1 |= UCTR | UCTXSTT;                 // Set master to write mode and send start condition
    while(!(UCB0IFG & UCTXIFG0));               // Poll while waiting for start ack
    UCB0TXBUF = reg_addr;                       // Transmit the address you want to read from
    while(!(UCB0IFG & UCTXIFG0));               // Poll while waiting for tx ack

    /* 2. Set the master to read mode and send a restart condition. Restart     *
    *     condition is when you sent two starts without a stop inbetween.       *
    *     We sent one start in step one, so the slave treats this as a restart. */
    UCB0CTLW0 &= ~UCTR;                         // Set master to read mode
    UCB0CTL1  |=  UCTXSTT;                      // Send restart condition
    while(UCB0CTL1 & UCTXSTT);


    /* 3. I think, every time you read from the rx buffer the slave will *
     *    automatically send the next byte. For this reason, you want to *
     *    read 1 less than the total number of bytes, and send a stop    *
     *    condition before reading the last byte in the buffer.          */
    uint8_t i = 0;
    for(i = 0; i < length-1; i++) {
        while(!(UCB0IFG & UCRXIFG0));           // Poll while waiting for Rx data
        data[i] = UCB0RXBUF;                    // Get Rx data
    }

    /* 4. Indicate to the slave that we don't want any more data *
     *    after we read this last byte from the rx buffer.       */
    UCB0CTL1 |= UCTXSTP;                        // Send stop condition
    while(!(UCB0IFG & UCRXIFG0));               // Poll while waiting for last Rx data byte
    data[i] = UCB0RXBUF;                        // Get Rx data
    while(UCB0CTL1 & UCTXSTP);                  // Poll whilw waiting for stop ack
}


/*******************************************************************************/
/* I2C_Write - Writes one byte of data to a specified slave's reg. The data    */
/*             array should be of size 2 and contain the reg_addr in data[0]   */
/*             and the value to be written in data[1].                         */
/*******************************************************************************/
void I2C_Write(uint8_t slave_addr, uint8_t *data, uint8_t length) {
    /* 1. Configure slave address, write mode, and start condition. */
    UCB0CTLW0 |= UCSWRST;                       // Remember, you must enable sofware reset
    UCB0I2CSA  = slave_addr;                    // to reset the slave address
    UCB0CTL1 &= ~UCSWRST;                       // Exit software reset
    while(UCB0STAT & UCBBUSY);                  // Poll while waiting for clear SDA bus
    UCB0CTL1 |= UCTR | UCTXSTT;                 // Set master to write mode and send start condition
    while(UCB0CTL1 & UCTXSTT);                  // Poll while waiting for start ack

    /* 2. Write reg_addr and then value to be written to register. */
    uint8_t i;
    for(i = 0; i < length; i++) {
        while(!(UCB0IFG & UCTXIFG0));           // Poll while waiting for clear Tx buf
        UCB0TXBUF = data[i];                    // Send Tx data
    }

    /* 3. Indicate to the slave that we're done writing. */
    while(!(UCB0IFG & UCTXIFG0));               // Poll while waiting for clear Tx buf
    UCB0CTL1 |= UCTXSTP;                        // Send stop condition
    while(UCB0CTL1 & UCTXSTP);                  // Poll while waiting for stop ack
}