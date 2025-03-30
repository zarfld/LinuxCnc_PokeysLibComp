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
    PK_PEAxisCommand_axHOMINGWAITFINALMOVE = 3, // move to homeposition
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

bool Homing_ArmEncodereDone[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int pokeys_1joint_state_machine(int joint_num) {
    emcmot_joint_t *joint;
    double offset, tmp;
    int home_sw_active, homing_flag;
    bool immediate_state = 0;

    homing_flag = 0;
    joint = &joints[joint_num];
    home_sw_active = H[joint_num].home_sw;
    /*if (H[joint_num].home_state != HOME_IDLE)
    {
        homing_flag = 1; // at least one joint is homing
    }*/

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

        switch (H[joint_num].PEv2_AxesState) {
            case PK_PEAxisState_axSTOPPED:
                /* Axis is stopped */
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d stopped\n", __FILE__, __FUNCTION__, joint_num);
                if (H[joint_num].homing) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d stopped - set homing=0\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].homing = 0;
                }

                if (H[joint_num].homed) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d stopped - set homed=0\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].homed = 0;
                }

                H[joint_num].home_state = HOME_IDLE;
                break;

            case PK_PEAxisState_axREADY:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY\n", __FILE__, __FUNCTION__, joint_num);
                /* Axis ready */
                if (H[joint_num].home_state == HOME_FINAL_MOVE_WAIT) {
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
                    H[joint_num].home_state = HOME_FINISHED;
                } else if (H[joint_num].home_state == HOME_START) {
                    if (H[joint_num].PEv2_AxesCommand != PK_PEAxisCommand_axIDLE) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY/HOME_START - set PEv2_AxesCommand=0\n", __FILE__, __FUNCTION__, joint_num);
                        H[joint_num].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                    }

                } else {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d PK_PEAxisState_axREADY - unexpected state !!!\n", __FILE__, __FUNCTION__, joint_num);
                }

                break;

            case PK_PEAxisState_axRUNNING:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d running\n", __FILE__, __FUNCTION__, joint_num);
                /* Axis is running */

                if (H[joint_num].homing) {
                    /* Axis is homing */
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d running - set homing=0\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].homing = 0;
                }
                if (H[joint_num].home_state != HOME_IDLE) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d running - set homing=0\n", __FILE__, __FUNCTION__, joint_num);
                    H[joint_num].home_state = HOME_IDLE;
                }
                break;

            case PK_PEAxisState_axHOMING_RESETTING:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "homing resetting\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Stopping the axis to reset the position counters */
                H[joint_num].homing = 1;
                H[joint_num].home_state = HOME_UNLOCK;
                H[joint_num].homed = 0;
                homing_flag = 1;
                break;

            case PK_PEAxisState_axHOMING_BACKING_OFF:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "homing backing off\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Backing off switch */
                H[joint_num].homing = 1;
                H[joint_num].home_state = HOME_INITIAL_BACKOFF_WAIT;
                Homing_ArmEncodereDone[joint_num] = 0;
                homing_flag = 1;
                break;

            case PEAxisStateEx_axReadyToFinalizeHoming:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "ready to finalize homing\n",
                                __FILE__, __FUNCTION__, joint_num);
                if (H[joint_num].home_state != HOME_INDEX_SEARCH_START) {
                    H[joint_num].homing = 1;
                    // H[joint_num].index_enable = index_search_active;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                    "ready to finalize homing - index_enable %d\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].index_enable);
                    H[joint_num].home_state = HOME_INDEX_SEARCH_START;
                }
                break;
            case PEAxisStateEx_axReadyToArmEncoder:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "ready to arm encoder\n",
                                __FILE__, __FUNCTION__, joint_num);
                /** This state is called after the machine has made a low
                   speed pass to determine the limit switch location. It
                   sets index-enable, which tells the encoder driver to
                   reset its counter to zero and clear the enable when the
                   next index pulse arrives. */
                /** set the index enable */
                if (H[joint_num].index_enable != index_search_active) {
                    H[joint_num].index_enable = index_search_active;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                    "ready to arm encoder - index_search_active %d\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].index_enable);
                }
                H[joint_num].homing = 1;
                H[joint_num].home_state = HOME_INDEX_SEARCH_WAIT;

                break;
            case PEAxisStateEx_HOMINGARMENCODER:
                /** This state is called after the machine has found the
                       home switch and "armed" the encoder counter to reset on
                       the next index pulse. It continues at low speed until
                       an index pulse is detected, at which point it sets the
                       final home position.  If the move ends or hits a limit
                       before an index pulse occurs, the home is aborted. */
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "homing arm encoder\n",
                                __FILE__, __FUNCTION__, joint_num);
                /**  has an index pulse arrived yet? encoder driver clears
                   enable when it does */

                if (H[joint_num].index_enable == index_search_armed && H[joint_num].home_state != HOME_SET_INDEX_POSITION) {
                    /* yes, stop motion */
                    joint->free_tp.enable = 0;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                    "homing arm encoder - index pulse arrived joint->free_tp.enable %d\n",
                                    __FILE__, __FUNCTION__, joint_num, joint->free_tp.enable);
                    // joint->free_tp.pos_cmd = joint->free_tp.curr_pos; // set to zero
                    joint->free_tp.pos_cmd = 0.0; // set to zero
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                    "homing arm encoder - index pulse arrived joint->free_tp.pos_cmd %f\n",
                                    __FILE__, __FUNCTION__, joint_num, joint->free_tp.pos_cmd);
                    /* Pokeys resets encoder position to zeros */
                    if (H[joint_num].index_enable != index_search_armed) {
                        H[joint_num].index_enable = index_search_armed;
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                        "homing arm encoder - index pulse arrived H[joint_num].index_enable %d\n",
                                        __FILE__, __FUNCTION__, joint_num, H[joint_num].index_enable);
                    }
                    H[joint_num].homing = 1;
                    H[joint_num].home_state = HOME_SET_INDEX_POSITION;
                    immediate_state = 1;
                }

                break;

            case PEAxisStateEx_HOMINGWaitFINALMOVE:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "homing wait final move\n",
                                __FILE__, __FUNCTION__, joint_num);

                if (H[joint_num].home_state == HOME_SET_INDEX_POSITION) {
                    /* This state is called when the encoder has been reset at
                    the index pulse position.  It sets the current joint
                    position to 'home_offset', which is the location of the
                    index pulse in joint coordinates. */
                }

                /* waiting for sync before Pokeys moves to homeposition */
                H[joint_num].homing = 1;

                if (H[joint_num].index_enable != index_search_armed) {

                    H[joint_num].index_enable = index_search_armed;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                    "PEAxisStateEx_HOMINGWaitFINALMOVE - index pulse arrived H[joint_num].index_enable %d\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].index_enable);
                }

                joints_in_sequence = 0;
                ready_in_sequence = 0;

                for (int jj = 0; jj < all_joints; jj++) {
                    if (abs(H[jj].home_sequence) == abs(H[joint_num].home_sequence)) {
                        joints_in_sequence++;
                        if (H[jj].PEv2_AxesState == PEAxisStateEx_HOMINGWaitFINALMOVE) {
                            ready_in_sequence++;
                        }
                    }
                }

                if (joints_in_sequence == ready_in_sequence) {
                    // if all Joints of the Sequence show Hommed
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: PEAxisStateEx_HOMINGWaitFINALMOVE joint %d "
                                    "joints_in_sequence:%d  ready_in_sequence:%d \n",
                                    __FILE__, __FUNCTION__, joint_num, joints_in_sequence, ready_in_sequence);
                    for (int jj = 0; jj < all_joints; jj++) {

                        if (abs(H[jj].home_sequence) == abs(H[joint_num].home_sequence)) {
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_home_joint.PK_PEAxisState_axHOME "
                                            "joint %d homed (home_sequence %d)\n",
                                            __FILE__, __FUNCTION__, jj, H[joint_num].home_sequence);
                            H[jj].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALMOVE;
                            H[jj].home_state = HOME_FINAL_MOVE_START;
                            // H[jj].homing = 0;
                            H[jj].homed = 0;
                        }
                    }
                    rtapi_print_msg(RTAPI_MSG_DBG,
                                    "PoKeys_homecomp: %s:%s: do_home_joint.PEAxisStateEx_HOMINGWaitFINALMOVE joint %d "
                                    "homed (home_sequence %d)\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].home_sequence);
                    // H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE;
                    H[joint_num].home_state = HOME_FINAL_MOVE_START;
                    // H[joint_num].homing = 0;
                    H[joint_num].homed = 0;
                }

                break;
            case PEAxisStateEx_HOMINGFINALMOVE:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "PEAxisStateEx_HOMINGFINALMOVE\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Pokeys moves to homeposition */
                if (!H[joint_num].homing) {
                    H[joint_num].homing = 1;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                    "PEAxisStateEx_HOMINGFINALMOVE - set homing=1\n",
                                    __FILE__, __FUNCTION__, joint_num);
                }
                if (H[joint_num].home_state != HOME_FINAL_MOVE_WAIT) {
                    H[joint_num].home_state = HOME_FINAL_MOVE_WAIT;
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                    "PEAxisStateEx_HOMINGFINALMOVE - set home_state=HOME_FINAL_MOVE_WAIT\n",
                                    __FILE__, __FUNCTION__, joint_num);
                }

                // H[joint_num].index_enable = 1;
                break;

            case PK_PEAxisState_axHOME:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d homed\n", __FILE__, __FUNCTION__, joint_num);
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

                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: PK_PEAxisState_axHOME joint %d "
                                "joints_in_sequence:%d  homed_in_sequence:%d \n",
                                __FILE__, __FUNCTION__, joint_num, joints_in_sequence, homed_in_sequence);
                if (joints_in_sequence == homed_in_sequence) {
                    // if all Joints of the Sequence show Hommed
                    for (int jj = 0; jj < all_joints; jj++) {

                        if (abs(H[jj].home_sequence) == abs(H[joint_num].home_sequence)) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys_homecomp: %s:%s: do_home_joint.PK_PEAxisState_axHOME "
                                            "joint %d homed (home_sequence %d)\n",
                                            __FILE__, __FUNCTION__, jj, H[joint_num].home_sequence);
                            // H[jj].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE;
                            H[jj].home_state = HOME_FINISHED;
                            // H[jj].homing = 0;
                            // H[jj].homed = 1;
                        }
                    }
                    rtapi_print_msg(RTAPI_MSG_ERR,
                                    "PoKeys_homecomp: %s:%s: do_home_joint.PK_PEAxisState_axHOME joint %d "
                                    "homed (home_sequence %d)\n",
                                    __FILE__, __FUNCTION__, joint_num, H[joint_num].home_sequence);
                    H[joint_num].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                    H[joint_num].home_state = HOME_FINISHED;
                    // H[joint_num].homing = 0;
                    // H[joint_num].homed = 1;
                }

                break;

            case PK_PEAxisState_axHOMINGSTART:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "homing start\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Homing procedure is starting on axis */
                H[joint_num].homing = 1;
                H[joint_num].home_state = HOME_START;
                homing_flag = 1;
                break;

            case PK_PEAxisState_axHOMINGSEARCH:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "homing search\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Homing procedure first step - going to home */
                H[joint_num].homing = 1;
                H[joint_num].home_state = HOME_INITIAL_SEARCH_WAIT;
                homing_flag = 1;
                break;

            case PK_PEAxisState_axHOMINGBACK:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "homing back\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Homing procedure second step - slow homing */
                H[joint_num].homing = 1;
                H[joint_num].home_state = HOME_FINAL_BACKOFF_START;
                homing_flag = 1;
                break;

            case PK_PEAxisState_axPROBED:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d probed\n", __FILE__, __FUNCTION__, joint_num);
                /* Probing completed for this axis */
                H[joint_num].homing = 0;
                H[joint_num].home_state = HOME_IDLE;

                break;

            case PK_PEAxisState_axPROBESTART:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "probed start\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Probing procedure is starting on axis */
                H[joint_num].homing = 0;
                H[joint_num].home_state = HOME_IDLE;
                break;

            case PK_PEAxisState_axPROBESEARCH:
                rtapi_print_msg(RTAPI_MSG_DBG,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d "
                                "probe search\n",
                                __FILE__, __FUNCTION__, joint_num);
                /* Probing procedure - probing */
                H[joint_num].homing = 0;
                H[joint_num].home_state = HOME_IDLE;
                break;

            case PK_PEAxisState_axERROR:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d error\n", __FILE__, __FUNCTION__, joint_num);
                /* Axis error */
                H[joint_num].homing = 0;
                H[joint_num].home_state = HOME_IDLE;
                break;

            case PK_PEAxisState_axLIMIT:
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine joint %d limit\n", __FILE__, __FUNCTION__, joint_num);
                /* Axis limit tripped */
                H[joint_num].homing = 0;
                H[joint_num].home_state = HOME_IDLE;
                break;

            default:
                /* should never get here */
                rtapi_print_msg(RTAPI_MSG_ERR,
                                "PoKeys_homecomp: %s:%s: pokeys_1joint_state_machine unknown "
                                "state '%d' during homing j=%d",
                                __FILE__, __FUNCTION__, H[joint_num].home_state, joint_num);
                H[joint_num].home_state = HOME_ABORT;
                // immediate_state = 1;
                break;
        } /* end of switch(H[joint_num].home_state) */
    } while (immediate_state);

    return homing_flag;
} // pokeys_1joint_state_machine()

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
                pokeys_1joint_state_machine(jno);
                H[jno].homed = 0;
                H[jno].homing = 1;
                H[jno].joint_in_sequence = 1;
                H[jno].home_state = HOME_START;
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
                pokeys_1joint_state_machine(jno);
                int int_AxesState = H[jno].PEv2_AxesState;
                switch (int_AxesState) {
                    case PK_PEAxisState_axSTOPPED:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint %d stopped "
                                        "PK_PEAxisCommand_axHOMINGSTART\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                        break;

                    case PK_PEAxisState_axREADY:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint %d ready "
                                        "PK_PEAxisCommand_axHOMINGSTART\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                        break;

                    case PK_PEAxisState_axRUNNING:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint %d running "
                                        "PK_PEAxisCommand_axHOMINGSTART\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGSTART;
                        break;

                    case PK_PEAxisState_axERROR:
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint %d error "
                                        "PK_PEAxisCommand_axHOMINGCANCEL\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_IDLE;
                        H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGCANCEL;
                        return;
                        break;

                    default:
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_START joint %d unknown state %d\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno, int_AxesState);
                        break;
                }
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
                // pokeys_1joint_state_machine(jno);
                int int_AxesState = H[jno].PEv2_AxesState;
                // rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_WAIT_JOINTS joint %d AxesState %d\n", __FILE__, __FUNCTION__,current_sequence, jno, int_AxesState);
                switch (int_AxesState) {
                    case PK_PEAxisState_axSTOPPED:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axSTOPPED\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);

                        if (sequence_state != HOME_SEQUENCE_START_JOINTS && sequence_state != HOME_SEQUENCE_WAIT_JOINTS) {

                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                            "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                            "PK_PEAxisState_axSTOPPED -> set sequence_state:HOME_SEQUENCE_START_JOINTS\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                            sequence_state = HOME_SEQUENCE_START_JOINTS; // back to start
                        }
                        break;

                    case PK_PEAxisState_axREADY:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axREADY\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        if (H[jj].home_state == HOME_FINAL_MOVE_WAIT) {

                        } else if (H[jj].home_state == HOME_FINISHED) {
                            if (H[jj].homed) {
                                homed_count++;
                            }

                        } else if (H[jj].home_state == HOME_INDEX_SEARCH_WAIT || H[jj].home_state == HOME_SET_INDEX_POSITION) {

                        } else if (sequence_state != HOME_SEQUENCE_START_JOINTS && sequence_state != HOME_SEQUENCE_WAIT_JOINTS) {

                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                            "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                            "PK_PEAxisState_axREADY -> set sequence_state:HOME_SEQUENCE_START_JOINTS %d\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno, HOME_SEQUENCE_START_JOINTS);
                            sequence_state = HOME_SEQUENCE_START_JOINTS; // back to start
                        }
                        break;

                    case PK_PEAxisState_axRUNNING:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axRUNNING\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        if (sequence_state != HOME_SEQUENCE_START_JOINTS && sequence_state != HOME_SEQUENCE_WAIT_JOINTS) {

                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                            "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                            "PK_PEAxisState_axRUNNING -> set sequence_state:HOME_SEQUENCE_START_JOINTS %d\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno, HOME_SEQUENCE_START_JOINTS);
                            sequence_state = HOME_SEQUENCE_START_JOINTS; // back to start
                        }

                        break;

                    case PK_PEAxisState_axHOMINGSTART:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axHOMINGSTART\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        break;

                    case PK_PEAxisState_axHOMING_RESETTING:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axHOMING_RESETTING\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        break;

                    case PK_PEAxisState_axHOMING_BACKING_OFF:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axHOMING_BACKING_OFF\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        break;

                    case PK_PEAxisState_axHOME:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axHOME\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        if (H[jno].homed == 1) {
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                            "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                            "PK_PEAxisState_axHOME flags set ok\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                            //     homed_count++;
                        }
                        homed_count++;
                        break;

                    case PK_PEAxisState_axHOMINGSEARCH:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "PK_PEAxisState_axHOMINGSEARCH\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        break;

                    case PK_PEAxisState_axHOMINGBACK:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                        "%d HOME_SEQUENCE_WAIT_JOINTS "
                                        "PK_PEAxisState_axHOMINGBACK\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        break;

                    case PK_PEAxisState_axERROR:
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                        "%d HOME_SEQUENCE_WAIT_JOINTS "
                                        "PK_PEAxisState_axERROR\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_IDLE;
                        if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGCANCEL) {
                            H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGCANCEL;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                            "%d HOME_SEQUENCE_WAIT_JOINTS "
                                            "PK_PEAxisState_axERROR -> PK_PEAxisCommand_axHOMINGCANCEL\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                        }

                        break;
                    case PEAxisStateEx_axReadyToFinalizeHoming:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                        "%d HOME_SEQUENCE_WAIT_JOINTS "
                                        "PEAxisStateEx_axReadyToFinalizeHoming\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGFINALIZE) {
                            H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALIZE;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                            "%d HOME_SEQUENCE_WAIT_JOINTS "
                                            "PEAxisStateEx_axReadyToFinalizeHoming -> PK_PEAxisCommand_axHOMINGFINALIZE\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                        }

                        break;
                    case PEAxisStateEx_axReadyToArmEncoder:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                        "%d HOME_SEQUENCE_WAIT_JOINTS "
                                        "PEAxisStateEx_axReadyToArmEncoder\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axARMENCODER) {
                            H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axARMENCODER;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                            "%d HOME_SEQUENCE_WAIT_JOINTS "
                                            "PEAxisStateEx_axReadyToArmEncoder -> PK_PEAxisCommand_axARMENCODER\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                        }

                        break;
                    case PEAxisStateEx_HOMINGARMENCODER:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                        "%d HOME_SEQUENCE_WAIT_JOINTS "
                                        "PEAxisStateEx_HOMINGARMENCODER\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGWAITFINALMOVE) {
                            H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGWAITFINALMOVE;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                            "%d HOME_SEQUENCE_WAIT_JOINTS -> "
                                            "PEAxisStateEx_HOMINGARMENCODER\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                        }

                        break;
                    case PEAxisStateEx_HOMINGWaitFINALMOVE:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                        "%d HOME_SEQUENCE_WAIT_JOINTS "
                                        "PEAxisStateEx_HOMINGWaitFINALMOVE\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axHOMINGFINALMOVE) {
                            H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axHOMINGFINALMOVE;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                            "%d HOME_SEQUENCE_WAIT_JOINTS "
                                            "PEAxisStateEx_HOMINGWaitFINALMOVE -> PK_PEAxisCommand_axHOMINGFINALMOVE\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                        }

                        break;
                    case PEAxisStateEx_HOMINGFINALMOVE:
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                        "%d HOME_SEQUENCE_WAIT_JOINTS "
                                        "PEAxisStateEx_HOMINGFINALMOVE\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno);
                        sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
                        if (H[jno].PEv2_AxesCommand != PK_PEAxisCommand_axIDLE) {
                            H[jno].PEv2_AxesCommand = PK_PEAxisCommand_axIDLE;
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) joint "
                                            "%d HOME_SEQUENCE_WAIT_JOINTS "
                                            "PEAxisStateEx_HOMINGFINALMOVE -> PK_PEAxisCommand_axIDLE\n",
                                            __FILE__, __FUNCTION__, current_sequence, jno);
                        }

                        break;
                    default:
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) "
                                        "HOME_SEQUENCE_WAIT_JOINTS joint %d "
                                        "unknown state '%d'\n",
                                        __FILE__, __FUNCTION__, current_sequence, jno, int_AxesState);
                        break;
                }
            }
            rtapi_print_msg(RTAPI_MSG_ERR,
                            "PoKeys_homecomp: %s:%s: do_homing_sequence(%d) HOME_SEQUENCE_WAIT_JOINTS "
                            "homed_count:%d joints_in_sequence:%d \n",
                            __FILE__, __FUNCTION__, current_sequence, homed_count, joints_in_sequence);
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

