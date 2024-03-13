/*

Copyright (C) 2013 Matevž Bošnak (matevz@poscope.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <stdlib.h>
#include "PoKeysLib.h"
#include "PoKeysLibCore.h"
#include "stdio.h"


#define MAX_PINS_COUNT 100
#define MAX_CAPS_PER_PIN 20

typedef struct
{
    int32_t pinID;
    int32_t pinCapabilities[MAX_CAPS_PER_PIN];
} sPoKeys_PinDescriptor;

typedef struct
{
    int32_t deviceID;
    sPoKeys_PinDescriptor pins[MAX_PINS_COUNT];
} sPoKeys_DeviceDescriptor;



/*
PK_PinCap_pinRestricted  = 0,           // Pin is not used
    PK_PinCap_reserved       = 1,           // --
    PK_PinCap_digitalInput   = 2,           // Digital input
    PK_PinCap_digitalOutput  = 4,           // Digital output
    PK_PinCap_analogInput    = 8,           // Analog input (only on selected pins)
    PK_PinCap_analogOutput   = 16,          // Analog output (only on selected pins)
    PK_PinCap_triggeredInput = 32,          // Triggered input
    PK_PinCap_digitalCounter = 64,          // Digital counter (only on selected pins)
    PK_PinCap_invertPin      = 128          // Invert digital pin polarity (set together with digital input, output or triggered input)*/

#define PIN_DI PK_AllPinCap_digitalInput, PK_AllPinCap_keyboardMapping, PK_AllPinCap_triggeredInput
#define PIN_DO PK_AllPinCap_digitalOutput
#define PIN_DIO PK_AllPinCap_digitalInput, PK_AllPinCap_digitalOutput, PK_AllPinCap_keyboardMapping, PK_AllPinCap_triggeredInput

#define CAP(...) { __VA_ARGS__, -1 }

const sPoKeys_DeviceDescriptor deviceDesc[] = {

    // PoKeys57CNC Pins descriptor
    { PK_DeviceID_PoKeys57CNC,  {
		  { 1, CAP( PIN_DIO, PK_AllPinCap_digitalCounter, PK_AllPinCap_fastEncoder1A ) },
		  { 2, CAP( PIN_DIO, PK_AllPinCap_digitalCounter, PK_AllPinCap_fastEncoder1B ) },
          { 3, CAP( PIN_DI,  PK_AllPinCap_digitalCounter, PK_AllPinCap_fastEncoder2A ) },
          { 4, CAP( PIN_DI,  PK_AllPinCap_digitalCounter, PK_AllPinCap_fastEncoder2B ) },
          { 5, CAP( PIN_DI,  PK_AllPinCap_digitalCounter, PK_AllPinCap_fastEncoder3A ) },
          { 6, CAP( PIN_DI,  PK_AllPinCap_digitalCounter, PK_AllPinCap_fastEncoder3B ) },

          // Ultra-fast encoder pins
		  { 8,  CAP( PIN_DIO ) },
		  { 12, CAP( PIN_DIO ) },
		  { 13, CAP( PIN_DIO ) },

          // Pendant connector pins
		  { 9,  CAP( PIN_DI, PK_AllPinCap_digitalCounter ) },
          { 10, CAP( PIN_DI, PK_AllPinCap_digitalCounter ) },
          { 11, CAP( PIN_DI, PK_AllPinCap_digitalCounter ) },
          { 15, CAP( PIN_DI, PK_AllPinCap_digitalCounter ) },
          { 16, CAP( PIN_DI, PK_AllPinCap_digitalCounter ) },

          // Spindle error input
		  { 14, CAP( PIN_DI ) },

          // 0-10V output
		  { 17, CAP( PIN_DO, PK_AllPinCap_PWMOut ) },

          // LCD contrast
          { 18, CAP( PIN_DIO, PK_AllPinCap_PWMOut ) },

          // Probe input
		  { 19, CAP( PIN_DI, PK_AllPinCap_digitalCounter ) },

          // PWM out, pin 20
		  { 20, CAP( PIN_DIO, PK_AllPinCap_PWMOut) },

          // Pendant LED, PWM out, pin 21
		  { 21, CAP( PIN_DO, PK_AllPinCap_PWMOut ) },

          // LCD backlight
		  { 22, CAP( PIN_DO, PK_AllPinCap_PWMOut ) },

          // LCD pins
		  { 23, CAP( PIN_DIO ) },
          { 24, CAP( PIN_DIO ) },
          { 25, CAP( PIN_DIO ) },
          { 26, CAP( PIN_DIO ) },
          { 28, CAP( PIN_DIO ) },
          { 29, CAP( PIN_DIO ) },
          { 30, CAP( PIN_DIO ) },

          // PoExtension2 connector pins
          { 31, CAP( PIN_DIO ) },
          { 32, CAP( PIN_DIO ) },
		  { 33, CAP( PIN_DIO, PK_AllPinCap_digitalCounter ) },
          { 34, CAP( PIN_DIO, PK_AllPinCap_digitalCounter ) },
          { 35, CAP( PIN_DIO, PK_AllPinCap_digitalCounter ) },
          { 36, CAP( PIN_DIO, PK_AllPinCap_digitalCounter ) },
          { 37, CAP( PIN_DIO, PK_AllPinCap_digitalCounter ) },
          { 38, CAP( PIN_DIO, PK_AllPinCap_digitalCounter ) },

          // Analog inputs
          { 41, CAP( PIN_DIO, PK_AllPinCap_digitalCounter, PK_AllPinCap_analogInput ) },
          { 42, CAP( PIN_DIO, PK_AllPinCap_digitalCounter, PK_AllPinCap_analogInput ) },
          { 43, CAP( PIN_DIO, PK_AllPinCap_digitalCounter, PK_AllPinCap_analogInput ) },
          { 44, CAP( PIN_DIO, PK_AllPinCap_digitalCounter, PK_AllPinCap_analogInput ) },
          { 45, CAP( PIN_DIO, PK_AllPinCap_analogInput ) },

          // Emergency switch input
          { 52, CAP( PIN_DI ) },

          // End of pin list
		  { -1, CAP( -1) }
      }},

    // PoKeys57CNCpro4x25 Pins descriptor
    { PK_DeviceID_PoKeys57CNCpro4x25,  {

          // Ultra-fast encoder pins
          { 8,  CAP( PIN_DIO ) },
          { 12, CAP( PIN_DIO ) },
          { 13, CAP( PIN_DIO ) },

          // Spindle error
          { 14, CAP( PIN_DI ) },

          // 0-10V output
          { 17, CAP( PIN_DO, PK_AllPinCap_PWMOut ) },

          // MOT - fault 2, 1, 4, 3
          { 18, CAP( PIN_DI ) },
          { 28, CAP( PIN_DI ) },
          { 29, CAP( PIN_DI ) },
          { 30, CAP( PIN_DI ) },

          // Probe
          { 19, CAP( PIN_DI ) },

          // PWM out, pin 20
          { 20, CAP( PIN_DO, PK_AllPinCap_PWMOut) },

          // Axis limit 4, 3, 2, 1
          { 23, CAP( PIN_DI ) },
          { 24, CAP( PIN_DI ) },
          { 25, CAP( PIN_DI ) },
          { 26, CAP( PIN_DI ) },

          // PoExtension2 connector pins
          { 31, CAP( PIN_DIO ) },
          { 32, CAP( PIN_DIO ) },
          { 33, CAP( PIN_DIO ) },
          { 34, CAP( PIN_DIO ) },
          { 35, CAP( PIN_DIO ) },
          { 36, CAP( PIN_DIO ) },
          { 37, CAP( PIN_DIO ) },
          { 38, CAP( PIN_DIO ) },

          // Analog inputs
          { 41, CAP( PIN_DI, PK_AllPinCap_analogInput ) }, // AnalogInput_0_10V
          { 42, CAP( PIN_DI, PK_AllPinCap_analogInput ) }, // PlasmaIn_0_10V
          { 43, CAP( PIN_DI, PK_AllPinCap_analogInput ) }, // AIN_24V
          { 44, CAP( PIN_DI, PK_AllPinCap_analogInput ) }, // NTC_Temperature

          // Emergency switch input
          { 52, CAP( PIN_DI ) },

          // End of pin list
          { -1, CAP( -1) }
      }},

      // PoKeys57CNCdb25 Pins descriptor
	  { PK_DeviceID_PoKeys57CNCdb25,  {
		  { 1, CAP( PIN_DO ) },
		  { 2, CAP( PIN_DO ) },
          { 3, CAP( PIN_DO ) },
          { 4, CAP( PIN_DO ) },
          { 5, CAP( PIN_DO ) },
          { 6, CAP( PIN_DO ) },
          { 7, CAP( PIN_DO ) },
          { 8, CAP( PIN_DO ) },
          { 9, CAP( PIN_DO ) },

		  { 10, CAP( PIN_DI ) },
		  { 11, CAP( PIN_DI ) },
		  { 12, CAP( PIN_DI ) },
		  { 13, CAP( PIN_DI ) },

          { 14, CAP( PIN_DO ) },

		  { 15, CAP( PIN_DI ) },

          { 16, CAP( PIN_DO ) },
          { 17, CAP( PIN_DO ) },

          // End of pin list
		  { -1, CAP( -1) }
      }},


	  // FabricCoder pins descriptor
	  { PK_DeviceID_OEM1, {
		  { 1, CAP(PIN_DO) },
		  { 10, CAP(PIN_DI) },
		  { 11, CAP(PIN_DI) },
		  { 12, CAP(PIN_DI) },
		  { 13, CAP(PIN_DI) },
		  { 14, CAP(PIN_DO) },
		  { 16, CAP(PIN_DO) },
		  { 17, CAP(PIN_DO) },

		  // End of pin list
		  { -1, CAP(-1) }
	  } },

    // End of device list
    { -1, 0 }
};



