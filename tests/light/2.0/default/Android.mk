LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.sprd.hardware.tests.light@2.0-service
LOCAL_INIT_RC := vendor.sprd.hardware.tests.light@2.0-service.rc
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := \
    ExtLight.cpp \
    service.cpp

LOCAL_SHARED_LIBRARIES := \
    libhidlbase \
    libhidltransport \
    libutils \
    liblog \
    libcutils \
    libhardware \
    libbase \
    libcutils \
    android.hardware.light@2.0 \
    libhidlbase \
    libhidltransport \
    vendor.sprd.hardware.tests.light@2.0 \

include $(BUILD_EXECUTABLE)
