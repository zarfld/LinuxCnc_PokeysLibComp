/********************************************************************
* Description:  hal_pokeys.c
*               This file, 'hal_pokeys.c', is a HAL component that 
*               provides a driver for the Pokeys devices - based on pokeys.lib.
*
* Author: Dominik Zarfl
* License: GPL Version 2
*    
* Copyright (c) 2022 
*
* Last change: 
********************************************************************/

/** This file, 'hal_pokeys.c', is a HAL component that provides a
    driver for the standard PC parallel port.

    It supports up to eight parallel ports, and if the port hardware
    is bidirectional, the eight data bits can be configured as inputs
    or outputs.

    The configuration is determined by command line arguments for the
    user space version of the driver, and by a config string passed
    to insmod for the realtime version.  The format is similar for
    both, and consists of a port address, followed by an optional
    direction, repeated for each port.  The direction is either "in"
    or "out" and determines the direction of the 8 bit data port.
    The default is out.  The 5 bits of the status port are always
    inputs, and the 4 bits of the control port are always outputs.
    Example command lines are as follows:

    user:        hal_pokeys <Device_ID>
    realtime:    insmod hal_pokeys.o cfg="<Device_ID>"

    Both of these commands install the driver and configure pokeys
    at with provided SerialNumber (USB or UDP/ethernet)

    The driver creates HAL pins and parameters for each port pin
    as follows:
    Each physical output has a corresponding HAL pin, named
    'pokeys.<device-num>.digout.<chan-num>.out', and a HAL parameter
    'pokeys.<device-num>.digout.<chan-num>.out-invert'.
    Each physical input has two corresponding HAL pins, named
    'pokeys.<device-num>.digin.<chan-num>.in' and
    'pokeys.<device-num>.digin.<chan-num>.in-not'

    <devicenum> is the number, starting from zero. <pinnum> is
    the physical pin number on the pokeys device.

    The realtime version of the driver exports two HAL functions for
    each port, 'pokeys.<devicenum>.read' and 'pokeys.<devicenum>.write'.
    It also exports two additional functions, 'pokeys.read-all' and
    'pokeys.write-all'.  Any or all of these functions can be added
    to realtime HAL threads to update the port data periodically.

    The user space version of the driver cannot export functions,
    instead it exports parameters with the same names.  The main()
    function sits in a loop checking the parameters.  If they are
    zero, it does nothing.  If any parameter is greater than zero,
    the corresponding function runs once, then the parameter is
    reset to zero.  If any parameter is less than zero, the
    corresponding function runs on every pass through the loop.
    The driver will loop forever, until it receives either
    SIGINT (ctrl-C) or SIGTERM, at which point it cleans up and
    exits.

*/


/** This program is free software; you can redistribute it and/or
    modify it under the terms of version 2 of the GNU General
    Public License as published by the Free Software Foundation.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    THE AUTHORS OF THIS LIBRARY ACCEPT ABSOLUTELY NO LIABILITY FOR
    ANY HARM OR LOSS RESULTING FROM ITS USE.  IT IS _EXTREMELY_ UNWISE
    TO RELY ON SOFTWARE ALONE FOR SAFETY.  Any machinery capable of
    harming persons must have provisions for completely removing power
    from all motors, etc, before persons enter any danger area.  All
    machinery must be designed to comply with local and national safety
    codes, and the authors of this software can not, and do not, take
    any responsibility for such compliance.

    This code was written as part of the EMC HAL project.  For more
    information, go to www.linuxcnc.org.
*/

#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_ctype.h"	/* isspace() */
#include "rtapi_app.h"		/* RTAPI realtime module decls */

#include "hal.h"		/* HAL public API decls */

#include <rtapi_io.h>

/* PoKeysLib */
#include "<PoKeysLib.h>"   /* installed by pokeyslib */
#include "<PoKeysLibCore.h>"  /* needs manualy copy to in pokeys-folder 
                                            cp PoKeysLib.h /usr/include */

/* module information */
MODULE_AUTHOR("Dominik Zarfl");
MODULE_DESCRIPTION("Pokeys Driver for EMC HAL");
MODULE_LICENSE("GPL");
static char *cfg = "27295";	/* config string, default 1 output port at 27295 */
RTAPI_MP_STRING(cfg, "config string");

