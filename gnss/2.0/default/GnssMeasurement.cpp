#define LOG_TAG "GnssHAL_V_GnssMeasurement"
#define TWO_INIT
#include "GnssMeasurement.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

sp<IGnssMeasurementCallback> GnssMeasurement::sGnssMeasureCbIface = nullptr;
sp<IGnssMeasurementCallback_1_1> GnssMeasurement::sGnssMeasureCbIface_1_1 = nullptr;
sp<IGnssMeasurementCallback_2_0> GnssMeasurement::sGnssMeasureCbIface_2_0 = nullptr;

GpsMeasurementCallbacks GnssMeasurement::sGnssMeasurementCbs = {
    .size = sizeof(GpsMeasurementCallbacks),
    .measurement_callback = gpsMeasurementCb,
    .gnss_measurement_callback = gnssMeasurementCbs
};

#ifdef MOCK
GnssMeasurement::GnssMeasurement(const GpsMeasurementInterface* gpsMeasurementIface)
    : mMinIntervalMillis(1000) {}
#else
GnssMeasurement::GnssMeasurement(const GpsMeasurementInterface* gpsMeasurementIface)
    : mGnssMeasureIface(gpsMeasurementIface)  {}
#endif
void GnssMeasurement::gnssMeasurementCb(LegacyGnssData* legacyGnssData) {

    if (legacyGnssData == nullptr) {
        ALOGE("%s: Invalid GnssData from GNSS HAL", __func__);
        return;
    }

    if ((sGnssMeasureCbIface != nullptr) ||  (sGnssMeasureCbIface_1_1 != nullptr)){
        ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData gnssData;
        gnssData.measurementCount = std::min(legacyGnssData->measurement_count,
                                             static_cast<size_t>(::android::hardware::gnss::V1_0::GnssMax::SVS_COUNT));

        using ::android::hardware::gnss::V1_0::IGnssMeasurementCallback;
        for (size_t i = 0; i < gnssData.measurementCount; i++) {
            auto entry = legacyGnssData->measurements[i];
            auto state = static_cast<GnssMeasurementState>(entry.state);
            if (state & IGnssMeasurementCallback::GnssMeasurementState::STATE_TOW_DECODED) {
              state |= IGnssMeasurementCallback::GnssMeasurementState::STATE_TOW_KNOWN;
            }
            if (state & IGnssMeasurementCallback::GnssMeasurementState::STATE_GLO_TOD_DECODED) {
              state |= IGnssMeasurementCallback::GnssMeasurementState::STATE_GLO_TOD_KNOWN;
            }
            gnssData.measurements[i] = {
                .flags = entry.flags,
                .svid = entry.svid,
                .constellation = static_cast<GnssConstellationType>(entry.constellation),
                .timeOffsetNs = entry.time_offset_ns,
                .state = state,
                .receivedSvTimeInNs = entry.received_sv_time_in_ns,
                .receivedSvTimeUncertaintyInNs = entry.received_sv_time_uncertainty_in_ns,
                .cN0DbHz = entry.c_n0_dbhz,
                .pseudorangeRateMps = entry.pseudorange_rate_mps,
                .pseudorangeRateUncertaintyMps = entry.pseudorange_rate_uncertainty_mps,
                .accumulatedDeltaRangeState = entry.accumulated_delta_range_state,
                .accumulatedDeltaRangeM = entry.accumulated_delta_range_m,
                .accumulatedDeltaRangeUncertaintyM = entry.accumulated_delta_range_uncertainty_m,
                .carrierFrequencyHz = entry.carrier_frequency_hz,
                .carrierCycles = entry.carrier_cycles,
                .carrierPhase = entry.carrier_phase,
                .carrierPhaseUncertainty = entry.carrier_phase_uncertainty,
                .multipathIndicator = static_cast<IGnssMeasurementCallback::GnssMultipathIndicator>(
                        entry.multipath_indicator),
                .snrDb = entry.snr_db,
                .agcLevelDb    = entry.agcLevelDb
            };
            ALOGD("%s: 2.0  gnssData.measurements[%d].flags = %d", __func__,i, gnssData.measurements[i].flags);
            ALOGD("%s: 2.0  gnssData.measurements[%d].svid = %d", __func__,i, gnssData.measurements[i].svid);
            ALOGD("%s: 2.0  gnssData.measurements[%d].constellation = %d", __func__,i, gnssData.measurements[i].constellation);
            ALOGD("%s: 2.0  gnssData.measurements[%d].timeOffsetNs = %f", __func__,i, gnssData.measurements[i].timeOffsetNs);
            ALOGD("%s: 2.0  gnssData.measurements[%d].cN0DbHz = %f", __func__,i, gnssData.measurements[i].cN0DbHz);
            ALOGD("%s: 2.0  gnssData.measurements[%d].snrDb = %f", __func__,i, gnssData.measurements[i].snrDb);
            ALOGD("%s: 2.0  gnssData.measurements[%d].agcLevelDb = %f", __func__,i, gnssData.measurements[i].agcLevelDb);
        }

        auto clockVal = legacyGnssData->clock;
        gnssData.clock = {
            .gnssClockFlags = clockVal.flags,
            .leapSecond = clockVal.leap_second,
            .timeNs = clockVal.time_ns,
            .timeUncertaintyNs = clockVal.time_uncertainty_ns,
            .fullBiasNs = clockVal.full_bias_ns,
            .biasNs = clockVal.bias_ns,
            .biasUncertaintyNs = clockVal.bias_uncertainty_ns,
            .driftNsps = clockVal.drift_nsps,
            .driftUncertaintyNsps = clockVal.drift_uncertainty_nsps,
            .hwClockDiscontinuityCount = clockVal.hw_clock_discontinuity_count
        };

        if(sGnssMeasureCbIface_1_1  != nullptr){
            auto ret = sGnssMeasureCbIface_1_1->GnssMeasurementCb(gnssData);
            if (!ret.isOk()) {
                ALOGE("%s: Unable to invoke callback_1_1", __func__);
            }
        }
        if(sGnssMeasureCbIface  != nullptr){
            auto ret = sGnssMeasureCbIface->GnssMeasurementCb(gnssData);
            if (!ret.isOk()) {
                ALOGE("%s: Unable to invoke callback", __func__);
            }
        }
    }
}

