#include <Arduino.h>
#include "constants.hpp"


// This is the struct that will be passed in as the params to each thread created for each LED
struct LedTaskParams {
    int pin;
    int delay;
};


// Parameter array. Includes pin # and blink delay
static LedTaskParams ledParams[6] = {
    {  L_BELT_RED,        L_BELT_REDL_DELAY      },
    {  L_BELT_GREEN_0,    L_BELT_GREEN_0_DELAY   },
    {  L_BELT_GREEN_1,    L_BELT_GREEN_1_DELAY   },
    {  R_BELT_RED,        R_BELT_RED_DELAY       },
    {  R_BELT_GREEN_0,    R_BELT_GREEN_0_DELAY   }, 
    {  R_BELT_GREEN_1,    R_BELT_GREEN_1_DELAY   },  
};


// Definition of thread that is created for each LED
void ledBlinkTask(void *pvParameters) {
    LedTaskParams* params = (LedTaskParams*)pvParameters;
    while (true) {
        digitalWrite(params->pin, HIGH);
        vTaskDelay(params->delay / portTICK_PERIOD_MS);
        digitalWrite(params->pin, LOW);
        vTaskDelay(params->delay / portTICK_PERIOD_MS);
    }
}


void setup() {
    // Set each LED GPIO pun as output pin
    for (LedTaskParams entry : ledParams) {
        pinMode(entry.pin, OUTPUT);
    }

    // Create separate thread for each LED
    for (int i = 0; i < 6; i++) {
        char taskName[20];
        sprintf(taskName, "LED%d", i);
        xTaskCreate(
            ledBlinkTask, 
            taskName,
            1000,
            &ledParams[i], 
            1,
            NULL
        );
    }
}


void loop() {} // Empty, FreeRTOS threads take over