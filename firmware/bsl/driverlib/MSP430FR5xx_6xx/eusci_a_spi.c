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
// eusci_a_spi.c - Driver for the eusci_a_spi Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup eusci_a_spi_api
//! @{
//
//*****************************************************************************

#include "inc/hw_regaccess.h"
#include "inc/hw_memmap.h"

#define __MSP430_HAS_EUSCI_Ax__

#ifdef __MSP430_HAS_EUSCI_Ax__
#include "eusci_a_spi.h"

/************************************************************
* USCI Ax
************************************************************/
#define OFS_UCAxCTLW0         (0x0000)  /* USCI Ax Control Word Register 0 */
#define OFS_UCAxCTLW0_L        OFS_UCAxCTLW0
#define OFS_UCAxCTLW0_H        OFS_UCAxCTLW0+1
#define OFS_UCAxCTL0          (0x0001)
#define OFS_UCAxCTL1          (0x0000)
#define UCAxCTL1            UCAxCTLW0_L  /* USCI Ax Control Register 1 */
#define UCAxCTL0            UCAxCTLW0_H  /* USCI Ax Control Register 0 */
#define OFS_UCAxCTLW1         (0x0002)  /* USCI Ax Control Word Register 1 */
#define OFS_UCAxCTLW1_L        OFS_UCAxCTLW1
#define OFS_UCAxCTLW1_H        OFS_UCAxCTLW1+1
#define OFS_UCAxBRW           (0x0006)  /* USCI Ax Baud Word Rate 0 */
#define OFS_UCAxBRW_L          OFS_UCAxBRW
#define OFS_UCAxBRW_H          OFS_UCAxBRW+1
#define OFS_UCAxBR0           (0x0006)
#define OFS_UCAxBR1           (0x0007)
#define UCAxBR0             UCAxBRW_L /* USCI Ax Baud Rate 0 */
#define UCAxBR1             UCAxBRW_H /* USCI Ax Baud Rate 1 */
#define OFS_UCAxMCTLW         (0x0008)  /* USCI Ax Modulation Control */
#define OFS_UCAxMCTLW_L        OFS_UCAxMCTLW
#define OFS_UCAxMCTLW_H        OFS_UCAxMCTLW+1
#define OFS_UCAxSTATW         (0x000A)  /* USCI Ax Status Register */
#define OFS_UCAxRXBUF         (0x000C)  /* USCI Ax Receive Buffer */
#define OFS_UCAxRXBUF_L        OFS_UCAxRXBUF
#define OFS_UCAxRXBUF_H        OFS_UCAxRXBUF+1
#define OFS_UCAxTXBUF         (0x000E)  /* USCI Ax Transmit Buffer */
#define OFS_UCAxTXBUF_L        OFS_UCAxTXBUF
#define OFS_UCAxTXBUF_H        OFS_UCAxTXBUF+1
#define OFS_UCAxABCTL         (0x0010)  /* USCI Ax LIN Control */
#define OFS_UCAxIRCTL         (0x0012)  /* USCI Ax IrDA Transmit Control */
#define OFS_UCAxIRCTL_L        OFS_UCAxIRCTL
#define OFS_UCAxIRCTL_H        OFS_UCAxIRCTL+1
#define OFS_UCAxIRTCTL        (0x0012)
#define OFS_UCAxIRRCTL        (0x0013)
#define UCAxIRTCTL          UCAxIRCTL_L  /* USCI Ax IrDA Transmit Control */
#define UCAxIRRCTL          UCAxIRCTL_H  /* USCI Ax IrDA Receive Control */
#define OFS_UCAxIE            (0x001A)  /* USCI Ax Interrupt Enable Register */
#define OFS_UCAxIE_L           OFS_UCAxIE
#define OFS_UCAxIE_H           OFS_UCAxIE+1
#define OFS_UCAxIFG           (0x001C)  /* USCI Ax Interrupt Flags Register */
#define OFS_UCAxIFG_L          OFS_UCAxIFG
#define OFS_UCAxIFG_H          OFS_UCAxIFG+1
#define OFS_UCAxIE__UART      (0x001A)
#define OFS_UCAxIE__UART_L     OFS_UCAxIE__UART
#define OFS_UCAxIE__UART_H     OFS_UCAxIE__UART+1
#define OFS_UCAxIFG__UART     (0x001C)
#define OFS_UCAxIFG__UART_L    OFS_UCAxIFG__UART
#define OFS_UCAxIFG__UART_H    OFS_UCAxIFG__UART+1
#define OFS_UCAxIV            (0x001E)  /* USCI Ax Interrupt Vector Register */

