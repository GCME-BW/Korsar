#ifndef _STATIC_FILE_MANAGER_H
#define _STATIC_FILE_MANAGER_H
#include "LittleFS.h"
#include <ESPAsyncWebServer.h>
#include "defines/defines.h"

void registerStaticFiles(AsyncWebServer &server);
#endif
