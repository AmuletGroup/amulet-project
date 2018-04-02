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

#include "ADXL362.h"
#include "SPI.h"
#include "util.h"
#include "bsp_init.h" // To configure for bracelet or dev kit

// Uncomment to turn on debug prints
#define ADXL362_DEBUG

// ADXL362 register definitions
#define FIFO_CONTROL_REGISTER 0x28
#define FIFO_SAMPLES_REGISTER 0x29
#define FIFO_ENTRIES_REGISTER 0x0c
#define INTMAP1_REGISTER 0x2a
#define FILTER_CONTROL_REGISTER 0x2C

uint8_t FIFO_length = 0;  // In number of (X,Y,Z,temp) samples.

void ADXL362ChipSelect(uint8_t level) {
	if(level == LOW) {
		ACCEL_CS_PxOUT &= ~ACCEL_CS_BITx;
	} else {
		ACCEL_CS_PxOUT |= ACCEL_CS_BITx;
	}
}


//
//  begin()
//  Initial SPI setup, soft reset of device
//  SPI port SHOULD ALREADY BE SETUP by the display at this point
//
void ADXL362begin() {
        // Set CS pin to be an output
	ACCEL_CS_PxDIR |= ACCEL_CS_BITx;

	//SPIbegin();
	//SPIsetDataMode(SPI_MODE0);	//CPHA = CPOL = 0    MODE = 0
	delay(100);
    
	// soft reset
	ADXL362SPIwriteOneRegister(0x1F, 0x52);  // Write to SOFT RESET, "R"
	delay(10);

}

void ADXL362end() {
	// soft reset
	ADXL362SPIwriteOneRegister(0x1F, 0x52);  // Write to SOFT RESET, "R"
	delay(10);

}
 
//
//  beginMeasure()
//  turn on Measurement mode - required after reset
// 
void ADXL362beginMeasure() {
	uint8_t temp = ADXL362SPIreadOneRegister(0x2D);	// read Reg 2D before modifying for measure mode

	// turn on measurement mode
	uint8_t tempwrite = temp | 0x02;			// turn on measurement bit in Reg 2D
	ADXL362SPIwriteOneRegister(0x2D, tempwrite); // Write to POWER_CTL_REG, Measurement Mode
	delay(10);	

}

//
//  readXData(), readYData(), readZData(), readTemp()
//  Read X, Y, Z, and Temp registers
//
int16_t ADXL362readXData(){
	int16_t XDATA = ADXL362SPIreadTwoRegisters(0x0E);
	
	return XDATA;
}

int16_t ADXL362readYData(){
	int16_t YDATA = ADXL362SPIreadTwoRegisters(0x10);
	
	return YDATA;
}

int16_t ADXL362readZData(){
	int16_t ZDATA = ADXL362SPIreadTwoRegisters(0x12);

	return ZDATA;
}

int16_t ADXL362readTemp(){
	int16_t TEMP = ADXL362SPIreadTwoRegisters(0x14);

	return TEMP;
}

void ADXL362readXYZTData(int16_t *XData, int16_t *YData, int16_t *ZData, int16_t *Temperature){
	  // burst SPI read
	  // A burst read of all three axis is required to guarantee all measurements correspond to same sample time
	  ADXL362ChipSelect(LOW);
	  SPItransfer(0x0B);  // read instruction
	  SPItransfer(0x0E);  // Start at XData Reg
	  *XData = SPItransfer(0x00);
	  *XData = *XData + (SPItransfer(0x00) << 8);
	  *YData = SPItransfer(0x00);
	  *YData = *YData + (SPItransfer(0x00) << 8);
	  *ZData = SPItransfer(0x00);
	  *ZData = *ZData + (SPItransfer(0x00) << 8);
	  *Temperature = SPItransfer(0x00);
	  *Temperature = *Temperature + (SPItransfer(0x00) << 8);
	  ADXL362ChipSelect(HIGH);
  
}

