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
//*****************************************************************************
//
//pmm.c - Driver for the FRPMM Module.
//
//*****************************************************************************
#include "inc/hw_regaccess.h"
#include "assert.h"
#include "pmm.h"
#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430xgeneric.h"
#else
#include "deprecated/CCS/msp430xgeneric.h"
#endif

//*****************************************************************************
//
//! Enables the low power reset.  SVSH does not reset device, but triggers
//!  a system NMI
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_enableLowPowerReset (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0) |= PMM_LPRST;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Disables the low power reset.  SVSH resets device.
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_disableLowPowerReset (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0) &= ~PMM_LPRST;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Enables the high-side SVS circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_enableSVSH (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0_L) |= PMM_SVSHE;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Disables the high-side SVS circuitry
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_disableSVSH (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0_L) &= ~PMM_SVSHE;//~PMM_SVSHE;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Makes the low-dropout voltage regulator (LDO) remain ON when going into LPM 3/4. [Default]
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_regOn (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0) &= ~PMM_PMMREGOFF;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Turns OFF the low-dropout voltage regulator (LDO) when going into LPM3/4,
//!   thus the system will enter LPM3.5 or LPM4.5 respectively
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_regOff (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0) |= PMM_PMMREGOFF;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Calling this function will trigger a software Power On Reset (POR).
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_trigPOR (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0) |= PMM_PMMSWPOR;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Calling this function will trigger a software Brown Out Rest (BOR).
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! Modified registers are \b PMMCTL0.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_trigBOR (uint32_t baseAddress)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG8(baseAddress + OFS_PMMCTL0) |= PMM_PMMSWBOR;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}


//*****************************************************************************
//
//! Clear all interrupt flags for the PMM
//!
//! \param baseAddress is the base address of the PMM module.
//! \param mask is the mask for specifying the required flag
//!        Valid values are
//!            \b  PMM_PMMBORIFG
//!            \b  PMM_PMMRSTIFG,
//!            \b  PMM_PMMPORIFG,
//!            \b  PMM_SVSLIFG,
//!            \b  PMM_SVSHIFG,
//!            \b  PMM_PMMLPM5IFG,
//!            \b  PMM_ALL
//!
//! Modified registers are \b PMMCTL0, \b PMMIFG.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_clearInterrupt (uint32_t baseAddress,
		uint16_t mask)
{
    HWREG8(baseAddress + OFS_PMMCTL0_H) = PMM_PW_H;
    HWREG16(baseAddress + OFS_PMMIFG) &= ~mask;
    HWREG8(baseAddress + OFS_PMMCTL0_H) = 0x00;
}

//*****************************************************************************
//
//! Returns interrupt status
//!
//! \param baseAddress is the base address of the PMM module.
//! \param mask is the mask for specifying the required flag
//!        Valid values are
//!            \b  PMM_PMMBORIFG
//!            \b  PMM_PMMRSTIFG,
//!            \b  PMM_PMMPORIFG,
//!            \b  PMM_SVSLIFG,
//!            \b  PMM_SVSHIFG,
//!            \b  PMM_PMMLPM5IFG,
//!
//! \return STATUS_SUCCESS (0x01) or STATUS_FAIL (0x00)
//
//*****************************************************************************
uint16_t PMM_getInterruptStatus (uint32_t baseAddress,
    uint16_t mask)
{
    return ( (HWREG16(baseAddress + OFS_PMMIFG)) & mask );
}


//*****************************************************************************
//
//! Lock I/O pin and other LPMx.5 relevant (e.g. RTC) configurations
//!  upon entry/exit to/from LPMx.5. Once power is applied to the device,
//!  this bit, once set, can only be cleared by the user or
//!  via another power cycle. LPMx.5 configuration remains locked.
//!  Pin state is held during LPMx.5 entry and exit.
//! Disable the GPIO power-on default high-impedance mode to activate previously
//!  configured port settings 
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_lockLPM5 (uint32_t baseAddress)
{
	HWREG8(baseAddress + OFS_PM5CTL0) |= PMM_LOCKLPM5;
}

//*****************************************************************************
//
//! LPMx.5 configuration is not locked and defaults to its reset condition.
//! Disable the GPIO power-on default high-impedance mode to activate previously
//! configured port settings
//!
//! \param baseAddress is the base address of the PMM module.
//!
//! \return NONE
//
//*****************************************************************************
void PMM_unlockLPM5 (uint32_t baseAddress)
{
	HWREG8(baseAddress + OFS_PM5CTL0) &= ~PMM_LOCKLPM5;
}


//*****************************************************************************
//
//Close the Doxygen group.
//! @}
//
//*****************************************************************************
