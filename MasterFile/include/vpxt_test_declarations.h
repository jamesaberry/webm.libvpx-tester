#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_test_definitions.h"
#include "onyx.h"
#include "onyxd.h"
#include "yv12config.h"
#include "vpx_mem.h"
#include "ivf.h"
#include "comp_ivf.h"
#include <map>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "x86.h"
#include "utilities.h"
#include "driver.h"
using namespace std;

#if defined(_WIN32)
#include "on2vpplugin.h"
#include <windows.h>
#include <tchar.h>
#include <direct.h>
#define snprintf _snprintf
#elif defined(linux)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#elif defined(__APPLE__)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#elif defined(__POWERPC__)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#endif

extern "C"
{
#include "vpx_timer.h"
    extern vpx_codec_iface_t vpx_codec_vp8_cx_algo;
}

#define TEST_ONLY 3
#define COMP_ONLY 2
#define TEST_AND_COMP 1