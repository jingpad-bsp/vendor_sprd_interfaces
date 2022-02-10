#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSS_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSS_H

#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/1.0/IAGnss.h>
#include <hidl/MQDescriptor.h>
#include <hardware/gps_internal.h>
#include <hidl/Status.h>
#include <netinet/in.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {


using ::android::hardware::gnss::V1_0::IAGnss;
using ::android::hardware::gnss::V1_0::IAGnssCallback;

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;

using ::android::hardware::Return;
using ::android::hardware::Void;

using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

/*
 * Extended interface for AGNSS support. Also contains wrapper methods to allow
 * methods from IAGnssCallback interface to be passed into the conventional
 * implementation of the GNSS HAL.
 */
struct AGnss : public IAGnss {
    AGnss(const AGpsInterface* agpsIface);
    ~AGnss();
    /*
     * Methods from ::android::hardware::gnss::V1_0::IAGnss interface follow.
     * These declarations were generated from IAGnss.hal.
     */
    Return<void> setCallback(const sp<IAGnssCallback>& callback) override;
    Return<bool> dataConnClosed() override;
    Return<bool> dataConnFailed() override;
    Return<bool> setServer(IAGnssCallback::AGnssType type,
                         const hidl_string& hostname, int32_t port) override;
    Return<bool> dataConnOpen(const hidl_string& apn,
                                           IAGnss::ApnIpType apnIpType) override;

    /*
     * Callback methods to be passed into the conventional GNSS HAL by the default
     * implementation. These methods are not part of the IAGnss base class.
     */
    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);
    static void statusCb(AGpsStatus* status);

    /*
     * Holds function pointers to the callback methods.
     */
    static AGpsCallbacks sAGnssCb;

 private:
    const AGpsInterface* mAGnssIface = nullptr;
    static sp<IAGnssCallback> sAGnssCbIface;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IAGnss* HIDL_FETCH_IAGnss(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSS_H
