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
            try:
                self.pokeyslib.PK_SL_SetPinFunction(self.device, pin, 1)  # 1 for analog input
            except ValueError:
                raise ValueError(f"Invalid pin {pin}")
        elif direction == 'output':
            try:
                self.pokeyslib.PK_SL_SetPinFunction(self.device, pin, 3)  # 3 for analog output
            except ValueError:
                raise ValueError(f"Invalid pin {pin}")
        else:
            raise ValueError("Invalid direction. Use 'input' or 'output'.")

    def fetch(self, pin):
        """
        Fetch the current value of an analog input pin.
        :param pin: Pin number
        :return: Pin value
        """
        try:
            return self.pokeyslib.PK_SL_AnalogInputGet(self.device, pin)
        except ValueError:
            raise ValueError(f"Invalid pin {pin}")

    def set(self, pin, value):
        """
        Set the value of an analog output pin.
        :param pin: Pin number
        :param value: Pin value
        """
        try:
            self.pokeyslib.PK_SL_AnalogOutputSet(self.device, pin, value)
        except ValueError:
            raise ValueError(f"Invalid pin {pin}")
