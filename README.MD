# RTTTL Player for ESP32-C3

A lightweight, non-blocking RTTTL music player library for ESP32-C3 microcontrollers. Plays tunes in the background via FreeRTOS while your main code continues running.

## 📦 Installation

### PlatformIO

Add to your `platformio.ini`:

```ini
[env:esp32-c3]
platform = espressif32
board = esp32-c3-devkitc-02
framework = arduino
lib_deps = 
    https://github.com/econode-nz/RTTTLPlayer.git