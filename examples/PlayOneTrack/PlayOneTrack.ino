/*
 * DIYables Mini Mp3 Player - Play One Track
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
 * Plays track 001 once, then stops.
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
    delay(1000); // Wait for the module to initialize

    mp3.setVolume(25); // Set volume (0 to 30)

    Serial.println("Playing track 1...");
    mp3.play(1); // Play track 001.mp3
}

void loop()
{
    // Nothing to do here
}
