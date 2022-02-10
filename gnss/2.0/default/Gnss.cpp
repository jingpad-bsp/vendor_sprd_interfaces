#include <vendor/sprd/hardware/gnss/2.0/types.h>
#include <log/log.h>
#include "Gnss.h"
#include "GnssConstants.h"
#include "GnssDebug.h"
#include <GnssBatching.h>

#include <GnssUtils.h>
#include <errno.h>
#include "GnssVisibilityControl.h"

#define LOG_TAG "GnssHAL_V_Gnss"
namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using GnssSvFlags = IGnssCallback::GnssSvFlags;

const uint32_t MIN_INTERVAL_MILLIS = 100;
std::vector<std::unique_ptr<ThreadFuncArgs>> Gnss::sThreadFuncArgsList;
sp<IGnssCallback> Gnss::sGnssCbIface = nullptr;
sp<IGnssCallback_1_1> Gnss::sGnssCbIface_1_1 = nullptr;
sp<IGnssCallback_2_0> Gnss::sGnssCbIface_2_0 = nullptr;

bool Gnss::sInterfaceExists = false;
bool Gnss::sWakelockHeldGnss = false;
bool Gnss::sWakelockHeldFused = false;

GpsCallbacks Gnss::sGnssCb = {
    .size = sizeof(GpsCallbacks),
    .location_cb = locationCb,
    .status_cb = statusCb,
    .sv_status_cb = gpsSvStatusCb,
    .nmea_cb = nmeaCb,
    .set_capabilities_cb = setCapabilitiesCb,
    .acquire_wakelock_cb = acquireWakelockCb,
    .release_wakelock_cb = releaseWakelockCb,
    .create_thread_cb = createThreadCb,
    .request_utc_time_cb = requestUtcTimeCb,
    .set_system_info_cb = setSystemInfoCb,
    .gnss_sv_status_cb = gnssSvStatusCb,
};

uint32_t Gnss::sCapabilitiesCached = 0;
uint16_t Gnss::sYearOfHwCached = 0;

Gnss::Gnss(gps_device_t* gnssDevice) :
        mDeathRecipient(new GnssHidlDeathRecipient(this)) {
    /* Error out if an instance of the interface already exists. */
    LOG_ALWAYS_FATAL_IF(sInterfaceExists);
    sInterfaceExists = true;
    ALOGE("%s: Enter", __func__);
    if (gnssDevice == nullptr) {
        ALOGE("%s: Invalid device_t handle", __func__);
        return;
    }

    mGnssIface = gnssDevice->get_gps_interface(gnssDevice);
}

Gnss::~Gnss() {
    sInterfaceExists = false;
    sThreadFuncArgsList.clear();
}
void Gnss::locationCb(GpsLocation* location) {
    if (location == nullptr) {
        ALOGE("%s: Invalid location from GNSS HAL", __func__);
        return;
    }

    if(sGnssCbIface != nullptr){
        android::hardware::gnss::V1_0::GnssLocation gnssLocation = convertToGnssLocation(location);
        auto ret = sGnssCbIface->gnssLocationCb(gnssLocation);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }

    if(sGnssCbIface_1_1 != nullptr){
        android::hardware::gnss::V1_0::GnssLocation gnssLocation = convertToGnssLocation(location);
        auto ret = sGnssCbIface_1_1->gnssLocationCb(gnssLocation);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }

    if(sGnssCbIface_2_0 != nullptr){
        android::hardware::gnss::V2_0::GnssLocation gnssLocation_2_0 = convertToGnssLocation_2_0(location);
        auto ret = sGnssCbIface_2_0->gnssLocationCb_2_0(gnssLocation_2_0);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }
}

void Gnss::statusCb(GpsStatus* gnssStatus) {
    if (gnssStatus == nullptr) {
        ALOGE("%s: Invalid GpsStatus from GNSS HAL", __func__);
        return;
    }

    IGnssCallback::GnssStatusValue status =
            static_cast<IGnssCallback::GnssStatusValue>(gnssStatus->status);

    if(sGnssCbIface != nullptr){
        auto ret = sGnssCbIface->gnssStatusCb(status);
        ALOGE("%s: GNSS Callback Interface called", __func__);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }

    if(sGnssCbIface_1_1 != nullptr){
        auto ret = sGnssCbIface_1_1->gnssStatusCb(status);
        ALOGE("%s: GNSS Callback_1_1 Interface called", __func__);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }

    if(sGnssCbIface_2_0 != nullptr){
        auto ret = sGnssCbIface_2_0->gnssStatusCb(status);
        ALOGE("%s: GNSS Callback_2_0 Interface called", __func__);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }
}

