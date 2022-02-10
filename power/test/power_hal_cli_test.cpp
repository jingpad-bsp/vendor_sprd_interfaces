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

#include <string>
#include <iostream>

#include "power_hal_cli.h"

int main() {
    std::cout << "new PowerHALManager" << std::endl;
    ::android::sp<::android::PowerHALManager> powerManager = new ::android::PowerHALManager();
    powerManager->init();
    std::cout << "new PowerHintScene:" << std::endl;
    ::android::sp<::android::PowerHintScene> powerHint = powerManager->createPowerHintScene("test", static_cast<int>(PowerHintVendor::LAUNCH), "");

    std::cout << "disable():" << std::endl;
    powerManager->disable();
    std::cout << "Bgn acquire():" << std::endl;
    powerHint->acquire();
    std::cout << "End acquire()" << std::endl;
    sleep(4);
    powerHint->release();
    std::cout << "End release()" << std::endl;

    std::cout << "enable():" << std::endl;
    powerManager->enable();
    std::cout << "Bgn acquire():" << std::endl;
    powerHint->acquire();
    std::cout << "End acquire()" << std::endl;
    sleep(4);
    powerHint->release();
    std::cout << "End release()" << std::endl;

    while (1) {
        std::cout << "for test service died" << std::endl;
        sleep(600000000);
    }

    return 0;
}
