#include "CPxInfo.h"
#include "AprdInfoSync.h"
#include "AprdInfoSync.h"

using vendor::sprd::hardware::aprd::V1_0::implementation::AprdInfoSync;
using namespace std;

//VendorAprPub is a virtual base, impl NULL funcs.
VendorAprPub::VendorAprPub(){}
VendorAprPub::~VendorAprPub(){}
void VendorAprPub::Setup(){}
void VendorAprPub::Execute(void *pAprdInfoSync){}

//Modem
ModemInfoThread::ModemInfoThread(char *p)
{
    cmd = p;
    Setup();
}

ModemInfoThread::~ModemInfoThread(){}

int ModemInfoThread::GetphoneNUM()
{
    return phoneNum;
}

int ModemInfoThread::ReqRspAT(int phoneId, const char *atCmd, char *resp, size_t respLen)
{
    return sendATCmd(phoneId, atCmd, resp, respLen);
}

int ModemInfoThread::isOK_clr(string &tmp)
{
    int isOK=0, index=0;
    while((index=tmp.find("OK",index)) != string::npos)
    {
        isOK++;
        tmp.erase(index,2);
    }
    return isOK;
}
int ModemInfoThread::isModemAlive()
{
    char rsp[1024]={0};
    int ret=0;
    string tmp;
    
    ret=ReqRspAT(0, "AT", rsp, 1024);
    //ALOGD("ATcmd>AT, ret=%d, rsp: %s rsplen %d", ret, rsp, strlen(rsp));
    if(ret==-1 || strlen(rsp)==0)
    {
        ALOGD("AT rsp fail||empty\n");
        return -1;
    }
    if(isOK_clr(tmp.assign(rsp))==0)
    {
        ALOGD("AT rsp error\n");
        return -1;
    }
    return 0;
}
int ModemInfoThread::GetIMEIs(int phoneId, char* p, int len)
{
	ALOGD("ModemInfoThread::GetIMEIs start\n");
    char rsp[1024]={0};
    int ret=0;
    string tmp;
    regex tReg("\\s+");
    
    ret=ReqRspAT(phoneId, "AT+SPIMEI?", rsp, 1024);
    ALOGD("ATcmd>AT+SPIMEI?, ret=%d, rsp: %s rsplen %d\n", ret, rsp, strlen(rsp));
    if(ret==-1 || strlen(rsp)==0)
    {
        ALOGD("AT rsp fail||empty\n");
        return -1;
    }

    if(isOK_clr(tmp.assign(rsp))==0)
    {
        ALOGD("AT rsp error\n");
        return -1;
    }
    tmp=regex_replace(tmp,tReg, "");
    if(tmp.length()<len)
    {
        strcpy(p,tmp.c_str());
        return 0;
    }
    else
    {
        ALOGD("Dest strlen is shorter\n");
        return -1;
    }
}

int ModemInfoThread::GetVer(char* dest, int dstlen)
{
	ALOGD("ModemInfoThread::GetVer start\n");
    char rsp[1024]={0}, buf[512]={0};
    int ret=0, i=0, index=0, cplen=0;
    string tmp,tmp1,tmp2;
    
    regex tReg0("\\S+\\s+"), tReg1("\\s+");
    
    ret=ReqRspAT(1, "AT+CGMR", rsp, 1024);
    ALOGD("ATcmd>AT+CGMR, ret=%d, rsp:%s\n", ret, rsp);

    if(ret==-1 || strlen(rsp)==0)
    {
        ALOGD("AT rsp fail||empty\n");
        return -1;
    }

    if(isOK_clr(tmp.assign(rsp))==0)
    {
        ALOGD("AT rsp error\n");
        return -1;
    }
    while((index = tmp.find(' ',index)) != string::npos) //delete space(avoid match in //s+)
    {
        tmp.erase(index,1);
    }

    regex_iterator<string::iterator> rit(tmp.begin(),tmp.end(),tReg0); //Every line is fomarted by tReg0("\\S+\\s+")
    regex_iterator<string::iterator> rend;
    
    i=0;
    index=0;
    tmp2="";
    memset(rsp, 0, sizeof(rsp));
    while (rit!=rend)  //loop lines
    {
        tmp1=regex_replace(rit->str(),tReg1, "");  //Cut line tail by replace tReg1("\\s+")

        if(i<2)
            goto NEXTLINE;
        else if(i<4)  //Copy logic of lib-rild source-code(where setprop gsm.version.baseband) that just fetch AT rsp line 2,3,4. And save str after ':', seprate whith '|'
        {
            if((index = tmp1.find(':',0)) != string::npos)
            {
                cplen=tmp1.copy(rsp, tmp1.length()-index, index+1);
            }
            else
            {
                cplen=tmp1.copy(rsp, tmp1.length(), 0);
            }
            rsp[cplen]='\0';
            
            tmp2=tmp2+rsp+"|";
        }
        else
        {
            if((index = tmp1.find(':',0)) != string::npos)
            {
                tmp2 += tmp1.insert(index-2, 1, ' ');
            }
            else
            {
                tmp2+=tmp1;
            }
        }
NEXTLINE:
        i++;
        rit++;
    }
    ALOGD("modem version=%s\n", tmp2.c_str());
    if(dstlen<=tmp2.length())
    {
        ALOGD("modem ver dest not long enough\n");
        return -1;
    }
    strcpy(dest, tmp2.c_str());
    return 0;
}

