#pragma once

#include "gnss_debug.h"
#include <AGnss.h>
#include <AGnssRil.h>
#include <GnssBatching.h>
#include <GnssConfiguration.h>
#include <GnssDebug.h>
#include <GnssGeofencing.h>
#include <GnssMeasurement.h>
#include <GnssNavigationMessage.h>
#include <GnssNi.h>
#include <GnssXtra.h>
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IGnss.h>
#include <hardware/fused_location.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <atomic>
#include <mutex>
#include <thread>
#include "GnssConfiguration.h"
#include "GnssUnisocExt.h"
#include "GnssMeasurementCorrections.h"
#include "GnssVisibilityControl.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using IGnssCallback = ::android::hardware::gnss::V1_0::IGnssCallback;
using IGnssCallback_1_1 = ::android::hardware::gnss::V1_1::IGnssCallback;
using IGnssCallback_2_0 = ::android::hardware::gnss::V2_0::IGnssCallback;

using ::android::hardware::gnss::V1_0::GnssMax;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;
using ::android::wp;
using ::android::hardware::hidl_death_recipient; 
using LegacyGnssSystemInfo = ::GnssSystemInfo;

using GnssConstellationType = ::android::hardware::gnss::V1_0::GnssConstellationType;
using GnssLocation = ::android::hardware::gnss::V1_0::GnssLocation;
using GnssLocation_2_0 = ::android::hardware::gnss::V2_0::GnssLocation;

using GnssSvInfo = ::android::hardware::gnss::V1_0::IGnssCallback::GnssSvInfo;
//using ::android::hardware::gnss::measurement_corrections::V1_0::IMeasurementCorrections;
using ::android::hardware::gnss::measurement_corrections::V1_0::implementation::GnssMeasurementCorrections;
using ::android::hardware::gnss::visibility_control::V1_0::implementation::GnssVisibilityControl;

struct Gnss : public IGnss {
    Gnss(gps_device_t* gnss_device);
    ~Gnss();
    // Methods from ::android::hardware::gnss::V1_0::IGnss follow.
    Return<bool> setCallback(const sp<::android::hardware::gnss::V1_0::IGnssCallback>& callback) override;
    Return<bool> start() override;
    Return<bool> stop() override;
    Return<void> cleanup() override;
    Return<bool> injectTime(int64_t timeMs, int64_t timeReferenceMs,
                    int32_t uncertaintyMs) override;
    Return<bool> injectLocation(double latitudeDegrees, double longitudeDegrees,
                    float accuracyMeters) override;
    Return<void> deleteAidingData(
                    ::android::hardware::gnss::V1_0::IGnss::GnssAidingData aidingDataFlags) override;
    Return<bool> setPositionMode(
                    ::android::hardware::gnss::V1_0::IGnss::GnssPositionMode mode,
                    ::android::hardware::gnss::V1_0::IGnss::GnssPositionRecurrence recurrence,
                    uint32_t minIntervalMs, uint32_t preferredAccuracyMeters,
                    uint32_t preferredTimeMs) override;
    Return<sp<::android::hardware::gnss::V1_0::IAGnssRil>> getExtensionAGnssRil() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssGeofencing>> getExtensionGnssGeofencing() override;
    Return<sp<::android::hardware::gnss::V1_0::IAGnss>> getExtensionAGnss() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssNi>> getExtensionGnssNi() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssMeasurement>> getExtensionGnssMeasurement() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssNavigationMessage>> getExtensionGnssNavigationMessage() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssXtra>> getExtensionXtra() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssConfiguration>> getExtensionGnssConfiguration() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssDebug>> getExtensionGnssDebug() override;
    Return<sp<::android::hardware::gnss::V2_0::IGnssDebug>> getExtensionGnssDebug_2_0() override;
    Return<sp<::android::hardware::gnss::V1_0::IGnssBatching>> getExtensionGnssBatching() override;
    Return<sp<::android::hardware::gnss::V2_0::IGnssBatching>> getExtensionGnssBatching_2_0() override;

    // Methods from ::android::hardware::gnss::V1_1::IGnss follow.
    Return<bool> setCallback_1_1(
		const sp<::android::hardware::gnss::V1_1::IGnssCallback>& callback) override;
    Return<bool> setPositionMode_1_1(
	::android::hardware::gnss::V1_0::IGnss::GnssPositionMode mode, 
	::android::hardware::gnss::V1_0::IGnss::GnssPositionRecurrence recurrence, 
	uint32_t minIntervalMs, uint32_t preferredAccuracyMeters, uint32_t preferredTimeMs, 
	bool lowPowerMode) override;
    Return<sp<::android::hardware::gnss::V1_1::IGnssConfiguration>> getExtensionGnssConfiguration_1_1() override;
    Return<sp<::android::hardware::gnss::V1_1::IGnssMeasurement>> getExtensionGnssMeasurement_1_1() override;
    Return<bool> injectBestLocation(const ::android::hardware::gnss::V1_0::GnssLocation& location) override;

