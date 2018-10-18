/***************************************************
  This is a library for the L3GD20 GYROSCOPE

  Designed specifically to work with the Adafruit L3GD20 Breakout 
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
#ifndef __GYRO_H__
#define __GYRO_H__
#include <msp430.h>
#include <stdint.h>

#define L3GD20_ADDRESS                (0x6B)        // 1101011
#define LSM6DSL_ADDRESS               (0x6B)        // 1101011
#define L3GD20_POLL_TIMEOUT           (100)         // Maximum number of read attempts
#define L3GD20_ID                     0xD4
#define L3GD20H_ID                    0xD7
#define LSM6DSL_ID                    0x6A

#define L3GD20_SENSITIVITY_250DPS  (0.00875F)      // Roughly 22/256 for fixed point match
#define L3GD20_SENSITIVITY_500DPS  (0.0175F)       // Roughly 45/256
#define L3GD20_SENSITIVITY_2000DPS (0.070F)        // Roughly 18/256
#define L3GD20_DPS_TO_RADS         (0.017453293F)  // degress/s to rad/s multiplier

typedef enum
{                                               // DEFAULT    TYPE
  L3GD20_REGISTER_WHO_AM_I            = 0x0F,   // 00001111   r
  L3GD20_REGISTER_CTRL_REG1           = 0x20,   // 00000111   rw
  L3GD20_REGISTER_CTRL_REG2           = 0x21,   // 00000000   rw
  L3GD20_REGISTER_CTRL_REG3           = 0x22,   // 00000000   rw
  L3GD20_REGISTER_CTRL_REG4           = 0x23,   // 00000000   rw
  L3GD20_REGISTER_CTRL_REG5           = 0x24,   // 00000000   rw
  L3GD20_REGISTER_REFERENCE           = 0x25,   // 00000000   rw
  L3GD20_REGISTER_OUT_TEMP            = 0x26,   //            r
  L3GD20_REGISTER_STATUS_REG          = 0x27,   //            r
  L3GD20_REGISTER_OUT_X_L             = 0x28,   //            r
  L3GD20_REGISTER_OUT_X_H             = 0x29,   //            r
  L3GD20_REGISTER_OUT_Y_L             = 0x2A,   //            r
  L3GD20_REGISTER_OUT_Y_H             = 0x2B,   //            r
  L3GD20_REGISTER_OUT_Z_L             = 0x2C,   //            r
  L3GD20_REGISTER_OUT_Z_H             = 0x2D,   //            r
  L3GD20_REGISTER_FIFO_CTRL_REG       = 0x2E,   // 00000000   rw
  L3GD20_REGISTER_FIFO_SRC_REG        = 0x2F,   //            r
  L3GD20_REGISTER_INT1_CFG            = 0x30,   // 00000000   rw
  L3GD20_REGISTER_INT1_SRC            = 0x31,   //            r
  L3GD20_REGISTER_TSH_XH              = 0x32,   // 00000000   rw
  L3GD20_REGISTER_TSH_XL              = 0x33,   // 00000000   rw
  L3GD20_REGISTER_TSH_YH              = 0x34,   // 00000000   rw
  L3GD20_REGISTER_TSH_YL              = 0x35,   // 00000000   rw
  L3GD20_REGISTER_TSH_ZH              = 0x36,   // 00000000   rw
  L3GD20_REGISTER_TSH_ZL              = 0x37,   // 00000000   rw
  L3GD20_REGISTER_INT1_DURATION       = 0x38    // 00000000   rw
} l3gd20Registers_t;

typedef enum
{
  // RESERVED                                         // DEFAULT    TYPE
  LSM6DSL_REGISTER_RESERVED1                = 0x00,   //

  // Embedded functions configuration register
  LSM6DSL_REGISTER_FUNC_CFG_ACCESS          = 0x01,   // 00000000   rw

  // RESERVED
  LSM6DSL_REGISTER_RESERVED2                = 0x02,   //
  LSM6DSL_REGISTER_RESERVED3                = 0x03,   //

  // Sensor sync configuration registers
  LSM6DSL_REGISTER_SENSOR_SYNC_TIME_FRAME   = 0x04,   // 00000000   rw
  LSM6DSL_REGISTER_SENSOR_SYNC_RES_RATIO    = 0x05,   // 00000000   rw
  
  // FIFO configuration registers
  LSM6DSL_REGISTER_FIFO_CTRL1               = 0x06,   // 00000000   rw
  LSM6DSL_REGISTER_FIFO_CTRL2               = 0x07,   // 00000000   rw
  LSM6DSL_REGISTER_FIFO_CTRL3               = 0x08,   // 00000000   rw
  LSM6DSL_REGISTER_FIFO_CTRL4               = 0x09,   // 00000000   rw
  LSM6DSL_REGISTER_FIFO_CTRL5               = 0x0A,   // 00000000   rw

  // DataReady configuration register
  LSM6DSL_REGISTER_DRDY_PULSE_CFG_G         = 0x0B,   // 00000000   rw

  // RESERVED
  LSM6DSL_REGISTER_RESERVED4                = 0x0C,   // 00000000   rw

  // Interrupt pins 1 & 2 controls
  LSM6DSL_REGISTER_INT1_CTRL                = 0x0D,   // 00000000   rw
  LSM6DSL_REGISTER_INT2_CTRL                = 0x0E,   // 00000000   rw

  // Who I am ID
  LSM6DSL_REGISTER_WHO_AM_I                 = 0x0F,   // 01101010   r

  // Accelerometer and Gyroscope control registers
  LSM6DSL_REGISTER_CTRL1_XL                 = 0x10,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL2_G                  = 0x11,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL3_C                  = 0x12,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL4_C                  = 0x13,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL5_C                  = 0x14,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL6_C                  = 0x15,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL7_G                  = 0x16,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL8_XL                 = 0x17,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL9_XL                 = 0x18,   // 00000000   rw
  LSM6DSL_REGISTER_CTRL10_C                 = 0x19,   // 00000000   rw

  // I2C master configuration register
  LSM6DSL_REGISTER_MASTER_CONFIG            = 0x1A,   // 00000000   rw

  // Wake-up interrupt source register
  LSM6DSL_REGISTER_WAKE_UP_SRC              = 0x1B,   // output     r

  // Tap interrupt source register
  LSM6DSL_REGISTER_TAP_SRC                  = 0x1C,   // output     r

  // Portrait, landscape, face-up and face-down source register
  LSM6DSL_REGISTER_D6D_SRC                  = 0x1D,   // output     r

  // Status data register for user interface
  LSM6DSL_REGISTER_STATUS_REG               = 0x1E,   // output     r

  // RESERVED
  LSM6DSL_REGISTER_RESERVED5                = 0x1F,   //

  // Temperature output data registers
  LSM6DSL_REGISTER_OUT_TEMP_L               = 0x20,   // output     r
  LSM6DSL_REGISTER_OUT_TEMP_H               = 0x21,   // output     r

  // Gyroscope output registers
  LSM6DSL_REGISTER_OUTX_L_G                 = 0x22,   // output     r
  LSM6DSL_REGISTER_OUTX_H_G                 = 0x23,   // output     r
  LSM6DSL_REGISTER_OUTY_L_G                 = 0x24,   // output     r
  LSM6DSL_REGISTER_OUTY_H_G                 = 0x25,   // output     r
  LSM6DSL_REGISTER_OUTZ_L_G                 = 0x26,   // output     r
  LSM6DSL_REGISTER_OUTZ_H_G                 = 0x27,   // output     r

  // Acceleromter output registers
  LSM6DSL_REGISTER_OUTX_L_XL                = 0x28,   // output     r
  LSM6DSL_REGISTER_OUTX_H_XL                = 0x29,   // output     r
  LSM6DSL_REGISTER_OUTY_L_XL                = 0x2A,   // output     r
  LSM6DSL_REGISTER_OUTY_H_XL                = 0x2B,   // output     r
  LSM6DSL_REGISTER_OUTZ_L_XL                = 0x2C,   // output     r
  LSM6DSL_REGISTER_OUTZ_H_XL                = 0x2D,   // output     r

  // Sensor hub output registers
  LSM6DSL_REGISTER_SENSORHUB1_REG           = 0x2E,   // output     r
  LSM6DSL_REGISTER_SENSORHUB2_REG           = 0x2F,   // output     r
  LSM6DSL_REGISTER_SENSORHUB3_REG           = 0x30,   // output     r
  LSM6DSL_REGISTER_SENSORHUB4_REG           = 0x31,   // output     r
  LSM6DSL_REGISTER_SENSORHUB5_REG           = 0x32,   // output     r
  LSM6DSL_REGISTER_SENSORHUB6_REG           = 0x33,   // output     r
  LSM6DSL_REGISTER_SENSORHUB7_REG           = 0x34,   // output     r
  LSM6DSL_REGISTER_SENSORHUB8_REG           = 0x35,   // output     r
  LSM6DSL_REGISTER_SENSORHUB9_REG           = 0x36,   // output     r
  LSM6DSL_REGISTER_SENSORHUB10_REG          = 0x37,   // output     r
  LSM6DSL_REGISTER_SENSORHUB11_REG          = 0x38,   // output     r
  LSM6DSL_REGISTER_SENSORHUB12_REG          = 0x39,   // output     r

  // FIFO status registers
  LSM6DSL_REGISTER_FIFO_STATUS1             = 0x3A,   // output     r
  LSM6DSL_REGISTER_FIFO_STATUS2             = 0x3B,   // output     r
  LSM6DSL_REGISTER_FIFO_STATUS3             = 0x3C,   // output     r
  LSM6DSL_REGISTER_FIFO_STATUS4             = 0x3D,   // output     r

  // FIFO data output registers
  LSM6DSL_REGISTER_FIFO_DATA_OUT_L          = 0x3E,   // output     r
  LSM6DSL_REGISTER_FIFO_DATA_OUT_H          = 0x3F,   // output     r

  // Timestamp output registers
  LSM6DSL_REGISTER_TIMESTAMP0_REG           = 0x40,   // output     r
  LSM6DSL_REGISTER_TIMESTAMP1_REG           = 0x41,   // output     r
  LSM6DSL_REGISTER_TIMESTAMP2_REG           = 0x42,   // output     r

  // RESERVED
  LSM6DSL_REGISTER_RESERVED6                = 0x43,   //

  // Step counter timestamp registers
  LSM6DSL_REGISTER_STEP_TIMESTAMP_L         = 0x49,   // output     r
  LSM6DSL_REGISTER_STEP_TIMESTAMP_H         = 0x4A,   // output     r

  // Step counter output registers
  LSM6DSL_REGISTER_STEP_COUNTER_L           = 0x4B,   // output     r
  LSM6DSL_REGISTER_STEP_COUNTER_H           = 0x4C,   // output     r

  // Sensor hub output registers
  LSM6DSL_REGISTER_SENSORHUB13_REG          = 0x4D,   // output     r
  LSM6DSL_REGISTER_SENSORHUB14_REG          = 0x4E,   // output     r
  LSM6DSL_REGISTER_SENSORHUB15_REG          = 0x4F,   // output     r
  LSM6DSL_REGISTER_SENSORHUB16_REG          = 0x50,   // output     r
  LSM6DSL_REGISTER_SENSORHUB17_REG          = 0x51,   // output     r
  LSM6DSL_REGISTER_SENSORHUB18_REG          = 0x52,   // output     r

  // Significant motion, tilt, step detector, hard/soft-iron
  // and sensor hub interrupt source register
  LSM6DSL_REGISTER_FUNC_SRC1                = 0x53,   // output     r

  // Wrist tilt registers
  LSM6DSL_REGISTER_FUNC_SRC2                = 0x54,   // output     r
  LSM6DSL_REGISTER_WRIST_TILT_IA            = 0x55,   // output     r

  // RESERVED
  LSM6DSL_REGISTER_RESERVED7                = 0x56,   //

  // Tap configuration and interrupt registers
  LSM6DSL_REGISTER_TAP_CFG                  = 0x58,   // 00000000   rw
  LSM6DSL_REGISTER_TAP_THIS_6D              = 0x59,   // 00000000   rw
  LSM6DSL_REGISTER_INT_DUR2                 = 0x5A,   // 00000000   rw
  LSM6DSL_REGISTER_WAKE_UP_THS              = 0x5B,   // 00000000   rw
  LSM6DSL_REGISTER_WAKE_UP_DUR              = 0x5C,   // 00000000   rw

  // Free-fall function duration setting register
  LSM6DSL_REGISTER_FREE_FALL                = 0x5D,   // 00000000   rw

  // Functions routing on int 1 and 2 pins
  LSM6DSL_REGISTER_MD1_CFG                  = 0x5E,   // 00000000   rw
  LSM6DSL_REGISTER_MD2_CFG                  = 0x5F,   // 00000000   rw

  // Master command code used for stamping for sensor sync
  LSM6DSL_REGISTER_MASTER_CMD_CODE          = 0x60,   // 00000000   rw

  // Error code used for sensor synchronization
  LSM6DSL_REGISTER_SENS_SYNC_SPI_ERROR_CODE = 0x61,   // 00000000   rw

  // RESERVED
  LSM6DSL_REGISTER_RESERVED8                = 0x62,   //

  // External magnetometer raw data output registers
  LSM6DSL_REGISTER_OUT_MAG_RAW_X_L          = 0x66,   // output     r
  LSM6DSL_REGISTER_OUT_MAG_RAW_X_H          = 0x67,   // output     r
  LSM6DSL_REGISTER_OUT_MAG_RAW_Y_L          = 0x68,   // output     r
  LSM6DSL_REGISTER_OUT_MAG_RAW_Y_H          = 0x69,   // output     r
  LSM6DSL_REGISTER_OUT_MAG_RAW_Z_L          = 0x6A,   // output     r
  LSM6DSL_REGISTER_OUT_MAG_RAW_Z_H          = 0x6B,   // output     r

  // RESERVED
  LSM6DSL_REGISTER_RESERVED9                = 0x6C,   //

  // Accelerometer user offset correction
  LSM6DSL_REGISTER_X_OFS_USR                = 0x73,   // 00000000   rw
  LSM6DSL_REGISTER_Y_OFS_USR                = 0x74,   // 00000000   rw
  LSM6DSL_REGISTER_Z_OFS_USR                = 0x75,   // 00000000   rw

  // RESERVED
  LSM6DSL_REGISTER_RESERVED10               = 0x76    //
} lsm6dslRegisters_t;

typedef enum
{
  L3DS20_RANGE_250DPS,
  L3DS20_RANGE_500DPS,
  L3DS20_RANGE_2000DPS
} l3gd20Range_t;

typedef struct l3gd20Data_s
{
  float x;
  float y;
  float z;
} l3gd20Data;


// l3gd20Range_t rng=L3DS20_RANGE_250DPS, uint8_t addr=L3GD20_ADDRESS
int8_t GyroBegin();
void GyroPowerDownMode();
void GyroNormalMode();
void GyroReadXYZData(l3gd20Data * data);

uint8_t address;
l3gd20Range_t range;
int8_t _miso, _mosi, _clk, _cs;

#endif // __GYRO_H__