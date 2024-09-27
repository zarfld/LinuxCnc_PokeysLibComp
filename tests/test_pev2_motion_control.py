import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.pev2_motion_control import PEv2MotionControl

class TestPEv2MotionControl(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.pev2_motion_control = PEv2MotionControl(self.device)

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
        mock_pokeyslib.PK_PEv2_StartHoming.side_effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.start_homing(99)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_cancel_homing_invalid_axis(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_CancelHoming.side_effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.cancel_homing(99)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_status_invalid_axis(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingStatus.side_effect = ValueError("Invalid axis")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.get_homing_status(99)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_fetch_status_invalid_device(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_StatusGet.side_effect = ValueError("Invalid device")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.fetch_status()

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_position_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_SetPosition.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.set_position(1, -100)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_set_velocity_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_SetVelocity.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.set_velocity(1, -200)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_start_homing_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_StartHoming.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.start_homing(1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_cancel_homing_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_CancelHoming.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.cancel_homing(1)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_get_homing_status_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_PEv2_GetHomingStatus.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pev2_motion_control.get_homing_status(1)

if __name__ == '__main__':
    unittest.main()
