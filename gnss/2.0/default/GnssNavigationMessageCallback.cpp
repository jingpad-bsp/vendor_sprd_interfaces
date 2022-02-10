#include "GnssNavigationMessageCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback follow.
Return<void> GnssNavigationMessageCallback::gnssNavigationMessageCb(const ::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback::GnssNavigationMessage& message) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssNavigationMessageCallback* HIDL_FETCH_IGnssNavigationMessageCallback(const char* /* name */) {
    //return new GnssNavigationMessageCallback();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
