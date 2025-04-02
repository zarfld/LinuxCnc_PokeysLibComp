/**
 * @file pokeys_homecomp.c
 * @ingroup pokeys_homecomp
 * @brief HAL component for managing homing sequence and state logic using PoKeys PulseEngine v2.
 *
 * This component implements a state machine to control synchronized homing of multiple joints
 * based on PoKeys Pulse Engine v2 capabilities. It interacts with other components by exchanging
 * `command` and `status` signals and monitors joint-specific flags such as `is_homed`.
 *
 * Features:
 * - Homing coordination across multiple axes
 * - HAL pin exports for command and status per joint
 * - Integration with the PoKeys PulseEngine hardware abstraction
 * - Debug and tracing via HAL pins or internal flags
 *
 * The component is designed for use with LinuxCNC and PoKeys57E devices, and provides
 * real-time feedback and control of the homing process using HAL-compatible pins.
 *
 * \dot
 * digraph AxisHoming {
 *   rankdir=LR;
 *   splines=polyline;
 *   node [fontname="Helvetica"];
 *
 *   // Global Homing Sequence states (LinuxCNC side)
 *   subgraph cluster_sequence {
 *     label="Homing-Sequenz (global)";
 *     style=rounded;
 *     color=gray;
 *     node [shape=box, style=rounded];
 *     seq_idle  [label="HOME_SEQUENCE_IDLE"];
 *     seq_start [label="HOME_SEQUENCE_START"];
 *     seq_wait  [label="HOME_SEQUENCE_WAIT_JOINTS"];
 *     // initial state indicator for sequence
 *     startS [shape=point, label=""];
 *     startS -> seq_idle;
 *   }
 *
 *   // Axis states (PoKeys Pulse Engine side)
 *   subgraph cluster_axis {
 *     label="Achsen-Zustände (PoKeys PulseEngine)";
 *     style=rounded;
 *     color=gray;
 *     node [shape=ellipse];
 *     idle    [label="axREADY (Idle)"];
 *     reset   [label="axHOMING_RESETTING"];
 *     backoff [label="axHOMING_BACKING_OFF"];
 *     search  [label="axHOMINGSEARCH"];
 *     back    [label="axHOMINGBACK"];
 *     homed   [label="axHOME (Homed)"];
 *     // initial state indicator for axis
 *     startA [shape=point, label=""];
 *     startA -> idle;
 *   }
 *
 *   // Homing sequence transitions
 *   seq_idle  -> seq_start [label="Homing gestartet (Benutzer)"];
 *   seq_start -> seq_wait  [label="Homing läuft (warte auf Abschluss)"];
 *   seq_wait  -> seq_idle  [label="Alle Achsen referenziert"];
 *
 *   // Interaction between sequence and axis: start homing command
 *   seq_start -> reset  [label="PK_PEAxisCommand_axHOMINGSTART\n(über PoKeysCompPulseEngine_v2)"];
 *
 *   // Axis homing state transitions (internal device progression)
 *   reset  -> backoff [label="(Home-Schalter aktiv)", style=dashed];
 *   reset  -> search  [label="(Home-Schalter nicht aktiv)"];
 *   backoff -> search [label="Schalter freigegeben"];
 *   search -> back    [label="Home-Schalter gefunden (Feinfahrt)"];
 *   back   -> homed   [label="Referenzpunkt erreicht"];
 *
 *   // Interaction: homing complete – finalize (after all axes in sequence are done)
 *   homed -> idle [label="PK_PEAxisCommand_axHOMINGFINALIZE\n(über PoKeysCompPulseEngine_v2, ausgelöst in PoKeysCompPulseEngine_base)"];
 *
 *   // Abort path: Homing cancel command (can be issued during search/back)
 *   search -> idle [label="PK_PEAxisCommand_axHOMINGCANCEL\n(Abbruch)", style=dashed];
 *   back   -> idle [label="PK_PEAxisCommand_axHOMINGCANCEL\n(Abbruch)", style=dashed];
 * }
 * \enddot
 * @author zarfld
 * @date 2025
 */

/**
* @brief Sequence diagram: Homing flow (PoKeys Pulse Engine v2)
*
* @dot
* digraph homing_flow {
*   rankdir=LR;
*   node [shape=box];

*   user       [label="User (GUI/MDI)"];
*   controller [label="emcmotController()"];
*   hal        [label="pokeys_homecomp"];
*   device     [label="PKPEv2_Update()"];
*   firmware   [label="PoKeys Device"];

*   user -> controller [label="do_home_all()"];
*   controller -> hal [label="do_homing() (cyclic)"];
*   hal -> hal [label="do_homing_sequence()"];
*   hal -> hal [label="pokeys_1joint_state_machine(j)"];
*   hal -> hal [label="Set PEv2_AxesCommand[j]"];
*   controller -> device [label="PKPEv2_Update(dev)"];

*   device -> firmware [label="Trigger: HomingStart / Finalize / ArmEncoder / FinalMove"];
*   firmware -> device [label="Report: PEv2_AxesState[j]"];
*   device -> hal [label="Update: PEv2_AxesState[j]"];

*   hal -> hal [label="Wait for sync state"];
*   hal -> hal [label="Advance home_state"];
*   hal -> controller [label="do_homing() returns true"];
* }
* @enddot
*
* This diagram shows the full cyclic interaction during a homing operation.
* - The HAL layer (`pokeys_homecomp`) sets the desired command via `PEv2_AxesCommand`
* - The low-level library (`PKPEv2_Update()`) maps these to firmware triggers
* - Firmware executes homing steps and reports status back via `PEv2_AxesState`
* - The HAL FSMs (`do_homing_sequence()` and `pokeys_1joint_state_machine()`) react to the reported state
* - Homing completes when all axes in the sequence reach final states and the HAL component returns true
*/

#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "rtapi_math64.h"

static int comp_id;

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "component:pokeys_homecomp:homing module template");
MODULE_INFO(linuxcnc, "descr:\nExample of a homing module buildable with "
                      "halcompile.\nDemonstrates required code for #includes, function "
                      "definitions, etc.\n\nIf \\fBHOMING_BASE\\fR is #defined and points to a "
                      "valid homing.c file,\nan example of a customized homing module is built.  "
                      "This module\ncreates input hal pins joint.n.request-custom-homing that "
                      "enable an\nalternate joint homing state machine for requested joints.  A "
                      "hal output\npin joint.N.is_custom-homing verifies selection\"\n\nThe "
                      "customized homing module utilizes many of the base homing api\nroutines "
                      "from homing.c without modification but augments other base\nfunctions to "
                      "add support for custom hal pins and custom joint homing\nstate machines.  "
                      "A user-built module will likely replace additional\napi functions or "
                      "augment them with other customizations.\n\nIf \\fBHOMING_BASE\\fR Is not "
                      "#defined, an  actual homing scheme is\n\\fBnot\\fR implemented but all "
                      "necessary functions are included as\nskeleton code.   (All joints are "
                      "effectively homed at all times and\ncannot be unhomed).\n\nSee the source "
                      "code file: src/emc/motion/homing.c for the baseline\nimplementation that "
                      "includes all functions for the default \\fBhomemod\\fR\nmodule.\n\nTo "
                      "avoid updates that overwrite homecomp.comp, best practice is\nto rename "
                      "the file and its component name (example:\n\\fBuser_homecomp.comp, "
                      "user_homecomp\\fR).\n\nThe (renamed) component can be built and installed "
                      "with\nhalcompile and then substituted for the default homing "
                      "module\n(\\fBhomemod\\fR) using:\n\n  $ linuxcnc \\fB-m user_homecomp\\fR "
                      "someconfig.ini\n\nor by inifile setting: "
                      "\\fB[EMCMOT]HOMEMOD=user_homecomp\\fR\n\n\\fBNote:\\fRIf using a deb "
                      "install:\n\n1) halcompile is provided by the package linuxcnc-dev\n\n2) "
                      "This source file for BRANCHNAME (master,2.9,etc) is downloadable from "
                      "github:\n\nhttps://github.com/LinuxCNC/linuxcnc/blob/BRANCHNAME/src/hal/"
                      "components/homecomp.comp\n");
MODULE_INFO(linuxcnc, "pin:is_module:bit:0:out::1:None");
MODULE_INFO(linuxcnc, "license:GPL");
MODULE_INFO(linuxcnc, "author:Dewey Garrett");
MODULE_LICENSE("GPL");
#endif // MODULE_INFO

/**
 * @defgroup pokeys_homecomp PoKeys Homing Component
 * @ingroup pokeys_components
 * @brief LinuxCNC HAL component for managing homing sequences via PoKeys PulseEngine v2.
 *
 * This module implements a finite state machine for handling homing commands and statuses
 * for each axis controlled by a PoKeys device. It communicates via HAL pins and interacts
 * with PulseEngine v2 state and command structures.
 *
 * The component is suitable for synchronized multi-axis homing and integrates tightly
 * with other components like `pokeys.comp`.
 *
 * @{
 */

/**
 * @class home_state_t
 * @brief Internal state representation for the homing state machine of one joint.
 *
 * Holds flags and counters required to manage the homing process for an individual axis.
 * Used internally by the `pokeys_homecomp` HAL component.
 */

struct __comp_state {
    struct __comp_state *_next;
    hal_bit_t *is_module;
};
struct __comp_state *__comp_first_inst = 0, *__comp_last_inst = 0;

static int __comp_get_data_size(void);
static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg);
#undef TRUE
#define TRUE (1)
#undef FALSE
#define FALSE (0)
#undef true
#define true (1)
#undef false
#define false (0)

static int export(char *prefix, long extra_arg) {
    int r = 0;
    int sz = sizeof(struct __comp_state) + __comp_get_data_size();
    struct __comp_state *inst = hal_malloc(sz);
    memset(inst, 0, sz);
    r = extra_setup(inst, prefix, extra_arg);
    if (r != 0)
        return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->is_module), comp_id, "%s.is-module", prefix);
    if (r != 0)
        return r;
    *(inst->is_module) = 1;
    if (__comp_last_inst)
        __comp_last_inst->_next = inst;
    __comp_last_inst = inst;
    if (!__comp_first_inst)
        __comp_first_inst = inst;
    return 0;
}
int rtapi_app_main(void) {
    int r = 0;
    comp_id = hal_init("pokeys_homecomp");
    if (comp_id < 0)
        return comp_id;
    r = export("pokeys-homecomp", 0);
    if (r) {
        hal_exit(comp_id);
    } else {
        hal_ready(comp_id);
    }
    return r;
}

void rtapi_app_exit(void) {
    hal_exit(comp_id);
}

#undef FUNCTION
#define FUNCTION(name) static void name(struct __comp_state *__comp_inst, long period)
#undef EXTRA_SETUP
#define EXTRA_SETUP() static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg)
#undef EXTRA_CLEANUP
#define EXTRA_CLEANUP() static void extra_cleanup(void)
#undef fperiod
#define fperiod (period * 1e-9)
#undef is_module
#define is_module (*__comp_inst->is_module)

// #line 51 "pokeys_homecomp.comp"

/* To incorporate default homing.c file from a local git src tree:
** enable #define HOMING_BASE set to the path to the current homing.c file.
** (Note: CUSTOM_HOMEMODULE precludes duplicate api symbols)
** (Edit myname as required for valid path)
*/

// #define HOMING_BASE /home/myname/linuxcnc-dev/src/emc/motion/homing.c

#define STR(s) #s
#define XSTR(s) STR(s)

#include "motion.h"
#include "homing.h"
#include <stdlib.h>

static char *home_parms;
RTAPI_MP_STRING(home_parms, "Example home parms");

// EXTRA_SETUP is executed before rtapi_app_main()
/**
 * @brief Extra setup function
 * @param home_parms
 * @return 0 on success, -1 on failure
 */
EXTRA_SETUP() {
    if (!home_parms) {
        home_parms = "no_home_parms";
    }
    rtapi_print("@@@%s:%s: home_parms=%s\n", __FILE__, __FUNCTION__, home_parms);
#ifndef HOMING_BASE
    rtapi_print("\n!!!%s: Skeleton Homing Module\n", __FILE__);
#else
    rtapi_print("\n!!!%s: HOMING_BASE=%s\n"
                "!!!Customize using hal pin(s): joint.N.request-custom-homing\n",
                __FILE__, XSTR(HOMING_BASE));
#endif

    return 0;
}

//=====================================================================
// All (skeleton) functions required for homing api follow:
static emcmot_joint_t *joints;
static bool homing_active = 0;
static bool homing_active_old = 0;
static int all_joints = 0;
static int current_sequence = 0;

static bool index_search_active = 1;
static bool index_search_armed = 0;
/**
 * @brief State of the homing sequence
 * @param joint_num
 * @return 1 if at least one joint is homing
 */
typedef enum { HOME_SEQUENCE_IDLE = 0, HOME_SEQUENCE_START, HOME_SEQUENCE_DO_ONE_JOINT, HOME_SEQUENCE_DO_ONE_SEQUENCE, HOME_SEQUENCE_START_JOINTS, HOME_SEQUENCE_WAIT_JOINTS } home_sequence_state_t;
static home_sequence_state_t sequence_state = HOME_SEQUENCE_IDLE;

/**
 * @brief Enumeration of the homing state of pokeys component

* @ingroup PoKeys_PEv2_AxisState */
typedef enum {
    PK_PEAxisState_axSTOPPED = 0, // Axis is stopped
    PK_PEAxisState_axREADY = 1,   // Axis ready
    PK_PEAxisState_axRUNNING = 2, // Axis is running

    PEAxisStateEx_axReadyToFinalizeHoming = 3, // (linuxcnc spec additional state) pokeys ready to finalize PKhoming
    PEAxisStateEx_axReadyToArmEncoder = 4,     // (linuxcnc spec additional state) pokeys ready to finalize PKhoming

    PK_PEAxisState_axHOMING_RESETTING = 8,   // Stopping the axis to reset the position counters
    PK_PEAxisState_axHOMING_BACKING_OFF = 9, // Backing off switch
    PK_PEAxisState_axHOME = 10,              // Axis is homed
    PK_PEAxisState_axHOMINGSTART = 11,       // Homing procedure is starting on axis
    PK_PEAxisState_axHOMINGSEARCH = 12,      // Homing procedure first step - going to home
    PK_PEAxisState_axHOMINGBACK = 13,        // Homing procedure second step - slow homing

    PK_PEAxisState_axPROBED = 14,      // Probing completed for this axis
    PK_PEAxisState_axPROBESTART = 15,  // Probing procedure is starting on axis
    PK_PEAxisState_axPROBESEARCH = 16, // Probing procedure - probing

    PEAxisStateEx_HOMINGARMENCODER = 17,    // (linuxcnc spec additional state) pokeys resets encoder position to zeros
    PEAxisStateEx_HOMINGWaitFINALMOVE = 18, // (linuxcnc spec additional state) Pokeys moves to homeposition
    PEAxisStateEx_HOMINGFINALMOVE = 19,     // (linuxcnc spec additional state) Pokeys moves to homeposition

    PK_PEAxisState_axERROR = 20, // Axis error
    PK_PEAxisState_axLIMIT = 30  // Axis limit tripped
} pokeys_home_state_t;

/**
 * @brief Commands to the pulse engine
 * @param joint_num
 * @return 1 if at least one joint is homing
 */
typedef enum {
    PK_PEAxisCommand_axIDLE = 0,                // Axis  in IDLE
    PK_PEAxisCommand_axHOMINGSTART = 1,         // Start Homing procedure
    PK_PEAxisCommand_axARMENCODER = 2,          // reset position to zeros
    PK_PEAxisCommand_axHOMINGWAITFINALMOVE = 3, // sync before move to homeposition
    PK_PEAxisCommand_axHOMINGFINALMOVE = 4,     // move to homeposition
    PK_PEAxisCommand_axHOMINGCANCEL = 5,        // Cancel Homing procedure
    PK_PEAxisCommand_axHOMINGFINALIZE = 6,      // Finish Homing procedure
} pokeys_home_command_t;

