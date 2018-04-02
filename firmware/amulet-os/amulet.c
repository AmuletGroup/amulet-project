#include "amulet.h"

//#define PROFLILNG
#define PROFLILNG_DELAY 800
#define PROFLILNG_DELAY_BEFORE 0
#define PROFLILNG_LOOP 1

#ifdef PROFLILNG
  #define PROFILING_PRE_NO_LOOP PROFLILNG_PORT |= PROFLILNG_PIN; {
  #define PROFILING_PRE PROFLILNG_PORT |= PROFLILNG_PIN; for(uint16_t k=0;k<PROFLILNG_LOOP;k++){
  #define PROFILING_POST } do{busy_wait(PROFLILNG_DELAY_BEFORE); \
                              PROFLILNG_PORT &= (~PROFLILNG_PIN);\
                              busy_wait(PROFLILNG_DELAY);} while(0)
#else
  #define PROFILING_PRE_NO_LOOP
  #define PROFILING_PRE
  #define PROFILING_POST
#endif


extern void itoa(int16_t value, char* result, int base);

uint16_t lfsr = 11387432;
uint16_t rand_bit;

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
    PROFILING_PRE
    SubscribeSensor(sensor_id, expiration, window_size, window_interval, requestor);
    PROFILING_POST
}

void AmuletUnsubscribeInternalSensor(uint8_t sensor_id, uint8_t requestor) {
    PROFILING_PRE

    UnsubscribeSensor(sensor_id, requestor);

    PROFILING_POST
}

// BLE packets from HRM arrive once per second, but they can contain
// multiple RR values since a heart rate can be above 60bpm (once per
// second).  So AmuletGetHRandRRI() returns HR, multiple RRI values, plus
// other info:
//
//  Index ValueReturned
//   0    Heart rate
//   1    1st RRI
//   2    2nd RRI
//   ...  ...
//   7    7th RRI
//   8    expended energy
//   9    sensor in contact with skin
//
// Any nonexistent RRI values will be returned as zero.
// Valid index values are defined by an enum in amulet.h:
//    HRM_HR = 0,
//    HRM_RRI1 = 1,
//    HRM_RRI2 = 2,
//    HRM_RRI3 = 3,
//    HRM_RRI4 = 4,
//    HRM_RRI5 = 5,
//    HRM_RRI6 = 6,
//    HRM_RRI7 = 7,
//    HRM_ENERGY = 8,
//    HRM_CONTACT = 9
//
uint16_t AmuletGetHRandRRI(uint8_t idx, uint8_t requestor) {
  uint16_t rv=0;

  // Sanity check on idx value

  if((idx >= 0) && (idx <= 9))
  {
    // Byte 0 of sensorData is sensor type
    // Byte 1+2 are heart rate
    // Byte 3+4 are RRI #1, etc.
    rv = (sensorData[1+(idx*2)] + (sensorData[2+(idx*2)]<<8));
  }
  return rv;
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

  PROFILING_PRE

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

  PROFILING_POST

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

    PROFILING_PRE

    battery_reading = get_battery_level();

    PROFILING_POST

    return battery_reading;
}

uint16_t AmuletGetLightLevel(uint8_t requestor) {
    uint16_t light_reading;

    PROFILING_PRE

    light_reading = take_light_reading();

    PROFILING_POST

    return light_reading;
}

uint16_t AmuletGetTemperature(uint8_t requestor) {
    uint16_t temp_reading;

    PROFILING_PRE

    temp_reading = get_temperature();

    PROFILING_POST

    return temp_reading;
}

