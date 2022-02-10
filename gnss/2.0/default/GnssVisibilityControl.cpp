/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "GnssHAL_V_GnssVisibilityControl"

#include "GnssVisibilityControl.h"
#include <log/log.h>

namespace android {
namespace hardware {
namespace gnss {
namespace visibility_control {
namespace V1_0 {
namespace implementation {

sp<::android::hardware::gnss::visibility_control::V1_0::IGnssVisibilityControlCallback>
	GnssVisibilityControl::sGnssVisibilityControlCbIface = nullptr;
GnssVisibilityControlCallbacks GnssVisibilityControl::sGnssVisibilityControlCbs = {
    .size = sizeof(GnssVisibilityControlCallbacks),
    .nfw_notify_callback = noFrameworkNotifyCb,
    .is_in_emergency_session_calback = isInEmergencySessionCb,
};

GnssVisibilityControl::GnssVisibilityControl(const GpsVisibilityControlInterface* gpsVisibilityControlIface)
    : mGnssVisibilityControlIface(gpsVisibilityControlIface) 
{
    ALOGD("%s: Enter", __func__);
}


// Methods from ::android::hardware::gnss::visibility_control::V1_0::IGnssVisibilityControl follow.
Return<bool> GnssVisibilityControl::enableNfwLocationAccess(
        const hidl_vec<hidl_string>& proxyApps) {
    int ret = 0;
    std::string os;
    bool first = true;
    for (const auto& proxyApp : proxyApps) {
        if (first) {
            first = false;
        } else {
            os += " ";
        }

        os += proxyApp;
    }

    ALOGD("enableNfwLocationAccess proxyApps: %s", os.c_str());
    ret =  mGnssVisibilityControlIface->enableNfwLocationAccess(os.c_str());
    if(ret == 0){
        return true;
    }
    return false;
}

Return<bool> GnssVisibilityControl::setCallback(const sp<::android::hardware::gnss::visibility_control::V1_0::IGnssVisibilityControlCallback>& callback) {
	ALOGD("%s enter", __func__);
	if (callback == nullptr)
	{
		ALOGE("%s: Null callback ignored", __func__);
		return false;
	}
	sGnssVisibilityControlCbIface = callback;
	return (mGnssVisibilityControlIface->init(&sGnssVisibilityControlCbs) == 0);
}

void GnssVisibilityControl::noFrameworkNotifyCb(GnssNfwNotification* notification)
{
	NfwNotification nfwNotify = {};
	ALOGD("%s enter", __func__);

	if (sGnssVisibilityControlCbIface == nullptr) {
	ALOGE("%s: GNSS VisibilityControl Callback Interface configured incorrectly", __func__);
	return;
	}

	if (notification == nullptr) {
	ALOGE("%s: Invalid GnssVisibilityControl callback from GNSS HAL", __func__);
	return;
	}

	nfwNotify = {
	//string
	.proxyAppPackageName = notification->proxyAppPackageName,
	//enum: NfwProtocolStack
	.protocolStack = static_cast<IGnssVisibilityControlCallback::NfwProtocolStack>(notification->protocolStack),
	//string
	.otherProtocolStackName	 = notification->otherProtocolStackName,
	//enum: NfwRequestor
	.requestor = static_cast<IGnssVisibilityControlCallback::NfwRequestor>(notification->requestor),
	//string
	.requestorId = notification->requestorId,
	//enum: NfwResponseType
	.responseType = static_cast<IGnssVisibilityControlCallback::NfwResponseType>(notification->responseType),
	//bool
	.inEmergencyMode = notification->inEmergencyMode, 
	//bool
	.isCachedLocation = notification->isCachedLocation
	};

	sGnssVisibilityControlCbIface->nfwNotifyCb(nfwNotify);

	return;
}

bool GnssVisibilityControl::isInEmergencySessionCb() 
{
    return sGnssVisibilityControlCbIface->isInEmergencySession();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace visibility_control
}  // namespace gnss
}  // namespace hardware
}  // namespace android
