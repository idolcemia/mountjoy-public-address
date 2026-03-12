#include "PressureControl.h"
#include "ui/ui.h"
#include "lvgl.h"
#include "Wire.h"
#include "Globals.h"

PressureControl *PressureControl::_activeInstance = nullptr;

void PressureControl::requestEventThunk()
{
    if (_activeInstance != nullptr)
    {
        _activeInstance->requestEvent();
    }
}

// ---------------- Constructor ----------------
PressureControl::PressureControl(

    int valvePin,
    int ventPin,
    int overPressure,
    int underPressure)
    : _valvePin(valvePin), _ventPin(ventPin), _overPressure(overPressure), _underPressure(underPressure)

{
}

void PressureControl::begin()
{
    pinMode(_valvePin, OUTPUT);
    pinMode(_ventPin, OUTPUT);
    pinMode(_overPressure, INPUT_PULLUP);
    pinMode(_underPressure, INPUT_PULLUP);
    digitalWrite(_valvePin, LOW); // Ensure valve is closed at startup
    digitalWrite(_ventPin, HIGH); // Ensure vent is open at startup

    Wire.begin(_publicAddress); // Initiate communication with PublicAddress system
    _activeInstance = this;
    Wire.onRequest(PressureControl::requestEventThunk); // Set up event handler for requests from PublicAddress system
    _state = PressureControlState::PC_DONE;
}

void PressureControl::start()
{
    digitalWrite(_valvePin, HIGH);
    digitalWrite(_ventPin, LOW);
    _state = PressureControlState::PC_RUN;
}
void PressureControl::stop()
{
    digitalWrite(_valvePin, LOW);
    digitalWrite(_ventPin, HIGH);
    _state = PressureControlState::PC_DONE;
}
void PressureControl::reset()
{
    stop();
}
int PressureControl::getPressure()
{
    // Placeholder for actual pressure reading logic
    if (digitalRead(_overPressure) == LOW)
    {
        // Over pressure condition
        return 1; // Example value indicating over pressure
    }
    else if (digitalRead(_underPressure) == LOW)
    {
        // Under pressure condition
        return -1; // Example value indicating under pressure
    }
    else
    {
        // Normal pressure condition
        return 0; // Example value indicating normal pressure
    }
}

int PressureControl::underPressure()
{
    return digitalRead(_underPressure) == LOW ? 1 : 0; // Return 1 if under pressure, 0 if normal
}
int PressureControl::overPressure()
{
    return digitalRead(_overPressure) == LOW ? 1 : 0; // Return 1 if over pressure, 0 if normal
}

void PressureControl::requestEvent()
{
    logger.info("Event request received " + String(getPressure()) + " from PublicAddress system");
    // This function will be called when the PublicAddress system requests data
    // You can send the current pressure status back to the PublicAddress system here
    int pressureStatus = getPressure(); // Get current pressure status

    pressureStatus = -1;        // Placeholder for testing - simulates under pressure condition
    Wire.write(pressureStatus); // Send pressure status back to PublicAddress system
}

float PressureControl::updateUI()
{
    // Placeholder until dedicated pressure UI widgets are connected.
    return static_cast<float>(getPressure());
}
