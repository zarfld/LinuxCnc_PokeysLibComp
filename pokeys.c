component pokeys "PoKeys IO driver, by Mit Zot";

option userspace yes;

pin out unsigned enum_usb_dev;
pin out unsigned enum_fusb_dev;
pin out unsigned enum_udp_dev;
pin out s32 a_debout;

pin out bit err;
pin out bit connected;
pin in unsigned devSerial;
pin out bit alive;

pin in bit machine_is_on;

// PoKeys device information
//sPoKeysDevice_Info info;
pin out unsigned info.PinCount;                        // Number of pins, physically on the device
pin out unsigned info.PWMCount;                        // Number of pins that support PWM output
pin out unsigned info.BasicEncoderCount;               // Number of basic encoders
pin out unsigned info.EncodersCount;                   // Number of encoder slots available
pin out unsigned info.FastEncoders;                    // Number of fast encoders supported
pin out unsigned info.UltraFastEncoders;               // Number of available ultra fast encoders
pin out unsigned info.PWMinternalFrequency;             // Main PWM peripheral clock
pin out unsigned info.AnalogInputs;                    // Number of available analog inputs
pin out unsigned info.KeyMapping;                      // Device supports key mapping (acts as a USB keyboard)
pin out unsigned info.TriggeredKeyMapping;             // Device supports triggered key mapping
pin out unsigned info.KeyRepeatDelay;                  // Device supports user customizable key repeat rates and delays
pin out unsigned info.DigitalCounters;                 // Device supports digital counters
pin out unsigned info.JoystickButtonAxisMapping;       // Device supports mapping of joystick buttons
pin out unsigned info.JoystickAnalogToDigitalMapping;  // Device supports mapping of analog inputs to digital keys
pin out unsigned info.Macros;                          // Device supports customizable macro sequences
pin out unsigned info.MatrixKeyboard;                  // Device supports matrix keyboard
pin out unsigned info.MatrixKeyboardTriggeredMapping;  // Device supports matrix keyboard triggered key mapping
pin out unsigned info.LCD;                             // Device supports alphanumeric LCD display
pin out unsigned info.MatrixLED;                       // Device supports matrix LED display
pin out unsigned info.ConnectionSignal;                // Device supports connection signal output
pin out unsigned info.PoExtBus;                        // Device supports PoExtBus digital outputs
pin out unsigned info.PoNET;                           // Device supports PoNET bus devices
pin out unsigned info.AnalogFiltering;                 // Device supports analog inputs low-pass digital filtering
pin out unsigned info.InitOutputsStart;                // Device supports initializing outputs at startup
pin out unsigned info.protI2C;                         // Device supports I2C bus (master)
pin out unsigned info.prot1wire;                       // Device supports 1-wire bus (master)
pin out unsigned info.AdditionalOptions;               // Device supports additional options with activation keys
pin out unsigned info.LoadStatus;                      // Device supports reporting load status
pin out unsigned info.CustomDeviceName;                // Device supports specifying custom device names
pin out unsigned info.PoTLog27support;                 // Device supports PoTLog27 firmware
pin out unsigned info.SensorList;                      // Device supports sensor lists
pin out unsigned info.WebInterface;                    // Device supports web interface
pin out unsigned info.FailSafeSettings;                // Device supports fail-safe mode
pin out unsigned info.JoystickHATswitch;               // Device supports joystick HAT switch mapping
pin out unsigned info.PulseEngine;                     // Device supports Pulse engine
pin out unsigned info.PulseEnginev2;                   // Device supports Pulse engine v2
pin out unsigned info.EasySensors;                     // Device supports EasySensors

//pin out unsigned PoExtBus.#.DataGet [10];   
pin out bit PoExtBus.#.In-0 [10];                   
pin out bit PoExtBus.#.In-1 [10];    
pin out bit PoExtBus.#.In-2 [10];    
pin out bit PoExtBus.#.In-3 [10];    
pin out bit PoExtBus.#.In-4 [10];    
pin out bit PoExtBus.#.In-5 [10];    
pin out bit PoExtBus.#.In-6 [10];    
pin out bit PoExtBus.#.In-7 [10]; 


//pin io unsigned PoExtBus.#.DataSet [10];                 
pin in bit PoExtBus.#.Out-0 [10];                   
pin in bit PoExtBus.#.Out-1 [10];    
pin in bit PoExtBus.#.Out-2 [10];    
pin in bit PoExtBus.#.Out-3 [10];    
pin in bit PoExtBus.#.Out-4 [10];    
pin in bit PoExtBus.#.Out-5 [10];    
pin in bit PoExtBus.#.Out-6 [10];    
pin in bit PoExtBus.#.Out-7 [10];

// Pulse engine v2 information
pin out unsigned PEv2.nrOfAxes;
pin out unsigned PEv2.maxPulseFrequency;
pin out unsigned PEv2.bufferDepth;
pin out unsigned PEv2.slotTiming;

//***********************************************************************
//[TRAJ] information in HAL-INI 
pin in s32 PEv2.params.COORDINATES;
pin in s32 PEv2.params.LINEAR_UNITS;
pin in s32 PEv2.params.ANGULAR_UNITS;
pin in float PEv2.params.CYCLE_TIME;
pin in float PEv2.params.DEFAULT_LINEAR_VELOCITY;
pin in float PEv2.params.MAX_LINEAR_VELOCITY;

pin io bit PEv2.params.ApplyIniSettings;
//***********************************************************************

// Pulse engine v2 structure...
pin out unsigned PEv2.#.AxesState [8];				// Axis states (bit-mapped) - see ePK_PEAxisState
pin out unsigned PEv2.#.AxesConfig [8];				// Axis configuration - see ePK_PEv2_AxisConfig
pin in unsigned PEv2.#.AxesSwitchConfig [8];		// Axis switch configuration - see ePK_PulseEngineV2_AxisSwitchOptions

pin io u32 PEv2.#.InvertAxisEnable [8];		// Invert axis enable signal
pin io u32 PEv2.#.SoftLimitMaximum [8];		// Soft limit maximum position
pin io u32 PEv2.#.SoftLimitMinimum [8];		// Soft limit minimum position
pin io unsigned PEv2.#.HomingSpeed [8];			// Homing speed per axis (in %)
pin io unsigned PEv2.#.HomingReturnSpeed [8];		// Homing return speed per axis (in % of the homing speed)
pin io u32 PEv2.#.HomeOffsets [8];				// Home position offset
pin io unsigned PEv2.#.HomingAlgorithm [8];		// Homing algorithm configuration
pin io unsigned PEv2.#.FilterLimitMSwitch [8];		// Digital filter for limit- switch
pin io unsigned PEv2.#.FilterLimitPSwitch [8];		// Digital filter for limit+ switch
pin io unsigned PEv2.#.FilterHomeSwitch [8];		// Digital filter for home switch
pin io u32 PEv2.#.ProbePosition [8];			// Position where probe detected change
pin io u32 PEv2.#.ProbeMaxPosition [8];		// Maximum position to travel to until stopping and returning error

pin out u32 PEv2.#.CurrentPosition [8];			// Current position
pin io u32 PEv2.#.PositionSetup [8];			// Position to be set as current position
pin io u32 PEv2.#.ReferencePositionSpeed [8];	// Reference position or speed (position or pulses/s)
pin io float PEv2.#.MaxSpeed [8];					// Maximum axis speed (in pulses per ms)
pin io float PEv2.#.MaxAcceleration [8];			// Maximum axis acceleration (in pulses/ms/ms)
pin io float PEv2.#.MaxDecceleration [8];			// Maximum axis deceleration (in pulses/ms/ms)

pin io float PEv2.#.MaxSpeed_mm_per_sec [8];			// Maximum axis speed (mm/s)
pin io float PEv2.#.MaxAcceleration_mm_per_sec2 [8];	// Maximum axis acceleration (in mm/s�)
pin io float PEv2.#.MaxDecceleration_mm_per_sec2 [8];	// Maximum axis deceleration (in mm/s�)

	
pin in float PEv2.#.joint_vel_cmd [8];				
pin out float PEv2.#.joint_vel_fb [8];				
pin in float PEv2.#.joint_pos_cmd [8];				
pin out float PEv2.#.joint_pos_fb [8];				
    
//***********************************************************************
//[JOINT_n] information in HAL-INI 
pin io s32 PEv2.#.params.TYPE [8];			//LINEAR / ANGULAR
pin io float PEv2.#.params.HOME [8];
pin io float PEv2.#.params.FERROR [8];
pin io float PEv2.#.params.MIN_FERROR [8];
pin io float PEv2.#.params.MAX_VELOCITY [8];
pin io float PEv2.#.params.MAX_ACCELERATION [8];
pin io float PEv2.#.params.STEPGEN_MAXVEL [8];
pin io float PEv2.#.params.STEPGEN_MAXACCEL [8];
pin io float PEv2.#.params.Feedback_Encoder_Id [8];
//pin in float PEv2.#.params.P [8];
//pin in float PEv2.#.params.I [8];
//pin in float PEv2.#.params.D [8];
//pin in float PEv2.#.params.FF0 [8];
//pin in float PEv2.#.params.FF1 [8];
//pin in float PEv2.#.params.FF2 [8];
//pin in float PEv2.#.params.BIAS [8];
pin io float PEv2.#.params.DEADBAND [8];
pin io float PEv2.#.params.MAX_OUTPUT [8];
pin io float PEv2.#.params.ENCODER_SCALE [8];
// these are in nanoseconds
pin io float PEv2.#.params.DIRSETUP [8] = 0;					// (ns)
pin io float PEv2.#.params.DIRHOLD [8] = 0;					// (ns)
pin io float PEv2.#.params.STEPLEN [8] = 0;					// (ns)
pin io float PEv2.#.params.STEPSPACE [8] = 0;					// (ns)
pin io float PEv2.#.params.STEP_SCALE [8] = 0;					// (pulses/mm)
pin io float PEv2.#.params.MIN_LIMIT [8] = 0;					// position (mm)
pin io float PEv2.#.params.MAX_LIMIT [8] = 0;					// position (mm)
pin io float PEv2.#.params.HOME_OFFSET [8] = 0;				// position (mm)
pin io float PEv2.#.params.HOME_SEARCH_VEL [8] = 0;			// position (mm)
pin io float PEv2.#.params.HOME_LATCH_VEL [8] = 0;				// position (mm)
pin io float PEv2.#.params.HOME_FINAL_VEL [8] = 0;				// position (mm)
pin io s32 PEv2.#.params.HOME_USE_INDEX [8] = 0;				// YES / NO
pin io s32 PEv2.#.params.HOME_IGNORE_LIMITS [8] = 0;			// YES / NO
//***********************************************************************

pin io u32 PEv2.#.MPGjogMultiplier [8];		// MPG jog multiplier value
pin io unsigned PEv2.#.MPGjogEncoder [8];			// MPG jog encoder ID

pin io unsigned PEv2.#.PinHomeSwitch [8];			// Home switch pin (0 for external dedicated input)
pin io unsigned PEv2.#.PinLimitMSwitch [8];			// Limit- switch pin (0 for external dedicated input)
pin io unsigned PEv2.#.PinLimitPSwitch [8];			// Limit+ switch pin (0 for external dedicated input)
pin io unsigned PEv2.#.AxisEnableOutputPins [8];		// Axis enabled output pin (0 for external dedicated output)

pin out bit PEv2.#.HomeSwitchStat [8];				// Home switch
pin out bit PEv2.#.LimitMSwitch [8];			// Limit- switch
pin out bit PEv2.#.LimitPSwitch [8];			// Limit+ switch
pin in bit PEv2.#.AxisEnableOutput [8];		// Axis enabled output
pin out bit PEv2.HomeSwitch;				// Home switch

pin io unsigned PEv2.#.HomeBackOffDistance [8];		// Back-off distance after homing
pin io unsigned PEv2.#.MPGjogDivider [8];			// Divider for the MPG jogging (enhanced encoder resolution)

pin io unsigned PEv2.PulseEngineEnabled;			// Pulse engine enabled status, also number of enabled axes
pin io unsigned PEv2.PulseGeneratorType;			// Pulse engine generator type (0: external, 1: internal 3ch)
pin in unsigned PEv2.PG_swap_stepdir;
pin in unsigned PEv2.PG_extended_io;

pin io unsigned PEv2.ChargePumpEnabled;				// Charge pump output enabled
pin io unsigned PEv2.EmergencySwitchPolarity;		// Emergency switch polarity (set to 1 to invert)
pin out unsigned PEv2.PulseEngineActivated;			// Pulse engine activation status
pin out unsigned PEv2.#.LimitStatusP[8];					// Limit+ status (NOT bit-mapped)
pin out unsigned PEv2.#.LimitStatusN[8];					// Limit- status (NOT bit-mapped)
pin out unsigned PEv2.#.HomeStatus[8];					// Home status (NOT bit-mapped)
pin out unsigned PEv2.ErrorInputStatus;				// Stepper motor driver error inputs status (bit-mapped)
pin out unsigned PEv2.MiscInputStatus;				// Miscelenous digital inputs...
pin io unsigned PEv2.LimitOverride;					// Limit override status
pin io unsigned PEv2.LimitOverrideSetup;			// Limit override configuration
pin io unsigned PEv2.PulseEngineState;				// State of pulse engine - see ePoKeysPEState
pin io unsigned PEv2.AxisEnabledMask;				// Bit-mapped ouput enabled mask

pin io unsigned PEv2.EmergencyInputPin;
pin out bit PEv2.EmergencyInput;
pin io unsigned PEv2.EmergencyOutputPin;				//added in component only for estop-out to reset estop (if 0 EmergencyInput will be reused)
pin in bit PEv2.EmergencyOutput;

pin io unsigned PEv2.param1;
pin io unsigned PEv2.param2;
pin io unsigned PEv2.param3;
pin io unsigned PEv2.AxisEnabledStatesMask;		// Bit-mapped states, where axis enabled and charge pump signals are active
pin io unsigned PEv2.PulseEngineStateSetup;		// Pulse engine new state configuration
pin out unsigned PEv2.#.SoftLimitStatus [8] ;				// Bit-mapped soft-limit statuses per axes
/* 			PEv2_DedicatedLimitNInputs(i) =  Get_BitOfByte(bm_DedicatedLimitNInputs,i);
			PEv2_DedicatedLimitPInputs(i) =  Get_BitOfByte(bm_DedicatedLimitPInputs,i);
			PEv2_DedicatedHomeInputs(i) =  Get_BitOfByte(bm_DedicatedHomeInputs,i);
								PEv2_AxisEnabledStates(i) = false;
					PEv2_AxisLimitOverride(i) = false;
			*/
pin out bit PEv2.#.DedicatedLimitNInputs [8] ;
pin out bit PEv2.#.DedicatedLimitPInputs [8] ;
pin out bit PEv2.#.DedicatedHomeInputs [8] ;
pin out bit PEv2.#.AxisEnabledStates [8] ;
pin out bit PEv2.#.AxisLimitOverride [8] ;

pin io unsigned PEv2.ExternalRelayOutputs;			// External relay outputs
pin io unsigned PEv2.ExternalOCOutputs;			// External open-collector outputs
pin io unsigned PEv2.PulseEngineBufferSize;		// Buffer size information...
pin io unsigned PEv2.motionBufferEntriesAccepted;
pin io unsigned PEv2.newMotionBufferEntries;
pin io unsigned PEv2.HomingStartMaskSetup;			// Bit-mapped axes indexes to be homed
pin io unsigned PEv2.ProbeStartMaskSetup;			// Bit-mapped axis indexes for probing
pin io unsigned PEv2.ProbeInput;					// Probe input (0:disabled, 1-8:external inputs, 9+ Pin ID-9)
pin io unsigned PEv2.ProbeInputPolarity;			// Probe input polarity
pin out unsigned PEv2.ProbeStatus;					// Probe status (probe completion bit-mapped status)
//pin out unsigned PEv2.MotionBuffer[448];			// Motion buffer
pin io float PEv2.ProbeSpeed;						// Probe speed (ratio of the maximum speed)
pin io unsigned PEv2.#.BacklashWidth [8];			// half of real backlash width
pin io unsigned PEv2.#.BacklashRegister [8];		// current value of the backlash register
pin io unsigned PEv2.#.BacklashAcceleration [8];	// in pulses per ms^2
pin io unsigned PEv2.BacklashCompensationEnabled;

pin out unsigned rtc.sec;
pin out unsigned rtc.min;
pin out unsigned rtc.hour;
pin out unsigned rtc.dow;
pin out unsigned rtc.dom;
pin out unsigned rtc.tmp;
pin out unsigned rtc.doy;
pin out unsigned rtc.month;
pin out unsigned rtc.year;
pin out unsigned rtc.loopcount;


pin out unsigned rtc.lastmin;
pin out unsigned rtc.lastsec;
pin out unsigned rtc.loop_frequ;
pin in unsigned rtc.loop_frequ_demand;
pin in unsigned rtc.sec_ret;
pin out unsigned rtc.hal_latency;

pin out unsigned Pins.#.DigitalCounterValue [55];
pin out unsigned Pins.#.AnalogValue [55];
pin out unsigned Pins.#.PinFunction [55];
pin out unsigned Pins.#.CounterOptions [55];
pin out bit Pins.#.DigitalValueGet [55];
pin in bit Pins.#.DigitalValueSet [55];
pin out unsigned Pins.#.DigitalCounterAvailable [55];
//pin out unsigned Pins.#.MappingType [55];
//pin out unsigned Pins.#.KeyCodeMacroID [55];
//pin out unsigned Pins.#.KeyModifier [55];
//pin out unsigned Pins.#.downKeyCodeMacroID [55];
//pin out unsigned Pins.#.downKeyModifier [55];
//pin out unsigned Pins.#.upKeyCodeMacroID [55];
//pin out unsigned Pins.#.upKeyModifier [55];
//pin out unsigned Pins.#.preventUpdate [55];

/*
pin out unsigned MatrixKB.matrixKBconfiguration;		// Matrix keyboard configuration (set to 1 to enable matrix keyboard support)
pin out unsigned MatrixKB.matrixKBwidth;				// Matrix keyboard width (number of columns)
pin out unsigned MatrixKB.matrixKBheight;				// Matrix keyboard height (number of rows)
pin out unsigned MatrixKB.matrixKBcolumnsPins.#[8];		// List of matrix keyboard column connections
pin out unsigned MatrixKB.matrixKBrowsPins.#[16];			// List of matrix keyboard row connections
//pin out unsigned MatrixKB.matrixKBvalues.#[128];			// Current state of each matrix keyboard key (assumes fixed width of 8 columns)
//pin out bit MatrixKB.matrixKBPins.#[128];			// matrixKBrowsPins
*/

pin out bit kbd48CNC.available;
pin io unsigned kbd48CNC.PoNetID;
pin io unsigned kbd48CNC.KeyBrightness;
pin io unsigned kbd48CNC.prevBrightness;
pin io unsigned kbd48CNC.lightValue;
//pin io unsigned kbd48CNC.#.ID [48];
pin in bit kbd48CNC.#.LED [48];
pin out bit kbd48CNC.#.Button [48];


pin out unsigned Encoders.#.encoderValue [26];       // Encoder current value
pin io unsigned Encoders.#.encoderOptions [26];      // Encoder options -    bit 0: enable encoder
													//                      bit 1: 4x sampling
													//                      bit 2: 2x sampling
													//                      bit 3: reserved
													//                      bit 4: direct key mapping for direction A
													//                      bit 5: mapped to macro for direction A
													//                      bit 6: direct key mapping for direction B
													//                      bit 7: mapped to macro for direction B
pin io unsigned Encoders.#.channelApin [26];         // Channel A encoder pin
pin io unsigned Encoders.#.channelBpin [26];         // Channel B encoder pin



pin out u32 EasySensors.#.sensorValue [16];            // Current sensor value
pin out unsigned EasySensors.#.sensorType [16];             // Type of the sensor
pin out unsigned EasySensors.#.sensorRefreshPeriod [16];    // Refresh period in 0.1s
pin out unsigned EasySensors.#.sensorFailsafeConfig [16];   // Failsafe configuration (bits 0-5: timeout in seconds, bit 6: invalid=0, bit 7: invalid=0x7FFFFFFF)
pin out unsigned EasySensors.#.sensorReadingID [16];        // Sensor reading selection (see Protocol description document for details)


/*
pin io unsigned PoStep.EnablePoStepCommunication;
    // Status
pin out unsigned PoStep.#.SupplyVoltage [8];
pin out unsigned PoStep.#.Temperature [8];
pin out unsigned PoStep.#.InputStatus [8];
pin out unsigned PoStep.#.DriverStatus [8];
pin out unsigned PoStep.#.FaultStatus [8];
pin out unsigned PoStep.#.UpdateState [8];

    // Settings
pin io unsigned PoStep.#.DriverMode [8];
pin io unsigned PoStep.#.StepMode [8];
pin io unsigned PoStep.#.Current_FS [8];
pin io unsigned PoStep.#.Current_Idle [8];
pin io unsigned PoStep.#.Current_Overheat [8];
pin io unsigned PoStep.#.TemperatureLimit [8];

    // Configuration
pin io unsigned PoStep.#.AddressI2C [8];
pin io unsigned PoStep.#.DriverType [8];
pin io unsigned PoStep.#.UpdateConfig [8];


pin io unsigned PWM.PWMperiod;
pin io unsigned PWM.#.PWMduty [6];
pin io unsigned PWM.#.PWMenabledChannels [6];
pin io unsigned PWM.#.PWMpinIDs [6];
*/
pin out unsigned PoNET.#.moduleID[16];
pin out unsigned PoNET.#.i2cAddress[16];
pin out unsigned PoNET.#.moduleType[16];
pin out unsigned PoNET.#.moduleSize[16];
pin out unsigned PoNET.#.moduleOptions[16];

pin io unsigned PoNET.PWMduty;
pin io unsigned PoNET.lightValue;
pin io unsigned PoNET.PoNETstatus;
pin out unsigned PoNET.#.statusIn [16];
pin in unsigned PoNET.#.statusOut [16];
/*
pin io unsigned MatrixLED.displayEnabled;                    // Display enabled byte - set to 1 to enable the display
pin io unsigned MatrixLED.rows;                              // Number of Matrix LED rows
pin io unsigned MatrixLED.columns;                           // Number of Matrix LED columns
pin io unsigned MatrixLED.RefreshFlag;                       // Flag for refreshing data - set to 1 to refresh the display
pin io unsigned MatrixLED.#.data [8];                           // Matrix LED buffer - one byte per row (assumes 8 columns)

pin io unsigned LCD.Configuration;                     // LCD configuration byte - 0: disabled, 1: enabled on primary pins, 2: enabled on secondary pins
pin io unsigned LCD.Rows;                              // Number of LCD module rows
pin io unsigned LCD.Columns;                           // Number of LCD module columns
pin io unsigned LCD.RowRefreshFlags;                   // Flag for refreshing data - bit 0: row 1, bit 1: row 2, bit 2: row 3, bit 3: row 4
    
pin io unsigned LCD.line1.# [20];                         // Line 1 buffer
pin io unsigned LCD.line2.# [20];                         // Line 2 buffer
pin io unsigned LCD.line3.# [20];                         // Line 3 buffer
pin io unsigned LCD.line4.# [20];                         // Line 4 buffer
*/
pin out unsigned I2C.#.presentDevices [16];
pin io bit I2C.scanning ;

license "GPL";

option extra_link_args "-lPoKeys";

;;

#include <unistd.h>   /* UNIX standard function definitions */
#include "PoKeysLib.h"



sPoKeysDevice * dev=0;

bool HAL_Machine_On = false;
bool setPkConfig = false;
bool setPinConfig = false;
bool secBlink = false;
unsigned Loop_Frequ =0;
uint8_t kbd48CNC_Counter[48];
uint8_t rtc_latencycheck_set = 0;
int rtc_latencyCounter = 0;
int i=0;







