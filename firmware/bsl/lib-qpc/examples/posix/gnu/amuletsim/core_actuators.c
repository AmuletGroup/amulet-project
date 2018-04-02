/*
 * core_actuators.c
 *
 *  Created on: Oct 11, 2013
 *      Author: Andres
 */
#include <stdint.h>
#include <stdio.h>
#ifdef DEBUG_UART
#include "utils/uartstdio.h"
#endif
#include "core_actuators.h"
//#include "bsp_actuators.h"
//#include "bsp_uart.h"
//#include "core_memory.h"

void LedOn(uint8_t led_id){
	//TurnLEDOn(led_id);
    printf("LedOn\n");
}

void LedOff(uint8_t led_id){
	//TurnLEDOff(led_id);
    printf("LedOff\n");
}
/**
 * This will eventually display the messages in the SHARPS deisplay
 * for now it only sends a message to the debugging UART
 */
void DisplayMessage(uint8_t *message, uint32_t len){
	//UART0Send(message, len);
	//TestWrite();
#ifdef DEBUG_UART
	UARTprintf((const char* )message);
#else
	//UART0Send(message, len);
#endif

}
