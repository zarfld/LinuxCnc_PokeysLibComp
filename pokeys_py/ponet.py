import ctypes

# Load the PoKeysLib shared library
pokeyslib = ctypes.CDLL('./pokeys_rt/PoKeysLib.so')

class PoNET:
    def __init__(self, device):
        self.device = device

    def setup(self, module_id):
        """
        Setup a PoNET module.
        :param module_id: Module ID
        """
        pokeyslib.PK_PoNETGetModuleSettings(self.device, module_id)

    def fetch(self, module_id):
        """
        Fetch data from a PoNET module.
        :param module_id: Module ID
        :return: Module data
        """
        pokeyslib.PK_PoNETGetModuleStatusRequest(self.device, module_id)
        return pokeyslib.PK_PoNETGetModuleStatus(self.device, module_id)

    def set(self, module_id, data):
        """
        Send data to a PoNET module.
        :param module_id: Module ID
        :param data: Data to send
        """
        pokeyslib.PK_PoNETSetModuleStatus(self.device, module_id, data)