void Gnss::gnssSvStatusCb(GnssSvStatus* status) {
    if (status == nullptr) {
        ALOGE("Invalid status from GNSS HAL %s", __func__);
        return;
    }

    ::android::hardware::gnss::V1_0::IGnssCallback::IGnssCallback::GnssSvStatus svStatus;
    svStatus.numSvs = status->num_svs;

    if (svStatus.numSvs > static_cast<uint32_t>(GnssMax::SVS_COUNT)) {
        ALOGW("Too many satellites %zd. Clamps to %d.", svStatus.numSvs, GnssMax::SVS_COUNT);
        svStatus.numSvs = static_cast<uint32_t>(GnssMax::SVS_COUNT);
    }

    for (size_t i = 0; i < svStatus.numSvs; i++) {
        auto svInfo = status->gnss_sv_list[i];
        IGnssCallback::GnssSvInfo gnssSvInfo = {
            .svid = svInfo.svid,
            .constellation = static_cast<
                android::hardware::gnss::V1_0::GnssConstellationType>(
                svInfo.constellation),
            .cN0Dbhz = svInfo.c_n0_dbhz,
            .elevationDegrees = svInfo.elevation,
            .azimuthDegrees = svInfo.azimuth,
            // Older chipsets do not provide carrier frequency, hence
            // HAS_CARRIER_FREQUENCY flag and the carrierFrequencyHz fields
            // are not set. So we are resetting both fields here.
            .svFlag = static_cast<uint8_t>(svInfo.flags),
            .carrierFrequencyHz = svInfo.carrier_freq};
       	 svStatus.gnssSvList[i] = gnssSvInfo;
    }

    if(sGnssCbIface != nullptr){
        auto ret = sGnssCbIface->gnssSvStatusCb(svStatus);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }

    if(sGnssCbIface_1_1 != nullptr){
        auto ret = sGnssCbIface_1_1->gnssSvStatusCb(svStatus);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }

    if(sGnssCbIface_2_0 != nullptr){
        auto ret = sGnssCbIface_2_0->gnssSvStatusCb(svStatus);  //gnssSvStatusCb_2_0
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }
}

/*
 * This enum is used by gpsSvStatusCb() method below to convert GpsSvStatus
 * to GnssSvStatus for backward compatibility. It is only used by the default
 * implementation and is not part of the GNSS interface.
 */
enum SvidValues : uint16_t {
    GLONASS_SVID_OFFSET = 64,
    GLONASS_SVID_COUNT = 24,
    BEIDOU_SVID_OFFSET = 200,
    BEIDOU_SVID_COUNT = 35,
    SBAS_SVID_MIN = 33,
    SBAS_SVID_MAX = 64,
    SBAS_SVID_ADD = 87,
    QZSS_SVID_MIN = 193,
    QZSS_SVID_MAX = 200
};

/*
 * The following code that converts GpsSvStatus to GnssSvStatus is moved here from
 * GnssLocationProvider. GnssLocationProvider does not require it anymore since GpsSvStatus is
 * being deprecated and is no longer part of the GNSS interface.
 */
void Gnss::gpsSvStatusCb(GpsSvStatus* svInfo) {
    if (svInfo == nullptr) {
        ALOGE("Invalid status from GNSS HAL %s", __func__);
        return;
    }

    IGnssCallback::GnssSvStatus svStatus;
    svStatus.numSvs = svInfo->num_svs;
    /*
     * Clamp the list size since GnssSvStatus can support a maximum of
     * GnssMax::SVS_COUNT entries.
     */
    if (svStatus.numSvs > static_cast<uint32_t>(GnssMax::SVS_COUNT)) {
        ALOGW("Too many satellites %zd. Clamps to %d.", svStatus.numSvs, GnssMax::SVS_COUNT);
        svStatus.numSvs = static_cast<uint32_t>(GnssMax::SVS_COUNT);
    }

    uint32_t ephemerisMask = svInfo->ephemeris_mask;
    uint32_t almanacMask = svInfo->almanac_mask;
    uint32_t usedInFixMask = svInfo->used_in_fix_mask;
    /*
     * Conversion from GpsSvInfo to IGnssCallback::GnssSvInfo happens below.
     */
    for (size_t i = 0; i < svStatus.numSvs; i++) {
        IGnssCallback::GnssSvInfo& info = svStatus.gnssSvList[i];
        info.svid = svInfo->sv_list[i].prn;
        if (info.svid >= 1 && info.svid <= 32) {
            info.constellation = GnssConstellationType::GPS;
        } else if (info.svid > GLONASS_SVID_OFFSET &&
                   info.svid <= GLONASS_SVID_OFFSET + GLONASS_SVID_COUNT) {
            info.constellation = GnssConstellationType::GLONASS;
            info.svid -= GLONASS_SVID_OFFSET;
        } else if (info.svid > BEIDOU_SVID_OFFSET &&
                 info.svid <= BEIDOU_SVID_OFFSET + BEIDOU_SVID_COUNT) {
            info.constellation = GnssConstellationType::BEIDOU;
            info.svid -= BEIDOU_SVID_OFFSET;
        } else if (info.svid >= SBAS_SVID_MIN && info.svid <= SBAS_SVID_MAX) {
            info.constellation = GnssConstellationType::SBAS;
            info.svid += SBAS_SVID_ADD;
        } else if (info.svid >= QZSS_SVID_MIN && info.svid <= QZSS_SVID_MAX) {
            info.constellation = GnssConstellationType::QZSS;
        } else {
            ALOGD("Unknown constellation type with Svid = %d.", info.svid);
            info.constellation = GnssConstellationType::UNKNOWN;
        }

        info.cN0Dbhz = svInfo->sv_list[i].snr;
        info.elevationDegrees = svInfo->sv_list[i].elevation;
        info.azimuthDegrees = svInfo->sv_list[i].azimuth;
        // TODO: b/31702236
        info.svFlag = static_cast<uint8_t>(IGnssCallback::GnssSvFlags::NONE);

        /*
         * Only GPS info is valid for these fields, as these masks are just 32
         * bits, by GPS prn.
         */
        if (info.constellation == GnssConstellationType::GPS) {
            int32_t svidMask = (1 << (info.svid - 1));
            if ((ephemerisMask & svidMask) != 0) {
                info.svFlag |= IGnssCallback::GnssSvFlags::HAS_EPHEMERIS_DATA;
            }
            if ((almanacMask & svidMask) != 0) {
                info.svFlag |= IGnssCallback::GnssSvFlags::HAS_ALMANAC_DATA;
            }
            if ((usedInFixMask & svidMask) != 0) {
                info.svFlag |= IGnssCallback::GnssSvFlags::USED_IN_FIX;
            }
        }
    }

    if(sGnssCbIface != nullptr){
    auto ret = sGnssCbIface->gnssSvStatusCb(svStatus);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }
    if(sGnssCbIface_1_1 != nullptr){
    auto ret = sGnssCbIface_1_1->gnssSvStatusCb(svStatus);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }
    if(sGnssCbIface_2_0 != nullptr){
    auto ret = sGnssCbIface_2_0->gnssSvStatusCb(svStatus);  //gnssSvStatusCb_2_0
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }
}

