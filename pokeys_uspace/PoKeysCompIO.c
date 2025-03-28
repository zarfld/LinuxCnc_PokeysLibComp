/**
 * @file PoKeysCompIO.c
 * @brief HAL interface for PoKeys digital and analog I/O handling.
 *
 * This file implements the creation, reading, and writing of HAL pins
 * associated with PoKeys device I/O (Input/Output), such as digital inputs,
 * digital outputs, analog inputs, analog outputs, and counters.
 *
 * It contains all HAL-related functions required to interface these I/O
 * types with LinuxCNC. These functions populate HAL pins and parameters
 * with values retrieved from or written to the PoKeys device through
 * the PoKeysLib API.
 *
 * Functions include:
 * - Creation of HAL pins for each supported I/O type
 * - Reading values from the device and updating HAL pins accordingly
 * - Writing values from HAL pins to the device
 *
 * This code is part of the HAL component layer and works with the
 * `sPoKeysDevice` structure, which is defined and maintained in the
 * `pokeyslib` library.
 *
 * @author zarfld
 * @date 2023
 * @copyright MIT License
 *
 * @ingroup PoKeysHALComponent
 */

#include "PoKeysLib.h"
#include "rtapi.h"
// #include "rtapi_app.h"
#include "hal.h"
#include "stdio.h"

#include "pokeys_ini.c"

#ifdef ULAPI
extern unsigned int sleepdur;
#endif
extern bool ApplyIniSettings;

/**
 * @brief Pointer to the global I/O data structure used by the HAL component.
 *
 * This static pointer references the main `all_IO_data_t` structure instance,
 * which holds all analog input/output and digital I/O data mapped to HAL pins.
 * It is initialized during component setup and used throughout the component
 * lifecycle to access I/O-related values and settings.
 *
 * @note This pointer is shared across all I/O-related helper functions and should
 * not be reallocated or reassigned after initialization.
 */
static all_IO_data_t *IO_data = 0;

/**
 * @brief Export HAL pins and parameters for PoKeys analog and digital I/O.
 *
 * This function creates and registers HAL pins and parameters for all supported
 * analog outputs, analog inputs, digital I/O, and counters based on the device
 * capabilities and configuration. It binds the relevant fields in the provided
 * `all_IO_data_t` structure to HAL pins using the specified `prefix`.
 *
 * @param prefix      The prefix string for naming HAL pins (typically the HAL component name).
 * @param extra_arg   Unused extra argument (reserved for future use or callback compatibility).
 * @param id          The HAL component ID.
 * @param Io_data     Pointer to the I/O data structure containing HAL pin references.
 *                    If NULL, a new structure is allocated via `hal_malloc`.
 * @param dev         Pointer to the PoKeys device structure with hardware capabilities.
 *
 * @return Zero on success, or a negative error code if HAL pin/parameter creation fails.
 *
 * @note If `Io_data` is NULL, this function allocates and initializes a new I/O data block
 *       and assigns it to the static global pointer `IO_data`.
 *
 * @see all_IO_data_t
 * @see one_adcout_data_t
 * @see one_adcin_data_t
 * @see one_digiIO_data_t
 */
