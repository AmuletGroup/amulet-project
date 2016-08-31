/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 * @file: core_mode.c
 *
 * Contains definitions and signatures for System Mode management.
 *
 * @author Travis Peters (traviswp@cs.dartmouth.edu)
 * @author amulet-dev (amulet-dev@cs.dartmouth.edu)
 *  
 * Created: March 2015.
 */

#include "core_mode.h"

/**
 * Default system mode: NORMAL_MODE
 */
SystemMode currSystemMode = NORMAL_MODE;

void CoreSetSystemMode(SystemMode mode) {
	currSystemMode = mode;
}

SystemMode CoreGetSystemMode() {
	return currSystemMode;
}
