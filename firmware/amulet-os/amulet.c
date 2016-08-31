/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#include "amulet.h"

#ifdef SIFT_DATA_HACK
#include "amulet_data.h"
#endif

//#define PROFLILNG
#define PROFLILNG_DELAY 800
#define PROFLILNG_DELAY_BEFORE 0
#define PROFLILNG_LOOP 1

extern void itoa(int16_t value, char* result, int base);

void busy_wait(uint32_t cycles) {
  volatile uint16_t i = 0;
  for (; i < cycles; i++) {
  }
}
/*****************************************************************************
 * SENSORS
 *****************************************************************************/

void AmuletSubscribeInternalSensor(uint8_t sensor_id, uint16_t expiration,
  uint16_t window_size, uint16_t window_interval, uint8_t requestor) {
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    SubscribeSensor(sensor_id, expiration, window_size, window_interval, requestor);
    #ifdef PROFLILNG
  }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

void AmuletUnsubscribeInternalSensor(uint8_t sensor_id, uint8_t requestor) {
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    UnsubscribeSensor(sensor_id, requestor);
    #ifdef PROFLILNG
  }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

uint16_t AmuletGetRR(uint8_t requestor) {
  uint16_t rv = (sensorData[3] + (sensorData[4]<<8));
  return rv;
}

uint16_t AmuletGetHR(uint8_t requestor) {
  uint16_t rv = (sensorData[1] + (sensorData[2]<<8));
  return rv;
}

uint16_t AmuletGetADC(enum InternalResource ir, uint8_t requestor) {
  uint16_t retval = 0;
  #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
    
  switch (ir) { // Decode day of week
  case BATTERY:
      retval = take_battery_reading();
      break;
  case TEMPERATURE:
      retval = take_temp_reading();
      break;
  case LIGHT:
      retval = take_light_reading();
      break;
  case AUDIO:
      retval = take_audio_reading();
      break;
  default:
    retval = 0;
    break;
  }
  
  #ifdef PROFLILNG
  }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
  #endif 
  return retval; 
}

uint16_t AmuletAnalytics(enum AnalyticsResource ar, uint8_t requestor) {
  uint16_t retval = 0;
  switch (ar) {
    case BLE_EVENT:
        retval = most_recent_ble_events;
        most_recent_ble_events = 0;
        break;
    case UI_EVENT:
        retval = most_recent_ui_events;
        most_recent_ui_events = 0;
        break;
  }   
  return retval;
}

bool AmuletIsBLEPaired() {
  return is_ble_peripheral_paired();
}

uint8_t AmuletGetBatteryLevel(uint8_t requestor) {
    uint8_t battery_reading;
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    battery_reading = get_battery_level();
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
    return battery_reading;
}

uint16_t AmuletGetLightLevel(uint8_t requestor) {
    uint16_t light_reading;
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    light_reading = take_light_reading();
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
    return light_reading;
}

uint16_t AmuletGetTemperature(uint8_t requestor) {
    uint16_t temp_reading;
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    temp_reading = get_temperature();
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
    return temp_reading;
}

uint16_t AmuletGetAudio(uint8_t requestor) {
    uint16_t audio_reading;
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    audio_reading = take_audio_reading();
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
    return audio_reading;
}

int16_t AmuletGetAccelX(uint8_t idx, uint8_t requestor) {
  int16_t value;
  //uint8_t i = idx*4;//0-->0
  //value = (sensorData[i] | sensorData[i+1]<<8);
  value = acc_buffer_x[idx];
  return value;
}

int16_t AmuletGetAccelY(uint8_t idx, uint8_t requestor) {
  int16_t value;
  //uint8_t i = idx*4;//0-->0
  //value = (sensorData[i+2] | sensorData[i+3]<<8);
  value = acc_buffer_y[idx];
  return value;
}

int16_t AmuletGetAccelZ(uint8_t idx, uint8_t requestor) {
  int16_t value;
  //uint8_t i = idx*4;//0-->0
  //value = (sensorData[i+4] | sensorData[i+5]<<8);
  value = acc_buffer_z[idx];
  return value;
}

