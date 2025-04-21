#include "static_file_manager.h"

void handle_OnConnect(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/index.html", "text/html");
}

void handle_NotFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

/*void handle_OnMillis(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", String(millis()));
}*/

void registerStaticFiles(AsyncWebServer &server)
{
    server.on("/", HTTP_GET, handle_OnConnect);
    server.serveStatic("/i2c/terminal", LittleFS, "/i2c_terminal.html");
    server.serveStatic("/i2c/scan", LittleFS, "/scan.html");
    server.serveStatic("/i2c/terminal", LittleFS, "/i2c_terminal.html");
    server.serveStatic("/i2c/sh1106", LittleFS, "/sh1106.html");
    // server.serveStatic("/i2c/eeprom", LittleFS, "/i2c_eeprom.html");
    server.serveStatic("/spi/terminal", LittleFS, "/spi_terminal.html");
    // server.serveStatic("/spi/flash", LittleFS, "/spi_flash.html");
    server.serveStatic("/spi/ws2812", LittleFS, "/ws2812.html");
    server.serveStatic("/js/lib/i2c_addr_db.js", LittleFS, "/lib/i2c_addr_db.js");
    // server.serveStatic("/js/lib/spi_flash_db.js", LittleFS, "/lib/spi_flash_db2.js");
    server.serveStatic("/js/lib/utils.js", LittleFS, "/lib/utils.js");
    server.serveStatic("/js/lib/pb.js", LittleFS, "/lib/pb.js");
    server.serveStatic("/js/lib/ws.js", LittleFS, "/lib/ws.js");
    server.serveStatic("/js/lib/i2c.js", LittleFS, "/lib/i2c.js");
    server.serveStatic("/js/lib/spi.js", LittleFS, "/lib/spi.js");
    server.serveStatic("/js/protobuf.min.js", LittleFS, "/protobuf.min.js");
    server.serveStatic("/proto/i2c.proto", LittleFS, "/proto/i2c.proto");
    server.serveStatic("/proto/spi.proto", LittleFS, "/proto/spi.proto");
    server.onNotFound(handle_NotFound);

    // server.on("/test/millis", HTTP_GET, handle_OnMillis);
}
