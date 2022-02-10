#pragma once

#include "gnss_debug.h"
#include <vendor/sprd/hardware/gnss/2.0/IGnssNavigationMessage.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::gnss::V1_0::IGnssNavigationMessage;
using ::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

using LegacyGnssNavigationMessage = ::GnssNavigationMessage;
struct GnssNavigationMessage : public IGnssNavigationMessage {
    GnssNavigationMessage(const GpsNavigationMessageInterface* gpsNavigationMessageIface);
    // Methods from ::android::hardware::gnss::V1_0::IGnssNavigationMessage follow.
    Return<::android::hardware::gnss::V1_0::IGnssNavigationMessage::GnssNavigationMessageStatus> setCallback(
		const sp<::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback>& callback) override;
    Return<void> close() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    static void gnssNavigationMessageCb(LegacyGnssNavigationMessage* message);


    static GpsNavigationMessageCallbacks sGnssNavigationMessageCb;
 private:
    const GpsNavigationMessageInterface* mGnssNavigationMessageIface = nullptr;
    static sp<IGnssNavigationMessageCallback> sGnssNavigationMsgCbIface;
};

// extern "C" IGnssNavigationMessage* HIDL_FETCH_IGnssNavigationMessage(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