int PKIO_export_pins(char *prefix, long extra_arg, int id, all_IO_data_t *Io_data, sPoKeysDevice *dev) {
    int r = 0;
    int j = 0;
    IO_data = Io_data;

    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: prefix: %s\n", __FILE__, __FUNCTION__, prefix);
    if (dev == NULL) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s dev == NULL\n", __FILE__, __FUNCTION__);
    }

    if (Io_data == NULL) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s Io_data == NULL\n", __FILE__, __FUNCTION__);
        IO_data = hal_malloc(sizeof(all_IO_data_t));
        if (IO_data == NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: all_IO_data_t malloc failed\n", __FILE__, __FUNCTION__);
            return -1;
        }
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: Io_data != NULL\n", __FILE__, __FUNCTION__);
        IO_data = Io_data;
    }

    // AnalogOut Pins
    int analogOutCount = dev->info.iPWMCount;
    for (j = 0; j < (analogOutCount); j++) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.deb.out\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_u32_newf(HAL_OUT, &(IO_data->adcout[j].deb_out), id, "%s.adcout.%01d.deb.out", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.deb.out failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.deb.setval\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_u32_newf(HAL_OUT, &(IO_data->adcout[j].deb_setval), id, "%s.adcout.%01d.deb.setval", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.deb.setval failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.deb.setval\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_float_newf(HAL_IN, &(IO_data->adcout[j].value), id, "%s.adcout.%01d.value", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.value failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.enable\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_pin_bit_newf(HAL_IN, &(IO_data->adcout[j].enable), id, "%s.adcout.%01d.enable", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.enable failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.offset\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].offset), id, "%s.adcout.%01d.offset", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.offset failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.scale\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].scale), id, "%s.adcout.%01d.scale", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.scale failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.high-limit\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].high_limit), id, "%s.adcout.%01d.high-limit", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.high-limit failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.low-limit\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].low_limit), id, "%s.adcout.%01d.low-limit", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.low-limit failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].max_v), id, "%s.adcout.%01d.max-v", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.max-v failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        r = hal_param_u32_newf(HAL_RO, &(IO_data->adcout[j].PinId), id, "%s.adcout.%01d.PinId", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.PinId failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }
    }
    r = hal_pin_u32_newf(HAL_OUT, &(IO_data->adcout_deb_outv), id, "%s.adcout.deb.outv", prefix);

    r = hal_param_u32_newf(HAL_RW, &(IO_data->adcout_pwm_period), id, "%s.adcout.pwm.period", prefix);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.pwm.period failed\n", __FILE__, __FUNCTION__, prefix);
        return r;
    }

    // AnalogIn Pins
    int analogInCount = 7;
    for (j = 0; j < (analogInCount); j++) {
        r = hal_pin_float_newf(HAL_OUT, &(IO_data->adcin[j].value_raw), id, "%s.adcin.%01d.value-raw", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.value-raw failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        r = hal_pin_float_newf(HAL_OUT, &(IO_data->adcin[j].value), id, "%s.adcin.%01d.value", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.value failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        r = hal_param_float_newf(HAL_RW, &(IO_data->adcin[j].scale), id, "%s.adcin.%01d.scale", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.scale failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        r = hal_param_float_newf(HAL_RW, &(IO_data->adcin[j].offset), id, "%s.adcin.%01d.offset", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.offset failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }
    }

    // Digital Pins
    int digitalCount = dev->info.iPinCount;
    for (j = 0; j < (digitalCount); j++) {

        // PinFunction
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.Pins.%01d.PinFunction\n", __FILE__, __FUNCTION__, prefix, j);
        r = hal_param_u32_newf(HAL_RW, &(IO_data->Pin[j].PinFunction), id, "%s.pins.%01d.PinFunction", prefix, j);
        if (r != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digout.%01d.invert failed\n", __FILE__, __FUNCTION__, prefix, j);
            return r;
        }

        if (PK_CheckPinCapability(dev, j, PK_AllPinCap_digitalInput) == 1) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digin.%01d.deb.out\n", __FILE__, __FUNCTION__, prefix, j);
            r = hal_pin_bit_newf(HAL_OUT, &(IO_data->Pin[j].digin_in), id, "%s.digin.%01d.in", prefix, j);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digin.%01d.in failed\n", __FILE__, __FUNCTION__, prefix, j);
                return r;
            }
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digin.%01d.in-not\n", __FILE__, __FUNCTION__, prefix, j);
            r = hal_pin_bit_newf(HAL_OUT, &(IO_data->Pin[j].digin_in_not), id, "%s.digin.%01d.in-not", prefix, j);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digin.%01d.in-not failed\n", __FILE__, __FUNCTION__, prefix, j);
                return r;
            }

            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digin.%01d.invert\n", __FILE__, __FUNCTION__, prefix, j);
            r = hal_param_bit_newf(HAL_RW, &(IO_data->Pin[j].digin_invert), id, "%s.digin.%01d.invert", prefix, j);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digin.%01d.invert failed\n", __FILE__, __FUNCTION__, prefix, j);
                return r;
            }
        }

        if (PK_CheckPinCapability(dev, j, PK_AllPinCap_digitalOutput) == 1) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digout.%01d.out\n", __FILE__, __FUNCTION__, prefix, j);
            r = hal_pin_bit_newf(HAL_IN, &(IO_data->Pin[j].digout_out), id, "%s.digout.%01d.out", prefix, j);

            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digout.%01d.out failed\n", __FILE__, __FUNCTION__, prefix, j);
                return r;
            }
            IO_data->Pin[j].DigitalValueSet_ignore = 0;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digout.%01d.invert\n", __FILE__, __FUNCTION__, prefix, j);
            r = hal_param_bit_newf(HAL_RW, &(IO_data->Pin[j].digout_invert), id, "%s.digout.%01d.invert", prefix, j);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digout.%01d.invert failed\n", __FILE__, __FUNCTION__, prefix, j);
                return r;
            }
        }

        if (PK_CheckPinCapability(dev, j, PK_AllPinCap_digitalCounter) == 1) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.counter.%01d.value\n", __FILE__, __FUNCTION__, prefix, j);
            r = hal_pin_u32_newf(HAL_OUT, &(IO_data->Pin[j].counter_value), id, "%s.counter.%01d.value", prefix, j);
            if (r != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.counter.%01d.value failed\n", __FILE__, __FUNCTION__, prefix, j);
                return r;
            }
        }
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digin.%01d.deb.out\n", __FILE__, __FUNCTION__, prefix, j);
    r = hal_pin_u32_newf(HAL_OUT, &(IO_data->deb_out), id, "%s.digin.deb.out", prefix, j);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digin.deb.out failed\n", __FILE__, __FUNCTION__, prefix, j);
        return r;
    }
    return r;
}

/**
 * @brief Updates PoKeys I/O states from and to hardware.
 *
 * This function performs periodic synchronization between the PoKeys hardware device
 * and the corresponding LinuxCNC HAL pins. It:
 * - Retrieves current pin configuration, digital I/O states, analog input values,
 *   digital counters, and PWM settings from the device.
 * - Updates internal HAL structures (`IO_data`) with the retrieved values.
 * - Writes analog and digital output values from HAL back to the device if enabled.
 * - Handles logic such as scaling, clamping, and PWM duty calculation.
 * - Optionally reconfigures PWM period or enables/disables channels when values change.
 *
 * @param dev Pointer to the sPoKeysDevice structure representing the connected PoKeys device.
 *
 * @note The static pointer `IO_data` must be initialized with valid memory prior to calling this function.
 * @note The function uses `sleepdur` (if compiled with `ULAPI`) to avoid overrunning communication.
 *
 * @details
 * This function combines the following device interactions:
 * - `PK_PinConfigurationGet()`
 * - `PK_DigitalIOGet()` / `PK_DigitalIOSet()`
 * - `PK_AnalogIOGet()`
 * - `PK_DigitalCounterGet()`
 * - `PK_PWMConfigurationGet()` / `PK_PWMConfigurationSet()` / `PK_PWMUpdate()`
 *
 * @see PKIO_export_pins()
 * @see sPoKeysDevice
 * @see all_IO_data_t
 */
