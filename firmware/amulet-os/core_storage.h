/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 * @file core_storage.h
 *
 * For SD card read and write, the functions in amulet.h are just wrapper
 * to functions here
 *
 *
 */

#ifndef CORE_STORAGE_H
#define CORE_STORAGE_H

#include "amulet.h"
#include "sd_driver.h"
/**
 * Amulet Log Defintions.
 */
#define LINE_SIZE     128  /**< Each line in the log can be at most 128 characters. */

/**
 * Check if the SD card exists.
 *
 * NOTE: We determine whether SD card exists by mounting it.
 *
 * @return 0 yes (true), 1 no (false)
 */
uint8_t SDExist();

/**
 * A log will be created for the requestor's corresponding app
 * TODO Implematation
 */
// void LogCreate(uint8_t logKey, uint8_t requestor);

/**
 * A log will be removed for the requestor's corresponding app
 * TODO Implematation
 */
// void LogRm(uint8_t logKey, uint8_t requestor);

/**
 * Append a line (i.e., `line_contents`) to `requestor`'s log (i.e., `log_name`).
 *
 * @return 0 (success), 1 (failure)
 */
uint8_t LogAppend(uint8_t log_name, char* line_contents, uint8_t line_size, uint8_t requestor);

/**
 * Read `n_lines` from `requestor`'s log starting at `start_line` in the log.
 *
 * @return 0 (success), 1 (failure)
 */
uint8_t LogRead(uint8_t log_name, char* line_contents, uint8_t line_size, uint8_t start_line, int n_lines, uint8_t requestor);

/*****************************************************************************
 * Add the floating array inside of the AmuletOS for SIFT APP
 * Author: Hang,Daniel
 *****************************************************************************/
uint16_t LogRead2(uint8_t log_name, char* line_contents, uint16_t line_size, uint16_t start_line, int n_lines, uint8_t requestor);

#endif //CORE_STORAGE_H
