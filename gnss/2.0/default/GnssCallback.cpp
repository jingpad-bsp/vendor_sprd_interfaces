#include "GnssCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssCallback follow.
Return<void> GnssCallback::gnssLocationCb(const ::android::hardware::gnss::V1_0::GnssLocation& location) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssStatusCb(::android::hardware::gnss::V1_0::IGnssCallback::GnssStatusValue status) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssSvStatusCb(const ::android::hardware::gnss::V1_0::IGnssCallback::GnssSvStatus& svInfo) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssNmeaCb(int64_t timestamp, const hidl_string& nmea) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssSetCapabilitesCb(hidl_bitfield<Capabilities> capabilities) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssAcquireWakelockCb() {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssReleaseWakelockCb() {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssRequestTimeCb() {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssSetSystemInfoCb(const ::android::hardware::gnss::V1_0::IGnssCallback::GnssSystemInfo& info) {
    // TODO implement
    return Void();
}


// Methods from ::android::hardware::gnss::V1_1::IGnssCallback follow.
Return<void> GnssCallback::gnssNameCb(const hidl_string& name) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssRequestLocationCb(bool independentFromGnss) {
    // TODO implement
    return Void();
}


// Methods from ::android::hardware::gnss::V2_0::IGnssCallback follow.
Return<void> GnssCallback::gnssSetCapabilitiesCb_2_0(hidl_bitfield<Capabilities> capabilities) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssLocationCb_2_0(const ::android::hardware::gnss::V2_0::GnssLocation& location) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssRequestLocationCb_2_0(bool independentFromGnss, bool isUserEmergency) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssCallback* HIDL_FETCH_IGnssCallback(const char* /* name */) {
    //return new GnssCallback();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
