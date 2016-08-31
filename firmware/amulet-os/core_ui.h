/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 *  @file core_ui.h
 *  @brief functions to interact with the screen 
 *
 * TODO:
 * - Resolve where certain definitions will live. Below, definitions that are
 *   needed have been commented out and labeled with a comment that identifies
 *   where the definition *currently* lives. This may need to change.
 *
 * Features for Later:
 * - back light
 * - toggling background/text (black-on-white, white-on-black)
 * - haptic buzzer
 * - analog dial
 */

#include "amulet.h"

#include "led_driver.h"
#include "buttons_driver.h"
#include "haptic_driver.h"

#ifndef CORE_UI_H_
#define CORE_UI_H_

#define UI_SERVICE_ID   0

/* ************************************************************************** *
 *                                UI DEFINITIONS
 * ************************************************************************** */

/**
 * Button location defintions.
 */
typedef enum ButtonSide {
    LEFT,             /**< Button is located on the left side of the case. */
    RIGHT,            /**< Button is located on the right side of the case. */
    INVALID_SIDE      /**< Indicates that the button doesn't exist. */
} ButtonSide;

#define BUTTON_L0   0 /**< Button available on Amulet device: button on left side; first button down from the top. */
#define BUTTON_L1   1 /**< Button available on Amulet device: button on left side; second button down from the top. */
#define BUTTON_R0   2 /**< Button available on Amulet device: button on right side; first button down from the top. */
#define BUTTON_R1   3 /**< Button available on Amulet device: button on right side; second button down from the top.*/

#define BUTTON0 BUTTON_R0    /**< Simplified button interface (BUTTON0 is BUTTON_R0). Developers should specify the button they want by number. Use buttonSide() function to determine which side of the case the button resides on. */
#define BUTTON1 BUTTON_R1    /**< Simplified button interface (BUTTON1 is BUTTON_R1). Developers should specify the button they want by number. Use buttonSide() function to determine which side of the case the button resides on. */
// #define BUTTON2 BUTTON_L0 /**< Simplified button interface (BUTTON2 is BUTTON_L0). Developers should specify the button they want by number. Use buttonSide() function to determine which side of the case the button resides on. */
// #define BUTTON3 BUTTON_L1 /**< Simplified button interface (BUTTON3 is BUTTON_L1). Developers should specify the button they want by number. Use buttonSide() function to determine which side of the case the button resides on. */

#define TOTAL_BUTTONS 2  /**< Only 2 buttons are actually connected and functional. */

/**
 * Returns the side of the case that the button is located on.
 *
 * @param id The button ID (see: BUTTON_XX #define statemetns above).
 * @param requestor  An ID corresponding to the requesting app.
 * @return ButtonSide The side of the case where the button resides
 */
ButtonSide buttonSide(uint8_t id, uint8_t requestor);

/**
 * LED definitions
 */
typedef enum LED {
    EXTERNAL_GREEN,   /**< Green LED exposed on surface of the device. */
    EXTERNAL_RED,     /**< Red LED exposed on the surface of the device. */
    INTERNAL          /**< Internal (red?) LED located inside the device's case. */
} LED;

#define TOTAL_LEDS 3  /**< Total number of LEDs available on the device. */

/**
 * LED Modes
 */
typedef enum LEDState {
    LED_OFF,    /**< (0) Turn an LED off. */
    LED_ON,     /**< (1) Turn an LED on. */
    LED_BLINK   /**< (3) Blink an LED on and off (not implemented). */
} LEDState;


/* START: OLD DEPRECATED graphics constants */
/*
 * Display definitions. The STATUS_LINE (0) can only be accessed directly by the system.
 * Lines 1 through DISPLAY_LINE_COUNT-1 can be used by the foreground app. Also note:
 * DISPLAY_LINE_LENGTH refers to the # of characters that fit on a single line of the display.
 */
#define DISPLAY_STATUS_LINE    0  /**< Line # for the "status line" on the display (top line).*/
#define DISPLAY_LINE_COUNT     9  /**< Total # of lines available on display (only the last 8 lines avail. to the "front" app). */
#define MAX_DISPLAY_LEN        16 /**< Width of the display (i.e., the # of characters that fit on one line). */
#define MAX_STATUS_LEN         3  /**< Total # of characters each app gets for displaying a status in the status line. */
/* END: OLD DEPRECATED graphics constants */

/* New graphics constants */
#define STAT_BAR_HEIGHT 14
#define DISPLAY_BUFFER_WIDTH (LCD_HORIZONTAL_MAX/8)

/* ************************************************************************** *
 *                                 Core UI API                                *
 * ************************************************************************** */

/**
 * All UI initializations.
 * This function will be called by main.c, which will be automatically generated
 * by AFT
 */
void CoreUIInit();

/**
 * Switch between apps. This routine changes the foreground app (ID), and will
 * call CoreDisplayMessage to refresh the display with the new foreground app.
 *
 * NOTE: likely called in response to a “button tap” event on the system button).
 */
void CoreSwitchApp();

/**
 * Switch to the Emergency Response app. This routine changes the foreground app, and
 * will refresh the display (if needed).
 *
 * If the foreground app is already the ER app, calling this function is essentially a no-op.
 */
void CoreBringERAppForward();

/**
 * Get the ID of the current emergency app.
 */
uint16_t CoreGetEmergencyAppId();

/**
 * Get the ID of the current foreground ("front") app.
 */
uint16_t CoreGetForegroundAppId();

/**
 * Switch to the app with App ID 'requestor'. This routine changes the foreground
 * app, andd will refresh the display (if needed).
 *
 * If the requesting app is already in the foreground, calling this function is
 * essentially a no-op and 'false' will be returned to the caller.
 */