void PKIO_Update(sPoKeysDevice *dev) {
    bool PinConfigurationGet = false;
    bool DigitalIOGet = false;
    bool AnalogIOGet = false;
    bool AnalogIOSet = false;
    bool DigitalCounterGet = false;
    bool DigitalIOSet = false;
    bool doPwmConfig = false;
    bool loopPins = false;
    bool DoPWM = false;

    if (dev->info.iPinCount > 0) {
        *(IO_data->deb_out) = 210;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationGet(dev)\n", __FILE__, __FUNCTION__);
        if (PK_PinConfigurationGet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
            PinConfigurationGet = true;
            *(IO_data->deb_out) = 211;
            loopPins = true;
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PinConfigurationGet(dev) failed\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOGet(dev)\n", __FILE__, __FUNCTION__);
        if (PK_DigitalIOGet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOGet(dev) OK\n", __FILE__, __FUNCTION__);
            *(IO_data->deb_out) = 212;
            DigitalIOGet = true;
            loopPins = true;
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_DigitalIOGet(dev) failed\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_AnalogIOGet(dev)\n", __FILE__, __FUNCTION__);
        if (PK_AnalogIOGet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_AnalogIOGet(dev) OK\n", __FILE__, __FUNCTION__);
            *(IO_data->deb_out) = 213;
            AnalogIOGet = true;
            loopPins = true;
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_AnalogIOGet(dev) failed\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_iDigitalCounters = %d\n", __FILE__, __FUNCTION__, dev->info.iDigitalCounters);
    if (dev->info.iDigitalCounters > 0) {
        *(IO_data->deb_out) = 214;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalCounterGet(dev)\n", __FILE__, __FUNCTION__);
        if (PK_DigitalCounterGet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalCounterGet(dev) OK\n", __FILE__, __FUNCTION__);
            *(IO_data->deb_out) = 215;
            DigitalCounterGet = true;
            loopPins = true;
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_DigitalCounterGet(dev) failed\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PWMCount = %d\n", __FILE__, __FUNCTION__, dev->info.iPWMCount);
    if (dev->info.iPWMCount && DoPWM) {
        *(IO_data->adcout_deb_outv) = 100;
        *(IO_data->adcout_deb_outv) = 100;
        uint32_t PWMperiod; // PWM period, shared among all channels

        uint32_t PWMduty[6];           // PWM duty cycles (range between 0 and PWM period)
        uint8_t PWMenabledChannels[6]; // List of enabled PWM channels
        uint8_t PWMpinIDs[6];
        float PWM_SCale[6];
        float max_v[6];
        float high_limit[6];
        float low_limit[6];
        float PWM_value[6];
        float PWM_OffSet[6];
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev)\n", __FILE__, __FUNCTION__);

        if (PK_PWMConfigurationGet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
            PWMperiod = IO_data->adcout_pwm_period;
// PWMperiod = adcout_pwm_period;
#ifdef ULAPI
            usleep(sleepdur);
#endif
            if (PWMperiod == 0) {
                PWMperiod = 2500;
            }
            PWMperiod = 2500;
            if (dev->PWM.PWMperiod != PWMperiod) {
                dev->PWM.PWMperiod = PWMperiod;
                doPwmConfig = true;
                PK_PWMConfigurationSet(dev);
#ifdef ULAPI
                usleep(sleepdur);
#endif
            }

            if (IO_data->adcout_pwm_period != PWMperiod) {
                IO_data->adcout_pwm_period = PWMperiod;
            }

            // Pins_PinFunction(i)=dev->Pins[i].PinFunction;

            for (int i = 0; i < 6; i++) {
                int PwmId = 5 - i;
                *(IO_data->adcout[PwmId]).deb_setval = 100;
                // adcout_deb_out(PwmId) = 100;

                PWMduty[i] = dev->PWM.PWMduty[i];                         // PWM duty cycles (range between 0 and PWM period)
                PWMenabledChannels[i] = *(IO_data->adcout[PwmId]).enable; // List of enabled PWM channels
                PWMpinIDs[i] = dev->PWM.PWMpinIDs[i];
                IO_data->adcout[PwmId].PinId = PWMpinIDs[i];
                // adcout_PinId(PwmId) = PWMpinIDs[i];
                PWM_SCale[i] = 1;

                PWM_value[i] = *(IO_data->adcout[PwmId]).value;
                PWM_OffSet[i] = IO_data->adcout[PwmId].offset;
                max_v[i] = IO_data->adcout[PwmId].max_v;
                high_limit[i] = IO_data->adcout[PwmId].high_limit;
                low_limit[i] = IO_data->adcout[PwmId].low_limit;

                if (max_v[PwmId] == 0) {
                    if ((dev->DeviceData.DeviceTypeID == PK_DeviceID_PoKeys57CNC) && (PWMpinIDs[i] = 17)) {
                        max_v[PwmId] = 10.000; // Pin17 0-10V
                    } else {
                        max_v[PwmId] = 5.000; // usually pokeys57 provide 0..5V PWM
                    }
                }

                if (high_limit[PwmId] = low_limit[PwmId]) {
                    if (high_limit[PwmId] == 0) {
                        high_limit[PwmId] = max_v[PwmId];
                    }
                }

                if (PWMenabledChannels[i] == true) {
                    float tmp = PWM_value[i] + PWM_OffSet[i];

                    if (tmp <= low_limit[i]) {
                        tmp = low_limit[i];
                    } else if (tmp >= high_limit[i]) {
                        tmp = high_limit[i];
                    }

                    tmp = tmp * PWM_SCale[i];

                    PWMduty[i] = (uint32_t)((tmp / max_v[i]) * PWMperiod);
                } else {
                    PWMduty[i] = 0;
                }

                *(IO_data->adcout[PwmId]).deb_setval = PWMduty[i];

                if (IO_data->adcout[PwmId].scale != PWM_SCale[i]) {
                    IO_data->adcout[PwmId].scale = PWM_SCale[i];
                }

                if (IO_data->adcout[PwmId].max_v != max_v[i]) {
                    IO_data->adcout[PwmId].max_v = max_v[i];
                }

                if (IO_data->adcout[PwmId].high_limit != high_limit[i]) {
                    IO_data->adcout[PwmId].high_limit = high_limit[i];
                }

                if (IO_data->adcout[PwmId].low_limit != low_limit[i]) {
                    IO_data->adcout[PwmId].low_limit = low_limit[i];
                }

                if (dev->PWM.PWMenabledChannels[i] != PWMenabledChannels[i]) {
                    dev->PWM.PWMenabledChannels[i] = PWMenabledChannels[i];
                    doPwmConfig = true;
                }

                if (dev->PWM.PWMduty[i] != PWMduty[i]) {
                    dev->PWM.PWMduty[i] = PWMduty[i];
                    AnalogIOSet = true;
                }
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev) failed\n", __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
        if (AnalogIOSet == true) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMUpdate(dev)\n", __FILE__, __FUNCTION__);
            PK_PWMUpdate(dev);
#ifdef ULAPI
            usleep(sleepdur);
#endif
            if (PK_PWMUpdate(dev) != PK_OK) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PWMUpdate(dev) failed\n", __FILE__, __FUNCTION__);
                PK_PWMUpdate(dev);
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMUpdate(dev) OK\n", __FILE__, __FUNCTION__);
            }
#ifdef ULAPI
            usleep(sleepdur);
#endif
        }
    } else {

        *(IO_data->adcout_deb_outv) = 150;
    }
    if (loopPins == true) { // gets IO data and checks return value
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read IO data\n", __FILE__, __FUNCTION__);

        sPoKeysPinData iPin;
        int AnalogPinOffset = 40;
        int AnalogPinCount = 7;

        int PwmPinOffset = 17;
        int PwmPinCount = 6;

        for (int i = 0; i < dev->info.iPinCount - 1; i++) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Pin %d\n", __FILE__, __FUNCTION__, i);
            if (dev->Pins[i].PinFunction & PK_PinCap_analogInput) {

                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: AnalogInput\n", __FILE__, __FUNCTION__);
                *(IO_data->deb_out) = 2320 + i;
                int AinNr = i - AnalogPinOffset;

                if ((AnalogIOGet = true) && (AinNr >= 0) && (AinNr < AnalogPinCount)) {
                    if (IO_data->adcin[AinNr].scale == 0) {
                        IO_data->adcin[AinNr].scale = 1;
                    }
                    float ainVal = 3.3 * dev->Pins[i].AnalogValue / 4096;

                    *(IO_data->adcin[AinNr].value_raw) = ainVal;
                    rtapi_print_msg(RTAPI_MSG_DBG,
                                    "PoKeys: %s:%s: IO_data->adcin[AinNr].value_raw(%d) = "
                                    "%f\n",
                                    __FILE__, __FUNCTION__, AinNr, IO_data->adcin[AinNr].value_raw);
                    *(IO_data->adcin[AinNr].value) = ainVal * IO_data->adcin[AinNr].scale - IO_data->adcin[AinNr].offset;
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: adc value %d = %f\n", __FILE__, __FUNCTION__, AinNr, IO_data->adcin[AinNr].value);
                }
            }

            if (dev->Pins[i].PinFunction & PK_PinCap_analogOutput) {
            }

            if (dev->Pins[i].PinFunction & PK_PinCap_digitalInput) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalInput\n", __FILE__, __FUNCTION__);
                *(IO_data->deb_out) = 2321 + i;
                if (PK_CheckPinCapability(dev, i, PK_AllPinCap_digitalInput) == 1) {
                    if (DigitalIOGet == true) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalValueGet\n", __FILE__, __FUNCTION__);
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys: %s:%s: "
                                        "dev->Pins[i].DigitalValueGet = %d\n",
                                        __FILE__, __FUNCTION__, dev->Pins[i].DigitalValueGet);

                        if (!dev->Pins[i].DigitalValueGet) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: dev->Pins[%d].DigitalValueGet "
                                            "= 0\n",
                                            __FILE__, __FUNCTION__, i);
                            // Pins_DigitalValueGet(i)=false;
                            *(IO_data->Pin[i]).digin_in = false;
                            *(IO_data->Pin[i].digin_in_not) = true;
                        } else {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: dev->Pins[%d].DigitalValueGet "
                                            "= 1\n",
                                            __FILE__, __FUNCTION__, i);
                            // Pins_DigitalValueGet(i)=true;
                            *(IO_data->Pin[i]).digin_in = true;
                            *(IO_data->Pin[i]).digin_in_not = false;
                        }
                    }
                }
            }

            if (dev->Pins[i].PinFunction & PK_PinCap_digitalOutput) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalOutput\n", __FILE__, __FUNCTION__);
                *(IO_data->deb_out) = 2322 + i;
                if (PK_CheckPinCapability(dev, i, PK_AllPinCap_digitalOutput) == 1) {
                    if (!IO_data->Pin[i].DigitalValueSet_ignore) {
                        bool setDigoutvalue = false;
                        if (*(IO_data->Pin[i]).digout_out == 1) {
                            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[%d].digout_out = 1\n", __FILE__, __FUNCTION__, i);
                            setDigoutvalue = true;
                        }

                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: setDigoutvalue = %d\n", __FILE__, __FUNCTION__, setDigoutvalue);
                        if ((setDigoutvalue) && (!dev->Pins[i].DigitalValueSet)) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: dev->Pins[%d].DigitalValueSet "
                                            "= 1\n",
                                            __FILE__, __FUNCTION__, i);
                            dev->Pins[i].DigitalValueSet = 1;
                            DigitalIOSet = true;
                        } else if ((!setDigoutvalue) && (dev->Pins[i].DigitalValueSet)) {
                            dev->Pins[i].DigitalValueSet = 0;
                            DigitalIOSet = true;
                        }
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalValueSet[%d] = %d\n", __FILE__, __FUNCTION__, i, dev->Pins[i].DigitalValueSet);
                    } else {
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys: %s:%s: "
                                        "IO_data->Pin[%d].DigitalValueSet_ignore = true\n",
                                        __FILE__, __FUNCTION__, i);
                    }
                }
            }

            if (dev->Pins[i].PinFunction & PK_PinCap_digitalCounter) {
                if (PK_CheckPinCapability(dev, i, PK_AllPinCap_digitalCounter) == 1) {
                    // Pins_DigitalCounterAvailable(i)=dev->Pins[i].DigitalCounterAvailable;
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalCounterAvailable[%d] = %d\n", __FILE__, __FUNCTION__, i, dev->Pins[i].DigitalCounterAvailable);
                    if (dev->Pins[i].DigitalCounterAvailable) {
                        // Pins_DigitalCounterValue(i) = dev->Pins[i].DigitalCounterValue;
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:counter value %d = %d\n", __FILE__, __FUNCTION__, i, dev->Pins[i].DigitalCounterValue);
                        dev->Pins[i].DigitalCounterValue = dev->Pins[i].DigitalCounterValue;
                        // Pins_CounterOptions(i) = dev->Pins[i].CounterOptions;
                    }
                }
            }

            // Pins_MappingType(i)=dev->Pins[i].MappingType;
            // Pins_KeyCodeMacroID(i)=dev->Pins[i].KeyCodeMacroID;
            // Pins_KeyModifier(i)=dev->Pins[i].KeyModifier;
            // Pins_downKeyCodeMacroID(i)=dev->Pins[i].downKeyCodeMacroID;
            // Pins_downKeyModifier(i)=dev->Pins[i].downKeyModifier;
            // Pins_upKeyCodeMacroID(i)=dev->Pins[i].upKeyCodeMacroID;
            // Pins_upKeyModifier(i)=dev->Pins[i].upKeyModifier;
            // dev->Pins[i].preventUpdate=Pins_preventUpdate(i);

            *(IO_data->deb_out) = 2330 + i;
        }

        *(IO_data->deb_out) = 236;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev)\n", __FILE__, __FUNCTION__);
        if (DigitalIOSet == true) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev)\n", __FILE__, __FUNCTION__);
            if (PK_DigitalIOSet(dev) != PK_OK) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_DigitalIOSet(dev) failed\n", __FILE__, __FUNCTION__);
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev) passed\n", __FILE__, __FUNCTION__);
            }
