/****************************************************************************
 *
 *   Module Title : header.h
 *
 *   Description  : frame header
 *
 *   Copyright (c) 1999 - 2005  On2 Technologies Inc. All Rights Reserved.
 *
 ***************************************************************************/
#ifndef __INC_HEADER_H
#define __INC_HEADER_H

/* 24 bits total */
typedef struct
{
    unsigned int type: 1;
    unsigned int version: 3;
    unsigned int showFrame: 1;

    /* Allow 2^20 bytes = 8 megabits for first partition */

    unsigned int firstPartitionLengthInBytes: 19;

#ifdef PACKET_TESTING
    unsigned int frameNumber;
    unsigned int updateGold: 1;
    unsigned int usesGold: 1;
    unsigned int updateLast: 1;
    unsigned int usesLast: 1;
#endif

} VP8_HEADER;

#ifdef PACKET_TESTING
#define VP8_HEADER_SIZE 8
#else
#define VP8_HEADER_SIZE 3
#endif


#endif
