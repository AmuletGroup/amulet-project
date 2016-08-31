/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 * @file core_app_manager.h
 *
 * This is the queue container for the app.
 *
 * @author amolina
 *
 * Created on February 11, 2014, 10:25 AM
 */

#ifndef CORE_APP_MANAGER_H
#define	CORE_APP_MANAGER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "amulet.h"
#include "core_ui.h"
#include "bsp_setup.h"
#include "qp_port.h"

typedef struct AppQueue App;

struct AppQueue {
    uint16_t appID;                                                 // app's ID (see: apps.h)
    uint16_t appLogID;                                              // app's Log ID (see: apps.h)
    QActive * activeObject;                                         // app's active object
    char appName[MAX_DISPLAY_LEN];                                  // app's human-readable name
    char appShortName[MAX_STATUS_LEN];                              // app's short name (intended for status line)
    unsigned char appType;                                          // app's "type" (see: amulet.h)
	uint8_t display_buffer[LCD_VERTICAL_MAX][DISPLAY_BUFFER_WIDTH];  // app's display buffer
    char status_buffer[MAX_STATUS_LEN];                             // apps' status buffer
    App * next;                                                     // ptr to next app in LL
};

extern App * GlobalAppQueueHead;

/**
 * Adds and element at the end of the queue.
 *
 * @param head Head to the app queue
 * @param new The pointer to the new app.
 *
 * @return None.
 */
void enqueueApp(App * head, App * new);

/**
 * If the queue with specific ID already in the apps queue
 *
 * @param head Head to the app queue
 * @param appID The ID for the query app
 *
 * @return 1 if yes, -1 if no.
 */
int inQueueApp(App * head, uint16_t appID) ;

/**
 * Given the id of a app, get the pointer to Active Object of the app. 
 * The Active Object is the QM state machine of the app. 
 *
 * @param head Head to the app queue
 * @param appID The ID for the query app
 *
 * @return The pointer to Active Object of the app
 */
QActive * getAppActiveObject(App * head, uint16_t appID) ;
App * _removeElementApp(App * head, uint16_t appID) ;
void printQueueApp(App * head) ;

/**
 * Given the id of a app, remove that app
 *
 * @param head Head to the app queue
 * @param appID The ID for the query app
 *
 * @return The head of the apps queue, because of the app to be removed may be the original head of the apps queue
 */
App * removeElementApp(App * head, uint16_t appID);

/**
 * Return of the head of the app queue, which is the input parameter for other functions
 *
 * @return the head of the app queue, which is the input parameter for other functions
 */
App * getGlobalAppQueueHead();

/**
 * Given the id of a app, return the pointer to the App data object
 *
 * @param appID The ID for the query app
 *
 * @return The pointer to the App data object, app's object has the pointer to the active object
 */
App * getAppById(uint8_t appId);

/**
 * Assumes there is only 1 emergency app. If more than 1 exists, this will 
 * select the first such app in the queue.
 *
 * @return None.
 */
App * getEmergencyApp();

/**
 * Assumes there is only 1 EMA app
 *
 * @return None.
 */
App * getEMAApp();

#ifdef	__cplusplus
}
#endif

#endif	/* CORE_APP_MANAGER_H */
