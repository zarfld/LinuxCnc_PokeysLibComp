import ctypes

class Counter:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib

    def setup(self, pin):
        if not self.pokeyslib.PK_IsCounterAvailable(self.device, pin):
            raise ValueError(f"Counter not available on pin {pin}")
        self.device.Pins[pin].PinFunction = self.pokeyslib.PK_PinCap_digitalCounter
        self.pokeyslib.PK_PinConfigurationSet(self.device)

    def fetch(self, pin):
        self.pokeyslib.PK_DigitalCounterGet(self.device)
        return self.device.Pins[pin].DigitalCounterValue

    def clear(self, pin):
        self.device.Pins[pin].DigitalCounterValue = 0
        self.pokeyslib.PK_DigitalCounterClear(self.device)
