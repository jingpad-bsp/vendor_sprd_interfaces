#define LOG_TAG "GnssHAL_V_GnssXtra"

#include "GnssXtra.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

std::vector<std::unique_ptr<ThreadFuncArgs>> GnssXtra::sThreadFuncArgsList;
sp<IGnssXtraCallback> GnssXtra::sGnssXtraCbIface = nullptr;
bool GnssXtra::sInterfaceExists = false;

GpsXtraCallbacks GnssXtra::sGnssXtraCb = {
    .download_request_cb = gnssXtraDownloadRequestCb,
    .create_thread_cb = createThreadCb,
};

GnssXtra::~GnssXtra() {
    sThreadFuncArgsList.clear();
    sInterfaceExists = false;
}

pthread_t GnssXtra::createThreadCb(const char* name, void (*start)(void*), void* arg) {
    return createPthread(name, start, arg, &sThreadFuncArgsList);
}

GnssXtra::GnssXtra(const GpsXtraInterface* xtraIface) : mGnssXtraIface(xtraIface) {
    /* Error out if an instance of the interface already exists. */
    LOG_ALWAYS_FATAL_IF(sInterfaceExists);
    sInterfaceExists = true;
}

void GnssXtra::gnssXtraDownloadRequestCb() {
    if (sGnssXtraCbIface == nullptr) {
        ALOGE("%s: GNSS Callback Interface configured incorrectly", __func__);
        return;
    }

    auto ret = sGnssXtraCbIface->downloadRequestCb();
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}
// Methods from ::android::hardware::gnss::V1_0::IGnssXtra follow.
Return<bool> GnssXtra::setCallback(const sp<::android::hardware::gnss::V1_0::IGnssXtraCallback>& callback) {
    if (mGnssXtraIface == nullptr) {
        ALOGE("%s: Gnss Xtra interface is unavailable", __func__);
        return false;
    }

    sGnssXtraCbIface = callback;

    return (mGnssXtraIface->init(&sGnssXtraCb) == 0);
}

Return<bool> GnssXtra::injectXtraData(const hidl_string& xtraData)  {
    if (mGnssXtraIface == nullptr) {
        ALOGE("%s: Gnss Xtra interface is unavailable", __func__);
        return false;
    }

    char* buf = new char[xtraData.size()];
    const char* data = xtraData.c_str();

    memcpy(buf, data, xtraData.size());

    int ret = mGnssXtraIface->inject_xtra_data(buf, xtraData.size());
    delete[] buf;
    return (ret == 0);
}



// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssXtra* HIDL_FETCH_IGnssXtra(const char* /* name */) {
    //return new GnssXtra();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
