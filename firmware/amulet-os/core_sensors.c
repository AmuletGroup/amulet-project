/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 * core_sensors.c
 *
 *  Created on: Oct 11, 2013
 *      Author: Andres
 */

#include "core_sensors.h"
#include "ADXL362driver/ADXL362.h"
#include "nrf51822.h"
#include "analytics.h"
#include "rtc.h" 
 
uint16_t lastSubscriptionID;
volatile int8_t i;

extern uint32_t amulet_startup_timer;

//#define BSP_DEBUG_IPC
#define IPC_PARAMETERS 7 //IPC parameter length
#define TEST_MESSAGE_SIZE (8+IPC_PARAMETERS)

uint16_t isPrincipalSubscribedService(uint8_t principal, uint8_t service) {
    for (i = 0; i < currentNumSubscriptions; i++) {
      if ((sensorSubscriptions[i].principal == principal) && (service == sensorSubscriptions[i].service))
        return sensorSubscriptions[i].service;
    }
    return -1;
}

void removeElementSubscription(uint16_t subscriptionID) {
    // Does not work for the last subscriptions
    for (i = 0; i < currentNumSubscriptions-1; i++) {
      if(sensorSubscriptions[i].subscriptionID == subscriptionID) {
        // Move everything with ndx > i down one index
        memmove(&sensorSubscriptions[i], &sensorSubscriptions[i+1], currentNumSubscriptions-(i+1));
        currentNumSubscriptions--;
        return;
      }
    }
    //Handle the last subscription problem
    if(i == currentNumSubscriptions-1 && sensorSubscriptions[i].subscriptionID == subscriptionID) {
      currentNumSubscriptions--;
    }
}

void NotifyApp(uint8_t* payload, uint16_t length) {
  for (uint8_t j = 0; j < currentNumSubscriptions; j++) {
    uint16_t subscribedAppID = (uint16_t) sensorSubscriptions[j].principal;

    // Branch based on the service type
    uint8_t service = (uint8_t) sensorSubscriptions[j].service;
    AmuletSensorsEvt *dataEvt;
    if (service == *payload && sensorSubscriptions[j].disabled == false) {// The first byte of the payload is sensor type
      switch(service) {
        case TIMER:
          sensorSubscriptions[j].current_time+=1;
          if(sensorSubscriptions[j].current_time >= sensorSubscriptions[j].window_interval){
              sensorSubscriptions[j].current_time=0;
              AmuletSensorsEvt *timeReqEvt;
              timeReqEvt = Q_NEW(AmuletSensorsEvt, AMULET_REQ_CHANGE_SIG);
              QACTIVE_POST(getAppActiveObject(GlobalAppQueueHead, subscribedAppID), &timeReqEvt->super, NULL);
          }
          break;
        case IMU:
          break;
        case LIGHT:
          break;
        case AUDIO:
          break;
        case TEMPERATURE:
          break;
        // Default can handle sensors like Acc                        
        default:
          if (service == ACCELEROMETER) {
            dataEvt = Q_NEW(AmuletSensorsEvt, AMULET_DEVICE_DATA_XL_SIG);
          } else if(service == HEARTRATE) {
            dataEvt = Q_NEW(AmuletSensorsEvt, AMULET_DEVICE_DATA_HR_SIG);
            // sensorData should be renamed sensorDataHR and new variables should
            // be created for every sensor type.
 
            sensorData = payload;// The last 1 is the length of the data 
          } else {
            dataEvt = Q_NEW(AmuletSensorsEvt, AMULET_DEVICE_DATA_SIG);
          }
          dataEvt->sensor_id = *payload;
          dataEvt->length = length;
          //uint8_t type
    	    //uint32_t timestamp;   
   	      //uint16_t timestamp_milli; 
          //sensorData = payload;// The last 1 is the length of the data 
          QACTIVE_POST(getAppActiveObject(GlobalAppQueueHead, subscribedAppID), &dataEvt->super, NULL);
      }
    }
  }

  for (uint8_t j = 0; j < currentNumSubscriptions; j++) {
    if (*payload == TIMER) {
      //Because this timer event happens every one second, we can also use it 
      // to remove expired sensor subscriptions.
      sensorSubscriptions[j].expiration--;
      if (sensorSubscriptions[j].expiration <=0 && sensorSubscriptions[j].continuous==false) {
        if(ACCELEROMETER == sensorSubscriptions[j].service){
          acc_ref_counter--;
        }
        if(sensorSubscriptions[j].service == HEARTRATE ||
            sensorSubscriptions[j].service == RUNNING_SPEED){
          ble_ref_counter--;
          if(ble_ref_counter <= 0) {
            // If no subscriptions to BLE sensors, then turn off the BLE chip
            nrf51822_turn_off();
          }
        }
        
        removeElementSubscription(sensorSubscriptions[j].subscriptionID);
      }
    }
  }
}

// TODO This need to be implemented
bool isBLESensor(uint8_t sensor_id) {
  if(sensor_id == HEARTRATE ||
      sensor_id == RUNNING_SPEED) {
    return true;
  } else return false;
}

