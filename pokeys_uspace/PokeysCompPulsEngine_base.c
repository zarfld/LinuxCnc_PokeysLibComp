typedef struct {
	hal_s32_t* PEv2_deb_out;
	hal_s32_t* PEv2_deb_estop;
	hal_s32_t* PEv2_deb_axxisout[8];
	hal_bit_t* PEv2_deb_ishoming[8];
	hal_bit_t* PEv2_deb_inposition[8];
	hal_bit_t* PEv2_deb_PosMode[8];
	hal_bit_t* PEv2_deb_PosModeAct[8];
	hal_s32_t* PEv2_deb_velmode_count[8];
	hal_s32_t* PEv2_deb_posmode_count[8];
	hal_bit_t* PEv2_deb_doMove[8];
	hal_float_t* PEv2_deb_RefSpeed[8];
	hal_float_t* PEv2_deb_RefPos[8];
	hal_float_t* PEv2_deb_RefPosSpeed[8];
	hal_u32_t* PEv2_nrOfAxes;
	hal_u32_t* PEv2_maxPulseFrequency;
	hal_u32_t* PEv2_bufferDepth;
	hal_u32_t* PEv2_slotTiming;
	hal_bit_t* PEv2_params_ApplyIniSettings;
	hal_u32_t* PEv2_AxesState[8];
	hal_u32_t* PEv2_AxesCommand[8];
	hal_u32_t PEv2_AxesConfig[8];
	hal_u32_t PEv2_SoftLimitMaximum[8];
	hal_u32_t PEv2_SoftLimitMinimum[8];
	hal_u32_t PEv2_HomingSpeed[8];
	hal_u32_t PEv2_HomingReturnSpeed[8];
	hal_u32_t PEv2_HomeOffsets[8];
	hal_u32_t* PEv2_ProbePosition[8];
	hal_u32_t* PEv2_ProbeMaxPosition[8];
	hal_s32_t* PEv2_CurrentPosition[8];
	hal_s32_t* PEv2_PositionSetup[8];
	hal_u32_t* PEv2_ReferencePositionSpeed[8];
	hal_u32_t* PEv2_ReferencePosition[8];
	hal_u32_t* PEv2_ReferenceSpeed[8];
	hal_float_t PEv2_MaxSpeed[8];
	hal_float_t PEv2_MaxAcceleration[8];
	hal_float_t PEv2_MaxDecceleration[8];
	hal_float_t* PEv2_params_Feedback_Encoder_Id[8];
	hal_s32_t PEv2_ZeroPosition[8];
	hal_s32_t PEv2_ArmPosition[8];
	hal_s32_t PEv2_HomePosition[8];

	hal_s32_t PEv2_PositionScale[8];
	hal_s32_t PEv2_PositionOffset[8];

	hal_float_t* PEv2_joint_vel_cmd[8];
	hal_float_t* PEv2_joint_pos_cmd[8];
	hal_float_t* PEv2_joint_pos_fb[8];
	hal_bit_t* PEv2_joint_out_home[8];
	hal_bit_t* PEv2_joint_in_position[8];
	hal_bit_t* PEv2_joint_kb_jog_active[8];
	hal_bit_t* PEv2_joint_wheel_jog_active[8];
	hal_s32_t* PEv2_stepgen_TYPE[8];
	hal_float_t PEv2_stepgen_HOME[8];
	hal_float_t PEv2_stepgen_STEPGEN_MAXVEL[8];
	hal_float_t PEv2_stepgen_STEPGEN_MAXACCEL[8];

	hal_float_t* PEv2_stepgen_DEADBAND[8];
	hal_float_t* PEv2_stepgen_MAX_OUTPUT[8];
	hal_float_t* PEv2_stepgen_ENCODER_SCALE[8];
	hal_float_t PEv2_stepgen_STEP_SCALE[8];
	hal_float_t PEv2_stepgen_MIN_LIMIT[8];
	hal_float_t PEv2_stepgen_MAX_LIMIT[8];

	hal_float_t PEv2_stepgen_HOME_OFFSET[8];
	hal_float_t PEv2_stepgen_HOME_SEARCH_VEL[8];
	hal_float_t PEv2_stepgen_HOME_LATCH_VEL[8];
	hal_float_t* PEv2_stepgen_HOME_FINAL_VEL[8];
	hal_s32_t* PEv2_stepgen_HOME_IGNORE_LIMITS[8];
	hal_u32_t PEv2_MPGjogMultiplier[8];
	hal_u32_t PEv2_MPGjogEncoder[8];
	hal_u32_t PEv2_MPGjogDivider[8];
	hal_u32_t PEv2_HomeBackOffDistance[8];
	hal_bit_t* PEv2_digin_Error_in[8];
	hal_bit_t* PEv2_digin_Error_in_not[8];
	hal_u32_t* PEv2_MiscInputStatus;
	hal_u32_t* PEv2_BacklashWidth[8];
	hal_u32_t* PEv2_BacklashRegister[8];
	hal_u32_t* PEv2_BacklashAcceleration[8];
	hal_s32_t PEv2_home_sequence[8];
	hal_s32_t PEv2_AxisEnabled[8];
	hal_s32_t PEv2_AxisInverted[8];
	hal_s32_t PEv2_AxisInternalPlanner[8];
	hal_s32_t PEv2_AxisPositionMode[8];
	hal_s32_t PEv2_AxisInvertedHome[8];
	hal_s32_t PEv2_AxisSoftLimitEnabled[8];
	hal_s32_t PEv2_AxisEnabledMasked[8];
	hal_u32_t PEv2_AxesSwitchConfig[8];
	hal_u32_t PEv2_HomingAlgorithm[8];
	hal_bit_t PEv2_HomeAlg_OnHome_Stop[8];
	hal_bit_t PEv2_HomeAlg_OnHome_ArmEncoder[8];
	hal_bit_t PEv2_HomeAlg_OnHome_RevDirection[8];
	hal_bit_t PEv2_HomeAlg_OnHome_ReducedSpeed[8];
	hal_bit_t PEv2_HomeAlg_OutHome_Stop[8];
	hal_bit_t PEv2_HomeAlg_OutHome_ArmEncoder[8];
	hal_bit_t PEv2_HomeAlg_OutHome_RevDirection[8];
	hal_bit_t PEv2_HomeAlg_OutHome_ReducedSpeed[8];
	hal_u32_t PEv2_digin_Home_Offset[8];
	hal_u32_t PEv2_digin_SoftLimit_PosMin[8];
	hal_u32_t PEv2_digin_SoftLimit_PosMax[8];
	hal_bit_t PEv2_digin_LimitN_Enabled[8];
	hal_bit_t PEv2_digin_LimitP_Enabled[8];
	hal_bit_t PEv2_digin_Home_Enabled[8];
	hal_bit_t PEv2_digin_Home_OnLimitN[8];
	hal_bit_t PEv2_digin_Home_OnLimitP[8];
	hal_bit_t PEv2_digin_LimitN_invert[8];
	hal_bit_t PEv2_digin_LimitP_invert[8];
	hal_bit_t PEv2_digin_Home_invert[8];
	hal_u32_t PEv2_digin_LimitN_Pin[8];
	hal_u32_t PEv2_digin_LimitN_Filter[8];
	hal_u32_t PEv2_digin_LimitP_Pin[8];
	hal_u32_t PEv2_digin_LimitP_Filter[8];
	hal_u32_t PEv2_digin_Home_Pin[8];
	hal_u32_t PEv2_digin_Home_Filter[8];
	hal_u32_t PEv2_digout_AxisEnable_Pin[8];
	hal_bit_t PEv2_digout_AxisEnable_invert[8];
	hal_bit_t* PEv2_digin_Misc_in[8];
	hal_bit_t* PEv2_digin_Misc_in_not[8];
	hal_bit_t* PEv2_digin_Probe_in[8];
	hal_bit_t* PEv2_digin_Probe_in_not[8];
	hal_u32_t PEv2_digin_Probe_Filter[8];
	hal_u32_t* PEv2_digin_SoftLimit_in[8];
	hal_bit_t* PEv2_digout_AxisEnabled_out[8];
	hal_bit_t* PEv2_digin_AxisEnabled_in[8];
	hal_bit_t* PEv2_digout_LimitOverride_out[8];
	hal_bit_t* PEv2_digin_LimitN_in[8];
	hal_bit_t* PEv2_digin_LimitN_in_not[8];
	hal_bit_t* PEv2_digin_LimitN_DedicatedInput[8];
	hal_bit_t* PEv2_digin_LimitP_in[8];
	hal_bit_t* PEv2_digin_LimitP_in_not[8];
	hal_bit_t* PEv2_digin_LimitP_DedicatedInput[8];
	hal_bit_t* PEv2_digin_Home_in[8];
	hal_bit_t* PEv2_digin_Home_in_not[8];
	hal_bit_t* PEv2_digin_Home_DedicatedInput[8];
	hal_bit_t* PEv2_digout_AxisEnable_out[8];
	hal_u32_t PEv2_PulseEngineEnabled;
	hal_u32_t PEv2_PulseGeneratorType;
	hal_bit_t PEv2_PG_swap_stepdir;
	hal_bit_t PEv2_PG_extended_io;
	hal_u32_t PEv2_ChargePumpEnabled;
	hal_u32_t* PEv2_PulseEngineActivated;
	hal_u32_t* PEv2_PulseEngineState;
	hal_u32_t* PEv2_LimitOverride;
	hal_u32_t* PEv2_LimitOverrideSetup;
	hal_bit_t* PEv2_digin_Probed_in;
	hal_bit_t* PEv2_digin_Emergency_in;
	hal_bit_t* PEv2_digin_Emergency_in_not;
	hal_bit_t* PEv2_digout_Emergency_out;
	hal_bit_t* PEv2_digout_ExternalRelay_out[8];
	hal_bit_t* PEv2_digout_ExternalOC_out[8];
	hal_u32_t PEv2_digin_Emergency_Pin;
	hal_u32_t PEv2_digin_Emergency_invert;
	hal_u32_t PEv2_digout_Emergency_Pin;
	hal_u32_t PEv2_digin_Probe_Pin;
	hal_u32_t PEv2_digin_Probe_invert;
	hal_u32_t* PEv2_AxisEnabledMask;
	hal_u32_t* PEv2_AxisEnabledStatesMask;
	hal_u32_t* PEv2_ExternalRelayOutputs;
	hal_u32_t* PEv2_ExternalOCOutputs;
	hal_u32_t* PEv2_HomingStartMaskSetup;
	hal_u32_t* PEv2_ProbeStartMaskSetup;
	hal_u32_t* PEv2_ProbeStatus;
	hal_float_t* PEv2_ProbeSpeed;
	hal_u32_t* PEv2_BacklashCompensationEnabled;
}PEv2_data_t;

