/**
 * @file pokeys.c
 * @brief HAL component for integrating PoKeys57 devices with LinuxCNC.
 *
 * This file defines the main HAL component `pokeys` for userspace operation. It provides the entry point
 * for initializing, updating, and cleaning up the PoKeys HAL module. The component manages digital and analog I/O,
 * encoder and counter inputs, analog outputs, watchdog handling, and generic communication with PoKeys devices
 * over Ethernet or USB.
 *
 * Key functionalities include:
 * - Detection and initialization of connected PoKeys devices.
 * - Real-time synchronization and state updates via `update()` and `read_data()`.
 * - HAL pin and parameter creation via helper functions in `PokeysCompPulsEngine_base.c`.
 * - Integration of Pulse Engine v2 features, if enabled.
 * - Handling of HAL real-time lifecycle: `init`, `exit`, `ready`, `update`.
 *
 * This file is typically compiled as the main HAL component (via `comp --userspace pokeys.comp`).
 * It relies on PoKeysLib and its submodules to communicate with hardware.
 *
 * @note This component supports multiple devices and dynamically allocates data structures.
 *       It also checks device capabilities and adapts HAL pin creation accordingly.
 *
 * @author zarfld
 * @date 2024
 * @copyright MIT License
 *
 * @see PoKeysLib.h
 * @see pokeys.c
 * @see PokeysCompPulsEngine_base.c
 * @see PoKeysCompPulseEngine_v2.c
 */
/**
 * @defgroup hal_component_pokeys HAL Component: pokeys
 * @brief Main HAL component that integrates PoKeys devices into LinuxCNC.
 *
 * Handles HAL lifecycle, device discovery, data polling, and pin creation.
 * Supports digital/analog I/O, encoders, counters, analog outputs, and watchdog logic.
 */

#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif

#if defined(ULAPI) && defined(RTAPI)
#error "Both ULAPI and RTAPI defined. This should not happen."
#endif

#ifndef RTAPI
#ifndef ULAPI
#define ULAPI
#endif
#endif

#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "rtapi_math64.h"
#include <unistd.h> /* UNIX standard function definitions */

// #include "<math.h>"
#include "PoKeysLib.h"
#include "PoKeysComp.h"
#include "pokeys_ini.h"
#include "rtapi.h"
// #include "rtapi_app.h"
#include "hal.h"
#include "stdio.h"
#include "PoKeysCompEncoders.c"
#include "PoKeysCompPoExtBus.c"
#include "PoKeysCompPoNet.c"
#include "PoKeysCompIO.c"
#include "PoKeysCompPulseEngine_v2.c"
#include <stdlib.h>
#include "inifile.h"

static int comp_id = 0;
sPoKeysDevice *dev = NULL;

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "component:pokeys:(uSpace)PoKeys IO driver");
MODULE_INFO(linuxcnc, "pin:enum_usb_dev:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:enum_fusb_dev:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:enum_udp_dev:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:deb.out:s32:0:out::None:None");

MODULE_INFO(linuxcnc, "pin:err:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:connected:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:connected.usb:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:connected.fusb:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:connected.udp:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:connected.net:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "param:devSerial:u32:0:rw:PoKeys device serial number:None:None");
MODULE_INFO(linuxcnc, "pin:alive:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:machine_is_on:bit:0:in::None:None");

// info pins
MODULE_INFO(linuxcnc, "pin:info.PinCount:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.PWMCount:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.BasicEncoderCount:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.EncodersCount:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.FastEncoders:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.UltraFastEncoders:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.PWMinternalFrequency:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.AnalogInputs:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.KeyMapping:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.TriggeredKeyMapping:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.KeyRepeatDelay:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.DigitalCounters:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.JoystickButtonAxisMapping:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.JoystickAnalogToDigitalMapping:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.Macros:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.MatrixKeyboard:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.MatrixKeyboardTriggeredMapping:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.LCD:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.MatrixLED:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.ConnectionSignal:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.PoExtBus:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.PoNET:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.AnalogFiltering:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.InitOutputsStart:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.protI2C:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.prot1wire:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.AdditionalOptions:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.LoadStatus:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.CustomDeviceName:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.PoTLog27support:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.SensorList:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.WebInterface:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.FailSafeSettings:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.JoystickHATswitch:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.PulseEngine:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.PulseEnginev2:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:info.EasySensors:u32:0:out::None:None");

// rtc pins
MODULE_INFO(linuxcnc, "pin:rtc.sec:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.min:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.hour:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.dow:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.dom:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.tmp:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.doy:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.month:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.year:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.loopcount:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.lastmin:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.lastsec:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.loop_frequ:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.loop_frequ_demand:u32:0:in::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.sec_ret:u32:0:in::None:None");
MODULE_INFO(linuxcnc, "pin:rtc.hal_latency:u32:0:out::None:None");

MODULE_INFO(linuxcnc, "license:GPL");
MODULE_INFO(linuxcnc, "author:Dominik Zarfl");
MODULE_LICENSE("GPL");
#endif // MODULE_INFO
/**
 * @brief     PoKeys component state
 *
 */
struct __comp_state {

    struct __comp_state *_next;
    hal_u32_t *enum_usb_dev;
    hal_u32_t *enum_fusb_dev;
    hal_u32_t *enum_udp_dev;

    hal_s32_t *deb_out;
    hal_bit_t *err;
    hal_bit_t *connected;
    hal_bit_t *connected_usb;
    hal_bit_t *connected_fusb;
    hal_bit_t *connected_udp;
    hal_bit_t *connected_net;
    hal_bit_t *alive;
    hal_bit_t *machine_is_on;

    // info pins
    hal_u32_t *info_PinCount;
    hal_u32_t *info_PWMCount;
    hal_u32_t *info_BasicEncoderCount;
    hal_u32_t *info_EncodersCount;
    hal_u32_t *info_FastEncoders;
    hal_u32_t *info_UltraFastEncoders;
    hal_u32_t *info_PWMinternalFrequency;
    hal_u32_t *info_AnalogInputs;
    hal_u32_t *info_KeyMapping;
    hal_u32_t *info_TriggeredKeyMapping;
    hal_u32_t *info_KeyRepeatDelay;
    hal_u32_t *info_DigitalCounters;
    hal_u32_t *info_JoystickButtonAxisMapping;
    hal_u32_t *info_JoystickAnalogToDigitalMapping;
    hal_u32_t *info_Macros;
    hal_u32_t *info_MatrixKeyboard;
    hal_u32_t *info_MatrixKeyboardTriggeredMapping;
    hal_u32_t *info_LCD;
    hal_u32_t *info_MatrixLED;
    hal_u32_t *info_ConnectionSignal;
    hal_u32_t *info_PoExtBus;
    hal_u32_t *info_PoNET;
    hal_u32_t *info_AnalogFiltering;
    hal_u32_t *info_InitOutputsStart;
    hal_u32_t *info_protI2C;
    hal_u32_t *info_prot1wire;
    hal_u32_t *info_AdditionalOptions;
    hal_u32_t *info_LoadStatus;
    hal_u32_t *info_CustomDeviceName;
    hal_u32_t *info_PoTLog27support;
    hal_u32_t *info_SensorList;
    hal_u32_t *info_WebInterface;
    hal_u32_t *info_FailSafeSettings;
    hal_u32_t *info_JoystickHATswitch;
    hal_u32_t *info_PulseEngine;
    hal_u32_t *info_PulseEnginev2;
    hal_u32_t *info_EasySensors;

    all_PoExtBus_data_t *PoExtBus_data;

    PEv2_data_t *PEv2_data;

    hal_u32_t *rtc_sec;
    hal_u32_t *rtc_min;
    hal_u32_t *rtc_hour;
    hal_u32_t *rtc_dow;
    hal_u32_t *rtc_dom;
    hal_u32_t *rtc_tmp;
    hal_u32_t *rtc_doy;
    hal_u32_t *rtc_month;
    hal_u32_t *rtc_year;
    hal_u32_t *rtc_loopcount;
    hal_u32_t *rtc_lastmin;
    hal_u32_t *rtc_lastsec;
    hal_u32_t *rtc_loop_frequ;
    hal_u32_t *rtc_loop_frequ_demand;
    hal_u32_t *rtc_sec_ret;
    hal_u32_t *rtc_hal_latency;

    all_IO_data_t *IO_data;

    all_encoder_data_t *encoder_data;
    all_PoNET_data_t *poNET_data;

    hal_u32_t devSerial;
};

struct __comp_state *__comp_first_inst = 0, *__comp_last_inst = 0;

static int __comp_get_data_size(void);
static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg);
uint32_t device_id = 0;
bool ApplyIniSettings;
#undef TRUE
#define TRUE (1)
#undef FALSE
#define FALSE (0)
#undef true
#define true (1)
#undef false
#define false (0)
/**
 * @brief     Get the size of the component state
 *
 * @return    size of the component state
 */

