#ifndef NRF51822_H
#define NRF51822_H

bool is_ble_enabled();
void nrf51822_turn_on();
void nrf51822_turn_off();
void nrf51822_init_spi();
void nrf51822_select();
void nrf51822_deselect();
uint8_t nrf51822_spi_transfer(const uint8_t _data);
uint8_t parseHeartRateAndRRI(uint8_t *packet, uint8_t length, uint16_t data[10]);
#endif  // NRF51822_H
