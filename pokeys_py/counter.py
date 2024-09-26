import ctypes
from pokeys_py import pokeyslib

class Counter:
    def __init__(self, device):
        self.device = device

    def setup(self, pin):
        if not pokeyslib.PK_IsCounterAvailable(self.device, pin):
            raise ValueError(f"Counter not available on pin {pin}")
        self.device.Pins[pin].PinFunction = pokeyslib.PK_PinCap_digitalCounter
        pokeyslib.PK_PinConfigurationSet(self.device)

    def fetch(self, pin):
        pokeyslib.PK_DigitalCounterGet(self.device)
        return self.device.Pins[pin].DigitalCounterValue

    def clear(self, pin):
        self.device.Pins[pin].DigitalCounterValue = 0
        pokeyslib.PK_DigitalCounterClear(self.device)
