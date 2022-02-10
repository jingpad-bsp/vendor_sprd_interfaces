/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "GnssUtils.h"
#include <log/log.h>

#define LOG_TAG "GnssHAL_V_GnssUtil"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

GnssLocation convertToGnssLocation(GpsLocation* location) {
    GnssLocation gnssLocation = {};
    if (location != nullptr) {
        gnssLocation = {
            // Bit operation AND with 1f below is needed to clear vertical accuracy,
            // speed accuracy and bearing accuracy flags as some vendors are found
            // to be setting these bits in pre-Android-O devices
            .gnssLocationFlags = static_cast<uint16_t>(location->flags & 0xff),
            .latitudeDegrees = location->latitude,
            .longitudeDegrees = location->longitude,
            .altitudeMeters = location->altitude,
            .speedMetersPerSec = location->speed,
            .bearingDegrees = location->bearing,
            .horizontalAccuracyMeters = location->accuracy,
            // Older chipsets do not provide the following 3 fields, hence the flags
            // HAS_VERTICAL_ACCURACY, HAS_SPEED_ACCURACY and HAS_BEARING_ACCURACY are
            // not set and the field are set to zeros.
            .verticalAccuracyMeters = location->veraccuracy,
            .speedAccuracyMetersPerSecond = location->speedaccuracy,
            .bearingAccuracyDegrees = location->bearaccuracy,
            .timestamp = location->timestamp
        };
    }

    return gnssLocation;
}

GnssLocation convertToGnssLocation(FlpLocation* location) {
    GnssLocation gnssLocation = {};
    if (location != nullptr) {
        gnssLocation = {
            // Bit mask applied (and 0's below) for same reason as above with GpsLocation
            .gnssLocationFlags = static_cast<uint16_t>(location->flags & 0x1f),
            .latitudeDegrees = location->latitude,
            .longitudeDegrees = location->longitude,
            .altitudeMeters = location->altitude,
            .speedMetersPerSec = location->speed,
            .bearingDegrees = location->bearing,
            .horizontalAccuracyMeters = location->accuracy,
            .verticalAccuracyMeters = 0,
            .speedAccuracyMetersPerSecond = 0,
            .bearingAccuracyDegrees = 0,
            .timestamp = location->timestamp
        };
    }

    return gnssLocation;
}

GnssLocation_2_0 convertToGnssLocation_2_0(GpsLocation* location) {
     GnssLocation_2_0 gnssLocation = {};

    ALOGD("%s: enter", __func__);
    if (location != nullptr) {
        gnssLocation = {
            .v1_0.gnssLocationFlags = static_cast<uint16_t>(location->flags & 0xff),
            .v1_0.latitudeDegrees = location->latitude,
            .v1_0.longitudeDegrees = location->longitude,
            .v1_0.altitudeMeters = location->altitude,
            .v1_0.speedMetersPerSec = location->speed,
            .v1_0.bearingDegrees = location->bearing,
            .v1_0.horizontalAccuracyMeters = location->accuracy,
            .v1_0.verticalAccuracyMeters = location->veraccuracy,
            .v1_0.speedAccuracyMetersPerSecond = location->speedaccuracy,
            .v1_0.bearingAccuracyDegrees = location->bearaccuracy,
            .v1_0.timestamp = location->timestamp,
            .elapsedRealtime.flags = 0,
            .elapsedRealtime.timestampNs = 0,
            .elapsedRealtime.timeUncertaintyNs = 0
        };
    }
    ALOGD("%s: latitudeDegrees:%f, longitudeDegrees: %f, elapsedRealtime.flags:%d, elapsedRealtime.timestampNs: %d, elapsedRealtime.timeUncertaintyNs: %d",
		__func__, gnssLocation.v1_0.latitudeDegrees, gnssLocation.v1_0.longitudeDegrees, gnssLocation.elapsedRealtime.flags, 
		gnssLocation.elapsedRealtime.timestampNs, gnssLocation.elapsedRealtime.timeUncertaintyNs);

    return gnssLocation;
}

GnssLocation_2_0 convertToGnssLocation_2_0(FlpLocation* location) {
     GnssLocation_2_0 gnssLocation = {};
    if (location != nullptr) {
        gnssLocation = {
            .v1_0.gnssLocationFlags = static_cast<uint16_t>(location->flags & 0x1f),
            .v1_0.latitudeDegrees = location->latitude,
            .v1_0.longitudeDegrees = location->longitude,
            .v1_0.altitudeMeters = location->altitude,
            .v1_0.speedMetersPerSec = location->speed,
            .v1_0.bearingDegrees = location->bearing,
            .v1_0.horizontalAccuracyMeters = location->accuracy,
            .v1_0.verticalAccuracyMeters = 0,
            .v1_0.speedAccuracyMetersPerSecond = 0,
            .v1_0.bearingAccuracyDegrees = 0,
            .v1_0.timestamp = location->timestamp,
            .elapsedRealtime.flags = 0,
            .elapsedRealtime.timestampNs = 0,
            .elapsedRealtime.timeUncertaintyNs = 0
        };
    }
    return gnssLocation;
}


}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

