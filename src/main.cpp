#include <Arduino.h>
#include "RTTTLPlayer.h"
#include "RTTTLTunes.h"

// Create player instance
RTTTLPlayer player(3, 180);

// Variables for main loop tasks
unsigned long lastBlinkTime = 0;
unsigned long lastSensorRead = 0;
int blinkState = LOW;

// Function prototypes
void playTune(const char* name, const char* tune, uint8_t loopCount = 0);
void printMenu();
void enterCustomRTTTL();
void doOtherWork();

void setup() {
    Serial.begin(115200);
    delay(3000);
    
    Serial.println("\n==========================================");
    Serial.println("   ESP32-C3 RTTTL Music Player");
    Serial.println("   http://www.econode.nz ");
    Serial.println("==========================================");
    printMenu();
    
    // Initialize player
    player.begin();
    player.setDebug(true);
    
    // Play startup sound
    player.play(RTTTLTunes::startup);
    
    // Setup other things
    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.println("System ready! Music plays in background.");
    printMenu();
}

void loop() {
    // Check for serial commands
    if (Serial.available()) {
        char choice = Serial.read();
        Serial.println(choice);
        Serial.println();
        
        switch(choice) {
            case '1': playTune("Nokia", RTTTLTunes::nokia); break;
            case '2': playTune("Super Mario Bros", RTTTLTunes::mario); break;
            case '3': playTune("James Bond", RTTTLTunes::bond); break;
            case '4': playTune("Pink Panther", RTTTLTunes::pinkPanther); break;
            case '5': playTune("Imperial March", RTTTLTunes::imperial); break;
            case '6': playTune("Addams Family", RTTTLTunes::addams); break;
            case '7': playTune("Mission Impossible", RTTTLTunes::mission); break;
            case '8': playTune("Indiana Jones", RTTTLTunes::indiana); break;
            case '9': playTune("Tetris", RTTTLTunes::tetris); break;
            case '0': playTune("MASH", RTTTLTunes::mash); break;
            case 'a': playTune("Happy Birthday", RTTTLTunes::happyBirthday); break;
            case 'b': playTune("Canon in D", RTTTLTunes::canon); break;
            case 'c': playTune("Für Elise", RTTTLTunes::furElise); break;
            case 'd': playTune("Game of Thrones", RTTTLTunes::gameOfThrones); break;
            case 'e': playTune("Harry Potter", RTTTLTunes::harryPotter); break;
            case 'f': playTune("Zelda's Lullaby", RTTTLTunes::zelda); break;
            case 'g': playTune("Mario Power-Up", RTTTLTunes::marioPowerUp); break;
            case 'h': playTune("Star Wars Main", RTTTLTunes::starWars); break;
            case 'i': playTune("Test Scale", RTTTLTunes::testScale); break;
            case 'j': playTune("Beethoven 5th Symphony", RTTTLTunes::beethoven5th); break;
            case 'k': playTune("Monty Python", RTTTLTunes::montyPython); break;
            
            // Special loop examples
            case 'L': playTune("Loop Test", RTTTLTunes::testScale, 255); break; // Loop forever
            case 'M': playTune("Loop 3 times", RTTTLTunes::marioPowerUp, 3); break;
            
            // Control functions
            case 's': player.play(RTTTLTunes::success); break;
            case 'r': player.play(RTTTLTunes::error); break;
            case 'x': enterCustomRTTTL(); break;
            case 'v': 
            {
                Serial.print("Current volume: ");
                Serial.println(player.getVolume());
                Serial.print("Enter new volume (0-255): ");
                while (!Serial.available()) delay(50);
                int newVol = Serial.parseInt();
                player.setVolume(newVol);
                Serial.print("Volume set to: ");
                Serial.println(newVol);
            }
            break;
            case 'p':
                Serial.print("Player status: ");
                if (player.isLooping()) {
                    Serial.println("Looping");
                } else if (player.isPlaying()) {
                    Serial.println("Playing");
                } else {
                    Serial.println("Idle");
                }
                break;
            case 'q': 
                player.stop();
                Serial.println("Playback stopped");
                break;
            case 'm':
                printMenu();
                break;
            default:
                Serial.println("Invalid choice! Press 'm' for menu.");
        }
    }
    
    // Do other work while music plays in background
    doOtherWork();
    
    // Small delay to prevent starving other tasks
    delay(10);
}