uint16_t AmuletGetAudio(uint8_t requestor) {
    uint16_t audio_reading;

    PROFILING_PRE

    audio_reading = take_audio_reading();

    PROFILING_POST

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

int16_t AmuletGetGyroX(uint8_t idx, uint8_t requestor) {
  int16_t value;
  //uint8_t i = idx*4;//0-->0
  //value = (sensorData[i] | sensorData[i+1]<<8);
  value = gyro_buffer_x[idx];
  return value;
}

int16_t AmuletGetGyroY(uint8_t idx, uint8_t requestor) {
  int16_t value;
  //uint8_t i = idx*4;//0-->0
  //value = (sensorData[i+2] | sensorData[i+3]<<8);
  value = gyro_buffer_y[idx];
  return value;
}

int16_t AmuletGetGyroZ(uint8_t idx, uint8_t requestor) {
  int16_t value;
  //uint8_t i = idx*4;//0-->0
  //value = (sensorData[i+4] | sensorData[i+5]<<8);
  value = gyro_buffer_z[idx];
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


    PROFILING_PRE

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


    PROFILING_POST
}

void AmuletClockText(__char_array target1, __char_array target2, __char_array target3, uint8_t requestor){

    PROFILING_PRE

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

    PROFILING_POST
}

void AmuletDateText(__char_array target, uint8_t requestor){

    PROFILING_PRE

    uint16_t year;
    uint8_t month, day, hour, min, sec, weekday;
    CoreClock(&year, &month, &day, &hour, &min, &sec, &weekday);

    // TIANLONG: this should work just the same in less code: please test.
    __char_array fmt = { .values = "%02d-%02d-%02d", .__arr_len = 8 };
    AmuletSprintf(target, fmt, month+1, day, year);
    // AmuletSprintf(target, "%d-%d-%d", month, day, year);

    PROFILING_POST
}

void AmuletWeekText(__char_array target, uint8_t requestor){

    PROFILING_PRE

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

    PROFILING_POST
}

/*****************************************************************************
 * TIMER
 *****************************************************************************/

void AmuletTimer(uint16_t interval, uint8_t requestor) {

  PROFILING_PRE

	CoreTimer(interval, requestor);

  PROFILING_POST
}

void AmuletTimerCancel(uint8_t requestor) {

  PROFILING_PRE_NO_LOOP

	CoreTimerCancel(requestor);

  PROFILING_POST
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
  PROFILING_PRE_NO_LOOP

  __helper_AmuletSetFont(BOLD_FONT, requestor);
  CoreText(x, y, message.values, requestor);

  PROFILING_POST
}

void AmuletMediumText(uint8_t x, uint8_t y, __char_array message, uint8_t requestor) {

  PROFILING_PRE_NO_LOOP

  __helper_AmuletSetFont(MEDIUM_FONT, requestor);
  CoreText(x, y, message.values, requestor);

  PROFILING_POST
}

void AmuletRegularText(uint8_t x, uint8_t y, __char_array message, uint8_t requestor) {

  PROFILING_PRE_NO_LOOP

  __helper_AmuletSetFont(REGULAR_FONT, requestor);
  CoreText(x, y, message.values, requestor);

  PROFILING_POST
}

void AmuletBoldCenteredText(uint8_t y,  __char_array message, uint8_t requestor) {

  PROFILING_PRE_NO_LOOP

  __helper_AmuletSetFont(BOLD_FONT, requestor);
  CoreCenteredText(y, message.values, requestor);

  PROFILING_POST
}

void AmuletMediumCenteredText(uint8_t y,  __char_array message, uint8_t requestor) {

  PROFILING_PRE_NO_LOOP

  __helper_AmuletSetFont(MEDIUM_FONT, requestor);
  CoreCenteredText(y, message.values, requestor);

  PROFILING_POST
}

void AmuletRegularCenteredText(uint8_t y,  __char_array message, uint8_t requestor) {

  PROFILING_PRE_NO_LOOP

  __helper_AmuletSetFont(REGULAR_FONT, requestor);
  CoreCenteredText(y, message.values, requestor);

  PROFILING_POST
}

void AmuletDrawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawRect(x, y, w, h, requestor);

  PROFILING_POST
}

void AmuletClearRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t requestor) {

  PROFILING_PRE

  CoreClearRect(x, y, w, h, requestor);

  PROFILING_POST
}

void AmuletFillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t requestor) {

  PROFILING_PRE

  CoreFillRect(x, y, w, h, requestor);

  PROFILING_POST
}

