#ifndef BSP_SENSORS_H_
#define BSP_SENSORS_H_

#include "ADC_driver.h"

/*
	Base drivers for using the analog sensors currently on the Amulet app board
	Light, Microphone, and the Temperature.
*/
uint16_t take_battery_reading();
uint8_t get_battery_level();
uint16_t take_light_reading();
uint16_t take_temp_reading();
uint8_t get_temperature();
uint16_t take_audio_reading();

#endif /* BSP_SENSORS_H_ */
