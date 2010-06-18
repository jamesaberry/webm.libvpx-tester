//==========================================================================
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1999 - 2001  On2 Technologies Inc. All Rights Reserved.
//
//--------------------------------------------------------------------------
#ifndef ON2VPPLUGIN_H
#define ON2VPPLUGIN_H 1

#if defined (_WIN32) && !defined (_WIN32_WCE)
#include "windows.h"
#include "vfw.h"
typedef unsigned long LPARAM_64;
typedef unsigned long LRESULT_64;

#else
#include "on2windows.h"
#include "on2vfw.h"


#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

#include "string.h"
#include "on2plugmsgs.h"

#ifndef INLINE
#define INLINE __inline
#endif

#ifndef __ON2TYPES_H__

#ifdef linux
#define HASLONGLONG 1
#endif

#if defined(HASLONGLONG)
# define ON264 "lld"
typedef long long on2s64;
#elif defined(WIN32) || defined(_WIN32_WCE)
# define ON264 "I64d"
typedef __int64 on2s64;
#elif defined(__uClinux__) && defined(CHIP_DM642)
# include <lddk.h>
# define ON264 "lld"
typedef long on2s64;
#elif defined(__SYMBIAN32__)
# define ON264 "u"
typedef unsigned int on2s64;
#elif defined(TARGET_OS_MAC)
typedef SInt64 on2s64;
#elif defined (CHIP_DM642)
# define ON264 "ld"
typedef long on2s64;
#else
# define NO_64BIT_SUPPORT
#endif

#endif /*__ON2TYPES_H__*/

// Constants passed to the codec through the VFW ICSendMessage function

