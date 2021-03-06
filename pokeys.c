//component pokeys "PoKeys IO driver, by Mit Zot";
#include <unistd.h>   /* UNIX standard function definitions */
#include "hal.h"
#include "PoKeysLib.h"
#include "PoKeysLibCore.h"
//option userspace yes;

hal_u32_t enum_usb_dev;
hal_u32_t enum_fusb_dev;
hal_u32_t enum_udp_dev;
hal_s32_t a_debout;

hal_bit_t err;
hal_bit_t connected;
hal_u32_t devSerial;
hal_bit_t alive;

hal_bit_t machine_is_on;

// PoKeys device information
//sPoKeysDevice_Info info;
hal_u32_t info_PinCount;                        // Number of pins, physically on the device
hal_u32_t info_PWMCount;                        // Number of pins that support PWM output
hal_u32_t info_BasicEncoderCount;               // Number of basic encoders
hal_u32_t info_EncodersCount;                   // Number of encoder slots available
hal_u32_t info_FastEncoders;                    // Number of fast encoders supported
hal_u32_t info_UltraFastEncoders;               // Number of available ultra fast encoders
hal_u32_t info_PWMinternalFrequency;             // Main PWM peripheral clock
hal_u32_t info_AnalogInputs;                    // Number of available analog inputs
hal_u32_t info_KeyMapping;                      // Device supports key mapping (acts as a USB keyboard)
hal_u32_t info_TriggeredKeyMapping;             // Device supports triggered key mapping
hal_u32_t info_KeyRepeatDelay;                  // Device supports user customizable key repeat rates and delays
hal_u32_t info_DigitalCounters;                 // Device supports digital counters
hal_u32_t info_JoystickButtonAxisMapping;       // Device supports mapping of joystick buttons
hal_u32_t info_JoystickAnalogToDigitalMapping;  // Device supports mapping of analog inputs to digital keys
hal_u32_t info_Macros;                          // Device supports customizable macro sequences
hal_u32_t info_MatrixKeyboard;                  // Device supports matrix keyboard
hal_u32_t info_MatrixKeyboardTriggeredMapping;  // Device supports matrix keyboard triggered key mapping
hal_u32_t info_LCD;                             // Device supports alphanumeric LCD display
hal_u32_t info_MatrixLED;                       // Device supports matrix LED display
hal_u32_t info_ConnectionSignal;                // Device supports connection signal output
hal_u32_t info_PoExtBus;                        // Device supports PoExtBus digital outputs
hal_u32_t info_PoNET;                           // Device supports PoNET bus devices
hal_u32_t info_AnalogFiltering;                 // Device supports analog inputs low-pass digital filtering
hal_u32_t info_InitOutputsStart;                // Device supports initializing outputs at startup
hal_u32_t info_protI2C;                         // Device supports I2C bus (master)
hal_u32_t info_prot1wire;                       // Device supports 1-wire bus (master)
hal_u32_t info_AdditionalOptions;               // Device supports additional options with activation keys
hal_u32_t info_LoadStatus;                      // Device supports reporting load status
hal_u32_t info_CustomDeviceName;                // Device supports specifying custom device names
hal_u32_t info_PoTLog27support;                 // Device supports PoTLog27 firmware
hal_u32_t info_SensorList;                      // Device supports sensor lists
hal_u32_t info_WebInterface;                    // Device supports web interface
hal_u32_t info_FailSafeSettings;                // Device supports fail-safe mode
hal_u32_t info_JoystickHATswitch;               // Device supports joystick HAT switch mapping
hal_u32_t info_PulseEngine;                     // Device supports Pulse engine
hal_u32_t info_PulseEnginev2;                   // Device supports Pulse engine v2
hal_u32_t info_EasySensors;                     // Device supports EasySensors