// Add subscription using existing memory
void SubscribeSensor(uint8_t sensor_id, uint16_t expiration, 
  uint16_t window_size, uint16_t window_interval, uint8_t requestor) {
    if(!isBLESensor(sensor_id) && sensor_id != TIMER) {
      uint8_t subscription_already_exist = 0;
      for (uint8_t j = 0; j < currentNumSubscriptions; j++) {
        uint8_t service = (uint8_t) sensorSubscriptions[j].service;
        if(service == sensor_id) {
          subscription_already_exist++;
        }
      }
      if(subscription_already_exist == 0) {
        // Send the IPC message to M0        
        // This is the enqueue operation, use static array instead of malloc.
        //enqueueIPC(sensor_id, expiration, window_size, window_interval, requestor,
        //  IPC_REQUEST, SUBSCRIBE_INTERNAL_SENSOR, INT_SENSOR_MAN_M0, 8, 500);
      }
    }
    
    if(ACCELEROMETER == sensor_id){
      acc_ref_counter++;
    }
    if(sensor_id == HEARTRATE ||
        sensor_id == RUNNING_SPEED){
      ble_ref_counter++;
      // If service is on BLE then start up the BLE chip and set flag
      // if we have not already
      if(!is_ble_enabled()) {
        nrf51822_turn_on();
        nrf51822_init_spi();
        nrf51822_deselect();
      }
    }    

    // Handle other sensor subscriptions, i.e. start up the sensing duty cycle I guess
    Subscription *newSubscription = &sensorSubscriptions[currentNumSubscriptions];
    newSubscription->subscriptionID = lastSubscriptionID;
    newSubscription->principal = requestor;
    newSubscription->service = sensor_id;
    newSubscription->expiration = expiration;
    newSubscription->window_interval = window_interval;
    newSubscription->current_time = 0;
    newSubscription->disabled = false;
    newSubscription->next = NULL;
    if(expiration==0)
      newSubscription->continuous = true;
    else
      newSubscription->continuous = false;
    currentNumSubscriptions++;
    lastSubscriptionID++;

}

void UnsubscribeSensor(uint8_t sensor_id, uint8_t requestor) {
   uint16_t subscriptionID=isPrincipalSubscribedService(requestor, sensor_id);
   
   if(ACCELEROMETER == sensor_id){
     acc_ref_counter--;
   }
  if(sensor_id == HEARTRATE ||
      sensor_id == RUNNING_SPEED){
    ble_ref_counter--;
    if(ble_ref_counter <= 0) {
      // If no subscriptions to BLE sensors, then turn off the BLE chip
      nrf51822_turn_off();
      log_ble_event(BLE_EVENT_DISCONNECT);
    }

  }

   // Instead of remove, we simply put a "disabled" flag, avoid send unecessary
   // new IPC request message to M0
   for (i = 0; i < currentNumSubscriptions; i++) {
      if ((sensorSubscriptions[i].principal == requestor) && (sensor_id == sensorSubscriptions[i].service)) {
        sensorSubscriptions[i].disabled = true;
      }
   }

   if(!isBLESensor(sensor_id)) {
     uint8_t subscription_already_exist = 0;
     for (uint8_t j = 0; j < currentNumSubscriptions; j++) {
       uint8_t service = (uint8_t) sensorSubscriptions[j].service;
       if(service == sensor_id) {
         subscription_already_exist++;
       }
     }
   }
}


// The reason we put the timer in core_sensor.c is because timer and sensors
// work in a similar fasion.
// TODO Refactor it with SubscribeInternalSensor
void CoreTimer(uint16_t seconds, uint8_t requestor) {
    for (uint8_t j = 0; j < currentNumSubscriptions; j++) {
        uint8_t service = (uint8_t) sensorSubscriptions[j].service;
        if(service == TIMER && sensorSubscriptions[j].principal == requestor) {
          sensorSubscriptions[i].disabled = false;
          sensorSubscriptions[j].window_interval = seconds;
          sensorSubscriptions[j].current_time = 0;
          return;
        }
      }
    SubscribeSensor(TIMER, 0, 0, seconds, requestor);
}

// TODO The limitation here is that one app can only subscribe to one timer.
void CoreTimerCancel(uint8_t requestor){
   UnsubscribeSensor(TIMER, requestor);
}

// TODO Should this function directly access hardware layer variables.
void CoreClock(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* min, uint8_t* sec, uint8_t* weekday){
  *year = RTCYEAR;
  *month = RTCMON;
  *day = RTCDAY;
  *hour = RTCHOUR;
  *min = RTCMIN;
  *sec = RTCSEC;
  *weekday = RTCDOW;
}

void CoreSetClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                  uint8_t min, uint8_t sec, uint8_t weekday) {
  pause_rtc();
  RTCYEAR = year;
  RTCMON = month;
  RTCDAY = day;
  RTCHOUR = hour;
  RTCMIN = min;
  RTCSEC = sec;
  RTCDOW = weekday;
  resume_rtc();
}

uint32_t CoreSeconds(uint8_t requestor) {
  return amulet_startup_timer;
}
