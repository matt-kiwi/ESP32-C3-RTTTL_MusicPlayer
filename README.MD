# RTTTL Player for ESP32-C3

[![PlatformIO Registry](https://badges.platformio.org/packages/econode-nz/library/RTTTLPlayer.svg)](https://registry.platformio.org/libraries/econode-nz/RTTTLPlayer)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/econode-nz/RTTTLPlayer)

A lightweight, non-blocking RTTTL music player library for ESP32-C3 microcontrollers. Plays tunes in the background via FreeRTOS while your main code continues running—perfect for games, alarms, notifications, or background music.

**Features:**
- 🎵 **Non-blocking playback** via FreeRTOS task
- 🔄 **Loop control** (once, X times, or forever)
- 🔊 **Real-time volume control**
- 📦 **20+ popular tunes included** (Nokia, Mario, Star Wars, etc.)
- 🎹 **Custom RTTTL support** via string or serial input
- 🔧 **ESP32-C3 optimized** with minimal memory footprint
- 📡 **Serial menu system** for easy testing

## 📦 Installation

### PlatformIO (Recommended)
Add to your `platformio.ini`:
```ini
[env:esp32-c3]
platform = espressif32
board = esp32-c3-devkitc-02
framework = arduino
lib_deps = 
    econode-nz/RTTTLPlayer
```

### Arduino IDE
1. **Sketch → Include Library → Manage Libraries**
2. Search for **"RTTTLPlayer"**
3. Click **Install**

### Manual Installation
1. Download the [latest release](https://github.com/econode-nz/RTTTLPlayer/releases)
2. Extract to your `Arduino/libraries/` folder
3. Restart Arduino IDE

## 🚀 Quick Start

```cpp
#include <RTTTLPlayer.h>
#include <RTTTLTunes.h>

RTTTLPlayer player(3, 180);  // GPIO pin 3, volume 180

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

## 🔌 Hardware Setup

```
ESP32-C3        Amplifier Module       Speaker
GPIO 3  -------> SIG (Signal)          
5V      -------> VCC           ------> + (Positive)
GND     -------> GND           ------> - (Negative)
```

**Tested with:**
- **Amplifier**: SC8002B module (Jaycar XC3744 or similar)
- **Speaker**: 8Ω 0.5W-2W
- **ESP32-C3**: ESP32-C3-DevKitC-02 or similar

*Adjust the blue potentiometer on the amplifier module for hardware volume control.*

## 📖 API Reference

### Basic Methods
```cpp
// Initialize (call in setup())
void begin();

// Play RTTTL string with loop control
// loopCount: 0=once, 1-254=X times, 255=forever
bool play(const char* rtttl, uint8_t loopCount = 0);

// Stop playback immediately  
void stop();

// Volume control (0-255)
void setVolume(uint8_t volume);
uint8_t getVolume();

// Status checking
bool isPlaying();
bool isLooping();
uint8_t getRemainingLoops();  // 0=not looping, 1-254=loops left, 255=forever
```

### Included Tunes (in `RTTTLTunes.h`)

| Tune | Constant | Description |
|------|----------|-------------|
| Nokia Ringtone | `RTTTLTunes::nokia` | Classic Nokia tune |
| Super Mario Bros | `RTTTLTunes::mario` | Main theme |
| Star Wars | `RTTTLTunes::starWars` | Main theme |
| Imperial March | `RTTTLTunes::imperial` | Darth Vader's theme |
| Tetris | `RTTTLTunes::tetris` | Korobeiniki theme |
| Happy Birthday | `RTTTLTunes::happyBirthday` | Birthday song |
| James Bond | `RTTTLTunes::bond` | 007 theme |
| Pink Panther | `RTTTLTunes::pinkPanther` | Henry Mancini theme |
| And 15+ more... | | See RTTTLTunes.h |

### Sound Effects
```cpp
player.play(RTTTLTunes::success);  // Success sound
player.play(RTTTLTunes::error);    // Error sound  
player.play(RTTTLTunes::alert);    // Alert/notification
player.play(RTTTLTunes::startup);  // Startup beep
```

## 🎮 Examples

### 1. Menu Player (Complete Example)
Located in `examples/MenuPlayer/`. Open Serial Monitor at 115200 baud for interactive menu:

```
=== RTTTL Player Menu ===
1. Nokia Ringtone
2. Super Mario Bros
3. James Bond Theme
...
Select:
```

### 2. Game with Background Music
```cpp
void gameLoop() {
    // Start looping background music
    player.play(RTTTLTunes::gameOfThrones, 255);
    
    while(gameRunning) {
        // Game logic here
        if (playerScored) {
            player.play(RTTTLTunes::success); // Sound effect
        }
    }
    
    player.stop(); // Stop music when game ends
}
```

### 3. Alarm System
```cpp
void triggerAlarm() {
    // Play urgent alarm (loop forever until stopped)
    player.play("Alarm:d=4,o=7,b=200:8c7,8c7", 255);
    
    // Flash LED while alarm sounds
    while(alarmActive) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        delay(500);
    }
    
    player.stop();
}
```

## 🎵 RTTTL Format

RTTTL (Ring Tone Text Transfer Language) format:
```
Name:d=default_duration,o=default_octave,b=beats_per_minute:notes
```

### Example
```
Nokia:d=4,o=5,b=160:8e6,8d6,f#,8e6,8d6,f#
```

- **8e6**: Eighth note, E, octave 6
- **f#**: F sharp (default duration and octave)
- **p**: Pause/rest

### Adding Custom Tunes

1. **Add to `RTTTLTunes.h`:**
   ```cpp
   const char myTune[] = "MySong:d=4,o=5,b=120:8c,8d,8e,8f,8g,8a,8b,8c6";
   ```

2. **Play it:**
   ```cpp
   player.play(RTTTLTunes::myTune);
   ```

3. **Or enter via Serial Monitor** (in MenuPlayer example):
   ```
   Select: x
   Enter RTTTL: MySong:d=4,o=5,b=120:8c,8d,8e,8f 255  # Loop forever
   ```

## 🔗 More RTTTL Tunes Online

Find thousands of RTTTL tunes at:
- **[Cell Ringtones](http://www.cellringtones.com/)** - Large collection of ringtones
- **[RTTTL.js Online Player](https://1j01.github.io/rtttl.js/)** - Test and convert tunes
- **Search for** "RTTTL ringtones" or "RTTTL converter"

*Tip: Test tunes online first, then copy the RTTTL string into your code.*

## ⚙️ Configuration

### Debug Output
```cpp
player.setDebug(true);  // Enable serial debug messages
```

### FreeRTOS Task Settings
The player uses:
- **Stack size**: 4096 bytes (increase if complex tunes crash)
- **Priority**: 1 (above idle task)
- **Queue size**: 5 commands

### Volume Guidelines
- **0-80**: Very quiet (testing)
- **80-150**: Normal listening
- **150-220**: Loud
- **220-255**: Very loud (may distort)

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| **No sound** | Check wiring (5V, GND, SIG), adjust amplifier potentiometer |
| **Distorted audio** | Reduce volume: `player.setVolume(150)` |
| **Crashes with long tunes** | Increase stack size in `RTTTLPlayer.cpp` (4096 → 8192) |
| **Can't stop loop** | Call `player.stop()` and wait 100ms |
| **Serial menu not showing** | Set Serial Monitor to 115200 baud |
| **Compile errors** | Ensure ESP32-C3 board is selected in PlatformIO/Arduino IDE |

## 📁 Library Structure

```
RTTTLPlayer/
├── src/
│   ├── RTTTLPlayer.h     # Main library interface
│   ├── RTTTLPlayer.cpp   # Library implementation
│   └── RTTTLTunes.h      # 20+ pre-loaded tunes
├── examples/
│   └── MenuPlayer/       # Complete serial menu example
│       └── src/main.cpp
├── library.json          # PlatformIO configuration
├── library.properties    # Arduino IDE configuration
├── README.md            # This file
└── LICENSE              # MIT License
```

## ❓ Frequently Asked Questions

**Q: Can I use this with other ESP32 boards?**  
A: Yes! It works with ESP32, ESP32-S2, and ESP32-S3, but is optimized for ESP32-C3.

**Q: How much memory does it use?**  
A: Approximately 5KB RAM during playback, plus tune storage.

**Q: Can I play multiple tunes at once?**  
A: No, only one tune at a time (monophonic playback).

**Q: What's the maximum tune length?**  
A: Limited by available heap memory. Typical tunes are 100-500 bytes.

**Q: Does it work with PlatformIO and Arduino IDE?**  
A: Yes! Both are fully supported.

## 📄 License

MIT License

Copyright (c) 2024 Econode NZ  
Author: Matt Way

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

## 👥 Credits & Acknowledgments

- **Author**: Matt Way, [Econode NZ](https://www.econode.nz)
- **RTTTL Format**: Originally from Nokia
- **FreeRTOS**: For non-blocking task management
- **ESP32 Arduino Core**: Espressif Systems
- **Test Hardware**: Jaycar XC3744 amplifier module

## 🤝 Contributing

Found a bug or have a feature request?  
1. Check the [Issues](https://github.com/econode-nz/RTTTLPlayer/issues)
2. Fork the repository
3. Create a feature branch
4. Submit a Pull Request

---

**Happy music making!** 🎶  
For questions or support, open an issue on [GitHub](https://github.com/econode-nz/RTTTLPlayer/issues).

*If this library helped your project, consider giving it a ⭐ on GitHub!*