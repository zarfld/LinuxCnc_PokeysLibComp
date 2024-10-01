# Initialize the pokeys_py package

__all__ = ["digital_io", "analog_io", "pwm", "counter", "ponet", "pev2_motion_control"]

import os

if os.getenv('CI') == 'true':
    from tests.mocks.pokeyslib import pokeyslib
else:
    import ctypes
    pokeyslib = ctypes.CDLL('/usr/lib/linuxcnc/modules/PoKeysLib.so')

from .telemetry import Telemetry

# Initialize telemetry collection if the user has opted in
telemetry = Telemetry(dsn="your_sentry_dsn_here", opt_in=True)
telemetry.initialize_sentry()

# Add pins for device information
class DeviceInfo:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib
        self.devSerial = None
        self.alive = None
        self.connected = None
        self.err = None
        self.info = {
            "PinCount": None,
            "PWMCount": None,
            "BasicEncoderCount": None,
            "EncodersCount": None,
            "FastEncoders": None,
            "UltraFastEncoders": None,
            "PWMinternalFrequency": None,
            "AnalogInputs": None,
            "KeyMapping": None,
            "TriggeredKeyMapping": None,
            "KeyRepeatDelay": None,
            "DigitalCounters": None,
            "JoystickButtonAxisMapping": None,
            "JoystickAnalogToDigitalMapping": None,
            "Macros": None,
            "MatrixKeyboard": None,
            "MatrixKeyboardTriggeredMapping": None,
            "LCD": None,
            "MatrixLED": None,
            "ConnectionSignal": None,
            "PoExtBus": None,
            "PoNET": None,
            "AnalogFiltering": None,
            "InitOutputsStart": None,
            "protI2C": None,
            "prot1wire": None,
            "AdditionalOptions": None,
            "LoadStatus": None,
            "CustomDeviceName": None,
            "PoTLog27support": None,
            "SensorList": None,
            "WebInterface": None,
            "FailSafeSettings": None,
            "JoystickHATswitch": None,
            "PulseEngine": None,
            "PulseEnginev2": None,
            "EasySensors": None
        }

    def initialize_pins(self):
        self.devSerial = self.pokeyslib.PK_GetDeviceSerial(self.device)
        self.alive = self.pokeyslib.PK_GetDeviceAlive(self.device)
        self.connected = self.pokeyslib.PK_GetDeviceConnected(self.device)
        self.err = self.pokeyslib.PK_GetDeviceError(self.device)
        self.info["PinCount"] = self.pokeyslib.PK_GetPinCount(self.device)
        self.info["PWMCount"] = self.pokeyslib.PK_GetPWMCount(self.device)
        self.info["BasicEncoderCount"] = self.pokeyslib.PK_GetBasicEncoderCount(self.device)
        self.info["EncodersCount"] = self.pokeyslib.PK_GetEncodersCount(self.device)
        self.info["FastEncoders"] = self.pokeyslib.PK_GetFastEncoders(self.device)
        self.info["UltraFastEncoders"] = self.pokeyslib.PK_GetUltraFastEncoders(self.device)
        self.info["PWMinternalFrequency"] = self.pokeyslib.PK_GetPWMinternalFrequency(self.device)
        self.info["AnalogInputs"] = self.pokeyslib.PK_GetAnalogInputs(self.device)
        self.info["KeyMapping"] = self.pokeyslib.PK_GetKeyMapping(self.device)
        self.info["TriggeredKeyMapping"] = self.pokeyslib.PK_GetTriggeredKeyMapping(self.device)
        self.info["KeyRepeatDelay"] = self.pokeyslib.PK_GetKeyRepeatDelay(self.device)
        self.info["DigitalCounters"] = self.pokeyslib.PK_GetDigitalCounters(self.device)
        self.info["JoystickButtonAxisMapping"] = self.pokeyslib.PK_GetJoystickButtonAxisMapping(self.device)
        self.info["JoystickAnalogToDigitalMapping"] = self.pokeyslib.PK_GetJoystickAnalogToDigitalMapping(self.device)
        self.info["Macros"] = self.pokeyslib.PK_GetMacros(self.device)
        self.info["MatrixKeyboard"] = self.pokeyslib.PK_GetMatrixKeyboard(self.device)
        self.info["MatrixKeyboardTriggeredMapping"] = self.pokeyslib.PK_GetMatrixKeyboardTriggeredMapping(self.device)
        self.info["LCD"] = self.pokeyslib.PK_GetLCD(self.device)
        self.info["MatrixLED"] = self.pokeyslib.PK_GetMatrixLED(self.device)
        self.info["ConnectionSignal"] = self.pokeyslib.PK_GetConnectionSignal(self.device)
        self.info["PoExtBus"] = self.pokeyslib.PK_GetPoExtBus(self.device)
        self.info["PoNET"] = self.pokeyslib.PK_GetPoNET(self.device)
        self.info["AnalogFiltering"] = self.pokeyslib.PK_GetAnalogFiltering(self.device)
        self.info["InitOutputsStart"] = self.pokeyslib.PK_GetInitOutputsStart(self.device)
        self.info["protI2C"] = self.pokeyslib.PK_GetprotI2C(self.device)
        self.info["prot1wire"] = self.pokeyslib.PK_Getprot1wire(self.device)
        self.info["AdditionalOptions"] = self.pokeyslib.PK_GetAdditionalOptions(self.device)
        self.info["LoadStatus"] = self.pokeyslib.PK_GetLoadStatus(self.device)
        self.info["CustomDeviceName"] = self.pokeyslib.PK_GetCustomDeviceName(self.device)
        self.info["PoTLog27support"] = self.pokeyslib.PK_GetPoTLog27support(self.device)
        self.info["SensorList"] = self.pokeyslib.PK_GetSensorList(self.device)
        self.info["WebInterface"] = self.pokeyslib.PK_GetWebInterface(self.device)
        self.info["FailSafeSettings"] = self.pokeyslib.PK_GetFailSafeSettings(self.device)
        self.info["JoystickHATswitch"] = self.pokeyslib.PK_GetJoystickHATswitch(self.device)
        self.info["PulseEngine"] = self.pokeyslib.PK_GetPulseEngine(self.device)
        self.info["PulseEnginev2"] = self.pokeyslib.PK_GetPulseEnginev2(self.device)
        self.info["EasySensors"] = self.pokeyslib.PK_GetEasySensors(self.device)

# Initialize the device information pins
device_info = DeviceInfo(device=pokeyslib, pokeyslib=pokeyslib)
device_info.initialize_pins()
