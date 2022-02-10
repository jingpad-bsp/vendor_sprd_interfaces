#include "Connmgr.h"
#include <log/log.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <poll.h>
#include <inttypes.h>
#include <ctype.h>
#include <cutils/sockets.h>
#include <string.h>


#define LOG_TAG "Connmgr_hidl"
#define WCND_SOCKET_NAME "wcnd"
#define WCND_ENG_SOCKET_NAME "wcnd_eng"

namespace vendor {
namespace sprd {
namespace hardware {
namespace connmgr {
namespace V1_0 {
namespace implementation {

// Methods from IConnmgr follow.
Return<bool> Connmgr::registerCallback(const sp<IConnmgrCallback>& callback) {
	 mCallback = callback;
	 ALOGD("registerCallback");
	 return true;
}


Return<void> Connmgr::SendStringCommand(const hidl_string& type, SendStringCommand_cb _hidl_cb) {
    ALOGD("doHostapdstringCommand %s" , type.c_str());
    return validateAndCall(this,&Connmgr::SendStringCommandInternal,_hidl_cb,type);
}

std::string Connmgr::SendStringCommandInternal(
    const hidl_string& type)
{
    char reply[4097];
    int reply_size= 0;
    char cmd[1024];
    int engmode = 0;
    char *socket_name;
    int client_fd = -1;
    int i = 0;

    memset(cmd,0,1024);
    strncpy(cmd,type.c_str(),type.size());
    if(strstr(cmd, "eng")){
        socket_name = WCND_ENG_SOCKET_NAME;
	} else if(strstr(cmd, "wcn")) {
		socket_name = WCND_SOCKET_NAME;

	} else {
        return "socket  error";
	}
	
    client_fd = socket_local_client(
      socket_name, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
    ALOGD("%s: Unable bind server %s, waiting...\n", __func__, socket_name);

     // remove the last ''
    cmd[strlen(cmd) - 1] = '\0';

    ALOGD("cmd: %s\n", cmd);

    TEMP_FAILURE_RETRY(write(client_fd, cmd, strlen(cmd)));

    for (i = 0 ; i < 5; i++) {
        memset(reply, 0, 128);
        ALOGD("%s: waiting for server %s\n", __func__, socket_name);
        reply_size = read(client_fd, reply, 128);
        ALOGD("%s: get %d bytes %s\n", __func__, reply_size, reply);
        if (reply_size >= 0) break;
   }

    close(client_fd);
    ALOGD("SendStringCommandInternal reply %s" , reply);
    return reply;
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IConnmgr* HIDL_FETCH_IConnmgr(const char* /* name */) {
    return new Connmgr();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace connmgr
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
