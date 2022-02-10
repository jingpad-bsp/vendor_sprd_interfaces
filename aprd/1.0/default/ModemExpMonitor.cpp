
#include "ModemExpMonitor.h"
#include "AprdInfoSync.h"
#include "AprdInfoSync.h"

using vendor::sprd::hardware::aprd::V1_0::implementation::AprdInfoSync;
#define MODEM_SOCKET_NAME         "modemd"
#define WCND_SOCKET_NAME          "wcnd"

ModemThread::ModemThread(char *p)
{
    cmd = p;
    Setup();
}

ModemThread::~ModemThread()
{}

void ModemThread::Setup()
{
    ALOGD("ModemExpThread::Setup()\n");
}

void ModemThread::Execute(void *pAprdInfoSync)
{
    ALOGD("ModemExpThread::Execute()\n");
    int modemfd = -1;
    int wcnfd = -1;
    char buf[256];
    int numRead;
	int retry_num = 0;
	Return<void> status; 

reconnect:
    if (modemfd < 0)
        modemfd = ConnectService(MODEM_SOCKET_NAME);
    if (wcnfd < 0)
        wcnfd = ConnectService(WCND_SOCKET_NAME);

    if (modemfd < 0 && wcnfd < 0) {
        ALOGE("%s: cannot create local socket server", __FUNCTION__);
        exit(-1);
    }

    for(;;) {
        fd_set read_fds;
        int rc = 0;
        int max = -1;
	 sp<IAprdCallback> fun=NULL;
	 AprdInfoSync* pInfo=(AprdInfoSync *)pAprdInfoSync;
        FD_ZERO(&read_fds);

        if (modemfd >= 0) {
            FD_SET(modemfd, &read_fds);
            if (modemfd > max)
                max = modemfd;
        }
        if (wcnfd >= 0) {
            FD_SET(wcnfd, &read_fds);
            if (wcnfd > max)
                max = wcnfd;
        }

        if ((rc = select(max + 1, &read_fds, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR)
                continue;
            ALOGE("select failed (%s) modemfd=%d, wcnfd=%d, max=%d\n",
                strerror(errno), modemfd, wcnfd, max);
            sleep(1);
            continue;
        } else if (!rc)
            continue;

        if (modemfd >= 0 && FD_ISSET(modemfd, &read_fds)) {
            memset(buf, 0, sizeof(buf));
            do {
                numRead = read(modemfd, buf, sizeof(buf));
            } while(numRead < 0 && errno == EINTR);

            if (numRead <= 0) {
                close(modemfd);
                modemfd = -1;
                goto reconnect;
            }
            buf[255] = '\0';

            ALOGD("receive \"%s\"\n", buf);
            m_ei.private_data = buf;
            if ((strstr(buf, "Modem Blocked") != NULL)&&(strstr(buf, "Modem Reset") == NULL)&&(strstr(buf, "Modem Alive") == NULL )){
               m_ei.et = EExceptionType_V::E_MODEM_BLOCKED;
		
		retry_num = 0;
		CALLBACK_RETRY:
        while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("ModemExpThread CALLBACK_RETRY getcallback null \n");
            sleep(1);
        }		
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("Modem Blocked callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY;
              }
        }else{
            ALOGE("Modem Blocked callback OK \n");
        }   
            }
			else if ((strstr(buf, "Assert") != NULL)&&(strstr(buf, "Modem Reset") == NULL)&&(strstr(buf, "Modem Alive") == NULL )) {
                m_ei.et = EExceptionType_V::E_MODEM_ASSERT;
		
		retry_num = 0;
		CALLBACK_RETRY2:
        while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("ModemExpThread CALLBACK_RETRY2 getcallback null \n");
            sleep(1);
        }		
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("Modem Assert callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY2;
              }
        }else{
            ALOGE("Modem Assert callback OK \n");
        }   
            }
            
        }
        if (wcnfd >= 0 && FD_ISSET(wcnfd, &read_fds)) {
            memset(buf, 0, sizeof(buf));
            do {
                numRead = read(wcnfd, buf, sizeof(buf));
            } while(numRead < 0 && errno == EINTR);

            if (numRead <= 0) {
                close(wcnfd);
                wcnfd = -1;
                goto reconnect;
            }
            buf[255] = '\0';

            ALOGD("receive \"%s\"\n", buf);
            m_ei.private_data = buf;
            if (strstr(buf, "WCN-CP2-EXCEPTION") != NULL) {
                m_ei.et = EExceptionType_V::E_WCN_ASSERT;
            }else if (strstr(buf, "WCN-GE2-EXCEPTION") != NULL) {
                m_ei.et = EExceptionType_V::E_WCN_GE2;
            }
        
		retry_num = 0;		
		CALLBACK_RETRY3:
        while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("ModemExpThread CALLBACK_RETRY3 getcallback null \n");
            sleep(1);
        }			
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("WCN-GE2-EXCEPTION callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY3;
              }
        }else{
            ALOGE("WCN-GE2-EXCEPTION callback OK \n");
        } 
        }
    }

    close(modemfd);
    close(wcnfd);
}

int ModemThread::ConnectService(const char *socket_name)
{
    int cfd, iter;
    for (iter = 0; iter < 30; iter++) {
        cfd = socket_local_client(socket_name,
        ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
        ALOGD("socket_name=%s, cfd=%d\n", socket_name, cfd);
        if (cfd >= 0)
            break;
        sleep(1);
    }

    return cfd;
}
