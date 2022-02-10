#define LOG_TAG "GnssHAL_AGnssRilInterfaceExt"
#include "AGnssRilExt.h"

namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

std::vector<std::unique_ptr<ThreadFuncArgs>> AGnssRilExt::sThreadFuncArgsList;
sp<IAGnssRilCallback> AGnssRilExt::sAGnssRilCbIface = nullptr;
bool AGnssRilExt::sInterfaceExists = false;

AGpsRilCallbacks AGnssRilExt::sAGnssRilCb = {
    .request_setid = AGnssRilExt::requestSetId,
    .request_refloc = AGnssRilExt::requestRefLoc,
    .create_thread_cb = AGnssRilExt::createThreadCb
};

AGnssRilExt::AGnssRilExt(const AGpsRilInterface* aGpsRilIface) : mAGnssRilIface(aGpsRilIface) {
    /* Error out if an instance of the interface already exists. */
    LOG_ALWAYS_FATAL_IF(sInterfaceExists);
    sInterfaceExists = true;
}

AGnssRilExt::~AGnssRilExt() {
    sThreadFuncArgsList.clear();
    sInterfaceExists = false;
}

void AGnssRilExt::requestSetId(uint32_t flags) {
    if (sAGnssRilCbIface == nullptr) {
        ALOGE("%s: AGnssRilExt Callback Interface configured incorrectly", __func__);
        return;
    }

    auto ret = sAGnssRilCbIface->requestSetIdCb(flags);
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}

void AGnssRilExt::requestRefLoc(uint32_t /*flags*/) {
    if (sAGnssRilCbIface == nullptr) {
        ALOGE("%s: AGnssRilExt Callback Interface configured incorrectly", __func__);
        return;
    }

    auto ret = sAGnssRilCbIface->requestRefLocCb();
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}

pthread_t AGnssRilExt::createThreadCb(const char* name, void (*start)(void*), void* arg) {
    return createPthread(name, start, arg, &sThreadFuncArgsList);
}

// Methods from ::android::hardware::gnss::V1_0::IAGnssRil follow.
Return<void> AGnssRilExt::setCallback(const sp<::android::hardware::gnss::V1_0::IAGnssRilCallback>& callback) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRilExt interface is unavailable", __func__);
        return Void();
    }

    sAGnssRilCbIface = callback;

    mAGnssRilIface->init(&sAGnssRilCb);
    return Void();
}

Return<void> AGnssRilExt::setRefLocation(const ::android::hardware::gnss::V1_0::IAGnssRil::AGnssRefLocation& aGnssRefLocation)  {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRilExt interface is unavailable", __func__);
        return Void();
    }

    AGpsRefLocation aGnssRefloc;
    aGnssRefloc.type = static_cast<uint16_t>(aGnssRefLocation.type);

    auto& cellID = aGnssRefLocation.cellID;
    aGnssRefloc.u.cellID = {
        .type = static_cast<uint16_t>(cellID.type),
        .mcc = cellID.mcc,
        .mnc = cellID.mnc,
        .lac = cellID.lac,
        .cid = cellID.cid,
        .tac = cellID.tac,
        .pcid = cellID.pcid
    };

    mAGnssRilIface->set_ref_location(&aGnssRefloc, sizeof(aGnssRefloc));
    return Void();
}

Return<bool> AGnssRilExt::setSetId(::android::hardware::gnss::V1_0::IAGnssRil::SetIDType type, const hidl_string& setid) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRilExt interface is unavailable", __func__);
        return false;
    }

    mAGnssRilIface->set_set_id(static_cast<uint16_t>(type), setid.c_str());
    return true;
}

Return<bool> AGnssRilExt::updateNetworkState(bool connected, ::android::hardware::gnss::V1_0::IAGnssRil::NetworkType type, bool roaming) {

    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRilExt interface is unavailable", __func__);
        return false;
    }

    mAGnssRilIface->update_network_state(connected,
                                         static_cast<int>(type),
                                         roaming,
                                         nullptr /* extra_info */);
    return true;
}

Return<bool> AGnssRilExt::updateNetworkAvailability(bool available, const hidl_string& apn) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRilExt interface is unavailable", __func__);
        return false;
    }

    mAGnssRilIface->update_network_availability(available, apn.c_str());
    return true;
}
// Methods from IAGnssRil follow.
//Return<void> AGnssRil::agpsNiMessage(const hidl_string& data, uint16_t sz) {

Return<void> AGnssRilExt::agpsNiMessage(const hidl_string& data, int32_t len) { 
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRilExt interface is unavailable", __func__);
        return Void();
    }
	
    ALOGE("agpsNiMessage IN");		
    char* buf = new char[data.size()];
    const char* cString = data.c_str();
    memcpy(buf, cString, data.size());
	
    mAGnssRilIface->ni_message((uint8_t *)buf, data.size());
    ALOGE("agpsNiMessage out");
    delete[] buf;
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//AGnssRilExt* HIDL_FETCH_IAGnssRilExt(const char* /* name */) {
//    return new AGnssRilExt();
//}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