/***********************************************************************
*                STRUCTURES AND GLOBAL VARIABLES                       *
************************************************************************/

/* this structure contains the runtime data needed by the
   parallel port driver for a single port
*/

bool bl_true = true;
bool bl_false = false;

typedef struct
{
    /*
    The canonical digital input (I/O type field: digin) is quite simple.
    */
    sPoKeysDevice* device;

   // char32_t IO_type_field = "digin"

    //Pins
    hal_bit_t** in;
    //hal_bit_t in-not;
    hal_bit_t** inverted;
    //Parameters

    //Functions
    // read
}pokeys_DigitalInput_t;


typedef struct
{
    /*
    The canonical digital output (I/O type field: digout) is also very simple.
    */
   // char8_t IO_type_field = "digout"
    sPoKeysDevice* device;

    //Pins
    hal_bit_t** out;

    //Parameters
    hal_bit_t* invert;
    //Functions
    // read

    hal_s32_t* read;
}pokeys_DigitalOutput_t;

typedef struct
{
    /*
    The canonical analog input (I/O type: adcin). This is expected to be used for analog to digital
    converters, which convert e.g. voltage to a continuous range of values.
    */
    sPoKeysDevice* device;

    //Pins
    hal_float_t value;

    //Parameters
    hal_float_t scale;
    hal_float_t offset;
    hal_float_t bit_weight;
    hal_float_t hw_offset;
    //Functions
    // read
}pokeys_AnalogInput_t;

typedef struct
{
    /*
    The canonical analog output (I/O Type: adcout). This is intended for any kind of hardware that
    can output a more-or-less continuous range of values. Examples are digital to analog converters or
    PWM generators
    */
    sPoKeysDevice* device;

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
}pokeys_AnalogOutput_t;

typedef struct
{
    sPoKeysDevice* device;

}pokeys_Info_t;

typedef struct {

    sPoKeysDevice* device;

    pokeys_DigitalInput_t* DigitalInput[55];
    pokeys_DigitalOutput_t* DigitalOutput[55];
    pokeys_AnalogInput_t* AnalogInput[7];
    pokeys_AnalogOutput_t* AnalogOutput[6];

    unsigned short base_addr;	/* base I/O address (0x378, etc.) */
    unsigned char data_dir;	/* non-zero if pins 2-9 are input */
    unsigned char use_control_in; /* non-zero if pins 1, 4, 16, 17 are input */ 
    hal_bit_t *status_in[10];	/* ptrs for in pins 15, 13, 12, 10, 11 */
    hal_bit_t *data_in[16];	/* ptrs for input pins 2 - 9 */
    hal_bit_t *data_out[8];	/* ptrs for output pins 2 - 9 */
    hal_bit_t data_inv[8];	/* polarity params for output pins 2 - 9 */
    hal_bit_t data_reset[8];	/* reset flag for output pins 2 - 9 */
    hal_bit_t *control_in[8];	/* ptrs for in pins 1, 14, 16, 17 */
    hal_bit_t *control_out[4];	/* ptrs for out pins 1, 14, 16, 17 */
    hal_bit_t control_inv[4];	/* pol. params for output pins 1, 14, 16, 17 */
    hal_bit_t control_reset[4];	/* reset flag for output pins 1, 14, 16, 17 */
    hal_u32_t reset_time;       /* min ns between write and reset */
    hal_u32_t debug1, debug2;
    long long write_time;
    unsigned char outdata;
    unsigned char reset_mask;       /* reset flag for pin 2..9 */
    unsigned char reset_val;        /* reset values for pin 2..9 */
    long long write_time_ctrl;
    unsigned char outdata_ctrl;
    unsigned char reset_mask_ctrl;  /* reset flag for pin 1, 14, 16, 17 */
    unsigned char reset_val_ctrl;   /* reset values for pin 1, 14, 16, 17 */
   // struct hal_parport_t portdata;
} pokeys_t;

/* pointer to array of pokeys_t structs in shared memory, 1 per port */
static pokeys_t *device_array;

/* other globals */
static int comp_id;		/* component ID */
static int num_ports;		/* number of ports configured */

static unsigned long ns2tsc_factor;
#define ns2tsc(x) (((x) * (unsigned long long)ns2tsc_factor) >> 12)