/**
 * @brief State machine for the homing process
 * @param joint_num
 * @return 1 if at least one joint is homing
 */
typedef enum {
    HOME_IDLE = 0,
    HOME_START,                 // 1
    HOME_UNLOCK,                // 2
    HOME_UNLOCK_WAIT,           // 3
    HOME_INITIAL_BACKOFF_START, // 4
    HOME_INITIAL_BACKOFF_WAIT,  // 5
    HOME_INITIAL_SEARCH_START,  // 6
    HOME_INITIAL_SEARCH_WAIT,   // 7
    HOME_SET_COARSE_POSITION,   // 8
    HOME_FINAL_BACKOFF_START,   // 9
    HOME_FINAL_BACKOFF_WAIT,    // 10
    HOME_RISE_SEARCH_START,     // 11
    HOME_RISE_SEARCH_WAIT,      // 12
    HOME_FALL_SEARCH_START,     // 13
    HOME_FALL_SEARCH_WAIT,      // 14
    HOME_SET_SWITCH_POSITION,   // 15
    HOME_INDEX_ONLY_START,      // 16
    HOME_INDEX_SEARCH_START,    // 17
    HOME_INDEX_SEARCH_WAIT,     // 18
    HOME_SET_INDEX_POSITION,    // 19
    HOME_FINAL_MOVE_START,      // 20
    HOME_FINAL_MOVE_WAIT,       // 21
    HOME_LOCK,                  // 22
    HOME_LOCK_WAIT,             // 23
    HOME_FINISHED,              // 24
    HOME_ABORT                  // 25
} local_home_state_t;

const char *const PEv2_AxisCommand_Names[] = {
    [PK_PEAxisCommand_axIDLE] = "PK_PEAxisCommand_axIDLE", [PK_PEAxisCommand_axHOMINGSTART] = "PK_PEAxisCommand_axHOMINGSTART", [PK_PEAxisCommand_axARMENCODER] = "PK_PEAxisCommand_axARMENCODER", [PK_PEAxisCommand_axHOMINGWAITFINALMOVE] = "PK_PEAxisCommand_axHOMINGWAITFINALMOVE", [PK_PEAxisCommand_axHOMINGFINALMOVE] = "PK_PEAxisCommand_axHOMINGFINALMOVE", [PK_PEAxisCommand_axHOMINGCANCEL] = "PK_PEAxisCommand_axHOMINGCANCEL", [PK_PEAxisCommand_axHOMINGFINALIZE] = "PK_PEAxisCommand_axHOMINGFINALIZE"
};

const char *const local_home_state_names[] = { [HOME_IDLE] = "HOME_IDLE",
                                               [HOME_START] = "HOME_START",
                                               [HOME_UNLOCK] = "HOME_UNLOCK",
                                               [HOME_UNLOCK_WAIT] = "HOME_UNLOCK_WAIT",
                                               [HOME_INITIAL_BACKOFF_START] = "HOME_INITIAL_BACKOFF_START",
                                               [HOME_INITIAL_BACKOFF_WAIT] = "HOME_INITIAL_BACKOFF_WAIT",
                                               [HOME_INITIAL_SEARCH_START] = "HOME_INITIAL_SEARCH_START",
                                               [HOME_INITIAL_SEARCH_WAIT] = "HOME_INITIAL_SEARCH_WAIT",
                                               [HOME_SET_COARSE_POSITION] = "HOME_SET_COARSE_POSITION",
                                               [HOME_FINAL_BACKOFF_START] = "HOME_FINAL_BACKOFF_START",
                                               [HOME_FINAL_BACKOFF_WAIT] = "HOME_FINAL_BACKOFF_WAIT",
                                               [HOME_RISE_SEARCH_START] = "HOME_RISE_SEARCH_START",
                                               [HOME_RISE_SEARCH_WAIT] = "HOME_RISE_SEARCH_WAIT",
                                               [HOME_FALL_SEARCH_START] = "HOME_FALL_SEARCH_START",
                                               [HOME_FALL_SEARCH_WAIT] = "HOME_FALL_SEARCH_WAIT",
                                               [HOME_SET_SWITCH_POSITION] = "HOME_SET_SWITCH_POSITION",
                                               [HOME_INDEX_ONLY_START] = "HOME_INDEX_ONLY_START",
                                               [HOME_INDEX_SEARCH_START] = "HOME_INDEX_SEARCH_START",
                                               [HOME_INDEX_SEARCH_WAIT] = "HOME_INDEX_SEARCH_WAIT",
                                               [HOME_SET_INDEX_POSITION] = "HOME_SET_INDEX_POSITION",
                                               [HOME_FINAL_MOVE_START] = "HOME_FINAL_MOVE_START",
                                               [HOME_FINAL_MOVE_WAIT] = "HOME_FINAL_MOVE_WAIT",
                                               [HOME_LOCK] = "HOME_LOCK",
                                               [HOME_LOCK_WAIT] = "HOME_LOCK_WAIT",
                                               [HOME_FINISHED] = "HOME_FINISHED",
                                               [HOME_ABORT] = "HOME_ABORT" };

static const char *const PK_PEState_names[] = { [PK_PEAxisState_axSTOPPED] = "PK_PEAxisState_axSTOPPED",
                                                [PK_PEAxisState_axINTERNAL] = "PK_PEAxisState_axINTERNAL",
                                                [PK_PEAxisState_axBUFFER] = "PK_PEAxisState_axBUFFER",
                                                [PK_PEAxisState_axRUNNING] = "PK_PEAxisState_axRUNNING",

                                                [PK_PEAxisState_axJOGGING] = "PK_PEAxisState_axJOGGING",
                                                [PK_PEAxisState_axSTOPPING] = "PK_PEAxisState_axSTOPPING",

                                                [PK_PEAxisState_axHOME] = "PK_PEAxisState_axHOME",
                                                [PK_PEAxisState_axHOMING] = "PK_PEAxisState_axHOMING",

                                                [PK_PEAxisState_axPROBECOMPLETE] = "PK_PEAxisState_axPROBECOMPLETE",
                                                [PK_PEAxisState_axPROBE] = "PK_PEAxisState_axPROBE",
                                                [PK_PEAxisState_axPROBEERROR] = "PK_PEAxisState_axPROBEERROR",

                                                [PK_PEAxisState_axHYBRIDPROBE_STOPPING] = "PK_PEAxisState_axHYBRIDPROBE_STOPPING",
                                                [PK_PEAxisState_axHYBRIDPROBE_COMPLETE] = "PK_PEAxisState_axHYBRIDPROBE_COMPLETE",

                                                [PK_PEAxisState_axSTOP_LIMIT] = "PK_PEAxisState_axSTOP_LIMIT",
                                                [PK_PEAxisState_axSTOP_EMERGENCY] = "PK_PEAxisState_axSTOP_EMERGENCY" };

static const char *const PK_PEAxisState_names[] = { [PK_PEAxisState_axSTOPPED] = "PK_PEAxisState_axSTOPPED",
                                                    [PK_PEAxisState_axREADY] = "PK_PEAxisState_axREADY",
                                                    [PK_PEAxisState_axRUNNING] = "PK_PEAxisState_axRUNNING",
                                                    [PK_PEAxisState_axHOMING_RESETTING] = "PK_PEAxisState_axHOMING_RESETTING",
                                                    [PK_PEAxisState_axHOMING_BACKING_OFF] = "PK_PEAxisState_axHOMING_BACKING_OFF",
                                                    [PK_PEAxisState_axHOME] = "PK_PEAxisState_axHOME",
                                                    [PK_PEAxisState_axHOMINGSTART] = "PK_PEAxisState_axHOMINGSTART",
                                                    [PK_PEAxisState_axHOMINGSEARCH] = "PK_PEAxisState_axHOMINGSEARCH",
                                                    [PK_PEAxisState_axHOMINGBACK] = "PK_PEAxisState_axHOMINGBACK",
                                                    [PK_PEAxisState_axPROBED] = "PK_PEAxisState_axPROBED",
                                                    [PK_PEAxisState_axPROBESTART] = "PK_PEAxisState_axPROBESTART",
                                                    [PK_PEAxisState_axPROBESEARCH] = "PK_PEAxisState_axPROBESEARCH",
                                                    [PK_PEAxisState_axERROR] = "PK_PEAxisState_axERROR",
                                                    [PK_PEAxisState_axLIMIT] = "PK_PEAxisState_axLIMIT",

                                                    [PEAxisStateEx_axReadyToFinalizeHoming] = "PEAxisStateEx_axReadyToFinalizeHoming",
                                                    [PEAxisStateEx_axReadyToArmEncoder] = "PEAxisStateEx_axReadyToArmEncoder",
                                                    [PEAxisStateEx_HOMINGARMENCODER] = "PEAxisStateEx_HOMINGARMENCODER",
                                                    [PEAxisStateEx_HOMINGWaitFINALMOVE] = "PEAxisStateEx_HOMINGWaitFINALMOVE",
                                                    [PEAxisStateEx_HOMINGFINALMOVE] = "PEAxisStateEx_HOMINGFINALMOVE" };

/**
 * @struct one_joint_home_data_t
 * @brief HAL pin structure for managing homing state per joint.
 *
 * This structure holds pointers to all relevant HAL pins that control and monitor
 * the homing process of a single joint.
 *
 * It includes:
 * - Standard HAL homing pins like home switch, homing status, and homed status
 * - Index handling support via `index_enable`
 * - Internal state tracking via `home_state`
 * - Communication pins with the PoKeys PulseEngine v2 for real-time motion control
 *
 * @note The members `PEv2_AxesState` and `PEv2_AxesCommand` are mapped directly
 *       to the HAL pins exported in the PulseEngine HAL component
 *       (see @ref PoKeysCompPulseEngine_v2).
 *
 * These allow `pokeys_homecomp` to read axis state (e.g., position reached, done)
 * and write commands (e.g., start homing) to the PulseEngine.
 *
 * @see ePoKeysPEState
 * @see ePoKeysPECmd
 * @see pokeys_homecomp
 * @see PoKeysCompPulseEngine_v2
 * @ingroup PoKeys_HomingState
 */
typedef struct {
    hal_bit_t *home_sw;      // home switch input
    hal_bit_t *homing;       // joint is homing
    hal_bit_t *homed;        // joint was homed
    hal_bit_t *index_enable; // motmod sets: request reset on index
                             //        encoder clears: index arrived
    hal_s32_t *home_state;   // homing state machine state

    // Custom Pins for comunication with PoKeys
    /**
     * @brief Current state of the PulseEngine for this joint.
     *
     * Pointer to the HAL pin exported by the PulseEngine HAL component,
     * typically named like `pokeys.PEv2.axis.N.state`, where `N` is the joint number.
     *
     * The value corresponds to an entry in the @ref ePoKeysPEState enum,
     * such as `PE_STATE_IDLE`, `PE_STATE_HOMING`, etc.
     *
     * Used by `pokeys_homecomp` to react to motion state changes during the homing sequence.
     *
     * @see ePoKeysPEState
     * @see PoKeysCompPulseEngine_v2
     */
    hal_u32_t *PEv2_AxesState; // State of pulse engine - see ePoKeysPEState

    /**
     * @brief Command output to the PulseEngine for this joint.
     *
     * Pointer to the HAL pin used to issue commands to the PulseEngine,
     * typically named `pokeys_homecomp.N.PEv2_AxesCommand` for joint N.
     *
     * The value corresponds to an entry in the @ref ePoKeysPECmd enum,
     * such as `PE_CMD_NONE`, `PE_CMD_HOME`, `PE_CMD_ENABLE`, etc.
     *
     * This value is evaluated by the PulseEngine logic (e.g., in `PoKeysCompPulseEngine_v2`)
     * and triggers state changes such as starting the homing sequence.
     *
     * @note Only written by `pokeys_homecomp`, and typically connected to
     * the corresponding `PEv2.axis.N.command` pin in the PulseEngine component.
     *
     * @see ePoKeysPECmd
     * @see PEv2_AxesState
     * @see PoKeysCompPulseEngine_v2
     */
    hal_u32_t *PEv2_AxesCommand; // Commands to  pulse engine - see ePoKeysPECmd

} one_joint_home_data_t;

/**
 * @brief Lokale Datenstruktur für einen Joint zur Homing-Verwaltung.
 *
 * Diese Struktur enthält die aktuellen lokalen Kopien der wichtigsten HAL-Pinwerte
 * sowie zusätzliche Parameter und Steuerinformationen. Sie wird intern vom
 * Homing-Framework (`pokeys_homecomp`) verwendet.
 *
 * @ingroup PoKeys_HomingState
 */
typedef struct {
    // pin data for all joints
    bool home_sw;
    bool homing;
    bool homed;
    bool index_enable;
    local_home_state_t home_state;

    // Custom Pins for comunication with PoKeys
    /**
     * @brief Local copy of the current PulseEngine state for this joint.
     *
     * This variable mirrors the value read from the HAL pin `PEv2_AxesState`
     * (e.g., `pokeys_homecomp.N.PEv2_AxesState`) and represents the current
     * state of the PulseEngine for this joint.
     *
     * It corresponds to the @ref ePoKeysPEState enumeration.
     *
     * Updated in @ref read_homing_in_pins and used for state machine transitions.
     *
     * @see one_joint_home_data_t::PEv2_AxesState
     * @see ePoKeysPEState
     */
    pokeys_home_state_t PEv2_AxesState;

    /**
     * @brief Local command to the PulseEngine for this joint.
     *
     * This variable holds the next command to be issued to the PulseEngine
     * (e.g., `PE_CMD_HOME`, `PE_CMD_NONE`, etc.) and will be written to
     * the corresponding HAL pin `PEv2_AxesCommand` during
     * @ref write_homing_out_pins.
     *
     * It corresponds to the @ref ePoKeysPECmd enumeration.
     *
     * @see one_joint_home_data_t::PEv2_AxesCommand
     * @see ePoKeysPECmd
     */
    pokeys_home_command_t PEv2_AxesCommand;

    // homeparams
    double offset;
    double home;
    double home_final_vel;
    double home_search_vel;
    double home_latch_vel;
    int home_flags;
    int home_sequence;
    bool volatile_home;

    bool home_is_synchronized;

    bool joint_in_sequence;
} home_local_data;

static home_local_data H[EMCMOT_MAX_JOINTS];

/**
 * @brief Containerstruktur für alle HAL-Pin-Sätze pro Joint.
 *
 * Wird bei der Initialisierung von HAL-Pins mit `hal_pin_*_newf()` verwendet.
 *
 * @ingroup PoKeys_HomingState
 */
typedef struct {
    one_joint_home_data_t jhd[EMCMOT_MAX_JOINTS];

} all_joints_home_data_t;

static all_joints_home_data_t *joint_home_data = 0;

/**
 * @brief Repräsentiert eine einzelne Homing-Sequenz mit einer Gruppe von Achsen (Joints).
 *
 * Diese Struktur definiert die Metadaten und den Zustand einer Homing-Sequenz.
 * Eine Sequenz kann mehrere Joints beinhalten, die gemeinsam oder in Abhängigkeit
 * voneinander ge-homed werden sollen. Sie wird von der `pokeys_homecomp` verwendet,
 * um die Abfolge der Homing-Schritte zu verwalten.
 *
 * Die Sequenzen sind durchnummeriert (über `home_sequence`), und es kann angegeben werden,
 * ob eine Sequenz die letzte in der Kette ist (`is_last`) und welche Sequenz als nächstes folgt (`next_sequence`).
 *
 * @ingroup PoKeys_HomingState
 */
typedef struct {
    int home_sequence; // my sequence ID
    bool homing;
    bool homed;

    int joints_in_sequence;
    int joint_ids[EMCMOT_MAX_JOINTS];

    bool is_last;
    int next_sequence;

    local_home_state_t home_state;

} one_sequence_home_data_t;