//hal_u32_t PoExtBus_DataGet [10];   
hal_bit_t PoExtBus_In0 [10];                   
hal_bit_t PoExtBus_In1 [10];    
hal_bit_t PoExtBus_In2 [10];    
hal_bit_t PoExtBus_In3 [10];    
hal_bit_t PoExtBus_In4 [10];    
hal_bit_t PoExtBus_In5 [10];    
hal_bit_t PoExtBus_In6 [10];    
hal_bit_t PoExtBus_In7 [10]; 


//hal_u32_t PoExtBus_DataSet [10];                 
hal_bit_t PoExtBus_Out0 [10];                   
hal_bit_t PoExtBus_Out1 [10];    
hal_bit_t PoExtBus_Out2 [10];    
hal_bit_t PoExtBus_Out3 [10];    
hal_bit_t PoExtBus_Out4 [10];    
hal_bit_t PoExtBus_Out5 [10];    
hal_bit_t PoExtBus_Out6 [10];    
hal_bit_t PoExtBus_Out7 [10];

// Pulse engine v2 information
hal_u32_t PEv2_nrOfAxes;
hal_u32_t PEv2_maxPulseFrequency;
hal_u32_t PEv2_bufferDepth;
hal_u32_t PEv2_slotTiming;

//***********************************************************************
//[TRAJ] information in HAL-INI 
hal_s32_t PEv2_params_COORDINATES;
hal_s32_t PEv2_params_LINEAR_UNITS;
hal_s32_t PEv2_params_ANGULAR_UNITS;
hal_float_t PEv2_params_CYCLE_TIME;
hal_float_t PEv2_params_DEFAULT_LINEAR_VELOCITY;
hal_float_t PEv2_params_MAX_LINEAR_VELOCITY;

hal_bit_t PEv2_params_ApplyIniSettings;
//***********************************************************************

// Pulse engine v2 structure...
hal_u32_t PEv2_AxesState [8];				// Axis states (bit-mapped) - see ePK_PEAxisState
hal_u32_t PEv2_AxesConfig [8];				// Axis configuration - see ePK_PEv2_AxisConfig
hal_u32_t PEv2_AxesSwitchConfig [8];		// Axis switch configuration - see ePK_PulseEngineV2_AxisSwitchOptions

hal_u32_t PEv2_InvertAxisEnable [8];		// Invert axis enable signal
hal_u32_t PEv2_SoftLimitMaximum [8];		// Soft limit maximum position
hal_u32_t PEv2_SoftLimitMinimum [8];		// Soft limit minimum position
hal_u32_t PEv2_HomingSpeed [8];			// Homing speed per axis (in %)
hal_u32_t PEv2_HomingReturnSpeed [8];		// Homing return speed per axis (in % of the homing speed)
hal_u32_t PEv2_HomeOffsets [8];				// Home position offset
hal_u32_t PEv2_HomingAlgorithm [8];		// Homing algorithm configuration
hal_u32_t PEv2_FilterLimitMSwitch [8];		// Digital filter for limit- switch
hal_u32_t PEv2_FilterLimitPSwitch [8];		// Digital filter for limit+ switch
hal_u32_t PEv2_FilterHomeSwitch [8];		// Digital filter for home switch
hal_u32_t PEv2_ProbePosition [8];			// Position where probe detected change
hal_u32_t PEv2_ProbeMaxPosition [8];		// Maximum position to travel to until stopping and returning error

hal_u32_t PEv2_CurrentPosition [8];			// Current position
hal_u32_t PEv2_PositionSetup [8];			// Position to be set as current position
hal_u32_t PEv2_ReferencePositionSpeed [8];	// Reference position or speed (position or pulses/s)
hal_float_t PEv2_MaxSpeed [8];					// Maximum axis speed (in pulses per ms)
hal_float_t PEv2_MaxAcceleration [8];			// Maximum axis acceleration (in pulses/ms/ms)
hal_float_t PEv2_MaxDecceleration [8];			// Maximum axis deceleration (in pulses/ms/ms)

