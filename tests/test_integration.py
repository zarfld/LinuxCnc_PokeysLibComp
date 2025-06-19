import unittest
from unittest.mock import MagicMock
from pokeys_py import digital_io, analog_io, pwm, counter, ponet, pev2_motion_control

class TestIntegrationPokeysPy(unittest.TestCase):

    def setUp(self):
        self.device = MagicMock()
        self.lib = MagicMock()
        self.digital_io = digital_io.DigitalIO(self.device, self.lib)
        self.analog_io = analog_io.AnalogIO(self.device, self.lib)
        self.pwm = pwm.PWM(self.device, self.lib)
        self.counter = counter.Counter(self.device, self.lib)
        self.ponet = ponet.PoNET(self.device, self.lib)
        self.pev2 = pev2_motion_control.PEv2MotionControl(self.device, self.lib)

    def test_initialization_and_connection(self):
        self.lib.PK_ConnectToDevice.assert_called_once()

    def test_data_exchange(self):
        self.digital_io.setup(1, 'input')
        self.digital_io.fetch(1)
        self.digital_io.set(1, 1)
        self.lib.PK_SL_SetPinFunction.assert_called_with(self.device, 1, 2)
        self.lib.PK_SL_DigitalInputGet.assert_called_with(self.device, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)

    def test_registration_and_communication_with_hal(self):
        self.pev2.setup(1, {'param': 'value'})
        self.pev2.fetch_status()
        self.pev2.set_position(1, 100)
        self.pev2.set_velocity(1, 50)
        self.pev2.start_homing(1)
        self.pev2.cancel_homing(1)
        self.pev2.get_homing_status(1)
        self.lib.PK_PEv2_AxisConfigurationSet.assert_called_with(self.device, 1, {'param': 'value'})
        self.lib.PK_PEv2_StatusGet.assert_called_with(self.device)
        self.lib.PK_PEv2_GetPosition.assert_called_with(self.device)
        self.lib.PK_PEv2_SetPosition.assert_called_with(self.device, 1, 100)
        self.lib.PK_PEv2_SetVelocity.assert_called_with(self.device, 1, 50)
        self.lib.PK_PEv2_StartHoming.assert_called_with(self.device, 1)
        self.lib.PK_PEv2_CancelHoming.assert_called_with(self.device, 1)
        self.lib.PK_PEv2_GetHomingStatus.assert_called_with(self.device, 1)

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

    def test_integration_pokeys_py(self):
        # Test integration of pokeys_py functionalities
        self.digital_io.setup(1, 'input')
        self.digital_io.fetch(1)
        self.digital_io.set(1, 1)
        self.lib.PK_SL_SetPinFunction.assert_called_with(self.device, 1, 2)
        self.lib.PK_SL_DigitalInputGet.assert_called_with(self.device, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)

        self.analog_io.setup(1, 'input')
        self.analog_io.fetch(1)
        self.analog_io.set(1, 1.0)
        self.lib.PK_SL_SetPinFunction.assert_called_with(self.device, 1, 1)
        self.lib.PK_SL_AnalogInputGet.assert_called_with(self.device, 1)
        self.lib.PK_SL_AnalogOutputSet.assert_called_with(self.device, 1, 1.0)

        self.pwm.setup(1, 1000, 50)
        self.pwm.fetch(1)
        self.pwm.set(1, 1000, 50)
        self.lib.PK_PWM_Setup.assert_called_with(self.device, 1, 1000, 50)
        self.lib.PK_PWM_Fetch.assert_called_with(self.device, 1, unittest.mock.ANY, unittest.mock.ANY)
        self.lib.PK_PWM_Set.assert_called_with(self.device, 1, 1000, 50)

        self.counter.setup(1)
        self.counter.fetch(1)
        self.counter.clear(1)
        self.lib.PK_IsCounterAvailable.assert_called_with(self.device, 1)
        self.lib.PK_DigitalCounterGet.assert_called_with(self.device)
        self.lib.PK_DigitalCounterClear.assert_called_with(self.device)

        self.ponet.setup(1)
        self.ponet.fetch(1)
        self.ponet.set(1, 'data')
        self.lib.PK_PoNETGetModuleSettings.assert_called_with(self.device, 1)
        self.lib.PK_PoNETGetModuleStatusRequest.assert_called_with(self.device, 1)
        self.lib.PK_PoNETGetModuleStatus.assert_called_with(self.device, 1)
        self.lib.PK_PoNETSetModuleStatus.assert_called_with(self.device, 1, 'data')

        self.pev2.setup(1, {'param': 'value'})
        self.pev2.fetch_status()
        self.pev2.set_position(1, 100)
        self.pev2.set_velocity(1, 50)
        self.pev2.start_homing(1)
        self.pev2.cancel_homing(1)
        self.pev2.get_homing_status(1)
        self.lib.PK_PEv2_AxisConfigurationSet.assert_called_with(self.device, 1, {'param': 'value'})
        self.lib.PK_PEv2_StatusGet.assert_called_with(self.device)
        self.lib.PK_PEv2_GetPosition.assert_called_with(self.device)
        self.lib.PK_PEv2_SetPosition.assert_called_with(self.device, 1, 100)
        self.lib.PK_PEv2_SetVelocity.assert_called_with(self.device, 1, 50)
        self.lib.PK_PEv2_StartHoming.assert_called_with(self.device, 1)
        self.lib.PK_PEv2_CancelHoming.assert_called_with(self.device, 1)
        self.lib.PK_PEv2_GetHomingStatus.assert_called_with(self.device, 1)

if __name__ == '__main__':
    unittest.main()
