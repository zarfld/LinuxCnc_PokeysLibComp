/*  thois header is only for .comp files

component pokeys_rt "realtime module for pokeys";

description """
This component provides a real-time interface to the PoKeys device.
It is based on the PoKeysLibRt library. (which is PoKeysLib.H modified not tu use libusb)
It is a work in progress and is not yet functional.
""";

pin in unsigned devSerial;
pin in unsigned inPulseEngineState;				// State of pulse engine - see ePoKeysPEState
pin out bit is_module = 1; //one pin is required to use halcompile)

pin out bit err;
pin out bit connected;
pin out bit connected.usb;
pin out bit connected.fusb;
pin out bit connected.udp;
pin out bit connected.net;
pin out bit alive;

license "GPL";
author "Dominik Zarfl";
option homemod;
option extra_link_args "-lPoKeysRt -lpthread";
option extra_setup;
;;
*/
/* To incorporate default homing.c file from a local git src tree:
** enable #define HOMING_BASE set to the path to the current homing.c file.
** (Note: CUSTOM_HOMEMODULE precludes duplicate api symbols)
** (Edit myname as required for valid path)
*/

// #define HOMING_BASE /home/myname/linuxcnc-dev/src/emc/motion/homing.c
//#define RTAPI

#define STR(s)  #s
#define XSTR(s) STR(s)
#undef POKEYSLIB_USE_LIBUSB
#include "motion.h"
#include "homing.h"
#include "rtapi.h"
#include "rtapi_ctype.h"	/* isspace() */
#include "rtapi_app.h"		/* RTAPI realtime module decls */

#include <hal.h>		/* HAL public API decls */

#include "rtapi_io.h"

#include "stdlib.h"

//usr/include/bsd/stdlib.h
//usr/include/stdlib.h
//usr/include/c++/10/stdlib.h
//usr/include/c++/10/tr1/stdlib.h
//usr/include/c++/10/tr1/stdlib.h
//usr/include/tcl8.6/tk-private/compat/stdlib.h
//usr/include/tcl8.6/tcl-private/compat/stdlib.h
///usr/include/c++/10/tr1/stdlib.h

#include "PoKeysLibRt.h"

#undef POKEYSLIB_USE_LIBUSB

sPoKeysDevice* dev = 0;		// PoKeys device

static char* serial_number = "";
RTAPI_MP_STRING(serial_number, "Device Serial Number") 

static int ConnectionType = 0;
RTAPI_MP_INT(ConnectionType, "Connection Type (1..USB, 2..UDP, 3..Network, 4..fastUSB)")

static char* IP = "0.0.0.0";
RTAPI_MP_STRING(IP, "IP Address")

static int timeout_ms = 500;
RTAPI_MP_INT(timeout_ms, "Timeout in ms")

