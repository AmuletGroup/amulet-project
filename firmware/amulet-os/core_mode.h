/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 * @file: core_mode.h
 *
 * Contains definitions and signatures for System Mode management.
 *
 * @author Travis Peters (traviswp@cs.dartmouth.edu)
 * @author amulet-dev (amulet-dev@cs.dartmouth.edu)
 *  
 * Created: March 2015.
 */

#ifndef CORE_MODE_H_
#define CORE_MODE_H_

/**
 * System modes
 */
typedef enum SystemMode {
	NORMAL_MODE,            // default
    EMERGENCY_MAYBE_MODE, 
	EMERGENCY_MODE, 
	DEBUG_MODE
} SystemMode;

/**
 * Set the current system mode.
 *
 * TODO: What does it mean for an app to be privileged and to be able to call this routine?
 *
 * @param mode the desired mode for the system (see: SystemMode).
 */
void CoreSetSystemMode(SystemMode mode);

/**
 * Return the current system mode (see: SystemMode enum).
 *
 * @return SystemMode enum
 */
SystemMode CoreGetSystemMode();

#endif /* CORE_MODE_H_ */
