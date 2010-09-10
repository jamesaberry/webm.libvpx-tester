
/*==============================================================================
Copyright © 2005 On2 Technology, Inc. All rights reserved.

FILE NAME:
    on2vfw.h

PURPOSE:


FUNCTION(S):
    <Afunction> - <Adescription>
    <Bfunction> - <Bdescription>

    local:
        <AlocalFunction> - <AlocalDescription>
        <BlocalFunction> - <BlocalDescription>

NOTES:


    <note description>

CHANGE HISTORY:
$History: $


==============================================================================*/
#ifndef __ON2VFW_H
#define __ON2VFW_H
/*==============================================================================
                              Includes
==============================================================================*/

//#ifndef _WIN32

/*==============================================================================
                              Defines
==============================================================================*/

#ifndef mmioFOURCC
#define mmioFOURCC( ch0, ch1, ch2, ch3 )                \
    ( (DWORD)(BYTE)(ch0) | ( (DWORD)(BYTE)(ch1) << 8 ) |    \
      ( (DWORD)(BYTE)(ch2) << 16 ) | ( (DWORD)(BYTE)(ch3) << 24 ) )
#endif

#ifndef ICERR_OK
#define ICERR_OK                0L
#define ICERR_DONTDRAW          1L
#define ICERR_NEWPALETTE        2L
#define ICERR_GOTOKEYFRAME  3L
#define ICERR_STOPDRAWING   4L

#define ICERR_UNSUPPORTED      -1L
#define ICERR_BADFORMAT        -2L
#define ICERR_MEMORY           -3L
#define ICERR_INTERNAL         -4L
#define ICERR_BADFLAGS         -5L
#define ICERR_BADPARAM         -6L
#define ICERR_BADSIZE          -7L
#define ICERR_BADHANDLE        -8L
#define ICERR_CANTUPDATE       -9L
#define ICERR_ABORT        -10L
#define ICERR_ERROR            -100L
#define ICERR_BADBITDEPTH      -200L
#define ICERR_BADIMAGESIZE     -201L

#define ICERR_CUSTOM           -400L    // errors less than ICERR_CUSTOM...

#endif

#define ICDECOMPRESS_HURRYUP      0x80000000L   // don't draw just buffer (hurry up!)
#define ICDECOMPRESS_UPDATE       0x40000000L   // don't draw just update screen
#define ICDECOMPRESS_PREROLL      0x20000000L   // this frame is before real start
#define ICDECOMPRESS_NULLFRAME    0x10000000L   // repeat last frame
#define ICDECOMPRESS_NOTKEYFRAME  0x08000000L   // this frame is not a key frame


/* Driver messages */
#define DRV_LOAD                0x0001
#define DRV_ENABLE              0x0002
#define DRV_OPEN                0x0003
#define DRV_CLOSE               0x0004
#define DRV_DISABLE             0x0005
#define DRV_FREE                0x0006
#define DRV_CONFIGURE           0x0007
#define DRV_QUERYCONFIGURE      0x0008
#define DRV_INSTALL             0x0009
#define DRV_REMOVE              0x000A
#define DRV_EXITSESSION         0x000B
#define DRV_POWER               0x000F
#define DRV_RESERVED            0x0800
#define DRV_USER                0x4000

#define ICM_USER                    (DRV_USER+0x0000)
#define ICM_RESERVED_LOW            (DRV_USER+0x1000)
#define ICM_RESERVED_HIGH           (DRV_USER+0x2000)
#define ICM_RESERVED                ICM_RESERVED_LOW
#define ICM_GETSTATE                (ICM_RESERVED+0)    // Get compressor state
#define ICM_SETSTATE                (ICM_RESERVED+1)    // Set compressor state
#define ICM_GETINFO                 (ICM_RESERVED+2)    // Query info vp7about the compressor
#define ICM_CONFIGURE               (ICM_RESERVED+10)   // show the configure dialog
#define ICM_ABOUT                   (ICM_RESERVED+11)   // show the vp7about box
#define ICM_GETDEFAULTQUALITY       (ICM_RESERVED+30)   // get the default value for quality
#define ICM_GETQUALITY              (ICM_RESERVED+31)   // get the current value for quality
#define ICM_SETQUALITY              (ICM_RESERVED+32)   // set the default value for quality
#define ICM_SET                     (ICM_RESERVED+40)   // Tell the driver something
#define ICM_GET                     (ICM_RESERVED+41)   // Ask the driver something

#define ICM_GETDEFAULTKEYFRAMERATE  (ICM_USER+42)   // get the default value for key frames

