#include "FillControl.h"
#include "ui/ui.h"
#include "lvgl.h"

void FillControl::begin()
{

    pinMode(_valvePin, OUTPUT);
    digitalWrite(_valvePin, LOW); // Ensure valve is closed at startup
    pinMode(_sensorPin, INPUT);

    _recentVolumeIndex = 0;
    _recentVolumeCount = 0;
    _recentVolumeSum = 0.0f;

    for (int i = 0; i < FILL_AVG_WINDOW; ++i) {
        _recentVolumes[i] = 0.0f;
    }

    _state = FillControlState::FILL_DONE;

}

void FillControl::start()
{
    digitalWrite(_valvePin, HIGH);
    _state = FillControlState::FILL_RUN;
    _startTimeMillis = millis();
 
}

void FillControl::stop()
{
    digitalWrite(_valvePin, LOW);

    if (_state == FillControlState::FILL_RUN) {
        _state = FillControlState::FILL_PAUSE;
    }
    else {
        _state = FillControlState::FILL_DONE;
    }
}

void FillControl::reset()
{
    _state = FillControlState::FILL_DONE;
    lv_obj_clear_state(ui_switchFill, LV_STATE_CHECKED);
    lv_slider_set_value(ui_sliderFill, 0, LV_ANIM_OFF);

    stop();
}

void FillControl::updateUI()
{
    float fillPercent = getFillVolume() / _fillAmount * 100.0;
    lv_slider_set_value(ui_sliderFill, int(fillPercent), LV_ANIM_OFF);
    //  lv_arc_set_value(ui_arcFill, int(_pulses / _pulsesPerLiter * 100));
    //  lv_label_set_text(ui_labelFillValue, String(fillPercent, 1).c_str());
}

float FillControl::getFillVolume()
{
    float resistance = readResistance(_sensorPin, SERIES_RESISTOR);
    float volume = resistanceToVolume(resistance, ZERO_VOLUME_RESISTANCE, CALIBRATION_RESISTANCE, CALIBRATION_VOLUME);

    // Keep a rolling sum so each update is O(1).
    if (_recentVolumeCount == FILL_AVG_WINDOW) {
        _recentVolumeSum -= _recentVolumes[_recentVolumeIndex];
    }
    else {
        _recentVolumeCount++;
    }

    _recentVolumes[_recentVolumeIndex] = volume;
    _recentVolumeSum += volume;
    _recentVolumeIndex = (_recentVolumeIndex + 1) % FILL_AVG_WINDOW;

    return _recentVolumeSum / _recentVolumeCount;
}

float FillControl::readResistance(int pin, int seriesResistance)
{
    // Get ADC value.
    float resistance = analogRead(pin);
    // Convert ADC reading to resistance.
    resistance = (1023.0 / resistance) - 1.0;
    resistance = seriesResistance / resistance;
    return resistance;
}

float FillControl::resistanceToVolume(float resistance, float zeroResistance, float calResistance, float calVolume)
{
    if (resistance < zeroResistance || (zeroResistance - calResistance) == 0.0)
    {
        // Stop if the value is above the zero threshold, or no max resistance is set (would be divide by zero).
        return 0.0;
    }
    // Compute scale factor by mapping resistance to 0...1.0+ range relative to maxResistance value.
    float scale = (zeroResistance - resistance) / (zeroResistance - calResistance);
    // Scale maxVolume based on computed scale factor.
    return calVolume * scale;
}
