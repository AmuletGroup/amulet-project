/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

#ifndef NRF51822_H
#define NRF51822_H

bool is_ble_enabled();
void nrf51822_turn_on();
void nrf51822_turn_off();
void nrf51822_init_spi();
void nrf51822_select();
void nrf51822_deselect();
uint8_t nrf51822_spi_transfer(const uint8_t _data);
#endif  // NRF51822_H