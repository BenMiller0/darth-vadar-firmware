#ifndef NORMAL_MODE_HPP
#define NORMAL_MODE_HPP

#include "led_blink_task.hpp"

// Chest red LEDs stay off most of the time, then blink on briefly.
void handleNormalModeLed(LedTaskParams* params);
void chestNormalModeLedTask(void* pvParameters);

#endif // NORMAL_MODE_HPP
