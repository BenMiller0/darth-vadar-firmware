#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// =============================================================================
// CHEST LED CONTROLLER - CONSTANTS
// =============================================================================

// -----------------------------------------------------------------------------
// GPIO PIN DEFINITIONS
// -----------------------------------------------------------------------------
// LED Layout: 3 Chest Red LEDs only

// Chest LEDs
#define CHEST_RED_1             4
#define CHEST_RED_2             22
#define CHEST_RED_3             23

// -----------------------------------------------------------------------------
// BLINK TIMING CONFIGURATION
// -----------------------------------------------------------------------------
#define CHEST_RED_1_DELAY       1000
#define CHEST_RED_2_DELAY       1000
#define CHEST_RED_3_DELAY       1000

// -----------------------------------------------------------------------------
// NORMAL MODE CHEST RED LED CONFIGURATION
// -----------------------------------------------------------------------------
#define CHEST_RED_BASE_OFF_TIME 15000  // Base off time for chest red LEDs (15 seconds)
#define CHEST_RED_ON_TIME       1000   // On time for chest red LEDs (1 second)
#define CHEST_RED_RANDOM_RANGE  10000  // Random variation range (+/- 10 seconds)

// -----------------------------------------------------------------------------
// OPERATIONAL MODES
// -----------------------------------------------------------------------------
#define TEST_MODE               0
#define NORMAL_MODE             1
#define VOLATILE_BLINKING       0
#define SMOOTH_BLINKING         0

// -----------------------------------------------------------------------------
// SYSTEM CONFIGURATION
// -----------------------------------------------------------------------------
#define NUM_LEDS               3     // Total number of LEDs in the system

// -----------------------------------------------------------------------------
// PWM SMOOTH BLINKING CONFIGURATION
// -----------------------------------------------------------------------------
#define FADE_STEPS              50    // Number of steps for fade in/out
#define FADE_DELAY              10    // Delay between fade steps (ms)
#define PWM_FREQUENCY           5000  // PWM frequency in Hz
#define PWM_RESOLUTION          8     // PWM resolution (8 bits = 0-255)

// -----------------------------------------------------------------------------
// VOLATILITY MULTIPLIERS
// -----------------------------------------------------------------------------
// Higher values = more volatile/random blinking
#define CHEST_RED_1_VOLATILITY         0.05
#define CHEST_RED_2_VOLATILITY         0.05
#define CHEST_RED_3_VOLATILITY         0.05

// -----------------------------------------------------------------------------
// LED BRIGHTNESS CONFIGURATION
// -----------------------------------------------------------------------------
// Brightness values for each LED (0-255 PWM range)
// 255 = maximum brightness, 0 = off
#define CHEST_RED_1_BRIGHTNESS         255
#define CHEST_RED_2_BRIGHTNESS         255
#define CHEST_RED_3_BRIGHTNESS         255

// -----------------------------------------------------------------------------
// POWER MANAGEMENT CONFIGURATION (BATTERY MODE)
// -----------------------------------------------------------------------------
#define ENABLE_SERIAL_OUTPUT           0     // Enable Serial for debugging
#define ENABLE_MEMORY_PROFILING        0     // Disable memory profiler in normal mode
#define DISABLE_WIFI                   1     // Disable WiFi for power savings
#define CPU_FREQUENCY_MHZ              80    // Lower CPU frequency (80MHz instead of 240MHz)
#define ENABLE_LIGHT_SLEEP             1     // Enable light sleep during LED off periods

#endif // CONSTANTS_HPP
