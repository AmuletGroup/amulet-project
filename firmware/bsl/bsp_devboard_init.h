
#ifndef bsp_devboard_init_h
#define bsp_devboard_init_h

/**
 * ADC driver related definitions for the Amulet custom (bracelet) boards.
 * These include the analog sensors, Light, Temp, and Audio
 */

#define PROFLILNG_DIR P2DIR
#define PROFLILNG_PORT P2OUT
#define PROFLILNG_PIN BIT7

// Pin 4.2 App Board Expansion port 0 (Scroll Wheel)
#define adc_PxSEL0 P4SEL0
#define adc_PxSEL1 P4SEL1
#define adc_PxDIR P4DIR
#define adc_input_BITx BIT2
#define THUMBWHEEL_INCH ADC12INCH_10

// Pin 4.3 App Board Expansion port 1 (Scroll Wheel)
#define thumbwheel_PxDIR P4DIR
#define thumbwheel_PxOUT P4OUT
#define thumbwheel_BITx BIT3  // good

// Light ADC  -- P1.1
#define LIGHT_ADC_PXSEL0 P1SEL0
#define LIGHT_ADC_PXSEL1 P1SEL1
#define LIGHT_ADC_PXDIR P1DIR
#define LIGHT_ADC_INPUT_BITX BIT1
#define LIGHT_INCH ADC12INCH_1
// P1.0
#define LIGHT_POWER_OUT P1OUT
#define LIGHT_POWER_DIR P1DIR
#define LIGHT_POWER_BIT BIT0

// Temp ADC  -- P1.2
#define TEMP_ADC_PXSEL0 P1SEL0
#define TEMP_ADC_PXSEL1 P1SEL1
#define TEMP_ADC_PXDIR P1DIR
#define TEMP_ADC_INPUT_BITX BIT2
#define TEMP_INCH ADC12INCH_2
// P3.0
#define TEMP_POWER_OUT P3OUT
#define TEMP_POWER_DIR P3DIR
#define TEMP_POWER_BIT BIT0

// Mic ADC  -- P3.1
#define AUDIO_ADC_PXSEL0 P3SEL0
#define AUDIO_ADC_PXSEL1 P3SEL1
#define AUDIO_ADC_PXDIR P3DIR
#define AUDIO_ADC_INPUT_BITX BIT1
#define AUDIO_INCH ADC12INCH_13
// P3.2
#define AUDIO_POWER_OUT P3OUT
#define AUDIO_POWER_DIR P3DIR
#define AUDIO_POWER_BIT BIT2

/**
 * Button driver related definitions for the Amulet development boards.
 *
 * @note These definitions were moved to here from buttons_driver.h.
 */
#define button0_PxDIR P4DIR
#define button0_PxREN P4REN
#define button0_PxSEL0 P4SEL0
#define button0_PxSEL1 P4SEL1
#define button0_PxIE P4IE
#define button0_PxIFG P4IFG
#define button0_PxIES P4IES
#define button0_PxIN P4IN
#define button0_PxOUT P4OUT
#define button0_BITx BIT5

#define button1_PxDIR P1DIR
#define button1_PxREN P1REN
#define button1_PxSEL0 P1SEL0
#define button1_PxSEL1 P1SEL1
#define button1_PxIE P1IE
#define button1_PxIFG P1IFG
#define button1_PxIES P1IES
#define button1_PxIN P1IN
#define button1_PxOUT P1OUT
#define button1_BITx BIT1

/**
 * Haptic driver related definitions for the Amulet development boards.
 *
 * @note These definitions were moved to here from haptic_driver.h.
 */
#define haptic_PxDIR P1DIR
#define haptic_PxOUT P1OUT
#define haptic_BITx BIT0

/**
 * LED driver related definitions for the Amulet development boards.
 *
 * @note These definitions were moved to here from led_driver.h.
 */
#define LED1_PxDIR P1DIR
#define LED1_PxOUT P1OUT
#define LED1_BITx BIT0

#define LED2_PxDIR P4DIR
#define LED2_PxOUT P4OUT
#define LED2_BITx BIT6

#define LED1 LED1_BITx
#define LED2 LED2_BITx

// compiles to here

// taken from bsp_setup.c
/* Switches bits and GPIO ports on the MSP-EXP430F5529LP LaunchPad
 *  S1: P2.1, S2: P1.1
 */
#define S1 ((uint8_t)(1U << 1))
#define S2 ((uint8_t)(1U << 1))
#define S1_read() ((uint8_t)(P2IN & S1))
#define S2_read() ((uint8_t)(P1IN & S2))

// TAKEN FROM bsp_uart.c
#define UART_PxSEL1 P2SEL1
#define UART_PxSEL0 P2SEL0
#define UART_PIN0 BIT0
#define UART_PIN1 BIT1

