#define _CRT_SECURE_NO_WARNINGS
////////////////////////
#include "on2-vpx-shim.h"
#include "test-definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
/////VP8////////////////
#include "onyx.h"
#include "onyxd.h"
#include "vp8.h"
#include <map>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
/////VP8 IVF PSNR///////
#include <cassert>
#include "yv12config.h"
/////PSNR///
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "ivf.h"
#include "header.h"
using namespace std;
////////////////////////
#include <stdio.h>
#include <sys/stat.h>
////////////////////////
///////////itoa_custom/////////
#include <algorithm>
////////////////////////

#if defined(_WIN32)
#include <windows.h>
#include "on2vpplugin.h"
#define snprintf _snprintf
#endif
#include <cstdio>
//////////////////////////////////////////////////////////////
////////////////////////Global Slash Character Definion for multiplat////////////////////////
extern char slashChar;
extern string slashCharStr;
/////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DSHOW
extern char TesterExePath[256];
#endif

#if defined(_WIN32)
#include "stdafx.h"
#endif
const int PSNR_MAX = 999.;
const int sizBuff = 512;

extern int IVF2Raw(char *inputFile, char *outputDir);
extern void FileName(char *input, char *FileName);
extern void FormatedPrint(string SummaryStr, int selector);

extern void VP8DefaultParms(VP8_CONFIG &opt);
extern void writeframe(YV12_BUFFER_CONFIG &sd, ofstream &outputfile);

extern double VP8_CalcSSIM2(YV12_BUFFER_CONFIG *source, YV12_BUFFER_CONFIG *dest, int lumamask, double *weight);
extern int FormatIVFHeaderRead(IVF_HEADER *ivf);

/////////////////////////////////////////////Endian Conversions/////////////////////////////////////////////
typedef unsigned char BYTE;

#ifndef _WIN32
typedef unsigned int  DWORD;
#endif

#define HEADER_SIZE 32
#define IVF_SCALE	1000

#ifdef __POWERPC__
# define make_endian_16(a) \
    (((unsigned int)(a & 0xff)) << 8) | (((unsigned int)(a & 0xff00)) >> 8)
# define make_endian_32(a)																	\
    (((unsigned int)(a & 0xff)) << 24)    | (((unsigned int)(a & 0xff00)) << 8) |	\
    (((unsigned int)(a & 0xff0000)) >> 8) | (((unsigned int)(a & 0xff000000)) >> 24)
# define make_endian_64(a)	\
    ((a & 0xff) << 56 			|	\
     ((a >>  8) & 0xff) << 48	|	\
     ((a >> 16) & 0xff) << 40	|	\
     ((a >> 24) & 0xff) << 32	|	\
     ((a >> 32) & 0xff) << 24	|	\
     ((a >> 40) & 0xff) << 16	|	\
     ((a >> 48) & 0xff) <<  8	|	\
     ((a >> 56) & 0xff))
# define MAKEFOURCC(ch0, ch1, ch2, ch3)									\
    ((DWORD)(BYTE)(ch0) << 24 | ((DWORD)(BYTE)(ch1) << 16) |	\
     ((DWORD)(BYTE)(ch2) << 8) | ((DWORD)(BYTE)(ch3)))
#else
# define make_endian_16(a)	a
# define make_endian_32(a)	a
# define make_endian_64(a)	a
# define MAKEFOURCC(ch0, ch1, ch2, ch3)									\
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |			\
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

#if defined(__POWERPC__)
# define swap4(d)\
    ((d&0x000000ff)<<24) |	\
    ((d&0x0000ff00)<<8)  |	\
    ((d&0x00ff0000)>>8)  |	\
    ((d&0xff000000)>>24)
#else
# define swap4(d) d
#endif

extern "C"
{
    double VP8_CalcSSIM
    (
        YV12_BUFFER_CONFIG *source,
        YV12_BUFFER_CONFIG *dest,
        int lumamask,
        double *weight
    );

    double VP8_CalcPSNR
    (
        YV12_BUFFER_CONFIG *source,
        YV12_BUFFER_CONFIG *dest,
        double *YPsnr,
        double *UPsnr,
        double *VPsnr,
        double *SqError
    );
    double VP8_Mse2Psnr
    (
        double Samples,
        double Peak,
        double Mse
    );

    unsigned int ON2_GetHighResTimerTick();
    unsigned int ON2_GetTimeInMicroSec(unsigned int startTick, unsigned int stopTick);
    extern int vp8_On2YV12_AllocFrameBuffer(YV12_BUFFER_CONFIG *ybf, int width, int height, int border);
    extern void vp8_ScaleMachineSpecificConfig(void);
    extern int vp8_On2YV12_DeAllocFrameBuffer(YV12_BUFFER_CONFIG *ybf);
    extern on2_codec_iface_t on2_enc_vp8_algo;

    extern void vp8_yv12_scale_or_center
    (
        YV12_BUFFER_CONFIG *src_yuv_config,
        YV12_BUFFER_CONFIG *dst_yuv_config,
        int expanded_frame_width,
        int expanded_frame_height,
        int scaling_mode,
        int HScale,
        int HRatio,
        int VScale,
        int VRatio
    );


}

#if CONFIG_MD5
#include "md5_utils.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////API 2.0//////////////////////////////////////////////////////
#ifdef _MSC_VER
#define USE_POSIX_MMAP 0
#else
#define USE_POSIX_MMAP 1
#endif
//////////////API 2.0//////////
#include "vp8cx.h"
#include "vpx_encoder.h"
#include "mem_ops.h"
#include "vpx_timer.h"
/////////////////////////////
#if USE_POSIX_MMAP
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif
/////////////API//////////
#include "vpx_integer.h"
//////////////////////////

struct vp8_extracfg
{
    struct on2_codec__pkt_list *pkt_list;
    on2_vp8e_encoding_mode      EncodingMode;               /** best, good, realtime            */
    int                         CpuUsed;                    /** available cpu percentage in 1/16*/
    unsigned int                EnableAutoAltRef;           /** if encoder decides to uses alternate reference frame */
    unsigned int                NoiseSensitivity;
    unsigned int                Sharpness;
    unsigned int                StaticThresh;
    unsigned int                TokenPartitions;

};
struct on2_codec_alg_priv
{
    on2_codec_priv_t        base;
    on2_codec_enc_cfg_t     cfg;
    struct vp8_extracfg     vp8_cfg;
    VP8_CONFIG              oxcf;
    VP8_PTR             cpi;
    unsigned char          *cx_data;
    unsigned int            cx_data_sz;
    on2_image_t             preview_img;
    unsigned int            next_frame_flag;
    vp8_postproc_cfg_t      preview_ppcfg;
    on2_codec__pkt_list_decl(26) pkt_list;              // changed to accomendate the maximum number of lagged frames allowed
    int                         deprecated_mode;
    unsigned int                fixed_kf_cntr;
};

extern "C" on2_codec_iface_t on2_codec_vp8_cx_algo;
static const char *exec_name;

static const struct codec_item
{
    char const              *name;
    const on2_codec_iface_t *iface;
    unsigned int             fourcc;
} codecs[] = {{"vp8",  &on2_codec_vp8_cx_algo, 0x30385056}};

static void usage_exit();

void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    printf("\n");
    usage_exit();
}

static void ctx_exit_on_error(on2_codec_ctx_t *ctx, const char *s)
{
    if (ctx->err)
    {
        const char *detail = on2_codec_error_detail(ctx);

        printf("%s: %s\n", s, on2_codec_error(ctx));

        if (detail)
            printf("    %s\n", detail);

        exit(EXIT_FAILURE);
    }
}

/* This structure is used to abstract the different ways of handling
* first pass statistics.
*/
typedef struct
{
    on2_fixed_buf_t buf;
    int             pass;
    FILE           *file;
    char           *buf_ptr;
    size_t          buf_alloc_sz;
} stats_io_t;
int stats_open_file(stats_io_t *stats, const char *fpf, int pass)
{
    int res;

    stats->pass = pass;

    if (pass == 0)
    {
        stats->file = fopen(fpf, "wb");
        stats->buf.sz = 0;
        stats->buf.buf = NULL,
        res = (stats->file != NULL);
    }
    else
    {
#if 0
#elif USE_POSIX_MMAP
        struct stat stat_buf;
        int fd;

        fd = open(fpf, O_RDONLY);
        stats->file = fdopen(fd, "rb");
        fstat(fd, &stat_buf);
        stats->buf.sz = stat_buf.st_size;
        stats->buf.buf = mmap(NULL, stats->buf.sz, PROT_READ, MAP_PRIVATE,
        fd, 0);
        res = (stats->buf.buf != NULL);
#else
        size_t nbytes;

        stats->file = fopen(fpf, "rb");

        if (fseek(stats->file, 0, SEEK_END))
        {
            fprintf(stderr, "First-pass stats file must be seekable!\n");
            exit(EXIT_FAILURE);
        }

        stats->buf.sz = stats->buf_alloc_sz = ftell(stats->file);
        rewind(stats->file);

        stats->buf.buf = malloc(stats->buf_alloc_sz);

        if (!stats->buf.buf)
        {
            fprintf(stderr, "Failed to allocate first-pass stats buffer (%d bytes)\n",
            stats->buf_alloc_sz);
            exit(EXIT_FAILURE);
        }

        nbytes = fread(stats->buf.buf, 1, stats->buf.sz, stats->file);
        res = (nbytes == stats->buf.sz);
#endif
    }

    return res;
}

int stats_open_mem(stats_io_t *stats, int pass)
{
    int res;
    stats->pass = pass;

    if (!pass)
    {
        stats->buf.sz = 0;
        stats->buf_alloc_sz = 64 * 1024;
        stats->buf.buf = malloc(stats->buf_alloc_sz);
    }

    stats->buf_ptr = (char *)stats->buf.buf;
    res = (stats->buf.buf != NULL);
    return res;
}


void stats_close(stats_io_t *stats)
{
    if (stats->file)
    {
        if (stats->pass == 1)
        {
#if 0
#elif USE_POSIX_MMAP
            munmap(stats->buf.buf, stats->buf.sz);
#else
            free(stats->buf.buf);
#endif
        }

        fclose(stats->file);
        stats->file = NULL;
    }
    else
    {
        if (stats->pass == 1)
            free(stats->buf.buf);
    }
}
void stats_write(stats_io_t *stats, const void *pkt, size_t len)
{
    if (stats->file)
    {
        fwrite(pkt, 1, len, stats->file);
    }
    else
    {
        if (stats->buf.sz + len > stats->buf_alloc_sz)
        {
            size_t  new_sz = stats->buf_alloc_sz + 64 * 1024;
            char   *new_ptr = (char *)realloc(stats->buf.buf, new_sz);

            if (new_ptr)
            {
                stats->buf_ptr = new_ptr + (stats->buf_ptr - (char *)stats->buf.buf);
                stats->buf.buf = new_ptr;
                stats->buf_alloc_sz = new_sz;
            } /* else ... */
        }

        memcpy(stats->buf_ptr, pkt, len);
        stats->buf.sz += len;
        stats->buf_ptr += len;
    }
}

on2_fixed_buf_t stats_get(stats_io_t *stats)
{
    return stats->buf;
}

static int read_frame(FILE *f, on2_image_t *img)
{
    size_t nbytes, to_read;
    int    res = 1;

    to_read = img->w * img->h * 3 / 2;
    nbytes = fread(img->planes[0], 1, to_read, f);

    if (nbytes != to_read)
    {
        res = 0;

        if (nbytes > 0)
            printf("Warning: Read partial frame. Check your width & height!\n");
    }

    return res;
}

static void write_ivf_file_header(FILE *outfile,
const on2_codec_enc_cfg_t *cfg,
unsigned int fourcc,
int frame_cnt)
{
    char header[32];

    if (cfg->g_pass != ON2_RC_ONE_PASS && cfg->g_pass != ON2_RC_LAST_PASS)
        return;

    header[0] = 'D';
    header[1] = 'K';
    header[2] = 'I';
    header[3] = 'F';
    mem_put_le16(header + 4,  0);                 /* version */
    mem_put_le16(header + 6,  32);                /* headersize */
    mem_put_le32(header + 8,  fourcc);            /* headersize */
    mem_put_le16(header + 12, cfg->g_w);          /* width */
    mem_put_le16(header + 14, cfg->g_h);          /* height */
    mem_put_le32(header + 16, cfg->g_timebase.den); /* rate */
    mem_put_le32(header + 20, cfg->g_timebase.num); /* scale */
    mem_put_le32(header + 24, frame_cnt);         /* length */
    mem_put_le32(header + 28, 0);                 /* unused */

    fwrite(header, 1, 32, outfile);
}


static void write_ivf_frame_header(FILE *outfile,
const on2_codec_cx_pkt_t *pkt)
{
    char             header[12];
    on2_codec_pts_t  pts;

    if (pkt->kind != ON2_CODEC_CX_FRAME_PKT)
        return;

    pts = pkt->data.frame.pts;
    mem_put_le32(header, pkt->data.frame.sz);
    mem_put_le32(header + 4, pts & 0xFFFFFFFF);
    mem_put_le32(header + 8, pts >> 32);

    fwrite(header, 1, 12, outfile);
}

#include "args.h"

static const arg_def_t use_yv12 = ARG_DEF(NULL, "yv12", 0,
"Input file is YV12 ");
static const arg_def_t use_i420 = ARG_DEF(NULL, "i420", 0,
"Input file is I420 (default)");
static const arg_def_t codecarg = ARG_DEF(NULL, "codec", 1,
"Codec to use");
static const arg_def_t passes           = ARG_DEF("p", "passes", 1,
"Number of passes (1/2)");
static const arg_def_t limit = ARG_DEF(NULL, "limit", 1,
"Stop encoding after n input frames");
static const arg_def_t deadline         = ARG_DEF("d", "deadline", 1,
"Deadline per frame (usec)");
static const arg_def_t best_dl          = ARG_DEF(NULL, "best", 0,
"Use Best Quality Deadline");
static const arg_def_t good_dl          = ARG_DEF(NULL, "good", 0,
"Use Good Quality Deadline");
static const arg_def_t rt_dl            = ARG_DEF(NULL, "rt", 0,
"Use Realtime Quality Deadline");
static const arg_def_t verbosearg       = ARG_DEF("v", "verbose", 0,
"Show encoder parameters");
static const arg_def_t *main_args[] =
{
    &codecarg, &passes, &limit, &deadline, &best_dl, &good_dl, &rt_dl,
    &verbosearg,
    NULL
};

static const arg_def_t usage            = ARG_DEF("u", "usage", 1,
        "Usage profile number to use");
static const arg_def_t threads          = ARG_DEF("t", "threads", 1,
        "Max number of threads to use");
static const arg_def_t profile          = ARG_DEF(NULL, "profile", 1,
        "Bitstream profile number to use");
static const arg_def_t width            = ARG_DEF("w", "width", 1,
        "Frame width");
static const arg_def_t height           = ARG_DEF("h", "height", 1,
        "Frame height");
static const arg_def_t timebase         = ARG_DEF(NULL, "timebase", 1,
        "Stream timebase (frame duration)");
static const arg_def_t error_resilient  = ARG_DEF(NULL, "error-resilient", 1,
        "Enable error resiliency features");
static const arg_def_t lag_in_frames    = ARG_DEF(NULL, "lag-in-frames", 1,
        "Max number of frames to lag");

static const arg_def_t *global_args[] =
{
    &use_yv12, &use_i420, &usage, &threads, &profile,
    &width, &height, &timebase, &error_resilient,
    &lag_in_frames, NULL
};

static const arg_def_t dropframe_thresh   = ARG_DEF(NULL, "drop-frame", 1,
        "Temporal resampling threshold (buf %)");
static const arg_def_t resize_allowed     = ARG_DEF(NULL, "resize-allowed", 1,
        "Spatial resampling enabled (bool)");
static const arg_def_t resize_up_thresh   = ARG_DEF(NULL, "resize-up", 1,
        "Upscale threshold (buf %)");
static const arg_def_t resize_down_thresh = ARG_DEF(NULL, "resize-down", 1,
        "Downscale threshold (buf %)");
static const arg_def_t end_usage          = ARG_DEF(NULL, "end-usage", 1,
        "CBR | VBR");
static const arg_def_t target_bitrate     = ARG_DEF(NULL, "target-bitrate", 1,
        "Bitrate (kbps)");
static const arg_def_t min_quantizer      = ARG_DEF(NULL, "min-q", 1,
        "Minimum (best) quantizer");
static const arg_def_t max_quantizer      = ARG_DEF(NULL, "max-q", 1,
        "Maximum (worst) quantizer");
static const arg_def_t undershoot_pct     = ARG_DEF(NULL, "undershoot-pct", 1,
        "Datarate undershoot (min) target (%)");
static const arg_def_t overshoot_pct      = ARG_DEF(NULL, "overshoot-pct", 1,
        "Datarate overshoot (max) target (%)");
static const arg_def_t buf_sz             = ARG_DEF(NULL, "buf-sz", 1,
        "Client buffer size (ms)");
static const arg_def_t buf_initial_sz     = ARG_DEF(NULL, "buf-initial-sz", 1,
        "Client initial buffer size (ms)");
static const arg_def_t buf_optimal_sz     = ARG_DEF(NULL, "buf-optimal-sz", 1,
        "Client optimal buffer size (ms)");
static const arg_def_t *rc_args[] =
{
    &dropframe_thresh, &resize_allowed, &resize_up_thresh, &resize_down_thresh,
    &end_usage, &target_bitrate, &min_quantizer, &max_quantizer,
    &undershoot_pct, &overshoot_pct, &buf_sz, &buf_initial_sz, &buf_optimal_sz,
    NULL
};


static const arg_def_t bias_pct = ARG_DEF(NULL, "bias-pct", 1,
                                  "CBR/VBR bias (0=CBR, 100=VBR)");
static const arg_def_t minsection_pct = ARG_DEF(NULL, "minsection-pct", 1,
                                        "GOP min bitrate (% of target)");
static const arg_def_t maxsection_pct = ARG_DEF(NULL, "maxsection-pct", 1,
                                        "GOP max bitrate (% of target)");
static const arg_def_t *rc_twopass_args[] =
{
    &bias_pct, &minsection_pct, &maxsection_pct, NULL
};


static const arg_def_t kf_min_dist = ARG_DEF(NULL, "kf-min-dist", 1,
                                     "Minimum keyframe interval (frames)");
static const arg_def_t kf_max_dist = ARG_DEF(NULL, "kf-max-dist", 1,
                                     "Maximum keyframe interval (frames)");
static const arg_def_t *kf_args[] =
{
    &kf_min_dist, &kf_max_dist, NULL
};



static const arg_def_t noise_sens = ARG_DEF(NULL, "noise-sensitivity", 1,
                                    "Noise sensitivity (frames to blur)");
static const arg_def_t sharpness = ARG_DEF(NULL, "sharpness", 1,
                                   "Filter sharpness (0-7)");
static const arg_def_t static_thresh = ARG_DEF(NULL, "static-thresh", 1,
                                       "Motion detection threshold");



static const arg_def_t cpu_used = ARG_DEF(NULL, "cpu-used", 1,
                                  "CPU Used (-15..0)");






static const arg_def_t auto_altref = ARG_DEF(NULL, "auto-alt-ref", 1,
                                     "Enable automatic alt reference frames");
static const arg_def_t *vp8_args[] =
{
    &cpu_used, &auto_altref, &noise_sens, &sharpness, &static_thresh, NULL
};


static const arg_def_t *no_args[] = { NULL };

static void usage_exit()
{
    int i;

    printf("Usage: %s <options> src_filename dst_filename\n", exec_name);

    printf("\nOptions:\n");
    printf("\nEncoder Global Options:\n");
    printf("\nRate Control Options:\n");
    printf("\nTwopass Rate Control Options:\n");
    printf("\nKeyframe Placement Options:\n");
    printf("\nVP8 Specific Options:\n");

    printf("\n"
           "Included encoders:\n"
           "\n");

    for (i = 0; i < sizeof(codecs) / sizeof(codecs[0]); i++)
        printf("    %-6s - %s\n",
               codecs[i].name,
               on2_codec_iface_name(codecs[i].iface));

    exit(EXIT_FAILURE);
}

#define ARG_CTRL_CNT_MAX 10

///////////////////////////////DEC///////////////////////////////
/* This is a simple program that reads "VP6 raw" files and decodes them
* using the new interface. Decoded frames are output as YV12 raw
* YV12 images.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define ON2_CODEC_DISABLE_COMPAT 1
#include "on2_codecs_config.h"
#include "vpx_decoder.h"
#include "vpx_timer.h"

#include "vp8dx.h"

void ivf_write_headerDec(FILE *outfile, unsigned int width, unsigned short height, unsigned int rate, unsigned int scale, unsigned int length)
{
    IVF_HEADER ivf;
    strncpy((char *)(ivf.signature), "DKIF", 4);
    ivf.version	= 0;
    ivf.headersize = make_endian_16(32);
    ivf.FourCC     = MAKEFOURCC('I', '4', '1', '0');
    ivf.width      = make_endian_16(width);
    ivf.height     = make_endian_16(height);
    ivf.scale      = make_endian_32(scale);
    ivf.rate       = make_endian_32(rate);
    ivf.length     = make_endian_32(length);
    fseek(outfile, 0, 0);
    fwrite((char *)&ivf, sizeof(ivf), 1, outfile);
}


void ivf_write_frameAPI(FILE *outfile, uint64_t timeStamp, uint32_t frameSize)
{
    IVF_FRAME_HEADER ivf_fh;

    ivf_fh.timeStamp = make_endian_64(timeStamp);
    ivf_fh.frameSize = make_endian_32(frameSize);

    fwrite((char *)&ivf_fh, sizeof(ivf_fh), 1, outfile);

}
static const struct
{
    char const *name;
    const on2_codec_iface_t *iface;
    unsigned int             fourcc;
    unsigned int             fourcc_mask;
} ifaces[] =
{
    {"vp8",  &on2_codec_vp8_dx_algo,   0x00385056, 0x00FFFFFF},
};

#include "args.h"
static const arg_def_t prefixarg = ARG_DEF("p", "prefix", 1,
                                   "Prefix to use when saving frames");
static const arg_def_t flipuvarg = ARG_DEF(NULL, "flipuv", 0,
                                   "Synonym for --yv12");
static const arg_def_t noblitarg = ARG_DEF(NULL, "noblit", 0,
                                   "Don't process the decoded frames");
static const arg_def_t progressarg = ARG_DEF(NULL, "progress", 0,
                                     "Show progress after each frame decodes");
static const arg_def_t limitarg = ARG_DEF(NULL, "limit", 1,
                                  "Stop decoding after n frames");
static const arg_def_t postprocarg = ARG_DEF(NULL, "postproc", 0,
                                     "Postprocess decoded frames");
static const arg_def_t summaryarg = ARG_DEF(NULL, "summary", 0,
                                    "Show timing summary");
static const arg_def_t outputfile = ARG_DEF("o", "output-raw-file", 1,
                                    "Output raw yv12 file instead of images");
static const arg_def_t threadsarg = ARG_DEF("t", "threads", 1,
                                    "Max threads to use");

static const arg_def_t *all_args[] =
{
    &codecarg, &prefixarg, &use_yv12, &use_i420, &flipuvarg, &noblitarg,
    &progressarg, &limitarg, &postprocarg, &summaryarg, &outputfile,
    &threadsarg,
    NULL
};

#define IVF_FRAME_HDR_SZ (sizeof(uint32_t) + sizeof(uint64_t))
#define RAW_FRAME_HDR_SZ (sizeof(uint32_t))
static int read_frame(FILE *infile, uint8_t **buf, uint32_t *buf_sz, uint32_t *buf_alloc_sz, int is_ivf)
{
    char     raw_hdr[IVF_FRAME_HDR_SZ];
    uint32_t new_buf_sz;

    /* For both the raw and ivf formats, the frame size is the first 4 bytes
    * of the frame header. We just need to special case on the header
    * size.
    */
    if (fread(raw_hdr, is_ivf ? IVF_FRAME_HDR_SZ : RAW_FRAME_HDR_SZ, 1, infile) != 1)
    {
        if (!feof(infile))
            printf("Failed to read frame size\n");

        new_buf_sz = 0;
    }
    else
    {
        new_buf_sz = mem_get_le32(raw_hdr);

        if (new_buf_sz > 256 * 1024)
        {
            printf("Read invalid frame size - not a raw file?\n");
            new_buf_sz = 0;
        }
        else if (new_buf_sz > *buf_alloc_sz)
        {
            uint8_t *new_buf = (uint8_t *)realloc(*buf, 2 * new_buf_sz);

            if (new_buf)
            {
                *buf = new_buf;
                *buf_alloc_sz = 2 * new_buf_sz;
            }
            else
            {
                printf("Failed to allocate compressed data buffer\n");
                new_buf_sz = 0;
            }
        }
    }

    *buf_sz = new_buf_sz;

    if (*buf_sz)
    {
        if (fread(*buf, 1, *buf_sz, infile) != *buf_sz)
        {
            printf("Failed to read full frame\n");
            return 1;
        }

        return 0;
    }

    return 1;
}

void *out_open(const char *out_fn, int do_md5)
{
    void *out = NULL;

    if (do_md5)
    {
#if CONFIG_MD5
        out = malloc(sizeof(MD5Context));
        MD5Context *md5_ctx = (MD5Context *) out;
        (void)out_fn;
        MD5Init(md5_ctx);
#endif
    }
    else
    {
        out = fopen(out_fn, "wb");
        FILE *outfile = (FILE *)out;

        if (!outfile)
        {
            printf("Failed to open output file: %s", out_fn);
            fprintf(stderr, "Failed to open output file: %s", out_fn);
            exit(EXIT_FAILURE);
        }
    }

    return out;
}

void out_put(void *out, const uint8_t *buf, unsigned int len, int do_md5)
{
    if (do_md5)
    {
#if CONFIG_MD5
        MD5Update((MD5Context *) out, buf, len);
#endif
    }
    else
    {
        fwrite(buf, 1, len, (FILE *)out);
    }
}
void out_close(void *out, const char *out_fn, int do_md5)
{
    if (do_md5)
    {
#if CONFIG_MD5
        uint8_t md5[16];
        int i;

        MD5Final((unsigned char *) md5, (MD5Context *) out);
        free(out);

        printf("\n");
        //fprintf(stderr, "\n");

        for (i = 0; i < 16; i++)
        {
            printf("%02x", md5[i]);
            fprintf(stderr, "%02x", md5[i]);
        }

        printf("\n");
        fprintf(stderr, "\n");

        FILE *outfile;
        outfile = fopen(out_fn, "w");

        for (i = 0; i < 16; i++)
        {
            fprintf(outfile, "%02x", md5[i]);
        }

        fclose(outfile);
#endif
    }
    else
    {
        fclose((FILE *)out);
    }
}

unsigned int file_is_ivf(FILE *infile, unsigned int *fourcc)
{
    char raw_hdr[32];
    int is_ivf = 0;

    if (fread(raw_hdr, 1, 32, infile) == 32)
    {
        if (raw_hdr[0] == 'D' && raw_hdr[1] == 'K'
            && raw_hdr[2] == 'I' && raw_hdr[3] == 'F')
        {
            is_ivf = 1;

            if (mem_get_le16(raw_hdr + 4) != 0)
                printf("Error: Unrecognized IVF version! This file may not"
                       " decode properly.");

            *fourcc = mem_get_le32(raw_hdr + 8);
        }
    }

    if (!is_ivf)
        rewind(infile);

    return is_ivf;
}
unsigned int file_is_ivf_IVF(FILE *infile, unsigned int *fourcc, FILE *out, unsigned short *width, unsigned short *height)
{
    char raw_hdr[32];
    int is_ivf = 0;

    if (fread(raw_hdr, 1, 32, infile) == 32)
    {
        if (raw_hdr[0] == 'D' && raw_hdr[1] == 'K'
            && raw_hdr[2] == 'I' && raw_hdr[3] == 'F')
        {
            is_ivf = 1;
            ///////////////////Write Header Info///////////////////
            IVF_HEADER ivfhRaw;
            memcpy(&ivfhRaw, raw_hdr, 32);
            ivfhRaw.FourCC = 808596553;
            *width = ivfhRaw.width;
            *height = ivfhRaw.height;
            ivf_write_headerDec(out, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate, ivfhRaw.scale, ivfhRaw.length);

            ///////////////////////////////////////////////////////
            if (mem_get_le16(raw_hdr + 4) != 0)
                printf("Error: Unrecognized IVF version! This file may not"
                       " decode properly.");

            *fourcc = mem_get_le32(raw_hdr + 8);
        }
    }

    if (!is_ivf)
        rewind(infile);

    return is_ivf;
}

//-----------------------------------------------------VP8 Settings-------------------------------------------------------------------
void VP8DefaultParms(VP8_CONFIG &opt)
{
    /////////Orig Tester Parms//////////
    opt.AllowLag = 1;
    opt.AltFreq = 16;
    opt.AltQ = 20;
    opt.CpuUsed = 0;
    opt.EncodeBreakout = 0;
    opt.GoldQ = 28;
    opt.KeyQ = 12;
    opt.PlayAlternate = 1;
    opt.WorstAllowedQ = 56;
    opt.LagInFrames = 10;
    //////////////////////////////////
    //////////////IVFEnc Parms////////////
    //opt.AllowLag = 0;
    //opt.AltFreq = 0;
    //opt.AltQ = 0;
    //opt.CpuUsed = -4;
    //opt.EncodeBreakout = 800;
    //opt.GoldQ = 0;
    //opt.KeyQ = 0;
    //opt.PlayAlternate = 0;
    //opt.WorstAllowedQ = 63;
    //opt.LagInFrames = 0;
    //////////////////////////////////////

    opt.Width = 0;
    opt.Height = 0;
    //included in default settings file
    opt.AllowDF = 0;
    //opt.AllowLag = 1;
    opt.AllowSpatialResampling = 0;
    //opt.AltFreq = 16;
    //opt.AltQ = 20;
    opt.AutoKey = 1;
    opt.BestAllowedQ = 4;
    //opt.CpuUsed = 0;
    opt.DropFramesWaterMark = 70;
    //opt.EncodeBreakout = 0; //this may need to be set to 800 defaultly check vp8_cx_iface.c LN 46
    opt.EndUsage = 1;
    opt.FixedQ = -1;
    //opt.GoldQ = 28;
    opt.KeyFreq = 999999;
    //opt.KeyQ = 12;
    opt.MaximumBufferSize = 6;
    opt.Mode = 2;
    opt.NoiseSensitivity = 0;
    opt.OptimalBufferLevel = 5;
    //opt.PlayAlternate = 1; //this may be being used incorrectly for EnableAutoAltRef for some reason
    opt.ResampleDownWaterMark = 30;
    opt.ResampleUpWaterMark = 60;
    opt.Sharpness = 0;
    opt.StartingBufferLevel = 4;
    opt.TargetBandwidth = 40;
    opt.TwoPassVBRBias = 50;
    opt.TwoPassVBRMaxSection = 400;
    opt.TwoPassVBRMinSection = 0;
    opt.UnderShootPct = 95;
    opt.Version = 0;
    //opt.WorstAllowedQ = 56;

    //not included in default settings file
    opt.Height = 0;
    opt.MultiThreaded = 0;
    opt.Width = 0;

    opt.TokenPartitions = 0;
    opt.ErrorResilientMode = 0;


}
int OutPutSettingsAPI(char *outputFile, on2_codec_enc_cfg_t cfg)
{
    //Saves all on2_codec_enc_cfg_t settings to a settings file

    ofstream outfile(outputFile);

    outfile << cfg.g_usage << "\tg_usage\n";
    outfile << cfg.g_threads << "\tg_threads\n";
    outfile << cfg.g_profile << "\tg_profile\n";
    outfile << cfg.g_w << "\tg_w\n";
    outfile << cfg.g_h << "\tg_h\n";
    outfile << cfg.g_timebase.num << "\tcfg.g_timebase.num\n";
    outfile << cfg.g_timebase.den << "\tcfg.g_timebase.den\n";
    outfile << cfg.g_error_resilient << "\tg_error_resilient\n";
    outfile << cfg.g_pass << "\tg_pass\n";
    outfile << cfg.g_lag_in_frames << "\tg_lag_in_frames\n";
    outfile << cfg.rc_dropframe_thresh << "\trc_dropframe_thresh\n";
    outfile << cfg.rc_resize_allowed  << "\trc_resize_allowed\n";
    outfile << cfg.rc_resize_up_thresh << "\trc_resize_up_thresh\n";
    outfile << cfg.rc_resize_down_thresh << "\trc_resize_down_thresh\n";
    outfile << cfg.rc_end_usage << "\trc_end_usage\n";
    outfile << cfg.rc_target_bitrate << "\trc_target_bitrate\n";
    outfile << cfg.rc_min_quantizer << "\trc_min_quantizer\n";
    outfile << cfg.rc_max_quantizer << "\trc_max_quantizer\n";
    outfile << cfg.rc_undershoot_pct  << "\trc_undershoot_pct\n";
    outfile << cfg.rc_overshoot_pct  << "\trc_overshoot_pct\n";
    outfile << cfg.rc_buf_sz << "\trc_buf_sz\n";
    outfile << cfg.rc_buf_initial_sz  << "\trc_buf_initial_sz \n";
    outfile << cfg.rc_buf_optimal_sz << "\trc_buf_optimal_sz\n";
    outfile << cfg.rc_2pass_vbr_bias_pct << "\trc_2pass_vbr_bias_pct\n";
    outfile << cfg.rc_2pass_vbr_minsection_pct << "\trc_2pass_vbr_minsection_pct\n";
    outfile << cfg.rc_2pass_vbr_maxsection_pct << "\trc_2pass_vbr_maxsection_pct\n";
    outfile << cfg.kf_mode << "\tkf_mode\n";
    outfile << cfg.kf_min_dist << "\tkf_min_dist\n";
    outfile << cfg.kf_max_dist << "\tkf_max_dist\n";

    outfile.close();
    return 0;
}

