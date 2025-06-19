from . import pokeyslib

class PoRelay8:
    def __init__(self, device, lib=None):
        self.device = device
        self.lib = lib or pokeyslib

    def setup(self, module_id):
        return self.lib.PK_PoRelay8_GetModuleSettings(self.device, module_id)

    def fetch(self, module_id):
        self.lib.PK_PoRelay8_GetModuleStatusRequest(self.device, module_id)
        return self.lib.PK_PoRelay8_GetModuleStatus(self.device, module_id)

    def set(self, module_id, data):
        self.lib.PK_PoRelay8_SetModuleStatus(self.device, module_id, data)