/**
 * @brief Struktur zur Verwaltung aller Homing-Sequenzen.
 *
 * Diese Struktur dient der Organisation und Kontrolle von Homing-Sequenzen in einem
 * System mit mehreren Achsen. Jeder Eintrag im `shd`-Array repräsentiert eine Sequenz,
 * die mehrere Joints beinhalten kann, welche gleichzeitig oder nacheinander homed werden.
 *
 * Die Felder `min_sequence`, `max_sequence` und `sequence_count` definieren die
 * gültigen Sequenzbereiche und ermöglichen eine automatische Abarbeitung in logischer
 * Reihenfolge. `current_sequence` gibt an, welche Sequenz aktuell aktiv ist.
 *
 * Wird vor allem in der Homing-Logik der `pokeys_homecomp` verwendet.
 *
 * @ingroup PoKeys_HomingState
 */
typedef struct {
    one_sequence_home_data_t shd[EMCMOT_MAX_JOINTS];
    int sequence_count;

    int min_sequence;
    int max_sequence;

    int current_sequence;

} all_sequences_home_data_t;

/**
 * @brief Globale Instanz zur Verwaltung aller Homing-Sequenzen.
 *
 * Diese Variable hält die aktuellen Zustände und Konfigurationen aller
 * Homing-Sequenzen im System. Sie wird in Funktionen wie
 * `update_sequence_home_data()`, `do_home_sequence()` und `check_home_sequence()`
 * aktiv verwendet und verändert.
 *
 * @ingroup PoKeys_HomingState
 */
static all_sequences_home_data_t sequence_home_data;
static bool allhomed = 0;
/**
 * @brief Erstellt HAL-Pins für Homing-Logik aller Joints.
 *
 * Diese Funktion reserviert und erstellt die notwendigen HAL-Pins pro Joint
 * für die Homing-State-Machine der pokeys_homecomp-Komponente.
 *
 * Die erzeugten Pins beinhalten Standard-Homing-Signale wie
 * `home-sw-in`, `homing`, `homed`, `home-state`, `index-enable` sowie
 * PoKeys-spezifische Signale `PEv2.AxesState` (Statusrückmeldung vom Controller)
 * und `PEv2.AxesCommand` (Kommando zum Homing-Steuergerät).
 *
 * @param id Die HAL-Komponenten-ID.
 * @param njoints Anzahl der zu konfigurierenden Joints.
 *
 * @retval 0 bei Erfolg, andernfalls negative Zahl.
 *
 * @ingroup PoKeys_HomingInit
 */
static int makepins(int id, int njoints) {
    // home_pins needed to work with configs expecting them:
    int jno, retval;
    one_joint_home_data_t *addr;

    joint_home_data = hal_malloc(sizeof(all_joints_home_data_t));
    if (joint_home_data == 0) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: all_joints_home_data_t malloc failed\n", __FILE__, __FUNCTION__);
        return -1;
    }

    retval = 0;
    for (jno = 0; jno < njoints; jno++) {
        addr = &(joint_home_data->jhd[jno]);

        retval += hal_pin_bit_newf(HAL_IN, &(addr->home_sw), id, "joint.%d.home-sw-in", jno);
        retval += hal_pin_bit_newf(HAL_OUT, &(addr->homing), id, "joint.%d.homing", jno);
        retval += hal_pin_bit_newf(HAL_OUT, &(addr->homed), id, "joint.%d.homed", jno);
        retval += hal_pin_s32_newf(HAL_OUT, &(addr->home_state), id, "joint.%d.home-state", jno);
        retval += hal_pin_bit_newf(HAL_IO, &(addr->index_enable), id, "joint.%d.index-enable", jno);

        // add custom hal pins here:
        retval += hal_pin_u32_newf(HAL_IN, &(addr->PEv2_AxesState), id, "joint.%d.PEv2.AxesState", jno);
        retval += hal_pin_u32_newf(HAL_OUT, &(addr->PEv2_AxesCommand), id, "joint.%d.PEv2.AxesCommand", jno);
        /*retval += hal_pin_s32_newf(HAL_IO, &(addr->home_sequence), id,
                                      "joint.%d.home_sequence", jno);*/
    }
    return retval;
}

/**
 * @brief Initialisiert Funktionen für das Homing von rotatorischen Achsen.
 *
 * Diese Funktion dient als Schnittstelle zur Übergabe von Funktionen für das
 * Entsperren und Abfragen von Rotary-Achsen (z. B. Drehachsen mit mechanischer Verriegelung).
 * Derzeit ist sie als Platzhalter implementiert und enthält keine Logik.
 *
 * @param pSetRotaryUnlock Funktionszeiger, der eine Achse und einen Status übergibt,
 *                         um die Achse zu entsperren.
 *                         Signature: `void func(int joint, int unlock)`
 * @param pGetRotaryIsUnlocked Funktionszeiger zur Abfrage, ob eine Achse entsperrt ist.
 *                             Signature: `int func(int joint)` (Rückgabewert: 1 = entsperrt)
 *
 * @ingroup PoKeys_HomingRotary
 */
void homeMotFunctions(void (*pSetRotaryUnlock)(int, int), int (*pGetRotaryIsUnlocked)(int)) {
    return;
}

/**
 * @brief Aktualisiert die Metadaten zu allen Homing-Sequenzen.
 *
 * Diese Funktion ermittelt die kleinste und größte verwendete `home_sequence`
 * aller Achsen und berechnet daraus `min_sequence`, `max_sequence` und `sequence_count`.
 * Anschließend wird für jede Sequenz die Liste der beteiligten Joints gefüllt.
 *
 * Außerdem wird gesetzt:
 * - `is_last`: Markierung für das Ende der Sequenzkette.
 * - `next_sequence`: Nächste Sequenz in der Kette.
 * - `home_state`: Auf `HOME_IDLE` initialisiert.
 *
 * Hinweis: Diese Funktion *überschreibt nicht* aktiv laufende `homing`- oder `homed`-Flags,
 * um laufende Homing-Prozesse nicht zu unterbrechen.
 *
 * @ingroup PoKeys_HomingSequence
 * @ingroup PoKeys_HomingInit
 */
static void update_sequence_home_data(void) {
    int min_sequence = abs(H[0].home_sequence);
    int max_sequence = abs(H[0].home_sequence);
    int sequence_count = 0;
    int current_sequence = 0;

    for (int jno = 0; jno < all_joints; jno++) {
        int seq = abs(H[jno].home_sequence);
        if (seq < min_sequence) {
            min_sequence = seq; // find min sequence
        }
        if (seq > max_sequence) {
            max_sequence = seq; // find max sequence
        }
    }
    sequence_count = max_sequence - min_sequence + 1;
    rtapi_print_msg(RTAPI_MSG_DBG,
                    "PoKeys_homecomp: %s:%s: update_sequence_home_data(min_sequence:%d  to "
                    "max_sequence:%d)\n",
                    __FILE__, __FUNCTION__, min_sequence, max_sequence);
    sequence_home_data.min_sequence = min_sequence;
    sequence_home_data.max_sequence = max_sequence;
    sequence_home_data.sequence_count = sequence_count;

    for (int sno = min_sequence; sno <= max_sequence; sno++) {
        one_sequence_home_data_t saddr = (sequence_home_data.shd[sno]);
        sequence_home_data.shd[sno].home_sequence = sno;
        // saddr.homing = 0; should not interrupt homing
        // saddr.homed = 0;
        sequence_home_data.shd[sno].joints_in_sequence = 0;
        if (sno == max_sequence) {
            sequence_home_data.shd[sno].is_last = 1;
            sequence_home_data.shd[sno].next_sequence = 0;
        } else {
            sequence_home_data.shd[sno].is_last = 0;
            sequence_home_data.shd[sno].next_sequence = sno + 1;
        }

        sequence_home_data.shd[sno].home_state = HOME_IDLE;

        for (int jno = 0; jno < all_joints; jno++) {
            if (abs(H[jno].home_sequence) == sno) {
                sequence_home_data.shd[sno].joint_ids[saddr.joints_in_sequence] = jno;
                sequence_home_data.shd[sno].joints_in_sequence++;
            }
        }

        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: update_sequence_home_data(%d) joints_in_sequence:%d\n", __FILE__, __FUNCTION__, sno, sequence_home_data.shd[sno].joints_in_sequence);
    }
}

/**
 * @brief Initialisiert die Homing-Datenstrukturen für alle Achsen.
 *
 * Diese Funktion wird beim Start des Homing-Subsystems aufgerufen und setzt
 * alle Zustände und Parameter der Homing-Finite-State-Machine auf definierte Anfangswerte.
 *
 * Dabei wird auch der Ausgangszustand der PEv2-Achsenkommunikation gesetzt:
 * - PEv2_AxesState = `PK_PEAxisState_axSTOPPED`
 * - PEv2_AxesCommand = `PK_PEAxisCommand_axIDLE`
 *
 * @param id HAL-Komponenten-ID
 * @param servo_period Servoperiode in Sekunden
 * @param n_joints Anzahl der konfigurierten Achsen
 * @param n_extrajoints Zusätzliche Achsen (z. B. Spindel)
 * @param pjoints Pointer auf die `emcmot_joint_t`-Struktur aller Achsen
 * @return 0 bei Erfolg, sonst Fehlercode von `makepins()`
 *
 * @ingroup PoKeys_HomingFSM
 * @ingroup PoKeys_PEv2_AxisState
 * @ingroup PoKeys_PEv2_AxisCommand
 */
int homing_init(int id, double servo_period, int n_joints, int n_extrajoints, emcmot_joint_t *pjoints) {
    joints = pjoints;
    all_joints = n_joints;
    return makepins(id, n_joints);

    // initialize jid[] and jhd[] data here
    for (int jno = 0; jno < EMCMOT_MAX_JOINTS; jno++) {

        H[jno].offset = 0;
        H[jno].home = 0;
        H[jno].home_final_vel = 0;
        H[jno].home_search_vel = 0;
        H[jno].home_latch_vel = 0;
        H[jno].home_flags = 0;
        H[jno].home_sequence = 0;
        H[jno].volatile_home = 1;

        one_joint_home_data_t *addr = &(joint_home_data->jhd[jno]);
        H[jno].home_sw = 0;
        H[jno].homing = 0;
        H[jno].homed = 0;
        H[jno].home_state = HOME_IDLE;
        H[jno].index_enable = index_search_armed;

        H[jno].PEv2_AxesState = PK_PEAxisState_axSTOPPED;
        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;

        one_sequence_home_data_t saddr = (sequence_home_data.shd[jno]);
        saddr.home_sequence = 0;
        saddr.homing = 0;
        saddr.homed = 0;
        saddr.joints_in_sequence = 0;
        saddr.is_last = 0;
        saddr.next_sequence = 0;
        saddr.home_state = HOME_IDLE;
    }
}

/**
 * @brief Prüft, ob alle Achsen einer Homing-Sequenz erfolgreich ge-homed wurden.
 *
 * Diese Funktion iteriert über alle Achsen der gegebenen Sequenz und überprüft das `homed`-Flag.
 * Nur wenn *alle* Achsen in der Sequenz als homed markiert sind, wird `true` zurückgegeben.
 * Andernfalls erfolgt ein vorzeitiger Abbruch mit `false`.
 *
 * @param seq Die Indexnummer der Homing-Sequenz.
 * @return `true` (1), wenn alle Achsen der Sequenz ge-homed sind, sonst `false` (0).
 *
 * @ingroup PoKeys_HomingStatus
 * @ingroup PoKeys_HomingSequence
 */
bool get_sequence_homed(int seq) {
    one_sequence_home_data_t addr = (sequence_home_data.shd[seq]);
    int joints_in_sequence = addr.joints_in_sequence;

    for (int jj = 0; jj < joints_in_sequence; jj++) {
        int jno = addr.joint_ids[jj];
        if (!H[jno].homed) {
            return 0;
        }
    }
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: get_sequence_homed(%d) TRUE\n", __FILE__, __FUNCTION__, seq);
    return 1;
}

/**
 * @brief Prüft, ob in einer Homing-Sequenz derzeit noch mindestens eine Achse homed.
 *
 * Diese Funktion durchläuft alle Achsen in der angegebenen Sequenz und prüft,
 * ob für mindestens eine Achse das Flag `homing` aktiv ist. Sie wird z. B. in
 * der Zustandsmaschine verwendet, um festzustellen, ob eine Sequenz noch läuft
 * oder abgeschlossen ist.
 *
 * @param seq Die Indexnummer der Homing-Sequenz.
 * @return `true` (1), wenn mindestens eine Achse in der Sequenz noch homed, sonst `false` (0).
 *
 * @ingroup PoKeys_HomingStatus
 */
bool get_sequence_homing(int seq) {
    one_sequence_home_data_t addr = (sequence_home_data.shd[seq]);
    int joints_in_sequence = addr.joints_in_sequence;

    for (int jj = 0; jj < joints_in_sequence; jj++) {
        int jno = addr.joint_ids[jj];
        if (H[jno].homing) {
            return 1;
        }
    }

    return 0;
}

bool get_sequence_synchronized_state(int seq, int desired_state) {
    int joints_in_sequence = 0;
    int ready_in_sequence = 0;

    for (int jj = 0; jj < all_joints; jj++) {
        if (abs(H[jj].home_sequence) == abs(seq)) {
            joints_in_sequence++;
            if (H[jj].PEv2_AxesState == desired_state) {
                ready_in_sequence++;
            }
        }
    }

    if (joints_in_sequence == ready_in_sequence) {
    }
    return 1;
}
else {
    return 0;
}
}

/**
 * @brief Homing state machine handler for a single PoKeys-controlled joint.
 *
 * This function implements the homing logic for a single joint, using
 * feedback from the PoKeys Pulse Engine v2 axis state. It maps internal
 * PoKeys axis states to LinuxCNC-style homing state transitions and
 * drives the associated joint’s homing process forward.
 *
 * The function updates `H[joint_num].home_state`, `homed`, and `homing` flags,
 * and manages synchronization of multiple joints within the same `home_sequence`.
 *
 * The homing states follow the Pulse Engine v2 sequence and extend it with
 * LinuxCNC-style behaviors (e.g. index-enable logic, homing-finalize handoff).
 *
 *
 * This function synchronizes the internal PoKeys Pulse Engine v2 homing state
 * (`PEv2_AxesState`) with the LinuxCNC-side `home_state` and commands the corresponding
 * `PEv2_AxesCommand` when needed.
 *
 * @param joint_num Index of the joint to process.
 * @return 1 if homing is active for the joint, 0 otherwise.
 *
 * @dot
 * @startuml
 * participant LinuxCNC
 * participant "pokeys_homecomp" as HAL
 * participant PoKeys57 as Device

 * == Initialization ==
 * LinuxCNC -> HAL : call pokeys_1joint_state_machine(joint_num)
 * HAL -> HAL : read PEv2_AxesState
 * HAL -> HAL : update home_state accordingly

 * == Homing Start ==
 * HAL -> Device : set PEv2_AxesCommand = HOMINGSTART
 * HAL -> HAL : set home_state = HOME_UNLOCK

 * == Reset & Backoff ==
 * Device -> HAL : PEv2_AxesState = axHOMING_RESETTING
 * HAL -> HAL : home_state = HOME_UNLOCK
 * Device -> HAL : PEv2_AxesState = axHOMING_BACKING_OFF
 * HAL -> HAL : home_state = HOME_INITIAL_BACKOFF_WAIT

 * == Switch Search ==
 * Device -> HAL : PEv2_AxesState = axHOMINGSEARCH
 * HAL -> HAL : home_state = HOME_INITIAL_SEARCH_WAIT

 * == Final Backoff ==
 * Device -> HAL : PEv2_AxesState = axHOMINGBACK
 * HAL -> HAL : home_state = HOME_FINAL_BACKOFF_START
 * HAL -> Device : PEv2_AxesCommand = HOMINGFinalize
 * HAL -> HAL : home_state = HOME_INDEX_SEARCH_START

 * == Index Arming ==
 * Device -> HAL : PEv2_AxesState = ReadyToArmEncoder
 * HAL -> HAL : index_enable = true
 * HAL -> HAL : home_state = HOME_INDEX_SEARCH_WAIT
 * Device -> HAL : index pulse -> set home_state = HOME_SET_INDEX_POSITION
 * HAL -> Device : PEv2_AxesCommand = HOMINGWAITFINALMOVE

 * == Final Move ==
 * HAL -> Device : PEv2_AxesCommand = HOMINGFINALMOVE
 * HAL -> HAL : home_state = HOME_FINAL_MOVE_WAIT
 * Device -> HAL : PEv2_AxesState = axREADY
 * HAL -> Device : PEv2_AxesCommand = IDLE
 * HAL -> HAL : home_state = HOME_FINISHED
 * HAL -> HAL : mark homing complete

 * == Abort ==
 * Device -> HAL : PEv2_AxesState = axLIMIT / axERROR
 * HAL -> Device : PEv2_AxesCommand = HOMINGCANCEL
 * HAL -> HAL : home_state = HOME_ABORT -> HOME_IDLE

 * @enduml
 * @enddot
 */

