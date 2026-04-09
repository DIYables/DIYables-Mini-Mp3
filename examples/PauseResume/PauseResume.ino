/*
 * DIYables Mini Mp3 Player - Pause and Resume
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
 * Demonstrates pausing and resuming playback using a single button.
 * Press the button to toggle between pause and resume.
 *
 * Wiring (Arduino Uno):
 *   Mini Mp3 RX  -> Arduino Pin 11 (via 1K resistor)
 *   Mini Mp3 TX  -> Arduino Pin 10
 *   Mini Mp3 VCC -> 5V
 *   Mini Mp3 GND -> GND
 *   Speaker connected to SPK_1 and SPK_2 pins
 *   Button       -> Pin 2 (other leg to GND)
 *
 * SD Card: Put mp3 files in root, named 001.mp3, 002.mp3, etc.
 */

#include <DIYables_MiniMp3.h>
#include <SoftwareSerial.h>

SoftwareSerial mp3Serial(10, 11); // RX, TX
DIYables_MiniMp3 mp3;

const int BUTTON_PIN = 2;
bool paused = false;

void setup()
{
    Serial.begin(9600);
    mp3Serial.begin(9600);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    mp3.begin(mp3Serial);
    delay(1000);

    mp3.setVolume(20);
    mp3.play(1);

    Serial.println("Playing. Press button to pause/resume.");
}

void loop()
{
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        if (paused)
        {
            mp3.resume();
            Serial.println("Resumed");
        }
        else
        {
            mp3.pause();
            Serial.println("Paused");
        }
        paused = !paused;
        delay(300); // Simple debounce
    }
}
