#pragma once
#include <Arduino.h>
#include "hashlength.h"

#define CHALLENGE_CODE_LENGTH 20

uint8_t *get_challenge_code();

bool response_valid(unsigned char code, unsigned char message[HASH_LENGTH]);