sPoKeysDevice* TryConnectToDevice(uint32_t intSerial)
{
	rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: serial_number: %d\n", intSerial);
	rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: serial_number=%d\n", __FILE__, __FUNCTION__, intSerial);
	sPoKeysDevice* retDev = NULL;
	uint32_t i_Timeout = 1000;
	uint32_t enm_usb_dev = 0;
	uint32_t enm_fusb_dev = 0;
	uint32_t enm_udp_dev = 0;
	int32_t nDevs = 0;
	uint32_t lastConectionTypeTried = 0;

	if (timeout_ms > 0)
	{
		i_Timeout = timeout_ms;
	}
	if (intSerial != 0)
	{

		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: intSerial=%d\n", __FILE__, __FUNCTION__, intSerial);
		/* no USB in RT
		if (retDev == NULL)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_EnumerateUSBDevices\n", __FILE__, __FUNCTION__);
			enm_usb_dev = PK_EnumerateUSBDevices();
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: enm_usb_dev :%d\n", __FILE__, __FUNCTION__, enm_usb_dev);
			if (enm_usb_dev != 0)
			{
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToDeviceWSerial(%d, %d)", __FILE__, __FUNCTION__, intSerial, i_Timeout);
				retDev = PK_ConnectToDeviceWSerial(intSerial, i_Timeout);  //waits for usb device
				lastConectionTypeTried = 1;
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: lastConectionTypeTried = 1\n", __FILE__, __FUNCTION__);
			}
		}*/
		if (retDev == NULL)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToDeviceWSerial_UDP(%d, %d)", __FILE__, __FUNCTION__, intSerial, i_Timeout);
			retDev = PK_ConnectToDeviceWSerial_UDP(intSerial, i_Timeout);  //waits for udp device
			lastConectionTypeTried = 2;
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: lastConectionTypeTried = 2\n", __FILE__, __FUNCTION__);
		}
		if (retDev == NULL)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_SearchNetworkDevices(net_devices, %d, %d)\n", __FILE__, __FUNCTION__, i_Timeout, intSerial);
			sPoKeysNetworkDeviceSummary* net_devices;
			int32_t nDevs = PK_SearchNetworkDevices(net_devices, i_Timeout, intSerial);
			if (nDevs != 0)
			{
				enm_udp_dev = nDevs;
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToNetworkDevice(net_devices)\n", __FILE__, __FUNCTION__);
				retDev = PK_ConnectToNetworkDevice(net_devices);
				lastConectionTypeTried = 3;
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: lastConectionTypeTried = 3\n", __FILE__, __FUNCTION__);
			}
			else
			{
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_EnumerateNetworkDevices(net_devices, %d)\n", __FILE__, __FUNCTION__, i_Timeout);
				//	nDevs = PK_EnumerateNetworkDevices(net_devices, i_Timeout); // does not work - it hangs here
				//deb_out = 1135;
				if (nDevs != 0)
				{
					rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: nDevs: %s\n", __FILE__, __FUNCTION__, nDevs);
					enm_udp_dev = nDevs;
					rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToNetworkDevice(net_devices)\n", __FILE__, __FUNCTION__);
					retDev = PK_ConnectToNetworkDevice(net_devices);
					lastConectionTypeTried = 3;
					rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: lastConectionTypeTried = 3\n", __FILE__, __FUNCTION__);
				}
			}
		}
	}
	else
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: udp_devices[16]\n", __FILE__, __FUNCTION__);
		sPoKeysNetworkDeviceSummary udp_devices[16];
		/* no USB in RT
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_EnumerateUSBDevices()\n", __FILE__, __FUNCTION__);
		enm_usb_dev = PK_EnumerateUSBDevices();
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_EnumerateUSBDevices()=%d\n", __FILE__, __FUNCTION__, enm_usb_dev);

		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_EnumerateFastUSBDevices()\n", __FILE__, __FUNCTION__);
		//enm_fusb_dev = PK_EnumerateFastUSBDevices();
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_EnumerateFastUSBDevices()=%d\n", __FILE__, __FUNCTION__, enm_fusb_dev);*/
		i_Timeout = 1000;
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_EnumerateNetworkDevices(udp_devices, %d)\n", __FILE__, __FUNCTION__, i_Timeout);
		int32_t nDevs = PK_EnumerateNetworkDevices(udp_devices, i_Timeout); // does not work - it hangs here
		//	rtapi_print("");
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: nDevs: %d\n", __FILE__, __FUNCTION__, nDevs);
		//enm_udp_dev = nDevs;

		if (nDevs != 0)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToNetworkDevice(udp_devices) - %d\n", __FILE__, __FUNCTION__, nDevs);
			for (int i = 0; i < 16; i++)
			{
				if (retDev == NULL)
				{
					rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: udp_devices[%d].SerialNumber: %d\n", __FILE__, __FUNCTION__, i, udp_devices[i].SerialNumber);
					if (udp_devices[i].SerialNumber != 0)
					{
						rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToDeviceWSerial_UDP(%d, %d)\n", __FILE__, __FUNCTION__, udp_devices[i].SerialNumber, i_Timeout);
						retDev = PK_ConnectToDeviceWSerial_UDP(udp_devices[i].SerialNumber, i_Timeout);  //waits for udp device
						lastConectionTypeTried = 2;
						rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: lastConectionTypeTried = 2\n", __FILE__, __FUNCTION__);
					}
				}
				else
				{
					break;
				}
			}
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: lastConectionTypeTried = 3\n", __FILE__, __FUNCTION__);
		}
		/* no USB in RT
		if (enm_fusb_dev > 0)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToDevice FastUSB\n", __FILE__, __FUNCTION__);
			for (int i = 0; i < enm_fusb_dev; i++)
			{
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToDevice(%d)\n", __FILE__, __FUNCTION__, i);
				retDev = PK_ConnectToDevice(i);
			}
			lastConectionTypeTried = 4;
		}

		if (enm_usb_dev > 0)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToDevice FastUSB\n", __FILE__, __FUNCTION__);
			for (int i = 0; i < enm_usb_dev; i++)
			{
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: PK_ConnectToDevice(%d)\n", __FILE__, __FUNCTION__, i);
				retDev = PK_ConnectToDevice(i);
			}
			lastConectionTypeTried = 4;
		}*/
		//deb_out = 120;
	}
	if (retDev != NULL)
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: Connected to device\n", __FILE__, __FUNCTION__);
		//connected = 1;
		if (lastConectionTypeTried == 1)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: USB\n", __FILE__, __FUNCTION__);
			// cannot set pins outside of user_mainloop()
			//	connected_usb = 1;
			//	connected_fusb = 0;
			//	connected_udp = 0;
			//	connected_net = 0;
		}
		if (lastConectionTypeTried == 2)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: UDP\n", __FILE__, __FUNCTION__);
			// cannot set pins outside of user_mainloop()
			//	connected_usb = 0;
			//	connected_fusb = 0;
			//	connected_udp = 1;
			//	connected_net = 0;
		}
		if (lastConectionTypeTried == 3)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: Network\n", __FILE__, __FUNCTION__);
			// cannot set pins outside of user_mainloop()
			//	connected_usb = 0;
			//	connected_fusb = 0;
			//	connected_udp = 0;
			//	connected_net = 1;
		}
		return retDev;
	}
	else
	{
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: failed to connect to device\n", __FILE__, __FUNCTION__);
		//connected_usb = 0;
		//connected_fusb = 0;
		//connected_udp = 0;
		//connected_net = 0;
		//err = 1;
		return NULL;
	}
}

