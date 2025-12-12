#ifndef RTTTLPLAYER_H
#define RTTTLPLAYER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

class RTTTLPlayer {
public:
    /**
     * @brief Constructor for RTTTL Player
     * @param pin GPIO pin connected to audio amplifier (default: GPIO3)
     * @param defaultVolume Initial volume level 0-255 (default: 180)
     */
    RTTTLPlayer(uint8_t pin = 3, uint8_t defaultVolume = 180);
    
    /**
     * @brief Destructor - cleans up FreeRTOS resources
     */
    ~RTTTLPlayer();
    
    /**
     * @brief Initialize the player (must be called in setup())
     */
    void begin();
    
    /**
     * @brief Play an RTTTL string
     * @param rtttl RTTTL format string to play
     * @param loopCount 0=play once, 1-254=loop X times, 255=loop forever
     * @return true if playback started successfully
     */
    bool play(const char* rtttl, uint8_t loopCount = 0);
    
    /**
     * @brief Stop playback immediately
     */
    void stop();
    
    /**
     * @brief Check if player is currently playing
     * @return true if playing
     */
    bool isPlaying() const { return _playing; }
    
    /**
     * @brief Check if player is in loop mode
     * @return true if looping
     */
    bool isLooping() const { return _loopCount > 0; }
    
    /**
     * @brief Set playback volume
     * @param volume Volume level (0-255)
     */
    void setVolume(uint8_t volume);
    
    /**
     * @brief Get current volume
     * @return Current volume level (0-255)
     */
    uint8_t getVolume() const { return _volume; }
    
    /**
     * @brief Enable/disable debug serial output
     * @param enabled true to enable debug messages
     */
    void setDebug(bool enabled) { _debug = enabled; }
    
    /**
     * @brief Get remaining loop count
     * @return 0=not looping, 1-254=loops remaining, 255=infinite loop
     */
    uint8_t getRemainingLoops() const { return _loopCount; }
    
private:
    // Static task function for FreeRTOS
    static void playerTask(void* parameter);
    
    // Internal parsing and playback
    bool parseAndPlay(const char* rtttl);
    int parseNumber(const char* &ptr);
    float getFrequency(uint8_t note, uint8_t octave);
    int calculateDuration(int duration, int dots, int bpm);
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
    
    // Note frequency table (C0 to B8)
    static const float NOTE_FREQUENCIES[108];
    
    // Command types
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