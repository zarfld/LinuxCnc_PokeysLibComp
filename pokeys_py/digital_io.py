import ctypes

# Load the PoKeysLib shared library
pokeyslib = ctypes.CDLL('./pokeys_rt/PoKeysLib.so')

class DigitalIO:
    def __init__(self, device):
        self.device = device

    def setup(self, pin, direction):
        """
        Setup a digital pin as input or output.
        :param pin: Pin number
        :param direction: 'input' or 'output'
        """
        if direction == 'input':
            pokeyslib.PK_SL_SetPinFunction(self.device, pin, 2)  # 2 for digital input
        elif direction == 'output':
            pokeyslib.PK_SL_SetPinFunction(self.device, pin, 4)  # 4 for digital output
        else:
            raise ValueError("Invalid direction. Use 'input' or 'output'.")

    def fetch(self, pin):
        """
        Fetch the current state of a digital input pin.
        :param pin: Pin number
        :return: Pin state (0 or 1)
        """
        return pokeyslib.PK_SL_DigitalInputGet(self.device, pin)

    def set(self, pin, value):
        """
        Set the state of a digital output pin.
        :param pin: Pin number
        :param value: Pin state (0 or 1)
        """
        pokeyslib.PK_SL_DigitalOutputSet(self.device, pin, value)