void Gnss::nmeaCb(GpsUtcTime timestamp, const char* nmea, int length) {
    android::hardware::hidl_string nmeaString;
    nmeaString.setToExternal(nmea, length);
    if(sGnssCbIface != nullptr){
    auto ret = sGnssCbIface->gnssNmeaCb(timestamp, nmeaString);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }
    if(sGnssCbIface_1_1 != nullptr){
    auto ret = sGnssCbIface_1_1->gnssNmeaCb(timestamp, nmeaString);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }
    if(sGnssCbIface_2_0 != nullptr){
    auto ret = sGnssCbIface_2_0->gnssNmeaCb(timestamp, nmeaString);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }
}

void Gnss::setCapabilitiesCb(uint32_t capabilities) {
    if(sGnssCbIface != nullptr){
    auto ret = sGnssCbIface->gnssSetCapabilitesCb(capabilities);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }
    if(sGnssCbIface_1_1 != nullptr){
    auto ret = sGnssCbIface_1_1->gnssSetCapabilitesCb(capabilities);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }
    if(sGnssCbIface_2_0 != nullptr){
    auto ret = sGnssCbIface_2_0->gnssSetCapabilitesCb(capabilities);    //gnssSetCapabilitiesCb_2_0
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }
    // Save for reconnection when some legacy hal's don't resend this info
    sCapabilitiesCached = capabilities;
}

void Gnss::acquireWakelockCb() {
    acquireWakelockGnss();
}

void Gnss::releaseWakelockCb() {
    releaseWakelockGnss();
}


void Gnss::acquireWakelockGnss() {
    sWakelockHeldGnss = true;
    updateWakelock();
}

void Gnss::releaseWakelockGnss() {
    sWakelockHeldGnss = false;
    updateWakelock();
}

void Gnss::acquireWakelockFused() {
    sWakelockHeldFused = true;
    updateWakelock();
}

void Gnss::releaseWakelockFused() {
    sWakelockHeldFused = false;
    updateWakelock();
}

void Gnss::updateWakelock() {
    // Track the state of the last request - in case the wake lock in the layer above is reference
    // counted.
    static bool sWakelockHeld = false;

    if (sWakelockHeldGnss || sWakelockHeldFused) {
        if (!sWakelockHeld) {
            ALOGI("%s: GNSS HAL Wakelock acquired due to gps: %d, fused: %d", __func__,
                    sWakelockHeldGnss, sWakelockHeldFused);
            sWakelockHeld = true;

            if(sGnssCbIface != nullptr){
            auto ret = sGnssCbIface->gnssAcquireWakelockCb();
                if (!ret.isOk()) {
                    ALOGE("%s: Unable to invoke callback", __func__);
                }
            }
            if(sGnssCbIface_1_1 != nullptr){
            auto ret = sGnssCbIface_1_1->gnssAcquireWakelockCb();
                if (!ret.isOk()) {
                    ALOGE("%s: Unable to invoke callback_1_1", __func__);
                }
            }
            if(sGnssCbIface_2_0 != nullptr){
            auto ret = sGnssCbIface_2_0->gnssAcquireWakelockCb();
                if (!ret.isOk()) {
                    ALOGE("%s: Unable to invoke callback_2_0", __func__);
                }
            }
        }
    } else {
        if (sWakelockHeld) {
            ALOGI("%s: GNSS HAL Wakelock released", __func__);
        } else  {
            // To avoid burning power, always release, even if logic got here with sWakelock false
            // which it shouldn't, unless underlying *.h implementation makes duplicate requests.
            ALOGW("%s: GNSS HAL Wakelock released, duplicate request", __func__);
        }
        sWakelockHeld = false;

        if(sGnssCbIface != nullptr){
        auto ret = sGnssCbIface->gnssReleaseWakelockCb();
            if (!ret.isOk()) {
                ALOGE("%s: Unable to invoke callback", __func__);
            }
        }
        if(sGnssCbIface_1_1 != nullptr){
        auto ret = sGnssCbIface_1_1->gnssReleaseWakelockCb();
            if (!ret.isOk()) {
                ALOGE("%s: Unable to invoke callback_1_1", __func__);
            }
        }
        if(sGnssCbIface_2_0 != nullptr){
        auto ret = sGnssCbIface_2_0->gnssReleaseWakelockCb();
            if (!ret.isOk()) {
                ALOGE("%s: Unable to invoke callback_2_0", __func__);
            }
        }
    }
}

