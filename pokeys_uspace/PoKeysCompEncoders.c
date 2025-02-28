

#include "PoKeysLib.h"
#include "PoKeysComp.h"
#include "rtapi.h"
// #include "rtapi_app.h"
#include "hal.h"
#include "stdio.h"


// Pin creation:
/*
pin out s32 encoder.#.count[29];
pin out float encoder.#.position[29];
pin out float encoder.#.velocity[29];
pin in bit encoder.#.reset[29];
pin in bit encoder.#.index-enable[29];
param rw float encoder.#.scale[29]  "The scale factor used to convert counts to position units. It is in “counts per position unit”";
*/
// pin out unsigned Encoders.#.encoderValue [26];       // Encoder current value
// pin io unsigned Encoders.#.encoderOptions [26];      // Encoder options -    bit 0: enable encoder
//                       bit 1: 4x sampling
//                       bit 2: 2x sampling
//                       bit 3: reserved
//                       bit 4: direct key mapping for direction A
//                       bit 5: mapped to macro for direction A
//                       bit 6: direct key mapping for direction B
//                       bit 7: mapped to macro for direction B
// pin io unsigned Encoders.#.channelApin [26];         // Channel A encoder pin
// pin io unsigned Encoders.#.channelBpin [26];         // Channel B encoder pin