hal_float_t PEv2_MaxSpeed_mm_per_sec [8];			// Maximum axis speed (mm/s)
hal_float_t PEv2_MaxAcceleration_mm_per_sec2 [8];	// Maximum axis acceleration (in mm/s???)
hal_float_t PEv2_MaxDecceleration_mm_per_sec2 [8];	// Maximum axis deceleration (in mm/s???)

	
hal_float_t PEv2_joint_vel_cmd [8];				
hal_float_t PEv2_joint_vel_fb [8];				
hal_float_t PEv2_joint_pos_cmd [8];				
hal_float_t PEv2_joint_pos_fb [8];				
    
//***********************************************************************
//[JOINT_n] information in HAL-INI 
hal_s32_t PEv2_params_TYPE [8];			//LINEAR / ANGULAR
hal_float_t PEv2_params_HOME [8];
hal_float_t PEv2_params_FERROR [8];
hal_float_t PEv2_params_MIN_FERROR [8];
hal_float_t PEv2_params_MAX_VELOCITY [8];
hal_float_t PEv2_params_MAX_ACCELERATION [8];
hal_float_t PEv2_params_STEPGEN_MAXVEL [8];
hal_float_t PEv2_params_STEPGEN_MAXACCEL [8];
hal_float_t PEv2_params_Feedback_Encoder_Id [8];
//hal_float_t PEv2_params_P [8];
//hal_float_t PEv2_params_I [8];
//hal_float_t PEv2_params_D [8];
//hal_float_t PEv2_params_FF0 [8];
//hal_float_t PEv2_params_FF1 [8];
//hal_float_t PEv2_params_FF2 [8];
//hal_float_t PEv2_params_BIAS [8];
hal_float_t PEv2_params_DEADBAND [8];
hal_float_t PEv2_params_MAX_OUTPUT [8];
hal_float_t PEv2_params_ENCODER_SCALE [8];
// these are in nanoseconds
hal_float_t PEv2_params_DIRSETUP [8];					// (ns)
hal_float_t PEv2_params_DIRHOLD [8];					// (ns)
hal_float_t PEv2_params_STEPLEN [8];					// (ns)
hal_float_t PEv2_params_STEPSPACE [8];					// (ns)
hal_float_t PEv2_params_STEP_SCALE [8];					// (pulses/mm)
hal_float_t PEv2_params_MIN_LIMIT [8];					// position (mm)
hal_float_t PEv2_params_MAX_LIMIT [8];					// position (mm)
hal_float_t PEv2_params_HOME_OFFSET [8];				// position (mm)
hal_float_t PEv2_params_HOME_SEARCH_VEL [8];			// position (mm)
hal_float_t PEv2_params_HOME_LATCH_VEL [8];				// position (mm)
hal_float_t PEv2_params_HOME_FINAL_VEL [8];				// position (mm)
hal_s32_t PEv2_params_HOME_USE_INDEX [8];				// YES / NO
hal_s32_t PEv2_params_HOME_IGNORE_LIMITS [8];			// YES / NO
//***********************************************************************

hal_u32_t PEv2_MPGjogMultiplier [8];		// MPG jog multiplier value
hal_u32_t PEv2_MPGjogEncoder [8];			// MPG jog encoder ID

hal_u32_t PEv2_PinHomeSwitch [8];			// Home switch pin (0 for external dedicated input)
hal_u32_t PEv2_PinLimitMSwitch [8];			// Limit- switch pin (0 for external dedicated input)
hal_u32_t PEv2_PinLimitPSwitch [8];			// Limit+ switch pin (0 for external dedicated input)
hal_u32_t PEv2_AxisEnableOutputPins [8];		// Axis enabled output pin (0 for external dedicated output)

hal_bit_t PEv2_HomeSwitchStat [8];				// Home switch
hal_bit_t PEv2_LimitMSwitch [8];			// Limit- switch
hal_bit_t PEv2_LimitPSwitch [8];			// Limit+ switch
hal_bit_t PEv2_AxisEnableOutput [8];		// Axis enabled output
hal_bit_t PEv2_HomeSwitch;				// Home switch

