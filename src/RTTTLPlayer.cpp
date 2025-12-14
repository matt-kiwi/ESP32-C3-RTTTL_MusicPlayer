#include "RTTTLPlayer.h"
#include <string.h>

// ============================================================================
// NOTE FREQUENCY TABLE (FLASH MEMORY)
// ============================================================================

const float RTTTLPlayer::NOTE_FREQUENCIES[108] = {
    // Octave 0
    16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87,
    // Octave 1
    32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55.00, 58.27, 61.74,
    // Octave 2
    65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47,
    // Octave 3
    130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
    // Octave 4 (A4 = 440Hz is at index 57)
    261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
    // Octave 5
    523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77,
    // Octave 6
    1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760.00, 1864.66, 1975.53,
    // Octave 7
    2093.00, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.00, 3729.31, 3951.07,
    // Octave 8
    4186.01, 4434.92, 4698.63, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040.00, 7458.62, 7902.13
};

// ============================================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================================

RTTTLPlayer::RTTTLPlayer(uint8_t pin, uint8_t defaultVolume) 
    : _pin(pin), _volume(defaultVolume), _state(STATE_IDLE), _debug(true),
      _loopCount(0), _currentTune(NULL), _tunePtr(NULL),
      _noteEndTime(0), _currentFrequency(0), _currentBpm(120),
      _tempoScale(1.0f) {
}

RTTTLPlayer::~RTTTLPlayer() {
    stop();
    ledcWriteTone(_pin, 0);
}

// ============================================================================
// PUBLIC METHODS
// ============================================================================

void RTTTLPlayer::begin() {
    ledcAttach(_pin, 1000, 8);
    ledcWrite(_pin, 0);
    if (_debug) Serial.println("[RTTTL] Player initialized");
}

bool RTTTLPlayer::play(const char* rtttl, uint8_t loopCount) {
    stop();
    
    _currentTune = rtttl;
    _tunePtr = rtttl;
    _loopCount = loopCount;
    _state = STATE_PLAYING;
    _noteEndTime = 0;
    _tempoScale = 1.0f;  // Reset tempo on new play
    
    // Skip to the notes section
    while (*_tunePtr && *_tunePtr != ':') _tunePtr++;
    if (!*_tunePtr) return false;
    _tunePtr++;
    
    // Parse default settings
    int defaultDuration = 4;
    int defaultOctave = 6;
    _currentBpm = 63;
    
    while (*_tunePtr && *_tunePtr != ':') {
        if (*_tunePtr == 'd' && *(_tunePtr+1) == '=') {
            _tunePtr += 2;
            defaultDuration = parseNumber(_tunePtr);
        }
        else if (*_tunePtr == 'o' && *(_tunePtr+1) == '=') {
            _tunePtr += 2;
            defaultOctave = parseNumber(_tunePtr);
        }
        else if (*_tunePtr == 'b' && *(_tunePtr+1) == '=') {
            _tunePtr += 2;
            _currentBpm = parseNumber(_tunePtr);
        }
        if (*_tunePtr == ',') _tunePtr++;
    }
    
    if (!*_tunePtr) return false;
    _tunePtr++;
    
    if (_debug) {
        Serial.print("[RTTTL] Playing: ");
        Serial.println(rtttl);
    }
    
    return true;
}

void RTTTLPlayer::stop() {
    _state = STATE_IDLE;
    _loopCount = 0;
    _currentTune = NULL;
    _tunePtr = NULL;
    ledcWriteTone(_pin, 0);
    _currentFrequency = 0;
}

void RTTTLPlayer::loop() {
    if (_state != STATE_PLAYING) return;
    
    if (millis() >= _noteEndTime) {
        parseNextNote();
    }
}

void RTTTLPlayer::setVolume(uint8_t volume) {
    _volume = volume;
    if (_state == STATE_PLAYING && _currentFrequency > 20.0) {
        ledcWrite(_pin, _volume);
    }
}

void RTTTLPlayer::setTempoScale(float scale) {
    if (scale < 0.1f) scale = 0.1f;   // Minimum 10% speed
    if (scale > 4.0f) scale = 4.0f;   // Maximum 400% speed
    _tempoScale = scale;
    
    if (_debug) {
        Serial.printf("[RTTTL] Tempo scale set to: %.2fx\n", _tempoScale);
    }
}

// ============================================================================
// PRIVATE HELPER METHODS
// ============================================================================

void RTTTLPlayer::parseNextNote() {
    if (!_tunePtr || !*_tunePtr) {
        // End of tune
        if (_loopCount > 0) {
            if (_loopCount < 255) _loopCount--;
            if (_loopCount > 0) {
                play(_currentTune, _loopCount);
                return;
            }
        }
        stop();
        return;
    }
    
    // Skip spaces and commas
    while (*_tunePtr == ' ' || *_tunePtr == ',') _tunePtr++;
    if (!*_tunePtr) {
        stop();
        return;
    }
    
    // Parse one note
    int duration = 4;
    int noteValue = -1;
    int octave = 6;
    int dots = 0;
    
    if (*_tunePtr >= '0' && *_tunePtr <= '9') {
        duration = parseNumber(_tunePtr);
    }
    
    char noteChar = *_tunePtr;
    switch (noteChar) {
        case 'c': case 'C': noteValue = 0; break;
        case 'd': case 'D': noteValue = 2; break;
        case 'e': case 'E': noteValue = 4; break;
        case 'f': case 'F': noteValue = 5; break;
        case 'g': case 'G': noteValue = 7; break;
        case 'a': case 'A': noteValue = 9; break;
        case 'b': case 'B': noteValue = 11; break;
        case 'p': case 'P': noteValue = -1; break;
    }
    
    if (noteValue == -1) {
        _tunePtr++;
    } else {
        _tunePtr++;
        if (*_tunePtr == '#') {
            noteValue++;
            _tunePtr++;
        }
        if (*_tunePtr >= '0' && *_tunePtr <= '9') {
            octave = *_tunePtr - '0';
            _tunePtr++;
        }
    }
    
    while (*_tunePtr == '.') {
        dots++;
        _tunePtr++;
    }
    
    // Calculate frequency (0.0 for rests)
    float frequency = 0.0;
    if (noteValue >= 0) {
        frequency = getFrequency(noteValue, octave);
    }
    
    // Calculate duration with tempo scaling
    int durationMs = calculateDuration(duration, dots, _currentBpm);
    if (_tempoScale != 1.0f) {
        durationMs = static_cast<int>(durationMs / _tempoScale);
    }
    
    // Play the note
    _currentFrequency = frequency;
    
    if (frequency > 20.0) {
        ledcWriteTone(_pin, frequency);
        ledcWrite(_pin, _volume);
    } else {
        ledcWriteTone(_pin, 0);
    }
    
    // Schedule next note
    _noteEndTime = millis() + durationMs;
    
    if (_debug) {
        Serial.printf("[RTTTL] Note: %.1fHz for %dms (BPM: %d)\n", 
                     frequency, durationMs, getCurrentBPM());
    }
}

int RTTTLPlayer::parseNumber(const char* &ptr) {
    int num = 0;
    while (*ptr >= '0' && *ptr <= '9') {
        num = num * 10 + (*ptr - '0');
        ptr++;
    }
    return num;
}

int RTTTLPlayer::calculateDuration(int duration, int dots, int bpm) {
    int baseMs = (60000 / bpm) * 4 / duration;
    int total = baseMs;
    int add = baseMs / 2;
    for (int i = 0; i < dots; i++) {
        total += add;
        add /= 2;
    }
    return total;
}

float RTTTLPlayer::getFrequency(uint8_t note, uint8_t octave) {
    int index = (octave * 12) + note;
    if (index >= 0 && index < 108) {
        return NOTE_FREQUENCIES[index];
    }
    return 0.0;
}