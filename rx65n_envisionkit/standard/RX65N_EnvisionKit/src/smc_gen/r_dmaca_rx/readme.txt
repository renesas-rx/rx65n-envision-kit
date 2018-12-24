PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_dmaca_rx
=========

Document Number 
---------------
R01AN2063EJ0105
R01AN2063JJ0105

Version
-------
v1.05

Overview
--------
The DMACA driver provides a method to transmit the data using the Direct Memory Access Controller (DMAC).
The driver includes API functions to initialize DMAC, create Transfer data, Control and get status of DMAC.
The driver can be reduced in size by removing code used for parameter checking. 
All configuration options can be found in "r_config\r_dmaca_rx_config.h". 
An original copy of the configuration file is stored in "r_dmaca_rx\ref\r_dmaca_rx_config_reference.h".

Features
--------
* Support Normal mode, Repeat mode and Block mode
* Support chain transfer

Supported MCUs
--------------
* RX230 Group
* RX231 Group
* RX64M Group
* RX65N Group
* RX71M Group

Boards Tested On
----------------
* RSKRX230
* RSKRX231
* RSKRX64M
* RSKRX65N
* RSKRX65N_2MB
* RSKRX71M

Limitations
-----------
* None

Peripherals Used Directly
-------------------------
* Direct Memory Access Controller (DMAC)

Required Packages
-----------------
* r_bsp v3.60

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (3).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (2) or (4) for unsupported RX devices.

(1)	Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2)	Adding the FIT module to your project using "FIT Configurator" in e2 studio
By using the "FIT Configurator" in e2 studio,
the FIT module is automatically added to your project.
Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3)	Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(4)	Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

Toolchain(s) Used
-----------------
* Renesas RX v2.07 release 00

File Structure
--------------
r_dmaca_rx
|   r_dmaca_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an2063ej0105-rx.pdf
|   +---ja
|           r01an2063jj0105-rx.pdf
|
+---ref
|       r_dmaca_rx_config_reference.h
|
+---src
    |   r_dmaca_rx.c
    |   r_dmaca_rx_private.h
    |
    +---targets
        |
        +---rx64m
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx65n
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx71m
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx230
        |       r_dmaca_rx_target.c
        |       r_dmaca_rx_target.h
        |
        +---rx231
                r_dmaca_rx_target.c
                r_dmaca_rx_target.h

r_config
    r_dmaca_rx_config.h
