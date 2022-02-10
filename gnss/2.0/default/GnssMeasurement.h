#pragma once

#include "gnss_debug.h"
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IGnssMeasurement.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

//#define MOCK

#ifdef MOCK
#include <atomic>
#include <mutex>
#include <thread>
#endif

#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif
#include <GnssUtils.h>
namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::gnss::V2_0::ElapsedRealtime;
using ::android::hardware::gnss::V2_0::ElapsedRealtimeFlags;

using IGnssMeasurementCallback = ::android::hardware::gnss::V1_0::IGnssMeasurementCallback;
using IGnssMeasurementCallback_1_1 = ::android::hardware::gnss::V1_1::IGnssMeasurementCallback;
using IGnssMeasurementCallback_2_0 = ::android::hardware::gnss::V2_0::IGnssMeasurementCallback;

using GnssData = ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData;
using GnssData_2_0 = ::android::hardware::gnss::V2_0::IGnssMeasurementCallback::GnssData;

using GnssConstellationType = ::android::hardware::gnss::V1_0::GnssConstellationType;
using GnssConstellationType_2_0 = ::android::hardware::gnss::V2_0::GnssConstellationType;
using GnssMeasurementFlags = ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssMeasurementFlags;
using GnssMeasurementState_2_0 = ::android::hardware::gnss::V2_0::IGnssMeasurementCallback::GnssMeasurementState;

using LegacyGnssData = ::GnssData;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssMeasurement : public IGnssMeasurement {
    // Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.
    Return<::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus> setCallback(const sp<::android::hardware::gnss::V1_0::IGnssMeasurementCallback>& callback) override;
    Return<void> close() override;

    // Methods from ::android::hardware::gnss::V1_1::IGnssMeasurement follow.
    Return<::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus> setCallback_1_1(const sp<::android::hardware::gnss::V1_1::IGnssMeasurementCallback>& callback, bool enableFullTracking) override;

    // Methods from ::android::hardware::gnss::V2_0::IGnssMeasurement follow.
    Return<::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus> setCallback_2_0(const sp<::android::hardware::gnss::V2_0::IGnssMeasurementCallback>& callback, bool enableFullTracking) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    GnssMeasurement(const GpsMeasurementInterface* gpsMeasurementIface);


    /*
     * Callback methods to be passed into the conventional GNSS HAL by the default
     * implementation. These methods are not part of the IGnssMeasurement base class.
     */
    static void gnssMeasurementCb(LegacyGnssData* data);
     /*
      * Deprecated callback added for backward compatibity for devices that do
      * not support GnssData measurements.
      */
    static void gnssMeasurementCb_2_0(LegacyGnssData* legacyGnssData);
    static void gnssMeasurementCbs(LegacyGnssData* legacyGnssData);
    static void gpsMeasurementCb(GpsData* data);

    /*
     * Holds function pointers to the callback methods.
     */
    static GpsMeasurementCallbacks sGnssMeasurementCbs;

 private:
    const GpsMeasurementInterface* mGnssMeasureIface = nullptr;
    static sp<IGnssMeasurementCallback> sGnssMeasureCbIface;
    static sp<IGnssMeasurementCallback_1_1> sGnssMeasureCbIface_1_1;
    static sp<IGnssMeasurementCallback_2_0> sGnssMeasureCbIface_2_0;

#ifdef MOCK
    void start();
    void stop();
    GnssData_2_0 getMockMeasurement();
    void reportMeasurement(const GnssData_2_0&);

    std::atomic<long> mMinIntervalMillis;
    std::atomic<bool> mIsActive;
    std::thread mThread;
    mutable std::mutex mMutex;
#endif
};

// extern "C" IGnssMeasurement* HIDL_FETCH_IGnssMeasurement(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