int myid = 0;
// EXTRA_SETUP is executed before rtapi_app_main()
EXTRA_SETUP()
{
	
	myid = hal_init("pokeys_rt");
	rtapi_print_msg(RTAPI_MSG_ERR,"PoKeysRt: %s:%s: hal_init(pokeys_rt):%d\n", __FILE__, __FUNCTION__);
	int wait_ms = 5000;
	int retry = 5;
	rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: extra_arg=%s\n", __FILE__, __FUNCTION__);
	//usleep(wait_ms);  // wait for the HAL to start up
	for (int i = 0; i < retry; i++)
	{
		if (dev == NULL)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: TryConnectToDevice(0)\n", __FILE__, __FUNCTION__);
			if (serial_number != "") {
				dev = TryConnectToDevice(atoi(serial_number));
			}
			else{
				dev = TryConnectToDevice(0);
			}

		}
		if (dev != NULL)
		{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeysRt: %s:%s: Connected\n", __FILE__, __FUNCTION__);
			break;
		}
	}

	rtapi_print("");
	// devSerial = extra_arg;
	return 0;
}

EXTRA_CLEANUP()
{
	if (dev != NULL)
	{
		PK_DisconnectDevice(dev);
	}
	if (myid > 0)
		hal_exit(myid);
}

/* channels provided by PEv2 which might be relevant:
pin out unsigned PEv2.PulseEngineState;				// State of pulse engine - see ePoKeysPEState

// Pulse engine v2 structure...
pin out unsigned PEv2.#.AxesState [8];				// Axis states (bit-mapped) - see ePK_PEAxisState
pin out unsigned PEv2.#.AxesConfig [8];				// Axis configuration - see ePK_PEv2_AxisConfig
pin in unsigned PEv2.#.AxesSwitchConfig [8];		// Axis switch configuration - see ePK_PulseEngineV2_AxisSwitchOptions

pin io unsigned PEv2.#.HomingSpeed [8];			// Homing speed per axis (in %)
pin io unsigned PEv2.#.HomingReturnSpeed [8];		// Homing return speed per axis (in % of the homing speed)
pin io unsigned PEv2.#.HomingAlgorithm[8];		// Homing algorithm configuration
pin io u32 PEv2.#.HomeOffsets [8];				// Home position offset
pin io unsigned PEv2.#.HomeBackOffDistance [8];		// Back-off distance after homing
*/
static emcmot_joint_t* joints;

// local per-joint data (includes hal pin data)
typedef struct {
   // local_home_state_t home_state;           // OUT pin
    bool         homing;               // OUT pin
    bool         homed;                // OUT pin
    bool         home_sw;              // IN  pin
    bool         index_enable;         // IO  pin
    bool         joint_in_sequence;
    int          pause_timer;
    double       home_offset;          // intfc, updateable
    double       home;                 // intfc, updateable
    double       home_final_vel;       // intfc
    double       home_search_vel;      // intfc
    double       home_latch_vel;       // intfc
    int          home_flags;           // intfc
    int          home_sequence;        // intfc, updateable
    bool         volatile_home;        // intfc
    bool         home_is_synchronized;
} home_local_data;

static  home_local_data H[EMCMOT_MAX_JOINTS];

