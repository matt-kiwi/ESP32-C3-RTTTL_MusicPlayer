#include "RTTTLPlayer.h"
#include <string.h>

// Note frequency table (C0 to B8)
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

// Constructor
RTTTLPlayer::RTTTLPlayer(uint8_t pin, uint8_t defaultVolume) 
    : _pin(pin), _volume(defaultVolume), _playing(false), _debug(true),
      _playerTask(NULL), _commandQueue(NULL), _loopCount(0), _currentTune(NULL) {
}

// Destructor
RTTTLPlayer::~RTTTLPlayer() {
    stop();
    if (_playerTask) {
        vTaskDelete(_playerTask);
    }
    if (_commandQueue) {
        vQueueDelete(_commandQueue);
    }
    if (_currentTune) {
        free(_currentTune);
    }
}

// Initialize the player
void RTTTLPlayer::begin() {
    ledcAttach(_pin, 1000, 8);
    ledcWrite(_pin, _volume);
    
    // Create command queue
    _commandQueue = xQueueCreate(5, sizeof(PlayerCommand));
    
    // Create player task
    xTaskCreate(
        playerTask,       // Task function
        "RTTTLPlayer",    // Task name
        4096,            // Stack size
        this,            // Task parameter
        1,               // Priority
        &_playerTask     // Task handle
    );
    
    if (_debug) {
        Serial.println("RTTTLPlayer initialized");
    }
}

// Set volume
void RTTTLPlayer::setVolume(uint8_t volume) {
    _volume = volume;
    PlayerCommand cmd;
    cmd.type = SET_VOLUME;
    cmd.volume = volume;
    if (_commandQueue) {
        xQueueSend(_commandQueue, &cmd, portMAX_DELAY);
    }
}

// Play an RTTTL string with loop option
bool RTTTLPlayer::play(const char* rtttl, uint8_t loopCount) {
    if (!_commandQueue) {
        return false;
    }
    
    // Stop any current playback
    stop();
    
    // Copy the tune to heap
    if (_currentTune) {
        free(_currentTune);
    }
    _currentTune = strdup(rtttl);
    
    if (!_currentTune) {
        return false;
    }
    
    // Set loop count
    _loopCount = loopCount;
    
    // Send play command
    PlayerCommand cmd;
    cmd.type = PLAY;
    cmd.tune = _currentTune;
    cmd.loopCount = loopCount;
    
    if (xQueueSend(_commandQueue, &cmd, portMAX_DELAY) == pdTRUE) {
        _playing = true;
        if (_debug) {
            Serial.print("Playing: ");
            Serial.print(rtttl);
            if (loopCount == 0) {
                Serial.println(" (no loop)");
            } else if (loopCount == 255) {
                Serial.println(" (loop forever)");
            } else {
                Serial.print(" (loop ");
                Serial.print(loopCount);
                Serial.println(" times)");
            }
        }
        return true;
    }
    
    free(_currentTune);
    _currentTune = NULL;
    _loopCount = 0;
    return false;
}

// Stop playback
void RTTTLPlayer::stop() {
    _playing = false;
    _loopCount = 0;
    
    PlayerCommand cmd;
    cmd.type = STOP;
    if (_commandQueue) {
        xQueueSend(_commandQueue, &cmd, portMAX_DELAY);
    }
    
    // Also stop audio immediately
    ledcWriteTone(_pin, 0);
}

// Player task function
void RTTTLPlayer::playerTask(void* parameter) {
    RTTTLPlayer* player = static_cast<RTTTLPlayer*>(parameter);
    PlayerCommand cmd;
    
    while (true) {
        // Wait for commands
        if (xQueueReceive(player->_commandQueue, &cmd, portMAX_DELAY) == pdTRUE) {
            switch (cmd.type) {
                case PLAY: {
                    player->_playing = true;
                    uint8_t loopsRemaining = cmd.loopCount;
                    
                    do {
                        // Play one iteration
                        player->parseAndPlay(cmd.tune);
                        
                        // Update loop counter if not infinite
                        if (loopsRemaining > 0 && loopsRemaining < 255) {
                            loopsRemaining--;
                        }
                        
                        // Brief pause between loops (except when stopping)
                        if (player->_playing && loopsRemaining > 0) {
                            delay(50);
                        }
                        
                    } while (player->_playing && loopsRemaining > 0);
                    
                    player->_playing = false;
                    player->_loopCount = 0;
                    
                    // Free the tune memory
                    if (player->_currentTune) {
                        free(player->_currentTune);
                        player->_currentTune = NULL;
                    }
                    
                    break;
                }
                case STOP:
                    player->_playing = false;
                    player->_loopCount = 0;
                    ledcWriteTone(player->_pin, 0);
                    if (player->_debug) {
                        Serial.println("Playback stopped");
                    }
                    break;
                case SET_VOLUME:
                    player->_volume = cmd.volume;
                    ledcWrite(player->_pin, player->_volume);
                    if (player->_debug) {
                        Serial.printf("Volume set to: %d\n", player->_volume);
                    }
                    break;
            }
        }
    }
}

