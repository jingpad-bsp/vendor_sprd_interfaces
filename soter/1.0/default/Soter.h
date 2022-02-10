#ifndef VENDOR_SPRD_HARDWARE_SOTER_V1_0_SOTER_H
#define VENDOR_SPRD_HARDWARE_SOTER_V1_0_SOTER_H

#include <vendor/sprd/hardware/soter/1.0/ISoter.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace soter {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Soter : public ISoter {

public:
    static ISoter *getInstance();
    ~Soter();

    // Methods from ::vendor::sprd::hardware::soter::V1_0::ISoter follow.
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> generateAttkKeyPair(uint8_t copyNum) override;
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> verifyAttkKeyPair() override;
    Return<void> exportAttkPublicKey(exportAttkPublicKey_cb _hidl_cb) override;
    Return<void> getDeviceId(getDeviceId_cb _hidl_cb) override;
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> generateAskKeyPair(uint32_t uid) override;
    Return<void> exportAskPublicKey(uint32_t uid, exportAskPublicKey_cb _hidl_cb) override;
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> removeAllUidKey(uint32_t uid) override;
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> hasAskAlready(uint32_t uid) override;
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> generateAuthKeyPair(uint32_t uid, const hidl_string& name) override;
    Return<void> exportAuthKeyPublicKey(uint32_t uid, const hidl_string& name, exportAuthKeyPublicKey_cb _hidl_cb) override;
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> removeAuthKey(uint32_t uid, const hidl_string& name) override;
    Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> hasAuthKey(uint32_t uid, const hidl_string& name) override;
    Return<void> initSign(uint32_t uid, const hidl_string& name, const hidl_string& challenge, initSign_cb _hidl_cb) override;
    Return<void> finishSign(uint64_t session, finishSign_cb _hidl_cb) override;

private:
    static Soter *sInstance;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" ISoter* HIDL_FETCH_ISoter(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace soter
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_SOTER_V1_0_SOTER_H
