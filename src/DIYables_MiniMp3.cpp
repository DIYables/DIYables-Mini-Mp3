/*!
 * @file DIYables_MiniMp3.cpp
 *
 * @mainpage DIYables Mini Mp3 Player
 *
 * @section intro_sec Introduction
 *
 * Arduino library for the DIYables Mini Mp3 Player module (YX5200-24SS).
 * Communicates via UART (TX + RX pins) at 9600 baud.
 *
 * These Mp3 players use UART to communicate, 2 pins (TX + RX) are required
 * to interface with the module. An external power supply and output amplifier
 * improves the module's functionality.
 *
 * @section author Author
 *
 * DIYables - diyables.io
 *
 * @section license License
 *
 * MIT
 */

#include "DIYables_MiniMp3.h"


// ============================================================================
// Initialization
// ============================================================================

bool DIYables_MiniMp3::begin(Stream& serial, bool debug, unsigned long timeout, Stream& debugSerial)
{
    _serial      = &serial;
    _debugSerial = &debugSerial;
    _debug       = debug;
    _timeout    = timeout;
    _parseState = WAIT_START;

    _txPacket.startByte = SB;
    _txPacket.version   = VER;
    _txPacket.length    = LEN;
    _txPacket.endByte   = EB;

    _rxPacket.startByte = SB;
    _rxPacket.version   = VER;
    _rxPacket.length    = LEN;
    _rxPacket.endByte   = EB;

    return true;
}


// ============================================================================
// Playback Control
// ============================================================================

void DIYables_MiniMp3::play(uint16_t trackNum)  { sendCommand16(CMD_PLAY, trackNum); }
void DIYables_MiniMp3::playNext()                { sendCommand(CMD_NEXT); }
void DIYables_MiniMp3::playPrevious()            { sendCommand(CMD_PREV); }
void DIYables_MiniMp3::pause()                   { sendCommand(CMD_PAUSE); }
void DIYables_MiniMp3::resume()                  { sendCommand(CMD_RESUME); }
void DIYables_MiniMp3::stop()                    { sendCommand(CMD_STOP); }


// ============================================================================
// Folder Playback
// ============================================================================

void DIYables_MiniMp3::playFolder(uint8_t folder, uint8_t track)
{
    sendCommand(CMD_FOLDER, folder, track);
}

void DIYables_MiniMp3::playLargeFolder(uint8_t folder, uint16_t track)
{
    uint16_t arg = ((uint16_t)folder << 12) | (track & 0x0FFF);
    sendCommand16(CMD_LARGE_FOLDER, arg);
}

void DIYables_MiniMp3::playFromMP3Folder(uint16_t trackNum)
{
    sendCommand16(CMD_MP3_FOLDER, trackNum);
}


// ============================================================================
// Advertisement (interrupt current track)
// ============================================================================

void DIYables_MiniMp3::playAdvertisement(uint16_t trackNum)
{
    sendCommand16(CMD_ADVERT, trackNum);
}

void DIYables_MiniMp3::stopAdvertisement() { sendCommand(CMD_STOP_ADVERT); }


// ============================================================================
// Volume
// ============================================================================

void DIYables_MiniMp3::setVolume(uint8_t vol)
{
    if (vol <= 30)
        sendCommand(CMD_VOLUME, 0, vol);
}

void DIYables_MiniMp3::volumeUp()   { sendCommand(CMD_INC_VOL); }
void DIYables_MiniMp3::volumeDown() { sendCommand(CMD_DEC_VOL); }


// ============================================================================
// Equalizer
// ============================================================================

void DIYables_MiniMp3::setEQ(uint8_t eq)
{
    if (eq <= 5)
        sendCommand(CMD_EQ, 0, eq);
}


// ============================================================================
// Repeat & Shuffle
// ============================================================================

void DIYables_MiniMp3::loopTrack(uint16_t trackNum) { sendCommand16(CMD_LOOP_TRACK, trackNum); }
void DIYables_MiniMp3::loopFolder(uint16_t folder)  { sendCommand16(CMD_LOOP_FOLDER, folder); }
void DIYables_MiniMp3::loopAll()                     { sendCommand(CMD_LOOP_ALL, 0, 1); }
void DIYables_MiniMp3::stopLoop()                    { sendCommand(CMD_LOOP_ALL, 0, 0); }
void DIYables_MiniMp3::shuffle()                     { sendCommand(CMD_SHUFFLE); }


