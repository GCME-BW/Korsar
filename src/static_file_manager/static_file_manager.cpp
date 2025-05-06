#include "static_file_manager.h"

void handle_OnConnect(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/index.html", "text/html");
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
    server.serveStatic("/i2c/terminal", SPIFFS, "/i2c_terminal.html");
    server.serveStatic("/i2c/scan", SPIFFS, "/scan.html");
    server.serveStatic("/i2c/terminal", SPIFFS, "/i2c_terminal.html");
    server.serveStatic("/i2c/sh1106", SPIFFS, "/sh1106.html");
    // server.serveStatic("/i2c/eeprom", SPIFFS, "/i2c_eeprom.html");
    server.serveStatic("/spi/terminal", SPIFFS, "/spi_terminal.html");
    // server.serveStatic("/spi/flash", SPIFFS, "/spi_flash.html");
    server.serveStatic("/spi/ws2812", SPIFFS, "/ws2812.html");
    server.serveStatic("/js/lib/i2c_addr_db.js", SPIFFS, "/lib/i2c_addr_db.js");
    // server.serveStatic("/js/lib/spi_flash_db.js", SPIFFS, "/lib/spi_flash_db2.js");
    server.serveStatic("/js/lib/utils.js", SPIFFS, "/lib/utils.js");
    server.serveStatic("/js/lib/pb.js", SPIFFS, "/lib/pb.js");
    server.serveStatic("/js/lib/ws.js", SPIFFS, "/lib/ws.js");
    server.serveStatic("/js/lib/i2c.js", SPIFFS, "/lib/i2c.js");
    server.serveStatic("/js/lib/spi.js", SPIFFS, "/lib/spi.js");
    server.serveStatic("/js/protobuf.min.js", SPIFFS, "/protobuf.min.js");
    server.serveStatic("/proto/i2c.proto", SPIFFS, "/proto/i2c.proto");
    server.serveStatic("/proto/spi.proto", SPIFFS, "/proto/spi.proto");
    server.onNotFound(handle_NotFound);

    // server.on("/test/millis", HTTP_GET, handle_OnMillis);
}
