/*****************************************************************************
 * Product: PELICAN crossing example for MSP-EXP430F5529LP LaunchPad
 * Last updated for version 5.3.0
 * Last updated on  2014-04-18
 *
 *                    Q u a n t u m     L e a P s
 *                    ---------------------------
 *                    innovating embedded systems
 *
 * Copyright (C) Quantum Leaps, www.state-machine.com.
 *
 * This program is open source software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Alternatively, this program may be distributed and modified under the
 * terms of Quantum Leaps commercial licenses, which expressly supersede
 * the GNU General Public License and are specifically designed for
 * licensees interested in retaining the proprietary status of their code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact information:
 * Web:   www.state-machine.com
 * Email: info@state-machine.com
 *****************************************************************************/

#include "bsp_setup.h" /* Board Support Package (BSP) */
#include "inc/hw_memmap.h"
#include "qp_port.h"
#include <msp430fr5989.h>

#include "FatFS/diskio.h"
#include "FatFS/diskio.h"
#include "FatFS/ff.h"
#include "core_sensors.h" //for call back to notify button press
#include "driverlibHeaders.h"
#include "nrf51822.h"
#include "stdint.h"
#include "util.h"

#include "ADXL362driver/ADXL362.h"
#include "Gyro/gyro.h"
#include "CTS/CTS_wolverine.h"
#include "analytics.h"
#include "bsp_init.h"
#include "clocks_driver.h"
#include "haptic_driver.h"
#include "led_driver.h"
#include "sd_driver.h"

#include "MPR121driver/MPR121.h"
#include "MPR121driver/MPR121.c"
#include "MPR121driver/msp430fr59xx_i2c.h"
#include "MPR121driver/msp430fr59xx_i2c.c"

//#define NDEBUG
//#define BSP_DEBUG_IPC
//#define PROFLILNG
//#define SCRIPT_EVENTS //Enable this flag to auto send events to the apps

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#include <intrinsics.h>
#elif defined(__GNUC__)
#include <in430.h>
#endif

/* Local-scope objects -----------------------------------------------------*/

/*--------------------------------------------------------------------------*/
#define BSP_SMCLK 8000000UL
#define BSP_ACLK 32768UL
#define msp430_get_smclk_freq() 8000000UL
#define __MSP430_HAS_EUSCI_Ax__
#define CAPTOUCH
#define BATTERYLEVL
// Uncomment to turn on Amulet watchdog timer to force reboot if
// system locks up.
// As of 3/18/16 you have to turn off CAPTOUCH for the watchdog timer to work.
//#define AMULET_WATCHDOG

#ifdef SCRIPT_EVENTS
// TEMPERATURE_APP
#define SCRIPT_PERIOD 10
#define SCRIPT_SIZE 1
#define NEXT_STARTING_TICK 0
#define NEXT_STARTING_POINTER 0
//#define SCRIPT_EVENTS_GPIO
uint16_t script_times[SCRIPT_SIZE] = {5}; // in seconds
uint8_t script_pointer = 0;
uint16_t current_ticks = 0;
uint16_t script_events[SCRIPT_SIZE] = {
    AMULET_BUTTON_TAP_SIG}; // check this in the AFT generated apps.h
uint8_t target_apps_ids[SCRIPT_SIZE] = {
    100}; // check this in the AFT generated apps.h

// FALLDETECTION_APP
/*#define SCRIPT_PERIOD 60
#define SCRIPT_SIZE 3
#define NEXT_STARTING_TICK 0
#define NEXT_STARTING_POINTER 0
//#define SCRIPT_EVENTS_GPIO
uint16_t script_times[SCRIPT_SIZE] = {4,22,50};//in seconds
uint8_t script_pointer = 0;
uint16_t current_ticks = 0;
uint16_t script_events[SCRIPT_SIZE] = {AMULET_BUTTON_TAP_SIG,
                                      AMULET_APPLICATIONS_Q_SIG_START_SIG+0,
                                      AMULET_APPLICATIONS_Q_SIG_START_SIG+4
                                      };// check this in the AFT generated
apps.h
uint8_t target_apps_ids[SCRIPT_SIZE] = {100, 100, 100}; // check this in the AFT
generated apps.h
*/
#endif

// The following variables are related to tasks managers
short spiEnabled = 0;
static unsigned char spi_rx_ready_flag = 0;
uint16_t length_msg_len = 0;
bool ipc_buffer_ready = false;
bool ipcDataPending = false;

uint8_t lastBatteryLevel = 100;
uint16_t batteryCheckCount = 0;