#ifdef ULAPI
            usleep(sleepdur);
#endif
            if (PK_DigitalIOSet(dev) != PK_OK) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_DigitalIOSet(dev) failed\n", __FILE__, __FUNCTION__);
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev) passed\n", __FILE__, __FUNCTION__);
            }
#ifdef ULAPI
            usleep(sleepdur);
#endif
        }

        *(IO_data->deb_out) = 237;
    } else {
        *(IO_data->deb_out) = 250;
    }
}

/**
 * @brief Initializes the PoKeys digital and analog I/O configuration.
 *
 * This function configures the I/O pins and PWM settings of the PoKeys device
 * based on either previously applied settings or the current device state.
 *
 * It performs the following actions:
 * - Reads the current pin configuration from the device via `PK_PinConfigurationGet()`.
 * - For each pin:
 *   - Sets the pin function and inversion flags (input/output polarity) based on `ApplyIniSettings`.
 *   - Compares and updates the pin configuration if changes are detected.
 * - Applies updated pin configuration to the device via `PK_PinConfigurationSet()` if needed.
 * - For devices with PWM outputs:
 *   - Retrieves PWM configuration using `PK_PWMConfigurationGet()`.
 *   - Applies period, channel enablement, and value settings (scale, offset, limits).
 *   - Calls `PK_PWMConfigurationSet()` if PWM configuration changes were detected.
 *
 * @param dev Pointer to the PoKeys device structure (`sPoKeysDevice`).
 *
 * @note The global `IO_data` structure must be initialized before calling this function.
 * @note PWM scaling and voltage limits are normalized and defaulted if not set.
 * @note The last PWM period is forcibly set to 2500 if not specified.
 * @note This function is typically called during HAL component initialization.
 *
 * @see ApplyIniSettings
 * @see PK_PinConfigurationGet()
 * @see PK_PinConfigurationSet()
 * @see PK_PWMConfigurationGet()
 * @see PK_PWMConfigurationSet()
 * @see all_IO_data_t
 */