static int export(char *prefix, long extra_arg) {
    int r = 0;
    int j = 0;
    int sz = sizeof(struct __comp_state) + __comp_get_data_size();
    struct __comp_state *inst = hal_malloc(sz);
    memset(inst, 0, sz);
    r = extra_setup(inst, prefix, extra_arg);
    if (r != 0)
        return r;

    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: export %s\n", __FILE__, __FUNCTION__, prefix);

    // PoExtBus Pins
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PoExtBus pins\n");
    r = PKPoExtBus_export_pins(prefix, extra_arg, comp_id, 10, inst->PoExtBus_data);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PKPoExtBus_export_pins failed\n");
        return r;
    }
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PoExtBus pins done \n");

    // Encoder pins
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: Encoder pins\n");
    r = PKEncoder_export_pins(prefix, extra_arg, comp_id, 29, *&(inst->encoder_data));
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PKEncoder_export_pins failed\n");
        return r;
    }
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: Encoder pins done \n");

    // params
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: Encoder params\n");
    r = PKEncoder_export_params(prefix, extra_arg, comp_id, 29);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PKEncoder_export_params failed\n");
        return r;
    }
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: Encoder params done \n");

    // int PKPoNet_export_pins(char *prefix, long extra_arg, int id, int njoints, all_PoNET_data_t *poNET_data, sPoKeysDevice *dev)
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PoNET pins\n");
    r = PKPoNet_export_pins(prefix, extra_arg, comp_id, 16, inst->poNET_data, dev);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PKPoNet_export_pins failed\n");
        return r;
    }
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PoNET pins done \n");

    // PoKeys IO pins
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: IO pins\n");
    r = PKIO_export_pins(prefix, extra_arg, comp_id, inst->IO_data, dev);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PKIO_export_pins failed\n");
        return r;
    }
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: IO pins done \n");

    // PoKeys PulseEngine pins
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PulseEngine pins\n");
    r = PKPEv2_export_pins(prefix, extra_arg, comp_id, inst->PEv2_data, dev);
    if (r != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PKPulseEngine_export_pins failed\n");
        return r;
    }
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: PulseEngine pins done \n");

    r = hal_pin_u32_newf(HAL_OUT, &(inst->enum_usb_dev), comp_id, "%s.enum-usb-dev", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->enum_fusb_dev), comp_id, "%s.enum-fusb-dev", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->enum_udp_dev), comp_id, "%s.enum-udp-dev", prefix);
    if (r != 0)
        return r;
    r = hal_pin_s32_newf(HAL_OUT, &(inst->deb_out), comp_id, "%s.deb.out", prefix);
    if (r != 0)
        return r;

    r = hal_pin_bit_newf(HAL_OUT, &(inst->err), comp_id, "%s.err", prefix);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->connected), comp_id, "%s.connected", prefix);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->connected_usb), comp_id, "%s.connected.usb", prefix);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->connected_fusb), comp_id, "%s.connected.fusb", prefix);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->connected_udp), comp_id, "%s.connected.udp", prefix);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->connected_net), comp_id, "%s.connected.net", prefix);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->alive), comp_id, "%s.alive", prefix);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_IN, &(inst->machine_is_on), comp_id, "%s.machine-is-on", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PinCount), comp_id, "%s.info.PinCount", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PWMCount), comp_id, "%s.info.PWMCount", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_BasicEncoderCount), comp_id, "%s.info.BasicEncoderCount", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_EncodersCount), comp_id, "%s.info.EncodersCount", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_FastEncoders), comp_id, "%s.info.FastEncoders", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_UltraFastEncoders), comp_id, "%s.info.UltraFastEncoders", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PWMinternalFrequency), comp_id, "%s.info.PWMinternalFrequency", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_AnalogInputs), comp_id, "%s.info.AnalogInputs", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_KeyMapping), comp_id, "%s.info.KeyMapping", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_TriggeredKeyMapping), comp_id, "%s.info.TriggeredKeyMapping", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_KeyRepeatDelay), comp_id, "%s.info.KeyRepeatDelay", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_DigitalCounters), comp_id, "%s.info.DigitalCounters", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_JoystickButtonAxisMapping), comp_id, "%s.info.JoystickButtonAxisMapping", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_JoystickAnalogToDigitalMapping), comp_id, "%s.info.JoystickAnalogToDigitalMapping", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_Macros), comp_id, "%s.info.Macros", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_MatrixKeyboard), comp_id, "%s.info.MatrixKeyboard", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_MatrixKeyboardTriggeredMapping), comp_id, "%s.info.MatrixKeyboardTriggeredMapping", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_LCD), comp_id, "%s.info.LCD", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_MatrixLED), comp_id, "%s.info.MatrixLED", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_ConnectionSignal), comp_id, "%s.info.ConnectionSignal", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PoExtBus), comp_id, "%s.info.PoExtBus", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PoNET), comp_id, "%s.info.PoNET", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_AnalogFiltering), comp_id, "%s.info.AnalogFiltering", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_InitOutputsStart), comp_id, "%s.info.InitOutputsStart", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_protI2C), comp_id, "%s.info.protI2C", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_prot1wire), comp_id, "%s.info.prot1wire", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_AdditionalOptions), comp_id, "%s.info.AdditionalOptions", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_LoadStatus), comp_id, "%s.info.LoadStatus", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_CustomDeviceName), comp_id, "%s.info.CustomDeviceName", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PoTLog27support), comp_id, "%s.info.PoTLog27support", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_SensorList), comp_id, "%s.info.SensorList", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_WebInterface), comp_id, "%s.info.WebInterface", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_FailSafeSettings), comp_id, "%s.info.FailSafeSettings", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_JoystickHATswitch), comp_id, "%s.info.JoystickHATswitch", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PulseEngine), comp_id, "%s.info.PulseEngine", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_PulseEnginev2), comp_id, "%s.info.PulseEnginev2", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->info_EasySensors), comp_id, "%s.info.EasySensors", prefix);
    if (r != 0)
        return r;

    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_sec), comp_id, "%s.rtc.sec", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_min), comp_id, "%s.rtc.min", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_hour), comp_id, "%s.rtc.hour", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_dow), comp_id, "%s.rtc.dow", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_dom), comp_id, "%s.rtc.dom", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_tmp), comp_id, "%s.rtc.tmp", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_doy), comp_id, "%s.rtc.doy", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_month), comp_id, "%s.rtc.month", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_year), comp_id, "%s.rtc.year", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_loopcount), comp_id, "%s.rtc.loopcount", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_lastmin), comp_id, "%s.rtc.lastmin", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_lastsec), comp_id, "%s.rtc.lastsec", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_loop_frequ), comp_id, "%s.rtc.loop-frequ", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_IN, &(inst->rtc_loop_frequ_demand), comp_id, "%s.rtc.loop-frequ-demand", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_IN, &(inst->rtc_sec_ret), comp_id, "%s.rtc.sec-ret", prefix);
    if (r != 0)
        return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->rtc_hal_latency), comp_id, "%s.rtc.hal-latency", prefix);
    if (r != 0)
        return r;

    r = hal_param_u32_newf(HAL_RW, &(inst->devSerial), comp_id, "%s.devSerial", prefix);
    if (r != 0)
        return r;

    pokeys_read_ini(dev);
    if (__comp_last_inst)
        __comp_last_inst->_next = inst;
    __comp_last_inst = inst;
    if (!__comp_first_inst)
        __comp_first_inst = inst;
    return 0;
}
static int default_count = 1, count = 0;
char *names[16] = {
    0,
};

/**
 * @function rtapi_app_main
 * @brief   Main function for the PoKeys component
 *
 */
int rtapi_app_main(void) {
    int r = 0;
    int i;
    comp_id = hal_init("pokeys");
    if (comp_id < 0)
        return comp_id;
    if (count && names[0]) {
        rtapi_print_msg(RTAPI_MSG_ERR, "count= and names= are mutually exclusive\n");
        return -EINVAL;
    }
    if (!count && !names[0])
        count = default_count;
    if (count) {
        for (i = 0; i < count; i++) {
            char buf[HAL_NAME_LEN + 1];
            rtapi_snprintf(buf, sizeof(buf), "pokeys.%d", i);
            r = export(buf, i);
            if (r != 0)
                break;
        }
    } else {
        int max_names = sizeof(names) / sizeof(names[0]);
        for (i = 0; (i < max_names) && names[i]; i++) {
            if (strlen(names[i]) < 1) {
                rtapi_print_msg(RTAPI_MSG_ERR, "names[%d] is invalid (empty string)\n", i);
                r = -EINVAL;
                break;
            }
            r = export(names[i], i);
            if (r != 0)
                break;
        }
    }
    if (r) {
        hal_exit(comp_id);
    } else {
        hal_ready(comp_id);
    }
    return r;
}

