LOCAL_DIR := $(GET_LOCAL_DIR)

#DEFINES += ANDROID_NAND=1
DEFINES += PLATFORM_TCC=1
#DEFINES += ANDROID_BOOT=1
DEFINES += WITH_DMA_ZONE=1
#DEFINES += NAND_DEVICE_TREE=1

#==================================================================
# D-Audio1.1 board
#==================================================================
ifeq ($(DAUDIO),1)
DEFINES += TARGET_BOARD_DAUDIO11
DEFINES += INCLUDE_DAUDIO_ECO

DEFINES += TCC_ADC_NO_TOUCHSCREEN

#==================================================================
# D-Audio BSP version
#==================================================================
DEFINES += BSP_LK_VERSION=20160512

#==================================================================
# D-Audio Quickboot SIG
#==================================================================
ifneq ($(TARGET_PRODUCT),)
DEFINES_C += QUICKBOOT_SIG=\"$(TARGET_PRODUCT)\"
endif

#==================================================================
# D-Audio HDMI LCD 12.3 inch 1920 X 720
#==================================================================
ifeq ($(INCLUDE_LCD_HDMI_1920x720_12_3), true)
DEFINES += HDMI_1920_720_12_3
endif

#==================================================================
# PIO Vertical Panel HDMI LCD 960 X 1280
#==================================================================
ifeq ($(INCLUDE_LCD_HDMI_960x1280), true)
DEFINES += HDMI_1920_720_12_3
DEFINES += HDMI_960_1280
endif

#==================================================================
# PIO 3.0 WIDE New Partition (64GB eMMC)
#==================================================================
ifeq ($(PIO_WIDE_128GB_PARTITION), true)
DEFINES += PIO_WIDE_128GB_PARTITION
endif

ifeq ($(PIO_WIDE_64GB_PARTITION), true)
DEFINES += PIO_WIDE_64GB_PARTITION
endif

ifeq ($(PIO_WIDE_4GB_PARTITION), true)
DEFINES += PIO_WIDE_4GB_PARTITION
endif

ifeq ($(PIO_WIDE_8GB_PARTITION), true)
DEFINES += PIO_WIDE_8GB_PARTITION
endif

endif

ifeq ($(INCLUDE_CURVED_TI_SERDES), true)
DEFINES += INCLUDE_CURVED_TI_SERDES
endif

ifneq ($(DMA_SIZE),)
DEFINES += _DMA_SIZE=$(DMA_SIZE)
endif

DEFINES += $(TARGET_XRES)
DEFINES += $(TARGET_YRES)

INCLUDES += -I$(LOCAL_DIR)/include -I$(LOCAL_DIR)/include/fwdn -I$(LOCAL_DIR)/include/tnftl -I$(LOCAL_DIR)/include/tcc_remocon  -I$(LOCAL_DIR)/include/usb3.0 -I$(LOCAL_DIR)/include/tcsb -I$(LOCAL_DIR)/include/splash -I$(LOCAL_DIR)/../$(PLATFORM)

OBJS += \
	$(LOCAL_DIR)/debug.o \
	$(LOCAL_DIR)/div64.o \
	$(LOCAL_DIR)/timer.o \
	$(LOCAL_DIR)/clock.o \
    	$(LOCAL_DIR)/power.o \
	$(LOCAL_DIR)/tca_tco.o \
	$(LOCAL_DIR)/sfl.o\
	$(LOCAL_DIR)/partition_parser.o \
	$(LOCAL_DIR)/component_cs4954.o \
	$(LOCAL_DIR)/component_ths8200.o \
	$(LOCAL_DIR)/tcc_edi.o \
	$(LOCAL_DIR)/pwm.o \
	$(LOCAL_DIR)/adc.Ao

OBJS += \
	$(LOCAL_DIR)/usb_device.o \
	$(LOCAL_DIR)/usbdev_class.o \
	$(LOCAL_DIR)/usbphy.o \
	$(LOCAL_DIR)/vtc.o \
	$(LOCAL_DIR)/fastbootusb.o \
	$(LOCAL_DIR)/hsusb.o \
	$(LOCAL_DIR)/usb_manager.o

ifeq ($(TCC_USB_30_USE), 1)
OBJS += \
	$(LOCAL_DIR)/usb3.0/usb30dev.o \
	$(LOCAL_DIR)/usb3.0/usb30reset.o \
	$(LOCAL_DIR)/usb3.0/pcd.o \
	$(LOCAL_DIR)/usb3.0/pcd_intr.o \
	$(LOCAL_DIR)/usb3.0/cil.o \
	$(LOCAL_DIR)/usb3.0/cil_intr.o
else
OBJS += \
	$(LOCAL_DIR)/otgcore.o \
	$(LOCAL_DIR)/otgdev_io.o 
endif

OBJS += \
	$(LOCAL_DIR)/nand.o

OBJS += \
	$(LOCAL_DIR)/fwdn_drv_v7.o \
	$(LOCAL_DIR)/fwdn_main.Ao \
	$(LOCAL_DIR)/fwdn_nand.o \
	$(LOCAL_DIR)/disk.o \
	$(LOCAL_DIR)/fwdn_protocol_v7.o

OBJS += \
	$(LOCAL_DIR)/lcd/lcd_at070tn93.o \
	$(LOCAL_DIR)/lcd/lcd_ED090NA.o\
	$(LOCAL_DIR)/lcd/lcd_FLD0800.o\
	$(LOCAL_DIR)/lcd/lcd_select.o\
	$(LOCAL_DIR)/dev_tree.o

ifeq ($(TCC_HDMI_USE), 1)
OBJS += \
	$(LOCAL_DIR)/hdmi_1280x720.o
endif

ifneq ($(filter true,$(BOOTING_BY_REMOTE_KEY) $(RECOVERY_BY_REMOTE_KEY) $(START_FWDN_BY_REMOTE_KEY)),)
OBJS+= \
	$(LOCAL_DIR)/tcc_remocon.o
endif

ifeq ($(EMMC_BOOT),1)
OBJS+= \
	$(LOCAL_DIR)/emmc.o \
	$(LOCAL_DIR)/sd_memory.o \
	$(LOCAL_DIR)/sd_hw.o \
	$(LOCAL_DIR)/sd_bus.o \
	$(LOCAL_DIR)/sd_slot.o \
	$(LOCAL_DIR)/sd_update.o \
	$(LOCAL_DIR)/fwdn_sdmmc.o \
	$(LOCAL_DIR)/update.o
endif

ifeq ($(TCC_SPLASH_USE), 1)

OBJS += \
	$(LOCAL_DIR)/splashimg.o

ifeq ($(DAUDIO),1)
OBJS += \
	$(LOCAL_DIR)/bitmap.o \
	$(LOCAL_DIR)/daudio_settings.o
endif

ifeq ($(DAUDIO_VER),1)
OBJS += \
	$(LOCAL_DIR)/daudio_ver.o
endif

endif

#ifeq ($(PLATFORM), tcc896x)
#OBJS += $(LOCAL_DIR)/nand_drv_dummy.o
#else
include ../../../hardware/telechips/nand_v8/Makefile
#endif

include platform/tcc_shared/tools/Makefile
include platform/tcc_shared/tcsb/Makefile
include platform/tcc_shared/mkimg/Makefile
