#ifndef POKEYSCOMP_H
#define POKEYSCOMP_H

#include "PoKeysLib.h"
#include "rtapi.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	hal_u32_t PoNET_moduleID; 		// RO Parameter
	hal_u32_t PoNET_i2cAddress; 	// RO Parameter
	hal_u32_t PoNET_moduleType; 	// RO Parameter
	hal_u32_t PoNET_moduleSize;		// RO Parameter
	hal_u32_t PoNET_moduleOptions; 	// RO Parameter
	hal_u32_t *PoNET_statusIn[16];  // OUT pin
	hal_u32_t *PoNET_statusOut[16]; // IN pin
}one_PoNET_data_t;

typedef struct
{
	hal_bit_t *LED;
	hal_bit_t *Button;
	
}one_kbd48CNCButton_data_t ;

typedef struct
{
	one_PoNET_data_t PoNET[16];
	hal_u32_t *PoNET_PWMduty;
	hal_u32_t *PoNET_lightValue;
	hal_u32_t *PoNET_PoNETstatus;
	hal_u32_t PoNET_DevCount; // RO Parameter

	hal_bit_t *kbd48CNC_available;
	hal_u32_t *kbd48CNC_PoNetID;
	hal_u32_t *kbd48CNC_KeyBrightness;
	hal_u32_t *kbd48CNC_prevBrightness;
	hal_u32_t *kbd48CNC_lightValue;
	one_kbd48CNCButton_data_t *kbd48CNCio[48];
	uint8_t kbd48CNC_Counter;
	hal_s32_t *deb_out;
}all_PoNET_data_t;

typedef struct
{
	hal_bit_t *PoExtBus_digin_in[8];

	hal_bit_t *PoExtBus_digin_in_not[8];

	hal_bit_t *PoExtBus_digout_out[8];

	hal_bit_t PoExtBus_digout_invert[8];

	hal_u32_t PoExtBus_DataSet;
    hal_u32_t PoExtBus_DataGet;
	
} one_PoExtBus_data_t;

typedef struct
{
	one_PoExtBus_data_t PoExtBus[10];
	hal_s32_t *PoExtBus_deb_out; // pin out s32 deb.out;

	hal_u32_t PoExtBus_count;
} all_PoExtBus_data_t;

typedef struct
{
	// canonicaldevice interface pins
    hal_s32_t *count;      		// pointer for "pin out s32 encoder.#.count[29]"
    hal_float_t *position;		// pointer for "pin out float encoder.#.position[29]"
    hal_float_t *velocity;		// pointer for "pin out float encoder.#.velocity[29]"
    hal_bit_t *reset; 			// pointer for "pin in bit encoder.#.reset[29]"
	hal_bit_t *index_enable;	// pointer for "pin in bit encoder.#.index-enable[29]"

	hal_float_t scale;			// pointer for "param rw float encoder.#.scale[29]  "The scale factor used to convert counts to position units. It is in “counts per position unit”""

    // Custom parameters for communication with PoKeys
    hal_u32_t encoderOptions;  // pointer for "pin io unsigned Encoders.#.encoderOptions [26]"		
													// Encoder options -    bit 0: enable encoder
													//                      bit 1: 4x sampling
													//                      bit 2: 2x sampling
													//                      bit 3: reserved
													//                      bit 4: direct key mapping for direction A
													//                      bit 5: mapped to macro for direction A
													//                      bit 6: direct key mapping for direction B
													//                      bit 7: mapped to macro for direction B
    hal_u32_t channelApin; 	// pointer for "pin io unsigned Encoders.#.channelApin [26]"         // Channel A encoder pin"
	hal_u32_t channelBpin; 	// pointer for "pin io unsigned Encoders.#.channelBpin [26]"         // Channel B encoder pin"

} one_encoder_data_t;

typedef struct
{
    one_encoder_data_t encoder[29];
	hal_s32_t *encoder_deb_out; // pin out s32 deb.out;

} all_encoder_data_t;

int PKEncoder_init(int id, sPoKeysDevice* dev);
void PKEncoder_Update(sPoKeysDevice* dev);

void PKPoExtBus_Update(sPoKeysDevice *dev);

#ifdef __cplusplus
}
#endif


#endif // POKEYSCOMP_H