#define OFS_UCAxCTLW0__SPI    (0x0000)
#define OFS_UCAxCTLW0__SPI_L   OFS_UCAxCTLW0__SPI
#define OFS_UCAxCTLW0__SPI_H   OFS_UCAxCTLW0__SPI+1
#define OFS_UCAxCTL0__SPI     (0x0001)
#define OFS_UCAxCTL1__SPI     (0x0000)
#define OFS_UCAxBRW__SPI      (0x0006)
#define OFS_UCAxBRW__SPI_L     OFS_UCAxBRW__SPI
#define OFS_UCAxBRW__SPI_H     OFS_UCAxBRW__SPI+1
#define OFS_UCAxBR0__SPI      (0x0006)
#define OFS_UCAxBR1__SPI      (0x0007)
#define OFS_UCAxSTATW__SPI    (0x000A)
#define OFS_UCAxRXBUF__SPI    (0x000C)
#define OFS_UCAxRXBUF__SPI_L   OFS_UCAxRXBUF__SPI
#define OFS_UCAxRXBUF__SPI_H   OFS_UCAxRXBUF__SPI+1
#define OFS_UCAxTXBUF__SPI    (0x000E)
#define OFS_UCAxTXBUF__SPI_L   OFS_UCAxTXBUF__SPI
#define OFS_UCAxTXBUF__SPI_H   OFS_UCAxTXBUF__SPI+1
#define OFS_UCAxIE__SPI       (0x001A)
#define OFS_UCAxIFG__SPI      (0x001C)
#define OFS_UCAxIV__SPI       (0x001E)


/*
#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430xgeneric.h"
#elif defined(__TI_COMPILER_VERSION__)
#include "deprecated/CCS/msp430xgeneric.h"
#elif defined(__GNUC__)
#include <msp430xgeneric.h>
#endif
*/ 
#include <assert.h>

//*****************************************************************************
//
//! \brief Initializes the SPI Master block.
//!
//! Upon successful initialization of the SPI master block, this function will
//! have set the bus speed for the master, but the SPI Master block still
//! remains disabled and must be enabled with EUSCI_A_SPI_enable()
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI Master module.
//! \param selectClockSource selects Clock source.
//!        Valid values are:
//!        - \b EUSCI_A_SPI_CLOCKSOURCE_ACLK
//!        - \b EUSCI_A_SPI_CLOCKSOURCE_SMCLK
//! \param clockSourceFrequency is the frequency of the slected clock source
//! \param desiredSpiClock is the desired clock rate for SPI communication
//! \param msbFirst controls the direction of the receive and transmit shift
//!        register.
//!        Valid values are:
//!        - \b EUSCI_A_SPI_MSB_FIRST
//!        - \b EUSCI_A_SPI_LSB_FIRST [Default]
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity is clock polarity select
//!        Valid values are:
//!        - \b EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//! \param spiMode is SPI mode select
//!        Valid values are:
//!        - \b EUSCI_A_SPI_3PIN
//!        - \b EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_HIGH
//!        - \b EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_LOW
//!
//! Modified bits are \b UCCKPH, \b UCCKPL, \b UC7BIT, \b UCMSB, \b UCSSELx and
//! \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
bool EUSCI_A_SPI_masterInit(uint32_t baseAddress,
                            uint8_t selectClockSource,
                            uint32_t clockSourceFrequency,
                            uint32_t desiredSpiClock,
                            uint16_t msbFirst,
                            uint16_t clockPhase,
                            uint16_t clockPolarity,
                            uint16_t spiMode
                            )
{
        assert(
                (EUSCI_A_SPI_CLOCKSOURCE_ACLK == selectClockSource) ||
                (EUSCI_A_SPI_CLOCKSOURCE_SMCLK == selectClockSource)
                );

        assert(  (EUSCI_A_SPI_MSB_FIRST == msbFirst) ||
                 (EUSCI_A_SPI_LSB_FIRST == msbFirst)
                 );

        assert(  (EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT == clockPhase) ||
                 (EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT == clockPhase)
                 );

        assert(  (EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH == clockPolarity) ||
                 (EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW == clockPolarity)
                 );

        assert(
                (EUSCI_A_SPI_3PIN == spiMode) ||
                (EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_HIGH == spiMode) ||
                (EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_LOW == spiMode)
                );

        //Disable the USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCSWRST;

        //Reset OFS_UCAxCTLW0 values
        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB +
                                                  UCMST + UCMODE_3 + UCSYNC);


        //Reset OFS_UCAxCTLW0 values
        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCSSEL_3);

        //Select Clock
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= selectClockSource;


        HWREG16(baseAddress + OFS_UCAxBRW) =
                (uint16_t)(clockSourceFrequency / desiredSpiClock);


        /*
         * Configure as SPI master mode.
         * Clock phase select, polarity, msb
         * UCMST = Master mode
         * UCSYNC = Synchronous mode
         * UCMODE_0 = 3-pin SPI
         */
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= (
                msbFirst +
                clockPhase +
                clockPolarity +
                UCMST +
                UCSYNC +
                spiMode
                );
        //No modulation
        HWREG16(baseAddress + OFS_UCAxMCTLW) = 0;

        return STATUS_SUCCESS;
}

