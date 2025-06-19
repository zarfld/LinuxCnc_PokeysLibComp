import ctypes

class PEv2MotionControl:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib
        if not hasattr(self.device, "Axes"):
            # Default to eight axes for tests using simple mocks
            self.device.Axes = [None] * 8

    def setup(self, axis, parameters):
        """
        Configure an axis for motion control.
        :param axis: Axis number
        :param parameters: Dictionary of parameters
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_AxisConfigurationSet(self.device, axis, parameters)

    def fetch_status(self):
        """
        Retrieve the current motion status and position feedback.
        :return: Status and position feedback
        """
        try:
            status = self.pokeyslib.PK_PEv2_StatusGet(self.device)
            position = self.pokeyslib.PK_PEv2_GetPosition(self.device)
            return status, position
        except ValueError:
            raise ValueError("Invalid device")

    def set_position(self, axis, position):
        """
        Send position command to an axis.
        :param axis: Axis number
        :param position: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetPosition(self.device, axis, position)

    def set_velocity(self, axis, velocity):
        """
        Send velocity command to an axis.
        :param axis: Axis number
        :param velocity: Velocity value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetVelocity(self.device, axis, velocity)

    def start_homing(self, axis):
        """
        Start the homing procedure for an axis.
        :param axis: Axis number
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_StartHoming(self.device, axis)

    def cancel_homing(self, axis):
        """
        Cancel the homing procedure for an axis.
        :param axis: Axis number
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_CancelHoming(self.device, axis)

    def get_homing_status(self, axis):
        """
        Get the homing status for an axis.
        :param axis: Axis number
        :return: Homing status
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetHomingStatus(self.device, axis)

    def manage_homing_signals(self, axis):
        """
        Manage the homing signals for an axis.
        :param axis: Axis number
        """
        homing_status = self.get_homing_status(axis)
        if (homing_status == 11):  # PK_PEAxisState_axHOMINGSTART
            self.start_homing(axis)
        elif (homing_status == 12):  # PK_PEAxisState_axHOMINGSEARCH
            self.set_velocity(axis, 0.5)  # Example velocity value
        elif (homing_status == 13):  # PK_PEAxisState_axHOMINGBACK
            self.set_velocity(axis, 0.1)  # Example velocity value
        elif (homing_status == 10):  # PK_PEAxisState_axHOME
            self.set_position(axis, 0)  # Example position value
        else:
            self.cancel_homing(axis)

    def add_pins(self):
        """
        Add missing pins and parameters for PEv2.
        """
        self.pokeyslib.PK_PEv2_AddPins(self.device)

    def handle_new_pins(self):
        """
        Implement methods to handle new pins and parameters.
        """
        self.pokeyslib.PK_PEv2_HandleNewPins(self.device)

    def initialize_pins(self):
        """
        Initialize PEv2 pins using correct enumerations and commands.
        """
        self.pokeyslib.PK_PEv2_InitializePins(self.device)

    def create_required_pins(self):
        """
        Ensure pins for required and enabled axes are created only.
        """
        self.pokeyslib.PK_PEv2_CreateRequiredPins(self.device)

    def update_documentation(self):
        """
        Update documentation accordingly.
        """
        self.pokeyslib.PK_PEv2_UpdateDocumentation(self.device)

    def set_axis_state(self, axis, state):
        """
        Set the state of an axis.
        :param axis: Axis number
        :param state: State value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetAxisState(self.device, axis, state)

    def get_axis_state(self, axis):
        """
        Get the state of an axis.
        :param axis: Axis number
        :return: State value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetAxisState(self.device, axis)

    def set_axis_config(self, axis, config):
        """
        Set the configuration of an axis.
        :param axis: Axis number
        :param config: Configuration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetAxisConfig(self.device, axis, config)

    def get_axis_config(self, axis):
        """
        Get the configuration of an axis.
        :param axis: Axis number
        :return: Configuration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetAxisConfig(self.device, axis)

    def set_axis_switch_config(self, axis, switch_config):
        """
        Set the switch configuration of an axis.
        :param axis: Axis number
        :param switch_config: Switch configuration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetAxisSwitchConfig(self.device, axis, switch_config)

    def get_axis_switch_config(self, axis):
        """
        Get the switch configuration of an axis.
        :param axis: Axis number
        :return: Switch configuration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetAxisSwitchConfig(self.device, axis)

    def set_soft_limit_maximum(self, axis, limit):
        """
        Set the soft limit maximum position for an axis.
        :param axis: Axis number
        :param limit: Limit value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetSoftLimitMaximum(self.device, axis, limit)

    def get_soft_limit_maximum(self, axis):
        """
        Get the soft limit maximum position for an axis.
        :param axis: Axis number
        :return: Limit value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetSoftLimitMaximum(self.device, axis)

    def set_soft_limit_minimum(self, axis, limit):
        """
        Set the soft limit minimum position for an axis.
        :param axis: Axis number
        :param limit: Limit value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetSoftLimitMinimum(self.device, axis, limit)

    def get_soft_limit_minimum(self, axis):
        """
        Get the soft limit minimum position for an axis.
        :param axis: Axis number
        :return: Limit value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetSoftLimitMinimum(self.device, axis)

    def set_homing_speed(self, axis, speed):
        """
        Set the homing speed for an axis.
        :param axis: Axis number
        :param speed: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetHomingSpeed(self.device, axis, speed)

    def get_homing_speed(self, axis):
        """
        Get the homing speed for an axis.
        :param axis: Axis number
        :return: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetHomingSpeed(self.device, axis)

    def set_homing_return_speed(self, axis, speed):
        """
        Set the homing return speed for an axis.
        :param axis: Axis number
        :param speed: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetHomingReturnSpeed(self.device, axis, speed)

    def get_homing_return_speed(self, axis):
        """
        Get the homing return speed for an axis.
        :param axis: Axis number
        :return: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetHomingReturnSpeed(self.device, axis)

    def set_homing_algorithm(self, axis, algorithm):
        """
        Set the homing algorithm for an axis.
        :param axis: Axis number
        :param algorithm: Algorithm value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetHomingAlgorithm(self.device, axis, algorithm)

    def get_homing_algorithm(self, axis):
        """
        Get the homing algorithm for an axis.
        :param axis: Axis number
        :return: Algorithm value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetHomingAlgorithm(self.device, axis)

    def set_home_offsets(self, axis, offset):
        """
        Set the home position offset for an axis.
        :param axis: Axis number
        :param offset: Offset value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetHomeOffsets(self.device, axis, offset)

    def get_home_offsets(self, axis):
        """
        Get the home position offset for an axis.
        :param axis: Axis number
        :return: Offset value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetHomeOffsets(self.device, axis)

    def set_probe_position(self, axis, position):
        """
        Set the probe position for an axis.
        :param axis: Axis number
        :param position: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetProbePosition(self.device, axis, position)

    def get_probe_position(self, axis):
        """
        Get the probe position for an axis.
        :param axis: Axis number
        :return: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetProbePosition(self.device, axis)

    def set_probe_max_position(self, axis, position):
        """
        Set the maximum probe position for an axis.
        :param axis: Axis number
        :param position: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetProbeMaxPosition(self.device, axis, position)

    def get_probe_max_position(self, axis):
        """
        Get the maximum probe position for an axis.
        :param axis: Axis number
        :return: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetProbeMaxPosition(self.device, axis)

    def set_current_position(self, axis, position):
        """
        Set the current position for an axis.
        :param axis: Axis number
        :param position: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetCurrentPosition(self.device, axis, position)

    def get_current_position(self, axis):
        """
        Get the current position for an axis.
        :param axis: Axis number
        :return: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetCurrentPosition(self.device, axis)

    def set_position_setup(self, axis, position):
        """
        Set the position to be set as current position for an axis.
        :param axis: Axis number
        :param position: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetPositionSetup(self.device, axis, position)

    def get_position_setup(self, axis):
        """
        Get the position to be set as current position for an axis.
        :param axis: Axis number
        :return: Position value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetPositionSetup(self.device, axis)

    def set_reference_position_speed(self, axis, speed):
        """
        Set the reference position or speed for an axis.
        :param axis: Axis number
        :param speed: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetReferencePositionSpeed(self.device, axis, speed)

    def get_reference_position_speed(self, axis):
        """
        Get the reference position or speed for an axis.
        :param axis: Axis number
        :return: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetReferencePositionSpeed(self.device, axis)

    def set_max_speed(self, axis, speed):
        """
        Set the maximum speed for an axis.
        :param axis: Axis number
        :param speed: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetMaxSpeed(self.device, axis, speed)

    def get_max_speed(self, axis):
        """
        Get the maximum speed for an axis.
        :param axis: Axis number
        :return: Speed value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetMaxSpeed(self.device, axis)

    def set_max_acceleration(self, axis, acceleration):
        """
        Set the maximum acceleration for an axis.
        :param axis: Axis number
        :param acceleration: Acceleration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetMaxAcceleration(self.device, axis, acceleration)

    def get_max_acceleration(self, axis):
        """
        Get the maximum acceleration for an axis.
        :param axis: Axis number
        :return: Acceleration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetMaxAcceleration(self.device, axis)

    def set_mpg_jog_multiplier(self, axis, multiplier):
        """
        Set the MPG jog multiplier value for an axis.
        :param axis: Axis number
        :param multiplier: Multiplier value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetMPGJogMultiplier(self.device, axis, multiplier)

    def get_mpg_jog_multiplier(self, axis):
        """
        Get the MPG jog multiplier value for an axis.
        :param axis: Axis number
        :return: Multiplier value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetMPGJogMultiplier(self.device, axis)

    def set_mpg_jog_encoder(self, axis, encoder):
        """
        Set the MPG jog encoder ID for an axis.
        :param axis: Axis number
        :param encoder: Encoder ID
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetMPGJogEncoder(self.device, axis, encoder)

    def get_mpg_jog_encoder(self, axis):
        """
        Get the MPG jog encoder ID for an axis.
        :param axis: Axis number
        :return: Encoder ID
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetMPGJogEncoder(self.device, axis)

    def set_mpg_jog_divider(self, axis, divider):
        """
        Set the divider for the MPG jogging for an axis.
        :param axis: Axis number
        :param divider: Divider value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetMPGJogDivider(self.device, axis, divider)

    def get_mpg_jog_divider(self, axis):
        """
        Get the divider for the MPG jogging for an axis.
        :param axis: Axis number
        :return: Divider value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetMPGJogDivider(self.device, axis)

    def set_digin_limit_n_in(self, axis, value):
        """
        Set the digital input limit N for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginLimitNIn(self.device, axis, value)

    def get_digin_limit_n_in(self, axis):
        """
        Get the digital input limit N for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginLimitNIn(self.device, axis)

    def set_digin_limit_n_in_not(self, axis, value):
        """
        Set the digital input limit N (inverted) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginLimitNInNot(self.device, axis, value)

    def get_digin_limit_n_in_not(self, axis):
        """
        Get the digital input limit N (inverted) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginLimitNInNot(self.device, axis)

    def set_digin_limit_n_dedicated_input(self, axis, value):
        """
        Set the digital input limit N (dedicated) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginLimitNDedicatedInput(self.device, axis, value)

    def get_digin_limit_n_dedicated_input(self, axis):
        """
        Get the digital input limit N (dedicated) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginLimitNDedicatedInput(self.device, axis)

    def set_digin_limit_p_in(self, axis, value):
        """
        Set the digital input limit P for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginLimitPIn(self.device, axis, value)

    def get_digin_limit_p_in(self, axis):
        """
        Get the digital input limit P for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginLimitPIn(self.device, axis)

    def set_digin_limit_p_in_not(self, axis, value):
        """
        Set the digital input limit P (inverted) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginLimitPInNot(self.device, axis, value)

    def get_digin_limit_p_in_not(self, axis):
        """
        Get the digital input limit P (inverted) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginLimitPInNot(self.device, axis)

    def set_digin_limit_p_dedicated_input(self, axis, value):
        """
        Set the digital input limit P (dedicated) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginLimitPDedicatedInput(self.device, axis, value)

    def get_digin_limit_p_dedicated_input(self, axis):
        """
        Get the digital input limit P (dedicated) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginLimitPDedicatedInput(self.device, axis)

    def set_digout_axis_enable_out(self, axis, value):
        """
        Set the digital output axis enable for an axis.
        :param axis: Axis number
        :param value: Output value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDigoutAxisEnableOut(self.device, axis, value)

    def get_digout_axis_enable_out(self, axis):
        """
        Get the digital output axis enable for an axis.
        :param axis: Axis number
        :return: Output value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDigoutAxisEnableOut(self.device, axis)

    def set_digin_home_in(self, axis, value):
        """
        Set the digital input home for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginHomeIn(self.device, axis, value)

    def get_digin_home_in(self, axis):
        """
        Get the digital input home for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginHomeIn(self.device, axis)

    def set_digin_home_in_not(self, axis, value):
        """
        Set the digital input home (inverted) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginHomeInNot(self.device, axis, value)

    def get_digin_home_in_not(self, axis):
        """
        Get the digital input home (inverted) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginHomeInNot(self.device, axis)

    def set_digin_home_dedicated_input(self, axis, value):
        """
        Set the digital input home (dedicated) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginHomeDedicatedInput(self.device, axis, value)

    def get_digin_home_dedicated_input(self, axis):
        """
        Get the digital input home (dedicated) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginHomeDedicatedInput(self.device, axis)

    def set_home_back_off_distance(self, axis, distance):
        """
        Set the back-off distance after homing for an axis.
        :param axis: Axis number
        :param distance: Distance value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetHomeBackOffDistance(self.device, axis, distance)

    def get_home_back_off_distance(self, axis):
        """
        Get the back-off distance after homing for an axis.
        :param axis: Axis number
        :return: Distance value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetHomeBackOffDistance(self.device, axis)

    def set_digin_error_in(self, axis, value):
        """
        Set the digital input error for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginErrorIn(self.device, axis, value)

    def get_digin_error_in(self, axis):
        """
        Get the digital input error for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginErrorIn(self.device, axis)

    def set_digin_error_in_not(self, axis, value):
        """
        Set the digital input error (inverted) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginErrorInNot(self.device, axis, value)

    def get_digin_error_in_not(self, axis):
        """
        Get the digital input error (inverted) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginErrorInNot(self.device, axis)

    def set_digin_probe_in(self, axis, value):
        """
        Set the digital input probe for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginProbeIn(self.device, axis, value)

    def get_digin_probe_in(self, axis):
        """
        Get the digital input probe for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginProbeIn(self.device, axis)

    def set_digin_probe_in_not(self, axis, value):
        """
        Set the digital input probe (inverted) for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginProbeInNot(self.device, axis, value)

    def get_digin_probe_in_not(self, axis):
        """
        Get the digital input probe (inverted) for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginProbeInNot(self.device, axis)

    def set_digin_soft_limit_in(self, axis, value):
        """
        Set the digital input soft limit for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginSoftLimitIn(self.device, axis, value)

    def get_digin_soft_limit_in(self, axis):
        """
        Get the digital input soft limit for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginSoftLimitIn(self.device, axis)

    def set_digout_axis_enabled_out(self, axis, value):
        """
        Set the digital output axis enabled for an axis.
        :param axis: Axis number
        :param value: Output value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDigoutAxisEnabledOut(self.device, axis, value)

    def get_digout_axis_enabled_out(self, axis):
        """
        Get the digital output axis enabled for an axis.
        :param axis: Axis number
        :return: Output value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDigoutAxisEnabledOut(self.device, axis)

    def set_digin_axis_enabled_in(self, axis, value):
        """
        Set the digital input axis enabled for an axis.
        :param axis: Axis number
        :param value: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDiginAxisEnabledIn(self.device, axis, value)

    def get_digin_axis_enabled_in(self, axis):
        """
        Get the digital input axis enabled for an axis.
        :param axis: Axis number
        :return: Input value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDiginAxisEnabledIn(self.device, axis)

    def set_digout_limit_override_out(self, axis, value):
        """
        Set the digital output limit override for an axis.
        :param axis: Axis number
        :param value: Output value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetDigoutLimitOverrideOut(self.device, axis, value)

    def get_digout_limit_override_out(self, axis):
        """
        Get the digital output limit override for an axis.
        :param axis: Axis number
        :return: Output value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetDigoutLimitOverrideOut(self.device, axis)

    def set_backlash_width(self, axis, width):
        """
        Set the backlash width for an axis.
        :param axis: Axis number
        :param width: Width value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetBacklashWidth(self.device, axis, width)

    def get_backlash_width(self, axis):
        """
        Get the backlash width for an axis.
        :param axis: Axis number
        :return: Width value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetBacklashWidth(self.device, axis)

    def set_backlash_register(self, axis, register):
        """
        Set the backlash register value for an axis.
        :param axis: Axis number
        :param register: Register value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetBacklashRegister(self.device, axis, register)

    def get_backlash_register(self, axis):
        """
        Get the backlash register value for an axis.
        :param axis: Axis number
        :return: Register value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetBacklashRegister(self.device, axis)

    def set_backlash_acceleration(self, axis, acceleration):
        """
        Set the backlash acceleration for an axis.
        :param axis: Axis number
        :param acceleration: Acceleration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        self.pokeyslib.PK_PEv2_SetBacklashAcceleration(self.device, axis, acceleration)

    def get_backlash_acceleration(self, axis):
        """
        Get the backlash acceleration for an axis.
        :param axis: Axis number
        :return: Acceleration value
        """
        if axis < 0 or axis >= len(self.device.Axes):
            raise ValueError(f"Invalid axis {axis}")
        return self.pokeyslib.PK_PEv2_GetBacklashAcceleration(self.device, axis)

    def set_digin_emergency_pin(self, pin):
        """
        Set the digital input emergency pin.
        :param pin: Pin value
        """
        self.pokeyslib.PK_PEv2_SetDiginEmergencyPin(self.device, pin)

    def get_digin_emergency_pin(self):
        """
        Get the digital input emergency pin.
        :return: Pin value
        """
        return self.pokeyslib.PK_PEv2_GetDiginEmergencyPin(self.device)

    def set_digin_emergency_invert(self, invert):
        """
        Set the digital input emergency invert.
        :param invert: Invert value
        """
        self.pokeyslib.PK_PEv2_SetDiginEmergencyInvert(self.device, invert)

    def get_digin_emergency_invert(self):
        """
        Get the digital input emergency invert.
        :return: Invert value
        """
        return self.pokeyslib.PK_PEv2_GetDiginEmergencyInvert(self.device)

    def set_digout_emergency_pin(self, pin):
        """
        Set the digital output emergency pin.
        :param pin: Pin value
        """
        self.pokeyslib.PK_PEv2_SetDigoutEmergencyPin(self.device, pin)

    def get_digout_emergency_pin(self):
        """
        Get the digital output emergency pin.
        :return: Pin value
        """
        return self.pokeyslib.PK_PEv2_GetDigoutEmergencyPin(self.device)

    def set_digin_probe_pin(self, pin):
        """
        Set the digital input probe pin.
        :param pin: Pin value
        """
        self.pokeyslib.PK_PEv2_SetDiginProbePin(self.device, pin)

    def get_digin_probe_pin(self):
        """
        Get the digital input probe pin.
        :return: Pin value
        """
        return self.pokeyslib.PK_PEv2_GetDiginProbePin(self.device)

    def set_digin_probe_invert(self, invert):
        """
        Set the digital input probe invert.
        :param invert: Invert value
        """
        self.pokeyslib.PK_PEv2_SetDiginProbeInvert(self.device, invert)

    def get_digin_probe_invert(self):
        """
        Get the digital input probe invert.
        :return: Invert value
        """
        return self.pokeyslib.PK_PEv2_GetDiginProbeInvert(self.device)

    def update_input_pins(self):
        """
        Update input pins based on corresponding pointers in pokeyslib.
        """
        self.pokeyslib.PK_PEv2_UpdateInputPins(self.device)

    def update_output_pins(self):
        """
        Update pokeyslib pointers based on corresponding output pins.
        """
        self.pokeyslib.PK_PEv2_UpdateOutputPins(self.device)
