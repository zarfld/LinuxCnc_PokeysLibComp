/**
 * @file PoKeysCompPulseEngine_v2.c
 * @brief HAL component implementation for PoKeys Pulse Engine v2 (PEv2) in LinuxCNC.
 *
 * This file provides the userspace implementation of the HAL integration for the PoKeys Pulse Engine v2.
 * It defines the behavior and synchronization logic for motion control, homing, limit switch handling,
 * and real-time parameter exchange between LinuxCNC and PoKeys57E devices.
 *
 * Key functionalities include:
 * - Setting up and updating HAL pins and parameters for each axis.
 * - Managing homing states and synchronized homing sequences.
 * - Reading and writing axis state, command, and configuration data.
 * - Handling emergency stop and probe inputs.
 * - Coordinating PEv2 setup and real-time synchronization via the PoKeysLib.
 *
 * The file interacts directly with:
 * - `PoKeysLib` for hardware communication
 * - `PokeysCompPulsEngine_base.c` for HAL pin creation
 * - `pokeys_homecomp.c` for advanced homing state machines
 *
 * This component runs in userspace and is designed to be modular and extendable for future PEv2 features.
 *
 * @author zarfld
 * @date First added: 2024
 * @copyright MIT License
 *
 * @see PoKeysLib.h
 * @see pokeys_homecomp.c
 * @see PokeysCompPulsEngine_base.c
 * @see pokeys.comp
 */
/**
 * @defgroup pulse_engine_v2 Pulse Engine v2 Configuration
 * @brief Functions related to configuring and controlling PulseEngine v2.
 *
 * Handles PEv2 setup, axis configuration, state updates, probing, and synchronized homing.
 * Relies on the base module (`pulse_engine_base`) for low-level HAL integration.
 */

#include <stdlib.h>
#include "PoKeysComp.h"
#include "PokeysCompPulsEngine_base.c"

PEv2_data_t *PEv2_data = NULL;
extern all_IO_data_t *IO_data;

#undef PEv2_deb_out
#define PEv2_deb_out (*PEv2_data->PEv2_deb_out)

#undef PEv2_deb_axxisout
#define PEv2_deb_axxisout(i) (*(PEv2_data->PEv2_deb_axxisout[i]))
#undef PEv2_deb_ishoming
#define PEv2_deb_ishoming(i) (*(PEv2_data->PEv2_deb_ishoming[i]))
#undef PEv2_deb_inposition
#define PEv2_deb_inposition(i) (*(PEv2_data->PEv2_deb_inposition[i]))
#undef PEv2_deb_PosMode
#define PEv2_deb_PosMode(i) (*(PEv2_data->PEv2_deb_PosMode[i]))
#undef PEv2_deb_PosModeAct
#define PEv2_deb_PosModeAct(i) (*(PEv2_data->PEv2_deb_PosModeAct[i]))
#undef PEv2_deb_velmode_count
#define PEv2_deb_velmode_count(i) (*(PEv2_data->PEv2_deb_velmode_count[i]))
#undef PEv2_deb_posmode_count
#define PEv2_deb_posmode_count(i) (*(PEv2_data->PEv2_deb_posmode_count[i]))
#undef PEv2_deb_doMove
#define PEv2_deb_doMove(i) (*(PEv2_data->PEv2_deb_doMove[i]))
#undef PEv2_deb_RefSpeed
#define PEv2_deb_RefSpeed(i) (*(PEv2_data->PEv2_deb_RefSpeed[i]))
#undef PEv2_deb_RefPos
#define PEv2_deb_RefPos(i) (*(PEv2_data->PEv2_deb_RefPos[i]))
#undef PEv2_deb_RefPosSpeed
#define PEv2_deb_RefPosSpeed(i) (*(PEv2_data->PEv2_deb_RefPosSpeed[i]))

#undef PEv2_params_ApplyIniSettings
#define PEv2_params_ApplyIniSettings (*PEv2_data->PEv2_params_ApplyIniSettings)
#undef PEv2_AxesState
#define PEv2_AxesState(i) (*(PEv2_data->PEv2_AxesState[i]))
#undef PEv2_AxesCommand
#define PEv2_AxesCommand(i) (0 + *(PEv2_data->PEv2_AxesCommand[i]))

#undef PEv2_ProbePosition
#define PEv2_ProbePosition(i) (*(PEv2_data->PEv2_ProbePosition[i]))
#undef PEv2_ProbeMaxPosition
#define PEv2_ProbeMaxPosition(i) (*(PEv2_data->PEv2_ProbeMaxPosition[i]))
#undef PEv2_CurrentPosition
#define PEv2_CurrentPosition(i) (*(PEv2_data->PEv2_CurrentPosition[i]))
#undef PEv2_PositionSetup
#define PEv2_PositionSetup(i) (*(PEv2_data->PEv2_PositionSetup[i]))
#undef PEv2_ReferencePositionSpeed
#define PEv2_ReferencePositionSpeed(i) (*(PEv2_data->PEv2_ReferencePositionSpeed[i]))

#undef PEv2_joint_vel_cmd
#define PEv2_joint_vel_cmd(i) (0 + *(PEv2_data->PEv2_joint_vel_cmd[i]))
#undef PEv2_joint_pos_cmd
#define PEv2_joint_pos_cmd(i) (0 + *(PEv2_data->PEv2_joint_pos_cmd[i]))
#undef PEv2_joint_pos_fb
#define PEv2_joint_pos_fb(i) (*(PEv2_data->PEv2_joint_pos_fb[i]))
#undef PEv2_joint_out_home
#define PEv2_joint_out_home(i) (0 + *(PEv2_data->PEv2_joint_out_home[i]))
#undef PEv2_joint_in_position
#define PEv2_joint_in_position(i) (0 + *(PEv2_data->PEv2_joint_in_position[i]))
#undef PEv2_joint_kb_jog_active
#define PEv2_joint_kb_jog_active(i) (0 + *(PEv2_data->PEv2_joint_kb_jog_active[i]))
#undef PEv2_joint_wheel_jog_active
#define PEv2_joint_wheel_jog_active(i) (0 + *(PEv2_data->PEv2_joint_wheel_jog_active[i]))
#undef PEv2_stepgen_TYPE
#define PEv2_stepgen_TYPE(i) (0 + *(PEv2_data->PEv2_stepgen_TYPE[i]))
#undef PEv2_stepgen_HOME
#define PEv2_stepgen_HOME(i) (0 + (PEv2_data->PEv2_stepgen_HOME[i]))

#undef PEv2_params_Feedback_Encoder_Id
#define PEv2_params_Feedback_Encoder_Id(i) (0 + *(PEv2_data->PEv2_params_Feedback_Encoder_Id[i]))
#undef PEv2_stepgen_DEADBAND
#define PEv2_stepgen_DEADBAND(i) (0 + *(PEv2_data->PEv2_stepgen_DEADBAND[i]))
#undef PEv2_stepgen_MAX_OUTPUT
#define PEv2_stepgen_MAX_OUTPUT(i) (0 + *(PEv2_data->PEv2_stepgen_MAX_OUTPUT[i]))
#undef PEv2_stepgen_ENCODER_SCALE
#define PEv2_stepgen_ENCODER_SCALE(i) (0 + *(PEv2_data->PEv2_stepgen_ENCODER_SCALE[i]))

#undef PEv2_digin_LimitN_DedicatedInput
#define PEv2_digin_LimitN_DedicatedInput(i) (*(PEv2_data->PEv2_digin_LimitN_DedicatedInput[i]))

#undef PEv2_digin_LimitP_in_not
#define PEv2_digin_LimitP_in_not(i) (*(PEv2_data->PEv2_digin_LimitP_in_not[i]))

#undef PEv2_digout_AxisEnable_out
#define PEv2_digout_AxisEnable_out(i) (0 + *(PEv2_data->PEv2_digout_AxisEnable_out[i]))

#undef PEv2_digin_Error_in
#define PEv2_digin_Error_in(i) (*(PEv2_data->PEv2_digin_Error_in[i]))
#undef PEv2_digin_Error_in_not
#define PEv2_digin_Error_in_not(i) (*(PEv2_data->PEv2_digin_Error_in_not[i]))

#undef PEv2_digin_Misc_in
#define PEv2_digin_Misc_in(i) (*(PEv2_data->PEv2_digin_Misc_in[i]))
#undef PEv2_digin_Misc_in_not
#define PEv2_digin_Misc_in_not(i) (*(PEv2_data->PEv2_digin_Misc_in_not[i]))
#undef PEv2_LimitOverride
#define PEv2_LimitOverride (*PEv2_data->PEv2_LimitOverride)
#undef PEv2_LimitOverrideSetup
#define PEv2_LimitOverrideSetup (*PEv2_data->PEv2_LimitOverrideSetup)
#undef PEv2_digin_Probed_in
#define PEv2_digin_Probed_in (*PEv2_data->PEv2_digin_Probed_in)
#undef PEv2_digin_Probe_in
#define PEv2_digin_Probe_in(i) (*(PEv2_data->PEv2_digin_Probe_in[i]))
#undef PEv2_digin_Probe_in_not
#define PEv2_digin_Probe_in_not(i) (*(PEv2_data->PEv2_digin_Probe_in_not[i]))

#undef PEv2_digout_Emergency_out
#define PEv2_digout_Emergency_out (0 + *PEv2_data->PEv2_digout_Emergency_out)
#undef PEv2_digin_SoftLimit_in
#define PEv2_digin_SoftLimit_in(i) (*(PEv2_data->PEv2_digin_SoftLimit_in[i]))
#undef PEv2_AxisEnabledMask
#define PEv2_AxisEnabledMask (*PEv2_data->PEv2_AxisEnabledMask)

#undef PEv2_digout_AxisEnabled_out
#define PEv2_digout_AxisEnabled_out(i) (0 + *(PEv2_data->PEv2_digout_AxisEnabled_out[i]))
#undef PEv2_digin_AxisEnabled_in
#define PEv2_digin_AxisEnabled_in(i) (*(PEv2_data->PEv2_digin_AxisEnabled_in[i]))
#undef PEv2_digout_LimitOverride_out
#define PEv2_digout_LimitOverride_out(i) (*(PEv2_data->PEv2_digout_LimitOverride_out[i]))

#undef PEv2_ExternalOCOutputs
#define PEv2_ExternalOCOutputs (*PEv2_data->PEv2_ExternalOCOutputs)
#undef PEv2_digout_ExternalRelay_out
#define PEv2_digout_ExternalRelay_out(i) (0 + *(PEv2_data->PEv2_digout_ExternalRelay_out[i]))
#undef PEv2_digout_ExternalOC_out
#define PEv2_digout_ExternalOC_out(i) (0 + *(PEv2_data->PEv2_digout_ExternalOC_out[i]))
#undef PEv2_HomingStartMaskSetup
#define PEv2_HomingStartMaskSetup (*PEv2_data->PEv2_HomingStartMaskSetup)
#undef PEv2_ProbeStartMaskSetup
#define PEv2_ProbeStartMaskSetup (*PEv2_data->PEv2_ProbeStartMaskSetup)
#undef PEv2_ProbeStatus
#define PEv2_ProbeStatus (*PEv2_data->PEv2_ProbeStatus)
#undef PEv2_ProbeSpeed
#define PEv2_ProbeSpeed (*PEv2_data->PEv2_ProbeSpeed)
#undef PEv2_BacklashWidth
#define PEv2_BacklashWidth(i) (*(PEv2_data->PEv2_BacklashWidth[i]))
#undef PEv2_BacklashRegister
#define PEv2_BacklashRegister(i) (*(PEv2_data->PEv2_BacklashRegister[i]))
#undef PEv2_BacklashAcceleration
#define PEv2_BacklashAcceleration(i) (*(PEv2_data->PEv2_BacklashAcceleration[i]))
#undef PEv2_BacklashCompensationEnabled
#define PEv2_BacklashCompensationEnabled (*PEv2_data->PEv2_BacklashCompensationEnabled)
#undef PEv2_home_sequence
#define PEv2_home_sequence(i) (PEv2_data->PEv2_home_sequence[i])
#undef PEv2_AxisEnabled
#define PEv2_AxisEnabled(i) (PEv2_data->PEv2_AxisEnabled[i])
#undef PEv2_AxisInverted
#define PEv2_AxisInverted(i) (PEv2_data->PEv2_AxisInverted[i])
#undef PEv2_AxisInternalPlanner
#define PEv2_AxisInternalPlanner(i) (PEv2_data->PEv2_AxisInternalPlanner[i])
#undef PEv2_AxisPositionMode
#define PEv2_AxisPositionMode(i) (PEv2_data->PEv2_AxisPositionMode[i])
#undef PEv2_AxisInvertedHome
#define PEv2_AxisInvertedHome(i) (PEv2_data->PEv2_AxisInvertedHome[i])
#undef PEv2_AxisSoftLimitEnabled
#define PEv2_AxisSoftLimitEnabled(i) (PEv2_data->PEv2_AxisSoftLimitEnabled[i])
#undef PEv2_AxisEnabledMasked
#define PEv2_AxisEnabledMasked(i) (PEv2_data->PEv2_AxisEnabledMasked[i])
#undef PEv2_digin_SoftLimit_PosMin
#define PEv2_digin_SoftLimit_PosMin(i) (PEv2_data->PEv2_digin_SoftLimit_PosMin[i])
#undef PEv2_digin_SoftLimit_PosMax
#define PEv2_digin_SoftLimit_PosMax(i) (PEv2_data->PEv2_digin_SoftLimit_PosMax[i])
#undef PEv2_HomingAlgorithm
#define PEv2_HomingAlgorithm(i) (PEv2_data->PEv2_HomingAlgorithm[i])

