

#include "PoKeysLib.h"
#include "rtapi.h"
//#include "rtapi_app.h"
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
//pin out unsigned Encoders.#.encoderValue [26];       // Encoder current value
//pin io unsigned Encoders.#.encoderOptions [26];      // Encoder options -    bit 0: enable encoder
													//                      bit 1: 4x sampling
													//                      bit 2: 2x sampling
													//                      bit 3: reserved
													//                      bit 4: direct key mapping for direction A
													//                      bit 5: mapped to macro for direction A
													//                      bit 6: direct key mapping for direction B
													//                      bit 7: mapped to macro for direction B
//pin io unsigned Encoders.#.channelApin [26];         // Channel A encoder pin
//pin io unsigned Encoders.#.channelBpin [26];         // Channel B encoder pin

// data for per-joint homing-specific hal pins:
typedef struct
{
	// canonicaldevice interface pins
    hal_s32_t *count;      		// pointer for "pin out s32 encoder.#.count[29]"
    hal_float_t *position;		// pointer for "pin out float encoder.#.position[29]"
    hal_float_t *velocity;		// pointer for "pin out float encoder.#.velocity[29]"
    hal_bit_t *reset; 			// pointer for "pin in bit encoder.#.reset[29]"
	hal_bit_t *index_enable;	// pointer for "pin in bit encoder.#.index-enable[29]"

	hal_float_t *scale;			// pointer for "param rw float encoder.#.scale[29]  "The scale factor used to convert counts to position units. It is in “counts per position unit”""

    // Custom parameters for communication with PoKeys
    hal_u32_t *encoderOptions;  // pointer for "pin io unsigned Encoders.#.encoderOptions [26]"		
													// Encoder options -    bit 0: enable encoder
													//                      bit 1: 4x sampling
													//                      bit 2: 2x sampling
													//                      bit 3: reserved
													//                      bit 4: direct key mapping for direction A
													//                      bit 5: mapped to macro for direction A
													//                      bit 6: direct key mapping for direction B
													//                      bit 7: mapped to macro for direction B
    hal_u32_t *channelApin; 	// pointer for "pin io unsigned Encoders.#.channelApin [26]"         // Channel A encoder pin"
	hal_u32_t *channelBpin; 	// pointer for "pin io unsigned Encoders.#.channelBpin [26]"         // Channel B encoder pin"

} one_encoder_data_t;

typedef struct
{
    one_encoder_data_t encoder[29];
	hal_s32_t *encoder_deb_out; // pin out s32 deb.out;

} all_encoder_data_t;

static all_encoder_data_t *encoder_data = 0;

static int makepins(int id, int njoints)
{
    // home_pins needed to work with configs expecting them:
    int jno, retval;
    one_encoder_data_t *addr;

    encoder_data = hal_malloc(sizeof(all_encoder_data_t));
    if (encoder_data == 0)
    {
        rtapi_print_msg(RTAPI_MSG_DBG, "PK_Encoders: all_encoder_data_t malloc failed\n");
        return -1;
    }

    retval = 0;
	retval += hal_pin_s32_newf(HAL_OUT, &(encoder_data->encoder_deb_out), id,"encoder.deb.out", jno); // create pin for "pin out s32 encoder.#.count[29]"
		
    for (jno = 0; jno < njoints; jno++)
    {
        addr = &(encoder_data->encoder[jno]);

        retval += hal_pin_s32_newf(HAL_OUT, &(addr->count), id,"encoder.%d.count", jno); // create pin for "pin out s32 encoder.#.count[29]"
		retval += hal_pin_float_newf(HAL_OUT, &(addr->position), id,"encoder.%d.position", jno); // create pin for "pin out float encoder.#.position[29]"
		retval += hal_pin_float_newf(HAL_OUT, &(addr->velocity), id,"encoder.%d.velocity", jno); // create pin for "pin out float encoder.#.velocity[29]"
		retval += hal_pin_bit_newf(HAL_IN, &(addr->reset), id,"encoder.%d.reset", jno); // create pin for "pin in bit encoder.#.reset[29]"
		retval += hal_pin_bit_newf(HAL_IO, &(addr->index_enable), id,"encoder.%d.index-enable", jno); // create pin for "pin in bit encoder.#.index-enable[29]"
		
	//	retval += hal_param_float_newf(HAL_RW, &(addr->scale), id,"encoder.%d.scale", jno); // create param for "param rw float encoder.#.scale[29]  "The scale factor used to convert counts to position units. It is in “counts per position unit”""

		retval += hal_pin_u32_newf(HAL_IO, &(addr->encoderOptions), id,"encoder.%d.encoderOptions", jno); // create pin for "pin io unsigned Encoders.#.encoderOptions [26]"		
													// Encoder options -    bit 0: enable encoder
													//                      bit 1: 4x sampling
													//                      bit 2: 2x sampling
													//                      bit 3: reserved
													//                      bit 4: direct key mapping for direction A
													//                      bit 5: mapped to macro for direction A
													//                      bit 6: direct key mapping for direction B
													//                      bit 7: mapped to macro for direction B
		retval += hal_pin_u32_newf(HAL_IO, &(addr->channelApin), id,"encoder.%d.channelApin", jno); // create pin for "pin io unsigned Encoders.#.channelApin [26]"         // Channel A encoder pin"
		retval += hal_pin_u32_newf(HAL_IO, &(addr->channelBpin), id,"encoder.%d.channelBpin", jno); // create pin for "pin io unsigned Encoders.#.channelBpin [26]"         // Channel B encoder pin"
    }
    return retval;
}



