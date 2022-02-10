#include "AGnssRilCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IAGnssRilCallback follow.
Return<void> AGnssRilCallback::requestSetIdCb(uint32_t setIdflag) {
    // TODO implement
    return Void();
}

Return<void> AGnssRilCallback::requestRefLocCb() {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IAGnssRilCallback* HIDL_FETCH_IAGnssRilCallback(const char* /* name */) {
//    return new AGnssRilCallback();
//}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
