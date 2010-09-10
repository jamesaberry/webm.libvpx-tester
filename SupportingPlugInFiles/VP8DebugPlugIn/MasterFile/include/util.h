//------------------------------------------------------------------------------
//
//  Copyright (c) 1999-2009  On2 Technologies Inc.  All Rights Reserved.
//
//------------------------------------------------------------------------------

#include "on2_image.h"
#include <stdio.h>
#include "ivf.h"

/*
 * Initialize the IVF file header structure
 */
void ivf_write_header
(
    FILE *outfile,
    unsigned int    width,
    unsigned short  height,
    unsigned int    rate,
    unsigned int    scale,
    unsigned int    length
);
int ivf_read_header
(
    FILE *infile,
    IVF_HEADER *ivf
);

void ivf_write_frame
(
    FILE *outfile,
    uint64_t timeStamp,
    uint32_t frameSize,
    uint8_t *data
);

int ivf_read_frame(FILE       *infile,
                   uint8_t               **buf,
                   uint32_t              *buf_sz,
                   uint32_t              *buf_alloc_sz) ;

/*
 * Reading a video frame data from a file into an on2_image_t structure
 */

int read_frame(FILE *f, on2_image_t *img) ;

/*
 * writting a video frame data to a file from an on2_image_t structure
 */

void out_put_frame(void *out, on2_image_t *img) ;