/**
 * @brief   Exit function for the PoKeys component
 *
 */
void rtapi_app_exit(void) {
    hal_exit(comp_id);
}
static void user_mainloop(void);

/**
 * @brief   Main loop for the PoKeys component
 *
 */
int __comp_parse_count(int *argc, char **argv) {
    int i;
    for (i = 0; i < *argc; i++) {
        if (strncmp(argv[i], "count=", 6) == 0) {
            errno = 0;
            count = strtoul(&argv[i][6], NULL, 0);
            for (; i + 1 < *argc; i++) {
                argv[i] = argv[i + 1];
            }
            argv[i] = NULL;
            (*argc)--;
            if (errno == 0) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief  Parse the names from the command line arguments
 *
 * @param argc  Pointer to the argument count
 * @param argv  Pointer to the argument vector
 * @return int  1 if names were found, 0 otherwise
 */
int __comp_parse_names(int *argc, char **argv) {
    int i;
    for (i = 0; i < *argc; i++) {
        if (strncmp(argv[i], "names=", 6) == 0) {
            char *p = &argv[i][6];
            int j;
            for (; i + 1 < *argc; i++) {
                argv[i] = argv[i + 1];
            }
            argv[i] = NULL;
            (*argc)--;
            for (j = 0; j < 16; j++) {
                names[j] = strtok(p, ",");
                p = NULL;
                if (names[j] == NULL) {
                    return 1;
                }
            }
            return 1;
        }
    }
    return 0;
}

int argc = 0;
char **argv = 0;
/**
 * @brief  Main function for the PoKeys component
 *
 * @param argc_  Argument count
 * @param argv_  Argument vector
 * @return int   0 on success, 1 on failure
 */
int main(int argc_, char **argv_) {
    argc = argc_;
    argv = argv_;
    int found_count, found_names;
    found_count = __comp_parse_count(&argc, argv);
    found_names = __comp_parse_names(&argc, argv);
    if (found_count && found_names) {
        rtapi_print_msg(RTAPI_MSG_ERR, "count= and names= are mutually exclusive\n");
        return 1;
    }

    if (rtapi_app_main() < 0)
        return 1;
    user_mainloop();
    rtapi_app_exit();
    return 0;
}

#undef FUNCTION
#define FUNCTION(name) static void name(struct __comp_state *__comp_inst, long period)
#undef EXTRA_SETUP
#define EXTRA_SETUP() static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg)
#undef EXTRA_CLEANUP
#define EXTRA_CLEANUP() static void extra_cleanup(void)
#undef fperiod
#define fperiod (period * 1e-9)
#undef enum_usb_dev
#define enum_usb_dev (*__comp_inst->enum_usb_dev)
#undef enum_fusb_dev
#define enum_fusb_dev (*__comp_inst->enum_fusb_dev)
#undef enum_udp_dev
#define enum_udp_dev (*__comp_inst->enum_udp_dev)
#undef deb_out
#define deb_out (*__comp_inst->deb_out)
#undef adcout_deb_out
#define adcout_deb_out(i) (*(__comp_inst->adcout_deb_out[i]))
#undef adcout_deb_setval
#define adcout_deb_setval(i) (*(__comp_inst->adcout_deb_setval[i]))
#undef adcout_deb_outv
#define adcout_deb_outv (*__comp_inst->adcout_deb_outv)
#undef err
#define err (*__comp_inst->err)
#undef connected
#define connected (*__comp_inst->connected)
#undef connected_usb
#define connected_usb (*__comp_inst->connected_usb)
#undef connected_fusb
#define connected_fusb (*__comp_inst->connected_fusb)
#undef connected_udp
#define connected_udp (*__comp_inst->connected_udp)
#undef connected_net
#define connected_net (*__comp_inst->connected_net)
#undef alive
#define alive (*__comp_inst->alive)
#undef machine_is_on
#define machine_is_on (0 + *__comp_inst->machine_is_on)
#undef info_PinCount
#define info_PinCount (*__comp_inst->info_PinCount)
#undef info_PWMCount
#define info_PWMCount (*__comp_inst->info_PWMCount)
#undef info_BasicEncoderCount
#define info_BasicEncoderCount (*__comp_inst->info_BasicEncoderCount)
#undef info_EncodersCount
#define info_EncodersCount (*__comp_inst->info_EncodersCount)
#undef info_FastEncoders
#define info_FastEncoders (*__comp_inst->info_FastEncoders)
#undef info_UltraFastEncoders
#define info_UltraFastEncoders (*__comp_inst->info_UltraFastEncoders)
#undef info_PWMinternalFrequency
#define info_PWMinternalFrequency (*__comp_inst->info_PWMinternalFrequency)
#undef info_AnalogInputs
#define info_AnalogInputs (*__comp_inst->info_AnalogInputs)
#undef info_KeyMapping
#define info_KeyMapping (*__comp_inst->info_KeyMapping)
#undef info_TriggeredKeyMapping
#define info_TriggeredKeyMapping (*__comp_inst->info_TriggeredKeyMapping)
#undef info_KeyRepeatDelay
#define info_KeyRepeatDelay (*__comp_inst->info_KeyRepeatDelay)
#undef info_DigitalCounters
#define info_DigitalCounters (*__comp_inst->info_DigitalCounters)
#undef info_JoystickButtonAxisMapping
#define info_JoystickButtonAxisMapping (*__comp_inst->info_JoystickButtonAxisMapping)
#undef info_JoystickAnalogToDigitalMapping
#define info_JoystickAnalogToDigitalMapping (*__comp_inst->info_JoystickAnalogToDigitalMapping)
#undef info_Macros
#define info_Macros (*__comp_inst->info_Macros)
#undef info_MatrixKeyboard
#define info_MatrixKeyboard (*__comp_inst->info_MatrixKeyboard)
#undef info_MatrixKeyboardTriggeredMapping
#define info_MatrixKeyboardTriggeredMapping (*__comp_inst->info_MatrixKeyboardTriggeredMapping)
#undef info_LCD
#define info_LCD (*__comp_inst->info_LCD)
#undef info_MatrixLED
#define info_MatrixLED (*__comp_inst->info_MatrixLED)
#undef info_ConnectionSignal
#define info_ConnectionSignal (*__comp_inst->info_ConnectionSignal)
#undef info_PoExtBus
#define info_PoExtBus (*__comp_inst->info_PoExtBus)
#undef info_PoNET
#define info_PoNET (*__comp_inst->info_PoNET)
#undef info_AnalogFiltering
#define info_AnalogFiltering (*__comp_inst->info_AnalogFiltering)
#undef info_InitOutputsStart
#define info_InitOutputsStart (*__comp_inst->info_InitOutputsStart)
#undef info_protI2C
#define info_protI2C (*__comp_inst->info_protI2C)
#undef info_prot1wire
#define info_prot1wire (*__comp_inst->info_prot1wire)
#undef info_AdditionalOptions
#define info_AdditionalOptions (*__comp_inst->info_AdditionalOptions)
#undef info_LoadStatus
#define info_LoadStatus (*__comp_inst->info_LoadStatus)
#undef info_CustomDeviceName
#define info_CustomDeviceName (*__comp_inst->info_CustomDeviceName)
#undef info_PoTLog27support
#define info_PoTLog27support (*__comp_inst->info_PoTLog27support)
#undef info_SensorList
#define info_SensorList (*__comp_inst->info_SensorList)
#undef info_WebInterface
#define info_WebInterface (*__comp_inst->info_WebInterface)
#undef info_FailSafeSettings
#define info_FailSafeSettings (*__comp_inst->info_FailSafeSettings)
#undef info_JoystickHATswitch
#define info_JoystickHATswitch (*__comp_inst->info_JoystickHATswitch)
#undef info_PulseEngine
#define info_PulseEngine (*__comp_inst->info_PulseEngine)
#undef info_PulseEnginev2
#define info_PulseEnginev2 (*__comp_inst->info_PulseEnginev2)
#undef info_EasySensors
#define info_EasySensors (*__comp_inst->info_EasySensors)
#undef rtc_sec
#define rtc_sec (*__comp_inst->rtc_sec)
#undef rtc_min
#define rtc_min (*__comp_inst->rtc_min)
#undef rtc_hour
#define rtc_hour (*__comp_inst->rtc_hour)
#undef rtc_dow
#define rtc_dow (*__comp_inst->rtc_dow)
#undef rtc_dom
#define rtc_dom (*__comp_inst->rtc_dom)
#undef rtc_tmp
#define rtc_tmp (*__comp_inst->rtc_tmp)
#undef rtc_doy
#define rtc_doy (*__comp_inst->rtc_doy)
#undef rtc_month
#define rtc_month (*__comp_inst->rtc_month)
#undef rtc_year
#define rtc_year (*__comp_inst->rtc_year)
#undef rtc_loopcount
#define rtc_loopcount (*__comp_inst->rtc_loopcount)
#undef rtc_lastmin
#define rtc_lastmin (*__comp_inst->rtc_lastmin)
#undef rtc_lastsec
#define rtc_lastsec (*__comp_inst->rtc_lastsec)
#undef rtc_loop_frequ
#define rtc_loop_frequ (*__comp_inst->rtc_loop_frequ)
#undef rtc_loop_frequ_demand
#define rtc_loop_frequ_demand (0 + *__comp_inst->rtc_loop_frequ_demand)
#undef rtc_sec_ret
#define rtc_sec_ret (0 + *__comp_inst->rtc_sec_ret)
#undef rtc_hal_latency
#define rtc_hal_latency (*__comp_inst->rtc_hal_latency)
#undef counter_value
#define counter_value(i) (*(__comp_inst->counter_value[i]))
#undef adcin_value_raw
#define adcin_value_raw(i) (*(__comp_inst->adcin_value_raw[i]))
#undef adcin_value
#define adcin_value(i) (*(__comp_inst->adcin_value[i]))
#undef digin_in
#define digin_in(i) (*(__comp_inst->digin_in[i]))
#undef digin_in_not
#define digin_in_not(i) (*(__comp_inst->digin_in_not[i]))
#undef digout_out
#define digout_out(i) (0 + *(__comp_inst->digout_out[i]))
#undef encoder_count
#undef adcout_value
#define adcout_value(i) (0 + *(__comp_inst->adcout_value[i]))
#undef adcout_enable
#define adcout_enable(i) (0 + *(__comp_inst->adcout_enable[i]))
#undef devSerial
#define devSerial (__comp_inst->devSerial)
#undef adcin_scale
#define adcin_scale(i) (__comp_inst->adcin_scale[i])
#undef adcin_offset
#define adcin_offset(i) (__comp_inst->adcin_offset[i])
#undef digout_invert
#define digout_invert(i) (__comp_inst->digout_invert[i])
// #undef encoder_scale
// #define encoder_scale(i) (__comp_inst->encoder_scale[i])
#undef adcout_offset
#define adcout_offset(i) (__comp_inst->adcout_offset[i])
#undef adcout_scale
#define adcout_scale(i) (__comp_inst->adcout_scale[i])
#undef adcout_high_limit
#define adcout_high_limit(i) (__comp_inst->adcout_high_limit[i])
#undef adcout_low_limit
#define adcout_low_limit(i) (__comp_inst->adcout_low_limit[i])
#undef adcout_max_v
#define adcout_max_v(i) (__comp_inst->adcout_max_v[i])
#undef adcout_PinId
#define adcout_PinId(i) (__comp_inst->adcout_PinId[i])
#undef adcout_pwm_period
#define adcout_pwm_period (__comp_inst->adcout_pwm_period)
#undef FOR_ALL_INSTS
#define FOR_ALL_INSTS()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        \
    struct __comp_state *__comp_inst;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          \
    for (__comp_inst = __comp_first_inst; __comp_inst; __comp_inst = __comp_inst->_next)

// #line 863 "/home/zarfld/Documents/LinuxCnc_PokeysLibComp/pokeys_uspace/pokeys.c"

static int comp_id; /* component ID */
// bool initEncodersDone = 0;

bool setPkConfig = false;
bool setPinConfig = false;
bool I2C_isscanning = false;
bool secBlink = false;
unsigned Loop_Frequ = 0;
// uint8_t kbd48CNC_Counter[48];

uint8_t rtc_latencycheck_set = 0;
int rtc_latencyCounter = 0;

int i = 0;

float temp_MaxAcceleration[8];
float minAccel = 0.0001;
/**
 * @brief  Structure to hold the state of the component
 *
 */
typedef struct {
    uint8_t matrixKBconfiguration;    // Matrix keyboard configuration (set to 1 to enable matrix keyboard support)
    uint8_t matrixKBwidth;            // Matrix keyboard width (number of columns)
    uint8_t matrixKBheight;           // Matrix keyboard height (number of rows)
    uint8_t reserved[5];              // placeholder
    uint8_t matrixKBcolumnsPins[8];   // List of matrix keyboard column connections
    uint8_t matrixKBrowsPins[16];     // List of matrix keyboard row connections
    uint8_t macroMappingOptions[128]; // Selects between direct key mapping and mapping to macro sequence for each key (assumes fixed width of 8 columns)
    //    uint8_t keyMappingKeyCode[128];            // USB keyboard key code for each key (assumes fixed width of 8 columns), also down key code in triggered mapping mode
    //    uint8_t keyMappingKeyModifier[128];        // USB keyboard key modifier, also down key modifier in triggered mapping mode (assumes fixed width of 8 columns)
    //    uint8_t keyMappingTriggeredKey[128];       // Selects between normal direct key mapping and triggered key mapping for each key (assumes fixed width of 8 columns)
    //    uint8_t keyMappingKeyCodeUp[128];          // USB keyboard up key code in triggered mapping mode (assumes fixed width of 8 columns)
    //    uint8_t keyMappingKeyModifierUp[128];      // USB keyboard up key modifier in triggered mapping mode (assumes fixed width of 8 columns)
    //    uint8_t matrixKBvalues[128];               // Current state of each matrix keyboard key (assumes fixed width of 8 columns)
    //	bool matrixKBPins[128];               // extracted from matrixKBrowsPins

} PK_MatrixKB_Parameters;

unsigned int sleepdur = 1000;
bool use_sleepdur1 = true;
unsigned int sleepdur1 = 1000;
unsigned int sleepdur2 = 1000;

unsigned int sleepdur_S[20] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 };

// bool DoPWM = false;
bool DoEncoders = true;

PK_MatrixKB_Parameters MatrixKB;
/**
 * @brief   Structure to hold the state of the component
 *
 */
uint8_t Merge_8BitsToByte(bool Bit_array[8]) {
    uint8_t sum = 0;
    for (int i = 0; i < 8; i++) {
        //    sum += Bit_array[i] - '0';
        //    sum<<=1;

        if (Bit_array[i] == true) {
            sum |= 1 << i;
        } else {
            sum &= ~(1 << i);
        }
    }
    return sum;
}

/**
 * @brief  Config_MatrixKB
 *
 */
int Config_MatrixKB() {
    if (PK_MatrixKBConfigurationGet(dev) == PK_OK) {
        MatrixKB.matrixKBconfiguration = dev->matrixKB.matrixKBconfiguration;
        MatrixKB.matrixKBwidth = dev->matrixKB.matrixKBheight;
        MatrixKB.matrixKBheight = dev->matrixKB.matrixKBwidth;

        for (i = 0; i < 8; i++) {
            MatrixKB.matrixKBrowsPins[i] = dev->matrixKB.matrixKBrowsPins[i];
            MatrixKB.matrixKBrowsPins[8 + i] = dev->matrixKB.matrixKBrowsPins[8 + i];

            MatrixKB.matrixKBcolumnsPins[i] = dev->matrixKB.matrixKBcolumnsPins[i];
        }

        for (i = 0; i < 128; i++) {
            MatrixKB.macroMappingOptions[i] = dev->matrixKB.macroMappingOptions[i];
        }
        usleep(sleepdur);
    }

    // PK_MatrixKBConfigurationSet
    return 0;
}
/**
 * @brief  Update_MatrixKB
 *
 */
int Update_MatrixKB() {
    if (PK_MatrixKBStatusGet(dev) == PK_OK) {
        for (i = 0; i < 128; i++) {
            //	MatrixKB.matrixKBvalues[i] = dev->matrixKB.matrixKBvalues[i];
        }
        usleep(sleepdur);
    }
    if (PK_MatrixKBConfigurationGet(dev) == PK_OK) {
        int i = 0;
        int h = 0;
        int w = 0;
        for (h = 0; h < MatrixKB.matrixKBheight; h++) {
            for (w = 0; w < MatrixKB.matrixKBwidth; w++) {
                //	MatrixKB.matrixKBPins[i] = Get_BitOfByte(MatrixKB.matrixKBrowsPins[h], w);
                i++;
            }
        }
        usleep(sleepdur);
    }
    return 0;
}

/**
 * @brief
 *
 */
int Update_LCD() {
    if (PK_LCDUpdate(dev) == PK_OK) {
        usleep(sleepdur);
    }
    if (PK_LCDSetCustomCharacters(dev) == PK_OK) {
        usleep(sleepdur);
    }
    return 0;
}

/**
 * @brief
 *
 */
int Config_PoStep() {
    if (PK_PoStep_ConfigurationGet(dev) == PK_OK) {
        usleep(sleepdur);
    }

    if (PK_PoStep_DriverConfigurationGet(dev) == PK_OK) {
        usleep(sleepdur);
    }

    // PK_PoStep_ConfigurationSet(dev)
    // PK_PoStep_DriverConfigurationSet(dev)
    return 0;
}
/**
 * @brief
 *
 */
int Update_PoStep() {
    if (PK_PoStep_StatusGet(dev) == PK_OK) {
        usleep(sleepdur);
    }
    return 0;
}

static char *serial_number = "";

static int ConnectionType = 0; // 1..USB, 2..UDP, 3..Network, 4..fastUSB

static char *IP = "0.0.0.0";

static int timeout_ms = 2000;

static int retry = 3;

int instance_number = 0;
/**
 * @brief
 *
 */
sPoKeysDevice *TryConnectToDevice(uint32_t intSerial) {
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: serial_number: %d\n", intSerial);
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: serial_number=%d\n", __FILE__, __FUNCTION__, intSerial);
    sPoKeysDevice *retDev = NULL;
    uint32_t i_Timeout = 1000;
    uint32_t enm_usb_dev = 0;
    uint32_t enm_fusb_dev = 0;
    uint32_t enm_udp_dev = 0;
    int32_t nDevs = 0;
    uint32_t lastConectionTypeTried = 0;

    if (timeout_ms > 0) {
        i_Timeout = timeout_ms;
    }
    if (intSerial != 0) {
        retDev = PK_ConnectToDevice(0); // waits for usb device
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: intSerial=%d\n", __FILE__, __FUNCTION__, intSerial);
        if (retDev == NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EnumerateUSBDevices\n", __FILE__, __FUNCTION__);
            enm_usb_dev = PK_EnumerateUSBDevices();
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: enm_usb_dev :%d\n", __FILE__, __FUNCTION__, enm_usb_dev);
            // enm_fusb_dev = PK_EnumerateFastUSBDevices();
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: enm_fusb_dev :%d\n", __FILE__, __FUNCTION__, enm_fusb_dev);
            if (enm_usb_dev != 0 || enm_fusb_dev != 0) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDeviceWSerial(%d, %d)\n", __FILE__, __FUNCTION__, intSerial, i_Timeout);
                retDev = PK_ConnectToDeviceWSerial((uint32_t)intSerial, i_Timeout); // waits for usb device
                lastConectionTypeTried = 1;
                if (retDev == NULL) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys: %s:%s: PK_ConnectToDeviceWSerial(%d, %d) "
                                    "FAILED\n",
                                    __FILE__, __FUNCTION__, intSerial, i_Timeout);
                    retDev = PK_ConnectToDeviceWSerial((uint32_t)intSerial, i_Timeout); // waits for usb device
                    lastConectionTypeTried = 1;
                    if (retDev == NULL) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PK_ConnectToDeviceWSerial(%d, %d) "
                                        "FAILED\n",
                                        __FILE__, __FUNCTION__, intSerial, i_Timeout);

                        retDev = PK_ConnectToDeviceWSerial(intSerial, i_Timeout); // waits for usb device
                        lastConectionTypeTried = 1;
                        if (retDev == NULL) {
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys: %s:%s: PK_ConnectToDeviceWSerial(%d, "
                                            "%d) FAILED\n",
                                            __FILE__, __FUNCTION__, intSerial, i_Timeout);

                            retDev = PK_ConnectToDevice(0); // waits for usb device
                            lastConectionTypeTried = 1;
                            if (retDev == NULL) {
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys: %s:%s: PK_ConnectToDevice(0) "
                                                "FAILED\n",
                                                __FILE__, __FUNCTION__);

                                retDev = PK_ConnectToDevice(intSerial); // waits for usb device
                                lastConectionTypeTried = 1;
                                if (retDev == NULL) {
                                    rtapi_print_msg(RTAPI_MSG_ERR,
                                                    "PoKeys: %s:%s: PK_ConnectToDevice(%d) "
                                                    "FAILED\n",
                                                    __FILE__, __FUNCTION__, intSerial);
                                } else {
                                    rtapi_print_msg(RTAPI_MSG_ERR,
                                                    "PoKeys: %s:%s: PK_ConnectToDevice(%d) "
                                                    "OK\n",
                                                    __FILE__, __FUNCTION__, intSerial);
                                }

                            } else {
                                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice(0) OK\n", __FILE__, __FUNCTION__);
                            }
                        } else {
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys: %s:%s: PK_ConnectToDeviceWSerial(%d, "
                                            "%d) OK\n",
                                            __FILE__, __FUNCTION__, intSerial, i_Timeout);
                        }
                    } else {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PK_ConnectToDeviceWSerial(%d, %d) "
                                        "OK\n",
                                        __FILE__, __FUNCTION__, intSerial, i_Timeout);
                    }

                } else {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDeviceWSerial(%d, %d) OK\n", __FILE__, __FUNCTION__, intSerial, i_Timeout);
                }
            }
        }
        if (retDev == NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDeviceWSerial_UDP(%d, %d)\n", __FILE__, __FUNCTION__, intSerial, i_Timeout);
            retDev = PK_ConnectToDeviceWSerial_UDP(intSerial, i_Timeout); // waits for udp device
            if (retDev == NULL) {
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys: %s:%s: PK_ConnectToDeviceWSerial_UDP(%d, %d) "
                                "FAILED\n",
                                __FILE__, __FUNCTION__, intSerial, i_Timeout);
            } else {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDeviceWSerial_UDP(%d, %d) OK\n", __FILE__, __FUNCTION__, intSerial, i_Timeout);
            }
            lastConectionTypeTried = 2;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: lastConectionTypeTried = 2\n", __FILE__, __FUNCTION__);
        }
        if (retDev == NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_SearchNetworkDevices(net_devices, %d, %d)\n", __FILE__, __FUNCTION__, i_Timeout, intSerial);
            sPoKeysNetworkDeviceSummary *net_devices;
            int32_t nDevs = PK_SearchNetworkDevices(net_devices, i_Timeout, intSerial);
            if (nDevs != 0) {
                enm_udp_dev = nDevs;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_ConnectToNetworkDevice(net_devices)\n", __FILE__, __FUNCTION__);
                retDev = PK_ConnectToNetworkDevice(net_devices);
                if (retDev == NULL) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys: %s:%s: PK_ConnectToNetworkDevice(net_devices) "
                                    "FAILED\n",
                                    __FILE__, __FUNCTION__);
                } else {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys: %s:%s: PK_ConnectToNetworkDevice(net_devices) "
                                    "OK\n",
                                    __FILE__, __FUNCTION__);
                }
                lastConectionTypeTried = 3;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: lastConectionTypeTried = 3\n", __FILE__, __FUNCTION__);
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys: %s:%s: "
                                "PK_EnumerateNetworkDevices(net_devices, %d)\n",
                                __FILE__, __FUNCTION__, i_Timeout);
                //	nDevs = PK_EnumerateNetworkDevices(net_devices, i_Timeout); // does not work - it hangs here
                // deb_out = 1135;
                if (nDevs != 0) {
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: nDevs: %s\n", __FILE__, __FUNCTION__, nDevs);
                    enm_udp_dev = nDevs;
                    rtapi_print_msg(RTAPI_MSG_DBG,
                                    "PoKeys: %s:%s: "
                                    "PK_ConnectToNetworkDevice(net_devices)\n",
                                    __FILE__, __FUNCTION__);
                    retDev = PK_ConnectToNetworkDevice(net_devices);
                    lastConectionTypeTried = 3;
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: lastConectionTypeTried = 3\n", __FILE__, __FUNCTION__);
                }
            }
        }
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: udp_devices[16]\n", __FILE__, __FUNCTION__);
        sPoKeysNetworkDeviceSummary udp_devices[16];

        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EnumerateUSBDevices()\n", __FILE__, __FUNCTION__);
        enm_usb_dev = PK_EnumerateUSBDevices();
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EnumerateUSBDevices()=%d\n", __FILE__, __FUNCTION__, enm_usb_dev);

        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EnumerateFastUSBDevices()\n", __FILE__, __FUNCTION__);
        // enm_fusb_dev = PK_EnumerateFastUSBDevices();
        // rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EnumerateFastUSBDevices()=%d\n", __FILE__, __FUNCTION__, enm_fusb_dev);

        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_EnumerateNetworkDevices(udp_devices, %d)\n", __FILE__, __FUNCTION__, i_Timeout);
        int32_t nDevs = PK_EnumerateNetworkDevices(udp_devices, i_Timeout); // does not work - it hangs here
        //	rtapi_print("");
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: nDevs: %d\n", __FILE__, __FUNCTION__, nDevs);
        // enm_udp_dev = nDevs;

        if (nDevs != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToNetworkDevice(udp_devices) - %d\n", __FILE__, __FUNCTION__, nDevs);
            for (int i = 0; i < 16; i++) {
                if (retDev == NULL) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: udp_devices[%d].SerialNumber: %d\n", __FILE__, __FUNCTION__, i, udp_devices[i].SerialNumber);
                    if (udp_devices[i].SerialNumber != 0) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PK_ConnectToDeviceWSerial_UDP(%d, "
                                        "%d)\n",
                                        __FILE__, __FUNCTION__, udp_devices[i].SerialNumber, i_Timeout);
                        retDev = PK_ConnectToDeviceWSerial_UDP(udp_devices[i].SerialNumber,
                                                               i_Timeout); // waits for udp device
                        lastConectionTypeTried = 2;
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: lastConectionTypeTried = 2\n", __FILE__, __FUNCTION__);
                    }
                } else {
                    break;
                }
            }
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: lastConectionTypeTried = 3\n", __FILE__, __FUNCTION__);
        }
        if (enm_fusb_dev > 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice FastUSB\n", __FILE__, __FUNCTION__);
            for (int i = 0; i < enm_fusb_dev + 1; i++) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice(%d)\n", __FILE__, __FUNCTION__, i);
                retDev = PK_ConnectToDevice(i);
                if (retDev == NULL) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice(%d) FAILED\n", __FILE__, __FUNCTION__, i);
                } else {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice(%d) OK\n", __FILE__, __FUNCTION__, i);
                }
            }
            lastConectionTypeTried = 4;
        }

        if (enm_usb_dev > 0) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice USB\n", __FILE__, __FUNCTION__);
            for (int i = 0; i < enm_usb_dev + 1; i++) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice(%d)\n", __FILE__, __FUNCTION__, i);
                retDev = PK_ConnectToDevice(i);
                if (retDev == NULL) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice(%d) FAILED\n", __FILE__, __FUNCTION__, i);
                } else {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_ConnectToDevice(%d) OK\n", __FILE__, __FUNCTION__, i);
                }
            }
            lastConectionTypeTried = 4;
        }
        // deb_out = 120;
    }
    if (retDev != NULL) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: Connected to device\n", __FILE__, __FUNCTION__);
        // connected = 1;
        if (lastConectionTypeTried == 1) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: USB\n", __FILE__, __FUNCTION__);
        }
        if (lastConectionTypeTried == 2) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: UDP\n", __FILE__, __FUNCTION__);
        }
        if (lastConectionTypeTried == 3) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: Network\n", __FILE__, __FUNCTION__);
        }
        return retDev;
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: failed to connect to device\n", __FILE__, __FUNCTION__);
        return NULL;
    }
}

