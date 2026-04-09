/*
 * DIYables Mini Mp3 Player - Serial Command Control
 *
 * Product: DIYables Mini Mp3 Player Module
 *   https://diyables.io/products/mini-mp3-player-module
 *
 * Tested with:
 *   - Arduino Uno R3
 *   - Arduino Uno R4 WiFi / R4 Minima
 *   - Arduino Mega
 *   - Arduino Due
 *   - Arduino Giga
 *   - DIYables STEM V3          https://diyables.io/stem-v3
 *   - DIYables STEM V4 IoT      https://diyables.io/stem-v4-iot
 *   - DIYables STEM V4B IoT     https://diyables.io/stem-v4b-iot
 *   - DIYables STEM V4 Edu      https://diyables.io/stem-v4-edu
 *   - DIYables MEGA2560 R3      https://diyables.io/atmega2560-board
 *   - DIYables Nano R3           https://diyables.io/nano-board
 *   - DIYables ESP32 Board       https://diyables.io/esp32-board
 *   - DIYables ESP32 S3 (Uno)    https://diyables.io/esp32-s3-uno
 *   - Expected to work with other Arduino-compatible boards
 *
 * Control the Mp3 player by typing commands in the Serial Monitor.
 * Great for testing all functions without extra hardware.
 *
 * Commands (type in Serial Monitor, then press Enter):
 *   1-9     Play track 1 to 9
 *   +       Volume up
 *   -       Volume down
 *   p       Pause
 *   r       Resume
 *   s       Stop
 *   n       Next track
 *   b       Previous track (back)
 *   ?       Show current status
 *
 * Wiring (Arduino Uno):
 *   Mini Mp3 RX  -> Arduino Pin 11 (via 1K resistor)
 *   Mini Mp3 TX  -> Arduino Pin 10
 *   Mini Mp3 VCC -> 5V
 *   Mini Mp3 GND -> GND
 *   Speaker connected to SPK_1 and SPK_2 pins
 *
 * SD Card: Put mp3 files in root, named 001.mp3, 002.mp3, etc.
 */

#include <DIYables_MiniMp3.h>
#include <SoftwareSerial.h>

SoftwareSerial mp3Serial(10, 11); // RX, TX
DIYables_MiniMp3 mp3;

void setup()
{
    Serial.begin(9600);
    mp3Serial.begin(9600);

    mp3.begin(mp3Serial);
    delay(1000);

    mp3.setVolume(20);

    Serial.println("=== DIYables Mini Mp3 Player ===");
    Serial.println("Commands:");
    Serial.println("  1-9  Play track number");
    Serial.println("  +    Volume up");
    Serial.println("  -    Volume down");
    Serial.println("  p    Pause");
    Serial.println("  r    Resume");
    Serial.println("  s    Stop");
    Serial.println("  n    Next track");
    Serial.println("  b    Previous track");
    Serial.println("  ?    Show status");
    Serial.println("================================");
}

void loop()
{
    if (Serial.available())
    {
        char cmd = Serial.read();

        switch (cmd)
        {
            case '1': case '2': case '3':
            case '4': case '5': case '6':
            case '7': case '8': case '9':
                Serial.print("Playing track ");
                Serial.println(cmd - '0');
                mp3.play(cmd - '0');
                break;

            case '+':
                Serial.println("Volume up");
                mp3.volumeUp();
                break;

            case '-':
                Serial.println("Volume down");
                mp3.volumeDown();
                break;

            case 'p':
                Serial.println("Paused");
                mp3.pause();
                break;

            case 'r':
                Serial.println("Resumed");
                mp3.resume();
                break;

            case 's':
                Serial.println("Stopped");
                mp3.stop();
                break;

            case 'n':
                Serial.println("Next track");
                mp3.playNext();
                break;

            case 'b':
                Serial.println("Previous track");
                mp3.playPrevious();
                break;

            case '?':
            {
                Serial.println("--- Status ---");

                int16_t vol = mp3.getVolume();
                Serial.print("Volume: ");
                Serial.println(vol);

                int16_t track = mp3.getCurrentTrack();
                Serial.print("Current track: ");
                Serial.println(track);

                bool playing = mp3.isPlaying();
                Serial.print("Playing: ");
                Serial.println(playing ? "Yes" : "No");

                int16_t total = mp3.getTrackCount();
                Serial.print("Total tracks: ");
                Serial.println(total);

                Serial.println("--------------");
                break;
            }

            default:
                break;
        }
    }
}