#define ICM_COMPRESS_GET_FORMAT     (ICM_USER+4)    // get compress format or size
#define ICM_COMPRESS_GET_SIZE       (ICM_USER+5)    // get output size
#define ICM_COMPRESS_QUERY          (ICM_USER+6)    // query support for compress
#define ICM_COMPRESS_BEGIN          (ICM_USER+7)    // begin a series of compress calls.
#define ICM_COMPRESS                (ICM_USER+8)    // compress a frame
#define ICM_COMPRESS_END            (ICM_USER+9)    // end of a series of compress calls.

#define ICM_DECOMPRESS_GET_FORMAT   (ICM_USER+10)   // get decompress format or size
#define ICM_DECOMPRESS_QUERY        (ICM_USER+11)   // query support for dempress
#define ICM_DECOMPRESS_BEGIN        (ICM_USER+12)   // start a series of decompress calls
#define ICM_DECOMPRESS              (ICM_USER+13)   // decompress a frame
#define ICM_DECOMPRESS_END          (ICM_USER+14)   // end a series of decompress calls
#define ICM_DECOMPRESS_SET_PALETTE  (ICM_USER+29)   // fill in the DIB color table
#define ICM_DECOMPRESS_GET_PALETTE  (ICM_USER+30)   // fill in the DIB color table
#define ICM_DECOMPRESSEX_BEGIN      (ICM_USER+60)   // start a series of decompress calls
#define ICM_DECOMPRESSEX_QUERY      (ICM_USER+61)   // start a series of decompress calls
#define ICM_DECOMPRESSEX            (ICM_USER+62)   // decompress a frame
#define ICM_DECOMPRESSEX_END        (ICM_USER+63)   // end a series of decompress calls

#define ICM_COMPRESS_FRAMES_INFO    (ICM_USER+70)   // tell vp7about compress to come
#define ICM_SET_STATUS_PROC         (ICM_USER+72)   // set status callback


#define ICMF_CONFIGURE_QUERY     0x00000001
#define ICMF_ABOUT_QUERY         0x00000001

#ifndef ICTYPE_VIDEO
#define ICTYPE_VIDEO    mmioFOURCC('v', 'i', 'd', 'c')
#define ICTYPE_AUDIO    mmioFOURCC('a', 'u', 'd', 'c')
#endif


/* Values for dwFlags of ICOpen() */
#ifndef ICMODE_COMPRESS
#define ICMODE_COMPRESS     1
#define ICMODE_DECOMPRESS   2
#define ICMODE_FASTDECOMPRESS   3
#define ICMODE_QUERY            4
#define ICMODE_FASTCOMPRESS     5
#define ICMODE_DRAW             8
#endif

#define ICVERSION       0x0104

/* Flags for the <dwFlags> field of the <ICINFO> structure. */
#define VIDCF_QUALITY        0x0001  // supports quality
#define VIDCF_CRUNCH         0x0002  // supports crunching to a frame size
#define VIDCF_TEMPORAL       0x0004  // supports inter-frame compress
#define VIDCF_COMPRESSFRAMES 0x0008  // wants the compress all frames message
#define VIDCF_DRAW           0x0010  // supports drawing
#define VIDCF_FASTTEMPORALC  0x0020  // does not need prev frame on compress
#define VIDCF_FASTTEMPORALD  0x0080  // does not need prev frame on decompress



#define ICCOMPRESS_KEYFRAME 0x00000001L
#define AVIIF_LIST          0x00000001L // chunk is a 'LIST'
#define AVIIF_KEYFRAME      0x00000010L // this frame is a key frame.




/* return values from DriverProc() function */
#define DRVCNF_CANCEL           0x0000
#define DRVCNF_OK               0x0001
#define DRVCNF_RESTART          0x0002
#define DRV_CANCEL             DRVCNF_CANCEL
#define DRV_OK                 DRVCNF_OK
#define DRV_RESTART            DRVCNF_RESTART




/*==============================================================================
                            Type Definitions
==============================================================================*/


typedef struct tagON2BITMAPINFOHEADER
{
    DWORD      biSize;
    LONG       biWidth;
    LONG       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    LONG       biXPelsPerMeter;
    LONG       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;

} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct
{
    //
    // same as ICM_DECOMPRESS
    //
    DWORD               dwFlags;

    LPBITMAPINFOHEADER  lpbiSrc;    // BITMAPINFO of compressed data
    LPVOID              lpSrc;      // compressed data

    LPBITMAPINFOHEADER  lpbiDst;    // DIB to decompress to
    LPVOID              lpDst;      // output data

    //
    // new for ICM_DECOMPRESSEX
    //
    int                 xDst;       // destination rectangle
    int                 yDst;
    int                 dxDst;
    int                 dyDst;

    int                 xSrc;       // source rectangle
    int                 ySrc;
    int                 dxSrc;
    int                 dySrc;

} ICDECOMPRESSEX;


