#ifndef _SPI_API_H
#define _SPI_API_H
#include <SPI.h>
#include <ESPAsyncWebServer.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "protobuff/spi.pb.h"
#include "defines/defines.h"

void setupSpi();
void registerSpiApiMethods(AsyncWebServer &server);
void processSpiApi();
#endif