// This structure is using real pin numbers
const sPoKeys_PinCapabilities pinCaps[] = {
    { PK_AllPinCap_digitalInput,        1, 55, 0, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 | PK_DeviceMask_27 },
    { PK_AllPinCap_digitalOutput,       1, 55, 0, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 | PK_DeviceMask_27 },
    { PK_AllPinCap_analogInput,        43, 47, 0, PK_DeviceMask_55 },
    { PK_AllPinCap_analogInput,        41, 47, 0, PK_DeviceMask_56 | PK_DeviceMask_57 | PK_DeviceMask_27 },
    { PK_AllPinCap_analogOutput,       43, 43, 0, PK_DeviceMask_55 },
    { PK_AllPinCap_keyboardMapping,     1, 55, 0, PK_DeviceMask_55 | PK_DeviceMask_57U | PK_DeviceMask_56U },
    { PK_AllPinCap_triggeredInput,      1, 55, 0, PK_DeviceMask_55 | PK_DeviceMask_57U | PK_DeviceMask_56U },
    { PK_AllPinCap_digitalCounter,      1, 55, 1, PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_PWMOut,             17, 22, 0, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_fastEncoder1A,       1,  1, 0, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_fastEncoder1B,       2,  2, 0, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_fastEncoder1I,       9,  9, 1, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },

    { PK_AllPinCap_fastEncoder2A,       3,  3, 1, PK_DeviceMask_55 },
    { PK_AllPinCap_fastEncoder2B,       4,  4, 1, PK_DeviceMask_55 },
    { PK_AllPinCap_fastEncoder2A,       5,  5, 1, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_fastEncoder2B,       6,  6, 1, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_fastEncoder2I,      11, 11, 1, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },

    { PK_AllPinCap_fastEncoder3A,      15, 15, 0, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_fastEncoder3B,      16, 16, 0, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },
    { PK_AllPinCap_fastEncoder3I,      27, 27, 1, PK_DeviceMask_55 | PK_DeviceMask_56 | PK_DeviceMask_57 },


    { PK_AllPinCap_digitalInput,        9, 19, 0, PK_DeviceMask_PoPLC58 },
    { PK_AllPinCap_digitalOutput,      20, 27, 0, PK_DeviceMask_PoPLC58 },
    { PK_AllPinCap_analogInput,         1,  8, 0, PK_DeviceMask_PoPLC58 },
    { PK_AllPinCap_MFanalogInput,       1,  8, 0, PK_DeviceMask_PoPLC58 },
    { -1, 0, 0, 0 }
};



