/*
 * core_network.c
 *
 *  Created on: Oct 11, 2013
 *  Andres
 */

#include <stdlib.h>
#include <stdint.h>
#include "qp_port.h"
#include "apps.h"
//#include "hello.h"
#ifdef DEBUG_UART
#include "utils/uartstdio.h"
#endif
#include "core_network.h"
#include "amulet_sensors.h"
//#include "amulet_ext_sensors.h"
#include "amulet_network.h"
//#include "stress.h"
//#include "bsp_uart.h"
//#include "core_memory.h"
//#include "utils/crc16.h"

//#define RELAY_UART0_TO_UART1

//extern ipcMessage messages[5];
//extern ipcMessageQueue messageQueue;

//extern ipcMessage messages[QLIFO_SIZE];
//extern qlifo queue;

//#define DEBUG_IPC

/**
 * Adds and element at the end of the queue.
 */
void enqueue(ipcMessage * head, ipcMessage * new){
    if(head->next == NULL) {
        head->next = new;
    } else {
        enqueue(head->next, new);
    }
}

int inQueue(ipcMessage * head, uint16_t messageID){
    if(head!=NULL){
        if(head->messageID == messageID){
            return 1;
        } else {
            return(inQueue(head->next, messageID));
        }
    } else {
        return -1;
    }
}


ipcMessage * _removeElement(ipcMessage * head, uint16_t messageID){
    if ((head != NULL) &&(head->messageID == messageID)) {
        return(head->next);
    } else if ((head->next != NULL) && (head->next->messageID == messageID)){
        head->next = head->next->next;
        return(head);
    } else {
        //123456789
        head->next = _removeElement(head->next,messageID);
        //return removeElement(head->next,messageID);
        return(head);
    }
}

void printQueue(ipcMessage * head) {
#ifdef DEBUG_UART
	UARTprintf("Queue: -------------------\r\n");
#endif
	if(head != NULL) {
#ifdef DEBUG_UART
		UARTprintf("%x, %d\r\n", head->messageID, head->minorType);
#endif
		if(head->next != NULL) {
			printQueue(head->next);
		}
	}
}


ipcMessage * removeElement(ipcMessage * head, uint16_t messageID){
	//printQueue(head);
    if(1 == inQueue(head, messageID)) {
        head = _removeElement(head, messageID);
    }
    return head;
}

void processIpc(uint8_t *buffer, uint16_t length){
#ifdef STRESS_APP
	uint8_t i;
#endif
	uint16_t messageID;
	uint16_t majorType = buffer[6];
	uint16_t minorType = buffer[7];
	uint16_t byte8 = buffer[8];
	uint16_t byte9 = buffer[9];
	uint16_t targetID = buffer[10];

	messageID = byte9 << 8;
	messageID = messageID | byte8;
#ifdef DEBUG_IPC
	UARTprintf("MessageID: %d\r\n", messageID);
	UARTprintf("Len: %d\r\n", length);
	UARTprintf("Major: %d\r\n", majorType);
	UARTprintf("Minor: %d\r\n", minorType);
	UARTprintf("Target: %d\r\n", targetID);
	UARTprintf("Params: ");
	for(i=11; i< length -5; i++){ //length - 1crc -4 endframe
		UARTprintf("%X ", buffer[i]);
	}
	UARTprintf("\r\n");
#endif

#ifdef RELAY_UART0_TO_UART1
	UARTprintf((const char *) buffer); //TODO: DISABLE THIS FEATURE ONCE IPC is stable
#endif

	if(IPC_ACK == majorType) {
#ifdef DEBUG_IPC
		UARTprintf("Got an ACK\r\n");
#endif
		AmuletNetworkEvt *ackEvt;
		ackEvt = Q_NEW(AmuletNetworkEvt, INCOMMING_ACK_SIG);
		ackEvt->messageID = messageID;
		QACTIVE_POST(AO_AmuletNetwork, &ackEvt->super, NULL);
	} else if(IPC_EVENT == majorType)  {
		if(EXT_SENSOR_MAN_M4 == targetID) {
			if(EXTERNAL_SENSOR_DATA  == minorType){
#ifdef DEBUG_IPC
				UARTprintf("Got sensor data!\r\n");
#endif
				//Create an event and post it to the ExternalSensorManager
#ifdef STRESS_APP
				uint8_t * sensorData = malloc(length - 16);
				for(i=11; i< length -5; i++){ //length - 1crc -4 endframe
					sensorData[i-11] = buffer[i];
				}
				ExternalSensorsEvt *receivedDataEvt;
				receivedDataEvt = Q_NEW(ExternalSensorsEvt, DEVICE_DATA_SIG);
				receivedDataEvt->sensor_id = buffer[11];
				receivedDataEvt->length = length - 16;//nparams 8 special symbols and 8 bytes with other info
				receivedDataEvt->sensor_type =  buffer[11];
				receivedDataEvt->data = sensorData;
				QACTIVE_POST(AO_Stress , &receivedDataEvt->super, NULL);
#endif
			}
		}
	}


}


uint8_t calculateLRC(const uint8_t *buffer, uint8_t offset, uint16_t length) {
	uint8_t checksum = 0;
	while (length > offset) {
		checksum = checksum + *buffer++;
		length = length - 1;
	}
	checksum ^= 0xFF;
	checksum++;
	return(0xFF);
}

void messageToBytes(ipcMessage *message, uint8_t *bytes) {
	//uint8_t start[4] = { '@', '@', '@', '@' };
	//uint8_t end[4] = { '&', '&', '&', '&' };
	uint16_t i = 0;
	uint16_t length = 8 + message->nparams;
	bytes[0] = '@';
	bytes[1] = '@';
	bytes[2] = '@';
	bytes[3] = '@';
	bytes[4] = (uint8_t) length;
	bytes[5] = (uint8_t) (length >> 8);
	bytes[6] = message->majorType;
	bytes[7] = message->minorType;
	bytes[8] = (uint8_t) message->messageID;
	bytes[9] = (uint8_t) (message->messageID >> 8);
	bytes[10] = message->targetID;
	for(i = 0; i<message->nparams; i++){
		bytes[i+11] = message->configParams[i];
	}

	//CRC  includes minor_type, params, target_id

	bytes[10+message->nparams + 1] = '%';//(uint8_t) crc16_compute(bytes, 10 + message->length, 0);//calculateLRC(bytes, 7, 7 + message.length);
	bytes[10+message->nparams + 2] = '&';
	bytes[10+message->nparams + 3] = '&';
	bytes[10+message->nparams + 4] = '&';
	bytes[10+message->nparams + 5] = '&';
}

void ipcSend(ipcMessage * message){
	//messageQueue.nMessages++;
	///uint32_t lenWithSpecialChars;
	uint8_t messageBytes[MAX_IPC_MESSAGE_SIZE];
	messageToBytes(message, messageBytes);
	///lenWithSpecialChars = 16+message->nparams;
	///UART0Send(messageBytes, lenWithSpecialChars);
}

