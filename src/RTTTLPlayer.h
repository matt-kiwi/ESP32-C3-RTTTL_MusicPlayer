#ifndef RTTTLPLAYER_H
#define RTTTLPLAYER_H

#include <Arduino.h>

class RTTTLPlayer {
public:
    /**
     * @brief Construct a new RTTTLPlayer object
     * @param pin GPIO pin for audio output (PWM)
     * @param defaultVolume Initial volume level 0-255 (default: 180)
     */
    RTTTLPlayer(uint8_t pin = 3, uint8_t defaultVolume = 180);
    
    /**
     * @brief Destroy the RTTTLPlayer object
     */
    ~RTTTLPlayer();
    
    /**
     * @brief Initialize the player hardware
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
     * @brief Update player state - call this in your main loop()
     */
    void loop();
    
    /**
     * @brief Check if player is currently playing
     * @return true if audio is playing
     */
    bool isPlaying() const { return _state == STATE_PLAYING; }
    
    /**
     * @brief Check if player is in loop mode
     * @return true if looping enabled
     */
    bool isLooping() const { return _loopCount > 0; }
    
    /**
     * @brief Get remaining loop count
     * @return 0=not looping, 1-254=loops remaining, 255=infinite loop
     */
    uint8_t getRemainingLoops() const { return _loopCount; }
    
    /**
     * @brief Set playback volume
     * @param volume Volume level (0=silent, 255=max)
     */
    void setVolume(uint8_t volume);
    
    /**
     * @brief Get current volume setting
     * @return Current volume level (0-255)
     */
    uint8_t getVolume() const { return _volume; }
    
    /**
     * @brief Set tempo scaling factor
     * @param scale 0.5 = half speed, 1.0 = normal, 2.0 = double speed
     * @note Takes effect on next note
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
     * @note Useful for EQ meters and visualizers
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
    enum PlayerState { STATE_IDLE, STATE_PLAYING, STATE_PAUSED };
    
    /// @brief Note frequency table (C0 to B8) - stored in flash
    static const float NOTE_FREQUENCIES[108];
    
    // Helper functions
    int parseNumber(const char* &ptr);
    int calculateDuration(int duration, int dots, int bpm);
    float getFrequency(uint8_t note, uint8_t octave);
    void parseNextNote();
    
    // Hardware properties
    uint8_t _pin;               ///< GPIO pin for audio output
    uint8_t _volume;            ///< Current volume level (0-255)
    
    // Player state
    PlayerState _state;         ///< Current playback state
    bool _debug;                ///< Debug output enabled
    uint8_t _loopCount;         ///< Remaining loop count
    
    // Playback tracking
    const char* _currentTune;   ///< Pointer to current RTTTL string
    const char* _tunePtr;       ///< Current position in RTTTL string
    unsigned long _noteEndTime; ///< When current note ends (millis)
    
    // Frequency and tempo tracking
    volatile float _currentFrequency; ///< Frequency of currently playing note
    volatile int _currentBpm;         ///< BPM from current RTTTL
    volatile float _tempoScale;       ///< Tempo multiplier (default 1.0)
};

#endif // RTTTLPLAYER_H