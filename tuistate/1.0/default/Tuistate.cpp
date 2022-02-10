#include "Tuistate.h"
#include <log/log.h>


namespace vendor {
namespace sprd {
namespace hardware {
namespace tuistate {
namespace V1_0 {
namespace implementation {


ITuistate* Tuistate::getInstance() {
    static Tuistate sInstance;
    return &sInstance;
}

// Methods from ::vendor::sprd::hardware::tuistate::V1_0::ITuistate follow.
Return<int32_t> Tuistate::listenTuiState(const sp<::vendor::sprd::hardware::tuistate::V1_0::ITuiStateChangeCallback>& stateChangeClientCb) {
    // TODO implement
    std::lock_guard<std::mutex> lock(mClientCbMutex);
    mClientCb = stateChangeClientCb;
    return int32_t {0};
}

Return<void> Tuistate::notify(bool tuion) {
    // TODO implement
    Tuistate* thisPtr = static_cast<Tuistate*>(Tuistate::getInstance());
    if (thisPtr == nullptr) {
        ALOGE("Receiving callbacks before the client callback is registered!\n");
        return Void();
    }
    if(mClientCb == nullptr) {
        ALOGE("tui state changed callback not set! \n");
        return Void();
    }
    std::lock_guard<std::mutex> lock(thisPtr->mClientCbMutex);
    ALOGD("callback tui state changed %d ... \n", tuion);
    mClientCb->stateChange(tuion);

    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.
//ITuistate* HIDL_FETCH_ITuistate(const char* /* name */) {
//    return Tuistate::getInstance();
//}

}  // namespace implementation
}  // namespace V1_0
}  // namespace tuistate
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
