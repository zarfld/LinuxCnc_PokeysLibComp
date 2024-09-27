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

if __name__ == '__main__':
    unittest.main()
