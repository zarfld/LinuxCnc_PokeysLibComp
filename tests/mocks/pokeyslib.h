#ifndef MOCKED_POKEYSLIB_H
#define MOCKED_POKEYSLIB_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <cstring>

// Mocked PoKeysLib structures and enums

enum ePK_PinCap {
    PK_PinCap_pinRestricted = 0,
    PK_PinCap_reserved = 1,
    PK_PinCap_digitalInput = 2,
    PK_PinCap_digitalOutput = 4,
    PK_PinCap_analogInput = 8,
    PK_PinCap_analogOutput = 16,
    PK_PinCap_triggeredInput = 32,
    PK_PinCap_digitalCounter = 64,
    PK_PinCap_invertPin = 128
};

typedef enum {
    PK_AllPinCap_digitalInput = 1,
    PK_AllPinCap_digitalOutput,
    PK_AllPinCap_analogInput,
    PK_AllPinCap_MFanalogInput,
    PK_AllPinCap_analogOutput,
    PK_AllPinCap_keyboardMapping,
    PK_AllPinCap_triggeredInput,
    PK_AllPinCap_digitalCounter,
    PK_AllPinCap_PWMOut,
    PK_AllPinCap_fastEncoder1A,
    PK_AllPinCap_fastEncoder1B,
    PK_AllPinCap_fastEncoder1I,
    PK_AllPinCap_fastEncoder2A,
    PK_AllPinCap_fastEncoder2B,
    PK_AllPinCap_fastEncoder2I,
    PK_AllPinCap_fastEncoder3A,
    PK_AllPinCap_fastEncoder3B,
    PK_AllPinCap_fastEncoder3I,
    PK_AllPinCap_ultraFastEncoderA,
    PK_AllPinCap_ultraFastEncoderB,
    PK_AllPinCap_ultraFastEncoderI,
    PK_AllPinCap_LCD_E,
    PK_AllPinCap_LCD_RW,
    PK_AllPinCap_LCD_RS,
    PK_AllPinCap_LCD_D4,
    PK_AllPinCap_LCD_D5,
    PK_AllPinCap_LCD_D6,
    PK_AllPinCap_LCD_D7
} ePK_AllPinCap;

typedef enum {
    PK_DeviceMask_Bootloader = (1 << 0),
    PK_DeviceMask_Bootloader55 = (1 << 1),
    PK_DeviceMask_Bootloader56 = (1 << 2),
    PK_DeviceMask_Bootloader56U = (1 << 3),
    PK_DeviceMask_Bootloader56E = (1 << 4),
    PK_DeviceMask_Bootloader58 = (1 << 5),
    PK_DeviceMask_55 = (1 << 10),
    PK_DeviceMask_55v1 = (1 << 11),
    PK_DeviceMask_55v2 = (1 << 12),
    PK_DeviceMask_55v3 = (1 << 13),
    PK_DeviceMask_56 = (1 << 14),
    PK_DeviceMask_56U = (1 << 15),
    PK_DeviceMask_56E = (1 << 16),
    PK_DeviceMask_27 = (1 << 17),
    PK_DeviceMask_27U = (1 << 18),
    PK_DeviceMask_27E = (1 << 19),
    PK_DeviceMask_57 = (1 << 20),
    PK_DeviceMask_57U = (1 << 24),
    PK_DeviceMask_57E = (1 << 25),
    PK_DeviceMask_57CNC = (1 << 26),
    PK_DeviceMask_57CNCdb25 = (1 << 27),
    PK_DeviceMask_57Utest = (1 << 28),
    PK_DeviceMask_58 = (1 << 21),
    PK_DeviceMask_PoPLC58 = (1 << 22),
    PK_DeviceMask_PoKeys16RF = (1 << 23)
} ePK_DeviceTypeMask;

