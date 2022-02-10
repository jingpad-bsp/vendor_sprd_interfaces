#ifndef VENDOR_SPRD_HARDWARE_FINGERPRINTMMI_V1_0_FINGERPRINTMMI_H
#define VENDOR_SPRD_HARDWARE_FINGERPRINTMMI_V1_0_FINGERPRINTMMI_H

#include <vendor/sprd/hardware/fingerprintmmi/1.0/IFingerprintmmi.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <log/log.h>
#include <android/log.h>
#include <dlfcn.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace fingerprintmmi {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Fingerprintmmi : public IFingerprintmmi {
    // Methods from ::vendor::sprd::hardware::fingerprintmmi::V1_0::IFingerprintmmi follow.
    Return<uint32_t> factory_init() override;
    Return<uint32_t> spi_test() override;
    Return<uint32_t> deadpixel_test() override;
    Return<uint32_t> interrupt_test() override;
    Return<uint32_t> finger_detect() override;
    Return<uint32_t> factory_exit() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
extern "C" IFingerprintmmi* HIDL_FETCH_IFingerprintmmi(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace fingerprintmmi
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_FINGERPRINTMMI_V1_0_FINGERPRINTMMI_H