uint8_t circularstart = 0;
uint8_t circularend = 0;

uint16_t timer_ticks = 0;

uint8_t timer_sensor_type = TIMER;
uint8_t acc_sensor_type = ACCELEROMETER;
uint8_t gyro_sensor_type = IMU;

// Variables for CTS
uint8_t last_slider_loc = 0;
uint8_t slider_up_count = 0;
uint8_t slider_down_count = 0;
enum AmuletSignal sliderSig;
extern void sliderSignal(enum AmuletSignal sliderSig);

int16_t acc_buffer_x[BSP_TICKS_PER_SEC];
int16_t acc_buffer_y[BSP_TICKS_PER_SEC];
int16_t acc_buffer_z[BSP_TICKS_PER_SEC];
int8_t acc_ref_counter = 0; 

int16_t gyro_buffer_x[BSP_TICKS_PER_SEC];
int16_t gyro_buffer_y[BSP_TICKS_PER_SEC];
int16_t gyro_buffer_z[BSP_TICKS_PER_SEC];
int8_t gyro_ref_counter = 0;
// int16_t acc_buffer[120];

uint32_t amulet_startup_timer;

// BLE variables
int8_t ble_ref_counter = 0;
bool is_bluetooth_paired = false;

// Variables for the battery monitoring and management
#define BATT_CHECK_INTERVAL_SECONDS 60 * 10
uint16_t secs_since_last_batt_reading = 0;

// uint16_t secs_since_last_heartrate = 0;

// external functions declaration for buttons and timers, gp 2/14/2015
// declared in buttons_driver.h, timer_A1.h, timer_A2.h
extern void button_init(uint8_t);
extern void debounce(uint8_t);
extern void LED1init();
extern void LED2init();
extern void singleBuzz();
extern void doubleBuzz();
// extern uint16_t read_ADC();
extern void adc_init();
extern void enable_ADC();
extern void clocks_init(void);

// The following variables are used for cap touch slide detection on the snail kite
int capLastPos = 0;
int capUpCount = 0;
int capDownCount = 0;
int capSlideThreshold = 2;
uint8_t capTouchStates[12] = {0};

