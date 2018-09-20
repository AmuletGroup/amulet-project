#### Table of Contents

- [Obtain an Amulet](#obtain-an-amulet)
- [Setup an Amulet Build Environment on Mac OSX](#setup-an-amulet-build-environment-on-mac-osx)
- [Setup an Amulet Build Environment on Linux](#setup-an-amulet-build-environment-on-linux)
- [Setup an Amulet Build Environment on Windows](#setup-an-amulet-build-environment-on-windows)
- [Program an Amulet Device](#program-an-amulet-device)
- [BLE Development](#ble-development)

Obtain an Amulet
===
You can build your own using the hardware files provided or request pre-built Amulet's from us by contacting rapjr@cs.dartmouth.edu. If you are part of a research lab we will generally accept your request.

Setup an Amulet Build Environment on Mac OSX
===
**NOTE: To be able to do this you may need to disable the System Integrity Protection (SIP). This can be done by booting into recovery mode (⌘ + R on boot) and accessing the Terminal (Utilities > Terminal). To disable type the command `csrutil disable` followed by reboot to restart your machine.**

1. [Download and install GCC compiler and tools using the installer for Mac OSX.](http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/index_FDS.html)

2. Download the amulet repo with the following command, which will make sure that you also get all of the associated submodules.

		git clone --recursive https://github.com/AmuletGroup/amulet-project.git

3. Run `buildscript-mac` to setup your development environment.


Setup an Amulet Build Environment on Linux
===
1. [Download the GCC compiler and tools for Linux.](http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/index_FDS.html)

2. Download the amulet repo with the following command, which will make sure that you also get all of the associated submodules.

		git clone --recursive https://github.com/AmuletGroup/amulet-project.git

3. Run `buildscript-linux` to setup your development environment.

Setup an Amulet Build Environment on Windows
===
1. Run a Linux VM and follow the above setup steps.


Program an Amulet Device
====
You can view our guide on developing applications and programming an Amulet device [here.](applications/README.md)

BLE Development
====
You can view our guide on BLE development [here.](firmware/ble/README.md)
