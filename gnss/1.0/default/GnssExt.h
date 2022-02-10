#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSS_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSS_H

#include <AGnss.h>
#include <AGnssRilExt.h>
#include <GnssBatching.h>
#include <GnssConfiguration.h>
#include <GnssDebug.h>
#include <GnssGeofencing.h>
#include <GnssMeasurement.h>
#include <GnssNavigationMessage.h>
#include <GnssNi.h>
#include <GnssXtra.h>
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/1.0/IGnssExt.h>
//#include <android/hardware/gnss/1.0/IGnss.h>
#include <hardware/fused_location.h>
#include <hardware/gps.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

//harry 0312
//using vendor::sprd::hardware::gnss::V1_0::IAGnssRil;
//using ::android::hardware::gnss::V1_0::IAGnssRilCallback;

using ::android::hardware::gnss::V1_0::IGnssCallback;
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

using ::vendor::sprd::hardware::gnss::V1_0::IGnssExt;
using LegacyGnssSystemInfo = ::GnssSystemInfo;

/*
 * Represents the standard GNSS interface. Also contains wrapper methods to allow methods from
 * IGnssCallback interface to be passed into the conventional implementation of the GNSS HAL.
 */
class GnssExt : public IGnssExt {
  public:
    GnssExt(gps_device_t* gnss_device);
    ~GnssExt();

    /*
     * Methods from ::android::hardware::gnss::V1_0::IGnss follow.
     * These declarations were generated from Gnss.hal.
     */
    Return<bool> setCallback(const sp<IGnssCallback>& callback)  override;
    Return<bool> start()  override;
    Return<bool> stop()  override;
    Return<void> cleanup()  override;
    Return<bool> injectLocation(double latitudeDegrees,
                                double longitudeDegrees,
                                float accuracyMeters)  override;
    Return<bool> injectTime(int64_t timeMs,
                            int64_t timeReferenceMs,
                            int32_t uncertaintyMs) override;
    Return<void> deleteAidingData(IGnss::GnssAidingData aidingDataFlags)  override;
    Return<bool> setPositionMode(IGnss::GnssPositionMode mode,
                                 IGnss::GnssPositionRecurrence recurrence,
                                 uint32_t minIntervalMs,
                                 uint32_t preferredAccuracyMeters,
                                 uint32_t preferredTimeMs)  override;
    Return<sp<::android::hardware::gnss::V1_0::IAGnssRil>> getExtensionAGnssRil() override;								 
    Return<sp<IAGnssRilExt>> getExtensionAGnssRilExt();
    Return<sp<IGnssGeofencing>> getExtensionGnssGeofencing() override;
    Return<sp<IAGnss>> getExtensionAGnss() override;
    Return<sp<IGnssNi>> getExtensionGnssNi() override;
    Return<sp<IGnssMeasurement>> getExtensionGnssMeasurement() override;
    Return<sp<IGnssNavigationMessage>> getExtensionGnssNavigationMessage() override;
    Return<sp<IGnssXtra>> getExtensionXtra() override;
    Return<sp<IGnssConfiguration>> getExtensionGnssConfiguration() override;
    Return<sp<IGnssDebug>> getExtensionGnssDebug() override;
    Return<sp<IGnssBatching>> getExtensionGnssBatching() override;

    /*
     * Callback methods to be passed into the conventional GNSS HAL by the default
     * implementation. These methods are not part of the IGnss base class.
     */
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
        GnssHidlDeathRecipient(const sp<GnssExt> gnss) : mGnss(gnss) {
        }

        virtual void serviceDied(uint64_t /*cookie*/,
                const wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
            mGnss->handleHidlDeath();
        }
      private:
        sp<GnssExt> mGnss;
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
    sp<AGnssRilExt> mGnssRil = nullptr;
    sp<GnssGeofencing> mGnssGeofencingIface = nullptr;
    sp<AGnss> mAGnssIface = nullptr;
    sp<GnssNi> mGnssNi = nullptr;
    sp<GnssMeasurement> mGnssMeasurement = nullptr;
    sp<GnssNavigationMessage> mGnssNavigationMessage = nullptr;
    sp<GnssDebug> mGnssDebug = nullptr;
    sp<GnssConfiguration> mGnssConfig = nullptr;
    sp<GnssBatching> mGnssBatching = nullptr;

    sp<GnssHidlDeathRecipient> mDeathRecipient;

    const GpsInterface* mGnssIface = nullptr;
    static sp<IGnssCallback> sGnssCbIface;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;

    // Values saved for resend
    static uint32_t sCapabilitiesCached;
    static uint16_t sYearOfHwCached;
};

extern "C" IGnssExt* HIDL_FETCH_IGnssExt(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSS_H