//*****************************************************************************
//
//! \brief Selects 4Pin Functionality
//!
//! This function should be invoked only in 4-wire mode. Invoking this function
//! has no effect in 3-wire mode.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param select4PinFunctionality selects 4 pin functionality
//!        Valid values are:
//!        - \b EUSCI_A_SPI_PREVENT_CONFLICTS_WITH_OTHER_MASTERS
//!        - \b EUSCI_A_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE
//!
//! Modified bits are \b UCSTEM of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_select4PinFunctionality(uint32_t baseAddress,
                                         uint8_t select4PinFunctionality
                                         )
{
        assert(  (EUSCI_A_SPI_PREVENT_CONFLICTS_WITH_OTHER_MASTERS == select4PinFunctionality) ||
                 (EUSCI_A_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE == select4PinFunctionality)
                 );

        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~UCSTEM;
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= select4PinFunctionality;
}

//*****************************************************************************
//
//! \brief Initializes the SPI Master clock. At the end of this function call,
//! SPI module is left enabled.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param clockSourceFrequency is the frequency of the slected clock source
//! \param desiredSpiClock is the desired clock rate for SPI communication
//!
//! Modified bits are \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_masterChangeClock(uint32_t baseAddress,
                                   uint32_t clockSourceFrequency,
                                   uint32_t desiredSpiClock
                                   )
{
        //Disable the USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCSWRST;

        HWREG16(baseAddress + OFS_UCAxBRW) =
                (uint16_t)(clockSourceFrequency / desiredSpiClock);

        //Reset the UCSWRST bit to enable the USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCSWRST);
}

