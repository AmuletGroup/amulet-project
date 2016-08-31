/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
/* 
 * ======== HAL_SDCard.c ========
 */

/***************************************************************************//**
 * @file       HAL_SDCard.c
 * @addtogroup HAL_SDCard
 * @{
 ******************************************************************************/
#include "msp430.h"
#include "HAL_SDCard.h"
#include "bsp_init.h"

//Use eUSCIA1
// TODO: Check all clock dependent functions, adjust to 1MHz clock


/***************************************************************************//**
 * @brief   Initialize SD Card
 * @param   None
 * @return  None
 ******************************************************************************/
void SDCard_init (void)
{
    // //Port initialization for SD Card operation
    // SPI_SEL |= SPI_CLK | SPI_SOMI | SPI_SIMO;
    // SPI_DIR |= SPI_CLK | SPI_SIMO;
    // SD_CS_SEL &= ~SD_CS;
    // SD_CS_OUT |= SD_CS;
    // SD_CS_DIR |= SD_CS;

    // //KLQ
    // SPI_REN |= SPI_SOMI | SPI_SIMO;
    // SPI_OUT |= SPI_SOMI | SPI_SIMO;
    // //KLQ

    // //Initialize USCI_B1 for SPI Master operation
    // UCA0CTLW0 |= UCSWRST;                                    //Put state machine in reset
    // UCA0CTLW0 = UCCKPL | UCMSB | UCMST | UCMODE_0 | UCSYNC;  //3-pin, 8-bit SPI master
    // //Clock polarity select - The inactive state is high
    // //MSB first
    // UCA0CTLW0 = UCSWRST | UCSSEL_2;                          //Use SMCLK, keep RESET
    // UCA1BR0 = 3;                                           //Initial SPI clock must be <400kHz
    // UCA1BR1 = 0;                                            //f_UCxCLK = 1MHz/(3+1) = 250kHz
    // UCA0CTLW0 &= ~UCSWRST;                                   //Release USCI state machine
    // UCA1IFG &= ~UCRXIFG;
//************************************************testing for braclet below********************
    //Port initialization for SD Card operation
    SPI_SEL |= SPI_CLK | SPI_SOMI | SPI_SIMO;
    SPI_DIR |= SPI_CLK | SPI_SIMO;
    SD_CS_SEL &= ~SD_CS;
    SD_CS_OUT |= SD_CS;
    SD_CS_DIR |= SD_CS;

    //KLQ
    SPI_REN |= SPI_SOMI | SPI_SIMO;
    SPI_OUT |= SPI_SOMI | SPI_SIMO;
    //KLQ

    // //Initialize USCI_B1 for SPI Master operation
    // UCA0CTLW0 |= UCSWRST;                                    //Put state machine in reset
    // UCA0CTLW0 = UCCKPL | UCMSB | UCMST | UCMODE_0 | UCSYNC;  //3-pin, 8-bit SPI master
    // //Clock polarity select - The inactive state is high
    // //MSB first
    // UCA0CTLW0 = UCSWRST | UCSSEL_2;                          //Use SMCLK, keep RESET
    //UCB0BR0 = 3;                                           //Initial SPI clock must be <400kHz
    //UCB0BR1 = 0;                                            //f_UCxCLK = 1MHz/(3+1) = 250kHz
    //UCA0CTLW0 &= ~UCSWRST;                                   //Release USCI state machine
    //UCB0IFG &= ~UCRXIFG;
}

/***************************************************************************//**
 * @brief   Enable fast SD Card SPI transfers. This function is typically
 *          called after the initial SD Card setup is done to maximize
 *          transfer speed.
 * @param   None
 * @return  None
 ******************************************************************************/
