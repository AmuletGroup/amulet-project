/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 * core_sensors.h
 *
 *  Created on: Oct 11, 2013
 *      Author: Andres
 */

#ifndef CORE_ALGORITHM_H_
#define CORE_ALGORITHM_H_

#include "amulet.h"

/*
 * Adapted from code found of physionet. The link is in the comments at the top of the file.
 * Gets the LF/HF ratio for the given data.
 * @rrs - pointer to an array of the rr intervals.
 * @total_times - pointer an array of the time elapsed up to the corresponding rr interval.
 * @return - the LF/HF ratio.
 */
float getStdDev(float* x, float total_time, uint8_t numBeats);

#endif /* CORE_ALGORITHM_H_ */