bool Homing_ArmEncodereDone[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int jsm_AxesState_memory[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int jsm_home_state_memory[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
bool home_sw_active_memory[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int pokeys_1joint_state_machine(int joint_num) {
    emcmot_joint_t *joint;
    double offset, tmp;
    int home_sw_active, homing_flag;
    bool immediate_state = 0;

    homing_flag = 0;
    joint = &joints[joint_num];
    home_sw_active = H[joint_num].home_sw;
    if (home_sw_active_memory[joint_num] != home_sw_active) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] home_sw_active_memory != home_sw_active(%d)\n", __FILE__, __FUNCTION__, joint_num, home_sw_active);
        home_sw_active_memory[joint_num] = home_sw_active;
    }
    if (H[joint_num].home_state != HOME_IDLE) {
        homing_flag = 1; // at least one joint is homing
    }

    /* when a joint is homing, 'check_for_faults()' ignores its limit
       switches, so that this code can do the right thing with them. Once
       the homing process is finished, the 'check_for_faults()' resumes
       checking */

    /* homing state machine */

    /* Some portions of the homing sequence can run thru two or more
       states during a single servo period.  This is done using
       'immediate_state'.  If a state transition sets it true (non-zero),
       this 'do-while' will loop executing switch(home_state) immediately
       to run the new state code.  Otherwise, the loop will fall thru, and
       switch(home_state) runs only once per servo period. Do _not_ set
       'immediate_state' true unless you also change 'home_state', unless
       you want an infinite loop! */
    do {
        immediate_state = 0;
        int joints_in_sequence = 0;
        int ready_in_sequence = 0;
        /** switch to directly map PEv2_AxesState to home_state as transitions are done during pokeys homing, and status changes just report*/
        switch (H[joint_num].PEv2_AxesState) {
            case PK_PEAxisState_axSTOPPED:
                /* Axis is stopped */
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axSTOPPED\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // no home_state here - not exactly defined
                break;
            case PK_PEAxisState_axREADY:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axREADY\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // no home_state here - not exactly defined
                break;
            case PK_PEAxisState_axRUNNING:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axRUNNING\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // no home_state here - not exactly defined
                break;
            case PK_PEAxisState_axHOMING_RESETTING:

                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMING_RESETTING\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                if (H[joint_num].home_state != HOME_UNLOCK) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMING_RESETTING - set home_state = HOME_UNLOCK\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_UNLOCK;
                }
                break;
            case PK_PEAxisState_axHOMING_BACKING_OFF:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMING_BACKING_OFF\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                if (H[joint_num].home_state != HOME_INITIAL_BACKOFF_WAIT) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMING_BACKING_OFF - set home_state = HOME_INITIAL_BACKOFF_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_INITIAL_BACKOFF_WAIT;
                }
                break;
            case PK_PEAxisState_axHOME:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOME\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // no home_state here - not exactly defined
                break;
            case PK_PEAxisState_axHOMINGSTART:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMINGSTART\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                if (H[joint_num].home_state != HOME_START) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMINGSTART - set home_state = HOME_START\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_START;
                }
                break;
            case PK_PEAxisState_axHOMINGSEARCH:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMINGSEARCH\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                if (H[joint_num].home_state != HOME_INITIAL_SEARCH_WAIT) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMINGSEARCH - set home_state = HOME_INITIAL_SEARCH_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_INITIAL_SEARCH_WAIT;
                }
                break;
            case PK_PEAxisState_axHOMINGBACK:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMINGBACK\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }

                if (H[joint_num].home_state != HOME_FINAL_BACKOFF_START) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMINGBACK - set home_state = HOME_FINAL_BACKOFF_START\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_FINAL_BACKOFF_START;
                }
                break;
            case PEAxisStateEx_axReadyToFinalizeHoming:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_axReadyToFinalizeHoming\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // HOME_INDEX_SEARCH_START
                if (H[joint_num].home_state != HOME_INDEX_SEARCH_START) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_axReadyToFinalizeHoming - set home_state = HOME_INDEX_SEARCH_START\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_INDEX_SEARCH_START;
                }
                break;
            case PEAxisStateEx_axReadyToArmEncoder:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_axReadyToArmEncoder\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                if (H[joint_num].home_state != HOME_INDEX_SEARCH_START && H[joint_num].home_state != HOME_INDEX_SEARCH_WAIT) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_axReadyToArmEncoder - set home_state = HOME_INDEX_SEARCH_START\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_INDEX_SEARCH_START;
                }
                break;
            case PEAxisStateEx_HOMINGARMENCODER:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_HOMINGARMENCODER\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                if (H[joint_num].home_state != HOME_SET_INDEX_POSITION && H[joint_num].home_state != HOME_INDEX_SEARCH_WAIT) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_HOMINGARMENCODER - set home_state = HOME_SET_INDEX_POSITION\n", __FILE__, __FUNCTION__, joint_num);
                    // H[joint_num].home_state = HOME_SET_INDEX_POSITION;
                    H[joint_num].home_state = HOME_INDEX_SEARCH_WAIT;
                }
                break;
            case PEAxisStateEx_HOMINGWaitFINALMOVE:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_HOMINGWaitFINALMOVE\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // no home_state here - not exactly defined
                break;
            case PEAxisStateEx_HOMINGFINALMOVE:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PEAxisStateEx_HOMINGFINALMOVE\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // no home_state here - not exactly defined
                break;
            case PK_PEAxisState_axLIMIT:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axLIMIT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                // HOME_ABORT
                if (H[joint_num].home_state != HOME_ABORT) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axLIMIT - set home_state = HOME_ABORT\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_ABORT;
                }
                break;
            case PK_PEAxisState_axERROR:
                if (jsm_AxesState_memory[joint_num] != H[joint_num].PEv2_AxesState) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axERROR\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_AxesState_memory[joint_num] = H[joint_num].PEv2_AxesState;
                }
                if (H[joint_num].home_state != HOME_ABORT) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axERROR - set home_state = HOME_ABORT\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_ABORT;
                }
                break;
            default:
                /* should never get here */
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine unknown "
                                "state '%d' during homing j=%d",
                                __FILE__, __FUNCTION__, H[joint_num].home_state, joint_num);
                //  H[joint_num].home_state = HOME_ABORT;
                // immediate_state = 1;
                break;
        }

        int Set_PEAxisCommand;
        int Set_home_state;
        int requested_PEAxisState;
        const char* home_state_case;

        /** switch to analyz homestate */
        switch (H[joint_num].home_state) {
            case HOME_IDLE:
                /** @brief  nothing to do */
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_IDLE\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                break;
            case HOME_START:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_START\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }

                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] "
                                "homing start\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Homing procedure is starting on axis */
                if (!H[joint_num].homing) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_START - set homing=1\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].homing = 1;
                }
                if (!homing_flag) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_START - set homing_flag=1\n", __FILE__, __FUNCTION__, joint_num);
                    homing_flag = 1;
                }
                if (H[joint_num].homed) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_START - set homed=0\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].homed = 0;
                }

                Set_PEAxisCommand = PK_PEAxisCommand_axHOMINGSTART;
                Set_home_state = HOME_UNLOCK;
                home_state_case = "HOME_START";

                for (int jj = 0; jj < all_joints; jj++) {
                    if (abs(H[jj].home_sequence) == abs(seq)) {
                        if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                            H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                            "(home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                            __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                        }
                        if (H[joint_num].home_state != Set_home_state) {
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                            " (home_sequence %d) - set home_state=%s\n",
                                            __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                            H[joint_num].home_state = Set_home_state;
                        }
                    }
                }

                if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                    H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                    "homed (home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                    __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                }
                if (H[joint_num].home_state != Set_home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                    "homed (home_sequence %d) - set home_state=%s\n",
                                    __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                    H[joint_num].home_state = Set_home_state;
                }

                break;

            case HOME_UNLOCK:
                // Statetransition done at pokeys procedure
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_UNLOCK\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                // prevent following errors due to external movement
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;
                if (!H[joint_num].homing) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] PK_PEAxisState_axHOMING_RESETTING - set homing=1\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].homing = 1;
                }
                break;
            case HOME_UNLOCK_WAIT:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_UNLOCK_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }

                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;

                break;
            case HOME_INITIAL_BACKOFF_START:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_INITIAL_BACKOFF_START\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                break;
            case HOME_INITIAL_BACKOFF_WAIT:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_INITIAL_BACKOFF_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;
                /* Backing off switch */

                break;
            case HOME_INITIAL_SEARCH_START:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_INITIAL_SEARCH_START\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;
                break;
            case HOME_INITIAL_SEARCH_WAIT:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_INITIAL_SEARCH_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;
                /* Homing procedure first step - going to home */
                H[joint_num].homing = 1;

                homing_flag = 1;
                break;
            case HOME_SET_COARSE_POSITION:
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;
                break;

            case HOME_FINAL_BACKOFF_START:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_FINAL_BACKOFF_START\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;
                /* Homing procedure second step - slow homing */
                H[joint_num].homing = 1;
                homing_flag = 1;
                break;

            case HOME_FINAL_BACKOFF_WAIT:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_FINAL_BACKOFF_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;

                Set_PEAxisCommand = PK_PEAxisCommand_axHOMINGFINALIZE;
                Set_home_state = HOME_INDEX_SEARCH_START;
                requested_PEAxisState = PEAxisStateEx_axReadyToFinalizeHoming;
                home_state_case = "HOME_FINAL_BACKOFF_WAIT";
                if (get_sequence_synchronized_state(H[joint_num].home_sequence, requested_PEAxisState)) {
                    for (int jj = 0; jj < all_joints; jj++) {
                        if (abs(H[jj].home_sequence) == abs(seq)) {
                            if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                                H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                "(home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                            }
                            if (H[joint_num].home_state != Set_home_state) {
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                " (home_sequence %d) - set home_state=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                                H[joint_num].home_state = Set_home_state;
                            }
                        }
                    }

                    if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                        H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                    }
                    if (H[joint_num].home_state != Set_home_state) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set home_state=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                        H[joint_num].home_state = Set_home_state;
                    }
                }
                homing_flag = 1;
                break;
            case HOME_INDEX_SEARCH_START:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s:  joint[%d] HOME_INDEX_SEARCH_START\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }

                /** This state is called after the machine has made a low
                   speed pass to determine the limit switch location. It
                   sets index-enable, which tells the encoder driver to
                   reset its counter to zero and clear the enable when the
                   next index pulse arrives. */
                /** set the index enable */
                if (H[joint_num].index_enable != index_search_active) {
                    H[joint_num].index_enable = index_search_active;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] "
                                    "ready to arm encoder - index_search_active %d\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].index_enable);
                }
                H[joint_num].homing = 1;
                H[joint_num].home_state = HOME_INDEX_SEARCH_WAIT;
                homing_flag = 1;
                break;
            case HOME_INDEX_SEARCH_WAIT:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s:  joint[%d] HOME_INDEX_SEARCH_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                /** PEAxisStateEx_HOMINGARMENCODERThis state is called after the machine has found the
                       home switch and "armed" the encoder counter to reset on
                       the next index pulse. It continues at low speed until
                       an index pulse is detected, at which point it sets the
                       final home position.  If the move ends or hits a limit
                       before an index pulse occurs, the home is aborted. */

                /**  has an index pulse arrived yet? encoder driver clears
                   enable when it does */

                if (H[joint_num].index_enable == index_search_armed) {

                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] "
                                    "HOME_INDEX_SEARCH_WAIT - index pulse arrived joint->free_tp.enable %d\n",
                                    __FILE__, __FUNCTION__, joint_num, joint->free_tp.enable);

                    /* Pokeys resets encoder position to zeros */

                    H[joint_num].homing = 1;
                    H[joint_num].home_state = HOME_SET_INDEX_POSITION;
                    is being set from pokeye comp with immediate_state = 1;
                }
                homing_flag = 1;
                break;
            case HOME_SET_INDEX_POSITION:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_SET_INDEX_POSITION\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                /* yes, stop motion */
                joint->free_tp.enable = 0;
                // joint->free_tp.pos_cmd = joint->free_tp.curr_pos; // set to zero
                joint->free_tp.pos_cmd = 0.0; // set to zero
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] "
                                "homing arm encoder - index pulse arrived joint->free_tp.pos_cmd %f\n",
                                __FILE__, __FUNCTION__, joint_num, joint->free_tp.pos_cmd);

                if (H[joint_num].index_enable != index_search_armed) {
                    H[joint_num].index_enable = index_search_armed;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] "
                                    "homing arm encoder - index pulse arrived H[joint_num].index_enable %d\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].index_enable);
                }

                Set_home_state = HOME_FINAL_MOVE_START;
                home_state_case = "HOME_SET_INDEX_POSITION";
                if (H[joint_num].home_state != Set_home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                    "homed (home_sequence %d) - set home_state=%s\n",
                                    __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                    H[joint_num].home_state = Set_home_state;
                }

                Set_PEAxisCommand = PK_PEAxisCommand_axHOMINGWAITFINALMOVE;
                Set_home_state = HOME_FINAL_MOVE_START;
                requested_PEAxisState = PEAxisStateEx_HOMINGARMENCODER;
                home_state_case = "HOME_SET_INDEX_POSITION";
                if (get_sequence_synchronized_state(H[joint_num].home_sequence, requested_PEAxisState)) {
                    for (int jj = 0; jj < all_joints; jj++) {
                        if (abs(H[jj].home_sequence) == abs(seq)) {
                            if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                                H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                "(home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                            }
                            if (H[joint_num].home_state != Set_home_state) {
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                " (home_sequence %d) - set home_state=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                                H[joint_num].home_state = Set_home_state;
                            }
                        }
                    }

                    if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                        H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                    }
                    if (H[joint_num].home_state != Set_home_state) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set home_state=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                        H[joint_num].home_state = Set_home_state;
                    }
                }

                /**  set the index position */
                break;
            case HOME_FINAL_MOVE_START:
                /**  waits until all Joints in Sequence are Ready, then   */
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_FINAL_MOVE_START\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }

                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;

                /* waiting for sync before Pokeys moves to homeposition */
                Set_PEAxisCommand = PK_PEAxisCommand_axHOMINGFINALMOVE;
                Set_home_state = HOME_FINAL_MOVE_WAIT;
                requested_PEAxisState = PEAxisStateEx_HOMINGWaitFINALMOVE;
                home_state_case = "HOME_FINAL_MOVE_START";
                if (get_sequence_synchronized_state(H[joint_num].home_sequence, requested_PEAxisState)) {
                    for (int jj = 0; jj < all_joints; jj++) {
                        if (abs(H[jj].home_sequence) == abs(seq)) {
                            if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                                H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                "(home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                            }
                            if (H[joint_num].home_state != Set_home_state) {
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                " (home_sequence %d) - set home_state=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                                H[joint_num].home_state = Set_home_state;
                            }
                        }
                    }

                    if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                        H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                    }
                    if (H[joint_num].home_state != Set_home_state) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set home_state=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                        H[joint_num].home_state = Set_home_state;
                    }
                }

                homing_flag = 1;
                break;
            case HOME_FINAL_MOVE_WAIT:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: joint[%d] HOME_FINAL_MOVE_WAIT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                joint->free_tp.enable = 0;
                joint->free_tp.pos_cmd = joint->free_tp.curr_pos;
                joint->free_tp.vel_cmd = 0;

                Set_PEAxisCommand = PK_PEAxisCommand_axIDLE;
                Set_home_state = HOME_FINISHED;
                requested_PEAxisState = PK_PEAxisState_axREADY;
                home_state_case = "HOME_FINAL_MOVE_WAIT";
                if (get_sequence_synchronized_state(H[joint_num].home_sequence, requested_PEAxisState)) {
                    for (int jj = 0; jj < all_joints; jj++) {
                        if (abs(H[jj].home_sequence) == abs(seq)) {
                            if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                                H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                "(home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                            }
                            if (H[joint_num].home_state != Set_home_state) {
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                                " (home_sequence %d) - set home_state=%s\n",
                                                __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                                H[joint_num].home_state = Set_home_state;
                            }
                        }
                    }

                    if (H[joint_num].PEv2_AxesCommand != Set_PEAxisCommand) {
                        H[joint_num].PEv2_AxesCommand = Set_PEAxisCommand;
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set PEv2_AxesCommand=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, PEv2_AxisCommand_Names[Set_PEAxisCommand]);
                    }
                    if (H[joint_num].home_state != Set_home_state) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: %s joint[%d] "
                                        "homed (home_sequence %d) - set home_state=%s\n",
                                        __FILE__, __FUNCTION__, home_state_case, joint_num, H[joint_num].home_sequence, local_home_state_names[Set_home_state]);
                        H[joint_num].home_state = Set_home_state;
                    }
                }
                break;

            case HOME_FINISHED:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_FINISHED\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                /* Axis is homed */
                joints_in_sequence = 0;
                int homed_in_sequence = 0;

                for (int jj = 0; jj < all_joints; jj++) {
                    if (abs(H[jj].home_sequence) == abs(H[joint_num].home_sequence)) {
                        joints_in_sequence++;
                        if (H[jj].PEv2_AxesState == PK_PEAxisState_axHOME) {
                            homed_in_sequence++;
                        }
                    }
                }

                if (joints_in_sequence == homed_in_sequence) {
                    // if all Joints of the Sequence show Hommed
                    for (int jj = 0; jj < all_joints; jj++) {

                        if (abs(H[jj].home_sequence) == abs(H[joint_num].home_sequence)) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys_homecomp: %s:%s: PK_PEAxisState_axHOME "
                                            "joint[%d] homed (home_sequence %d)\n",
                                            __FILE__, __FUNCTION__, jj, H[jj].home_sequence);
                            if (!joint->free_tp.enable) {
                                //    joint->free_tp.enable = 1;
                                //       rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set free_tp.enable=1\n", __FILE__, __FUNCTION__, joint_num);
                            }
                            if (!H[jj].homed) {
                                /* Axis is homing */
                                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set homed=1\n", __FILE__, __FUNCTION__, joint_num);
                                H[jj].homed = 1;
                            }
                            if (H[jj].homing) {
                                /* Axis is homing */
                                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set homing=0\n", __FILE__, __FUNCTION__, joint_num);
                                H[jj].homing = 0;
                            }

                            if (H[jj].home_state != HOME_IDLE) {
                                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set homing=0\n", __FILE__, __FUNCTION__, joint_num);
                                H[jj].home_state = HOME_IDLE;
                            }
                            if (H[jj].PEv2_AxesCommand != PK_PEAxisCommand_axIDLE) {
                                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set PEv2_AxesCommand=0\n", __FILE__, __FUNCTION__, joint_num);
                                H[jj].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                            }
                        }
                    }
                    rtapi_print_msg(RTAPI_MSG_DBG,
                                    "PoKeys_homecomp: %s:%s: PK_PEAxisState_axHOME joint[%d] "
                                    "homed (home_sequence %d)\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].home_sequence);

                    if (!joint->free_tp.enable) {
                        //    joint->free_tp.enable = 1;
                        //       rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set free_tp.enable=1\n", __FILE__, __FUNCTION__, joint_num);
                    }
                    if (!H[joint_num].homed) {
                        /* Axis is homing */
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set homed=1\n", __FILE__, __FUNCTION__, joint_num);
                        H[joint_num].homed = 1;
                    }
                    if (H[joint_num].homing) {
                        /* Axis is homing */
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set homing=0\n", __FILE__, __FUNCTION__, joint_num);
                        H[joint_num].homing = 0;
                    }

                    if (H[joint_num].home_state != HOME_IDLE) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set homing=0\n", __FILE__, __FUNCTION__, joint_num);
                        H[joint_num].home_state = HOME_IDLE;
                    }
                    if (H[joint_num].PEv2_AxesCommand != PK_PEAxisCommand_axIDLE) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_FINAL_MOVE_WAIT - set PEv2_AxesCommand=0\n", __FILE__, __FUNCTION__, joint_num);
                        H[joint_num].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                    }
                }

                break;

            case HOME_ABORT:
                if (jsm_home_state_memory[joint_num] != H[joint_num].home_state) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] HOME_ABORT\n", __FILE__, __FUNCTION__, joint_num);
                    jsm_home_state_memory[joint_num] = H[joint_num].home_state;
                }
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint[%d] limit\n", __FILE__, __FUNCTION__, joint_num);
                /* Axis limit tripped */
                H[joint_num].homing = 0;
                H[joint_num].homed = 0;
                H[joint_num].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGCANCEL;
                H[joint_num].home_state = HOME_IDLE;
                break;

            default:
                /* should never get here */
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine unknown "
                                "state '%d' during homing j=%d",
                                __FILE__, __FUNCTION__, H[joint_num].home_state, joint_num);
                //  H[joint_num].home_state = HOME_ABORT;
                // immediate_state = 1;
                break;
        } /* end of switch(H[joint_num].home_state) */
    } while (immediate_state);

    return homing_flag;
} // pokeys_1joint_state_machine()

