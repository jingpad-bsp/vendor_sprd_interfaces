/*
 * Copyright (C) 2012-2013 The Android Open Source Project
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

#include <ctype.h>
#include <inttypes.h>
#include <poll.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cutils/sockets.h>
#include "CmdListener.h"
#include <vendor/sprd/hardware/log/1.0/ILogCallback.h>
#include <string.h>
#include <fcntl.h>
#include <cutils/sockets.h>
#undef LOG_TAG
#define LOG_TAG "CmdListener"
CmdListener::CmdListener(LogControl * service)
    : SocketListener(getLogSocket(), true), mService(service) {
}

// When we are notified a new log entry is available, inform
// all of our listening sockets.
void CmdListener::newCmd(char *cmd, int len, char *response, int response_size) {
    if(len == 0){
        ALOGE("cmd lenth is 0");
        return;
    }
    ALOGD("new cmd comes:%s", cmd);
    sp<ILogCallback> callback = mService->getCallback();
    if(callback != NULL){
        ALOGD("send new cmd to callback:%s", cmd);
        string pStr;
        auto cb = [&](hidl_string atResp) {
            pStr = atResp.c_str();
        };
        Return<void> status = callback->onCommand(cmd, cb);
        if (!status.isOk()) {
            ALOGE("log_service died");
            snprintf(response, response_size - 1, "can not get response frome log service");
        }else{
            snprintf(response, response_size - 1, "%s", pStr.c_str());
        }
    }else{
        snprintf(response, response_size - 1, "no callback,cmd can not send");
        ALOGE("callback is null,no result return");
    }
}

bool CmdListener::onDataAvailable(SocketClient* cli) {
    static bool name_set;
    if (!name_set) {
        prctl(PR_SET_NAME, "cmdlistener");
        name_set = true;
    }

    char buffer[4096] = {0};
    char retrun_buffer[4096] = {0};
    int len = read(cli->getSocket(), buffer, sizeof(buffer) - 1);
    if (len <= 0) {
        ALOGE("read failed");
        return false;
    }
    buffer[len] = '\0';
    newCmd(buffer, sizeof(buffer), retrun_buffer, sizeof(retrun_buffer));
    ALOGD("read from cli->getSocket() : %d, get result is :%s",cli->getSocket(), retrun_buffer);
    write(cli->getSocket(), retrun_buffer, strlen(retrun_buffer));
    return true;
}


int CmdListener::getLogSocket() {
    static const char socketName[] = "ylog_cli_cmd";
    int  sock= socket_local_server(socketName, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
    if (sock < 0) {
        ALOGE("open %s failed: %s\n", socketName, strerror(errno));
        return -1;
    }
    ALOGD("open %s success,socket is : %d\n", socketName, sock);
    return sock;
}
