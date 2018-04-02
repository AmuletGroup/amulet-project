/*****************************************************************************
 *   timer32.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __TIMER32_H
#define __TIMER32_H

// CodeRed - variable name changed in CMSIS 1.3
#define SystemFrequency SystemCoreClock

#define TIME_INTERVAL    ((SystemFrequency/LPC_SYSCON->SYSAHBCLKDIV)/100 - 1)
/* depending on the SystemFrequency and System AHB clock divider setting,
if SystemFrequency = 60Mhz, SYSAHBCLKDIV = 4, SystemAHBFrequency = 1/4 SystemFrequency,
10mSec = 150.000-1 counts */

void delay32Ms(uint8_t timer_num, uint32_t delayInMs);
void TIMER32_0_IRQHandler(void);
void TIMER32_1_IRQHandler(void);
void enable_timer32(uint8_t timer_num);
void disable_timer32(uint8_t timer_num);
void reset_timer32(uint8_t timer_num);
void init_timer32(uint8_t timer_num, uint32_t timerInterval);

#endif /* end __TIMER32_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