void ModemInfoThread::Setup()
{
    //To Do
    int i=0;
    ALOGD("ModemInfoThread[CP0 info]::Setup()\n");
    int simCount = 1;
    char prop[PROPERTY_VALUE_MAX] = {0};
    property_get("persist.vendor.radio.phone_count", prop, "unknown");
    for(i=0;i<60;i++)
    {
        if(strcmp(prop, "unknown")==0)
        {
            ALOGD("Prop phone_count unprepared\n");
            sleep(1);
            property_get("persist.vendor.radio.phone_count", prop, "unknown");  //system prop but in white list for vendor
        }else{
            break;
        }
    }
    if(i>=60){
        simCount=2;
    }else{
        simCount=atoi(prop);
        if(simCount<=4)
            phoneNum = simCount;
    }
    return;
}
void ModemInfoThread::Execute(void *pAprdInfoSync)
{
    //To Do
	ALOGD("ModemInfoThread::Execute\n");
    int phonum=0, ret=0, phoch=0;
    char modeminfo[256];
    string tmp;
    int retry_num = 0;
    Return<void> status;
    sp<IAprdCallback> fun=NULL;
    AprdInfoSync* pInfo=(AprdInfoSync *)pAprdInfoSync;	

    bzero(modeminfo, sizeof(modeminfo));
    sleep(300); //While phone booting, CPx stbiality is stupid. avoid to crah.
    phonum = GetphoneNUM();
    ALOGD("AT phonum=%d\n", phonum);

REWAITING:
    while((ret=isModemAlive())!=0)
    {
        ALOGD("Waiting for modem prepared\n");
        sleep(5);
    }
    while(1)
    {

        if((ret=isModemAlive())!=0)
        {
            ALOGD("Modem may be crashed\n");
            goto REWAITING;
        }
        ret=GetVer(modeminfo, 256);
        if(ret==-1)
        {
            sleep(5);
            ALOGD("AT Get modemVer failed\n");
            continue;//while(1)
        }
        m_ei.et = EExceptionType_V::I_MODEMVER_GET;
        m_ei.private_data = modeminfo;
        CALLBACK_RETRY:
        while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("ModemInfoThread CALLBACK_RETRY getcallback null \n");
            sleep(1);
        }			
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("ModemInfoThread callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY;
              }
        }else{
            ALOGE("ModemInfoThread callback OK \n");
        }     



PROCESSIMEI:  //Get IMEI

        if((ret=isModemAlive())!=0)
        {
            ALOGD("Modem may be crashed\n");
            goto REWAITING;
        }
        tmp.clear();
        for(phoch=0; phoch<phonum; phoch++)
        {
            ret=GetIMEIs(phoch, modeminfo, 256);
            if(ret==-1)
            {
                sleep(5);
                ALOGD("AT Get IMEIs %d failed\n", phoch);
                goto PROCESSIMEI;
            }
            tmp+=modeminfo;
            if(phoch<phonum-1)
                tmp+=",";
        }

        m_ei.et = EExceptionType_V::I_IMEI_GET;
        m_ei.private_data = tmp.c_str();
		retry_num = 0;
        CALLBACK_RETRY2:
        while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("ModemInfoThread CALLBACK_RETRY getcallback null \n");
            sleep(1);
        }			
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("I_IMEI_GET callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY2;
              }
        }else{
            ALOGE("I_IMEI_GET callback OK \n");
        }     
	
		
        break;  //Got ModemVersion and IMEIS
    }

    return;
}
//Modem

