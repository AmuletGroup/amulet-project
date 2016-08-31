/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 *  @file core_ui.h
 *
 *  @author Travis Peters (traviswp@cs.dartmouth.edu)
 *  @author amulet-dev (amulet-dev@cs.dartmouth.edu)
 *
 *  Created: February 2015.
 */

#include "core_ui.h"

// Uncomment for debug/testing output only!
// #define CORE_UI_DEBUG_V    // Verbosity level 1
// #define CORE_UI_DEBUG_VV   // Verbosity level 2 (more verbose)
// #define CORE_UI_TESTING

#ifdef CORE_UI_TESTING
// Prototypes of test routines to avoid "implicit declaration" warnings.
void test_SystemModes();
void test_LEDs();
void test_AppSwitching();
void test_ButtonSideReporting();
void test_CoreDisplayMessage();
#endif

extern void itoa(uint16_t value, char* result, int base);
static char system_status_buffer[MAX_DISPLAY_LEN];

/* ************************************************************************** *
 *                               App management
 * ************************************************************************** */

App *emergency_app = NULL;
App *ema_app = NULL;
App *foreground_app = NULL;

void CoreUIInit() {
	if(getGlobalAppQueueHead() != NULL) {
		// Set the emergency app. (! Might be NULL).
		emergency_app = getEmergencyApp();
		ema_app = getEMAApp();
		// Set the foreground_app.
		foreground_app = getGlobalAppQueueHead();
	} else {
		// TODO: add ERROR text to display? disable any calls to UI related calls?
		return;
	}

	App * curr = getGlobalAppQueueHead();
	uint8_t i, j;
	while (curr != NULL) {
		// Initialize the curr app's display buffer.
    	for (i = 0; i< LCD_VERTICAL_MAX; i++)
        	for (j = 0; j< DISPLAY_BUFFER_WIDTH; j++)
            	curr->display_buffer[i][j] = 0xff;


		curr = curr->next;
	}


	// Initialize the system's status buffer.
	for(i = 0; i < MAX_DISPLAY_LEN; i++) {
		system_status_buffer[i] = '\0';
	}

	uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = foreground_app->display_buffer;
    setDisplayBuffer(display_buf_ptr);
    display_clear();
	// Re-render the updated status line.
	CoreRefreshStatus(foreground_app->appID);
	// Show it on display
	display_refresh();

	#ifdef CORE_UI_DEBUG_V
	printQueueApp(foreground_app);
	// Show apps currently installed after UI initialization.
	#endif



}

void CoreSwitchApp() {
	App * oldForegroundApp = foreground_app;

	// wrap back around to the front of the app queue
	if (foreground_app == NULL || foreground_app->next == NULL) {
    	foreground_app = getGlobalAppQueueHead();
	} else { // advance to next app
        foreground_app = foreground_app->next;
    }

    // Refresh the screen only if a new app has come into the foreground.
    if (oldForegroundApp != foreground_app) {
    	// Clear the screen, but not the buffer
    	display_clear_no_buffer();

    	// Change displays and push all
   		uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = foreground_app->display_buffer;
    	setDisplayBuffer(display_buf_ptr);
    	CoreRefreshStatus(foreground_app->appID);
		display_refresh();
    }

}

void CoreBringERAppForward() {
    // Refresh the screen only if a new app has come into the foreground.
    if (emergency_app != NULL && foreground_app != emergency_app) {
		//bsp_printf("CoreBringAppForward: app switched -- refresh display.\r\n");
		foreground_app = emergency_app;
		//CoreRefreshDisplay(-1);
		display_refresh();
    }
}

uint16_t CoreGetEmergencyAppId() {
	if (emergency_app == NULL) {
		return (uint16_t)-1;
	}
	return emergency_app->appID;
}

uint16_t CoreGetForegroundAppId() {
	if (foreground_app == NULL) {
		return (uint16_t)-1;
	}
	return foreground_app->appID;
}

