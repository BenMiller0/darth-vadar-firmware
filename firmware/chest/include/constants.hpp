#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Chest LEDs
// ESP32-C3 SuperMini GPIO assignments. GPIO 5-7 are exposed on the board and
// avoid the C3 strapping pins, USB pins, and UART pins.
#define CHEST_RED_1                     5
#define CHEST_RED_2                     6
#define CHEST_RED_3                     7

// Chest red LEDs stay dark most of the time, then flash on briefly.
#define CHEST_RED_MIN_OFF_TIME          5000
#define CHEST_RED_MAX_OFF_TIME          20000
#define CHEST_RED_ON_TIME               1000

#define NUM_LEDS                        3
#define LED_UPDATE_INTERVAL_MS          20

// PWM is used so LED brightness can be controlled by duty cycle.
#define PWM_FREQUENCY                   5000  // PWM frequency in Hz
#define PWM_RESOLUTION                  8     // PWM resolution (8 bits = 0-255)

#define CHEST_RED_1_BRIGHTNESS          255
#define CHEST_RED_2_BRIGHTNESS          255
#define CHEST_RED_3_BRIGHTNESS          255

#define ENABLE_SERIAL_OUTPUT            0     // Enable Serial for debugging
#define CPU_FREQUENCY_MHZ               80    // Lower CPU frequency (80MHz instead of 240MHz)

#endif // CONSTANTS_HPP
