Preface
====
If you do not already have the Amulet hardware, or have not yet set it up, please refer to our hardware guide [here](hardware/README.md) for help.

#### Table of Contents

- [Setup an Amulet Build Environment on Mac OSX](#setup-an-amulet-build-environment-on-mac-osx)
- [Setup an Amulet Firmware Toolchain Config File](#setup-an-amulet-firmware-toolchain-config-file)
- [Program an Amulet Device](#program-an-amulet-device)
- [Troubleshooting](#troubleshooting)
- [BLE Development](#ble-development)

Setup an Amulet Build Environment on Mac OSX
===
1. Download the amulet repo with the following command, which will make sure that you also get all of the associated submodules. NOTE: Depending on your version of git, the --recursive option might not work. If that is the case, then you will need to navigate to both ``firmware/native`` and ``firmware/ble`` and run ``git submodule init`` and ``git submodule update`` to pull in the submodules.

		git clone --recursive https://gitlab.cs.dartmouth.edu/amulet/amulet-dev.git

2. Download and install Homebrew (if not already installed):

		ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

3. Now install the dependancies for mspdebug using brew:

		brew install libusb libusb-compat libelf

4. [Download and install GCC compiler and tools using the installer for Mac OSX.](http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/3_05_00_00/index_FDS.html)
	
		http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/3_05_00_00/index_FDS.html

5. Download (using git) the mspdebug source code for Mac OSX from buffet.cs.clemson.edu, go into the directory, and build the binary:

		git clone anonymous@buffet.cs.clemson.edu:jhester/mspdebug-osx
		cd mspdebug-osx
		make

6. Install the binary, you can put it anywhere in your path, this is a suggestion, sudo at your own risk:

		sudo cp mspdebug /usr/bin/

7. Copy tilib to the bin for mspdebug, and add the other msp430 tools to your path. It is recommended that you place the export command in your ``.bash_profile`` or ``.zshrc`` file. If you are having trouble with the export command when using El Capitan, try disabling the System Integrity Proection (SIP) mechanism. Or, at your own risk, you could copy all of ``ti/gcc/bin`` to your ``/usr/bin`` directory.

		sudo cp ~/ti/gcc/bin/libmsp430.dylib /usr/bin/
		export PATH="$PATH:~/ti/gcc/bin"

8. Test an installation by running `mspdebug tilib` which should show this output:

		MSPDebug version 0.23 - debugging tool for MSP430 MCUs
		Copyright (C) 2009-2015 Daniel Beer <dlbeer@gmail.com>
		Modified by Josiah Hester <jhester@clemson.edu>
		This is free software; see the source for copying conditions.  There is NO
		warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
		Chip info database from MSP430.dll v3.3.1.4 Copyright (C) 2013 TI, Inc.

9. Download and install the QM and QMC apps from sourceforge, these allow you to edit QM files, and compile them into C source code. Install them both in /Applications/ on your Mac. 
	
		http://sourceforge.net/projects/qpc/files/QM/3.3.0/
		qm_3.3.0-mac64.dmg
		qmc_3.3.0-mac64.dmg

10. Finally, navigate to ``~/path_to_amulet-dev/amulet-dev/firmware/native/lib-qpc/ports/msp430/vanilla/ccs-mspx``, and update lines 53, 54, and 65 (shown below) with YOUR username. Then, run ``make`` to build ``~/path_to_amulet-dev/amulet-dev/firmware/native/lib-qpc/ports/msp430/vanilla/ccs-mspx/dbg/libqp.a``, which will be used during the firmware compilation process.

		CC  = /Users/taylorhardin/ti/gcc/bin/msp430-elf-gcc
		LIB = /Users/taylorhardin/ti/gcc/bin/msp430-elf-ar
		CCINC = -I$(QP_PRTDIR) -I$(QP_INCDIR) -I/Users/taylorhardin/ti/gcc/include/

Setup an Amulet Firmware Toolchain Config File
====
The following code is an example of an Amulet Firmware Toolchain (AFT) .config file, and is based off of [this](firmware/aft/example_firmware.config) config file. Please remember to update the following file paths with your own.

	amulet-config:

	    #
	    # Environment Configuration.
	    #

	    #amulet_root is the path to root of the amulet repo
	    amulet_root: ~/Repos/amulet-project 								# mac build environment

	    #amulet_apps is the path to root of the applications directory
	    amulet_apps: ~/Repos/amulet-project/applications 					# mac build environment

	    #qpc_root is the path to root of the lib-qpc repo
	    qpc_root: ~/Repos/amulet-project/firmware/native/lib-qpc            # mac build environment

	    # qmc_path is the path of the qmc executable
	    qmc_path: /Applications/qmc.app/Contents/MacOS 					# mac build environment

	    #
	    # Build Configuration.
	    #

	    # apps provides a list of names of apps that you'd like to include in the firmware.
	    apps:
	        - name: clock
	        - name: heartrate

	    # device configuration is denoted using config. The value for config is directly used as a define C flag
	    config: BSP_SNAIL_KITE

	    # flags are set to build firmware process. You can specify flags here, or on the command line when invoking the aft script.
	    flags:
	        # if pins is true, the -t flag is set to add toggle pin code
	        pins: false

	        # if aft is true, the -p flag is set and apps go through AFT translation and runtime checks
	        aft: true

	        # if resource-profiling is true, the -r flag is set and resource usage/energecy consumption information is produced
	        resource_profiling: true
		
	    compiler: gcc
	    gcc_root: ~/ti/gcc/bin

Program an Amulet Device
====
1. Make sure that your .config file contains all of the apps that you want to run on the Amulet.

2. If you HAVE NOT compiled a firmware image before, then you will first need to compile the build system. To do this, navigate to ``amulet-dev/firmware/aft``, and run the following command. This will compile the build system, as well as compile your firmware image. If you don't want to see all of the output, then leave out the ``--verbose`` flag.

		./aft --all --verbose --rebuild-abt Path_To_Your_Config_File/[your_config_file].config

4. If you already compiled the build system, then you can use the following command inside of the ``amulet-dev/firmware/aft`` folder to compile your firmware image. If, for whatever reason, you want to re-compile the build system, then use the above command instead. Again, ``--verbose`` is optional.

		./aft --all --verbose Path_To_Your_Config_File/[your_config_file].config 

5. Connect your Amulet to the computer using the [TagConnect JTAG Dongle](http://www.tag-connect.com/what-is-tag-connect) connected to a programmer; this programmer can either be a MSP430FET, or a MSP430FRX9X9 launchpad.

6. Run the following command to install the firmware image onto your Amulet. Afterwards, the device should be fully programmed and ready to use. You should see a clock on the display with the current time.

		./aft --install-binary Path_To_Your_Config_File/[your_config_file].config

When you first run the above command you might get a message indicating that your firmware needs to be updated. If that is the case, run the `mspdebug tilib —allow-fw-update`. Ignore this note if you don't see that message.

You can see an in depth explanation of how the aft works and it's flag options [here.](firmware/aft/README.md)

## Coding Style

You should write comment with [doxygen](http://www.stack.nl/~dimitri/doxygen/) style. 
Generally, all Amulet API should start with Amulet prefix.
All middle layer API between Amulet API and BSP(board support) should start with the Core prefix.
Include headers in c file instead of h file.
It is recommended to use clang format for all the C code (Both Sublime and Atom have clang formatter plugin) 

**NOTE:** *We are using the Doxygen formatting style for generating documentation from comments in our source code. Please refer to the [Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html) documentation for details on how to format comments.*

Troubleshooting
====
Use the -d option in AFT to start debugging the firmware, it will break on main, then you use GDB as usual.
I also recommend you print debug message on the Sharp display, use the following to print and clear:
`BSP_display_clr();`
`BSP_display("IPC interrupt", 1);`
The second parameter is the line number.

UART debug has been removed from this document as UART no longer exist in our current hardware rev. If you need that, please find them in git history.

If you have trouble going into gdb with ./aft -d script, you can try to manually start debugging session with command in the `src/aft/tools.gdb.cmd`

Inevitably you will encounter issues while developing different aspects of the amulet system, be it with application development, amulet-os development, BSP development, AFT development, or something else. This page exists to capture information about some known issues and may help you resolve a problem you have. 

## Cannot merge apps into a single file

When using the AFT to build a firmware image for the amulet you may encounter a message similar to this:

```
SUCCESS:: App built successfully
...adding EmergencyResponse
Error...Step failed. Exiting!
```

It may be that the location of QM tool is not correct. The `do-aft` script assumes that you have QM installed and specifies a variable called `qpc_root` where you are supposed to specify the location of the `qmc` executable (you should have set this while you were following the "getting started" instructions). 

## One of the core state machine stop working
The queue(s) for the state machine can fill up quickly if you carelessly use them (ex. if you create an event but do not use it). The best practice is to create an event right before you are about to post it. 

```
AmuletSensorsEvt *timeReqEvt;
timeReqEvt = Q_NEW(AmuletSensorsEvt, AMULET_REQ_CHANGE_SIG);
QACTIVE_POST(getAppActiveObject(GlobalAppQueueHead, subscribedAppID), &timeReqEvt->super, NULL);
```

BLE Development
====
You can view our guide on BLE development [here.](firmware/ble/README.md)