void PKIO_Setup(sPoKeysDevice *dev) {
    bool PinConfigurationSet = false;
    // bool readonly = false;

    // Setting PinFunction
    if (PK_PinConfigurationGet(dev) == PK_OK) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
        // readonly = true;

        for (int i = 0; i < dev->info.iPinCount - 1; i++) {

            if (ApplyIniSettings == false || IO_data->Pin[i].PinFunction == 0) {
                IO_data->Pin[i].PinFunction = dev->Pins[i].PinFunction;

                if (dev->Pins[i].PinFunction & PK_PinCap_digitalOutput) {
                    if (dev->Pins[i].PinFunction & PK_PinCap_invertPin) {
                        IO_data->Pin[i].digout_invert = 1;
                    } else {
                        IO_data->Pin[i].digout_invert = 0;
                    }
                } else if (dev->Pins[i].PinFunction & PK_PinCap_digitalInput) {
                    if (dev->Pins[i].PinFunction & PK_PinCap_invertPin) {
                        IO_data->Pin[i].digin_invert = 1;
                    } else {
                        IO_data->Pin[i].digin_invert = 0;
                    }
                }
            } else {

                // for Digin and Digout Check (IO_data->Pin[i].digout_invert == 1)

                if (dev->Pins[i].PinFunction & PK_PinCap_digitalOutput) {

                    if (IO_data->Pin[i].digout_invert == 1) {

                        if (!(dev->Pins[i].PinFunction & PK_PinCap_invertPin)) {
                            // bitwise set PK_PinCap_invertPin
                            IO_data->Pin[i].PinFunction += PK_PinCap_invertPin;
                        }
                    } else {
                        // bitwise unsset PK_PinCap_invertPin
                        if (dev->Pins[i].PinFunction & PK_PinCap_invertPin) {
                            IO_data->Pin[i].PinFunction -= PK_PinCap_invertPin;
                        }
                    }

                } else if (dev->Pins[i].PinFunction & PK_PinCap_digitalInput) {

                    if (IO_data->Pin[i].digin_invert == 1) {
                        // bitwise set PK_PinCap_invertPin
                        IO_data->Pin[i].PinFunction |= ~(PK_PinCap_invertPin);
                    } else {
                        // bitwise unsset PK_PinCap_invertPin
                        IO_data->Pin[i].PinFunction &= ~(PK_PinCap_invertPin);
                    }
                }

                if (dev->Pins[i].PinFunction != IO_data->Pin[i].PinFunction) {
                    dev->Pins[i].PinFunction = IO_data->Pin[i].PinFunction;
                    PinConfigurationSet = true;
                }
            }
        }

        if (PinConfigurationSet == true) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationSet(dev)\n", __FILE__, __FUNCTION__);
            PK_PinConfigurationSet(dev);
            if (PK_PinConfigurationSet(dev) != PK_OK) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PinConfigurationSet(dev) failed\n", __FILE__, __FUNCTION__);
                PK_PinConfigurationSet(dev);
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationSet(dev) passed\n", __FILE__, __FUNCTION__);
            }
