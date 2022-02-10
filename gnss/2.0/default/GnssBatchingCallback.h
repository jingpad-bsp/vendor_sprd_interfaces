#pragma once

#include <vendor/sprd/hardware/gnss/2.0/IGnssBatchingCallback.h>
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

struct GnssBatchingCallback : public IGnssBatchingCallback {
    // Methods from ::android::hardware::gnss::V2_0::IGnssBatchingCallback follow.
    Return<void> gnssLocationBatchCb(const hidl_vec<::android::hardware::gnss::V2_0::GnssLocation>& locations) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// extern "C" IGnssBatchingCallback* HIDL_FETCH_IGnssBatchingCallback(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
