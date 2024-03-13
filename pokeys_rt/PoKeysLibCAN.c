/*

Copyright (C) 2016 Matevž Bošnak (matevz@poscope.com)

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

int32_t PK_CANConfigure(sPoKeysDevice* device, uint32_t bitrate)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Configure UART
    CreateRequest(device->request, 0x86, 0x01, 0, 0, 0);
    *(uint32_t*)(device->request + 8) = bitrate;
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    return PK_OK;
}

int32_t PK_CANRegisterFilter(sPoKeysDevice* device, uint8_t format, uint32_t CANid)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Configure UART
    CreateRequest(device->request, 0x86, 0x10, format, 0, 0);
    *(uint32_t*)(device->request + 8) = CANid;
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    return PK_OK;
}

int32_t PK_CANWrite(sPoKeysDevice* device, sPoKeysCANmsg * msg)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Configure UART
    CreateRequest(device->request, 0x86, 0x20, 0, 0, 0);
    memcpy(device->request + 8, msg, sizeof(sPoKeysCANmsg));
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    return PK_OK;
}

int32_t PK_CANRead(sPoKeysDevice* device, sPoKeysCANmsg * msg, uint8_t * status)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Configure UART
    CreateRequest(device->request, 0x86, 0x31, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    *status = device->response[3];
    if (*status)
        memcpy(msg, device->response + 8, sizeof(sPoKeysCANmsg));
    return PK_OK;
}


int32_t PK_CANFlush(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Configure UART
    CreateRequest(device->request, 0x86, 0x32, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    return PK_OK;
}