#ifdef ULAPI
            usleep(sleepdur);
#endif
        }
    }

    if (dev->info.iPWMCount) {

        *(IO_data->adcout_deb_outv) = 100;
        *(IO_data->adcout_deb_outv) = 100;
        uint32_t PWMperiod; // PWM period, shared among all channels
        bool doPwmConfig = false;

        uint32_t PWMduty[6];           // PWM duty cycles (range between 0 and PWM period)
        uint8_t PWMenabledChannels[6]; // List of enabled PWM channels
        uint8_t PWMpinIDs[6];
        float PWM_SCale[6];
        float max_v[6];
        float high_limit[6];
        float low_limit[6];
        float PWM_value[6];
        float PWM_OffSet[6];
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev)\n", __FILE__, __FUNCTION__);

        if (PK_PWMConfigurationGet(dev) == PK_OK) {

            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
            PWMperiod = IO_data->adcout_pwm_period;
#ifdef ULAPI
            usleep(sleepdur);
#endif

            if (PWMperiod == 0) {
                PWMperiod = 2500;
            }
            PWMperiod = 2500;
            if (dev->PWM.PWMperiod != PWMperiod) {
                dev->PWM.PWMperiod = PWMperiod;
                doPwmConfig = true;
            }

            if (IO_data->adcout_pwm_period != PWMperiod) {
                IO_data->adcout_pwm_period = PWMperiod;
            }

            for (int i = 0; i < 6; i++) {
                int PwmId = 5 - i;
                *(IO_data->adcout[PwmId]).deb_setval = 100;
                // adcout_deb_out(PwmId) = 100;

                PWMduty[i] = dev->PWM.PWMduty[i];                         // PWM duty cycles (range between 0 and PWM period)
                PWMenabledChannels[i] = *(IO_data->adcout[PwmId]).enable; // List of enabled PWM channels
                PWMpinIDs[i] = dev->PWM.PWMpinIDs[i];
                IO_data->adcout[PwmId].PinId = PWMpinIDs[i];
                // adcout_PinId(PwmId) = PWMpinIDs[i];
                PWM_SCale[i] = 1;

                PWM_value[i] = *(IO_data->adcout[PwmId]).value;
                PWM_OffSet[i] = IO_data->adcout[PwmId].offset;
                max_v[i] = IO_data->adcout[PwmId].max_v;
                high_limit[i] = IO_data->adcout[PwmId].high_limit;
                low_limit[i] = IO_data->adcout[PwmId].low_limit;

                if (PWMenabledChannels[i] == true) {
                    if (max_v[PwmId] == 0) {
                        if ((dev->DeviceData.DeviceTypeID == PK_DeviceID_PoKeys57CNC) && (PWMpinIDs[i] = 17)) {
                            max_v[PwmId] = 10.000; // Pin17 0-10V
                        } else {
                            max_v[PwmId] = 5.000; // usually pokeys57 provide 0..5V PWM
                        }
                    }

                    if (high_limit[PwmId] = low_limit[PwmId]) {
                        if (high_limit[PwmId] == 0) {
                            high_limit[PwmId] = max_v[PwmId];
                        }
                    }
                }

                if (IO_data->adcout[PwmId].scale != PWM_SCale[i]) {
                    IO_data->adcout[PwmId].scale = PWM_SCale[i];
                }

                if (IO_data->adcout[PwmId].max_v != max_v[i]) {
                    IO_data->adcout[PwmId].max_v = max_v[i];
                }

                if (IO_data->adcout[PwmId].high_limit != high_limit[i]) {
                    IO_data->adcout[PwmId].high_limit = high_limit[i];
                }

                if (IO_data->adcout[PwmId].low_limit != low_limit[i]) {
                    IO_data->adcout[PwmId].low_limit = low_limit[i];
                }

                if (dev->PWM.PWMenabledChannels[i] != PWMenabledChannels[i]) {
                    dev->PWM.PWMenabledChannels[i] = PWMenabledChannels[i];
                    doPwmConfig = true;
                }
            }

            if (doPwmConfig == true) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationSet(dev)\n", __FILE__, __FUNCTION__);
                PK_PWMConfigurationSet(dev);
                if (PK_PWMConfigurationSet(dev) != PK_OK) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PWMConfigurationSet(dev) failed\n", __FILE__, __FUNCTION__);
                    PK_PWMConfigurationSet(dev);
                } else {
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationSet(dev) passed\n", __FILE__, __FUNCTION__);
                }
