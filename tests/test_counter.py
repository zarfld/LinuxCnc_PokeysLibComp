import unittest
from unittest.mock import MagicMock
from pokeys_py.counter import Counter

class TestCounter(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.lib = MagicMock()
        self.counter = Counter(self.device, self.lib)

    def test_setup(self):
        self.lib.PK_IsCounterAvailable.return_value = True
        self.counter.setup(1)
        self.lib.PK_IsCounterAvailable.assert_called_once_with(self.device, 1)
        self.lib.PK_PinConfigurationSet.assert_called_once_with(self.device)

    def test_setup_counter_not_available(self):
        self.lib.PK_IsCounterAvailable.return_value = False
        with self.assertRaises(ValueError):
            self.counter.setup(1)

    def test_fetch(self):
        self.device.Pins[1].DigitalCounterValue = 123
        value = self.counter.fetch(1)
        self.assertEqual(value, 123)
        self.lib.PK_DigitalCounterGet.assert_called_once_with(self.device)

    def test_clear(self):
        self.counter.clear(1)
        self.assertEqual(self.device.Pins[1].DigitalCounterValue, 0)
        self.lib.PK_DigitalCounterClear.assert_called_once_with(self.device)

    def test_fetch_invalid_pin(self):
        self.lib.PK_DigitalCounterGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.fetch(99)

    def test_clear_invalid_pin(self):
        self.lib.PK_DigitalCounterClear.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.clear(99)

    def test_setup_invalid_pin(self):
        self.lib.PK_IsCounterAvailable.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.setup(99)

    def test_fetch_invalid_value(self):
        self.lib.PK_DigitalCounterGet.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.counter.fetch(1)

    def test_clear_invalid_value(self):
        self.lib.PK_DigitalCounterClear.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.counter.clear(1)

    def test_setup_invalid_value(self):
        self.lib.PK_IsCounterAvailable.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.counter.setup(1)

    def test_counter_functionality(self):
        for i in range(8):
            # Test correct mapping of counter pins
            expected_value = i * 100
            self.counter.setup(i)
            self.lib.PK_DigitalCounterSet(self.device, i, expected_value)
            actual_value = self.counter.fetch(i)
            self.assertEqual(actual_value, expected_value, f"Counter pin {i} expected {expected_value} but got {actual_value}")

            # Test state changes and expected values
            self.counter.clear(i)
            actual_value = self.counter.fetch(i)
            self.assertEqual(actual_value, 0, f"Counter pin {i} clear expected 0 but got {actual_value}")

    def test_setup_invalid_pin_error_handling(self):
        self.lib.PK_IsCounterAvailable.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.setup(99)

    def test_fetch_invalid_pin_error_handling(self):
        self.lib.PK_DigitalCounterGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.fetch(99)

    def test_clear_invalid_pin_error_handling(self):
        self.lib.PK_DigitalCounterClear.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.clear(99)

if __name__ == '__main__':
    unittest.main()
