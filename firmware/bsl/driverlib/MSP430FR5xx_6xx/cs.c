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
//******************************************************************************
//
//cs.c - Driver for the CS Module.
//
//******************************************************************************

#include "inc/hw_regaccess.h"
#include "inc/hw_memmap.h"
#include "cs.h"
/*#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430xgeneric.h"
#elif defined(__TI_COMPILER_VERSION__)
#include "deprecated/CCS/msp430xgeneric.h"
#elif defined(__GNUC__)
#include <msp430xgeneric.h>
#endif*/

#define OFS_CSCTL0            (0x0000)  /* CS Control Register 0 */
#define OFS_CSCTL0_L           OFS_CSCTL0
#define OFS_CSCTL0_H           OFS_CSCTL0+1
#define OFS_CSCTL1            (0x0002)  /* CS Control Register 1 */
#define OFS_CSCTL1_L           OFS_CSCTL1
#define OFS_CSCTL1_H           OFS_CSCTL1+1
#define OFS_CSCTL2            (0x0004)  /* CS Control Register 2 */
#define OFS_CSCTL2_L           OFS_CSCTL2
#define OFS_CSCTL2_H           OFS_CSCTL2+1
#define OFS_CSCTL3            (0x0006)  /* CS Control Register 3 */
#define OFS_CSCTL3_L           OFS_CSCTL3
#define OFS_CSCTL3_H           OFS_CSCTL3+1
#define OFS_CSCTL4            (0x0008)  /* CS Control Register 4 */
#define OFS_CSCTL4_L           OFS_CSCTL4
#define OFS_CSCTL4_H           OFS_CSCTL4+1
#define OFS_CSCTL5            (0x000A)  /* CS Control Register 5 */
#define OFS_CSCTL5_L           OFS_CSCTL5
#define OFS_CSCTL5_H           OFS_CSCTL5+1
#define OFS_CSCTL6            (0x000C)  /* CS Control Register 6 */
#define OFS_CSCTL6_L           OFS_CSCTL6
#define OFS_CSCTL6_H           OFS_CSCTL6+1

#define OFS_SFRIFG1           (0x0002)  /* Interrupt Flag 1 */
#define OFS_SFRIFG1_L          OFS_SFRIFG1
#define OFS_SFRIFG1_H          OFS_SFRIFG1+1
 
#include "assert.h"

static uint32_t
privateCSASourceClockFromDCO ( uint32_t baseAddress,
		uint8_t clockdivider);
		
static uint32_t
privateCSAComputeCLKFrequency ( uint32_t baseAddress,
    uint16_t CLKSource,
    uint16_t CLKSourceDivider
    );		
		
//******************************************************************************
//
// LFXT for FR58xx/FR59xx crystal frequency. Should be set with
//CS_externalClockSourceInit if LFXT is used and user intends to invoke
//CS_getSMCLK, CS_getMCLK, CS_getACLK and
//CS_LFXTStart, CS_LFXTByPass, CS_LFXTStartWithTimeout,
//CS_LFXTByPassWithTimeout.
//
//******************************************************************************
uint32_t CS_LFXTClockFrequency = 0;

//******************************************************************************
//
//The HFXT (or HFXT for FR58xx/FR59xx) crystal frequency. Should be set with
//CS_externalClockSourceInit if HFXT is used and user intends to invoke
//CS_getSMCLK, CS_getMCLK, CS_getACLK,
//CS_LFXTStart, CS_LFXTByPass, CS_LFXTStartWithTimeout,
//CS_LFXTByPassWithTimeout.
//
//******************************************************************************
uint32_t CS_HFXTClockFrequency = 0;

//******************************************************************************
//
//! This function sets the external clock sources LFXT and HFXT crystal
//! oscillator frequency values. This function must be called if an external
//! crystal LFXT or HFXT is used and the user intends to call
//! CS_getMCLK, CS_getSMCLK, CS_getACLK and
//! CS_LFXTStart, CS_LFXTByPass, CS_LFXTStartWithTimeout,
//! CS_LFXTByPassWithTimeout, CS_HFXTStart, CS_HFXTByPass,
//! CS_HFXTStartWithTimeout, CS_HFXTByPassWithTimeout.
//!
//! \param baseAddress is the base address of the CSA module.
//! \param LFXTCLK_frequency is the LFXT crystal frequencies in Hz
//! \param HFXTCLK_frequency is the HFXT crystal frequencies in Hz
//!
//! \return None
//
//******************************************************************************
void
CS_setExternalClockSource (uint32_t baseAddress,
    uint32_t LFXTCLK_frequency,
    uint32_t HFXTCLK_frequency
    )
{
    CS_LFXTClockFrequency = LFXTCLK_frequency;
    CS_HFXTClockFrequency = HFXTCLK_frequency;
}