void GnssMeasurement::gnssMeasurementCb_2_0(LegacyGnssData* legacyGnssData)
{
    if (sGnssMeasureCbIface_2_0 == nullptr) {
        ALOGE("%s: GNSSMeasurement Callback Interface configured incorrectly", __func__);
        return;
    }

    if (legacyGnssData == nullptr) {
        ALOGE("%s: Invalid GnssData from GNSS HAL", __func__);
        return;
    }

    ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData gnssData;
    using GnssData_2_0 = ::android::hardware::gnss::V2_0::IGnssMeasurementCallback::GnssData;
    gnssData.measurementCount = std::min(legacyGnssData->measurement_count,
    static_cast<size_t>(::android::hardware::gnss::V1_0::GnssMax::SVS_COUNT));

    using ::android::hardware::gnss::V1_0::IGnssMeasurementCallback;
    hidl_vec<IGnssMeasurementCallback_2_0::GnssMeasurement> measurementS(gnssData.measurementCount);

    ALOGD("%s: 2.0    gnssData.measurementCount = %d", __func__, gnssData.measurementCount);

    for (size_t i = 0; i < gnssData.measurementCount; i++) {
        auto entry = legacyGnssData->measurements[i];
        auto state = static_cast<GnssMeasurementState>(entry.state);

        if (state & IGnssMeasurementCallback::GnssMeasurementState::STATE_TOW_DECODED) {
            state |= IGnssMeasurementCallback::GnssMeasurementState::STATE_TOW_KNOWN;
        }
        if (state & IGnssMeasurementCallback::GnssMeasurementState::STATE_GLO_TOD_DECODED) {
            state |= IGnssMeasurementCallback::GnssMeasurementState::STATE_GLO_TOD_KNOWN;
        }

        ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssMeasurement measurement_1_0 = {
            .flags = entry.flags,
            .svid = entry.svid,
            .constellation = static_cast<GnssConstellationType>(entry.constellation),
            .timeOffsetNs = entry.time_offset_ns,
            .state = state,
            .receivedSvTimeInNs = entry.received_sv_time_in_ns,
            .receivedSvTimeUncertaintyInNs = entry.received_sv_time_uncertainty_in_ns,
            .cN0DbHz = entry.c_n0_dbhz,
            .pseudorangeRateMps = entry.pseudorange_rate_mps,
            .pseudorangeRateUncertaintyMps = entry.pseudorange_rate_uncertainty_mps,
            .accumulatedDeltaRangeState = entry.accumulated_delta_range_state,
            .accumulatedDeltaRangeM = entry.accumulated_delta_range_m,
            .accumulatedDeltaRangeUncertaintyM = entry.accumulated_delta_range_uncertainty_m,
            .carrierFrequencyHz = entry.carrier_frequency_hz,
            .carrierCycles = entry.carrier_cycles,
            .carrierPhase = entry.carrier_phase,
            .carrierPhaseUncertainty = entry.carrier_phase_uncertainty,
            .multipathIndicator = static_cast<IGnssMeasurementCallback::GnssMultipathIndicator>(
            entry.multipath_indicator),
            .snrDb = entry.snr_db,
            .agcLevelDb = entry.agcLevelDb
        };


#if 0
    ALOGD("%s: 2.0    measurement_1_0[%d].svid = %d", __func__,i, measurement_1_0.svid);
    ALOGD("%s: 2.0    measurement_1_0[%d].flags = %d", __func__,i, measurement_1_0.flags);
    ALOGD("%s: 2.0    measurement_1_0[%d].constellation = %d", __func__,i, measurement_1_0.constellation);
    ALOGD("%s: 2.0    measurement_1_0[%d].timeOffsetNs = %f", __func__,i, measurement_1_0.timeOffsetNs);
    ALOGD("%s: 2.0    measurement_1_0[%d].cN0DbHz = %f", __func__,i, measurement_1_0.cN0DbHz);
    ALOGD("%s: 2.0    measurement_1_0[%d].snrDb = %f", __func__,i, measurement_1_0.snrDb);
    ALOGD("%s: 2.0    measurement_1_0[%d].agcLevelDb = %f", __func__,i, measurement_1_0.agcLevelDb);
    ALOGD("%s: 2.0    measurement_1_0[%d].pseudorangeRateUncertaintyMps = %f", __func__,i, measurement_1_0.pseudorangeRateUncertaintyMps);
#endif

    ::android::hardware::gnss::V1_1::IGnssMeasurementCallback::GnssMeasurement measurement_1_1 = {.v1_0 = measurement_1_0};
    ::android::hardware::gnss::V2_0::IGnssMeasurementCallback::GnssMeasurement measurement_2_0 = {
        .v1_1 = measurement_1_1,
        .codeType = "C",
        .state = state,
        .constellation = static_cast<GnssConstellationType_2_0>(entry.constellation)};

        measurementS[i] = measurement_2_0;
}

    auto clockVal = legacyGnssData->clock;
    ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssClock clock = {
        .gnssClockFlags = clockVal.flags,
        .leapSecond = clockVal.leap_second,
        .timeNs = clockVal.time_ns,
        .timeUncertaintyNs = clockVal.time_uncertainty_ns,
        .fullBiasNs = clockVal.full_bias_ns,
        .biasNs = clockVal.bias_ns,
        .biasUncertaintyNs = clockVal.bias_uncertainty_ns,
        .driftNsps = clockVal.drift_nsps,
        .driftUncertaintyNsps = clockVal.drift_uncertainty_nsps,
        .hwClockDiscontinuityCount = clockVal.hw_clock_discontinuity_count
    };

#if 0
    ElapsedRealtime timestamp = {
        .flags = ElapsedRealtimeFlags::HAS_TIMESTAMP_NS |
        ElapsedRealtimeFlags::HAS_TIME_UNCERTAINTY_NS,
        .timestampNs = 0,
    };

    GnssData_2_0 gnssData_2_0 = {
        .measurements = measurementS, .clock = clock, .elapsedRealtime = timestamp};
#else
    ElapsedRealtime timestamp = {
        .flags = ElapsedRealtimeFlags::HAS_TIMESTAMP_NS |
        ElapsedRealtimeFlags::HAS_TIME_UNCERTAINTY_NS,
        .timestampNs = 0,
    };

    GnssData_2_0 gnssData_2_0 = {
        .measurements = measurementS, .clock = clock};
#endif

    for (size_t i = 0; i < gnssData.measurementCount; i++) {
        ALOGD("%s: 2.0    gnssData_2_0.measurements[%d].svid = %d", __func__, i, gnssData_2_0.measurements[i].v1_1.v1_0.svid);
    }
    auto ret = sGnssMeasureCbIface_2_0->gnssMeasurementCb_2_0(gnssData_2_0);

    if (!ret.isOk()) {
    ALOGE("%s: Unable to invoke callback", __func__);
}
}

