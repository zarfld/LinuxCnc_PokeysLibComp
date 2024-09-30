#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstring>
#include "PoKeysLib.h"

struct MockDeviceState {
    std::unordered_map<uint8_t, uint8_t> digitalPins;
    std::unordered_map<uint8_t, uint32_t> analogPins;
    std::unordered_map<uint8_t, uint32_t> counters;
    std::vector<std::string> logs;
};

MockDeviceState mockDeviceState;

void logFunctionCall(const std::string& functionName) {
    mockDeviceState.logs.push_back(functionName);
}

extern "C" {

sPoKeysDevice* PK_ConnectToDevice(uint32_t deviceIndex) {
    logFunctionCall("PK_ConnectToDevice");
    return new sPoKeysDevice();
}

sPoKeysDevice* PK_ConnectToDeviceWSerial(uint32_t serialNumber, uint32_t checkForNetworkDevicesAndTimeout) {
    logFunctionCall("PK_ConnectToDeviceWSerial");
    return new sPoKeysDevice();
}

sPoKeysDevice* PK_ConnectToDeviceWSerial_UDP(uint32_t serialNumber, uint32_t checkForNetworkDevicesAndTimeout) {
    logFunctionCall("PK_ConnectToDeviceWSerial_UDP");
    return new sPoKeysDevice();
}

sPoKeysDevice* PK_ConnectToNetworkDevice(sPoKeysNetworkDeviceSummary * device) {
    logFunctionCall("PK_ConnectToNetworkDevice");
    return new sPoKeysDevice();
}

void PK_DisconnectDevice(sPoKeysDevice* device) {
    logFunctionCall("PK_DisconnectDevice");
    delete device;
}

int32_t PK_DigitalIOSet(sPoKeysDevice* device) {
    logFunctionCall("PK_DigitalIOSet");
    for (const auto& pin : mockDeviceState.digitalPins) {
        mockDeviceState.digitalPins[pin.first] = pin.second;
    }
    return PK_OK;
}

int32_t PK_DigitalIOGet(sPoKeysDevice* device) {
    logFunctionCall("PK_DigitalIOGet");
    return PK_OK;
}

int32_t PK_DigitalIOSetSingle(sPoKeysDevice* device, uint8_t pinID, uint8_t pinValue) {
    logFunctionCall("PK_DigitalIOSetSingle");
    mockDeviceState.digitalPins[pinID] = pinValue;
    return PK_OK;
}

int32_t PK_DigitalIOGetSingle(sPoKeysDevice* device, uint8_t pinID, uint8_t* pinValue) {
    logFunctionCall("PK_DigitalIOGetSingle");
    *pinValue = mockDeviceState.digitalPins[pinID];
    return PK_OK;
}

int32_t PK_AnalogIOGet(sPoKeysDevice* device) {
    logFunctionCall("PK_AnalogIOGet");
    return PK_OK;
}

int32_t PK_AnalogIOGetAsArray(sPoKeysDevice* device, uint32_t * buffer) {
    logFunctionCall("PK_AnalogIOGetAsArray");
    for (const auto& pin : mockDeviceState.analogPins) {
        buffer[pin.first] = pin.second;
    }
    return PK_OK;
}

int32_t PK_DigitalCounterGet(sPoKeysDevice* device) {
    logFunctionCall("PK_DigitalCounterGet");
    return PK_OK;
}

int32_t PK_IsCounterAvailable(sPoKeysDevice* device, uint8_t pinID) {
    logFunctionCall("PK_IsCounterAvailable");
    return mockDeviceState.counters.find(pinID) != mockDeviceState.counters.end();
}

int32_t PK_IsCounterAvailableByDevice(uint32_t deviceTypeMask, uint8_t pinID) {
    logFunctionCall("PK_IsCounterAvailableByDevice");
    return mockDeviceState.counters.find(pinID) != mockDeviceState.counters.end();
}

int32_t PK_IsCounterAvailableByTypeID(uint32_t deviceTypeID, uint8_t pinID) {
    logFunctionCall("PK_IsCounterAvailableByTypeID");
    return mockDeviceState.counters.find(pinID) != mockDeviceState.counters.end();
}

int32_t PK_PEv2_PulseEngineMove(sPoKeysDevice* device) {
    logFunctionCall("PK_PEv2_PulseEngineMove");
    return PK_OK;
}

int32_t PK_PEv2_StatusGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_StatusGet");
    return PK_OK;
}

