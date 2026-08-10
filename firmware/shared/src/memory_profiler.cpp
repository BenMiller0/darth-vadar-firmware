#include "memory_profiler.hpp"
#include <Arduino.h>

// =============================================================================
// MEMORY PROFILER - TASK MONITORING AND SAFETY
// =============================================================================

// -----------------------------------------------------------------------------
// GLOBAL PROFILING STATE
// -----------------------------------------------------------------------------
TaskMemoryStats taskStats[NUM_LEDS];
HeapStats globalHeapStats;
bool profilingEnabled = true;

// -----------------------------------------------------------------------------
// MEMORY PROFILER IMPLEMENTATION
// -----------------------------------------------------------------------------
void initMemoryProfiler() {
    for (int i = 0; i < NUM_LEDS; i++) {
        taskStats[i].taskHandle = NULL;
        taskStats[i].taskName[0] = '\0';
        taskStats[i].stackHighWaterMark = 0;
        taskStats[i].stackSize = 0;
        taskStats[i].stackUsagePercent = 0;
        taskStats[i].isHealthy = true;
        taskStats[i].lastCheckTime = 0;
    }
    
    globalHeapStats.totalHeap = ESP.getHeapSize();
    globalHeapStats.freeHeap = ESP.getFreeHeap();
    globalHeapStats.minFreeHeap = globalHeapStats.freeHeap;
    globalHeapStats.heapUsagePercent = ((globalHeapStats.totalHeap - globalHeapStats.freeHeap) * 100) / globalHeapStats.totalHeap;
    globalHeapStats.largestFreeBlock = ESP.getMaxAllocHeap();
    globalHeapStats.fragmentationPercent = 0;
    globalHeapStats.isHealthy = true;
}

void registerTaskForProfiling(TaskHandle_t taskHandle, const char* taskName, uint32_t stackSize) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle == NULL) {
            taskStats[i].taskHandle = taskHandle;
            strncpy(taskStats[i].taskName, taskName, 15);
            taskStats[i].taskName[15] = '\0';
            taskStats[i].stackSize = stackSize;
            taskStats[i].stackHighWaterMark = stackSize;
            taskStats[i].stackUsagePercent = 0;
            taskStats[i].isHealthy = true;
            taskStats[i].lastCheckTime = millis();
            break;
        }
    }
}

void updateMemoryStats() {
    if (!profilingEnabled) return;
    
    // Update task stats
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle != NULL) {
            taskStats[i].stackHighWaterMark = uxTaskGetStackHighWaterMark(taskStats[i].taskHandle);
            taskStats[i].stackUsagePercent = ((taskStats[i].stackSize - taskStats[i].stackHighWaterMark) * 100) / taskStats[i].stackSize;
            taskStats[i].isHealthy = taskStats[i].stackUsagePercent < STACK_CRITICAL_THRESHOLD;
            taskStats[i].lastCheckTime = millis();
        }
    }
    
    // Update heap stats
    globalHeapStats.freeHeap = ESP.getFreeHeap();
    if (globalHeapStats.freeHeap < globalHeapStats.minFreeHeap) {
        globalHeapStats.minFreeHeap = globalHeapStats.freeHeap;
    }
    globalHeapStats.heapUsagePercent = ((globalHeapStats.totalHeap - globalHeapStats.freeHeap) * 100) / globalHeapStats.totalHeap;
    globalHeapStats.largestFreeBlock = ESP.getMaxAllocHeap();
    globalHeapStats.fragmentationPercent = getHeapFragmentation();
    globalHeapStats.isHealthy = globalHeapStats.heapUsagePercent < HEAP_CRITICAL_THRESHOLD;
}

bool checkTaskStackHealth(TaskHandle_t taskHandle) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle == taskHandle) {
            return taskStats[i].isHealthy;
        }
    }
    return true;
}

bool checkHeapHealth() {
    return globalHeapStats.isHealthy;
}

void printMemoryStats() {
#if ENABLE_SERIAL_OUTPUT
    Serial.println("=== Memory Statistics ===");
    Serial.printf("Heap: %u / %u bytes (%u%%)\n", globalHeapStats.freeHeap, globalHeapStats.totalHeap, globalHeapStats.heapUsagePercent);
    Serial.printf("Min Free Heap: %u bytes\n", globalHeapStats.minFreeHeap);
    Serial.printf("Largest Free Block: %u bytes\n", globalHeapStats.largestFreeBlock);
    Serial.printf("Fragmentation: %u%%\n", globalHeapStats.fragmentationPercent);
    
    Serial.println("\nTask Stack Usage:");
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle != NULL) {
            Serial.printf("%s: %u%% (%u / %u bytes)\n", 
                taskStats[i].taskName, 
                taskStats[i].stackUsagePercent,
                taskStats[i].stackSize - taskStats[i].stackHighWaterMark,
                taskStats[i].stackSize);
        }
    }
#endif
}

void printMemoryWarnings() {
#if ENABLE_SERIAL_OUTPUT
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle != NULL) {
            if (taskStats[i].stackUsagePercent >= STACK_CRITICAL_THRESHOLD) {
                Serial.printf("CRITICAL: %s stack usage at %u%%\n", taskStats[i].taskName, taskStats[i].stackUsagePercent);
            } else if (taskStats[i].stackUsagePercent >= STACK_WARNING_THRESHOLD) {
                Serial.printf("WARNING: %s stack usage at %u%%\n", taskStats[i].taskName, taskStats[i].stackUsagePercent);
            }
        }
    }
    
    if (globalHeapStats.heapUsagePercent >= HEAP_CRITICAL_THRESHOLD) {
        Serial.printf("CRITICAL: Heap usage at %u%%\n", globalHeapStats.heapUsagePercent);
    } else if (globalHeapStats.heapUsagePercent >= HEAP_WARNING_THRESHOLD) {
        Serial.printf("WARNING: Heap usage at %u%%\n", globalHeapStats.heapUsagePercent);
    }
#endif
}

uint8_t getHeapFragmentation() {
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t largestBlock = ESP.getMaxAllocHeap();
    if (freeHeap == 0) return 0;
    return ((freeHeap - largestBlock) * 100) / freeHeap;
}

uint8_t getTaskStackUsage(TaskHandle_t taskHandle) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle == taskHandle) {
            return taskStats[i].stackUsagePercent;
        }
    }
    return 0;
}

void performEmergencyMemoryRecovery() {
    // Emergency memory recovery - currently just a placeholder
    // In a real implementation, this might restart tasks, clear caches, etc.
}

void memoryProfilerTask(void* pvParameters) {
    while (true) {
        updateMemoryStats();
        printMemoryWarnings();
        vTaskDelay(PROFILING_INTERVAL_MS / portTICK_PERIOD_MS);
    }
}

bool isSafeToCreateTask(uint32_t requiredStackSize) {
    uint32_t freeHeap = ESP.getFreeHeap();
    return (freeHeap > requiredStackSize * 2); // Safety margin
}

void logMemoryEvent(const char* event, const char* details) {
#if ENABLE_SERIAL_OUTPUT
    Serial.printf("[MEM] %s: %s\n", event, details);
#endif
}
