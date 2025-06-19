import unittest
from unittest.mock import MagicMock
from pokeys_py.pwm import PWM

class TestPWM(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.lib = MagicMock()
        self.pwm = PWM(self.device, self.lib)

    def test_setup(self):
        self.pwm.setup(1, 1000, 50)
        self.lib.PK_PWM_Setup.assert_called_once_with(self.device, 1, 1000, 50)

    def test_fetch(self):
        def fetch_side_effect(device, channel, freq_ptr, duty_ptr):
            freq_ptr._obj.value = 1000
            duty_ptr._obj.value = 50

        self.lib.PK_PWM_Fetch.side_effect = fetch_side_effect
        frequency, duty_cycle = self.pwm.fetch(1)
        self.assertEqual(frequency, 1000)
        self.assertEqual(duty_cycle, 50)
        self.lib.PK_PWM_Fetch.assert_called_once_with(self.device, 1, unittest.mock.ANY, unittest.mock.ANY)

    def test_set(self):
        self.pwm.set(1, 1000, 50)
        self.lib.PK_PWM_Set.assert_called_once_with(self.device, 1, 1000, 50)

    def test_setup_invalid_channel(self):
        self.lib.PK_PWM_Setup.side_effect = ValueError("Invalid channel")
        with self.assertRaises(ValueError):
            self.pwm.setup(99, 1000, 50)

    def test_fetch_invalid_channel(self):
        self.lib.PK_PWM_Fetch.side_effect = ValueError("Invalid channel")
        with self.assertRaises(ValueError):
            self.pwm.fetch(99)

    def test_set_invalid_channel(self):
        self.lib.PK_PWM_Set.side_effect = ValueError("Invalid channel")
        with self.assertRaises(ValueError):
            self.pwm.set(99, 1000, 50)

    def test_setup_invalid_value(self):
        self.lib.PK_PWM_Setup.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.pwm.setup(1, -1000, 50)


    def test_pwm_functionalities(self):
        for i in range(6):
            expected_duty_cycle = 50.0 * i
            self.pwm.set(i, 1000, expected_duty_cycle)
            actual_duty_cycle = self.pwm.fetch(i)[1]
            self.assertEqual(actual_duty_cycle, expected_duty_cycle, f"PWM pin {i} expected {expected_duty_cycle} but got {actual_duty_cycle}")

            self.pwm.set(i, 1000, 100.0 - expected_duty_cycle)
            actual_duty_cycle = self.pwm.fetch(i)[1]
            self.assertEqual(actual_duty_cycle, 100.0 - expected_duty_cycle, f"PWM pin {i} state change expected {100.0 - expected_duty_cycle} but got {actual_duty_cycle}")

if __name__ == '__main__':
    unittest.main()
