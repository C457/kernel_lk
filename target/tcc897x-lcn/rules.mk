LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/include -I$(LK_TOP_DIR)/platform/tcc_shared

PLATFORM := tcc897x

#===========================================================================
# Define board revision
#===========================================================================
# 4th 4bits      / 3rd 4bits  / 2nd 4bits           / 1st 4bits
# (Board)        / (chipset)  / (Memory)            / (ver.)
# --------------------------------------------------------------------------
# Define board revision
# 0x4210 : LCN   /TCC8971     /DDR3 1024MB(16bit*2) /TCC8971_LCN_CPU_MOD_4L_SV1.0
# 0x4211 : LCN   /TCC8972     /DDR3 1024MB(16bit*2) /TCC8972_LCN_CPU_MOD_4L_SV1.0

HW_REV=0x4210


#==================================================================
# System Setting
#==================================================================


#------------------------------------------------------------------
# BASE Address
#------------------------------------------------------------------

#	0x80000000 |------------------------------|
#	0x80000100 |------------------------------|
#	           |             ATAG             |
#	0x80008000 |------------------------------|
#	           |         Kernel Image         |
#	0x81000000 |------------------------------|
#	           |           ramdisk            |
#	0x82000000 |------------------------------|
#	           |        Secure Kernel         |
#	0x83000000 |------------------------------|
#	           |         Device Tree          |
#	           | ~  ~  ~  ~  ~  ~  ~  ~  ~  ~ |
#	           |------------------------------| PMAP_BASE
#	           |        FB_VIDEO_BASE         |
#	0x88000000 |------------------------------|
#	           |     LK Bootloader Image      |
#	           |------------------------------|
#	           |          heap area           |
#	0x89000000 |------------------------------|
#	           |             dma              |
#	0x89800000 |------------------------------|
#	0x8C000000 |------------------------------|
#	           |       QB Scratch area        |
#	0x8C400000 |------------------------------|
#	           |           Scratch            |
#	           | ~  ~  ~  ~  ~  ~  ~  ~  ~  ~ |
#	           |------------------------------|

BASE_ADDR        := 0x80000000

# kernel parts
TAGS_ADDR        := BASE_ADDR+0x00000100
KERNEL_ADDR      := BASE_ADDR+0x00008000
RAMDISK_ADDR     := BASE_ADDR+0x02000000
SKERNEL_ADDR     := BASE_ADDR+0x03000000
DTB_ADDR         := BASE_ADDR+0x04000000

# lk bootloader parts
MEMBASE          := 0xA8000000		# kernel's mem reserve area (PMAP)
MEMSIZE          := 0x01000000		# 16MB
DMA_SIZE         := 0x00800000		# 8MB
QB_SCRATCH_ADDR  := MEMBASE+0x04000000    # 2MB
JPEG_DECODE_ADDR := MEMBASE+0x04400000
SCRATCH_ADDR     := MEMBASE+0x06200000


#==================================================================
# SDRAM Setting
#==================================================================
#------------------------------------------------------------------
# SDRAM CONTROLLER TYPE
#------------------------------------------------------------------
#TCC_MEM_TYPE := DRAM_LPDDR2
TCC_MEM_TYPE := DRAM_DDR3

#------------------------------------------------------------------
# Define memory bus width
#------------------------------------------------------------------
#DEFINES += CONFIG_DRAM_16BIT_USED
DEFINES += CONFIG_DRAM_32BIT_USED

#------------------------------------------------------------------
# Define memory size in MB
#------------------------------------------------------------------
ifneq ($(filter $(HW_REV),0x4210 0x4211),)
ifeq ($(BOARD_HAS_2GB_DDR), true)
	TCC_MEM_SIZE := 2048
else
	TCC_MEM_SIZE := 1024
endif
else
#	TCC_MEM_SIZE := 2048
endif

#------------------------------------------------------------------
# SDRAM DDR3 Config
#------------------------------------------------------------------
ifeq ($(TCC_MEM_TYPE), DRAM_DDR3)
	#parameter setting
