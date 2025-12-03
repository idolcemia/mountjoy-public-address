#ifndef UI_GLOBAL_LABELS_H
#define UI_GLOBAL_LABELS_H

#include <lvgl.h>
#include "Globals.h" // for network object

namespace ui_GlobalLabels
{
    // Labels shared across screens
    extern lv_obj_t *networkStatusLabel;
    extern lv_obj_t *userSelectionLabel;
    extern lv_obj_t *menuDisplayLabel;

    // Time since last check (in milliseconds)
    extern uint32_t lastNetworkCheck;

    // Initialize network status label and optionally assign NetworkManager
    void initNetworkStatus(lv_obj_t *parent = nullptr);
    // Update network label text
    void updateNetworkStatus(lv_obj_t *parent = nullptr);

    void initUserSelectionLabel(lv_obj_t *parent = nullptr);
    void updateUserSelectionLabel(lv_obj_t *parent = nullptr);

    inline void networkChecked()
    {
        lastNetworkCheck = millis();
    }

}

#endif // UI_GLOBAL_LABELS_H