bool initEncodersDone = 0;
bool EncoderValuesGet = false; 

//bool DoEncoders = true;
//unsigned int  sleepdur = 1000;
//bool  use_sleepdur1 = true;
//unsigned int  sleepdur1 = 1000;
//unsigned int  sleepdur2 = 1000;


void PKEncoder_Update(sPoKeysDevice* dev)
{

			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_iBasicEncoderCount = %d\n", __FILE__, __FUNCTION__, dev->info.iBasicEncoderCount);
			if (dev->info.iBasicEncoderCount > 0)
			{
				*(encoder_data->encoder_deb_out) = 216;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev)\n", __FILE__, __FUNCTION__);
				if (PK_EncoderValuesGet(dev) == PK_OK)
				{
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev) OK\n", __FILE__, __FUNCTION__);
					//usleep(sleepdur);
					*(encoder_data->encoder_deb_out) = 217;
					EncoderValuesGet = true;
					bool resetEncoders = false;

					resetEncoders = 0;
					
					/* known issue: since update to Bullseye & Lcnc 2.9 it hangs here*/
					for (int i = 0; i < dev->info.iBasicEncoderCount; i++)
					{
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read encoder %d\n", __FILE__, __FUNCTION__, i);
						*(encoder_data->encoder_deb_out) = 218*100+i;
						//encoder_count(i) = dev->Encoders[i].encoderValue;
						*(encoder_data->encoder[i].count) = dev->Encoders[i].encoderValue;
						*(encoder_data->encoder_deb_out) = 2180;
						//encoder_position(i) = dev->Encoders[i].encoderValue * encoder_scale(i);
						*(encoder_data->encoder[i].position) = dev->Encoders[i].encoderValue * *(encoder_data->encoder[i].scale);
						*(encoder_data->encoder_deb_out) = 2181;
						if ((*(encoder_data->encoder[i].reset) != 0) || (initEncodersDone == false))
						{
							*(encoder_data->encoder_deb_out) = 2182;
							dev->Encoders[i].encoderValue = 0;
							*(encoder_data->encoder_deb_out) = 2183;
							resetEncoders = true;
							*(encoder_data->encoder_deb_out) = 2184;
						}
						*(encoder_data->encoder_deb_out) = 2185;
						//usleep(sleepdur);
					}
					
					/*
					known issue: since update to Bullseye & Lcnc 2.9 it hangs here*/
					*(encoder_data->encoder_deb_out) = 219;
					if (dev->info.iUltraFastEncoders)
					{
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read UltraFastEncoders\n", __FILE__, __FUNCTION__);
						//usleep(sleepdur);
						*(encoder_data->encoder_deb_out) = 220;
						
						for (int i = dev->info.iBasicEncoderCount; i < (dev->info.iBasicEncoderCount + dev->info.iUltraFastEncoders); i++)
						{
							rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: UltraFastEncoders %d\n", __FILE__, __FUNCTION__, i);
							*(encoder_data->encoder_deb_out) = 221;
							*(encoder_data->encoder[i].count) = dev->Encoders[i].encoderValue;
							*(encoder_data->encoder[i].position) = dev->Encoders[i].encoderValue * *(encoder_data->encoder[i].scale);
							if ((encoder_data->encoder[i].reset != 0) || (initEncodersDone == false))
							{
								dev->Encoders[i].encoderValue = 0;
								resetEncoders = true;
							}
							//usleep(sleepdur);
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
					if (resetEncoders == true)
					{
						*(encoder_data->encoder_deb_out) = 140;
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesSet(dev)\n", __FILE__, __FUNCTION__);
						if (PK_EncoderValuesSet(dev) == PK_OK)
						{
							rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesSet(dev) OK\n", __FILE__, __FUNCTION__);
							//usleep(sleepdur);
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
                sPoKeysDevice * dev)
{
   // dev = device;
    return makepins(id, 26);
}


EXPORT_SYMBOL(PKEncoder_init);
EXPORT_SYMBOL(PKEncoder_Update);
//EXPORT_SYMBOL(write_PKEncoder_out_pins);
//EXPORT_SYMBOL(read_PKEncoder_in_pins);
//EXPORT_SYMBOL(update_PKEncoder_params);