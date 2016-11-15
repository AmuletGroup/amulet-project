Firmware
====
This folder contains all the firmware for running applications on Amulet wearables.
```
firmware/
		aft/          the Amulet Firmware Toolchain
		amulet-os/    the developer facing Amulet API
		ble/          the Bluetooth Low Energy code for the nRF51822 modem
		bsl/          the MSP430 implementation of AmuletOS
```

## Coding Style

You should write comment with [doxygen](http://www.stack.nl/~dimitri/doxygen/) style. 
Generally, all Amulet API should start with Amulet prefix.
All middle layer API between Amulet API and BSP(board support) should start with the Core prefix.
Include headers in c file instead of h file.
It is recommended to use clang format for all the C code (Both Sublime and Atom have clang formatter plugin) 

**NOTE:** *We are using the Doxygen formatting style for generating documentation from comments in our source code. Please refer to the [Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html) documentation for details on how to format comments.*