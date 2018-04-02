/*
 * core_network.h
 *
 *  Created on: Oct 11, 2013
 *      Author: Andres
 */

#ifndef CORE_NETWORK_H_
#define CORE_NETWORK_H_

#include <stdint.h>

#include "amulet_network.h"

#define MAX_IPC_MESSAGE_SIZE  32

#define QLIFO_SIZE 5



//Major types
#define IPC_REQUEST 40 //M4 to M0
#define IPC_EVENT 1  //M0 to M4
#define IPC_NACK 2  //both directions
#define IPC_ACK 3   //both directions

//#define REQ  0x01
//#define ACK  0x02
//#define NACK 0x03
//#define EVT  0x04

//Minor types

//#define SUBSCRIBE_INTERNAL_SENSOR  0xA0
//#define INTERNAL_SENSOR_DATA  0xB0

// M4 to M0:
#define SUBSCRIBE_INTERNAL_SENSOR 128
#define SUBSCRIBE_EXTERNAL_SENSOR 129
#define SUBSCRIBE_ACTUATOR 130
#define SUBSCRIBE_SMARTPHONE 131
#define SEND_ANT_MESSAGE 132
#define FIND_SENSOR 133
// M0 to M4:
#define INTERNAL_SENSOR_DATA 192
#define EXTERNAL_SENSOR_DATA 193
#define EXTERNAL_ACTUATOR_DATA 194
#define EXTERNAL_SMARTPHONE_DATA 195
#define SENSOR_CHANNEL_DROPPED 196
#define ACTUATOR_CHANNEL_DROPPED 197
#define SMARTPHONE_CHANNEL_DROPPED 198
#define FOUND_SENSOR 199

//#define LEDON   0x01
//#define LEDOFF  0x02

//#define DISPLAY_TEXT_ON_SCREEN
//#define DISPLAY_IMAGE_ON_SCREEN
//#define CLEAR_SCREEN

//#define FIND_ACTUATOR
//#define BIND_ACTUATOR
//#define ACTUATE

//#define FIND_SENSOR
//#define SUBSCRIBE_EXT_SENSOR





//Target IDs

//M4 Service Managers
#define INT_SENSOR_MAN_M4  0x10
#define INT_ACTUATOR_MAN_M4  0x11
#define EXT_SENSOR_MAN_M4  0x12
#define EXT_ACTUATOR_MAN_M4  0x13
#define NETWORK_MAN_M4  0x14
#define MEMORY_MAN_M4  0x15
#define KEY_MAN_M4  0x16
#define AUTH_MAN_M4  0x17
#define TIME_MAN_M4  0x18
#define CRYPTO_MAN_M4  0x19

//M0 Service Managers

#define INT_SENSOR_MAN_M0  0x01
#define INT_ACTUATOR_MAN_M0  0x02
#define EXT_SENSOR_MAN_M0  0x03
#define EXT_ACTUATOR_MAN_M0  0x04
#define NETWORK_MAN_M0  0x05
#define MEMORY_MAN_M0  0x06
#define KEY_MAN_M0  0x07
#define AUTH_MAN_M0  0x08
#define TIME_MAN_M0  0x09
#define CRYPTO_MAN_M0  0x0A

typedef struct ipcMessageQueue ipcMessage;

struct ipcMessageQueue {
	uint8_t messageStart[4]; // {'@', '@', '@', '@'};
	uint8_t majorType;
	uint8_t minorType;
	uint8_t targetID;
	uint8_t *configParams;
	uint8_t nparams;
	uint16_t messageID;
	uint8_t checksum;
	uint8_t messagEnd[4];  // {'&', '&', '&', '&'};
    ipcMessage * next;
};

/**
typedef struct {
	uint8_t *messageStart; // {'@','@','@','@'};
	uint8_t majorType;
	uint8_t minorType;
	uint8_t targetID;
	uint16_t nparams;
	uint8_t* configParams;
	uint16_t messageID;
	uint8_t checksum;
	uint8_t *messagEnd;  // {'&', '&', '&', '&'};
} ipcMessage;

typedef struct {
	ipcMessage *messages;
	uint8_t nMessages;
} ipcMessageQueue;

typedef struct {
	ipcMessage * messages;
    uint8_t nmessages;
    uint8_t head;
} qlifo;

*/

/**
 * Adds and element at the end of the queue.
 */
void enqueue(ipcMessage * head, ipcMessage * new);

int inQueue(ipcMessage * head, uint16_t messageID);

void printQueue(ipcMessage * head);

ipcMessage * removeElement(ipcMessage * head, uint16_t messageID);

uint8_t calculateLRC(const uint8_t *buffer, uint8_t offset, uint16_t length);

void messageToBytes(ipcMessage *message, uint8_t *bytes);

void ipcSend(ipcMessage *message);

void processIpc(uint8_t *buffer, uint16_t length);

/**
 * Action: Create a directed event to caller_id that includes the list of all devices that have ever been paired with the amulet
Response event: NETWORK_DEVICES(list)
Where the list includes:
device_id, amulet_device_type, online_status
Note that because amulet needs to know when devices are available, amulet devices need to have a heart beat that lets the amulet register devices that are available.
 *
 */
void ListDevices(uint8_t caller_id);

/**
 * Action: Determine if the device with device_id is in range and available. Create an directed event to caller_id with the result.
Response event: DEVICE_STATUS(result)
 */
void FindDevice(uint32_t device_id, uint8_t caller_id);

/**
 * Action: Configure the device (e.g., set the frequency, sensitivity, etc.). This in particular can be used to request that a device starts streaming sensor data.
Response event: CONFIG_DEVICE_RESULT(result)
 */
void ConfigDevice(uint32_t device_id, uint32_t * params, uint8_t nparams, uint8_t caller_id);

/**
 * Action: Amulet accumulates window_size tuples corresponding to data from the device with ID device_id. It then makes available the dataset to the caller_id. Amulet notifies caller_id via an event when a window of data is ready.
Response event: DEVICE_DATA(dataset,caller_id)
The amulet will notify an app when the subscription can no longer be served (e.g., because a device gets out of range.)
Response event: DEVICE_UNAVAIL(device_id)
 */
void SubsDevice(uint32_t device_id, uint32_t expiration, uint8_t window_size, uint8_t caller_id);



#endif /* CORE_NETWORK_H_ */
