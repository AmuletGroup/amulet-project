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
DRIVERSOURCES =$(wildcard port/fonts/*.c) $(wildcard port/SharpDisplay/*.c) $(wildcard port/CTS/*.c) $(wildcard port/FatFS/*.c) $(wildcard port/driverlib/MSP430FR5xx_6xx/*.c) $(wildcard port/ADXL362driver/*.c) $(wildcard port/L3GD20Hdriver/*.c)
DRIVEROBJECTS =$(patsubst %.c, %.obj, $(DRIVERSOURCES))

NANOSOURCES =$(wildcard */*_n.c)
ALLSOURCES = $(wildcard */*.c) $(wildcard *.c)
#NANOSOURCES = $(shell find . -name '*_n.c')
#ALLSOURCES = $(shell find . -name '*.c')
SOURCES = $(filter-out $(NANOSOURCES), $(ALLSOURCES))
OBJECTS = $(patsubst %.c, %.obj, $(SOURCES))
#BSP_INCS = $(patsubst %.c, %.h, $(BSP_SRCS)) driverlibHeaders.h


#
# Amulet Core
#



#
# Compiler and Linker configurations.
#

# TODO: Compiler & Compiler Flags (gcc? TI compiler?)
#OBJECTS=blink.o

CCS_DIR =/opt/ccsv6/ccsv6 
CCS_BIN_DIR =$(join $(CCS_DIR), /tools/compiler/msp430_4.3.3/bin)
CCS_INC_DIR =$(join $(CCS_DIR), /tools/compiler/msp430_4.3.3/include)
CCS_BASE_DIR =$(join $(CCS_DIR), /ccs_base/msp430/include)

SUPPORT_FILE_DIRECTORY =/opt/ti-mspgcc/include
export PATH := $(CCS_BIN_DIR):$(PATH)

ifeq ($(CC),cl430)
	#CCS DEFINES
	DEVICE = __MSP430FR5969__
	QPCLIB = $(QPCPORTDIR)/dbg/qp.lib

	ifeq ($(verbose),true)
		override CFLAGS += --advice:power=all 
	endif
	
	#-I /opt/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include/
	override CFLAGS += -vmspx --abi=eabi -g --c99 --define=$(DEVICE) --define=WITH_QPC  --include_path="." \
		--include_path="core" --include_path="port" --include_path="port/grlib" \
		--include_path="port/LcdDriver" --include_path="port/driverlib/MSP430FR5xx_6xx" \
		--include_path=$(QPCINC) --include_path=$(QPCPORTDIR) --include_path=$(CCS_INC_DIR) \
		--include_path="$(CCS_BASE_DIR)" 
	LFLAGS = --code_model=small --obj_directory="$(QPCLIB)" --diag_warning=225 \
		--display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 \
		--silicon_errata=CPU40 --printf_support=minimal -z -m"amulet-MSP-EXP430FR5969.map" \
		--heap_size=800 --stack_size=160 --use_hw_mpy=F5 --cinit_hold_wdt=on \
		-i"/opt/ccsv6/ccsv6/ccs_base/msp430/include" -i"/opt/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/lib" \
		-i"/opt/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" -i"/opt/ccsv6/ccsv6/ccs_base/msp430/lib/5xx_6xx_FRxx" \
		-i"/opt/ccsv6/ccsv6/ccs_base/msp430/lib/FR59xx" --reread_libs --priority --warn_sections \
		--display_error_number --diag_wrap=off --xml_link_info="amulet-MSP-EXP430FR5969_linkInfo.xml" --rom_model
	LIBS = -l"libc.a" -l"$(QPCLIB)" port/lnk_msp430fr5969.cmd
	#-vmspx --abi=eabi --advice:power=all -g --c99 --define=__MSP430FR5969__ --define=WITH_QPC --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal -z -m"amulet-MSP-EXP430FR5969.map" --heap_size=800 --stack_size=160 --use_hw_mpy=F5 --cinit_hold_wdt=on -i"/opt/ccsv6/ccsv6/ccs_base/msp430/include" -i"/opt/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/lib" -i"/opt/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" -i"/opt/ccsv6/ccsv6/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"/opt/ccsv6/ccsv6/ccs_base/msp430/lib/FR59xx" --reread_libs --priority --warn_sections --display_error_number --diag_wrap=off --xml_link_info="amulet-MSP-EXP430FR5969_linkInfo.xml" --rom_model -o "amulet-MSP-EXP430FR5969.out"
else
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
	LIBS = -L $(GCC_ROOT)/../include/ $(QPCLIB) -L libc.a -T port/gcc_lnk_msp430frX989.ld
endif





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
