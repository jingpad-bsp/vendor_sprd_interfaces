#include "GnssCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
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

Return<void> GnssCallback::gnssSetCapabilitesCb(uint32_t capabilities) {
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


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssCallback* HIDL_FETCH_IGnssCallback(const char* /* name */) {
//    return new GnssCallback();
//}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
