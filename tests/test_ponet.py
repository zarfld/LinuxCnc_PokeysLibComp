import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.ponet import PoNET

class TestPoNET(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.ponet = PoNET(self.device)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_setup(self, mock_pokeyslib):
        self.ponet.setup(1)
        mock_pokeyslib.PK_PoNETGetModuleSettings.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoNETGetModuleStatus.return_value = 'module_data'
        data = self.ponet.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoNETGetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoNETGetModuleStatus.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_set(self, mock_pokeyslib):
        self.ponet.set(1, 'data')
        mock_pokeyslib.PK_PoNETSetModuleStatus.assert_called_once_with(self.device, 1, 'data')

if __name__ == '__main__':
    unittest.main()