void Gnss::requestUtcTimeCb() {
    if(sGnssCbIface != nullptr){
    auto ret = sGnssCbIface->gnssRequestTimeCb();
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }
    if(sGnssCbIface_1_1 != nullptr){
    auto ret = sGnssCbIface_1_1->gnssRequestTimeCb();
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }
    if(sGnssCbIface_2_0 != nullptr){
    auto ret = sGnssCbIface_2_0->gnssRequestTimeCb();
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }
}

pthread_t Gnss::createThreadCb(const char* name, void (*start)(void*), void* arg) {
    return createPthread(name, start, arg, &sThreadFuncArgsList);
}

void Gnss::setSystemInfoCb(const LegacyGnssSystemInfo* info) {
    if (info == nullptr) {
        ALOGE("Invalid GnssSystemInfo from GNSS HAL %s", __func__);
        return;
    }

    IGnssCallback::GnssSystemInfo gnssInfo = {
        .yearOfHw = info->year_of_hw
    };

    if(sGnssCbIface != nullptr){
    auto ret = sGnssCbIface->gnssSetSystemInfoCb(gnssInfo);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback", __func__);
        }
    }
    if(sGnssCbIface_1_1 != nullptr){
    auto ret = sGnssCbIface_1_1->gnssSetSystemInfoCb(gnssInfo);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_1_1", __func__);
        }
    }
    if(sGnssCbIface_2_0 != nullptr){
    auto ret = sGnssCbIface_2_0->gnssSetSystemInfoCb(gnssInfo);
        if (!ret.isOk()) {
            ALOGE("%s: Unable to invoke callback_2_0", __func__);
        }
    }

    // Save for reconnection when some legacy hal's don't resend this info
    sYearOfHwCached = info->year_of_hw;
}
// Methods from ::android::hardware::gnss::V1_0::IGnss follow.
Return<bool> Gnss::setCallback(const sp<::android::hardware::gnss::V1_0::IGnssCallback>& callback) {
    ALOGI("%s: enter", __func__);
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return false;
    }

    if (callback == nullptr)  {
        ALOGE("%s: Null callback ignored", __func__);
        return false;
    }

    if (sGnssCbIface != NULL) {
        ALOGW("%s called more than once. Unexpected unless test.", __func__);
        sGnssCbIface->unlinkToDeath(mDeathRecipient);
    }

    sGnssCbIface = callback;
    callback->linkToDeath(mDeathRecipient, 0 /*cookie*/);

    // If this was received in the past, send it up again to refresh caller.
    // mGnssIface will override after init() is called below, if needed
    // (though it's unlikely the gps.h capabilities or system info will change.)
    if (sCapabilitiesCached != 0) {
        setCapabilitiesCb(sCapabilitiesCached);
    }
    if (sYearOfHwCached != 0) {
        LegacyGnssSystemInfo info;
        info.year_of_hw = sYearOfHwCached;
        setSystemInfoCb(&info);
    }

    return (mGnssIface->init(&sGnssCb) == 0);
}

Return<bool> Gnss::start() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return false;
    }
    if (mIsActive) {
        ALOGW("Gnss has started. Restarting...");
        stop();
	}

    mIsActive = true;
    return (mGnssIface->start() == 0);
}

Return<bool> Gnss::stop() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return false;
    }
    ALOGW("gps_stop entry");
    mIsActive = false;
    return (mGnssIface->stop() == 0);
}

Return<void> Gnss::cleanup() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
    } else {
        mGnssIface->cleanup();
    }
    return Void();
}

Return<bool> Gnss::injectTime(int64_t timeMs, int64_t timeReferenceMs, int32_t uncertaintyMs) {
    // TODO implement
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return false;
    }

    return (mGnssIface->inject_time(timeMs, timeReferenceMs, uncertaintyMs) == 0);
}

Return<bool> Gnss::injectLocation(double latitudeDegrees,
                                double longitudeDegrees,
                                float accuracyMeters) {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return false;
    }

    return (mGnssIface->inject_location(latitudeDegrees, longitudeDegrees, accuracyMeters) == 0);
}

Return<void> Gnss::deleteAidingData(::android::hardware::gnss::V1_0::IGnss::GnssAidingData aidingDataFlags) {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
    } else {
        mGnssIface->delete_aiding_data(static_cast<GpsAidingData>(aidingDataFlags));
    }
    return Void();
}

Return<bool> Gnss::setPositionMode(::android::hardware::gnss::V1_0::IGnss::GnssPositionMode mode,
    ::android::hardware::gnss::V1_0::IGnss::GnssPositionRecurrence recurrence,
    uint32_t minIntervalMs,
    uint32_t preferredAccuracyMeters,
    uint32_t preferredTimeMs) {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return false;
    }

    return (mGnssIface->set_position_mode(static_cast<GpsPositionMode>(mode),
                                          static_cast<GpsPositionRecurrence>(recurrence),
                                          minIntervalMs,
                                          preferredAccuracyMeters,
                                          preferredTimeMs) == 0);
}

Return<sp<::android::hardware::gnss::V1_0::IAGnssRil>> Gnss::getExtensionAGnssRil() {
    // TODO implement
    return ::android::sp<::android::hardware::gnss::V1_0::IAGnssRil> {};
}

Return<sp<::android::hardware::gnss::V1_0::IGnssGeofencing>> Gnss::getExtensionGnssGeofencing() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssGeofencingIface == nullptr) {
        const GpsGeofencingInterface* gpsGeofencingIface =
                static_cast<const GpsGeofencingInterface*>(
                        mGnssIface->get_extension(GPS_GEOFENCING_INTERFACE));

        if (gpsGeofencingIface == nullptr) {
            ALOGE("%s GnssGeofencing interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssGeofencingIface = new GnssGeofencing(gpsGeofencingIface);
        }
    }

    return mGnssGeofencingIface;
}

