/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 Arduino Library for Analog Devices ADXL362 - Micropower 3-axis accelerometer
 go to http://www.analog.com/ADXL362 for datasheet
 
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created June 2012
 by Anne Mahaffey - hosted on http://annem.github.com/ADXL362
 
 Modified May 2013
 by Jonathan Ruiz de Garibay

 */ 

#ifndef ADXL362_h
#define ADXL362_h

#include <stdint.h>

	//
	// Basic Device control and readback functions
	//
	void ADXL362begin(); 		
	void ADXL362beginMeasure(); 
	int16_t ADXL362readXData();
	int16_t ADXL362readYData();
	int16_t ADXL362readZData();
	void ADXL362readXYZTData(int16_t *XData, int16_t *YData, int16_t *ZData, int16_t *Temperature);
	int16_t ADXL362readTemp();
	
	//
	// Activity/Inactivity interrupt functions
	//
	void ADXL362setupDCActivityInterrupt(int16_t threshold, uint8_t time);	
	void ADXL362setupDCInactivityInterrupt(int16_t threshold, int16_t time);
        void ADXL362setupACActivityInterrupt(int16_t threshold, uint8_t time);
	void ADXL362setupACInactivityInterrupt(int16_t threshold, int16_t time);
	
	// need to add the following functions
	// void mapINT1(
	// void mapINT2
	// void autoSleep
	// void activityInterruptControl
	//		-Activity, Inactivity, Both
	//		- Referenced, Absolute
	//		- Free Fall, Linked Mode, Loop Mode
	
	void ADXL362checkAllControlRegs();
	void ADXL362setupFIFOandInterrupts(uint8_t FIFOsize);
  void ADXL362readFIFO(int16_t FIFOarray[], uint8_t numberOfSamples);

	
	//  Low-level SPI control, to simplify overall coding
	uint8_t ADXL362SPIreadOneRegister(uint8_t regAddress);
	void ADXL362SPIwriteOneRegister(uint8_t regAddress, uint8_t regValue);
	int16_t  ADXL362SPIreadTwoRegisters(uint8_t regAddress);
	void ADXL362SPIwriteTwoRegisters(uint8_t regAddress, int16_t twoRegValue);

	void ADXL362pinMode(uint8_t pin, uint8_t mode);
	void ADXL362ChipSelect(uint8_t level);

#endif
