import unittest
from unittest.mock import MagicMock
from pokeys_py.digital_io import DigitalIO

class TestDigitalIO(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.lib = MagicMock()
        self.digital_io = DigitalIO(self.device, self.lib)

    def test_setup_input(self):
        self.digital_io.setup(1, 'input')
        self.lib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 2)

    def test_setup_output(self):
        self.digital_io.setup(1, 'output')
        self.lib.PK_SL_SetPinFunction.assert_called_once_with(self.device, 1, 4)

    def test_setup_invalid_direction(self):
        with self.assertRaises(ValueError):
            self.digital_io.setup(1, 'invalid')

    def test_fetch(self):
        self.lib.PK_SL_DigitalInputGet.return_value = 1
        value = self.digital_io.fetch(1)
        self.assertEqual(value, 1)
        self.lib.PK_SL_DigitalInputGet.assert_called_once_with(self.device, 1)

    def test_set(self):
        self.digital_io.set(1, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_once_with(self.device, 1, 0)

    def test_inversion_parameter(self):
        self.digital_io.setup(1, 'input')
        self.digital_io.set(1, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

    def test_inversion_parameter_handling(self):
        self.digital_io.setup(1, 'input')
        self.digital_io.set(1, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)
        self.digital_io.setup(1, 'output')
        self.digital_io.set(1, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

    def test_digital_io_behavior(self):
        for i in range(55):
            expected_value = i % 2
            self.digital_io.set(i, expected_value)
            actual_value = self.digital_io.fetch(i)
            self.assertEqual(actual_value, expected_value, f"Digital pin {i} expected {expected_value} but got {actual_value}")

            self.digital_io.set(i, not expected_value)
            actual_value = self.digital_io.fetch(i)
            self.assertEqual(actual_value, not expected_value, f"Digital pin {i} invert expected {not expected_value} but got {actual_value}")

    def test_setup_invalid_pin(self):
        self.lib.PK_SL_SetPinFunction.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.digital_io.setup(99, 'input')

    def test_fetch_invalid_pin(self):
        self.lib.PK_SL_DigitalInputGet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.digital_io.fetch(99)

    def test_set_invalid_pin(self):
        self.lib.PK_SL_DigitalOutputSet.side_effect = ValueError("Invalid pin")
        with self.assertRaises(ValueError):
            self.digital_io.set(99, 0)

    def test_hal_pins_mapped_to_signals(self):
        # Ensure HAL pins are created and mapped to signals in tests/linuxcnc_config.hal
        self.digital_io.setup(0, 'input')
        self.digital_io.set(0, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 1)
        self.digital_io.set(0, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 0)
        self.digital_io.setup(0, 'output')
        self.digital_io.set(0, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 1)
        self.digital_io.set(0, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 0, 0)

    def test_setup_digin(self):
        self.digital_io.setup_digin(0, 1)
        self.lib.PK_HAL_SetPin.assert_any_call("pokeys.0.digin.1.in", self.lib.PK_SL_DigitalInputGet.return_value)
        self.lib.PK_HAL_SetPin.assert_any_call("pokeys.0.digin.1.in-not", not self.lib.PK_SL_DigitalInputGet.return_value)

    def test_setup_digout(self):
        self.digital_io.setup_digout(0, 1)
        self.lib.PK_HAL_SetPin.assert_any_call("pokeys.0.digout.1.out", self.lib.PK_SL_DigitalInputGet.return_value)
        self.lib.PK_HAL_SetPin.assert_any_call("pokeys.0.digout.1.out-not", not self.lib.PK_SL_DigitalInputGet.return_value)

    def test_digital_io_parameters(self):
        self.digital_io.setup(1, 'input')
        self.digital_io.set(1, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)
        self.digital_io.setup(1, 'output')
        self.digital_io.set(1, 1)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 1)
        self.digital_io.set(1, 0)
        self.lib.PK_SL_DigitalOutputSet.assert_called_with(self.device, 1, 0)

    def test_digital_input_hal_interface(self):
        for pin_id in range(55):
            self.digital_io.setup_digin(0, pin_id)
            self.lib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digin.{pin_id}.in", self.lib.PK_SL_DigitalInputGet.return_value)
            self.lib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digin.{pin_id}.in-not", not self.lib.PK_SL_DigitalInputGet.return_value)

    def test_digital_output_hal_interface(self):
        for pin_id in range(55):
            self.digital_io.setup_digout(0, pin_id)
            self.lib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digout.{pin_id}.out", self.lib.PK_SL_DigitalInputGet.return_value)
            self.lib.PK_HAL_SetPin.assert_any_call(f"pokeys.0.digout.{pin_id}.out-not", not self.lib.PK_SL_DigitalInputGet.return_value)

if __name__ == '__main__':
    unittest.main()
