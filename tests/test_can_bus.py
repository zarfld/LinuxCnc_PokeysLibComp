import unittest
from unittest.mock import MagicMock, patch
from pokeys_py import can_bus

class TestCANBus(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.can_bus = can_bus.CANBus(self.device)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_configure(self, mock_pokeyslib):
        self.can_bus.configure(500000)
        mock_pokeyslib.PK_CANConfigure.assert_called_once_with(self.device, 500000)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_register_filter(self, mock_pokeyslib):
        self.can_bus.register_filter(0, 0x123)
        mock_pokeyslib.PK_CANRegisterFilter.assert_called_once_with(self.device, 0, 0x123)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_write(self, mock_pokeyslib):
        msg = MagicMock()
        self.can_bus.write(msg)
        mock_pokeyslib.PK_CANWrite.assert_called_once_with(self.device, msg)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_read(self, mock_pokeyslib):
        mock_pokeyslib.PK_CANRead.return_value = ('msg', 'status')
        msg, status = self.can_bus.read()
        self.assertEqual(msg, 'msg')
        self.assertEqual(status, 'status')
        mock_pokeyslib.PK_CANRead.assert_called_once_with(self.device, unittest.mock.ANY, unittest.mock.ANY)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_flush(self, mock_pokeyslib):
        self.can_bus.flush()
        mock_pokeyslib.PK_CANFlush.assert_called_once_with(self.device)

if __name__ == '__main__':
    unittest.main()
