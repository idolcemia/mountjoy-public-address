#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <string>
#include <functional>
#include <Arduino.h>
#include "lvgl.h"

/**
 * @class MenuManager
 * @brief Manages the list of application menus and provides
 * utility functions for dropdowns and selection.
 */
class MenuManager
{
public:
    MenuManager() = default;
    ~MenuManager() = default;

    /**
     * Function to load a screen
     */
    using ScreenLoader = std::function<void(void)>;

    /**
     * @brief Add a new menu entry.
     * @param name Name of the menu.
     * @param loader Screen loader.
     */
    void addMenu(const String &name, ScreenLoader loader);

    /**
     * @brief Add a menu entry given a screen and optional init function.
     * @param name Menu display name.
     * @param screen LVGL screen object pointer.
     * @param initFunc Optional init function to call before loading screen.
     */
    void addMenu(const String &name, lv_obj_t *screen, void (*initFunc)() = nullptr);

    /**
     * @brief Get the number of menus.
     * @return Number of menus.
     */
    size_t getMenuCount() const;

    /**
     * @brief Get menu name by index.
     * @param index Index of the menu.
     * @return Menu name as String.
     */
    String getMenu(size_t index) const;

    /**
     * @brief Get all menus as a single string delimited by \n.
     *        Useful for LVGL dropdown options.
     * @return Delimited string of menu names.
     */
    String getMenusDelimit(const String &delimiter = "\n") const;

    /**
     * @brief Call to load a menu.
     */
    void loadMenu(size_t index) const;

private:
    struct MenuEntry
    {
        String name;
        ScreenLoader loader;
    };

    std::vector<MenuEntry> _menus;

    /**
     * @brief Private helper to create a ScreenLoader from a screen + init function.
     */
    static ScreenLoader makeLoader(lv_obj_t *screen, void (*initFunc)());
};

#endif // MENU_MANAGER_H
