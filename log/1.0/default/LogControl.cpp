#include "LogControl.h"
#include "CmdListener.h"
#include "LogCallback.h"
#include <log/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <cutils/sockets.h>
#include <cutils/properties.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <poll.h>
#define LOG_TAG "LogControl"
#define ARRAY_LEN(A) (sizeof(A)/sizeof((A)[0]))
namespace vendor {
namespace sprd {
namespace hardware {
namespace log {
namespace V1_0 {
namespace implementation {

using android::sp;

// Methods from ILogControl follow.
Return<void> LogControl::setCallback(const sp<ILogCallback>& callback) {
    // TODO implement
    mCallback = callback;
    ALOGD("set call back");
    return Void();
}
sp<ILogCallback> LogControl::getCallback() {
    // TODO implement
    return mCallback;
}

Return<void> LogControl::sendCmd(const hidl_string& desSocket, const hidl_string& cmd, sendCmd_cb _hidl_cb) {
    // TODO implement
    // TODO implement
    int res = 0;
    const char *socket = desSocket.c_str();
    const char *p = cmd.c_str();
    hidl_string ret;
    char response[4096] = {0};
    int size = sizeof(response);
    ALOGD("send cmd: %s to socket: %s", p, socket);
    res = processCmd(socket, p, response, size);
    ALOGD("cmd is %s,and response is %s", p, response);
    ret.setToExternal(response, strlen(response)+1);
    _hidl_cb(ret);
    return Void();
}

int LogControl::processCmd(const char *socket, const char *cmd, char *response, int size){
    int ret = -1;
    char buf[4096] = {0};
    int buf_size = sizeof(buf);
    struct pollfd pfd[2];
    int fd = socket_local_client(socket, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
    if (fd < 0) {
        snprintf(response, size, "open %s failed",socket);
        ALOGD("%s open %s failed: %s\n", __func__, socket, strerror(errno));
        return -1;
    }
	
    ret = write(fd, cmd, strlen(cmd));
    if(ret < 0){
        ALOGW("cli write failed: %s\n", strerror(errno));
        close(fd);
        return ret;
    }
    pfd[0].fd = fd;
    pfd[0].events = POLLIN;
    for (;;) {
        if ((ret = poll(pfd, ARRAY_LEN(pfd), -1)) <= 0) {
            ALOGE("poll error %s", strerror(errno));
            close(fd);
            return ret;
        }
        if (pfd[0].revents) {
			memset(buf,0,buf_size);
            ret = read(pfd[0].fd, buf, buf_size);
            //ALOGD("read buffer:%s",buf);
            if (ret > 0) {
                snprintf(response, size, "%s", buf);
                break;
            } else if (ret == 0) {
                ALOGW("server closed\n");
                snprintf(response, size, "server closed");
                break;
            } else {
                ALOGW("cli read failed: %s\n", strerror(errno));
                snprintf(response, size, "cli read failed");
                break;
            }
        }else{
            //ALOGE("pfd[0].revents :%d",pfd[0].revents);
            continue;
        }
    }
    close(fd);
    return 0;
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

ILogControl* HIDL_FETCH_ILogControl(const char* /* name */) {
    LogControl *service = new LogControl();
    // TODO: make below statements active
    CmdListener *cmdListener = new CmdListener(service);
    cmdListener->startListener();
    return service;
    //return new LogControl();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace log
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