#ifdef __cplusplus
extern "C" {
#endif

    LRESULT_64 DriverProcVP7
    (
        DWORD dwDriverID,
        HDRVR hDriver,
        UINT uiMessage,
        LPARAM_64 lParam1,
        LPARAM_64 lParam2
    ) ;
    LRESULT_64 DriverProcVP6
    (
        DWORD dwDriverID,
        HDRVR hDriver,
        UINT uiMessage,
        LPARAM_64 lParam1,
        LPARAM_64 lParam2
    ) ;



// *********************************************************************************
// VP6 ONLY CALLS
#if defined(VP6_ONLY)
#define DRIVERPROC DriverProcVP6
#define THISHIC hic
    static INLINE HIC   ON2Open(DWORD fccType, DWORD fccHandler, UINT wMode)
    {
        ICOPEN ico;
        ico.dwSize = sizeof(ICOPEN);
        ico.fccType = fccType;
        ico.fccHandler = fccHandler;
        ico.dwFlags = wMode;

        return (HIC) DriverProcVP6((DWORD) 0, 0, DRV_OPEN, 0, (LPARAM_64) &ico);
    };
    static INLINE LRESULT_64   ON2Close(HIC hic)
    {
        DriverProcVP6((DWORD) hic, 0, DRV_CLOSE, 0, 0);
        return 0;
    }
// *********************************************************************************
// VP7 ONLY CALLS
#elif defined(VP7_ONLY)

#define DRIVERPROC DriverProcVP7
#define THISHIC hic

    static INLINE HIC   ON2Open(DWORD fccType, DWORD fccHandler, UINT wMode)
    {
        ICOPEN ico;
        ico.dwSize = sizeof(ICOPEN);
        ico.fccType = fccType;
        ico.fccHandler = fccHandler;
        ico.dwFlags = wMode;

        return (HIC) DriverProcVP7((DWORD) 0, 0, DRV_OPEN, 0, (LPARAM_64) &ico);
    };
    static INLINE LRESULT_64   ON2Close(HIC hic)
    {
        DriverProcVP7((DWORD) hic, 0, DRV_CLOSE, 0, 0);
        return 0;
    }

// *********************************************************************************
// MIX BETWEEN VP6 AND VP7
#else
    typedef struct
    {
        LRESULT_64(*DriverProc)
        (
            DWORD dwDriverID,
            HDRVR hDriver,
            UINT uiMessage,
            LPARAM_64 lParam1,
            LPARAM_64 lParam2
        ) ;
        HIC hic;
    } CODEC;

#define DRIVERPROC (((CODEC *) hic)->DriverProc)
#define THISHIC (((CODEC *) hic)->hic)

    static INLINE HIC   ON2Open(DWORD fccType, DWORD fccHandler, UINT wMode)
    {
        ICOPEN ico;
        CODEC *c;
        ico.dwSize = sizeof(ICOPEN);
        ico.fccType = fccType;
        ico.fccHandler = fccHandler;
        ico.dwFlags = wMode;

        c = (CODEC *) malloc(sizeof(CODEC));

        if (fccHandler == mmioFOURCC('V', 'P', '7', '0') ||
            fccHandler == mmioFOURCC('V', 'P', '7', '1'))
            c->DriverProc = DriverProcVP7;
        else
            c->DriverProc = DriverProcVP6;

        c->hic = (HIC) c->DriverProc((DWORD) 0, 0, DRV_OPEN, 0, (LPARAM_64) &ico);
        return (HIC) c;
    };
    static INLINE LRESULT   ON2Close(HIC hic)
    {
        CODEC *c = (CODEC *) hic;
        c->DriverProc((DWORD) c->hic, 0, DRV_CLOSE, 0, 0);
        free(c);
        return 0;
    }

#endif

// *********************************************************************************

    extern void *on2Logger;
    extern void      ON2SetLogger(void *pLogger);


    static INLINE LRESULT_64   ON2QueryAbout(HIC hic)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_ABOUT, (LPARAM_64) - 1, ICMF_ABOUT_QUERY) == ICERR_OK;
    }
    static INLINE LRESULT_64   ON2About(HIC hic,  HWND hwnd)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_ABOUT, (LPARAM_64)(UINT)(hwnd), 0);
    }
    static INLINE LRESULT_64   ON2QueryConfigure(HIC hic)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_CONFIGURE, (LPARAM_64) - 1, ICMF_CONFIGURE_QUERY) == ICERR_OK;
    }
    static INLINE LRESULT_64   ON2Configure(HIC hic,  HWND hwnd)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_CONFIGURE, (LPARAM_64)(UINT)(hwnd), 0);
    }
    static INLINE LRESULT_64   ON2GetState(HIC hic,  LPVOID pv, DWORD cb)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_GETSTATE, (LPARAM_64)(pv), (LPARAM_64)(cb));
    }
    static INLINE LRESULT_64   ON2SetState(HIC hic, LPVOID pv, DWORD cb)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_SETSTATE, (LPARAM_64)(pv), (LPARAM_64)(cb));
    }
    static INLINE LRESULT_64   ON2GetStateSize(HIC hic)
    {
        return ON2GetState(hic, NULL, 0);
    }
    INLINE static LRESULT_64 ON2GetDefaultQuality(HIC hic)
    {
        DWORD dwICValue;
        DRIVERPROC((DWORD) THISHIC, 0, ICM_GETDEFAULTQUALITY, (LPARAM_64)&dwICValue, sizeof(LPARAM_64));
        return dwICValue;
    }

    INLINE static LRESULT_64 ON2GetDefaultKeyFrameRate(HIC hic)
    {
        DWORD dwICValue;
        DRIVERPROC((DWORD) THISHIC, 0, ICM_GETDEFAULTKEYFRAMERATE, (LPARAM_64)&dwICValue, sizeof(LPARAM_64));
        return dwICValue;
    }
    static INLINE LRESULT_64   ON2CompressBegin(HIC hic,  BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_COMPRESS_BEGIN, (LPARAM_64)(lpbiInput), (LPARAM_64)(LPVOID)(lpbiOutput));
    }
    static INLINE LRESULT_64   ON2CompressQuery(HIC hic,  BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_COMPRESS_QUERY, (LPARAM_64)(lpbiInput), (LPARAM_64)(LPVOID)(lpbiOutput));
    }
    static INLINE LRESULT_64  ON2CompressGetFormat(HIC hic,  BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_COMPRESS_GET_FORMAT, (LPARAM_64)(lpbiInput), (LPARAM_64)(LPVOID)(lpbiOutput));
    }
    static INLINE LRESULT_64   ON2CompressGetFormatSize(HIC hic,  BITMAPINFO *lpbi)
    {
        return ON2CompressGetFormat(hic, lpbi, NULL);
    }
    static INLINE LRESULT_64   ON2CompressGetSize(HIC hic,  BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_COMPRESS_GET_SIZE, (LPARAM_64)(lpbiInput), (LPARAM_64)(LPVOID)(lpbiOutput));
    }
    static INLINE LRESULT_64   ON2CompressEnd(HIC hic)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_COMPRESS_END, 0, 0);
    }
    static INLINE LRESULT_64   ON2DecompressBegin(HIC hic, BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESS_BEGIN, (LPARAM_64)(lpbiInput), (LPARAM_64)(LPVOID)(lpbiOutput));
    }
    static INLINE LRESULT_64   ON2DecompressQuery(HIC hic,  BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESS_QUERY, (LPARAM_64)(lpbiInput), (LPARAM_64)(LPVOID)(lpbiOutput));
    }
    static INLINE LRESULT_64   ON2DecompressGetFormat(HIC hic,  BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESS_GET_FORMAT, (LPARAM_64)(lpbiInput), (LPARAM_64)(lpbiOutput));
    }
    static INLINE LRESULT_64   ON2DecompressGetFormatSize(HIC hic,  BITMAPINFO *lpbi)
    {
        return ON2DecompressGetFormat(hic, lpbi, NULL);
    }
    static INLINE LRESULT_64   ON2DecompressGetPalette(HIC hic,  BITMAPINFO *lpbiInput, BITMAPINFO *lpbiOutput)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESS_GET_PALETTE, (LPARAM_64)(lpbiInput), (LPARAM_64)(LPVOID)(lpbiOutput));
    }
    static INLINE LRESULT_64   ON2DecompressSetPalette(HIC hic,  BITMAPINFO *lpbiPalette)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESS_SET_PALETTE, (LPARAM_64)(lpbiPalette), 0);
    }
    static INLINE LRESULT   ON2DecompressEnd(HIC hic)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESS_END, 0, 0);
    }
    static INLINE LRESULT_64   ON2DecompressExEnd(HIC hic)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESSEX_END, 0, 0);
    }


    static INLINE LRESULT_64  ON2DecompressEx(HIC hic, DWORD dwFlags, LPBITMAPINFOHEADER lpbiSrc, LPVOID lpSrc, int xSrc, int ySrc, int dxSrc, int dySrc, LPBITMAPINFOHEADER lpbiDst, LPVOID lpDst, int xDst, int yDst, int dxDst, int dyDst)
    {
        ICDECOMPRESSEX ic;
        //BITMAPINFOHEADER bmi;  //MJH
        BITMAPV4HEADER bmi;

        if (lpbiDst->biCompression == BI_BITFIELDS)
            memcpy(&bmi, lpbiDst, sizeof(BITMAPINFOHEADER) + 12);
        else
            memcpy(&bmi, lpbiDst, sizeof(BITMAPINFOHEADER));

        if (dyDst < 0)
            bmi.bV4Height *= -1;

        ic.dwFlags = dwFlags;
        ic.lpbiSrc = lpbiSrc;
        ic.lpSrc = lpSrc;
        ic.xSrc = xSrc;
        ic.ySrc = ySrc;
        ic.dxSrc = dxSrc;
        ic.dySrc = dySrc;
        ic.lpbiDst = (BITMAPINFOHEADER *)&bmi; //lpbiDst;
        ic.lpDst = lpDst;
        ic.xDst = xDst;
        ic.yDst = yDst;
        ic.dxDst = dxDst;
        ic.dyDst = dyDst;

        // note that ICM swaps round the length and pointer
        // length in lparam2, pointer in lparam1

        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESSEX, (LPARAM_64)&ic, sizeof(ic));
    }

    /*
     *  ICDecompressExBegin()
     *
     *  start compression from a source format (lpbiInput) to a dest
     *  format (lpbiOutput) is supported.
     *
     */
    static INLINE LRESULT_64  ON2DecompressExBegin(HIC hic, DWORD dwFlags, LPBITMAPINFOHEADER lpbiSrc, LPVOID lpSrc, int xSrc, int ySrc, int dxSrc, int dySrc, LPBITMAPINFOHEADER lpbiDst, LPVOID lpDst, int xDst, int yDst, int dxDst, int dyDst)
    {
        ICDECOMPRESSEX ic;

        ic.dwFlags = dwFlags;
        ic.lpbiSrc = lpbiSrc;
        ic.lpSrc = lpSrc;
        ic.xSrc = xSrc;
        ic.ySrc = ySrc;
        ic.dxSrc = dxSrc;
        ic.dySrc = dySrc;
        ic.lpbiDst = lpbiDst;
        ic.lpDst = lpDst;
        ic.xDst = xDst;
        ic.yDst = yDst;
        ic.dxDst = dxDst;
        ic.dyDst = dyDst;

        // note that ICM swaps round the length and pointer
        // length in lparam2, pointer in lparam1

        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESSEX_BEGIN, (LPARAM_64)&ic, sizeof(ic));
    }

    /*
     *  ICDecompressExQuery()
     *
     */
    static INLINE LRESULT_64  ON2DecompressExQuery(HIC hic, DWORD dwFlags, LPBITMAPINFOHEADER lpbiSrc, LPVOID lpSrc, int xSrc, int ySrc, int dxSrc, int dySrc, LPBITMAPINFOHEADER lpbiDst, LPVOID lpDst, int xDst, int yDst, int dxDst, int dyDst)
    {
        ICDECOMPRESSEX ic;

        ic.dwFlags = dwFlags;
        ic.lpbiSrc = lpbiSrc;
        ic.lpSrc = lpSrc;
        ic.xSrc = xSrc;
        ic.ySrc = ySrc;
        ic.dxSrc = dxSrc;
        ic.dySrc = dySrc;
        ic.lpbiDst = lpbiDst;
        ic.lpDst = lpDst;
        ic.xDst = xDst;
        ic.yDst = yDst;
        ic.dxDst = dxDst;
        ic.dyDst = dyDst;

        // note that ICM swaps round the length and pointer
        // length in lparam2, pointer in lparam1

        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESSEX_QUERY, (LPARAM_64)&ic, sizeof(ic));
    }



    static INLINE LRESULT_64 ON2Decompress(HIC hic, DWORD dwFlags, LPBITMAPINFOHEADER  lpbiInput, LPVOID  lpInput, LPBITMAPINFOHEADER  lpbiOutput, LPVOID lpOutput)
    {
        ICDECOMPRESS ic;

        ic.dwFlags = dwFlags;
        ic.lpbiInput = lpbiInput;
        ic.lpInput = lpInput;
        ic.lpbiOutput = lpbiOutput;
        ic.lpOutput = lpOutput;

        // note that ICM swaps round the length and pointer
        // length in lparam2, pointer in lparam1

        return DRIVERPROC((DWORD) THISHIC, 0, ICM_DECOMPRESS, (LPARAM_64)&ic, sizeof(ic));
    }


    static INLINE LRESULT_64  ON2Compress(HIC hic, DWORD dwFlags, LPBITMAPINFOHEADER lpbiOutput, LPVOID lpOutput, LPBITMAPINFOHEADER lpbiInput, LPVOID lpInput, LPDWORD lpckid, LPDWORD lpdwFlags, int lFrameNum, DWORD dwFrameSize, DWORD dwQuality, LPBITMAPINFOHEADER lpbiPrev, LPVOID lpPrev)
    {
        ICCOMPRESS ic;

        ic.dwFlags =    dwFlags;
        ic.lpbiOutput = lpbiOutput;
        ic.lpOutput = lpOutput;
        ic.lpbiInput = lpbiInput;
        ic.lpInput = lpInput;
        ic.lpckid = lpckid;
        ic.lpdwFlags = lpdwFlags;
        ic.lFrameNum = lFrameNum;
        ic.dwFrameSize = dwFrameSize;
        ic.dwQuality = dwQuality;
        ic.lpbiPrev = lpbiPrev;
        ic.lpPrev  = lpPrev;

        return DRIVERPROC((DWORD) THISHIC, 0, ICM_COMPRESS, (LPARAM_64)&ic, sizeof(ic));
    }
