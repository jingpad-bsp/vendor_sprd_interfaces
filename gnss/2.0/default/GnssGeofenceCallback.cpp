#include "GnssGeofenceCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssGeofenceCallback follow.
Return<void> GnssGeofenceCallback::gnssGeofenceTransitionCb(int32_t geofenceId, const ::android::hardware::gnss::V1_0::GnssLocation& location, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceTransition transition, int64_t timestamp) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceStatusCb(::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceAvailability status, const ::android::hardware::gnss::V1_0::GnssLocation& lastLocation) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceAddCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceRemoveCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofencePauseCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceResumeCb(int32_t geofenceId, ::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssGeofenceCallback* HIDL_FETCH_IGnssGeofenceCallback(const char* /* name */) {
    //return new GnssGeofenceCallback();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
