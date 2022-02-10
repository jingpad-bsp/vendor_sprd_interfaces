#include "GnssMeasurementCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssMeasurementCallback follow.
Return<void> GnssMeasurementCallback::GnssMeasurementCb(const ::android::hardware::gnss::V1_0::IGnssMeasurementCallback::GnssData& data) {
    // TODO implement
    return Void();
}


// Methods from ::android::hardware::gnss::V1_1::IGnssMeasurementCallback follow.
Return<void> GnssMeasurementCallback::gnssMeasurementCb(const ::android::hardware::gnss::V1_1::IGnssMeasurementCallback::GnssData& data) {
    // TODO implement
    return Void();
}


// Methods from ::android::hardware::gnss::V2_0::IGnssMeasurementCallback follow.
Return<void> GnssMeasurementCallback::gnssMeasurementCb_2_0(const ::android::hardware::gnss::V2_0::IGnssMeasurementCallback::GnssData& data) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssMeasurementCallback* HIDL_FETCH_IGnssMeasurementCallback(const char* /* name */) {
    //return new GnssMeasurementCallback();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
