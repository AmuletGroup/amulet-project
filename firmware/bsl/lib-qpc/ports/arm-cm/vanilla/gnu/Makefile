#-----------------------------------------------------------------------------
# GNU ARM compiler
#
ifeq ($(GNU_ARM),)
#GNU_ARM = ~/sat
GNU_ARM = /opt/sat
endif

CC    := $(GNU_ARM)/bin/arm-none-eabi-gcc
ASM    := $(GNU_ARM)/bin/arm-none-eabi-as
LIB    := $(GNU_ARM)/bin/arm-none-eabi-ar
LINK  := $(GNU_ARM)/bin/arm-none-eabi-gcc
BIN   := $(GNU_ARM)/bin/arm-none-eabi-objcopy
RM    := rm -rf
MKDIR := mkdir
#RM    := rm

QP_INCDIR :=../../../../include
QP_PRTDIR :=.

ARM_CORE = cortex-m4
ARM_FPU  = vfp

#-----------------------------------------------------------------------------
# build options for various configurations
#

ifeq (rel, $(CONF))       # Release configuration ............................

BINDIR := rel
CCFLAGS=-c -mcpu=$(ARM_CORE) -mfpu=$(ARM_FPU) -mfloat-abi=softfp -D__VFP_FP__ -mthumb -Wall -Os -DNDEBUG
ASMFLAGS= -mcpu=$(ARM_CORE) -defsym=FPU_VFP_V4_SP_D16=1

else ifeq (spy, $(CONF))  # Spy configuration ................................

BINDIR := spy

CCFLAGS = -g -c -mcpu=$(ARM_CORE) -mfpu=$(ARM_FPU) -mfloat-abi=softfp -D__VFP_FP__ -mthumb -Wall -O -DQ_SPY

ASMFLAGS = -g -mcpu=$(ARM_CORE) -defsym=FPU_VFP_V4_SP_D16=1

else                     # default Debug configuration .......................

BINDIR := dbg

CCFLAGS=-g -c -mcpu=$(ARM_CORE) -mfpu=$(ARM_FPU) -mfloat-abi=softfp -D__VFP_FP__ -mthumb -Wall -O

ASMFLAGS=-g -mcpu=$(ARM_CORE) -defsym=FPU_VFP_V4_SP_D16=1

endif



all: QEP QF QS

# QEP
QEPSRCDIR :=../../../../qep/source
QEPCCINC = -I$(QP_PRTDIR) -I$(QP_INCDIR) -I$(QEPSRCDIR)
LIBDIR = $(BINDIR)
LIBFLAGS = rs

QFSRCDIR :=../../../../qf/source
QFCCINC = -I$(QP_PRTDIR) -I$(QP_INCDIR) -I$(QFSRCDIR)

QSSRCDIR=../../../../qs/source
QSCCINC=-I$(QP_PRTDIR) -I$(QP_INCDIR) -I$(QSSRCDIR)