// data for per-joint homing-specific hal pins:
typedef struct {
    hal_bit_t* home_sw;      // home switch input
    hal_bit_t* homing;       // joint is homing
    hal_bit_t* homed;        // joint was homed
    hal_bit_t* index_enable; // motmod sets: request reset on index
                             //        encoder clears: index arrived
 //   hal_s32_t* home_state;   // homing state machine state


    hal_u32_t* PEv2_AxesState; // State of pulse engine - see ePoKeysPEState
    hal_u32_t* PEv2_AxesCommand; // Commands to  pulse engine - see ePoKeysPECmd
    hal_u32_t* PEv2_AxesConfig; // Axis configuration - see ePK_PEv2_AxisConfig
    hal_u32_t* PEv2_AxesSwitchConfig; // Axis switch configuration - see ePK_PulseEngineV2_AxisSwitchOptions

    hal_u32_t* PEv2_HomingSpeed; // Homing speed per axis (in %)
    hal_u32_t* PEv2_HomingReturnSpeed; // Homing return speed per axis (in % of the homing speed)
    hal_u32_t* PEv2_HomingAlgorithm; // Homing algorithm configuration
    hal_u32_t* PEv2_HomeOffsets; // Home position offset
    hal_u32_t* PEv2_HomeBackOffDistance; // Back-off distance after homing

} one_joint_home_data_t;


typedef struct {
    one_joint_home_data_t jhd[EMCMOT_MAX_JOINTS];
} all_joints_home_data_t;

static all_joints_home_data_t* joint_home_data = 0;

static int makepins(int id, int njoints)
{
    // home_pins needed to work with configs expecting them:
    int jno, retval;
    one_joint_home_data_t* addr;

    joint_home_data = hal_malloc(sizeof(all_joints_home_data_t));
    if (joint_home_data == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "HOMING: all_joints_home_data_t malloc failed\n");
        return -1;
    }

    retval = 0;
    for (jno = 0; jno < njoints; jno++) {
        addr = &(joint_home_data->jhd[jno]);

        retval += hal_pin_bit_newf(HAL_IN, &(addr->home_sw), id,
            "joint.%d.home-sw-in", jno);

        retval += hal_pin_bit_newf(HAL_OUT, &(addr->homing), id,
            "joint.%d.homing", jno);

        retval += hal_pin_bit_newf(HAL_OUT, &(addr->homed), id,
            "joint.%d.homed", jno);

    //    retval += hal_pin_s32_newf(HAL_OUT, &(addr->home_state), id,
    //        "joint.%d.home-state", jno);

        retval += hal_pin_bit_newf(HAL_IO, &(addr->index_enable), id,
            "joint.%d.index-enable", jno);

        retval += hal_pin_u32_newf(HAL_IN, &(addr->PEv2_AxesState), id,
            "joint.%d.PEv2.AxesState", jno);

        retval += hal_pin_u32_newf(HAL_OUT, &(addr->PEv2_AxesCommand), id,
            "joint.%d.PEv2.AxesCommand", jno);

        retval += hal_pin_u32_newf(HAL_IN, &(addr->PEv2_AxesConfig), id,
            "joint.%d.PEv2.AxesConfig", jno);

        retval += hal_pin_u32_newf(HAL_OUT, &(addr->PEv2_AxesSwitchConfig), id,
            "joint.%d.PEv2.AxesSwitchConfig", jno);

        retval += hal_pin_u32_newf(HAL_IO, &(addr->PEv2_HomingSpeed), id,
            "joint.%d.PEv2.HomingSpeed", jno);

        retval += hal_pin_u32_newf(HAL_IO, &(addr->PEv2_HomingReturnSpeed), id,
            "joint.%d.PEv2.HomingReturnSpeed", jno);

        retval += hal_pin_u32_newf(HAL_IO, &(addr->PEv2_HomingAlgorithm), id,
            "joint.%d.PEv2.HomingAlgorithm", jno);

        retval += hal_pin_u32_newf(HAL_IO, &(addr->PEv2_HomeOffsets), id,
            "joint.%d.PEv2.HomeOffsets", jno);

        retval += hal_pin_u32_newf(HAL_IO, &(addr->PEv2_HomeBackOffDistance), id,
            "joint.%d.PEv2.HomeBackOffDistance", jno);


    }
    return retval;
}
// All (skeleton) functions required for homing api follow:
void homeMotFunctions(void(*pSetRotaryUnlock)(int, int)
    , int (*pGetRotaryIsUnlocked)(int)
)
{
    return;
}




// one-time initialization (return 0 if ok):
int  homing_init(int id,
    double servo_period,
    int n_joints,
    int n_extrajoints,
    emcmot_joint_t* pjoints) {
    joints = pjoints;
    if (n_joints == 0) { n_joints == 8; }

    return makepins(id, n_joints);
}

