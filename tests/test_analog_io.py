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

if __name__ == '__main__':
    unittest.main()