//******************************************************************************
//
//! This function initializes each of the clock signals. The user must ensure
//! that this function is called for each clock signal. If not, the default
//! state is assumed for the particular clock signal. Refer MSP430ware
//! documentation for CSA module or Device Family User's Guide for details of
//! default clock signal states.
//!
//! \param baseAddress is the base address of the CSA module.
//! \param selectedClockSignal - Valid values are
//!           \b CS_ACLK,
//!           \b CS_MCLK,
//!           \b CS_SMCLK,
//! \param clockSource is Clock source for the selectedClock Signal
//!            \b CS_LFXTCLK_SELECT,
//!            \b CS_VLOCLK_SELECT,
//!            \b CS_DCOCLK_SELECT,	[Not available for ACLK]
//!            \b CS_HFXTCLK_SELECT,	[Not available for ACLK]
//!            \b CS_LFMODOSC_SELECT,
//!            \b CS_MODOSC_SELECT		[Not available for ACLK]
//! \param clockSourceDivider - selected the clock divider to calculate
//!         clock signal from clock source. Valid values are
//!           \b CS_CLOCK_DIVIDER_1,	[Default for ACLK]
//!           \b CS_CLOCK_DIVIDER_2,
//!           \b CS_CLOCK_DIVIDER_4,
//!           \b CS_CLOCK_DIVIDER_8,	[Default for SMCLK and MCLK]
//!           \b CS_CLOCK_DIVIDER_16,
//!           \b CS_CLOCK_DIVIDER_32
//!
//! Modified registers are \b CSCTL0, \b CSCTL2, \b CSCTL3
//!
//! \return NONE
//
//******************************************************************************
void
CS_clockSignalInit ( uint32_t baseAddress,
    uint8_t selectedClockSignal,
    uint16_t clockSource,
    uint16_t clockSourceDivider
    )
{

	//Verify User has selected a valid Frequency divider
	assert(
			(CS_CLOCK_DIVIDER_1 == clockSourceDivider) ||
			(CS_CLOCK_DIVIDER_2 == clockSourceDivider) ||
			(CS_CLOCK_DIVIDER_4 == clockSourceDivider) ||
			(CS_CLOCK_DIVIDER_8 == clockSourceDivider) ||
			(CS_CLOCK_DIVIDER_16 == clockSourceDivider) ||
			(CS_CLOCK_DIVIDER_32 == clockSourceDivider)
			);


		// Unlock CS control register
	HWREG16(baseAddress + OFS_CSCTL0) = CSKEY;

	switch (selectedClockSignal){
		case CS_ACLK:
				assert(
					(CS_LFXTCLK_SELECT == clockSource)  ||
					(CS_VLOCLK_SELECT == clockSource)   ||
					(CS_LFMODOSC_SELECT == clockSource) 
					);

			clockSourceDivider = clockSourceDivider << 8;
			clockSource = clockSource << 8;

			HWREG16(baseAddress + OFS_CSCTL2) &= ~(SELA_7);
			HWREG16(baseAddress + OFS_CSCTL2) |= (clockSource);
			HWREG16(baseAddress + OFS_CSCTL3) &= ~(DIVA0 + DIVA1 + DIVA2);
			HWREG16(baseAddress + OFS_CSCTL3) |= clockSourceDivider;
			break;
		case CS_SMCLK:
			assert(
				(CS_LFXTCLK_SELECT == clockSource) ||
				(CS_VLOCLK_SELECT == clockSource) ||
				(CS_DCOCLK_SELECT == clockSource) ||
				(CS_HFXTCLK_SELECT == clockSource) ||
				(CS_LFMODOSC_SELECT== clockSource)||
				(CS_MODOSC_SELECT == clockSource)
				);

			clockSource = clockSource << 4;
			clockSourceDivider = clockSourceDivider << 4;

			HWREG16(baseAddress + OFS_CSCTL2) &= ~(SELS_7);
			HWREG16(baseAddress + OFS_CSCTL2) |= clockSource;
			HWREG16(baseAddress + OFS_CSCTL3) &= ~(DIVS0 + DIVS1 + DIVS2);
			HWREG16(baseAddress + OFS_CSCTL3) |= clockSourceDivider;
			break;
		case CS_MCLK:
			assert(
			(CS_LFXTCLK_SELECT == clockSource) ||
			(CS_VLOCLK_SELECT == clockSource) ||
			(CS_DCOCLK_SELECT == clockSource) ||
			(CS_HFXTCLK_SELECT == clockSource) ||
			(CS_LFMODOSC_SELECT== clockSource)||
			(CS_MODOSC_SELECT == clockSource)
			);

			HWREG16(baseAddress + OFS_CSCTL2) &= ~(SELM_7);
			HWREG16(baseAddress + OFS_CSCTL2) |= clockSource;
			HWREG16(baseAddress + OFS_CSCTL3) &= ~(DIVM0 + DIVM1 + DIVM2);
			HWREG16(baseAddress + OFS_CSCTL3) |= clockSourceDivider;
			break;
	}
}

//******************************************************************************
//
//! Initializes the LFXT crystal oscillator in low frequency mode. Loops
//! until all oscillator fault flags are cleared, with no timeout. See the
//! device-specific data sheet for appropriate drive settings. IMPORTANT: User
//! must call CS_setExternalClockSource function to set frequency of external
//! clocks this function is call.
//!
//! \param baseAddress is the base address of the CS module.
//! \param lfxtdrive is the target drive strength for the LFXT crystal oscillator.
//!         Valid values:
//!         \b CS_LFXT_DRIVE0,
//!         \b CS_LFXT_DRIVE1,
//!         \b CS_LFXT_DRIVE2,
//!         \b CS_LFXT_DRIVE3.	[Default]
//! Modified registers are \b CSCTL0, \b CSCTL4, \b CSCTL5 and \b SFRIFG1
//!
//! \return None
//
//******************************************************************************
void
CS_LFXTStart ( uint32_t baseAddress,
    uint16_t lfxtdrive
    )
{
	assert(CS_LFXTClockFrequency !=0);

	    assert((lfxtdrive == CS_LFXT_DRIVE0 ) ||
	        (lfxtdrive == CS_LFXT_DRIVE1 ) ||
	        (lfxtdrive == CS_LFXT_DRIVE2 ) ||
	        (lfxtdrive == CS_LFXT_DRIVE3 ));

		// Unlock CS control register
		HWREG16(baseAddress + OFS_CSCTL0) = CSKEY;

	    //If the drive setting is not already set to maximum
	    //Set it to max for LFXT startup
	    if ((HWREG16(baseAddress + OFS_CSCTL4) & LFXTDRIVE_3) != LFXTDRIVE_3){
	        //Highest drive setting for LFXTstartup
	        HWREG16(baseAddress + OFS_CSCTL4_L) |= LFXTDRIVE1_L + LFXTDRIVE0_L;
	    }

	    HWREG16(baseAddress + OFS_CSCTL4) &= ~LFXTBYPASS;

	    //Wait for Crystal to stabilize
	    while (HWREG8(baseAddress + OFS_CSCTL5) & LFXTOFFG)
	    {
	        //Clear OSC flaut Flags fault flags
	        HWREG8(baseAddress + OFS_CSCTL5) &= ~(LFXTOFFG);

	        //Clear OFIFG fault flag
	        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
	    }


	    //set requested Drive mode
	    HWREG16(baseAddress + OFS_CSCTL4) = ( HWREG16(baseAddress + OFS_CSCTL4) &
	                                         ~(LFXTDRIVE_3)
	                                         ) |
	                                       (lfxtdrive);

	    //Switch ON LFXT oscillator
	    HWREG16(baseAddress + OFS_CSCTL4) &= ~LFXTOFF;

}

