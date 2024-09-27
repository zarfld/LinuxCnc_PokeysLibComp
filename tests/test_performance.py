import time
import unittest
from unittest.mock import MagicMock, patch
from pokeys_py import digital_io, analog_io, pwm, counter, ponet, pev2_motion_control

class TestPerformance(unittest.TestCase):

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_digital_io_performance(self, mock_pokeyslib):
        device = MagicMock()
        dio = digital_io.DigitalIO(device)
        start_time = time.time()
        for _ in range(1000):
            dio.set(1, 1)
            dio.fetch(1)
        end_time = time.time()
        latency = (end_time - start_time) / 1000
        self.assertLess(latency, 0.001, "Digital IO latency is too high")

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_analog_io_performance(self, mock_pokeyslib):
        device = MagicMock()
        aio = analog_io.AnalogIO(device)
        start_time = time.time()
        for _ in range(1000):
            aio.set(1, 1.0)
            aio.fetch(1)
        end_time = time.time()
        latency = (end_time - start_time) / 1000
        self.assertLess(latency, 0.001, "Analog IO latency is too high")

    @patch('pokeys_py.pwm.pokeyslib')
    def test_pwm_performance(self, mock_pokeyslib):
        device = MagicMock()
        pwm_io = pwm.PWM(device)
        start_time = time.time()
        for _ in range(1000):
            pwm_io.set(1, 1000, 50)
            pwm_io.fetch(1)
        end_time = time.time()
        latency = (end_time - start_time) / 1000
        self.assertLess(latency, 0.001, "PWM latency is too high")

    @patch('pokeys_py.counter.pokeyslib')
    def test_counter_performance(self, mock_pokeyslib):
        device = MagicMock()
        counter_io = counter.Counter(device)
        start_time = time.time()
        for _ in range(1000):
            counter_io.clear(1)
            counter_io.fetch(1)
        end_time = time.time()
        latency = (end_time - start_time) / 1000
        self.assertLess(latency, 0.001, "Counter latency is too high")

    @patch('pokeys_py.ponet.pokeyslib')
    def test_ponet_performance(self, mock_pokeyslib):
        device = MagicMock()
        ponet_io = ponet.PoNET(device)
        start_time = time.time()
        for _ in range(1000):
            ponet_io.set(1, 1)
            ponet_io.fetch(1)
        end_time = time.time()
        latency = (end_time - start_time) / 1000
        self.assertLess(latency, 0.001, "PoNET latency is too high")

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_pev2_motion_control_performance(self, mock_pokeyslib):
        device = MagicMock()
        pev2_io = pev2_motion_control.PEv2MotionControl(device)
        start_time = time.time()
        for _ in range(1000):
            pev2_io.set_position(1, 1000)
            pev2_io.fetch_status()
        end_time = time.time()
        latency = (end_time - start_time) / 1000
        self.assertLess(latency, 0.001, "PEv2 Motion Control latency is too high")

    @patch('pokeys_py.digital_io.pokeyslib')
    def test_digital_io_error_conditions(self, mock_pokeyslib):
        device = MagicMock()
        dio = digital_io.DigitalIO(device)
        with self.assertRaises(ValueError):
            dio.set(99, 1)
        with self.assertRaises(ValueError):
            dio.fetch(99)

    @patch('pokeys_py.analog_io.pokeyslib')
    def test_analog_io_error_conditions(self, mock_pokeyslib):
        device = MagicMock()
        aio = analog_io.AnalogIO(device)
        with self.assertRaises(ValueError):
            aio.set(99, 1.0)
        with self.assertRaises(ValueError):
            aio.fetch(99)

    @patch('pokeys_py.pwm.pokeyslib')
    def test_pwm_error_conditions(self, mock_pokeyslib):
        device = MagicMock()
        pwm_io = pwm.PWM(device)
        with self.assertRaises(ValueError):
            pwm_io.set(99, 1000, 50)
        with self.assertRaises(ValueError):
            pwm_io.fetch(99)

    @patch('pokeys_py.counter.pokeyslib')
    def test_counter_error_conditions(self, mock_pokeyslib):
        device = MagicMock()
        counter_io = counter.Counter(device)
        with self.assertRaises(ValueError):
            counter_io.clear(99)
        with self.assertRaises(ValueError):
            counter_io.fetch(99)

    @patch('pokeys_py.ponet.pokeyslib')
    def test_ponet_error_conditions(self, mock_pokeyslib):
        device = MagicMock()
        ponet_io = ponet.PoNET(device)
        with self.assertRaises(ValueError):
            ponet_io.set(99, 1)
        with self.assertRaises(ValueError):
            ponet_io.fetch(99)

    @patch('pokeys_py.pev2_motion_control.pokeyslib')
    def test_pev2_motion_control_error_conditions(self, mock_pokeyslib):
        device = MagicMock()
        pev2_io = pev2_motion_control.PEv2MotionControl(device)
        with self.assertRaises(ValueError):
            pev2_io.set_position(99, 1000)
        with self.assertRaises(ValueError):
            pev2_io.fetch_status()

    @patch('pokeys_py.pokeyslib')
    def test_performance_under_load(self, mock_pokeyslib):
        device = MagicMock()
        dio = digital_io.DigitalIO(device)
        aio = analog_io.AnalogIO(device)
        pwm_io = pwm.PWM(device)
        counter_io = counter.Counter(device)
        ponet_io = ponet.PoNET(device)
        pev2_io = pev2_motion_control.PEv2MotionControl(device)

        start_time = time.time()
        for _ in range(1000):
            dio.set(1, 1)
            dio.fetch(1)
            aio.set(1, 1.0)
            aio.fetch(1)
            pwm_io.set(1, 1000, 50)
            pwm_io.fetch(1)
            counter_io.clear(1)
            counter_io.fetch(1)
            ponet_io.set(1, 1)
            ponet_io.fetch(1)
            pev2_io.set_position(1, 1000)
            pev2_io.fetch_status()
        end_time = time.time()
        latency = (end_time - start_time) / 1000
        self.assertLess(latency, 0.005, "System latency under load is too high")

if __name__ == '__main__':
    unittest.main()
