#include <VendorPropMonitor.h>
#include "AprdInfoSync.h"
#include "AprdInfoSync.h"

using vendor::sprd::hardware::aprd::V1_0::implementation::AprdInfoSync;
PropMonitor::PropMonitor(char *p)
{
    cmd = p;
    Setup();
}
PropMonitor::~PropMonitor(){}
void PropMonitor::Setup()
{
    ALOGD("PropMonitorThread::Setup()\n");
}
void PropMonitor::Execute(void *pAprdInfoSync)
{
    char value_old[PROPERTY_VALUE_MAX];
    char value_new[PROPERTY_VALUE_MAX];
    char *default_value = (char*)"unknown";
    sp<IAprdCallback> fun=NULL;
    AprdInfoSync* pInfo=(AprdInfoSync *)pAprdInfoSync;	
    memset(value_old, 0, sizeof(value_old));
    memset(value_new, 0, sizeof(value_new));
	
	int retry_num = 0;
	Return<void> status; 
	
    while(1)
    {
        property_get("persist.vendor.radio.modem.config", value_old, default_value);
        if(strcmp(value_old, default_value)!=0)
        {
            m_ei.et = EExceptionType_V::I_MODEMMODE_FLASH;
            m_ei.private_data = value_old;

		retry_num = 0;
        CALLBACK_RETRY1:
         while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("PropMonitor1 getcallback null \n");
            sleep(1);
        }			
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("persist.vendor.radio.modem.config callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY1;
              }
        }else{
            ALOGE("persist.vendor.radio.modem.config callback OK \n");
        } 
            break;
        }
        sleep(1);
    }
    //In multi SIM card phone, hot plug sim will change value "persist.vendor.radio.modem.config", but phone doesn't reboot and triger vendor aprd reread prop, so read it every 20s.
    while(1)
    {
        sleep(20);		
        property_get("persist.vendor.radio.modem.config", value_new, default_value);
        if(strcmp(value_old, value_new)!=0)
        {
            m_ei.et = EExceptionType_V::I_MODEMMODE_FLASH;
            m_ei.private_data = value_new;

		retry_num = 0;
        CALLBACK_RETRY2:
         while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("PropMonitor2 getcallback null \n");
            sleep(1);
        }			
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("persist.vendor.radio.modem.config2 callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY2;
              }
        }else{
            ALOGE("persist.vendor.radio.modem.config2 callback OK \n");
        } 
            strcpy(value_old, value_new);
        }
    }
}
