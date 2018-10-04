/*
 * core_sensors.c
 *
 *  Created on: Oct 11, 2013
 *      Author: Andres
 */

#include <stdint.h>
#include <stdlib.h>

#include "qp_port.h"
#include "bsp.h"
#include "apps.h"
//#include "hello.h"
#include "amulet_sensors.h"
#include "amulet_network.h"
#include "core_sensors.h"
#include "core_network.h"
//#include "bsp_uart.h"

extern QActive * AppPtrs[2];

#define MAX_SENSOR_SUBSCRIPTIONS 5

static volatile sensorSubscription sensorSubscriptions[MAX_SENSOR_SUBSCRIPTIONS];
static volatile uint8_t currentNumSubscriptions = 0;

/**
 * Action: Enable the temperature sensor
 */
void EnableTempSensor(void) {

}

/**
 * Action: Disable the temperature sensor
 */
void DisableTempSensor(void) {

}

/**
 * Action: Read the temperature from the temperature sensor and create an event with the temperature directed to a handler with ID caller_id.
 Response event: TEMPERATURE(result)
 */
void GetTemperature(uint8_t caller_id) {

}

/**
 * Action: Enable the light sensor
 */
void EnableLightSensor(void) {

}

/**
 * Action: Disable the light sensor
 */
void DisableLightSensor(void) {

}

/**
 * Action: Read the value from the light sensor and create an event with the value directed to a handler with ID caller_id
 Response event: LIGHT(result)
 */
void GetLightSensor(uint8_t caller_id) {

}

/**
 * Action: Enable the MPU
 */
void EnableMPU(void) {

}

/**
 * Action: Disable the MPU
 */
void DisableMPU(void) {

}

/**
 * Action: Configure the MPU according to the filter settings, sensor range settings and frequency. If the request fails create an event with an error value directed to a handler with ID caller_id.
 Response event: CONFIG_MPU_RESPONSE(result)
 */
void ConfigMPU(uint8_t mpu_filter, uint8_t mpu_range, uint8_t mpu_freq,
		uint8_t caller_id) {

}

/**
 * Action: Amulet accumulates window_size triples corresponding to accelerometer data. It then makes available the window_size by 3 dataset to the caller_id. Amulet notifies caller_id via an event  when a window of data is ready.
 Response event: ACCEL_DATA(dataset,caller_id)
 */
void SubsMPUAccel(uint32_t expiration, uint8_t window_size, uint8_t caller_id) {

}

/**
 * Action: Amulet accumulates window_size triples corresponding to gyro data. It then makes available the window_size by 3 dataset to the caller_id. Amulet notifies caller_id via an event when a window of data is ready.
 Response event: GYRO_DATA(dataset,caller_id)
 */
void SubsMPUGyro(uint32_t expiration, uint8_t window_size, uint8_t caller_id) {

}

/**
 * Action: Amulet accumulates window_size triples corresponding to magnetometer data. It then makes available the window_size by 3 dataset to the caller_id. Amulet notifies caller_id via an event when a window of data is ready.
 Response event: MAGNET_DATA(dataset,caller_id)
 */
void SubsMPUMagnet(uint32_t expiration, uint8_t window_size, uint8_t caller_id) {

}

/**
 * Action: Register that the handler with ID caller_id should be notified when the button with button_id is pressed.
 Response event: BUTTON(button_id)
 */
void SubsButton(uint8_t button_id, uint8_t caller_id) {

}

/**
 * Action: Deregister that the handler with ID caller_id should be notified when the button with button_id is pressed:
 Response event:none?
 */
void UnsubsButton(uint8_t button_id, uint8_t caller_id) {

}