#ifndef NO_64BIT_SUPPORT
    static INLINE DWORD ON2Compress2(HIC hic, DWORD dwFlags, LPBITMAPINFOHEADER lpbiOutput, LPVOID lpOutput, LPBITMAPINFOHEADER lpbiInput, LPVOID lpInput, LPDWORD lpckid, LPDWORD lpdwFlags, int lFrameNum, DWORD dwFrameSize, DWORD dwQuality, LPBITMAPINFOHEADER lpbiPrev, LPVOID lpPrev, on2s64 timeStamp)
    {
        DRIVERPROC((DWORD) THISHIC, 0, SET_TIMESTAMP, (LPARAM_64)&timeStamp, sizeof(timeStamp));
        return ON2Compress(hic, dwFlags, lpbiOutput, lpOutput, lpbiInput, lpInput, lpckid, lpdwFlags, lFrameNum, dwFrameSize, dwQuality, lpbiPrev, lpPrev);
    }
#endif


    static INLINE LRESULT_64   ON2SetReference(HIC hic, LPBITMAPINFO lpbiInput, char *frame)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_REFERENCE, (LPARAM_64) lpbiInput, (LPARAM_64) frame);
    }

    static INLINE LRESULT_64   ON2SetBlitBuff(HIC hic, LPBITMAPINFO lpbiInput, char *frame)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_BLITBUFF, (LPARAM_64) lpbiInput, (LPARAM_64) frame);
    }

    static INLINE LRESULT_64   ON2GetReference(HIC hic, LPBITMAPINFO lpbiInput, char *frame)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, GET_REFERENCE, (LPARAM_64) lpbiInput, (LPARAM_64) frame);
    }

    static INLINE LRESULT_64   ON2SetRecoveryFrame(HIC hic)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_RECOVERY_FRAME, 0, 0);
    }

    static INLINE LRESULT_64   ON2SetInternalSize(HIC hic, int wr, int ws, int hr, int hs)
    {
        char message[80];
        sprintf(message, "%d %d %d %d", wr, ws, hr, hs);
        return DRIVERPROC((DWORD) THISHIC, 0, SET_INTERNAL_SIZE, (LPARAM_64) message, 0);
    }

    static INLINE LRESULT_64   ON2SetInternalSize2(HIC hic, int widthscale, int heightscale)
    {
        if (widthscale < 4 && widthscale >= 0 && heightscale < 4 && heightscale >= 0)
        {
            int choice = (heightscale << 2) | (widthscale);
            return DRIVERPROC((DWORD) THISHIC, 0, SET_INTERNAL_SIZE, (LPARAM_64) choice, 0);
        }

        return (LRESULT_64) ICERR_UNSUPPORTED;
    }

    static INLINE LRESULT_64   ON2SendMessage(HIC hic, UINT msg, DWORD dw1, DWORD dw2)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, msg, dw1, dw2);
    }

    static INLINE LRESULT_64   ON2SetCPUFree(HIC hic, int cpuFree)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_CPUFREE, cpuFree, 0);
    }

    static INLINE LRESULT_64   ON2SetUpdateLast(HIC hic, int value)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_UPDATE_LAST, value, 0);
    }
    static INLINE LRESULT_64   ON2SetUpdateGold(HIC hic, int value)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_UPDATE_GOLD, value, 0);
    }
    static INLINE LRESULT_64   ON2SetUseOnlyLast(HIC hic, int value)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_USEONLY_LAST, value, 0);
    }
    static INLINE LRESULT_64   ON2SetUseOnlyGold(HIC hic, int value)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_USEONLY_GOLD, value, 0);
    }
    static INLINE LRESULT_64   ON2SetKey(HIC hic, int value)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_KEY, value, 0);
    }

    static INLINE LRESULT_64 ON2EncodeAlpha(HIC hic, int set, int *encode)
    {
        return DRIVERPROC((DWORD) THISHIC, 0,
                          set ? SET_ENCODE_ALPHA : GET_ENCODE_ALPHA, (LPARAM_64)encode, 0);
    }

    static INLINE LRESULT_64 ON2AlphaEncoded(HIC hic, int alpha_encoded)
    {
        return DRIVERPROC((DWORD) THISHIC, 0,
                          ALPHA_ENCODED, (LPARAM_64)alpha_encoded, 0);
    }

    static INLINE LRESULT_64 ON2Similarity(HIC hic, int *similarity)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, GET_SIMILARITY,
                          (LPARAM_64)similarity, 0);
    }

    static INLINE LRESULT ON2SetCPUused(HIC hic, int cpu_used)
    {
        return DRIVERPROC((unsigned long) THISHIC, 0,
                          SET_CPU_USED, (DWORD)cpu_used, 0);
    }
    static INLINE LRESULT ON2GetDropCount(HIC hic)
    {
        return DRIVERPROC((unsigned long) THISHIC, 0, GET_DROPCOUNT, 0, 0);
    }
    static INLINE LRESULT_64   ON2SetImportanceMap(HIC hic, int *frame)
    {
        return DRIVERPROC((DWORD) THISHIC, 0, SET_IMPORTANCEMAP, (LPARAM_64) frame , 0);
    }

#ifdef __cplusplus
}
#endif


#endif

