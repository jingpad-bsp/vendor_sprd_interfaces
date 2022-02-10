#define LOG_TAG "GnssHAL_V_GnssDebug"

#include <log/log.h>
#include "GnssConstants.h"
#include "GnssDebug.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

GnssDebug::GnssDebug(const GpsDebugInterface* gpsDebugIface) : mGnssDebugIface(gpsDebugIface) {}
// Methods from ::android::hardware::gnss::V1_0::IGnssDebug follow.
Return<void> GnssDebug::getDebugData(getDebugData_cb _hidl_cb) {
    using PositionDebug = ::android::hardware::gnss::V1_0::IGnssDebug::PositionDebug;
    PositionDebug positionDebug = {
        .valid = true,
        .latitudeDegrees = kMockLatitudeDegrees,
        .longitudeDegrees = kMockLongitudeDegrees,
        .altitudeMeters = kMockAltitudeMeters,
        .speedMetersPerSec = kMockSpeedMetersPerSec,
        .bearingDegrees = kMockBearingDegrees,
        .horizontalAccuracyMeters = kMockHorizontalAccuracyMeters,
        .verticalAccuracyMeters = kMockVerticalAccuracyMeters,
        .speedAccuracyMetersPerSecond = kMockSpeedAccuracyMetersPerSecond,
        .bearingAccuracyDegrees = kMockBearingAccuracyDegrees,
        .ageSeconds = 0.99};

    using TimeDebug = ::android::hardware::gnss::V1_0::IGnssDebug::TimeDebug;
    TimeDebug timeDebug = {.timeEstimate = kMockTimestamp,
                           .timeUncertaintyNs = 1000,
                           .frequencyUncertaintyNsPerSec = 5.0e4};

    using DebugData = ::android::hardware::gnss::V1_0::IGnssDebug::DebugData;
    DebugData data = {
		.position = positionDebug,
		.time = timeDebug
		};

    _hidl_cb(data);

    return Void();
}

// Methods from ::android::hardware::gnss::V2_0::IGnssDebug follow.
Return<void> GnssDebug::getDebugData_2_0(getDebugData_2_0_cb _hidl_cb) {
	 PositionDebug positionDebug = {
		 .valid = true,
		 .latitudeDegrees = kMockLatitudeDegrees,
		 .longitudeDegrees = kMockLongitudeDegrees,
		 .altitudeMeters = kMockAltitudeMeters,
		 .speedMetersPerSec = kMockSpeedMetersPerSec,
		 .bearingDegrees = kMockBearingDegrees,
		 .horizontalAccuracyMeters = kMockHorizontalAccuracyMeters,
		 .verticalAccuracyMeters = kMockVerticalAccuracyMeters,
		 .speedAccuracyMetersPerSecond = kMockSpeedAccuracyMetersPerSecond,
		 .bearingAccuracyDegrees = kMockBearingAccuracyDegrees,
		 .ageSeconds = 0.99};
	
	 TimeDebug timeDebug = {.timeEstimate = kMockTimestamp,
							.timeUncertaintyNs = 1000,
							.frequencyUncertaintyNsPerSec = 5.0e4};
	
	 DebugData data = {
		 .position = positionDebug, 
		 .time = timeDebug
		 };
	
	 _hidl_cb(data);
	
	 return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssDebug* HIDL_FETCH_IGnssDebug(const char* /* name */) {
    //return new GnssDebug();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