/**
 * @brief Starts the homing process for all joints in a homing sequence.
 *
 * This function iterates over all joints in the specified sequence `seq`, resets their homing flags,
 * initializes them to `HOME_START`, and sends the `PK_PEAxisCommand_axHOMINGSTART` command to the PoKeys
 * Pulse Engine if the current state allows it (i.e., STOPPED or READY). All joints in the sequence are marked
 * as part of the active sequence via `joint_in_sequence = 1`.
 *
 * @param seq Index of the sequence to start.
 *
 * @dot
 * digraph do_home_sequence {
 *   rankdir=LR;
 *   node [shape=ellipse, style=filled, fillcolor=lightgray];

 *   START [label="HOME_SEQUENCE_DO_ONE_SEQUENCE", fillcolor=lightblue];
 *   STOPPED -> HOMINGSTART [label="AxState=STOPPED"];
 *   READY   -> HOMINGSTART [label="AxState=READY"];
 *   UNKNOWN -> (no command) [style=dashed];

 *   STOPPED [fillcolor=lightyellow];
 *   READY   [fillcolor=lightyellow];
 *   HOMINGSTART [fillcolor=lightgreen];

 *   subgraph cluster_0 {
 *     label = "Joints in Sequence";
 *     style = dashed;
 *     color = gray;
 *     START -> STOPPED;
 *     START -> READY;
 *     START -> UNKNOWN;
 *   }
 * }
 * @enddot
 */
void do_home_sequence(int seq) {
    one_sequence_home_data_t addr = (sequence_home_data.shd[seq]);
    sequence_home_data.current_sequence = seq;

    int joints_in_sequence = addr.joints_in_sequence;
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_home_sequence sequence %d\n", __FILE__, __FUNCTION__, seq);
    current_sequence = seq;
    sequence_state = HOME_SEQUENCE_DO_ONE_SEQUENCE;

    for (int jj = 0; jj < joints_in_sequence; jj++) {
        int jno = addr.joint_ids[jj];
        pokeys_1joint_state_machine(jno);

        H[jno].homed = 0;
        H[jno].homing = 1;
        H[jno].home_state = HOME_START;
        H[jno].joint_in_sequence = 1;

        int int_AxesState = H[jno].PEv2_AxesState;
        switch (int_AxesState) {
            case PK_PEAxisState_axSTOPPED:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_home_sequence joint[%d] stopped\n", __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                break;

            case PK_PEAxisState_axREADY:
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_home_sequence joint[%d] ready\n", __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                break;

            default:
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_home_sequence(%d) joint[%d] unknown state %d\n", __FILE__, __FUNCTION__, seq, jno, int_AxesState);
                break;
        }
    }
    return;
}

/**
 * @brief Checks the homing state of a joint sequence and advances to the next if needed.
 *
 * This function evaluates the status flags (`homed`, `homing`, `home_state`) for the current sequence.
 * If the sequence is marked as completed (`homed`), and it is the last sequence, the homing process is finished.
 * Otherwise, it continues with the next sequence via `do_home_sequence()`.
 *
 * If the sequence is in homing progress (`homing`) and all joints report as homed, the state is updated accordingly.
 * It also resets the state if no joints report homing anymore.
 *
 * @param seq The index of the homing sequence to check.
 *
 * @dot
 * digraph check_home_sequence {
 *   rankdir=LR;
 *   node [shape=ellipse, style=filled, fillcolor=lightgray];

 *   IDLE        [label="HOME_SEQUENCE_IDLE", fillcolor=lightblue];
 *   DO_SEQ      [label="HOME_SEQUENCE_DO_ONE_SEQUENCE", fillcolor=lightblue];
 *   HOMING      [label="addr.homing", fillcolor=lightyellow];
 *   HOMED       [label="addr.homed", fillcolor=lightgreen];
 *   NEXT        [label="do_home_sequence(next)", fillcolor=lightgreen];
 *   FINISHED    [label="Sequence Finished", fillcolor=lightgreen];
 *   RESET       [label="addr.home_state = HOME_IDLE", fillcolor=lightgray];

 *   DO_SEQ -> HOMED [label="addr.homed == 1"];
 *   HOMED -> FINISHED [label="is_last == 1"];
 *   HOMED -> NEXT     [label="is_last == 0"];
 *   DO_SEQ -> HOMING [label="addr.homing == 1"];
 *   HOMING -> HOMED [label="get_sequence_homed(seq)"];
 *   HOMING -> HOMING [label="get_sequence_homing(seq)"];
 *   HOMING -> RESET [label="no joints homing"];
 * }
 * @enddot
 */
void check_home_sequence(int seq) {
    if (sequence_state != HOME_SEQUENCE_DO_ONE_SEQUENCE) {
        return;
    }

    one_sequence_home_data_t addr = (sequence_home_data.shd[seq]);
    int joints_in_sequence = addr.joints_in_sequence;

    if (addr.homed) {
        if (addr.is_last) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: check_home_sequence sequence %d homed - finished\n", __FILE__, __FUNCTION__, seq);
            sequence_state = HOME_SEQUENCE_IDLE;
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: check_home_sequence sequence %d homed - next sequ.\n", __FILE__, __FUNCTION__, seq);
            sequence_state = HOME_SEQUENCE_DO_ONE_SEQUENCE;
            do_home_sequence(addr.next_sequence);
        }
        return;
    } else if (addr.homing) {
        if (get_sequence_homed(seq)) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: check_home_sequence sequence %d homed\n", __FILE__, __FUNCTION__, seq);
            // addr.homed = 1;
            // addr.homing = 0;
            addr.home_state = HOME_FINISHED;
            if (addr.is_last) {
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: check_home_sequence sequence %d "
                                "sequence_state = HOME_SEQUENCE_IDLE\n",
                                __FILE__, __FUNCTION__, seq);
                sequence_state = HOME_SEQUENCE_IDLE;
            } else {
                sequence_state = HOME_SEQUENCE_DO_ONE_SEQUENCE;
                do_home_sequence(addr.next_sequence);
            }
        } else if (get_sequence_homing(seq)) {
            addr.homed = 0;
            addr.homing = 1;
            addr.home_state = HOME_START;
        } else {
            addr.homed = 0;
            addr.homing = 0;
            addr.home_state = HOME_IDLE;
        }
    }
}

/**
 * @brief Gibt an, ob alle Achsen erfolgreich ge-homed wurden.
 *
 * Diese Funktion gibt den globalen Zustand `allhomed` zurück,
 * der in der Homing-Logik aktualisiert wird, sobald alle Achsen
 * ihre Homing-Sequenz erfolgreich abgeschlossen haben.
 *
 * Hinweis: Der ursprünglich implementierte Prüfdurchlauf über alle
 * Achsen ist derzeit auskommentiert, die Funktion verlässt sich
 * auf den zentral verwalteten Zustand `allhomed`.
 *
 * @return `true` (1), wenn alle Achsen ge-homed sind, sonst `false` (0).
 *
 * @ingroup PoKeys_HomingStatus
 */
bool get_allhomed() {
    /* int ret = 1;
    for (int jno = 0; jno < EMCMOT_MAX_JOINTS; jno++)
    {

        one_joint_home_data_t *addr = &(joint_home_data->jhd[jno]);
        if (!*addr->homed)
        {
            ret = 0;
            break;
        }
        //usleep(10); // Add a small delay to prevent CPU hogging
    }
    if(ret==1){
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: get_allhomed homed %d\n", __FILE__, __FUNCTION__, ret);
    }*/
    return allhomed;
    // return ret ? 1 : 0;
}

/**
 * @brief Prüft, ob eine Achse erfolgreich ge-homed wurde.
 *
 * Gibt `true` zurück, wenn der Homing-Vorgang für die angegebene Achse
 * abgeschlossen wurde und die Achse als "ge-homed" markiert ist.
 * Optional wird eine Debug-Ausgabe generiert.
 *
 * @param jno Die Joint-Nummer (Achse), die geprüft werden soll.
 * @return `true` (1), wenn die Achse ge-homed ist, sonst `false` (0).
 *
 * @ingroup PoKeys_HomingStatus
 */
bool get_homed_memory[8];
bool get_homed(int jno) {
    if (H[jno].homed != get_homed_memory[jno]) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: get_homed(%d) homed %d\n", __FILE__, __FUNCTION__, jno, H[jno].homed);
        get_homed_memory[jno] = H[jno].homed;
    }
    return H[jno].homed ? 1 : 0;
}

/**
 * @brief Prüft, ob der Home-State einer Achse auf IDLE steht.
 *
 * Diese Funktion gibt zurück, ob sich die angegebene Achse aktuell
 * im Zustand `HOME_IDLE` befindet, also kein aktiver Homing-Vorgang läuft.
 *
 * @param jno Die Joint-Nummer (Achse), die geprüft werden soll.
 * @return `true` (1), wenn sich die Achse im Zustand `HOME_IDLE` befindet, sonst `false` (0).
 *
 * @ingroup PoKeys_HomingStatus
 */
bool get_home_is_idle(int jno) {
    return H[jno].home_state == HOME_IDLE ? 1 : 0;
}

/**
 * @brief Prüft, ob die Achse synchronisiert homed wurde.
 *
 * Diese Funktion gibt zurück, ob der Homing-Vorgang für die angegebene Achse
 * im Rahmen einer synchronisierten Sequenz erfolgte.
 *
 * @param jno Die Joint-Nummer (Achse), für die der Synchronisationsstatus
 * abgefragt werden soll.
 * @return `true` (1), wenn die Achse synchronisiert homed wurde, sonst `false` (0).
 *
 * @ingroup PoKeys_HomingStatus
 */
