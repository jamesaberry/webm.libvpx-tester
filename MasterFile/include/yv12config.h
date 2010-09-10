/****************************************************************************
 *
 *   Module Title : yv12config.h
 *
 *   Description  : definition of a yv12 frame
 *
 *   Copyright (c) 1999 - 2005  On2 Technologies Inc. All Rights Reserved.
 *
 ***************************************************************************/
#ifndef YV12_CONFIG_H
#define YV12_CONFIG_H
#ifdef __cplusplus
extern "C"
{
#endif

#define VP7BORDERINPIXELS       48
#define VP8BORDERINPIXELS       32

    /*************************************
     For INT_YUV:

     Y = (R+G*2+B)/4;
     U = (R-B)/2;
     V =  (G*2 - R - B)/4;
    And
     R = Y+U-V;
     G = Y+V;
     B = Y-U-V;
    ************************************/
    typedef enum
    {
        REG_YUV = 0,    // Regular yuv
        INT_YUV = 1     // The type of yuv that can be tranfer to and from RGB through integer transform
              }
              YUV_TYPE;

    typedef struct
    {
        int   YWidth;
        int   YHeight;
        int   YStride;
//    int   YInternalWidth;

        int   UVWidth;
        int   UVHeight;
        int   UVStride;
//    int   UVInternalWidth;

        unsigned char *YBuffer;
        unsigned char *UBuffer;
        unsigned char *VBuffer;

        unsigned char *BufferAlloc;
        int border;
        int frameSize;
        YUV_TYPE clrtype;
    } YV12_BUFFER_CONFIG;

    int vp8_On2YV12_AllocFrameBuffer(YV12_BUFFER_CONFIG *ybf, int width, int height, int border);
    int vp8_On2YV12_DeAllocFrameBuffer(YV12_BUFFER_CONFIG *ybf);
    int vp8_On2YV12_BlackFrameBuffer(YV12_BUFFER_CONFIG *ybf);

#ifdef __cplusplus
}
#endif


#endif //YV12_CONFIG_H