Return<sp<::android::hardware::gnss::V1_0::IAGnss>> Gnss::getExtensionAGnss() {
#if 0
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mAGnssIface == nullptr) {
        const AGpsInterface* agpsIface = static_cast<const AGpsInterface*>(
                mGnssIface->get_extension(AGPS_INTERFACE));
        if (agpsIface == nullptr) {
            ALOGE("%s AGnss interface not implemented by GNSS HAL", __func__);
        } else {
            mAGnssIface = new AGnss(agpsIface);
        }
    }
    return mAGnssIface;
#else
	return sp<::android::hardware::gnss::V1_0::IAGnss>{};
#endif
}

Return<sp<::android::hardware::gnss::V1_0::IGnssNi>> Gnss::getExtensionGnssNi() {
    ALOGE("%s: entry and deprecated", __func__);
    return nullptr;
}

Return<sp<::android::hardware::gnss::V1_0::IGnssMeasurement>> Gnss::getExtensionGnssMeasurement() {
    ALOGE("%s: entry", __func__);
    if (mGnssIface == nullptr) {
        ALOGE("%s: mGnssIface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssMeasurement == nullptr) {
        ALOGE("%s: mGnssMeasurement is unavailable", __func__);
        const GpsMeasurementInterface* gpsMeasurementIface =
                static_cast<const GpsMeasurementInterface*>(
                        mGnssIface->get_extension(GPS_MEASUREMENT_INTERFACE));

        if (gpsMeasurementIface == nullptr) {
            ALOGE("%s gpsMeasurementIface interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssMeasurement = new GnssMeasurement(gpsMeasurementIface);
            ALOGE("%s Get mGnssMeasurement", __func__);
        }
    }
    return mGnssMeasurement;

}

Return<sp<::android::hardware::gnss::V1_0::IGnssNavigationMessage>> Gnss::getExtensionGnssNavigationMessage() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssNavigationMessage == nullptr) {
        const GpsNavigationMessageInterface* gpsNavigationMessageIface =
                static_cast<const GpsNavigationMessageInterface*>(
                        mGnssIface->get_extension(GPS_NAVIGATION_MESSAGE_INTERFACE));

        if (gpsNavigationMessageIface == nullptr) {
            ALOGE("%s GnssNavigationMessage interface not implemented by GNSS HAL",
                  __func__);
        } else {
            mGnssNavigationMessage = new GnssNavigationMessage(gpsNavigationMessageIface);
        }
    }

    return mGnssNavigationMessage;
}

Return<sp<::android::hardware::gnss::V1_0::IGnssXtra>> Gnss::getExtensionXtra() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssXtraIface == nullptr) {
        const GpsXtraInterface* gpsXtraIface = static_cast<const GpsXtraInterface*>(
                mGnssIface->get_extension(GPS_XTRA_INTERFACE));

        if (gpsXtraIface == nullptr) {
            ALOGE("%s GnssXtra interface not implemented by HAL", __func__);
        } else {
            mGnssXtraIface = new GnssXtra(gpsXtraIface);
        }
    }

    return mGnssXtraIface;
}

Return<sp<::android::hardware::gnss::V1_0::IGnssConfiguration>> Gnss::getExtensionGnssConfiguration() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssConfig == nullptr) {
        const GnssConfigurationInterface* gnssConfigIface =
                static_cast<const GnssConfigurationInterface*>(
                        mGnssIface->get_extension(GNSS_CONFIGURATION_INTERFACE));

        if (gnssConfigIface == nullptr) {
            ALOGE("%s GnssConfiguration interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssConfig = new GnssConfiguration(gnssConfigIface);
        }
    }
    return mGnssConfig;
}

Return<sp<::android::hardware::gnss::V1_0::IGnssDebug>> Gnss::getExtensionGnssDebug() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssDebug == nullptr) {
        const GpsDebugInterface* gpsDebugIface = static_cast<const GpsDebugInterface*>(
                mGnssIface->get_extension(GPS_DEBUG_INTERFACE));

        if (gpsDebugIface == nullptr) {
            ALOGE("%s: GnssDebug interface is not implemented by HAL", __func__);
        } else {
            mGnssDebug = new GnssDebug(gpsDebugIface);
        }
    }

    return mGnssDebug;
}

Return<sp<::android::hardware::gnss::V2_0::IGnssDebug>> Gnss::getExtensionGnssDebug_2_0() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssDebug == nullptr) {
        const GpsDebugInterface* gpsDebugIface = static_cast<const GpsDebugInterface*>(
                mGnssIface->get_extension(GPS_DEBUG_INTERFACE));

        if (gpsDebugIface == nullptr) {
            ALOGE("%s: GnssDebug interface is not implemented by HAL", __func__);
        } else {
            mGnssDebug = new GnssDebug(gpsDebugIface);
        }
    }

    return mGnssDebug;
}