bool get_home_is_synchronized(int jno) {
    return H[jno].home_is_synchronized ? 1 : 0;
}

/**
 * @brief Prüft, ob die Achse vor dem Homing entsperrt werden muss.
 *
 * Diese Funktion überprüft, ob das `HOME_UNLOCK_FIRST`-Flag im
 * `home_flags`-Feld der angegebenen Achse gesetzt ist.
 * Falls ja, muss die Achse vor dem Start des Homing-Vorgangs entriegelt oder
 * bewegt werden (z. B. um einen Endschalter zu verlassen).
 *
 * @param jno Die Joint-Nummer (Achse), für die geprüft werden soll.
 * @return `true` (1), wenn die Achse zuerst "unlocked" werden muss, sonst `false` (0).
 *
 * @ingroup PoKeys_HomingFlags
 */
bool get_home_needs_unlock_first(int jno) {
    return (H[jno].home_flags & HOME_UNLOCK_FIRST) ? 1 : 0;
}

/**
 * @brief Gibt die Homing-Sequenznummer der angegebenen Achse zurück.
 *
 * Diese Funktion liefert den aktuellen Wert der `home_sequence`-Eigenschaft
 * der Achse mit der angegebenen Joint-Nummer. Die Homing-Sequenz definiert,
 * ob eine Achse einzeln oder gemeinsam mit anderen Achsen referenziert wird.
 *
 * @param jno Die Joint-Nummer (Achse), deren Homing-Sequenz abgefragt werden soll.
 * @return Die Homing-Sequenznummer dieser Achse (negativ = gruppiert, positiv = einzeln).
 *
 * @ingroup PoKeys_HomingStatusQuery
 */
int get_home_sequence_memory[8];
int get_home_sequence(int jno) {
    if (get_home_sequence_memory[jno] != H[jno].home_sequence) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: get_home_sequence(%d) home_sequence %d\n", __FILE__, __FUNCTION__, jno, H[jno].home_sequence);
        get_home_sequence_memory[jno] = H[jno].home_sequence;
    }
    return H[jno].home_sequence;
}

/**
 * @brief Gibt zurück, ob eine Achse aktuell im Homing-Vorgang ist.
 *
 * Diese Funktion liest das `homing`-Flag aus der `joint_home_data`
 * Struktur für die angegebene Achse und gibt zurück, ob Homing aktiv ist.
 *
 * @param jno Die Joint-Nummer (Achse), deren Homing-Status abgefragt werden soll.
 * @return true, wenn sich die Achse im Homing-Prozess befindet, sonst false.
 *
 * @ingroup PoKeys_HomingStatusQuery
 */
bool get_homing_memory[8];
bool get_homing(int jno) {
    one_joint_home_data_t *addr = &(joint_home_data->jhd[jno]);
    if (get_homing_memory[jno] != *addr->homing) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: get_homing joint[%d] homing %d\n", __FILE__, __FUNCTION__, jno, *addr->homing);
        get_homing_memory[jno] = *addr->homing;
    }
    return *addr->homing ? 1 : 0;
}

/**
 * @brief Prüft, ob der Index-Suchmodus beim Homing aktiv ist.
 *
 * Diese Funktion gibt zurück, ob für die angegebene Achse (`jno`)
 * das Flag `index_enable` gesetzt ist. Dies weist typischerweise
 * darauf hin, dass sich die Achse im Homing-Abschnitt zur
 * Index-Suche befindet.
 *
 * @param jno Die Joint-Nummer (Achse), deren Status abgefragt werden soll.
 * @return true, wenn `index_enable` gesetzt ist, sonst false.
 *
 * @ingroup PoKeys_HomingRuntime
 * @ingroup PoKeys_HomingStatusQuery
 */
bool get_homing_at_index_search_wait_memory[8];
bool get_homing_at_index_search_wait(int jno) {
    // return 0;

    if (get_homing_at_index_search_wait_memory[jno] != (HOME_INDEX_SEARCH_WAIT ? 1 : 0)) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: get_homing_at_index_search_wait joint[%d] index_enable %d\n", __FILE__, __FUNCTION__, jno, H[jno].index_enable);
        get_homing_at_index_search_wait_memory[jno] = HOME_INDEX_SEARCH_WAIT ? 1 : 0;
    }

    return H[jno].home_state == HOME_INDEX_SEARCH_WAIT ? 1 : 0;
    // return H[jno] ? index_search_active : 0;
}

/**
 * @brief Gibt zurück, ob der Homing-Vorgang aktuell aktiv ist.
 *
 * Diese Funktion liefert den Status des `homing_active`-Flags. Zusätzlich
 * gibt sie eine Debug-Nachricht aus, wenn sich der Status seit dem letzten
 * Aufruf geändert hat.
 *
 * @return true, wenn das System gerade eine Homing-Sequenz ausführt, sonst false.
 *
 * @ingroup PoKeys_HomingRuntime
 * @ingroup PoKeys_HomingStatusQuery
 */
bool get_homing_is_active() {
    if (homing_active != homing_active_old) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: get_homing_is_active ==  %d\n", __FILE__, __FUNCTION__, homing_active);
        homing_active_old = homing_active;
    }
    return homing_active;
}

/**
 * @brief Gibt zurück, ob der Index-Enable-Status für eine Achse gesetzt ist.
 *
 * Diese Funktion prüft, ob das Indexsignal für die angegebene Achse (Joint)
 * aktuell aktiviert ist. Dies kann im Homing-Prozess verwendet werden, um zu
 * erkennen, ob die Achse auf ein Index-Event wartet oder dieses verwendet.
 *
 * @param jno Joint-Nummer der Achse.
 * @return true, wenn Index aktiviert ist, sonst false.
 *
 * @ingroup PoKeys_HomingRuntime
 */
bool get_index_enable_memory[8];
bool get_index_enable(int jno) {
    if (get_index_enable_memory[jno] != H[jno].index_enable) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: get_index_enable joint[%d] index_enable %d\n", __FILE__, __FUNCTION__, jno, H[jno].index_enable);
        get_index_enable_memory[jno] = H[jno].index_enable;
    }
    return H[jno].index_enable ? 1 : 0;
}

/**
 * @brief Liest aktuelle Homing-relevante Zustände von den HAL-Input-Pins ein.
 *
 * Diese Funktion aktualisiert die internen Homing-Datenstrukturen (`H[jno]`) mit Werten,
 * die über die HAL-Eingabepins zur Verfügung gestellt werden. Typischerweise geschieht dies
 * einmal pro Servo-Zyklus.
 *
 * Sie liest folgende Werte ein:
 * - `home_sw` (Home-Schalter-Zustand)
 * - `index_enable` (Aktivierung des Indexsignals)
 * - `PEv2_AxesState` (aktueller Achsstatus der PulseEngine v2)
 *
 * Es wird ein Logeintrag erzeugt, wenn sich der Achsstatus im Vergleich zum letzten
 * bekannten Zustand verändert hat.
 *
 * @param njoints Anzahl der konfigurierten Achsen (Joints).
 *
 * @ingroup PoKeys_HomingRuntime
 */
void read_homing_in_pins(int njoints) {
    int jno;
    int org_state;
    int org_index_enable;
    int org_home_sw;
    one_joint_home_data_t *addr;
    for (jno = 0; jno < njoints; jno++) {
        addr = &(joint_home_data->jhd[jno]);

        org_home_sw = H[jno].home_sw;
        H[jno].home_sw = *(addr->home_sw); // IN
        if (H[jno].home_sw != org_home_sw) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: read_homing_in_pins joint[%d] home_sw changed :%d\n", __FILE__, __FUNCTION__, jno, H[jno].home_sw);
        }

        org_state = H[jno].PEv2_AxesState;
        H[jno].PEv2_AxesState = *(addr->PEv2_AxesState); // IN
        if (H[jno].PEv2_AxesState != org_state) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: read_homing_in_pins joint[%d] state changed :%d\n", __FILE__, __FUNCTION__, jno, H[jno].PEv2_AxesState);
        }

        org_index_enable = H[jno].index_enable;
        H[jno].index_enable = *(addr->index_enable); // IN
        if (H[jno].index_enable != org_index_enable) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: read_homing_in_pins joint[%d] index_enable changed :%d\n", __FILE__, __FUNCTION__, jno, *(addr->index_enable));
        }
    }
    return;
}

/**
 * @brief Schreibt aktuelle Homing-Zustände in die verbundenen HAL-Output-Pins.
 *
 * Diese Funktion überträgt den internen Zustand (`H[jno]`) jeder Achse auf die zugehörigen
 * HAL-Ausgabepins, die in `joint_home_data->jhd[jno]` gespeichert sind.
 * Sie wird typischerweise einmal pro Servo-Zyklus aufgerufen.
 *
 * Zusätzlich wird ein Logeintrag geschrieben, wenn sich der `PEv2_AxesCommand` für eine Achse ändert.
 *
 * @param njoints Anzahl der konfigurierten Achsen (Joints).
 *
 * @ingroup PoKeys_HomingRuntime
 */
void write_homing_out_pins(int njoints) {
    int jno;
    one_joint_home_data_t *addr;
    int org_cmd;
    int org_index_enable;
    int org_homed;
    int org_homing;
    int org_home_state;
    for (jno = 0; jno < njoints; jno++) {
        addr = &(joint_home_data->jhd[jno]);

        org_homing = *(addr->homing);
        *(addr->homing) = H[jno].homing; // OUT
        if (org_homing != H[jno].homing) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: write_homing_out_pins joint[%d] homing changed :%d\n", __FILE__, __FUNCTION__, jno, H[jno].homing);
        }

        org_homed = *(addr->homed);
        *(addr->homed) = H[jno].homed; // OUT
        if (org_homed != H[jno].homed) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: write_homing_out_pins joint[%d] homed changed :%d\n", __FILE__, __FUNCTION__, jno, H[jno].homed);
        }

        org_home_state = *(addr->home_state);
        *(addr->home_state) = H[jno].home_state; // OUT
        if (org_home_state != H[jno].home_state) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: write_homing_out_pins joint[%d] home_state changed :%d\n", __FILE__, __FUNCTION__, jno, H[jno].home_state);
        }

        org_index_enable = *(addr->index_enable);
        *(addr->index_enable) = H[jno].index_enable; // OUT
        if (org_index_enable != H[jno].index_enable) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: write_homing_out_pins joint[%d] index_enable changed :%d\n", __FILE__, __FUNCTION__, jno, H[jno].index_enable);
        }

        org_cmd = *(addr->PEv2_AxesCommand);
        *(addr->PEv2_AxesCommand) = H[jno].PEv2_AxesCommand; // OUT
        if (org_cmd != H[jno].PEv2_AxesCommand) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: write_homing_out_pins joint[%d] command changed :%d\n", __FILE__, __FUNCTION__, jno, H[jno].PEv2_AxesCommand);
        }
    }
    //  rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: write_homing_out_pins\n");
    return;
}

void do_home_all(void) {
    if (!get_homing_is_active()) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_home_all - set sequence_state = HOME_SEQUENCE_START\n", __FILE__, __FUNCTION__);
        sequence_state = HOME_SEQUENCE_START;
    }
    return;
} // do_home_all()

/**
 * @brief Initiates and manages the homing process for a given joint or sequence.
 *
 * This function determines the current `AxesState` of the specified joint and issues appropriate commands
 * (e.g. `HOMINGSTART`, `IDLE`, `HOMINGFINALIZE`) to the PoKeys Pulse Engine. If a sequence of joints is
 * being homed together (`home_sequence < 0`), it checks if all involved joints are ready or finished before
 * advancing the sequence.
 *
 * If `jno` is negative, it attempts to start the homing sequence at index 0.
 *
 * @param jno Joint number to home; if negative, the first sequence is used.
 *
 * @dot
 * digraph do_home_joint {
 *   rankdir=LR;
 *   node [shape=box, style=filled, fillcolor=lightgray];

 *   STOPPED     [label="axSTOPPED\nto HOMINGSTART", fillcolor=lightyellow];
 *   READY       [label="axREADY\nto HOMINGSTART", fillcolor=lightyellow];
 *   SEARCH      [label="axHOMINGSEARCH\n-> IDLE", fillcolor=orange];
 *   START       [label="axHOMINGSTART\n-> IDLE", fillcolor=orange];
 *   BACK        [label="axHOMINGBACK\n-> IDLE", fillcolor=orange];
 *   BACKING_OFF [label="axHOMING_BACKING_OFF\n-> IDLE", fillcolor=orange];
 *   RESETTING   [label="axHOMING_RESETTING\n-> IDLE", fillcolor=orange];
 *   HOME        [label="axHOME\n-> HOMINGFINALIZE if all homed", fillcolor=lightgreen];
 *   DEFAULT     [label="default / unknown", fillcolor=gray];

 *   SEQ_START   [label="home_sequence < 0\nsequence_state = DO_ONE_SEQUENCE", shape=ellipse, fillcolor=lightblue];
 *   SEQ_SINGLE  [label="home_sequence >= 0\nsequence_state = DO_ONE_JOINT", shape=ellipse, fillcolor=lightblue];

 *   STOPPED     -> SEQ_START   [label="if part of sequence"];
 *   READY       -> SEQ_START   [label="if part of sequence"];
 *   STOPPED     -> SEQ_SINGLE  [label="otherwise"];
 *   READY       -> SEQ_SINGLE  [label="otherwise"];
 *   HOME        -> SEQ_START   [label="if all homed"];
 * }
 * @enddot
 */