/*..........................................................................*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerA_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(TIMER0_A0_VECTOR))) timerA_ISR(void)
#else
#error Compiler not supported!
#endif
{
#ifdef NDEBUG
  // GPIO_setOutputHighOnPin(CORE_PROFILING_PORT,
  //          CORE_PROFILING_PIN);
  // volatile uint8_t i = 0;
  // for (; i < 64; i++) {
  //}
  // GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
  //          CORE_PROFILING_PIN);
  // Test 2
  // GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
  //          CORE_PROFILING_PIN);
  _bic_SR_register_on_exit(LPM3_bits);
#ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif

#endif
  QF_TICK(&l_timerA_ISR);

  // Timer
  amulet_startup_timer++;

  // Now we no longer have Amulet Timer
  timer_ticks = (timer_ticks >= AMULET_TICKS_PER_SEC) ? AMULET_TICKS_PER_SEC
                                                      : timer_ticks + 1;

  if (acc_ref_counter > 0) {
    int16_t XValue = 0, YValue = 0, ZValue = 0, Temperature = 0;
    ADXL362readXYZTData(&XValue, &YValue, &ZValue, &Temperature);
    acc_buffer_x[timer_ticks - 1] = XValue;
    acc_buffer_y[timer_ticks - 1] = YValue;
    acc_buffer_z[timer_ticks - 1] = ZValue;
  }

  if (gyro_ref_counter > 0) {
    l3gd20Data gyrodata;
    GyroReadXYZData(&gyrodata);
    // Just pass range compensated values of the sensor
    gyro_buffer_x[timer_ticks - 1] = gyrodata.x;
    gyro_buffer_y[timer_ticks - 1] = gyrodata.y;
    gyro_buffer_z[timer_ticks - 1] = gyrodata.z;
  }
    #ifdef BATTERYLEVL
      batteryCheckCount++;
      if (batteryCheckCount > 6000) {

        uint8_t battery_reading = get_battery_level();
        if (lastBatteryLevel!=battery_reading){
          lastBatteryLevel = battery_reading;
          sliderSignal(AMULET_BATTERY_CHANGE_SIG);
        }
        if (battery_reading<20){
          sliderSignal(AMULET_LOW_BATTERY_SIG);

        }
        batteryCheckCount = 0;

      }

    #endif
// If we got a continuous increasing or decreasing CTS sequence, then send
// signal to the app
    #ifdef CAPTOUCH

        #ifdef BSP_WHITE_OWL
            if (is_captouch_enabled()) {
                CTS_getReading();
                if (sliderPos[1] > last_slider_loc) {
                    slider_up_count++;
                    slider_down_count = 0;
                } else if (sliderPos[1] < last_slider_loc) {
                    slider_down_count++;
                    slider_up_count = 0;
                }
                if (slider_down_count > 2) {
                    slider_down_count = 0;
                    sliderSig = AMULET_SLIDER_DOWN_SIG;
                    sliderSignal(sliderSig);
                }
                if (slider_up_count > 2) {
                    slider_up_count = 0;
                    sliderSig = AMULET_SLIDER_UP_SIG;
                    sliderSignal(sliderSig);
                    log_user_interaction(UI_EVENT_CAPTOUCH);
                }
                last_slider_loc = sliderPos[1];
            }
        #endif

        #ifdef BSP_SNAIL_KITE
            /* 1. Read the touch states from the MPR121. States 0-7 are stored *
             *    in touch_states[0] and 8-11 are in touch_states[1].          */
            uint8_t touch_states[2] = {0,0};
            MPR121_Read_Touch_States(0x5A, touch_states);
            uint16_t touched = (touch_states[1] << 8) | touch_states[0];

            /* 2. Loop over the first 12 bits of touched and check to see if *
             *    there have been any changes in the states of the pins.     */
            int i,j = 1;
            for (i=0; i < 12; i++){
                if(touched & j) {                       // Pin i is currently being touched. If this
                    if(capTouchStates[i] == 0){         // is the first time, mark it as touched
                        capTouchStates[i] = 1;
                        if(i > capLastPos) {            // If the current pin is "higher" than the
                            capUpCount++;               // last touched pin, then the user is sliding
                            capDownCount = 0;           // up. Increment up count, reset down count, and
                            capLastPos = i;             // break for loop
                            break;
                        }
                        if(i < capLastPos) {            // If the current pin is "lower" than the
                            capUpCount = 0;             // last touched pin, then the user is sliding
                            capDownCount++;             // down. Increment down count, reset up count,
                            capLastPos = i;             // and break for loop
                            break;
                        }
                    }
                } else{                                 // Pin i is not currently being touched
                    if(capTouchStates[i] == 1){         // Make sure that it is not marked as touched
                        capTouchStates[i] = 0;
                    }
                }
                j = j << 1;                             // Move to the next pin state
            }

            /* 3. The capSlideThreshold determines how many pins need to be     *
             *    touched in a row (either up or down) to constitute a "slide". */
            if(capUpCount >= capSlideThreshold) {
                capUpCount = 0;
                sliderSignal(AMULET_SLIDER_UP_SIG);
            }
            if(capDownCount >= capSlideThreshold) {
                capDownCount = 0;
                sliderSignal(AMULET_SLIDER_DOWN_SIG);
            }
        #endif
    #endif
}

/*..........................................................................*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void unmi_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(UNMI_VECTOR))) unmi_ISR(void)
#else
#error Compiler not supported!
#endif
{
  WDTCTL = (WDTPW | WDTHOLD); /* Stop WDT */
}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(PORT2_VECTOR))) PORT2_ISR(void)
#else
#error Compiler not supported!
#endif
{

  _bic_SR_register_on_exit(LPM3_bits);
#ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif
}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(WDT_VECTOR))) WDT_ISR(void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
#ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif
}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(RTC_VECTOR))) RTC_ISR(void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(RTCIV, RTC_RTCOFIFG)) {
  case RTCIV_NONE:
    break;
  case RTCIV_RTCRDYIFG:
    break;
  case RTCIV_RTCTEVIFG:
    __bic_SR_register_on_exit(LPM4_bits);
#ifdef PROFLILNG
    GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif
    __no_operation();
    break;
  case RTCIV_RTCAIFG:
    break; // RTCAIFG
  case RTCIV_RT0PSIFG:
    break;             // RT0PSIFG
  case RTCIV_RT1PSIFG: // RT1PSIFG
    __bic_SR_register_on_exit(LPM3_bits);
#ifdef PROFLILNG
    GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif
    __no_operation();
    break;
  case RTCIV_RTCOFIFG:
    break; // RTCOFIFG
  default:
    break;
  }
}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR(void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
#ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT4_VECTOR
__interrupt void PORT4_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(PORT4_VECTOR))) PORT4_ISR(void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
#ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif
}

