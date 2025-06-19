import ctypes


class PoNET:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib

    def setup(self, module_id):
        """
        Setup a PoNET module.
        :param module_id: Module ID
        """
        try:
            self.pokeyslib.PK_PoNETGetModuleSettings(self.device, module_id)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")

    def fetch(self, module_id):
        """
        Fetch data from a PoNET module.
        :param module_id: Module ID
        :return: Module data
        """
        try:
            self.pokeyslib.PK_PoNETGetModuleStatusRequest(self.device, module_id)
            return self.pokeyslib.PK_PoNETGetModuleStatus(self.device, module_id)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")

    def set(self, module_id, data):
        """
        Send data to a PoNET module.
        :param module_id: Module ID
        :param data: Data to send
        """
        try:
            self.pokeyslib.PK_PoNETSetModuleStatus(self.device, module_id, data)
        except ValueError:
            raise ValueError(f"Invalid module ID {module_id}")