int32_t CompareName(int8_t *device, int8_t *search)
{
    int32_t len = (int32_t)strlen(search);
    int32_t i;

    for (i = 0; i < len; i++)
    {
        if (device[i] != search[i]) return i+1;
    }
    return 0;
}

int32_t PK_DeviceDataGet(sPoKeysDevice* device)
{
    int32_t i;
    sPoKeysDevice_Data * data;
    sPoKeysDevice_Info * info;

    uint8_t devSeries55 = 0;
    uint8_t devSeries56 = 0;
    uint8_t devSeries57 = 0;
    uint8_t devSeries27 = 0;
    uint8_t devSeries58 = 0;
    uint8_t devSeries16 = 0;
    uint8_t devExtendedID = 0;
    uint8_t devUSB = 0;
    uint8_t devEth = 0;
    uint8_t devBootloader = 0;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    data = &device->DeviceData;
    info = &device->info;

	memset(info, 0, sizeof(sPoKeysDevice_Info));

	memset(device->request, 0, 64);
	memset(device->response, 0, 64);

    data->DeviceType = 0;

	// Read serial and firmware version
	CreateRequest(device->request, 0x00, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
        if (CompareName(device->response+8, "PK58") == 0 || CompareName(device->response+8, "PKEx") == 0)
        {
            // Device with extended ID
            data->FirmwareVersionMajor = device->response[4];
            data->FirmwareVersionMinor = device->response[5];

            data->SerialNumber = (int)device->response[12] + ((int)device->response[13] << 8)
                    + ((int)device->response[14] << 16) + ((int)device->response[15] << 24);
            data->HWtype = device->response[18];
            data->ProductID = device->response[57];

            // Read device name
            memset(data->DeviceName, 0, 11);
            memcpy(data->DeviceName, device->response + 31, 10);

            // Read build date
            memset(data->BuildDate, 0, 12);
            memcpy(data->BuildDate, device->response + 20, 11);

            data->FWtype = device->response[41];
            data->SecondaryFirmwareVersionMajor = device->response[42];
            data->SecondaryFirmwareVersionMinor = device->response[43];
            data->deviceIsBootloader = device->response[61];

            devExtendedID = 1;
        } else if (CompareName(device->response+8, "RF16") == 0)
        {
            // Series 16 device
            data->FirmwareVersionMajor = device->response[4];
            data->FirmwareVersionMinor = device->response[5];

            data->SerialNumber = (int)device->response[12] + ((int)device->response[13] << 8)
                    + ((int)device->response[14] << 16) + ((int)device->response[15] << 24);
            data->HWtype = device->response[18];
            data->ProductID = device->response[57];

            // Read device name
            memset(data->DeviceName, 0, 11);
            memcpy(data->DeviceName, device->response + 31, 10);

            // Read build date
            memset(data->BuildDate, 0, 12);
            memcpy(data->BuildDate, device->response + 20, 11);

            data->FWtype = 0;
            data->SecondaryFirmwareVersionMajor = data->FirmwareVersionMajor;
            data->SecondaryFirmwareVersionMinor = data->FirmwareVersionMinor;

            devExtendedID = 1;
        } else
        {
            data->FirmwareVersionMajor = device->response[4];
            data->FirmwareVersionMinor = device->response[5];

            data->SerialNumber = ((long)device->response[2]*256 + (long)device->response[3]);
            data->DeviceName[0] = 0;
            data->BuildDate[0] = 0;

            data->FWtype = 0;
            data->SecondaryFirmwareVersionMajor = data->FirmwareVersionMajor;
            data->SecondaryFirmwareVersionMinor = data->FirmwareVersionMinor;
        }
    } else return PK_ERR_TRANSFER;

	// Read User ID
    CreateRequest(device->request, 0x03, 0, 0, 0, 0);
	if (SendRequest(device) == PK_OK)
    {
        data->UserID = device->response[2];
        data->DeviceLockStatus = device->response[3];
	} else return PK_ERR_TRANSFER;

    if (devExtendedID == 0)
    {
        data->deviceIsBootloader = 0;

        // Resolve device type
        if (data->SerialNumber == 0xFFFF)
        {
            data->DeviceType = 3; // old bootloader - recovery mode
            devSeries55 = 1;
            devBootloader = 1;

            data->DeviceTypeID = PK_DeviceMask_Bootloader | PK_DeviceMask_Bootloader55;
            data->deviceIsBootloader = 1;

        // PoKeys56 devices have serial numbers above 20000
        } else if (data->SerialNumber >= 20000 && data->SerialNumber < 65536)
        {
            // PoKeys56 bootloaders have bit 7 set in the major firmware version
            if ((data->FirmwareVersionMajor & (1 << 7)) > 0)
            {
                if (device->connectionType == PK_DeviceType_NetworkDevice)
                {
                    data->DeviceType = 16; // PoKeys56E bootloader
                    devBootloader = 1;
                    devSeries56 = 1;

                    data->DeviceTypeID = PK_DeviceMask_Bootloader | PK_DeviceMask_Bootloader56 | PK_DeviceMask_Bootloader56E;
                    data->deviceIsBootloader = 1;
                }
                else
                {
                    data->DeviceType = 15; // PoKeys56U bootloader
                    devUSB = 1;
                    devBootloader = 1;
                    devSeries56 = 1;

                    data->DeviceTypeID = PK_DeviceMask_Bootloader | PK_DeviceMask_Bootloader56 | PK_DeviceMask_Bootloader56U;
                    data->deviceIsBootloader = 1;
                }
            }
            else
            {
                // PoTLog27
                if (data->FirmwareVersionMajor == 64)
                {
                    if (device->connectionType == PK_DeviceType_NetworkDevice)
                    {
                        data->DeviceType = 21; // PoTLog27E
                        devSeries27 = 1;
                        devEth = 1;

                        data->DeviceTypeID = PK_DeviceMask_27 | PK_DeviceMask_27E;
                    }
                    else
                    {
                        devUSB = 1;
                        devSeries27 = 1;
                        data->DeviceType = 20; // PoTLog27U
                        data->DeviceTypeID = PK_DeviceMask_27 | PK_DeviceMask_27U;
                    }
                }
                else
                {
                    if (device->connectionType == PK_DeviceType_NetworkDevice)
                    {
                        data->DeviceType = 11; // PoKeys56E
                        devSeries56 = 1;
                        devEth = 1;
                        data->DeviceTypeID = PK_DeviceMask_56 | PK_DeviceMask_56E;
                    }
                    else
                    {
                        devUSB = 1;
                        data->DeviceType = 10; // PoKeys56U
                        devSeries56 = 1;
                        data->DeviceTypeID = PK_DeviceMask_56 | PK_DeviceMask_56U;
                    }
                }
            }
        }
        // PoKeys55 v3
        else if (data->SerialNumber >= 11500)
        {
            devUSB = 1;
            devSeries55 = 1;
            data->DeviceType = 2;
            data->DeviceTypeID = PK_DeviceMask_55 | PK_DeviceMask_55v3;
        }
        // PoKeys55 v2
        else if (data->SerialNumber >= 10113)
        {
            devUSB = 1;
            devSeries55 = 1;
            data->DeviceType = 1;
            data->DeviceTypeID = PK_DeviceMask_55 | PK_DeviceMask_55v2;
        }
        // PoKeys55 v1
        else
        {
            devUSB = 1;
            devSeries55 = 1;
            data->DeviceType = 0;
            data->DeviceTypeID = PK_DeviceMask_55 | PK_DeviceMask_55v1;
        }

    // New devices report type already
    } else
    {
        // PoKeys58 series devices report their type by themselves
        data->DeviceType = data->HWtype;

        switch (data->DeviceType)
        {
            // PoKeys58EU
            case 40:
                devSeries58 = 1;

                devUSB = 1;
                devEth = 1;

                data->DeviceTypeID = PK_DeviceMask_58;
                break;
            // PoBootload series 58
            case 41:
                devSeries58 = 1;

                devUSB = 1;
                devEth = 1;

                data->DeviceTypeID = PK_DeviceMask_Bootloader58;
                devBootloader = 1;
                break;
            // PoPLC58
            case 50:
                devSeries58 = 1;

                devUSB = 1;
                devEth = 1;

                data->DeviceTypeID = PK_DeviceMask_PoPLC58;
                break;

            // PoKeys16RF
            case 60:
                devSeries16 = 1;
                devUSB = 1;
                data->DeviceTypeID = PK_DeviceMask_PoKeys16RF;
                break;

            // PoKeys57U
            case PK_DeviceID_57U_v0:
            case PK_DeviceID_57U:
                devSeries57 = 1;
                devUSB = 1;
                data->DeviceTypeID = PK_DeviceMask_57 | PK_DeviceMask_57U;
                break;

            // PoKeys57E
            case PK_DeviceID_57E_v0:
            case PK_DeviceID_57E:
                devSeries57 = 1;
                devEth = 1;
                data->DeviceTypeID = PK_DeviceMask_57 | PK_DeviceMask_57E;
                break;

            // PoKeys57CNC
            case PK_DeviceID_PoKeys57CNC:
                devSeries57 = 1;
                devEth = 1;
                devUSB = 1;
                data->DeviceTypeID = PK_DeviceMask_57 | PK_DeviceMask_57CNC;
                break;

            // PoKeys57CNCpro4x25
            case PK_DeviceID_PoKeys57CNCpro4x25:
                devSeries57 = 1;
                devEth = 1;
                devUSB = 1;
                data->DeviceTypeID = PK_DeviceMask_57 | PK_DeviceMask_57CNCpro4x25;
                break;

			// PoKeys57CNCdb25
            case PK_DeviceID_PoKeys57CNCdb25:
                devSeries57 = 1;
                devUSB = 1;
                data->DeviceTypeID = PK_DeviceMask_57 | PK_DeviceMask_57CNCdb25;
                break;

            // PoKeys57Utest
            case PK_DeviceID_PoKeys57Utest:
                devSeries57 = 1;
                devUSB = 1;
                data->DeviceTypeID = PK_DeviceMask_57 | PK_DeviceMask_57Utest;
                break;


            case PK_DeviceID_OEM1:
                devSeries57 = 1;
                devEth = 1;
                data->DeviceTypeID = PK_DeviceMask_57;
                break;

            case PK_DeviceID_SerialReader:
                devSeries57 = 1;
                devUSB = 1;
                data->DeviceTypeID = PK_DeviceMask_57;
                break;

            case PK_DeviceID_X15_02_24:
                devSeries57 = 1;
                devEth = 1;
                data->DeviceTypeID = PK_DeviceMask_57;
                break;

        }
    }


	// Resolve the type names
	switch (data->DeviceType)
    {
        case 0:
            sprintf(data->DeviceTypeName, "generic PoKeys device");
			break;
        case 1:
            sprintf(data->DeviceTypeName, "PoKeys55");
			break;
        case 2:
            sprintf(data->DeviceTypeName, "PoKeys55");
			break;
        case 3:
            sprintf(data->DeviceTypeName, "PoKeys55 - recovery");
			break;
        case PK_DeviceID_56U:
            sprintf(data->DeviceTypeName, "PoKeys56U");
			break;
        case PK_DeviceID_56E:
            sprintf(data->DeviceTypeName, "PoKeys56E");
			break;
        case PK_DeviceID_Bootloader56U:
            sprintf(data->DeviceTypeName, "PoKeys56U - recovery");
			break;
        case PK_DeviceID_Bootloader56E:
            sprintf(data->DeviceTypeName, "PoKeys56E - recovery");
			break;
        case PK_DeviceID_27U:
            sprintf(data->DeviceTypeName, "PoTLog27U");
			break;
        case PK_DeviceID_27E:
            sprintf(data->DeviceTypeName, "PoTLog27E");
			break;            
        case PK_DeviceID_57U:
        case PK_DeviceID_57U_v0:
            sprintf(data->DeviceTypeName, "PoKeys57U");
            break;
        case PK_DeviceID_57E_v0:
        case PK_DeviceID_57E:
            sprintf(data->DeviceTypeName, "PoKeys57E");
            break;
        case PK_DeviceID_PoKeys57CNC:
            sprintf(data->DeviceTypeName, "PoKeys57CNC");
            break;
        case PK_DeviceID_PoKeys57CNCpro4x25:
            sprintf(data->DeviceTypeName, "PoKeys57CNCpro4x25");
            break;
        case PK_DeviceID_PoKeys57CNCdb25:
            sprintf(data->DeviceTypeName, "PoKeys57CNCdb25");
            break;
        case PK_DeviceID_PoKeys57Utest:
            sprintf(data->DeviceTypeName, "PoKeys57Utest");
            break;
        case 40:
            sprintf(data->DeviceTypeName, "PoKeys58EU");
            break;
        case 41:
            sprintf(data->DeviceTypeName, "PoBootload");
            break;
        case 50:
            sprintf(data->DeviceTypeName, "PoPLC58");
            break;
        case 60:
            sprintf(data->DeviceTypeName, "PoKeys16RF");
            break;
        case PK_DeviceID_OEM1:
			sprintf(data->DeviceTypeName, "MartelliMotion");
            break;
        case PK_DeviceID_SerialReader:
            sprintf(data->DeviceTypeName, "SerialReader");
            break;
        case PK_DeviceID_X15_02_24:
            sprintf(data->DeviceTypeName, "X15-02-24");
            break;

        default:
            sprintf(data->DeviceTypeName, "PoKeys");
			break;
    }

	switch (data->DeviceType)
    {
		// PoKeys55
        case 0:
		case 1:
		case 2:
			info->iPinCount = 55;
			info->iEncodersCount = 25;
			info->iPWMCount = 6;
			info->iBasicEncoderCount = 25;

			device->info.PWMinternalFrequency = 12000000;
			break;

		// PoKeys55 bootloader
		case 3:
			info->iPinCount = 0;
			info->iEncodersCount = 0;
			info->iPWMCount = 0;
			info->iBasicEncoderCount = 0;
			break;
        
		// PoKeys56U, PoKeys56E
		case PK_DeviceID_56U:
		case PK_DeviceID_56E:
			info->iPinCount = 55;
			info->iEncodersCount = 26;
			info->iBasicEncoderCount = 25;
			info->iPWMCount = 6;

			device->info.PWMinternalFrequency = 25000000;
			break;

		// PoKeys56U, PoKeys56E bootloader
		case PK_DeviceID_Bootloader56U:
        case PK_DeviceID_Bootloader56E:
			info->iPinCount = 0;
			info->iEncodersCount = 0;
			info->iBasicEncoderCount = 0;
			info->iPWMCount = 0;
			break;
		
		// PoTLog27U, PoTLog27E
		case 20:
		case 21:
			info->iPinCount = 55;
			info->iBasicEncoderCount = 0;
			info->iEncodersCount = 0;
			info->iPWMCount = 0;
			break;

        // PoKeys57U, PoKeys57E
		case PK_DeviceID_57E_v0:
        case PK_DeviceID_57U_v0:
        case PK_DeviceID_57U:
        case PK_DeviceID_57E:
            info->iPinCount = 55;
            info->iEncodersCount = 26;
            info->iBasicEncoderCount = 25;
            info->iPWMCount = 6;

            device->info.PWMinternalFrequency = 25000000;
            break;

        // PoKeys57Utest
        case PK_DeviceID_PoKeys57Utest:
            info->iPinCount = 55;
            info->iEncodersCount = 0;
            info->iBasicEncoderCount = 0;
            info->iPWMCount = 0;
            break;

        // PoKeys57CNC
        case PK_DeviceID_PoKeys57CNC:
            info->iPinCount = 55;
            info->iEncodersCount = 26;
            info->iBasicEncoderCount = 25;
            info->iPWMCount = 6;

            device->info.PWMinternalFrequency = 25000000;
            break;

        // PoKeys57CNCpro4x25
        case PK_DeviceID_PoKeys57CNCpro4x25:
            info->iPinCount = 55;
            info->iEncodersCount = 26;
            info->iBasicEncoderCount = 25;
            info->iPWMCount = 6;

            device->info.PWMinternalFrequency = 25000000;
            break;

		// PoKeys57CNCdb25
        case PK_DeviceID_PoKeys57CNCdb25:
            info->iPinCount = 17;
            info->iEncodersCount = 0;
            info->iBasicEncoderCount = 0;
            info->iPWMCount = 6;

            device->info.PWMinternalFrequency = 25000000;
            break;

        // PoKeys58EU
        case 40:
            break;

        // PoBootload
        case 41:
            info->iPinCount = 0;
            info->iEncodersCount = 0;
            info->iBasicEncoderCount = 0;
            info->iPWMCount = 0;
            break;

        // PoPLC58
        case 50:
            info->iPinCount = 27;
            info->iEncodersCount = 0;
            info->iPWMCount = 0;
            info->iBasicEncoderCount = 0;
            break;

        // PoKeys16RF
        case 60:
            info->iPinCount = 16;
            info->iEncodersCount = 0;
            info->iPWMCount = 0;
            info->iBasicEncoderCount = 0;
            break;

        case PK_DeviceID_OEM1:
            info->iPinCount = 55;
            info->iEncodersCount = 5;
            info->iBasicEncoderCount = 5;
            info->iPWMCount = 0;
            break;
        case PK_DeviceID_SerialReader:
            info->iPinCount = 55;
            info->iEncodersCount = 5;
            info->iBasicEncoderCount = 5;
            info->iPWMCount = 3;
            device->info.PWMinternalFrequency = 25000000;
            break;

        case PK_DeviceID_X15_02_24:
            info->iPinCount = 55;
            info->iEncodersCount = 26;
            info->iBasicEncoderCount = 25;
            info->iPWMCount = 6;
            device->info.PWMinternalFrequency = 25000000;
            break;

		default:
			info->iPinCount = 0;
			info->iBasicEncoderCount = 0;
			info->iEncodersCount = 0;
			info->iPWMCount = 0;

			break;
    }

    if (devEth)
    {
        device->netDeviceData = (sPoKeysNetworkDeviceInfo *)malloc(sizeof(sPoKeysNetworkDeviceInfo));

        // Get network device information...
        CreateRequest(device->request, 0xE0, 0, 0, 0, 0);
        if (SendRequest(device) == PK_OK)
        {
            device->netDeviceData->DHCP = device->response[8];

            memcpy(device->netDeviceData->IPAddressSetup, device->response + 9, 4);
            memcpy(device->netDeviceData->IPAddressCurrent, device->response + 13, 4);

            device->netDeviceData->TCPtimeout = (unsigned short)device->response[17] + ((unsigned short)device->response[18] << 8);

            memcpy(device->netDeviceData->DefaultGateway, device->response + 19, 4);
            memcpy(device->netDeviceData->Subnetmask, device->response + 23, 4);

            if ((device->response[27] & 0xA0) == 0xA0)
            {
                device->netDeviceData->AdditionalNetworkOptions = device->response[27];
            }
            else
            {
                device->netDeviceData->AdditionalNetworkOptions = 0;
            }
        } else return PK_ERR_TRANSFER;
    } else
    {
        device->netDeviceData = 0;
    }

    // Read device name
    CreateRequest(device->request, 0x06, 0, 0, 0, 0);
    if (SendRequest(device) == PK_OK)
    {
		// Check if long name is supported...
		for (i = 0; i < 10; i++)
		{
			if (device->response[8 + i] != device->response[35 + i]) break;
		}

		if (i == 10)
		{
			// Ok, names match - use the long name!
			memcpy(data->DeviceName, device->response + 35, 20);
		} else
		{
			memcpy(data->DeviceName, device->response + 8, 10);		
		}
    } else return PK_ERR_TRANSFER;

	// If the device name is empty, rewrite the name with the type name
    if (strlen(data->DeviceName) == 0)
	{
        strcpy(data->DeviceName, data->DeviceTypeName);
	}

    if (!devExtendedID)
    {
        // Read firmware build date
        CreateRequest(device->request, 0x04, 0, 0, 0, 0);
        if (SendRequest(device) == PK_OK)
        {
            for (i = 0; i < 4; i++)	data->BuildDate[i + 0] = device->response[2 + i];
        } else return PK_ERR_TRANSFER;

        CreateRequest(device->request, 0x04, 1, 0, 0, 0);
        if (SendRequest(device) == PK_OK)
        {
            for (i = 0; i < 4; i++)	data->BuildDate[i + 4] = device->response[2 + i];
        } else return PK_ERR_TRANSFER;

        CreateRequest(device->request, 0x04, 2, 0, 0, 0);
        if (SendRequest(device) == PK_OK)
        {
            for (i = 0; i < 3; i++)	data->BuildDate[i + 8] = device->response[2 + i];
            data->BuildDate[11] = 0;
        } else return PK_ERR_TRANSFER;
    }


	// Check device capabilities
	if (!devBootloader)
	{
        if (devSeries27 || devSeries55 || devSeries56 || devSeries57)
        {
            if (devUSB)						info->iKeyMapping = 1;
            if (devUSB && !devSeries27)		info->iTriggeredKeyMapping = 1;
            if (devUSB && !devSeries27)		info->iKeyRepeatDelay = 1;
            if (devSeries56 || devSeries57)	info->iDigitalCounters = 1;
            if (devUSB)						info->iJoystickButtonAxisMapping = 1;
            if (devUSB)						info->iJoystickAnalogToDigitalMapping = 1;
            if (devSeries55 || devSeries56 || devSeries57)	info->iFastEncoders = 3;
            if (devSeries56 || devSeries57)	info->iUltraFastEncoders = 1;
            if (devUSB)						info->iMacros = 1;
                                            info->iMatrixKeyboard = 1;
            if (devUSB)						info->iMatrixKeyboardTriggeredMapping = 1;
            if (devSeries56 || devSeries57)	info->iPoNET = 1;
            if (!devSeries27)				info->iLCD = 1;
            if (!devSeries27)				info->iMatrixLED = 2;
            if (!devSeries27)				info->iConnectionSignal = 1;
            if (!devSeries27)				info->iPoExtBus = 10;
            if (!devSeries55)				info->iInitOutputsStart = 1;
            if (devSeries56 || devSeries57 || devSeries27) info->iprotI2C = 1;
            if (devSeries56 || devSeries57)				info->iprot1wire = 1;
            if (devSeries57 || (devSeries56 && !(data->FirmwareVersionMajor == 32 && data->FirmwareVersionMinor < 13))) info->iAdditionalOptions = 1;
            if (!devSeries27)				info->iCustomDeviceName = 1;
            if (devSeries56 || devSeries27) info->iPoTLog27support = 1;
            if (devSeries56 || devSeries27) info->iSensorList = 1;
            if (devEth)						info->iWebInterface = 1;
            if (devSeries56 || devSeries57)				info->iFailSafeSettings = 1;
            if ((devSeries56 || devSeries57) && devUSB)		info->iJoystickHATswitch = 1;
        }
        if (1)                              info->iAnalogInputs = 1;
        if (1)                              info->iAnalogFiltering = 1;
        if (!devSeries27 && !devSeries16)	info->iLoadStatus = 1;
	}

	// Special handling
	if (data->DeviceType == PK_DeviceID_PoKeys57CNCdb25)
	{
		info->iDigitalCounters = 0;
		info->iFastEncoders = 0;
		info->iUltraFastEncoders = 0;
		info->iAnalogInputs = 0;
		info->iAnalogFiltering = 0;
		info->iSensorList = 0;
		info->iLCD = 0;
		info->iMatrixLED = 0;
		info->iprot1wire = 0;
	}

    if (data->DeviceType == PK_DeviceID_PoKeys57Utest)
    {
        info->iDigitalCounters = 0;
        info->iFastEncoders = 0;
        info->iUltraFastEncoders = 0;
        info->iAnalogInputs = 1;
        info->iAnalogFiltering = 1;
        info->iSensorList = 1;
        info->iLCD = 0;
        info->iMatrixLED = 0;
        info->iprot1wire = 0;
    }

    if (data->DeviceType == PK_DeviceID_X15_02_24)
    {
		info->iDigitalCounters = 0;
		info->iFastEncoders = 0;
		info->iUltraFastEncoders = 0;
		info->iAnalogInputs = 0;
		info->iAnalogFiltering = 0;
        info->iSensorList = 0;
        info->iLCD = 0;
        info->iMatrixLED = 0;
        info->iprot1wire = 0;
		info->iMatrixKeyboard = 0;
		info->iPulseEngine = 0;
		info->iPulseEnginev2 = 0;
    }

    if (devSeries27 || devSeries55 || devSeries56 || devSeries57 || devSeries58)
    {
        // Read activated options
        CreateRequest(device->request, 0x8F, 0, 0, 0, 0);
        if (SendRequest(device) == PK_OK)
        {
            data->ActivatedOptions = device->response[8];
        } else return PK_ERR_TRANSFER;
    }

    if (devSeries56)
    {
        // Firmware < 3.0.59
        if (data->FirmwareVersionMajor == 32 && data->FirmwareVersionMinor < 59)
        {
            if (data->ActivatedOptions & 1) info->iPulseEngine = 1;
        } else if (data->FirmwareVersionMajor == 32 && data->FirmwareVersionMinor >= 59)
        {
            // On 3.0.59 and on, Pulse engine is automatically activated
            info->iPulseEngine = 1;
        } else if (data->FirmwareVersionMajor == 33)
        {
            // On 3.1.0 and on, Pulse engine v2 is automatically activated
            info->iPulseEnginev2 = 1;
        }
    } else if (devSeries57)
    {
        info->iPulseEnginev2 = 1;
        info->iEasySensors = 100;
    }

	return PK_OK;
}