hal_u32_t PEv2_HomeBackOffDistance [8];		// Back-off distance after homing
hal_u32_t PEv2_MPGjogDivider [8];			// Divider for the MPG jogging (enhanced encoder resolution)

hal_u32_t PEv2_PulseEngineEnabled;			// Pulse engine enabled status, also number of enabled axes
hal_u32_t PEv2_PulseGeneratorType;			// Pulse engine generator type (0: external, 1: internal 3ch)
hal_bit_t PEv2_PG_swap_stepdir;
hal_bit_t PEv2_PG_extended_io;

hal_u32_t PEv2_ChargePumpEnabled;				// Charge pump output enabled
hal_u32_t PEv2_EmergencySwitchPolarity;		// Emergency switch polarity (set to 1 to invert)
hal_u32_t PEv2_PulseEngineActivated;			// Pulse engine activation status
hal_u32_t PEv2_LimitStatusP[8];					// Limit+ status (NOT bit-mapped)
hal_u32_t PEv2_LimitStatusN[8];					// Limit- status (NOT bit-mapped)
hal_u32_t PEv2_HomeStatus[8];					// Home status (NOT bit-mapped)
hal_u32_t PEv2_ErrorInputStatus;				// Stepper motor driver error inputs status (bit-mapped)
hal_u32_t PEv2_MiscInputStatus;				// Miscelenous digital inputs...
hal_u32_t PEv2_LimitOverride;					// Limit override status
hal_u32_t PEv2_LimitOverrideSetup;			// Limit override configuration
hal_u32_t PEv2_PulseEngineState;				// State of pulse engine - see ePoKeysPEState
hal_u32_t PEv2_AxisEnabledMask;				// Bit-mapped ouput enabled mask

hal_u32_t PEv2_EmergencyInputPin;
hal_bit_t PEv2_EmergencyInput;
hal_u32_t PEv2_EmergencyOutputPin;				//added in component only for estop-out to reset estop (if 0 EmergencyInput will be reused)
hal_bit_t PEv2_EmergencyOutput;

hal_u32_t PEv2_param1;
hal_u32_t PEv2_param2;
hal_u32_t PEv2_param3;
hal_u32_t PEv2_AxisEnabledStatesMask;		// Bit-mapped states, where axis enabled and charge pump signals are active
hal_u32_t PEv2_PulseEngineStateSetup;		// Pulse engine new state configuration
hal_u32_t PEv2_SoftLimitStatus [8] ;				// Bit-mapped soft-limit statuses per axes
/* 			PEv2_DedicatedLimitNInputs(i) =  Get_BitOfByte(bm_DedicatedLimitNInputs,i);
			PEv2_DedicatedLimitPInputs(i) =  Get_BitOfByte(bm_DedicatedLimitPInputs,i);
			PEv2_DedicatedHomeInputs(i) =  Get_BitOfByte(bm_DedicatedHomeInputs,i);
								PEv2_AxisEnabledStates(i) = false;
					PEv2_AxisLimitOverride(i) = false;
			*/
hal_bit_t PEv2_DedicatedLimitNInputs [8] ;
hal_bit_t PEv2_DedicatedLimitPInputs [8] ;
hal_bit_t PEv2_DedicatedHomeInputs [8] ;
hal_bit_t PEv2_AxisEnabledStates [8] ;
hal_bit_t PEv2_AxisLimitOverride [8] ;

