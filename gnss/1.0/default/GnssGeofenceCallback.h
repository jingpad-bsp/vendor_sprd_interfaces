#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSGEOFENCECALLBACK_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSGEOFENCECALLBACK_H

#include <vendor/sprd/hardware/gnss/1.0/IGnssGeofenceCallback.h>
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

struct GnssGeofenceCallback : public IGnssGeofenceCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssGeofenceCallback follow.
    Return<void> gnssGeofenceTransitionCb(int32_t geofenceId, const ::android::hardware::gnss::V1_0::GnssLocation& location, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceTransition transition, int64_t timestamp) override;
    Return<void> gnssGeofenceStatusCb(::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceAvailability status, const ::android::hardware::gnss::V1_0::GnssLocation& lastLocation) override;
    Return<void> gnssGeofenceAddCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) override;
    Return<void> gnssGeofenceRemoveCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) override;
    Return<void> gnssGeofencePauseCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) override;
    Return<void> gnssGeofenceResumeCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IGnssGeofenceCallback* HIDL_FETCH_IGnssGeofenceCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSGEOFENCECALLBACK_H
