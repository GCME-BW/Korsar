#ifndef _WIFI_H
#define _WIFI_H
#include "WiFiManager.h"
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "protobuff/wifi_settings.pb.h"
#include "defines/defines.h"

void setupWifi();
void processWifi();
#endif
