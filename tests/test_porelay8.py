import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.porelay8 import PoRelay8

class TestPoRelay8(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.porelay8 = PoRelay8(self.device)

    @patch('pokeys_py.porelay8.pokeyslib')
    def test_setup(self, mock_pokeyslib):
        self.porelay8.setup(1)
        mock_pokeyslib.PK_PoRelay8_GetModuleSettings.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.porelay8.pokeyslib')
    def test_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoRelay8_GetModuleStatus.return_value = 'module_data'
        data = self.porelay8.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoRelay8_GetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoRelay8_GetModuleStatus.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.porelay8.pokeyslib')
    def test_set(self, mock_pokeyslib):
        self.porelay8.set(1, 'data')
        mock_pokeyslib.PK_PoRelay8_SetModuleStatus.assert_called_once_with(self.device, 1, 'data')

    @patch('pokeys_py.porelay8.pokeyslib')
    def test_fetch_invalid_module_id(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoRelay8_GetModuleStatusRequest.side_effect = ValueError("Invalid module ID")
        with self.assertRaises(ValueError):
            self.porelay8.fetch(99)

    @patch('pokeys_py.porelay8.pokeyslib')
    def test_set_invalid_module_id(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoRelay8_SetModuleStatus.side_effect = ValueError("Invalid module ID")
        with self.assertRaises(ValueError):
            self.porelay8.set(99, 'data')

    @patch('pokeys_py.porelay8.pokeyslib')
    def test_setup_invalid_module_id(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoRelay8_GetModuleSettings.side_effect = ValueError("Invalid module ID")
        with self.assertRaises(ValueError):
            self.porelay8.setup(99)

    @patch('pokeys_py.porelay8.pokeyslib')
    def test_porelay8_functionality(self, mock_pokeyslib):
        for i in range(8):
            expected_value = i % 2
            self.porelay8.set(i, expected_value)
            actual_value = self.porelay8.fetch(i)
            self.assertEqual(actual_value, expected_value, f"PoRelay8 pin {i} expected {expected_value} but got {actual_value}")

            self.porelay8.set(i, not expected_value)
            actual_value = self.porelay8.fetch(i)
            self.assertEqual(actual_value, not expected_value, f"PoRelay8 pin {i} invert expected {not expected_value} but got {actual_value}")

if __name__ == '__main__':
    unittest.main()
