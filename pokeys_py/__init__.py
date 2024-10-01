# Initialize the pokeys_py package

__all__ = ["digital_io", "analog_io", "pwm", "counter", "ponet", "pev2_motion_control"]

import os

if os.getenv('CI') == 'true':
    from tests.mocks.pokeyslib import pokeyslib
else:
    import ctypes
    pokeyslib = ctypes.CDLL('/usr/lib/linuxcnc/modules/PoKeysLib.so')

from .telemetry import Telemetry

# Initialize telemetry collection if the user has opted in
telemetry = Telemetry(dsn="your_sentry_dsn_here", opt_in=True)
telemetry.initialize_sentry()
