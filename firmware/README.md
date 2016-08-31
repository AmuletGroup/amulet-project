Firmware
====
This folder contains all the firmware for running applications on Amulet wearables.
```
firmware/
		aft/          the Amulet Firmware Toolchain
		amulet-os/    the developer facing Amulet API
		ble/          the Bluetooth Low Energy code for the nRF51822 modem
		native/       the MSP430 implementation of AmuletOS
```


### Amulet Firmware Toolchain
The Amulet Firmware Toolchain (AFT) manages the analysis, translation, and compilation of firmware. 
Here, we focus on two critical AFT roles: application isolation and resource profiling. 
First, the AFT ensures that apps can only access Amulet hardware by sending a well-formed request to the Amulet-OS core via the Amulet API, and prevents malicious or buggy apps from reading or modifying the memory of either the OS or another app. 
Second, with the AFT’s profiling tools an app developer can predict her app’s resource usage.

### AmuletOS
...TODO...

### Bluetooth
...TODO...

### MSP430 Native Code
...TODO...
