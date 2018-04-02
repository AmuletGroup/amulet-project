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
#ifndef __MSP430WARE_CS_H__
#define __MSP430WARE_CS_H__

//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
//*****************************************************************************
#define __MSP430_HAS_SFR__
#define __MSP430_HAS_CS__

#include "inc/hw_regaccess.h"

//*****************************************************************************
//
//The following are values that can be passed to the CS_clockSignalInit() API
//as the selectedClockSignal parameter.
//
//*****************************************************************************
#define CS_ACLK                0x01
#define CS_MCLK                0x02
#define CS_SMCLK               0x04

//*****************************************************************************
//
//The following along with CS_ACLK, CS_MCLK, CS_SMCLK may be passed to the
//CS_clockRequestEnable() and CS_clockRequestDisable() API
//as the selectClock parameter.
//
//*****************************************************************************
#define CS_MODOSC              MODCLKREQEN

//*****************************************************************************
//
//The following are values that can be passed to the CS_clockSignalInit() API
//as the clockSource parameter. CS_VLOCLK_SELECT may not be used for
//selectedClockSignal CS_FLLREF
//
//*****************************************************************************
#define CS_VLOCLK_SELECT        SELM__VLOCLK
#define CS_DCOCLK_SELECT        SELM__DCOCLK
#define CS_LFXTCLK_SELECT		SELM__LFXTCLK
#define CS_HFXTCLK_SELECT		SELM__HFXTCLK
#define CS_LFMODOSC_SELECT		SELM__LFMODOSC
#define CS_MODOSC_SELECT		SELM__MODOSC

//*****************************************************************************
//
//The following are values that can be passed to the CS_clockSignalInit() API
//as the clockSourceDivider parameter.
//
//*****************************************************************************
#define CS_CLOCK_DIVIDER_1     DIVM__1
#define CS_CLOCK_DIVIDER_2     DIVM__2
#define CS_CLOCK_DIVIDER_4     DIVM__4
#define CS_CLOCK_DIVIDER_8     DIVM__8
#define CS_CLOCK_DIVIDER_16    DIVM__16
#define CS_CLOCK_DIVIDER_32    DIVM__32


//*****************************************************************************
//
//The following are values that can be passed to the CS_LFXTStart(),
//CS_LFXTStartWithTimeout(), as the lfxtdrive parameter.
//
//*****************************************************************************
#define CS_LFXT_DRIVE0	LFXTDRIVE_0
#define CS_LFXT_DRIVE1	LFXTDRIVE_1
#define CS_LFXT_DRIVE2	LFXTDRIVE_2
#define CS_LFXT_DRIVE3	LFXTDRIVE_3


//*****************************************************************************
//
//The following are values that can be passed to the CS_SetDCOFreq() API
//as the dcorsel parameter.
//
//*****************************************************************************
#define CS_DCORSEL_0	0x00
#define CS_DCORSEL_1	DCORSEL


//*****************************************************************************
//
//The following are values that can be passed to the CS_SetDCOFreq() API
//as the dcofsel parameter.
//
//*****************************************************************************
#define CS_DCOFSEL_0	DCOFSEL_0
#define CS_DCOFSEL_1	DCOFSEL_1
#define CS_DCOFSEL_2	DCOFSEL_2
#define CS_DCOFSEL_3	DCOFSEL_3
#define CS_DCOFSEL_4	DCOFSEL_4
#define CS_DCOFSEL_5	DCOFSEL_5
#define CS_DCOFSEL_6	DCOFSEL_6

//*****************************************************************************
//
//FOR FR57xx DEVICES. The following are values can be passed to CS_XT2_Start
//and CS_XT2_StartWithTimeOut as the hfxtdtive parameter.
//
//*****************************************************************************
#define CS_HFXTDRIVE_4MHZ_8MHZ       HFXTDRIVE_0
#define CS_HFXTDRIVE_8MHZ_16MHZ      HFXTDRIVE_1
#define CS_HFXTDRIVE_16MHZ_24MHZ     HFXTDRIVE_2
#define CS_HFXTDRIVE_24MHZ_32MHZ     HFXTDRIVE_3


//*****************************************************************************
//
// The following are values can be passed to CS_HFXT_Start
//and CS_HFXT_StartWithTimeOut as the hfxtdtive parameter.
//
//*****************************************************************************
#define CS_HFXTDRIVE_0     HFXTDRIVE_0
#define CS_HFXTDRIVE_1     HFXTDRIVE_1
#define CS_HFXTDRIVE_2     HFXTDRIVE_2
#define CS_HFXTDRIVE_3     HFXTDRIVE_3


//*****************************************************************************
//
//The following are values that can be passed to the CS_faultFlagStatus() and
//CS_clearFaultFlag API as the mask parameter.
//
//*****************************************************************************
#define CS_XT2OFFG   XT2OFFG
#define CS_XT1OFFG   XT1OFFG

//*****************************************************************************
//
//FOR WOLVERINE DEVICES. The following are values that can be passed to the
//CS_faultFlagStatus and CS_clearFaultFlag as the mask parameter.
//
//*****************************************************************************
#define CS_LFXTOFFG		LFXTOFFG
#define CS_HFXTOFFG		HFXTOFFG

//*****************************************************************************
//
//FOR FR57xx devices. The following are values that can be passed to the
// CS_LFXT1Start() and CS_LFXT1StartWithTimeout() API as the lfxtdrive parameter.
//
//*****************************************************************************
#define CS_XT1DRIVE_0	XT1DRIVE_0
#define CS_XT1DRIVE_1	XT1DRIVE_1
#define CS_XT1DRIVE_2	XT1DRIVE_2
#define CS_XT1DRIVE_3	XT1DRIVE_3

