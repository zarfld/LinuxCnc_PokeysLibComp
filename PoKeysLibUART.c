/*

Copyright (C) 2016 Matev Bošnak (matevz@poscope.com)

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

int32_t PK_UARTConfigure(sPoKeysDevice* device, uint32_t baudrate, uint8_t format, uint8_t interfaceID)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Configure UART
    CreateRequest(device->request, 0xDE, 0x10, interfaceID, format & 0x7F, 0);
    *(uint32_t*)(device->request + 8) = baudrate;
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    return PK_OK;
}

int32_t PK_UARTWrite(sPoKeysDevice* device, uint8_t interfaceID, uint8_t *dataPtr, uint32_t dataWriteLen)
{
    uint32_t totalLen = dataWriteLen;
    uint32_t len = totalLen;
    uint32_t writePtr = 0;
    uint32_t transferred = 0;

    uint32_t retryCount = 0;
    uint32_t maxRetryCount = (1+dataWriteLen / 55) * 100;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    while (len > 0)
    {
        if (len > 55) len = 55;

        CreateRequest(device->request, 0xDE, 0x20, interfaceID, len, 0);

        memcpy(device->request + 8, dataPtr + writePtr, len);
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        transferred = device->response[3];
        if (transferred <= len)
        {
            writePtr += len;
            len = totalLen - writePtr;
        } else
        {
            return PK_ERR_GENERIC;
        }

        if (++retryCount > maxRetryCount)
        {
            return PK_ERR_GENERIC;
        }
    }
    return PK_OK;
}

int32_t PK_UARTRead(sPoKeysDevice* device, uint8_t interfaceID, uint8_t *dataPtr, uint8_t *dataReadLen)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0xDE, 0x30, interfaceID, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *dataReadLen = device->response[3];
    memcpy(dataPtr, device->response + 8, *dataReadLen);
    return PK_OK;
}
