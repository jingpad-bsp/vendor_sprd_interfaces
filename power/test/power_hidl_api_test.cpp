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

#define LOG_TAG PowerHALTest

#include <utils/Log.h>
#include <string>
#include <iostream>

#include <vendor/sprd/hardware/power/4.0/IPower.h>

using ::vendor::sprd::hardware::power::V4_0::IPower;
using ::android::hidl::base::V1_0::IBase;

int main() {
    ::android::sp<::vendor::sprd::hardware::power::V4_0::IPower> gPowerHal = IPower::getService();
    std::string sceneName;
    int32_t sceneId = 0;
    int32_t enable = 0;
    int32_t request = 0;
    ::android::sp<IBase> lock1 = new IBase();
    ::android::sp<IBase> lock2 = new IBase();

    std::cout << "Pls enter the scene name:" << std::endl;
    std::cin >> sceneName;

    if (gPowerHal == nullptr) {
        std::cout << "gPowerHal is nullptr!!!" << std::endl;
    }

    std::cout << "getSceneIdByName: " << sceneName << std::endl;
    sceneId = gPowerHal->getSceneIdByName(sceneName);
    std::cout << "sceneName:" << sceneName << "--->id: " << sceneId << std::endl;

    std::cout << "Request sceneId:" << sceneId << std::endl;
    gPowerHal->acquirePowerHintBySceneId(lock1, sceneName, sceneId);
    gPowerHal->acquirePowerHintBySceneId(lock2, sceneName, sceneId);
    sleep(2);
    std::cout << "Release sceneId:" << sceneId << std::endl;
    gPowerHal->releasePowerHintBySceneId(lock1, sceneId);
    gPowerHal->releasePowerHintBySceneId(lock2, sceneId);

    std::cout << "Request sceneName:" << sceneName << std::endl;
    gPowerHal->acquirePowerHintBySceneName(lock1, sceneName, sceneName);
    gPowerHal->acquirePowerHintBySceneName(lock2, sceneName, sceneName);
    sleep(2);
    std::cout << "Release sceneName:" << sceneName << std::endl;
    gPowerHal->releasePowerHintBySceneName(lock1, sceneName);
    gPowerHal->releasePowerHintBySceneName(lock2, sceneName);

    while (1) {
        std::cout << "Pls Enter: <sceneId> <request/release: 1/0> <enable/disable: 1/0>" << std::endl;
        std::cin >> sceneId >> request >> enable;
        if (enable) {
            std::cout << "PowerHint enable"<< std::endl;
            gPowerHal->controlPowerHint(1);
        } else {
            std::cout << "PowerHint disable"<< std::endl;
            gPowerHal->controlPowerHint(0);
        }

        if (request) {
            std::cout << "Request sceneId:" << sceneId << std::endl;
            gPowerHal->acquirePowerHintBySceneId(lock1, "aa", sceneId);
        } else {
            std::cout << "Release sceneId:" << sceneId << std::endl;
            gPowerHal->releasePowerHintBySceneId(lock1, sceneId);
        }
    }

    return 0;
}
