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

#ifndef _POWER_HINT_
#define _POWER_HINT_

#include <string>
#include <utils/RefBase.h>

#include <vendor/sprd/hardware/power/4.0/IPower.h>
#include <vendor/sprd/hardware/power/4.0/types.h>

using IBase = ::android::hidl::base::V1_0::IBase;
using IPower = ::vendor::sprd::hardware::power::V4_0::IPower;
using PowerHintVendor = ::vendor::sprd::hardware::power::V4_0::PowerHint;
using std::string;

namespace android {

class PowerHALManager;

class PowerHintScene: public RefBase {
public:
    PowerHintScene(PowerHALManager* pm, string tag, int sceneId, string sceneName);
    void acquire();
    void release();

private:
    sp<PowerHALManager> mPm;
    sp<IPower>& mIPower;
    string mTag;
    int mSceneId;
    string mSceneName;
    sp<IBase> mLock;
};

class PowerHALManager: public hardware::hidl_death_recipient {
public:
    PowerHALManager();
    void init();
    void deinit();
    friend class PowerHintScene;
    PowerHintScene* createPowerHintScene(string tag, int sceneId, string sceneName);
    void serviceDied(uint64_t cookie,
        const ::android::wp<::android::hidl::base::V1_0::IBase>& who) override;
    void enable();
    void disable();

private:
    sp<IPower> mIPower;
};

} //namespace android
#endif