/*
 * The code in the following method has been moved here from GnssLocationProvider.
 * It converts GpsData to GnssData. This code is no longer required in
 * GnssLocationProvider since GpsData is deprecated and no longer part of the
 * GNSS interface.
 */
void GnssMeasurement::gpsMeasurementCb(GpsData* gpsData) {
    if (sGnssMeasureCbIface == nullptr) {
        ALOGE("%s: GNSSMeasurement Callback Interface configured incorrectly", __func__);
        return;
    }

    if (gpsData == nullptr) {
        ALOGE("%s: Invalid GpsData from GNSS HAL", __func__);
        return;
    }

    ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData gnssData;
    gnssData.measurementCount = std::min(gpsData->measurement_count,
                                         static_cast<size_t>(::android::hardware::gnss::V1_0::GnssMax::SVS_COUNT));


    for (size_t i = 0; i < gnssData.measurementCount; i++) {
        auto entry = gpsData->measurements[i];
        gnssData.measurements[i].flags = entry.flags;
        gnssData.measurements[i].svid = static_cast<int32_t>(entry.prn);
        if (entry.prn >= 1 && entry.prn <= 32) {
            gnssData.measurements[i].constellation = ::android::hardware::gnss::V1_0::GnssConstellationType::GPS;
        } else {
            gnssData.measurements[i].constellation =
                  ::android::hardware::gnss::V1_0::GnssConstellationType::UNKNOWN;
        }

        gnssData.measurements[i].timeOffsetNs = entry.time_offset_ns;
        gnssData.measurements[i].state = entry.state;
        gnssData.measurements[i].receivedSvTimeInNs = entry.received_gps_tow_ns;
        gnssData.measurements[i].receivedSvTimeUncertaintyInNs =
            entry.received_gps_tow_uncertainty_ns;
        gnssData.measurements[i].cN0DbHz = entry.c_n0_dbhz;
        gnssData.measurements[i].pseudorangeRateMps = entry.pseudorange_rate_mps;
        gnssData.measurements[i].pseudorangeRateUncertaintyMps =
                entry.pseudorange_rate_uncertainty_mps;
        gnssData.measurements[i].accumulatedDeltaRangeState =
                entry.accumulated_delta_range_state;
        gnssData.measurements[i].accumulatedDeltaRangeM =
                entry.accumulated_delta_range_m;
        gnssData.measurements[i].accumulatedDeltaRangeUncertaintyM =
                entry.accumulated_delta_range_uncertainty_m;

        if (entry.flags & GNSS_MEASUREMENT_HAS_CARRIER_FREQUENCY) {
            gnssData.measurements[i].carrierFrequencyHz = entry.carrier_frequency_hz;
        } else {
            gnssData.measurements[i].carrierFrequencyHz = 0;
        }

        if (entry.flags & GNSS_MEASUREMENT_HAS_CARRIER_PHASE) {
            gnssData.measurements[i].carrierPhase = entry.carrier_phase;
        } else {
            gnssData.measurements[i].carrierPhase = 0;
        }

        if (entry.flags & GNSS_MEASUREMENT_HAS_CARRIER_PHASE_UNCERTAINTY) {
            gnssData.measurements[i].carrierPhaseUncertainty = entry.carrier_phase_uncertainty;
        } else {
            gnssData.measurements[i].carrierPhaseUncertainty = 0;
        }

        gnssData.measurements[i].multipathIndicator =
                static_cast<IGnssMeasurementCallback::GnssMultipathIndicator>(
                        entry.multipath_indicator);

        if (entry.flags & GNSS_MEASUREMENT_HAS_SNR) {
            gnssData.measurements[i].snrDb = entry.snr_db;
        } else {
            gnssData.measurements[i].snrDb = 0;
        }
    }

    auto clockVal = gpsData->clock;
    static uint32_t discontinuity_count_to_handle_old_clock_type = 0;

    gnssData.clock.leapSecond = clockVal.leap_second;
    /*
     * GnssClock only supports the more effective HW_CLOCK type, so type
     * handling and documentation complexity has been removed.  To convert the
     * old GPS_CLOCK types (active only in a limited number of older devices),
     * the GPS time information is handled as an always discontinuous HW clock,
     * with the GPS time information put into the full_bias_ns instead - so that
     * time_ns - full_bias_ns = local estimate of GPS time. Additionally, the
     * sign of full_bias_ns and bias_ns has flipped between GpsClock &
     * GnssClock, so that is also handled below.
     */
    switch (clockVal.type) {
        case GPS_CLOCK_TYPE_UNKNOWN:
            // Clock type unsupported.
            ALOGE("Unknown clock type provided.");
            break;
        case GPS_CLOCK_TYPE_LOCAL_HW_TIME:
            // Already local hardware time. No need to do anything.
            break;
        case GPS_CLOCK_TYPE_GPS_TIME:
            // GPS time, need to convert.
            clockVal.flags |= GPS_CLOCK_HAS_FULL_BIAS;
            clockVal.full_bias_ns = clockVal.time_ns;
            clockVal.time_ns = 0;
            gnssData.clock.hwClockDiscontinuityCount =
                    discontinuity_count_to_handle_old_clock_type++;
            break;
    }

    gnssData.clock.timeNs = clockVal.time_ns;
    gnssData.clock.timeUncertaintyNs = clockVal.time_uncertainty_ns;
    /*
     * Definition of sign for full_bias_ns & bias_ns has been changed since N,
     * so flip signs here.
     */
    gnssData.clock.fullBiasNs = -(clockVal.full_bias_ns);
    gnssData.clock.biasNs = -(clockVal.bias_ns);
    gnssData.clock.biasUncertaintyNs = clockVal.bias_uncertainty_ns;
    gnssData.clock.driftNsps = clockVal.drift_nsps;
    gnssData.clock.driftUncertaintyNsps = clockVal.drift_uncertainty_nsps;
    gnssData.clock.gnssClockFlags = clockVal.flags;

    auto ret = sGnssMeasureCbIface->GnssMeasurementCb(gnssData);
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}

