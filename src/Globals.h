#ifndef GLOBALS_H
#define GLOBALS_H

#include <RumpshiftLogger.h>
#include <Users.h>
#include <NetworkManager.h>
#include "ui/MenuManager.h"

extern RumpshiftLogger logger;
extern Users *users;
extern NetworkManager *network;
extern MenuManager menus;

#endif
