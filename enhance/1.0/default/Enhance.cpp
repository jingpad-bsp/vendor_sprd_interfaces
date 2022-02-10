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

#define LOG_TAG "enhance"

#include <log/log.h>

#include "Enhance.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace enhance {
namespace V1_0 {
namespace implementation {

Enhance::Enhance(std::map<Type, enhance_device_t*> &&deviceMap)
  : mDeviceMap(std::move(deviceMap)) {}

// Methods from ::vendor::sprd::hardware::enhance::V1_0::IEnhance follow.
Return<Status> Enhance::setValue(Type type, int32_t val) {
    auto it = mDeviceMap.find(type);

    if (it == mDeviceMap.end()) {
        return Status::FAILED;
    }

    enhance_device_t* enhance = it->second;

    int ret = enhance->set_value(val);

    return ret ? Status::FAILED : Status::SUCCESS;
}

Return<Status> Enhance::setMode(Type type, int32_t mode) {
    auto it = mDeviceMap.find(type);

    if (it == mDeviceMap.end()) {
        return Status::FAILED;
    }

    enhance_device_t* enhance = it->second;

    int ret = enhance->set_mode(mode);

    return ret ? Status::FAILED : Status::SUCCESS;
}

const static std::map<Type, const char*> kLogicalEnhances = {
    {Type::CMS,		PQ_ID_CMS},
    {Type::SLP,		PQ_ID_SLP},
    {Type::BLP,		PQ_ID_BLP},
    {Type::GAM,		PQ_ID_GAM},
    {Type::CABC,	PQ_ID_CABC},
};

static enhance_device_t* getEnhanceDevice(const char* name) {
    enhance_device_t* enhance;
    const hw_module_t* module = NULL;

    int ret = hw_get_module(ENHANCE_HARDWARE_MODULE_ID, &module);
    if (ret == 0) {
        ret = module->methods->open(module, name, reinterpret_cast<hw_device_t**>(&enhance));
        if (ret != 0) {
            ALOGE("%s module open %s device failed: %d", ENHANCE_HARDWARE_MODULE_ID, name, ret);
        }
    } else {
        ALOGE("hw_get_module %s %s failed: %d", ENHANCE_HARDWARE_MODULE_ID, name, ret);
    }

    if (ret == 0) {
        return enhance;
    } else {
        ALOGE("Enhance passthrough failed to load legacy HAL.");
        return nullptr;
    }
}

IEnhance* HIDL_FETCH_IEnhance(const char* /* name */) {
    std::map<Type, enhance_device_t*> deviceMap;

    for(auto const &pair : kLogicalEnhances) {
        Type type = pair.first;
        const char* name = pair.second;

        enhance_device_t* enhance = getEnhanceDevice(name);

        if (enhance != nullptr) {
            deviceMap[type] = enhance;
        }
    }

    if (deviceMap.size() == 0) {
        // Log information, but still return new Enhance.
        // Some devices may not have any enhances.
        ALOGI("Could not open any enhance devices.");
    }

    return new Enhance(std::move(deviceMap));
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace enhance
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
