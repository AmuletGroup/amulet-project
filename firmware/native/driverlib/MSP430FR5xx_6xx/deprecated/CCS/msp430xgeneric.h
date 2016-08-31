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
/********************************************************************
*
* Standard register and bit definitions for the Texas Instruments
* MSP430 microcontroller.
*
* This file supports assembler and C development for
* msp430XGENERIC device.
*
* Texas Instruments, Version 1.0 draft
*
* Rev. 1.0, Setup
*
*
********************************************************************/

#ifndef __msp430XGENERIC
#define __msp430XGENERIC

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------*/
/* PERIPHERAL FILE MAP                                                        */
/*----------------------------------------------------------------------------*/

#ifndef SFR_8BIT
/* External references resolved by a device-specific linker command file */
#define SFR_8BIT(address)   extern volatile unsigned char address
#define SFR_16BIT(address)  extern volatile unsigned int address
//#define SFR_20BIT(address)  extern volatile unsigned int address
typedef void (* __SFR_FARPTR)();
#define SFR_20BIT(address) extern __SFR_FARPTR address
#define SFR_32BIT(address)  extern volatile unsigned long address

#endif


/************************************************************
* STANDARD BITS
************************************************************/

#define BIT0                   (0x0001)
#define BIT1                   (0x0002)
#define BIT2                   (0x0004)
#define BIT3                   (0x0008)
#define BIT4                   (0x0010)
#define BIT5                   (0x0020)
#define BIT6                   (0x0040)
#define BIT7                   (0x0080)
#define BIT8                   (0x0100)
#define BIT9                   (0x0200)
#define BITA                   (0x0400)
#define BITB                   (0x0800)
#define BITC                   (0x1000)
#define BITD                   (0x2000)
#define BITE                   (0x4000)
#define BITF                   (0x8000)

/************************************************************
* STATUS REGISTER BITS
************************************************************/

#define C                      (0x0001)
#define Z                      (0x0002)
#define N                      (0x0004)
#define V                      (0x0100)
#define GIE                    (0x0008)
#define CPUOFF                 (0x0010)
#define OSCOFF                 (0x0020)
#define SCG0                   (0x0040)
#define SCG1                   (0x0080)

/* Low Power Modes coded with Bits 4-7 in SR */

#ifdef __ASM_HEADER__ /* Begin #defines for assembler */
#define LPM0                   (CPUOFF)
#define LPM1                   (SCG0+CPUOFF)
#define LPM2                   (SCG1+CPUOFF)
#define LPM3                   (SCG1+SCG0+CPUOFF)
#define LPM4                   (SCG1+SCG0+OSCOFF+CPUOFF)
/* End #defines for assembler */

#else /* Begin #defines for C */
#define LPM0_bits              (CPUOFF)
#define LPM1_bits              (SCG0+CPUOFF)
#define LPM2_bits              (SCG1+CPUOFF)
#define LPM3_bits              (SCG1+SCG0+CPUOFF)
#define LPM4_bits              (SCG1+SCG0+OSCOFF+CPUOFF)

#include "in430.h"

#define LPM0         _bis_SR_register(LPM0_bits)         /* Enter Low Power Mode 0 */
#define LPM0_EXIT    _bic_SR_register_on_exit(LPM0_bits) /* Exit Low Power Mode 0 */
#define LPM1         _bis_SR_register(LPM1_bits)         /* Enter Low Power Mode 1 */
#define LPM1_EXIT    _bic_SR_register_on_exit(LPM1_bits) /* Exit Low Power Mode 1 */
#define LPM2         _bis_SR_register(LPM2_bits)         /* Enter Low Power Mode 2 */
#define LPM2_EXIT    _bic_SR_register_on_exit(LPM2_bits) /* Exit Low Power Mode 2 */
#define LPM3         _bis_SR_register(LPM3_bits)         /* Enter Low Power Mode 3 */
#define LPM3_EXIT    _bic_SR_register_on_exit(LPM3_bits) /* Exit Low Power Mode 3 */
#define LPM4         _bis_SR_register(LPM4_bits)         /* Enter Low Power Mode 4 */
#define LPM4_EXIT    _bic_SR_register_on_exit(LPM4_bits) /* Exit Low Power Mode 4 */
#endif /* End #defines for C */

/************************************************************
* CPU
************************************************************/
#define __MSP430_HAS_MSP430XV2_CPU__                /* Definition to show that it has MSP430XV2 CPU */

/************************************************************
* PERIPHERAL FILE MAP
************************************************************/
/************************************************************
* ADC12_B
************************************************************/
#ifdef  __MSP430_HAS_ADC12_B__         /* Definition to show that Module is available */

#define OFS_ADC12CTL0          (0x0000)       /* ADC12 B Control 0 */
#define OFS_ADC12CTL0_L        OFS_ADC12CTL0
#define OFS_ADC12CTL0_H        OFS_ADC12CTL0+1
#define OFS_ADC12CTL1          (0x0002)       /* ADC12 B Control 1 */
#define OFS_ADC12CTL1_L        OFS_ADC12CTL1
#define OFS_ADC12CTL1_H        OFS_ADC12CTL1+1
#define OFS_ADC12CTL2          (0x0004)       /* ADC12 B Control 2 */
#define OFS_ADC12CTL2_L        OFS_ADC12CTL2
#define OFS_ADC12CTL2_H        OFS_ADC12CTL2+1
#define OFS_ADC12CTL3          (0x0006)       /* ADC12 B Control 3 */
#define OFS_ADC12CTL3_L        OFS_ADC12CTL3
#define OFS_ADC12CTL3_H        OFS_ADC12CTL3+1
#define OFS_ADC12LO            (0x0008)       /* ADC12 B Window Comparator High Threshold */
#define OFS_ADC12LO_L          OFS_ADC12LO
#define OFS_ADC12LO_H          OFS_ADC12LO+1
#define OFS_ADC12HI            (0x000A)       /* ADC12 B Window Comparator High Threshold */
#define OFS_ADC12HI_L          OFS_ADC12HI
#define OFS_ADC12HI_H          OFS_ADC12HI+1
#define OFS_ADC12IFGR0         (0x000C)       /* ADC12 B Interrupt Flag 0 */
#define OFS_ADC12IFGR0_L       OFS_ADC12IFGR0
#define OFS_ADC12IFGR0_H       OFS_ADC12IFGR0+1
#define OFS_ADC12IFGR1         (0x000E)       /* ADC12 B Interrupt Flag 1 */
#define OFS_ADC12IFGR1_L       OFS_ADC12IFGR1
#define OFS_ADC12IFGR1_H       OFS_ADC12IFGR1+1
#define OFS_ADC12IFGR2         (0x0010)       /* ADC12 B Interrupt Flag 2 */
#define OFS_ADC12IFGR2_L       OFS_ADC12IFGR2
#define OFS_ADC12IFGR2_H       OFS_ADC12IFGR2+1
#define OFS_ADC12IER0          (0x0012)       /* ADC12 B Interrupt Enable 0 */
#define OFS_ADC12IER0_L        OFS_ADC12IER0
#define OFS_ADC12IER0_H        OFS_ADC12IER0+1
#define OFS_ADC12IER1          (0x0014)       /* ADC12 B Interrupt Enable 1 */
#define OFS_ADC12IER1_L        OFS_ADC12IER1
#define OFS_ADC12IER1_H        OFS_ADC12IER1+1
#define OFS_ADC12IER2          (0x0016)       /* ADC12 B Interrupt Enable 2 */
#define OFS_ADC12IER2_L        OFS_ADC12IER2
#define OFS_ADC12IER2_H        OFS_ADC12IER2+1
#define OFS_ADC12IV            (0x0018)       /* ADC12 B Interrupt Vector Word */
#define OFS_ADC12IV_L          OFS_ADC12IV
#define OFS_ADC12IV_H          OFS_ADC12IV+1

#define OFS_ADC12MCTL0         (0x0020)       /* ADC12 Memory Control 0 */
#define OFS_ADC12MCTL0_L       OFS_ADC12MCTL0
#define OFS_ADC12MCTL0_H       OFS_ADC12MCTL0+1
#define OFS_ADC12MCTL1         (0x0022)       /* ADC12 Memory Control 1 */
#define OFS_ADC12MCTL1_L       OFS_ADC12MCTL1
#define OFS_ADC12MCTL1_H       OFS_ADC12MCTL1+1
#define OFS_ADC12MCTL2         (0x0024)       /* ADC12 Memory Control 2 */
#define OFS_ADC12MCTL2_L       OFS_ADC12MCTL2
#define OFS_ADC12MCTL2_H       OFS_ADC12MCTL2+1
#define OFS_ADC12MCTL3         (0x0026)       /* ADC12 Memory Control 3 */
#define OFS_ADC12MCTL3_L       OFS_ADC12MCTL3
#define OFS_ADC12MCTL3_H       OFS_ADC12MCTL3+1
#define OFS_ADC12MCTL4         (0x0028)       /* ADC12 Memory Control 4 */
#define OFS_ADC12MCTL4_L       OFS_ADC12MCTL4
#define OFS_ADC12MCTL4_H       OFS_ADC12MCTL4+1
#define OFS_ADC12MCTL5         (0x002A)       /* ADC12 Memory Control 5 */
#define OFS_ADC12MCTL5_L       OFS_ADC12MCTL5
#define OFS_ADC12MCTL5_H       OFS_ADC12MCTL5+1
#define OFS_ADC12MCTL6         (0x002C)       /* ADC12 Memory Control 6 */
#define OFS_ADC12MCTL6_L       OFS_ADC12MCTL6
#define OFS_ADC12MCTL6_H       OFS_ADC12MCTL6+1
#define OFS_ADC12MCTL7         (0x002E)       /* ADC12 Memory Control 7 */
#define OFS_ADC12MCTL7_L       OFS_ADC12MCTL7
#define OFS_ADC12MCTL7_H       OFS_ADC12MCTL7+1
#define OFS_ADC12MCTL8         (0x0030)       /* ADC12 Memory Control 8 */
#define OFS_ADC12MCTL8_L       OFS_ADC12MCTL8
#define OFS_ADC12MCTL8_H       OFS_ADC12MCTL8+1
#define OFS_ADC12MCTL9         (0x0032)       /* ADC12 Memory Control 9 */
#define OFS_ADC12MCTL9_L       OFS_ADC12MCTL9
#define OFS_ADC12MCTL9_H       OFS_ADC12MCTL9+1
#define OFS_ADC12MCTL10        (0x0034)       /* ADC12 Memory Control 10 */
#define OFS_ADC12MCTL10_L      OFS_ADC12MCTL10
#define OFS_ADC12MCTL10_H      OFS_ADC12MCTL10+1
#define OFS_ADC12MCTL11        (0x0036)       /* ADC12 Memory Control 11 */
#define OFS_ADC12MCTL11_L      OFS_ADC12MCTL11
#define OFS_ADC12MCTL11_H      OFS_ADC12MCTL11+1
#define OFS_ADC12MCTL12        (0x0038)       /* ADC12 Memory Control 12 */
#define OFS_ADC12MCTL12_L      OFS_ADC12MCTL12
#define OFS_ADC12MCTL12_H      OFS_ADC12MCTL12+1
#define OFS_ADC12MCTL13        (0x003A)       /* ADC12 Memory Control 13 */
#define OFS_ADC12MCTL13_L      OFS_ADC12MCTL13
#define OFS_ADC12MCTL13_H      OFS_ADC12MCTL13+1
#define OFS_ADC12MCTL14        (0x003C)       /* ADC12 Memory Control 14 */
#define OFS_ADC12MCTL14_L      OFS_ADC12MCTL14
#define OFS_ADC12MCTL14_H      OFS_ADC12MCTL14+1
#define OFS_ADC12MCTL15        (0x003E)       /* ADC12 Memory Control 15 */
#define OFS_ADC12MCTL15_L      OFS_ADC12MCTL15
#define OFS_ADC12MCTL15_H      OFS_ADC12MCTL15+1
#define OFS_ADC12MCTL16        (0x0040)       /* ADC12 Memory Control 16 */
#define OFS_ADC12MCTL16_L      OFS_ADC12MCTL16
#define OFS_ADC12MCTL16_H      OFS_ADC12MCTL16+1
#define OFS_ADC12MCTL17        (0x0042)       /* ADC12 Memory Control 17 */
#define OFS_ADC12MCTL17_L      OFS_ADC12MCTL17
#define OFS_ADC12MCTL17_H      OFS_ADC12MCTL17+1
#define OFS_ADC12MCTL18        (0x0044)       /* ADC12 Memory Control 18 */
#define OFS_ADC12MCTL18_L      OFS_ADC12MCTL18
#define OFS_ADC12MCTL18_H      OFS_ADC12MCTL18+1
#define OFS_ADC12MCTL19        (0x0046)       /* ADC12 Memory Control 19 */
#define OFS_ADC12MCTL19_L      OFS_ADC12MCTL19
#define OFS_ADC12MCTL19_H      OFS_ADC12MCTL19+1
#define OFS_ADC12MCTL20        (0x0048)       /* ADC12 Memory Control 20 */
#define OFS_ADC12MCTL20_L      OFS_ADC12MCTL20
#define OFS_ADC12MCTL20_H      OFS_ADC12MCTL20+1
#define OFS_ADC12MCTL21        (0x004A)       /* ADC12 Memory Control 21 */
#define OFS_ADC12MCTL21_L      OFS_ADC12MCTL21
#define OFS_ADC12MCTL21_H      OFS_ADC12MCTL21+1
#define OFS_ADC12MCTL22        (0x004C)       /* ADC12 Memory Control 22 */
#define OFS_ADC12MCTL22_L      OFS_ADC12MCTL22
#define OFS_ADC12MCTL22_H      OFS_ADC12MCTL22+1
#define OFS_ADC12MCTL23        (0x004E)       /* ADC12 Memory Control 23 */
#define OFS_ADC12MCTL23_L      OFS_ADC12MCTL23
#define OFS_ADC12MCTL23_H      OFS_ADC12MCTL23+1
#define OFS_ADC12MCTL24        (0x0050)       /* ADC12 Memory Control 24 */
#define OFS_ADC12MCTL24_L      OFS_ADC12MCTL24
#define OFS_ADC12MCTL24_H      OFS_ADC12MCTL24+1
#define OFS_ADC12MCTL25        (0x0052)       /* ADC12 Memory Control 25 */
#define OFS_ADC12MCTL25_L      OFS_ADC12MCTL25
#define OFS_ADC12MCTL25_H      OFS_ADC12MCTL25+1
#define OFS_ADC12MCTL26        (0x0054)       /* ADC12 Memory Control 26 */
#define OFS_ADC12MCTL26_L      OFS_ADC12MCTL26
#define OFS_ADC12MCTL26_H      OFS_ADC12MCTL26+1
#define OFS_ADC12MCTL27        (0x0056)       /* ADC12 Memory Control 27 */
#define OFS_ADC12MCTL27_L      OFS_ADC12MCTL27
#define OFS_ADC12MCTL27_H      OFS_ADC12MCTL27+1
#define OFS_ADC12MCTL28        (0x0058)       /* ADC12 Memory Control 28 */
#define OFS_ADC12MCTL28_L      OFS_ADC12MCTL28
#define OFS_ADC12MCTL28_H      OFS_ADC12MCTL28+1
#define OFS_ADC12MCTL29        (0x005A)       /* ADC12 Memory Control 29 */
#define OFS_ADC12MCTL29_L      OFS_ADC12MCTL29
#define OFS_ADC12MCTL29_H      OFS_ADC12MCTL29+1
#define OFS_ADC12MCTL30        (0x005C)       /* ADC12 Memory Control 30 */
#define OFS_ADC12MCTL30_L      OFS_ADC12MCTL30
#define OFS_ADC12MCTL30_H      OFS_ADC12MCTL30+1
#define OFS_ADC12MCTL31        (0x005E)       /* ADC12 Memory Control 31 */
#define OFS_ADC12MCTL31_L      OFS_ADC12MCTL31
#define OFS_ADC12MCTL31_H      OFS_ADC12MCTL31+1
#define ADC12MCTL_             ADC12MCTL      /* ADC12 Memory Control */
#ifdef __ASM_HEADER__
#define ADC12MCTL              ADC12MCTL0     /* ADC12 Memory Control (for assembler) */
#else
#define ADC12MCTL              ((char*)       &ADC12MCTL0) /* ADC12 Memory Control (for C) */
#endif

#define OFS_ADC12MEM0          (0x0060)       /* ADC12 Conversion Memory 0 */
#define OFS_ADC12MEM0_L        OFS_ADC12MEM0
#define OFS_ADC12MEM0_H        OFS_ADC12MEM0+1
#define OFS_ADC12MEM1          (0x0062)       /* ADC12 Conversion Memory 1 */
#define OFS_ADC12MEM1_L        OFS_ADC12MEM1
#define OFS_ADC12MEM1_H        OFS_ADC12MEM1+1
#define OFS_ADC12MEM2          (0x0064)       /* ADC12 Conversion Memory 2 */
#define OFS_ADC12MEM2_L        OFS_ADC12MEM2
#define OFS_ADC12MEM2_H        OFS_ADC12MEM2+1
#define OFS_ADC12MEM3          (0x0066)       /* ADC12 Conversion Memory 3 */
#define OFS_ADC12MEM3_L        OFS_ADC12MEM3
#define OFS_ADC12MEM3_H        OFS_ADC12MEM3+1
#define OFS_ADC12MEM4          (0x0068)       /* ADC12 Conversion Memory 4 */
#define OFS_ADC12MEM4_L        OFS_ADC12MEM4
#define OFS_ADC12MEM4_H        OFS_ADC12MEM4+1
#define OFS_ADC12MEM5          (0x006A)       /* ADC12 Conversion Memory 5 */
#define OFS_ADC12MEM5_L        OFS_ADC12MEM5
#define OFS_ADC12MEM5_H        OFS_ADC12MEM5+1
#define OFS_ADC12MEM6          (0x006C)       /* ADC12 Conversion Memory 6 */
#define OFS_ADC12MEM6_L        OFS_ADC12MEM6
#define OFS_ADC12MEM6_H        OFS_ADC12MEM6+1
#define OFS_ADC12MEM7          (0x006E)       /* ADC12 Conversion Memory 7 */
#define OFS_ADC12MEM7_L        OFS_ADC12MEM7
#define OFS_ADC12MEM7_H        OFS_ADC12MEM7+1
#define OFS_ADC12MEM8          (0x0070)       /* ADC12 Conversion Memory 8 */
#define OFS_ADC12MEM8_L        OFS_ADC12MEM8
#define OFS_ADC12MEM8_H        OFS_ADC12MEM8+1
#define OFS_ADC12MEM9          (0x0072)       /* ADC12 Conversion Memory 9 */
#define OFS_ADC12MEM9_L        OFS_ADC12MEM9
#define OFS_ADC12MEM9_H        OFS_ADC12MEM9+1
#define OFS_ADC12MEM10         (0x0074)       /* ADC12 Conversion Memory 10 */
#define OFS_ADC12MEM10_L       OFS_ADC12MEM10
#define OFS_ADC12MEM10_H       OFS_ADC12MEM10+1
#define OFS_ADC12MEM11         (0x0076)       /* ADC12 Conversion Memory 11 */
#define OFS_ADC12MEM11_L       OFS_ADC12MEM11
#define OFS_ADC12MEM11_H       OFS_ADC12MEM11+1
#define OFS_ADC12MEM12         (0x0078)       /* ADC12 Conversion Memory 12 */
#define OFS_ADC12MEM12_L       OFS_ADC12MEM12
#define OFS_ADC12MEM12_H       OFS_ADC12MEM12+1
#define OFS_ADC12MEM13         (0x007A)       /* ADC12 Conversion Memory 13 */
#define OFS_ADC12MEM13_L       OFS_ADC12MEM13
#define OFS_ADC12MEM13_H       OFS_ADC12MEM13+1
#define OFS_ADC12MEM14         (0x007C)       /* ADC12 Conversion Memory 14 */
#define OFS_ADC12MEM14_L       OFS_ADC12MEM14
#define OFS_ADC12MEM14_H       OFS_ADC12MEM14+1
#define OFS_ADC12MEM15         (0x007E)       /* ADC12 Conversion Memory 15 */
#define OFS_ADC12MEM15_L       OFS_ADC12MEM15
#define OFS_ADC12MEM15_H       OFS_ADC12MEM15+1
#define OFS_ADC12MEM16         (0x0080)       /* ADC12 Conversion Memory 16 */
#define OFS_ADC12MEM16_L       OFS_ADC12MEM16
#define OFS_ADC12MEM16_H       OFS_ADC12MEM16+1
#define OFS_ADC12MEM17         (0x0082)       /* ADC12 Conversion Memory 17 */
#define OFS_ADC12MEM17_L       OFS_ADC12MEM17
#define OFS_ADC12MEM17_H       OFS_ADC12MEM17+1
#define OFS_ADC12MEM18         (0x0084)       /* ADC12 Conversion Memory 18 */
#define OFS_ADC12MEM18_L       OFS_ADC12MEM18
#define OFS_ADC12MEM18_H       OFS_ADC12MEM18+1
#define OFS_ADC12MEM19         (0x0086)       /* ADC12 Conversion Memory 19 */
#define OFS_ADC12MEM19_L       OFS_ADC12MEM19
#define OFS_ADC12MEM19_H       OFS_ADC12MEM19+1
#define OFS_ADC12MEM20         (0x0088)       /* ADC12 Conversion Memory 20 */
#define OFS_ADC12MEM20_L       OFS_ADC12MEM20
#define OFS_ADC12MEM20_H       OFS_ADC12MEM20+1
#define OFS_ADC12MEM21         (0x008A)       /* ADC12 Conversion Memory 21 */
#define OFS_ADC12MEM21_L       OFS_ADC12MEM21
#define OFS_ADC12MEM21_H       OFS_ADC12MEM21+1
#define OFS_ADC12MEM22         (0x008C)       /* ADC12 Conversion Memory 22 */
#define OFS_ADC12MEM22_L       OFS_ADC12MEM22
#define OFS_ADC12MEM22_H       OFS_ADC12MEM22+1
#define OFS_ADC12MEM23         (0x008E)       /* ADC12 Conversion Memory 23 */
#define OFS_ADC12MEM23_L       OFS_ADC12MEM23
#define OFS_ADC12MEM23_H       OFS_ADC12MEM23+1
#define OFS_ADC12MEM24         (0x0090)       /* ADC12 Conversion Memory 24 */
#define OFS_ADC12MEM24_L       OFS_ADC12MEM24
#define OFS_ADC12MEM24_H       OFS_ADC12MEM24+1
#define OFS_ADC12MEM25         (0x0092)       /* ADC12 Conversion Memory 25 */
#define OFS_ADC12MEM25_L       OFS_ADC12MEM25
#define OFS_ADC12MEM25_H       OFS_ADC12MEM25+1
#define OFS_ADC12MEM26         (0x0094)       /* ADC12 Conversion Memory 26 */
#define OFS_ADC12MEM26_L       OFS_ADC12MEM26
#define OFS_ADC12MEM26_H       OFS_ADC12MEM26+1
#define OFS_ADC12MEM27         (0x0096)       /* ADC12 Conversion Memory 27 */
#define OFS_ADC12MEM27_L       OFS_ADC12MEM27
#define OFS_ADC12MEM27_H       OFS_ADC12MEM27+1
#define OFS_ADC12MEM28         (0x0098)       /* ADC12 Conversion Memory 28 */
#define OFS_ADC12MEM28_L       OFS_ADC12MEM28
#define OFS_ADC12MEM28_H       OFS_ADC12MEM28+1
#define OFS_ADC12MEM29         (0x009A)       /* ADC12 Conversion Memory 29 */
#define OFS_ADC12MEM29_L       OFS_ADC12MEM29
#define OFS_ADC12MEM29_H       OFS_ADC12MEM29+1
#define OFS_ADC12MEM30         (0x009C)       /* ADC12 Conversion Memory 30 */
#define OFS_ADC12MEM30_L       OFS_ADC12MEM30
#define OFS_ADC12MEM30_H       OFS_ADC12MEM30+1
#define OFS_ADC12MEM31         (0x009E)       /* ADC12 Conversion Memory 31 */
#define OFS_ADC12MEM31_L       OFS_ADC12MEM31
#define OFS_ADC12MEM31_H       OFS_ADC12MEM31+1
#define ADC12MEM_              ADC12MEM       /* ADC12 Conversion Memory */
#ifdef __ASM_HEADER__
#define ADC12MEM               ADC12MEM0      /* ADC12 Conversion Memory (for assembler) */
#else
#define ADC12MEM               ((int*)        &ADC12MEM0) /* ADC12 Conversion Memory (for C) */
#endif

/* ADC12CTL0 Control Bits */
#define ADC12SC                (0x0001)       /* ADC12 Start Conversion */
#define ADC12ENC               (0x0002)       /* ADC12 Enable Conversion */
#define ADC12ON                (0x0010)       /* ADC12 On/enable */
#define ADC12MSC               (0x0080)       /* ADC12 Multiple SampleConversion */
#define ADC12SHT00             (0x0100)       /* ADC12 Sample Hold 0 Select Bit: 0 */
#define ADC12SHT01             (0x0200)       /* ADC12 Sample Hold 0 Select Bit: 1 */
#define ADC12SHT02             (0x0400)       /* ADC12 Sample Hold 0 Select Bit: 2 */
#define ADC12SHT03             (0x0800)       /* ADC12 Sample Hold 0 Select Bit: 3 */
#define ADC12SHT10             (0x1000)       /* ADC12 Sample Hold 1 Select Bit: 0 */
#define ADC12SHT11             (0x2000)       /* ADC12 Sample Hold 1 Select Bit: 1 */
#define ADC12SHT12             (0x4000)       /* ADC12 Sample Hold 1 Select Bit: 2 */
#define ADC12SHT13             (0x8000)       /* ADC12 Sample Hold 1 Select Bit: 3 */

/* ADC12CTL0 Control Bits */
#define ADC12SC_L              (0x0001)       /* ADC12 Start Conversion */
#define ADC12ENC_L             (0x0002)       /* ADC12 Enable Conversion */
#define ADC12ON_L              (0x0010)       /* ADC12 On/enable */
#define ADC12MSC_L             (0x0080)       /* ADC12 Multiple SampleConversion */

/* ADC12CTL0 Control Bits */
#define ADC12SHT00_H           (0x0001)       /* ADC12 Sample Hold 0 Select Bit: 0 */
#define ADC12SHT01_H           (0x0002)       /* ADC12 Sample Hold 0 Select Bit: 1 */
#define ADC12SHT02_H           (0x0004)       /* ADC12 Sample Hold 0 Select Bit: 2 */
#define ADC12SHT03_H           (0x0008)       /* ADC12 Sample Hold 0 Select Bit: 3 */
#define ADC12SHT10_H           (0x0010)       /* ADC12 Sample Hold 1 Select Bit: 0 */
#define ADC12SHT11_H           (0x0020)       /* ADC12 Sample Hold 1 Select Bit: 1 */
#define ADC12SHT12_H           (0x0040)       /* ADC12 Sample Hold 1 Select Bit: 2 */
#define ADC12SHT13_H           (0x0080)       /* ADC12 Sample Hold 1 Select Bit: 3 */

#define ADC12SHT0_0            (0*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 0 */
#define ADC12SHT0_1            (1*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 1 */
#define ADC12SHT0_2            (2*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 2 */
#define ADC12SHT0_3            (3*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 3 */
#define ADC12SHT0_4            (4*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 4 */
#define ADC12SHT0_5            (5*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 5 */
#define ADC12SHT0_6            (6*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 6 */
#define ADC12SHT0_7            (7*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 7 */
#define ADC12SHT0_8            (8*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 8 */
#define ADC12SHT0_9            (9*0x100u)     /* ADC12 Sample Hold 0 Select Bit: 9 */
#define ADC12SHT0_10           (10*0x100u)    /* ADC12 Sample Hold 0 Select Bit: 10 */
#define ADC12SHT0_11           (11*0x100u)    /* ADC12 Sample Hold 0 Select Bit: 11 */
#define ADC12SHT0_12           (12*0x100u)    /* ADC12 Sample Hold 0 Select Bit: 12 */
#define ADC12SHT0_13           (13*0x100u)    /* ADC12 Sample Hold 0 Select Bit: 13 */
#define ADC12SHT0_14           (14*0x100u)    /* ADC12 Sample Hold 0 Select Bit: 14 */
#define ADC12SHT0_15           (15*0x100u)    /* ADC12 Sample Hold 0 Select Bit: 15 */

#define ADC12SHT1_0            (0*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 0 */
#define ADC12SHT1_1            (1*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 1 */
#define ADC12SHT1_2            (2*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 2 */
#define ADC12SHT1_3            (3*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 3 */
#define ADC12SHT1_4            (4*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 4 */
#define ADC12SHT1_5            (5*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 5 */
#define ADC12SHT1_6            (6*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 6 */
#define ADC12SHT1_7            (7*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 7 */
#define ADC12SHT1_8            (8*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 8 */
#define ADC12SHT1_9            (9*0x1000u)    /* ADC12 Sample Hold 1 Select Bit: 9 */
#define ADC12SHT1_10           (10*0x1000u)   /* ADC12 Sample Hold 1 Select Bit: 10 */
#define ADC12SHT1_11           (11*0x1000u)   /* ADC12 Sample Hold 1 Select Bit: 11 */
#define ADC12SHT1_12           (12*0x1000u)   /* ADC12 Sample Hold 1 Select Bit: 12 */
#define ADC12SHT1_13           (13*0x1000u)   /* ADC12 Sample Hold 1 Select Bit: 13 */
#define ADC12SHT1_14           (14*0x1000u)   /* ADC12 Sample Hold 1 Select Bit: 14 */
#define ADC12SHT1_15           (15*0x1000u)   /* ADC12 Sample Hold 1 Select Bit: 15 */

/* ADC12CTL1 Control Bits */
#define ADC12BUSY              (0x0001)       /* ADC12 Busy */
#define ADC12CONSEQ0           (0x0002)       /* ADC12 Conversion Sequence Select Bit: 0 */
#define ADC12CONSEQ1           (0x0004)       /* ADC12 Conversion Sequence Select Bit: 1 */
#define ADC12SSEL0             (0x0008)       /* ADC12 Clock Source Select Bit: 0 */
#define ADC12SSEL1             (0x0010)       /* ADC12 Clock Source Select Bit: 1 */
#define ADC12DIV0              (0x0020)       /* ADC12 Clock Divider Select Bit: 0 */
#define ADC12DIV1              (0x0040)       /* ADC12 Clock Divider Select Bit: 1 */
#define ADC12DIV2              (0x0080)       /* ADC12 Clock Divider Select Bit: 2 */
#define ADC12ISSH              (0x0100)       /* ADC12 Invert Sample Hold Signal */
#define ADC12SHP               (0x0200)       /* ADC12 Sample/Hold Pulse Mode */
#define ADC12SHS0              (0x0400)       /* ADC12 Sample/Hold Source Bit: 0 */
#define ADC12SHS1              (0x0800)       /* ADC12 Sample/Hold Source Bit: 1 */
#define ADC12SHS2              (0x1000)       /* ADC12 Sample/Hold Source Bit: 2 */
#define ADC12PDIV0             (0x2000)       /* ADC12 Predivider Bit: 0 */
#define ADC12PDIV1             (0x4000)       /* ADC12 Predivider Bit: 1 */

/* ADC12CTL1 Control Bits */
#define ADC12BUSY_L            (0x0001)       /* ADC12 Busy */
#define ADC12CONSEQ0_L         (0x0002)       /* ADC12 Conversion Sequence Select Bit: 0 */
#define ADC12CONSEQ1_L         (0x0004)       /* ADC12 Conversion Sequence Select Bit: 1 */
#define ADC12SSEL0_L           (0x0008)       /* ADC12 Clock Source Select Bit: 0 */
#define ADC12SSEL1_L           (0x0010)       /* ADC12 Clock Source Select Bit: 1 */
#define ADC12DIV0_L            (0x0020)       /* ADC12 Clock Divider Select Bit: 0 */
#define ADC12DIV1_L            (0x0040)       /* ADC12 Clock Divider Select Bit: 1 */
#define ADC12DIV2_L            (0x0080)       /* ADC12 Clock Divider Select Bit: 2 */

/* ADC12CTL1 Control Bits */
#define ADC12ISSH_H            (0x0001)       /* ADC12 Invert Sample Hold Signal */
#define ADC12SHP_H             (0x0002)       /* ADC12 Sample/Hold Pulse Mode */
#define ADC12SHS0_H            (0x0004)       /* ADC12 Sample/Hold Source Bit: 0 */
#define ADC12SHS1_H            (0x0008)       /* ADC12 Sample/Hold Source Bit: 1 */
#define ADC12SHS2_H            (0x0010)       /* ADC12 Sample/Hold Source Bit: 2 */
#define ADC12PDIV0_H           (0x0020)       /* ADC12 Predivider Bit: 0 */
#define ADC12PDIV1_H           (0x0040)       /* ADC12 Predivider Bit: 1 */

#define ADC12CONSEQ_0          (0*0x0002u)    /* ADC12 Conversion Sequence Select: 0 */
#define ADC12CONSEQ_1          (1*0x0002u)    /* ADC12 Conversion Sequence Select: 1 */
#define ADC12CONSEQ_2          (2*0x0002u)    /* ADC12 Conversion Sequence Select: 2 */
#define ADC12CONSEQ_3          (3*0x0002u)    /* ADC12 Conversion Sequence Select: 3 */

#define ADC12SSEL_0            (0*0x0008u)    /* ADC12 Clock Source Select: 0 */
#define ADC12SSEL_1            (1*0x0008u)    /* ADC12 Clock Source Select: 1 */
#define ADC12SSEL_2            (2*0x0008u)    /* ADC12 Clock Source Select: 2 */
#define ADC12SSEL_3            (3*0x0008u)    /* ADC12 Clock Source Select: 3 */

#define ADC12DIV_0             (0*0x0020u)    /* ADC12 Clock Divider Select: 0 */
#define ADC12DIV_1             (1*0x0020u)    /* ADC12 Clock Divider Select: 1 */
#define ADC12DIV_2             (2*0x0020u)    /* ADC12 Clock Divider Select: 2 */
#define ADC12DIV_3             (3*0x0020u)    /* ADC12 Clock Divider Select: 3 */
#define ADC12DIV_4             (4*0x0020u)    /* ADC12 Clock Divider Select: 4 */
#define ADC12DIV_5             (5*0x0020u)    /* ADC12 Clock Divider Select: 5 */
#define ADC12DIV_6             (6*0x0020u)    /* ADC12 Clock Divider Select: 6 */
#define ADC12DIV_7             (7*0x0020u)    /* ADC12 Clock Divider Select: 7 */

#define ADC12SHS_0             (0*0x0400u)    /* ADC12 Sample/Hold Source: 0 */
#define ADC12SHS_1             (1*0x0400u)    /* ADC12 Sample/Hold Source: 1 */
#define ADC12SHS_2             (2*0x0400u)    /* ADC12 Sample/Hold Source: 2 */
#define ADC12SHS_3             (3*0x0400u)    /* ADC12 Sample/Hold Source: 3 */
#define ADC12SHS_4             (4*0x0400u)    /* ADC12 Sample/Hold Source: 4 */
#define ADC12SHS_5             (5*0x0400u)    /* ADC12 Sample/Hold Source: 5 */
#define ADC12SHS_6             (6*0x0400u)    /* ADC12 Sample/Hold Source: 6 */
#define ADC12SHS_7             (7*0x0400u)    /* ADC12 Sample/Hold Source: 7 */

#define ADC12PDIV_0            (0*0x2000u)    /* ADC12 Clock predivider Select 0 */
#define ADC12PDIV_1            (1*0x2000u)    /* ADC12 Clock predivider Select 1 */
#define ADC12PDIV_2            (2*0x2000u)    /* ADC12 Clock predivider Select 2 */
#define ADC12PDIV_3            (3*0x2000u)    /* ADC12 Clock predivider Select 3 */
#define ADC12PDIV__1           (0*0x2000u)    /* ADC12 Clock predivider Select: /1 */
#define ADC12PDIV__4           (1*0x2000u)    /* ADC12 Clock predivider Select: /4 */
#define ADC12PDIV__32          (2*0x2000u)    /* ADC12 Clock predivider Select: /32 */
#define ADC12PDIV__64          (3*0x2000u)    /* ADC12 Clock predivider Select: /64 */

/* ADC12CTL2 Control Bits */
#define ADC12PWRMD             (0x0001)       /* ADC12 Power Mode */
#define ADC12DF                (0x0008)       /* ADC12 Data Format */
#define ADC12RES0              (0x0010)       /* ADC12 Resolution Bit: 0 */
#define ADC12RES1              (0x0020)       /* ADC12 Resolution Bit: 1 */

/* ADC12CTL2 Control Bits */
#define ADC12PWRMD_L           (0x0001)       /* ADC12 Power Mode */
#define ADC12DF_L              (0x0008)       /* ADC12 Data Format */
#define ADC12RES0_L            (0x0010)       /* ADC12 Resolution Bit: 0 */
#define ADC12RES1_L            (0x0020)       /* ADC12 Resolution Bit: 1 */

/* ADC12CTL2 Control Bits */

#define ADC12RES_0             (0x0000)       /* ADC12+ Resolution : 8 Bit */
#define ADC12RES_1             (0x0010)       /* ADC12+ Resolution : 10 Bit */
#define ADC12RES_2             (0x0020)       /* ADC12+ Resolution : 12 Bit */
#define ADC12RES_3             (0x0030)       /* ADC12+ Resolution : reserved */

#define ADC12RES__8BIT         (0x0000)       /* ADC12+ Resolution : 8 Bit */
#define ADC12RES__10BIT        (0x0010)       /* ADC12+ Resolution : 10 Bit */
#define ADC12RES__12BIT        (0x0020)       /* ADC12+ Resolution : 12 Bit */

/* ADC12CTL3 Control Bits */
#define ADC12CSTARTADD0        (0x0001)       /* ADC12 Conversion Start Address Bit: 0 */
#define ADC12CSTARTADD1        (0x0002)       /* ADC12 Conversion Start Address Bit: 1 */
#define ADC12CSTARTADD2        (0x0004)       /* ADC12 Conversion Start Address Bit: 2 */
#define ADC12CSTARTADD3        (0x0008)       /* ADC12 Conversion Start Address Bit: 3 */
#define ADC12CSTARTADD4        (0x0010)       /* ADC12 Conversion Start Address Bit: 4 */
#define ADC12BATMAP            (0x0040)       /* ADC12 Internal AVCC/2 select */
#define ADC12TCMAP             (0x0080)       /* ADC12 Internal TempSensor select */
#define ADC12ICH0MAP           (0x0100)       /* ADC12 Internal Channel 0 select */
#define ADC12ICH1MAP           (0x0200)       /* ADC12 Internal Channel 1 select */
#define ADC12ICH2MAP           (0x0400)       /* ADC12 Internal Channel 2 select */
#define ADC12ICH3MAP           (0x0800)       /* ADC12 Internal Channel 3 select */

/* ADC12CTL3 Control Bits */
#define ADC12CSTARTADD0_L      (0x0001)       /* ADC12 Conversion Start Address Bit: 0 */
#define ADC12CSTARTADD1_L      (0x0002)       /* ADC12 Conversion Start Address Bit: 1 */
#define ADC12CSTARTADD2_L      (0x0004)       /* ADC12 Conversion Start Address Bit: 2 */
#define ADC12CSTARTADD3_L      (0x0008)       /* ADC12 Conversion Start Address Bit: 3 */
#define ADC12CSTARTADD4_L      (0x0010)       /* ADC12 Conversion Start Address Bit: 4 */
#define ADC12BATMAP_L          (0x0040)       /* ADC12 Internal AVCC/2 select */
#define ADC12TCMAP_L           (0x0080)       /* ADC12 Internal TempSensor select */

/* ADC12CTL3 Control Bits */
#define ADC12ICH0MAP_H         (0x0001)       /* ADC12 Internal Channel 0 select */
#define ADC12ICH1MAP_H         (0x0002)       /* ADC12 Internal Channel 1 select */
#define ADC12ICH2MAP_H         (0x0004)       /* ADC12 Internal Channel 2 select */
#define ADC12ICH3MAP_H         (0x0008)       /* ADC12 Internal Channel 3 select */

#define ADC12CSTARTADD_0       (              0*0x0001u) /* ADC12 Conversion Start Address: 0 */
#define ADC12CSTARTADD_1       (              1*0x0001u) /* ADC12 Conversion Start Address: 1 */
#define ADC12CSTARTADD_2       (              2*0x0001u) /* ADC12 Conversion Start Address: 2 */
#define ADC12CSTARTADD_3       (              3*0x0001u) /* ADC12 Conversion Start Address: 3 */
#define ADC12CSTARTADD_4       (              4*0x0001u) /* ADC12 Conversion Start Address: 4 */
#define ADC12CSTARTADD_5       (              5*0x0001u) /* ADC12 Conversion Start Address: 5 */
#define ADC12CSTARTADD_6       (              6*0x0001u) /* ADC12 Conversion Start Address: 6 */
#define ADC12CSTARTADD_7       (              7*0x0001u) /* ADC12 Conversion Start Address: 7 */
#define ADC12CSTARTADD_8       (              8*0x0001u) /* ADC12 Conversion Start Address: 8 */
#define ADC12CSTARTADD_9       (              9*0x0001u) /* ADC12 Conversion Start Address: 9 */
#define ADC12CSTARTADD_10      (10*0x0001u)   /* ADC12 Conversion Start Address: 10 */
#define ADC12CSTARTADD_11      (11*0x0001u)   /* ADC12 Conversion Start Address: 11 */
#define ADC12CSTARTADD_12      (12*0x0001u)   /* ADC12 Conversion Start Address: 12 */
#define ADC12CSTARTADD_13      (13*0x0001u)   /* ADC12 Conversion Start Address: 13 */
#define ADC12CSTARTADD_14      (14*0x0001u)   /* ADC12 Conversion Start Address: 14 */
#define ADC12CSTARTADD_15      (15*0x0001u)   /* ADC12 Conversion Start Address: 15 */
#define ADC12CSTARTADD_16      (16*0x0001u)   /* ADC12 Conversion Start Address: 16 */
#define ADC12CSTARTADD_17      (17*0x0001u)   /* ADC12 Conversion Start Address: 17 */
#define ADC12CSTARTADD_18      (18*0x0001u)   /* ADC12 Conversion Start Address: 18 */
#define ADC12CSTARTADD_19      (19*0x0001u)   /* ADC12 Conversion Start Address: 19 */
#define ADC12CSTARTADD_20      (20*0x0001u)   /* ADC12 Conversion Start Address: 20 */
#define ADC12CSTARTADD_21      (21*0x0001u)   /* ADC12 Conversion Start Address: 21 */
#define ADC12CSTARTADD_22      (22*0x0001u)   /* ADC12 Conversion Start Address: 22 */
#define ADC12CSTARTADD_23      (23*0x0001u)   /* ADC12 Conversion Start Address: 23 */
#define ADC12CSTARTADD_24      (24*0x0001u)   /* ADC12 Conversion Start Address: 24 */
#define ADC12CSTARTADD_25      (25*0x0001u)   /* ADC12 Conversion Start Address: 25 */
#define ADC12CSTARTADD_26      (26*0x0001u)   /* ADC12 Conversion Start Address: 26 */
#define ADC12CSTARTADD_27      (27*0x0001u)   /* ADC12 Conversion Start Address: 27 */
#define ADC12CSTARTADD_28      (28*0x0001u)   /* ADC12 Conversion Start Address: 28 */
#define ADC12CSTARTADD_29      (29*0x0001u)   /* ADC12 Conversion Start Address: 29 */
#define ADC12CSTARTADD_30      (30*0x0001u)   /* ADC12 Conversion Start Address: 30 */
#define ADC12CSTARTADD_31      (31*0x0001u)   /* ADC12 Conversion Start Address: 31 */

/* ADC12MCTLx Control Bits */
#define ADC12INCH0             (0x0001)       /* ADC12 Input Channel Select Bit 0 */
#define ADC12INCH1             (0x0002)       /* ADC12 Input Channel Select Bit 1 */
#define ADC12INCH2             (0x0004)       /* ADC12 Input Channel Select Bit 2 */
#define ADC12INCH3             (0x0008)       /* ADC12 Input Channel Select Bit 3 */
#define ADC12INCH4             (0x0010)       /* ADC12 Input Channel Select Bit 4 */
#define ADC12EOS               (0x0080)       /* ADC12 End of Sequence */
#define ADC12VRSEL0            (0x0100)       /* ADC12 VR Select Bit 0 */
#define ADC12VRSEL1            (0x0200)       /* ADC12 VR Select Bit 1 */
#define ADC12VRSEL2            (0x0400)       /* ADC12 VR Select Bit 2 */
#define ADC12VRSEL3            (0x0800)       /* ADC12 VR Select Bit 3 */
#define ADC12DIF               (0x2000)       /* ADC12 Differential mode (only for even Registers) */
#define ADC12WINC              (0x4000)       /* ADC12 Comparator window enable */

/* ADC12MCTLx Control Bits */
#define ADC12INCH0_L           (0x0001)       /* ADC12 Input Channel Select Bit 0 */
#define ADC12INCH1_L           (0x0002)       /* ADC12 Input Channel Select Bit 1 */
#define ADC12INCH2_L           (0x0004)       /* ADC12 Input Channel Select Bit 2 */
#define ADC12INCH3_L           (0x0008)       /* ADC12 Input Channel Select Bit 3 */
#define ADC12INCH4_L           (0x0010)       /* ADC12 Input Channel Select Bit 4 */
#define ADC12EOS_L             (0x0080)       /* ADC12 End of Sequence */

/* ADC12MCTLx Control Bits */
#define ADC12VRSEL0_H          (0x0001)       /* ADC12 VR Select Bit 0 */
#define ADC12VRSEL1_H          (0x0002)       /* ADC12 VR Select Bit 1 */
#define ADC12VRSEL2_H          (0x0004)       /* ADC12 VR Select Bit 2 */
#define ADC12VRSEL3_H          (0x0008)       /* ADC12 VR Select Bit 3 */
#define ADC12DIF_H             (0x0020)       /* ADC12 Differential mode (only for even Registers) */
#define ADC12WINC_H            (0x0040)       /* ADC12 Comparator window enable */

#define ADC12INCH_0            (0x0000)       /* ADC12 Input Channel 0 */
#define ADC12INCH_1            (0x0001)       /* ADC12 Input Channel 1 */
#define ADC12INCH_2            (0x0002)       /* ADC12 Input Channel 2 */
#define ADC12INCH_3            (0x0003)       /* ADC12 Input Channel 3 */
#define ADC12INCH_4            (0x0004)       /* ADC12 Input Channel 4 */
#define ADC12INCH_5            (0x0005)       /* ADC12 Input Channel 5 */
#define ADC12INCH_6            (0x0006)       /* ADC12 Input Channel 6 */
#define ADC12INCH_7            (0x0007)       /* ADC12 Input Channel 7 */
#define ADC12INCH_8            (0x0008)       /* ADC12 Input Channel 8 */
#define ADC12INCH_9            (0x0009)       /* ADC12 Input Channel 9 */
#define ADC12INCH_10           (0x000A)       /* ADC12 Input Channel 10 */
#define ADC12INCH_11           (0x000B)       /* ADC12 Input Channel 11 */
#define ADC12INCH_12           (0x000C)       /* ADC12 Input Channel 12 */
#define ADC12INCH_13           (0x000D)       /* ADC12 Input Channel 13 */
#define ADC12INCH_14           (0x000E)       /* ADC12 Input Channel 14 */
#define ADC12INCH_15           (0x000F)       /* ADC12 Input Channel 15 */
#define ADC12INCH_16           (0x0010)       /* ADC12 Input Channel 16 */
#define ADC12INCH_17           (0x0011)       /* ADC12 Input Channel 17 */
#define ADC12INCH_18           (0x0012)       /* ADC12 Input Channel 18 */
#define ADC12INCH_19           (0x0013)       /* ADC12 Input Channel 19 */
#define ADC12INCH_20           (0x0014)       /* ADC12 Input Channel 20 */
#define ADC12INCH_21           (0x0015)       /* ADC12 Input Channel 21 */
#define ADC12INCH_22           (0x0016)       /* ADC12 Input Channel 22 */
#define ADC12INCH_23           (0x0017)       /* ADC12 Input Channel 23 */
#define ADC12INCH_24           (0x0018)       /* ADC12 Input Channel 24 */
#define ADC12INCH_25           (0x0019)       /* ADC12 Input Channel 25 */
#define ADC12INCH_26           (0x001A)       /* ADC12 Input Channel 26 */
#define ADC12INCH_27           (0x001B)       /* ADC12 Input Channel 27 */
#define ADC12INCH_28           (0x001C)       /* ADC12 Input Channel 28 */
#define ADC12INCH_29           (0x001D)       /* ADC12 Input Channel 29 */
#define ADC12INCH_30           (0x001E)       /* ADC12 Input Channel 30 */
#define ADC12INCH_31           (0x001F)       /* ADC12 Input Channel 31 */

#define ADC12VRSEL_0           (0*0x100u)     /* ADC12 Select Reference 0 */
#define ADC12VRSEL_1           (1*0x100u)     /* ADC12 Select Reference 1 */
#define ADC12VRSEL_2           (2*0x100u)     /* ADC12 Select Reference 2 */
#define ADC12VRSEL_3           (3*0x100u)     /* ADC12 Select Reference 3 */
#define ADC12VRSEL_4           (4*0x100u)     /* ADC12 Select Reference 4 */
#define ADC12VRSEL_5           (5*0x100u)     /* ADC12 Select Reference 5 */
#define ADC12VRSEL_6           (6*0x100u)     /* ADC12 Select Reference 6 */
#define ADC12VRSEL_7           (7*0x100u)     /* ADC12 Select Reference 7 */
#define ADC12VRSEL_8           (8*0x100u)     /* ADC12 Select Reference 8  */
#define ADC12VRSEL_9           (9*0x100u)     /* ADC12 Select Reference 9  */
#define ADC12VRSEL_10          (10*0x100u)    /* ADC12 Select Reference 10 */
#define ADC12VRSEL_11          (11*0x100u)    /* ADC12 Select Reference 11 */
#define ADC12VRSEL_12          (12*0x100u)    /* ADC12 Select Reference 12 */
#define ADC12VRSEL_13          (13*0x100u)    /* ADC12 Select Reference 13 */
#define ADC12VRSEL_14          (14*0x100u)    /* ADC12 Select Reference 14 */
#define ADC12VRSEL_15          (15*0x100u)    /* ADC12 Select Reference 15 */

/* ADC12HI Control Bits */

/* ADC12HI Control Bits */

/* ADC12HI Control Bits */

/* ADC12LO Control Bits */

/* ADC12LO Control Bits */

/* ADC12LO Control Bits */

/* ADC12IER0 Control Bits */
#define ADC12IE0               (0x0001)       /* ADC12 Memory 0 Interrupt Enable */
#define ADC12IE1               (0x0002)       /* ADC12 Memory 1 Interrupt Enable */
#define ADC12IE2               (0x0004)       /* ADC12 Memory 2 Interrupt Enable */
#define ADC12IE3               (0x0008)       /* ADC12 Memory 3 Interrupt Enable */
#define ADC12IE4               (0x0010)       /* ADC12 Memory 4 Interrupt Enable */
#define ADC12IE5               (0x0020)       /* ADC12 Memory 5 Interrupt Enable */
#define ADC12IE6               (0x0040)       /* ADC12 Memory 6 Interrupt Enable */
#define ADC12IE7               (0x0080)       /* ADC12 Memory 7 Interrupt Enable */
#define ADC12IE8               (0x0100)       /* ADC12 Memory 8 Interrupt Enable */
#define ADC12IE9               (0x0200)       /* ADC12 Memory 9 Interrupt Enable */
#define ADC12IE10              (0x0400)       /* ADC12 Memory 10 Interrupt Enable */
#define ADC12IE11              (0x0800)       /* ADC12 Memory 11 Interrupt Enable */
#define ADC12IE12              (0x1000)       /* ADC12 Memory 12 Interrupt Enable */
#define ADC12IE13              (0x2000)       /* ADC12 Memory 13 Interrupt Enable */
#define ADC12IE14              (0x4000)       /* ADC12 Memory 14 Interrupt Enable */
#define ADC12IE15              (0x8000)       /* ADC12 Memory 15 Interrupt Enable */

/* ADC12IER0 Control Bits */
#define ADC12IE0_L             (0x0001)       /* ADC12 Memory 0 Interrupt Enable */
#define ADC12IE1_L             (0x0002)       /* ADC12 Memory 1 Interrupt Enable */
#define ADC12IE2_L             (0x0004)       /* ADC12 Memory 2 Interrupt Enable */
#define ADC12IE3_L             (0x0008)       /* ADC12 Memory 3 Interrupt Enable */
#define ADC12IE4_L             (0x0010)       /* ADC12 Memory 4 Interrupt Enable */
#define ADC12IE5_L             (0x0020)       /* ADC12 Memory 5 Interrupt Enable */
#define ADC12IE6_L             (0x0040)       /* ADC12 Memory 6 Interrupt Enable */
#define ADC12IE7_L             (0x0080)       /* ADC12 Memory 7 Interrupt Enable */

/* ADC12IER0 Control Bits */
#define ADC12IE8_H             (0x0001)       /* ADC12 Memory 8 Interrupt Enable */
#define ADC12IE9_H             (0x0002)       /* ADC12 Memory 9 Interrupt Enable */
#define ADC12IE10_H            (0x0004)       /* ADC12 Memory 10 Interrupt Enable */
#define ADC12IE11_H            (0x0008)       /* ADC12 Memory 11 Interrupt Enable */
#define ADC12IE12_H            (0x0010)       /* ADC12 Memory 12 Interrupt Enable */
#define ADC12IE13_H            (0x0020)       /* ADC12 Memory 13 Interrupt Enable */
#define ADC12IE14_H            (0x0040)       /* ADC12 Memory 14 Interrupt Enable */
#define ADC12IE15_H            (0x0080)       /* ADC12 Memory 15 Interrupt Enable */

/* ADC12IER1 Control Bits */
#define ADC12IE16              (0x0001)       /* ADC12 Memory 16 Interrupt Enable */
#define ADC12IE17              (0x0002)       /* ADC12 Memory 17 Interrupt Enable */
#define ADC12IE18              (0x0004)       /* ADC12 Memory 18 Interrupt Enable */
#define ADC12IE19              (0x0008)       /* ADC12 Memory 19 Interrupt Enable */
#define ADC12IE20              (0x0010)       /* ADC12 Memory 20 Interrupt Enable */
#define ADC12IE21              (0x0020)       /* ADC12 Memory 21 Interrupt Enable */
#define ADC12IE22              (0x0040)       /* ADC12 Memory 22 Interrupt Enable */
#define ADC12IE23              (0x0080)       /* ADC12 Memory 23 Interrupt Enable */
#define ADC12IE24              (0x0100)       /* ADC12 Memory 24 Interrupt Enable */
#define ADC12IE25              (0x0200)       /* ADC12 Memory 25 Interrupt Enable */
#define ADC12IE26              (0x0400)       /* ADC12 Memory 26 Interrupt Enable */
#define ADC12IE27              (0x0800)       /* ADC12 Memory 27 Interrupt Enable */
#define ADC12IE28              (0x1000)       /* ADC12 Memory 28 Interrupt Enable */
#define ADC12IE29              (0x2000)       /* ADC12 Memory 29 Interrupt Enable */
#define ADC12IE30              (0x4000)       /* ADC12 Memory 30 Interrupt Enable */
#define ADC12IE31              (0x8000)       /* ADC12 Memory 31 Interrupt Enable */

/* ADC12IER1 Control Bits */
#define ADC12IE16_L            (0x0001)       /* ADC12 Memory 16 Interrupt Enable */
#define ADC12IE17_L            (0x0002)       /* ADC12 Memory 17 Interrupt Enable */
#define ADC12IE18_L            (0x0004)       /* ADC12 Memory 18 Interrupt Enable */
#define ADC12IE19_L            (0x0008)       /* ADC12 Memory 19 Interrupt Enable */
#define ADC12IE20_L            (0x0010)       /* ADC12 Memory 20 Interrupt Enable */
#define ADC12IE21_L            (0x0020)       /* ADC12 Memory 21 Interrupt Enable */
#define ADC12IE22_L            (0x0040)       /* ADC12 Memory 22 Interrupt Enable */
#define ADC12IE23_L            (0x0080)       /* ADC12 Memory 23 Interrupt Enable */

/* ADC12IER1 Control Bits */
#define ADC12IE24_H            (0x0001)       /* ADC12 Memory 24 Interrupt Enable */
#define ADC12IE25_H            (0x0002)       /* ADC12 Memory 25 Interrupt Enable */
#define ADC12IE26_H            (0x0004)       /* ADC12 Memory 26 Interrupt Enable */
#define ADC12IE27_H            (0x0008)       /* ADC12 Memory 27 Interrupt Enable */
#define ADC12IE28_H            (0x0010)       /* ADC12 Memory 28 Interrupt Enable */
#define ADC12IE29_H            (0x0020)       /* ADC12 Memory 29 Interrupt Enable */
#define ADC12IE30_H            (0x0040)       /* ADC12 Memory 30 Interrupt Enable */
#define ADC12IE31_H            (0x0080)       /* ADC12 Memory 31 Interrupt Enable */

/* ADC12IER2 Control Bits */
#define ADC12INIE              (0x0002)       /* ADC12 Interrupt enable for the inside of window of the Window comparator */
#define ADC12LOIE              (0x0004)       /* ADC12 Interrupt enable for lower threshold of the Window comparator */
#define ADC12HIIE              (0x0008)       /* ADC12 Interrupt enable for upper threshold of the Window comparator */
#define ADC12OVIE              (0x0010)       /* ADC12 ADC12MEMx Overflow interrupt enable */
#define ADC12TOVIE             (0x0020)       /* ADC12 Timer Overflow interrupt enable */
#define ADC12RDYIE             (0x0040)       /* ADC12 local buffered reference ready interrupt enable */

/* ADC12IER2 Control Bits */
#define ADC12INIE_L            (0x0002)       /* ADC12 Interrupt enable for the inside of window of the Window comparator */
#define ADC12LOIE_L            (0x0004)       /* ADC12 Interrupt enable for lower threshold of the Window comparator */
#define ADC12HIIE_L            (0x0008)       /* ADC12 Interrupt enable for upper threshold of the Window comparator */
#define ADC12OVIE_L            (0x0010)       /* ADC12 ADC12MEMx Overflow interrupt enable */
#define ADC12TOVIE_L           (0x0020)       /* ADC12 Timer Overflow interrupt enable */
#define ADC12RDYIE_L           (0x0040)       /* ADC12 local buffered reference ready interrupt enable */

/* ADC12IER2 Control Bits */

/* ADC12IFGR0 Control Bits */
#define ADC12IFG0              (0x0001)       /* ADC12 Memory 0 Interrupt Flag */
#define ADC12IFG1              (0x0002)       /* ADC12 Memory 1 Interrupt Flag */
#define ADC12IFG2              (0x0004)       /* ADC12 Memory 2 Interrupt Flag */
#define ADC12IFG3              (0x0008)       /* ADC12 Memory 3 Interrupt Flag */
#define ADC12IFG4              (0x0010)       /* ADC12 Memory 4 Interrupt Flag */
#define ADC12IFG5              (0x0020)       /* ADC12 Memory 5 Interrupt Flag */
#define ADC12IFG6              (0x0040)       /* ADC12 Memory 6 Interrupt Flag */
#define ADC12IFG7              (0x0080)       /* ADC12 Memory 7 Interrupt Flag */
#define ADC12IFG8              (0x0100)       /* ADC12 Memory 8 Interrupt Flag */
#define ADC12IFG9              (0x0200)       /* ADC12 Memory 9 Interrupt Flag */
#define ADC12IFG10             (0x0400)       /* ADC12 Memory 10 Interrupt Flag */
#define ADC12IFG11             (0x0800)       /* ADC12 Memory 11 Interrupt Flag */
#define ADC12IFG12             (0x1000)       /* ADC12 Memory 12 Interrupt Flag */
#define ADC12IFG13             (0x2000)       /* ADC12 Memory 13 Interrupt Flag */
#define ADC12IFG14             (0x4000)       /* ADC12 Memory 14 Interrupt Flag */
#define ADC12IFG15             (0x8000)       /* ADC12 Memory 15 Interrupt Flag */

/* ADC12IFGR0 Control Bits */
#define ADC12IFG0_L            (0x0001)       /* ADC12 Memory 0 Interrupt Flag */
#define ADC12IFG1_L            (0x0002)       /* ADC12 Memory 1 Interrupt Flag */
#define ADC12IFG2_L            (0x0004)       /* ADC12 Memory 2 Interrupt Flag */
#define ADC12IFG3_L            (0x0008)       /* ADC12 Memory 3 Interrupt Flag */
#define ADC12IFG4_L            (0x0010)       /* ADC12 Memory 4 Interrupt Flag */
#define ADC12IFG5_L            (0x0020)       /* ADC12 Memory 5 Interrupt Flag */
#define ADC12IFG6_L            (0x0040)       /* ADC12 Memory 6 Interrupt Flag */
#define ADC12IFG7_L            (0x0080)       /* ADC12 Memory 7 Interrupt Flag */

/* ADC12IFGR0 Control Bits */
#define ADC12IFG8_H            (0x0001)       /* ADC12 Memory 8 Interrupt Flag */
#define ADC12IFG9_H            (0x0002)       /* ADC12 Memory 9 Interrupt Flag */
#define ADC12IFG10_H           (0x0004)       /* ADC12 Memory 10 Interrupt Flag */
#define ADC12IFG11_H           (0x0008)       /* ADC12 Memory 11 Interrupt Flag */
#define ADC12IFG12_H           (0x0010)       /* ADC12 Memory 12 Interrupt Flag */
#define ADC12IFG13_H           (0x0020)       /* ADC12 Memory 13 Interrupt Flag */
#define ADC12IFG14_H           (0x0040)       /* ADC12 Memory 14 Interrupt Flag */
#define ADC12IFG15_H           (0x0080)       /* ADC12 Memory 15 Interrupt Flag */

/* ADC12IFGR1 Control Bits */
#define ADC12IFG16             (0x0001)       /* ADC12 Memory 16 Interrupt Flag */
#define ADC12IFG17             (0x0002)       /* ADC12 Memory 17 Interrupt Flag */
#define ADC12IFG18             (0x0004)       /* ADC12 Memory 18 Interrupt Flag */
#define ADC12IFG19             (0x0008)       /* ADC12 Memory 19 Interrupt Flag */
#define ADC12IFG20             (0x0010)       /* ADC12 Memory 20 Interrupt Flag */
#define ADC12IFG21             (0x0020)       /* ADC12 Memory 21 Interrupt Flag */
#define ADC12IFG22             (0x0040)       /* ADC12 Memory 22 Interrupt Flag */
#define ADC12IFG23             (0x0080)       /* ADC12 Memory 23 Interrupt Flag */
#define ADC12IFG24             (0x0100)       /* ADC12 Memory 24 Interrupt Flag */
#define ADC12IFG25             (0x0200)       /* ADC12 Memory 25 Interrupt Flag */
#define ADC12IFG26             (0x0400)       /* ADC12 Memory 26 Interrupt Flag */
#define ADC12IFG27             (0x0800)       /* ADC12 Memory 27 Interrupt Flag */
#define ADC12IFG28             (0x1000)       /* ADC12 Memory 28 Interrupt Flag */
#define ADC12IFG29             (0x2000)       /* ADC12 Memory 29 Interrupt Flag */
#define ADC12IFG30             (0x4000)       /* ADC12 Memory 30 Interrupt Flag */
#define ADC12IFG31             (0x8000)       /* ADC12 Memory 31 Interrupt Flag */

/* ADC12IFGR1 Control Bits */
#define ADC12IFG16_L           (0x0001)       /* ADC12 Memory 16 Interrupt Flag */
#define ADC12IFG17_L           (0x0002)       /* ADC12 Memory 17 Interrupt Flag */
#define ADC12IFG18_L           (0x0004)       /* ADC12 Memory 18 Interrupt Flag */
#define ADC12IFG19_L           (0x0008)       /* ADC12 Memory 19 Interrupt Flag */
#define ADC12IFG20_L           (0x0010)       /* ADC12 Memory 20 Interrupt Flag */
#define ADC12IFG21_L           (0x0020)       /* ADC12 Memory 21 Interrupt Flag */
#define ADC12IFG22_L           (0x0040)       /* ADC12 Memory 22 Interrupt Flag */
#define ADC12IFG23_L           (0x0080)       /* ADC12 Memory 23 Interrupt Flag */

/* ADC12IFGR1 Control Bits */
#define ADC12IFG24_H           (0x0001)       /* ADC12 Memory 24 Interrupt Flag */
#define ADC12IFG25_H           (0x0002)       /* ADC12 Memory 25 Interrupt Flag */
#define ADC12IFG26_H           (0x0004)       /* ADC12 Memory 26 Interrupt Flag */
#define ADC12IFG27_H           (0x0008)       /* ADC12 Memory 27 Interrupt Flag */
#define ADC12IFG28_H           (0x0010)       /* ADC12 Memory 28 Interrupt Flag */
#define ADC12IFG29_H           (0x0020)       /* ADC12 Memory 29 Interrupt Flag */
#define ADC12IFG30_H           (0x0040)       /* ADC12 Memory 30 Interrupt Flag */
#define ADC12IFG31_H           (0x0080)       /* ADC12 Memory 31 Interrupt Flag */

/* ADC12IFGR2 Control Bits */
#define ADC12INIFG             (0x0002)       /* ADC12 Interrupt Flag for the inside of window of the Window comparator */
#define ADC12LOIFG             (0x0004)       /* ADC12 Interrupt Flag for lower threshold of the Window comparator */
#define ADC12HIIFG             (0x0008)       /* ADC12 Interrupt Flag for upper threshold of the Window comparator */
#define ADC12OVIFG             (0x0010)       /* ADC12 ADC12MEMx Overflow interrupt Flag */
#define ADC12TOVIFG            (0x0020)       /* ADC12 Timer Overflow interrupt Flag */
#define ADC12RDYIFG            (0x0040)       /* ADC12 local buffered reference ready interrupt Flag */

/* ADC12IFGR2 Control Bits */
#define ADC12INIFG_L           (0x0002)       /* ADC12 Interrupt Flag for the inside of window of the Window comparator */
#define ADC12LOIFG_L           (0x0004)       /* ADC12 Interrupt Flag for lower threshold of the Window comparator */
#define ADC12HIIFG_L           (0x0008)       /* ADC12 Interrupt Flag for upper threshold of the Window comparator */
#define ADC12OVIFG_L           (0x0010)       /* ADC12 ADC12MEMx Overflow interrupt Flag */
#define ADC12TOVIFG_L          (0x0020)       /* ADC12 Timer Overflow interrupt Flag */
#define ADC12RDYIFG_L          (0x0040)       /* ADC12 local buffered reference ready interrupt Flag */

/* ADC12IFGR2 Control Bits */

/* ADC12IV Definitions */
#define ADC12IV_NONE           (0x0000)       /* No Interrupt pending */
#define ADC12IV_ADC12OVIFG     (0x0002)       /* ADC12OVIFG */
#define ADC12IV_ADC12TOVIFG    (0x0004)       /* ADC12TOVIFG */
#define ADC12IV_ADC10HIIFG     (0x0006)       /* ADC10HIIFG */
#define ADC12IV_ADC10LOIFG     (0x0008)       /* ADC10LOIFG */
#define ADC12IV_ADC10INIFG     (0x000A)       /* ADC10INIFG */
#define ADC12IV_ADC12IFG0      (0x000C)       /* ADC12IFG0 */
#define ADC12IV_ADC12IFG1      (0x000E)       /* ADC12IFG1 */
#define ADC12IV_ADC12IFG2      (0x0010)       /* ADC12IFG2 */
#define ADC12IV_ADC12IFG3      (0x0012)       /* ADC12IFG3 */
#define ADC12IV_ADC12IFG4      (0x0014)       /* ADC12IFG4 */
#define ADC12IV_ADC12IFG5      (0x0016)       /* ADC12IFG5 */
#define ADC12IV_ADC12IFG6      (0x0018)       /* ADC12IFG6 */
#define ADC12IV_ADC12IFG7      (0x001A)       /* ADC12IFG7 */
#define ADC12IV_ADC12IFG8      (0x001C)       /* ADC12IFG8 */
#define ADC12IV_ADC12IFG9      (0x001E)       /* ADC12IFG9 */
#define ADC12IV_ADC12IFG10     (0x0020)       /* ADC12IFG10 */
#define ADC12IV_ADC12IFG11     (0x0022)       /* ADC12IFG11 */
#define ADC12IV_ADC12IFG12     (0x0024)       /* ADC12IFG12 */
#define ADC12IV_ADC12IFG13     (0x0026)       /* ADC12IFG13 */
#define ADC12IV_ADC12IFG14     (0x0028)       /* ADC12IFG14 */
#define ADC12IV_ADC12IFG15     (0x002A)       /* ADC12IFG15 */
#define ADC12IV_ADC12IFG16     (0x002C)       /* ADC12IFG16 */
#define ADC12IV_ADC12IFG17     (0x002E)       /* ADC12IFG17 */
#define ADC12IV_ADC12IFG18     (0x0030)       /* ADC12IFG18 */
#define ADC12IV_ADC12IFG19     (0x0032)       /* ADC12IFG19 */
#define ADC12IV_ADC12IFG20     (0x0034)       /* ADC12IFG20 */
#define ADC12IV_ADC12IFG21     (0x0036)       /* ADC12IFG21 */
#define ADC12IV_ADC12IFG22     (0x0038)       /* ADC12IFG22 */
#define ADC12IV_ADC12IFG23     (0x003A)       /* ADC12IFG23 */
#define ADC12IV_ADC12IFG24     (0x003C)       /* ADC12IFG24 */
#define ADC12IV_ADC12IFG25     (0x003E)       /* ADC12IFG25 */
#define ADC12IV_ADC12IFG26     (0x0040)       /* ADC12IFG26 */
#define ADC12IV_ADC12IFG27     (0x0042)       /* ADC12IFG27 */
#define ADC12IV_ADC12IFG28     (0x0044)       /* ADC12IFG28 */
#define ADC12IV_ADC12IFG29     (0x0046)       /* ADC12IFG29 */
#define ADC12IV_ADC12IFG30     (0x0048)       /* ADC12IFG30 */
#define ADC12IV_ADC12IFG31     (0x004A)       /* ADC12IFG31 */
#define ADC12IV_ADC12RDYIFG    (0x004C)       /* ADC12RDYIFG */


#endif
/************************************************************
* AES256 Accelerator
************************************************************/
#ifdef  __MSP430_HAS_AES256__          /* Definition to show that Module is available */

#define OFS_AESACTL0           (0x0000)       /* AES accelerator control register 0 */
#define OFS_AESACTL0_L         OFS_AESACTL0
#define OFS_AESACTL0_H         OFS_AESACTL0+1
#define OFS_AESACTL1           (0x0002)       /* AES accelerator control register 1 */
#define OFS_AESACTL1_L         OFS_AESACTL1
#define OFS_AESACTL1_H         OFS_AESACTL1+1
#define OFS_AESASTAT           (0x0004)       /* AES accelerator status register */
#define OFS_AESASTAT_L         OFS_AESASTAT
#define OFS_AESASTAT_H         OFS_AESASTAT+1
#define OFS_AESAKEY            (0x0006)       /* AES accelerator key register */
#define OFS_AESAKEY_L          OFS_AESAKEY
#define OFS_AESAKEY_H          OFS_AESAKEY+1
#define OFS_AESADIN            (0x0008)       /* AES accelerator data in register */
#define OFS_AESADIN_L          OFS_AESADIN
#define OFS_AESADIN_H          OFS_AESADIN+1
#define OFS_AESADOUT           (0x000A)       /* AES accelerator data out register  */
#define OFS_AESADOUT_L         OFS_AESADOUT
#define OFS_AESADOUT_H         OFS_AESADOUT+1
#define OFS_AESAXDIN           (0x000C)       /* AES accelerator XORed data in register */
#define OFS_AESAXDIN_L         OFS_AESAXDIN
#define OFS_AESAXDIN_H         OFS_AESAXDIN+1
#define OFS_AESAXIN            (0x000E)       /* AES accelerator XORed data in register (no trigger)  */
#define OFS_AESAXIN_L          OFS_AESAXIN
#define OFS_AESAXIN_H          OFS_AESAXIN+1

/* AESACTL0 Control Bits */
#define AESOP0                 (0x0001)       /* AES Operation Bit: 0 */
#define AESOP1                 (0x0002)       /* AES Operation Bit: 1 */
#define AESKL0                 (0x0004)       /* AES Key length Bit: 0 */
#define AESKL1                 (0x0008)       /* AES Key length Bit: 1 */
#define AESTRIG                (0x0010)       /* AES Trigger Select */
#define AESCM0                 (0x0020)       /* AES Cipher mode select Bit: 0 */
#define AESCM1                 (0x0040)       /* AES Cipher mode select Bit: 1 */
#define AESSWRST               (0x0080)       /* AES Software Reset */
#define AESRDYIFG              (0x0100)       /* AES ready interrupt flag */
#define AESERRFG               (0x0800)       /* AES Error Flag */
#define AESRDYIE               (0x1000)       /* AES ready interrupt enable*/
#define AESCMEN                (0x8000)       /* AES DMA cipher mode enable*/

/* AESACTL0 Control Bits */
#define AESOP0_L               (0x0001)       /* AES Operation Bit: 0 */
#define AESOP1_L               (0x0002)       /* AES Operation Bit: 1 */
#define AESKL0_L               (0x0004)       /* AES Key length Bit: 0 */
#define AESKL1_L               (0x0008)       /* AES Key length Bit: 1 */
#define AESTRIG_L              (0x0010)       /* AES Trigger Select */
#define AESCM0_L               (0x0020)       /* AES Cipher mode select Bit: 0 */
#define AESCM1_L               (0x0040)       /* AES Cipher mode select Bit: 1 */
#define AESSWRST_L             (0x0080)       /* AES Software Reset */

/* AESACTL0 Control Bits */
#define AESRDYIFG_H            (0x0001)       /* AES ready interrupt flag */
#define AESERRFG_H             (0x0008)       /* AES Error Flag */
#define AESRDYIE_H             (0x0010)       /* AES ready interrupt enable*/
#define AESCMEN_H              (0x0080)       /* AES DMA cipher mode enable*/

#define AESOP_0                (0x0000)       /* AES Operation: Encrypt */
#define AESOP_1                (0x0001)       /* AES Operation: Decrypt (same Key) */
#define AESOP_2                (0x0002)       /* AES Operation: Decrypt (frist round Key) */
#define AESOP_3                (0x0003)       /* AES Operation: Generate first round Key */

#define AESKL_0                (0x0000)       /* AES Key length: AES128 */
#define AESKL_1                (0x0004)       /* AES Key length: AES192 */
#define AESKL_2                (0x0008)       /* AES Key length: AES256 */
#define AESKL__128             (0x0000)       /* AES Key length: AES128 */
#define AESKL__192             (0x0004)       /* AES Key length: AES192 */
#define AESKL__256             (0x0008)       /* AES Key length: AES256 */

#define AESCM_0                (0x0000)       /* AES Cipher mode select: ECB */
#define AESCM_1                (0x0020)       /* AES Cipher mode select: CBC */
#define AESCM_2                (0x0040)       /* AES Cipher mode select: OFB */
#define AESCM_3                (0x0060)       /* AES Cipher mode select: CFB */
#define AESCM__ECB             (0x0000)       /* AES Cipher mode select: ECB */
#define AESCM__CBC             (0x0020)       /* AES Cipher mode select: CBC */
#define AESCM__OFB             (0x0040)       /* AES Cipher mode select: OFB */
#define AESCM__CFB             (0x0060)       /* AES Cipher mode select: CFB */

/* AESACTL1 Control Bits */
#define AESBLKCNT0             (0x0001)       /* AES Cipher Block Counter Bit: 0 */
#define AESBLKCNT1             (0x0002)       /* AES Cipher Block Counter Bit: 1 */
#define AESBLKCNT2             (0x0004)       /* AES Cipher Block Counter Bit: 2 */
#define AESBLKCNT3             (0x0008)       /* AES Cipher Block Counter Bit: 3 */
#define AESBLKCNT4             (0x0010)       /* AES Cipher Block Counter Bit: 4 */
#define AESBLKCNT5             (0x0020)       /* AES Cipher Block Counter Bit: 5 */
#define AESBLKCNT6             (0x0040)       /* AES Cipher Block Counter Bit: 6 */
#define AESBLKCNT7             (0x0080)       /* AES Cipher Block Counter Bit: 7 */

/* AESACTL1 Control Bits */
#define AESBLKCNT0_L           (0x0001)       /* AES Cipher Block Counter Bit: 0 */
#define AESBLKCNT1_L           (0x0002)       /* AES Cipher Block Counter Bit: 1 */
#define AESBLKCNT2_L           (0x0004)       /* AES Cipher Block Counter Bit: 2 */
#define AESBLKCNT3_L           (0x0008)       /* AES Cipher Block Counter Bit: 3 */
#define AESBLKCNT4_L           (0x0010)       /* AES Cipher Block Counter Bit: 4 */
#define AESBLKCNT5_L           (0x0020)       /* AES Cipher Block Counter Bit: 5 */
#define AESBLKCNT6_L           (0x0040)       /* AES Cipher Block Counter Bit: 6 */
#define AESBLKCNT7_L           (0x0080)       /* AES Cipher Block Counter Bit: 7 */

/* AESACTL1 Control Bits */

/* AESASTAT Control Bits */
#define AESBUSY                (0x0001)       /* AES Busy */
#define AESKEYWR               (0x0002)       /* AES All 16 bytes written to AESAKEY */
#define AESDINWR               (0x0004)       /* AES All 16 bytes written to AESADIN */
#define AESDOUTRD              (0x0008)       /* AES All 16 bytes read from AESADOUT */
#define AESKEYCNT0             (0x0010)       /* AES Bytes written via AESAKEY Bit: 0 */
#define AESKEYCNT1             (0x0020)       /* AES Bytes written via AESAKEY Bit: 1 */
#define AESKEYCNT2             (0x0040)       /* AES Bytes written via AESAKEY Bit: 2 */
#define AESKEYCNT3             (0x0080)       /* AES Bytes written via AESAKEY Bit: 3 */
#define AESDINCNT0             (0x0100)       /* AES Bytes written via AESADIN Bit: 0 */
#define AESDINCNT1             (0x0200)       /* AES Bytes written via AESADIN Bit: 1 */
#define AESDINCNT2             (0x0400)       /* AES Bytes written via AESADIN Bit: 2 */
#define AESDINCNT3             (0x0800)       /* AES Bytes written via AESADIN Bit: 3 */
#define AESDOUTCNT0            (0x1000)       /* AES Bytes read via AESADOUT Bit: 0 */
#define AESDOUTCNT1            (0x2000)       /* AES Bytes read via AESADOUT Bit: 1 */
#define AESDOUTCNT2            (0x4000)       /* AES Bytes read via AESADOUT Bit: 2 */
#define AESDOUTCNT3            (0x8000)       /* AES Bytes read via AESADOUT Bit: 3 */

/* AESASTAT Control Bits */
#define AESBUSY_L              (0x0001)       /* AES Busy */
#define AESKEYWR_L             (0x0002)       /* AES All 16 bytes written to AESAKEY */
#define AESDINWR_L             (0x0004)       /* AES All 16 bytes written to AESADIN */
#define AESDOUTRD_L            (0x0008)       /* AES All 16 bytes read from AESADOUT */
#define AESKEYCNT0_L           (0x0010)       /* AES Bytes written via AESAKEY Bit: 0 */
#define AESKEYCNT1_L           (0x0020)       /* AES Bytes written via AESAKEY Bit: 1 */
#define AESKEYCNT2_L           (0x0040)       /* AES Bytes written via AESAKEY Bit: 2 */
#define AESKEYCNT3_L           (0x0080)       /* AES Bytes written via AESAKEY Bit: 3 */

/* AESASTAT Control Bits */
#define AESDINCNT0_H           (0x0001)       /* AES Bytes written via AESADIN Bit: 0 */
#define AESDINCNT1_H           (0x0002)       /* AES Bytes written via AESADIN Bit: 1 */
#define AESDINCNT2_H           (0x0004)       /* AES Bytes written via AESADIN Bit: 2 */
#define AESDINCNT3_H           (0x0008)       /* AES Bytes written via AESADIN Bit: 3 */
#define AESDOUTCNT0_H          (0x0010)       /* AES Bytes read via AESADOUT Bit: 0 */
#define AESDOUTCNT1_H          (0x0020)       /* AES Bytes read via AESADOUT Bit: 1 */
#define AESDOUTCNT2_H          (0x0040)       /* AES Bytes read via AESADOUT Bit: 2 */
#define AESDOUTCNT3_H          (0x0080)       /* AES Bytes read via AESADOUT Bit: 3 */

#endif
/*************************************************************
* Backup RAM Module
*************************************************************/
#ifdef  __MSP430_HAS_BACKUP_RAM__      /* Definition to show that Module is available */

#define OFS_BAKMEM0            (0x0000)       /* Battery Backup Memory 0 */
#define OFS_BAKMEM0_L          OFS_BAKMEM0
#define OFS_BAKMEM0_H          OFS_BAKMEM0+1
#define OFS_BAKMEM1            (0x0002)       /* Battery Backup Memory 0 */
#define OFS_BAKMEM1_L          OFS_BAKMEM1
#define OFS_BAKMEM1_H          OFS_BAKMEM1+1
#define OFS_BAKMEM2            (0x0004)       /* Battery Backup Memory 0 */
#define OFS_BAKMEM2_L          OFS_BAKMEM2
#define OFS_BAKMEM2_H          OFS_BAKMEM2+1
#define OFS_BAKMEM3            (0x0006)       /* Battery Backup Memory 0 */
#define OFS_BAKMEM3_L          OFS_BAKMEM3
#define OFS_BAKMEM3_H          OFS_BAKMEM3+1

#endif
/************************************************************
* Comparator E
************************************************************/
#ifdef  __MSP430_HAS_COMP_E__          /* Definition to show that Module is available */

#define OFS_CECTL0             (0x0000)       /* Comparator E Control Register 0 */
#define OFS_CECTL0_L           OFS_CECTL0
#define OFS_CECTL0_H           OFS_CECTL0+1
#define OFS_CECTL1             (0x0002)       /* Comparator E Control Register 1 */
#define OFS_CECTL1_L           OFS_CECTL1
#define OFS_CECTL1_H           OFS_CECTL1+1
#define OFS_CECTL2             (0x0004)       /* Comparator E Control Register 2 */
#define OFS_CECTL2_L           OFS_CECTL2
#define OFS_CECTL2_H           OFS_CECTL2+1
#define OFS_CECTL3             (0x0006)       /* Comparator E Control Register 3 */
#define OFS_CECTL3_L           OFS_CECTL3
#define OFS_CECTL3_H           OFS_CECTL3+1
#define OFS_CEINT              (0x000C)       /* Comparator E Interrupt Register */
#define OFS_CEINT_L            OFS_CEINT
#define OFS_CEINT_H            OFS_CEINT+1
#define OFS_CEIV               (0x000E)       /* Comparator E Interrupt Vector Word */
#define OFS_CEIV_L             OFS_CEIV
#define OFS_CEIV_H             OFS_CEIV+1

/* CECTL0 Control Bits */
#define CEIPSEL0               (0x0001)       /* Comp. E Pos. Channel Input Select 0 */
#define CEIPSEL1               (0x0002)       /* Comp. E Pos. Channel Input Select 1 */
#define CEIPSEL2               (0x0004)       /* Comp. E Pos. Channel Input Select 2 */
#define CEIPSEL3               (0x0008)       /* Comp. E Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010)  /* Comp. E */
//#define RESERVED            (0x0020)  /* Comp. E */
//#define RESERVED            (0x0040)  /* Comp. E */
#define CEIPEN                 (0x0080)       /* Comp. E Pos. Channel Input Enable */
#define CEIMSEL0               (0x0100)       /* Comp. E Neg. Channel Input Select 0 */
#define CEIMSEL1               (0x0200)       /* Comp. E Neg. Channel Input Select 1 */
#define CEIMSEL2               (0x0400)       /* Comp. E Neg. Channel Input Select 2 */
#define CEIMSEL3               (0x0800)       /* Comp. E Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000)  /* Comp. E */
//#define RESERVED            (0x2000)  /* Comp. E */
//#define RESERVED            (0x4000)  /* Comp. E */
#define CEIMEN                 (0x8000)       /* Comp. E Neg. Channel Input Enable */

/* CECTL0 Control Bits */
#define CEIPSEL0_L             (0x0001)       /* Comp. E Pos. Channel Input Select 0 */
#define CEIPSEL1_L             (0x0002)       /* Comp. E Pos. Channel Input Select 1 */
#define CEIPSEL2_L             (0x0004)       /* Comp. E Pos. Channel Input Select 2 */
#define CEIPSEL3_L             (0x0008)       /* Comp. E Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010)  /* Comp. E */
//#define RESERVED            (0x0020)  /* Comp. E */
//#define RESERVED            (0x0040)  /* Comp. E */
#define CEIPEN_L               (0x0080)       /* Comp. E Pos. Channel Input Enable */
//#define RESERVED            (0x1000)  /* Comp. E */
//#define RESERVED            (0x2000)  /* Comp. E */
//#define RESERVED            (0x4000)  /* Comp. E */

/* CECTL0 Control Bits */
//#define RESERVED            (0x0010)  /* Comp. E */
//#define RESERVED            (0x0020)  /* Comp. E */
//#define RESERVED            (0x0040)  /* Comp. E */
#define CEIMSEL0_H             (0x0001)       /* Comp. E Neg. Channel Input Select 0 */
#define CEIMSEL1_H             (0x0002)       /* Comp. E Neg. Channel Input Select 1 */
#define CEIMSEL2_H             (0x0004)       /* Comp. E Neg. Channel Input Select 2 */
#define CEIMSEL3_H             (0x0008)       /* Comp. E Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000)  /* Comp. E */
//#define RESERVED            (0x2000)  /* Comp. E */
//#define RESERVED            (0x4000)  /* Comp. E */
#define CEIMEN_H               (0x0080)       /* Comp. E Neg. Channel Input Enable */

#define CEIPSEL_0              (0x0000)       /* Comp. E V+ terminal Input Select: Channel 0 */
#define CEIPSEL_1              (0x0001)       /* Comp. E V+ terminal Input Select: Channel 1 */
#define CEIPSEL_2              (0x0002)       /* Comp. E V+ terminal Input Select: Channel 2 */
#define CEIPSEL_3              (0x0003)       /* Comp. E V+ terminal Input Select: Channel 3 */
#define CEIPSEL_4              (0x0004)       /* Comp. E V+ terminal Input Select: Channel 4 */
#define CEIPSEL_5              (0x0005)       /* Comp. E V+ terminal Input Select: Channel 5 */
#define CEIPSEL_6              (0x0006)       /* Comp. E V+ terminal Input Select: Channel 6 */
#define CEIPSEL_7              (0x0007)       /* Comp. E V+ terminal Input Select: Channel 7 */
#define CEIPSEL_8              (0x0008)       /* Comp. E V+ terminal Input Select: Channel 8 */
#define CEIPSEL_9              (0x0009)       /* Comp. E V+ terminal Input Select: Channel 9 */
#define CEIPSEL_10             (0x000A)       /* Comp. E V+ terminal Input Select: Channel 10 */
#define CEIPSEL_11             (0x000B)       /* Comp. E V+ terminal Input Select: Channel 11 */
#define CEIPSEL_12             (0x000C)       /* Comp. E V+ terminal Input Select: Channel 12 */
#define CEIPSEL_13             (0x000D)       /* Comp. E V+ terminal Input Select: Channel 13 */
#define CEIPSEL_14             (0x000E)       /* Comp. E V+ terminal Input Select: Channel 14 */
#define CEIPSEL_15             (0x000F)       /* Comp. E V+ terminal Input Select: Channel 15 */

#define CEIMSEL_0              (0x0000)       /* Comp. E V- Terminal Input Select: Channel 0 */
#define CEIMSEL_1              (0x0100)       /* Comp. E V- Terminal Input Select: Channel 1 */
#define CEIMSEL_2              (0x0200)       /* Comp. E V- Terminal Input Select: Channel 2 */
#define CEIMSEL_3              (0x0300)       /* Comp. E V- Terminal Input Select: Channel 3 */
#define CEIMSEL_4              (0x0400)       /* Comp. E V- Terminal Input Select: Channel 4 */
#define CEIMSEL_5              (0x0500)       /* Comp. E V- Terminal Input Select: Channel 5 */
#define CEIMSEL_6              (0x0600)       /* Comp. E V- Terminal Input Select: Channel 6 */
#define CEIMSEL_7              (0x0700)       /* Comp. E V- Terminal Input Select: Channel 7 */
#define CEIMSEL_8              (0x0800)       /* Comp. E V- terminal Input Select: Channel 8 */
#define CEIMSEL_9              (0x0900)       /* Comp. E V- terminal Input Select: Channel 9 */
#define CEIMSEL_10             (0x0A00)       /* Comp. E V- terminal Input Select: Channel 10 */
#define CEIMSEL_11             (0x0B00)       /* Comp. E V- terminal Input Select: Channel 11 */
#define CEIMSEL_12             (0x0C00)       /* Comp. E V- terminal Input Select: Channel 12 */
#define CEIMSEL_13             (0x0D00)       /* Comp. E V- terminal Input Select: Channel 13 */
#define CEIMSEL_14             (0x0E00)       /* Comp. E V- terminal Input Select: Channel 14 */
#define CEIMSEL_15             (0x0F00)       /* Comp. E V- terminal Input Select: Channel 15 */

/* CECTL1 Control Bits */
#define CEOUT                  (0x0001)       /* Comp. E Output */
#define CEOUTPOL               (0x0002)       /* Comp. E Output Polarity */
#define CEF                    (0x0004)       /* Comp. E Enable Output Filter */
#define CEIES                  (0x0008)       /* Comp. E Interrupt Edge Select */
#define CESHORT                (0x0010)       /* Comp. E Input Short */
#define CEEX                   (0x0020)       /* Comp. E Exchange Inputs */
#define CEFDLY0                (0x0040)       /* Comp. E Filter delay Bit 0 */
#define CEFDLY1                (0x0080)       /* Comp. E Filter delay Bit 1 */
#define CEPWRMD0               (0x0100)       /* Comp. E Power mode Bit 0 */
#define CEPWRMD1               (0x0200)       /* Comp. E Power mode Bit 1 */
#define CEON                   (0x0400)       /* Comp. E enable */
#define CEMRVL                 (0x0800)       /* Comp. E CEMRV Level */
#define CEMRVS                 (0x1000)       /* Comp. E Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000)  /* Comp. E */
//#define RESERVED            (0x4000)  /* Comp. E */
//#define RESERVED            (0x8000)  /* Comp. E */

/* CECTL1 Control Bits */
#define CEOUT_L                (0x0001)       /* Comp. E Output */
#define CEOUTPOL_L             (0x0002)       /* Comp. E Output Polarity */
#define CEF_L                  (0x0004)       /* Comp. E Enable Output Filter */
#define CEIES_L                (0x0008)       /* Comp. E Interrupt Edge Select */
#define CESHORT_L              (0x0010)       /* Comp. E Input Short */
#define CEEX_L                 (0x0020)       /* Comp. E Exchange Inputs */
#define CEFDLY0_L              (0x0040)       /* Comp. E Filter delay Bit 0 */
#define CEFDLY1_L              (0x0080)       /* Comp. E Filter delay Bit 1 */
//#define RESERVED            (0x2000)  /* Comp. E */
//#define RESERVED            (0x4000)  /* Comp. E */
//#define RESERVED            (0x8000)  /* Comp. E */

/* CECTL1 Control Bits */
#define CEPWRMD0_H             (0x0001)       /* Comp. E Power mode Bit 0 */
#define CEPWRMD1_H             (0x0002)       /* Comp. E Power mode Bit 1 */
#define CEON_H                 (0x0004)       /* Comp. E enable */
#define CEMRVL_H               (0x0008)       /* Comp. E CEMRV Level */
#define CEMRVS_H               (0x0010)       /* Comp. E Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000)  /* Comp. E */
//#define RESERVED            (0x4000)  /* Comp. E */
//#define RESERVED            (0x8000)  /* Comp. E */

#define CEPWRMD_0              (0x0000)       /* Comp. E Power mode 0 */
#define CEPWRMD_1              (0x0100)       /* Comp. E Power mode 1 */
#define CEPWRMD_2              (0x0200)       /* Comp. E Power mode 2 */
#define CEPWRMD_3              (0x0300)       /* Comp. E Power mode 3*/

#define CEFDLY_0               (0x0000)       /* Comp. E Filter delay 0 : 450ns */
#define CEFDLY_1               (0x0040)       /* Comp. E Filter delay 1 : 900ns */
#define CEFDLY_2               (0x0080)       /* Comp. E Filter delay 2 : 1800ns */
#define CEFDLY_3               (0x00C0)       /* Comp. E Filter delay 3 : 3600ns */

/* CECTL2 Control Bits */
#define CEREF00                (0x0001)       /* Comp. E Reference 0 Resistor Select Bit : 0 */
#define CEREF01                (0x0002)       /* Comp. E Reference 0 Resistor Select Bit : 1 */
#define CEREF02                (0x0004)       /* Comp. E Reference 0 Resistor Select Bit : 2 */
#define CEREF03                (0x0008)       /* Comp. E Reference 0 Resistor Select Bit : 3 */
#define CEREF04                (0x0010)       /* Comp. E Reference 0 Resistor Select Bit : 4 */
#define CERSEL                 (0x0020)       /* Comp. E Reference select */
#define CERS0                  (0x0040)       /* Comp. E Reference Source Bit : 0 */
#define CERS1                  (0x0080)       /* Comp. E Reference Source Bit : 1 */
#define CEREF10                (0x0100)       /* Comp. E Reference 1 Resistor Select Bit : 0 */
#define CEREF11                (0x0200)       /* Comp. E Reference 1 Resistor Select Bit : 1 */
#define CEREF12                (0x0400)       /* Comp. E Reference 1 Resistor Select Bit : 2 */
#define CEREF13                (0x0800)       /* Comp. E Reference 1 Resistor Select Bit : 3 */
#define CEREF14                (0x1000)       /* Comp. E Reference 1 Resistor Select Bit : 4 */
#define CEREFL0                (0x2000)       /* Comp. E Reference voltage level Bit : 0 */
#define CEREFL1                (0x4000)       /* Comp. E Reference voltage level Bit : 1 */
#define CEREFACC               (0x8000)       /* Comp. E Reference Accuracy */

/* CECTL2 Control Bits */
#define CEREF00_L              (0x0001)       /* Comp. E Reference 0 Resistor Select Bit : 0 */
#define CEREF01_L              (0x0002)       /* Comp. E Reference 0 Resistor Select Bit : 1 */
#define CEREF02_L              (0x0004)       /* Comp. E Reference 0 Resistor Select Bit : 2 */
#define CEREF03_L              (0x0008)       /* Comp. E Reference 0 Resistor Select Bit : 3 */
#define CEREF04_L              (0x0010)       /* Comp. E Reference 0 Resistor Select Bit : 4 */
#define CERSEL_L               (0x0020)       /* Comp. E Reference select */
#define CERS0_L                (0x0040)       /* Comp. E Reference Source Bit : 0 */
#define CERS1_L                (0x0080)       /* Comp. E Reference Source Bit : 1 */

/* CECTL2 Control Bits */
#define CEREF10_H              (0x0001)       /* Comp. E Reference 1 Resistor Select Bit : 0 */
#define CEREF11_H              (0x0002)       /* Comp. E Reference 1 Resistor Select Bit : 1 */
#define CEREF12_H              (0x0004)       /* Comp. E Reference 1 Resistor Select Bit : 2 */
#define CEREF13_H              (0x0008)       /* Comp. E Reference 1 Resistor Select Bit : 3 */
#define CEREF14_H              (0x0010)       /* Comp. E Reference 1 Resistor Select Bit : 4 */
#define CEREFL0_H              (0x0020)       /* Comp. E Reference voltage level Bit : 0 */
#define CEREFL1_H              (0x0040)       /* Comp. E Reference voltage level Bit : 1 */
#define CEREFACC_H             (0x0080)       /* Comp. E Reference Accuracy */

#define CEREF0_0               (0x0000)       /* Comp. E Int. Ref.0 Select 0 : 1/32 */
#define CEREF0_1               (0x0001)       /* Comp. E Int. Ref.0 Select 1 : 2/32 */
#define CEREF0_2               (0x0002)       /* Comp. E Int. Ref.0 Select 2 : 3/32 */
#define CEREF0_3               (0x0003)       /* Comp. E Int. Ref.0 Select 3 : 4/32 */
#define CEREF0_4               (0x0004)       /* Comp. E Int. Ref.0 Select 4 : 5/32 */
#define CEREF0_5               (0x0005)       /* Comp. E Int. Ref.0 Select 5 : 6/32 */
#define CEREF0_6               (0x0006)       /* Comp. E Int. Ref.0 Select 6 : 7/32 */
#define CEREF0_7               (0x0007)       /* Comp. E Int. Ref.0 Select 7 : 8/32 */
#define CEREF0_8               (0x0008)       /* Comp. E Int. Ref.0 Select 0 : 9/32 */
#define CEREF0_9               (0x0009)       /* Comp. E Int. Ref.0 Select 1 : 10/32 */
#define CEREF0_10              (0x000A)       /* Comp. E Int. Ref.0 Select 2 : 11/32 */
#define CEREF0_11              (0x000B)       /* Comp. E Int. Ref.0 Select 3 : 12/32 */
#define CEREF0_12              (0x000C)       /* Comp. E Int. Ref.0 Select 4 : 13/32 */
#define CEREF0_13              (0x000D)       /* Comp. E Int. Ref.0 Select 5 : 14/32 */
#define CEREF0_14              (0x000E)       /* Comp. E Int. Ref.0 Select 6 : 15/32 */
#define CEREF0_15              (0x000F)       /* Comp. E Int. Ref.0 Select 7 : 16/32 */
#define CEREF0_16              (0x0010)       /* Comp. E Int. Ref.0 Select 0 : 17/32 */
#define CEREF0_17              (0x0011)       /* Comp. E Int. Ref.0 Select 1 : 18/32 */
#define CEREF0_18              (0x0012)       /* Comp. E Int. Ref.0 Select 2 : 19/32 */
#define CEREF0_19              (0x0013)       /* Comp. E Int. Ref.0 Select 3 : 20/32 */
#define CEREF0_20              (0x0014)       /* Comp. E Int. Ref.0 Select 4 : 21/32 */
#define CEREF0_21              (0x0015)       /* Comp. E Int. Ref.0 Select 5 : 22/32 */
#define CEREF0_22              (0x0016)       /* Comp. E Int. Ref.0 Select 6 : 23/32 */
#define CEREF0_23              (0x0017)       /* Comp. E Int. Ref.0 Select 7 : 24/32 */
#define CEREF0_24              (0x0018)       /* Comp. E Int. Ref.0 Select 0 : 25/32 */
#define CEREF0_25              (0x0019)       /* Comp. E Int. Ref.0 Select 1 : 26/32 */
#define CEREF0_26              (0x001A)       /* Comp. E Int. Ref.0 Select 2 : 27/32 */
#define CEREF0_27              (0x001B)       /* Comp. E Int. Ref.0 Select 3 : 28/32 */
#define CEREF0_28              (0x001C)       /* Comp. E Int. Ref.0 Select 4 : 29/32 */
#define CEREF0_29              (0x001D)       /* Comp. E Int. Ref.0 Select 5 : 30/32 */
#define CEREF0_30              (0x001E)       /* Comp. E Int. Ref.0 Select 6 : 31/32 */
#define CEREF0_31              (0x001F)       /* Comp. E Int. Ref.0 Select 7 : 32/32 */

#define CERS_0                 (0x0000)       /* Comp. E Reference Source 0 : Off */
#define CERS_1                 (0x0040)       /* Comp. E Reference Source 1 : Vcc */
#define CERS_2                 (0x0080)       /* Comp. E Reference Source 2 : Shared Ref. */
#define CERS_3                 (0x00C0)       /* Comp. E Reference Source 3 : Shared Ref. / Off */

#define CEREF1_0               (0x0000)       /* Comp. E Int. Ref.1 Select 0 : 1/32 */
#define CEREF1_1               (0x0100)       /* Comp. E Int. Ref.1 Select 1 : 2/32 */
#define CEREF1_2               (0x0200)       /* Comp. E Int. Ref.1 Select 2 : 3/32 */
#define CEREF1_3               (0x0300)       /* Comp. E Int. Ref.1 Select 3 : 4/32 */
#define CEREF1_4               (0x0400)       /* Comp. E Int. Ref.1 Select 4 : 5/32 */
#define CEREF1_5               (0x0500)       /* Comp. E Int. Ref.1 Select 5 : 6/32 */
#define CEREF1_6               (0x0600)       /* Comp. E Int. Ref.1 Select 6 : 7/32 */
#define CEREF1_7               (0x0700)       /* Comp. E Int. Ref.1 Select 7 : 8/32 */
#define CEREF1_8               (0x0800)       /* Comp. E Int. Ref.1 Select 0 : 9/32 */
#define CEREF1_9               (0x0900)       /* Comp. E Int. Ref.1 Select 1 : 10/32 */
#define CEREF1_10              (0x0A00)       /* Comp. E Int. Ref.1 Select 2 : 11/32 */
#define CEREF1_11              (0x0B00)       /* Comp. E Int. Ref.1 Select 3 : 12/32 */
#define CEREF1_12              (0x0C00)       /* Comp. E Int. Ref.1 Select 4 : 13/32 */
#define CEREF1_13              (0x0D00)       /* Comp. E Int. Ref.1 Select 5 : 14/32 */
#define CEREF1_14              (0x0E00)       /* Comp. E Int. Ref.1 Select 6 : 15/32 */
#define CEREF1_15              (0x0F00)       /* Comp. E Int. Ref.1 Select 7 : 16/32 */
#define CEREF1_16              (0x1000)       /* Comp. E Int. Ref.1 Select 0 : 17/32 */
#define CEREF1_17              (0x1100)       /* Comp. E Int. Ref.1 Select 1 : 18/32 */
#define CEREF1_18              (0x1200)       /* Comp. E Int. Ref.1 Select 2 : 19/32 */
#define CEREF1_19              (0x1300)       /* Comp. E Int. Ref.1 Select 3 : 20/32 */
#define CEREF1_20              (0x1400)       /* Comp. E Int. Ref.1 Select 4 : 21/32 */
#define CEREF1_21              (0x1500)       /* Comp. E Int. Ref.1 Select 5 : 22/32 */
#define CEREF1_22              (0x1600)       /* Comp. E Int. Ref.1 Select 6 : 23/32 */
#define CEREF1_23              (0x1700)       /* Comp. E Int. Ref.1 Select 7 : 24/32 */
#define CEREF1_24              (0x1800)       /* Comp. E Int. Ref.1 Select 0 : 25/32 */
#define CEREF1_25              (0x1900)       /* Comp. E Int. Ref.1 Select 1 : 26/32 */
#define CEREF1_26              (0x1A00)       /* Comp. E Int. Ref.1 Select 2 : 27/32 */
#define CEREF1_27              (0x1B00)       /* Comp. E Int. Ref.1 Select 3 : 28/32 */
#define CEREF1_28              (0x1C00)       /* Comp. E Int. Ref.1 Select 4 : 29/32 */
#define CEREF1_29              (0x1D00)       /* Comp. E Int. Ref.1 Select 5 : 30/32 */
#define CEREF1_30              (0x1E00)       /* Comp. E Int. Ref.1 Select 6 : 31/32 */
#define CEREF1_31              (0x1F00)       /* Comp. E Int. Ref.1 Select 7 : 32/32 */

#define CEREFL_0               (0x0000)       /* Comp. E Reference voltage level 0 : None */
#define CEREFL_1               (0x2000)       /* Comp. E Reference voltage level 1 : 1.5V */
#define CEREFL_2               (0x4000)       /* Comp. E Reference voltage level 2 : 2.0V  */
#define CEREFL_3               (0x6000)       /* Comp. E Reference voltage level 3 : 2.5V  */

#define CEPD0                  (0x0001)       /* Comp. E Disable Input Buffer of Port Register .0 */
#define CEPD1                  (0x0002)       /* Comp. E Disable Input Buffer of Port Register .1 */
#define CEPD2                  (0x0004)       /* Comp. E Disable Input Buffer of Port Register .2 */
#define CEPD3                  (0x0008)       /* Comp. E Disable Input Buffer of Port Register .3 */
#define CEPD4                  (0x0010)       /* Comp. E Disable Input Buffer of Port Register .4 */
#define CEPD5                  (0x0020)       /* Comp. E Disable Input Buffer of Port Register .5 */
#define CEPD6                  (0x0040)       /* Comp. E Disable Input Buffer of Port Register .6 */
#define CEPD7                  (0x0080)       /* Comp. E Disable Input Buffer of Port Register .7 */
#define CEPD8                  (0x0100)       /* Comp. E Disable Input Buffer of Port Register .8 */
#define CEPD9                  (0x0200)       /* Comp. E Disable Input Buffer of Port Register .9 */
#define CEPD10                 (0x0400)       /* Comp. E Disable Input Buffer of Port Register .10 */
#define CEPD11                 (0x0800)       /* Comp. E Disable Input Buffer of Port Register .11 */
#define CEPD12                 (0x1000)       /* Comp. E Disable Input Buffer of Port Register .12 */
#define CEPD13                 (0x2000)       /* Comp. E Disable Input Buffer of Port Register .13 */
#define CEPD14                 (0x4000)       /* Comp. E Disable Input Buffer of Port Register .14 */
#define CEPD15                 (0x8000)       /* Comp. E Disable Input Buffer of Port Register .15 */

#define CEPD0_L                (0x0001)       /* Comp. E Disable Input Buffer of Port Register .0 */
#define CEPD1_L                (0x0002)       /* Comp. E Disable Input Buffer of Port Register .1 */
#define CEPD2_L                (0x0004)       /* Comp. E Disable Input Buffer of Port Register .2 */
#define CEPD3_L                (0x0008)       /* Comp. E Disable Input Buffer of Port Register .3 */
#define CEPD4_L                (0x0010)       /* Comp. E Disable Input Buffer of Port Register .4 */
#define CEPD5_L                (0x0020)       /* Comp. E Disable Input Buffer of Port Register .5 */
#define CEPD6_L                (0x0040)       /* Comp. E Disable Input Buffer of Port Register .6 */
#define CEPD7_L                (0x0080)       /* Comp. E Disable Input Buffer of Port Register .7 */

#define CEPD8_H                (0x0001)       /* Comp. E Disable Input Buffer of Port Register .8 */
#define CEPD9_H                (0x0002)       /* Comp. E Disable Input Buffer of Port Register .9 */
#define CEPD10_H               (0x0004)       /* Comp. E Disable Input Buffer of Port Register .10 */
#define CEPD11_H               (0x0008)       /* Comp. E Disable Input Buffer of Port Register .11 */
#define CEPD12_H               (0x0010)       /* Comp. E Disable Input Buffer of Port Register .12 */
#define CEPD13_H               (0x0020)       /* Comp. E Disable Input Buffer of Port Register .13 */
#define CEPD14_H               (0x0040)       /* Comp. E Disable Input Buffer of Port Register .14 */
#define CEPD15_H               (0x0080)       /* Comp. E Disable Input Buffer of Port Register .15 */

/* CEINT Control Bits */
#define CEIFG                  (0x0001)       /* Comp. E Interrupt Flag */
#define CEIIFG                 (0x0002)       /* Comp. E Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004)  /* Comp. E */
//#define RESERVED             (0x0008)  /* Comp. E */
#define CERDYIFG               (0x0010)       /* Comp. E Comparator_E ready interrupt flag */
//#define RESERVED             (0x0020)  /* Comp. E */
//#define RESERVED             (0x0040)  /* Comp. E */
//#define RESERVED             (0x0080)  /* Comp. E */
#define CEIE                   (0x0100)       /* Comp. E Interrupt Enable */
#define CEIIE                  (0x0200)       /* Comp. E Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400)  /* Comp. E */
//#define RESERVED             (0x0800)  /* Comp. E */
#define CERDYIE                (0x1000)       /* Comp. E Comparator_E ready interrupt enable */
//#define RESERVED             (0x2000)  /* Comp. E */
//#define RESERVED             (0x4000)  /* Comp. E */
//#define RESERVED             (0x8000)  /* Comp. E */

/* CEINT Control Bits */
#define CEIFG_L                (0x0001)       /* Comp. E Interrupt Flag */
#define CEIIFG_L               (0x0002)       /* Comp. E Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004)  /* Comp. E */
//#define RESERVED             (0x0008)  /* Comp. E */
#define CERDYIFG_L             (0x0010)       /* Comp. E Comparator_E ready interrupt flag */
//#define RESERVED             (0x0020)  /* Comp. E */
//#define RESERVED             (0x0040)  /* Comp. E */
//#define RESERVED             (0x0080)  /* Comp. E */
//#define RESERVED             (0x0400)  /* Comp. E */
//#define RESERVED             (0x0800)  /* Comp. E */
//#define RESERVED             (0x2000)  /* Comp. E */
//#define RESERVED             (0x4000)  /* Comp. E */
//#define RESERVED             (0x8000)  /* Comp. E */

/* CEINT Control Bits */
//#define RESERVED             (0x0004)  /* Comp. E */
//#define RESERVED             (0x0008)  /* Comp. E */
//#define RESERVED             (0x0020)  /* Comp. E */
//#define RESERVED             (0x0040)  /* Comp. E */
//#define RESERVED             (0x0080)  /* Comp. E */
#define CEIE_H                 (0x0001)       /* Comp. E Interrupt Enable */
#define CEIIE_H                (0x0002)       /* Comp. E Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400)  /* Comp. E */
//#define RESERVED             (0x0800)  /* Comp. E */
#define CERDYIE_H              (0x0010)       /* Comp. E Comparator_E ready interrupt enable */
//#define RESERVED             (0x2000)  /* Comp. E */
//#define RESERVED             (0x4000)  /* Comp. E */
//#define RESERVED             (0x8000)  /* Comp. E */

/* CEIV Definitions */
#define CEIV_NONE              (0x0000)       /* No Interrupt pending */
#define CEIV_CEIFG             (0x0002)       /* CEIFG */
#define CEIV_CEIIFG            (0x0004)       /* CEIIFG */
#define CEIV_CERDYIFG          (0x000A)       /* CERDYIFG */

#endif
/*************************************************************
* CRC Module
*************************************************************/
#ifdef  __MSP430_HAS_CRC__            /* Definition to show that Module is available */

#define OFS_CRCDI              (0x0000)       /* CRC Data In Register */
#define OFS_CRCDI_L            OFS_CRCDI
#define OFS_CRCDI_H            OFS_CRCDI+1
#define OFS_CRCDIRB            (0x0002)       /* CRC data in reverse byte Register */
#define OFS_CRCDIRB_L          OFS_CRCDIRB
#define OFS_CRCDIRB_H          OFS_CRCDIRB+1
#define OFS_CRCINIRES          (0x0004)       /* CRC Initialisation Register and Result Register */
#define OFS_CRCINIRES_L        OFS_CRCINIRES
#define OFS_CRCINIRES_H        OFS_CRCINIRES+1
#define OFS_CRCRESR            (0x0006)       /* CRC reverse result Register */
#define OFS_CRCRESR_L          OFS_CRCRESR
#define OFS_CRCRESR_H          OFS_CRCRESR+1

#endif

/************************************************************
* CLOCK SYSTEM
************************************************************/
#ifdef  __MSP430_HAS_CS__            /* Definition to show that Module is available */

#define OFS_CSCTL0             (0x0000)       /* CS Control Register 0 */
#define OFS_CSCTL0_L           OFS_CSCTL0
#define OFS_CSCTL0_H           OFS_CSCTL0+1
#define OFS_CSCTL1             (0x0002)       /* CS Control Register 1 */
#define OFS_CSCTL1_L           OFS_CSCTL1
#define OFS_CSCTL1_H           OFS_CSCTL1+1
#define OFS_CSCTL2             (0x0004)       /* CS Control Register 2 */
#define OFS_CSCTL2_L           OFS_CSCTL2
#define OFS_CSCTL2_H           OFS_CSCTL2+1
#define OFS_CSCTL3             (0x0006)       /* CS Control Register 3 */
#define OFS_CSCTL3_L           OFS_CSCTL3
#define OFS_CSCTL3_H           OFS_CSCTL3+1
#define OFS_CSCTL4             (0x0008)       /* CS Control Register 4 */
#define OFS_CSCTL4_L           OFS_CSCTL4
#define OFS_CSCTL4_H           OFS_CSCTL4+1
#define OFS_CSCTL5             (0x000A)       /* CS Control Register 5 */
#define OFS_CSCTL5_L           OFS_CSCTL5
#define OFS_CSCTL5_H           OFS_CSCTL5+1
#define OFS_CSCTL6             (0x000C)       /* CS Control Register 6 */
#define OFS_CSCTL6_L           OFS_CSCTL6
#define OFS_CSCTL6_H           OFS_CSCTL6+1

/* CSCTL0 Control Bits */

/* CSCTL0 Control Bits */

/* CSCTL0 Control Bits */
#define CSKEY                  (0xA500)       /* CS Password */

/* CSCTL1 Control Bits */
#define DCOFSEL0               (0x0002)       /* DCO frequency select Bit: 0 */
#define DCOFSEL1               (0x0004)       /* DCO frequency select Bit: 1 */
#define DCOFSEL2               (0x0008)       /* DCO frequency select Bit: 2 */
#define DCORSEL                (0x0040)       /* DCO range select. */

/* CSCTL1 Control Bits */
#define DCOFSEL0_L             (0x0002)       /* DCO frequency select Bit: 0 */
#define DCOFSEL1_L             (0x0004)       /* DCO frequency select Bit: 1 */
#define DCOFSEL2_L             (0x0008)       /* DCO frequency select Bit: 2 */
#define DCORSEL_L              (0x0040)       /* DCO range select. */

/* CSCTL1 Control Bits */

#define DCOFSEL_0              (0x0000)       /* DCO frequency select: 0 */
#define DCOFSEL_1              (0x0002)       /* DCO frequency select: 1 */
#define DCOFSEL_2              (0x0004)       /* DCO frequency select: 2 */
#define DCOFSEL_3              (0x0006)       /* DCO frequency select: 3 */
#define DCOFSEL_4              (0x0008)       /* DCO frequency select: 4 */
#define DCOFSEL_5              (0x000A)       /* DCO frequency select: 5 */
#define DCOFSEL_6              (0x000C)       /* DCO frequency select: 6 */
#define DCOFSEL_7              (0x000E)       /* DCO frequency select: 7 */

/* CSCTL2 Control Bits */
#define SELM0                  (0x0001)       /* MCLK Source Select Bit: 0 */
#define SELM1                  (0x0002)       /* MCLK Source Select Bit: 1 */
#define SELM2                  (0x0004)       /* MCLK Source Select Bit: 2 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELS0                  (0x0010)       /* SMCLK Source Select Bit: 0 */
#define SELS1                  (0x0020)       /* SMCLK Source Select Bit: 1 */
#define SELS2                  (0x0040)       /* SMCLK Source Select Bit: 2 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define SELA0                  (0x0100)       /* ACLK Source Select Bit: 0 */
#define SELA1                  (0x0200)       /* ACLK Source Select Bit: 1 */
#define SELA2                  (0x0400)       /* ACLK Source Select Bit: 2 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CSCTL2 Control Bits */
#define SELM0_L                (0x0001)       /* MCLK Source Select Bit: 0 */
#define SELM1_L                (0x0002)       /* MCLK Source Select Bit: 1 */
#define SELM2_L                (0x0004)       /* MCLK Source Select Bit: 2 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELS0_L                (0x0010)       /* SMCLK Source Select Bit: 0 */
#define SELS1_L                (0x0020)       /* SMCLK Source Select Bit: 1 */
#define SELS2_L                (0x0040)       /* SMCLK Source Select Bit: 2 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CSCTL2 Control Bits */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define SELA0_H                (0x0001)       /* ACLK Source Select Bit: 0 */
#define SELA1_H                (0x0002)       /* ACLK Source Select Bit: 1 */
#define SELA2_H                (0x0004)       /* ACLK Source Select Bit: 2 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

#define SELM_0                 (0x0000)       /* MCLK Source Select 0 */
#define SELM_1                 (0x0001)       /* MCLK Source Select 1 */
#define SELM_2                 (0x0002)       /* MCLK Source Select 2 */
#define SELM_3                 (0x0003)       /* MCLK Source Select 3 */
#define SELM_4                 (0x0004)       /* MCLK Source Select 4 */
#define SELM_5                 (0x0005)       /* MCLK Source Select 5 */
#define SELM_6                 (0x0006)       /* MCLK Source Select 6 */
#define SELM_7                 (0x0007)       /* MCLK Source Select 7 */
#define SELM__LFXTCLK          (0x0000)       /* MCLK Source Select LFXTCLK */
#define SELM__VLOCLK           (0x0001)       /* MCLK Source Select VLOCLK */
#define SELM__LFMODOSC         (0x0002)       /* MCLK Source Select LFMODOSC */
#define SELM__DCOCLK           (0x0003)       /* MCLK Source Select DCOCLK */
#define SELM__MODOSC           (0x0004)       /* MCLK Source Select MODOSC */
#define SELM__HFXTCLK          (0x0005)       /* MCLK Source Select HFXTCLK */

#define SELS_0                 (0x0000)       /* SMCLK Source Select 0 */
#define SELS_1                 (0x0010)       /* SMCLK Source Select 1 */
#define SELS_2                 (0x0020)       /* SMCLK Source Select 2 */
#define SELS_3                 (0x0030)       /* SMCLK Source Select 3 */
#define SELS_4                 (0x0040)       /* SMCLK Source Select 4 */
#define SELS_5                 (0x0050)       /* SMCLK Source Select 5 */
#define SELS_6                 (0x0060)       /* SMCLK Source Select 6 */
#define SELS_7                 (0x0070)       /* SMCLK Source Select 7 */
#define SELS__LFXTCLK          (0x0000)       /* SMCLK Source Select LFXTCLK */
#define SELS__VLOCLK           (0x0010)       /* SMCLK Source Select VLOCLK */
#define SELS__LFMODOSC         (0x0020)       /* SMCLK Source Select LFMODOSC */
#define SELS__DCOCLK           (0x0030)       /* SMCLK Source Select DCOCLK */
#define SELS__MODOSC           (0x0040)       /* SMCLK Source Select MODOSC */
#define SELS__HFXTCLK          (0x0050)       /* SMCLK Source Select HFXTCLK */

#define SELA_0                 (0x0000)       /* ACLK Source Select 0 */
#define SELA_1                 (0x0100)       /* ACLK Source Select 1 */
#define SELA_2                 (0x0200)       /* ACLK Source Select 2 */
#define SELA_3                 (0x0300)       /* ACLK Source Select 3 */
#define SELA_4                 (0x0400)       /* ACLK Source Select 4 */
#define SELA_5                 (0x0500)       /* ACLK Source Select 5 */
#define SELA_6                 (0x0600)       /* ACLK Source Select 6 */
#define SELA_7                 (0x0700)       /* ACLK Source Select 7 */
#define SELA__LFXTCLK          (0x0000)       /* ACLK Source Select LFXTCLK */
#define SELA__VLOCLK           (0x0100)       /* ACLK Source Select VLOCLK */
#define SELA__LFMODOSC         (0x0200)       /* ACLK Source Select LFMODOSC */

/* CSCTL3 Control Bits */
#define DIVM0                  (0x0001)       /* MCLK Divider Bit: 0 */
#define DIVM1                  (0x0002)       /* MCLK Divider Bit: 1 */
#define DIVM2                  (0x0004)       /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DIVS0                  (0x0010)       /* SMCLK Divider Bit: 0 */
#define DIVS1                  (0x0020)       /* SMCLK Divider Bit: 1 */
#define DIVS2                  (0x0040)       /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define DIVA0                  (0x0100)       /* ACLK Divider Bit: 0 */
#define DIVA1                  (0x0200)       /* ACLK Divider Bit: 1 */
#define DIVA2                  (0x0400)       /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CSCTL3 Control Bits */
#define DIVM0_L                (0x0001)       /* MCLK Divider Bit: 0 */
#define DIVM1_L                (0x0002)       /* MCLK Divider Bit: 1 */
#define DIVM2_L                (0x0004)       /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DIVS0_L                (0x0010)       /* SMCLK Divider Bit: 0 */
#define DIVS1_L                (0x0020)       /* SMCLK Divider Bit: 1 */
#define DIVS2_L                (0x0040)       /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CSCTL3 Control Bits */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define DIVA0_H                (0x0001)       /* ACLK Divider Bit: 0 */
#define DIVA1_H                (0x0002)       /* ACLK Divider Bit: 1 */
#define DIVA2_H                (0x0004)       /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

#define DIVM_0                 (0x0000)       /* MCLK Source Divider 0 */
#define DIVM_1                 (0x0001)       /* MCLK Source Divider 1 */
#define DIVM_2                 (0x0002)       /* MCLK Source Divider 2 */
#define DIVM_3                 (0x0003)       /* MCLK Source Divider 3 */
#define DIVM_4                 (0x0004)       /* MCLK Source Divider 4 */
#define DIVM_5                 (0x0005)       /* MCLK Source Divider 5 */
#define DIVM__1                (0x0000)       /* MCLK Source Divider f(MCLK)/1 */
#define DIVM__2                (0x0001)       /* MCLK Source Divider f(MCLK)/2 */
#define DIVM__4                (0x0002)       /* MCLK Source Divider f(MCLK)/4 */
#define DIVM__8                (0x0003)       /* MCLK Source Divider f(MCLK)/8 */
#define DIVM__16               (0x0004)       /* MCLK Source Divider f(MCLK)/16 */
#define DIVM__32               (0x0005)       /* MCLK Source Divider f(MCLK)/32 */

#define DIVS_0                 (0x0000)       /* SMCLK Source Divider 0 */
#define DIVS_1                 (0x0010)       /* SMCLK Source Divider 1 */
#define DIVS_2                 (0x0020)       /* SMCLK Source Divider 2 */
#define DIVS_3                 (0x0030)       /* SMCLK Source Divider 3 */
#define DIVS_4                 (0x0040)       /* SMCLK Source Divider 4 */
#define DIVS_5                 (0x0050)       /* SMCLK Source Divider 5 */
#define DIVS__1                (0x0000)       /* SMCLK Source Divider f(SMCLK)/1 */
#define DIVS__2                (0x0010)       /* SMCLK Source Divider f(SMCLK)/2 */
#define DIVS__4                (0x0020)       /* SMCLK Source Divider f(SMCLK)/4 */
#define DIVS__8                (0x0030)       /* SMCLK Source Divider f(SMCLK)/8 */
#define DIVS__16               (0x0040)       /* SMCLK Source Divider f(SMCLK)/16 */
#define DIVS__32               (0x0050)       /* SMCLK Source Divider f(SMCLK)/32 */

#define DIVA_0                 (0x0000)       /* ACLK Source Divider 0 */
#define DIVA_1                 (0x0100)       /* ACLK Source Divider 1 */
#define DIVA_2                 (0x0200)       /* ACLK Source Divider 2 */
#define DIVA_3                 (0x0300)       /* ACLK Source Divider 3 */
#define DIVA_4                 (0x0400)       /* ACLK Source Divider 4 */
#define DIVA_5                 (0x0500)       /* ACLK Source Divider 5 */
#define DIVA__1                (0x0000)       /* ACLK Source Divider f(ACLK)/1 */
#define DIVA__2                (0x0100)       /* ACLK Source Divider f(ACLK)/2 */
#define DIVA__4                (0x0200)       /* ACLK Source Divider f(ACLK)/4 */
#define DIVA__8                (0x0300)       /* ACLK Source Divider f(ACLK)/8 */
#define DIVA__16               (0x0400)       /* ACLK Source Divider f(ACLK)/16 */
#define DIVA__32               (0x0500)       /* ACLK Source Divider f(ACLK)/32 */

/* CSCTL4 Control Bits */
#define LFXTOFF                (0x0001)       /* High Frequency Oscillator 1 (XT1) disable */
#define SMCLKOFF               (0x0002)       /* SMCLK Off */
#define VLOOFF                 (0x0008)       /* VLO Off */
#define LFXTBYPASS             (0x0010)       /* LFXT bypass mode : 0: internal 1:sourced from external pin */
#define LFXTDRIVE0             (0x0040)       /* LFXT Drive Level mode Bit 0 */
#define LFXTDRIVE1             (0x0080)       /* LFXT Drive Level mode Bit 1 */
#define HFXTOFF                (0x0100)       /* High Frequency Oscillator disable */
#define HFFREQ0                (0x0400)       /* HFXT frequency selection Bit 1 */
#define HFFREQ1                (0x0800)       /* HFXT frequency selection Bit 0 */
#define HFXTBYPASS             (0x1000)       /* HFXT bypass mode : 0: internal 1:sourced from external pin */
#define HFXTDRIVE0             (0x4000)       /* HFXT Drive Level mode Bit 0 */
#define HFXTDRIVE1             (0x8000)       /* HFXT Drive Level mode Bit 1 */

/* CSCTL4 Control Bits */
#define LFXTOFF_L              (0x0001)       /* High Frequency Oscillator 1 (XT1) disable */
#define SMCLKOFF_L             (0x0002)       /* SMCLK Off */
#define VLOOFF_L               (0x0008)       /* VLO Off */
#define LFXTBYPASS_L           (0x0010)       /* LFXT bypass mode : 0: internal 1:sourced from external pin */
#define LFXTDRIVE0_L           (0x0040)       /* LFXT Drive Level mode Bit 0 */
#define LFXTDRIVE1_L           (0x0080)       /* LFXT Drive Level mode Bit 1 */

/* CSCTL4 Control Bits */
#define HFXTOFF_H              (0x0001)       /* High Frequency Oscillator disable */
#define HFFREQ0_H              (0x0004)       /* HFXT frequency selection Bit 1 */
#define HFFREQ1_H              (0x0008)       /* HFXT frequency selection Bit 0 */
#define HFXTBYPASS_H           (0x0010)       /* HFXT bypass mode : 0: internal 1:sourced from external pin */
#define HFXTDRIVE0_H           (0x0040)       /* HFXT Drive Level mode Bit 0 */
#define HFXTDRIVE1_H           (0x0080)       /* HFXT Drive Level mode Bit 1 */

#define LFXTDRIVE_0            (0x0000)       /* LFXT Drive Level mode: 0 */
#define LFXTDRIVE_1            (0x0040)       /* LFXT Drive Level mode: 1 */
#define LFXTDRIVE_2            (0x0080)       /* LFXT Drive Level mode: 2 */
#define LFXTDRIVE_3            (0x00C0)       /* LFXT Drive Level mode: 3 */

#define HFFREQ_0               (0x0000)       /* HFXT frequency selection: 0 */
#define HFFREQ_1               (0x0400)       /* HFXT frequency selection: 1 */
#define HFFREQ_2               (0x0800)       /* HFXT frequency selection: 2 */
#define HFFREQ_3               (0x0C00)       /* HFXT frequency selection: 3 */

#define HFXTDRIVE_0            (0x0000)       /* HFXT Drive Level mode: 0 */
#define HFXTDRIVE_1            (0x4000)       /* HFXT Drive Level mode: 1 */
#define HFXTDRIVE_2            (0x8000)       /* HFXT Drive Level mode: 2 */
#define HFXTDRIVE_3            (0xC000)       /* HFXT Drive Level mode: 3 */

/* CSCTL5 Control Bits */
#define LFXTOFFG               (0x0001)       /* LFXT Low Frequency Oscillator Fault Flag */
#define HFXTOFFG               (0x0002)       /* HFXT High Frequency Oscillator Fault Flag */
#define ENSTFCNT1              (0x0040)       /* Enable start counter for XT1 */
#define ENSTFCNT2              (0x0080)       /* Enable start counter for XT2 */

/* CSCTL5 Control Bits */
#define LFXTOFFG_L             (0x0001)       /* LFXT Low Frequency Oscillator Fault Flag */
#define HFXTOFFG_L             (0x0002)       /* HFXT High Frequency Oscillator Fault Flag */
#define ENSTFCNT1_L            (0x0040)       /* Enable start counter for XT1 */
#define ENSTFCNT2_L            (0x0080)       /* Enable start counter for XT2 */

/* CSCTL5 Control Bits */

/* CSCTL6 Control Bits */
#define ACLKREQEN              (0x0001)       /* ACLK Clock Request Enable */
#define MCLKREQEN              (0x0002)       /* MCLK Clock Request Enable */
#define SMCLKREQEN             (0x0004)       /* SMCLK Clock Request Enable */
#define MODCLKREQEN            (0x0008)       /* MODOSC Clock Request Enable */

/* CSCTL6 Control Bits */
#define ACLKREQEN_L            (0x0001)       /* ACLK Clock Request Enable */
#define MCLKREQEN_L            (0x0002)       /* MCLK Clock Request Enable */
#define SMCLKREQEN_L           (0x0004)       /* SMCLK Clock Request Enable */
#define MODCLKREQEN_L          (0x0008)       /* MODOSC Clock Request Enable */

/* CSCTL6 Control Bits */

#endif
/************************************************************
* DMA_X
************************************************************/
#ifdef  __MSP430_HAS_DMAX_3__           /* Definition to show that Module is available */

#define OFS_DMACTL0            (0x0000)       /* DMA Module Control 0 */
#define OFS_DMACTL0_L          OFS_DMACTL0
#define OFS_DMACTL0_H          OFS_DMACTL0+1
#define OFS_DMACTL1            (0x0002)       /* DMA Module Control 1 */
#define OFS_DMACTL1_L          OFS_DMACTL1
#define OFS_DMACTL1_H          OFS_DMACTL1+1
#define OFS_DMACTL2            (0x0004)       /* DMA Module Control 2 */
#define OFS_DMACTL2_L          OFS_DMACTL2
#define OFS_DMACTL2_H          OFS_DMACTL2+1
#define OFS_DMACTL3            (0x0006)       /* DMA Module Control 3 */
#define OFS_DMACTL3_L          OFS_DMACTL3
#define OFS_DMACTL3_H          OFS_DMACTL3+1
#define OFS_DMACTL4            (0x0008)       /* DMA Module Control 4 */
#define OFS_DMACTL4_L          OFS_DMACTL4
#define OFS_DMACTL4_H          OFS_DMACTL4+1
#define OFS_DMAIV              (0x000E)       /* DMA Interrupt Vector Word */
#define OFS_DMAIV_L            OFS_DMAIV
#define OFS_DMAIV_H            OFS_DMAIV+1

#define OFS_DMA0CTL            (0x0010)       /* DMA Channel 0 Control */
#define OFS_DMA0CTL_L          OFS_DMA0CTL
#define OFS_DMA0CTL_H          OFS_DMA0CTL+1
#define     DMA0SA_O           (0x0012)       /* DMA Channel 0 Source Address */
#define     DMA0DA_O           (0x0016)       /* DMA Channel 0 Destination Address */
#define OFS_DMA0SZ             (0x001A)       /* DMA Channel 0 Transfer Size */

#define OFS_DMA1CTL            (0x0020)       /* DMA Channel 1 Control */
#define OFS_DMA1CTL_L          OFS_DMA1CTL
#define OFS_DMA1CTL_H          OFS_DMA1CTL+1
#define     DMA1SA_O           (0x0022)       /* DMA Channel 1 Source Address */
#define     DMA1DA_O           (0x0026)       /* DMA Channel 1 Destination Address */
#define OFS_DMA1SZ             (0x002A)       /* DMA Channel 1 Transfer Size */

#define OFS_DMA2CTL            (0x0030)       /* DMA Channel 2 Control */
#define OFS_DMA2CTL_L          OFS_DMA2CTL
#define OFS_DMA2CTL_H          OFS_DMA2CTL+1
#define     DMA2SA_O           (0x0032)       /* DMA Channel 2 Source Address */
#define     DMA2DA_O           (0x0036)       /* DMA Channel 2 Destination Address */
#define OFS_DMA2SZ             (0x003A)       /* DMA Channel 2 Transfer Size */

/* DMACTL0 Control Bits */
#define DMA0TSEL0              (0x0001)       /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1              (0x0002)       /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2              (0x0004)       /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3              (0x0008)       /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4              (0x0010)       /* DMA channel 0 transfer select bit 4 */
#define DMA1TSEL0              (0x0100)       /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1              (0x0200)       /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2              (0x0400)       /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3              (0x0800)       /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4              (0x1000)       /* DMA channel 1 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA0TSEL0_L            (0x0001)       /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1_L            (0x0002)       /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2_L            (0x0004)       /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3_L            (0x0008)       /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4_L            (0x0010)       /* DMA channel 0 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA1TSEL0_H            (0x0001)       /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1_H            (0x0002)       /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2_H            (0x0004)       /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3_H            (0x0008)       /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4_H            (0x0010)       /* DMA channel 1 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0              (0x0001)       /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1              (0x0002)       /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2              (0x0004)       /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3              (0x0008)       /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4              (0x0010)       /* DMA channel 2 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0_L            (0x0001)       /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1_L            (0x0002)       /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2_L            (0x0004)       /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3_L            (0x0008)       /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4_L            (0x0010)       /* DMA channel 2 transfer select bit 4 */

/* DMACTL01 Control Bits */

/* DMACTL4 Control Bits */
#define ENNMI                  (0x0001)       /* Enable NMI interruption of DMA */
#define ROUNDROBIN             (0x0002)       /* Round-Robin DMA channel priorities */
#define DMARMWDIS              (0x0004)       /* Inhibited DMA transfers during read-modify-write CPU operations */

/* DMACTL4 Control Bits */
#define ENNMI_L                (0x0001)       /* Enable NMI interruption of DMA */
#define ROUNDROBIN_L           (0x0002)       /* Round-Robin DMA channel priorities */
#define DMARMWDIS_L            (0x0004)       /* Inhibited DMA transfers during read-modify-write CPU operations */

/* DMACTL4 Control Bits */

/* DMAxCTL Control Bits */
#define DMAREQ                 (0x0001)       /* Initiate DMA transfer with DMATSEL */
#define DMAABORT               (0x0002)       /* DMA transfer aborted by NMI */
#define DMAIE                  (0x0004)       /* DMA interrupt enable */
#define DMAIFG                 (0x0008)       /* DMA interrupt flag */
#define DMAEN                  (0x0010)       /* DMA enable */
#define DMALEVEL               (0x0020)       /* DMA level sensitive trigger select */
#define DMASRCBYTE             (0x0040)       /* DMA source byte */
#define DMADSTBYTE             (0x0080)       /* DMA destination byte */
#define DMASRCINCR0            (0x0100)       /* DMA source increment bit 0 */
#define DMASRCINCR1            (0x0200)       /* DMA source increment bit 1 */
#define DMADSTINCR0            (0x0400)       /* DMA destination increment bit 0 */
#define DMADSTINCR1            (0x0800)       /* DMA destination increment bit 1 */
#define DMADT0                 (0x1000)       /* DMA transfer mode bit 0 */
#define DMADT1                 (0x2000)       /* DMA transfer mode bit 1 */
#define DMADT2                 (0x4000)       /* DMA transfer mode bit 2 */

/* DMAxCTL Control Bits */
#define DMAREQ_L               (0x0001)       /* Initiate DMA transfer with DMATSEL */
#define DMAABORT_L             (0x0002)       /* DMA transfer aborted by NMI */
#define DMAIE_L                (0x0004)       /* DMA interrupt enable */
#define DMAIFG_L               (0x0008)       /* DMA interrupt flag */
#define DMAEN_L                (0x0010)       /* DMA enable */
#define DMALEVEL_L             (0x0020)       /* DMA level sensitive trigger select */
#define DMASRCBYTE_L           (0x0040)       /* DMA source byte */
#define DMADSTBYTE_L           (0x0080)       /* DMA destination byte */

/* DMAxCTL Control Bits */
#define DMASRCINCR0_H          (0x0001)       /* DMA source increment bit 0 */
#define DMASRCINCR1_H          (0x0002)       /* DMA source increment bit 1 */
#define DMADSTINCR0_H          (0x0004)       /* DMA destination increment bit 0 */
#define DMADSTINCR1_H          (0x0008)       /* DMA destination increment bit 1 */
#define DMADT0_H               (0x0010)       /* DMA transfer mode bit 0 */
#define DMADT1_H               (0x0020)       /* DMA transfer mode bit 1 */
#define DMADT2_H               (0x0040)       /* DMA transfer mode bit 2 */

#define DMASWDW                (0*0x0040u)    /* DMA transfer: source word to destination word */
#define DMASBDW                (1*0x0040u)    /* DMA transfer: source byte to destination word */
#define DMASWDB                (2*0x0040u)    /* DMA transfer: source word to destination byte */
#define DMASBDB                (3*0x0040u)    /* DMA transfer: source byte to destination byte */

#define DMASRCINCR_0           (0*0x0100u)    /* DMA source increment 0: source address unchanged */
#define DMASRCINCR_1           (1*0x0100u)    /* DMA source increment 1: source address unchanged */
#define DMASRCINCR_2           (2*0x0100u)    /* DMA source increment 2: source address decremented */
#define DMASRCINCR_3           (3*0x0100u)    /* DMA source increment 3: source address incremented */

#define DMADSTINCR_0           (0*0x0400u)    /* DMA destination increment 0: destination address unchanged */
#define DMADSTINCR_1           (1*0x0400u)    /* DMA destination increment 1: destination address unchanged */
#define DMADSTINCR_2           (2*0x0400u)    /* DMA destination increment 2: destination address decremented */
#define DMADSTINCR_3           (3*0x0400u)    /* DMA destination increment 3: destination address incremented */

#define DMADT_0                (0*0x1000u)    /* DMA transfer mode 0: Single transfer */
#define DMADT_1                (1*0x1000u)    /* DMA transfer mode 1: Block transfer */
#define DMADT_2                (2*0x1000u)    /* DMA transfer mode 2: Burst-Block transfer */
#define DMADT_3                (3*0x1000u)    /* DMA transfer mode 3: Burst-Block transfer */
#define DMADT_4                (4*0x1000u)    /* DMA transfer mode 4: Repeated Single transfer */
#define DMADT_5                (5*0x1000u)    /* DMA transfer mode 5: Repeated Block transfer */
#define DMADT_6                (6*0x1000u)    /* DMA transfer mode 6: Repeated Burst-Block transfer */
#define DMADT_7                (7*0x1000u)    /* DMA transfer mode 7: Repeated Burst-Block transfer */

/* DMAIV Definitions */
#define DMAIV_NONE             (0x0000)       /* No Interrupt pending */
#define DMAIV_DMA0IFG          (0x0002)       /* DMA0IFG*/
#define DMAIV_DMA1IFG          (0x0004)       /* DMA1IFG*/
#define DMAIV_DMA2IFG          (0x0006)       /* DMA2IFG*/

#endif
/************************************************************
* DMA_X
************************************************************/
#ifdef  __MSP430_HAS_DMAX_6__           /* Definition to show that Module is available */

#define OFS_DMACTL0            (0x0000)       /* DMA Module Control 0 */
#define OFS_DMACTL0_L          OFS_DMACTL0
#define OFS_DMACTL0_H          OFS_DMACTL0+1
#define OFS_DMACTL1            (0x0002)       /* DMA Module Control 1 */
#define OFS_DMACTL1_L          OFS_DMACTL1
#define OFS_DMACTL1_H          OFS_DMACTL1+1
#define OFS_DMACTL2            (0x0004)       /* DMA Module Control 2 */
#define OFS_DMACTL2_L          OFS_DMACTL2
#define OFS_DMACTL2_H          OFS_DMACTL2+1
#define OFS_DMACTL3            (0x0006)       /* DMA Module Control 3 */
#define OFS_DMACTL3_L          OFS_DMACTL3
#define OFS_DMACTL3_H          OFS_DMACTL3+1
#define OFS_DMACTL4            (0x0008)       /* DMA Module Control 4 */
#define OFS_DMACTL4_L          OFS_DMACTL4
#define OFS_DMACTL4_H          OFS_DMACTL4+1
#define OFS_DMAIV              (0x000E)       /* DMA Interrupt Vector Word */
#define OFS_DMAIV_L            OFS_DMAIV
#define OFS_DMAIV_H            OFS_DMAIV+1

#define OFS_DMA0CTL            (0x0010)       /* DMA Channel 0 Control */
#define OFS_DMA0CTL_L          OFS_DMA0CTL
#define OFS_DMA0CTL_H          OFS_DMA0CTL+1
#define     DMA0SA_O           (0x0012)       /* DMA Channel 0 Source Address */
#define     DMA0DA_O           (0x0016)       /* DMA Channel 0 Destination Address */
#define OFS_DMA0SZ             (0x001A)       /* DMA Channel 0 Transfer Size */

#define OFS_DMA1CTL            (0x0020)       /* DMA Channel 1 Control */
#define OFS_DMA1CTL_L          OFS_DMA1CTL
#define OFS_DMA1CTL_H          OFS_DMA1CTL+1
#define     DMA1SA_O           (0x0022)       /* DMA Channel 1 Source Address */
#define     DMA1DA_O           (0x0026)       /* DMA Channel 1 Destination Address */
#define OFS_DMA1SZ             (0x002A)       /* DMA Channel 1 Transfer Size */

#define OFS_DMA2CTL            (0x0030)       /* DMA Channel 2 Control */
#define OFS_DMA2CTL_L          OFS_DMA2CTL
#define OFS_DMA2CTL_H          OFS_DMA2CTL+1
#define     DMA2SA_O           (0x0032)       /* DMA Channel 2 Source Address */
#define     DMA2DA_O           (0x0036)       /* DMA Channel 2 Destination Address */
#define OFS_DMA2SZ             (0x003A)       /* DMA Channel 2 Transfer Size */

#define OFS_DMA3CTL            (0x0040)       /* DMA Channel 3 Control */
#define OFS_DMA3CTL_L          OFS_DMA3CTL
#define OFS_DMA3CTL_H          OFS_DMA3CTL+1
#define     DMA3SA_O           (0x0042)       /* DMA Channel 3 Source Address */
#define     DMA3DA_O           (0x0046)       /* DMA Channel 3 Destination Address */
#define OFS_DMA3SZ             (0x004A)       /* DMA Channel 3 Transfer Size */

#define OFS_DMA4CTL            (0x0050)       /* DMA Channel 4 Control */
#define OFS_DMA4CTL_L          OFS_DMA4CTL
#define OFS_DMA4CTL_H          OFS_DMA4CTL+1
#define     DMA4SA_O           (0x0052)       /* DMA Channel 4 Source Address */
#define     DMA4DA_O           (0x0056)       /* DMA Channel 4 Destination Address */
#define OFS_DMA4SZ             (0x005A)       /* DMA Channel 4 Transfer Size */

#define OFS_DMA5CTL            (0x0060)       /* DMA Channel 5 Control */
#define OFS_DMA5CTL_L          OFS_DMA5CTL
#define OFS_DMA5CTL_H          OFS_DMA5CTL+1
#define     DMA5SA_O           (0x0062)       /* DMA Channel 5 Source Address */
#define     DMA5DA_O           (0x0066)       /* DMA Channel 5 Destination Address */
#define OFS_DMA5SZ             (0x006A)       /* DMA Channel 5 Transfer Size */

/* DMACTL0 Control Bits */
#define DMA0TSEL0              (0x0001)       /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1              (0x0002)       /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2              (0x0004)       /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3              (0x0008)       /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4              (0x0010)       /* DMA channel 0 transfer select bit 4 */
#define DMA1TSEL0              (0x0100)       /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1              (0x0200)       /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2              (0x0400)       /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3              (0x0800)       /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4              (0x1000)       /* DMA channel 1 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA0TSEL0_L            (0x0001)       /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1_L            (0x0002)       /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2_L            (0x0004)       /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3_L            (0x0008)       /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4_L            (0x0010)       /* DMA channel 0 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA1TSEL0_H            (0x0001)       /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1_H            (0x0002)       /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2_H            (0x0004)       /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3_H            (0x0008)       /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4_H            (0x0010)       /* DMA channel 1 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0              (0x0001)       /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1              (0x0002)       /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2              (0x0004)       /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3              (0x0008)       /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4              (0x0010)       /* DMA channel 2 transfer select bit 4 */
#define DMA3TSEL0              (0x0100)       /* DMA channel 3 transfer select bit 0 */
#define DMA3TSEL1              (0x0200)       /* DMA channel 3 transfer select bit 1 */
#define DMA3TSEL2              (0x0400)       /* DMA channel 3 transfer select bit 2 */
#define DMA3TSEL3              (0x0800)       /* DMA channel 3 transfer select bit 3 */
#define DMA3TSEL4              (0x1000)       /* DMA channel 3 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0_L            (0x0001)       /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1_L            (0x0002)       /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2_L            (0x0004)       /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3_L            (0x0008)       /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4_L            (0x0010)       /* DMA channel 2 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA3TSEL0_H            (0x0001)       /* DMA channel 3 transfer select bit 0 */
#define DMA3TSEL1_H            (0x0002)       /* DMA channel 3 transfer select bit 1 */
#define DMA3TSEL2_H            (0x0004)       /* DMA channel 3 transfer select bit 2 */
#define DMA3TSEL3_H            (0x0008)       /* DMA channel 3 transfer select bit 3 */
#define DMA3TSEL4_H            (0x0010)       /* DMA channel 3 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA4TSEL0              (0x0001)       /* DMA channel 4 transfer select bit 0 */
#define DMA4TSEL1              (0x0002)       /* DMA channel 4 transfer select bit 1 */
#define DMA4TSEL2              (0x0004)       /* DMA channel 4 transfer select bit 2 */
#define DMA4TSEL3              (0x0008)       /* DMA channel 4 transfer select bit 3 */
#define DMA4TSEL4              (0x0010)       /* DMA channel 4 transfer select bit 4 */
#define DMA5TSEL0              (0x0100)       /* DMA channel 5 transfer select bit 0 */
#define DMA5TSEL1              (0x0200)       /* DMA channel 5 transfer select bit 1 */
#define DMA5TSEL2              (0x0400)       /* DMA channel 5 transfer select bit 2 */
#define DMA5TSEL3              (0x0800)       /* DMA channel 5 transfer select bit 3 */
#define DMA5TSEL4              (0x1000)       /* DMA channel 5 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA4TSEL0_L            (0x0001)       /* DMA channel 4 transfer select bit 0 */
#define DMA4TSEL1_L            (0x0002)       /* DMA channel 4 transfer select bit 1 */
#define DMA4TSEL2_L            (0x0004)       /* DMA channel 4 transfer select bit 2 */
#define DMA4TSEL3_L            (0x0008)       /* DMA channel 4 transfer select bit 3 */
#define DMA4TSEL4_L            (0x0010)       /* DMA channel 4 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA5TSEL0_H            (0x0001)       /* DMA channel 5 transfer select bit 0 */
#define DMA5TSEL1_H            (0x0002)       /* DMA channel 5 transfer select bit 1 */
#define DMA5TSEL2_H            (0x0004)       /* DMA channel 5 transfer select bit 2 */
#define DMA5TSEL3_H            (0x0008)       /* DMA channel 5 transfer select bit 3 */
#define DMA5TSEL4_H            (0x0010)       /* DMA channel 5 transfer select bit 4 */

/* DMACTL4 Control Bits */
#define ENNMI                  (0x0001)       /* Enable NMI interruption of DMA */
#define ROUNDROBIN             (0x0002)       /* Round-Robin DMA channel priorities */
#define DMARMWDIS              (0x0004)       /* Inhibited DMA transfers during read-modify-write CPU operations */

/* DMACTL4 Control Bits */
#define ENNMI_L                (0x0001)       /* Enable NMI interruption of DMA */
#define ROUNDROBIN_L           (0x0002)       /* Round-Robin DMA channel priorities */
#define DMARMWDIS_L            (0x0004)       /* Inhibited DMA transfers during read-modify-write CPU operations */

/* DMACTL4 Control Bits */

/* DMAxCTL Control Bits */
#define DMAREQ                 (0x0001)       /* Initiate DMA transfer with DMATSEL */
#define DMAABORT               (0x0002)       /* DMA transfer aborted by NMI */
#define DMAIE                  (0x0004)       /* DMA interrupt enable */
#define DMAIFG                 (0x0008)       /* DMA interrupt flag */
#define DMAEN                  (0x0010)       /* DMA enable */
#define DMALEVEL               (0x0020)       /* DMA level sensitive trigger select */
#define DMASRCBYTE             (0x0040)       /* DMA source byte */
#define DMADSTBYTE             (0x0080)       /* DMA destination byte */
#define DMASRCINCR0            (0x0100)       /* DMA source increment bit 0 */
#define DMASRCINCR1            (0x0200)       /* DMA source increment bit 1 */
#define DMADSTINCR0            (0x0400)       /* DMA destination increment bit 0 */
#define DMADSTINCR1            (0x0800)       /* DMA destination increment bit 1 */
#define DMADT0                 (0x1000)       /* DMA transfer mode bit 0 */
#define DMADT1                 (0x2000)       /* DMA transfer mode bit 1 */
#define DMADT2                 (0x4000)       /* DMA transfer mode bit 2 */

/* DMAxCTL Control Bits */
#define DMAREQ_L               (0x0001)       /* Initiate DMA transfer with DMATSEL */
#define DMAABORT_L             (0x0002)       /* DMA transfer aborted by NMI */
#define DMAIE_L                (0x0004)       /* DMA interrupt enable */
#define DMAIFG_L               (0x0008)       /* DMA interrupt flag */
#define DMAEN_L                (0x0010)       /* DMA enable */
#define DMALEVEL_L             (0x0020)       /* DMA level sensitive trigger select */
#define DMASRCBYTE_L           (0x0040)       /* DMA source byte */
#define DMADSTBYTE_L           (0x0080)       /* DMA destination byte */

/* DMAxCTL Control Bits */
#define DMASRCINCR0_H          (0x0001)       /* DMA source increment bit 0 */
#define DMASRCINCR1_H          (0x0002)       /* DMA source increment bit 1 */
#define DMADSTINCR0_H          (0x0004)       /* DMA destination increment bit 0 */
#define DMADSTINCR1_H          (0x0008)       /* DMA destination increment bit 1 */
#define DMADT0_H               (0x0010)       /* DMA transfer mode bit 0 */
#define DMADT1_H               (0x0020)       /* DMA transfer mode bit 1 */
#define DMADT2_H               (0x0040)       /* DMA transfer mode bit 2 */

#define DMASWDW                (0*0x0040u)    /* DMA transfer: source word to destination word */
#define DMASBDW                (1*0x0040u)    /* DMA transfer: source byte to destination word */
#define DMASWDB                (2*0x0040u)    /* DMA transfer: source word to destination byte */
#define DMASBDB                (3*0x0040u)    /* DMA transfer: source byte to destination byte */

#define DMASRCINCR_0           (0*0x0100u)    /* DMA source increment 0: source address unchanged */
#define DMASRCINCR_1           (1*0x0100u)    /* DMA source increment 1: source address unchanged */
#define DMASRCINCR_2           (2*0x0100u)    /* DMA source increment 2: source address decremented */
#define DMASRCINCR_3           (3*0x0100u)    /* DMA source increment 3: source address incremented */

#define DMADSTINCR_0           (0*0x0400u)    /* DMA destination increment 0: destination address unchanged */
#define DMADSTINCR_1           (1*0x0400u)    /* DMA destination increment 1: destination address unchanged */
#define DMADSTINCR_2           (2*0x0400u)    /* DMA destination increment 2: destination address decremented */
#define DMADSTINCR_3           (3*0x0400u)    /* DMA destination increment 3: destination address incremented */

#define DMADT_0                (0*0x1000u)    /* DMA transfer mode 0: Single transfer */
#define DMADT_1                (1*0x1000u)    /* DMA transfer mode 1: Block transfer */
#define DMADT_2                (2*0x1000u)    /* DMA transfer mode 2: Burst-Block transfer */
#define DMADT_3                (3*0x1000u)    /* DMA transfer mode 3: Burst-Block transfer */
#define DMADT_4                (4*0x1000u)    /* DMA transfer mode 4: Repeated Single transfer */
#define DMADT_5                (5*0x1000u)    /* DMA transfer mode 5: Repeated Block transfer */
#define DMADT_6                (6*0x1000u)    /* DMA transfer mode 6: Repeated Burst-Block transfer */
#define DMADT_7                (7*0x1000u)    /* DMA transfer mode 7: Repeated Burst-Block transfer */

/* DMAIV Definitions */
#define DMAIV_NONE             (0x0000)       /* No Interrupt pending */
#define DMAIV_DMA0IFG          (0x0002)       /* DMA0IFG*/
#define DMAIV_DMA1IFG          (0x0004)       /* DMA1IFG*/
#define DMAIV_DMA2IFG          (0x0006)       /* DMA2IFG*/
#define DMAIV_DMA3IFG          (0x0008)       /* DMA3IFG*/
#define DMAIV_DMA4IFG          (0x000A)       /* DMA4IFG*/
#define DMAIV_DMA5IFG          (0x000C)       /* DMA5IFG*/

#endif
/*************************************************************
* FRAM Memory
*************************************************************/
#ifdef  __MSP430_HAS_FRAM_FR5XX__     /* Definition to show that Module is available */

#define OFS_FRCTL0             (0x0000)       /* FRAM Controller Control 0 */
#define OFS_FRCTL0_L           OFS_FRCTL0
#define OFS_FRCTL0_H           OFS_FRCTL0+1
#define OFS_GCCTL0             (0x0004)       /* General Control 0 */
#define OFS_GCCTL0_L           OFS_GCCTL0
#define OFS_GCCTL0_H           OFS_GCCTL0+1
#define OFS_GCCTL1             (0x0006)       /* General Control 1 */
#define OFS_GCCTL1_L           OFS_GCCTL1
#define OFS_GCCTL1_H           OFS_GCCTL1+1

#define FRPW                   (0x9600)       /* FRAM password returned by read */
#define FWPW                   (0xA500)       /* FRAM password for write */
#define FXPW                   (0x3300)       /* for use with XOR instruction */

/* FRCTL0 Control Bits */
#define NPRECHG0               (0x0001)       /* FRAM Wait state Generator Precharge Time control Bit: 0 */
#define NPRECHG1               (0x0002)       /* FRAM Wait state Generator Precharge Time control Bit: 1 */
#define NPRECHG2               (0x0004)       /* FRAM Wait state Generator Precharge Time control Bit: 2 */
#define NAUTO                  (0x0008)       /* FRAM Disables the wait state generator */
#define NACCESS0               (0x0010)       /* FRAM Wait state Generator Access Time control Bit: 0 */
#define NACCESS1               (0x0020)       /* FRAM Wait state Generator Access Time control Bit: 1 */
#define NACCESS2               (0x0040)       /* FRAM Wait state Generator Access Time control Bit: 2 */
//#define RESERVED            (0x0080)  /* RESERVED */

/* FRCTL0 Control Bits */
#define NPRECHG0_L             (0x0001)       /* FRAM Wait state Generator Precharge Time control Bit: 0 */
#define NPRECHG1_L             (0x0002)       /* FRAM Wait state Generator Precharge Time control Bit: 1 */
#define NPRECHG2_L             (0x0004)       /* FRAM Wait state Generator Precharge Time control Bit: 2 */
#define NAUTO_L                (0x0008)       /* FRAM Disables the wait state generator */
#define NACCESS0_L             (0x0010)       /* FRAM Wait state Generator Access Time control Bit: 0 */
#define NACCESS1_L             (0x0020)       /* FRAM Wait state Generator Access Time control Bit: 1 */
#define NACCESS2_L             (0x0040)       /* FRAM Wait state Generator Access Time control Bit: 2 */
//#define RESERVED            (0x0080)  /* RESERVED */

/* FRCTL0 Control Bits */
//#define RESERVED            (0x0080)  /* RESERVED */

#define NPRECHG_0              (0x0000)       /* FRAM Wait state Generator Precharge Time control: 0 */
#define NPRECHG_1              (0x0001)       /* FRAM Wait state Generator Precharge Time control: 1 */
#define NPRECHG_2              (0x0002)       /* FRAM Wait state Generator Precharge Time control: 2 */
#define NPRECHG_3              (0x0003)       /* FRAM Wait state Generator Precharge Time control: 3 */
#define NPRECHG_4              (0x0004)       /* FRAM Wait state Generator Precharge Time control: 4 */
#define NPRECHG_5              (0x0005)       /* FRAM Wait state Generator Precharge Time control: 5 */
#define NPRECHG_6              (0x0006)       /* FRAM Wait state Generator Precharge Time control: 6 */
#define NPRECHG_7              (0x0007)       /* FRAM Wait state Generator Precharge Time control: 7 */

#define NACCESS_0              (0x0000)       /* FRAM Wait state Generator Access Time control: 0 */
#define NACCESS_1              (0x0010)       /* FRAM Wait state Generator Access Time control: 1 */
#define NACCESS_2              (0x0020)       /* FRAM Wait state Generator Access Time control: 2 */
#define NACCESS_3              (0x0030)       /* FRAM Wait state Generator Access Time control: 3 */
#define NACCESS_4              (0x0040)       /* FRAM Wait state Generator Access Time control: 4 */
#define NACCESS_5              (0x0050)       /* FRAM Wait state Generator Access Time control: 5 */
#define NACCESS_6              (0x0060)       /* FRAM Wait state Generator Access Time control: 6 */
#define NACCESS_7              (0x0070)       /* FRAM Wait state Generator Access Time control: 7 */

/* GCCTL0 Control Bits */
#define BUSY                   (0x0001)       /* FRAM is currently busy programming */
//#define RESERVED            (0x0002)  /* RESERVED */
//#define RESERVED            (0x0004)  /* RESERVED */
#define ACCTEIE                (0x0008)       /* Enable NMI event if Access time error occurs */
#define ACCVIE                 (0x0010)       /* Enable NMI event if Access Violation occurs */
#define CBDIE                  (0x0020)       /* Enable NMI event if correctable bit error detected */
#define UBDIE                  (0x0040)       /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN               (0x0080)       /* Enable Power Up Clear on uncorrectable bit error */

/* GCCTL0 Control Bits */
#define BUSY_L                 (0x0001)       /* FRAM is currently busy programming */
//#define RESERVED            (0x0002)  /* RESERVED */
//#define RESERVED            (0x0004)  /* RESERVED */
#define ACCTEIE_L              (0x0008)       /* Enable NMI event if Access time error occurs */
#define ACCVIE_L               (0x0010)       /* Enable NMI event if Access Violation occurs */
#define CBDIE_L                (0x0020)       /* Enable NMI event if correctable bit error detected */
#define UBDIE_L                (0x0040)       /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN_L             (0x0080)       /* Enable Power Up Clear on uncorrectable bit error */

/* GCCTL0 Control Bits */
//#define RESERVED            (0x0002)  /* RESERVED */
//#define RESERVED            (0x0004)  /* RESERVED */

/* GCCTL1 Control Bits */
#define ACCVIFG                (0x0001)       /* Access Violation Interrupt Flag */
#define CBDIFG                 (0x0002)       /* FRAM correctable bit error flag */
#define UBDIFG                 (0x0004)       /* FRAM uncorrectable bit error flag */
#define ACCTEIFG               (0x0008)       /* Access time error flag */

/* GCCTL1 Control Bits */
#define ACCVIFG_L              (0x0001)       /* Access Violation Interrupt Flag */
#define CBDIFG_L               (0x0002)       /* FRAM correctable bit error flag */
#define UBDIFG_L               (0x0004)       /* FRAM uncorrectable bit error flag */
#define ACCTEIFG_L             (0x0008)       /* Access time error flag */

/* GCCTL1 Control Bits */

/* Legacy Definitions */
#define SBDIEN                 (0x0020)       /* Enable NMI event if single bit error detected */
#define DBDIEN                 (0x0040)       /* Enable NMI event if double bit error detected */
#define DBDRSTEN               (0x0080)       /* Enable Power Up Clear on double bit error */
#define SBDIFG                 (0x0002)       /* FRAM single bit error flag */
#define DBDIFG                 (0x0004)       /* FRAM double bit error flag */

#endif
/************************************************************
* Memory Protection Unit A
************************************************************/
#ifdef  __MSP430_HAS_MPU__           /* Definition to show that Module is available */

#define OFS_MPUCTL0            (0x0000)       /* MPU Control Register 0 */
#define OFS_MPUCTL0_L          OFS_MPUCTL0
#define OFS_MPUCTL0_H          OFS_MPUCTL0+1
#define OFS_MPUCTL1            (0x0002)       /* MPU Control Register 1 */
#define OFS_MPUCTL1_L          OFS_MPUCTL1
#define OFS_MPUCTL1_H          OFS_MPUCTL1+1
#define OFS_MPUSEGB2           (0x0004)       /* MPU Segmentation Border 2 Register */
#define OFS_MPUSEGB2_L         OFS_MPUSEGB2
#define OFS_MPUSEGB2_H         OFS_MPUSEGB2+1
#define OFS_MPUSEGB1           (0x0006)       /* MPU Segmentation Border 1 Register */
#define OFS_MPUSEGB1_L         OFS_MPUSEGB1
#define OFS_MPUSEGB1_H         OFS_MPUSEGB1+1
#define OFS_MPUSAM             (0x0008)       /* MPU Access Management Register */
#define OFS_MPUSAM_L           OFS_MPUSAM
#define OFS_MPUSAM_H           OFS_MPUSAM+1
#define OFS_MPUIPC0            (0x000A)       /* MPU IP Control 0 Register */
#define OFS_MPUIPC0_L          OFS_MPUIPC0
#define OFS_MPUIPC0_H          OFS_MPUIPC0+1
#define OFS_MPUIPSEGB2         (0x000C)       /* MPU IP Segment Border 2 Register */
#define OFS_MPUIPSEGB2_L       OFS_MPUIPSEGB2
#define OFS_MPUIPSEGB2_H       OFS_MPUIPSEGB2+1
#define OFS_MPUIPSEGB1         (0x000E)       /* MPU IP Segment Border 1 Register */
#define OFS_MPUIPSEGB1_L       OFS_MPUIPSEGB1
#define OFS_MPUIPSEGB1_H       OFS_MPUIPSEGB1+1

/* MPUCTL0 Control Bits */
#define MPUENA                 (0x0001)       /* MPU Enable */
#define MPULOCK                (0x0002)       /* MPU Lock */
#define MPUSEGIE               (0x0010)       /* MPU Enable NMI on Segment violation */

/* MPUCTL0 Control Bits */
#define MPUENA_L               (0x0001)       /* MPU Enable */
#define MPULOCK_L              (0x0002)       /* MPU Lock */
#define MPUSEGIE_L             (0x0010)       /* MPU Enable NMI on Segment violation */

/* MPUCTL0 Control Bits */
#define MPUPW                  (0xA500)       /* MPU Access Password */
#define MPUPW_H                (0xA5)         /* MPU Access Password */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG             (0x0001)       /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG             (0x0002)       /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG             (0x0004)       /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG             (0x0008)       /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG            (0x0010)       /* MPU IP Memory Segment violation interupt flag */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG_L           (0x0001)       /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG_L           (0x0002)       /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG_L           (0x0004)       /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG_L           (0x0008)       /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG_L          (0x0010)       /* MPU IP Memory Segment violation interupt flag */

/* MPUCTL1 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSAM Control Bits */
#define MPUSEG1RE              (0x0001)       /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE              (0x0002)       /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE              (0x0004)       /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS              (0x0008)       /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE              (0x0010)       /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE              (0x0020)       /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE              (0x0040)       /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS              (0x0080)       /* MPU Main memory Segment 2 Violation select */
#define MPUSEG3RE              (0x0100)       /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE              (0x0200)       /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE              (0x0400)       /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS              (0x0800)       /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE              (0x1000)       /* MPU Info memory Segment Read enable */
#define MPUSEGIWE              (0x2000)       /* MPU Info memory Segment Write enable */
#define MPUSEGIXE              (0x4000)       /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS              (0x8000)       /* MPU Info memory Segment Violation select */

/* MPUSAM Control Bits */
#define MPUSEG1RE_L            (0x0001)       /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE_L            (0x0002)       /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE_L            (0x0004)       /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS_L            (0x0008)       /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE_L            (0x0010)       /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE_L            (0x0020)       /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE_L            (0x0040)       /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS_L            (0x0080)       /* MPU Main memory Segment 2 Violation select */

/* MPUSAM Control Bits */
#define MPUSEG3RE_H            (0x0001)       /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE_H            (0x0002)       /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE_H            (0x0004)       /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS_H            (0x0008)       /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE_H            (0x0010)       /* MPU Info memory Segment Read enable */
#define MPUSEGIWE_H            (0x0020)       /* MPU Info memory Segment Write enable */
#define MPUSEGIXE_H            (0x0040)       /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS_H            (0x0080)       /* MPU Info memory Segment Violation select */

/* MPUIPC0 Control Bits */
#define MPUIPVS                (0x0020)       /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA               (0x0040)       /* MPU MPU IP Protection Enable */
#define MPUIPLOCK              (0x0080)       /* MPU IP Protection Lock */

/* MPUIPC0 Control Bits */
#define MPUIPVS_L              (0x0020)       /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA_L             (0x0040)       /* MPU MPU IP Protection Enable */
#define MPUIPLOCK_L            (0x0080)       /* MPU IP Protection Lock */

/* MPUIPC0 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

#endif

/************************************************************
* HARDWARE MULTIPLIER 32Bit
************************************************************/
#ifdef  __MSP430_HAS_MPY32__          /* Definition to show that Module is available */

#define OFS_MPY                (0x0000)       /* Multiply Unsigned/Operand 1 */
#define OFS_MPY_L              OFS_MPY
#define OFS_MPY_H              OFS_MPY+1
#define OFS_MPYS               (0x0002)       /* Multiply Signed/Operand 1 */
#define OFS_MPYS_L             OFS_MPYS
#define OFS_MPYS_H             OFS_MPYS+1
#define OFS_MAC                (0x0004)       /* Multiply Unsigned and Accumulate/Operand 1 */
#define OFS_MAC_L              OFS_MAC
#define OFS_MAC_H              OFS_MAC+1
#define OFS_MACS               (0x0006)       /* Multiply Signed and Accumulate/Operand 1 */
#define OFS_MACS_L             OFS_MACS
#define OFS_MACS_H             OFS_MACS+1
#define OFS_OP2                (0x0008)       /* Operand 2 */
#define OFS_OP2_L              OFS_OP2
#define OFS_OP2_H              OFS_OP2+1
#define OFS_RESLO              (0x000A)       /* Result Low Word */
#define OFS_RESLO_L            OFS_RESLO
#define OFS_RESLO_H            OFS_RESLO+1
#define OFS_RESHI              (0x000C)       /* Result High Word */
#define OFS_RESHI_L            OFS_RESHI
#define OFS_RESHI_H            OFS_RESHI+1
#define OFS_SUMEXT             (0x000E)       /* Sum Extend */
#define OFS_SUMEXT_L           OFS_SUMEXT
#define OFS_SUMEXT_H           OFS_SUMEXT+1
#define OFS_MPY32CTL0          (0x002C)
#define OFS_MPY32CTL0_L        OFS_MPY32CTL0
#define OFS_MPY32CTL0_H        OFS_MPY32CTL0+1

#define OFS_MPY32L             (0x0010)       /* 32-bit operand 1 - multiply - low word */
#define OFS_MPY32L_L           OFS_MPY32L
#define OFS_MPY32L_H           OFS_MPY32L+1
#define OFS_MPY32H             (0x0012)       /* 32-bit operand 1 - multiply - high word */
#define OFS_MPY32H_L           OFS_MPY32H
#define OFS_MPY32H_H           OFS_MPY32H+1
#define OFS_MPYS32L            (0x0014)       /* 32-bit operand 1 - signed multiply - low word */
#define OFS_MPYS32L_L          OFS_MPYS32L
#define OFS_MPYS32L_H          OFS_MPYS32L+1
#define OFS_MPYS32H            (0x0016)       /* 32-bit operand 1 - signed multiply - high word */
#define OFS_MPYS32H_L          OFS_MPYS32H
#define OFS_MPYS32H_H          OFS_MPYS32H+1
#define OFS_MAC32L             (0x0018)       /* 32-bit operand 1 - multiply accumulate - low word */
#define OFS_MAC32L_L           OFS_MAC32L
#define OFS_MAC32L_H           OFS_MAC32L+1
#define OFS_MAC32H             (0x001A)       /* 32-bit operand 1 - multiply accumulate - high word */
#define OFS_MAC32H_L           OFS_MAC32H
#define OFS_MAC32H_H           OFS_MAC32H+1
#define OFS_MACS32L            (0x001C)       /* 32-bit operand 1 - signed multiply accumulate - low word */
#define OFS_MACS32L_L          OFS_MACS32L
#define OFS_MACS32L_H          OFS_MACS32L+1
#define OFS_MACS32H            (0x001E)       /* 32-bit operand 1 - signed multiply accumulate - high word */
#define OFS_MACS32H_L          OFS_MACS32H
#define OFS_MACS32H_H          OFS_MACS32H+1
#define OFS_OP2L               (0x0020)       /* 32-bit operand 2 - low word */
#define OFS_OP2L_L             OFS_OP2L
#define OFS_OP2L_H             OFS_OP2L+1
#define OFS_OP2H               (0x0022)       /* 32-bit operand 2 - high word */
#define OFS_OP2H_L             OFS_OP2H
#define OFS_OP2H_H             OFS_OP2H+1
#define OFS_RES0               (0x0024)       /* 32x32-bit result 0 - least significant word */
#define OFS_RES0_L             OFS_RES0
#define OFS_RES0_H             OFS_RES0+1
#define OFS_RES1               (0x0026)       /* 32x32-bit result 1 */
#define OFS_RES1_L             OFS_RES1
#define OFS_RES1_H             OFS_RES1+1
#define OFS_RES2               (0x0028)       /* 32x32-bit result 2 */
#define OFS_RES2_L             OFS_RES2
#define OFS_RES2_H             OFS_RES2+1
#define OFS_RES3               (0x002A)       /* 32x32-bit result 3 - most significant word */
#define OFS_RES3_L             OFS_RES3
#define OFS_RES3_H             OFS_RES3+1
#define OFS_SUMEXT             (0x000E)
#define OFS_SUMEXT_L           OFS_SUMEXT
#define OFS_SUMEXT_H           OFS_SUMEXT+1
#define OFS_MPY32CTL0          (0x002C)       /* MPY32 Control Register 0 */
#define OFS_MPY32CTL0_L        OFS_MPY32CTL0
#define OFS_MPY32CTL0_H        OFS_MPY32CTL0+1

#define MPY_B                  MPY_L          /* Multiply Unsigned/Operand 1 (Byte Access) */
#define MPYS_B                 MPYS_L         /* Multiply Signed/Operand 1 (Byte Access) */
#define MAC_B                  MAC_L          /* Multiply Unsigned and Accumulate/Operand 1 (Byte Access) */
#define MACS_B                 MACS_L         /* Multiply Signed and Accumulate/Operand 1 (Byte Access) */
#define OP2_B                  OP2_L          /* Operand 2 (Byte Access) */
#define MPY32L_B               MPY32L_L       /* 32-bit operand 1 - multiply - low word (Byte Access) */
#define MPY32H_B               MPY32H_L       /* 32-bit operand 1 - multiply - high word (Byte Access) */
#define MPYS32L_B              MPYS32L_L      /* 32-bit operand 1 - signed multiply - low word (Byte Access) */
#define MPYS32H_B              MPYS32H_L      /* 32-bit operand 1 - signed multiply - high word (Byte Access) */
#define MAC32L_B               MAC32L_L       /* 32-bit operand 1 - multiply accumulate - low word (Byte Access) */
#define MAC32H_B               MAC32H_L       /* 32-bit operand 1 - multiply accumulate - high word (Byte Access) */
#define MACS32L_B              MACS32L_L      /* 32-bit operand 1 - signed multiply accumulate - low word (Byte Access) */
#define MACS32H_B              MACS32H_L      /* 32-bit operand 1 - signed multiply accumulate - high word (Byte Access) */
#define OP2L_B                 OP2L_L         /* 32-bit operand 2 - low word (Byte Access) */
#define OP2H_B                 OP2H_L         /* 32-bit operand 2 - high word (Byte Access) */

/* MPY32CTL0 Control Bits */
#define MPYC                   (0x0001)       /* Carry of the multiplier */
//#define RESERVED            (0x0002)  /* Reserved */
#define MPYFRAC                (0x0004)       /* Fractional mode */
#define MPYSAT                 (0x0008)       /* Saturation mode */
#define MPYM0                  (0x0010)       /* Multiplier mode Bit:0 */
#define MPYM1                  (0x0020)       /* Multiplier mode Bit:1 */
#define OP1_32                 (0x0040)       /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32                 (0x0080)       /* Bit-width of operand 2 0:16Bit / 1:32Bit */
#define MPYDLYWRTEN            (0x0100)       /* Delayed write enable */
#define MPYDLY32               (0x0200)       /* Delayed write mode */

/* MPY32CTL0 Control Bits */
#define MPYC_L                 (0x0001)       /* Carry of the multiplier */
//#define RESERVED            (0x0002)  /* Reserved */
#define MPYFRAC_L              (0x0004)       /* Fractional mode */
#define MPYSAT_L               (0x0008)       /* Saturation mode */
#define MPYM0_L                (0x0010)       /* Multiplier mode Bit:0 */
#define MPYM1_L                (0x0020)       /* Multiplier mode Bit:1 */
#define OP1_32_L               (0x0040)       /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32_L               (0x0080)       /* Bit-width of operand 2 0:16Bit / 1:32Bit */

/* MPY32CTL0 Control Bits */
//#define RESERVED            (0x0002)  /* Reserved */
#define MPYDLYWRTEN_H          (0x0001)       /* Delayed write enable */
#define MPYDLY32_H             (0x0002)       /* Delayed write mode */

#define MPYM_0                 (0x0000)       /* Multiplier mode: MPY */
#define MPYM_1                 (0x0010)       /* Multiplier mode: MPYS */
#define MPYM_2                 (0x0020)       /* Multiplier mode: MAC */
#define MPYM_3                 (0x0030)       /* Multiplier mode: MACS */
#define MPYM__MPY              (0x0000)       /* Multiplier mode: MPY */
#define MPYM__MPYS             (0x0010)       /* Multiplier mode: MPYS */
#define MPYM__MAC              (0x0020)       /* Multiplier mode: MAC */
#define MPYM__MACS             (0x0030)       /* Multiplier mode: MACS */

#endif

/************************************************************
* DIGITAL I/O Port1/2 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT1_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT2_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTA_R__        /* Definition to show that Module is available */

#define OFS_PAIN               (0x0000)       /* Port A Input */
#define OFS_PAIN_L             OFS_PAIN
#define OFS_PAIN_H             OFS_PAIN+1
#define OFS_PAOUT              (0x0002)       /* Port A Output */
#define OFS_PAOUT_L            OFS_PAOUT
#define OFS_PAOUT_H            OFS_PAOUT+1
#define OFS_PADIR              (0x0004)       /* Port A Direction */
#define OFS_PADIR_L            OFS_PADIR
#define OFS_PADIR_H            OFS_PADIR+1
#define OFS_PAREN              (0x0006)       /* Port A Resistor Enable */
#define OFS_PAREN_L            OFS_PAREN
#define OFS_PAREN_H            OFS_PAREN+1
#define OFS_PADS               (0x0008)       /* Port A Drive Strenght */
#define OFS_PADS_L             OFS_PADS
#define OFS_PADS_H             OFS_PADS+1
#define OFS_PASEL              (0x000A)       /* Port A Selection */
#define OFS_PASEL_L            OFS_PASEL
#define OFS_PASEL_H            OFS_PASEL+1
#define OFS_PASEL0             (0x000A)       /* Port A Selection 0 */
#define OFS_PASEL0_L           OFS_PASEL0
#define OFS_PASEL0_H           OFS_PASEL0+1
#define OFS_PASEL1             (0x000C)       /* Port A Selection 1 */
#define OFS_PASEL1_L           OFS_PASEL1
#define OFS_PASEL1_H           OFS_PASEL1+1
#define OFS_PASELC             (0x0016)       /* Port A Complement Selection */
#define OFS_PASELC_L           OFS_PASELC
#define OFS_PASELC_H           OFS_PASELC+1
#define OFS_PAIES              (0x0018)       /* Port A Interrupt Edge Select */
#define OFS_PAIES_L            OFS_PAIES
#define OFS_PAIES_H            OFS_PAIES+1
#define OFS_PAIE               (0x001A)       /* Port A Interrupt Enable */
#define OFS_PAIE_L             OFS_PAIE
#define OFS_PAIE_H             OFS_PAIE+1
#define OFS_PAIFG              (0x001C)       /* Port A Interrupt Flag */
#define OFS_PAIFG_L            OFS_PAIFG
#define OFS_PAIFG_H            OFS_PAIFG+1


#define OFS_P1IN               (0x0000)
#define OFS_P1OUT              (0x0002)
#define OFS_P1DIR              (0x0004)
#define OFS_P1REN              (0x0006)
#define OFS_P1DS               (0x0008)
#define OFS_P1SEL              (0x000A)
#define OFS_P1SEL0             (0x000A)
#define OFS_P1SEL1             (0x000C)
#define OFS_P1SELC             (0x0016)
#define OFS_P1IV               (0x000E)       /* Port 1 Interrupt Vector Word */
#define OFS_P1IES              (0x0018)
#define OFS_P1IE               (0x001A)
#define OFS_P1IFG              (0x001C)
#define OFS_P2IN               (0x0001)
#define OFS_P2OUT              (0x0003)
#define OFS_P2DIR              (0x0005)
#define OFS_P2REN              (0x0007)
#define OFS_P2DS               (0x0009)
#define OFS_P2SEL              (0x000B)
#define OFS_P2SEL0             (0x000B)
#define OFS_P2SEL1             (0x000D)
#define OFS_P2SELC             (0x0017)
#define OFS_P2IV               (0x001E)       /* Port 2 Interrupt Vector Word */
#define OFS_P2IES              (0x0019)
#define OFS_P2IE               (0x001B)
#define OFS_P2IFG              (0x001d)
#define P1IN                   (PAIN_L)       /* Port 1 Input */
#define P1OUT                  (PAOUT_L)      /* Port 1 Output */
#define P1DIR                  (PADIR_L)      /* Port 1 Direction */
#define P1REN                  (PAREN_L)      /* Port 1 Resistor Enable */
#define P1DS                   (PADS_L)       /* Port 1 Drive Strenght */
#define P1SEL                  (PASEL_L)      /* Port 1 Selection */
#define P1SEL0                 (PASEL0_L)     /* Port 1 Selection 0 */
#define P1SEL1                 (PASEL1_L)     /* Port 1 Selection 1 */
#define P1SELC                 (PASELC_L)     /* Port 1 Complement Selection */
#define P1IES                  (PAIES_L)      /* Port 1 Interrupt Edge Select */
#define P1IE                   (PAIE_L)       /* Port 1 Interrupt Enable */
#define P1IFG                  (PAIFG_L)      /* Port 1 Interrupt Flag */

//Definitions for P1IV
#define P1IV_NONE              (0x0000)       /* No Interrupt pending */
#define P1IV_P1IFG0            (0x0002)       /* P1IV P1IFG.0 */
#define P1IV_P1IFG1            (0x0004)       /* P1IV P1IFG.1 */
#define P1IV_P1IFG2            (0x0006)       /* P1IV P1IFG.2 */
#define P1IV_P1IFG3            (0x0008)       /* P1IV P1IFG.3 */
#define P1IV_P1IFG4            (0x000A)       /* P1IV P1IFG.4 */
#define P1IV_P1IFG5            (0x000C)       /* P1IV P1IFG.5 */
#define P1IV_P1IFG6            (0x000E)       /* P1IV P1IFG.6 */
#define P1IV_P1IFG7            (0x0010)       /* P1IV P1IFG.7 */

#define P2IN                   (PAIN_H)       /* Port 2 Input */
#define P2OUT                  (PAOUT_H)      /* Port 2 Output */
#define P2DIR                  (PADIR_H)      /* Port 2 Direction */
#define P2REN                  (PAREN_H)      /* Port 2 Resistor Enable */
#define P2DS                   (PADS_H)       /* Port 2 Drive Strenght */
#define P2SEL                  (PASEL_H)      /* Port 2 Selection */
#define P2SEL0                 (PASEL0_H)     /* Port 2 Selection 0 */
#define P2SEL1                 (PASEL1_H)     /* Port 2 Selection 1 */
#define P2SELC                 (PASELC_H)     /* Port 2 Complement Selection */
#define P2IES                  (PAIES_H)      /* Port 2 Interrupt Edge Select */
#define P2IE                   (PAIE_H)       /* Port 2 Interrupt Enable */
#define P2IFG                  (PAIFG_H)      /* Port 2 Interrupt Flag */

//Definitions for P2IV
#define P2IV_NONE              (0x0000)       /* No Interrupt pending */
#define P2IV_P2IFG0            (0x0002)       /* P2IV P2IFG.0 */
#define P2IV_P2IFG1            (0x0004)       /* P2IV P2IFG.1 */
#define P2IV_P2IFG2            (0x0006)       /* P2IV P2IFG.2 */
#define P2IV_P2IFG3            (0x0008)       /* P2IV P2IFG.3 */
#define P2IV_P2IFG4            (0x000A)       /* P2IV P2IFG.4 */
#define P2IV_P2IFG5            (0x000C)       /* P2IV P2IFG.5 */
#define P2IV_P2IFG6            (0x000E)       /* P2IV P2IFG.6 */
#define P2IV_P2IFG7            (0x0010)       /* P2IV P2IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port3/4 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT3_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT4_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTB_R__        /* Definition to show that Module is available */

#define OFS_PBIN               (0x0000)       /* Port B Input */
#define OFS_PBIN_L             OFS_PBIN
#define OFS_PBIN_H             OFS_PBIN+1
#define OFS_PBOUT              (0x0002)       /* Port B Output */
#define OFS_PBOUT_L            OFS_PBOUT
#define OFS_PBOUT_H            OFS_PBOUT+1
#define OFS_PBDIR              (0x0004)       /* Port B Direction */
#define OFS_PBDIR_L            OFS_PBDIR
#define OFS_PBDIR_H            OFS_PBDIR+1
#define OFS_PBREN              (0x0006)       /* Port B Resistor Enable */
#define OFS_PBREN_L            OFS_PBREN
#define OFS_PBREN_H            OFS_PBREN+1
#define OFS_PBDS               (0x0008)       /* Port B Drive Strenght */
#define OFS_PBDS_L             OFS_PBDS
#define OFS_PBDS_H             OFS_PBDS+1
#define OFS_PBSEL              (0x000A)       /* Port B Selection */
#define OFS_PBSEL_L            OFS_PBSEL
#define OFS_PBSEL_H            OFS_PBSEL+1
#define OFS_PBSEL0             (0x000A)       /* Port B Selection 0 */
#define OFS_PBSEL0_L           OFS_PBSEL0
#define OFS_PBSEL0_H           OFS_PBSEL0+1
#define OFS_PBSEL1             (0x000C)       /* Port B Selection 1 */
#define OFS_PBSEL1_L           OFS_PBSEL1
#define OFS_PBSEL1_H           OFS_PBSEL1+1
#define OFS_PBSELC             (0x0016)       /* Port B Complement Selection */
#define OFS_PBSELC_L           OFS_PBSELC
#define OFS_PBSELC_H           OFS_PBSELC+1
#define OFS_PBIES              (0x0018)       /* Port B Interrupt Edge Select */
#define OFS_PBIES_L            OFS_PBIES
#define OFS_PBIES_H            OFS_PBIES+1
#define OFS_PBIE               (0x001A)       /* Port B Interrupt Enable */
#define OFS_PBIE_L             OFS_PBIE
#define OFS_PBIE_H             OFS_PBIE+1
#define OFS_PBIFG              (0x001C)       /* Port B Interrupt Flag */
#define OFS_PBIFG_L            OFS_PBIFG
#define OFS_PBIFG_H            OFS_PBIFG+1

#define OFS_P3IN               (0x0000)
#define OFS_P3OUT              (0x0002)
#define OFS_P3DIR              (0x0004)
#define OFS_P3REN              (0x0006)
#define OFS_P3DS               (0x0008)
#define OFS_P3SEL              (0x000A)
#define OFS_P3SEL0             (0x000A)
#define OFS_P3SEL1             (0x000C)
#define OFS_P3SELC             (0x0016)
#define OFS_P3IV               (0x000E)       /* Port 3 Interrupt Vector Word */
#define OFS_P3IES              (0x0018)
#define OFS_P3IE               (0x001A)
#define OFS_P3IFG              (0x001C)
#define OFS_P4IN               (0x0001)
#define OFS_P4OUT              (0x0003)
#define OFS_P4DIR              (0x0005)
#define OFS_P4REN              (0x0007)
#define OFS_P4DS               (0x0009)
#define OFS_P4SEL              (0x000B)
#define OFS_P4SEL0             (0x000B)
#define OFS_P4SEL1             (0x000D)
#define OFS_P4SELC             (0x0017)
#define OFS_P4IV               (0x001E)       /* Port 4 Interrupt Vector Word */
#define OFS_P4IES              (0x0019)
#define OFS_P4IE               (0x001B)
#define OFS_P4IFG              (0x001d)
#define P3IN                   (PBIN_L)       /* Port 3 Input */
#define P3OUT                  (PBOUT_L)      /* Port 3 Output */
#define P3DIR                  (PBDIR_L)      /* Port 3 Direction */
#define P3REN                  (PBREN_L)      /* Port 3 Resistor Enable */
#define P3DS                   (PBDS_L)       /* Port 3 Drive Strenght */
#define P3SEL                  (PBSEL_L)      /* Port 3 Selection */
#define P3SEL0                 (PBSEL0_L)     /* Port 3 Selection 0 */
#define P3SEL1                 (PBSEL1_L)     /* Port 3 Selection 1 */
#define P3SELC                 (PBSELC_L)     /* Port 3 Complement Selection */
#define P3IES                  (PBIES_L)      /* Port 3 Interrupt Edge Select */
#define P3IE                   (PBIE_L)       /* Port 3 Interrupt Enable */
#define P3IFG                  (PBIFG_L)      /* Port 3 Interrupt Flag */

//Definitions for P3IV
#define P3IV_NONE              (0x0000)       /* No Interrupt pending */
#define P3IV_P3IFG0            (0x0002)       /* P3IV P3IFG.0 */
#define P3IV_P3IFG1            (0x0004)       /* P3IV P3IFG.1 */
#define P3IV_P3IFG2            (0x0006)       /* P3IV P3IFG.2 */
#define P3IV_P3IFG3            (0x0008)       /* P3IV P3IFG.3 */
#define P3IV_P3IFG4            (0x000A)       /* P3IV P3IFG.4 */
#define P3IV_P3IFG5            (0x000C)       /* P3IV P3IFG.5 */
#define P3IV_P3IFG6            (0x000E)       /* P3IV P3IFG.6 */
#define P3IV_P3IFG7            (0x0010)       /* P3IV P3IFG.7 */

#define P4IN                   (PBIN_H)       /* Port 4 Input */
#define P4OUT                  (PBOUT_H)      /* Port 4 Output */
#define P4DIR                  (PBDIR_H)      /* Port 4 Direction */
#define P4REN                  (PBREN_H)      /* Port 4 Resistor Enable */
#define P4DS                   (PBDS_H)       /* Port 4 Drive Strenght */
#define P4SEL                  (PBSEL_H)      /* Port 4 Selection */
#define P4SEL0                 (PBSEL0_H)     /* Port 4 Selection 0 */
#define P4SEL1                 (PBSEL1_H)     /* Port 4 Selection 1 */
#define P4SELC                 (PBSELC_H)     /* Port 4 Complement Selection */
#define P4IES                  (PBIES_H)      /* Port 4 Interrupt Edge Select */
#define P4IE                   (PBIE_H)       /* Port 4 Interrupt Enable */
#define P4IFG                  (PBIFG_H)      /* Port 4 Interrupt Flag */

//Definitions for P4IV
#define P4IV_NONE              (0x0000)       /* No Interrupt pending */
#define P4IV_P4IFG0            (0x0002)       /* P4IV P4IFG.0 */
#define P4IV_P4IFG1            (0x0004)       /* P4IV P4IFG.1 */
#define P4IV_P4IFG2            (0x0006)       /* P4IV P4IFG.2 */
#define P4IV_P4IFG3            (0x0008)       /* P4IV P4IFG.3 */
#define P4IV_P4IFG4            (0x000A)       /* P4IV P4IFG.4 */
#define P4IV_P4IFG5            (0x000C)       /* P4IV P4IFG.5 */
#define P4IV_P4IFG6            (0x000E)       /* P4IV P4IFG.6 */
#define P4IV_P4IFG7            (0x0010)       /* P4IV P4IFG.7 */

#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port5/6 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT5_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT6_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTC_R__        /* Definition to show that Module is available */

#define OFS_PCIN               (0x0000)       /* Port C Input */
#define OFS_PCIN_L             OFS_PCIN
#define OFS_PCIN_H             OFS_PCIN+1
#define OFS_PCOUT              (0x0002)       /* Port C Output */
#define OFS_PCOUT_L            OFS_PCOUT
#define OFS_PCOUT_H            OFS_PCOUT+1
#define OFS_PCDIR              (0x0004)       /* Port C Direction */
#define OFS_PCDIR_L            OFS_PCDIR
#define OFS_PCDIR_H            OFS_PCDIR+1
#define OFS_PCREN              (0x0006)       /* Port C Resistor Enable */
#define OFS_PCREN_L            OFS_PCREN
#define OFS_PCREN_H            OFS_PCREN+1
#define OFS_PCDS               (0x0008)       /* Port C Drive Strenght */
#define OFS_PCDS_L             OFS_PCDS
#define OFS_PCDS_H             OFS_PCDS+1
#define OFS_PCSEL              (0x000A)       /* Port C Selection */
#define OFS_PCSEL_L            OFS_PCSEL
#define OFS_PCSEL_H            OFS_PCSEL+1
#define OFS_PCSEL0             (0x000A)       /* Port C Selection 0 */
#define OFS_PCSEL0_L           OFS_PCSEL0
#define OFS_PCSEL0_H           OFS_PCSEL0+1
#define OFS_PCSEL1             (0x000C)       /* Port C Selection 1 */
#define OFS_PCSEL1_L           OFS_PCSEL1
#define OFS_PCSEL1_H           OFS_PCSEL1+1
#define OFS_PCSELC             (0x0016)       /* Port C Complement Selection */
#define OFS_PCSELC_L           OFS_PCSELC
#define OFS_PCSELC_H           OFS_PCSELC+1
#define OFS_PCIES              (0x0018)       /* Port C Interrupt Edge Select */
#define OFS_PCIES_L            OFS_PCIES
#define OFS_PCIES_H            OFS_PCIES+1
#define OFS_PCIE               (0x001A)       /* Port C Interrupt Enable */
#define OFS_PCIE_L             OFS_PCIE
#define OFS_PCIE_H             OFS_PCIE+1
#define OFS_PCIFG              (0x001C)       /* Port C Interrupt Flag */
#define OFS_PCIFG_L            OFS_PCIFG
#define OFS_PCIFG_H            OFS_PCIFG+1

#define OFS_P5IN               (0x0000)
#define OFS_P5OUT              (0x0002)
#define OFS_P5DIR              (0x0004)
#define OFS_P5REN              (0x0006)
#define OFS_P5DS               (0x0008)
#define OFS_P5SEL              (0x000A)
#define OFS_P5SEL0             (0x000A)
#define OFS_P5SEL1             (0x000C)
#define OFS_P5SELC             (0x0016)
#define OFS_P5IV               (0x000E)       /* Port 5 Interrupt Vector Word */
#define OFS_P5IES              (0x0018)
#define OFS_P5IE               (0x001A)
#define OFS_P5IFG              (0x001C)
#define OFS_P6IN               (0x0001)
#define OFS_P6OUT              (0x0003)
#define OFS_P6DIR              (0x0005)
#define OFS_P6REN              (0x0007)
#define OFS_P6DS               (0x0009)
#define OFS_P6SEL              (0x000B)
#define OFS_P6SEL0             (0x000B)
#define OFS_P6SEL1             (0x000D)
#define OFS_P6SELC             (0x0017)
#define OFS_P6IV               (0x001E)       /* Port 6 Interrupt Vector Word */
#define OFS_P6IES              (0x0019)
#define OFS_P6IE               (0x001B)
#define OFS_P6IFG              (0x001d)
#define P5IN                   (PCIN_L)       /* Port 5 Input */
#define P5OUT                  (PCOUT_L)      /* Port 5 Output */
#define P5DIR                  (PCDIR_L)      /* Port 5 Direction */
#define P5REN                  (PCREN_L)      /* Port 5 Resistor Enable */
#define P5DS                   (PCDS_L)       /* Port 5 Drive Strenght */
#define P5SEL                  (PCSEL_L)      /* Port 5 Selection */
#define P5SEL0                 (PCSEL0_L)     /* Port 5 Selection 0 */
#define P5SEL1                 (PCSEL1_L)     /* Port 5 Selection 1 */
#define P5SELC                 (PCSELC_L)     /* Port 5 Complement Selection */
#define P5IES                  (PCIES_L)      /* Port 5 Interrupt Edge Select */
#define P5IE                   (PCIE_L)       /* Port 5 Interrupt Enable */
#define P5IFG                  (PCIFG_L)      /* Port 5 Interrupt Flag */

//Definitions for P5IV
#define P5IV_NONE              (0x0000)       /* No Interrupt pending */
#define P5IV_P5IFG0            (0x0002)       /* P5IV P5IFG.0 */
#define P5IV_P5IFG1            (0x0004)       /* P5IV P5IFG.1 */
#define P5IV_P5IFG2            (0x0006)       /* P5IV P5IFG.2 */
#define P5IV_P5IFG3            (0x0008)       /* P5IV P5IFG.3 */
#define P5IV_P5IFG4            (0x000A)       /* P5IV P5IFG.4 */
#define P5IV_P5IFG5            (0x000C)       /* P5IV P5IFG.5 */
#define P5IV_P5IFG6            (0x000E)       /* P5IV P5IFG.6 */
#define P5IV_P5IFG7            (0x0010)       /* P5IV P5IFG.7 */

#define P6IN                   (PCIN_H)       /* Port 6 Input */
#define P6OUT                  (PCOUT_H)      /* Port 6 Output */
#define P6DIR                  (PCDIR_H)      /* Port 6 Direction */
#define P6REN                  (PCREN_H)      /* Port 6 Resistor Enable */
#define P6DS                   (PCDS_H)       /* Port 6 Drive Strenght */
#define P6SEL                  (PCSEL_H)      /* Port 6 Selection */
#define P6SEL0                 (PCSEL0_H)     /* Port 6 Selection 0 */
#define P6SEL1                 (PCSEL1_H)     /* Port 6 Selection 1 */
#define P6SELC                 (PCSELC_H)     /* Port 6 Complement Selection */
#define P6IES                  (PCIES_H)      /* Port 6 Interrupt Edge Select */
#define P6IE                   (PCIE_H)       /* Port 6 Interrupt Enable */
#define P6IFG                  (PCIFG_H)      /* Port 6 Interrupt Flag */

//Definitions for P6IV
#define P6IV_NONE              (0x0000)       /* No Interrupt pending */
#define P6IV_P6IFG0            (0x0002)       /* P6IV P6IFG.0 */
#define P6IV_P6IFG1            (0x0004)       /* P6IV P6IFG.1 */
#define P6IV_P6IFG2            (0x0006)       /* P6IV P6IFG.2 */
#define P6IV_P6IFG3            (0x0008)       /* P6IV P6IFG.3 */
#define P6IV_P6IFG4            (0x000A)       /* P6IV P6IFG.4 */
#define P6IV_P6IFG5            (0x000C)       /* P6IV P6IFG.5 */
#define P6IV_P6IFG6            (0x000E)       /* P6IV P6IFG.6 */
#define P6IV_P6IFG7            (0x0010)       /* P6IV P6IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port7/8 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT7_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT8_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTD_R__        /* Definition to show that Module is available */

#define OFS_PDIN               (0x0000)       /* Port D Input */
#define OFS_PDIN_L             OFS_PDIN
#define OFS_PDIN_H             OFS_PDIN+1
#define OFS_PDOUT              (0x0002)       /* Port D Output */
#define OFS_PDOUT_L            OFS_PDOUT
#define OFS_PDOUT_H            OFS_PDOUT+1
#define OFS_PDDIR              (0x0004)       /* Port D Direction */
#define OFS_PDDIR_L            OFS_PDDIR
#define OFS_PDDIR_H            OFS_PDDIR+1
#define OFS_PDREN              (0x0006)       /* Port D Resistor Enable */
#define OFS_PDREN_L            OFS_PDREN
#define OFS_PDREN_H            OFS_PDREN+1
#define OFS_PDDS               (0x0008)       /* Port D Drive Strenght */
#define OFS_PDDS_L             OFS_PDDS
#define OFS_PDDS_H             OFS_PDDS+1
#define OFS_PDSEL              (0x000A)       /* Port D Selection */
#define OFS_PDSEL_L            OFS_PDSEL
#define OFS_PDSEL_H            OFS_PDSEL+1
#define OFS_PDSEL0             (0x000A)       /* Port D Selection 0 */
#define OFS_PDSEL0_L           OFS_PDSEL0
#define OFS_PDSEL0_H           OFS_PDSEL0+1
#define OFS_PDSEL1             (0x000C)       /* Port D Selection 1 */
#define OFS_PDSEL1_L           OFS_PDSEL1
#define OFS_PDSEL1_H           OFS_PDSEL1+1
#define OFS_PDSELC             (0x0016)       /* Port D Complement Selection */
#define OFS_PDSELC_L           OFS_PDSELC
#define OFS_PDSELC_H           OFS_PDSELC+1
#define OFS_PDIES              (0x0018)       /* Port D Interrupt Edge Select */
#define OFS_PDIES_L            OFS_PDIES
#define OFS_PDIES_H            OFS_PDIES+1
#define OFS_PDIE               (0x001A)       /* Port D Interrupt Enable */
#define OFS_PDIE_L             OFS_PDIE
#define OFS_PDIE_H             OFS_PDIE+1
#define OFS_PDIFG              (0x001C)       /* Port D Interrupt Flag */
#define OFS_PDIFG_L            OFS_PDIFG
#define OFS_PDIFG_H            OFS_PDIFG+1

#define OFS_P7IN               (0x0000)
#define OFS_P7OUT              (0x0002)
#define OFS_P7DIR              (0x0004)
#define OFS_P7REN              (0x0006)
#define OFS_P7DS               (0x0008)
#define OFS_P7SEL              (0x000A)
#define OFS_P7SEL0             (0x000A)
#define OFS_P7SEL1             (0x000C)
#define OFS_P7SELC             (0x0016)
#define OFS_P7IV               (0x000E)       /* Port 7 Interrupt Vector Word */
#define OFS_P7IES              (0x0018)
#define OFS_P7IE               (0x001A)
#define OFS_P7IFG              (0x001C)
#define OFS_P8IN               (0x0001)
#define OFS_P8OUT              (0x0003)
#define OFS_P8DIR              (0x0005)
#define OFS_P8REN              (0x0007)
#define OFS_P8DS               (0x0009)
#define OFS_P8SEL              (0x000B)
#define OFS_P8SEL0             (0x000B)
#define OFS_P8SEL1             (0x000D)
#define OFS_P8SELC             (0x0017)
#define OFS_P8IV               (0x001E)       /* Port 8 Interrupt Vector Word */
#define OFS_P8IES              (0x0019)
#define OFS_P8IE               (0x001B)
#define OFS_P8IFG              (0x001d)
#define P7IN                   (PDIN_L)       /* Port 7 Input */
#define P7OUT                  (PDOUT_L)      /* Port 7 Output */
#define P7DIR                  (PDDIR_L)      /* Port 7 Direction */
#define P7REN                  (PDREN_L)      /* Port 7 Resistor Enable */
#define P7DS                   (PDDS_L)       /* Port 7 Drive Strenght */
#define P7SEL                  (PDSEL_L)      /* Port 7 Selection */
#define P7SEL0                 (PDSEL0_L)     /* Port 7 Selection 0 */
#define P7SEL1                 (PDSEL1_L)     /* Port 7 Selection 1 */
#define P7SELC                 (PDSELC_L)     /* Port 7 Complement Selection */
#define P7IES                  (PDIES_L)      /* Port 7 Interrupt Edge Select */
#define P7IE                   (PDIE_L)       /* Port 7 Interrupt Enable */
#define P7IFG                  (PDIFG_L)      /* Port 7 Interrupt Flag */

//Definitions for P7IV
#define P7IV_NONE              (0x0000)       /* No Interrupt pending */
#define P7IV_P7IFG0            (0x0002)       /* P7IV P7IFG.0 */
#define P7IV_P7IFG1            (0x0004)       /* P7IV P7IFG.1 */
#define P7IV_P7IFG2            (0x0006)       /* P7IV P7IFG.2 */
#define P7IV_P7IFG3            (0x0008)       /* P7IV P7IFG.3 */
#define P7IV_P7IFG4            (0x000A)       /* P7IV P7IFG.4 */
#define P7IV_P7IFG5            (0x000C)       /* P7IV P7IFG.5 */
#define P7IV_P7IFG6            (0x000E)       /* P7IV P7IFG.6 */
#define P7IV_P7IFG7            (0x0010)       /* P7IV P7IFG.7 */

#define P8IN                   (PDIN_H)       /* Port 8 Input */
#define P8OUT                  (PDOUT_H)      /* Port 8 Output */
#define P8DIR                  (PDDIR_H)      /* Port 8 Direction */
#define P8REN                  (PDREN_H)      /* Port 8 Resistor Enable */
#define P8DS                   (PDDS_H)       /* Port 8 Drive Strenght */
#define P8SEL                  (PDSEL_H)      /* Port 8 Selection */
#define P8SEL0                 (PDSEL0_H)     /* Port 8 Selection 0 */
#define P8SEL1                 (PDSEL1_H)     /* Port 8 Selection 1 */
#define P8SELC                 (PDSELC_H)     /* Port 8 Complement Selection */
#define P8IES                  (PDIES_H)      /* Port 8 Interrupt Edge Select */
#define P8IE                   (PDIE_H)       /* Port 8 Interrupt Enable */
#define P8IFG                  (PDIFG_H)      /* Port 8 Interrupt Flag */

//Definitions for P8IV
#define P8IV_NONE              (0x0000)       /* No Interrupt pending */
#define P8IV_P8IFG0            (0x0002)       /* P8IV P8IFG.0 */
#define P8IV_P8IFG1            (0x0004)       /* P8IV P8IFG.1 */
#define P8IV_P8IFG2            (0x0006)       /* P8IV P8IFG.2 */
#define P8IV_P8IFG3            (0x0008)       /* P8IV P8IFG.3 */
#define P8IV_P8IFG4            (0x000A)       /* P8IV P8IFG.4 */
#define P8IV_P8IFG5            (0x000C)       /* P8IV P8IFG.5 */
#define P8IV_P8IFG6            (0x000E)       /* P8IV P8IFG.6 */
#define P8IV_P8IFG7            (0x0010)       /* P8IV P8IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port9/10 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT9_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT10_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTE_R__        /* Definition to show that Module is available */

#define OFS_PEIN               (0x0000)       /* Port E Input */
#define OFS_PEIN_L             OFS_PEIN
#define OFS_PEIN_H             OFS_PEIN+1
#define OFS_PEOUT              (0x0002)       /* Port E Output */
#define OFS_PEOUT_L            OFS_PEOUT
#define OFS_PEOUT_H            OFS_PEOUT+1
#define OFS_PEDIR              (0x0004)       /* Port E Direction */
#define OFS_PEDIR_L            OFS_PEDIR
#define OFS_PEDIR_H            OFS_PEDIR+1
#define OFS_PEREN              (0x0006)       /* Port E Resistor Enable */
#define OFS_PEREN_L            OFS_PEREN
#define OFS_PEREN_H            OFS_PEREN+1
#define OFS_PEDS               (0x0008)       /* Port E Drive Strenght */
#define OFS_PEDS_L             OFS_PEDS
#define OFS_PEDS_H             OFS_PEDS+1
#define OFS_PESEL              (0x000A)       /* Port E Selection */
#define OFS_PESEL_L            OFS_PESEL
#define OFS_PESEL_H            OFS_PESEL+1
#define OFS_PESEL0             (0x000A)       /* Port E Selection 0 */
#define OFS_PESEL0_L           OFS_PESEL0
#define OFS_PESEL0_H           OFS_PESEL0+1
#define OFS_PESEL1             (0x000C)       /* Port E Selection 1 */
#define OFS_PESEL1_L           OFS_PESEL1
#define OFS_PESEL1_H           OFS_PESEL1+1
#define OFS_PESELC             (0x0016)       /* Port E Complement Selection */
#define OFS_PESELC_L           OFS_PESELC
#define OFS_PESELC_H           OFS_PESELC+1
#define OFS_PEIES              (0x0018)       /* Port E Interrupt Edge Select */
#define OFS_PEIES_L            OFS_PEIES
#define OFS_PEIES_H            OFS_PEIES+1
#define OFS_PEIE               (0x001A)       /* Port E Interrupt Enable */
#define OFS_PEIE_L             OFS_PEIE
#define OFS_PEIE_H             OFS_PEIE+1
#define OFS_PEIFG              (0x001C)       /* Port E Interrupt Flag */
#define OFS_PEIFG_L            OFS_PEIFG
#define OFS_PEIFG_H            OFS_PEIFG+1


#define OFS_P9IN               (0x0000)
#define OFS_P9OUT              (0x0002)
#define OFS_P9DIR              (0x0004)
#define OFS_P9REN              (0x0006)
#define OFS_P9DS               (0x0008)
#define OFS_P9SEL              (0x000A)
#define OFS_P9SEL0             (0x000A)
#define OFS_P9SEL1             (0x000C)
#define OFS_P9SELC             (0x0016)
#define OFS_P9IV               (0x000E)       /* Port 9 Interrupt Vector Word */
#define OFS_P9IES              (0x0018)
#define OFS_P9IE               (0x001A)
#define OFS_P9IFG              (0x001C)
#define OFS_P10IN              (0x0001)
#define OFS_P10OUT             (0x0003)
#define OFS_P10DIR             (0x0005)
#define OFS_P10REN             (0x0007)
#define OFS_P10DS              (0x0009)
#define OFS_P10SEL             (0x000B)
#define OFS_P10SEL0            (0x000B)
#define OFS_P10SEL1            (0x000D)
#define OFS_P10SELC            (0x0017)
#define OFS_P10IV              (0x001E)       /* Port 10 Interrupt Vector Word */
#define OFS_P10IES             (0x0019)
#define OFS_P10IE              (0x001B)
#define OFS_P10IFG             (0x001d)
#define P9IN                   (PEIN_L)       /* Port 9 Input */
#define P9OUT                  (PEOUT_L)      /* Port 9 Output */
#define P9DIR                  (PEDIR_L)      /* Port 9 Direction */
#define P9REN                  (PEREN_L)      /* Port 9 Resistor Enable */
#define P9DS                   (PEDS_L)       /* Port 9 Drive Strenght */
#define P9SEL                  (PESEL_L)      /* Port 9 Selection */
#define P9SEL0                 (PESEL0_L)     /* Port 9 Selection 0 */
#define P9SEL1                 (PESEL1_L)     /* Port 9 Selection 1 */
#define P9SELC                 (PESELC_L)     /* Port 9 Complement Selection */
#define P9IES                  (PEIES_L)      /* Port 9 Interrupt Edge Select */
#define P9IE                   (PEIE_L)       /* Port 9 Interrupt Enable */
#define P9IFG                  (PEIFG_L)      /* Port 9 Interrupt Flag */

//Definitions for P9IV
#define P9IV_NONE              (0x0000)       /* No Interrupt pending */
#define P9IV_P9IFG0            (0x0002)       /* P9IV P9IFG.0 */
#define P9IV_P9IFG1            (0x0004)       /* P9IV P9IFG.1 */
#define P9IV_P9IFG2            (0x0006)       /* P9IV P9IFG.2 */
#define P9IV_P9IFG3            (0x0008)       /* P9IV P9IFG.3 */
#define P9IV_P9IFG4            (0x000A)       /* P9IV P9IFG.4 */
#define P9IV_P9IFG5            (0x000C)       /* P9IV P9IFG.5 */
#define P9IV_P9IFG6            (0x000E)       /* P9IV P9IFG.6 */
#define P9IV_P9IFG7            (0x0010)       /* P9IV P9IFG.7 */

#define P10IN                  (PEIN_H)       /* Port 10 Input */
#define P10OUT                 (PEOUT_H)      /* Port 10 Output */
#define P10DIR                 (PEDIR_H)      /* Port 10 Direction */
#define P10REN                 (PEREN_H)      /* Port 10 Resistor Enable */
#define P10DS                  (PEDS_H)       /* Port 10 Drive Strenght */
#define P10SEL                 (PESEL_H)      /* Port 10 Selection */
#define P10SEL0                (PESEL0_H)     /* Port 10 Selection 0 */
#define P10SEL1                (PESEL1_H)     /* Port 10 Selection 1 */
#define P10SELC                (PESELC_H)     /* Port 10 Complement Selection */
#define P10IES                 (PEIES_H)      /* Port 10 Interrupt Edge Select */
#define P10IE                  (PEIE_H)       /* Port 10 Interrupt Enable */
#define P10IFG                 (PEIFG_H)      /* Port 10 Interrupt Flag */

//Definitions for P10IV
#define P10IV_NONE             (0x0000)       /* No Interrupt pending */
#define P10IV_P10IFG0          (0x0002)       /* P10IV P10IFG.0 */
#define P10IV_P10IFG1          (0x0004)       /* P10IV P10IFG.1 */
#define P10IV_P10IFG2          (0x0006)       /* P10IV P10IFG.2 */
#define P10IV_P10IFG3          (0x0008)       /* P10IV P10IFG.3 */
#define P10IV_P10IFG4          (0x000A)       /* P10IV P10IFG.4 */
#define P10IV_P10IFG5          (0x000C)       /* P10IV P10IFG.5 */
#define P10IV_P10IFG6          (0x000E)       /* P10IV P10IFG.6 */
#define P10IV_P10IFG7          (0x0010)       /* P10IV P10IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port11 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT11_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTF_R__        /* Definition to show that Module is available */

#define OFS_PFIN               (0x0000)       /* Port F Input */
#define OFS_PFIN_L             OFS_PFIN
#define OFS_PFIN_H             OFS_PFIN+1
#define OFS_PFOUT              (0x0002)       /* Port F Output */
#define OFS_PFOUT_L            OFS_PFOUT
#define OFS_PFOUT_H            OFS_PFOUT+1
#define OFS_PFDIR              (0x0004)       /* Port F Direction */
#define OFS_PFDIR_L            OFS_PFDIR
#define OFS_PFDIR_H            OFS_PFDIR+1
#define OFS_PFREN              (0x0006)       /* Port F Resistor Enable */
#define OFS_PFREN_L            OFS_PFREN
#define OFS_PFREN_H            OFS_PFREN+1
#define OFS_PFDS               (0x0008)       /* Port F Drive Strenght */
#define OFS_PFDS_L             OFS_PFDS
#define OFS_PFDS_H             OFS_PFDS+1
#define OFS_PFSEL              (0x000A)       /* Port F Selection */
#define OFS_PFSEL_L            OFS_PFSEL
#define OFS_PFSEL_H            OFS_PFSEL+1
#define OFS_PFSEL0             (0x000A)       /* Port F Selection 0 */
#define OFS_PFSEL0_L           OFS_PFSEL0
#define OFS_PFSEL0_H           OFS_PFSEL0+1
#define OFS_PFSEL1             (0x000C)       /* Port F Selection 1 */
#define OFS_PFSEL1_L           OFS_PFSEL1
#define OFS_PFSEL1_H           OFS_PFSEL1+1
#define OFS_PFSELC             (0x0016)       /* Port F Complement Selection */
#define OFS_PFSELC_L           OFS_PFSELC
#define OFS_PFSELC_H           OFS_PFSELC+1
#define OFS_PFIES              (0x0018)       /* Port F Interrupt Edge Select */
#define OFS_PFIES_L            OFS_PFIES
#define OFS_PFIES_H            OFS_PFIES+1
#define OFS_PFIE               (0x001A)       /* Port F Interrupt Enable */
#define OFS_PFIE_L             OFS_PFIE
#define OFS_PFIE_H             OFS_PFIE+1
#define OFS_PFIFG              (0x001C)       /* Port F Interrupt Flag */
#define OFS_PFIFG_L            OFS_PFIFG
#define OFS_PFIFG_H            OFS_PFIFG+1


#define OFS_P11IN              (0x0000)
#define OFS_P11OUT             (0x0002)
#define OFS_P11DIR             (0x0004)
#define OFS_P11REN             (0x0006)
#define OFS_P11DS              (0x0008)
#define OFS_P11SEL             (0x000A)
#define OFS_P11SEL0            (0x000A)
#define OFS_P11SEL1            (0x000C)
#define OFS_P11SELC            (0x0016)
#define OFS_P11IV              (0x000E)       /* Port 11 Interrupt Vector Word */
#define OFS_P11IES             (0x0018)
#define OFS_P11IE              (0x001A)
#define OFS_P11IFG             (0x001C)
#define P11IN                  (PFIN_L)       /* Port 11 Input */
#define P11OUT                 (PFOUT_L)      /* Port 11 Output */
#define P11DIR                 (PFDIR_L)      /* Port 11 Direction */
#define P11REN                 (PFREN_L)      /* Port 11 Resistor Enable */
#define P11DS                  (PFDS_L)       /* Port 11 Drive Strenght */
#define P11SEL                 (PFSEL_L)      /* Port 11 Selection */
#define P11SEL0                (PFSEL0_L)     /* Port 11 Selection0 */
#define P11SEL1                (PFSEL1_L)     /* Port 11 Selection1 */

#define P11IES                 (PFIES_L)      /* Port 11 Interrupt Edge Select */
#define P11IE                  (PFIE_L)       /* Port 11 Interrupt Enable */
#define P11IFG                 (PFIFG_L)      /* Port 11 Interrupt Flag */

//Definitions for P11IV
#define P11IV_NONE             (0x0000)       /* No Interrupt pending */
#define P11IV_P11IFG0          (0x0002)       /* P11IV P11IFG.0 */
#define P11IV_P11IFG1          (0x0004)       /* P11IV P11IFG.1 */
#define P11IV_P11IFG2          (0x0006)       /* P11IV P11IFG.2 */
#define P11IV_P11IFG3          (0x0008)       /* P11IV P11IFG.3 */
#define P11IV_P11IFG4          (0x000A)       /* P11IV P11IFG.4 */
#define P11IV_P11IFG5          (0x000C)       /* P11IV P11IFG.5 */
#define P11IV_P11IFG6          (0x000E)       /* P11IV P11IFG.6 */
#define P11IV_P11IFG7          (0x0010)       /* P11IV P11IFG.7 */


#endif
#endif
/************************************************************
* DIGITAL I/O PortJ Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORTJ_R__        /* Definition to show that Module is available */

#define OFS_PJIN               (0x0000)       /* Port J Input */
#define OFS_PJIN_L             OFS_PJIN
#define OFS_PJIN_H             OFS_PJIN+1
#define OFS_PJOUT              (0x0002)       /* Port J Output */
#define OFS_PJOUT_L            OFS_PJOUT
#define OFS_PJOUT_H            OFS_PJOUT+1
#define OFS_PJDIR              (0x0004)       /* Port J Direction */
#define OFS_PJDIR_L            OFS_PJDIR
#define OFS_PJDIR_H            OFS_PJDIR+1
#define OFS_PJREN              (0x0006)       /* Port J Resistor Enable */
#define OFS_PJREN_L            OFS_PJREN
#define OFS_PJREN_H            OFS_PJREN+1
#define OFS_PJDS               (0x0008)       /* Port J Drive Strenght */
#define OFS_PJDS_L             OFS_PJDS
#define OFS_PJDS_H             OFS_PJDS+1
#define OFS_PJSEL              (0x000A)       /* Port J Selection */
#define OFS_PJSEL_L            OFS_PJSEL
#define OFS_PJSEL_H            OFS_PJSEL+1
#define OFS_PJSEL0             (0x000A)       /* Port J Selection 0 */
#define OFS_PJSEL0_L           OFS_PJSEL0
#define OFS_PJSEL0_H           OFS_PJSEL0+1
#define OFS_PJSEL1             (0x000C)       /* Port J Selection 1 */
#define OFS_PJSEL1_L           OFS_PJSEL1
#define OFS_PJSEL1_H           OFS_PJSEL1+1
#define OFS_PJSELC             (0x0016)       /* Port J Complement Selection */
#define OFS_PJSELC_L           OFS_PJSELC
#define OFS_PJSELC_H           OFS_PJSELC+1

#endif

/************************************************************
* PMM - Power Management System for FRAM
************************************************************/
#ifdef  __MSP430_HAS_PMM_FRAM__       /* Definition to show that Module is available */

#define OFS_PMMCTL0            (0x0000)       /* PMM Control 0 */
#define OFS_PMMCTL0_L          OFS_PMMCTL0
#define OFS_PMMCTL0_H          OFS_PMMCTL0+1
#define OFS_PMMCTL1            (0x0002)       /* PMM Control 1 */
#define OFS_PMMIFG             (0x000A)       /* PMM Interrupt Flag */
#define OFS_PMMIFG_L           OFS_PMMIFG
#define OFS_PMMIFG_H           OFS_PMMIFG+1
#define OFS_PM5CTL0            (0x0010)       /* PMM Power Mode 5 Control Register 0 */
#define OFS_PM5CTL0_L          OFS_PM5CTL0
#define OFS_PM5CTL0_H          OFS_PM5CTL0+1

#define PMMPW                  (0xA500)       /* PMM Register Write Password */
#define PMMPW_H                (0xA5)         /* PMM Register Write Password for high word access */

/* PMMCTL0 Control Bits */
#define PMMSWBOR               (0x0004)       /* PMM Software BOR */
#define PMMSWPOR               (0x0008)       /* PMM Software POR */
#define PMMREGOFF              (0x0010)       /* PMM Turn Regulator off */
#define SVSHE                  (0x0040)       /* SVS high side enable */
#define PMMLPRST               (0x0080)       /* PMM Low-Power Reset Enable */

/* PMMCTL0 Control Bits */
#define PMMSWBOR_L             (0x0004)       /* PMM Software BOR */
#define PMMSWPOR_L             (0x0008)       /* PMM Software POR */
#define PMMREGOFF_L            (0x0010)       /* PMM Turn Regulator off */
#define SVSHE_L                (0x0040)       /* SVS high side enable */
#define PMMLPRST_L             (0x0080)       /* PMM Low-Power Reset Enable */

/* PMMCTL0 Control Bits */

/* PMMCTL1 Control Bits */
#define PMMLPSVEN              (0x0002)       /* PMM Low-Power Supervision Enable */
#define PMMLPRNG0              (0x0004)       /* PMM Load Range Control overwrite for LPM2, LPM3 and LPM4 Bit: 0 */
#define PMMLPRNG1              (0x0008)       /* PMM Load Range Control overwrite for LPM2, LPM3 and LPM4 Bit: 1 */
#define PMMAMRNG0              (0x0010)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 0 */
#define PMMAMRNG1              (0x0020)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 1 */
#define PMMAMRNG2              (0x0040)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 2 */
#define PMMAMRNG3              (0x0080)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 3 */

/* PMMCTL1 Control Bits */
#define PMMLPSVEN_L            (0x0002)       /* PMM Low-Power Supervision Enable */
#define PMMLPRNG0_L            (0x0004)       /* PMM Load Range Control overwrite for LPM2, LPM3 and LPM4 Bit: 0 */
#define PMMLPRNG1_L            (0x0008)       /* PMM Load Range Control overwrite for LPM2, LPM3 and LPM4 Bit: 1 */
#define PMMAMRNG0_L            (0x0010)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 0 */
#define PMMAMRNG1_L            (0x0020)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 1 */
#define PMMAMRNG2_L            (0x0040)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 2 */
#define PMMAMRNG3_L            (0x0080)       /* Load Range Control overwrite for AM, LPM0 and LPM1 Bit: 3 */

/* PMMCTL1 Control Bits */
#define PMMCTL1KEY             (0xCC00)       /* PMM PMMCTL1 Register Write Password */

/* PMMIFG Control Bits */
#define PMMBORIFG              (0x0100)       /* PMM Software BOR interrupt flag */
#define PMMRSTIFG              (0x0200)       /* PMM RESET pin interrupt flag */
#define PMMPORIFG              (0x0400)       /* PMM Software POR interrupt flag */
#define SVSHIFG                (0x2000)       /* SVS low side interrupt flag */
#define PMMLPM5IFG             (0x8000)       /* LPM5 indication Flag */

/* PMMIFG Control Bits */

/* PMMIFG Control Bits */
#define PMMBORIFG_H            (0x0001)       /* PMM Software BOR interrupt flag */
#define PMMRSTIFG_H            (0x0002)       /* PMM RESET pin interrupt flag */
#define PMMPORIFG_H            (0x0004)       /* PMM Software POR interrupt flag */
#define SVSHIFG_H              (0x0020)       /* SVS low side interrupt flag */
#define PMMLPM5IFG_H           (0x0080)       /* LPM5 indication Flag */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5               (0x0001)       /* Lock I/O pin configuration upon entry/exit to/from LPM5 */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5_L             (0x0001)       /* Lock I/O pin configuration upon entry/exit to/from LPM5 */

/* PM5CTL0 Power Mode 5 Control Bits */


#endif
/************************************************************
* Port U
************************************************************/
#ifdef  __MSP430_HAS_PU__             /* Definition to show that Module is available */

/* ========================================================================= */
/* Port U and LDO Control Registers */
/* ========================================================================= */
#define OFS_LDOKEYPID          (0x0000)       /* LDO Controller peripheral ID and key register */
#define OFS_LDOKEYPID_L        OFS_LDOKEYPID
#define OFS_LDOKEYPID_H        OFS_LDOKEYPID+1
#define OFS_PUCTL              (0x0004)       /* PU Control register */
#define OFS_PUCTL_L            OFS_PUCTL
#define OFS_PUCTL_H            OFS_PUCTL+1
#define OFS_LDOPWRCTL          (0x0008)       /* LDO Power control register */
#define OFS_LDOPWRCTL_L        OFS_LDOPWRCTL
#define OFS_LDOPWRCTL_H        OFS_LDOPWRCTL+1

#define LDOKEY                 (0x9628)       /* LDO Control Register key */
#define LDOKEYID               LDOKEYPID      /* Legacy Definiton */

/* PUCTL Control Bits */
#define PUOUT0                 (0x0001)       /* PU - PU Output Signal Bit 0 */
#define PUOUT1                 (0x0002)       /* PU - PU Output Signal Bit 1 */
#define PUIN0                  (0x0004)       /* PU - PU0/DP Input Data */
#define PUIN1                  (0x0008)       /* PU - PU1/DM Input Data */
#define PUOPE                  (0x0020)       /* PU - Port Output Enable */
#define PUIPE                  (0x0100)       /* PU - PHY Single Ended Input enable */

/* PUCTL Control Bits */
#define PUOUT0_L               (0x0001)       /* PU - PU Output Signal Bit 0 */
#define PUOUT1_L               (0x0002)       /* PU - PU Output Signal Bit 1 */
#define PUIN0_L                (0x0004)       /* PU - PU0/DP Input Data */
#define PUIN1_L                (0x0008)       /* PU - PU1/DM Input Data */
#define PUOPE_L                (0x0020)       /* PU - Port Output Enable */

/* PUCTL Control Bits */
#define PUIPE_H                (0x0001)       /* PU - PHY Single Ended Input enable */

#define PUDIR                  (0x0020)       /* Legacy Definiton */
#define PSEIEN                 (0x0100)       /* Legacy Definiton */

/* LDOPWRCTL Control Bits */
#define LDOOVLIFG              (0x0001)       /* PU - LDOO Overload Interrupt Flag */
#define LDOONIFG               (0x0002)       /* PU - LDOI "Coming ON" Interrupt Flag */
#define LDOOFFIFG              (0x0004)       /* PU - LDOI "Going OFF" Interrupt Flag */
#define LDOBGVBV               (0x0008)       /* PU - LDO Bandgap and LDOI valid */
#define OVLAOFF                (0x0020)       /* PU - LDO overload auto off enable */
#define LDOOVLIE               (0x0100)       /* PU - Overload indication Interrupt Enable */
#define LDOONIE                (0x0200)       /* PU - LDOI "Coming ON" Interrupt Enable */
#define LDOOFFIE               (0x0400)       /* PU - LDOI "Going OFF" Interrupt Enable */
#define LDOOEN                 (0x0800)       /* PU - LDO Enable (3.3V) */

/* LDOPWRCTL Control Bits */
#define LDOOVLIFG_L            (0x0001)       /* PU - LDOO Overload Interrupt Flag */
#define LDOONIFG_L             (0x0002)       /* PU - LDOI "Coming ON" Interrupt Flag */
#define LDOOFFIFG_L            (0x0004)       /* PU - LDOI "Going OFF" Interrupt Flag */
#define LDOBGVBV_L             (0x0008)       /* PU - LDO Bandgap and LDOI valid */
#define OVLAOFF_L              (0x0020)       /* PU - LDO overload auto off enable */

/* LDOPWRCTL Control Bits */
#define LDOOVLIE_H             (0x0001)       /* PU - Overload indication Interrupt Enable */
#define LDOONIE_H              (0x0002)       /* PU - LDOI "Coming ON" Interrupt Enable */
#define LDOOFFIE_H             (0x0004)       /* PU - LDOI "Going OFF" Interrupt Enable */
#define LDOOEN_H               (0x0008)       /* PU - LDO Enable (3.3V) */

#define VUOVLIFG               (0x0001)       /* PU - Legacy Definiton: LDOO Overload Interrupt Flag */
#define VBONIFG                (0x0002)       /* PU - Legacy Definiton: LDOI "Coming ON" Interrupt Flag */
#define VBOFFIFG               (0x0004)       /* PU - Legacy Definiton: LDOI "Going OFF" Interrupt Flag */
#define VUOVLIE                (0x0100)       /* PU - Legacy Definiton: Overload indication Interrupt Enable */
#define VBONIE                 (0x0200)       /* PU - Legacy Definiton: LDOI "Coming ON" Interrupt Enable */
#define VBOFFIE                (0x0400)       /* PU - Legacy Definiton: LDOI "Going OFF" Interrupt Enable */

#endif
/************************************************************
* Shared Reference
************************************************************/
#ifdef  __MSP430_HAS_REF_A__           /* Definition to show that Module is available */

#define OFS_REFCTL0            (0x0000)       /* REF Shared Reference control register 0 */
#define OFS_REFCTL0_L          OFS_REFCTL0
#define OFS_REFCTL0_H          OFS_REFCTL0+1

/* REFCTL0 Control Bits */
#define REFON                  (0x0001)       /* REF Reference On */
#define REFOUT                 (0x0002)       /* REF Reference output Buffer On */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFTCOFF               (0x0008)       /* REF Temp.Sensor off */
#define REFVSEL0               (0x0010)       /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1               (0x0020)       /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT               (0x0040)       /* REF Reference generator one-time trigger */
#define REFBGOT                (0x0080)       /* REF Bandgap and bandgap buffer one-time trigger */
#define REFGENACT              (0x0100)       /* REF Reference generator active */
#define REFBGACT               (0x0200)       /* REF Reference bandgap active */
#define REFGENBUSY             (0x0400)       /* REF Reference generator busy */
#define BGMODE                 (0x0800)       /* REF Bandgap mode */
#define REFGENRDY              (0x1000)       /* REF Reference generator ready */
#define REFBGRDY               (0x2000)       /* REF Reference bandgap ready */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

/* REFCTL0 Control Bits */
#define REFON_L                (0x0001)       /* REF Reference On */
#define REFOUT_L               (0x0002)       /* REF Reference output Buffer On */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFTCOFF_L             (0x0008)       /* REF Temp.Sensor off */
#define REFVSEL0_L             (0x0010)       /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1_L             (0x0020)       /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT_L             (0x0040)       /* REF Reference generator one-time trigger */
#define REFBGOT_L              (0x0080)       /* REF Bandgap and bandgap buffer one-time trigger */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

/* REFCTL0 Control Bits */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFGENACT_H            (0x0001)       /* REF Reference generator active */
#define REFBGACT_H             (0x0002)       /* REF Reference bandgap active */
#define REFGENBUSY_H           (0x0004)       /* REF Reference generator busy */
#define BGMODE_H               (0x0008)       /* REF Bandgap mode */
#define REFGENRDY_H            (0x0010)       /* REF Reference generator ready */
#define REFBGRDY_H             (0x0020)       /* REF Reference bandgap ready */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

#define REFVSEL_0              (0x0000)       /* REF Reference Voltage Level Select 1.2V */
#define REFVSEL_1              (0x0010)       /* REF Reference Voltage Level Select 2.0V */
#define REFVSEL_2              (0x0020)       /* REF Reference Voltage Level Select 2.5V */
#define REFVSEL_3              (0x0030)       /* REF Reference Voltage Level Select 2.5V */

#endif
/************************************************************
* Real Time Clock
************************************************************/
#ifdef  __MSP430_HAS_RTC_B__          /* Definition to show that Module is available */

#define OFS_RTCCTL01           (0x0000)       /* Real Timer Control 0/1 */
#define OFS_RTCCTL01_L         OFS_RTCCTL01
#define OFS_RTCCTL01_H         OFS_RTCCTL01+1
#define OFS_RTCCTL23           (0x0002)       /* Real Timer Control 2/3 */
#define OFS_RTCCTL23_L         OFS_RTCCTL23
#define OFS_RTCCTL23_H         OFS_RTCCTL23+1
#define OFS_RTCPS0CTL          (0x0008)       /* Real Timer Prescale Timer 0 Control */
#define OFS_RTCPS0CTL_L        OFS_RTCPS0CTL
#define OFS_RTCPS0CTL_H        OFS_RTCPS0CTL+1
#define OFS_RTCPS1CTL          (0x000A)       /* Real Timer Prescale Timer 1 Control */
#define OFS_RTCPS1CTL_L        OFS_RTCPS1CTL
#define OFS_RTCPS1CTL_H        OFS_RTCPS1CTL+1
#define OFS_RTCPS              (0x000C)       /* Real Timer Prescale Timer Control */
#define OFS_RTCPS_L            OFS_RTCPS
#define OFS_RTCPS_H            OFS_RTCPS+1
#define OFS_RTCIV              (0x000E)       /* Real Time Clock Interrupt Vector */
#define OFS_RTCTIM0            (0x0010)       /* Real Time Clock Time 0 */
#define OFS_RTCTIM0_L          OFS_RTCTIM0
#define OFS_RTCTIM0_H          OFS_RTCTIM0+1
#define OFS_RTCTIM1            (0x0012)       /* Real Time Clock Time 1 */
#define OFS_RTCTIM1_L          OFS_RTCTIM1
#define OFS_RTCTIM1_H          OFS_RTCTIM1+1
#define OFS_RTCDATE            (0x0014)       /* Real Time Clock Date */
#define OFS_RTCDATE_L          OFS_RTCDATE
#define OFS_RTCDATE_H          OFS_RTCDATE+1
#define OFS_RTCYEAR            (0x0016)       /* Real Time Clock Year */
#define OFS_RTCYEAR_L          OFS_RTCYEAR
#define OFS_RTCYEAR_H          OFS_RTCYEAR+1
#define OFS_RTCAMINHR          (0x0018)       /* Real Time Clock Alarm Min/Hour */
#define OFS_RTCAMINHR_L        OFS_RTCAMINHR
#define OFS_RTCAMINHR_H        OFS_RTCAMINHR+1
#define OFS_RTCADOWDAY         (0x001A)       /* Real Time Clock Alarm day of week/day */
#define OFS_RTCADOWDAY_L       OFS_RTCADOWDAY
#define OFS_RTCADOWDAY_H       OFS_RTCADOWDAY+1
#define OFS_BIN2BCD            (0x001C)       /* Real Time Binary-to-BCD conversion register */
#define OFS_BCD2BIN            (0x001E)       /* Real Time BCD-to-binary conversion register */

#define RTCCTL0                RTCCTL01_L     /* Real Time Clock Control 0 */
#define RTCCTL1                RTCCTL01_H     /* Real Time Clock Control 1 */
#define RTCCTL2                RTCCTL23_L     /* Real Time Clock Control 2 */
#define RTCCTL3                RTCCTL23_H     /* Real Time Clock Control 3 */
#define RTCNT12                RTCTIM0
#define RTCNT34                RTCTIM1
#define RTCNT1                 RTCTIM0_L
#define RTCNT2                 RTCTIM0_H
#define RTCNT3                 RTCTIM1_L
#define RTCNT4                 RTCTIM1_H
#define RTCSEC                 RTCTIM0_L
#define RTCMIN                 RTCTIM0_H
#define RTCHOUR                RTCTIM1_L
#define RTCDOW                 RTCTIM1_H
#define RTCDAY                 RTCDATE_L
#define RTCMON                 RTCDATE_H
#define RTCYEARL               RTCYEAR_L
#define RTCYEARH               RTCYEAR_H
#define RT0PS                  RTCPS_L
#define RT1PS                  RTCPS_H
#define RTCAMIN                RTCAMINHR_L    /* Real Time Clock Alarm Min */
#define RTCAHOUR               RTCAMINHR_H    /* Real Time Clock Alarm Hour */
#define RTCADOW                RTCADOWDAY_L   /* Real Time Clock Alarm day of week */
#define RTCADAY                RTCADOWDAY_H   /* Real Time Clock Alarm day */

/* RTCCTL01 Control Bits */
#define RTCBCD                 (0x8000)       /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD                (0x4000)       /* RTC Hold */
//#define RESERVED            (0x2000)     /* RESERVED */
#define RTCRDY                 (0x1000)       /* RTC Ready */
//#define RESERVED            (0x0800)     /* RESERVED */
//#define RESERVED            (0x0400)     /* RESERVED */
#define RTCTEV1                (0x0200)       /* RTC Time Event 1 */
#define RTCTEV0                (0x0100)       /* RTC Time Event 0 */
#define RTCOFIE                (0x0080)       /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE               (0x0040)       /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE                 (0x0020)       /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE               (0x0010)       /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG               (0x0008)       /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG              (0x0004)       /* RTC Time Event Interrupt Flag */
#define RTCAIFG                (0x0002)       /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG              (0x0001)       /* RTC Ready Interrupt Flag */

/* RTCCTL01 Control Bits */
//#define RESERVED            (0x2000)     /* RESERVED */
//#define RESERVED            (0x0800)     /* RESERVED */
//#define RESERVED            (0x0400)     /* RESERVED */
#define RTCOFIE_L              (0x0080)       /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE_L             (0x0040)       /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE_L               (0x0020)       /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE_L             (0x0010)       /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG_L             (0x0008)       /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG_L            (0x0004)       /* RTC Time Event Interrupt Flag */
#define RTCAIFG_L              (0x0002)       /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG_L            (0x0001)       /* RTC Ready Interrupt Flag */

/* RTCCTL01 Control Bits */
#define RTCBCD_H               (0x0080)       /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD_H              (0x0040)       /* RTC Hold */
//#define RESERVED            (0x2000)     /* RESERVED */
#define RTCRDY_H               (0x0010)       /* RTC Ready */
//#define RESERVED            (0x0800)     /* RESERVED */
//#define RESERVED            (0x0400)     /* RESERVED */
#define RTCTEV1_H              (0x0002)       /* RTC Time Event 1 */
#define RTCTEV0_H              (0x0001)       /* RTC Time Event 0 */

#define RTCTEV_0               (0x0000)       /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV_1               (0x0100)       /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV_2               (0x0200)       /* RTC Time Event: 2 (12:00 changed) */
#define RTCTEV_3               (0x0300)       /* RTC Time Event: 3 (00:00 changed) */
#define RTCTEV__MIN            (0x0000)       /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV__HOUR           (0x0100)       /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV__0000           (0x0200)       /* RTC Time Event: 2 (00:00 changed) */
#define RTCTEV__1200           (0x0300)       /* RTC Time Event: 3 (12:00 changed) */

/* RTCCTL23 Control Bits */
#define RTCCALF1               (0x0200)       /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0               (0x0100)       /* RTC Calibration Frequency Bit 0 */
#define RTCCALS                (0x0080)       /* RTC Calibration Sign */
//#define Reserved          (0x0040)
#define RTCCAL5                (0x0020)       /* RTC Calibration Bit 5 */
#define RTCCAL4                (0x0010)       /* RTC Calibration Bit 4 */
#define RTCCAL3                (0x0008)       /* RTC Calibration Bit 3 */
#define RTCCAL2                (0x0004)       /* RTC Calibration Bit 2 */
#define RTCCAL1                (0x0002)       /* RTC Calibration Bit 1 */
#define RTCCAL0                (0x0001)       /* RTC Calibration Bit 0 */

/* RTCCTL23 Control Bits */
#define RTCCALS_L              (0x0080)       /* RTC Calibration Sign */
//#define Reserved          (0x0040)
#define RTCCAL5_L              (0x0020)       /* RTC Calibration Bit 5 */
#define RTCCAL4_L              (0x0010)       /* RTC Calibration Bit 4 */
#define RTCCAL3_L              (0x0008)       /* RTC Calibration Bit 3 */
#define RTCCAL2_L              (0x0004)       /* RTC Calibration Bit 2 */
#define RTCCAL1_L              (0x0002)       /* RTC Calibration Bit 1 */
#define RTCCAL0_L              (0x0001)       /* RTC Calibration Bit 0 */

/* RTCCTL23 Control Bits */
#define RTCCALF1_H             (0x0002)       /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0_H             (0x0001)       /* RTC Calibration Frequency Bit 0 */
//#define Reserved          (0x0040)

#define RTCCALF_0              (0x0000)       /* RTC Calibration Frequency: No Output */
#define RTCCALF_1              (0x0100)       /* RTC Calibration Frequency: 512 Hz */
#define RTCCALF_2              (0x0200)       /* RTC Calibration Frequency: 256 Hz */
#define RTCCALF_3              (0x0300)       /* RTC Calibration Frequency: 1 Hz */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT0IP2                 (0x0010)       /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1                 (0x0008)       /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0                 (0x0004)       /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE                (0x0002)       /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG               (0x0001)       /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT0IP2_L               (0x0010)       /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1_L               (0x0008)       /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0_L               (0x0004)       /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE_L              (0x0002)       /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG_L             (0x0001)       /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)

#define RT0IP_0                (0x0000)       /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP_1                (0x0004)       /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP_2                (0x0008)       /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP_3                (0x000C)       /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP_4                (0x0010)       /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP_5                (0x0014)       /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP_6                (0x0018)       /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP_7                (0x001C)       /* RTC Prescale Timer 0 Interrupt Interval /256 */

#define RT0IP__2               (0x0000)       /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP__4               (0x0004)       /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP__8               (0x0008)       /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP__16              (0x000C)       /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP__32              (0x0010)       /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP__64              (0x0014)       /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP__128             (0x0018)       /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP__256             (0x001C)       /* RTC Prescale Timer 0 Interrupt Interval /256 */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT1IP2                 (0x0010)       /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1                 (0x0008)       /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0                 (0x0004)       /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE                (0x0002)       /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG               (0x0001)       /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT1IP2_L               (0x0010)       /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1_L               (0x0008)       /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0_L               (0x0004)       /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE_L              (0x0002)       /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG_L             (0x0001)       /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)

#define RT1IP_0                (0x0000)       /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP_1                (0x0004)       /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP_2                (0x0008)       /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP_3                (0x000C)       /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP_4                (0x0010)       /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP_5                (0x0014)       /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP_6                (0x0018)       /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP_7                (0x001C)       /* RTC Prescale Timer 1 Interrupt Interval /256 */

#define RT1IP__2               (0x0000)       /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP__4               (0x0004)       /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP__8               (0x0008)       /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP__16              (0x000C)       /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP__32              (0x0010)       /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP__64              (0x0014)       /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP__128             (0x0018)       /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP__256             (0x001C)       /* RTC Prescale Timer 1 Interrupt Interval /256 */

/* RTC Definitions */
#define RTCIV_NONE             (0x0000)       /* No Interrupt pending */
#define RTCIV_RTCRDYIFG        (0x0002)       /* RTC ready: RTCRDYIFG */
#define RTCIV_RTCTEVIFG        (0x0004)       /* RTC interval timer: RTCTEVIFG */
#define RTCIV_RTCAIFG          (0x0006)       /* RTC user alarm: RTCAIFG */
#define RTCIV_RT0PSIFG         (0x0008)       /* RTC prescaler 0: RT0PSIFG */
#define RTCIV_RT1PSIFG         (0x000A)       /* RTC prescaler 1: RT1PSIFG */
#define RTCIV_RTCOFIFG         (0x000C)       /* RTC Oscillator fault */

/* Legacy Definitions */
#define RTC_NONE               (0x0000)       /* No Interrupt pending */
#define RTC_RTCRDYIFG          (0x0002)       /* RTC ready: RTCRDYIFG */
#define RTC_RTCTEVIFG          (0x0004)       /* RTC interval timer: RTCTEVIFG */
#define RTC_RTCAIFG            (0x0006)       /* RTC user alarm: RTCAIFG */
#define RTC_RT0PSIFG           (0x0008)       /* RTC prescaler 0: RT0PSIFG */
#define RTC_RT1PSIFG           (0x000A)       /* RTC prescaler 1: RT1PSIFG */
#define RTC_RTCOFIFG           (0x000C)       /* RTC Oscillator fault */

#endif
/************************************************************
* SFR - Special Function Register Module
************************************************************/
#ifdef  __MSP430_HAS_SFR__            /* Definition to show that Module is available */

#define OFS_SFRIE1             (0x0000)       /* Interrupt Enable 1 */
#define OFS_SFRIE1_L           OFS_SFRIE1
#define OFS_SFRIE1_H           OFS_SFRIE1+1

/* SFRIE1 Control Bits */
#define WDTIE                  (0x0001)       /* WDT Interrupt Enable */
#define OFIE                   (0x0002)       /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE                  (0x0008)       /* Vacant Memory Interrupt Enable */
#define NMIIE                  (0x0010)       /* NMI Interrupt Enable */
//#define ACCVIE                 (0x0020)       /* Flash Access Violation Interrupt Enable */
#define JMBINIE                (0x0040)       /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE               (0x0080)       /* JTAG Mail Box output Interrupt Enable */

#define WDTIE_L                (0x0001)       /* WDT Interrupt Enable */
#define OFIE_L                 (0x0002)       /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE_L                (0x0008)       /* Vacant Memory Interrupt Enable */
#define NMIIE_L                (0x0010)       /* NMI Interrupt Enable */
//#define ACCVIE_L               (0x0020)       /* Flash Access Violation Interrupt Enable */
#define JMBINIE_L              (0x0040)       /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE_L             (0x0080)       /* JTAG Mail Box output Interrupt Enable */

//#define Reserved          (0x0004)

#define OFS_SFRIFG1            (0x0002)       /* Interrupt Flag 1 */
#define OFS_SFRIFG1_L          OFS_SFRIFG1
#define OFS_SFRIFG1_H          OFS_SFRIFG1+1
/* SFRIFG1 Control Bits */
#define WDTIFG                 (0x0001)       /* WDT Interrupt Flag */
#define OFIFG                  (0x0002)       /* Osc Fault Flag */
//#define Reserved          (0x0004)
#define VMAIFG                 (0x0008)       /* Vacant Memory Interrupt Flag */
#define NMIIFG                 (0x0010)       /* NMI Interrupt Flag */
//#define Reserved          (0x0020)
#define JMBINIFG               (0x0040)       /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG              (0x0080)       /* JTAG Mail Box output Interrupt Flag */

#define WDTIFG_L               (0x0001)       /* WDT Interrupt Flag */
#define OFIFG_L                (0x0002)       /* Osc Fault Flag */
//#define Reserved          (0x0004)
#define VMAIFG_L               (0x0008)       /* Vacant Memory Interrupt Flag */
#define NMIIFG_L               (0x0010)       /* NMI Interrupt Flag */
//#define Reserved          (0x0020)
#define JMBINIFG_L             (0x0040)       /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG_L            (0x0080)       /* JTAG Mail Box output Interrupt Flag */

//#define Reserved          (0x0004)
//#define Reserved          (0x0020)

#define OFS_SFRRPCR            (0x0004)       /* RESET Pin Control Register */
#define OFS_SFRRPCR_L          OFS_SFRRPCR
#define OFS_SFRRPCR_H          OFS_SFRRPCR+1
/* SFRRPCR Control Bits */
#define SYSNMI                 (0x0001)       /* NMI select */
#define SYSNMIIES              (0x0002)       /* NMI edge select */
#define SYSRSTUP               (0x0004)       /* RESET Pin pull down/up select */
#define SYSRSTRE               (0x0008)       /* RESET Pin Resistor enable */

#define SYSNMI_L               (0x0001)       /* NMI select */
#define SYSNMIIES_L            (0x0002)       /* NMI edge select */
#define SYSRSTUP_L             (0x0004)       /* RESET Pin pull down/up select */
#define SYSRSTRE_L             (0x0008)       /* RESET Pin Resistor enable */

#endif
/************************************************************
* SYS - System Module
************************************************************/
#ifdef  __MSP430_HAS_SYS__            /* Definition to show that Module is available */

#define OFS_SYSCTL             (0x0000)       /* System control */
#define OFS_SYSCTL_L           OFS_SYSCTL
#define OFS_SYSCTL_H           OFS_SYSCTL+1
#define OFS_SYSBSLC            (0x0002)       /* Boot strap configuration area */
#define OFS_SYSBSLC_L          OFS_SYSBSLC
#define OFS_SYSBSLC_H          OFS_SYSBSLC+1
#define OFS_SYSJMBC            (0x0006)       /* JTAG mailbox control */
#define OFS_SYSJMBC_L          OFS_SYSJMBC
#define OFS_SYSJMBC_H          OFS_SYSJMBC+1
#define OFS_SYSJMBI0           (0x0008)       /* JTAG mailbox input 0 */
#define OFS_SYSJMBI0_L         OFS_SYSJMBI0
#define OFS_SYSJMBI0_H         OFS_SYSJMBI0+1
#define OFS_SYSJMBI1           (0x000A)       /* JTAG mailbox input 1 */
#define OFS_SYSJMBI1_L         OFS_SYSJMBI1
#define OFS_SYSJMBI1_H         OFS_SYSJMBI1+1
#define OFS_SYSJMBO0           (0x000C)       /* JTAG mailbox output 0 */
#define OFS_SYSJMBO0_L         OFS_SYSJMBO0
#define OFS_SYSJMBO0_H         OFS_SYSJMBO0+1
#define OFS_SYSJMBO1           (0x000E)       /* JTAG mailbox output 1 */
#define OFS_SYSJMBO1_L         OFS_SYSJMBO1
#define OFS_SYSJMBO1_H         OFS_SYSJMBO1+1

#define OFS_SYSBERRIV          (0x0018)       /* Bus Error vector generator */
#define OFS_SYSBERRIV_L        OFS_SYSBERRIV
#define OFS_SYSBERRIV_H        OFS_SYSBERRIV+1
#define OFS_SYSUNIV            (0x001A)       /* User NMI vector generator */
#define OFS_SYSUNIV_L          OFS_SYSUNIV
#define OFS_SYSUNIV_H          OFS_SYSUNIV+1
#define OFS_SYSSNIV            (0x001C)       /* System NMI vector generator */
#define OFS_SYSSNIV_L          OFS_SYSSNIV
#define OFS_SYSSNIV_H          OFS_SYSSNIV+1
#define OFS_SYSRSTIV           (0x001E)       /* Reset vector generator */
#define OFS_SYSRSTIV_L         OFS_SYSRSTIV
#define OFS_SYSRSTIV_H         OFS_SYSRSTIV+1

/* SYSCTL Control Bits */
#define SYSRIVECT              (0x0001)       /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002)  /* SYS - Reserved */
#define SYSPMMPE               (0x0004)       /* SYS - PMM access protect */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
#define SYSBSLIND              (0x0010)       /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN             (0x0020)       /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

/* SYSCTL Control Bits */
#define SYSRIVECT_L            (0x0001)       /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002)  /* SYS - Reserved */
#define SYSPMMPE_L             (0x0004)       /* SYS - PMM access protect */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
#define SYSBSLIND_L            (0x0010)       /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN_L           (0x0020)       /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

/* SYSCTL Control Bits */
//#define RESERVED            (0x0002)  /* SYS - Reserved */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

/* SYSBSLC Control Bits */
#define SYSBSLSIZE0            (0x0001)       /* SYS - BSL Protection Size 0 */
#define SYSBSLSIZE1            (0x0002)       /* SYS - BSL Protection Size 1 */
#define SYSBSLR                (0x0004)       /* SYS - RAM assigned to BSL */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
//#define RESERVED            (0x0010)  /* SYS - Reserved */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
#define SYSBSLOFF              (0x4000)       /* SYS - BSL Memeory disabled */
#define SYSBSLPE               (0x8000)       /* SYS - BSL Memory protection enabled */

/* SYSBSLC Control Bits */
#define SYSBSLSIZE0_L          (0x0001)       /* SYS - BSL Protection Size 0 */
#define SYSBSLSIZE1_L          (0x0002)       /* SYS - BSL Protection Size 1 */
#define SYSBSLR_L              (0x0004)       /* SYS - RAM assigned to BSL */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
//#define RESERVED            (0x0010)  /* SYS - Reserved */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */

/* SYSBSLC Control Bits */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
//#define RESERVED            (0x0010)  /* SYS - Reserved */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
#define SYSBSLOFF_H            (0x0040)       /* SYS - BSL Memeory disabled */
#define SYSBSLPE_H             (0x0080)       /* SYS - BSL Memory protection enabled */

/* SYSJMBC Control Bits */
#define JMBIN0FG               (0x0001)       /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG               (0x0002)       /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG              (0x0004)       /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG              (0x0008)       /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE                (0x0010)       /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
#define JMBCLR0OFF             (0x0040)       /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF             (0x0080)       /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

/* SYSJMBC Control Bits */
#define JMBIN0FG_L             (0x0001)       /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG_L             (0x0002)       /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG_L            (0x0004)       /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG_L            (0x0008)       /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE_L              (0x0010)       /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
#define JMBCLR0OFF_L           (0x0040)       /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF_L           (0x0080)       /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

/* SYSJMBC Control Bits */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

#endif
/************************************************************
* Timerx_A7
************************************************************/
#ifdef  __MSP430_HAS_TxA7__            /* Definition to show that Module is available */

#define OFS_TAxCTL             (0x0000)       /* Timerx_A7 Control */
#define OFS_TAxCCTL0           (0x0002)       /* Timerx_A7 Capture/Compare Control 0 */
#define OFS_TAxCCTL1           (0x0004)       /* Timerx_A7 Capture/Compare Control 1 */
#define OFS_TAxCCTL2           (0x0006)       /* Timerx_A7 Capture/Compare Control 2 */
#define OFS_TAxCCTL3           (0x0008)       /* Timerx_A7 Capture/Compare Control 3 */
#define OFS_TAxCCTL4           (0x000A)       /* Timerx_A7 Capture/Compare Control 4 */
#define OFS_TAxCCTL5           (0x000C)       /* Timerx_A7 Capture/Compare Control 5 */
#define OFS_TAxCCTL6           (0x000E)       /* Timerx_A7 Capture/Compare Control 6 */
#define OFS_TAxR               (0x0010)       /* Timerx_A7 */
#define OFS_TAxCCR0            (0x0012)       /* Timerx_A7 Capture/Compare 0 */
#define OFS_TAxCCR1            (0x0014)       /* Timerx_A7 Capture/Compare 1 */
#define OFS_TAxCCR2            (0x0016)       /* Timerx_A7 Capture/Compare 2 */
#define OFS_TAxCCR3            (0x0018)       /* Timerx_A7 Capture/Compare 3 */
#define OFS_TAxCCR4            (0x001A)       /* Timerx_A7 Capture/Compare 4 */
#define OFS_TAxCCR5            (0x001C)       /* Timerx_A7 Capture/Compare 5 */
#define OFS_TAxCCR6            (0x001E)       /* Timerx_A7 Capture/Compare 6 */
#define OFS_TAxIV              (0x002E)       /* Timerx_A7 Interrupt Vector Word */
#define OFS_TAxEX0             (0x0020)       /* Timerx_A7 Expansion Register 0 */

/* Bits are already defined within the Timer0_Ax */

/* TAxIV Definitions */
#define TAxIV_NONE             (0x0000)       /* No Interrupt pending */
#define TAxIV_TA1CCR1          (0x0002)       /* TAxCCR1_CCIFG */
#define TAxIV_TA1CCR2          (0x0004)       /* TAxCCR2_CCIFG */
#define TAxIV_TA1CCR3          (0x0006)       /* TAxCCR3_CCIFG */
#define TAxIV_TA1CCR4          (0x0008)       /* TAxCCR4_CCIFG */
#define TAxIV_TA1CCR5          (0x000A)       /* TAxCCR5_CCIFG */
#define TAxIV_TA1CCR6          (0x000C)       /* TAxCCR6_CCIFG */
#define TAxIV_TAxIFG           (0x000E)       /* TAxIFG */

/* TAxCTL Control Bits */
#define TASSEL1                (0x0200)       /* Timer A clock source select 1 */
#define TASSEL0                (0x0100)       /* Timer A clock source select 0 */
#define ID1                    (0x0080)       /* Timer A clock input divider 1 */
#define ID0                    (0x0040)       /* Timer A clock input divider 0 */
#define MC1                    (0x0020)       /* Timer A mode control 1 */
#define MC0                    (0x0010)       /* Timer A mode control 0 */
#define TACLR                  (0x0004)       /* Timer A counter clear */
#define TAIE                   (0x0002)       /* Timer A counter interrupt enable */
#define TAIFG                  (0x0001)       /* Timer A counter interrupt flag */

#define MC_0                   (0*0x10u)      /* Timer A mode control: 0 - Stop */
#define MC_1                   (1*0x10u)      /* Timer A mode control: 1 - Up to CCR0 */
#define MC_2                   (2*0x10u)      /* Timer A mode control: 2 - Continuous up */
#define MC_3                   (3*0x10u)      /* Timer A mode control: 3 - Up/Down */
#define ID_0                   (0*0x40u)      /* Timer A input divider: 0 - /1 */
#define ID_1                   (1*0x40u)      /* Timer A input divider: 1 - /2 */
#define ID_2                   (2*0x40u)      /* Timer A input divider: 2 - /4 */
#define ID_3                   (3*0x40u)      /* Timer A input divider: 3 - /8 */
#define TASSEL_0               (0*0x100u)     /* Timer A clock source select: 0 - TACLK */
#define TASSEL_1               (1*0x100u)     /* Timer A clock source select: 1 - ACLK  */
#define TASSEL_2               (2*0x100u)     /* Timer A clock source select: 2 - SMCLK */
#define TASSEL_3               (3*0x100u)     /* Timer A clock source select: 3 - INCLK */
#define MC__STOP               (0*0x10u)      /* Timer A mode control: 0 - Stop */
#define MC__UP                 (1*0x10u)      /* Timer A mode control: 1 - Up to CCR0 */
#define MC__CONTINUOUS         (2*0x10u)      /* Timer A mode control: 2 - Continuous up */
#define MC__CONTINOUS          (2*0x10u)      /* Legacy define */
#define MC__UPDOWN             (3*0x10u)      /* Timer A mode control: 3 - Up/Down */
#define ID__1                  (0*0x40u)      /* Timer A input divider: 0 - /1 */
#define ID__2                  (1*0x40u)      /* Timer A input divider: 1 - /2 */
#define ID__4                  (2*0x40u)      /* Timer A input divider: 2 - /4 */
#define ID__8                  (3*0x40u)      /* Timer A input divider: 3 - /8 */
#define TASSEL__TACLK          (0*0x100u)     /* Timer A clock source select: 0 - TACLK */
#define TASSEL__ACLK           (1*0x100u)     /* Timer A clock source select: 1 - ACLK  */
#define TASSEL__SMCLK          (2*0x100u)     /* Timer A clock source select: 2 - SMCLK */
#define TASSEL__INCLK          (3*0x100u)     /* Timer A clock source select: 3 - INCLK */

/* TAxCCTLx Control Bits */
#define CM1                    (0x8000)       /* Capture mode 1 */
#define CM0                    (0x4000)       /* Capture mode 0 */
#define CCIS1                  (0x2000)       /* Capture input select 1 */
#define CCIS0                  (0x1000)       /* Capture input select 0 */
#define SCS                    (0x0800)       /* Capture sychronize */
#define SCCI                   (0x0400)       /* Latched capture signal (read) */
#define CAP                    (0x0100)       /* Capture mode: 1 /Compare mode : 0 */
#define OUTMOD2                (0x0080)       /* Output mode 2 */
#define OUTMOD1                (0x0040)       /* Output mode 1 */
#define OUTMOD0                (0x0020)       /* Output mode 0 */
#define CCIE                   (0x0010)       /* Capture/compare interrupt enable */
#define CCI                    (0x0008)       /* Capture input signal (read) */
#define OUT                    (0x0004)       /* PWM Output signal if output mode 0 */
#define COV                    (0x0002)       /* Capture/compare overflow flag */
#define CCIFG                  (0x0001)       /* Capture/compare interrupt flag */

#define OUTMOD_0               (0*0x20u)      /* PWM output mode: 0 - output only */
#define OUTMOD_1               (1*0x20u)      /* PWM output mode: 1 - set */
#define OUTMOD_2               (2*0x20u)      /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3               (3*0x20u)      /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4               (4*0x20u)      /* PWM output mode: 4 - toggle */
#define OUTMOD_5               (5*0x20u)      /* PWM output mode: 5 - Reset */
#define OUTMOD_6               (6*0x20u)      /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7               (7*0x20u)      /* PWM output mode: 7 - PWM reset/set */
#define CCIS_0                 (0*0x1000u)    /* Capture input select: 0 - CCIxA */
#define CCIS_1                 (1*0x1000u)    /* Capture input select: 1 - CCIxB */
#define CCIS_2                 (2*0x1000u)    /* Capture input select: 2 - GND */
#define CCIS_3                 (3*0x1000u)    /* Capture input select: 3 - Vcc */
#define CM_0                   (0*0x4000u)    /* Capture mode: 0 - disabled */
#define CM_1                   (1*0x4000u)    /* Capture mode: 1 - pos. edge */
#define CM_2                   (2*0x4000u)    /* Capture mode: 1 - neg. edge */
#define CM_3                   (3*0x4000u)    /* Capture mode: 1 - both edges */

/* TAxEX0 Control Bits */
#define TAIDEX0                (0x0001)       /* Timer A Input divider expansion Bit: 0 */
#define TAIDEX1                (0x0002)       /* Timer A Input divider expansion Bit: 1 */
#define TAIDEX2                (0x0004)       /* Timer A Input divider expansion Bit: 2 */

#define TAIDEX_0               (0*0x0001u)    /* Timer A Input divider expansion : /1 */
#define TAIDEX_1               (1*0x0001u)    /* Timer A Input divider expansion : /2 */
#define TAIDEX_2               (2*0x0001u)    /* Timer A Input divider expansion : /3 */
#define TAIDEX_3               (3*0x0001u)    /* Timer A Input divider expansion : /4 */
#define TAIDEX_4               (4*0x0001u)    /* Timer A Input divider expansion : /5 */
#define TAIDEX_5               (5*0x0001u)    /* Timer A Input divider expansion : /6 */
#define TAIDEX_6               (6*0x0001u)    /* Timer A Input divider expansion : /7 */
#define TAIDEX_7               (7*0x0001u)    /* Timer A Input divider expansion : /8 */

#endif
/************************************************************
* Timerx_B7
************************************************************/
#ifdef  __MSP430_HAS_TxB7__            /* Definition to show that Module is available */

#define OFS_TBxCTL             (0x0000)       /* Timerx_B7 Control */
#define OFS_TBxCCTL0           (0x0002)       /* Timerx_B7 Capture/Compare Control 0 */
#define OFS_TBxCCTL1           (0x0004)       /* Timerx_B7 Capture/Compare Control 1 */
#define OFS_TBxCCTL2           (0x0006)       /* Timerx_B7 Capture/Compare Control 2 */
#define OFS_TBxCCTL3           (0x0008)       /* Timerx_B7 Capture/Compare Control 3 */
#define OFS_TBxCCTL4           (0x000A)       /* Timerx_B7 Capture/Compare Control 4 */
#define OFS_TBxCCTL5           (0x000C)       /* Timerx_B7 Capture/Compare Control 5 */
#define OFS_TBxCCTL6           (0x000E)       /* Timerx_B7 Capture/Compare Control 6 */
#define OFS_TBxR               (0x0010)       /* Timerx_B7 */
#define OFS_TBxCCR0            (0x0012)       /* Timerx_B7 Capture/Compare 0 */
#define OFS_TBxCCR1            (0x0014)       /* Timerx_B7 Capture/Compare 1 */
#define OFS_TBxCCR2            (0x0016)       /* Timerx_B7 Capture/Compare 2 */
#define OFS_TBxCCR3            (0x0018)       /* Timerx_B7 Capture/Compare 3 */
#define OFS_TBxCCR4            (0x001A)       /* Timerx_B7 Capture/Compare 4 */
#define OFS_TBxCCR5            (0x001C)       /* Timerx_B7 Capture/Compare 5 */
#define OFS_TBxCCR6            (0x001E)       /* Timerx_B7 Capture/Compare 6 */
#define OFS_TBxIV              (0x002E)       /* Timerx_B7 Interrupt Vector Word */
#define OFS_TBxEX0             (0x0020)       /* Timerx_B7 Expansion Register 0 */

/* Bits are already defined within the Timer0_Ax */

/* TBxIV Definitions */
#define TBxIV_NONE             (0x0000)       /* No Interrupt pending */
#define TBxIV_TB1CCR1          (0x0002)       /* TBxCCR1_CCIFG */
#define TBxIV_TB1CCR2          (0x0004)       /* TBxCCR2_CCIFG */
#define TBxIV_TB1CCR3          (0x0006)       /* TBxCCR3_CCIFG */
#define TBxIV_TB1CCR4          (0x0008)       /* TBxCCR4_CCIFG */
#define TBxIV_TB1CCR5          (0x000A)       /* TBxCCR5_CCIFG */
#define TBxIV_TB1CCR6          (0x000C)       /* TBxCCR6_CCIFG */
#define TBxIV_TBxIFG           (0x000E)       /* TBxIFG */

/* TBxCTL Control Bits */
#define TBCLGRP1               (0x4000)       /* Timer_B7 Compare latch load group 1 */
#define TBCLGRP0               (0x2000)       /* Timer_B7 Compare latch load group 0 */
#define CNTL1                  (0x1000)       /* Counter lenght 1 */
#define CNTL0                  (0x0800)       /* Counter lenght 0 */
#define TBSSEL1                (0x0200)       /* Clock source 1 */
#define TBSSEL0                (0x0100)       /* Clock source 0 */
#define TBCLR                  (0x0004)       /* Timer_B7 counter clear */
#define TBIE                   (0x0002)       /* Timer_B7 interrupt enable */
#define TBIFG                  (0x0001)       /* Timer_B7 interrupt flag */

#define SHR1                   (0x4000)       /* Timer_B7 Compare latch load group 1 */
#define SHR0                   (0x2000)       /* Timer_B7 Compare latch load group 0 */

#define TBSSEL_0               (0*0x0100u)    /* Clock Source: TBCLK */
#define TBSSEL_1               (1*0x0100u)    /* Clock Source: ACLK  */
#define TBSSEL_2               (2*0x0100u)    /* Clock Source: SMCLK */
#define TBSSEL_3               (3*0x0100u)    /* Clock Source: INCLK */
#define CNTL_0                 (0*0x0800u)    /* Counter lenght: 16 bit */
#define CNTL_1                 (1*0x0800u)    /* Counter lenght: 12 bit */
#define CNTL_2                 (2*0x0800u)    /* Counter lenght: 10 bit */
#define CNTL_3                 (3*0x0800u)    /* Counter lenght:  8 bit */
#define SHR_0                  (0*0x2000u)    /* Timer_B7 Group: 0 - individually */
#define SHR_1                  (1*0x2000u)    /* Timer_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define SHR_2                  (2*0x2000u)    /* Timer_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define SHR_3                  (3*0x2000u)    /* Timer_B7 Group: 3 - 1 group (all) */
#define TBCLGRP_0              (0*0x2000u)    /* Timer_B7 Group: 0 - individually */
#define TBCLGRP_1              (1*0x2000u)    /* Timer_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define TBCLGRP_2              (2*0x2000u)    /* Timer_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define TBCLGRP_3              (3*0x2000u)    /* Timer_B7 Group: 3 - 1 group (all) */
#define TBSSEL__TACLK          (0*0x0100u)    /* Timer_B7 clock source select: 0 - TACLK */
#define TBSSEL__ACLK           (1*0x0100u)    /* Timer_B7 clock source select: 1 - ACLK  */
#define TBSSEL__SMCLK          (2*0x0100u)    /* Timer_B7 clock source select: 2 - SMCLK */
#define TBSSEL__INCLK          (3*0x0100u)    /* Timer_B7 clock source select: 3 - INCLK */
#define CNTL__16               (0*0x0800u)    /* Counter lenght: 16 bit */
#define CNTL__12               (1*0x0800u)    /* Counter lenght: 12 bit */
#define CNTL__10               (2*0x0800u)    /* Counter lenght: 10 bit */
#define CNTL__8                (3*0x0800u)    /* Counter lenght:  8 bit */

/* Additional Timer B Control Register bits are defined in Timer A */
/* TBxCCTLx Control Bits */
#define CLLD1                  (0x0400)       /* Compare latch load source 1 */
#define CLLD0                  (0x0200)       /* Compare latch load source 0 */

#define SLSHR1                 (0x0400)       /* Compare latch load source 1 */
#define SLSHR0                 (0x0200)       /* Compare latch load source 0 */

#define SLSHR_0                (0*0x0200u)    /* Compare latch load sourec : 0 - immediate */
#define SLSHR_1                (1*0x0200u)    /* Compare latch load sourec : 1 - TBR counts to 0 */
#define SLSHR_2                (2*0x0200u)    /* Compare latch load sourec : 2 - up/down */
#define SLSHR_3                (3*0x0200u)    /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

#define CLLD_0                 (0*0x0200u)    /* Compare latch load sourec : 0 - immediate */
#define CLLD_1                 (1*0x0200u)    /* Compare latch load sourec : 1 - TBR counts to 0 */
#define CLLD_2                 (2*0x0200u)    /* Compare latch load sourec : 2 - up/down */
#define CLLD_3                 (3*0x0200u)    /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

/* TBxEX0 Control Bits */
#define TBIDEX0                (0x0001)       /* Timer_B7 Input divider expansion Bit: 0 */
#define TBIDEX1                (0x0002)       /* Timer_B7 Input divider expansion Bit: 1 */
#define TBIDEX2                (0x0004)       /* Timer_B7 Input divider expansion Bit: 2 */

#define TBIDEX_0               (0*0x0001u)    /* Timer_B7 Input divider expansion : /1 */
#define TBIDEX_1               (1*0x0001u)    /* Timer_B7 Input divider expansion : /2 */
#define TBIDEX_2               (2*0x0001u)    /* Timer_B7 Input divider expansion : /3 */
#define TBIDEX_3               (3*0x0001u)    /* Timer_B7 Input divider expansion : /4 */
#define TBIDEX_4               (4*0x0001u)    /* Timer_B7 Input divider expansion : /5 */
#define TBIDEX_5               (5*0x0001u)    /* Timer_B7 Input divider expansion : /6 */
#define TBIDEX_6               (6*0x0001u)    /* Timer_B7 Input divider expansion : /7 */
#define TBIDEX_7               (7*0x0001u)    /* Timer_B7 Input divider expansion : /8 */
#define TBIDEX__1              (0*0x0001u)    /* Timer_B7 Input divider expansion : /1 */
#define TBIDEX__2              (1*0x0001u)    /* Timer_B7 Input divider expansion : /2 */
#define TBIDEX__3              (2*0x0001u)    /* Timer_B7 Input divider expansion : /3 */
#define TBIDEX__4              (3*0x0001u)    /* Timer_B7 Input divider expansion : /4 */
#define TBIDEX__5              (4*0x0001u)    /* Timer_B7 Input divider expansion : /5 */
#define TBIDEX__6              (5*0x0001u)    /* Timer_B7 Input divider expansion : /6 */
#define TBIDEX__7              (6*0x0001u)    /* Timer_B7 Input divider expansion : /7 */
#define TBIDEX__8              (7*0x0001u)    /* Timer_B7 Input divider expansion : /8 */


#define ID1                    (0x0080u)      /* Timer A clock input divider 1 */
#define ID0                    (0x0040u)      /* Timer A clock input divider 0 */
#define MC1                    (0x0020u)      /* Timer A mode control 1 */
#define MC0                    (0x0010u)      /* Timer A mode control 0 */
#define MC__STOP               (0*0x10u)      /* Timer A mode control: 0 - Stop */
#define MC__UP                 (1*0x10u)      /* Timer A mode control: 1 - Up to CCR0 */
#define MC__CONTINUOUS         (2*0x10u)      /* Timer A mode control: 2 - Continuous up */
#define MC__CONTINOUS          (2*0x10u)      /* Legacy define */
#define MC__UPDOWN             (3*0x10u)      /* Timer A mode control: 3 - Up/Down */
#define CM1                    (0x8000u)      /* Capture mode 1 */
#define CM0                    (0x4000u)      /* Capture mode 0 */
#define MC_0                   (0*0x10u)      /* Timer A mode control: 0 - Stop */
#define MC_1                   (1*0x10u)      /* Timer A mode control: 1 - Up to CCR0 */
#define MC_2                   (2*0x10u)      /* Timer A mode control: 2 - Continuous up */
#define MC_3                   (3*0x10u)      /* Timer A mode control: 3 - Up/Down */
#define CAP                    (0x0100)       /* Capture mode: 1 /Compare mode : 0 */
#define CCIE                   (0x0010)       /* Capture/compare interrupt enable */
#define CCIFG                  (0x0001)       /* Capture/compare interrupt flag */
#define CCIS_0                 (0*0x1000u)
#define CCIS_1                 (1*0x1000u)
#define CCIS_2                 (2*0x1000u)
#define CCIS_3                 (3*0x1000u)
#define CM_0                   (0*0x4000u)    /* Capture mode: 0 - disabled */
#define CM_1                   (1*0x4000u)    /* Capture mode: 1 - pos. edge */
#define CM_2                   (2*0x4000u)    /* Capture mode: 1 - neg. edge */
#define CM_3                   (3*0x4000u)    /* Capture mode: 1 - both edges */
#define OUT                    (0x0004)       /* PWM Output signal if output mode 0 */
#define OUTMOD_0               (0*0x20u)      /* PWM output mode: 0 - output only */
#define OUTMOD_1               (1*0x20u)      /* PWM output mode: 1 - set */
#define OUTMOD_2               (2*0x20u)      /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3               (3*0x20u)      /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4               (4*0x20u)      /* PWM output mode: 4 - toggle */
#define OUTMOD_5               (5*0x20u)      /* PWM output mode: 5 - Reset */
#define OUTMOD_6               (6*0x20u)      /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7               (7*0x20u)      /* PWM output mode: 7 - PWM reset/set */
#define SCCI                   (0x0400)       /* Latched capture signal (read) */
#define SCS                    (0x0800)       /* Capture sychronize */
#define CCI                    (0x0008)       /* Capture input signal (read) */
#define ID__1                  (0*0x40u)      /* Timer A input divider: 0 - /1 */
#define ID__2                  (1*0x40u)      /* Timer A input divider: 1 - /2 */
#define ID__4                  (2*0x40u)      /* Timer A input divider: 2 - /4 */
#define ID__8                  (3*0x40u)      /* Timer A input divider: 3 - /8 */
#define ID_0                   (0*0x40u)      /* Timer A input divider: 0 - /1 */
#define ID_1                   (1*0x40u)      /* Timer A input divider: 1 - /2 */
#define ID_2                   (2*0x40u)      /* Timer A input divider: 2 - /4 */
#define ID_3                   (3*0x40u)      /* Timer A input divider: 3 - /8 */

#endif
/************************************************************
* USCI Ax
************************************************************/
#ifdef  __MSP430_HAS_EUSCI_Ax__      /* Definition to show that Module is available */

#define OFS_UCAxCTLW0          (0x0000)       /* USCI Ax Control Word Register 0 */
#define OFS_UCAxCTLW0_L        OFS_UCAxCTLW0
#define OFS_UCAxCTLW0_H        OFS_UCAxCTLW0+1
#define OFS_UCAxCTL0           (0x0001)
#define OFS_UCAxCTL1           (0x0000)
#define UCAxCTL1               UCAxCTLW0_L    /* USCI Ax Control Register 1 */
#define UCAxCTL0               UCAxCTLW0_H    /* USCI Ax Control Register 0 */
#define OFS_UCAxCTLW1          (0x0002)       /* USCI Ax Control Word Register 1 */
#define OFS_UCAxCTLW1_L        OFS_UCAxCTLW1
#define OFS_UCAxCTLW1_H        OFS_UCAxCTLW1+1
#define OFS_UCAxBRW            (0x0006)       /* USCI Ax Baud Word Rate 0 */
#define OFS_UCAxBRW_L          OFS_UCAxBRW
#define OFS_UCAxBRW_H          OFS_UCAxBRW+1
#define OFS_UCAxBR0            (0x0006)
#define OFS_UCAxBR1            (0x0007)
#define UCAxBR0                UCAxBRW_L      /* USCI Ax Baud Rate 0 */
#define UCAxBR1                UCAxBRW_H      /* USCI Ax Baud Rate 1 */
#define OFS_UCAxMCTLW          (0x0008)       /* USCI Ax Modulation Control */
#define OFS_UCAxMCTLW_L        OFS_UCAxMCTLW
#define OFS_UCAxMCTLW_H        OFS_UCAxMCTLW+1
#define OFS_UCAxSTATW          (0x000A)       /* USCI Ax Status Register */
#define OFS_UCAxRXBUF          (0x000C)       /* USCI Ax Receive Buffer */
#define OFS_UCAxRXBUF_L        OFS_UCAxRXBUF
#define OFS_UCAxRXBUF_H        OFS_UCAxRXBUF+1
#define OFS_UCAxTXBUF          (0x000E)       /* USCI Ax Transmit Buffer */
#define OFS_UCAxTXBUF_L        OFS_UCAxTXBUF
#define OFS_UCAxTXBUF_H        OFS_UCAxTXBUF+1
#define OFS_UCAxABCTL          (0x0010)       /* USCI Ax LIN Control */
#define OFS_UCAxIRCTL          (0x0012)       /* USCI Ax IrDA Transmit Control */
#define OFS_UCAxIRCTL_L        OFS_UCAxIRCTL
#define OFS_UCAxIRCTL_H        OFS_UCAxIRCTL+1
#define OFS_UCAxIRTCTL         (0x0012)
#define OFS_UCAxIRRCTL         (0x0013)
#define UCAxIRTCTL             UCAxIRCTL_L    /* USCI Ax IrDA Transmit Control */
#define UCAxIRRCTL             UCAxIRCTL_H    /* USCI Ax IrDA Receive Control */
#define OFS_UCAxIE             (0x001A)       /* USCI Ax Interrupt Enable Register */
#define OFS_UCAxIE_L           OFS_UCAxIE
#define OFS_UCAxIE_H           OFS_UCAxIE+1
#define OFS_UCAxIFG            (0x001C)       /* USCI Ax Interrupt Flags Register */
#define OFS_UCAxIFG_L          OFS_UCAxIFG
#define OFS_UCAxIFG_H          OFS_UCAxIFG+1
#define OFS_UCAxIE__UART       (0x001A)
#define OFS_UCAxIE__UART_L     OFS_UCAxIE__UART
#define OFS_UCAxIE__UART_H     OFS_UCAxIE__UART+1
#define OFS_UCAxIFG__UART      (0x001C)
#define OFS_UCAxIFG__UART_L    OFS_UCAxIFG__UART
#define OFS_UCAxIFG__UART_H    OFS_UCAxIFG__UART+1
#define OFS_UCAxIV             (0x001E)       /* USCI Ax Interrupt Vector Register */

#define OFS_UCAxCTLW0__SPI     (0x0000)
#define OFS_UCAxCTLW0__SPI_L   OFS_UCAxCTLW0__SPI
#define OFS_UCAxCTLW0__SPI_H   OFS_UCAxCTLW0__SPI+1
#define OFS_UCAxCTL0__SPI      (0x0001)
#define OFS_UCAxCTL1__SPI      (0x0000)
#define OFS_UCAxBRW__SPI       (0x0006)
#define OFS_UCAxBRW__SPI_L     OFS_UCAxBRW__SPI
#define OFS_UCAxBRW__SPI_H     OFS_UCAxBRW__SPI+1
#define OFS_UCAxBR0__SPI       (0x0006)
#define OFS_UCAxBR1__SPI       (0x0007)
#define OFS_UCAxSTATW__SPI     (0x000A)
#define OFS_UCAxRXBUF__SPI     (0x000C)
#define OFS_UCAxRXBUF__SPI_L   OFS_UCAxRXBUF__SPI
#define OFS_UCAxRXBUF__SPI_H   OFS_UCAxRXBUF__SPI+1
#define OFS_UCAxTXBUF__SPI     (0x000E)
#define OFS_UCAxTXBUF__SPI_L   OFS_UCAxTXBUF__SPI
#define OFS_UCAxTXBUF__SPI_H   OFS_UCAxTXBUF__SPI+1
#define OFS_UCAxIE__SPI        (0x001A)
#define OFS_UCAxIFG__SPI       (0x001C)
#define OFS_UCAxIV__SPI        (0x001E)

#endif
/************************************************************
* USCI Bx
************************************************************/
#ifdef  __MSP430_HAS_EUSCI_Bx__       /* Definition to show that Module is available */

#define OFS_UCBxCTLW0__SPI     (0x0000)
#define OFS_UCBxCTLW0__SPI_L   OFS_UCBxCTLW0__SPI
#define OFS_UCBxCTLW0__SPI_H   OFS_UCBxCTLW0__SPI+1
#define OFS_UCBxCTL0__SPI      (0x0001)
#define OFS_UCBxCTL1__SPI      (0x0000)
#define OFS_UCBxBRW__SPI       (0x0006)
#define OFS_UCBxBRW__SPI_L     OFS_UCBxBRW__SPI
#define OFS_UCBxBRW__SPI_H     OFS_UCBxBRW__SPI+1
#define OFS_UCBxBR0__SPI       (0x0006)
#define OFS_UCBxBR1__SPI       (0x0007)
#define OFS_UCBxSTATW__SPI     (0x0008)
#define OFS_UCBxSTATW__SPI_L   OFS_UCBxSTATW__SPI
#define OFS_UCBxSTATW__SPI_H   OFS_UCBxSTATW__SPI+1
#define OFS_UCBxRXBUF__SPI     (0x000C)
#define OFS_UCBxRXBUF__SPI_L   OFS_UCBxRXBUF__SPI
#define OFS_UCBxRXBUF__SPI_H   OFS_UCBxRXBUF__SPI+1
#define OFS_UCBxTXBUF__SPI     (0x000E)
#define OFS_UCBxTXBUF__SPI_L   OFS_UCBxTXBUF__SPI
#define OFS_UCBxTXBUF__SPI_H   OFS_UCBxTXBUF__SPI+1
#define OFS_UCBxIE__SPI        (0x002A)
#define OFS_UCBxIE__SPI_L      OFS_UCBxIE__SPI
#define OFS_UCBxIE__SPI_H      OFS_UCBxIE__SPI+1
#define OFS_UCBxIFG__SPI       (0x002C)
#define OFS_UCBxIFG__SPI_L     OFS_UCBxIFG__SPI
#define OFS_UCBxIFG__SPI_H     OFS_UCBxIFG__SPI+1
#define OFS_UCBxIV__SPI        (0x002E)

#define OFS_UCBxCTLW0          (0x0000)       /* USCI Bx Control Word Register 0 */
#define OFS_UCBxCTLW0_L        OFS_UCBxCTLW0
#define OFS_UCBxCTLW0_H        OFS_UCBxCTLW0+1
#define OFS_UCBxCTL0           (0x0001)
#define OFS_UCBxCTL1           (0x0000)
#define UCBxCTL1               UCBxCTLW0_L    /* USCI Bx Control Register 1 */
#define UCBxCTL0               UCBxCTLW0_H    /* USCI Bx Control Register 0 */
#define OFS_UCBxCTLW1          (0x0002)       /* USCI Bx Control Word Register 1 */
#define OFS_UCBxCTLW1_L        OFS_UCBxCTLW1
#define OFS_UCBxCTLW1_H        OFS_UCBxCTLW1+1
#define OFS_UCBxBRW            (0x0006)       /* USCI Bx Baud Word Rate 0 */
#define OFS_UCBxBRW_L          OFS_UCBxBRW
#define OFS_UCBxBRW_H          OFS_UCBxBRW+1
#define OFS_UCBxBR0            (0x0006)
#define OFS_UCBxBR1            (0x0007)
#define UCBxBR0                UCBxBRW_L      /* USCI Bx Baud Rate 0 */
#define UCBxBR1                UCBxBRW_H      /* USCI Bx Baud Rate 1 */
#define OFS_UCBxSTATW          (0x0008)       /* USCI Bx Status Word Register */
#define OFS_UCBxSTATW_L        OFS_UCBxSTATW
#define OFS_UCBxSTATW_H        OFS_UCBxSTATW+1
#define OFS_UCBxSTATW__I2C     (0x0008)
#define OFS_UCBxSTAT__I2C      (0x0008)
#define OFS_UCBxBCNT__I2C      (0x0009)
#define UCBxSTAT               UCBxSTATW_L    /* USCI Bx Status Register */
#define UCBxBCNT               UCBxSTATW_H    /* USCI Bx Byte Counter Register */
#define OFS_UCBxTBCNT          (0x000A)       /* USCI Bx Byte Counter Threshold Register */
#define OFS_UCBxTBCNT_L        OFS_UCBxTBCNT
#define OFS_UCBxTBCNT_H        OFS_UCBxTBCNT+1
#define OFS_UCBxRXBUF          (0x000C)       /* USCI Bx Receive Buffer */
#define OFS_UCBxRXBUF_L        OFS_UCBxRXBUF
#define OFS_UCBxRXBUF_H        OFS_UCBxRXBUF+1
#define OFS_UCBxTXBUF          (0x000E)       /* USCI Bx Transmit Buffer */
#define OFS_UCBxTXBUF_L        OFS_UCBxTXBUF
#define OFS_UCBxTXBUF_H        OFS_UCBxTXBUF+1
#define OFS_UCBxI2COA0         (0x0014)       /* USCI Bx I2C Own Address 0 */
#define OFS_UCBxI2COA0_L       OFS_UCBxI2COA0
#define OFS_UCBxI2COA0_H       OFS_UCBxI2COA0+1
#define OFS_UCBxI2COA1         (0x0016)       /* USCI Bx I2C Own Address 1 */
#define OFS_UCBxI2COA1_L       OFS_UCBxI2COA1
#define OFS_UCBxI2COA1_H       OFS_UCBxI2COA1+1
#define OFS_UCBxI2COA2         (0x0018)       /* USCI Bx I2C Own Address 2 */
#define OFS_UCBxI2COA2_L       OFS_UCBxI2COA2
#define OFS_UCBxI2COA2_H       OFS_UCBxI2COA2+1
#define OFS_UCBxI2COA3         (0x001A)       /* USCI Bx I2C Own Address 3 */
#define OFS_UCBxI2COA3_L       OFS_UCBxI2COA3
#define OFS_UCBxI2COA3_H       OFS_UCBxI2COA3+1
#define OFS_UCBxADDRX          (0x001C)       /* USCI Bx Received Address Register */
#define OFS_UCBxADDRX_L        OFS_UCBxADDRX
#define OFS_UCBxADDRX_H        OFS_UCBxADDRX+1
#define OFS_UCBxADDMASK        (0x001E)       /* USCI Bx Address Mask Register */
#define OFS_UCBxADDMASK_L      OFS_UCBxADDMASK
#define OFS_UCBxADDMASK_H      OFS_UCBxADDMASK+1
#define OFS_UCBxI2CSA          (0x0020)       /* USCI Bx I2C Slave Address */
#define OFS_UCBxI2CSA_L        OFS_UCBxI2CSA
#define OFS_UCBxI2CSA_H        OFS_UCBxI2CSA+1
#define OFS_UCBxIE             (0x002A)       /* USCI Bx Interrupt Enable Register */
#define OFS_UCBxIE_L           OFS_UCBxIE
#define OFS_UCBxIE_H           OFS_UCBxIE+1
#define OFS_UCBxIFG            (0x002C)       /* USCI Bx Interrupt Flags Register */
#define OFS_UCBxIFG_L          OFS_UCBxIFG
#define OFS_UCBxIFG_H          OFS_UCBxIFG+1
#define OFS_UCBxIE__I2C        (0x002A)
#define OFS_UCBxIE__I2C_L      OFS_UCBxIE__I2C
#define OFS_UCBxIE__I2C_H      OFS_UCBxIE__I2C+1
#define OFS_UCBxIFG__I2C       (0x002C)
#define OFS_UCBxIFG__I2C_L     OFS_UCBxIFG__I2C
#define OFS_UCBxIFG__I2C_H     OFS_UCBxIFG__I2C+1
#define OFS_UCBxIV             (0x002E)       /* USCI Bx Interrupt Vector Register */

#endif
#if (defined(__MSP430_HAS_EUSCI_Ax__) || defined(__MSP430_HAS_EUSCI_Bx__))

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN                  (0x8000)       /* Async. Mode: Parity enable */
#define UCPAR                  (0x4000)       /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB                  (0x2000)       /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT                 (0x1000)       /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB                  (0x0800)       /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1                (0x0400)       /* Async. Mode: USCI Mode 1 */
#define UCMODE0                (0x0200)       /* Async. Mode: USCI Mode 0 */
#define UCSYNC                 (0x0100)       /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */
#define UCSSEL1                (0x0080)       /* USCI 0 Clock Source Select 1 */
#define UCSSEL0                (0x0040)       /* USCI 0 Clock Source Select 0 */
#define UCRXEIE                (0x0020)       /* RX Error interrupt enable */
#define UCBRKIE                (0x0010)       /* Break interrupt enable */
#define UCDORM                 (0x0008)       /* Dormant (Sleep) Mode */
#define UCTXADDR               (0x0004)       /* Send next Data as Address */
#define UCTXBRK                (0x0002)       /* Send next Data as Break */
#define UCSWRST                (0x0001)       /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCSSEL1_L              (0x0080)       /* USCI 0 Clock Source Select 1 */
#define UCSSEL0_L              (0x0040)       /* USCI 0 Clock Source Select 0 */
#define UCRXEIE_L              (0x0020)       /* RX Error interrupt enable */
#define UCBRKIE_L              (0x0010)       /* Break interrupt enable */
#define UCDORM_L               (0x0008)       /* Dormant (Sleep) Mode */
#define UCTXADDR_L             (0x0004)       /* Send next Data as Address */
#define UCTXBRK_L              (0x0002)       /* Send next Data as Break */
#define UCSWRST_L              (0x0001)       /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN_H                (0x0080)       /* Async. Mode: Parity enable */
#define UCPAR_H                (0x0040)       /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB_H                (0x0020)       /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT_H               (0x0010)       /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB_H                (0x0008)       /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1_H              (0x0004)       /* Async. Mode: USCI Mode 1 */
#define UCMODE0_H              (0x0002)       /* Async. Mode: USCI Mode 0 */
#define UCSYNC_H               (0x0001)       /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// UCxxCTLW0 SPI-Mode Control Bits
#define UCCKPH                 (0x8000)       /* Sync. Mode: Clock Phase */
#define UCCKPL                 (0x4000)       /* Sync. Mode: Clock Polarity */
#define UCMST                  (0x0800)       /* Sync. Mode: Master Select */
//#define res               (0x0020)    /* reserved */
//#define res               (0x0010)    /* reserved */
//#define res               (0x0008)    /* reserved */
//#define res               (0x0004)    /* reserved */
#define UCSTEM                 (0x0002)       /* USCI STE Mode */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10                  (0x8000)       /* 10-bit Address Mode */
#define UCSLA10                (0x4000)       /* 10-bit Slave Address Mode */
#define UCMM                   (0x2000)       /* Multi-Master Environment */
//#define res               (0x1000)    /* reserved */
//#define res               (0x0100)    /* reserved */
#define UCTXACK                (0x0020)       /* Transmit ACK */
#define UCTR                   (0x0010)       /* Transmit/Receive Select/Flag */
#define UCTXNACK               (0x0008)       /* Transmit NACK */
#define UCTXSTP                (0x0004)       /* Transmit STOP */
#define UCTXSTT                (0x0002)       /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
//#define res               (0x1000)    /* reserved */
//#define res               (0x0100)    /* reserved */
#define UCTXACK_L              (0x0020)       /* Transmit ACK */
#define UCTR_L                 (0x0010)       /* Transmit/Receive Select/Flag */
#define UCTXNACK_L             (0x0008)       /* Transmit NACK */
#define UCTXSTP_L              (0x0004)       /* Transmit STOP */
#define UCTXSTT_L              (0x0002)       /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10_H                (0x0080)       /* 10-bit Address Mode */
#define UCSLA10_H              (0x0040)       /* 10-bit Slave Address Mode */
#define UCMM_H                 (0x0020)       /* Multi-Master Environment */
//#define res               (0x1000)    /* reserved */
//#define res               (0x0100)    /* reserved */

#define UCMODE_0               (0x0000)       /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1               (0x0200)       /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2               (0x0400)       /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3               (0x0600)       /* Sync. Mode: USCI Mode: 3 */

#define UCSSEL_0               (0x0000)       /* USCI 0 Clock Source: 0 */
#define UCSSEL_1               (0x0040)       /* USCI 0 Clock Source: 1 */
#define UCSSEL_2               (0x0080)       /* USCI 0 Clock Source: 2 */
#define UCSSEL_3               (0x00C0)       /* USCI 0 Clock Source: 3 */
#define UCSSEL__UCLK           (0x0000)       /* USCI 0 Clock Source: UCLK */
#define UCSSEL__ACLK           (0x0040)       /* USCI 0 Clock Source: ACLK */
#define UCSSEL__SMCLK          (0x0080)       /* USCI 0 Clock Source: SMCLK */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1                (0x0002)       /* USCI Deglitch Time Bit 1 */
#define UCGLIT0                (0x0001)       /* USCI Deglitch Time Bit 0 */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1_L              (0x0002)       /* USCI Deglitch Time Bit 1 */
#define UCGLIT0_L              (0x0001)       /* USCI Deglitch Time Bit 0 */

// UCAxCTLW1 UART-Mode Control Bits

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT               (0x0100)       /* USCI Early UCTXIFG0 */
#define UCCLTO1                (0x0080)       /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0                (0x0040)       /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK              (0x0020)       /* USCI Acknowledge Stop last byte */
#define UCSWACK                (0x0010)       /* USCI Software controlled ACK */
#define UCASTP1                (0x0008)       /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0                (0x0004)       /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1                (0x0002)       /* USCI Deglitch time Bit: 1 */
#define UCGLIT0                (0x0001)       /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCCLTO1_L              (0x0080)       /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0_L              (0x0040)       /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK_L            (0x0020)       /* USCI Acknowledge Stop last byte */
#define UCSWACK_L              (0x0010)       /* USCI Software controlled ACK */
#define UCASTP1_L              (0x0008)       /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0_L              (0x0004)       /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1_L              (0x0002)       /* USCI Deglitch time Bit: 1 */
#define UCGLIT0_L              (0x0001)       /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT_H             (0x0001)       /* USCI Early UCTXIFG0 */

#define UCGLIT_0               (0x0000)       /* USCI Deglitch time: 0 */
#define UCGLIT_1               (0x0001)       /* USCI Deglitch time: 1 */
#define UCGLIT_2               (0x0002)       /* USCI Deglitch time: 2 */
#define UCGLIT_3               (0x0003)       /* USCI Deglitch time: 3 */

#define UCASTP_0               (0x0000)       /* USCI Automatic Stop condition generation: 0 */
#define UCASTP_1               (0x0004)       /* USCI Automatic Stop condition generation: 1 */
#define UCASTP_2               (0x0008)       /* USCI Automatic Stop condition generation: 2 */
#define UCASTP_3               (0x000C)       /* USCI Automatic Stop condition generation: 3 */

#define UCCLTO_0               (0x0000)       /* USCI Clock low timeout: 0 */
#define UCCLTO_1               (0x0040)       /* USCI Clock low timeout: 1 */
#define UCCLTO_2               (0x0080)       /* USCI Clock low timeout: 2 */
#define UCCLTO_3               (0x00C0)       /* USCI Clock low timeout: 3 */

/* UCAxMCTLW Control Bits */
#define UCBRS7                 (0x8000)       /* USCI Second Stage Modulation Select 7 */
#define UCBRS6                 (0x4000)       /* USCI Second Stage Modulation Select 6 */
#define UCBRS5                 (0x2000)       /* USCI Second Stage Modulation Select 5 */
#define UCBRS4                 (0x1000)       /* USCI Second Stage Modulation Select 4 */
#define UCBRS3                 (0x0800)       /* USCI Second Stage Modulation Select 3 */
#define UCBRS2                 (0x0400)       /* USCI Second Stage Modulation Select 2 */
#define UCBRS1                 (0x0200)       /* USCI Second Stage Modulation Select 1 */
#define UCBRS0                 (0x0100)       /* USCI Second Stage Modulation Select 0 */
#define UCBRF3                 (0x0080)       /* USCI First Stage Modulation Select 3 */
#define UCBRF2                 (0x0040)       /* USCI First Stage Modulation Select 2 */
#define UCBRF1                 (0x0020)       /* USCI First Stage Modulation Select 1 */
#define UCBRF0                 (0x0010)       /* USCI First Stage Modulation Select 0 */
#define UCOS16                 (0x0001)       /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRF3_L               (0x0080)       /* USCI First Stage Modulation Select 3 */
#define UCBRF2_L               (0x0040)       /* USCI First Stage Modulation Select 2 */
#define UCBRF1_L               (0x0020)       /* USCI First Stage Modulation Select 1 */
#define UCBRF0_L               (0x0010)       /* USCI First Stage Modulation Select 0 */
#define UCOS16_L               (0x0001)       /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRS7_H               (0x0080)       /* USCI Second Stage Modulation Select 7 */
#define UCBRS6_H               (0x0040)       /* USCI Second Stage Modulation Select 6 */
#define UCBRS5_H               (0x0020)       /* USCI Second Stage Modulation Select 5 */
#define UCBRS4_H               (0x0010)       /* USCI Second Stage Modulation Select 4 */
#define UCBRS3_H               (0x0008)       /* USCI Second Stage Modulation Select 3 */
#define UCBRS2_H               (0x0004)       /* USCI Second Stage Modulation Select 2 */
#define UCBRS1_H               (0x0002)       /* USCI Second Stage Modulation Select 1 */
#define UCBRS0_H               (0x0001)       /* USCI Second Stage Modulation Select 0 */

#define UCBRF_0                (0x00)         /* USCI First Stage Modulation: 0 */
#define UCBRF_1                (0x10)         /* USCI First Stage Modulation: 1 */
#define UCBRF_2                (0x20)         /* USCI First Stage Modulation: 2 */
#define UCBRF_3                (0x30)         /* USCI First Stage Modulation: 3 */
#define UCBRF_4                (0x40)         /* USCI First Stage Modulation: 4 */
#define UCBRF_5                (0x50)         /* USCI First Stage Modulation: 5 */
#define UCBRF_6                (0x60)         /* USCI First Stage Modulation: 6 */
#define UCBRF_7                (0x70)         /* USCI First Stage Modulation: 7 */
#define UCBRF_8                (0x80)         /* USCI First Stage Modulation: 8 */
#define UCBRF_9                (0x90)         /* USCI First Stage Modulation: 9 */
#define UCBRF_10               (0xA0)         /* USCI First Stage Modulation: A */
#define UCBRF_11               (0xB0)         /* USCI First Stage Modulation: B */
#define UCBRF_12               (0xC0)         /* USCI First Stage Modulation: C */
#define UCBRF_13               (0xD0)         /* USCI First Stage Modulation: D */
#define UCBRF_14               (0xE0)         /* USCI First Stage Modulation: E */
#define UCBRF_15               (0xF0)         /* USCI First Stage Modulation: F */

/* UCAxSTATW Control Bits */
#define UCLISTEN               (0x0080)       /* USCI Listen mode */
#define UCFE                   (0x0040)       /* USCI Frame Error Flag */
#define UCOE                   (0x0020)       /* USCI Overrun Error Flag */
#define UCPE                   (0x0010)       /* USCI Parity Error Flag */
#define UCBRK                  (0x0008)       /* USCI Break received */
#define UCRXERR                (0x0004)       /* USCI RX Error Flag */
#define UCADDR                 (0x0002)       /* USCI Address received Flag */
#define UCBUSY                 (0x0001)       /* USCI Busy Flag */
#define UCIDLE                 (0x0002)       /* USCI Idle line detected Flag */

/* UCBxSTATW I2C Control Bits */
#define UCBCNT7                (0x8000)       /* USCI Byte Counter Bit 7 */
#define UCBCNT6                (0x4000)       /* USCI Byte Counter Bit 6 */
#define UCBCNT5                (0x2000)       /* USCI Byte Counter Bit 5 */
#define UCBCNT4                (0x1000)       /* USCI Byte Counter Bit 4 */
#define UCBCNT3                (0x0800)       /* USCI Byte Counter Bit 3 */
#define UCBCNT2                (0x0400)       /* USCI Byte Counter Bit 2 */
#define UCBCNT1                (0x0200)       /* USCI Byte Counter Bit 1 */
#define UCBCNT0                (0x0100)       /* USCI Byte Counter Bit 0 */
#define UCSCLLOW               (0x0040)       /* SCL low */
#define UCGC                   (0x0020)       /* General Call address received Flag */
#define UCBBUSY                (0x0010)       /* Bus Busy Flag */

/* UCBxTBCNT I2C Control Bits */
#define UCTBCNT7               (0x0080)       /* USCI Byte Counter Bit 7 */
#define UCTBCNT6               (0x0040)       /* USCI Byte Counter Bit 6 */
#define UCTBCNT5               (0x0020)       /* USCI Byte Counter Bit 5 */
#define UCTBCNT4               (0x0010)       /* USCI Byte Counter Bit 4 */
#define UCTBCNT3               (0x0008)       /* USCI Byte Counter Bit 3 */
#define UCTBCNT2               (0x0004)       /* USCI Byte Counter Bit 2 */
#define UCTBCNT1               (0x0002)       /* USCI Byte Counter Bit 1 */
#define UCTBCNT0               (0x0001)       /* USCI Byte Counter Bit 0 */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5              (0x8000)       /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4              (0x4000)       /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3              (0x2000)       /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2              (0x1000)       /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1              (0x0800)       /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0              (0x0400)       /* IRDA Receive Filter Length 0 */
#define UCIRRXPL               (0x0200)       /* IRDA Receive Input Polarity */
#define UCIRRXFE               (0x0100)       /* IRDA Receive Filter enable */
#define UCIRTXPL5              (0x0080)       /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4              (0x0040)       /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3              (0x0020)       /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2              (0x0010)       /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1              (0x0008)       /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0              (0x0004)       /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK              (0x0002)       /* IRDA Transmit Pulse Clock Select */
#define UCIREN                 (0x0001)       /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRTXPL5_L            (0x0080)       /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4_L            (0x0040)       /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3_L            (0x0020)       /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2_L            (0x0010)       /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1_L            (0x0008)       /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0_L            (0x0004)       /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK_L            (0x0002)       /* IRDA Transmit Pulse Clock Select */
#define UCIREN_L               (0x0001)       /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5_H            (0x0080)       /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4_H            (0x0040)       /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3_H            (0x0020)       /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2_H            (0x0010)       /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1_H            (0x0008)       /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0_H            (0x0004)       /* IRDA Receive Filter Length 0 */
#define UCIRRXPL_H             (0x0002)       /* IRDA Receive Input Polarity */
#define UCIRRXFE_H             (0x0001)       /* IRDA Receive Filter enable */

/* UCAxABCTL Control Bits */
//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
#define UCDELIM1               (0x20)         /* Break Sync Delimiter 1 */
#define UCDELIM0               (0x10)         /* Break Sync Delimiter 0 */
#define UCSTOE                 (0x08)         /* Sync-Field Timeout error */
#define UCBTOE                 (0x04)         /* Break Timeout error */
//#define res               (0x02)    /* reserved */
#define UCABDEN                (0x01)         /* Auto Baud Rate detect enable */

/* UCBxI2COA0 Control Bits */
#define UCGCEN                 (0x8000)       /* I2C General Call enable */
#define UCOAEN                 (0x0400)       /* I2C Own Address enable */
#define UCOA9                  (0x0200)       /* I2C Own Address Bit 9 */
#define UCOA8                  (0x0100)       /* I2C Own Address Bit 8 */
#define UCOA7                  (0x0080)       /* I2C Own Address Bit 7 */
#define UCOA6                  (0x0040)       /* I2C Own Address Bit 6 */
#define UCOA5                  (0x0020)       /* I2C Own Address Bit 5 */
#define UCOA4                  (0x0010)       /* I2C Own Address Bit 4 */
#define UCOA3                  (0x0008)       /* I2C Own Address Bit 3 */
#define UCOA2                  (0x0004)       /* I2C Own Address Bit 2 */
#define UCOA1                  (0x0002)       /* I2C Own Address Bit 1 */
#define UCOA0                  (0x0001)       /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCOA7_L                (0x0080)       /* I2C Own Address Bit 7 */
#define UCOA6_L                (0x0040)       /* I2C Own Address Bit 6 */
#define UCOA5_L                (0x0020)       /* I2C Own Address Bit 5 */
#define UCOA4_L                (0x0010)       /* I2C Own Address Bit 4 */
#define UCOA3_L                (0x0008)       /* I2C Own Address Bit 3 */
#define UCOA2_L                (0x0004)       /* I2C Own Address Bit 2 */
#define UCOA1_L                (0x0002)       /* I2C Own Address Bit 1 */
#define UCOA0_L                (0x0001)       /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCGCEN_H               (0x0080)       /* I2C General Call enable */
#define UCOAEN_H               (0x0004)       /* I2C Own Address enable */
#define UCOA9_H                (0x0002)       /* I2C Own Address Bit 9 */
#define UCOA8_H                (0x0001)       /* I2C Own Address Bit 8 */

/* UCBxI2COAx Control Bits */
#define UCOAEN                 (0x0400)       /* I2C Own Address enable */
#define UCOA9                  (0x0200)       /* I2C Own Address Bit 9 */
#define UCOA8                  (0x0100)       /* I2C Own Address Bit 8 */
#define UCOA7                  (0x0080)       /* I2C Own Address Bit 7 */
#define UCOA6                  (0x0040)       /* I2C Own Address Bit 6 */
#define UCOA5                  (0x0020)       /* I2C Own Address Bit 5 */
#define UCOA4                  (0x0010)       /* I2C Own Address Bit 4 */
#define UCOA3                  (0x0008)       /* I2C Own Address Bit 3 */
#define UCOA2                  (0x0004)       /* I2C Own Address Bit 2 */
#define UCOA1                  (0x0002)       /* I2C Own Address Bit 1 */
#define UCOA0                  (0x0001)       /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOA7_L                (0x0080)       /* I2C Own Address Bit 7 */
#define UCOA6_L                (0x0040)       /* I2C Own Address Bit 6 */
#define UCOA5_L                (0x0020)       /* I2C Own Address Bit 5 */
#define UCOA4_L                (0x0010)       /* I2C Own Address Bit 4 */
#define UCOA3_L                (0x0008)       /* I2C Own Address Bit 3 */
#define UCOA2_L                (0x0004)       /* I2C Own Address Bit 2 */
#define UCOA1_L                (0x0002)       /* I2C Own Address Bit 1 */
#define UCOA0_L                (0x0001)       /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOAEN_H               (0x0004)       /* I2C Own Address enable */
#define UCOA9_H                (0x0002)       /* I2C Own Address Bit 9 */
#define UCOA8_H                (0x0001)       /* I2C Own Address Bit 8 */

/* UCBxADDRX Control Bits */
#define UCADDRX9               (0x0200)       /* I2C Receive Address Bit 9 */
#define UCADDRX8               (0x0100)       /* I2C Receive Address Bit 8 */
#define UCADDRX7               (0x0080)       /* I2C Receive Address Bit 7 */
#define UCADDRX6               (0x0040)       /* I2C Receive Address Bit 6 */
#define UCADDRX5               (0x0020)       /* I2C Receive Address Bit 5 */
#define UCADDRX4               (0x0010)       /* I2C Receive Address Bit 4 */
#define UCADDRX3               (0x0008)       /* I2C Receive Address Bit 3 */
#define UCADDRX2               (0x0004)       /* I2C Receive Address Bit 2 */
#define UCADDRX1               (0x0002)       /* I2C Receive Address Bit 1 */
#define UCADDRX0               (0x0001)       /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX7_L             (0x0080)       /* I2C Receive Address Bit 7 */
#define UCADDRX6_L             (0x0040)       /* I2C Receive Address Bit 6 */
#define UCADDRX5_L             (0x0020)       /* I2C Receive Address Bit 5 */
#define UCADDRX4_L             (0x0010)       /* I2C Receive Address Bit 4 */
#define UCADDRX3_L             (0x0008)       /* I2C Receive Address Bit 3 */
#define UCADDRX2_L             (0x0004)       /* I2C Receive Address Bit 2 */
#define UCADDRX1_L             (0x0002)       /* I2C Receive Address Bit 1 */
#define UCADDRX0_L             (0x0001)       /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX9_H             (0x0002)       /* I2C Receive Address Bit 9 */
#define UCADDRX8_H             (0x0001)       /* I2C Receive Address Bit 8 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9             (0x0200)       /* I2C Address Mask Bit 9 */
#define UCADDMASK8             (0x0100)       /* I2C Address Mask Bit 8 */
#define UCADDMASK7             (0x0080)       /* I2C Address Mask Bit 7 */
#define UCADDMASK6             (0x0040)       /* I2C Address Mask Bit 6 */
#define UCADDMASK5             (0x0020)       /* I2C Address Mask Bit 5 */
#define UCADDMASK4             (0x0010)       /* I2C Address Mask Bit 4 */
#define UCADDMASK3             (0x0008)       /* I2C Address Mask Bit 3 */
#define UCADDMASK2             (0x0004)       /* I2C Address Mask Bit 2 */
#define UCADDMASK1             (0x0002)       /* I2C Address Mask Bit 1 */
#define UCADDMASK0             (0x0001)       /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK7_L           (0x0080)       /* I2C Address Mask Bit 7 */
#define UCADDMASK6_L           (0x0040)       /* I2C Address Mask Bit 6 */
#define UCADDMASK5_L           (0x0020)       /* I2C Address Mask Bit 5 */
#define UCADDMASK4_L           (0x0010)       /* I2C Address Mask Bit 4 */
#define UCADDMASK3_L           (0x0008)       /* I2C Address Mask Bit 3 */
#define UCADDMASK2_L           (0x0004)       /* I2C Address Mask Bit 2 */
#define UCADDMASK1_L           (0x0002)       /* I2C Address Mask Bit 1 */
#define UCADDMASK0_L           (0x0001)       /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9_H           (0x0002)       /* I2C Address Mask Bit 9 */
#define UCADDMASK8_H           (0x0001)       /* I2C Address Mask Bit 8 */

/* UCBxI2CSA Control Bits */
#define UCSA9                  (0x0200)       /* I2C Slave Address Bit 9 */
#define UCSA8                  (0x0100)       /* I2C Slave Address Bit 8 */
#define UCSA7                  (0x0080)       /* I2C Slave Address Bit 7 */
#define UCSA6                  (0x0040)       /* I2C Slave Address Bit 6 */
#define UCSA5                  (0x0020)       /* I2C Slave Address Bit 5 */
#define UCSA4                  (0x0010)       /* I2C Slave Address Bit 4 */
#define UCSA3                  (0x0008)       /* I2C Slave Address Bit 3 */
#define UCSA2                  (0x0004)       /* I2C Slave Address Bit 2 */
#define UCSA1                  (0x0002)       /* I2C Slave Address Bit 1 */
#define UCSA0                  (0x0001)       /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA7_L                (0x0080)       /* I2C Slave Address Bit 7 */
#define UCSA6_L                (0x0040)       /* I2C Slave Address Bit 6 */
#define UCSA5_L                (0x0020)       /* I2C Slave Address Bit 5 */
#define UCSA4_L                (0x0010)       /* I2C Slave Address Bit 4 */
#define UCSA3_L                (0x0008)       /* I2C Slave Address Bit 3 */
#define UCSA2_L                (0x0004)       /* I2C Slave Address Bit 2 */
#define UCSA1_L                (0x0002)       /* I2C Slave Address Bit 1 */
#define UCSA0_L                (0x0001)       /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA9_H                (0x0002)       /* I2C Slave Address Bit 9 */
#define UCSA8_H                (0x0001)       /* I2C Slave Address Bit 8 */

/* UCAxIE UART Control Bits */
#define UCTXCPTIE              (0x0008)       /* UART Transmit Complete Interrupt Enable */
#define UCSTTIE                (0x0004)       /* UART Start Bit Interrupt Enalble */
#define UCTXIE                 (0x0002)       /* UART Transmit Interrupt Enable */
#define UCRXIE                 (0x0001)       /* UART Receive Interrupt Enable */

/* UCAxIE/UCBxIE SPI Control Bits */

/* UCBxIE I2C Control Bits */
#define UCBIT9IE               (0x4000)       /* I2C Bit 9 Position Interrupt Enable 3 */
#define UCTXIE3                (0x2000)       /* I2C Transmit Interrupt Enable 3 */
#define UCRXIE3                (0x1000)       /* I2C Receive Interrupt Enable 3 */
#define UCTXIE2                (0x0800)       /* I2C Transmit Interrupt Enable 2 */
#define UCRXIE2                (0x0400)       /* I2C Receive Interrupt Enable 2 */
#define UCTXIE1                (0x0200)       /* I2C Transmit Interrupt Enable 1 */
#define UCRXIE1                (0x0100)       /* I2C Receive Interrupt Enable 1 */
#define UCCLTOIE               (0x0080)       /* I2C Clock Low Timeout interrupt enable */
#define UCBCNTIE               (0x0040)       /* I2C Automatic stop assertion interrupt enable */
#define UCNACKIE               (0x0020)       /* I2C NACK Condition interrupt enable */
#define UCALIE                 (0x0010)       /* I2C Arbitration Lost interrupt enable */
#define UCSTPIE                (0x0008)       /* I2C STOP Condition interrupt enable */
#define UCSTTIE                (0x0004)       /* I2C START Condition interrupt enable */
#define UCTXIE0                (0x0002)       /* I2C Transmit Interrupt Enable 0 */
#define UCRXIE0                (0x0001)       /* I2C Receive Interrupt Enable 0 */

/* UCAxIFG UART Control Bits */
#define UCTXCPTIFG             (0x0008)       /* UART Transmit Complete Interrupt Flag */
#define UCSTTIFG               (0x0004)       /* UART Start Bit Interrupt Flag */
#define UCTXIFG                (0x0002)       /* UART Transmit Interrupt Flag */
#define UCRXIFG                (0x0001)       /* UART Receive Interrupt Flag */

/* UCAxIFG/UCBxIFG SPI Control Bits */
#define UCTXIFG                (0x0002)       /* SPI Transmit Interrupt Flag */
#define UCRXIFG                (0x0001)       /* SPI Receive Interrupt Flag */

/* UCBxIFG Control Bits */
#define UCBIT9IFG              (0x4000)       /* I2C Bit 9 Possition Interrupt Flag 3 */
#define UCTXIFG3               (0x2000)       /* I2C Transmit Interrupt Flag 3 */
#define UCRXIFG3               (0x1000)       /* I2C Receive Interrupt Flag 3 */
#define UCTXIFG2               (0x0800)       /* I2C Transmit Interrupt Flag 2 */
#define UCRXIFG2               (0x0400)       /* I2C Receive Interrupt Flag 2 */
#define UCTXIFG1               (0x0200)       /* I2C Transmit Interrupt Flag 1 */
#define UCRXIFG1               (0x0100)       /* I2C Receive Interrupt Flag 1 */
#define UCCLTOIFG              (0x0080)       /* I2C Clock low Timeout interrupt Flag */
#define UCBCNTIFG              (0x0040)       /* I2C Byte counter interrupt flag */
#define UCNACKIFG              (0x0020)       /* I2C NACK Condition interrupt Flag */
#define UCALIFG                (0x0010)       /* I2C Arbitration Lost interrupt Flag */
#define UCSTPIFG               (0x0008)       /* I2C STOP Condition interrupt Flag */
#define UCSTTIFG               (0x0004)       /* I2C START Condition interrupt Flag */
#define UCTXIFG0               (0x0002)       /* I2C Transmit Interrupt Flag 0 */
#define UCRXIFG0               (0x0001)       /* I2C Receive Interrupt Flag 0 */

/* USCI UART Definitions */
#define USCI_NONE              (0x0000)       /* No Interrupt pending */
#define USCI_UART_UCRXIFG      (0x0002)       /* USCI UCRXIFG */
#define USCI_UART_UCTXIFG      (0x0004)       /* USCI UCTXIFG */
#define USCI_UART_UCSTTIFG     (0x0006)       /* USCI UCSTTIFG */
#define USCI_UART_UCTXCPTIFG   (0x0008)       /* USCI UCTXCPTIFG */

/* USCI SPI Definitions */
#define USCI_SPI_UCRXIFG       (0x0002)       /* USCI UCRXIFG */
#define USCI_SPI_UCTXIFG       (0x0004)       /* USCI UCTXIFG */

/* USCI I2C Definitions */
#define USCI_I2C_UCALIFG       (0x0002)       /* USCI I2C Mode: UCALIFG */
#define USCI_I2C_UCNACKIFG     (0x0004)       /* USCI I2C Mode: UCNACKIFG */
#define USCI_I2C_UCSTTIFG      (0x0006)       /* USCI I2C Mode: UCSTTIFG*/
#define USCI_I2C_UCSTPIFG      (0x0008)       /* USCI I2C Mode: UCSTPIFG*/
#define USCI_I2C_UCRXIFG3      (0x000A)       /* USCI I2C Mode: UCRXIFG3 */
#define USCI_I2C_UCTXIFG3      (0x000C)       /* USCI I2C Mode: UCTXIFG3 */
#define USCI_I2C_UCRXIFG2      (0x000E)       /* USCI I2C Mode: UCRXIFG2 */
#define USCI_I2C_UCTXIFG2      (0x0010)       /* USCI I2C Mode: UCTXIFG2 */
#define USCI_I2C_UCRXIFG1      (0x0012)       /* USCI I2C Mode: UCRXIFG1 */
#define USCI_I2C_UCTXIFG1      (0x0014)       /* USCI I2C Mode: UCTXIFG1 */
#define USCI_I2C_UCRXIFG0      (0x0016)       /* USCI I2C Mode: UCRXIFG0 */
#define USCI_I2C_UCTXIFG0      (0x0018)       /* USCI I2C Mode: UCTXIFG0 */
#define USCI_I2C_UCBCNTIFG     (0x001A)       /* USCI I2C Mode: UCBCNTIFG */
#define USCI_I2C_UCCLTOIFG     (0x001C)       /* USCI I2C Mode: UCCLTOIFG */
#define USCI_I2C_UCBIT9IFG     (0x001E)       /* USCI I2C Mode: UCBIT9IFG */

#endif
/************************************************************
* WATCHDOG TIMER A
************************************************************/
#ifdef  __MSP430_HAS_WDT_A__          /* Definition to show that Module is available */

#define OFS_WDTCTL             (0x000C)       /* Watchdog Timer Control */
#define OFS_WDTCTL_L           OFS_WDTCTL
#define OFS_WDTCTL_H           OFS_WDTCTL+1
/* The bit names have been prefixed with "WDT" */
/* WDTCTL Control Bits */
#define WDTIS0                 (0x0001)       /* WDT - Timer Interval Select 0 */
#define WDTIS1                 (0x0002)       /* WDT - Timer Interval Select 1 */
#define WDTIS2                 (0x0004)       /* WDT - Timer Interval Select 2 */
#define WDTCNTCL               (0x0008)       /* WDT - Timer Clear */
#define WDTTMSEL               (0x0010)       /* WDT - Timer Mode Select */
#define WDTSSEL0               (0x0020)       /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1               (0x0040)       /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD                (0x0080)       /* WDT - Timer hold */

/* WDTCTL Control Bits */
#define WDTIS0_L               (0x0001)       /* WDT - Timer Interval Select 0 */
#define WDTIS1_L               (0x0002)       /* WDT - Timer Interval Select 1 */
#define WDTIS2_L               (0x0004)       /* WDT - Timer Interval Select 2 */
#define WDTCNTCL_L             (0x0008)       /* WDT - Timer Clear */
#define WDTTMSEL_L             (0x0010)       /* WDT - Timer Mode Select */
#define WDTSSEL0_L             (0x0020)       /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1_L             (0x0040)       /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD_L              (0x0080)       /* WDT - Timer hold */

/* WDTCTL Control Bits */

#define WDTPW                  (0x5A00)

#define WDTIS_0                (0*0x0001u)    /* WDT - Timer Interval Select: /2G */
#define WDTIS_1                (1*0x0001u)    /* WDT - Timer Interval Select: /128M */
#define WDTIS_2                (2*0x0001u)    /* WDT - Timer Interval Select: /8192k */
#define WDTIS_3                (3*0x0001u)    /* WDT - Timer Interval Select: /512k */
#define WDTIS_4                (4*0x0001u)    /* WDT - Timer Interval Select: /32k */
#define WDTIS_5                (5*0x0001u)    /* WDT - Timer Interval Select: /8192 */
#define WDTIS_6                (6*0x0001u)    /* WDT - Timer Interval Select: /512 */
#define WDTIS_7                (7*0x0001u)    /* WDT - Timer Interval Select: /64 */
#define WDTIS__2G              (0*0x0001u)    /* WDT - Timer Interval Select: /2G */
#define WDTIS__128M            (1*0x0001u)    /* WDT - Timer Interval Select: /128M */
#define WDTIS__8192K           (2*0x0001u)    /* WDT - Timer Interval Select: /8192k */
#define WDTIS__512K            (3*0x0001u)    /* WDT - Timer Interval Select: /512k */
#define WDTIS__32K             (4*0x0001u)    /* WDT - Timer Interval Select: /32k */
#define WDTIS__8192            (5*0x0001u)    /* WDT - Timer Interval Select: /8192 */
#define WDTIS__512             (6*0x0001u)    /* WDT - Timer Interval Select: /512 */
#define WDTIS__64              (7*0x0001u)    /* WDT - Timer Interval Select: /64 */

#define WDTSSEL_0              (0*0x0020u)    /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL_1              (1*0x0020u)    /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL_2              (2*0x0020u)    /* WDT - Timer Clock Source Select: VLO_CLK */
#define WDTSSEL_3              (3*0x0020u)    /* WDT - Timer Clock Source Select: reserved */
#define WDTSSEL__SMCLK         (0*0x0020u)    /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL__ACLK          (1*0x0020u)    /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL__VLO           (2*0x0020u)    /* WDT - Timer Clock Source Select: VLO_CLK */

/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2)                         /* 32ms interval (default) */
#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS0)                  /* 8ms     " */
#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1)                  /* 0.5ms   " */
#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)                /* 1000ms  " */
#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS0)         /* 250ms   " */
#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1)         /* 16ms    " */
#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1+WDTIS0)  /* 1.9ms   " */
/* Watchdog mode -> reset after expired time */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MRST_32         (WDTPW+WDTCNTCL+WDTIS2)                                  /* 32ms interval (default) */
#define WDT_MRST_8          (WDTPW+WDTCNTCL+WDTIS2+WDTIS0)                           /* 8ms     " */
#define WDT_MRST_0_5        (WDTPW+WDTCNTCL+WDTIS2+WDTIS1)                           /* 0.5ms   " */
#define WDT_MRST_0_064      (WDTPW+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)                    /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ARST_1000       (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2)                         /* 1000ms  " */
#define WDT_ARST_250        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS0)                  /* 250ms   " */
#define WDT_ARST_16         (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1)                  /* 16ms    " */
#define WDT_ARST_1_9        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1+WDTIS0)           /* 1.9ms   " */

#endif

/************************************************************
* TLV Descriptors
************************************************************/
#define __MSP430_HAS_TLV__                    /* Definition to show that Module is available */

#define TLV_START              (0x1A08)       /* Start Address of the TLV structure */
#define TLV_END                (0x1AFF)       /* End Address of the TLV structure */

#define TLV_LDTAG              (0x01)         /*  Legacy descriptor (1xx, 2xx, 4xx families) */
#define TLV_PDTAG              (0x02)         /*  Peripheral discovery descriptor */
#define TLV_Reserved3          (0x03)         /*  Future usage */
#define TLV_Reserved4          (0x04)         /*  Future usage */
#define TLV_BLANK              (0x05)         /*  Blank descriptor */
#define TLV_Reserved6          (0x06)         /*  Future usage */
#define TLV_Reserved7          (0x07)         /*  Serial Number */
#define TLV_DIERECORD          (0x08)         /*  Die Record  */
#define TLV_ADCCAL             (0x11)         /*  ADC12 calibration */
#define TLV_ADC12CAL           (0x11)         /*  ADC12 calibration */
#define TLV_REFCAL             (0x12)         /*  REF calibration */
#define TLV_ADC10CAL           (0x13)         /*  ADC10 calibration */
#define TLV_TIMERDCAL          (0x15)         /*  TIMER_D calibration */
#define TLV_TAGEXT             (0xFE)         /*  Tag extender */
#define TLV_TAGEND             (0xFF)         /*  Tag End of Table */

/************************************************************
* Interrupt Vectors (offset from 0xFF80)
************************************************************/

#pragma diag_suppress 1107
#define VECTOR_NAME(name)             name##_ptr
#define EMIT_PRAGMA(x)                _Pragma(#x)
#define CREATE_VECTOR(name)           void * const VECTOR_NAME(name) = (void *)(long)&name
#define PLACE_VECTOR(vector,section)  EMIT_PRAGMA(DATA_SECTION(vector,section))
#define PLACE_INTERRUPT(func)         EMIT_PRAGMA(CODE_SECTION(func,".text:_isr"))
#define ISR_VECTOR(func,offset)       CREATE_VECTOR(func); \
                                      PLACE_VECTOR(VECTOR_NAME(func), offset) \
                                      PLACE_INTERRUPT(func)


/************************************************************
* End of Modules
************************************************************/

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __msp430XGENERIC */

