# DIYables Mini Mp3 Player Library

Arduino library for the DIYables Mini Mp3 Player module (YX5200-24SS). Easy-to-use API designed for beginners.

![DIYables Mini Mp3 Player Module](https://diyables.io/images/products/mini-mp3-player-2.jpg)

## PRODUCT

[DIYables Mini Mp3 Player Module](https://diyables.io/products/mini-mp3-player-module)

## Tested Hardware

| Board                   | Tested |
|-------------------------|:------:|
| Arduino Uno R3          |   ✅   |
| Arduino Uno R4 WiFi     |   ✅   |
| Arduino Uno R4 Minima   |   ✅   |
| Arduino Mega            |   ✅   |
| Arduino Due             |   ✅    |
| Arduino Giga            |   ✅    |
| [DIYables STEM V4 IoT *(works like Arduino Uno R4 WiFi)*](https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi) |   ✅   |
| [DIYables STEM V4B IoT *(works like Arduino Uno R4 WiFi)*](https://diyables.io/products/diyables-stem-v4b-iot-development-board-compatible-with-arduino-uno-r4-wifi-ra4m1-32-bit-arm-cortex-m4-with-esp32-s3-wifi-bluetooth-usb-c-for-learning-prototyping-education) |   ✅   |
| [DIYables STEM V3 *(works like Arduino Uno R3)*](https://diyables.io/products/diyables-stem-v3-fully-compatible-with-arduino-uno-r3) |   ✅   |
| [DIYables STEM V4 Edu *(works like Arduino Uno R4 Minima)*](https://diyables.io/stem-v4-edu) |   ✅   |
| [DIYables MEGA2560 R3 *(works like Arduino Mega 2560 Rev3)*](https://diyables.io/atmega2560-board) |   ✅   |
| [DIYables Nano R3 *(works like Arduino Nano R3)*](https://diyables.io/nano-board) |   ✅   |
| [DIYables ESP32 Development Board](https://diyables.io/esp32-board) |   ✅   |
| [DIYables ESP32 S3, Uno-form factor](https://diyables.io/products/esp32-s3-development-board-with-esp32-s3-wroom-1-n16r8-wifi-bluetooth-uno-compatible-form-factor-works-with-arduino-ide) |   ✅   |
| Other boards            |   Not yet, expected to work    |



## Features
- Simple, beginner-friendly API — no complex setup needed
- No external library dependencies
- Non-blocking playback commands (play, pause, stop, volume, etc.)
- Play tracks by number, from folders, or from the special `/mp3` folder
- Volume control (0-30), 6 EQ modes
- Loop track, loop folder, loop all, shuffle
- Advertisement support (interrupt and resume)
- DAC control, sleep/wake, reset
- Status queries (current track, volume, track count, etc.)
- Configurable debug output to any serial port
- Works with both hardware and software serial
- Compatible with all Arduino boards (Uno, Mega, ESP32, ESP8266, Due, STM32, RP2040, etc.)

## Tutorials
- [Arduino - Mini Mp3 Player Module](https://arduinogetstarted.com/tutorials/arduino-mini-mp3-player-module)
- [Arduino Uno R4 - Mini Mp3 Player Module](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-mini-mp3-player-module)
- [Arduino Mega - Mini Mp3 Player Module](https://newbiely.com/tutorials/arduino-mega/arduino-mega-mini-mp3-player-module)
- [Arduino Nano - Mini Mp3 Player Module](https://newbiely.com/tutorials/arduino-nano/arduino-nano-mini-mp3-player-module)
- [Arduino Nano ESP32 - Mini Mp3 Player Module](https://newbiely.com/tutorials/arduino-nano-esp32/arduino-nano-esp32-mini-mp3-player-module)
- [Arduino Nano 33 IoT - Mini Mp3 Player Module](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-mini-mp3-player-module)
- [Arduino MKR WiFi 1010 - Mini Mp3 Player Module](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-mini-mp3-player-module)
- [Arduino Giga R1 WiFi - Mini Mp3 Player Module](https://newbiely.com/tutorials/arduino-giga/arduino-giga-r1-wifi-mini-mp3-player-module)
- [ESP8266 - Mini Mp3 Player Module](https://newbiely.com/tutorials/esp8266/esp8266-mini-mp3-player-module)
- [ESP32 - Mini Mp3 Player Module](https://esp32io.com/tutorials/esp32-mini-mp3-player-module)

## Wiring

### DIYables Mini Mp3 Player Pinout

| Pin | Description |
|---|---|
| VCC | Power supply (3.2V ~ 5.0V) |
| GND | Ground |
| RX | UART receive (connect to Arduino TX via 1K resistor) |
| TX | UART transmit (connect to Arduino RX) |
| SPK_1 | Speaker+ (for direct speaker connection, max 3W) |
| SPK_2 | Speaker- (for direct speaker connection, max 3W) |
| DAC_R | Right audio output (for external amplifier) |
| DAC_L | Left audio output (for external amplifier) |
| BUSY | Low when playing, high when idle (optional) |
| IO_1 | Short press: previous track, long press: volume down |
| IO_2 | Short press: next track, long press: volume up |

### Wiring with Arduino Uno (Software Serial)

| Mini Mp3 Pin | Arduino Pin | Note |
|---|---|---|
| VCC | 5V | |
| GND | GND | |
| RX | Pin 11 | **Must use a 1K resistor in series** (module is 3.3V logic) |
| TX | Pin 10 | |
| SPK_1 | Speaker+ | Direct speaker connection (max 3W) |
| SPK_2 | Speaker- | Direct speaker connection (max 3W) |

### Wiring with Arduino Mega / ESP32 (Hardware Serial)

| Mini Mp3 Pin | Arduino Pin | Note |
|---|---|---|
| VCC | 5V (or 3.3V for ESP32) | |
| GND | GND | |
| RX | TX1 (Pin 18 on Mega) | **Use a 1K resistor for 5V boards** |
| TX | RX1 (Pin 19 on Mega) | |
| SPK_1 | Speaker+ | |
| SPK_2 | Speaker- | |

> **Important:** The 1K resistor on the RX pin is required when using 5V Arduino boards. It protects the module's 3.3V logic input. Without it, the module may not work or could be damaged.

> **Tip:** For better audio quality, connect DAC_R and DAC_L to an external amplifier instead of using the direct speaker output.

## SD Card File Structure

> **Important:** Format your SD card as **FAT16** or **FAT32** before use.

### Track Numbering

- Track numbers start from **1** (not 0).
- The module assigns track numbers based on the **order files were copied** to the SD card, **NOT by filename**.
- To ensure correct ordering: **format the SD card first**, then copy files **one by one in order** (001.mp3 first, then 002.mp3, etc.).
- File names must start with a zero-padded number. You can add text after the number: `001-hello.mp3`, `002-goodbye.mp3`.

Root folder files (used by `play()`):
```
/001.mp3          <- play(1)
/002.mp3          <- play(2)
/003-mysong.mp3   <- play(3)
```

### Folder Numbering

- Folder numbers start from **1** (not 0).
- Folder names must be **2-digit** zero-padded numbers: `01`, `02`, ... `99`.
- Track names inside folders must be **3-digit** zero-padded numbers: `001`, `002`, ... `255`.
- Maximum: **99 folders**, **255 tracks per folder** (using `playFolder()`).

```
/01/001.mp3       <- playFolder(1, 1)
/01/002.mp3       <- playFolder(1, 2)
/02/001.mp3       <- playFolder(2, 1)
/02/002.mp3       <- playFolder(2, 2)
```

### Large Folder Mode

- For more than 255 tracks per folder, use `playLargeFolder()`.
- Folder names: `01` to `15` (max **15 folders**).
- Track names must be **4-digit** zero-padded numbers: `0001` to `3000`.

```
/01/0001.mp3      <- playLargeFolder(1, 1)
/01/0002.mp3      <- playLargeFolder(1, 2)
/15/3000.mp3      <- playLargeFolder(15, 3000)
```

### MP3 Folder

- The folder must be named `mp3` (lowercase) in the root of the SD card.
- Track names must be **4-digit** zero-padded: `0001.mp3` to `9999.mp3`.

```
/mp3/0001.mp3     <- playFromMP3Folder(1)
/mp3/0002.mp3     <- playFromMP3Folder(2)
```

### Advertisement Folder

- The folder must be named `advert` (lowercase) in the root.
- Track names must be **4-digit** zero-padded: `0001.mp3` to `9999.mp3`.
- Used to interrupt the current track (e.g., announcements).

```
/advert/0001.mp3  <- playAdvertisement(1)
/advert/0002.mp3  <- playAdvertisement(2)
```

### Summary Table

| Function | Folder Name | File Name Format | Range |
|---|---|---|---|
| `play(n)` | Root `/` | `001.mp3` - `255.mp3` | 1 - 9999 |
| `playFolder(f, t)` | `01` - `99` | `001.mp3` - `255.mp3` | folder 1-99, track 1-255 |
| `playLargeFolder(f, t)` | `01` - `15` | `0001.mp3` - `3000.mp3` | folder 1-15, track 1-3000 |
| `playFromMP3Folder(n)` | `mp3` | `0001.mp3` - `9999.mp3` | 1 - 9999 |
| `playAdvertisement(n)` | `advert` | `0001.mp3` - `9999.mp3` | 1 - 9999 |

## Examples

| Example | Description |
|---|---|
| [PlayOneTrack](examples/PlayOneTrack) | Simplest example — play a single track |
| [LoopTrack](examples/LoopTrack) | Loop (repeat) a track continuously with EQ setting |
| [PlayMultipleTracks](examples/PlayMultipleTracks) | Play tracks one after another automatically |
| [PlayFromFolder](examples/PlayFromFolder) | Play tracks from numbered folders |
| [PauseResume](examples/PauseResume) | Toggle pause/resume with a button |
| [NextPrevButtons](examples/NextPrevButtons) | Next/previous track with two buttons |
| [VolumeControl](examples/VolumeControl) | Volume up/down with two buttons |
| [SerialControl](examples/SerialControl) | Control everything from the Serial Monitor (great for testing) |
| [HardwareSerial](examples/HardwareSerial) | Use with hardware serial (Mega, ESP32, Due, etc.) |

## Quick Start

```cpp
#include <DIYables_MiniMp3.h>
#include <SoftwareSerial.h>

SoftwareSerial mp3Serial(10, 11); // RX, TX
DIYables_MiniMp3 mp3;

void setup() {
    mp3Serial.begin(9600);
    mp3.begin(mp3Serial);
    delay(1000);

    mp3.setVolume(25);
    mp3.play(1);
}

void loop() {
}
```

For the full API with detailed parameters, return values, and examples, see the [Library Reference](https://newbiely.com/library-references/diyables-mini-mp3-player-library-reference).
