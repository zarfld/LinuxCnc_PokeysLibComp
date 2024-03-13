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


void PK_PEv2_DecodeStatus(sPoKeysDevice * device)
{
    uint8_t * ans;
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
    memcpy(device->PEv2.CurrentPosition, ans + 24, 8*4);

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

int32_t PK_PEv2_StatusGet(sPoKeysDevice * device)
{
    // Do some 'random' magic with numbers
    uint8_t tstB;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    tstB = (0x10 + device->requestID) % 199;

    // Send request
    CreateRequest(device->request, 0x85, 0x00, tstB, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Check if response is valid
    if (device->response[63] == (uint8_t)(tstB + 0x5A))
    {
        // Decode status
        PK_PEv2_DecodeStatus(device);
        return PK_OK;
    } else
    {
        device->PEv2.PulseEngineActivated = 0;
        device->PEv2.PulseEngineEnabled = 0;
        return PK_ERR_GENERIC;
    }

}



int32_t PK_PEv2_Status2Get(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Send request
    CreateRequest(device->request, 0x85, 0x08, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	device->PEv2.DedicatedLimitNInputs = device->response[8];
	device->PEv2.DedicatedLimitPInputs = device->response[9];
	device->PEv2.DedicatedHomeInputs = device->response[10];
	return PK_OK;
}

// Configure (setup) the pulse engine
int32_t PK_PEv2_PulseEngineSetup(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    CreateRequest(device->request, 0x85, 0x01, 0, 0, 0);

    // Fill the information
    device->request[8] = device->PEv2.PulseEngineEnabled;
    device->request[9] = device->PEv2.ChargePumpEnabled;
    device->request[10] = device->PEv2.PulseGeneratorType;
    device->request[11] = device->PEv2.PulseEngineBufferSize;
    device->request[12] = device->PEv2.EmergencySwitchPolarity;
    device->request[13] = device->PEv2.AxisEnabledStatesMask;

    // Send request
    return SendRequest(device);
}



// Read additional parameters
int32_t PK_PEv2_AdditionalParametersGet(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Send request
    CreateRequest(device->request, 0x85, 0x06, 0, 0, 1);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	device->PEv2.EmergencyInputPin = device->response[8];    

    return PK_OK;
}

// Set additional parameters
int32_t PK_PEv2_AdditionalParametersSet(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x06, 1, 0, 0);
	device->request[8] = device->PEv2.EmergencyInputPin;

    // Send request
    return SendRequest(device);
}


// Retrieve single axis parameters. Axis ID is in param1
int32_t PK_PEv2_AxisConfigurationGet(sPoKeysDevice * device)
{
    sPoKeysPEv2 * pe;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->PEv2.param1 >= 8) return PK_ERR_PARAMETER;

    // Send request
    CreateRequest(device->request, 0x85, 0x10, device->PEv2.param1, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Pointer to PEv2 structure for better code readability
    pe = &device->PEv2;

    // Read the structure
    pe->AxesConfig[pe->param1] = device->response[8];
    pe->AxesSwitchConfig[pe->param1] = device->response[9];

    pe->PinHomeSwitch[pe->param1] = device->response[10];
    pe->PinLimitMSwitch[pe->param1] = device->response[11];
    pe->PinLimitPSwitch[pe->param1] = device->response[12];

    pe->HomingSpeed[pe->param1] = device->response[13];
    pe->HomingReturnSpeed[pe->param1] = device->response[14];

    pe->MPGjogEncoder[pe->param1] = device->response[15];

    // Convert parameters... assume little-endian format
    pe->MaxSpeed[pe->param1] = *(float*)(device->response + 16);
    pe->MaxAcceleration[pe->param1] = *(float*)(device->response + 20);
    pe->MaxDecceleration[pe->param1] = *(float*)(device->response + 24);

    pe->SoftLimitMinimum[pe->param1] = *(int32_t*)(device->response + 28);
    pe->SoftLimitMaximum[pe->param1] = *(int32_t*)(device->response + 32);

    pe->MPGjogMultiplier[pe->param1] = *(int16_t*)(device->response + 36);

    pe->AxisEnableOutputPins[pe->param1] = device->response[38];
    pe->InvertAxisEnable[pe->param1] = device->response[39];

	pe->FilterLimitMSwitch[pe->param1] = device->response[40];
	pe->FilterLimitPSwitch[pe->param1] = device->response[41];
	pe->FilterHomeSwitch[pe->param1] = device->response[42];

	pe->HomingAlgorithm[pe->param1] = device->response[43];
	// MPG 1x mode here
	pe->HomeBackOffDistance[pe->param1] = *(uint32_t*)(device->response + 45);
    pe->MPGjogDivider[pe->param1] = *(uint16_t*)(device->response + 49);
	pe->AxisSignalOptions[pe->param1] = device->response[51];
	pe->FilterProbeInput = device->response[52];
    return PK_OK;
}

// Set single axis parameters. Axis ID is in param1
int32_t PK_PEv2_AxisConfigurationSet(sPoKeysDevice * device)
{
    sPoKeysPEv2 * pe;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->PEv2.param1 >= 8) return PK_ERR_PARAMETER;

    // Create request
    CreateRequest(device->request, 0x85, 0x11, device->PEv2.param1, 0, 0);

    // Pointer to PEv2 structure for better code readability
    pe = &device->PEv2;

    // Fill the structure
    device->request[8] = pe->AxesConfig[pe->param1];
    device->request[9] = pe->AxesSwitchConfig[pe->param1];

    device->request[10] = pe->PinHomeSwitch[pe->param1];
    device->request[11] = pe->PinLimitMSwitch[pe->param1];
    device->request[12] = pe->PinLimitPSwitch[pe->param1];

    device->request[13] = pe->HomingSpeed[pe->param1];
    device->request[14] = pe->HomingReturnSpeed[pe->param1];

    device->request[15] = pe->MPGjogEncoder[pe->param1];

    *(float*)(device->request + 16) = pe->MaxSpeed[pe->param1];
    *(float*)(device->request + 20) = pe->MaxAcceleration[pe->param1];
    *(float*)(device->request + 24) = pe->MaxDecceleration[pe->param1];

    *(int32_t*)(device->request + 28) = pe->SoftLimitMinimum[pe->param1];
    *(int32_t*)(device->request + 32) = pe->SoftLimitMaximum[pe->param1];
    *(int16_t*)(device->request + 36) = (int16_t)pe->MPGjogMultiplier[pe->param1];

    device->request[38] = pe->AxisEnableOutputPins[pe->param1];
    device->request[39] = pe->InvertAxisEnable[pe->param1];

	device->request[40] = pe->FilterLimitMSwitch[pe->param1];
	device->request[41] = pe->FilterLimitPSwitch[pe->param1];
	device->request[42] = pe->FilterHomeSwitch[pe->param1];
	device->request[43] = pe->HomingAlgorithm[pe->param1];
	device->request[44] = 0;

	*(uint32_t*)(device->request + 45) = pe->HomeBackOffDistance[pe->param1];
    *(uint16_t*)(device->request + 49) = pe->MPGjogDivider[pe->param1];

	device->request[51] = pe->AxisSignalOptions[pe->param1];
	device->request[52] = pe->FilterProbeInput;

    // Send request
    return SendRequest(device);
}

// Set positions - param2 is used for bit-mapped axis selection
int32_t PK_PEv2_PositionSet(sPoKeysDevice * device)
{
    int i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->PEv2.param2 == 0) return PK_ERR_PARAMETER;

    // Create request
    CreateRequest(device->request, 0x85, 0x03, device->PEv2.param2, 0, 0);

    for (i = 0; i < 8; i++)
    {
        *(int32_t*)(&device->request[8+i*4]) = device->PEv2.PositionSetup[i];
    }

    // Send request
    return SendRequest(device);
}

