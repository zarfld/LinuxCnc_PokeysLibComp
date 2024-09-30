import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.analog_io import AnalogIO

class TestAnalogIO(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.analog_io = AnalogIO(self.device)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_setup_input(self, mock_pokeyslib):
        self.analog_io.setup(1, 'input')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 1)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_setup_output(self, mock_pokeyslib):
        self.analog_io.setup(1, 'output')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 3)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_setup_invalid_direction(self, mock_pokeyslib):
        with self.assertRaises(ValueError):
            self.analog_io.setup(1, 'invalid')

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_AnalogInputGet.return_value = 123
        value = self.analog_io.fetch(1)
        self.assertEqual(value, 123)
        mock_pokeyslib.PK_SL_AnalogInputGet.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_set(self, mock_pokeyslib):
        self.analog_io.set(1, 456)
        mock_pokeyslib.PK_SL_AnalogOutputSet.assert_called_once_with(self.device, 1, 456)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_fetch_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_AnalogInputGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.fetch(99)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_set_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_AnalogOutputSet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.set(99, 456)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_setup_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_SetPinFunction.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.setup(99, 'input')

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_fetch_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_AnalogInputGet.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.analog_io.fetch(1)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_set_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_AnalogOutputSet.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.analog_io.set(1, -1)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_setup_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_SetPinFunction.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.analog_io.setup(1, 'invalid')

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_analog_input_output_behavior(self, mock_pokeyslib):
        for i in range(7):
            expected_value = 1.23 * i
            self.analog_io.set(i, expected_value)
            actual_value = self.analog_io.fetch(i)
            self.assertEqual(actual_value, expected_value, f"Analog pin {i} expected {expected_value} but got {actual_value}")

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_setup_invalid_direction_error_handling(self, mock_pokeyslib):
        with self.assertRaises(ValueError):
            self.analog_io.setup(1, 'invalid_direction')

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_fetch_invalid_pin_error_handling(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_AnalogInputGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.fetch(99)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_set_invalid_pin_error_handling(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_AnalogOutputSet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.analog_io.set(99, 456)

if __name__ == '__main__':
    unittest.main()