// ============================================================================
// DAC Control
// ============================================================================

void DIYables_MiniMp3::enableDAC()  { sendCommand(CMD_DAC, 0, 0); }
void DIYables_MiniMp3::disableDAC() { sendCommand(CMD_DAC, 0, 1); }


// ============================================================================
// Power Management
// ============================================================================

void DIYables_MiniMp3::sleep()  { sendCommand(CMD_SOURCE, 0, SRC_SLEEP); }
void DIYables_MiniMp3::wakeUp() { sendCommand(CMD_SOURCE, 0, SRC_SD); }
void DIYables_MiniMp3::reset()  { sendCommand(CMD_RESET); }


// ============================================================================
// Status Queries
// ============================================================================

bool DIYables_MiniMp3::isPlaying()
{
    int16_t result = query(QRY_STATUS);
    if (result != -1)
        return (result & 1);
    return false;
}

int16_t DIYables_MiniMp3::getVolume()       { return query(QRY_VOLUME); }
int16_t DIYables_MiniMp3::getEQ()           { return query(QRY_EQ); }
int16_t DIYables_MiniMp3::getTrackCount()   { return query(QRY_SD_FILES); }
int16_t DIYables_MiniMp3::getCurrentTrack() { return query(QRY_SD_TRACK); }
int16_t DIYables_MiniMp3::getFolderCount()  { return query(QRY_FOLDERS); }

int16_t DIYables_MiniMp3::getTrackCountInFolder(uint8_t folder)
{
    return query(QRY_FOLDER_FILES, 0, folder);
}


// ============================================================================
// Configuration
// ============================================================================

void DIYables_MiniMp3::setTimeout(unsigned long ms) { _timeout = ms; }


// ============================================================================
// Internal: Send Command
// ============================================================================

void DIYables_MiniMp3::sendCommand(uint8_t cmd, uint8_t msb, uint8_t lsb)
{
    _txPacket.command  = cmd;
    _txPacket.feedback = 0;
    _txPacket.paramMSB = msb;
    _txPacket.paramLSB = lsb;
    calcChecksum(_txPacket);
    transmit();
}

void DIYables_MiniMp3::sendCommand16(uint8_t cmd, uint16_t param)
{
    sendCommand(cmd, (param >> 8) & 0xFF, param & 0xFF);
}


// ============================================================================
// Internal: Checksum
// ============================================================================

void DIYables_MiniMp3::calcChecksum(Packet& pkt)
{
    int16_t sum = -(pkt.version + pkt.length + pkt.command + pkt.feedback + pkt.paramMSB + pkt.paramLSB);
    pkt.checksumMSB = (sum >> 8) & 0xFF;
    pkt.checksumLSB = sum & 0xFF;
}


// ============================================================================
// Internal: Transmit
// ============================================================================

void DIYables_MiniMp3::transmit()
{
    _serial->write(_txPacket.startByte);
    _serial->write(_txPacket.version);
    _serial->write(_txPacket.length);
    _serial->write(_txPacket.command);
    _serial->write(_txPacket.feedback);
    _serial->write(_txPacket.paramMSB);
    _serial->write(_txPacket.paramLSB);
    _serial->write(_txPacket.checksumMSB);
    _serial->write(_txPacket.checksumLSB);
    _serial->write(_txPacket.endByte);

    if (_debug)
    {
        _debugSerial->print(F("TX: "));
        printPacket(_txPacket);
    }
}


// ============================================================================
// Internal: Flush
// ============================================================================

void DIYables_MiniMp3::flushInput()
{
    while (_serial->available())
        _serial->read();
}


// ============================================================================
// Internal: Query
// ============================================================================

int16_t DIYables_MiniMp3::query(uint8_t cmd, uint8_t msb, uint8_t lsb)
{
    flushInput();

    _txPacket.command  = cmd;
    _txPacket.feedback = 0;
    _txPacket.paramMSB = msb;
    _txPacket.paramLSB = lsb;
    calcChecksum(_txPacket);
    transmit();

    _timeoutStart = millis();

    if (parseResponse())
        if (_rxPacket.command != 0x40)
            return (_rxPacket.paramMSB << 8) | _rxPacket.paramLSB;

    return -1;
}


// ============================================================================
// Internal: Parse Response
// ============================================================================

