################################################################################
# Amulet Makefile
#

#
# Relevant project directores.
#
QPCDIR  = $(QPC_ROOT)
QPCPORTDIR = $(QPCDIR)/ports/msp430/vanilla/ccs-mspx
QPCINC = $(QPCDIR)/include
#
# BSP
#
# Other files?
#
#BSP_SRCS = bsp_actuators.c bsp_setup.c bsp_uart.c IPC.c
DRIVERSOURCES =$(wildcard port/fonts/*.c) $(wildcard port/SharpDisplay/*.c) $(wildcard port/FatFS/*.c) $(wildcard port/driverlib/MSP430FR5xx_6xx/*.c) $(wildcard port/ADXL362driver/*.c) $(wildcard port/Gyro/*.c)
DRIVEROBJECTS =$(patsubst %.c, %.obj, $(DRIVERSOURCES))

NANOSOURCES =$(wildcard */*_n.c)
ALLSOURCES = $(wildcard */*.c) $(wildcard *.c)
#NANOSOURCES = $(shell find . -name '*_n.c')
#ALLSOURCES = $(shell find . -name '*.c')
SOURCES = $(filter-out $(NANOSOURCES), $(ALLSOURCES))
OBJECTS = $(patsubst %.c, %.obj, $(SOURCES))
#BSP_INCS = $(patsubst %.c, %.h, $(BSP_SRCS)) driverlibHeaders.h

#
# Compiler and Linker configurations.
#

#GCC DEFINES
MAKEFLAGS += --quiet
QPCLIB = $(QPCPORTDIR)/dbg/libqp.a
MSP430_CORE = msp430x
DEVICE  = msp430fr5989
GCC_ROOT = $(GCC_DIR)
GDB     = $(GCC_ROOT)/msp430-elf-gdb

# GCC flags
CSTD_FLAGS = -std=c99 -funsigned-char
DEBUG_FLAGS = -g -g3 -ggdb -gdwarf-2
ERROR_FLAGS = -Wall -Wextra -Wshadow -Wfatal-errors
NO_ERROR_FLAGS = -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable -Wno-type-limits -Wno-comment -Wno-old-style-declaration
FLAGS_TO_FIX = -Wno-int-to-pointer-cast
LIB_INCLUDES = -D=WITH_QPC -I. -Icore -Iport -Iport/SharpDisplay -Iport/SharpDisplay/fonts -Iport/driverlib/MSP430FR5xx_6xx -I$(QPCINC) -I$(QPCPORTDIR) -I $(GCC_ROOT)/../include/
MSP430_FLAGS = -mmcu=$(DEVICE) -mhwmult=f5series -D__MSP430FR5989__=1 -mlarge
REDUCE_SIZE_FLAGS = -fdata-sections -ffunction-sections -finline-small-functions -Os

override CFLAGS += $(CSTD_FLAGS) $(DEBUG_FLAGS) $(ERROR_FLAGS) $(NO_ERROR_FLAGS) $(FLAGS_TO_FIX) $(LIB_INCLUDES) $(MSP430_FLAGS) $(REDUCE_SIZE_FLAGS)
	 
LFLAGS = -lm -Wl,--gc-sections -Wl,--reduce-memory-overheads -Wl,--stats -Wl,--relax -Xlinker -Map=gcc.map 

ifeq ($(GCC_VERSION),6)
LFLAGS += -T port/msp430frX989_gcc6.ld
endif

ifeq ($(GCC_VERSION),5) 
LFLAGS += -T port/msp430frX989_gcc4.ld
endif

ifeq ($(GCC_VERSION),4) 
LFLAGS += -T port/msp430frX989_gcc4.ld
endif

LIBS = -L $(GCC_ROOT)/../include/ $(QPCLIB) -L libc.a 




################################################################################
# Targets for different makes
#
# TODO:
#   - I'm not sure how/where the "lnk_msp430fr5969.cmd" file fits in here...
#   - I'm not sure how to properly setup the additonal include directories we need...
#   - We probably want to add a target for running the compiler tools (translator + anything else?)

all: compile

%.obj: %.c $(DEPS)
ifeq ($(CC),cl430)
	$(CC) $(CFLAGS) $(LFLAGS)  $< -o
else
	$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@
endif

drivers: $(DRIVEROBJECTS)
	echo $(DRIVEROBJECTS) 

# Change -o to $(DEVICE) when we need multiple devices
compile: $(OBJECTS) $(DRIVEROBJECTS)
ifeq ($(CC),cl430)
	$(CC) $(CFLAGS) $(LFLAGS) -o msp430fr5969.out *.obj $(LIBS) 
else
	$(CC) $(CFLAGS) $(OBJECTS) $(DRIVEROBJECTS) $(LFLAGS) -o msp430fr5969.out $(LIBS) 
endif

clean:
	rm -f *.obj *~ *.out