typedef enum {
    PK_DeviceID_Bootloader55 = 3,
    PK_DeviceID_Bootloader56U = 15,
    PK_DeviceID_Bootloader56E = 16,
    PK_DeviceID_Bootloader58 = 41,
    PK_DeviceID_55v1 = 0,
    PK_DeviceID_55v2 = 1,
    PK_DeviceID_55v3 = 2,
    PK_DeviceID_56U = 10,
    PK_DeviceID_56E = 11,
    PK_DeviceID_27U = 20,
    PK_DeviceID_27E = 21,
    PK_DeviceID_57U = 30,
    PK_DeviceID_57E = 31,
    PK_DeviceID_PoKeys57CNC = 32,
    PK_DeviceID_PoKeys57CNCdb25 = 38,
    PK_DeviceID_PoKeys57Utest = 39,
    PK_DeviceID_57U_v0 = 28,
    PK_DeviceID_57E_v0 = 29,
    PK_DeviceID_58EU = 40,
    PK_DeviceID_PoPLC58 = 50,
    PK_DeviceID_OEM1 = 100,
    PK_DeviceID_SerialReader = 101,
    PK_DeviceID_X15_02_24 = 102
} ePK_DeviceTypeID;

typedef struct {
    int32_t cap;
    uint32_t pinStart;
    uint32_t pinEnd;
    uint32_t additionalCheck;
    uint32_t devTypes;
} sPoKeys_PinCapabilities;

enum ePK_DeviceConnectionType {
    PK_DeviceType_USBDevice = 0,
    PK_DeviceType_NetworkDevice = 1,
    PK_DeviceType_FastUSBDevice = 2
};

enum ePK_DeviceConnectionParam {
    PK_ConnectionParam_TCP = 0,
    PK_ConnectionParam_UDP = 1
};

enum ePK_PEState {
    PK_PEState_peSTOPPED = 0,
    PK_PEState_peINTERNAL = 1,
    PK_PEState_peBUFFER = 2,
    PK_PEState_peRUNNING = 3,
    PK_PEState_peJOGGING = 10,
    PK_PEState_peSTOPPING = 11,
    PK_PEState_peHOME = 20,
    PK_PEState_peHOMING = 21,
    PK_PEState_pePROBECOMPLETE = 30,
    PK_PEState_pePROBE = 31,
    PK_PEState_pePROBEERROR = 32,
    PK_PEState_peHYBRIDPROBE_STOPPING = 40,
    PK_PEState_peHYBRIDPROBE_COMPLETE = 41,
    PK_PEState_peSTOP_LIMIT = 100,
    PK_PEState_peSTOP_EMERGENCY = 101
};

enum ePK_PEAxisState {
    PK_PEAxisState_axSTOPPED = 0,
    PK_PEAxisState_axREADY = 1,
    PK_PEAxisState_axRUNNING = 2,
    PK_PEAxisState_axHOMING_RESETTING = 8,
    PK_PEAxisState_axHOMING_BACKING_OFF = 9,
    PK_PEAxisState_axHOME = 10,
    PK_PEAxisState_axHOMINGSTART = 11,
    PK_PEAxisState_axHOMINGSEARCH = 12,
    PK_PEAxisState_axHOMINGBACK = 13,
    PK_PEAxisState_axPROBED = 14,
    PK_PEAxisState_axPROBESTART = 15,
    PK_PEAxisState_axPROBESEARCH = 16,
    PK_PEAxisState_axERROR = 20,
    PK_PEAxisState_axLIMIT = 30
};

enum ePK_PEv2_AxisConfig {
    PK_AC_ENABLED = (1 << 0),
    PK_AC_INVERTED = (1 << 1),
    PK_AC_INTERNAL_PLANNER = (1 << 2),
    PK_AC_POSITION_MODE = (1 << 3),
    PK_AC_INVERTED_HOME = (1 << 4),
    PK_AC_SOFT_LIMIT_ENABLED = (1 << 5),
    PK_AC_ENABLED_MASKED = (1 << 7)
};

enum ePK_PEv2_AxisSwitchOptions {
    PK_ASO_SWITCH_LIMIT_N = (1 << 0),
    PK_ASO_SWITCH_LIMIT_P = (1 << 1),
    PK_ASO_SWITCH_HOME = (1 << 2),
    PK_ASO_SWITCH_COMBINED_LN_H = (1 << 3),
    PK_ASO_SWITCH_COMBINED_LP_H = (1 << 4),
    PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5),
    PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6),
    PK_ASO_SWITCH_INVERT_HOME = (1 << 7)
};

