LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/include/dev

OBJS += \
	$(LOCAL_DIR)/sensor_if.o \
	$(LOCAL_DIR)/daudio_ver.o

#Selece video decoder
ifeq ($(SELECTED_DECODER), adv7182)
OBJS += \
	$(LOCAL_DIR)/adv7182.o
endif
ifeq ($(SELECTED_DECODER), tvp5150)
OBJS += \
	$(LOCAL_DIR)/tvp5150.o
endif
ifeq ($(SELECTED_DECODER), tw9912)
OBJS += \
	$(LOCAL_DIR)/tw9912.o
endif
ifeq ($(SELECTED_DECODER), tw9990)
OBJS += \
	$(LOCAL_DIR)/tw9990.o \
	$(LOCAL_DIR)/lvds.o
DEFINES += CONFIG_SUPPORT_LVDS
endif
ifeq ($(SELECTED_DECODER), )
OBJS += \
	$(LOCAL_DIR)/tw9921.o \
	$(LOCAL_DIR)/lvds.o
endif