int32_t PK_FillPWMPinNumbers(sPoKeysDevice * device)
{
	uint8_t PWM_PK_pins[] = { 22, 21, 20, 19, 18, 17 };
	uint8_t PWM_db25_pins[] = { 17, 1, 14, 2, 3, 16 };

	if (device->info.iPWMCount == 0) return PK_ERR_NOT_SUPPORTED;

	if (device->DeviceData.DeviceType == PK_DeviceID_PoKeys57CNCdb25)
	{
		memcpy(device->PWM.PWMpinIDs, &PWM_db25_pins, 6);
	} else
	{
		memcpy(device->PWM.PWMpinIDs, &PWM_PK_pins, 6);
	}
	return PK_OK;
}

int32_t PK_DeviceNameSet(sPoKeysDevice* device)
{
    uint32_t i;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x06, 0x01, 0, 0, 0);

    for (i = 0; i < 10; i++)
    {
        device->request[8+i] = device->DeviceData.DeviceName[i];
    }

    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    return PK_OK;
}

int32_t PK_NetworkConfigurationSet(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xE0, 10, 0, 0, 0);

    device->request[8] = device->netDeviceData->DHCP;

    memcpy(device->request + 9, device->netDeviceData->IPAddressSetup, 4);
    memcpy(device->request + 13, device->netDeviceData->IPAddressCurrent, 4);

    device->request[17] = device->netDeviceData->TCPtimeout & 0xFF;
    device->request[18] = (device->netDeviceData->TCPtimeout >> 8) & 0xFF;

    memcpy(device->request + 19, device->netDeviceData->DefaultGateway, 4);
    memcpy(device->request + 23, device->netDeviceData->Subnetmask, 4);

    device->request[27] = 1; // This request supports subnet and gateway
    device->request[28] = 0xA0 | (device->netDeviceData->AdditionalNetworkOptions & 0x0F);

    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    return PK_OK;
}


