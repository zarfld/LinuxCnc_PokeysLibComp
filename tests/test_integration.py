import unittest
from unittest.mock import MagicMock, patch
from pokeys_py import digital_io, analog_io, pwm, counter, ponet, pev2_motion_control

class TestIntegrationPokeysPy(unittest.TestCase):

    @patch('pokeys_py.digital_io.pokeyslib')
    @patch('pokeys_py.analog_io.pokeyslib')
    @patch('pokeys_py.pwm.pokeyslib')
    @patch('pokeys_py.counter.pokeyslib')
    @patch('pokeys_py.ponet.pokeyslib')
    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def setUp(self, mock_pev2, mock_ponet, mock_counter, mock_pwm, mock_analog, mock_digital):
        self.device = MagicMock()
        self.digital_io = digital_io.DigitalIO(self.device)
        self.analog_io = analog_io.AnalogIO(self.device)
        self.pwm = pwm.PWM(self.device)
        self.counter = counter.Counter(self.device)
        self.ponet = ponet.PoNET(self.device)
        self.pev2 = pev2_motion_control.PEv2MotionControl(self.device)

    def test_initialization_and_connection(self):
        self.device.PK_ConnectToDevice.assert_called_once()

    def test_data_exchange(self):
        self.digital_io.setup(1, 'input')
        self.digital_io.fetch(1)
        self.digital_io.set(1, 1)
        self.device.PK_SL_SetPinFunction.assert_called_with(self.device, 1, 2)
        self.device.PK_SL_DigitalInputGet.assert_called_with(self.device, 1)
        self.device.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)

    def test_registration_and_communication_with_hal(self):
        self.pev2.setup(1, {'param': 'value'})
        self.pev2.fetch_status()
        self.pev2.set_position(1, 100)
        self.pev2.set_velocity(1, 50)
        self.pev2.start_homing(1)
        self.pev2.cancel_homing(1)
        self.pev2.get_homing_status(1)
        self.device.PK_PEv2_AxisConfigurationSet.assert_called_with(self.device, 1, {'param': 'value'})
        self.device.PK_PEv2_StatusGet.assert_called_with(self.device)
        self.device.PK_PEv2_GetPosition.assert_called_with(self.device)
        self.device.PK_PEv2_SetPosition.assert_called_with(self.device, 1, 100)
        self.device.PK_PEv2_SetVelocity.assert_called_with(self.device, 1, 50)
        self.device.PK_PEv2_StartHoming.assert_called_with(self.device, 1)
        self.device.PK_PEv2_CancelHoming.assert_called_with(self.device, 1)
        self.device.PK_PEv2_GetHomingStatus.assert_called_with(self.device, 1)

    def test_error_conditions_and_edge_cases(self):
        # Test invalid pin for digital I/O
        with self.assertRaises(ValueError):
            self.digital_io.setup(99, 'input')
        with self.assertRaises(ValueError):
            self.digital_io.fetch(99)
        with self.assertRaises(ValueError):
            self.digital_io.set(99, 1)

        # Test invalid pin for analog I/O
        with self.assertRaises(ValueError):
            self.analog_io.setup(99, 'input')
        with self.assertRaises(ValueError):
            self.analog_io.fetch(99)
        with self.assertRaises(ValueError):
            self.analog_io.set(99, 1.0)

        # Test invalid pin for PWM
        with self.assertRaises(ValueError):
            self.pwm.setup(99, 1000, 50)
        with self.assertRaises(ValueError):
            self.pwm.fetch(99)
        with self.assertRaises(ValueError):
            self.pwm.set(99, 1000, 50)

        # Test invalid pin for counter
        with self.assertRaises(ValueError):
            self.counter.setup(99)
        with self.assertRaises(ValueError):
            self.counter.fetch(99)
        with self.assertRaises(ValueError):
            self.counter.clear(99)

        # Test invalid module ID for PoNET
        with self.assertRaises(ValueError):
            self.ponet.setup(99)
        with self.assertRaises(ValueError):
            self.ponet.fetch(99)
        with self.assertRaises(ValueError):
            self.ponet.set(99, 'data')

        # Test invalid axis for PEv2 motion control
        with self.assertRaises(ValueError):
            self.pev2.setup(99, {'param': 'value'})
        with self.assertRaises(ValueError):
            self.pev2.set_position(99, 100)
        with self.assertRaises(ValueError):
            self.pev2.set_velocity(99, 50)
        with self.assertRaises(ValueError):
            self.pev2.start_homing(99)
        with self.assertRaises(ValueError):
            self.pev2.cancel_homing(99)
        with self.assertRaises(ValueError):
            self.pev2.get_homing_status(99)

if __name__ == '__main__':
    unittest.main()
