#include <gtest/gtest.h>
#include "PoKeysLib.h"

class MockPoKeysTest : public ::testing::Test {
protected:
    sPoKeysDevice* device;

    void SetUp() override {
        device = PK_ConnectToDevice(0);
    }

    void TearDown() override {
        PK_DisconnectDevice(device);
    }
};

TEST_F(MockPoKeysTest, TestDigitalIO) {
    uint8_t pinValue = 0;

    PK_DigitalIOSetSingle(device, 0, 1);
    PK_DigitalIOGetSingle(device, 0, &pinValue);
    ASSERT_EQ(pinValue, 1);

    PK_DigitalIOSetSingle(device, 0, 0);
    PK_DigitalIOGetSingle(device, 0, &pinValue);
    ASSERT_EQ(pinValue, 0);
}

TEST_F(MockPoKeysTest, TestAnalogIO) {
    uint32_t buffer[7] = {0};

    PK_AnalogIOGetAsArray(device, buffer);
    for (int i = 0; i < 7; ++i) {
        ASSERT_EQ(buffer[i], 0);
    }
}

TEST_F(MockPoKeysTest, TestCounter) {
    uint8_t pinID = 0;
    ASSERT_EQ(PK_IsCounterAvailable(device, pinID), 0);

    mockDeviceState.counters[pinID] = 1;
    ASSERT_EQ(PK_IsCounterAvailable(device, pinID), 1);
}

TEST_F(MockPoKeysTest, TestPulseEngine) {
    ASSERT_EQ(PK_PEv2_PulseEngineMove(device), PK_OK);
    ASSERT_EQ(PK_PEv2_StatusGet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_Status2Get(device), PK_OK);
    ASSERT_EQ(PK_PEv2_PulseEngineSetup(device), PK_OK);
    ASSERT_EQ(PK_PEv2_AdditionalParametersGet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_AdditionalParametersSet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_AxisConfigurationGet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_AxisConfigurationSet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_PositionSet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_PulseEngineStateSet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ExternalOutputsGet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ExternalOutputsSet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_BufferFill(device), PK_OK);
    ASSERT_EQ(PK_PEv2_BufferFill_16(device), PK_OK);
    ASSERT_EQ(PK_PEv2_BufferFillLarge(device), PK_OK);
    ASSERT_EQ(PK_PEv2_BufferFillLarge_16(device), PK_OK);
    ASSERT_EQ(PK_PEv2_BufferClear(device), PK_OK);
    ASSERT_EQ(PK_PEv2_PulseEngineReboot(device), PK_OK);
    ASSERT_EQ(PK_PEv2_HomingStart(device), PK_OK);
    ASSERT_EQ(PK_PEv2_HomingFinish(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ProbingStart(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ProbingHybridStart(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ProbingFinish(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ProbingFinishSimple(device), PK_OK);
    ASSERT_EQ(PK_PEv2_SyncedPWMSetup(device, 1, 0, 0), PK_OK);
    ASSERT_EQ(PK_PEv2_ThreadingPrepareForTrigger(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ThreadingForceTriggerReady(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ThreadingTrigger(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ThreadingRelease(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ThreadingStatusGet(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ThreadingCancel(device), PK_OK);
    ASSERT_EQ(PK_PEv2_ThreadingSetup(device, 0, 0, 0, 0, 0), PK_OK);
    ASSERT_EQ(PK_PEv2_BacklashCompensationSettings_Get(device), PK_OK);
    ASSERT_EQ(PK_PEv2_BacklashCompensationSettings_Set(device), PK_OK);
}
