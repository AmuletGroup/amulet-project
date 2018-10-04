/*
 * core_sensors.h
 *
 *  Created on: Oct 11, 2013
 *      Author: Andres
 */

#ifndef CORE_SENSORS_H_
#define CORE_SENSORS_H_

#include <stdint.h>

#include "amulet_sensors.h"

typedef struct {
	uint8_t application_id;
	uint8_t sensor_id;
	uint32_t expiration;
	uint8_t* config_params;
	uint8_t nparams;
	uint8_t window_size;
	uint8_t window_interval;
} sensorSubscription;

/**
 * Action: If the sensor requires configuration (currently, only the IMU requires
 * configuration), the Internal Sensor Manager will determine whether or not the
 * configuration in this request conflicts with another current configuration.
 * (Right now this only means that the IMU settings differ.) If the configuration
 * requires a higher sample rate than a current configuration, it will attempt to
 * reconfigure. If it is lower, then it will not reconfigure and instead the
 * Internal Sensor Manager will register that the application should only receive
 * subsampled data so that when a subscription takes place, the Internal Sensor
 * Manager produces events accordingly.
 *
 * The Internal Sensor Manager in the M4 creates an IPC message to the Internal
 * Sensor Manager in the M0. As a consequence the Internal Sensor Manager in the
 * M0 will reconfigure the sensor and proceed to accumulate window_size tuples
 * corresponding to data from the sensor with ID sensor_id.
 *
 * The Internal Sensor Manager in the M4 needs to keep track of the subscriptions,
 * so it generates a subscription ID for each of them. This subscription ID is added
 * as one of the parameters in the corresponding IPC message.
 */
void SubscribeInternalSensor(uint8_t sensor_id, uint8_t* config_params, uint8_t nparams,
		uint32_t expiration, uint8_t window_size, uint8_t window_interval,
		uint8_t caller_id);

void notifyAppsButtonPressed(uint8_t button_id) ;

void notifyAppButton(uint8_t button_id, uint8_t application_id);

/**
 * Action: Enable the temperature sensor
 */
void EnableTempSensor(void);

/**
 * Action: Disable the temperature sensor
 */
void DisableTempSensor(void);

/**
 * Action: Read the temperature from the temperature sensor and create an event with the temperature directed to a handler with ID caller_id.
Response event: TEMPERATURE(result)
 */
void GetTemperature(uint8_t caller_id);


/**
 * Action: Enable the light sensor
 */
void EnableLightSensor(void);

/**
 * Action: Disable the light sensor
 */
void DisableLightSensor(void);

/**
 * Action: Read the value from the light sensor and create an event with the value directed to a handler with ID caller_id
Response event: LIGHT(result)
 */
void GetLightSensor(uint8_t caller_id);

/**
 * Action: Enable the MPU
 */
void EnableMPU(void);


/**
 * Action: Disable the MPU
 */
void DisableMPU(void);


/**
 * Action: Configure the MPU according to the filter settings, sensor range settings and frequency. If the request fails create an event with an error value directed to a handler with ID caller_id.
Response event: CONFIG_MPU_RESPONSE(result)
 */
void ConfigMPU(uint8_t mpu_filter, uint8_t mpu_range, uint8_t mpu_freq, uint8_t caller_id);

/**
 * Action: Amulet accumulates window_size triples corresponding to accelerometer data. It then makes available the window_size by 3 dataset to the caller_id. Amulet notifies caller_id via an event  when a window of data is ready.
Response event: ACCEL_DATA(dataset,caller_id)
 */
void SubsMPUAccel(uint32_t expiration, uint8_t window_size, uint8_t caller_id);

/**
 * Action: Amulet accumulates window_size triples corresponding to gyro data. It then makes available the window_size by 3 dataset to the caller_id. Amulet notifies caller_id via an event when a window of data is ready.
Response event: GYRO_DATA(dataset,caller_id)
 */
void SubsMPUGyro(uint32_t expiration, uint8_t window_size, uint8_t caller_id);

/**
 * Action: Amulet accumulates window_size triples corresponding to magnetometer data. It then makes available the window_size by 3 dataset to the caller_id. Amulet notifies caller_id via an event when a window of data is ready.
Response event: MAGNET_DATA(dataset,caller_id)
 */
void SubsMPUMagnet(uint32_t expiration, uint8_t window_size, uint8_t caller_id);

/**
 * Action: Register that the handler with ID caller_id should be notified when the button with button_id is pressed.
Response event: BUTTON(button_id)
 */
void SubsButton(uint8_t button_id, uint8_t caller_id);

/**
 * Action: Deregister that the handler with ID caller_id should be notified when the button with button_id is pressed:
Response event:none?
 */
void UnsubsButton(uint8_t button_id, uint8_t caller_id);

void ListSubscriptions();

#endif /* CORE_SENSORS_H_ */