//*****************************************************************************
//
//FOR FR57xx devices. The following are values that can be passed to the
// CS_LFXT1Start() and CS_LFXT1StartWithTimeout() API as the lfxtdrive parameter.
//
//*****************************************************************************
#define CS_LFXTDRIVE_0	LFXTDRIVE_0
#define CS_LFXTDRIVE_1	LFXTDRIVE_1
#define CS_LFXTDRIVE_2	LFXTDRIVE_2
#define CS_LFXTDRIVE_3	LFXTDRIVE_3

//*****************************************************************************
//
//Internal very low power VLOCLK, low frequency oscillator with
//10 kHz typical frequency
//Internal low-power oscillator MODCLK with 5 MHz typical 
//frequency and LFMODCLK is MODCLK divided by 128.
//
//
//*****************************************************************************
#define CS_VLOCLK_FREQUENCY	10000
#define CS_MODCLK_FREQUENCY	5000000
#define CS_LFMODCLK_FREQUENCY	39062


//*****************************************************************************
//
//The following value is used by CS_XT1Start, CS_bypassXT1,
//CS_XT1StartWithTimeout, CS_bypassXT1WithTimeout on FR57xx devices to properly
//set the XTS bit. This frequnecy threshold is specified in the FR5xx
//User's Guide
//
//*****************************************************************************

#define LFXT_FREQUENCY_THRESHOLD		50000


//*****************************************************************************
//
//The following value is used by CS_getACLK, CS_getSMCLK, CS_getMCLK to
//determine the operating frequency based on the available DCO frequencies for
//FR58xx_FR59xx devices
//
//*****************************************************************************
#define CS_DCO_FREQ_1 	1000000
#define CS_DCO_FREQ_2 	2670000
#define CS_DCO_FREQ_3 	3330000
#define CS_DCO_FREQ_4 	4000000
#define CS_DCO_FREQ_5 	5330000
#define CS_DCO_FREQ_6 	6670000
#define CS_DCO_FREQ_7 	8000000
#define CS_DCO_FREQ_8 	16000000
#define CS_DCO_FREQ_9 	20000000
#define CS_DCO_FREQ_10 	24000000

//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern void
CS_setExternalClockSource (uint32_t baseAddress,
    uint32_t LFXT_LFXT_CLK_frequency,
    uint32_t HFXT_HFXT_CLK_frequency
    );
extern void
CS_clockSignalInit ( uint32_t baseAddress,
    uint8_t selectedClockSignal,
    uint16_t clockSource,
    uint16_t clockSourceDivider
    );

extern void
CS_XT1Start ( uint32_t baseAddress,
    uint16_t lfxtdrive
    );

extern void
CS_LFXTStart(
		uint32_t baseAddress,
	    uint16_t lfxtdrive
		);

extern void
CS_bypassXT1 ( uint32_t baseAddress
    );

extern void
CS_bypassLFXT ( uint32_t baseAddress
    );


extern unsigned short
CS_bypassXT1WithTimeout (
    uint32_t baseAddress,
    uint32_t timeout
    );

extern unsigned short
CS_bypassLFXTWithTimeout (
    uint32_t baseAddress,
    uint32_t timeout
    );

extern unsigned short
CS_XT1StartWithTimeout (
    uint32_t baseAddress,
    uint16_t lfxtdrive,
    uint32_t timeout
    );

unsigned short
CS_LFXTStartWithTimeout(
		uint32_t baseAddress,
	    uint16_t lfxtdrive,
	    uint32_t timeout
		);

extern void
CS_XT1Off (uint32_t baseAddress);

extern void
CS_LFXTOff (uint32_t baseAddress);

extern void CS_XT2Start (  uint32_t baseAddress,
    uint16_t hfxtdtive
    );

extern void CS_HFXTStart (  uint32_t baseAddress,
    uint16_t hfxtdtive
    );

extern void CS_bypassXT2 (  uint32_t baseAddress );

extern void CS_bypassHFXT (  uint32_t baseAddress );

extern unsigned short
CS_XT2StartWithTimeout ( uint32_t baseAddress,
    uint16_t hfxtdtive,
    uint32_t timeout
    );

extern unsigned short
CS_HFXTStartWithTimeout ( uint32_t baseAddress,
    uint16_t hfxtdrive,
    uint32_t timeout
    );

extern unsigned short
CS_bypassXT2WithTimeout ( uint32_t baseAddress,
    uint32_t timeout
    );

extern unsigned short
CS_bypassHFXTWithTimeout ( uint32_t baseAddress,
    uint32_t timeout
    );

extern void
CS_XT2Off (uint32_t baseAddress);

extern void
CS_HFXTOff (uint32_t baseAddress);


extern void CS_enableClockRequest (    uint32_t baseAddress,
    uint8_t selectClock
    );
extern void CS_disableClockRequest (
    uint32_t baseAddress,
    uint8_t selectClock
    );
extern uint8_t CS_faultFlagStatus (
    uint32_t baseAddress,
    uint8_t mask
    );

extern void CS_clearFaultFlag (
    uint32_t baseAddress,
    uint8_t mask
    );

extern uint32_t
CS_getACLK (
		uint32_t baseAddress);

extern uint32_t
CS_getSMCLK (
		uint32_t baseAddress);

extern uint32_t
CS_getMCLK (
		uint32_t baseAddress);

extern uint16_t
CS_clearAllOscFlagsWithTimeout(
		uint32_t baseAddress,
        uint32_t timeout);

extern void
CS_VLOoff(uint32_t baseAddress);

extern void
CS_setDCOFreq(
		uint32_t baseAddress,
		uint16_t dcorsel,
		uint16_t dcofsel);

#endif
