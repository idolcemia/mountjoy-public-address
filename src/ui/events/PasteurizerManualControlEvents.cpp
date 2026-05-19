#include <Arduino.h>
#include "events.h"
#include "ui/ui.h"
#include "Globals.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "ui/screens/ui_GlobalButtons.h"

// Helper function to log button state change
static void logButtonState(const char *name, bool state)
{
    logger.info("[PasteurizerManualControlEvents] " + String(name) + " button " + (state ? "enabled" : "disabled"));
}

void dropdownUserChanged(lv_event_t *e)
{
    // If user set, enable checkpoint switch

    if (lv_dropdown_get_selected(ui_dropdownUser) > 0) // If a user is selected (index 0 is "Select User")
    {
        //       lv_obj_remove_state(ui_switchCheckPoint, LV_STATE_DISABLED); // Make checkpoint switch enabled
    }
}

// Generic button handler to toggle state and call appropriate event
void handleManualControlButton(
    lv_event_t *e,
    lv_obj_t *button, bool &state,
    void (*onEnabled)(lv_event_t *),
    void (*onDisabled)(lv_event_t *))
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        state = !state;

        // Update button color
        lv_color_t color = state ? lv_color_hex(0x28A745) : lv_color_hex(0xDC3545);
        lv_obj_set_style_bg_color(button, color, LV_PART_MAIN);

        // Call appropriate event
        if (state && onEnabled)
            onEnabled(e);
        if (!state && onDisabled)
            onDisabled(e);
    }
}

void PlayButtonPressed(lv_event_t *e)
{
    audioMaster.playAudioFile("medscan2.wav");
}

void BlinkEnabled(lv_event_t *e)
{
    audioMaster.setLight(true);
}

void BlinkDisabled(lv_event_t *e)
{
    audioMaster.setLight(false);
}

void StopButtonPressed(lv_event_t *e)
{
    audioMaster.stopAudio();
}