void GnssMeasurement::gnssMeasurementCbs(LegacyGnssData* legacyGnssData)
{
    if(sGnssMeasureCbIface_2_0 != nullptr)
    {
        gnssMeasurementCb_2_0(legacyGnssData);
    }
    else if((sGnssMeasureCbIface_1_1 != nullptr) || (sGnssMeasureCbIface != nullptr))
    {
        gnssMeasurementCb(legacyGnssData);
    }
    else
    {
        ALOGE("%s: sGnssMeasureCbIface_2_0 is Null", __func__);
    }
}

// Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.
Return<::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus> 
GnssMeasurement::setCallback(const sp<::android::hardware::gnss::V1_0::IGnssMeasurementCallback>& callback) {
        ALOGE("%s: enter", __func__);
    if (mGnssMeasureIface == nullptr) {
        ALOGE("%s: GnssMeasure interface is unavailable", __func__);
        return GnssMeasurementStatus::ERROR_GENERIC;
    }
    sGnssMeasureCbIface = callback;

    return static_cast<GnssMeasurement::GnssMeasurementStatus>(
            mGnssMeasureIface->init(&sGnssMeasurementCbs));
}

Return<void> GnssMeasurement::close()  {
    if (mGnssMeasureIface == nullptr) {
        ALOGE("%s: GnssMeasure interface is unavailable", __func__);
    } else {
        mGnssMeasureIface->close();
    }
    return Void();
}

