# Initialize the pokeys_py package

__all__ = [
    "digital_io",
    "analog_io",
    "pwm",
    "counter",
    "ponet",
    "can_bus",
    "pev2_motion_control",
]

import os
from .telemetry import Telemetry

pokeyslib = None
telemetry = None
device_info = None

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
            "EasySensors": None,
            "PEv2": {
                "nrOfAxes": None,
                "maxPulseFrequency": None,
                "bufferDepth": None,
                "slotTiming": None,
                "digin": {
                    "Emergency": {
                        "in": None,
                        "in_not": None,
                        "Pin": None,
                        "invert": None
                    },
                    "Misc": {
                        "in": [None] * 8,
                        "in_not": [None] * 8
                    },
                    "Probed": {
                        "in": None
                    },
                    "Probe": {
                        "Pin": None,
                        "invert": None
                    }
                },
                "PulseEngineEnabled": None,
                "PulseGeneratorType": None,
                "PG_swap_stepdir": None,
                "PG_extended_io": None,
                "ChargePumpEnabled": None,
                "PulseEngineActivated": None,
                "PulseEngineState": None,
                "MiscInputStatus": None,
                "LimitOverride": None,
                "LimitOverrideSetup": None,
                "digout": {
                    "Emergency": {
                        "out": None,
                        "Pin": None
                    },
                    "ExternalRelay": {
                        "out": [None] * 4
                    },
                    "ExternalOC": {
                        "out": [None] * 4
                    },
                    "AxisEnable": {
                        "out": [None] * 8,
                        "Pin": [None] * 8,
                        "invert": [None] * 8
                    },
                    "LimitOverride": {
                        "out": [None] * 8
                    }
                },
                "AxisEnabledMask": None,
                "AxisEnabledStatesMask": None,
                "ExternalRelayOutputs": None,
                "ExternalOCOutputs": None,
                "HomingStartMaskSetup": None,
                "ProbeStartMaskSetup": None,
                "ProbeStatus": None,
                "ProbeSpeed": None,
                "BacklashCompensationEnabled": None,
                "AxesState": [None] * 8,
                "AxesConfig": [None] * 8,
                "AxesSwitchConfig": [None] * 8,
                "SoftLimitMaximum": [None] * 8,
                "SoftLimitMinimum": [None] * 8,
                "HomingSpeed": [None] * 8,
                "HomingReturnSpeed": [None] * 8,
                "HomingAlgorithm": [None] * 8,
                "HomeOffsets": [None] * 8,
                "ProbePosition": [None] * 8,
                "ProbeMaxPosition": [None] * 8,
                "CurrentPosition": [None] * 8,
                "PositionSetup": [None] * 8,
                "ReferencePositionSpeed": [None] * 8,
                "MaxSpeed": [None] * 8,
                "MaxAcceleration": [None] * 8,
                "MPGjogMultiplier": [None] * 8,
                "MPGjogEncoder": [None] * 8,
                "MPGjogDivider": [None] * 8,
                "digin": {
                    "LimitN": {
                        "in": [None] * 8,
                        "in_not": [None] * 8,
                        "DedicatedInput": [None] * 8,
                        "Pin": [None] * 8,
                        "Filter": [None] * 8,
                        "invert": [None] * 8
                    },
                    "LimitP": {
                        "in": [None] * 8,
                        "in_not": [None] * 8,
                        "DedicatedInput": [None] * 8,
                        "Pin": [None] * 8,
                        "Filter": [None] * 8,
                        "invert": [None] * 8
                    },
                    "Home": {
                        "in": [None] * 8,
                        "in_not": [None] * 8,
                        "DedicatedInput": [None] * 8,
                        "Pin": [None] * 8,
                        "Filter": [None] * 8,
                        "invert": [None] * 8
                    },
                    "Error": {
                        "in": [None] * 8,
                        "in_not": [None] * 8
                    },
                    "Probe": {
                        "in": [None] * 8,
                        "in_not": [None] * 8
                    },
                    "SoftLimit": {
                        "in": [None] * 8,
                        "PosMin": [None] * 8,
                        "PosMax": [None] * 8
                    },
                    "AxisEnabled": {
                        "in": [None] * 8
                    }
                },
                "BacklashWidth": [None] * 8,
                "BacklashRegister": [None] * 8,
                "BacklashAcceleration": [None] * 8,
                "HomeBackOffDistance": [None] * 8
            }
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
        self.info["PEv2"]["nrOfAxes"] = self.pokeyslib.PK_GetPEv2NrOfAxes(self.device)
        self.info["PEv2"]["maxPulseFrequency"] = self.pokeyslib.PK_GetPEv2MaxPulseFrequency(self.device)
        self.info["PEv2"]["bufferDepth"] = self.pokeyslib.PK_GetPEv2BufferDepth(self.device)
        self.info["PEv2"]["slotTiming"] = self.pokeyslib.PK_GetPEv2SlotTiming(self.device)
        self.info["PEv2"]["digin"]["Emergency"]["in"] = self.pokeyslib.PK_GetPEv2DiginEmergencyIn(self.device)
        self.info["PEv2"]["digin"]["Emergency"]["in_not"] = self.pokeyslib.PK_GetPEv2DiginEmergencyInNot(self.device)
        self.info["PEv2"]["digin"]["Emergency"]["Pin"] = self.pokeyslib.PK_GetPEv2DiginEmergencyPin(self.device)
        self.info["PEv2"]["digin"]["Emergency"]["invert"] = self.pokeyslib.PK_GetPEv2DiginEmergencyInvert(self.device)
        self.info["PEv2"]["digin"]["Misc"]["in"] = [self.pokeyslib.PK_GetPEv2DiginMiscIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Misc"]["in_not"] = [self.pokeyslib.PK_GetPEv2DiginMiscInNot(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Probed"]["in"] = self.pokeyslib.PK_GetPEv2DiginProbedIn(self.device)
        self.info["PEv2"]["digin"]["Probe"]["Pin"] = self.pokeyslib.PK_GetPEv2DiginProbePin(self.device)
        self.info["PEv2"]["digin"]["Probe"]["invert"] = self.pokeyslib.PK_GetPEv2DiginProbeInvert(self.device)
        self.info["PEv2"]["PulseEngineEnabled"] = self.pokeyslib.PK_GetPEv2PulseEngineEnabled(self.device)
        self.info["PEv2"]["PulseGeneratorType"] = self.pokeyslib.PK_GetPEv2PulseGeneratorType(self.device)
        self.info["PEv2"]["PG_swap_stepdir"] = self.pokeyslib.PK_GetPEv2PGSwapStepdir(self.device)
        self.info["PEv2"]["PG_extended_io"] = self.pokeyslib.PK_GetPEv2PGExtendedIo(self.device)
        self.info["PEv2"]["ChargePumpEnabled"] = self.pokeyslib.PK_GetPEv2ChargePumpEnabled(self.device)
        self.info["PEv2"]["PulseEngineActivated"] = self.pokeyslib.PK_GetPEv2PulseEngineActivated(self.device)
        self.info["PEv2"]["PulseEngineState"] = self.pokeyslib.PK_GetPEv2PulseEngineState(self.device)
        self.info["PEv2"]["MiscInputStatus"] = self.pokeyslib.PK_GetPEv2MiscInputStatus(self.device)
        self.info["PEv2"]["LimitOverride"] = self.pokeyslib.PK_GetPEv2LimitOverride(self.device)
        self.info["PEv2"]["LimitOverrideSetup"] = self.pokeyslib.PK_GetPEv2LimitOverrideSetup(self.device)
        self.info["PEv2"]["digout"]["Emergency"]["out"] = self.pokeyslib.PK_GetPEv2DigoutEmergencyOut(self.device)
        self.info["PEv2"]["digout"]["Emergency"]["Pin"] = self.pokeyslib.PK_GetPEv2DigoutEmergencyPin(self.device)
        self.info["PEv2"]["digout"]["ExternalRelay"]["out"] = [self.pokeyslib.PK_GetPEv2DigoutExternalRelayOut(self.device, i) for i in range(4)]
        self.info["PEv2"]["digout"]["ExternalOC"]["out"] = [self.pokeyslib.PK_GetPEv2DigoutExternalOCOut(self.device, i) for i in range(4)]
        self.info["PEv2"]["digout"]["AxisEnable"]["out"] = [self.pokeyslib.PK_GetPEv2DigoutAxisEnableOut(self.device, i) for i in range(8)]
        self.info["PEv2"]["digout"]["AxisEnable"]["Pin"] = [self.pokeyslib.PK_GetPEv2DigoutAxisEnablePin(self.device, i) for i in range(8)]
        self.info["PEv2"]["digout"]["AxisEnable"]["invert"] = [self.pokeyslib.PK_GetPEv2DigoutAxisEnableInvert(self.device, i) for i in range(8)]
        self.info["PEv2"]["digout"]["LimitOverride"]["out"] = [self.pokeyslib.PK_GetPEv2DigoutLimitOverrideOut(self.device, i) for i in range(8)]
        self.info["PEv2"]["AxisEnabledMask"] = self.pokeyslib.PK_GetPEv2AxisEnabledMask(self.device)
        self.info["PEv2"]["AxisEnabledStatesMask"] = self.pokeyslib.PK_GetPEv2AxisEnabledStatesMask(self.device)
        self.info["PEv2"]["ExternalRelayOutputs"] = self.pokeyslib.PK_GetPEv2ExternalRelayOutputs(self.device)
        self.info["PEv2"]["ExternalOCOutputs"] = self.pokeyslib.PK_GetPEv2ExternalOCOutputs(self.device)
        self.info["PEv2"]["HomingStartMaskSetup"] = self.pokeyslib.PK_GetPEv2HomingStartMaskSetup(self.device)
        self.info["PEv2"]["ProbeStartMaskSetup"] = self.pokeyslib.PK_GetPEv2ProbeStartMaskSetup(self.device)
        self.info["PEv2"]["ProbeStatus"] = self.pokeyslib.PK_GetPEv2ProbeStatus(self.device)
        self.info["PEv2"]["ProbeSpeed"] = self.pokeyslib.PK_GetPEv2ProbeSpeed(self.device)
        self.info["PEv2"]["BacklashCompensationEnabled"] = self.pokeyslib.PK_GetPEv2BacklashCompensationEnabled(self.device)
        self.info["PEv2"]["AxesState"] = [self.pokeyslib.PK_GetPEv2AxesState(self.device, i) for i in range(8)]
        self.info["PEv2"]["AxesConfig"] = [self.pokeyslib.PK_GetPEv2AxesConfig(self.device, i) for i in range(8)]
        self.info["PEv2"]["AxesSwitchConfig"] = [self.pokeyslib.PK_GetPEv2AxesSwitchConfig(self.device, i) for i in range(8)]
        self.info["PEv2"]["SoftLimitMaximum"] = [self.pokeyslib.PK_GetPEv2SoftLimitMaximum(self.device, i) for i in range(8)]
        self.info["PEv2"]["SoftLimitMinimum"] = [self.pokeyslib.PK_GetPEv2SoftLimitMinimum(self.device, i) for i in range(8)]
        self.info["PEv2"]["HomingSpeed"] = [self.pokeyslib.PK_GetPEv2HomingSpeed(self.device, i) for i in range(8)]
        self.info["PEv2"]["HomingReturnSpeed"] = [self.pokeyslib.PK_GetPEv2HomingReturnSpeed(self.device, i) for i in range(8)]
        self.info["PEv2"]["HomingAlgorithm"] = [self.pokeyslib.PK_GetPEv2HomingAlgorithm(self.device, i) for i in range(8)]
        self.info["PEv2"]["HomeOffsets"] = [self.pokeyslib.PK_GetPEv2HomeOffsets(self.device, i) for i in range(8)]
        self.info["PEv2"]["ProbePosition"] = [self.pokeyslib.PK_GetPEv2ProbePosition(self.device, i) for i in range(8)]
        self.info["PEv2"]["ProbeMaxPosition"] = [self.pokeyslib.PK_GetPEv2ProbeMaxPosition(self.device, i) for i in range(8)]
        self.info["PEv2"]["CurrentPosition"] = [self.pokeyslib.PK_GetPEv2CurrentPosition(self.device, i) for i in range(8)]
        self.info["PEv2"]["PositionSetup"] = [self.pokeyslib.PK_GetPEv2PositionSetup(self.device, i) for i in range(8)]
        self.info["PEv2"]["ReferencePositionSpeed"] = [self.pokeyslib.PK_GetPEv2ReferencePositionSpeed(self.device, i) for i in range(8)]
        self.info["PEv2"]["MaxSpeed"] = [self.pokeyslib.PK_GetPEv2MaxSpeed(self.device, i) for i in range(8)]
        self.info["PEv2"]["MaxAcceleration"] = [self.pokeyslib.PK_GetPEv2MaxAcceleration(self.device, i) for i in range(8)]
        self.info["PEv2"]["MPGjogMultiplier"] = [self.pokeyslib.PK_GetPEv2MPGjogMultiplier(self.device, i) for i in range(8)]
        self.info["PEv2"]["MPGjogEncoder"] = [self.pokeyslib.PK_GetPEv2MPGjogEncoder(self.device, i) for i in range(8)]
        self.info["PEv2"]["MPGjogDivider"] = [self.pokeyslib.PK_GetPEv2MPGjogDivider(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitN"]["in"] = [self.pokeyslib.PK_GetPEv2DiginLimitNIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitN"]["in_not"] = [self.pokeyslib.PK_GetPEv2DiginLimitNInNot(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitN"]["DedicatedInput"] = [self.pokeyslib.PK_GetPEv2DiginLimitNDedicatedInput(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitN"]["Pin"] = [self.pokeyslib.PK_GetPEv2DiginLimitNPin(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitN"]["Filter"] = [self.pokeyslib.PK_GetPEv2DiginLimitNFilter(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitN"]["invert"] = [self.pokeyslib.PK_GetPEv2DiginLimitNInvert(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitP"]["in"] = [self.pokeyslib.PK_GetPEv2DiginLimitPIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitP"]["in_not"] = [self.pokeyslib.PK_GetPEv2DiginLimitPInNot(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitP"]["DedicatedInput"] = [self.pokeyslib.PK_GetPEv2DiginLimitPDedicatedInput(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitP"]["Pin"] = [self.pokeyslib.PK_GetPEv2DiginLimitPPin(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitP"]["Filter"] = [self.pokeyslib.PK_GetPEv2DiginLimitPFilter(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["LimitP"]["invert"] = [self.pokeyslib.PK_GetPEv2DiginLimitPInvert(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Home"]["in"] = [self.pokeyslib.PK_GetPEv2DiginHomeIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Home"]["in_not"] = [self.pokeyslib.PK_GetPEv2DiginHomeInNot(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Home"]["DedicatedInput"] = [self.pokeyslib.PK_GetPEv2DiginHomeDedicatedInput(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Home"]["Pin"] = [self.pokeyslib.PK_GetPEv2DiginHomePin(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Home"]["Filter"] = [self.pokeyslib.PK_GetPEv2DiginHomeFilter(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Home"]["invert"] = [self.pokeyslib.PK_GetPEv2DiginHomeInvert(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Error"]["in"] = [self.pokeyslib.PK_GetPEv2DiginErrorIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Error"]["in_not"] = [self.pokeyslib.PK_GetPEv2DiginErrorInNot(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Probe"]["in"] = [self.pokeyslib.PK_GetPEv2DiginProbeIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["Probe"]["in_not"] = [self.pokeyslib.PK_GetPEv2DiginProbeInNot(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["SoftLimit"]["in"] = [self.pokeyslib.PK_GetPEv2DiginSoftLimitIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["SoftLimit"]["PosMin"] = [self.pokeyslib.PK_GetPEv2DiginSoftLimitPosMin(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["SoftLimit"]["PosMax"] = [self.pokeyslib.PK_GetPEv2DiginSoftLimitPosMax(self.device, i) for i in range(8)]
        self.info["PEv2"]["digin"]["AxisEnabled"]["in"] = [self.pokeyslib.PK_GetPEv2DiginAxisEnabledIn(self.device, i) for i in range(8)]
        self.info["PEv2"]["BacklashWidth"] = [self.pokeyslib.PK_GetPEv2BacklashWidth(self.device, i) for i in range(8)]
        self.info["PEv2"]["BacklashRegister"] = [self.pokeyslib.PK_GetPEv2BacklashRegister(self.device, i) for i in range(8)]
        self.info["PEv2"]["BacklashAcceleration"] = [self.pokeyslib.PK_GetPEv2BacklashAcceleration(self.device, i) for i in range(8)]
        self.info["PEv2"]["HomeBackOffDistance"] = [self.pokeyslib.PK_GetPEv2HomeBackOffDistance(self.device, i) for i in range(8)]

def run_ui_setup_tool(opt_in=True):
    """Initialize the device and launch the graphical setup tool."""
    global pokeyslib, telemetry, device_info

    if os.getenv("CI") == "true":
        from tests.mocks.pokeyslib import pokeyslib as lib
        telemetry = Telemetry(dsn="", opt_in=False)
    else:
        import ctypes
        lib = ctypes.CDLL("/usr/lib/linuxcnc/modules/PoKeysLib.so")
        telemetry = Telemetry(dsn="your_sentry_dsn_here", opt_in=opt_in)

    telemetry.initialize_sentry()

    pokeyslib = lib
    device_info = DeviceInfo(device=pokeyslib, pokeyslib=pokeyslib)
    device_info.initialize_pins()

    from .ui_setup_tool import UISetupTool

    ui_setup_tool = UISetupTool()
    ui_setup_tool.run()

