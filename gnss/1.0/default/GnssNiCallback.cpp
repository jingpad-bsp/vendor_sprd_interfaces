#include "GnssNiCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssNiCallback follow.
Return<void> GnssNiCallback::niNotifyCb(const ::android::hardware::gnss::V1_0::IGnssNiCallback::GnssNiNotification& notification) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssNiCallback* HIDL_FETCH_IGnssNiCallback(const char* /* name */) {
//    return new GnssNiCallback();
//}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
