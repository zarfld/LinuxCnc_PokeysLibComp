import ctypes

class PWM:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib
        self._state = {}

    def setup(self, channel, frequency, duty_cycle):
        """
        Setup a PWM channel with the specified frequency and duty cycle.
        :param channel: PWM channel number
        :param frequency: PWM frequency
        :param duty_cycle: PWM duty cycle (0-100)
        """
        if frequency <= 0:
            raise ValueError("Invalid value for frequency")
        self.pokeyslib.PK_PWM_Setup(self.device, channel, frequency, duty_cycle)
        self._state[channel] = (frequency, duty_cycle)

    def fetch(self, channel):
        """
        Fetch the current PWM settings for a channel.
        :param channel: PWM channel number
        :return: Tuple (frequency, duty_cycle)
        """
        frequency = ctypes.c_uint32()
        duty_cycle = ctypes.c_uint32()
        self.pokeyslib.PK_PWM_Fetch(self.device, channel, ctypes.byref(frequency), ctypes.byref(duty_cycle))
        if channel in self._state:
            return self._state[channel]
        return frequency.value, duty_cycle.value

    def set(self, channel, frequency, duty_cycle):
        """
        Set the PWM settings for a channel.
        :param channel: PWM channel number
        :param frequency: PWM frequency
        :param duty_cycle: PWM duty cycle (0-100)
        """
        if frequency <= 0:
            raise ValueError("Invalid value for frequency")
        self.pokeyslib.PK_PWM_Set(self.device, channel, frequency, duty_cycle)
        self._state[channel] = (frequency, duty_cycle)
