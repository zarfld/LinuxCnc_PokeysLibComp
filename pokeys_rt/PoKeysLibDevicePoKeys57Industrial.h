/*

   Copyright (C) 2020 Matevž Bošnak (matevz@poscope.com)

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

#ifndef __POKEYSLIBDEVICEPOKEYS57INDUSTRIAL_H
    #define __POKEYSLIBDEVICEPOKEYS57INDUSTRIAL_H

    #include "PoKeysLib.h"

    // C
    #include <stdint.h>

    #ifdef __cplusplus
    // C++
    #include <cstdint>

    extern "C"
    {
    #endif
        // Device-specific structures

        typedef struct
        {
            // Device info section
            uint32_t SerialNumber;                                   // Serial number
            uint8_t  FirmwareVersionMajor;                           // Firmware version - major
            uint8_t  FirmwareVersionMinor;                           // Firmware version - minor
            uint8_t  UserID;                                         // User ID
            uint8_t  HWtype;                                         // HW type, reported by device

            uint8_t  IPaddress[4];                                   // IP address of the device
            uint8_t  reserved[4];

            // I/O related structures
            uint32_t digitalInputs[8];

            uint32_t digitalOutputs[8];
            uint32_t digitalOutputsEnable[8];
            uint32_t digitalOutputsFault[8];

            uint32_t analogInputs[8];

            uint32_t analogOutputs[8];
            uint32_t analogOutputsFault[8];

            // Device structure
            void*    deviceStructure;
#if INTPTR_MAX == INT64_MAX
    // 64-bit
#elif INTPTR_MAX == INT32_MAX
    // 32-bit - pad the structure with 4 bytes
            uint32_t pointerPad;

#endif
        } sPoKeys57Industrial;

        POKEYSDECL sPoKeys57Industrial* PK57i_Connect(void);
        POKEYSDECL void PK57i_Disconnect(sPoKeys57Industrial * device);
        POKEYSDECL int32_t PK57i_Update(sPoKeys57Industrial* device, uint8_t resetFaults);

    #ifdef __cplusplus
    }
    #endif
#endif
