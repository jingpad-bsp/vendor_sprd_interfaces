#ifndef VENDOR_SPRD_HARDWARE_APRD_V1_0_APRDINFOSYNC_H
#define VENDOR_SPRD_HARDWARE_APRD_V1_0_APRDINFOSYNC_H

#include <vendor/sprd/hardware/aprd/1.0/IAprdInfoSync.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <semaphore.h>
extern "C"{
#include <pthread.h>
};
namespace vendor {
namespace sprd {
namespace hardware {
namespace aprd {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct AprdInfoSync : public IAprdInfoSync {
    // Methods from ::vendor::sprd::hardware::aprd::V1_0::IAprdInfoSync follow.
    AprdInfoSync();
    ~AprdInfoSync();
    Return<void> setCallback(const sp<::vendor::sprd::hardware::aprd::V1_0::IAprdCallback>& callback) override;
    Return<void> sendCmd(const hidl_string& cmd, sendCmd_cb _hidl_cb) override;
    static void* LVendorFunc(void *arg);
    sp<IAprdCallback> getcallback();
private:
    sp<IAprdCallback> mCallback;
    // Methods from ::android::hidl::base::V1_0::IBase follow.
	sem_t modem_sem;
	sem_t wcn_sem;
	sem_t gps_sem;
	sem_t modem_exp_sem;
	sem_t prop_sem;

};

// FIXME: most likely delete, this is only for passthrough implementations
extern "C" IAprdInfoSync* HIDL_FETCH_IAprdInfoSync(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace aprd
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_APRD_V1_0_APRDINFOSYNC_H
