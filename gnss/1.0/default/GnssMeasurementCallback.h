#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSMEASUREMENTCALLBACK_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSMEASUREMENTCALLBACK_H

#include <vendor/sprd/hardware/gnss/1.0/IGnssMeasurementCallback.h>
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

struct GnssMeasurementCallback : public IGnssMeasurementCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssMeasurementCallback follow.
    Return<void> GnssMeasurementCb(const ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData& data) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IGnssMeasurementCallback* HIDL_FETCH_IGnssMeasurementCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSMEASUREMENTCALLBACK_H
