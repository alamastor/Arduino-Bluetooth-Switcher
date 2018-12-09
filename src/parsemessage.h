#pragma once
#include <SoftwareSerial.h>

struct Message
{
    unsigned char code;
    unsigned char hash[32];
};

int parse_message_stream(Message &message, SoftwareSerial &serial);