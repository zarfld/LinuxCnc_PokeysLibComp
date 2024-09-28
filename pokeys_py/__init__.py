# Initialize the pokeys_py package

__all__ = ["digital_io", "analog_io", "pwm", "counter", "ponet", "pev2_motion_control"]

import os

if os.getenv('CI') == 'true':
    from tests.mocks.pokeyslib_mock import pokeyslib
else:
    import ctypes
    pokeyslib = ctypes.CDLL('/usr/lib/linuxcnc/modules/PoKeysLib.so')