void ADXL362setupFIFOandInterrupts(uint8_t FIFOsize){
        // Read the current control register value
        uint8_t ctrlreg = ADXL362SPIreadOneRegister(FIFO_CONTROL_REGISTER);

        // Store the FIFOsize for later use in error checking
        FIFO_length = FIFOsize;
        if((4*FIFOsize) > 512)return; // Can only store 4*128 = 512 (X,Y,Z,temp) samples.

        // Put the FIFO in Stream Mode and store temperature in FIFO also
        // 0b100 = store temperature in FIFO
        // 0b010 = enable Stream Mode
        // May need to set FIFO size top bit in FIFO_CONTROL_REGISTER
        if((4*FIFOsize) > 255)
        {
           FIFOsize = FIFOsize-256;
	   ADXL362SPIwriteOneRegister(FIFO_CONTROL_REGISTER, ctrlreg | 0b1110);
        } else
        {
	   ADXL362SPIwriteOneRegister(FIFO_CONTROL_REGISTER, ctrlreg | 0b0110);
        }

        // Configure the number of samples to read into the FIFO before the 
        // interrupt is triggered.  Note, if you want more than 255 samples
        // you need to set the appropriate bit in the FIFO_CONTROL register.
        // FIFO max size is 511.  We're storing (X,Y,Z,temp) so we need to
        // multiply the FIFO size specified in samples by 4.
	ADXL362SPIwriteOneRegister(FIFO_SAMPLES_REGISTER, 4*FIFOsize);

        // Verify registers were properly written
        ctrlreg = ADXL362SPIreadOneRegister(FIFO_CONTROL_REGISTER);
        ADXL362SPIreadOneRegister(FIFO_SAMPLES_REGISTER);

        // Set up the sample rate and g range in the filter control register.
        // Top most 2 bits 7-6: 00=2g, 01=4g, 1X=8g
        // bit 5 = reserved
        // bit 4 = 0 = Disable "halved bandwidth" or else the data will be smoothed.
        // bit 3 = 0 = don't require external interrupt to trigger sample
        // bit 2-0 = 
        //            000 12.5 Hz
        //            001 25 Hz
        //            010 50 Hz
        //            011 100 Hz (reset default)
        //            100 200 Hz
        //            101…111 400 Hz
        //    I set it to 25Hz for now.  We could make these available via API also.
        ADXL362SPIwriteOneRegister(FILTER_CONTROL_REGISTER, 0b00000001);
        ADXL362SPIreadOneRegister(FILTER_CONTROL_REGISTER);

        // Set up the INT1 interrupt to trigger when the FIFO reaches FIFOsize
        // This is known as the "FIFO watermark interrupt"
        // Setting bit 7 (MSB) to zero means the generated interrupt will
        // set the INT1 pin high.  All other interrupts are currently
        // disabled.
        ADXL362SPIwriteOneRegister(INTMAP1_REGISTER, 0b00000100);
        ADXL362SPIreadOneRegister(INTMAP1_REGISTER);

} // End of ADXL362setupFIFOandInterrupts()...

// Call this function to read the FIFO when an INT1 interrupt occurs because
// the FIFO is full.  The caller needs to supply FIFOarray[] which has a size
// of (4 * numberOfSamples) int16's or else this function will overwrite 
// unallocated memory space.  
// numberOfSamples = number of (X,Y,Z,temp) samples.
void ADXL362readFIFO(int16_t FIFOarray[], uint8_t numberOfSamples){
   uint8_t regValueL = 0;
   uint8_t regValueH = 0;
   uint8_t it;

   // Simple error checks.
   if(FIFOarray == 0x0000)return;  // An array must be passed.
   if(numberOfSamples > FIFO_length)return;  // Can't read more than FIFO size.

   // Read all the data.  We want the full 12 bit resolution so we need to
   // read two bytes for each value, including temperature.
   // This read also automatically clears the FIFO watermark interrupt.
   ADXL362ChipSelect(LOW); // Start transmission
   SPItransfer(0x0D);  // Send the FIFO read command
   // Each sample consists of (X,Y,Z,temperature) so need to read (4 x numberOfSamples) int16 values.
   for(it=0; it<(numberOfSamples*4); it++)
   {
        // Chip immediately responds with data
        // First low byte, then high byte.  The high byte has additional
        // information that we're ignoring for now.  That info could be
        // used for error checking if we find transfer is not reliable.
        // The values are 12 bit and follow the order (X,Y,Z,temperature)
        // As we read data two bytes at a time, space is freed on the stack.
        // Must be read two bytes at a time or FIFO space will be reused before
        // the second byte can be read.
	regValueL = SPItransfer(0x00); // Send a dummy value to read the next byte.
	regValueH = SPItransfer(0x00); // Send a dummy value to read the next byte.
        // Mask off upper nibble of high byte and combine to get a 12 bit value
        // Also move sign bit to most significant bit.
        FIFOarray[it] = regValueL + ((regValueH & 0b00000111) << 8) +
           ((regValueH & 0b00001000) << 12);
   }
   ADXL362ChipSelect(HIGH); // End transmission
} // End of ADXL362readFIFO()...