    // Methods from ::android::hardware::gnss::V2_0::IGnss follow.
    Return<bool> setCallback_2_0(const sp<::android::hardware::gnss::V2_0::IGnssCallback>& callback) override;
    Return<sp<::android::hardware::gnss::V2_0::IGnssConfiguration>> getExtensionGnssConfiguration_2_0() override;
    Return<sp<::android::hardware::gnss::V2_0::IAGnss>> getExtensionAGnss_2_0() override;
    Return<sp<::android::hardware::gnss::V2_0::IAGnssRil>> getExtensionAGnssRil_2_0() override;
    Return<sp<::android::hardware::gnss::V2_0::IGnssMeasurement>> getExtensionGnssMeasurement_2_0() override;
    Return<sp<::android::hardware::gnss::measurement_corrections::V1_0::IMeasurementCorrections>> getExtensionMeasurementCorrections() override;
    Return<sp<::android::hardware::gnss::visibility_control::V1_0::IGnssVisibilityControl>> getExtensionVisibilityControl() override;
    Return<bool> injectBestLocation_2_0(const ::android::hardware::gnss::V2_0::GnssLocation& location) override;

    // Methods from ::vendor::sprd::hardware::gnss::V2_0::IGnss follow.
    Return<sp<::vendor::sprd::hardware::gnss::V2_0::IGnssUnisocExt>> getExtensionUnisocExt() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    static void locationCb(GpsLocation* location);
    static void statusCb(GpsStatus* gnss_status);
    static void nmeaCb(GpsUtcTime timestamp, const char* nmea, int length);
    static void setCapabilitiesCb(uint32_t capabilities);
    static void acquireWakelockCb();
    static void releaseWakelockCb();
    static void requestUtcTimeCb();
    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);
    static void gnssSvStatusCb(GnssSvStatus* status);
    /*
     * Deprecated callback added for backward compatibility to devices that do
     * not support GnssSvStatus.
     */
    static void gpsSvStatusCb(GpsSvStatus* status);
    static void setSystemInfoCb(const LegacyGnssSystemInfo* info);

    /*
     * Wakelock consolidation, only needed for dual use of a gps.h & fused_location.h HAL
     *
     * Ensures that if the last call from either legacy .h was to acquire a wakelock, that a
     * wakelock is held.  Otherwise releases it.
     */
    static void acquireWakelockFused();
    static void releaseWakelockFused();

    /*
     * Holds function pointers to the callback methods.
     */
    static GpsCallbacks sGnssCb;
	
   private:
    /*
     * For handling system-server death while GNSS service lives on.
     */
    class GnssHidlDeathRecipient : public hidl_death_recipient {
      public:
        GnssHidlDeathRecipient(const sp<Gnss> gnss) : mGnss(gnss) {
        }

        virtual void serviceDied(uint64_t /*cookie*/,
                const wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
            mGnss->handleHidlDeath();
        }
      private:
        sp<Gnss> mGnss;
    };

    // for wakelock consolidation, see above
    static void acquireWakelockGnss();
    static void releaseWakelockGnss();
    static void updateWakelock();
    static bool sWakelockHeldGnss;
    static bool sWakelockHeldFused;

    /*
     * Cleanup for death notification
     */
    void handleHidlDeath();

    sp<GnssXtra> mGnssXtraIface = nullptr;
    sp<AGnssRil> mGnssRil = nullptr;
    sp<GnssUnisocExt> mGnssUnisocExt = nullptr;
    sp<GnssGeofencing> mGnssGeofencingIface = nullptr;
    sp<AGnss> mAGnssIface = nullptr;
    sp<GnssNi> mGnssNi = nullptr;
    sp<GnssMeasurement> mGnssMeasurement = nullptr;
    sp<GnssNavigationMessage> mGnssNavigationMessage = nullptr;
    sp<GnssDebug> mGnssDebug = nullptr;
    sp<GnssConfiguration> mGnssConfig = nullptr;
    sp<GnssBatching> mGnssBatching = nullptr;
    sp<::android::hardware::gnss::measurement_corrections::V1_0::IMeasurementCorrections> mMeasurementCorrections = nullptr;
    sp<::android::hardware::gnss::visibility_control::V1_0::IGnssVisibilityControl> mGnssVisibilityControl = nullptr;
    sp<GnssHidlDeathRecipient> mDeathRecipient;

    const GpsInterface* mGnssIface = nullptr;
    static sp<IGnssCallback> sGnssCbIface;
    static sp<IGnssCallback_1_1> sGnssCbIface_1_1;
    static sp<IGnssCallback_2_0> sGnssCbIface_2_0;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;

    // Values saved for resend
    static uint32_t sCapabilitiesCached;
    static uint16_t sYearOfHwCached;
	
#if 0
    Return<GnssLocation> getMockLocation() const;
    Return<GnssSvStatus> getMockSvStatus() const;

    Return<GnssSvInfo> getSvInfo(int16_t svid, GnssConstellationType type, float cN0DbHz,
	                     float elevationDegress, float azimuthDegress) const;
    Return<void> reportLocation(const GnssLocation&) const;
    Return<void> reportSvStatus(const GnssSvStatus&) const;
#endif

    static sp<IGnssCallback> sGnssCallback;
    std::atomic<long> mMinIntervalMs;
    sp<GnssConfiguration> mGnssConfiguration;
    std::atomic<bool> mIsActive;
    std::thread mThread;
    mutable std::mutex mMutex;
};

extern "C" IGnss* HIDL_FETCH_IGnss(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
