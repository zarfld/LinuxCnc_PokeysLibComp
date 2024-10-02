import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.pev2_motion_control import PEv2MotionControl

class TestPEv2MotionControl(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.pev2_motion_control = PEv2MotionControl(self.device, pokeyslib)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_setup(self, mock_pokeyslib):
        parameters = {'param1': 1, 'param2': 2}
        self.pev2_motion_control.setup(1, parameters)
        mock_pokeyslib.PK_PEv2_AxisConfigurationSet.assert_called_once_with(self.device, 1, parameters)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_fetch_status(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_StatusGet.return_value = 'status'
        mock_pokeyslib.PK_PEv2_GetPosition.return_value = 'position'
        status, position = self.pev2_motion_control.fetch_status()
        self.assertEqual(status, 'status')
        self.assertEqual(position, 'position')
        mock_pokeyslib.PK_PEv2_StatusGet.assert_called_once_with(self.device)
        mock_pokeyslib.PK_PEv2_GetPosition.assert_called_once_with(self.device)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_position(self, mock_pokeyslib):
        self.pev2_motion_control.set_position(1, 100)
        mock_pokeyslib.PK_PEv2_SetPosition.assert_called_once_with(self.device, 1, 100)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_velocity(self, mock_pokeyslib):
        self.pev2_motion_control.set_velocity(1, 200)
        mock_pokeyslib.PK_PEv2_SetVelocity.assert_called_once_with(self.device, 1, 200)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_start_homing(self, mock_pokeyslib):
        self.pev2_motion_control.start_homing(1)
        mock_pokeyslib.PK_PEv2_StartHoming.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_cancel_homing(self, mock_pokeyslib):
        self.pev2_motion_control.cancel_homing(1)
        mock_pokeyslib.PK_PEv2_CancelHoming.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_status(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingStatus.return_value = 'homing_status'
        homing_status = self.pev2_motion_control.get_homing_status(1)
        self.assertEqual(homing_status, 'homing_status')
        mock_pokeyslib.PK_PEv2_GetHomingStatus.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_manage_homing_signals(self, mock_pokeyslib):
        self.pev2_motion_control.get_homing_status = MagicMock(return_value=11)
        self.pev2_motion_control.start_homing = MagicMock()
        self.pev2_motion_control.set_velocity = MagicMock()
        self.pev2_motion_control.set_position = MagicMock()
        self.pev2_motion_control.cancel_homing = MagicMock()

        self.pev2_motion_control.manage_homing_signals(1)
        self.pev2_motion_control.start_homing.assert_called_once_with(1)

        self.pev2_motion_control.get_homing_status = MagicMock(return_value=12)
        self.pev2_motion_control.manage_homing_signals(1)
        self.pev2_motion_control.set_velocity.assert_called_once_with(1, 0.5)

        self.pev2_motion_control.get_homing_status = MagicMock(return_value=13)
        self.pev2_motion_control.manage_homing_signals(1)
        self.pev2_motion_control.set_velocity.assert_called_with(1, 0.1)

        self.pev2_motion_control.get_homing_status = MagicMock(return_value=10)
        self.pev2_motion_control.manage_homing_signals(1)
        self.pev2_motion_control.set_position.assert_called_once_with(1, 0)

        self.pev2_motion_control.get_homing_status = MagicMock(return_value=0)
        self.pev2_motion_control.manage_homing_signals(1)
        self.pev2_motion_control.cancel_homing.assert_called_once_with(1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_position_invalid_axis(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_SetPosition.side_effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.set_position(99, 100)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_velocity_invalid_axis(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_SetVelocity.side_effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.set_velocity(99, 200)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_start_homing_invalid_axis(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_StartHoming.side.effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.start_homing(99)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_cancel_homing_invalid_axis(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_CancelHoming.side.effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.cancel_homing(99)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_status_invalid_axis(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingStatus.side.effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.get_homing_status(99)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_fetch_status_invalid_device(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_StatusGet.side.effect = ValueError("Invalid device")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.fetch_status()

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_position_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_SetPosition.side.effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.set_position(1, -100)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_velocity_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_SetVelocity.side.effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.set_velocity(1, -200)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_start_homing_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_StartHoming.side.effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.start_homing(1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_cancel_homing_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_CancelHoming.side.effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.cancel_homing(1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_status_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingStatus.side.effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.get_homing_status(1)

    def test_position_feedback(self):
        for i in range(8):
            expected_position = 1.23 * i
            self.pev2_motion_control.set_position(i, expected_position)
            actual_position = self.pev2_motion_control.fetch_status()[1]
            self.assertEqual(actual_position, expected_position, f"Position feedback pin {i} expected {expected_position} but got {actual_position}")

    def test_velocity_commands(self):
        for i in range(8):
            expected_velocity = 0.5 * i
            self.pev2_motion_control.set_velocity(i, expected_velocity)
            actual_velocity = self.pev2_motion_control.fetch_status()[0]
            self.assertEqual(actual_velocity, expected_velocity, f"Velocity command pin {i} expected {expected_velocity} but got {actual_velocity}")

    def test_homing_routines(self):
        for i in range(8):
            self.pev2_motion_control.start_homing(i)
            homing_status = self.pev2_motion_control.get_homing_status(i)
            self.assertEqual(homing_status, 11, f"Homing status pin {i} expected 11 but got {homing_status}")
            self.pev2_motion_control.cancel_homing(i)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_add_pins(self, mock_pokeyslib):
        self.pev2_motion_control.add_pins()
        mock_pokeyslib.PK_PEv2_AddPins.assert_called_once_with(self.device)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_handle_new_pins(self, mock_pokeyslib):
        self.pev2_motion_control.handle_new_pins()
        mock_pokeyslib.PK_PEv2_HandleNewPins.assert_called_once_with(self.device)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_initialize_pins(self, mock_pokeyslib):
        self.pev2_motion_control.initialize_pins()
        mock_pokeyslib.PK_PEv2_InitializePins.assert_called_once_with(self.device)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_create_required_pins(self, mock_pokeyslib):
        self.pev2_motion_control.create_required_pins()
        mock_pokeyslib.PK_PEv2_CreateRequiredPins.assert_called_once_with(self.device)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_update_documentation(self, mock_pokeyslib):
        self.pev2_motion_control.update_documentation()
        mock_pokeyslib.PK_PEv2_UpdateDocumentation.assert_called_once_with(self.device)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_axis_state(self, mock_pokeyslib):
        self.pev2_motion_control.set_axis_state(1, 'state')
        mock_pokeyslib.PK_PEv2_SetAxisState.assert_called_once_with(self.device, 1, 'state')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_axis_state(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetAxisState.return_value = 'state'
        state = self.pev2_motion_control.get_axis_state(1)
        self.assertEqual(state, 'state')
        mock_pokeyslib.PK_PEv2_GetAxisState.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_axis_config(self, mock_pokeyslib):
        self.pev2_motion_control.set_axis_config(1, 'config')
        mock_pokeyslib.PK_PEv2_SetAxisConfig.assert_called_once_with(self.device, 1, 'config')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_axis_config(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetAxisConfig.return_value = 'config'
        config = self.pev2_motion_control.get_axis_config(1)
        self.assertEqual(config, 'config')
        mock_pokeyslib.PK_PEv2_GetAxisConfig.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_axis_switch_config(self, mock_pokeyslib):
        self.pev2_motion_control.set_axis_switch_config(1, 'switch_config')
        mock_pokeyslib.PK_PEv2_SetAxisSwitchConfig.assert_called_once_with(self.device, 1, 'switch_config')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_axis_switch_config(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetAxisSwitchConfig.return_value = 'switch_config'
        switch_config = self.pev2_motion_control.get_axis_switch_config(1)
        self.assertEqual(switch_config, 'switch_config')
        mock_pokeyslib.PK_PEv2_GetAxisSwitchConfig.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_soft_limit_maximum(self, mock_pokeyslib):
        self.pev2_motion_control.set_soft_limit_maximum(1, 'limit')
        mock_pokeyslib.PK_PEv2_SetSoftLimitMaximum.assert_called_once_with(self.device, 1, 'limit')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_soft_limit_maximum(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetSoftLimitMaximum.return_value = 'limit'
        limit = self.pev2_motion_control.get_soft_limit_maximum(1)
        self.assertEqual(limit, 'limit')
        mock_pokeyslib.PK_PEv2_GetSoftLimitMaximum.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_soft_limit_minimum(self, mock_pokeyslib):
        self.pev2_motion_control.set_soft_limit_minimum(1, 'limit')
        mock_pokeyslib.PK_PEv2_SetSoftLimitMinimum.assert_called_once_with(self.device, 1, 'limit')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_soft_limit_minimum(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetSoftLimitMinimum.return_value = 'limit'
        limit = self.pev2_motion_control.get_soft_limit_minimum(1)
        self.assertEqual(limit, 'limit')
        mock_pokeyslib.PK_PEv2_GetSoftLimitMinimum.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_homing_speed(self, mock_pokeyslib):
        self.pev2_motion_control.set_homing_speed(1, 'speed')
        mock_pokeyslib.PK_PEv2_SetHomingSpeed.assert_called_once_with(self.device, 1, 'speed')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_speed(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingSpeed.return_value = 'speed'
        speed = self.pev2_motion_control.get_homing_speed(1)
        self.assertEqual(speed, 'speed')
        mock_pokeyslib.PK_PEv2_GetHomingSpeed.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_homing_return_speed(self, mock_pokeyslib):
        self.pev2_motion_control.set_homing_return_speed(1, 'speed')
        mock_pokeyslib.PK_PEv2_SetHomingReturnSpeed.assert_called_once_with(self.device, 1, 'speed')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_return_speed(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingReturnSpeed.return_value = 'speed'
        speed = self.pev2_motion_control.get_homing_return_speed(1)
        self.assertEqual(speed, 'speed')
        mock_pokeyslib.PK_PEv2_GetHomingReturnSpeed.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_homing_algorithm(self, mock_pokeyslib):
        self.pev2_motion_control.set_homing_algorithm(1, 'algorithm')
        mock_pokeyslib.PK_PEv2_SetHomingAlgorithm.assert_called_once_with(self.device, 1, 'algorithm')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_algorithm(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingAlgorithm.return_value = 'algorithm'
        algorithm = self.pev2_motion_control.get_homing_algorithm(1)
        self.assertEqual(algorithm, 'algorithm')
        mock_pokeyslib.PK_PEv2_GetHomingAlgorithm.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_home_offsets(self, mock_pokeyslib):
        self.pev2_motion_control.set_home_offsets(1, 'offset')
        mock_pokeyslib.PK_PEv2_SetHomeOffsets.assert_called_once_with(self.device, 1, 'offset')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_home_offsets(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomeOffsets.return_value = 'offset'
        offset = self.pev2_motion_control.get_home_offsets(1)
        self.assertEqual(offset, 'offset')
        mock_pokeyslib.PK_PEv2_GetHomeOffsets.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_probe_position(self, mock_pokeyslib):
        self.pev2_motion_control.set_probe_position(1, 'position')
        mock_pokeyslib.PK_PEv2_SetProbePosition.assert_called_once_with(self.device, 1, 'position')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_probe_position(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetProbePosition.return_value = 'position'
        position = self.pev2_motion_control.get_probe_position(1)
        self.assertEqual(position, 'position')
        mock_pokeyslib.PK_PEv2_GetProbePosition.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_probe_max_position(self, mock_pokeyslib):
        self.pev2_motion_control.set_probe_max_position(1, 'position')
        mock_pokeyslib.PK_PEv2_SetProbeMaxPosition.assert_called_once_with(self.device, 1, 'position')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_probe_max_position(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetProbeMaxPosition.return_value = 'position'
        position = self.pev2_motion_control.get_probe_max_position(1)
        self.assertEqual(position, 'position')
        mock_pokeyslib.PK_PEv2_GetProbeMaxPosition.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_current_position(self, mock_pokeyslib):
        self.pev2_motion_control.set_current_position(1, 'position')
        mock_pokeyslib.PK_PEv2_SetCurrentPosition.assert_called_once_with(self.device, 1, 'position')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_current_position(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetCurrentPosition.return_value = 'position'
        position = self.pev2_motion_control.get_current_position(1)
        self.assertEqual(position, 'position')
        mock_pokeyslib.PK_PEv2_GetCurrentPosition.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_position_setup(self, mock_pokeyslib):
        self.pev2_motion_control.set_position_setup(1, 'position')
        mock_pokeyslib.PK_PEv2_SetPositionSetup.assert_called_once_with(self.device, 1, 'position')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_position_setup(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetPositionSetup.return_value = 'position'
        position = self.pev2_motion_control.get_position_setup(1)
        self.assertEqual(position, 'position')
        mock_pokeyslib.PK_PEv2_GetPositionSetup.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_reference_position_speed(self, mock_pokeyslib):
        self.pev2_motion_control.set_reference_position_speed(1, 'speed')
        mock_pokeyslib.PK_PEv2_SetReferencePositionSpeed.assert_called_once_with(self.device, 1, 'speed')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_reference_position_speed(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetReferencePositionSpeed.return_value = 'speed'
        speed = self.pev2_motion_control.get_reference_position_speed(1)
        self.assertEqual(speed, 'speed')
        mock_pokeyslib.PK_PEv2_GetReferencePositionSpeed.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_max_speed(self, mock_pokeyslib):
        self.pev2_motion_control.set_max_speed(1, 'speed')
        mock_pokeyslib.PK_PEv2_SetMaxSpeed.assert_called_once_with(self.device, 1, 'speed')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_max_speed(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetMaxSpeed.return_value = 'speed'
        speed = self.pev2_motion_control.get_max_speed(1)
        self.assertEqual(speed, 'speed')
        mock_pokeyslib.PK_PEv2_GetMaxSpeed.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_max_acceleration(self, mock_pokeyslib):
        self.pev2_motion_control.set_max_acceleration(1, 'acceleration')
        mock_pokeyslib.PK_PEv2_SetMaxAcceleration.assert_called_once_with(self.device, 1, 'acceleration')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_max_acceleration(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetMaxAcceleration.return_value = 'acceleration'
        acceleration = self.pev2_motion_control.get_max_acceleration(1)
        self.assertEqual(acceleration, 'acceleration')
        mock_pokeyslib.PK_PEv2_GetMaxAcceleration.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_mpg_jog_multiplier(self, mock_pokeyslib):
        self.pev2_motion_control.set_mpg_jog_multiplier(1, 'multiplier')
        mock_pokeyslib.PK_PEv2_SetMPGJogMultiplier.assert_called_once_with(self.device, 1, 'multiplier')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_mpg_jog_multiplier(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetMPGJogMultiplier.return_value = 'multiplier'
        multiplier = self.pev2_motion_control.get_mpg_jog_multiplier(1)
        self.assertEqual(multiplier, 'multiplier')
        mock_pokeyslib.PK_PEv2_GetMPGJogMultiplier.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_mpg_jog_encoder(self, mock_pokeyslib):
        self.pev2_motion_control.set_mpg_jog_encoder(1, 'encoder')
        mock_pokeyslib.PK_PEv2_SetMPGJogEncoder.assert_called_once_with(self.device, 1, 'encoder')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_mpg_jog_encoder(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetMPGJogEncoder.return_value = 'encoder'
        encoder = self.pev2_motion_control.get_mpg_jog_encoder(1)
        self.assertEqual(encoder, 'encoder')
        mock_pokeyslib.PK_PEv2_GetMPGJogEncoder.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_mpg_jog_divider(self, mock_pokeyslib):
        self.pev2_motion_control.set_mpg_jog_divider(1, 'divider')
        mock_pokeyslib.PK_PEv2_SetMPGJogDivider.assert_called_once_with(self.device, 1, 'divider')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_mpg_jog_divider(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetMPGJogDivider.return_value = 'divider'
        divider = self.pev2_motion_control.get_mpg_jog_divider(1)
        self.assertEqual(divider, 'divider')
        mock_pokeyslib.PK_PEv2_GetMPGJogDivider.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_limit_n_in(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_limit_n_in(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginLimitNIn.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_limit_n_in(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginLimitNIn.return_value = 'value'
        value = self.pev2_motion_control.get_digin_limit_n_in(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginLimitNIn.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_limit_n_in_not(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_limit_n_in_not(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginLimitNInNot.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_limit_n_in_not(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginLimitNInNot.return_value = 'value'
        value = self.pev2_motion_control.get_digin_limit_n_in_not(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginLimitNInNot.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_limit_n_dedicated_input(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_limit_n_dedicated_input(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginLimitNDedicatedInput.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_limit_n_dedicated_input(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginLimitNDedicatedInput.return_value = 'value'
        value = self.pev2_motion_control.get_digin_limit_n_dedicated_input(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginLimitNDedicatedInput.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_limit_p_in(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_limit_p_in(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginLimitPIn.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_limit_p_in(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginLimitPIn.return_value = 'value'
        value = self.pev2_motion_control.get_digin_limit_p_in(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginLimitPIn.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_limit_p_in_not(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_limit_p_in_not(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginLimitPInNot.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_limit_p_in_not(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginLimitPInNot.return_value = 'value'
        value = self.pev2_motion_control.get_digin_limit_p_in_not(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginLimitPInNot.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_limit_p_dedicated_input(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_limit_p_dedicated_input(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginLimitPDedicatedInput.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_limit_p_dedicated_input(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginLimitPDedicatedInput.return_value = 'value'
        value = self.pev2_motion_control.get_digin_limit_p_dedicated_input(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginLimitPDedicatedInput.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digout_axis_enable_out(self, mock_pokeyslib):
        self.pev2_motion_control.set_digout_axis_enable_out(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDigoutAxisEnableOut.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digout_axis_enable_out(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDigoutAxisEnableOut.return_value = 'value'
        value = self.pev2_motion_control.get_digout_axis_enable_out(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDigoutAxisEnableOut.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_home_in(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_home_in(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginHomeIn.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_home_in(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginHomeIn.return_value = 'value'
        value = self.pev2_motion_control.get_digin_home_in(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginHomeIn.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_home_in_not(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_home_in_not(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginHomeInNot.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_home_in_not(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginHomeInNot.return_value = 'value'
        value = self.pev2_motion_control.get_digin_home_in_not(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginHomeInNot.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_home_dedicated_input(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_home_dedicated_input(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginHomeDedicatedInput.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_home_dedicated_input(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginHomeDedicatedInput.return_value = 'value'
        value = self.pev2_motion_control.get_digin_home_dedicated_input(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginHomeDedicatedInput.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_home_back_off_distance(self, mock_pokeyslib):
        self.pev2_motion_control.set_home_back_off_distance(1, 'distance')
        mock_pokeyslib.PK_PEv2_SetHomeBackOffDistance.assert_called_once_with(self.device, 1, 'distance')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_home_back_off_distance(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomeBackOffDistance.return_value = 'distance'
        distance = self.pev2_motion_control.get_home_back_off_distance(1)
        self.assertEqual(distance, 'distance')
        mock_pokeyslib.PK_PEv2_GetHomeBackOffDistance.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_error_in(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_error_in(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginErrorIn.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_error_in(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginErrorIn.return_value = 'value'
        value = self.pev2_motion_control.get_digin_error_in(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginErrorIn.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_error_in_not(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_error_in_not(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginErrorInNot.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_error_in_not(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginErrorInNot.return_value = 'value'
        value = self.pev2_motion_control.get_digin_error_in_not(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginErrorInNot.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_probe_in(self, mock_pokeyslib):
        self.pev2_motion_control.set_digin_probe_in(1, 'value')
        mock_pokeyslib.PK_PEv2_SetDiginProbeIn.assert_called_once_with(self.device, 1, 'value')

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_digin_probe_in(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetDiginProbeIn.return_value = 'value'
        value = self.pev2_motion_control.get_digin_probe_in(1)
        self.assertEqual(value, 'value')
        mock_pokeyslib.PK_PEv2_GetDiginProbeIn.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_digin_probe_in_not(self, mock_pokeyslib):
        self.pev2_motion
