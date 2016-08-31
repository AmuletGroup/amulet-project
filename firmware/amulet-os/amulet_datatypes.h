/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#ifndef _AMULET_DATA_TYPES_H
#define _AMULET_DATA_TYPES_H

#include <stdint.h>

// Amulet AFT will tranlsate regular array to more secure array type. This file
// defines their mapping relationship, and the definitions are necessary to
// make the apps correctly built.
typedef struct {
	int *values;
	unsigned int __arr_len;
} __int_array;
typedef struct {
	uint8_t *values;
	unsigned int __arr_len;
} __uint8_t_array;
typedef struct {
	uint16_t *values;
	unsigned int __arr_len;
} __uint16_t_array;
typedef struct {
	int32_t *values;
	unsigned int __arr_len;
} __int32_t_array;
typedef struct {
	float *values;
	unsigned int __arr_len;
} __float_array;
typedef struct {
	char *values;
	unsigned int __arr_len;
} __char_array;
typedef struct {
	double *values;
	unsigned int __arr_len;
} __double_array;


#endif
