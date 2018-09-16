int pins[] = {8, 9, 10, 11};
int state = 0;
int flag = 0;

void setup()
{
    Serial.println("initializing");
    for (int i = 0; i < 4; i++)
    {
        pinMode(pins[i], OUTPUT);
    }
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available() > 0)
    {
        char val = Serial.read();
        char str[] = {val, '\0'};
        Serial.println(str);

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
    }
}
