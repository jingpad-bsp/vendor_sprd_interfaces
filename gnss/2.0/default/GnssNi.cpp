#include "GnssNi.h"
#define LOG_TAG "GnssHAL_V_GnssNiInterface"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

std::vector<std::unique_ptr<ThreadFuncArgs>> GnssNi::sThreadFuncArgsList;
sp<IGnssNiCallback> GnssNi::sGnssNiCbIface = nullptr;
bool GnssNi::sInterfaceExists = false;

GpsNiCallbacks GnssNi::sGnssNiCb = {
    .notify_cb = niNotifyCb,
    .create_thread_cb = createThreadCb
};

GnssNi::GnssNi(const GpsNiInterface* gpsNiIface) : mGnssNiIface(gpsNiIface) {
    /* Error out if an instance of the interface already exists. */
    LOG_ALWAYS_FATAL_IF(sInterfaceExists);
    sInterfaceExists = true;
}

GnssNi::~GnssNi() {
    sThreadFuncArgsList.clear();
    sInterfaceExists = false;
}

pthread_t GnssNi::createThreadCb(const char* name, void (*start)(void*), void* arg) {
    return createPthread(name, start, arg, &sThreadFuncArgsList);
}

void GnssNi::niNotifyCb(GpsNiNotification* notification) {
    if (sGnssNiCbIface == nullptr) {
        ALOGE("%s: GNSS NI Callback Interface configured incorrectly", __func__);
        return;
    }

    if (notification == nullptr) {
        ALOGE("%s: Invalid GpsNotification callback from GNSS HAL", __func__);
        return;
    }

    IGnssNiCallback::GnssNiNotification notificationGnss = {
        .notificationId =  notification->notification_id,
        .niType = static_cast<IGnssNiCallback::GnssNiType>(notification->ni_type),
        .notifyFlags = notification->notify_flags,
        .timeoutSec = static_cast<uint32_t>(notification->timeout),
        .defaultResponse =
                static_cast<IGnssNiCallback::GnssUserResponseType>(notification->default_response),
        .requestorId = notification->requestor_id,
        .notificationMessage = notification->text,
        .requestorIdEncoding =
                static_cast<IGnssNiCallback::GnssNiEncodingType>(notification->requestor_id_encoding),
        .notificationIdEncoding =
                static_cast<IGnssNiCallback::GnssNiEncodingType>(notification->text_encoding)
    };

    auto ret = sGnssNiCbIface->niNotifyCb(notificationGnss);
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}
// Methods from ::android::hardware::gnss::V1_0::IGnssNi follow.
Return<void> GnssNi::setCallback(const sp<::android::hardware::gnss::V1_0::IGnssNiCallback>& callback) {
    if (mGnssNiIface == nullptr) {
       ALOGE("%s: GnssNi interface is unavailable", __func__);
       return Void();
    }

    sGnssNiCbIface = callback;

    mGnssNiIface->init(&sGnssNiCb);
    return Void();

}

Return<void> GnssNi::respond(int32_t notifId, ::android::hardware::gnss::V1_0::IGnssNiCallback::GnssUserResponseType userResponse) {
    if (mGnssNiIface == nullptr) {
        ALOGE("%s: GnssNi interface is unavailable", __func__);
    } else {
        mGnssNiIface->respond(notifId, static_cast<GpsUserResponseType>(userResponse));
    }
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssNi* HIDL_FETCH_IGnssNi(const char* /* name */) {
    //return new GnssNi();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
