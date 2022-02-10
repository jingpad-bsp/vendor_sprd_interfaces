#ifndef VENDOR_SPRD_HARDWARE_LOG_V1_0_LOGCALLBACK_H
#define VENDOR_SPRD_HARDWARE_LOG_V1_0_LOGCALLBACK_H

#include <vendor/sprd/hardware/log/1.0/ILogCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using vendor::sprd::hardware::log::V1_0::ILogCallback;
using ::android::sp;
namespace vendor {
namespace sprd {
namespace hardware {
namespace log {
namespace V1_0 {
namespace implementation  {
class LogCallback : public ILogCallback {
    // Methods from ILogCallback follow.
    public:
        LogCallback(char *socketName);
        Return<void> onCommand(const hidl_string& cmd, onCommand_cb _hidl_cb) override;
        int processCmd(const char *cmd, char *response, int size);
    private:
        char *mSocketName;
        int connect_socket_local_server();
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace log
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_LOG_V1_0_LOGCALLBACK_H