void AmuletPushChangesToDisplay(uint8_t requestor) {

  PROFILING_PRE

  CorePushChangesToDisplay(requestor);

  PROFILING_POST
}

uint8_t AmuletTextWidth(__char_array message, uint8_t requestor) {
  uint8_t retval;

  PROFILING_PRE

  retval = textWidth(message.values);

  PROFILING_POST

  return retval;
}

uint8_t AmuletTextHeight(uint8_t requestor) {
  uint8_t retval;
  retval = textHeight();
  return retval;
}

void AmuletDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawLine(x0, y0, x1, y1, requestor);

  PROFILING_POST
}

void AmuletDrawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawFastVLine(x, y,  h, requestor);

  PROFILING_POST
}

void AmuletDrawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawFastHLine( x,  y,  w,  requestor);

  PROFILING_POST
}

void AmuletDrawHeartImage(uint8_t x,uint8_t y, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawHeartImage(x, y, requestor);

  PROFILING_POST
}

void AmuletDrawWalkerImage(uint8_t x,uint8_t y, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawWalkerImage(x, y, requestor);

  PROFILING_POST
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

  PROFILING_PRE

  retval = CoreAddGraph(type, x, y, w, h, data_min, data_max);

  PROFILING_POST
  return retval;
}

void AmuletAddGraphData(uint8_t data, uint8_t graph_id, uint8_t requestor) {

  PROFILING_PRE

  CoreAddGraphData(data, graph_id);

  PROFILING_POST
}

void AmuletDrawGraph(uint8_t graph_id, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawGraph(graph_id, requestor);

  PROFILING_POST
}

void AmuletDisplayMessage(__char_array message, uint8_t line, uint8_t requestor) {

  PROFILING_PRE

	CoreDisplayMessage(message.values, message.__arr_len, line, requestor);

  PROFILING_POST
}

void AmuletDisplayClr(uint8_t requestor) {

  PROFILING_PRE

	CoreDisplayClr(requestor);
  CoreRefreshStatus(requestor);

  PROFILING_POST
}

void AmuletDisplayClrLN(uint8_t lineNumber, uint8_t requestor){

  PROFILING_PRE

	CoreDisplayClrLN(lineNumber, requestor);

  PROFILING_POST
}

void AmuletDrawSunImage(uint8_t x,uint8_t y, uint8_t requestor) {

  PROFILING_PRE

  CoreDrawSunImage(x, y, requestor);

  PROFILING_POST
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

  PROFILING_PRE

	CoreSetLed(ledId, LED_OFF, requestor);

  PROFILING_POST
}

void AmuletLEDOn(LED ledId, uint8_t requestor) {

  PROFILING_PRE

	CoreSetLed(ledId, LED_ON, requestor);

  PROFILING_POST
}

void AmuletLEDBlink(LED ledId, uint8_t requestor) {

  PROFILING_PRE

	// need to use timer to blink
	CoreSetLed(ledId, LED_BLINK, requestor);

  PROFILING_POST
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

  PROFILING_PRE

  CoreHapticSingleBuzz(requestor);

  PROFILING_POST
}

