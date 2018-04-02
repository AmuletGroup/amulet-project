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
