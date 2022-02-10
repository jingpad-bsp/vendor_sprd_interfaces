#pragma once

#include "gnss_debug.h"
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IAGnssRil.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

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
using ::android::hardware::gnss::V2_0::IAGnssRil;
using ::android::hardware::gnss::V1_0::IAGnssRilCallback;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct AGnssRil : public IAGnssRil {
    AGnssRil(const AGpsRilInterface* aGpsRilIface);
    ~AGnssRil();
    // Methods from ::android::hardware::gnss::V1_0::IAGnssRil follow.
    Return<void> setCallback(const sp<::android::hardware::gnss::V1_0::IAGnssRilCallback>& callback) override;
    Return<void> setRefLocation(const ::android::hardware::gnss::V1_0::IAGnssRil::AGnssRefLocation& agnssReflocation) override;
    Return<bool> setSetId(::android::hardware::gnss::V1_0::IAGnssRil::SetIDType type, const hidl_string& setid) override;
    Return<bool> updateNetworkState(bool connected, ::android::hardware::gnss::V1_0::IAGnssRil::NetworkType type, bool roaming) override;
    Return<bool> updateNetworkAvailability(bool available, const hidl_string& apn) override;
    static void requestSetId(uint32_t flags);
    static void requestRefLoc(uint32_t flags);

    // Methods from ::android::hardware::gnss::V2_0::IAGnssRil follow.
    Return<bool> updateNetworkState_2_0(const ::android::hardware::gnss::V2_0::IAGnssRil::NetworkAttributes& attributes) override;

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

// extern "C" IAGnssRil* HIDL_FETCH_IAGnssRil(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
