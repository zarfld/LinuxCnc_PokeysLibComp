/*

Copyright (C) 2014 Matevž Bošnak (matevz@poscope.com)

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

int32_t PK_1WireStatusSet(sPoKeysDevice* device, uint8_t activated)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDC, activated, 0, 0, 0);
    return SendRequest(device);
}

int32_t PK_1WireStatusGet(sPoKeysDevice* device, uint8_t* activated)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDC, 0x11, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *activated = device->response[3];
    return PK_OK;
}

int32_t PK_1WireWriteReadStart(sPoKeysDevice* device, uint8_t WriteCount, uint8_t ReadCount, uint8_t * data)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (WriteCount > 16) WriteCount = 16;
    if (ReadCount > 16) ReadCount = 16;

    CreateRequest(device->request, 0xDC, 0x10, WriteCount, ReadCount, 0);
    for (i = 0; i < WriteCount; i++)
    {
        device->request[8+i] = data[i];
    }
    return SendRequest(device);
}

int32_t PK_1WireReadStatusGet(sPoKeysDevice* device, uint8_t * readStatus, uint8_t * ReadCount, uint8_t * data)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDC, 0x11, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *readStatus = device->response[8];
    if (*readStatus == 1)
    {
        *ReadCount = device->response[9];

        if (*ReadCount > 16)
        {
            *readStatus = 0;
            return PK_ERR_PARAMETER;
        }

        for (i = 0; i < *ReadCount; i++)
        {
            data[i] = device->response[10 + i];
        }
    }
    return PK_OK;
}



int32_t PK_1WireWriteReadStartEx(sPoKeysDevice* device, uint8_t pinID, uint8_t WriteCount, uint8_t ReadCount, uint8_t * data)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (WriteCount > 16) WriteCount = 16;
    if (ReadCount > 16) ReadCount = 16;

    CreateRequest(device->request, 0xDC, 0x10, WriteCount, ReadCount, pinID);
    for (i = 0; i < WriteCount; i++)
    {
        device->request[8+i] = data[i];
    }
    return SendRequest(device);
}


int32_t PK_1WireBusScanStart(sPoKeysDevice* device, uint8_t pinID)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDC, 0x20, pinID, 0, 0);
    return SendRequest(device);
}

int32_t PK_1WireBusScanGetResults(sPoKeysDevice* device, uint8_t * operationStatus, uint8_t * scanResult, uint8_t * deviceROM)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDC, 0x21, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *operationStatus = device->response[8];
    *scanResult = device->response[9];
    memcpy(deviceROM, device->response + 10, 8);

    return PK_OK;
}



int32_t PK_1WireBusScanContinue(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDC, 0x22, 0, 0, 0);
    return SendRequest(device);
}


int32_t PK_1WireBusScanStop(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDC, 0x23, 0, 0, 0);
    return SendRequest(device);
}


