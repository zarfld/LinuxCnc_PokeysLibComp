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
 * @brief Structure holding HAL pin references and configuration for one PoExtBus bank.
 *
 * Each PoExtBus bank contains 8 digital input and 8 digital output lines.
 * This structure manages the HAL connections for a single PoExtBus module.
 *
 * It supports:
 * - Input pins (active high and active low)
 * - Output pins
 * - Output inversion flags
 * - Byte-wise representation of input and output states
 */
typedef struct {
    hal_bit_t *PoExtBus_digin_in[8];

    hal_bit_t *PoExtBus_digin_in_not[8];

    hal_bit_t *PoExtBus_digout_out[8];

    hal_bit_t PoExtBus_digout_invert[8];

    hal_u32_t PoExtBus_DataSet;
    hal_u32_t PoExtBus_DataGet;

} one_PoExtBus_data_t;

/**
 * @brief Structure holding HAL-related data for all PoExtBus banks.
 *
 * This struct contains the configuration and runtime data for up to 10 PoExtBus banks
 * (external digital I/O expanders) used in conjunction with PoKeys devices.
 * Each bank provides 8 digital inputs and 8 digital outputs.
 *
 * The structure includes:
 * - An array of 10 `one_PoExtBus_data_t` entries, each describing one bus.
 * - A HAL output pin for debugging (`PoExtBus.deb.out`)
 * - A count parameter indicating how many PoExtBus banks are active.
 */
typedef struct {
    one_PoExtBus_data_t PoExtBus[10];
    hal_s32_t *PoExtBus_deb_out; // pin out s32 deb.out;

    hal_u32_t PoExtBus_count;
} all_PoExtBus_data_t;

/**
 * @brief Pointer to the global PoExtBus data structure.
 *
 * This pointer is initialized either via HAL component initialization or
 * dynamically via `hal_malloc()` in `PKPoExtBus_export_pins()`.
 */
static all_PoExtBus_data_t *PoExtBus_data = 0;

/**
 * @brief Export HAL pins for PoKeys PoExtBus (external digital I/O bus).
 *
 * This function creates all necessary HAL pins and parameters to expose
 * the PoExtBus functionality (8 digital inputs and outputs per bus, up to 10 buses)
 * to the LinuxCNC HAL system.
 *
 * It performs the following steps:
 * - Allocates memory for the PoExtBus data structure if not already provided
 * - Creates a debug output pin (`PoExtBus.deb.out`)
 * - Creates a parameter for the number of active PoExtBus entries (`PoExtBus.count`)
 * - For each configured PoExtBus:
 *   - Creates 8 digital input pins: `PoExtBus.<id>.digin.<pin>.in`
 *   - Creates 8 inverted input pins: `PoExtBus.<id>.digin.<pin>.in_not`
 *   - Creates 8 digital output pins: `PoExtBus.<id>.digout.<pin>.out`
 *   - Creates 8 parameters to optionally invert digital outputs: `PoExtBus.<id>.digout.<pin>.invert`
 *   - Adds a parameter to reflect the full byte read from the device (`digin.byte`)
 *   - Adds a parameter representing the desired output byte (`digout.byte`)
 *
 * @param[in] prefix        Prefix for HAL pin names (e.g. "pokeys")
 * @param[in] extra_arg     Reserved (currently unused)
 * @param[in] id            Unique HAL component ID
 * @param[in] njoints       Number of PoExtBus banks (max 10 supported)
 * @param[in] poExtBus_data Pointer to a PoExtBus data structure. If NULL, memory is allocated.
 * @return 0 on success, or a negative HAL error code on failure
 */
