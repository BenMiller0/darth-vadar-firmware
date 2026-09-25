#include <Arduino.h>
#include "constants.hpp"
#include "led_blink_task.hpp"
#include "blink_helpers.hpp"
#include "normal_mode.hpp"
#include "memory_profiler.hpp"

#if TEST_MODE
#include "test_mode.hpp"
#endif

// Chest firmware controls three red LEDs. Normal mode ignores the generic blink
// flags, but keeping the same LedTaskParams shape lets shared test code compile.
static LedTaskParams ledParams[NUM_LEDS] = {
    {CHEST_RED_1, CHEST_RED_1_DELAY, VOLATILE_BLINKING, SMOOTH_BLINKING, CHEST_RED_1_VOLATILITY, CHEST_RED_1_BRIGHTNESS},
    {CHEST_RED_2, CHEST_RED_2_DELAY, VOLATILE_BLINKING, SMOOTH_BLINKING, CHEST_RED_2_VOLATILITY, CHEST_RED_2_BRIGHTNESS},
    {CHEST_RED_3, CHEST_RED_3_DELAY, VOLATILE_BLINKING, SMOOTH_BLINKING, CHEST_RED_3_VOLATILITY, CHEST_RED_3_BRIGHTNESS}
};

void setup() {
#if ENABLE_SERIAL_OUTPUT
    Serial.begin(115200);
    delay(1000);
    Serial.println("Chest LED Blink Controller Starting...");
#endif
    
#if ENABLE_MEMORY_PROFILING
    initMemoryProfiler();
    
    xTaskCreate(
        memoryProfilerTask, 
        "Memory Profiler",
        4096,
        NULL, 
        1,
        NULL
    );
#endif

#if CPU_FREQUENCY_MHZ != 240
    setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
#endif
    
#if TEST_MODE
#if ENABLE_SERIAL_OUTPUT
    Serial.println("TEST MODE ENABLED - Entering test mode");
#endif
    runTestMode();
#else
#if ENABLE_SERIAL_OUTPUT
    Serial.println("NORMAL MODE - Initializing chest LED tasks");
#endif
    
    // PWM is used for normal on/off blinking too, because brightness is set by duty cycle.
    initializePwmPins(ledParams, NUM_LEDS);

    TaskHandle_t ledTaskHandles[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        char taskName[20];
        sprintf(taskName, "ChestLED%d", i);
        xTaskCreate(
            ledBlinkTask, 
            taskName,
            1000,
            &ledParams[i], 
            1,
            &ledTaskHandles[i]
        );
        
#if ENABLE_MEMORY_PROFILING
        registerTaskForProfiling(ledTaskHandles[i], taskName, 1000);
#endif
    }
#endif
}

void loop() {
#if TEST_MODE
    delay(1000);
#else
    delay(1000);
#endif
}
