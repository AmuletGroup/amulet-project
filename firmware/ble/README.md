Amulet BLE Development
==========================
This document outlines how to get started with the BLE toolchain for Amulet devices. It is terribly important that you [read this first.](https://github.com/lab11/nrf5x-base)

# Table of Contents
1. [Prerequisites](#pre)
2. [Getting Started](#start)
2. [BLE Applications](#apps)
3. [MSP430 Applications](#apps2)
3. [Development Tools](#tools)

## <a name="pre"></a>Prerequisites
Before you can do BLE development, you need an Amulet board, and the MSP430 GCC toolchain setup.

You will need to have the battery plugged into the Amulet, and have firmware on the application board (MSP430) that toggles the power on OR use the JLink debugger that supplies power.

**Download and Install:** The [SEGGER JLink Software](https://www.segger.com/jlink-software.html) for interfacing with the debugger and flashing firmware onto the nRF51822_QFAC

## <a name="start"></a>Getting Started
These instructions are for OSX and Linux. They were tested on OSX 10.10.5 because I'm terrified of upgrading.

1 - **Download:** As a first step, [install ARM GCC toolchain using this link](https://launchpad.net/gcc-arm-embedded), or any other method, really.

2 - **Install:** Once downloaded, move it to whereever you keep things like compilers, and add the `gcc-arm-none-eabi-X_X-201XXX/bin` directory to your PATH (presumably your `~./zshrc` or `~/.bashrc` file.

3 - **Compile:** Now test out the basic beacon application. Navigate to `project-amulet/src/amulet-os/ble/apps/beacon` and type `make debug`

4 - **Flash:** Once this builds with no problems. Plug in the SEGGER debugger, attach debugger to Amulet device, attach battery to Amulet device, and type `make flash`. You should see the script install the softdevice, and then the firmware.

5 - **Verify:** Now check to make sure Amulet is sending out advertising packets using any method. The simplest is to download [nRF Master Control Panel](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en) and scan for the `amulet_beacon` device.

If this works, your toolchain, and your amulet device are all working.

Now read through the makefile and the [nrf51 base repository](https://github.com/lab11/nrf5x-base) to familiarize yourself with what is possible, specifically the debugging commands, and the linking scripts.

## <a name="apps"></a>BLE Applications
**beacon:** Simple starter application that advertises the name `amulet_beacon` and mostly just sleeps. Good for measuring power consumption and testing that new hardware works, or that toolchain setup succeeded. ***Use this as a starting point for other projects.***

**hrm_peripheral:** This is adapted from the heart rate monitor examples from Nordic. After flashing this, use any HRM App [(like this one for Android)](https://play.google.com/store/apps/details?id=com.ndevgroup.hrmonitor&hl=en) to see fake heart rate data. THis is a good starter app for designing peripherals.

**spi_slave:** This attempts to setup a SPI slave on the nrf51822, and send bytes back on request of the master (MSP430). There is a bug: currently it will only send one buffer of data, and once it is exhausted will just send an overread character. Even though the buffers are reset by nrf_drv_spis_buffers_set.

**uart:** This just waits for incoming packets on UART then sends out a canned byte message back. Simple test of the uart driver.

**central:** This is the (in-progress) amulet firmware. This currently allows for up to three HRM beacons to connect, and gathers all their data. This forwards the bytes over UART to the Amulet application processor.

## <a name="apps2"></a>MSP430 Applications

**uart:** This can be loaded on the MSP430 of the White Owl to test the BLE chip and connections using UART. It prints the bytes recieved from the BLE chip to the screen. It also has functions to get the SPI slave to work if necessary.

## <a name="tools"></a>Development Tools


When developing a BLE application, several tools exist to make your life easier.
The easiest option, if you have access to an android phone, is [nRF Master Control Panel](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en).
On iOS, [LightBlue Explorer](https://itunes.apple.com/us/app/lightblue-explorer-bluetooth/id557428110?mt=8)
has similar or better functionality. Alternatively,
[noble](https://github.com/sandeepmistry/noble) is a NodeJS library for interacting with BLE that can run from
a Linux or Mac computer.
