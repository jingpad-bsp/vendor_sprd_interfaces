#define LOG_TAG "Ifaa[HIDL]"

#include <dlfcn.h>
#include <log/log.h>
#include <utils/Vector.h>
#include "Ifaa.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace ifaa {
namespace V1_0 {
namespace implementation {

#define OUT_BUF_EXTRA_SIZE (1024 * 1)

Ifaa::Ifaa() {
    int ret = hw_get_module_by_class(IFAA_HARDWARE_MODULE_ID, NULL, &mModule);
    mDevice = NULL;
    ALOGD("after hw_get_module_by_class, ret = %d\n", ret);
    if (!ret) {
        ALOGD("before ifaa_open.\n");
        ret = ifaa_open(mModule, &mDevice);
        ALOGD("after ifaa_open.\n");
    }
    if (ret < 0) {
        LOG_ALWAYS_FATAL_IF(ret < 0, "Unable to open IFAA HAL");
    }
}

Ifaa::~Ifaa() {
    if (mDevice != nullptr) {
        int ret = ifaa_close(mDevice);
        if (ret < 0) {
            ALOGE("Unable to close IFAA HAL");
        }
    }
    dlclose(mModule->dso);
}

// Methods from IIfaa follow.
Return<void> Ifaa::processCmdV2(const hidl_vec<uint8_t>& param, processCmdV2_cb _hidl_cb) {
    ALOGD("Ifaa[HIDL]: ==processCmdV2 >>>>>>");
    ssize_t out_size = param.size() + OUT_BUF_EXTRA_SIZE;

    android::Vector<uint8_t> out;
    out.resize(out_size);
    ALOGD("Ifaa[HIDL]: processCmdV2-> out_size = %d\n", out_size);
    int result = mDevice->process_cmd_v2(mDevice, param.data(), param.size(), out.editArray(), &out_size);
    out.resize(out_size);
    ALOGD("Ifaa[HIDL]: processCmdV2-> after out_size = %d\n", out_size);
    hidl_vec<uint8_t> out_vec;
    out_vec.setToExternal(out.editArray(), out.size());
    _hidl_cb(out_vec);
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IIfaa* HIDL_FETCH_IIfaa(const char* /* name */) {
    return new Ifaa();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace ifaa
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