//Wcn
WCNInfoThread::WCNInfoThread(char *p)
{
    cmd = p;
    Setup();
}

WCNInfoThread::~WCNInfoThread(){}

int WCNInfoThread::WCNcmdReqRsp(const char *socket_name, const char* cmd, char *resp, size_t respLen)
{
    int client_fd = -1, n = 0;
    char buffer[256];

    client_fd = socket_local_client(
        socket_name, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);

    while (client_fd < 0) {
        ALOGD("WCNInfoThread::%s: Unable bind server %s, waiting...\n", __func__, socket_name);
        usleep(100 * 1000);
        client_fd = socket_local_client(
            socket_name, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
    }

    TEMP_FAILURE_RETRY(write(client_fd, cmd, strlen(cmd)));    
    for (;;) 
    {
      memset(buffer, 0, 128);
      ALOGD("%s: waiting for server %s\n", __func__, socket_name);
      n = read(client_fd, buffer, 256);
      ALOGD("%s: get %d bytes %s\n", __func__, n, buffer);
      if (n >= 0)
        break;
      else
        sleep(1);
    }
    close(client_fd);
    if(respLen<strlen(buffer))
    {
        ALOGD("WCNInfo resp str not long enough\n");
        return -1;
    }
    strcpy(resp, buffer);
    return 0;
}

int WCNInfoThread::isOK(string &tmp)
{
    int isOK=0, index=0;
    while((index=tmp.find("OK",index+1)) != string::npos)
    {
        isOK++;
    }
    return isOK;
}

int WCNInfoThread::SetPowerOn()
{
    ALOGD("WCNInfoThread::SetPowerOn start\n");
	char rsp[1024]={0};
    int ret=0;
    string tmp;
    
    ret=WCNcmdReqRsp("wcnd", "wcn poweron", rsp, 1024);
    if(isOK(tmp.assign(rsp))>0)
        return 0;
    else
        return -1;
}

int WCNInfoThread::GetVer(char* p, int len)
{
	ALOGD("WCNInfoThread::GetVer\n");
    char rsp[1024]={0}, buf[768]={0};
    char (*pbuf)[256]= (char(*)[256])buf;
    int ret=0, i=0, j[4]={-1}, index=0, cplen=0;
    string tmp,tmp1,tmp2;
    
    regex tReg0("PlatformVersion:(\\S+)ProjectVersion:(\\S+)HWVersion:(\\S+)"), tReg1("\\s+"),
            tReg2("..-..-......:..:.."), tRegMarlin3("PlatformVersion:(\\S+)ProjectVersion:(\\S+)");
    smatch s_result, s_result1;

    ret=WCNcmdReqRsp("wcnd", "wcn at+spatgetcp2info", rsp, 1024);
    ALOGD("wcn AT+spatgetcp2info, ret=%d, rsp:%s\n", ret, rsp);
    //test
    //marlin 15a// ????? Some cp2 chip version start with "marlin", seem like marlin_15a.
    //                  But we doesn't a phone whith cp2 chip like this to test logic branch "if((index=tmp.find("marlin",0)) == 0)". So this branch is not been tested and not trusted.

    //marlin_15c// strcpy(rsp, "Platform Version: MARLIN_15C_W17.10.2_Debug 03-07-2017 14:42:55 Project Version: MARLIN HW Version:AA");

    //marlin2   // strcpy(rsp, "Platform Version:Marlin2_17A_W18.23.1Project Version:   sharkle_cm4HW Version:....06-04-2018 18:34:13");

    //2351      // strcpy(rsp, "Platform Version: MOCORTM_14B_TShark_MPII_W16.47_Debug\r\nProject Version:   CP2_WCN_14D_W17.36.3_Release\r\nHW Version:        sc8830g_modem\r\n09-06-2017 17:08:35\r\n");

    //marlin3   // strcpy(rsp, "WCN_VER:Platform Version:MARLIN3_W18.05.5~Project Version:sc2355_marlin3~02-02-2018 12:03:56~");
    
    //ALOGD("wcn at+spatgetcp2info, ret=%d, rsp:\n%s", ret, rsp);

    if(ret==-1 || strlen(rsp)==0)
    {
        ALOGD("WCN rsp fail||empty\n");
        return -1;
    }
    tmp.assign(rsp);
    if((index=tmp.find("marlin",0)) == 0) //marlin 15A ??
    {
        tmp2="Debug";
        if((index=tmp.find("Release",6)) != string::npos) /*6=strlen("marlin")*/
        {
            tmp2="Release";
        }
        
        index=0;
        i=0;
        while((index = tmp.find(':', index)) != string::npos)
        {
            i++;
            if(i==2){
                j[0]=index;
            }
            if(i==3){
                j[1]=index;
            }
            if(i==4){
                j[2]=index;
            }
            if(i==5){
                j[3]=index;
            }           
            index++;
        }
        if(j[3]==-1)
            j[3]=tmp.length();
        for(i=0;i<3;i++)
        {
            cplen=tmp.copy(pbuf[i], j[i+1]-j[i]-1/*len between : : */, j[i]+1);
            pbuf[i][cplen+1]='\0';
            tmp1=pbuf[i];
            if(i>0)
            {
                tmp1.erase(0,tmp1.find_first_not_of(" "));
                tmp1.erase(tmp1.find_last_not_of(" ") + 1); 
                tmp1.erase(0,tmp1.find_first_not_of("\t"));
                tmp1.erase(tmp1.find_last_not_of("\t") + 1);
            }
            cplen=tmp1.copy(pbuf[i], tmp1.length());
        }
        tmp1=pbuf[0];
        if((index=tmp1.find(tmp2,0)) != string::npos)
        {
            index+=tmp2.length();
            tmp1.insert(index, 1, '|');
        }
        tmp1=tmp1+":"+pbuf[1]+pbuf[2];
        if(len<=tmp1.length())
        {
            ALOGD("marlin 15B ver dest not long enough\n");
            return -1;
        }
        tmp1.copy(p, tmp1.length());
        ALOGD("marlin 15B, format version: %s\n", p);
    }
    else  //Some chip version str start with mariln (15A ??), and others with MARLINxxx or WCN_VER.
    {
        tmp=regex_replace(tmp,tReg1, "");
        if((index=tmp.find("WCN_VER",0)) == 0) //MARLIN3
        {
            ret=regex_search(tmp, s_result, tRegMarlin3);
            if(ret==false)
                return -1;

            /*for (i=0; i<s_result.size(); ++i)
            {
                ALOGD("hyli AT s_result [%d] :%s \n", i, s_result[i].str().c_str());
            }*/
            tmp1=s_result[1].str();
            if((index = tmp1.find('~', 0)) != string::npos)
                tmp1.erase(index); 
                //tmp1.replace (index, 1, 1, '\0');

            tmp2=s_result[2].str();
            if((index = tmp2.find('~', 0)) != string::npos)
                tmp2.replace (index, 1, 1, '|');
            
            if((index = tmp2.find('~', index)) != string::npos)
                tmp2.erase(index);
                //tmp2.replace (index, 1, 1, '\0');

            tmp=tmp2;
            ret=regex_search(tmp2, s_result1, tReg2);
            if(ret==true)
            {
                tmp2=s_result1[0].str();
                index=tmp.find(tmp2, 0);
                tmp.insert(index+10, 1, ' ');
            }
            tmp=tmp1+"|"+tmp;
        }
        else //MARLIN(15C??)/MARLIN2/2351
        {
            tmp=regex_replace(tmp,tReg1, "");
            ret=regex_search(tmp, s_result, tReg0);
            if(ret==false)
                return -1;
                
            /*for (i=0; i<s_result.size(); ++i)
            {
                ALOGD("hyli AT s_result [%d] :%s \n", i, s_result[i].str().c_str());
            }*/
            
            tmp1=s_result[3].str();
            index=tmp1.find("modem",0);
            if(tmp1.length()>strlen("..-..-......:..:.."))
            {/*marlin2 and 2351 date time is after HV Version:, contented in result[3].
             **marlin15C, date time isn't after HV Version:(after Platform Version:, see next if()). result[3]==AA or BA.*/
                index+=5; //5: strlen of modem
                tmp1.insert(index+10, 1, ' ');
            }
            else{
                index=0;
            }
            /*"modem" is substring of 2351 version string, but is't of marlin2. In 2351 str, the posion after "modem", point to date substring. 
            **However, in marlin2 str. Func find() shall return index=-1, and whith 4 '.', So index+5=4 point to afer "....", where is date substring too.*/
            tmp1.insert(index, 1, '|');
            
            tmp2=s_result[1].str();
            ret=regex_search(tmp2, s_result1, tReg2);
            /*if ret==true: marlin15C, date time is after Platform Version: result[1]*/
            tmp=tmp2;
            if(ret==true)
            {
                tmp2=s_result1[0].str();
                index=tmp.find(tmp2, 0);
                tmp.insert(index++, 1, '|');
                tmp.insert(index+10, 1, ' ');
            }
            tmp=tmp+"|"+s_result[2].str()+"|"+tmp1;
        }
        if(len<=tmp.length())
        {
            ALOGD("MARLIN15C|MARLIN2|MARLIN3 ver dest not long enough\n");
            return -1;
        }
        tmp.copy(p, tmp.length());
        ALOGD("MARLIN15C|MARLIN2|MARLIN3, format version: %s\n", p);
    }
    return 0;
}

void WCNInfoThread::Setup()
{
    //To Do
    ALOGD("WCNInfoThread[CP2 info]::Setup()\n");
    return;
}
void WCNInfoThread::Execute(void *pAprdInfoSync)
{
    //To Do
	 ALOGD("WCNInfoThread::Execute\n");
    int ret=0;
    char modeminfo[256];
    string tmp;
    int retry_num = 0;
    Return<void> status;
    sp<IAprdCallback> fun=NULL;
    AprdInfoSync* pInfo=(AprdInfoSync *)pAprdInfoSync;	

    bzero(modeminfo, sizeof(modeminfo));
    sleep(300);  //While phone booting, CPx stability is stupid. avoid to crah.
    /*while((ret=SetPowerOn())!=0)
    {
        ALOGD("WCN PowerOn fail\n");
        sleep(5);
    }*/
    while(1)
    {
        ret=GetVer(modeminfo, 256);
        if(ret==-1)
        {
            sleep(5);
            continue;//while(1)
            ALOGD("WCN Get CP2Ver failed\n");
        }

        m_ei.et = EExceptionType_V::I_CP2VER_GET;
        m_ei.private_data = modeminfo;
        CALLBACK_RETRY:
        while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("WCNInfoThread CALLBACK_RETRY getcallback null \n");
            sleep(1);
        }			
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("I_CP2VER_GET callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY;
              }
        }else{
            ALOGE("I_CP2VER_GET callback OK \n");
        }   

        break;
    }
    return;
}
//Wcn

