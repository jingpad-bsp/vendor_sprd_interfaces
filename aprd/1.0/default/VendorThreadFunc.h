#ifndef VENDORTHREADFUNC_H
#define VENDORTHREADFUNC_H

#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <cutils/sockets.h>
#include <log/log.h>
#include <cutils/properties.h>
#include <dirent.h>
#include <errno.h>
#include <cutils/sockets.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vendor/sprd/hardware/aprd/1.0/types.h>
#include <vendor/sprd/hardware/aprd/1.0/IAprdCallback.h>

using ::android::sp;
using ::android::hardware::hidl_string;
using vendor::sprd::hardware::aprd::V1_0::IAprdCallback;
using vendor::sprd::hardware::aprd::V1_0::e_info_V;
using vendor::sprd::hardware::aprd::V1_0::EExceptionType_V;

class VendorAprPub
{
public:
    //struct e_info_V(*m_ei)[];
    VendorAprPub();
    virtual ~VendorAprPub();

    virtual void Setup();
    virtual void Execute(void *pAprdInfoSync);
};

typedef struct VendorAprdThreadArg{
    void *pAprdInfoSync;
    VendorAprPub* pubSym;
}VTArg;

#endif
