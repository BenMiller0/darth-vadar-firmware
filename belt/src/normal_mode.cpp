#include "normal_mode.hpp"
#include "blink_helpers.hpp"
#include "constants.hpp"
#include <Arduino.h>

// =============================================================================
// NORMAL MODE - DARTH VADER BELT BEHAVIOR
// =============================================================================
// Normal mode implements the canonical Darth Vader belt LED behavior:
// - Red belt LEDs: Blink on for ~10 seconds, off for 1 second (with random variation)
// - Green belt LEDs: Solid on (no blinking)

// Handle red belt LED behavior in normal mode
// Pattern: ON for ~10 seconds (with +/- 3s random variation), OFF for 1 second
// Used for: L_BELT_RED, R_BELT_RED
void handleNormalModeRedLED(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    // Turn LED on at configured brightness
    ledcWrite(channel, params->brightness);
    
    // Calculate on time with random variation (10s +/- 3s)
    int onTime = RED_LED_BASE_ON_TIME + random(-RED_LED_RANDOM_RANGE, RED_LED_RANDOM_RANGE);
    vTaskDelay(onTime / portTICK_PERIOD_MS);
    
    // Turn LED off
    ledcWrite(channel, 0);
    
    // Keep off for 1 second
    vTaskDelay(RED_LED_OFF_TIME / portTICK_PERIOD_MS);
}

// Check if a given pin should use normal mode red LED behavior
bool isNormalModeRedLED(int pin) {
    return (pin == L_BELT_RED || pin == R_BELT_RED);
}