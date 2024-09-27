import ctypes

class AnalogIO:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib

    def setup(self, pin, direction):
        """
        Setup an analog pin as input or output.
        :param pin: Pin number
        :param direction: 'input' or 'output'
        """
        if direction == 'input':
            self.pokeyslib.PK_SL_SetPinFunction(self.device, pin, 1)  # 1 for analog input
        elif direction == 'output':
            self.pokeyslib.PK_SL_SetPinFunction(self.device, pin, 3)  # 3 for analog output
        else:
            raise ValueError("Invalid direction. Use 'input' or 'output'.")

    def fetch(self, pin):
        """
        Fetch the current value of an analog input pin.
        :param pin: Pin number
        :return: Pin value
        """
        return self.pokeyslib.PK_SL_AnalogInputGet(self.device, pin)

    def set(self, pin, value):
        """
        Set the value of an analog output pin.
        :param pin: Pin number
        :param value: Pin value
        """
        self.pokeyslib.PK_SL_AnalogOutputSet(self.device, pin, value)