// added by Gunnar on 4/1/2015 for use by the haptic buzzer
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER3_A1_VECTOR
__interrupt void TimerA3_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(TIMER3_A1_VECTOR))) TimerA3_ISR(void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
#ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif

  if (TA3IV & TA3IV_TACCR1) {
    haptic();
  }

} // end interrupt

void white_owl_initialize_pins() {
  // Init all pins to a known state
  // Everything as an input, with a pulldown resistor first

  // Port 1 low power
  P1DIR = 0;
  P1OUT = 0;
  P1REN = 0xFF;

  // Port 2 low power
  P2DIR = 0;
  P2OUT = 0;
  P2REN = 0xFF;

  // Port 3 low power
  P3DIR = 0;
  P3OUT = 0;
  P3REN = 0xFF;

  // Port 4 low power
  P4DIR = 0;
  P4OUT = 0;
  P4REN = 0xFF;

  // Port 5 low power
  P5DIR = 0;
  P5OUT = 0;
  P5REN = 0xFF;

  // Port 9 low power
  P9DIR = 0;
  P9OUT = 0;
  P9REN = 0xFF;

  // Port J low power
  PJDIR = 0;
  PJOUT = 0;
  PJREN = 0xFF;

  // Buttons to pull-up so they dont draw anything when not pressed
  button0_PxOUT |= button0_BITx;
  button0_PxREN |= button0_BITx;
  button1_PxOUT |= button1_BITx;
  button1_PxREN |= button1_BITx;

  // Set SOMI to output and low
  SPI_DIR |= SPI_SOMI;
  SPI_OUT &= ~SPI_SOMI;

  // Set SIMO to output and low
  SPI_DIR |= SPI_SIMO;
  SPI_OUT &= ~SPI_SIMO;

  // Set CLK to output and low
  SPI_CLK_DIR |= SPI_CLK;
  SPI_CLK_OUT &= ~SPI_CLK;

  // Display deassert
  // PJDIR |= BIT7;
  // PJOUT &= ~BIT7;

  // Delay to settle
  __delay_cycles(1000000);

  // Turn SD off
  SD_POWER_DIR |= SD_POWER;
  SD_POWER_OUT |= SD_POWER;

  // SD CS low (so no power)
  SD_CS_DIR |= SD_CS;
  SD_CS_OUT &= ~SD_CS;

  // BLE turn off
  IPC_PWR_PxDIR |= IPC_PWR_BITx;
  IPC_PWR_PxOUT |= IPC_PWR_BITx;

  // BLE SPI pins output and low
  IPC_SPI_DIR |= IPC_SPI_CLK | IPC_SPI_SOMI | IPC_SPI_SIMO;
  IPC_SPI_OUT &= ~(IPC_SPI_CLK | IPC_SPI_SOMI | IPC_SPI_SIMO);

  // IPC Chip select
  IPC_CS_PxDIR |= IPC_CS_BITx;
  IPC_CS_PxOUT &= ~IPC_CS_BITx;

  // Gyro CS low
  GYRO_CS_PxDIR |= GYRO_CS_BITx;
  GYRO_CS_PxOUT &= ~GYRO_CS_BITx;

  // Gyro turn off
  GYRO_ENABLE_PxDIR |= GYRO_ENABLE_BITx;
  GYRO_ENABLE_PxOUT |= GYRO_ENABLE_BITx;

  // XL CS deselect
  ACCEL_CS_PxDIR |= ACCEL_CS_BITx;
  ACCEL_CS_PxOUT |= ACCEL_CS_BITx;

  // Turn off Mic
  AUDIO_ADC_PXDIR |= AUDIO_ADC_INPUT_BITX;
  AUDIO_ADC_PXOUT &= ~AUDIO_ADC_INPUT_BITX;

  // Delay to settle
  __delay_cycles(100000);

  // Turn off the temp
  TEMP_POWER_DIR |= TEMP_POWER_BIT;
  TEMP_POWER_OUT &= ~TEMP_POWER_BIT;

  // Delay to settle
  __delay_cycles(100000);

  // Turn off the light sensor
  LIGHT_POWER_DIR |= LIGHT_POWER_BIT;
  LIGHT_POWER_OUT &= ~LIGHT_POWER_BIT;

  // Delay to settle
  __delay_cycles(100000);
}

