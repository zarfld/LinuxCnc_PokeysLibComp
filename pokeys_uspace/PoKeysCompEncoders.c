
/**
 * @file
 * 
*/
#include "PoKeysLib.h"
#include "PoKeysComp.h"
#include "rtapi.h"
// #include "rtapi_app.h"
#include "hal.h"
#include "stdio.h"

#ifdef ULAPI
extern unsigned int sleepdur;
#endif
extern bool ApplyIniSettings;

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

/**
 * @brief Pointer to the global encoder data structure.
 *
 * This static pointer is initialized during setup and used by all encoder-related
 * functions to access HAL pins and parameters for all 29 encoder channels.
 *
 * The structure it points to contains:
 * - Canonical encoder interface pins (count, position, velocity, reset, index-enable)
 * - PoKeys-specific configuration options (pin mapping, sampling mode, etc.)
 * - A debug output pin for diagnostics
 *
 * @see all_encoder_data_t
 * @see PKENC_export_encoders()
 * @see PKENC_Update()
 */
static all_encoder_data_t *encoder_data = 0;

/**
 * @brief Exports HAL encoder pins for the PoKeys device.
 *
 * This function creates and registers all necessary HAL pins for a specified number of encoders 
 * based on the given joint count. It assigns pointers to the internal `encoder_data` structure 
 * for later use during runtime updates.
 *
 * The following HAL pins are exported per encoder channel:
 * - `encoder.N.count` (s32, output): Current encoder count
 * - `encoder.N.position` (float, output): Calculated position
 * - `encoder.N.velocity` (float, output): Calculated velocity
 * - `encoder.N.reset` (bit, input): Resets encoder count
 * - `encoder.N.index-enable` (bit, input): Enables index pulse processing
 *
 * Additionally, the following debug pin is exported:
 * - `encoder.deb.out` (s32, output): Debug value for diagnostics
 *
 * If `Encoder_data` is `NULL`, memory is allocated via `hal_malloc`.
 *
 * @param[in] prefix       HAL prefix string used for naming the exported pins
 * @param[in] extra_arg    Reserved, not used
 * @param[in] id           HAL component ID
 * @param[in] njoints      Number of encoders (typically equal to number of joints)
 * @param[in] Encoder_data Pointer to encoder data structure (can be NULL to auto-allocate)
 * @return 0 on success, or negative HAL error code on failure
 *
 * @note This function must be called during HAL component initialization.
 * @see PKENC_Update()
 * @see one_encoder_data_t
 * @see all_encoder_data_t
 */

int PKEncoder_export_pins(char *prefix, long extra_arg, int id, int njoints, all_encoder_data_t *Encoder_data) {
    int r = 0;
    int j = 0;
    one_encoder_data_t *addr;
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
    } else {
        encoder_data = Encoder_data;
    }
    /*
	if (encoder_data == 0)
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "PK_Encoders: all_encoder_data_t malloc failed\n");
		return -1;
	}*/

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.deb.out\n", __FILE__, __FUNCTION__, prefix);
    r = hal_pin_s32_newf(HAL_OUT, &(encoder_data->encoder_deb_out), id, "%s.encoder.deb.out", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.deb.out failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    for (j = 0; j < (njoints); j++) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: encoder %d\n", __FILE__, __FUNCTION__, j);
        addr = &(encoder_data->encoder[j]);

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.count\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_s32_newf(HAL_OUT, &(addr->count), id, "%s.encoder.%01d.count", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.count failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.position\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_float_newf(HAL_OUT, &(addr->position), id, "%s.encoder.%01d.position", prefix, j);
        if (r != 0)
            return r;

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.velocity\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_float_newf(HAL_OUT, &(addr->velocity), id, "%s.encoder.%01d.velocity", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.velocity failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.reset\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_bit_newf(HAL_IN, &(addr->reset), id, "%s.encoder.%01d.reset", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.reset failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.index-enable\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_bit_newf(HAL_IN, &(addr->index_enable), id, "%s.encoder.%01d.index-enable", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.index-enable failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }
    }
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: return: %d\n", __FILE__, __FUNCTION__, r);
    return r;
}

/**
 * @brief Exports HAL parameters related to encoder configuration.
 *
 * This function registers additional HAL parameters for each encoder channel,
 * such as scale factor and PoKeys-specific encoder settings like pin assignments
 * and options. These parameters allow runtime configuration and status feedback.
 *
 * For each encoder (up to `njoints`), the following HAL parameters are created:
 * - `encoder.N.scale` (float, rw): Scale factor used to convert counts to position units
 * - `Encoders.N.encoderOptions` (u32, rw): Bitmask defining encoder behavior
 * - `Encoders.N.channelApin` (u32, rw): Assigned PoKeys pin for channel A
 * - `Encoders.N.channelBpin` (u32, rw): Assigned PoKeys pin for channel B
 *
 * @param[in] prefix   HAL prefix string used for naming the exported parameters
 * @param[in] extra_arg Reserved, not used
 * @param[in] id       HAL component ID
 * @param[in] njoints  Number of encoders (typically equal to number of joints)
 * @return 0 on success, or a negative HAL error code on failure
 *
 * @note This function should be called during component initialization, after memory allocation
 *       for `encoder_data` has been completed.
 *
 * @see PKEncoder_export_pins()
 * @see one_encoder_data_t
 */
int PKEncoder_export_params(char *prefix, long extra_arg, int id, int njoints) {
    int r = 0;
    int j = 0;
    one_encoder_data_t *addr;
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

/**
 * @brief Updates encoder data from the PoKeys device and writes values to HAL pins.
 *
 * This function retrieves encoder values (basic and ultra-fast) from the PoKeys device
 * and updates the corresponding HAL pins for count and position. It also checks if the 
 * encoder should be reset (based on HAL reset pin or during initialization).
 * If reset is requested, it sets the encoder value on the device to zero.
 *
 * The following values are updated for each encoder:
 * - `encoder.#.count`         – the raw encoder count value
 * - `encoder.#.position`      – the scaled position based on count and scale parameter
 * - `encoder.#.velocity`      – currently not set (placeholder)
 *
 * If `reset` pin is high or this is the first update (`initEncodersDone == false`), 
 * the encoder is reset on the device using `PK_EncoderValuesSet()`.
 *
 * Debug values are written to `encoder_data->encoder_deb_out` to help trace progress or hangs.
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 *
 * @note The function supports both basic and ultra-fast encoders.
 *       Fast encoders are not handled in the current implementation (commented out).
 *
 * @see PK_EncoderValuesGet()
 * @see PK_EncoderValuesSet()
 * @see one_encoder_data_t
 */
void PKEncoder_Update(sPoKeysDevice *dev) {

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_iBasicEncoderCount = %d\n", __FILE__, __FUNCTION__, dev->info.iBasicEncoderCount);
    if (dev->info.iBasicEncoderCount > 0) {
        *(encoder_data->encoder_deb_out) = 216;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev)\n", __FILE__, __FUNCTION__);
        if (PK_EncoderValuesGet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderValuesGet(dev) OK\n", __FILE__, __FUNCTION__);
#ifdef ULAPI
            usleep(sleepdur);
#endif
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
#ifdef ULAPI
                usleep(sleepdur);
#endif
            }

            /*
			known issue: since update to Bullseye & Lcnc 2.9 it hangs here*/
            *(encoder_data->encoder_deb_out) = 219;
            if (dev->info.iUltraFastEncoders) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read UltraFastEncoders\n", __FILE__, __FUNCTION__);
#ifdef ULAPI
                usleep(sleepdur);
#endif
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
#ifdef ULAPI
                    usleep(sleepdur);
#endif
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
#ifdef ULAPI
                    usleep(sleepdur);
#endif
                    resetEncoders = false;
                    *(encoder_data->encoder_deb_out) = 141;
                    initEncodersDone = true;
                }
                *(encoder_data->encoder_deb_out) = 142;
            }
        }
    }
}