int VP8CoreConfigToAPIcfg(VP8_CONFIG coreCfg, on2_codec_enc_cfg_t *cfg)
{
    //Converts a core configuration to api configuration

    cfg->g_threads = coreCfg.MultiThreaded;
    cfg->g_profile = coreCfg.Version;
    cfg->g_error_resilient = coreCfg.ErrorResilientMode;
    cfg->rc_resize_allowed = coreCfg.AllowSpatialResampling;
    cfg->rc_resize_up_thresh = coreCfg.ResampleUpWaterMark;
    cfg->rc_resize_down_thresh = coreCfg.ResampleDownWaterMark;
    cfg->rc_target_bitrate = coreCfg.TargetBandwidth;
    cfg->rc_min_quantizer = coreCfg.BestAllowedQ;
    cfg->rc_max_quantizer = coreCfg.WorstAllowedQ;
    cfg->rc_undershoot_pct = coreCfg.UnderShootPct;
    cfg->rc_buf_sz = coreCfg.MaximumBufferSize * 1000;
    cfg->rc_buf_initial_sz	= coreCfg.StartingBufferLevel * 1000;
    cfg->rc_buf_optimal_sz	= coreCfg.OptimalBufferLevel * 1000;
    cfg->rc_2pass_vbr_bias_pct		= coreCfg.TwoPassVBRBias;
    cfg->rc_2pass_vbr_minsection_pct	= coreCfg.TwoPassVBRMinSection;
    cfg->rc_2pass_vbr_maxsection_pct  = coreCfg.TwoPassVBRMaxSection;

    if (coreCfg.AutoKey == 0)
    {
        cfg->kf_mode				= ON2_KF_FIXED;
    }

    if (coreCfg.AutoKey == 1)
    {
        cfg->kf_mode				= ON2_KF_AUTO;
    }

    cfg->kf_max_dist				= coreCfg.KeyFreq;

    if (coreCfg.FixedQ != -1)
    {
        if (coreCfg.FixedQ > 63)
        {
            coreCfg.FixedQ = 63;
        }

        if (coreCfg.FixedQ < 0)
        {
            coreCfg.FixedQ = 0;
        }

        cfg->rc_min_quantizer = coreCfg.FixedQ;
        cfg->rc_max_quantizer = coreCfg.FixedQ;
    }

    if (coreCfg.AllowLag == 0)
    {
        cfg->g_lag_in_frames = 0;
    }
    else
    {
        cfg->g_lag_in_frames = coreCfg.LagInFrames;
    }

    if (coreCfg.AllowDF == 0)
    {
        cfg->rc_dropframe_thresh = 0;
    }
    else
    {
        cfg->rc_dropframe_thresh = coreCfg.DropFramesWaterMark;
    }

    if (coreCfg.EndUsage == USAGE_LOCAL_FILE_PLAYBACK)
    {
        cfg->rc_end_usage = ON2_VBR;
    }
    else if (coreCfg.EndUsage == USAGE_STREAM_FROM_SERVER)
    {
        cfg->rc_end_usage = ON2_CBR;
    }

    return 0;
}
VP8_CONFIG VP8RandomParms(VP8_CONFIG &opt, char *inputfile, int display)
{
    //Ranges can be found in validate_config in vp8_cx_iface.c

    //////////////////////////////////Randomly Generated\\\\\\\\\\\\\\\\\\\\\

    srand(ON2_GetHighResTimerTick());
    int w = 0;
    int h = 0;
    int fr  = 0;
    int length = 0;


    FILE *GetWHinfile = fopen(inputfile, "rb");

    if (GetWHinfile == NULL)
    {
        w = rand() % 1920;
        h = rand() % 1080;
        length = rand() % 100 + rand() % 5;
    }
    else
    {
        IVF_HEADER ivfhRaw;
        IVF_FRAME_HEADER ivf_fhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
        FormatIVFHeaderRead(&ivfhRaw);

        w		= ivfhRaw.width;
        h		= ivfhRaw.height;
        fr		= (ivfhRaw.rate / ivfhRaw.scale);
        length  = ivfhRaw.length;

        fclose(GetWHinfile);
    }

    opt.Mode = rand() % 5;              //valid Range: (0 to 4)

    if (opt.Mode == 0)
        opt.NoiseSensitivity = 0;           //valid Range:
    else	                            //if Not Real Time Mode 0 to 6
        opt.NoiseSensitivity = rand() % 7;  //if Real Time Mode 0 to 0

    if (opt.Mode == 0)
        opt.LagInFrames = 0;                //valid Range:
    else                                //if Not Real Time Mode 0 to 25
        opt.LagInFrames = rand() % 26;      //if Real Time Mode 0 to 0

    if (opt.LagInFrames > 0)
        opt.AllowLag = 1;                   //valid Range:
    else                                //if Not Real Time Mode 0 to 1
        opt.AllowLag = 0;                   //if Real Time Mode 0

    if (opt.Mode == 0)
    {
        opt.CpuUsed = rand() % 13 + 4;  //valid Range:

        if (rand() % 2)			      //if Not Real Time Mode -16 to 16
            opt.CpuUsed = opt.CpuUsed * -1; //if Real Time Mode -16 to -4 or 4 to 16
    }
    else
    {
        opt.CpuUsed = rand() % 17;

        if (rand() % 2)
            opt.CpuUsed = opt.CpuUsed * -1;
    }

    if (rand() % 21 == 20) //1 out of 20 chance of a fixed q
    {
        opt.FixedQ = rand() % 64; //valid Range: 0 to 63 or -1 (-1 = fixedQ off)
        opt.BestAllowedQ = opt.FixedQ; //valid Range: 0 to 63
        opt.WorstAllowedQ   = opt.FixedQ; //valid Range: 0 to 63
    }
    else
    {
        opt.FixedQ = -1; //valid Range: 0 to 63 or -1 (-1 = fixedQ off)
        opt.BestAllowedQ = rand() % 64; //valid Range: 0 to 63
        opt.WorstAllowedQ   = rand() % 64; //valid Range: 0 to 63
    }

    opt.AutoKey = rand() % 2; //valid Range: 0 to 1
    opt.MultiThreaded =  rand() % 65; //valid Range: 0 to 64
    opt.UnderShootPct = rand() % 101; //valid Range: 0 to 100
    opt.AllowDF = rand() % 2; //valid Range: 0 to 1

    if (opt.AllowDF > 0)
        opt.DropFramesWaterMark = rand() % 101; //valid Range: 0 to 100
    else
        opt.DropFramesWaterMark = 0;

    opt.AllowSpatialResampling = rand() % 2; //valid Range: 0 to 1
    opt.ResampleUpWaterMark = rand() % 101; //valid Range: 0 to 100
    opt.ResampleDownWaterMark = rand() % 101; //valid Range:  0 to 100
    opt.TwoPassVBRBias = rand() % 101; //valid Range: 0 to 100
    opt.EncodeBreakout = rand() % 101; //valid Range:
    opt.EndUsage = rand() % 2; //valid Range: 0 to 1
    opt.Version = rand() % 4; //valid Range: 0 to 3
    opt.Sharpness = rand() % 8; //valid Range: 0 to 7
    opt.PlayAlternate = rand() % 2; //valid Range: 0 to 1
    opt.TokenPartitions = rand() % 4; //valid Range: 0 to 3
    opt.ErrorResilientMode = rand() % 101; //valid Range: 0-100

    opt.TargetBandwidth = rand() % ((w * h) / (320 * 240) * 2048); //valid Range: No Max so based on resolution
    opt.KeyFreq = rand() % length + 2; //valid Range: No Max so based on number of frames

    ////////////////////////////////No Range Documentation////////////////////////////////
    //opt.MaximumBufferSize  = rand()% 101; //valid Range:

    //randnum = rand()% 101;
    //randomCalc =opt.MaximumBufferSize * (randnum/100.0);
    //opt.StartingBufferLevel = randomCalc;

    //randnum = rand()% 101; //valid Range:
    //randomCalc = 15*(randnum/100.0)+opt.MaximumBufferSize;
    //opt.OptimalBufferLevel  = randomCalc;

    //opt.TwoPassVBRMinSection = rand()% 101; //valid Range:
    //opt.TwoPassVBRMaxSection = rand()% 101; //valid Range:
    /////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////Cannot be accessed through API////////////////////////////
    //opt.AltFreq = rand()% 101; //valid Range:
    //opt.AltQ = rand()% 101; //valid Range:
    //opt.GoldQ = rand()% 101; //valid Range:
    //opt.KeyQ = rand()% 101; //valid Range:
    //opt.FrameRate = rand()% 101; //valid Range:
    //////////////////////////////////////////////////////////////////////////////////////

    opt.MaximumBufferSize = 6;
    opt.StartingBufferLevel = 4;
    opt.OptimalBufferLevel = 5;
    opt.TwoPassVBRMinSection = 400;
    opt.TwoPassVBRMaxSection = 0;

    opt.AltFreq = 16;
    opt.AltQ = 20;
    opt.GoldQ = 28;
    opt.KeyQ = 12;

    printf("\nRandom Parameters Generated:");
    cout << "\n\n";
    cout << "TargetBandwidth " << opt.TargetBandwidth << "\n";
    cout << "NoiseSensitivity " << opt.NoiseSensitivity << "\n";
    cout << "Sharpness " << opt.Sharpness << "\n";
    cout << "CpuUsed " << opt.CpuUsed << "\n";
    cout << "Mode " << opt.Mode << "\n";
    cout << "AutoKey " << opt.AutoKey << "\n";
    cout << "KeyFreq " << opt.KeyFreq << "\n";
    cout << "EndUsage " << opt.EndUsage << "\n";
    cout << "UnderShootPct " << opt.UnderShootPct << "\n";
    cout << "StartingBufferLevel " << opt.StartingBufferLevel << "\n";
    cout << "OptimalBufferLevel " << opt.OptimalBufferLevel << "\n";
    cout << "MaximumBufferSize " << opt.MaximumBufferSize << "\n";
    cout << "FixedQ " << opt.FixedQ << "\n";
    cout << "WorstAllowedQ " << opt.WorstAllowedQ  << "\n";
    cout << "BestAllowedQ " << opt.BestAllowedQ << "\n";
    cout << "AllowSpatialResampling " << opt.AllowSpatialResampling << "\n";
    cout << "ResampleDownWaterMark " << opt.ResampleDownWaterMark << "\n";
    cout << "ResampleUpWaterMark " << opt.ResampleUpWaterMark << "\n";
    cout << "AllowDF " << opt.AllowDF << "\n";
    cout << "DropFramesWaterMark " << opt.DropFramesWaterMark << "\n";
    cout << "AllowLag " << opt.AllowLag << "\n";
    cout << "PlayAlternate " << opt.PlayAlternate << "\n";
    cout << "AltQ " << opt.AltQ << "\n";
    cout << "AltFreq " << opt.AltFreq << "\n";
    cout << "GoldQ " << opt.GoldQ << "\n";
    cout << "KeyQ " << opt.KeyQ << "\n";
    cout << "Version " << opt.Version << "\n";
    cout << "LagInFrames " << opt.LagInFrames << "\n";
    cout << "TwoPassVBRBias " << opt.TwoPassVBRBias << "\n";
    cout << "TwoPassVBRMinSection " << opt.TwoPassVBRMinSection << "\n";
    cout << "TwoPassVBRMaxSection " << opt.TwoPassVBRMaxSection << "\n";
    cout << "EncodeBreakout " << opt.EncodeBreakout << "\n";
    cout << "TokenPartitions " << opt.TokenPartitions << "\n";
    cout << "MultiThreaded " << opt.MultiThreaded << "\n";
    cout << "ErrorResilientMode " << opt.ErrorResilientMode << "\n";

    if (display == 1)
    {
        fprintf(stderr, "\nRandom Parameters Generated:");
        cerr << "\n\n";
        cerr << "TargetBandwidth " << opt.TargetBandwidth << "\n";
        cerr << "NoiseSensitivity " << opt.NoiseSensitivity << "\n";
        cerr << "Sharpness " << opt.Sharpness << "\n";
        cerr << "CpuUsed " << opt.CpuUsed << "\n";
        cerr << "Mode " << opt.Mode << "\n";
        cerr << "AutoKey " << opt.AutoKey << "\n";
        cerr << "KeyFreq " << opt.KeyFreq << "\n";
        cerr << "EndUsage " << opt.EndUsage << "\n";
        cerr << "UnderShootPct " << opt.UnderShootPct << "\n";
        cerr << "StartingBufferLevel " << opt.StartingBufferLevel << "\n";
        cerr << "OptimalBufferLevel " << opt.OptimalBufferLevel << "\n";
        cerr << "MaximumBufferSize " << opt.MaximumBufferSize << "\n";
        cerr << "FixedQ " << opt.FixedQ << "\n";
        cerr << "WorstAllowedQ " << opt.WorstAllowedQ  << "\n";
        cerr << "BestAllowedQ " << opt.BestAllowedQ << "\n";
        cerr << "AllowSpatialResampling " << opt.AllowSpatialResampling << "\n";
        cerr << "ResampleDownWaterMark " << opt.ResampleDownWaterMark << "\n";
        cerr << "ResampleUpWaterMark " << opt.ResampleUpWaterMark << "\n";
        cerr << "AllowDF " << opt.AllowDF << "\n";
        cerr << "DropFramesWaterMark " << opt.DropFramesWaterMark << "\n";
        cerr << "AllowLag " << opt.AllowLag << "\n";
        cerr << "PlayAlternate " << opt.PlayAlternate << "\n";
        cerr << "AltQ " << opt.AltQ << "\n";
        cerr << "AltFreq " << opt.AltFreq << "\n";
        cerr << "GoldQ " << opt.GoldQ << "\n";
        cerr << "KeyQ " << opt.KeyQ << "\n";
        cerr << "Version " << opt.Version << "\n";
        cerr << "LagInFrames " << opt.LagInFrames << "\n";
        cerr << "TwoPassVBRBias " << opt.TwoPassVBRBias << "\n";
        cerr << "TwoPassVBRMinSection " << opt.TwoPassVBRMinSection << "\n";
        cerr << "TwoPassVBRMaxSection " << opt.TwoPassVBRMaxSection << "\n";
        cerr << "EncodeBreakout " << opt.EncodeBreakout << "\n";
        cerr << "TokenPartitions " << opt.TokenPartitions << "\n";
        cerr << "MultiThreaded " << opt.MultiThreaded << "\n";
        cerr << "ErrorResilientMode " << opt.ErrorResilientMode << "\n";
    }

    //cout << "FrameRate " << opt.FrameRate << "\n";
    //cerr << "FrameRate " << opt.FrameRate << "\n";

    return opt;
}
VP8_CONFIG InPutSettings(char *inputFile)
{
    // Reads an input file and sets VP8_CONFIG accordingly
    ifstream infile2(inputFile);

    char firstPFile[256];
    string Garbage;

    VP8_CONFIG opt;

    infile2 >> opt.TargetBandwidth;
    infile2 >> Garbage;
    infile2 >> opt.NoiseSensitivity;
    infile2 >> Garbage;
    infile2 >> opt.Sharpness;
    infile2 >> Garbage;
    infile2 >> opt.CpuUsed;
    infile2 >> Garbage;
    infile2 >> opt.Mode;
    infile2 >> Garbage;
    infile2 >> opt.AutoKey;
    infile2 >> Garbage;
    infile2 >> opt.KeyFreq;
    infile2 >> Garbage;
    infile2 >> opt.EndUsage;
    infile2 >> Garbage;
    infile2 >> opt.UnderShootPct;
    infile2 >> Garbage;
    infile2 >> opt.StartingBufferLevel;
    infile2 >> Garbage;
    infile2 >> opt.OptimalBufferLevel;
    infile2 >> Garbage;
    infile2 >> opt.MaximumBufferSize;
    infile2 >> Garbage;
    infile2 >> opt.FixedQ;
    infile2 >> Garbage;
    infile2 >> opt.WorstAllowedQ;
    infile2 >> Garbage;
    infile2 >> opt.BestAllowedQ;
    infile2 >> Garbage;
    infile2 >> opt.AllowSpatialResampling;
    infile2 >> Garbage;
    infile2 >> opt.ResampleDownWaterMark;
    infile2 >> Garbage;
    infile2 >> opt.ResampleUpWaterMark;
    infile2 >> Garbage;
    infile2 >> opt.AllowDF;
    infile2 >> Garbage;
    infile2 >> opt.DropFramesWaterMark;
    infile2 >> Garbage;
    infile2 >> opt.AllowLag;
    infile2 >> Garbage;
    infile2 >> opt.PlayAlternate;
    infile2 >> Garbage;
    infile2 >> opt.AltQ;
    infile2 >> Garbage;
    infile2 >> opt.AltFreq;
    infile2 >> Garbage;
    infile2 >> opt.GoldQ;
    infile2 >> Garbage;
    infile2 >> opt.KeyQ;
    infile2 >> Garbage;
    infile2 >> opt.Version;
    infile2 >> Garbage;
    infile2 >> opt.LagInFrames;
    infile2 >> Garbage;
    infile2 >> opt.TwoPassVBRBias;
    infile2 >> Garbage;
    infile2 >> opt.TwoPassVBRMinSection;
    infile2 >> Garbage;
    infile2 >> opt.TwoPassVBRMaxSection;
    infile2 >> Garbage;
    infile2 >> opt.EncodeBreakout;
    infile2 >> Garbage;
    infile2 >> opt.TokenPartitions ;
    infile2 >> Garbage;
    infile2 >> opt.MultiThreaded;
    infile2 >> Garbage;
    infile2 >> opt.ErrorResilientMode;
    infile2 >> Garbage;

    //not included in default settings file
    infile2 >> firstPFile;
    infile2 >> Garbage;
    infile2 >> opt.Height;
    infile2 >> Garbage;
    infile2 >> opt.Width;
    infile2 >> Garbage;

    infile2.close();

    return opt;
}
int OutPutSettings(char *outputFile, VP8_CONFIG opt)
{
    //Saves all VP8_CONFIG settings to a settings file readable by InputSettings

    ofstream outfile(outputFile);

    outfile <<  opt.TargetBandwidth << " TargetBandwidth\n";
    outfile <<  opt.NoiseSensitivity << " NoiseSensitivity\n";
    outfile <<  opt.Sharpness << " Sharpness\n";
    outfile <<  opt.CpuUsed << " CpuUsed\n";
    outfile <<  opt.Mode << " Mode\n";
    outfile <<  opt.AutoKey << " AutoKey\n";
    outfile <<  opt.KeyFreq << " KeyFreq\n";
    outfile <<  opt.EndUsage << " EndUsage\n";
    outfile <<  opt.UnderShootPct << " UnderShootPct\n";
    outfile <<  opt.StartingBufferLevel << " StartingBufferLevel\n";
    outfile <<  opt.OptimalBufferLevel << " OptimalBufferLevel\n";
    outfile <<  opt.MaximumBufferSize << " MaximumBufferSize\n";
    outfile <<  opt.FixedQ << " FixedQ\n";
    outfile <<  opt.WorstAllowedQ << " WorstAllowedQ\n";
    outfile <<  opt.BestAllowedQ << " BestAllowedQ\n";
    outfile <<  opt.AllowSpatialResampling << " AllowSpatialResampling\n";
    outfile <<  opt.ResampleDownWaterMark << " ResampleDownWaterMark\n";
    outfile <<  opt.ResampleUpWaterMark << " ResampleUpWaterMark\n";
    outfile <<  opt.AllowDF << " AllowDF\n";
    outfile <<  opt.DropFramesWaterMark << " DropFramesWaterMark\n";
    outfile <<  opt.AllowLag << " AllowLag\n";
    outfile <<  opt.PlayAlternate << " PlayAlternate\n";
    outfile <<  opt.AltQ << " AltQ\n";
    outfile <<  opt.AltFreq << " AltFreq\n";
    outfile <<  opt.GoldQ << " GoldQ\n";
    outfile <<  opt.KeyQ << " KeyQ\n";
    outfile <<  opt.Version << " Version\n";
    outfile <<  opt.LagInFrames << " LagInFrames\n";
    outfile <<  opt.TwoPassVBRBias << " TwoPassVBRBias\n";
    outfile <<  opt.TwoPassVBRMinSection << " TwoPassVBRMinSection\n";
    outfile <<  opt.TwoPassVBRMaxSection << " TwoPassVBRMaxSection\n";
    outfile <<  opt.EncodeBreakout << " EncodeBreakout\n";
    outfile <<  opt.TokenPartitions  << " TokenPartitions\n";
    outfile <<  opt.MultiThreaded << " MultiThreaded\n";
    outfile <<  opt.ErrorResilientMode << " ErrorResilientMode\n";

    //not included in default settings file
    outfile << opt.Height << " Height\n";
    outfile << opt.Width << " Width\n";

    outfile.close();
    return 0;
}
int OutPutCompatSettings(char *outputFile, VP8_CONFIG opt, int ParVersionNum)
{
    //Saves all VP8_CONFIG settings to a settings file readable by InputSettings
    //Tester Uses prebuilt executables from prior builds

    ofstream outfile(outputFile);

    outfile <<  opt.TargetBandwidth << " TargetBandwidth\n";
    outfile <<  opt.NoiseSensitivity << " NoiseSensitivity\n";
    outfile <<  opt.Sharpness << " Sharpness\n";
    outfile <<  opt.CpuUsed << " CpuUsed\n";
    outfile <<  opt.Mode << " Mode\n";

    if (ParVersionNum == 1)
    {
        printf("\nRunning in 1.0.4 and earlier Parameter Compatability Mode\n");
        fprintf(stderr, "\nRunning in 1.0.4 and earlier Parameter Compatability Mode\n");
        outfile <<  1 << " DeleteFirstPassFile\n";
    }

    outfile <<  opt.AutoKey << " AutoKey\n";
    outfile <<  opt.KeyFreq << " KeyFreq\n";
    outfile <<  opt.EndUsage << " EndUsage\n";
    outfile <<  opt.UnderShootPct << " UnderShootPct\n";
    outfile <<  opt.StartingBufferLevel << " StartingBufferLevel\n";
    outfile <<  opt.OptimalBufferLevel << " OptimalBufferLevel\n";
    outfile <<  opt.MaximumBufferSize << " MaximumBufferSize\n";
    outfile <<  opt.FixedQ << " FixedQ\n";
    outfile <<  opt.WorstAllowedQ << " WorstAllowedQ\n";
    outfile <<  opt.BestAllowedQ << " BestAllowedQ\n";
    outfile <<  opt.AllowSpatialResampling << " AllowSpatialResampling\n";
    outfile <<  opt.ResampleDownWaterMark << " ResampleDownWaterMark\n";
    outfile <<  opt.ResampleUpWaterMark << " ResampleUpWaterMark\n";
    outfile <<  opt.AllowDF << " AllowDF\n";
    outfile <<  opt.DropFramesWaterMark << " DropFramesWaterMark\n";
    outfile <<  opt.AllowLag << " AllowLag\n";
    outfile <<  opt.PlayAlternate << " PlayAlternate\n";
    outfile <<  opt.AltQ << " AltQ\n";
    outfile <<  opt.AltFreq << " AltFreq\n";
    outfile <<  opt.GoldQ << " GoldQ\n";
    outfile <<  opt.KeyQ << " KeyQ\n";
    outfile <<  opt.Version << " Version\n";
    outfile <<  opt.LagInFrames << " LagInFrames\n";
    outfile <<  opt.TwoPassVBRBias << " TwoPassVBRBias\n";
    outfile <<  opt.TwoPassVBRMinSection << " TwoPassVBRMinSection\n";
    outfile <<  opt.TwoPassVBRMaxSection << " TwoPassVBRMaxSection\n";
    outfile <<  opt.EncodeBreakout << " EncodeBreakout\n";
    outfile <<  opt.TokenPartitions  << " TokenPartitions\n";
    outfile <<  opt.MultiThreaded << " MultiThreaded\n";
    outfile <<  opt.ErrorResilientMode << " ErrorResilientMode\n";

    //not included in default settings file
    if (ParVersionNum == 1)
    {
        string DummyFPFFile = outputFile;
        DummyFPFFile.append(".fpf");
        outfile << "Test" << " FirstPassFile\n";
    }

    outfile << opt.Height << " Height\n";
    outfile << opt.Width << " Width\n";

    outfile.close();
    return 0;
}
int OutPutSettingsIVFEnc(char *outputFile, VP8_CONFIG opt)
{
    //Saves all VP8_CONFIG settings to a settings file readable by InputSettings

    ofstream outfile(outputFile);

    outfile << "TargetBandwidth " << opt.TargetBandwidth << "\n";
    outfile << "NoiseSensitivity " << opt.NoiseSensitivity <<  "\n";
    outfile << "Sharpness " << opt.Sharpness <<  "\n";
    outfile << "CpuUsed " << opt.CpuUsed <<  "\n";
    outfile << "Mode " << opt.Mode <<  "\n";
    outfile << "AutoKey " << opt.AutoKey <<  "\n";
    outfile << "KeyFreq " << opt.KeyFreq <<  "\n";
    outfile << "EndUsage " << opt.EndUsage <<  "\n";
    outfile << "UnderShootPct " << opt.UnderShootPct <<  "\n";
    outfile << "StartingBufferLevel " << opt.StartingBufferLevel <<  "\n";
    outfile << "OptimalBufferLevel " << opt.OptimalBufferLevel <<  "\n";
    outfile << "MaximumBufferSize " << opt.MaximumBufferSize <<  "\n";
    outfile << " FixedQ " << opt.FixedQ <<  "\n";
    outfile << "WorstAllowedQ " << opt.WorstAllowedQ <<  "\n";
    outfile << "BestAllowedQ " << opt.BestAllowedQ <<  "\n";
    outfile << " llowSpatialResampling " << opt.AllowSpatialResampling <<  "\n";
    outfile << "ResampleDownWaterMark " << opt.ResampleDownWaterMark <<  "\n";
    outfile << "ResampleUpWaterMark " << opt.ResampleUpWaterMark <<  "\n";
    outfile << "AllowDF " << opt.AllowDF << "\n";
    outfile << "DropFramesWaterMark " << opt.DropFramesWaterMark <<  "\n";
    outfile << "TwoPassVBRBias " << opt.TwoPassVBRBias <<  "\n";
    outfile << "TwoPassVBRMinSection " << opt.TwoPassVBRMinSection <<  "\n";
    outfile << "TwoPassVBRMaxSection " << opt.TwoPassVBRMaxSection <<  "\n";
    outfile << "AllowLag " << opt.AllowLag <<  "\n";
    outfile << "LagInFrames " << opt.LagInFrames <<  "\n";
    outfile << "AltFreq " << opt.AltFreq <<  "\n";
    outfile << "KeyQ " << opt.KeyQ <<  "\n";
    outfile << "AltQ " << opt.AltQ <<  "\n";
    outfile << "GoldQ " << opt.GoldQ <<  "\n";
    outfile << "PlayAlternate " << opt.PlayAlternate <<  "\n";
    outfile << "Version " << opt.Version <<  "\n";
    outfile << "EncodeBreakout " << opt.EncodeBreakout <<  "\n";

    outfile.close();
    return 0;
}
//---------------------------------------------------IVF Header Data------------------------------------------------------------------
int PrintIVFFileHeaderData(IVF_HEADER ivf)
{
    printf("IVF FIle Header Data\n\n"
           "FILE HEADER \n\n"
           "File Header            - %c%c%c%c \n"
           "File Format Version    - %i \n"
           "File Header Size       - %i \n"
           "Video Data FourCC      - %i \n"
           "Video Image Width      - %i \n"
           "Video Image Height     - %i \n"
           "Frame Rate Rate        - %i \n"
           "Frame Rate Scale       - %i \n"
           "Video Length in Frames - %i \n"
           "Unused                 - %c \n"
           "\n\n"
           , ivf.signature[0], ivf.signature[1], ivf.signature[2], ivf.signature[3]
           , ivf.version, ivf.headersize, ivf.FourCC, ivf.width, ivf.height, ivf.rate
           , ivf.scale, ivf.length, ivf.unused);
    return 0;
}
int FormatIVFHeaderRead(IVF_HEADER *ivf)
{
#ifdef __POWERPC__
    //cout << "\n\n\n\n\nPPC DEFINED\n\n\n\n\n";
    // For big endian systems need to swap bytes on height and width
    ivf->width  = ((ivf->width & 0xff) << 8)  | ((ivf->width >> 8) & 0xff);
    ivf->height = ((ivf->height & 0xff) << 8) | ((ivf->height >> 8) & 0xff);
    ivf->rate = (((ivf->rate & 0xff)) << 24)    | (((ivf->rate & 0xff00)) << 8) |	\
                (((ivf->rate & 0xff0000)) >> 8) | (((ivf->rate & 0xff000000)) >> 24);
    ivf->scale = (((ivf->scale & 0xff)) << 24)    | (((ivf->scale & 0xff00)) << 8) |	\
                 (((ivf->scale & 0xff0000)) >> 8) | (((ivf->scale & 0xff000000)) >> 24);
    ivf->length = (((ivf->length & 0xff)) << 24)    | (((ivf->length & 0xff00)) << 8) |	\
                  (((ivf->length & 0xff0000)) >> 8) | (((ivf->length & 0xff000000)) >> 24);
#endif

    return 0;
}
int FormatIVFHeaderWrite(IVF_HEADER &ivf)
{
    ivf.version	= 0;
    ivf.headersize = make_endian_16(32);

    ivf.width      = make_endian_16(ivf.width);
    ivf.height     = make_endian_16(ivf.height);
    ivf.scale      = make_endian_32(ivf.scale);
    ivf.rate       = make_endian_32(ivf.rate);
    ivf.length     = make_endian_32(ivf.length);

    if (ivf.FourCC != 842094169 && ivf.FourCC != 808596553)
    {
        //printf("\nUtil_FourCC: %i",ivf.FourCC);
        //printf("\nini vp8 fourcc\n");
        ivf.FourCC     = MAKEFOURCC('V', 'P', '8', '0');
    }

    return 0;
}
int FormatFrameHeaderRead(IVF_FRAME_HEADER &ivf_fh)
{
#ifdef __POWERPC__
    ivf_fh.frameSize = ((ivf_fh.frameSize & 0xff) << 24) |
                       ((ivf_fh.frameSize & 0xff00) << 8) |
                       ((ivf_fh.frameSize & 0xff0000) >> 8) |
                       ((ivf_fh.frameSize & 0xff000000) >> 24);
    ivf_fh.timeStamp = ((ivf_fh.timeStamp & 0xff) << 56 			|	\
                        ((ivf_fh.timeStamp >>  8) & 0xff) << 48	|	\
                        ((ivf_fh.timeStamp >> 16) & 0xff) << 40	|	\
                        ((ivf_fh.timeStamp >> 24) & 0xff) << 32	|	\
                        ((ivf_fh.timeStamp >> 32) & 0xff) << 24	|	\
                        ((ivf_fh.timeStamp >> 40) & 0xff) << 16	|	\
                        ((ivf_fh.timeStamp >> 48) & 0xff) <<  8	|	\
                        ((ivf_fh.timeStamp >> 56) & 0xff));

    //cout << "POWERPC-Read\n";
#endif

    return 0;
}
int FormatFrameHeaderWrite(IVF_FRAME_HEADER &ivf_fh)
{
#ifdef __POWERPC__
    ivf_fh.timeStamp = make_endian_64(ivf_fh.timeStamp);
    ivf_fh.frameSize = make_endian_32(ivf_fh.frameSize);
#endif

    return 0;
}
//---------------------------------------------------File Management------------------------------------------------------------------
long FileSize(char *inFile)
{
    //finds and returns the size of a file with output.
    char FileNameinFile[256];
    FileName(inFile, FileNameinFile);

    printf("Size of %s: ", FileNameinFile);
    fprintf(stderr, "Size of %s: ", FileNameinFile);
    long l, m;
    ifstream file;
    file.open(inFile, ios::in | ios::binary);

    if (!file.is_open())
    {
        printf("Error opening file: %s\n", inFile);
        fprintf(stderr, "Error opening file: %s\n", inFile);
        return -1;
    }

    l = file.tellg();
    file.seekg(0, ios::end);
    m = file.tellg();
    file.close();
    long Filebytes;
    Filebytes = m - l;

    printf("%i bytes", Filebytes);
    fprintf(stderr, "%i bytes", Filebytes);

    return Filebytes;
}
long FileSize2(char *inFile)
{
    //finds and returns the size of a file without output.
    long l, m;
    ifstream file;
    file.open(inFile, ios::in | ios::binary);

    l = file.tellg();
    file.seekg(0, ios::end);
    m = file.tellg();
    file.close();
    long Filebytes;
    Filebytes = m - l;
    return Filebytes;
}
void FileName(char *input, char *FileName)
{
    //Extracts only the files name from its full path.

    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;

    while (input[parser] != '\0')
    {
        if (input[parser] == slashChar)
        {
            slashcount++;
        }

        parser++;
    }

    parser = 0;
    int parser2 = 0;

    while (input[parser] != '\0')
    {

        if (slashcount2 > slashcount - 1)
        {
            FileName[parser2] = input[parser];
            parser2++;
        }

        if (input[parser] == slashChar)
        {
            slashcount2++;
        }

        parser++;
    }

    FileName[parser2] = '\0';

    return;
}


void FolderName(char *input, char *output)
{
    //Gets the full name of the folder a file is in and returns it.

    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;
    char *Dir = input;

    while (Dir[parser] != '\0')
    {
        if (Dir[parser] == slashChar)
        {
            slashcount++;
        }

        parser++;
    }

    parser = 0;

    while (slashcount2 < slashcount)
    {
        if (Dir[parser] == slashChar)
        {
            slashcount2++;
        }

        output[parser] = Dir[parser];
        parser++;
    }

    output[parser] = '\0';

    return;
}

void FolderName2(char *DirIn, char *DirOut)
{
    //Takes in the full name of a file and writes the directory and file name (without its extention) to the second input.
    int ArraySize = 0;
    int parser = 0;

    while (DirIn[ArraySize] != '\0')
    {
        ArraySize++;
    }

    while (parser < ArraySize - 4)
    {

        DirOut[parser] = DirIn[parser];
        parser++;
    }

    DirOut[parser] = '_';
    DirOut[parser+1] = '\0';

    return;
}

void CopyCharArray(char *input, char *output)
{
    //Gets the full name of the folder a file is in and copies it to the second input.

    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;
    char *Dir = input;

    while (Dir[parser] != '\0')
    {
        output[parser] = Dir[parser];
        parser++;
    }

    output[parser] = '\0';
    return;
}

