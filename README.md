# LinuxCnc_PokeysLibComp
Pokeys comp for LinuxCnc using https://bitbucket.org/mbosnak/pokeyslib.git

started at 
https://forum.linuxcnc.org/24-hal-components/29816-pokeys?start=90#162953

## installation
```bash
git clone https://github.com/zarfld/LinuxCnc_PokeysLibComp.git
cd LinuxCnc_PokeysLibComp
sudo sh install.sh
```

## currently under development using following HW
- RaspberryPi4 (for LinuxCnc)
- [x] Pokeys57E (see https://www.poscope.com/product/pokeys57e/)
- [x] PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir (see https://www.poscope.com/product/poextbusoc16cnc/)
- [x] CNC keyboard – PoNETkbd48CNC (see https://www.poscope.com/product/ponet-kbd48cnc/)
- [x] PoRelay8 – Relay extension board (see https://www.poscope.com/product/porelay8/) 
- [x] PoExtBusOC16
- [x] Pokeys57CNC 
- [ ] PoStep25-32 (v1.3) 

---- Library status - verified commands ----

- [x] Connecting to USB devices
- [x] Connecting to network devices
- [x] Reading device data
- [x] Reading digital inputs
- [x] Writing digital outputs
- [x] Reading analog inputs
- [x] PoExtBus writing
- [x] PoExtBus reading
- [ ] LCD operations
- [ ] Matrix LED
- [x] Setting pin functions
- [x] Reading pin functions
- [ ] Setting pin key codes
- [ ] Reading pin key codes
- [X] PWM operations
- [x] Pulse engine operations
- [i] Matrix keyboard setup
- [i] Matrix keyboard reading
- [x] kbd48CNC set LED
- [x] kbd48CNC get Button Status
- [x] Using encoders
- [ ] I2C operations
- [ ] SPI operation
- [ ] PoIL operations
- [i] 1-wire operations


x ... available
i ... implemented but not tested
! ... implemented but not working yet
p ... planned


### PoRelay8:
- [x] needs to be connected in Parallel to kbd48CNC with white PoExt Cable OR using Canbus (connecting it using the Red PoExtBus cable to the i2C extender which is delivered with kbd48CNC will not work)
- [x] if it is used together with "non smart" PoExtBus devices (e.g. PoExtBusOC16) use "PoExtBus - Smart" page on Pokeys-SW to set "Sequence ID"
- [ ] inputs not available (yet?)

### PoExtBusOC16
- [x] can be set using pokeys.0.PoExtBus.#.Out-0..7  (all 16outs can be used pokeys.0.PoExtBus.0 and pokeys.0.PoExtBus.1)
- [ ] inputs not available

### PoExtBusOC16CNC
- see "PulseEnginev2"

### kbd48CNC:
- [x] using as PoNet-extension buard attached to Pokeys57E
- [x] using CanBus (6Pin MicroMatch Connector) directly on Pokeys57CNC

### Pokeys57E
- [x] connected using Ethernet
- [x] Read&Set of Digital IO on HalPins pokeys.0.Pins.*.DigitalValueGet / DigitalValueSet
- [x] Readt of AnalogValue IO on HalPins pokeys.0.Pins.*.AnalogValue on the pins that support that
- periphals listed above was tested on this device.

changing structure of IOs to "Canonical Device Interface" in work

### Pokeys57CNC
- [x] connected using USB
- [x] connected using Ethernet
- [x] LinuxCNC example config in Work (using the pinsetup as available on PCB)
- [x] Switchng OC Outputs  pokeys.0.PEv2.ExternalOCOutput.0..3
- [x] Switchng Relay Outputs (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1) (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1)
- using USB connection seems "faster" than using ethernet - may depend on networksetup (networkswitch vs. direct connection)


### PulseEnginev2 
- [x] Setting Status Running/Stop depending on LinuxCNC state ("machine-on")
- [x] Reading Status of Limit+/- and Home switches
- [x] setting External OC and RelayOutputs - in case that "Extended IO" is available and activated (Pokeys57CNC)
- [x] setting PinConfiguration for Limits, Home, Probe & Emergency switches based in LinuxCNC-INI configuration - not reliable
- [x] parametrizing MaxSpeed, MaxAccelleration, ... based on LinuxCNC configuarion (mm/sec to pulses/sec conversion is being done using JOINT.*.STEPSCALE)


## HAL-Interface

### Digital Input
Count: 55
#### Pins
- pokeys.[DevID].digin.[PinID].in			bit		State of the hardware input
- pokeys.[DevID].digin.[PinID].in-not		bit		Inverted state of the input.
##### for PoExtBus Devices (currently prepared for future Firmware)
- pokeys.[DevID].PoExtBus.[PoExtBusId]digin.[PinID].in			bit		State of the PoExtBus input (currently prepared for future Firmware)
- pokeys.[DevID].PoExtBus.[PoExtBusId]digin.[PinID].in-not		bit		Inverted state of the PoExtBus input.
##### for PulseEnginev2
- pokeys.[DevID].PEv2.digin.Emergency.in
- pokeys.[DevID].PEv2.digin.Probe.in
 ##### for each Axis of PulseEnginev2
- pokeys.[DevID].PEv2.[PEv2Id].digin.Probe.in
- pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in			bit		State of the  Home input for Joint[PEv2Id]
- pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in-not		bit		Inverted state of the Home input for Joint[PEv2Id]
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in			bit		State of the  Home input for Joint[PEv2Id]
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in-not		bit		Inverted state of the Home input for Joint[PEv2Id]
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in			bit		State of the  Home input for Joint[PEv2Id]
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in-not		bit		Inverted state of the Home input for Joint[PEv2Id]
- pokeys.[DevID].PEv2.[PEv2Id].digin.AxisEnabled.in
##### for each Button on kbd48CNC 
- pokeys.[DevID].kbd48CNC.[ButtonId].Button					// will get True if Button pressed

#### Parameters
- pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Pin					"Home switch pin (0 for external dedicated input)";			// Limit+ switch pin (0 for external dedicated input)
- pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Filter				"Digital filter for Home switch";		// Digital filter for limit+ switch
- pokeys.[DevID].PEv2.[PEv2Id].digin.Home.invert				"Invert Home (pokeys functionality) ";
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Pin				"Limit- switch pin (0 for external dedicated input)";			// Limit+ switch pin (0 for external dedicated input)
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Filter				"Digital filter for limit- switch";		// Digital filter for limit+ switch
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.invert				"Invert limit- (pokeys functionality) ";
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Pin				"Limit+ switch pin (0 for external dedicated input)";			// Limit+ switch pin (0 for external dedicated input)
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Filter				"Digital filter for limit+ switch";		// Digital filter for limit+ switch
- pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.invert				"Invert limit+ (pokeys functionality) ";
- pokeys.[DevID].PEv2.digin.Emergency.Pin
- pokeys.[DevID].PEv2.digin.Emergency.invert
- pokeys.[DevID].PEv2.digin.Probe.Pin
- pokeys.[DevID].PEv2.digin.Probe.invert


### Digital Output
Count: 55
#### Pins
- pokeys.[DevID].digout.[PinID].out			bit		Value to be written (possibly inverted) to the hardware output.
##### for PoExtBus Devices (e.g. PoRelay8 or PoExtBusOC16)
  - pokeys.[DevID].PoExtBus.[PoExtBusId]digout.[PinID].out			bit		Value to be written (possibly inverted) to the PoExtBus output.
##### for PulseEnginev2
  - pokeys.0.PEv2.digout.Emergency.out
- pokeys.[DevID].PEv2.[PEv2Id].digout.AxisEnabled.out
- pokeys.[DevID].PEv2.[PEv2Id].digout.LimitOverride.out
- pokeys.[DevID].PEv2.digout.ExternalRelay-[RelId].out;			// External relay outputs
- pokeys.[DevID].PEv2.digout.ExternalOC-[OcId].out;			// External open-collector outputs
##### for each Button on kbd48CNC 
- pokeys.[DevID].kbd48CNC.[ButtonId].LED					// switch Button LED On/Off

#### Parameters
- pokeys.[DevID].digout.[PinID].invert		bit		If TRUE, out is inverted before writing to the hardware.
- pokeys.[DevID].PoExtBus.[PoExtBusId]digout.[PinID].invert		bit		If TRUE, out is inverted before writing to the PoExtBus output.
- pokeys.0.PEv2.digout.Emergency.Pin

### Analog Input
Count: 7
#### Pins
- pokeys.[DevID].adcin.[AdcId].value-raw			float	The hardware reading 
- pokeys.[DevID].adcin.[AdcId].value				float	The hardware reading, scaled according to the scale and offset parameters.

#### Parameters
- pokeys.[DevID].adcin.[AdcId].scale  "The input voltage (or current) will be multiplied by scale before being output to value.";
- pokeys.[DevID].adcin.[AdcId].offset "This will be subtracted from the hardware input voltage (or current) after the scale multiplier has been applied.";

### Analog Output
Count: 6
#### Pins
- pokeys.[DevID].adcout.[AdcId].value;			 The value to be written. The actual value output to the hardware will depend on the scale and offset parameters.
- pokeys.[DevID].adcout.[AdcId].enable;		If false, then output 0 to the hardware, regardless of the value pin.

#### Parameters
- pokeys.[DevID].adcout.[AdcId].offset  "This will be added to the value before the hardware is updated";
- pokeys.[DevID].adcout.[AdcId].scale  "This should be set so that an input of 1 on the value pin will cause 1V";
- pokeys.[DevID].adcout.[AdcId].high_limit  "When calculating the value to output to the hardware, if value +offset is greater than high_limit, then high_limit will be used instead.";
- pokeys.[DevID].adcout.[AdcId].low_limit  "When calculating the value to output to the hardware, if value +offset is less than low_limit, then low_limit will be used instead";
- pokeys.[DevID].adcout.[AdcId].max_v "max output-voltage";
- pokeys.[DevID].float adcout.pwm.period  "PWM period, shared among all channels"

### Encoder
Count: 29
#### Pins
- pokeys.[DevID].encoder.[EncId].count;			S32	Encoder value in counts.
- pokeys.[DevID].encoder.[EncId].position;		FLOAT Encoder value in position units (see parameter “scale”).
- ~~pokeys.[DevID].encoder.[EncId].velocity;		FLOAT Velocity in position units per second~~
- pokeys.[DevID].encoder.[EncId].reset;			BIT	When True, force counter to zero
- ~~pokeys.[DevID].encoder.[EncId].index-enable;	BIT	(bidirectional) When True, reset to zero on next index pulse, and set pin False.~~

#### Parameters
- pokeys.[DevID]. encoder.[EncId].scale	FLOAT	"The scale factor used to convert counts to position units. It is in “counts per position unit”";

### RealTimeClock

#### Pins
- pokeys.[DevID].rtc.sec;				unsigned Second
- pokeys.[DevID].rtc.min;				unsigned Minute
- pokeys.[DevID].rtc.hour;				unsigned Hour	
- pokeys.[DevID].rtc.dow;				unsigned Day of week
- pokeys.[DevID].rtc.dom;				unsigned Day of month
- pokeys.[DevID].rtc.doy;				unsigned Day of year
- pokeys.[DevID].rtc.month;				unsigned Month
- pokeys.[DevID].rtc.year;				unsigned Year
- pokeys.[DevID].rtc.loop_frequ;		unsigned Actual Loop frequency of pokeys.comp updated after rts.sec changed
- pokeys.[DevID].rtc.loop_frequ_demand;	unsigned demand value for loop frequency (if 0 default of 10Hz will be used)

## Running Tests

To ensure the reliability and correctness of the `pokeys_py` component, a comprehensive test suite has been developed. The tests cover various functionalities, including digital I/O, analog I/O, PEv2, counters, PWM outputs, and peripheral devices. The tests are categorized into unit tests, integration tests, functional tests, and performance tests.

### Prerequisites

Before running the tests, make sure you have the following prerequisites installed:

- Python 3.x
- `pytest` testing framework
- `unittest.mock` for mocking PoKeys device responses

You can install `pytest` using pip:

```bash
pip install pytest
```

### Running Unit Tests

Unit tests are designed to test individual functions and methods within the `pokeys_py` component. To run the unit tests, navigate to the `tests` directory and execute the following command:

```bash
pytest test_analog_io.py
pytest test_counter.py
pytest test_digital_io.py
pytest test_pev2_motion_control.py
pytest test_ponet.py
pytest test_pwm.py
```

### Running Integration Tests

Integration tests verify the interaction between the `pokeys_py` component and the PoKeys hardware and LinuxCNC. To run the integration tests, execute the following command:

```bash
pytest test_integration.py
```

### Running Functional Tests

Functional tests simulate real-world usage scenarios for the `pokeys_py` component. To run the functional tests, execute the following command:

```bash
pytest test_functional.py
```

### Running Performance Tests

Performance tests ensure that the `pokeys_py` component meets real-time operation requirements. To run the performance tests, execute the following command:

```bash
pytest test_performance.py
```

### Interpreting Test Results

After running the tests, `pytest` will display the test results in the terminal. The results will indicate the number of tests passed, failed, or skipped. Additionally, you can generate a coverage report to identify untested code.

To generate a coverage report, use the following command:

```bash
pytest --cov=pokeys_py --cov-report=html
```

The coverage report will be generated in the `htmlcov` directory. Open the `index.html` file in a web browser to view the detailed coverage report.

### Hardware-Dependent Tests

Some tests may require physical hardware to run. For hardware-dependent tests, make sure the PoKeys device is connected and properly configured. Follow the setup steps provided in the documentation to ensure the hardware is ready for testing.

### Continuous Integration (CI)

To integrate the tests into an automated testing pipeline, set up continuous integration (CI) to run the tests on every commit or pull request. This ensures that the code is continuously tested and any issues are detected early in the development process.

### Running Tests with Mocks or Simulators

To run tests with mocks or simulators, follow these steps:

1. Set the environment variable `CI` to `true` to enable the use of mocks in the CI environment.
2. Run the tests as usual using `pytest`.

For example:

```bash
export CI=true
pytest test_analog_io.py
pytest test_counter.py
pytest test_digital_io.py
pytest test_pev2_motion_control.py
pytest test_ponet.py
pytest test_pwm.py
pytest test_integration.py
pytest test_functional.py
pytest test_performance.py
```

### Setting Environment Variables

To toggle between real and mock devices, set the `CI` environment variable accordingly:

- For running tests with real hardware, unset the `CI` environment variable or set it to `false`:

```bash
unset CI
# or
export CI=false
```

- For running tests with mocks or simulators, set the `CI` environment variable to `true`:

```bash
export CI=true
```

## Documentation

The documentation provides clear instructions on running tests, setup steps for hardware-dependent tests, and interpreting test results and coverage reports. It is essential to keep the documentation up to date as new features are implemented and tests are added.

By following these instructions, you can ensure the reliability, correctness, and performance of the `pokeys_py` component, making it easier to maintain and develop in the future.

## Configuring GitHub Actions Runners

To ensure that the CI workflow runs smoothly, it is important to configure GitHub Actions runners properly. This section provides instructions for setting up self-hosted runners and verifying the required permissions and access for runners.

### Setting Up Self-Hosted Runners

1. **Create a Runner**: Follow the instructions in the GitHub documentation to create a self-hosted runner. You can find the documentation [here](https://docs.github.com/en/actions/hosting-your-own-runners/adding-self-hosted-runners).

2. **Install the Runner**: Download and install the runner on your machine. Make sure to follow the installation steps provided in the documentation.

3. **Configure the Runner**: Configure the runner by running the `config.sh` script. Provide the necessary details such as the repository URL, runner name, and labels.

4. **Start the Runner**: Start the runner by running the `run.sh` script. The runner should now be online and ready to pick up jobs.

### Verifying Permissions and Access

1. **Repository Access**: Ensure that the runner has access to the repository. This can be done by adding the runner to the repository settings under the "Actions" tab.

2. **Permissions**: Verify that the runner has the necessary permissions to access the repository. This includes read and write permissions for the repository.

3. **Runner Logs**: Check the runner logs for any errors or issues that might be preventing the job from being picked up. The logs can be found in the `runner/logs` directory.

4. **Self-Hosted Runners**: If using self-hosted runners, ensure they are online and properly configured. This includes verifying the network connectivity and ensuring that the runner is not blocked by any firewall or security settings.

5. **GitHub-Hosted Runners**: If using GitHub-hosted runners, verify that there are no restrictions or limitations on the number of concurrent jobs. This can be done by checking the GitHub Actions settings in the repository.

By following these steps, you can ensure that the GitHub Actions runners are properly configured and have the necessary permissions and access to run the CI workflow successfully.
