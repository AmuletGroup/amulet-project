/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 * buttons_driver.c
 *
 * written by Gunnar Pope 02/05/2015
 * charles.g.pope.th@dartmouth.edu
 *
 */


/*
 * This buttons driver is used to initialize two buttons, defined
 * in the "buttons_driver.h" file. When pressed, buttons are then debounced
 * with timers timerA1 and timerA2, which must have interrupts defined
 * in the bsp_setup.c file.
 *
 * All Ports and Pins used to configure the butons are defined in the buttons_driver.h file.
 * To alter which button ports are used for the switches,
 * you only have to modify the #define statements in the buttons_driver.h file
 *
 * the Button Hold and Button Tap signals are published to applications/UI service
 * by the first two functions listed below, generateButtonHoldSig() and
 * generateButtonsTapSig().
 *
 */

#include <msp430.h>
#include <msp430fr5989.h>
#include <stdint.h>
#include "buttons_driver.h"
#include "bsp_init.h"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#include <intrinsics.h>
#elif defined(__GNUC__)
#include <in430.h>
#endif

// buttonSig is passed to core layer to generate button pressed event
extern void buttonPressedSignal(enum AmuletSignal buttonSig, int buttonId);
enum AmuletSignal buttonSig;
static state_t button0_state = waiting;
static state_t button1_state = waiting;

uint8_t is_captouch_enabled_bool = 0;

// external functions defined in the timerA1.h and timerA2.h files
extern void timerA1_init();
extern void timerA2_init();
extern void timerA1_addCount();
extern void timerA1_disable();
extern void timerA1_resetCount();
extern void timerA2_addCount();
extern void timerA2_disable();
extern void timerA2_resetCount();
extern uint16_t timerA1_getCount();
extern uint16_t timerA2_getCount();

uint8_t is_captouch_enabled() { return is_captouch_enabled_bool; }
void turnOnCapTouch() { is_captouch_enabled_bool = 1; }
void turnOffCapTouch() { is_captouch_enabled_bool = 0; }

/*
 * ********************************************************************************
 * Start of Button Hold/Tap Event functions
 * These two functions are used to notify the core that a button event has
 * occured.
 * ******************************************************************************
 */

void generateBothHoldSig() {
  buttonSig = AMULET_BOTH_HOLD_SIG;
  buttonPressedSignal(buttonSig, BUTTON0);
}

void generateButtonHoldSig(uint8_t button) {
  switch (button) {
    case BUTTON0:
      // notify the apps
      buttonSig = AMULET_BUTTON_HOLD_SIG;

      // send a button pressed signal to the AMULET CORE layer
      buttonPressedSignal(buttonSig, BUTTON0);
      break;
    case BUTTON1:
      // send a button pressed signal to the AMULET CORE layer
      buttonSig = AMULET_BUTTON_HOLD_SIG;

      buttonPressedSignal(buttonSig, BUTTON1);
      break;
    default:
      break;
  }
}

/*
 * Generate the Button Hold Event to notify the apps/UI serice here
 *
 */
void generateButtonTapSig(uint8_t button) {
  switch (button) {
    case BUTTON0:
      // send a button pressed signal to the AMULET CORE layer
      buttonSig = AMULET_BUTTON_TAP_SIG;
      buttonPressedSignal(buttonSig, BUTTON0);
      break;
    case BUTTON1:
      // send a button pressed signal to the AMULET CORE layer
      buttonSig = AMULET_BUTTON_TAP_SIG;
      buttonPressedSignal(buttonSig, BUTTON1);

      break;
    default:
      break;
  }
}

/*
 * ********************************************************************************
 * End of Button Hold/Tap Event functions
 * ******************************************************************************
 */

// initialize a button
void button_init(uint8_t button) {
  switch (button) {
    case BUTTON0:

      // configure the interrupt button as input and config interrupt
      button0_PxDIR &= ~button0_BITx;  // configure port as input
      button0_PxSEL0 &= ~button0_BITx;  // set PxCEL0 = 0 for GPIO setting
      button0_PxSEL1 &= ~button0_BITx;  // set PxCEL1 = 0 for GPIO setting
      button0_PxREN |= button0_BITx;  // enable the pullup resistor
      button0_PxOUT |= button0_BITx;  // select the pull up resistor
      button0_PxIES |= button0_BITx;  // enable interrupt trigger on rising edge
      PM5CTL0 &=
          ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode
      button0_PxIE |= button0_BITx;  // enable the interrupt
      button0_PxIFG &= ~button0_BITx;  // clear pending interrupts
      // end enable GPIOs
      break;

    case BUTTON1:

      // configure the interrupt button as input and config interrupt
      button1_PxDIR &= ~button1_BITx;  // configure port as input
      button1_PxSEL0 &= ~button1_BITx;  // set PxCEL0 = 0 for GPIO setting
      button1_PxSEL1 &= ~button1_BITx;  // set PxCEL1 = 0 for GPIO setting
      button1_PxREN |= button1_BITx;  // enable the pullup resistor
      button1_PxOUT |= button1_BITx;  // select the pull up resistor
      button1_PxIES |= button1_BITx;  // enable interrupt trigger on rising edge
      PM5CTL0 &=
          ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode
      button1_PxIE |= button1_BITx;  // enable the interrupt
      button1_PxIFG &= ~button1_BITx;  // clear pending interrupts
      break;
    default:
      break;
  }  // end switch
}

