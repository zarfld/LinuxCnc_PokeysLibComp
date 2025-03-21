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

bool Get_BitOfByte(uint8_t in_Byte, int Bit_Id) {
	return (in_Byte >> Bit_Id) & 0x01;
}

uint8_t Set_BitOfByte(uint8_t in_Byte, int Bit_Id, bool value) {

	if (value == true) {
		in_Byte |= 1 << Bit_Id;
	}
	else {
		in_Byte &= ~(1 << Bit_Id);
	}
	return in_Byte;
}

int32_t PEv2_StatusGet(sPoKeysDevice* dev) {
	uint8_t bm_LimitStatusP; // Limit+ status (bit-mapped)
	uint8_t bm_LimitStatusN; // Limit- status (bit-mapped)
	uint8_t bm_HomeStatus;	 // Home status (bit-mapped)
	uint8_t bm_ErrorStatus;
	uint8_t bm_ProbeStatus = dev->PEv2.ProbeStatus; // will be update in "PK_PEv2_ProbingFinish" or "PK_PEv2_ProbingFinishSimple"
	int32_t ret = PK_OK;
	ret = PK_PEv2_StatusGet(dev);

	if (ret == PK_OK) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_StatusGet(dev) = PK_OK\n", __FILE__, __FUNCTION__);
		// Engine info
		PEv2_nrOfAxes = dev->PEv2.info.nrOfAxes;
		PEv2_maxPulseFrequency = dev->PEv2.info.maxPulseFrequency;
		PEv2_bufferDepth = dev->PEv2.info.bufferDepth;
		PEv2_slotTiming = dev->PEv2.info.slotTiming;
		*PEv2_data->PEv2_AxisEnabledStatesMask = dev->PEv2.AxisEnabledStatesMask;
		PEv2_LimitOverride = dev->PEv2.LimitOverride;

		// Basic engine states
		PEv2_data->PEv2_PulseEngineEnabled = dev->PEv2.PulseEngineEnabled;
		*PEv2_data->PEv2_PulseEngineActivated = dev->PEv2.PulseEngineActivated;

		PulseEngineState = dev->PEv2.PulseEngineState;
		//
		PEv2_PulseEngineStateSetup = PulseEngineState;
		PEv2_data->PEv2_ChargePumpEnabled = dev->PEv2.ChargePumpEnabled;
		PEv2_data->PEv2_PulseGeneratorType = dev->PEv2.PulseGeneratorType;

		// Switch states
		bm_LimitStatusP = dev->PEv2.LimitStatusP; // Limit+ status (bit-mapped)
		bm_LimitStatusN = dev->PEv2.LimitStatusN; // Limit- status (bit-mapped)
		bm_HomeStatus = dev->PEv2.HomeStatus;	  // Home status (bit-mapped)
		if (ApplyIniSettings == false) {
			PEv2_data->PEv2_digin_Emergency_invert = dev->PEv2.EmergencySwitchPolarity;
		}

		// Other inputs
		bm_ErrorStatus = dev->PEv2.ErrorInputStatus;
		PEv2_MiscInputStatus = dev->PEv2.MiscInputStatus;

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: HAL_Machine_On = %s\n", __FILE__, __FUNCTION__, HAL_Machine_On);

		if (HAL_Machine_On == 0) {
			PEv2_PulseEngineStateSetup = PK_PEState_peSTOPPED;
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_PulseEngineStateSetup = PK_PEState_peSTOPPED\n", __FILE__, __FUNCTION__);
		}
		else if (PulseEngineState == PK_PEState_peSTOPPED) {
			PEv2_PulseEngineStateSetup = PK_PEState_peRUNNING;
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_PulseEngineStateSetup = PK_PEState_peRUNNING\n", __FILE__, __FUNCTION__);
		}
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PulseEngineState = %d\n", __FILE__, __FUNCTION__, PulseEngineState);
		switch (PulseEngineState) {
		case PK_PEState_peSTOPPED: // Pulse engine is stopped
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peSTOPPED\n", __FILE__, __FUNCTION__);
			if (HAL_Machine_On != 0) {
				PEv2_PulseEngineStateSetup = PK_PEState_peRUNNING;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_PulseEngineStateSetup = PK_PEState_peRUNNING\n", __FILE__, __FUNCTION__);
			}

			break;
		case PK_PEState_peINTERNAL: // PEv1: Internal motion controller is in use, PEv2: not used
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peINTERNAL\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peBUFFER: // PEv1: Buffered operation mode is in use, PEv2: not used
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peBUFFER\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peRUNNING: // Pulse engine is activated
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peRUNNING\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peJOGGING: // Jogging mode enabled
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peJOGGING\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peSTOPPING: // Pulse engine is stopping
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peSTOPPING\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peHOME: // All axes are homed
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peHOME\n", __FILE__, __FUNCTION__);
			if (Homing_active) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_HomingFinish(dev) \n", __FILE__, __FUNCTION__);
				//	PK_PEv2_HomingFinish(dev);
				Homing_active = false;
			}
			break;
		case PK_PEState_peHOMING: // Axes homing is in progress
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peHOMING\n", __FILE__, __FUNCTION__);
			Homing_active = true;
			break;
		case PK_PEState_pePROBECOMPLETE: // All axes are homed
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_pePROBECOMPLETE\n", __FILE__, __FUNCTION__);

			break;
		case PK_PEState_pePROBE: // Axes probing is in progress
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_pePROBE\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_pePROBEERROR: // Error occured during probing
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_pePROBEERROR\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peHYBRIDPROBE_STOPPING:
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peHYBRIDPROBE_STOPPING\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peHYBRIDPROBE_COMPLETE:
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peHYBRIDPROBE_COMPLETE\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peSTOP_LIMIT: // Pulse engine stopped due to limit reached
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEState_peSTOP_LIMIT\n", __FILE__, __FUNCTION__);
			break;
		case PK_PEState_peSTOP_EMERGENCY: // Pulse engine stopped due to emergency switch
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEState_peSTOP_EMERGENCY\n", __FILE__, __FUNCTION__);
			PEv2_digin_Emergency_in = true;
			PEv2_digin_Emergency_in_not = false;
			PEv2_deb_estop = 8;
			break;
		}

	}
	else {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_StatusGet(dev) != PK_OK (%i)\n", __FILE__, __FUNCTION__, ret);
	}
	usleep(sleepdur);
	return ret;
}

int32_t PEv2_Status2Get(sPoKeysDevice* dev) {
	uint8_t bm_DedicatedLimitNInputs;
	uint8_t bm_DedicatedLimitPInputs;
	uint8_t bm_DedicatedHomeInputs;
	int32_t ret = PK_PEv2_Status2Get(dev);
	if (ret == PK_OK) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_Status2Get(dev) = PK_OK\n", __FILE__, __FUNCTION__);
		bm_DedicatedLimitNInputs = dev->PEv2.DedicatedLimitNInputs;
		bm_DedicatedLimitPInputs = dev->PEv2.DedicatedLimitPInputs;
		bm_DedicatedHomeInputs = dev->PEv2.DedicatedHomeInputs;

		for (int i = 0; i < PEv2_nrOfAxes; i++) {

			if (PEv2_data->PEv2_digin_LimitN_Pin[i] > 0) {
				PEv2_digin_LimitN_DedicatedInput(i) = Get_BitOfByte(bm_DedicatedLimitNInputs, i);
			}
			else if (PEv2_digin_LimitN_Enabled(i) != 0) {
				if (PEv2_data->PEv2_digin_LimitN_invert[i] != 0) {
					PEv2_digin_LimitN_in(i) = !Get_BitOfByte(bm_DedicatedLimitNInputs, i);
					PEv2_digin_LimitN_in_not(i) = Get_BitOfByte(bm_DedicatedLimitNInputs, i);
					PEv2_digin_LimitN_DedicatedInput(i) = !Get_BitOfByte(bm_DedicatedLimitNInputs, i);
				}
				else {
					PEv2_digin_LimitN_in(i) = Get_BitOfByte(bm_DedicatedLimitNInputs, i);
					PEv2_digin_LimitN_in_not(i) = !Get_BitOfByte(bm_DedicatedLimitNInputs, i);
					PEv2_digin_LimitN_DedicatedInput(i) = Get_BitOfByte(bm_DedicatedLimitNInputs, i);
				}
			}

			if (PEv2_data->PEv2_digin_LimitP_Pin[i] > 0) {
				PEv2_digin_LimitP_DedicatedInput(i) = Get_BitOfByte(bm_DedicatedLimitPInputs, i);
			}
			else if (PEv2_digin_LimitP_Enabled(i) != 0) {
				if (PEv2_data->PEv2_digin_LimitP_invert[i] != 0) {
					PEv2_digin_LimitP_in(i) = !Get_BitOfByte(bm_DedicatedLimitPInputs, i);
					PEv2_digin_LimitP_in_not(i) = Get_BitOfByte(bm_DedicatedLimitPInputs, i);
					PEv2_digin_LimitP_DedicatedInput(i) = !Get_BitOfByte(bm_DedicatedLimitPInputs, i);
				}
				else {
					PEv2_digin_LimitP_in(i) = Get_BitOfByte(bm_DedicatedLimitPInputs, i);
					PEv2_digin_LimitP_in_not(i) = !Get_BitOfByte(bm_DedicatedLimitPInputs, i);
					PEv2_digin_LimitP_DedicatedInput(i) = Get_BitOfByte(bm_DedicatedLimitPInputs, i);
				}
			}

			if (PEv2_data->PEv2_digin_Home_Pin[i] > 0) {
				PEv2_digin_Home_DedicatedInput(i) = Get_BitOfByte(bm_DedicatedHomeInputs, i);
			}
			else if (PEv2_data->PEv2_digin_Home_Enabled[i] != 0) {
				if (PEv2_data->PEv2_digin_Home_invert[i] != 0) {
					PEv2_digin_Home_in(i) = !Get_BitOfByte(bm_DedicatedHomeInputs, i);
					PEv2_digin_Home_in_not(i) = Get_BitOfByte(bm_DedicatedHomeInputs, i);
				}
				else {
					PEv2_digin_Home_in(i) = Get_BitOfByte(bm_DedicatedHomeInputs, i);
					PEv2_digin_Home_in_not(i) = !Get_BitOfByte(bm_DedicatedHomeInputs, i);
				}
			}
		}
	}
	else {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_Status2Get(dev) != PK_OK\n", __FILE__, __FUNCTION__);
	}
	usleep(sleepdur);
	return ret;
}