void ADXL362setupDCActivityInterrupt(int16_t threshold, uint8_t time){
	//  Setup motion and time thresholds
	ADXL362SPIwriteTwoRegisters(0x20, threshold);
	ADXL362SPIwriteOneRegister(0x22, time);

	// turn on activity interrupt
	uint8_t ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);  // Read current reg value
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x01);     // turn on bit 1, ACT_EN  
	ADXL362SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);       // Write new reg value 
	ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);       // Verify properly written

}

void ADXL362setupACActivityInterrupt(int16_t threshold, uint8_t time){
	//  Setup motion and time thresholds
	ADXL362SPIwriteTwoRegisters(0x20, threshold);
	ADXL362SPIwriteOneRegister(0x22, time);
  
	// turn on activity interrupt
	uint8_t ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);  // Read current reg value
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x03);     // turn on bit 2 and 1, ACT_AC_DCB, ACT_EN  
	ADXL362SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);       // Write new reg value 
	ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);       // Verify properly written

}

void ADXL362setupDCInactivityInterrupt(int16_t threshold, int16_t time){
	// Setup motion and time thresholds
	ADXL362SPIwriteTwoRegisters(0x23, threshold);
	ADXL362SPIwriteTwoRegisters(0x25, time);

	// turn on inactivity interrupt
	uint8_t ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);   // Read current reg value 
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x04);      // turn on bit 3, INACT_EN  
	ADXL362SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);        // Write new reg value 
	ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);        // Verify properly written

}

void ADXL362setupACInactivityInterrupt(int16_t threshold, int16_t time){
	//  Setup motion and time thresholds
	ADXL362SPIwriteTwoRegisters(0x23, threshold);
	ADXL362SPIwriteTwoRegisters(0x25, time);
 
	// turn on inactivity interrupt
	uint8_t ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);   // Read current reg value
	ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x0C);      // turn on bit 3 and 4, INACT_AC_DCB, INACT_EN  
	ADXL362SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);        // Write new reg value 
	ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);        // Verify properly written

}

void ADXL362checkAllControlRegs(){
	//uint8_t filterCntlReg = ADXL362SPIreadOneRegister(0x2C);
	//uint8_t ODR = filterCntlReg & 0x07;  bsp_printf("ODR = ");  bsp_printf("%x", ODR);
	//bsp_printf("\n");
	//uint8_t ACT_INACT_CTL_Reg = ADXL362SPIreadOneRegister(0x27);      bsp_printf("ACT_INACT_CTL_Reg = "); bsp_printf("%x", ACT_INACT_CTL_Reg);
	//bsp_printf("\n");
	ADXL362ChipSelect(LOW);
	SPItransfer(0x0B);  // read instruction
	SPItransfer(0x20);  // Start burst read at Reg 20
	ADXL362ChipSelect(HIGH);
}

// Basic SPI routines to simplify code
// read and write one register
uint8_t ADXL362SPIreadOneRegister(uint8_t regAddress){
	uint8_t regValue = 0;
  
	ADXL362ChipSelect(LOW);
	SPItransfer(0x0B);  // read instruction
	SPItransfer(regAddress);
	regValue = SPItransfer(0x00);
	ADXL362ChipSelect(HIGH);

	return regValue;
}

void ADXL362SPIwriteOneRegister(uint8_t regAddress, uint8_t regValue){
  
	ADXL362ChipSelect(LOW);
	SPItransfer(0x0A);  // write instruction
	SPItransfer(regAddress);
	SPItransfer(regValue);
	ADXL362ChipSelect(HIGH);
}

int16_t ADXL362SPIreadTwoRegisters(uint8_t regAddress){
	int16_t twoRegValue = 0;
  
	ADXL362ChipSelect(LOW);
	SPItransfer(0x0B);  // read instruction
	SPItransfer(regAddress);  
	twoRegValue = SPItransfer(0x00);
	twoRegValue = twoRegValue + (SPItransfer(0x00) << 8);
	ADXL362ChipSelect(HIGH);

	return twoRegValue;
}  

void ADXL362SPIwriteTwoRegisters(uint8_t regAddress, int16_t twoRegValue){
	uint8_t twoRegValueH = twoRegValue >> 8;
	uint8_t twoRegValueL = twoRegValue;
  
	ADXL362ChipSelect(LOW);
	SPItransfer(0x0A);  // write instruction
	SPItransfer(regAddress);  
	SPItransfer(twoRegValueL);
	SPItransfer(twoRegValueH);
	ADXL362ChipSelect(HIGH);
}