void playTune(const char* name, const char* tune, uint8_t loopCount) {
    Serial.print("Playing: ");
    Serial.print(name);
    if (loopCount == 0) {
        Serial.println(" (once)");
    } else if (loopCount == 255) {
        Serial.println(" (forever)");
    } else {
        Serial.print(" (loop ");
        Serial.print(loopCount);
        Serial.println(" times)");
    }
    
    if (!player.play(tune, loopCount)) {
        Serial.println("Failed to start playback");
    }
}

void printMenu() {
    Serial.println("\n=== RTTTL Player Menu ===");
    Serial.println("--- Music ---");
    Serial.println(" 1. Nokia Ringtone");
    Serial.println(" 2. Super Mario Bros");
    Serial.println(" 3. James Bond Theme");
    Serial.println(" 4. Pink Panther");
    Serial.println(" 5. Imperial March (Star Wars)");
    Serial.println(" 6. Addams Family");
    Serial.println(" 7. Mission Impossible");
    Serial.println(" 8. Indiana Jones");
    Serial.println(" 9. Tetris Theme");
    Serial.println(" 0. MASH Theme");
    Serial.println(" a. Happy Birthday");
    Serial.println(" b. Canon in D");
    Serial.println(" c. Für Elise");
    Serial.println(" d. Game of Thrones");
    Serial.println(" e. Harry Potter");
    Serial.println(" f. Zelda's Lullaby");
    Serial.println(" g. Mario Power-Up");
    Serial.println(" h. Star Wars Main Theme");
    Serial.println(" i. Test Scale");
    Serial.println(" j. Beethoven 5th Symphony");
    Serial.println(" k. Monty Python");
    
    Serial.println("--- Loop Examples ---");
    Serial.println(" L. Loop Test Scale forever");
    Serial.println(" M. Loop Mario Power-Up 3 times");
    
    Serial.println("--- Player Control ---");
    Serial.println(" s. Play Success Sound");
    Serial.println(" r. Play Error Sound");
    Serial.println(" v. Change Volume");
    Serial.println(" p. Check Player Status");
    Serial.println(" q. Stop Playback");
    Serial.println(" x. Enter Custom RTTTL");
    Serial.println(" m. Show this Menu");
    Serial.println("==============================");
    Serial.println("Note: Loop forever = 255, No loop = 0");
    Serial.print("Select: ");
}

void enterCustomRTTTL() {
    Serial.println("\nEnter RTTTL string (end with newline):");
    Serial.println("Format: Name:d=4,o=5,b=120:c,d,e,f,g,a,b,c6");
    Serial.println("Optional: Add loop count after string");
    Serial.println("Example: Test:d=4,o=5,b=120:c,d,e,f 255 (loop forever)");
    Serial.println("Example: Test:d=4,o=5,b=120:c,d,e,f 3 (loop 3 times)");
    Serial.println("Paste and press Enter...");
    
    // Clear buffer
    while (Serial.available()) Serial.read();
    
    // Wait for input
    unsigned long startTime = millis();
    while (!Serial.available() && (millis() - startTime < 10000)) {
        delay(100);
    }
    
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        
        if (input.length() > 0) {
            // Check if loop count is specified
            uint8_t loopCount = 0;
            int spaceIndex = input.lastIndexOf(' ');
            
            if (spaceIndex > 0) {
                // Try to parse loop count from end
                String loopStr = input.substring(spaceIndex + 1);
                input = input.substring(0, spaceIndex);
                
                if (loopStr == "255") {
                    loopCount = 255;
                } else {
                    loopCount = loopStr.toInt();
                    if (loopCount > 254) loopCount = 0; // Sanitize
                }
            }
            
            Serial.print("Playing: ");
            Serial.print(input);
            if (loopCount == 0) {
                Serial.println(" (once)");
            } else if (loopCount == 255) {
                Serial.println(" (forever)");
            } else {
                Serial.print(" (loop ");
                Serial.print(loopCount);
                Serial.println(" times)");
            }
            
            player.play(input.c_str(), loopCount);
        } else {
            Serial.println("No input received.");
        }
    } else {
        Serial.println("Input timeout.");
    }
}

// Simulate other tasks
void doOtherWork() {
    unsigned long currentTime = millis();
}