enum ePK_RETURN_CODES {
    PK_OK = 0,
    PK_ERR_GENERIC = -1,
    PK_ERR_NOT_CONNECTED = -5,
    PK_ERR_TRANSFER = -10,
    PK_ERR_PARAMETER = -20,
    PK_ERR_NOT_SUPPORTED = -30,
    PK_ERR_CANNOT_CLAIM_USB = -100,
    PK_ERR_CANNOT_CONNECT = -101
};

enum ePK_I2C_STATUS {
    PK_I2C_STAT_ERR = 0,
    PK_I2C_STAT_OK = 1,
    PK_I2C_STAT_COMPLETE = 1,
    PK_I2C_STAT_IN_PROGRESS = 0x10
};

enum ePK_LCD_MODE { PK_LCD_MODE_DIRECT = 0, PK_LCD_MODE_BUFFERED = 1 };

typedef struct {
    uint32_t iPinCount;
    uint32_t iPWMCount;
    uint32_t iBasicEncoderCount;
    uint32_t iEncodersCount;
    uint32_t iFastEncoders;
    uint32_t iUltraFastEncoders;
    uint32_t PWMinternalFrequency;
    uint32_t iAnalogInputs;
    uint32_t iKeyMapping;
    uint32_t iTriggeredKeyMapping;
    uint32_t iKeyRepeatDelay;
    uint32_t iDigitalCounters;
    uint32_t iJoystickButtonAxisMapping;
    uint32_t iJoystickAnalogToDigitalMapping;
    uint32_t iMacros;
    uint32_t iMatrixKeyboard;
    uint32_t iMatrixKeyboardTriggeredMapping;
    uint32_t iLCD;
    uint32_t iMatrixLED;
    uint32_t iConnectionSignal;
    uint32_t iPoExtBus;
    uint32_t iPoNET;
    uint32_t iAnalogFiltering;
    uint32_t iInitOutputsStart;
    uint32_t iprotI2C;
    uint32_t iprot1wire;
    uint32_t iAdditionalOptions;
    uint32_t iLoadStatus;
    uint32_t iCustomDeviceName;
    uint32_t iPoTLog27support;
    uint32_t iSensorList;
    uint32_t iWebInterface;
    uint32_t iFailSafeSettings;
    uint32_t iJoystickHATswitch;
    uint32_t iPulseEngine;
    uint32_t iPulseEnginev2;
    uint32_t iEasySensors;
    uint32_t ikbd48CNC;
    uint32_t reserved[3];
} sPoKeysDevice_Info;

typedef struct {
    uint8_t nrOfAxes;
    uint8_t maxPulseFrequency;
    uint8_t bufferDepth;
    uint8_t slotTiming;
    uint8_t reserved[4];
} sPoKeysPEv2info;

typedef struct {
    sPoKeysPEv2info info;
    uint8_t AxesState[8];
    uint8_t AxesConfig[8];
    uint8_t AxesSwitchConfig[8];
    int32_t CurrentPosition[8];
    int32_t PositionSetup[8];
    int32_t ReferencePositionSpeed[8];
    int8_t InvertAxisEnable[8];
    int32_t SoftLimitMaximum[8];
    int32_t SoftLimitMinimum[8];
    uint8_t HomingSpeed[8];
    uint8_t HomingReturnSpeed[8];
    int32_t HomeOffsets[8];
    uint8_t HomingAlgorithm[8];
    uint8_t FilterLimitMSwitch[8];
    uint8_t FilterLimitPSwitch[8];
    uint8_t FilterHomeSwitch[8];
    int32_t ProbePosition[8];
    int32_t ProbeMaxPosition[8];
    float MaxSpeed[8];
    float MaxAcceleration[8];
    float MaxDecceleration[8];
    int32_t MPGjogMultiplier[8];
    uint8_t MPGjogEncoder[8];
    uint8_t PinHomeSwitch[8];
    uint8_t PinLimitMSwitch[8];
    uint8_t PinLimitPSwitch[8];
    uint8_t AxisEnableOutputPins[8];
    uint32_t HomeBackOffDistance[8];
    uint16_t MPGjogDivider[8];
    uint8_t reserved[8];
    uint8_t ReservedSafety[8];
    uint8_t PulseEngineEnabled;
    uint8_t PulseGeneratorType;
    uint8_t ChargePumpEnabled;
    uint8_t EmergencySwitchPolarity;
    uint8_t PulseEngineActivated;
    uint8_t LimitStatusP;
    uint8_t LimitStatusN;
    uint8_t HomeStatus;
    uint8_t ErrorInputStatus;
    uint8_t MiscInputStatus;
    uint8_t LimitOverride;
    uint8_t LimitOverrideSetup;
    uint8_t PulseEngineState;
    uint8_t AxisEnabledMask;
    uint8_t EmergencyInputPin;
    uint8_t reserved2;
    uint8_t param1;
    uint8_t param2;
    uint8_t param3;
    uint8_t AxisEnabledStatesMask;
    uint8_t PulseEngineStateSetup;
    uint8_t SoftLimitStatus;
    uint8_t ExternalRelayOutputs;
    uint8_t ExternalOCOutputs;
    uint8_t PulseEngineBufferSize;
    uint8_t motionBufferEntriesAccepted;
    uint8_t newMotionBufferEntries;
    uint8_t HomingStartMaskSetup;
    uint8_t ProbeStartMaskSetup;
    uint8_t ProbeInput;
    uint8_t ProbeInputPolarity;
    uint8_t ProbeStatus;
    uint8_t MotionBuffer[448];
    float ProbeSpeed;
    float reservedf;
    uint16_t BacklashWidth[8];
    int16_t BacklashRegister[8];
    uint8_t BacklashAcceleration[8];
    uint8_t BacklashCompensationEnabled;
    uint8_t reserved_back[3];
    uint8_t TriggerPreparing;
    uint8_t TriggerPrepared;
    uint8_t TriggerPending;
    uint8_t TriggerActive;
    int32_t SpindleSpeedEstimate;
    int32_t SpindlePositionError;
    uint32_t SpindleRPM;
    uint8_t DedicatedLimitNInputs;
    uint8_t DedicatedLimitPInputs;
    uint8_t DedicatedHomeInputs;
    uint8_t TriggerIngnoredAxisMask;
} sPoKeysPEv2;

typedef struct {
    uint8_t SupplyVoltage;
    uint8_t Temperature;
    uint8_t InputStatus;
    uint8_t DriverStatus;
    uint8_t FaultStatus;
    uint8_t UpdateState;
    uint8_t DriverMode;
    uint8_t StepMode;
    uint16_t Current_FS;
    uint16_t Current_Idle;
    uint16_t Current_Overheat;
    uint8_t TemperatureLimit;
    uint8_t AddressI2C;
    uint8_t DriverType;
    uint8_t UpdateConfig;
    uint8_t reserved[6];
} sPoPoStepDriverConfig;

typedef struct {
    sPoPoStepDriverConfig drivers[8];
    uint8_t EnablePoStepCommunication;
    uint8_t reserved[7];
} sPoKeysPoStepInterface;

typedef struct {
    uint32_t DeviceTypeID;
    uint32_t SerialNumber;
    uint8_t DeviceName[30];
    uint8_t DeviceTypeName[30];
    uint8_t BuildDate[12];
    uint8_t ActivationCode[8];
    uint8_t FirmwareVersionMajor;
    uint8_t FirmwareVersionMinor;
    uint8_t UserID;
    uint8_t DeviceType;
    uint8_t ActivatedOptions;
    uint8_t DeviceLockStatus;
    uint8_t HWtype;
    uint8_t FWtype;
    uint8_t ProductID;
    uint8_t SecondaryFirmwareVersionMajor;
    uint8_t SecondaryFirmwareVersionMinor;
    uint8_t deviceIsBootloader;
    uint8_t reserved[4];
} sPoKeysDevice_Data;

