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

#include "power_hal_cli.h"

#define LOG_TAG "PowerHALCli"
#include <log/log.h>

namespace android {

PowerHintScene::PowerHintScene(PowerHALManager* pm, string tag, int sceneId
    , string sceneName): mIPower(pm->mIPower) {
    mPm = pm;
    mTag = tag;
    mSceneId = sceneId;
    mSceneName = sceneName;
    mLock = new IBase();
}

void PowerHintScene::acquire() {
    if (mIPower == nullptr) {
        mIPower = IPower::getService();
        if (mIPower != nullptr) {
            mIPower->linkToDeath(mPm, 0);
        }
    }

    if (mIPower == nullptr) {
        ALOGD("mIPower is nullptr");
        return;
    }

    if (mSceneId != 0) {
        mIPower->acquirePowerHintBySceneId(mLock, mTag, mSceneId);
    } else {
        mIPower->acquirePowerHintBySceneName(mLock, mTag, mSceneName);
    }
}

void PowerHintScene::release() {
    if (mIPower == nullptr) {
        mIPower = IPower::getService();
        if (mIPower != nullptr) {
            mIPower->linkToDeath(mPm, 0);
        }
    }

    if (mIPower == nullptr) {
        ALOGD("mIPower is nullptr");
        return;
    }

    if (mSceneId != 0) {
        mIPower->releasePowerHintBySceneId(mLock, mSceneId);
    } else {
        mIPower->releasePowerHintBySceneName(mLock, mSceneName);
    }
}

PowerHALManager::PowerHALManager() {
}

void PowerHALManager::init() {
    mIPower = IPower::getService();
    if (mIPower != nullptr) {
        mIPower->linkToDeath(this, 0);
    }
}

void PowerHALManager::deinit() {
    if (mIPower != nullptr) {
        mIPower->unlinkToDeath(this);
    }
}

PowerHintScene* PowerHALManager::createPowerHintScene(string tag, int sceneId, string sceneName) {
    if (mIPower == nullptr) {
        mIPower = IPower::getService();
        if (mIPower != nullptr) {
            mIPower->linkToDeath(this, 0);
        }
    }

    return new PowerHintScene(this, tag, sceneId, sceneName);
}

void PowerHALManager::serviceDied(uint64_t cookie,
    const ::android::wp<::android::hidl::base::V1_0::IBase>& who) {
    ALOGD("Enter %s", __func__);
    mIPower = nullptr;
}

void PowerHALManager::enable() {
    if (mIPower == nullptr) {
        mIPower = IPower::getService();
        if (mIPower != nullptr) {
            mIPower->linkToDeath(this, 0);
        }
    }

    mIPower->controlPowerHint(1);
}

void PowerHALManager::disable() {
    if (mIPower == nullptr) {
        mIPower = IPower::getService();
        if (mIPower != nullptr) {
            mIPower->linkToDeath(this, 0);
        }
    }

    mIPower->controlPowerHint(0);
}

} //namespace android
