import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.can_bus import CANBus

class TestCANBus(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.can_bus = CANBus(self.device)

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
        msg = MagicMock()
        status = MagicMock()
        mock_pokeyslib.PK_CANRead.return_value = 0
        result = self.can_bus.read(msg, status)
        self.assertEqual(result, 0)
        mock_pokeyslib.PK_CANRead.assert_called_once_with(self.device, msg, status)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_flush(self, mock_pokeyslib):
        self.can_bus.flush()
        mock_pokeyslib.PK_CANFlush.assert_called_once_with(self.device)

if __name__ == '__main__':
    unittest.main()
