#ifndef MODEMEXPMONITOR_H
#define MODEMEXPMONITOR_H
#include "VendorThreadFunc.h"

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

using ::android::hardware::Return;
using ::android::hardware::Void;

class ModemThread : public VendorAprPub
{
public:
    ModemThread(char *p);
    ~ModemThread();
    virtual void Setup();
    virtual void Execute(void *pAprdInfoSync);
protected:
    int ConnectService(const char *socket_name);
private:
    struct e_info_V m_ei;
    hidl_string cmd;
};

#endif

