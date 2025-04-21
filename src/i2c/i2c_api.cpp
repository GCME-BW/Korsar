#include "i2c_api.h"

uint8_t encoded_i2c_iie_answer[64];
size_t encoded_i2c_iie_answer_length = 0;

void setupI2c()
{
    Wire.begin();
    Wire.setBufferSize(256);

    I2CReadWriteResponse answer = I2CReadWriteResponse_init_zero;
    pb_ostream_t stream = pb_ostream_from_buffer(encoded_i2c_iie_answer, sizeof(encoded_i2c_iie_answer));
    answer.error = I2CError_UNKNOWN;
    answer.read.size = 0;
    pb_encode(&stream, I2CReadWriteResponse_fields, &answer);
    encoded_i2c_iie_answer_length = stream.bytes_written;
}

void handle_OnI2CScan(AsyncWebServerRequest *request)
{
    byte error, address;
    String mapArr = "00000000";
    for (address = 8; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            mapArr.concat("1");
        }
        else if (error == 4)
        {
            // TODO
            mapArr.concat("0");
        }
        else
        {
            mapArr.concat("0");
        }
    }
    request->send(200, "text/plain", mapArr);
}

void handle_OnI2CWrite(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    I2CReadWriteRequest message = I2CReadWriteRequest_init_zero;

    pb_istream_t stream = pb_istream_from_buffer(data, len);

    bool status = pb_decode(&stream, I2CReadWriteRequest_fields, &message);

    if (!status)
    {
        request->send(500);
        return;
    }

    if (message.address < 0 || message.address > 127 || message.write.size > 256)
    {
        request->send(200, "application/octet-stream", encoded_i2c_iie_answer, encoded_i2c_iie_answer_length);
        return;
    }
    if (message.frequency != Wire.getClock())
    {
        Wire.setClock(message.frequency);
    }
    Wire.beginTransmission((uint8_t) message.address);
    for (int i = 0; i < message.write.size; i++)
    {
        Wire.write(message.write.bytes[i]);
    }
    int code = Wire.endTransmission();
    int bytes = message.byte_number;
    char answer[bytes];
    if (bytes > 0)
    {
        Wire.requestFrom((uint8_t)message.address, (uint8_t)bytes);
        for (int i = 0; i < bytes; i++)
        {
            if (Wire.available())
            {
                answer[i] = Wire.read();
            }
            else
            {
                bytes = i;
                break;
            }
        }
    }

    uint8_t pbuffer[128 + bytes];
    I2CReadWriteResponse response = I2CReadWriteResponse_init_zero;

    pb_ostream_t stream2 = pb_ostream_from_buffer(pbuffer, sizeof(pbuffer));

    switch (code)
    {
    case 0:
        response.error = I2CError_SUCCESS;
        break;
    case 1:
        response.error = I2CError_INCORRECT_INPUT;
        break;
    case 2:
        response.error = I2CError_NO_ACKNOWLEDGE_ADDRESS;
        break;
    case 3:
        response.error = I2CError_NO_ACKNOWLEDGE_DATA;
        break;
    case 4:
        response.error = I2CError_UNKNOWN;
        break;
    case 5:
        response.error = I2CError_TIMEOUT;
        break;
    default:
        response.error = I2CError_UNKNOWN;
        break;
    }

    response.read.size = bytes;
    memcpy(response.read.bytes, answer, bytes);

    status = pb_encode(&stream2, I2CReadWriteResponse_fields, &response);
    size_t answer_length = stream2.bytes_written;

    if (!status)
    {
        request->send(500);
        return;
    }

    request->send(200, "application/octet-stream", pbuffer, answer_length);
}

void registerI2cApiMethods(AsyncWebServer &server)
{
    server.on("/api/v1/i2c/scan", HTTP_GET, handle_OnI2CScan);
    server.on(
        "/api/v1/i2c/read_write",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {},
        NULL,
        handle_OnI2CWrite);
}

void processI2cApi() {}
