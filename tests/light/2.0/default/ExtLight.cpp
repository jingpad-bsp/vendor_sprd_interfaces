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
#define LOG_TAG "extlight"

#include <log/log.h>
#include "ExtLight.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace tests {
namespace light {
namespace V2_0 {
namespace implementation {


ExtLight::ExtLight(std::map<Type, light_device_t*> &&lights)
  : mExtLights(std::move(lights)) {}


// Methods from ::android::hardware::light::V2_0::ILight follow.
Return<Status> ExtLight::setLight(Type type, const LightState& state)  {
    auto it = mExtLights.find(type);

    if (it == mExtLights.end()) {
        return Status::LIGHT_NOT_SUPPORTED;
    }

    light_device_t* hwLight = it->second;

    light_state_t legacyState {
        .color = state.color,
        .flashMode = static_cast<int>(state.flashMode),
        .flashOnMS = state.flashOnMs,
        .flashOffMS = state.flashOffMs,
        .brightnessMode = static_cast<int>(state.brightnessMode),
    };

    int ret = hwLight->set_light(hwLight, &legacyState);

    switch (ret) {
        case -ENOSYS:
            return Status::BRIGHTNESS_NOT_SUPPORTED;
        case 0:
            return Status::SUCCESS;
        default:
            return Status::UNKNOWN;
    }
}

Return<void> ExtLight::getSupportedTypes(getSupportedTypes_cb _hidl_cb)  {
    Type *types = new Type[mExtLights.size()];

    int idx = 0;
    for(auto const &pair : mExtLights) {
        Type type = pair.first;

        types[idx++] = type;
    }

    {
        hidl_vec<Type> hidl_types{};
        hidl_types.setToExternal(types, mExtLights.size());

        _hidl_cb(hidl_types);
    }

    delete[] types;

    return Void();
}

const static std::map<Type, const char*> kLogicalLights = {
    {Type::BACKLIGHT,     LIGHT_ID_BACKLIGHT},
    {Type::KEYBOARD,      LIGHT_ID_KEYBOARD},
    {Type::BUTTONS,       LIGHT_ID_BUTTONS},
    {Type::BATTERY,       LIGHT_ID_BATTERY},
    {Type::NOTIFICATIONS, LIGHT_ID_NOTIFICATIONS},
    {Type::ATTENTION,     LIGHT_ID_ATTENTION},
    {Type::BLUETOOTH,     LIGHT_ID_BLUETOOTH},
    {Type::WIFI,          LIGHT_ID_WIFI}
};

light_device_t* getLightDevice(const char* name) {
    light_device_t* lightDevice;
    const hw_module_t* hwModule = NULL;

    int ret = hw_get_module (LIGHTS_HARDWARE_MODULE_ID, &hwModule);
    if (ret == 0) {
        ret = hwModule->methods->open(hwModule, name,
            reinterpret_cast<hw_device_t**>(&lightDevice));
        if (ret != 0) {
            ALOGE("light_open %s %s failed: %d", LIGHTS_HARDWARE_MODULE_ID, name, ret);
        }
    } else {
        ALOGE("hw_get_module %s %s failed: %d", LIGHTS_HARDWARE_MODULE_ID, name, ret);
    }

    if (ret == 0) {
        return lightDevice;
    } else {
        ALOGE("Light passthrough failed to load legacy HAL.");
        return nullptr;
    }
}

IExtLight* HIDL_FETCH_IExtLight(const char* /* name */) {
    std::map<Type, light_device_t*> lights;

    for(auto const &pair : kLogicalLights) {
        Type type = pair.first;
        const char* name = pair.second;

        light_device_t* light = getLightDevice(name);

        if (light != nullptr) {
            lights[type] = light;
        }
    }

    if (lights.size() == 0) {
        // Log information, but still return new Light.
        // Some devices may not have any lights.
        ALOGI("Could not open any lights.");
    }

    return new ExtLight(std::move(lights));
}


// Methods from ::android::hardware::example::extension::light::V2_0::ILight follow.
Return<Status> ExtLight::setExtLight(Type /* type */,
                                  const ExtLightState& /* state */)  {

    // ******************************************************
    // Note: awesome proprietary hardware implementation here
    // ******************************************************

    return Status::SUCCESS;
}

} // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace tests
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
