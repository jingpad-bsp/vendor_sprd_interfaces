#ifndef VENDOR_SPRD_HARDWARE_CONNMGR_V1_0_CONNMGR_H
#define VENDOR_SPRD_HARDWARE_CONNMGR_V1_0_CONNMGR_H

#include <vendor/sprd/hardware/connmgr/1.0/IConnmgr.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace connmgr {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Connmgr : public IConnmgr {
    // Methods from IConnmgr follow.
    Return<bool> registerCallback(const sp<IConnmgrCallback>& callback) override;
    Return<void> SendStringCommand(const hidl_string& type, SendStringCommand_cb _hidl_cb) override;
    sp<IConnmgrCallback> mCallback;
	private:
         std::string SendStringCommandInternal(
                                           const hidl_string& type);
template <typename ObjT, typename WorkFuncT, typename... Args>
Return<void> validateAndCall(
    ObjT* obj, WorkFuncT&& work,
    const std::function<void(const std::string&)>& hidl_cb, Args&&... args)
{
	if (true) {
		hidl_cb((obj->*work)(std::forward<Args>(args)...));
	} else {
		hidl_cb("");
	}
	return Void();
}
    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
 extern "C" IConnmgr* HIDL_FETCH_IConnmgr(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace connmgr
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_CONNMGR_V1_0_CONNMGR_H