// Set pulse engine state
int32_t PK_PEv2_PulseEngineStateSet(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
	CreateRequest(device->request, 0x85, 0x02, device->PEv2.PulseEngineStateSetup, device->PEv2.LimitOverrideSetup, device->PEv2.AxisEnabledMask);

    // Send request
    return SendRequest(device);
}

// Execute the move. Position or speed is specified by the ReferencePositionSpeed
int32_t PK_PEv2_PulseEngineMove(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x20, 0, 0, 0);

    memcpy(&device->request[8], device->PEv2.ReferencePositionSpeed, 8*4);

    // Send request
    return SendRequest(device);
}


// Execute the move. Position or speed is specified by the ReferencePositionSpeed
int32_t PK_PEv2_PulseEngineMovePV(sPoKeysDevice * device)
{
    int i;
    uint16_t tmpVelocity;

    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x25, device->PEv2.param2, 0, 0);

    memcpy(&device->request[8], device->PEv2.ReferencePositionSpeed, 8*4);
    for (i = 0; i < 8; i++) {
        tmpVelocity = (uint16_t)(device->PEv2.ReferenceVelocityPV[i] * 65535);
        memcpy(&device->request[40 + i * 2], &tmpVelocity, 2);
    }

    // Send request
    return SendRequest(device);
}