Return<sp<::android::hardware::gnss::V1_0::IGnssBatching>> Gnss::getExtensionGnssBatching() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssBatching == nullptr) {
        hw_module_t* module;
        const FlpLocationInterface* flpLocationIface = nullptr;
        int err = hw_get_module(FUSED_LOCATION_HARDWARE_MODULE_ID, (hw_module_t const**)&module);

        if (err != 0) {
            ALOGE("gnss flp hw_get_module failed: %d, err is %s", err, strerror(err));
        } else if (module == nullptr) {
            ALOGE("Fused Location hw_get_module returned null module");
        } else if (module->methods == nullptr) {
            ALOGE("Fused Location hw_get_module returned null methods");
        } else {
            hw_device_t* device;
            err = module->methods->open(module, FUSED_LOCATION_HARDWARE_MODULE_ID, &device);
            if (err != 0) {
                ALOGE("flpDevice open failed: %d", err);
            } else {
                flp_device_t * flpDevice = reinterpret_cast<flp_device_t*>(device);
                flpLocationIface = flpDevice->get_flp_interface(flpDevice);
            }
        }

        if (flpLocationIface == nullptr) {
            ALOGE("%s: GnssBatching interface is not implemented by HAL", __func__);
        } else {
            mGnssBatching = new GnssBatching(flpLocationIface);
        }
    }
    return mGnssBatching;
}

Return<sp<::android::hardware::gnss::V2_0::IGnssBatching>> Gnss::getExtensionGnssBatching_2_0() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssBatching == nullptr) {
        hw_module_t* module;
        const FlpLocationInterface* flpLocationIface = nullptr;
        int err = hw_get_module(FUSED_LOCATION_HARDWARE_MODULE_ID, (hw_module_t const**)&module);

        if (err != 0) {
            ALOGE("gnss flp hw_get_module failed: %d, err is %s", err, strerror(err));
        } else if (module == nullptr) {
            ALOGE("Fused Location hw_get_module returned null module");
        } else if (module->methods == nullptr) {
            ALOGE("Fused Location hw_get_module returned null methods");
        } else {
            hw_device_t* device;
            err = module->methods->open(module, FUSED_LOCATION_HARDWARE_MODULE_ID, &device);
            if (err != 0) {
                ALOGE("flpDevice open failed: %d", err);
            } else {
                flp_device_t * flpDevice = reinterpret_cast<flp_device_t*>(device);
                flpLocationIface = flpDevice->get_flp_interface(flpDevice);
            }
        }

        if (flpLocationIface == nullptr) {
            ALOGE("%s: GnssBatching interface is not implemented by HAL", __func__);
        } else {
            mGnssBatching = new GnssBatching(flpLocationIface);
        }
    }
    return mGnssBatching;
}


// Methods from ::android::hardware::gnss::V1_1::IGnss follow.
Return<bool> Gnss::setCallback_1_1(const sp<::android::hardware::gnss::V1_1::IGnssCallback>& callback) {
    ALOGI("%s: enter.", __func__);

    if (callback == nullptr) {
        ALOGE("%s: Null callback ignored", __func__);
        return false;
    }
    sGnssCbIface_1_1 = callback;
    ALOGE("%s: sCapabilitiesCached =0x%x", __func__,sCapabilitiesCached);
    auto ret = callback->gnssSetCapabilitesCb(sCapabilitiesCached);
    if (!ret.isOk()) {
        ALOGE("%s: gnssSetCapabilitesCb failed", __func__);
    }

    IGnssCallback::GnssSystemInfo gnssInfo = {.yearOfHw = 2018};

    ret = callback->gnssSetSystemInfoCb(gnssInfo);
    if (!ret.isOk()) {
        ALOGE("%s:gnssSetSystemInfoCb failed", __func__);
    }

    auto gnssName = "Google Mock GNSS Implementation v1.1";
    ret = callback->gnssNameCb(gnssName);
    if (!ret.isOk()) {
        ALOGE("%s:gnssNameCb failed", __func__);
    }
    return (mGnssIface->init(&sGnssCb) == 0);
}

Return<bool> Gnss::setPositionMode_1_1(
::android::hardware::gnss::V1_0::IGnss::GnssPositionMode mode,
::android::hardware::gnss::V1_0::IGnss::GnssPositionRecurrence recurrence, uint32_t minIntervalMs,
uint32_t preferredAccuracyMeters, uint32_t preferredTimeMs, bool lowPowerMode) {
    mMinIntervalMs = (minIntervalMs < MIN_INTERVAL_MILLIS) ? MIN_INTERVAL_MILLIS : minIntervalMs;
	mGnssIface->set_position_mode(static_cast<GpsPositionMode>(mode),
                                          static_cast<GpsPositionRecurrence>(recurrence),
                                          mMinIntervalMs,
                                          preferredAccuracyMeters,
                                          preferredTimeMs);
	return true;
}

Return<sp<::android::hardware::gnss::V1_1::IGnssConfiguration>>
Gnss::getExtensionGnssConfiguration_1_1() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }
    ALOGE("%s: entry", __func__);
    if (mGnssConfig == nullptr) {
        const GnssConfigurationInterface* gnssConfigIface =
                static_cast<const GnssConfigurationInterface*>(
                        mGnssIface->get_extension(GNSS_CONFIGURATION_INTERFACE));

        if (gnssConfigIface == nullptr) {
            ALOGE("%s GnssConfiguration interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssConfig = new GnssConfiguration(gnssConfigIface);
        }
    }
    return mGnssConfig;
}

