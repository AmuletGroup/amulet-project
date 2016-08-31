/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/*
 * Eric Chen, August, 2014
 * Functions that calculate the drowsiness of an individual
 * Uses an FFT and compares the low and high frequency components.
 * The algorithm is adapted from code released by PhysioNet HRV toolkit.
 * The initial code can be found at physionet.org/tutorials/hrv-tookit
 * and physionet.org/physiotools/wfdb/psd/lomb.c
 */

//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//#include <string.h>

#include "core_algorithm.h"

float absolute(float x)
{
     if (x < 0)
         x = -x;
     return x;
}

float newton_sqrt(float x) {
	float guess = 1;
  while  ( absolute((guess * guess) / x  - 1.0) >= 0.001 ) {
		guess = ((x/guess) + guess) / 2;
	}
	return guess;
}

/* 
 * Finds the average RR interval for the samples
 */
float getAverage(float total_time, int numBeats){
	return total_time/numBeats;

}

/*
 * Finds the standard deviation for the RR intervals given.
 * @x - pointer to an array of the rr_intervals
 * @total_time - the sum of all the RR intervals
 * @numBeats - the number of RR samples.
 * @return - the standard deviation.
 */
float getStdDev(float* x, float total_time, uint8_t numBeats){
	float stdDev = 0.0;
	float average = total_time/numBeats;
	int i;
	for (i = 0; i < numBeats; i++){
		stdDev += (x[i] - average)*(x[i] - average);
	}
	stdDev = stdDev/numBeats;
	stdDev = newton_sqrt(stdDev);
	return stdDev;
}
