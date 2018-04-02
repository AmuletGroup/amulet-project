// Gunnar Pope
// timer_A2.h

#define DELAY20ms 0x9C40  // 20ms delay for debounce
#define DELAY10ms 0x4E20
#define DELAY5ms 0x2710
#define MAXCOUNT 0XF000

// global functions of the timer
void timerA2_init();
void timerA2_disable();
void timerA2_addCount();
void timerA2_resetCount();
uint16_t timerA2_getCount();
