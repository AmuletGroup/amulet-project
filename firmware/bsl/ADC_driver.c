/*
 * ADC_driver.c
 * written by gunnar pope on 4/2/2015
 * modified by Josiah Hester
 * modified by Steven Hearndon 3/9/2016
 * shearnd@clemson.edu
 * example taken from MSP430 Microcontroller basics by John Davies, p 418
 *
 *
 */

#include "ADC_driver.h"

void adc_init() {
  // init the battery voltage adc
  BATTERY_ADC_PXDIR &= ~BATTERY_ADC_INPUT_BITX;
  BATTERY_ADC_PXSEL0 |= BATTERY_ADC_INPUT_BITX;
  BATTERY_ADC_PXSEL1 |= BATTERY_ADC_INPUT_BITX;

  // initialize the light ADC pins
  LIGHT_ADC_PXDIR &= ~LIGHT_ADC_INPUT_BITX;
  LIGHT_ADC_PXSEL0 |= LIGHT_ADC_INPUT_BITX;
  LIGHT_ADC_PXSEL1 |= LIGHT_ADC_INPUT_BITX;

  // initialize the temp ADC pins
  TEMP_ADC_PXDIR &= ~TEMP_ADC_INPUT_BITX;
  TEMP_ADC_PXSEL0 |= TEMP_ADC_INPUT_BITX;
  TEMP_ADC_PXSEL1 |= TEMP_ADC_INPUT_BITX;

  // initialize the mic audio ADC pins
  AUDIO_ADC_PXDIR &= ~AUDIO_ADC_INPUT_BITX;
  AUDIO_ADC_PXSEL0 |= AUDIO_ADC_INPUT_BITX;
  AUDIO_ADC_PXSEL1 |= AUDIO_ADC_INPUT_BITX;

  // sample and hold time of 128 clock cycles
  // for the 8MHz clock this means max sample rate of 62.5kHz
  ADC12CTL0 = (ADC12SHT0_6 | ADC12SHT1_6);

  // clk divide by 8, sample and hold off of ASC12SC bit, SAMPCON=1 (conv
  // triggered on sample timer), no inversion, use MCLK,
  // single chan, single conversion, use MCLK as timer
  ADC12CTL1 = (ADC12DIV0 | ADC12DIV1 | ADC12DIV2 | 0x0200 | ADC12SSEL1);

  // set resolution to 12 bit, binary unsigned format (0x0FFF),
  // set ADC to low power mode, since we sample slowly anyway
  // ADC12CLK can not be greater than 1/4 the device-specific data sheet
  // specified maximum for ADC12PWRMD = 0
  ADC12CTL2 = (ADC12RES1 | ADC12PWRMD);

  // set starting memory address to ADC12MEM0;
  ADC12CTL3 = 0x0000;

  // enable interrupts on the ADC Memory 0 (Don't do this anymore - Steven
  // Hearndon, 3/9/16)
  // ADC12IER0 = ADC12IE0;

  // since we have a stable battery voltage, no need for the reference to be
  // used
}

void enable_ADC() {
  // power up the adc core
  ADC12CTL0 |= ADC12ON;
}

void disable_ADC() {
  // power down the adc core
  ADC12CTL0 &= ~ADC12ENC;
  ADC12CTL0 &= ~ADC12ON;
}

// Reads the adc on the passed 12-bit input channel
// (many #defined in bsp_bracelet_init.h)
uint16_t read_ADC(uint32_t input_channel) {
  // Can only change inputs when ADC12ENC is disabled
  ADC12CTL0 &= ~ADC12ENC;
  ADC12MCTL0 = input_channel;
  // enable a software-triggered conversion
  ADC12CTL0 |= (ADC12SC | ADC12ENC);

  // Busy wait till conversion completes
  while (ADC12CTL1 & ADC12BUSY);

  // Return adc value
  return (uint16_t)(ADC12MEM0)&0x0FFF;
}
