# LinuxCnc_PokeysLibComp
Pokeys comp for LinuxCnc using https://bitbucket.org/mbosnak/pokeyslib.git

started at 
https://forum.linuxcnc.org/24-hal-components/29816-pokeys?start=90#162953

currently under development using following HW
- RaspberryPi4 (for LinuxCnc)
- Pokeys57E (see https://www.poscope.com/product/pokeys57e/)
- PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir (see https://www.poscope.com/product/poextbusoc16cnc/)
- CNC keyboard – PoNETkbd48CNC (not tested yet - see https://www.poscope.com/product/ponet-kbd48cnc/)
- PoRelay8 – Relay extension board (not tested yet - see https://www.poscope.com/product/porelay8/) 

reading:
    - Pins (DigitalIn, DigitalOuts, AnalogIn)
    - started implementing PEv2 (see https://www.poscope.com/wp-content/uploads/downloads/Pokeys/Manuals/PoKeys%20Pulse%20engine%20v2%20manual%20(6.3.2016).pdf)



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
- [x] kbd48CNC get Button Status (currently poor perfromance on Sapberry)
- [p] Using encoders
- [p] I2C operations
- [p] SPI operation
- [p] PoIL operations
- [i] 1-wire operations
- [i] PoNET bus support
- [!] PoRelay8 & PoExtBus-Smart (missing in pokeyslib - see PoRelay Manual for  ommand-reference)

x ... available
i ... implemented but not tested
! ... implemented but not working yet
p ... planned


PoRelay8
PoRelay