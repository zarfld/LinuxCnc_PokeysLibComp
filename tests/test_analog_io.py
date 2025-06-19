import unittest
from unittest.mock import MagicMock
from pokeys_py.analog_io import AnalogIO

class TestAnalogIO(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.lib = MagicMock()
        self.analog_io = AnalogIO(self.device, self.lib)

    def test_setup_input(self):
        self.analog_io.setup(1, 'input')
        self.lib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 1)

    def test_setup_output(self):
        self.analog_io.setup(1, 'output')
        self.lib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 3)

    def test_setup_invalid_direction(self):
        with self.assertRaises(ValueError):
            self.analog_io.setup(1, 'invalid')

    def test_fetch(self):
        self.lib.PK_SL_AnalogInputGet.return_value = 123
        value = self.analog_io.fetch(1)
        self.assertEqual(value, 123)
        self.lib.PK_SL_AnalogInputGet.assert_called_once_with(self.device, 1)

    def test_set(self):
        self.analog_io.set(1, 456)
        self.lib.PK_SL_AnalogOutputSet.assert_called_once_with(self.device, 1, 456)

    def test_fetch_invalid_pin(self):
        self.lib.PK_SL_AnalogInputGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.fetch(99)

    def test_set_invalid_pin(self):
        self.lib.PK_SL_AnalogOutputSet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.set(99, 456)

    def test_setup_invalid_pin(self):
        self.lib.PK_SL_SetPinFunction.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.setup(99, 'input')

    def test_fetch_invalid_value(self):
        self.lib.PK_SL_AnalogInputGet.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.analog_io.fetch(1)

    def test_set_invalid_value(self):
        self.lib.PK_SL_AnalogOutputSet.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.analog_io.set(1, -1)

    def test_setup_invalid_value(self):
        self.lib.PK_SL_SetPinFunction.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.analog_io.setup(1, 'invalid')

    def test_analog_input_output_behavior(self):
        for i in range(7):
            expected_value = 1.23 * i
            self.analog_io.set(i, expected_value)
            actual_value = self.analog_io.fetch(i)
            self.assertEqual(actual_value, expected_value, f"Analog pin {i} expected {expected_value} but got {actual_value}")

    def test_setup_invalid_direction_error_handling(self):
        with self.assertRaises(ValueError):
            self.analog_io.setup(1, 'invalid_direction')

    def test_fetch_invalid_pin_error_handling(self):
        self.lib.PK_SL_AnalogInputGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.fetch(99)

    def test_set_invalid_pin_error_handling(self):
        self.lib.PK_SL_AnalogOutputSet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.set(99, 456)

if __name__ == '__main__':
    unittest.main()