#	DEFINES += CONFIG_DDR3_1866
#	DEFINES += CONFIG_DDR3_1600
#	DEFINES += CONFIG_DDR3_1333H
#	DEFINES += CONFIG_DDR3_1066
#	DEFINES += CONFIG_DDR3_800E

ifneq ($(filter $(HW_REV),0x4210 0x4211),)
ifeq ($(BOARD_HAS_2GB_DDR), true)
ifeq ($(BOARD3HW_HAS_2GB_DDR), true)
DEFINES += CONFIG_DDR3_MODULE_512MB
DEFINES += CONFIG_DDR3_1600
DEFINES += CONFIG_DDR3_IF_8
DEFINES += CONFIG_DDR3_1CS
else
DEFINES += CONFIG_DDR3_MODULE_1024MB
DEFINES += CONFIG_DDR3_1600
DEFINES += CONFIG_DDR3_IF_16
DEFINES += CONFIG_DDR3_1CS
endif
else
DEFINES += CONFIG_DDR3_MODULE_512MB
DEFINES += CONFIG_DDR3_1600
DEFINES += CONFIG_DDR3_IF_16
DEFINES += CONFIG_DDR3_1CS
endif
endif
DEFINES += DRAM_DDR3
endif

ifeq ($(INCLUDE_LCD_HDMI_1920x720_12_3), true)
CFLAGS += -DWIDE_LCD_DEFINE
DEFINES += DEFAULT_DISPLAY_HDMI
DEFINES += HDMI_1920X720
TCC_HDMI_USE := 1
DEFINES += DISPLAY_SPLASH_SCREEN_DIRECT=1

else

ifeq ($(INCLUDE_LCD_HDMI_960_1280), true) 
CFLAGS += -DWIDE_LCD_DEFINE
DEFINES += DEFAULT_DISPLAY_HDMI
DEFINES += HDMI_1920X720
TCC_HDMI_USE := 1
DEFINES += DISPLAY_SPLASH_SCREEN_DIRECT=1
else
DEFINES += DEFAULT_DISPLAY_LCD
DEFINES += FLD0800	  # 1024x600 
TCC_HDMI_USE := 1
endif

endif

ifeq ($(TCC_HDMI_USE), 1)
  DEFINES += TCC_HDMI_USE
  DEFINES += TCC_HDMI_USE_XIN_24MHZ
  DEFINES += TCC_HDMI_DRIVER_V1_4
endif

DEFINES += DISPLAY_SPLASH_SCREEN=1

#DEFINES += DISPLAY_TYPE_DSI6G=1

#TCC_SPLASH_USE := 1

#------------------------------------------------------------------
# Daudio Config add
#------------------------------------------------------------------
TCC_SPLASH_USE := 1
DEFINES += CONFIG_DAUDIO

#------------------------------------------------------------------
# for enable early camera(used cortex-m4)
#------------------------------------------------------------------
USE_CM4_EARLY_CAM := 1
ifeq ($(DAUDIO),1)
    ifeq ($(BOARD3HW_GPIO), true)
        EARLY_CAM_MODULE := tw9990
    else
        EARLY_CAM_MODULE := tw9921
    endif
else
EARLY_CAM_MODULE := adv7182
endif

ifeq ($(USE_CM4_EARLY_CAM),1)
	DEFINES += USE_CM4_EARLY_CAM
	DEFINES += CONFIG_VIDEO_TCCXXX_V4L_DEVICE
	MODULES += dev/camera
	ifeq ($(DAUDIO),1)
        DEFINES += CONFIG_TCC_PARKING_GUIDE_LINE
	endif
ifeq ($(EARLY_CAM_MODULE),tw9921)
	DEFINES += CONFIG_CM4_EARLY_LVDS
endif
endif

#------------------------------------------------------------------
# GPIO setting by Board Version.
#------------------------------------------------------------------
ifeq ($(BOARD3HW_GPIO), true)
        DEFINES += DAUDIO_KK_3HW_GPIO
endif


#------------------------------------------------------------------
# Keypad Type
#------------------------------------------------------------------
ifneq ($(DAUDIO),1)
KEYS_USE_GPIO_KEYPAD := 1
endif
#KEYS_USE_ADC_KEYPAD := 1
ifeq ($(DAUDIO),1)
MODULES += \
	dev/pmic/pfuze100 \
	lib/ptable \
	lib/libfdt
else
MODULES += \
	dev/pmic/rt5028 \
	lib/ptable \
	lib/libfdt
endif

# //+[TCCQB] Add QuickBoot Scratch Area.
DEFINES += \
	MEMSIZE=$(MEMSIZE) \
	MEMBASE=$(MEMBASE) \
	BASE_ADDR=$(BASE_ADDR) \
	TAGS_ADDR=$(TAGS_ADDR) \
	KERNEL_ADDR=$(KERNEL_ADDR) \
	RAMDISK_ADDR=$(RAMDISK_ADDR) \
	QB_SCRATCH_ADDR=$(QB_SCRATCH_ADDR) \
	JPEG_DECODE_ADDR=$(JPEG_DECODE_ADDR) \
	SCRATCH_ADDR=$(SCRATCH_ADDR) \
	SKERNEL_ADDR=$(SKERNEL_ADDR) \
	DTB_ADDR=$(DTB_ADDR)\
	TCC_MEM_SIZE=$(TCC_MEM_SIZE) \
	HW_REV=$(HW_REV)
# //-[TCCQB]
# //

#------------------------------------------------------------------
OBJS += \
	$(LOCAL_DIR)/clock.o \
	$(LOCAL_DIR)/init.o \
	$(LOCAL_DIR)/gpio.o \
	$(LOCAL_DIR)/target_display.o \
	#$(LOCAL_DIR)/atags.o

ifeq ($(DAUDIO),1)
	DEFINES += PFUZE100_PMIC
else
	DEFINES += RT5028_PMIC
endif
#--------------------------------------------------------------
# for USB 3.0 Device
#--------------------------------------------------------------
TCC_USB_30_USE := 0

ifeq ($(DAUDIO),1)
	DEBUG := 4
endif

ifeq ($(TCC_USB_30_USE), 1)
  DEFINES += TCC_USB_30_USE
endif

#==================================================================
# SXM 	
#================================================================== 	
ifeq ($(INCLUDE_XM), true)	
DEFINES += INCLUDE_XM=1	
endif	

#==================================================================
# ISDB 	
#================================================================== 	
ifeq ($(INCLUDE_ISDB), true)	
DEFINES += INCLUDE_ISDB=1	
endif	

#==================================================================
# FM1688
#==================================================================
ifeq ($(INCLUDE_FM1688), true)
DEFINES += INCLUDE_FM1688=1
endif



#==================================================================
# MAX_SER_DES
#==================================================================
ifeq ($(INCLUDE_MAX_SER_DES), true)
DEFINES += INCLUDE_MAX_SER_DES=1
endif


#================================================================== 
# U-Boot Features
# check ./u-boot/u-boot.rom
#==================================================================
UBOOT_RECOVERY_USE := 1

#==================================================================
# SUPPORT SPLASH JPEG
#================================================================== 	
TCC_SUPPORT_SPLASH_JPEG := 1

ifeq ($(TCC_SUPPORT_SPLASH_JPEG), 1)
  DEFINES += TCC_SUPPORT_SPLASH_JPEG
endif

# Support Wide Platform
ifeq ($(INCLUDE_LCD_HDMI_1920x720_12_3), true)
	DEFINES += CONFIG_LCD_HDMI1280X720
endif

# Support Video Decoding PMAP
ifeq ($(SUPPORT_VIDEO_DECODING_MEMORY), true)
	DEFINES += CONFIG_VIDEO_DECODING_MEMORY
endif

include platform/tcc_shared/daudio_rules.mk