bool do_homing(void) {
    int joint_num;
    int homing_flag = 0;
    int active_joints = 0;
    bool beginning_allhomed = get_allhomed();

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
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing joint %d not in sequence\n", __FILE__, __FUNCTION__, joint_num);
            continue;
        }
        if (!GET_JOINT_ACTIVE_FLAG(&joints[joint_num])) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing joint %d not active\n", __FILE__, __FUNCTION__, joint_num);
            continue;
        }
        active_joints++;
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_homing joint %d - pokeys_1joint_state_machine\n", __FILE__, __FUNCTION__, joint_num);
        homing_flag += pokeys_1joint_state_machine(joint_num);
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_homing joint: %d homing_flag: %d \n", __FILE__, __FUNCTION__, joint_num, homing_flag);
    }
    // return 1 if homing completed this period

    bool end_allhomed = get_allhomed();
    if (beginning_allhomed == 0 && end_allhomed == 1 && homing_flag == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys_homecomp: %s:%s: do_homing homing completed\n", __FILE__, __FUNCTION__);
        homing_active = 0;
        sequence_state = HOME_SEQUENCE_IDLE;
        return true;
    } else {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys_homecomp: %s:%s: do_homing homing_flag %d\n", __FILE__, __FUNCTION__, homing_flag);
    }
    rtapi_print_msg(RTAPI_MSG_DBG,
                    "PoKeys_homecomp: %s:%s: beginning_allhomed: %d / end_allhomed: %d  "
                    "homing_active: %d \n",
                    __FILE__, __FUNCTION__, beginning_allhomed, end_allhomed, homing_active);
    return false;
}