//*****************************************************************************
//
//! \brief Initializes the SPI Slave block.
//!
//! Upon successful initialization of the SPI slave block, this function will
//! have initialized the slave block, but the SPI Slave block still remains
//! disabled and must be enabled with EUSCI_A_SPI_enable()
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI Slave module.
//! \param msbFirst controls the direction of the receive and transmit shift
//!        register.
//!        Valid values are:
//!        - \b EUSCI_A_SPI_MSB_FIRST
//!        - \b EUSCI_A_SPI_LSB_FIRST [Default]
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity is clock polarity select
//!        Valid values are:
//!        - \b EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//! \param spiMode is SPI mode select
//!        Valid values are:
//!        - \b EUSCI_A_SPI_3PIN
//!        - \b EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_HIGH
//!        - \b EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_LOW
//!
//! Modified bits are \b UCMSB, \b UCMST, \b UC7BIT, \b UCCKPL, \b UCCKPH, \b
//! UCMODE and \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
bool EUSCI_A_SPI_slaveInit(uint32_t baseAddress,
                           uint16_t msbFirst,
                           uint16_t clockPhase,
                           uint16_t clockPolarity,
                           uint16_t spiMode
                           )
{
        assert(
                (EUSCI_A_SPI_MSB_FIRST == msbFirst) ||
                (EUSCI_A_SPI_LSB_FIRST == msbFirst)
                );

        assert(
                (EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT == clockPhase) ||
                (EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT == clockPhase)
                );

        assert(
                (EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH == clockPolarity) ||
                (EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW == clockPolarity)
                );

        assert(
                (EUSCI_A_SPI_3PIN == spiMode) ||
                (EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_HIGH == spiMode) ||
                (EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_LOW == spiMode)
                );

        //Disable USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0)  |= UCSWRST;

        //Reset OFS_UCAxCTLW0 register
        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCMSB +
                                                  UC7BIT +
                                                  UCMST +
                                                  UCCKPL +
                                                  UCCKPH +
                                                  UCMODE_3
                                                  );


        //Clock polarity, phase select, msbFirst, SYNC, Mode0
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= ( clockPhase +
                                                  clockPolarity +
                                                  msbFirst +
                                                  UCSYNC +
                                                  spiMode
                                                  );


        return STATUS_SUCCESS;
}

//*****************************************************************************
//
//! \brief Changes the SPI colock phase and polarity. At the end of this
//! function call, SPI module is left enabled.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity is clock polarity select
//!        Valid values are:
//!        - \b EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//!
//! Modified bits are \b UCCKPL, \b UCCKPH and \b UCSWRST of \b UCAxCTLW0
//! register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_changeClockPhasePolarity(uint32_t baseAddress,
                                          uint16_t clockPhase,
                                          uint16_t clockPolarity
                                          )
{

        assert(  (EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH == clockPolarity) ||
                 (EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW == clockPolarity)
                 );

        assert(  (EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT == clockPhase) ||
                 (EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT == clockPhase)
                 );

        //Disable the USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCSWRST;

        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCCKPH + UCCKPL);

        HWREG16(baseAddress + OFS_UCAxCTLW0) |= (
                clockPhase +
                clockPolarity
                );

        //Reset the UCSWRST bit to enable the USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCSWRST);
}

//*****************************************************************************
//
//! \brief Transmits a byte from the SPI Module.
//!
//! This function will place the supplied data into SPI trasmit data register
//! to start transmission.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param transmitData data to be transmitted from the SPI module
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_transmitData( uint32_t baseAddress,
                               uint8_t transmitData
                               )
{
        HWREG16(baseAddress + OFS_UCAxTXBUF) = transmitData;
}

//*****************************************************************************
//
//! \brief Receives a byte that has been sent to the SPI Module.
//!
//! This function reads a byte of data from the SPI receive data Register.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//!
//! \return Returns the byte received from by the SPI module, cast as an
//!         uint8_t.
//
//*****************************************************************************
uint8_t EUSCI_A_SPI_receiveData(uint32_t baseAddress)
{
        return HWREG16(baseAddress + OFS_UCAxRXBUF);
}

//*****************************************************************************
//
//! \brief Enables individual SPI interrupt sources.
//!
//! Enables the indicated SPI interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param mask is the bit mask of the interrupt sources to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_A_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIFG register and bits of \b UCAxIE register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_enableInterrupt(uint32_t baseAddress,
                                 uint8_t mask
                                 )
{
        assert(!(mask & ~(EUSCI_A_SPI_RECEIVE_INTERRUPT
                          | EUSCI_A_SPI_TRANSMIT_INTERRUPT)));

        HWREG16(baseAddress + OFS_UCAxIE) |= mask;
}

//*****************************************************************************
//
//! \brief Disables individual SPI interrupt sources.
//!
//! Disables the indicated SPI interrupt sources. Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param mask is the bit mask of the interrupt sources to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_A_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIE register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_disableInterrupt(uint32_t baseAddress,
                                  uint8_t mask
                                  )
{
        assert(!(mask & ~(EUSCI_A_SPI_RECEIVE_INTERRUPT
                          | EUSCI_A_SPI_TRANSMIT_INTERRUPT)));

        HWREG16(baseAddress + OFS_UCAxIE) &= ~mask;
}