int32_t PEv2_ExternalOutputsSet(sPoKeysDevice* dev) {
	int32_t ret = PK_OK;
	if (PEv2_data->PEv2_PG_extended_io != 0) {
		ret = PK_PEv2_ExternalOutputsGet(dev);
		if (ret == PK_OK) {
			PEv2_ExternalRelayOutputs = dev->PEv2.ExternalRelayOutputs;
			PEv2_ExternalOCOutputs = dev->PEv2.ExternalOCOutputs;
			rtapi_print_msg(RTAPI_MSG_INFO, "PoKeys: %s:%s: PK_PEv2_ExternalOutputsGet==PK_OK\n", __FILE__, __FUNCTION__);
		}
		else {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_ExternalOutputsGet!=PK_OK\n", __FILE__, __FUNCTION__);
		}
		usleep(sleepdur);



		uint8_t ExternalRelayOutputs_set = 0;
		uint8_t ExternalOCOutputs_set = 0;

		bool DoExternalOutputsSet = false;
		/*strange behaviour: it seems that values on "" do the following:
		- bit 1 / int 1 -> switches ext_Relays1
		- bit 2 / int 2 -> switches ext_Relays3
		- bit 3 / int 4 -> switches ext_Relays2
		- bit 4 / int 8 -> switches ext_OC-Output1
		- bit 5 / int 16 -> switches ext_OC-Output2
		- bit 6 / int 32 -> switches ext_OC-Output3
		- bit 7 / int 64 -> switches ext_OC-Output4
		- bit 8 / int 128 -> switches switches ext_Relays0

		*/
		/*
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 0, PEv2_digout_ExternalRelay_out(0));
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 1, PEv2_digout_ExternalRelay_out(1));
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 2, PEv2_digout_ExternalRelay_out(2));
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 3, PEv2_digout_ExternalRelay_out(3));
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 4, PEv2_digout_ExternalRelay_out(4));
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 5, PEv2_digout_ExternalRelay_out(5));
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 6, PEv2_digout_ExternalRelay_out(6));
			ExternalRelayOutputs_set = Set_BitOfByte(ExternalRelayOutputs_set, 7, PEv2_digout_ExternalRelay_out(7));

			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 0, PEv2_digout_ExternalOC_out(0));
			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 1, PEv2_digout_ExternalOC_out(1));
			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 2, PEv2_digout_ExternalOC_out(2));
			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 3, PEv2_digout_ExternalOC_out(3));
			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 4, PEv2_digout_ExternalOC_out(4));
			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 5, PEv2_digout_ExternalOC_out(5));
			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 6, PEv2_digout_ExternalOC_out(6));
			ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 7, PEv2_digout_ExternalOC_out(7));
		*/

		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 0, PEv2_digout_ExternalRelay_out(1));
		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 1, PEv2_digout_ExternalRelay_out(3));
		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 2, PEv2_digout_ExternalRelay_out(2));
		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 3, PEv2_digout_ExternalOC_out(0));
		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 4, PEv2_digout_ExternalOC_out(1));
		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 5, PEv2_digout_ExternalOC_out(2));
		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 6, PEv2_digout_ExternalOC_out(3));
		ExternalOCOutputs_set = Set_BitOfByte(ExternalOCOutputs_set, 7, PEv2_digout_ExternalRelay_out(0));

		if (ExternalRelayOutputs_set != dev->PEv2.ExternalRelayOutputs) {
			dev->PEv2.ExternalRelayOutputs = ExternalRelayOutputs_set;
			DoExternalOutputsSet = true;
		}
		if (ExternalOCOutputs_set != dev->PEv2.ExternalOCOutputs) {
			dev->PEv2.ExternalOCOutputs = ExternalOCOutputs_set;
			DoExternalOutputsSet = true;
		}


		if (DoExternalOutputsSet) {
			dev->PEv2.ExternalRelayOutputs = ExternalRelayOutputs_set;
			dev->PEv2.ExternalOCOutputs = ExternalOCOutputs_set;
			PK_PEv2_ExternalOutputsSet(dev);
			usleep(sleepdur);

			dev->PEv2.ExternalRelayOutputs = ExternalRelayOutputs_set;
			dev->PEv2.ExternalOCOutputs = ExternalOCOutputs_set;
			PK_PEv2_ExternalOutputsSet(dev);
			usleep(sleepdur);
		}
	}

	return ret;
}

int32_t PEv2_PulseEngineSetup(sPoKeysDevice* dev) {
	bool doSetup = false;

	switch (dev->DeviceData.DeviceTypeID) {
	case PK_DeviceID_PoKeys57CNC:
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DeviceID_PoKeys57CNC\n", __FILE__, __FUNCTION__);
		PEv2_data->PEv2_PulseEngineEnabled = 8;
		PEv2_data->PEv2_PG_extended_io = 1;
		break;
	case PK_DeviceID_PoKeys57CNCdb25:
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DeviceID_PoKeys57CNCdb25\n", __FILE__, __FUNCTION__);
		PEv2_data->PEv2_PG_extended_io = 0;
		break;
	case PK_DeviceID_57U:
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DeviceID_57U\n", __FILE__, __FUNCTION__);
		PEv2_data->PEv2_PG_extended_io = 0;
		break;
	case PK_DeviceID_57E:
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DeviceID_57E\n", __FILE__, __FUNCTION__);
		PEv2_data->PEv2_PG_extended_io = 0;
		break;
	}



	if (dev->PEv2.PulseEngineEnabled != dev->PEv2.info.nrOfAxes) {
		dev->PEv2.PulseEngineEnabled = dev->PEv2.info.nrOfAxes;
		PEv2_data->PEv2_PulseEngineEnabled = dev->PEv2.info.nrOfAxes;
		doSetup = true;
	}

	//dev->PEv2.PulseEngineEnabled = PEv2_data->PEv2_PulseEngineEnabled;
	if (dev->PEv2.PulseEngineEnabled != PEv2_data->PEv2_PulseEngineEnabled) {
		dev->PEv2.PulseEngineEnabled = PEv2_data->PEv2_PulseEngineEnabled;
		doSetup = true;
	}
	//dev->PEv2.ChargePumpEnabled = PEv2_data->PEv2_ChargePumpEnabled;
	if (dev->PEv2.ChargePumpEnabled != PEv2_data->PEv2_ChargePumpEnabled) {
		dev->PEv2.ChargePumpEnabled = PEv2_data->PEv2_ChargePumpEnabled;
		doSetup = true;
	}
	//dev->PEv2.PulseGeneratorType = PEv2_data->PEv2_PulseGeneratorType;

	if (PEv2_data->PEv2_PG_swap_stepdir != false) {
		PEv2_data->PEv2_PulseGeneratorType = Set_BitOfByte(PEv2_data->PEv2_PulseGeneratorType, 6, true); // swap step / dir signals
	}
	else {
		PEv2_data->PEv2_PulseGeneratorType = Set_BitOfByte(PEv2_data->PEv2_PulseGeneratorType, 6, false); // swap step / dir signals
	}

	if (PEv2_data->PEv2_PG_extended_io != 0) {
		PEv2_data->PEv2_PulseGeneratorType = Set_BitOfByte(PEv2_data->PEv2_PulseGeneratorType, 7, true); // extended io
	}
	else {
		PEv2_data->PEv2_PulseGeneratorType = Set_BitOfByte(PEv2_data->PEv2_PulseGeneratorType, 7, false); // no extended io
	}


	if (dev->PEv2.PulseGeneratorType != PEv2_data->PEv2_PulseGeneratorType) {
		dev->PEv2.PulseGeneratorType = PEv2_data->PEv2_PulseGeneratorType;
		doSetup = true;
	}
	//dev->PEv2.EmergencySwitchPolarity = PEv2_data->PEv2_digin_Emergency_invert;
	if (dev->PEv2.EmergencySwitchPolarity != PEv2_data->PEv2_digin_Emergency_invert) {
		dev->PEv2.EmergencySwitchPolarity = PEv2_data->PEv2_digin_Emergency_invert;
		doSetup = true;
	}
	//dev->PEv2.AxisEnabledStatesMask = PEv2_data->PEv2_AxisEnabledStatesMask;
	if (dev->PEv2.AxisEnabledStatesMask != *PEv2_data->PEv2_AxisEnabledStatesMask) {
		dev->PEv2.AxisEnabledStatesMask = *PEv2_data->PEv2_AxisEnabledStatesMask;
		doSetup = true;
	}

	// currently n9ot in use
	// dev->PEv2.PulseEngineBufferSize;

	if (doSetup == true) {
		if (PK_PEv2_PulseEngineSetup(dev) != PK_OK) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_PulseEngineSetup() != PK_OK\n", __FILE__, __FUNCTION__);
			usleep(sleepdur);
			if (PK_PEv2_PulseEngineSetup(dev) != PK_OK) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_PulseEngineSetup() != PK_OK\n", __FILE__, __FUNCTION__);
			}
		}
		else {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_PulseEngineSetup() == PK_OK\n", __FILE__, __FUNCTION__);
			usleep(sleepdur);
			if (PK_SaveConfiguration(dev) == PK_OK) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_SaveConfiguration() == PK_OK\n", __FILE__, __FUNCTION__);
				usleep(sleepdur);
				//	PK_PEv2_PulseEngineReboot(dev);
				usleep(1000000);
			}
			else {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_SaveConfiguration() != PK_OK\n", __FILE__, __FUNCTION__);
			}

		}
		usleep(sleepdur);
	}


}