/**
 * @brief
 * 
 */
int PKEncoder_init(int id, sPoKeysDevice *dev) {
    // dev = device;
    //return makepins(id, 26);
}

/**
 * @brief Placeholder function to perform encoder-specific setup actions.
 *
 * This function is intended to configure encoder-related parameters or device settings 
 * before starting regular encoder operation. Currently, it is not implemented.
 *
 * You may extend this function to:
 * - Configure encoder sampling modes (1x, 2x, 4x)
 * - Assign pins for channel A/B
 * - Set scaling or filtering options
 * - Apply configuration stored from INI
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 */
void PKEncoder_Setup(sPoKeysDevice *dev) {
}

/**
 * @brief Reads encoder-related settings from the INI configuration file.
 *
 * This function is intended to load encoder configuration values (e.g., scale, pin assignments,
 * inversion flags, or other options) from an INI file section, typically used for persisting
 * user-defined settings between sessions.
 *
 * Currently, this function is a placeholder and does not perform any operation.
 *
 * You may extend it to:
 * - Load encoder scale factors
 * - Load encoder pin mappings (channel A/B)
 * - Load encoder mode or options
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 */
void PKEncoder_ReadIniFile(sPoKeysDevice *dev) {
}

/**
 * @brief Writes encoder-related configuration to the INI file.
 *
 * This function is intended to save current encoder configuration values (e.g., scale, pin assignments,
 * inversion flags, or other options) into an INI file section. This enables restoring consistent
 * settings across restarts or deployments.
 *
 * Currently, this function is a placeholder and does not perform any operation.
 *
 * You may extend it to:
 * - Write encoder scale factors
 * - Write encoder pin mappings (channel A/B)
 * - Write encoder options or flags
 *
 * @param[in] dev Pointer to the initialized PoKeys device structure
 */
void PKEncoder_WriteIniFile(sPoKeysDevice *dev) {
}

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "pin:encoder.#.count:s32:29:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.position:float:29:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.velocity:float:29:out::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.reset:bit:29:in::None:None");
MODULE_INFO(linuxcnc, "pin:encoder.#.index-enable:bit:29:in::None:None");
MODULE_INFO(linuxcnc, "param:encoder.#.scale:float:29:rw:The scale factor used to convert counts "
                      "to position units. It is in “counts per position unit”:None:None");
#endif // MODULE_INFO

// EXPORT_SYMBOL(PKEncoder_init);
// EXPORT_SYMBOL(PKEncoder_Update);
// EXPORT_SYMBOL(write_PKEncoder_out_pins);
// EXPORT_SYMBOL(read_PKEncoder_in_pins);
// EXPORT_SYMBOL(update_PKEncoder_params);