void AmuletHapticDoubleBuzz(uint8_t requestor) {

  PROFILING_PRE

  CoreHapticDoubleBuzz(requestor);

  PROFILING_POST
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

uint64_t AmuletGetUUID(uint8_t requestor) {

    uint64_t * const uuidPtr = (uint64_t * const) AMULET_UUID_ADDR;
    return *uuidPtr;
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

    PROFILING_PRE

    rv = SDExist();

    PROFILING_POST

    return rv;
}

/*
 * IS THIS A BLOCKING ROUTINE NOW THAT WE AREN'T USING STATE MACHINES?!
 */
uint8_t AmuletLogAppend(uint8_t log_name, __char_array line_contents, uint8_t requestor) {
    uint8_t rv;

    PROFILING_PRE

    rv = LogAppend(log_name, line_contents.values, line_contents.__arr_len, requestor);

    PROFILING_POST
    return rv;
}

// uint8_t AmuletLogTextData(uint8_t log_name, char *field_name, __char_array
// data,
//                           uint8_t requestor) {
//   uint8_t rv;
// #ifdef PROFILING
//   PROFILING_PORT |= PROFILING_PIN;
//   for (uint16_t k = 0; k < PROFILING_LOOP; k++) {
// #endif
//     rv = LogTextData(log_name, field_name, data.values, requestor);
// #ifdef PROFILING
//   }
//   busy_wait(PROFILING_DELAY_BEFORE);
//   PROFILING_PORT &= (~PROFILING_PIN);
//   busy_wait(PROFILING_DELAY);
// #endif
//   return rv;
// }
//
// uint8_t AmuletLogIntData(uint8_t log_name, char *field_name, uint16_t value,
//                          uint8_t requestor) {
//   uint8_t rv;
// #ifdef PROFILING
//   PROFILING_PORT |= PROFILING_PIN;
//   for (uint16_t k = 0; k < PROFILING_LOOP; k++) {
// #endif
//     rv = LogIntData(log_name, field_name, value, requestor);
// #ifdef PROFILING
//   }
//   busy_wait(PROFILING_DELAY_BEFORE);
//   PROFILING_PORT &= (~PROFILING_PIN);
//   busy_wait(PROFILING_DELAY);
// #endif
//   return rv;
// }

void AmuletStartLogTransaction() {
  CoreStartLogTransaction();
}

uint8_t AmuletCommitLogTransaction() {
  return CoreCommitLogTransaction();
}


/*
 * value_headers: the title for each column of values being logged separated
 *                by commas. e.g. for BatteryMeter, it could be "batt_%, adc"
 * data: all data to be logged; in the form of data[0] = log id,
 *       [1] = number of data values, [3] = data values
 */
uint8_t AmuletLogData(uint16_t logid, __char_array value_headers, __float_array data, 
                      uint8_t requestor) {
  uint8_t rv;
  PROFILING_PRE

    rv = LogData(logid,value_headers.values, data.values, data.__arr_len);

  PROFILING_POST

  return rv;
}

/*****************************************************************************
 * QP WRAPPERS
 *****************************************************************************/

void AmuletStateTransition(QActive * const me, int sig, uint8_t requestor) {

    PROFILING_PRE_NO_LOOP

    AmuletEvt *evt;
    evt = Q_NEW(AmuletEvt, sig);
    QACTIVE_POST(me, &evt->super, NULL);

    PROFILING_POST
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

void AmuletWrapText(__char_array target, uint16_t wrapline, uint8_t requestor ){

    PROFILING_PRE
    char s[150];
    strcpy(s,target.values);
    int lastwrap = 0; // saves character index after most recent line wrap
    int wraploc = 0; // used to find the location for next word wrap
    for (int i = 0; s[i] != '\0'; ++i, ++wraploc) {

        if (wraploc >= wrapline) {
            for (int k = i; k > 0; --k) {
                // make sure word wrap doesn't overflow past maximum length
                if (k - lastwrap <= wrapline && s[k] == ' ') {
                    s[k] = '\n';
                    lastwrap = k+1;
                    break;
                }
            }
            wraploc = i-lastwrap;
        }

    } // end main loop
    strcpy(target.values, s);

    PROFILING_POST
}

void AmuletITOA(int16_t source, __char_array target, uint8_t requestor ){

    PROFILING_PRE

    char result[10];
    itoa(source, result,10);
    strcpy(target.values, result);

    PROFILING_POST
}

void AmuletConcat(__char_array target, __char_array to_add, uint8_t requestor) {

    PROFILING_PRE

    strcat(target.values, to_add.values);

    PROFILING_POST
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

uint16_t AmuletRand(uint16_t seed, uint8_t requestor){
  if (seed>10000){
    lfsr=seed;
  }
  rand_bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  return lfsr =  (lfsr >> 1) | (rand_bit << 15);
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