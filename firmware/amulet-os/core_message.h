/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */


/**
 *  @file core_message.h
 *  @brief File for IPC message format
 *
 *  This function constans function about IPC byte stream conversion, also serve a 
 *  middleware of core and bsp. Is also has the IPC message queue management
 *  functions
 *
 */

#ifndef CORE_NETWORK_H_
#define CORE_NETWORK_H_

#include "amulet.h"

#define MAX_IPC_MESSAGE_SIZE  32
#define IPC_CIRCULAR_SIZE 10 // The size of circular buffer hold outbound IPC 

//Major types
#define IPC_REQUEST 40 //M4 to M0
#define IPC_EVENT 1  //M0 to M4
#define IPC_NACK 2  //both directions
#define IPC_ACK 3   //both directions

// The folloing meta data is in IPC message, details at:
// https://docs.google.com/document/d/1Z0go9Bf2XbA3kBXXa-9VGm-7Ev9-9kZ4_3t6pQn_JIY

//Minor types
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

/**
 * This ipcMessageQueue defines the data structure of each item in a circular buffer
 */
struct ipcMessageQueue {
	uint8_t messageStart[4]; // {'@', '@', '@', '@'};
	uint8_t majorType;
	uint8_t minorType;
	uint8_t targetID;
	uint8_t configParams[8];
	uint8_t nparams;
	uint16_t messageID;
	uint8_t checksum;
	uint8_t messagEnd[4];  // {'&', '&', '&', '&'};
};

ipcMessage circularIPCMsg[IPC_CIRCULAR_SIZE];

/**
 * Serialize the IPC data to bytes.
 *
 * @param message The IPC data structure
 * @param bytes serialized data
 *
 * @return None.
 */
void MessageToBytes(ipcMessage *message, uint8_t *bytes);

/**
 * Convert raw IPC data to ipcMessage data structure.
 *
 * @param message The IPC data structure
 * @param bytes serialized data
 *
 * @return None.
 */
void BytesToBuffer(ipcMessage *message, uint8_t *bytes);

/**
 * Originally used to serialize IPC data structure into bytes stream
 * currently, not in use, check the implentation for details
 *
 * @param message The IPC data structure
 *
 * @return None.
 */
void ipcSend(ipcMessage *message);

/**
 * Process IPC message from the radio board based on IPC message meta data.
 * Based on the message header, forward to corresponding services, currently
 * only the NotifyApp service is used, as all the messages are for the Apps
 *
 * @param buffer The pointer to the global buffer that store the received data from IPC 
 * @param length As we use a global buffer, but the length of each IPC message could be very different
 *
 * @return None.
 */
void ProcessIPC(uint8_t *buffer, uint16_t length);

/**
 * Put outbound IPC message to a queue. The queue is a global variable and a circular 
 * buffer, the longest size of the buffer is defined as IPC_CIRCULAR_SIZE(10)
 * Currently, the new M0 software does not take any incomming message, so this
 * function does not have any effect.
 *
 * @param sensor_id The sensor to subscribe to (defined in amulet.h).
 * @param expiration (depreciated) The Sensor Manager will stop sending data and events after this expiration (in seconds).
 * @param window_size (depreciated) The sampling window size (in seconds).
 * @param window_interval (depreciated) The sampling interval (in seconds); must be greater than or equal to the window_size.
 * @param majorType The type of the message, mainly for directions
 * @param minorType The type of the message, like data/command etc.
 * @param nparams currently not used
 * @param messageID ID of the message, currently not useful
 *
 * @return None.
 */
void enqueueIPC(uint8_t sensor_id, uint16_t expiration, 
  uint16_t window_size, uint16_t window_interval, uint8_t requestor,
  uint8_t majorType, uint8_t minorType, uint8_t targetID, uint8_t nparams,
	uint16_t messageID);

#endif /* CORE_NETWORK_H_ */
