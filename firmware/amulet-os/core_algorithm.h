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
