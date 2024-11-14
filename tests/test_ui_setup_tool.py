import unittest
from unittest.mock import patch, MagicMock
from pokeys_py.ui_setup_tool import UISetupTool

class TestUISetupTool(unittest.TestCase):

    @patch('pokeys_py.ui_setup_tool.ctypes.CDLL')
    def setUp(self, mock_cdll):
        self.mock_pokeyslib = MagicMock()
        mock_cdll.return_value = self.mock_pokeyslib
        self.ui_setup_tool = UISetupTool()

    def test_connect_device(self):
        self.mock_pokeyslib.PK_ConnectToDevice.return_value = 1
        self.ui_setup_tool.connect_device(0)
        self.assertEqual(self.ui_setup_tool.device, 1)
        self.mock_pokeyslib.PK_ConnectToDevice.assert_called_once_with(0)

    def test_disconnect_device(self):
        self.mock_pokeyslib.PK_ConnectToDevice.return_value = 1
        self.ui_setup_tool.connect_device(0)
        self.ui_setup_tool.disconnect_device()
        self.assertIsNone(self.ui_setup_tool.device)
        self.mock_pokeyslib.PK_DisconnectDevice.assert_called_once()

    def test_get_device_data(self):
        self.mock_pokeyslib.PK_ConnectToDevice.return_value = 1
        self.mock_pokeyslib.PK_GetDeviceSerial.return_value = 12345
        self.mock_pokeyslib.PK_GetDeviceAlive.return_value = 1
        self.mock_pokeyslib.PK_GetDeviceConnected.return_value = 1
        self.mock_pokeyslib.PK_GetDeviceError.return_value = 0
        self.ui_setup_tool.connect_device(0)
        device_data = self.ui_setup_tool.get_device_data()
        self.assertEqual(device_data["serial"], 12345)
        self.assertEqual(device_data["alive"], 1)
        self.assertEqual(device_data["connected"], 1)
        self.assertEqual(device_data["error"], 0)

    def test_check_pin_capability(self):
        self.mock_pokeyslib.PK_ConnectToDevice.return_value = 1
        self.mock_pokeyslib.PK_CheckPinCapability.return_value = 1
        self.ui_setup_tool.connect_device(0)
        result = self.ui_setup_tool.check_pin_capability(0, "digital_input")
        self.assertTrue(result)
        self.mock_pokeyslib.PK_CheckPinCapability.assert_called_once_with(1, 0, "digital_input")

    def test_validate_configuration(self):
        self.mock_pokeyslib.PK_ConnectToDevice.return_value = 1
        self.mock_pokeyslib.PK_CheckPinCapability.return_value = 1
        self.ui_setup_tool.connect_device(0)
        config = {0: "digital_input", 1: "digital_output"}
        valid = self.ui_setup_tool.validate_configuration(config)
        self.assertTrue(valid)

    def test_generate_hal_ini_files(self):
        self.mock_pokeyslib.PK_ConnectToDevice.return_value = 1
        self.mock_pokeyslib.PK_CheckPinCapability.return_value = 1
        self.ui_setup_tool.connect_device(0)
        config = {0: "digital_input", 1: "digital_output"}
        self.ui_setup_tool.generate_hal_ini_files(config, "/tmp")
        with open("/tmp/config.hal", "r") as hal_file:
            hal_content = hal_file.read()
        with open("/tmp/config.ini", "r") as ini_file:
            ini_content = ini_file.read()
        self.assertIn("setp pokeys.0 digital_input", hal_content)
        self.assertIn("setp pokeys.1 digital_output", ini_content)

if __name__ == "__main__":
    unittest.main()
