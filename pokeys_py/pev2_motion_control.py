import ctypes

# Load the PoKeysLib shared library
pokeyslib = ctypes.CDLL('./pokeys_rt/PoKeysLib.so')

class PEv2MotionControl:
    def __init__(self, device):
        self.device = device

    def setup(self, axis, parameters):
        """
        Configure an axis for motion control.
        :param axis: Axis number
        :param parameters: Dictionary of parameters
        """
        pokeyslib.PK_PEv2_AxisConfigurationSet(self.device, axis, parameters)

    def fetch_status(self):
        """
        Retrieve the current motion status and position feedback.
        :return: Status and position feedback
        """
        status = pokeyslib.PK_PEv2_StatusGet(self.device)
        position = pokeyslib.PK_PEv2_GetPosition(self.device)
        return status, position

    def set_position(self, axis, position):
        """
        Send position command to an axis.
        :param axis: Axis number
        :param position: Position value
        """
        pokeyslib.PK_PEv2_SetPosition(self.device, axis, position)

    def set_velocity(self, axis, velocity):
        """
        Send velocity command to an axis.
        :param axis: Axis number
        :param velocity: Velocity value
        """
        pokeyslib.PK_PEv2_SetVelocity(self.device, axis, velocity)
