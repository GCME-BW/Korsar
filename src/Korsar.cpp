#include "wifi/wifi.h"
#include <static_file_manager/static_file_manager.h>
#include <i2c/i2c_api.h>
#include <spi/spi_api.h>
#include <servo/servo_api.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"

AsyncWebServer server(80);

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    Serial.println("\n Starting");

    if (!LittleFS.begin(true))
    {
        Serial.println("An error has occurred while mounting LittleFS");
    }
    Serial.println("LittleFS mounted successfully");

    setupI2c();
    setupSpi();
    setupServo();
    setupWifi();

    registerStaticFiles(server);
    registerI2cApiMethods(server);
    registerSpiApiMethods(server);
    registerServoApiMethods(server);

    server.begin();
}

void loop()
{
    processWifi();
    processSpiApi();
    processI2cApi();
    processServoApi();
}