int PKPEv2_export_pins(char* prefix, long extra_arg, int comp_id, PEv2_data_t* Pev2_data, sPoKeysDevice* dev) {

	int r = 0;
	int j = 0;

	rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: prefix: %s\n", __FILE__, __FUNCTION__, prefix);
	if (dev == NULL) {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s dev == NULL\n", __FILE__, __FUNCTION__);
	}

	if (Pev2_data == NULL) {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s Pev2_data == NULL\n", __FILE__, __FUNCTION__);
		PEv2_data = hal_malloc(sizeof(PEv2_data_t));
		if (PEv2_data == NULL) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s PEv2_data == NULL\n", __FILE__, __FUNCTION__);
			return -1;
		}
	}
	else {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s Pev2_data != NULL\n", __FILE__, __FUNCTION__);
		PEv2_data = Pev2_data;
	}

	r = hal_pin_s32_newf(HAL_OUT, &(PEv2_data->PEv2_deb_out), comp_id,
		"%s.PEv2.deb.out", prefix);
	if (r != 0)
		return r;
	r = hal_pin_s32_newf(HAL_OUT, &(PEv2_data->PEv2_deb_estop), comp_id,
		"%s.PEv2.deb.estop", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_nrOfAxes), comp_id,
		"%s.PEv2.nrOfAxes", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_maxPulseFrequency), comp_id,
		"%s.PEv2.maxPulseFrequency", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_bufferDepth), comp_id,
		"%s.PEv2.bufferDepth", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_slotTiming), comp_id,
		"%s.PEv2.slotTiming", prefix);
	if (r != 0)
		return r;
	r = hal_pin_bit_newf(HAL_IO, &(PEv2_data->PEv2_params_ApplyIniSettings), comp_id,
		"%s.PEv2.params.ApplyIniSettings", prefix);
	if (r != 0)
		return r;

	for (j = 0; j < (8); j++) {
		r = hal_pin_s32_newf(HAL_OUT, &(PEv2_data->PEv2_deb_axxisout[j]), comp_id,
			"%s.PEv2.%01d.deb.axxisout", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_deb_ishoming[j]), comp_id,
			"%s.PEv2.%01d.deb.ishoming", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_deb_inposition[j]), comp_id,
			"%s.PEv2.%01d.deb.inposition", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_deb_PosMode[j]), comp_id,
			"%s.PEv2.%01d.deb.PosMode", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_deb_PosModeAct[j]), comp_id,
			"%s.PEv2.%01d.deb.PosModeAct", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_s32_newf(HAL_OUT, &(PEv2_data->PEv2_deb_velmode_count[j]), comp_id,
			"%s.PEv2.%01d.deb.velmode-count", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_s32_newf(HAL_OUT, &(PEv2_data->PEv2_deb_posmode_count[j]), comp_id,
			"%s.PEv2.%01d.deb.posmode-count", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_deb_doMove[j]), comp_id,
			"%s.PEv2.%01d.deb.doMove", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_OUT, &(PEv2_data->PEv2_deb_RefSpeed[j]), comp_id,
			"%s.PEv2.%01d.deb.RefSpeed", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_OUT, &(PEv2_data->PEv2_deb_RefPos[j]), comp_id,
			"%s.PEv2.%01d.deb.RefPos", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_OUT, &(PEv2_data->PEv2_deb_RefPosSpeed[j]), comp_id,
			"%s.PEv2.%01d.deb.RefPosSpeed", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_AxesState[j]), comp_id,
			"%s.PEv2.%01d.AxesState", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_IN, &(PEv2_data->PEv2_AxesCommand[j]), comp_id,
			"%s.PEv2.%01d.AxesCommand", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_AxesConfig[j]), comp_id,
			"%s.PEv2.%01d.AxesConfig", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_SoftLimitMaximum[j]), comp_id,
			"%s.PEv2.%01d.SoftLimitMaximum", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_SoftLimitMinimum[j]), comp_id,
			"%s.hal_param_u32_newf.%01d.SoftLimitMinimum", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_HomingSpeed[j]), comp_id,
			"%s.PEv2.%01d.HomingSpeed", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_HomingReturnSpeed[j]), comp_id,
			"%s.PEv2.%01d.HomingReturnSpeed", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_HomeOffsets[j]), comp_id,
			"%s.PEv2.%01d.HomeOffsets", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_ProbePosition[j]), comp_id,
			"%s.PEv2.%01d.ProbePosition", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_ProbeMaxPosition[j]), comp_id,
			"%s.PEv2.%01d.ProbeMaxPosition", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_s32_newf(HAL_OUT, &(PEv2_data->PEv2_CurrentPosition[j]), comp_id,
			"%s.PEv2.%01d.CurrentPosition", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_s32_newf(HAL_IO, &(PEv2_data->PEv2_PositionSetup[j]), comp_id,
			"%s.PEv2.%01d.PositionSetup", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_ReferencePositionSpeed[j]), comp_id,
			"%s.PEv2.%01d.ReferencePositionSpeed", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_ReferencePosition[j]), comp_id,
		"%s.PEv2.%01d.ReferencePosition", prefix, j);
		if (r != 0)
			return r;

			r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_ReferenceSpeed[j]), comp_id,
			"%s.PEv2.%01d.ReferenceSpeed", prefix, j);
			if (r != 0)
				return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_MaxSpeed[j]), comp_id,
			"%s.PEv2.%01d.MaxSpeed", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_MaxAcceleration[j]), comp_id,
			"%s.PEv2.%01d.MaxAcceleration", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_MaxDecceleration[j]), comp_id,
			"%s.PEv2.%01d.MaxDecceleration", prefix, j);
		if (r != 0)
			return r;

		//PEv2_HomePosition
		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_HomePosition[j]), comp_id,
			"%s.PEv2.%01d.HomePosition", prefix, j);
		if (r != 0)
			return r;

		//PEv2_ArmPosition
		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_ArmPosition[j]), comp_id,
			"%s.PEv2.%01d.ArmPosition", prefix, j);
		if (r != 0)
			return r;

		//PEv2_ZeroPosition
		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_ZeroPosition[j]), comp_id,
			"%s.PEv2.%01d.ZeroPosition", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_PositionScale[j]), comp_id,
			"%s.PEv2.%01d.PositionScale", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_PositionOffset[j]), comp_id,
			"%s.PEv2.%01d.PositionOffset", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_IN, &(PEv2_data->PEv2_joint_vel_cmd[j]), comp_id,
			"%s.PEv2.%01d.joint-vel-cmd", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_IN, &(PEv2_data->PEv2_joint_pos_cmd[j]), comp_id,
			"%s.PEv2.%01d.joint-pos-cmd", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_OUT, &(PEv2_data->PEv2_joint_pos_fb[j]), comp_id,
			"%s.PEv2.%01d.joint-pos-fb", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_joint_out_home[j]), comp_id,
			"%s.PEv2.%01d.joint-out-home", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_joint_in_position[j]), comp_id,
			"%s.PEv2.%01d.joint-in-position", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_joint_kb_jog_active[j]), comp_id,
			"%s.PEv2.%01d.joint-kb-jog-active", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_joint_wheel_jog_active[j]), comp_id,
			"%s.PEv2.%01d.joint-wheel-jog-active", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_s32_newf(HAL_IN, &(PEv2_data->PEv2_stepgen_TYPE[j]), comp_id,
			"%s.PEv2.%01d.stepgen.TYPE", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_HOME[j]), comp_id,
			"%s.PEv2.%01d.stepgen.HOME", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[j]), comp_id,
			"%s.PEv2.%01d.stepgen.STEPGEN-MAXVEL", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_STEPGEN_MAXACCEL[j]), comp_id,
			"%s.PEv2.%01d.stepgen.STEPGEN-MAXACCEL", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_IN, &(PEv2_data->PEv2_params_Feedback_Encoder_Id[j]), comp_id,
			"%s.PEv2.%01d.params.Feedback-Encoder-Id", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_IN, &(PEv2_data->PEv2_stepgen_DEADBAND[j]), comp_id,
			"%s.PEv2.%01d.stepgen.DEADBAND", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_IN, &(PEv2_data->PEv2_stepgen_MAX_OUTPUT[j]), comp_id,
			"%s.PEv2.%01d.stepgen.MAX-OUTPUT", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_IN, &(PEv2_data->PEv2_stepgen_ENCODER_SCALE[j]), comp_id,
			"%s.PEv2.%01d.stepgen.ENCODER-SCALE", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_STEP_SCALE[j]), comp_id,
			"%s.PEv2.%01d.stepgen.STEP-SCALE", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_MIN_LIMIT[j]), comp_id,
			"%s.PEv2.%01d.stepgen.MIN-LIMIT", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_MAX_LIMIT[j]), comp_id,
			"%s.PEv2.%01d.stepgen.MAX-LIMIT", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_HOME_OFFSET[j]), comp_id,
			"%s.PEv2.%01d.stepgen.HOME-OFFSET", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_HOME_SEARCH_VEL[j]), comp_id,
			"%s.PEv2.%01d.stepgen.HOME-SEARCH-VEL", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_float_newf(HAL_RW, &(PEv2_data->PEv2_stepgen_HOME_LATCH_VEL[j]), comp_id,
			"%s.PEv2.%01d.stepgen.HOME-LATCH-VEL", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_float_newf(HAL_IN, &(PEv2_data->PEv2_stepgen_HOME_FINAL_VEL[j]), comp_id,
			"%s.PEv2.%01d.stepgen.HOME-FINAL-VEL", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_s32_newf(HAL_IN, &(PEv2_data->PEv2_stepgen_HOME_IGNORE_LIMITS[j]), comp_id,
			"%s.PEv2.%01d.stepgen.HOME-IGNORE-LIMITS", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_MPGjogMultiplier[j]), comp_id,
			"%s.PEv2.%01d.MPGjogMultiplier", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_MPGjogEncoder[j]), comp_id,
			"%s.PEv2.%01d.MPGjogEncoder", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_MPGjogDivider[j]), comp_id,
			"%s.PEv2.%01d.MPGjogDivider", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_LimitN_in[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitN.in", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_LimitN_in_not[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitN.in-not", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_LimitN_DedicatedInput[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitN.DedicatedInput", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_LimitP_in[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitP.in", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_LimitP_in_not[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitP.in-not", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_LimitP_DedicatedInput[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitP.DedicatedInput", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Home_in[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.in", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Home_in_not[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.in-not", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Home_DedicatedInput[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.DedicatedInput", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_digout_AxisEnable_out[j]), comp_id,
			"%s.PEv2.%01d.digout.AxisEnable.out", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_HomeBackOffDistance[j]), comp_id,
			"%s.PEv2.%01d.HomeBackOffDistance", prefix, j);
		if (r != 0)
			return r;
	}

	/*
	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_PulseEngineEnabled), comp_id,
		"%s.PEv2.PulseEngineEnabled", prefix);
	if (r != 0)
		return r;*/

	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_PulseEngineEnabled), comp_id,
		"%s.PEv2.PulseEngineEnabled", prefix);
	if (r != 0)
		return r;

	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_PulseGeneratorType), comp_id,
		"%s.PEv2.PulseGeneratorType", prefix);
	if (r != 0)
		return r;
	r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_PG_swap_stepdir), comp_id,
		"%s.PEv2.PG-swap-stepdir", prefix);
	if (r != 0)
		return r;
	r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_PG_extended_io), comp_id,
		"%s.PEv2.PG-extended-io", prefix);
	if (r != 0)
		return r;
	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_ChargePumpEnabled), comp_id,
		"%s.PEv2.ChargePumpEnabled", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_PulseEngineActivated), comp_id,
		"%s.PEv2.PulseEngineActivated", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_PulseEngineState), comp_id,
		"%s.PEv2.PulseEngineState", prefix);
	if (r != 0)
		return r;

	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_MiscInputStatus), comp_id,
		"%s.PEv2.MiscInputStatus", prefix);
	if (r != 0)
		return r;

	for (j = 0; j < (8); j++) {
		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Error_in[j]), comp_id,
			"%s.PEv2.%01d.digin.Error.in", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Error_in_not[j]), comp_id,
			"%s.PEv2.%01d.digin.Error.in-not", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Misc_in[j]), comp_id,
			"%s.PEv2.digin.Misc-%01d.in", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Misc_in_not[j]), comp_id,
			"%s.PEv2.digin.Misc-%01d.in-not", prefix, j);
		if (r != 0)
			return r;
	}
	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_LimitOverride), comp_id,
		"%s.PEv2.LimitOverride", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_LimitOverrideSetup), comp_id,
		"%s.PEv2.LimitOverrideSetup", prefix);
	if (r != 0)
		return r;
	r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Probed_in), comp_id,
		"%s.PEv2.digin.Probed.in", prefix);
	if (r != 0)
		return r;
	for (j = 0; j < (8); j++) {
		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Probe_in[j]), comp_id,
			"%s.PEv2.%01d.digin.Probe.in", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Probe_in_not[j]), comp_id,
			"%s.PEv2.%01d.digin.Probe.in-not", prefix, j);
		if (r != 0)
			return r;
	}
	r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Emergency_in), comp_id,
		"%s.PEv2.digin.Emergency.in", prefix);
	if (r != 0)
		return r;
	r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_Emergency_in_not), comp_id,
		"%s.PEv2.digin.Emergency.in-not", prefix);
	if (r != 0)
		return r;
	r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_digout_Emergency_out), comp_id,
		"%s.PEv2.digout.Emergency.out", prefix);
	if (r != 0)
		return r;
	for (j = 0; j < (8); j++) {
		r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_digin_SoftLimit_in[j]), comp_id,
			"%s.PEv2.%01d.digin.SoftLimit.in", prefix, j);
		if (r != 0)
			return r;
	}
	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_AxisEnabledMask), comp_id,
		"%s.PEv2.AxisEnabledMask", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_AxisEnabledStatesMask), comp_id,
		"%s.PEv2.AxisEnabledStatesMask", prefix);
	if (r != 0)
		return r;
	for (j = 0; j < (8); j++) {
		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_digout_AxisEnabled_out[j]), comp_id,
			"%s.PEv2.%01d.digout.AxisEnabled.out", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digin_AxisEnabled_in[j]), comp_id,
			"%s.PEv2.%01d.digin.AxisEnabled.in", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_OUT, &(PEv2_data->PEv2_digout_LimitOverride_out[j]), comp_id,
			"%s.PEv2.%01d.digout.LimitOverride.out", prefix, j);
		if (r != 0)
			return r;
	}
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_ExternalRelayOutputs), comp_id,
		"%s.PEv2.ExternalRelayOutputs", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_ExternalOCOutputs), comp_id,
		"%s.PEv2.ExternalOCOutputs", prefix);
	if (r != 0)
		return r;

	for (j = 0; j < (4); j++) {
		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_digout_ExternalRelay_out[j]), comp_id,
			"%s.PEv2.digout.ExternalRelay-%01d.out", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_bit_newf(HAL_IN, &(PEv2_data->PEv2_digout_ExternalOC_out[j]), comp_id,
			"%s.PEv2.digout.ExternalOC-%01d.out", prefix, j);
		if (r != 0)
			return r;
	}

	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_HomingStartMaskSetup), comp_id,
		"%s.PEv2.HomingStartMaskSetup", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_ProbeStartMaskSetup), comp_id,
		"%s.PEv2.ProbeStartMaskSetup", prefix);
	if (r != 0)
		return r;
	r = hal_pin_u32_newf(HAL_OUT, &(PEv2_data->PEv2_ProbeStatus), comp_id,
		"%s.PEv2.ProbeStatus", prefix);
	if (r != 0)
		return r;
	r = hal_pin_float_newf(HAL_IO, &(PEv2_data->PEv2_ProbeSpeed), comp_id,
		"%s.PEv2.ProbeSpeed", prefix);
	if (r != 0)
		return r;
	for (j = 0; j < (8); j++) {
		r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_BacklashWidth[j]), comp_id,
			"%s.PEv2.%01d.BacklashWidth", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_BacklashRegister[j]), comp_id,
			"%s.PEv2.%01d.BacklashRegister", prefix, j);
		if (r != 0)
			return r;

		r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_BacklashAcceleration[j]), comp_id,
			"%s.PEv2.%01d.BacklashAcceleration", prefix, j);
		if (r != 0)
			return r;
	}
	r = hal_pin_u32_newf(HAL_IO, &(PEv2_data->PEv2_BacklashCompensationEnabled), comp_id,
		"%s.PEv2.BacklashCompensationEnabled", prefix);
	if (r != 0)
		return r;


	for (j = 0; j < (8); j++) {
		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_home_sequence[j]), comp_id,
			"%s.PEv2.%01d.home-sequence", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_AxisEnabled[j]), comp_id,
			"%s.PEv2.%01d.AxisEnabled", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_AxisInverted[j]), comp_id,
			"%s.PEv2.%01d.AxisInverted", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_AxisInternalPlanner[j]), comp_id,
			"%s.PEv2.%01d.AxisInternalPlanner", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_AxisPositionMode[j]), comp_id,
			"%s.PEv2.%01d.AxisPositionMode", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_AxisInvertedHome[j]), comp_id,
			"%s.PEv2.%01d.AxisInvertedHome", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_AxisSoftLimitEnabled[j]), comp_id,
			"%s.PEv2.%01d.AxisSoftLimitEnabled", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_s32_newf(HAL_RW, &(PEv2_data->PEv2_AxisEnabledMasked[j]), comp_id,
			"%s.PEv2.%01d.AxisEnabledMasked", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_SoftLimit_PosMin[j]), comp_id,
			"%s.PEv2.%01d.digin.SoftLimit.PosMin", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_SoftLimit_PosMax[j]), comp_id,
			"%s.PEv2.%01d.digin.SoftLimit.PosMax", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_HomingAlgorithm[j]), comp_id,
			"%s.PEv2.%01d.HomingAlgorithm", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OnHome_Stop[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OnHome.Stop", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OnHome_ArmEncoder[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OnHome.ArmEncoder", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OnHome_RevDirection[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OnHome.RevDirection", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OnHome_ReducedSpeed[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OnHome.ReducedSpeed", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OutHome_Stop[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OutHome.Stop", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OutHome_ArmEncoder[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OutHome.ArmEncoder", prefix, j);
		if (r != 0)
			return r;
		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OutHome_RevDirection[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OutHome.RevDirection", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_HomeAlg_OutHome_ReducedSpeed[j]), comp_id,
			"%s.PEv2.%01d.HomeAlg.OutHome.ReducedSpeed", prefix, j);
		if (r != 0)
			return r;
		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Home_Offset[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.Offset", prefix, j);
		if (r != 0)
			return r;
		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_AxesSwitchConfig[j]), comp_id,
			"%s.PEv2.%01d.AxesSwitchConfig", prefix, j);
		if (r != 0)
			return r;
		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitN_Enabled[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitN.Enabled", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitP_Enabled[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitP.Enabled", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_Home_Enabled[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.Enabled", prefix, j);
		if (r != 0)
			return r;
		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_Home_OnLimitN[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.OnLimitN", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_Home_OnLimitP[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.OnLimitP", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitN_invert[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitN.invert", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitP_invert[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitP.invert", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digin_Home_invert[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.invert", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitN_Pin[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitN.Pin", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitN_Filter[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitN.Filter", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitP_Pin[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitP.Pin", prefix, j);
		if (r != 0)
			return r;

		//PEv2_digin_Probe_Filter
		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Probe_Filter[j]), comp_id,
			"%s.PEv2.%01d.digin.Probe.Filter", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_LimitP_Filter[j]), comp_id,
			"%s.PEv2.%01d.digin.LimitP.Filter", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Home_Pin[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.Pin", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Home_Filter[j]), comp_id,
			"%s.PEv2.%01d.digin.Home.Filter", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digout_AxisEnable_Pin[j]), comp_id,
			"%s.PEv2.%01d.digout.AxisEnable.Pin", prefix, j);
		if (r != 0)
			return r;

		r = hal_param_bit_newf(HAL_RW, &(PEv2_data->PEv2_digout_AxisEnable_invert[j]), comp_id,
			"%s.PEv2.%01d.digout.AxisEnable.invert", prefix, j);
		if (r != 0)
			return r;
	}
	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Emergency_Pin), comp_id,
		"%s.PEv2.digin.Emergency.Pin", prefix);
	if (r != 0)
		return r;
	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Emergency_invert), comp_id,
		"%s.PEv2.digin.Emergency.invert", prefix);
	if (r != 0)
		return r;
	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digout_Emergency_Pin), comp_id,
		"%s.PEv2.digout.Emergency.Pin", prefix);
	if (r != 0)
		return r;
	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Probe_Pin), comp_id,
		"%s.PEv2.digin.Probe.Pin", prefix);
	if (r != 0)
		return r;
	r = hal_param_u32_newf(HAL_RW, &(PEv2_data->PEv2_digin_Probe_invert), comp_id,
		"%s.PEv2.digin.Probe.invert", prefix);
	if (r != 0)
		return r;

}
