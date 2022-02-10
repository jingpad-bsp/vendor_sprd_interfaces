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
#define LOG_TAG "vendor.sprd.hardware.ILogControl@1.0-service"
#include <android/log.h>
#include <vendor/sprd/hardware/log/1.0/ILogCallback.h>
#include <vendor/sprd/hardware/log/1.0/ILogControl.h>
#include <hidl/LegacySupport.h>
#include <hidl/HidlTransportSupport.h>
#include "LogControl.h"
#include "CmdListener.h"
#include "LogCallback.h"
using vendor::sprd::hardware::log::V1_0::ILogCallback;
using android::sp;
using vendor::sprd::hardware::log::V1_0::ILogControl;
using android::hardware::defaultPassthroughServiceImplementation;
//using vendor::sprd::hardware::log::V1_0::implementation::LogControl;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::status_t;
using android::OK;

int main() {
#if 0
    android::sp<LogControl> service = new LogControl();
    CmdListener *cmdListener = new CmdListener(service);
    cmdListener->startListener();
    configureRpcThreadpool(1, true /*callerWillJoin*/);
    status_t status = service->registerAsService();

    if (status == OK) {
        ALOGI("log HAL Ready.");
        joinRpcThreadpool();
    }

    ALOGE("Cannot register log HAL service");
    return 1;
#else
    return defaultPassthroughServiceImplementation<ILogControl>();
#endif
}