/***********************************************************************
*                  LOCAL FUNCTION DECLARATIONS                         *
************************************************************************/

/* These are the functions that actually do the I/O
   everything else is just init code
*/

static void DigitalIOGet(void* arg, long period);
static void DigitalIOSet(void* arg, long period);

/* 'pins_and_params()' does most of the work involved in setting up
   the driver.  It parses the command line (argv[]), then if the
   command line is OK, it calls hal_init(), allocates shared memory
   for the pokeys_t data structure(s), and exports pins and parameters
   It does not set up functions, since that is handled differently in
   realtime and user space.
*/
static int pins_and_params(char *argv[]);

static unsigned short parse_port_addr(char *cp);
static int export_device(int devicenum, pokeys_t * addr);
static int export_DigitalInput(int devicenum, int pin, pokeys_DigitalInput_t* pt_IoObject, int n);
static int export_DigitalOutput(int devicenum, int pin, pokeys_DigitalOutput_t* pt_IoObject, int n);
static int export_input_pin(int devicenum, int pin, hal_bit_t ** base, int n);
static int export_output_pin(int devicenum, int pin, hal_bit_t ** dbase,
    hal_bit_t * pbase, hal_bit_t * rbase, int n);

/***********************************************************************
*                       INIT AND EXIT CODE                             *
************************************************************************/

#define MAX_PORTS 8

#define MAX_TOK ((MAX_PORTS*2)+3)

int rtapi_app_main(void)
{
    char *cp;
    char *argv[MAX_TOK];
    char name[HAL_NAME_LEN + 1];
    int n, retval;


#ifdef __KERNEL__
    // this calculation fits in a 32-bit unsigned 
    // as long as CPUs are under about 6GHz
    ns2tsc_factor = (cpu_khz << 6) / 15625ul;
#else
    ns2tsc_factor = 1ll<<12;
#endif

    /* test for config string */
    if (cfg == 0) {
	    rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: no config string\n");
	    return -1;
    }
rtapi_print ( "config string '%s'\n", cfg );
    /* as a RT module, we don't get a nice argc/argv command line, we only
       get a single string... so we need to tokenize it ourselves */
    /* in addition, it seems that insmod under kernel 2.6 will truncate 
       a string parameter at the first whitespace.  So we allow '_' as
       an alternate token separator. */
    cp = cfg;
    for (n = 0; n < MAX_TOK; n++) {
	    /* strip leading whitespace */
       
	    while ((*cp != '\0') && ( isspace(*cp) || ( *cp == '_') ))
	        cp++;
	    /* mark beginning of token */
	    argv[n] = cp;
	    /* find end of token */
	    while ((*cp != '\0') && !( isspace(*cp) || ( *cp == '_') ))
	        cp++;
	    /* mark end of this token, prepare to search for next one */
	    if (*cp != '\0') {
	        *cp = '\0';
	        cp++;
	    }
    }
    for (n = 0; n < MAX_TOK; n++) {
	    /* is token empty? */
	    if (argv[n][0] == '\0') {
	        /* yes - make pointer NULL */
	        argv[n] = NULL;
	    }
    }
    /* parse "command line", set up pins and parameters */
    retval = pins_and_params(argv);
    if (retval != 0) {
    	return retval;
    }
    /* export functions for each port */
    for (n = 0; n < num_ports; n++) {
	    /* make read function name */
	    rtapi_snprintf(name, sizeof(name), "pokeys.%d.digin.read", n);
	    /* export read function */
	    retval = hal_export_funct(name, DigitalIOGet, &(device_array[n]),
	        0, 0, comp_id);
	    if (retval != 0) {
	        rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: port %d read funct export failed\n", n);
	        hal_exit(comp_id);
	        return -1;
	    }

	    /* make write function name */
	    rtapi_snprintf(name, sizeof(name), "pokeys.%d.digout.write", n);
	    /* export write function */
	    retval = hal_export_funct(name, DigitalIOSet, &(device_array[n]), 0, 0, comp_id);
	    if (retval != 0) {
	        rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: port %d write funct export failed\n", n);
	        hal_exit(comp_id);
	        return -1;
	    }

    }
    /* export functions that read and write all ports */

    rtapi_print_msg(RTAPI_MSG_INFO,
	"POKEYS: installed driver for %d ports\n", num_ports);
    hal_ready(comp_id);
    return 0;
}