void do_home_joint(int jno) {
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing start\n", __FILE__, __FUNCTION__, jno);
    if (jno >= 0) {
        // one_joint_home_data_t *addr = &(joint_home_data->jhd[jno]);
        pokeys_1joint_state_machine(jno);
        int int_AxesState = H[jno].PEv2_AxesState;
        switch (int_AxesState) {
            case PK_PEAxisState_axHOMINGSTART:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing start - "
                                "PK_PEAxisCommand_axIDLE\n",
                                __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                break;

            case PK_PEAxisState_axHOMINGSEARCH:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing search "
                                "- PK_PEAxisCommand_axIDLE\n",
                                __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                break;
            case PK_PEAxisState_axHOMINGBACK:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing back - "
                                "PK_PEAxisCommand_axIDLE\n",
                                __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                break;
            case PK_PEAxisState_axHOMING_RESETTING:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing "
                                "resetting PK_PEAxisCommand_axIDLE\n",
                                __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                break;

            case PK_PEAxisState_axHOMING_BACKING_OFF:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing state:PK_PEAxisState_axHOMING_BACKING_OFF "
                                "command:PK_PEAxisCommand_axARMENCODER\n",
                                __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axARMENCODER;
                break;

            case PEAxisStateEx_HOMINGARMENCODER:
                if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGWAITFINALMOVE) {
                    rtapi_print_msg(RTAPI_MSG_DBG,
                                    "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing state:PEAxisStateEx_HOMINGARMENCODER "
                                    "command:PK_PEAxisCommand_axHOMINGWAITFINALMOVE\n",
                                    __FILE__, __FUNCTION__, jno);
                }

                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGWAITFINALMOVE;
                break;

            case PEAxisStateEx_HOMINGWaitFINALMOVE:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing finalize "
                                "PK_PEAxisCommand_axIDLE\n",
                                __FILE__, __FUNCTION__, jno);
                if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGFINALMOVE) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] homing finalize "
                                    "PK_PEAxisCommand_axHOMINGFINALMOVE\n",
                                    __FILE__, __FUNCTION__, jno);
                    H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALMOVE;
                }

                break;
            case PEAxisStateEx_HOMINGFINALMOVE:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] PEAxisStateEx_HOMINGFINALMOVE "
                                "PK_PEAxisCommand_axIDLE\n",
                                __FILE__, __FUNCTION__, jno);
                if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axIDLE) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] PEAxisStateEx_HOMINGFINALMOVE "
                                    "PK_PEAxisCommand_axIDLE\n",
                                    __FILE__, __FUNCTION__, jno);
                    H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                }

                break;
            case PK_PEAxisState_axHOME:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint.PK_PEAxisState_axHOME do_home_joint "
                                "joint[%d] homed (home_sequence %d)\n",
                                __FILE__, __FUNCTION__, jno, H[jno].home_sequence);

                // check if all joints in sequence are homed
                if (H[jno].home_sequence < 0) {

                    int joints_in_sequence = 0;
                    int homed_in_sequence = 0;
                    int jj;
                    for (jj = 0; jj < all_joints; jj++) {
                        if (abs(H[jj].home_sequence) == abs(H[jno].home_sequence)) {
                            joints_in_sequence++;
                            if (H[jj].PEv2_AxesState == PK_PEAxisState_axHOME) {
                                homed_in_sequence++;
                            }
                        }
                    }

                    rtapi_print_msg(RTAPI_MSG_DBG,
                                    "PoKeys_homecomp: %s:%s: PK_PEAxisState_axHOME joint[%d] "
                                    "joints_in_sequence:%d  homed_in_sequence:%d \n",
                                    __FILE__, __FUNCTION__, jno, joints_in_sequence, homed_in_sequence);
                    if (joints_in_sequence == homed_in_sequence) {
                        // if all Joints of the Sequence show Hommed
                        for (jj = 0; jj < all_joints; jj++) {

                            if (abs(H[jj].home_sequence) == abs(H[jno].home_sequence)) {
                                rtapi_print_msg(RTAPI_MSG_DBG,
                                                "PoKeys_homecomp: %s:%s: do_home_joint.PK_"
                                                "PEAxisState_axHOME joint[%d] "
                                                "homed (home_sequence %d) PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE\n",
                                                __FILE__, __FUNCTION__, jj, H[jno].home_sequence);
                                if (H[jj].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGFINALIZE) {
                                    rtapi_print_msg(RTAPI_MSG_ERR,
                                                    "PoKeys_homecomp: %s:%s: do_home_joint.PK_"
                                                    "PEAxisState_axHOME joint[%d] "
                                                    "homed (home_sequence %d) PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE\n",
                                                    __FILE__, __FUNCTION__, jj, H[jno].home_sequence);
                                    H[jj].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE;
                                }
                            }
                        }
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_home_joint.PK_PEAxisState_axHOME joint "
                                        "%d homed (home_sequence %d) PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE\n",
                                        __FILE__, __FUNCTION__, jno, H[jno].home_sequence);
                        if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGFINALIZE) {
                            H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_home_joint.PK_PEAxisState_axHOME joint "
                                            "%d homed (home_sequence %d) PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE\n",
                                            __FILE__, __FUNCTION__, jno, H[jno].home_sequence);
                        }
                    }
                } else {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: do_home_joint.PK_PEAxisState_axHOME joint[%d] "
                                    "homed (home_sequence %d)\n",
                                    __FILE__, __FUNCTION__, jno, H[jno].home_sequence);
                    H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE;
                }
                break;
            case PK_PEAxisState_axSTOPPED:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] stopped\n", __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                H[jno].home_state = HOME_START;
                if (H[jno].home_sequence < 0) {
                    sequence_state = HOME_SEQUENCE_DO_ONE_SEQUENCE;
                    int jj;
                    for (jj = 0; jj < all_joints; jj++) {
                        if (abs(H[jj].home_sequence) == abs(H[jno].home_sequence)) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys_homecomp: %s:%s: joint[%d] in sequence %d "
                                            "PK_PEAxisCommand_axHOMINGSTART\n",
                                            __FILE__, __FUNCTION__, jj, H[jno].home_sequence);
                            H[jj].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                            H[jj].home_state = HOME_START;
                        }
                    }
                } else {
                    sequence_state = HOME_SEQUENCE_DO_ONE_JOINT;
                }

                break;
            case PK_PEAxisState_axREADY:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] ready "
                                "PK_PEAxisCommand_axHOMINGSTART\n",
                                __FILE__, __FUNCTION__, jno);
                H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                H[jno].home_state = HOME_START;
                if (H[jno].home_sequence < 0) {
                    sequence_state = HOME_SEQUENCE_DO_ONE_SEQUENCE;
                    int jj;
                    for (jj = 0; jj < all_joints; jj++) {
                        if (abs(H[jj].home_sequence) == abs(H[jno].home_sequence)) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys_homecomp: %s:%s: joint[%d] in sequence %d "
                                            "PK_PEAxisCommand_axHOMINGSTART\n",
                                            __FILE__, __FUNCTION__, jj, H[jj].home_sequence);
                            H[jj].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                            H[jj].home_state = HOME_START;
                        }
                    }
                } else {
                    sequence_state = HOME_SEQUENCE_DO_ONE_JOINT;
                }

                break;

            default:
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_home_joint joint[%d] unknown state %d\n", __FILE__, __FUNCTION__, jno, int_AxesState);
                break;
        }
    } else {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_home_joint joint number %d out of range\n", __FILE__, __FUNCTION__, jno);

        // triger homing for first sequence (index 0)

        // do_home_sequence(0);
        update_sequence_home_data();
        sequence_state = HOME_SEQUENCE_DO_ONE_SEQUENCE;
        int jj;
        current_sequence = 0;
        int trigered = 0;
        for (jj = 0; jj < all_joints; jj++) {
            if (abs(H[jj].home_sequence) == abs(sequence_home_data.min_sequence)) {
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: joint[%d] in sequence %d "
                                "PK_PEAxisCommand_axHOMINGSTART\n",
                                __FILE__, __FUNCTION__, jj, sequence_home_data.min_sequence);
                H[jj].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                H[jno].home_state = HOME_START;
                trigered++;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: joint[%d] not in sequence %d\n", __FILE__, __FUNCTION__, jj, sequence_home_data.min_sequence);
            }
        }

        if (trigered == 0) {
            rtapi_print_msg(RTAPI_MSG_WARN, "PoKeys_homecomp: %s:%s: no joints in sequence %d\n", __FILE__, __FUNCTION__, sequence_home_data.shd[current_sequence].home_sequence);
        }
    }
    return;
}

/**
 * @brief Executes the homing sequence state machine for multiple CNC joints.
 *
 * This function handles joint homing by advancing the global `sequence_state` and controlling individual joints via `PEv2_AxesCommand`.
 *
 * ## Detailed Behavior:
 * Depending on the current `sequence_state`, this function:
 * - Selects joints involved in the current homing sequence.
 * - Updates joint status flags (`H[*].home_state`, `H[*].homed`, `H[*].homing`).
 * - Issues homing commands (`PEv2_AxesCommand`) based on the feedback from `PEv2_AxesState`.
 * - Manages index-enable logic (`H[*].index_enable`) and sequence transitions.
 *
 * ## Relevant State Variables:
 * - **sequence_state**: Controls global sequence flow.
 * - **H[*].home_state**: Individual joint homing state (`HOME_START`, etc.).
 * - **H[*].homed**: Indicates joint has completed homing.
 * - **H[*].homing**: Indicates joint homing is active.
 * - **H[*].PEv2_AxesState**: Current PoKeys Pulse Engine axis state.
 * - **H[*].PEv2_AxesCommand**: Commands sent to the PoKeys Pulse Engine.
 * - **H[*].index_enable**: Controls position indexing during homing.
 *
 * ## State Machine (`sequence_state`) Overview:
 *
 * @dot
 * digraph homing_sequence_state_machine {
 *   rankdir=LR;
 *   node [shape=ellipse];
 *
 *   IDLE [label="IDLE"];
 *   DO_ONE_JOINT [label="DO_ONE_JOINT"];
 *   DO_ONE_SEQUENCE [label="DO_ONE_SEQUENCE"];
 *   START [label="START"];
 *   START_JOINTS [label="START_JOINTS"];
 *   WAIT_JOINTS [label="WAIT_JOINTS"];
 *
 *   IDLE -> DO_ONE_JOINT [label="H[*].home_state=START"];
 *   DO_ONE_JOINT -> START [label="joints selected"];
 *   DO_ONE_SEQUENCE -> START [label="multiple joints selected"];
 *   START -> WAIT_JOINTS [label="homing initiated"];
 *   WAIT_JOINTS -> IDLE [label="all joints homed,\nlast sequence"];
 *   WAIT_JOINTS -> DO_ONE_SEQUENCE [label="next sequence"];
 *   WAIT_JOINTS -> START_JOINTS [label="state reset needed"];
 *   START_JOINTS -> WAIT_JOINTS [label="re-init joints"];
 * }
 * @enddot
 *
 * ## PlantUML Interaction Diagram:
 *
 * @startuml
 * title Homing Sequence Interaction
 *
 * state IDLE
 * state DO_ONE_JOINT
 * state DO_ONE_SEQUENCE
 * state START
 * state START_JOINTS
 * state WAIT_JOINTS
 *
 * [*] --> IDLE
 * IDLE --> DO_ONE_JOINT : H[*].home_state = START
 * DO_ONE_JOINT --> START : joints_in_sequence > 0
 * DO_ONE_SEQUENCE --> START : prepare multi-joint homing
 * START --> WAIT_JOINTS : issue HOMINGSTART
 * WAIT_JOINTS --> IDLE : all joints homed\n(last sequence)
 * WAIT_JOINTS --> DO_ONE_SEQUENCE : proceed next sequence
 * WAIT_JOINTS --> START_JOINTS : AxesState = READY/RUNNING
 * START_JOINTS --> WAIT_JOINTS : reset joint states
 * @enduml
 *
 * ## State Variables & Commands:
 * | Variable                  | Description                                 |
 * |---------------------------|---------------------------------------------|
 * | H[*].home_state           | Individual joint homing state               |
 * | H[*].homed                | Set to 1 when joint homing is completed     |
 * | H[*].homing               | Set to 1 during active homing               |
 * | H[*].PEv2_AxesState       | Axis state reported from PoKeys Pulse Engine|
 * | H[*].PEv2_AxesCommand     | Axis command sent to PoKeys Pulse Engine    |
 * | H[*].index_enable         | Controls encoder indexing during homing     |
 *
 * ## Axis State Transitions (`PEv2_AxesState` → `PEv2_AxesCommand`):
 * | PEv2_AxesState                           | Resulting Command (`PEv2_AxesCommand`) |
 * |------------------------------------------|----------------------------------------|
 * | axSTOPPED, axREADY, axRUNNING            | axHOMINGSTART                         |
 * | axERROR                                  | axHOMINGCANCEL                        |
 * | axHOME (final state)                     | axIDLE                                |
 * | axReadyToFinalizeHoming                  | axHOMINGFINALIZE                      |
 * | axReadyToArmEncoder                      | axARMENCODER                          |
 * | axHOMINGARMENCODER                       | axHOMINGWAITFINALMOVE                 |
 * | axHOMINGWaitFINALMOVE                    | axHOMINGFINALMOVE                     |
 * | axHOMINGFINALMOVE                        | axIDLE                                |
 *
 * @note All transitions are logged via `rtapi_print_msg` for detailed debugging.
 */