// Read external outputs state - save them to ExternalRelayOutputs and ExternalOCOutputs
int32_t PK_PEv2_ExternalOutputsGet(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Send request
    CreateRequest(device->request, 0x85, 0x04, 0, 0, 1);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    device->PEv2.ExternalRelayOutputs = device->response[3];
    device->PEv2.ExternalOCOutputs = device->response[4];

    return PK_OK;
}

// Set external outputs state (from ExternalRelayOutputs and ExternalOCOutputs)
int32_t PK_PEv2_ExternalOutputsSet(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x04, device->PEv2.ExternalRelayOutputs, device->PEv2.ExternalOCOutputs, 0);
    // Send request
    return SendRequest(device);
}

// Transfer motion buffer to device. The number of new entries (newMotionBufferEntries) must be specified
// The number of accepted entries is saved to motionBufferEntriesAccepted.
// In addition, pulse engine state is read (PEv2_GetStatus)
int32_t PK_PEv2_BufferFill(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0xFF, device->PEv2.newMotionBufferEntries, device->PEv2.PulseEngineEnabled & 0x0F, 0);

    // Copy buffer
    memcpy(&device->request[8], device->PEv2.MotionBuffer, 56);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    device->PEv2.motionBufferEntriesAccepted = device->response[2];

    // Decode status
    PK_PEv2_DecodeStatus(device);

    return PK_OK;
}

// Transfer motion buffer to device. The number of new entries (newMotionBufferEntries) must be specified
// The number of accepted entries is saved to motionBufferEntriesAccepted.
// In addition, pulse engine state is read (PEv2_GetStatus)
int32_t PK_PEv2_BufferFill_16(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0xFE, device->PEv2.newMotionBufferEntries, device->PEv2.PulseEngineEnabled & 0x0F, 0);

    // Copy buffer
    memcpy(&device->request[8], device->PEv2.MotionBuffer, 56);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    device->PEv2.motionBufferEntriesAccepted = device->response[2];

    // Decode status
    PK_PEv2_DecodeStatus(device);

    return PK_OK;
}

int32_t PK_PEv2_BufferFillLarge(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;	

    // Create request
    CreateRequest(device->request, 0xB0, 0, 0xFF, device->PEv2.newMotionBufferEntries, device->PEv2.PulseEngineEnabled & 0x0F);

    // Copy buffer
    memcpy(device->multiPartData, device->PEv2.MotionBuffer, 448);

    // Send request
	if (SendRequest_multiPart(device) != PK_OK) return PK_ERR_TRANSFER;

    device->PEv2.motionBufferEntriesAccepted = device->response[2];

    // Decode status
    PK_PEv2_DecodeStatus(device);

    return PK_OK;
}

int32_t PK_PEv2_BufferFillLarge_16(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;	

    // Create request
    CreateRequest(device->request, 0xB0, 0, 0xFE, device->PEv2.newMotionBufferEntries, device->PEv2.PulseEngineEnabled & 0x0F);

    // Copy buffer
    memcpy(device->multiPartData, device->PEv2.MotionBuffer, 448);

    // Send request
	if (SendRequest_multiPart(device) != PK_OK) return PK_ERR_TRANSFER;

    device->PEv2.motionBufferEntriesAccepted = device->response[2];

    // Decode status
    PK_PEv2_DecodeStatus(device);

    return PK_OK;
}

// Clear motion buffer in device
int32_t PK_PEv2_BufferClear(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0xF0, 0, 0, 0);
    // Send request
    return SendRequest(device);
}

// Reboot pulse engine v2
int32_t PK_PEv2_PulseEngineReboot(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x05, 0, 0, 0);
    // Send request
    return SendRequest(device);
}

// Start the homing procedure. Home offsets must be provided in the HomeOffsets
// Axes to home are selected as bit-mapped HomingStartMaskSetup value
int32_t PK_PEv2_HomingStart(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x21, device->PEv2.HomingStartMaskSetup, 0, 0);

    memcpy(&device->request[8], device->PEv2.HomeOffsets, 8 * 4);

    // Send request
    return SendRequest(device);
}

// Finish the homing procedure
int32_t PK_PEv2_HomingFinish(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
	CreateRequest(device->request, 0x85, 0x22, device->PEv2.PulseEngineStateSetup, 1, 0);
    // Send request
    return SendRequest(device);
}

// Start the probing procedure.
// ProbeMaxPosition defines the maximum position in position ticks where probing error will be thrown
// ProbeSpeed defines the probing speed (1 = max speed)
// ProbeInput defines the extenal input (values 1-8) or PoKeys pin (0-based Pin ID + 9)
// ProbeInputPolarity defines the polarity of the probe signal
int32_t PK_PEv2_ProbingStart(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x23, device->PEv2.ProbeStartMaskSetup, 0, 0);

    memcpy(&device->request[8], device->PEv2.ProbeMaxPosition, 8 * 4);
    *(float*)(&device->request[40]) = device->PEv2.ProbeSpeed;
    device->request[44] = device->PEv2.ProbeInput;
    device->request[45] = device->PEv2.ProbeInputPolarity;

    // Send request
    return SendRequest(device);
}

int32_t PK_PEv2_ProbingHybridStart(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x23, 0, 1, 0);

    device->request[44] = device->PEv2.ProbeInput;
    device->request[45] = device->PEv2.ProbeInputPolarity;

    // Send request
    return SendRequest(device);
}

// Finish the probing procedure. Probe position and status are saved to ProbePosition and ProbeStatus
int32_t PK_PEv2_ProbingFinish(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x24, 0, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Copy the probe result position
    memcpy(device->PEv2.ProbePosition, &device->response[8], 8*4);
    device->PEv2.ProbeStatus = device->response[40];

    return PK_OK;
}
// Finish the probing procedure. Probe position and status are saved to ProbePosition and ProbeStatus
int32_t PK_PEv2_ProbingFinishSimple(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x24, 1, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Copy the probe result position
    memcpy(device->PEv2.ProbePosition, &device->response[8], 8*4);

    return PK_OK;
}

int32_t PK_PEv2_ThreadingPrepareForTrigger(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x30, 0, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_PEv2_ThreadingForceTriggerReady(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x31, 0, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_PEv2_ThreadingTrigger(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x32, 0, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_PEv2_ThreadingRelease(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x33, 0, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_PEv2_ThreadingCancel(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x34, 0, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_PEv2_ThreadingStatusGet(sPoKeysDevice * device)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x35, 0, 0, 0);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;


	device->PEv2.TriggerPreparing = device->response[8];
	device->PEv2.TriggerPrepared = device->response[9];
	device->PEv2.TriggerPending = device->response[10];
	device->PEv2.TriggerActive = device->response[11];

	device->PEv2.SpindleSpeedEstimate = *(int32_t*)(device->response + 12);
	device->PEv2.SpindlePositionError = *(int32_t*)(device->response + 16);
	device->PEv2.SpindleRPM =			*(int32_t*)(device->response + 20);

	device->PEv2.TriggerIngnoredAxisMask = device->response[24];

	device->PEv2.spindleIndexCounter  = *(int32_t*)(device->response + 25);

	memcpy(device->PEv2.DebugValues, device->response + 29, 35);
	return PK_OK;

}

int32_t PK_PEv2_ThreadingSetup(sPoKeysDevice * device, uint8_t sensorMode, uint16_t ticksPerRevolution, uint16_t tagetSpindleRPM, uint16_t filterGainSpeed, uint16_t filterGainPosition)
{
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x36, 0, 0, 0);

	device->request[8] = sensorMode;
	*(uint16_t*)(device->request + 12) = ticksPerRevolution;
	*(uint16_t*)(device->request + 14) = tagetSpindleRPM;
	*(uint16_t*)(device->request + 16) = filterGainSpeed;
	*(uint16_t*)(device->request + 18) = filterGainPosition;
	device->request[20] = device->PEv2.TriggerIngnoredAxisMask;

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_PEv2_BacklashCompensationSettings_Get(sPoKeysDevice * device)
{
	int32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x40, 0, 0, 0);

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	for (i = 0; i < 8; i++)
	{
		device->PEv2.BacklashWidth[i] = *(uint16_t*)(device->response + 8 + i * 4);
		device->PEv2.BacklashAcceleration[i] = device->response[10 + i * 4];
		device->PEv2.BacklashRegister[i] = *(int16_t*)(device->response + 40 + i * 2);
	}
	device->PEv2.BacklashCompensationEnabled = device->response[3];
	device->PEv2.BacklashCompensationMaxSpeed = device->response[4];
			
	return PK_OK;
}

int32_t PK_PEv2_BacklashCompensationSettings_Set(sPoKeysDevice * device)
{
	int32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Create request
	CreateRequest(device->request, 0x85, 0x41, device->PEv2.BacklashCompensationEnabled, device->PEv2.BacklashCompensationMaxSpeed, 0);

	for (i = 0; i < 8; i++)
	{
		*(uint16_t*)(device->request + 8 + i * 4) = device->PEv2.BacklashWidth[i];
		device->request[10 + i * 4] = device->PEv2.BacklashAcceleration[i];
	}		

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}


int32_t PK_PEv2_SyncedPWMSetup(sPoKeysDevice * device, uint8_t enabled, uint8_t srcAxis, uint8_t dstPWMChannel)
{    
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x0A, enabled, srcAxis, dstPWMChannel);

    // Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    return PK_OK;
}

int32_t PK_PEv2_SyncOutputsSetup(sPoKeysDevice * device)
{
	if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Create request
	CreateRequest(device->request, 0x85, 0x0B, device->PEv2.SyncFastOutputsAxisID > 0, device->PEv2.SyncFastOutputsAxisID - 1, 0);
	memcpy(device->request + 8, device->PEv2.SyncFastOutputsMapping, 8);

	// Send request
	if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	return PK_OK;
}

int32_t PK_PoStep_ConfigurationGet(sPoKeysDevice * device)
{
	int32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x50, 0, 0, 0);

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	// Parse settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->PoSteps.drivers[i].DriverType = device->response[8 + i*3];
		device->PoSteps.drivers[i].AddressI2C = device->response[9 + i*3];
		device->PoSteps.drivers[i].UpdateConfig = device->response[10 + i*3];
	}
	device->PoSteps.EnablePoStepCommunication = device->response[4];
			
	return PK_OK;
}

int32_t PK_PoStep_ConfigurationSet(sPoKeysDevice * device)
{
	int32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Create request
	CreateRequest(device->request, 0x85, 0x50, 0x10, device->PoSteps.EnablePoStepCommunication, 0);

	// Insert settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->request[8 + i * 3] = device->PoSteps.drivers[i].DriverType;
		device->request[9 + i * 3] = device->PoSteps.drivers[i].AddressI2C;
		device->request[10 + i * 3] = device->PoSteps.drivers[i].UpdateConfig;
	}

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	// Parse settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->PoSteps.drivers[i].DriverType = device->response[8 + i*3];
		device->PoSteps.drivers[i].AddressI2C = device->response[9 + i*3];
		device->PoSteps.drivers[i].UpdateConfig = device->response[10 + i*3];
	}
	device->PoSteps.EnablePoStepCommunication = device->response[4];
	
	return PK_OK;
}


int32_t PK_PoStep_StatusGet(sPoKeysDevice * device)
{
	int32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x51, 0, 0, 0);

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	// Parse settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->PoSteps.drivers[i].SupplyVoltage = device->response[8 + i*6];
		device->PoSteps.drivers[i].Temperature = device->response[9 + i*6];
		device->PoSteps.drivers[i].InputStatus = device->response[10 + i*6];
		device->PoSteps.drivers[i].DriverStatus = device->response[11 + i*6];
		device->PoSteps.drivers[i].FaultStatus = device->response[12 + i*6];
		device->PoSteps.drivers[i].UpdateState = device->response[13 + i*6];
	}
			
	return PK_OK;
}


