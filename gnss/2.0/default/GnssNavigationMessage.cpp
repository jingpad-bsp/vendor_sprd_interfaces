#include <log/log.h>
#include "GnssNavigationMessage.h"
#define LOG_TAG "GnssHAL_V_GnssNavigationMessage"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {


sp<IGnssNavigationMessageCallback> GnssNavigationMessage::sGnssNavigationMsgCbIface = nullptr;

GpsNavigationMessageCallbacks GnssNavigationMessage::sGnssNavigationMessageCb = {
    .size = sizeof(GpsNavigationMessageCallbacks),
    .navigation_message_callback = nullptr,
    .gnss_navigation_message_callback = gnssNavigationMessageCb
};

GnssNavigationMessage::GnssNavigationMessage(
        const GpsNavigationMessageInterface* gpsNavigationMessageIface) :
    mGnssNavigationMessageIface(gpsNavigationMessageIface) {}

void GnssNavigationMessage::gnssNavigationMessageCb(LegacyGnssNavigationMessage* message) {
    if (sGnssNavigationMsgCbIface == nullptr) {
        ALOGE("%s: GnssNavigation Message Callback Interface configured incorrectly", __func__);
        return;
    }

    if (message == nullptr) {
        ALOGE("%s, received invalid GnssNavigationMessage from GNSS HAL", __func__);
        return;
    }

    IGnssNavigationMessageCallback::GnssNavigationMessage navigationMsg;

    navigationMsg.svid = message->svid;
    navigationMsg.type =
            static_cast<IGnssNavigationMessageCallback::GnssNavigationMessageType>(message->type);
    navigationMsg.status = message->status;
    navigationMsg.messageId = message->message_id;
    navigationMsg.submessageId = message->submessage_id;
    navigationMsg.data.setToExternal(message->data, message->data_length);

    auto ret = sGnssNavigationMsgCbIface->gnssNavigationMessageCb(navigationMsg);
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}
// Methods from ::android::hardware::gnss::V1_0::IGnssNavigationMessage follow.
Return<::android::hardware::gnss::V1_0::IGnssNavigationMessage::GnssNavigationMessageStatus> GnssNavigationMessage::setCallback(
		const sp<::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback>& callback) {
    if (mGnssNavigationMessageIface == nullptr) {
        ALOGE("%s: GnssNavigationMessage not available", __func__);
        return GnssNavigationMessageStatus::ERROR_GENERIC;
    }

    sGnssNavigationMsgCbIface = callback;

    return static_cast<GnssNavigationMessage::GnssNavigationMessageStatus>(
            mGnssNavigationMessageIface->init(&sGnssNavigationMessageCb));
}

Return<void> GnssNavigationMessage::close() {
    if (mGnssNavigationMessageIface == nullptr) {
        ALOGE("%s: GnssNavigationMessage not available", __func__);
    } else {
        mGnssNavigationMessageIface->close();
    }
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssNavigationMessage* HIDL_FETCH_IGnssNavigationMessage(const char* /* name */) {
    //return new GnssNavigationMessage();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
