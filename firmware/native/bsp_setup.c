/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */


#include "qp_port.h"
#include <msp430fr5989.h>
#include "bsp_setup.h"       /* Board Support Package (BSP) */
#include "inc/hw_memmap.h"

#include "driverlibHeaders.h"
#include "stdint.h"
#include "nrf51822.h"
#include "util.h"
#include "core_sensors.h"//for call back to notify button press
#include "FatFS/ff.h"
#include "FatFS/diskio.h"
#include "FatFS/diskio.h"

#include "led_driver.h"
#include "haptic_driver.h"
#include "bsp_init.h"
#include "clocks_driver.h"
#include "CTS/CTS_wolverine.h"
#include "analytics.h"
#include "ADXL362driver/ADXL362.h"
#include "sd_driver.h"

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
#define BSP_SMCLK   8000000UL
#define BSP_ACLK   32768UL
#define msp430_get_smclk_freq() 8000000UL
#define __MSP430_HAS_EUSCI_Ax__
//#define CAPTOUCH
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
  uint16_t script_times[SCRIPT_SIZE] = {5};//in seconds
  uint8_t script_pointer = 0;
  uint16_t current_ticks = 0;
  uint16_t script_events[SCRIPT_SIZE] = {AMULET_BUTTON_TAP_SIG};// check this in the AFT generated apps.h
  uint8_t target_apps_ids[SCRIPT_SIZE] = {100}; // check this in the AFT generated apps.h

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
                                        };// check this in the AFT generated apps.h
  uint8_t target_apps_ids[SCRIPT_SIZE] = {100, 100, 100}; // check this in the AFT generated apps.h
  */
#endif

// The following variables are related to tasks managers
short spiEnabled = 0;
static unsigned char spi_rx_ready_flag=0;
uint16_t length_msg_len = 0;
bool ipc_buffer_ready = false;
bool ipcDataPending=false;

uint8_t circularstart = 0;
uint8_t circularend = 0;

uint16_t timer_ticks = 0;

uint8_t timer_sensor_type = TIMER;
uint8_t acc_sensor_type = ACCELEROMETER;

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
//int16_t acc_buffer[120];

uint32_t amulet_startup_timer;

// BLE variables
int8_t ble_ref_counter = 0;
bool is_bluetooth_paired = false;

// Variables for the battery monitoring and management
#define BATT_CHECK_INTERVAL_SECONDS 60 * 10
uint16_t secs_since_last_batt_reading = 0;

//uint16_t secs_since_last_heartrate = 0;

//external functions declaration for buttons and timers, gp 2/14/2015
//declared in buttons_driver.h, timer_A1.h, timer_A2.h
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





/*..........................................................................*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timerA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) timerA_ISR (void)
#else
#error Compiler not supported!
#endif
{
#ifdef NDEBUG
  //GPIO_setOutputHighOnPin(CORE_PROFILING_PORT,
  //          CORE_PROFILING_PIN);
  //volatile uint8_t i = 0;
  //for (; i < 64; i++) {
  //}
  //GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
  //          CORE_PROFILING_PIN);
  // Test 2
  // GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
  //          CORE_PROFILING_PIN);
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif

#endif
  QF_TICK(&l_timerA_ISR);

  // Timer
  amulet_startup_timer++;

  //Now we no longer have Amulet Timer
  timer_ticks = (timer_ticks>=AMULET_TICKS_PER_SEC) ? AMULET_TICKS_PER_SEC : timer_ticks + 1;

  if(acc_ref_counter > 0){
    int16_t XValue=0, YValue=0, ZValue=0, Temperature=0;
    ADXL362readXYZTData(&XValue, &YValue, &ZValue, &Temperature);
    acc_buffer_x[timer_ticks-1] = XValue;
    acc_buffer_y[timer_ticks-1] = YValue;
    acc_buffer_z[timer_ticks-1] = ZValue;
  }

  // If we got a continuous increasing or decreasing CTS sequence, then send signal to the app
#ifdef CAPTOUCH
  if(is_captouch_enabled()) {
    CTS_getReading();
    if(sliderPos[1]>last_slider_loc){
      slider_up_count++;
      slider_down_count=0;
    } else if(sliderPos[1]<last_slider_loc){
      slider_down_count++;
      slider_up_count=0;
    }
    if(slider_down_count>2){
      slider_down_count = 0;
      sliderSig = AMULET_SLIDER_DOWN_SIG;
      sliderSignal(sliderSig);
    }
    if(slider_up_count>2){
      slider_up_count = 0;
      sliderSig = AMULET_SLIDER_UP_SIG;
      sliderSignal(sliderSig);
      log_user_interaction(UI_EVENT_CAPTOUCH);
    }
    last_slider_loc = sliderPos[1];
  }
#endif
}

/*..........................................................................*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt void unmi_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(UNMI_VECTOR))) unmi_ISR (void)
#else
#error Compiler not supported!
#endif
{
  WDTCTL = (WDTPW | WDTHOLD); /* Stop WDT */
}


// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT2_VECTOR
__interrupt void    PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{

  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif
}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt void    WDT_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) WDT_ISR (void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif
}

// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = RTC_VECTOR
__interrupt void    RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(RTCIV, RTC_RTCOFIFG)) {
  case RTCIV_NONE:      break;
  case RTCIV_RTCRDYIFG: break;
  case RTCIV_RTCTEVIFG:
    __bic_SR_register_on_exit(LPM4_bits);
    #ifdef PROFLILNG
    GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
              CORE_PROFILING_PIN);
    #endif
    __no_operation();
    break;
  case RTCIV_RTCAIFG:  break;             // RTCAIFG
  case RTCIV_RT0PSIFG: break;             // RT0PSIFG
  case RTCIV_RT1PSIFG:                    // RT1PSIFG
      __bic_SR_register_on_exit(LPM3_bits);
      #ifdef PROFLILNG
      GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
                CORE_PROFILING_PIN);
      #endif
      __no_operation();
    break;
  case RTCIV_RTCOFIFG: break;             // RTCOFIFG
  default: break;
  }

}


// trap isr assignation - put all unused ISR vector here
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) PORT1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif

}

//added by Gunnar on 4/1/2015 for use by the haptic buzzer
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER3_A1_VECTOR
__interrupt void TimerA3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER3_A1_VECTOR))) TimerA3_ISR (void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif

  if (TA3IV & TA3IV_TACCR1){
    haptic();
  }

}//end interrupt



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
  //PJDIR |= BIT7;
  //PJOUT &= ~BIT7;

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
  enable_ADC();  // call this function to power up the ADC and thumbwheel, see
                 // ADC_driver.h for details
#endif

  TA0CCR0 = (unsigned)((BSP_ACLK + BSP_TICKS_PER_SEC / 2) / BSP_TICKS_PER_SEC);
  TA0CTL = (TASSEL_1 | MC_1 |
            TACLR);  // set clock source to ACLK, upmode and reset clock divider
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
  ADXL362begin(10);       // Setup SPI protocol, issue device soft reset
  ADXL362beginMeasure();  // Switch ADXL362 to measure mode

// Init CapTouch Sliders
#ifdef CAPTOUCH
  TI_CAPT_Init_Baseline(&slider1);
  TI_CAPT_Update_Baseline(&slider1, 10);
  GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN0);
  GPIO_setAsInputPin(GPIO_PORT_P9, GPIO_PIN0 | GPIO_PIN1);
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
void BSP_terminate(int16_t result) {
  (void)result;
}

/*..........................................................................*/
void QF_onStartup(void) {
  TA0CCTL0 = CCIE; /* enable Timer0 interrupt */
}

