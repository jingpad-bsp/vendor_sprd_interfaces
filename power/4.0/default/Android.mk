# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.sprd.hardware.power@4.0-impl
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_C_INCLUDES := \
    vendor/sprd/modules/power \
    hardware/libhardware/include

LOCAL_SRC_FILES := \
    Power.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libhidlbase \
    libhidltransport \
    liblog \
    libutils \
    libhwbinder \
    android.hardware.power@1.0 \
    android.hardware.power@1.1 \
    android.hardware.power@1.2 \
    android.hardware.power@1.3 \
    vendor.sprd.hardware.power@4.0

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/utils/health/2.0

LOCAL_SRC_FILES += \
    utils/health/2.0/HealthInfoCallback.cpp

LOCAL_SHARED_LIBRARIES += \
    android.hardware.health@2.0

$(warning $(BOARD_POWERHINT_HAL))
ifneq ($(BOARD_POWERHINT_HAL),)
LOCAL_CFLAGS := -DBOARD_POWERHINT_HAL
LOCAL_SHARED_LIBRARIES += power.sprd
endif

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.sprd.hardware.power@4.0-service
LOCAL_INIT_RC := vendor.sprd.hardware.power@4.0-service.rc
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_C_INCLUDES := \
    vendor/sprd/modules/power \
    hardware/libhardware/include

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES:= service.cpp

LOCAL_SHARED_LIBRARIES := \
    libhidlbase \
    libhidltransport \
    libbase \
    liblog \
    libutils \
    vendor.sprd.hardware.power@4.0 \

include $(BUILD_EXECUTABLE)
