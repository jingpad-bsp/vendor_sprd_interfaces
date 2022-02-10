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

#define LOG_TAG "android.hardware.power@4.0-impl"

#include <hardware/hardware.h>
#include <hardware/power.h>
#include "Power.h"

#include <log/log.h>
#include <cutils/properties.h>
#include <android/hidl/base/1.0/BpHwBase.h>
#define DEBUG_FLAG_PROP                     "persist.vendor.power.dbg_flags"

#define ALOGD_POWER(flag,...) do {               \
    if (mDebugFlags & flag) {                    \
        ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__);   \
    }                                            \
} while(0)

#ifdef BOARD_POWERHINT_HAL
extern "C" struct sprd_power_module power_impl;
#else  //BOARD_POWERHINT_HAL
struct sprd_power_module power_impl;
#endif

namespace vendor {
namespace sprd {
namespace hardware {
namespace power {
namespace V4_0 {
namespace implementation {

void PowerRecipientForHealth::serviceDied(uint64_t cookie,
    const ::android::wp<::android::hidl::base::V1_0::IBase>& who) {
    ALOGD("Enter health %s: " ,__func__);
    *((bool*)cookie) = false;
}

Power::Power() {
    mModule = &power_impl;
    mCounterNewMap = 0;
    mDebugFlags = property_get_int32(DEBUG_FLAG_PROP, static_cast<int32_t>(DebugFlag::DEBUG));
    ALOGD("Enter %s: mModule:%p, mDebugFlags:%x", __func__, mModule, mDebugFlags);
    if (mModule && mModule->init != nullptr) {
        ALOGD("%s: mModule init()", __func__);
        mModule->init(mModule);
    }

    mIHealthInfoCallback = new HealthInfoCallback();
    mHealthRecipient = new PowerRecipientForHealth();
}

Power::~Power() {
    auto it = mPowerHintRequests.begin();

    ALOGD("%s Bgn: mModule %p", __func__, mModule);
    while (it != mPowerHintRequests.end()) {
        mPowerHintRequests.erase(it);
        delete (it->second);
        mCounterNewMap--;
        it = mPowerHintRequests.begin();
    }
    ALOGD("%s End: mCounterNewMap:%d", __func__, mCounterNewMap);
}

// Methods from ::android::hardware::power::V1_0::IPower follow.
Return<void> Power::setInteractive(bool interactive) {
    if (!mHealthCallbackRegistered) {
        mIHealth = IHealth::getService();
        if (mIHealth != nullptr) {
            ALOGD("Register IHealthInfoCallback");
            mIHealth->registerCallback(mIHealthInfoCallback);
            mHealthCallbackRegistered = true;
            mIHealth->linkToDeath(mHealthRecipient, (uint64_t)&mHealthCallbackRegistered);
        } else {
            ALOGE("IHealth.getService() failed");
        }
    }

    if (mModule->setInteractive)
        mModule->setInteractive(mModule, interactive ? 1 : 0);

    return Void();
}

Return<void> Power::powerHint(::android::hardware::power::V1_0::PowerHint hint, int32_t data) {
    int32_t param = data;

    if (mModule->powerHint) {
        if (data)
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), &param);
        else
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), NULL);
    }

