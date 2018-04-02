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
