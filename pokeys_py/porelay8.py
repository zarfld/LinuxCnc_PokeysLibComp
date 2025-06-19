from . import pokeyslib


def _get_lib(lib=None):
    """Return the library to use, falling back to the module level pokeyslib."""
    return lib or pokeyslib

class PoRelay8:
    def __init__(self, device, lib=None):
        self.device = device
        self._lib = lib
        self._state = {}

    def setup(self, module_id):
        lib = _get_lib(self._lib)
        return lib.PK_PoRelay8_GetModuleSettings(self.device, module_id)

    def fetch(self, module_id):
        lib = _get_lib(self._lib)
        lib.PK_PoRelay8_GetModuleStatusRequest(self.device, module_id)
        value = lib.PK_PoRelay8_GetModuleStatus(self.device, module_id)
        return self._state.get(module_id, value)

    def set(self, module_id, data):
        lib = _get_lib(self._lib)
        lib.PK_PoRelay8_SetModuleStatus(self.device, module_id, data)
        self._state[module_id] = data
