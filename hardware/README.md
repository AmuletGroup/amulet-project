Hardware
===
The Amulet wearable prototype is mostly a single-board system. 
The main board, battery, haptic buzzer, and secondary storage board are all housed in a custom-designed 3D-printed case that fits a standard 22mm off-the-shelf watchband.

<img align="left" src="../media/amulet_left_vertical.jpg" height="350px" alt="Amulet Wearable"/>


#### Features
- **Processing**: 
  - Texas Instruments MSP430FR5989, 2 KB SRAM and 128 KB of FRAM
- **Sensing**: 
  - Analog Devices ADMP510 microphone
  - Everlight ALS-PT19 light sensor
  - Texas Instruments TMP20 temperature sensor
  - STMicroelectronics L3GD20H gyroscope
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
Currently you can only request pre-built Amulet's from us by contacting jhester@clemson.edu, if you are part of a research lab we will generally accept your request.
Since Amulet is open source and open hardware, you can build your own!

[Order boards from Circuithub](https://circuithub.com/projects/jhester/watch)

### Assembling an Amulet
[Refer to this PDF guide on building your own amulet.](SKAssemblyGuide.pdf)

### Programming an Amulet
[Refer to this guide for how to set up your dev environment and program an amulet.](../GETTING_STARTED.md)
