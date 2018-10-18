/***************************************************
  This is a library for the L3GD20 and L3GD20H GYROSCOPE

  Designed specifically to work with the Adafruit L3GD20(H) Breakout 
  ----> https://www.adafruit.com/products/1032

  These sensors use I2C or SPI to communicate, 2 pins (I2C) 
  or 4 pins (SPI) are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Kevin "KTOWN" Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  ------------------------
  Adapted for Amulet by Josiah Hester (7/26/17)
  ------------------------
  Adapted for LSM6DSL by Taylor Hardin (6/25/18)
 ****************************************************/
#include "gyro.h"
#include "ADXL362driver/spi_430.h"
#include "bsp_init.h" // To configure for bracelet or dev kit
#include "ADXL362driver/util.h"


void __L3GD20_write8(l3gd20Registers_t reg, uint8_t value) {
    GYRO_CS_PxOUT &= ~GYRO_CS_BITx;
    spi_send(reg);
    spi_send(value);
    GYRO_CS_PxOUT |= GYRO_CS_BITx;
}

void __LSM6DSL_write8(lsm6dslRegisters_t reg, uint8_t value) {
    GYRO_CS_PxOUT &= ~GYRO_CS_BITx;
    spi_send(reg);
    spi_send(value);
    GYRO_CS_PxOUT |= GYRO_CS_BITx;
}


uint8_t __L3GD20_read8(l3gd20Registers_t reg) {
    uint8_t value;
    GYRO_CS_PxOUT &= ~GYRO_CS_BITx;
    spi_send((uint8_t)reg | 0x80); // set READ bit
    value = spi_send(0xFF);
    GYRO_CS_PxOUT |= GYRO_CS_BITx;
    return value;
}

uint8_t __LSM6DSL_read8(lsm6dslRegisters_t reg) {
    uint8_t value;
    GYRO_CS_PxOUT &= ~GYRO_CS_BITx;
    spi_send((uint8_t)reg | 0x80); // set READ bit
    value = spi_send(0xFF);
    GYRO_CS_PxOUT |= GYRO_CS_BITx;
    return value;
}


int8_t GyroBegin() {
    /* 1. Configure the Gyro's chip select pin (active low)  *
     *    and set it's output to HIGH so it is not selected. */
    GYRO_CS_PxDIR |= GYRO_CS_BITx;
    GYRO_CS_PxOUT |= GYRO_CS_BITx;

    /* 2. Set the chip address (if using I2C) and data range. */
    #ifdef BSP_SNAIL_KITE
        address = L3GD20_ADDRESS;
    #endif
    #ifdef BSP_SNAIL_KITE_D
        address = LSM6DSL_ADDRESS;
    #endif
    range = L3DS20_RANGE_250DPS;

    /* 3. Grab the gyro chip ID from the WHO_AM_I register. */
    uint8_t id;
    #ifdef BSP_SNAIL_KITE
        id = __L3GD20_read8(L3GD20_REGISTER_WHO_AM_I);
    #endif
    #ifdef BSP_SNAIL_KITE_D
        id = __LSM6DSL_read8(LSM6DSL_REGISTER_WHO_AM_I);
    #endif

    /* 4. If the returned ID does not match one of our supported    *
    *    gyro chips, then exit without further chip configuration. */
    if ((id != L3GD20_ID) && (id != L3GD20H_ID) && (id != LSM6DSL_ID)) {
        return 0;
    }

    /* 5. Put the chip in PD mode. It should only be turned on *
    *    if an application has a subscription to the gyro.    */
    GyroPowerDownMode();

    /* 6. Set the data range for the gyro. Both chips support *
    *    250, 500, and 2000 so I didnt bother making another *
    *    special enum type for the LSM6DSL chip.             */
    switch(range) {
        case L3DS20_RANGE_250DPS:
            /* Kite.c - Bits 5-4 of CTRL_REG4 control dps, where *
             *          00 = 250, 01 = 500, and 10 = 2000.       */
            #ifdef BSP_SNAIL_KITE
                __L3GD20_write8(L3GD20_REGISTER_CTRL_REG4, 0x00);
            #endif

            /* Kite.d - Bits 3-2 of CTRL2_G control dps, where *
             *          00 = 250, 01 = 500, and 11 = 2000.     */
            #ifdef BSP_SNAIL_KITE_D
                __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL2_G, 0x00);
            #endif
            break;

        case L3DS20_RANGE_500DPS:
            #ifdef BSP_SNAIL_KITE
                __L3GD20_write8(L3GD20_REGISTER_CTRL_REG4, 0x10);
            #endif
            #ifdef BSP_SNAIL_KITE_D
                __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL2_G, 0x04);
            #endif
            break;

        case L3DS20_RANGE_2000DPS:
            #ifdef BSP_SNAIL_KITE
                __L3GD20_write8(L3GD20_REGISTER_CTRL_REG4, 0x20);
            #endif
            #ifdef BSP_SNAIL_KITE_D
                __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL2_G, 0x0C);
            #endif
            break;
    }
    return 1;
}