void SDCard_fastMode (void)
{
    // UCA0CTLW0 |= UCSWRST;                                    //Put state machine in reset
    // UCA1BR0 = 2;                                            //f_UCxCLK = 25MHz/2 = 12.5MHz
    // UCA1BR1 = 0;
    // UCA0CTLW0 &= ~UCSWRST;                                   //Release USCI state machine

//************************************************testing for braclet below********************
    //UCA0CTLW0 |= UCSWRST;                                    //Put state machine in reset
    //UCB0BR0 = 2;                                            //f_UCxCLK = 25MHz/2 = 12.5MHz
    //UCB0BR1 = 0;
    //UCA0CTLW0 &= ~UCSWRST;                                   //Release USCI state machine
}

/***************************************************************************//**
 * @brief   Read a frame of bytes via SPI
 * @param   pBuffer Place to store the received bytes
 * @param   size Indicator of how many bytes to receive
 * @return  None
 ******************************************************************************/
void SDCard_readFrame (uint8_t *pBuffer, uint16_t size)
{
    uint16_t gie = __get_SR_register() & GIE;               //Store current GIE state

    __disable_interrupt();                                  //Make this operation atomic

    // UCA1IFG &= ~UCRXIFG;                                    //Ensure RXIFG is clear

    // //Clock the actual data transfer and receive the bytes
    // while (size--){
    //     while (!(UCA1IFG & UCTXIFG)) ;                      //Wait while not ready for TX
    //     UCA1TXBUF = 0xff;                                   //Write dummy byte
    //     while (!(UCA1IFG & UCRXIFG)) ;                      //Wait for RX buffer (full)
    //     *pBuffer++ = UCA1RXBUF;
    // }

//************************************************testing for braclet below********************
    UCB0IFG &= ~UCRXIFG;                                    //Ensure RXIFG is clear

    //Clock the actual data transfer and receive the bytes
    while (size--){
        while (!(UCB0IFG & UCTXIFG)) ;                      //Wait while not ready for TX
        UCB0TXBUF = 0xff;                                   //Write dummy byte
        while (!(UCB0IFG & UCRXIFG)) ;                      //Wait for RX buffer (full)
        *pBuffer++ = UCB0RXBUF;
    }
    __bis_SR_register(gie);                                 //Restore original GIE state
}

/***************************************************************************//**
 * @brief   Send a frame of bytes via SPI
 * @param   pBuffer Place that holds the bytes to send
 * @param   size Indicator of how many bytes to send
 * @return  None
 ******************************************************************************/
void SDCard_sendFrame (uint8_t *pBuffer, uint16_t size)
{
    uint16_t gie = __get_SR_register() & GIE;               //Store current GIE state

    __disable_interrupt();                                  //Make this operation atomic

    //Clock the actual data transfer and send the bytes. Note that we
    //intentionally not read out the receive buffer during frame transmission
    //in order to optimize transfer speed, however we need to take care of the
    //resulting overrun condition.
    // while (size--){
    //     while (!(UCA1IFG & UCTXIFG)) ;                      //Wait while not ready for TX
    //     UCA1TXBUF = *pBuffer++;                             //Write byte
    // }
    // while (UCA1STATW & UCBUSY) ;                             //Wait for all TX/RX to finish

    // UCA1RXBUF;                                              //Dummy read to empty RX buffer
    //                                                         //and clear any overrun conditions

    //************************************************testing for braclet below********************
    while (size--){
        while (!(UCB0IFG & UCTXIFG)) ;                      //Wait while not ready for TX
        UCB0TXBUF = *pBuffer++;                             //Write byte
    }
    while (UCB0STATW & UCBUSY) ;                             //Wait for all TX/RX to finish

    UCB0RXBUF;                                              //Dummy read to empty RX buffer
                                                            //and clear any overrun conditions

    __bis_SR_register(gie);                                 //Restore original GIE state
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to high
 * @param   None
 * @return  None
 ******************************************************************************/
void SDCard_setCSHigh (void)
{
    SD_CS_OUT |= SD_CS;
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to low
 * @param   None
 * @return  None
 ******************************************************************************/
void SDCard_setCSLow (void)
{
    SD_CS_OUT &= ~SD_CS;
}

/***************************************************************************//**
 * @}
 ******************************************************************************/
//Released_Version_4_10_02
