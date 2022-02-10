#ifndef CPXINFO_H
#define CPXINFO_H
#include <string>
#include <regex>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <atci.h>

#include "VendorThreadFunc.h"
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

using ::android::hardware::Return;
using ::android::hardware::Void;


/*******************************************************/
/*#####################################################*/
class ModemInfoThread : public VendorAprPub
{
public:
    ModemInfoThread(char *p);
    ~ModemInfoThread();
    
    int ReqRspAT(int phoneId, const char *atCmd, char *resp, size_t respLen);
    int GetphoneNUM();
    int GetVer(char* p, int len);
    int GetIMEIs(int phoneNUM, char* p, int len);
    int isModemAlive();
    int isOK_clr(std::string &p);

    virtual void Setup();
    virtual void Execute(void *pAprdInfoSync);
private:
    int phoneNum;
    struct e_info_V m_ei;
    hidl_string cmd;
};

/*******************************************************/
/*#####################################################*/
class WCNInfoThread : public VendorAprPub
{
public:
    WCNInfoThread(char *p);
    ~WCNInfoThread();
    int WCNcmdReqRsp(const char *socket_name, const char* cmd, char *resp, size_t respLen);
    int SetPowerOn();
    int GetVer(char* p, int len);
    int isOK(std::string &p);

    virtual void Setup();
    virtual void Execute(void *pAprdInfoSync);
private:
    struct e_info_V m_ei;
    hidl_string cmd;
};

/*******************************************************/
/*#####################################################*/
class GeInfoThread : public VendorAprPub
{
public:
    GeInfoThread(char *p);
    ~GeInfoThread();
    int GetVer(char* p, int len);

    virtual void Setup();
    virtual void Execute(void *pAprdInfoSync);
private:
    struct e_info_V m_ei;
    hidl_string cmd;
};

#endif
