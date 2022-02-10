#pragma once

#include "gnss_debug.h"
#include <vendor/sprd/hardware/gnss/2.0/IGnssDebug.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif

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

struct GnssDebug : public IGnssDebug {
    GnssDebug(const GpsDebugInterface* gpsDebugIface);
    // Methods from ::android::hardware::gnss::V1_0::IGnssDebug follow.
    Return<void> getDebugData(getDebugData_cb _hidl_cb) override;
    // Methods from ::android::hardware::gnss::V2_0::IGnssDebug follow.
    Return<void> getDebugData_2_0(getDebugData_2_0_cb _hidl_cb) override;

 private:
    /*
     * Constant added for backward compatibility to conventional GPS Hals which
     * returned a debug string.
     */
    const size_t kMaxDebugStrLen = 2047;
    const GpsDebugInterface* mGnssDebugIface = nullptr;
};

// extern "C" IGnssDebug* HIDL_FETCH_IGnssDebug(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
