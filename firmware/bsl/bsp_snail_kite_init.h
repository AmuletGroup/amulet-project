/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */


#ifndef bsp_snail_kite_init_h
#define bsp_snail_kite_init_h

/**
 * ADC driver related definitions for the Amulet custom (bracelet) boards.
 * These include the analog sensors, Light, Temp, and Audio
 */
#define ACCEL_CS_PxDIR PJDIR
#define ACCEL_CS_PxOUT PJOUT
#define ACCEL_CS_BITx BIT6

#define ACCEL_ENABLE_PxSEL0 P4SEL0
#define ACCEL_ENABLE_PxSEL1 P4SEL1
#define ACCEL_ENABLE_PxDIR P4DIR
#define ACCEL_ENABLE_PxOUT P4OUT
#define ACCEL_ENABLE_BITx BIT3

#define UCxxIFG_ACCEL UCB1IFG
#define UCxxTXBUF_ACCEL UCB1TXBUF
#define UCxxSTATW_ACCEL UCB1STATW
#define UCxxRXBUF_ACCEL UCB1RXBUF
#define UCxxCTLW0_ACCEL UCB1CTLW0
#define UCxxBR0_ACCEL UCB1BR0
#define UCxxBR1_ACCEL UCB1BR1
#define UCxxCTL1_ACCEL UCB1CTL1

#define ACCEL_SPI_REN P4REN
#define ACCEL_SPI_OUT P4OUT
#define ACCEL_SPI_SEL0 P4SEL0  // P1SEL0
#define ACCEL_SPI_SEL1 P4SEL1  // P1SEL0
#define ACCEL_SPI_DIR P4DIR    // P1DIR
#define ACCEL_SPI_SOMI BIT1  // P1.7 expansion port MISO shared b/w SD card and display
#define ACCEL_SPI_SIMO BIT0  // P1.6 expansion port MOSI used by SD card
#define ACCEL_SPI_CLK_DIR P4DIR
#define ACCEL_SPI_CLK BIT2  // P2.2 expansion port SCLK shared b/w SD card and display
#define ACCEL_SPI_CLK_SEL0 P4SEL0
#define ACCEL_SPI_CLK_SEL1 P4SEL1

/**
 * Display definitions
 */

#define DISP_CS_PxDIR PJDIR
#define DISP_CS_PxOUT PJOUT
#define DISP_CS_BITx BIT7

#define UCxxIFG_DISP UCB1IFG
#define UCxxTXBUF_DISP UCB1TXBUF
#define UCxxSTATW_DISP UCB1STATW
#define UCxxRXBUF_DISP UCB1RXBUF
#define UCxxCTLW0_DISP UCB1CTLW0
#define UCxxBR0_DISP UCB1BR0
#define UCxxBR1_DISP UCB1BR1
#define UCxxCTL1_DISP UCB1CTL1

#define DISP_SPI_SEL0 P4SEL0
#define DISP_SPI_SEL1 P4SEL1
#define DISP_SPI_DIR P4DIR
#define DISP_SPI_SOMI BIT1
#define DISP_SPI_SIMO BIT0
#define DISP_SPI_CLK_DIR P4DIR
#define DISP_SPI_CLK BIT2  // P2.2 expansion port SCLK shared b/w SD card and display
#define DISP_SPI_CLK_SEL0 P4SEL0
#define DISP_SPI_CLK_SEL1 P4SEL1

/**
* IPC definitions (Bluetooth)
*/

#define IPC_CS_PxDIR P2DIR
#define IPC_CS_PxOUT P2OUT
#define IPC_CS_BITx BIT3

#define IPC_PWR_PxDIR PJDIR
#define IPC_PWR_PxOUT PJOUT
#define IPC_PWR_BITx BIT3

#define UCxxIFG_IPC UCA0IFG
#define UCxxTXBUF_IPC UCA0TXBUF
//#define UCxxSTATW_IPC UCB0STATW
#define UCxxRXBUF_IPC UCA0RXBUF
#define UCxxCTLW0_IPC UCA0CTLW0
#define UCxxBR0_IPC UCA0BR0
#define UCxxBR1_IPC UCA0BR1
//#define UCxxCTL1_IPC UCB0CTL1

#define IPC_SPI_REN P2REN
#define IPC_SPI_SEL0 P2SEL0 
#define IPC_SPI_SEL1 P2SEL1  
#define IPC_SPI_DIR P2DIR    
#define IPC_SPI_OUT P2OUT    
#define IPC_SPI_SOMI BIT1  
#define IPC_SPI_SIMO BIT0  
#define IPC_SPI_CLK_DIR P2DIR
#define IPC_SPI_CLK BIT2  
#define IPC_SPI_CLK_SEL0 P2SEL0
#define IPC_SPI_CLK_SEL1 P2SEL1

#define GYRO_ENABLE_PxSEL0 P1SEL0
#define GYRO_ENABLE_PxSEL1 P1SEL1
#define GYRO_ENABLE_PxDIR P1DIR
#define GYRO_ENABLE_PxOUT P1OUT
#define GYRO_ENABLE_BITx BIT4

#define GYRO_CS_PxSEL0 P2SEL0
#define GYRO_CS_PxSEL1 P2SEL1
#define GYRO_CS_PxDIR P2DIR
#define GYRO_CS_PxOUT P2OUT
#define GYRO_CS_BITx BIT5

#define PROFLILNG_DIR P9DIR
#define PROFLILNG_PORT P9OUT
#define PROFLILNG_PIN BIT3

#define CORE_PROFILING_PORT GPIO_PORT_P2
#define CORE_PROFILING_PIN GPIO_PIN4

// Battery voltage ADC -- P1.1
#define BATTERY_ADC_PXSEL0 P1SEL0
#define BATTERY_ADC_PXSEL1 P1SEL1
#define BATTERY_ADC_PXDIR P1DIR
#define BATTERY_ADC_INPUT_BITX BIT1
#define BATTERY_INCH ADC12INCH_1

// Light ADC  -- P9.7
#define LIGHT_ADC_PXSEL0 P9SEL0
#define LIGHT_ADC_PXSEL1 P9SEL1
#define LIGHT_ADC_PXDIR P9DIR
#define LIGHT_ADC_INPUT_BITX BIT7
#define LIGHT_INCH ADC12INCH_15
// P9.6
#define LIGHT_POWER_OUT P9OUT
#define LIGHT_POWER_DIR P9DIR
#define LIGHT_POWER_BIT BIT6

// Temp ADC  -- P1.2
#define TEMP_ADC_PXSEL0 P1SEL0
#define TEMP_ADC_PXSEL1 P1SEL1
#define TEMP_ADC_PXDIR P1DIR
#define TEMP_ADC_INPUT_BITX BIT2
#define TEMP_INCH ADC12INCH_2
// P1.3
#define TEMP_POWER_OUT P1OUT
#define TEMP_POWER_DIR P1DIR
#define TEMP_POWER_BIT BIT3

// Mic ADC  -- P9.2
#define AUDIO_ADC_PXSEL0 P9SEL0
#define AUDIO_ADC_PXSEL1 P9SEL1
#define AUDIO_ADC_PXDIR P9DIR
#define AUDIO_ADC_PXOUT P9OUT
#define AUDIO_ADC_INPUT_BITX BIT2
#define AUDIO_INCH ADC12INCH_10
// P3.3
#define AUDIO_POWER_OUT PJOUT
#define AUDIO_POWER_DIR PJDIR
#define AUDIO_POWER_BIT BIT0

/**
 * Button driver related definitions for the Amulet custom (bracelet) boards.
 * Button 0: P3.2
 * Button 1: P4.3
 */
#define button0_PxDIR P3DIR
#define button0_PxREN P3REN
#define button0_PxSEL0 P3SEL0
#define button0_PxSEL1 P3SEL1
#define button0_PxIE P3IE
#define button0_PxIFG P3IFG
#define button0_PxIES P3IES
#define button0_PxIN P3IN
#define button0_PxOUT P3OUT
#define button0_BITx BIT1

#define button1_PxDIR P1DIR
#define button1_PxREN P1REN
#define button1_PxSEL0 P1SEL0
#define button1_PxSEL1 P1SEL1
#define button1_PxIE P1IE
#define button1_PxIFG P1IFG
#define button1_PxIES P1IES
#define button1_PxIN P1IN
#define button1_PxOUT P1OUT
#define button1_BITx BIT5

/**
 * Haptic driver related definitions for the Amulet custom (bracelet) boards.
 * P9.3
 */
#define haptic_PxDIR P9DIR
#define haptic_PxOUT P9OUT
#define haptic_BITx BIT3

/**
 * LED driver related definitions for the Amulet custom (bracelet) boards.
 * LED1: P9.5
 * LED2: P9.4
 */
#define LED1_PxDIR P9DIR
#define LED1_PxOUT P9OUT
#define LED1_BITx BIT5
#define LED2_PxDIR P9DIR
#define LED2_PxOUT P9OUT
#define LED2_BITx BIT4
#define LED1 LED1_BITx
#define LED2 LED2_BITx

/**
 * IPC pin definitions
 */

// IRQ in from M0, P3.7
#define IPC_PxOUT P2OUT
#define IPC_PxDIR P2DIR
#define IPC_PxIE P2IE
#define IPC_PxREN P2REN
#define IPC_PxIES P2IES
#define IPC_PxIFG P2IFG
#define IPC_interrupt_pin BIT4

// IRQ Out to M0, P2.2
// THESE ARE NOT CORRECT...
#define IPC_OUPUT_TO_M0_PORT P5OUT
#define IPC_OUPUT_TO_M0_DIR P5DIR
#define IPC_OUTPUT_PIN BIT0

/**
 * SD card definitions
 */

#define SPI_REN P3REN
#define SPI_OUT P3OUT
#define SPI_SEL P3SEL0
#define SPI_DIR P3DIR
#define SPI_SOMI BIT5
#define SPI_SIMO BIT4

//#define UCxxIFG_imu UCB0IFG

#define SPI_CLK_SEL P3SEL0
#define SPI_CLK_DIR P3DIR
#define SPI_CLK_OUT P3OUT
#define SPI_CLK BIT6  // P2.2 expansion port SCLK shared b/w SD card and display

#define SD_CS_SEL P3SEL1
#define SD_CS_OUT P3OUT
#define SD_CS_DIR P3DIR
#define SD_CS BIT3  //  Active low chip select line for SD card
#define SD_POWER_OUT P3OUT
#define SD_POWER_DIR P3DIR
#define SD_POWER BIT2

//#defines for the diskio.c files
#define UCxxIFG UCA1IFG
#define UCxxTXBUF UCA1TXBUF
#define UCxxSTATW UCA1STATW
#define UCxxRXBUF UCA1RXBUF
#define UCxxCTLW0 UCA1CTLW0
#define UCxxBR0 UCA1BR0
#define UCxxBR1 UCA1BR1


#endif // bsp_snail_kite_init_h
