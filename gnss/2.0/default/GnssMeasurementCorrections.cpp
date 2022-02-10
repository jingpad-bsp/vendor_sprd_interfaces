/*
 * Copyright (C) 2019 The Android Open Source Project
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

#define LOG_TAG "GnssHAL_V_GnssMeasurementCorrections"

#include "GnssMeasurementCorrections.h"
#include <log/log.h>

namespace android {
namespace hardware {
namespace gnss {
namespace measurement_corrections {
namespace V1_0 {
namespace implementation {

sp<::android::hardware::gnss::measurement_corrections::V1_0::IMeasurementCorrectionsCallback>
	GnssMeasurementCorrections::sGnssMeasureCorrCbIface = nullptr;
GnssMeasurementCorrectionCallbacks GnssMeasurementCorrections::sGnssMeasuremenCorrectionCbs = {
    .size = sizeof(GnssMeasurementCorrectionCallbacks),
    .mea_correction_capability_callback = gnssMeasurementCorrectionCapabilityCb,
};

GnssMeasurementCorrections::GnssMeasurementCorrections(const GpsMeasureCorrectionsInterface* gpsMeasurementCorrectionIface)
    : mGnssMeasureCorrectionIface(gpsMeasurementCorrectionIface) {}


// Methods from V1_0::IMeasurementCorrections follow.
Return<bool> GnssMeasurementCorrections::setCorrections(const ::android::hardware::gnss::measurement_corrections::V1_0::MeasurementCorrections& corrections) {
    int single_count = 0;
    GnssMeasurementCorrections_t *GnssCorrections;

    for (auto singleSatCorrection : corrections.satCorrections) {
        single_count++;
    }

    GnssCorrections = (GnssMeasurementCorrections_t*)malloc(sizeof(GnssMeasurementCorrections_t));
    GnssCorrections->satCorrections = (SingleSatCorrection_t*)malloc(sizeof(SingleSatCorrection_t)*single_count);
    memset(GnssCorrections, 0, sizeof(GnssMeasurementCorrections_t));
    memset(GnssCorrections->satCorrections, 0, sizeof(SingleSatCorrection_t));
    memcpy(GnssCorrections, &corrections, sizeof(GnssMeasurementCorrections_t));

    mGnssMeasureCorrectionIface->setCorrections(GnssCorrections, single_count);
    free(GnssCorrections->satCorrections);
    free(GnssCorrections);
    return true;
}

Return<bool> GnssMeasurementCorrections::setCallback(const sp<V1_0::IMeasurementCorrectionsCallback>& callback) {
    int ret = false;

    if (callback == nullptr) {
        ALOGE("%s: Null callback ignored", __func__);
        return false;
    }

    sGnssMeasureCorrCbIface = callback;
    ret =  mGnssMeasureCorrectionIface->init(&sGnssMeasuremenCorrectionCbs);
    if (ret == 0)
        return true;
    return false;
}

void GnssMeasurementCorrections::gnssMeasurementCorrectionCapabilityCb(int capability) 
{
    if (sGnssMeasureCorrCbIface == nullptr) {
        ALOGE("%s: Null callback ignored", __func__);
        return;
    }

    auto ret =sGnssMeasureCorrCbIface->setCapabilitiesCb(capability);
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}
}  // namespace implementation
}  // namespace V1_0
}  // namespace measurement_corrections
}  // namespace gnss
}  // namespace hardware
}  // namespace android