void snail_kite_initialize_pins() {
  // Gyro power on
  P1DIR |= BIT4;
  P1OUT |= BIT4;

  // Gyro CS high
  P2DIR |= BIT5;
  P2OUT |= BIT5;

  PJSEL0 &= ~(BIT7 | BIT6);
  PJSEL1 &= ~(BIT7 | BIT6);

  // XL pwr off
  P4DIR |= BIT3;
  P4OUT &= ~BIT3;

  __delay_cycles(10000);

  // XL pwr on
  P4DIR |= BIT3;
  P4OUT |= BIT3;

  // XL CS high
  PJDIR |= BIT6;
  PJOUT |= BIT6;

  // Turn SD off
  SD_POWER_DIR |= SD_POWER;
  SD_POWER_OUT |= SD_POWER;
}

/*..........................................................................*/
/*
 * To run display make sure to:
 *   - uncomment BSP_display_init(), and
 *   - comment out setupIPC().
 */
void BSP_init(void) {
  WDTCTL = (WDTPW | WDTHOLD); /* stop the watchdog timer */

// This is platform hardware dependant, and is included by bsp_init.h
#ifdef BSP_SNAIL_KITE
  snail_kite_initialize_pins();
#endif
#ifdef BSP_WHITE_OWL
  white_owl_initialize_pins();
#endif

  PM5CTL0 &= ~LOCKLPM5;

  // Clear the log counters
  init_analytic_logs();

  // We may be here because we crashed
  increment_crash_counter();

  clocks_init();

  // configure the buttons for input, added by gunnar 2/14/2015
  button_init(BUTTON0);
  button_init(BUTTON1);

  // enable the haptic buzzer
  haptic_init();

  // Set the timer
  amulet_startup_timer = 0;

// Only use the ADC with the bracelet for now, the devboard IO's are all taken
// up
#ifndef BSP_DEV
  adc_init();
  enable_ADC(); // call this function to power up the ADC and thumbwheel, see
                // ADC_driver.h for details
#endif

  TA0CCR0 = (unsigned)((BSP_ACLK + BSP_TICKS_PER_SEC / 2) / BSP_TICKS_PER_SEC);
  TA0CTL = (TASSEL_1 | MC_1 |
            TACLR); // set clock source to ACLK, upmode and reset clock divider
  TA0CTL &= ~TAIFG;
  __no_operation();

  // Enable RTC(real time clock)
  RTC_C_holdClock(__MSP430_BASEADDRESS_RTC_C__);

  // Set the RTC Time using macros from Makefile / Ruby
  RTCSEC = RTCSEC_DEF;
  RTCMIN = RTCMIN_DEF;
  RTCHOUR = RTCHOUR_DEF;
  RTCDOW = RTCDOW_DEF;
  RTCDAY = RTCDAY_DEF;
  RTCMON = RTCMONTH_DEF;
  RTCYEAR = RTCYEAR_DEF;

  RTC_C_enableInterrupt(__MSP430_BASEADDRESS_RTC_C__,
                        RTC_C_TIME_EVENT_INTERRUPT);
  RTC_C_setCalendarEvent(__MSP430_BASEADDRESS_RTC_C__,
                         RTC_C_CALENDAREVENT_MINUTECHANGE);
  RTC_C_startClock(__MSP430_BASEADDRESS_RTC_C__);

  // Set directions of LEDs and turn them off
  LED1init();
  LED2init();
  turn_LED1_Off();
  turn_LED2_Off();

  // Uncomment to test out sleep mode
  //__bis_SR_register(LPM3_bits);
  //__no_operation();                         // For debugger

  // Initiliaze SD card power pin and SPI pins
  SDinit();

  // Init the display, set the default font
  // DO NOT DO ANYTHING TO DISPLAY TILL BUFFER IS SET IN CoreUIInit
  // Display has a pointer to upcomgin app displaybuffer address
  display_begin();
  setFont(SourceSansProBold7);

  // int16_t XValue, YValue, ZValue, Temperature;
  ADXL362begin(10);      // Setup SPI protocol, issue device soft reset
  ADXL362beginMeasure(); // Switch ADXL362 to measure mode

  // Gyro, init into power down mode 
  // Same bus as XL so SPI is already init
  GyroBegin();

// Init CapTouch Sliders
#ifdef CAPTOUCH
    #ifdef BSP_WHITE_OWL
        TI_CAPT_Init_Baseline(&slider1);
        TI_CAPT_Update_Baseline(&slider1, 10);
        GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setAsInputPin(GPIO_PORT_P9, GPIO_PIN0 | GPIO_PIN1);
    #endif

    #ifdef BSP_SNAIL_KITE
        P2DIR &= ~BIT4;                                 // Configure P2.4 as input for MPR IRQ pin
        P2OUT |= BIT4;                                  // Configure pull-up resistor on P2.4
        P2REN |= BIT4;                                  // Enable pull-up res on P2.4
        P1SEL0 |= BIT6 | BIT7;                          // Configure I2C pins P1.6 = SDA P1.7 = SCL
        P1SEL1 &= ~(BIT6 | BIT7);                       // Configure I2C pins P1.6 = SDA P1.7 = SCL
        I2C_Init(UCSSEL__SMCLK, 64);

        MPR121_Init(0x5A);
    #endif
#endif

#ifdef PROFLILNG
// PROFLILNG_DIR |= PROFLILNG_PIN;
// P2DIR |= BIT3;//App profiling
#endif

#ifdef SCRIPT_EVENTS
  current_ticks = 0;
  script_pointer = 0;
#endif

// Enable the watchdog.  Clearing the watchdog happens in QF_onIdle().
// What the settings mean:
//    WDTSSEL = leaving this unset (zero) puts watchdog into watchdog mode
//    instead of timer mode
//    WDTPW = is the password required to make changes to the watchdog
//    WDTSSEL__SMCLK = selects the SMCLK as the clock source
//    WDTCNTCL = clears the watchdog timer
//    WDTIS__128M sets the watchdog timeout to 128M clock cycles
//
// Our SMCLK is running at 16Mhz.
// So at 16MHz (16,000,000Hz) the possible settings are:
//
// WDTIS_2G    2147483648/16MHz = 134.2 seconds
// WDTIS_128M  134217728/16MHz = 8.39 seconds
// WDTIS_8192K 8388608/16MHz = 0.524 seconds
// WDTIS_512K  524288/16MHz =  32.768 milliseconds
// WDTIS_32K   32768/16MHz =  2.048 milliseconds
// WDTIS_8192  8192/16MHz = 0.512 milliseconds
// WDTIS_512   512/16MHz = 32 microseconds
// WDTIS_64    64/16000000 = 4 microseconds
// These WDTIS__ #defines come from
// driverlib/MSP430FR5xx_6xx/deprecated/CCS/msp430xgeneric.h

// 8 seconds seems like a good watchdog interval, we should call the idle loop
// faster than that.
#ifdef AMULET_WATCHDOG
  WDTCTL = WDTPW | WDTSSEL__SMCLK | WDTCNTCL | WDTIS__128M;
#endif
  // The watchdog is now cleared and counting from zero.

  // Enable interrupts.  This starts everything going.
  __bis_SR_register(GIE);
}

