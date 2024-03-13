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

void PK_ParsePoILStateResponse(sPoKeysDevice* device)
{
    if (device == NULL) return;

    device->PoIL.info.CodeMemorySize = device->response[8] + ((uint32_t)device->response[9] << 8);
    device->PoIL.info.DataMemorySize = device->response[10] + ((uint32_t)device->response[11] << 8);

    device->PoIL.info.Version = device->response[12];

    device->PoIL.CoreState = device->response[13] + ((uint32_t)device->response[14] << 8);
    device->PoIL.CoreDebugMode = device->response[15];
    device->PoIL.CoreDebugBreakpoint = device->response[17] + ((uint32_t)device->response[18] << 8);

    device->PoIL.PC = device->response[19] + ((uint32_t)device->response[20] << 8);
    device->PoIL.STATUS = device->response[21];

    device->PoIL.W = device->response[22] + ((uint32_t)device->response[23] << 8) +
                  ((uint32_t)device->response[24] << 16) + ((uint32_t)device->response[25] << 24);
    device->PoIL.ExceptionPC = device->response[26] + ((uint32_t)device->response[27] << 8);

    device->PoIL.functionStack.stackPtr = device->response[28] + ((uint32_t)device->response[29] << 8);
    device->PoIL.dataStack.stackPtr = device->response[30] + ((uint32_t)device->response[31] << 8);

    memcpy(&device->PoIL.ExceptionLocation, device->response + 32, 4);
    memcpy(device->PoIL.ExceptionDescription, device->response + 36, 16);

    device->PoIL.MasterEnable = device->response[59];
    device->PoIL.currentTask = device->response[16];
    device->PoIL.taskCount = device->response[60];
}

int32_t PK_PoILGetState(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x00, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    device->PoIL.functionStack.stackSize = 64;
    device->PoIL.dataStack.stackSize = 32;

    return PK_OK;
}

int32_t PK_PoILSetCoreState(sPoKeysDevice* device, uint16_t state)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x01, state & 0xFF, state >> 8, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int32_t PK_PoILSetMasterEnable(sPoKeysDevice* device, uint8_t masterEnable)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x03, masterEnable, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int32_t PK_PoILResetCore(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x02, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int32_t PK_PoILSetDebugMode(sPoKeysDevice* device, uint8_t debugMode, uint16_t breakpoint)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x05, debugMode, breakpoint & 0xFF, breakpoint >> 8);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    PK_ParsePoILStateResponse(device);

    return PK_OK;
}

int32_t PK_PoILReadMemory(sPoKeysDevice* device, uint8_t memoryType, uint16_t address, uint16_t size, uint8_t * dest)
{
    uint32_t i;
    uint16_t address2 = 0;
    uint16_t readLen = 0;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Read in chunks of 54 bytes
    for (i = 0; i < size; i += 54)
    {
        if (i + 54 < size)
        {
            readLen = 54;
        }
        else readLen = size - i;

        address2 = address + i;

        CreateRequest(device->request, 0x82, 0x10, memoryType, (uint8_t)address2, (uint8_t)(address2 >> 8));
        device->request[8] = (uint8_t)readLen;
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        memcpy(dest + i, device->response + 8, readLen);
    }
    return PK_OK;
}


int32_t PK_PoILWriteMemory(sPoKeysDevice* device, uint8_t memoryType, uint16_t address, uint16_t size, uint8_t * src)
{
    uint32_t i;
    uint16_t address2 = 0;
    uint16_t writeLen = 0;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (memoryType == 0)
    {
        for (i = 0; i < size; i += 256)
        {
            uint8_t tmp[256];
            writeLen = size - i;
            if (writeLen > 256) writeLen = 256;

            memcpy(tmp, src + i, writeLen);
            PK_PoILWriteMemory(device, 1, 0, 256, tmp);

            // Write data to flash
            CreateRequest(device->request, 0x82, 0x15, 0, 0, (uint8_t)(i >> 8));
            if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
        }
    } else
        {

        // Write in chunks of 54 bytes
        for (i = 0; i < size; i += 54)
        {
            if (i + 54 < size)
            {
                writeLen = 54;
            }
            else writeLen = size - i;

            address2 = address + i;

            CreateRequest(device->request, 0x82, 0x15, memoryType, (uint8_t)address2, (uint8_t)(address2 >> 8));
            device->request[8] = (uint8_t)writeLen;
            memcpy(device->request + 9, src + i, writeLen);

            if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
        }
    }
    return PK_OK;
}

