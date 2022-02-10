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
//#define LOG_TAG "vendor.sprd.hardware.IAprd@1.0-service"
#include <android/log.h>
#include <vendor/sprd/hardware/aprd/1.0/IAprdCallback.h>
#include <vendor/sprd/hardware/aprd/1.0/IAprdInfoSync.h>
#include <hidl/LegacySupport.h>
#include <hidl/HidlTransportSupport.h>

using vendor::sprd::hardware::aprd::V1_0::IAprdCallback;
using android::sp;
using vendor::sprd::hardware::aprd::V1_0::IAprdInfoSync;
using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::status_t;
using android::OK;

int main() {
    return defaultPassthroughServiceImplementation<IAprdInfoSync>();
}

