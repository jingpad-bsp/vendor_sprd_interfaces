#pragma once

#include <vendor/sprd/hardware/gnss/2.0/IGnssUnisocExtCallback.h>
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

struct GnssUnisocExtCallback : public IGnssUnisocExtCallback {
    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// extern "C" IGnssUnisocExtCallback* HIDL_FETCH_IGnssUnisocExtCallback(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
