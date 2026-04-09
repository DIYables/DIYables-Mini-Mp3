/*
 * DIYables Mini Mp3 Player - Volume Control
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
 * Use two buttons to increase/decrease the volume.
 * Press button on pin 2 to volume up, pin 3 to volume down.
 *
 * Wiring (Arduino Uno):
 *   Mini Mp3 RX  -> Arduino Pin 11 (via 1K resistor)
 *   Mini Mp3 TX  -> Arduino Pin 10
 *   Mini Mp3 VCC -> 5V
 *   Mini Mp3 GND -> GND
 *   Speaker connected to SPK_1 and SPK_2 pins
 *   Button UP    -> Pin 2 (other leg to GND)
 *   Button DOWN  -> Pin 3 (other leg to GND)
 *
 * SD Card: Put mp3 files in root, named 001.mp3, 002.mp3, etc.
 */

#include <DIYables_MiniMp3.h>
#include <SoftwareSerial.h>

SoftwareSerial mp3Serial(10, 11); // RX, TX
DIYables_MiniMp3 mp3;

const int BUTTON_VOL_UP   = 2;
const int BUTTON_VOL_DOWN = 3;

int volume = 15; // Start at half volume

void setup()
{
    Serial.begin(9600);
    mp3Serial.begin(9600);

    pinMode(BUTTON_VOL_UP,   INPUT_PULLUP);
    pinMode(BUTTON_VOL_DOWN, INPUT_PULLUP);

    mp3.begin(mp3Serial);
    delay(1000);

    mp3.setVolume(volume);
    mp3.loopTrack(1); // Play track 1 on repeat

    Serial.print("Volume: ");
    Serial.println(volume);
}

void loop()
{
    // Volume Up button (pressed = LOW because of INPUT_PULLUP)
    if (digitalRead(BUTTON_VOL_UP) == LOW)
    {
        if (volume < 30)
        {
            volume++;
            mp3.setVolume(volume);
            Serial.print("Volume: ");
            Serial.println(volume);
        }
        delay(200); // Simple debounce
    }

    // Volume Down button
    if (digitalRead(BUTTON_VOL_DOWN) == LOW)
    {
        if (volume > 0)
        {
            volume--;
            mp3.setVolume(volume);
            Serial.print("Volume: ");
            Serial.println(volume);
        }
        delay(200); // Simple debounce
    }
}