void rtapi_app_exit(void)
{
    int n;
    for (n = 0; n < num_ports; n++) {
 //       hal_parport_release(&device_array[n].portdata);
    }
    hal_exit(comp_id);
}

/***********************************************************************
*                  REALTIME PORT READ AND WRITE FUNCTIONS              *
************************************************************************/

static void DigitalIOGet(void *arg, long period)
{
    pokeys_t * halDev;
    halDev = arg;

    uint32_t i;


    // Get digital inputs
    CreateRequest(halDev->device->request, 0xCC, 0, 0, 0, 0);
    if (SendRequest(halDev->device) == PK_OK)
    {
        for (i = 0; i < halDev->device->info.iPinCount; i++)
        {
            halDev->device->Pins[i].DigitalValueGet = ((unsigned char)(halDev->device->response[8 + i / 8] & (1 << (i % 8))) > 0) ? 1 : 0;
            if (halDev->device->Pins[i].DigitalValueGet != 0)
            {
                halDev->DigitalInput[i]->in = (hal_bit_t **)bl_true;
                halDev->DigitalInput[i]->inverted = (hal_bit_t**)bl_false;
            }
            else
            {
                halDev->DigitalInput[i]->in = (hal_bit_t**)bl_false;
                halDev->DigitalInput[i]->inverted = (hal_bit_t**)bl_true;
            }
        }
    }
    else
    {
        rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: DigitalIOGet");
    }

}

static void DigitalIOSet(void* arg, long period)
{
    pokeys_t* halDev;
    halDev = arg;

    uint32_t i;

    // Set digital outputs
    CreateRequest(halDev->device->request, 0xCC, 1, 0, 0, 0);
    for (i = 0; i < halDev->device->info.iPinCount; i++)
    {
        if (halDev->DigitalOutput[i]->out == (hal_bit_t**)bl_true)
        {
            halDev->device->Pins[i].DigitalValueSet = 1;
        }
        else
        {
            halDev->device->Pins[i].DigitalValueSet = 0;
        }
        if (halDev->device->Pins[i].preventUpdate > 0)
        {
            halDev->device->request[20 + i / 8] |= (unsigned char)(1 << (i % 8));
        }
        else if (halDev->device->Pins[i].DigitalValueSet > 0)
        {
            halDev->device->request[8 + i / 8] |= (unsigned char)(1 << (i % 8));
        }
    }
    if (SendRequest(halDev->device) != PK_OK)
    {
        rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: DigitalIOSet");
    }

}


/***********************************************************************
*                   LOCAL FUNCTION DEFINITIONS                         *
************************************************************************/

