#pragma once

#include "gnss_debug.h"
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IGnssXtra.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssXtra;
using ::android::hardware::gnss::V1_0::IGnssXtraCallback;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssXtra : public IGnssXtra {
    GnssXtra(const GpsXtraInterface* xtraIface);
    ~GnssXtra();
    // Methods from ::android::hardware::gnss::V1_0::IGnssXtra follow.
    Return<bool> setCallback(const sp<::android::hardware::gnss::V1_0::IGnssXtraCallback>& callback) override;
    Return<bool> injectXtraData(const hidl_string& xtraData) override;

    static pthread_t createThreadCb(const char* name, void (*start)(void*), void* arg);
    static void gnssXtraDownloadRequestCb();
    static GpsXtraCallbacks sGnssXtraCb;

 private:
    const GpsXtraInterface* mGnssXtraIface = nullptr;
    static sp<IGnssXtraCallback> sGnssXtraCbIface;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;
};

// extern "C" IGnssXtra* HIDL_FETCH_IGnssXtra(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