int32_t PEv2_AdditionalParametersGet(sPoKeysDevice* dev) {

	if (PK_PEv2_AdditionalParametersGet(dev) == PK_OK) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersGet() == PK_OK\n", __FILE__, __FUNCTION__);
		if (ApplyIniSettings == false || PEv2_data->PEv2_digin_Emergency_Pin == 0) {
			PEv2_data->PEv2_digin_Emergency_Pin = dev->PEv2.EmergencyInputPin - 9;
		}
	}
	else {

		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersGet() != PK_OK\n", __FILE__, __FUNCTION__);
	}
	usleep(sleepdur);
}

int32_t  PEv2_AdditionalParametersSet(sPoKeysDevice* dev) {
	bool doSetup = false;
	int32_t ret = PK_PEv2_AdditionalParametersGet(dev);
	if (ret == PK_OK) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersSet() == PK_OK\n", __FILE__, __FUNCTION__);

		// seems to apply here also defines the extenal input (values 1-8) or PoKeys pin (0-based Pin ID + 9)
		if (ApplyIniSettings == true && PEv2_data->PEv2_digin_Emergency_Pin != dev->PEv2.EmergencyInputPin + 9) {
			dev->PEv2.EmergencyInputPin = PEv2_data->PEv2_digin_Emergency_Pin + 9; //(0:dedicated, 1-8:external inputs, 9+ Pin ID-9)
			doSetup = true;

			if (PEv2_data->PEv2_digin_Emergency_Pin != 0) { // check if pin is parametrized in HAL
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: IO_data->Pin[%d].PinFunction = %d\n", __FILE__, __FUNCTION__, PEv2_data->PEv2_digin_Emergency_Pin - 1, IO_data->Pin[PEv2_data->PEv2_digin_Emergency_Pin - 1].PinFunction);
				if (IO_data->Pin[PEv2_data->PEv2_digin_Emergency_Pin - 1].PinFunction == 0) {
					IO_data->Pin[PEv2_data->PEv2_digin_Emergency_Pin - 1].PinFunction = PK_PinCap_digitalInput;
				}
				else if (IO_data->Pin[PEv2_data->PEv2_digin_Emergency_Pin - 1].PinFunction != PK_PinCap_digitalInput) {
					rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: Emergency input pin is not set as digital input\n", __FILE__, __FUNCTION__);
					IO_data->Pin[PEv2_data->PEv2_digin_Emergency_Pin - 1].PinFunction = PK_PinCap_digitalInput;
				}

			}
		}
	}
	else {

		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersSet() != PK_OK\n", __FILE__, __FUNCTION__);
	}
	usleep(sleepdur);

	if (doSetup == true) {
		ret = PK_PEv2_AdditionalParametersSet(dev);

		if (ret != PK_OK) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersSet() != PK_OK\n", __FILE__, __FUNCTION__);
			usleep(sleepdur);
			ret = PK_PEv2_AdditionalParametersSet(dev);
			if (ret != PK_OK) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersSet() != PK_OK\n", __FILE__, __FUNCTION__);
			}
		}
		else {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_AdditionalParametersSet() == PK_OK\n", __FILE__, __FUNCTION__);
			usleep(sleepdur);
			if (PK_SaveConfiguration(dev) == PK_OK) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_SaveConfiguration() == PK_OK\n", __FILE__, __FUNCTION__);
				usleep(sleepdur);
				//	PK_PEv2_PulseEngineReboot(dev);
				usleep(1000000);
			}
			else {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_SaveConfiguration() != PK_OK\n", __FILE__, __FUNCTION__);
			}
		}
		usleep(sleepdur);
	}
	return ret;
}

