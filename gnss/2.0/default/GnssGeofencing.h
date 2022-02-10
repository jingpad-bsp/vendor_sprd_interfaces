#pragma once

#include "gnss_debug.h"
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IGnssGeofencing.h>
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
using ::android::hardware::gnss::V1_0::IGnssGeofenceCallback;
using ::android::hardware::gnss::V1_0::IGnssGeofencing;
using ::android::sp;

struct GnssGeofencing : public IGnssGeofencing {
    GnssGeofencing(const GpsGeofencingInterface* gpsGeofencingIface);
    ~GnssGeofencing();
    // Methods from ::android::hardware::gnss::V1_0::IGnssGeofencing follow.
    Return<void> setCallback(const sp<::android::hardware::gnss::V1_0::IGnssGeofenceCallback>& callback) override;
    Return<void> addGeofence(int32_t geofenceId, 
	double latitudeDegrees, 
	double longitudeDegrees, 
	double radiusMeters, 
	::android::hardware::gnss::V1_0::IGnssGeofenceCallback::GeofenceTransition lastTransition, 
    int32_t monitorTransitions,
	uint32_t notificationResponsivenessMs, 
	uint32_t unknownTimerMs) override;
    Return<void> pauseGeofence(int32_t geofenceId) override;
    Return<void> resumeGeofence(int32_t geofenceId, int32_t monitorTransitions)  override;
    Return<void> removeGeofence(int32_t geofenceId) override;

    /*
     * Callback methods to be passed into the conventional GNSS HAL by the default
     * implementation. These methods are not part of the IGnssGeofencing base class.
     */
    static void gnssGfTransitionCb(int32_t geofence_id, GpsLocation* location,
                                   int32_t transition, GpsUtcTime timestamp);
    static void gnssGfStatusCb(int32_t status, GpsLocation* last_location);
    static void gnssGfAddCb(int32_t geofence_id, int32_t status);
    static void gnssGfRemoveCb(int32_t geofence_id, int32_t status);
    static void gnssGfPauseCb(int32_t geofence_id, int32_t status);
    static void gnssGfResumeCb(int32_t geofence_id, int32_t status);
    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);

    /*
     * Holds function pointers to the callback methods.
     */
    static GpsGeofenceCallbacks sGnssGfCb;

 private:
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static sp<IGnssGeofenceCallback> mGnssGeofencingCbIface;
    const GpsGeofencingInterface* mGnssGeofencingIface = nullptr;
    static bool sInterfaceExists;
};

// extern "C" IGnssGeofencing* HIDL_FETCH_IGnssGeofencing(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