int32_t PK_PoStep_DriverConfigurationGet(sPoKeysDevice * device)
{
	int32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Current settings
    // Create request
    CreateRequest(device->request, 0x85, 0x52, 0, 0, 0);

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	// Parse settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->PoSteps.drivers[i].Current_FS = *(uint16_t*)(device->response + 8 + i*6);
		device->PoSteps.drivers[i].Current_Idle = *(uint16_t*)(device->response + 10 + i*6);
		device->PoSteps.drivers[i].Current_Overheat = *(uint16_t*)(device->response + 12 + i*6);
	}

	// Mode settings
    // Create request
    CreateRequest(device->request, 0x85, 0x53, 0, 0, 0);

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	// Parse settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->PoSteps.drivers[i].DriverMode = device->response[8 + i*6];
		device->PoSteps.drivers[i].StepMode = device->response[9 + i*6];
		device->PoSteps.drivers[i].TemperatureLimit = device->response[10 + i*6];
	}
		
	return PK_OK;
}


int32_t PK_PoStep_DriverConfigurationSet(sPoKeysDevice * device)
{
	int32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

	// Current
	// Create request
	CreateRequest(device->request, 0x85, 0x52, 0x10, 0, 0);

	// Insert settings for each axis
	for (i = 0; i < 8; i++)
	{
		*(uint16_t*)(device->request + 8 + i*6) = device->PoSteps.drivers[i].Current_FS;
		*(uint16_t*)(device->request + 10 + i*6) = device->PoSteps.drivers[i].Current_Idle;
		*(uint16_t*)(device->request + 12 + i*6) = device->PoSteps.drivers[i].Current_Overheat;
	}

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	// Parse settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->PoSteps.drivers[i].Current_FS = *(uint16_t*)(device->response + 8 + i*6);
		device->PoSteps.drivers[i].Current_Idle = *(uint16_t*)(device->response + 10 + i*6);
		device->PoSteps.drivers[i].Current_Overheat = *(uint16_t*)(device->response + 12 + i*6);
	}

	// Modes
	// Create request
	CreateRequest(device->request, 0x85, 0x53, 0x10, 0, 0);

	// Insert settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->request[8 + i * 6] = device->PoSteps.drivers[i].DriverMode;
		device->request[9 + i * 6] = device->PoSteps.drivers[i].StepMode;
		device->request[10 + i * 6] = device->PoSteps.drivers[i].TemperatureLimit;
		device->request[11 + i * 6] = 0;
		device->request[12 + i * 6] = 0;
		device->request[13 + i * 6] = 0;
	}

	// Send request
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

	// Parse settings for each axis
	for (i = 0; i < 8; i++)
	{
		device->PoSteps.drivers[i].DriverMode = device->response[8 + i*6];
		device->PoSteps.drivers[i].StepMode = device->response[9 + i*6];
		device->PoSteps.drivers[i].TemperatureLimit = device->response[10 + i*6];
	}
	
	return PK_OK;
}

// Retrieve internal motor drivers parameters
int32_t PK_PEv2_InternalDriversConfigurationGet(sPoKeysDevice * device)
{
    sPoKeysPEv2 * pe;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Send request
    CreateRequest(device->request, 0x85, 0x18, 0, 0, 0);
    if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

    // Pointer to PEv2 structure for better code readability
    pe = &device->PEv2;

    for (int32_t i = 0; i < 4; i++) {
        pe->InternalDriverStepConfig[i] = device->response[8 + i*2];
        pe->InternalDriverCurrentConfig[i] = device->response[9 + i*2];
    }
    return PK_OK;
}

// Set internal motor drivers parameters
int32_t PK_PEv2_InternalDriversConfigurationSet(sPoKeysDevice * device)
{
    sPoKeysPEv2 * pe;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    // Create request
    CreateRequest(device->request, 0x85, 0x19, 0, 0, 0);

    // Pointer to PEv2 structure for better code readability
    pe = &device->PEv2;

    for (int32_t i = 0; i < 4; i++) {
        device->request[8 + i*2] = pe->InternalDriverStepConfig[i];
        device->request[9 + i*2] = pe->InternalDriverCurrentConfig[i];
    }

    // Send request
    return SendRequest(device);
}