int32_t  PEv2_AxisConfigurationGet(sPoKeysDevice* dev, int AxisId) {
	dev->PEv2.param1 = AxisId;
	int32_t ret = PK_PEv2_AxisConfigurationGet(dev);
	uint8_t AxesConfig[8] = { 0 };
	uint8_t AxesSwitchConfig[8] = { 0 };

	if (ret == PK_OK) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_AxisConfigurationGet(%d) == PK_OK\n", __FILE__, __FUNCTION__, AxisId);

		if (ApplyIniSettings == false) {
			/* read parameters for Axis configuration - see ePK_PEv2_AxisConfig
			PK_AC_ENABLED            = (1 << 0),       // 1 Axis enabled
			PK_AC_INVERTED           = (1 << 1),       // 2 Axis inverted
			PK_AC_INTERNAL_PLANNER   = (1 << 2),       // 4 Axis uses internal motion planner
			PK_AC_POSITION_MODE      = (1 << 3),       // 8 Internal motion planner for this axis is in position mode
			PK_AC_INVERTED_HOME      = (1 << 4),       // 16 Axis homing direction is inverted
			PK_AC_SOFT_LIMIT_ENABLED = (1 << 5),       // 32 Use soft-limits for this axis
			PK_AC_ENABLED_MASKED     = (1 << 7)        // 128 Use output enable pin masking

			pin out unsigned PEv2.#.AxesConfig[8];				// Axis configuration - see ePK_PEv2_AxisConfig
			param rw s32 PEv2.#.AxisEnabled[8] "Axis enabled";
			param rw s32 PEv2.#.AxisInverted[8] "Axis inverted";
			param rw s32 PEv2.#.AxisInternalPlanner[8] "Axis uses internal motion planner";
			param rw s32 PEv2.#.AxisPositionMode[8] "Internal motion planner for this axis is in position mode";
			param rw s32 PEv2.#.AxisInvertedHome[8] "Axis homing direction is inverted";
			param rw s32 PEv2.#.AxisSoftLimitEnabled[8] "Use soft-limits for this axis";
			param rw s32 PEv2.#.AxisEnabledMasked[8] "Use output enable pin masking";

			PEv2_data->PEv2_AxisEnabled[AxisId]
			PEv2_data->PEv2_AxisInverted[AxisId]
			PEv2_data->PEv2_AxisInternalPlanner[AxisId]
			PEv2_data->PEv2_AxisPositionMode[AxisId]
			PEv2_data->PEv2_AxisInvertedHome[AxisId]
			PEv2_data->PEv2_AxisSoftLimitEnabled[AxisId]
			PEv2_data->PEv2_AxisEnabledMasked[AxisId]
			 */
			PEv2_data->PEv2_AxesConfig[AxisId] = dev->PEv2.AxesConfig[AxisId];
			uint8_t axisConfig = dev->PEv2.AxesConfig[AxisId];

			PEv2_data->PEv2_AxisEnabled[AxisId] = (int)((axisConfig & PK_AC_ENABLED) != 0);
			PEv2_data->PEv2_AxisInverted[AxisId] = (int)((axisConfig & PK_AC_INVERTED) != 0);
			PEv2_data->PEv2_AxisInternalPlanner[AxisId] = (int)((axisConfig & PK_AC_INTERNAL_PLANNER) != 0);
			PEv2_data->PEv2_AxisPositionMode[AxisId] = (int)((axisConfig & PK_AC_POSITION_MODE) != 0);
			PEv2_data->PEv2_AxisInvertedHome[AxisId] = (int)((axisConfig & PK_AC_INVERTED_HOME) != 0);
			PEv2_data->PEv2_AxisSoftLimitEnabled[AxisId] = (int)((axisConfig & PK_AC_SOFT_LIMIT_ENABLED) != 0);
			PEv2_data->PEv2_AxisEnabledMasked[AxisId] = (int)((axisConfig & PK_AC_ENABLED_MASKED) != 0);

			/* PEv2_AxesSwitchConfig
			PK_ASO_SWITCH_LIMIT_N: 1 << 0 = 1
			PK_ASO_SWITCH_LIMIT_P : 1 << 1 = 2
			PK_ASO_SWITCH_HOME : 1 << 2 = 4
			PK_ASO_SWITCH_COMBINED_LN_H : 1 << 3 = 8
			PK_ASO_SWITCH_COMBINED_LP_H : 1 << 4 = 16
			PK_ASO_SWITCH_INVERT_LIMIT_N : 1 << 5 = 32
			PK_ASO_SWITCH_INVERT_LIMIT_P : 1 << 6 = 64
			PK_ASO_SWITCH_INVERT_HOME : 1 << 7 = 128

			param rw unsigned PEv2.#.AxesSwitchConfig[8];		// Axis switch configuration - see ePK_PulseEngineV2_AxisSwitchOptions
			param rw bit PEv2.#.digin.LimitN.Enabled[8] "Limit- is available (PK_ASO_SWITCH_LIMIT_N)";
			param rw bit PEv2.#.digin.LimitP.Enabled[8] "Limit+ is available (PK_ASO_SWITCH_LIMIT_P)";
			param rw bit PEv2.#.digin.Home.Enabled[8] "Invert home-switch (PK_ASO_SWITCH_HOME)";
			param rw bit PEv2.#.digin.Home.OnLimitN[8] "Shared with Limit- (PK_ASO_SWITCH_COMBINED_LN_H)";
			param rw bit PEv2.#.digin.Home.OnLimitP[8] "Shared with Limit+ (PK_ASO_SWITCH_COMBINED_LP_H)";
			param rw bit PEv2.#.digin.LimitN.invert[8] "Invert limit- (PK_ASO_SWITCH_INVERT_LIMIT_N)";
			param rw bit PEv2.#.digin.LimitP.invert[8] "Invert limit+ (PK_ASO_SWITCH_INVERT_LIMIT_P)";
			param rw bit PEv2.#.digin.Home.invert[8] "Invert home-switch (PK_ASO_SWITCH_INVERT_HOME)";
			*/

			PEv2_data->PEv2_AxesSwitchConfig[AxisId] = dev->PEv2.AxesSwitchConfig[AxisId];

			uint8_t axisSwitchConfig = dev->PEv2.AxesSwitchConfig[AxisId];
			PEv2_data->PEv2_digin_LimitN_Enabled[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_LIMIT_N) != 0);
			PEv2_data->PEv2_digin_LimitP_Enabled[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_LIMIT_P) != 0);
			PEv2_data->PEv2_digin_Home_Enabled[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_HOME) != 0);
			PEv2_data->PEv2_digin_Home_OnLimitN[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_COMBINED_LN_H) != 0);
			PEv2_data->PEv2_digin_Home_OnLimitP[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_COMBINED_LP_H) != 0);
			PEv2_data->PEv2_digin_LimitN_invert[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_INVERT_LIMIT_N) != 0);
			PEv2_data->PEv2_digin_LimitP_invert[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_INVERT_LIMIT_P) != 0);
			PEv2_data->PEv2_digin_Home_invert[AxisId] = (int)((axisSwitchConfig & PK_ASO_SWITCH_INVERT_HOME) != 0);
		}
		if (ApplyIniSettings == false) {
			PEv2_data->PEv2_digin_Home_Pin[AxisId] = dev->PEv2.PinHomeSwitch[AxisId];
		}
		if (ApplyIniSettings == false) {
			PEv2_data->PEv2_digin_LimitN_Pin[AxisId] = dev->PEv2.PinLimitMSwitch[AxisId];
		}
		if (ApplyIniSettings == false) {
			PEv2_data->PEv2_digin_LimitP_Pin[AxisId] = dev->PEv2.PinLimitPSwitch[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_HomingSpeed[AxisId] == 0) {
			PEv2_data->PEv2_HomingSpeed[AxisId] = dev->PEv2.HomingSpeed[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_HomingReturnSpeed[AxisId] == 0) {
			PEv2_data->PEv2_HomingReturnSpeed[AxisId] = dev->PEv2.HomingReturnSpeed[AxisId];
		}

		if (ApplyIniSettings == false || PEv2_data->PEv2_MPGjogEncoder[AxisId] == 0) {
			PEv2_data->PEv2_MPGjogEncoder[AxisId] = dev->PEv2.MPGjogEncoder[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_MaxSpeed[AxisId] == 0) {
			PEv2_data->PEv2_MaxSpeed[AxisId] = dev->PEv2.MaxSpeed[AxisId] * 1000;
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_MaxAcceleration[AxisId] == 0) {
			PEv2_data->PEv2_MaxAcceleration[AxisId] = dev->PEv2.MaxAcceleration[AxisId] * 1000000;
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_MaxDecceleration[AxisId] == 0) {
			PEv2_data->PEv2_MaxDecceleration[AxisId] = dev->PEv2.MaxDecceleration[AxisId] * 1000000;
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_SoftLimitMinimum[AxisId] == 0) {
			PEv2_data->PEv2_SoftLimitMinimum[AxisId] = dev->PEv2.SoftLimitMinimum[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_SoftLimitMaximum[AxisId] == 0) {
			PEv2_data->PEv2_SoftLimitMaximum[AxisId] = dev->PEv2.SoftLimitMaximum[AxisId];
		}
		//PEv2_data->PEv2_HomePosition[j]


		if (ApplyIniSettings == false || PEv2_data->PEv2_MPGjogEncoder[AxisId] == 0) {
			PEv2_data->PEv2_MPGjogEncoder[AxisId] = dev->PEv2.MPGjogEncoder[AxisId];
		}

		if (PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId] != 0) {

			if (ApplyIniSettings == false || PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId] == 0) {
				PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId] = PEv2_data->PEv2_MaxSpeed[AxisId] / PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId];
			}

			if (ApplyIniSettings == false || PEv2_data->PEv2_stepgen_STEPGEN_MAXACCEL[AxisId] == 0) {
				PEv2_data->PEv2_stepgen_STEPGEN_MAXACCEL[AxisId] = PEv2_data->PEv2_MaxAcceleration[AxisId] / PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId];
			}

			if (ApplyIniSettings == false || PEv2_data->PEv2_stepgen_HOME_OFFSET[AxisId] == 0) {
				PEv2_data->PEv2_stepgen_HOME_OFFSET[AxisId] = PEv2_data->PEv2_digin_Home_Offset[AxisId] / PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId];
			}


			if (ApplyIniSettings == false ) {
				PEv2_data->PEv2_stepgen_HOME_SEARCH_VEL[AxisId] = PEv2_data->PEv2_HomingSpeed[AxisId] * PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId] / 100;
			}


			if (ApplyIniSettings == false ) {
				PEv2_data->PEv2_stepgen_HOME_LATCH_VEL[AxisId] = PEv2_data->PEv2_HomingReturnSpeed[AxisId] * PEv2_data->PEv2_stepgen_HOME_SEARCH_VEL[AxisId] / 100;
			}

		}

		if (ApplyIniSettings == false || PEv2_data->PEv2_MPGjogMultiplier[AxisId] == 0) {
			PEv2_data->PEv2_MPGjogMultiplier[AxisId] = dev->PEv2.MPGjogMultiplier[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] == 0) {
			PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] = dev->PEv2.AxisEnableOutputPins[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_digout_AxisEnable_invert[AxisId] == 0) {
			PEv2_data->PEv2_digout_AxisEnable_invert[AxisId] = dev->PEv2.InvertAxisEnable[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_digin_LimitN_Filter[AxisId] == 0) {
			PEv2_data->PEv2_digin_LimitN_Filter[AxisId] = dev->PEv2.FilterLimitMSwitch[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_digin_LimitP_Filter[AxisId] == 0) {
			PEv2_data->PEv2_digin_LimitP_Filter[AxisId] = dev->PEv2.FilterLimitPSwitch[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_digin_Home_Filter[AxisId] == 0) {
			PEv2_data->PEv2_digin_Home_Filter[AxisId] = dev->PEv2.FilterHomeSwitch[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_HomingAlgorithm[AxisId] == 0) {
			PEv2_data->PEv2_HomingAlgorithm[AxisId] = dev->PEv2.HomingAlgorithm[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_HomeBackOffDistance[AxisId] == 0) {
			PEv2_data->PEv2_HomeBackOffDistance[AxisId] = dev->PEv2.HomeBackOffDistance[AxisId];
		}
		if (ApplyIniSettings == false || PEv2_data->PEv2_MPGjogDivider[AxisId] == 0) {
			PEv2_data->PEv2_MPGjogDivider[AxisId] = dev->PEv2.MPGjogDivider[AxisId];
		}
		/*if(ApplyIniSettings==false || PEv2_data->PEv2_AxisSignalOptions[AxisId]==0){
			PEv2_data->PEv2_AxisSignalOptions[AxisId] = dev->PEv2.AxisSignalOptions[AxisId];
		}*/
		if (ApplyIniSettings == false || PEv2_data->PEv2_digin_Probe_Filter[AxisId] == 0) {
			PEv2_data->PEv2_digin_Probe_Filter[AxisId] = dev->PEv2.FilterProbeInput;
		}


	}
	else {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AxisConfigurationGet(%d) != PK_OK\n", __FILE__, __FUNCTION__, AxisId);
	}
	return ret;
}

int32_t PEv2_AxisConfigurationSet(sPoKeysDevice* dev, int AxisId) {
	bool doSetup = false;
	uint8_t AxesConfig[8] = { 0 };
	uint8_t AxesSwitchConfig[8] = { 0 };
	int32_t ret = PEv2_AxisConfigurationGet(dev, AxisId);
	if (ret == PK_OK) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PEv2_AxisConfigurationSet(%d) == PK_OK\n", __FILE__, __FUNCTION__, AxisId);

		if (ApplyIniSettings == true) {


			// Read the structure and set the bits accordingly
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
			AxesConfig[AxisId] = 0;
			if (PEv2_data->PEv2_AxisEnabled[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 0, true); // PK_AC_ENABLED ;
			}
			else {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 0, false); // PK_AC_ENABLED ;
			}
			if (PEv2_data->PEv2_AxisInverted[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 1, true); // PK_AC_INVERTED ;
			}
			else {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 1, false); // PK_AC_INVERTED ;
			}
			if (PEv2_data->PEv2_AxisInternalPlanner[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 2, true); // PK_AC_INTERNAL_PLANNER ;
			}
			else {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 2, false); // PK_AC_INTERNAL_PLANNER ;
			}
			if (PEv2_data->PEv2_AxisPositionMode[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 3, true); // PK_AC_POSITION_MODE ;
			}
			else {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 3, false); // PK_AC_POSITION_MODE ;
			}
			if (PEv2_data->PEv2_AxisInvertedHome[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 4, true); // PK_AC_INVERTED_HOME ;
			}
			else {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 4, false); // PK_AC_INVERTED_HOME ;
			}
			if (PEv2_data->PEv2_AxisSoftLimitEnabled[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 5, true); // PK_AC_SOFT_LIMIT_ENABLED ;
			}
			else {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 5, false); // PK_AC_SOFT_LIMIT_ENABLED ;
			}
			if (PEv2_data->PEv2_AxisEnabledMasked[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 7, true); // PK_AC_ENABLED_MASKED ;
			}
			else {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 7, false); // PK_AC_ENABLED_MASKED ;
			}
			// PEv2_stepgen_AxesConfig(i) = PK_AC_ENABLED_MASKED;
			if (PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId] != 0) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 0, true); // PK_AC_ENABLED ;
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 2, true); // PK_AC_INTERNAL_PLANNER;
			}

			if (posMode[AxisId] == true) {
				AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 3, true); // PK_AC_POSITION_MODE;
				// posMode[AxisId] = true;
			}
			if (PEv2_data->PEv2_AxesConfig[AxisId] != AxesConfig[AxisId]) {
				PEv2_data->PEv2_AxesConfig[AxisId] = AxesConfig[AxisId];
			}
			if (dev->PEv2.AxesConfig[AxisId] != PEv2_data->PEv2_AxesConfig[AxisId]) {
				dev->PEv2.AxesConfig[AxisId] = PEv2_data->PEv2_AxesConfig[AxisId];
				doSetup = true;
			}

			/*
			Section: Axis switch configuration
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
			AxesSwitchConfig[AxisId] = PEv2_data->PEv2_AxesSwitchConfig[AxisId]; // initial value from ini file
			if (PEv2_data->PEv2_digin_Home_Pin[AxisId] != 0) {
				// path if pin (not dedicated) is used for home switch
				// check if pin is parametrized in HAL
				int Home = PEv2_data->PEv2_digin_Home_Pin[AxisId];
				int LimP = PEv2_data->PEv2_digin_LimitP_Pin[AxisId];
				int LimM = PEv2_data->PEv2_digin_LimitN_Pin[AxisId];

				if (dev->PEv2.PinHomeSwitch[AxisId] != Home) {
					dev->PEv2.PinHomeSwitch[AxisId] = Home;
					doSetup = true;
				}
				if (Home != LimM && Home != LimP) {

					if (IO_data->Pin[Home - 1].PinFunction != PK_PinCap_digitalInput) {
						IO_data->Pin[Home - 1].PinFunction = PK_PinCap_digitalInput;
					}
				}
				else if (Home == LimP) {
					// dev->PEv2.PinHomeSwitch[AxisId]=0;
					//AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 4, true); // 16 Home switch is shared with Limit+ switch
					if (dev->PEv2.PinHomeSwitch[AxisId] != 0) {
						dev->PEv2.PinHomeSwitch[AxisId] = 0;
						doSetup = true;
					}

					PEv2_data->PEv2_digin_Home_OnLimitP[AxisId] = 1;
					PEv2_data->PEv2_digin_Home_Enabled[AxisId] = 0;
				}
				else if (Home == LimM) {
					// dev->PEv2.PinHomeSwitch(i)=0;
					//AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 3, true); // 8 Home switch is shared with Limit- switch
					if (dev->PEv2.PinHomeSwitch[AxisId] != 0) {
						dev->PEv2.PinHomeSwitch[AxisId] = 0;
						doSetup = true;
					}
					PEv2_data->PEv2_digin_Home_OnLimitN[AxisId] = 1;
					PEv2_data->PEv2_digin_Home_Enabled[AxisId] = 0;
				}
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 2, true); // | PK_ASO_SWITCH_HOME;

			}
			else {
				dev->PEv2.PinHomeSwitch[AxisId] = 0;
			}
			if (PEv2_data->PEv2_digin_LimitN_Enabled[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 0, true); // PK_ASO_SWITCH_LIMIT_N ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 0, false); // PK_ASO_SWITCH_LIMIT_N ;
			}
			if (PEv2_data->PEv2_digin_LimitP_Enabled[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 1, true); // PK_ASO_SWITCH_LIMIT_P ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 1, false); // PK_ASO_SWITCH_LIMIT_P ;
			}
			if (PEv2_data->PEv2_digin_Home_Enabled[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 2, true); // PK_ASO_SWITCH_HOME ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 2, false); // PK_ASO_SWITCH_HOME ;
			}

			if (PEv2_data->PEv2_digin_LimitN_invert[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 5, true); // PK_ASO_SWITCH_INVERT_LIMIT_N ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 5, false); // PK_ASO_SWITCH_INVERT_LIMIT_N ;
			}
			if (PEv2_data->PEv2_digin_LimitP_invert[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 6, true); // PK_ASO_SWITCH_INVERT_LIMIT_P ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 6, false); // PK_ASO_SWITCH_INVERT_LIMIT_P ;
			}
			if (PEv2_data->PEv2_digin_Home_invert[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 7, true); // PK_ASO_SWITCH_INVERT_HOME ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 7, false); // PK_ASO_SWITCH_INVERT_HOME ;
			}


			if (PEv2_data->PEv2_digin_Home_OnLimitN[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 3, true); // PK_ASO_SWITCH_COMBINED_LN_H ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 3, false); // PK_ASO_SWITCH_COMBINED_LN_H ;
			}
			if (PEv2_data->PEv2_digin_Home_OnLimitP[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 4, true); // PK_ASO_SWITCH_COMBINED_LP_H ;
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 4, false); // PK_ASO_SWITCH_COMBINED_LP_H ;
			}

			if (PEv2_data->PEv2_digin_Home_OnLimitN[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 3, true); // PK_ASO_SWITCH_COMBINED_LN_H = (1 << 3),   // 8 Home switch is shared with Limit- switch
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 3, false); // PK_ASO_SWITCH_COMBINED_LN_H = (1 << 3),   // 8 Home switch is shared with Limit- switch
			}
			if (PEv2_data->PEv2_digin_Home_OnLimitP[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 4, true); // PK_ASO_SWITCH_COMBINED_LP_H = (1 << 4),   // 16 Home switch is shared with Limit+ switch
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 4, false); // PK_ASO_SWITCH_COMBINED_LP_H = (1 << 4),   // 16 Home switch is shared with Limit+ switch
			}
			if (PEv2_data->PEv2_digin_Home_invert[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 7, true); // PK_ASO_SWITCH_INVERT_HOME    = (1 << 7)    // 128 Invert home switch polarity
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 7, false); // PK_ASO_SWITCH_INVERT_HOME    = (1 << 7)    // 128 Invert home switch polarity
			}
			if (PEv2_data->PEv2_digin_LimitN_invert[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 5, true); // PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5),   // 32 Invert limit- switch polarity
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 5, false); // PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5),   // 32 Invert limit- switch polarity
			}
			if (PEv2_data->PEv2_digin_LimitP_invert[AxisId] != 0) {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 6, true); // PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6),   // 64 Invert limit+ switch polarity
			}
			else {
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 6, false); // PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6),   // 64 Invert limit+ switch polarity
			}
			if (PEv2_data->PEv2_AxesSwitchConfig[AxisId] != AxesSwitchConfig[AxisId]) {
				PEv2_data->PEv2_AxesSwitchConfig[AxisId] = AxesSwitchConfig[AxisId];
			}

			if (dev->PEv2.AxesSwitchConfig[AxisId] != PEv2_data->PEv2_AxesSwitchConfig[AxisId]) {
				dev->PEv2.AxesSwitchConfig[AxisId] = PEv2_data->PEv2_AxesSwitchConfig[AxisId];
				doSetup = true;
			}

			/* Section: Pin configuration
			*/
			if (dev->PEv2.PinHomeSwitch[AxisId] != PEv2_data->PEv2_digin_Home_Pin[AxisId]) {
				dev->PEv2.PinHomeSwitch[AxisId] = PEv2_data->PEv2_digin_Home_Pin[AxisId];
				doSetup = true;
			}

			if (dev->PEv2.PinLimitMSwitch[AxisId] != PEv2_data->PEv2_digin_LimitN_Pin[AxisId]) {
				dev->PEv2.PinLimitMSwitch[AxisId] = PEv2_data->PEv2_digin_LimitN_Pin[AxisId];
				doSetup = true;
			}
			if (PEv2_data->PEv2_digin_LimitN_Pin[AxisId] != 0) {																	   // check if pin is parametrized in HAL
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 0, true); // | PK_ASO_SWITCH_LIMIT_N;

				if (IO_data->Pin[PEv2_data->PEv2_digin_LimitN_Pin[AxisId] - 1].PinFunction != PK_PinCap_digitalInput) {
					IO_data->Pin[PEv2_data->PEv2_digin_LimitN_Pin[AxisId] - 1].PinFunction = PK_PinCap_digitalInput;
				}
			}

			if (dev->PEv2.PinLimitPSwitch[AxisId] != PEv2_data->PEv2_digin_LimitP_Pin[AxisId]) {
				dev->PEv2.PinLimitPSwitch[AxisId] = PEv2_data->PEv2_digin_LimitP_Pin[AxisId];
				doSetup = true;
			}
			if (PEv2_data->PEv2_digin_LimitP_Pin[AxisId] != 0) {																	   // check if pin is parametrized in HAL
				AxesSwitchConfig[AxisId] = Set_BitOfByte(AxesSwitchConfig[AxisId], 1, true); // | PK_ASO_SWITCH_LIMIT_P;

				if (IO_data->Pin[PEv2_data->PEv2_digin_LimitP_Pin[AxisId] - 1].PinFunction != PK_PinCap_digitalInput) {
					IO_data->Pin[PEv2_data->PEv2_digin_LimitP_Pin[AxisId] - 1].PinFunction = PK_PinCap_digitalInput;
				}
			}

			/*
				Section Speed and Acceleration settings

			*/

			if (PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId] != 0) {
				// need to ensure positve values for the following calculations otherwise machine will not move
				PEv2_data->PEv2_MaxSpeed[AxisId] = abs(PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId] * PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId]);				 // Maximum axis speed convert (mm/s) to (pulses / s)

				PEv2_data->PEv2_MaxAcceleration[AxisId] = abs(PEv2_data->PEv2_stepgen_STEPGEN_MAXACCEL[AxisId] * PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId]); // Maximum axis deceleration convert (mm/s²) to (in pulses/s²)

				PEv2_data->PEv2_MaxDecceleration[AxisId] = 1.5 * abs(PEv2_data->PEv2_stepgen_STEPGEN_MAXACCEL[AxisId] * PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId]); // Maximum axis deceleration convert (mm/s²) to (in pulses/s²)

				//PEv2_data->PEv2_HomePosition[AxisId]
			}
			if (PEv2_data->PEv2_stepgen_HOME_SEARCH_VEL[AxisId] > 0 && PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId] > 0 && PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId] > PEv2_data->PEv2_stepgen_HOME_SEARCH_VEL[AxisId]) {
				PEv2_data->PEv2_HomingSpeed[AxisId] = abs(PEv2_data->PEv2_stepgen_HOME_SEARCH_VEL[AxisId] * 100 / PEv2_data->PEv2_stepgen_STEPGEN_MAXVEL[AxisId]);		 // Homing speed per axis (in %)
				PEv2_data->PEv2_HomingReturnSpeed[AxisId] = abs(PEv2_data->PEv2_stepgen_HOME_LATCH_VEL[AxisId] * 100 / PEv2_data->PEv2_stepgen_HOME_SEARCH_VEL[AxisId]); // Homing return speed per axis (in % of the homing speed)
			}
			else {
				PEv2_data->PEv2_HomingSpeed[AxisId] = 30;
				PEv2_data->PEv2_HomingReturnSpeed[AxisId] = 50;
			}

			if (dev->PEv2.HomingSpeed[AxisId] != PEv2_data->PEv2_HomingSpeed[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: HomingSpeed[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_HomingSpeed[AxisId]);
				dev->PEv2.HomingSpeed[AxisId] = PEv2_data->PEv2_HomingSpeed[AxisId];
				doSetup = true;
			}
			if (dev->PEv2.HomingReturnSpeed[AxisId] != PEv2_data->PEv2_HomingReturnSpeed[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: HomingReturnSpeed[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_HomingReturnSpeed[AxisId]);
				dev->PEv2.HomingReturnSpeed[AxisId] = PEv2_data->PEv2_HomingReturnSpeed[AxisId];
				doSetup = true;
			}

			if (dev->PEv2.MaxSpeed[AxisId] != PEv2_data->PEv2_MaxSpeed[AxisId] / 1000 && PEv2_data->PEv2_MaxSpeed[AxisId] > 0) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: MaxSpeed[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_MaxSpeed[AxisId]);
				dev->PEv2.MaxSpeed[AxisId] = PEv2_data->PEv2_MaxSpeed[AxisId] / 1000;
				doSetup = true;
			}
			else if (PEv2_data->PEv2_MaxSpeed[AxisId] == 0 && dev->PEv2.MaxSpeed[AxisId] == 0) {
				PEv2_data->PEv2_MaxSpeed[AxisId] = 1;
				dev->PEv2.MaxSpeed[AxisId] = 1000;
				doSetup = true;
			}
			int relevantdigitsfactor = 1000;
			int unitconversionfactor = 1000000;
			if ((int)(dev->PEv2.MaxAcceleration[AxisId] * unitconversionfactor * relevantdigitsfactor) != (int)(PEv2_data->PEv2_MaxAcceleration[AxisId] * relevantdigitsfactor)) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: MaxAcceleration[%d] = %d ; dev->PEv2.MaxAcceleration = %d \n", __FILE__, __FUNCTION__, AxisId, (int)(PEv2_data->PEv2_MaxAcceleration[AxisId] * 1000), (int)(dev->PEv2.MaxAcceleration[AxisId] * 1000000 * 1000));
				dev->PEv2.MaxAcceleration[AxisId] = PEv2_data->PEv2_MaxAcceleration[AxisId] / unitconversionfactor;
				doSetup = true;
			}

			if ((int)(dev->PEv2.MaxDecceleration[AxisId] * unitconversionfactor * relevantdigitsfactor) != (int)(PEv2_data->PEv2_MaxDecceleration[AxisId] * relevantdigitsfactor)) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: MaxDecceleration[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_MaxDecceleration[AxisId]);
				dev->PEv2.MaxDecceleration[AxisId] = PEv2_data->PEv2_MaxDecceleration[AxisId] / unitconversionfactor;
				doSetup = true;
			}

			/*
			Section: Soft limits
			*/
			//float LimitOffset = 0.0;
			if (PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId] > PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId]) {

				if (PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId] != abs(PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId])) {
					// in pokeys only positive values are allowed - Shift reference to 0 and make sure it is positive
					PEv2_data->PEv2_PositionOffset[AxisId] = PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId] - PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId]; // shift reference to 0 and make sure it is positive
				}
				else {
					// PEv2_stepgen_HOME[AxisId]
					PEv2_data->PEv2_PositionOffset[AxisId] = 0;
				}

				if (PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId] == 0) {
					PEv2_data->PEv2_PositionScale[AxisId] = 1;
				}
				else {
					PEv2_data->PEv2_PositionScale[AxisId] = PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId];
				}

				PEv2_data->PEv2_HomePosition[AxisId] = (PEv2_data->PEv2_PositionOffset[AxisId] + PEv2_data->PEv2_stepgen_HOME[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId];
				PEv2_data->PEv2_digin_SoftLimit_PosMax[AxisId] = (PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId]; // Soft limit maximum position
				PEv2_data->PEv2_digin_SoftLimit_PosMin[AxisId] = (PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId]; // Soft limit minimum position

				PEv2_data->PEv2_ZeroPosition[AxisId] = PEv2_data->PEv2_PositionOffset[AxisId] * PEv2_data->PEv2_PositionScale[AxisId];
				PEv2_data->PEv2_ArmPosition[AxisId] = (PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId];



			}
			else {
				// for pokeys MaxLimits must be higher than MinLimits otherwise machine will not move
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: MaxLimit[%d] = %d ; MinLimit[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId], AxisId, PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId]);

				if (PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId] != abs(PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId])) {
					// in pokeys only positive values are allowed - Shift reference to 0 and make sure it is positive
					PEv2_data->PEv2_PositionOffset[AxisId] = PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId] - PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId]; // shift reference to 0 and make sure it is positive
				}
				else {
					// PEv2_stepgen_HOME[AxisId]
					PEv2_data->PEv2_PositionOffset[AxisId] = 0;
				}

				if (PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId] == 0) {
					PEv2_data->PEv2_PositionScale[AxisId] = 0 - 1;
				}
				else {
					PEv2_data->PEv2_PositionScale[AxisId] = PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId] * (0 - 1);
				}

				PEv2_data->PEv2_digin_SoftLimit_PosMax[AxisId] = (PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId]; // Soft limit maximum position
				PEv2_data->PEv2_digin_SoftLimit_PosMin[AxisId] = (PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId]; // Soft limit minimum position

				PEv2_data->PEv2_HomePosition[AxisId] = (PEv2_data->PEv2_PositionOffset[AxisId] + PEv2_data->PEv2_stepgen_HOME[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId];
				PEv2_data->PEv2_digin_SoftLimit_PosMax[AxisId] = (PEv2_data->PEv2_stepgen_MAX_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId]; // Soft limit maximum position
				PEv2_data->PEv2_digin_SoftLimit_PosMin[AxisId] = (PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId]; // Soft limit minimum position

				PEv2_data->PEv2_ZeroPosition[AxisId] = PEv2_data->PEv2_PositionOffset[AxisId] * PEv2_data->PEv2_PositionScale[AxisId];
				PEv2_data->PEv2_ArmPosition[AxisId] = (PEv2_data->PEv2_stepgen_MIN_LIMIT[AxisId] + PEv2_data->PEv2_PositionOffset[AxisId]) * PEv2_data->PEv2_PositionScale[AxisId];

				// AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 1, true);  // PK_AC_INVERTED ;
				// AxesConfig[AxisId] = Set_BitOfByte(AxesConfig[AxisId], 5, true);  // PK_AC_SOFT_LIMIT_ENABLED ;

			}
			if (dev->PEv2.SoftLimitMinimum[AxisId] != PEv2_data->PEv2_SoftLimitMinimum[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: SoftLimitMinimum[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_SoftLimitMinimum[AxisId]);
				dev->PEv2.SoftLimitMinimum[AxisId] = PEv2_data->PEv2_SoftLimitMinimum[AxisId];
				doSetup = true;
			}
			if (dev->PEv2.SoftLimitMaximum[AxisId] != PEv2_data->PEv2_SoftLimitMaximum[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: SoftLimitMaximum[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_SoftLimitMaximum[AxisId]);
				dev->PEv2.SoftLimitMaximum[AxisId] = PEv2_data->PEv2_SoftLimitMaximum[AxisId];
				doSetup = true;
			}

			if (dev->PEv2.AxisEnableOutputPins[AxisId] != PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: AxisEnableOutputPins[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId]);
				dev->PEv2.AxisEnableOutputPins[AxisId] = PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId];
				doSetup = true;
			}
			if (PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] != 0) { // check if pin is parametrized in HAL
				if (IO_data->Pin[PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] - 1].PinFunction != PK_PinCap_digitalOutput) {
					IO_data->Pin[PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] - 1].PinFunction = PK_PinCap_digitalOutput;
				}
				Pins_DigitalValueSet_ignore[PEv2_data->PEv2_digout_AxisEnable_Pin[AxisId] - 1] = true;
			}
			if (dev->PEv2.InvertAxisEnable[AxisId] != PEv2_data->PEv2_digout_AxisEnable_invert[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: InvertAxisEnable[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_digout_AxisEnable_invert[AxisId]);
				dev->PEv2.InvertAxisEnable[AxisId] = PEv2_data->PEv2_digout_AxisEnable_invert[AxisId];
				doSetup = true;
			}
			if (dev->PEv2.FilterLimitMSwitch[AxisId] != PEv2_data->PEv2_digin_LimitN_Filter[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FilterLimitMSwitch[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_digin_LimitN_Filter[AxisId]);
				dev->PEv2.FilterLimitMSwitch[AxisId] = PEv2_data->PEv2_digin_LimitN_Filter[AxisId];
				doSetup = true;
			}
			if (dev->PEv2.FilterLimitPSwitch[AxisId] != PEv2_data->PEv2_digin_LimitP_Filter[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FilterLimitPSwitch[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_digin_LimitP_Filter[AxisId]);
				dev->PEv2.FilterLimitPSwitch[AxisId] = PEv2_data->PEv2_digin_LimitP_Filter[AxisId];
				doSetup = true;
			}
			if (dev->PEv2.FilterHomeSwitch[AxisId] != PEv2_data->PEv2_digin_Home_Filter[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FilterHomeSwitch[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_digin_Home_Filter[AxisId]);
				dev->PEv2.FilterHomeSwitch[AxisId] = PEv2_data->PEv2_digin_Home_Filter[AxisId];
				doSetup = true;
			}

			/*
			Section: HomingAlgorithm
			*/
			int HomingAlgorithm = 0;
			if (PEv2_data->PEv2_HomeAlg_OnHome_Stop[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OnHomeStop;
			}
			if (PEv2_data->PEv2_HomeAlg_OnHome_ArmEncoder[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OnHomeArmEncoder;
			}
			if (PEv2_data->PEv2_HomeAlg_OnHome_RevDirection[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OnHomeReverseDirection;
			}
			if (PEv2_data->PEv2_HomeAlg_OnHome_ReducedSpeed[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OnHomeReducedSpeed;
			}
			if (PEv2_data->PEv2_HomeAlg_OutHome_Stop[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OutHomeStop;
			}
			if (PEv2_data->PEv2_HomeAlg_OutHome_ArmEncoder[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OutHomeArmEncoder;
			}
			if (PEv2_data->PEv2_HomeAlg_OutHome_RevDirection[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OutHomeReverseDirection;
			}
			if (PEv2_data->PEv2_HomeAlg_OutHome_ReducedSpeed[AxisId] != 0) {
				HomingAlgorithm |= PK_PEv2Homing_OutHomeReducedSpeed;
			}
			if (PEv2_data->PEv2_HomingAlgorithm[AxisId] != HomingAlgorithm) {
				PEv2_data->PEv2_HomingAlgorithm[AxisId] = HomingAlgorithm;
			}
			if (dev->PEv2.HomingAlgorithm[AxisId] != PEv2_data->PEv2_HomingAlgorithm[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: HomingAlgorithm[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_HomingAlgorithm[AxisId]);
				dev->PEv2.HomingAlgorithm[AxisId] = PEv2_data->PEv2_HomingAlgorithm[AxisId];
				doSetup = true;
			}

			if (PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId] != 0) {
				PEv2_data->PEv2_HomeBackOffDistance[AxisId] = PEv2_data->PEv2_stepgen_HOME_OFFSET[AxisId] * PEv2_data->PEv2_stepgen_STEP_SCALE[AxisId]; // Home position offset
			}
			if (dev->PEv2.HomeBackOffDistance[AxisId] != PEv2_data->PEv2_HomeBackOffDistance[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: HomeBackOffDistance[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_HomeBackOffDistance[AxisId]);
				dev->PEv2.HomeBackOffDistance[AxisId] = PEv2_data->PEv2_HomeBackOffDistance[AxisId];
				doSetup = true;
			}
			/*
			 Section: MPG jog settings
			*/
			if (dev->PEv2.MPGjogEncoder[AxisId] != PEv2_data->PEv2_MPGjogEncoder[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: MPGjogEncoder[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_MPGjogEncoder[AxisId]);
				dev->PEv2.MPGjogEncoder[AxisId] = PEv2_data->PEv2_MPGjogEncoder[AxisId];

				doSetup = true;
			}
			if (dev->PEv2.MPGjogDivider[AxisId] != PEv2_data->PEv2_MPGjogDivider[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: MPGjogDivider[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_MPGjogDivider[AxisId]);
				dev->PEv2.MPGjogDivider[AxisId] = PEv2_data->PEv2_MPGjogDivider[AxisId];
				doSetup = true;
			}
			if (dev->PEv2.MPGjogMultiplier[AxisId] != PEv2_data->PEv2_MPGjogMultiplier[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: MPGjogMultiplier[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_MPGjogMultiplier[AxisId]);
				dev->PEv2.MPGjogMultiplier[AxisId] = PEv2_data->PEv2_MPGjogMultiplier[AxisId];
				doSetup = true;
			}
			/*if(dev->PEv2.AxisSignalOptions[AxisId] != PEv2_data->PEv2_AxisSignalOptions[AxisId]){
				PEv2_data->PEv2_AxisSignalOptions[AxisId] = dev->PEv2.AxisSignalOptions[AxisId];
				doSetup = true;
			}*/
			if (dev->PEv2.FilterProbeInput != PEv2_data->PEv2_digin_Probe_Filter[AxisId]) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: FilterProbeInput[%d] = %d\n", __FILE__, __FUNCTION__, AxisId, PEv2_data->PEv2_digin_Probe_Filter[AxisId]);
				dev->PEv2.FilterProbeInput = PEv2_data->PEv2_digin_Probe_Filter[AxisId];
				doSetup = true;
			}

			if (doSetup == true) {
				if (PK_PEv2_AxisConfigurationSet(dev) != PK_OK) {
					rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AxisConfigurationSet(%d) != PK_OK\n", __FILE__, __FUNCTION__, AxisId);
					if (PK_PEv2_AxisConfigurationSet(dev) != PK_OK) {
						rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PEv2_AxisConfigurationSet(%d) != PK_OK\n", __FILE__, __FUNCTION__, AxisId);
					}
				}
				else {
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PEv2_AxisConfigurationSet(%d) == PK_OK\n", __FILE__, __FUNCTION__, AxisId);
					usleep(sleepdur);
					if (PK_SaveConfiguration(dev) == PK_OK) {
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_SaveConfiguration() == PK_OK\n", __FILE__, __FUNCTION__);
						usleep(sleepdur);
						//	PK_PEv2_PulseEngineReboot(dev);
						usleep(1000000);
					}
					else {
						rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_SaveConfiguration() != PK_OK\n", __FILE__, __FUNCTION__);
					}
				}
				usleep(sleepdur);
			}
			return ret;
		}
	}
	else {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PEv2_AxisConfigurationSet(%d) != PK_OK\n", __FILE__, __FUNCTION__, AxisId);
		return ret;
	}
	return ret;
}


#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "pin:PEv2.deb.out:s32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.deb.estop:s32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.axxisout:s32:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.ishoming:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.inposition:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.PosMode:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.PosModeAct:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.velmode_count:s32:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.posmode_count:s32:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.doMove:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.RefSpeed:float:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.RefPos:float:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.deb.RefPosSpeed:float:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.nrOfAxes:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.maxPulseFrequency:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.bufferDepth:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.slotTiming:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.params.ApplyIniSettings:bit:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.AxesState:u32:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.AxesCommand:u32:8:in::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.home_sequence:s32:8:rw:home_sequence:None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.AxesConfig:u32:8:out::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxisEnabled:s32:8:rw:Axis enabled:None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxisInverted:s32:8:rw:Axis inverted:None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxisInternalPlanner:s32:8:rw:Axis uses internal motion planner:None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxisPositionMode:s32:8:rw:Internal motion planner for this axis is in position mode:None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxisInvertedHome:s32:8:rw:Axis homing direction is inverted:None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxisSoftLimitEnabled:s32:8:rw:Use soft-limits for this axis:None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxisEnabledMasked:s32:8:rw:Use output enable pin masking:None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.SoftLimitMaximum:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.SoftLimitMinimum:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.SoftLimit.PosMin:u32:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.SoftLimit.PosMax:u32:8:rw::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.HomingSpeed:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.HomingReturnSpeed:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomingAlgorithm:u32:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OnHome.Stop:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OnHome.ArmEncoder:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OnHome.RevDirection:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OnHome.ReducedSpeed:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OutHome.Stop:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OutHome.ArmEncoder:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OutHome.RevDirection:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.HomeAlg.OutHome.ReducedSpeed:bit:8:rw::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.HomeOffsets:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.Home.Offset:u32:8:rw::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.ProbePosition:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.ProbeMaxPosition:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.CurrentPosition:s32:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.PositionSetup:s32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.ReferencePositionSpeed:u32:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.MaxSpeed:float:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.MaxAcceleration:float:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.MaxDecceleration:float:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.joint-vel-cmd:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.joint-pos-cmd:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.joint-pos-fb:float:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.joint-out-home:bit:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.joint-in-position:bit:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.joint-kb-jog-active:bit:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.joint-wheel-jog-active:bit:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.TYPE:s32:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.HOME:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.STEPGEN_MAXVEL:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.STEPGEN_MAXACCEL:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.params.Feedback_Encoder_Id:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.DEADBAND:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.MAX-OUTPUT:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.ENCODER-SCALE:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.STEP-SCALE:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.MIN-LIMIT:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.MAX-LIMIT:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.HOME-OFFSET:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.HOME-SEARCH_VEL:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.HOME-LATCH-VEL:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.HOME-FINAL-VEL:float:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.stepgen.HOME-IGNORE-LIMITS:s32:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.MPGjogMultiplier:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.MPGjogEncoder:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.MPGjogDivider:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.AxesSwitchConfig:u32:8:rw::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitN.Enabled:bit:8:rw:Limit- is available (PK_ASO_SWITCH_LIMIT_N):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitP.Enabled:bit:8:rw:Limit+ is available (PK_ASO_SWITCH_LIMIT_P):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.Home.Enabled:bit:8:rw:Invert home-switch (PK_ASO_SWITCH_HOME):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.Home.OnLimitN:bit:8:rw:Shared with Limit- (PK_ASO_SWITCH_COMBINED_LN_H):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.Home.OnLimitP:bit:8:rw:Shared with Limit+ (PK_ASO_SWITCH_COMBINED_LP_H):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitN.invert:bit:8:rw:Invert limit- (PK_ASO_SWITCH_INVERT_LIMIT_N):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitP.invert:bit:8:rw:Invert limit+ (PK_ASO_SWITCH_INVERT_LIMIT_P):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.Home.invert:bit:8:rw:Invert home-switch (PK_ASO_SWITCH_INVERT_HOME):None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.LimitN.in:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.LimitN.in-not:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.LimitN.DedicatedInput:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitN.Pin:u32:8:rw:Limit- switch pin (0 for external dedicated input):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitN.Filter:u32:8:rw:Digital filter for limit- switch:None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.LimitP.in:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.LimitP.in-not:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.LimitP.DedicatedInput:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitP.Pin:u32:8:rw:Limit+ switch pin (0 for external dedicated input):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.LimitP.Filter:u32:8:rw:Digital filter for limit+ switch:None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.Home.in:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.Home.in-not:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.Home.DedicatedInput:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.Home.Pin:u32:8:rw:Home switch pin (0 for external dedicated input):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digin.Home.Filter:u32:8:rw:Digital filter for home-switch:None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digout.AxisEnable.out:bit:8:in::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digout.AxisEnable.Pin:u32:8:rw:Axis enabled output pin (0 for external dedicated output):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.#.digout.AxisEnable.invert:bit:8:rw:Invert axis enable signal:None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.HomeBackOffDistance:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.PulseEngineEnabled:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.PulseGeneratorType:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.PG_swap_stepdir:bit:0:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.PG_extended_io:bit:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.ChargePumpEnabled:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.PulseEngineActivated:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.PulseEngineState:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.Error.in:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.Error.in-not:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.MiscInputStatus:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digin.Misc-#.in:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digin.Misc-#.in-not:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.LimitOverride:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.LimitOverrideSetup:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digin.Probed.in:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.Probe.in:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.Probe.in-not:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.digin.Emergency.Pin:u32:0:rw::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digin.Emergency.in:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digin.Emergency.in-not:bit:0:out::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.digin.Emergency.invert:u32:0:rw:Emergency switch polarity (set to 1 to invert):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.digout.Emergency.Pin:u32:0:rw:added in component only for estop-out to reset estop (if 0 EmergencyInput will be reused):None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digout.Emergency.out:bit:0:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.SoftLimit.in:u32:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.AxisEnabledMask:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.AxisEnabledStatesMask:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digout.AxisEnabled.out:bit:8:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digin.AxisEnabled.in:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.digout.LimitOverride.out:bit:8:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.ExternalRelayOutputs:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.ExternalOCOutputs:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digout.ExternalRelay-#.out:bit:4:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.digout.ExternalOC-#.out:bit:4:in::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.HomingStartMaskSetup:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.ProbeStartMaskSetup:u32:0:io::None:None");
MODULE_INFO(linuxcnc, "param:PEv2.digin.Probe.Pin:u32:0:rw:Probe input (0:disabled, 1-8:external inputs, 9+ Pin ID-9):None:None");
MODULE_INFO(linuxcnc, "param:PEv2.digin.Probe.invert:u32:0:rw:Probe input polarity:None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.ProbeStatus:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.ProbeSpeed:float:0:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.BacklashWidth:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.BacklashRegister:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.#.BacklashAcceleration:u32:8:io::None:None");
MODULE_INFO(linuxcnc, "pin:PEv2.BacklashCompensationEnabled:u32:0:io::None:None");
#endif // MODULE_INFO