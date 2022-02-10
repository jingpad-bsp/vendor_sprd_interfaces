#define LOG_TAG "Fingerprintmmi"

#include "Fingerprintmmi.h"
#include <stdio.h>
#include <stdlib.h>

namespace vendor {
namespace sprd {
namespace hardware {
namespace fingerprintmmi {
namespace V1_0 {
namespace implementation {

//#define FINGERPRINT_LIB_Default "libfactorylib.so"             //default fingerprint lib path


static const char *fp_lib[] = {
    "libgf_factory_test.so",
    "libfactorylib_silead.so",
};

static const int FP_LIB_COUNT =
    (sizeof(fp_lib)/sizeof(fp_lib[0]));

static int lib_count = 0;

void *handle;

typedef int (*CAC_FUNC)(void);

CAC_FUNC fp_factory_init = NULL;
CAC_FUNC fp_spi_test = NULL;
CAC_FUNC fp_interrupt_test = NULL;
CAC_FUNC fp_deadpixel_test = NULL;
CAC_FUNC fp_finger_detect = NULL;
CAC_FUNC fp_factory_exit = NULL;

static int goodix_test_result = -1;
static bool is_goodix = false;

// Methods from ::vendor::sprd::hardware::fingerprintmmi::V1_0::IFingerprintmmi follow.
Return<uint32_t> Fingerprintmmi::factory_init() {
    // TODO implement
    ALOGD(" factory_init()\n");
	
	goodix_test_result = -1;
	is_goodix = false;
    ALOGD("is_goodix111: %d", is_goodix);

	//File *goodixFp = = fopen ("/sys/devices/platform/soc/soc:ap-apb/70800000.spi/spi_master/spi0/spi0.0/debug/debug", "r");
	int goodixFp = open("/sys/devices/platform/soc/soc:ap-apb/70800000.spi/spi_master/spi0/spi0.0/debug/debug", O_RDONLY);
	//if(goodixFp) {
	if(goodixFp != -1) {
		is_goodix = true;
		close(goodixFp);
	} 
    ALOGD("is_goodix222: %d", is_goodix);

    int ret = -1;
    for(lib_count = 0; lib_count < FP_LIB_COUNT; lib_count++) {
        ALOGD("dlopen fingerprint lib: %s", fp_lib[lib_count]);
        handle = dlopen(fp_lib[lib_count], RTLD_LAZY);
        if (!handle) {
            ALOGD("fingersor lib dlopen failed! %s\n", dlerror());
        }else{
			if(is_goodix) {
				fp_factory_init = (CAC_FUNC)dlsym(handle, "gf_factory_test");
			} else {
	            fp_factory_init = (CAC_FUNC)dlsym(handle, "factory_init");
			}
				
            if(!fp_factory_init)
            {
                ALOGD("could not find symbol 'factory_init', %d IN\n", __LINE__);
            }
            else
            {
                ret = (*fp_factory_init)();

				if(is_goodix) {
					goodix_test_result = ret;
				} 
			 	

                if (ret != 0) {//ret != 0 means test fail,should continue retry next lib
                    ALOGD("factory_init fail, ret = %d\n", ret);
                    ret = -1;
                    continue;
                }
                else{//else ret == 0 means test pass,should break
                    break;
                }
            }
        }
    }

    return ret;
    //return 0;
}

Return<uint32_t> Fingerprintmmi::spi_test() {
    // TODO implement
    ALOGD(" spi_test()\n");
    int ret = -1;
	
	ALOGD("spi_test is_goodix: %d", is_goodix);
	if(is_goodix) {
	    ret = goodix_test_result;
	} else {
		fp_spi_test = (CAC_FUNC)dlsym(handle, "spi_test");
		ALOGD("do spi_test");
		if(!fp_spi_test)
		{
		    ALOGD("could not find symbol 'spi_test', %d IN\n", __LINE__);
		}
		else
		{
		    ret = (*fp_spi_test)();
		    if (ret != 0) {
		        ALOGD("spi_test fail, ret = %d\n", ret);
		        ret = -1;
		    }
		}

		if(ret != 0){//ret != 0 means test fail,should continue retry next lib
		    for(; lib_count < FP_LIB_COUNT; lib_count++) {
		        ALOGD("spi_test() retry dlopen fingerprint lib: %s", fp_lib[lib_count]);
		        handle = dlopen(fp_lib[lib_count], RTLD_LAZY);
		        if (!handle)
		        {
		            ALOGD("spi_test() fingersor lib dlopen failed! %s\n", dlerror());
		            continue;
		        }
		        fp_factory_init = (CAC_FUNC)dlsym(handle, "factory_init");
		        fp_spi_test = (CAC_FUNC)dlsym(handle, "spi_test");


		        ALOGD("do factory_init");
		        if(!fp_factory_init)
		        {
		            ALOGD("could not find symbol 'factory_init', %d IN\n", __LINE__);
		            continue;
		        }
		        else
		        {
		            ret = (*fp_factory_init)();
		            if (ret != 0) {
		                ALOGD("factory_init fail, ret = %d\n", ret);
		                ret = -1;
		                continue;
		            }
		        }

		        ALOGD("do spi_test");
		        if(!fp_spi_test)
		        {
		            ALOGD("could not find symbol 'spi_test', %d IN\n", __LINE__);
		            continue;
		        }
		        else
		        {
		            ret = (*fp_spi_test)();
		            if (ret != 0) {
		                ALOGD("spi_test fail, ret = %d\n", ret);
		                ret = -1;
		                continue;
		            }
		            else {//test pass,should break
		                break;
		            }
		        }

		    }
		}

	}
    return ret;
    //return 0;
}

Return<uint32_t> Fingerprintmmi::deadpixel_test() {
    // TODO implement
    ALOGD(" deadpixel_test()\n");
    int ret = -1;

	ALOGD("deadpixel_test is_goodix: %d", is_goodix);
	if(is_goodix) {
	    ret = goodix_test_result;
	} else {
		fp_deadpixel_test = (CAC_FUNC)dlsym(handle, "deadpixel_test");
		ALOGD("do deadpixel_test");
		if(!fp_deadpixel_test)
		{
		    ALOGD("could not find symbol 'deadpixel_test', %d IN\n", __LINE__);
		}
		else
		{
		    ret = (*fp_deadpixel_test)();
		    if (ret != 0) {
		        ALOGD("deadpixel_test fail, ret = %d\n", ret);
		        ret = -1;
		    }
		}
	}

    return ret;
    //return 0;
}

Return<uint32_t> Fingerprintmmi::interrupt_test() {
    // TODO implement
    ALOGD(" interrupt_test()\n");
    int ret = -1;

	ALOGD("interrupt_test is_goodix: %d", is_goodix);
	if(is_goodix) {
	    ret = goodix_test_result;
	} else {
		fp_interrupt_test = (CAC_FUNC)dlsym(handle, "interrupt_test");
		ALOGD("do interrupt_test");
		if(!fp_interrupt_test)
		{
		    ALOGD("could not find symbol 'interrupt_test', %d IN\n", __LINE__);
		}
		else
		{
		    ret = (*fp_interrupt_test)();
		    if (ret != 0) {
		        ALOGD("interrupt_test fail, ret = %d\n", ret);
		        ret = -1;
		    }
		}
	}

    return ret;
    //return 0;
}

Return<uint32_t> Fingerprintmmi::finger_detect() {
    // TODO implement
    ALOGD(" finger_detect()\n");
    int ret = -1;

	ALOGD("finger_detect is_goodix: %d", is_goodix);
	if(is_goodix) {
	    ret = goodix_test_result;
	} else {

		fp_finger_detect = (CAC_FUNC)dlsym(handle, "finger_detect");
		ALOGD("do finger_detect");
		if(!fp_finger_detect)
		{
		    ALOGD("could not find symbol 'finger_detect', %d IN\n", __LINE__);
		}
		else
		{
		    ret = (*fp_finger_detect)();
		    if (ret != 0) {
		        ALOGD("finger_detect fail, ret = %d\n", ret);
		        ret = -1;
		    }
		}

	}

    return ret;
    //return 0;
}

Return<uint32_t> Fingerprintmmi::factory_exit() {
    // TODO implement
    ALOGD(" factory_exit()\n");
    int ret = -1;

	ALOGD("factory_exit is_goodix: %d", is_goodix);
	if(is_goodix) {
	    ret = goodix_test_result;
	} else {
		fp_factory_exit = (CAC_FUNC)dlsym(handle, "factory_exit");
		ALOGD("do factory_exit");
		if(!fp_factory_exit)
		{
		    ALOGD("could not find symbol 'factory_exit', %d IN\n", __LINE__);
		}
		else
		{
		    ret = (*fp_factory_exit)();
		    if (ret != 0) {
		        ALOGD("factory_exit fail, ret = %d\n", ret);
		        ret = -1;
		    }
		}
	}

    return ret;
    //return 0;
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IFingerprintmmi* HIDL_FETCH_IFingerprintmmi(const char* /* name */) {
    ALOGD("HIDL_FETCH_IFingerprintmmi\n");
    return new Fingerprintmmi();
}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace fingerprintmmi
}  // namespace hardware
}  // namespace sprd
}  // namespace vendor
