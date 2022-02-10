#define LOG_TAG "GnssHAL_V_AGnss"
#include "AGnss.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {
  
std::vector<std::unique_ptr<ThreadFuncArgs>> AGnss::sThreadFuncArgsList;
sp<IAGnssCallback> AGnss::sAGnssCbIface = nullptr;
bool AGnss::sInterfaceExists = false;

AGpsCallbacks AGnss::sAGnssCb = {
    .status_cb = statusCb,
    .create_thread_cb = createThreadCb
};

AGnss::AGnss(const AGpsInterface* aGpsIface) : mAGnssIface(aGpsIface) {
    /* Error out if an instance of the interface already exists. */
    LOG_ALWAYS_FATAL_IF(sInterfaceExists);
    sInterfaceExists = true;
}

AGnss::~AGnss() {
    sThreadFuncArgsList.clear();
    sInterfaceExists = false;
}

void AGnss::statusCb(AGpsStatus* status) {
    ALOGE("%s: AGnss Enter", __func__);
    if (sAGnssCbIface == nullptr) {
        ALOGE("%s: AGNSS Callback Interface configured incorrectly", __func__);
        return;
    }

    if (status == nullptr) {
        ALOGE("AGNSS status is invalid");
        return;
    }

    auto ret = sAGnssCbIface->agnssStatusCb(static_cast<IAGnssCallback::AGnssType>(status->type),static_cast<IAGnssCallback::AGnssStatusValue>(status->status));
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}

pthread_t AGnss::createThreadCb(const char* name, void (*start)(void*), void* arg) {
    return createPthread(name, start, arg, &sThreadFuncArgsList);
}
// Methods from ::android::hardware::gnss::V2_0::IAGnss follow.
Return<void> AGnss::setCallback(const sp<IAGnssCallback>& callback) {
    ALOGE("%s: AGnss Enter", __func__);
    if (mAGnssIface == nullptr) {
        ALOGE("%s: AGnss interface is unavailable", __func__);
        return Void();
    }

    sAGnssCbIface = callback;

    mAGnssIface->init(&sAGnssCb);
    return Void();
}

Return<bool> AGnss::dataConnClosed() {
    if (mAGnssIface == nullptr) {
        ALOGE("%s: AGnss interface is unavailable", __func__);
        return false;
    }

    return (mAGnssIface->data_conn_closed() == 0);
}

Return<bool> AGnss::dataConnFailed() {
    if (mAGnssIface == nullptr) {
        ALOGE("%s: AGnss interface is unavailable", __func__);
        return false;
    }

    return (mAGnssIface->data_conn_failed() == 0);
}

Return<bool> AGnss::setServer(IAGnssCallback::AGnssType type, 
                              const hidl_string& hostname,
                              int32_t port) {
    if (mAGnssIface == nullptr) {
        ALOGE("%s: AGnss interface is unavailable", __func__);
        return false;
    }

    return (mAGnssIface->set_server(static_cast<AGpsType>(type), hostname.c_str(), port) == 0);
}

Return<bool> AGnss::dataConnOpen(uint64_t networkHandle, const hidl_string& apn, IAGnss::ApnIpType apnIpType) {
    ALOGE("%s: enter", __func__);
    if (mAGnssIface == nullptr) {
        ALOGE("%s: AGnss interface is unavailable", __func__);
        return false;
    }

    return (mAGnssIface->data_conn_open_with_apn_ip_type(apn.c_str(),
                                                     static_cast<uint16_t>(apnIpType)) == 0);
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

//IAGnss* HIDL_FETCH_IAGnss(const char* /* name */) {
    //return new AGnss();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
