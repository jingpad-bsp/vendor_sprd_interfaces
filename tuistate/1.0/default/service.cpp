#define LOG_TAG "vendor.sprd.hardware.tuistate@1.0-service"

#include <vendor/sprd/hardware/tuistate/1.0/ITuistate.h>
#include <hidl/HidlTransportSupport.h>
#include <android/log.h>

#include "Tuistate.h"

using android::hardware::joinRpcThreadpool;
using vendor::sprd::hardware::tuistate::V1_0::ITuistate;
using vendor::sprd::hardware::tuistate::V1_0::implementation::Tuistate;
using android::sp;

int main() {
    ::android::hardware::configureRpcThreadpool(1, true /*willJoinThreadpool*/);
    sp<ITuistate> TuiState = Tuistate::getInstance();
    if(TuiState != nullptr) {
        android::status_t status = TuiState->registerAsService();
        if (status != android::OK) {
            ALOGE("Can't create instance of TuiState, nullptr\n");
            return -1;
        }
    }
    joinRpcThreadpool();
    return 0;  // Should never get here.
}
