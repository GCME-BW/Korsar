#ifndef _I2C_API_H
#define _I2C_API_H
#include <Wire.h>
#include "ESPAsyncWebServer.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "protobuff/i2c.pb.h"
#include "defines/defines.h"

void setupI2c();
void registerI2cApiMethods(AsyncWebServer &server);
void processI2cApi();
#endif
