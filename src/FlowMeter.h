#ifndef FLOW_METER_H
#define FLOW_METER_H

#include "FillControl.h"

// Temporary compatibility header: FlowMeter is now named FillControl.
// Existing code that still refers to FlowMeter will continue to compile.
// You can remove these aliases once all references are updated.

using FlowMeter = FillControl;
using FlowMeterState = FillControlState;

// The old enum values are kept for compatibility
static constexpr FillControlState FLOW_RESET = FILL_RESET;
static constexpr FillControlState FLOW_RUN   = FILL_RUN;
static constexpr FillControlState FLOW_PAUSE = FILL_PAUSE;
static constexpr FillControlState FLOW_DONE  = FILL_DONE;

#endif // FLOW_METER_H
