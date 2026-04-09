/*!
 * @file DIYables_MiniMp3.h
 *
 * Arduino library for the DIYables Mini Mp3 Player module (YX5200-24SS).
 * Communicates via UART (TX + RX pins) at 9600 baud.
 *
 * Product page: https://diyables.io/
 */

#pragma once
#include "Arduino.h"

class DIYables_MiniMp3 {
public:
    /** EQ Settings */
    static const uint8_t EQ_NORMAL  = 0;
    static const uint8_t EQ_POP     = 1;
    static const uint8_t EQ_ROCK    = 2;
    static const uint8_t EQ_JAZZ    = 3;
    static const uint8_t EQ_CLASSIC = 4;
    static const uint8_t EQ_BASS    = 5;

    /**
     * Initialize the Mini Mp3 player.
     * @param serial      Reference to a Serial stream (hardware or software).
     * @param debug       Enable debug output.
     * @param timeout     Response timeout in milliseconds.
     * @param debugSerial Stream for debug output (default: Serial).
     * @return true on success.
     */
    bool begin(Stream& serial, bool debug = false, unsigned long timeout = 100, Stream& debugSerial = Serial);

    // ---- Playback Control ----
    void play(uint16_t trackNum);
    void playNext();
    void playPrevious();
    void pause();
    void resume();
    void stop();

    // ---- Folder Playback ----
    void playFolder(uint8_t folder, uint8_t track);
    void playLargeFolder(uint8_t folder, uint16_t track);
    void playFromMP3Folder(uint16_t trackNum);

    // ---- Advertisement (interrupt current track) ----
    void playAdvertisement(uint16_t trackNum);
    void stopAdvertisement();

    // ---- Volume (0-30) ----
    void setVolume(uint8_t vol);
    void volumeUp();
    void volumeDown();

    // ---- Equalizer ----
    void setEQ(uint8_t eq);

    // ---- Repeat & Shuffle ----
    void loopTrack(uint16_t trackNum);
    void loopFolder(uint16_t folder);
    void loopAll();
    void stopLoop();
    void shuffle();

    // ---- DAC Control ----
    void enableDAC();
    void disableDAC();

    // ---- Power Management ----
    void sleep();
    void wakeUp();
    void reset();

    // ---- Status Queries ----
    bool isPlaying();
    int16_t getVolume();
    int16_t getEQ();
    int16_t getTrackCount();
    int16_t getCurrentTrack();
    int16_t getFolderCount();
    int16_t getTrackCountInFolder(uint8_t folder);

    // ---- Configuration ----
    void setTimeout(unsigned long ms);

    // ---- Debug ----
    void printError();

private:
    // Protocol constants
    static const uint8_t SB  = 0x7E;
    static const uint8_t VER = 0xFF;
    static const uint8_t LEN = 0x06;
    static const uint8_t EB  = 0xEF;

    // Command codes
    static const uint8_t CMD_NEXT         = 0x01;
    static const uint8_t CMD_PREV         = 0x02;
    static const uint8_t CMD_PLAY         = 0x03;
    static const uint8_t CMD_INC_VOL      = 0x04;
    static const uint8_t CMD_DEC_VOL      = 0x05;
    static const uint8_t CMD_VOLUME       = 0x06;
    static const uint8_t CMD_EQ           = 0x07;
    static const uint8_t CMD_LOOP_TRACK   = 0x08;
    static const uint8_t CMD_SOURCE       = 0x09;
    static const uint8_t CMD_STANDBY      = 0x0A;
    static const uint8_t CMD_NORMAL       = 0x0B;
    static const uint8_t CMD_RESET        = 0x0C;
    static const uint8_t CMD_RESUME       = 0x0D;
    static const uint8_t CMD_PAUSE        = 0x0E;
    static const uint8_t CMD_FOLDER       = 0x0F;
    static const uint8_t CMD_VOL_ADJ      = 0x10;
    static const uint8_t CMD_LOOP_ALL     = 0x11;
    static const uint8_t CMD_MP3_FOLDER   = 0x12;
    static const uint8_t CMD_ADVERT       = 0x13;
    static const uint8_t CMD_LARGE_FOLDER = 0x14;
    static const uint8_t CMD_STOP_ADVERT  = 0x15;
    static const uint8_t CMD_STOP         = 0x16;
    static const uint8_t CMD_LOOP_FOLDER  = 0x17;
    static const uint8_t CMD_SHUFFLE      = 0x18;
    static const uint8_t CMD_REPEAT       = 0x19;
    static const uint8_t CMD_DAC          = 0x1A;

    // Query codes
    static const uint8_t QRY_STATUS       = 0x42;
    static const uint8_t QRY_VOLUME       = 0x43;
    static const uint8_t QRY_EQ           = 0x44;
    static const uint8_t QRY_SD_FILES     = 0x47;
    static const uint8_t QRY_SD_TRACK     = 0x4B;
    static const uint8_t QRY_FOLDER_FILES = 0x4E;
    static const uint8_t QRY_FOLDERS      = 0x4F;

    // Source constants
    static const uint8_t SRC_SD    = 2;
    static const uint8_t SRC_SLEEP = 4;

    struct Packet {
        uint8_t startByte;
        uint8_t version;
        uint8_t length;
        uint8_t command;
        uint8_t feedback;
        uint8_t paramMSB;
        uint8_t paramLSB;
        uint8_t checksumMSB;
        uint8_t checksumLSB;
        uint8_t endByte;
    };

    Stream* _serial;
    Stream* _debugSerial;
    bool    _debug;
    unsigned long _timeout;
    unsigned long _timeoutStart;
    Packet  _txPacket;
    Packet  _rxPacket;

    enum ParseState {
        WAIT_START, WAIT_VER, WAIT_LEN, WAIT_CMD,
        WAIT_FEEDBACK, WAIT_MSB, WAIT_LSB,
        WAIT_CHECKSUM_MSB, WAIT_CHECKSUM_LSB, WAIT_END
    };
    ParseState _parseState;

    void sendCommand(uint8_t cmd, uint8_t msb = 0, uint8_t lsb = 0);
    void sendCommand16(uint8_t cmd, uint16_t param);
    void calcChecksum(Packet& pkt);
    void transmit();
    void flushInput();
    int16_t query(uint8_t cmd, uint8_t msb = 0, uint8_t lsb = 0);
    bool parseResponse();
    void printPacket(const Packet& pkt);

    inline void debugLog(const __FlashStringHelper* msg) { if (_debug) _debugSerial->println(msg); }
    inline void debugLogVal(const __FlashStringHelper* prefix, uint8_t val) {
        if (_debug) { _debugSerial->print(prefix); _debugSerial->println(val, HEX); }
    }
};
