LOCAL_DIR := $(GET_LOCAL_DIR)

#ARCH := arm
##Compiling this as cortex-a8 until the compiler supports cortex-a7
#ARM_CPU := cortex-a8
#CPU	:= generic

#DEFINES += ARM_CPU_CORE_A7
DEFINES += TCC802X
#DEFINES += \
#	_LINUX_

#INCLUDES += -I$(LOCAL_DIR)/include -I$(TOP_DIR)/platform/tcc_shared/include

#==================================================================
# TCC802x Platform Defines
#==================================================================

# System devices
OBJS += \
	$(LOCAL_DIR)/gpio.o

# Peripharal devices
#OBJS += \
#	$(LOCAL_DIR)/uart.o \
#	$(LOCAL_DIR)/i2c.o

# VIOC Display devices
#OBJS += \
#	$(LOCAL_DIR)/vioc/vioc_vin.o \
#	$(LOCAL_DIR)/vioc/vioc_viqe.o \
#	$(LOCAL_DIR)/vioc/vioc_rdma.o \
#	$(LOCAL_DIR)/vioc/vioc_wdma.o \
#	$(LOCAL_DIR)/vioc/vioc_wmix.o \
#	$(LOCAL_DIR)/vioc/vioc_disp.o \
#	$(LOCAL_DIR)/vioc/vioc_scaler.o \
#	$(LOCAL_DIR)/vioc/vioc_lvds.o \
#	$(LOCAL_DIR)/vioc/vioc_config.o

#LINKER_SCRIPT += $(BUILDDIR)/standardalone.ld

#include platform/tcc_shared/rules.mk