string ExtractDateTime(string InputStr)
{
    //Extracts only the files name from its full path.

#if defined(_WIN32)
    enum { len = 255 };
    char input[len];
    const errno_t e = strcpy_s(input, len, InputStr.c_str());
    assert(e == 0);
#elif defined(linux)
    char input[255];
    snprintf(input, 255, "%s", InputStr.c_str());
#elif defined(__APPLE__)
    char input[255];
    snprintf(input, 255, "%s", InputStr.c_str());
#elif defined(__POWERPC__)
    char input[255];
    snprintf(input, 255, "%s", InputStr.c_str());
#endif

    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;
    char  FileName[255];

    while (input[parser] != '\0')
    {
        if (input[parser] == slashChar)
        {
            slashcount++;
        }

        parser++;
    }

    parser = 0;
    int parser2 = 0;

    while (input[parser] != '\0')
    {

        if (slashcount2 > slashcount - 1)
        {
            FileName[parser2] = input[parser];
            parser2++;
        }

        if (input[parser] == slashChar)
        {
            slashcount2++;
        }

        parser++;
    }

    FileName[parser2] = '\0';
    string FileNameStr = FileName;
    return FileNameStr;
}
int TimeStampCompare(string TimeStampNow, string TimeStampPrevious)
{
    char TimeStamp1[255];
    char TimeStamp2[255];

    snprintf(TimeStamp1, 255, "%s", TimeStampNow.c_str());
    snprintf(TimeStamp2, 255, "%s", TimeStampPrevious.c_str());

    int	i = 0;

    while (i < 24)
    {
        if (TimeStamp1[i] != TimeStamp2[i])
        {
            return 0;
        }

        i++;
    }

    return 1;
}
int Test0InputTextCheck(char *input, int MoreInfo)
{
    //function returns number of tests found if input is correct -1 if not correct and -3 if there is an error

    //////////////////////////////Read Number of Tests//////////////////////////////////////////
    fstream infile2;
    infile2.open(input);

    if (!infile2.good())
    {
        printf("\nInput File does not exist\n");
        infile2.close();
        return 0;
    }

    int numberoftests = 0;

    while (!infile2.eof())
    {
        char buffer3[1024];
        infile2.getline(buffer3, 1024);

        int Buffer0CharAscii = buffer3[0];

        //skips over any line starting with a % in the input file to allow for comenting
        if (Buffer0CharAscii == 37 || Buffer0CharAscii == '\0' || Buffer0CharAscii == '\r')
        {
            //linesskippedtracker++;
        }
        else
        {
            numberoftests++;
        }
    }

    infile2.clear();
    infile2.close();
    ////////////////////////////////////////////////////////////////////////////////////////////

    int linesskippedtracker = 0;

    char buffer[1024];
    char buffer2[1024];

    int Buf1Var = 0;
    int DummyArgvVar = 1;
    int CommentBool = 0;

    //int PassFail[9999];
    int *PassFail = new int[numberoftests+2];
    //PassFail[0] = 0;
    int PassFailInt = 0;
    int TestsRun = 0;

    //string StringAr[9999];
    string *StringAr = new string[99];
    //StringAr[0] = "";
    //string SelectorAr[9999];
    string *SelectorAr = new string[99];
    //SelectorAr[0] = "";
    //SelectorAr = NULL;
    //string SelectorAr2[9999];
    string *SelectorAr2 = new string[numberoftests+2];
    //SelectorAr2[0] = "";

    int SelectorArInt = 0;
    int y;
    int PassFailReturn = 1;



    fstream infile;
    infile.open(input);

    if (!infile.good())
    {
        printf("\nInput File does not exist\n");
        infile.close();
        delete [] PassFail;
        delete [] StringAr;
        delete [] SelectorAr;
        delete [] SelectorAr2;
        return 0;
    }

    int NumberOfTestsRun = 1;
    ///////////////////////////////////////
    int trackthis1 = 0;
    int trackthis2 = 0;

    ////////////////////////////////////////
    while (!infile.eof())
    {
        const char *DummyArgv[999];
        DummyArgv[0] = "";
        DummyArgvVar = 1;
        infile.getline(buffer, 1024);

        cout << buffer << "\n";

        trackthis1++;

        int Buffer0CharAscii = buffer[0];

        //skips over any line starting with a % in the input file to allow for comenting
        if (Buffer0CharAscii == 37 || Buffer0CharAscii == '\0' || Buffer0CharAscii == '\r')
        {
            linesskippedtracker++;
        }
        else
        {
            Buf1Var = 0;

            //parses through gotline and seperates commands out
            while (buffer[Buf1Var] != '\0' && buffer[Buf1Var] != '\r')
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\0' && buffer[Buf1Var] != '\r')
                {
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\0' && buffer[Buf1Var] != '\r')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

            y = 1;

            while (y < DummyArgvVar)
            {
                DummyArgv[y] = StringAr[y].c_str();
                y++;
            }

            ////this is for Mode 3 test only mode in order to find the right TimeStamp
            ////////////////////////////////////////////////////////////////////
            if (CommentBool == 0)
            {
                int selector = atoi(DummyArgv[1]);

                if (selector >= 0 && selector < 52)
                {
                    //if (selector != 11 && selector != 33 && selector != 35 && selector != 0)
                    NumberOfTestsRun++;
                }

                if (selector == 0)
                {

                }

                if (selector == AlWDFNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "AllowDropFrames";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == ALWLGNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "AllowLagTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == ALWSRNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "AllowSpatialResampling";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                /*if (selector == ALWS2NUM)
                {
                if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                {
                SelectorAr[SelectorArInt].append(buffer);
                SelectorAr2[SelectorArInt] = "AllowSpatialResampling2";
                PassFail[PassFailInt] = trackthis1;
                }
                else
                {

                PassFail[PassFailInt] = -1;
                }
                }*/

                if (selector == ALTFRNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 8))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "AltFreqTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == AUTKFNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 6))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "AutoKeyFramingWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == BUFLVNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "BufferLevelWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == CPUDENUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 6))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "CPUDecOnlyWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                /*if (selector == CHGCPNUM)
                {
                if (!(DummyArgvVar == 7 || DummyArgvVar == 6))
                {
                SelectorAr[SelectorArInt].append(buffer);
                SelectorAr2[SelectorArInt] = "ChangeCPUDec";
                PassFail[PassFailInt] = trackthis1;
                }
                else
                {

                PassFail[PassFailInt] = -1;
                }
                }*/

                if (selector == CHGWRNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 6))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "ChangeCPUWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == DFWMWNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "DropFramesWaterMarkWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == DTARTNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "DataRateTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == DBMRLNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 8))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "DebugMatchesRelease";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == ENCBONUM)
                {

                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "EncoderBreakOut";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == ERRMWNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "ErrorResilientModeWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == EXTFINUM)
                {
                    if (!(DummyArgvVar == 3 || DummyArgvVar == 4))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "ExtraFileCheck";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == FIXDQNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 8))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "FixedQ";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == FKEFRNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 7))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "ForceKeyFameWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == GQVBQNUM)
                {
                    if (!(DummyArgvVar == 4 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "GoodQualityVsBestQuality";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        //SelectorAr[SelectorArInt] = buffer;
                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == LGIFRNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 8))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "LagInFramesTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == MAXQUNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "MaxQuantizerTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == MEML1NUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 7))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "MemLeakCheck";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == MEML2NUM)
                {

                    if (!(DummyArgvVar == 4 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "MemLeakCheck2";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == MINQUNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "MinQuantizerTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == MULTTNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "MultiThreadedTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == NVOPSNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 8))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "NewVsOldPSNR";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == NVORTNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 6))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "NewVsOldRealTimeSpeed";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == NOISENUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "NoiseSensititivityWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == OV2PSNUM)
                {
                    if (!(DummyArgvVar == 4 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "OnePassVsTwoPass";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == PLYALNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "PlayAlternate";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == POSTPNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "PostProcessorWorks";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                /*if (selector == PREPRNUM)
                {
                if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                {
                SelectorAr[SelectorArInt].append(buffer);
                SelectorAr2[SelectorArInt] = "PreProcessorWorks";
                PassFail[PassFailInt] = trackthis1;
                }
                else
                {

                PassFail[PassFailInt] = -1;
                }
                }*/

                if (selector == RECONBUF)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "ReconBuffer";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == RSDWMNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "ResampleDownWaterMark";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == SPEEDNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 7))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "SpeedTest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {
                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == TVECTNUM)
                {

                    if (!(DummyArgvVar == 3 || DummyArgvVar == 4))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "TestVectorCheck";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == TV2BTNUM)
                {
                    if (!(DummyArgvVar == 4 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "TwoPassVsTwoPassBest";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == UNDSHNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "UnderShoot";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == VERSINUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "Version";
                        PassFail[PassFailInt] = trackthis1;
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == WMLMMNUM)
                {
                    if (!(DummyArgvVar == 9 || DummyArgvVar == 10))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "WindowsMatchesLinux";
                        PassFail[PassFailInt] = trackthis1;
                        //cout << "\n\n\n\n\n" << DummyArgvVar << "\n\n\n\n\n";
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                //Make sure that all tests input are vaild tests by checking the list (make sure to add new tests here!)
                if (selector != RTFFINUM && selector != AlWDFNUM && selector != ALWLGNUM && selector != ALTFRNUM &&
                    selector != AUTKFNUM && selector != BUFLVNUM && selector != CPUDENUM && selector != CHGWRNUM &&
                    selector != DFWMWNUM && selector != DTARTNUM && selector != DBMRLNUM && selector != ENCBONUM && selector != ERRMWNUM &&
                    selector != EXTFINUM && selector != FIXDQNUM && selector != FKEFRNUM && selector != GQVBQNUM && selector != LGIFRNUM &&
                    selector != MAXQUNUM && selector != MEML1NUM && selector != MEML2NUM && selector != MINQUNUM && selector != MULTTNUM &&
                    selector != NVOPSNUM && selector != NVORTNUM && selector != NOISENUM && selector != OV2PSNUM && selector != PLYALNUM &&
                    selector != POSTPNUM && selector != RSDWMNUM && selector != SPEEDNUM && selector != TVECTNUM && selector != RECONBUF &&
                    selector != TV2BTNUM && selector != UNDSHNUM && selector != VERSINUM && selector != WMLMMNUM && selector != ALWSRNUM)
                {
                    SelectorAr[SelectorArInt].append(buffer);
                    SelectorAr2[SelectorArInt] = "Test Not Found";
                    PassFail[PassFailInt] = trackthis1;
                }

                PassFailInt++;
                SelectorArInt++;
            }
        }
    }

    y = 0;
    printf("\n");


    while (y < SelectorArInt)
    {
        if (PassFail[y] != -1)
        {
            PassFailReturn = 0;
            printf("Line: %4i Test: %-25s - Not properly Formatted\n", PassFail[y], SelectorAr2[y].c_str());
            printf("%s\n\n", SelectorAr[y].c_str());
        }

        y++;
    }

    //more info if == 1 then will display text file statistics if 0 then will not 0 ment for
    //File Check prior to running External Test runner to ensure input is correct
    if (MoreInfo == 0)
    {
        if (PassFailReturn == 0)
        {

            infile.close();
            delete [] PassFail;
            delete [] StringAr;
            delete [] SelectorAr;
            delete [] SelectorAr2;
            //return 0;
            return -1;

        }
        else
        {
            if (PassFailInt == 0)
            {
                printf("Test File Specified is empty.");
                delete [] PassFail;
                delete [] StringAr;
                delete [] SelectorAr;
                delete [] SelectorAr2;
                return 0;
            }
            else
            {

                printf("\nAll %i Tests in text file: %s - are properly Formatted\n\n", y, input);

                //return 1;
                delete [] PassFail;
                delete [] StringAr;   //will cause error
                //delete [] SelectorAr; //will cause error
                delete [] SelectorAr2;
                infile.close();
                return SelectorArInt;
            }
        }
    }
    else
    {

        if (PassFailReturn == 0)
        {
            printf("\nFile Contains %i Tests\n", SelectorArInt);
            printf("\nFile Contains %i Lines\n", trackthis1);
            infile.close();
            delete [] PassFail;
            delete [] StringAr;
            delete [] SelectorAr;
            delete [] SelectorAr2;
            //return 0;
            return -1;
        }
        else
        {
            printf("\nAll %i Tests in text file: %s - are properly Formatted\n\n", y, input);
            printf("\nFile Contains %i Tests\n", SelectorArInt);
            printf("\nFile Contains %i Lines\n", trackthis1);
            infile.close();
            //return 1;
            delete [] PassFail;
            delete [] StringAr;
            delete [] SelectorAr;
            delete [] SelectorAr2;
            return SelectorArInt;
        }
    }

    infile.close();
    //return 3;
    delete [] PassFail;
    delete [] StringAr;
    delete [] SelectorAr;
    delete [] SelectorAr2;
    return -3;
}
int FileExistsCheck(string input)
{
    char inputchar[255];
    snprintf(inputchar, 255, "%s", input.c_str());

    ifstream infile(inputchar);

    if (infile)
    {
        return 1;
    }

    return 0;
}
void SubFolderName(char *input, char *FileName)
{
    //extracts name of last two folders a target file is in and returns it
    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;

    while (input[parser] != '\0')
    {
        if (input[parser] == slashChar)
        {
            slashcount++;
        }

        parser++;
    }

    parser = 0;
    int parser2 = 0;

    while (input[parser] != '\0')
    {

        if (slashcount2 > slashcount - 2)
        {
            FileName[parser2] = input[parser];
            parser2++;
        }

        if (input[parser] == slashChar)
        {
            slashcount2++;
        }

        parser++;
    }

    FileName[parser2] = '\0';

    return;
}
void TestName(char *input, char *FileName)
{
    //Gets the directory test name from an input sting
    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;
    char OpSlashChar;

    //find out what the default shash char is then define the oposite slash char
    if (slashChar == '\\')
    {
        OpSlashChar = '/';
    }

    if (slashChar == '/')
    {
        OpSlashChar = '\\';
    }

    //Parse through stirng replaceing any instance of an oposite slash char with a
    //correct slash char in order to run cross plat.
    while (input[parser] != '\0')
    {
        if (input[parser] == OpSlashChar)
        {
            input[parser] = slashChar;
        }

        parser++;
    }

    //continue with the function after proper initialization.
    parser = 0;

    while (input[parser] != '\0' && input[parser] != slashChar)
    {
        FileName[parser] = input[parser];
        parser++;
    }

    FileName[parser] = '\0';

    return;
}
//--------------------------------------------------------Math------------------------------------------------------------------------
int DecimalPlaces(int InputNumber)
{
    int y = 0;
    int totaltensplaces = 0;

    //find out how many decimal places
    while (y >= 0)
    {
        y = InputNumber - pow(10.0, totaltensplaces);
        //printf("%i = %i - %f\n",y,total_number_of_frames,pow(10.0,totaltensplaces));
        totaltensplaces++;
    }

    //cout << "\n" << totaltensplaces << "\n";
    totaltensplaces = totaltensplaces - 1;
    //cout << "\n" << totaltensplaces << "\n";

    return totaltensplaces;
}
int GCD(int a, int b)
{
    while (1)
    {
        a = a % b;

        if (a == 0)
            return b;

        b = b % a;

        if (b == 0)
            return a;
    }
}
int absInt(int input)
{
    if (input < 0)
    {
        input = input * -1;
    }

    return input;
}
float absFloat(float input)
{
    if (input < 0.0)
    {
        input = input * -1.0;
    }

    return input;
}
double absDouble(double input)
{
    if (input < 0)
    {
        input = input * -1.0;
    }

    return input;
}
int SolveQuadradic(float X1, float X2, float X3, float Y1, float Y2, float Y3, float &A, float &B, float &C)
{
    /*float X1 = 0;
    float X2 = 0;
    float X3 = 0;

    float Y1 = 0;
    float Y2 = 0;
    float Y3 = 0;

    float A = 0;
    float B = 0;
    float C = 0;

    printf("Intput X1: ");
    cin >> X1;
    printf("Intput X2: ");
    cin >> X2;
    printf("Intput X3: ");
    cin >> X3;

    printf("Intput Y1: ");
    cin >> Y1;
    printf("Intput Y2: ");
    cin >> Y2;
    printf("Intput Y3: ");
    cin >> Y3;*/


    A = ((Y2 - Y1) * (X1 - X3) + (Y3 - Y1) * (X2 - X1)) / ((X1 - X3) * ((X2 * X2) - (X1 * X1)) + (X2 - X1) * ((X3 * X3) - (X1 * X1)));
    B = ((Y2 - Y1) - A * ((X2 * X2) - (X1 * X1))) / (X2 - X1);
    C = Y1 - A * (X1 * X1) - B * X1;

    //cout << "\nA = " << A;
    //cout << "\nB = " << B;
    //cout << "\nC = " << C << "\n";

    //printf("\n\ny = %.2fx^2 + %.2fx + %.2f\n\n",A,B,C);

    return 0;
}
float AreaUnderQuadradic(float A, float B, float C, float X1, float X2)
{
    float Area1 = ((A * X1 * X1 * X1) / 3) + ((B * X1 * X1) / 2) + C * X1;
    float Area2 = ((A * X2 * X2 * X2) / 3) + ((B * X2 * X2) / 2) + C * X2;
    float TotalArea = Area2 - Area1;
    return TotalArea;
}
char *itoa_custom(int value, char *result, int base)
{
    int x = 0;

    if (base < 2 || base > 16)
    {
        *result = 0;
        return result;
    }

    char *out = result;
    int quotient = value;

    do
    {
        *out = "0123456789abcdef"[ abs(quotient % base)];
        ++out;
        quotient /= base;
    }
    while (quotient);

    // Only apply negative sign for base 10
    if (value < 0 && base == 10) *out++ = '-';

    reverse(result, out);
    *out = 0;
    return result;
}
//----------------------------------------------------Cross Plat----------------------------------------------------------------------
unsigned int ON2_GetHighResTimerTick()
{
#if defined(_WIN32)
    LARGE_INTEGER pf;                       // Performance Counter Frequency
    LARGE_INTEGER currentTime;
    unsigned int currentTick;

    if (QueryPerformanceFrequency(&pf))
    {
        // read the counter and TSC at start
        QueryPerformanceCounter(&currentTime);
        currentTick  = currentTime.LowPart;
    }
    else
    {
        currentTick = timeGetTime();
    }

    return currentTick;
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (0xffffffff & (tv.tv_sec * 1000000 + tv.tv_usec)) ;
#endif
}
unsigned int ON2_GetTimeInMicroSec(unsigned int startTick, unsigned int stopTick)
{
#if defined(_WIN32)
    LARGE_INTEGER pf;
    __int64 duration = stopTick - startTick;

    if (QueryPerformanceFrequency(&pf))
    {
        return (unsigned int)(duration * 1000000 /  pf.LowPart);
    }
    else
    {
        return (unsigned int)(duration * 1000);
    }

#else
    long long duration = stopTick - startTick;
    return duration;
#endif
}
unsigned int ON2_GetProcCoreCount()
{
    return 2;
}
unsigned int GetTime()
{
    unsigned int Time = 0;
    Time = ON2_GetHighResTimerTick();
    return Time;
}
int MakeDirVPX(string CreateDir2)
{
#if defined(_WIN32)
    /////////////////////////////////////
    CreateDir2.erase(0, 4);
    CreateDir2.insert(0, "mkdir \"");
    CreateDir2.append("\"");
    system(CreateDir2.c_str());
    /////////////////////////////////////
#elif defined(linux)
    CreateDir2.erase(0, 4);
    CreateDir2.insert(0, "mkdir -p \"");
    CreateDir2.append("\"");
    system(CreateDir2.c_str());
#elif defined(__APPLE__)
    CreateDir2.erase(0, 4);
    CreateDir2.insert(0, "mkdir -p \"");
    CreateDir2.append("\"");
    system(CreateDir2.c_str());
#elif defined(__POWERPC__)
    CreateDir2.erase(0, 4);
    CreateDir2.insert(0, "mkdir -p \"");
    CreateDir2.append("\"");
    system(CreateDir2.c_str());
#endif

    return 0;
}

