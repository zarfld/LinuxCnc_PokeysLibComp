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
- needs to be connected in Parallel to kbd48CNC with white PoExt Cable OR using Canbus (connecting it using the Red PoExtBus cable to the i2C extender which is delivered with kbd48CNC will not work)
- if it is used together with "non smart" PoExtBus devices (e.g. PoExtBusOC16) use "PoExtBus - Smart" page on Pokeys-SW to set "Sequence ID"

kbd48CNC:
- [x] using as PoNet-extension buard attached to Pokeys57E
- [x] using CanBus (6Pin MicroMatch Connector) directly on Pokeys57CNC


Pokeys57CNC
- [x] connected using USB
- [x] connected using Ethernet
- [ ] LinuxCNC example config in Work


PulseEnginev2
- [x] connected using USB