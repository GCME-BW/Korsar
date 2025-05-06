#ifndef _STATIC_FILE_MANAGER_H
#define _STATIC_FILE_MANAGER_H
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include "defines/defines.h"

void registerStaticFiles(AsyncWebServer &server);
#endif