void RunExe(string RunExe)
{
    printf("\nAtempting to run: %s\n\n", RunExe.c_str());
#if defined(_WIN32)
    system(RunExe.c_str());
#elif defined(linux)
    system(RunExe.c_str());
#elif defined(__APPLE__)

    system(RunExe.c_str());

#elif defined(__POWERPC__)

    system(RunExe.c_str());
#endif

    return;
}
//---------------------------------------------------------IVF------------------------------------------------------------------------
int image2yuvconfig(const on2_image_t   *img, YV12_BUFFER_CONFIG  *yv12)
{
    //on2_codec_err_t        res = ON2_CODEC_OK;
    yv12->BufferAlloc = img->planes[PLANE_Y];
    yv12->YBuffer = img->planes[PLANE_Y];
    yv12->UBuffer = img->planes[PLANE_U];
    yv12->VBuffer = img->planes[PLANE_V];

    yv12->YWidth  = img->w;
    yv12->YHeight = img->h;
    yv12->UVWidth = yv12->YWidth / 2;
    yv12->UVHeight = yv12->YHeight / 2;

    yv12->YStride = img->stride[PLANE_Y];
    yv12->UVStride = img->stride[PLANE_U];

    yv12->border  = (img->stride[PLANE_Y] - img->w) / 2;

    int yplane_size = (img->d_h + 2 * yv12->border) * (img->d_w + 2 * yv12->border);
    int uvplane_size = ((1 + img->d_h) / 2 + yv12->border) * ((1 + img->d_w) / 2 + yv12->border);
    yv12->frame_size = yplane_size + 2 * uvplane_size;

    //	yv12->clrtype = (/*img->fmt == IMG_FMT_ON2I420 || img->fmt == */IMG_FMT_ON2YV12); //REG_YUV = 0
    return 0;
}
double IVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, double &SsimOut)
{
    double summedQuality = 0;
    double summedWeights = 0;
    double summedPsnr = 0;
    double summedYPsnr = 0;
    double summedUPsnr = 0;
    double summedVPsnr = 0;
    double sumSqError = 0;
    double sumBytes = 0;
    double sumBytes2 = 0;

    unsigned int currentVideo1Frame = 0;
    int RawFrameOffset = 0;
    int CompressedFrameOffset = 0;
    unsigned int maximumFrameCount = 0;

    forceUVswap = 0;

    ////////////////////////Initilize Raw File////////////////////////

    FILE *RawFile = fopen(inputFile1, "rb");

    if (RawFile == NULL)
    {
        printf("\nError Opening Raw File: %s\n", inputFile1);
        fprintf(stderr, "\nError Opening Raw File: %s\n", inputFile1);
        fclose(RawFile);
        return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    FormatIVFHeaderRead(&ivfhRaw);

    int buffer_sz = 32;
    unsigned int frameCount = ivfhRaw.length;

    unsigned char *RawVideoBuffer = new unsigned char[ivfhRaw.width*ivfhRaw.height*3];

    YV12_BUFFER_CONFIG Raw_YV12;
    Raw_YV12.YWidth   = ivfhRaw.width;
    Raw_YV12.YHeight  = ivfhRaw.height;
    Raw_YV12.YStride  = Raw_YV12.YWidth;
    Raw_YV12.UVWidth  = Raw_YV12.YWidth >> 1;
    Raw_YV12.UVHeight = Raw_YV12.YHeight >> 1;
    Raw_YV12.UVStride = Raw_YV12.YStride >> 1;
    Raw_YV12.BufferAlloc		= RawVideoBuffer;
    Raw_YV12.YBuffer			= RawVideoBuffer;
    Raw_YV12.UBuffer			= Raw_YV12.YBuffer + Raw_YV12.YWidth * Raw_YV12.YHeight;
    Raw_YV12.VBuffer			= Raw_YV12.UBuffer + Raw_YV12.UVWidth * Raw_YV12.UVHeight;

    if (RawFrameOffset > 0) //Burn Frames untill Raw frame offset reached - currently disabled by override of RawFrameOffset
    {
        for (int i = 0; i < RawFrameOffset; i++)
        {
        }
    }

    if (ivfhRaw.FourCC == 842094169)
    {
        forceUVswap = 0;   //if YV12 Do not swap Frames
    }

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.UBuffer;
        Raw_YV12.UBuffer = Raw_YV12.VBuffer;
        Raw_YV12.VBuffer = temp;
    }

    ////////////////////////Initilize Compressed File////////////////////////
    FILE *CompFile = fopen(inputFile2, "rb");

    if (CompFile == NULL)
    {
        printf("\nError Opening Compressed File: %s\n", inputFile2);
        fprintf(stderr, "\nError Opening Compressed File: %s\n", inputFile2);
        fclose(RawFile);
        fclose(CompFile);
        delete [] RawVideoBuffer;
        return 0;
    }

    IVF_HEADER ivfhComp;
    InitIVFHeader(&ivfhComp);
    fread(&ivfhComp, 1, sizeof(ivfhComp), CompFile);
    FormatIVFHeaderRead(&ivfhComp);

    YV12_BUFFER_CONFIG Comp_YV12;
    //memset(&Comp_YV12, 0, sizeof(Comp_YV12));
    //vp8_yv12_alloc_frame_buffer(&Comp_YV12, ivfhRaw.height, ivfhRaw.width, 32);

    if (CompressedFrameOffset > 0) //Burn Frames untill Compressed frame offset reached - currently disabled by override of CompressedFrameOffset
    {
    }

    /////////////////////////////////////////////////////////////////////////
    ////////Printing////////
    if (printvar != 0)
    {
        printf("\n\n                        ---------Computing PSNR---------");
        fprintf(stderr, "\n\n                        ---------Computing PSNR---------");
    }

    if (printvar == 0)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 1)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
        fprintf(stderr, "\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 5)
    {
        printf("\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
        fprintf(stderr, "\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
        else
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
            fprintf(stderr, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
    }

    ////////////////////////

    __int64 *timeStamp2 = new __int64;
    __int64 *timeEndStamp2 = new __int64;
    int deblock_level2 = 0;
    int noise_level2 = 0;
    int flags2 = 0;
    int currentFrame2 = 0;

    //////////////////////////////////////////New//////////////////////////////////////////
    on2_codec_ctx_t       decoder;
    on2_codec_iface_t       *iface = NULL;
    on2_codec_iface_t  *ivf_iface = ifaces[0].iface;
    on2_codec_dec_cfg_t     cfg = {0};
    iface = ivf_iface;

    vp8_postproc_cfg_t ppcfg = {0};

    ppcfg.DeblockingLevel = deblock_level2;
    ppcfg.NoiseLevel = noise_level2;
    ppcfg.PostProcFlag = flags2;

    if (on2_codec_dec_init(&decoder, ifaces[0].iface, &cfg, 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /////////////////////Setup Temp YV12 Buffer to Resize if nessicary/////////////////////
    vp8_scale_machine_specific_config();

    YV12_BUFFER_CONFIG Temp_YV12;
    memset(&Temp_YV12, 0, sizeof(Temp_YV12));
    vp8_yv12_alloc_frame_buffer(&Temp_YV12, ivfhRaw.width, ivfhRaw.height, 0);
    ///////////////////////////////////////////////////////////////////////////////////////

    on2_codec_control(&decoder, VP8_SET_POSTPROC, &ppcfg);
    //////////////////////////////////////////New//////////////////////////////////////////

    while (currentVideo1Frame <= frameCount - 1)
    {
        unsigned long lpdwFlags = 0;
        unsigned long lpckid = 0;
        long bytes1;
        long bytes2;

        //////////////////////Get YV12 Data For Compressed File//////////////////////
        IVF_FRAME_HEADER ivf_fhComp;

        if (!fread(&ivf_fhComp, 1, sizeof(ivf_fhComp), CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] RawVideoBuffer;
            return 0;
        }

        FormatFrameHeaderRead(ivf_fhComp);

        bytes2 = ivf_fhComp.frameSize;
        sumBytes2 += bytes2;

        char *CompBuff = new char[ivf_fhComp.frameSize*2];

        if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] RawVideoBuffer;
            delete [] CompBuff;
            return 0;
        }

        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;

        if (on2_codec_decode(&decoder, (uint8_t *) CompBuff, ivf_fhComp.frameSize, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));
        }

        img = on2_codec_get_frame(&decoder, &iter);

        if (img)
        {

            image2yuvconfig(img, &Comp_YV12);

            if (img->d_w != ivfhRaw.width || img->d_h != ivfhRaw.height) //if frame not correct size resize it for psnr
            {
                int GCDInt1 = GCD(img->d_w, ivfhRaw.width);
                int GCDInt2 = GCD(img->d_h, ivfhRaw.height);

                vp8_yv12_scale_or_center(&Comp_YV12, &Temp_YV12, ivfhRaw.width, ivfhRaw.height, 0, (ivfhRaw.width / GCDInt1), (img->d_w / GCDInt1), (ivfhRaw.height / GCDInt2), (img->d_h / GCDInt2));

                Comp_YV12 = Temp_YV12;
            }

            delete [] CompBuff;
            /////////////////////////////////////////////////////////////////////////////

            //////////////////////Get YV12 Data For Raw File//////////////////////
            IVF_FRAME_HEADER ivf_fhRaw;

            if (!fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), RawFile))
            {
                printf("\nError Computing PSNR\n");
                fprintf(stderr, "\nError Computing PSNR\n");
                fclose(RawFile);
                fclose(CompFile);
                delete timeStamp2;
                delete timeEndStamp2;
                delete [] RawVideoBuffer;
                return 0;
            }

            FormatFrameHeaderRead(ivf_fhRaw);

            bytes1 = ivf_fhRaw.frameSize;
            sumBytes += bytes1;

            memset(RawVideoBuffer, 0, ivfhRaw.width * ivfhRaw.height * 3);

            if (!fread(RawVideoBuffer, 1, ivf_fhRaw.frameSize, RawFile))
            {
                printf("\nError Computing PSNR\n");
                fprintf(stderr, "\nError Computing PSNR\n");
                fclose(RawFile);
                fclose(CompFile);
                delete timeStamp2;
                delete timeEndStamp2;
                delete [] RawVideoBuffer;
                return 0;
            }

            //////////////////////////////////////////////////////////////////////

            ///////////////////////////Preform PSNR Calc///////////////////////////////////
            double weight;
            double thisSsim = VP8_CalcSSIM2(&Raw_YV12, &Comp_YV12, 1, &weight);
            summedQuality += thisSsim * weight ;
            summedWeights += weight;

            double YPsnr;
            double UPsnr;
            double VPsnr;
            double SqError;
            double thisPsnr = VP8_CalcPSNR(&Raw_YV12, &Comp_YV12, &YPsnr, &UPsnr, &VPsnr, &SqError);

            summedYPsnr += YPsnr;
            summedUPsnr += UPsnr;
            summedVPsnr += VPsnr;
            summedPsnr += thisPsnr;
            sumSqError += SqError;
            ///////////////////////////////////////////////////////////////////////////////

            ////////Printing////////
            if (printvar == 1 || printvar == 5 || printvar == 0)
            {
                if (frameStats == 0)
                {
                    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                           8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                           currentVideo1Frame, frameCount
                          );
                }

                if (frameStats == 1)
                {
                    printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                           currentVideo1Frame,
                           bytes1 * 8.0,
                           bytes2 * 8.0,
                           thisPsnr, 1.0 * YPsnr ,
                           1.0 * UPsnr ,
                           1.0 * VPsnr);

                    fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                            currentVideo1Frame,
                            bytes1 * 8.0,
                            bytes2 * 8.0,
                            thisPsnr, 1.0 * YPsnr ,
                            1.0 * UPsnr ,
                            1.0 * VPsnr);
                }

                if (frameStats == 2)
                {
                    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                           8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                           currentVideo1Frame, frameCount
                          );

                    fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                            currentVideo1Frame,
                            bytes1 * 8.0,
                            bytes2 * 8.0,
                            thisPsnr, 1.0 * YPsnr ,
                            1.0 * UPsnr ,
                            1.0 * VPsnr);
                }

                if (frameStats == 3)
                {
                    printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                           currentVideo1Frame,
                           bytes1 * 8.0,
                           bytes2 * 8.0,
                           thisPsnr, 1.0 * YPsnr ,
                           1.0 * UPsnr ,
                           1.0 * VPsnr);
                }
            }

            ////////////////////////

            ++currentVideo1Frame;

        }
        else
        {
            delete [] CompBuff;
        }
    }

    on2_codec_destroy(&decoder);

    //Over All PSNR Calc
    double samples = 3.0 / 2 * frameCount * Raw_YV12.YWidth * Raw_YV12.YHeight;
    double avgPsnr = summedPsnr / frameCount;
    double totalPsnr = VP8_Mse2Psnr(samples, 255.0, sumSqError);
    double totalSSim = 100 * pow(summedQuality / summedWeights, 8.0);

    ////////Printing////////
    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,					//divided by two added when rate doubled to handle doubling of timestamp
                   sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,				//divided by two added when rate doubled to handle doubling of timestamp
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

        }
        else
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,           //divided by two added when rate doubled to handle doubling of timestamp
                   sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,       //divided by two added when rate doubled to handle doubling of timestamp
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

            fprintf(stderr, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                    sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,           //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,       //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr, totalSSim);
        }
    }

    if (printvar != 0)
    {
        printf("\n                        --------------------------------\n");
        fprintf(stderr, "\n                        --------------------------------\n");
    }

    ////////////////////////
    fclose(RawFile);
    fclose(CompFile);

    delete timeStamp2;
    delete timeEndStamp2;
    delete [] RawVideoBuffer;

    //vp8_yv12_de_alloc_frame_buffer(&Tempsd);

    return totalPsnr;
}
double IVFPSNR_CORE(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, double &SsimOut)
{
    double summedQuality = 0;
    double summedWeights = 0;
    double summedPsnr = 0;
    double summedYPsnr = 0;
    double summedUPsnr = 0;
    double summedVPsnr = 0;
    double sumSqError = 0;
    double sumBytes = 0;
    double sumBytes2 = 0;

    unsigned int currentVideo1Frame = 0;
    int RawFrameOffset = 0;
    int CompressedFrameOffset = 0;
    unsigned int maximumFrameCount = 0;

    forceUVswap = 0;

    ////////////////////////Initilize Raw File////////////////////////

    FILE *RawFile = fopen(inputFile1, "rb");

    if (RawFile == NULL)
    {
        printf("\nError Opening Raw File: %s\n", inputFile1);
        fprintf(stderr, "\nError Opening Raw File: %s\n", inputFile1);
        fclose(RawFile);
        return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    FormatIVFHeaderRead(&ivfhRaw);

    int buffer_sz = 32;
    unsigned int frameCount = ivfhRaw.length;

    unsigned char *RawVideoBuffer = new unsigned char[ivfhRaw.width*ivfhRaw.height*3];

    YV12_BUFFER_CONFIG Raw_YV12;
    Raw_YV12.YWidth   = ivfhRaw.width;
    Raw_YV12.YHeight  = ivfhRaw.height;
    Raw_YV12.YStride  = Raw_YV12.YWidth;
    Raw_YV12.UVWidth  = Raw_YV12.YWidth >> 1;
    Raw_YV12.UVHeight = Raw_YV12.YHeight >> 1;
    Raw_YV12.UVStride = Raw_YV12.YStride >> 1;
    Raw_YV12.BufferAlloc		= RawVideoBuffer;
    Raw_YV12.YBuffer			= RawVideoBuffer;
    Raw_YV12.UBuffer			= Raw_YV12.YBuffer + Raw_YV12.YWidth * Raw_YV12.YHeight;
    Raw_YV12.VBuffer			= Raw_YV12.UBuffer + Raw_YV12.UVWidth * Raw_YV12.UVHeight;

    if (RawFrameOffset > 0) //Burn Frames untill Raw frame offset reached - currently disabled by override of RawFrameOffset
    {
        for (int i = 0; i < RawFrameOffset; i++)
        {
        }
    }

    if (ivfhRaw.FourCC == 842094169)
    {
        forceUVswap = 0;   //if YV12 Do not swap Frames
    }

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.UBuffer;
        Raw_YV12.UBuffer = Raw_YV12.VBuffer;
        Raw_YV12.VBuffer = temp;
    }

    ////////////////////////Initilize Compressed File////////////////////////
    FILE *CompFile = fopen(inputFile2, "rb");

    if (CompFile == NULL)
    {
        printf("\nError Opening Compressed File: %s\n", inputFile2);
        fprintf(stderr, "\nError Opening Compressed File: %s\n", inputFile2);
        fclose(RawFile);
        fclose(CompFile);
        delete [] RawVideoBuffer;
        return 0;
    }

    IVF_HEADER ivfhComp;
    InitIVFHeader(&ivfhComp);
    fread(&ivfhComp, 1, sizeof(ivfhComp), CompFile);
    FormatIVFHeaderRead(&ivfhComp);

    YV12_BUFFER_CONFIG Comp_YV12;
    Comp_YV12.YWidth   = ivfhComp.width;
    Comp_YV12.YHeight  = ivfhComp.height;
    Comp_YV12.YStride  = ivfhComp.width + 2 * buffer_sz;
    Comp_YV12.UVWidth  = ivfhComp.width / 2;
    Comp_YV12.UVHeight = ivfhComp.height / 2;
    Comp_YV12.UVStride = Comp_YV12.YStride / 2;

    unsigned char *Comp_YBuffer			= new unsigned char [ivfhComp.width * ivfhComp.height];
    unsigned char *Comp_UBuffer			= new unsigned char [Comp_YV12.UVWidth * Comp_YV12.UVWidth];
    unsigned char *Comp_VBuffer			= new unsigned char [Comp_YV12.UVWidth * Comp_YV12.UVWidth];

    Comp_YV12.YBuffer			= &Comp_YBuffer[ivfhComp.width * ivfhComp.height];
    Comp_YV12.UBuffer			= &Comp_UBuffer[Comp_YV12.UVWidth * Comp_YV12.UVWidth];
    Comp_YV12.VBuffer			= &Comp_VBuffer[Comp_YV12.UVWidth * Comp_YV12.UVWidth];

    if (CompressedFrameOffset > 0) //Burn Frames untill Compressed frame offset reached - currently disabled by override of CompressedFrameOffset
    {
    }

    /////////////////////////////////////////////////////////////////////////
    ////////Printing////////
    if (printvar != 0)
    {
        printf("\n\n                        ---------Computing PSNR---------");
        fprintf(stderr, "\n\n                        ---------Computing PSNR---------");
    }

    if (printvar == 0)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 1)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
        fprintf(stderr, "\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 5)
    {
        printf("\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
        fprintf(stderr, "\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
        else
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
            fprintf(stderr, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
    }

    ////////////////////////

    vp8dx_Initialize();

    VP8D_CONFIG config2;
    config2.Width = Raw_YV12.YWidth;
    config2.Height = Raw_YV12.YHeight;
    config2.Version = 9;
    config2.postprocess = 0;
    config2.max_threads = 1;

    VP8D_PTR comp2;

    __int64 *timeStamp2 = new __int64;
    __int64 *timeEndStamp2 = new __int64;
    int deblock_level2 = 0;
    int noise_level2 = 0;
    int flags2 = 0;
    int currentFrame2 = 0;

    comp2 = vp8dx_CreateDecompressor(&config2);

    while (currentVideo1Frame <= frameCount - 1)
    {

        unsigned long lpdwFlags = 0;
        unsigned long lpckid = 0;
        long bytes1;
        long bytes2;

        //////////////////////Get YV12 Data For Compressed File//////////////////////
        IVF_FRAME_HEADER ivf_fhComp;

        if (!fread(&ivf_fhComp, 1, sizeof(ivf_fhComp), CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            return 0;
        }

        FormatFrameHeaderRead(ivf_fhComp);

        bytes2 = ivf_fhComp.frameSize;
        sumBytes2 += bytes2;

        unsigned char *CompBuff = new unsigned char[ivf_fhComp.frameSize];

        if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            delete [] CompBuff;
            return 0;
        }

        vp8dx_ReceiveCompressedData(comp2, bytes2, CompBuff, ivf_fhComp.timeStamp);

        if (vp8dx_GetRawFrame(comp2, &Comp_YV12, timeStamp2, timeEndStamp2, deblock_level2, noise_level2, flags2) < 0)
        {
            continue;
        }

        delete [] CompBuff;
        /////////////////////////////////////////////////////////////////////////////

        //////////////////////Get YV12 Data For Raw File//////////////////////
        IVF_FRAME_HEADER ivf_fhRaw;

        if (!fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), RawFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            return 0;
        }

        FormatFrameHeaderRead(ivf_fhRaw);

        bytes1 = ivf_fhRaw.frameSize;
        sumBytes += bytes1;

        memset(RawVideoBuffer, 0, ivfhRaw.width * ivfhRaw.height * 3);

        if (!fread(RawVideoBuffer, 1, ivf_fhRaw.frameSize, RawFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            return 0;
        }

        //////////////////////////////////////////////////////////////////////

        ///////////////////////////Preform PSNR Calc///////////////////////////////////
        double weight;
        double thisSsim = VP8_CalcSSIM2(&Raw_YV12, &Comp_YV12, 1, &weight);
        summedQuality += thisSsim * weight ;
        summedWeights += weight;

        double YPsnr;
        double UPsnr;
        double VPsnr;
        double SqError;
        double thisPsnr = VP8_CalcPSNR(&Raw_YV12, &Comp_YV12, &YPsnr, &UPsnr, &VPsnr, &SqError);

        summedYPsnr += YPsnr;
        summedUPsnr += UPsnr;
        summedVPsnr += VPsnr;
        summedPsnr += thisPsnr;
        sumSqError += SqError;
        ///////////////////////////////////////////////////////////////////////////////

        ////////Printing////////
        if (printvar == 1 || printvar == 5 || printvar == 0)
        {
            if (frameStats == 0)
            {
                printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                       currentVideo1Frame, frameCount
                      );
            }

            if (frameStats == 1)
            {
                printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                       currentVideo1Frame,
                       bytes1 * 8.0,
                       bytes2 * 8.0,
                       thisPsnr, 1.0 * YPsnr ,
                       1.0 * UPsnr ,
                       1.0 * VPsnr);

                fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                        currentVideo1Frame,
                        bytes1 * 8.0,
                        bytes2 * 8.0,
                        thisPsnr, 1.0 * YPsnr ,
                        1.0 * UPsnr ,
                        1.0 * VPsnr);
            }

            if (frameStats == 2)
            {
                printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                       currentVideo1Frame, frameCount
                      );

                fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                        currentVideo1Frame,
                        bytes1 * 8.0,
                        bytes2 * 8.0,
                        thisPsnr, 1.0 * YPsnr ,
                        1.0 * UPsnr ,
                        1.0 * VPsnr);
            }

            if (frameStats == 3)
            {
                printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                       currentVideo1Frame,
                       bytes1 * 8.0,
                       bytes2 * 8.0,
                       thisPsnr, 1.0 * YPsnr ,
                       1.0 * UPsnr ,
                       1.0 * VPsnr);
            }
        }

        ////////////////////////

        ++currentVideo1Frame;

    }

    vp8dx_RemoveDecompressor(comp2);
    //vp8dx_Shutdown();

    //Over All PSNR Calc
    double samples = 3.0 / 2 * frameCount * Raw_YV12.YWidth * Raw_YV12.YHeight;
    double avgPsnr = summedPsnr / frameCount;
    double totalPsnr = VP8_Mse2Psnr(samples, 255.0, sumSqError);
    double totalSSim = 100 * pow(summedQuality / summedWeights, 8.0);

    ////////Printing////////
    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length * ivfhRaw.rate / ivfhRaw.scale / 1000,
                   sumBytes2 * 8.0 / ivfhComp.length * ivfhComp.rate / ivfhComp.scale / 1000,
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

        }
        else
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length * ivfhRaw.rate / ivfhRaw.scale / 1000,
                   sumBytes2 * 8.0 / ivfhComp.length * ivfhComp.rate / ivfhComp.scale / 1000,
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

            fprintf(stderr, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                    sumBytes * 8.0 / ivfhRaw.length * ivfhRaw.rate / ivfhRaw.scale / 1000,
                    sumBytes2 * 8.0 / ivfhComp.length * ivfhComp.rate / ivfhComp.scale / 1000,
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr, totalSSim);
        }
    }

    if (printvar != 0)
    {
        printf("\n                        --------------------------------\n");
        fprintf(stderr, "\n                        --------------------------------\n");
    }

    ////////////////////////
    fclose(RawFile);
    fclose(CompFile);

    delete timeStamp2;
    delete timeEndStamp2;
    delete [] Comp_YBuffer;
    delete [] Comp_UBuffer;
    delete [] Comp_VBuffer;
    delete [] RawVideoBuffer;

    return totalPsnr;
}
double PostProcIVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double &SsimOut)
{
    double summedQuality = 0;
    double summedWeights = 0;
    double summedPsnr = 0;
    double summedYPsnr = 0;
    double summedUPsnr = 0;
    double summedVPsnr = 0;
    double sumSqError = 0;
    double sumBytes = 0;
    double sumBytes2 = 0;

    unsigned int currentVideo1Frame = 0;
    int RawFrameOffset = 0;
    int CompressedFrameOffset = 0;
    unsigned int maximumFrameCount = 0;

    forceUVswap = 0;

    ////////////////////////Initilize Raw File////////////////////////

    FILE *RawFile = fopen(inputFile1, "rb");

    if (RawFile == NULL)
    {
        printf("\nError Opening Raw File: %s\n", inputFile1);
        fprintf(stderr, "\nError Opening Raw File: %s\n", inputFile1);
        fclose(RawFile);
        return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    FormatIVFHeaderRead(&ivfhRaw);

    int buffer_sz = 32;
    unsigned int frameCount = ivfhRaw.length;

    unsigned char *RawVideoBuffer = new unsigned char[ivfhRaw.width*ivfhRaw.height*3];

    YV12_BUFFER_CONFIG Raw_YV12;
    Raw_YV12.YWidth   = ivfhRaw.width;
    Raw_YV12.YHeight  = ivfhRaw.height;
    Raw_YV12.YStride  = Raw_YV12.YWidth;
    Raw_YV12.UVWidth  = Raw_YV12.YWidth >> 1;
    Raw_YV12.UVHeight = Raw_YV12.YHeight >> 1;
    Raw_YV12.UVStride = Raw_YV12.YStride >> 1;
    Raw_YV12.BufferAlloc		= RawVideoBuffer;
    Raw_YV12.YBuffer			= RawVideoBuffer;
    Raw_YV12.UBuffer			= Raw_YV12.YBuffer + Raw_YV12.YWidth * Raw_YV12.YHeight;
    Raw_YV12.VBuffer			= Raw_YV12.UBuffer + Raw_YV12.UVWidth * Raw_YV12.UVHeight;

    if (RawFrameOffset > 0) //Burn Frames untill Raw frame offset reached - currently disabled by override of RawFrameOffset
    {
        for (int i = 0; i < RawFrameOffset; i++)
        {
        }
    }

    if (ivfhRaw.FourCC == 842094169)
    {
        forceUVswap = 0;   //if YV12 Do not swap Frames
    }

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.UBuffer;
        Raw_YV12.UBuffer = Raw_YV12.VBuffer;
        Raw_YV12.VBuffer = temp;
    }

    ////////////////////////Initilize Compressed File////////////////////////
    FILE *CompFile = fopen(inputFile2, "rb");

    if (CompFile == NULL)
    {
        printf("\nError Opening Compressed File: %s\n", inputFile2);
        fprintf(stderr, "\nError Opening Compressed File: %s\n", inputFile2);
        fclose(RawFile);
        fclose(CompFile);
        delete [] RawVideoBuffer;
        return 0;
    }

    IVF_HEADER ivfhComp;
    InitIVFHeader(&ivfhComp);
    fread(&ivfhComp, 1, sizeof(ivfhComp), CompFile);
    FormatIVFHeaderRead(&ivfhComp);

    YV12_BUFFER_CONFIG Comp_YV12;

    if (CompressedFrameOffset > 0) //Burn Frames untill Compressed frame offset reached - currently disabled by override of CompressedFrameOffset
    {
    }

    /////////////////////////////////////////////////////////////////////////
    ////////Printing////////
    if (printvar != 0)
    {
        printf("\n\n                        ---------Computing PSNR---------");
        fprintf(stderr, "\n\n                        ---------Computing PSNR---------");
    }

    if (printvar == 0)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 1)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
        fprintf(stderr, "\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 5)
    {
        printf("\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
        fprintf(stderr, "\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
        else
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
            fprintf(stderr, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
    }

    ////////////////////////

    __int64 *timeStamp2 = new __int64;
    __int64 *timeEndStamp2 = new __int64;
    int deblock_level2 = deblock_level;
    int noise_level2 = noise_level;
    int flags2 = flags;
    int currentFrame2 = 0;

    //////////////////////////////////////////New//////////////////////////////////////////
    on2_codec_ctx_t       decoder;
    on2_codec_iface_t       *iface = &on2_codec_vp8_algo;
    on2_codec_dec_cfg_t     cfg = {0};

    vp8_postproc_cfg_t ppcfg = {0};

    //if(noise_level != 0)
    //{
    //	ppcfg.PostProcFlag |= VP8_ADDNOISE;
    //	ppcfg.NoiseLevel = noise_level;

    //}
    //if(deblock_level != 0)
    //{
    //	ppcfg.PostProcFlag |= VP8_DEMACROBLOCK;
    //	ppcfg.DeblockingLevel = deblock_level;

    //}

    //ppcfg.PostProcFlag |= VP8_DEBLOCK;
    //ppcfg.PostProcFlag = flags;

    ppcfg.PostProcFlag = flags;
    ppcfg.NoiseLevel = noise_level;
    ppcfg.DeblockingLevel = deblock_level;

    if (on2_codec_dec_init(&decoder, iface, &cfg, ON2_CODEC_USE_POSTPROC))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (on2_codec_control(&decoder, VP8_SET_POSTPROC, &ppcfg) != 0)
    {
        printf("Failed to update decoder post processor settings\n");
    }

    /////////////////////Setup Temp YV12 Buffer to Resize if nessicary/////////////////////
    vp8_scale_machine_specific_config();

    YV12_BUFFER_CONFIG Temp_YV12;
    memset(&Temp_YV12, 0, sizeof(Temp_YV12));
    vp8_yv12_alloc_frame_buffer(&Temp_YV12, ivfhRaw.width, ivfhRaw.height, 0);
    ///////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////New//////////////////////////////////////////

    while (currentVideo1Frame <= frameCount - 1)
    {

        unsigned long lpdwFlags = 0;
        unsigned long lpckid = 0;
        long bytes1;
        long bytes2;

        //////////////////////Get YV12 Data For Compressed File//////////////////////
        IVF_FRAME_HEADER ivf_fhComp;

        if (!fread(&ivf_fhComp, 1, sizeof(ivf_fhComp), CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] RawVideoBuffer;
            return 0;
        }

        FormatFrameHeaderRead(ivf_fhComp);

        bytes2 = ivf_fhComp.frameSize;
        sumBytes2 += bytes2;

        char *CompBuff = new char[ivf_fhComp.frameSize * 2];

        if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] RawVideoBuffer;
            delete [] CompBuff;
            return 0;
        }

        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;

        //on2_codec_control(&decoder, VP8_SET_POSTPROC, &ppcfg);

        if (on2_codec_decode(&decoder, (uint8_t *) CompBuff, ivf_fhComp.frameSize, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));
        }

        img = on2_codec_get_frame(&decoder, &iter);

        if (img)
        {

            image2yuvconfig(img, &Comp_YV12);

            if (img->d_w != ivfhRaw.width || img->d_h != ivfhRaw.height) //if frame not correct size resize it for psnr
            {
                int GCDInt1 = GCD(img->d_w, ivfhRaw.width);
                int GCDInt2 = GCD(img->d_h, ivfhRaw.height);

                vp8_yv12_scale_or_center(&Comp_YV12, &Temp_YV12, ivfhRaw.width, ivfhRaw.height, 0, (ivfhRaw.width / GCDInt1), (img->d_w / GCDInt1), (ivfhRaw.height / GCDInt2), (img->d_h / GCDInt2));

                Comp_YV12 = Temp_YV12;
            }

            delete [] CompBuff;
            /////////////////////////////////////////////////////////////////////////////

            //////////////////////Get YV12 Data For Raw File//////////////////////
            IVF_FRAME_HEADER ivf_fhRaw;

            if (!fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), RawFile))
            {
                printf("\nError Computing PSNR\n");
                fprintf(stderr, "\nError Computing PSNR\n");
                fclose(RawFile);
                fclose(CompFile);
                delete timeStamp2;
                delete timeEndStamp2;
                delete [] RawVideoBuffer;
                return 0;
            }

            FormatFrameHeaderRead(ivf_fhRaw);

            bytes1 = ivf_fhRaw.frameSize;
            sumBytes += bytes1;

            memset(RawVideoBuffer, 0, ivfhRaw.width * ivfhRaw.height * 3);

            if (!fread(RawVideoBuffer, 1, ivf_fhRaw.frameSize, RawFile))
            {
                printf("\nError Computing PSNR\n");
                fprintf(stderr, "\nError Computing PSNR\n");
                fclose(RawFile);
                fclose(CompFile);
                delete timeStamp2;
                delete timeEndStamp2;
                delete [] RawVideoBuffer;
                return 0;
            }

            //////////////////////////////////////////////////////////////////////

            ///////////////////////////Preform PSNR Calc///////////////////////////////////
            double weight;
            double thisSsim = VP8_CalcSSIM2(&Raw_YV12, &Comp_YV12, 1, &weight);
            summedQuality += thisSsim * weight ;
            summedWeights += weight;

            double YPsnr;
            double UPsnr;
            double VPsnr;
            double SqError;
            double thisPsnr = VP8_CalcPSNR(&Raw_YV12, &Comp_YV12, &YPsnr, &UPsnr, &VPsnr, &SqError);

            summedYPsnr += YPsnr;
            summedUPsnr += UPsnr;
            summedVPsnr += VPsnr;
            summedPsnr += thisPsnr;
            sumSqError += SqError;
            ///////////////////////////////////////////////////////////////////////////////

            ////////Printing////////
            if (printvar == 1 || printvar == 5 || printvar == 0)
            {
                if (frameStats == 0)
                {
                    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                           8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                           currentVideo1Frame, frameCount
                          );
                }

                if (frameStats == 1)
                {
                    printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                           currentVideo1Frame,
                           bytes1 * 8.0,
                           bytes2 * 8.0,
                           thisPsnr, 1.0 * YPsnr ,
                           1.0 * UPsnr ,
                           1.0 * VPsnr);

                    fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                            currentVideo1Frame,
                            bytes1 * 8.0,
                            bytes2 * 8.0,
                            thisPsnr, 1.0 * YPsnr ,
                            1.0 * UPsnr ,
                            1.0 * VPsnr);
                }

                if (frameStats == 2)
                {
                    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                           8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                           currentVideo1Frame, frameCount
                          );

                    fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                            currentVideo1Frame,
                            bytes1 * 8.0,
                            bytes2 * 8.0,
                            thisPsnr, 1.0 * YPsnr ,
                            1.0 * UPsnr ,
                            1.0 * VPsnr);
                }

                if (frameStats == 3)
                {
                    printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                           currentVideo1Frame,
                           bytes1 * 8.0,
                           bytes2 * 8.0,
                           thisPsnr, 1.0 * YPsnr ,
                           1.0 * UPsnr ,
                           1.0 * VPsnr);
                }
            }

            ////////////////////////

            ++currentVideo1Frame;
        }
        else
        {
            delete [] CompBuff;
        }

    }

    on2_codec_destroy(&decoder);

    //Over All PSNR Calc
    double samples = 3.0 / 2 * frameCount * Raw_YV12.YWidth * Raw_YV12.YHeight;
    double avgPsnr = summedPsnr / frameCount;
    double totalPsnr = VP8_Mse2Psnr(samples, 255.0, sumSqError);
    double totalSSim = 100 * pow(summedQuality / summedWeights, 8.0);

    ////////Printing////////
    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,						//divided by two added when rate doubled to handle doubling of timestamp
                   sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,					//divided by two added when rate doubled to handle doubling of timestamp
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

        }
        else
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,						//divided by two added when rate doubled to handle doubling of timestamp
                   sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,					//divided by two added when rate doubled to handle doubling of timestamp
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

            fprintf(stderr, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                    sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,						//divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,					//divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr, totalSSim);
        }
    }

    if (printvar != 0)
    {
        printf("\n                        --------------------------------\n");
        fprintf(stderr, "\n                        --------------------------------\n");
    }

    ////////////////////////
    fclose(RawFile);
    fclose(CompFile);

    delete timeStamp2;
    delete timeEndStamp2;
    delete [] RawVideoBuffer;

    return totalPsnr;
}
double PostProcIVFPSNR_CORE(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double &SsimOut)
{
    double summedQuality = 0;
    double summedWeights = 0;
    double summedPsnr = 0;
    double summedYPsnr = 0;
    double summedUPsnr = 0;
    double summedVPsnr = 0;
    double sumSqError = 0;
    double sumBytes = 0;
    double sumBytes2 = 0;

    unsigned int currentVideo1Frame = 0;
    int CompressedFrameOffset = 0;
    unsigned int maximumFrameCount = 0;
    int RawFrameOffset = 0;

    ////////////////////////Initilize Raw File////////////////////////

    FILE *RawFile = fopen(inputFile1, "rb");

    if (RawFile == NULL)
    {
        printf("\nError Opening Raw File: %s\n", inputFile1);
        fprintf(stderr, "\nError Opening Raw File: %s\n", inputFile1);
        fclose(RawFile);
        return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    FormatIVFHeaderRead(&ivfhRaw);

    int buffer_sz = 32;
    unsigned int frameCount = ivfhRaw.length;

    unsigned char *RawVideoBuffer = new unsigned char[ivfhRaw.width*ivfhRaw.height*3];

    YV12_BUFFER_CONFIG Raw_YV12;
    Raw_YV12.YWidth   = ivfhRaw.width;
    Raw_YV12.YHeight  = ivfhRaw.height;
    Raw_YV12.YStride  = Raw_YV12.YWidth;
    Raw_YV12.UVWidth  = Raw_YV12.YWidth >> 1;
    Raw_YV12.UVHeight = Raw_YV12.YHeight >> 1;
    Raw_YV12.UVStride = Raw_YV12.YStride >> 1;
    Raw_YV12.BufferAlloc		= RawVideoBuffer;
    Raw_YV12.YBuffer			= RawVideoBuffer;
    Raw_YV12.UBuffer			= Raw_YV12.YBuffer + Raw_YV12.YWidth * Raw_YV12.YHeight;
    Raw_YV12.VBuffer			= Raw_YV12.UBuffer + Raw_YV12.UVWidth * Raw_YV12.UVHeight;

    if (RawFrameOffset > 0) //Burn Frames untill Raw frame offset reached - currently disabled by override of RawFrameOffset
    {
        for (int i = 0; i < RawFrameOffset; i++)
        {
        }
    }

    //////////////////////////////////////////////////////////////////
    if (ivfhRaw.FourCC == 842094169)
    {
        forceUVswap = 0;   //if YV12 Do not swap Frames
    }

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.UBuffer;
        Raw_YV12.UBuffer = Raw_YV12.VBuffer;
        Raw_YV12.VBuffer = temp;
    }

    ///////////////////////////////////////////////////////////////////////////////
    ////////////////////////Initilize Compressed File////////////////////////
    FILE *CompFile = fopen(inputFile2, "rb");

    if (CompFile == NULL)
    {
        printf("\nError Opening Compressed File: %s\n", inputFile2);
        fprintf(stderr, "\nError Opening Compressed File: %s\n", inputFile2);
        fclose(RawFile);
        fclose(CompFile);
        delete [] RawVideoBuffer;
        return 0;
    }

    IVF_HEADER ivfhComp;
    InitIVFHeader(&ivfhComp);
    fread(&ivfhComp, 1, sizeof(ivfhComp), CompFile);
    FormatIVFHeaderRead(&ivfhComp);

    YV12_BUFFER_CONFIG Comp_YV12;
    Comp_YV12.YWidth   = ivfhComp.width;
    Comp_YV12.YHeight  = ivfhComp.height;
    Comp_YV12.YStride  = ivfhComp.width + 2 * buffer_sz;
    Comp_YV12.UVWidth  = ivfhComp.width / 2;
    Comp_YV12.UVHeight = ivfhComp.height / 2;
    Comp_YV12.UVStride = Comp_YV12.YStride / 2;

    unsigned char *Comp_YBuffer			= new unsigned char [ivfhComp.width * ivfhComp.height];
    unsigned char *Comp_UBuffer			= new unsigned char [Comp_YV12.UVWidth * Comp_YV12.UVWidth];
    unsigned char *Comp_VBuffer			= new unsigned char [Comp_YV12.UVWidth * Comp_YV12.UVWidth];

    Comp_YV12.YBuffer			= &Comp_YBuffer[ivfhComp.width * ivfhComp.height];
    Comp_YV12.UBuffer			= &Comp_UBuffer[Comp_YV12.UVWidth * Comp_YV12.UVWidth];
    Comp_YV12.VBuffer			= &Comp_VBuffer[Comp_YV12.UVWidth * Comp_YV12.UVWidth];

    if (CompressedFrameOffset > 0) //Burn Frames untill Compressed frame offset reached - currently disabled by override of CompressedFrameOffset
    {
    }

    /////////////////////////////////////////////////////////////////////////
    ////////Printing////////
    if (printvar != 0)
    {
        printf("\n\n                        ---------Computing PSNR---------");
        fprintf(stderr, "\n\n                        ---------Computing PSNR---------");
    }

    if (printvar == 0)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 1)
    {
        printf("\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
        fprintf(stderr, "\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 5)
    {
        printf("\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
        fprintf(stderr, "\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
        else
        {
            printf("File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
            fprintf(stderr, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
    }

    ////////////////////////

    vp8dx_Initialize();

    VP8D_CONFIG config2;
    config2.Width = Raw_YV12.YWidth;
    config2.Height = Raw_YV12.YHeight;
    config2.Version = 9;
    config2.postprocess = 0;
    config2.max_threads = 1;

    VP8D_PTR comp2;

    __int64 *timeStamp2 = new __int64;
    __int64 *timeEndStamp2 = new __int64;
    int deblock_level2 = deblock_level;
    int noise_level2 = noise_level;
    int flags2 = flags;
    int currentFrame2 = 0;

    comp2 = vp8dx_CreateDecompressor(&config2);

    while (currentVideo1Frame <= frameCount - 2)
    {

        unsigned long lpdwFlags = 0;
        unsigned long lpckid = 0;
        long bytes1;
        long bytes2;

        //////////////////////Get YV12 Data For Compressed File//////////////////////
        IVF_FRAME_HEADER ivf_fhComp;

        if (!fread(&ivf_fhComp, 1, sizeof(ivf_fhComp), CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            return 0;
        }

        FormatFrameHeaderRead(ivf_fhComp);

        bytes2 = ivf_fhComp.frameSize;

        unsigned char *CompBuff = new unsigned char[ivf_fhComp.frameSize];

        if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            delete [] CompBuff;
            return 0;
        }

        //Temp test
        vp8dx_ReceiveCompressedData(comp2, bytes2, CompBuff, ivf_fhComp.timeStamp);

        if (vp8dx_GetRawFrame(comp2, &Comp_YV12, timeStamp2, timeEndStamp2, deblock_level2, noise_level2, flags2) < 0)
        {
            continue;
        }

        delete [] CompBuff;
        /////////////////////////////////////////////////////////////////////////////

        //////////////////////Get YV12 Data For Raw File//////////////////////
        IVF_FRAME_HEADER ivf_fhRaw;

        if (!fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), RawFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            return 0;
        }

        FormatFrameHeaderRead(ivf_fhRaw);

        bytes1 = ivf_fhRaw.frameSize;

        memset(RawVideoBuffer, 0, ivfhRaw.width * ivfhRaw.height * 3);

        if (!fread(RawVideoBuffer, 1, ivf_fhRaw.frameSize, RawFile))
        {
            printf("\nError Computing PSNR\n");
            fprintf(stderr, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] Comp_YBuffer;
            delete [] Comp_UBuffer;
            delete [] Comp_VBuffer;
            delete [] RawVideoBuffer;
            return 0;
        }

        //////////////////////////////////////////////////////////////////////

        ///////////////////////////Preform PSNR Calc///////////////////////////////////
        double weight;
        double thisSsim = VP8_CalcSSIM2(&Raw_YV12, &Comp_YV12, 1, &weight);
        //double thisSsim = VP8_CalcSSIM2(&Raw_YV12,&Raw_YV12,1,&weight);
        summedQuality += thisSsim * weight ;
        summedWeights += weight;

        double YPsnr;
        double UPsnr;
        double VPsnr;
        double SqError;
        double thisPsnr = VP8_CalcPSNR(&Raw_YV12, &Comp_YV12, &YPsnr, &UPsnr, &VPsnr, &SqError);
        //double thisPsnr = VP8_CalcPSNR (&Raw_YV12,&Raw_YV12,&YPsnr,&UPsnr,&VPsnr,&SqError);

        summedYPsnr += YPsnr;
        summedUPsnr += UPsnr;
        summedVPsnr += VPsnr;
        summedPsnr += thisPsnr;
        sumSqError += SqError;
        ///////////////////////////////////////////////////////////////////////////////

        ////////Printing////////
        if (printvar == 1 || printvar == 5 || printvar == 0)
        {
            if (frameStats == 0)
            {
                printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                       currentVideo1Frame, frameCount
                      );
            }

            if (frameStats == 1)
            {
                printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                       currentVideo1Frame,
                       bytes1 * 8.0,
                       bytes2 * 8.0,
                       thisPsnr, 1.0 * YPsnr ,
                       1.0 * UPsnr ,
                       1.0 * VPsnr);

                fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                        currentVideo1Frame,
                        bytes1 * 8.0,
                        bytes2 * 8.0,
                        thisPsnr, 1.0 * YPsnr ,
                        1.0 * UPsnr ,
                        1.0 * VPsnr);
            }

            if (frameStats == 2)
            {
                printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                       currentVideo1Frame, frameCount
                      );

                fprintf(stderr, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                        currentVideo1Frame,
                        bytes1 * 8.0,
                        bytes2 * 8.0,
                        thisPsnr, 1.0 * YPsnr ,
                        1.0 * UPsnr ,
                        1.0 * VPsnr);
            }

            if (frameStats == 3)
            {
                printf("F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                       currentVideo1Frame,
                       bytes1 * 8.0,
                       bytes2 * 8.0,
                       thisPsnr, 1.0 * YPsnr ,
                       1.0 * UPsnr ,
                       1.0 * VPsnr);
            }
        }

        ////////////////////////

        ++currentVideo1Frame;

        sumBytes += bytes1;
        sumBytes2 += bytes2;
    }

    vp8dx_RemoveDecompressor(comp2);
    //vp8dx_Shutdown();

    //Over All PSNR Calc
    double samples = 3.0 / 2 * frameCount * Raw_YV12.YWidth * Raw_YV12.YHeight;
    double avgPsnr = summedPsnr / frameCount;
    double totalPsnr = VP8_Mse2Psnr(samples, 255.0, sumSqError);
    double totalSSim = 100 * pow(summedQuality / summedWeights, 8.0);

    ////////Printing////////
    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length * ivfhRaw.rate / ivfhRaw.scale / 1000,
                   sumBytes2 * 8.0 / ivfhComp.length * ivfhComp.rate / ivfhComp.scale / 1000,
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

        }
        else
        {
            printf("\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                   sumBytes * 8.0 / ivfhRaw.length * ivfhRaw.rate / ivfhRaw.scale / 1000,
                   sumBytes2 * 8.0 / ivfhComp.length * ivfhComp.rate / ivfhComp.scale / 1000,
                   avgPsnr, 1.0 * summedYPsnr / frameCount,
                   1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                   totalPsnr, totalSSim);

            fprintf(stderr, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, SSIM: %8.2f\n",
                    sumBytes * 8.0 / ivfhRaw.length * ivfhRaw.rate / ivfhRaw.scale / 1000,
                    sumBytes2 * 8.0 / ivfhComp.length * ivfhComp.rate / ivfhComp.scale / 1000,
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr, totalSSim);
        }
    }

    if (printvar != 0)
    {
        printf("\n                        --------------------------------\n");
        fprintf(stderr, "\n                        --------------------------------\n");
    }

    ////////////////////////

    fclose(RawFile);
    fclose(CompFile);

    delete timeStamp2;
    delete timeEndStamp2;
    delete [] Comp_YBuffer;
    delete [] Comp_UBuffer;
    delete [] Comp_VBuffer;
    delete [] RawVideoBuffer;

    return totalPsnr;
}
int PSNRSelect(char *inFile, char *outFile)
{
    int PSNRToggle = 1;
    return PSNRToggle;
}
double IVFDataRate(char *inputFile, int DROuputSel)
{
    if (DROuputSel != 2)
    {
        printf("\n--------Data Rate-------\n");
        fprintf(stderr, "\n--------Data Rate-------\n");

        char FileNameOnly[256];

        FileName(inputFile, FileNameOnly);
        printf("Data Rate for: %s", FileNameOnly);
        fprintf(stderr, "Data Rate for: %s", FileNameOnly);
    }

    ///////////////////////////////////
    long PosSize = FileSize2(inputFile);
    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        printf("\nCan not open Data Rate Input file");
        fprintf(stderr, "\nCan not open Data Rate Input file");
        fclose(in);
        return 0;
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);
    unsigned char *outputVideoBuffer = new unsigned char [ivfhRaw.width * ivfhRaw.height];

    /*printf( "IVF DataRate\n\n"
    "FILE HEADER \n\n"
    "File Header            - %c%c%c%c \n"
    "File Format Version    - %i \n"
    "File Header Size       - %i \n"
    "Video Data FourCC      - %i \n"
    "Video Image Width      - %i \n"
    "Video Image Height     - %i \n"
    "Frame Rate Rate        - %i \n"
    "Frame Rate Scale       - %i \n"
    "Video Length in Frames - %i \n"
    "Unused                 - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    frameCount = ivfhRaw.length;

    long nSamples = frameCount;
    long lRateNum = ivfhRaw.rate / 2; //divided by two added when rate doubled to handle doubling of timestamp
    long lRateDenom = ivfhRaw.scale;

    long nSamplesPerBlock = 1;

    long nBytes = 0;
    long nBytesMin = 999999;
    long nBytesMax = 0;

    fpos_t position;
    fgetpos(in, &position);
    cout << "\n";

    while (currentVideoFrame < frameCount)
    {
        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        nBytes = nBytes + ivf_fhRaw.frameSize;

        if (ivf_fhRaw.frameSize < nBytesMin)
        {
            nBytesMin = ivf_fhRaw.frameSize;
        }

        if (ivf_fhRaw.frameSize > nBytesMax)
        {
            nBytesMax = ivf_fhRaw.frameSize;
        }

        fseek(in , ivf_fhRaw.frameSize , SEEK_CUR);

        currentVideoFrame ++;
    }

    double dRateFactor = static_cast<double>(lRateNum) / static_cast<double>(lRateDenom) * static_cast<double>(8) / static_cast<double>(1000);

    double Avg = (double)nBytes * (double)dRateFactor / (double)nSamples;
    double Min = (double)nBytesMin * (double)dRateFactor / (double)nSamplesPerBlock;
    double Max = (double)nBytesMax * (double)dRateFactor / (double)nSamplesPerBlock;
    double File = (double)PosSize * (double)dRateFactor / (double)nSamples;

    printf("\nData rate for frames 0..%i\n", frameCount - 1);
    printf("Average %*.2f kb/s\n", 10, Avg);
    printf("Min     %*.2f kb/s\n", 10, Min);
    printf("Max     %*.2f kb/s\n", 10, Max);
    printf("File    %*.2f kb/s\n", 10, File);

    if (DROuputSel == 1)
    {
        fprintf(stderr, "\nData rate for frames 0..%i\n", frameCount - 1);
        fprintf(stderr, "Average %*.2f kb/s\n", 10, Avg);
        fprintf(stderr, "Min     %*.2f kb/s\n", 10, Min);
        fprintf(stderr, "Max     %*.2f kb/s\n", 10, Max);
        fprintf(stderr, "File    %*.2f kb/s\n", 10, File);
    }

    fclose(in);

    if (DROuputSel != 2)
    {
        printf("\n------------------------\n");
        fprintf(stderr, "\n------------------------\n");
    }

    delete [] outputVideoBuffer;

    return Avg;
}

int IVFCheckPBM(char *inputFile, int bitRate, int maxBuffer, int preBuffer)
{
    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        printf("\nInput file does not exist");
        fprintf(stderr, "\nInput file does not exist");
        fclose(in);
        return 0;
    }

    ///////////////////////////////////
    //long PosSize = FileSize2(inputFile);

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);

    IVF_FRAME_HEADER ivf_fhRaw;

    frameCount = ivfhRaw.length;
    int nFrameFail = 0;

    bool checkOverrun = false;
    double secondsperframe = ((double)ivfhRaw.scale / (double)ivfhRaw.rate) * 2;//(the *2 is due to scale being doubled for extra frame padding)
    double bitsAddedPerFrame = ((bitRate * 1024 * secondsperframe));
    double bitsInBuffer = preBuffer * 0.001 * bitRate * 1024;
    double maxBitsInBuffer = maxBuffer * 0.001 * bitRate * 1024;

    fpos_t position;
    fgetpos(in, &position);

    while (currentVideoFrame < frameCount)
    {
        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        bitsInBuffer += bitsAddedPerFrame;
        bitsInBuffer -= ivf_fhRaw.frameSize * 8;

        if (bitsInBuffer < 0.)
        {
            fclose(in);
            return currentVideoFrame;
        }

        if (bitsInBuffer > maxBitsInBuffer)
        {
            if (checkOverrun)
            {
                fclose(in);
                return currentVideoFrame;
            }
            else
            {
                bitsInBuffer = maxBitsInBuffer;
            }
        }

        fseek(in , ivf_fhRaw.frameSize , SEEK_CUR);

        currentVideoFrame ++;
    }

    fclose(in);
    return -11;
}

int IVFCheckPBMThreshold(char *inputFile, double bitRate, int maxBuffer, int preBuffer, int optimalbuffer, int Threshold)
{
    string ResizeInStr = inputFile;
    ResizeInStr.erase(ResizeInStr.length() - 4, 4);
    ResizeInStr.append("_CheckPBMThresh.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", ResizeInStr.c_str());

    FILE *out;

    int PrintSwitch = 1;

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");
    }

    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        printf("\nInput file does not exist");
        fprintf(stderr, "\nInput file does not exist");
        fclose(in);
        return 0;
    }

    ///////////////////////////////////
    //long PosSize = FileSize2(inputFile);

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);

    IVF_FRAME_HEADER ivf_fhRaw;

    frameCount = ivfhRaw.length;
    int nFrameFail = 0;

    bool checkOverrun = false;
    //double secondsperframe = ((double)ivfhRaw.scale / (double)ivfhRaw.rate) * 2;//(the *2 is due to scale being doubled for extra frame padding)
    //double bitsAddedPerFrame = ((bitRate * 1024 * secondsperframe));
    //double bitsInBuffer = (double)preBuffer * 0.001 * (double)bitRate * 1024.0;
    //double maxBitsInBuffer = (double)maxBuffer * 0.001 * (double)bitRate * 1024.0;

    double secondsperframe = ((double)ivfhRaw.scale / (double)ivfhRaw.rate) * 2;//(the *2 is due to scale being doubled for extra frame padding)
    double bitsAddedPerFrame = ((bitRate * 1024 * secondsperframe));
    double bitsInBuffer = (double)preBuffer * 0.001 * (double)bitRate * 1024.0;
    double maxBitsInBuffer = (double)maxBuffer * 0.001 * (double)bitRate * 1024.0;

    fpos_t position;
    fgetpos(in, &position);

    while (currentVideoFrame < frameCount)
    {
        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        bitsInBuffer += bitsAddedPerFrame;
        bitsInBuffer -= ivf_fhRaw.frameSize * 8;

        //fprintf(out, "Frame: %4i Bits in Buffer: %f MaxBitsin Buffer %f ", currentVideoFrame, bitsInBuffer, maxBitsInBuffer);


        if (bitsInBuffer < 0.)
        {
            //fclose(in);
            //return currentVideoFrame;
        }

        double optimalbufferFloat = optimalbuffer / 1000.0;
        //double MaxPercentBuffer = (maxBitsInBuffer*Threshold*.01);
        double MaxPercentBuffer = (((double)Threshold * optimalbufferFloat) / 100.0) * (double)bitRate * 1024.0;

        //printf("\n %f %f %i", bitsInBuffer, MaxPercentBuffer,currentVideoFrame);
        if (bitsInBuffer < MaxPercentBuffer)
        {
            //printf(" 1");
            fprintf(out, "%i %i\n", currentVideoFrame, 1);
            //fprintf(out, "- Threshold %i Reached ", Threshold);
        }
        else
        {
            //printf(" 0");
            fprintf(out, "%i %i\n", currentVideoFrame, 0);
        }

        if (bitsInBuffer > maxBitsInBuffer)
        {
            if (checkOverrun)
            {
                //fclose(in);
                //return currentVideoFrame;
            }
            else
            {
                //fprintf(out, "Over Run", Threshold);
                bitsInBuffer = maxBitsInBuffer;
            }
        }

        fseek(in , ivf_fhRaw.frameSize , SEEK_CUR);

        currentVideoFrame ++;
    }

    fclose(in);

    if (PrintSwitch == 1)
    {
        fclose(out);
    }

    //printf("\n NO UNDERRUN DETECTED \n");
    return -11;
}

int IVFCheckPBMThreshold_orig(char *inputFile, double bitRate, int maxBuffer, int preBuffer, int Threshold)
{
    string ResizeInStr = inputFile;
    ResizeInStr.erase(ResizeInStr.length() - 4, 4);
    ResizeInStr.append("_CheckPBMThresh.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", ResizeInStr.c_str());

    FILE *out;

    int PrintSwitch = 1;

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");
    }

    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        printf("\nInput file does not exist");
        fprintf(stderr, "\nInput file does not exist");
        fclose(in);
        return 0;
    }

    ///////////////////////////////////
    //long PosSize = FileSize2(inputFile);

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);

    IVF_FRAME_HEADER ivf_fhRaw;

    frameCount = ivfhRaw.length;
    int nFrameFail = 0;

    bool checkOverrun = false;
    double secondsperframe = ((double)ivfhRaw.scale / (double)ivfhRaw.rate) * 2;//(the *2 is due to scale being doubled for extra frame padding)
    double bitsAddedPerFrame = ((bitRate * 1024 * secondsperframe));
    double bitsInBuffer = preBuffer * 0.001 * bitRate * 1024;
    double maxBitsInBuffer = maxBuffer * 0.001 * bitRate * 1024;

    fpos_t position;
    fgetpos(in, &position);

    while (currentVideoFrame < frameCount)
    {
        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        bitsInBuffer += bitsAddedPerFrame;
        bitsInBuffer -= ivf_fhRaw.frameSize * 8;

        fprintf(out, "Frame: %4i Bits in Buffer: %f MaxBitsin Buffer %f ", currentVideoFrame, bitsInBuffer, maxBitsInBuffer);

        if (bitsInBuffer < 0.)
        {
            fclose(in);
            return currentVideoFrame;
        }

        double MaxPercentBuffer = (maxBitsInBuffer * Threshold * .01);

        if (bitsInBuffer <= MaxPercentBuffer)
        {
            fprintf(out, "- Threshold %i Reached ", Threshold);
        }

        if (bitsInBuffer > maxBitsInBuffer)
        {
            if (checkOverrun)
            {
                fclose(in);
                return currentVideoFrame;
            }
            else
            {
                fprintf(out, "Over Run", Threshold);
                bitsInBuffer = maxBitsInBuffer;
            }
        }

        fprintf(out, "\n");
        fseek(in , ivf_fhRaw.frameSize , SEEK_CUR);

        currentVideoFrame ++;
    }

    fclose(in);

    if (PrintSwitch == 1)
    {
        fclose(out);
    }

    printf("\n NO UNDERRUN DETECTED \n");
    return -11;
}

int FauxCompress()
{
#ifdef API
    on2_codec_ctx_t        encoder;
    on2_codec_iface_t     *iface = &on2_enc_vp8_algo;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_enc_config_default(iface, &cfg, 0);
    on2_codec_enc_init(&encoder, iface, &cfg, 0);
    on2_codec_destroy(&encoder);
#else
    VP8_CONFIG opt;
    VP8DefaultParms(opt);
    VP8_Initialize();
    VP8_PTR optr = VP8_CreateCompressor(&opt);
    VP8_RemoveCompressor(&optr);
    VP8_Shutdown();
#endif

    return 1;
}
int FauxDecompress(char *inputChar)
{
#ifdef API

    on2_dec_ctx_t          decoder;
    on2_codec_iface_t     *iface = &on2_codec_vp8_dx_algo;
    on2_codec_dec_cfg_t     cfg;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    int					   width, height;
    FILE *infile = fopen(inputChar, "rb");

    char raw_hdr[32];
    int is_ivf = 0;

    if (fread(raw_hdr, 1, 32, infile) == 32)
    {
        if (raw_hdr[0] == 'D' && raw_hdr[1] == 'K'
            && raw_hdr[2] == 'I' && raw_hdr[3] == 'F')
        {
            is_ivf = 1;
            ///////////////////Write Header Info///////////////////
            IVF_HEADER ivfhRaw;
            memcpy(&ivfhRaw, raw_hdr, 32);
            ivfhRaw.FourCC = 808596553;
            width = ivfhRaw.width;
            height = ivfhRaw.height;
            ///////////////////////////////////////////////////////
        }
    }

    on2_codec_dec_init(&decoder, iface, &cfg, 0);
    read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf);
    on2_codec_decode(&decoder, buf, buf_sz, NULL, 0);
    on2_codec_destroy(&decoder);
    free(buf);
    fclose(infile);

#else
    vp8dx_Initialize();
    VP8D_CONFIG oxcf;
    VP8D_PTR optr = vp8dx_CreateDecompressor(&oxcf);
    vp8dx_RemoveDecompressor(optr);
    //vp8dx_Shutdown();
#endif

    return 1;
}
//---------------------------------------------------------IVF DShow------------------------------------------------------------------------
#ifdef DSHOW
int CompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck)
{
    printf("\nDSHOW - Compressing IVF File to VPX IVF File: \n");
    fprintf(stderr, "\nDSHOW - Compressing IVF File to VPX IVF File: \n");

    char TesterExePathout[255];
    CopyCharArray(TesterExePath, TesterExePathout);

    string IVFMuxerStr = "\"";
    string IVFSourceStr = "\"";
    string On2VP8DecoderStr = "\"";
    string On2VP8Encoder = "\"";

    IVFMuxerStr.append(TesterExePathout);
    IVFSourceStr.append(TesterExePathout);
    On2VP8DecoderStr.append(TesterExePathout);
    On2VP8Encoder.append(TesterExePathout);

    IVFMuxerStr.erase(IVFMuxerStr.length() - 4, 4);
    IVFSourceStr.erase(IVFSourceStr.length() - 4, 4);
    On2VP8DecoderStr.erase(On2VP8DecoderStr.length() - 4, 4);
    On2VP8Encoder.erase(On2VP8Encoder.length() - 4, 4);

    //printf("IVFMuxerStr: %s\n",IVFMuxerStr.c_str());
    //printf("IVFSourceStr: %s\n",IVFSourceStr.c_str());
    //printf("SystemOn2VP8DecoderStr: %s\n",On2VP8DecoderStr.c_str());
    //printf("SystemOn2VP8Encoder: %s\n",On2VP8Encoder.c_str());

    IVFMuxerStr.append("_DLL\\IVFMuxer.dll");
    IVFSourceStr.append("_DLL\\IVFSource.dll");
    On2VP8DecoderStr.append("_DLL\\On2VP8Decoder.dll");
    On2VP8Encoder.append("_DLL\\On2VP8Encoder.dll");

    string SystemIVFMuxerStr = "\"Regsvr32.exe /s ";
    string SystemIVFSourceStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8DecoderStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8Encoder = "\"Regsvr32.exe /s ";

    SystemIVFMuxerStr.append(IVFMuxerStr);
    SystemIVFSourceStr.append(IVFSourceStr);
    SystemOn2VP8DecoderStr.append(On2VP8DecoderStr);
    SystemOn2VP8Encoder.append(On2VP8Encoder);

    SystemIVFMuxerStr.append("\"\"");
    SystemIVFSourceStr.append("\"\"");
    SystemOn2VP8DecoderStr.append("\"\"");
    SystemOn2VP8Encoder.append("\"\"");

    system(SystemIVFMuxerStr.c_str());
    system(SystemIVFSourceStr.c_str());
    system(SystemOn2VP8DecoderStr.c_str());
    system(SystemOn2VP8Encoder.c_str());

    printf("%s\n", SystemIVFMuxerStr.c_str());
    printf("%s\n", SystemIVFSourceStr.c_str());
    printf("%s\n", SystemOn2VP8DecoderStr.c_str());
    printf("%s\n", SystemOn2VP8Encoder.c_str());

    //fprintf(stderr, "%s\n",SystemIVFMuxerStr.c_str());
    //fprintf(stderr, "%s\n",SystemIVFSourceStr.c_str());
    //fprintf(stderr, "%s\n",SystemOn2VP8DecoderStr.c_str());
    //fprintf(stderr, "%s\n",SystemOn2VP8Encoder.c_str());

    //////////////////////////////////////Set Fpf File/////////////////////////////////////////
    string UniqueFirstPassFile = outputFile2;
    UniqueFirstPassFile.erase(UniqueFirstPassFile.length() - 4, 4);
    UniqueFirstPassFile.append(".fpf");
    //	strcpy(oxcf.FirstPassFile, UniqueFirstPassFile.c_str());
    ///////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////
    FILE *in = fopen(inputFile, "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        printf("\nCan not open Compression Source file");
        fprintf(stderr, "\nCan not open Compression Source file");
        fclose(in);
        return 0;
    }

    ////////////////////////////////////Read in IVF Header Data///////////////////////////////
    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);

    oxcf.Width		= ivfhRaw.width;
    oxcf.Height		= ivfhRaw.height;

    oxcf.FrameRate	= (double)((double)ivfhRaw.rate / (double)ivfhRaw.scale);
    fclose(in);

    printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);
    fprintf(stderr, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

    if (oxcf.Mode == MODE_REALTIME)
    {
        printf(" RealTime\n\n");
        fprintf(stderr, " RealTime\n\n");
    }

    if (oxcf.Mode == MODE_GOODQUALITY)
    {
        printf(" GoodQuality\n\n");
        fprintf(stderr, " GoodQuality\n\n");
    }

    if (oxcf.Mode == MODE_BESTQUALITY)
    {
        printf(" BestQuality\n\n");
        fprintf(stderr, " BestQuality\n\n");
    }

    if (oxcf.Mode == MODE_FIRSTPASS)
    {
        printf(" FirstPass\n\n");
        fprintf(stderr, " FirstPass\n\n");
    }

    if (oxcf.Mode == MODE_SECONDPASS)
    {
        printf(" SecondPass\n\n");
        fprintf(stderr, " SecondPass\n\n");
    }

    if (oxcf.Mode == MODE_SECONDPASS_BEST)
    {
        printf(" SecondPassBestQuality\n\n");
        fprintf(stderr, " SecondPassBestQuality\n\n");
    }

    /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
    string OutputsettingsFile = outputFile2;
    OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
    OutputsettingsFile.append("_Paramaters.txt");
    char OutputsettingsFileChar[255];

    snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
    //strcpy(OutputsettingsFileChar, OutputsettingsFile.c_str());
    OutPutSettings(OutputsettingsFileChar,  oxcf);
    ///////////////////////////////////////////////////////////////////////////////////////////



    char exeFile[256];
    FolderName(TesterExePath, exeFile);
    string exeFileStr = "\"\"";
    exeFileStr.append(exeFile);
    exeFileStr.append("vp8tester.exe");
    exeFileStr.append("\" \"");
    exeFileStr.append(inputFile);
    exeFileStr.append("\" \"");
    exeFileStr.append(outputFile2);
    exeFileStr.append("\" \"");
    exeFileStr.append(OutputsettingsFileChar);
    exeFileStr.append("\"\"");

    system(exeFileStr.c_str());

    //printf("\n\nFOLDER NAME2: %s\n\n\n",TesterExePath);
    printf("\n\nSystem Command: %s\n\n\n", exeFileStr.c_str());
    fprintf(stderr, "\n\nSystem Command: %s\n\n\n", exeFileStr.c_str());

    return 0;
}
int CompressIVFtoIVFNoErrorOutput(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck)
{
    printf("\nDSHOW - Compressing IVF File to VPX IVF File: \n\n");
    //fprintf(stderr, "DSHOW - Compressing IVF File to VPX IVF File: \n");


    //char TesterFileName[255];
    //FileName(TesterExePath, TesterFileName);

    //printf("\n\nTesterFileName: %s\n",TesterFileName);

    char TesterExePathout[255];
    CopyCharArray(TesterExePath, TesterExePathout);

    string IVFMuxerStr = "\"";
    string IVFSourceStr = "\"";
    string On2VP8DecoderStr = "\"";
    string On2VP8Encoder = "\"";

    IVFMuxerStr.append(TesterExePathout);
    IVFSourceStr.append(TesterExePathout);
    On2VP8DecoderStr.append(TesterExePathout);
    On2VP8Encoder.append(TesterExePathout);

    IVFMuxerStr.erase(IVFMuxerStr.length() - 4, 4);
    IVFSourceStr.erase(IVFSourceStr.length() - 4, 4);
    On2VP8DecoderStr.erase(On2VP8DecoderStr.length() - 4, 4);
    On2VP8Encoder.erase(On2VP8Encoder.length() - 4, 4);

    //printf("IVFMuxerStr: %s\n",IVFMuxerStr.c_str());
    //printf("IVFSourceStr: %s\n",IVFSourceStr.c_str());
    //printf("SystemOn2VP8DecoderStr: %s\n",On2VP8DecoderStr.c_str());
    //printf("SystemOn2VP8Encoder: %s\n",On2VP8Encoder.c_str());

    IVFMuxerStr.append("_DLL\\IVFMuxer.dll");
    IVFSourceStr.append("_DLL\\IVFSource.dll");
    On2VP8DecoderStr.append("_DLL\\On2VP8Decoder.dll");
    On2VP8Encoder.append("_DLL\\On2VP8Encoder.dll");

    string SystemIVFMuxerStr = "\"Regsvr32.exe /s ";
    string SystemIVFSourceStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8DecoderStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8Encoder = "\"Regsvr32.exe /s ";

    SystemIVFMuxerStr.append(IVFMuxerStr);
    SystemIVFSourceStr.append(IVFSourceStr);
    SystemOn2VP8DecoderStr.append(On2VP8DecoderStr);
    SystemOn2VP8Encoder.append(On2VP8Encoder);

    SystemIVFMuxerStr.append("\"\"");
    SystemIVFSourceStr.append("\"\"");
    SystemOn2VP8DecoderStr.append("\"\"");
    SystemOn2VP8Encoder.append("\"\"");

    system(SystemIVFMuxerStr.c_str());
    system(SystemIVFSourceStr.c_str());
    system(SystemOn2VP8DecoderStr.c_str());
    system(SystemOn2VP8Encoder.c_str());

    printf("%s\n", SystemIVFMuxerStr.c_str());
    printf("%s\n", SystemIVFSourceStr.c_str());
    printf("%s\n", SystemOn2VP8DecoderStr.c_str());
    printf("%s\n", SystemOn2VP8Encoder.c_str());

    //////////////////////////////////////Set Fpf File/////////////////////////////////////////
    string UniqueFirstPassFile = outputFile2;
    UniqueFirstPassFile.erase(UniqueFirstPassFile.length() - 4, 4);
    UniqueFirstPassFile.append(".fpf");
    //	strcpy(oxcf.FirstPassFile, UniqueFirstPassFile.c_str());
    ///////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////
    FILE *in = fopen(inputFile, "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        printf("\nCan not open Compression Source file");
        fprintf(stderr, "\nCan not open Compression Source file");
        fclose(in);
        return 0;
    }

    ////////////////////////////////////Read in IVF Header Data///////////////////////////////
    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);

    oxcf.Width		= ivfhRaw.width;
    oxcf.Height		= ivfhRaw.height;

    oxcf.FrameRate	= (double)((double)ivfhRaw.rate / (double)ivfhRaw.scale);
    fclose(in);

    printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);
    //fprintf(stderr, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

    if (oxcf.Mode == MODE_REALTIME)
    {
        printf(" RealTime\n\n");
        //fprintf(stderr, " RealTime\n\n");
    }

    if (oxcf.Mode == MODE_GOODQUALITY)
    {
        printf(" GoodQuality\n\n");
        //fprintf(stderr, " GoodQuality\n\n");
    }

    if (oxcf.Mode == MODE_BESTQUALITY)
    {
        printf(" BestQuality\n\n");
        //fprintf(stderr, " BestQuality\n\n");
    }

    if (oxcf.Mode == MODE_FIRSTPASS)
    {
        printf(" FirstPass\n\n");
        //fprintf(stderr, " FirstPass\n\n");
    }

    if (oxcf.Mode == MODE_SECONDPASS)
    {
        printf(" SecondPass\n\n");
        //fprintf(stderr, " SecondPass\n\n");
    }

    if (oxcf.Mode == MODE_SECONDPASS_BEST)
    {
        printf(" SecondPassBestQuality\n\n");
        //fprintf(stderr, " SecondPassBestQuality\n\n");
    }

    /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
    string OutputsettingsFile = outputFile2;
    OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
    OutputsettingsFile.append("_Paramaters.txt");
    char OutputsettingsFileChar[255];

    snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
    //strcpy(OutputsettingsFileChar, OutputsettingsFile.c_str());
    OutPutSettings(OutputsettingsFileChar,  oxcf);
    ///////////////////////////////////////////////////////////////////////////////////////////


    char exeFile[256];
    FolderName(TesterExePath, exeFile);
    string exeFileStr = "\"\"";
    exeFileStr.append(exeFile);
    exeFileStr.append("vp8tester.exe");
    exeFileStr.append("\" \"");
    exeFileStr.append(inputFile);
    exeFileStr.append("\" \"");
    exeFileStr.append(outputFile2);
    exeFileStr.append("\" \"");
    exeFileStr.append(OutputsettingsFileChar);
    exeFileStr.append("\"\"");

    system(exeFileStr.c_str());

    printf("\n\nSystem Command: %s\n\n\n", exeFileStr.c_str());


    return 0;
}
unsigned int TimeCompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck)
{
    printf("\nDSHOW - Compressing IVF File to VPX IVF File: \n");
    fprintf(stderr, "\nDSHOW - Compressing IVF File to VPX IVF File: \n");

    char TesterExePathout[255];
    CopyCharArray(TesterExePath, TesterExePathout);

    string IVFMuxerStr = "\"";
    string IVFSourceStr = "\"";
    string On2VP8DecoderStr = "\"";
    string On2VP8Encoder = "\"";

    IVFMuxerStr.append(TesterExePathout);
    IVFSourceStr.append(TesterExePathout);
    On2VP8DecoderStr.append(TesterExePathout);
    On2VP8Encoder.append(TesterExePathout);

    IVFMuxerStr.erase(IVFMuxerStr.length() - 4, 4);
    IVFSourceStr.erase(IVFSourceStr.length() - 4, 4);
    On2VP8DecoderStr.erase(On2VP8DecoderStr.length() - 4, 4);
    On2VP8Encoder.erase(On2VP8Encoder.length() - 4, 4);

    //printf("IVFMuxerStr: %s\n",IVFMuxerStr.c_str());
    //printf("IVFSourceStr: %s\n",IVFSourceStr.c_str());
    //printf("SystemOn2VP8DecoderStr: %s\n",On2VP8DecoderStr.c_str());
    //printf("SystemOn2VP8Encoder: %s\n",On2VP8Encoder.c_str());

    IVFMuxerStr.append("_DLL\\IVFMuxer.dll");
    IVFSourceStr.append("_DLL\\IVFSource.dll");
    On2VP8DecoderStr.append("_DLL\\On2VP8Decoder.dll");
    On2VP8Encoder.append("_DLL\\On2VP8Encoder.dll");

    string SystemIVFMuxerStr = "\"Regsvr32.exe /s ";
    string SystemIVFSourceStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8DecoderStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8Encoder = "\"Regsvr32.exe /s ";

    SystemIVFMuxerStr.append(IVFMuxerStr);
    SystemIVFSourceStr.append(IVFSourceStr);
    SystemOn2VP8DecoderStr.append(On2VP8DecoderStr);
    SystemOn2VP8Encoder.append(On2VP8Encoder);

    SystemIVFMuxerStr.append("\"\"");
    SystemIVFSourceStr.append("\"\"");
    SystemOn2VP8DecoderStr.append("\"\"");
    SystemOn2VP8Encoder.append("\"\"");

    system(SystemIVFMuxerStr.c_str());
    system(SystemIVFSourceStr.c_str());
    system(SystemOn2VP8DecoderStr.c_str());
    system(SystemOn2VP8Encoder.c_str());

    printf("%s\n", SystemIVFMuxerStr.c_str());
    printf("%s\n", SystemIVFSourceStr.c_str());
    printf("%s\n", SystemOn2VP8DecoderStr.c_str());
    printf("%s\n", SystemOn2VP8Encoder.c_str());

    fprintf(stderr, "%s\n", SystemIVFMuxerStr.c_str());
    fprintf(stderr, "%s\n", SystemIVFSourceStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8DecoderStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8Encoder.c_str());

    //////////////////////////////////////Set Fpf File/////////////////////////////////////////
    string UniqueFirstPassFile = outputFile2;
    UniqueFirstPassFile.erase(UniqueFirstPassFile.length() - 4, 4);
    UniqueFirstPassFile.append(".fpf");
    //	strcpy(oxcf.FirstPassFile, UniqueFirstPassFile.c_str());
    ///////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////
    FILE *in = fopen(inputFile, "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        printf("\nCan not open Compression Source file");
        fprintf(stderr, "\nCan not open Compression Source file");
        fclose(in);
        return 0;
    }

    ////////////////////////////////////Read in IVF Header Data///////////////////////////////
    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);

    oxcf.Width		= ivfhRaw.width;
    oxcf.Height		= ivfhRaw.height;

    oxcf.FrameRate	= (double)((double)ivfhRaw.rate / (double)ivfhRaw.scale);
    fclose(in);

    printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);
    fprintf(stderr, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

    if (oxcf.Mode == MODE_REALTIME)
    {
        printf(" RealTime\n\n");
        fprintf(stderr, " RealTime\n\n");
    }

    if (oxcf.Mode == MODE_GOODQUALITY)
    {
        printf(" GoodQuality\n\n");
        fprintf(stderr, " GoodQuality\n\n");
    }

    if (oxcf.Mode == MODE_BESTQUALITY)
    {
        printf(" BestQuality\n\n");
        fprintf(stderr, " BestQuality\n\n");
    }

    if (oxcf.Mode == MODE_FIRSTPASS)
    {
        printf(" FirstPass\n\n");
        fprintf(stderr, " FirstPass\n\n");
    }

    if (oxcf.Mode == MODE_SECONDPASS)
    {
        printf(" SecondPass\n\n");
        fprintf(stderr, " SecondPass\n\n");
    }

    if (oxcf.Mode == MODE_SECONDPASS_BEST)
    {
        printf(" SecondPassBestQuality\n\n");
        fprintf(stderr, " SecondPassBestQuality\n\n");
    }

    /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
    string OutputsettingsFile = outputFile2;
    OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
    OutputsettingsFile.append("_Paramaters.txt");
    char OutputsettingsFileChar[255];

    snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
    //strcpy(OutputsettingsFileChar, OutputsettingsFile.c_str());
    OutPutSettings(OutputsettingsFileChar,  oxcf);
    ///////////////////////////////////////////////////////////////////////////////////////////



    char exeFile[256];
    FolderName(TesterExePath, exeFile);
    string exeFileStr = "\"\"";
    exeFileStr.append(exeFile);
    exeFileStr.append("vp8tester.exe");
    exeFileStr.append("\" \"");
    exeFileStr.append(inputFile);
    exeFileStr.append("\" \"");
    exeFileStr.append(outputFile2);
    exeFileStr.append("\" \"");
    exeFileStr.append(OutputsettingsFileChar);
    exeFileStr.append("\"\"");

    system(exeFileStr.c_str());

    printf("\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());
    fprintf(stderr, "\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());

    int totalms = 1;
    return(totalms);

}

int DecompressIVFtoIVF(char *inputFile, char *outputFile2)
{
    printf("\nDSHOW - Decompressing IVF File to VPX IVF File: \n");
    fprintf(stderr, "\nDSHOW - Decompressing IVF File to VPX IVF File: \n");

    char TesterExePathout[255];
    CopyCharArray(TesterExePath, TesterExePathout);

    string IVFMuxerStr = "\"";
    string IVFSourceStr = "\"";
    string On2VP8DecoderStr = "\"";
    string On2VP8Encoder = "\"";

    IVFMuxerStr.append(TesterExePathout);
    IVFSourceStr.append(TesterExePathout);
    On2VP8DecoderStr.append(TesterExePathout);
    On2VP8Encoder.append(TesterExePathout);

    IVFMuxerStr.erase(IVFMuxerStr.length() - 4, 4);
    IVFSourceStr.erase(IVFSourceStr.length() - 4, 4);
    On2VP8DecoderStr.erase(On2VP8DecoderStr.length() - 4, 4);
    On2VP8Encoder.erase(On2VP8Encoder.length() - 4, 4);

    //printf("IVFMuxerStr: %s\n",IVFMuxerStr.c_str());
    //printf("IVFSourceStr: %s\n",IVFSourceStr.c_str());
    //printf("SystemOn2VP8DecoderStr: %s\n",On2VP8DecoderStr.c_str());
    //printf("SystemOn2VP8Encoder: %s\n",On2VP8Encoder.c_str());

    IVFMuxerStr.append("_DLL\\IVFMuxer.dll");
    IVFSourceStr.append("_DLL\\IVFSource.dll");
    On2VP8DecoderStr.append("_DLL\\On2VP8Decoder.dll");
    On2VP8Encoder.append("_DLL\\On2VP8Encoder.dll");

    string SystemIVFMuxerStr = "\"Regsvr32.exe /s ";
    string SystemIVFSourceStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8DecoderStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8Encoder = "\"Regsvr32.exe /s ";

    SystemIVFMuxerStr.append(IVFMuxerStr);
    SystemIVFSourceStr.append(IVFSourceStr);
    SystemOn2VP8DecoderStr.append(On2VP8DecoderStr);
    SystemOn2VP8Encoder.append(On2VP8Encoder);

    SystemIVFMuxerStr.append("\"\"");
    SystemIVFSourceStr.append("\"\"");
    SystemOn2VP8DecoderStr.append("\"\"");
    SystemOn2VP8Encoder.append("\"\"");

    system(SystemIVFMuxerStr.c_str());
    system(SystemIVFSourceStr.c_str());
    system(SystemOn2VP8DecoderStr.c_str());
    system(SystemOn2VP8Encoder.c_str());

    printf("%s\n", SystemIVFMuxerStr.c_str());
    printf("%s\n", SystemIVFSourceStr.c_str());
    printf("%s\n", SystemOn2VP8DecoderStr.c_str());
    printf("%s\n", SystemOn2VP8Encoder.c_str());

    fprintf(stderr, "%s\n", SystemIVFMuxerStr.c_str());
    fprintf(stderr, "%s\n", SystemIVFSourceStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8DecoderStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8Encoder.c_str());

    char exeFile[256];
    FolderName(TesterExePath, exeFile);
    string exeFileStr = "\"\"";
    exeFileStr.append(exeFile);
    exeFileStr.append("vp8tester.exe");
    exeFileStr.append("\" \"");
    exeFileStr.append(inputFile);
    exeFileStr.append("\" \"");
    exeFileStr.append(outputFile2);
    exeFileStr.append("\" 0 0 0\"");

    system(exeFileStr.c_str());

    printf("\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());
    fprintf(stderr, "\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());
    return 0;
}



int DecompressIVFtoRaw(char *inputFile, char *outputFile2)
{
    string outputFile3 = "";
    outputFile3.append(outputFile2);

    //cout << "\n\noutputFile3: " << outputFile3.c_str() << "\n";
    outputFile3.erase(outputFile3.length() - 4, 4);
    outputFile3.append("_ivf.ivf");
    //cout << "\n\noutputFile3: " << outputFile3.c_str() << "\n";

    printf("\nDSHOW - Decompressing IVF File to VPX IVF File: \n");
    fprintf(stderr, "\nDSHOW - Decompressing IVF File to VPX IVF File: \n");

    char TesterExePathout[255];
    CopyCharArray(TesterExePath, TesterExePathout);

    string IVFMuxerStr = "\"";
    string IVFSourceStr = "\"";
    string On2VP8DecoderStr = "\"";
    string On2VP8Encoder = "\"";

    IVFMuxerStr.append(TesterExePathout);
    IVFSourceStr.append(TesterExePathout);
    On2VP8DecoderStr.append(TesterExePathout);
    On2VP8Encoder.append(TesterExePathout);

    IVFMuxerStr.erase(IVFMuxerStr.length() - 4, 4);
    IVFSourceStr.erase(IVFSourceStr.length() - 4, 4);
    On2VP8DecoderStr.erase(On2VP8DecoderStr.length() - 4, 4);
    On2VP8Encoder.erase(On2VP8Encoder.length() - 4, 4);

    //printf("IVFMuxerStr: %s\n",IVFMuxerStr.c_str());
    //printf("IVFSourceStr: %s\n",IVFSourceStr.c_str());
    //printf("SystemOn2VP8DecoderStr: %s\n",On2VP8DecoderStr.c_str());
    //printf("SystemOn2VP8Encoder: %s\n",On2VP8Encoder.c_str());

    IVFMuxerStr.append("_DLL\\IVFMuxer.dll");
    IVFSourceStr.append("_DLL\\IVFSource.dll");
    On2VP8DecoderStr.append("_DLL\\On2VP8Decoder.dll");
    On2VP8Encoder.append("_DLL\\On2VP8Encoder.dll");

    string SystemIVFMuxerStr = "\"Regsvr32.exe /s ";
    string SystemIVFSourceStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8DecoderStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8Encoder = "\"Regsvr32.exe /s ";

    SystemIVFMuxerStr.append(IVFMuxerStr);
    SystemIVFSourceStr.append(IVFSourceStr);
    SystemOn2VP8DecoderStr.append(On2VP8DecoderStr);
    SystemOn2VP8Encoder.append(On2VP8Encoder);

    SystemIVFMuxerStr.append("\"\"");
    SystemIVFSourceStr.append("\"\"");
    SystemOn2VP8DecoderStr.append("\"\"");
    SystemOn2VP8Encoder.append("\"\"");

    system(SystemIVFMuxerStr.c_str());
    system(SystemIVFSourceStr.c_str());
    system(SystemOn2VP8DecoderStr.c_str());
    system(SystemOn2VP8Encoder.c_str());

    printf("%s\n", SystemIVFMuxerStr.c_str());
    printf("%s\n", SystemIVFSourceStr.c_str());
    printf("%s\n", SystemOn2VP8DecoderStr.c_str());
    printf("%s\n", SystemOn2VP8Encoder.c_str());

    fprintf(stderr, "%s\n", SystemIVFMuxerStr.c_str());
    fprintf(stderr, "%s\n", SystemIVFSourceStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8DecoderStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8Encoder.c_str());

    char exeFile[256];
    FolderName(TesterExePath, exeFile);
    string exeFileStr = "\"\"";
    exeFileStr.append(exeFile);
    exeFileStr.append("vp8tester.exe");
    exeFileStr.append("\" \"");
    exeFileStr.append(inputFile);
    exeFileStr.append("\" \"");
    exeFileStr.append(outputFile3.c_str());
    exeFileStr.append("\" 0 0 0\"");

    system(exeFileStr.c_str());
    char output4[255];
    snprintf(output4, 255, "%s", outputFile3.c_str());
    //strcpy(output4,outputFile3.c_str());
    IVF2Raw(output4, outputFile2);

    if (remove(output4) != 0)
    {
        cout << "\nError deleting file: " << output4 << "\n";
        cerr << "\nError deleting file: " << output4 << "\n";
    }

    printf("\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());
    fprintf(stderr, "\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());

    return 0;
}
int DecompressIVFtoIVFNoOutput(char *inputFile, char *outputFile2)
{
    printf("/nDSHOW - Decompressing IVF File to VPX IVF File: \n");
    //fprintf(stderr, "\nDSHOW - Decompressing IVF File to VPX IVF File: \n");

    char TesterExePathout[255];
    CopyCharArray(TesterExePath, TesterExePathout);

    string IVFMuxerStr = "\"";
    string IVFSourceStr = "\"";
    string On2VP8DecoderStr = "\"";
    string On2VP8Encoder = "\"";

    IVFMuxerStr.append(TesterExePathout);
    IVFSourceStr.append(TesterExePathout);
    On2VP8DecoderStr.append(TesterExePathout);
    On2VP8Encoder.append(TesterExePathout);

    IVFMuxerStr.erase(IVFMuxerStr.length() - 4, 4);
    IVFSourceStr.erase(IVFSourceStr.length() - 4, 4);
    On2VP8DecoderStr.erase(On2VP8DecoderStr.length() - 4, 4);
    On2VP8Encoder.erase(On2VP8Encoder.length() - 4, 4);

    IVFMuxerStr.append("_DLL\\IVFMuxer.dll");
    IVFSourceStr.append("_DLL\\IVFSource.dll");
    On2VP8DecoderStr.append("_DLL\\On2VP8Decoder.dll");
    On2VP8Encoder.append("_DLL\\On2VP8Encoder.dll");

    string SystemIVFMuxerStr = "\"Regsvr32.exe /s ";
    string SystemIVFSourceStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8DecoderStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8Encoder = "\"Regsvr32.exe /s ";

    SystemIVFMuxerStr.append(IVFMuxerStr);
    SystemIVFSourceStr.append(IVFSourceStr);
    SystemOn2VP8DecoderStr.append(On2VP8DecoderStr);
    SystemOn2VP8Encoder.append(On2VP8Encoder);

    SystemIVFMuxerStr.append("\"\"");
    SystemIVFSourceStr.append("\"\"");
    SystemOn2VP8DecoderStr.append("\"\"");
    SystemOn2VP8Encoder.append("\"\"");

    system(SystemIVFMuxerStr.c_str());
    system(SystemIVFSourceStr.c_str());
    system(SystemOn2VP8DecoderStr.c_str());
    system(SystemOn2VP8Encoder.c_str());

    printf("\n\nIVFMuxerStr: %s\n", SystemIVFMuxerStr.c_str());
    printf("IVFSourceStr: %s\n", SystemIVFSourceStr.c_str());
    printf("SystemOn2VP8DecoderStr: %s\n", SystemOn2VP8DecoderStr.c_str());
    printf("SystemOn2VP8Encoder: %s\n", SystemOn2VP8Encoder.c_str());

    printf("%s\n", SystemIVFMuxerStr.c_str());
    printf("%s\n", SystemIVFSourceStr.c_str());
    printf("%s\n", SystemOn2VP8DecoderStr.c_str());
    printf("%s\n", SystemOn2VP8Encoder.c_str());

    //fprintf(stderr, "%s\n\n",SystemIVFMuxerStr.c_str());
    //fprintf(stderr, "%s\n",SystemIVFSourceStr.c_str());
    //fprintf(stderr, "%s\n",SystemOn2VP8DecoderStr.c_str());
    //fprintf(stderr, "%s\n",SystemOn2VP8Encoder.c_str());

    char exeFile[256];
    FolderName(TesterExePath, exeFile);
    string exeFileStr = "\"\"";
    exeFileStr.append(exeFile);
    exeFileStr.append("vp8tester.exe");
    exeFileStr.append("\" \"");
    exeFileStr.append(inputFile);
    exeFileStr.append("\" \"");
    exeFileStr.append(outputFile2);
    exeFileStr.append("\" 0 0 0\"");

    system(exeFileStr.c_str());

    printf("\n\nSystem Command: %s\n\n\n", exeFileStr.c_str());
    return 0;
}
unsigned int TimeDecompressIVFtoIVF(char *inputFile, char *outputFile2)
{

    printf("\nDSHOW - Decompressing IVF File to VPX IVF File: \n");

    char TesterExePathout[255];
    CopyCharArray(TesterExePath, TesterExePathout);

    string IVFMuxerStr = "\"";
    string IVFSourceStr = "\"";
    string On2VP8DecoderStr = "\"";
    string On2VP8Encoder = "\"";

    IVFMuxerStr.append(TesterExePathout);
    IVFSourceStr.append(TesterExePathout);
    On2VP8DecoderStr.append(TesterExePathout);
    On2VP8Encoder.append(TesterExePathout);

    IVFMuxerStr.erase(IVFMuxerStr.length() - 4, 4);
    IVFSourceStr.erase(IVFSourceStr.length() - 4, 4);
    On2VP8DecoderStr.erase(On2VP8DecoderStr.length() - 4, 4);
    On2VP8Encoder.erase(On2VP8Encoder.length() - 4, 4);

    //printf("IVFMuxerStr: %s\n",IVFMuxerStr.c_str());
    //printf("IVFSourceStr: %s\n",IVFSourceStr.c_str());
    //printf("SystemOn2VP8DecoderStr: %s\n",On2VP8DecoderStr.c_str());
    //printf("SystemOn2VP8Encoder: %s\n",On2VP8Encoder.c_str());

    IVFMuxerStr.append("_DLL\\IVFMuxer.dll");
    IVFSourceStr.append("_DLL\\IVFSource.dll");
    On2VP8DecoderStr.append("_DLL\\On2VP8Decoder.dll");
    On2VP8Encoder.append("_DLL\\On2VP8Encoder.dll");

    string SystemIVFMuxerStr = "\"Regsvr32.exe /s ";
    string SystemIVFSourceStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8DecoderStr = "\"Regsvr32.exe /s ";
    string SystemOn2VP8Encoder = "\"Regsvr32.exe /s ";

    SystemIVFMuxerStr.append(IVFMuxerStr);
    SystemIVFSourceStr.append(IVFSourceStr);
    SystemOn2VP8DecoderStr.append(On2VP8DecoderStr);
    SystemOn2VP8Encoder.append(On2VP8Encoder);

    SystemIVFMuxerStr.append("\"\"");
    SystemIVFSourceStr.append("\"\"");
    SystemOn2VP8DecoderStr.append("\"\"");
    SystemOn2VP8Encoder.append("\"\"");

    system(SystemIVFMuxerStr.c_str());
    system(SystemIVFSourceStr.c_str());
    system(SystemOn2VP8DecoderStr.c_str());
    system(SystemOn2VP8Encoder.c_str());

    printf("%s\n", SystemIVFMuxerStr.c_str());
    printf("%s\n", SystemIVFSourceStr.c_str());
    printf("%s\n", SystemOn2VP8DecoderStr.c_str());
    printf("%s\n", SystemOn2VP8Encoder.c_str());

    fprintf(stderr, "%s\n\n", SystemIVFMuxerStr.c_str());
    fprintf(stderr, "%s\n", SystemIVFSourceStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8DecoderStr.c_str());
    fprintf(stderr, "%s\n", SystemOn2VP8Encoder.c_str());

    //fprintf(stderr, "DSHOW - Decompressing IVF File to VPX IVF File: \n");

    char exeFile[256];
    FolderName(TesterExePath, exeFile);
    string exeFileStr = "\"\"";
    exeFileStr.append(exeFile);
    exeFileStr.append("vp8tester.exe");
    exeFileStr.append("\" \"");
    exeFileStr.append(inputFile);
    exeFileStr.append("\" \"");
    exeFileStr.append(outputFile2);
    exeFileStr.append("\" 0 0 0\"");

    system(exeFileStr.c_str());

    printf("\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());
    fprintf(stderr, "\n\nSysCommand: %s\n\n\n", exeFileStr.c_str());

    return 1;
    //return(totalms);

}
//DecompressIVFtoIVFTimeAndOutput Not Needed for DShow Filters (tests that use it wont work anyway)
unsigned int DecompressIVFtoIVFTimeAndOutput(char *inputFile, char *outputFile2)
{
    return 1;
}
//KeyFrameInOutCompressIVFtoIVF The only test this is located in is not a valid VP8 Test

#endif
//----------------------------------------------------------IVF API-------------------------------------------------------------------------
#ifdef API
int CompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck)
{
    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        string QuantOutStr = outputFile2;
        QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        QuantOutStr.append("_Quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    on2_codec_ctx_t        encoder;
    char                  *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    on2_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    unsigned long            cx_time = 0;

    /* Populate encoder configuration */
    res = on2_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        printf("Failed to get config: %s\n", on2_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    VP8CoreConfigToAPIcfg(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        printf("Input File not found: %s\n", in_fn);
        fprintf(stderr, "Input File not found: %s\n", in_fn);
        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    FormatIVFHeaderRead(&ivfhRaw);

    int w		= ivfhRaw.width;
    int h		= ivfhRaw.height;
    int fr		= (ivfhRaw.rate / ivfhRaw.scale);

    oxcf.Width = w;
    oxcf.Height = h;

    cfg.g_w = w;                                                          //
    cfg.g_h = h;
    cfg.g_timebase.num = 1000;
    cfg.g_timebase.den = fr * 1000;
    fclose(GetWHinfile);

    //Set Mode Pass and BitRate Here
    cfg.rc_target_bitrate = BitRate;

    if (oxcf.Mode == 0) //Real Time Mode
    {
        arg_deadline = 1;
    }

    if (oxcf.Mode == 1) //One Pass Good
    {
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 2) //One Pass Best
    {
        arg_deadline = 0;
    }

    if (oxcf.Mode == 3) //First Pass
    {
        //arg_deadline =
    }

    if (oxcf.Mode == 4) //Two Pass Good
    {
        arg_passes = 2;
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 5) //Two Pass Best
    {
        arg_passes = 2;
        arg_deadline = 0;
    }

    ///////////////////////////////////////////////////////////////////

    /* Handle codec specific options */
    if (codec->iface == &on2_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    on2_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);
        fprintf(stderr, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            printf("\nFirst Pass - ");
            fprintf(stderr, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            printf("\nSecond Pass - ");
            fprintf(stderr, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            printf(" RealTime\n\n");
            fprintf(stderr, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            printf(" GoodQuality\n\n");
            fprintf(stderr, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            printf(" BestQuality\n\n");
            fprintf(stderr, " BestQuality\n\n");
        }

        printf("API - Compressing Raw IVF File to VP8 IVF File: \n");
        fprintf(stderr, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            printf("Failed to open input file: %s", in_fn);
            fprintf(stderr, "Failed to open input file: %s", in_fn);
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            printf("Failed to open output file: %s", out_fn);
            fprintf(stderr, "Failed to open output file: %s", out_fn);
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? ON2_RC_LAST_PASS : ON2_RC_FIRST_PASS
                 : ON2_RC_ONE_PASS;
#if ON2_ENCODER_ABI_VERSION > (1 + ON2_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = ON2_KF_FIXED;

        on2_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");
        ///////////Set Encoder Custom Settings/////////////////
        on2_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.CpuUsed);
        on2_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.EncodeBreakout);
        on2_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.PlayAlternate);
        on2_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.NoiseSensitivity);
        on2_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        on2_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (on2_vp8e_token_partitions) oxcf.TokenPartitions);
        ///////////////////////////////////////////////////////
        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_Paramaters.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 15, 15);
            OutputsettingsFile.append("_APIParamaters.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettingsAPI(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        while (frame_avail || got_data)
        {
            on2_codec_iter_t iter = NULL;
            const on2_codec_cx_pkt_t *pkt;
            struct on2_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame(infile, &raw);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");
            }
            else
                frame_avail = 0;

            on2_usec_timer_start(&timer);
            on2_codec_encode(&encoder, frame_avail ? &raw : NULL, (frames_in - 1) * 2, 2, 0, arg_deadline);
            on2_usec_timer_mark(&timer);
            cx_time += on2_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            if (RunQCheck == 1)
            {
                if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
                {
                    int lastQuantizerValue = 0;
                    on2_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                    QuantOutFile << frames_out << " " << lastQuantizerValue << "\n";
                }
            }

            if (RunQCheck == 2)
            {
                //Print Quantizers
                int lastQuantizerValue = 0;
                on2_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                printf("frame %i Quantizer: %i\n", frames_out, lastQuantizerValue);
            }

            while ((pkt = on2_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case ON2_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case ON2_CODEC_STATS_PKT:
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    break;
                default:
                    break;
                }
            }

            fflush(stdout);
        }

        /* this bitrate calc is simplified and relies on the fact that this
        * application uses 1/timebase for framerate.
        */
        {
            uint64_t temp = (uint64_t)frames_in * 1000000;

        }
        on2_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats);
        printf("\n");
        fprintf(stderr, "\n");
    }

    on2_img_free(&raw);

    if (RunQCheck == 1)
    {
        QuantOutFile.close();
    }

    return 0;
}
int CompressIVFtoIVFNoErrorOutput(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck)
{
    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    on2_codec_ctx_t        encoder;
    char                  *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    on2_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    unsigned long            cx_time = 0;

    /* Populate encoder configuration */
    res = on2_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        printf("Failed to get config: %s\n", on2_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    VP8CoreConfigToAPIcfg(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        printf("Input File not found: %s\n", in_fn);
        fprintf(stderr, "Input File not found: %s\n", in_fn);
        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    FormatIVFHeaderRead(&ivfhRaw);

    int w		= ivfhRaw.width;
    int h		= ivfhRaw.height;
    int fr		= (ivfhRaw.rate / ivfhRaw.scale);

    oxcf.Width = w;
    oxcf.Height = h;

    cfg.g_w = w;                                                          //
    cfg.g_h = h;
    cfg.g_timebase.num = 1000;
    cfg.g_timebase.den = fr * 1000;
    fclose(GetWHinfile);
    //Deal with Mode Pass and BitRate Here
    cfg.rc_target_bitrate = BitRate;

    if (oxcf.Mode == 0) //Real Time Mode
    {
        arg_deadline = 1;
    }

    if (oxcf.Mode == 1) //One Pass Good
    {
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 2) //One Pass Best
    {
        arg_deadline = 0;
    }

    if (oxcf.Mode == 3) //First Pass No longer exists
    {
    }

    if (oxcf.Mode == 4) //Two Pass Good
    {
        arg_passes = 2;
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 5) //Two Pass Best
    {
        arg_passes = 2;
        arg_deadline = 0;
    }

    ///////////////////////////////////////////////////////////////////

    /* Handle codec specific options */
    if (codec->iface == &on2_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    on2_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            printf("\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            printf("\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            printf(" RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            printf(" GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            printf(" BestQuality\n\n");
        }

        printf("API - Compressing Raw IVF File to VP8 IVF File: \n");
        //fprintf(stderr, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            printf("Failed to open input file: %s", in_fn);
            fprintf(stderr, "Failed to open input file: %s", in_fn);
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            printf("Failed to open output file: %s", out_fn);
            fprintf(stderr, "Failed to open output file: %s", out_fn);
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? ON2_RC_LAST_PASS : ON2_RC_FIRST_PASS
                 : ON2_RC_ONE_PASS;
#if ON2_ENCODER_ABI_VERSION > (1 + ON2_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = ON2_KF_FIXED;

        //on2_codec_enc_config_set(&encoder,&cfg);
        on2_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");
        ///////////Set Encoder Custom Settings/////////////////
        on2_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.CpuUsed);
        on2_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.EncodeBreakout);
        on2_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.PlayAlternate);
        on2_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.NoiseSensitivity);
        on2_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        on2_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (on2_vp8e_token_partitions) oxcf.TokenPartitions);
        ///////////////////////////////////////////////////////

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_Paramaters.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 15, 15);
            OutputsettingsFile.append("_APIParamaters.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettingsAPI(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        while (frame_avail || got_data)
        {
            on2_codec_iter_t iter = NULL;
            const on2_codec_cx_pkt_t *pkt;
            struct on2_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame(infile, &raw);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    printf("\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
            }
            else
                frame_avail = 0;

            on2_usec_timer_start(&timer);
            on2_codec_encode(&encoder, frame_avail ? &raw : NULL, (frames_in - 1) * 2,
                             2, 0, arg_deadline);
            on2_usec_timer_mark(&timer);
            cx_time += on2_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            //Print Quantizers
            //int lastQuantizerValue = 0;
            //on2_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
            //printf("frame %i Quantizer: %i\n",frames_out,lastQuantizerValue);

            while ((pkt = on2_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case ON2_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case ON2_CODEC_STATS_PKT:
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    break;
                default:
                    break;
                }
            }

            fflush(stdout);
        }

        /* this bitrate calc is simplified and relies on the fact that this
        * application uses 1/timebase for framerate.
        */
        {
            //uint64_t temp= (uint64_t)frames_in * 1000000;
        }
        on2_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats);
        printf("\n");
    }

    on2_img_free(&raw);
    return 0;
}
unsigned int TimeCompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck)
{
    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    on2_codec_ctx_t        encoder;
    char                  *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    on2_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;

    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    unsigned long            cx_time = 0;

    int framesoutrec = 0;

    /* Populate encoder configuration */
    res = on2_codec_enc_config_default(codec->iface, &cfg, 0);

    if (res)
    {
        printf("Failed to get config: %s\n", on2_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    VP8CoreConfigToAPIcfg(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        printf("Input File not found: %s\n", in_fn);
        fprintf(stderr, "Input File not found: %s\n", in_fn);
        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    FormatIVFHeaderRead(&ivfhRaw);

    int w		= ivfhRaw.width;
    int h		= ivfhRaw.height;
    int fr		= (ivfhRaw.rate / ivfhRaw.scale);

    oxcf.Width = w;
    oxcf.Height = h;

    cfg.g_w = w;                                                          //
    cfg.g_h = h;
    cfg.g_timebase.num = 1000;
    cfg.g_timebase.den = fr * 1000;
    fclose(GetWHinfile);
    //Deal with Mode Pass and BitRate Here
    cfg.rc_target_bitrate = BitRate;

    if (oxcf.Mode == 0) //Real Time Mode
    {
        arg_deadline = 1;
    }

    if (oxcf.Mode == 1) //One Pass Good
    {
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 2) //One Pass Best
    {
        arg_deadline = 0;
    }

    if (oxcf.Mode == 3) //First Pass
    {
    }

    if (oxcf.Mode == 4) //Two Pass Good
    {
        arg_passes = 2;
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 5) //Two Pass Best
    {
        arg_passes = 2;
        arg_deadline = 0;
    }

    ///////////////////////////////////////////////////////////////////

    /* Handle codec specific options */
    if (codec->iface == &on2_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    on2_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);
        fprintf(stderr, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            printf("\nFirst Pass - ");
            fprintf(stderr, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            printf("\nSecond Pass - ");
            fprintf(stderr, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            printf(" RealTime\n\n");
            fprintf(stderr, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            printf(" GoodQuality\n\n");
            fprintf(stderr, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            printf(" BestQuality\n\n");
            fprintf(stderr, " BestQuality\n\n");
        }

        printf("API - Compressing Raw IVF File to VP8 IVF File: \n");
        fprintf(stderr, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            printf("Failed to open input file");
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            printf("Failed to open output file");
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? ON2_RC_LAST_PASS : ON2_RC_FIRST_PASS
                 : ON2_RC_ONE_PASS;
#if ON2_ENCODER_ABI_VERSION > (1 + ON2_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = ON2_KF_FIXED;

        on2_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");
        ///////////Set Encoder Custom Settings/////////////////
        on2_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.CpuUsed);
        on2_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.EncodeBreakout);
        on2_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.PlayAlternate);
        on2_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.NoiseSensitivity);
        on2_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        on2_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (on2_vp8e_token_partitions) oxcf.TokenPartitions);
        ///////////////////////////////////////////////////////


        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_Paramaters.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 15, 15);
            OutputsettingsFile.append("_APIParamaters.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettingsAPI(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        while (frame_avail || got_data)
        {
            on2_codec_iter_t iter = NULL;
            const on2_codec_cx_pkt_t *pkt;
            struct on2_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame(infile, &raw);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");
            }
            else
                frame_avail = 0;

            on2_usec_timer_start(&timer);
            on2_codec_encode(&encoder, frame_avail ? &raw : NULL, (frames_in - 1) * 2, 2, 0, arg_deadline);
            on2_usec_timer_mark(&timer);
            cx_time += on2_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            while ((pkt = on2_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case ON2_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case ON2_CODEC_STATS_PKT:
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    break;
                default:
                    break;
                }
            }

            fflush(stdout);
        }

        /* this bitrate calc is simplified and relies on the fact that this
        * application uses 1/timebase for framerate.
        */
        {
            uint64_t temp = (uint64_t)frames_in * 1000000;
        }
        on2_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats);
        printf("\n");
        framesoutrec = frames_out;
    }

    on2_img_free(&raw);

    printf("\n File completed: Time in Microseconds: %u, Fps: %d \n",
           cx_time, 1000 * framesoutrec / (cx_time / 1000));
    fprintf(stderr, "\n File completed: Time in Microseconds: %u, Fps: %d \n",
            cx_time, 1000 * framesoutrec / (cx_time / 1000));

    char TextFilechar1[255];

    FolderName2(outputFile2, TextFilechar1);

    char *FullName = strcat(TextFilechar1, "CompressionTime.txt");

    ofstream outfile2(FullName);
    outfile2 << cx_time;
    outfile2.close();

    return cx_time;
}
int CompressIVFtoIVFForceKeyFrame(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, int forceKeyFrame)
{
    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        string QuantOutStr = outputFile2;
        QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        QuantOutStr.append("_Quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    on2_codec_ctx_t        encoder;
    char                  *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    on2_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    unsigned long            cx_time = 0;
    int                  flags = 0;


    /* Populate encoder configuration */
    res = on2_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        printf("Failed to get config: %s\n", on2_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    VP8CoreConfigToAPIcfg(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        printf("Input File not found: %s\n", in_fn);
        fprintf(stderr, "Input File not found: %s\n", in_fn);
        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    FormatIVFHeaderRead(&ivfhRaw);

    int w		= ivfhRaw.width;
    int h		= ivfhRaw.height;
    int fr		= (ivfhRaw.rate / ivfhRaw.scale);

    oxcf.Width = w;
    oxcf.Height = h;

    cfg.g_w = w;                                                          //
    cfg.g_h = h;
    cfg.g_timebase.num = 1000;
    cfg.g_timebase.den = fr * 1000;
    fclose(GetWHinfile);

    //Set Mode Pass and BitRate Here
    cfg.rc_target_bitrate = BitRate;

    if (oxcf.Mode == 0) //Real Time Mode
    {
        arg_deadline = 1;
    }

    if (oxcf.Mode == 1) //One Pass Good
    {
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 2) //One Pass Best
    {
        arg_deadline = 0;
    }

    if (oxcf.Mode == 3) //First Pass
    {
        //arg_deadline =
    }

    if (oxcf.Mode == 4) //Two Pass Good
    {
        arg_passes = 2;
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 5) //Two Pass Best
    {
        arg_passes = 2;
        arg_deadline = 0;
    }

    ///////////////////////////////////////////////////////////////////

    /* Handle codec specific options */
    if (codec->iface == &on2_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    on2_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);
        fprintf(stderr, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            printf("\nFirst Pass - ");
            fprintf(stderr, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            printf("\nSecond Pass - ");
            fprintf(stderr, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            printf(" RealTime\n\n");
            fprintf(stderr, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            printf(" GoodQuality\n\n");
            fprintf(stderr, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            printf(" BestQuality\n\n");
            fprintf(stderr, " BestQuality\n\n");
        }

        printf("API - Compressing Raw IVF File to VP8 IVF File: \n");
        fprintf(stderr, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            printf("Failed to open input file: %s", in_fn);
            fprintf(stderr, "Failed to open input file: %s", in_fn);
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            printf("Failed to open output file: %s", out_fn);
            fprintf(stderr, "Failed to open output file: %s", out_fn);
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? ON2_RC_LAST_PASS : ON2_RC_FIRST_PASS
                 : ON2_RC_ONE_PASS;
#if ON2_ENCODER_ABI_VERSION > (1 + ON2_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = ON2_KF_FIXED;

        on2_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");
        ///////////Set Encoder Custom Settings/////////////////
        on2_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.CpuUsed);
        on2_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.EncodeBreakout);
        on2_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.PlayAlternate);
        on2_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.NoiseSensitivity);
        on2_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        on2_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (on2_vp8e_token_partitions) oxcf.TokenPartitions);
        ///////////////////////////////////////////////////////
        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_Paramaters.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 15, 15);
            OutputsettingsFile.append("_APIParamaters.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettingsAPI(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        int forceKeyFrameTracker = forceKeyFrame;

        while (frame_avail || got_data)
        {
            on2_codec_iter_t iter = NULL;
            const on2_codec_cx_pkt_t *pkt;
            struct on2_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame(infile, &raw);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");
            }
            else
                frame_avail = 0;

            //cout << "\nframes_in: " << frames_in << " forceKeyFrameTracker: " << forceKeyFrameTracker;

            if (frames_in - 1 == forceKeyFrameTracker)
            {
                flags |= VPX_EFLAG_FORCE_KF;
                forceKeyFrameTracker = forceKeyFrameTracker + forceKeyFrame;
                //cout << "\nForce Key Frame: " << frames_in << " Next KeyFrame: " << forceKeyFrameTracker <<"\n";
            }
            else
            {
                flags &= ~VPX_EFLAG_FORCE_KF;
            }


            on2_usec_timer_start(&timer);
            on2_codec_encode(&encoder, frame_avail ? &raw : NULL, (frames_in - 1) * 2, 2, flags, arg_deadline);
            //on2_codec_encode(&encoder, frame_avail ? &raw : NULL, (frames_in - 1) * 2, 2, VPX_EFLAG_FORCE_KF, arg_deadline);
            on2_usec_timer_mark(&timer);
            cx_time += on2_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            if (RunQCheck == 1)
            {
                if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
                {
                    int lastQuantizerValue = 0;
                    on2_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                    QuantOutFile << frames_out << " " << lastQuantizerValue << "\n";
                }
            }

            if (RunQCheck == 2)
            {
                //Print Quantizers
                int lastQuantizerValue = 0;
                on2_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                printf("frame %i Quantizer: %i\n", frames_out, lastQuantizerValue);
            }

            while ((pkt = on2_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case ON2_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case ON2_CODEC_STATS_PKT:
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    break;
                default:
                    break;
                }
            }

            fflush(stdout);
        }

        /* this bitrate calc is simplified and relies on the fact that this
        * application uses 1/timebase for framerate.
        */
        {
            uint64_t temp = (uint64_t)frames_in * 1000000;

        }
        on2_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats);
        printf("\n");
        fprintf(stderr, "\n");
    }

    on2_img_free(&raw);

    if (RunQCheck == 1)
    {
        QuantOutFile.close();
    }

    return 0;
}
int CompressIVFtoIVFReconBufferCheck(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck)
{
    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        string QuantOutStr = outputFile2;
        QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        QuantOutStr.append("_Quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    //on2_codec_ctx_t       decoder;
    on2_codec_ctx_t       encoder;
    char                  *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile, *outfile3;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    on2_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    unsigned long            cx_time = 0;


    //outfile = encoded ivf file
    void *out;//all raw preview frames
    void *out2;//all raw decoded frames
    void *out3;//individual raw preview frames
    void *out4;//individual decoded preview frames
    //FILE* ReconOutFile;//text file output.



    /* Populate encoder configuration */
    res = on2_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        printf("Failed to get config: %s\n", on2_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    VP8CoreConfigToAPIcfg(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        printf("Input File not found: %s\n", in_fn);
        fprintf(stderr, "Input File not found: %s\n", in_fn);
        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    FormatIVFHeaderRead(&ivfhRaw);

    int w		= ivfhRaw.width;
    int h		= ivfhRaw.height;
    int fr		= (ivfhRaw.rate / ivfhRaw.scale);

    oxcf.Width = w;
    oxcf.Height = h;

    cfg.g_w = w;                                                          //
    cfg.g_h = h;
    cfg.g_timebase.num = 1000;
    cfg.g_timebase.den = fr * 1000;
    fclose(GetWHinfile);
    //Set Mode Pass and BitRate Here
    cfg.rc_target_bitrate = BitRate;

    if (oxcf.Mode == 0) //Real Time Mode
    {
        arg_deadline = 1;
    }

    if (oxcf.Mode == 1) //One Pass Good
    {
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 2) //One Pass Best
    {
        arg_deadline = 0;
    }

    if (oxcf.Mode == 3) //First Pass
    {
        //arg_deadline =
    }

    if (oxcf.Mode == 4) //Two Pass Good
    {
        arg_passes = 2;
        arg_deadline = 1000000;
    }

    if (oxcf.Mode == 5) //Two Pass Best
    {
        arg_passes = 2;
        arg_deadline = 0;
    }

    ///////////////////////////////////////////////////////////////////

    /* Handle codec specific options */
    if (codec->iface == &on2_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    on2_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    string out_fn4STRb = out_fn;
    out_fn4STRb.erase(out_fn4STRb.length() - 4, 4);
    out_fn4STRb.append("_DecodeFrame");
    out_fn4STRb.append(slashCharStr.c_str());

    string CreateDir3b = out_fn4STRb;
    CreateDir3b.insert(0, "mkdir \"");
    CreateDir3b.append("\"");
    system(CreateDir3b.c_str());

    string out_fn3STRb = out_fn;
    out_fn3STRb.erase(out_fn3STRb.length() - 4, 4);
    out_fn3STRb.append("_PreviewFrame");
    out_fn3STRb.append(slashCharStr.c_str());

    string CreateDir2b = out_fn3STRb;
    CreateDir2b.insert(0, "mkdir \"");
    CreateDir2b.append("\"");
    system(CreateDir2b.c_str());


    ofstream ReconOutFile;
    string ReconOutStr = outputFile2;
    ReconOutStr.erase(ReconOutStr.length() - 4, 4);
    ReconOutStr.append("_ReconFrameState.txt");
    char ReconOutChar[255];
    snprintf(ReconOutChar, 255, "%s", ReconOutStr.c_str());

    //FILE* ReconOutFile;//text file output.
    //ReconOutFile = fopen(ReconOutChar, "wb");

    ReconOutFile.open(ReconOutChar);

    for (pass = 0; pass < arg_passes; pass++)
    {
        printf("\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);
        fprintf(stderr, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.TargetBandwidth, oxcf.WorstAllowedQ, oxcf.BestAllowedQ, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            printf("\nFirst Pass - ");
            fprintf(stderr, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            printf("\nSecond Pass - ");
            fprintf(stderr, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            printf(" RealTime\n\n");
            fprintf(stderr, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            printf(" GoodQuality\n\n");
            fprintf(stderr, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            printf(" BestQuality\n\n");
            fprintf(stderr, " BestQuality\n\n");
        }

        printf("API - Compressing Raw IVF File to VP8 IVF File: \n");
        fprintf(stderr, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            printf("Failed to open input file: %s", in_fn);
            fprintf(stderr, "Failed to open input file: %s", in_fn);
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");
        /////////////////////////DELETEME//////////////////////////////////
        string out_fn2STR = out_fn;
        out_fn2STR.append("_Preview.raw");
        out = out_open(out_fn2STR.c_str(), 0);

        string out_fn3STR = out_fn;
        out_fn3STR.append("_Decode.raw");
        out2 = out_open(out_fn3STR.c_str(), 0);
        ///////////////////////////////////////////////////////////////////

        if (!outfile)
        {
            printf("Failed to open output file: %s", out_fn);
            fprintf(stderr, "Failed to open output file: %s", out_fn);
            //ReconOutFile.close();
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                printf("Failed to open statistics store\n");
                //ReconOutFile.close();
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                printf("Failed to open statistics store\n");
                //ReconOutFile.close();
                return EXIT_FAILURE;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? ON2_RC_LAST_PASS : ON2_RC_FIRST_PASS
                 : ON2_RC_ONE_PASS;
#if ON2_ENCODER_ABI_VERSION > (1 + ON2_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = ON2_KF_FIXED;

        //on2_codec_enc_config_set(&encoder,&cfg);
        on2_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");
        ///////////Set Encoder Custom Settings/////////////////
        on2_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.CpuUsed);
        on2_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.EncodeBreakout);
        on2_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.PlayAlternate);
        on2_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.NoiseSensitivity);
        on2_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        on2_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (on2_vp8e_token_partitions) oxcf.TokenPartitions);
        ///////////////////////////////////////////////////////

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_Paramaters.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 15, 15);
            OutputsettingsFile.append("_APIParamaters.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            OutPutSettingsAPI(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        /////////////////////////////////////////////////INI DECODER/////////////////////////////////////////////////
        on2_codec_ctx_t       decoder;
        vp8_postproc_cfg_t      vp8_pp_cfg = {0};
        on2_codec_iface_t       *iface = ifaces[0].iface;
        int postproc = 0;
        on2_codec_dec_cfg_t     cfgdec = {0};

        if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfgdec,
                               postproc ? ON2_CODEC_USE_POSTPROC : 0))
        {
            printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
            //ReconOutFile.close();
            return EXIT_FAILURE;
        }

        if (vp8_pp_cfg.post_proc_flag
            && vpx_codec_control(&decoder, VP8_SET_POSTPROC, &vp8_pp_cfg))
        {
            fprintf(stderr, "Failed to configure postproc: %s\n", vpx_codec_error(&decoder));
            //ReconOutFile.close();
            return EXIT_FAILURE;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (frame_avail || got_data)
        {
            on2_codec_iter_t iter = NULL;
            const on2_codec_cx_pkt_t *pkt;
            struct on2_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame(infile, &raw);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");
            }
            else
                frame_avail = 0;

            on2_usec_timer_start(&timer);
            on2_codec_encode(&encoder, frame_avail ? &raw : NULL, (frames_in - 1) * 2,
                             2, 0, arg_deadline);
            on2_usec_timer_mark(&timer);
            cx_time += on2_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            if (RunQCheck == 1)
            {
                if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
                {
                    int lastQuantizerValue = 0;
                    on2_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                    QuantOutFile << frames_out << " " << lastQuantizerValue << "\n";
                }
            }

            if (RunQCheck == 2)
            {
                //Print Quantizers
                int lastQuantizerValue = 0;
                on2_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                printf("frame %i Quantizer: %i\n", frames_out, lastQuantizerValue);
            }

            while ((pkt = on2_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;
                on2_codec_iter_t  iterdec = NULL;
                int MemCheck1 = 1;
                int MemCheck2 = 1;
                int MemCheck3 = 1;

                switch (pkt->kind)
                {
                case ON2_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);

                    const on2_image_t    *imgPreview;
                    const on2_image_t    *imgDecode;

                    imgPreview = vpx_codec_get_preview_frame(&encoder);

                    on2_codec_decode(&decoder, (const uint8_t *)pkt->data.frame.buf, pkt->data.frame.sz, NULL, 0);
                    imgDecode = on2_codec_get_frame(&decoder, &iterdec);

                    if (imgPreview && imgDecode)
                    {
                        if (1)
                        {
                            string out_fn3STR = out_fn;
                            out_fn3STR.erase(out_fn3STR.length() - 4, 4);
                            out_fn3STR.append("_PreviewFrame");
                            out_fn3STR.append(slashCharStr.c_str());

                            //string CreateDir2 = out_fn3STR;
                            //CreateDir2.insert(0, "mkdir \"");
                            //CreateDir2.append("\"");
                            //system(CreateDir2.c_str());

                            //MakeDirVPX(out_fn3STR);
                            char intchar[56];
                            itoa_custom(frames_out, intchar, 10);
                            out_fn3STR.append(intchar);
                            out_fn3STR.append(".raw");
                            out3 = out_open(out_fn3STR.c_str(), 0);

                            if (imgPreview)
                            {
                                unsigned int y;
                                char out_fn[128+24];
                                uint8_t *buf;

                                //void *out, const uint8_t *buf, unsigned int len, int do_md5
                                //fwrite(buf, 1, len, (FILE *)out);

                                buf = imgPreview->planes[PLANE_Y];

                                for (y = 0; y < imgPreview->d_h; y++)
                                {
                                    out_put(out3, buf, imgPreview->d_w, 0);
                                    buf += imgPreview->stride[PLANE_Y];
                                }

                                buf = imgPreview->planes[PLANE_U];

                                for (y = 0; y < imgPreview->d_h / 2; y++)
                                {
                                    out_put(out3, buf, imgPreview->d_w / 2, 0);
                                    buf += imgPreview->stride[PLANE_U];
                                }

                                buf = imgPreview->planes[PLANE_V];

                                for (y = 0; y < imgPreview->d_h / 2; y++)
                                {
                                    out_put(out3, buf, imgPreview->d_w / 2, 0);
                                    buf += imgPreview->stride[PLANE_V];
                                }
                            }


                            string out_fn4STR = out_fn;
                            out_fn4STR.erase(out_fn4STR.length() - 4, 4);
                            out_fn4STR.append("_DecodeFrame");
                            out_fn4STR.append(slashCharStr.c_str());

                            //string CreateDir3 = out_fn4STR;
                            //CreateDir3.insert(0, "mkdir \"");
                            //CreateDir3.append("\"");
                            //system(CreateDir3.c_str());

                            //MakeDirVPX(out_fn4STR);
                            char intchar2[56];
                            itoa_custom(frames_out, intchar2, 10);
                            out_fn4STR.append(intchar2);
                            out_fn4STR.append(".raw");
                            out4 = out_open(out_fn4STR.c_str(), 0);

                            if (imgDecode)
                            {
                                unsigned int y;
                                char out_fn[128+24];
                                uint8_t *buf;

                                buf = imgDecode->planes[PLANE_Y];

                                for (y = 0; y < imgDecode->d_h; y++)
                                {
                                    out_put(out4, buf, imgDecode->d_w, 0);
                                    buf += imgDecode->stride[PLANE_Y];
                                }

                                buf = imgDecode->planes[PLANE_U];

                                for (y = 0; y < imgDecode->d_h / 2; y++)
                                {
                                    out_put(out4, buf, imgDecode->d_w / 2, 0);
                                    buf += imgDecode->stride[PLANE_U];
                                }

                                buf = imgDecode->planes[PLANE_V];

                                for (y = 0; y < imgDecode->d_h / 2; y++)
                                {
                                    out_put(out4, buf, imgDecode->d_w / 2, 0);
                                    buf += imgDecode->stride[PLANE_V];
                                }
                            }

                            out_close(out3, out_fn3STR.c_str(), 0);
                            out_close(out4, out_fn4STR.c_str(), 0);

                            //printf("\nMEMORY 1 NOT SAME size: %i For frame: %i\n",imgPreview->d_h*imgPreview->d_w,frames_out, frames_out);
                            //printf("\nFrame: %i",frames_out);
                        }

                        MemCheck1 = memcmp(imgPreview->planes[PLANE_Y], imgDecode->planes[PLANE_Y], imgPreview->d_h * imgPreview->d_w);

                        if (MemCheck1 != 0)
                        {
                            ReconOutFile << frames_out << " Y " << 0 << "\n";
                        }
                        else
                        {
                            ReconOutFile << frames_out << " Y " << 1 << "\n";
                        }

                        MemCheck2 = memcmp(imgPreview->planes[PLANE_U], imgDecode->planes[PLANE_U], (imgPreview->d_w / 2 * imgPreview->d_h / 2));

                        if (MemCheck2 != 0)
                        {
                            ReconOutFile << frames_out << " U " << 0 << "\n";
                        }
                        else
                        {
                            ReconOutFile << frames_out << " U " << 1 << "\n";
                        }

                        MemCheck3 = memcmp(imgPreview->planes[PLANE_V], imgDecode->planes[PLANE_V], (imgPreview->d_w / 2 * imgPreview->d_h / 2));

                        if (MemCheck3 != 0)
                        {
                            ReconOutFile << frames_out << " V " << 0 << "\n";
                        }
                        else
                        {
                            ReconOutFile << frames_out << " V " << 1 << "\n";
                        }
                    }

                    if (imgPreview)
                    {
                        unsigned int y;
                        char out_fn[128+24];
                        uint8_t *buf;

                        //void *out, const uint8_t *buf, unsigned int len, int do_md5
                        //fwrite(buf, 1, len, (FILE *)out);

                        buf = imgPreview->planes[PLANE_Y];

                        for (y = 0; y < imgPreview->d_h; y++)
                        {
                            out_put(out, buf, imgPreview->d_w, 0);
                            buf += imgPreview->stride[PLANE_Y];
                        }

                        buf = imgPreview->planes[PLANE_U];

                        for (y = 0; y < imgPreview->d_h / 2; y++)
                        {
                            out_put(out, buf, imgPreview->d_w / 2, 0);
                            buf += imgPreview->stride[PLANE_U];
                        }

                        buf = imgPreview->planes[PLANE_V];

                        for (y = 0; y < imgPreview->d_h / 2; y++)
                        {
                            out_put(out, buf, imgPreview->d_w / 2, 0);
                            buf += imgPreview->stride[PLANE_V];
                        }
                    }

                    if (imgDecode)
                    {
                        unsigned int y;
                        char out_fn[128+24];
                        uint8_t *buf;

                        buf = imgDecode->planes[PLANE_Y];

                        for (y = 0; y < imgDecode->d_h; y++)
                        {
                            out_put(out2, buf, imgDecode->d_w, 0);
                            buf += imgDecode->stride[PLANE_Y];
                        }

                        buf = imgDecode->planes[PLANE_U];

                        for (y = 0; y < imgDecode->d_h / 2; y++)
                        {
                            out_put(out2, buf, imgDecode->d_w / 2, 0);
                            buf += imgDecode->stride[PLANE_U];
                        }

                        buf = imgDecode->planes[PLANE_V];

                        for (y = 0; y < imgDecode->d_h / 2; y++)
                        {
                            out_put(out2, buf, imgDecode->d_w / 2, 0);
                            buf += imgDecode->stride[PLANE_V];
                        }
                    }

                    break;
                case ON2_CODEC_STATS_PKT:
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    break;
                default:
                    break;
                }
            }

            fflush(stdout);
        }

        /* this bitrate calc is simplified and relies on the fact that this
        * application uses 1/timebase for framerate.
        */
        {
            //uint64_t temp= (uint64_t)frames_in * 1000000;
        }
        on2_codec_destroy(&encoder);

        fclose(infile);
        out_close(out, out_fn2STR.c_str(), 0);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        //fclose(outfile3);//deleteme
        stats_close(&stats);
        printf("\n");
        fprintf(stderr, "\n");
    }

    on2_img_free(&raw);
    //fclose(ReconOutFile);
    ReconOutFile.close();

    if (RunQCheck == 1)
    {
        QuantOutFile.close();
    }

    return 0;
}
int DecompressIVFtoIVF(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                   *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};

    int CharCount = 0;

    printf("\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    fprintf(stderr, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open input file: %s", fn);
        fprintf(stderr, "Failed to open input file: %s", fn);
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);

    unsigned short width = 0;
    unsigned short height = 0;
    is_ivf = file_is_ivf_IVF(infile, &fourcc, (FILE *) out, &width, &height);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
        {
        }

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

                ivf_write_frameAPI((FILE *)out, TimeStamp, FrameSize);

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
int DecompressIVFtoRaw(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                  *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);



    is_ivf = file_is_ivf(infile, &fourcc);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (vp8_pp_cfg.post_proc_flag
        && vpx_codec_control(&decoder, VP8_SET_POSTPROC, &vp8_pp_cfg))
    {
        fprintf(stderr, "Failed to configure postproc: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            printf("decoded frame %d.\n", frame);

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }

            //else
            //{
            //	printf("\n\n NO IMG \n\n");
            //}
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("Decoded %d frames in %lu us (%.2f fps)\n",
               frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
int DecompressIVFtoRawNoErrorOutput(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                  *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);



    is_ivf = file_is_ivf(infile, &fourcc);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            printf("decoded frame %d.\n", frame);

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

                if (CharCount == 79)
                {
                    printf("\n");
                    //fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                //fprintf(stderr, ".");

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("Decoded %d frames in %lu us (%.2f fps)\n",
               frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
int DecompressIVFtoIVFNoOutput(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                   *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};

    int CharCount = 0;

    printf("\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);

    unsigned short width = 0;
    unsigned short height = 0;
    is_ivf = file_is_ivf_IVF(infile, &fourcc, (FILE *) out, &width, &height);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

                ivf_write_frameAPI((FILE *)out, TimeStamp, FrameSize);

                if (CharCount == 79)
                {
                    printf("\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
unsigned int TimeDecompressIVFtoIVF(char *inputchar, char *outputchar)
{
    //Time Decompress is not supposed to save output that is the only difference between it and
    //DecompressIVFtoIVFTimeAndOutput
    on2_codec_ctx_t       decoder;
    char                  *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 1, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};

    int CharCount = 0;

    printf("\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    fprintf(stderr, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);

    unsigned short width = 0;
    unsigned short height = 0;
    is_ivf = file_is_ivf_IVF(infile, &fourcc, (FILE *) out, &width, &height);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

                ivf_write_frameAPI((FILE *)out, TimeStamp, FrameSize);

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
        fprintf(stderr, "\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    char TextFilechar1[255];

    FolderName2(outputchar, TextFilechar1);

    char *FullName = strcat(TextFilechar1, "DecompressionTime.txt");

    ofstream outfile2(FullName);
    outfile2 << dx_time;
    outfile2.close();

    return dx_time;
}
unsigned int DecompressIVFtoIVFTimeAndOutput(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                   *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};

    int CharCount = 0;

    printf("\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    fprintf(stderr, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);

    unsigned short width = 0;
    unsigned short height = 0;
    is_ivf = file_is_ivf_IVF(infile, &fourcc, (FILE *) out, &width, &height);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
        {
            //printf("decoded frame %d.\n",frame);
        }

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

                ivf_write_frameAPI((FILE *)out, TimeStamp, FrameSize);

                if (CharCount == 79)
                {
                    printf("\n");
                    fprintf(stderr, "\n");
                    CharCount = 0;
                }

                CharCount++;
                printf(".");
                fprintf(stderr, ".");

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
        fprintf(stderr, "\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    char TextFilechar1[255];

    FolderName2(outputchar, TextFilechar1);

    char *FullName = strcat(TextFilechar1, "DecompressionTime.txt");

    ofstream outfile2(FullName);
    outfile2 << dx_time;
    outfile2.close();

    return dx_time;
}
int DecComputeMD5(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                  *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};

    int CharCount = 0;

    do_md5 = 1;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);



    is_ivf = file_is_ivf(infile, &fourcc);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (vp8_pp_cfg.post_proc_flag
        && vpx_codec_control(&decoder, VP8_SET_POSTPROC, &vp8_pp_cfg))
    {
        fprintf(stderr, "Failed to configure postproc: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            printf("decoded frame %d.\n", frame);

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

                /*if(CharCount == 79)
                {
                printf("\n");
                fprintf(stderr, "\n");
                CharCount=0;
                }
                CharCount++;
                printf(".");
                fprintf(stderr, ".");*/

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("Decoded %d frames in %lu us (%.2f fps)\n",
               frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
#endif
//--------------------------------------------------------IVF Tools-------------------------------------------------------------------------
int CutIVF(char *inputFile, char *outputFile, int StartingFrame, int EndingFrame)
{
    bool verbose = 1;

    FILE *out = fopen(outputFile, "wb");
    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        printf("\nInput file does not exist");
        fclose(in);
        fclose(out);
        return 0;
    }

    if (out == NULL)
    {
        printf("\nOutput file does not exist");
        fclose(in);
        fclose(out);
        return 0;
    }

    int currentVideoFrame = 0;
    int CharCount = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    memset(&ivfhRaw, 0, sizeof(ivfhRaw));
    int storage;
    fread(&ivfhRaw.signature, 1, 4, in);
    fread(&ivfhRaw.version, 1, 2, in);
    fread(&ivfhRaw.headersize, 1, 2, in);
    fread(&ivfhRaw.FourCC, 1, 4, in);
    fread(&ivfhRaw.width, 1, 2, in);
    fread(&ivfhRaw.height, 1, 2, in);
    fread(&ivfhRaw.rate, 1, 4, in);
    fread(&ivfhRaw.scale, 1, 4, in);
    fread(&ivfhRaw.length, 1, 4, in);
    fread(&storage, 1, 4, in);
    FormatIVFHeaderRead(&ivfhRaw);

    unsigned char *inputVideoBuffer = new unsigned char [ivfhRaw.width * ivfhRaw.height*3/2];
    /*printf( "Copy IVF to IVF\n\n"
    "FILE HEADER \n\n"
    "File Header            - %c%c%c%c \n"
    "File Format Version    - %i \n"
    "File Header Size       - %i \n"
    "Video Data FourCC      - %i \n"
    "Video Image Width      - %i \n"
    "Video Image Height     - %i \n"
    "Frame Rate Rate        - %i \n"
    "Frame Rate Scale       - %i \n"
    "Video Length in Frames - %i \n"
    "Unused				    - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    /*printf("FRAME HEADER 0\n\n"
    "Frame Size            - %i \n"
    "Time Stamp            - %i \n"
    "\n"

    ,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);*/

    IVF_HEADER ivfhCompressed;
    InitIVFHeader(&ivfhCompressed);
    memset(&ivfhCompressed, 0, sizeof(ivfhCompressed));
    ivfhCompressed = ivfhRaw;
    IVF_FRAME_HEADER ivf_fhCompressed;
    FormatIVFHeaderWrite(ivfhCompressed);
    fwrite((char *)&ivfhCompressed, 1, 32, out);
    FormatIVFHeaderRead(&ivfhCompressed);
    printf("\nCopy IVF file to IVF file: \n");

    int frameCount = ivfhRaw.length;
    currentVideoFrame = 0;
    int WrittenoutVideoFrames = 0;
    int NewTimeStamp = 0;

    while (currentVideoFrame < frameCount)
    {
        if (currentVideoFrame >= StartingFrame && currentVideoFrame <= EndingFrame)
        {
            memset(inputVideoBuffer, 0, sizeof(inputVideoBuffer));

            if (!feof(in))
            {
                fread(&ivf_fhRaw.frameSize, 1, 4, in);
                fread(&ivf_fhRaw.timeStamp, 1, 8, in);
                FormatFrameHeaderRead(ivf_fhRaw);
                fread(inputVideoBuffer, 1, ivf_fhRaw.frameSize, in);

                //ivf_fhCompressed.timeStamp = NewTimeStamp;


                /*	if(currentVideoFrame != frameCount)
                {
                printf("FRAME HEADER %i\n\n"
                "Frame Size            - %i \n"
                "Time Stamp            - %i \n"
                "\n"

                ,currentVideoFrame,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);
                }*/
            }
            else
            {
                break;
            }

            memset(&ivf_fhCompressed, 0, sizeof(ivf_fhCompressed));

            unsigned int FrameSizeStorage = ivf_fhRaw.frameSize;
            ivf_fhCompressed.frameSize = ivf_fhRaw.frameSize;
            ivf_fhCompressed.timeStamp = NewTimeStamp;
            //ivf_fhCompressed.timeStamp = ivf_fhRaw.timeStamp;//0;		// not defined, use rate and scale in header replaced by line: memset(&ivf_fh, 0, sizeof(ivf_fh));

            FormatFrameHeaderWrite(ivf_fhCompressed);
            fwrite(&ivf_fhCompressed.frameSize, 1, sizeof(ivf_fhCompressed.frameSize), out);
            fwrite(&ivf_fhCompressed.timeStamp, 1, sizeof(ivf_fhCompressed.timeStamp), out);
            fwrite(inputVideoBuffer, 1, FrameSizeStorage, out);


            if (CharCount == 79)
            {
                printf("\n");
                CharCount = 0;
            }

            printf("*");
            ++currentVideoFrame;
            WrittenoutVideoFrames++;
            CharCount++;
            NewTimeStamp = NewTimeStamp + 2;

            ivf_fhRaw.frameSize = 0;
            ivf_fhRaw.timeStamp = 0;

            //memset(inputVideoBuffer,0,sizeof(inputVideoBuffer));

            //if(!feof(in))
            //{
            //	fread(&ivf_fhRaw.frameSize,1,4,in);
            //	fread(&ivf_fhRaw.timeStamp,1,8,in);
            //	FormatFrameHeaderRead(ivf_fhRaw);
            //	fread(inputVideoBuffer,1,ivf_fhRaw.frameSize,in);

            //	/*	if(currentVideoFrame != frameCount)
            //	{
            //	printf("FRAME HEADER %i\n\n"
            //	"Frame Size            - %i \n"
            //	"Time Stamp            - %i \n"
            //	"\n"

            //	,currentVideoFrame,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);
            //	}*/
            //}
            //else
            //{
            //	break;
            //}
        }
        else
        {
            if (CharCount == 79)
            {
                printf("\n");
                CharCount = 0;
            }

            printf(".");

            CharCount++;

            if (!feof(in))
            {
                fread(&ivf_fhRaw.frameSize, 1, 4, in);
                fread(&ivf_fhRaw.timeStamp, 1, 8, in);
                FormatFrameHeaderRead(ivf_fhRaw);
                fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);
            }
            else
            {
                break;
            }

            currentVideoFrame++;
        }
    }

    ivfhCompressed.length = WrittenoutVideoFrames;
    fseek(out , 0 , SEEK_SET);
    FormatIVFHeaderWrite(ivfhCompressed);
    fwrite(&ivfhCompressed, 1, 32, out);

    fclose(in);
    fclose(out);

    delete [] inputVideoBuffer;

    return(0);
}
int PasteIVF(char *inputFile1, char *inputFile2, char *outputFile, int StartingFrame)
{
    bool verbose = 1;

    FILE *out = fopen(outputFile, "wb");
    FILE *in1 = fopen(inputFile1, "rb");
    FILE *in2 = fopen(inputFile2, "rb");

    if (in1 == NULL)
    {
        printf("\nInput file 1 does not exist");
        fclose(in1);
        fclose(in2);
        fclose(out);
        return 0;
    }

    if (in2 == NULL)
    {
        printf("\nInput file 2 does not exist");
        fclose(in1);
        fclose(in2);
        fclose(out);
        return 0;
    }

    if (out == NULL)
    {
        printf("\nOutput file does not exist");
        fclose(in1);
        fclose(in2);
        fclose(out);
        return 0;
    }

    int currentVideoFrame = 0;
    int CharCount = 0;

    IVF_HEADER ivfhRaw1;

    InitIVFHeader(&ivfhRaw1);
    memset(&ivfhRaw1, 0, sizeof(ivfhRaw1));
    int storage;
    fread(&ivfhRaw1.signature, 1, 4, in1);
    fread(&ivfhRaw1.version, 1, 2, in1);
    fread(&ivfhRaw1.headersize, 1, 2, in1);
    fread(&ivfhRaw1.FourCC, 1, 4, in1);
    fread(&ivfhRaw1.width, 1, 2, in1);
    fread(&ivfhRaw1.height, 1, 2, in1);
    fread(&ivfhRaw1.rate, 1, 4, in1);
    fread(&ivfhRaw1.scale, 1, 4, in1);
    fread(&ivfhRaw1.length, 1, 4, in1);
    fread(&storage, 1, 4, in1);
    FormatIVFHeaderRead(&ivfhRaw1);

    unsigned char *inputVideoBuffer1 = new unsigned char [ivfhRaw1.width * ivfhRaw1.height*3/2];
    /*printf( "Copy IVF to IVF\n\n"
    "FILE HEADER \n\n"
    "File Header            - %c%c%c%c \n"
    "File Format Version    - %i \n"
    "File Header Size       - %i \n"
    "Video Data FourCC      - %i \n"
    "Video Image Width      - %i \n"
    "Video Image Height     - %i \n"
    "Frame Rate Rate        - %i \n"
    "Frame Rate Scale       - %i \n"
    "Video Length in Frames - %i \n"
    "Unused				    - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/
    IVF_FRAME_HEADER ivf_fhRaw1;
    fread(&ivf_fhRaw1.frameSize, 1, 4, in1);
    fread(&ivf_fhRaw1.timeStamp, 1, 8, in1);
    FormatFrameHeaderRead(ivf_fhRaw1);
    fread(inputVideoBuffer1, 1, ivf_fhRaw1.frameSize, in1);

    IVF_HEADER ivfhRaw2;

    InitIVFHeader(&ivfhRaw2);
    memset(&ivfhRaw2, 0, sizeof(ivfhRaw2));
    fread(&ivfhRaw2.signature, 1, 4, in2);
    fread(&ivfhRaw2.version, 1, 2, in2);
    fread(&ivfhRaw2.headersize, 1, 2, in2);
    fread(&ivfhRaw2.FourCC, 1, 4, in2);
    fread(&ivfhRaw2.width, 1, 2, in2);
    fread(&ivfhRaw2.height, 1, 2, in2);
    fread(&ivfhRaw2.rate, 1, 4, in2);
    fread(&ivfhRaw2.scale, 1, 4, in2);
    fread(&ivfhRaw2.length, 1, 4, in2);
    fread(&storage, 1, 4, in2);
    FormatIVFHeaderRead(&ivfhRaw2);

    unsigned char *inputVideoBuffer2 = new unsigned char [ivfhRaw2.width * ivfhRaw2.height*3/2];
    /*printf( "Copy IVF to IVF\n\n"
    "FILE HEADER \n\n"
    "File Header            - %c%c%c%c \n"
    "File Format Version    - %i \n"
    "File Header Size       - %i \n"
    "Video Data FourCC      - %i \n"
    "Video Image Width      - %i \n"
    "Video Image Height     - %i \n"
    "Frame Rate Rate        - %i \n"
    "Frame Rate Scale       - %i \n"
    "Video Length in Frames - %i \n"
    "Unused				    - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/
    IVF_FRAME_HEADER ivf_fhRaw2;
    fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
    fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
    FormatFrameHeaderRead(ivf_fhRaw2);
    fread(inputVideoBuffer2, 1, ivf_fhRaw2.frameSize, in2);


    /*printf("FRAME HEADER 0\n\n"
    "Frame Size            - %i \n"
    "Time Stamp            - %i \n"
    "\n"

    ,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);*/

    IVF_HEADER ivfhCompressed;
    InitIVFHeader(&ivfhCompressed);
    memset(&ivfhCompressed, 0, sizeof(ivfhCompressed));
    ivfhCompressed = ivfhRaw1;
    IVF_FRAME_HEADER ivf_fhCompressed;
    FormatIVFHeaderWrite(ivfhCompressed);
    fwrite((char *)&ivfhCompressed, 1, 32, out);
    FormatIVFHeaderRead(&ivfhCompressed);
    printf("\nPasting IVF file into IVF file: \n");

    int frameCount = ivfhRaw1.length + ivfhRaw2.length;
    int EndingFrame = StartingFrame + ivfhRaw2.length;
    currentVideoFrame = 0;
    int WrittenoutVideoFrames = 0;

    while (currentVideoFrame <= frameCount)
    {
        if (currentVideoFrame >= StartingFrame && currentVideoFrame <= EndingFrame)
        {
            memset(&ivf_fhCompressed, 0, sizeof(ivf_fhCompressed));

            unsigned int FrameSizeStorage = ivf_fhRaw2.frameSize;
            ivf_fhCompressed.frameSize = ivf_fhRaw2.frameSize;
            ivf_fhCompressed.timeStamp = ivf_fhRaw2.timeStamp;//0;		// not defined, use rate and scale in header replaced by line: memset(&ivf_fh, 0, sizeof(ivf_fh));

            FormatFrameHeaderWrite(ivf_fhCompressed);
            fwrite(&ivf_fhCompressed.frameSize, 1, sizeof(ivf_fhCompressed.frameSize), out);
            fwrite(&ivf_fhCompressed.timeStamp, 1, sizeof(ivf_fhCompressed.timeStamp), out);
            fwrite(inputVideoBuffer2, 1, FrameSizeStorage, out);


            if (CharCount == 79)
            {
                printf("\n");
                CharCount = 0;
            }

            printf("+");
            ++currentVideoFrame;
            WrittenoutVideoFrames++;
            CharCount++;

            ivf_fhRaw2.frameSize = 0;
            ivf_fhRaw2.timeStamp = 0;

            memset(inputVideoBuffer2, 0, sizeof(inputVideoBuffer2));

            if (!feof(in2))
            {
                fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
                fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
                FormatFrameHeaderRead(ivf_fhRaw2);
                fread(inputVideoBuffer2, 1, ivf_fhRaw2.frameSize, in2);

                /*	if(currentVideoFrame != frameCount)
                {
                printf("FRAME HEADER %i\n\n"
                "Frame Size            - %i \n"
                "Time Stamp            - %i \n"
                "\n"

                ,currentVideoFrame,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);
                }*/
            }

            /*else
            {
            break;
            }*/
        }
        else
        {
            memset(&ivf_fhCompressed, 0, sizeof(ivf_fhCompressed));

            unsigned int FrameSizeStorage = ivf_fhRaw1.frameSize;
            ivf_fhCompressed.frameSize = ivf_fhRaw1.frameSize;
            ivf_fhCompressed.timeStamp = ivf_fhRaw1.timeStamp;//0;		// not defined, use rate and scale in header replaced by line: memset(&ivf_fh, 0, sizeof(ivf_fh));

            FormatFrameHeaderWrite(ivf_fhCompressed);
            fwrite(&ivf_fhCompressed.frameSize, 1, sizeof(ivf_fhCompressed.frameSize), out);
            fwrite(&ivf_fhCompressed.timeStamp, 1, sizeof(ivf_fhCompressed.timeStamp), out);
            fwrite(inputVideoBuffer1, 1, FrameSizeStorage, out);


            if (CharCount == 79)
            {
                printf("\n");
                CharCount = 0;
            }

            printf(".");
            ++currentVideoFrame;
            WrittenoutVideoFrames++;
            CharCount++;

            ivf_fhRaw1.frameSize = 0;
            ivf_fhRaw1.timeStamp = 0;

            memset(inputVideoBuffer1, 0, sizeof(inputVideoBuffer1));


            if (!feof(in1))
            {
                fread(&ivf_fhRaw1.frameSize, 1, 4, in1);
                fread(&ivf_fhRaw1.timeStamp, 1, 8, in1);
                FormatFrameHeaderRead(ivf_fhRaw1);
                fread(inputVideoBuffer1, 1, ivf_fhRaw1.frameSize, in1);

                /*	if(currentVideoFrame != frameCount)
                {
                printf("FRAME HEADER %i\n\n"
                "Frame Size            - %i \n"
                "Time Stamp            - %i \n"
                "\n"

                ,currentVideoFrame,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);
                }*/
            }

            /*else
            {
            break;
            }*/
        }
    }

    ivfhCompressed.length = WrittenoutVideoFrames;
    fseek(out , 0 , SEEK_SET);
    FormatIVFHeaderWrite(ivfhCompressed);
    fwrite(&ivfhCompressed, 1, 32, out);

    fclose(in1);
    fclose(in2);
    fclose(out);

    delete [] inputVideoBuffer1;
    delete [] inputVideoBuffer2;

    return(0);
}
int DisplayIVFHeaderInfo(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("\n"
               "  DisplayIVFHeaderInfo \n\n"
               "    <Inputfile>\n"
               "    <Full File Info 1/yes-0/no>\n"
               "    <Optional Outputfile>\n\n"
              );
        return 0;
    }

    char *inputFile = argv[2];
    int extrafileinfo = 0;

    if (argc > 3)
    {
        extrafileinfo = atoi(argv[3]);
    }


    if (argc > 4)
    {
        char *outputFile = argv[4];

        FILE *in = fopen(inputFile, "rb");
        FILE *fp;

        if ((fp = freopen(outputFile, "w", stderr)) == NULL)
        {
            printf("Cannot open out put file: %s\n", outputFile);
            fclose(in);
            exit(1);
        }

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        printf("\n"
               "FILE HEADER \n\n"
               "File Header            - %c%c%c%c \n"
               "File Format Version    - %i \n"
               "File Header Size       - %i \n"
               "Video Data FourCC      - %i \n"
               "Video Image Width      - %i \n"
               "Video Image Height     - %i \n"
               "Frame Rate Rate        - %i \n"
               "Frame Rate Scale       - %i \n"
               "Video Length in Frames - %i \n"
               "Unused                 - %c \n"
               "\n\n"
               , ivfhRaw.signature[0], ivfhRaw.signature[1], ivfhRaw.signature[2], ivfhRaw.signature[3]
               , ivfhRaw.version, ivfhRaw.headersize, ivfhRaw.FourCC, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate
               , ivfhRaw.scale, ivfhRaw.length, ivfhRaw.unused);

        fprintf(stderr, "FILE HEADER \n\n"
                "File Header            - %c%c%c%c \n"
                "File Format Version    - %i \n"
                "File Header Size       - %i \n"
                "Video Data FourCC      - %i \n"
                "Video Image Width      - %i \n"
                "Video Image Height     - %i \n"
                "Frame Rate Rate        - %i \n"
                "Frame Rate Scale       - %i \n"
                "Video Length in Frames - %i \n"
                "Unused                 - %c \n"
                "\n\n"
                , ivfhRaw.signature[0], ivfhRaw.signature[1], ivfhRaw.signature[2], ivfhRaw.signature[3]
                , ivfhRaw.version, ivfhRaw.headersize, ivfhRaw.FourCC, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate
                , ivfhRaw.scale, ivfhRaw.length, ivfhRaw.unused);


        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), in);
        FormatFrameHeaderRead(ivf_fhRaw);

        fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);

        printf("FRAME HEADER 0\n"
               "Frame Size            - %i \n"
               "Time Stamp            - %i \n"
               "\n"

               , ivf_fhRaw.frameSize, (int)ivf_fhRaw.timeStamp);


        fprintf(stderr, "FRAME HEADER 0\n"
                "Frame Size            - %i \n"
                "Time Stamp            - %i \n"
                "\n"

                , ivf_fhRaw.frameSize, ivf_fhRaw.timeStamp);

        if (extrafileinfo != 1)
        {
            fclose(in);
            fclose(fp);
            return 0;
        }

        int frameCount = ivfhRaw.length;
        int currentVideoFrame = 1;

        while (1)
        {
            fread(&ivf_fhRaw.frameSize, 1, 4, in);

            if (feof(in))break;

            fread(&ivf_fhRaw.timeStamp, 1, 8, in);

            if (feof(in))break;

            FormatFrameHeaderRead(ivf_fhRaw);

            fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);

            if (feof(in))break;

            printf("FRAME HEADER %i\n"
                   "Frame Size            - %i \n"
                   "Time Stamp            - %i \n"
                   "\n"

                   , currentVideoFrame, ivf_fhRaw.frameSize, (int)ivf_fhRaw.timeStamp);


            fprintf(stderr, "FRAME HEADER %i\n"
                    "Frame Size            - %i \n"
                    "Time Stamp            - %i \n"
                    "\n"

                    , currentVideoFrame, ivf_fhRaw.frameSize, ivf_fhRaw.timeStamp);

            currentVideoFrame++;
        }

        fclose(in);
        fclose(fp);
    }

    else
    {
        FILE *in = fopen(inputFile, "rb");

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);

        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        printf("\n"
               "FILE HEADER \n\n"
               "File Header            - %c%c%c%c \n"
               "File Format Version    - %i \n"
               "File Header Size       - %i \n"
               "Video Data FourCC      - %i \n"
               "Video Image Width      - %i \n"
               "Video Image Height     - %i \n"
               "Frame Rate Rate        - %i \n"
               "Frame Rate Scale       - %i \n"
               "Video Length in Frames - %i \n"
               "Unused                 - %c \n"
               "\n\n"
               , ivfhRaw.signature[0], ivfhRaw.signature[1], ivfhRaw.signature[2], ivfhRaw.signature[3]
               , ivfhRaw.version, ivfhRaw.headersize, ivfhRaw.FourCC, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate
               , ivfhRaw.scale, ivfhRaw.length, ivfhRaw.unused);

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), in);
        FormatFrameHeaderRead(ivf_fhRaw);

        printf("FRAME HEADER 0\n"
               "Frame Size            - %i \n"
               "Time Stamp            - %i \n"
               "\n"

               , ivf_fhRaw.frameSize, (int)ivf_fhRaw.timeStamp);

        fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);

        int frameCount = ivfhRaw.length;
        int currentVideoFrame = 1;

        if (extrafileinfo != 1)
        {
            fclose(in);
            return 0;
        }

        while (1)
        {
            fread(&ivf_fhRaw.frameSize, 1, 4, in);

            if (feof(in))break;

            fread(&ivf_fhRaw.timeStamp, 1, 8, in);

            if (feof(in))break;

            FormatFrameHeaderRead(ivf_fhRaw);

            fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);

            if (feof(in))break;

            printf("FRAME HEADER %i\n"
                   "Frame Size            - %i \n"
                   "Time Stamp            - %i \n"
                   "\n"

                   , currentVideoFrame, ivf_fhRaw.frameSize, (int)ivf_fhRaw.timeStamp);

            currentVideoFrame++;
        }

        fclose(in);

    }

    return 0;
}
int CompareIVFHeaderInfo(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("\n"
               "  CompareIVFHeaderInfo \n\n"
               "    <inputfile1>\n"
               "    <inputfile2>\n"
               "    <Full File Info 1/yes-0/no>\n"
               "    <OutputFile>\n"
               "\n ");

        return 0;
    }

    char *inputFile1 = argv[2];
    char *inputFile2 = argv[3];

    int extrafileinfo;

    if (argc > 3)
    {
        extrafileinfo = atoi(argv[4]);
    }
    else
    {
        extrafileinfo = 0;
    }

    string strinputFile1 = inputFile1;
    string strinputFile2 = inputFile2;

    FILE *in = fopen(inputFile1, "rb");
    FILE *in2 = fopen(inputFile2, "rb");

    if (in == NULL)
    {
        printf("\nInput file 1 does not exist");
        fprintf(stderr, "\nInput file 1 does not exist");
        fclose(in);
        fclose(in2);
        return 0;
    }

    if (in2 == NULL)
    {
        printf("\nInput file 2 does not exist");
        fprintf(stderr, "\nInput file 2 does not exist");
        fclose(in);
        fclose(in2);
        return 0;
    }

    if (argc < 6) //if not output file
    {
        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);

        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        IVF_HEADER ivfhRaw2;

        InitIVFHeader(&ivfhRaw2);

        fread(&ivfhRaw2, 1, sizeof(ivfhRaw2), in2);
        FormatIVFHeaderRead(&ivfhRaw2);

        printf("\n"
               "        FILE HEADER1                        FILE HEADER2\n\n"
               "File Header            - %c%c%c%-*cFile Header            - %c%c%c%c\n"
               "File Format Version    - %-*iFile Format Version    - %i\n"
               "File Header Size       - %-*iFile Header Size       - %i\n"
               "Video Data FourCC      - %-*iVideo Data FourCC      - %i\n"
               "Video Image Width      - %-*iVideo Image Width      - %i\n"
               "Video Image Height     - %-*iVideo Image Height     - %i\n"
               "Frame Rate Rate        - %-*iFrame Rate Rate        - %i\n"
               "Frame Rate Scale       - %-*iFrame Rate Scale       - %i\n"
               "Video Length in Frames - %-*iVideo Length in Frames - %i\n"
               "Unused                 - %-*cUnused                 - %c\n"
               "\n\n"
               , ivfhRaw.signature[0], ivfhRaw.signature[1], ivfhRaw.signature[2], 9, ivfhRaw.signature[3]
               , ivfhRaw2.signature[0], ivfhRaw2.signature[1], ivfhRaw2.signature[2], ivfhRaw2.signature[3]
               , 12, ivfhRaw.version, ivfhRaw2.version
               , 12, ivfhRaw.headersize, ivfhRaw2.headersize
               , 12, ivfhRaw.FourCC, ivfhRaw2.FourCC
               , 12, ivfhRaw.width, ivfhRaw2.width
               , 12, ivfhRaw.height, ivfhRaw2.height
               , 12, ivfhRaw.rate, ivfhRaw2.rate
               , 12, ivfhRaw.scale, ivfhRaw2.scale
               , 12, ivfhRaw.length, ivfhRaw2.length
               , 12, ivfhRaw.unused, ivfhRaw2.unused);

        IVF_FRAME_HEADER ivf_fhRaw;


        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);



        IVF_FRAME_HEADER ivf_fhRaw2;

        fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
        fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
        FormatFrameHeaderRead(ivf_fhRaw2);

        fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);

        int currentVideoFrame = 0;

        printf("FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
               "Frame Size            - %-*iFrame Size            - %i\n"
               "Time Stamp            - %-*iTime Stamp            - %i\n"
               "\n"

               , 22, currentVideoFrame, currentVideoFrame
               , 12, ivf_fhRaw.frameSize, ivf_fhRaw2.frameSize
               , 12, (int)ivf_fhRaw.timeStamp, (int)ivf_fhRaw2.timeStamp);

        int frameCount = 0;
        int frameCount1 = ivfhRaw.length;
        int frameCount2 = ivfhRaw2.length;

        if (frameCount1 >= frameCount2)
            frameCount = frameCount1;
        else
            frameCount = frameCount2;

        currentVideoFrame++;

        if (extrafileinfo != 1)
        {
            fclose(in);
            fclose(in2);
            return 0;
        }

        while (currentVideoFrame < frameCount)
        {
            if (currentVideoFrame <= frameCount1)
            {
                fread(&ivf_fhRaw.frameSize, 1, 4, in);
                fread(&ivf_fhRaw.timeStamp, 1, 8, in);
                FormatFrameHeaderRead(ivf_fhRaw);

                fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount2)
            {
                fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
                fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
                FormatFrameHeaderRead(ivf_fhRaw2);

                fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount1 && currentVideoFrame <= frameCount2)
            {
                printf("FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
                       "Frame Size            - %-*iFrame Size            - %i\n"
                       "Time Stamp            - %-*iTime Stamp            - %i\n"
                       "\n"

                       , 22, currentVideoFrame, currentVideoFrame
                       , 12, ivf_fhRaw.frameSize, ivf_fhRaw2.frameSize
                       , 12, (int)ivf_fhRaw.timeStamp, (int)ivf_fhRaw2.timeStamp);
            }
            else
            {
                if (currentVideoFrame <= frameCount2)
                {
                    printf(
                        "                                    FRAME HEADER2 %i\n"
                        "                                    Frame Size            - %i\n"
                        "                                    Time Stamp            - %i\n"
                        "\n"

                        , currentVideoFrame
                        , ivf_fhRaw2.frameSize
                        , (int)ivf_fhRaw2.timeStamp);
                }
                else
                {
                    printf(
                        "FRAME HEADER1 %-*i\n"
                        "Frame Size            - %-*i\n"
                        "Time Stamp            - %-*i\n"
                        "\n"

                        , 22, currentVideoFrame
                        , 12, ivf_fhRaw.frameSize
                        , 12, (int)ivf_fhRaw.timeStamp);
                }
            }

            currentVideoFrame++;
        }
    }
    else
    {

        char *outputfile = argv[5];

        FILE *fp;

        if ((fp = freopen(outputfile, "w", stderr)) == NULL)
        {
            printf("Cannot open out put file: %s\n", outputfile);
            exit(1);
        }

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);

        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        IVF_HEADER ivfhRaw2;

        InitIVFHeader(&ivfhRaw2);

        fread(&ivfhRaw2, 1, sizeof(ivfhRaw2), in2);
        FormatIVFHeaderRead(&ivfhRaw2);

        printf("\n"
               "        FILE HEADER1                        FILE HEADER2\n\n"
               "File Header            - %c%c%c%-*cFile Header            - %c%c%c%c\n"
               "File Format Version    - %-*iFile Format Version    - %i\n"
               "File Header Size       - %-*iFile Header Size       - %i\n"
               "Video Data FourCC      - %-*iVideo Data FourCC      - %i\n"
               "Video Image Width      - %-*iVideo Image Width      - %i\n"
               "Video Image Height     - %-*iVideo Image Height     - %i\n"
               "Frame Rate Rate        - %-*iFrame Rate Rate        - %i\n"
               "Frame Rate Scale       - %-*iFrame Rate Scale       - %i\n"
               "Video Length in Frames - %-*iVideo Length in Frames - %i\n"
               "Unused                 - %-*cUnused                 - %c\n"
               "\n\n"
               , ivfhRaw.signature[0], ivfhRaw.signature[1], ivfhRaw.signature[2], 9, ivfhRaw.signature[3]
               , ivfhRaw2.signature[0], ivfhRaw2.signature[1], ivfhRaw2.signature[2], ivfhRaw2.signature[3]
               , 12, ivfhRaw.version, ivfhRaw2.version
               , 12, ivfhRaw.headersize, ivfhRaw2.headersize
               , 12, ivfhRaw.FourCC, ivfhRaw2.FourCC
               , 12, ivfhRaw.width, ivfhRaw2.width
               , 12, ivfhRaw.height, ivfhRaw2.height
               , 12, ivfhRaw.rate, ivfhRaw2.rate
               , 12, ivfhRaw.scale, ivfhRaw2.scale
               , 12, ivfhRaw.length, ivfhRaw2.length
               , 12, ivfhRaw.unused, ivfhRaw2.unused);

        fprintf(stderr, "        FILE HEADER1                        FILE HEADER2\n\n"
                "File Header            - %c%c%c%-*cFile Header            - %c%c%c%c\n"
                "File Format Version    - %-*iFile Format Version    - %i\n"
                "File Header Size       - %-*iFile Header Size       - %i\n"
                "Video Data FourCC      - %-*iVideo Data FourCC      - %i\n"
                "Video Image Width      - %-*iVideo Image Width      - %i\n"
                "Video Image Height     - %-*iVideo Image Height     - %i\n"
                "Frame Rate Rate        - %-*iFrame Rate Rate        - %i\n"
                "Frame Rate Scale       - %-*iFrame Rate Scale       - %i\n"
                "Video Length in Frames - %-*iVideo Length in Frames - %i\n"
                "Unused                 - %-*cUnused                 - %c\n"
                "\n\n"
                , ivfhRaw.signature[0], ivfhRaw.signature[1], ivfhRaw.signature[2], 9, ivfhRaw.signature[3]
                , ivfhRaw2.signature[0], ivfhRaw2.signature[1], ivfhRaw2.signature[2], ivfhRaw2.signature[3]
                , 12, ivfhRaw.version, ivfhRaw2.version
                , 12, ivfhRaw.headersize, ivfhRaw2.headersize
                , 12, ivfhRaw.FourCC, ivfhRaw2.FourCC
                , 12, ivfhRaw.width, ivfhRaw2.width
                , 12, ivfhRaw.height, ivfhRaw2.height
                , 12, ivfhRaw.rate, ivfhRaw2.rate
                , 12, ivfhRaw.scale, ivfhRaw2.scale
                , 12, ivfhRaw.length, ivfhRaw2.length
                , 12, ivfhRaw.unused, ivfhRaw2.unused);

        IVF_FRAME_HEADER ivf_fhRaw;


        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);



        IVF_FRAME_HEADER ivf_fhRaw2;

        fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
        fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
        FormatFrameHeaderRead(ivf_fhRaw2);

        fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);

        int currentVideoFrame = 0;

        cerr << "\n Time 2 Stamp: " << ivf_fhRaw2.timeStamp << "\n";

        printf("FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
               "Frame Size            - %-*iFrame Size            - %i\n"
               "Time Stamp            - %-*iTime Stamp            - %-*i\n"
               "\n"

               , 22, currentVideoFrame, currentVideoFrame
               , 12, ivf_fhRaw.frameSize, ivf_fhRaw2.frameSize
               , 12, (int)ivf_fhRaw.timeStamp, (int)ivf_fhRaw2.timeStamp);

        fprintf(stderr, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
                "Frame Size            - %-*iFrame Size            - %i\n"
                "Time Stamp            - %-*iTime Stamp            - %-*i\n"
                "\n"

                , 22, currentVideoFrame, currentVideoFrame
                , 12, ivf_fhRaw.frameSize, ivf_fhRaw2.frameSize
                , 12, ivf_fhRaw.timeStamp, ivf_fhRaw2.timeStamp);


        int frameCount = 0;
        int frameCount1 = ivfhRaw.length;
        int frameCount2 = ivfhRaw2.length;

        if (frameCount1 >= frameCount2)
            frameCount = frameCount1;
        else
            frameCount = frameCount2;

        currentVideoFrame++;

        if (extrafileinfo != 1)
        {
            fclose(fp);
            fclose(in);
            fclose(in2);
            return 0;
        }

        while (currentVideoFrame < frameCount)
        {
            //if file 1 isnt at the end read the next data in
            if (currentVideoFrame <= frameCount1)
            {
                fread(&ivf_fhRaw.frameSize, 1, 4, in);
                fread(&ivf_fhRaw.timeStamp, 1, 8, in);
                FormatFrameHeaderRead(ivf_fhRaw);

                fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount2)
            {
                fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
                fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
                FormatFrameHeaderRead(ivf_fhRaw2);

                fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount1 && currentVideoFrame <= frameCount2)
            {
                printf("FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
                       "Frame Size            - %-*iFrame Size            - %i\n"
                       "Time Stamp            - %-*iTime Stamp            - %i\n"
                       "\n"

                       , 22, currentVideoFrame, currentVideoFrame
                       , 12, ivf_fhRaw.frameSize, ivf_fhRaw2.frameSize
                       , 12, (int)ivf_fhRaw.timeStamp, (int)ivf_fhRaw2.timeStamp);

                fprintf(stderr, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
                        "Frame Size            - %-*iFrame Size            - %i\n"
                        "Time Stamp            - %-*iTime Stamp            - %i\n"
                        "\n"

                        , 22, currentVideoFrame, currentVideoFrame
                        , 12, ivf_fhRaw.frameSize, ivf_fhRaw2.frameSize
                        , 12, (int)ivf_fhRaw.timeStamp, (int)ivf_fhRaw2.timeStamp);
            }
            else
            {
                if (currentVideoFrame <= frameCount2)
                {
                    printf(
                        "                                    FRAME HEADER2 %i\n"
                        "                                    Frame Size            - %i\n"
                        "                                    Time Stamp            - %i\n"
                        "\n"

                        , currentVideoFrame
                        , ivf_fhRaw2.frameSize
                        , (int)ivf_fhRaw2.timeStamp);

                    fprintf(stderr,
                            "                                    FRAME HEADER2 %i\n"
                            "                                    Frame Size            - %i\n"
                            "                                    Time Stamp            - %i\n"
                            "\n"

                            , currentVideoFrame
                            , ivf_fhRaw2.frameSize
                            , (int)ivf_fhRaw2.timeStamp);
                }
                else
                {
                    printf(
                        "FRAME HEADER1 %-*i\n"
                        "Frame Size            - %-*i\n"
                        "Time Stamp            - %-*i\n"
                        "\n"

                        , 22, currentVideoFrame
                        , 12, ivf_fhRaw.frameSize
                        , 12, (int)ivf_fhRaw.timeStamp);

                    fprintf(stderr,
                            "FRAME HEADER1 %-*i\n"
                            "Frame Size            - %-*i\n"
                            "Time Stamp            - %-*i\n"
                            "\n"

                            , 22, currentVideoFrame
                            , 12, ivf_fhRaw.frameSize
                            , 12, (int)ivf_fhRaw.timeStamp);
                }
            }

            currentVideoFrame++;
        }

        fclose(fp);
    }


    fclose(in);
    fclose(in2);
    return 0;
}
int CompIVF(char *inputFile1, char *inputFile2)
{
    ////Returns:
    //-1 if files are identical
    //-2 if file 2 ends before File 1
    //-3 if file 1 ends before File 2
    //and >= 0 where the number the function returns is the frame that they differ first on.

    FILE *in = fopen(inputFile1, "rb");
    FILE *in2 = fopen(inputFile2, "rb");

    int returnval = -1;

    if (in == NULL)
    {
        printf("\nFile 1 does not exist");
        fprintf(stderr, "\nFile 1 does not exist");
        fclose(in);
        fclose(in2);
        return 0;
    }

    if (in2 == NULL)
    {
        printf("\nFile 2 does not exist");
        fprintf(stderr, "\nFile 2 does not exist");
        fclose(in);
        fclose(in2);
        return 0;
    }

    int currentVideoFrame = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, 32, in);
    FormatIVFHeaderRead(&ivfhRaw);

    IVF_HEADER ivfhRaw2;

    InitIVFHeader(&ivfhRaw2);
    fread(&ivfhRaw2, 1, 32, in2);
    FormatIVFHeaderRead(&ivfhRaw2);

    unsigned char *inputVideoBuffer = new unsigned char [ivfhRaw.width * ivfhRaw.height*3];
    unsigned char *inputVideo2Buffer = new unsigned char [ivfhRaw2.width * ivfhRaw2.height*3];
    memset(inputVideoBuffer, 0, ivfhRaw.width * ivfhRaw.height * 3);
    memset(inputVideo2Buffer, 0, ivfhRaw2.width * ivfhRaw2.height * 3);

    //////////////////////////////////OutPut////////////////////////////////
    //printf( "        FILE HEADER1                        FILE HEADER2\n\n"
    //"File Header            - %c%c%c%-*cFile Header            - %c%c%c%c\n"
    //"File Format Version    - %-*iFile Format Version    - %i\n"
    //"File Header Size       - %-*iFile Header Size       - %i\n"
    //"Video Data FourCC      - %-*iVideo Data FourCC      - %i\n"
    //"Video Image Width      - %-*iVideo Image Width      - %i\n"
    //"Video Image Height     - %-*iVideo Image Height     - %i\n"
    //"Frame Rate Rate        - %-*iFrame Rate Rate        - %i\n"
    //"Frame Rate Scale       - %-*iFrame Rate Scale       - %i\n"
    //"Video Length in Frames - %-*iVideo Length in Frames - %i\n"
    //"Unused                 - %-*cUnused                 - %c\n"
    //"\n\n"
    //,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],9,ivfhRaw.signature[3]
    //,ivfhRaw2.signature[0],ivfhRaw2.signature[1],ivfhRaw2.signature[2],ivfhRaw2.signature[3]
    //,12,ivfhRaw.version,ivfhRaw2.version
    //,12,ivfhRaw.headersize,ivfhRaw2.headersize
    //,12,ivfhRaw.FourCC,ivfhRaw2.FourCC
    //,12,ivfhRaw.width,ivfhRaw2.width
    //,12,ivfhRaw.height,ivfhRaw2.height
    //,12,ivfhRaw.rate,ivfhRaw2.rate
    //,12,ivfhRaw.scale,ivfhRaw2.scale
    //,12,ivfhRaw.length,ivfhRaw2.length
    //,12,ivfhRaw.unused,ivfhRaw2.unused);
    ////////////////////////////////////////////////////////////////////////

    if (ivfhRaw.signature[0] != ivfhRaw2.signature[0])
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.signature[1] != ivfhRaw2.signature[1])
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.signature[2] != ivfhRaw2.signature[2])
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.signature[3] != ivfhRaw2.signature[3])
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.version != ivfhRaw2.version)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.headersize != ivfhRaw2.headersize)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.FourCC != ivfhRaw2.FourCC)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.width != ivfhRaw2.width)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.height != ivfhRaw2.height)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.rate != ivfhRaw2.rate)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.scale != ivfhRaw2.scale)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivfhRaw.length != ivfhRaw2.length)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    IVF_FRAME_HEADER ivf_fhRaw1;

    fread(&ivf_fhRaw1.frameSize, 1, 4, in);
    fread(&ivf_fhRaw1.timeStamp, 1, 8, in);
    FormatFrameHeaderRead(ivf_fhRaw1);

    fread(inputVideoBuffer, 1, ivf_fhRaw1.frameSize, in);

    IVF_FRAME_HEADER ivf_fhRaw2;

    fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
    fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
    FormatFrameHeaderRead(ivf_fhRaw2);

    fread(inputVideo2Buffer, 1, ivf_fhRaw2.frameSize, in2);

    //////////////////////////////////OutPut////////////////////////////////
    //printf("FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
    //"Frame Size            - %-*iFrame Size            - %i\n"
    //"Time Stamp            - %-*iTime Stamp            - %i\n"
    //"\n"

    //,22,currentVideoFrame,currentVideoFrame
    //,12,ivf_fhRaw1.frameSize,ivf_fhRaw2.frameSize
    //,12,ivf_fhRaw1.timeStamp,ivf_fhRaw2.timeStamp);
    ////////////////////////////////////////////////////////////////////////

    int frameCount = ivfhRaw.length;
    currentVideoFrame++;

    if (ivf_fhRaw2.frameSize != ivf_fhRaw1.frameSize)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    if (ivf_fhRaw2.timeStamp != ivf_fhRaw1.timeStamp)
    {
        returnval = -2;
    }

    if (memcmp(inputVideoBuffer, inputVideo2Buffer, ivf_fhRaw1.frameSize) != 0)
    {
        fclose(in);
        fclose(in2);
        return currentVideoFrame;
    }

    while (!feof(in) || !feof(in2))
    {
        // if one file ends and the other doesnt exit
        if (feof(in) && !feof(in2))
        {
            fclose(in);
            fclose(in2);
            return -2;
        }

        if (!feof(in) && feof(in2))
        {
            fclose(in);
            fclose(in2);
            return -3;
        }

        if (currentVideoFrame == ivfhRaw2.length)
        {
            break;
        }

        memset(inputVideoBuffer, 0, ivfhRaw.width * ivfhRaw.height * 3);
        memset(inputVideo2Buffer, 0, ivfhRaw2.width * ivfhRaw2.height * 3);

        fread(&ivf_fhRaw1.frameSize, 1, 4, in);
        fread(&ivf_fhRaw1.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw1);
        fread(inputVideoBuffer, 1, ivf_fhRaw1.frameSize, in);

        fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
        fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
        FormatFrameHeaderRead(ivf_fhRaw2);
        fread(inputVideo2Buffer, 1, ivf_fhRaw2.frameSize, in2);

        //////////////////////////////////OutPut////////////////////////////////
        //printf("FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
        //"Frame Size            - %-*iFrame Size            - %i\n"
        //"Time Stamp            - %-*iTime Stamp            - %i\n"
        //"\n"

        //,22,currentVideoFrame,currentVideoFrame
        //,12,ivf_fhRaw1.frameSize,ivf_fhRaw2.frameSize
        //,12,ivf_fhRaw1.timeStamp,ivf_fhRaw2.timeStamp);
        ////////////////////////////////////////////////////////////////////////

        if (ivf_fhRaw2.frameSize != ivf_fhRaw1.frameSize)
        {
            fclose(in);
            fclose(in2);
            cout << "Frame Size not equal";
            return currentVideoFrame + 1;
        }

        if (ivf_fhRaw2.timeStamp != ivf_fhRaw1.timeStamp)
        {
            returnval = -2;
        }

        if (memcmp(inputVideoBuffer, inputVideo2Buffer, ivf_fhRaw1.frameSize) != 0)
        {

            fclose(in);
            fclose(in2);
            return currentVideoFrame + 1;
        }

        currentVideoFrame++;
    }

    fclose(in);
    fclose(in2);
    delete [] inputVideoBuffer;
    delete [] inputVideo2Buffer;

    return returnval;
}
double IVFDisplayDropedFrames(char *inputchar, int PrintSwitch)
{
    int dropedframecount = 0;

    string DropedInStr = inputchar;
    DropedInStr.erase(DropedInStr.length() - 4, 4);
    DropedInStr.append("_AproxDropedFrames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", DropedInStr.c_str());


    FILE *in = fopen(inputchar, "rb");
    FILE *out;

    ///////////////////////////////////
    //long PosSize = FileSize2(inputFile);

    if (in == NULL)
    {
        printf("\nInput file does not exist");
        fprintf(stderr, "\nInput file does not exist");
        fclose(in);
        return 0;
    }

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");
        //ofstream outfile(outputFile);
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);

    /*printf( "IVF DataRate\n\n"
    "FILE HEADER \n\n"
    "File Header            - %c%c%c%c \n"
    "File Format Version    - %i \n"
    "File Header Size       - %i \n"
    "Video Data FourCC      - %i \n"
    "Video Image Width      - %i \n"
    "Video Image Height     - %i \n"
    "Frame Rate Rate        - %i \n"
    "Frame Rate Scale       - %i \n"
    "Video Length in Frames - %i \n"
    "Unused                 - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    fread(&ivf_fhRaw.frameSize, 1, 4, in);
    fread(&ivf_fhRaw.timeStamp, 1, 8, in);
    FormatFrameHeaderRead(ivf_fhRaw);

    frameCount = ivfhRaw.length;

    long nSamples = frameCount;
    long lRateNum = ivfhRaw.rate;
    long lRateDenom = ivfhRaw.scale;

    long nSamplesPerBlock = 1;

    long nBytes = 0;
    long nBytesMin = 999999;
    long nBytesMax = 0;

    fpos_t position;
    fgetpos(in, &position);

    int priorTimeStamp = 0;

    while (currentVideoFrame < frameCount)
    {
        fseek(in , ivf_fhRaw.frameSize , SEEK_CUR);
        //fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        while (ivf_fhRaw.timeStamp != priorTimeStamp + 2 && !feof(in))
        {
            if (PrintSwitch == 1)
            {
                fprintf(out, "%i\n", currentVideoFrame);
            }

            priorTimeStamp = priorTimeStamp + 2;
            dropedframecount++;
        }

        priorTimeStamp = ivf_fhRaw.timeStamp;

        currentVideoFrame ++;
    }

    if (PrintSwitch == 1)
    {
        fclose(out);
    }

    fclose(in);

    return dropedframecount;
}
double IVFDisplayResizedFrames(char *inputchar, int PrintSwitch)
{
    //PrintSwitch == 0 -> Print to screen
    //PrintSwitch == 1 -> Print to file

    string ResizeInStr = inputchar;
    ResizeInStr.erase(ResizeInStr.length() - 4, 4);
    ResizeInStr.append("_ResizedFrames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", ResizeInStr.c_str());

    FILE *out;

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");
    }

    on2_codec_ctx_t       decoder;
    char                  *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    on2_codec_dec_cfg_t     cfg = {0};
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};

    int CharCount = 0;

    FILE *in = fopen(fn, "rb");
    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);
    fclose(in);

    int width = ivfhRaw.width;
    int height = ivfhRaw.height;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        if (PrintSwitch == 1)
        {
            fclose(out);
        }

        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    is_ivf = file_is_ivf(infile, &fourcc);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (vp8_pp_cfg.post_proc_flag
        && vpx_codec_control(&decoder, VP8_SET_POSTPROC, &vp8_pp_cfg))
    {
        fprintf(stderr, "Failed to configure postproc: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    int resizedIMGCount = 0;

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        if (progress)
            printf("decoded frame %d.\n", frame);



        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                if (img->d_w != width || img->d_h != height) //CheckFrameSize
                {
                    if (PrintSwitch == 0)
                    {
                        printf("%i %i %i \n", frame, img->d_w, img->d_h);
                    }

                    if (PrintSwitch == 1)
                    {
                        fprintf(out, "%i %i %i\n", frame, img->d_w, img->d_h);
                    }

                    resizedIMGCount++;
                }
            }
        }

        ++frame;

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("Decoded %d frames in %lu us (%.2f fps)\n",
               frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (PrintSwitch == 1)
    {
        fclose(out);
    }

    free(buf);
    fclose(infile);

    return resizedIMGCount;
}
double IVFDisplayVisibleFrames(char *inputFile, int Selector)
{
    // 0 = just display
    // 1 = write to file
    string VisInStr = inputFile;
    VisInStr.erase(VisInStr.length() - 4, 4);
    VisInStr.append("_VisibleFrames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", VisInStr.c_str());

    int VisableCount = 0;

    if (Selector == 0)
    {
        FILE *in = fopen(inputFile, "rb");
        ///////////////////////////////////
        long PosSize = FileSize2(inputFile);

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        /*printf( "IVF DataRate\n\n"
        "FILE HEADER \n\n"
        "File Header            - %c%c%c%c \n"
        "File Format Version    - %i \n"
        "File Header Size       - %i \n"
        "Video Data FourCC      - %i \n"
        "Video Image Width      - %i \n"
        "Video Image Height     - %i \n"
        "Frame Rate Rate        - %i \n"
        "Frame Rate Scale       - %i \n"
        "Video Length in Frames - %i \n"
        "Unused                 - %c \n"
        "\n\n"
        ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        frameCount = ivfhRaw.length;

        long nSamples = frameCount;
        long lRateNum = ivfhRaw.rate;
        long lRateDenom = ivfhRaw.scale;

        long nSamplesPerBlock = 1;

        long nBytes = 0;
        long nBytesMin = 999999;
        long nBytesMax = 0;

        fpos_t position;
        fgetpos(in, &position);
        cout << "\n";

        char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

        while (currentVideoFrame < frameCount)
        {
            //check to see if key frame
            VP8_HEADER oz;
            //VP8_COMMON pc;

            fread(inbuff, 1, ivf_fhRaw.frameSize, in);

#if defined(__POWERPC__)
            {
                int v = ((int *)inbuff)[0];
                v = swap4(v);
                oz.type = v & 1;
                oz.version = (v >> 1) & 7;
                oz.showFrame = (v >> 4) & 1;
                oz.firstPartitionLengthInBytes = (v >> 5) & 0x7ffff;
            }
#else
            memcpy(&oz, inbuff, 3); // copy 3 bytes;
#endif

            unsigned int type = oz.type;
            unsigned int showFrame = oz.showFrame;
            unsigned int version = oz.version;

            if (showFrame == 1)
            {
                VisableCount++;
                cout << "\n" << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            FormatFrameHeaderRead(ivf_fhRaw);

            currentVideoFrame ++;
        }

        fclose(in);
        delete[] inbuff;
    }

    if (Selector == 1)
    {
        FILE *in = fopen(inputFile, "rb");
        ofstream outfile(outputFile);

        ///////////////////////////////////
        long PosSize = FileSize2(inputFile);

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        /*printf( "IVF DataRate\n\n"
        "FILE HEADER \n\n"
        "File Header            - %c%c%c%c \n"
        "File Format Version    - %i \n"
        "File Header Size       - %i \n"
        "Video Data FourCC      - %i \n"
        "Video Image Width      - %i \n"
        "Video Image Height     - %i \n"
        "Frame Rate Rate        - %i \n"
        "Frame Rate Scale       - %i \n"
        "Video Length in Frames - %i \n"
        "Unused                 - %c \n"
        "\n\n"
        ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        frameCount = ivfhRaw.length;

        long nSamples = frameCount;
        long lRateNum = ivfhRaw.rate;
        long lRateDenom = ivfhRaw.scale;

        long nSamplesPerBlock = 1;

        long nBytes = 0;
        long nBytesMin = 999999;
        long nBytesMax = 0;

        fpos_t position;
        fgetpos(in, &position);
        //cout << "\n";

        char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

        while (currentVideoFrame < frameCount)
        {
            //check to see if key frame
            VP8_HEADER oz;

            fread(inbuff, 1, ivf_fhRaw.frameSize, in);

#if defined(__POWERPC__)
            {
                int v = ((int *)inbuff)[0];
                v = swap4(v);
                oz.type = v & 1;
                oz.version = (v >> 1) & 7;
                oz.showFrame = (v >> 4) & 1;
                oz.firstPartitionLengthInBytes = (v >> 5) & 0x7ffff;
            }
#else
            memcpy(&oz, inbuff, 3); // copy 3 bytes;
#endif
            unsigned int type = oz.type;
            unsigned int showFrame = oz.showFrame;
            unsigned int version = oz.version;

            if (showFrame == 1)
            {
                VisableCount++;
                outfile << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            FormatFrameHeaderRead(ivf_fhRaw);

            currentVideoFrame ++;
        }

        outfile.close();
        fclose(in);
        delete[] inbuff;

    }

    return VisableCount;

}
double IVFDisplayAltRefFrames(char *inputFile, int Selector)
{
    // 0 = just display
    // 1 = write to file

    string AltRefInStr = inputFile;
    AltRefInStr.erase(AltRefInStr.length() - 4, 4);
    AltRefInStr.append("_AltRefFrames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", AltRefInStr.c_str());

    int AltRefCount = 0;

    if (Selector == 0)
    {
        FILE *in = fopen(inputFile, "rb");
        ///////////////////////////////////
        long PosSize = FileSize2(inputFile);

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        /*printf( "IVF DataRate\n\n"
        "FILE HEADER \n\n"
        "File Header            - %c%c%c%c \n"
        "File Format Version    - %i \n"
        "File Header Size       - %i \n"
        "Video Data FourCC      - %i \n"
        "Video Image Width      - %i \n"
        "Video Image Height     - %i \n"
        "Frame Rate Rate        - %i \n"
        "Frame Rate Scale       - %i \n"
        "Video Length in Frames - %i \n"
        "Unused                 - %c \n"
        "\n\n"
        ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        frameCount = ivfhRaw.length;

        long nSamples = frameCount;
        long lRateNum = ivfhRaw.rate;
        long lRateDenom = ivfhRaw.scale;

        long nSamplesPerBlock = 1;

        long nBytes = 0;
        long nBytesMin = 999999;
        long nBytesMax = 0;

        fpos_t position;
        fgetpos(in, &position);
        cout << "\n";

        char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

        while (currentVideoFrame < frameCount)
        {
            //check to see if key frame
            VP8_HEADER oz;
            //VP8_COMMON pc;

            fread(inbuff, 1, ivf_fhRaw.frameSize, in);

#if defined(__POWERPC__)
            {
                int v = ((int *)inbuff)[0];
                v = swap4(v);
                oz.type = v & 1;
                oz.version = (v >> 1) & 7;
                oz.showFrame = (v >> 4) & 1;
                oz.firstPartitionLengthInBytes = (v >> 5) & 0x7ffff;
            }
#else
            memcpy(&oz, inbuff, 3); // copy 3 bytes;
#endif

            unsigned int type = oz.type;
            unsigned int showFrame = oz.showFrame;
            unsigned int version = oz.version;

            if (showFrame == 0)
            {
                AltRefCount++;
                cout << "\n" << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            FormatFrameHeaderRead(ivf_fhRaw);

            currentVideoFrame ++;
        }

        fclose(in);
        delete[] inbuff;
    }

    if (Selector == 1)
    {
        FILE *in = fopen(inputFile, "rb");
        ofstream outfile(outputFile);

        ///////////////////////////////////
        long PosSize = FileSize2(inputFile);

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        /*printf( "IVF DataRate\n\n"
        "FILE HEADER \n\n"
        "File Header            - %c%c%c%c \n"
        "File Format Version    - %i \n"
        "File Header Size       - %i \n"
        "Video Data FourCC      - %i \n"
        "Video Image Width      - %i \n"
        "Video Image Height     - %i \n"
        "Frame Rate Rate        - %i \n"
        "Frame Rate Scale       - %i \n"
        "Video Length in Frames - %i \n"
        "Unused                 - %c \n"
        "\n\n"
        ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        frameCount = ivfhRaw.length;

        long nSamples = frameCount;
        long lRateNum = ivfhRaw.rate;
        long lRateDenom = ivfhRaw.scale;

        long nSamplesPerBlock = 1;

        long nBytes = 0;
        long nBytesMin = 999999;
        long nBytesMax = 0;

        fpos_t position;
        fgetpos(in, &position);
        //cout << "\n";

        char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

        while (currentVideoFrame < frameCount)
        {
            //check to see if key frame
            VP8_HEADER oz;

            fread(inbuff, 1, ivf_fhRaw.frameSize, in);

#if defined(__POWERPC__)
            {
                int v = ((int *)inbuff)[0];
                v = swap4(v);
                oz.type = v & 1;
                oz.version = (v >> 1) & 7;
                oz.showFrame = (v >> 4) & 1;
                oz.firstPartitionLengthInBytes = (v >> 5) & 0x7ffff;
            }
#else
            memcpy(&oz, inbuff, 3); // copy 3 bytes;
#endif
            unsigned int type = oz.type;
            unsigned int showFrame = oz.showFrame;
            unsigned int version = oz.version;

            if (showFrame == 0)
            {
                AltRefCount++;
                outfile << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            FormatFrameHeaderRead(ivf_fhRaw);

            currentVideoFrame ++;
        }

        outfile.close();
        fclose(in);
        delete[] inbuff;

    }

    return AltRefCount;

}
double IVFDisplayKeyFrames(char *inputFile, int Selector)
{
    int keyframecount = 0;

    string KeyInStr = inputFile;
    KeyInStr.erase(KeyInStr.length() - 4, 4);
    KeyInStr.append("_KeyFrames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", KeyInStr.c_str());

    if (Selector == 0)
    {
        FILE *in = fopen(inputFile, "rb");
        ///////////////////////////////////
        long PosSize = FileSize2(inputFile);

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        /*printf( "IVF DataRate\n\n"
        "FILE HEADER \n\n"
        "File Header            - %c%c%c%c \n"
        "File Format Version    - %i \n"
        "File Header Size       - %i \n"
        "Video Data FourCC      - %i \n"
        "Video Image Width      - %i \n"
        "Video Image Height     - %i \n"
        "Frame Rate Rate        - %i \n"
        "Frame Rate Scale       - %i \n"
        "Video Length in Frames - %i \n"
        "Unused                 - %c \n"
        "\n\n"
        ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        frameCount = ivfhRaw.length;

        long nSamples = frameCount;
        long lRateNum = ivfhRaw.rate;
        long lRateDenom = ivfhRaw.scale;

        long nSamplesPerBlock = 1;

        long nBytes = 0;
        long nBytesMin = 999999;
        long nBytesMax = 0;

        fpos_t position;
        fgetpos(in, &position);
        cout << "\n";

        char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

        while (currentVideoFrame < frameCount)
        {
            //check to see if key frame
            VP8_HEADER oz;

            fread(inbuff, 1, ivf_fhRaw.frameSize, in);

#if defined(__POWERPC__)
            {
                int v = ((int *)inbuff)[0];
                v = swap4(v);
                oz.type = v & 1;
                oz.version = (v >> 1) & 7;
                oz.showFrame = (v >> 4) & 1;
                oz.firstPartitionLengthInBytes = (v >> 5) & 0x7ffff;
            }
#else
            memcpy(&oz, inbuff, 3); // copy 3 bytes;
#endif

            unsigned int type = oz.type;
            unsigned int showFrame = oz.showFrame;
            unsigned int version = oz.version;

            if (type == 0)
            {
                keyframecount++;
                cout << "\n" << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            FormatFrameHeaderRead(ivf_fhRaw);

            currentVideoFrame ++;
        }

        fclose(in);
        delete[] inbuff;
    }

    if (Selector == 1)
    {
        FILE *in = fopen(inputFile, "rb");
        ofstream outfile(outputFile);

        ///////////////////////////////////
        long PosSize = FileSize2(inputFile);

        if (in == NULL)
        {
            printf("\nInput file does not exist");
            fprintf(stderr, "\nInput file does not exist");
            fclose(in);
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        FormatIVFHeaderRead(&ivfhRaw);

        /*printf( "IVF DataRate\n\n"
        "FILE HEADER \n\n"
        "File Header            - %c%c%c%c \n"
        "File Format Version    - %i \n"
        "File Header Size       - %i \n"
        "Video Data FourCC      - %i \n"
        "Video Image Width      - %i \n"
        "Video Image Height     - %i \n"
        "Frame Rate Rate        - %i \n"
        "Frame Rate Scale       - %i \n"
        "Video Length in Frames - %i \n"
        "Unused                 - %c \n"
        "\n\n"
        ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        FormatFrameHeaderRead(ivf_fhRaw);

        frameCount = ivfhRaw.length;

        long nSamples = frameCount;
        long lRateNum = ivfhRaw.rate;
        long lRateDenom = ivfhRaw.scale;

        long nSamplesPerBlock = 1;

        long nBytes = 0;
        long nBytesMin = 999999;
        long nBytesMax = 0;

        fpos_t position;
        fgetpos(in, &position);
        //cout << "\n";

        char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

        while (currentVideoFrame < frameCount)
        {
            //check to see if key frame
            VP8_HEADER oz;

            fread(inbuff, 1, ivf_fhRaw.frameSize, in);

#if defined(__POWERPC__)
            {
                int v = ((int *)inbuff)[0];
                v = swap4(v);
                oz.type = v & 1;
                oz.version = (v >> 1) & 7;
                oz.showFrame = (v >> 4) & 1;
                oz.firstPartitionLengthInBytes = (v >> 5) & 0x7ffff;
            }
#else
            memcpy(&oz, inbuff, 3); // copy 3 bytes;
#endif
            unsigned int type = oz.type;
            unsigned int showFrame = oz.showFrame;
            unsigned int version = oz.version;

            if (type == 0)
            {
                keyframecount++;
                outfile << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            FormatFrameHeaderRead(ivf_fhRaw);

            currentVideoFrame ++;
        }

        outfile.close();
        fclose(in);
        delete[] inbuff;

    }

    return keyframecount;
}
int IVFLagInFramesCheck(char *QuantInChar)
{
    ifstream Quantinfile(QuantInChar);

    if (!Quantinfile.good())
    {
        printf("\nError: Cannot open file: %s\n", QuantInChar);
        fprintf(stderr, "\nError: Cannot open file: %s\n", QuantInChar);
        return -1;
    }

    int LagCounter = 0;
    int frame = 0;
    int quantizer = 0;
    int LagInFramesFound = 0;

    while (!Quantinfile.eof())
    {
        Quantinfile >> frame;
        Quantinfile >> quantizer;

        if (frame != 0)
        {
            //CorrectLagFail = 1;
            if (LagInFramesFound == 1)
                LagInFramesFound = 0;

            break;
        }
        else
        {
            LagInFramesFound++;
        }
    }

    Quantinfile.close();
    return LagInFramesFound;
    ////////////////////////////////////////////////////////
}
int IVFDFWMCheck(char *InputFile, int printselect)
{
    //return 0 = pass
    //return 1 = fail
    //return -1 = error
    //return -3 = threshold never reached

    string CheckPBMThreshStr = InputFile;
    CheckPBMThreshStr.erase(CheckPBMThreshStr.length() - 4, 4);
    CheckPBMThreshStr.append("_CheckPBMThresh.txt");
    char CheckPBMThreshChar[255] = "";
    snprintf(CheckPBMThreshChar, 255, "%s", CheckPBMThreshStr.c_str());

    string ResizeFramesStr = InputFile;
    ResizeFramesStr.erase(ResizeFramesStr.length() - 4, 4);
    ResizeFramesStr.append("_ResizedFrames.txt");
    char ResizeFramesChar[255] = "";
    snprintf(ResizeFramesChar, 255, "%s", ResizeFramesStr.c_str());

    string KeyFramesStr = InputFile;
    KeyFramesStr.erase(KeyFramesStr.length() - 4, 4);
    KeyFramesStr.append("_KeyFrames.txt");
    char KeyFramesChar[255] = "";
    snprintf(KeyFramesChar, 255, "%s", KeyFramesStr.c_str());


    ifstream CheckPBMFile(CheckPBMThreshChar);

    if (!CheckPBMFile.good())
    {
        printf("\nError: Cannot open file: %s\n", CheckPBMThreshChar);
        fprintf(stderr, "\nError: Cannot open file: %s\n", CheckPBMThreshChar);
        CheckPBMFile.close();
        return -1;
    }

    ifstream ResizeFramesFile(ResizeFramesChar);

    if (!ResizeFramesFile.good())
    {
        printf("\nError: Cannot open file: %s\n", ResizeFramesChar);
        fprintf(stderr, "\nError: Cannot open file: %s\n", ResizeFramesChar);
        ResizeFramesFile.close();
        CheckPBMFile.close();
        return -1;
    }

    ifstream KeyFramesFile(KeyFramesChar);

    if (!KeyFramesFile.good())
    {
        printf("\nError: Cannot open file: %s\n", KeyFramesChar);
        fprintf(stderr, "\nError: Cannot open file: %s\n", KeyFramesChar);
        KeyFramesFile.close();
        ResizeFramesFile.close();
        CheckPBMFile.close();
        return -1;
    }

    int fail = 0;

    int firstResizedFrame = 0;
    ResizeFramesFile >> firstResizedFrame;

    int curkeyframe = 0;
    int garbage = 0;
    int CheckPBMStatus = -1;

    int ThresholdTrigger = 0;//if threshold never hit this is 0 if it is hit somewhere it is 1


    KeyFramesFile >> curkeyframe;//get past trivial 0 case

    while (curkeyframe < firstResizedFrame)
    {
        KeyFramesFile >> curkeyframe;

        int curThreshFrame = 0;
        CheckPBMFile.seekg(0, ios::beg);

        while (!CheckPBMFile.eof() && curThreshFrame < curkeyframe) //get threshold status for frame just prior to keyframe
        {
            CheckPBMFile >> garbage;
            CheckPBMFile >> CheckPBMStatus;

            if (CheckPBMStatus == 1)
            {
                ThresholdTrigger = 1;
            }

            curThreshFrame++;
        }

        if (curkeyframe == firstResizedFrame) //if keyframe is also first resized frame then the threshold status for the frame prior to it shoudl be 1 if not it fails
        {
            if (CheckPBMStatus == 1)
            {
                if (printselect == 1)
                {
                    printf("For Key Frame %4i; frame %4i under buffer level for first resized frame -Pass\n", curkeyframe, curkeyframe - 1);
                }
            }
            else
            {
                if (printselect == 1)
                {
                    printf("For Key Frame %4i; frame %4i not under buffer level for first resized frame -Fail\n", curkeyframe, curkeyframe - 1);
                }

                fail = 1;
            }
        }
        else//if key frame isnt first resized frame then the threshold status for the frame prior to it should be 0 if not it fails
        {
            if (CheckPBMStatus == 0)
            {
                if (printselect == 1)
                {
                    printf("For Key Frame %4i; frame %4i not under buffer level -Pass\n", curkeyframe, curkeyframe - 1);
                }
            }
            else
            {
                if (printselect == 1)
                {
                    printf("For Key Frame %4i; frame %4i under buffer level -Fail\n", curkeyframe, curkeyframe - 1);
                }

                fail = 1;
            }
        }
    }

    KeyFramesFile.close();
    ResizeFramesFile.close();
    CheckPBMFile.close();

    if (ThresholdTrigger == 1)
    {
        return fail;
    }
    else
    {
        return -3;
    }

    ////////////////////////////////////////////////////////
}
int CheckMinQ(char *inputFile, int MinQuantizer)
{
    char QuantDispNameChar[255] = "";
    FileName(inputFile, QuantDispNameChar);

    printf("Checking %s min quantizer:\n", QuantDispNameChar);
    fprintf(stderr, "Checking %s min quantizer:\n", QuantDispNameChar);

    string QuantInStr = inputFile;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_Quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    ifstream infile(QuantInChar);

    if (!infile.good())
    {
        printf("\nError: Cannot open file: %s\n", QuantInChar);
        fprintf(stderr, "\nError: Cannot open file: %s\n", QuantInChar);
        return 0;
    }

    int frame = 0;
    int quantizer = 0;
    int result = 1;

    while (!infile.eof())
    {
        infile >> frame;
        infile >> quantizer;

        if (frame == 0)
        {
            if (quantizer != 0 && quantizer < MinQuantizer)
            {
                printf("     Quantizer not >= min for frame %i q=%i - Failed", frame, quantizer);
                fprintf(stderr, "     Quantizer not >= min for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }
        else
        {
            if (quantizer < MinQuantizer)
            {
                printf("     Quantizer not >= min for frame %i q=%i - Failed", frame, quantizer);
                fprintf(stderr, "     Quantizer not >= min for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }

        //cout << "FRAME: " << frame << " Quantizer: " << quantizer << "\n";

    }

    printf("     All quantizers >= min for all frames - Passed");
    fprintf(stderr, "     All quantizers >= min for all frames - Passed");

    return -1;//result > -1 -> fail | result = -1 pass
}
int CheckMaxQ(char *inputFile, int MaxQuantizer)
{
    char QuantDispNameChar[255] = "";
    FileName(inputFile, QuantDispNameChar);

    printf("Checking %s max quantizer:\n", QuantDispNameChar);
    fprintf(stderr, "Checking %s max quantizer:\n", QuantDispNameChar);

    string QuantInStr = inputFile;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_Quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    ifstream infile(QuantInChar);

    if (!infile.good())
    {
        printf("\nError: Cannot open file: %s\n", QuantInChar);
        fprintf(stderr, "\nError: Cannot open file: %s\n", QuantInChar);
        return 0;
    }

    int frame = 0;
    int quantizer = 0;
    int result = 1;

    while (!infile.eof())
    {
        infile >> frame;
        infile >> quantizer;

        if (frame == 0)
        {
            if (quantizer != 0 && quantizer > MaxQuantizer)
            {
                printf("     Quantizer not <= max for frame %i q=%i - Failed", frame, quantizer);
                fprintf(stderr, "     Quantizer not >= min for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }
        else
        {
            if (quantizer > MaxQuantizer)
            {
                printf("     Quantizer not <= max for frame %i q=%i - Failed", frame, quantizer);
                fprintf(stderr, "     Quantizer not <= max for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }

        //cout << "FRAME: " << frame << " Quantizer: " << quantizer << "\n";
    }

    printf(" All quantizers <= max for all frames");
    fprintf(stderr, " All quantizers <= max for all frames");

    return -1;//result > -1 -> fail | result = -1 pass
}
int CheckFixedQ(char *inputFile, int FixedQuantizer)
{
    char QuantDispNameChar[255] = "";
    FileName(inputFile, QuantDispNameChar);

    printf("Checking %s fixed quantizer:", QuantDispNameChar);
    fprintf(stderr, "Checking %s fixed quantizer:", QuantDispNameChar);

    string QuantInStr = inputFile;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_Quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    ifstream infile(QuantInChar);

    if (!infile.good())
    {
        printf("\nError: Cannot open file: %s\n", QuantInChar);
        fprintf(stderr, "\nError: Cannot open file: %s\n", QuantInChar);
        return 0;
    }

    int frame = 0;
    int quantizer = 0;
    int result = 1;

    while (!infile.eof())
    {
        infile >> frame;
        infile >> quantizer;

        if (frame == 0)
        {
            if (quantizer != 0 && quantizer != FixedQuantizer)
            {
                printf(" not fixed for frame %i q=%i - Failed", frame, quantizer);
                fprintf(stderr, " not fixed for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }
        else
        {
            if (quantizer != FixedQuantizer)
            {
                printf(" not fixed for frame %i q=%i", frame, quantizer);
                fprintf(stderr, " not fixed for frame %i q=%i", frame, quantizer);
                return frame;
            }
        }

        //cout << "FRAME: " << frame << " Quantizer: " << quantizer << "\n";

    }

    printf(" fixed for all frames");
    fprintf(stderr, " fixed for all frames");

    return -1;//result > -1 -> fail | result = -1 pass
}
int TimeReturn(char *infile)
{
    int speed;

    char TextFilechar1[255];

    FolderName2(infile, TextFilechar1);


    char *FullName = strcat(TextFilechar1, "CompressionTime.txt");

    ifstream infile2(FullName);

    if (!infile2.is_open())
    {
        printf("File: %s not opened", FullName);
        fprintf(stderr, "File: %s not opened", FullName);
        return 0;
    }

    infile2 >> speed;
    infile2.close();

    return speed;
}
int GetNumberofFrames(char *inputFile)
{
    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        printf("\nInput file does not exist");
        fprintf(stderr, "\nInput file does not exist");
        fclose(in);
        return 0;
    }

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);

    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    FormatIVFHeaderRead(&ivfhRaw);
    int FrameLength = ivfhRaw.length;

    fclose(in);

    return FrameLength;
}
//-----------------------------------------------------------IVF Enc------------------------------------------------------
int API20Encoder(long width, long height, char *infilechar, char *outfilechar)
{
    on2_codec_ctx_t        encoder;
    char                  *in_fn = infilechar, *out_fn = outfilechar, *stats_fn = NULL;
    int                    i;
    FILE                  *infile, *outfile;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    on2_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;

    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_ctrls[ARG_CTRL_CNT_MAX][2], arg_ctrl_cnt = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    static const int        *ctrl_args_map = NULL;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    static unsigned long            cx_time = 0;




    /* First parse the codec and usage values, because we want to apply other
    * parameters on top of the default configuration provided by the codec.
    */

    /* Populate encoder configuration */
    res = on2_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        printf("Failed to get config: %s\n", on2_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    cfg.g_w = width;                                                          //
    cfg.g_h = height;

    /* Now parse the remainder of the parameters. */


    /* Handle codec specific options */
    if (codec->iface == &on2_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

#define SHOW(field) printf("    %-28s = %d\n", #field, cfg.field)

    if (1)
    {
        printf("Source file: %s Format: %s\n", in_fn, arg_use_i420 ? "I420" : "YV12");
        printf("Destination file: %s\n", out_fn);
        printf("Encoder parameters:\n");

        SHOW(g_usage);
        SHOW(g_threads);
        SHOW(g_profile);
        SHOW(g_w);
        SHOW(g_h);
        SHOW(g_timebase.num);
        SHOW(g_timebase.den);
        SHOW(g_error_resilient);
        SHOW(g_pass);
        SHOW(g_lag_in_frames);
        SHOW(rc_dropframe_thresh);
        SHOW(rc_resize_allowed);
        SHOW(rc_resize_up_thresh);
        SHOW(rc_resize_down_thresh);
        SHOW(rc_end_usage);
        SHOW(rc_target_bitrate);
        SHOW(rc_min_quantizer);
        SHOW(rc_max_quantizer);
        SHOW(rc_undershoot_pct);
        SHOW(rc_overshoot_pct);
        SHOW(rc_buf_sz);
        SHOW(rc_buf_initial_sz);
        SHOW(rc_buf_optimal_sz);
        SHOW(rc_2pass_vbr_bias_pct);
        SHOW(rc_2pass_vbr_minsection_pct);
        SHOW(rc_2pass_vbr_maxsection_pct);
        SHOW(kf_mode);
        SHOW(kf_min_dist);
        SHOW(kf_max_dist);
    }

    on2_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);


    memset(&stats, 0, sizeof(stats));

    for (pass = one_pass_only ? one_pass_only - 1 : 0; pass < arg_passes; pass++)
    {
        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            printf("Failed to open input file");
            return EXIT_FAILURE;
        }

        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            printf("Failed to open output file");
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }


        cfg.g_pass = arg_passes == 2
                     ? pass ? ON2_RC_LAST_PASS : ON2_RC_FIRST_PASS
                 : ON2_RC_ONE_PASS;
