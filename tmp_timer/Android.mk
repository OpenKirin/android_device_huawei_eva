LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := tmp_timer.c
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_MODULE := tmp_timer
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)/sbin
LOCAL_CFLAGS := -Werror
include $(BUILD_EXECUTABLE)