int32_t PK_PEv2_Status2Get(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_Status2Get");
    return PK_OK;
}

int32_t PK_PEv2_PulseEngineSetup(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_PulseEngineSetup");
    return PK_OK;
}

int32_t PK_PEv2_AdditionalParametersGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_AdditionalParametersGet");
    return PK_OK;
}

int32_t PK_PEv2_AdditionalParametersSet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_AdditionalParametersSet");
    return PK_OK;
}

int32_t PK_PEv2_AxisConfigurationGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_AxisConfigurationGet");
    return PK_OK;
}

int32_t PK_PEv2_AxisConfigurationSet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_AxisConfigurationSet");
    return PK_OK;
}

int32_t PK_PEv2_PositionSet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_PositionSet");
    return PK_OK;
}

int32_t PK_PEv2_PulseEngineStateSet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_PulseEngineStateSet");
    return PK_OK;
}

int32_t PK_PEv2_ExternalOutputsGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ExternalOutputsGet");
    return PK_OK;
}

int32_t PK_PEv2_ExternalOutputsSet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ExternalOutputsSet");
    return PK_OK;
}

int32_t PK_PEv2_BufferFill(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_BufferFill");
    return PK_OK;
}

int32_t PK_PEv2_BufferFill_16(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_BufferFill_16");
    return PK_OK;
}

int32_t PK_PEv2_BufferFillLarge(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_BufferFillLarge");
    return PK_OK;
}

int32_t PK_PEv2_BufferFillLarge_16(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_BufferFillLarge_16");
    return PK_OK;
}

int32_t PK_PEv2_BufferClear(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_BufferClear");
    return PK_OK;
}

int32_t PK_PEv2_PulseEngineReboot(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_PulseEngineReboot");
    return PK_OK;
}

int32_t PK_PEv2_HomingStart(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_HomingStart");
    return PK_OK;
}

int32_t PK_PEv2_HomingFinish(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_HomingFinish");
    return PK_OK;
}

int32_t PK_PEv2_ProbingStart(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ProbingStart");
    return PK_OK;
}

int32_t PK_PEv2_ProbingHybridStart(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ProbingHybridStart");
    return PK_OK;
}

int32_t PK_PEv2_ProbingFinish(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ProbingFinish");
    return PK_OK;
}

int32_t PK_PEv2_ProbingFinishSimple(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ProbingFinishSimple");
    return PK_OK;
}

int32_t PK_PEv2_SyncedPWMSetup(sPoKeysDevice * device, uint8_t enabled, uint8_t srcAxis, uint8_t dstPWMChannel) {
    logFunctionCall("PK_PEv2_SyncedPWMSetup");
    return PK_OK;
}

int32_t PK_PEv2_ThreadingPrepareForTrigger(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ThreadingPrepareForTrigger");
    return PK_OK;
}

int32_t PK_PEv2_ThreadingForceTriggerReady(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ThreadingForceTriggerReady");
    return PK_OK;
}

int32_t PK_PEv2_ThreadingTrigger(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ThreadingTrigger");
    return PK_OK;
}

int32_t PK_PEv2_ThreadingRelease(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ThreadingRelease");
    return PK_OK;
}

int32_t PK_PEv2_ThreadingStatusGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ThreadingStatusGet");
    return PK_OK;
}

int32_t PK_PEv2_ThreadingCancel(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_ThreadingCancel");
    return PK_OK;
}