#if ON2_ENCODER_ABI_VERSION > (1 + ON2_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = ON2_KF_FIXED;

        on2_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");

        /* Note that we bypass the on2_codec_control wrapper macro because
        * we're being clever to store the control IDs in an array. Real
        * applications will want to make use of the enumerations directly
        */
        for (i = 0; i < arg_ctrl_cnt; i++)
        {
            if (on2_codec_control_(&encoder, arg_ctrls[i][0], arg_ctrls[i][1]))
                printf("Error: Tried to set control %d = %d\n",
                       arg_ctrls[i][0], arg_ctrls[i][1]);

            ctx_exit_on_error(&encoder, "Failed to control codec");
        }

        frame_avail = 1;
        got_data = 0;

        while (frame_avail || got_data)
        {
            on2_codec_iter_t iter = NULL;
            const on2_codec_cx_pkt_t *pkt;
            struct on2_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                frame_avail = read_frame(infile, &raw);

                if (frame_avail)
                    frames_in++;

                printf("\rPass %d/%d frame %4d/%-4d %7ldB \033[K", pass + 1,
                       arg_passes, frames_in, frames_out, nbytes);
            }
            else
                frame_avail = 0;

            on2_usec_timer_start(&timer);
            on2_codec_encode(&encoder, frame_avail ? &raw : NULL, frames_in - 1,
                             1, 0, arg_deadline);
            on2_usec_timer_mark(&timer);
            cx_time += on2_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            while ((pkt = on2_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case ON2_CODEC_CX_FRAME_PKT:
                    printf(" %6luF",
                           (unsigned long)pkt->data.frame.sz);
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case ON2_CODEC_STATS_PKT:
                    printf(" %6luS",
                           (unsigned long)pkt->data.twopass_stats.sz);
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    break;
                default:
                    break;
                }
            }

            fflush(stdout);
        }

        /* this bitrate calc is simplified and relies on the fact that this
        * application uses 1/timebase for framerate.
        */
        {
            //uint64_t temp= (uint64_t)frames_in * 1000000;
        }
        on2_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats);
        printf("\n");
    }

    on2_img_free(&raw);
    return 0;
}
int API20EncoderIVF2IVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt)
{
    on2_codec_ctx_t        encoder;
    char                  *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    int                    i;
    FILE                  *infile, *outfile;
    on2_codec_enc_cfg_t    cfg;
    on2_codec_err_t        res;
    int                    pass;
    stats_io_t             stats;
    on2_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;

    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_ctrls[ARG_CTRL_CNT_MAX][2], arg_ctrl_cnt = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    static const int        *ctrl_args_map = NULL;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    static unsigned long            cx_time = 0;

    /* Populate encoder configuration */
    res = on2_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        printf("Failed to get config: %s\n", on2_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    VP8CoreConfigToAPIcfg(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");
    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    FormatIVFHeaderRead(&ivfhRaw);

    int w		= ivfhRaw.width;
    int h		= ivfhRaw.height;
    int fr		= (ivfhRaw.rate / ivfhRaw.scale);

    cfg.g_w = w;                                                          //
    cfg.g_h = h;
    cfg.g_timebase.num = 1000;
    cfg.g_timebase.den = fr * 1000;
    fclose(GetWHinfile);
    //Deal with Mode Pass and BitRate Here
    cfg.rc_target_bitrate = BitRate;

    if (oxcf.Mode == 0) //Real Time Mode
    {
        cfg.g_profile = 0;
    }

    if (oxcf.Mode == 1) //One Pass Good
    {
        //			on2_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_GOOD_QUALITY_ENCODING);
        cfg.g_profile = 1;
    }

    if (oxcf.Mode == 2) //One Pass Best
    {
        //			on2_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_BEST_QUALITY_ENCODING);
        cfg.g_profile = 2;
    }

    if (oxcf.Mode == 3) //First Pass
    {
        //			on2_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_GOOD_QUALITY_ENCODING);
        cfg.g_profile = 3;
    }

    if (oxcf.Mode == 4) //Two Pass Good
    {
        //			on2_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_GOOD_QUALITY_ENCODING);
        cfg.g_profile = 0;
        arg_passes = 2;

    }

    if (oxcf.Mode == 5) //Two Pass Best
    {
        //			on2_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_BEST_QUALITY_ENCODING);
        cfg.g_profile = 2;
        arg_passes = 2;
    }

    ///////////////////////////////////////////////////////////////////

    /* Handle codec specific options */
    if (codec->iface == &on2_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

#define SHOW(field) printf("    %-28s = %d\n", #field, cfg.field)

    if (1)
    {
        printf("Source file: %s Format: %s\n", in_fn, arg_use_i420 ? "I420" : "YV12");
        printf("Destination file: %s\n", out_fn);
        printf("Encoder parameters:\n");

        SHOW(g_usage);
        SHOW(g_threads);
        SHOW(g_profile);
        SHOW(g_w);
        SHOW(g_h);
        SHOW(g_timebase.num);
        SHOW(g_timebase.den);
        SHOW(g_error_resilient);
        SHOW(g_pass);
        SHOW(g_lag_in_frames);
        SHOW(rc_dropframe_thresh);
        SHOW(rc_resize_allowed);
        SHOW(rc_resize_up_thresh);
        SHOW(rc_resize_down_thresh);
        SHOW(rc_end_usage);
        SHOW(rc_target_bitrate);
        SHOW(rc_min_quantizer);
        SHOW(rc_max_quantizer);
        SHOW(rc_undershoot_pct);
        SHOW(rc_overshoot_pct);
        SHOW(rc_buf_sz);
        SHOW(rc_buf_initial_sz);
        SHOW(rc_buf_optimal_sz);
        SHOW(rc_2pass_vbr_bias_pct);
        SHOW(rc_2pass_vbr_minsection_pct);
        SHOW(rc_2pass_vbr_maxsection_pct);
        SHOW(kf_mode);
        SHOW(kf_min_dist);
        SHOW(kf_max_dist);
    }

    on2_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    for (pass = 0; pass < arg_passes; pass++)
    {
        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            printf("Failed to open input file");
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            printf("Failed to open output file");
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                printf("Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? ON2_RC_LAST_PASS : ON2_RC_FIRST_PASS
                 : ON2_RC_ONE_PASS;
#if ON2_ENCODER_ABI_VERSION > (1 + ON2_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = ON2_KF_FIXED;

        on2_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");

        /* Note that we bypass the on2_codec_control wrapper macro because
        * we're being clever to store the control IDs in an array. Real
        * applications will want to make use of the enumerations directly
        */
        for (i = 0; i < arg_ctrl_cnt; i++)
        {
            if (on2_codec_control_(&encoder, arg_ctrls[i][0], arg_ctrls[i][1]))
                printf("Error: Tried to set control %d = %d\n",
                       arg_ctrls[i][0], arg_ctrls[i][1]);

            ctx_exit_on_error(&encoder, "Failed to control codec");
        }

        frame_avail = 1;
        got_data = 0;

        while (frame_avail || got_data)
        {
            on2_codec_iter_t iter = NULL;
            const on2_codec_cx_pkt_t *pkt;
            struct on2_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame(infile, &raw);

                if (frame_avail)
                    frames_in++;

                printf("\rPass %d/%d frame %4d/%-4d %7ldB \033[K", pass + 1,
                       arg_passes, frames_in, frames_out, nbytes);
            }
            else
                frame_avail = 0;

            on2_usec_timer_start(&timer);
            on2_codec_encode(&encoder, frame_avail ? &raw : NULL, frames_in - 1,
                             1, 0, arg_deadline);
            on2_usec_timer_mark(&timer);
            cx_time += on2_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            while ((pkt = on2_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case ON2_CODEC_CX_FRAME_PKT:
                    printf(" %6luF",
                           (unsigned long)pkt->data.frame.sz);
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case ON2_CODEC_STATS_PKT:
                    printf(" %6luS",
                           (unsigned long)pkt->data.twopass_stats.sz);
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    break;
                default:
                    break;
                }
            }

            fflush(stdout);
        }

        /* this bitrate calc is simplified and relies on the fact that this
        * application uses 1/timebase for framerate.
        */
        {
            uint64_t temp = (uint64_t)frames_in * 1000000;

        }
        on2_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats);
        printf("\n");
    }

    on2_img_free(&raw);
    return 0;
}
int API20Decoder(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                  *prefix = NULL, *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);



    is_ivf = file_is_ivf(infile, &fourcc);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    printf("%s\n", decoder.name);
    cout << "\nWidth: " << cfg.w << "\nHeight: " << cfg.h << "\n";

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            printf("decoded frame %d.\n", frame);

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    sprintf(out_fn, "%s-%dx%d-%04d.%s",
                            prefix, img->d_w, img->d_h, frame, sfx);
                    out = out_open(out_fn, do_md5);
                }

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("Decoded %d frames in %lu us (%.2f fps)\n",
               frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);
    free(prefix);

    return 0;
}
int API20DecoderIVF2IVF(char *inputchar, char *outputchar)
{
    on2_codec_ctx_t       decoder;
    char                  *prefix = NULL, *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    on2_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    char                   *fn2 = outputchar;
    void *out;
    on2_codec_dec_cfg_t     cfg = {0};

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        printf("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fn2)
        out = out_open(fn2, do_md5);

    unsigned short width = 0;
    unsigned short height = 0;
    is_ivf = file_is_ivf_IVF(infile, &fourcc, (FILE *) out, &width, &height);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                on2_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    printf("Notice -- IVF header indicates codec: %s\n",
                           ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (on2_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? ON2_CODEC_USE_POSTPROC : 0))
    {
        printf("Failed to initialize decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    printf("%s\n", decoder.name);

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        on2_codec_iter_t  iter = NULL;
        on2_image_t    *img;
        struct on2_usec_timer timer;

        on2_usec_timer_start(&timer);

        if (on2_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = on2_codec_error_detail(&decoder);
            printf("Failed to decode frame: %s\n", on2_codec_error(&decoder));

            if (detail)
                printf("  Additional information: %s\n", detail);

            goto fail;
        }

        on2_usec_timer_mark(&timer);
        dx_time += on2_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            printf("decoded frame %d.\n", frame);

        if (!noblit)
        {
            img = on2_codec_get_frame(&decoder, &iter);

            if (img)
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    sprintf(out_fn, "%s-%dx%d-%04d.%s",
                            prefix, img->d_w, img->d_h, frame, sfx);
                    out = out_open(out_fn, do_md5);
                }

                ivf_write_frameAPI((FILE *)out, TimeStamp, FrameSize);

                buf = img->planes[PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[PLANE_Y];
                }

                buf = img->planes[flipuv?PLANE_V:PLANE_U];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_U];
                }

                buf = img->planes[flipuv?PLANE_U:PLANE_V];

                for (y = 0; y < img->d_h / 2; y++)
                {
                    out_put(out, buf, img->d_w / 2, do_md5);
                    buf += img->stride[PLANE_V];
                }

                if (!fn2)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    if (summary)
    {
        printf("Decoded %d frames in %lu us (%.2f fps)\n",
               frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (on2_codec_destroy(&decoder))
    {
        printf("Failed to destroy decoder: %s\n", on2_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);
    free(prefix);

    return 0;
}