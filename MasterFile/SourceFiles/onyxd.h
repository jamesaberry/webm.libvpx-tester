/****************************************************************************
 *
 *   Module Title : onyxd.h
 *
 *   Description  : main published interface to decoder
 *
 *   Copyright (c) 1999 - 2005  On2 Technologies Inc. All Rights Reserved.
 *
 ***************************************************************************/
#ifndef __INC_VP8D_H
#define __INC_VP8D_H


/* Create/destroy static data structures. */
#ifdef __cplusplus
extern "C"
{
#endif
#include "type_aliases.h"
#include "yv12config.h"

#ifndef API
#include "ppflags.h"
#endif

    typedef void   *VP8D_PTR;
    typedef struct
    {
        int		Width;
        int		Height;
        int		Version;
        int     postprocess;
        int     max_threads;
    } VP8D_CONFIG;

#ifndef onyxreferenceframetype
#define onyxreferenceframetype
    typedef enum
    {
        VP8_LAST_FLAG = 1,
        VP8_GOLD_FLAG = 2,
        VP8_ALT_FLAG = 4
    } VP8_REFFRAME;
#endif

    typedef enum
    {
        VP8D_OK = 0
    } VP8D_SETTING;

    void vp8dx_Initialize(void);

    void vp8dx_Shutdown(void);

    void vp8dx_SetSetting(VP8D_PTR comp, VP8D_SETTING oxst, int x);

    int vp8dx_GetSetting(VP8D_PTR comp, VP8D_SETTING oxst);

    int vp8dx_ReceiveCompressedData(VP8D_PTR comp, unsigned long size, unsigned char *dest, INT64 timeStamp);
    int vp8dx_GetRawFrame(VP8D_PTR comp, YV12_BUFFER_CONFIG *sd, INT64 *timeStamp, INT64 *timeEndStamp, int deblock_level,  int noise_level, int flags);

    int vp8dx_GetReference(VP8D_PTR comp, VP8_REFFRAME RefFrameFlag, YV12_BUFFER_CONFIG *sd);
    int vp8dx_SetReference(VP8D_PTR comp, VP8_REFFRAME RefFrameFlag, YV12_BUFFER_CONFIG *sd);

    VP8D_PTR vp8dx_CreateDecompressor(VP8D_CONFIG *oxcf);

    void vp8dx_RemoveDecompressor(VP8D_PTR comp);

#ifdef __cplusplus
}
#endif


#endif