Return<sp<::android::hardware::gnss::V1_1::IGnssMeasurement>> 
Gnss::getExtensionGnssMeasurement_1_1() {
    ALOGE("%s: entry", __func__);
    if (mGnssIface == nullptr) {
        ALOGE("%s: mGnssIface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssMeasurement == nullptr) {
        ALOGE("%s: mGnssMeasurement is unavailable", __func__);
        const GpsMeasurementInterface* gpsMeasurementIface =
                static_cast<const GpsMeasurementInterface*>(
                        mGnssIface->get_extension(GPS_MEASUREMENT_INTERFACE));

        if (gpsMeasurementIface == nullptr) {
            ALOGE("%s gpsMeasurementIface interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssMeasurement = new GnssMeasurement(gpsMeasurementIface);
            ALOGE("%s Get mGnssMeasurement", __func__);
        }
    }
    return mGnssMeasurement;
}

Return<bool> Gnss::injectBestLocation(const ::android::hardware::gnss::V1_0::GnssLocation& location) {
    return true;
}


// Methods from ::android::hardware::gnss::V2_0::IGnss follow.
Return<bool> Gnss::setCallback_2_0(const sp<::android::hardware::gnss::V2_0::IGnssCallback>& callback) {
    if (callback == nullptr) {
        ALOGE("%s: Null callback ignored", __func__);
        return false;
    }
    sGnssCbIface_2_0 = callback;
    ALOGE("%s:  sCapabilitiesCached =0x%x", __func__,sCapabilitiesCached);
    auto ret = callback->gnssSetCapabilitesCb(sCapabilitiesCached);
    if (!ret.isOk()) {
        ALOGE("%s: gnssSetCapabilitesCb failed", __func__);
    }

    IGnssCallback::GnssSystemInfo gnssInfo = {.yearOfHw = 2018};

    ret = callback->gnssSetSystemInfoCb(gnssInfo);
    if (!ret.isOk()) {
        ALOGE("%s:gnssSetSystemInfoCb failed", __func__);
    }

    auto gnssName = "Google Mock GNSS Implementation v1.1";
    ret = callback->gnssNameCb(gnssName);
    if (!ret.isOk()) {
        ALOGE("%s:gnssNameCb failed", __func__);
    }
    return (mGnssIface->init(&sGnssCb) == 0);
}

Return<sp<::android::hardware::gnss::V2_0::IGnssConfiguration>> Gnss::getExtensionGnssConfiguration_2_0() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }
    ALOGE("%s: entry", __func__);
    if (mGnssConfig == nullptr) {
        const GnssConfigurationInterface* gnssConfigIface =
                static_cast<const GnssConfigurationInterface*>(
                        mGnssIface->get_extension(GNSS_CONFIGURATION_INTERFACE));

        if (gnssConfigIface == nullptr) {
            ALOGE("%s GnssConfiguration interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssConfig = new GnssConfiguration(gnssConfigIface);
            ALOGE("%s Get GnssConfiguration", __func__);
        }
    }
    return mGnssConfig;
}

Return<sp<::android::hardware::gnss::V2_0::IAGnss>> Gnss::getExtensionAGnss_2_0() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    if (mAGnssIface == nullptr) {
        const AGpsInterface* agpsIface = static_cast<const AGpsInterface*>(
                mGnssIface->get_extension(AGPS_INTERFACE));
        if (agpsIface == nullptr) {
            ALOGE("%s AGnss interface not implemented by GNSS HAL", __func__);
        } else {
            mAGnssIface = new AGnss(agpsIface);
        }
    }
    return mAGnssIface;
}

Return<sp<::android::hardware::gnss::V2_0::IAGnssRil>> Gnss::getExtensionAGnssRil_2_0() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    ALOGE("%s: enter", __func__);
    if (mGnssRil == nullptr) {
        const AGpsRilInterface* agpsRilIface = static_cast<const AGpsRilInterface*>(
                mGnssIface->get_extension(AGPS_RIL_INTERFACE));
        if (agpsRilIface == nullptr) {
            ALOGE("%s GnssRil interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssRil = new AGnssRil(agpsRilIface);
            ALOGE("%s GnssRil interface implemented by GNSS HAL", __func__);
        }
    }
    return mGnssRil;
}