bool initdone = 0;
int doSetup = 0;

int next_setup = 1;
/**
 * @brief
 *
 */
void pokeys_read_ini(sPoKeysDevice *dev) {
    const char *ini_path = getenv("INI_FILE_NAME");

    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: pokeys_read_ini -start \n", __FILE__, __FUNCTION__);
    set_pokeys_ini_path(ini_path);

    PKIO_ReadIniFile(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKIO_ReadIniFile - done\n", __FILE__, __FUNCTION__);
    PKPEv2_ReadIniFile(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKPEv2_ReadIniFile - done\n", __FILE__, __FUNCTION__);
    PKEncoder_ReadIniFile(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKEncoder_ReadIniFile - done\n", __FILE__, __FUNCTION__);

    PKIO_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKIO_Setup - done\n", __FILE__, __FUNCTION__);
    PKPEv2_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKPEv2_Setup - done\n", __FILE__, __FUNCTION__);
    PKEncoder_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKEncoder_Setup - done\n", __FILE__, __FUNCTION__);
    PKPoExtBus_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKPoExtBus_Setup - done\n", __FILE__, __FUNCTION__);
    PKPoNet_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKPoNet_Setup - done\n", __FILE__, __FUNCTION__);
    //    PKPoStep_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKPoStep_Setup - done\n", __FILE__, __FUNCTION__);
    //    PKLCD_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKLCD_Setup - done\n", __FILE__, __FUNCTION__);
    //    PKMatrixKB_Setup(dev);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PKMatrixKB_Setup - done\n", __FILE__, __FUNCTION__);
}
/**
 * @brief
 *
 */
void pokeys_write_ini(sPoKeysDevice *dev) {

    PKIO_WriteIniFile(dev);
    PKPEv2_WriteIniFile(dev);
    PKEncoder_WriteIniFile(dev);
}

void pokeys_update(sPoKeysDevice *dev) {

    PKPEv2_Update(dev, HAL_Machine_On);

    usleep(sleepdur);

    PKEncoder_Update(dev);

    usleep(sleepdur);

    PKIO_Update(dev);

    usleep(sleepdur);

    // rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: info_PoExtBus = %d\n", __FILE__, __FUNCTION__, info_PoExtBus);
    PKPoExtBus_Update(dev);

    usleep(sleepdur);

    // rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: info_PoNET = %d\n", __FILE__, __FUNCTION__, info_PoNET);
    PKPoNet_Update(dev);

    usleep(sleepdur);
}

void pokeys_setup(sPoKeysDevice *dev) {
    /*  if (doSetup == 1) {
        rtapi_print_msg(RTAPI_MSG_DBG,
                        "PoKeys: %s:%s: info_PulseEnginev2 = %d\n", __FILE__,
                        __FUNCTION__, info_PulseEnginev2);
        if (info_PulseEnginev2 != 0) {
            PKPEv2_Setup(dev);
            deb_out = 313;
        }
        next_setup = 2;
    } else if (doSetup == 2) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PinCount = %d\n",
                        __FILE__, __FUNCTION__, info_PinCount);
        if (info_PinCount != 0) {
            PKIO_Setup(dev);
            deb_out = 312;
        }
        next_setup = 3;
    } else if (doSetup == 3) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PoExtBus = %d\n",
                        __FILE__, __FUNCTION__, info_PoExtBus);
        if (info_PoExtBus != 0) {
            PKPoExtBus_Setup(dev);
            deb_out = 314;
        }
        next_setup = 4;
    } else if (doSetup == 4) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PoNET = %d\n",
                        __FILE__, __FUNCTION__, info_PoNET);
        if (info_PoNET != 0) {
            PKPoNet_Setup(dev);
            deb_out = 315;
        }
        next_setup = 5;
    } else if (doSetup == 5) {
        rtapi_print_msg(RTAPI_MSG_DBG,
                        "PoKeys: %s:%s: info_EncodersCount = %d\n", __FILE__,
                        __FUNCTION__, info_EncodersCount);
        if (info_EncodersCount != 0) {
            PKEncoder_Setup(dev);
            deb_out = 316;
        }
        next_setup = 1;
    }
    doSetup = 0;*/
}

/**
 * @brief user_mainloop
 *
 */
void user_mainloop(void) {

    rtapi_print("  \n");
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: user_mainloop started  \n", __FILE__, __FUNCTION__);
    rtapi_print("  \n");
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: comp_id: %d\n", __FILE__, __FUNCTION__, comp_id);
    rtapi_print("  \n");
    while (0xb) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: while(0xb) \n", __FILE__, __FUNCTION__);

        FOR_ALL_INSTS() {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: FOR_ALL_INSTS() \n", __FILE__, __FUNCTION__);
            rtc_loopcount++;
            HAL_Machine_On = machine_is_on;

            deb_out = 100;

            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: initdone: %s\n", __FILE__, __FUNCTION__, initdone ? "true" : "false");
            while (dev == NULL || initdone != 1) {
                Loop_Frequ = rtc_loop_frequ;
                uint32_t lastConectionTypeTried = 0;

                if (dev == NULL) {
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: TryConnectToDevice %d \n", __FILE__, __FUNCTION__, devSerial);
                    dev = TryConnectToDevice(devSerial);
                    if (dev != NULL) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Connected to device\n", __FILE__, __FUNCTION__);
                    }
                }

                if (dev != NULL) {
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev != NULL\n", __FILE__, __FUNCTION__);
                    if (devSerial == 0) {

                        devSerial = dev->DeviceData.SerialNumber;
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: devSerial: %d\n", __FILE__, __FUNCTION__, devSerial);
                    }
                    switch (dev->connectionType) {
                        case PK_DeviceType_USBDevice:
                            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: USB\n", __FILE__, __FUNCTION__);
                            connected_usb = 1;
                            connected_fusb = 0;
                            connected_udp = 0;
                            connected_net = 0;
                            break;
                        case PK_DeviceType_FastUSBDevice:
                            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: FastUSB\n", __FILE__, __FUNCTION__);
                            connected_usb = 0;
                            connected_fusb = 1;
                            connected_udp = 0;
                            connected_net = 0;
                            break;
                        case PK_DeviceType_NetworkDevice:
                            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: NetworkDevice\n", __FILE__, __FUNCTION__);

                            /*  PK_ConnectionParam_TCP = 0,
                                                        PK_ConnectionParam_UDP = 1*/
                            if (dev->connectionParam == 0) {
                                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: TCP\n", __FILE__, __FUNCTION__);
                                connected_usb = 0;
                                connected_fusb = 0;
                                connected_udp = 0;
                                connected_net = 1;
                            } else {
                                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: UDP\n", __FILE__, __FUNCTION__);
                                connected_usb = 0;
                                connected_fusb = 0;
                                connected_udp = 1;
                                connected_net = 0;
                            }

                            break;
                    }

                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: initialize info pins\n", __FILE__, __FUNCTION__);

                    info_PinCount = dev->info.iPinCount;                                             // Number of pins, physically on the device
                    info_PWMCount = dev->info.iPWMCount;                                             // Number of pins that support PWM output
                    info_BasicEncoderCount = dev->info.iBasicEncoderCount;                           // Number of basic encoders
                    info_EncodersCount = dev->info.iEncodersCount;                                   // Number of encoder slots available
                    info_FastEncoders = dev->info.iFastEncoders;                                     // Number of fast encoders supported
                    info_UltraFastEncoders = dev->info.iUltraFastEncoders;                           // Number of available ultra fast encoders
                    info_PWMinternalFrequency = dev->info.PWMinternalFrequency;                      // Main PWM peripheral clock
                    info_AnalogInputs = dev->info.iAnalogInputs;                                     // Number of info_protI2Cavailable analog inputs
                    info_KeyMapping = dev->info.iKeyMapping;                                         // Device supports key mapping (acts as a USB keyboard)
                    info_TriggeredKeyMapping = dev->info.iTriggeredKeyMapping;                       // Device supports triggered key mapping
                    info_KeyRepeatDelay = dev->info.iKeyRepeatDelay;                                 // Device supports user customizable key repeat rates and delays
                    info_DigitalCounters = dev->info.iDigitalCounters;                               // Device supports digital counters
                    info_JoystickButtonAxisMapping = dev->info.iJoystickButtonAxisMapping;           // Device supports mapping of joystick buttons
                    info_JoystickAnalogToDigitalMapping = dev->info.iJoystickAnalogToDigitalMapping; // Device supports mapping of analog inputs to digital keys
                    info_Macros = dev->info.iMacros;                                                 // Device supports customizable macro sequences
                    info_MatrixKeyboard = dev->info.iMatrixKeyboard;                                 // Device supports matrix keyboard
                    info_MatrixKeyboardTriggeredMapping = dev->info.iMatrixKeyboardTriggeredMapping; // Device supports matrix keyboard triggered key mapping
                    info_LCD = dev->info.iLCD;                                                       // Device supports alphanumeric LCD display
                    info_MatrixLED = dev->info.iMatrixLED;                                           // Device supports matrix LED display
                    info_ConnectionSignal = dev->info.iConnectionSignal;                             // Device supports connection signal output
                    info_PoExtBus = dev->info.iPoExtBus;                                             // Device supports PoExtBus digital outputs
                    info_PoNET = dev->info.iPoNET;                                                   // Device supports PoNET bus devices
                    info_AnalogFiltering = dev->info.iAnalogFiltering;                               // Device supports analog inputs low-pass digital filtering
                    info_InitOutputsStart = dev->info.iInitOutputsStart;                             // Device supports initializing outputs at startup
                    info_protI2C = dev->info.iprotI2C;                                               // Device supports I2C bus (master)
                    info_prot1wire = dev->info.iprot1wire;                                           // Device supports 1-wire bus (master)
                    info_AdditionalOptions = dev->info.iAdditionalOptions;                           // Device supports additional options with activation keys
                    info_LoadStatus = dev->info.iLoadStatus;                                         // Device supports reporting load status
                    info_CustomDeviceName = dev->info.iCustomDeviceName;                             // Device supports specifying custom device names
                    info_PoTLog27support = dev->info.iPoTLog27support;                               // Device supports PoTLog27 firmware
                    info_SensorList = dev->info.iSensorList;                                         // Device supports sensor lists
                    info_WebInterface = dev->info.iWebInterface;                                     // Device supports web interface
                    info_FailSafeSettings = dev->info.iFailSafeSettings;                             // Device supports fail-safe mode
                    info_JoystickHATswitch = dev->info.iJoystickHATswitch;                           // Device supports joystick HAT switch mapping
                    info_PulseEngine = dev->info.iPulseEngine;                                       // Device supports Pulse engine
                    info_PulseEnginev2 = dev->info.iPulseEnginev2;                                   // Device supports Pulse engine v2
                    info_EasySensors = dev->info.iEasySensors;                                       // Device supports EasySensors

                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Device info pins initialized\n", __FILE__, __FUNCTION__);

                    if (info_protI2C != 0) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_protI2C = %d \n", __FILE__, __FUNCTION__, info_protI2C);

                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_I2CBusScanStart\n", __FILE__, __FUNCTION__);
                        if (PK_I2CBusScanStart(dev) == PK_OK) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: PK_I2CBusScanStart "
                                            "- scan started\n",
                                            __FILE__, __FUNCTION__);
                            I2C_isscanning = true;
                            usleep(sleepdur);
                        }
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: I2C_isscanning = %d\n", __FILE__, __FUNCTION__, I2C_isscanning);
                    }

                    if (setPinConfig == true) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: setPinConfig = true\n", __FILE__, __FUNCTION__);
                        if (PK_PinConfigurationSet(dev) == PK_OK) {
                            usleep(sleepdur);
                            setPinConfig = false;
                        }
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: setPinConfig -done\n", __FILE__, __FUNCTION__);
                    }

                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Device info pins initialized\n", __FILE__, __FUNCTION__);
                    initdone = 1;
                }

                usleep(sleepdur);
            }
            alive = 1;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: alive=1\n", __FILE__, __FUNCTION__);
            deb_out = 200;
            // RTC
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_RTCGet(dev)\n", __FILE__, __FUNCTION__);
            if (PK_RTCGet(dev) == PK_OK) {
                deb_out = 210;
                rtc_sec = dev->RTC.SEC;
                rtc_min = dev->RTC.MIN;
                rtc_hour = dev->RTC.HOUR;
                rtc_dow = dev->RTC.DOW;
                rtc_dom = dev->RTC.DOM;
                rtc_tmp = dev->RTC.tmp;
                rtc_doy = dev->RTC.DOY;
                rtc_month = dev->RTC.MONTH;
                rtc_year = dev->RTC.YEAR;

                if (rtc_lastsec != rtc_sec) {
                    deb_out = 211;
                    rtc_loop_frequ = rtc_loopcount;
                    rtc_loopcount = 0;
                    rtc_lastsec = rtc_sec;

                    if (secBlink) {
                        secBlink = false;
                    } else {
                        secBlink = true;
                    }

                    doSetup = next_setup;

                    // sleepdur
                    if (rtc_loop_frequ > 15) {
                        if (rtc_loop_frequ_demand == 0) {
                            sleepdur = sleepdur * rtc_loop_frequ / 25;
                            // rtc_loop_frequ_demand = 25;
                        } else {
                            sleepdur = sleepdur * rtc_loop_frequ / rtc_loop_frequ_demand;
                        }
                    } else {
                        sleepdur = sleepdur / 2;
                    }
                    if (sleepdur > 1000000) {
                        sleepdur = 1000000;
                    }
                    if (sleepdur < 50) {
                        sleepdur = 50;
                    }

                    if (rtc_latencycheck_set == -1) {
                        rtc_latencyCounter = 0;
                        rtc_latencycheck_set = dev->RTC.SEC;
                    }
                }
                // hope to get loopfrequency more stable - as on everyminute additional actions
                if (HAL_Machine_On == false) {
                    if (doSetup > 0) {
                    }
                }
                if (use_sleepdur1 == false) {       // detect if last loop was do setup
                    sleepdur_S[doSetup] = sleepdur; // rememeber sleepdur of Setup loop
                    sleepdur = sleepdur_S[0];       // reset to normal sleepduration
                    use_sleepdur1 = true;
                } else {
                    sleepdur_S[0] = sleepdur; // use normal Sleepduration
                }
                if (rtc_lastsec != rtc_sec && !HAL_Machine_On) { // time for a setup loop
                    use_sleepdur1 = false;
                    sleepdur_S[0] = sleepdur;       // store normal sleepduration
                    sleepdur = sleepdur_S[doSetup]; // apply setup sleepduration
                }

                if (rtc_sec_ret >= rtc_latencycheck_set && rtc_latencycheck_set > 0) {
                    rtc_hal_latency = rtc_latencyCounter;
                    rtc_latencycheck_set = -1;
                } else if (rtc_latencycheck_set >= -0) {
                    rtc_latencyCounter++;
                }
                deb_out = 212;
            }

            // pokeys_update(dev);

            deb_out = 219;
            // PulseEnginev2
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PulseEnginev2 = %d\n", __FILE__, __FUNCTION__, info_PulseEnginev2);
            PKPEv2_Update(dev, HAL_Machine_On);
            deb_out = 220;
            usleep(sleepdur);

            PKEncoder_Update(dev);
            deb_out = 220;
            usleep(sleepdur);

            PKIO_Update(dev);
            deb_out = 230;
            usleep(sleepdur);

            // rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: info_PoExtBus = %d\n", __FILE__, __FUNCTION__, info_PoExtBus);
            PKPoExtBus_Update(dev);
            deb_out = 238;
            usleep(sleepdur);

            // rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: info_PoNET = %d\n", __FILE__, __FUNCTION__, info_PoNET);
            PKPoNet_Update(dev);
            deb_out = 239;
            usleep(sleepdur);

            if (doSetup > 0) {
                // rtc_lastmin = rtc_min;
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: rtc_lastmin = %d\n", __FILE__, __FUNCTION__, rtc_lastmin);

                if (HAL_Machine_On == false) {
                    alive = 0;

                    deb_out = 310;

                    info_PinCount = dev->info.iPinCount;                                             // Number of pins, physically on the device
                    info_PWMCount = dev->info.iPWMCount;                                             // Number of pins that support PWM output
                    info_BasicEncoderCount = dev->info.iBasicEncoderCount;                           // Number of basic encoders
                    info_EncodersCount = dev->info.iEncodersCount;                                   // Number of encoder slots available
                    info_FastEncoders = dev->info.iFastEncoders;                                     // Number of fast encoders supported
                    info_UltraFastEncoders = dev->info.iUltraFastEncoders;                           // Number of available ultra fast encoders
                    info_PWMinternalFrequency = dev->info.PWMinternalFrequency;                      // Main PWM peripheral clock
                    info_AnalogInputs = dev->info.iAnalogInputs;                                     // Number of available analog inputs
                    info_KeyMapping = dev->info.iKeyMapping;                                         // Device supports key mapping (acts as a USB keyboard)
                    info_TriggeredKeyMapping = dev->info.iTriggeredKeyMapping;                       // Device supports triggered key mapping
                    info_KeyRepeatDelay = dev->info.iKeyRepeatDelay;                                 // Device supports user customizable key repeat rates and delays
                    info_DigitalCounters = dev->info.iDigitalCounters;                               // Device supports digital counters
                    info_JoystickButtonAxisMapping = dev->info.iJoystickButtonAxisMapping;           // Device supports mapping of joystick buttons
                    info_JoystickAnalogToDigitalMapping = dev->info.iJoystickAnalogToDigitalMapping; // Device supports mapping of analog inputs to digital keys
                    info_Macros = dev->info.iMacros;                                                 // Device supports customizable macro sequences
                    info_MatrixKeyboard = dev->info.iMatrixKeyboard;                                 // Device supports matrix keyboard
                    info_MatrixKeyboardTriggeredMapping = dev->info.iMatrixKeyboardTriggeredMapping; // Device supports matrix keyboard triggered key mapping
                    info_LCD = dev->info.iLCD;                                                       // Device supports alphanumeric LCD display
                    info_MatrixLED = dev->info.iMatrixLED;                                           // Device supports matrix LED display
                    info_ConnectionSignal = dev->info.iConnectionSignal;                             // Device supports connection signal output
                    info_PoExtBus = dev->info.iPoExtBus;                                             // Device supports PoExtBus digital outputs
                    info_PoNET = dev->info.iPoNET;                                                   // Device supports PoNET bus devices
                    info_AnalogFiltering = dev->info.iAnalogFiltering;                               // Device supports analog inputs low-pass digital filtering
                    info_InitOutputsStart = dev->info.iInitOutputsStart;                             // Device supports initializing outputs at startup
                    info_protI2C = dev->info.iprotI2C;                                               // Device supports I2C bus (master)
                    info_prot1wire = dev->info.iprot1wire;                                           // Device supports 1-wire bus (master)
                    info_AdditionalOptions = dev->info.iAdditionalOptions;                           // Device supports additional options with activation keys
                    info_LoadStatus = dev->info.iLoadStatus;                                         // Device supports reporting load status
                    info_CustomDeviceName = dev->info.iCustomDeviceName;                             // Device supports specifying custom device names
                    info_PoTLog27support = dev->info.iPoTLog27support;                               // Device supports PoTLog27 firmware
                    info_SensorList = dev->info.iSensorList;                                         // Device supports sensor lists
                    info_WebInterface = dev->info.iWebInterface;                                     // Device supports web interface
                    info_FailSafeSettings = dev->info.iFailSafeSettings;                             // Device supports fail-safe mode
                    info_JoystickHATswitch = dev->info.iJoystickHATswitch;                           // Device supports joystick HAT switch mapping
                    info_PulseEngine = dev->info.iPulseEngine;                                       // Device supports Pulse engine
                    info_PulseEnginev2 = dev->info.iPulseEnginev2;                                   // Device supports Pulse engine v2
                    info_EasySensors = dev->info.iEasySensors;                                       // Device supports EasySensors
                    deb_out = 311;

                    //                    pokeys_setup(dev);
                    if (doSetup == 1) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PulseEnginev2 = %d\n", __FILE__, __FUNCTION__, info_PulseEnginev2);
                        if (info_PulseEnginev2 != 0) {
                            PKPEv2_Setup(dev);
                            deb_out = 313;
                        }
                        next_setup = 2;
                    } else if (doSetup == 2) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PinCount = %d\n", __FILE__, __FUNCTION__, info_PinCount);
                        if (info_PinCount != 0) {
                            PKIO_Setup(dev);
                            deb_out = 312;
                        }
                        next_setup = 3;
                    } else if (doSetup == 3) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PoExtBus = %d\n", __FILE__, __FUNCTION__, info_PoExtBus);
                        if (info_PoExtBus != 0) {
                            PKPoExtBus_Setup(dev);
                            deb_out = 314;
                        }
                        next_setup = 4;
                    } else if (doSetup == 4) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PoNET = %d\n", __FILE__, __FUNCTION__, info_PoNET);
                        if (info_PoNET != 0) {
                            PKPoNet_Setup(dev);
                            deb_out = 315;
                        }
                        next_setup = 5;
                    } else if (doSetup == 5) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_EncodersCount = %d\n", __FILE__, __FUNCTION__, info_EncodersCount);
                        if (info_EncodersCount != 0) {
                            PKEncoder_Setup(dev);
                            deb_out = 316;
                        }
                        next_setup = 1;
                    }
                    doSetup = 0;

                } else {
                    alive = 0;
                    usleep(sleepdur * 50);
                }
            }
        }
    }

    exit(0);
};

