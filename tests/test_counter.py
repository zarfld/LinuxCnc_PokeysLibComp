import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.counter import Counter

class TestCounter(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.counter = Counter(self.device)

    @patch('pokeys_py.counter.pokeyslib')
    def test_setup(self, mock_pokeyslib):
        mock_pokeyslib.PK_IsCounterAvailable.return_value = True
        self.counter.setup(1)
        mock_pokeyslib.PK_IsCounterAvailable.assert_called_once_with(self.device, 1)
        mock_pokeyslib.PK_PinConfigurationSet.assert_called_once_with(self.device)

    @patch('pokeys_py.counter.pokeyslib')
    def test_setup_counter_not_available(self, mock_pokeyslib):
        mock_pokeyslib.PK_IsCounterAvailable.return_value = False
        with self.assertRaises(ValueError):
            self.counter.setup(1)

    @patch('pokeys_py.counter.pokeyslib')
    def test_fetch(self, mock_pokeyslib):
        self.device.Pins[1].DigitalCounterValue = 123
        value = self.counter.fetch(1)
        self.assertEqual(value, 123)
        mock_pokeyslib.PK_DigitalCounterGet.assert_called_once_with(self.device)

    @patch('pokeys_py.counter.pokeyslib')
    def test_clear(self, mock_pokeyslib):
        self.counter.clear(1)
        self.assertEqual(self.device.Pins[1].DigitalCounterValue, 0)
        mock_pokeyslib.PK_DigitalCounterClear.assert_called_once_with(self.device)

    @patch('pokeys_py.counter.pokeyslib')
    def test_fetch_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_DigitalCounterGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.fetch(99)

    @patch('pokeys_py.counter.pokeyslib')
    def test_clear_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_DigitalCounterClear.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.clear(99)

    @patch('pokeys_py.counter.pokeyslib')
    def test_setup_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_IsCounterAvailable.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.setup(99)

    @patch('pokeys_py.counter.pokeyslib')
    def test_fetch_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_DigitalCounterGet.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.counter.fetch(1)

    @patch('pokeys_py.counter.pokeyslib')
    def test_clear_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_DigitalCounterClear.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.counter.clear(1)

    @patch('pokeys_py.counter.pokeyslib')
    def test_setup_invalid_value(self, mock_pokeyslib):
        mock_pokeyslib.PK_IsCounterAvailable.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.counter.setup(1)

    @patch('pokeys_py.counter.pokeyslib')
    def test_counter_functionality(self, mock_pokeyslib):
        for i in range(8):
            # Test correct mapping of counter pins
            expected_value = i * 100
            self.counter.setup(i)
            mock_pokeyslib.PK_DigitalCounterSet(self.device, i, expected_value)
            actual_value = self.counter.fetch(i)
            self.assertEqual(actual_value, expected_value, f"Counter pin {i} expected {expected_value} but got {actual_value}")

            # Test state changes and expected values
            self.counter.clear(i)
            actual_value = self.counter.fetch(i)
            self.assertEqual(actual_value, 0, f"Counter pin {i} clear expected 0 but got {actual_value}")

    @patch('pokeys_py.counter.pokeyslib')
    def test_setup_invalid_pin_error_handling(self, mock_pokeyslib):
        mock_pokeyslib.PK_IsCounterAvailable.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.setup(99)

    @patch('pokeys_py.counter.pokeyslib')
    def test_fetch_invalid_pin_error_handling(self, mock_pokeyslib):
        mock_pokeyslib.PK_DigitalCounterGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.fetch(99)

    @patch('pokeys_py.counter.pokeyslib')
    def test_clear_invalid_pin_error_handling(self, mock_pokeyslib):
        mock_pokeyslib.PK_DigitalCounterClear.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.counter.clear(99)

    @patch('pokeys_py.counter.pokeyslib')
    def test_counter_edge_cases(self, mock_pokeyslib):
        # Test edge case: maximum counter value
        max_value = 2**32 - 1
        self.device.Pins[1].DigitalCounterValue = max_value
        value = self.counter.fetch(1)
        self.assertEqual(value, max_value)
        mock_pokeyslib.PK_DigitalCounterGet.assert_called_with(self.device)

        # Test edge case: counter overflow
        self.device.Pins[1].DigitalCounterValue = max_value + 1
        value = self.counter.fetch(1)
        self.assertEqual(value, 0)  # Assuming counter wraps around
        mock_pokeyslib.PK_DigitalCounterGet.assert_called_with(self.device)

        # Test edge case: negative counter value
        self.device.Pins[1].DigitalCounterValue = -1
        value = self.counter.fetch(1)
        self.assertEqual(value, 0)  # Assuming counter does not support negative values
        mock_pokeyslib.PK_DigitalCounterGet.assert_called_with(self.device)

    @patch('pokeys_py.counter.pokeyslib')
    def test_counter_performance(self, mock_pokeyslib):
        import time
        start_time = time.time()
        for i in range(1000):
            self.counter.fetch(1)
        end_time = time.time()
        duration = end_time - start_time
        self.assertLess(duration, 1.0, "Counter fetch performance test failed")

if __name__ == '__main__':
    unittest.main()
