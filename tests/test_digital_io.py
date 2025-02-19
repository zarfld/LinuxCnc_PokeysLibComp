import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.digital_io import DigitalIO

class TestDigitalIO(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.digital_io = DigitalIO(self.device, pokeyslib)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_input(self, mock_pokeyslib):
        self.digital_io.setup(1, 'input')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 2)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_output(self, mock_pokeyslib):
        self.digital_io.setup(1, 'output')
        mock_pokeyslib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 4)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_invalid_direction(self, mock_pokeyslib):
        with self.assertRaises(ValueError):
            self.digital_io.setup(1, 'invalid')

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_fetch(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_DigitalInputGet.return_value = 1
        value = self.digital_io.fetch(1)
        self.assertEqual(value, 1)
        mock_pokeyslib.PK_SL_DigitalInputGet.assert_called_once_with(self.device, 1)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_set(self, mock_pokeyslib):
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_once_with(self.device, 1, 0)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_inversion_parameter(self, mock_pokeyslib):
        self.digital_io.setup(1, 'input')
        self.digital_io.set(1, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_inversion_parameter_handling(self, mock_pokeyslib):
        self.digital_io.setup(1, 'input')
        self.digital_io.set(1, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)
        self.digital_io.setup(1, 'output')
        self.digital_io.set(1, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_digital_io_behavior(self, mock_pokeyslib):
        for i in range(55):
            expected_value = i % 2
            self.digital_io.set(i, expected_value)
            actual_value = self.digital_io.fetch(i)
            self.assertEqual(actual_value, expected_value, f"Digital pin {i} expected {expected_value} but got {actual_value}")

            self.digital_io.set(i, not expected_value)
            actual_value = self.digital_io.fetch(i)
            self.assertEqual(actual_value, not expected_value, f"Digital pin {i} invert expected {not expected_value} but got {actual_value}")

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_SetPinFunction.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.digital_io.setup(99, 'input')

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_fetch_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_DigitalInputGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.digital_io.fetch(99)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_set_invalid_pin(self, mock_pokeyslib):
        mock_pokeyslib.PK_SL_DigitalOutputSet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.digital_io.set(99, 0)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_hal_pins_mapped_to_signals(self, mock_pokeyslib):
        # Ensure HAL pins are created and mapped to signals in tests/linuxcnc_config.hal
        self.digital_io.setup(0, 'input')
        self.digital_io.set(0, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 1)
        self.digital_io.set(0, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 0)
        self.digital_io.setup(0, 'output')
        self.digital_io.set(0, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 1)
        self.digital_io.set(0, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 0)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_digin(self, mock_pokeyslib):
        self.digital_io.setup_digin(0, 1)
        mock_pokeyslib.PK_HAL_SetPin.assert_any_call("pokeys.0.digin.1.in", mock_pokeyslib.PK_SL_DigitalInputGet.return_value)
        mock_pokeyslib.PK_HAL_SetPin.assert_any_call("pokeys.0.digin.1.in-not", not mock_pokeyslib.PK_SL_DigitalInputGet.return_value)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_setup_digout(self, mock_pokeyslib):
        self.digital_io.setup_digout(0, 1)
        mock_pokeyslib.PK_HAL_SetPin.assert_any_call("pokeys.0.digout.1.out", mock_pokeyslib.PK_SL_DigitalInputGet.return_value)
        mock_pokeyslib.PK_HAL_SetPin.assert_any_call("pokeys.0.digout.1.out-not", not mock_pokeyslib.PK_SL_DigitalInputGet.return_value)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_digital_io_parameters(self, mock_pokeyslib):
        self.digital_io.setup(1, 'input')
        self.digital_io.set(1, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)
        self.digital_io.setup(1, 'output')
        self.digital_io.set(1, 1)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        mock_pokeyslib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_digital_input_hal_interface(self, mock_pokeyslib):
        for pin_id in range(55):
            self.digital_io.setup_digin(0, pin_id)
            mock_pokeyslib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digin.{pin_id}.in", mock_pokeyslib.PK_SL_DigitalInputGet.return_value)
            mock_pokeyslib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digin.{pin_id}.in-not", not mock_pokeyslib.PK_SL_DigitalInputGet.return_value)

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_digital_output_hal_interface(self, mock_pokeyslib):
        for pin_id in range(55):
            self.digital_io.setup_digout(0, pin_id)
            mock_pokeyslib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digout.{pin_id}.out", mock_pokeyslib.PK_SL_DigitalInputGet.return_value)
            mock_pokeyslib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digout.{pin_id}.out-not", not mock_pokeyslib.PK_SL_DigitalInputGet.return_value)

if __name__ == '__main__':
    unittest.main()
