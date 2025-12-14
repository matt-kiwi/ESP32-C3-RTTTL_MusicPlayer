# RTTTL Player for ESP32-C3

[![PlatformIO Registry](https://badges.platformio.org/packages/econode-nz/library/RTTTLPlayer.svg)](https://registry.platformio.org/libraries/econode-nz/RTTTLPlayer)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/econode-nz/RTTTLPlayer)

A lightweight, polling RTTTL music player library for ESP32 series microcontrollers.

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

# RTTTL Player API Reference

## 📋 **Overview**
A non-blocking, polling-based RTTTL music player for ESP32 with NeoPixel/FastLED compatibility. Uses flash memory for note storage to conserve RAM. Compatible with ESP32, ESP32-C3, and ESP32-S3.

## 📁 **Class Definition**
```cpp
class RTTTLPlayer
```

## 🏗️ **Constructor**

### `RTTTLPlayer(pin, defaultVolume)`
Creates a new RTTTL player instance.

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `pin` | `uint8_t` | `3` | GPIO pin for audio PWM output |
| `defaultVolume` | `uint8_t` | `180` | Initial volume (0-255) |

**Example:**
```cpp
RTTTLPlayer player(32, 200);  // Pin 32, volume 200
```

## 🎵 **Playback Control**

### `begin()`
Initializes the player hardware. **Must be called in `setup()`**.

**Parameters:** None  
**Returns:** `void`

**Example:**
```cpp
void setup() {
    player.begin();
}
```

### `play(rtttl, loopCount)`
Starts playing an RTTTL string.

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `rtttl` | `const char*` | Required | RTTTL format string |
| `loopCount` | `uint8_t` | `0` | `0`=once, `1-254`=X times, `255`=forever |

**Returns:** `bool` - `true` if playback started successfully

**Example:**
```cpp
player.play("Nokia:d=4,o=5,b=160:8e6,8d6,f#,8e6", 2);  // Play twice
player.play(RTTTLTunes::marioPowerUp, 255);           // Loop forever
```

### `stop()`
Stops playback immediately and silences audio.

**Parameters:** None  
**Returns:** `void`

**Example:**
```cpp
player.stop();
```

### `loop()`
Updates player state. **Must be called in main `loop()`**.

**Parameters:** None  
**Returns:** `void`

**Example:**
```cpp
void loop() {
    player.loop();  // Essential for non-blocking playback
    // ... other code ...
}
```

## 🔊 **Volume Control**

### `setVolume(volume)`
Sets playback volume.

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| `volume` | `uint8_t` | 0-255 | `0`=silent, `255`=maximum |

**Returns:** `void`

**Example:**
```cpp
player.setVolume(150);  // Medium volume
```

### `getVolume()`
Gets current volume setting.

**Returns:** `uint8_t` - Current volume (0-255)

**Example:**
```cpp
uint8_t vol = player.getVolume();  // Returns current volume
```

## ⏱️ **Tempo Control**

### `setTempoScale(scale)`
Sets tempo scaling factor.

| Parameter | Type | Range | Description |
|-----------|------|-------|-------------|
| `scale` | `float` | 0.1-4.0 | `0.5`=half speed, `1.0`=normal, `2.0`=double speed |

**Returns:** `void`

**Example:**
```cpp
player.setTempoScale(0.5);   // Play at half speed
player.setTempoScale(2.0);   // Play at double speed
```

### `getTempoScale()`
Gets current tempo scaling factor.

**Returns:** `float` - Current tempo multiplier

**Example:**
```cpp
float tempo = player.getTempoScale();  // e.g., 1.5
```

### `getCurrentBPM()`
Gets effective BPM including tempo scaling.

**Returns:** `int` - Current BPM (original BPM × tempo scale)

**Example:**
```cpp
int bpm = player.getCurrentBPM();  // e.g., 180
```

## 📊 **EQ / Frequency Tracking**

### `getCurrentFrequency()`
Gets frequency of currently playing note.

**Returns:** `float` - Frequency in Hz, or `0.0` for rests/pauses

**Example:**
```cpp
float freq = player.getCurrentFrequency();
if (freq > 0) {
    Serial.printf("Playing: %.1fHz\n", freq);
}
```

## 🔍 **Status Queries**

### `isPlaying()`
Checks if player is currently playing audio.

**Returns:** `bool` - `true` if playing, `false` if idle

**Example:**
```cpp
if (player.isPlaying()) {
    Serial.println("Audio is playing");
}
```

### `isLooping()`
Checks if player is in loop mode.

**Returns:** `bool` - `true` if looping, `false` if playing once

**Example:**
```cpp
if (player.isLooping()) {
    Serial.println("In loop mode");
}
```

### `getRemainingLoops()`
Gets remaining loop count.

**Returns:** `uint8_t` - `0`=not looping, `1-254`=loops remaining, `255`=infinite loop

**Example:**
```cpp
uint8_t loops = player.getRemainingLoops();
```

## 🐛 **Debugging**

### `setDebug(enabled)`
Enables/disables debug serial output.

| Parameter | Type | Description |
|-----------|------|-------------|
| `enabled` | `bool` | `true` to enable debug messages |

**Returns:** `void`

**Example:**
```cpp
player.setDebug(true);  // Enable debug output
```

## 📝 **RTTTL Format Reference**

### Basic Structure
```
Name:d=default_duration,o=default_octave,b=beats_per_minute:notes
```

### Example RTTTL String
```
Nokia:d=4,o=5,b=160:8e6,8d6,f#,8e6
```

### Note Syntax
- **`8e6`**: Eighth note, E, octave 6
- **`f#`**: F sharp (uses default duration/octave)
- **`p`**: Pause/rest

## 🎯 **Quick Start Example**

```cpp
#include "RTTTLPlayer.h"
#include <RTTTLTunes.h>

RTTTLPlayer player(32, 180);

void setup() {
    Serial.begin(115200);
    player.begin();
    player.setDebug(true);
}

void loop() {
    player.loop();  // Essential!
    
    // Control example
    if (someCondition) {
        player.play(RTTTLTunes::marioPowerUp);
        player.setTempoScale(1.5);  // Speed up
    }
    
    // EQ visualization example
    float freq = player.getCurrentFrequency();
    if (freq > 0) {
        // Visualize frequency...
    }
    
    delay(10);
}
```

## 📊 **Memory Usage**
- **Flash**: ~4KB (code + frequency table)
- **RAM**: ~100 bytes (state variables only)
- **Frequency Table**: Stored in flash memory

## ⚠️ **Important Notes**
1. **Always call `player.loop()`** in your main loop
2. **Initialize with `player.begin()`** in `setup()`
3. **Volume range**: 0-255 (0=silent)
4. **Tempo range**: 0.1-4.0 (10% to 400% speed)
5. **Compatible** with FastLED/NeoPixels (no FreeRTOS conflicts)

## 🔧 **Troubleshooting**

| Issue | Solution |
|-------|----------|
| No sound | Check `player.loop()` is called in main loop |
| Audio stops with FastLED | Ensure `player.loop()` is called every iteration |
| Volume too low | Use `player.setVolume(200+)` |
| Compilation errors | Ensure `RTTTLPlayer.h` and `.cpp` are in project |

This API provides complete control over RTTTL playback with minimal RAM usage and full NeoPixel compatibility.


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
    player.loop();
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

**Tested with:**
- **Amplifier**: SC8002B module (Jaycar XC3744 or similar)
- **Speaker**: 8Ω 0.5W-2W
- **ESP32-C3**: ESP32-C3-DevKitC-02 or similar


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

## Examples

 * `examples/simple` - Basic Usage
 * `examples/simple_eq` - Text EQ Meter
 * `examples/MenuPlayer` - Complete Interactive Player


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


## 🔗 More RTTTL Tunes Online

Find thousands of RTTTL tunes at:
- **[Cell Ringtones](http://www.cellringtones.com/)** - Large collection of ringtones
- **[RTTTL.js Online Player](https://1j01.github.io/rtttl.js/)** - Test and convert tunes
- **Search for** "RTTTL ringtones" or "RTTTL converter"




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


## 📄 License

MIT License

Copyright (c) 2025 Econode NZ  
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