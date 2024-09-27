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

    def start_homing(self, axis):
        """
        Start the homing procedure for an axis.
        :param axis: Axis number
        """
        pokeyslib.PK_PEv2_StartHoming(self.device, axis)

    def cancel_homing(self, axis):
        """
        Cancel the homing procedure for an axis.
        :param axis: Axis number
        """
        pokeyslib.PK_PEv2_CancelHoming(self.device, axis)

    def get_homing_status(self, axis):
        """
        Get the homing status for an axis.
        :param axis: Axis number
        :return: Homing status
        """
        return pokeyslib.PK_PEv2_GetHomingStatus(self.device, axis)

    def manage_homing_signals(self, axis):
        """
        Manage the homing signals for an axis.
        :param axis: Axis number
        """
        homing_status = self.get_homing_status(axis)
        if homing_status == 11:  # PK_PEAxisState_axHOMINGSTART
            self.start_homing(axis)
        elif homing_status == 12:  # PK_PEAxisState_axHOMINGSEARCH
            self.set_velocity(axis, 0.5)  # Example velocity value
        elif homing_status == 13:  # PK_PEAxisState_axHOMINGBACK
            self.set_velocity(axis, 0.1)  # Example velocity value
        elif homing_status == 10:  # PK_PEAxisState_axHOME
            self.set_position(axis, 0)  # Example position value
        else:
            self.cancel_homing(axis)
