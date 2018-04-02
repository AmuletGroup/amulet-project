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
#ifndef __MSP430WARE_EUSCI_B_SPI_H__
#define __MSP430WARE_EUSCI_B_SPI_H__

//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
//*****************************************************************************
#define __MSP430_HAS_EUSCI_Bx__

#include "inc/hw_regaccess.h"

//*****************************************************************************
//
//The following are values that can be passed to the SPI_masterInit() API
//as the selectClockSource parameter.
//
//*****************************************************************************
#define EUSCI_B_SPI_CLOCKSOURCE_ACLK    UCSSEL__ACLK
#define EUSCI_B_SPI_CLOCKSOURCE_SMCLK   UCSSEL__SMCLK

//*****************************************************************************
//
//The following are values that can be passed to the EUSCI_B_SPI_masterInit() ,
//EUSCI_B_SPI_slaveInit() API as the msbFirst parameter.
//
//*****************************************************************************
#define EUSCI_B_SPI_MSB_FIRST    UCMSB
#define EUSCI_B_SPI_LSB_FIRST    0x00

//*****************************************************************************
//
//The following are values that can be returned by the EUSCI_B_SPI_isBusy() API
//
//*****************************************************************************
#define EUSCI_B_SPI_BUSY        UCBUSY
#define EUSCI_B_SPI_NOT_BUSY    0x00

//*****************************************************************************
//
//The following are values that can be passed to the EUSCI_B_SPI_masterInit() ,
//EUSCI_B_SPI_slaveInit() API as the clockPhase parameter.
//
//*****************************************************************************
#define EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT    0x00
#define EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT    UCCKPH

//*****************************************************************************
//
//The following are values that can be passed to the EUSCI_B_SPI_masterInit() ,
//EUSCI_B_SPI_slaveInit as the spiMode parameter.
//
//*****************************************************************************
#define EUSCI_B_SPI_3PIN                      UCMODE_0
#define EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_HIGH   UCMODE_1
#define EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW    UCMODE_2

//*****************************************************************************
//
//The following are values that can be passed to the EUSCI_B_SPI_masterInit() ,
//EUSCI_B_SPI_slaveInit() API as the clockPolarity parameter.
//
//*****************************************************************************
#define EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH    UCCKPL
#define EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW     0x00

//*****************************************************************************
//
//The following are values that can be passed to the EUSCI_B_SPI_enableInterrupt() ,
//EUSCI_B_SPI_disableInterrupt(), EUSCI_B_SPI_getInterruptStatus(),  API as the mask parameter.
//
//*****************************************************************************
#define EUSCI_B_SPI_TRANSMIT_INTERRUPT    UCTXIE
#define EUSCI_B_SPI_RECEIVE_INTERRUPT     UCRXIE

//*****************************************************************************
//
//The following are values that can be passed to the 
//EUSCI_B_SPI_select4PinFunctionality() API as the select4PinFunctionality parameter.
//
//*****************************************************************************
#define EUSCI_B_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE			UCSTEM
#define EUSCI_B_SPI_PREVENT_CONFLICTS_WITH_OTHER_MASTERS	0x00

//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern unsigned short EUSCI_B_SPI_masterInit (uint32_t baseAddress,
    uint8_t selectClockSource,
    uint32_t clockSourceFrequency,
    uint32_t desiredSpiClock,
    uint16_t msbFirst,
    uint16_t clockPhase,
    uint16_t clockPolarity,
    uint16_t spiMode
    );
extern void EUSCI_B_SPI_select4PinFunctionality (uint32_t baseAddress,
    uint8_t select4PinFunctionality
    );
extern void EUSCI_B_SPI_masterChangeClock (uint32_t baseAddress,
    uint32_t clockSourceFrequency,
    uint32_t desiredSpiClock
    );

extern unsigned short EUSCI_B_SPI_slaveInit (uint32_t baseAddress,
    uint16_t msbFirst,
    uint16_t clockPhase,
    uint16_t clockPolarity,
	uint16_t spiMode
    );
extern void EUSCI_B_SPI_changeClockPhasePolarity (uint32_t baseAddress,
    uint16_t clockPhase,
    uint16_t clockPolarity
    );
extern void EUSCI_B_SPI_transmitData ( uint32_t baseAddress,
    uint8_t transmitData
    );

extern uint8_t EUSCI_B_SPI_receiveData (uint32_t baseAddress);
extern void EUSCI_B_SPI_enableInterrupt (uint32_t baseAddress,
    uint8_t mask
    );
extern void EUSCI_B_SPI_disableInterrupt (uint32_t baseAddress,
    uint8_t mask
    );
extern uint8_t EUSCI_B_SPI_getInterruptStatus (uint32_t baseAddress,
    uint8_t mask
    );
extern void EUSCI_B_SPI_enable (uint32_t baseAddress);
extern void EUSCI_B_SPI_disable (uint32_t baseAddress);
extern uint32_t EUSCI_B_SPI_getReceiveBufferAddressForDMA
    (uint32_t baseAddress);
extern uint32_t EUSCI_B_SPI_getTransmitBufferAddressForDMA
    (uint32_t baseAddress);
extern uint16_t EUSCI_B_SPI_isBusy (uint32_t baseAddress);
extern void EUSCI_B_SPI_clearInterruptFlag (uint32_t baseAddress,
    uint8_t mask
    );
#endif

