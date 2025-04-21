#include "servo_api.h"

Servo servo;

void setupServo()
{
    ESP32PWM::allocateTimer(2);
    servo.setPeriodHertz(50);
    servo.attach(2, 1000, 2000);
}

void handle_OnServoWrite(AsyncWebServerRequest *request)
{
    if (request->hasParam("angle"))
    {
        servo.write(request->getParam("angle")->value().toInt());
    }
    request->send(200, "text/plain", "");
}

// void handle_OnI2CWrite(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
// {
//     I2CReadWriteRequest message = I2CReadWriteRequest_init_zero;
//
//     pb_istream_t stream = pb_istream_from_buffer(data, len);
//
//     bool status = pb_decode(&stream, I2CReadWriteRequest_fields, &message);
//
//     if (!status)
//     {
//         request->send(500);
//         return;
//     }
//
//     if (message.address < 0 || message.address > 127 || message.write.size > 256)
//     {
//         request->send(200, "application/octet-stream", encoded_i2c_iie_answer, encoded_i2c_iie_answer_length);
//         return;
//     }
//     if (message.frequency != Wire.getClock())
//     {
//         Wire.setClock(message.frequency);
//     }
//     Wire.beginTransmission(message.address);
//     for (int i = 0; i < message.write.size; i++)
//     {
//         Wire.write(message.write.bytes[i]);
//     }
//     int code = Wire.endTransmission();
//     int bytes = message.byte_number;
//     char answer[bytes];
//     if (bytes > 0)
//     {
//         Wire.requestFrom(message.address, (uint8_t)bytes);
//         for (int i = 0; i < bytes; i++)
//         {
//             if (Wire.available())
//             {
//                 answer[i] = Wire.read();
//             }
//             else
//             {
//                 bytes = i;
//                 break;
//             }
//         }
//     }
//
//     uint8_t pbuffer[128 + bytes];
//     I2CReadWriteResponse response = I2CReadWriteResponse_init_zero;
//
//     pb_ostream_t stream2 = pb_ostream_from_buffer(pbuffer, sizeof(pbuffer));
//
//     switch (code)
//     {
//     case 0:
//         response.error = I2CError_SUCCESS;
//         break;
//     case 1:
//         response.error = I2CError_INCORRECT_INPUT;
//         break;
//     case 2:
//         response.error = I2CError_NO_ACKNOWLEDGE_ADDRESS;
//         break;
//     case 3:
//         response.error = I2CError_NO_ACKNOWLEDGE_DATA;
//         break;
//     case 4:
//         response.error = I2CError_UNKNOWN;
//         break;
//     case 5:
//         response.error = I2CError_TIMEOUT;
//         break;
//     default:
//         response.error = I2CError_UNKNOWN;
//         break;
//     }
//
//     response.read.size = bytes;
//     memcpy(response.read.bytes, answer, bytes);
//
//     status = pb_encode(&stream2, I2CReadWriteResponse_fields, &response);
//     size_t answer_length = stream2.bytes_written;
//
//     if (!status)
//     {
//         request->send(500);
//         return;
//     }
//
//     request->send(200, "application/octet-stream", pbuffer, answer_length);
// }

void registerServoApiMethods(AsyncWebServer &server)
{
    server.on("/api/v1/servo/write", HTTP_GET, handle_OnServoWrite);
    // server.on(
    //     "/api/v1/i2c/read_write",
    //     HTTP_POST,
    //     [](AsyncWebServerRequest *request) {},
    //     NULL,
    //     handle_OnI2CWrite);
}

void processServoApi() {}