// pass the addr of button to be initialized
void button_set_state(uint8_t button, state_t state) {
  switch (button) {
    case BUTTON0:
      button0_state = state;
      break;
    case BUTTON1:
      button1_state = state;
      break;
    default:
      break;
  }
}

// debounce button
void debounce(uint8_t button) {
  switch (button) {
    case BUTTON0:
      button0_PxIE &= ~button0_BITx;  // disable the interrupt
      button0_PxIFG &= ~button0_BITx;  // clear port 4 interrupt flags

      timerA1_init();
      break;

    case BUTTON1:
      button1_PxIE &= ~button1_BITx;  // disable the interrupt
      button1_PxIFG &= ~button1_BITx;  // clear port 4 interrupt flags

      timerA2_init();
      break;

    default:
      break;
  }
}

void checkIfDebounced(uint8_t button) {
  switch (button) {
    case BUTTON0:

      // read the register, is button still down (LOW-TRUE)?
      if ((button0_PxIN & button0_BITx) == 0) {
        // this is where you generate the QP signal that the button has been
        // pressed******//
        button_set_state(BUTTON0, button_pressed);
        // generateButtonTapSig(BUTTON0);

        // this is where you generate the QP signal that the button has been
        // pressed******//

        button0_PxIE &= ~button0_BITx;  // disable interrupts
        button0_PxIES &= ~button0_BITx;  // enable interrupt trigger, L->H
        // enable interrupts
        button0_PxIE |= button0_BITx;  // enable the interrupt
        // button0_PxIFG  = 0; 			//clear port 4 interrupt flags

      }

      else {
        // ignore it, stop the timer and return to waiting
        button_set_state(BUTTON0, waiting);
        timerA1_disable();

        // set interrupt to rising edge
        button0_PxIE &= ~button0_BITx;
        button0_PxIES |= button0_BITx;  // enable interrupt trigger, H->L
        // enable interrupts
        button0_PxIE |= button0_BITx;  // enable the interrupt
        // button0_PxIFG = 0; 			//clear port 4 interrupt flags
      }
      break;  // end button0 case

    case BUTTON1:

      // read the register, is button still down (LOW-TRUE)?
      if ((button1_PxIN & button1_BITx) == 0) {
        button_set_state(BUTTON1, button_pressed);

        button1_PxIE &= ~button1_BITx;  // disable interrupts
        button1_PxIES &= ~button1_BITx;  // enable interrupt trigger, L->H
        // enable interrupts
        button1_PxIE |= button1_BITx;  // enable the interrupt
        // button1_PxIFG  = 0; 			//clear port 4 interrupt flags

      } else {
        // ignore it, stop the timer and return to waiting
        button_set_state(BUTTON1, waiting);
        timerA2_disable();

        // set interrupt to rising edge
        button1_PxIE &= ~button1_BITx;
        button1_PxIES |= button1_BITx;  // enable interrupt trigger, H->L
        // enable interrupts
        button1_PxIE |= button1_BITx;  // enable the interrupt
        // button1_PxIFG = 0; 			//clear port 4 interrupt flags
      }
      break;  // end button1 case

    default:
      break;
  }
}

state_t getState(uint8_t button) {
  switch (button) {
    case BUTTON0:
      return (button0_state);
    case BUTTON1:
      return (button1_state);
    default:
      return (waiting);  // TODO: make default more robust
  }
}