int PKPoExtBus_export_pins(char *prefix, long extra_arg, int id, int njoints, all_PoExtBus_data_t *poExtBus_data) {

    int r = 0;
    int j = 0;
    one_PoExtBus_data_t *addr;
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: prefix: %s\n", __FILE__, __FUNCTION__, prefix);
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: id: %d\n", __FILE__, __FUNCTION__, id);
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: njoints: %d\n", __FILE__, __FUNCTION__, njoints);
    if (poExtBus_data == NULL) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: poExtBus_data == NULL\n", __FILE__, __FUNCTION__);
        PoExtBus_data = hal_malloc(sizeof(all_PoExtBus_data_t));
        if (PoExtBus_data == NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PK_PoExtBus: all_PoExtBus_data_t malloc failed\n");
            return -1;
        }
    } else {
        PoExtBus_data = poExtBus_data;
    }
    /*
        if (poExtBus_data == 0)
        {
                rtapi_print_msg(RTAPI_MSG_ERR, "PK_PoExtBus: all_PoExtBus_data_t malloc failed\n");
                return -1;
        }*/

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoExtBus.deb.out\n", __FILE__, __FUNCTION__, prefix);
    r = hal_pin_s32_newf(HAL_OUT, &(PoExtBus_data->PoExtBus_deb_out), id, "%s.PoExtBus.deb.out", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoExtBus.deb.out failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    r = hal_param_u32_newf(HAL_RW, &(PoExtBus_data->PoExtBus_count), id, "%s.PoExtBus.count", prefix);

    PoExtBus_data->PoExtBus_count = njoints;
    for (j = 0; j < (njoints); j++) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PoExtBus %d\n", __FILE__, __FUNCTION__, j);
        addr = &(PoExtBus_data->PoExtBus[j]);

        int PoExtBusId = 9 - j;
        for (int i = 0; i < 8; i++) {
            int Pin_ID = 8 - i;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoExtBus.%01d.digin.%01d.in\n", __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
            r = hal_pin_bit_newf(HAL_OUT, &(addr->PoExtBus_digin_in[i]), id, "%s.PoExtBus.%01d.digin.%01d.in", prefix, PoExtBusId, Pin_ID);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoExtBus.%01d.digin.%01d.in failed\n", __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
                return r;
            }

            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoExtBus.%01d.digin.%01d.in_not\n", __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
            r = hal_pin_bit_newf(HAL_OUT, &(addr->PoExtBus_digin_in_not[i]), id, "%s.PoExtBus.%01d.digin.%01d.in_not", prefix, PoExtBusId, Pin_ID);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys: %s:%s: "
                                "%s.PoExtBus.%01d.digin.%01d.in_not failed\n",
                                __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
                return r;
            }

            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoExtBus.%01d.digout.%01d.out\n", __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
            r = hal_pin_bit_newf(HAL_IN, &(addr->PoExtBus_digout_out[i]), id, "%s.PoExtBus.%01d.digout.%01d.out", prefix, PoExtBusId, Pin_ID);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoExtBus.%01d.digout.%01d.out failed\n", __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
                return r;
            }

            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoExtBus.%01d.digout.%01d.invert\n", __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
            r = hal_param_bit_newf(HAL_RW, &(addr->PoExtBus_digout_invert[i]), id, "%s.PoExtBus.%01d.digout.%01d.invert", prefix, PoExtBusId, Pin_ID);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys: %s:%s: "
                                "%s.PoExtBus.%01d.digout.%01d.invert failed\n",
                                __FILE__, __FUNCTION__, prefix, PoExtBusId, Pin_ID);
                return r;
            }
        }
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoExtBus.%01d.digin.byte\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_u32_newf(HAL_RO, &(addr->PoExtBus_DataGet), id, "%s.PoExtBus.%01d.digin.byte", prefix, PoExtBusId);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoExtBus.%01d.digin.byte failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.PoExtBus.%01d.digout.byte\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_u32_newf(HAL_RW, &(addr->PoExtBus_DataSet), id, "%s.PoExtBus.%01d.digout.byte", prefix, PoExtBusId);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.PoExtBus.%01d.digout.byte failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }
    }
}

/**
 * @brief
 *
 */
bool PoExtBus_Get_BitOfByte(uint8_t in_Byte, int Bit_Id) {
    return (in_Byte >> Bit_Id) & 0x01;
}

/**
 * @brief
 *
 */
uint8_t PoExtBus_Set_BitOfByte(uint8_t in_Byte, int Bit_Id, bool value) {

    if (value == true) {
        in_Byte |= 1 << Bit_Id;
    } else {
        in_Byte &= ~(1 << Bit_Id);
    }
    return in_Byte;
}

