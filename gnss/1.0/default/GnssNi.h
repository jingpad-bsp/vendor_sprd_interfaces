#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSNI_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSNI_H

#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/1.0/IGnssNi.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <hardware/gps.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssNi;
using ::android::hardware::gnss::V1_0::IGnssNiCallback;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;

using ::android::hardware::Return;
using ::android::hardware::Void;

using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

struct GnssNi : public IGnssNi {
    GnssNi(const GpsNiInterface* gpsNiIface);
    ~GnssNi();

    /*
     * Methods from ::android::hardware::gnss::V1_0::IGnssNi follow.
     * These declarations were generated from IGnssNi.hal.
     */
    Return<void> setCallback(const sp<IGnssNiCallback>& callback) override;
    Return<void> respond(int32_t notifId,
                         IGnssNiCallback::GnssUserResponseType userResponse) override;

    /*
     * Callback methods to be passed into the conventional GNSS HAL by the default
     * implementation. These methods are not part of the IGnssNi base class.
     */
    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);
    static void niNotifyCb(GpsNiNotification* notification);

    /*
     * Holds function pointers to the callback methods.
     */
    static GpsNiCallbacks sGnssNiCb;

 private:
    const GpsNiInterface* mGnssNiIface = nullptr;
    static sp<IGnssNiCallback> sGnssNiCbIface;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IGnssNi* HIDL_FETCH_IGnssNi(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSNI_H
