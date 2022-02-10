#ifndef VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSCONFIGURATION_H
#define VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSCONFIGURATION_H

#include <vendor/sprd/hardware/gnss/1.0/IGnssConfiguration.h>
#include <hardware/gps.h>
#include <hidl/Status.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssConfiguration;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;

using ::android::hardware::Return;
using ::android::hardware::Void;

using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

/*
 * Interface for passing GNSS configuration info from platform to HAL.
 */
struct GnssConfiguration : public IGnssConfiguration {
    GnssConfiguration(const GnssConfigurationInterface* gnssConfigIface);

    /*
     * Methods from ::android::hardware::gnss::V1_0::IGnssConfiguration follow.
     * These declarations were generated from IGnssConfiguration.hal.
     */
    Return<bool> setSuplVersion(uint32_t version) override;
    Return<bool> setSuplMode(uint8_t mode) override;
    Return<bool> setSuplEs(bool enabled) override;
    Return<bool> setLppProfile(uint8_t lppProfile) override;
    Return<bool> setGlonassPositioningProtocol(uint8_t protocol) override;
    Return<bool> setEmergencySuplPdn(bool enable) override;
    Return<bool> setGpsLock(uint8_t lock) override;

 private:
    const GnssConfigurationInterface* mGnssConfigIface = nullptr;
};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IGnssConfiguration* HIDL_FETCH_IGnssConfiguration(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor

#endif  // VENDOR_SPRD_HARDWARE_GNSS_V1_0_GNSSCONFIGURATION_H
