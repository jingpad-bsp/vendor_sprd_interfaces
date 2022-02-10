#include <android/log.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include <vendor/sprd/hardware/soter/1.0/ISoter.h>
#include <vendor/sprd/hardware/soter/1.0/types.h>
#include "Soter.h"

#define LOG_TAG "Soter[HIDL]"
#define LOG_NDDEBUG 0

using vendor::sprd::hardware::soter::V1_0::ISoter;
using vendor::sprd::hardware::soter::V1_0::implementation::Soter;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;

int main() {
    ALOGD("soter hidl main()...");
    android::sp<ISoter> soter = Soter::getInstance();

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (soter != nullptr) {
        if (::android::OK != soter->registerAsService()) {
            ALOGE("Error while registering ISoter service");
            return 1;
        }
    } else {
        ALOGE("Can't create instance of Soter, nullptr");
    }

    joinRpcThreadpool();

    return 0; // should never get here
}

