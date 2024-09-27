import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.ponet import PoNET
from pokeys_py.pokeys_poextbus import PoExtBusOC16
from pokeys_py.pokeys_porelay8 import PoRelay8

class TestPeripheralDevices(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.ponet = PoNET(self.device)
        self.poextbus = PoExtBusOC16(self.device)
        self.porelay = PoRelay8(self.device)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_ponetkbd48cnc(self, mock_pokeyslib):
        self.ponet.setup(1)
        mock_pokeyslib.PK_PoNETGetModuleSettings.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoNETGetModuleStatusRequest.return_value = 'module_data'
        data = self.ponet.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoNETGetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoNETGetModuleStatus.assert_called_once_with(self.device, 1)
        self.ponet.set(1, 'data')
        mock_pokeyslib.PK_PoNETSetModuleStatus.assert_called_once_with(self.device, 1, 'data')

    @patch('pokeys_py.pokeys_poextbus.pokeyslib')
    def test_poextbusoc16(self, mock_pokeyslib):
        self.poextbus.setup(1)
        mock_pokeyslib.PK_PoExtBusGetModuleSettings.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoExtBusGetModuleStatusRequest.return_value = 'module_data'
        data = self.poextbus.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoExtBusGetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoExtBusGetModuleStatus.assert_called_once_with(self.device, 1)
        self.poextbus.set(1, 'data')
        mock_pokeyslib.PK_PoExtBusSetModuleStatus.assert_called_once_with(self.device, 1, 'data')

    @patch('pokeys_py.pokeys_porelay8.pokeyslib')
    def test_porelay8(self, mock_pokeyslib):
        self.porelay.setup(1)
        mock_pokeyslib.PK_PoRelay8GetModuleSettings.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoRelay8GetModuleStatusRequest.return_value = 'module_data'
        data = self.porelay.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoRelay8GetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoRelay8GetModuleStatus.assert_called_once_with(self.device, 1)
        self.porelay.set(1, 'data')
        mock_pokeyslib.PK_PoRelay8SetModuleStatus.assert_called_once_with(self.device, 1, 'data')

if __name__ == '__main__':
    unittest.main()
