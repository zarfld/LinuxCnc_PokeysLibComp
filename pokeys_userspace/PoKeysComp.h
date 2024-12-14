#ifndef POKEYSCOMP_H
#define POKEYSCOMP_H

#include "PoKeysLib.h"

int32_t PK_SetupEncoders(sPoKeysDevice* device);
int32_t PK_GetEncoderConfig(sPoKeysDevice* device);
int32_t PK_SetEncoderConfig(sPoKeysDevice* device);
int32_t PK_GetEncoderValues(sPoKeysDevice* device);
int32_t PK_SetEncoderValues(sPoKeysDevice* device);

#endif // POKEYSCOMP_H
