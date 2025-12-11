# RTTTL Music Player for ESP32-C3

A lightweight, non-blocking RTTTL (Ring Tone Text Transfer Language) music player library for ESP32-C3 microcontrollers. Plays tunes in the background via FreeRTOS while your main code continues running.

## 🎵 Features

- **Non-blocking Playback**: Music plays in a FreeRTOS task, leaving your main loop free
- **Loop Support**: Play tunes once, a specific number of times, or loop forever (0=no loop, 1-254=loop count, 255=loop forever)
- **Volume Control**: Adjust volume on-the-fly
- **Multiple Tunes Included**: Over 20 popular tunes pre-loaded (Nokia, Mario, Star Wars, etc.)
- **Custom Tunes**: Play any RTTTL string from memory or serial input
- **Error Handling**: Graceful stop and status checking
- **Minimal Memory**: Uses FreeRTOS queue for thread-safe communication

## 🚀 Quick Start

### 1. Hardware Setup

Connect your audio amplifier module (like SC8002B) to ESP32-C3:

```
ESP32-C3        Amplifier Module
GPIO 3  -------> SIG (Signal)
5V      -------> VCC
GND     -------> GND
```

### 2. Installation

1. Copy these files to your PlatformIO project:
   - `RTTTLPlayer.h` - Main library header
   - `RTTTLPlayer.cpp` - Library implementation
   - `RTTTLTunes.h` - Pre-loaded music collection
   - `main.cpp` - Example player with menu

2. Update `platformio.ini`:
   ```ini
   [env:esp32-c3]
   platform = espressif32
   board = esp32-c3-devkitc-02
   framework = arduino
   monitor_speed = 115200
   ```

### 3. Basic Usage

```cpp
#include "RTTTLPlayer.h"
#include "RTTTLTunes.h"

RTTTLPlayer player(3, 180); // Pin 3, volume 180

void setup() {
    Serial.begin(115200);
    player.begin();
    
    // Play Nokia tune once
    player.play(RTTTLTunes::nokia);
    
    // Loop Star Wars theme forever
    player.play(RTTTLTunes::starWars, 255);
    
    // Loop Mario 3 times
    player.play(RTTTLTunes::mario, 3);
}

void loop() {
    // Your code runs while music plays in background!
    delay(10);
}
```

## 📋 API Reference

### Initialization
```cpp
RTTTLPlayer player(pin, volume); // Constructor
player.begin();                  // Initialize (call in setup())
player.setDebug(true/false);     // Enable/disable serial output
```

### Playback Control
```cpp
// Play a tune with optional loop count
bool play(const char* rtttl, uint8_t loopCount = 0);
// 0 = play once, 1-254 = loop X times, 255 = loop forever

void stop();                    // Stop playback immediately
bool isPlaying();               // Check if playing
bool isLooping();               // Check if looping
```

### Volume Control
```cpp
player.setVolume(volume);      // 0-255
uint8_t vol = player.getVolume();
```

## 🎮 Serial Menu System

The example `main.cpp` includes a complete serial menu system. Open Serial Monitor at 115200 baud:

```
=== RTTTL Player Menu ===
--- Music ---
 1. Nokia Ringtone
 2. Super Mario Bros
 3. James Bond Theme
 4. Pink Panther
 5. Imperial March (Star Wars)
 6. Addams Family
 7. Mission Impossible
 8. Indiana Jones
 9. Tetris Theme
 0. MASH Theme
 a. Happy Birthday
 b. Canon in D
 c. Für Elise
 d. Game of Thrones
 e. Harry Potter
 f. Zelda's Lullaby
 g. Mario Power-Up
 h. Star Wars Main Theme
 i. Test Scale
 j. Beethoven 5th Symphony
 k. Monty Python

--- Player Control ---
 s. Play Success Sound
 r. Play Error Sound
 v. Change Volume
 p. Check Player Status
 q. Stop Playback
 x. Enter Custom RTTTL
 m. Show this Menu
```

## 🎼 RTTTL Format

RTTTL format: `Name:d=4,o=5,b=120:c,d,e,f,g,a,b,c6`

