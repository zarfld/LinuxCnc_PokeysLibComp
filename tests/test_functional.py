import unittest
from unittest.mock import MagicMock, patch
from pokeys_py.pev2_motion_control import PEv2MotionControl

class TestFunctionalPEv2MotionControl(unittest.TestCase):
    def setUp(self):
        self.device = MagicMock()
        self.pev2 = PEv2MotionControl(self.device)

    def test_motion_control_sequences(self):
        # Test starting motion control
        self.pev2.set_position(0, 100)
        self.device.PK_PEv2_SetPosition.assert_called_with(self.device, 0, 100)

        # Test stopping motion control
        self.pev2.set_velocity(0, 0)
        self.device.PK_PEv2_SetVelocity.assert_called_with(self.device, 0, 0)

        # Test emergency stop
        self.pev2.set_velocity(0, -1)
        self.device.PK_PEv2_SetVelocity.assert_called_with(self.device, 0, -1)

    def test_homing_procedures(self):
        # Test starting homing
        self.pev2.start_homing(0)
        self.device.PK_PEv2_StartHoming.assert_called_with(self.device, 0)

        # Test canceling homing
        self.pev2.cancel_homing(0)
        self.device.PK_PEv2_CancelHoming.assert_called_with(self.device, 0)

        # Test getting homing status
        self.pev2.get_homing_status(0)
        self.device.PK_PEv2_GetHomingStatus.assert_called_with(self.device, 0)

if __name__ == '__main__':
    unittest.main()
