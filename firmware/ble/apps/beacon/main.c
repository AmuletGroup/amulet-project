/*
 * Send an advertisement periodically
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "led.h"

#include "simple_ble.h"
#include "simple_adv.h"

#include "amulet_board.h"
#include "amulet_func.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x00,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = "amulet_beacon",
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

int main(void) {
    // Setup BLE
    simple_ble_init(&ble_config);

    // Advertise because why not
    simple_adv_only_name();
    
    // Do this first, not softdevice specific
    amulet_comm_bus_init();

    while (1) {
        power_manage();
    }
}
