/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University,
 * and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research
 * Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only),
 * as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#include "core_storage.h"
#include "aes256.h"
#include "core_clock.h"

#include <stdlib.h>
#include <string.h>

#include "FatFS/diskio.h"
#include "FatFS/ff.h"

//#define BSP_DEBUG_SD

// TODO As the SD card will share SPI with some other devices (LCD ..), and
// other devices will have different clock frequecy, we need to double check
// the SD card driver's frequency is compatible. It is used to compatible
// without the use of the fastmode.

// As we currently use app's ID as its filename, the function is used to convert
// an integer app id to char array filename.
extern void itoa(uint16_t value, char *result, int base);

// [Different ways to put the SDvolume into the FRAM - JDH]
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR

#pragma NOINIT(sdVolume)
FATFS sdVolume; // FatFs work area needed for each volume
#pragma NOINIT(logfile)
FIL logfile; // File object needed for each open file

#elif defined(__GNUC__)

FATFS sdVolume;
__attribute__((section(".noinit"))) // FatFs work area needed for each volume
FIL logfile;
__attribute__((section(".noinit"))) // File object needed for each open file

char sd_commit_buffer[1024]; // large(ish) buffer for writing to SD card
__attribute__((section(".noinit")))

#else
#error Compiler not supported!
#endif

// We initialize the status with an invalid status. The status values are
// defined
// in the SD card driver.
// TODO Use a more meaningful name instead of a number.
uint8_t status = 17;

char log_filename[13] = "0.ADL\0";
char header_filename[13] = "0.ADH\0";
bool log_initialized = false;
uint8_t test_key[16] = "dkd874hcEI78Bfe";

// Commit bytes for buffering SD writes
// Last written index in the commit buffer
uint16_t commit_length_sd = 0;
uint8_t transaction_in_progress = 0;


/*
 * Initializes the log file names,
 * sets the cipher key for AES encryption,
 * and sets the value_headers_written flag to false for each app
 */
void LogInit() {
  CoreTimeStampFile(log_filename);
  memcpy(header_filename, log_filename, 9);
  strcat(log_filename, ".ADL");
  strcat(header_filename, ".ADH"); // this was cpy
  log_initialized = true;
  AES256_setCipherKey(0x09C0, test_key, AES256_KEYLENGTH_128BIT);

  // no value_headers have been written to a file yet
  for (App *curr = GlobalAppQueueHead; curr != NULL; curr = curr->next)
    curr->value_headers_written = false;
}

/*
 * First, try to mount the FS using SDExist. If this works, try to open and
 * write to a file. If either of these fail, return false.
 */
