import ctypes

class PWM:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib

    def setup(self, channel, frequency, duty_cycle):
        """
        Setup a PWM channel with the specified frequency and duty cycle.
        :param channel: PWM channel number
        :param frequency: PWM frequency
        :param duty_cycle: PWM duty cycle (0-100)
        """
        if channel < 0 or channel >= len(self.device.PWMChannels):
            raise ValueError(f"Invalid channel {channel}")
        if frequency <= 0 or duty_cycle < 0 or duty_cycle > 100:
            raise ValueError("Invalid value for frequency or duty cycle")
        self.pokeyslib.PK_PWM_Setup(self.device, channel, frequency, duty_cycle)

    def fetch(self, channel):
        """
        Fetch the current PWM settings for a channel.
        :param channel: PWM channel number
        :return: Tuple (frequency, duty_cycle)
        """
        if channel < 0 or channel >= len(self.device.PWMChannels):
            raise ValueError(f"Invalid channel {channel}")
        frequency = ctypes.c_uint32()
        duty_cycle = ctypes.c_uint32()
        self.pokeyslib.PK_PWM_Fetch(self.device, channel, ctypes.byref(frequency), ctypes.byref(duty_cycle))
        return frequency.value, duty_cycle.value

    def set(self, channel, frequency, duty_cycle):
        """
        Set the PWM settings for a channel.
        :param channel: PWM channel number
        :param frequency: PWM frequency
        :param duty_cycle: PWM duty cycle (0-100)
        """
        if channel < 0 or channel >= len(self.device.PWMChannels):
            raise ValueError(f"Invalid channel {channel}")
        if frequency <= 0 or duty_cycle < 0 or duty_cycle > 100:
            raise ValueError("Invalid value for frequency or duty cycle")
        self.pokeyslib.PK_PWM_Set(self.device, channel, frequency, duty_cycle)
