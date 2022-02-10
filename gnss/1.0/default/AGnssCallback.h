#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSCALLBACK_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSCALLBACK_H

#include <vendor/sprd/hardware/gnss/1.0/IAGnssCallback.h>
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

struct AGnssCallback : public IAGnssCallback {
    // Methods from ::android::hardware::gnss::V1_0::IAGnssCallback follow.
    Return<void> agnssStatusIpV4Cb(const ::android::hardware::gnss::V1_0::IAGnssCallback::AGnssStatusIpV4& status) override;
    Return<void> agnssStatusIpV6Cb(const ::android::hardware::gnss::V1_0::IAGnssCallback::AGnssStatusIpV6& status) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IAGnssCallback* HIDL_FETCH_IAGnssCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSCALLBACK_H
