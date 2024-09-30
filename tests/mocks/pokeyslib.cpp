#include "pokeyslib.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstring>

PoKeysLibMock pokeyslib;

void PoKeysLibMock::PK_SL_SetPinFunction(sPoKeysDevice* device, uint8_t pin, uint8_t function) {
    // Mock implementation
}

uint8_t PoKeysLibMock::PK_SL_DigitalInputGet(sPoKeysDevice* device, uint8_t pin) {
    return digital_inputs[pin];
}

void PoKeysLibMock::PK_SL_DigitalOutputSet(sPoKeysDevice* device, uint8_t pin, uint8_t value) {
    digital_outputs[pin] = value;
}

uint32_t PoKeysLibMock::PK_SL_AnalogInputGet(sPoKeysDevice* device, uint8_t pin) {
    return analog_inputs[pin];
}

void PoKeysLibMock::PK_SL_AnalogOutputSet(sPoKeysDevice* device, uint8_t pin, uint32_t value) {
    analog_outputs[pin] = value;
}

int32_t PoKeysLibMock::PK_IsCounterAvailable(sPoKeysDevice* device, uint8_t pin) {
    return counters.count(pin) > 0;
}

int32_t PoKeysLibMock::PK_DigitalCounterGet(sPoKeysDevice* device) {
    // Mock implementation
    return PK_OK;
}

void PoKeysLibMock::PK_DigitalCounterClear(sPoKeysDevice* device) {
    for (auto& counter : counters) {
        counter.second = 0;
    }
}

void PoKeysLibMock::PK_PWM_Setup(sPoKeysDevice* device, uint8_t channel, uint32_t frequency, uint32_t duty_cycle) {
    pwm_channels[channel] = {frequency, duty_cycle};
}

std::pair<uint32_t, uint32_t> PoKeysLibMock::PK_PWM_Fetch(sPoKeysDevice* device, uint8_t channel) {
    return pwm_channels[channel];
}

void PoKeysLibMock::PK_PWM_Set(sPoKeysDevice* device, uint8_t channel, uint32_t frequency, uint32_t duty_cycle) {
    pwm_channels[channel] = {frequency, duty_cycle};
}

void PoKeysLibMock::PK_PEv2_AxisConfigurationSet(sPoKeysDevice* device, uint8_t axis, const sPoKeysPEv2& parameters) {
    // Mock implementation
}

int32_t PoKeysLibMock::PK_PEv2_StatusGet(sPoKeysDevice* device) {
    // Mock implementation
    return PK_OK;
}

float PoKeysLibMock::PK_PEv2_GetPosition(sPoKeysDevice* device, uint8_t axis) {
    return position_feedback[axis];
}

void PoKeysLibMock::PK_PEv2_SetPosition(sPoKeysDevice* device, uint8_t axis, float position) {
    position_feedback[axis] = position;
}

void PoKeysLibMock::PK_PEv2_SetVelocity(sPoKeysDevice* device, uint8_t axis, float velocity) {
    velocity_commands[axis] = velocity;
}

void PoKeysLibMock::PK_PEv2_StartHoming(sPoKeysDevice* device, uint8_t axis) {
    homing_status[axis] = PK_PEAxisState_axHOMINGSTART;
}

void PoKeysLibMock::PK_PEv2_CancelHoming(sPoKeysDevice* device, uint8_t axis) {
    homing_status[axis] = PK_PEAxisState_axHOME;
}

uint8_t PoKeysLibMock::PK_PEv2_GetHomingStatus(sPoKeysDevice* device, uint8_t axis) {
    return homing_status[axis];
}

void PoKeysLibMock::PK_PoNETGetModuleSettings(sPoKeysDevice* device, uint8_t module_id) {
    // Mock implementation
}

void PoKeysLibMock::PK_PoNETGetModuleStatusRequest(sPoKeysDevice* device, uint8_t module_id) {
    // Mock implementation
}

const char* PoKeysLibMock::PK_PoNETGetModuleStatus(sPoKeysDevice* device, uint8_t module_id) {
    return "module_status";
}

void PoKeysLibMock::PK_PoNETSetModuleStatus(sPoKeysDevice* device, uint8_t module_id, const char* data) {
    // Mock implementation
}
