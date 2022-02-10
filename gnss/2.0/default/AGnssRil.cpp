#include "AGnssRil.h"

#define LOG_TAG "GnssHAL_V_AGnssRil"
namespace vendor {
namespace sprd {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

std::vector<std::unique_ptr<ThreadFuncArgs>> AGnssRil::sThreadFuncArgsList;
sp<IAGnssRilCallback> AGnssRil::sAGnssRilCbIface = nullptr;
bool AGnssRil::sInterfaceExists = false;

AGpsRilCallbacks AGnssRil::sAGnssRilCb = {
    .request_setid = AGnssRil::requestSetId,
    .request_refloc = AGnssRil::requestRefLoc,
    .create_thread_cb = AGnssRil::createThreadCb
};

AGnssRil::AGnssRil(const AGpsRilInterface* aGpsRilIface) : mAGnssRilIface(aGpsRilIface) {
    /* Error out if an instance of the interface already exists. */
    LOG_ALWAYS_FATAL_IF(sInterfaceExists);
    sInterfaceExists = true;
}

AGnssRil::~AGnssRil() {
    sThreadFuncArgsList.clear();
    sInterfaceExists = false;
}

void AGnssRil::requestSetId(uint32_t flags) {
    if (sAGnssRilCbIface == nullptr) {
        ALOGE("%s: AGnssRil Callback Interface configured incorrectly", __func__);
        return;
    }

    auto ret = sAGnssRilCbIface->requestSetIdCb(flags);
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}

void AGnssRil::requestRefLoc(uint32_t /*flags*/) {
    if (sAGnssRilCbIface == nullptr) {
        ALOGE("%s: AGnssRil Callback Interface configured incorrectly", __func__);
        return;
    }

    auto ret = sAGnssRilCbIface->requestRefLocCb();
    if (!ret.isOk()) {
        ALOGE("%s: Unable to invoke callback", __func__);
    }
}

pthread_t AGnssRil::createThreadCb(const char* name, void (*start)(void*), void* arg) {
    return createPthread(name, start, arg, &sThreadFuncArgsList);
}
// Methods from ::android::hardware::gnss::V1_0::IAGnssRil follow.
Return<void> AGnssRil::setCallback(const sp<::android::hardware::gnss::V1_0::IAGnssRilCallback>& callback) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRil interface is unavailable", __func__);
        return Void();
    }

    sAGnssRilCbIface = callback;

    mAGnssRilIface->init(&sAGnssRilCb);
    return Void();
}

#if 1
Return<void> AGnssRil::setRefLocation(const ::android::hardware::gnss::V1_0::IAGnssRil::AGnssRefLocation& aGnssReflocation) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRil interface is unavailable", __func__);
        return Void();
    }

    AGpsRefLocation aGnssRefloc;
    aGnssRefloc.type = static_cast<uint16_t>(aGnssReflocation.type);

    auto& cellID = aGnssReflocation.cellID;
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

Return<bool> AGnssRil::setSetId(::android::hardware::gnss::V1_0::IAGnssRil::SetIDType type, const hidl_string& setid) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRil interface is unavailable", __func__);
        return false;
    }

    mAGnssRilIface->set_set_id(static_cast<uint16_t>(type), setid.c_str());
    return true;
}
#endif

Return<bool> AGnssRil::updateNetworkState(bool connected, ::android::hardware::gnss::V1_0::IAGnssRil::NetworkType type, bool roaming) {

    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRil interface is unavailable", __func__);
        return false;
    }

    mAGnssRilIface->update_network_state(connected,
                                         static_cast<int>(type),
                                         roaming,
                                         nullptr /* extra_info */);
    return true;
}

Return<bool> AGnssRil::updateNetworkAvailability(bool available, const hidl_string& apn) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRil interface is unavailable", __func__);
        return false;
    }

    mAGnssRilIface->update_network_availability(available, apn.c_str());
    return true;

}


// Methods from ::android::hardware::gnss::V2_0::IAGnssRil follow.
Return<bool> AGnssRil::updateNetworkState_2_0(const ::android::hardware::gnss::V2_0::IAGnssRil::NetworkAttributes& attributes) {
    ALOGE("%s: Enter", __func__);
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRilExt interface is unavailable", __func__);
        return false;
    }

    mAGnssRilIface->update_network_state(attributes.isConnected,
                                         0,
                                         0/*roaming*/,
                                         nullptr /* extra_info */);
    return true;
}


// Methods from ::vendor::sprd::hardware::gnss::V2_0::IAGnssRil follow.
Return<void> AGnssRil::agpsNiMessage(const hidl_string& data, int32_t len) {
    if (mAGnssRilIface == nullptr) {
        ALOGE("%s: AGnssRil interface is unavailable", __func__);
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

//IAGnssRil* HIDL_FETCH_IAGnssRil(const char* /* name */) {
    //return new AGnssRil();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