bool CoreRequestMoveToFront(uint8_t requestor);

/**
 * Write/update text to the app’s display buffer and if the current app is the foreground
 * app, it will refresh the screen.
 *
 * @param message the text that is to be displayed for the calling app.
 * @param line the line number where the desired text will be written.
 * @param requestor an ID corresponding to the requesting app
 */
void CoreDisplayMessage(char message[MAX_DISPLAY_LEN], unsigned int size, uint8_t line, uint8_t requestor);

/**
 * Clear an app's display buffer.
 */
void CoreDisplayClr(uint8_t requestor);

/**
 * Clear the  display buffer of the calling app for specified line.
 */
void CoreDisplayClrLN(uint8_t lineNumber, uint8_t requestor);


/**
 * Refresh the status line of the display (called after an app's status has changed
 * and the system status buffer needs to be updated).
 */
void CoreRefreshStatus(uint8_t requestor);

/**
 * Set the state of a particular LED.
 *
 * NOTE: support for custom coloring is being left to a later iteration.
 *
 * @param ledId the ID of the LED that is to be set (see: LED enum)
 * @param ledState the desired state of the LED (see: LEDState enum)
 * @param requestor an ID corresponding to the requesting app.
 */
void CoreSetLed(LED ledId, LEDState ledState, uint8_t requestor);

/**
 * Trigger a single, 0.1 second "buzz" from the haptic buzzer.
 *
 * @param requestor an ID corresponding to the requesting app.
 */
void CoreHapticSingleBuzz(uint8_t requestor);

/**
 * Trigger two back-to-back, 0.1 second "buzz"es from the haptic buzzer.
 *
 * @param requestor an ID corresponding to the requesting app.
 */
void CoreHapticDoubleBuzz(uint8_t requestor);

/**
 * The buttons_driver will call this routine to inform the UI Service
 * that there was a button event. When called, the driver will supply
 * information about the type of event as well as the button's ID.
 *
 * @param buttonSig the type of button interaction (e.g., tap, hold)
 * @param buttonId the ID of the button.
 */
// void buttonPressedSignal(enum ButtonSignals buttonSig, int buttonId);


/************************************************************
 **     			  New Application UI
 **
 ************************************************************/
/**
 * Set regular font or bold font
 *
 * @param font The font defination are defined in amulet.h, currently we have REGULAR_FONT and BOLD_FONT to choose
 *
 * @return None.
 */
void CoreSetFont(tFonts font, uint8_t requestor);

/**
 * Print the text on the screen
 *
 * @param x x coordinate of the starting point
 * @param y y coordinate of the starting point
 * @param s string to be printed
 *
 * @return None.
 */
void CoreText(uint8_t x, uint8_t y, char * s, uint8_t requestor);

/**
 * Same to the CoreText but the x will be at the center of the screen
 *
 * @param y y coordinate of the starting point
 * @param s string to be printed
 *
 * @return None.
 */
void CoreCenteredText(uint8_t y, char * s, uint8_t requestor);

/**
 * Draw a rectangle with line width 1 pixel
 *
 * @param y y coordinate of the left top corner
 * @param y y coordinate of the left top corner
 * @param w width of the rectangle
 * @param h height of the rectangle
 *
 * @return None.
 */
void CoreDrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t requestor);

/**
 * Draw a filled rectangle
 *
 * @param y y coordinate of the left top corner
 * @param y y coordinate of the left top corner
 * @param w width of the rectangle
 * @param h height of the rectangle
 *
 * @return None.
 */
void CoreFillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t requestor);

/**
 * Erase all the content inside the rectangle
 *
 * @param y y coordinate of the left top corner
 * @param y y coordinate of the left top corner
 * @param w width of the rectangle
 * @param h height of the rectangle
 *
 * @return None.
 */
void CoreClearRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t requestor);

/**
 * All the other functions only change the display buffer, this API really dump all the changes to the display
 *
 * @return None.
 */
void CorePushChangesToDisplay(uint8_t requestor);

/**
 * Draw a line with line width 1 pixel
 *
 * @param x0 x coordinate of the starting point
 * @param y0 y coordinate of the starting point
 * @param x1 x coordinate of the ending point
 * @param y1 y coordinate of the ending point
 *
 * @return None.
 */
void CoreDrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t requestor);

/**
 * Draw vertical line on the screen, you should refresh the screen after draw
 * 
 * @param x the X coordinate of the starting point
 * @param y the Y coordinate of the starting point
 * @param h length of the line
 */
void CoreDrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t requestor);

/**
 * Draw horizontal line on the screen, you should refresh the screen after draw
 * 
 * @param x the X coordinate of the starting point
 * @param y the Y coordinate of the starting point
 * @param h length of the line
 */
void CoreDrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t requestor);
void CoreDrawHeartImage(uint8_t x,uint8_t y, uint8_t requestor);
void CoreDrawWalkerImage(uint8_t x,uint8_t y, uint8_t requestor);
void CoreDrawSunImage(uint8_t x,uint8_t y, uint8_t requestor);
uint8_t CoreDrawingAreaWidth();
uint8_t CoreDrawingAreaHeight();

/**
 * 
 * Turns on the capacitive touch timers
 * @param requestor [description]
 */
void CoreTurnOnCapTouch(uint8_t requestor);

/**
 * Turns off the capacitive touch timers
 * @param requestor [description]
 */
void CoreTurnOffCapTouch(uint8_t requestor);
#endif /* CORE_UI_H_ */