// Pulse engine v2 information
typedef struct
{

	bool         SoftLimitStatus[8];           //soft-limit statuses per axes
	bool         AxisEnabledStates[8];     // Bit-mapped states, where axis enabled and charge pump signals are active
	uint8_t         AxisEnabledStatesMask;     // Bit-mapped states, where axis enabled and charge pump signals are active
	uint8_t         LimitOverride;             // Limit override status
	uint8_t         EmergencySwitchPolarity;   // Emergency switch polarity (set to 1 to invert)


	// Basic engine states
    uint8_t         PulseEngineEnabled;        // Pulse engine enabled status, also number of enabled axes
    uint8_t         PulseEngineActivated;      // Pulse engine activation status
    uint8_t         PulseEngineState;			 // State of pulse engine - see ePoKeysPEState
    uint8_t         ChargePumpEnabled;         // Charge pump output enabled
    uint8_t         PulseGeneratorType;        // Pulse engine generator type (0: external, 1: internal 3ch)

	// Switch states
    bool         LimitStatusP[8];              // Limit+ status (NOT bit-mapped)
    bool         LimitStatusN[8];              // Limit- status (NOT bit-mapped)
    bool         HomeStatus[8];                // Home status (bit-mapped)
	bool         EmergencyStatus;                // Home status 0=ok 1=Emergency

    uint8_t         AxesState[8];              // Axis states (bit-mapped) - see ePK_PEAxisState
    int32_t         CurrentPosition[8];        // Current position

	// Engine info
    uint8_t nrOfAxes;
    uint8_t maxPulseFrequency;
    uint8_t bufferDepth;
    uint8_t slotTiming;

	// Other inputs
    uint8_t         ErrorInputStatus;          // Stepper motor driver error inputs status (bit-mapped)
    uint8_t         MiscInputStatus;           // Miscelenous digital inputs...

	bool			DedicatedLimitNInputs[8];
	bool			DedicatedLimitPInputs[8];
	bool			DedicatedHomeInputs[8];

	uint8_t         ExternalRelayOutputs;      // External relay outputs
    uint8_t         ExternalOCOutputs;         // External open-collector outputs

	int32_t         PositionSetup[8];          // Position to be set as current position
    int32_t         ReferencePositionSpeed[8]; // Reference position or speed (position or pulses/s)
	
	// Cyclic Setting
	float			joint_vel_cmd[8];			// SetVelocity
	float			joint_pos_cmd[8];			// SetVelocity
	float			joint_vel_fb[8];			// GetVelocity
	float			joint_pos_fb[8];			// SetVelocity

	uint8_t         PulseEngineStateSetup;     // Pulse engine new state configuration
	uint8_t         LimitOverrideSetup;        // Limit override configuration
	uint8_t         AxisLimitOverride[8];

	bool         ExternalRelayOutput[8] ;      // External relay outputs
    bool         ExternalOCOutput[8];         // External open-collector outputs

} PK_PEv2_Stat;




typedef struct
{
// PK_PEv2_PulseEngineSetup   
    uint8_t         PulseEngineEnabled;        // Pulse engine enabled status, also number of enabled axes
    uint8_t         ChargePumpEnabled;         // Charge pump output enabled
    uint8_t         PulseGeneratorType;        // Pulse engine generator type (0: external, 1: internal 3ch)
	uint8_t			PG_swap_stepdir;
	uint8_t			PG_extended_io;
    //dev->request[11] = dev->PEv2.PulseEngineBufferSize;
    uint8_t         EmergencySwitchPolarity;   // Emergency switch polarity (set to 1 to invert)
    //dev->request[13] = dev->PEv2.AxisEnabledStatesMask;
	
	
//PK_PEv2_AxisConfigurationGet
   // Read the structure
    uint8_t         AxesConfig[8];             // Axis configuration - see ePK_PEv2_AxisConfig
    uint8_t         AxesSwitchConfig[8];       // Axis switch configuration - see ePK_PEv2_AxisSwitchOptions

    uint8_t         PinHomeSwitch[8];          // Home switch pin (0 for external dedicated input)
    uint8_t         PinLimitMSwitch[8];        // Limit- switch pin (0 for external dedicated input)
    uint8_t         PinLimitPSwitch[8];        // Limit+ switch pin (0 for external dedicated input)

    uint8_t         HomingSpeed[8];            // Homing speed per axis (in %)
    uint8_t         HomingReturnSpeed[8];      // Homing return speed per axis (in % of the homing speed)

    uint8_t         MPGjogEncoder[8];          // MPG jog encoder ID

    // Convert parameters... assume little-endian format
    float           MaxSpeed[8];               // Maximum axis speed (in pulses per ms)
    float           MaxAcceleration[8];        // Maximum axis acceleration (in pulses/ms/ms)
    float           MaxDecceleration[8];       // Maximum axis deceleration (in pulses/ms/ms)

    int32_t         SoftLimitMaximum[8];       // Soft limit maximum position
    int32_t         SoftLimitMinimum[8];       // Soft limit minimum position

    int32_t         MPGjogMultiplier[8];       // MPG jog multiplier value

    uint8_t         AxisEnableOutputPins[8];   // Axis enabled output pin (0 for external dedicated output)
    int8_t          InvertAxisEnable[8];       // Invert axis enable signal

	uint8_t			FilterLimitMSwitch[8];	   // Digital filter for limit- switch
	uint8_t			FilterLimitPSwitch[8];	   // Digital filter for limit+ switch
	uint8_t			FilterHomeSwitch[8];	   // Digital filter for home switch

	int32_t         HomeOffsets[8];            // Home position offset
	uint8_t			HomingAlgorithm[8];		   // Homing algorithm configuration
	// MPG 1x mode here
	uint32_t		HomeBackOffDistance[8];	   // Back-off distance after homing
    uint16_t        MPGjogDivider[8];          // Divider for the MPG jogging (enhanced encoder resolution)

// 	uint16_t		BacklashWidth[8];			// half of real backlash width
	int16_t			BacklashRegister[8];		// current value of the backlash register
	uint8_t			BacklashAcceleration[8];	// in pulses per ms^2
	uint8_t			BacklashCompensationEnabled;

// add pins
    uint8_t         ProbeInput;                // Probe input (0:disabled, 1-8:external inputs, 9+ Pin ID-9)
    uint8_t         ProbeInputPolarity;        // Probe input polarity

//int32_t PK_PEv2_AdditionalParametersGet(sPoKeysDevice * device)
//int32_t PK_PEv2_AdditionalParametersSet(sPoKeysDevice * device)
    uint8_t         EmergencyInputPin;
    uint8_t         EmergencyOutputPin;

// HAL-INI parameters
		bool ApplyIniSettings;
		char COORDINATES;
		char LINEAR_UNITS;
		char ANGULAR_UNITS;
		float CYCLE_TIME;
		float DEFAULT_LINEAR_VELOCITY;
		float MAX_LINEAR_VELOCITY;
		char TYPE [8];			//LINEAR / ANGULAR
		float HOME [8];
		float FERROR [8];
		float MIN_FERROR [8];
		float MAX_VELOCITY [8];
		float MAX_ACCELERATION [8];
		float STEPGEN_MAXVEL [8];
		float STEPGEN_MAXACCEL [8];
		uint Feedback_Encoder_Id [8];
//		float P [8];
//		float I [8];
//		float D [8];
//		float FF0 [8];
//		float FF1 [8];
//		float FF2 [8];
//		float BIAS [8];
		float DEADBAND [8];
		float MAX_OUTPUT [8];
		float ENCODER_SCALE [8];
// these are in nanoseconds
		float DIRSETUP [8];					// (ns)
		float DIRHOLD [8];					// (ns)
		float STEPLEN [8];					// (ns)
		float STEPSPACE [8];					// (ns)
		float STEP_SCALE [8];					// (pulses/mm)
		float MIN_LIMIT [8];					// position (mm)
		float MAX_LIMIT [8];					// position (mm)
		float HOME_OFFSET [8];				// position (mm)
		float HOME_SEARCH_VEL [8];			// position (mm)
		float HOME_LATCH_VEL [8];				// position (mm)
		float HOME_FINAL_VEL [8];				// position (mm)
		bool HOME_USE_INDEX [8];				// YES / NO
		bool HOME_IGNORE_LIMITS [8];			// YES / NO
} PK_PEv2_Parameters;

typedef struct
{
    uint8_t matrixKBconfiguration;             // Matrix keyboard configuration (set to 1 to enable matrix keyboard support)
    uint8_t matrixKBwidth;                     // Matrix keyboard width (number of columns)
    uint8_t matrixKBheight;                    // Matrix keyboard height (number of rows)
    uint8_t reserved[5];                       // placeholder
    uint8_t matrixKBcolumnsPins[8];            // List of matrix keyboard column connections
    uint8_t matrixKBrowsPins[16];              // List of matrix keyboard row connections
    uint8_t macroMappingOptions[128];          // Selects between direct key mapping and mapping to macro sequence for each key (assumes fixed width of 8 columns)
//    uint8_t keyMappingKeyCode[128];            // USB keyboard key code for each key (assumes fixed width of 8 columns), also down key code in triggered mapping mode
//    uint8_t keyMappingKeyModifier[128];        // USB keyboard key modifier, also down key modifier in triggered mapping mode (assumes fixed width of 8 columns)
//    uint8_t keyMappingTriggeredKey[128];       // Selects between normal direct key mapping and triggered key mapping for each key (assumes fixed width of 8 columns)
//    uint8_t keyMappingKeyCodeUp[128];          // USB keyboard up key code in triggered mapping mode (assumes fixed width of 8 columns)
//    uint8_t keyMappingKeyModifierUp[128];      // USB keyboard up key modifier in triggered mapping mode (assumes fixed width of 8 columns)
//    uint8_t matrixKBvalues[128];               // Current state of each matrix keyboard key (assumes fixed width of 8 columns)
//	bool matrixKBPins[128];               // extracted from matrixKBrowsPins

} PK_MatrixKB_Parameters;

PK_PEv2_Parameters PEv2_params;
PK_PEv2_Stat PEv2_status;
sPoKeysEasySensor EasySensors[100];
sPoNETmodule PoNet[16];

uint8_t PoExtBus_DataSet[10];
uint8_t PoExtBus_DataGet[10];

unsigned int  sleepdur = 2000;

PK_MatrixKB_Parameters MatrixKB;

bool Get_BitOfByte(uint8_t in_Byte, int Bit_Id)
{
	return (in_Byte >> Bit_Id) & 1;
}

uint8_t Set_BitOfByte(uint8_t in_Byte, int Bit_Id, bool value)
{

	if (value == true)
	{
		in_Byte |= 1 << Bit_Id;
	}
	else
	{
		in_Byte &= ~(1 << Bit_Id);
	}
	return in_Byte;

}

uint8_t Merge_8BitsToByte(bool Bit_array[8])
{
  uint8_t sum = 0;
  for (int i = 0; i < 8; i++)
  {
//    sum += Bit_array[i] - '0';
//    sum<<=1;

	if (Bit_array[i]  == true)
	{
		sum |= 1 << i;
	}
	else
	{
		sum &= ~(1 << i);
	}

  }
  return sum;
}


int Update_PoNet()
{
	if(PK_PoNETGetPoNETStatus(dev) == PK_OK)
	{
		usleep(sleepdur); 
	}
	if(PK_PoNETGetModuleSettings(dev) == PK_OK)
	{
		usleep(sleepdur); 	
	}
	return 0;
}



int Config_MatrixKB()
{

/* no setting
dev->matrixKB.matrixKBconfiguration = 1;
dev->matrixKB.matrixKBheight = 6;
dev->matrixKB.matrixKBwidth = 8;
PK_MatrixKBConfigurationSet(dev);
PK_MatrixKBConfigurationSet(dev);*/

	if(PK_MatrixKBConfigurationGet(dev) == PK_OK)
	{
			MatrixKB.matrixKBconfiguration = dev->matrixKB.matrixKBconfiguration;
			MatrixKB.matrixKBwidth = dev->matrixKB.matrixKBheight;
			MatrixKB.matrixKBheight = dev->matrixKB.matrixKBwidth;

			for (i = 0; i < 8; i++)
			{
				MatrixKB.matrixKBrowsPins[i] = dev->matrixKB.matrixKBrowsPins[i];
				MatrixKB.matrixKBrowsPins[8+i] = dev->matrixKB.matrixKBrowsPins[8 + i];

				MatrixKB.matrixKBcolumnsPins[i] = dev->matrixKB.matrixKBcolumnsPins[i];
			}

			for (i = 0; i < 128; i++)
			{
				MatrixKB.macroMappingOptions[i]  = dev->matrixKB.macroMappingOptions[i];
			}
			usleep(sleepdur); 

	}

	//PK_MatrixKBConfigurationSet
	return 0;
}

int Update_MatrixKB()
{
	if(PK_MatrixKBStatusGet(dev) == PK_OK)
	{
		for (i = 0; i < 128; i++)
		{
		//	MatrixKB.matrixKBvalues[i] = dev->matrixKB.matrixKBvalues[i];
		}
		usleep(sleepdur); 
	}
	if(PK_MatrixKBConfigurationGet(dev) == PK_OK)
	{
		int i =0;
		int h =0;
		int w =0;
		for (h = 0; h < MatrixKB.matrixKBheight; h++)
		{
			for (w = 0; w < MatrixKB.matrixKBwidth; w++)
			{
			//	MatrixKB.matrixKBPins[i] = Get_BitOfByte(MatrixKB.matrixKBrowsPins[h], w);
				i++;
			}
		}
		usleep(sleepdur); 
	}
	return 0;
}




