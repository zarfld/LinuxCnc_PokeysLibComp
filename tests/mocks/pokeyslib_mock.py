import random

class PoKeysLibMock:
    def __init__(self):
        self.digital_inputs = {}
        self.digital_outputs = {}
        self.analog_inputs = {}
        self.analog_outputs = {}
        self.counters = {}
        self.pwm_channels = {}

    def PK_SL_SetPinFunction(self, device, pin, function):
        pass

    def PK_SL_DigitalInputGet(self, device, pin):
        return self.digital_inputs.get(pin, 0)

    def PK_SL_DigitalOutputSet(self, device, pin, value):
        self.digital_outputs[pin] = value

    def PK_SL_AnalogInputGet(self, device, pin):
        return self.analog_inputs.get(pin, 0.0)

    def PK_SL_AnalogOutputSet(self, device, pin, value):
        self.analog_outputs[pin] = value

    def PK_IsCounterAvailable(self, device, pin):
        return pin in self.counters

    def PK_DigitalCounterGet(self, device):
        return self.counters

    def PK_DigitalCounterClear(self, device):
        self.counters = {pin: 0 for pin in self.counters}

    def PK_PWM_Setup(self, device, channel, frequency, duty_cycle):
        self.pwm_channels[channel] = (frequency, duty_cycle)

    def PK_PWM_Fetch(self, device, channel, frequency, duty_cycle):
        return self.pwm_channels.get(channel, (0, 0))

    def PK_PWM_Set(self, device, channel, frequency, duty_cycle):
        self.pwm_channels[channel] = (frequency, duty_cycle)

    def PK_PEv2_AxisConfigurationSet(self, device, axis, parameters):
        pass

    def PK_PEv2_StatusGet(self, device):
        return random.choice(['status1', 'status2', 'status3'])

    def PK_PEv2_GetPosition(self, device):
        return random.uniform(0, 100)

    def PK_PEv2_SetPosition(self, device, axis, position):
        pass

    def PK_PEv2_SetVelocity(self, device, axis, velocity):
        pass

    def PK_PEv2_StartHoming(self, device, axis):
        pass

    def PK_PEv2_CancelHoming(self, device, axis):
        pass

    def PK_PEv2_GetHomingStatus(self, device, axis):
        return random.choice([10, 11, 12, 13])

    def PK_PoNETGetModuleSettings(self, device, module_id):
        pass

    def PK_PoNETGetModuleStatusRequest(self, device, module_id):
        pass

    def PK_PoNETGetModuleStatus(self, device, module_id):
        return 'module_status'

    def PK_PoNETSetModuleStatus(self, device, module_id, data):
        pass

pokeyslib = PoKeysLibMock()
