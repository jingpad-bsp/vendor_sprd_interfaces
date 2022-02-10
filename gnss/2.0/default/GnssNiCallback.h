#pragma once

#include <vendor/sprd/hardware/gnss/2.0/IGnssNiCallback.h>
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

struct GnssNiCallback : public IGnssNiCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssNiCallback follow.
    Return<void> niNotifyCb(const ::android::hardware::gnss::V1_0::IGnssNiCallback::GnssNiNotification& notification) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// extern "C" IGnssNiCallback* HIDL_FETCH_IGnssNiCallback(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
