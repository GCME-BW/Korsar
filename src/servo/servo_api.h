#ifndef _SERVO_API_H
#define _SERVO_API_H
#include <Wire.h>
#include <ESPAsyncWebServer.h>
// #include <pb_encode.h>
// #include <pb_decode.h>
#include "defines/defines.h"
#include "ESP32Servo.h"

void setupServo();
void registerServoApiMethods(AsyncWebServer &server);
void processServoApi();
#endif
