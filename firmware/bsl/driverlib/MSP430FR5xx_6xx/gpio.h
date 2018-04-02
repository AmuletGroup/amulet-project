/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#ifndef __MSP430WARE_GPIO_H__
#define __MSP430WARE_GPIO_H__
//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
// The reason for only including Ports 1,2,A is that most of the others are the exact
//  same format with exceptions specific to your device.  It is up to the user
//  to make sure the ports are available by looking in the datasheet for their device.
//  These API's will work, however; ports that are not fully implemented will not work.
//
//*****************************************************************************
#define __MSP430_HAS_PORT1_R__        /* Definition to show that Module is available */
#define __MSP430_HAS_PORT2_R__        /* Definition to show that Module is available */
#define __MSP430_HAS_PORTA_R__        /* Definition to show that Module is available */


#include "inc/hw_regaccess.h"

//*****************************************************************************
//
//The following are values that can be passed to the all GPIO module API
//as the selectedPort parameter.
//
//*****************************************************************************
#define GPIO_PORT_P1     	1
#define GPIO_PORT_P2     	2
#define GPIO_PORT_P3     	3
#define GPIO_PORT_P4    	4
#define GPIO_PORT_P5     	5
#define GPIO_PORT_P6     	6
#define GPIO_PORT_P7     	7
#define GPIO_PORT_P8    	8
#define GPIO_PORT_P9     	9
#define GPIO_PORT_P10       10
#define GPIO_PORT_P11    	11
#define GPIO_PORT_PA    	12
#define GPIO_PORT_PB     	13
#define GPIO_PORT_PC    	14
#define GPIO_PORT_PD    	15
#define GPIO_PORT_PE     	16
#define GPIO_PORT_PF    	17
#define GPIO_PORT_PJ    	18


//*****************************************************************************
//
//The following are values that can be passed to the all GPIO module API
//as the selectedPin parameter.
//
//*****************************************************************************
#define GPIO_PIN0    (0x0001)
#define GPIO_PIN1    (0x0002)
#define GPIO_PIN2    (0x0004)
#define GPIO_PIN3    (0x0008)
#define GPIO_PIN4    (0x0010)
#define GPIO_PIN5    (0x0020)
#define GPIO_PIN6    (0x0040)
#define GPIO_PIN7    (0x0080)
#define GPIO_PIN8    (0x0100)
#define GPIO_PIN9    (0x0200)
#define GPIO_PIN10   (0x0400)
#define GPIO_PIN11   (0x0800)
#define GPIO_PIN12   (0x1000)
#define GPIO_PIN13   (0x2000)
#define GPIO_PIN14   (0x4000)
#define GPIO_PIN15   (0x8000)

//*****************************************************************************
//
//The following are values that may be returned by the GPIO_getInputPinValue()
//API.
//
//*****************************************************************************
#define GPIO_INPUT_PIN_HIGH (0x01)
#define GPIO_INPUT_PIN_LOW  (0x00)

//*****************************************************************************
//
//The following are values that can be passed to the GPIO_interruptEdgeSelect()
//API as the edgeSelect parameter.
//
//*****************************************************************************
#define GPIO_HIGH_TO_LOW_TRANSITION (0x01)
#define GPIO_LOW_TO_HIGH_TRANSITION (0x00)

//*****************************************************************************
//
//The following are values that can be passed to the
//  GPIO_setAsPeripheralModuleFunctionOutputPin() or GPIO_setAsPeripheralModuleFunctionInputPin() API
//  as the mode parameter.
//
//*****************************************************************************
#define GPIO_PRIMARY_MODULE_FUNCTION 		(0x01)
#define GPIO_SECONDARY_MODULE_FUNCTION 	(0x02)
#define GPIO_TERNARY_MODULE_FUNCTION 		(0x03)

//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern void GPIO_setAsOutputPin (uint8_t selectedPort,
								uint16_t selectedPin);

extern void GPIO_setAsInputPin (uint8_t selectedPort,
								uint16_t selectedPin);

extern void GPIO_setOutputHighOnPin ( uint8_t selectedPort,
										uint16_t selectedPin);

extern void GPIO_setOutputLowOnPin (uint8_t selectedPort,
										uint16_t selectedPin);

extern void GPIO_toggleOutputOnPin (uint8_t selectedPort,
									uint16_t selectedPin);

extern void GPIO_setAsInputPinWithPullDownresistor (uint8_t selectedPort,
													uint16_t selectedPin);

extern void GPIO_setAsInputPinWithPullUpresistor (uint8_t selectedPort,
													uint16_t selectedPin);

extern void GPIO_enableInterrupt (uint8_t selectedPort,
									uint16_t selectedPins);

extern void GPIO_disableInterrupt (uint8_t selectedPort,
									uint16_t selectedPins);

extern void GPIO_clearInterruptFlag (uint8_t selectedPort,
										uint16_t selectedPins);

extern uint16_t GPIO_getInterruptStatus (uint8_t selectedPort,
											uint16_t selectedPins);

extern void GPIO_interruptEdgeSelect ( uint8_t selectedPort,
										uint16_t selectedPins,
										uint8_t edgeSelect);

extern uint8_t GPIO_getInputPinValue (	uint8_t selectedPort,
												uint16_t selectedPin);


extern void GPIO_setAsPeripheralModuleFunctionOutputPin ( 
												uint8_t selectedPort,
												uint16_t selectedPin,
												uint8_t mode);

extern void GPIO_setAsPeripheralModuleFunctionInputPin ( 
												uint8_t selectedPort,
												uint16_t selectedPin,
												uint8_t mode);
#endif