//GPS
GeInfoThread::GeInfoThread(char *p)
{
	ALOGD("GeInfoThread::GeInfoThread start \n");
    cmd = p;
    Setup();
}
GeInfoThread::~GeInfoThread(){}

int GeInfoThread::GetVer(char* p, int len)
{
    xmlDocPtr doc = NULL;
    xmlXPathObjectPtr result;  
    xmlXPathContextPtr context;
    xmlChar* xpath_exp=BAD_CAST "//PROPERTY";
    xmlNodeSetPtr nodeset = NULL;
    xmlChar* attrName;
    xmlChar* geVerName;
	xmlChar* prop;
    
    int i = 0;
    char gpstype[PROPERTY_VALUE_MAX];

    property_get("ro.vendor.wcn.gpschip",gpstype,"ge2");
    //This vendor-prop is Ref. from "vendor/sprd/platform/packages/apps/SGPS/src/com/spreadtrum/sgps/SgpsUtils.java"
    //But the current value in running still is a system-prop(ro.wcn.gpschip). GPS group may modify it after.
    //So set the default value to ge2, most of new platform use this chip. 
    if ((strcmp(gpstype, "ge2") == 0 ) || (NULL !=strstr(gpstype, "marlin3") )){
        doc = xmlParseFile("/data/vendor/gnss/config/config.xml");
        attrName=BAD_CAST "GE2-VERSION";
    }else{
        doc = xmlParseFile("/data/vendor/cg/supl/supl.xml");
        attrName=BAD_CAST "VERSION";
    }
    
    if(NULL == doc)
    {
        ALOGD("GE xmlDoc is NULL, may not set GPS on. GPS type=%s\n", gpstype);
        return -1;
    }
    context = xmlXPathNewContext(doc);
    result = xmlXPathEvalExpression(xpath_exp, context);
    xmlXPathFreeContext(context);

    if(NULL != result) {
        nodeset = result->nodesetval;  
        for(i = 0; i < nodeset->nodeNr; i++)
        {
            geVerName = xmlGetProp(nodeset->nodeTab[i],BAD_CAST "NAME");
            //ALOGD("GE PROPERTY: %s\n",geVerName);

            if(xmlStrcmp(attrName, geVerName)==0)
            {
                prop = xmlGetProp(nodeset->nodeTab[i],BAD_CAST "VALUE");
                
                if( prop != NULL)
                {
     				strcpy(p, (char *)prop);
     				xmlFree(prop);
                }
                ALOGD("GPS Version: %s\n", p);
                xmlFree(geVerName); 
                break;
            }

            xmlFree(geVerName);  
        }
        xmlXPathFreeObject(result);
    }
    xmlFreeDoc(doc);
    return 0;
}

