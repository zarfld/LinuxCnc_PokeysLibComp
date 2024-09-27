import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.digital_io import DigitalIO

class TestDigitalIO(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.digital_io = DigitalIO(self.device)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_input(self, mock_pokeyslib):
        self.digital_io.setup(1, 'input')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 2)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_output(self, mock_pokeyslib):
        self.digital_io.setup(1, 'output')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 4)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_invalid_direction(self, mock_pokeyslib):
        with self.assertRaises(ValueError):
            self.digital_io.setup(1, 'invalid')

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_DigitalInputGet.return_value = 1
        value = self.digital_io.fetch(1)
        self.assertEqual(value, 1)
        mock_pokeyslib.PK_SL_DigitalInputGet.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_set(self, mock_pokeyslib):
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_once_with(self.device, 1, 0)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_inversion_parameter(self, mock_pokeyslib):
        self.digital_io.setup(1, 'input')
        self.digital_io.set(1, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

if __name__ == '__main__':
    unittest.main()
