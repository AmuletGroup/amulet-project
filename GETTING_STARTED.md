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

1. Download and install the [MSP-GCC compiler and tools](http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/index_FDS.html) using the installer for Mac OSX.

2. Download the amulet repo with the following command, which will make sure that you also get all of the associated submodules.

		git clone --recursive https://github.com/AmuletGroup/amulet-project.git

3. Run `buildscript-mac` to setup your development environment.


Setup an Amulet Build Environment on Linux
===
1. Download the [MSP-GCC compiler and tools](http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/index_FDS.html) for Linux.

2. Download the amulet repo with the following command, which will make sure that you also get all of the associated submodules.

		git clone --recursive https://github.com/AmuletGroup/amulet-project.git

3. Run `buildscript-linux` to setup your development environment.

Setup an Amulet Build Environment on Windows
===
There is no native support for developing Amulet applications on Windows, but we do provide a Linux VM image that you can use to develop for Amulet.

1. Download and install [VirtualBox](https://www.virtualbox.org).

2. Download our [Amulet VM image](https://drive.google.com/file/d/1PUncyOrKgLr5kVQ1M6YulubZqJR3-wOM/view?usp=sharing).

3. Run VirtualBox and select `File > Import Appliance`. Select `AmuletLinuxVM.ova` and follow the default prompts.

4. The image should now appear on the left-hand side of the main VirtualBox window, and can be run by selecting it and clicking the green start button. The default **user** is `amuletuser` and the **password** is `amulet`.


Program an Amulet Device
====
You can view our guide on developing applications and programming an Amulet device [here.](applications/README.md)

BLE Development
====
You can view our guide on BLE development [here.](firmware/ble/README.md)
