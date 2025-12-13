# RTTTL Player for ESP32-C3

[![PlatformIO Registry](https://badges.platformio.org/packages/econode-nz/library/RTTTLPlayer.svg)](https://registry.platformio.org/libraries/econode-nz/RTTTLPlayer)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/econode-nz/RTTTLPlayer)

A lightweight, non-blocking RTTTL music player library for ESP32-C3 microcontrollers. Plays tunes in the background via FreeRTOS while your main code continues running—perfect for games, alarms, notifications, or background music.

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

## 🚀 Quick Start

### Basic Playback
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

### With Tempo Control
```cpp
// Speed up playback (2x faster)
player.setTempoScale(2.0);
player.play(RTTTLTunes::mario);

// Slow down playback (0.5x slower)
player.setTempoScale(0.5);
player.play(RTTTLTunes::imperial);

// Get current BPM (including tempo scaling)
int currentBPM = player.getCurrentBPM();
```

### With EQ Meter / Frequency Tracking
```cpp
void loop() {
    // Get current frequency for visual effects
    float currentFreq = player.getCurrentFrequency();
    
    if (currentFreq > 20) {
        // Note is playing - use for visualizer
        if (currentFreq < 250) {
            // Bass frequency - light up bass LEDs
        } else if (currentFreq < 2000) {
            // Mid frequency
        } else {
            // Treble frequency
        }
    }
    
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

### Basic Playback
```cpp
void begin();                            // Initialize (call in setup())
bool play(const char* rtttl, uint8_t loopCount = 0);  // Play RTTTL string
void stop();                             // Stop playback immediately

// Loop control: 0=once, 1-254=X times, 255=forever
bool isPlaying();                        // Check if playing
bool isLooping();                        // Check if looping
uint8_t getRemainingLoops();             // Get loops remaining
```

### Volume Control
```cpp
void setVolume(uint8_t volume);          // 0-255
uint8_t getVolume();                     // Get current volume
```

### Tempo Control
```cpp
/**
 * Set tempo scaling factor
 * @param scale 0.25 = quarter speed, 1.0 = normal, 4.0 = 4x speed
 */
void setTempoScale(float scale);

/**
 * Get current tempo scaling
 * @return Current tempo multiplier
 */
float getTempoScale();

/**
 * Get effective BPM (original BPM × tempo scale)
 * @return Current BPM including tempo scaling
 */
int getCurrentBPM();
```

### Frequency Tracking
```cpp
/**
 * Get current playing frequency
 * @return Frequency in Hz, or 0 if no note playing (rest/pause)
 * 
 * Useful for:
 * - EQ meters and visualizers
 * - Frequency-based lighting effects
 * - Real-time audio analysis
 */
float getCurrentFrequency();
```

## 🎮 Example Projects

### 1. `examples/simple` - Basic Usage
Minimal example showing core functionality:
```cpp
// Plays a series of tunes with different loop settings
player.play(RTTTLTunes::nokia);           // Play once
player.play(RTTTLTunes::mario, 3);        // Loop 3 times  
player.play(RTTTLTunes::starWars, 255);   // Loop forever
```

**Features:**
- Basic playback with loop control
- Volume adjustment
- Status checking

### 2. `examples/simple_eq` - Text EQ Meter
Real-time frequency visualization in Serial Monitor:
```cpp
// Displays formatted EQ meter as notes play
┌─────────────────────────────────────────────────────────────┐
│  Freq (Hz)  │   BASS   │    MID    │  TREBLE  │  BPM  │ Tempo │
├─────────────────────────────────────────────────────────────┤
│  932 Hz    │          │ ####      │          │  100 │ 1.00x │
│ 1319 Hz    │          │ ######    │          │  100 │ 1.00x │
│ 1568 Hz    │          │ #######   │          │  100 │ 1.00x │
```

**Features:**
- Real-time frequency display
- Text-based EQ visualization (Bass/Mid/Treble)
- Tempo control and BPM display
- Perfect alignment using printf formatting

### 3. `examples/MenuPlayer` - Complete Interactive Player
Full-featured serial menu system with all tunes:
```
=== RTTTL Player Menu ===
--- Music ---
 1. Nokia Ringtone
 2. Super Mario Bros
 3. James Bond Theme
 4. Pink Panther
 5. Imperial March (Star Wars)
...20+ tunes
--- Player Control ---
 s. Play Success Sound
 r. Play Error Sound
 v. Change Volume
 p. Check Player Status
 q. Stop Playback
 x. Enter Custom RTTTL
 m. Show this Menu
```

**Features:**
- Interactive serial menu (115200 baud)
- All 20+ included tunes
- Custom RTTTL input with loop control
- Volume adjustment
- Player status display
- Loop examples (forever, 3x, etc.)

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
│   ├── simple/           # Basic usage example
│   │   └── src/main.cpp
│   ├── simple_eq/        # Text EQ meter example
│   │   └── src/main.cpp
│   └── MenuPlayer/       # Complete interactive menu
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

**Q: Can I visualize the music in real-time?**  
A: Yes! Use `getCurrentFrequency()` for EQ meters, LED visualizers, or serial displays.

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