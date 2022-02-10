#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSNAVIGATIONMESSAGE_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSNAVIGATIONMESSAGE_H

#include <vendor/sprd/hardware/gnss/1.0/IGnssNavigationMessage.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <hardware/gps.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::gnss::V1_0::IGnssNavigationMessage;
using ::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback;

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

using LegacyGnssNavigationMessage = ::GnssNavigationMessage;

/*
 * Extended interface for GNSS navigation message reporting support. Also contains wrapper methods
 * to allow methods from IGnssNavigationMessageCallback interface to be passed into the conventional
 * implementation of the GNSS HAL.
 */
struct GnssNavigationMessage : public IGnssNavigationMessage {
    GnssNavigationMessage(const GpsNavigationMessageInterface* gpsNavigationMessageIface);

    /*
     * Methods from ::android::hardware::gnss::V1_0::IGnssNavigationMessage follow.
     * These declarations were generated from IGnssNavigationMessage.hal.
     */
    Return<GnssNavigationMessageStatus> setCallback(
        const sp<IGnssNavigationMessageCallback>& callback) override;
    Return<void> close() override;

    /*
     * Callback methods to be passed into the conventional GNSS HAL by the default implementation.
     * These methods are not part of the IGnssNavigationMessage base class.
     */
    static void gnssNavigationMessageCb(LegacyGnssNavigationMessage* message);

    /*
     * Holds function pointers to the callback methods.
     */
    static GpsNavigationMessageCallbacks sGnssNavigationMessageCb;
 private:
    const GpsNavigationMessageInterface* mGnssNavigationMessageIface = nullptr;
    static sp<IGnssNavigationMessageCallback> sGnssNavigationMsgCbIface;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IGnssNavigationMessage* HIDL_FETCH_IGnssNavigationMessage(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSNAVIGATIONMESSAGE_H
