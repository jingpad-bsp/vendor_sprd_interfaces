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

#define LOG_TAG "vendor.sprd.hardware.fingerprintmmi@1.0-service"

#include <android/log.h>
#include <vendor/sprd/hardware/fingerprintmmi/1.0/IFingerprintmmi.h>
#include <hidl/LegacySupport.h>
#include <hidl/HidlTransportSupport.h>

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::hardware::registerPassthroughServiceImplementation;

using vendor::sprd::hardware::fingerprintmmi::V1_0::IFingerprintmmi;

int main() {
    ALOGD("Enter fingerprintmmi HAL...\n");
    //return defaultPassthroughServiceImplementation<IFingerprintmmi>();

    configureRpcThreadpool(2, true /* callerWillJoin */);
    android::status_t status = registerPassthroughServiceImplementation<IFingerprintmmi>();
    LOG_ALWAYS_FATAL_IF(
        status != android::OK,
        "Error while registering Fingerprintmmi service: %d", status);
    joinRpcThreadpool();
    return 0;
}
