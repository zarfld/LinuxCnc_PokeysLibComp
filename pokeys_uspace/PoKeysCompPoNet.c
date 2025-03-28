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

/**
 * @brief Global pointer to the PoNET data structure used by the HAL component.
 *
 * Initialized during `PKPoNet_export_pins()` or similar setup routines.
 */
static all_PoNET_data_t *PoNet_data = 0;

/**
 * @brief Exports HAL pins and parameters for PoNET modules and optionally kbd48CNC devices.
 *
 * This function initializes and exports all relevant HAL pins and parameters for PoNET modules
 * connected to a PoKeys device. It dynamically allocates memory for internal state (if needed)
 * and assigns module metadata (ID, type, size, options) and I/O channels as HAL pins.
 *
 * Additionally, if a `kbd48CNC` module is detected, it exports button and LED pins
 * for up to 48 keys and sets up backlight brightness and control pins.
 *
 * The function supports up to 16 PoNET modules with up to 16 I/O channels each.
 *
 * @param prefix         The string prefix for the HAL pin and parameter names (e.g. "pokeys").
 * @param extra_arg      Reserved extra argument, unused.
 * @param id             Unique HAL component ID used for pin/parameter creation.
 * @param njoints        Number of PoNET modules expected or to be scanned.
 * @param poNET_data     Pointer to an existing `all_PoNET_data_t` structure or NULL to auto-allocate.
 * @param dev            Pointer to the PoKeys device structure. Must not be NULL.
 *
 * @return 0 on success, negative value on failure (e.g. malloc failure, pin export error).
 *
 * @note This function uses `PK_PoNETGetModuleSettings()` to query each module slot (0–15)
 *       and only exports pins for detected and valid modules.
 * @note Memory allocation for `PoNet_data->kbd48CNCio[]` elements is done on demand.
 * @note Requires `dev->info.iPoNET` > 0 (i.e., PoNET support available on the device).
 */
