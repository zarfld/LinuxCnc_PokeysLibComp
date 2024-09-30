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
        try:
            pokeyslib.PK_PoNETGetModuleSettings(self.device, module_id)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")

    def fetch(self, module_id):
        """
        Fetch data from a PoNET module.
        :param module_id: Module ID
        :return: Module data
        """
        try:
            pokeyslib.PK_PoNETGetModuleStatusRequest(self.device, module_id)
            return pokeyslib.PK_PoNETGetModuleStatus(self.device, module_id)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")

    def set(self, module_id, data):
        """
        Send data to a PoNET module.
        :param module_id: Module ID
        :param data: Data to send
        """
        try:
            pokeyslib.PK_PoNETSetModuleStatus(self.device, module_id, data)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")

    def get_module_info(self, module_id):
        """
        Get information about a PoNET module.
        :param module_id: Module ID
        :return: Module information
        """
        try:
            return pokeyslib.PK_PoNETGetModuleInfo(self.device, module_id)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")

    def reset_module(self, module_id):
        """
        Reset a PoNET module.
        :param module_id: Module ID
        """
        try:
            pokeyslib.PK_PoNETResetModule(self.device, module_id)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")
