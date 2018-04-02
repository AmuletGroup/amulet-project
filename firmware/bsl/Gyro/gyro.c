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
 ****************************************************/

#include "gyro.h"
#include "ADXL362driver/spi_430.h"
#include "bsp_init.h" // To configure for bracelet or dev kit
#include "ADXL362driver/util.h"

void __L3GD20_write8(l3gd20Registers_t reg, uint8_t value)
{
  GYRO_CS_PxOUT &= ~GYRO_CS_BITx;
  spi_send(reg);
  spi_send(value);
  GYRO_CS_PxOUT |= GYRO_CS_BITx;
}

uint8_t __L3GD20_read8(l3gd20Registers_t reg)
{
  uint8_t value;
  GYRO_CS_PxOUT &= ~GYRO_CS_BITx;
  spi_send((uint8_t)reg | 0x80); // set READ bit
  value = spi_send(0xFF);
  GYRO_CS_PxOUT |= GYRO_CS_BITx;
  return value;
}


int8_t GyroBegin()
{
    //pinMode(_cs, OUTPUT);
    //SPI.begin();
    //SPI.setBitOrder(MSBFIRST); // I think this is default actually
    //SPI.setDataMode(SPI_MODE0); //CPHA = CPOL = 0    MODE = 0

    //pinMode(_clk, OUTPUT);
    //pinMode(_mosi, OUTPUT);
    //pinMode(_miso, INPUT);
    //digitalWrite(_cs, HIGH);

  // Turn on power to Gyro
  GYRO_ENABLE_PxDIR |= GYRO_ENABLE_BITx;
  GYRO_ENABLE_PxOUT &= ~GYRO_ENABLE_BITx;

  // Chip select high for Gyro
  GYRO_CS_PxDIR |= GYRO_CS_BITx;
  GYRO_CS_PxOUT |= GYRO_CS_BITx;

  address = L3GD20_ADDRESS;
  range = L3DS20_RANGE_250DPS;

  /* Make sure we have the correct chip ID since this checks
     for correct address and that the IC is properly connected */
  uint8_t id = __L3GD20_read8(L3GD20_REGISTER_WHO_AM_I);
  //Serial.println(id, HEX);
  if ((id != L3GD20_ID) && (id != L3GD20H_ID))
  {
    return 0;
  }

  /* Set CTRL_REG1 (0x20)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
   7-6  DR1/0     Output data rate                                   00
   5-4  BW1/0     Bandwidth selection                                00
     3  PD        0 = Power-down mode, 1 = normal/sleep mode          0
     2  ZEN       Z-axis enable (0 = disabled, 1 = enabled)           1
     1  YEN       Y-axis enable (0 = disabled, 1 = enabled)           1
     0  XEN       X-axis enable (0 = disabled, 1 = enabled)           1 */

  /* Switch to power down mode and enable all three channels */
  GyroPowerDownMode();

  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG2 (0x21)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
   5-4  HPM1/0    High-pass filter mode selection                    00
   3-0  HPCF3..0  High-pass filter cutoff frequency selection      0000 */

  /* Nothing to do ... keep default values */
  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG3 (0x22)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
     7  I1_Int1   Interrupt enable on INT1 (0=disable,1=enable)       0
     6  I1_Boot   Boot status on INT1 (0=disable,1=enable)            0
     5  H-Lactive Interrupt active config on INT1 (0=high,1=low)      0
     4  PP_OD     Push-Pull/Open-Drain (0=PP, 1=OD)                   0
     3  I2_DRDY   Data ready on DRDY/INT2 (0=disable,1=enable)        0
     2  I2_WTM    FIFO wtrmrk int on DRDY/INT2 (0=dsbl,1=enbl)        0
     1  I2_ORun   FIFO overrun int on DRDY/INT2 (0=dsbl,1=enbl)       0
     0  I2_Empty  FIFI empty int on DRDY/INT2 (0=dsbl,1=enbl)         0 */

  /* Nothing to do ... keep default values */
  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG4 (0x23)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
     7  BDU       Block Data Update (0=continuous, 1=LSB/MSB)         0
     6  BLE       Big/Little-Endian (0=Data LSB, 1=Data MSB)          0
   5-4  FS1/0     Full scale selection                               00
                                  00 = 250 dps
                                  01 = 500 dps
                                  10 = 2000 dps
                                  11 = 2000 dps
     0  SIM       SPI Mode (0=4-wire, 1=3-wire)                       0 */

  /* Adjust resolution if requested */
  switch(range)
  {
    case L3DS20_RANGE_250DPS:
      __L3GD20_write8(L3GD20_REGISTER_CTRL_REG4, 0x00);
      break;
    case L3DS20_RANGE_500DPS:
      __L3GD20_write8(L3GD20_REGISTER_CTRL_REG4, 0x10);
      break;
    case L3DS20_RANGE_2000DPS:
      __L3GD20_write8(L3GD20_REGISTER_CTRL_REG4, 0x20);
      break;
  }
  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG5 (0x24)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
     7  BOOT      Reboot memory content (0=normal, 1=reboot)          0
     6  FIFO_EN   FIFO enable (0=FIFO disable, 1=enable)              0
     4  HPen      High-pass filter enable (0=disable,1=enable)        0
   3-2  INT1_SEL  INT1 Selection config                              00
   1-0  OUT_SEL   Out selection config                               00 */

  /* Nothing to do ... keep default values */
  /* ------------------------------------------------------------------ */

  return 1;
}

void GyroReadXYZData(l3gd20Data * data)
{ 
  uint8_t xhi, xlo, ylo, yhi, zlo, zhi;

  //digitalWrite(_cs, LOW);
  GYRO_CS_PxOUT &= ~GYRO_CS_BITx;

  spi_send(L3GD20_REGISTER_OUT_X_L | 0x80 | 0x40); // SPI read, autoincrement
  delay(10);
  xlo = spi_send(0xFF);
  xhi = spi_send(0xFF);
  ylo = spi_send(0xFF);
  yhi = spi_send(0xFF);
  zlo = spi_send(0xFF);
  zhi = spi_send(0xFF);

  //digitalWrite(_cs, HIGH);
  GYRO_CS_PxOUT |= GYRO_CS_BITx;

  // Shift values to create properly formed integer (low uint8_t first)
  data->x = (int16_t)(xlo | (xhi << 8));
  data->y = (int16_t)(ylo | (yhi << 8));
  data->z = (int16_t)(zlo | (zhi << 8));
  
  // Compensate values depending on the resolution
  switch(range)
  {
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

/* Set CTRL_REG1 (0x20)
 ====================================================================
 BIT  Symbol    Description                                   Default
 ---  ------    --------------------------------------------- -------
 7-6  DR1/0     Output data rate                                   00
 5-4  BW1/0     Bandwidth selection                                00
   3  PD        0 = Power-down mode, 1 = normal/sleep mode          0
   2  ZEN       Z-axis enable (0 = disabled, 1 = enabled)           1
   1  YEN       Y-axis enable (0 = disabled, 1 = enabled)           1
   0  XEN       X-axis enable (0 = disabled, 1 = enabled)           1 */

/* Switch to normal mode and enable all three channels */
void GyroPowerDownMode() {
  __L3GD20_write8(L3GD20_REGISTER_CTRL_REG1, 0x07);
}

void GyroNormalMode() {
  __L3GD20_write8(L3GD20_REGISTER_CTRL_REG1, 0x0F);
}