int32_t PK_DeviceActivation(sPoKeysDevice* device)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Send new activation code
    CreateRequest(device->request, 0x8F, 0x01, 0, 0, 0);
	memcpy(&(device->request[8]), device->DeviceData.ActivationCode, 8);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	device->DeviceData.ActivatedOptions = device->response[8];

	return PK_OK;
}

int32_t PK_DeviceActivationClear(sPoKeysDevice* device)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Send new activation code
    CreateRequest(device->request, 0x8F, 0xFF, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	device->DeviceData.ActivatedOptions = device->response[8];

	return PK_OK;
}

int32_t PK_SaveConfiguration(sPoKeysDevice* device)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x50, 0xAA, 0x55, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_ClearConfiguration(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x52, 0xAA, 0x55, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    return PK_OK;
}

int32_t PK_CheckPinCapabilityByDevice(uint64_t deviceTypeMask, uint32_t pin, ePK_AllPinCap cap)
{
    const sPoKeys_PinCapabilities * ptr;

    ptr = &pinCaps[0];

    while (ptr->cap != -1)
    {
        if ((ptr->devTypes & deviceTypeMask) && (ptr->cap == cap))
        {
            if (pin + 1 >= ptr->pinStart &&
                pin + 1 <= ptr->pinEnd)
            {
                if (ptr->additionalCheck == 0) return 1;
                switch (ptr->cap)
                {
                    case PK_AllPinCap_digitalCounter:
                        return PK_IsCounterAvailableByDevice((uint32_t)deviceTypeMask, pin);

                    case PK_AllPinCap_fastEncoder1I:
                    case PK_AllPinCap_fastEncoder2I:
                    case PK_AllPinCap_fastEncoder3I:
                        if (deviceTypeMask & PK_DeviceMask_56) return 1; else return 0;

                    case PK_AllPinCap_fastEncoder2A:
                        if (deviceTypeMask & PK_DeviceMask_56) return 1; else return 0;
                        break;
                    case PK_AllPinCap_fastEncoder2B:
                        if (deviceTypeMask & PK_DeviceMask_56) return 1; else return 0;
                        break;
                }
            }
        }
        ptr++;
    }


    return 0;
}

