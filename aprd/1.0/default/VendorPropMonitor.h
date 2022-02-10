#ifndef VENDORPROPMONITOR_H
#define VENDORPROPMONITOR_H
#include "VendorThreadFunc.h"

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

using ::android::hardware::Return;
using ::android::hardware::Void;

class PropMonitor : public VendorAprPub
{
public:
    PropMonitor(char *p);
    ~PropMonitor();
    virtual void Setup();
    virtual void Execute(void *pAprdInfoSync);
private:
    struct e_info_V m_ei;
    hidl_string cmd;
};

#endif