static int pins_and_params(char *argv[])
{
    long port_addr[MAX_PORTS];
    int data_dir[MAX_PORTS];
    int use_control_in[MAX_PORTS];
    int force_epp[MAX_PORTS];
    int n, retval;

    /* clear port_addr and data_dir arrays */
    for (n = 0; n < MAX_PORTS; n++) {
	    port_addr[n] = 0;
	    data_dir[n] = 0;
	    use_control_in[n] = 0;
	    force_epp[n] = 0;
    }
    /* parse config string, results in port_addr[] and data_dir[] arrays */
    num_ports = 0;
    n = 0;
    while ((num_ports < MAX_PORTS) && (argv[n] != 0)) {
	    port_addr[num_ports] = parse_port_addr(argv[n]);
	    if (port_addr[num_ports] < 0) {
	        rtapi_print_msg(RTAPI_MSG_ERR,
		    "POKEYS: ERROR: bad port address '%s'\n", argv[n]);
	        return -1;
	    }
	    n++;
	    if (argv[n] != 0) {
	        /* is the next token 'in' or 'out' ? */
	        if ((argv[n][0] == 'i') || (argv[n][0] == 'I')) {
		    /* we aren't picky, anything starting with 'i' means 'in' ;-) 
		     */
		    data_dir[num_ports] = 1;
                    use_control_in[num_ports] = 0;
		    n++;
	        } else if ((argv[n][0] == 'o') || (argv[n][0] == 'O')) {
		    /* anything starting with 'o' means 'out' */
		    data_dir[num_ports] = 0;
                    use_control_in[num_ports] = 0;
		    n++;
	        } else if ((argv[n][0] == 'e') || (argv[n][0] == 'E')) {
		    /* anything starting with 'e' means 'epp', which is just
                       like 'out' but with EPP mode requested, primarily for
                       the G540 with its charge pump missing-pullup drive
                       issue */
                    data_dir[num_ports] = 0;
                    use_control_in[num_ports] = 0;
                    force_epp[num_ports] = 1;
		    n++;
	        } else if ((argv[n][0] == 'x') || (argv[n][0] == 'X')) {
                    /* experimental: some parports support a bidirectional
                     * control port.  Enable this with pins 2-9 in output mode, 
                     * which gives a very nice 8 outs and 9 ins. */
                    data_dir[num_ports] = 0;
                    use_control_in[num_ports] = 1;
		    n++;
                }
	    }
	    num_ports++;
    }
    /* OK, now we've parsed everything */
    if (num_ports == 0) {
	    rtapi_print_msg(RTAPI_MSG_ERR,
	        "POKEYS: ERROR: no ports configured\n");
	    return -1;
    }
    /* have good config info, connect to the HAL */
    comp_id = hal_init("hal_pokeys");
    if (comp_id < 0) {
	    rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: hal_init() failed\n");
	    return -1;
    }
    /* allocate shared memory for parport data */

    num_ports = 2;

    device_array = hal_malloc(num_ports * sizeof(pokeys_t));
    if (device_array == 0) {
	    rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: hal_malloc() failed\n");
	    hal_exit(comp_id);
	    return -1;
    }
    /* export all the pins and params for each port */
    for (n = 0; n < num_ports; n++) {
            int modes = 0;

            unsigned devSerial;
            unsigned enum_usb_dev;
            unsigned enum_fusb_dev;
            unsigned enum_udp_dev;

            if (n = 0)
            { //temporary use hc values 57E
                devSerial = 27295;
            }
            else
            {//temporary use hc values 57CNC
                devSerial = 53386;
            }


            if (devSerial != 0)
            {
    
                //enum_usb_dev = PK_EnumerateUSBDevices();
               
                if (enum_usb_dev != 0)
                {
                    rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: connecting to %d via USB \n", devSerial);
                    device_array[n].device = PK_ConnectToDeviceWSerial(devSerial, 5000);  //waits for usb device
                }

                if (device_array[n].device == NULL)
                {
                    rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: connecting to %d via UDP \n", devSerial);
                    device_array[n].device = PK_ConnectToDeviceWSerial_UDP(devSerial, 5000);  //waits for usb device
                }

            }
            else
            {
                 sPoKeysNetworkDeviceSummary* udp_devices;
                //enum_usb_dev = PK_EnumerateUSBDevices();
                //enum_fusb_dev = PK_EnumerateUSBDevices();
                //enum_udp_dev = PK_EnumerateNetworkDevices(udp_devices,180); // does not work - it hangs here

            }





	    /* config addr and direction */
	    //device_array[n].base_addr = device_array[n].portdata.base;
	    device_array[n].data_dir = data_dir[n];
	    device_array[n].use_control_in = use_control_in[n];



	    /* set data port (pins 2-9) direction to "in" if needed */
	    if (data_dir[n]) {
	        rtapi_outb(rtapi_inb(device_array[n].base_addr+2) | 0x20, device_array[n].base_addr+2);
	    }

	    /* export all vars */
	    retval = export_device(n, &(device_array[n]));
	    if (retval != 0) {
	        rtapi_print_msg(RTAPI_MSG_ERR, "POKEYS: ERROR: port %d var export failed\n", n);
	        hal_exit(comp_id);
	        return retval;
	    }
    }
    return 0;
}

static unsigned short parse_port_addr(char *cp)
{
    unsigned short result;

    /* initial value */
    result = 0;
    /* test for leading '0x' */
    if (cp[0] == '0') {
	if ((cp[1] == 'X') || (cp[1] == 'x')) {
	    /* leading '0x', skip it */
	    cp += 2;
	}
    }
    /* ok, now parse digits */
    while (*cp != '\0') {
	/* if char is a hex digit, add it to result */
	if ((*cp >= '0') && (*cp <= '9')) {
	    result <<= 4;
	    result += *cp - '0';
	} else if ((*cp >= 'A') && (*cp <= 'F')) {
	    result <<= 4;
	    result += (*cp - 'A') + 10;
	} else if ((*cp >= 'a') && (*cp <= 'f')) {
	    result <<= 4;
	    result += (*cp - 'a') + 10;
	} else {
	    /* not a valid hex digit */
	    return -1;
	}
	/* next char */
	cp++;
    }

    return result;
}

