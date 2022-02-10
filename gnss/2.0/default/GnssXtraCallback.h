#pragma once

#include <vendor/sprd/hardware/gnss/2.0/IGnssXtraCallback.h>
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

struct GnssXtraCallback : public IGnssXtraCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssXtraCallback follow.
    Return<void> downloadRequestCb() override;
};

// extern "C" IGnssXtraCallback* HIDL_FETCH_IGnssXtraCallback(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