void GeInfoThread::Setup()
{
    ALOGD("GeInfoThread[GPS info]::Setup()\n");
    return;
}
void GeInfoThread::Execute(void *pAprdInfoSync)
{
    //To Do
	ALOGD("GeInfoThread::Execute\n");
    int ret=0;
    char modeminfo[256];
    string tmp;
    bzero(modeminfo, sizeof(modeminfo));
    int retry_num = 0;
    Return<void> status; 
    sp<IAprdCallback> fun=NULL;
    AprdInfoSync* pInfo=(AprdInfoSync *)pAprdInfoSync;	
	
    while(1)
    {
        ret=GetVer(modeminfo, 256);
        if(ret==-1)
        {
            sleep(5);
            continue;//while(1)
            ALOGD("GPS Get GPSver failed %s\n", strerror(errno));
        }
        m_ei.et = EExceptionType_V::I_GPSVER_GET;
        m_ei.private_data = modeminfo;
        CALLBACK_RETRY:
        while((fun=pInfo->getcallback())==NULL)
        {
            ALOGE("GeInfoThread CALLBACK_RETRY getcallback null \n");
            sleep(1);
        }			
        status = fun->triger(cmd, m_ei);
        if (!status.isOk()) {
            ALOGE("I_GPSVER_GET callback failed, retry time is %d \n", retry_num);
              if(retry_num++ < 5){
                  sleep(1);
                  goto CALLBACK_RETRY;
              }
        }else{
            ALOGE("I_GPSVER_GET callback OK \n");
        }      


        break;
    }
    return;
}

