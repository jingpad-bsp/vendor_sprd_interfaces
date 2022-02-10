#include <log/log.h>
extern "C" {
#include <soter_tee.h>
}

#include "Soter.h"

#define LOG_TAG "Soter[HIDL]"
#define LOG_NDDEBUG 0

#define DEFAULT_DEVICE_ID_SIZE 40
#define DEFAULT_SIGN_RESULT_SIZE 1536

namespace vendor {
namespace sprd {
namespace hardware {
namespace soter {
namespace V1_0 {
namespace implementation {

typedef int32_t ca_ret;

Soter *Soter::sInstance = nullptr;

ISoter *Soter::getInstance() {
    // connect ta now
    ca_ret rc = soter_open_tee();
    if (rc != 0) {
        ALOGE("soter_open_tee() error, just return null");
        return nullptr;
    }

    if (!sInstance) {
        sInstance = new Soter();
    }
    ALOGD("soter hidl initilized...");
    return sInstance;
}

Soter::~Soter() {
    // disconnect ta
    soter_close_tee();
    ALOGD("soter hidl destructor...");
}

// Methods from ::vendor::sprd::hardware::soter::V1_0::ISoter follow.
Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::generateAttkKeyPair(uint8_t copyNum) {
    ALOGD("generateAttkKeyPair(copyNum=%d)", copyNum);

    ca_ret rc = generate_attk_key_pair(copyNum);
    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::verifyAttkKeyPair() {
    ca_ret rc = verify_attk_key_pair();
    ALOGD("%s rc=%d", __func__, rc);

    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<void> Soter::exportAttkPublicKey(exportAttkPublicKey_cb _hidl_cb) {
    uint8_t *key = new uint8_t[EXPORT_KEY_BUF_SIZE];
    size_t key_size = 0;
    hidl_vec<uint8_t> out_vec;

    memset(key, 0, EXPORT_KEY_BUF_SIZE);
    ca_ret rc = export_attk_public_key(key, &key_size);
    ALOGD("%s exported key_len=%u", __func__, key_size);

    out_vec.setToExternal(key, key_size);
    _hidl_cb(static_cast<SoterErrorCode>(rc), out_vec, key_size);
    delete[] key;
    return Void();
}

Return<void> Soter::getDeviceId(getDeviceId_cb _hidl_cb) {
    uint8_t device_id[DEFAULT_DEVICE_ID_SIZE] = { '\0' };
    size_t size = 0;
    hidl_vec<uint8_t> out_vec;

    // we get all data from ask.json, ask.signature is uesless
    ca_ret rc = get_device_id(device_id, &size);
    ALOGD("%s: device id=%s(%u)", __func__, device_id, size);

    out_vec.setToExternal(device_id, size);
    _hidl_cb(static_cast<SoterErrorCode>(rc), out_vec, size);
    return Void();
}

Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::generateAskKeyPair(uint32_t uid) {
    ALOGD("generateAskKeyPair(uid=%u)", uid);

    ca_ret rc = generate_ask_key_pair(uid);
    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<void> Soter::exportAskPublicKey(uint32_t uid, exportAskPublicKey_cb _hidl_cb) {
    uint8_t *json = new uint8_t[EXPORT_KEY_BUF_SIZE];
    soter_ask_t ask;
    hidl_vec<uint8_t> out_vec;

    // we get all data from ask.json, ask.signature is uesless
    memset(json, 0, EXPORT_KEY_BUF_SIZE);
    memset(&ask, 0, sizeof(soter_ask_t));
    ask.json = (const char *)json;
    ca_ret rc = export_ask_public_key(uid, &ask);
    ALOGD("%s ask.json_len=%u ask.sign_len=%u", __func__, ask.json_length, ask.signature_length);

    out_vec.setToExternal(json, ask.json_length);
    _hidl_cb(static_cast<SoterErrorCode>(rc), out_vec, ask.json_length);
    delete[] json;
    return Void();
}

Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::removeAllUidKey(uint32_t uid) {
    ALOGD("removeAllUidKey(uid=%u)", uid);

    ca_ret rc = remove_all_uid_key(uid);
    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::hasAskAlready(uint32_t uid) {
    ALOGD("hasAskAlready(uid=%u)", uid);

    ca_ret rc = has_ask_already(uid);
    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::generateAuthKeyPair(uint32_t uid, const hidl_string& name) {
    ALOGD("generateAuthKeyPair(uid=%u, name=%s)", uid, name.c_str());

    ca_ret rc = generate_auth_key_pair(uid, name.c_str());
    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<void> Soter::exportAuthKeyPublicKey(uint32_t uid, const hidl_string& name, exportAuthKeyPublicKey_cb _hidl_cb) {
    uint8_t *json = new uint8_t[EXPORT_KEY_BUF_SIZE];
    soter_auth_key_t ak;
    hidl_vec<uint8_t> out_vec;

    // we get all data from ask.json, ask.signature is uesless
    memset(json, 0, EXPORT_KEY_BUF_SIZE);
    memset(&ak, 0, sizeof(soter_auth_key_t));
    ak.json = (const char *) json;
    ca_ret rc = export_auth_key_public_key(uid, name.c_str(), &ak);
    ALOGD("%s ak.json_len=%u ak.sign_len=%u", __func__, ak.json_length, ak.signature_length);

    out_vec.setToExternal(json, ak.json_length);
    _hidl_cb(static_cast<SoterErrorCode>(rc), out_vec, ak.json_length);
    delete[] json;
    return Void();
}

Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::removeAuthKey(uint32_t uid, const hidl_string& name) {
    ALOGD("removeAuthKey(uid=%u, name=%s)", uid, name.c_str());

    ca_ret rc = remove_auth_key(uid, name.c_str());
    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<::vendor::sprd::hardware::soter::V1_0::SoterErrorCode> Soter::hasAuthKey(uint32_t uid, const hidl_string& name) {
    ALOGD("hasAuthKey(uid=%u, name=%s)", uid, name.c_str());

    ca_ret rc = has_auth_key(uid, name.c_str());
    return ::vendor::sprd::hardware::soter::V1_0::SoterErrorCode { static_cast<SoterErrorCode>(rc) };
}

Return<void> Soter::initSign(uint32_t uid, const hidl_string& name, const hidl_string& challenge, initSign_cb _hidl_cb) {
    uint64_t id = 0;
    soter_sign_session_t session;

    memset(&session, 0, sizeof(soter_sign_session_t));
    session.session = (const char *) &id;
    ca_ret rc = init_sign(uid, name.c_str(), challenge.c_str(), &session);
    ALOGD("%s session.length=%u", __func__, session.session_length);

    _hidl_cb(static_cast<SoterErrorCode>(rc), id);
    return Void();
}

Return<void> Soter::finishSign(uint64_t id, finishSign_cb _hidl_cb) {
    uint8_t *json = new uint8_t[DEFAULT_SIGN_RESULT_SIZE];
    soter_sign_session_t session;
    soter_sign_result_t result;
    hidl_vec<uint8_t> out_vec;

    memset(&session, 0, sizeof(soter_sign_session_t));
    memset(json, 0, DEFAULT_SIGN_RESULT_SIZE);
    memset(&result, 0, sizeof(soter_sign_result_t));
    session.session = (const char *) &id;
    session.session_length = sizeof(uint64_t);
    result.json = (const char *)json;
    ca_ret rc = finish_sign(&session, &result);
    ALOGE("finishSign:%d %s", result.json_length, json);

    out_vec.setToExternal(json, result.json_length);
    _hidl_cb(static_cast<SoterErrorCode>(rc), out_vec, result.json_length);
    delete[] json;
    return Void();
}



// Methods from ::android::hidl::base::V1_0::IBase follow.

//ISoter* HIDL_FETCH_ISoter(const char* /* name */) {
    //return new Soter();
//}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace soter
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