int32_t PK_CheckPinCapabilityByTypeID(uint64_t deviceID, uint32_t pin, ePK_AllPinCap cap)
{
    const sPoKeys_PinCapabilities * ptr;
    uint32_t deviceTypeMask = 0;
    uint32_t iDev, iPin, iCap;

    switch (deviceID)
    {
        // New approach
		case PK_DeviceID_PoKeys57CNCdb25:
        case PK_DeviceID_PoKeys57CNC:
        case PK_DeviceID_PoKeys57CNCpro4x25:
		case PK_DeviceID_OEM1:
            // Find the device in the device list
            //if (cap == PK_AllPinCap_digitalCounter) return PK_IsCounterAvailableByDevice(deviceTypeMask, pin);

            iDev = 0;
            // Look for device...
            while(deviceDesc[iDev].deviceID >= 0)
            {
                if (deviceDesc[iDev].deviceID == deviceID)
                {
                    iPin = 0;

                    // Look for the specific pin
                    while (deviceDesc[iDev].pins[iPin].pinID >= 0)
                    {
                        if (deviceDesc[iDev].pins[iPin].pinID == pin + 1)
                        {
							iCap = 0;

							while (deviceDesc[iDev].pins[iPin].pinCapabilities[iCap] >= 0)
							{
								// Return 1 if pin has requested capability
								if (deviceDesc[iDev].pins[iPin].pinCapabilities[iCap] == cap) return 1;
								iCap++;
							}
                        }
                        iPin++;
                    }

					return 0;
                }
                iDev++;
            }
            return 0;


        // Previous approach...
        case PK_DeviceID_27E:
            deviceTypeMask = PK_DeviceMask_27 | PK_DeviceID_27E;
            break;
        case PK_DeviceID_27U:
            deviceTypeMask = PK_DeviceMask_27 | PK_DeviceID_27U;
            break;
        case PK_DeviceID_55v1:
        case PK_DeviceID_55v2:
        case PK_DeviceID_55v3:
            deviceTypeMask = PK_DeviceMask_55;
            break;
        case PK_DeviceID_56E:
            deviceTypeMask = PK_DeviceMask_56 | PK_DeviceMask_56E;
            break;
        case PK_DeviceID_56U:
            deviceTypeMask = PK_DeviceMask_56 | PK_DeviceMask_56U;
            break;
        case PK_DeviceID_57E:
        case PK_DeviceID_57E_v0:
            deviceTypeMask = PK_DeviceMask_57 | PK_DeviceMask_57E;
            break;
        case PK_DeviceID_57U:
        case PK_DeviceID_57U_v0:
            deviceTypeMask = PK_DeviceMask_57 | PK_DeviceMask_57U;
            break;
        case PK_DeviceID_58EU:
            deviceTypeMask = PK_DeviceMask_58;
            break;
        case PK_DeviceID_PoPLC58:
            deviceTypeMask = PK_DeviceMask_PoPLC58;
            break;
    }

    ptr = &pinCaps[0];

    while (ptr->cap != -1)
    {
        if ((ptr->devTypes & deviceTypeMask) && (ptr->cap == cap))
        {
            if (pin + 1 >= ptr->pinStart &&
                pin + 1 <= ptr->pinEnd)
            {
                if (ptr->additionalCheck == 0) return 1;
                switch (ptr->cap)
                {
                    case PK_AllPinCap_digitalCounter:
                        return PK_IsCounterAvailableByDevice(deviceTypeMask, pin);

                    case PK_AllPinCap_fastEncoder1I:
                    case PK_AllPinCap_fastEncoder2I:
                    case PK_AllPinCap_fastEncoder3I:
                        if (deviceTypeMask & PK_DeviceMask_56) return 1; else return 0;

                    case PK_AllPinCap_fastEncoder2A:
                        if (deviceTypeMask & PK_DeviceMask_56) return 1; else return 0;
                        break;
                    case PK_AllPinCap_fastEncoder2B:
                        if (deviceTypeMask & PK_DeviceMask_56) return 1; else return 0;
                        break;
                }
            }
        }
        ptr++;
    }


    return 0;
}



