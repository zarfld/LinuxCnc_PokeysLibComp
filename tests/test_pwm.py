import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.pwm import PWM

class TestPWM(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.pwm = PWM(self.device)

    @patch('pokeys_py.pwm.pokeyslib')
    def test_setup(self, mock_pokeyslib):
        self.pwm.setup(1, 1000, 50)
        mock_pokeyslib.PK_PWM_Setup.assert_called_once_with(self.device, 1, 1000, 50)

    @patch('pokeys_py.pwm.pokeyslib')
    def test_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_PWM_Fetch.return_value = (1000, 50)
        frequency, duty_cycle = self.pwm.fetch(1)
        self.assertEqual(frequency, 1000)
        self.assertEqual(duty_cycle, 50)
        mock_pokeyslib.PK_PWM_Fetch.assert_called_once_with(self.device, 1, unittest.mock.ANY, unittest.mock.ANY)

    @patch('pokeys_py.pwm.pokeyslib')
    def test_set(self, mock_pokeyslib):
        self.pwm.set(1, 1000, 50)
        mock_pokeyslib.PK_PWM_Set.assert_called_once_with(self.device, 1, 1000, 50)

if __name__ == '__main__':
    unittest.main()