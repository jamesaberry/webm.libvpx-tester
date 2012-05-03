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

#define NO_TEST   0
#define FULL_TEST 1
#define COMP_ONLY 2
#define TEST_ONLY 3
#define RESU_TEST 4

#define TEST_FAILED  0 // failed
#define TEST_PASSED  1 // passed
#define TEST_INDT    2 // indeterminate
#define TEST_COMBO   3 // see combo run log
#define TEST_PSNRL   4 // see psnr log
#define TEST_RANDT   5 // random test completed
#define TEST_ERROR   6 // test error
#define TEST_MINPA   8 // min passed
#define TEST_COMPM  10 // compression made
#define TEST_ERRFM  11 // error file mismatch
#define TEST_TSTNS  12 // test not supported

