#include "AGnssCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V2_0::IAGnssCallback follow.
Return<void> AGnssCallback::agnssStatusCb(::android::hardware::gnss::V2_0::IAGnssCallback::AGnssType type, ::android::hardware::gnss::V2_0::IAGnssCallback::AGnssStatusValue status) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IAGnssCallback* HIDL_FETCH_IAGnssCallback(const char* /* name */) {
    //return new AGnssCallback();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