Return<sp<::android::hardware::gnss::V2_0::IGnssMeasurement>> Gnss::getExtensionGnssMeasurement_2_0() {
    ALOGE("%s: entry", __func__);
    if (mGnssIface == nullptr) {
        ALOGE("%s: mGnssIface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssMeasurement == nullptr) {
        ALOGE("%s: mGnssMeasurement is unavailable", __func__);
        const GpsMeasurementInterface* gpsMeasurementIface =
                static_cast<const GpsMeasurementInterface*>(
                        mGnssIface->get_extension(GPS_MEASUREMENT_INTERFACE));

        if (gpsMeasurementIface == nullptr) {
            ALOGE("%s gpsMeasurementIface interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssMeasurement = new GnssMeasurement(gpsMeasurementIface);
            ALOGE("%s Get mGnssMeasurement", __func__);
        }
    }
    return mGnssMeasurement;
}

Return<sp<::android::hardware::gnss::measurement_corrections::V1_0::IMeasurementCorrections>> Gnss::getExtensionMeasurementCorrections() {
    ALOGE("%s: entry", __func__);
    if (mGnssIface == nullptr) {
        ALOGE("%s: mGnssIface is unavailable", __func__);
        return nullptr;
    }

    if (mMeasurementCorrections == nullptr) {
        ALOGE("%s: mMeasurementCorrections is unavailable", __func__);
      const GpsMeasureCorrectionsInterface* gpsMeasureCorrectionsIface =
                static_cast<const GpsMeasureCorrectionsInterface*>(
                        mGnssIface->get_extension(GNSS_MEASUREMENT_CORRECTIONS_INTERFACE));

        if (gpsMeasureCorrectionsIface == nullptr) {
            ALOGE("%s gpsMeasureCorrectionsIface interface not implemented by GNSS HAL", __func__);
        } else {
            mMeasurementCorrections = new GnssMeasurementCorrections(gpsMeasureCorrectionsIface);
            ALOGE("%s Get mMeasurementCorrections", __func__);
        }
    }
    return mMeasurementCorrections;
}

Return<sp<::android::hardware::gnss::visibility_control::V1_0::IGnssVisibilityControl>> Gnss::getExtensionVisibilityControl() {
    ALOGE("%s: entry", __func__);
    if (mGnssIface == nullptr) {
        ALOGE("%s: mGnssIface is unavailable", __func__);
        return nullptr;
    }

    if (mGnssVisibilityControl  == nullptr) {
        ALOGE("%GpsVisibilityControlInterface: mGnssVisibilityControl  is unavailable", __func__);
      const GpsVisibilityControlInterface* gpsVisibilityControlIface =
                static_cast<const GpsVisibilityControlInterface*>(
                        mGnssIface->get_extension(GNSS_VISIBILITY_CONTROL_INTERFACE));

        if (gpsVisibilityControlIface == nullptr) {
            ALOGE("%s gpsVisibilityControlIface interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssVisibilityControl = new GnssVisibilityControl(gpsVisibilityControlIface);
            ALOGE("%s Get mGnssVisibilityControl", __func__);
        }
    }
    return mGnssVisibilityControl;
}

Return<bool> Gnss::injectBestLocation_2_0(const ::android::hardware::gnss::V2_0::GnssLocation& location) {
    int ret = -1;
    GnssLocationEx*  GnssLocations;

    ALOGE("%s: entry", __func__);
    if (mGnssIface == nullptr) {
        ALOGE("%s: mGnssIface is unavailable", __func__);
        return false;
    }

    GnssLocations = (GnssLocationEx*)malloc(sizeof(GnssLocationEx));
    if(GnssLocations == nullptr)
    {
        ALOGE("%s: GnssLocations malloc failed", __func__);
    }
    else
    {
        memset(GnssLocations, 0, sizeof(GnssLocationEx));
        memcpy(GnssLocations, &location, sizeof(GnssLocationEx));
        ret = mGnssIface->inject_best_location(GnssLocations);
#ifdef GNSS_HIDL_DEBUG
        ALOGD("%s, ret=%d, latitude=%f, longitude=%f, timestampNs=%ld",
                __func__,
                ret,
                GnssLocations->latitude,
                GnssLocations->longitude,
                GnssLocations->elapsedRealtime.timestampNs);
#endif
        free(GnssLocations);
        if(ret == 0) {
            return true;
        }
    }
    return false;
}

Return<sp<::vendor::sprd::hardware::gnss::V2_0::IGnssUnisocExt>>
Gnss::getExtensionUnisocExt() {
    if (mGnssIface == nullptr) {
        ALOGE("%s: Gnss interface is unavailable", __func__);
        return nullptr;
    }

    ALOGE("%s: enter", __func__);
    if (mGnssUnisocExt == nullptr) {
        const GnssUnisocExtInterface* gnssUnisocExtIface = static_cast<const GnssUnisocExtInterface*>(
                mGnssIface->get_extension(GNSS_UNISOC_EXT_INTERFACE));
        if (gnssUnisocExtIface == nullptr) {
            ALOGE("%s interface not implemented by GNSS HAL", __func__);
        } else {
            mGnssUnisocExt = new GnssUnisocExt(gnssUnisocExtIface);
        }
    }
    return mGnssUnisocExt;
}

void Gnss::handleHidlDeath() {
    ALOGW("GNSS service noticed HIDL death. Stopping all GNSS operations.");

    // commands down to the HAL implementation
    stop(); // stop ongoing GPS tracking
    if (mGnssMeasurement != nullptr) {
        mGnssMeasurement->close();
    }
    if (mGnssNavigationMessage != nullptr) {
        mGnssNavigationMessage->close();
    }
    if (mGnssBatching != nullptr) {
        mGnssBatching->stop();
        mGnssBatching->cleanup();
    }
    cleanup();

    /*
     * This has died, so close it off in case (race condition) callbacks happen
     * before HAL processes above messages.
     */
    if(sGnssCbIface != nullptr){
        sGnssCbIface = nullptr;
    }
    if(sGnssCbIface_1_1 != nullptr){
        sGnssCbIface_1_1 = nullptr;
    }
    if(sGnssCbIface_2_0 != nullptr){
        sGnssCbIface_2_0 = nullptr;
    }
}

IGnss* HIDL_FETCH_IGnss(const char* /* hal */) {
    hw_module_t* module;
    IGnss* iface = nullptr;
    int err = hw_get_module(GPS_HARDWARE_MODULE_ID, (hw_module_t const**)&module);

    if (err == 0) {
        hw_device_t* device;
        err = module->methods->open(module, GPS_HARDWARE_MODULE_ID, &device);
        if (err == 0) {
            iface = new Gnss(reinterpret_cast<gps_device_t*>(device));
        } else {
            ALOGE("gnssDevice open %s failed: %d", GPS_HARDWARE_MODULE_ID, err);
        }
    } else {
      ALOGE("gnss hw_get_module %s failed: %d", GPS_HARDWARE_MODULE_ID, err);
    }
    return iface;
}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