int32_t PK_PEv2_ThreadingSetup(sPoKeysDevice * device, uint8_t sensorMode, uint16_t ticksPerRevolution, uint16_t tagetSpindleRPM, uint16_t filterGainSpeed, uint16_t filterGainPosition) {
    logFunctionCall("PK_PEv2_ThreadingSetup");
    return PK_OK;
}

int32_t PK_PEv2_BacklashCompensationSettings_Get(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_BacklashCompensationSettings_Get");
    return PK_OK;
}

int32_t PK_PEv2_BacklashCompensationSettings_Set(sPoKeysDevice * device) {
    logFunctionCall("PK_PEv2_BacklashCompensationSettings_Set");
    return PK_OK;
}

int32_t PK_PoStep_ConfigurationGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PoStep_ConfigurationGet");
    return PK_OK;
}

int32_t PK_PoStep_ConfigurationSet(sPoKeysDevice * device) {
    logFunctionCall("PK_PoStep_ConfigurationSet");
    return PK_OK;
}

int32_t PK_PoStep_StatusGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PoStep_StatusGet");
    return PK_OK;
}

int32_t PK_PoStep_DriverConfigurationGet(sPoKeysDevice * device) {
    logFunctionCall("PK_PoStep_DriverConfigurationGet");
    return PK_OK;
}

int32_t PK_PoStep_DriverConfigurationSet(sPoKeysDevice * device) {
    logFunctionCall("PK_PoStep_DriverConfigurationSet");
    return PK_OK;
}

int32_t PK_I2CSetStatus(sPoKeysDevice* device, uint8_t activated) {
    logFunctionCall("PK_I2CSetStatus");
    return PK_OK;
}

int32_t PK_I2CGetStatus(sPoKeysDevice* device, uint8_t* activated) {
    logFunctionCall("PK_I2CGetStatus");
    *activated = 1;
    return PK_OK;
}

int32_t PK_I2CWriteStart(sPoKeysDevice* device, uint8_t address, uint8_t* buffer, uint8_t iDataLength) {
    logFunctionCall("PK_I2CWriteStart");
    return PK_OK;
}

int32_t PK_I2CWriteAndReadStart(sPoKeysDevice* device, uint8_t address, uint8_t* buffer, uint8_t iDataLengthWrite, uint8_t iDataLengthRead) {
    logFunctionCall("PK_I2CWriteAndReadStart");
    return PK_OK;
}

int32_t PK_I2CWriteStatusGet(sPoKeysDevice* device, uint8_t* status) {
    logFunctionCall("PK_I2CWriteStatusGet");
    *status = PK_I2C_STAT_OK;
    return PK_OK;
}

int32_t PK_I2CReadStart(sPoKeysDevice* device, uint8_t address, uint8_t iDataLength) {
    logFunctionCall("PK_I2CReadStart");
    return PK_OK;
}

int32_t PK_I2CReadStatusGet(sPoKeysDevice* device, uint8_t* status, uint8_t* iReadBytes, uint8_t* buffer, uint8_t iMaxBufferLength) {
    logFunctionCall("PK_I2CReadStatusGet");
    *status = PK_I2C_STAT_OK;
    *iReadBytes = iMaxBufferLength;
    std::memset(buffer, 0, iMaxBufferLength);
    return PK_OK;
}

int32_t PK_I2CBusScanStart(sPoKeysDevice* device) {
    logFunctionCall("PK_I2CBusScanStart");
    return PK_OK;
}

int32_t PK_I2CBusScanGetResults(sPoKeysDevice* device, uint8_t* status, uint8_t* presentDevices, uint8_t iMaxDevices) {
    logFunctionCall("PK_I2CBusScanGetResults");
    *status = PK_I2C_STAT_OK;
    std::memset(presentDevices, 0, iMaxDevices);
    return PK_OK;
}

int32_t PK_PoNETGetPoNETStatus(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETGetPoNETStatus");
    return PK_OK;
}

int32_t PK_PoNETGetModuleSettings(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETGetModuleSettings");
    return PK_OK;
}

int32_t PK_PoNETGetModuleStatusRequest(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETGetModuleStatusRequest");
    return PK_OK;
}

int32_t PK_PoNETGetModuleStatus(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETGetModuleStatus");
    return PK_OK;
}

int32_t PK_PoNETSetModuleStatus(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETSetModuleStatus");
    return PK_OK;
}

int32_t PK_PoNETSetModulePWM(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETSetModulePWM");
    return PK_OK;
}

int32_t PK_PoNETGetModuleLightRequest(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETGetModuleLightRequest");
    return PK_OK;
}

int32_t PK_PoNETGetModuleLight(sPoKeysDevice* device) {
    logFunctionCall("PK_PoNETGetModuleLight");
    return PK_OK;
}

int32_t PK_1WireStatusSet(sPoKeysDevice* device, uint8_t activated) {
    logFunctionCall("PK_1WireStatusSet");
    return PK_OK;
}

int32_t PK_1WireStatusGet(sPoKeysDevice* device, uint8_t* activated) {
    logFunctionCall("PK_1WireStatusGet");
    *activated = 1;
    return PK_OK;
}

int32_t PK_1WireWriteReadStart(sPoKeysDevice* device, uint8_t WriteCount, uint8_t ReadCount, uint8_t * data) {
    logFunctionCall("PK_1WireWriteReadStart");
    return PK_OK;
}

int32_t PK_1WireReadStatusGet(sPoKeysDevice* device, uint8_t * readStatus, uint8_t * ReadCount, uint8_t * data) {
    logFunctionCall("PK_1WireReadStatusGet");
    *readStatus = PK_I2C_STAT_OK;
    *ReadCount = ReadCount;
    std::memset(data, 0, ReadCount);
    return PK_OK;
}

int32_t PK_1WireWriteReadStartEx(sPoKeysDevice* device, uint8_t pinID, uint8_t WriteCount, uint8_t ReadCount, uint8_t * data) {
    logFunctionCall("PK_1WireWriteReadStartEx");
    return PK_OK;
}

int32_t PK_1WireBusScanStart(sPoKeysDevice* device, uint8_t pinID) {
    logFunctionCall("PK_1WireBusScanStart");
    return PK_OK;
}

int32_t PK_1WireBusScanGetResults(sPoKeysDevice* device, uint8_t * operationStatus, uint8_t * scanResult, uint8_t * deviceROM) {
    logFunctionCall("PK_1WireBusScanGetResults");
    *operationStatus = PK_I2C_STAT_OK;
    *scanResult = 1;
    std::memset(deviceROM, 0, 8);
    return PK_OK;
}

int32_t PK_1WireBusScanContinue(sPoKeysDevice* device) {
    logFunctionCall("PK_1WireBusScanContinue");
    return PK_OK;
}

int32_t PK_1WireBusScanStop(sPoKeysDevice* device) {
    logFunctionCall("PK_1WireBusScanStop");
    return PK_OK;
}

int32_t PK_EasySensorsSetupGet(sPoKeysDevice* device) {
    logFunctionCall("PK_EasySensorsSetupGet");
    return PK_OK;
}

int32_t PK_EasySensorsSetupSet(sPoKeysDevice* device) {
    logFunctionCall("PK_EasySensorsSetupSet");
    return PK_OK;
}

int32_t PK_EasySensorsValueGetAll(sPoKeysDevice* device) {
    logFunctionCall("PK_EasySensorsValueGetAll");
    return PK_OK;
}

int32_t PK_FailsafeSettingsGet(sPoKeysDevice* device) {
    logFunctionCall("PK_FailsafeSettingsGet");
    return PK_OK;
}

int32_t PK_FailsafeSettingsSet(sPoKeysDevice* device) {
    logFunctionCall("PK_FailsafeSettingsSet");
    return PK_OK;
}

int32_t PK_SPIConfigure(sPoKeysDevice * device, uint8_t prescaler, uint8_t frameFormat) {
    logFunctionCall("PK_SPIConfigure");
    return PK_OK;
}