typedef struct {
	// canonicaldevice interface pins
	hal_s32_t* count;      		// pointer for "pin out s32 encoder.#.count[29]"
	hal_float_t* position;		// pointer for "pin out float encoder.#.position[29]"
	hal_float_t* velocity;		// pointer for "pin out float encoder.#.velocity[29]"
	hal_bit_t* reset; 			// pointer for "pin in bit encoder.#.reset[29]"
	hal_bit_t* index_enable;	// pointer for "pin in bit encoder.#.index-enable[29]"

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

typedef struct {
	one_encoder_data_t encoder[29];
	hal_s32_t* encoder_deb_out; // pin out s32 deb.out;

} all_encoder_data_t;

static all_encoder_data_t* encoder_data = 0;

int PKEncoder_export_pins(char* prefix, long extra_arg, int id, int njoints, all_encoder_data_t* Encoder_data) {
	int r = 0;
	int j = 0;
	one_encoder_data_t* addr;
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: prefix: %s\n", __FILE__, __FUNCTION__, prefix);
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: id: %d\n", __FILE__, __FUNCTION__, id);
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: njoints: %d\n", __FILE__, __FUNCTION__, njoints);
	if (Encoder_data == NULL) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Encoder_data == NULL\n", __FILE__, __FUNCTION__);
		encoder_data = hal_malloc(sizeof(all_encoder_data_t));
		if (encoder_data == NULL) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PK_Encoders: all_encoder_data_t malloc failed\n");
			return -1;
		}
	}
	else {
		encoder_data = Encoder_data;
	}
	/*
	if (encoder_data == 0)
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "PK_Encoders: all_encoder_data_t malloc failed\n");
		return -1;
	}*/

	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.deb.out\n", __FILE__, __FUNCTION__, prefix);
	r = hal_pin_s32_newf(HAL_OUT, &(encoder_data->encoder_deb_out), id,
		"%s.encoder.deb.out", prefix);
	if (r != 0) {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.deb.out failed\n", __FILE__, __FUNCTION__, prefix);
		return r;
	}

	for (j = 0; j < (njoints); j++) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: encoder %d\n", __FILE__, __FUNCTION__, j);
		addr = &(encoder_data->encoder[j]);

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.count\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_s32_newf(HAL_OUT, &(addr->count), id,
			"%s.encoder.%01d.count", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.count failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.position\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_float_newf(HAL_OUT, &(addr->position), id,
			"%s.encoder.%01d.position", prefix, j);
		if (r != 0)
			return r;

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.velocity\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_float_newf(HAL_OUT, &(addr->velocity), id,
			"%s.encoder.%01d.velocity", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.velocity failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.reset\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_bit_newf(HAL_IN, &(addr->reset), id,
			"%s.encoder.%01d.reset", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.reset failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.index-enable\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_bit_newf(HAL_IN, &(addr->index_enable), id,
			"%s.encoder.%01d.index-enable", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.index-enable failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

	}
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: return: %d\n", __FILE__, __FUNCTION__, r);
	return r;
}

int PKEncoder_export_params(char* prefix, long extra_arg, int id, int njoints) {
	int r = 0;
	int j = 0;
	one_encoder_data_t* addr;
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: prefix: %s\n", __FILE__, __FUNCTION__, prefix);
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: id: %d\n", __FILE__, __FUNCTION__, id);
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: njoints: %d\n", __FILE__, __FUNCTION__, njoints);

	/*
	if (encoder_data == 0)
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "PK_Encoders: all_encoder_data_t malloc failed\n");
		return -1;
	}*/



	for (j = 0; j < (njoints); j++) {
		addr = &(encoder_data->encoder[j]);

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.scale\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_float_newf(HAL_RW, &(addr->scale), id, "%s.encoder.%01d.scale", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.scale failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.encoderOptions\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_u32_newf(HAL_RW, &(addr->encoderOptions), id, "%s.encoder.%01d.encoderOptions", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.encoderOptions failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.channelApin\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_u32_newf(HAL_RW, &(addr->channelApin), id, "%s.encoder.%01d.channelApin", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.channelApin failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.channelBpin\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_u32_newf(HAL_RW, &(addr->channelBpin), id, "%s.encoder.%01d.channelBpin", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.channelBpin failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}
	}
	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: return: %d\n", __FILE__, __FUNCTION__, r);
	return r;
}

bool initEncodersDone = 0;
bool EncoderValuesGet = false;
extern unsigned int sleepdur;
// bool DoEncoders = true;
// unsigned int  sleepdur = 1000;
// bool  use_sleepdur1 = true;
// unsigned int  sleepdur1 = 1000;
// unsigned int  sleepdur2 = 1000;

void PKEncoder_Update(sPoKeysDevice* dev) {

	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_iBasicEncoderCount = %d\n", __FILE__, __FUNCTION__, dev->info.iBasicEncoderCount);
	if (dev->info.iBasicEncoderCount > 0) {
		*(encoder_data->encoder_deb_out) = 216;
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev)\n", __FILE__, __FUNCTION__);
		if (PK_EncoderValuesGet(dev) == PK_OK) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev) OK\n", __FILE__, __FUNCTION__);
			usleep(sleepdur);
			*(encoder_data->encoder_deb_out) = 217;
			EncoderValuesGet = true;
			bool resetEncoders = false;

			resetEncoders = 0;
			double Scale = 1;
			/* known issue: since update to Bullseye & Lcnc 2.9 it hangs here*/
			for (int i = 0; i < dev->info.iBasicEncoderCount; i++) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read encoder %d\n", __FILE__, __FUNCTION__, i);
				*(encoder_data->encoder_deb_out) = 218 * 100 + i;
				// encoder_count(i) = dev->Encoders[i].encoderValue;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: encoder %d count = %d\n", __FILE__, __FUNCTION__, i, dev->Encoders[i].encoderValue);
				*(encoder_data->encoder[i].count) = dev->Encoders[i].encoderValue;
				*(encoder_data->encoder_deb_out) = 2190;
				// encoder_position(i) = dev->Encoders[i].encoderValue * encoder_scale(i);
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: encoder %d scale = %f\n", __FILE__, __FUNCTION__, i, (encoder_data->encoder[i].scale));
				Scale = (encoder_data->encoder[i].scale);
				*(encoder_data->encoder_deb_out) = 2191;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: encoder %d position = %d\n", __FILE__, __FUNCTION__, i, dev->Encoders[i].encoderValue * Scale);
				*(encoder_data->encoder[i].position) = dev->Encoders[i].encoderValue * Scale;
				*(encoder_data->encoder_deb_out) = 2201;
				if ((*(encoder_data->encoder[i].reset) != 0) || (initEncodersDone == false)) {
					*(encoder_data->encoder_deb_out) = 2212;
					dev->Encoders[i].encoderValue = 0;
					*(encoder_data->encoder_deb_out) = 2213;
					resetEncoders = true;
					*(encoder_data->encoder_deb_out) = 2214;
				}
				*(encoder_data->encoder_deb_out) = 2215;
				usleep(sleepdur);
			}

			/*
			known issue: since update to Bullseye & Lcnc 2.9 it hangs here*/
			*(encoder_data->encoder_deb_out) = 219;
			if (dev->info.iUltraFastEncoders) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read UltraFastEncoders\n", __FILE__, __FUNCTION__);
				usleep(sleepdur);
				*(encoder_data->encoder_deb_out) = 220;

				for (int i = dev->info.iBasicEncoderCount; i < (dev->info.iBasicEncoderCount + dev->info.iUltraFastEncoders); i++) {
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: UltraFastEncoders %d\n", __FILE__, __FUNCTION__, i);
					*(encoder_data->encoder_deb_out) = 221;
					*(encoder_data->encoder[i].count) = dev->Encoders[i].encoderValue;
					*(encoder_data->encoder[i].position) = dev->Encoders[i].encoderValue * (encoder_data->encoder[i].scale);
					if ((encoder_data->encoder[i].reset != 0) || (initEncodersDone == false)) {
						dev->Encoders[i].encoderValue = 0;
						resetEncoders = true;
					}
					usleep(sleepdur);
				}
			}
			/*
			if (dev->info.iFastEncoders)
			{
				for (i = dev->info.iBasicEncoderCount+ dev->info.iUltraFastEncoders; i < (dev->info.iBasicEncoderCount + dev->info.iUltraFastEncoders + dev->info.iFastEncoders ); i++)
				{
					encoder_count(i) = dev->Encoders[i].encoderValue;
				}
			}*/
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: resetEncoders = %d\n", __FILE__, __FUNCTION__, resetEncoders);
			if (resetEncoders == true) {
				*(encoder_data->encoder_deb_out) = 140;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesSet(dev)\n", __FILE__, __FUNCTION__);
				if (PK_EncoderValuesSet(dev) == PK_OK) {
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesSet(dev) OK\n", __FILE__, __FUNCTION__);
					usleep(sleepdur);
					resetEncoders = false;
					*(encoder_data->encoder_deb_out) = 141;
					initEncodersDone = true;
				}
				*(encoder_data->encoder_deb_out) = 142;
			}
		}
	}
}

int PKEncoder_init(int id,
	sPoKeysDevice* dev) {
	// dev = device;
	//return makepins(id, 26);
}

void PKEncoder_Setup(sPoKeysDevice* dev) {

}

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "pin:encoder.#.count:s32:29:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.position:float:29:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.velocity:float:29:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.reset:bit:29:in::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.index-enable:bit:29:in::None:None");
MODULE_INFO(linuxcnc, "param:encoder.#.scale:float:29:rw:The scale factor used to convert counts to position units. It is in “counts per position unit”:None:None");
#endif // MODULE_INFO

// EXPORT_SYMBOL(PKEncoder_init);
// EXPORT_SYMBOL(PKEncoder_Update);
// EXPORT_SYMBOL(write_PKEncoder_out_pins);
// EXPORT_SYMBOL(read_PKEncoder_in_pins);
// EXPORT_SYMBOL(update_PKEncoder_params);