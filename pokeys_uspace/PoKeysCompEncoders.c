
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

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.enable\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_bit_newf(HAL_RW, &(addr->enable), id, "%s.encoder.%01d.enable", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.enable failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.4x_sampling\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_bit_newf(HAL_RW, &(addr->x4_sampling), id, "%s.encoder.%01d.4x_sampling", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.4x_sampling failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.2x_sampling\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_bit_newf(HAL_RW, &(addr->x2_sampling), id, "%s.encoder.%01d.2x_sampling", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.2x_sampling failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        /* prepared but it seems that there is no use for that in LinuxCnc
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.direct_key_mapping_A\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_bit_newf(HAL_RW, &(addr->direct_key_mapping_A), id, "%s.encoder.%01d.direct_key_mapping_A", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.direct_key_mapping_A failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.macro_mapping_A\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_bit_newf(HAL_RW, &(addr->macro_mapping_A), id, "%s.encoder.%01d.macro_mapping_A", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.macro_mapping_A failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.direct_key_mapping_B\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_bit_newf(HAL_RW, &(addr->direct_key_mapping_B), id, "%s.encoder.%01d.direct_key_mapping_B", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.direct_key_mapping_B failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.%01d.macro_mapping_B\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_bit_newf(HAL_RW, &(addr->macro_mapping_B), id, "%s.encoder.%01d.macro_mapping_B", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.%01d.macro_mapping_B failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
        */
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

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.encoder.FastEncoders.Options\n", __FILE__, __FUNCTION__, prefix);
    r = hal_param_u32_newf(HAL_RW, &(encoder_data->FastEncodersOptions), id, "%s.encoder.FastEncoders.Options", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.encoder.FastEncoders.Options failed\n", __FILE__, __FUNCTION__, prefix, j);
        return r;
    }



    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: return: %d\n", __FILE__, __FUNCTION__, r);
    return r;
}

bool initEncodersDone = 0;
bool EncoderValuesGet = false;

/**
 * @brief Updates encoder values and handles reset/index-enable logic.
 *
 * This function is called periodically to:
 * - Retrieve encoder counts from the PoKeys device via `PK_EncoderValuesGet()`
 * - Update corresponding HAL output pins:
 *   - `count`: raw encoder value
 *   - `position`: scaled encoder value (`count * scale`)
 * - Perform reset operations when one of the following conditions is met:
 *   - HAL pin `reset` is set to `TRUE`
 *   - HAL pin `index-enable` is set to `TRUE` (LinuxCNC-style homing)
 *   - The component is being initialized (`initEncodersDone == false`)
 *
 * When any reset condition is met, the corresponding encoder value is set to zero,
 * and `PK_EncoderValuesSet()` is used to push the updated values to the PoKeys device.
 *
 * The `index-enable` pin is automatically cleared after use to prevent repeated resets.
 *
 * Ultra-fast encoders (if present) are also updated and reset in the same pass.
 *
 * Debug status values are written to `encoder_deb_out` for traceability.
 *
 * @param dev Pointer to the PoKeys device structure.
 *
 * @note Uses `usleep()` if compiled with `ULAPI` to avoid overloading the USB bus.
 * @note All encoders are reset once at startup before the first completed update.
 * @note This logic does not currently include fast encoders unless they are mapped as basic/ultra-fast.
 *
 * @see PK_EncoderValuesGet
 * @see PK_EncoderValuesSet
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
                if ((*(encoder_data->encoder[i].reset) != 0) || (*(encoder_data->encoder[i].index_enable) != 0) || (initEncodersDone == false)) {
                    *(encoder_data->encoder_deb_out) = 2212;
                    dev->Encoders[i].encoderValue = 0;
                    *(encoder_data->encoder_deb_out) = 2213;
                    resetEncoders = true;
                    *(encoder_data->encoder_deb_out) = 2214;
                    if (*(encoder_data->encoder[i].index_enable) != 0) {
                        *(encoder_data->encoder[i].index_enable) = 0;
                    }
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
                    if ((encoder_data->encoder[i].reset != 0) || (*(encoder_data->encoder[i].index_enable) != 0) || (initEncodersDone == false)) {
                        dev->Encoders[i].encoderValue = 0;
                        resetEncoders = true;
                        if (*(encoder_data->encoder[i].index_enable) != 0) {
                            *(encoder_data->encoder[i].index_enable) = 0;
                        }
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
    // return makepins(id, 26);
}

/**
 * @brief Synchronizes encoder configuration between HAL and the PoKeys device.
 *
 * This function manages encoder settings for all supported encoder types:
 * - Basic encoders (PoKeys standard inputs)
 * - Fast encoders (3x special dual-input configurations)
 * - Ultra-fast encoder (1x high-speed input pair)
 *
 * For each encoder type, the following logic is applied:
 * - If `ApplyIniSettings == false`: configuration is read from the device and written to HAL parameters.
 * - If `ApplyIniSettings == true`: configuration is read from HAL parameters and applied to the device.
 *
 * @details
 * ### Basic Encoder Fields:
 * - `encoderOptions`: Enable, sampling mode (1x/2x/4x), key mapping, macro mapping
 * - `channelApin`, `channelBpin`: Pin assignment
 *
 * ### Fast Encoder Fields:
 * - `FastEncodersConfiguration`: Configuration set (CFG1 or CFG2)
 * - `FastEncodersOptions`: Inversion bits, disable 4x sampling
 * - HAL parameters: `encoderFastEnable`, `alternativeconfig`, `disable_4x_sampling`, `FastEncodersInvert[]`
 *
 * ### Ultra-Fast Encoder Fields:
 * - `UltraFastEncoderConfiguration`: Enable
 * - `UltraFastEncoderOptions`: Direction invert, signal mode, 4x sampling
 * - `UltraFastEncoderFilter`: Digital filter setting
 *
 * @note
 * - Configuration is only written to the PoKeys device if differences are detected.
 * - Final application of settings is triggered via `PK_EncoderConfigurationSet()` only if needed.
 *
 * @warning
 * Index reset for the ultra-fast encoder (byte 5 of command 0x1C) is currently **not supported**
 * in the PoKeysLib API (`PK_EncoderConfigurationSet()` and `PK_EncoderConfigurationGet()`).
 * This feature cannot be enabled or read back until the API is extended accordingly.
 *
 * @param dev Pointer to the PoKeys device structure (must be connected and initialized).
 *
 * @see PK_EncoderConfigurationGet
 * @see PK_EncoderConfigurationSet
 * @see sPoKeysDevice
 * @see all_encoder_data_t
 */
void PKEncoder_Setup(sPoKeysDevice *dev) {
    bool EncoderConfigurationSet = false;

    if (dev->info.iBasicEncoderCount == 0 && dev->info.iUltraFastEncoders == 0 && dev->info.iFastEncoders == 0) {
        // No encoders available
        return;
    }

    if (PK_EncoderConfigurationGet(dev) != PK_OK) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EncoderConfigurationGet failed\n", __FILE__, __FUNCTION__);
        return;
    }



    if (dev->info.iBasicEncoderCount > 0) {
        for (int i = 0; i < dev->info.iBasicEncoderCount; i++)
        { 
            encoder_data->encoder[i].encoderOptions = dev->Encoders[i].encoderOptions;

            hal_u32_t encoderOptions = 0;
            bool enable       = ((encoder_data->encoder[i].encoderOptions) & PK_ENCODER_OPTION_ENABLE)         ? 1 : 0;
            bool x4_sampling  = ((encoder_data->encoder[i].encoderOptions) & PK_ENCODER_OPTION_4X_SAMPLING)    ? 1 : 0;
            bool x2_sampling  = ((encoder_data->encoder[i].encoderOptions) & PK_ENCODER_OPTION_2X_SAMPLING)    ? 1 : 0;
            bool keymap_dirA  = ((encoder_data->encoder[i].encoderOptions) & PK_ENCODER_OPTION_KEYMAP_DIR_A)   ? 1 : 0;
            bool macro_dirA   = ((encoder_data->encoder[i].encoderOptions) & PK_ENCODER_OPTION_MACRO_DIR_A)    ? 1 : 0;
            bool keymap_dirB  = ((encoder_data->encoder[i].encoderOptions) & PK_ENCODER_OPTION_KEYMAP_DIR_B)   ? 1 : 0;
            bool macro_dirB   = ((encoder_data->encoder[i].encoderOptions) & PK_ENCODER_OPTION_MACRO_DIR_B)    ? 1 : 0;

            if (ApplyIniSettings == false || encoder_data->encoder[i].enable == 0) {
                encoder_data->encoder[i].enable = enable;
            }
            else if(encoder_data->encoder[i].enable != enable) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d enable = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].enable);
                enable = encoder_data->encoder[i].enable;
            }

            if (ApplyIniSettings == false || encoder_data->encoder[i].x4_sampling == 0) {
                encoder_data->encoder[i].x4_sampling = x4_sampling;
            }
            else if(encoder_data->encoder[i].x4_sampling != x4_sampling) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d x4_sampling = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].x4_sampling);
                x4_sampling = encoder_data->encoder[i].x4_sampling;
            }

            if (ApplyIniSettings == false || encoder_data->encoder[i].x2_sampling == 0) {
                encoder_data->encoder[i].x2_sampling = x2_sampling;
            }
            else if(encoder_data->encoder[i].x2_sampling != x2_sampling) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d x2_sampling = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].x2_sampling);
                x2_sampling = encoder_data->encoder[i].x2_sampling;
            }
       
            /*if (ApplyIniSettings == false || encoder_data->encoder[i].keymap_dirA == 0) {
                encoder_data->encoder[i].keymap_dirA = keymap_dirA;
            }
            else if(encoder_data->encoder[i].keymap_dirA != dev->Encoders[i].keymap_dirA) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d keymap_dirA = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].keymap_dirA);
                dev->Encoders[i].keymap_dirA = encoder_data->encoder[i].keymap_dirA;
            }

            if (ApplyIniSettings == false || encoder_data->encoder[i].macro_dirA == 0) {
                encoder_data->encoder[i].macro_dirA = macro_dirA;
            }
            else if(encoder_data->encoder[i].macro_dirA != macro_dirA) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d macro_dirA = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].macro_dirA);
                macro_dirA = encoder_data->encoder[i].macro_dirA;
            }
            if (ApplyIniSettings == false || encoder_data->encoder[i].keymap_dirB == 0) {
                encoder_data->encoder[i].keymap_dirB = keymap_dirB;
            }
            else if(encoder_data->encoder[i].keymap_dirB != keymap_dirB) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d keymap_dirB = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].keymap_dirB);
                keymap_dirB = encoder_data->encoder[i].keymap_dirB;
            }
            if (ApplyIniSettings == false || encoder_data->encoder[i].macro_dirB == 0) {
                encoder_data->encoder[i].macro_dirB = macro_dirB;
            }
            else if(encoder_data->encoder[i].macro_dirB != macro_dirB) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d macro_dirB = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].macro_dirB);
                macro_dirB = encoder_data->encoder[i].macro_dirB;
            }*/
            
            if (enable)       encoderOptions |= PK_ENCODER_OPTION_ENABLE;
            if (x4_sampling)  encoderOptions |= PK_ENCODER_OPTION_4X_SAMPLING;
            if (x2_sampling)  encoderOptions |= PK_ENCODER_OPTION_2X_SAMPLING;
            if (keymap_dirA)  encoderOptions |= PK_ENCODER_OPTION_KEYMAP_DIR_A;
            if (macro_dirA)   encoderOptions |= PK_ENCODER_OPTION_MACRO_DIR_A;
            if (keymap_dirB)  encoderOptions |= PK_ENCODER_OPTION_KEYMAP_DIR_B;
            if (macro_dirB)   encoderOptions |= PK_ENCODER_OPTION_MACRO_DIR_B;

            if (ApplyIniSettings && dev->Encoders[i].encoderOptions != encoderOptions) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d encoderOptions = %d\n", __FILE__, __FUNCTION__, i, dev->Encoders[i].encoderOptions);
                dev->Encoders[i].encoderOptions = encoderOptions;
                EncoderConfigurationSet = true;
            }     

            if (ApplyIniSettings == false || encoder_data->encoder[i].channelApin == 0) {
                encoder_data->encoder[i].channelApin = dev->Encoders[i].channelApin ;
            }
            else if (encoder_data->encoder[i].channelApin != dev->Encoders[i].channelApin){
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d channelApin = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].channelApin);
                dev->Encoders[i].channelApin = encoder_data->encoder[i].channelApin;
                EncoderConfigurationSet = true;
            }

            if (ApplyIniSettings == false || encoder_data->encoder[i].channelBpin == 0) {
                encoder_data->encoder[i].channelBpin = dev->Encoders[i].channelBpin;
            }
            else if (encoder_data->encoder[i].channelBpin != dev->Encoders[i].channelBpin){
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d channelBpin = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].channelBpin);
                dev->Encoders[i].channelBpin = encoder_data->encoder[i].channelBpin;
                EncoderConfigurationSet = true;
            }

            /* prepared not existing - in current tasks of Linuxcnc not in use
            if (ApplyIniSettings == false || encoder_data->encoder[i].dirAkeyCode == 0) {
                encoder_data->encoder[i].dirAkeyCode = dev->Encoders[i].dirAkeyCode;
            }
            else if (encoder_data->encoder[i].dirAkeyCode != dev->Encoders[i].dirAkeyCode){
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d dirAkeyCode = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].dirAkeyCode);
                dev->Encoders[i].dirAkeyCode = encoder_data->encoder[i].dirAkeyCode;
                EncoderConfigurationSet = true;
            }

            if (ApplyIniSettings == false || encoder_data->encoder[i].dirAkeyModifier == 0) {
                encoder_data->encoder[i].dirAkeyModifier = dev->Encoders[i].dirAkeyModifier;
            }
            else if (encoder_data->encoder[i].dirAkeyModifier != dev->Encoders[i].dirAkeyModifier){
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d dirAkeyModifier = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].dirAkeyModifier);
                dev->Encoders[i].dirAkeyModifier = encoder_data->encoder[i].dirAkeyModifier;
                EncoderConfigurationSet = true;
            }

            if (ApplyIniSettings == false || encoder_data->encoder[i].dirBkeyCode == 0) {
                encoder_data->encoder[i].dirBkeyCode = dev->Encoders[i].dirBkeyCode;
            }
            else if (encoder_data->encoder[i].dirBkeyCode != dev->Encoders[i].dirBkeyCode){
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d dirBkeyCode = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].dirBkeyCode);
                dev->Encoders[i].dirBkeyCode = encoder_data->encoder[i].dirBkeyCode;
                EncoderConfigurationSet = true;
            }

            if (ApplyIniSettings == false || encoder_data->encoder[i].keymap_dirB == 0) {
                encoder_data->encoder[i].keymap_dirB = dev->Encoders[i].dirBkeyModifier;
            }
            else if (encoder_data->encoder[i].keymap_dirB != dev->Encoders[i].dirBkeyModifier){
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: encoder %d dirBkeyModifier = %d\n", __FILE__, __FUNCTION__, i, encoder_data->encoder[i].dirBkeyModifier);
                dev->Encoders[i].dirBkeyModifier = encoder_data->encoder[i].keymap_dirB;
                EncoderConfigurationSet = true;
            }*/
        }
    }

    if (dev->info.iFastEncoders) {
        uint8_t config = 0;
        uint8_t options = 0;
    
        encoder_data->FastEncodersOptions = dev->FastEncodersOptions;
        encoder_data->FastEncodersConfiguration = dev->FastEncodersConfiguration;

        if (encoder_data->FastEncodersConfiguration == 0) {
            encoder_data->encoderFastEnable = 0;
        }
        else {

            encoder_data->encoderFastEnable = 1;

            if (dev->DeviceData.DeviceType != PK_DeviceID_55v1 || dev->DeviceData.DeviceType != PK_DeviceID_55v2 || dev->DeviceData.DeviceType != PK_DeviceID_55v3) {
                /*There are two different fast encoders configurations. On newer PoKeys56 and PoKeys57 series devices, only second configuration can be selected.
                */
                encoder_data->alternativeconfig = 0;
            }
            else if ((encoder_data->FastEncodersConfiguration & 0x0F) == PK_FASTENCODER_CONF_CFG1) {
                //Configuration 1: pins 1-2 as encoder 1, pins 3-4 as encoder 2, pins 15-16 as encoder 3

                encoder_data->alternativeconfig = 1;
            }
            else if ((encoder_data->FastEncodersConfiguration & 0x0F) == PK_FASTENCODER_CONF_CFG2) {
                //Configuration 2: pins 1-2 as encoder 1, pins 5-6 as encoder 2, pins 15-16 as encoder 3

                encoder_data->alternativeconfig = 0;
            }
            else {
                encoder_data->alternativeconfig = 0;
                
            }
        }
        //bool encoderFastEnable = ((encoder_data->FastEncodersOptions) & PK_FASTENCODER_ENABLE)         ? 1 : 0;
        bool encoderFastInvertE1 = ((encoder_data->FastEncodersOptions) & PK_FASTENCODER_INVERT_E1)     ? 1 : 0;
        bool encoderFastInvertE2 = ((encoder_data->FastEncodersOptions) & PK_FASTENCODER_INVERT_E2)     ? 1 : 0;
        bool encoderFastInvertE3 = ((encoder_data->FastEncodersOptions) & PK_FASTENCODER_INVERT_E3)     ? 1 : 0;
        bool encoderFastDisable4xSampling = ((encoder_data->FastEncodersOptions) & PK_FASTENCODER_DISABLE_4X_SAMPLING) ? 1 : 0;

        if (ApplyIniSettings == false ) {
           // encoder_data->encoderFastEnable = encoderFastEnable;
            encoder_data->FastEncodersInvert[0] = encoderFastInvertE1;
            encoder_data->FastEncodersInvert[1] = encoderFastInvertE2;
            encoder_data->FastEncodersInvert[2] = encoderFastInvertE3;
            encoder_data->disable_4x_sampling = encoderFastDisable4xSampling;
        }
        else{

            if(encoder_data->encoderFastEnable != encoderFastEnable) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FastEncoder enable = %d\n", __FILE__, __FUNCTION__, encoder_data->encoderFastEnable);
                encoderFastEnable = encoder_data->encoderFastEnable;
                EncoderConfigurationSet = true;
            }

            if(encoder_data->FastEncodersInvert[0] != encoderFastInvertE1) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FastEncoder E1 invert = %d\n", __FILE__, __FUNCTION__, encoder_data->FastEncodersInvert[0]);
                encoderFastInvertE1 = encoder_data->FastEncodersInvert[0];
                EncoderConfigurationSet = true;
            }
            if(encoder_data->FastEncodersInvert[1] != encoderFastInvertE2) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FastEncoder E2 invert = %d\n", __FILE__, __FUNCTION__, encoder_data->FastEncodersInvert[1]);
                encoderFastInvertE2 = encoder_data->FastEncodersInvert[1];
                EncoderConfigurationSet = true;
            }
            if(encoder_data->FastEncodersInvert[2] != encoderFastInvertE3) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FastEncoder E3 invert = %d\n", __FILE__, __FUNCTION__, encoder_data->FastEncodersInvert[2]);
                encoderFastInvertE3 = encoder_data->FastEncodersInvert[2];
                EncoderConfigurationSet = true;
            }
            if(encoder_data->disable_4x_sampling != encoderFastDisable4xSampling) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FastEncoder disable_4x_sampling = %d\n", __FILE__, __FUNCTION__, encoder_data->disable_4x_sampling);
                encoderFastDisable4xSampling = encoder_data->disable_4x_sampling;
                EncoderConfigurationSet = true;
            }
        }




        // 1. CONFIG: Which pin group config to use?
        if (encoder_data->encoderFastEnable) {
            if (!encoder_data->alternativeconfig)
                config = PK_FASTENCODER_CONF_CFG2;
            else
                config = PK_FASTENCODER_CONF_CFG1;
        } else {
            config = 0; // Disabled
        }
    
        // 2. OPTIONS: Pack inversion and sampling settings
        if (encoderFastDisable4xSampling)
            options |= PK_FASTENCODER_DISABLE_4X_SAMPLING;
    
        if (encoderFastInvertE1)
            options |= PK_FASTENCODER_INVERT_E1;
        if (encoderFastInvertE2)
            options |= PK_FASTENCODER_INVERT_E2;
        if (encoderFastInvertE3)
            options |= PK_FASTENCODER_INVERT_E3;
    
        // 3. Update HAL feedback
        
    
        // 4. Compare and apply
        if (ApplyIniSettings && (
                dev->FastEncodersConfiguration != config ||
                dev->FastEncodersOptions != options)) {
    
            rtapi_print_msg(RTAPI_MSG_ERR,
                "PoKeys: %s:%s: FastEncoder config changed: cfg=%d, opts=0x%02X\n",
                __FILE__, __FUNCTION__, config, options);
    
            dev->FastEncodersConfiguration = config;
            dev->FastEncodersOptions = options;
            EncoderConfigurationSet = true;
        }
    }


    if(dev->info.iUltraFastEncoders){


        if (ApplyIniSettings == false) {
            encoder_data->UltraFastEncoderConfiguration = dev->UltraFastEncoderConfiguration;
        }
        encoder_data->UltraFastEncoderOptions = dev->UltraFastEncoderOptions;

        uint8_t UltraFastEncoderOptions = 0;
        bool UltraFastEncoderOptions_INVERT_DIRECTION       = ((encoder_data->UltraFastEncoderOptions) & PK_UFENCODER_INVERT_DIRECTION)         ? 1 : 0;
        bool UltraFastEncoderOptions_SIGNAL_MODE          = ((encoder_data->UltraFastEncoderOptions) & PK_UFENCODER_SIGNAL_MODE)            ? 1 : 0;
        bool UltraFastEncoderOptions_ENABLE_4X_SAMPLING   = ((encoder_data->UltraFastEncoderOptions) & PK_UFENCODER_ENABLE_4X_SAMPLING)       ? 1 : 0;
        

        if (ApplyIniSettings == false || encoder_data->UltraFastEncoderOptions_INVERT_DIRECTION == 0) {
            encoder_data->UltraFastEncoderOptions_INVERT_DIRECTION = UltraFastEncoderOptions_INVERT_DIRECTION;
        }
        else if(encoder_data->UltraFastEncoderOptions_INVERT_DIRECTION != UltraFastEncoderOptions_INVERT_DIRECTION) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: UltraFastEncoder INVERT_DIRECTION = %d\n", __FILE__, __FUNCTION__,  encoder_data->UltraFastEncoderOptions_INVERT_DIRECTION);
            UltraFastEncoderOptions_INVERT_DIRECTION = encoder_data->UltraFastEncoderOptions_INVERT_DIRECTION;
        }

        if (ApplyIniSettings == false || encoder_data->UltraFastEncoderOptions_SIGNAL_MODE == 0) {
            encoder_data->UltraFastEncoderOptions_SIGNAL_MODE = UltraFastEncoderOptions_SIGNAL_MODE;
        }
        else if(encoder_data->UltraFastEncoderOptions_SIGNAL_MODE != UltraFastEncoderOptions_SIGNAL_MODE) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: UltraFastEncoder SIGNAL_MODE = %d\n", __FILE__, __FUNCTION__,  encoder_data->UltraFastEncoderOptions_SIGNAL_MODE);
            UltraFastEncoderOptions_SIGNAL_MODE = encoder_data->UltraFastEncoderOptions_SIGNAL_MODE;
        }
        if (ApplyIniSettings == false || encoder_data->UltraFastEncoderOptions_ENABLE_4X_SAMPLING == 0) {
            encoder_data->UltraFastEncoderOptions_ENABLE_4X_SAMPLING = UltraFastEncoderOptions_ENABLE_4X_SAMPLING;
        }
        else if(encoder_data->UltraFastEncoderOptions_ENABLE_4X_SAMPLING != UltraFastEncoderOptions_ENABLE_4X_SAMPLING) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: UltraFastEncoder ENABLE_4X_SAMPLING = %d\n", __FILE__, __FUNCTION__,  encoder_data->UltraFastEncoderOptions_ENABLE_4X_SAMPLING);
            UltraFastEncoderOptions_ENABLE_4X_SAMPLING = encoder_data->UltraFastEncoderOptions_ENABLE_4X_SAMPLING;
        }
        if (ApplyIniSettings == false || encoder_data->UltraFastEncoderFilter == 0) {
            encoder_data->UltraFastEncoderFilter = dev->UltraFastEncoderFilter;
        }
        else if(encoder_data->UltraFastEncoderFilter != dev->UltraFastEncoderFilter) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: UltraFastEncoder Filter = %d\n", __FILE__, __FUNCTION__,  encoder_data->UltraFastEncoderFilter);
            dev->UltraFastEncoderFilter = encoder_data->UltraFastEncoderFilter;
        }



        // Compose UltraFastEncoderOptions from HAL bits
        uint8_t options = dev->UltraFastEncoderOptions;
        if (encoder_data->UltraFastEncoderOptions_INVERT_DIRECTION) options |= PK_UFENCODER_INVERT_DIRECTION;
        if (encoder_data->UltraFastEncoderOptions_SIGNAL_MODE)      options |= PK_UFENCODER_SIGNAL_MODE;
        if (encoder_data->UltraFastEncoderOptions_ENABLE_4X_SAMPLING) options |= PK_UFENCODER_ENABLE_4X_SAMPLING;
       // if (encoder_data->UltraFastEncoderOptions_RESET_ON_INDEX)   options |= PK_UFENCODER_RESET_ON_INDEX;
    
       if (ApplyIniSettings && encoder_data->UltraFastEncoderOptions != options) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: UltraFastEncoderOptions = %d\n", __FILE__, __FUNCTION__,  encoder_data->UltraFastEncoderOptions);
        dev->UltraFastEncoderOptions = options;
        EncoderConfigurationSet = true;
       }

       if (ApplyIniSettings && dev->UltraFastEncoderConfiguration != encoder_data->UltraFastEncoderConfiguration){
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: UltraFastEncoderConfiguration = %d\n", __FILE__, __FUNCTION__,  dev->UltraFastEncoderConfiguration);
            dev->UltraFastEncoderConfiguration = encoder_data->UltraFastEncoderConfiguration;
       }
        
        
    }


    if (EncoderConfigurationSet == true) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_EncoderConfigurationSet(dev)\n", __FILE__, __FUNCTION__);
        if (PK_EncoderConfigurationSet(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EncoderConfigurationSet failed\n", __FILE__, __FUNCTION__);
            return;
        }
        EncoderConfigurationSet = false;
    }

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