// pin io unsigned PEv2.PulseEngineStateSetup;		// Pulse engine new state configuration  - No Pin needed
// uint8_t PEv2_PulseEngineStateSetup = 0;
// uint8_t PulseEngineState = 0;

bool posMode[8];
uint8_t posCount[8];
uint8_t velCount[8];
float last_joint_pos_cmd[8];
float last_joint_vel_cmd[8];

bool Homing_active = false;
bool Homing_done[8] = { false, false, false, false, false, false, false, false };
bool Homing_PkHomeFinalizeeDone[8] = { false, false, false, false, false, false, false, false };
bool Homing_ArmEncodereDone[8] = { false, false, false, false, false, false, false, false };
bool Homing_FinalMoveActive[8] = { false, false, false, false, false, false, false, false };
bool Homing_FinalMoveDone[8] = { false, false, false, false, false, false, false, false };
bool IsHoming[8] = { false, false, false, false, false, false, false, false };
float StepScale[8];
bool Pins_DigitalValueSet_ignore[55];
bool HAL_Machine_On = false;
pokeys_home_command_t old_PEv2_AxesCommand[8] = { 0 };

int oldAxxiState[8] = { 0 };
int repAxxiState[8] = { 0 };
extern unsigned int sleepdur;
extern bool ApplyIniSettings;

/**
 * @brief Main update routine for the PoKeys Pulse Engine v2.
 *
 * This function handles the real-time synchronization between the HAL component
 * and the PoKeys device by reading the current state of each axis, evaluating homing,
 * probing, motion commands, and updating control and feedback variables.
 *
 * It performs the following key tasks:
 * - Applies INI-based initialization settings if enabled
 * - Reads status and diagnostics from the Pulse Engine
 * - Handles homing sequences, including sub-states and transitions
 * - Computes and sets reference positions and speeds based on command inputs
 * - Switches between velocity and position control modes depending on motion state
 * - Sends motion and state commands to the Pulse Engine
 * - Updates feedback pins (e.g., position, limit switches, emergency stop)
 *
 * The function evaluates each axis state in a loop and decides the proper action
 * such as triggering homing sequences or changing the control mode. It also reacts
 * to emergency stop states and ensures correct feedback even during homing.
 *
 * @param dev Pointer to the PoKeys device structure.
 * @param HAL_Machine_On Flag indicating if the HAL machine is enabled (i.e., in operation).
  * @memberof PoKeysHALComponent

 */
