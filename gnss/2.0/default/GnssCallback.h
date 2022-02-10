#pragma once

#include <vendor/sprd/hardware/gnss/2.0/IGnssCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssCallback : public IGnssCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssCallback follow.
    Return<void> gnssLocationCb(const ::android::hardware::gnss::V1_0::GnssLocation& location) override;
    Return<void> gnssStatusCb(::android::hardware::gnss::V1_0::IGnssCallback::GnssStatusValue status) override;
    Return<void> gnssSvStatusCb(const ::android::hardware::gnss::V1_0::IGnssCallback::GnssSvStatus& svInfo) override;
    Return<void> gnssNmeaCb(int64_t timestamp, const hidl_string& nmea) override;
    Return<void> gnssSetCapabilitesCb(hidl_bitfield<Capabilities> capabilities) override;
    Return<void> gnssAcquireWakelockCb() override;
    Return<void> gnssReleaseWakelockCb() override;
    Return<void> gnssRequestTimeCb() override;
    Return<void> gnssSetSystemInfoCb(const ::android::hardware::gnss::V1_0::IGnssCallback::GnssSystemInfo& info) override;

    // Methods from ::android::hardware::gnss::V1_1::IGnssCallback follow.
    Return<void> gnssNameCb(const hidl_string& name) override;
    Return<void> gnssRequestLocationCb(bool independentFromGnss) override;

    // Methods from ::android::hardware::gnss::V2_0::IGnssCallback follow.
    Return<void> gnssSetCapabilitiesCb_2_0(hidl_bitfield<Capabilities> capabilities) override;
    Return<void> gnssLocationCb_2_0(const ::android::hardware::gnss::V2_0::GnssLocation& location) override;
    Return<void> gnssRequestLocationCb_2_0(bool independentFromGnss, bool isUserEmergency) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// extern "C" IGnssCallback* HIDL_FETCH_IGnssCallback(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
