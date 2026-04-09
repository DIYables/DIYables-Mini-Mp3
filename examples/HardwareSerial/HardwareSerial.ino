/*
 * DIYables Mini Mp3 Player - Hardware Serial
 *
 * Uses a hardware serial port instead of SoftwareSerial.
 * Works on boards with extra hardware serial ports:
 *   - Arduino Mega: Serial1, Serial2, Serial3
 *   - ESP32: Serial1, Serial2
 *   - Arduino Due: Serial1, Serial2, Serial3
 *   - ESP8266: Serial1 (TX only) — use swap() for full UART
 *
 * Wiring:
 *   Mini Mp3 RX  -> Board's TX pin of the chosen serial port
 *   Mini Mp3 TX  -> Board's RX pin of the chosen serial port
 *   Mini Mp3 VCC -> 5V (or 3.3V for 3.3V boards)
 *   Mini Mp3 GND -> GND
 *   Speaker connected to SPK_1 and SPK_2 pins
 *
 * Note: Use a 1K resistor on Mini Mp3 RX when using 5V boards.
 *       No resistor needed for 3.3V boards (ESP32, Due, etc.).
 *
 * SD Card: Put mp3 files in root, named 001.mp3, 002.mp3, etc.
 */

#include <DIYables_MiniMp3.h>

DIYables_MiniMp3 mp3;

void setup()
{
    Serial.begin(115200);
    Serial2.begin(9600); // Change to Serial1 or Serial3 depending on your board

    mp3.begin(Serial2);
    delay(1000);

    mp3.setVolume(20);

    Serial.println("Playing track 1...");
    mp3.play(1);
}

void loop()
{
    // Your code here
}
