#pragma once

#include <Arduino.h>

namespace AudioConfig {

constexpr uint8_t DFPLAYER_RX_PIN = 4;
constexpr uint8_t DFPLAYER_TX_PIN = 5;
constexpr uint32_t DFPLAYER_BAUD = 115200;

constexpr int16_t BREATHING_TRACK = 1;
constexpr int16_t STARTUP_VOICE_TRACK = 2;
constexpr uint8_t VOLUME = 20;

}  // namespace AudioConfig