QEP:
	#mkdir $(BINDIR)
	mkdir -p $(BINDIR)
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qep.c      -o$(BINDIR)/qep.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qmsm_ini.c -o$(BINDIR)/qmsm_ini.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qmsm_dis.c -o$(BINDIR)/qmsm_dis.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qfsm_ini.c -o$(BINDIR)/qfsm_ini.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qfsm_dis.c -o$(BINDIR)/qfsm_dis.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qhsm_ini.c -o$(BINDIR)/qhsm_ini.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qhsm_dis.c -o$(BINDIR)/qhsm_dis.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qhsm_top.c -o$(BINDIR)/qhsm_top.o
	$(CC) $(CCFLAGS) $(QEPCCINC) $(QEPSRCDIR)/qhsm_in.c  -o$(BINDIR)/qhsm_in.o
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqp_$(ARM_CORE)f_cs.a $(BINDIR)/qep.o $(BINDIR)/qmsm_ini.o \
	$(BINDIR)/qmsm_dis.o $(BINDIR)/qfsm_ini.o $(BINDIR)/qfsm_dis.o $(BINDIR)/qhsm_ini.o \
	$(BINDIR)/qhsm_dis.o $(BINDIR)/qhsm_top.o $(BINDIR)/qhsm_in.o
	$(RM) $(BINDIR)/*.o


QF:
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_ctor.c  -o$(BINDIR)/qa_ctor.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_defer.c -o$(BINDIR)/qa_defer.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_fifo.c  -o$(BINDIR)/qa_fifo.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_lifo.c  -o$(BINDIR)/qa_lifo.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_get_.c  -o$(BINDIR)/qa_get_.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_sub.c   -o$(BINDIR)/qa_sub.o  
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_usub.c  -o$(BINDIR)/qa_usub.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qa_usuba.c -o$(BINDIR)/qa_usuba.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qeq_fifo.c -o$(BINDIR)/qeq_fifo.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qeq_get.c  -o$(BINDIR)/qeq_get.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qeq_init.c -o$(BINDIR)/qeq_init.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qeq_lifo.c -o$(BINDIR)/qeq_lifo.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_act.c   -o$(BINDIR)/qf_act.o  
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_gc.c    -o$(BINDIR)/qf_gc.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_log2.c  -o$(BINDIR)/qf_log2.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_new.c   -o$(BINDIR)/qf_new.o  
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_pool.c  -o$(BINDIR)/qf_pool.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_psini.c -o$(BINDIR)/qf_psini.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_pspub.c -o$(BINDIR)/qf_pspub.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_pwr2.c  -o$(BINDIR)/qf_pwr2.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qf_tick.c  -o$(BINDIR)/qf_tick.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qma_ctor.c -o$(BINDIR)/qma_ctor.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qmp_get.c  -o$(BINDIR)/qmp_get.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qmp_init.c -o$(BINDIR)/qmp_init.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qmp_put.c  -o$(BINDIR)/qmp_put.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qte_ctor.c -o$(BINDIR)/qte_ctor.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qte_arm.c  -o$(BINDIR)/qte_arm.o 
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qte_darm.c -o$(BINDIR)/qte_darm.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qte_rarm.c -o$(BINDIR)/qte_rarm.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qte_ctr.c  -o$(BINDIR)/qte_ctr.o
	$(CC) $(CCFLAGS) $(QFCCINC) $(QFSRCDIR)/qvanilla.c -o$(BINDIR)/qvanilla.o
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqp_$(ARM_CORE)f_cs.a $(BINDIR)/qa_ctor.o $(BINDIR)/qa_defer.o \
	$(BINDIR)/qa_fifo.o $(BINDIR)/qa_lifo.o $(BINDIR)/qa_get_.o $(BINDIR)/qa_sub.o $(BINDIR)/qa_usub.o \
	$(BINDIR)/qa_usuba.o $(BINDIR)/qeq_fifo.o $(BINDIR)/qeq_get.o $(BINDIR)/qeq_init.o $(BINDIR)/qeq_lifo.o \
	$(BINDIR)/qf_act.o $(BINDIR)/qf_gc.o $(BINDIR)/qf_log2.o $(BINDIR)/qf_new.o $(BINDIR)/qf_pool.o \
	$(BINDIR)/qf_psini.o $(BINDIR)/qf_pspub.o $(BINDIR)/qf_pwr2.o $(BINDIR)/qf_tick.o $(BINDIR)/qma_ctor.o \
	$(BINDIR)/qmp_get.o $(BINDIR)/qmp_init.o $(BINDIR)/qmp_put.o $(BINDIR)/qte_ctor.o $(BINDIR)/qte_arm.o \
	$(BINDIR)/qte_darm.o $(BINDIR)/qte_rarm.o $(BINDIR)/qte_ctr.o $(BINDIR)/qvanilla.o
	$(RM) $(BINDIR)/*.o
	
ifeq (spy, $(CONF)) 

QS:
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs.c      -o$(BINDIR)/qs.o     
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_.c     -o$(BINDIR)/qs_.o     
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_blk.c  -o$(BINDIR)/qs_blk.o 
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_byte.c -o$(BINDIR)/qs_byte.o
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_dict.c -o$(BINDIR)/qs_dict.o
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_f32.c  -o$(BINDIR)/qs_f32.o 
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_f64.c  -o$(BINDIR)/qs_f64.o 
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_mem.c  -o$(BINDIR)/qs_mem.o 
	$(CC) $(CCFLAGS) $(QSCCINC) $(QSSRCDIR)/qs_str.c  -o$(BINDIR)/qs_str.o
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqp_$(ARM_CORE)f_cs.a $(BINDIR)/qs.o $(BINDIR)/qs_.o \
	$(BINDIR)/qs_blk.o $(BINDIR)/qs_byte.o $(BINDIR)/qs_dict.o $(BINDIR)/qs_f32.o \
	$(BINDIR)/qs_f64.o $(BINDIR)/qs_mem.o $(BINDIR)/qs_str.o
	$(RM) $(BINDIR)/*.o

else

QS:
	@echo CONF = $(CONF)

endif

clean:
	rm -f $(LIBDIR)/libqp_$(ARM_CORE)f_cs.a	
	
# QS
#SRCDIR :=../../../../qs/source
#CCINC=-I$(QP_PRTDIR) -I$(QP_INCDIR) -I$(SRCDIR)
