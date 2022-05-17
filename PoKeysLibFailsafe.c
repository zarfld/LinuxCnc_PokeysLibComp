/*

Copyright (C) 2018 Matevž Bošnak (matevz@poscope.com)

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

#include "PoKeysLib.h"
#include "PoKeysLibCore.h"


int32_t PK_FailsafeSettingsGet(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x81, 0, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Parse the settings
    device->failsafeSettings.bFailSafeEnabled = device->response[3];
    device->failsafeSettings.bFailSafePeripherals = device->response[4];

    memcpy(device->failsafeSettings.bFailSafeIO, device->response + 8, 7);
    memcpy(device->failsafeSettings.bFailSafePoExtBus, device->response + 15, 10);
    memcpy(device->failsafeSettings.bFailSafePWM, device->response + 25, 6);

    return PK_OK;
}


int32_t PK_FailsafeSettingsSet(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x81, 0x01, 0, 0, 0);

    // Insert the settings
    device->request[3] = device->failsafeSettings.bFailSafeEnabled;
    device->request[4] = device->failsafeSettings.bFailSafePeripherals;

    memcpy(device->request + 8, device->failsafeSettings.bFailSafeIO, 7);
    memcpy(device->request + 15, device->failsafeSettings.bFailSafePoExtBus, 10);
    memcpy(device->request + 25, device->failsafeSettings.bFailSafePWM, 6);

    return SendRequest(device);
}