void PKPEv2_Update(sPoKeysDevice *dev, bool HAL_Machine_On) {
    uint8_t bm_LimitStatusP; // Limit+ status (bit-mapped)
    uint8_t bm_LimitStatusN; // Limit- status (bit-mapped)
    uint8_t bm_HomeStatus;   // Home status (bit-mapped)
    uint8_t bm_ErrorStatus;
    uint8_t bm_ProbeStatus = dev->PEv2.ProbeStatus; // will be update in "PK_PEv2_ProbingFinish" or "PK_PEv2_ProbingFinishSimple"
    uint8_t bm_DedicatedLimitNInputs;
    uint8_t bm_DedicatedLimitPInputs;
    uint8_t bm_DedicatedHomeInputs;

    if (PEv2_params_ApplyIniSettings == false) {
        ApplyIniSettings = false;
    } else {
        ApplyIniSettings = true;
    }

    PEv2_deb_out = 100;
    if (bm_ProbeStatus != 0) {
        PEv2_digin_Probed_in = true;
    } else {
        PEv2_digin_Probed_in = false;
    }
    PEv2_deb_out = 110;
    if (PEv2_StatusGet(dev) == PK_OK) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_StatusGet(dev) = PK_OK\n", __FILE__, __FUNCTION__);
        PEv2_deb_out = 111;
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_StatusGet(dev) != PK_OK\n", __FILE__, __FUNCTION__);
        PEv2_deb_out = 115;
    }

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_Status2Get(dev)\n", __FILE__, __FUNCTION__);
    PEv2_deb_out = 120;
    if (PEv2_Status2Get(dev) == PK_OK) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_Status2Get(dev) = PK_OK\n", __FILE__, __FUNCTION__);
        PEv2_deb_out = 121;
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Status2Get(dev) != PK_OK\n", __FILE__, __FUNCTION__);
        PEv2_deb_out = 125;
    }

    bool doPositionSet = false;
    bool doMove = false;
    bool isMoving = false;
    bool doStateSet = false;
    bool doHomingStart = false;
    bool doHomingEnd = false;
    int HomingStartMaskSetup = 0;
    bool allhomed = true;
    bool InPosition[8] = { false };
    bool POSITION_MODE_active[8] = { false };

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PEv2_nrOfAxes = %d\n", __FILE__, __FUNCTION__, (*PEv2_data->PEv2_nrOfAxes));
    if ((*PEv2_data->PEv2_nrOfAxes) != 0) {
        uint8_t bm_SoftLimitStatus = dev->PEv2.SoftLimitStatus;
        int tAxisEnabledMask = 0;
        uint8_t bm_DoPositionSet = 0;
        bool finalizingHoming[8] = { false };
        int32_t intCurrentPosition[8] = { 0 };
        float PosFb[8] = { 0 };
        for (int i = 0; i < (*PEv2_data->PEv2_nrOfAxes); i++) {

            if (PEv2_data->PEv2_AxisEnabled[i] == false) {
                rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_Axis[%d] is disabled\n", __FILE__, __FUNCTION__, i);
                continue;
            }
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_Axis[%d] \n", __FILE__, __FUNCTION__, i);
            finalizingHoming[i] = false;
            InPosition[i] = false;
            uint8_t intAxesState = dev->PEv2.AxesState[i];
            uint8_t intAxesCommand = *(PEv2_data->PEv2_AxesCommand[i]);
            // *PEv2_data->PEv2_AxesState[i] = intAxesState; moved later as there were introduced substates of Home
            PEv2_deb_axxisout(i) = 200 + i;
            StepScale[i] = PEv2_data->PEv2_stepgen_STEP_SCALE[i];
            PEv2_deb_axxisout(i) = 210 + i;
            *(PEv2_data->PEv2_CurrentPosition[i]) = dev->PEv2.CurrentPosition[i];
            // intCurrentPosition[i] = dev->PEv2.CurrentPosition[i];
            //*(PEv2_data->PEv2_CurrentPosition[i]) = dev->PEv2.CurrentPosition[i]
            PEv2_deb_axxisout(i) = 220 + i;
            PEv2_digin_Error_in(i) = Get_BitOfByte(bm_ErrorStatus, i);
            PEv2_digin_Error_in_not(i) = !Get_BitOfByte(bm_ErrorStatus, i);
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: intAxesState = %d\n", __FILE__, __FUNCTION__, dev->PEv2.AxesState[i]);
            if (intAxesState != oldAxxiState[i] || old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i])) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d]: new values on AxesState: %s(%d) or AxesCommand:  %s(%d) \n", __FILE__, __FUNCTION__, i, PK_PEAxisState_names[intAxesState], intAxesState, PEv2_AxisCommand_Names[*(PEv2_data->PEv2_AxesCommand[i])], *(PEv2_data->PEv2_AxesCommand[i]));
                //  oldAxxiState[i] = intAxesState;
            }
            /**
             * @brief Synchronised homing state machine trigger for PoKeys axes
             *
             * This function checks if all axes in a homing sequence have reached a specific required state,
             * and transitions them to the given next state synchronously. The transition is only performed
             * if all axes in the sequence are ready.
             *
             * The homing state machine follows this logic:
             * @dot
             * digraph HomingState {
             *   IDLE -> HOMINGSTART;
             *   HOMINGSTART -> HOMINGFinalize;
             *   HOMINGFinalize -> ARMENCODER;
             *   ARMENCODER -> HOMINGWaitFinalMove;
             *   HOMINGWaitFinalMove -> HOMINGFinalMove;
             *   HOMINGFinalMove -> HOMINGFinalize;
             *   HOMINGSTART -> HOMINGCancel;
             *   HOMINGCancel -> IDLE;
             *   HOMINGFinalMove -> IDLE [label="Already at position"];
             * }
             * @enddot
             *
             * Additional logic:
             * - ARMENCODER uses `PK_PEv2_PositionSet()` to reset axis position.
             * - HOMINGFinalMove triggers `PK_PEv2_PulseEngineMove()` only if not already at HomePosition.
             * - `Homing_FinalMoveActive[i]` and `Homing_FinalMoveDone[i]` are used to track motion state and prevent repeated execution.
             * - Final transition to IDLE sets `index_enable = false` and clears `deb_ishoming`.
             *
             * This logic ensures compatibility with LinuxCNC's homing expectations as described in:
             * https://linuxcnc.org/docs/html/config/ini-homing.html
             *
             * @param dev The PoKeys device instance
             * @param seq Homing sequence number (can be shared by multiple joints)
             * @param RequiredState State that must be met before transition is triggered
             * @param NextState Target state to apply if all involved axes are ready
             * @return 0 if transition triggered, 1 if not all axes ready or already transitioned
             */
            switch (intAxesState) {
                case PK_PEAxisState_axSTOPPED: // Axis is stopped
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEAxisState_axSTOPPED\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axSTOPPED \n",
                                        __FILE__, __FUNCTION__, i);
                        *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                        // allhomed = false;
                    }
                    PEv2_deb_out = 310 + i;
                    //  *(PEv2_data->PEv2_HomingStatus[i]) = PK_Homing_axIDLE;
                    // PEv2_digin_AxisEnabled_in(i) = false;
                    // PEv2_digin_LimitOverride_in(i) = false;

                    break;
                case PK_PEAxisState_axREADY: // Axis ready
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEAxisState_axREADY\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axREADY \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // allhomed = false;
                    //  *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                    PEv2_deb_out = 320 + i;
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = false;

                    break;
                case PK_PEAxisState_axRUNNING: // Axis is running
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEAxisState_axRUNNING\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axRUNNING \n",
                                        __FILE__, __FUNCTION__, i);
                    }

                    PEv2_deb_out = 330 + i;
                    break;
                case PK_PEAxisState_axHOMING_RESETTING: // Stopping the axis to reset the position counters
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axHOMING_RESETTING\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axHOMING_RESETTING \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    allhomed = false;
                    // PEv2_digin_LimitOverride_in(i) = true;
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    *(PEv2_data->PEv2_deb_ishoming[i]) = true;
                    PEv2_deb_out = 340 + i;
                    break;
                case PK_PEAxisState_axHOMING_BACKING_OFF: // Backing off switch
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axHOMING_BACKING_OFF\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = PK_PEAxisState_axHOMING_BACKING_OFF \n", __FILE__, __FUNCTION__, i);
                    }
                    allhomed = false;
                    *(PEv2_data->PEv2_deb_ishoming[i]) = true;
                    PEv2_deb_out = 340 + i;

                    // PEv2_digin_LimitOverride_in(i) = true;
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    break;
                case PK_PEAxisState_axHOME: // Axis is homed
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = PK_PEAxisState_axHOME\n", __FILE__, __FUNCTION__, i);
                    // Homing_PkHomeFinalizeeDone[8]
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axHOME \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    PEv2_deb_out = 340 + i;

                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = true;
                    // PEv2_joint_
                    break;
                case PK_PEAxisState_axHOMINGSTART: // Homing procedure is starting on axis
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axHOMINGSTART\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axHOMINGSTART \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = true;
                    allhomed = false;

                    *(PEv2_data->PEv2_deb_ishoming[i]) = true;
                    *(PEv2_data->PEv2_joint_pos_fb[i]) = 0;
                    Homing_active = true;
                    PEv2_deb_out = 340 + i;
                    break;
                case PK_PEAxisState_axHOMINGSEARCH: // Homing procedure first step - going to home
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axHOMINGSEARCH\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axHOMINGSEARCH \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = true;
                    allhomed = false;
                    *(PEv2_data->PEv2_deb_ishoming[i]) = true;

                    Homing_active = true;
                    PEv2_deb_out = 340 + i;
                    break;
                case PK_PEAxisState_axHOMINGBACK: // Homing procedure second step - slow homing
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axHOMINGBACK\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axHOMINGBACK \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = true;
                    *(PEv2_data->PEv2_deb_ishoming[i]) = true;

                    allhomed = false;
                    Homing_active = true;
                    PEv2_deb_out = 340 + i;
                    break;
                case PK_PEAxisState_axPROBED: // Probing completed for this axis
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axPROBED\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axPROBED \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = false;
                    // *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                    PEv2_deb_out = 350 + i;
                    break;
                case PK_PEAxisState_axPROBESTART: // Probing procedure is starting on axis
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axPROBESTART\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axPROBESTART \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = false;
                    // *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                    PEv2_deb_out = 350 + i;
                    break;
                case PK_PEAxisState_axPROBESEARCH: // Probing procedure - probing
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axPROBESEARCH\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axPROBESEARCH \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = false;
                    // *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                    PEv2_deb_out = 350 + i;
                    break;
                case PK_PEAxisState_axERROR: // Axis error
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axERROR\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axERROR \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = false;
                    // PEv2_digin_LimitOverride_in(i) = false;
                    *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                    PEv2_deb_out = 360 + i;

                    break;
                case PK_PEAxisState_axLIMIT: // Axis limit tripped
                    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:PK_PEAxisState_axLIMIT\n", __FILE__, __FUNCTION__);
                    if (oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR,
                                        "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                        "PK_PEAxisState_axLIMIT \n",
                                        __FILE__, __FUNCTION__, i);
                    }
                    // PEv2_digin_AxisEnabled_in(i) = true;
                    // PEv2_digin_LimitOverride_in(i) = false;
                    *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                    PEv2_deb_out = 370 + i;
                    break;
                default:
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s:unknown status\n", __FILE__, __FUNCTION__);
                    PEv2_deb_out = 380 + i;
                    break;
            }

            

            switch (*(PEv2_data->PEv2_AxesCommand[i])) {
                case PK_PEAxisCommand_axIDLE:
                    if (old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i])) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axIDLE\n", __FILE__, __FUNCTION__, i);
                    }

                    if (Homing_FinalMoveActive[i] && !Homing_FinalMoveDone[i]) {
                        if ((dev->PEv2.CurrentPosition[i] == (int32_t)PEv2_data->PEv2_HomePosition[i])) {

                            if (PEv2_HomingStateSyncedTrigger(dev, PEv2_data->PEv2_home_sequence[i], PK_Homing_axHOMINGFinalMove, PK_Homing_axIDLE) == 0) {
                                // intAxesState is already set from dev->PEv2.AxesState[i]
                                Homing_FinalMoveActive[i] = false;
                                Homing_FinalMoveDone[i] = true;
                                InPosition[i] = true;
                                *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                            }
                        }
                    }

                    if (Homing_FinalMoveActive[i] && !Homing_FinalMoveDone[i]) {
                        intAxesState = PEAxisStateEx_HOMINGFINALMOVE; // FinalMove still active
                    }

                    break;
                case PK_PEAxisCommand_axHOMINGSTART:
                    if (old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i]) || intAxesState != repAxxiState[i]) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axHOMINGSTART - (intAxesState:%d IsHoming:%d Homing_PkHomeFinalizeeDone:%d)\n", __FILE__, __FUNCTION__, i, dev->PEv2.AxesState[i], IsHoming[i], Homing_PkHomeFinalizeeDone[i]);
                    }

                    /**
                     * @brief Trigger synchronized homing start based on AxisCommand change.
                     *
                     * This logic checks whether the commanded state of an axis (`PEv2_AxesCommand[i]`)
                     * has changed to a homing start state (`PK_PEAxisCommand_axHOMINGSTART`) and if the axis
                     * is in a valid initial state (STOPPED, READY, or already in HOME).
                     *
                     * When this transition is detected, `PEv2_HomingStateSyncedTrigger()` is called to initiate
                     * homing for all axes that belong to the same homing sequence (`PEv2_home_sequence[i]`).
                     *
                     * This ensures that all axes in the same synchronized group start homing together.
                     *
                     * @note The full mask-based coordination across axes in the same sequence is currently commented out,
                     * but `PEv2_HomingStateSyncedTrigger()` still ensures the correct trigger propagation for synchronization.
                     *
                     * @see PEv2_HomingStateSyncedTrigger()
                     * @see PEv2_AxesCommand
                     * @see PEv2_home_sequence
                     * @memberof PoKeysHALComponent
                     */

                    if ((intAxesState == PK_PEAxisState_axSTOPPED || intAxesState == PK_PEAxisState_axREADY || intAxesState == PK_PEAxisState_axHOME) && IsHoming[i] != true) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Trigger HomingStart\n", __FILE__, __FUNCTION__);
                        if (PEv2_HomingStateSyncedTrigger(dev, PEv2_data->PEv2_home_sequence[i], PK_Homing_axIDLE, PK_Homing_axHOMINGSTART) == 0) {
                            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: ensure that all axes with same Sequence start homing at the same time\n", __FILE__, __FUNCTION__);
                            doHomingStart = true;
                            IsHoming[i] = true;
                            Homing_ArmEncodereDone[i] = false;
                            Homing_PkHomeFinalizeeDone[i] = false; // ensure it is initialized correctly
                            Homing_FinalMoveDone[i] = false;
                            Homing_FinalMoveActive[i] = false;
                            Homing_done[i] = false;
                            // HomingStartMaskSetup = (1 << i); // Home my axis only (bit MyHomeSequ)
                            // rtapi_print_msg(RTAPI_MSG_DBG, "PK_HOMING: ensurinig that all axes (%d) with same Sequence(%d) startmask initialized (%d) \n",  i, PEv2_data->PEv2_home_sequence[i], HomingStartMaskSetup);
                        } else {
                            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axHOMINGSTART - PEv2_HomingStateSyncedTrigger not ready (intAxesState:%d IsHoming:%d Homing_PkHomeFinalizeeDone:%d)\n", __FILE__, __FUNCTION__, i, intAxesState, IsHoming[i], Homing_PkHomeFinalizeeDone[i]);
                        }
                    } else if (intAxesState == PK_PEAxisState_axHOME && !Homing_PkHomeFinalizeeDone[i] && IsHoming[i]) {
                        // ready to Finalize homing

                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisState_axHOME - ready to Finalize homing (%d)\n", __FILE__, __FUNCTION__, i, intAxesState);

                        intAxesState = PEAxisStateEx_axReadyToFinalizeHoming;

                        IsHoming[i] = true;
                        Homing_ArmEncodereDone[i] = false;
                        Homing_PkHomeFinalizeeDone[i] = false;
                        Homing_FinalMoveDone[i] = false;
                        Homing_FinalMoveActive[i] = false;
                        Homing_done[i] = false;
                        if (repAxxiState[i] != intAxesState) {
                            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisState_axHOME - ready to Finalize homing\n", __FILE__, __FUNCTION__, i);
                        }
                    } else if (intAxesState == PK_PEAxisState_axSTOPPED || intAxesState == PK_PEAxisState_axREADY ){
                        if (!Homing_PkHomeFinalizeeDone[i] && IsHoming[i]) {
                            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisState_axSTOPPED - ready to Finalize homing (%d)\n", __FILE__, __FUNCTION__, i, intAxesState);
                            intAxesState = PEAxisStateEx_axReadyToFinalizeHoming;
                        } else if (Homing_PkHomeFinalizeeDone[i] && IsHoming[i]) {
                            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisState_axSTOPPED - Finalize homing done(%d)\n", __FILE__, __FUNCTION__, i, intAxesState);
                            if (intAxesState == PK_PEAxisState_axREADY) {
                                intAxesState = PEAxisStateEx_axReadyToArmEncoder;
                            }
                            else {
                                intAxesState = PEAxisStateEx_axReadyToArmEncoder;
                            }
                            
                        }
                    }

                    break;
                case PK_PEAxisCommand_axARMENCODER:
                    if (old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i]) || oldAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axARMENCODER\n", __FILE__, __FUNCTION__, i);
                    }
                    intAxesState = PEAxisStateEx_axReadyToArmEncoder;
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axARMENCODER - Homing_PkHomeFinalizeeDone: %s Homing_ArmEncodereDone:%s\n", __FILE__, __FUNCTION__, i,Homing_PkHomeFinalizeeDone[i], Homing_ArmEncodereDone[i]);
                    if ((intAxesState == PK_PEAxisState_axREADY) && (Homing_PkHomeFinalizeeDone[i]) && (!Homing_ArmEncodereDone[i])) {
                        // PEAxisStateEx_HOMINGARMENCODER = 17,         // (linuxcnc spec additional state) pokeys resets encoder position to zeros

                        if (PEv2_HomingStateSyncedTrigger(dev, PEv2_data->PEv2_home_sequence[i], PK_Homing_axHOMINGFinalize, PK_Homing_axARMENCODER) == 0) {
                            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axARMENCODER - PEAxisStateEx_HOMINGARMENCODER\n", __FILE__, __FUNCTION__, i);
                            intAxesState = PEAxisStateEx_HOMINGARMENCODER;
                            Homing_ArmEncodereDone[i] = true;
                        }
                        /*dev->PEv2.PositionSetup[i] = PEv2_data->PEv2_ZeroPosition[i];
                        bm_DoPositionSet = Set_BitOfByte(bm_DoPositionSet, i, 1);*/
                    }

                    if (repAxxiState[i] != intAxesState) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axARMENCODER: new reported AxesState:  %s(%d)\n", __FILE__, __FUNCTION__, i,PK_PEAxisState_names[intAxesState],intAxesState);
                    }
                    break;
                case PK_PEAxisCommand_axHOMINGWaitFinalMove:
                    if (old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i])) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axHOMINGWaitFinalMove\n", __FILE__, __FUNCTION__, i);
                    }

                    if (PEv2_HomingStateSyncedTrigger(dev, PEv2_data->PEv2_home_sequence[i], PK_Homing_axARMENCODER, PK_Homing_axHOMINGWaitFinalMove) == 0) {
                        intAxesState = PEAxisStateEx_HOMINGWaitFINALMOVE; // PK_PEAxisState_axHOMINGWaitFINALMOVE = 18,          // (linuxcnc spec additional state) Pokeys moves to homeposition
                        Homing_ArmEncodereDone[i] = true;
                    } else {
                        intAxesState = PEAxisStateEx_HOMINGARMENCODER;
                    }
                    break;

                case PK_PEAxisCommand_axHOMINGFinalMove:
                    if (old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i])) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axHOMINGFinalMove\n", __FILE__, __FUNCTION__, i);
                    }
                    intAxesState = PEAxisStateEx_HOMINGWaitFINALMOVE;
                    if (Homing_FinalMoveDone[i] != true) {

                        if (Homing_FinalMoveActive[i] != true) {
                            if (PEv2_HomingStateSyncedTrigger(dev, PEv2_data->PEv2_home_sequence[i], PK_Homing_axHOMINGWaitFinalMove, PK_Homing_axHOMINGFinalMove) == 0) {
                                intAxesState = PEAxisStateEx_HOMINGFINALMOVE; //	PK_PEAxisState_axHOMINGFINALMOVE = 19,          // (linuxcnc spec additional state) Pokeys moves to homeposition
                                Homing_FinalMoveActive[i] = true;
                            }
                        } else if ((dev->PEv2.CurrentPosition[i] == (int32_t)PEv2_data->PEv2_HomePosition[i])) {

                            if (PEv2_HomingStateSyncedTrigger(dev, PEv2_data->PEv2_home_sequence[i], PK_Homing_axHOMINGFinalMove, PK_Homing_axIDLE) == 0) {
                                // intAxesState is already set from dev->PEv2.AxesState[i]
                                Homing_FinalMoveActive[i] = false;
                                Homing_FinalMoveDone[i] = true;
                                InPosition[i] = true;
                                *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                            }
                        }

                    } else if (Homing_FinalMoveActive[i] != false) {
                        //	PEAxisStateEx_HOMINGFINALMOVE = 19,          // (linuxcnc spec additional state) Pokeys moves to homeposition
                        intAxesState = PEAxisStateEx_HOMINGFINALMOVE;
                        if (old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i])) {
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                            "PK_PEAxisState_axHOMINGFINALMOVE - InPosition[i] = "
                                            "false\n",
                                            __FILE__, __FUNCTION__, i);
                        }
                        if ((dev->PEv2.CurrentPosition[i] != (int32_t)PEv2_data->PEv2_HomePosition[i])) {
                            intAxesState = PEAxisStateEx_HOMINGFINALMOVE;
                            InPosition[i] = false;
                        } else {
                            rtapi_print_msg(RTAPI_MSG_ERR,
                                            "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                            "PK_PEAxisState_axHOME - InPosition[i] = "
                                            "true\n",
                                            __FILE__, __FUNCTION__, i);
                            InPosition[i] = true;
                            *(PEv2_data->PEv2_deb_ishoming[i]) = false;
                            Homing_FinalMoveActive[i] = false;
                        }
                    }
                    break;
                case PK_PEAxisCommand_axHOMINGCancel:
                    if (old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i])) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_Axis[%d] PK_PEAxisCommand_axHOMINGCancel\n", __FILE__, __FUNCTION__, i);
                    }

                    if (*(PEv2_data->PEv2_AxesCommand[i]) == PK_PEAxisCommand_axHOMINGCancel && old_PEv2_AxesCommand[i] != *(PEv2_data->PEv2_AxesCommand[i]) && (intAxesState == PK_PEAxisState_axHOMINGSTART || intAxesState == PK_PEAxisState_axHOMINGSEARCH || intAxesState == PK_PEAxisState_axHOMINGBACK)) {
                        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Trigger HomingCancel\n", __FILE__, __FUNCTION__);
                        //	dev->PEv2.PulseEngineStateSetup = PK_PEState_peSTOPPED;
                    }
                    break;
                case PK_PEAxisCommand_axHOMINGFinalize:

                    /**
                     * @brief Triggers homing finalization for all axes in the same synchronized homing sequence.
                     *
                     * If an axis is currently in the `axHOME` state and receives the `axHOMINGFinalize` command,
                     * this indicates that the homing process for that axis has completed, and it's time to
                     * finalize the homing sequence for all axes that share the same `PEv2_home_sequence`.
                     *
                     * The function `PEv2_HomingStateSyncedTrigger()` is used to propagate the `axHOMINGFinalize`
                     * trigger to all other axes within the same sequence, ensuring that all axes complete
                     * the homing procedure in a synchronized way.
                     *
                     * @note The actual condition that all axes have truly completed homing (`allhomed`) is
                     * commented out here, so finalization may currently rely only on the trigger signal
                     * without verifying the readiness of all involved axes.
                     *
                     * @see PEv2_HomingStateSyncedTrigger()
                     * @see PEv2_AxesCommand
                     * @see PEv2_home_sequence
                     * @see PK_PEAxisState_axHOME
                     * @see PK_PEAxisCommand_axHOMINGFinalize
                     * @memberof PoKeysHALComponent
                     */
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEAxisCommand_axHOMINGFinalize\n", __FILE__, __FUNCTION__);
                    if (dev->PEv2.AxesState[i] == PK_PEAxisState_axHOME || intAxesState == PEAxisStateEx_axReadyToFinalizeHoming) {
                        int MyHomeSequ, seq;
                        MyHomeSequ = PEv2_data->PEv2_home_sequence[i];

                        if (Homing_PkHomeFinalizeeDone[i] != true) {
                            // PEAxisStateEx_HOMINGARMENCODER = 17,         // (linuxcnc spec additional state) pokeys resets encoder position to zeros

                            if (PEv2_HomingStateSyncedTrigger(dev, PEv2_data->PEv2_home_sequence[i], PK_Homing_axHOMINGSTART, PK_Homing_axHOMINGFinalize) == 0) {

                                Homing_PkHomeFinalizeeDone[i] = true;
                                intAxesState = PEAxisStateEx_axReadyToArmEncoder; // keep previous state
                            } else {
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys: %s:%s: PEv2_Axis[%d].AxesState = "
                                                "PK_PEAxisState_axHOME - Homing_PkHomeFinalizeeDone[i] = false\n",
                                                __FILE__, __FUNCTION__, i);
                            }
                        }
                    }
                    break;
                default:
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s:unknown status\n", __FILE__, __FUNCTION__);
                    break;
            }

            if (intAxesState != repAxxiState[i]) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d]: reported Status Changed to: %s (%d) \n", __FILE__, __FUNCTION__, i, PK_PEAxisState_names[intAxesState], intAxesState);

                repAxxiState[i] = intAxesState;
            }
            old_PEv2_AxesCommand[i] = *(PEv2_data->PEv2_AxesCommand[i]);
            oldAxxiState[i] = intAxesState;

            // placed here to as substates PK_PEAxisState_axHOME
            *PEv2_data->PEv2_AxesState[i] = intAxesState;

            // calculate actual velocity by position difference (time estimated by actual rtc_loop_frequ [Hz] / [1/sec] )
            if (*(PEv2_data->PEv2_deb_ishoming[i]) == false && (*PEv2_data->PEv2_AxesState[i] != PEAxisStateEx_HOMINGARMENCODER) && (*PEv2_data->PEv2_AxesState[i] != PEAxisStateEx_HOMINGWaitFINALMOVE) && (*PEv2_data->PEv2_AxesState[i] != PEAxisStateEx_HOMINGFINALMOVE)) {

                // PosFb[i] = (dev->PEv2.CurrentPosition[i] / PEv2_data->PEv2_PositionScale[i]) - PEv2_data->PEv2_PositionOffset[i];

                *(PEv2_data->PEv2_joint_pos_fb[i]) = ((float)*(PEv2_data->PEv2_CurrentPosition[i]) / PEv2_data->PEv2_PositionScale[i]) - PEv2_data->PEv2_PositionOffset[i];

            } else {
                // when homing, use the command position as feedback
                // during homing the position is being reset to 0, so the feedback would be 0
                // which causes FERROR
                *(PEv2_data->PEv2_joint_pos_fb[i]) = PEv2_joint_pos_cmd(i);
                // PosFb[i] = 0;
                //*(PEv2_data->PEv2_joint_pos_fb[i]) = ((float)intCurrentPosition[i] / PEv2_data->PEv2_PositionScale[i]) - PEv2_data->PEv2_PositionOffset[i];
            }

            PEv2_deb_axxisout(i) = 250 + i;

            PEv2_deb_axxisout(i) = 260 + i;

            PEv2_digin_Probe_in(i) = Get_BitOfByte(bm_ProbeStatus, i);

            Read_digin_LimitHome_Pins(dev, i);

            PEv2_deb_axxisout(i) = 280;

            PEv2_digin_SoftLimit_in(i) = Get_BitOfByte(bm_SoftLimitStatus, i);
            PEv2_deb_axxisout(i) = 290 + i;

            /*
                        PEv2.#.digin.Error.in[8]

                        bm_ErrorStatus
                        */

            tAxisEnabledMask = Set_BitOfByte(tAxisEnabledMask, i, PEv2_digout_AxisEnabled_out(i));
            PEv2_LimitOverrideSetup = Set_BitOfByte(PEv2_LimitOverrideSetup, i, PEv2_digout_LimitOverride_out(i));
            PEv2_deb_axxisout(i) = 2900 + i;
            POSITION_MODE_active[i] = Get_BitOfByte(dev->PEv2.AxesConfig[i], 3);
            PEv2_deb_axxisout(i) = 29000 + i;

            if (PEv2_joint_in_position(i) != 0) {
                InPosition[i] = true;
            } else {
                InPosition[i] = false;
            }
            // PEv2_deb_ishoming(i) = IsHoming[i];
            PEv2_deb_inposition(i) = InPosition[i];

            if (HAL_Machine_On != 0) {
                PEv2_deb_out = 390 + i;
                posMode[i] = false;
                if (*(PEv2_data->PEv2_deb_ishoming[i])) {
                    PEv2_deb_axxisout(i) = 2400 + i;
                    // dev->PEv2.ReferencePositionSpeed[i]=0;
                    switch (intAxesState) {
                        case PK_PEAxisState_axHOMING_RESETTING: // Stopping the axis to reset the position counters
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: Axis:%i "
                                            "PK_PEAxisState_axHOMING_RESETTING\n",
                                            __FILE__, __FUNCTION__, i);
                            break;
                        case PK_PEAxisState_axHOMING_BACKING_OFF: // Backing off switch
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: Axis:%i "
                                            "PK_PEAxisState_axHOMING_BACKING_OFF\n",
                                            __FILE__, __FUNCTION__, i);
                            break;
                        case PK_PEAxisState_axHOME: // Axis is homed
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: Axis:%i "
                                            "PK_PEAxisState_axHOME\n",
                                            __FILE__, __FUNCTION__, i);
                            break;
                        case PK_PEAxisState_axHOMINGSTART: // Homing procedure is starting on axis
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: Axis:%i "
                                            "PK_PEAxisState_axHOMINGSTART\n",
                                            __FILE__, __FUNCTION__, i);
                            break;
                        case PK_PEAxisState_axHOMINGSEARCH: // Homing procedure first step - going to home
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: Axis:%i "
                                            "PK_PEAxisState_axHOMINGSEARCH\n",
                                            __FILE__, __FUNCTION__, i);
                            break;
                        case PK_PEAxisState_axHOMINGBACK: // Homing procedure second step - slow homing
                            rtapi_print_msg(RTAPI_MSG_DBG,
                                            "PoKeys: %s:%s: Axis:%i "
                                            "PK_PEAxisState_axHOMINGBACK\n",
                                            __FILE__, __FUNCTION__, i);
                            break;

                        default:
                            break;
                    }
                    PEv2_deb_axxisout(i) = 2410 + i;
                } else {
                    float VelCmd = *(PEv2_data->PEv2_joint_vel_cmd[i]);
                    float PosCmd = PEv2_joint_pos_cmd(i);

                    PEv2_deb_out = 400 + i;

                    float ReferenceSpeed;
                    float ReferencePosition;
                    PEv2_deb_axxisout(i) = 1000;

                    if (StepScale[i] != 0) {
                        PEv2_deb_axxisout(i) = 1100 + i;
                        *(PEv2_data->PEv2_ReferenceSpeed[i]) = VelCmd * StepScale[i];
                        PEv2_deb_axxisout(i) = 1110 + i;
                        *(PEv2_data->PEv2_ReferencePosition[i]) = (PosCmd + PEv2_data->PEv2_PositionOffset[i]) * PEv2_data->PEv2_PositionScale[i];
                        //	*(PEv2_data->PEv2_PositionSetup[i]) = ReferencePosition;
                        PEv2_deb_axxisout(i) = 1120 + i;
                    } else {
                        PEv2_deb_axxisout(i) = 1200 + i;
                        *(PEv2_data->PEv2_ReferenceSpeed[i]) = VelCmd;
                        PEv2_deb_axxisout(i) = 1210 + i;
                        *(PEv2_data->PEv2_ReferencePosition[i]) = PosCmd;
                        //*(PEv2_data->PEv2_PositionSetup[i]) = ReferencePosition;
                        PEv2_deb_axxisout(i) = 1220 + i;
                    }

                    /*
                                        for the last short move before in-position is reached switch to positionmode for more precise positioning
                                        */
                    if (InPosition[i] && (dev->PEv2.CurrentPosition[i] != (int32_t) * (PEv2_data->PEv2_ReferencePosition[i]))) {
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys: %s:%s: Axis:%i InPosition[i] && "
                                        "(dev->PEv2.CurrentPosition[i] (%d) != "
                                        "(int32_t)ReferencePosition (%d)) \n",
                                        __FILE__, __FUNCTION__, i, dev->PEv2.CurrentPosition[i], (int32_t) * (PEv2_data->PEv2_ReferencePosition[i]));
                        InPosition[i] = false;
                    }
                    // InPosition[i] might have changed in previous junction
                    if (InPosition[i] == false) {
                        dev->PEv2.param1 = i;
                        PK_PEv2_AxisConfigurationGet(dev);
                        POSITION_MODE_active[i] = Get_BitOfByte(dev->PEv2.AxesConfig[i], 3);
#ifdef ULAPI
                        usleep(sleepdur);
#endif
                        PEv2_deb_out = 210;
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys: %s:%s: 'Position Mode' "
                                        "Axis:%i InPosition[i] == false "
                                        "POSITION_MODE_active[i] = %d \n",
                                        __FILE__, __FUNCTION__, i, POSITION_MODE_active[i]);

                        if (*(PEv2_data->PEv2_ReferenceSpeed[i]) == 0) {
                            posMode[i] = true;

                            dev->PEv2.param1 = i;
                            PK_PEv2_AxisConfigurationGet(dev);
                            POSITION_MODE_active[i] = Get_BitOfByte(dev->PEv2.AxesConfig[i], 3);
#ifdef ULAPI
                            usleep(sleepdur);
#endif
                            if ((POSITION_MODE_active[i] == false)) {
                                posCount[i]++;
                                // first ensure stopped Axis
                                if (dev->PEv2.ReferencePositionSpeed[i] != 0) {
                                    dev->PEv2.ReferencePositionSpeed[i] = 0;
                                    PK_PEv2_PulseEngineMove(dev);
                                    // usleep(sleepdur);
                                }

                                // then switch to Position Mode
                                dev->PEv2.AxesConfig[i] = Set_BitOfByte(dev->PEv2.AxesConfig[i], 3, true);
                                dev->PEv2.param1 = i;

                                if (PK_PEv2_AxisConfigurationSet(dev) != PK_OK) {
                                    rtapi_print_msg(RTAPI_MSG_ERR,
                                                    "PoKeys: %s:%s: 'switch to Position "
                                                    "Mode' "
                                                    "PK_PEv2_AxisConfigurationSet!=PK_OK\n",
                                                    __FILE__, __FUNCTION__);
                                    PK_PEv2_AxisConfigurationSet(dev);
                                } else {
                                    rtapi_print_msg(RTAPI_MSG_DBG,
                                                    "PoKeys: %s:%s: 'switch to Position "
                                                    "Mode' "
                                                    "PK_PEv2_AxisConfigurationSet=PK_OK\n",
                                                    __FILE__, __FUNCTION__);
                                }
#ifdef ULAPI
                                usleep(sleepdur);
#endif
                            } else {
                                posCount[i] += 1000;
                            }
                        } else {
                            posCount[i] += 100000;
                        }
                    } else {
                        // posMode[i] = false;
                        dev->PEv2.param1 = i;
                        PK_PEv2_AxisConfigurationGet(dev);
                        POSITION_MODE_active[i] = Get_BitOfByte(dev->PEv2.AxesConfig[i], 3);
                        rtapi_print_msg(RTAPI_MSG_DBG,
                                        "PoKeys: %s:%s: 'Velocity Mode' "
                                        "Axis:%i InPosition[i] == true "
                                        "POSITION_MODE_active[i] = %d \n",
                                        __FILE__, __FUNCTION__, i, POSITION_MODE_active[i]);
#ifdef ULAPI
                        usleep(sleepdur);
#endif
                        if (POSITION_MODE_active[i] == true) {
                            velCount[i]++;
                            dev->PEv2.AxesConfig[i] = Set_BitOfByte(dev->PEv2.AxesConfig[i], 3, false);
                            dev->PEv2.param1 = i;
                            if (PK_PEv2_AxisConfigurationSet(dev) != PK_OK) {
                                rtapi_print_msg(RTAPI_MSG_ERR,
                                                "PoKeys: %s:%s: 'switch to Velocity Mode' "
                                                "PK_PEv2_AxisConfigurationSet!=PK_OK\n",
                                                __FILE__, __FUNCTION__);
                                PK_PEv2_AxisConfigurationSet(dev);
                            } else {
                                rtapi_print_msg(RTAPI_MSG_DBG,
                                                "PoKeys: %s:%s: 'switch to Velocity Mode' "
                                                "PK_PEv2_AxisConfigurationSet=PK_OK\n",
                                                __FILE__, __FUNCTION__);
                            }
#ifdef ULAPI
                            usleep(sleepdur);
#endif

                            posMode[i] = false;
                        }
                    }

                    dev->PEv2.param1 = i;
                    PK_PEv2_AxisConfigurationGet(dev);
                    POSITION_MODE_active[i] = Get_BitOfByte(dev->PEv2.AxesConfig[i], 3);
