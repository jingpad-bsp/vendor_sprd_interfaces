#ifndef VENDOR_SPRD_HARDWARE_TUISTATE_V1_0_TUISTATE_H
#define VENDOR_SPRD_HARDWARE_TUISTATE_V1_0_TUISTATE_H

#include <vendor/sprd/hardware/tuistate/1.0/ITuistate.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace tuistate {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Tuistate : public ITuistate {
    // Method to wrap legacy HAL with Tuistate class
    static ITuistate* getInstance();
    // Methods from ::vendor::sprd::hardware::tuistate::V1_0::ITuistate follow.
    Return<int32_t> listenTuiState(const sp<::vendor::sprd::hardware::tuistate::V1_0::ITuiStateChangeCallback>& stateChangeClientCb) override;
    Return<void> notify(bool tuion) override;

private:
    Tuistate() : mClientCb(nullptr) {};
    std::mutex mClientCbMutex;
    sp<::vendor::sprd::hardware::tuistate::V1_0::ITuiStateChangeCallback> mClientCb;
    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
//extern "C" ITuistate* HIDL_FETCH_ITuistate(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace tuistate
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_TUISTATE_V1_0_TUISTATE_H
