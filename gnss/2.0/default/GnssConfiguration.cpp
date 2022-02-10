#define LOG_TAG "GnssHAL_V_GnssConfiguration"
#include "GnssConfiguration.h"
#include <log/log.h>
namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

GnssConfiguration::GnssConfiguration(const GnssConfigurationInterface* gnssConfigInfc)
    : mGnssConfigIface(gnssConfigInfc) {}
// Methods from ::android::hardware::gps::V1_0::IGnssConfiguration follow.
Return<bool> GnssConfiguration::setSuplEs(bool enabled)  {
    if (mGnssConfigIface == nullptr) {
        ALOGE("%s: GNSS Configuration interface is not available.", __func__);
        return false;
    }

    std::string config = "SUPL_ES=" + std::to_string(enabled ? 1 : 0) + "\n";
    mGnssConfigIface->configuration_update(config.c_str(), config.size());
    return enabled;
}

Return<bool> GnssConfiguration::setSuplVersion(uint32_t version)  {
    if (mGnssConfigIface == nullptr) {
        ALOGE("%s: GNSS Configuration interface is not available.", __func__);
        return false;
    }

    std::string config = "SUPL_VER=" + std::to_string(version) + "\n";
    mGnssConfigIface->configuration_update(config.c_str(), config.size());

    return true;
}

Return<bool> GnssConfiguration::setSuplMode(uint8_t mode)  {
    if (mGnssConfigIface == nullptr) {
        ALOGE("%s: GNSS Configuration interface is not available.", __func__);
        return false;
    }

    std::string config = "SUPL_MODE=" + std::to_string(mode) + "\n";
    mGnssConfigIface->configuration_update(config.c_str(), config.size());
    return true;
}

Return<bool> GnssConfiguration::setLppProfile(uint8_t lppProfile) {
    if (mGnssConfigIface == nullptr) {
        ALOGE("%s: GNSS Configuration interface is not available.", __func__);
        return false;
    }

    std::string config = "LPP_PROFILE=" + std::to_string(lppProfile) + "\n";
    mGnssConfigIface->configuration_update(config.c_str(), config.size());
    return true;
}

Return<bool> GnssConfiguration::setGlonassPositioningProtocol(uint8_t protocol) {
    if (mGnssConfigIface == nullptr) {
        ALOGE("%s: GNSS Configuration interface is not available.", __func__);
        return false;
    }

    std::string config = "A_GLONASS_POS_PROTOCOL_SELECT=" +
            std::to_string(protocol) + "\n";
    mGnssConfigIface->configuration_update(config.c_str(), config.size());
    return true;
}

Return<bool> GnssConfiguration::setGpsLock(uint8_t lock) {
    if (mGnssConfigIface == nullptr) {
        ALOGE("%s: GNSS Configuration interface is not available.", __func__);
        return false;
    }

    std::string config = "GPS_LOCK=" + std::to_string(lock) + "\n";
    mGnssConfigIface->configuration_update(config.c_str(), config.size());
    if(lock) {
        ALOGD("%s lock is %d", lock);
        return true;
    }
    return false;
}

Return<bool> GnssConfiguration::setEmergencySuplPdn(bool enabled) {
    if (mGnssConfigIface == nullptr) {
        ALOGE("%s: GNSS Configuration interface is not available.", __func__);
        return false;
    }

    std::string config = "USE_EMERGENCY_PDN_FOR_EMERGENCY_SUPL=" + std::to_string(enabled ? 1 : 0)
            + "\n";
    mGnssConfigIface->configuration_update(config.c_str(), config.size());
    return true;
}


// Methods from ::android::hardware::gnss::V1_1::IGnssConfiguration follow.
Return<bool> GnssConfiguration::setBlacklist(const hidl_vec<::android::hardware::gnss::V1_1::IGnssConfiguration::BlacklistedSource>& blacklist) {
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    mBlacklistedConstellationSet.clear();
    mBlacklistedSourceSet.clear();
    struct BL{
       uint8_t blackConstellation;
        int svid;
    };
    struct BLC{
        uint8_t BlackConstellationMask;
        BL svinfo;
    };

    struct CONFIG_UPDATE{
        uint8_t type;
        char* data;
    };

    BLC blc_data;
    CONFIG_UPDATE config_data;
    int temp_constellation;



    memset(&blc_data,0,sizeof(BLC));
    for (auto source : blacklist) {
        if (source.svid == 0) {
            // Wildcard blacklist, i.e., blacklist entire constellation.
            mBlacklistedConstellationSet.insert(source.constellation);
            ALOGE("%s: constellation :%d", __func__,source.constellation);
            temp_constellation = (int)source.constellation;
            if((temp_constellation > 0)&&(temp_constellation < 7)){
                blc_data.BlackConstellationMask |= (1 << temp_constellation);
                ALOGE("%s: mask %d", __func__,blc_data.BlackConstellationMask);
            }
        } else {
            mBlacklistedSourceSet.insert(source);
            ALOGE("%s: constellation %d svid %d.", __func__,
                source.constellation, source.svid);
            temp_constellation = (int)source.constellation;
            if((temp_constellation  > 0)&&(temp_constellation  < 7)){
                blc_data.svinfo.blackConstellation |= (1 << temp_constellation);
                blc_data.svinfo.svid = source.svid;
                ALOGE("%s:svinof constelltion %d ,svid %d", __func__,
                    blc_data.svinfo.blackConstellation, blc_data.svinfo.svid);
            }
        }
    }

    if (mGnssConfigIface == nullptr)
    {
        ALOGE("%s: GNSS config inface invaled.", __func__);
        return true;
    }
    else
    {
        config_data.type = 33;
        config_data.data = (char *)&blc_data;
        mGnssConfigIface->configuration_update((char *)&config_data, sizeof(CONFIG_UPDATE));
        ALOGE("%s: update mask:%d con %d ,svid %d config size %d", __func__,
        blc_data.BlackConstellationMask, blc_data.svinfo.blackConstellation,
        blc_data.svinfo.svid, sizeof(CONFIG_UPDATE));
    }

    return true;
}



// Methods from ::android::hardware::gnss::V2_0::IGnssConfiguration follow.
Return<bool> GnssConfiguration::setEsExtensionSec(uint32_t emergencyExtensionSeconds) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IGnssConfiguration* HIDL_FETCH_IGnssConfiguration(const char* /* name */) {
    //return new GnssConfiguration();
//}
//
Return<bool> GnssConfiguration::isBlacklisted(const GnssSvInfo& gnssSvInfo) const {
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    if (mBlacklistedConstellationSet.find(gnssSvInfo.constellation) !=
        mBlacklistedConstellationSet.end()) {
        return true;
    }
    BlacklistedSource source = {.constellation = gnssSvInfo.constellation, .svid = gnssSvInfo.svid};
    return (mBlacklistedSourceSet.find(source) != mBlacklistedSourceSet.end());
}

std::recursive_mutex& GnssConfiguration::getMutex() const {
    return mMutex;
}
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
