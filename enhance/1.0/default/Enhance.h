/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef VENDOR_SPRD_HARDWARE_ENHANCE_V1_0_ENHANCE_H
#define VENDOR_SPRD_HARDWARE_ENHANCE_V1_0_ENHANCE_H

#include <vendor/sprd/hardware/enhance/1.0/IEnhance.h>
#include <hardware/hardware.h>
#include <hardware/enhance.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <map>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "enhanceHIDL"

namespace vendor {
namespace sprd {
namespace hardware {
namespace enhance {
namespace V1_0 {
namespace implementation {

using ::vendor::sprd::hardware::enhance::V1_0::IEnhance;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Enhance : public IEnhance {
    Enhance(std::map<Type, enhance_device_t*> &&deviceMap);

    // Methods from IEnhance follow.
    Return<Status> setValue(Type type, int32_t val)  override;
    Return<Status> setMode(Type type, int32_t mode)  override;

private:
    std::map<Type, enhance_device_t*> mDeviceMap;
};

extern "C" IEnhance* HIDL_FETCH_IEnhance(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace enhance
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_ENHANCE_V1_0_ENHANCE_H
