#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSDEBUG_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSDEBUG_H

#include <vendor/sprd/hardware/gnss/1.0/IGnssDebug.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <hardware/gps.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssDebug;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;

using ::android::hardware::Return;
using ::android::hardware::Void;

using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

/* Interface for GNSS Debug support. */
struct GnssDebug : public IGnssDebug {
    GnssDebug(const GpsDebugInterface* gpsDebugIface);

    /*
     * Methods from ::android::hardware::gnss::V1_0::IGnssDebug follow.
     * These declarations were generated from IGnssDebug.hal.
     */
    Return<void> getDebugData(getDebugData_cb _hidl_cb)  override;

 private:
    /*
     * Constant added for backward compatibility to conventional GPS Hals which
     * returned a debug string.
     */
    const size_t kMaxDebugStrLen = 2047;
    const GpsDebugInterface* mGnssDebugIface = nullptr;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IGnssDebug* HIDL_FETCH_IGnssDebug(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSDEBUG_H