typedef struct {
    uint32_t DigitalCounterValue;
    uint32_t AnalogValue;
    uint8_t PinFunction;
    uint8_t CounterOptions;
    uint8_t DigitalValueGet;
    uint8_t DigitalValueSet;
    uint8_t DigitalCounterAvailable;
    uint8_t MappingType;
    uint8_t KeyCodeMacroID;
    uint8_t KeyModifier;
    uint8_t downKeyCodeMacroID;
    uint8_t downKeyModifier;
    uint8_t upKeyCodeMacroID;
    uint8_t upKeyModifier;
    uint8_t preventUpdate;
    uint8_t reserved[3];
} sPoKeysPinData;

typedef struct {
    int32_t encoderValue;
    uint8_t encoderOptions;
    uint8_t channelApin;
    uint8_t channelBpin;
    uint8_t dirAkeyCode;
    uint8_t dirAkeyModifier;
    uint8_t dirBkeyCode;
    uint8_t dirBkeyModifier;
    uint8_t reserved[5];
} sPoKeysEncoder;

typedef struct {
    uint32_t PWMperiod;
    uint32_t reserved;
    uint32_t *PWMduty;
    uint8_t *PWMenabledChannels;
    uint8_t *PWMpinIDs;
} sPoKeysPWM;

typedef struct {
    uint8_t matrixKBconfiguration;
    uint8_t matrixKBwidth;
    uint8_t matrixKBheight;
    uint8_t reserved[5];
    uint8_t matrixKBcolumnsPins[8];
    uint8_t matrixKBrowsPins[16];
    uint8_t macroMappingOptions[128];
    uint8_t keyMappingKeyCode[128];
    uint8_t keyMappingKeyModifier[128];
    uint8_t keyMappingTriggeredKey[128];
    uint8_t keyMappingKeyCodeUp[128];
    uint8_t keyMappingKeyModifierUp[128];
    uint8_t matrixKBvalues[128];
} sMatrixKeyboard;

typedef struct {
    uint8_t Configuration;
    uint8_t Rows;
    uint8_t Columns;
    uint8_t RowRefreshFlags;
    uint8_t reserved[4];
    uint8_t line1[20];
    uint8_t line2[20];
    uint8_t line3[20];
    uint8_t line4[20];
    uint8_t customCharacters[8][8];
} sPoKeysLCD;

typedef struct {
    uint8_t displayEnabled;
    uint8_t rows;
    uint8_t columns;
    uint8_t RefreshFlag;
    uint8_t data[8];
    uint8_t reserved[4];
} sPoKeysMatrixLED;

typedef struct {
    uint8_t statusIn[16];
    uint8_t statusOut[16];
    uint8_t moduleID;
    uint8_t i2cAddress;
    uint8_t moduleType;
    uint8_t moduleSize;
    uint8_t moduleOptions;
    uint8_t PWMduty;
    uint8_t lightValue;
    uint8_t PoNETstatus;
} sPoNETmodule;

typedef struct {
    uint32_t DataMemorySize;
    uint32_t CodeMemorySize;
    uint32_t Version;
    uint32_t reserved;
} sPoILinfo;

typedef struct {
    uint32_t stackPtr;
    uint32_t stackSize;
    unsigned char StackContents[1024];
} sPoILStack;

typedef struct {
    uint16_t address;
    uint8_t chunkLength;
    uint8_t reserved;
    uint32_t reserved2;
} sPoILmemoryChunk;

typedef struct {
    uint16_t taskPeriod;
    uint16_t taskRealPeriod;
    uint16_t taskRealPeriodFiltered;
    uint8_t taskStatus;
    uint8_t taskLoad;
} sPoILTask;

typedef struct {
    sPoILinfo info;
    uint32_t MasterEnable;
    uint32_t currentTask;
    uint32_t STATUS;
    uint32_t W;
    uint32_t PC;
    uint32_t ExceptionPC;
    uint32_t CoreState;
    uint32_t CoreDebugMode;
    uint32_t CoreDebugBreakpoint;
    uint32_t reserved0;
    sPoILStack functionStack;
    sPoILStack dataStack;
    uint8_t codeMemoryPage[256];
    uint8_t dataMemoryPage[256];
    sPoILmemoryChunk monitorChunks[18];
    sPoILTask tasks[32];
    uint8_t inactiveLoad;
    uint8_t taskCount;
    uint8_t reserved[2];
} sPoILStatus;