hal_u32_t PEv2_ExternalRelayOutputs;			// External relay outputs
hal_u32_t PEv2_ExternalOCOutputs;			// External open-collector outputs
hal_bit_t PEv2_ExternalRelayOutput[4];			// External relay outputs
hal_bit_t PEv2_ExternalOCOutput[4];			// External open-collector outputs
hal_u32_t PEv2_PulseEngineBufferSize;		// Buffer size information...
hal_u32_t PEv2_motionBufferEntriesAccepted;
hal_u32_t PEv2_newMotionBufferEntries;
hal_u32_t PEv2_HomingStartMaskSetup;			// Bit-mapped axes indexes to be homed
hal_u32_t PEv2_ProbeStartMaskSetup;			// Bit-mapped axis indexes for probing
hal_u32_t PEv2_ProbeInput;					// Probe input (0:disabled, 1-8:external inputs, 9+ Pin ID-9)
hal_u32_t PEv2_ProbeInputPolarity;			// Probe input polarity
hal_u32_t PEv2_ProbeStatus;					// Probe status (probe completion bit-mapped status)
//hal_u32_t PEv2_MotionBuffer[448];			// Motion buffer
hal_float_t PEv2_ProbeSpeed;						// Probe speed (ratio of the maximum speed)
hal_u32_t PEv2_BacklashWidth [8];			// half of real backlash width
hal_u32_t PEv2_BacklashRegister [8];		// current value of the backlash register
hal_u32_t PEv2_BacklashAcceleration [8];	// in pulses per ms^2
hal_u32_t PEv2_BacklashCompensationEnabled;

hal_u32_t rtc_sec;
hal_u32_t rtc_min;
hal_u32_t rtc_hour;
hal_u32_t rtc_dow;
hal_u32_t rtc_dom;
hal_u32_t rtc_tmp;
hal_u32_t rtc_doy;
hal_u32_t rtc_month;
hal_u32_t rtc_year;
hal_u32_t rtc_loopcount;


hal_u32_t rtc_lastmin;
hal_u32_t rtc_lastsec;
hal_u32_t rtc_loop_frequ;
hal_u32_t rtc_loop_frequ_demand;
hal_u32_t rtc_sec_ret;
hal_u32_t rtc_hal_latency;

hal_u32_t Pins_DigitalCounterValue [55];
hal_u32_t Pins_AnalogValue [55];
hal_u32_t Pins_PinFunction [55];
hal_u32_t Pins_CounterOptions [55];
hal_bit_t Pins_DigitalValueGet [55];
hal_bit_t Pins_DigitalValueSet [55];
hal_u32_t Pins_DigitalCounterAvailable [55];
//hal_u32_t Pins_MappingType [55];
//hal_u32_t Pins_KeyCodeMacroID [55];
//hal_u32_t Pins_KeyModifier [55];
//hal_u32_t Pins_downKeyCodeMacroID [55];
//hal_u32_t Pins_downKeyModifier [55];
//hal_u32_t Pins_upKeyCodeMacroID [55];
//hal_u32_t Pins_upKeyModifier [55];
//hal_u32_t Pins_preventUpdate [55];

/*
hal_u32_t MatrixKB.matrixKBconfiguration;		// Matrix keyboard configuration (set to 1 to enable matrix keyboard support)
hal_u32_t MatrixKB.matrixKBwidth;				// Matrix keyboard width (number of columns)
hal_u32_t MatrixKB.matrixKBheight;				// Matrix keyboard height (number of rows)
hal_u32_t MatrixKB.matrixKBcolumnsPins.#[8];		// List of matrix keyboard column connections
hal_u32_t MatrixKB.matrixKBrowsPins.#[16];			// List of matrix keyboard row connections
//hal_u32_t MatrixKB.matrixKBvalues.#[128];			// Current state of each matrix keyboard key (assumes fixed width of 8 columns)
//hal_bit_t MatrixKB.matrixKBPins.#[128];			// matrixKBrowsPins
*/

hal_bit_t kbd48CNC_available;
hal_u32_t kbd48CNC_PoNetID;
hal_u32_t kbd48CNC_KeyBrightness;
hal_u32_t kbd48CNC_prevBrightness;
hal_u32_t kbd48CNC_lightValue;
//hal_u32_t kbd48CNC_ID [48];
hal_bit_t kbd48CNC_LED [48];
hal_bit_t kbd48CNC_Button [48];


