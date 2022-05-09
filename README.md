# LinuxCnc_PokeysLibComp
Pokeys comp for LinuxCnc using https://bitbucket.org/mbosnak/pokeyslib.git

started at 
https://forum.linuxcnc.org/24-hal-components/29816-pokeys?start=90#162953

currently under development using following HW
- RaspberryPi4 (for LinuxCnc)
- [x] Pokeys57E (see https://www.poscope.com/product/pokeys57e/)
- [x] PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir (see https://www.poscope.com/product/poextbusoc16cnc/)
- [x] CNC keyboard – PoNETkbd48CNC (see https://www.poscope.com/product/ponet-kbd48cnc/)
- [x] PoRelay8 – Relay extension board (see https://www.poscope.com/product/porelay8/) 
- [x] PoExtBusOC16
- [] Pokeys57CNC 

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
- [p] Matrix LED
- [x] Setting pin functions
- [x] Reading pin functions
- [ ] Setting pin key codes
- [ ] Reading pin key codes
- [p] PWM operations
- [!] Pulse engine operations
- [i] Matrix keyboard setup
- [i] Matrix keyboard reading
- [x] kbd48CNC set LED
- [x] kbd48CNC get Button Status
- [p] Using encoders
- [] I2C operations
- [p] SPI operation
- [ ] PoIL operations
- [i] 1-wire operations
- [i] PoNET bus support

x ... available
i ... implemented but not tested
! ... implemented but not working yet
p ... planned


PoRelay8:
- [x] needs to be connected in Parallel to kbd48CNC with white PoExt Cable OR using Canbus (connecting it using the Red PoExtBus cable to the i2C extender which is delivered with kbd48CNC will not work)
- [x] if it is used together with "non smart" PoExtBus devices (e.g. PoExtBusOC16) use "PoExtBus - Smart" page on Pokeys-SW to set "Sequence ID"
- [ ] inputs not available (yet?)

PoExtBusOC16
- [x] can be set using pokeys.0.PoExtBus.[id].Out-0..15
- [ ] inputs not available

kbd48CNC:
- [x] using as PoNet-extension buard attached to Pokeys57E
- [x] using CanBus (6Pin MicroMatch Connector) directly on Pokeys57CNC


Pokeys57CNC
- [x] connected using USB
- [x] connected using Ethernet
- [ ] LinuxCNC example config in Work (using the pinsetup as available on PCB)
- [x] Switchng OC Outputs  pokeys.0.PEv2.ExternalOCOutput.0..3
- [x] Switchng Relay Outputs (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1) (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1)
- using USB connection seems "faster" than using ethernet - may depend on networksetup (networkswitch vs. direct connection)


PulseEnginev2
- [x] Setting Status Running/Stop depending on LinuxCNC state ("machine-on")
- [x] Reading Status of Limit+/- and Home switches
- [x] setting External OC and RelayOutputs - in case that "Extended IO" is available and activated (Pokeys57CNC)
- [!] setting PinConfiguration for Limits, Home, Probe & Emergency switches based in LinuxCNC-INI configuration
- [x] parametrizing MaxSpeed, MaxAccelleration, ... based on LinuxCNC configuarion (mm/sec to pulses/sec conversion is being done using JOINT.*.STEPSCALE)
- [ ] moving axis currently doesn't work yet due to FollowingError (FERROR)