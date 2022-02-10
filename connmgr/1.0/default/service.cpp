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
#define LOG_TAG "vendor.sprd.hardware.IConnmgr@1.0-service"
#include <android/log.h>
#include <vendor/sprd/hardware/connmgr/1.0/IConnmgr.h>
#include <vendor/sprd/hardware/connmgr/1.0/IConnmgrCallback.h>
#include <hidl/LegacySupport.h>
#include <hidl/HidlTransportSupport.h>
#include "Connmgr.h"
#include "ConnmgrCallback.h"

using vendor::sprd::hardware::connmgr::V1_0::IConnmgrCallback;
using android::sp;
using vendor::sprd::hardware::connmgr::V1_0::IConnmgr;
using android::hardware::defaultPassthroughServiceImplementation;
//using vendor::sprd::hardware::connmgr::V1_0::implementation::Connmgr;

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::status_t;
using android::OK;

int main() {
#if 0
    sp<Connmgr> service = new Connmgr();
    configureRpcThreadpool(1, true /*callerWillJoin*/);
    status_t status = service->registerAsService();

    if (status == OK) {
        ALOGI("connmgr HAL Ready.");
        joinRpcThreadpool();
    }

    ALOGE("Cannot register connmgr HAL service");
    return 1;
#endif
    return defaultPassthroughServiceImplementation<IConnmgr>();
}
