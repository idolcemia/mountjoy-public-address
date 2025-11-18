#include "MenuManager.h"

void MenuManager::addMenu(const String &name, ScreenLoader loader)
{
    _menus.push_back({name, loader});
}

void MenuManager::addMenu(const String &name, lv_obj_t *screen, void (*initFunc)())
{
    _menus.push_back({name, makeLoader(screen, initFunc)});
}

MenuManager::ScreenLoader MenuManager::makeLoader(lv_obj_t *screen, void (*initFunc)())
{
    return [screen, initFunc]()
    {
        if (initFunc)
            initFunc();
        lv_scr_load(screen);
    };
}

size_t MenuManager::getMenuCount() const
{
    return _menus.size();
}

String MenuManager::getMenu(size_t index) const
{
    if (index < _menus.size())
        return _menus[index].name;
    return "";
}

String MenuManager::getMenusDelimit(const String &delimiter) const
{
    String result;
    for (size_t i = 0; i < _menus.size(); ++i)
    {
        result += _menus[i].name;
        if (i < _menus.size() - 1)
            result += delimiter;
    }
    return result;
}

void MenuManager::loadMenu(size_t index) const
{
    if (index < _menus.size() && _menus[index].loader)
        _menus[index].loader();
}
