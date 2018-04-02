/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#include "bsp_sensors.h"
const uint32_t BATT_MIN = 2194;  // 3.0 V
const uint32_t BATT_10P = 2650;  // 3.6239 V = 10% life left
const uint32_t BATT_MAX = 2999;  // 4.2 V

uint16_t take_battery_reading() { return read_ADC(BATTERY_INCH); }

uint8_t get_battery_level() {
  // Get battery adc level
  uint16_t batt_adc = take_battery_reading();

  // Convert adc to percent with 2 simple linear functions:
  // 100% to 10% = 4.2 V to 3.6239 V
  // 10% to 0% = 3.6239 V to 3.0 V
  uint32_t batt_percent;
  if (batt_adc > BATT_10P)
    batt_percent =
        ((batt_adc - BATT_10P) * 90) / (BATT_MAX - BATT_10P) + 10;  // >10%
  else
    batt_percent =
        ((batt_adc - BATT_MIN) * 10) / (BATT_10P - BATT_MIN);  // <=10%

  return (uint8_t)batt_percent;
}

uint16_t take_light_reading() {
  uint16_t retval = 0;
  // Turn on the sensor
  LIGHT_POWER_OUT |= LIGHT_POWER_BIT;
  LIGHT_POWER_DIR |= LIGHT_POWER_BIT;
  __delay_cycles(10000);

  // Sense the reading
  retval = read_ADC(LIGHT_INCH);
  // Turn off the power
  LIGHT_POWER_OUT &= ~LIGHT_POWER_BIT;
  // 	LIGHT_POWER_DIR &= ~LIGHT_POWER_BIT;
  return retval;
}

uint16_t take_temp_reading() {
  uint16_t adc_val = 0;
  // Turn on the sensor
  TEMP_POWER_DIR |= TEMP_POWER_BIT;
  TEMP_POWER_OUT |= TEMP_POWER_BIT;
  __delay_cycles(10000);

  // Sense the reading
  adc_val = read_ADC(TEMP_INCH);

  // Turn off the power
  TEMP_POWER_OUT &= ~TEMP_POWER_BIT;
  // TEMP_POWER_DIR &= ~TEMP_POWER_BIT;
  return adc_val;
}

uint8_t get_temperature() {
  uint16_t adc_val = take_temp_reading();

  // Convert adc to temperature (Steven Hearndon, 2/25/2016)
  adc_val *= 1.02;                        // Adjustment for accuracy
  int32_t v = (adc_val / 4096.0) * 2800;  // convert adc to mV
  // Calculate temp and convert to Fahrenheit:
  int32_t temp = ((v - 1864) * 9) / (-11.71 * 5) + 32;

  return (uint8_t)temp;
}

uint16_t take_audio_reading() {
  uint16_t retval = 0;
  // Turn on the sensor
  AUDIO_POWER_OUT |= AUDIO_POWER_BIT;
  AUDIO_POWER_DIR |= AUDIO_POWER_BIT;
  __delay_cycles(10000);

  // Sense the reading
  retval = read_ADC(AUDIO_INCH);
  // Turn off the power
  AUDIO_POWER_OUT &= ~AUDIO_POWER_BIT;
  // 	AUDIO_POWER_DIR &= ~AUDIO_POWER_BIT;
  return retval;
}