bool CoreRequestMoveToFront(uint8_t requestor) {
	// Get the requesting app by its ID.
	App * requestingApp = getAppById(requestor);

    // Refresh the screen only if a new app has come into the foreground.
    if (requestingApp != NULL && foreground_app != requestingApp) {
		foreground_app = requestingApp;
		// Clear the screen, but not the buffer
    	display_clear_no_buffer();

    	// Change displays and push all
   		uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = foreground_app->display_buffer;
    	setDisplayBuffer(display_buf_ptr);
    	CoreRefreshStatus(foreground_app->appID);
		display_refresh();
		return true;
    }
    return false;
}

/* ************************************************************************** *
 *                                   Display
 * ************************************************************************** */

static uint8_t min(uint8_t a, uint8_t b) {
	if (a <= b)
		return a;
	return b;
}

void CoreDisplayMessage(char message[MAX_DISPLAY_LEN], unsigned int size, uint8_t line, uint8_t requestor) {
	// Update app's display buffer.
	App * requestingApp = getAppById(requestor);

	// Verify requestor app ID is valid.
	if (requestingApp == NULL) {
		return;
	}

	// Make sure app line is in legal bounds.
	if (line >= (DISPLAY_LINE_COUNT - 1)) {
		return;
	}

	// Update app display buffer
	uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = requestingApp->display_buffer;
    setDisplayBuffer(display_buf_ptr);
	clear_text_on_line(line, STAT_BAR_HEIGHT);
	display_text_on_line(message, line, STAT_BAR_HEIGHT);

	// Update the display if the calling app is the foreground app.
	if (requestor == CoreGetForegroundAppId()) {
		display_refresh();
	}
}

void CoreDisplayClr(uint8_t requestor) {
	// Update app's display buffer.
	App * requestingApp = getAppById(requestor);

	// Verify requestor app ID is valid.
	if (requestingApp == NULL) {
		return;
	}

	// Clear app's display buffer.
	uint8_t i, j;
	// Initialize the curr app's display buffer.
	/*for (i = 0; i< LCD_VERTICAL_MAX; i++)
    	for (j = 0; j< DISPLAY_BUFFER_WIDTH; j++)
        	requestingApp->display_buffer[i][j] = 0xff;*/

    uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = requestingApp->display_buffer;
    setDisplayBuffer(display_buf_ptr);
    display_clearBuffer();

    // Clear the actual screen only if the calling app is in the foreground.
    if (requestor == CoreGetForegroundAppId()) {
		display_clear();
    }
}

void CoreDisplayClrLN(uint8_t lineNumber, uint8_t requestor) {
	// Update app's display buffer.
	App * requestingApp = getAppById(requestor);

	// Verify requestor app ID is valid.
	if (requestingApp == NULL) {
		return;
	}
	if (lineNumber >= (DISPLAY_LINE_COUNT - 1)) {
		return;
	}

	uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = requestingApp->display_buffer;
    setDisplayBuffer(display_buf_ptr);
    clear_text_on_line(lineNumber, STAT_BAR_HEIGHT);

    // Refresh the screen only if the calling app is in the foreground.
    if (requestor == CoreGetForegroundAppId()) {
		display_refresh();
    }
}

void CoreRefreshStatus(uint8_t requestor) {
	// Update app's display buffer.
	App * requestingApp = getAppById(requestor);
	uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = requestingApp->display_buffer;
    setDisplayBuffer(display_buf_ptr);

	// Clear it first
	fillRect(0, 0, display_width(), STAT_BAR_HEIGHT, 0);

	// App name
	setFont(SourceSansProBold7);
	drawText(0, 0, foreground_app->appName, 1);

	// Divider line
	drawFastHLine(0,STAT_BAR_HEIGHT-1,display_width(),1);
    drawFastHLine(0, STAT_BAR_HEIGHT,display_width(),1);

	// Battery status
	uint8_t battstat = get_battery_level();
	battstat = min(battstat, 100);
	itoa(battstat, system_status_buffer, 10);
	strcat(system_status_buffer,"%");
	drawText(display_width() - textWidth(system_status_buffer) - 17, 0, system_status_buffer, 1);
	drawRect(display_width() - 14,1,13,11,1);
	fillRect(display_width() - 16,4,3,6,1);

	// Fill based on battery percent
	fillRect(display_width() - 3 - battstat/10,1,battstat/10+1,11,1);
}

/************************************************************
 **     			  New Application UI
 **
 ************************************************************/

