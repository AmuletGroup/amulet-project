PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
# Various C libraries that need to be included
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += device_manager_central.c
APPLICATION_SRCS += ble_db_discovery.c
APPLICATION_SRCS += pstorage.c
APPLICATION_SRCS += app_trace.c
APPLICATION_SRCS += ble_hrs_c.c
APPLICATION_SRCS += ble_bas_c.c
APPLICATION_SRCS += app_uart.c
APPLICATION_SRCS += nrf_drv_uart.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += sensorsim.c
APPLICATION_SRCS += ble_bas.c
APPLICATION_SRCS += ble_hrs.c
APPLICATION_SRCS += ble_dis.c
APPLICATION_SRCS += ble_advertising.c

# Add other libraries here!
LIBRARY_PATHS += .
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/device_manager/config/)

# platform-level headers and source files
LIBRARY_PATHS += ../../include
SOURCE_PATHS += ../../src

# Device info
NRF_IC = nrf51822_xxac
RAM_KB = 32
FLASH_KB = 256

# Set the softdevice needed for the application
SOFTDEVICE_MODEL = s130

# Set to NOT optimize for size. With S130 this optimization often screws things up
CFLAGS = -O0

# Include the main Makefile
NRF_BASE_PATH ?= ../../nrf5x-base
include $(NRF_BASE_PATH)/make/Makefile	