int32_t PK_SPIWrite(sPoKeysDevice * device, uint8_t * buffer, uint8_t iDataLength, uint8_t pinCS) {
    logFunctionCall("PK_SPIWrite");
    return PK_OK;
}

int32_t PK_SPIRead(sPoKeysDevice * device, uint8_t * buffer, uint8_t iDataLength) {
    logFunctionCall("PK_SPIRead");
    return PK_OK;
}

int32_t PK_PoILGetState(sPoKeysDevice* device) {
    logFunctionCall("PK_PoILGetState");
    return PK_OK;
}

int32_t PK_PoILSetCoreState(sPoKeysDevice* device, uint16_t state) {
    logFunctionCall("PK_PoILSetCoreState");
    return PK_OK;
}

int32_t PK_PoILSetMasterEnable(sPoKeysDevice* device, uint8_t masterEnable) {
    logFunctionCall("PK_PoILSetMasterEnable");
    return PK_OK;
}

int32_t PK_PoILResetCore(sPoKeysDevice* device) {
    logFunctionCall("PK_PoILResetCore");
    return PK_OK;
}

int32_t PK_PoILSetDebugMode(sPoKeysDevice* device, uint8_t debugMode, uint16_t breakpoint) {
    logFunctionCall("PK_PoILSetDebugMode");
    return PK_OK;
}

int32_t PK_PoILReadMemory(sPoKeysDevice* device, uint8_t memoryType, uint16_t address, uint16_t size, uint8_t * dest) {
    logFunctionCall("PK_PoILReadMemory");
    std::memset(dest, 0, size);
    return PK_OK;
}

int32_t PK_PoILWriteMemory(sPoKeysDevice* device, uint8_t memoryType, uint16_t address, uint16_t size, uint8_t * src) {
    logFunctionCall("PK_PoILWriteMemory");
    return PK_OK;
}

int32_t PK_PoILEraseMemory(sPoKeysDevice* device, uint8_t memoryType) {
    logFunctionCall("PK_PoILEraseMemory");
    return PK_OK;
}

int32_t PK_PoILChunkReadMemory(sPoKeysDevice * device, uint8_t * dest) {
    logFunctionCall("PK_PoILChunkReadMemory");
    std::memset(dest, 0, 256);
    return PK_OK;
}

int32_t PK_PoILChunkReadMemoryInternalAddress(sPoKeysDevice * device, uint8_t * dest) {
    logFunctionCall("PK_PoILChunkReadMemoryInternalAddress");
    std::memset(dest, 0, 256);
    return PK_OK;
}

int32_t PK_PoILReadSharedSlot(sPoKeysDevice* device, uint16_t firstSlotID, uint16_t slotsNum, int32_t * dest) {
    logFunctionCall("PK_PoILReadSharedSlot");
    std::memset(dest, 0, slotsNum * sizeof(int32_t));
    return PK_OK;
}

int32_t PK_PoILWriteSharedSlot(sPoKeysDevice* device, uint16_t firstSlotID, uint16_t slotsNum, int32_t * src) {
    logFunctionCall("PK_PoILWriteSharedSlot");
    return PK_OK;
}

int32_t PK_PoILTaskStatus(sPoKeysDevice * device) {
    logFunctionCall("PK_PoILTaskStatus");
    return PK_OK;
}

int32_t PK_RTCGet(sPoKeysDevice* device) {
    logFunctionCall("PK_RTCGet");
    return PK_OK;
}

int32_t PK_RTCSet(sPoKeysDevice* device) {
    logFunctionCall("PK_RTCSet");
    return PK_OK;
}

int32_t PK_UARTConfigure(sPoKeysDevice* device, uint32_t baudrate, uint8_t format, uint8_t interfaceID) {
    logFunctionCall("PK_UARTConfigure");
    return PK_OK;
}

int32_t PK_UARTWrite(sPoKeysDevice* device, uint8_t interfaceID, uint8_t *dataPtr, uint32_t dataWriteLen) {
    logFunctionCall("PK_UARTWrite");
    return PK_OK;
}

