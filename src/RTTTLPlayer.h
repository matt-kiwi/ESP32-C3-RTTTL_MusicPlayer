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
     * @brief Get remaining loop count
     * @return 0=not looping, 1-254=loops remaining, 255=infinite loop
     */
    uint8_t getRemainingLoops() const { return _loopCount; }
    
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
     * @brief Set tempo scaling factor
     * @param scale 0.5 = half speed, 1.0 = normal, 2.0 = double speed
     */
    void setTempoScale(float scale);
    
    /**
     * @brief Get current tempo scaling
     * @return Current tempo multiplier
     */
    float getTempoScale() const { return _tempoScale; }
    
    /**
     * @brief Get current frequency being played
     * @return Frequency in Hz, or 0 if no note is playing (rest/pause)
     */
    float getCurrentFrequency() const { return _currentFrequency; }
    
    /**
     * @brief Get current BPM including tempo scaling
     * @return Effective BPM (original BPM × tempo scale)
     */
    int getCurrentBPM() const { return static_cast<int>(_currentBpm * _tempoScale + 0.5f); }
    
    /**
     * @brief Enable/disable debug serial output
     * @param enabled true to enable debug messages
     */
    void setDebug(bool enabled) { _debug = enabled; }
    
private:
    /**
     * @brief FreeRTOS task function for background playback
     */
    static void playerTask(void* parameter);
    
    /**
     * @brief Parse and play an RTTTL string
     * @param rtttl RTTTL string to play
     * @return true if successful
     */
    bool parseAndPlay(const char* rtttl);
    
    /**
     * @brief Parse a number from string
     * @param ptr Reference to string pointer (will be advanced)
     * @return Parsed number
     */
    int parseNumber(const char* &ptr);
    
    /**
     * @brief Get frequency for a note and octave
     * @param note Note index (0=C, 1=C#, 2=D, ..., 11=B)
     * @param octave Octave number (0-8)
     * @return Frequency in Hz
     */
    float getFrequency(uint8_t note, uint8_t octave);
    
    /**
     * @brief Calculate note duration in milliseconds
     * @param duration Note duration value (4=quarter, 8=eighth, etc.)
     * @param dots Number of dots (dotted notes)
     * @param bpm Beats per minute
     * @return Duration in milliseconds
     */
    int calculateDuration(int duration, int dots, int bpm);
    
    /**
     * @brief Play a single note
     * @param frequency Note frequency in Hz (0 for rest)
     * @param durationMs Duration in milliseconds
     */
    void playNote(float frequency, int durationMs);
    
    // Member variables
    uint8_t _pin;               ///< GPIO pin for audio output
    uint8_t _volume;            ///< Current volume level (0-255)
    volatile bool _playing;     ///< True if currently playing
    bool _debug;                ///< Debug output enabled
    TaskHandle_t _playerTask;   ///< FreeRTOS task handle
    QueueHandle_t _commandQueue;///< Command queue for task communication
    
    // Loop control
    volatile uint8_t _loopCount;///< Current loop count (0=once, 255=forever)
    char* _currentTune;         ///< Current RTTTL string (in heap)
    
    // New: Tempo and frequency tracking
    volatile float _tempoScale;      ///< Tempo multiplier (default 1.0)
    volatile int _currentBpm;        ///< Original BPM from RTTTL
    volatile float _currentFrequency;///< Current note frequency (0 for rest)
    
    // Note frequency table (C0 to B8)
    static const float NOTE_FREQUENCIES[108];
    
    // Command types for queue
    enum CommandType { PLAY, STOP, SET_VOLUME, SET_TEMPO };
    
    /**
     * @brief Queue message structure
     */
    struct PlayerCommand {
        CommandType type;        ///< Command type
        const char* tune;        ///< RTTTL string (for PLAY)
        uint8_t loopCount;       ///< Loop count (for PLAY)
        uint8_t volume;          ///< Volume (for SET_VOLUME)
        float tempoScale;        ///< Tempo scale (for SET_TEMPO)
    };
};

#endif // RTTTLPLAYER_H