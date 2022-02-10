/*
 * Copyright (C) 2016 The Android Open Source Project
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
#ifndef VENDOR_SPRD_HARDWARE_TESTS_LIGHT_V2_0_LIGHT_H
#define VENDOR_SPRD_HARDWARE_TESTS_LIGHT_V2_0_LIGHT_H

#include <vendor/sprd/hardware/tests/light/2.0/IExtLight.h>
#include <hardware/hardware.h>
#include <hardware/lights.h>
#include <hidl/Status.h>

#include <hidl/MQDescriptor.h>
#include <map>

namespace vendor {
namespace sprd {
namespace hardware {
namespace tests {
namespace light {
namespace V2_0 {
namespace implementation {

using ::vendor::sprd::hardware::tests::light::V2_0::ExtLightState;
using ::vendor::sprd::hardware::tests::light::V2_0::IExtLight;
using ::android::hardware::light::V2_0::ILight;
using ::android::hardware::light::V2_0::LightState;
using ::android::hardware::light::V2_0::Status;
using ::android::hardware::light::V2_0::Type;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

struct ExtLight : public IExtLight {
    ExtLight(std::map<Type, light_device_t*> &&lights);
    // Methods from ::android::hardware::light::V2_0::ILight follow.
    Return<Status> setLight(Type type, const LightState& state)  override;
    Return<void> getSupportedTypes(getSupportedTypes_cb _hidl_cb)  override;

    // Methods from ::android::hardware::example::extension::light::V2_0::ILight follow.
    Return<Status> setExtLight(Type type, const ExtLightState& state)  override;

private:
    std::map<Type, light_device_t*> mExtLights;
};

extern "C" IExtLight* HIDL_FETCH_IExtLight(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace tests
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_TESTS_LIGHT_V2_0_LIGHT_H