//GPS

#if 0
int main(int argc, char *argv[])
{
    int phonum=0, ret=0, phoch=0;
    char modeminfo[256];
    AprData aprData;

    ModemInfoThread modemInfo(&aprData);
    WCNInfoThread wcnInfo(&aprData);
    //GeInfoThread geInfo(&aprData);
    
    phonum = modemInfo.GetphoneNUM();
    ALOGD("AT phonum=%d\n", phonum);
    
    ALOGD("AT modemAlive=%d\n", modemInfo.isModemAlive());
    
    ALOGD("argv[1]=%s\n", argv[1]);
    if(atoi(argv[1])==0)
    {
        ret=modemInfo.GetVer(modeminfo, 256);
        if(ret!=-1)
        {
            ALOGD("AT modemVer=%s\n", modeminfo);
        }
    }
    
    if(atoi(argv[1])==1)
    {
        for(phoch=0; phoch<phonum; phoch++)
        {
            ret=modemInfo.GetIMEIs(phoch, modeminfo, 256);
            if(ret!=-1)
            {
                ALOGD("AT phonch %d, modemImei=%s\n", phoch, modeminfo);
            }
        }
    }
    
    if(atoi(argv[1])==2)
    {
        wcnInfo.SetPowerOn();
        ret=wcnInfo.GetVer(modeminfo, 256);
        if(ret!=-1)
        {
            ALOGD("get wcnd wcnVer=%s\n", modeminfo);
        }
    }
    ALOGD("Main ret=%d\n", ret);
}
#endif