/*..........................................................................*/
void BSP_terminate(int16_t result) { (void)result; }

/*..........................................................................*/
void QF_onStartup(void) { TA0CCTL0 = CCIE; /* enable Timer0 interrupt */ }

/*..........................................................................*/
void QF_onCleanup(void) {}

/*..........................................................................*/
uint8_t hr_message_buff_bs[22] __attribute__((section(".noinit")));
uint8_t ble_check_seconds;
char count_str[10];
//uint8_t toggle=0;

void QF_onIdle(void) {
#ifdef NDEBUG
  QF_INT_DISABLE();
// All the task here can be captured
#ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif

// Clear the watchdog counter to prevent a reboot.
// To test the watchdog comment out this line and the Amulet
// should reboot once a second (there is a one second delay at the
// start of main()).

#ifdef AMULET_WATCHDOG
  WDTCTL = WDTPW + WDTCNTCL; // WDT CNT is cleared and counting from 0
#endif

  // enable interrupts, go into lpm, will come back after timer elapses

  // Test 2: used to show we are in the onIdle function, use scope to measure.
  // GPIO_setOutputHighOnPin(CORE_PROFILING_PORT,
  //          CORE_PROFILING_PIN);

  // New IPC, any byte just asks for recent heartrate data (rx three bytes)
  // Using polling for now since dont have an interrupt pin.
  // Only check once a second
  if (is_ble_enabled() && timer_ticks >= AMULET_TICKS_PER_SEC) {
    ble_check_seconds++;
    if (ble_check_seconds >= 1) {
      uint8_t it;
      uint8_t length=0;

// This sets the SPI buffer size for received data
#define SPI_RX_BUF_SIZE 10
      uint8_t packet[SPI_RX_BUF_SIZE];
// This sets the size for buffer for parsed HRM data.  The data will
// consist of uint16_t's with HR first, then zero or more RRI values.
#define HRM_MAX_DATA_SIZE 10
      uint16_t hrm_data[HRM_MAX_DATA_SIZE];

      // RON: Right now the only thing that comes via IPC from the
      // RON: radio is HRM packets.  Soon we'll have to handle other
      // RON: sensor types though.  So will need to use the CS line,
      // RON: and double buffering on the radio, and add # packets,
      // RON: and sensor type to be able to handle multiple packets
      // RON: being transferred.  This code will then parse multiple
      // RON: types of radio packets.

      // Read over SPI the HR and RR-interval data.
      // The current packet forwarding format is:
      // uint8_t length
      // uint8_t data[length]

      // Turn on the chip select line to the radio.
      // This lets the radio know it should not write data into
      // the SPI buffer now because a transfer is about to take
      // place.

      nrf51822_select();

      // The first byte will be the length of the HRM packet to follow.
      // SPI is bidirectional so we're sending a 255 back.  Eventually
      // we'll send a command instead telling the radio to send data.

//#if 0
      //if(toggle == 0)
      //{
      //   toggle = 1;
      //   turn_LED1_On();
      //   turn_LED2_On();
      //} else
      //{
      //   toggle = 0;
      //   turn_LED1_Off();
      //   turn_LED2_Off();
      //}
      length = nrf51822_spi_transfer(255);
      if((length > 0) && (length < SPI_RX_BUF_SIZE)) // Sanity check
      {
         // Read the rest of the HRM packet

         for(it=0; it<length; it++)
         {
            // Currently we're sending 255's back to the radio at the same
            // time as we're reading data since SPI is bidirectional.

            packet[it] = nrf51822_spi_transfer(255);
         }
      }

      // Turn off chip select line to the radio.

      nrf51822_deselect();

      // Parse the HRM packet
      // The format of the data returned by the parser is 10 uint16_t numbers:
      // uint16_t HR
      // uint16_t energyExpended
      // uint16_t RRI[7]
      // uint16_t sensorContact
//#if 0
      length = parseHeartRateAndRRI(packet, length, hrm_data);

      // length is the number of RRI, rest of fields are fixed.
      // hrm_data[0] is always HR
      // hrm_data[1] is always energyExpended
      // # RRI is variable, unused slots will be zero
      // hrm_data[9] is always sensorContact

      // Assemble the HRM data into a message to the apps that have
      // subscribed to the data.

      // How many RRI values might there be per HR measurement?
      // Assume an extreme HR of 300 bpm (user soon to be dead!)
      // The reporting rate from the HRM is once per second.  So that's
      // (300bpm/(60s/m)) * 1s = 5 beats
      // So there can be up to four RRI intervals per message.  At a
      // normal bpm of around 60bpm there is usually only 1 RRI, sometimes
      // two.  But when running or excited there may be 2 or 3 RRI.

      // RON: Need to increase the size of hr_message_buff_bs to handle
      // RON: more RRI values.  Say up to 3 though I've only seen up to
      // RON: two with the Polar.
      // RON: What to do with the other data from HRM?  Energy expended and
      // RON: sensor contact.  I'm changing the whole data format so should
      // RON: leave the first part the same for now so I don't break the apps
      // RON: which would make testing easier, or do I break it and add data
      // RON: in format parser returns it, or reformat it, or reformat it in
      // RON: the parser?  The apps have to assemble uint16_t's from bytes?
      // RON: Or is do they see 16bit numbers?
      // RON: For now I'm trying not to break the apps.

      // Apps will see HRM event data in this format:
      // uint8_t HEARTRATE event (do apps even see this?  Probably just event type)
      // uint16_t HR
      // uint16_t RRI[7]
      // uint16_t energyExpended
      // uint16_t sensorContact
      //
      // Previously it was just HR then one RRI, so apps that expect that
      // will still work.  RRI can be zero.

      // Add sensor type
      hr_message_buff_bs[0] = HEARTRATE;

      // Add the data, we are packing uint16_t's into uint8_t's here.
      // I'm keeping the format identical to the old format for the first
      // 5 bytes so apps don't have to change, but can use the extra data
      // if they want to.

      // Add the HR
      hr_message_buff_bs[1] = (uint8_t)(hrm_data[0] & 0x00FF);
      hr_message_buff_bs[2] = (uint8_t)((hrm_data[0] & 0xFF00) >> 8);

      // Add the RRI's.  hrm_data[1] is energyExpended so start at index 2.
      for(it=3; it<=17; it=it+2) // Copy 14 bytes into 7 uint16_t's.
      {
         hr_message_buff_bs[it] = (uint8_t)(hrm_data[2+((it-2)/2)] & 0x00FF);
         hr_message_buff_bs[it+1] = (uint8_t)((hrm_data[2+((it-2)/2)] & 0xFF00) >> 8);
      }

      // Add the expendedEnergy field
      hr_message_buff_bs[18] = (uint8_t)(hrm_data[1] & 0x00FF);
      hr_message_buff_bs[19] = (uint8_t)((hrm_data[1] & 0xFF00) >> 8);

      // Add the sensorContact field
      hr_message_buff_bs[20] = (uint8_t)(hrm_data[9] & 0x00FF);
      hr_message_buff_bs[21] = (uint8_t)((hrm_data[9] & 0xFF00) >> 8);

      // Send the data to the apps as an event.

      NotifyApp(hr_message_buff_bs, 22);
//#endif
      // BLE analytics, if getting data then we must have a connection.
      // If not getting data then the connection was dropped.  So we can
      // infer the connection state from the data, however this will soon
      // be replaced with explicit CONNECT and DISCONNECT messages sent
      // from the radio.  For now, this still works.

      if (hrm_data[0] == 0 && is_bluetooth_paired) {
        log_ble_event(BLE_EVENT_DISCONNECT);
        is_bluetooth_paired = false;
      }
      if (hrm_data[0] != 0 && !is_bluetooth_paired) {
        log_ble_event(BLE_EVENT_CONNECT);
        is_bluetooth_paired = true;
      }
      ble_check_seconds = 0;
    }
  }

  // Battery voltage check
  if (timer_ticks >= AMULET_TICKS_PER_SEC) {
    secs_since_last_batt_reading++;
    if (secs_since_last_batt_reading > BATT_CHECK_INTERVAL_SECONDS) {
      uint16_t batt_voltage_adc = take_battery_reading();
      log_batt_voltage(batt_voltage_adc);
      secs_since_last_batt_reading = 0;
    }
  }

  // if(P5IN & BIT0){
  //  ADXL362readFIFO(acc_buffer, 120);
  //  NotifyApp(&acc_sensor_type, 30);
  // P5IN &= ~BIT0;
  //}

  if (timer_ticks >= AMULET_TICKS_PER_SEC) {
#ifdef SCRIPT_EVENTS

    if (script_pointer < SCRIPT_SIZE &&
        script_times[script_pointer] == current_ticks) {
      AmuletEvt *bEvt = Q_NEW(AmuletEvt, script_events[script_pointer]);
      QACTIVE_POST(getAppActiveObject(GlobalAppQueueHead,
                                      target_apps_ids[script_pointer]),
                   &bEvt->super, NULL);
      script_pointer++;
#ifdef SCRIPT_EVENTS_GPIO
      GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);
      volatile uint8_t ii = 0;
      for (; ii < 64; ii++) {
      }
      GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN3);
#endif
    }
    current_ticks = current_ticks + 1;
    if (current_ticks > SCRIPT_PERIOD) {
      current_ticks = NEXT_STARTING_TICK;
      script_pointer = NEXT_STARTING_POINTER;
    }