void AmuletSetClock(ClockType type, uint16_t value, uint8_t requstor) {
  uint8_t month, day, hour, min, sec, weekday;
  uint16_t year;
  CoreClock(&year, &month, &day, &hour, &min, &sec, &weekday);
  switch (type) {
    case YEAR:
      CoreSetClock(value, month, day, hour, min, sec, weekday);
      break;
    case MONTH:
      CoreSetClock(year, (uint8_t)value, day, hour, min, sec, weekday);
      break;
    case DAY:
      CoreSetClock(year, month, (uint8_t)value, hour, min, sec, weekday);
      break;
    case HOUR:
      CoreSetClock(year, month, day, (uint8_t)value, min, sec, weekday);
      break;
    case MINUTE:
      CoreSetClock(year, month, day, hour, (uint8_t)value, sec, weekday);
      break;
    case SECOND:
      CoreSetClock(year, month, day, hour, min, (uint8_t)value, weekday);
      break;
    case WEEKDAY:
      CoreSetClock(year, month, day, hour, min, sec, (uint8_t)value);
      break;
  }
  return;
}

/*****************************************************************************
 * CLOCK
 *****************************************************************************/

uint8_t AmuletClock(ClockType type, uint8_t requstor) {
    uint8_t retval = 0;
    uint8_t month, day, hour, min, sec, weekday;
    uint16_t year;
    CoreClock(&year, &month, &day, &hour, &min, &sec, &weekday);
    switch(type) {
    case YEAR:
        //Only return 2 digit year since return type is uint8_t
        retval = year-2000;
        break;
    case MONTH:
        retval = month;
        break;
    case DAY:
        retval = day;
        break;
    case HOUR:
        retval = hour;
        break;
    case MINUTE:
        retval = min;
        break;
    case SECOND:
        retval = sec;
        break;
    case WEEKDAY:
        retval = weekday;
        break;
    }
    return retval;
}