void __display_buffer_sel_helper(uint8_t requestor) {
	App * requestingApp = getAppById(requestor);
	uint8_t (*display_buf_ptr)[LCD_HORIZONTAL_MAX/8] = requestingApp->display_buffer;
    setDisplayBuffer(display_buf_ptr);
}

void CoreSetFont(tFonts font, uint8_t requestor) {
	setFont(font);
}

void CoreText(uint8_t x, uint8_t y, char * s, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
	drawText(x, y+STAT_BAR_HEIGHT, s, 1);
}

void CoreCenteredText(uint8_t y, char * s, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
	drawTextCentered(STAT_BAR_HEIGHT+y, s, 1);
}

void CoreDrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
    drawRect(x, y+STAT_BAR_HEIGHT, w, h, 1);
}

void CoreFillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
    fillRect(x, y+STAT_BAR_HEIGHT, w, h, 1);
}

void CoreClearRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
    fillRect(x, y+STAT_BAR_HEIGHT, w, h, 0);
}

void CoreDrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
  	drawLine(x0, y0+STAT_BAR_HEIGHT, x1, y1+STAT_BAR_HEIGHT, 1);
}

void CoreDrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
  	drawFastVLine(x, y+STAT_BAR_HEIGHT, h, 1);
}

void CoreDrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
  	drawFastHLine(x, y+STAT_BAR_HEIGHT, w, 1);
}

void CoreDrawHeartImage(uint8_t x,uint8_t y, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
	drawHeartImage(x, y+STAT_BAR_HEIGHT);
}

void CoreDrawWalkerImage(uint8_t x,uint8_t y, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
	drawWalkerManImage(x, y+STAT_BAR_HEIGHT);
}

void CoreDrawSunImage(uint8_t x,uint8_t y, uint8_t requestor) {
	__display_buffer_sel_helper(requestor);
	drawSunImage(x, y+STAT_BAR_HEIGHT);
}

void CorePushChangesToDisplay(uint8_t requestor) {
	if (requestor == CoreGetForegroundAppId()) {
		display_refresh();
	}
}

uint8_t CoreDrawingAreaWidth() {
	return LCD_HORIZONTAL_MAX;
}
uint8_t CoreDrawingAreaHeight() {
	return LCD_VERTICAL_MAX - STAT_BAR_HEIGHT;
}

/**
 * 
 * Turns on the capacitive touch timers
 * @param requestor [description]
 */
void CoreTurnOnCapTouch(uint8_t requestor) {
	turnOnCapTouch();
}

/**
 * Turns off the capacitive touch timers
 * @param requestor [description]
 */
void CoreTurnOffCapTouch(uint8_t requestor) {
	turnOffCapTouch();
}


/* ************************************************************************** *
 *                                    LEDs
 * ************************************************************************** */

void CoreSetLed(LED ledId, LEDState ledState, uint8_t requestor) {
	if(ledState == LED_ON) {
		turn_LED_on(ledId);
	} else {
		turn_LED_off(ledId);
	}
}

/* ************************************************************************** *
 *                             Haptic Buzzer
 * ************************************************************************** */

void CoreHapticSingleBuzz(uint8_t requestor) {
	singleBuzz();
}

void CoreHapticDoubleBuzz(uint8_t requestor) {
	doubleBuzz();
}

/* ************************************************************************** *
 *                                   Slider
 * ************************************************************************** */
void sliderSignal(enum AmuletSignal sliderSig) {
	AmuletButtonEvt *bEvt = Q_NEW(AmuletButtonEvt, sliderSig);
	QACTIVE_POST(foreground_app->activeObject, &bEvt->super, NULL);
}

/* ************************************************************************** *
 *                                   Buttons
 * ************************************************************************** */