static int export_device(int devicenum, pokeys_t * port)
{
    int retval, msg;

    /* This function exports a lot of stuff, which results in a lot of
       logging if msg_level is at INFO or ALL. So we save the current value
       of msg_level and restore it later.  If you actually need to log this
       function's actions, change the second line below */
    msg = rtapi_get_msg_level();
    rtapi_set_msg_level(RTAPI_MSG_WARN);

    retval = 0;

    /* declare input pins (data port) */
    uint32_t n;
    for (n = 0; n < port->device->info.iPinCount; n++)
    {
        port->DigitalInput[n]->device = port->device;
        port->DigitalOutput[n]->device = port->device;
        retval += export_DigitalInput(devicenum, n, port->DigitalInput[n] , n);
        retval += export_DigitalOutput(devicenum, n, port->DigitalOutput[n], n);
    }
    /* restore saved message level */
    rtapi_set_msg_level(msg);
    return retval;
}


static int export_DigitalInput(int devicenum, int pin, pokeys_DigitalInput_t * pt_IoObject, int n)
{
    int retval;

    /* export write only HAL pin for the input bit 
    
    pokeys.<device-num>.digin.<chan-num>.in
    pokeys.<device-num>.digin.<chan-num>.in-not
    */


    retval = hal_pin_bit_newf(HAL_OUT, pt_IoObject->in + (2 * n), comp_id,
        "pokeys.%d.digin.%02d.in", devicenum, pin);
    if (retval != 0) {
        return retval;
    }
    /* export another write only HAL pin for the same bit inverted */
    retval = hal_pin_bit_newf(HAL_OUT, pt_IoObject->inverted + (2 * n) + 1, comp_id,
        "pokeys.%d.digin.%02d.in-not", devicenum, pin);
    return retval;
}

static int export_DigitalOutput(int devicenum, int pin, pokeys_DigitalOutput_t* pt_IoObject, int n)
{
    int retval;

    /* export write only HAL pin for the input bit

    <device-name>.<device-num>.<io-type>.<chan-num>.<specific-name>

    */


    retval = hal_pin_bit_newf(HAL_OUT, pt_IoObject->out + (2 * n), comp_id, "pokeys.%d.digout.%02d.out", devicenum, pin);
    if (retval != 0) {
        return retval;
    }
    /* export parameter for polarity */
    retval = hal_param_bit_newf(HAL_RW, pt_IoObject->invert, comp_id, "pokeys.%d.digout.%02d.invert", devicenum, pin);

   return retval;

}

static int export_input_pin(int devicenum, int pin, hal_bit_t ** base, int n)
{
    int retval;

    /* export write only HAL pin for the input bit */
    retval = hal_pin_bit_newf(HAL_OUT, base + (2 * n), comp_id,
            "pokeys.%d.pin-%02d-in", devicenum, pin);
    if (retval != 0) {
	return retval;
    }
    /* export another write only HAL pin for the same bit inverted */
    retval = hal_pin_bit_newf(HAL_OUT, base + (2 * n) + 1, comp_id,
            "pokeys.%d.pin-%02d-in-not", devicenum, pin);
    return retval;
}

static int export_output_pin(int devicenum, int pin, hal_bit_t ** dbase,
    hal_bit_t * pbase, hal_bit_t * rbase, int n)
{
    int retval;

    /* export read only HAL pin for output data */
    retval = hal_pin_bit_newf(HAL_IN, dbase + n, comp_id,
            "pokeys.%d.pin-%02d-out", devicenum, pin);
    if (retval != 0) {
	return retval;
    }

    /* export parameter for reset */
    if (rbase)
	retval = hal_param_bit_newf(HAL_RW, rbase + n, comp_id,
		"pokeys.%d.pin-%02d-out-reset", devicenum, pin);
    return retval;
}