// Methods from ::android::hardware::gnss::V1_1::IGnssMeasurement follow.
Return<::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus>
GnssMeasurement::setCallback_1_1(const sp<::android::hardware::gnss::V1_1::IGnssMeasurementCallback>& callback, bool enableFullTracking) {
        ALOGE("%s: enter", __func__);
    if (mGnssMeasureIface == nullptr) {
        ALOGE("%s: GnssMeasure interface is unavailable", __func__);
        return GnssMeasurementStatus::ERROR_GENERIC;
    }
    sGnssMeasureCbIface_1_1 = callback;

    return static_cast<GnssMeasurement::GnssMeasurementStatus>(
            mGnssMeasureIface->init(&sGnssMeasurementCbs));

}


#ifndef MOCK
Return<::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus>
GnssMeasurement::setCallback_2_0(const sp<::android::hardware::gnss::V2_0::IGnssMeasurementCallback>& callback, bool enableFullTracking) {
#ifdef  TWO_INIT
    int ret;
#endif

    ALOGE("%s: enter", __func__);

    if (mGnssMeasureIface == nullptr) {
        ALOGE("%s: GnssMeasure interface is unavailable", __func__);
        return GnssMeasurementStatus::ERROR_GENERIC;
    }

    sGnssMeasureCbIface_2_0 = callback;
#ifdef  TWO_INIT
    ret = mGnssMeasureIface->init(&sGnssMeasurementCbs);
    if((ret == GPS_MEASUREMENT_ERROR_ALREADY_INIT) || (ret == GPS_MEASUREMENT_SUCCESS))
    {
        return GnssMeasurementStatus::SUCCESS;
    }
    return GnssMeasurementStatus::ERROR_GENERIC;
#else
    return static_cast<GnssMeasurement::GnssMeasurementStatus>(mGnssMeasureIface->init(&sGnssMeasurementCbs));
#endif
}
#else //MOCK
 Return<::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus>
 GnssMeasurement::setCallback_2_0(const sp< ::android::hardware::gnss::V2_0::IGnssMeasurementCallback>& callback, bool) {
    ALOGD("setCallback_2_0");
    std::unique_lock<std::mutex> lock(mMutex);
    sGnssMeasureCbIface_2_0 = callback;

    if (mIsActive) {
        ALOGW("GnssMeasurement callback already set. Resetting the callback...");
        stop();
    }
    start();

    return ::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus::SUCCESS;
}