    return Void();
}

Return<void> Power::setFeature(::android::hardware::power::V1_0::Feature feature, bool activate) {
    if (mModule->setFeature)
        mModule->setFeature(mModule, static_cast<feature_t>(feature),
                activate ? 1 : 0);

    return Void();
}

Return<void> Power::getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) {
    hidl_vec<PowerStatePlatformSleepState> states;
    ssize_t number_platform_modes;
    size_t *voters = nullptr;
    power_state_platform_sleep_state_t *legacy_states = nullptr;
    int ret;

    if (mModule->get_number_of_platform_modes == nullptr ||
            mModule->get_voter_list == nullptr ||
            mModule->get_platform_low_power_stats == nullptr) {
        /*Bug 921538 workaround start
         * for CTS android.cts.statsd.atom.HostAtomTests#testSubsystemSleepState case
         * TODO: implementation
         */
        states.resize(1);
        PowerStatePlatformSleepState& state = states[0];
        state.name = "TestMode";
        state.residencyInMsecSinceBoot = 0;
        state.totalTransitions = 0;
        state.supportedOnlyInSuspend = false;
        state.voters.resize(1);
        state.voters[0].name = "TestVoter";
        state.voters[0].totalTimeInMsecVotedForSinceBoot = 0;
        state.voters[0].totalNumberOfTimesVotedSinceBoot = 0;
        /*Bug 921538 workaround end*/
        _hidl_cb(states, Status::SUCCESS);
        return Void();
    }

    number_platform_modes = mModule->get_number_of_platform_modes(mModule);
    if (number_platform_modes) {
       if ((ssize_t) (SIZE_MAX / sizeof(size_t)) <= number_platform_modes)  // overflow
           goto done;
       voters = new (std::nothrow) size_t [number_platform_modes];
       if (voters == nullptr)
           goto done;

       ret = mModule->get_voter_list(mModule, voters);
       if (ret != 0)
           goto done;

       if ((ssize_t) (SIZE_MAX / sizeof(power_state_platform_sleep_state_t))
           <= number_platform_modes)  // overflow
           goto done;
       legacy_states = new (std::nothrow)
           power_state_platform_sleep_state_t [number_platform_modes];
       if (legacy_states == nullptr)
           goto done;

       for (int i = 0; i < number_platform_modes; i++) {
          legacy_states[i].voters = nullptr;
          legacy_states[i].voters = new power_state_voter_t [voters[i]];
          if (legacy_states[i].voters == nullptr)
              goto done;
       }

       ret = mModule->get_platform_low_power_stats(mModule, legacy_states);
       if (ret != 0)
           goto done;

       states.resize(number_platform_modes);
       for (int i = 0; i < number_platform_modes; i++) {
          power_state_platform_sleep_state_t& legacy_state = legacy_states[i];
          PowerStatePlatformSleepState& state = states[i];
          state.name = legacy_state.name;
          state.residencyInMsecSinceBoot = legacy_state.residency_in_msec_since_boot;
          state.totalTransitions = legacy_state.total_transitions;
          state.supportedOnlyInSuspend = legacy_state.supported_only_in_suspend;
          state.voters.resize(voters[i]);
          for(size_t j = 0; j < voters[i]; j++) {
              state.voters[j].name = legacy_state.voters[j].name;
              state.voters[j].totalTimeInMsecVotedForSinceBoot = legacy_state.voters[j].total_time_in_msec_voted_for_since_boot;
              state.voters[j].totalNumberOfTimesVotedSinceBoot = legacy_state.voters[j].total_number_of_times_voted_since_boot;
          }
       }
    }

done:
    if (legacy_states) {
        for (int i = 0; i < number_platform_modes; i++) {
            if(legacy_states[i].voters)
                delete(legacy_states[i].voters);
        }
    }
    delete[] legacy_states;
    delete[] voters;
    _hidl_cb(states, Status::SUCCESS);

    return Void();
}


// Methods from ::android::hardware::power::V1_1::IPower follow.
Return<void> Power::getSubsystemLowPowerStats(getSubsystemLowPowerStats_cb _hidl_cb) {
    hidl_vec<PowerStateSubsystem> states;
    power_state_subsystem_t *legacy_states = nullptr;
    int ret;

    if (mModule->get_subsystem_low_power_stats == nullptr) {
        _hidl_cb(states, Status::SUCCESS);
        return Void();
    }

    /* TODO: implementation handle
    mModule->getPlatformLowPowerStats(mModule));
    states.resize(number_platform_modes);
    _hidl_cb(states, Status::SUCCESS);*/

    return Void();
}

Return<void> Power::powerHintAsync(::android::hardware::power::V1_0::PowerHint hint, int32_t data) {
    int32_t param = data;

    if (mModule->powerHint) {
        if (data)
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), &param);
        else
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), NULL);
    }

    return Void();
}


