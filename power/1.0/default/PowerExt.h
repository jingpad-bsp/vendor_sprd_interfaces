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

#ifndef VENDOR_SPRD_HARDWARE_POWER_V1_0_POWEREXT_H
#define VENDOR_SPRD_HARDWARE_POWER_V1_0_POWEREXT_H

#include <vendor/sprd/hardware/power/1.0/IPowerExt.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::IPower;
using ::android::hardware::power::V1_0::PowerHint;
using ::android::hardware::power::V1_0::PowerStatePlatformSleepState;
using ::android::hardware::power::V1_0::Status;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::sprd::hardware::power::V1_0::IPowerExt;
using ::vendor::sprd::hardware::power::V1_0::PowerHintExt;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct PowerExt : public IPowerExt {
    PowerExt(power_module_t* module);
    ~PowerExt();

    // Methods from ::android::hardware::power::V1_0::IPower follow.
    Return<void> setInteractive(bool interactive) override;
    Return<void> powerHint(PowerHint hint, int32_t data) override;
    Return<void> setFeature(Feature feature, bool activate) override;
    Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) override;

    // Methods from ::vendor::sprd::hardware::power::V1_0::IPowerExt follow.
    Return<void> powerHintExt(PowerHintExt hint, int32_t data) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

  private:
    power_module_t* mModule;

};

extern "C" IPowerExt* HIDL_FETCH_IPowerExt(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_POWER_V1_0_POWEREXT_H
