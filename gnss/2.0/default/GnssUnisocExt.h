#pragma once

#include "gnss_debug.h"
#include <ThreadCreationWrapper.h>
#include <vendor/sprd/hardware/gnss/2.0/IGnssUnisocExt.h>
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
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssUnisocExt : public IGnssUnisocExt {
    GnssUnisocExt(const GnssUnisocExtInterface* GnssUnisocIface);
    ~GnssUnisocExt();
    // Methods from ::vendor::sprd::hardware::gnss::V2_0::IGnssUnisocExt follow.
    Return<bool> setCallback(const sp<::vendor::sprd::hardware::gnss::V2_0::IGnssUnisocExtCallback>& callback) override;
    Return<bool> setNetWorkHandle(int64_t networkhandle) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
 private:
    const GnssUnisocExtInterface* mGnssUnisocExtIface = nullptr;
    static std::vector<std::unique_ptr<ThreadFuncArgs>> sThreadFuncArgsList;
    static bool sInterfaceExists;

};

// extern "C" IGnssUnisocExt* HIDL_FETCH_IGnssUnisocExt(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
