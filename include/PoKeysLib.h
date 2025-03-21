/*

   Copyright (C) 2014 Matev� Bo�nak (matevz@poscope.com)

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

#ifndef __POKEYSLIB
#define __POKEYSLIB
#define USE_STD_INT

#ifdef USE_STD_INT
#include "stdint.h"
#endif

#if !defined(_STDINT) && !defined(_STDINT_H)
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#ifdef POKEYSDLL
#ifdef POKEYSDLLEXPORT
#define POKEYSDECL __declspec(dllexport)
#else
#define POKEYSDECL __declspec(dllimport)
#endif
#else
#define POKEYSDECL
#endif

//#define USE_ALIGN_TEST

//#define POKEYSLIB_USE_LIBUSB
#ifdef POKEYSLIB_USE_LIBUSB
#include "libusb-1.0/libusb.h"
#endif

#ifdef USE_ALIGN_TEST
#define ALIGN_TEST(x) uint64_t alignTest##x;
#else
#define ALIGN_TEST(x)
#endif

#pragma warning(disable : 4996)

#ifdef __cplusplus
extern "C" {
#endif
// Pin capabilities / configuration
enum ePK_PinCap {
    PK_PinCap_pinRestricted = 0,   // Pin is not used
    PK_PinCap_reserved = 1,        // --
    PK_PinCap_digitalInput = 2,    // Digital input
    PK_PinCap_digitalOutput = 4,   // Digital output
    PK_PinCap_analogInput = 8,     // Analog input (only on selected pins)
    PK_PinCap_analogOutput = 16,   // Analog output (only on selected pins)
    PK_PinCap_triggeredInput = 32, // Triggered input
    PK_PinCap_digitalCounter = 64, // Digital counter (only on selected pins)
    PK_PinCap_invertPin =
        128 // Invert digital pin polarity (set together with digital input, output or triggered input)
};

// Pin capabilities / configuration
typedef enum {
    PK_AllPinCap_digitalInput = 1, // Digital input supported
    PK_AllPinCap_digitalOutput,    // Digital output supported
    PK_AllPinCap_analogInput,      // Analog input supported
    PK_AllPinCap_MFanalogInput,    // Analog input supported
    PK_AllPinCap_analogOutput,     // Analog output supported
    PK_AllPinCap_keyboardMapping,
    PK_AllPinCap_triggeredInput, // Triggered input supported
    PK_AllPinCap_digitalCounter, // Digital counter supported
    PK_AllPinCap_PWMOut,         // PWM output supported
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

// Connection type
enum ePK_DeviceConnectionType {
    PK_DeviceType_USBDevice = 0,
    PK_DeviceType_NetworkDevice = 1,
    PK_DeviceType_FastUSBDevice = 2
};

enum ePK_DeviceConnectionParam {
    PK_ConnectionParam_TCP = 0,
    PK_ConnectionParam_UDP = 1
};

// Pulse engine state
enum ePK_PEState {
    PK_PEState_peSTOPPED = 0, // Pulse engine is stopped
    PK_PEState_peINTERNAL =
        1, // PEv1: Internal motion controller is in use, PEv2: not used
    PK_PEState_peBUFFER =
        2, // PEv1: Buffered operation mode is in use, PEv2: not used
    PK_PEState_peRUNNING = 3, // Pulse engine is activated

    PK_PEState_peJOGGING = 10,  // Jogging mode enabled
    PK_PEState_peSTOPPING = 11, // Pulse engine is stopping

    PK_PEState_peHOME = 20,   // All axes are homed
    PK_PEState_peHOMING = 21, // Axes homing is in progress

    PK_PEState_pePROBECOMPLETE = 30, // All axes are homed
    PK_PEState_pePROBE = 31,         // Axes probing is in progress
    PK_PEState_pePROBEERROR = 32,    // Error occured during probing

    PK_PEState_peHYBRIDPROBE_STOPPING = 40,
    PK_PEState_peHYBRIDPROBE_COMPLETE = 41,

    PK_PEState_peSTOP_LIMIT = 100, // Pulse engine stopped due to limit reached
    PK_PEState_peSTOP_EMERGENCY =
        101 // Pulse engine stopped due to emergency switch
};

// Pulse engine axis state
enum ePK_PEAxisState {
    PK_PEAxisState_axSTOPPED = 0, // Axis is stopped
    PK_PEAxisState_axREADY = 1,   // Axis ready
    PK_PEAxisState_axRUNNING = 2, // Axis is running

    PK_PEAxisState_axHOMING_RESETTING =
        8, // Stopping the axis to reset the position counters
    PK_PEAxisState_axHOMING_BACKING_OFF = 9, // Backing off switch
    PK_PEAxisState_axHOME = 10,              // Axis is homed
    PK_PEAxisState_axHOMINGSTART = 11, // Homing procedure is starting on axis
    PK_PEAxisState_axHOMINGSEARCH =
        12, // Homing procedure first step - going to home
    PK_PEAxisState_axHOMINGBACK =
        13, // Homing procedure second step - slow homing

    PK_PEAxisState_axPROBED = 14,      // Probing completed for this axis
    PK_PEAxisState_axPROBESTART = 15,  // Probing procedure is starting on axis
    PK_PEAxisState_axPROBESEARCH = 16, // Probing procedure - probing

    PK_PEAxisState_axERROR = 20, // Axis error
    PK_PEAxisState_axLIMIT = 30  // Axis limit tripped
};

enum ePK_PEv2_AxisConfig {
    PK_AC_ENABLED = (1 << 0),          // Axis enabled
    PK_AC_INVERTED = (1 << 1),         // Axis inverted
    PK_AC_INTERNAL_PLANNER = (1 << 2), // Axis uses internal motion planner
    PK_AC_POSITION_MODE =
        (1 << 3), // Internal motion planner for this axis is in position mode
    PK_AC_INVERTED_HOME = (1 << 4),      // Axis homing direction is inverted
    PK_AC_SOFT_LIMIT_ENABLED = (1 << 5), // Use soft-limits for this axis
    PK_AC_ENABLED_MASKED = (1 << 7)      // Use output enable pin masking
};

enum ePK_PEv2_AxisSwitchOptions {
    PK_ASO_SWITCH_LIMIT_N = (1 << 0), // Limit- switch
    PK_ASO_SWITCH_LIMIT_P = (1 << 1), // Limit+ switch
    PK_ASO_SWITCH_HOME = (1 << 2),    // Home switch
    PK_ASO_SWITCH_COMBINED_LN_H =
        (1 << 3), // Home switch is shared with Limit- switch
    PK_ASO_SWITCH_COMBINED_LP_H =
        (1 << 4), // Home switch is shared with Limit+ switch
    PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5), // Invert limit- switch polarity
    PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6), // Invert limit+ switch polarity
    PK_ASO_SWITCH_INVERT_HOME = (1 << 7)     // Invert home switch polarity
};

// Return codes for various functions
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
    PK_I2C_STAT_ERR = 0,           // An error occured
    PK_I2C_STAT_OK = 1,            // All is OK
    PK_I2C_STAT_COMPLETE = 1,      // Operation complete
    PK_I2C_STAT_IN_PROGRESS = 0x10 // Operation still in progress
};

enum ePK_LCD_MODE { PK_LCD_MODE_DIRECT = 0, PK_LCD_MODE_BUFFERED = 1 };

// PoKeys device information
typedef struct {
    uint32_t iPinCount;            // Number of pins, physically on the device
    uint32_t iPWMCount;            // Number of pins that support PWM output
    uint32_t iBasicEncoderCount;   // Number of basic encoders
    uint32_t iEncodersCount;       // Number of encoder slots available
    uint32_t iFastEncoders;        // Number of fast encoders supported
    uint32_t iUltraFastEncoders;   // Number of available ultra fast encoders
    uint32_t PWMinternalFrequency; // Main PWM peripheral clock
    uint32_t iAnalogInputs;        // Number of available analog inputs

    uint32_t
        iKeyMapping; // Device supports key mapping (acts as a USB keyboard)
    uint32_t iTriggeredKeyMapping; // Device supports triggered key mapping
    uint32_t
        iKeyRepeatDelay; // Device supports user customizable key repeat rates and delays
    uint32_t iDigitalCounters; // Device supports digital counters
    uint32_t
        iJoystickButtonAxisMapping; // Device supports mapping of joystick buttons
    uint32_t
        iJoystickAnalogToDigitalMapping; // Device supports mapping of analog inputs to digital keys
    uint32_t iMacros;         // Device supports customizable macro sequences
    uint32_t iMatrixKeyboard; // Device supports matrix keyboard
    uint32_t
        iMatrixKeyboardTriggeredMapping; // Device supports matrix keyboard triggered key mapping
    uint32_t iLCD;              // Device supports alphanumeric LCD display
    uint32_t iMatrixLED;        // Device supports matrix LED display
    uint32_t iConnectionSignal; // Device supports connection signal output
    uint32_t iPoExtBus;         // Device supports PoExtBus digital outputs
    uint32_t iPoNET;            // Device supports PoNET bus devices
    uint32_t
        iAnalogFiltering; // Device supports analog inputs low-pass digital filtering
    uint32_t
        iInitOutputsStart; // Device supports initializing outputs at startup
    uint32_t iprotI2C;     // Device supports I2C bus (master)
    uint32_t iprot1wire;   // Device supports 1-wire bus (master)
    uint32_t
        iAdditionalOptions; // Device supports additional options with activation keys
    uint32_t iLoadStatus; // Device supports reporting load status
    uint32_t
        iCustomDeviceName;     // Device supports specifying custom device names
    uint32_t iPoTLog27support; // Device supports PoTLog27 firmware
    uint32_t iSensorList;      // Device supports sensor lists
    uint32_t iWebInterface;    // Device supports web interface
    uint32_t iFailSafeSettings;  // Device supports fail-safe mode
    uint32_t iJoystickHATswitch; // Device supports joystick HAT switch mapping
    uint32_t iPulseEngine;       // Device supports Pulse engine
    uint32_t iPulseEnginev2;     // Device supports Pulse engine v2
    uint32_t iEasySensors;       // Device supports EasySensors
    uint32_t ikbd48CNC;          // kbd48CNC was found in PoNet
    uint32_t reserved[3];
} sPoKeysDevice_Info;

// Pulse engine v2 information
typedef struct {
    uint8_t nrOfAxes;
    uint8_t maxPulseFrequency;
    uint8_t bufferDepth;
    uint8_t slotTiming;

    uint8_t reserved[4];
} sPoKeysPEv2info;

// Pulse engine v2 structure...
typedef struct {
    sPoKeysPEv2info info; // Pulse engine info

    uint8_t AxesState[8];  // Axis states (bit-mapped) - see ePK_PEAxisState
    uint8_t AxesConfig[8]; // Axis configuration - see ePK_PEv2_AxisConfig
    uint8_t AxesSwitchConfig
        [8]; // Axis switch configuration - see ePK_PEv2_AxisSwitchOptions
    int32_t CurrentPosition[8]; // Current position
    int32_t PositionSetup[8];   // Position to be set as current position
    int32_t ReferencePositionSpeed
        [8]; // Reference position or speed (position or pulses/s)
    int8_t InvertAxisEnable[8]; // Invert axis enable signal

    int32_t SoftLimitMaximum[8]; // Soft limit maximum position
    int32_t SoftLimitMinimum[8]; // Soft limit minimum position

    uint8_t HomingSpeed[8]; // Homing speed per axis (in %)
    uint8_t HomingReturnSpeed
        [8]; // Homing return speed per axis (in % of the homing speed)

    int32_t HomeOffsets[8];     // Home position offset
    uint8_t HomingAlgorithm[8]; // Homing algorithm configuration

    uint8_t FilterLimitMSwitch[8]; // Digital filter for limit- switch
    uint8_t FilterLimitPSwitch[8]; // Digital filter for limit+ switch
    uint8_t FilterHomeSwitch[8];   // Digital filter for home switch

    int32_t ProbePosition[8]; // Position where probe detected change
    int32_t ProbeMaxPosition
        [8]; // Maximum position to travel to until stopping and returning error
    float MaxSpeed[8];         // Maximum axis speed (in pulses per ms)
    float MaxAcceleration[8];  // Maximum axis acceleration (in pulses/ms/ms)
    float MaxDecceleration[8]; // Maximum axis deceleration (in pulses/ms/ms)

    int32_t MPGjogMultiplier[8]; // MPG jog multiplier value
    uint8_t MPGjogEncoder[8];    // MPG jog encoder ID
    uint8_t
        PinHomeSwitch[8]; // Home switch pin (0 for external dedicated input)
    uint8_t PinLimitMSwitch
        [8]; // Limit- switch pin (0 for external dedicated input)
    uint8_t PinLimitPSwitch
        [8]; // Limit+ switch pin (0 for external dedicated input)
    uint8_t AxisEnableOutputPins
        [8]; // Axis enabled output pin (0 for external dedicated output)
    uint32_t HomeBackOffDistance[8]; // Back-off distance after homing
    uint16_t MPGjogDivider
        [8]; // Divider for the MPG jogging (enhanced encoder resolution)
    uint8_t reserved[8]; // Motion buffer entries - moved further down...
    uint8_t ReservedSafety[8];

    // ------ 64-bit region boundary ------
    uint8_t
        PulseEngineEnabled; // Pulse engine enabled status, also number of enabled axes
    uint8_t
        PulseGeneratorType; // Pulse engine generator type (0: external, 1: internal 3ch)
    uint8_t ChargePumpEnabled; // Charge pump output enabled
    uint8_t
        EmergencySwitchPolarity; // Emergency switch polarity (set to 1 to invert)

    uint8_t PulseEngineActivated; // Pulse engine activation status
    uint8_t LimitStatusP;         // Limit+ status (bit-mapped)
    uint8_t LimitStatusN;         // Limit- status (bit-mapped)
    uint8_t HomeStatus;           // Home status (bit-mapped)

    uint8_t
        ErrorInputStatus; // Stepper motor driver error inputs status (bit-mapped)
    uint8_t MiscInputStatus;    // Miscelenous digital inputs...
    uint8_t LimitOverride;      // Limit override status
    uint8_t LimitOverrideSetup; // Limit override configuration

    // State of pulse engine - see ePoKeysPEState
    uint8_t PulseEngineState;

    uint8_t AxisEnabledMask; // Bit-mapped ouput enabled mask
    uint8_t EmergencyInputPin;
    uint8_t reserved2;

    // ------ 64-bit region boundary ------
    uint8_t param1; // Parameter 1 value
    uint8_t param2;
    uint8_t param3;

    uint8_t
        AxisEnabledStatesMask; // Bit-mapped states, where axis enabled and charge pump signals are active
    uint8_t PulseEngineStateSetup; // Pulse engine new state configuration

    uint8_t SoftLimitStatus;       // Bit-mapped soft-limit statuses per axes
    uint8_t ExternalRelayOutputs;  // External relay outputs
    uint8_t ExternalOCOutputs;     // External open-collector outputs
    uint8_t PulseEngineBufferSize; // Buffer size information...

    uint8_t motionBufferEntriesAccepted;
    uint8_t newMotionBufferEntries;

    uint8_t HomingStartMaskSetup; // Bit-mapped axes indexes to be homed
    uint8_t ProbeStartMaskSetup;  // Bit-mapped axis indexes for probing

    uint8_t
        ProbeInput; // Probe input (0:disabled, 1-8:external inputs, 9+ Pin ID-9)
    uint8_t ProbeInputPolarity; // Probe input polarity
    uint8_t ProbeStatus; // Probe status (probe completion bit-mapped status)

    // ------ 64-bit region boundary ------
    uint8_t MotionBuffer[448]; // Motion buffer

    // ------ 64-bit region boundary ------
    float ProbeSpeed; // Probe speed (ratio of the maximum speed)
    float reservedf;

    uint16_t BacklashWidth[8];       // half of real backlash width
    int16_t BacklashRegister[8];     // current value of the backlash register
    uint8_t BacklashAcceleration[8]; // in pulses per ms^2
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

// PoStep driver configuration
typedef struct {
    // Status
    uint8_t SupplyVoltage;
    uint8_t Temperature;
    uint8_t InputStatus;
    uint8_t DriverStatus;
    uint8_t FaultStatus;
    uint8_t UpdateState;

    // Settings
    uint8_t DriverMode;
    uint8_t StepMode;
    uint16_t Current_FS;
    uint16_t Current_Idle;
    uint16_t Current_Overheat;
    uint8_t TemperatureLimit;

    // Configuration
    uint8_t AddressI2C;
    uint8_t DriverType;
    uint8_t UpdateConfig;

    uint8_t reserved[6];
} sPoPoStepDriverConfig;

// PoKeys-PoStep interface
typedef struct {
    sPoPoStepDriverConfig drivers[8];
    uint8_t EnablePoStepCommunication;
    uint8_t reserved[7];
} sPoKeysPoStepInterface;

// Device-specific data of the PoKeys device
typedef struct {
    uint32_t DeviceTypeID; // ePK_DeviceTypes ID
    uint32_t SerialNumber; // Serial number of the device

    uint8_t DeviceName[30];     // Device name (generic or user-specified)
    uint8_t DeviceTypeName[30]; // Device type name
    uint8_t BuildDate[12];      // Build date string
    uint8_t ActivationCode
        [8]; // Activation code (when activating the device additional options)

    uint8_t
        FirmwareVersionMajor; // Major firmware version number v(1+[4-7]).([0-3]) - upper 4 bits plus 1 for first part, lower 4 bits for second part
    uint8_t FirmwareVersionMinor; // Minor firmware version number
    uint8_t UserID;               // Device user ID
    uint8_t DeviceType;           // Device type code

    uint8_t
        ActivatedOptions; // Additional activated options - bit 0 for Pulse engine
    uint8_t DeviceLockStatus; // Device lock status (if 1, device is locked)
    uint8_t HWtype;           // HW type reported by the device
    uint8_t FWtype;           // FW type reported by the device
    uint8_t ProductID;

    uint8_t SecondaryFirmwareVersionMajor;
    uint8_t SecondaryFirmwareVersionMinor;
    uint8_t deviceIsBootloader;
    uint8_t reserved[4];
} sPoKeysDevice_Data;

// Pin-specific data
typedef struct {
    uint32_t
        DigitalCounterValue; // Digital counter current value (on supported pins when PinFunction is set to digital counter - use PK_IsCounterAvailable to check the pin)
    uint32_t
        AnalogValue; // Analog input value (on supported pins when PinFunction is set as analog input)
    uint8_t PinFunction;     // Pin function code - see ePK_PinCap for values
    uint8_t CounterOptions;  // Digital counter settings (on supported pins)
    uint8_t DigitalValueGet; // Digital input value read
    uint8_t DigitalValueSet; // Digital output value set
    uint8_t
        DigitalCounterAvailable; // 1 if digital counter is available on this pin
    uint8_t
        MappingType; // Digital input to USB keyboard mapping type - selects between direct key mapping and mapping to macro
    uint8_t
        KeyCodeMacroID; // USB keyboard key code or macro ID (depends on MappingType)
    uint8_t KeyModifier; // USB keyboard key modifier
    uint8_t
        downKeyCodeMacroID; // USB keyboard down key code (for triggered mapping)
    uint8_t
        downKeyModifier; // USB keyboard down key modifier (for triggered mapping)
    uint8_t
        upKeyCodeMacroID; // USB keyboard up key code (for triggered mapping)
    uint8_t
        upKeyModifier; // USB keyboard up key modifier (for triggered mapping)
    uint8_t preventUpdate;
    uint8_t reserved[3];
} sPoKeysPinData;

// Encoder-specific data
typedef struct {
    int32_t encoderValue;   // Encoder current value
    uint8_t encoderOptions; // Encoder options -    bit 0: enable encoder
                            //                        bit 1: 4x sampling
                            //                        bit 2: 2x sampling
                            //                        bit 3: reserved
        //                        bit 4: direct key mapping for direction A
        //                        bit 5: mapped to macro for direction A
        //                        bit 6: direct key mapping for direction B
        //                        bit 7: mapped to macro for direction B
    uint8_t channelApin;     // Channel A encoder pin
    uint8_t channelBpin;     // Channel B encoder pin
    uint8_t dirAkeyCode;     // USB keyboard key code for direction A
    uint8_t dirAkeyModifier; // USB keyboard key modifier for direction A
    uint8_t dirBkeyCode;     // USB keyboard key code for direction B
    uint8_t dirBkeyModifier; // USB keyboard key modifier for direction B
    uint8_t reserved[5];     // placeholder
} sPoKeysEncoder;

// PWM-specific data
typedef struct {
    uint32_t PWMperiod; // PWM period, shared among all channels
    uint32_t reserved;
    uint32_t *PWMduty; // PWM duty cycles (range between 0 and PWM period)
    uint8_t *PWMenabledChannels; // List of enabled PWM channels
    uint8_t *PWMpinIDs;
} sPoKeysPWM;

// Matrix keyboard specific data
typedef struct {
    uint8_t
        matrixKBconfiguration; // Matrix keyboard configuration (set to 1 to enable matrix keyboard support)
    uint8_t matrixKBwidth;  // Matrix keyboard width (number of columns)
    uint8_t matrixKBheight; // Matrix keyboard height (number of rows)
    uint8_t reserved[5];    // placeholder
    uint8_t
        matrixKBcolumnsPins[8];   // List of matrix keyboard column connections
    uint8_t matrixKBrowsPins[16]; // List of matrix keyboard row connections
    uint8_t macroMappingOptions
        [128]; // Selects between direct key mapping and mapping to macro sequence for each key (assumes fixed width of 8 columns)
    uint8_t keyMappingKeyCode
        [128]; // USB keyboard key code for each key (assumes fixed width of 8 columns), also down key code in triggered mapping mode
    uint8_t keyMappingKeyModifier
        [128]; // USB keyboard key modifier, also down key modifier in triggered mapping mode (assumes fixed width of 8 columns)
    uint8_t keyMappingTriggeredKey
        [128]; // Selects between normal direct key mapping and triggered key mapping for each key (assumes fixed width of 8 columns)
    uint8_t keyMappingKeyCodeUp
        [128]; // USB keyboard up key code in triggered mapping mode (assumes fixed width of 8 columns)
    uint8_t keyMappingKeyModifierUp
        [128]; // USB keyboard up key modifier in triggered mapping mode (assumes fixed width of 8 columns)
    uint8_t matrixKBvalues
        [128]; // Current state of each matrix keyboard key (assumes fixed width of 8 columns)
} sMatrixKeyboard;

// LCD-specific data
typedef struct {
    uint8_t
        Configuration; // LCD configuration byte - 0: disabled, 1: enabled on primary pins, 2: enabled on secondary pins
    uint8_t Rows;      // Number of LCD module rows
    uint8_t Columns;   // Number of LCD module columns
    uint8_t
        RowRefreshFlags; // Flag for refreshing data - bit 0: row 1, bit 1: row 2, bit 2: row 3, bit 3: row 4

    uint8_t reserved[4];

    uint8_t line1[20]; // Line 1 buffer
    uint8_t line2[20]; // Line 2 buffer
    uint8_t line3[20]; // Line 3 buffer
    uint8_t line4[20]; // Line 4 buffer

    uint8_t customCharacters[8][8]; // Buffer for custom characters
} sPoKeysLCD;

// Matrix LED specific data
typedef struct {
    uint8_t
        displayEnabled; // Display enabled byte - set to 1 to enable the display
    uint8_t rows;       // Number of Matrix LED rows
    uint8_t columns;    // Number of Matrix LED columns
    uint8_t
        RefreshFlag; // Flag for refreshing data - set to 1 to refresh the display
    uint8_t data[8]; // Matrix LED buffer - one byte per row (assumes 8 columns)
    uint8_t reserved[4];
} sPoKeysMatrixLED;

// PoNET module data
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

// PoIL-related structures

// PoIL core info
typedef struct {
    uint32_t DataMemorySize;
    uint32_t CodeMemorySize;
    uint32_t Version;
    uint32_t reserved;
} sPoILinfo;

// PoIL stack info
typedef struct {
    uint32_t stackPtr;
    uint32_t stackSize;

    unsigned char StackContents[1024];
} sPoILStack;

// Monitor mode memory chunk descriptor
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

// PoIL core status
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

// EasySensor structure
typedef struct {
    int32_t sensorValue; // Current sensor value

    uint8_t sensorType;          // Type of the sensor
    uint8_t sensorRefreshPeriod; // Refresh period in 0.1s
    uint8_t
        sensorFailsafeConfig; // Failsafe configuration (bits 0-5: timeout in seconds, bit 6: invalid=0, bit 7: invalid=0x7FFFFFFF)
    uint8_t
        sensorReadingID; // Sensor reading selection (see Protocol description document for details)
    uint8_t sensorID
        [8]; // 8 byte sensor ID - see protocol specifications for details

    uint8_t sensorOKstatus; // Sensor OK status
    uint8_t reserved[7];
} sPoKeysEasySensor;

// Custom sensor unit descriptor
typedef struct {
    uint8_t HTMLcode[32];  // 32 character custom sensor unit HTML code
    uint8_t simpleText[8]; // 8 character custom sensor unit text
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

// CAN message structure
typedef struct {
    uint32_t id;
    uint8_t data[8];
    uint8_t len;
    uint8_t format;
    uint8_t type;
} sPoKeysCANmsg;

// Network device structure - used for network device enumeration
typedef struct {
    uint32_t SerialNumber;        // Serial number
    uint8_t IPaddress[4];         // IP address of the device
    uint8_t hostIP[4];            // IP address of the host PC
    uint8_t FirmwareVersionMajor; // Firmware version - major
    uint8_t FirmwareVersionMinor; // Firmware version - minor
    uint8_t UserID;               // User ID
    uint8_t DHCP;                 // DHCP setting of the device
    uint8_t HWtype;               // HW type, reported by device
    uint8_t
        useUDP; // If set to 1, UDP connection will be established with the device
} sPoKeysNetworkDeviceSummary;

// Network device info structure - used for network device information/setup
typedef struct {
    uint8_t IPAddressCurrent
        [4]; // Current (temporary) address may differ from the setup one
    uint8_t IPAddressSetup[4]; // Setup IP address
    uint8_t Subnetmask[4];     // Subnet mask
    uint8_t DefaultGateway[4]; // Default gateway
    uint16_t TCPtimeout;       // TCP timeout value
    uint8_t
        AdditionalNetworkOptions; // Additional network options: bits 7:4 are 0xA, lower are the following:
        // - bit 3: reserved
        // - bit 2: disable IP configuration via UDP broadcast
        // - bit 1: disable automatic device IP configuration during discovery
        // - bit 0: disable automatic device discovery mechanism
    uint8_t DHCP; // DHCP setting of the device
} sPoKeysNetworkDeviceInfo;

// Other device peripheral data
typedef struct {
    uint32_t AnalogRCFilter;
} sPoKeysOtherPeripherals;

// Failsafe settings
typedef struct {
    uint8_t
        bFailSafeEnabled; // Failsafe enabled and timeout setting (0 disabled other define timeout period in 100ms steps)
    uint8_t bFailSafePeripherals; // Bit-mapped enabled peripherals
                                  // 0 - Digital IO
                                  // 1 - PoExtBus
                                  // 2 - PWM
                                  // 3 - Pulse engine
    uint8_t padding1[6];
    uint8_t bFailSafeIO[7]; // Digital outputs values
    uint8_t padding2[1];
    uint8_t bFailSafePoExtBus[10]; // PoExtBus outputs values
    uint8_t bFailSafePWM[6];       // PWM outputs values
} sPoKeysFailsafeSettings;

// Main PoKeys structure
typedef struct {
    void *devHandle; // Communication device handle
    void *devHandle2;

    sPoKeysDevice_Info info;       // PoKeys device info
    sPoKeysDevice_Data DeviceData; // PoKeys device-specific data
    sPoKeysNetworkDeviceInfo *netDeviceData;

    sPoKeysPinData *Pins;     // PoKeys pins
    sPoKeysEncoder *Encoders; // PoKeys encoders
    ALIGN_TEST(1)
    sMatrixKeyboard matrixKB;    // Matrix keyboard structure
    sPoKeysPWM PWM;              // PWM outputs structure
    sPoKeysMatrixLED *MatrixLED; // Matrix LED structure
    sPoKeysLCD LCD;              // LCD structure
    ALIGN_TEST(7)
    sPoKeysPEv2 PEv2; // Pulse engine v2 structure
    ALIGN_TEST(8)
    sPoKeysPoStepInterface PoSteps; // PoKeys-PoStep interface

    ALIGN_TEST(9)
    sPoNETmodule PoNETmodule;
    ALIGN_TEST(10)
    sPoILStatus PoIL;
    ALIGN_TEST(11)
    sPoKeysRTC RTC;
    ALIGN_TEST(2)

    sPoKeysEasySensor *EasySensors; // EasySensors array

    sPoKeysOtherPeripherals otherPeripherals;
    sPoKeysFailsafeSettings failsafeSettings;
    ALIGN_TEST(3)

    uint8_t
        FastEncodersConfiguration; // Fast encoders configuration, invert settings and 4x sampling (see protocol specification for details)
    uint8_t FastEncodersOptions;   // Fast encoders additional options
    uint8_t
        UltraFastEncoderConfiguration; // Ultra fast encoder configuration (see protocol specification for details)
    uint8_t UltraFastEncoderOptions; // Ultra fast encoder additional options
    uint32_t
        UltraFastEncoderFilter; // Ultra fast encoder digital filter setting

    ALIGN_TEST(4)
    uint8_t *PoExtBusData; // PoExtBus outputs buffer

    uint8_t connectionType;  // Connection type
    uint8_t connectionParam; // Additional connection parameter
    uint8_t requestID;       // Communication request ID
    uint8_t reserved;
    ALIGN_TEST(5)
    uint32_t sendRetries;
    uint32_t readRetries;
    uint32_t socketTimeout;
    uint8_t request[68];  // Communication buffer
    uint8_t response[68]; // Communication buffer

    ALIGN_TEST(6)
    uint8_t multiPartData[448]; // Multi-part request buffer
    uint64_t reserved64;
    uint8_t *multiPartBuffer;
} sPoKeysDevice;

// Enumerate USB devices. Returns number of USB devices detected.
POKEYSDECL int32_t PK_EnumerateUSBDevices(void);
POKEYSDECL int32_t PK_EnumerateFastUSBDevices(void);
// Enumerate network devices. Return the number of ethernet devices detected and the list of detected devices (parameter devices) is filled with devices' data
POKEYSDECL int32_t PK_EnumerateNetworkDevices(
    sPoKeysNetworkDeviceSummary *devices, uint32_t timeout);
POKEYSDECL int32_t PK_SearchNetworkDevices(sPoKeysNetworkDeviceSummary *devices,
                                           uint32_t timeout,
                                           uint32_t serialNumberToFind);
POKEYSDECL void PK_SetEthernetRetryCountAndTimeout(sPoKeysDevice *device,
                                                   uint32_t sendRetries,
                                                   uint32_t readRetries,
                                                   uint32_t timeout);

// Connect to USB PoKeys device, returns pointer to a newly created PoKeys device structure. Returns NULL if the connection is not successfull
POKEYSDECL sPoKeysDevice *PK_ConnectToDevice(uint32_t deviceIndex);
// Connect to a PoKeys device with the specific serial number. Returns pointer to a newly created PoKeys device structure. Returns NULL if the connection is not successfull
POKEYSDECL sPoKeysDevice *
PK_ConnectToDeviceWSerial(uint32_t serialNumber,
                          uint32_t checkForNetworkDevicesAndTimeout);
// Same as above, but uses UDP for connection
POKEYSDECL sPoKeysDevice *
PK_ConnectToDeviceWSerial_UDP(uint32_t serialNumber,
                              uint32_t checkForNetworkDevicesAndTimeout);

// Connect to a network PoKeys device. Returns NULL if the connection is not successfull
POKEYSDECL sPoKeysDevice *
PK_ConnectToNetworkDevice(sPoKeysNetworkDeviceSummary *device);
// Disconnect from a PoKeys device
POKEYSDECL void PK_DisconnectDevice(sPoKeysDevice *device);
// Returns connection type of the specified device
POKEYSDECL int32_t PK_GetCurrentDeviceConnectionType(sPoKeysDevice *device);
// Save current configuration in the device
POKEYSDECL int32_t PK_SaveConfiguration(sPoKeysDevice *device);
// Clear configuration in device - sPoKeysDevice structure is not cleared, so it must be repopulated!
POKEYSDECL int32_t PK_ClearConfiguration(sPoKeysDevice *device);
// Exchange custom request
POKEYSDECL int32_t PK_CustomRequest(sPoKeysDevice *device, unsigned char type,
                                    unsigned char param1, unsigned char param2,
                                    unsigned char param3, unsigned char param4);
POKEYSDECL int32_t PK_GetDebugValues(sPoKeysDevice *device, int32_t *buffer);
// Enable (1) or disable (0) fast USB interface in PoKeys56U/PoKeys57U devices
POKEYSDECL int32_t PK_SetFastUSBEnableStatus(sPoKeysDevice *device,
                                             uint32_t newState);
POKEYSDECL int32_t PK_GetFastUSBEnableStatus(sPoKeysDevice *device,
                                             uint32_t *state);

// Set device name (from device->DeviceData.DeviceName)
POKEYSDECL int32_t PK_DeviceNameSet(sPoKeysDevice *device);

// Check pin capabilities
POKEYSDECL int32_t PK_CheckPinCapability(sPoKeysDevice *device, uint32_t pin,
                                         ePK_AllPinCap cap);
// Check pin capabilities by device, defined by device type mask (see ePK_DeviceTypeMask)
POKEYSDECL int32_t PK_CheckPinCapabilityByDevice(uint64_t deviceTypeMask,
                                                 uint32_t pin,
                                                 ePK_AllPinCap cap);
POKEYSDECL int32_t PK_CheckPinCapabilityByTypeID(uint64_t deviceID,
                                                 uint32_t pin,
                                                 ePK_AllPinCap cap);

// Clone an existing device data structure into a new one
POKEYSDECL void PK_CloneDeviceStructure(sPoKeysDevice *original,
                                        sPoKeysDevice *destination);
// Release the device data structure from memory
POKEYSDECL void PK_ReleaseDeviceStructure(sPoKeysDevice *device);

// Retrieve device-specific information (this also gets automatically called when the connection with the device is established)
POKEYSDECL int32_t PK_DeviceDataGet(sPoKeysDevice *device);
// Start device activation with the activation code, specified in the device structure
POKEYSDECL int32_t PK_DeviceActivation(sPoKeysDevice *device);
// Clear activated options in the device
POKEYSDECL int32_t PK_DeviceActivationClear(sPoKeysDevice *device);

// Configure network device - uses structure device->netDeviceData
POKEYSDECL int32_t PK_NetworkConfigurationSet(sPoKeysDevice *device);

// Retrieve pin configuration from the device
POKEYSDECL int32_t PK_PinConfigurationGet(sPoKeysDevice *device);
// Send pin configuration to device
POKEYSDECL int32_t PK_PinConfigurationSet(sPoKeysDevice *device);

// Retrieve encoder configuration from the device
POKEYSDECL int32_t PK_EncoderConfigurationGet(sPoKeysDevice *device);
// Send encoder configuration to device
POKEYSDECL int32_t PK_EncoderConfigurationSet(sPoKeysDevice *device);
// Retrieve encoder values from device
POKEYSDECL int32_t PK_EncoderValuesGet(sPoKeysDevice *device);
// Send encoder values to device
POKEYSDECL int32_t PK_EncoderValuesSet(sPoKeysDevice *device);

// Set digital outputs values
POKEYSDECL int32_t PK_DigitalIOSet(sPoKeysDevice *device);
// Get digital inputs values
POKEYSDECL int32_t PK_DigitalIOGet(sPoKeysDevice *device);
// Set digital outputs and get digital input values in one call
POKEYSDECL int32_t PK_DigitalIOSetGet(sPoKeysDevice *device);
// Set single digital output
POKEYSDECL int32_t PK_DigitalIOSetSingle(sPoKeysDevice *device, uint8_t pinID,
                                         uint8_t pinValue);
// Get single digital input value
POKEYSDECL int32_t PK_DigitalIOGetSingle(sPoKeysDevice *device, uint8_t pinID,
                                         uint8_t *pinValue);

// Set PoExtBus outputs
POKEYSDECL int32_t PK_PoExtBusSet(sPoKeysDevice *device);
// Get current PoExtBus outputs values
POKEYSDECL int32_t PK_PoExtBusGet(sPoKeysDevice *device);

// Get digital counter values
POKEYSDECL int32_t PK_DigitalCounterGet(sPoKeysDevice *device);
// Check whether digital counter is available for the specified pin. Return True if digital counter is supported.
POKEYSDECL int32_t PK_IsCounterAvailable(sPoKeysDevice *device, uint8_t pinID);
// Check digital counter availability by device, defined by device type mask (see ePK_DeviceTypeMask)
POKEYSDECL int32_t PK_IsCounterAvailableByDevice(uint32_t deviceTypeMask,
                                                 uint8_t pinID);
// Check digital counter availability by device, defined by device ID (see ePK_DeviceTypeID)
POKEYSDECL int32_t PK_IsCounterAvailableByTypeID(uint32_t deviceTypeID,
                                                 uint8_t pinID);

// Get analog input values
POKEYSDECL int32_t PK_AnalogIOGet(sPoKeysDevice *device);
// Get analog input values as an array
POKEYSDECL int32_t PK_AnalogIOGetAsArray(sPoKeysDevice *device,
                                         uint32_t *buffer);

POKEYSDECL int32_t PK_AnalogRCFilterGet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_AnalogRCFilterSet(sPoKeysDevice *device);

// Get matrix keyboard configuration
POKEYSDECL int32_t PK_MatrixKBConfigurationGet(sPoKeysDevice *device);
// Set matrix keyboard configuration
POKEYSDECL int32_t PK_MatrixKBConfigurationSet(sPoKeysDevice *device);
// Get matrix keyboard current key states
POKEYSDECL int32_t PK_MatrixKBStatusGet(sPoKeysDevice *device);

// Set PWM outputs configuration
POKEYSDECL int32_t PK_PWMConfigurationSet(sPoKeysDevice *device);
// Update PWM output duty cycles (PWM period is left unchanged)
POKEYSDECL int32_t PK_PWMUpdate(sPoKeysDevice *device);
// Retrieve PWM configuration
POKEYSDECL int32_t PK_PWMConfigurationGet(sPoKeysDevice *device);

POKEYSDECL int32_t PK_PWMConfigurationSetDirectly(sPoKeysDevice *device,
                                                  uint32_t PWMperiod,
                                                  uint8_t *enabledChannels);
POKEYSDECL int32_t PK_PWMUpdateDirectly(sPoKeysDevice *device,
                                        uint32_t *dutyCycles);

// Get LCD configuration
POKEYSDECL int32_t PK_LCDConfigurationGet(sPoKeysDevice *device);
// Set LCD configuration
POKEYSDECL int32_t PK_LCDConfigurationSet(sPoKeysDevice *device);
// Update LCD contents (only the lines with the refresh flag set)
POKEYSDECL int32_t PK_LCDUpdate(sPoKeysDevice *device);
// Transfer custom characters from device->LCD.customCharacters array
POKEYSDECL int32_t PK_LCDSetCustomCharacters(sPoKeysDevice *device);
// Change between modes PK_LCD_MODE_DIRECT and PK_LCD_MODE_BUFFERED
POKEYSDECL int32_t PK_LCDChangeMode(sPoKeysDevice *device, uint8_t mode);

// The following LCD-related functions can be used in PK_LCD_MODE_DIRECT mode only
// Initialize LCD module
POKEYSDECL int32_t PK_LCDInit(sPoKeysDevice *device);
// Clear LCD screen contents
POKEYSDECL int32_t PK_LCDClear(sPoKeysDevice *device);
// Move cursor to the specified position
POKEYSDECL int32_t PK_LCDMoveCursor(sPoKeysDevice *device, uint8_t row,
                                    uint8_t column);
// Print string to LCD
POKEYSDECL int32_t PK_LCDPrint(sPoKeysDevice *device, uint8_t *text,
                               uint8_t textLen);
// Put single character on LCD
POKEYSDECL int32_t PK_LCDPutChar(sPoKeysDevice *device, uint8_t character);
// Change LCD entry mode register
POKEYSDECL int32_t PK_LCDEntryModeSet(sPoKeysDevice *device,
                                      uint8_t cursorMoveDirection,
                                      uint8_t displayShift);
// Change LCD display on/off control register
POKEYSDECL int32_t PK_LCDDisplayOnOffControl(sPoKeysDevice *device,
                                             uint8_t displayOnOff,
                                             uint8_t cursorOnOff,
                                             uint8_t cursorBlinking);

// Set matrix LED configuration
POKEYSDECL int32_t PK_MatrixLEDConfigurationSet(sPoKeysDevice *device);
// Get matrix LED configuration
POKEYSDECL int32_t PK_MatrixLEDConfigurationGet(sPoKeysDevice *device);
// Update matrix LED (only the displays with refresh flag set)
POKEYSDECL int32_t PK_MatrixLEDUpdate(sPoKeysDevice *device);

// Pulse engine v2 commands

// Get status of Pulse engine
POKEYSDECL int32_t PK_PEv2_StatusGet(sPoKeysDevice *device);
// Get additional status data
POKEYSDECL int32_t PK_PEv2_Status2Get(sPoKeysDevice *device);
// Configure (setup) the pulse engine
POKEYSDECL int32_t PK_PEv2_PulseEngineSetup(sPoKeysDevice *device);
// Read additional parameters
POKEYSDECL int32_t PK_PEv2_AdditionalParametersGet(sPoKeysDevice *device);
// Set additional parameters
POKEYSDECL int32_t PK_PEv2_AdditionalParametersSet(sPoKeysDevice *device);
// Retrieve single axis parameters. Axis ID is in param1
POKEYSDECL int32_t PK_PEv2_AxisConfigurationGet(sPoKeysDevice *device);
// Set single axis parameters. Axis ID is in param1
POKEYSDECL int32_t PK_PEv2_AxisConfigurationSet(sPoKeysDevice *device);
// Set positions - param2 is used for bit-mapped axis selection
POKEYSDECL int32_t PK_PEv2_PositionSet(sPoKeysDevice *device);
// Set pulse engine state
POKEYSDECL int32_t PK_PEv2_PulseEngineStateSet(sPoKeysDevice *device);
// Execute the move. Position or speed is specified by the ReferencePositionSpeed
POKEYSDECL int32_t PK_PEv2_PulseEngineMove(sPoKeysDevice *device);
// Read external outputs state - save them to ExternalRelayOutputs and ExternalOCOutputs
POKEYSDECL int32_t PK_PEv2_ExternalOutputsGet(sPoKeysDevice *device);
// Set external outputs state (from ExternalRelayOutputs and ExternalOCOutputs)
POKEYSDECL int32_t PK_PEv2_ExternalOutputsSet(sPoKeysDevice *device);
// Transfer motion buffer to device. The number of new entries (newMotionBufferEntries) must be specified
// The number of accepted entries is saved to motionBufferEntriesAccepted.
// In addition, pulse engine state is read (PEv2_GetStatus)
POKEYSDECL int32_t PK_PEv2_BufferFill(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_BufferFill_16(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_BufferFillLarge(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_BufferFillLarge_16(sPoKeysDevice *device);
// Clear motion buffer in device
POKEYSDECL int32_t PK_PEv2_BufferClear(sPoKeysDevice *device);
// Reboot pulse engine v2
POKEYSDECL int32_t PK_PEv2_PulseEngineReboot(sPoKeysDevice *device);
// Start the homing procedure. Home offsets must be provided in the HomeOffsets
// Axes to home are selected as bit-mapped HomingStartMaskSetup value
POKEYSDECL int32_t PK_PEv2_HomingStart(sPoKeysDevice *device);
// Finish the homing procedure
POKEYSDECL int32_t PK_PEv2_HomingFinish(sPoKeysDevice *device);
// Star the probing procedure.
// ProbeMaxPosition defines the maximum position in position ticks where probing error will be thrown
// ProbeSpeed defines the probing speed (1 = max speed)
// ProbeInput defines the extenal input (values 1-8) or PoKeys pin (0-based Pin ID + 9)
// ProbeInputPolarity defines the polarity of the probe signal
POKEYSDECL int32_t PK_PEv2_ProbingStart(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_ProbingHybridStart(sPoKeysDevice *device);
// Finish the probing procedure. Probe position and status are saved to ProbePosition and ProbeStatus
POKEYSDECL int32_t PK_PEv2_ProbingFinish(sPoKeysDevice *device);
// Same as previous command, except for pulse engine states not being reset to 'STOPPED'
POKEYSDECL int32_t PK_PEv2_ProbingFinishSimple(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_SyncedPWMSetup(sPoKeysDevice *device,
                                          uint8_t enabled, uint8_t srcAxis,
                                          uint8_t dstPWMChannel);

POKEYSDECL int32_t PK_PEv2_ThreadingPrepareForTrigger(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_ThreadingForceTriggerReady(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_ThreadingTrigger(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_ThreadingRelease(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_ThreadingStatusGet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_ThreadingCancel(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PEv2_ThreadingSetup(sPoKeysDevice *device,
                                          uint8_t sensorMode,
                                          uint16_t ticksPerRevolution,
                                          uint16_t tagetSpindleRPM,
                                          uint16_t filterGainSpeed,
                                          uint16_t filterGainPosition);

POKEYSDECL int32_t
PK_PEv2_BacklashCompensationSettings_Get(sPoKeysDevice *device);
POKEYSDECL int32_t
PK_PEv2_BacklashCompensationSettings_Set(sPoKeysDevice *device);

POKEYSDECL int32_t PK_PoStep_ConfigurationGet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoStep_ConfigurationSet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoStep_StatusGet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoStep_DriverConfigurationGet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoStep_DriverConfigurationSet(sPoKeysDevice *device);

// I2C operations status return ePK_I2C_STATUS, described above
// Set I2C status - does nothing in the device as I2C is ON all the time
POKEYSDECL int32_t PK_I2CSetStatus(sPoKeysDevice *device, uint8_t activated);
// Retrieves I2C bus activation status
POKEYSDECL int32_t PK_I2CGetStatus(sPoKeysDevice *device, uint8_t *activated);
// Execute write to the specified address. iDataLength specifies how many bytes should be sent from the buffer (0 to 32)
POKEYSDECL int32_t PK_I2CWriteStart(sPoKeysDevice *device, uint8_t address,
                                    uint8_t *buffer, uint8_t iDataLength);
POKEYSDECL int32_t PK_I2CWriteAndReadStart(sPoKeysDevice *device,
                                           uint8_t address, uint8_t *buffer,
                                           uint8_t iDataLengthWrite,
                                           uint8_t iDataLengthRead);
// Get write operation status (1 if successfull, 0 unsuccessfull, 0x10 � operation still executing)
POKEYSDECL int32_t PK_I2CWriteStatusGet(sPoKeysDevice *device, uint8_t *status);
// Execute read from the specified address. iDataLength specifies how many bytes should be requested
POKEYSDECL int32_t PK_I2CReadStart(sPoKeysDevice *device, uint8_t address,
                                   uint8_t iDataLength);
// Get read operation results. iReadBytes returns the number of bytes read from the selected device, iMaxBufferLength specifies how many bytes buffer can accept
POKEYSDECL int32_t PK_I2CReadStatusGet(sPoKeysDevice *device, uint8_t *status,
                                       uint8_t *iReadBytes, uint8_t *buffer,
                                       uint8_t iMaxBufferLength);
// Execute bus scan
POKEYSDECL int32_t PK_I2CBusScanStart(sPoKeysDevice *device);
// Get bus scan results. iMaxDevices specifies how big presentDevices buffer is. presentDevices returns one entry per device
POKEYSDECL int32_t PK_I2CBusScanGetResults(sPoKeysDevice *device,
                                           uint8_t *status,
                                           uint8_t *presentDevices,
                                           uint8_t iMaxDevices);

POKEYSDECL int32_t PK_PoNETGetPoNETStatus(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoNETGetModuleSettings(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoNETGetModuleStatusRequest(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoNETGetModuleStatus(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoNETSetModuleStatus(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoNETSetModulePWM(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoNETGetModuleLightRequest(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoNETGetModuleLight(sPoKeysDevice *device);

// 1-wire operations
// Set 1-wire activation status
POKEYSDECL int32_t PK_1WireStatusSet(sPoKeysDevice *device, uint8_t activated);
// Get 1-wire activation status
POKEYSDECL int32_t PK_1WireStatusGet(sPoKeysDevice *device, uint8_t *activated);
// Start 1-wire write and read operation
POKEYSDECL int32_t PK_1WireWriteReadStart(sPoKeysDevice *device,
                                          uint8_t WriteCount, uint8_t ReadCount,
                                          uint8_t *data);
// Start 1-wire write and read operation on specific pin (only PoKeys57 series devices)
POKEYSDECL int32_t PK_1WireWriteReadStartEx(sPoKeysDevice *device,
                                            uint8_t pinID, uint8_t WriteCount,
                                            uint8_t ReadCount, uint8_t *data);
// Get the result of the read operation
POKEYSDECL int32_t PK_1WireReadStatusGet(sPoKeysDevice *device,
                                         uint8_t *readStatus,
                                         uint8_t *ReadCount, uint8_t *data);

// Start scanning for 1-wire devices
POKEYSDECL int32_t PK_1WireBusScanStart(sPoKeysDevice *device, uint8_t pinID);
// Get scan result
POKEYSDECL int32_t PK_1WireBusScanGetResults(sPoKeysDevice *device,
                                             uint8_t *operationStatus,
                                             uint8_t *scanResult,
                                             uint8_t *deviceROM);
// Continue with device scanning
POKEYSDECL int32_t PK_1WireBusScanContinue(sPoKeysDevice *device);
// Stop the device scan
POKEYSDECL int32_t PK_1WireBusScanStop(sPoKeysDevice *device);

// Get the configuration of EasySensors
POKEYSDECL int32_t PK_EasySensorsSetupGet(sPoKeysDevice *device);
// Set the configuration of EasySensors
POKEYSDECL int32_t PK_EasySensorsSetupSet(sPoKeysDevice *device);
// Get all EasySensors values
POKEYSDECL int32_t PK_EasySensorsValueGetAll(sPoKeysDevice *device);

// Failsafe settings
POKEYSDECL int32_t PK_FailsafeSettingsGet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_FailsafeSettingsSet(sPoKeysDevice *device);

// SPI operations
POKEYSDECL int32_t PK_SPIConfigure(sPoKeysDevice *device, uint8_t prescaler,
                                   uint8_t frameFormat);
POKEYSDECL int32_t PK_SPIWrite(sPoKeysDevice *device, uint8_t *buffer,
                               uint8_t iDataLength, uint8_t pinCS);
POKEYSDECL int32_t PK_SPIRead(sPoKeysDevice *device, uint8_t *buffer,
                              uint8_t iDataLength);

// PoIL commands
POKEYSDECL int32_t PK_PoILGetState(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoILSetCoreState(sPoKeysDevice *device, uint16_t state);
POKEYSDECL int32_t PK_PoILSetMasterEnable(sPoKeysDevice *device,
                                          uint8_t masterEnable);
POKEYSDECL int32_t PK_PoILResetCore(sPoKeysDevice *device);
POKEYSDECL int32_t PK_PoILSetDebugMode(sPoKeysDevice *device, uint8_t debugMode,
                                       uint16_t breakpoint);
POKEYSDECL int32_t PK_PoILReadMemory(sPoKeysDevice *device, uint8_t memoryType,
                                     uint16_t address, uint16_t size,
                                     uint8_t *dest);
POKEYSDECL int32_t PK_PoILWriteMemory(sPoKeysDevice *device, uint8_t memoryType,
                                      uint16_t address, uint16_t size,
                                      uint8_t *src);
POKEYSDECL int32_t PK_PoILEraseMemory(sPoKeysDevice *device,
                                      uint8_t memoryType);
POKEYSDECL int32_t PK_PoILChunkReadMemory(sPoKeysDevice *device, uint8_t *dest);
POKEYSDECL int32_t PK_PoILChunkReadMemoryInternalAddress(sPoKeysDevice *device,
                                                         uint8_t *dest);
POKEYSDECL int32_t PK_PoILReadSharedSlot(sPoKeysDevice *device,
                                         uint16_t firstSlotID,
                                         uint16_t slotsNum, int32_t *dest);
POKEYSDECL int32_t PK_PoILWriteSharedSlot(sPoKeysDevice *device,
                                          uint16_t firstSlotID,
                                          uint16_t slotsNum, int32_t *src);
POKEYSDECL int32_t PK_PoILTaskStatus(sPoKeysDevice *device);

// RTC commands (real-time clock)
POKEYSDECL int32_t PK_RTCGet(sPoKeysDevice *device);
POKEYSDECL int32_t PK_RTCSet(sPoKeysDevice *device);

// UART commands
POKEYSDECL int32_t PK_UARTConfigure(sPoKeysDevice *device, uint32_t baudrate,
                                    uint8_t format, uint8_t interfaceID);
POKEYSDECL int32_t PK_UARTWrite(sPoKeysDevice *device, uint8_t interfaceID,
                                uint8_t *dataPtr, uint32_t dataWriteLen);
POKEYSDECL int32_t PK_UARTRead(sPoKeysDevice *device, uint8_t interfaceID,
                               uint8_t *dataPtr, uint8_t *dataReadLen);

// CAN commands
POKEYSDECL int32_t PK_CANConfigure(sPoKeysDevice *device, uint32_t bitrate);
POKEYSDECL int32_t PK_CANRegisterFilter(sPoKeysDevice *device, uint8_t format,
                                        uint32_t CANid);
POKEYSDECL int32_t PK_CANWrite(sPoKeysDevice *device, sPoKeysCANmsg *msg);
POKEYSDECL int32_t PK_CANRead(sPoKeysDevice *device, sPoKeysCANmsg *msg,
                              uint8_t *status);

// WS2812 commands
POKEYSDECL int32_t PK_WS2812_Update(sPoKeysDevice *device, uint16_t LEDcount,
                                    uint8_t updateFlag);
POKEYSDECL int32_t PK_WS2812_SendLEDdata(sPoKeysDevice *device,
                                         uint32_t *LEDdata, uint16_t startLED,
                                         uint8_t LEDcount);

// Simplified interface...
POKEYSDECL void PK_SL_SetPinFunction(sPoKeysDevice *device, uint8_t pin,
                                     uint8_t function);
POKEYSDECL uint8_t PK_SL_GetPinFunction(sPoKeysDevice *device, uint8_t pin);
POKEYSDECL void PK_SL_DigitalOutputSet(sPoKeysDevice *device, uint8_t pin,
                                       uint8_t value);
POKEYSDECL uint8_t PK_SL_DigitalInputGet(sPoKeysDevice *device, uint8_t pin);
POKEYSDECL uint32_t PK_SL_AnalogInputGet(sPoKeysDevice *device, uint8_t pin);

POKEYSDECL uint32_t PK_SL_EncoderValueGet(sPoKeysDevice *device, uint8_t index);
POKEYSDECL int32_t PK_SL_EasySensorValueGet(sPoKeysDevice *device,
                                            uint8_t index);

POKEYSDECL uint32_t PK_SL_PWMConfig(sPoKeysDevice *device, uint8_t index);
POKEYSDECL int32_t PK_SL_PWM_SetPeriod(sPoKeysDevice *device,
                                       uint32_t PWMperiod);
POKEYSDECL int32_t PK_SL_PWM_SetChannelEnabled(sPoKeysDevice *device,
                                               uint8_t channel, uint8_t enabled,
                                               uint32_t defaultDuty);
POKEYSDECL int32_t PK_SL_PWM_SetDuty(sPoKeysDevice *device, uint8_t channel,
                                     uint32_t duty);

extern int32_t LastRetryCount;
extern int32_t LastWaitCount;
#ifdef __cplusplus
}
#endif

#endif
