#include "GnssUnisocExt.h"
#define LOG_TAG "GnssHAL_V_GnssUnisocInterface"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {
std::vector<std::unique_ptr<ThreadFuncArgs>> GnssUnisocExt::sThreadFuncArgsList;
bool GnssUnisocExt::sInterfaceExists = false;

GnssUnisocExt::GnssUnisocExt(const GnssUnisocExtInterface* GnssUnisocIface) : mGnssUnisocExtIface(GnssUnisocIface) {
    /* Error out if an instance of the interface already exists. */
    LOG_ALWAYS_FATAL_IF(sInterfaceExists);
    sInterfaceExists = true;
}

GnssUnisocExt::~GnssUnisocExt() {
    sThreadFuncArgsList.clear();
    sInterfaceExists = false;
}
// Methods from ::vendor::sprd::hardware::gnss::V2_0::IGnssUnisocExt follow.
Return<bool> GnssUnisocExt::setCallback(const sp<::vendor::sprd::hardware::gnss::V2_0::IGnssUnisocExtCallback>& callback) {
    // TODO implement
    return bool {};
}

Return<bool> GnssUnisocExt::setNetWorkHandle(int64_t networkhandle) {
    if (mGnssUnisocExtIface == nullptr) {
        ALOGE("%s: interface is unavailable", __func__);
        return false;
    }
    ALOGE("%s: interface enter %ld", __func__, networkhandle);
    mGnssUnisocExtIface->network_handle_bind(networkhandle);
    return true;
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssUnisocExt* HIDL_FETCH_IGnssUnisocExt(const char* /* name */) {
    //return new GnssUnisocExt();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
