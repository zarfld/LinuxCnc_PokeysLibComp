/*

Copyright (C) 2015 Matevž Bošnak (matevz@poscope.com)

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

int32_t PK_EasySensorsSetupGet(sPoKeysDevice* device)
{
    uint32_t i;
    sPoKeysEasySensor * es;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iEasySensors == 0)
    {
        return PK_ERR_NOT_SUPPORTED;
    }

    for (i = 0; i < device->info.iEasySensors; i++)
    {
        es = &device->EasySensors[i];
        // Retrieve i-th EasySensors configuration
        CreateRequest(device->request, 0x76, i, 1, 0, 0);
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        es->sensorValue = 0;

        es->sensorType = device->response[8];
        es->sensorReadingID = device->response[9];
        es->sensorRefreshPeriod = device->response[10];
        es->sensorFailsafeConfig = device->response[11];
        memcpy(es->sensorID, device->response + 12, 8);
    }
    return PK_OK;
}

int32_t PK_EasySensorsSetupSet(sPoKeysDevice* device)
{
    uint32_t i;
    sPoKeysEasySensor * es;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iEasySensors == 0)
    {
        return PK_ERR_NOT_SUPPORTED;
    }

    for (i = 0; i < device->info.iEasySensors; i++)
    {
        es = &device->EasySensors[i];
        // Retrieve i-th EasySensors configuration
        CreateRequest(device->request, 0x76, i, 1, 1, 0);

        device->request[8] = es->sensorType;
        device->request[9] = es->sensorReadingID;
        device->request[10] = es->sensorRefreshPeriod;
        device->request[11] = es->sensorFailsafeConfig;
        memcpy(&device->request[12], es->sensorID, 8);

        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    }
    return PK_OK;
}

int32_t PK_EasySensorsValueGetAll(sPoKeysDevice* device)
{
    uint32_t i, t, readNum;
    sPoKeysEasySensor * es;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iEasySensors == 0)
    {
        return PK_ERR_NOT_SUPPORTED;
    }

    for (i = 0; i < device->info.iEasySensors; i += 13)
    {
        // Determine the number of values to read
        readNum = 13;
        if (i + 13 > device->info.iEasySensors)
            readNum = device->info.iEasySensors - i;

        CreateRequest(device->request, 0x77, i, readNum, 0, 0);
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        for (t = 0; t < readNum; t++)
        {
            es = &device->EasySensors[i + t];

            memcpy(&es->sensorValue, &device->response[8 + t*4], 4);
            // OK statuses are bit-mapped to bytes 4 and 5
            es->sensorOKstatus = (device->response[4 + t/8] >> (t % 8)) & 1;
        }
    }
    return PK_OK;
}

int32_t PK_SL_EasySensorValueGet(sPoKeysDevice* device, uint8_t index)
{
    if (device == NULL) return 0;
    if (index >= device->info.iEasySensors) return 0;
    return device->EasySensors[index].sensorValue;
}
