#ifndef POKEYSCOMP_H
#define POKEYSCOMP_H

#include "PoKeysLib.h"
#include "rtapi.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

int PKEncoder_init(int id, sPoKeysDevice* dev);
void PKEncoder_Update(sPoKeysDevice* dev);

#ifdef __cplusplus
}
#endif


#endif // POKEYSCOMP_H