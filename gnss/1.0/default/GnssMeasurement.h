#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSMEASUREMENT_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSMEASUREMENT_H

#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/1.0/IGnssMeasurement.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <hardware/gps.h>
//include util is not work, so include type directly
#include <GnssUtils.h>
#include <android/hardware/gnss/1.0/IGnss.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssMeasurement;
using ::android::hardware::gnss::V1_0::IGnssMeasurementCallback;
using GnssData = ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData;
using ::android::hardware::gnss::V1_0::GnssConstellationType;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;

using ::android::hardware::Return;
using ::android::hardware::Void;

using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;
using android::hardware::gnss::V1_0::IGnss;
using ::android::hardware::gnss::V1_0::IGnss;

//using LegacyGnssData = ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData;
using LegacyGnssData = ::GnssData;

/*
 * Extended interface for GNSS Measurements support. Also contains wrapper methods to allow methods
 * from IGnssMeasurementCallback interface to be passed into the conventional implementation of the
 * GNSS HAL.
 */
struct GnssMeasurement : public IGnssMeasurement {
    GnssMeasurement(const GpsMeasurementInterface* gpsMeasurementIface);

    /*
     * Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.
     * These declarations were generated from IGnssMeasurement.hal.
     */
    Return<GnssMeasurementStatus> setCallback(
        const sp<IGnssMeasurementCallback>& callback) override;
    Return<void> close() override;

    /*
     * Callback methods to be passed into the conventional GNSS HAL by the default
     * implementation. These methods are not part of the IGnssMeasurement base class.
     */
    static void gnssMeasurementCb(LegacyGnssData* data);
     /*
      * Deprecated callback added for backward compatibity for devices that do
      * not support GnssData measurements.
      */
    static void gpsMeasurementCb(GpsData* data);

    /*
     * Holds function pointers to the callback methods.
     */
    static GpsMeasurementCallbacks sGnssMeasurementCbs;

 private:
    const GpsMeasurementInterface* mGnssMeasureIface = nullptr;
    static sp<IGnssMeasurementCallback> sGnssMeasureCbIface;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IGnssMeasurement* HIDL_FETCH_IGnssMeasurement(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSMEASUREMENT_H