/**
 * @brief Updates the PoExtBus data from and to the PoKeys device.
 *
 * This function synchronizes the state of the PoExtBus (PoKeys Extension Bus), which supports
 * digital input and output via external shift registers or I/O expanders.
 *
 * It performs the following steps:
 * - Reads the current state from the PoKeys device using `PK_PoExtBusGet()`
 * - Updates the `PoExtBus_digin_in` and `PoExtBus_digin_in_not` HAL pins for all bus channels
 * - Reads desired digital output values from `PoExtBus_digout_out` HAL pins
 * - Applies optional inversion logic for outputs
 * - Compares desired vs actual state (`PoExtBus_DataSet` vs `PoExtBus_DataGet`)
 * - If there are changes, updates the PoKeys device using `PK_PoExtBusSet()`
 *
 * Debug values are stored in `PoExtBus_deb_out` for diagnostics.
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 */
void PKPoExtBus_Update(sPoKeysDevice *dev) {
    int i = 0;
    int j = 0;
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: 1 info_PoExtBusCount = %d\n", __FILE__, __FUNCTION__, PoExtBus_data->PoExtBus_count);
    if (PoExtBus_data->PoExtBus_count != 0) {
        *(PoExtBus_data->PoExtBus_deb_out) = 10;
        if (PK_PoExtBusGet(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: 2 PK_PoExtBusGet(dev) failed\n", __FILE__, __FUNCTION__);
            *(PoExtBus_data->PoExtBus_deb_out) = 11;
            PK_PoExtBusGet(dev);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
        one_PoExtBus_data_t *addr;
        *(PoExtBus_data->PoExtBus_deb_out) = 12;

        bool PoExtBus_DoSet = false;
        for (i = 0; i < 10; i++) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: 3 PK_PoExtBusGet(dev) i = %d\n", __FILE__, __FUNCTION__, i);

            *(PoExtBus_data->PoExtBus_deb_out) = 130 + i;
            // PoExtBus_DataGet[i] = dev->PoExtBusData[i];
            addr = &(PoExtBus_data->PoExtBus[i]);
            (addr->PoExtBus_DataGet) = dev->PoExtBusData[i]; // uint8_t*                  PoExtBusData;                  // PoExtBus outputs buffer
            (addr->PoExtBus_DataGet) = dev->PoExtBusData[i]; // uint8_t*                  PoExtBusData;                  // PoExtBus outputs buffer
            *(PoExtBus_data->PoExtBus_deb_out) = 140 + i;
            dev->PoExtBusData[i] = 0;
            for (j = 0; j < 8; j++) {
                *(PoExtBus_data->PoExtBus_deb_out) = 1400 + i * 10 + j;
                //*(addr->PoExtBus_digin_in[j]) = PoExtBus_Get_BitOfByte(addr->PoExtBus_DataGet, j);
                *(addr->PoExtBus_digin_in[j]) = (addr->PoExtBus_DataGet >> j) & 0x01;
                //*(addr->PoExtBus_digin_in_not[j]) = !PoExtBus_Get_BitOfByte(addr->PoExtBus_DataGet, j);
                *(addr->PoExtBus_digin_in_not[j]) = !((addr->PoExtBus_DataGet >> j) & 0x01);

                *(PoExtBus_data->PoExtBus_deb_out) = 1500 + i * 10 + j;

                if (addr->PoExtBus_digout_invert[j] == false) {
                    *(PoExtBus_data->PoExtBus_deb_out) = 1600 + i * 10 + j;
                    //(addr->PoExtBus_DataSet) = PoExtBus_Set_BitOfByte(addr->PoExtBus_DataSet, j, (addr->PoExtBus_digout_out[j]));
                    if (*(addr->PoExtBus_digout_out[j]) == 1) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: 3 PoExtBus.%d.out.%d = true\n", __FILE__, __FUNCTION__, i, j);
                        dev->PoExtBusData[i] |= (1 << j);
                    } else {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: 3 PoExtBus.%d.out.%d = false\n", __FILE__, __FUNCTION__, i, j);
                        dev->PoExtBusData[i] &= ~(1 << j);
                    }
                } else {
                    *(PoExtBus_data->PoExtBus_deb_out) = 1700 + i * 10 + j;
                    //(addr->PoExtBus_DataSet) = PoExtBus_Set_BitOfByte(addr->PoExtBus_DataSet, j, !(addr->PoExtBus_digout_out[j]));
                    if (*(addr->PoExtBus_digout_out[j]) != 0) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: 3 invert "
                                        "PoExtBus.%d.out.%d = true\n",
                                        __FILE__, __FUNCTION__, i, j);
                        dev->PoExtBusData[i] &= ~(1 << j);
                    } else {
                        dev->PoExtBusData[i] |= (1 << j);
                    }
                }
                addr->PoExtBus_DataSet = dev->PoExtBusData[i];
                if (addr->PoExtBus_DataSet != addr->PoExtBus_DataGet) {
                    PoExtBus_DoSet = true;
                }
            }
        }
        if (PoExtBus_DoSet) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: 7 PK_PoExtBusSet(dev)\n", __FILE__, __FUNCTION__);
            // PK_PoExtBusSet(dev);
            // PK_PoExtBusSet(dev);
            // PK_PoExtBusSet(dev);
            if (PK_PoExtBusSet(dev) != PK_OK) {
                *(PoExtBus_data->PoExtBus_deb_out) = 200;
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: 8 PK_PoExtBusSet(dev) failed\n", __FILE__, __FUNCTION__);
                PK_PoExtBusSet(dev);
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: 9 PK_PoExtBusSet(dev) done\n", __FILE__, __FUNCTION__);
                *(PoExtBus_data->PoExtBus_deb_out) = 210;
            }
