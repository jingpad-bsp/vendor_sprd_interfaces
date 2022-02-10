#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSRILCALLBACK_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSRILCALLBACK_H

#include <vendor/sprd/hardware/gnss/1.0/IAGnssRilCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct AGnssRilCallback : public IAGnssRilCallback {
    // Methods from ::android::hardware::gnss::V1_0::IAGnssRilCallback follow.
    Return<void> requestSetIdCb(uint32_t setIdflag) override;
    Return<void> requestRefLocCb() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IAGnssRilCallback* HIDL_FETCH_IAGnssRilCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSRILCALLBACK_H
