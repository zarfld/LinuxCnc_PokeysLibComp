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

void PK_PoRelay8_DecodeStatus(sPoKeysDevice* device)
{
    uint8_t* ans;
    if (device == NULL) return;

    ans = device->response;

    device->PEv2.SoftLimitStatus = ans[3];
    device->PEv2.AxisEnabledStatesMask = ans[4];
    device->PEv2.LimitOverride = ans[5];

    // Basic engine states  
    device->PEv2.PulseEngineEnabled = ans[8];
    device->PEv2.PulseEngineActivated = ans[9];
    device->PEv2.PulseEngineState = ans[10];
    device->PEv2.ChargePumpEnabled = ans[11];
    device->PEv2.PulseGeneratorType = ans[15];

    // Switch states
    device->PEv2.LimitStatusP = ans[12];
    device->PEv2.LimitStatusN = ans[13];
    device->PEv2.HomeStatus = ans[14];

    memcpy(device->PEv2.AxesState, ans + 16, 8);
    memcpy(device->PEv2.CurrentPosition, ans + 24, 8 * 4);

    // Engine info
    device->PEv2.info.nrOfAxes = ans[56];
    device->PEv2.info.maxPulseFrequency = ans[57];
    device->PEv2.info.bufferDepth = ans[58];
    device->PEv2.info.slotTiming = ans[59];

    device->PEv2.EmergencySwitchPolarity = ans[60];

    // Other inputs
    device->PEv2.ErrorInputStatus = ans[61];
    device->PEv2.MiscInputStatus = ans[62];
}

/*
Device identification (0x10)
Device identification returns information on the PoRelay8 device, including device type (10/1),
firmware version and 32-bit device identifier.
*/
int32_t PK_PoRelay8_DeviceIdentification(sPoKeysDevice* device)
{

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x7B, 0x10, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;


    //ans[0] - Address  0x7B (R)
    //ans[1] - STATUS   (0x1A)
    device->PoRelay8.TYPE[0] = device->response[2];      // TYPE_1 (10)
    device->PoRelay8.TYPE[1] = device->response[3];      // TYPE_2 (1)
    device->PoRelay8.FW_ver[0] = device->response[4];    // FW_ver_1 
    device->PoRelay8.FW_ver[1] = device->response[5];    // FW_ver_2 
    device->PoRelay8.DeviceID = device->response[6];     // Device ID   (32 - bit)
}


/*
Configuration read (0x11)
This command is used to access the settings of the PoRelay8 device

The following parameters are available (by index):
Index   Description                                         Default value
0       Device's I2C address                                0x7B
1       PoExtBus daisy-chain position (data index)          0
2       CAN daisy-chain position (data index)               0
3       Number of PoRelay8 devices on CAN bus (additional
        CAN frames are sent if more than 8 PoRelay8 devices
        present)
                                                            10
4       Failsafe timeout (in ms)                            5000
5       Disable CRC check on PoExtBus                       0
6       CAN bus timing option
            0 – default CAN bitrate of 250 kbit/s
            125 – CAN bitrate of 125 kbit/s
            250 – CAN bitrate of 250 kbit/s
            500 – CAN bitrate of 500 kbit/s
            1000 – CAN bitrate of 1000 kbit/s
                                                            0
7       CAN bus message ID                                  0x108
8       PoIL master enable switch and PoIL core ID          0

*/
int32_t PK_PoRelay8_ConfigurationRead(sPoKeysDevice* device) // Retrieves I2C bus activation status
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;
    int i;
    for (i = 0; i < 9; i++)
    {
        uint8_t chkSum;
        CreateRequest(device->request, 0x7B, 0x11, 1, 0, 0);
            chkSum = getChecksum(device->request);
            device->request[4] = chkSum;
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
        // expecting problem 32Bit response but uint8 for response[3]
        // need to find out if response[3] to response[11] need to be merged to uint32 before reading values
        switch (i)
        {
            case 0:
                //0 Device's I2C address 0x7B
                device->PoRelay8.i2cAddress = device->response[3];
            case 1:
                //1 PoExtBus daisy-chain position (data index) 0
                device->PoRelay8.PoExtBus_Position = device->response[3];
            case 2:
                //2 CAN daisy-chain position (data index)
                device->PoRelay8.CAN_Position = device->response[3];
            case 3:
                //3 Number of PoRelay8 devices on CAN bus (additional CAN frames are sent if more than 8 PoRelay8 devices present)
                device->PoRelay8.CAN_PoRelayCount = device->response[3];
            case 4:
                //4 Failsafe timeout (in ms)
                device->PoRelay8.Failsafe_timeout = device->response[3];
            case 5:
                //5 Disable CRC check on PoExtBus
                device->PoRelay8.DisableCRC = device->response[3];
            case 6:
                //6 CAN bus timing option
                device->PoRelay8.CAN_TimingOption = device->response[3];
            case 7:
                //7 CAN bus message ID 0x108
                device->PoRelay8.CAN_MessageID = device->response[3];
            case 8:
                //8 PoIL master enable switch and PoIL core ID 0
                device->PoRelay8.POIL_MasterEnable = device->response[3];
        }
    }

    return PK_OK;
}

int32_t PK_PoRelay8_ConfigurationWrite(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;
    int i;
    for (i = 0; i < 9; i++)
    {
        uint8_t chkSum;
        CreateRequest(device->request, 0x7B, 0x11, 1, 0, 0);


        // expecting problem 32Bit response but uint8 for response[3]
        // need to find out if response[3] to response[11] need to be merged to uint32 before reading values
        switch (i)
        {
        case 0:
            //0 Device's I2C address 0x7B
            device->response[3] = device->PoRelay8.i2cAddress;
        case 1:
            //1 PoExtBus daisy-chain position (data index) 0
            device->response[3] = device->PoRelay8.PoExtBus_Position;
        case 2:
            //2 CAN daisy-chain position (data index)
            device->response[3] = device->PoRelay8.CAN_Position;
        case 3:
            //3 Number of PoRelay8 devices on CAN bus (additional CAN frames are sent if more than 8 PoRelay8 devices present)
            device->response[3] = device->PoRelay8.CAN_PoRelayCount;
        case 4:
            //4 Failsafe timeout (in ms)
            device->response[3] = device->PoRelay8.Failsafe_timeout;
        case 5:
            //5 Disable CRC check on PoExtBus
            device->response[3] = device->PoRelay8.DisableCRC;
        case 6:
            //6 CAN bus timing option
            device->response[3] = device->PoRelay8.CAN_TimingOption;
        case 7:
            //7 CAN bus message ID 0x108
            device->response[3] = device->PoRelay8.CAN_MessageID;
        case 8:
            //8 PoIL master enable switch and PoIL core ID 0
            device->response[3] = device->PoRelay8.POIL_MasterEnable;
        }
        chkSum = getChecksum(device->request);
        device->request[12] = chkSum;
        if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
    }

    return PK_OK;
}

int32_t PK_PoRelay8_ConfigurationSave(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x7B, 0xA5, 0, 0, 0);
     return SendRequest(device);
}

int32_t PK_PoRelay8_SetOutputs(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;
    uint8_t chkSum;
    CreateRequest(device->request, 0x7B, 0x20, 0, 0, 0);
    device->request[3] = device->PoRelay8.statusOut[0];
    chkSum = getChecksum(device->request);
    device->request[4] = chkSum;
    if (SendRequest(device) == PK_OK)
    {
        device->PoRelay8.statusIn = device->response[2];
    }
    else
    {
        return PK_ERR_GENERIC;
    }
}

int32_t PK_PoRelay8_SetOutputsArray(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x7B, 0x21, 0, 0, 0);
    device->request[1] = 0x7B;
    device->request[2] = 0x21;
    device->request[3] = device->PoRelay8.statusOut[0];
    device->request[4] = device->PoRelay8.statusOut[1];
    device->request[5] = device->PoRelay8.statusOut[2];
    device->request[6] = device->PoRelay8.statusOut[3];
    device->request[7] = device->PoRelay8.statusOut[4];
    device->request[8] = device->PoRelay8.statusOut[5];
    device->request[9] = device->PoRelay8.statusOut[6];
    device->request[10] = device->PoRelay8.statusOut[7];
    device->request[11] = device->PoRelay8.statusOut[8];
    device->request[12] = device->PoRelay8.statusOut[9];


    device->request[13] = getChecksum(device->request);

    if (SendRequest(device) == PK_OK)
    {
        device->PoRelay8.statusIn = device->response[2];
    }
    else
    {
        return PK_ERR_GENERIC;
    }
   
}

int32_t PK_PoRelay8_SetOutputsX1(sPoKeysDevice* device, uint8_t OutPuts)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;


    CreateRequest(device->request, 0x7B, 0x20, 0, 0, 0);
    device->request[3] = OutPuts;

    return SendRequest(device);
}

int32_t PK_PoRelay8_SetOutputsX10(sPoKeysDevice* device, uint8_t OutPuts[10])
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;


    CreateRequest(device->request, 0x7B, 0x21, 0, 0, 0);
    device->request[3] = OutPuts[0];
    device->request[4] = OutPuts[1];
    device->request[5] = OutPuts[2];
    device->request[6] = OutPuts[3];
    device->request[7] = OutPuts[4];
    device->request[8] = OutPuts[5];
    device->request[9] = OutPuts[6];
    device->request[10] = OutPuts[7];
    device->request[11] = OutPuts[8];
    device->request[12] = OutPuts[9];

    return SendRequest(device);
}

int32_t PK_PoRelay8_ReEnablePoExtBus(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x7B, 0x2F, 0, 0, 0);

    return SendRequest(device);
}

int32_t PK_PoRelay8_POILcommand(sPoKeysDevice* device, uint8_t CanMsgId, uint8_t CanMsgData)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x7B, 0x30, CanMsgId, CanMsgData,0);

    return SendRequest(device);
}

int32_t PK_PoRelay8_SendMessage2CanBus(sPoKeysDevice* device, uint8_t Flags, uint8_t CanMsgId, uint8_t CanMsgData)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x7B, 0x40, Flags, CanMsgId, CanMsgData);

    return SendRequest(device);
}