void GyroReadXYZData(l3gd20Data * data) {
    /* 1. Set the gyros CS pin output to LOW to indicate *
     *    that we wish to communicate with it via SPI.   */
    uint8_t xhi, xlo, ylo, yhi, zlo, zhi;
    GYRO_CS_PxOUT &= ~GYRO_CS_BITx;

    /* 2. Kite.c - Send the address we wish to start reading from (gyro X *
     *             output) and set the SPI read and autoincrement bits    *
     *             (0x80, 0x40). Then read the low/high x,y,z registers.  *
     *    Kite.d - Exact same as Kite.c, except we do not need the auto   *
     *             increment bits as that is set in CTRL3_C in GyroBegin. */
    #ifdef BSP_SNAIL_KITE
        spi_send(L3GD20_REGISTER_OUT_X_L | 0x80 | 0x40);
    #endif
    #ifdef BSP_SNAIL_KITE_D
        spi_send(LSM6DSL_REGISTER_OUTX_L_G | 0x80);
    #endif
    delay(10);
    xlo = spi_send(0xFF);
    xhi = spi_send(0xFF);
    ylo = spi_send(0xFF);
    yhi = spi_send(0xFF);
    zlo = spi_send(0xFF);
    zhi = spi_send(0xFF);

    /* 4. Set the CS pin to HIGH to finish SPI communication. */
    GYRO_CS_PxOUT |= GYRO_CS_BITx;

    /* 5. By default, the gyro stores values in little endian order.      *
     *    Combine byte-pairs with the lower one first to build x,y,z vals */
    data->x = (int16_t)(xlo | (xhi << 8));
    data->y = (int16_t)(ylo | (yhi << 8));
    data->z = (int16_t)(zlo | (zhi << 8));

    /* TODO: Is this the same for the newer gyro? */
    /* 6. Compensate values depending on the resolution. */
    switch(range) {
        case L3DS20_RANGE_250DPS:
            data->x *= L3GD20_SENSITIVITY_250DPS;
            data->y *= L3GD20_SENSITIVITY_250DPS;
            data->z *= L3GD20_SENSITIVITY_250DPS;
            break;
        case L3DS20_RANGE_500DPS:
            data->x *= L3GD20_SENSITIVITY_500DPS;
            data->y *= L3GD20_SENSITIVITY_500DPS;
            data->z *= L3GD20_SENSITIVITY_500DPS;
            break;
        case L3DS20_RANGE_2000DPS:
            data->x *= L3GD20_SENSITIVITY_2000DPS;
            data->y *= L3GD20_SENSITIVITY_2000DPS;
            data->z *= L3GD20_SENSITIVITY_2000DPS;
        break;
    }
}

void GyroPowerDownMode() {
    /* Kite.c - Set the output data rate (bits 7-4) to 95Hz, put the chip *
     *          in power down mode (bit 3), and enable the Z, Y, and X    *
     *          channels (bits 2-0).                                      */
    #ifdef BSP_SNAIL_KITE
        __L3GD20_write8(L3GD20_REGISTER_CTRL_REG1, 0x07);
    #endif

    /* Kite.d - The LSM6DSL has an accelerometer and gyro, and both need  *
     *          to be off for full PD mode. Write 0s to bits 7-4 of       *
     *          CTRL1_XL and CTRL2_G to turn off the accel and gyro.      *
     *          Disable high-performance mode by setting bit 4 of CTRL6_C */
    #ifdef BSP_SNAIL_KITE_D
        __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL1_XL, 0x00);
        __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL6_C, 0x10);
        __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL3_C, 0x04);
        __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL2_G, 0x00);
    #endif
}

void GyroNormalMode() {
    #ifdef BSP_SNAIL_KITE
        __L3GD20_write8(L3GD20_REGISTER_CTRL_REG1, 0x0F);
    #endif

    /* Kite.d - Turn on the gyro (LEAVE ACCEL OFF!). Set to normal mode *
     *          at 104Hz. Turn off high performance mode to save power. */
    #ifdef BSP_SNAIL_KITE_D
        __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL2_G, 0x40);
        __LSM6DSL_write8(LSM6DSL_REGISTER_CTRL6_C, 0x10);
    #endif
}