typedef struct {
    int32_t sensorValue;
    uint8_t sensorType;
    uint8_t sensorRefreshPeriod;
    uint8_t sensorFailsafeConfig;
    uint8_t sensorReadingID;
    uint8_t sensorID[8];
    uint8_t sensorOKstatus;
    uint8_t reserved[7];
} sPoKeysEasySensor;

typedef struct {
    uint8_t HTMLcode[32];
    uint8_t simpleText[8];
} sPoKeysCustomSensorUnit;

typedef struct {
    uint8_t SEC;
    uint8_t MIN;
    uint8_t HOUR;
    uint8_t DOW;
    uint8_t DOM;
    uint8_t tmp;
    uint16_t DOY;
    uint16_t MONTH;
    uint16_t YEAR;
    uint32_t reserved;
} sPoKeysRTC;

typedef struct {
    uint32_t id;
    uint8_t data[8];
    uint8_t len;
    uint8_t format;
    uint8_t type;
} sPoKeysCANmsg;

typedef struct {
    uint32_t SerialNumber;
    uint8_t IPaddress[4];
    uint8_t hostIP[4];
    uint8_t FirmwareVersionMajor;
    uint8_t FirmwareVersionMinor;
    uint8_t UserID;
    uint8_t DHCP;
    uint8_t HWtype;
    uint8_t useUDP;
} sPoKeysNetworkDeviceSummary;

typedef struct {
    uint8_t IPAddressCurrent[4];
    uint8_t IPAddressSetup[4];
    uint8_t Subnetmask[4];
    uint8_t DefaultGateway[4];
    uint16_t TCPtimeout;
    uint8_t AdditionalNetworkOptions;
    uint8_t DHCP;
} sPoKeysNetworkDeviceInfo;

typedef struct {
    uint32_t AnalogRCFilter;
} sPoKeysOtherPeripherals;

typedef struct {
    uint8_t bFailSafeEnabled;
    uint8_t bFailSafePeripherals;
    uint8_t padding1[6];
    uint8_t bFailSafeIO[7];
    uint8_t padding2[1];
    uint8_t bFailSafePoExtBus[10];
    uint8_t bFailSafePWM[6];
} sPoKeysFailsafeSettings;

typedef struct {
    void *devHandle;
    void *devHandle2;
    sPoKeysDevice_Info info;
    sPoKeysDevice_Data DeviceData;
    sPoKeysNetworkDeviceInfo *netDeviceData;
    sPoKeysPinData *Pins;
    sPoKeysEncoder *Encoders;
    sMatrixKeyboard matrixKB;
    sPoKeysPWM PWM;
    sPoKeysMatrixLED *MatrixLED;
    sPoKeysLCD LCD;
    sPoKeysPEv2 PEv2;
    sPoKeysPoStepInterface PoSteps;
    sPoNETmodule PoNETmodule;
    sPoILStatus PoIL;
    sPoKeysRTC RTC;
    sPoKeysEasySensor *EasySensors;
    sPoKeysOtherPeripherals otherPeripherals;
    sPoKeysFailsafeSettings failsafeSettings;
    uint8_t FastEncodersConfiguration;
    uint8_t FastEncodersOptions;
    uint8_t UltraFastEncoderConfiguration;
    uint8_t UltraFastEncoderOptions;
    uint32_t UltraFastEncoderFilter;
    uint8_t *PoExtBusData;
    uint8_t connectionType;
    uint8_t connectionParam;
    uint8_t requestID;
    uint8_t reserved;
    uint32_t sendRetries;
    uint32_t readRetries;
    uint32_t socketTimeout;
    uint8_t request[68];
    uint8_t response[68];
    uint8_t multiPartData[448];
    uint64_t reserved64;
    uint8_t *multiPartBuffer;
} sPoKeysDevice;

// Mocked PoKeysLib functions

class PoKeysLibMock {
  public:
    std::unordered_map<uint8_t, uint8_t> digital_inputs;
    std::unordered_map<uint8_t, uint8_t> digital_outputs;
    std::unordered_map<uint8_t, uint32_t> analog_inputs;
    std::unordered_map<uint8_t, uint32_t> analog_outputs;
    std::unordered_map<uint8_t, uint32_t> counters;
    std::unordered_map<uint8_t, std::pair<uint32_t, uint32_t>> pwm_channels;
    std::unordered_map<uint8_t, float> position_feedback;
    std::unordered_map<uint8_t, float> velocity_commands;
    std::unordered_map<uint8_t, uint8_t> homing_status;

