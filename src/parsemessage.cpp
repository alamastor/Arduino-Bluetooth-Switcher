#include <Arduino.h>
#include "parsemessage.h"
#include "hashlength.h"

int parse_hash_message_stream(Message &message, SoftwareSerial &serial)
{
    const char prefix[] = "<hash:";
    for (int idx = 0; idx < sizeof(prefix) - 1;)
    {
        if (serial.available())
        {
            char val = serial.read();
            if (val == prefix[idx])
            {
                idx++;
            }
            else
            {
                Serial.println("unexpected char '" + String(val) + "' while parsing hash prefix");
                return 1;
            }
        }
    }
    for (int idx = 0; idx < HASH_LENGTH;)
    {
        if (serial.available())
        {
            message.hash[idx] = serial.read();
            idx++;
        }
    }
    for (;;)
    {
        if (serial.available())
        {
            char val = serial.read();
            if (val == '>')
            {
                return 0;
            }
            else
            {
                Serial.println("unexpected char '" + String(val) + "' while parsing hash suffix");
                return 1;
            }
        }
    }
}

int parse_code_message_stream(Message &message, SoftwareSerial &serial)
{
    const char prefix[] = "<code:";
    for (int idx = 0; idx < sizeof(prefix) - 1;)
    {
        if (serial.available())
        {
            char val = serial.read();
            if (val == prefix[idx])
            {
                idx++;
            }
            else
            {
                Serial.println("unexpected char '" + String(val) + "' while parsing code prefix");
                return 1;
            }
        }
    }
    for (int idx = 0; idx < 1;)
    {
        if (serial.available())
        {
            message.code = serial.read();
            idx++;
        }
    }
    for (;;)
    {
        if (serial.available())
        {
            char val = serial.read();
            if (val == '>')
            {
                return 0;
            }
            else
            {
                Serial.println("unexpected char '" + String(val) + "' while parsing hash suffix");
                return 1;
            }
        }
    }
    return 0;
}

int parse_message_stream(Message &message, SoftwareSerial &serial)
{
    if (parse_hash_message_stream(message, serial) != 0)
        return 1;
    if (parse_code_message_stream(message, serial) != 0)
        return 1;
    return 0;
}