#define UCxxCTLW0_uart UCA0CTLW0
#define UCxxCTL1_uart UCA0CTL1
#define UCxxBR0_uart UCA0BR0
#define UCxxMCTLW_uart UCA0MCTLW
#define UCxxBR1_uart UCA0BR1
#define UCxxCTL1_uart UCA0CTL1
#define UCxxIE_uart UCA0IE
#define UCxxIFG_uart UCA0IFG
#define UCxxTXBUF_uart UCA0TXBUF
#define UCxxRXBUF_uart UCA0RXBUF

// taken from IPC.c
#define IPC_PxOUT P3OUT
#define IPC_PxDIR P3DIR
#define IPC_PxIE P3IE
#define IPC_PxREN P3REN
#define IPC_PxIES P3IES  // P3.0 Lo/Hi edge
#define IPC_PxIFG P3IFG
#define IPC_interrupt_pin BIT0;

#define IPC_OUPUT_TO_M0_PORT P4OUT
#define IPC_OUPUT_TO_M0_DIR P4DIR
#define IPC_OUTPUT_PIN BIT3

#define IPC_P1SELx P1SEL1
#define IPC_P2SELx P2SEL1
#define IPC_P1SEL1_PORT_A BIT6
#define IPC_P1SEL1_PORT_B BIT7
#define IPC_P2SEL1_PORT_A BIT2

// IPC B0 for dev board
#define UCxxCTLW0_spi UCB0CTLW0  // **Initialize USCI state machine**
#define UCxxIE_spi UCB0IE  // Enable USCI_B0 RX Interrupt

#define UCxxIFG_spi UCB0IFG
#define UCxxTXBUF_spi UCB0TXBUF
#define UCxxRXBUF_spi UCB0RXBUF

// taken from display_driver.h

// Ports from Devboard connected to LCD
#define LCD_MOSI_PORT GPIO_PORT_P2  // 1
#define LCD_SCLK_PORT GPIO_PORT_P2
#define LCD_DISP_PORT GPIO_PORT_P1
#define LCD_POWER_PORT GPIO_PORT_P4

#define LCD_SCS_PORT GPIO_PORT_P1  // 2
#define LCD_SCS_PIN GPIO_PIN4

// Pins from Devboard connected to LCD
#define LCD_MOSI_PIN GPIO_PIN5  // 6
#define LCD_MOSI_PIN_FUNCTION GPIO_SECONDARY_MODULE_FUNCTION
#define LCD_SCLK_PIN GPIO_PIN4  // 2
#define LCD_SCLK_PIN_FUNCTION GPIO_SECONDARY_MODULE_FUNCTION
#define LCD_POWER_PIN GPIO_PIN2
#define LCD_DISP_PIN GPIO_PIN3

#define EUSCI_xx_SPI_isBusy EUSCI_A_SPI_isBusy
#define EUSCI_xx_BASE EUSCI_A1_BASE
#define EUSCI_xx_SPI_getInterruptStatus EUSCI_A_SPI_getInterruptStatus
#define EUSCI_xx_SPI_TRANSMIT_INTERRUPT EUSCI_A_SPI_TRANSMIT_INTERRUPT
#define EUSCI_xx_SPI_transmitData EUSCI_A_SPI_transmitData
#define EUSCI_xx_SPI_CLOCKSOURCE_SMCLK EUSCI_A_SPI_CLOCKSOURCE_SMCLK
#define EUSCI_xx_SPI_MSB_FIRST EUSCI_A_SPI_MSB_FIRST
#define EUSCI_xx_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
#define EUSCI_xx_SPI_CLOCKPOLARITY_INACTIVITY_LOW EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW
#define EUSCI_xx_SPI_3PIN EUSCI_A_SPI_3PIN
#define EUSCI_xx_SPI_masterInit EUSCI_A_SPI_masterInit
#define EUSCI_xx_SPI_enable EUSCI_A_SPI_enable

// taken from sdcard_driver.c
// Pins from MSP430 connected to the SD Card
#define SPI_SOMI BIT6  // P2.6
#define SPI_SIMO BIT5  // P2.5
#define SPI_CLK BIT4   // P2.4
#define SD_CS BIT4     // P3.4

#define UCxxIFG_imu UCA1IFG

#define IMU_CS_SEL P1SEL1
#define IMU_CS_OUT P1OUT
#define IMU_CS_DIR P1DIR
#define IMU_CS BIT4

// Ports
#define SPI_SEL P2SEL1
#define SPI_DIR P2DIR
#define SD_CS_SEL P3SEL1
#define SD_CS_OUT P3OUT
#define SD_CS_DIR P3DIR
#define SD_POWER_OUT P4OUT
#define SD_POWER_DIR P4DIR
#define SD_POWER BIT4

// KLQ - What is this?
#define SPI_REN P2REN
#define SPI_OUT P2OUT

#define SPI_CLK_SEL P2SEL1
#define SPI_CLK_DIR P2DIR

//#defines for the diskio.c files
#define UCxxIFG UCA1IFG
#define UCxxTXBUF UCA1TXBUF
#define UCxxSTATW UCA1STATW
#define UCxxRXBUF UCA1RXBUF
#define UCxxCTLW0 UCA1CTLW0
#define UCxxBR0 UCA1BR0
#define UCxxBR1 UCA1BR1

#endif