int32_t PK_CheckPinCapability(sPoKeysDevice* device, unsigned int pin, ePK_AllPinCap cap)
{
    //const sPoKeys_PinCapabilities * ptr;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

	return PK_CheckPinCapabilityByTypeID(device->DeviceData.DeviceType, pin, cap);

	/*
    ptr = &pinCaps[0];

    while (ptr->cap != -1)
    {
        if ((device->DeviceData.DeviceTypeID & ptr->devTypes) && (ptr->cap == cap))
        {
            if (pin + 1 >= ptr->pinStart &&
                pin + 1 <= ptr->pinEnd)
            {
                if (ptr->additionalCheck == 0) return 1;
                switch (ptr->cap)
                {
                    case PK_AllPinCap_digitalCounter:
                        return PK_IsCounterAvailable(device, pin);

                    case PK_AllPinCap_fastEncoder1I:
                    case PK_AllPinCap_fastEncoder2I:
                    case PK_AllPinCap_fastEncoder3I:
                        if (device->FastEncodersOptions == 1) return 1;
                        break;

                    case PK_AllPinCap_fastEncoder2A:
                        if (device->FastEncodersConfiguration == 1 && pin == 2) return 1;
                        else if (device->FastEncodersConfiguration == 10 && pin == 4) return 1;
                        break;
                    case PK_AllPinCap_fastEncoder2B:
                        if (device->FastEncodersConfiguration == 1 && pin == 3) return 1;
                        else if (device->FastEncodersConfiguration == 10 && pin == 5) return 1;
                        break;
                }
            }
        }

        ptr++;
    }


    return 0;
	*/
}


