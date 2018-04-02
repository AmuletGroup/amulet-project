/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#ifndef _BSP_INIT_H_
#define _BSP_INIT_H_

/**
 * Board Type defined as part of the AFT firmware generation process.
 *
 * See: Configuration file in `project-amulet/src/aft/`.
 */
#ifdef BSP_DEV
#include "bsp_devboard_init.h"
#endif

#ifdef BSP_SNAIL_KITE
#include "bsp_snail_kite_init.h"
#endif

#ifdef BSP_WHITE_OWL
#include "bsp_white_owl_init.h"
#endif

#endif
