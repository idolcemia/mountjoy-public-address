#include <Arduino.h>
#include <RumpshiftLogger.h>
#include "events.h"
#include "ui/ui.h"
#include "Globals.h"

extern RumpshiftLogger logger;

void handleUserDropdownEvent(lv_event_t *e)
{
    // Cast void* to lv_obj_t* (required in LVGL 9)
    lv_obj_t *dd = (lv_obj_t *)lv_event_get_target(e);
    uint16_t selected = lv_dropdown_get_selected(dd);

    logger.info("Dropdown1 selected index: ");
    logger.info(selected);

    if (users && selected < users->getUserCount())
    {
        const char *name = users->getUsers()[selected].getName().c_str();
        lv_label_set_text(ui_Label2, name);

        logger.info("Dropdown1 selected name: ");
        logger.info(String(name));
    }
    else
    {
        lv_label_set_text(ui_Label2, "Unknown");
    }

    // Refresh label to show new text
    lv_obj_invalidate(ui_Label2);
}
