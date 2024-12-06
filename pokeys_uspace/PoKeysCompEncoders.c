

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
	hal_bit_t *index-enable;	// pointer for "pin in bit encoder.#.index-enable[29]"

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

} all_encoder_data_t;

static all_encoder_data_t *encoder_data = 0;

static int makepins(int id, int njoints)
{
    // home_pins needed to work with configs expecting them:
    int jno, retval;
    one_encoder_data_t *addr;

    joint_home_data = hal_malloc(sizeof(all_encoder_data_t));
    if (joint_home_data == 0)
    {
        rtapi_print_msg(RTAPI_MSG_DBG, "PK_Encoders: all_encoder_data_t malloc failed\n");
        return -1;
    }

    retval = 0;
    for (no = 0; jno < njoints; jno++)
    {
        addr = &(joint_home_data->jhd[jno]);

        retval += hal_pin_bit_newf(HAL_IN, &(addr->home_sw), id,"joint.%d.home-sw-in", jno);
        retval += hal_pin_bit_newf(HAL_OUT, &(addr->homing), id,"joint.%d.homing", jno);
        retval += hal_pin_bit_newf(HAL_OUT, &(addr->homed), id, "joint.%d.homed", jno);
        retval += hal_pin_s32_newf(HAL_OUT, &(addr->home_state), id,"joint.%d.home-state", jno);
        retval += hal_pin_bit_newf(HAL_IO, &(addr->index_enable), id,"joint.%d.index-enable", jno);

        // add custom hal pins here:
        retval += hal_pin_u32_newf(HAL_IN, &(addr->PEv2_AxesState), id,"joint.%d.PEv2.AxesState", jno);
        retval += hal_pin_u32_newf(HAL_OUT, &(addr->PEv2_AxesCommand), id,"joint.%d.PEv2.AxesCommand", jno);

    }
    return retval;
}



bool initEncodersDone = 0;
bool EncoderValuesGet = false; 

bool DoEncoders = true;

public void PKEncoder_Update(sPoKeysDevice* dev)
{

			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_iBasicEncoderCount = %d\n", __FILE__, __FUNCTION__, dev->info.iBasicEncoderCount);
			if (dev->info.iBasicEncoderCount && DoEncoders)
			{
				deb_out = 216;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev)\n", __FILE__, __FUNCTION__);
				if (PK_EncoderValuesGet(dev) == PK_OK)
				{
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev) OK\n", __FILE__, __FUNCTION__);
					usleep(sleepdur);
					deb_out = 217;
					EncoderValuesGet = true;
					bool resetEncoders = false;

					resetEncoders = 0;
					
					/* known issue: since update to Bullseye & Lcnc 2.9 it hangs here*/
					for (i = 0; i < dev->info.iBasicEncoderCount; i++)
					{
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read encoder %d\n", __FILE__, __FUNCTION__, i);
						deb_out = 218*100+i;
						encoder_count(i) = dev->Encoders[i].encoderValue;
						deb_out = 2180;
						encoder_position(i) = dev->Encoders[i].encoderValue * encoder_scale(i);
						deb_out = 2181;
						if ((encoder_reset(i) != 0) || (initEncodersDone == false))
						{
							deb_out = 2182;
							dev->Encoders[i].encoderValue = 0;
							deb_out = 2183;
							resetEncoders = true;
							deb_out = 2184;
						}
						deb_out = 2185;
						usleep(sleepdur);
					}
					
					/*
					known issue: since update to Bullseye & Lcnc 2.9 it hangs here*/
					deb_out = 219;
					if (dev->info.iUltraFastEncoders)
					{
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read UltraFastEncoders\n", __FILE__, __FUNCTION__);
						usleep(sleepdur);
						deb_out = 220;
						
						for (i = dev->info.iBasicEncoderCount; i < (dev->info.iBasicEncoderCount + dev->info.iUltraFastEncoders); i++)
						{
							rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: UltraFastEncoders %d\n", __FILE__, __FUNCTION__, i);
							deb_out = 221;
							encoder_count(i) = dev->Encoders[i].encoderValue;
							encoder_position(i) = dev->Encoders[i].encoderValue * encoder_scale(i);
							if ((encoder_reset(i) != 0) || (initEncodersDone == false))
							{
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
					if (resetEncoders == true)
					{
						deb_out = 140;
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesSet(dev)\n", __FILE__, __FUNCTION__);
						if (PK_EncoderValuesSet(dev) == PK_OK)
						{
							rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesSet(dev) OK\n", __FILE__, __FUNCTION__);
							usleep(sleepdur);
							resetEncoders = false;
							deb_out = 141;
							initEncodersDone = true;
						}
						deb_out = 142;
					}
					
				}
			}

}

int PKEncoder_init(int id,
                dsPoKeysDevice * device)
{
    dev = device;
    return makepins(id, device);
}

EXPORT_SYMBOL(PKEncoder_init);
EXPORT_SYMBOL(PKEncoder_Update);
EXPORT_SYMBOL(write_PKEncoder_out_pins);
EXPORT_SYMBOL(read_PKEncoder_in_pins);
EXPORT_SYMBOL(update_PKEncoder_params);