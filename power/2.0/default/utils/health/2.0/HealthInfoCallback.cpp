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

#define LOG_TAG "Power-HealthInfoCallback"
#include <log/log.h>
#include "HealthInfoCallback.h"

extern "C" struct sprd_power_module power_impl;

namespace android {
namespace hardware {
namespace health {
namespace V2_0 {
namespace implementation {

// Methods from ::android::hardware::health::V2_0::IHealthInfoCallback follow.
Return<void> HealthInfoCallback::healthInfoChanged(const ::android::hardware::health::V2_0::HealthInfo& info) {
    // TODO implement


    if (info.legacy.chargerAcOnline || info.legacy.chargerUsbOnline
        || info.legacy.chargerWirelessOnline) {
        power_impl.isCharging = 1;
    } else {
        power_impl.isCharging = 0;
    }

    ALOGD("Update charge status to: %d", power_impl.isCharging);
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IHealthInfoCallback* HIDL_FETCH_IHealthInfoCallback(const char* /* name */) {
//    return new HealthInfoCallback();
//}

}  // namespace implementation
}  // namespace V2_0
}  // namespace health
}  // namespace hardware
}  // namespace android
