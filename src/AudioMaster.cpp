#include "AudioMaster.h"
#include "lvgl.h"

static void logAudioStage(const char *stage)
{
    Serial.println(String("[AUDIO] ") + stage);
}

struct AudioMaster::Impl
{

    Adafruit_VS1053_FilePlayer musicPlayer;

    Impl(int reset, int cs, int dcs, int dreq, int cardCs)
        : musicPlayer(reset, cs, dcs, dreq, cardCs)
    {
    }
};

AudioMaster::AudioMaster(
    int lightPin,
    int vs1053Reset,
    int vs1053Cs,
    int vs1053Dcs,
    int dreqPin,
    int cardCs)
    : _impl(new Impl(vs1053Reset, vs1053Cs, vs1053Dcs, dreqPin, cardCs)),
      _vs1053Reset(vs1053Reset),
      _vs1053Cs(vs1053Cs),
      _vs1053Dcs(vs1053Dcs),
      _dreqPin(dreqPin),
      _cardCs(cardCs),
      _targetAddress(9),
      _lightPin(lightPin),
      _lastReceivedValue(0),
      _initialized(false),
      _lastError("")
{
}

AudioMaster::~AudioMaster()
{
    delete _impl;
    _impl = nullptr;
}

bool AudioMaster::begin()
{
    logAudioStage("begin start");
    _initialized = false;
    _lastError = "";

    logAudioStage("SPI prime start");
    SPI.begin();
    pinMode(_cardCs, OUTPUT);
    digitalWrite(_cardCs, HIGH);
    delay(2);
    logAudioStage("SPI prime complete");

    Serial.println(String("[AUDIO] pin map rst=") + _vs1053Reset +
                   " cs=" + _vs1053Cs +
                   " dcs=" + _vs1053Dcs +
                   " dreq=" + _dreqPin +
                   " sd_cs=" + _cardCs);

    if (_vs1053Cs == _vs1053Dcs ||
        _vs1053Cs == _dreqPin ||
        _vs1053Dcs == _dreqPin)
    {
        _lastError = "Invalid VS1053 pin map: CS/DCS/DREQ pins must be unique";
        Serial.println(String("[AUDIO] ") + _lastError);
        return false;
    }

    if (_cardCs == _dreqPin || _cardCs == _vs1053Cs || _cardCs == _vs1053Dcs)
    {
        _lastError = "Invalid SPI pin map: SD card CS must not overlap VS1053 CS/DCS/DREQ";
        Serial.println(String("[AUDIO] ") + _lastError);
        return false;
    }

    logAudioStage("VS1053 begin start");
    if (!_impl->musicPlayer.begin())
    {
        if (_vs1053Reset >= 0)
        {
            logAudioStage("VS1053 begin retry with reset disabled");
            delete _impl;
            _impl = new Impl(-1, _vs1053Cs, _vs1053Dcs, _dreqPin, _cardCs);
            if (_impl->musicPlayer.begin())
            {
                _vs1053Reset = -1;
                logAudioStage("VS1053 begin complete (retry without reset)");
            }
            else
            {
                _lastError = "VS1053 init failed (check CS/DCS/DREQ/RESET and ensure SD CS is HIGH during probe)";
                Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
                return false;
            }
        }
        else
        {
            _lastError = "VS1053 init failed (check CS/DCS/DREQ/RESET and ensure SD CS is HIGH during probe)";
            Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
            return false;
        }
    }
    logAudioStage("VS1053 begin complete");

    logAudioStage("SD begin start");
    if (!SD.begin(_cardCs))
    {
        _lastError = String("SD init failed on CS pin ") + _cardCs;
        Serial.println(F("SD failed, or not present"));
        return false;
    }
    logAudioStage("SD begin complete");

    if (_targetAddress < 0x08 || _targetAddress > 0x77)
    {
        _lastError = String("Invalid I2C target address: ") + _targetAddress;
        Serial.println(F("Invalid I2C target address (must be 0x08-0x77)."));
        return false;
    }

    _impl->musicPlayer.setVolume(30, 30);
    // Keep playback in polling mode for stability on Giga.

    logAudioStage("Wire.begin start");
    Wire.begin();
    logAudioStage("Wire.begin complete");

    pinMode(_lightPin, OUTPUT);
    digitalWrite(_lightPin, HIGH);

    _initialized = true;
    logAudioStage("begin complete");

    return true;
}

void AudioMaster::update()
{
    if (!_initialized)
    {
        return;
    }

    logAudioStage("update beginTransmission");
    Wire.beginTransmission(_targetAddress);
    Wire.write(static_cast<uint8_t>(static_cast<int8_t>(-1)));
    Wire.endTransmission();

    logAudioStage("update requestFrom");
    Wire.requestFrom(static_cast<int>(_targetAddress), 1);
    while (Wire.available())
    {
        _lastReceivedValue = static_cast<int8_t>(Wire.read());
        Serial.print("Received from Extracting: ");
        Serial.println(_lastReceivedValue);
    }

    switch (_lastReceivedValue)
    {
    case 0:
        Serial.println("At pressure");
        break;
    case 1:
        Serial.println("Over-pressure");
        break;
    case -1:
        Serial.println("Under-pressure");
        Serial.println("Triggering audio playback...");

        setLight(true);
        playAudioFile("medscan2.wav");
        setLight(false);

        break;
    default:
        break;
    }

    if (_impl->musicPlayer.stopped())
    {
        Serial.println("Done playing music");
    }

    delay(100);
}

int AudioMaster::getLastReceivedValue() const
{
    return _lastReceivedValue;
}

bool AudioMaster::isInitialized() const
{
    return _initialized;
}

String AudioMaster::getLastError() const
{
    return _lastError;
}

void AudioMaster::playAudioFile(const char *filename)
{
    if (_initialized)
    {
        _impl->musicPlayer.playFullFile(filename);
    }
}

void AudioMaster::stopAudio()
{
    if (_initialized)
    {
        _impl->musicPlayer.stopPlaying();
    }
}

void AudioMaster::setVolume(uint8_t left, uint8_t right)
{
    if (_initialized)
    {
        _impl->musicPlayer.setVolume(left, right);
    }
}

void AudioMaster::setLight(bool on)
{
    if (_initialized)
    {
        digitalWrite(_lightPin, on ? LOW : HIGH);
    }
}