hal_u32_t Encoders_encoderValue [26];       // Encoder current value
hal_u32_t Encoders_encoderOptions [26];      // Encoder options -    bit 0: enable encoder
													//                      bit 1: 4x sampling
													//                      bit 2: 2x sampling
													//                      bit 3: reserved
													//                      bit 4: direct key mapping for direction A
													//                      bit 5: mapped to macro for direction A
													//                      bit 6: direct key mapping for direction B
													//                      bit 7: mapped to macro for direction B
hal_u32_t Encoders_channelApin [26];         // Channel A encoder pin
hal_u32_t Encoders_channelBpin [26];         // Channel B encoder pin



hal_u32_t EasySensors_sensorValue [16];            // Current sensor value
hal_u32_t EasySensors_sensorType [16];             // Type of the sensor
hal_u32_t EasySensors_sensorRefreshPeriod [16];    // Refresh period in 0.1s
hal_u32_t EasySensors_sensorFailsafeConfig [16];   // Failsafe configuration (bits 0-5: timeout in seconds, bit 6: invalid=0, bit 7: invalid=0x7FFFFFFF)
hal_u32_t EasySensors_sensorReadingID [16];        // Sensor reading selection (see Protocol description document for details)


/*
hal_u32_t PoStep.EnablePoStepCommunication;
    // Status
hal_u32_t PoStep_SupplyVoltage [8];
hal_u32_t PoStep_Temperature [8];
hal_u32_t PoStep_InputStatus [8];
hal_u32_t PoStep_DriverStatus [8];
hal_u32_t PoStep_FaultStatus [8];
hal_u32_t PoStep_UpdateState [8];

    // Settings
hal_u32_t PoStep_DriverMode [8];
hal_u32_t PoStep_StepMode [8];
hal_u32_t PoStep_Current_FS [8];
hal_u32_t PoStep_Current_Idle [8];
hal_u32_t PoStep_Current_Overheat [8];
hal_u32_t PoStep_TemperatureLimit [8];

    // Configuration
hal_u32_t PoStep_AddressI2C [8];
hal_u32_t PoStep_DriverType [8];
hal_u32_t PoStep_UpdateConfig [8];


hal_u32_t PWM.PWMperiod;
hal_u32_t PWM_PWMduty [6];
hal_u32_t PWM_PWMenabledChannels [6];
hal_u32_t PWM_PWMpinIDs [6];
*/
hal_u32_t PoNET_moduleID[16];
hal_u32_t PoNET_i2cAddress[16];
hal_u32_t PoNET_moduleType[16];
hal_u32_t PoNET_moduleSize[16];
hal_u32_t PoNET_moduleOptions[16];

hal_u32_t PoNET_PWMduty;
hal_u32_t PoNET_lightValue;
hal_u32_t PoNET_PoNETstatus;
hal_u32_t PoNET_statusIn [16];
hal_u32_t PoNET_statusOut [16];
/*
hal_u32_t MatrixLED.displayEnabled;                    // Display enabled byte - set to 1 to enable the display
hal_u32_t MatrixLED.rows;                              // Number of Matrix LED rows
hal_u32_t MatrixLED.columns;                           // Number of Matrix LED columns
hal_u32_t MatrixLED.RefreshFlag;                       // Flag for refreshing data - set to 1 to refresh the display
hal_u32_t MatrixLED_data [8];                           // Matrix LED buffer - one byte per row (assumes 8 columns)

hal_u32_t LCD.Configuration;                     // LCD configuration byte - 0: disabled, 1: enabled on primary pins, 2: enabled on secondary pins
hal_u32_t LCD.Rows;                              // Number of LCD module rows
hal_u32_t LCD.Columns;                           // Number of LCD module columns
hal_u32_t LCD.RowRefreshFlags;                   // Flag for refreshing data - bit 0: row 1, bit 1: row 2, bit 2: row 3, bit 3: row 4
    
hal_u32_t LCD.line1.# [20];                         // Line 1 buffer
hal_u32_t LCD.line2.# [20];                         // Line 2 buffer
hal_u32_t LCD.line3.# [20];                         // Line 3 buffer
hal_u32_t LCD.line4.# [20];                         // Line 4 buffer
*/
hal_u32_t I2C_presentDevices [16];
hal_bit_t I2C_scanning ;

