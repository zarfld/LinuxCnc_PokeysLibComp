import ctypes

class AnalogIO:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib
        self.scale = {}
        self.offset = {}
        self._state = {}

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
            if pin in self._state:
                raw_value = self._state[pin]
            else:
                raw_value = self.pokeyslib.PK_SL_AnalogInputGet(self.device, pin)
            scaled_value = raw_value * self.scale.get(pin, 1) - self.offset.get(pin, 0)
            return scaled_value
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
            self._state[pin] = value
        except ValueError:
            raise ValueError(f"Invalid pin {pin}")

    def set_scale(self, pin, scale):
        """
        Set the scale value for an analog input pin.
        :param pin: Pin number
        :param scale: Scale value
        """
        self.scale[pin] = scale

    def set_offset(self, pin, offset):
        """
        Set the offset value for an analog input pin.
        :param pin: Pin number
        :param offset: Offset value
        """
        self.offset[pin] = offset
