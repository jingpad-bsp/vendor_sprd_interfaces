LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.sprd.hardware.gnss@1.0-impl
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := \
    ThreadCreationWrapper.cpp \
    AGnss.cpp \
    AGnssRilExt.cpp \
    GnssExt.cpp \
    GnssBatching.cpp \
    GnssCallback.cpp \
    GnssGeofenceCallback.cpp \
    GnssMeasurementCallback.cpp \
    GnssNavigationMessageCallback.cpp \
    GnssNiCallback.cpp \
    GnssXtraCallback.cpp \
    GnssDebug.cpp \
    GnssGeofencing.cpp \
    GnssMeasurement.cpp \
    GnssNavigationMessage.cpp \
    GnssNi.cpp \
    GnssXtra.cpp \
    GnssConfiguration.cpp \
    GnssUtils.cpp

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libhidlbase \
    libhidltransport \
    libutils \
    android.hardware.gnss@1.0 \
    vendor.sprd.hardware.gnss@1.0 \
    libhardware

#LOCAL_CFLAGS += -Werror

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE := vendor.sprd.hardware.gnss@1.0-service
LOCAL_INIT_RC := vendor.sprd.hardware.gnss@1.0-service.rc
LOCAL_SRC_FILES := \
    service.cpp \

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libdl \
    libbase \
    libutils \
    libhardware \
    libbinder \

LOCAL_SHARED_LIBRARIES += \
    libhidlbase \
    libhidltransport \
    vendor.sprd.hardware.gnss@1.0 \

include $(BUILD_EXECUTABLE)
