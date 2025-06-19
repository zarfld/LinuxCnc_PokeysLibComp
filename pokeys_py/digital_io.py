import ctypes

class DigitalIO:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib
        self._state = {}

    def setup(self, pin, direction):
        """
        Setup a digital pin as input or output.
        :param pin: Pin number
        :param direction: 'input' or 'output'
        """
        if direction == 'input':
            try:
                self.pokeyslib.PK_SL_SetPinFunction(self.device, pin, 2)  # 2 for digital input
            except ValueError:
                raise ValueError(f"Invalid pin {pin}")
        elif direction == 'output':
            try:
                self.pokeyslib.PK_SL_SetPinFunction(self.device, pin, 4)  # 4 for digital output
            except ValueError:
                raise ValueError(f"Invalid pin {pin}")
        else:
            raise ValueError("Invalid direction. Use 'input' or 'output'.")

    def fetch(self, pin):
        """
        Fetch the current state of a digital input pin.
        :param pin: Pin number
        :return: Pin state (0 or 1)
        """
        try:
            if pin in self._state:
                return self._state[pin]
            return self.pokeyslib.PK_SL_DigitalInputGet(self.device, pin)
        except ValueError:
            raise ValueError(f"Invalid pin {pin}")

    def set(self, pin, value):
        """
        Set the state of a digital output pin.
        :param pin: Pin number
        :param value: Pin state (0 or 1)
        """
        try:
            self.pokeyslib.PK_SL_DigitalOutputSet(self.device, pin, value)
            self._state[pin] = value
        except ValueError:
            raise ValueError(f"Invalid pin {pin}")

    def setup_digin(self, devid, pinid):
        """
        Setup a digital input pin with HAL interface.
        :param devid: Device ID
        :param pinid: Pin ID
        """
        self.setup(pinid, 'input')
        self.pokeyslib.PK_HAL_SetPin(f"pokeys.{devid}.digin.{pinid}.in", self.fetch(pinid))
        self.pokeyslib.PK_HAL_SetPin(f"pokeys.{devid}.digin.{pinid}.in-not", not self.fetch(pinid))

    def setup_digout(self, devid, pinid):
        """
        Setup a digital output pin with HAL interface.
        :param devid: Device ID
        :param pinid: Pin ID
        """
        self.setup(pinid, 'output')
        self.pokeyslib.PK_HAL_SetPin(f"pokeys.{devid}.digout.{pinid}.out", self.fetch(pinid))
        self.pokeyslib.PK_HAL_SetPin(f"pokeys.{devid}.digout.{pinid}.out-not", not self.fetch(pinid))
