import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.digital_io import DigitalIO

class TestDigitalIOInversion(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.digital_io = DigitalIO(self.device)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_inversion_input(self, mock_pokeyslib):
        self.digital_io.setup(1, 'input')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 2)
        mock_pokeyslib.PK_SL_DigitalInputGet.return_value = 1
        value = self.digital_io.fetch(1)
        self.assertEqual(value, 1)
        mock_pokeyslib.PK_SL_DigitalInputGet.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_SL_DigitalInputGet.return_value = 0
        value = self.digital_io.fetch(1)
        self.assertEqual(value, 0)
        mock_pokeyslib.PK_SL_DigitalInputGet.assert_called_with(self.device, 1)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_inversion_output(self, mock_pokeyslib):
        self.digital_io.setup(1, 'output')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 4)
        self.digital_io.set(1, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_once_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

if __name__ == '__main__':
    unittest.main()
