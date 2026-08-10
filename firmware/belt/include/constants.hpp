#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// =============================================================================
// LED CONTROLLER - CONSTANTS
// =============================================================================

// -----------------------------------------------------------------------------
// GPIO PIN DEFINITIONS
// -----------------------------------------------------------------------------
// LED Layout: 2 software-controlled red LEDs.
// Green belt LEDs are powered directly from 3.3V and are not controlled here.

// Belt red LEDs
#define L_BELT_RED              5
#define R_BELT_RED              6

// Touch Sensor Pins
#define TOUCH_BRIGHTNESS_PIN    13    // D13 / GPIO 13 for brightness control

// -----------------------------------------------------------------------------
// BLINK TIMING CONFIGURATION
// -----------------------------------------------------------------------------
#define L_BELT_RED_DELAY        1000
#define R_BELT_RED_DELAY        1000

// -----------------------------------------------------------------------------
// NORMAL MODE RED LED CONFIGURATION
// -----------------------------------------------------------------------------
#define RED_LED_BASE_ON_TIME    10000  // Base on time for red LEDs (10 seconds)
#define RED_LED_OFF_TIME        1000   // Off time for red LEDs (1 second)
#define RED_LED_RANDOM_RANGE    3000   // Random variation range (+/- 3 seconds)

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
#define NUM_LEDS               2     // Total number of software-controlled LEDs

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
#define L_BELT_RED_VOLATILITY          0.1
#define R_BELT_RED_VOLATILITY          0.9

// -----------------------------------------------------------------------------
// LED BRIGHTNESS CONFIGURATION
// -----------------------------------------------------------------------------
// Brightness values for each LED (0-255 PWM range)
// 255 = maximum brightness, 0 = off
#define L_BELT_RED_BRIGHTNESS          100
#define R_BELT_RED_BRIGHTNESS          100

// -----------------------------------------------------------------------------
// POWER MANAGEMENT CONFIGURATION (BATTERY MODE)
// -----------------------------------------------------------------------------
#define ENABLE_SERIAL_OUTPUT           0     // Enable Serial for brightness feedback
#define ENABLE_MEMORY_PROFILING        0     // Disable memory profiler in normal mode
#define DISABLE_WIFI                   1     // Disable WiFi for power savings
#define CPU_FREQUENCY_MHZ              80    // Lower CPU frequency (80MHz instead of 240MHz)
#define ENABLE_LIGHT_SLEEP             1     // Enable light sleep during LED off periods

#endif // CONSTANTS_HPP
