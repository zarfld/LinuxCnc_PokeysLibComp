import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.ponet import PoNET
from pokeys_py.pokeys_poextbus import PoExtBusOC16
from pokeys_py.pokeys_porelay8 import PoRelay8
from pokeys_py.pokeys_ponetkbd48cnc import PoNETkbd48CNC

class TestPeripheralDevices(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.ponet = PoNET(self.device)
        self.poextbus = PoExtBusOC16(self.device)
        self.porelay = PoRelay8(self.device)
        self.ponetkbd = PoNETkbd48CNC(self.device)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_ponet_setup(self, mock_pokeyslib):
        self.ponet.setup(1)
        mock_pokeyslib.PK_PoNETGetModuleSettings.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_ponet_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoNETGetModuleStatus.return_value = 'module_data'
        data = self.ponet.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoNETGetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoNETGetModuleStatus.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_ponet_set(self, mock_pokeyslib):
        self.ponet.set(1, 'data')
        mock_pokeyslib.PK_PoNETSetModuleStatus.assert_called_once_with(self.device, 1, 'data')

    @patch('pokeys_py.pokeys_poextbus.pokeyslib')
    def test_poextbus_setup(self, mock_pokeyslib):
        self.poextbus.setup(1)
        mock_pokeyslib.PK_PoExtBusGetModuleSettings.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pokeys_poextbus.pokeyslib')
    def test_poextbus_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoExtBusGetModuleStatus.return_value = 'module_data'
        data = self.poextbus.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoExtBusGetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoExtBusGetModuleStatus.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pokeys_poextbus.pokeyslib')
    def test_poextbus_set(self, mock_pokeyslib):
        self.poextbus.set(1, 'data')
        mock_pokeyslib.PK_PoExtBusSetModuleStatus.assert_called_once_with(self.device, 1, 'data')

    @patch('pokeys_py.pokeys_porelay8.pokeyslib')
    def test_porelay_setup(self, mock_pokeyslib):
        self.porelay.setup(1)
        mock_pokeyslib.PK_PoRelay8GetModuleSettings.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pokeys_porelay8.pokeyslib')
    def test_porelay_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoRelay8GetModuleStatus.return_value = 'module_data'
        data = self.porelay.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoRelay8GetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoRelay8GetModuleStatus.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pokeys_porelay8.pokeyslib')
    def test_porelay_set(self, mock_pokeyslib):
        self.porelay.set(1, 'data')
        mock_pokeyslib.PK_PoRelay8SetModuleStatus.assert_called_once_with(self.device, 1, 'data')

    @patch('pokeys_py.pokeys_ponetkbd48cnc.pokeyslib')
    def test_ponetkbd_setup(self, mock_pokeyslib):
        self.ponetkbd.setup(1)
        mock_pokeyslib.PK_PoNETkbd48CNCGetModuleSettings.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pokeys_ponetkbd48cnc.pokeyslib')
    def test_ponetkbd_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_PoNETkbd48CNCGetModuleStatus.return_value = 'module_data'
        data = self.ponetkbd.fetch(1)
        self.assertEqual(data, 'module_data')
        mock_pokeyslib.PK_PoNETkbd48CNCGetModuleStatusRequest.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PoNETkbd48CNCGetModuleStatus.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.pokeys_ponetkbd48cnc.pokeyslib')
    def test_ponetkbd_set(self, mock_pokeyslib):
        self.ponetkbd.set(1, 'data')
        mock_pokeyslib.PK_PoNETkbd48CNCSetModuleStatus.assert_called_once_with(self.device, 1, 'data')

if __name__ == '__main__':
    unittest.main()
