#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_test_definitions.h"
#include "vpxt_utilities.h"
#include "vpxt_driver.h"
#include "yv12config.h"
#include "comp_ivf.h"
#include "vpx_mem.h"
#include "onyxd.h"
#include "onyx.h"
#include "ivf.h"
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#if defined(ARM)
#include "arm.h"
#else
#if !defined(_PPC)
#include "x86.h"
#endif
#endif

#if defined(_WIN32)
#include "on2vpplugin.h"
#include <windows.h>
#define snprintf _snprintf
#else
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#endif

extern "C"
{
#include "vpx_timer.h"
    extern vpx_codec_iface_t vpx_codec_vp8_cx_algo;
}

enum TestTypes {
    kNoTest = 0,
    kFullTest = 1,
    kCompOnly = 2,
    kTestOnly = 3,
    kResumeTest = 4
};

enum TestResultTypes {
    kTestFailed = 0,            // failed
    kTestPassed = 1,            // passed
    kTestIndeterminate = 2,     // indeterminate
    kTestComboLog = 3,          // see combo run log
    kTestPsnrLog = 4,           // see psnr log
    kTestRandomComplete = 5,    // random test completed
    kTestError = 6,             // test error
    kTestMinPassed = 8,         // min passed
    kTestEncCreated = 10,       // compression made
    kTestErrFileMismatch = 11,  // error file mismatch
    kTestNotSupported = 12,     // test not supported
    kTestPossibleArtifact = 13  // possible artifact
};
