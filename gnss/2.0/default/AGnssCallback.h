#pragma once

#include <vendor/sprd/hardware/gnss/2.0/IAGnssCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct AGnssCallback : public IAGnssCallback {
    // Methods from ::android::hardware::gnss::V2_0::IAGnssCallback follow.
    Return<void> agnssStatusCb(::android::hardware::gnss::V2_0::IAGnssCallback::AGnssType type, ::android::hardware::gnss::V2_0::IAGnssCallback::AGnssStatusValue status) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// extern "C" IAGnssCallback* HIDL_FETCH_IAGnssCallback(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
