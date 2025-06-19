import unittest
from unittest.mock import MagicMock
from pokeys_py.ponet import PoNET

class TestPoNET(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.lib = MagicMock()
        self.ponet = PoNET(self.device, self.lib)

    def test_setup(self):
        self.ponet.setup(1)
        self.lib.PK_PoNETGetModuleSettings.assert_called_once_with(self.device, 1)

    def test_fetch(self):
        self.lib.PK_PoNETGetModuleStatus.return_value = 'module_data'
        data = self.ponet.fetch(1)
        self.assertEqual(data, 'module_data')
        self.lib.PK_PoNETGetModuleStatusRequest.assert_called_once_with(self.device, 1)
        self.lib.PK_PoNETGetModuleStatus.assert_called_once_with(self.device, 1)

    def test_set(self):
        self.ponet.set(1, 'data')
        self.lib.PK_PoNETSetModuleStatus.assert_called_once_with(self.device, 1, 'data')

    def test_fetch_invalid_module_id(self):
        self.lib.PK_PoNETGetModuleStatusRequest.side_effect = ValueError("Invalid module ID")
        with self.assertRaises(ValueError):
            self.ponet.fetch(99)

    def test_set_invalid_module_id(self):
        self.lib.PK_PoNETSetModuleStatus.side_effect = ValueError("Invalid module ID")
        with self.assertRaises(ValueError):
            self.ponet.set(99, 'data')

    def test_setup_invalid_module_id(self):
        self.lib.PK_PoNETGetModuleSettings.side_effect = ValueError("Invalid module ID")
        with self.assertRaises(ValueError):
            self.ponet.setup(99)

    def test_fetch_invalid_value(self):
        self.lib.PK_PoNETGetModuleStatus.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.ponet.fetch(1)

    def test_set_invalid_value(self):
        self.lib.PK_PoNETSetModuleStatus.side_effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.ponet.set(1, 'invalid_data')

    def test_setup_invalid_value(self):
        self.lib.PK_PoNETGetModuleSettings.side.effect = ValueError("Invalid value")
        with self.assertRaises(ValueError):
            self.ponet.setup(1)

    def test_ponet_functionalities(self):
        for i in range(48):
            expected_value = i % 2
            self.ponet.set(i, expected_value)
            actual_value = self.ponet.fetch(i)
            self.assertEqual(actual_value, expected_value, f"PoNET pin {i} expected {expected_value} but got {actual_value}")

            self.ponet.set(i, not expected_value)
            actual_value = self.ponet.fetch(i)
            self.assertEqual(actual_value, not expected_value, f"PoNET pin {i} invert expected {not expected_value} but got {actual_value}")

if __name__ == '__main__':
    unittest.main()
