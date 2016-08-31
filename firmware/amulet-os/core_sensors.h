/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 *  @file core_sensors.h
 *  @brief functions to manage sensor subscriptions and notify apps
 */

#ifndef CORE_SENSORS_H_
#define CORE_SENSORS_H_

#include "amulet.h"

// TODO These parameters maybe too much for internal sensors.
typedef struct {
    uint8_t application_id;
    uint8_t sensor_id;
    // uint32_t expiration;
    // uint8_t* config_params;
    // uint8_t nparams;
    // uint8_t window_size;
    // uint8_t window_interval;
} sensorSubscription;


//Subscription
typedef struct SubscriptionQueue Subscription;

uint8_t* sensorData;

struct SubscriptionQueue {
    uint16_t subscriptionID;
    uint8_t principal;
    uint8_t service;
    uint16_t expiration;
    bool disabled;
    bool continuous;
    
    // This is a counter mainly for Timers, as the core level timer tick is 1
    //second, for every tick, the counter will increment by 1. When the counter
    // reach the window intervel, it then send signals to the apps that
    // subscribed to this timer.
    uint16_t current_time;
    uint16_t window_interval;
    Subscription * next;
};

// The Amulet_Sensors event handler will store the accelerometer here.
// TODO The size of these arrays should be defined elsewhere.
#define ACC_SAMPLES_PER_SECOND 30
uint16_t acc_data_x[ACC_SAMPLES_PER_SECOND];
uint16_t acc_data_y[ACC_SAMPLES_PER_SECOND];
uint16_t acc_data_z[ACC_SAMPLES_PER_SECOND];

#define MAX_SENSOR_SUBSCRIPTIONS 100
Subscription sensorSubscriptions[MAX_SENSOR_SUBSCRIPTIONS];
int8_t currentNumSubscriptions;

int inQueueSubscription(uint16_t subscriptionID);
uint16_t isPrincipalSubscribedService(uint8_t principal, uint8_t service);
void removeElementSubscription(uint16_t subscriptionID);

/**
 * Subscribe to an (internal) sensor. 
 *
 * @note This function will create a new node in the subscription queue.
 * @note Currently we don't have a sensor discovery protocol, so the sensor data 
 * from external sensors arrives in the radio board and will be passed to the 
 * application constanly---therefore we treat external sensors as internal sensors.
 */
 void SubscribeSensor(uint8_t sensor_id, uint16_t expiration, 
   uint16_t window_size, uint16_t window_interval, uint8_t requestor);
/**
 * Unsubscribe from an (internal) sensor. 
 *
 * @note This function will remove the requestor's node from the subscription queue.
 * @note Currently we don't have a sensor discovery protocol, so the sensor data 
 * from external sensors arrives in the radio board and will be passed to the 
 * application constanly---therefore we treat external sensors as internal sensors.
 */
void UnsubscribeSensor(uint8_t sensor_id, uint8_t requestor);

/**
 * This function is used to determine whether a sensor internal or external,
 * 1 for external and 0 for internal 
 */
bool isAppBoardSensor(uint8_t sensor_id);

/**
 * The requestor will subscribe to a timer. The requestor will then get notified
 * in every "seconds" seconds.
 */
void CoreTimer(uint16_t seconds, uint8_t requestor);

/**
 * This function should be called once new sensor data is ready, in the function,
 * it will check all the apps that subscribe to the sensor and send them event.
 * The timer will also use this function to work similarily
 */
void NotifyApp(uint8_t* payload, uint16_t length);

/**
 * Cancel the timer the requestor currently subscribed to.
 */
void CoreTimerCancel(uint8_t requestor);

/**
 * Seconds since the amulet started up
 */
uint32_t CoreSeconds(uint8_t requestor);


/**
 * Get current date and time all the way from the BSP layer.
 */
void CoreClock(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* min, uint8_t* sec, uint8_t* weekday);

/**
 * Set current date and time all the way to the BSP layer.
 */
void CoreSetClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                  uint8_t min, uint8_t sec, uint8_t weekday);

/**
 * This assumes only connecting to one sensor at a time
 */
bool CoreIsBLEConnected();

#endif /* CORE_SENSORS_H_ */