#ifdef ULAPI
                usleep(sleepdur);
#endif
            }
        }
    }
}

/**
 * @brief Reads and applies I/O configuration values for the PoKeys device from the INI file.
 *
 * This function loads pin functions, input/output inversion flags, analog input/output
 * scale and offset values, voltage limits, and PWM period from the [POKEYS] section
 * of the HAL component's INI configuration.
 *
 * It covers:
 * - Digital pin function and inversion flags:
 *   - `Pin_<n>_Function`
 *   - `DigIn_<n>_invert`
 *   - `DigOut_<n>_invert`
 * - Analog output (PWM) settings:
 *   - `AdcOut_<n>_offset`, `AdcOut_<n>_scale`
 *   - `AdcOut_<n>_high_limit`, `AdcOut_<n>_low_limit`, `AdcOut_<n>_max_v`
 *   - `AdcOut_<n>_enable`
 * - Global PWM period setting:
 *   - `AdcOut_PWM_Period`
 * - Analog input scale and offset values:
 *   - `AdcIn_<n>_scale`, `AdcIn_<n>_offset`
 *
 * @param dev Pointer to the PoKeys device structure (`sPoKeysDevice`) used to determine the number of digital and analog pins.
 *
 * @note The global `IO_data` pointer must point to a valid `all_IO_data_t` structure before calling this function.
 * @note Defaults are applied if keys are missing: digital pins get 0, analog input scale defaults to 1, offset to 0.
 * @note The function assumes the corresponding HAL pins and parameters were already created.
 *
 * @see all_IO_data_t
 * @see ini_read_int()
 * @see ini_read_float()
 */
void PKIO_ReadIniFile(sPoKeysDevice *dev) {

    char key[256]; // Puffer für den zusammengesetzten String

    int digitalCount = dev->info.iPinCount;
    for (int i = 0; i < digitalCount; i++) {
        snprintf(key, sizeof(key), "Pin_%i_Function", i);
        IO_data->Pin[i].PinFunction = ini_read_int("POKEYS", key, 0);
    }

    for (int i = 0; i < digitalCount; i++) {
        snprintf(key, sizeof(key), "DigIn_%i_invert", i);
        IO_data->Pin[i].digin_invert = ini_read_int("POKEYS", key, 0);
    }
    for (int i = 0; i < digitalCount; i++) {
        snprintf(key, sizeof(key), "DigOut_%i_invert", i);
        IO_data->Pin[i].digout_invert = ini_read_int("POKEYS", key, 0);
    }

    int analogOutCount = dev->info.iPWMCount;
    for (int j = 0; j < (analogOutCount); j++) {

        snprintf(key, sizeof(key), "AdcOut_%i_offset", j);
        IO_data->adcin[j].offset = ini_read_float("POKEYS", key, 0);

        snprintf(key, sizeof(key), "AdcOut_%i_scale", j);
        IO_data->adcout[j].scale = ini_read_float("POKEYS", key, 0);

        snprintf(key, sizeof(key), "AdcOut_%i_high_limit", j);
        IO_data->adcout[j].high_limit = ini_read_float("POKEYS", key, 0);

        snprintf(key, sizeof(key), "AdcOut_%i_low_limit", j);
        IO_data->adcout[j].low_limit = ini_read_float("POKEYS", key, 0);

        snprintf(key, sizeof(key), "AdcOut_%i_max_v", j);
        IO_data->adcout[j].max_v = ini_read_float("POKEYS", key, 0);

        snprintf(key, sizeof(key), "AdcOut_%i_enable", j);
        *IO_data->adcout[j].enable = ini_read_float("POKEYS", key, 0);
    }
    IO_data->adcout_pwm_period = ini_read_int("POKEYS", "AdcOut_PWM_Period", 0);

    int analogInCount = 7;
    for (int j = 0; j < (analogInCount); j++) {
        snprintf(key, sizeof(key), "AdcIn_%i_scale", j);
        IO_data->adcin[j].scale = ini_read_float("POKEYS", key, 1);

        snprintf(key, sizeof(key), "AdcIn_%i_offset", j);
        IO_data->adcin[j].offset = ini_read_float("POKEYS", key, 0);
    }
}

