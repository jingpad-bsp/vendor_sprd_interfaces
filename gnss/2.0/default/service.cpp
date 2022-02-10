#define LOG_TAG "GnssHAL_V_Service"

#include <vendor/sprd/hardware/gnss/2.0/IGnss.h>
#include <hidl/LegacySupport.h>

#include <binder/ProcessState.h>

using vendor::sprd::hardware::gnss::V2_0::IGnss;
using android::hardware::defaultPassthroughServiceImplementation;

int main() {
    // The GNSS HAL may communicate to other vendor components via
    // /dev/vndbinder
    android::ProcessState::initWithDriver("/dev/vndbinder");
    return defaultPassthroughServiceImplementation<IGnss>();
}
