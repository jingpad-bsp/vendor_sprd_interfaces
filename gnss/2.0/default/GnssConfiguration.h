#pragma once

#include "gnss_debug.h"
#include <vendor/sprd/hardware/gnss/2.0/IGnssCallback.h>
#include <vendor/sprd/hardware/gnss/2.0/IGnssConfiguration.h>
#include <hidl/MQDescriptor.h>
#ifdef GNSS_HIDL_DEBUG
#include "gps.h"
#else
#include <hardware/gps.h>
#endif
#include <hidl/Status.h>
#include <mutex>
#include <unordered_set>

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

using BlacklistedSource = ::android::hardware::gnss::V1_1::IGnssConfiguration::BlacklistedSource;
using GnssConstellationType = ::android::hardware::gnss::V1_0::GnssConstellationType;
using GnssSvInfo = ::android::hardware::gnss::V1_0::IGnssCallback::GnssSvInfo;

struct BlacklistedSourceHash {
    inline int operator()(const BlacklistedSource& source) const {
        return int(source.constellation) * 1000 + int(source.svid);
    }
};

struct BlacklistedSourceEqual {
    inline bool operator()(const BlacklistedSource& s1, const BlacklistedSource& s2) const {
        return (s1.constellation == s2.constellation) && (s1.svid == s2.svid);
    }
};

using BlacklistedSourceSet =
    std::unordered_set<BlacklistedSource, BlacklistedSourceHash, BlacklistedSourceEqual>;
using BlacklistedConstellationSet = std::unordered_set<GnssConstellationType>;

struct GnssConfiguration : public IGnssConfiguration {
    GnssConfiguration(const GnssConfigurationInterface* gnssConfigIface);
    // Methods from ::android::hardware::gnss::V1_0::IGnssConfiguration follow.
    Return<bool> setSuplEs(bool enabled) override;
    Return<bool> setSuplVersion(uint32_t version) override;
    Return<bool> setSuplMode(uint8_t mode) override;
    Return<bool> setGpsLock(uint8_t lock) override;
    Return<bool> setLppProfile(uint8_t lppProfile) override;
    Return<bool> setGlonassPositioningProtocol(uint8_t protocol) override;
    Return<bool> setEmergencySuplPdn(bool enable) override;

    // Methods from ::android::hardware::gnss::V1_1::IGnssConfiguration follow.
    Return<bool> setBlacklist(const hidl_vec<::android::hardware::gnss::V1_1::IGnssConfiguration::BlacklistedSource>& blacklist) override;

    // Methods from ::android::hardware::gnss::V2_0::IGnssConfiguration follow.
    Return<bool> setEsExtensionSec(uint32_t emergencyExtensionSeconds) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    Return<bool> isBlacklisted(const GnssSvInfo& gnssSvInfo) const;
    std::recursive_mutex& getMutex() const;

   private:
    BlacklistedSourceSet mBlacklistedSourceSet;
    BlacklistedConstellationSet mBlacklistedConstellationSet;
    mutable std::recursive_mutex mMutex;
    const GnssConfigurationInterface* mGnssConfigIface = nullptr;
};

// extern "C" IGnssConfiguration* HIDL_FETCH_IGnssConfiguration(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