typedef struct
{
    DWORD               dwFlags;    // flags (from AVI index...)

    LPBITMAPINFOHEADER  lpbiInput;  // BITMAPINFO of compressed data
    // biSizeImage has the chunk size
    LPVOID              lpInput;    // compressed data

    LPBITMAPINFOHEADER  lpbiOutput; // DIB to decompress to
    LPVOID              lpOutput;
    DWORD       ckid;       // ckid from AVI file
} ICDECOMPRESS;


typedef struct tagRGBQUAD
{
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved;
} RGBQUAD;



typedef struct
{
    DWORD               dwSize;         // sizeof(ICOPEN)
    DWORD               fccType;        // 'vidc'
    DWORD               fccHandler;     //
    DWORD               dwVersion;      // version of compman opening you
    DWORD               dwFlags;        // LOWORD is type specific
    LRESULT             dwError;        // error return.
    LPVOID              pV1Reserved;    // Reserved
    LPVOID              pV2Reserved;    // Reserved
    DWORD               dnDevNode;      // Devnode for PnP devices
} ICOPEN;


typedef struct
{
    DWORD   dwSize;                 // sizeof(ICINFO)
    DWORD   fccType;                // compressor type     'vidc' 'audc'
    DWORD   fccHandler;             // compressor sub-type 'rle ' 'jpeg' 'pcm '
    DWORD   dwFlags;                // flags LOWORD is type specific
    DWORD   dwVersion;              // version of the driver
    DWORD   dwVersionICM;           // version of the ICM used
    //
    // under Win32, the driver always returns UNICODE strings.
    //
    WCHAR   szName[16];             // short name
    WCHAR   szDescription[128];     // long name
    WCHAR   szDriver[128];          // driver that contains compressor
}   ICINFO;


typedef struct
{
    DWORD               dwFlags;        // flags

    LPBITMAPINFOHEADER  lpbiOutput;     // output format
    LPARAM              lOutput;        // output identifier

    LPBITMAPINFOHEADER  lpbiInput;      // format of frame to compress
    LPARAM              lInput;         // input identifier

    LONG                lStartFrame;    // start frame
    LONG                lFrameCount;    // # of frames

    LONG                lQuality;       // quality
    LONG                lDataRate;      // data rate
    LONG                lKeyRate;       // key frame rate

    DWORD       dwRate;     // frame rate, as always
    DWORD       dwScale;

    DWORD       dwOverheadPerFrame;
    DWORD       dwReserved2;

    LONG(CALLBACK *GetData)(LPARAM lInput, LONG lFrame, LPVOID lpBits, LONG len);
    LONG(CALLBACK *PutData)(LPARAM lOutput, LONG lFrame, LPVOID lpBits, LONG len);
} ICCOMPRESSFRAMES;

typedef struct
{
    DWORD               dwFlags;        // flags

    LPBITMAPINFOHEADER  lpbiOutput;     // output format
    LPVOID              lpOutput;       // output data

    LPBITMAPINFOHEADER  lpbiInput;      // format of frame to compress
    LPVOID              lpInput;        // frame data to compress

    LPDWORD             lpckid;         // ckid for data in AVI file
    LPDWORD             lpdwFlags;      // flags in the AVI index.
    LONG                lFrameNum;      // frame number of seq.
    DWORD               dwFrameSize;    // reqested size in bytes. (if non zero)

    DWORD               dwQuality;      // quality

    // these are new fields
    LPBITMAPINFOHEADER  lpbiPrev;       // format of previous frame
    LPVOID              lpPrev;         // previous frame

} ICCOMPRESS;


typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO, FAR *LPBITMAPINFO, *PBITMAPINFO;

/*=============================================================================
                                Enums
==============================================================================*/


/*==============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                             Constants
==============================================================================*/


/*==============================================================================
                               Variables
==============================================================================*/
static DWORD dwICValue;

/*==============================================================================
                            Function Protoypes/MICROS
==============================================================================*/
/* installable driver function prototypes */


LRESULT   WINAPI DrvClose(HDRVR hdrvr, LPARAM lParam1, LPARAM lParam2);
HDRVR     WINAPI DrvOpen(LPCSTR szDriverName, LPCSTR szSectionName, LPARAM lParam2);
LRESULT   WINAPI DrvSendMessage(HDRVR hdrvr, UINT uMsg, LPARAM lParam1, LPARAM lParam2);
HINSTANCE WINAPI DrvGetModuleHandle(HDRVR hdrvr);
LRESULT   WINAPI DrvDefDriverProc(DWORD dwDriverIdentifier, HDRVR hdrvr, UINT uMsg, LPARAM lParam1, LPARAM lParam2);
#define DefDriverProc DrvDefDriverProc
#else


//#endif /* else ifndef _WIN32 */

#endif