int PKPoNet_export_pins(char *prefix, long extra_arg, int id, int njoints, all_PoNET_data_t *poNET_data, sPoKeysDevice *dev) {
    int PoNet_Count = 0;
    int r = 0;
    int j = 0;

    if (dev == NULL) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: dev == NULL\n", __FILE__, __FUNCTION__);
        return -1;
    }
    if (dev->info.iPoNET == 0) {
        rtapi_print_msg(RTAPI_MSG_WARN, "PoKeys: %s:%s: PoNET not supported\n", __FILE__, __FUNCTION__);
        return 0;
    }
    one_PoNET_data_t *addr;
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: prefix: %s\n", __FILE__, __FUNCTION__, prefix);
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: id: %d\n", __FILE__, __FUNCTION__, id);
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: njoints: %d\n", __FILE__, __FUNCTION__, njoints);
    if (poNET_data == NULL) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: poNET_data == NULL\n", __FILE__, __FUNCTION__);
        PoNet_data = hal_malloc(sizeof(all_PoNET_data_t));
        if (PoNet_data == NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PK_PoNET: all_PoNET_data_t malloc failed\n");
            return -1;
        }
    } else {
        PoNet_data = poNET_data;
    }
    /*
        if (poNET_data == 0)
        {
                rtapi_print_msg(RTAPI_MSG_ERR, "PK_PoNET: all_PoNET_data_t malloc failed\n");
                return -1;
        }*/

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.PoNETstatus\n", __FILE__, __FUNCTION__, prefix);
    /*r = hal_param_u32_newf(HAL_RW, &(PoNet_data->PoNET_PoNETstatus), id,
                                                   "%s.PoNET.PoNETstatus", prefix);*/
    r = hal_pin_u32_newf(HAL_OUT, &(PoNet_data->PoNET_PoNETstatus), id, "%s.PoNET.PoNETstatus", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.PoNETstatus failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.DevCount\n", __FILE__, __FUNCTION__, prefix);
    r = hal_param_u32_newf(HAL_RW, &(PoNet_data->PoNET_DevCount), id, "%s.PoNET.DevCount", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.DevCount failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.available\n", __FILE__, __FUNCTION__, prefix);
    r = hal_param_bit_newf(HAL_RW, &(PoNet_data->kbd48CNC_available), id, "%s.kbd48CNC.available", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.available failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.PoNetID\n", __FILE__, __FUNCTION__, prefix);
    r = hal_param_u32_newf(HAL_RW, &(PoNet_data->kbd48CNC_PoNetID), id, "%s.kbd48CNC.PoNetID", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.PoNetID failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.KeyBrightness\n", __FILE__, __FUNCTION__, prefix);
    r = hal_pin_s32_newf(HAL_OUT, &(PoNet_data->deb_out), id, "%s.PoNET.Deb.Out", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.Deb.Out failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    PoNet_data->kbd48CNC_available = 0;
    PoNet_data->kbd48CNC_PoNetID = 0;
    PoNet_data->PoNET_DevCount = 0;
    for (j = 0; j < 16; j++) // up to 16 PoNet Devices Supported
    {
#ifdef ULAPI
        usleep(sleepdur);
#endif
        dev->PoNETmodule.moduleID = j;
        if (PK_PoNETGetModuleSettings(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PoNet[%d].moduleID = %d\n", __FILE__, __FUNCTION__, j, dev->PoNETmodule.moduleID);
            addr = &(PoNet_data->PoNET[j]);

            if (dev->PoNETmodule.moduleType != 0) {

                PoNet_data->PoNET_DevCount++;
                if (dev->PoNETmodule.moduleType == 0x10 && PoNet_data->kbd48CNC_available == 0) {
                    if ((dev->PoNETmodule.moduleOptions & (1 << 7)) != 0) {
                        // kbd48CNC is also detected
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kb48CNC detected\n", __FILE__, __FUNCTION__);
                        (PoNet_data->kbd48CNC_PoNetID) = j;
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PoNet[%d].kbd48CNC_PoNetID = %d\n", __FILE__, __FUNCTION__, j, PoNet_data->kbd48CNC_PoNetID);
                        // dev->deviceConfig->iEnableKBD48CNC = 1;
                        PoNet_data->kbd48CNC_available = 1;
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys: %s:%s: "
                                        "PoNet[%d].kbd48CNC_available = %d\n",
                                        __FILE__, __FUNCTION__, j, PoNet_data->kbd48CNC_available);
                    }
                }
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.moduleID\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_u32_newf(HAL_RO, &(addr->PoNET_moduleID), id, "%s.PoNET.%01d.moduleID", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.%01d.moduleID failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                addr->PoNET_moduleID = dev->PoNETmodule.moduleID;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PoNet[%d].moduleID = %d\n", __FILE__, __FUNCTION__, j, addr->PoNET_moduleID);

                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.i2cAddress\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_u32_newf(HAL_RO, &(addr->PoNET_i2cAddress), id, "%s.PoNET.%01d.i2cAddress", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.%01d.i2cAddress failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                addr->PoNET_i2cAddress = dev->PoNETmodule.i2cAddress;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PoNet[%d].i2cAddress = %d\n", __FILE__, __FUNCTION__, j, addr->PoNET_i2cAddress);

                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.moduleType\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_u32_newf(HAL_RO, &(addr->PoNET_moduleType), id, "%s.PoNET.%01d.moduleType", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.%01d.moduleType failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                addr->PoNET_moduleType = dev->PoNETmodule.moduleType;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PoNet[%d].moduleType = %d\n", __FILE__, __FUNCTION__, j, addr->PoNET_moduleType);

                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.moduleSize\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_u32_newf(HAL_RO, &(addr->PoNET_moduleSize), id, "%s.PoNET.%01d.moduleSize", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.%01d.moduleSize failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                addr->PoNET_moduleSize = dev->PoNETmodule.moduleSize;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PoNet[%d].moduleSize = %d\n", __FILE__, __FUNCTION__, j, addr->PoNET_moduleSize);

                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.moduleOptions\n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_param_u32_newf(HAL_RO, &(addr->PoNET_moduleOptions), id, "%s.PoNET.%01d.moduleOptions", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoNET.%01d.moduleOptions failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
                addr->PoNET_moduleOptions = dev->PoNETmodule.moduleOptions;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PoNet[%d].moduleOptions = %d\n", __FILE__, __FUNCTION__, j, addr->PoNET_moduleOptions);
                for (int p = 0; p < 16; p++) {
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.statusIn\n", __FILE__, __FUNCTION__, prefix, j);
                    r = hal_pin_u32_newf(HAL_OUT, &(addr->PoNET_statusIn[p]), id, "%s.PoNET.%01d.digin.%01d.in", prefix, j, p);
                    if (r != 0) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.statusIn failed\n", __FILE__, __FUNCTION__, prefix, j);
                        return r;
                    }

                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.statusOut\n", __FILE__, __FUNCTION__, prefix, j);
                    r = hal_pin_u32_newf(HAL_IN, &(addr->PoNET_statusOut[p]), id, "%s.PoNET.%01d.digout.%01d.out", prefix, j, p);
                    if (r != 0) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoNET.%01d.statusOut failed\n", __FILE__, __FUNCTION__, prefix, j);
                        return r;
                    }
                }
            }
        }
    }
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PoNet_Count = %d\n", __FILE__, __FUNCTION__, PoNet_Count);
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PoNet_data->kbd48CNC_available = %d\n", __FILE__, __FUNCTION__, PoNet_data->kbd48CNC_available);
    if (PoNet_data->kbd48CNC_available != 0) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC_available != 0 \n", __FILE__, __FUNCTION__);
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.lightValue\n", __FILE__, __FUNCTION__, prefix);
        r = hal_pin_u32_newf(HAL_OUT, &(PoNet_data->kbd48CNC_lightValue), id, "%s.kbd48CNC.lightValue", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.lightValue failed\n", __FILE__, __FUNCTION__);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.KeyBrightness\n", __FILE__, __FUNCTION__, prefix);
        r = hal_pin_u32_newf(HAL_OUT, &(PoNet_data->kbd48CNC_KeyBrightness), id, "%s.kbd48CNC.KeyBrightness", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.KeyBrightness failed\n", __FILE__, __FUNCTION__);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.prevBrightness\n", __FILE__, __FUNCTION__, prefix);
        r = hal_pin_u32_newf(HAL_OUT, &(PoNet_data->kbd48CNC_prevBrightness), id, "%s.kbd48CNC.prevBrightness", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.prevBrightness failed\n", __FILE__, __FUNCTION__);
            return r;
        }

        for (int j = 0; j < 48; j++) {

            if (PoNet_data->kbd48CNCio[j] == NULL) {
                PoNet_data->kbd48CNCio[j] = hal_malloc(sizeof(one_kbd48CNCButton_data_t));
                if (PoNet_data->kbd48CNCio[j] == NULL) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "Failed to allocate memory for kbd48CNCio[%d]\n", j);
                    return -1;
                }
            }
            // one_kbd48CNCButton_data_t *addr = *(PoNet_data->kbd48CNCio)[j];
            if (PoNet_data->kbd48CNCio[j] != NULL) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.%01d.LED \n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_pin_bit_newf(HAL_IN, &(PoNet_data->kbd48CNCio[j]->LED), id, "%s.kbd48CNC.%01d.LED", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.%01d.LED failed \n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }

                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.kbd48CNC.%01d.Button \n", __FILE__, __FUNCTION__, prefix, j);
                r = hal_pin_bit_newf(HAL_OUT, &(PoNet_data->kbd48CNCio[j]->Button), id, "%s.kbd48CNC.%01d.Button", prefix, j);
                if (r != 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.%01d.Button failed\n", __FILE__, __FUNCTION__, prefix, j);
                    return r;
                }
            } else {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.kbd48CNC.%01d failed\n", __FILE__, __FUNCTION__, prefix, j);
            }
        }
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kb48CNC init done\n", __FILE__, __FUNCTION__);
    }
    return r;
}

/**
 * @brief Updates the state of the connected PoNET modules, specifically kbd48CNC.
 *
 * This function performs a full update cycle for the `kbd48CNC` device connected via PoNET,
 * including:
 * - Fetching module settings and light (backlight) values
 * - Reading key/button states from the device
 * - Reflecting input state into HAL pins
 * - Updating LED states based on HAL input
 * - Writing updated status information back to the module
 *
 * The function contains debug outputs and a `deb_out` pin to trace execution and states.
 * It supports fault handling and retries for communication with the device.
 *
 * @param dev Pointer to the sPoKeysDevice structure, representing the connected PoKeys device.
 *
 * @note This function only performs actions if `kbd48CNC_available` is non-zero,
 *       indicating that a kbd48CNC PoNET module has been detected.
 *
 * @note The `kbd48CNC` buttons are processed in a remapped ID order using a 4-group offset scheme,
 *       to match hardware layout with software index logic.
 *
 * @warning This function assumes that `PoNet_data` has already been initialized and populated
 *          by `PKPoNet_export_pins()`. If not, behavior is undefined.
 */
void PKPoNet_Update(sPoKeysDevice *dev) {

    if (PoNet_data->kbd48CNC_available != 0) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kb48CNC\n", __FILE__, __FUNCTION__);
        *(PoNet_data->deb_out) = 200;
        dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
        *(PoNet_data->deb_out) = 210;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC_PoNetID = %d\n", __FILE__, __FUNCTION__, (PoNet_data->kbd48CNC_PoNetID));
#ifdef ULAPI
        usleep(sleepdur);
#endif
        *(PoNet_data->deb_out) = 220;
        if (PK_PoNETGetModuleSettings(dev) != PK_OK) {
            *(PoNet_data->deb_out) = 230;
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s PK_PoNETGetModuleSettings(dev) failed\n", __FILE__, __FUNCTION__);
            if (PK_PoNETGetModuleSettings(dev) != PK_OK) {
                *(PoNet_data->deb_out) = 240;
            }
        } else {
            *(PoNet_data->deb_out) = 250;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s PK_PoNETGetModuleSettings(dev) OK\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif

        dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
        *(PoNet_data->deb_out) = 260;
        PK_PoNETGetModuleLight(dev);
#ifdef ULAPI
        usleep(sleepdur);
#endif
        *(PoNet_data->deb_out) = 270;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleLight(dev)\n", __FILE__, __FUNCTION__);
        if (PK_PoNETGetModuleLight(dev) == PK_OK) {
            *(PoNet_data->deb_out) = 280;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleLight(dev) OK\n", __FILE__, __FUNCTION__);
            if (PoNet_data->kbd48CNC_lightValue != NULL) {
                *(PoNet_data->deb_out) = 281;
                *(PoNet_data->kbd48CNC_lightValue) = dev->PoNETmodule.lightValue;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC_lightValue is NULL\n", __FILE__, __FUNCTION__);
            }
            *(PoNet_data->deb_out) = 290;

            *(PoNet_data->kbd48CNC_KeyBrightness) = 255 - (int)(*(PoNet_data->kbd48CNC_lightValue));
            *(PoNet_data->deb_out) = 300;
            if (PoNet_data->kbd48CNC_prevBrightness != PoNet_data->kbd48CNC_KeyBrightness) {
                *(PoNet_data->deb_out) = 310;
                dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
                *(PoNet_data->deb_out) = 320;
                dev->PoNETmodule.PWMduty = *(PoNet_data->kbd48CNC_KeyBrightness);
                *(PoNet_data->deb_out) = 330;
#ifdef ULAPI
                usleep(sleepdur);
#endif
                if (PK_PoNETSetModulePWM(dev) != PK_OK) {
                    *(PoNet_data->deb_out) = 340;
                    if (PK_PoNETSetModulePWM(dev) != PK_OK) {
                        *(PoNet_data->deb_out) = 350;
                    }
                }
                *(PoNet_data->deb_out) = 360;
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PoNETGetModuleLight(dev) failed\n", __FILE__, __FUNCTION__);
            *(PoNet_data->deb_out) = 370;
#ifdef ULAPI
            usleep(sleepdur);
#endif
            if (PK_PoNETGetModuleLight(dev) != PK_OK) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PoNETGetModuleLight(dev) failed\n", __FILE__, __FUNCTION__);
                *(PoNet_data->deb_out) = 380;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleLight(dev) OK\n", __FILE__, __FUNCTION__);
            }
            *(PoNet_data->deb_out) = 390;
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif

        dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleStatusRequest(dev)\n", __FILE__, __FUNCTION__);
        if (PK_PoNETGetModuleStatusRequest(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PoNETGetModuleStatusRequest(dev) failed\n", __FILE__, __FUNCTION__);

#ifdef ULAPI
            usleep(sleepdur);
#endif
            dev->PoNETmodule.moduleID = 0;
            if (PK_PoNETGetModuleStatusRequest(dev) != PK_OK) {
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys: %s:%s: "
                                "PK_PoNETGetModuleStatusRequest(dev) failed\n",
                                __FILE__, __FUNCTION__);
                PoNet_data->kbd48CNC_PoNetID = 0;
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleStatusRequest(dev) OK\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
        PoNet_data->kbd48CNC_Counter = 0;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleStatus(dev)\n", __FILE__, __FUNCTION__);
        while (PK_PoNETGetModuleStatus(dev) != PK_OK && PoNet_data->kbd48CNC_Counter < 10) {
            rtapi_print_msg(RTAPI_MSG_ERR,
                            "PoKeys: %s:%s: PK_PoNETGetModuleStatus(dev) "
                            "failed kbd48CNC_Counter %d\n",
                            __FILE__, __FUNCTION__, PoNet_data->kbd48CNC_Counter);
            PoNet_data->kbd48CNC_Counter++;
#ifdef ULAPI
            usleep(sleepdur);
#endif
        }

#ifdef ULAPI
        usleep(sleepdur);
#endif
        dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleStatus(dev)\n", __FILE__, __FUNCTION__);
        if (PK_PoNETGetModuleStatus(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PoNETGetModuleStatus(dev) failed\n", __FILE__, __FUNCTION__);
            if (PK_PoNETGetModuleStatus(dev) != PK_OK) {
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETGetModuleStatus(dev) OK\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif

        for (int i = 0; i < 48; i++) {
            bool state = false;
            bool instate = false;
            // blinking 1Hz for Debug
            // state=secBlink;
            int offset[] = { 15, 8, 7, 0 };

            int top = (i & 0xF0) + offset[(i / 4) % 4];
            int y = i % 4;
            int ID = top + y;
            if (((i / 4) % 2) == 0) {
                ID = top - y;
            }
            one_kbd48CNCButton_data_t *addr = PoNet_data->kbd48CNCio[i];
            if (*(addr->LED) == true) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC_LED(%d) = true\n", __FILE__, __FUNCTION__, i);
                state = true;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC_LED(%d) = false\n", __FILE__, __FUNCTION__, i);
            }

            if ((dev->PoNETmodule.statusIn[ID / 8] & (1 << (ID % 8))) > 0) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC statusIn[%d] = %d\n", __FILE__, __FUNCTION__, ID / 8, dev->PoNETmodule.statusIn[ID / 8]);
                instate = true;
                state = true;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC statusIn[%d] = %d\n", __FILE__, __FUNCTION__, ID / 8, dev->PoNETmodule.statusIn[ID / 8]);
            }

            /*while (kbd48CNC_Button(i)!=instate)
                        {
                                kbd48CNC_Button(i)=instate;
                                //usleep(sleepdur/10);
                        }*/

            *(addr->Button) = instate;
            int tmp = dev->PoNETmodule.statusOut[ID / 8];
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC state[%d] = %d  tmp=%d\n", __FILE__, __FUNCTION__, ID / 8, state, tmp);
            if (state) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC state[%d] = %d  tmp=%d\n", __FILE__, __FUNCTION__, ID / 8, state, tmp);
                tmp |= (1 << (ID % 8));
            } else {

                tmp &= ~(1 << (ID % 8));
            }
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC state[%d] = %d  tmp=%d\n", __FILE__, __FUNCTION__, ID / 8, state, tmp);
            dev->PoNETmodule.statusOut[ID / 8] = tmp;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC statusOut[%d] = %d\n", __FILE__, __FUNCTION__, ID / 8, dev->PoNETmodule.statusOut[ID / 8]);
            /*while (dev->PoNETmodule.statusOut[ID/8] != tmp)
                        {
                                dev->PoNETmodule.statusOut[ID/8] = tmp;
                                //usleep(sleepdur/10);
                        }*/
        }

        dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
        PK_PoNETSetModuleStatus(dev);
#ifdef ULAPI
        usleep(sleepdur);
#endif
        PK_PoNETSetModuleStatus(dev);
#ifdef ULAPI
        usleep(sleepdur);
#endif

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETSetModuleStatus(dev)\n", __FILE__, __FUNCTION__);
        if (PK_PoNETSetModuleStatus(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PoNETSetModuleStatus(dev) failed\n", __FILE__, __FUNCTION__);

            dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
            if (PK_PoNETSetModuleStatus(dev) != PK_OK) {
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PoNETSetModuleStatus(dev) OK\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif

        dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC_PoNetID = %d\n", __FILE__, __FUNCTION__, PoNet_data->kbd48CNC_PoNetID);
        if (PK_PoNETSetModuleStatus(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: kbd48CNC PK_PoNETSetModuleStatus(dev)\n", __FILE__, __FUNCTION__);
            dev->PoNETmodule.moduleID = PoNet_data->kbd48CNC_PoNetID;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC PK_PoNETSetModuleStatus(dev)\n", __FILE__, __FUNCTION__);
            if (PK_PoNETSetModuleStatus(dev) != PK_OK) {
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kbd48CNC PK_PoNETSetModuleStatus(dev) OK\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif

        // dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
        // PK_PoNETSetModuleStatus(dev);
    } else {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: kb48CNC not available\n", __FILE__, __FUNCTION__);
    }
}

/**
 * @brief Initializes or configures PoNET-related settings for the given device.
 *
 * This function is a placeholder for any future setup logic required for PoNET modules.
 * Currently, it does not perform any operations.
 *
 * @param dev Pointer to the sPoKeysDevice structure representing the connected PoKeys device.
 *
 * @note Intended to be called during component initialization or device setup.
 *       Can be expanded to handle default configurations, pre-allocations, or hardware checks.
 */
void PKPoNet_Setup(sPoKeysDevice *dev) {
}

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "pin:kbd48CNC.available:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:kbd48CNC.PoNetID:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:kbd48CNC.KeyBrightness:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:kbd48CNC.prevBrightness:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:kbd48CNC.lightValue:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:kbd48CNC.#.LED:bit:48:in::None:None");
MODULE_INFO(linuxcnc, "pin:kbd48CNC.#.Button:bit:48:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.#.moduleID:u32:16:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.#.i2cAddress:u32:16:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.#.moduleType:u32:16:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.#.moduleSize:u32:16:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.#.moduleOptions:u32:16:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.PWMduty:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.lightValue:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.PoNETstatus:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.DevCount:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.#.statusIn:u32:16:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoNET.#.statusOut:u32:16:in::None:None");
#endif // MODULE_INFO