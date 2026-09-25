#ifndef NORMAL_MODE_HPP
#define NORMAL_MODE_HPP

#include "led_blink_task.hpp"

// Belt red LEDs stay on for about 10 seconds, then blink off for 1 second.
void handleNormalModeLed(LedTaskParams* params);

#endif // NORMAL_MODE_HPP