//******************************************************************************
//
//! Bypasses the LFXT crystal oscillator. Loops until all oscillator fault
//! flags are cleared, with no timeout. IMPORTANT: User must call
//! CS_setExternalClockSource function to set frequency of external clocks this
//! function is call.
//!
//! \param baseAddress is the base address of the CS module.
//!
//! Modified registers are \b CSCTL0, \b CSCTL4, \b CSCTL5, \b SFRIFG1
//! \return None
//
//******************************************************************************
void
CS_bypassLFXT ( uint32_t baseAddress
    )
{
	//Verify user has set frequency of LFXT with SetExternalClockSource
	assert(CS_LFXTClockFrequency!=0);

	// Unlock CS control register
	HWREG16(baseAddress + OFS_CSCTL0) = CSKEY;


	assert(CS_LFXTClockFrequency<LFXT_FREQUENCY_THRESHOLD);

	// Set LFXT in LF mode Switch off LFXT oscillator and enable BYPASS mode
	HWREG16(baseAddress + OFS_CSCTL4) |= (LFXTBYPASS + LFXTOFF);

	//Wait until LFXT stabilizes
	while (HWREG8(baseAddress + OFS_CSCTL5) & LFXTOFFG)
	{
		//Clear OSC flaut Flags fault flags
		HWREG8(baseAddress + OFS_CSCTL5) &= ~(LFXTOFFG);

		// Clear the global fault flag. In case the LFXT caused the global fault
		// flag to get set this will clear the global error condition. If any
		// error condition persists, global flag will get again.
		HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
	}
}

//******************************************************************************
//
//! Initializes the LFXT crystal oscillator in low frequency mode with timeout.
//! Loops until all oscillator fault flags are cleared or until a timeout
//! counter is decremented and equals to zero. See the device-specific
//! datasheet for appropriate drive settings. IMPORTANT: User
//! must call CS_setExternalClockSource to set frequency of external clocks
//! before calling this function
//!
//! \param baseAddress is the base address of the CS module.
//! \param lfxtdrive is the target drive strength for the LFXT crystal oscillator.
//!         Valid values are
//!         \b CS_LFXTDRIVE0,
//!         \b CS_LFXTDRIVE1,
//!         \b CS_LFXTDRIVE2,
//!         \b CS_LFXTDRIVE3.	[Default]
//! \param timeout is the count value that gets decremented every time the loop
//!         that clears oscillator fault flags gets executed.
//!
//! Modified registers are \b CSCTL0, \b CSCTL4, \b CSCTL5 and \b SFRIFG1
//!
//! \return None
//
//******************************************************************************
unsigned short
CS_LFXTStartWithTimeout (
		uint32_t baseAddress,
		uint16_t lfxtdrive,
		uint32_t timeout
    )
{
	assert(CS_LFXTClockFrequency !=0);

	assert((lfxtdrive == CS_LFXT_DRIVE0 ) ||
		(lfxtdrive == CS_LFXT_DRIVE1 ) ||
		(lfxtdrive == CS_LFXT_DRIVE2 ) ||
		(lfxtdrive == CS_LFXT_DRIVE3 ));

	// Unlock CS control register
	HWREG16(baseAddress + OFS_CSCTL0) = CSKEY;

	//If the drive setting is not already set to maximum
	//Set it to max for LFXT startup
	if ((HWREG16(baseAddress + OFS_CSCTL4) & LFXTDRIVE_3) != LFXTDRIVE_3){
		//Highest drive setting for LFXTstartup
		HWREG16(baseAddress + OFS_CSCTL4_L) |= LFXTDRIVE1_L + LFXTDRIVE0_L;
	}

	HWREG16(baseAddress + OFS_CSCTL4) &= ~LFXTBYPASS;

	while ((HWREG8(baseAddress + OFS_CSCTL5) & LFXTOFFG) && --timeout)
	{
		//Clear OSC fault Flags fault flags
		HWREG8(baseAddress + OFS_CSCTL5) &= ~(LFXTOFFG);

		// Clear the global fault flag. In case the LFXT caused the global fault
		// flag to get set this will clear the global error condition. If any
		// error condition persists, global flag will get again.
		HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

	}

	if(timeout){

	    //set requested Drive mode
		HWREG16(baseAddress + OFS_CSCTL4) = ( HWREG16(baseAddress + OFS_CSCTL4) &
												 ~(LFXTDRIVE_3)
												 ) |
											   (lfxtdrive);
		//Switch ON LFXT oscillator
		HWREG16(baseAddress + OFS_CSCTL4) &= ~LFXTOFF;

		return (STATUS_SUCCESS);
	} else   {

		return (STATUS_FAIL);
	}
}


