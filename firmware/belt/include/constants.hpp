#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Green belt LEDs are powered directly from 3.3V and are not controlled here.
#define L_BELT_RED              3
#define R_BELT_RED              4

// Belt red LEDs stay on most of the time, then blink off briefly.
#define RED_LED_MIN_ON_TIME     10000
#define RED_LED_MAX_ON_TIME     15000
#define RED_LED_OFF_TIME        1000

#define NUM_LEDS                2
#define LED_UPDATE_INTERVAL_MS  20

// PWM is used so the fixed LED output level can be set by duty cycle.
#define PWM_FREQUENCY           5000
#define PWM_RESOLUTION          8

#define L_BELT_RED_BRIGHTNESS   255
#define R_BELT_RED_BRIGHTNESS   255

#define ENABLE_SERIAL_OUTPUT    0
#define CPU_FREQUENCY_MHZ       80

#endif // CONSTANTS_HPP