// Interrupt handler for port 3.0 letting MSP430 know data is coming in from M0
// For integration with the Amulet core, this is an example of how to receive
// an IPC message from the M0.

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT3_VECTOR))) PORT_3 (void)
#else
#error Compiler not supported!
#endif
{

  //void CoreClock(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* min, uint8_t* sec, uint8_t* weekday){
  //uint16_t year = RTCYEAR; uint8_t month = RTCMON; uint8_t day = RTCDAY;
  //uint8_t hour = RTCHOUR; uint8_t min = RTCMIN; uint8_t sec = RTCSEC; uint8_t weekday = RTCDOW;
  //bsp_printf("%d:%d:%d, %d, %d-%d-%d\n\r",hour,min,sec,weekday, month,day,year);
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif
  uint16_t length = 0;

  switch (__even_in_range(P3IV, P3IV_P3IFG7)) {
  case P3IV_P3IFG0: break;                    // P2.0 Interrupt
  case P3IV_P3IFG1: break;                          // Option Select Interrupt
  case P3IV_P3IFG2:
    //enter a switch to find the button state, and debounce
    switch(getState(BUTTON0)){
      case button_disabled:
        button_set_state(BUTTON0, waiting);
        button0_PxIE  &= ~button0_BITx;   //disable interrupt flag
        button0_PxIES |=  button0_BITx;   //enable interrupt trigger, H->L
        button0_PxIE  |=  button0_BITx;   //enable the interrupt
        button0_PxIFG &= ~button0_BITx;   //clear port 4 interrupt flags
        timerA1_disable();
        break;
      case waiting:
        //start a timer to time the debounce sequence
        debounce(BUTTON0);
        break;
      case button_pressed:

        if (timerA1_getCount() < 48){
            generateButtonTapSig(BUTTON0);
            // Analytics
            log_user_interaction(UI_EVENT_BUTTON0_TAP);
          }
        else if(getState(BUTTON1) == button_pressed)
        {
          generateBothHoldSig();
          button_set_state(BUTTON1, button_disabled);
          // Analytics 
          log_user_interaction(UI_EVENT_BOTH_HOLD);
        }
        else
          {
            generateButtonHoldSig(BUTTON0);
            // Analytics 
            log_user_interaction(UI_EVENT_BUTTON0_HOLD);
          }

        timerA1_resetCount();

        //buttons pins and ports are defined in buttons_driver.h
        button_set_state(BUTTON0, waiting);
        button0_PxIE  &= ~button0_BITx;   //disable interrupt flag
        button0_PxIES |=  button0_BITx;   //enable interrupt trigger, H->L
        button0_PxIE  |=  button0_BITx;   //enable the interrupt
        button0_PxIFG &= ~button0_BITx;   //clear port 4 interrupt flags
        timerA1_disable();


        break;

      default:
        break;
      }//end button switch
    break;
  case P3IV_P3IFG3: break;                    // P2.3 Interrupt
  case P3IV_P3IFG4: break;                    // P2.4 Interrupt
  case P3IV_P3IFG5: break;                    // P2.5 Interrupt
  case P3IV_P3IFG6: break;                    // P2.6 Interrupt
  case P3IV_P3IFG7: break;                    // P2.7 Interrupt
  default: break;
  }


}

// Add by Tianlong 11/8/2014
/**********************************************************************//**
 * @brief  Port 4 ISR for Left Switch (S2)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT4_VECTOR))) PORT4_ISR (void)
#else
#error Compiler not supported!
#endif
{
  _bic_SR_register_on_exit(LPM3_bits);
  #ifdef PROFLILNG
  GPIO_setOutputLowOnPin(CORE_PROFILING_PORT,
            CORE_PROFILING_PIN);
  #endif

    //#ifdef BSP_DEV

    switch (__even_in_range(P4IV, P4IV_P4IFG7)) {
    case P4IV_P4IFG0: break;                    // P4.0 Interrupt
    case P4IV_P4IFG1: break;                    // P4.1 Interrupt
    case P4IV_P4IFG2: break;                    // P4.2 Interrupt
    case P4IV_P4IFG3:                     // P4.3 Interrupt
    //NotifyAppsButtonPressed(button0);



    switch(getState(BUTTON1)){
      case button_disabled:
        button_set_state(BUTTON1, waiting);
        button1_PxIE  &= ~button1_BITx;   //disable interrupt flag
        button1_PxIES |=  button1_BITx;   //enable interrupt trigger, H->L
        button1_PxIE  |=  button1_BITx;   //enable the interrupt
        button1_PxIFG &= ~button1_BITx;   //clear port 4 interrupt flags
        timerA2_disable();
        break;
      case waiting:
        //start a timer to time the debounce sequence
        debounce(BUTTON1);
        break;
      case button_pressed:

        if (timerA2_getCount() < 48){
            generateButtonTapSig(BUTTON1);
            // Analytics 
            log_user_interaction(UI_EVENT_BUTTON1_TAP);
          }
        else if(getState(BUTTON0) == button_pressed)
        {
          generateBothHoldSig();
          button_set_state(BUTTON0, button_disabled);
        }
        else
          {
            generateButtonHoldSig(BUTTON1);
            // Analytics 
            log_user_interaction(UI_EVENT_BUTTON1_HOLD);
          }

        timerA2_resetCount();

        //buttons pins and ports are defined in buttons_driver.h
        button_set_state(BUTTON1, waiting);
        button1_PxIE  &= ~button1_BITx;   //disable interrupt flag
        button1_PxIES |=  button1_BITx;   //enable interrupt trigger, H->L
        button1_PxIE  |=  button1_BITx;   //enable the interrupt
        button1_PxIFG &= ~button1_BITx;   //clear port 4 interrupt flags
        timerA2_disable();


        break;

      default:
        break;
      }//end button switch

      break;
    case P4IV_P4IFG4: break;                    // P4.4 Interrupt
    case P4IV_P4IFG5: break;                          // Menu Button Interrupt

    case P4IV_P4IFG6: break;                    // P4.6 Interrupt
    case P4IV_P4IFG7: break;                    // P4.7 Interrupt
    default: break;
    }

}

