import os
import ctypes
from .telemetry import Telemetry

class UISetupTool:
    def __init__(self, opt_in=False):
        self.pokeyslib = ctypes.CDLL('/usr/lib/linuxcnc/modules/PoKeysLib.so')
        self.device = None
        if os.getenv('CI') == 'true':
            self.telemetry = Telemetry(dsn="", opt_in=False)
        else:
            self.telemetry = Telemetry(dsn="your_sentry_dsn_here", opt_in=opt_in)
        self.telemetry.initialize_sentry()

    def connect_device(self, device_index):
        self.device = self.pokeyslib.PK_ConnectToDevice(device_index)
        if not self.device:
            raise Exception("Failed to connect to PoKeys device")
        self.telemetry.track_event("DeviceConnected", {"device_index": device_index})

    def disconnect_device(self):
        if self.device:
            self.pokeyslib.PK_DisconnectDevice(self.device)
            self.device = None
            self.telemetry.track_event("DeviceDisconnected")

    def get_device_data(self):
        if not self.device:
            raise Exception("No device connected")
        device_data = {}
        device_data["serial"] = self.pokeyslib.PK_GetDeviceSerial(self.device)
        device_data["alive"] = self.pokeyslib.PK_GetDeviceAlive(self.device)
        device_data["connected"] = self.pokeyslib.PK_GetDeviceConnected(self.device)
        device_data["error"] = self.pokeyslib.PK_GetDeviceError(self.device)
        self.telemetry.track_event("DeviceDataRetrieved", device_data)
        return device_data

    def check_pin_capability(self, pin, capability):
        if not self.device:
            raise Exception("No device connected")
        result = self.pokeyslib.PK_CheckPinCapability(self.device, pin, capability)
        self.telemetry.track_event("PinCapabilityChecked", {"pin": pin, "capability": capability, "result": result})
        return result

    def validate_configuration(self, config):
        if not self.device:
            raise Exception("No device connected")
        valid = True
        for pin, capability in config.items():
            if not self.check_pin_capability(pin, capability):
                valid = False
                break
        self.telemetry.track_event("ConfigurationValidated", {"config": config, "valid": valid})
        return valid

    def generate_hal_ini_files(self, config, output_dir):
        if not self.validate_configuration(config):
            raise Exception("Invalid configuration")
        hal_file = os.path.join(output_dir, "config.hal")
        ini_file = os.path.join(output_dir, "config.ini")
        with open(hal_file, "w") as hal, open(ini_file, "w") as ini:
            hal.write("# HAL configuration for PoKeys device\n")
            ini.write("# INI configuration for PoKeys device\n")
            for pin, capability in config.items():
                hal.write(f"setp pokeys.{pin} {capability}\n")
                ini.write(f"setp pokeys.{pin} {capability}\n")
        self.telemetry.track_event("HALINIFileGenerated", {"hal_file": hal_file, "ini_file": ini_file})

    def run(self):
        try:
            self.connect_device(0)
            device_data = self.get_device_data()
            print("Device Data:", device_data)
            config = {0: "digital_input", 1: "digital_output"}
            self.generate_hal_ini_files(config, "/tmp")
        except Exception as e:
            self.telemetry.track_exception(e)
            print("Error:", e)
        finally:
            self.disconnect_device()

if __name__ == "__main__":
    tool = UISetupTool()
    tool.run()
