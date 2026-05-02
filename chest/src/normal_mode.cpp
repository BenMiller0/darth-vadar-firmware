#include "normal_mode.hpp"
#include "blink_helpers.hpp"
#include "constants.hpp"
#include <Arduino.h>

// =============================================================================
// NORMAL MODE - DARTH VADER CHEST BEHAVIOR
// =============================================================================

// -----------------------------------------------------------------------------
// NORMAL MODE LED BEHAVIORS
// -----------------------------------------------------------------------------

// Handle chest red LED behavior in normal mode
// Pattern: OFF by default, randomly blink ON for 15s +/- 10s (5-25s range), then OFF for 1s
// Used for: CHEST_RED_1, CHEST_RED_2, CHEST_RED_3
void handleChestRedLED(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    // Keep LED off initially
    ledcWrite(channel, 0);
    
    // Calculate off time with random variation (15s +/- 10s = 5-25s range)
    int offTime = CHEST_RED_BASE_OFF_TIME + random(-CHEST_RED_RANDOM_RANGE, CHEST_RED_RANDOM_RANGE);
    offTime = max(5000, offTime);  // Minimum 5 seconds
    vTaskDelay(offTime / portTICK_PERIOD_MS);
    
    // Turn LED on at configured brightness
    ledcWrite(channel, params->brightness);
    
    // Keep on for 1 second
    vTaskDelay(CHEST_RED_ON_TIME / portTICK_PERIOD_MS);
    
    // Turn LED off
    ledcWrite(channel, 0);
}

// Check if a given pin should use normal mode rare blinking behavior
bool isNormalModeChestLED(int pin) {
    return (pin == CHEST_RED_1 || pin == CHEST_RED_2 || pin == CHEST_RED_3);
}
