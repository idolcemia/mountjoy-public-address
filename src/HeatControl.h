#ifndef HEAT_CONTROL_H
#define HEAT_CONTROL_H

#include "PressureControl.h"

// Temporary compatibility header: HeatControl renamed to PressureControl
using HeatControl = PressureControl;
using HeatControlState = PressureControlState;

static constexpr PressureControlState HC_RESET = PC_RESET;
static constexpr PressureControlState HC_RUN = PC_RUN;
static constexpr PressureControlState HC_PAUSE = PC_PAUSE;
static constexpr PressureControlState HC_DONE = PC_DONE;

#endif // HEAT_CONTROL_H
