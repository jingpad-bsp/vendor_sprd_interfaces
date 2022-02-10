#ifndef VENDOR_SPRD_HARDWARE_LOG_V1_0_LOGCONTROL_H
#define VENDOR_SPRD_HARDWARE_LOG_V1_0_LOGCONTROL_H

#include <vendor/sprd/hardware/log/1.0/ILogControl.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>


namespace vendor {
namespace sprd {
namespace hardware {
namespace log {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct LogControl : public ILogControl {
    // Methods from ILogControl follow.
    Return<void> setCallback(const sp<ILogCallback>& callback) override;
    Return<void> sendCmd(const hidl_string& desSocket, const hidl_string& cmd, sendCmd_cb _hidl_cb) override;
    int processCmd(const char *socket, const char *cmd, char *response, int size);
    sp<ILogCallback> getCallback();
    private:
        sp<ILogCallback> mCallback;
    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" ILogControl* HIDL_FETCH_ILogControl(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace log
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_LOG_V1_0_LOGCONTROL_H
