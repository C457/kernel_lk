
#******************************************************************************
#	@ 2015-12-25: JJongspi
#******************************************************************************

GET_LOCAL_DIR    = $(patsubst %/,%,$(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))))

include ./build/makedefs

BUILD_PATH	= ./build

PLATFORM := tcc897x
#PLATFORM := tcc802x

ifeq ($(BOARD3HW_GPIO), true)
    SELECTED_DECODER := tw9990
endif

ifeq ($(PLATFORM),tcc897x)
CFLAGS += -DPLATFORM_TCC897X
endif
ifeq ($(PLATFORM),tcc802x)
CFLAGS += -DPLATFORM_TCC802X
endif

CFLAGS +=						\
	-I./include					\
	-I./include/dev					\
	-I./platform/${PLATFORM}/include		\
	-I./platform/${PLATFORM}/include/platform	\
	-I./platform/${PLATFORM}/vioc			\
	-I./platform/tcc_shared/include


# Init
BOOTOBJS :=
OBJS :=
LIBS :=


# set OBJS
BOOTOBJS = platform/$(PLATFORM)/startup.o

OBJS +=	\
	source/cm4_main.o	\
	source/camera.o		\
	source/delay.o		\
	source/cm4_cmd.o	\
	source/printk.o		\
	source/lcdc.o		

#include platform/$(PLATFORM)/rules.mk
# System devices
OBJS += \
	platform/$(PLATFORM)/gpio.o

# Peripharal devices
OBJS += \
	platform/$(PLATFORM)/uart.o \
	platform/$(PLATFORM)/i2c.o

# VIOC Display devices
OBJS += \
	platform/$(PLATFORM)/vioc/vioc_vin.o		\
	platform/$(PLATFORM)/vioc/vioc_viqe.o		\
	platform/$(PLATFORM)/vioc/vioc_deintls.o	\
	platform/$(PLATFORM)/vioc/vioc_scaler.o		\
	platform/$(PLATFORM)/vioc/vioc_wmix.o		\
	platform/$(PLATFORM)/vioc/vioc_wdma.o		\
	platform/$(PLATFORM)/vioc/vioc_rdma.o		\
	platform/$(PLATFORM)/vioc/vioc_fifo.o		\
	platform/$(PLATFORM)/vioc/vioc_disp.o		\
	platform/$(PLATFORM)/vioc/vioc_config.o

# set dev OBJS
include dev/rules.mk

# combine
ALLOBJS := \
	$(BOOTOBJS) \
	$(OBJS)

ALLOBJS := $(addprefix $(OUTPUT)/,$(ALLOBJS))

# The default rule, which causes init to be built.
all:					\
	${OUTPUT}			\
	${OUTPUT}/TCCortexM.axf
	 
# The rule to clean out all the build products
clean:
	@rm -rf ${OUTPUT} ${wildcard *.bin} TCCortexM.axf
	
# The rule to create the target directory
${OUTPUT}:
	@mkdir -p ${OUTPUT}
	@mkdir -p ${OUTPUT}/source
	@mkdir -p ${OUTPUT}/platform/${PLATFORM}/vioc
	@mkdir -p ${OUTPUT}/dev

${OUTPUT}/TCCortexM.axf: ${ALLOBJS} ${LIBS}
SCATTER_TCCortexM = ./platform/$(PLATFORM)/standalone.ld
ENTRY_TCCortexM   = ResetISR

# Include the automatically generated dependency files.
#-include ${wildcard ${OUTPUT}/*.d} __dummy__

