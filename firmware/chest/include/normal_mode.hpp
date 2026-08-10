#ifndef NORMAL_MODE_HPP
#define NORMAL_MODE_HPP

#include "led_blink_task.hpp"

// =============================================================================
// NORMAL MODE - DARTH VADER CHEST BEHAVIOR
// =============================================================================
// Normal mode implements the canonical Darth Vader chest box LED behavior:
// - Chest red LEDs: Rare blinking (solid most of the time, occasional brief blink)

// -----------------------------------------------------------------------------
// NORMAL MODE LED BEHAVIORS
// -----------------------------------------------------------------------------

// Handle chest red LED behavior in normal mode
// Pattern: OFF by default, randomly blink ON for 15s +/- 10s (5-25s range), then OFF for 1s
// Used for: CHEST_RED_1, CHEST_RED_2, CHEST_RED_3
void handleChestRedLED(LedTaskParams* params);

// Check if a given pin should use normal mode rare blinking behavior
bool isNormalModeChestLED(int pin);

#endif // NORMAL_MODE_HPP
