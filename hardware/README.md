Hardware
===
The Amulet wearable prototype is mostly a single-board system. 
The main board, battery, haptic buzzer, and secondary storage board are all housed in a custom-designed 3D-printed case that fits a standard 20mm off-the-shelf watchband.

#### Features
- **Processing**: 
  - Texas Instruments MSP430FR5989, 2 KB SRAM and 128 KB of FRAM
- **Sensing**: 
  - Analog Devices ADMP510 microphone
  - Everlight ALS-PT19 light sensor
  - Texas Instruments TMP20 temperature sensor
  - STMicroelectronics LSM6DSL gyroscope
  - Analog Devices ADXL362 accelerometer
  - Lapis Semi ML8511 UVA/B sensor
- **Communication**: 
  - Nordic nRF51822 used as a modem for communicating with peripheral BLE devices (such as a heart-rate monitor)
- **User Interface**: 
  - Sharp LS013B7DH03 display with 128x128 resolution
  - Two buttons
  - Three capacitive touch sensors with low power MPR121 touch controller
  - Haptic buzzer, and two LEDs.
- **Storage**:  
 - The small secondary storage board holds a microSD card reader. 
- **Misc**:
  - The board also includes a battery charger (MCP73831)
  - 110mAh battery


### Getting an Amulet
You can build your own using the hardware files provided or request pre-built Amulet's from us by contacting rapjr@cs.dartmouth.edu. If you are part of a research lab we will generally accept your request.

### Assembling an Amulet
Refer to [this](../media/Amulet_DC_kitec_assembly.pdf) guide for assembling the Amulet companion board, and [this](../media/Amulet_kitec_case_assembly.pdf) guide for placing an amulet in a case.

### Programming an Amulet
[Refer to this guide for how to set up your dev environment and program an amulet.](../GETTING_STARTED.md)