void SubscribeInternalSensor(uint8_t sensor_id, uint8_t* config_params,
		uint8_t nparams, uint32_t expiration, uint8_t window_size,
		uint8_t window_interval, uint8_t caller_id) {
	//int i;
	if (currentNumSubscriptions < MAX_SENSOR_SUBSCRIPTIONS - 1) {
		currentNumSubscriptions++;
		sensorSubscriptions[currentNumSubscriptions - 1].application_id =
				caller_id;
		sensorSubscriptions[currentNumSubscriptions - 1].sensor_id = sensor_id;

		/**
		//Send message to M0
		//Generate an IPC message:
		//uint8_t messageParameters[2] = {1,2};
		//uint8_t messageBytes[25];
		//4 start, 4 end, 1 major type
		//2 len, 1 minor type, 1 target id
		//2 params, 1 crc
		ipcMessage subsMessage;
		subsMessage.messageID = 500; //TODO: This needs to be determined dynamically
		subsMessage.majorType = IPC_REQUEST;
		subsMessage.minorType = SUBSCRIBE_INTERNAL_SENSOR;
		subsMessage.targetID = INT_SENSOR_MAN_M0;
		subsMessage.configParams = config_params;
		subsMessage.nparams = nparams;
		//subsMessage.length = 16 + nparams;
		ipcSend(&subsMessage);
		//messageToBytes(&subsMessage, messageBytes);
		//UART0Send(messageBytes, 16 + nparams);

		//Debug:
		//for(i=0; i < 10; i++){
		//	UARTprintf("App %d subscribed to %d", caller_id, sensor_id);
		//}
		 *
		 */
	}
}

void ListSubscriptions() {
#ifdef DEBUG_UART
	int i = 0;

	UARTprintf("Apps subscribed: %d \r\n", currentNumSubscriptions);
	for (i = 0; i < currentNumSubscriptions; i++) {
		UARTprintf("App %d is subscribed to sensor: %d \r\n",
				sensorSubscriptions[i].application_id,
				sensorSubscriptions[i].sensor_id);
	}
#endif

}

void notifyAppsButtonPressed(uint8_t button_id) {
	int i = 0;
	for (i = 0; i < currentNumSubscriptions; i++) {
		if (button_id == sensorSubscriptions[i].sensor_id) {
			//Notify corresponding app that button button_id was pressed
			notifyAppButton(button_id, sensorSubscriptions[i].application_id);
		}
	}
}

void notifyAppButton(uint8_t button_id, uint8_t application_id) {
#ifdef HELLO_APP
	if (application_id == HELLO_APP) {

		if (button_id == BUTTONA) {
			//Using directed event messages
			SensorEvt *buttonPressedEvt;
			buttonPressedEvt = Q_NEW(SensorEvt, BUTTON_A_PRESSED_SIG);
			//QACTIVE_POST(AO_Hello , &buttonPressedEvt->super, NULL);
			//TODO: The corresponding application (AO) should be obtained
			//from a data structure e.g., a table.
			//QACTIVE_POST(AppPtrs[0], &buttonPressedEvt->super, NULL);
			QACTIVE_POST(AO_Hello, &buttonPressedEvt->super, NULL);

			//Using public subscribe
			//static QEvt const buttonPressedEvt = { BUTTON_A_PRESSED_SIG, 0U, 0U}; //this works!
			//QF_PUBLISH(&buttonPressedEvt, &l_SysTick_Handler);
		} else if (button_id == BUTTONB) {
			//Using directed event messages
			SensorEvt *buttonPressedEvt;
			buttonPressedEvt = Q_NEW(SensorEvt, BUTTON_B_PRESSED_SIG);
			QACTIVE_POST(AO_Hello , &buttonPressedEvt->super, NULL);
			//QACTIVE_POST(AppPtrs[0], &buttonPressedEvt->super, NULL);
			//Using public subscribe
			//static QEvt const buttonPressedEvt = { BUTTON_B_PRESSED_SIG, 0U, 0U};
			//QF_PUBLISH(&buttonPressedEvt, &l_SysTick_Handler);



			/*
			//4 start, 4 end, 1 major type
			//2 len, 1 minor type, 1 target id
			//2 params, 1 crc
			ipcMessage testMessage;
			testMessage.messageID = 500;
			testMessage.majorType = IPC_REQUEST;
			testMessage.minorType = SUBSCRIBE_INTERNAL_SENSOR;
			testMessage.targetID = INT_SENSOR_MAN_M0;
			testMessage.configParams = messageParameters;
			testMessage.nparams = 3;
			testMessage.length = 4;
			//ipcSend(&testMessage, 18);
			messageToBytes(&testMessage, messageBytes);
			UART0Send(messageBytes, 19);
			*/
			//UART0Send(messageBytes, 17);
			//UART0Send(messageBytes+2, 17);

		}
	}
#endif
}