void buttonPressedSignal(enum AmuletSignal buttonSig, int buttonId) {
	if(buttonSig == AMULET_BOTH_HOLD_SIG){
		AmuletButtonEvt *bEvt = Q_NEW(AmuletButtonEvt, buttonSig);
		bEvt->buttonId = buttonId;
		if(ema_app!=NULL){
			QACTIVE_POST(ema_app->activeObject, &bEvt->super, NULL);
		}
	}
	else if (buttonId == BUTTON0 && buttonSig == AMULET_BUTTON_TAP_SIG) {

		// Prevent app scrolling while in EMERGENCY_MAYBE or EMERGENCY mode.
		if (AmuletInEmergencyMaybeMode(UI_SERVICE_ID) || AmuletInEmergencyMode(UI_SERVICE_ID)) {
			return;
		}
		CoreSwitchApp();
	} else if (emergency_app != NULL && buttonId == BUTTON0 && buttonSig == AMULET_BUTTON_HOLD_SIG) {
		// System button hold events are special -- *only* meant for ER application.

        AmuletButtonEvt *bEvt = Q_NEW(AmuletButtonEvt, buttonSig);
        bEvt->buttonId = buttonId;
        QACTIVE_POST(emergency_app->activeObject, &bEvt->super, NULL);
	} else if (buttonId == BUTTON1) {
		// All app button events should be sent to the foreground application.

        AmuletButtonEvt *bEvt = Q_NEW(AmuletButtonEvt, buttonSig);
        bEvt->buttonId = buttonId;
        QACTIVE_POST(foreground_app->activeObject, &bEvt->super, NULL);
	}

	// Tests.
	//test_SystemModes();
	//test_LEDs();
	//test_AppSwitching();
	//test_ButtonSideReporting();
	//test_CoreDisplayMessage();
}

#ifdef CORE_UI_TESTING

/* ************************************************************************** *
 *                               Test helpers
 * ************************************************************************** */

bool check_SystemModes(SystemMode expected, SystemMode result) {
	return (expected == result);
}

bool check_ButtonSides(ButtonSide expected, uint8_t buttonId) {
	return (expected == AmuletButtonSide(buttonId, 0));
}

void print_result(bool test) {

}

// Test: system mode getter/setter.
void test_SystemModes() {
	
	print_result(check_SystemModes(NORMAL_MODE, CoreGetSystemMode()));

	CoreSetSystemMode(EMERGENCY_MAYBE_MODE);
	print_result(check_SystemModes(EMERGENCY_MAYBE_MODE, CoreGetSystemMode()));

	CoreSetSystemMode(EMERGENCY_MODE);
	print_result(check_SystemModes(EMERGENCY_MODE, CoreGetSystemMode()));

	CoreSetSystemMode(DEBUG_MODE);
	print_result(check_SystemModes(DEBUG_MODE, CoreGetSystemMode()));

	CoreSetSystemMode(NORMAL_MODE);
	print_result(check_SystemModes(NORMAL_MODE, CoreGetSystemMode()));
}

// Test: toggle LEDs.
uint8_t toggle = 0;
void test_LEDs() {
	if(toggle == 0) {
		CoreSetLed(EXTERNAL_GREEN, LED_ON, 100);
		CoreSetLed(EXTERNAL_RED, LED_ON, 100);
		toggle = 1;
	} else {
		CoreSetLed(EXTERNAL_GREEN, LED_OFF, 100);
		CoreSetLed(EXTERNAL_RED, LED_OFF, 100);
		toggle = 0;
	}
}

// Test: ability to scroll through apps.
void test_AppSwitching() {
	CoreSwitchApp();
}

void test_ButtonSideReporting() {
	print_result(check_ButtonSides(INVALID_SIDE, -1));

	print_result(check_ButtonSides(LEFT, BUTTON_L0));

	print_result(check_ButtonSides(LEFT, BUTTON_L1));

	print_result(check_ButtonSides(RIGHT, BUTTON0));

	print_result(check_ButtonSides(RIGHT, BUTTON1));

	print_result(check_ButtonSides(INVALID_SIDE, 4));
}

void test_CoreDisplayMessage() {
	// CoreDisplayMessage("**WHAT**", 9          2, 100); // act like the first installed app.
	// CoreDisplayMessage("is this guy", 12      4, 100); // act like the first installed app.
	// CoreDisplayMessage("talking about?", 15   6, 100); // act like the first installed app.

	// CoreDisplayMessage("**GIT IT**", 11, 4, 101); // act like the second installed app.
	// CoreDisplayMessage("**GOT IT**", 11, 5, 101); // act like the second installed app.
	// CoreDisplayMessage("**GOOD**",   9,  7, 101); // act like the second installed app.
}

#endif