/**
 * @brief Writes the current PoKeys I/O configuration to the INI file.
 *
 * This function saves all relevant I/O parameters from the current HAL state
 * (`IO_data`) into the [POKEYS] section of the INI file for later reuse.
 * This includes:
 * - Digital pin configuration:
 *   - `Pin_<n>_Function`
 *   - `DigIn_<n>_invert`
 *   - `DigOut_<n>_invert`
 * - Analog output (PWM) configuration:
 *   - `AdcOut_<n>_offset`, `AdcOut_<n>_scale`
 *   - `AdcOut_<n>_high_limit`, `AdcOut_<n>_low_limit`, `AdcOut_<n>_max_v`
 *   - `AdcOut_<n>_enable`
 * - Global PWM period:
 *   - `AdcOut_PWM_Period`
 * - Analog input scale and offset:
 *   - `AdcIn_<n>_scale`, `AdcIn_<n>_offset`
 *
 * @param dev Pointer to the PoKeys device structure (`sPoKeysDevice`), used to determine pin and channel counts.
 *
 * @note The global `IO_data` pointer must point to a valid `all_IO_data_t` structure before calling this function.
 * @note This function is the counterpart to `PKIO_ReadIniFile()`, and is used for persisting configuration across restarts.
 *
 * @see all_IO_data_t
 * @see PKIO_ReadIniFile()
 * @see ini_write_int()
 * @see ini_write_float()
 */
void PKIO_WriteIniFile(sPoKeysDevice *dev) {
    char key[256]; // Puffer für den zusammengesetzten String

    int digitalCount = dev->info.iPinCount;
    for (int i = 0; i < digitalCount; i++) {
        snprintf(key, sizeof(key), "Pin_%i_Function", i);
        ini_write_int("POKEYS", key, IO_data->Pin[i].PinFunction);
    }

    for (int i = 0; i < digitalCount; i++) {
        snprintf(key, sizeof(key), "DigIn_%i_invert", i);
        ini_write_int("POKEYS", key, IO_data->Pin[i].digin_invert);
    }

    for (int i = 0; i < digitalCount; i++) {

        snprintf(key, sizeof(key), "DigOut_%i_invert", i);
        ini_write_int("POKEYS", key, IO_data->Pin[i].digout_invert);
    }

    int analogOutCount = dev->info.iPWMCount;
    for (int j = 0; j < (analogOutCount); j++) {
        snprintf(key, sizeof(key), "AdcOut_%i_offset", j);
        ini_write_float("POKEYS", key, IO_data->adcout[j].offset);

        snprintf(key, sizeof(key), "AdcOut_%i_scale", j);
        ini_write_float("POKEYS", key, IO_data->adcout[j].scale);

        snprintf(key, sizeof(key), "AdcOut_%i_high_limit", j);
        ini_write_float("POKEYS", key, IO_data->adcout[j].high_limit);

        snprintf(key, sizeof(key), "AdcOut_%i_low_limit", j);
        ini_write_float("POKEYS", key, IO_data->adcout[j].low_limit);

        snprintf(key, sizeof(key), "AdcOut_%i_max_v", j);
        ini_write_float("POKEYS", key, IO_data->adcout[j].max_v);

        snprintf(key, sizeof(key), "AdcOut_%i_enable", j);
        ini_write_int("POKEYS", key, *IO_data->adcout[j].enable);
    }
    ini_write_int("POKEYS", "AdcOut_PWM_Period", IO_data->adcout_pwm_period);

    int analogInCount = 7;
    for (int j = 0; j < (analogInCount); j++) {
        snprintf(key, sizeof(key), "AdcIn_%i_scale", j);
        ini_write_float("POKEYS", key, IO_data->adcin[j].scale);

        snprintf(key, sizeof(key), "AdcIn_%i_offset", j);
        ini_write_float("POKEYS", key, IO_data->adcin[j].offset);
    }
}

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "pin:adcout.#.deb.out:u32:6:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcout.#.deb.setval:u32:6:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcout.deb.outv:u32:0:out::None:None");

MODULE_INFO(linuxcnc, "pin:counter.#.value:u32:55:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcin.#.value-raw:float:7:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcin.#.value:float:7:out::None:None");
MODULE_INFO(linuxcnc, "param:adcin.#.scale:float:7:rw:The input voltage (or current) will be "
                      "multiplied by scale before being output to value.:None:None");
MODULE_INFO(linuxcnc, "param:adcin.#.offset:float:7:rw:This will be subtracted "
                      "from the hardware input voltage (or current) after the "
                      "scale multiplier has been applied.:None:None");
MODULE_INFO(linuxcnc, "pin:digin.#.in:bit:55:out::None:None");
MODULE_INFO(linuxcnc, "pin:digin.#.in-not:bit:55:out::None:None");
MODULE_INFO(linuxcnc, "pin:digout.#.out:bit:55:in::None:None");
MODULE_INFO(linuxcnc, "param:digout.#.invert:bit:55:rw:If TRUE, out is "
                      "inverted before writing to the hardware.:None:None");

MODULE_INFO(linuxcnc, "pin:adcout.#.value:float:6:in::None:None");
MODULE_INFO(linuxcnc, "pin:adcout.#.enable:bit:6:in::None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.offset:float:6:rw:This will be added to "
                      "the value before the hardware is updated:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.scale:float:6:rw:This should be set so that an "
                      "input of 1 on the value pin will cause 1V:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.high_limit:float:6:rw:When calculating the value "
                      "to output to the hardware, if value +offset is greater than "
                      "high_limit, then high_limit will be used instead.:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.low_limit:float:6:rw:When calculating the value to "
                      "output to the hardware, if value +offset is less than low_limit, "
                      "then low_limit will be used instead:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.max_v:float:6:rw:max v:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.PinId:u32:6:r:max v:None:None");
MODULE_INFO(linuxcnc, "param:adcout.pwm.period:u32:0:rw:PWM period, shared "
                      "among all channels:None:None");
#endif // MODULE_INFO