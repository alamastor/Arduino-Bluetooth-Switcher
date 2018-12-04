#include <SoftwareSerial.h>

SoftwareSerial bleSerial(2, 3); // RX, TX
int pins[] = {8, 9, 10, 11};
int state = 0;
int flag = 0;

enum AppState
{
    AWAITING_REQUEST,
    AWAITING_MESSAGE
};

AppState appState = AWAITING_REQUEST;

void setup()
{
    Serial.println("initializing");
    for (int i = 0; i < 4; i++)
    {
        pinMode(pins[i], OUTPUT);
    }
    Serial.begin(9600);
    bleSerial.begin(9600);
}

void get_request()
{
    char expected[] = "<requesting code>";
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
                    Serial.println("recieved code request");
                    char str[] = {'h', 'i', '\0'};
                    Serial.println(str);
                    bleSerial.write(str);
                    appState = AWAITING_MESSAGE;
                    return;
                }
            }
            else
            {
                Serial.println("unexpected char '" + String(val) + "' while getting request");
                idx = 0;
            }
        }
    }
}

void get_message()
{
    if (bleSerial.available() > 0)
    {
        char val = bleSerial.read();
        Serial.println("recieved msg");
        Serial.println(val);

        switch (val)
        {
        case '1':
            digitalWrite(pins[0], HIGH);
            delay(1000);
            digitalWrite(pins[0], LOW);
            break;
        case '2':
            digitalWrite(pins[1], HIGH);
            delay(1000);
            digitalWrite(pins[1], LOW);
            break;
        case '3':
            digitalWrite(pins[2], HIGH);
            delay(1000);
            digitalWrite(pins[2], LOW);
            break;
        case '4':
            digitalWrite(pins[3], HIGH);
            delay(1000);
            digitalWrite(pins[3], LOW);
            break;
        }
        appState = AWAITING_REQUEST;
    }
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