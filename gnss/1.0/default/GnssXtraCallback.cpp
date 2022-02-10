#include "GnssXtraCallback.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssXtraCallback follow.
Return<void> GnssXtraCallback::downloadRequestCb() {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssXtraCallback* HIDL_FETCH_IGnssXtraCallback(const char* /* name */) {
//    return new GnssXtraCallback();
//}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