//******************************************************************************
//
//! Bypasses the LFXT crystal oscillator with time out. Loops until all
//! oscillator fault flags are cleared or until a timeout counter is
//! decremented and equals to zero. NOTE: User must call
//! CS_setExternalClockSource to set frequency of external clocks
//! before calling this function
//!
//! \param baseAddress is the base address of the CS module.
//! \param timeout is the count value that gets decremented every time the loop
//!         that clears oscillator fault flags gets executed.
//!
//! Modified registers are \b CSCTL0, \b CSCTL4, \b CSCTL5, \b SFRIFG1
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//******************************************************************************
unsigned short
CS_bypassLFXTWithTimeout (
    uint32_t baseAddress,
    uint32_t timeout
    )
{
    assert(CS_LFXTClockFrequency !=0);

	assert(CS_LFXTClockFrequency<LFXT_FREQUENCY_THRESHOLD);

	// Set LFXT in LF mode Switch off LFXT oscillator and enable BYPASS mode
	HWREG16(baseAddress + OFS_CSCTL4) |= (LFXTBYPASS + LFXTOFF);

	while ((HWREG8(baseAddress + OFS_CSCTL5) & LFXTOFFG) && --timeout)
	{
		//Clear OSC fault Flags fault flags
		HWREG8(baseAddress + OFS_CSCTL5) &= ~(LFXTOFFG);

		// Clear the global fault flag. In case the LFXT caused the global fault
		// flag to get set this will clear the global error condition. If any
		// error condition persists, global flag will get again.
		HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

	}



    if (timeout){
        return (STATUS_SUCCESS);
    } else {
        return (STATUS_FAIL);
    }
}