bool DIYables_MiniMp3::parseResponse()
{
    while (true)
    {
        if (_serial->available())
        {
            uint8_t b = _serial->read();

            debugLogVal(F("RX: 0x"), b);

            switch (_parseState)
            {
                case WAIT_START:
                    if (b == SB)
                    {
                        _rxPacket.startByte = b;
                        _parseState = WAIT_VER;
                    }
                    break;

                case WAIT_VER:
                    if (b != VER) { _parseState = WAIT_START; return false; }
                    _rxPacket.version = b;
                    _parseState = WAIT_LEN;
                    break;

                case WAIT_LEN:
                    if (b != LEN) { _parseState = WAIT_START; return false; }
                    _rxPacket.length = b;
                    _parseState = WAIT_CMD;
                    break;

                case WAIT_CMD:
                    _rxPacket.command = b;
                    _parseState = WAIT_FEEDBACK;
                    break;

                case WAIT_FEEDBACK:
                    _rxPacket.feedback = b;
                    _parseState = WAIT_MSB;
                    break;

                case WAIT_MSB:
                    _rxPacket.paramMSB = b;
                    _parseState = WAIT_LSB;
                    break;

                case WAIT_LSB:
                    _rxPacket.paramLSB = b;
                    _parseState = WAIT_CHECKSUM_MSB;
                    break;

                case WAIT_CHECKSUM_MSB:
                    _rxPacket.checksumMSB = b;
                    _parseState = WAIT_CHECKSUM_LSB;
                    break;

                case WAIT_CHECKSUM_LSB:
                {
                    _rxPacket.checksumLSB = b;
                    int16_t received = (_rxPacket.checksumMSB << 8) | _rxPacket.checksumLSB;
                    calcChecksum(_rxPacket);
                    int16_t calculated = (_rxPacket.checksumMSB << 8) | _rxPacket.checksumLSB;

                    if (received != calculated)
                    {
                        debugLog(F("Checksum error"));
                        _parseState = WAIT_START;
                        return false;
                    }
                    _parseState = WAIT_END;
                    break;
                }

                case WAIT_END:
                    if (b != EB) { _parseState = WAIT_START; return false; }
                    _rxPacket.endByte = b;
                    _parseState = WAIT_START;
                    return true;
            }
        }

        if (millis() - _timeoutStart >= _timeout)
        {
            debugLog(F("Timeout"));
            _parseState = WAIT_START;
            return false;
        }
    }
}


// ============================================================================
// Debug: Print Packet
// ============================================================================

void DIYables_MiniMp3::printPacket(const Packet& pkt)
{
    _debugSerial->print(pkt.startByte, HEX);    _debugSerial->print(' ');
    _debugSerial->print(pkt.version, HEX);      _debugSerial->print(' ');
    _debugSerial->print(pkt.length, HEX);       _debugSerial->print(' ');
    _debugSerial->print(pkt.command, HEX);      _debugSerial->print(' ');
    _debugSerial->print(pkt.feedback, HEX);     _debugSerial->print(' ');
    _debugSerial->print(pkt.paramMSB, HEX);     _debugSerial->print(' ');
    _debugSerial->print(pkt.paramLSB, HEX);     _debugSerial->print(' ');
    _debugSerial->print(pkt.checksumMSB, HEX);  _debugSerial->print(' ');
    _debugSerial->print(pkt.checksumLSB, HEX);  _debugSerial->print(' ');
    _debugSerial->println(pkt.endByte, HEX);
}


// ============================================================================
// Debug: Print Error
// ============================================================================

void DIYables_MiniMp3::printError()
{
    if (_rxPacket.command == 0x40)
    {
        switch (_rxPacket.paramLSB)
        {
            case 0x01: _debugSerial->println(F("Module busy")); break;
            case 0x02: _debugSerial->println(F("Sleep mode")); break;
            case 0x03: _debugSerial->println(F("Serial receive error")); break;
            case 0x04: _debugSerial->println(F("Checksum error")); break;
            case 0x05: _debugSerial->println(F("Track out of scope")); break;
            case 0x06: _debugSerial->println(F("Track not found")); break;
            case 0x07: _debugSerial->println(F("Advertisement error")); break;
            case 0x08: _debugSerial->println(F("SD card error")); break;
            case 0x0A: _debugSerial->println(F("Entered sleep mode")); break;
            default:
                _debugSerial->print(F("Unknown error: 0x"));
                _debugSerial->println(_rxPacket.paramLSB, HEX);
                break;
        }
    }
    else
    {
        _debugSerial->println(F("No error"));
    }
}