/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
uint8_t hr_message_buff_bs[9]  __attribute__ ((section (".noinit")));
uint8_t ble_check_seconds;
char count_str[10];
void QF_onIdle(void) {
#ifdef NDEBUG
  QF_INT_DISABLE();
  // All the task here can be captured
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
          CORE_PROFILING_PIN);
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
  if(is_ble_enabled() && timer_ticks >= AMULET_TICKS_PER_SEC) {
    ble_check_seconds++;
    if(ble_check_seconds > 1) {
      // Read over SPI the HR byte, and the RR-interval (2 bytes)
      nrf51822_select();
      uint8_t hr_msbyte = nrf51822_spi_transfer(255);
      uint8_t hr_lsbyte = nrf51822_spi_transfer(255);
      uint8_t rr_msbyte = nrf51822_spi_transfer(255);
      uint8_t rr_lsbyte =  nrf51822_spi_transfer(255);
      nrf51822_deselect();
      uint16_t hr_val = (hr_msbyte << 8) | hr_lsbyte;
      // Sensor type
      hr_message_buff_bs[0] = HEARTRATE;
      // Data
      hr_message_buff_bs[1] = hr_lsbyte;
      hr_message_buff_bs[2] = hr_msbyte;
      hr_message_buff_bs[3] = rr_lsbyte;
      hr_message_buff_bs[4] = rr_msbyte;
      NotifyApp(hr_message_buff_bs, 9);


      // BLE analytics
      if(hr_val == 0 && is_bluetooth_paired) {
        log_ble_event(BLE_EVENT_DISCONNECT);
        is_bluetooth_paired = false;
      }
      if(hr_val != 0 && !is_bluetooth_paired) {
        log_ble_event(BLE_EVENT_CONNECT);
        is_bluetooth_paired = true;
      }
      ble_check_seconds = 0;
    }
  }

  // Battery voltage check
  if (timer_ticks >= AMULET_TICKS_PER_SEC) {
    secs_since_last_batt_reading++;
    if(secs_since_last_batt_reading > BATT_CHECK_INTERVAL_SECONDS) {
      uint16_t batt_voltage_adc = take_battery_reading();
      log_batt_voltage(batt_voltage_adc);
      secs_since_last_batt_reading = 0;
    }
  }

  //if(P5IN & BIT0){
  //  ADXL362readFIFO(acc_buffer, 120);
  //  NotifyApp(&acc_sensor_type, 30);
    //P5IN &= ~BIT0;
  //}

  if (timer_ticks >= AMULET_TICKS_PER_SEC) {
    #ifdef SCRIPT_EVENTS

      if(script_pointer<SCRIPT_SIZE && script_times[script_pointer]==current_ticks){
        AmuletEvt *bEvt = Q_NEW(AmuletEvt, script_events[script_pointer]);
        QACTIVE_POST(getAppActiveObject(GlobalAppQueueHead, target_apps_ids[script_pointer]), &bEvt->super, NULL);
        script_pointer++;
        #ifdef SCRIPT_EVENTS_GPIO
          GPIO_setOutputHighOnPin(GPIO_PORT_P2,
                  GPIO_PIN3);
          volatile uint8_t ii = 0;
          for (; ii < 64; ii++) {
          }
          GPIO_setOutputLowOnPin(GPIO_PORT_P2,
                  GPIO_PIN3);
        #endif
      }
      current_ticks = current_ticks + 1;
      if(current_ticks>SCRIPT_PERIOD){
        current_ticks = NEXT_STARTING_TICK;
        script_pointer = NEXT_STARTING_POINTER;
      }

    #endif
    NotifyApp(&timer_sensor_type, 0);
    //ADXL362readFIFO(acc_buffer, 120);
    if(acc_ref_counter > 0){
      NotifyApp(&acc_sensor_type, 20);
    }
    //NotifyApp(&acc_sensor_type, 0);
    timer_ticks = 0;
  }

  #ifdef PROFLILNG
  GPIO_setOutputHighOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
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
void Q_onAssert(char const Q_ROM * const file, int line) {
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
