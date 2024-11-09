# Machine Description

## Overview

The machine is a 3-axis milling machine with an additional X2 axis, making it a 4-axis machine in total. It uses a Pokeys57CNC controller and four DM542 drivers for the XYZ and X2 axes.

## Limit Switches

The limit switches are normally closed and are connected to the "PoOptoIn" which is a 16-channel industrial digital input isolator. The configuration files `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini` and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini` show that the limit switches are set to be inverted, which means they are normally closed.

### Wiring

The limit switches for each axis are connected as follows:
- X axis (Joint 0) uses "Ax1 Home" and "Ax1 Limit -"
- X2 axis (Joint 1) uses "Ax6 Home" and "Ax6 Limit -"
- Y axis (Joint 2) uses "Ax2 Home" and "Ax2 Limit -"
- Z axis (Joint 3) uses "Ax3 Home" (with "Ax3 Limit -" reserved and not yet connected)

The "Ax* Limit +" is not connected on any axis.

### Configuration

The `PEv2_EnableLimitP_*` parameters are configured to enable the positive limit switches for AXIS_X and AXIS_X2, but not for AXIS_Y and AXIS_Z. This configuration can be found in the files `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini` and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini`.

The `PEv2_AxesSwitchConfig_*` parameters are set according to the hardware setup and the desired behavior of the limit and home switches. These parameters are configured using bitwise OR combinations of various options, such as `PK_ASO_SWITCH_LIMIT_N`, `PK_ASO_SWITCH_LIMIT_P`, `PK_ASO_SWITCH_HOME`, and others.

### Limit Switches Used

The limit switches used in this setup are NJK-5002C.

### Documentation

The limit switch connection parameterization is documented correctly in the repository.

## Additional Information

The configuration files `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini` and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini` provide detailed information about the limit switches and their configuration. The file `DM542_XXYZ_mill/postgui_call_list.hal` sets the `PEv2_AxesSwitchConfig_*` parameters for each axis, ensuring consistency with the wiring and connections of the limit and home switches to the PoOptoIn.
