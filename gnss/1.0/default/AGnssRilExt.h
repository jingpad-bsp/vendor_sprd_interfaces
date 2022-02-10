#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSRIL_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSRIL_H

#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/1.0/IAGnssRilExt.h>
#include <hardware/gps.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::gnss::V1_0::IAGnssRil;
using ::android::hardware::gnss::V1_0::IAGnssRilCallback;


using ::android::hardware::Return;
using ::android::hardware::Void;

using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

struct AGnssRilExt : public IAGnssRilExt {
    AGnssRilExt(const AGpsRilInterface* aGpsRilIface);
    ~AGnssRilExt();

    /*
     * Methods from ::android::hardware::gnss::V1_0::IAGnssRilExt follow.
     * These declarations were generated from IAGnssRil.hal.
     */
    Return<void> setCallback(const sp<::android::hardware::gnss::V1_0::IAGnssRilCallback>& callback) override;
    Return<void> setRefLocation(const ::android::hardware::gnss::V1_0::IAGnssRil::AGnssRefLocation& agnssReflocation) override;
    Return<bool> setSetId(::android::hardware::gnss::V1_0::IAGnssRil::SetIDType type, const hidl_string& setid) override;
    Return<bool> updateNetworkState(bool connected, ::android::hardware::gnss::V1_0::IAGnssRil::NetworkType type, bool roaming) override;

    Return<bool> updateNetworkAvailability(bool available, const hidl_string& apn) override;
    static void requestSetId(uint32_t flags);
    static void requestRefLoc(uint32_t flags);

    /*
     * Callback method to be passed into the conventional GNSS HAL by the default
     * implementation. This method is not part of the IAGnssRil base class.
     */
    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);

    /*
     * Holds function pointers to the callback methods.
     */
    static AGpsRilCallbacks sAGnssRilCb;

    // Methods from IAGnssRil follow.
    Return<void> agpsNiMessage(const hidl_string& data,int32_t len);
 private:
    const AGpsRilInterface* mAGnssRilIface = nullptr;
    static sp<IAGnssRilCallback> sAGnssRilCbIface;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IAGnssRil* HIDL_FETCH_IAGnssRil(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_AGNSSRIL_H