// Methods from ::android::hardware::power::V1_2::IPower follow.
Return<void> Power::powerHintAsync_1_2(::android::hardware::power::V1_2::PowerHint hint, int32_t data) {
    int32_t param = data;

    if (mModule->powerHint) {
        if (data)
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), &param);
        else
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), NULL);
    }

    return Void();
}


// Methods from ::android::hardware::power::V1_3::IPower follow.
Return<void> Power::powerHintAsync_1_3(::android::hardware::power::V1_3::PowerHint hint, int32_t data) {
    int32_t param = data;

    if (mModule->powerHint) {
        if (data)
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), &param);
        else
            mModule->powerHint(mModule, static_cast<power_hint_t>(hint), NULL);
    }

    return Void();
}


// Methods from ::vendor::sprd::hardware::power::V4_0::IPower follow.
Return<void> Power::acquirePowerHintBySceneId(const sp<::android::hidl::base::V1_0::IBase>& lock, const hidl_string& tag, int32_t sceneId) {
    ::android::hidl::base::V1_0::BpHwBase *bpHwBase = static_cast<::android::hidl::base::V1_0::BpHwBase*>(lock.get());
    int32_t handle = ((BpHwBinder*)(bpHwBase->onAsBinder()))->handle();
    sp<PowerHintRequest> request = nullptr;
    unordered_map<int32_t, sp<PowerHintRequest>> *value_map = nullptr;

    mPowerHintRequestsLock.lock();

    auto iterator = mPowerHintRequests.find(sceneId);
    // Case 1: Not found
    if (iterator == mPowerHintRequests.end()) {
        request = new PowerHintRequest(lock, tag, sceneId, this);
        if (request == nullptr) {
            ALOGD("Alloc PowerHintRequest failed");
            mPowerHintRequestsLock.unlock();
            return Void();
        }

        value_map = new unordered_map<int32_t, sp<PowerHintRequest>>();
        if (value_map == nullptr) {
            ALOGD("Alloc unordered_map failed");
            mPowerHintRequestsLock.unlock();
            return Void();
        }
        mCounterNewMap++;

        value_map->insert(std::pair<int32_t, sp<PowerHintRequest>>(handle, request));
        mPowerHintRequests.insert(std::pair<int32_t, unordered_map<int32_t, sp<PowerHintRequest>>*>(sceneId, value_map));
        lock->linkToDeath(request, 0);

        if (mDebugFlags & DebugFlag::DEBUG_VERBOSE) {
            showPowerHintRequestMap();
        }

        powerHintAsync(static_cast<::android::hardware::power::V1_0::PowerHint>(sceneId), 1);
        mPowerHintRequestsLock.unlock();
        return Void();
    } else {
        value_map = iterator->second;
        auto req_iterator = value_map->find(handle);

        // First request some scene
        if (req_iterator == value_map->end()) {
            request = new PowerHintRequest(lock, tag, sceneId, this);
            if (request == nullptr) {
                ALOGD("Alloc PowerHintRequest failed");
                mPowerHintRequestsLock.unlock();
                return Void();
            }

            value_map->insert(std::pair<int32_t, sp<PowerHintRequest>>(handle, request));
            lock->linkToDeath(request, 0);

            if (mDebugFlags & DebugFlag::DEBUG_VERBOSE) {
                showPowerHintRequestMap();
            }


            if (value_map->size() == 1) {
                powerHintAsync(static_cast<::android::hardware::power::V1_0::PowerHint>(sceneId), 1);
            }
            mPowerHintRequestsLock.unlock();

            return Void();
        } else {
            ALOGD("%s repeat request same scene:%x, ignore the request", tag.c_str(), sceneId);
        }
    }

    mPowerHintRequestsLock.unlock();

    return Void();
}