//*****************************************************************************
//
//! \brief Gets the current SPI interrupt status.
//!
//! This returns the interrupt status for the SPI module based on which flag is
//! passed.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_A_SPI_RECEIVE_INTERRUPT
//!
//! \return Logical OR of any of the following:
//!         - \b EUSCI_A_SPI_TRANSMIT_INTERRUPT
//!         - \b EUSCI_A_SPI_RECEIVE_INTERRUPT
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
uint8_t EUSCI_A_SPI_getInterruptStatus(uint32_t baseAddress,
                                       uint8_t mask
                                       )
{
        assert(!(mask & ~(EUSCI_A_SPI_RECEIVE_INTERRUPT
                          | EUSCI_A_SPI_TRANSMIT_INTERRUPT)));

        return HWREG16(baseAddress + OFS_UCAxIFG) & mask;
}

//*****************************************************************************
//
//! \brief Clears the selected SPI interrupt status flag.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//! \param mask is the masked interrupt flag to be cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_A_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIFG register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_clearInterruptFlag(uint32_t baseAddress,
                                    uint8_t mask
                                    )
{
        assert(!(mask & ~(EUSCI_A_SPI_RECEIVE_INTERRUPT
                          | EUSCI_A_SPI_TRANSMIT_INTERRUPT)));

        HWREG16(baseAddress + OFS_UCAxIFG) &=  ~mask;
}

//*****************************************************************************
//
//! \brief Enables the SPI block.
//!
//! This will enable operation of the SPI block.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//!
//! Modified bits are \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_enable(uint32_t baseAddress)
{
        //Reset the UCSWRST bit to enable the USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0) &= ~(UCSWRST);
}

//*****************************************************************************
//
//! \brief Disables the SPI block.
//!
//! This will disable operation of the SPI block.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//!
//! Modified bits are \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_A_SPI_disable(uint32_t baseAddress)
{
        //Set the UCSWRST bit to disable the USCI Module
        HWREG16(baseAddress + OFS_UCAxCTLW0) |= UCSWRST;
}

//*****************************************************************************
//
//! \brief Returns the address of the RX Buffer of the SPI for the DMA module.
//!
//! Returns the address of the SPI RX Buffer. This can be used in conjunction
//! with the DMA to store the received data directly to memory.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//!
//! \return the address of the RX Buffer
//
//*****************************************************************************
uint32_t EUSCI_A_SPI_getReceiveBufferAddressForDMA(uint32_t baseAddress)
{
        return baseAddress + OFS_UCAxRXBUF;
}

//*****************************************************************************
//
//! \brief Returns the address of the TX Buffer of the SPI for the DMA module.
//!
//! Returns the address of the SPI TX Buffer. This can be used in conjunction
//! with the DMA to obtain transmitted data directly from memory.
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//!
//! \return the address of the TX Buffer
//
//*****************************************************************************
uint32_t EUSCI_A_SPI_getTransmitBufferAddressForDMA(uint32_t baseAddress)
{
        return baseAddress + OFS_UCAxTXBUF;
}

//*****************************************************************************
//
//! \brief Indicates whether or not the SPI bus is busy.
//!
//! This function returns an indication of whether or not the SPI bus is
//! busy.This function checks the status of the bus via UCBBUSY bit
//!
//! \param baseAddress is the base address of the EUSCI_A_SPI module.
//!
//! \return One of the following:
//!         - \b EUSCI_A_SPI_BUSY
//!         - \b EUSCI_A_SPI_NOT_BUSY
//!         \n indicating if the EUSCI_A_SPI is busy
//
//*****************************************************************************
uint16_t EUSCI_A_SPI_isBusy(uint32_t baseAddress)
{
        //Return the bus busy status.
        return HWREG16(baseAddress + OFS_UCAxSTATW) & UCBBUSY;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for eusci_a_spi_api
//! @}
//
//*****************************************************************************