- **Name**: Tune identifier
- **d**: Default duration (4=quarter, 8=eighth, etc.)
- **o**: Default octave (4-7)
- **b**: Beats per minute
- **Notes**: c,d,e,f,g,a,b (add # for sharp, add 6/7 for octave)

### Adding Custom Tunes

1. Add to `RTTTLTunes.h`:
   ```cpp
   const char myTune[] = "MySong:d=4,o=5,b=120:8c,8d,8e,8f,8g,8a,8b,8c6";
   ```

2. Play it:
   ```cpp
   player.play(RTTTLTunes::myTune);
   ```

3. Or enter via Serial Monitor:
   ```
   Select: x
   Enter RTTTL string: MySong:d=4,o=5,b=120:8c,8d,8e,8f
   ```

## 🔗 More RTTTL Tunes

Find thousands of RTTTL tunes online:

- [Cell Ringtones](http://www.cellringtones.com/) - Large collection of ringtones in RTTTL format
- [RTTTL.js Online Player](https://1j01.github.io/rtttl.js/) - Test tunes online, copy RTTTL strings

To convert MIDI or other formats to RTTTL, search for "RTTTL converter" online.

## ⚙️ Configuration Tips

### Stack Size
If you experience crashes with complex tunes, increase stack size in `RTTTLPlayer.cpp`:
```cpp
xTaskCreate(playerTask, "RTTTLPlayer", 8192, this, 1, &_playerTask);
// Change 4096 to 8192
```

### Volume Levels
- **0-80**: Very quiet (good for testing)
- **80-150**: Normal listening
- **150-220**: Loud (be careful with speakers)
- **220-255**: Very loud (may cause distortion)

### Hardware Notes
- **ESP32-C3**: Uses FreeRTOS, single-core, 160MHz
- **Amplifier**: SC8002B module works well at 5V
- **Speaker**: 8Ω 0.5W-2W recommended
- **Volume Control**: Adjust blue potentiometer on amplifier module

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| No sound | Check wiring (5V, GND, SIG), adjust volume pot |
| Distorted sound | Reduce volume in code `player.setVolume(150)` |
| Crashes with long tunes | Increase stack size to 8192 |
| Can't stop playback | Call `player.stop()` and wait a few ms |
| Serial menu not showing | Set baud rate to 115200 in Serial Monitor |

## 🎯 Example Projects

### 1. Game with Background Music
```cpp
void gameLoop() {
    player.play(RTTTLTunes::mario, 255); // Loop game music
    
    while(gameRunning) {
        // Game logic here
        if (playerHit) {
            player.play(RTTTLTunes::marioPowerUp); // Sound effect
        }
    }
    
    player.stop();
}
```

### 2. Alarm System
```cpp
void triggerAlarm() {
    // Play urgent alarm loop
    player.play("Alarm:d=4,o=7,b=200:8c7,8c7", 255);
    
    // Flash LEDs while alarm sounds
    while(alarmActive) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        delay(500);
    }
    
    player.stop();
}
```

### 3. Interactive Toy
```cpp
void buttonPressed() {
    // Play random tune from list
    const char* tunes[] = {
        RTTTLTunes::marioPowerUp,
        RTTTLTunes::success,
        RTTTLTunes::zelda
    };
    
    int randomTune = random(0, 3);
    player.play(tunes[randomTune]);
}
```

## 📁 File Structure

```
project/
├── src/
│   ├── RTTTLPlayer.h     # Player class interface
│   ├── RTTTLPlayer.cpp   # Player implementation
│   ├── RTTTLTunes.h      # Pre-loaded music collection
│   └── main.cpp          # Example with serial menu
├── platformio.ini        # PlatformIO configuration
└── README.md             # This file
```

## 📄 License

MIT License

Copyright (c) 2024 Econode NZ, Author Matt Way

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## 🙏 Acknowledgments

- RTTTL format originally from Nokia
- FreeRTOS for task management
- ESP32 Arduino core developers
- All musicians whose works are represented in the example tunes

---

*Happy music making! For questions or improvements, please submit an issue or pull request.*