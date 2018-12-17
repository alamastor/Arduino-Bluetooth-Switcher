#include "challenge.h"
#include "../sharedkey.h"
#include "sha256.h"

#define CODE_VALID_MILLIS 2000

uint8_t challengeCode[CHALLENGE_CODE_LENGTH];
unsigned long challengeCodeCreationMillis;

uint8_t *get_challenge_code()
{
    for (int i = 0; i < CHALLENGE_CODE_LENGTH; i++)
        challengeCode[i] = random(0, 256);
    challengeCodeCreationMillis = millis();
    return challengeCode;
}

bool response_valid(unsigned char code, unsigned char hash[HASH_LENGTH])
{
    if ((millis() - challengeCodeCreationMillis) > CODE_VALID_MILLIS)
    {
        Serial.println("challenge code expired");
        return false;
    }

    Sha256 sha256 = Sha256();
    sha256.initHmac(sharedKey, sharedKeyLen);

    for (int i = 0; i < CHALLENGE_CODE_LENGTH; i++)
        sha256.write(challengeCode[i]);
    sha256.write(code);
    uint8_t *expected_hash = sha256.resultHmac();

    bool hash_valid(true);
    for (int i = 0; i < HASH_LENGTH; i++)
        if (expected_hash[i] != hash[i])
            hash_valid = false;

    if (!hash_valid)
    {
        Serial.println("hash validation failed");
        return false;
    }
    return true;
}