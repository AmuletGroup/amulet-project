// Ron Peterson
// 3/1/2016

#include <msp430.h>
#include <msp430fr5989.h>
#include "L3GD20H.h"  // Gyro function definitions
#include "bsp_init.h" // Bracelet or dev kit pin definitions

void gyroInit(){

   //
   // Set up gyro ENABLE pin
   //
   // Set PxSEL0 and PxSEL1 to zero to configure pin as normal GPIO
   GYRO_ENABLE_PxSEL0 &= ~GYRO_ENABLE_BITx;
   GYRO_ENABLE_PxSEL1 &= ~GYRO_ENABLE_BITx;
   // Set ENABLE pin direction to output (1=out, 0=in)
// This next line is causing a 144Hz signal out on P5.2!
   GYRO_ENABLE_PxDIR |= GYRO_ENABLE_BITx;
   // Set ENABLE to low to disable the gyro
   GYRO_ENABLE_PxOUT &= ~GYRO_ENABLE_BITx;

   //
   // Set up gyro CS pin
   //

   // Set PxSEL0 and PxSEL1 to zero to configure pin as normal GPIO
   GYRO_CS_PxSEL0 &= ~GYRO_CS_BITx;
   GYRO_CS_PxSEL1 &= ~GYRO_CS_BITx;
   // Set CS pin direction to output (1=out, 0=in)
   GYRO_CS_PxDIR |= GYRO_CS_BITx;
   // Set CS to high (1=I2C mode enable, 0=SPI mode enable)
   // Want to be in I2C mode so gyro ignores SPI traffic
   GYRO_CS_PxOUT |= GYRO_CS_BITx;

   // Gyro SCLK, MOSI, and MISO pins are shared currently with
   // the accelerometer, SD card, and display, so no need to
   // configure them here, they should already be configured.
   // In a future rev the gyro will be on it's own SPI bus so
   // I've included the configuration here, commented out.

#if 0 // Uncomment when gyro is on it's own SPI bus

   //
   // Set up gyro SCLK pin
   //

   // Set PxSEL0 and PxSEL1 to zero to configure pin as normal GPIO
   GYRO_SCLK_PxSEL0 &= ~GYRO_SCLK_BITx;
   GYRO_SCLK_PxSEL1 &= ~GYRO_SCLK_BITx;
   // Set SCLK pin direction to output (1=out, 0=in)
   GYRO_SCLK_PxDIR |= GYRO_SCLK_BITx;
   // Set SCLK to zero
   GYRO_SCLK_PxOUT &= ~GYRO_SCLK_BITx;

   //
   // Set up gyro MOSI pin
   //

   // Set PxSEL0 and PxSEL1 to zero to configure pin as normal GPIO
   GYRO_MOSI_PxSEL0 &= ~GYRO_MOSI_BITx;
   GYRO_MOSI_PxSEL1 &= ~GYRO_MOSI_BITx;
   // Set MOSI pin direction to output (1=out, 0=in)
   GYRO_MOSI_PxDIR |= GYRO_MOSI_BITx;
   // Set MOSI to low
   GYRO_MOSI_PxOUT &= ~GYRO_MOSI_BITx;

   //
   // Set up gyro MISO pin
   //

   // Set PxSEL0 and PxSEL1 to zero to configure pin as normal GPIO
   GYRO_CS_PxSEL0 &= ~GYRO_CS_BITx;
   GYRO_CS_PxSEL1 &= ~GYRO_CS_BITx;
   // Set MISO pin direction to input (1=out, 0=in)
   GYRO_CS_PxDIR &= ~GYRO_CS_BITx;
#endif

   // Make sure gyro is off at initialization (turn off FET transistor
   // controlling power to it.)
   turn_gyro_Off();
}

void turn_gyro_On(){
   // Set ENABLE to high to enable the gyro
   GYRO_ENABLE_PxOUT |= GYRO_ENABLE_BITx;
}

void turn_gyro_Off(){
   // Set ENABLE to low to disable the gyro
   GYRO_ENABLE_PxOUT &= ~GYRO_ENABLE_BITx;
}

