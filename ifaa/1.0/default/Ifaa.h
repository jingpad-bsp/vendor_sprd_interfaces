#ifndef VENDOR_SPRD_HARDWARE_IFAA_V1_0_IFAA_H
#define VENDOR_SPRD_HARDWARE_IFAA_V1_0_IFAA_H

#include <vendor/sprd/hardware/ifaa/1.0/IIfaa.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <hardware/hardware.h>
#include <hardware/ifaa.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace ifaa {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Ifaa : public IIfaa {
public:
    Ifaa();
    ~Ifaa();
    // Methods from IIfaa follow.
    Return<void> processCmdV2(const hidl_vec<uint8_t>& param, processCmdV2_cb _hidl_cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
private:
    ifaa_device_t *mDevice;
    const hw_module_t *mModule;
};

 extern "C" IIfaa* HIDL_FETCH_IIfaa(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace ifaa
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_IFAA_V1_0_IFAA_H