bool SDAvailable() {
    bool return_val = false;
    if(SDExist() == 0) {
        sd_enable();
        UINT bw;
        // Open & write - If nonexistent, this will create the file
        if (f_open(&logfile, "check", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
            f_close(&logfile); // Close the file
            return_val = true;
        }
        sd_disable();
    }
    return return_val;
}


/*
 * Check PJ.2 for presence of SD card (0 if present, 1 if not). If not there,
 * return 1 for failure. If present, try mounting the file system, and if
 * that works return 0. If any file system errors occur, return 1.
 */
uint8_t SDExist() {

    #ifdef BSP_SNAIL_KITE_D
        if(PJIN & BIT2) {
            return 1;
        }
    #endif

    sd_enable();
    switch (f_mount(&sdVolume, "", 0)) {
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
    if (status != 42) {
        return 1;
    } else {
        return 0;
    } 
}

/*
 * Writes char array contents to the SD card
 *
 * @param log_name = actual name of the log (i.e., $(log_name).ADH)
 * @param contents = data values
 * @param size = number of bytes to be written
 * @param encrypt = whether encrypt values or not
 */
uint8_t WriteToLogBuffer(char *log_name, char *contents, uint16_t size, bool encrypt) {
  uint8_t return_val = 1;

  if (SDAvailable()) {

    if (log_initialized == false)
      LogInit();

    sd_enable();
    UINT bw, bw_new;
    // Open & write
    // Open file - If nonexistent, this will create the log
    if (f_open(&logfile, log_name, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
      // Move forward by start_line*line_size;
      // logfile.fsize+1 is not needed in this application
      f_lseek(&logfile, logfile.fsize);
      f_write(&logfile, contents, size, &bw); // Append word
      f_close(&logfile); // Close the file
      sd_disable();

      return_val = 0;
    }
  } else { // SDAvailable == false
    // We assume that every time the card is taken out, the value headers will
    // be deleted or a new card will be used, because this is a safer assumption
    // to make than to assume that the same card will always be used without
    // ever having files deleted. To assume that the headers are deleted, we
    // need to go through each app, and set its value_headers_written flag
    // to false.
    for (App *curr = GlobalAppQueueHead; curr != NULL; curr = curr->next)
      curr->value_headers_written = false;
  }

  return return_val;
}



/*
 * Writes char array contents to the SD card
 *
 * @param log_name = actual name of the log (i.e., $(log_name).ADH)
 * @param contents = data values
 * @param size = number of bytes to be written
 * @param encrypt = whether encrypt values or not
 */
uint8_t WriteToLog(char *log_name, char *contents, uint16_t size, bool encrypt) {
  uint8_t return_val = 1;

  if (SDAvailable()) {
    // Allocate a buffer and copy the contents into it.
    uint16_t blocks = (size + 15) >> 4;
    uint8_t buffer[blocks * 16];
    uint8_t encrypted[16];
    memset(buffer, 0, blocks * 16);
    memcpy(buffer, contents, size);

    if (log_initialized == false)
      LogInit();

    sd_enable();
    encrypt = false;
    UINT bw, bw_new;
    // Open & write
    // Open file - If nonexistent, this will create the log
    if (f_open(&logfile, log_name, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
      // Move forward by start_line*line_size;
      // logfile.fsize+1 is not needed in this application
      f_lseek(&logfile, logfile.fsize);
      if (encrypt == true) {
        uint16_t i;
        for (i = 0; i < blocks; i++) {
          AES256_encryptData(0x09C0,  &buffer[i * 16], encrypted);
          f_write(&logfile, encrypted, 16, &bw);
        }
      } else {
        f_write(&logfile, buffer, size, &bw); // Append word
        // bw_new = bw;
        // f_write(&logfile, &bw_new, 4, &bw);
      }
      f_close(&logfile); // Close the file
      sd_disable();

      return_val = 0;
    }
  } else { // SDAvailable == false
    // We assume that every time the card is taken out, the value headers will
    // be deleted or a new card will be used, because this is a safer assumption
    // to make than to assume that the same card will always be used without
    // ever having files deleted. To assume that the headers are deleted, we
    // need to go through each app, and set its value_headers_written flag
    // to false.
    for (App *curr = GlobalAppQueueHead; curr != NULL; curr = curr->next)
      curr->value_headers_written = false;
  }

  return return_val;
}

/*
 * Log value_headers and data to SD card. Value headers are logged
 * to header_filename (e.g., 736B5150.ADH), and the data is logged
 * to log_filename (e.g., 736B5150.ADL). Header files end with the
 * ADH file extension, and data files, end with the ADL file
 * extension
 *
 * @param log_name = app's log ID
 * @param value_headers = header titles for each value
 * @param data = array of data to be logged
 * @param data_size = number of actual data values
 */
uint8_t LogData(uint8_t log_name, char *value_headers, float *data,
                uint8_t data_size) {
  // Size should be a multiple of 16 and big enough for the data
  uint8_t log_size = ((8 + (data_size * 4) + 15) >> 4) << 4;
  uint8_t datatype_size = sizeof data;
  char contents[log_size];

  contents[0] = log_name;      // Log ID
  contents[1] = data_size;     // # of data values
  CoreTimeStamp(&contents[2]); // Add binary timestamp

  memcpy(&contents[8], data, datatype_size * data_size);
  uint8_t bytes_left = log_size - (8 + datatype_size * data_size);
  memset(&contents[8 + datatype_size * data_size], 0, bytes_left);

  // only log the app's value_header info if it hasn't been logged yet
  for (App *curr = GlobalAppQueueHead; curr != NULL; curr = curr->next) {
    if (curr->appLogID == log_name)
      if (!curr->value_headers_written) {
        // we add 1 for the ',' delimeter and 1 for a null char
        uint8_t app_name_size = (uint8_t)strlen(curr->appName) + 2;
        char app_name[app_name_size];

        strcpy(app_name, curr->appName);
        app_name[app_name_size - 2] = ',';
        // we no longer need the space for null-terminating characters
        app_name_size -= 1;

        uint8_t value_headers_size = strlen(value_headers);
        // add 2 for the app's log id, and value_headers_size
        uint8_t total_size = app_name_size + value_headers_size + 2;

        uint8_t header_log_size = ((total_size + 15) >> 4) << 4;
        // header_contents format: app's name, app's log id,
        // value_headers_size, value_headers
        char header_contents[header_log_size];

        memcpy(&header_contents[0], app_name, app_name_size);

        // log_name is actually the app's log id
        header_contents[app_name_size] = log_name;

        header_contents[app_name_size + 1] = value_headers_size;

        memcpy(&header_contents[app_name_size + 2], value_headers,
               value_headers_size);
        bytes_left = header_log_size - total_size;
        memset(&header_contents[total_size], 0, bytes_left);

        if (WriteToLog(header_filename, header_contents, header_log_size,
                       true) == 0)
          curr->value_headers_written = true;
      }
  }

  // Backwards compatible to handle non-transactional (buffering) SD card writes
  if(transaction_in_progress) {
    memcpy(sd_commit_buffer+commit_length_sd, contents, log_size);
    commit_length_sd+=log_size;
    return 0;
  } else {
    uint8_t ret_val = WriteToLog(log_filename, contents, log_size, true);
    return ret_val;
  }
}

void CoreStartLogTransaction() {
  commit_length_sd = 0;
  transaction_in_progress = 1;
}

uint8_t CoreCommitLogTransaction() {
    uint8_t ret_val = WriteToLogBuffer(log_filename, sd_commit_buffer, commit_length_sd, true);
    transaction_in_progress = 0;
    commit_length_sd = 0;
    return ret_val;

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

/* ------------------ Deprecated -------------------*/
uint8_t LogRead(uint8_t log_name, char *line_contents, uint8_t line_size,
                uint8_t start_line, int n_lines, uint8_t requestor) {
  uint8_t return_val = 1;

  if (SDExist() != 0)
    return 1;

  sd_enable();

  // Convert the APP's log name (an integer) to a filename (a char array).
  char logKey[10];
  itoa(log_name, logKey, 10);

  UINT bw;
  if (f_open(&logfile, logKey, FA_READ) ==
      FR_OK) { // Open file - If nonexistent, this will create the log
    f_lseek(&logfile, start_line * line_size); // Move forward by
                                               // start_line*line_size;
                                               // logfile.fsize+1 is not needed
                                               // in this application
    f_read(&logfile, line_contents, line_size, &bw); // Read data from a file
    f_close(&logfile);                               // Close the file

    return_val = 0;
  }
  sd_disable();
  return return_val;
}

/* ------------------ Deprecated -------------------*/
uint16_t LogRead2(uint8_t log_name, char *line_contents, uint16_t line_size,
                  uint16_t start_line, int n_lines, uint8_t requestor) {
  uint8_t return_val = 1;

  if (SDExist() != 0)
    return 1;

  sd_enable();

  // Convert the APP's log name (an integer) to a filename (a char array).
  char logKey[10];
  itoa(log_name, logKey, 10);

  UINT bw;
  if (f_open(&logfile, logKey, FA_READ) ==
      FR_OK) { // Open file - If nonexistent, this will create the log
    f_lseek(&logfile, start_line * line_size); // Move forward by
                                               // start_line*line_size;
                                               // logfile.fsize+1 is not needed
                                               // in this application
    f_read(&logfile, line_contents, line_size, &bw); // Read data from a file
    f_close(&logfile);                               // Close the file

    return_val = 0;
  }
  sd_disable();
  return return_val;
}