//license "GPL";

//option extra_link_args "-lPoKeys";

;;




sPoKeysDevice * dev=0;
static int comp_id;		/* component ID */

bool HAL_Machine_On = false;
bool setPkConfig = false;
bool setPinConfig = false;
bool secBlink = false;
unsigned Loop_Frequ =0;
uint8_t kbd48CNC_Counter[48];
uint8_t rtc_latencycheck_set = 0;
int rtc_latencyCounter = 0;
int i=0;

typedef struct
{
/*
The canonical digital input (I/O type field: digin) is quite simple.
*/
//Pins
	hal_bit_t **in; 
	//hal_bit_t in-not;
	hal_bit_t **inverted;
//Parameters

//Functions
// read
}HALCDI_DigitalInput;
HALCDI_DigitalInput * DigitalInput;

typedef struct
{
/*
The canonical digital output (I/O type field: digout) is also very simple.
*/
//Pins
	hal_bit_t out; 

//Parameters
	hal_bit_t invert;
//Functions
// read
}HALCDI_DigitalOutput;

typedef struct
{
/*
The canonical analog input (I/O type: adcin). This is expected to be used for analog to digital
converters, which convert e.g. voltage to a continuous range of values.
*/

//Pins
	hal_float_t value; 

//Parameters
	hal_float_t scale; 
	hal_float_t offset;
	hal_float_t bit_weight;
	hal_float_t hw_offset;
//Functions
// read
}HALCDI_AnalogInput;

typedef struct
{
/*
The canonical analog output (I/O Type: adcout). This is intended for any kind of hardware that
can output a more-or-less continuous range of values. Examples are digital to analog converters or
PWM generators
*/

//Pins
	hal_float_t value; 
	hal_bit_t enable;

//Parameters
	hal_float_t scale; 
	hal_float_t offset;
	hal_float_t bit_weight;
	hal_float_t hw_offset;
	hal_float_t high_limit;
	hal_float_t low_limit;

//Functions
// write
}HALCDI_AnalogOutput;







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
bool  use_sleepdur1 = true;
unsigned int  sleepdur1 = 2000;
unsigned int  sleepdur2 = 2000;



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

