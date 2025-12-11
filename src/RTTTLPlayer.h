#ifndef RTTTLPLAYER_H
#define RTTTLPLAYER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

class RTTTLPlayer {
public:
    // Constructor
    RTTTLPlayer(uint8_t pin = 3, uint8_t defaultVolume = 180);
    
    // Destructor
    ~RTTTLPlayer();
    
    // Initialize the player (call in setup())
    void begin();
    
    // Play an RTTTL string (non-blocking)
    // loopCount: 0 = no loop, 1-254 = loop X times, 255 = loop forever
    bool play(const char* rtttl, uint8_t loopCount = 0);
    
    // Stop playback immediately
    void stop();
    
    // Check if currently playing
    bool isPlaying() const { return _playing; }
    
    // Check if currently looping
    bool isLooping() const { return _loopCount > 0; }
    
    // Get/set volume (0-255)
    void setVolume(uint8_t volume);
    uint8_t getVolume() const { return _volume; }
    
    // Enable/disable debug output
    void setDebug(bool enabled) { _debug = enabled; }
    
private:
    // Static task function
    static void playerTask(void* parameter);
    
    // Parse and play RTTTL
    bool parseAndPlay(const char* rtttl);
    
    // Parse a number from string
    int parseNumber(const char* &ptr);
    
    // Get frequency for a note
    float getFrequency(uint8_t note, uint8_t octave);
    
    // Calculate duration in milliseconds
    int calculateDuration(int duration, int dots, int bpm);
    
    // Play a single note
    void playNote(float frequency, int durationMs);
    
    // Member variables
    uint8_t _pin;
    uint8_t _volume;
    volatile bool _playing;
    bool _debug;
    TaskHandle_t _playerTask;
    QueueHandle_t _commandQueue;
    
    // Loop control
    volatile uint8_t _loopCount;
    char* _currentTune;
    
    // Note frequency table
    static const float NOTE_FREQUENCIES[108];
    
    // Command types for queue
    enum CommandType { PLAY, STOP, SET_VOLUME };
    
    // Queue message structure
    struct PlayerCommand {
        CommandType type;
        const char* tune;     // For PLAY commands
        uint8_t loopCount;    // For PLAY commands
        uint8_t volume;       // For SET_VOLUME commands
    };
};

#endif // RTTTLPLAYER_H