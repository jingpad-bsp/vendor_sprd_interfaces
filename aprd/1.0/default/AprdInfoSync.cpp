#include "AprdInfoSync.h"
#include "CPxInfo.h"
#include "ModemExpMonitor.h"
#include "VendorThreadFunc.h"
#include "VendorPropMonitor.h"
#define LOG_TAG "vendor.sprd.hardware.IAprd@1.0-service"

namespace vendor {
namespace sprd {
namespace hardware {
namespace aprd {
namespace V1_0 {
namespace implementation {

ModemInfoThread objmodem("VendorSyncModemInfo");
WCNInfoThread objwcn("VendorSyncWCNInfo");
GeInfoThread objgps("VendorSyncGPSInfo");
ModemThread objmodem_exp("VendorSyncModemExp");
PropMonitor objprop("VendorSyncProp");

AprdInfoSync *service = new AprdInfoSync();

// Methods from ::vendor::sprd::hardware::aprd::V1_0::IAprdInfoSync follow.
AprdInfoSync::AprdInfoSync()
{
    mCallback=NULL;
	
	//init sem for 5 service

	sem_init(&modem_sem, 0, 0);
	sem_init(&wcn_sem, 0, 0);
	sem_init(&gps_sem, 0, 0);
	sem_init(&modem_exp_sem, 0, 0);
	sem_init(&prop_sem, 0, 0);
	
}

AprdInfoSync::~AprdInfoSync()
{
    //TODO
}

Return<void> AprdInfoSync::setCallback(const sp<::vendor::sprd::hardware::aprd::V1_0::IAprdCallback>& callback) {
    // TODO implement
    mCallback=callback;
	
	sem_post(&modem_sem);
	sem_post(&wcn_sem);
	sem_post(&gps_sem);
	sem_post(&modem_exp_sem);
	sem_post(&prop_sem);
	
	//ALOGD("IAprdInfoSync setCallback already, modem_sem is %ld,wcn_sem is %ld,gps_sem is %ld,modem_exp is %ld,prop is %ld.",modem_sem,wcn_sem,gps_sem,modem_exp,prop); 
	
    return Void();
}

Return<void> AprdInfoSync::sendCmd(const hidl_string& cmd, sendCmd_cb _hidl_cb) {
    // TODO implement
    return Void();
}

sp<IAprdCallback> AprdInfoSync::getcallback()
{
    #if 0
	{
	while(mCallback==NULL)
    {
        ALOGD("IAprdInfoSync waite for callback be set."); 
        sleep(1); 
    }
	}
	#endif
	//ALOGD("IAprdInfoSync will getcallback, modem_sem is %ld,wcn_sem is %ld,gps_sem is %ld,modem_exp is %ld,prop is %ld.",modem_sem,wcn_sem,gps_sem,modem_exp,prop); 

	if(mCallback==NULL)
    {
        ALOGD("IAprdInfoSync getcallback mCallback==NULL.");  
    }
	
    return mCallback;
}

/*
typedef VendorAprdThreadArg{
    void *pAprdInfoSync;
    VendorAprPub* pubSym;
    char* cmd;
}VTArg;*/

void* AprdInfoSync::LVendorFunc(void *arg)
{
    VTArg* pVTArg=(VTArg*)arg;
    sp<IAprdCallback> fun=NULL;
    AprdInfoSync* pInfo=(AprdInfoSync *)pVTArg->pAprdInfoSync;
    VendorAprPub* pFunc=pVTArg->pubSym;


    while((fun=pInfo->getcallback())==NULL)
    {
	
    	if (pVTArg->pubSym==&objmodem)
	    {
		    sem_wait(&service->modem_sem);
    	}
	    else if (pVTArg->pubSym==&objwcn)
	    {
		    sem_wait(&service->wcn_sem);
	    }
	    else if (pVTArg->pubSym==&objgps)
	    {
		    sem_wait(&service->gps_sem);
	    }
	    else if (pVTArg->pubSym==&objmodem_exp)
	    {
		    sem_wait(&service->modem_exp_sem);
	    }
	    else if (pVTArg->pubSym==&objprop)
	    {
		    sem_wait(&service->prop_sem);
	    }
	    else 
	    {
	        ALOGD("IAprdInfoSync getcallback is null and sem_wait failed ."); 
	    }
	}

    if(NULL != pVTArg){
        pFunc->Execute(pVTArg->pAprdInfoSync);
    }

    return NULL;
}


#define nCHILDTHREADS 5
static VTArg vtarg[nCHILDTHREADS];



// Methods from ::android::hidl::base::V1_0::IBase follow.
IAprdInfoSync* HIDL_FETCH_IAprdInfoSync(const char* /* name */) {
    
    int err, i;
    pthread_t tid;

    vtarg[0].pAprdInfoSync=service;
    vtarg[0].pubSym=&objmodem;

    vtarg[1].pAprdInfoSync=service;
    vtarg[1].pubSym=&objwcn;
    
    vtarg[2].pAprdInfoSync=service;
    vtarg[2].pubSym=&objgps;
    
    vtarg[3].pAprdInfoSync=service;
    vtarg[3].pubSym=&objmodem_exp;
    
    vtarg[4].pAprdInfoSync=service;
    vtarg[4].pubSym=&objprop;

    for(i=0;i<nCHILDTHREADS;i++)
    {
        err = pthread_create(&tid, NULL, AprdInfoSync::LVendorFunc, (void*)&vtarg[i]);
        ALOGD("vendor aprd thread creat errno:%d-tid=%d", err, tid);
    }
    return service;
    //return new AprdInfoSync();
}

//
}  // namespace implementation
}  // namespace V1_0
}  // namespace aprd
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
