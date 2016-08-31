/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#include "amulet_func.h"

void amulet_comm_bus_init() {
     // This has to be in here to disconnect pins connected to the MSP430 so no power faults
    nrf_gpio_cfg_input(SPIS_CS, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(SPIS_SIMO, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(SPIS_SOMI, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(SPIS_SCLK, NRF_GPIO_PIN_NOPULL);
#if AMULET_DEVICE_NAME == SNAIL_KITE
    nrf_gpio_cfg_input(BLE_RDY, NRF_GPIO_PIN_NOPULL);
#endif
}