    void PK_SL_SetPinFunction(sPoKeysDevice *device, uint8_t pin,
                              uint8_t function) {
        // Mock implementation
    }

    uint8_t PK_SL_DigitalInputGet(sPoKeysDevice *device, uint8_t pin) {
        return digital_inputs[pin];
    }

    void PK_SL_DigitalOutputSet(sPoKeysDevice *device, uint8_t pin,
                                uint8_t value) {
        digital_outputs[pin] = value;
    }

    uint32_t PK_SL_AnalogInputGet(sPoKeysDevice *device, uint8_t pin) {
        return analog_inputs[pin];
    }

    void PK_SL_AnalogOutputSet(sPoKeysDevice *device, uint8_t pin,
                               uint32_t value) {
        analog_outputs[pin] = value;
    }

    int32_t PK_IsCounterAvailable(sPoKeysDevice *device, uint8_t pin) {
        return counters.count(pin) > 0;
    }

    int32_t PK_DigitalCounterGet(sPoKeysDevice *device) {
        // Mock implementation
        return PK_OK;
    }

    void PK_DigitalCounterClear(sPoKeysDevice *device) {
        for (auto &counter : counters) {
            counter.second = 0;
        }
    }

    void PK_PWM_Setup(sPoKeysDevice *device, uint8_t channel,
                      uint32_t frequency, uint32_t duty_cycle) {
        pwm_channels[channel] = { frequency, duty_cycle };
    }

    std::pair<uint32_t, uint32_t> PK_PWM_Fetch(sPoKeysDevice *device,
                                               uint8_t channel) {
        return pwm_channels[channel];
    }

    void PK_PWM_Set(sPoKeysDevice *device, uint8_t channel, uint32_t frequency,
                    uint32_t duty_cycle) {
        pwm_channels[channel] = { frequency, duty_cycle };
    }

    void PK_PEv2_AxisConfigurationSet(sPoKeysDevice *device, uint8_t axis,
                                      const sPoKeysPEv2 &parameters) {
        // Mock implementation
    }

    int32_t PK_PEv2_StatusGet(sPoKeysDevice *device) {
        // Mock implementation
        return PK_OK;
    }

    float PK_PEv2_GetPosition(sPoKeysDevice *device, uint8_t axis) {
        return position_feedback[axis];
    }

    void PK_PEv2_SetPosition(sPoKeysDevice *device, uint8_t axis,
                             float position) {
        position_feedback[axis] = position;
    }

    void PK_PEv2_SetVelocity(sPoKeysDevice *device, uint8_t axis,
                             float velocity) {
        velocity_commands[axis] = velocity;
    }

    void PK_PEv2_StartHoming(sPoKeysDevice *device, uint8_t axis) {
        homing_status[axis] = PK_PEAxisState_axHOMINGSTART;
    }

    void PK_PEv2_CancelHoming(sPoKeysDevice *device, uint8_t axis) {
        homing_status[axis] = PK_PEAxisState_axHOME;
    }

    uint8_t PK_PEv2_GetHomingStatus(sPoKeysDevice *device, uint8_t axis) {
        return homing_status[axis];
    }

    void PK_PoNETGetModuleSettings(sPoKeysDevice *device, uint8_t module_id) {
        // Mock implementation
    }

    void PK_PoNETGetModuleStatusRequest(sPoKeysDevice *device,
                                        uint8_t module_id) {
        // Mock implementation
    }

    const char *PK_PoNETGetModuleStatus(sPoKeysDevice *device,
                                        uint8_t module_id) {
        return "module_status";
    }

    void PK_PoNETSetModuleStatus(sPoKeysDevice *device, uint8_t module_id,
                                 const char *data) {
        // Mock implementation
    }
};

extern PoKeysLibMock pokeyslib;

#endif // MOCKED_POKEYSLIB_H
