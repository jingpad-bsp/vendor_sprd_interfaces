#define LOG_TAG "vendor.sprd.hardware.gnss@1.0-service"

#include <vendor/sprd/hardware/gnss/1.0/IGnssExt.h>
#include <hidl/LegacySupport.h>

#include <binder/ProcessState.h>

using vendor::sprd::hardware::gnss::V1_0::IGnssExt;
using android::hardware::defaultPassthroughServiceImplementation;

int main() {
    // The GNSS HAL may communicate to other vendor components via
    // /dev/vndbinder
    android::ProcessState::initWithDriver("/dev/vndbinder");
    return defaultPassthroughServiceImplementation<IGnssExt>();
}
