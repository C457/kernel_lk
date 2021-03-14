#==================================================================
# objs
#==================================================================
LOCAL_DIR := $(GET_LOCAL_DIR)

OBJS += \
	$(LOCAL_DIR)/daudio_test.o \

include $(LOCAL_DIR)/mkimg/Makefile