#ifdef ULAPI
            usleep(sleepdur);
#endif
        }
    }
}

/**
 * @brief Initializes or configures the PoExtBus interface on the PoKeys device.
 *
 * This function is intended to set up the PoExtBus (PoKeys Extension Bus), which allows
 * communication with external modules such as I/O expanders or shift registers.
 *
 * Currently, this function is a placeholder and does not perform any operation.
 *
 * You may extend it to:
 * - Configure the PoExtBus timing or mode
 * - Initialize attached modules (e.g., HCT595 shift registers, PoExtBusOC16)
 * - Load module-specific configuration from the INI file
 *
 * @param[in] dev Pointer to the initialized PoKeys device structure
 */
void PKPoExtBus_Setup(sPoKeysDevice *dev) {
}

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.0.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.1.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.2.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.3.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.4.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.5.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.6.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.7.in:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.0.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.1.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.2.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.3.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.4.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.5.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.6.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digin.7.in-not:bit:10:out::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.0.out:bit:10:in::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.1.out:bit:10:in::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.2.out:bit:10:in::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.3.out:bit:10:in::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.4.out:bit:10:in::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.5.out:bit:10:in::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.6.out:bit:10:in::None:None");
MODULE_INFO(linuxcnc, "pin:PoExtBus.#.digout.7.out:bit:10:in::None:None");
// MODULE_INFO(linuxcnc, "param:PoExtBus.#.digout.0.invert:bit:10:rw:If TRUE, out is inverted before writing to the hardware.:None:None");
MODULE_INFO(linuxcnc, "param:PoExtBus.#.digout.1.invert:bit:10:rw:If TRUE, out "
                      "is inverted before writing to the hardware.:None:None");
MODULE_INFO(linuxcnc, "param:PoExtBus.#.digout.2.invert:bit:10:rw:If TRUE, out "
                      "is inverted before writing to the hardware.:None:None");
MODULE_INFO(linuxcnc, "param:PoExtBus.#.digout.3.invert:bit:10:rw:If TRUE, out "
                      "is inverted before writing to the hardware.:None:None");
MODULE_INFO(linuxcnc, "param:PoExtBus.#.digout.4.invert:bit:10:rw:If TRUE, out "
                      "is inverted before writing to the hardware.:None:None");
MODULE_INFO(linuxcnc, "param:PoExtBus.0.digout.5.invert:bit:10:rw:If TRUE, out "
                      "is inverted before writing to the hardware.:None:None");
MODULE_INFO(linuxcnc, "param:PoExtBus.#.digout.6.invert:bit:10:rw:If TRUE, out "
                      "is inverted before writing to the hardware.:None:None");
MODULE_INFO(linuxcnc, "param:PoExtBus.#.digout.7.invert:bit:10:rw:If TRUE, out "
                      "is inverted before writing to the hardware.:None:None");
#endif // MODULE_INFO