#endif
    NotifyApp(&timer_sensor_type, 0);
    // ADXL362readFIFO(acc_buffer, 120);
    if (acc_ref_counter > 0) {
      NotifyApp(&acc_sensor_type, 20);
    }
    if (gyro_ref_counter > 0) {
      NotifyApp(&gyro_sensor_type, 20);
    }
    // NotifyApp(&acc_sensor_type, 0);
    timer_ticks = 0;
  }

#ifdef PROFLILNG
  GPIO_setOutputHighOnPin(CORE_PROFILING_PORT, CORE_PROFILING_PIN);
#endif

  QF_INT_ENABLE();
  __bis_SR_register(LPM3_bits);
  __no_operation();
  PM5CTL0 &= ~LOCKLPM5;
#else
  QF_INT_ENABLE();
#endif
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM *const file, int line) {
  (void)file;       /* avoid compiler warning */
  (void)line;       /* avoid compiler warning */
  QF_INT_DISABLE(); /* make sure that interrupts are locked */
}

/*****************************************************************************
 * NOTE1:
 * The MSP430 interrupt processing restores the CPU status register upon
 * exit from the ISR, so if any low-power mode has been set before the
 * interrupt, it will be restored upon the ISR exit. This is not what
 * you want in QP, because it will stop the CPU. Therefore, to prevent
 * this from happening, the macro __low_power_mode_off_on_exit() clears
 * any low-power mode in the *stacked* CPU status register, so that the
 * low-power mode won't be restored upon the ISR exit.
 */
