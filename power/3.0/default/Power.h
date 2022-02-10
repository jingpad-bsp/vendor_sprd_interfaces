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

#ifndef VENDOR_SPRD_HARDWARE_POWER_V3_0_POWER_H
#define VENDOR_SPRD_HARDWARE_POWER_V3_0_POWER_H

#include <vendor/sprd/hardware/power/3.0/IPower.h>
#include <vendor/sprd/hardware/power/3.0/types.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include <unordered_map>
#include <hwbinder/BpHwBinder.h>

//For Health
#include <android/hardware/health/2.0/IHealth.h>
#include <android/hardware/health/2.0/types.h>
#include <HealthInfoCallback.h>

#include <sprd_power.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace power {
namespace V3_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

using ::android::hardware::hidl_death_recipient;
using ::android::hardware::power::V1_0::PowerStatePlatformSleepState;
using ::android::hardware::power::V1_0::Status;
using ::android::hardware::power::V1_1::PowerStateSubsystem;
using ::vendor::sprd::hardware::power::V3_0::DebugFlag;
using ::android::hardware::BpHwBinder;
using std::unordered_map;

//For Health
using ::android::hardware::health::V2_0::IHealth;
using ::android::hardware::health::V2_0::IHealthInfoCallback;
using ::android::hardware::health::V2_0::implementation::HealthInfoCallback;

struct PowerRecipientForHealth: public hidl_death_recipient {
    void serviceDied(uint64_t cookie,
        const ::android::wp<::android::hidl::base::V1_0::IBase>& who) override;
};

struct PowerHintRequest;

struct Power : public IPower {
    // Methods from ::android::hardware::power::V1_0::IPower follow.
    Return<void> setInteractive(bool interactive) override;
    Return<void> powerHint(::android::hardware::power::V1_0::PowerHint hint, int32_t data) override;
    Return<void> setFeature(::android::hardware::power::V1_0::Feature feature, bool activate) override;
    Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) override;

    // Methods from ::android::hardware::power::V1_1::IPower follow.
    Return<void> getSubsystemLowPowerStats(getSubsystemLowPowerStats_cb _hidl_cb) override;
    Return<void> powerHintAsync(::android::hardware::power::V1_0::PowerHint hint, int32_t data) override;

    // Methods from ::android::hardware::power::V1_2::IPower follow.
    Return<void> powerHintAsync_1_2(::android::hardware::power::V1_2::PowerHint hint, int32_t data) override;

    // Methods from ::vendor::sprd::hardware::power::V3_0::IPower follow.
    Return<void> acquirePowerHintBySceneId(const sp<::android::hidl::base::V1_0::IBase>& lock, const hidl_string& tag, int32_t sceneId) override;
    Return<void> releasePowerHintBySceneId(const sp<::android::hidl::base::V1_0::IBase>& lock, int32_t sceneId) override;
    Return<void> acquirePowerHintBySceneName(const sp<::android::hidl::base::V1_0::IBase>& lock, const hidl_string& tag, const hidl_string& sceneName) override;
    Return<void> releasePowerHintBySceneName(const sp<::android::hidl::base::V1_0::IBase>& lock, const hidl_string& sceneName) override;
    Return<int32_t> getSceneIdByName(const hidl_string& sceneName) override;
    Return<void> controlPowerHint(int32_t enable) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

    Power();
    ~Power();

    void showPowerHintRequestMap();

    struct PowerHintRequest: public hidl_death_recipient {
    public:
        PowerHintRequest(sp<::android::hidl::base::V1_0::IBase> lock, hidl_string tag
            , int32_t sceneId, IPower *power): mLock(lock), mPower(power) {
            mSceneId = sceneId;
            mTag = tag;
        }

        void serviceDied(uint64_t cookie,
            const ::android::wp<::android::hidl::base::V1_0::IBase>& who) {
            ALOGD_IF("Enter %s: " ,__func__);
            mPower->releasePowerHintBySceneId(mLock, mSceneId);
        }

        sp<::android::hidl::base::V1_0::IBase> mLock;
        int32_t mSceneId;
        hidl_string mTag;
        sp<IPower> mPower;
    };

private:
    struct sprd_power_module *mModule;

    unordered_map<int32_t, unordered_map<int32_t, sp<PowerHintRequest>>*> mPowerHintRequests{};
    std::mutex mPowerHintRequestsLock;
    int32_t mDebugFlags;
    int32_t mCounterNewMap;

    // For health callback
    bool mHealthCallbackRegistered = false;
    sp<IHealth> mIHealth;
    sp<IHealthInfoCallback> mIHealthInfoCallback;
    sp<PowerRecipientForHealth> mHealthRecipient;
};

// FIXME: most likely delete, this is only for passthrough implementations
extern "C" IPower* HIDL_FETCH_IPower(const char* name);

}  // namespace implementation
}  // namespace V3_0
}  // namespace power
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_POWER_V3_0_POWER_H
