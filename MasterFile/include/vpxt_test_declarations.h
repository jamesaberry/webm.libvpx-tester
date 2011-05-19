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
#include "x86.h"
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

#define TEST_ONLY 3
#define COMP_ONLY 2
#define TEST_AND_COMP 1