/**
 * @brief
 *
 */
EXTRA_SETUP() {
    int wait_ms = 5000;
    const char *ini_path = getenv("INI_FILE_NAME");
    FILE *fp = fopen(ini_path, "r");
    if (fp) {
        iniFindInt(fp, "DEVICE_ID", "POKEYS", &device_id);
        iniFindInt(fp, "COMM_TIMEOUT", "POKEYS", &timeout_ms);

        // ApplyIniSettings
        int tmpIniSettings = 0;
        iniFindInt(fp, "ApplyIniSettings", "POKEYS", &tmpIniSettings);

        if (tmpIniSettings != 0) {
            ApplyIniSettings = true;
        }
    }

    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: extra_arg=%s device_id=%i \n", __FILE__, __FUNCTION__, extra_arg, device_id);

    // usleep(wait_ms);  // wait for the HAL to start up
    for (i = 0; i < retry; i++) {
        if (dev == NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: TryConnectToDevice(0)\n", __FILE__, __FUNCTION__);
            dev = TryConnectToDevice(device_id);
        }
        if (dev != NULL) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: Connected\n", __FILE__, __FUNCTION__);
            break;
        }
    }

    if (dev == NULL) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: could not connect to device\n", __FILE__, __FUNCTION__);
    }
    //	PKEncoder_init(comp_id, dev);
    rtapi_print("");
    // devSerial = extra_arg;
    return 0;
}

/**
 * @brief
 *
 */
EXTRA_CLEANUP() {
    if (dev != NULL) {
        PK_DisconnectDevice(dev);
    }
}

/**
 * @brief
 *
 */
static int __comp_get_data_size(void) {
    return 0;
}
#ifdef RTAPI
EXPORT_SYMBOL(pokeys_read_ini);
EXPORT_SYMBOL(pokeys_write_ini);
EXPORT_SYMBOL(pokeys_update);
EXPORT_SYMBOL(pokeys_setup);
#endif