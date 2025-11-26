#ifndef UI_GLOBAL_LABELS_H
#define UI_GLOBAL_LABELS_H

#include <lvgl.h>
#include "Globals.h" // for network object

namespace ui_GlobalLabels
{
    // Labels shared across screens
    extern lv_obj_t *networkStatusLabel;
    extern lv_obj_t *menuDisplayLabel;

    // Time since last check (in milliseconds)
    extern uint32_t lastNetworkCheck;

    // Initialize network status label and optionally assign NetworkManager
    void initNetworkStatus(lv_obj_t *parent);

    // Update network label text
    void updateNetworkStatus();

    void networkChecked();
}

#endif // UI_GLOBAL_LABELS_H
