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

if __name__ == '__main__':
    unittest.main()
