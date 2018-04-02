//Gunnar Pope
//haptic_driver.h

/*
 * This haptic buzzer uses TimerA3 to produce a 10ms "buzz" from
 * the haptic buzzer.
 *
 * The buzzer has 2 dependencies, the output pin must be configured
 * in the #define below and the TimerA3 ISR must be defined in the
 * bsp_setup.c file.
 *
 * To use the haptic buzzer, just call the functions
 * void singleBuzz();
 * void doubleBuzz();
 */

#ifndef haptic_driver_h
#define haptic_driver_h

/*
 * Haptic related pin definitions (see: bsp_devboard_init.h &
 * bsp_bracelet_init.h).
 */

void haptic_init();
void haptic();
void timerA3_disable();

void singleBuzz();
void doubleBuzz();

#endif
