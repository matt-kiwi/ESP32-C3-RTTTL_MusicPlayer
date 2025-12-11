#ifndef RTTTLTUNES_H
#define RTTTLTUNES_H

/*
 Some links for tunes;
 https://1j01.github.io/rtttl.js/

*/


// Collection of RTTTL ringtones
namespace RTTTLTunes {
    // Classic Nokia tune
    const char nokia[] = "Nokia:d=4,o=5,b=160:8e6,8d6,f#,8e6,8d6,f#,8e6,8d6,f#,8e6,8d6,f#";
    
    // Super Mario Bros theme (short version)
    const char mario[] = "Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,16e6,16c6,16d6,8b";
    
    // James Bond theme
    const char bond[] = "James Bond:d=4,o=5,b=112:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
    
    // Pink Panther theme
    const char pinkPanther[] = "PinkPanther:d=4,o=5,b=160:8d#,8e,8p,8f#,8g,8p,8a,8b,8p,16b,16p,16b,16p,8b,8p,8a,8g,8p,8f#,8e,8p,8d#,8d,8p,8d,8d#,8p,8e,8f,8p,8f#,8g,8p,8a,8b";
    
    // Star Wars Imperial March
    const char imperial[] = "Imperial March:d=4,o=5,b=90:32p,32d,32d,32d,8a#,16f,16d,8a#,16f,16d,8c,8c,8c,8a#,16f#,16d#,8a#,16f#,16d#,8d6,8d6,8d6,8d#6,16a#,16f#,8d#6,16a#,16f#,8g,8g,8g,8d#6,16a#,16f#,8d#6,16a#,16f#";
    
    // Addams Family theme
    const char addams[] = "Addams:d=4,o=5,b=160:8g,8d,8g,8d,8g,8d,8g,8d,8a#,8f,8a#,8f,8c6,8g,8c6,8g,8d6,8a#,8d6,8a#,8d6,8a#,8d6,8a#";
    
    // Mission Impossible theme (short version)
    const char mission[] = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p";
    
    // Indiana Jones theme
    const char indiana[] = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
    
    // Tetris theme (Korobeiniki)
    const char tetris[] = "Tetris:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a";
    
    // Happy Birthday
    const char happyBirthday[] = "Happy Birthday:d=4,o=5,b=120:g,g,a,g,c6,b,g,g,a,g,d6,c6,g,g,g6,e6,c6,b,a,f6,f6,e6,c6,d6,c6";
    
    // Beethoven 5th Symphony 
    const char beethoven5th[] = "Beethoven:d=4,o=6,b=180:8f5,8f5,8f5,1c_5,8p,8d_5,8d_5,8d_5,1c5,8p,8f5,8f5,8f5,8c_5,8f_5,8f_5,8f_5,8f5,8c_,8c_,8c_,2a_5,8p,8f5,8f5,8f5,8c5,8f_5,8f_5,8f_5,8f5,8d_,8d_,8d_,1c,8f,8f,8d_,8c_,8c_5,8c_5,8d_5,8f5,8f,8f,8d_,8c_,8c_5,8c_5,8d_5,8f5,8f,8f,8d_,c_,p,a_5,p,2f";
    
    // Monty Python
    const char montyPython[] = "Monty Python:o=5,d=8,b=180,b=180:d#6,d6,4c6,b,4a#,a,4g#,g,f,g,g#,4g,f,2a#,p,a#,g,p,g,g,f#,g,d#6,p,a#,a#,p,g,g#,p,g#,g#,p,a#,2c6,p,g#,f,p,f,f,e,f,d6,p,c6,c6,p,g#,g,p,g,g,p,g#,2a#,p,a#,g,p,g,g,f#,g,g6,p,d#6,d#6,p,a#,a,p,f6,f6,p,f6,2f6,p,d#6,4d6,f6,f6,e6,f6,4c6,f6,f6,e6,f6,a#,p,a,a#,p,a,2a#";
    
    // Canon in D (simplified)
    const char canon[] = "Canon:d=4,o=5,b=120:8a,8b,8c6,8d6,8e6,8d6,8c6,8b,8a,8g,8f#,8g,8a,8g,8f#,8e";
    
    // Für Elise
    const char furElise[] = "Für Elise:d=4,o=5,b=120:e6,d#6,e6,d#6,e6,b5,d6,c6,a5,p,c5,e5,a5,b5,p,e5,c6,b5,a5";
    
    // Game of Thrones theme
    const char gameOfThrones[] = "GoT:d=4,o=5,b=80:32c6,32d6,32e6,8f6,8g6,16c6,16d6,16e6,8f6,8d6,32c6,32d6,32e6,8f6,8g6,16c6,16d6,16e6,8f6,8d6";
    
    // Harry Potter theme
    const char harryPotter[] = "Harry Potter:d=4,o=5,b=80:32p,8b,8g6,8d6,8b,8g6,8d6,8b,8g6,8d6,8b,8g6";
    
    // Zelda's Lullaby
    const char zelda[] = "Zelda:d=4,o=5,b=120:a,f#,a,f#,a,d6,b5,d6,b5,d6,e6,f#6,e6,f#6,e6,c#6,a,a,f#,a,f#,a";
    
    // Super Mario Bros power-up
    const char marioPowerUp[] = "Mario PowerUp:d=4,o=5,b=100:16e6,16e6,16p,16e6,16p,16c6,16e6,16p,16g6,16p,16p,16p,16g";
    
    // Star Wars main theme
    const char starWars[] = "StarWars:d=4,o=5,b=45:32p,32f6,32f6,32f6,8b.6,16f6,32f6,32f6,32f6,8b.6,16f6,32f6,32f6,32f6,8b.6,16f6,32f6,32f6,32f6,8c7,8b.6";
    
    // Mash
    const char mash[] = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";


    // Test scale (for debugging)
    const char testScale[] = "Scale:d=4,o=5,b=120:c,d,e,f,g,a,b,c6";
    
    // Simple startup beep
    const char startup[] = "Startup:d=4,o=5,b=120:8c6,8e6,8g6";
    
    // Error sound
    const char error[] = "Error:d=4,o=5,b=120:8c5,8a4";
    
    // Success sound
    const char success[] = "Success:d=4,o=5,b=120:8c6,8e6,8g6,8c7";
    
    // Alert/notification
    const char alert[] = "Alert:d=4,o=5,b=200:8c6,16p,8c6";
}

#endif // RTTTLTUNES_H