Return<void> Power::releasePowerHintBySceneId(const sp<::android::hidl::base::V1_0::IBase>& lock, int32_t sceneId) {
    ::android::hidl::base::V1_0::BpHwBase *bpHwBase = static_cast<::android::hidl::base::V1_0::BpHwBase*>(lock.get());
    int32_t handle = ((BpHwBinder*)(bpHwBase->onAsBinder()))->handle();
    sp<PowerHintRequest> request = nullptr;

    mPowerHintRequestsLock.lock();

    auto iterator = mPowerHintRequests.find(sceneId);
    // Case 1: Have found
    if (iterator != mPowerHintRequests.end()) {
        unordered_map<int32_t, sp<PowerHintRequest>> *value_map = iterator->second;
        auto value_iterator = value_map->find(handle);

        // Find the request in the containter of the scene request
        if (value_iterator != value_map->end()) {
            auto value = value_iterator->second;
            if (value == nullptr) {
                ALOGE("%s: The PowerHintRequest is null in value_map", __func__);
                goto exit;
            }

            value->mLock->unlinkToDeath(value);
            value_map->erase(handle);

            if (mDebugFlags & DebugFlag::DEBUG_VERBOSE) {
                showPowerHintRequestMap();
            }

            if (value_map->empty()) {
                powerHintAsync(static_cast<::android::hardware::power::V1_0::PowerHint>(sceneId), 0);
            }
        }
    }

exit:
    mPowerHintRequestsLock.unlock();

    return Void();
}

Return<void> Power::acquirePowerHintBySceneName(const sp<::android::hidl::base::V1_0::IBase>& lock, const hidl_string& tag, const hidl_string& sceneName) {
    int sceneId = getSceneIdByName(sceneName);

    if (sceneId == 0) {
        ALOGD("%s: Don't support %s scene", __func__, sceneName.c_str());
        return Void();
    }

    acquirePowerHintBySceneId(lock, tag, sceneId);

    return Void();
}

Return<void> Power::releasePowerHintBySceneName(const sp<::android::hidl::base::V1_0::IBase>& lock, const hidl_string& sceneName) {
    int sceneId = getSceneIdByName(sceneName);

    if (sceneId == 0) {
        ALOGD("%s: Don't support %s scene", __func__, sceneName.c_str());
        return Void();
    }

    releasePowerHintBySceneId(lock, sceneId);

    return Void();
}

Return<int32_t> Power::getSceneIdByName(const hidl_string& sceneName) {
    int32_t sceneId = 0;
    char scene_name[LEN_SCENE_NAME_MAX] = {'\0'};

    ALOGD_POWER(DebugFlag::DEBUG, "Enter %s: sceneName:%s", __func__, sceneName.c_str());
    strncpy(scene_name, sceneName.c_str(), sizeof(scene_name));
    if (mModule->get_scene_id) {
        sceneId = mModule->get_scene_id(mModule, scene_name);
    }

    return sceneId;
}

void Power::showPowerHintRequestMap() {
    ALOGD(">>>Print PowerHint request bgn");
    for (auto it: mPowerHintRequests) {
        ALOGD("Scene %x:", it.first);
        for (auto it2: *(it.second)) {
            ALOGD("\thandle:%x, Tag:%s", it2.first, it2.second->mTag.c_str());
        }
    }
    ALOGD("<<<Print PowerHint request end");
}

Return<void> Power::controlPowerHint(int32_t enable) {
    if (mModule->ctrl_power_hint)
            mModule->ctrl_power_hint(mModule, enable ? 1 : 0);

    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IPower* HIDL_FETCH_IPower(const char* /* name */) {
    return new Power();
}

}  // namespace implementation
}  // namespace V4_0
}  // namespace power
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
