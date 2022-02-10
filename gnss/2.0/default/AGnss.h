#include "gnss_debug.h"
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IAGnss.h>
//#include <android/hardware/gnss/1.0/IAGnss.h>

#include <hidl/MQDescriptor.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps_internal.h"
#else
#include <hardware/gps_internal.h>
#endif
#include <hidl/Status.h>
#include <netinet/in.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::gnss::V2_0::IAGnss;
using ::android::hardware::gnss::V2_0::IAGnssCallback;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

struct AGnss : public IAGnss {
    AGnss(const AGpsInterface* agpsIface);
    ~AGnss();
    // Methods from ::android::hardware::gnss::V2_0::IAGnss follow.
    Return<void> setCallback(const sp<IAGnssCallback>& callback) override;
    Return<bool> dataConnClosed() override;
    Return<bool> dataConnFailed() override;
    Return<bool> setServer(IAGnssCallback::AGnssType type,
            const hidl_string& hostname, int32_t port);
    Return<bool> dataConnOpen(uint64_t networkHandle, const hidl_string& apn,
                                IAGnss::ApnIpType apnIpType);

    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);
    static void statusCb(AGpsStatus* status);

    static AGpsCallbacks sAGnssCb;

 private:
    const AGpsInterface* mAGnssIface = nullptr;
    static sp<IAGnssCallback> sAGnssCbIface;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;
};

// extern "C" IAGnss* HIDL_FETCH_IAGnss(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
