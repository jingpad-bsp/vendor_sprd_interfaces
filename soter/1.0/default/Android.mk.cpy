#only needed on treble version
ifeq ($(strip $(BOARD_SOTER_TRUSTY)), treble)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := vendor.sprd.hardware.soter@1.0-service
LOCAL_INIT_RC := vendor.sprd.hardware.soter@1.0-service.rc
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := \
    Soter.cpp \
    service.cpp \

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libhidlbase \
    libhidltransport \
    libhardware \
    libutils \
    libsoter_trusty \
    vendor.sprd.hardware.soter@1.0 \

LOCAL_SANITIZE := cfi
LOCAL_SANITIZE_DIAG := cfi

include $(BUILD_EXECUTABLE)

endif
