#pragma once

#include <vendor/sprd/hardware/gnss/2.0/IGnssBatching.h>
#include "gnss_debug.h"

#ifdef GNSS_HIDL_DEBUG
#include "fused_location.h"
#else
#include <hardware/fused_location.h>
#endif
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::gnss::V2_0::IGnssBatching;
using ::android::hardware::gnss::V2_0::IGnssBatchingCallback;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssBatching : public IGnssBatching {
    GnssBatching(const FlpLocationInterface* flpLocationIface);
    // Methods from ::android::hardware::gnss::V1_0::IGnssBatching follow.
    Return<bool> init(const sp<::android::hardware::gnss::V1_0::IGnssBatchingCallback>& callback) override;
    Return<uint16_t> getBatchSize() override;
    Return<bool> start(const ::android::hardware::gnss::V1_0::IGnssBatching::Options& options) override;
    Return<void> flush() override;
    Return<bool> stop() override;
    Return<void> cleanup() override;
    // Methods from ::android::hardware::gnss::V2_0::IGnssBatching follow.
    Return<bool> init_2_0(const sp<::android::hardware::gnss::V2_0::IGnssBatchingCallback>& callback) override;

    static void locationCb(int32_t locationsCount, FlpLocation** locations);
    static void acquireWakelockCb();
    static void releaseWakelockCb();
    static int32_t setThreadEventCb(ThreadEvent event);
    static void flpCapabilitiesCb(int32_t capabilities);
    static void flpStatusCb(int32_t status);
    // Methods from ::android::hidl::base::V1_0::IBase follow.

    static FlpCallbacks sFlpCb;

 private:
    const FlpLocationInterface* mFlpLocationIface = nullptr;
    static sp<IGnssBatchingCallback> sGnssBatchingCbIface;
    static bool sFlpSupportsBatching;
};

// extern "C" IGnssBatching* HIDL_FETCH_IGnssBatching(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