// Parse a number from string
int RTTTLPlayer::parseNumber(const char* &ptr) {
    int num = 0;
    while (*ptr >= '0' && *ptr <= '9') {
        num = num * 10 + (*ptr - '0');
        ptr++;
    }
    return num;
}

// Get frequency for a note
float RTTTLPlayer::getFrequency(uint8_t note, uint8_t octave) {
    int index = (octave * 12) + note;
    if (index >= 0 && index < 108) {
        return NOTE_FREQUENCIES[index];
    }
    return 0.0;
}

// Calculate duration in milliseconds
int RTTTLPlayer::calculateDuration(int duration, int dots, int bpm) {
    // Base duration: quarter note = (60000 / bpm)
    // Then divide by duration value (4 = quarter, 8 = eighth, etc.)
    int baseMs = (60000 / bpm) * 4 / duration;
    
    // Apply dots: dotted note = note + half of its value
    int total = baseMs;
    int add = baseMs / 2;
    for (int i = 0; i < dots; i++) {
        total += add;
        add /= 2;
    }
    
    return total;
}

// Play a single note
void RTTTLPlayer::playNote(float frequency, int durationMs) {
    if (frequency > 20.0) {
        ledcWriteTone(_pin, frequency);
        ledcWrite(_pin, _volume);
        
        // Play most of the note
        int playTime = durationMs - 20;
        if (playTime > 0) {
            delay(playTime);
        }
        
        // Gentle fade out
        for (int v = _volume; v > max(_volume/4, 30); v -= 15) {
            ledcWrite(_pin, v);
            delay(1);
        }
        
        ledcWriteTone(_pin, 0);
        delay(20); // Short silence between notes
    } else {
        // Pause
        ledcWriteTone(_pin, 0);
        delay(durationMs);
    }
}

// Parse and play RTTTL
bool RTTTLPlayer::parseAndPlay(const char* rtttl) {
    if (!rtttl || !*rtttl) {
        return false;
    }
    
    const char* p = rtttl;
    
    // Find the title separator
    while (*p && *p != ':') p++;
    if (!*p) return false;
    p++; // Skip first colon
    
    // Parse defaults
    int defaultDuration = 4;
    int defaultOctave = 6;
    int bpm = 63;
    
    while (*p && *p != ':') {
        if (*p == 'd' && *(p+1) == '=') {
            p += 2;
            defaultDuration = parseNumber(p);
        }
        else if (*p == 'o' && *(p+1) == '=') {
            p += 2;
            defaultOctave = parseNumber(p);
        }
        else if (*p == 'b' && *(p+1) == '=') {
            p += 2;
            bpm = parseNumber(p);
        }
        if (*p == ',') p++;
    }
    
    if (!*p) return false;
    p++; // Skip second colon
    
    // Parse and play notes
    while (*p && _playing) {
        // Skip spaces and commas
        while (*p == ' ' || *p == ',') p++;
        if (!*p) break;
        
        int duration = defaultDuration;
        int noteValue = -1; // -1 = pause
        int octave = defaultOctave;
        int dots = 0;
        
        // Parse duration
        if (*p >= '0' && *p <= '9') {
            duration = parseNumber(p);
        }
        
        // Parse note letter
        char noteChar = *p;
        switch (noteChar) {
            case 'c': case 'C': noteValue = 0; break;
            case 'd': case 'D': noteValue = 2; break;
            case 'e': case 'E': noteValue = 4; break;
            case 'f': case 'F': noteValue = 5; break;
            case 'g': case 'G': noteValue = 7; break;
            case 'a': case 'A': noteValue = 9; break;
            case 'b': case 'B': noteValue = 11; break;
            case 'p': case 'P': noteValue = -1; break; // Pause
        }
        
        if (noteValue == -1) {
            // It's a pause
            p++; // Skip 'p'
        } else {
            // It's a note
            p++; // Skip note letter
            
            // Check for sharp
            if (*p == '#') {
                noteValue++;
                p++;
            }
            
            // Parse octave
            if (*p >= '0' && *p <= '9') {
                octave = *p - '0';
                p++;
            }
        }
        
        // Parse dots
        while (*p == '.') {
            dots++;
            p++;
        }
        
        // Calculate frequency and duration
        float frequency = 0.0;
        if (noteValue >= 0) {
            frequency = getFrequency(noteValue, octave);
        }
        
        int durationMs = calculateDuration(duration, dots, bpm);
        playNote(frequency, durationMs);
    }
    
    ledcWriteTone(_pin, 0);
    return true;
}