#ifdef ULAPI
                    usleep(sleepdur);
#endif
                    // dev->PEv2.param1 = i;			// redundant ??!
                    // PK_PEv2_AxisConfigurationGet(dev);
                    // POSITION_MODE_active[i] = Get_BitOfByte(dev->PEv2.AxesConfig[i], 3);
                    // usleep(sleepdur);

                    PEv2_deb_posmode_count(i) = posCount[i];
                    PEv2_deb_velmode_count(i) = velCount[i];
                    PEv2_deb_axxisout(i) = 1300 + i;
                    PEv2_deb_RefSpeed(i) = *(PEv2_data->PEv2_ReferenceSpeed[i]);
                    PEv2_deb_RefPos(i) = *(PEv2_data->PEv2_ReferencePosition[i]);
                    PEv2_deb_PosMode(i) = posMode[i];
                    PEv2_deb_PosModeAct(i) = POSITION_MODE_active[i];

                    if (POSITION_MODE_active[i] == true) {
                        PEv2_deb_axxisout(i) = 13100 + i;
                        if (InPosition[i] == false) {
                            if (last_joint_pos_cmd[i] != PEv2_joint_pos_cmd(i)) {
                                PEv2_deb_axxisout(i) = 1310 + i;
                                dev->PEv2.ReferencePositionSpeed[i] = *(PEv2_data->PEv2_ReferencePosition[i]);
                                *(PEv2_data->PEv2_ReferencePositionSpeed[i]) = *(PEv2_data->PEv2_ReferencePosition[i]);
                                PEv2_deb_axxisout(i) = 1320 + i;
                                last_joint_pos_cmd[i] = PEv2_joint_pos_cmd(i);
                                if (dev->PEv2.AxesState[i] == PK_PEAxisState_axREADY || dev->PEv2.AxesState[i] == PK_PEAxisState_axSTOPPED || dev->PEv2.AxesState[i] == PK_PEAxisState_axRUNNING || dev->PEv2.AxesState[i] == PK_PEAxisState_axHOME) {
                                    // PK_PEv2_PulseEngineMove(dev);
                                    doMove = true;
                                    PEv2_deb_axxisout(i) = 1330 + i;
                                } else {
                                    PEv2_deb_axxisout(i) = 1340 + i;
                                }
                            }
                        } else {

                            PEv2_deb_axxisout(i) = 1600 + i;
                            if (dev->PEv2.ReferencePositionSpeed[i] != dev->PEv2.CurrentPosition[i]) {
                                PEv2_deb_axxisout(i) = 160 + i;
                                dev->PEv2.AxesConfig[i] = Set_BitOfByte(dev->PEv2.AxesConfig[i], 3, false);
                                dev->PEv2.param1 = i;
                                PK_PEv2_AxisConfigurationSet(dev);
#ifdef ULAPI
                                usleep(sleepdur);
#endif

                                dev->PEv2.ReferencePositionSpeed[i] = 0;
                                *(PEv2_data->PEv2_ReferencePositionSpeed[i]) = 0;
                                last_joint_vel_cmd[i] = 0;
                                doMove = true;
                            }

                            POSITION_MODE_active[i] = Get_BitOfByte(dev->PEv2.AxesConfig[i], 3);
                            if (POSITION_MODE_active[i] == true) {
                                dev->PEv2.AxesConfig[i] = Set_BitOfByte(dev->PEv2.AxesConfig[i], 3, false);
                                dev->PEv2.param1 = i;
                                PK_PEv2_AxisConfigurationSet(dev);
#ifdef ULAPI
                                usleep(sleepdur);
#endif
                                PK_PEv2_AxisConfigurationSet(dev);
#ifdef ULAPI
                                usleep(sleepdur);
#endif
                                // if (PK_PEv2_AxisConfigurationSet(dev) = PK_OK)
                                //{
                                dev->PEv2.ReferencePositionSpeed[i] = 0;
                                *(PEv2_data->PEv2_ReferencePositionSpeed[i]) = 0;
                                last_joint_vel_cmd[i] = 0;
                                doMove = true;
                                //}
                                /*else
                                                                {
                                                                        dev->PEv2.AxesConfig[i] = Set_BitOfByte(dev->PEv2.AxesConfig[i], 3, false);
                                                                        dev->PEv2.param1 = i;
                                                                        PK_PEv2_AxisConfigurationSet(dev);
                                                                        dev->PEv2.ReferencePositionSpeed[i] = 0;
                                                                        *(PEv2_data->PEv2_ReferencePositionSpeed[i]) = 0;
                                                                        last_joint_vel_cmd[i] = 0;
                                                                        doMove = true;
                                                                }*/
                            }
                        }
                    } else {
                        if (InPosition[i] != true) {
                            PEv2_deb_axxisout(i) = 14100 + i;
                            if (last_joint_vel_cmd[i] != *(PEv2_data->PEv2_joint_vel_cmd[i])) {
                                PEv2_deb_axxisout(i) = 1410 + i;

                                dev->PEv2.ReferencePositionSpeed[i] = (int32_t) * (PEv2_data->PEv2_ReferenceSpeed[i]);
                                *(PEv2_data->PEv2_ReferencePositionSpeed[i]) = (int)*(PEv2_data->PEv2_ReferenceSpeed[i]);
                                PEv2_deb_axxisout(i) = 1420 + i;
                                last_joint_vel_cmd[i] = *(PEv2_data->PEv2_joint_vel_cmd[i]);
                                if (dev->PEv2.AxesState[i] == PK_PEAxisState_axRUNNING || dev->PEv2.AxesState[i] == PK_PEAxisState_axREADY || dev->PEv2.AxesState[i] == PK_PEAxisState_axHOME) {
                                    rtapi_print_msg(RTAPI_MSG_DBG,
                                                    "PoKeys: %s:%s: Axis:%i "
                                                    "PK_PEAxisState_axRUNNING || "
                                                    "PK_PEAxisState_axREADY || "
                                                    "PK_PEAxisState_axHOME\n",
                                                    __FILE__, __FUNCTION__, i);
                                    doMove = true;
                                    PEv2_deb_axxisout(i) = 1430 + i;
                                } else {
                                    PEv2_deb_axxisout(i) = 1440 + i;
                                }
                            } else {
                                PEv2_deb_axxisout(i) = 1450 + i;
                            }
                        } else {
                            PEv2_deb_axxisout(i) = 1700 + i;
                            // InPosition -> StopAxis
                            if (dev->PEv2.ReferencePositionSpeed[i] != 0) {
                                dev->PEv2.ReferencePositionSpeed[i] = 0;
                                *(PEv2_data->PEv2_ReferencePositionSpeed[i]) = 0;
                                last_joint_vel_cmd[i] = 0;
                                doMove = true;
                            }
                        }
                    }
                }

            } else {
                PEv2_deb_out = 3900 + i;
            }

            PEv2_deb_doMove(i) = doMove;
            PEv2_deb_RefPosSpeed(i) = dev->PEv2.ReferencePositionSpeed[i];
        }

        /*if (bm_DoPositionSet != 0) {
            PEv2_deb_out = 4000;
            dev->PEv2.param2 = bm_DoPositionSet;
            for (int i = 0; i < (*PEv2_data->PEv2_nrOfAxes); i++) {
                // if bit is set, then set IsHoming to false
                if (bm_DoPositionSet & (1 << i)) {
                    rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d].DoPositionSetup = %d \n", __FILE__, __FUNCTION__, i, PEv2_data->PEv2_ZeroPosition[i]);
                    dev->PEv2.PositionSetup[i] = PEv2_data->PEv2_ZeroPosition[i];
                }
            }
            //
            if (PK_PEv2_PositionSet(dev) != PK_OK) {
                rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_PositionSet!=PK_OK\n", __FILE__, __FUNCTION__);
#ifdef ULAPI
                usleep(sleepdur);
#endif

            } else {
                for (int i = 0; i < (*PEv2_data->PEv2_nrOfAxes); i++) {
                    // if bit is set, then set IsHoming to false
                    if (bm_DoPositionSet & (1 << i)) {
                        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_Axis[%d].DoPositionSet = 0 \n", __FILE__, __FUNCTION__, i);
                        Homing_ArmEncodereDone[i] = true;
                    }
                }
                bm_DoPositionSet = 0;
            }

#ifdef ULAPI
            usleep(sleepdur);
#endif
        }*/
        if (allhomed != false) {

            for (int i = 0; i < (*PEv2_data->PEv2_nrOfAxes); i++) {
                //		*(PEv2_data->PEv2_deb_ishoming[i]) = false;
            }
        }
        if (tAxisEnabledMask != dev->PEv2.AxisEnabledMask) {
            dev->PEv2.AxisEnabledMask = tAxisEnabledMask;
            doStateSet = 1;
        }
        if (PEv2_LimitOverrideSetup != dev->PEv2.LimitOverrideSetup) {
            dev->PEv2.LimitOverrideSetup = PEv2_LimitOverrideSetup;
            doStateSet = 1;
        }
    }

    if (PEv2_ExternalOutputsSet(dev) == PK_OK) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_ExternalOutputsSet=PK_OK\n", __FILE__, __FUNCTION__);
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_ExternalOutputsSet!=PK_OK\n", __FILE__, __FUNCTION__);
    }

    if (dev->PEv2.HomingStartMaskSetup != HomingStartMaskSetup && HomingStartMaskSetup != 0 && doHomingStart) {

    } else if (dev->PEv2.HomingStartMaskSetup != HomingStartMaskSetup && HomingStartMaskSetup != 0 && doHomingEnd) {

    } else if (*(PEv2_data->PEv2_PulseEngineState) != *(PEv2_data->PEv2_PulseEngineStateSetup) && doHomingStart == 0) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PK_PEv2: PEv2_PulseEngineStateSetup (%d) \n", *(PEv2_data->PEv2_PulseEngineStateSetup));
        dev->PEv2.PulseEngineStateSetup = *(PEv2_data->PEv2_PulseEngineStateSetup);
        // dev->PEv2.AxisEnabledMask = PEv2_AxisEnabledMask;
        doStateSet = true;
    }

    if (PEv2_LimitOverrideSetup != dev->PEv2.LimitOverrideSetup) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PK_PEv2: PEv2_LimitOverrideSetup (%d) \n", PEv2_LimitOverrideSetup);
        dev->PEv2.LimitOverrideSetup = PEv2_LimitOverrideSetup;
        doStateSet = true;
    }

    if (doStateSet == true) {
        rtapi_print_msg(RTAPI_MSG_DBG, "PK_PEv2: PEv2_PulseEngineStateSet (%d) \n", dev->PEv2.PulseEngineStateSetup);

        if (PK_PEv2_PulseEngineStateSet(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_PulseEngineStateSet!=PK_OK\n", __FILE__, __FUNCTION__);
            if (PK_PEv2_PulseEngineStateSet(dev) != PK_OK) {
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_PulseEngineStateSet == PK_OK \n" __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
    }

    if (doMove == true) {
        PEv2_deb_out = 4000;
        rtapi_print_msg(RTAPI_MSG_DBG, "PK_PEv2: PEv2_PulseEngineMove  \n");
        if (PK_PEv2_PulseEngineMove(dev) != PK_OK) {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_PulseEngineMove!=PK_OK\n", __FILE__, __FUNCTION__);

            if (PK_PEv2_PulseEngineMove(dev) != PK_OK) {
            }
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_PulseEngineMove == PK_OK \n", __FILE__, __FUNCTION__);
            PEv2_deb_out = 4500;
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
    }

    PEv2_deb_out = 224;
    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PulseEngineState = %d\n", __FILE__, __FUNCTION__, dev->PEv2.PulseEngineState);
    if (dev->PEv2.PulseEngineState == PK_PEState_peSTOP_EMERGENCY) {
        (*PEv2_data->PEv2_digin_Emergency_in) = true;
        (*PEv2_data->PEv2_digin_Emergency_in_not) = false;
        (*PEv2_data->PEv2_deb_estop) = 9;
    } else if (dev->PEv2.EmergencyInputPin != 0) {
        int PinId = dev->PEv2.EmergencyInputPin - 9 - 1;
        int polarity = dev->PEv2.EmergencySwitchPolarity;
        if (polarity == false) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Emergency Switch PinId: %d polarity:%d \n", __FILE__, __FUNCTION__, PinId, polarity);
            (*PEv2_data->PEv2_digin_Emergency_in) = *(IO_data->Pin[PinId]).digin_in;
            (*PEv2_data->PEv2_digin_Emergency_in_not) = *(IO_data->Pin[PinId]).digin_in_not;
            ;

            (*PEv2_data->PEv2_deb_estop) = 10;
        } else {
            rtapi_print_msg(RTAPI_MSG_DBG,
                            "PoKeys: %s:%s: Emergency Switch inverted PinId: "
                            "%d polarity:%d \n",
                            __FILE__, __FUNCTION__, PinId, polarity);

            (*PEv2_data->PEv2_digin_Emergency_in) = *(IO_data->Pin[PinId]).digin_in_not;
            (*PEv2_data->PEv2_digin_Emergency_in_not) = *(IO_data->Pin[PinId]).digin_in;
            (*PEv2_data->PEv2_deb_estop) = 11;
        }
    } else {
        (*PEv2_data->PEv2_digin_Emergency_in) = true;
        (*PEv2_data->PEv2_digin_Emergency_in_not) = false;
        (*PEv2_data->PEv2_deb_estop) = 12;
    }
}

/**
 * @brief Applies configuration to the PoKeys Pulse Engine v2 during initialization.
 *
 * This function sets up the Pulse Engine v2 (PEv2) based on either the INI file settings or the current
 * runtime state of the PoKeys device. It handles the initialization and configuration of:
 * - Pulse Engine core settings (enabled state, charge pump, generator type)
 * - Digital I/O pin configuration (probe and emergency output)
 * - Axis-specific settings (enabled flags, motion planner modes, limits, homing behavior)
 * - Additional parameters such as emergency polarity and runtime-reported status
 *
 * If `ApplyIniSettings` is true, the function applies the values from the PEv2_data structure
 * (populated by `PKPEv2_ReadIniFile`) to the device using `PEv2_PulseEngineSetup()` and related calls.
 * Otherwise, it reads the current device configuration back into `PEv2_data` using `PK_PEv2_StatusGet()`,
 * `PK_PEv2_Status2Get()`, and `PEv2_AdditionalParametersGet()`.
 *
 * The probe pin is configured as a digital input if specified. The emergency output pin is also set
 * as a digital output and added to the ignore list for value setting, as it is not directly configured
 * by the PoKeys firmware.
 *
 * Finally, for each axis, either `PEv2_AxisConfigurationSet()` or `PEv2_AxisConfigurationGet()` is called,
 * depending on whether settings are to be applied or queried.
 *
 * @param dev Pointer to the PoKeys device structure. This must be initialized and connected.
 *
 * @note This function should be called after reading the INI file and before enabling motion control.
 * It ensures the hardware is properly configured to match the desired logical setup.
 * @memberof PoKeysHALComponent
 */
void PKPEv2_Setup(sPoKeysDevice *dev) {
    bool DoPeSetup = false;
    bool DoPeReboot = false;

    rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: ApplyIniSettings = %d\n", __FILE__, __FUNCTION__, ApplyIniSettings);

    if ((ApplyIniSettings == true)) {
        // dev->PEv2.AxisEnabledStatesMask=0; //Disable axis power when not in Running state
        // PK_PEv2_PulseEngineSetup(dev);

        if (PEv2_PulseEngineSetup(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_PulseEngineSetup == PK_OK\n", __FILE__, __FUNCTION__);
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_PulseEngineSetup != PK_OK\n", __FILE__, __FUNCTION__);
        }

        // Set Probe pin - but it will be send to pokeys on PK_PEv2_ProbingStart(dev) or PK_PEv2_ProbingHybridStart(dev)
        if (PEv2_data->PEv2_digin_Probe_Pin != 0) { // check if pin is parametrized in HAL
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: IO_data->Pin[%d].PinFunction = %d\n", __FILE__, __FUNCTION__, PEv2_data->PEv2_digin_Probe_Pin - 1, IO_data->Pin[PEv2_data->PEv2_digin_Probe_Pin - 1].PinFunction);
            if (IO_data->Pin[PEv2_data->PEv2_digin_Probe_Pin - 1].PinFunction != PK_PinCap_digitalInput) {
                IO_data->Pin[PEv2_data->PEv2_digin_Probe_Pin - 1].PinFunction = PK_PinCap_digitalInput;
            }
        }

        if (PEv2_AdditionalParametersSet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_AdditionalParametersSet == PK_OK\n", __FILE__, __FUNCTION__);
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_AdditionalParametersSet != PK_OK\n", __FILE__, __FUNCTION__);
        }

        // that Pin is not defined explicitely on pokeys
        if (PEv2_data->PEv2_digout_Emergency_Pin != 0) { // check if pin is parametrized in HAL
            if (IO_data->Pin[PEv2_data->PEv2_digout_Emergency_Pin - 1].PinFunction != PK_PinCap_digitalOutput) {
                IO_data->Pin[PEv2_data->PEv2_digout_Emergency_Pin - 1].PinFunction = PK_PinCap_digitalOutput;
                Pins_DigitalValueSet_ignore[PEv2_data->PEv2_digout_Emergency_Pin - 1] = true;
            }
        }
    } else {
        if (PK_PEv2_StatusGet(dev) == PK_OK && PK_PEv2_Status2Get(dev) == PK_OK) {
            PEv2_data->PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
            PEv2_data->PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
            PEv2_data->PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;
            PEv2_data->PEv2_digin_Emergency_invert = dev->PEv2.EmergencySwitchPolarity;

            PEv2_data->PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
            PEv2_data->PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
            PEv2_data->PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;
            PEv2_data->PEv2_digin_Emergency_invert = dev->PEv2.EmergencySwitchPolarity;
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR,
                            "PoKeys: %s:%s: PK_PEv2_StatusGet!=PK_OK || "
                            "PK_PEv2_Status2Get(dev) != PK_OK\n",
                            __FILE__, __FUNCTION__);
        }
#ifdef ULAPI
        usleep(sleepdur);
#endif
        if (PEv2_AdditionalParametersGet(dev) == PK_OK) {
            rtapi_print_msg(RTAPI_MSG_DBG,
                            "PoKeys: %s:%s: PEv2_AdditionalParametersGet == "
                            "PK_OK \n" __FILE__,
                            __FUNCTION__);
        } else {
            rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersGet!=PK_OK\n", __FILE__, __FUNCTION__);
        }
    }

    for (int i = 0; i < dev->PEv2.info.nrOfAxes; i++) {

        if (ApplyIniSettings == true) {
            if (PEv2_data->PEv2_AxisEnabled[i] != 0) {
                PEv2_AxisConfigurationSet(dev, i);
            }

        } else {
            PEv2_AxisConfigurationGet(dev, i);
        }
    }
    // dev->PEv2.param1 = 0;
    if (PK_PEv2_StatusGet(dev) == PK_OK) {
        PEv2_data->PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
        PEv2_data->PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
        PEv2_data->PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;
        // PEv2_stepgen_PulseEngineBufferSize = dev->PEv2.PulseEngineBufferSize;
        if (ApplyIniSettings != true) {
            PEv2_data->PEv2_digin_Emergency_invert = dev->PEv2.EmergencySwitchPolarity;
        }
        // PEv2_stepgen_AxisEnabledStatesMask = dev->PEv2.AxisEnabledStatesMask;
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_StatusGet() != PK_OK\n", __FILE__, __FUNCTION__);
    }
#ifdef ULAPI
    usleep(sleepdur);
#endif
}

/**
 * @brief Reads configuration parameters for the PoKeys Pulse Engine v2 from the INI file.
 *
 * This function loads all Pulse Engine v2 settings from the [POKEYS] section of the HAL INI file.
 * It initializes general settings like probe, emergency input/output pins, and the state of the
 * pulse engine and charge pump. For each available axis, it loads axis configuration, switch assignments,
 * soft limits, speed parameters, homing algorithms, filter settings, and MPG (manual pulse generator) jogging options.
 *
 * The values are stored into the shared PEv2_data structure, which is later used during runtime for control and motion logic.
 *
 * @param dev Pointer to the PoKeys device structure. This provides the number of available axes via `dev->PEv2.info.nrOfAxes`.
 *
 * @note This function should be called once during initialization to prepare the component for correct operation.
 *
 * INI parameters read include (per axis or globally):
 * - PEv2_ProbeInput, PEv2_EmergencyInputPin, PEv2_ChargePumpEnabled, etc.
 * - Axis-specific: PEv2_AxisEnabled_#, PEv2_AxesConfig_#, PEv2_MaxSpeed_#, PEv2_HomingSpeed_#, PEv2_AxesSwitchConfig_#, etc.
 * - Digital input/output pin assignments and inversion flags
 * - Homing logic and movement constraints
 * @memberof PoKeysHALComponent
 */
void PKPEv2_ReadIniFile(sPoKeysDevice *dev) {
    char key[256]; // Puffer für den zusammengesetzten String

    PEv2_data->PEv2_digin_Probe_Pin = ini_read_int("POKEYS", "PEv2_ProbeInput", 0);
    PEv2_data->PEv2_digin_Probe_invert = ini_read_int("POKEYS", "PEv2_ProbeInputPolarity", 0);

    PEv2_data->PEv2_digin_Emergency_Pin = ini_read_int("POKEYS", "PEv2_EmergencyInputPin", 0);

    PEv2_data->PEv2_digout_Emergency_Pin = ini_read_int("POKEYS", "PEv2_EmergencyOutputPin", 0);

    // PEv2_PulseEngineSetup settings
    PEv2_data->PEv2_PulseEngineEnabled = ini_read_int("POKEYS", "PEv2_PulseEngineEnabled", 0);
    PEv2_data->PEv2_ChargePumpEnabled = ini_read_int("POKEYS", "PEv2_ChargePumpEnabled", 0);
    PEv2_data->PEv2_PulseGeneratorType = ini_read_int("POKEYS", "PEv2_PulseGeneratorType", 0);
    PEv2_data->PEv2_digin_Emergency_invert = ini_read_int("POKEYS", "PEv2_EmergencyInputPolarity", 0);
    // not in use	*PEv2_data->PEv2_AxisEnabledStatesMask = ini_read_int("POKEYS", "PEv2_EmergencyOutputPolarity", 0);
    for (int AxisId = 0; AxisId < dev->PEv2.info.nrOfAxes; AxisId++) {
        // PEv2_AxisConfigurationGet settings

        /* Section: Axis configuration
                        PEv2_AxesConfig
                                PK_AC_ENABLED            = (1 << 0),       // 1 Axis enabled
                                PK_AC_INVERTED           = (1 << 1),       // 2 Axis inverted
                                PK_AC_INTERNAL_PLANNER   = (1 << 2),       // 4 Axis uses internal motion planner
                                PK_AC_POSITION_MODE      = (1 << 3),       // 8 Internal motion planner for this axis is in position mode
                                PK_AC_INVERTED_HOME      = (1 << 4),       // 16 Axis homing direction is inverted
                                PK_AC_SOFT_LIMIT_ENABLED = (1 << 5),       // 32 Use soft-limits for this axis
                                PK_AC_ENABLED_MASKED     = (1 << 7)        // 128 Use output enable pin masking
                        */
        snprintf(key, sizeof(key), "PEv2_AxesConfig_%i", AxisId);
        PEv2_data->PEv2_AxesConfig[AxisId] = ini_read_int("POKEYS", key, 0);

        snprintf(key, sizeof(key), "PEv2_AxisEnabled_%i", AxisId);
        PEv2_data->PEv2_AxisEnabled[AxisId] = ini_read_int("POKEYS", key, 0);

        snprintf(key, sizeof(key), "PEv2_AxisInverted_%i", AxisId);
        PEv2_data->PEv2_AxisInverted[AxisId] = ini_read_int("POKEYS", key, 0);

        snprintf(key, sizeof(key), "PEv2_AxisInternalPlanner_%i", AxisId);
        PEv2_data->PEv2_AxisInternalPlanner[AxisId] = ini_read_int("POKEYS", key, 0);

        snprintf(key, sizeof(key), "PEv2_AxisPositionMode_%i", AxisId);
        PEv2_data->PEv2_AxisPositionMode[AxisId] = ini_read_int("POKEYS", key, 0);

        snprintf(key, sizeof(key), "PEv2_AxisSoftLimitEnabled_%i", AxisId);
        PEv2_data->PEv2_AxisInvertedHome[AxisId] = ini_read_int("POKEYS", key, 0);

        snprintf(key, sizeof(key), "PEv2_AxisEnabled_%i", AxisId);
        PEv2_data->PEv2_AxisSoftLimitEnabled[AxisId] = ini_read_int("POKEYS", key, 0);

        snprintf(key, sizeof(key), "PEv2_AxisEnabledMasked_%i", AxisId);
        PEv2_data->PEv2_AxisEnabledMasked[AxisId] = ini_read_int("POKEYS", key, 0);
        // not in use		PEv2_data->PEv2_AxisSignalOptions[AxisId] = ini_read_int("POKEYS", "PEv2_AxisSignalOptions", 0);

        /* Section: Axis switch configuration
                PEv2_AxesSwitchConfig
                        PK_ASO_SWITCH_LIMIT_N        = (1 << 0),   // 1 Limit- switch
                        PK_ASO_SWITCH_LIMIT_P        = (1 << 1),   // 2 Limit+ switch
                        PK_ASO_SWITCH_HOME           = (1 << 2),   // 4 Home switch
                        PK_ASO_SWITCH_COMBINED_LN_H  = (1 << 3),   // 8 Home switch is shared with Limit- switch
                        PK_ASO_SWITCH_COMBINED_LP_H  = (1 << 4),   // 16 Home switch is shared with Limit+ switch
                        PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5),   // 32 Invert limit- switch polarity
                        PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6),   // 64 Invert limit+ switch polarity
                        PK_ASO_SWITCH_INVERT_HOME    = (1 << 7)    // 128 Invert home switch polarity
                */
        // PEv2_data->PEv2_AxesSwitchConfig[AxisId] - PEv2_AxesSwitchConfig_0
        snprintf(key, sizeof(key), "PEv2_AxesSwitchConfig_%i", AxisId);
        PEv2_data->PEv2_AxesSwitchConfig[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_LimitN_Pin[AxisId] - PEv2_LimitN_Pin_0
        snprintf(key, sizeof(key), "PEv2_EnableLimitN_%i", AxisId);
        PEv2_data->PEv2_digin_LimitN_Enabled[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_LimitP_Enabled[AxisId] - PEv2_EnableLimitP_0
        snprintf(key, sizeof(key), "PEv2_EnableLimitP_%i", AxisId);
        PEv2_data->PEv2_digin_LimitP_Enabled[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_Home_Enabled[AxisId] - PEv2_EnableHome_0
        snprintf(key, sizeof(key), "PEv2_EnableHome_%i", AxisId);
        PEv2_data->PEv2_digin_Home_Enabled[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_Home_OnLimitN[AxisId] - PEv2_HomeOnLimitN_0
        snprintf(key, sizeof(key), "PEv2_HomeOnLimitN_%i", AxisId);
        PEv2_data->PEv2_digin_Home_OnLimitN[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_Home_OnLimitP[AxisId] - PEv2_HomeOnLimitP_0
        snprintf(key, sizeof(key), "PEv2_HomeOnLimitP_%i", AxisId);
        PEv2_data->PEv2_digin_Home_OnLimitP[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_LimitN_invert[AxisId] - PEv2_InvertLimitN_0
        snprintf(key, sizeof(key), "PEv2_InvertLimitN_%i", AxisId);
        PEv2_data->PEv2_digin_LimitN_invert[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_LimitP_invert[AxisId] - PEv2_InvertLimitP_0
        snprintf(key, sizeof(key), "PEv2_InvertLimitP_%i", AxisId);
        PEv2_data->PEv2_digin_LimitP_invert[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_Home_invert[AxisId] - PEv2_InvertHome_0
        snprintf(key, sizeof(key), "PEv2_InvertHome_%i", AxisId);
        PEv2_data->PEv2_digin_Home_invert[AxisId] = ini_read_int("POKEYS", key, 0);

        /* Section: Pin configuration
         */
        // PEv2_data->PEv2_digin_LimitN_Pin[AxisId] - PEv2_PinLimitMSwitch_0
        snprintf(key, sizeof(key), "PEv2_PinLimitMSwitch_%i", AxisId);
        PEv2_data->PEv2_digin_LimitN_Pin[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_LimitP_Pin[AxisId] - PEv2_PinLimitPSwitch_0
        snprintf(key, sizeof(key), "PEv2_PinLimitPSwitch_%i", AxisId);
        PEv2_data->PEv2_digin_LimitP_Pin[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_Home_Pin[AxisId] - PEv2_PinHomeSwitch_0
        snprintf(key, sizeof(key), "PEv2_PinHomeSwitch_%i", AxisId);
        PEv2_data->PEv2_digin_Home_Pin[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_MaxSpeed[AxisId] - PEv2_MaxSpeed_0
        snprintf(key, sizeof(key), "PEv2_MaxSpeed_%i", AxisId);
        PEv2_data->PEv2_MaxSpeed[AxisId] = ini_read_float("POKEYS", key, 0);

        // PEv2_data->PEv2_MaxAcceleration[AxisId]
        snprintf(key, sizeof(key), "PEv2_MaxAcceleration_%i", AxisId);
        PEv2_data->PEv2_MaxAcceleration[AxisId] = ini_read_float("POKEYS", key, 0);

        // PEv2_data->PEv2_MaxDecceleration[AxisId]
        snprintf(key, sizeof(key), "PEv2_MaxDecceleration_%i", AxisId);
        PEv2_data->PEv2_MaxDecceleration[AxisId] = ini_read_float("POKEYS", key, 0);

        // PEv2_data->PEv2_HomingSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomingSpeed_%i", AxisId);
        PEv2_data->PEv2_HomingSpeed[AxisId] = ini_read_float("POKEYS", key, 0);

        // PEv2_data->PEv2_HomingReturnSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomingReturnSpeed_%i", AxisId);
        PEv2_data->PEv2_HomingReturnSpeed[AxisId] = ini_read_float("POKEYS", key, 0);

        // PEv2_data->PEv2_HomingAlgorithm[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomingAlgorithm_%i", AxisId);
        PEv2_data->PEv2_HomingAlgorithm[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OnHome_Stop[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_Stop_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OnHome_Stop[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OnHome_ArmEncoder[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_ArmEncoder_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OnHome_ArmEncoder[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OnHome_RevDirection[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_RevDirection_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OnHome_RevDirection[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OnHome_ReducedSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_ReducedSpeed_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OnHome_ReducedSpeed[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OutHome_Stop[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_Stop_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OutHome_Stop[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OutHome_ArmEncoder[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_ArmEncoder_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OutHome_ArmEncoder[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OutHome_RevDirection[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_RevDirection_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OutHome_RevDirection[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeAlg_OutHome_ReducedSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_ReducedSpeed_%i", AxisId);
        PEv2_data->PEv2_HomeAlg_OutHome_ReducedSpeed[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_HomeBackOffDistance[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeBackOffDistance_%i", AxisId);
        PEv2_data->PEv2_HomeBackOffDistance[AxisId] = ini_read_float("POKEYS", key, 0);

        // PEv2_data->PEv2_MPGjogEncoder[AxisId]
        snprintf(key, sizeof(key), "PEv2_MPGjogEncoder_%i", AxisId);
        PEv2_data->PEv2_MPGjogEncoder[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_MPGjogDivider[AxisId]
        snprintf(key, sizeof(key), "PEv2_MPGjogDivider_%i", AxisId);
        PEv2_data->PEv2_MPGjogDivider[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_MPGjogMultiplier[AxisId]
        snprintf(key, sizeof(key), "PEv2_MPGjogMultiplier_%i", AxisId);
        PEv2_data->PEv2_MPGjogMultiplier[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_FilterProbeInput
        snprintf(key, sizeof(key), "PEv2_FilterProbeInput_%i", AxisId);
        PEv2_data->PEv2_digin_Probe_Filter[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_LimitN_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterLimitMSwitch_%i", AxisId);
        PEv2_data->PEv2_digin_LimitN_Filter[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_LimitP_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterLimitPSwitch_%i", AxisId);
        PEv2_data->PEv2_digin_LimitP_Filter[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_Home_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterHomeSwitch_%i", AxisId);
        PEv2_data->PEv2_digin_Home_Filter[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digin_Probe_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterProbeSwitch_%i", AxisId);
        PEv2_data->PEv2_digin_Probe_Filter[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_SoftLimitMinimum[AxisId]
        snprintf(key, sizeof(key), "PEv2_SoftLimitMinimum_%i", AxisId);
        PEv2_data->PEv2_SoftLimitMinimum[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_SoftLimitMaximum[AxisId]
        snprintf(key, sizeof(key), "PEv2_SoftLimitMaximum_%i", AxisId);
        PEv2_data->PEv2_SoftLimitMaximum[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] - PEv2_AxisEnableOutputPins_0
        snprintf(key, sizeof(key), "PEv2_AxisEnableOutputPins_%i", AxisId);
        PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] = ini_read_int("POKEYS", key, 0);

        // PEv2_data->PEv2_digout_AxisEnable_invert[AxisId] - PEv2_InvertAxisEnable_0
        snprintf(key, sizeof(key), "PEv2_InvertAxisEnable_%i", AxisId);
        PEv2_data->PEv2_digout_AxisEnable_invert[AxisId] = ini_read_int("POKEYS", key, 0);
    }
}

/**
 * @brief Write the configuration settings to the INI file
 *
 * @param dev
 * @memberof PoKeysHALComponent
 */
void PKPEv2_WriteIniFile(sPoKeysDevice *dev) {
    char key[256]; // Puffer für den zusammengesetzten String

    ini_write_int("PROBE", "PEv2_ProbeInput", PEv2_data->PEv2_digin_Probe_Pin);
    ini_write_int("PROBE", "PEv2_ProbeInputPolarity", PEv2_data->PEv2_digin_Probe_invert);
    ini_write_int("ESTOP", "PEv2_EmergencyInputPin", PEv2_data->PEv2_digin_Emergency_Pin);
    ini_write_int("ESTOP", "PEv2_EmergencyInputPolarity", PEv2_data->PEv2_digin_Emergency_invert);
    ini_write_int("ESTOP", "PEv2_EmergencyOutputPin", PEv2_data->PEv2_digout_Emergency_Pin);
    // PEv2_PulseEngineSetup settings
    ini_write_int("PEv2", "PEv2_PulseEngineEnabled", PEv2_data->PEv2_PulseEngineEnabled);
    ini_write_int("PEv2", "PEv2_ChargePumpEnabled", PEv2_data->PEv2_ChargePumpEnabled);
    ini_write_int("PEv2", "PEv2_PulseGeneratorType", PEv2_data->PEv2_PulseGeneratorType);

    // not in use	*PEv2_data->PEv2_AxisEnabledStatesMask = ini_read_int("POKEYS", "PEv2_EmergencyOutputPolarity", 0);
    for (int AxisId = 0; AxisId < dev->PEv2.info.nrOfAxes; AxisId++) {
        // PEv2_AxisConfigurationGet settings

        /* Section: Axis configuration
                        PEv2_AxesConfig
                                PK_AC_ENABLED            = (1 << 0),       // 1 Axis enabled
                                PK_AC_INVERTED           = (1 << 1),       // 2 Axis inverted
                                PK_AC_INTERNAL_PLANNER   = (1 << 2),       // 4 Axis uses internal motion planner
                                PK_AC_POSITION_MODE      = (1 << 3),       // 8 Internal motion planner for this axis is in position mode
                                PK_AC_INVERTED_HOME      = (1 << 4),       // 16 Axis homing direction is inverted
                                PK_AC_SOFT_LIMIT_ENABLED = (1 << 5),       // 32 Use soft-limits for this axis
                                PK_AC_ENABLED_MASKED     = (1 << 7)        // 128 Use output enable pin masking
                        */
        snprintf(key, sizeof(key), "PEv2_AxesConfig_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxesConfig[AxisId]);

        snprintf(key, sizeof(key), "PEv2_AxisEnabled_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxisEnabled[AxisId]);

        snprintf(key, sizeof(key), "PEv2_AxisInverted_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxisInverted[AxisId]);

        snprintf(key, sizeof(key), "PEv2_AxisInternalPlanner_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxisInternalPlanner[AxisId]);

        snprintf(key, sizeof(key), "PEv2_AxisPositionMode_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxisPositionMode[AxisId]);

        snprintf(key, sizeof(key), "PEv2_AxisSoftLimitEnabled_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxisInvertedHome[AxisId]);
        snprintf(key, sizeof(key), "PEv2_AxisEnabled_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxisSoftLimitEnabled[AxisId]);
        snprintf(key, sizeof(key), "PEv2_AxisEnabledMasked_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxisEnabledMasked[AxisId]);
        // not in use		PEv2_data->PEv2_AxisSignalOptions[AxisId] = ini_read_int("POKEYS", "PEv2_AxisSignalOptions", 0);
        /* Section: Axis switch configuration
                PEv2_AxesSwitchConfig
                        PK_ASO_SWITCH_LIMIT_N        = (1 << 0),   // 1 Limit- switch
                        PK_ASO_SWITCH_LIMIT_P        = (1 << 1),   // 2 Limit+ switch
                        PK_ASO_SWITCH_HOME           = (1 << 2),   // 4 Home switch
                        PK_ASO_SWITCH_COMBINED_LN_H  = (1 << 3),   // 8 Home switch is shared with Limit- switch
                        PK_ASO_SWITCH_COMBINED_LP_H  = (1 << 4),   // 16 Home switch is shared with Limit+ switch
                        PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5),   // 32 Invert limit- switch polarity
                        PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6),   // 64 Invert limit+ switch polarity
                        PK_ASO_SWITCH_INVERT_HOME    = (1 << 7)    // 128 Invert home switch polarity
                */
        // PEv2_data->PEv2_AxesSwitchConfig[AxisId] - PEv2_AxesSwitchConfig_0
        snprintf(key, sizeof(key), "PEv2_AxesSwitchConfig_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_AxesSwitchConfig[AxisId]);
        // PEv2_data->PEv2_digin_LimitN_Pin[AxisId] - PEv2_LimitN_Pin_0
        snprintf(key, sizeof(key), "PEv2_EnableLimitN_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitN_Enabled[AxisId]);
        // PEv2_data->PEv2_digin_LimitP_Enabled[AxisId] - PEv2_EnableLimitP_0
        snprintf(key, sizeof(key), "PEv2_EnableLimitP_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitP_Enabled[AxisId]);
        // PEv2_data->PEv2_digin_Home_Enabled[AxisId] - PEv2_EnableHome_0
        snprintf(key, sizeof(key), "PEv2_EnableHome_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Home_Enabled[AxisId]);
        // PEv2_data->PEv2_digin_Home_OnLimitN[AxisId] - PEv2_HomeOnLimitN_0
        snprintf(key, sizeof(key), "PEv2_HomeOnLimitN_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Home_OnLimitN[AxisId]);
        // PEv2_data->PEv2_digin_Home_OnLimitP[AxisId] - PEv2_HomeOnLimitP_0
        snprintf(key, sizeof(key), "PEv2_HomeOnLimitP_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Home_OnLimitP[AxisId]);
        // PEv2_data->PEv2_digin_LimitN_invert[AxisId] - PEv2_InvertLimitN_0

        snprintf(key, sizeof(key), "PEv2_InvertLimitN_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitN_invert[AxisId]);
        // PEv2_data->PEv2_digin_LimitP_invert[AxisId] - PEv2_InvertLimitP_0
        snprintf(key, sizeof(key), "PEv2_InvertLimitP_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitP_invert[AxisId]);
        // PEv2_data->PEv2_digin_Home_invert[AxisId] - PEv2_InvertHome_0
        snprintf(key, sizeof(key), "PEv2_InvertHome_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Home_invert[AxisId]);
        /* Section: Pin configuration
         */
        // PEv2_data->PEv2_digin_LimitN_Pin[AxisId] - PEv2_PinLimitMSwitch_0
        snprintf(key, sizeof(key), "PEv2_PinLimitMSwitch_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitN_Pin[AxisId]);
        // PEv2_data->PEv2_digin_LimitP_Pin[AxisId] - PEv2_PinLimitPSwitch_0
        snprintf(key, sizeof(key), "PEv2_PinLimitPSwitch_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitP_Pin[AxisId]);
        // PEv2_data->PEv2_digin_Home_Pin[AxisId] - PEv2_PinHomeSwitch_0
        snprintf(key, sizeof(key), "PEv2_PinHomeSwitch_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Home_Pin[AxisId]);
        // PEv2_data->PEv2_MaxSpeed[AxisId] - PEv2_MaxSpeed_0
        snprintf(key, sizeof(key), "PEv2_MaxSpeed_%i", AxisId);
        ini_write_float("POKEYS", key, PEv2_data->PEv2_MaxSpeed[AxisId]);
        // PEv2_data->PEv2_MaxAcceleration[AxisId]
        snprintf(key, sizeof(key), "PEv2_MaxAcceleration_%i", AxisId);
        ini_write_float("POKEYS", key, PEv2_data->PEv2_MaxAcceleration[AxisId]);
        // PEv2_data->PEv2_MaxDecceleration[AxisId]
        snprintf(key, sizeof(key), "PEv2_MaxDecceleration_%i", AxisId);
        ini_write_float("POKEYS", key, PEv2_data->PEv2_MaxDecceleration[AxisId]);
        // PEv2_data->PEv2_HomingSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomingSpeed_%i", AxisId);
        ini_write_float("POKEYS", key, PEv2_data->PEv2_HomingSpeed[AxisId]);
        // PEv2_data->PEv2_HomingReturnSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomingReturnSpeed_%i", AxisId);
        ini_write_float("POKEYS", key, PEv2_data->PEv2_HomingReturnSpeed[AxisId]);
        // PEv2_data->PEv2_HomingAlgorithm[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomingAlgorithm_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomingAlgorithm[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OnHome_Stop[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_Stop_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OnHome_Stop[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OnHome_ArmEncoder[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_ArmEncoder_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OnHome_ArmEncoder[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OnHome_RevDirection[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_RevDirection_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OnHome_RevDirection[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OnHome_ReducedSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OnHome_ReducedSpeed_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OnHome_ReducedSpeed[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OutHome_Stop[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_Stop_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OutHome_Stop[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OutHome_ArmEncoder[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_ArmEncoder_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OutHome_ArmEncoder[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OutHome_RevDirection[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_RevDirection_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OutHome_RevDirection[AxisId]);
        // PEv2_data->PEv2_HomeAlg_OutHome_ReducedSpeed[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeAlg_OutHome_ReducedSpeed_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_HomeAlg_OutHome_ReducedSpeed[AxisId]);
        // PEv2_data->PEv2_HomeBackOffDistance[AxisId]
        snprintf(key, sizeof(key), "PEv2_HomeBackOffDistance_%i", AxisId);
        ini_write_float("POKEYS", key, PEv2_data->PEv2_HomeBackOffDistance[AxisId]);
        // PEv2_data->PEv2_MPGjogEncoder[AxisId]
        snprintf(key, sizeof(key), "PEv2_MPGjogEncoder_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_MPGjogEncoder[AxisId]);
        // PEv2_data->PEv2_MPGjogDivider[AxisId]
        snprintf(key, sizeof(key), "PEv2_MPGjogDivider_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_MPGjogDivider[AxisId]);
        // PEv2_data->PEv2_MPGjogMultiplier[AxisId]
        snprintf(key, sizeof(key), "PEv2_MPGjogMultiplier_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_MPGjogMultiplier[AxisId]);
        // PEv2_data->PEv2_FilterProbeInput
        snprintf(key, sizeof(key), "PEv2_FilterProbeInput_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Probe_Filter[AxisId]);
        // PEv2_data->PEv2_digin_LimitN_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterLimitMSwitch_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitN_Filter[AxisId]);
        // PEv2_data->PEv2_digin_LimitP_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterLimitPSwitch_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_LimitP_Filter[AxisId]);
        // PEv2_data->PEv2_digin_Home_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterHomeSwitch_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Home_Filter[AxisId]);
        // PEv2_data->PEv2_digin_Probe_Filter[AxisId]
        snprintf(key, sizeof(key), "PEv2_FilterProbeSwitch_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digin_Probe_Filter[AxisId]);
        // PEv2_data->PEv2_SoftLimitMinimum[AxisId]
        snprintf(key, sizeof(key), "PEv2_SoftLimitMinimum_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_SoftLimitMinimum[AxisId]);
        // PEv2_data->PEv2_SoftLimitMaximum[AxisId]
        snprintf(key, sizeof(key), "PEv2_SoftLimitMaximum_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_SoftLimitMaximum[AxisId]);
        // PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] - PEv2_AxisEnableOutputPins_0
        snprintf(key, sizeof(key), "PEv2_AxisEnableOutputPins_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId]);
        // PEv2_data->PEv2_digout_AxisEnable_invert[AxisId] - PEv2_InvertAxisEnable_0
        snprintf(key, sizeof(key), "PEv2_InvertAxisEnable_%i", AxisId);
        ini_write_int("POKEYS", key, PEv2_data->PEv2_digout_AxisEnable_invert[AxisId]);
        // PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] - PEv2_AxisEnableOutputPins_0
    }
}
#ifdef RTAPI
EXPORT_SYMBOL(PKPEv2_Init);
EXPORT_SYMBOL(PKPEv2_ReadIniFile);
EXPORT_SYMBOL(PKPEv2_WriteIniFile);
EXPORT_SYMBOL(PKPEv2_Setup);
EXPORT_SYMBOL(PKPEv2_Update);
#endif