//******************************************************************************
//
//! Stops the LFXT oscillator using the LFXTOFF bit.
//!
//! \param baseAddress is the base address of the CS module.
//!
//! Modified registers are \b CSCTL4
//!
//! \return NONE
//
//******************************************************************************
void
CS_LFXTOff (uint32_t baseAddress)
{
    //Switch off LFXT oscillator
    HWREG16(baseAddress + OFS_CSCTL4) |= LFXTOFF;
}
//******************************************************************************
//
//! Initializes the HFXT crystal oscillator, which supports crystal frequencies
//! between 0 MHz and 24 MHz, depending on the selected drive strength. Loops
//! until all oscillator fault flags are cleared, with no timeout. See the
//! device-specific data sheet for appropriate drive settings. NOTE: User must
//! call CS_setExternalClockSource to set frequency of external clocks
//! before calling this function.
//!
//! \param baseAddress is the base address of the CS module.
//! \param hfxtdrive is the target drive strength for the HFXT crystal oscillator.
//!      Valid values are
//!     \b CS_HFXTDRIVE_0,
//!     \b CS_HFXTDRIVE_1,
//!     \b CS_HFXTDRIVE_2,
//!     \b CS_HFXTDRIVE_3	[Default]
//!
//! Modified registers are \b CSCTL4, \b CSCTL5, \b SFRIFG1
//!
//! \return NONE
//
//******************************************************************************
void
CS_HFXTStart (  uint32_t baseAddress,
    uint16_t hfxtdrive
    )
{
	assert(CS_HFXTClockFrequency !=0);

    assert((hfxtdrive == CS_HFXTDRIVE_4MHZ_8MHZ  ) ||
    	   (hfxtdrive == CS_HFXTDRIVE_8MHZ_16MHZ ) ||
           (hfxtdrive == CS_HFXTDRIVE_16MHZ_24MHZ )||
           (hfxtdrive == CS_HFXTDRIVE_24MHZ_32MHZ ));

    //Disable HFXTBYPASS mode and Switch on HFXT oscillator
    HWREG16(baseAddress + OFS_CSCTL4) &= ~HFXTBYPASS;

	//If HFFrequency is 16MHz or above
	if (CS_HFXTClockFrequency>16000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_3;
	}
	//If HFFrequency is between 8MHz and 16MHz
	else if (CS_HFXTClockFrequency>8000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_2;
	}
	//If HFFrequency is between 0MHz and 4MHz
	else if (CS_HFXTClockFrequency<4000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_0;
	}
	//If HFFrequency is between 4MHz and 8MHz
	else{
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_1;
	}

    while (HWREG8(baseAddress + OFS_CSCTL5) & HFXTOFFG){
     //Clear OSC flaut Flags
     HWREG8(baseAddress + OFS_CSCTL5) &= ~(HFXTOFFG);

     //Clear OFIFG fault flag
     HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    HWREG16(baseAddress + OFS_CSCTL4) = ( HWREG16(baseAddress + OFS_CSCTL4) &
                                         ~(CS_HFXTDRIVE_24MHZ_32MHZ)
                                         ) |
                                       (hfxtdrive);

    HWREG16(baseAddress + OFS_CSCTL4) &= ~HFXTOFF;

}

//******************************************************************************
//
//! Bypasses the HFXT crystal oscillator, which supports crystal frequencies
//! between 0 MHz and 24 MHz. Loops until all oscillator fault flags are
//! cleared, with no timeout.NOTE: User must call CS_setExternalClockSource
//! to set frequency of external clocks before calling this function.
//!
//! \param baseAddress is the base address of the CS module.
//!
//! Modified registers are \b CSCTL4, \b CSCTL5, \b SFRIFG1
//!
//! \return NONE
//
//******************************************************************************
void
CS_bypassHFXT (  uint32_t baseAddress )
{
	//Verify user has initialized value of HFXTClock
	assert(CS_HFXTClockFrequency !=0);

    //Switch off HFXT oscillator and set it to BYPASS mode
    HWREG16(baseAddress + OFS_CSCTL4) |= ( HFXTBYPASS + HFXTOFF );


	//Set correct HFFREQ bit for FR58xx/FR59xx devices

	//If HFFrequency is 16MHz or above
	if (CS_HFXTClockFrequency>16000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_3;
	}
	//If HFFrequency is between 8MHz and 16MHz
	else if (CS_HFXTClockFrequency>8000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_2;
	}
	//If HFFrequency is between 0MHz and 4MHz
	else if (CS_HFXTClockFrequency<4000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_0;
	}
	//If HFFrequency is between 4MHz and 8MHz
	else{
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_1;
	}

    while (HWREG8(baseAddress + OFS_CSCTL5) & HFXTOFFG){
     //Clear OSC fault Flags
     HWREG8(baseAddress + OFS_CSCTL5) &= ~(HFXTOFFG);

     //Clear OFIFG fault flag
     HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }
}
//******************************************************************************
//
//! Initializes the HFXT crystal oscillator, which supports crystal frequencies
//! between 0 MHz and 24 MHz, depending on the selected drive strength. Loops
//! until all oscillator fault flags are cleared or until a timeout counter is
//! decremented and equals to zero. See the device-specific data sheet for
//! appropriate drive settings. NOTE: User must call CS_setExternalClockSource
//! to set frequency of external clocks before calling this function.
//!
//! \param baseAddress is the base address of the CS module.
//! \param hfxtdrive is the target drive strength for the HFXT crystal oscillator.
//!        Valid values are
//!        \b CS_HFXTDRIVE_0,
//!        \b CS_HFXTDRIVE_1,
//!        \b CS_HFXTDRIVE_2,
//!        \b CS_HFXTDRIVE_3	[Default]
//! \param timeout is the count value that gets decremented every time the loop
//!         that clears oscillator fault flags gets executed.
//!
//! Modified registers are \b CSCTL4, \b CSCTL5, \b SFRIFG1
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//******************************************************************************
unsigned short
CS_HFXTStartWithTimeout ( uint32_t baseAddress,
    uint16_t hfxtdrive,
    uint32_t timeout
    )
{
	//Verify user has initialized value of HFXTClock
	assert(CS_HFXTClockFrequency !=0);

	// Disable HFXTBYPASS mode
    HWREG16(baseAddress + OFS_CSCTL4) &= ~HFXTBYPASS;

	//Set correct HFFREQ bit for FR58xx/FR59xx devices based
	//on HFXTClockFrequency

	//If HFFrequency is 16MHz or above
	if (CS_HFXTClockFrequency>16000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_3;
	}
	//If HFFrequency is between 8MHz and 16MHz
	else if (CS_HFXTClockFrequency>8000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_2;
	}
	//If HFFrequency is between 0MHz and 4MHz
	else if (CS_HFXTClockFrequency<4000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_0;
	}
	//If HFFrequency is between 4MHz and 8MHz
	else{
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_1;
	}


	while ((HWREG8(baseAddress + OFS_CSCTL5) & HFXTOFFG) && --timeout)
	{
		//Clear OSC fault Flags fault flags
		HWREG8(baseAddress + OFS_CSCTL5) &= ~(HFXTOFFG);

		// Clear the global fault flag. In case the LFXT caused the global fault
		// flag to get set this will clear the global error condition. If any
		// error condition persists, global flag will get again.
		HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

	}

    if (timeout){
		//Set drive strength for HFXT
	    HWREG16(baseAddress + OFS_CSCTL4) = ( HWREG16(baseAddress + OFS_CSCTL4) &
	                                         ~(CS_HFXTDRIVE_24MHZ_32MHZ)
	                                         ) |
	                                       (hfxtdrive);

	    //Switch on HFXT oscillator
	    HWREG16(baseAddress + OFS_CSCTL4) &= ~HFXTOFF;

        return (STATUS_SUCCESS);
    } else   {
        return (STATUS_FAIL);
    }
}


//******************************************************************************
//
//! Bypasses the HFXT crystal oscillator, which supports crystal frequencies
//! between 0 MHz and 24 MHz. Loops until all oscillator fault flags are
//! cleared or until a timeout counter is decremented and equals to zero.
//! NOTE: User must call CS_setExternalClockSource to set frequency of external
//! clocks before calling this function.
//!
//! \param baseAddress is the base address of the CS module.
//! \param timeout is the count value that gets decremented every time the loop
//!         that clears oscillator fault flags gets executed.
//!
//! Modified registers are \b CSCTL4, \b CSCTL5, \b SFRIFG1G
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//******************************************************************************

extern unsigned short
CS_bypassHFXTWithTimeout ( uint32_t baseAddress,
    uint32_t timeout
    )
{
	//Verify user has initialized value of HFXTClock
	assert(CS_HFXTClockFrequency !=0);

	//If HFFrequency is 16MHz or above
	if (CS_HFXTClockFrequency>16000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_3;
	}
	//If HFFrequency is between 8MHz and 16MHz
	else if (CS_HFXTClockFrequency>8000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_2;
	}
	//If HFFrequency is between 0MHz and 4MHz
	else if (CS_HFXTClockFrequency<4000000) {
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_0;
	}
	//If HFFrequency is between 4MHz and 8MHz
	else{
		HWREG16(baseAddress + OFS_CSCTL4)=HFFREQ_1;
	}

    //Switch off HFXT oscillator and enable BYPASS mode
    HWREG16(baseAddress + OFS_CSCTL4) |= (HFXTBYPASS + HFXTOFF );


	while ((HWREG8(baseAddress + OFS_CSCTL5) & HFXTOFFG) && --timeout)
	{
		//Clear OSC fault Flags fault flags
		HWREG8(baseAddress + OFS_CSCTL5) &= ~(HFXTOFFG);

		// Clear the global fault flag. In case the LFXT caused the global fault
		// flag to get set this will clear the global error condition. If any
		// error condition persists, global flag will get again.
		HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

	}

    if (timeout){
        return (STATUS_SUCCESS);
    } else   {
        return (STATUS_FAIL);
    }
}

//******************************************************************************
//
//! Stops the HFXT oscillator using the HFXTOFF bit.
//!
//! \param baseAddress is the base address of the CS module.
//!
//! Modified registers are \b CSCTL4
//!
//! \return NONE
//
//******************************************************************************
void
CS_HFXTOff (uint32_t baseAddress)
{
	CS_HFXTOff(baseAddress);
}
//******************************************************************************
//
//! Enables conditional module requests
//!
//! \param baseAddress is the base address of the CS module.
//! \param selectClock selects specific request enables. Valid values are
//!        \b CS_ACLK,
//!        \b CS_SMCLK,
//!        \b CS_MCLK,
//!        \b CS_MODOSC
//!
//! Modified registers are \b CSCTL6
//!
//! \return NONE
//
//******************************************************************************
void
CS_enableClockRequest (
    uint32_t baseAddress,
    uint8_t selectClock
    )
{
	assert(
			(CS_ACLK  == selectClock )||
			(CS_SMCLK == selectClock )||
			(CS_MCLK  == selectClock )||
			(CS_MODOSC== selectClock ));

    HWREG8(baseAddress + OFS_CSCTL6) |= selectClock;
}

//******************************************************************************
//
//! Disables conditional module requests
//!
//! \param baseAddress is the base address of the CS module.
//! \param selectClock selects specific request enables. Valid values are
//!        \b CS_ACLK,
//!        \b CS_SMCLK,
//!        \b CS_MCLK,
//!        \b CS_MODOSC
//!
//! Modified registers are \b CSCTL6
//!
//! \return NONE
//
//******************************************************************************
void
CS_disableClockRequest (
    uint32_t baseAddress,
    uint8_t selectClock
    )
{
	assert(
			(CS_ACLK  == selectClock )||
			(CS_SMCLK == selectClock )||
			(CS_MCLK  == selectClock )||
			(CS_MODOSC== selectClock ));

    HWREG8(baseAddress + OFS_CSCTL6) &= ~selectClock;
}

//******************************************************************************
//
//! Gets the current CS fault flag status.
//!
//! \param baseAddress is the base address of the CS module.
//! \param mask is the masked interrupt flag status to be returned.
//!      Mask parameter can be either any of the following selection.
//!         - \b CS_HFXTOFFG - HFXT oscillator fault flag
//!         - \b CS_LFXTOFFG - LFXT oscillator fault flag

//! Modified registers are \b CSCTL5
//!
//! \return The current flag status for the corresponding masked bit
//
//******************************************************************************
uint8_t
CS_faultFlagStatus (
    uint32_t baseAddress,
    uint8_t mask
    )
{
    assert(
    			(CS_HFXTOFFG  == mask )||
    			(CS_LFXTOFFG == mask )
    			);
    return (HWREG8(baseAddress + OFS_CSCTL5) & mask);
}

//******************************************************************************
//
//! Clears the current CS fault flag status for the masked bit.
//!
//! \param baseAddress is the base address of the CS module.
//! \param mask is the masked interrupt flag status to be returned.
//!         mask parameter can be any one of the following
//!         - \b CS_HFXTOFFG - HFXT oscillator fault flag
//!         - \b CS_LFXTOFFG - LFXT oscillator fault flag

//!
//! Modified registers are \b CSCTL5
//!
//! \return NONE
//
//******************************************************************************
void
CS_clearFaultFlag (
    uint32_t baseAddress,
    uint8_t mask
    )
{
    assert(
			(CS_HFXTOFFG  == mask )||
			(CS_LFXTOFFG == mask )
			);
    HWREG8(baseAddress + OFS_CSCTL5) &= ~mask;
}

//******************************************************************************
//
//Compute the clock frequency when clock is source from DCO
//
//\param baseAddress is the base address of the CS module.
//\param clockdivider is clock source for FLL reference. Valid values are:
//           \b CS_CLOCK_DIVIDER_1,
//           \b CS_CLOCK_DIVIDER_2,
//           \b CS_CLOCK_DIVIDER_4,
//           \b CS_CLOCK_DIVIDER_8,
//           \b CS_CLOCK_DIVIDER_16,
//           \b CS_CLOCK_DIVIDER_32
//
//\return Calculated clock frequency in Hz
//
//******************************************************************************
static uint32_t
privateCSASourceClockFromDCO ( uint32_t baseAddress,
		uint8_t clockdivider)
{
    uint32_t CLKFrequency=0;

   	if (HWREG16(baseAddress + OFS_CSCTL1)& DCORSEL) {
			switch(HWREG16(baseAddress + OFS_CSCTL1)& DCOFSEL_7){
				case DCOFSEL_0:
					CLKFrequency=CS_DCO_FREQ_1/clockdivider;
					break;
				case DCOFSEL_1:
					CLKFrequency=CS_DCO_FREQ_5/clockdivider;
					break;
				case DCOFSEL_2:
					CLKFrequency=CS_DCO_FREQ_6/clockdivider;
					break;
				case DCOFSEL_3:
					CLKFrequency=CS_DCO_FREQ_7/clockdivider;
					break;
				case DCOFSEL_4:
					CLKFrequency=CS_DCO_FREQ_8/clockdivider;
					break;
				case DCOFSEL_5:
					CLKFrequency=CS_DCO_FREQ_9/clockdivider;
					break;
				case DCOFSEL_6:
				case DCOFSEL_7:
					CLKFrequency=CS_DCO_FREQ_10/clockdivider;
					break;
				default:
					CLKFrequency=0;
					break;
			}
		}else{
			switch(HWREG16(baseAddress + OFS_CSCTL1)& DCOFSEL_7){
				case DCOFSEL_0:
					CLKFrequency=CS_DCO_FREQ_1/clockdivider;
					break;
				case DCOFSEL_1:
					CLKFrequency=CS_DCO_FREQ_2/clockdivider;
					break;
				case DCOFSEL_2:
					CLKFrequency=CS_DCO_FREQ_3/clockdivider;
					break;
				case DCOFSEL_3:
					CLKFrequency=CS_DCO_FREQ_4/clockdivider;
					break;
				case DCOFSEL_4:
					CLKFrequency=CS_DCO_FREQ_5/clockdivider;
					break;
				case DCOFSEL_5:
					CLKFrequency=CS_DCO_FREQ_6/clockdivider;
					break;
				case DCOFSEL_6:
				case DCOFSEL_7:
					CLKFrequency=CS_DCO_FREQ_7/clockdivider;
					break;
				default:
					CLKFrequency=0;
					break;
			}

		}


    return (CLKFrequency);
}


//******************************************************************************
//
//Compute the clock frequency given the clock source and divider
//
//\param baseAddress is the base address of the CS module.
//\param CLKSource is the clock source. Valid values are:
//		\b SELM__LFXTCLK (SELM__LFCLK),
//		\b SELM__VLOCLK,
//		\b SELM__HFXTCLK (SELM__HFCLK),
//		\b SELM__DCOCLK,
//		\b SELM__HFXTCLK (SELM__HFCLK),
//		\b SELM__DCOCLK,
//		\b SELM__LFMODOSC	[Available for FR58xx/FR59xx]
//		\b SELM__MODOSC		[Available for FR58xx/FR59xx]
//\param CLKSourceDivider is the Clock source divider
//
//\return Calculated clock frequency in Hz
//
//******************************************************************************
static uint32_t
privateCSAComputeCLKFrequency ( uint32_t baseAddress,
    uint16_t CLKSource,
    uint16_t CLKSourceDivider
    )
{
    uint32_t CLKFrequency=0;
    uint8_t CLKSourceFrequencyDivider = 1;
    uint8_t i = 0;

    // Determine Frequency divider
    for ( i = 0; i < CLKSourceDivider; i++){
        CLKSourceFrequencyDivider *= 2;
    }

    // Determine clock source based on CLKSource
    switch (CLKSource){

    	// If LFXT is selected as clock source
        case SELM__LFXTCLK:
            CLKFrequency = (CS_LFXTClockFrequency /
                            CLKSourceFrequencyDivider);

            //Check if LFXTOFFG is not set. If fault flag is set
            //VLO is used as source clock
            if (HWREG8(baseAddress + OFS_CSCTL5) & LFXTOFFG){
                HWREG8(baseAddress + OFS_CSCTL5) &= ~(LFXTOFFG);
                //Clear OFIFG fault flag
                HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

                if (HWREG8(baseAddress + OFS_CSCTL5) & LFXTOFFG){
                			CLKFrequency = CS_LFMODCLK_FREQUENCY;
				}
            }
            break;

        case SELM__VLOCLK:
            CLKFrequency =
                (CS_VLOCLK_FREQUENCY / CLKSourceFrequencyDivider);
            break;

        case SELM__LFMODOSC:
            CLKFrequency =
                (CS_LFMODCLK_FREQUENCY / CLKSourceFrequencyDivider);

            break;

        case SELM__DCOCLK:
        	CLKFrequency =
        	privateCSASourceClockFromDCO(baseAddress, CLKSourceFrequencyDivider);

            break;

        case SELM__MODOSC:
            CLKFrequency =
                (CS_MODCLK_FREQUENCY / CLKSourceFrequencyDivider);

            break;

        case SELM__HFXTCLK:
            CLKFrequency =
                (CS_HFXTClockFrequency / CLKSourceFrequencyDivider);

            if (HWREG8(baseAddress + OFS_CSCTL5) & HFXTOFFG){

              HWREG8(baseAddress + OFS_CSCTL5) &=  ~HFXTOFFG;
              //Clear OFIFG fault flag
              HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
            }

            if (HWREG8(baseAddress + OFS_CSCTL5) & HFXTOFFG){
                CLKFrequency = CS_MODCLK_FREQUENCY;
            }
            break;
    }

    return (CLKFrequency) ;
}

//******************************************************************************
//
//! Get the current ACLK frequency.
//!
//! If a oscillator fault is set, the frequency returned will be based on the
//! fail safe mechanism of CS module. The user of this API must ensure that
//! CS_externalClockSourceInit API was invoked before in case LFXT or
//! HFXT is being used. 
//!
//! \param baseAddress is the base address of the CS module.
//!
//! \return Current ACLK frequency in Hz
//
//******************************************************************************
uint32_t
CS_getACLK (uint32_t baseAddress)
{

	//Find ACLK source
	uint16_t ACLKSource = (HWREG16(baseAddress + OFS_CSCTL2) & SELA_7);
	ACLKSource = ACLKSource >> 8;

	//Find ACLK frequency divider
	uint16_t ACLKSourceDivider =  HWREG16(baseAddress + OFS_CSCTL3) & SELA_7;
	ACLKSourceDivider = ACLKSourceDivider >>8;

	return (privateCSAComputeCLKFrequency(baseAddress,
				ACLKSource,
				ACLKSourceDivider));

}

//******************************************************************************
//
//! Get the current SMCLK frequency.
//!
//! If a oscillator fault is set, the frequency returned will be based on the
//! fail safe mechanism of CS module. The user of this API must ensure that
//! CS_externalClockSourceInit API was invoked before in case LFXT or
//! HFXT is being used. 
//!
//! \param baseAddress is the base address of the CS module.
//!
//! \return Current SMCLK frequency in Hz
//
//******************************************************************************
uint32_t
CS_getSMCLK (uint32_t baseAddress)
{
		//Find SMCLK source
		uint16_t SMCLKSource = HWREG8(baseAddress + OFS_CSCTL2) & SELS_7 ;

		SMCLKSource = SMCLKSource >> 4;

		//Find SMCLK frequency divider
		uint16_t SMCLKSourceDivider = HWREG16(baseAddress + OFS_CSCTL3) & SELS_7;
		SMCLKSourceDivider = SMCLKSourceDivider >> 4;

		return (privateCSAComputeCLKFrequency(baseAddress,
					SMCLKSource,
					SMCLKSourceDivider )
				);
}

//******************************************************************************
//
//! Get the current MCLK frequency.
//!
//! If a oscillator fault is set, the frequency returned will be based on the
//! fail safe mechanism of CS module. The user of this API must ensure that
//! CS_externalClockSourceInit API was invoked before in case LFXT or
//! HFXT is being used. 
//!
//! \param baseAddress is the base address of the CS module.
//!
//! \return Current MCLK frequency in Hz
//
//******************************************************************************
uint32_t
CS_getMCLK (uint32_t baseAddress)
{
		//Find MCLK source
		uint16_t MCLKSource = (HWREG16(baseAddress + OFS_CSCTL2) & SELM_7);
		//Find MCLK frequency divider
		uint16_t MCLKSourceDivider =  HWREG16(baseAddress + OFS_CSCTL3) & SELM_7;

		return (privateCSAComputeCLKFrequency(baseAddress,
					MCLKSource,
					MCLKSourceDivider )
				);
}

//******************************************************************************
//
//! Turns off VLO
//!
//! Modified registers are \b CSCTL4
//!
//
//******************************************************************************

void
CS_VLOoff(uint32_t baseAddress)
{
	HWREG16(baseAddress + OFS_CSCTL4) |= VLOOFF;
}

//******************************************************************************
//
//! Clears all the Oscillator Flags
//!
//! \param baseAddress is the base address of the CS module.
//! \param timeout is the count value that gets decremented every time the loop
//!         that clears oscillator fault flags gets executed.
//!
//! Modified registers are \b CSCTL5, \b SFRIFG1G
//!
//! \return the mask of the oscillator flag status.
//
//******************************************************************************
uint16_t CS_clearAllOscFlagsWithTimeout(uint32_t baseAddress,
                                             uint32_t timeout
                                             )
{
    do {
      // Clear all osc fault flags
      HWREG8(baseAddress + OFS_CSCTL5) &= ~(LFXTOFFG + HFXTOFFG );

      // Clear the global osc fault flag.
      HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

      // Check LFXT fault flags
    } while ((HWREG8(SFR_BASE + OFS_SFRIFG1) & OFIFG) && --timeout);

    return (HWREG8(baseAddress + OFS_CSCTL5) & (LFXTOFFG + HFXTOFFG));
}

//******************************************************************************
//
//! Set DCO frequency
//!
//!
//! \param baseAddress is the base address of the CS module.
//! \param dcofsel selects valid frequency options based on dco frequency range
//!			selection (dcorsel). Valid options are:
//!			\e For FR58xx/FR59xx devices:
//!			\b CS_DCOFSEL_0 - Low frequency option 1 MHZ. High frequency
//!				option 1 MHz.
//!			\b CS_DCOFSEL_1 - Low frequency option 2.67 MHZ. High frequency
//!				option 5.33 MHz.
//!			\b CS_DCOFSEL_2 Low frequency option 3.33 MHZ. High frequency
//!				option 6.67 MHz.
//!			\b CS_DCOFSEL_3 Low frequency option 4 MHZ. High frequency
//!				option 8 MHz.
//!			\b CS_DCOFSEL_4 Low frequency option 5.33 MHZ. High frequency
//!				option 16 MHz.
//!			\b CS_DCOFSEL_5 Low frequency option 6.67 MHZ. High frequency
//!				option 20 MHz.
//!			\b CS_DCOFSEL_6 Low frequency option 8 MHZ. High frequency
//!				option 24 MHz.
//!\param dcorsel selects frequency range option. Valid options are:
//!			\b CS_DCORSEL_0	[Default - Low Frequency Option]
//!			\b CS_DCORSEL_1	[High Frequency Option]
//!
//! \return NONE
//
//******************************************************************************
void
CS_setDCOFreq(
		uint32_t baseAddress,
		uint16_t dcorsel,
		uint16_t dcofsel)
{
		assert(
					(dcofsel==CS_DCOFSEL_0)||
					(dcofsel==CS_DCOFSEL_1)||
					(dcofsel==CS_DCOFSEL_2)||
					(dcofsel==CS_DCOFSEL_3)||
					(dcofsel==CS_DCOFSEL_4)||
					(dcofsel==CS_DCOFSEL_5)||
					(dcofsel==CS_DCOFSEL_6)
					);

		//Verify user has selected a valid DCO Frequency Range option
		assert(
				(dcorsel==CS_DCORSEL_0)||
				(dcorsel==CS_DCORSEL_1));

		//Unlock CS control register
		HWREG16(baseAddress + OFS_CSCTL0) = CSKEY;

		// Set user's frequency selection for DCO
		HWREG16(baseAddress + OFS_CSCTL1) = (dcorsel + dcofsel);

}

//******************************************************************************
//
//Close the Doxygen group.
//! @}
//
//******************************************************************************