// once-per-servo-period functions:
bool do_homing(void) { return 1; }//return 1 if allhomed
void read_homing_in_pins(int njoints)
{
    int jno;
    one_joint_home_data_t* addr;

    return;
}
void write_homing_out_pins(int njoints) {
    int jno;
    one_joint_home_data_t* addr;
    for (jno = 0; jno < njoints; jno++) {
        addr = &(joint_home_data->jhd[jno]);
        addr->homing = (hal_bit_t*)H[jno].homing;
        addr->homed = (hal_bit_t*)H[jno].homed;
   //     addr->home_state = (hal_s32_t *)H[jno].home_state;
        addr->index_enable = (hal_bit_t*)H[jno].index_enable;
    }
    return;
}

// responses to EMCMOT_JOINT_HOME message:
void do_home_joint(int joint_num) {
    one_joint_home_data_t* addr;
    addr = &(joint_home_data->jhd[joint_num]);
    addr->PEv2_AxesState = (hal_u32_t*)PK_PEAxisState_axHOMINGSTART;
    return; 
}


//---------------------------------------------------------------------
// QUERIES

// overall status:
bool get_allhomed() { return 1; }
bool get_homing_is_active() { return 0; }

// per-joint information:
int  get_home_sequence(int jno) { return H[jno].home_sequence; }//return s
bool get_homing(int jno) { return H[jno].homing; }
bool get_homed(int jno) { return H[jno].homed; }
bool get_index_enable(int jno) { return H[jno].index_enable; }
bool get_home_needs_unlock_first(int jno) { return 0; }
bool get_home_is_idle(int jno) { return 1; }
bool get_home_is_synchronized(int jno) { return H[jno].home_is_synchronized;}
bool get_homing_at_index_search_wait(int jno) { return 0; }

static void update_home_is_synchronized(void) {
    // invoke anytime H[*].home_sequence is altered
    int jno, jj, all_joints;
    all_joints = sizeof(H) / sizeof(H[0]);
    for (jno = 0; jno < all_joints; jno++) {
        H[jno].home_is_synchronized = 0;
        if (H[jno].home_sequence < 0) {
            // neg: sync all joints with same ABS(H[jno].home_sequence):
            for (jj = 0; jj < all_joints; jj++) {
                if (abs(H[jj].home_sequence) == abs(H[jno].home_sequence)) {
                    H[jj].home_is_synchronized = 1;
                }
            }
        }
    }
}

// per-joint interface parameters (one-time setup)
void set_joint_homing_params(int    jno,
    double offset,
    double home,
    double home_final_vel,
    double home_search_vel,
    double home_latch_vel,
    int    home_flags,
    int    home_sequence,
    bool   volatile_home
) {
    H[jno].home_offset = offset;
    H[jno].home = home;
    H[jno].home_final_vel = home_final_vel;
    H[jno].home_search_vel = home_search_vel;
    H[jno].home_latch_vel = home_latch_vel;
    H[jno].home_flags = home_flags;
    H[jno].home_sequence = home_sequence;
    H[jno].volatile_home = volatile_home;
    update_home_is_synchronized();
    return;
}

// updateable interface params (for inihal pin changes typically):
void update_joint_homing_params(int    jno,
    double offset,
    double home,
    int    home_sequence
) {
    H[jno].home_offset = offset;
    H[jno].home = home;
    H[jno].home_sequence = home_sequence;
 //   update_home_is_synchronized();
    return;
}

//=====================================================================

// all home functions for homing api
EXPORT_SYMBOL(homeMotFunctions);

EXPORT_SYMBOL(homing_init);
EXPORT_SYMBOL(do_homing);
EXPORT_SYMBOL(get_allhomed);
EXPORT_SYMBOL(get_homed);
EXPORT_SYMBOL(get_home_is_idle);
EXPORT_SYMBOL(get_home_is_synchronized);
EXPORT_SYMBOL(get_home_needs_unlock_first);
EXPORT_SYMBOL(get_home_sequence);
EXPORT_SYMBOL(get_homing);
EXPORT_SYMBOL(get_homing_at_index_search_wait);
EXPORT_SYMBOL(get_homing_is_active);
EXPORT_SYMBOL(get_index_enable);
EXPORT_SYMBOL(read_homing_in_pins);
EXPORT_SYMBOL(do_home_joint);
EXPORT_SYMBOL(do_cancel_homing);
EXPORT_SYMBOL(set_unhomed);
EXPORT_SYMBOL(set_joint_homing_params);
EXPORT_SYMBOL(update_joint_homing_params);
EXPORT_SYMBOL(write_homing_out_pins);





#undef XSTR
#undef STR
#undef HOMING_BASE
#undef USE_HOMING_BASE
#undef CUSTOM_HOMEMODULE