void AmuletDateTimeText(__char_array target, uint8_t requestor){
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    uint8_t month, day, hour, min, sec, weekday;
    uint16_t year;
    CoreClock(&year, &month, &day, &hour, &min, &sec, &weekday);
    uint8_t short_year = year - 2000;

    char zero[2] = "0";
    char colon[2] = ":";
    char slash[2] = "/";
    char space[2] = " ";
    char mon[3];
    char d[3];
    char y[3];
    char h[3];
    char m[3];
    char s[3];
    char am[3] = "AM";
    char pm[3] = "PM";
    bool is_pm = false;

    itoa(month,mon, 10);
    itoa(day,d,10);
    itoa(short_year,y,10);
    if (hour >= 12) {
      hour -= 12;
      is_pm = true;
    }
    if (hour == 0) hour = 12;
    itoa(hour,h,10);
    itoa(min,m,10);
    itoa(sec,s,10);

    strcpy(target.values,mon);
    strcat(target.values,slash);
    if (day < 10) strcat(target.values,zero);
    strcat(target.values,d);
    strcat(target.values,slash);
    if (short_year < 10) strcat(target.values,zero);
    strcat(target.values,y);
    strcat(target.values,space);
    strcat(target.values,h);
    strcat(target.values,colon);
    if (min < 10) strcat(target.values,zero);
    strcat(target.values,m);
    strcat(target.values,colon);
    if (sec < 10) strcat(target.values,zero);
    strcat(target.values,s);
    strcat(target.values,space);
    if (is_pm == true)
      strcat(target.values,pm);
    else
      strcat(target.values,am);


    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

void AmuletClockText(__char_array target1, __char_array target2, __char_array target3, uint8_t requestor){
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    uint8_t month, day, hour, min, sec, weekday;
    uint16_t year;
    CoreClock(&year, &month, &day, &hour, &min, &sec, &weekday);
    //bsp_printf("%d:%d:%d, %d, %d-%d-%d\n\r",hour,min,sec,weekday, month,day,year);

    // TIANLONG: this should work just the same in less code: please test.
    __char_array fmt = { .values = "%02d:%02d:%02d", .__arr_len = 8 };
    AmuletSprintf(target1, fmt, hour, min, sec);

    // if (sec>=10 && min>=10)
    //     AmuletSprintf(target1, "%d:%d:%d", hour, min, sec);
    // else if (sec<10 && min>=10)
    //     AmuletSprintf(target1, "%d:%d:0%d", hour, min, sec);
    // else if (sec>=10 && min<10)
    //     AmuletSprintf(target1, "%d:0%d:%d", hour, min, sec);
    // else
    //     AmuletSprintf(target1, "%d:0%d:0%d", hour, min, sec);

    switch (weekday) { // Decode day of week
    case 0:
        strcpy(target2.values, "SUNDAY");
        break;
    case 1:
        strcpy(target2.values, "MONDAY");
        break;
    case 2:
        strcpy(target2.values, "TUESDAY");
        break;
    case 3:
        strcpy(target2.values, "WEDNESDAY");
        break;
    case 4:
        strcpy(target2.values, "THURSDAY");
        break;
    case 5:
        strcpy(target2.values, "FRIDAY");
        break;
    case 6:
        strcpy(target2.values, "SATURDAY");
        break;
    default:
        break;
    }
    // TIANLONG: this should work just the same in less code: please test.
    __char_array fmt2 = { .values = "%02d-%02d-%02d", .__arr_len = 5 };
    AmuletSprintf(target3, fmt2, month+1, day, year);
    // AmuletSprintf(target3, "%d-%d", month+1, day);
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

void AmuletDateText(__char_array target, uint8_t requestor){
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    uint16_t year;
    uint8_t month, day, hour, min, sec, weekday;
    CoreClock(&year, &month, &day, &hour, &min, &sec, &weekday);

    // TIANLONG: this should work just the same in less code: please test.
    __char_array fmt = { .values = "%02d-%02d-%02d", .__arr_len = 8 };
    AmuletSprintf(target, fmt, month+1, day, year);
    // AmuletSprintf(target, "%d-%d-%d", month, day, year);
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

void AmuletWeekText(__char_array target, uint8_t requestor){
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    uint8_t month, day, hour, min, sec, weekday;
    uint16_t year;
    CoreClock(&year, &month, &day, &hour, &min, &sec, &weekday);

    switch (weekday) { // Decode day of week
    case 0:
        strcpy(target.values, "SUNDAY");
        break;
    case 1:
        strcpy(target.values, "MONDAY");
        break;
    case 2:
        strcpy(target.values, "TUESDAY");
        break;
    case 3:
        strcpy(target.values, "WEDNESDAY");
        break;
    case 4:
        strcpy(target.values, "THURSDAY");
        break;
    case 5:
        strcpy(target.values, "FRIDAY");
        break;
    case 6:
        strcpy(target.values, "SATURDAY");
        break;
    default:
        break;
    }
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

/*****************************************************************************
 * TIMER
 *****************************************************************************/

void AmuletTimer(uint16_t interval, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreTimer(interval, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

void AmuletTimerCancel(uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
  #endif
	CoreTimerCancel(requestor);
  #ifdef PROFLILNG
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

/* ************************************************************************* *
 * UI
 * ************************************************************************* */

//
// DISPLAY
//
void __helper_AmuletSetFont(FontType font, uint8_t requestor) {
  switch(font) {
    case BOLD_FONT:
      CoreSetFont(SourceSansProBlack15, requestor);
      break;
    case MEDIUM_FONT:
      CoreSetFont(SourceSansProBold10, requestor);
      break;
    case REGULAR_FONT:
      CoreSetFont(SourceSansProBold7, requestor);
      break;
  }
}

void AmuletBoldText(uint8_t x, uint8_t y, __char_array message, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
  #endif
  __helper_AmuletSetFont(BOLD_FONT, requestor);
  CoreText(x, y, message.values, requestor);
  #ifdef PROFLILNG
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletMediumText(uint8_t x, uint8_t y, __char_array message, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
  #endif
  __helper_AmuletSetFont(MEDIUM_FONT, requestor);
  CoreText(x, y, message.values, requestor);
  #ifdef PROFLILNG
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletRegularText(uint8_t x, uint8_t y, __char_array message, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
  #endif
  __helper_AmuletSetFont(REGULAR_FONT, requestor);
  CoreText(x, y, message.values, requestor);
  #ifdef PROFLILNG
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletBoldCenteredText(uint8_t y,  __char_array message, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
  #endif
  __helper_AmuletSetFont(BOLD_FONT, requestor);    
  CoreCenteredText(y, message.values, requestor);
  #ifdef PROFLILNG
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletMediumCenteredText(uint8_t y,  __char_array message, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
  #endif
  __helper_AmuletSetFont(MEDIUM_FONT, requestor);  
  CoreCenteredText(y, message.values, requestor);
  #ifdef PROFLILNG
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletRegularCenteredText(uint8_t y,  __char_array message, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
  #endif
  __helper_AmuletSetFont(REGULAR_FONT, requestor);
  CoreCenteredText(y, message.values, requestor);
  #ifdef PROFLILNG
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletDrawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawRect(x, y, w, h, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletClearRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreClearRect(x, y, w, h, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletFillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreFillRect(x, y, w, h, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletPushChangesToDisplay(uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CorePushChangesToDisplay(requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

uint8_t AmuletTextWidth(__char_array message, uint8_t requestor) {  
  uint8_t retval;
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  retval = textWidth(message.values);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
  return retval;
}

uint8_t AmuletTextHeight(uint8_t requestor) {
  uint8_t retval;
  retval = textHeight(); 
  return retval;
}

void AmuletDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawLine(x0, y0, x1, y1, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletDrawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawFastVLine(x, y,  h, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletDrawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawFastHLine( x,  y,  w,  requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletDrawHeartImage(uint8_t x,uint8_t y, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawHeartImage(x, y, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletDrawWalkerImage(uint8_t x,uint8_t y, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawWalkerImage(x, y, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

uint8_t AmuletDrawingAreaWidth() {
  return CoreDrawingAreaWidth();
}

uint8_t AmuletDrawingAreaHeight() {
  return CoreDrawingAreaHeight();
}

int8_t AmuletAddGraph(GraphType type, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                uint8_t data_min, uint8_t data_max, uint8_t requestor) {
  int8_t retval;
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  retval = CoreAddGraph(type, x, y, w, h, data_min, data_max);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif    
  return retval;
}

void AmuletAddGraphData(uint8_t data, uint8_t graph_id, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreAddGraphData(data, graph_id);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletDrawGraph(uint8_t graph_id, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawGraph(graph_id, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif  
}

void AmuletDisplayMessage(__char_array message, uint8_t line, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreDisplayMessage(message.values, message.__arr_len, line, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

void AmuletDisplayClr(uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreDisplayClr(requestor);
  CoreRefreshStatus(requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

void AmuletDisplayClrLN(uint8_t lineNumber, uint8_t requestor){
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreDisplayClrLN(lineNumber, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

void AmuletDrawSunImage(uint8_t x,uint8_t y, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
  CoreDrawSunImage(x, y, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

//
// BUTTONS
//

// TODO: need to add check if e is of type AmuletButtonEvt...
/*uint8_t AmuletGetButtonPressedId(QEvt const * const e, uint8_t requestor) {
	AmuletButtonEvt *bEvt = e;
	return (uint8_t) bEvt->buttonId;
}*/

ButtonSide AmuletButtonSide(uint8_t id, uint8_t requestor) {
	if (id == BUTTON_L0 || id == BUTTON_L1) {
		return LEFT;
	} else if (id == BUTTON_R0 || id == BUTTON_R1) {
		return RIGHT;
	} else {
		return INVALID_SIDE;
	}
}

//
// LEDs
//

void AmuletLEDOff(LED ledId, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreSetLed(ledId, LED_OFF, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

void AmuletLEDOn(LED ledId, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreSetLed(ledId, LED_ON, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

void AmuletLEDBlink(LED ledId, uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	// need to use timer to blink
	CoreSetLed(ledId, LED_BLINK, requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

/**
 * 
 * Turns on the capacitive touch timers
 * @param requestor [description]
 */
void AmuletTurnOnCapTouch(uint8_t requestor) {
  CoreTurnOnCapTouch(requestor);
}

/**
 * Turns off the capacitive touch timers
 * @param requestor [description]
 */
void AmuletTurnOffCapTouch(uint8_t requestor) {
  CoreTurnOffCapTouch(requestor);
}

//
// HAPTIC BUZZER
//

void AmuletHapticSingleBuzz(uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreHapticSingleBuzz(requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

void AmuletHapticDoubleBuzz(uint8_t requestor) {
  #ifdef PROFLILNG
    PROFLILNG_PORT |= PROFLILNG_PIN;
    for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #endif
	CoreHapticDoubleBuzz(requestor);
  #ifdef PROFLILNG
  }
    busy_wait(PROFLILNG_DELAY_BEFORE);
    PROFLILNG_PORT &= (~PROFLILNG_PIN);
    busy_wait(PROFLILNG_DELAY);
  #endif
}

/*****************************************************************************
 * SYSTEM
 *****************************************************************************/

/* INTERNAL FUNCTION: checkCanChangeSysMode().
 *
 * TODO: IMPLEMENT RUNTIME CHECKS
 *  -- ER app can do whatever it wants...
 *    --> Only ER app can put system back in normal mode after emergency maybe/emergency mode.
 * -- System app can toggle debug mode/normal mode.
 * -- Other apps can possbly put system into emergency maybe mode.
 *
 */
static bool checkCanChangeSysMode(SystemMode newMode, uint8_t requestor) {
    return true;

    // SystemMode currMode = CoreGetSystemMode();
    // App * requestingApp = getAppById(requestor);

    // If...
    //  - currently in an emergency (or possible emergency)
    //  - requesting app has appType TYPE_EMERGENCY_APP
    // Then, this app is free to change the system mode as it sees fit (likely,
    // setting app from emerency to normal or from emergency-maybe to emergency).
    // if((mode == EMERGENCY_MODE || mode == EMERGENCY_MAYBE_MODE) && (requestingApp->appType & TYPE_EMERGENCY_APP)) {
    //  return TRUE;
    // }

    // // System change requirements not met -- request to change system mode denied.
    // return FALSE;
}

bool AmuletIsForegroundApp(uint8_t requestor) {
    return (CoreGetForegroundAppId() == requestor);
}

void AmuletEnterNormalMode(uint8_t requestor) {
	if(checkCanChangeSysMode(NORMAL_MODE, requestor)) {
		CoreSetSystemMode(NORMAL_MODE);
	} else {
		//bsp_printf("ERROR: only TYPE_EMERGENCY_APP can return system mode to NORMAL during an emergency.\r\n");
	}
}

bool AmuletInNormalMode(uint8_t requestor) {
	return (CoreGetSystemMode() == NORMAL_MODE);
}

void AmuletEnterEmergencyMaybeMode(uint8_t requestor) {
	CoreSetSystemMode(EMERGENCY_MAYBE_MODE);
	CoreBringERAppForward();
}

bool AmuletInEmergencyMaybeMode(uint8_t requestor) {
	return (CoreGetSystemMode() == EMERGENCY_MAYBE_MODE);
}

void AmuletEnterEmergencyMode(uint8_t requestor) {
	CoreSetSystemMode(EMERGENCY_MODE);
	CoreBringERAppForward();
}

bool AmuletInEmergencyMode(uint8_t requestor) {
	return (CoreGetSystemMode() == EMERGENCY_MODE);
}

void AmuletEnterDebugMode(uint8_t requestor) {
	CoreSetSystemMode(DEBUG_MODE);
};

bool AmuletInDebugMode(uint8_t requestor) {
	return (CoreGetSystemMode() == DEBUG_MODE);
}

bool AmuletRequestMoveToFront(uint8_t requestor) {
  // 1: code to check if request should be granted/denied --> for now, do
  // nothing since this call always succeeds (i.e., is always permitted with no
  // further rules so long as the app passes compilation).

  // 2: if request is granted, make any necessary updates to internal state and the display.
  bool result = CoreRequestMoveToFront(requestor);

  // 3: return result of the request as a bool value.
  return result;
}

/*****************************************************************************
 * STORAGE
 *****************************************************************************/

uint8_t AmuletSDPresent(uint8_t requestor) {
    uint8_t rv;
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    rv = SDExist();
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
    return rv;
}

/*
 * IS THIS A BLOCKING ROUTINE NOW THAT WE AREN'T USING STATE MACHINES?!
 */
uint8_t AmuletLogAppend(uint8_t log_name, __char_array line_contents, uint8_t requestor) {
    uint8_t rv;
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    rv = LogAppend(log_name, line_contents.values, line_contents.__arr_len, requestor);
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
    return rv;
}

/*
 * IS THIS A BLOCKING ROUTINE NOW THAT WE AREN'T USING STATE MACHINES?!
 */
uint8_t AmuletLogRead(uint8_t log_name, __char_array line_contents, uint8_t start_line, int n_lines, uint8_t requestor) {
    uint8_t rv;
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    rv = LogRead(log_name, line_contents.values, line_contents.__arr_len, start_line, n_lines, requestor);
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
    return rv;
}

/*****************************************************************************
 * QP WRAPPERS
 *****************************************************************************/

void AmuletStateTransition(QActive * const me, int sig, uint8_t requestor) {
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      // These APIs' behavior will be quite different if call multiple times
      // do I didn't put the for loop here.
    #endif
    AmuletEvt *evt;
    evt = Q_NEW(AmuletEvt, sig);
    QACTIVE_POST(me, &evt->super, NULL);
    #ifdef PROFLILNG
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

/*****************************************************************************
 * UTILITY & DEBUG FUNCTIONS
 *****************************************************************************/

// TODO: add API for apps to get their name/short name.
// void AmuletGetAppName(uint8_t requestor) {
//  App * requestingApp = getAppById(requestor);
//  return requestingApp->appName;
// }

// void AmuletGetAppShortName(uint8_t requestor) {
//  App * requestingApp = getAppById(requestor);
//  return requestingApp->appShortName;
// }

void AmuletITOA(int16_t source, __char_array target, uint8_t requestor ){
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    char result[10];
    itoa(source, result,10);
    strcpy(target.values, result);
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

void AmuletConcat(__char_array target, __char_array to_add, uint8_t requestor) {
    #ifdef PROFLILNG
      PROFLILNG_PORT |= PROFLILNG_PIN;
      for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
    #endif
    strcat(target.values, to_add.values);
    #ifdef PROFLILNG
    }
      busy_wait(PROFLILNG_DELAY_BEFORE);
      PROFLILNG_PORT &= (~PROFLILNG_PIN);
      busy_wait(PROFLILNG_DELAY);
    #endif
}

void AmuletSprintf(__char_array target, __char_array fmt, ... ) {
    va_list va;
    va_start(va, fmt);
    sprintf(target.values, fmt.values, va);
}

float AmuletGetStdDev(__float_array rrs,  float total_time, uint8_t numBeats, uint8_t requestor){
  return getStdDev(rrs.values,  total_time, numBeats);
}

/*****************************************************************************
 * Functions for SIFT APP
 * Author: Hang
 *****************************************************************************/
float AmuletStringToFloat(__char_array source, uint8_t requstor)
{
  char* s = source.values;
  //char* s ="1.0592";

  int sign = 1;
  if (s[0] == '-'){
      sign = -1;
  }

    int n1=0,n2=0,i,t,c=1;
    float f1,f2;
    for(i=0;s[i]!='.';i++)
    {
        if((s[i]>=48)&&(s[i]<=57))
        {
            n1=n1*10 +(s[i]-48);
        }
    }
    int len;
    int float_len;
    len = strlen(source.values);
    float_len = (int)len;
    float_len = float_len - (i+1);

    int flag = 1;
    for(unsigned int kk=i+1;kk< source.__arr_len;kk++)
    {
        if((s[kk]>=48)&&(s[kk]<=57))
        {
            n2=n2*10 +(s[kk]-48);
        }
        /*this is trying to see how many zeros immediately after decimal point*/
        if((s[kk]==48)&&(flag == 1)){
            c = c*10;
        }
        else{
            flag = 0;
        }

    }
    t=n2;
    f1=n1;
    f2=n2;

    while(t)
    {
        t=t/10;
        c=c*10;
    }

    f2=f2/c;
    return sign==1?(f1+f2):-(f1+f2);
}

/*This function only allow the precision as 4 digits after decimal*/
void AmuletFloatToString(float f, __char_array target, uint8_t requestor){
    int count;
    char* curr;
    uint16_t value;
    int sign = 1;
    int len;
    int pos;
    char temp[10];
    char temp2[10];
    char temp3[10];
    int i;
    int j;
    int precision = 4;//need to modify, depends on how many digits after decimal

    if(f - 0.000001 > 0.000001)
        sign = 1;
    else{
        sign = -1;
        f = f*sign;// make f to a postive number, fitting for unsigned int
    }

    value = (uint16_t)f;
    itoa(value, target.values, target.__arr_len);

    len = strlen(target.values);  // find out how big the integer part was
    /*Add the sign if it is a negative number*/
    if(sign == -1){
        temp[0] = '-';
        strcpy(temp2, target.values);
        for(i = 1; i<=len;i++){
            j = i -1;
            temp[i] = temp2[j];
        }
        temp[i] = '\0';
        strcpy(target.values,temp);
        /*for(i = 1; i<=len;i++){
            j = i - 1;
            temp[i] = target.values[j];
        }
        temp[i] = NULL;
        strcpy(target.values,temp);*/
    }

    len = strlen(target.values);
    target.values[len] = '.';
    pos = len + 1;
    //target.values[pos] = NULL;


    for(i = 0; i<precision;i++){
        f = f - value;
        f *= 10;
        value = (uint16_t)f;
        itoa(value, temp3, target.__arr_len);
        target.values[pos] = temp3[0];
        pos++;
    }

    target.values[pos] = '\0';

}

uint16_t AmuletATOI(__char_array source, uint8_t requstor) {
    char *p = source.values;
    uint8_t k = 0, i = 0;

    for(i=0;i<source.__arr_len;i++)
    {
        if((p[i]>=48)&&(p[i]<=57)){
            k = k*10 + p[i] - '0';
        }
    }


    return k;
}

uint16_t AmuletReadfromFile(uint8_t log_name, __char_array line_contents, uint16_t start_line, int n_lines, uint8_t requestor) {
    return LogRead2(log_name, line_contents.values, line_contents.__arr_len, start_line, n_lines, requestor);
}

/**
 * Seconds since the amulet started up
 */
uint32_t AmuletSeconds(uint8_t requestor) {
  return CoreSeconds(requestor);
}

#ifdef SIFT_DATA_HACK
void AmuletGetECG(__float_array target, uint8_t requestor){
  uint16_t i = 0;
  for (i = 0; i < target.__arr_len;i++){
    target.values[i] = ecg[i];
  }
}

void AmuletGetABP(__float_array target, uint8_t requestor){
  uint16_t i = 0;
  for (i = 0; i < target.__arr_len;i++){
    target.values[i] = abp[i];
  }
}

void AmuletGetRPKS(__uint8_t_array target, uint8_t requestor){
  uint16_t i = 0;
  for (i = 0; i < target.__arr_len;i++){
    target.values[i] = rpks_annotation[i];
  }
}

void AmuletGetSPKS(__uint8_t_array target, uint8_t requestor){
  uint16_t i = 0;
  for (i = 0; i < target.__arr_len;i++){
    target.values[i] = spks_annotation[i];
  }
}
#endif
