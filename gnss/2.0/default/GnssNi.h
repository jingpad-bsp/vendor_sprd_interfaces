#pragma once

#include "gnss_debug.h"
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IGnssNi.h>
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

using ::android::hardware::gnss::V1_0::IGnssNi;
using ::android::hardware::gnss::V1_0::IGnssNiCallback;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssNi : public IGnssNi {
    GnssNi(const GpsNiInterface* gpsNiIface);
    ~GnssNi();
    // Methods from ::android::hardware::gnss::V1_0::IGnssNi follow.
    Return<void> setCallback(const sp<::android::hardware::gnss::V1_0::IGnssNiCallback>& callback) override;
    Return<void> respond(int32_t notifId,
						::android::hardware::gnss::V1_0::IGnssNiCallback::GnssUserResponseType userResponse) override;

    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);
    static void niNotifyCb(GpsNiNotification* notification);
    // Methods from ::android::hidl::base::V1_0::IBase follow.
    static GpsNiCallbacks sGnssNiCb;

 private:
    const GpsNiInterface* mGnssNiIface = nullptr;
    static sp<IGnssNiCallback> sGnssNiCbIface;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;
};

// extern "C" IGnssNi* HIDL_FETCH_IGnssNi(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
