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

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_configure_invalid_baudrate(self, mock_pokeyslib):
        mock_pokeyslib.PK_CANConfigure.side_effect = ValueError("Invalid baudrate")
        with self.assertRaises(ValueError):
            self.can_bus.configure(123456)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_register_filter_invalid_id(self, mock_pokeyslib):
        mock_pokeyslib.PK_CANRegisterFilter.side_effect = ValueError("Invalid filter ID")
        with self.assertRaises(ValueError):
            self.can_bus.register_filter(99, 0x123)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_write_invalid_message(self, mock_pokeyslib):
        mock_pokeyslib.PK_CANWrite.side_effect = ValueError("Invalid message")
        with self.assertRaises(ValueError):
            self.can_bus.write(None)

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_read_invalid_message(self, mock_pokeyslib):
        mock_pokeyslib.PK_CANRead.side_effect = ValueError("Invalid message")
        with self.assertRaises(ValueError):
            self.can_bus.read(None, MagicMock())

    @patch('pokeys_py.can_bus.pokeyslib')
    def test_flush_error(self, mock_pokeyslib):
        mock_pokeyslib.PK_CANFlush.side_effect = ValueError("Flush error")
        with self.assertRaises(ValueError):
            self.can_bus.flush()

if __name__ == '__main__':
    unittest.main()
