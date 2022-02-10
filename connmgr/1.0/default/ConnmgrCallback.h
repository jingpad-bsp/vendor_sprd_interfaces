#ifndef VENDOR_SPRD_HARDWARE_CONNMGR_V1_0_CONNMGRCALLBACK_H
#define VENDOR_SPRD_HARDWARE_CONNMGR_V1_0_CONNMGRCALLBACK_H

#include <vendor/sprd/hardware/connmgr/1.0/IConnmgrCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace connmgr {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct ConnmgrCallback : public IConnmgrCallback {
    // Methods from IConnmgrCallback follow.
    Return<void> WcndHandleEvents(const hidl_string& dataString) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IConnmgrCallback* HIDL_FETCH_IConnmgrCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace connmgr
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_CONNMGR_V1_0_CONNMGRCALLBACK_H
