/**
 * ESP32-RTTL_Player Example functional tests
 */
#include <Arduino.h>
#include <RTTTLPlayer.h>
#include <RTTTLTunes.h>

// Pin assignment for audio output
#define PIN_AUDIO_OUT 3
// Volume  0 - 255
#define AUDIO_VOLUME 180

RTTTLPlayer player(PIN_AUDIO_OUT, AUDIO_VOLUME);

void setup() {
    Serial.begin(115200);
    player.begin();
    player.play(RTTTLTunes::startup);
    delay(2000);
    
    Serial.println("\n🎵 RTTTL Player Quick Test");
    Serial.println("=========================");
}

void loop() {
    Serial.println("* Testing single playback (Nokia)...");
    player.play(RTTTLTunes::nokia);
    while( player.isPlaying() ) delay (100 );

    Serial.println("* Testing loop function startup 3 times...");
    player.play(RTTTLTunes::marioPowerUp, 3);
    while( player.isPlaying() ) delay (100 );
}