/*int Config_LCD()
{
	if(PK_LCDConfigurationGet(dev) == PK_OK)
	{
		usleep(sleepdur); 
	}

	//PK_LCDConfigurationSet
	// Change between modes PK_LCD_MODE_DIRECT and PK_LCD_MODE_BUFFERED
	// PK_LCDChangeMode(dev, PK_LCD_MODE_DIRECT);
	return 0;
}*/

int Update_LCD()
{
	if(PK_LCDUpdate(dev) == PK_OK)
	{
		usleep(sleepdur); 
	}
	if(PK_LCDSetCustomCharacters(dev) == PK_OK)
	{
		usleep(sleepdur); 
	}
	return 0;
}

int Config_EasySensors()
{
	if(PK_EasySensorsSetupGet(dev) == PK_OK)
	{
		for (i = 0; i < dev->info.iEasySensors; i++)
		{
			EasySensors[i] = dev->EasySensors[i];
		}
		usleep(sleepdur); 
	}

	//PK_EasySensorsSetupSet
	return 0;
}

int Update_EasySensors()
{
	if(PK_EasySensorsValueGetAll(dev) == PK_OK)
	{
		for (i = 0; i < dev->info.iEasySensors; i++)
		{
			EasySensors[i] = dev->EasySensors[i];
		}
		usleep(sleepdur); 
	}
	return 0;
}


int Config_PoStep()
{
	if(PK_PoStep_ConfigurationGet(dev) == PK_OK)
	{
		usleep(sleepdur); 
	}

	if(PK_PoStep_DriverConfigurationGet(dev) == PK_OK)
	{
		usleep(sleepdur); 
	}

	//PK_PoStep_ConfigurationSet(dev)
	//PK_PoStep_DriverConfigurationSet(dev)
	return 0;
}

int Update_PoStep()
{
	if(PK_PoStep_StatusGet(dev) == PK_OK)
	{
		usleep(sleepdur); 
	}
	return 0;
}





void user_mainloop(void) 
{ 


    while(0xb){
       FOR_ALL_INSTS() {
	   rtc_loopcount++;
	   HAL_Machine_On = machine_is_on;

	   a_debout = 100;
	 while(dev == NULL)
	 {
			
			Loop_Frequ = rtc_loop_frequ;
			if (PEv2_EmergencyOutput!=true)
			{
				usleep(sleepdur); 
			}

		 if (devSerial!=0 )
		 {
				a_debout = 110;
				enum_usb_dev = PK_EnumerateUSBDevices();
				a_debout = 111;
				if (enum_usb_dev != 0)
				{
					dev = PK_ConnectToDeviceWSerial(devSerial, 5000);  //waits for usb device
				}
				
				a_debout = 112;
				if (dev == NULL)
				{
					a_debout = 113;
					dev = PK_ConnectToDeviceWSerial_UDP(devSerial, 5000);  //waits for usb device
				}

		 }
		else
		{
			a_debout = 114;
			sPoKeysNetworkDeviceSummary * udp_devices;
			enum_usb_dev = PK_EnumerateUSBDevices();
			enum_fusb_dev = PK_EnumerateUSBDevices();
			//enum_udp_dev = PK_EnumerateNetworkDevices(udp_devices,180); // does not work - it hangs here

		}

		if(dev != NULL)
		{
			a_debout = 121;
			connected=1;

			 info_PinCount = dev->info.iPinCount;                        // Number of pins, physically on the device
			 info_PWMCount = dev->info.iPWMCount;                        // Number of pins that support PWM output
			 info_BasicEncoderCount = dev->info.iBasicEncoderCount;               // Number of basic encoders
			 info_EncodersCount = dev->info.iEncodersCount;                   // Number of encoder slots available
			 info_FastEncoders = dev->info.iFastEncoders;                    // Number of fast encoders supported
			 info_UltraFastEncoders = dev->info.iUltraFastEncoders;               // Number of available ultra fast encoders
			 info_PWMinternalFrequency = dev->info.PWMinternalFrequency;             // Main PWM peripheral clock
			 info_AnalogInputs = dev->info.iAnalogInputs;                    // Number of available analog inputs
			 info_KeyMapping = dev->info.iKeyMapping;                      // Device supports key mapping (acts as a USB keyboard)
			 info_TriggeredKeyMapping = dev->info.iTriggeredKeyMapping;             // Device supports triggered key mapping
			 info_KeyRepeatDelay = dev->info.iKeyRepeatDelay;                  // Device supports user customizable key repeat rates and delays
			 info_DigitalCounters = dev->info.iDigitalCounters;                 // Device supports digital counters
			 info_JoystickButtonAxisMapping = dev->info.iJoystickButtonAxisMapping;       // Device supports mapping of joystick buttons
			 info_JoystickAnalogToDigitalMapping = dev->info.iJoystickAnalogToDigitalMapping;  // Device supports mapping of analog inputs to digital keys
			 info_Macros = dev->info.iMacros;                          // Device supports customizable macro sequences
			 info_MatrixKeyboard = dev->info.iMatrixKeyboard;                  // Device supports matrix keyboard
			 info_MatrixKeyboardTriggeredMapping = dev->info.iMatrixKeyboardTriggeredMapping;  // Device supports matrix keyboard triggered key mapping
			 info_LCD = dev->info.iLCD;                             // Device supports alphanumeric LCD display
			 info_MatrixLED = dev->info.iMatrixLED;                       // Device supports matrix LED display
			 info_ConnectionSignal = dev->info.iConnectionSignal;                // Device supports connection signal output
			 info_PoExtBus = dev->info.iPoExtBus;                        // Device supports PoExtBus digital outputs
			 info_PoNET = dev->info.iPoNET;                           // Device supports PoNET bus devices
			 info_AnalogFiltering = dev->info.iAnalogFiltering;                 // Device supports analog inputs low-pass digital filtering
			 info_InitOutputsStart = dev->info.iInitOutputsStart;                // Device supports initializing outputs at startup
			 info_protI2C = dev->info.iprotI2C;                         // Device supports I2C bus (master)
			 info_prot1wire = dev->info.iprot1wire;                       // Device supports 1-wire bus (master)
			 info_AdditionalOptions = dev->info.iAdditionalOptions;               // Device supports additional options with activation keys
			 info_LoadStatus = dev->info.iLoadStatus;                      // Device supports reporting load status
			 info_CustomDeviceName = dev->info.iCustomDeviceName;                // Device supports specifying custom device names
			 info_PoTLog27support = dev->info.iPoTLog27support;                 // Device supports PoTLog27 firmware
			 info_SensorList = dev->info.iSensorList;                      // Device supports sensor lists
			 info_WebInterface = dev->info.iWebInterface;                    // Device supports web interface
			 info_FailSafeSettings = dev->info.iFailSafeSettings;                // Device supports fail-safe mode
			 info_JoystickHATswitch = dev->info.iJoystickHATswitch;               // Device supports joystick HAT switch mapping
			 info_PulseEngine = dev->info.iPulseEngine;                     // Device supports Pulse engine
			 info_PulseEnginev2 = dev->info.iPulseEnginev2;                   // Device supports Pulse engine v2
			 info_EasySensors = dev->info.iEasySensors;                     // Device supports EasySensors

			 a_debout = 122;
			 if ( info_PoNET!=0 )
			 {
				a_debout = 123;
				
				for (i = 0; i < 16; i++)
				{

						dev->PoNETmodule.moduleID = i;
						if(PK_PoNETGetModuleSettings(dev) == PK_OK)
						{
							PoNET_moduleID(i) = i;
							PoNET_i2cAddress(i) = dev->PoNETmodule.i2cAddress;
							PoNET_moduleType(i) = dev->PoNETmodule.moduleType;
							PoNET_moduleSize(i) = dev->PoNETmodule.moduleSize;
							PoNET_moduleOptions(i) = dev->PoNETmodule.moduleOptions;

							if (PoNET_moduleType(i)== 0x10 && kbd48CNC_available ==0)
							{
								if ((dev->PoNETmodule.moduleOptions & (1<<7)) != 0)
								{
									// kbd48CNC is also detected
									kbd48CNC_PoNetID = i;
									//dev->deviceConfig->iEnableKBD48CNC = 1;
									kbd48CNC_available=1;
								}
							}
							usleep(sleepdur); 
						}
				}
			 }
			 a_debout = 124;

			 if (kbd48CNC_available!=0)
			 {
				for (i = 0; i < 48; i++)
				{
					int offset[] = {15, 8, 7, 0};

					int top = (i & 0xF0) + offset[(i / 4) % 4];
					int y = i % 4;
					int ID = top + y;
					if (((i / 4) % 2) == 0)
						ID = top - y;
				}
			 }

			 if ( info_PulseEnginev2!=0 )
			 {
				if (PEv2_params_ApplyIniSettings!=0)
				{
					//dev->PEv2.AxisEnabledStatesMask=0; //Disable axis power when not in Running state
					//PK_PEv2_PulseEngineSetup(dev);
					usleep(sleepdur); 
					if ( PEv2_ProbeInput!=0)
					{	//check if pin is parametrized in HAL
						dev->Pins[ PEv2_ProbeInput-1].PinFunction = PK_PinCap_digitalInput;
						PK_SL_SetPinFunction(dev, PEv2_ProbeInput-1, PK_PinCap_digitalInput);
						setPinConfig=true;
					}

					dev->PEv2.EmergencyInputPin = PEv2_EmergencyInputPin;
					if ( PEv2_EmergencyInputPin!=0)
					{	//check if pin is parametrized in HAL
						dev->Pins[PEv2_EmergencyInputPin-1].PinFunction = PK_PinCap_digitalInput;
						PK_SL_SetPinFunction(dev, PEv2_EmergencyInputPin-1, PK_PinCap_digitalInput);
						setPinConfig=true;
					}
					if ( PEv2_EmergencyOutputPin!=0)
					{	//check if pin is parametrized in HAL
						dev->Pins[ PEv2_EmergencyOutputPin-1].PinFunction = PK_PinCap_digitalOutput;
						PK_SL_SetPinFunction(dev, PEv2_EmergencyInputPin-1, PK_PinCap_digitalOutput);
						setPinConfig=true;
					}
					PEv2_PulseEngineEnabled = dev->PEv2.info.nrOfAxes;
					if (PEv2_PulseEngineEnabled != 0)
					{
						dev->PEv2.PulseEngineActivated=1;
					}
					if (PEv2_PulseGeneratorType == 0)
					{
						
						if (PEv2_PG_swap_stepdir != 0)
						{
							PEv2_PulseGeneratorType=Set_BitOfByte(PEv2_PulseGeneratorType, 6, true); // swap step / dir signals
						}
						if (PEv2_PG_extended_io != 0)
						{
							PEv2_PulseGeneratorType=Set_BitOfByte(PEv2_PulseGeneratorType, 7, true); // extended io
						}
					}
					dev->PEv2.PulseEngineEnabled = PEv2_PulseEngineEnabled;
					dev->PEv2.ChargePumpEnabled = PEv2_ChargePumpEnabled;
					dev->PEv2.PulseGeneratorType = PEv2_PulseGeneratorType;
					//dev->PEv2.PulseEngineBufferSize = PEv2_params_PulseEngineBufferSize;
					dev->PEv2.EmergencySwitchPolarity = PEv2_EmergencySwitchPolarity;
					//dev->PEv2.AxisEnabledStatesMask = PEv2_params_AxisEnabledStatesMask;
					if(PK_PEv2_PulseEngineSetup(dev) != PK_OK)
					{
						usleep(sleepdur); 
					}
					if(PK_PEv2_PulseEngineSetup(dev) == PK_OK)
					{
						usleep(sleepdur); 
						PK_PEv2_PulseEngineReboot(dev);
						usleep(1000000); 
					}
				}
				else
				{
					if(PK_PEv2_StatusGet(dev) == PK_OK && PK_PEv2_Status2Get(dev) == PK_OK )
					{
						PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
						PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
						PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;
						//PEv2_params_PulseEngineBufferSize = dev->PEv2.PulseEngineBufferSize;
						PEv2_EmergencySwitchPolarity = dev->PEv2.EmergencySwitchPolarity;
						//PEv2_params_AxisEnabledStatesMask = dev->PEv2.AxisEnabledStatesMask;

						PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
						PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
						PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;
						//PEv2_params_PulseEngineBufferSize = dev->PEv2.PulseEngineBufferSize;
						PEv2_EmergencySwitchPolarity = dev->PEv2.EmergencySwitchPolarity;
						//PEv2_params_AxisEnabledStatesMask = dev->PEv2.AxisEnabledStatesMask;
					}

					if(PK_PEv2_AdditionalParametersGet(dev) == PK_OK)
					{
						PEv2_EmergencyInputPin = dev->PEv2.EmergencyInputPin;   
						usleep(sleepdur); 
					}	
				}

				uint8_t AxesConfig[8];
				uint8_t AxesSwitchConfig[8];
				for (i = 0; i < dev->PEv2.info.nrOfAxes; i++)
				{
						if (PEv2_params_ApplyIniSettings!=0)
						{
							// Convert mm/s -> pulses/s
							if(PEv2_params_STEP_SCALE(i)!=0)
							{
								PEv2_MaxSpeed(i) = PEv2_params_STEPGEN_MAXVEL(i)* PEv2_params_STEP_SCALE(i)/1000;				// Maximum axis speed convert (mm/s) to (pulses per ms)
								PEv2_MaxAcceleration(i) = PEv2_params_STEPGEN_MAXACCEL(i)* PEv2_params_STEP_SCALE(i) /1000000;	// Maximum axis acceleration convert (mm/s�) to (in pulses/ms/ms)
								PEv2_MaxDecceleration(i) = PEv2_params_STEPGEN_MAXACCEL(i)* PEv2_params_STEP_SCALE(i) /1000000;	// Maximum axis deceleration convert (mm/s�) to (in pulses/ms/ms)

								PEv2_SoftLimitMaximum(i) = PEv2_params_MAX_LIMIT(i)* PEv2_params_STEP_SCALE(i);		// Soft limit maximum position
								PEv2_SoftLimitMinimum(i) = PEv2_params_MIN_LIMIT(i)* PEv2_params_STEP_SCALE(i);		// Soft limit minimum position
								PEv2_HomeOffsets(i) = PEv2_params_HOME_OFFSET(i)* PEv2_params_STEP_SCALE(i);		// Home position offset

								PEv2_HomingSpeed(i) = PEv2_params_HOME_SEARCH_VEL(i)*100/ PEv2_params_MAX_VELOCITY(i) ;			// Homing speed per axis (in %)
								PEv2_HomingReturnSpeed(i) = PEv2_params_HOME_LATCH_VEL(i)*100/ PEv2_params_HOME_SEARCH_VEL(i);	// Homing return speed per axis (in % of the homing speed)

								// Convert parameters... assume little-endian format
								dev->PEv2.MaxSpeed[i] = PEv2_MaxSpeed(i);
								dev->PEv2.MaxAcceleration[i] = PEv2_MaxAcceleration(i);
								dev->PEv2.MaxDecceleration[i] = PEv2_MaxDecceleration(i);

								dev->PEv2.SoftLimitMinimum[i] = PEv2_SoftLimitMinimum(i);
								dev->PEv2.SoftLimitMaximum[i] = PEv2_SoftLimitMaximum(i);
								dev->PEv2.HomeOffsets[i] = PEv2_HomeOffsets(i);

								dev->PEv2.HomingSpeed[i] = PEv2_HomingSpeed(i);
								dev->PEv2.HomingReturnSpeed[i] = PEv2_HomingReturnSpeed(i);

							}

							 AxesConfig[i]=0;
							// Read the structure
							/*
								PK_AC_ENABLED            = (1 << 0),       // 1 Axis enabled 
								PK_AC_INVERTED           = (1 << 1),       // 2 Axis inverted
								PK_AC_INTERNAL_PLANNER   = (1 << 2),       // 4 Axis uses internal motion planner
								PK_AC_POSITION_MODE      = (1 << 3),       // 8 Internal motion planner for this axis is in position mode
								PK_AC_INVERTED_HOME      = (1 << 4),       // 16 Axis homing direction is inverted
								PK_AC_SOFT_LIMIT_ENABLED = (1 << 5),       // 32 Use soft-limits for this axis
								PK_AC_ENABLED_MASKED     = (1 << 7)        // 128 Use output enable pin masking
							*/
							//PEv2_params_AxesConfig(i) = PK_AC_ENABLED_MASKED;
							if (PEv2_params_STEPGEN_MAXVEL(i) !=0)
							{
								AxesConfig[i] =Set_BitOfByte(AxesConfig[i] , 0, true);  // PK_AC_ENABLED ;
								AxesConfig[i] =Set_BitOfByte(AxesConfig[i] , 2, true);  // PK_AC_INTERNAL_PLANNER;

							}

				
							if (PEv2_params_Feedback_Encoder_Id(i) = 0)
							{
								AxesConfig[i] =Set_BitOfByte(AxesConfig[i] , 3, true);  // PK_AC_POSITION_MODE;
							}
				

							/*
								PK_ASO_SWITCH_LIMIT_N        = (1 << 0),   // 1 Limit- switch
								PK_ASO_SWITCH_LIMIT_P        = (1 << 1),   // 2 Limit+ switch
								PK_ASO_SWITCH_HOME           = (1 << 2),   // 4 Home switch
								PK_ASO_SWITCH_COMBINED_LN_H  = (1 << 3),   // 8 Home switch is shared with Limit- switch
								PK_ASO_SWITCH_COMBINED_LP_H  = (1 << 4),   // 16 Home switch is shared with Limit+ switch
								PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5),   // 32 Invert limit- switch polarity
								PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6),   // 64 Invert limit+ switch polarity
								PK_ASO_SWITCH_INVERT_HOME    = (1 << 7)    // 128 Invert home switch polarity
							*/
							AxesSwitchConfig[i] = 0;
				
							if (PEv2_SoftLimitMaximum(i) !=0 | PEv2_SoftLimitMinimum(i) !=0)
							{
								AxesConfig[i] =Set_BitOfByte(AxesConfig[i] , 3, true); // PK_AC_SOFT_LIMIT_ENABLED;
							}

				
							if ( PEv2_PinHomeSwitch(i)!=0)
							{	
							//check if pin is parametrized in HAL
								if ( PEv2_PinHomeSwitch(i)!=PEv2_PinLimitMSwitch(i) && PEv2_PinHomeSwitch(i)!=PEv2_PinLimitPSwitch(i))
								{
									dev->PEv2.PinHomeSwitch[i] = PEv2_PinHomeSwitch(i);
								}
								else if ( PEv2_PinHomeSwitch(i)==PEv2_PinLimitPSwitch(i))
								{
									//dev->PEv2.PinHomeSwitch[i]=0;
									AxesSwitchConfig[i] = Set_BitOfByte(AxesSwitchConfig[i] , 4, true); // 16 Home switch is shared with Limit+ switch
								}
								else if ( PEv2_PinHomeSwitch(i)==PEv2_PinLimitMSwitch(i))
								{
									//dev->PEv2.PinHomeSwitch(i)=0;
									AxesSwitchConfig[i] = Set_BitOfByte(AxesSwitchConfig[i] , 3, true); // 8 Home switch is shared with Limit- switch
				
								}
								AxesSwitchConfig[i] = Set_BitOfByte(AxesSwitchConfig[i] , 2, true); // | PK_ASO_SWITCH_HOME;
								dev->Pins[ PEv2_PinHomeSwitch(i)-1].PinFunction = PK_PinCap_digitalInput;
								PK_SL_SetPinFunction(dev, PEv2_PinHomeSwitch(i)-1, PK_PinCap_digitalInput);
								setPinConfig=true;
							}
							else
							{
								dev->PEv2.PinHomeSwitch[i]=0;
							}

							dev->PEv2.PinLimitMSwitch[i] = PEv2_PinLimitMSwitch(i);
							if ( PEv2_PinLimitMSwitch(i)!=0)
							{	//check if pin is parametrized in HAL
								AxesSwitchConfig[i] = Set_BitOfByte(AxesSwitchConfig[i] , 0, true); // | PK_ASO_SWITCH_LIMIT_N;
								dev->Pins[ PEv2_PinLimitMSwitch(i)-1].PinFunction = PK_PinCap_digitalInput;
								PK_SL_SetPinFunction(dev, PEv2_PinLimitMSwitch(i)-1, PK_PinCap_digitalInput);

								AxesSwitchConfig[i] = Set_BitOfByte(AxesSwitchConfig[i] , 5, true); // 32 Invert limit- switch polarity

								setPinConfig=true;
							}

							dev->PEv2.PinLimitPSwitch[i] = PEv2_PinLimitPSwitch(i);
							if ( PEv2_PinLimitPSwitch(i)!=0)
							{	//check if pin is parametrized in HAL
								AxesSwitchConfig[i] = Set_BitOfByte(AxesSwitchConfig[i] , 1, true); // | PK_ASO_SWITCH_LIMIT_P;
								dev->Pins[ PEv2_PinLimitPSwitch(i)-1].PinFunction = PK_PinCap_digitalInput;
								PK_SL_SetPinFunction(dev, PEv2_PinLimitPSwitch(i)-1, PK_PinCap_digitalInput);

								AxesSwitchConfig[i] = Set_BitOfByte(AxesSwitchConfig[i] , 6, true); // 32 Invert limit+ switch polarity

								setPinConfig=true;
							}

							dev->PEv2.HomingSpeed[i] = PEv2_HomingSpeed(i);
							dev->PEv2.HomingReturnSpeed[i] = PEv2_HomingReturnSpeed(i);

							dev->PEv2.MPGjogEncoder[i] = PEv2_MPGjogEncoder(i);



							dev->PEv2.MPGjogMultiplier[i] = PEv2_MPGjogMultiplier(i);

							dev->PEv2.AxisEnableOutputPins[i] = PEv2_AxisEnableOutputPins(i);
							if ( PEv2_AxisEnableOutputPins(i)!=0)
							{	//check if pin is parametrized in HAL
								dev->Pins[ PEv2_AxisEnableOutputPins(i)-1].PinFunction = PK_PinCap_digitalOutput;
								PK_SL_SetPinFunction(dev, PEv2_AxisEnableOutputPins(i)-1, PK_PinCap_digitalOutput);
								setPinConfig=true;
							}
							
							dev->PEv2.InvertAxisEnable[i] = PEv2_InvertAxisEnable(i);

							dev->PEv2.FilterLimitMSwitch[i] = PEv2_FilterLimitMSwitch(i);
							dev->PEv2.FilterLimitPSwitch[i] = PEv2_FilterLimitPSwitch(i);
							dev->PEv2.FilterHomeSwitch[i] = PEv2_FilterHomeSwitch(i);

							dev->PEv2.HomingAlgorithm[i] = PEv2_HomingAlgorithm(i);
								// MPG 1x mode here
							dev->PEv2.HomeBackOffDistance[i] = PEv2_HomeBackOffDistance(i);
							dev->PEv2.MPGjogDivider[i] = PEv2_MPGjogDivider(i);	
							dev->PEv2.AxesConfig[i] =AxesConfig[i];
							//PEv2_AxesConfig(i) =AxesConfig[i]; Readonly!
							//PEv2_AxesSwitchConfig(i)=AxesSwitchConfig[i];
							dev->PEv2.AxesSwitchConfig[i] =AxesSwitchConfig[i];
							//dev->PEv2.AxesConfig[i] = 0;
							//dev->PEv2.AxesSwitchConfig[i] =0;

							dev->PEv2.param1 = i;
							if(PK_PEv2_AxisConfigurationSet(dev) != PK_OK)
							{
								usleep(sleepdur); 
								dev->PEv2.param1 = i;
								if(PK_PEv2_AxisConfigurationSet(dev) != PK_OK)
								{
									usleep(sleepdur); 
								}

							}
						}
						else
						{
							dev->PEv2.param1 = i;
							if(PK_PEv2_AxisConfigurationGet(dev) == PK_OK)
							{
								// Read the structure
								//PEv2_AxesConfig(i) = dev->PEv2.AxesConfig[i]; readonly
								//PEv2_AxesSwitchConfig(i)  = dev->PEv2.AxesSwitchConfig[i];

								PEv2_PinHomeSwitch(i)  = dev->PEv2.PinHomeSwitch[i];
								PEv2_PinLimitMSwitch(i)  = dev->PEv2.PinLimitMSwitch[i];
								PEv2_PinLimitPSwitch(i)  = dev->PEv2.PinLimitPSwitch[i];

								PEv2_HomingSpeed(i)  = dev->PEv2.HomingSpeed[i];
								PEv2_HomingReturnSpeed(i)  = dev->PEv2.HomingReturnSpeed[i];

								PEv2_MPGjogEncoder(i)  = dev->PEv2.MPGjogEncoder[i];

								// Convert parameters... assume little-endian format
								PEv2_MaxSpeed(i)  = dev->PEv2.MaxSpeed[i];
								PEv2_MaxAcceleration(i)  = dev->PEv2.MaxAcceleration[i];
								PEv2_MaxDecceleration(i)  = dev->PEv2.MaxDecceleration[i];

								PEv2_SoftLimitMinimum(i)  = dev->PEv2.SoftLimitMinimum[i];
								PEv2_SoftLimitMaximum(i)  = dev->PEv2.SoftLimitMaximum[i];

								PEv2_MPGjogMultiplier(i)  = dev->PEv2.MPGjogMultiplier[i];

								PEv2_AxisEnableOutputPins(i)  = dev->PEv2.AxisEnableOutputPins[i];
								PEv2_InvertAxisEnable(i)  = dev->PEv2.InvertAxisEnable[i];

								PEv2_FilterLimitMSwitch(i)  = dev->PEv2.FilterLimitMSwitch[i];
								PEv2_FilterLimitPSwitch(i)  = dev->PEv2.FilterLimitPSwitch[i];
								PEv2_FilterHomeSwitch(i)  = dev->PEv2.FilterHomeSwitch[i];

								PEv2_HomingAlgorithm(i)  = dev->PEv2.HomingAlgorithm[i];
								// MPG 1x mode here
								PEv2_HomeBackOffDistance(i)  = dev->PEv2.HomeBackOffDistance[i];
								PEv2_MPGjogDivider(i)  = dev->PEv2.MPGjogDivider[i];	
								usleep(sleepdur); 
							}
						}
					}
					//dev->PEv2.param1 = 0;
					if(PK_PEv2_StatusGet(dev) == PK_OK)
					{
						PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
						PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
						PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;
						//PEv2_params_PulseEngineBufferSize = dev->PEv2.PulseEngineBufferSize;
						PEv2_EmergencySwitchPolarity = dev->PEv2.EmergencySwitchPolarity;
						//PEv2_params_AxisEnabledStatesMask = dev->PEv2.AxisEnabledStatesMask;
					}

					if (setPinConfig==true)
					{
						if(PK_PinConfigurationSet(dev) != PK_OK)
						{
							usleep(sleepdur); 
							if(PK_PinConfigurationSet(dev) != PK_OK)
							{
								usleep(sleepdur); 
							}
							else
							{
								setPinConfig=false;
							}
						}
						else
						{
							setPinConfig=false;
						}
					}
			 }
			 
			 /*	if (info_MatrixKeyboard!=0)
			{
				dev->matrixKB.matrixKBconfiguration = 1;
				Config_MatrixKB;

			}*/
			/*if (info_EasySensors!=0)
			{
				Config_EasySensors;
				for (i = 0; i < dev->info.iEasySensors; i++)
				{
					EasySensors_sensorValue(i) = EasySensors[i].sensorValue;            // Current sensor value
					EasySensors_sensorType(i)= EasySensors[i].sensorType;             // Type of the sensor
					EasySensors_sensorRefreshPeriod(i)= EasySensors[i].sensorRefreshPeriod;    // Refresh period in 0.1s
					EasySensors_sensorFailsafeConfig(i)= EasySensors[i].sensorFailsafeConfig;   // Failsafe configuration (bits 0-5: timeout in seconds, bit 6: invalid=0, bit 7: invalid=0x7FFFFFFF)
					EasySensors_sensorReadingID(i)= EasySensors[i].sensorReadingID;        // Sensor reading selection (see Protocol description document for details)
					usleep(sleepdur); 
				}
			}*/

			if (info_protI2C!=0)
			{
				a_debout = 137;
				/* Pokeys Protokol Specification - page 65:  deprecated command, I2C bus is always activated

				uint8_t * i_activated;
				if (PK_I2SGetStatus(dev, i_activated) == PK_OK)
				{
					usleep(sleepdur); 
				} */


				a_debout = 138;
				if (PK_I2CBusScanStart(dev) == PK_OK)
				{
					I2C_scanning=true;
					usleep(sleepdur); 
				}
				a_debout = 139;
			}

			 if (setPinConfig==true)
			 {
				a_debout = 140;
				if (PK_PinConfigurationSet(dev) == PK_OK)
				{
					usleep(sleepdur); 
					setPinConfig = false;
				}
			 }
			 if (info_PoNET!=0)
			 {
				a_debout = 141;
				if(PK_PoNETGetPoNETStatus(dev) == PK_OK)
				{
					// device->PoNETmodule.PoNETstatus
				
					for (i = 0; i < 16; i++)
					{
						dev->PoNETmodule.moduleID = i;
						if(PK_PoNETGetModuleSettings(dev) == PK_OK)
						{
							PoNet[i].moduleID=i;
							PoNet[i].i2cAddress=dev->PoNETmodule.i2cAddress;
							PoNet[i].moduleType=dev->PoNETmodule.moduleType;
							PoNet[i].moduleSize=dev->PoNETmodule.moduleSize;
							PoNet[i].moduleOptions=dev->PoNETmodule.moduleOptions;
							usleep(sleepdur); 
						//	memset(dev->PoNETmodule.statusOut, 0, 10);
						}
					}
				}
				a_debout = 142;
				usleep(sleepdur); 
			 }
			 
			 
			 //dev->DeviceData.DeviceLockStatus=1;


			 if (PEv2_params_ApplyIniSettings !=0 && info_PulseEnginev2!=0 )
			{
				if (PK_SaveConfiguration(dev) != PK_OK)
				{
					usleep(sleepdur*2); 
					if (PK_SaveConfiguration(dev) != PK_OK)
					{
						usleep(sleepdur*2); 
					}
				}
				
			}
			 a_debout = 143;
		}
		else
		{
			connected=0;
			err=1;
		}
		usleep(sleepdur); 
	 }
	 alive=1; 

			a_debout = 200;
			 // RTC
			if (PK_RTCGet(dev) == PK_OK)
			{
				a_debout = 210;
				rtc_sec=dev->RTC.SEC;
				rtc_min=dev->RTC.MIN;
				rtc_hour=dev->RTC.HOUR;
				rtc_dow=dev->RTC.DOW;
				rtc_dom=dev->RTC.DOM;
				rtc_tmp=dev->RTC.tmp;
				rtc_doy=dev->RTC.DOY;
				rtc_month=dev->RTC.MONTH;
				rtc_year=dev->RTC.YEAR;

				if (rtc_lastsec!=rtc_sec)
				{
					a_debout = 211;
					rtc_loop_frequ=rtc_loopcount;
					rtc_loopcount=0;
					rtc_lastsec=rtc_sec;

					if (secBlink)
					{
						secBlink = false;
					}
					else
					{
						secBlink = true;
					}

					//sleepdur
					if (rtc_loop_frequ>5)
					{
						if (rtc_loop_frequ_demand == 0)
						{
							sleepdur = sleepdur*rtc_loop_frequ/10;
						}
						else
						{
							sleepdur = sleepdur*rtc_loop_frequ/rtc_loop_frequ_demand;
						}
					}
					else
					{
						sleepdur=sleepdur/2;
					}
					if (sleepdur>1000000)
					{
						sleepdur=1000000;
					}
					if (sleepdur<50)
					{
						sleepdur=50;
					}

					if (rtc_latencycheck_set = -1)
					{
						rtc_latencyCounter=0;
						rtc_latencycheck_set=dev->RTC.SEC;
					}
				}
				



				if (rtc_sec_ret >= rtc_latencycheck_set && rtc_latencycheck_set>0 )
				{
					rtc_hal_latency = rtc_latencyCounter;
					rtc_latencycheck_set = -1;
				}
				else if (rtc_latencycheck_set >= -0)
				{
					rtc_latencyCounter ++;
				}

				a_debout = 212;
			}

			if (info_protI2C!=0 && I2C_scanning!=false)
			{
				a_debout = 213; 
				/* it hangs here - but it is necessary to check which components are available (PoRelay8, ....)
					uint8_t* i_status; // = PK_I2C_STAT_IN_PROGRESS;
					uint8_t* i_presentDevices;

					if (PK_I2CBusScanGetResults(dev, i_status, i_presentDevices, 16) == PK_OK)
					{
						a_debout = 214;
						if (*i_status == PK_I2C_STAT_COMPLETE)
						{
							a_debout = 215;
							//I2C.#.presentDevices
							for (i = 0; i < 16; i++)
							{
								a_debout = 2150 + i;
								I2C_presentDevices(i)=i_presentDevices[i];
							}
							I2C_scanning=false;
						}
					}
				*/
			}
			// PulseEnginev2
			if (info_PulseEnginev2!=0)
			{
				uint8_t         bm_LimitStatusP;              // Limit+ status (bit-mapped)
				uint8_t         bm_LimitStatusN;              // Limit- status (bit-mapped)
				uint8_t         bm_HomeStatus;                // Home status (bit-mapped)
				uint8_t			bm_DedicatedLimitNInputs;
				uint8_t			bm_DedicatedLimitPInputs;
				uint8_t			bm_DedicatedHomeInputs;


				if(PK_PEv2_StatusGet(dev) == PK_OK)
				{
					// Engine info
					PEv2_nrOfAxes = dev->PEv2.info.nrOfAxes;
					PEv2_maxPulseFrequency = dev->PEv2.info.maxPulseFrequency;
					PEv2_bufferDepth = dev->PEv2.info.bufferDepth;
					PEv2_slotTiming = dev->PEv2.info.slotTiming;
					
					PEv2_AxisEnabledStatesMask = dev->PEv2.AxisEnabledStatesMask;
					PEv2_LimitOverride = dev->PEv2.LimitOverride;
					
					// Basic engine states
					PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
					PEv2_PulseEngineActivated = dev->PEv2.PulseEngineActivated;
					PEv2_PulseEngineState = dev->PEv2.PulseEngineState;
					PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
					PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;

					// Switch states
					bm_LimitStatusP = dev->PEv2.LimitStatusP;              // Limit+ status (bit-mapped)
					bm_LimitStatusN = dev->PEv2.LimitStatusN;              // Limit- status (bit-mapped)
					bm_HomeStatus = dev->PEv2.HomeStatus;                // Home status (bit-mapped)

					PEv2_EmergencySwitchPolarity = dev->PEv2.EmergencySwitchPolarity;

					// Other inputs
					PEv2_ErrorInputStatus = dev->PEv2.ErrorInputStatus;
					PEv2_MiscInputStatus = dev->PEv2.MiscInputStatus;

					if(HAL_Machine_On==true)
					{
					/*
						PK_PEState_peSTOPPED        = 0,           // Pulse engine is stopped
						PK_PEState_peINTERNAL       = 1,           // PEv1: Internal motion controller is in use, PEv2: not used
						PK_PEState_peBUFFER         = 2,           // PEv1: Buffered operation mode is in use, PEv2: not used
						PK_PEState_peRUNNING        = 3,           // Pulse engine is activated

						PK_PEState_peJOGGING        = 10,          // Jogging mode enabled
						PK_PEState_peSTOPPING       = 11,          // Pulse engine is stopping

						PK_PEState_peHOME           = 20,          // All axes are homed
						PK_PEState_peHOMING         = 21,          // Axes homing is in progress

						PK_PEState_pePROBECOMPLETE  = 30,          // All axes are homed
						PK_PEState_pePROBE          = 31,          // Axes probing is in progress
						PK_PEState_pePROBEERROR     = 32,          // Error occured during probing

						PK_PEState_peHYBRIDPROBE_STOPPING = 40,
						PK_PEState_peHYBRIDPROBE_COMPLETE = 41,

						PK_PEState_peSTOP_LIMIT     = 100,         // Pulse engine stopped due to limit reached
						PK_PEState_peSTOP_EMERGENCY = 101          // Pulse engine stopped due to emergency switch
					*/
						PEv2_PulseEngineStateSetup = PK_PEState_peRUNNING;
					}
					else
					{
						PEv2_PulseEngineStateSetup = PK_PEState_peSTOPPED;
					}

					switch (PEv2_PulseEngineState)
					{
						case PK_PEState_peSTOPPED:           // Pulse engine is stopped
							break;
						casePK_PEState_peINTERNAL:           // PEv1: Internal motion controller is in use, PEv2: not used
							break;
						casePK_PEState_peBUFFER:           // PEv1: Buffered operation mode is in use, PEv2: not used
							break;
						casePK_PEState_peRUNNING:           // Pulse engine is activated
							break;
						casePK_PEState_peJOGGING:          // Jogging mode enabled
							break;
						casePK_PEState_peSTOPPING:          // Pulse engine is stopping
							break;
						casePK_PEState_peHOME:          // All axes are homed
							break;
						casePK_PEState_peHOMING:          // Axes homing is in progress
							break;
						casePK_PEState_pePROBECOMPLETE:          // All axes are homed
							break;
						casePK_PEState_pePROBE:          // Axes probing is in progress
							break;
						casePK_PEState_pePROBEERROR:          // Error occured during probing

							break;
						casePK_PEState_peHYBRIDPROBE_STOPPING:
							break;
						casePK_PEState_peHYBRIDPROBE_COMPLETE:
							break;
						casePK_PEState_peSTOP_LIMIT:         // Pulse engine stopped due to limit reached
							break;
						casePK_PEState_peSTOP_EMERGENCY:          // Pulse engine stopped due to emergency switch
							break;
					}
					usleep(sleepdur); 
				}

				if(PK_PEv2_Status2Get(dev) == PK_OK)
				{
					bm_DedicatedLimitNInputs = dev->PEv2.DedicatedLimitNInputs;
					bm_DedicatedLimitPInputs = dev->PEv2.DedicatedLimitPInputs;
					bm_DedicatedHomeInputs = dev->PEv2.DedicatedHomeInputs;
					usleep(sleepdur); 
				}

				bool doPositionSet = false;
				bool doMove = false;
				bool doStateSet = false;
	
				if (PEv2_nrOfAxes!=0)
				{
					uint8_t  bm_SoftLimitStatus = dev->PEv2.SoftLimitStatus;
					int tAxisEnabledMask = 0;
					for (i = 0; i < PEv2_nrOfAxes; i++)
					{
						PEv2_AxesState(i) = dev->PEv2.AxesState[i];
						PEv2_CurrentPosition(i) = dev->PEv2.CurrentPosition[i];

						PEv2_LimitStatusP(i) = Get_BitOfByte(bm_LimitStatusP,i);
						PEv2_LimitStatusN(i) = Get_BitOfByte(bm_LimitStatusN,i);
						PEv2_HomeStatus(i) =  Get_BitOfByte(bm_HomeStatus,i);

						PEv2_DedicatedLimitNInputs(i) =  Get_BitOfByte(bm_DedicatedLimitNInputs,i);
						PEv2_DedicatedLimitPInputs(i) =  Get_BitOfByte(bm_DedicatedLimitPInputs,i);
						PEv2_DedicatedHomeInputs(i) =  Get_BitOfByte(bm_DedicatedHomeInputs,i);

						
						PEv2_SoftLimitStatus(i) = Get_BitOfByte(bm_SoftLimitStatus,i);

						switch( PEv2_AxesState(i))
						{
							case PK_PEAxisState_axSTOPPED:        // Axis is stopped
								PEv2_AxisEnabledStates(i) = false;
								PEv2_AxisLimitOverride(i) = false;
								break ;
							case PK_PEAxisState_axREADY:        // Axis ready
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = false;
								break ;
							case PK_PEAxisState_axRUNNING:        // Axis is running
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = false;
								if ( PEv2_params.AxesConfig[i]!=PK_AC_POSITION_MODE)
								{
									if(PEv2_ReferencePositionSpeed(i) != PEv2_joint_vel_cmd(i) * PEv2_params_STEP_SCALE(i))
									{
										PEv2_ReferencePositionSpeed(i) = PEv2_joint_vel_cmd(i) * PEv2_params_STEP_SCALE(i);
										PEv2_PositionSetup(i) = PEv2_joint_pos_cmd(i) * PEv2_params_STEP_SCALE(i);

										doMove=true;
										dev->PEv2.ReferencePositionSpeed[i] = PEv2_ReferencePositionSpeed(i);
										dev->PEv2.PositionSetup[i] = PEv2_PositionSetup(i);
									}
									PEv2_joint_vel_fb(i) = ( PEv2_joint_pos_fb(i) - (PEv2_CurrentPosition(i) / PEv2_params_STEP_SCALE(i)) )* Loop_Frequ;
								}
								else
								{

									if ((PEv2_CurrentPosition(i) - PEv2_ReferencePositionSpeed(i)) < 10 )
									{
										PEv2_ReferencePositionSpeed(i) = PEv2_joint_pos_cmd(i) * PEv2_params_STEP_SCALE(i);
										dev->PEv2.ReferencePositionSpeed[i] = PEv2_ReferencePositionSpeed(i);
										dev->PEv2.PositionSetup[i] = PEv2_PositionSetup(i);
										//doPositionSet =true;
										doMove=true;
									}
									PEv2_joint_vel_fb(i) = 0;
								}
								// calculate actual velocity by position difference (time estimated by actual rtc_loop_frequ [Hz] / [1/sec] )
								
								PEv2_joint_pos_fb(i) = dev->PEv2.CurrentPosition[i] / PEv2_params_STEP_SCALE(i);

								break ;
							case PK_PEAxisState_axHOMING_RESETTING:   // Stopping the axis to reset the position counters
								PEv2_AxisLimitOverride(i) = true;
								PEv2_AxisEnabledStates(i) = true;
								break ;
							case PK_PEAxisState_axHOMING_BACKING_OFF:   // Backing off switch
								PEv2_AxisLimitOverride(i) = true;
								PEv2_AxisEnabledStates(i) = true;
								break ;
							case PK_PEAxisState_axHOME:       // Axis is homed
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = true;
							// PEv2_joint_
								break ;
							case PK_PEAxisState_axHOMINGSTART:       // Homing procedure is starting on axis
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = true;
								break ;
							case PK_PEAxisState_axHOMINGSEARCH:       // Homing procedure first step - going to home
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = true;
								break ;
							case PK_PEAxisState_axHOMINGBACK:       // Homing procedure second step - slow homing
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = true;
								break ;
							case PK_PEAxisState_axPROBED:       // Probing completed for this axis
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = false;
								break ;
							case PK_PEAxisState_axPROBESTART:       // Probing procedure is starting on axis
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = false;
								break ;
							case PK_PEAxisState_axPROBESEARCH:       // Probing procedure - probing
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = false;
								break ;
							case PK_PEAxisState_axERROR:       // Axis error
								PEv2_AxisEnabledStates(i) = false;
								PEv2_AxisLimitOverride(i) = false;
								break ;
							case PK_PEAxisState_axLIMIT:        // Axis limit tripped
								PEv2_AxisEnabledStates(i) = true;
								PEv2_AxisLimitOverride(i) = false;
								break ;
							default:
								break ;
						}
						tAxisEnabledMask = Set_BitOfByte(tAxisEnabledMask,i,PEv2_AxisEnabledStates(i));
						PEv2_LimitOverrideSetup = Set_BitOfByte(PEv2_LimitOverrideSetup ,i,PEv2_AxisLimitOverride(i));

					}


					if (tAxisEnabledMask != dev->PEv2.AxisEnabledMask)
					{
						dev->PEv2.AxisEnabledMask = tAxisEnabledMask;
						doStateSet=true;
					}
					if (PEv2_LimitOverrideSetup != dev->PEv2.LimitOverrideSetup)
					{
						dev->PEv2.LimitOverrideSetup = PEv2_LimitOverrideSetup;
						doStateSet=true;
					}
				}


				if(PK_PEv2_ExternalOutputsGet(dev) == PK_OK)
				{
					bool RelBit_array[8];
					bool OcBit_array[8];


					//uint8_t ExternalRelayOutputs_set=  PEv2_ExternalRelayOutput;
					//uint8_t ExternalOCOutputs_set=  PEv2_ExternalOCOutput;
					bool doSetExtOut = false;

					if (PEv2_ExternalRelayOutputs != dev->PEv2.ExternalRelayOutputs)
					{
						dev->PEv2.ExternalRelayOutputs = PEv2_ExternalRelayOutputs ;
						doSetExtOut=true;
						usleep(sleepdur); 
					}
					if (PEv2_ExternalOCOutputs != dev->PEv2.ExternalOCOutputs)
					{
						dev->PEv2.ExternalOCOutputs = PEv2_ExternalOCOutputs;
						doSetExtOut=true;
						usleep(sleepdur); 
					}
					if (doSetExtOut==true)
					{
						if(PK_PEv2_ExternalOutputsSet(dev) != PK_OK)
						{
							usleep(sleepdur); 
							if(PK_PEv2_ExternalOutputsSet(dev) != PK_OK)
							{
								usleep(sleepdur); 
							}
						}
					}
				}

				if (PEv2_PulseEngineState != PEv2_PulseEngineStateSetup)
				{
					dev->PEv2.PulseEngineStateSetup = PEv2_PulseEngineStateSetup;
					//dev->PEv2.AxisEnabledMask = PEv2_AxisEnabledMask;
					doStateSet=true;
				}

				if (PEv2_LimitOverrideSetup != dev->PEv2.LimitOverrideSetup)
				{
					dev->PEv2.LimitOverrideSetup = PEv2_LimitOverrideSetup;
					doStateSet=true;
				}

				if (doStateSet==true)
				{
					if(PK_PEv2_PulseEngineStateSet(dev) != PK_OK)
					{
						usleep(sleepdur); 
						if(PK_PEv2_PulseEngineStateSet(dev) != PK_OK)
						{
							usleep(sleepdur); 
						}
					}
				}

				if (doMove==true)
				{
					if(PK_PEv2_PulseEngineMove(dev) != PK_OK)
					{
						usleep(sleepdur); 
						if(PK_PEv2_PulseEngineMove(dev) != PK_OK)
						{
							usleep(sleepdur); 
						}
					}
				}

				if (PEv2_params_ApplyIniSettings==0 && PEv2_nrOfAxes!=PEv2_PulseEngineEnabled )
				{
					PEv2_params_ApplyIniSettings=1;
				}

				a_debout = 224;
			}
	

			if ((PK_DigitalIOGet(dev) == PK_OK) && (PK_AnalogIOGet(dev) == PK_OK) && (PK_PinConfigurationGet(dev) == PK_OK)){  //gets IO data and checks return value 
				err=0;
				sPoKeysPinData iPin;
				a_debout = 230;

				/* 
				if ( PEv2_EmergencyOutputPin!=0 && PEv2_PulseEngineEnabled!=0)
				{	//check if pin is parametrized in HAL
					a_debout = 231;
					if (PEv2_EmergencyOutput==0)
					{
						//error: lvalue required as left operand of assignment
						Pins_DigitalValueSet(PEv2_EmergencyOutputPin-1)=0;
					}
					else
					{
						//error: lvalue required as left operand of assignment
						Pins_DigitalValueSet(PEv2_EmergencyOutputPin-1)=1;
					}
					
				}*/

				a_debout = 232;
				for(i=0;i<info_PinCount-1;i++)
				{
					a_debout = 2320+i;
					Pins_DigitalCounterValue(i)=dev->Pins[i].DigitalCounterValue;
					Pins_AnalogValue(i)=dev->Pins[i].AnalogValue;
					Pins_PinFunction(i)=dev->Pins[i].PinFunction;
					Pins_CounterOptions(i)=dev->Pins[i].CounterOptions;
					if (dev->Pins[i].DigitalValueGet==0)
					{
						Pins_DigitalValueGet(i)=false;
					}
					else
					{
						Pins_DigitalValueGet(i)=true;
					}
					


					if (Pins_DigitalValueSet(i)==1)
					{
						dev->Pins[i].DigitalValueSet=1;
					}
					else
					{
						dev->Pins[i].DigitalValueSet=0;
					}
					;
					Pins_DigitalCounterAvailable(i)=dev->Pins[i].DigitalCounterAvailable;
					//Pins_MappingType(i)=dev->Pins[i].MappingType;
					//Pins_KeyCodeMacroID(i)=dev->Pins[i].KeyCodeMacroID;
					//Pins_KeyModifier(i)=dev->Pins[i].KeyModifier;
					//Pins_downKeyCodeMacroID(i)=dev->Pins[i].downKeyCodeMacroID;
					//Pins_downKeyModifier(i)=dev->Pins[i].downKeyModifier;
					//Pins_upKeyCodeMacroID(i)=dev->Pins[i].upKeyCodeMacroID;
					//Pins_upKeyModifier(i)=dev->Pins[i].upKeyModifier;
					//dev->Pins[i].preventUpdate=Pins_preventUpdate(i);
					
					a_debout = 2330+i;
				}
					usleep(sleepdur); 
					if (PEv2_PulseEngineEnabled!=0)
					{
						if (dev->PEv2.EmergencyInputPin>0)
						{
							a_debout = 234;
							PEv2_EmergencyInput=Pins_DigitalValueGet(dev->PEv2.EmergencyInputPin-1);
						}
						else
						{
							a_debout = 235;
							if ( PEv2_EmergencyOutputPin!=0)
							{
								PEv2_EmergencyInput=PEv2_EmergencyOutput;
							}
							else
							{
								PEv2_EmergencyInput=false;
							}
						}
					}





				a_debout = 236;
				if (PK_DigitalIOSet(dev) != PK_OK)
				{
					usleep(sleepdur); 
				}
				if (PK_DigitalIOSet(dev) != PK_OK)
				{
					usleep(sleepdur); 
				}
				a_debout = 237;
			 }
			 else
			 {             		  //on connection error
				a_debout = 245;
				PK_DisconnectDevice(dev);
				dev=NULL;  		  //tries to reconnect
				err=1;

			 }
	
	/*if (info_MatrixKeyboard!=0)
	{
		a_debout = 330;
		Update_MatrixKB;
		for (i = 0; i < 128; i++)
		{
		//	MatrixKB_matrixKBvalues(i) = MatrixKB.matrixKBvalues[i];
		//	MatrixKB_matrixKBPins(i) = MatrixKB.matrixKBPins[i];
		//	usleep(sleepdur); 
		}
		usleep(sleepdur); 
	}*/
	
			if (kbd48CNC_available!=0)
			{
				a_debout = 340;
				dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
				PK_PoNETGetModuleSettings(dev);
				if (PK_PoNETGetModuleSettings(dev) != PK_OK)
				{

					if (PK_PoNETGetModuleSettings(dev) != PK_OK)
					{
					}
				}

				dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
				PK_PoNETGetModuleLight(dev);
				if (PK_PoNETGetModuleLight(dev) == PK_OK)
				{
					kbd48CNC_lightValue = dev->PoNETmodule.lightValue;


					kbd48CNC_KeyBrightness = 255-kbd48CNC_lightValue;

					if (kbd48CNC_prevBrightness != kbd48CNC_KeyBrightness)
					{
						dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
						dev->PoNETmodule.PWMduty = kbd48CNC_KeyBrightness;
						if (PK_PoNETSetModulePWM(dev) != PK_OK)
						{
							usleep(sleepdur); 
							if (PK_PoNETSetModulePWM(dev) != PK_OK)
							{

							}
						}
					
					}
					usleep(sleepdur); 
				}




				dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
				if (PK_PoNETGetModuleStatusRequest(dev) != PK_OK)
				{
					usleep(sleepdur);
					dev->PoNETmodule.moduleID = 0;
					if (PK_PoNETGetModuleStatusRequest(dev) != PK_OK)
					{
						kbd48CNC_PoNetID =0;
						usleep(sleepdur);
					}
				}
				kbd48CNC_Counter[i]=0;
				while (PK_PoNETGetModuleStatus(dev) != PK_OK && kbd48CNC_Counter[i]<10)
				{
					kbd48CNC_Counter[i]++;
				}

				dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
				if (PK_PoNETGetModuleStatus(dev) != PK_OK)
				{
					if (PK_PoNETGetModuleStatus(dev) != PK_OK)
					{
						usleep(sleepdur);
					}
				}

				for (i = 0; i < 48; i++)
				{
					bool state = false;
					bool instate = false;
					// blinking 1Hz for Debug
					//state=secBlink;
					int offset[] = {15, 8, 7, 0};

					int top = (i & 0xF0) + offset[(i / 4) % 4];
					int y = i % 4;
					int ID = top + y;
					if (((i / 4) % 2) == 0)
					{
						ID = top - y;
					}
						
					if (kbd48CNC_LED(i) == true)
					{
						state=true;
					}

					if ((dev->PoNETmodule.statusIn[ID/8] & (1<<(ID%8))) > 0)
					{
						instate=true;
						state = true;
					}

					/*while (kbd48CNC_Button(i)!=instate)
					{
						kbd48CNC_Button(i)=instate;
						//usleep(sleepdur/10);
					}*/
					
					kbd48CNC_Button(i)=instate;
					int tmp = dev->PoNETmodule.statusOut[ID/8];

					if (state)
					{
						tmp |= (1<<(ID%8));
					}
					else
					{
						tmp &= ~(1<<(ID%8));
					}
					dev->PoNETmodule.statusOut[ID/8] = tmp;

					/*while (dev->PoNETmodule.statusOut[ID/8] != tmp)
					{
						dev->PoNETmodule.statusOut[ID/8] = tmp;
						//usleep(sleepdur/10);
					}*/
				}

				dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
				PK_PoNETSetModuleStatus(dev);
				PK_PoNETSetModuleStatus(dev);
				if (PK_PoNETSetModuleStatus(dev) != PK_OK)
				{
					usleep(sleepdur);
					dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
					if (PK_PoNETSetModuleStatus(dev) != PK_OK)
					{
						usleep(sleepdur);
					}
				}

				dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
				if (PK_PoNETSetModuleStatus(dev) != PK_OK)
				{
					dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
					if (PK_PoNETSetModuleStatus(dev) != PK_OK)
					{
					}
				}
				
				//dev->PoNETmodule.moduleID = kbd48CNC_PoNetID;
				//PK_PoNETSetModuleStatus(dev);
			}

			if (info_EasySensors!=0)
			{
				Update_EasySensors;
				for (i = 0; i < dev->info.iEasySensors; i++)
				{
					EasySensors_sensorValue(i) = EasySensors[i].sensorValue;            // Current sensor value
				}
			}

	

			if (info_PoExtBus!=0)
			{
				for (i = 0; i < 10; i++)
				{
					PK_PoExtBusGet(dev);
					PoExtBus_DataGet[i] = dev->PoExtBusData[i];

					PoExtBus_In_0(i) = Get_BitOfByte(PoExtBus_DataGet[i], 0);
					PoExtBus_In_1(i) = Get_BitOfByte(PoExtBus_DataGet[i], 1);
					PoExtBus_In_2(i) = Get_BitOfByte(PoExtBus_DataGet[i], 2);
					PoExtBus_In_3(i) = Get_BitOfByte(PoExtBus_DataGet[i], 3);
					PoExtBus_In_4(i) = Get_BitOfByte(PoExtBus_DataGet[i], 4);
					PoExtBus_In_5(i) = Get_BitOfByte(PoExtBus_DataGet[i], 5);
					PoExtBus_In_6(i) = Get_BitOfByte(PoExtBus_DataGet[i], 6);
					PoExtBus_In_7(i) = Get_BitOfByte(PoExtBus_DataGet[i], 7);

					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 0, PoExtBus_Out_0(i));
					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 1, PoExtBus_Out_1(i));
					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 2, PoExtBus_Out_2(i));
					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 3, PoExtBus_Out_3(i));
					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 4, PoExtBus_Out_4(i));
					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 5, PoExtBus_Out_5(i));
					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 6, PoExtBus_Out_6(i));
					PoExtBus_DataSet[i] = Set_BitOfByte(PoExtBus_DataSet[i], 7, PoExtBus_Out_7(i));

					if (PoExtBus_DataGet[i] != PoExtBus_DataSet[i])
					{
						dev->PoExtBusData[i] = PoExtBus_DataSet[i];
						PK_PoExtBusSet(dev);
						usleep(sleepdur); 
						PK_PoExtBusSet(dev);
					}
					usleep(sleepdur); 
				}
			}

			if (info_protI2C!=0)
			{
			
			}


			if (rtc_lastmin!=rtc_min)
			{
				a_debout = 310;
				rtc_lastmin=rtc_min;
				
				 info_PinCount = dev->info.iPinCount;                        // Number of pins, physically on the device
				 info_PWMCount = dev->info.iPWMCount;                        // Number of pins that support PWM output
				 info_BasicEncoderCount = dev->info.iBasicEncoderCount;               // Number of basic encoders
				 info_EncodersCount = dev->info.iEncodersCount;                   // Number of encoder slots available
				 info_FastEncoders = dev->info.iFastEncoders;                    // Number of fast encoders supported
				 info_UltraFastEncoders = dev->info.iUltraFastEncoders;               // Number of available ultra fast encoders
				 info_PWMinternalFrequency = dev->info.PWMinternalFrequency;             // Main PWM peripheral clock
				 info_AnalogInputs = dev->info.iAnalogInputs;                    // Number of available analog inputs
				 info_KeyMapping = dev->info.iKeyMapping;                      // Device supports key mapping (acts as a USB keyboard)
				 info_TriggeredKeyMapping = dev->info.iTriggeredKeyMapping;             // Device supports triggered key mapping
				 info_KeyRepeatDelay = dev->info.iKeyRepeatDelay;                  // Device supports user customizable key repeat rates and delays
				 info_DigitalCounters = dev->info.iDigitalCounters;                 // Device supports digital counters
				 info_JoystickButtonAxisMapping = dev->info.iJoystickButtonAxisMapping;       // Device supports mapping of joystick buttons
				 info_JoystickAnalogToDigitalMapping = dev->info.iJoystickAnalogToDigitalMapping;  // Device supports mapping of analog inputs to digital keys
				 info_Macros = dev->info.iMacros;                          // Device supports customizable macro sequences
				 info_MatrixKeyboard = dev->info.iMatrixKeyboard;                  // Device supports matrix keyboard
				 info_MatrixKeyboardTriggeredMapping = dev->info.iMatrixKeyboardTriggeredMapping;  // Device supports matrix keyboard triggered key mapping
				 info_LCD = dev->info.iLCD;                             // Device supports alphanumeric LCD display
				 info_MatrixLED = dev->info.iMatrixLED;                       // Device supports matrix LED display
				 info_ConnectionSignal = dev->info.iConnectionSignal;                // Device supports connection signal output
				 info_PoExtBus = dev->info.iPoExtBus;                        // Device supports PoExtBus digital outputs
				 info_PoNET = dev->info.iPoNET;                           // Device supports PoNET bus devices
				 info_AnalogFiltering = dev->info.iAnalogFiltering;                 // Device supports analog inputs low-pass digital filtering
				 info_InitOutputsStart = dev->info.iInitOutputsStart;                // Device supports initializing outputs at startup
				 info_protI2C = dev->info.iprotI2C;                         // Device supports I2C bus (master)
				 info_prot1wire = dev->info.iprot1wire;                       // Device supports 1-wire bus (master)
				 info_AdditionalOptions = dev->info.iAdditionalOptions;               // Device supports additional options with activation keys
				 info_LoadStatus = dev->info.iLoadStatus;                      // Device supports reporting load status
				 info_CustomDeviceName = dev->info.iCustomDeviceName;                // Device supports specifying custom device names
				 info_PoTLog27support = dev->info.iPoTLog27support;                 // Device supports PoTLog27 firmware
				 info_SensorList = dev->info.iSensorList;                      // Device supports sensor lists
				 info_WebInterface = dev->info.iWebInterface;                    // Device supports web interface
				 info_FailSafeSettings = dev->info.iFailSafeSettings;                // Device supports fail-safe mode
				 info_JoystickHATswitch = dev->info.iJoystickHATswitch;               // Device supports joystick HAT switch mapping
				 info_PulseEngine = dev->info.iPulseEngine;                     // Device supports Pulse engine
				 info_PulseEnginev2 = dev->info.iPulseEnginev2;                   // Device supports Pulse engine v2
				 info_EasySensors = dev->info.iEasySensors;                     // Device supports EasySensors
				 a_debout = 311;

				 if ( info_PulseEnginev2!=0 && PEv2_params_ApplyIniSettings!=0 )
				{

					bool setPeConfig = false;
					PK_SaveConfiguration(dev);
					usleep(sleepdur*2); 
					PEv2_params_ApplyIniSettings = 0;
						
					a_debout = 136;
				 }
			}
			alive=0;
			usleep(sleepdur); 
        }
    }

    exit(0);
};