int Init_IOPins()
{
			 int PinCount = dev->info.iPinCount;                        // Number of pins, physically on the device
			 int PWMCount = dev->info.iPWMCount;                        // Number of pins that support PWM output
			 int BasicEncoderCount = dev->info.iBasicEncoderCount;               // Number of basic encoders
			 int EncodersCount = dev->info.iEncodersCount;                   // Number of encoder slots available
			 int FastEncoders = dev->info.iFastEncoders;                    // Number of fast encoders supported
			 int UltraFastEncoders = dev->info.iUltraFastEncoders;               // Number of available ultra fast encoders
			 int PWMinternalFrequency = dev->info.PWMinternalFrequency;             // Main PWM peripheral clock
			 int AnalogInCount = dev->info.iAnalogInputs;                    // Number of available analog inputs

			 int retval;

			DigitalInput = hal_malloc(PinCount * sizeof(HALCDI_DigitalInput));
			for (i = 0; i < PinCount; i++)
			{
				const char *PinName = "digin.in-"+i;
				hal_pin_bit_new(PinName, 16 , DigitalInput[i].in, comp_id);
				//hal_pin_bit_newf(HAL_OUT, base + (2 * n), comp_id, "pokeys57.%d.pin-%02d-in", portnum, pin);
			}


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

	comp_id = hal_init("pokeys");


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
			Init_IOPins();
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

			 switch (dev->DeviceData.DeviceTypeID)
			{
				case PK_DeviceID_PoKeys57CNC:           // Pulse engine is stopped
					PEv2_PulseEngineEnabled=8;
					PEv2_PG_extended_io=true;
					break;
				case PK_DeviceID_PoKeys57CNCdb25:           // Pulse engine is stopped
					PEv2_PG_extended_io=false;
					break;
				case PK_DeviceID_57U:           // Pulse engine is stopped
					PEv2_PG_extended_io=false;
					break;
				case PK_DeviceID_57E:           // Pulse engine is stopped
					PEv2_PG_extended_io=false;
					break;
			}
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
						
						if (PEv2_PG_swap_stepdir != false)
						{
							PEv2_PulseGeneratorType=Set_BitOfByte(PEv2_PulseGeneratorType, 6, true); // swap step / dir signals
						}


						if (PEv2_PG_extended_io != false)
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
								PEv2_MaxAcceleration(i) = PEv2_params_STEPGEN_MAXACCEL(i)* PEv2_params_STEP_SCALE(i) /1000000;	// Maximum axis acceleration convert (mm/s???) to (in pulses/ms/ms)
								PEv2_MaxDecceleration(i) = PEv2_params_STEPGEN_MAXACCEL(i)* PEv2_params_STEP_SCALE(i) /1000000;	// Maximum axis deceleration convert (mm/s???) to (in pulses/ms/ms)

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
				// hope to get loopfrequency more stable - as on everyminute additional actions
				if (use_sleepdur1==false)
				{
					sleepdur2=sleepdur;
					sleepdur=sleepdur1;
					use_sleepdur1=true;
				}
				else
				{
					sleepdur1=sleepdur;
				}
				if (rtc_lastmin!=rtc_min)
				{ 
					use_sleepdur1=false;
					sleepdur1=sleepdur;
					sleepdur=sleepdur2;
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
							//I2C_presentDevices
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
					PEv2_ExternalRelayOutputs= dev->PEv2.ExternalRelayOutputs;
					PEv2_ExternalOCOutputs= dev->PEv2.ExternalOCOutputs ;
				}
						

				if (PEv2_PG_extended_io != false)
				{
					/*
					hal_bit_t PEv2.ExternalRelayOutput.#[4];			// External relay outputs
					hal_bit_t PEv2.ExternalOCOutput.#[4];			// External open-collector outputs
					*/

					uint8_t ExternalRelayOutputs_set=  0;
					uint8_t ExternalOCOutputs_set=  0;


					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,7,PEv2_ExternalRelayOutput(0));
					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,0,PEv2_ExternalRelayOutput(1));
					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,2,PEv2_ExternalRelayOutput(2));
					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,1,PEv2_ExternalRelayOutput(3));

					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,3,PEv2_ExternalOCOutput(0));
					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,4,PEv2_ExternalOCOutput(1));
					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,5,PEv2_ExternalOCOutput(2));
					ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set ,6,PEv2_ExternalOCOutput(3));


					if (ExternalOCOutputs_set != dev->PEv2.ExternalOCOutputs)
					{
						dev->PEv2.ExternalOCOutputs = ExternalOCOutputs_set;
							PK_PEv2_ExternalOutputsSet(dev);
							PK_PEv2_ExternalOutputsSet(dev);
					}

					
				}
				if (PEv2_PulseEngineState != PEv2_PulseEngineStateSetup )
				{
					dev->PEv2.PulseEngineStateSetup = PEv2_PulseEngineStateSetup;
					//dev->PEv2.AxisEnabledMask = PEv2_AxisEnabledMask;
					doStateSet=true;
				}
				else if (rtc_loopcount==0)
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
					Pins_AnalogValue(i)= 3.3 * dev->Pins[i].AnalogValue /4096;
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

    hal_exit(comp_id);
};




