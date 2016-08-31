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
/*----------------------------------------------------------------------------*/
/* in430.h      - Intrinsic function prototypes and convenience mapping       */
/*                macros for migrating code from the IAR platform.            */
/*                                                                            */
/*  Ver | dd mmm yyyy | Who  | Description of changes                         */
/* =====|=============|======|=============================================   */
/*  0.01| 06 Apr 2004 | A.D. | First Prototype                                */
/*  0.02| 22 Jun 2004 | A.D. | File reformatted                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef __IN430_H
#define __IN430_H

/*----------------------------------------------------------------------------*/
/* COMPILER INTRINSIC FUNCTIONS                                               */
/*----------------------------------------------------------------------------*/

void _enable_interrupts(void); 
void _disable_interrupts(void); 
unsigned short _bic_SR_register(unsigned short mask);
unsigned short _bic_SR_register_on_exit(unsigned short mask); 
unsigned short _bis_SR_register(unsigned short mask); 
unsigned short _bis_SR_register_on_exit(unsigned short mask);
unsigned short _get_SR_register(void); 
unsigned short _get_SR_register_on_exit(void); 
unsigned short _swap_bytes(unsigned short src); 
void _nop(void); 
void _never_executed(void);

/*----------------------------------------------------------------------------*/
/* INTRINSIC MAPPING FOR IAR V1.XX                                            */
/*----------------------------------------------------------------------------*/

#define _EINT()                         _enable_interrupts()
#define _DINT()                         _disable_interrupts()
#define _BIC_SR(x)                      _bic_SR_register(x)
#define _BIC_SR_IRQ(x)                  _bic_SR_register_on_exit(x)
#define _BIS_SR(x)                      _bis_SR_register(x)
#define _BIS_SR_IRQ(x)                  _bis_SR_register_on_exit(x)
#define _SWAP_BYTES(x)                  _swap_bytes(x)
#define _NOP()                          _nop()

/*----------------------------------------------------------------------------*/
/* INTRINSIC MAPPING FOR IAR V2.XX/V3.XX                                      */
/*----------------------------------------------------------------------------*/

#define __enable_interrupt()            _enable_interrupts()
#define __disable_interrupt()           _disable_interrupts()
#define __bic_SR_register(x)            _bic_SR_register(x)
#define __bic_SR_register_on_exit(x)    _bic_SR_register_on_exit(x)
#define __bis_SR_register(x)            _bis_SR_register(x)
#define __bis_SR_register_on_exit(x)    _bis_SR_register_on_exit(x)
#define __get_SR_register()             _get_SR_register()
#define __get_SR_register_on_exit()     _get_SR_register_on_exit()
#define __swap_bytes(x)                 _swap_bytes(x)
#define __no_operation()                _nop()

#endif /* __IN430_H */
