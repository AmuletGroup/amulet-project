/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */



#include "core_storage.h"

#include "FatFS/ff.h"
#include "FatFS/diskio.h"

//#define BSP_DEBUG_SD

// TODO As the SD card will share SPI with some other devices (LCD ..), and
// other devices will have different clock frequecy, we need to double check
// the SD card driver's frequency is compatible. It is used to compatible
// without the use of the fastmode.

// As we currently use app's ID as its filename, the function is used to convert
// an integer app id to char array filename.
extern void itoa(uint16_t value, char* result, int base);

// [Different ways to put the SDvolume into the FRAM - JDH]
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR

#pragma NOINIT(sdVolume)
FATFS sdVolume ; // FatFs work area needed for each volume
#pragma NOINIT(logfile)
FIL logfile;	// File object needed for each open file

#elif defined(__GNUC__)

FATFS sdVolume ; __attribute__ ((section (".noinit")))   // FatFs work area needed for each volume
FIL logfile;	 __attribute__ ((section (".noinit")))   // File object needed for each open file

#else
#error Compiler not supported!
#endif

// We initialize the status with an invalid status. The status values are defined 
// in the SD card driver.
// TODO Use a more meaningful name instead of a number.
uint8_t status = 17;

uint8_t SDExist(){
	sd_enable();
	switch(f_mount(&sdVolume, "", 0) ){
		case FR_OK:
			status = 42;
			break;
		case FR_INVALID_DRIVE:
			status = 1;
			break;
		case FR_DISK_ERR:
			status = 2;
			break;
		case FR_NOT_READY:
			status = 3;
			break;
		case FR_NO_FILESYSTEM:
			status = 4;
			break;
		default:
			status = 5;
			break;
	}
	sd_disable();

	if(status != 42)
		return 1;
	return 0;
}

uint8_t LogAppend(uint8_t log_name, char* line_contents, uint8_t line_size, uint8_t requestor) {
	uint8_t return_val = 1;

	if (SDExist() != 0)
		return 1;

	sd_enable();

	// Convert the APP's log name (an integer) to a filename (a char array).	
	char logKey[10];
	itoa(log_name, logKey ,10);
	

	// Allocate a buffer and copy the contents into it. The size limit is LINE_SIZE; content beyond the size limit will be discarded.
	char fixed_length_buffer[LINE_SIZE];
	memset(fixed_length_buffer, ' ', LINE_SIZE);
	uint8_t copy_size = line_size > LINE_SIZE ? LINE_SIZE : line_size;
	memcpy(fixed_length_buffer, line_contents, copy_size);

	//line_size should be the real size
	line_size = strlen(line_contents);
	

	UINT bw;
	// Open & write
	if(f_open(&logfile, logKey, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {     // Open file - If nonexistent, this will create the log
		f_lseek(&logfile, logfile.fsize);                                  // Move forward by start_line*line_size; logfile.fsize+1 is not needed in this application
		f_write(&logfile, fixed_length_buffer, line_size, &bw);            // Append word
		f_close(&logfile);                                                 // Close the file

		return_val = 0;
	}
	sd_disable();
	return return_val;
}

uint8_t LogRead(uint8_t log_name, char* line_contents, uint8_t line_size, uint8_t start_line, int n_lines, uint8_t requestor) {
	uint8_t return_val = 1;

	if (SDExist() != 0)
		return 1;

	sd_enable();

	// Convert the APP's log name (an integer) to a filename (a char array).
 	char logKey[10];
 	itoa(log_name, logKey,10);

 	UINT bw;
	if(f_open(&logfile, logKey, FA_READ) == FR_OK) {           // Open file - If nonexistent, this will create the log
		f_lseek(&logfile, start_line*line_size);               // Move forward by start_line*line_size; logfile.fsize+1 is not needed in this application
		f_read (&logfile, line_contents, line_size, &bw);      // Read data from a file
		f_close(&logfile);                                     // Close the file

		return_val = 0;
	}
	sd_disable();
	return return_val;
}


uint16_t LogRead2(uint8_t log_name, char* line_contents, uint16_t line_size, uint16_t start_line, int n_lines, uint8_t requestor) {
	uint8_t return_val = 1;

	if (SDExist() != 0)
		return 1;

	sd_enable();

	// Convert the APP's log name (an integer) to a filename (a char array).
 	char logKey[10];
 	itoa(log_name, logKey,10);

 	UINT bw;
	if(f_open(&logfile, logKey, FA_READ) == FR_OK) {           // Open file - If nonexistent, this will create the log
		f_lseek(&logfile, start_line*line_size);               // Move forward by start_line*line_size; logfile.fsize+1 is not needed in this application
		f_read (&logfile, line_contents, line_size, &bw);      // Read data from a file
		f_close(&logfile);                                     // Close the file

		return_val = 0;
	}
	sd_disable();
	return return_val;
}