int32_t PK_PoILReadSharedSlot(sPoKeysDevice* device, uint16_t firstSlotID, uint16_t slotsNum, int * dest)
{
    uint32_t i;
    uint16_t address2 = 0;
    uint8_t requestedSlots;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Read in chunks of 54 bytes
    for (i = 0; i < slotsNum; i += 13)
    {
        if (slotsNum - i > 13)
            requestedSlots = 13;
        else
            requestedSlots = slotsNum - i;

        address2 = 4*(firstSlotID + i);

        // Unlike other memories, address is the ID of the shared slot
        CreateRequest(device->request, 0x82, 0x10, 4, (uint8_t)address2, (uint8_t)(address2 >> 8));
        device->request[8] = requestedSlots * 4;
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        memcpy((dest + i), device->response + 8, requestedSlots * 4);
    }
    return PK_OK;
}


int32_t PK_PoILWriteSharedSlot(sPoKeysDevice* device, uint16_t firstSlotID, uint16_t slotsNum, int32_t * src)
{
    uint32_t i;
    uint16_t address2 = 0;
    uint8_t requestedSlots;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Write in chunks of 54 bytes
    for (i = 0; i < slotsNum; i += 13)
    {
        if (slotsNum - i > 13)
            requestedSlots = 13;
        else
            requestedSlots = slotsNum - i;

        address2 = 4*(firstSlotID + i);

        // Unlike other memories, address is the ID of the shared slot
        CreateRequest(device->request, 0x82, 0x15, 4, (uint8_t)address2, (uint8_t)(address2 >> 8));
        device->request[8] = (uint8_t)requestedSlots * 4;

        memcpy(device->request + 9, (src + i), requestedSlots * 4);

        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    }

    return PK_OK;
}

int32_t PK_PoILEraseMemory(sPoKeysDevice* device, uint8_t memoryType)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x16, memoryType, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    return PK_OK;
}

int32_t PK_PoILChunkReadMemory(sPoKeysDevice * device, uint8_t * dest)
{
    uint32_t i;
    uint16_t address2 = 0;
    uint16_t readLen = 0;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x11, 1, 0, 0);
    for (i = 0; i < 18; i++)
    {
        device->request[8 + i*3] = device->PoIL.monitorChunks[i].address & 0xFF;
        device->request[8 + i*3 + 1] = (device->PoIL.monitorChunks[i].address >> 8) & 0xFF;
        device->request[8 + i*3 + 2] = device->PoIL.monitorChunks[i].chunkLength;
        if (readLen + device->PoIL.monitorChunks[i].chunkLength > 55) break;

        readLen += device->PoIL.monitorChunks[i].chunkLength;
    }
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    memcpy(dest, device->response + 8, readLen);
    return PK_OK;
}


int32_t PK_PoILChunkReadMemoryInternalAddress(sPoKeysDevice * device, uint8_t * dest)
{
    uint32_t i;
    uint16_t readLen = 0;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x82, 0x11, 5, 0, 0);
    for (i = 0; i < 18; i++)
    {
        device->request[8 + i*3] = device->PoIL.monitorChunks[i].address & 0xFF;
        device->request[8 + i*3 + 1] = (device->PoIL.monitorChunks[i].address >> 8) & 0xFF;
        device->request[8 + i*3 + 2] = device->PoIL.monitorChunks[i].chunkLength;
        if (readLen + device->PoIL.monitorChunks[i].chunkLength > 55) break;

        readLen += device->PoIL.monitorChunks[i].chunkLength;
    }
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    memcpy(dest, device->response + 8, readLen);
    return PK_OK;
}

int32_t PK_PoILTaskStatus(sPoKeysDevice * device)
{
    uint32_t i = 0, k = 0;
    uint8_t taskCountLeft;
    uint8_t * dataPtr;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    taskCountLeft = device->PoIL.taskCount;

    while(taskCountLeft > 0)
    {
        // Transfer task info in chunks of up to 7 tasks at a time
        if (taskCountLeft > 7)
        {
            CreateRequest(device->request, 0x82, 0x20, i, 7, 0);
            taskCountLeft -= 7;
        }
        else
        {
            CreateRequest(device->request, 0x82, 0x20, i, taskCountLeft, 0);
            taskCountLeft = 0;
        }
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

        // Copy task info
        device->PoIL.inactiveLoad = device->response[3];

        for (k = 0; k < device->request[4]; k++)
        {
            dataPtr = &device->response[8+k*8];

            device->PoIL.tasks[i + k].taskStatus = *dataPtr++;
            device->PoIL.tasks[i + k].taskLoad = *dataPtr++;
            device->PoIL.tasks[i + k].taskPeriod = *(uint16_t*)(dataPtr); dataPtr+=2;
            device->PoIL.tasks[i + k].taskRealPeriod = *(uint16_t*)(dataPtr); dataPtr+=2;
            device->PoIL.tasks[i + k].taskRealPeriodFiltered = *(uint16_t*)(dataPtr); dataPtr+=2;
        }

        i += device->request[4];
    }

    return PK_OK;
}