int32_t PK_UARTRead(sPoKeysDevice* device, uint8_t interfaceID, uint8_t *dataPtr, uint8_t *dataReadLen) {
    logFunctionCall("PK_UARTRead");
    std::memset(dataPtr, 0, *dataReadLen);
    return PK_OK;
}

int32_t PK_CANConfigure(sPoKeysDevice* device, uint32_t bitrate) {
    logFunctionCall("PK_CANConfigure");
    return PK_OK;
}

int32_t PK_CANRegisterFilter(sPoKeysDevice* device, uint8_t format, uint32_t CANid) {
    logFunctionCall("PK_CANRegisterFilter");
    return PK_OK;
}

int32_t PK_CANWrite(sPoKeysDevice* device, sPoKeysCANmsg * msg) {
    logFunctionCall("PK_CANWrite");
    return PK_OK;
}

int32_t PK_CANRead(sPoKeysDevice* device, sPoKeysCANmsg * msg, uint8_t * status) {
    logFunctionCall("PK_CANRead");
    std::memset(msg, 0, sizeof(sPoKeysCANmsg));
    *status = PK_I2C_STAT_OK;
    return PK_OK;
}

int32_t PK_WS2812_Update(sPoKeysDevice* device, uint16_t LEDcount, uint8_t updateFlag) {
    logFunctionCall("PK_WS2812_Update");
    return PK_OK;
}

int32_t PK_WS2812_SendLEDdata(sPoKeysDevice* device, uint32_t * LEDdata, uint16_t startLED, uint8_t LEDcount) {
    logFunctionCall("PK_WS2812_SendLEDdata");
    return PK_OK;
}

void PK_SL_SetPinFunction(sPoKeysDevice* device, uint8_t pin, uint8_t function) {
    logFunctionCall("PK_SL_SetPinFunction");
}

uint8_t PK_SL_GetPinFunction(sPoKeysDevice* device, uint8_t pin) {
    logFunctionCall("PK_SL_GetPinFunction");
    return 0;
}

void PK_SL_DigitalOutputSet(sPoKeysDevice* device, uint8_t pin, uint8_t value) {
    logFunctionCall("PK_SL_DigitalOutputSet");
    mockDeviceState.digitalPins[pin] = value;
}

uint8_t PK_SL_DigitalInputGet(sPoKeysDevice* device, uint8_t pin) {
    logFunctionCall("PK_SL_DigitalInputGet");
    return mockDeviceState.digitalPins[pin];
}

uint32_t PK_SL_AnalogInputGet(sPoKeysDevice* device, uint8_t pin) {
    logFunctionCall("PK_SL_AnalogInputGet");
    return mockDeviceState.analogPins[pin];
}

uint32_t PK_SL_EncoderValueGet(sPoKeysDevice* device, uint8_t index) {
    logFunctionCall("PK_SL_EncoderValueGet");
    return 0;
}

int32_t PK_SL_EasySensorValueGet(sPoKeysDevice* device, uint8_t index) {
    logFunctionCall("PK_SL_EasySensorValueGet");
    return 0;
}

uint32_t PK_SL_PWMConfig(sPoKeysDevice* device, uint8_t index) {
    logFunctionCall("PK_SL_PWMConfig");
    return 0;
}

int32_t PK_SL_PWM_SetPeriod(sPoKeysDevice* device, uint32_t PWMperiod) {
    logFunctionCall("PK_SL_PWM_SetPeriod");
    return PK_OK;
}

int32_t PK_SL_PWM_SetChannelEnabled(sPoKeysDevice* device, uint8_t channel, uint8_t enabled, uint32_t defaultDuty) {
    logFunctionCall("PK_SL_PWM_SetChannelEnabled");
    return PK_OK;
}

int32_t PK_SL_PWM_SetDuty(sPoKeysDevice* device, uint8_t channel, uint32_t duty) {
    logFunctionCall("PK_SL_PWM_SetDuty");
    return PK_OK;
}

} // extern "C"