void GnssMeasurement::start() {
    ALOGD("start");
    mIsActive = true;
    mThread = std::thread([this]() {
        while (mIsActive == true) {
            auto measurement = this->getMockMeasurement();
            this->reportMeasurement(measurement);

            std::this_thread::sleep_for(std::chrono::milliseconds(mMinIntervalMillis));
        }
    });
}

void GnssMeasurement::stop() {
    ALOGD("stop");
    mIsActive = false;
    if (mThread.joinable()) {
        mThread.join();
    }
}

GnssData_2_0 GnssMeasurement::getMockMeasurement() {
    using GnssMeasurementState = ::android::hardware::gnss::V2_0::IGnssMeasurementCallback::GnssMeasurementState;
    ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssMeasurement measurement_1_0 = {
            .flags = (uint32_t)GnssMeasurementFlags::HAS_CARRIER_FREQUENCY,
            .svid = (int16_t)6,
            .constellation = ::android::hardware::gnss::V1_0::GnssConstellationType::UNKNOWN,
            .timeOffsetNs = 0.0,
            .receivedSvTimeInNs = 8195997131077,
            .receivedSvTimeUncertaintyInNs = 15,
            .cN0DbHz = 30.0,
            .pseudorangeRateMps = -484.13739013671875,
            .pseudorangeRateUncertaintyMps = 1.0379999876022339,
            .accumulatedDeltaRangeState = (uint32_t)::android::hardware::gnss::V1_0::IGnssMeasurementCallback::
                    GnssAccumulatedDeltaRangeState::ADR_STATE_UNKNOWN,
            .accumulatedDeltaRangeM = 0.0,
            .accumulatedDeltaRangeUncertaintyM = 0.0,
            .carrierFrequencyHz = 1.59975e+09,
            .multipathIndicator =
                    ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssMultipathIndicator::INDICATOR_UNKNOWN};
     ::android::hardware::gnss::V1_1::IGnssMeasurementCallback::GnssMeasurement measurement_1_1 = {.v1_0 = measurement_1_0};
     ::android::hardware::gnss::V2_0::IGnssMeasurementCallback::GnssMeasurement measurement_2_0 = {
            .v1_1 = measurement_1_1,
            .codeType = "C",
            .constellation = ::android::hardware::gnss::V2_0::GnssConstellationType::GLONASS,
            .state = GnssMeasurementState::STATE_CODE_LOCK | GnssMeasurementState::STATE_BIT_SYNC |
                     GnssMeasurementState::STATE_SUBFRAME_SYNC |
                     GnssMeasurementState::STATE_TOW_DECODED |
                     GnssMeasurementState::STATE_GLO_STRING_SYNC |
                     GnssMeasurementState::STATE_GLO_TOD_DECODED};

    hidl_vec<IGnssMeasurementCallback_2_0::GnssMeasurement> measurements(1);
    measurements[0] = measurement_2_0;
    ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssClock clock = {.timeNs = 2713545000000,
                                                       .fullBiasNs = -1226701900521857520,
                                                       .biasNs = 0.59689998626708984,
                                                       .biasUncertaintyNs = 47514.989972114563,
                                                       .driftNsps = -51.757811607455452,
                                                       .driftUncertaintyNsps = 310.64968328491528,
                                                       .hwClockDiscontinuityCount = 1};

    ElapsedRealtime timestamp = {
            .flags = ElapsedRealtimeFlags::HAS_TIMESTAMP_NS |
                     ElapsedRealtimeFlags::HAS_TIME_UNCERTAINTY_NS,
            .timestampNs = 0,//static_cast<uint64_t>(::android::elapsedRealtimeNano()),
            // This is an hardcoded value indicating a 1ms of uncertainty between the two clocks.
            // In an actual implementation provide an estimate of the synchronization uncertainty
            // or don't set the field.
            .timeUncertaintyNs = 1000000};

    GnssData_2_0 gnssData = {
            .measurements = measurements, .clock = clock, .elapsedRealtime = timestamp};
    return gnssData;
}

void GnssMeasurement::reportMeasurement(const GnssData_2_0& data) {
    ALOGD("reportMeasurement()");
    std::unique_lock<std::mutex> lock(mMutex);
    if (sGnssMeasureCbIface_2_0 == nullptr) {
        ALOGE("%s: GnssMeasurement::sCallback is null.", __func__);
        return;
    }
    auto ret = sGnssMeasureCbIface_2_0->gnssMeasurementCb_2_0(data);
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}

#endif //MOCK

// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssMeasurement* HIDL_FETCH_IGnssMeasurement(const char* /* name */) {
    //return new GnssMeasurement();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
