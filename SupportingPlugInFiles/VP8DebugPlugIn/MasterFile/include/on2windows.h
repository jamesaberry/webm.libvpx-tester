
/*==============================================================================
Copyright  2005 On2 Technology, Inc. All rights reserved.

FILE NAME:
    on2windows.h

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
$History$


==============================================================================*/
#if defined(HAVE_WINDOWS_H) || (defined (_WIN32) && !defined (_WIN32_WCE))
#include "windows.h"

typedef unsigned long LRESULT_64;
typedef unsigned long LPARAM_64;

#define __ON2WINDOWS_H
#endif

#ifndef __ON2WINDOWS_H
#define __ON2WINDOWS_H
/*==============================================================================
                              Includes
==============================================================================*/
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32_WCE) && !defined(HAVE_WINDOWS_H)
#include <wtypes.h>
#endif
/*==============================================================================
                              Defines
==============================================================================*/
#ifndef FAR
#define FAR
#endif

#ifndef NEAR
#define NEAR
#endif

#ifndef WINAPI
#define WINAPI
#endif

#ifndef WINMMAPI
#define WINMMAPI
#endif

#ifndef PASCAL
#define PASCAL
#endif

#ifndef CALLBACK
#define CALLBACK
#endif

/* constants for the biCompression field */
#ifndef BI_RGB
#  define BI_RGB        0L
#  define BI_RLE8       1L
#  define BI_RLE4       2L
#  define BI_BITFIELDS  3L
#endif

#ifdef _INTEL_LINUX
#define PACK_STRUCT __attribute__((packed))
#pragma align 1
#endif

#ifndef PACK_STRUCT
#define PACK_STRUCT
#endif




/*==============================================================================
                            Type Definitions
==============================================================================*/
#ifndef _WIN32_WCE
typedef unsigned char BYTE;
typedef unsigned int UINT;

/*
// original typdefs
//typedef unsigned long LONG;
//typedef unsigned long DWORD;
//typedef long * LONG_PTR;
//typedef long FXPT2DOT30, FAR *LPFXPT2DOT30;

	A long on a TI part is 40bits so inorder to be compatible on the TI, a long
	must be defined as an int.
*/
typedef unsigned int DWORD;
typedef int LONG;
typedef int *LONG_PTR;


typedef unsigned short WORD;
typedef unsigned short WCHAR;
//typedef int BOOL;
typedef void   *LPVOID;
typedef DWORD *LPDWORD;
typedef UINT WPARAM;
typedef int LPARAM;
typedef unsigned int LRESULT;
//typedef int LRESULT;
typedef void *HDRVR;
typedef const WCHAR FAR *LPCWSTR;
typedef char FAR *LPSTR;
typedef LPSTR LPCTSTR;
typedef const char *LPCSTR;

typedef unsigned long LPARAM_64;
typedef unsigned long LRESULT_64;



typedef void   *HWND;
typedef HWND HIC;
typedef void FAR *FARPROC;
typedef void FAR *HINSTANCE;
#endif

typedef int FXPT2DOT30, FAR *LPFXPT2DOT30;

typedef struct tagCIEXYZ
{
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct tagICEXYZTRIPLE
{
    CIEXYZ  ciexyzRed;
    CIEXYZ  ciexyzGreen;
    CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct
{
    DWORD        bV4Size;
    LONG         bV4Width;
    LONG         bV4Height;
    WORD         bV4Planes;
    WORD         bV4BitCount;
    DWORD        bV4V4Compression;
    DWORD        bV4SizeImage;
    LONG         bV4XPelsPerMeter;
    LONG         bV4YPelsPerMeter;
    DWORD        bV4ClrUsed;
    DWORD        bV4ClrImportant;
    DWORD        bV4RedMask;
    DWORD        bV4GreenMask;
    DWORD        bV4BlueMask;
    DWORD        bV4AlphaMask;
    DWORD        bV4CSType;
    CIEXYZTRIPLE bV4Endpoints;
    DWORD        bV4GammaRed;
    DWORD        bV4GammaGreen;
    DWORD        bV4GammaBlue;
} BITMAPV4HEADER, FAR *LPBITMAPV4HEADER, *PBITMAPV4HEADER;

#ifdef __MWERKS__
#pragma options align= packed
#endif

#ifdef __GNUC__
#pragma pack(1)
#endif

#ifndef _WIN32_WCE
typedef struct PACK_STRUCT tagBITMAPFILEHEADER
{
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
#endif

#ifdef __GNUC__
#pragma pack(4)
#endif

#ifdef __MWERKS__
#pragma options align= reset
#endif

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

/*==============================================================================
                            Function Protoypes/MICROS
==============================================================================*/


#endif

