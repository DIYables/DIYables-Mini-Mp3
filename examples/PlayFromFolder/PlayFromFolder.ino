/*
 * DIYables Mini Mp3 Player - Play from Folder
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
 * Plays tracks from specific folders on the SD card.
 *
 * Wiring (Arduino Uno):
 *   Mini Mp3 RX  -> Arduino Pin 11 (via 1K resistor)
 *   Mini Mp3 TX  -> Arduino Pin 10
 *   Mini Mp3 VCC -> 5V
 *   Mini Mp3 GND -> GND
 *   Speaker connected to SPK_1 and SPK_2 pins
 *
 * SD Card file structure:
 *   /01/001.mp3    <- playFolder(1, 1)
 *   /01/002.mp3    <- playFolder(1, 2)
 *   /02/001.mp3    <- playFolder(2, 1)
 *   /02/002.mp3    <- playFolder(2, 2)
 *
 * IMPORTANT:
 *   - Numbering starts from 1, NOT 0
 *   - Folder names must be 2-digit zero-padded (01-99)
 *   - Track names must be 3-digit zero-padded (001-255)
 *   - Format SD card as FAT32, then copy files one by one in order
 *   - Track order is determined by the order files were copied,
 *     NOT by filename. So copy them in the correct sequence.
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

    // Play track 1 from folder 01
    Serial.println("Playing folder 01, track 001...");
    mp3.playFolder(1, 1);
    delay(5000);

    // Play track 2 from folder 01
    Serial.println("Playing folder 01, track 002...");
    mp3.playFolder(1, 2);
    delay(5000);

    // Play track 1 from folder 02
    Serial.println("Playing folder 02, track 001...");
    mp3.playFolder(2, 1);
}

void loop()
{
    // Nothing to do here
}
