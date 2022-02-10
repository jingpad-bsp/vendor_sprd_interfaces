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
#include "gnss_debug.h"
#include <android/hardware/gnss/measurement_corrections/1.0/IMeasurementCorrections.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif


namespace android {
namespace hardware {
namespace gnss {
namespace measurement_corrections {
namespace V1_0 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

struct GnssMeasurementCorrections : public IMeasurementCorrections {
    GnssMeasurementCorrections(const GpsMeasureCorrectionsInterface* gpsMeasurementCorrectionIface);

    // Methods from V1_0::IMeasurementCorrections follow.
    Return<bool> setCorrections(const ::android::hardware::gnss::measurement_corrections::V1_0::MeasurementCorrections& corrections) override;
    Return<bool> setCallback(const sp<V1_0::IMeasurementCorrectionsCallback>& callback) override;

    static void gnssMeasurementCorrectionCapabilityCb(int capability);
    static GnssMeasurementCorrectionCallbacks sGnssMeasuremenCorrectionCbs;

private:
    const GpsMeasureCorrectionsInterface* mGnssMeasureCorrectionIface = nullptr;
    static sp<::android::hardware::gnss::measurement_corrections::V1_0::IMeasurementCorrectionsCallback> sGnssMeasureCorrCbIface;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace measurement_corrections
}  // namespace gnss
}  // namespace hardware
}  // namespace android
