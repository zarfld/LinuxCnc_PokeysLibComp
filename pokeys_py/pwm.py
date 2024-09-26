import ctypes

# Load the PoKeysLib shared library
pokeyslib = ctypes.CDLL('./pokeys_rt/PoKeysLib.so')

class PWM:
    def __init__(self, device):
        self.device = device

    def setup(self, channel, frequency, duty_cycle):
        """
        Setup a PWM channel with the specified frequency and duty cycle.
        :param channel: PWM channel number
        :param frequency: PWM frequency
        :param duty_cycle: PWM duty cycle (0-100)
        """
        pokeyslib.PK_PWM_Setup(self.device, channel, frequency, duty_cycle)

    def fetch(self, channel):
        """
        Fetch the current PWM settings for a channel.
        :param channel: PWM channel number
        :return: Tuple (frequency, duty_cycle)
        """
        frequency = ctypes.c_uint32()
        duty_cycle = ctypes.c_uint32()
        pokeyslib.PK_PWM_Fetch(self.device, channel, ctypes.byref(frequency), ctypes.byref(duty_cycle))
        return frequency.value, duty_cycle.value

    def set(self, channel, frequency, duty_cycle):
        """
        Set the PWM settings for a channel.
        :param channel: PWM channel number
        :param frequency: PWM frequency
        :param duty_cycle: PWM duty cycle (0-100)
        """
        pokeyslib.PK_PWM_Set(self.device, channel, frequency, duty_cycle)
