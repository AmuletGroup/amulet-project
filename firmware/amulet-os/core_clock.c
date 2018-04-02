#include <string.h>

#include "core_clock.h"
#include "rtc.h"

extern void itoa(int16_t value, char *result, int base);
extern uint32_t amulet_startup_timer;

uint32_t CoreSeconds(uint8_t requestor) { return amulet_startup_timer; }

void CoreClock(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour,
               uint8_t *min, uint8_t *sec, uint8_t *weekday) {
  get_rtc_values(year, month, day, hour, min, sec, weekday);
}

void CoreSetClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                  uint8_t min, uint8_t sec, uint8_t weekday) {
  set_rtc_values(year, month, day, hour, min, sec, weekday);
}

void CoreTimeStamp(char *timestamp) {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t weekday;
  get_rtc_values(&year, &month, &day, &hour, &min, &sec, &weekday);
  uint8_t short_year = year - 2000;

  timestamp[0] = short_year;
  timestamp[1] = month;
  timestamp[2] = day;
  timestamp[3] = hour;
  timestamp[4] = min;
  timestamp[5] = sec;
  timestamp[6] = 0;
}

void CoreTimeStampFile(char *timestamp) {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t weekday;
  get_rtc_values(&year, &month, &day, &hour, &min, &sec, &weekday);
  uint8_t short_year = year - 2000;

  char y[3];
  char m[3];
  char s[3];
  itoa(short_year, y, 10);

  // Special format for datetime for file name to limit to 8 chars:
  // 1st = year: last digit only, only unique within decade
  // 2nd = month: 1-9, A-C (10-12)
  // 3rd = day: 1-9, A-V (10-31)
  // 4th = hour: 1-9, A-N (10-23)
  // 5th & 6th = minute: normal 2-digit
  // 7th & 8th = second: normal 2-digit
  // Simple conversion:
  //    0 to 9 + 48 = ASCII '0' to '9'
  //    10 to 31 + 55 = ASCII 'A' to 'V'

  timestamp[0] = y[1]; // Only get the last digit of the year
  timestamp[1] = (month < 10) ? month + 48 : month + 55;
  timestamp[2] = (day < 10) ? day + 48 : day + 55;
  timestamp[3] = (hour < 10) ? hour + 48 : hour + 55;
  timestamp[4] = 0;

  itoa(min, m, 10);
  itoa(sec, s, 10);

  if (min < 10)
    strcat(timestamp, "0");
  strcat(timestamp, m);
  if (sec < 10)
    strcat(timestamp, "0");
  strcat(timestamp, s);
}

void CoreTimeStampText(char *timestamp) {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t weekday;
  get_rtc_values(&year, &month, &day, &hour, &min, &sec, &weekday);

  char y[5];
  char mon[3];
  char d[3];
  char h[3];
  char m[3];
  char s[3];

  itoa(year, y, 10);
  itoa(month, mon, 10);
  itoa(day, d, 10);
  itoa(hour, h, 10);
  itoa(min, m, 10);
  itoa(sec, s, 10);

  strcpy(timestamp, y);
  strcat(timestamp, "-");
  if (month < 10)
    strcat(timestamp, "0");
  strcat(timestamp, mon);
  strcat(timestamp, "-");
  if (day < 10)
    strcat(timestamp, "0");
  strcat(timestamp, d);
  strcat(timestamp, " ");
  if (hour < 10)
    strcat(timestamp, "0");
  strcat(timestamp, h);
  strcat(timestamp, ":");
  if (min < 10)
    strcat(timestamp, "0");
  strcat(timestamp, m);
  strcat(timestamp, ":");
  if (sec < 10)
    strcat(timestamp, "0");
  strcat(timestamp, s);
}
