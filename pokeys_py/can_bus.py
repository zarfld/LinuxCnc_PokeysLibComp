import ctypes

class CANBus:
    def __init__(self, device, pokeyslib):
        self.device = device
        self.pokeyslib = pokeyslib

    def configure(self, bitrate):
        return self.pokeyslib.PK_CANConfigure(self.device, bitrate)

    def register_filter(self, filter_id, can_id):
        return self.pokeyslib.PK_CANRegisterFilter(self.device, filter_id, can_id)

    def write(self, msg):
        return self.pokeyslib.PK_CANWrite(self.device, msg)

    def read(self, msg, status):
        return self.pokeyslib.PK_CANRead(self.device, msg, status)

    def flush(self):
        return self.pokeyslib.PK_CANFlush(self.device)
