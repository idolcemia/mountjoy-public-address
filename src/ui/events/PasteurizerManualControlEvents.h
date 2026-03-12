#pragma once

#include "lvgl.h"




// --- Heat Button Events ---
void ui_event_buttonHeatEnabled(lv_event_t *e);
void ui_event_buttonHeatDisabled(lv_event_t *e);
// void ui_event_OperationButton(lv_event_t *e);

// --- Operation Button Events ---
void ui_event_OperationButtonEnabled(lv_event_t *e);
void ui_event_OperationButtonDisabled(lv_event_t *e);
// void ui_event_OperationButton(lv_event_t *e);

// --- Heat Button Events ---
void ui_event_HeatButtonEnabled(lv_event_t *e);
void ui_event_HeatButtonDisabled(lv_event_t *e);
// void ui_event_HeatButton(lv_event_t *e);

// --- Chill Button Events ---
void ui_event_ChillButtonEnabled(lv_event_t *e);
void ui_event_ChillButtonDisabled(lv_event_t *e);
// void ui_event_ChillButton(lv_event_t *e);

// --- Pump Button Events ---
void ui_event_PumpButtonEnabled(lv_event_t *e);
void ui_event_PumpButtonDisabled(lv_event_t *e);
// void ui_event_PumpButton(lv_event_t *e);

// --- Generic handler (optional if needed elsewhere) ---
void handleManualControlButton(
    lv_event_t *e, lv_obj_t *button, bool &state,
    void (*onEnabled)(lv_event_t *),
    void (*onDisabled)(lv_event_t *));
