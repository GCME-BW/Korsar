#include "spi_api.h"

AsyncWebSocket ws("/api/v1/spi/ws");
uint8_t encoded_spi_iie_answer[64];
size_t encoded_spi_iie_answer_length = 0;
uint8_t encoded_ue_answer[64];
size_t encoded_ue_answer_length = 0;

void setupSpi()
{
    pinMode(SPI_CS_PIN, OUTPUT);
    digitalWrite(SPI_CS_PIN, HIGH);
    SPI.begin();

    SPIReadWriteResponse answer = SPIReadWriteResponse_init_zero;
    pb_ostream_t stream = pb_ostream_from_buffer(encoded_spi_iie_answer, sizeof(encoded_spi_iie_answer));
    answer.error = SPIError_INCORRECT_INPUT;
    answer.read.size = 0;
    pb_encode(&stream, SPIReadWriteResponse_fields, &answer);
    encoded_spi_iie_answer_length = stream.bytes_written;

    answer = SPIReadWriteResponse_init_zero;
    stream = pb_ostream_from_buffer(encoded_ue_answer, sizeof(encoded_ue_answer));
    answer.error = SPIError_UNKNOWN;
    answer.read.size = 0;
    pb_encode(&stream, SPIReadWriteResponse_fields, &answer);
    encoded_ue_answer_length = stream.bytes_written;
}

void handleWebSocketMessage(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_BINARY)
    {
        SPIReadWriteRequest message = SPIReadWriteRequest_init_zero;

        pb_istream_t stream = pb_istream_from_buffer(data, len);

        bool status = pb_decode(&stream, SPIReadWriteRequest_fields, &message);

        if (!status)
        {
            Serial.println(1);
            ws.binary(client->id(), encoded_ue_answer, encoded_ue_answer_length);
            return;
        }

        if (!status || message.speed < 1000 || message.speed > 25000000 || message.rn_bytes < 0 || message.rn_bytes > 4096)
        {
            Serial.println(2);
            ws.binary(client->id(), encoded_spi_iie_answer, encoded_spi_iie_answer_length);
            return;
        }

        digitalWrite(SPI_CS_PIN, LOW);
        SPI.beginTransaction(SPISettings((int)message.speed, (int)message.bit_order, (int)message.mode));
        for (int i = 0; i < message.write.size; i++)
        {
            SPI.transfer(message.write.bytes[i]);
        }
        char readed_value[(int)message.rn_bytes];
        for (int i = 0; i < (int)message.rn_bytes; i++)
        {
            readed_value[i] = SPI.transfer((int)message.rw_byte);
        }
        SPI.endTransaction();
        digitalWrite(SPI_CS_PIN, HIGH);

        if (message.no_answer)
        {
            return;
        }

        uint8_t buffer[128 + message.rn_bytes];
        SPIReadWriteResponse answer = SPIReadWriteResponse_init_zero;

        pb_ostream_t stream2 = pb_ostream_from_buffer(buffer, sizeof(buffer));

        answer.error = SPIError_SUCCESS;
        answer.read.size = message.rn_bytes;
        memcpy(answer.read.bytes, readed_value, message.rn_bytes);

        status = pb_encode(&stream2, SPIReadWriteResponse_fields, &answer);
        size_t answer_length = stream2.bytes_written;

        if (!status)
        {
            Serial.println(3);
            ws.binary(client->id(), encoded_ue_answer, encoded_ue_answer_length);
            return;
        }

        if (answer_length == 0)
        {
            uint8_t tmp[4] = {0x08, 0x00, 0x12, 0x00};
            ws.binary(client->id(), tmp, 4);
            return;
        }

        ws.binary(client->id(), buffer, answer_length);
    }
}

void onSpiWS(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_DATA:
        handleWebSocketMessage(client, arg, data, len);
        break;
    case WS_EVT_CONNECT:
        // Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    case WS_EVT_DISCONNECT:
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void registerSpiApiMethods(AsyncWebServer &server)
{
    ws.onEvent(onSpiWS);
    server.addHandler(&ws);
}

void processSpiApi()
{
    ws.cleanupClients();
}