int dhs_homed_count_memory = 0;
int dhs_joints_in_sequence_memory = 0;
int homed_memory[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static void do_homing_sequence(void) {
    int i, ii;
    int seen;
    emcmot_joint_t *joint;
    int sequence_is_set = 0;
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: >do_homing_sequence(%d) sequence_state = %d\n", __FILE__, __FUNCTION__, current_sequence, sequence_state);
    if (current_sequence < sequence_home_data.min_sequence || current_sequence > sequence_home_data.max_sequence) {
        // no sequence set, use the first one
        current_sequence = sequence_home_data.min_sequence;
    }
    one_sequence_home_data_t addr = (sequence_home_data.shd[current_sequence]);
    int joints_in_sequence = addr.joints_in_sequence;
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joints_in_sequence = %d\n", __FILE__, __FUNCTION__, current_sequence, joints_in_sequence);
    int joints_in_sequence_DBG = 0;
    one_sequence_home_data_t addr_dbg;
    /** @startuml */
    switch (sequence_state) {

        /** state HOME_SEQUENCE_IDLE {*/
        case HOME_SEQUENCE_IDLE:
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing_sequence HOME_SEQUENCE_IDLE\n", __FILE__, __FUNCTION__);
            current_sequence = sequence_home_data.min_sequence;
            /* nothing to do */
            break;
            /** } */

        case HOME_SEQUENCE_DO_ONE_JOINT: /** state HOME_SEQUENCE_WAIT_JOINTS {*/
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing_sequence HOME_SEQUENCE_DO_ONE_JOINT\n", __FILE__, __FUNCTION__);
            update_sequence_home_data();
            // Expect one joint with home_state==HOME_START

            for (i = 0; i < all_joints; i++) {
                // pokeys_1joint_state_machine(i);
                if (H[i].home_state == HOME_START) {
                    H[i].joint_in_sequence = 1; // in sequence
                    H[i].homed = 0;
                    H[i].homing = 1;
                    joints_in_sequence_DBG++;
                    addr_dbg.joints_in_sequence = joints_in_sequence_DBG;
                    addr_dbg.joint_ids[joints_in_sequence_DBG] = i;
                    current_sequence = abs(H[i].home_sequence);
                    addr = (sequence_home_data.shd[current_sequence]);
                    joints_in_sequence = addr.joints_in_sequence; // count of all joints in sequence
                } else {
                    H[i].joint_in_sequence = 0; // not in sequence
                }
            }
            sequence_is_set = 1;

            /** state choice_joints_in_sequence_1 <<choice>> */
            if (joints_in_sequence == 0) {
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_DO_ONE_JOINT "
                                "joints_in_sequence = 0\n",
                                __FILE__, __FUNCTION__, current_sequence);
                update_sequence_home_data();
                /** choice_joints_in_sequence_1 -> HOME_SEQUENCE_IDLE : joints_in_sequence == 0*/
                sequence_state = HOME_SEQUENCE_IDLE;
                addr = (sequence_home_data.shd[current_sequence]);
                joints_in_sequence = addr.joints_in_sequence; // count of all joints in sequence
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) after update "
                                "HOME_SEQUENCE_DO_ONE_JOINT joints_in_sequence = %d \n",
                                __FILE__, __FUNCTION__, current_sequence, joints_in_sequence);

                break;
            } else {
                /** choice_joints_in_sequence_1 -> HOME_SEQUENCE_IDLE : joints_in_sequence != 0*/
                sequence_state = HOME_SEQUENCE_START;
            }

            break; /** } */

        /** state HOME_SEQUENCE_DO_ONE_SEQUENCE*/
        case HOME_SEQUENCE_DO_ONE_SEQUENCE:
            // Expect multiple joints with home_state==HOME_START
            // specified by a negative sequence
            // Determine current_sequence and set H[i].joint_in_sequence
            // based on home_state[i] == HOME_START
            rtapi_print_msg(RTAPI_MSG_DBG,
                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_DO_ONE_SEQUENCE "
                            "joints_in_sequence = %d\n",
                            __FILE__, __FUNCTION__, current_sequence, joints_in_sequence);

            for (int jj = 0; jj < joints_in_sequence; jj++) {
                int jno = addr.joint_ids[jj];

                // H[jno].homed = 0; -> to be set in pokeys_1joint_state_machine
                // H[jno].homing = 1; -> to be set in pokeys_1joint_state_machine
                H[jno].joint_in_sequence = 1;
                H[jno].home_state = HOME_START;
                pokeys_1joint_state_machine(jno);
            }

            sequence_state = HOME_SEQUENCE_START;
            break;
            // drop through----drop through----drop through----drop through

        case HOME_SEQUENCE_START:
            // Request to home all joints or a single sequence
            sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
            allhomed = 0;
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_START\n", __FILE__, __FUNCTION__, current_sequence);
            for (int jj = 0; jj < joints_in_sequence; jj++) {
                int jno = addr.joint_ids[jj];
                H[jno].home_state = HOME_START;
                pokeys_1joint_state_machine(jno);
                /*int int_AxesState = H[jno].PEv2_AxesState;
                switch (int_AxesState) {
                    case PK_PEAxisState_axSTOPPED:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint[%d] stopped "
                                        "PK_PEAxisCommand_axHOMINGSTART\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                        break;

                    case PK_PEAxisState_axREADY:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint[%d] ready "
                                        "PK_PEAxisCommand_axHOMINGSTART\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                        break;

                    case PK_PEAxisState_axRUNNING:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint[%d] running "
                                        "PK_PEAxisCommand_axHOMINGSTART\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                        break;

                    case PK_PEAxisState_axERROR:
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint[%d] error "
                                        "PK_PEAxisCommand_axHOMINGCANCEL\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_IDLE;
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGCANCEL;
                        return;
                        break;

                    default:
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint[%d] unknown state %d\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno, int_AxesState);
                        break;
                }*/
            }

            /* tell the world we're on the job */
            homing_active = 1;
            break;
            // drop through----drop through----drop through----drop through

        case HOME_SEQUENCE_START_JOINTS:
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_START_JOINTS\n", __FILE__, __FUNCTION__, current_sequence);
            seen = 0;
            allhomed = 0;
            /* start all joints whose sequence number matches H[i].home_sequence */
            for (i = 0; i < all_joints; i++) {
                joint = &joints[i];
                if (abs(H[i].home_sequence) == current_sequence) {
                    if (!H[i].joint_in_sequence)
                        continue;
                    /* start this joint */
                    joint->free_tp.enable = 0;
                    H[i].home_state = HOME_START;
                    seen++;
                }
            }
            if (seen || current_sequence == 0) {
                sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_START_JOINTS "
                                "no joints in sequence %d\n",
                                __FILE__, __FUNCTION__, current_sequence, current_sequence);
                /* no joints have this sequence number, we're done */
                sequence_state = HOME_SEQUENCE_IDLE;
                /* tell the world */
                homing_active = 0;
            }
            break;

        case HOME_SEQUENCE_WAIT_JOINTS:
            // rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_WAIT_JOINTS joints_in_sequence: %d \n", __FILE__, __FUNCTION__,current_sequence,joints_in_sequence);
            seen = 0;

            int homed_count = 0;

            for (int jj = 0; jj < joints_in_sequence; jj++) {
                int jno = addr.joint_ids[jj];
                // Handle per-joint state machine
                pokeys_1joint_state_machine(jno);

                // Let each joint update its `homed` status
                if (H[jno].homed) {
                    homed_count++;
                }
                if (homed_memory[jno] != H[jno].homed) {
                    rtapi_print_msg(RTAPI_MSG_DBG,
                                    "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                    "joint[%d] homed status changed to %d\n",
                                    __FILE__, __FUNCTION__, current_sequence, jno, H[jno].homed);
                    homed_memory[jno] = H[jno].homed;
                }
            }

            if (dhs_homed_count_memory != homed_count || dhs_joints_in_sequence_memory != joints_in_sequence) {
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_WAIT_JOINTS "
                                "homed_count:%d joints_in_sequence:%d \n",
                                __FILE__, __FUNCTION__, current_sequence, homed_count, joints_in_sequence);
                dhs_homed_count_memory = homed_count;
                dhs_joints_in_sequence_memory = joints_in_sequence;
            }

            if (homed_count == joints_in_sequence) {
                if (addr.is_last) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                    "HOME_SEQUENCE_WAIT_JOINTS last finished\n",
                                    __FILE__, __FUNCTION__, current_sequence);
                    // is set  in bool do_homing(void) based on pokeys_1joint_state_machine(joint_num)
                    // sequence_state = HOME_SEQUENCE_IDLE;
                    // homing_active = 0; is set  in bool do_homing(void)
                    allhomed = 1;
                } else {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                    "HOME_SEQUENCE_WAIT_JOINTS finished "
                                    "continue with sequence(%d) \n",
                                    __FILE__, __FUNCTION__, current_sequence, addr.next_sequence);
                    sequence_state = HOME_SEQUENCE_DO_ONE_SEQUENCE;
                    current_sequence = addr.next_sequence;
                }
            }
            break;

        default:
            /* should never get here */
            rtapi_print_msg(RTAPI_MSG_DBG,
                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) unknown state '%d' "
                            "during homing sequence\n",
                            __FILE__, __FUNCTION__, current_sequence, sequence_state);
            sequence_state = HOME_SEQUENCE_IDLE;
            homing_active = 0;
            break;
    }
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: <do_homing_sequence(%d) sequence_state %d\n", __FILE__, __FUNCTION__, current_sequence, sequence_state);
    return;
} // do_homing_sequence()

/*
 */
/**
 * @brief Main homing handler to be called periodically during motion.
 *
 * This function coordinates the full homing process by:
 * - Executing the current homing sequence state machine via `do_homing_sequence()`
 * - Calling `pokeys_1joint_state_machine()` for each active joint marked as part of the sequence
 * - Monitoring homing progress via the `homing_flag`
 * - Detecting when the homing process is complete by comparing the `get_allhomed()` state
 *
 * The function returns `true` once all joints are homed and the `homing_flag` is zero.
 * This typically triggers the final switch to operational mode (e.g. teleop).
 *
 * @returns `true` if homing has just completed in this cycle; otherwise `false`.
 *
 * @dot
 * digraph do_homing_state_flow {
 *   rankdir=LR;
 *   node [shape=box, style=filled, fillcolor=lightgray];

 *   start        [label="do_homing()", fillcolor=lightblue];
 *   do_seq       [label="do_homing_sequence()", fillcolor=lightyellow];
 *   loop_joints  [label="loop active joints\n(pokeys_1joint_state_machine)", fillcolor=lightgreen];
 *   check_flags  [label="check: beginning_allhomed==0 && end_allhomed==1 && homing_flag==0", fillcolor=khaki];
 *   return_true  [label="return true\n(homing completed)", shape=ellipse, fillcolor=palegreen];
 *   return_false [label="return false\n(homing not finished)", shape=ellipse, fillcolor=lightcoral];

 *   start        -> do_seq       [label="if sequence_state != IDLE"];
 *   do_seq       -> loop_joints;
 *   start        -> loop_joints  [label="else"];
 *   loop_joints  -> check_flags;
 *   check_flags  -> return_true  [label="if allhomed transition + flag==0"];
 *   check_flags  -> return_false [label="otherwise"];
 * }
 * @enddot
 */
bool beginning_allhomed_memory = 0;
bool end_allhomed_memory = 0;
bool homing_active_memory = 0;
int homing_flag_memory = 0;
bool do_homing_retval_memory = 0;
bool do_homing(void) {
    int joint_num;
    int homing_flag = 0;
    int active_joints = 0;
    bool beginning_allhomed = get_allhomed();
    bool retval = false;

    if (sequence_state != HOME_SEQUENCE_IDLE) {
        // check_home_sequence(current_sequence);
        rtapi_print_msg(RTAPI_MSG_DBG,
                        "PoKeys_homecomp: %s:%s: do_homing sequence_state %d  -  trigger "
                        "do_homing_sequence()\n",
                        __FILE__, __FUNCTION__, sequence_state);
        do_homing_sequence();
    }

    /* loop thru joints, treat each one individually */
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing loop thru joints, treat each one individually\n", __FILE__, __FUNCTION__);
    for (joint_num = 0; joint_num < all_joints; joint_num++) {
        if (!H[joint_num].joint_in_sequence) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing joint[%d] not in sequence\n", __FILE__, __FUNCTION__, joint_num);
            continue;
        }
        if (!GET_JOINT_ACTIVE_FLAG(&joints[joint_num])) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing joint[%d] not active\n", __FILE__, __FUNCTION__, joint_num);
            continue;
        }
        active_joints++;
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing joint[%d] - pokeys_1joint_state_machine\n", __FILE__, __FUNCTION__, joint_num);
        homing_flag += pokeys_1joint_state_machine(joint_num);
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing joint: %d homing_flag: %d \n", __FILE__, __FUNCTION__, joint_num, homing_flag);
    }
    // return 1 if homing completed this period

    bool end_allhomed = get_allhomed();
    //    sequence_state = HOME_SEQUENCE_IDLE;

    if (beginning_allhomed_memory != beginning_allhomed || end_allhomed_memory == 1 && end_allhomed == 0 || homing_active_memory != homing_active || homing_flag_memory != homing_flag) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                        "PoKeys_homecomp: %s:%s: beginning_allhomed: %d / end_allhomed: %d  "
                        "homing_active: %d homing_flag:%d\n",
                        __FILE__, __FUNCTION__, beginning_allhomed, end_allhomed, homing_active, homing_flag);
        beginning_allhomed_memory = beginning_allhomed;
        end_allhomed_memory = end_allhomed;
        homing_active_memory = homing_active;
        homing_flag_memory = homing_flag;
    }
    if (beginning_allhomed == 0 && end_allhomed == 1 && homing_flag == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_homing homing completed\n", __FILE__, __FUNCTION__);
        homing_active = 0;
        sequence_state = HOME_SEQUENCE_IDLE;

        retval = true;
    } else {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing homing_flag %d\n", __FILE__, __FUNCTION__, homing_flag);
    }
    if (do_homing_retval_memory != retval) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_homing retval %d\n", __FILE__, __FUNCTION__, retval);
        do_homing_retval_memory = retval;
    }
    return retval;
}

/**
 * @brief Setzt den Homing-Status einer Achse zurück.
 *
 * Diese Funktion markiert die angegebene Achse (`jno`) als "nicht gehome'd"
 * durch Setzen von `H[jno].homed = 0`. Sie wird typischerweise aufgerufen,
 * wenn sich der Maschinenzustand ändert oder wenn Homing erneut erforderlich ist.
 *
 * @param jno Die Joint-Nummer (Achse), deren Homing-Status zurückgesetzt werden soll.
 * @param motstate Der aktuelle Maschinenstatus (wird derzeit nicht verwendet).
 *
 * @ingroup PoKeys_HomingControl
 */
void set_unhomed(int jno, motion_state_t motstate) {
    // one_joint_home_data_t *addr = &(joint_home_data->jhd[jno]);
    H[jno].homed = 0;
    return;
}

/**
 * @brief Bricht den Homing-Vorgang für eine einzelne Achse ab.
 *
 * Diese Funktion sendet den Befehl `PK_PEAxisCommand_axHOMINGCANCEL` an die
 * spezifizierte Achse (`jno`) über das gemeinsame Steuerarray `H[]`.
 *
 * Wird typischerweise verwendet, wenn ein Fehler auftritt oder der Homing
 * manuell abgebrochen werden soll.
 *
 * @param jno Die Joint-Nummer (Achse), für die das Homing abgebrochen werden soll
 *
 * @ingroup PoKeys_HomingControl
 * @ingroup PoKeys_PEv2_AxisCommand
 */
void do_cancel_homing(int jno) {
    // one_joint_home_data_t *addr = &(joint_home_data->jhd[jno]);
    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_cancel_homing(%d)\n", __FILE__, __FUNCTION__, jno);
    H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGCANCEL;
    H[jno].home_state = HOME_ABORT;
    H[jno].homing = 0;
    // H[jno].homed = 0;
    H[jno].joint_in_sequence = 0;
    return;
}

/**
 * @brief Setzt die Homing-Parameter für eine bestimmte Achse.
 *
 * Diese Funktion initialisiert oder aktualisiert alle relevanten Homing-Parameter
 * für die angegebene Achse `jno`. Dazu zählen Geschwindigkeiten für verschiedene
 * Phasen des Homings, der Offset, die Zielposition, die Flags zur Steuerung des
 * Homing-Verhaltens sowie die Sequenzzugehörigkeit.
 *
 * Zusätzlich wird die gesamte Sequenzstruktur mit
 * `update_sequence_home_data()` aktualisiert.
 *
 * @param jno Joint-Nummer der zu konfigurierenden Achse.
 * @param offset Der Offset, der nach dem Homing angewendet wird.
 * @param home Zielposition nach dem Homing (z. B. 0.0).
 * @param home_final_vel Geschwindigkeit beim finalen Anfahren der Home-Position.
 * @param home_search_vel Geschwindigkeit beim ersten Suchen des Referenzpunktes.
 * @param home_latch_vel Geschwindigkeit beim Latch-Vorgang (z. B. bei Indexpulse).
 * @param home_flags Steuerflags (z. B. Verhalten bei Limit-Schaltern).
 * @param home_sequence Sequenznummer (negativ = Gruppensequenz, positiv = Einzelsequenz).
 * @param volatile_home Gibt an, ob das Homing flüchtig ist (z. B. bei Soft-Homing).
 *
 * @ingroup PoKeys_HomingSetup
 */
void set_joint_homing_params(int jno, double offset, double home, double home_final_vel, double home_search_vel, double home_latch_vel, int home_flags, int home_sequence, bool volatile_home) {
    rtapi_print_msg(RTAPI_MSG_DBG,
                    "PoKeys_homecomp: %s:%s: set_joint_homing_params(%d) offset:%f home:%f "
                    "home_final_vel:%f home_search_vel:%f home_latch_vel:%f "
                    "home_flags:%d home_sequence:%d volatile_home:%d\n",
                    __FILE__, __FUNCTION__, jno, offset, home, home_final_vel, home_search_vel, home_latch_vel, home_flags, home_sequence, volatile_home);
    H[jno].offset = offset;
    H[jno].home = home;
    H[jno].home_final_vel = home_final_vel;
    H[jno].home_search_vel = home_search_vel;
    H[jno].home_latch_vel = home_latch_vel;
    H[jno].home_flags = home_flags;
    H[jno].home_sequence = home_sequence;
    H[jno].volatile_home = volatile_home;

    update_sequence_home_data();
    return;
}

/**
 * @brief Aktualisiert ausgewählte Homing-Parameter einer Achse.
 *
 * Diese Funktion erlaubt es, einzelne wesentliche Homing-Parameter
 * (Offset, Zielposition und Sequenznummer) zur Laufzeit anzupassen,
 * ohne die restlichen Homing-Parameter zu verändern.
 * Anschließend wird die Sequenzstruktur mit `update_sequence_home_data()` aktualisiert.
 *
 * @param jno Joint-Nummer der zu aktualisierenden Achse.
 * @param offset Neuer Offset nach Homing.
 * @param home Neue Zielposition nach Homing.
 * @param home_sequence Zugehörige Homing-Sequenz (negativ = Gruppe, positiv = Einzelsequenz).
 *
 * @ingroup PoKeys_HomingSetup
 */
void update_joint_homing_params(int jno, double offset, double home, int home_sequence) {
    rtapi_print_msg(RTAPI_MSG_DBG,
                    "PoKeys_homecomp: %s:%s: update_joint_homing_params(%d) offset:%f home:%f "
                    "home_sequence:%d\n",
                    __FILE__, __FUNCTION__, jno, offset, home, home_sequence);
    H[jno].offset = offset;
    H[jno].home = home;
    H[jno].home_sequence = home_sequence;

    update_sequence_home_data();

    return;
}

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
EXPORT_SYMBOL(do_home_all);
EXPORT_SYMBOL(do_cancel_homing);
EXPORT_SYMBOL(set_unhomed);
EXPORT_SYMBOL(set_joint_homing_params);
EXPORT_SYMBOL(update_joint_homing_params);
EXPORT_SYMBOL(write_homing_out_pins);
/** @} */ // end of defgroup pokeys_homecomp
#undef XSTR
#undef STR
#undef HOMING_BASE
#undef USE_HOMING_BASE
#undef CUSTOM_HOMEMODULE
static int __comp_get_data_size(void) {
    return 0;
}
