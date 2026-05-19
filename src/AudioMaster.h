#ifndef AUDIO_MASTER_H
#define AUDIO_MASTER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_VS1053.h>

class AudioMaster
{
private:
    struct Impl;
    Impl *_impl;

    int _vs1053Reset;
    int _vs1053Cs;
    int _vs1053Dcs;
    int _dreqPin;
    int _cardCs;
    int _station;

    int _targetAddress;
    int _lightPin;

    int _lastReceivedValue;
    bool _initialized;
    String _lastError;

public:
    AudioMaster(
        int lightPin,
        int vs1053Reset = 9,
        int vs1053Cs = 7,
        int vs1053Dcs = 6,
        int dreqPin = 3,
        int cardCs = 4);

    ~AudioMaster();

    bool begin();
    void update();
    void playAudioFile(const char *filename);
    void stopAudio();
    void setVolume(uint8_t left, uint8_t right);
    void setLight(bool on);

    int getLastReceivedValue() const;
    bool isInitialized() const;
    String getLastError() const;
};

#endif