import unittest
from unittest.mock import MagicMock
from pokeys_py.can_bus import CANBus

class TestCANBus(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.lib = MagicMock()
        self.can_bus = CANBus(self.device, self.lib)

    def test_configure(self):
        self.can_bus.configure(500000)
        self.lib.PK_CANConfigure.assert_called_once_with(self.device, 500000)

    def test_register_filter(self):
        self.can_bus.register_filter(0, 0x123)
        self.lib.PK_CANRegisterFilter.assert_called_once_with(self.device, 0, 0x123)

    def test_write(self):
        msg = MagicMock()
        self.can_bus.write(msg)
        self.lib.PK_CANWrite.assert_called_once_with(self.device, msg)

    def test_read(self):
        msg = MagicMock()
        status = MagicMock()
        self.lib.PK_CANRead.return_value = 0
        result = self.can_bus.read(msg, status)
        self.assertEqual(result, 0)
        self.lib.PK_CANRead.assert_called_once_with(self.device, msg, status)

    def test_flush(self):
        self.can_bus.flush()
        self.lib.PK_CANFlush.assert_called_once_with(self.device)

    def test_configure_invalid_baudrate(self):
        self.lib.PK_CANConfigure.side_effect = ValueError("Invalid baudrate")
        with self.assertRaises(ValueError):
            self.can_bus.configure(123456)

    def test_register_filter_invalid_id(self):
        self.lib.PK_CANRegisterFilter.side_effect = ValueError("Invalid filter ID")
        with self.assertRaises(ValueError):
            self.can_bus.register_filter(99, 0x123)

    def test_write_invalid_message(self):
        self.lib.PK_CANWrite.side_effect = ValueError("Invalid message")
        with self.assertRaises(ValueError):
            self.can_bus.write(None)

    def test_read_invalid_message(self):
        self.lib.PK_CANRead.side_effect = ValueError("Invalid message")
        with self.assertRaises(ValueError):
            self.can_bus.read(None, MagicMock())

    def test_flush_error(self):
        self.lib.PK_CANFlush.side_effect = ValueError("Flush error")
        with self.assertRaises(ValueError):
            self.can_bus.flush()

    def test_can_bus_initialization(self):
        self.assertIsInstance(self.can_bus, CANBus)
        self.assertEqual(self.can_bus.device, self.device)

    def test_can_bus_configuration(self):
        self.can_bus.configure(250000)
        self.lib.PK_CANConfigure.assert_called_once_with(self.device, 250000)

    def test_can_bus_filter_registration(self):
        self.can_bus.register_filter(1, 0x456)
        self.lib.PK_CANRegisterFilter.assert_called_once_with(self.device, 1, 0x456)

    def test_can_bus_message_write(self):
        msg = MagicMock()
        self.can_bus.write(msg)
        self.lib.PK_CANWrite.assert_called_once_with(self.device, msg)

    def test_can_bus_message_read(self):
        msg = MagicMock()
        status = MagicMock()
        self.lib.PK_CANRead.return_value = 0
        result = self.can_bus.read(msg, status)
        self.assertEqual(result, 0)
        self.lib.PK_CANRead.assert_called_once_with(self.device, msg, status)

    def test_can_bus_flush(self):
        self.can_bus.flush()
        self.lib.PK_CANFlush.assert_called_once_with(self.device)

    def test_can_bus_invalid_baudrate(self):
        self.lib.PK_CANConfigure.side_effect = ValueError("Invalid baudrate")
        with self.assertRaises(ValueError):
            self.can_bus.configure(123456)

    def test_can_bus_invalid_filter_id(self):
        self.lib.PK_CANRegisterFilter.side_effect = ValueError("Invalid filter ID")
        with self.assertRaises(ValueError):
            self.can_bus.register_filter(99, 0x123)

    def test_can_bus_invalid_message_write(self):
        self.lib.PK_CANWrite.side_effect = ValueError("Invalid message")
        with self.assertRaises(ValueError):
            self.can_bus.write(None)

    def test_can_bus_invalid_message_read(self):
        self.lib.PK_CANRead.side_effect = ValueError("Invalid message")
        with self.assertRaises(ValueError):
            self.can_bus.read(None, MagicMock())

    def test_can_bus_flush_error(self):
        self.lib.PK_CANFlush.side_effect = ValueError("Flush error")
        with self.assertRaises(ValueError):
            self.can_bus.flush()

if __name__ == '__main__':
    unittest.main()
