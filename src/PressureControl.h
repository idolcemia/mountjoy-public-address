#ifndef PRESSURE_CONTROL_H
#define PRESSURE_CONTROL_H

#include <Arduino.h>
#include "PressureControl.h"

enum PressureControlState
{
    PC_RESET = 0,
    PC_RUN = 1,
    PC_PAUSE = 2,
    PC_DONE = 3
};

/**
 * Controls pressure in closed system.
 *
 * CIRCUIT CONFIGURATION:
 * HOW IT WORKS:
 */

class PressureControl
{
private:
    static PressureControl *_activeInstance;
    static void requestEventThunk();

    int
        _valvePin,
        _ventPin,
        _overPressure,
        _underPressure,
        _publicAddress = 9; // I2C bus address for 2-way communications with PublicAddress system
    ;

public:
    void requestEvent();

    /**
     * Constructor
     *
     */

    PressureControlState _state;

    void begin();

    PressureControl(
        int valvePin,
        int ventPin,
        int overPressure,
        int underPressure);

    /**
     * Start pressure control
     */
    void start();

    /**
     * Stop pressure control
     */
    void stop();

    /**
     * Reset the pressure control state and stop operation
     */
    void reset();

    /**
     * Take pressure reading and update state
     */
    int getPressure(); // remains pressure reading until pressure logic added

    /**
     * Take pressure reading and update state
     */

    int overPressure();

    int underPressure();


    /**
     * Update display elements based on current pressure and state. Returns current pressure for convenience.
     */
    float updateUI();
};

#endif
