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

#include "PoKeysLib.h"
#include "PoKeysLibCore.h"


int32_t PK_I2CSetStatus(sPoKeysDevice* device, uint8_t activated)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDB, activated, 0, 0, 0);
    return SendRequest(device);
}

int32_t PK_I2CGetStatus(sPoKeysDevice* device, uint8_t* activated) // Retrieves I2C bus activation status
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDB, 0x02, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *activated = device->response[3];
    return PK_OK;
}

int32_t PK_I2CWriteStart(sPoKeysDevice* device, uint8_t address, uint8_t* buffer, uint8_t iDataLength)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (iDataLength > 32) iDataLength = 32;

    CreateRequest(device->request, 0xDB, 0x10, address, iDataLength, 0);
    for (i = 0; i < iDataLength; i++)
    {
        device->request[8+i] = buffer[i];
    }
    return SendRequest(device);
}

int32_t PK_I2CWriteAndReadStart(sPoKeysDevice* device, uint8_t address, uint8_t* buffer, uint8_t iDataLengthWrite, uint8_t iDataLengthRead)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (iDataLengthWrite > 32) iDataLengthWrite = 32;

    CreateRequest(device->request, 0xDB, 0x10, address, iDataLengthWrite, iDataLengthRead);
    for (i = 0; i < iDataLengthWrite; i++)
    {
        device->request[8+i] = buffer[i];
    }
    return SendRequest(device);
}

int32_t PK_I2CWriteStatusGet(sPoKeysDevice* device, uint8_t* status)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDB, 0x11, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *status = device->response[3];
    return PK_OK;
}

int32_t PK_I2CReadStart(sPoKeysDevice* device, uint8_t address, uint8_t iDataLength)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (iDataLength > 32) iDataLength = 32;

    CreateRequest(device->request, 0xDB, 0x20, address, iDataLength, 0);
    return SendRequest(device);
}

int32_t PK_I2CReadStatusGet(sPoKeysDevice* device, uint8_t* status, uint8_t* iReadBytes, uint8_t* buffer, uint8_t iMaxBufferLength)
{
    uint32_t i;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDB, 0x21, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *status = device->response[3];
    *iReadBytes = 0;

    if (*status == PK_I2C_STAT_COMPLETE)
    {
        *iReadBytes = device->response[9];
        if (*iReadBytes > 32)
        {
            *status = PK_I2C_STAT_ERR;
            return PK_ERR_GENERIC;
        }

        for (i = 0; i < iMaxBufferLength && i < *iReadBytes; i++)
        {
            buffer[i] = device->response[10+i];
        }
    }

    return PK_OK;
}

int32_t PK_I2CBusScanStart(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDB, 0x30, 0, 0, 0);
    return SendRequest(device);
}

int32_t PK_I2CBusScanGetResults(sPoKeysDevice* device, uint8_t* status, uint8_t* presentDevices, uint8_t iMaxDevices)
{
    uint32_t i;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (iMaxDevices > 128) iMaxDevices = 128;

    CreateRequest(device->request, 0xDB, 0x31, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *status = device->response[3];

    if (*status == PK_I2C_STAT_COMPLETE)
    {
        for (i = 0; i < iMaxDevices; i++)
        {
            presentDevices[i] = ((device->response[9 + i / 8] & (1 << (i % 8))) > 0) ? PK_I2C_STAT_OK : PK_I2C_STAT_ERR;
        }
    }

    return PK_OK;
}
