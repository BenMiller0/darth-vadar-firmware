#ifndef NORMAL_MODE_HPP
#define NORMAL_MODE_HPP

#include "led_blink_task.hpp"

// Each belt red LED stays on for 10-15 seconds, then blinks off for 1 second.
void handleNormalModeLed(LedTaskParams* params);
void normalModeLedTask(void* pvParameters);

#endif // NORMAL_MODE_HPP