int32_t PK_CheckPinEnabledCapability(sPoKeysDevice* device, uint32_t pin, ePK_AllPinCap cap)
{
    return 0;
}

int32_t PK_GetDebugValues(sPoKeysDevice * device, int32_t * buffer)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xBB, 0, 1, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	memcpy(buffer + 0, &device->response[8], 13*4);

	CreateRequest(device->request, 0xBB, 1, 1, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	memcpy(buffer + 13, &device->response[8], 13*4);

    CreateRequest(device->request, 0xBB, 2, 1, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	memcpy(buffer + 26, &device->response[8], 13*4);

    CreateRequest(device->request, 0xBB, 3, 1, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	memcpy(buffer + 39, &device->response[8], 13*4);

	return PK_OK;

}

int32_t PK_SetFastUSBEnableStatus(sPoKeysDevice * device, uint32_t newState)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x07, newState, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_GetFastUSBEnableStatus(sPoKeysDevice * device, uint32_t * state)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

	CreateRequest(device->request, 0x07, 10, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	*state = device->response[2];
	return PK_OK;
}

int32_t PK_ReadDeviceLog(sPoKeysDevice * device, uint16_t * logBuffer, int32_t * logEntries)
{
    int32_t i;

	if (device == NULL) return PK_ERR_NOT_CONNECTED;

	CreateRequest(device->request, 0x84, 0, 0, 0, 0);
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	*logEntries = device->response[2];
	if (*logEntries > 27) *logEntries = 27;

    for (i = 0; i < *logEntries; i++)
	{
		logBuffer[i] = *(uint16_t*)(device->response + 8 + i * 2);
	}
	return PK_OK;
}
