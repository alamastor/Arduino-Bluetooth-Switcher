#include <SoftwareSerial.h>
#include "src/challenge.h"
#include "src/parsemessage.h"

SoftwareSerial bleSerial(2, 3); // RX, TX
int pins[] = {8, 9, 10, 11};
int state = 0;
int flag = 0;

enum AppState
{
    AWAITING_REQUEST,
    AWAITING_MESSAGE
};

unsigned long sharedKey;

AppState appState;

void setup()
{
    Serial.println("initializing");
    for (int i = 0; i < 4; i++)
    {
        pinMode(pins[i], OUTPUT);
    }
    Serial.begin(9600);
    bleSerial.begin(9600);
    randomSeed(analogRead(13));
    await_request();
}

void await_request()
{
    Serial.println("awaiting request");
    bleSerial.print("<awaiting request>");
    appState = AWAITING_REQUEST;
}

void await_message()
{
    Serial.println("awaiting message");
    appState = AWAITING_MESSAGE;
}

void get_request()
{
    char expected[] = "<requesting chall>";
    int idx = 0;
    for (;;)
    {
        if (bleSerial.available() > 0)
        {
            char val = bleSerial.read();
            if (val == expected[idx])
            {
                idx++;
                if (expected[idx] == '\0')
                {
                    Serial.println("recieved challenge request");
                    uint8_t *challengeCode = get_challenge_code();
                    for (int i = 0; i < CHALLENGE_CODE_LENGTH; i++)
                    {
                        bleSerial.write(challengeCode[i]);
                    }
                    await_message();
                    return;
                }
            }
            else
            {
                Serial.println("unexpected char '" + String(val) + "' at index '" + String(idx) + "' while getting request");
                idx = 0;
                await_request();
            }
        }
    }
}

void get_message()
{
    Message message;
    if (parse_message_stream(message, bleSerial) == 0)
    {
        Serial.println("successfully parsed message stream");
        if (response_valid(message.code, message.hash))
        {
            char pin_idx = constrain(message.code, 0, 3);
            Serial.println("message valid, opening lock " + String(pin_idx + 1));
            digitalWrite(pins[pin_idx], HIGH);
            delay(1000);
            digitalWrite(pins[pin_idx], LOW);
        }
        else
        {
            Serial.println("message validation failed");
        }
    }
    else
    {
        Serial.println("failed to parse message stream");
    }
    await_request();
}

void loop()
{
    switch (appState)
    {
    case AWAITING_REQUEST:
        get_request();
        break;
    case AWAITING_MESSAGE:
        get_message();
        break;
    }
}