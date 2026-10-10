#include <Arduino.h>
#include <DFRobot_DF1201S.h>

#include "audio_config.h"

HardwareSerial dfSerial(1);
DFRobot_DF1201S dfPlayer;

void setup() {
  Serial.begin(115200);
  dfSerial.begin(AudioConfig::DFPLAYER_BAUD, SERIAL_8N1,
                 AudioConfig::DFPLAYER_RX_PIN,
                 AudioConfig::DFPLAYER_TX_PIN);

  while (!dfPlayer.begin(dfSerial)) {
    Serial.println("DFPlayer not found. Retrying...");
    delay(1000);
  }

  dfPlayer.switchFunction(dfPlayer.MUSIC);
  dfPlayer.setVol(AudioConfig::VOLUME);

  // Play the startup voice once if track 2 exists.
  if (dfPlayer.getTotalFile() >= AudioConfig::STARTUP_VOICE_TRACK) {
    dfPlayer.setPlayMode(dfPlayer.SINGLE);
    dfPlayer.playFileNum(AudioConfig::STARTUP_VOICE_TRACK);
    while (dfPlayer.isPlaying()) {
      delay(50);
    }
  }

  // Loop breathing forever.
  dfPlayer.setPlayMode(dfPlayer.SINGLECYCLE);
  dfPlayer.playFileNum(AudioConfig::BREATHING_TRACK);
}

void loop() {}
