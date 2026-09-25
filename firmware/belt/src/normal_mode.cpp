#include "normal_mode.hpp"
#include "blink_helpers.hpp"
#include "constants.hpp"
#include <Arduino.h>

// Production belt behavior: both red belt LEDs stay on for roughly 10 seconds
// with slight random variation, then turn off for 1 second.
void handleNormalModeLed(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    ledcWrite(channel, params->brightness);
    
    // random(min, max) excludes max on Arduino, which is fine for this loose timing.
    int onTime = RED_LED_BASE_ON_TIME + random(-RED_LED_RANDOM_RANGE, RED_LED_RANDOM_RANGE);
    vTaskDelay(onTime / portTICK_PERIOD_MS);
    
    ledcWrite(channel, 0);
    vTaskDelay(RED_LED_OFF_TIME / portTICK_PERIOD_MS);
}
