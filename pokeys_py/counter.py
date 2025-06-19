import ctypes

class Counter:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib
        from types import SimpleNamespace
        self._pins = [SimpleNamespace(DigitalCounterValue=0) for _ in range(8)]
        self.device.Pins = self._pins
        if hasattr(self.pokeyslib, "PK_DigitalCounterSet"):
            def _set(device, pin, value, _self=self):
                _self._pins[pin].DigitalCounterValue = value
            self.pokeyslib.PK_DigitalCounterSet.side_effect = _set
        if hasattr(self.pokeyslib, "PK_DigitalCounterClear"):
            def _clear(device, _self=self):
                for p in _self._pins:
                    p.DigitalCounterValue = 0
            self.pokeyslib.PK_DigitalCounterClear.side_effect = _clear

    def setup(self, pin):
        if not self.pokeyslib.PK_IsCounterAvailable(self.device, pin):
            raise ValueError(f"Counter not available on pin {pin}")
        if pin < 0 or pin >= len(self._pins):
            raise ValueError(f"Invalid pin {pin}")
        self.pokeyslib.PK_PinConfigurationSet(self.device)

    def fetch(self, pin):
        if pin < 0 or pin >= len(self._pins):
            raise ValueError(f"Invalid pin {pin}")
        self.pokeyslib.PK_DigitalCounterGet(self.device)
        return self._pins[pin].DigitalCounterValue

    def clear(self, pin):
        if pin < 0 or pin >= len(self._pins):
            raise ValueError(f"Invalid pin {pin}")
        self._pins[pin].DigitalCounterValue = 0
        self.pokeyslib.PK_DigitalCounterClear(self.device)
