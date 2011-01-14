#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_test_definitions.h"
#include "vpxt_utilities.h"
#include "vpxt_driver.h"
#include "yv12config.h"
#include "header.h"
#include "onyx.h"
#include "ivf.h"
#include "vp8.h"
#include "args.h"
#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx_timer.h"
#include "vpx_encoder.h"
#include "vpx_decoder.h"
#include "mem_ops.h"
#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cstdlib>
#include <cctype>

#define ON2_CODEC_DISABLE_COMPAT 1

typedef unsigned char BYTE;
#define HEADER_SIZE 32
#define IVF_SCALE   1000

#if defined(_WIN32)
#include "on2vpplugin.h"
#define snprintf _snprintf
#else
#include "dirent.h"
typedef unsigned int  DWORD;
#endif
#ifdef _MSC_VER
#define USE_POSIX_MMAP 0
#else
#define USE_POSIX_MMAP 1
#endif
#if USE_POSIX_MMAP
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#if CONFIG_MD5
#include "md5_utils.h"
#endif

/////////////////////////////////////////////Endian Conversions////////////////////////////////////////////
#ifdef __POWERPC__
# define make_endian_16(a) \
    (((unsigned int)(a & 0xff)) << 8) | (((unsigned int)(a & 0xff00)) >> 8)
# define make_endian_32(a)                                                                  \
    (((unsigned int)(a & 0xff)) << 24)    | (((unsigned int)(a & 0xff00)) << 8) |   \
    (((unsigned int)(a & 0xff0000)) >> 8) | (((unsigned int)(a & 0xff000000)) >> 24)
# define make_endian_64(a)  \
    ((a & 0xff) << 56           |   \
     ((a >>  8) & 0xff) << 48   |   \
     ((a >> 16) & 0xff) << 40   |   \
     ((a >> 24) & 0xff) << 32   |   \
     ((a >> 32) & 0xff) << 24   |   \
     ((a >> 40) & 0xff) << 16   |   \
     ((a >> 48) & 0xff) <<  8   |   \
     ((a >> 56) & 0xff))
# define MAKEFOURCC(ch0, ch1, ch2, ch3)                                 \
    ((DWORD)(BYTE)(ch0) << 24 | ((DWORD)(BYTE)(ch1) << 16) |    \
     ((DWORD)(BYTE)(ch2) << 8) | ((DWORD)(BYTE)(ch3)))
# define swap4(d)\
    ((d&0x000000ff)<<24) |  \
    ((d&0x0000ff00)<<8)  |  \
    ((d&0x00ff0000)>>8)  |  \
    ((d&0xff000000)>>24)
#else
# define make_endian_16(a)  a
# define make_endian_32(a)  a
# define make_endian_64(a)  a
# define MAKEFOURCC(ch0, ch1, ch2, ch3)                                 \
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |           \
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
# define swap4(d) d
#endif

const int PSNR_MAX = 999.;
const int sizBuff = 512;

extern double VP8_CalcSSIM_Tester(YV12_BUFFER_CONFIG *source, YV12_BUFFER_CONFIG *dest, int lumamask, double *weight);
extern double VP8_CalcPSNR_Tester(YV12_BUFFER_CONFIG *source, YV12_BUFFER_CONFIG *dest, double *YPsnr, double *UPsnr, double *VPsnr, double *SqError);
extern double VP8_Mse2Psnr_Tester(double Samples, double Peak, double Mse);

extern "C"
{
    extern int vp8_yv12_alloc_frame_buffer(YV12_BUFFER_CONFIG *ybf, int width, int height, int border);
    extern void vp8_scale_machine_specific_config(void);
    extern int vp8_yv12_de_alloc_frame_buffer(YV12_BUFFER_CONFIG *ybf);
    extern vpx_codec_iface_t vpx_enc_vp8_algo;
    extern void vp8_yv12_scale_or_center(YV12_BUFFER_CONFIG *src_yuv_config, YV12_BUFFER_CONFIG *dst_yuv_config, int expanded_frame_width, int expanded_frame_height, int scaling_mode, int HScale, int HRatio, int VScale, int VRatio);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////API 2.0//////////////////////////////////////////////////////
struct vp8_extracfg
{
    struct vpx_codec_pkt_list *pkt_list;
    vp8e_encoding_mode      encoding_mode;               /** best, good, realtime            */
    int                         cpu_used;                    /** available cpu percentage in 1/16*/
    unsigned int                enable_auto_alt_ref;           /** if encoder decides to uses alternate reference frame */
    unsigned int                noise_sensitivity;
    unsigned int                Sharpness;
    unsigned int                static_thresh;
    unsigned int                token_partitions;
    unsigned int                arnr_max_frames;    /* alt_ref Noise Reduction Max Frame Count */
    unsigned int                arnr_strength;    /* alt_ref Noise Reduction Strength */
    unsigned int                arnr_type;        /* alt_ref filter type */
    vp8e_tuning                 tuning;
    unsigned int                cq_level;         /* constrained quality level */

};
struct vpx_codec_alg_priv
{
    vpx_codec_priv_t        base;
    vpx_codec_enc_cfg_t     cfg;
    struct vp8_extracfg     vp8_cfg;
    VP8_CONFIG              oxcf;
    VP8_PTR             cpi;
    unsigned char          *cx_data;
    unsigned int            cx_data_sz;
    vpx_image_t             preview_img;
    unsigned int            next_frame_flag;
    vp8_postproc_cfg_t      preview_ppcfg;
    vpx_codec_pkt_list_decl(64) pkt_list;              // changed to accomendate the maximum number of lagged frames allowed
    int                         deprecated_mode;
    unsigned int                fixed_kf_cntr;
};

extern "C" vpx_codec_iface_t vpx_codec_vp8_cx_algo;
static const char *exec_name;

static const struct codec_item
{
    char const              *name;
    const vpx_codec_iface_t *iface;
    unsigned int             fourcc;
} codecs[] = {{"vp8",  &vpx_codec_vp8_cx_algo, 0x30385056}};

static void usage_exit();

void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    tprintf(PRINT_STD, "\n");
    usage_exit();
}

int ctx_exit_on_error_tester(vpx_codec_ctx_t *ctx, const char *s)
{
    if (ctx->err)
    {
        const char *detail = vpx_codec_error_detail(ctx);

        tprintf(PRINT_STD, "%s: %s\n", s, vpx_codec_error(ctx));

        if (detail)
            tprintf(PRINT_STD, "    %s\n", detail);

        return -1;
    }

    return 0;
}
int ctx_exit_on_error(vpx_codec_ctx_t *ctx, const char *s)
{
    if (ctx->err)
    {
        const char *detail = vpx_codec_error_detail(ctx);

        tprintf(PRINT_STD, "%s: %s\n", s, vpx_codec_error(ctx));

        if (detail)
            tprintf(PRINT_STD, "    %s\n", detail);

        //exit(EXIT_FAILURE);
        return -1;
    }

    return 0;
}

/* This structure is used to abstract the different ways of handling
* first pass statistics.
*/
typedef struct
{
    vpx_fixed_buf_t buf;
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
        stats->buf.buf = malloc(stats->buf_alloc_sz); //valgrind keeps saying possible mem leak
    }

    stats->buf_ptr = (char *)stats->buf.buf;
    res = (stats->buf.buf != NULL);
    return res;
}


void stats_close(stats_io_t *stats, int last_pass)
{
    if (stats->file)
    {
        if (stats->pass == last_pass)
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
        if (stats->pass == last_pass)
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

vpx_fixed_buf_t stats_get(stats_io_t *stats)
{
    return stats->buf;
}

static int read_frame_enc(FILE *f, vpx_image_t *img, int to_read)
{

    int file_type = 0;

    int plane = 0;

    for (plane = 0; plane < 3; plane++)
    {
        unsigned char *ptr;
        int w = (plane ? (1 + img->d_w) / 2 : img->d_w);
        int h = (plane ? (1 + img->d_h) / 2 : img->d_h);
        int r;

        /* Determine the correct plane based on the image format. The for-loop
        * always counts in Y,U,V order, but this may not match the order of
        * the data on disk.
        */
        switch (plane)
        {
        case 1:
            ptr = img->planes[img->fmt==IMG_FMT_YV12? PLANE_V : PLANE_U];
            break;
        case 2:
            ptr = img->planes[img->fmt==IMG_FMT_YV12?PLANE_U : PLANE_V];
            break;
        default:
            ptr = img->planes[plane];
        }

        for (r = 0; r < h; r++)
        {
            fread(ptr, 1, w, f);
            ptr += img->stride[plane];
        }
    }

    return !feof(f);
}

static void write_ivf_file_header(FILE *outfile,
const vpx_codec_enc_cfg_t *cfg,
unsigned int fourcc,
int frame_cnt)
{
    char header[32];

    if (cfg->g_pass != VPX_RC_ONE_PASS && cfg->g_pass != VPX_RC_LAST_PASS)
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
const vpx_codec_cx_pkt_t *pkt)
{
    char             header[12];
    vpx_codec_pts_t  pts;

    if (pkt->kind != VPX_CODEC_CX_FRAME_PKT)
        return;

    pts = pkt->data.frame.pts;
    mem_put_le32(header, pkt->data.frame.sz);
    mem_put_le32(header + 4, pts & 0xFFFFFFFF);
    mem_put_le32(header + 8, pts >> 32);

    fwrite(header, 1, 12, outfile);
}



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

    tprintf(PRINT_STD, "Usage: %s <options> src_filename dst_filename\n", exec_name);

    tprintf(PRINT_STD, "\nOptions:\n");
    tprintf(PRINT_STD, "\nEncoder Global Options:\n");
    tprintf(PRINT_STD, "\nRate Control Options:\n");
    tprintf(PRINT_STD, "\nTwopass Rate Control Options:\n");
    tprintf(PRINT_STD, "\nKeyframe Placement Options:\n");
    tprintf(PRINT_STD, "\nVP8 Specific Options:\n");

    tprintf(PRINT_STD, "\n"
            "Included encoders:\n"
            "\n");

    for (i = 0; i < sizeof(codecs) / sizeof(codecs[0]); i++)
        tprintf(PRINT_STD, "    %-6s - %s\n",
                codecs[i].name,
                vpx_codec_iface_name(codecs[i].iface));

    exit(EXIT_FAILURE);
}

#define ARG_CTRL_CNT_MAX 10

///////////////////////////////DEC///////////////////////////////
/* This is a simple program that reads "VP6 raw" files and decodes them
* using the new interface. Decoded frames are output as YV12 raw
* YV12 images.
*/


void ivf_write_headerDec(FILE *outfile, unsigned int width, unsigned short height, unsigned int rate, unsigned int scale, unsigned int length)
{
    IVF_HEADER ivf;
    strncpy((char *)(ivf.signature), "DKIF", 4);
    ivf.version = 0;
    ivf.headersize = make_endian_16(32);
    ivf.four_cc     = MAKEFOURCC('I', '4', '2', '0');
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
    const vpx_codec_iface_t *iface;
    unsigned int             fourcc;
    unsigned int             fourcc_mask;
} ifaces[] =
{
    {"vp8",  &vpx_codec_vp8_dx_algo,   0x00385056, 0x00FFFFFF},
};


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
            tprintf(PRINT_STD, "Failed to read frame size\n");

        new_buf_sz = 0;
    }
    else
    {
        new_buf_sz = mem_get_le32(raw_hdr);

        if (new_buf_sz > 256 * 1024)
        {
            tprintf(PRINT_STD, "Read invalid frame size - not a raw file?\n");
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
                tprintf(PRINT_STD, "Failed to allocate compressed data buffer\n");
                new_buf_sz = 0;
            }
        }
    }

    *buf_sz = new_buf_sz;

    if (*buf_sz)
    {
        if (fread(*buf, 1, *buf_sz, infile) != *buf_sz)
        {
            tprintf(PRINT_STD, "Failed to read full frame\n");
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
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
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

        tprintf(PRINT_STD, "\n");

        for (i = 0; i < 16; i++)
        {
            tprintf(PRINT_BTH, "%02x", md5[i]);
        }

        tprintf(PRINT_BTH, "\n");

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
                tprintf(PRINT_STD, "Error: Unrecognized IVF version! This file may not"
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
            ivfhRaw.four_cc = 808596553;   //I420 FourCC
            *width = ivfhRaw.width;
            *height = ivfhRaw.height;
            ivf_write_headerDec(out, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate, ivfhRaw.scale, ivfhRaw.length);

            ///////////////////////////////////////////////////////
            if (mem_get_le16(raw_hdr + 4) != 0)
                tprintf(PRINT_STD, "Error: Unrecognized IVF version! This file may not"
                        " decode properly.");

            *fourcc = mem_get_le32(raw_hdr + 8);
        }
    }

    if (!is_ivf)
        rewind(infile);

    return is_ivf;
}

void tprintf(int PrintSelection, const char *fmt, ...)
{
    //Output for python
    //FILE * STDOUT_File;
    //STDOUT_File = fopen ("PathToPythonTxtFile","a");

    char buffer[2048];
    buffer[0] = NULL;
    va_list ap;
    va_start(ap, fmt);

    int charswritten = vsnprintf(buffer, sizeof(buffer) - 1, fmt, ap);

    if (charswritten < 2048)
    {
        std::string bufferStr = buffer;
        std::string bufferStr2 = buffer;
        int curPos = 0;
        int lastNewLine = 0;

        if (bufferStr.size() > 79)
        {
            while ((unsigned int)curPos < bufferStr.size())
            {
                if (bufferStr.substr(curPos, 1).compare("\n") == 0)
                {
                    lastNewLine = curPos;
                }

                if (curPos - lastNewLine > 79)
                {
                    bufferStr2.insert(curPos, "\n");
                    lastNewLine = curPos;
                    curPos++;
                }

                curPos++;
            }
        }

        if (PrintSelection == PRINT_BTH)
        {
            //fputs (bufferStr2.c_str(),STDOUT_File);
            printf("%s", bufferStr.c_str());
            fprintf(stderr, "%s", bufferStr2.c_str());
        }

        if (PrintSelection == PRINT_ERR)
        {
            fprintf(stderr, "%s", bufferStr2.c_str());
        }

        if (PrintSelection == PRINT_STD)
        {
            //fputs (bufferStr2.c_str(),STDOUT_File);
            printf("%s", bufferStr.c_str());
        }
    }
    else
    {
        if (PrintSelection == PRINT_BTH)
        {
            vprintf(fmt, ap);
            vfprintf(stderr, fmt, ap);
        }

        if (PrintSelection == PRINT_ERR)
        {
            vfprintf(stderr, fmt, ap);
        }

        if (PrintSelection == PRINT_STD)
        {
            vprintf(fmt, ap);
        }
    }

    //fclose(STDOUT_File);
    va_end(ap);
}
std::string slashCharStr()
{

#if defined(_WIN32)
    return "\\";
#elif defined(linux)
    return "/";
#elif defined(__APPLE__)
    return "/";
#elif defined(__POWERPC__)
    return "/";
#endif

    return "\\";
}
char slashChar()
{

#if defined(_WIN32)
    return '\\';
#elif defined(linux)
    return '/';
#elif defined(__APPLE__)
    return '/';
#elif defined(__POWERPC__)
    return'/';
#endif

    return '\\';
}
//-----------------------------------------------------VP8 Settings-------------------------------------------------------------------
void vpxt_default_parameters(VP8_CONFIG &opt)
{
    /////////Orig Tester Parms//////////
    opt.allow_lag = 1;
    opt.alt_freq = 16;
    opt.alt_q = 20;
    opt.cpu_used = 0;
    opt.encode_breakout = 0;
    opt.gold_q = 28;
    opt.key_q = 12;
    opt.play_alternate = 1;
    opt.worst_allowed_q = 56;
    opt.lag_in_frames = 0;
    //////////////////////////////////
    //////////////IVFEnc Parms////////////
    //opt.allow_lag = 0;
    //opt.alt_freq = 0;
    //opt.alt_q = 0;
    //opt.cpu_used = -4;
    //opt.encode_breakout = 800;
    //opt.gold_q = 0;
    //opt.key_q = 0;
    //opt.play_alternate = 0;
    //opt.worst_allowed_q = 63;
    //opt.lag_in_frames = 0;
    //////////////////////////////////////

    //included in default settings file
    opt.allow_df = 0;
    //opt.allow_lag = 1;
    opt.allow_spatial_resampling = 0;
    //opt.alt_freq = 16;
    //opt.alt_q = 20;
    opt.auto_key = 1;
    opt.best_allowed_q = 4;
    //opt.cpu_used = 0;
    opt.drop_frames_water_mark = 70;
    //opt.encode_breakout = 0; //this may need to be set to 800 defaultly check vp8_cx_iface.c LN 46
    opt.end_usage = 1;
    opt.fixed_q = -1;
    //opt.gold_q = 28;
    opt.key_freq = 999999;
    //opt.key_q = 12;
    opt.maximum_buffer_size = 6;
    opt.Mode = 2;
    opt.noise_sensitivity = 0;
    opt.optimal_buffer_level = 5;
    //opt.play_alternate = 1; //this may be being used incorrectly for EnableAutoAltRef for some reason
    opt.resample_down_water_mark = 30;
    opt.resample_up_water_mark = 60;
    opt.Sharpness = 0;
    opt.starting_buffer_level = 4;
    opt.target_bandwidth = 40;
    opt.two_pass_vbrbias = 50;
    opt.two_pass_vbrmax_section = 400;
    opt.two_pass_vbrmin_section = 0;
    opt.under_shoot_pct = 95;
    opt.Version = 0;
    //opt.worst_allowed_q = 56;

    //not included in default settings file
    opt.Height = 0;
    opt.multi_threaded = 0;
    opt.Width = 0;

    opt.token_partitions = 0;
    opt.error_resilient_mode = 0;

    opt.cq_level = 10;
    opt.arnr_max_frames = 0;
    opt.arnr_strength = 3;
    opt.arnr_type = 3;
}
int vpxt_core_config_to_api_config(VP8_CONFIG coreCfg, vpx_codec_enc_cfg_t *cfg)
{
    //Converts a core configuration to api configuration

    cfg->g_threads = coreCfg.multi_threaded;
    cfg->g_profile = coreCfg.Version;
    cfg->g_error_resilient = coreCfg.error_resilient_mode;
    cfg->rc_resize_allowed = coreCfg.allow_spatial_resampling;
    cfg->rc_resize_up_thresh = coreCfg.resample_up_water_mark;
    cfg->rc_resize_down_thresh = coreCfg.resample_down_water_mark;
    cfg->rc_target_bitrate = coreCfg.target_bandwidth;
    cfg->rc_min_quantizer = coreCfg.best_allowed_q;
    cfg->rc_max_quantizer = coreCfg.worst_allowed_q;
    cfg->rc_undershoot_pct = coreCfg.under_shoot_pct;
    cfg->rc_buf_sz = coreCfg.maximum_buffer_size * 1000;
    cfg->rc_buf_initial_sz  = coreCfg.starting_buffer_level * 1000;
    cfg->rc_buf_optimal_sz  = coreCfg.optimal_buffer_level * 1000;
    cfg->rc_2pass_vbr_bias_pct      = coreCfg.two_pass_vbrbias;
    cfg->rc_2pass_vbr_minsection_pct    = coreCfg.two_pass_vbrmin_section;
    cfg->rc_2pass_vbr_maxsection_pct  = coreCfg.two_pass_vbrmax_section;

    if (coreCfg.auto_key == 0)
    {
        cfg->kf_mode                = VPX_KF_FIXED;
    }

    if (coreCfg.auto_key == 1)
    {
        cfg->kf_mode                = VPX_KF_AUTO;
    }

    cfg->kf_max_dist                = coreCfg.key_freq;

    if (coreCfg.fixed_q != -1)
    {
        if (coreCfg.fixed_q > 63)
        {
            coreCfg.fixed_q = 63;
        }

        if (coreCfg.fixed_q < 0)
        {
            coreCfg.fixed_q = 0;
        }

        cfg->rc_min_quantizer = coreCfg.fixed_q;
        cfg->rc_max_quantizer = coreCfg.fixed_q;
    }

    if (coreCfg.allow_lag == 0)
    {
        cfg->g_lag_in_frames = 0;
    }
    else
    {
        cfg->g_lag_in_frames = coreCfg.lag_in_frames;
    }

    if (coreCfg.allow_df == 0)
    {
        cfg->rc_dropframe_thresh = 0;
    }
    else
    {
        cfg->rc_dropframe_thresh = coreCfg.drop_frames_water_mark;
    }

    if (coreCfg.end_usage == USAGE_LOCAL_FILE_PLAYBACK)
    {
        cfg->rc_end_usage = VPX_VBR;
    }

    if (coreCfg.end_usage == USAGE_STREAM_FROM_SERVER)
    {
        cfg->rc_end_usage = VPX_CBR;
    }

    if (coreCfg.end_usage == USAGE_CONSTRAINED_QUALITY)
    {
        cfg->rc_end_usage = VPX_CQ;
    }

    return 0;
}
VP8_CONFIG vpxt_random_parameters(VP8_CONFIG &opt, const char *inputfile, int display)
{
    //Ranges can be found in validate_config in vp8_cx_iface.c

    //////////////////////////////////Randomly Generated\\\\\\\\\\\\\\\\\\\\\

    srand(vpxt_get_high_res_timer_tick());
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
        vpxt_format_ivf_header_read(&ivfhRaw);

        w       = ivfhRaw.width;
        h       = ivfhRaw.height;
        fr      = (ivfhRaw.rate / ivfhRaw.scale);
        length  = ivfhRaw.length;

        fclose(GetWHinfile);
    }

    opt.Mode = rand() % 5;              //valid Range: (0 to 4)

    if (opt.Mode == 0)
        opt.noise_sensitivity = 0;           //valid Range:
    else                                     //if Not Real Time Mode 0 to 6
        opt.noise_sensitivity = rand() % 7;  //if Real Time Mode 0 to 0

    if (opt.Mode == 0)
        opt.lag_in_frames = 0;                //valid Range:
    else                                //if Not Real Time Mode 0 to 25
        opt.lag_in_frames = rand() % 26;      //if Real Time Mode 0 to 0

    if (opt.lag_in_frames > 0)
        opt.allow_lag = 1;                   //valid Range:
    else                                //if Not Real Time Mode 0 to 1
        opt.allow_lag = 0;                   //if Real Time Mode 0

    if (opt.Mode == 0)
    {
        opt.cpu_used = rand() % 13 + 4;  //valid Range:

        if (rand() % 2)               //if Not Real Time Mode -16 to 16
            opt.cpu_used = opt.cpu_used * -1; //if Real Time Mode -16 to -4 or 4 to 16
    }
    else
    {
        opt.cpu_used = rand() % 17;

        if (rand() % 2)
            opt.cpu_used = opt.cpu_used * -1;
    }

    if (rand() % 21 == 20) //1 out of 20 chance of a fixed q
    {
        opt.fixed_q = rand() % 64; //valid Range: 0 to 63 or -1 (-1 = fixedQ off)
        opt.best_allowed_q = opt.fixed_q; //valid Range: 0 to 63
        opt.worst_allowed_q   = opt.fixed_q; //valid Range: 0 to 63
        opt.cq_level   = rand() % 64; //valid Range: 0 to 63
    }
    else
    {
        opt.fixed_q = -1; //valid Range: 0 to 63 or -1 (-1 = fixedQ off)
        opt.best_allowed_q = rand() % 64; //valid Range: 0 to 63
        opt.worst_allowed_q   = rand() % 64; //valid Range: 0 to 63
        opt.cq_level   = rand() % 64; //valid Range: 0 to 63
    }

    opt.auto_key = rand() % 2; //valid Range: 0 to 1
    opt.multi_threaded =  rand() % 65; //valid Range: 0 to 64
    opt.under_shoot_pct = rand() % 101; //valid Range: 0 to 100
    opt.allow_df = rand() % 2; //valid Range: 0 to 1

    if (opt.allow_df > 0)
        opt.drop_frames_water_mark = rand() % 101; //valid Range: 0 to 100
    else
        opt.drop_frames_water_mark = 0;

    opt.allow_spatial_resampling = rand() % 2; //valid Range: 0 to 1
    opt.resample_up_water_mark = rand() % 101; //valid Range: 0 to 100
    opt.resample_down_water_mark = rand() % 101; //valid Range:  0 to 100
    opt.two_pass_vbrbias = rand() % 101; //valid Range: 0 to 100
    opt.encode_breakout = rand() % 101; //valid Range:
    opt.end_usage = rand() % 3; //valid Range: 0 to 2
    opt.Version = rand() % 4; //valid Range: 0 to 3
    opt.Sharpness = rand() % 8; //valid Range: 0 to 7
    opt.play_alternate = rand() % 2; //valid Range: 0 to 1
    opt.token_partitions = rand() % 4; //valid Range: 0 to 3
    opt.error_resilient_mode = rand() % 101; //valid Range: 0 to 100

    opt.target_bandwidth = rand() % ((w * h) / (320 * 240) * 2048); //valid Range: No Max so based on resolution
    opt.key_freq = rand() % length + 2; //valid Range: No Max so based on number of frames

    opt.arnr_max_frames = rand() % 16; //valid Range: 0 to 15
    opt.arnr_strength = rand() % 7; //valid Range: 6
    opt.arnr_type = rand() % 4;//valid Range: 1 to 3

    ////////////////////////////////No Range Documentation////////////////////////////////
    //opt.maximum_buffer_size  = rand()% 101; //valid Range:

    //randnum = rand()% 101;
    //randomCalc =opt.maximum_buffer_size * (randnum/100.0);
    //opt.starting_buffer_level = randomCalc;

    //randnum = rand()% 101; //valid Range:
    //randomCalc = 15*(randnum/100.0)+opt.maximum_buffer_size;
    //opt.optimal_buffer_level  = randomCalc;

    //opt.two_pass_vbrmin_section = rand()% 101; //valid Range:
    //opt.two_pass_vbrmax_section = rand()% 101; //valid Range:
    /////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////Cannot be accessed through API////////////////////////////
    //opt.alt_freq = rand()% 101; //valid Range:
    //opt.alt_q = rand()% 101; //valid Range:
    //opt.gold_q = rand()% 101; //valid Range:
    //opt.key_q = rand()% 101; //valid Range:
    //opt.FrameRate = rand()% 101; //valid Range:
    //////////////////////////////////////////////////////////////////////////////////////

    opt.maximum_buffer_size = 6;
    opt.starting_buffer_level = 4;
    opt.optimal_buffer_level = 5;
    opt.two_pass_vbrmin_section = 400;
    opt.two_pass_vbrmax_section = 0;

    opt.alt_freq = 16;
    opt.alt_q = 20;
    opt.gold_q = 28;
    opt.key_q = 12;

    tprintf(PRINT_STD, "\nRandom Parameters Generated:");
    std::cout << "\n\n";
    std::cout << "TargetBandwidth " << opt.target_bandwidth << "\n";
    std::cout << "NoiseSensitivity " << opt.noise_sensitivity << "\n";
    std::cout << "Sharpness " << opt.Sharpness << "\n";
    std::cout << "CpuUsed " << opt.cpu_used << "\n";
    std::cout << "Mode " << opt.Mode << "\n";
    std::cout << "AutoKey " << opt.auto_key << "\n";
    std::cout << "KeyFreq " << opt.key_freq << "\n";
    std::cout << "EndUsage " << opt.end_usage << "\n";
    std::cout << "UnderShootPct " << opt.under_shoot_pct << "\n";
    std::cout << "StartingBufferLevel " << opt.starting_buffer_level << "\n";
    std::cout << "OptimalBufferLevel " << opt.optimal_buffer_level << "\n";
    std::cout << "MaximumBufferSize " << opt.maximum_buffer_size << "\n";
    std::cout << "FixedQ " << opt.fixed_q << "\n";
    std::cout << "WorstAllowedQ " << opt.worst_allowed_q  << "\n";
    std::cout << "BestAllowedQ " << opt.best_allowed_q << "\n";
    std::cout << "AllowSpatialResampling " << opt.allow_spatial_resampling << "\n";
    std::cout << "ResampleDownWaterMark " << opt.resample_down_water_mark << "\n";
    std::cout << "ResampleUpWaterMark " << opt.resample_up_water_mark << "\n";
    std::cout << "AllowDF " << opt.allow_df << "\n";
    std::cout << "DropFramesWaterMark " << opt.drop_frames_water_mark << "\n";
    std::cout << "AllowLag " << opt.allow_lag << "\n";
    std::cout << "PlayAlternate " << opt.play_alternate << "\n";
    std::cout << "AltQ " << opt.alt_q << "\n";
    std::cout << "AltFreq " << opt.alt_freq << "\n";
    std::cout << "GoldQ " << opt.gold_q << "\n";
    std::cout << "KeyQ " << opt.key_q << "\n";
    std::cout << "Version " << opt.Version << "\n";
    std::cout << "LagInFrames " << opt.lag_in_frames << "\n";
    std::cout << "TwoPassVBRBias " << opt.two_pass_vbrbias << "\n";
    std::cout << "TwoPassVBRMinSection " << opt.two_pass_vbrmin_section << "\n";
    std::cout << "TwoPassVBRMaxSection " << opt.two_pass_vbrmax_section << "\n";
    std::cout << "EncodeBreakout " << opt.encode_breakout << "\n";
    std::cout << "TokenPartitions " << opt.token_partitions << "\n";
    std::cout << "MultiThreaded " << opt.multi_threaded << "\n";
    std::cout << "ErrorResilientMode " << opt.error_resilient_mode << "\n";

    if (display == 1)
    {
        fprintf(stderr, "\nRandom Parameters Generated:");
        std::cerr << "\n\n";
        std::cerr << "TargetBandwidth " << opt.target_bandwidth << "\n";
        std::cerr << "NoiseSensitivity " << opt.noise_sensitivity << "\n";
        std::cerr << "Sharpness " << opt.Sharpness << "\n";
        std::cerr << "CpuUsed " << opt.cpu_used << "\n";
        std::cerr << "Mode " << opt.Mode << "\n";
        std::cerr << "AutoKey " << opt.auto_key << "\n";
        std::cerr << "KeyFreq " << opt.key_freq << "\n";
        std::cerr << "EndUsage " << opt.end_usage << "\n";
        std::cerr << "UnderShootPct " << opt.under_shoot_pct << "\n";
        std::cerr << "StartingBufferLevel " << opt.starting_buffer_level << "\n";
        std::cerr << "OptimalBufferLevel " << opt.optimal_buffer_level << "\n";
        std::cerr << "MaximumBufferSize " << opt.maximum_buffer_size << "\n";
        std::cerr << "FixedQ " << opt.fixed_q << "\n";
        std::cerr << "WorstAllowedQ " << opt.worst_allowed_q  << "\n";
        std::cerr << "BestAllowedQ " << opt.best_allowed_q << "\n";
        std::cerr << "AllowSpatialResampling " << opt.allow_spatial_resampling << "\n";
        std::cerr << "ResampleDownWaterMark " << opt.resample_down_water_mark << "\n";
        std::cerr << "ResampleUpWaterMark " << opt.resample_up_water_mark << "\n";
        std::cerr << "AllowDF " << opt.allow_df << "\n";
        std::cerr << "DropFramesWaterMark " << opt.drop_frames_water_mark << "\n";
        std::cerr << "AllowLag " << opt.allow_lag << "\n";
        std::cerr << "PlayAlternate " << opt.play_alternate << "\n";
        std::cerr << "AltQ " << opt.alt_q << "\n";
        std::cerr << "AltFreq " << opt.alt_freq << "\n";
        std::cerr << "GoldQ " << opt.gold_q << "\n";
        std::cerr << "KeyQ " << opt.key_q << "\n";
        std::cerr << "Version " << opt.Version << "\n";
        std::cerr << "LagInFrames " << opt.lag_in_frames << "\n";
        std::cerr << "TwoPassVBRBias " << opt.two_pass_vbrbias << "\n";
        std::cerr << "TwoPassVBRMinSection " << opt.two_pass_vbrmin_section << "\n";
        std::cerr << "TwoPassVBRMaxSection " << opt.two_pass_vbrmax_section << "\n";
        std::cerr << "EncodeBreakout " << opt.encode_breakout << "\n";
        std::cerr << "TokenPartitions " << opt.token_partitions << "\n";
        std::cerr << "MultiThreaded " << opt.multi_threaded << "\n";
        std::cerr << "ErrorResilientMode " << opt.error_resilient_mode << "\n";
    }

    //std::cout << "FrameRate " << opt.FrameRate << "\n";
    //std::cerr << "FrameRate " << opt.FrameRate << "\n";

    return opt;
}
VP8_CONFIG vpxt_input_settings(const char *inputFile)
{
    // Reads an input file and sets VP8_CONFIG accordingly
    std::ifstream infile2(inputFile);

    char firstPFile[256];
    std::string Garbage;

    VP8_CONFIG opt;

    infile2 >> opt.target_bandwidth;
    infile2 >> Garbage;
    infile2 >> opt.noise_sensitivity;
    infile2 >> Garbage;
    infile2 >> opt.Sharpness;
    infile2 >> Garbage;
    infile2 >> opt.cpu_used;
    infile2 >> Garbage;
    infile2 >> opt.Mode;
    infile2 >> Garbage;
    infile2 >> opt.auto_key;
    infile2 >> Garbage;
    infile2 >> opt.key_freq;
    infile2 >> Garbage;
    infile2 >> opt.end_usage;
    infile2 >> Garbage;
    infile2 >> opt.under_shoot_pct;
    infile2 >> Garbage;
    infile2 >> opt.starting_buffer_level;
    infile2 >> Garbage;
    infile2 >> opt.optimal_buffer_level;
    infile2 >> Garbage;
    infile2 >> opt.maximum_buffer_size;
    infile2 >> Garbage;
    infile2 >> opt.fixed_q;
    infile2 >> Garbage;
    infile2 >> opt.worst_allowed_q;
    infile2 >> Garbage;
    infile2 >> opt.best_allowed_q;
    infile2 >> Garbage;
    infile2 >> opt.allow_spatial_resampling;
    infile2 >> Garbage;
    infile2 >> opt.resample_down_water_mark;
    infile2 >> Garbage;
    infile2 >> opt.resample_up_water_mark;
    infile2 >> Garbage;
    infile2 >> opt.allow_df;
    infile2 >> Garbage;
    infile2 >> opt.drop_frames_water_mark;
    infile2 >> Garbage;
    infile2 >> opt.allow_lag;
    infile2 >> Garbage;
    infile2 >> opt.play_alternate;
    infile2 >> Garbage;
    infile2 >> opt.alt_q;
    infile2 >> Garbage;
    infile2 >> opt.alt_freq;
    infile2 >> Garbage;
    infile2 >> opt.gold_q;
    infile2 >> Garbage;
    infile2 >> opt.key_q;
    infile2 >> Garbage;
    infile2 >> opt.Version;
    infile2 >> Garbage;
    infile2 >> opt.lag_in_frames;
    infile2 >> Garbage;
    infile2 >> opt.two_pass_vbrbias;
    infile2 >> Garbage;
    infile2 >> opt.two_pass_vbrmin_section;
    infile2 >> Garbage;
    infile2 >> opt.two_pass_vbrmax_section;
    infile2 >> Garbage;
    infile2 >> opt.encode_breakout;
    infile2 >> Garbage;
    infile2 >> opt.token_partitions ;
    infile2 >> Garbage;
    infile2 >> opt.multi_threaded;
    infile2 >> Garbage;
    infile2 >> opt.error_resilient_mode;
    infile2 >> Garbage;

    //not included in default settings file
    //infile2 >> firstPFile;
    //infile2 >> Garbage;
    infile2 >> opt.Height;
    infile2 >> Garbage;
    infile2 >> opt.Width;
    infile2 >> Garbage;

    infile2 >> opt.cq_level;
    infile2 >> Garbage;
    infile2 >> opt.arnr_max_frames;
    infile2 >> Garbage;
    infile2 >> opt.arnr_strength;
    infile2 >> Garbage;
    infile2 >> opt.arnr_type;
    infile2 >> Garbage;

    infile2.close();

    return opt;
}
int vpxt_output_settings(const char *outputFile, VP8_CONFIG opt)
{
    //Saves all VP8_CONFIG settings to a settings file readable by InputSettings

    std::ofstream outfile(outputFile);

    outfile <<  opt.target_bandwidth << " TargetBandwidth\n";
    outfile <<  opt.noise_sensitivity << " NoiseSensitivity\n";
    outfile <<  opt.Sharpness << " Sharpness\n";
    outfile <<  opt.cpu_used << " CpuUsed\n";
    outfile <<  opt.Mode << " Mode\n";
    outfile <<  opt.auto_key << " AutoKey\n";
    outfile <<  opt.key_freq << " KeyFreq\n";
    outfile <<  opt.end_usage << " EndUsage\n";
    outfile <<  opt.under_shoot_pct << " UnderShootPct\n";
    outfile <<  opt.starting_buffer_level << " StartingBufferLevel\n";
    outfile <<  opt.optimal_buffer_level << " OptimalBufferLevel\n";
    outfile <<  opt.maximum_buffer_size << " MaximumBufferSize\n";
    outfile <<  opt.fixed_q << " FixedQ\n";
    outfile <<  opt.worst_allowed_q << " WorstAllowedQ\n";
    outfile <<  opt.best_allowed_q << " BestAllowedQ\n";
    outfile <<  opt.allow_spatial_resampling << " AllowSpatialResampling\n";
    outfile <<  opt.resample_down_water_mark << " ResampleDownWaterMark\n";
    outfile <<  opt.resample_up_water_mark << " ResampleUpWaterMark\n";
    outfile <<  opt.allow_df << " AllowDF\n";
    outfile <<  opt.drop_frames_water_mark << " DropFramesWaterMark\n";
    outfile <<  opt.allow_lag << " AllowLag\n";
    outfile <<  opt.play_alternate << " PlayAlternate\n";
    outfile <<  opt.alt_q << " AltQ\n";
    outfile <<  opt.alt_freq << " AltFreq\n";
    outfile <<  opt.gold_q << " GoldQ\n";
    outfile <<  opt.key_q << " KeyQ\n";
    outfile <<  opt.Version << " Version\n";
    outfile <<  opt.lag_in_frames << " LagInFrames\n";
    outfile <<  opt.two_pass_vbrbias << " TwoPassVBRBias\n";
    outfile <<  opt.two_pass_vbrmin_section << " TwoPassVBRMinSection\n";
    outfile <<  opt.two_pass_vbrmax_section << " TwoPassVBRMaxSection\n";
    outfile <<  opt.encode_breakout << " EncodeBreakout\n";
    outfile <<  opt.token_partitions  << " TokenPartitions\n";
    outfile <<  opt.multi_threaded << " MultiThreaded\n";
    outfile <<  opt.error_resilient_mode << " ErrorResilientMode\n";

    //not included in default settings file
    outfile << opt.Height << " Height\n";
    outfile << opt.Width << " Width\n";

    outfile << opt.cq_level << " CQLevel\n";
    outfile << opt.arnr_max_frames << " ArnrMaxFrames\n";
    outfile << opt.arnr_strength << " ArnrStr\n";
    outfile << opt.arnr_type << " ArnrType\n";

    outfile.close();
    return 0;
}
int vpxt_output_compatable_settings(const char *outputFile, VP8_CONFIG opt, int ParVersionNum)
{
    //Saves all VP8_CONFIG settings to a settings file readable by InputSettings
    //Tester Uses prebuilt executables from prior builds

    std::ofstream outfile(outputFile);

    outfile <<  opt.target_bandwidth << " TargetBandwidth\n";
    outfile <<  opt.noise_sensitivity << " NoiseSensitivity\n";
    outfile <<  opt.Sharpness << " Sharpness\n";
    outfile <<  opt.cpu_used << " CpuUsed\n";
    outfile <<  opt.Mode << " Mode\n";

    if (ParVersionNum == 1)
    {
        tprintf(PRINT_BTH, "\nRunning in 1.0.4 and earlier Parameter Compatability Mode\n");
        outfile <<  1 << " DeleteFirstPassFile\n";
    }

    outfile <<  opt.auto_key << " AutoKey\n";
    outfile <<  opt.key_freq << " KeyFreq\n";
    outfile <<  opt.end_usage << " EndUsage\n";
    outfile <<  opt.under_shoot_pct << " UnderShootPct\n";
    outfile <<  opt.starting_buffer_level << " StartingBufferLevel\n";
    outfile <<  opt.optimal_buffer_level << " OptimalBufferLevel\n";
    outfile <<  opt.maximum_buffer_size << " MaximumBufferSize\n";
    outfile <<  opt.fixed_q << " FixedQ\n";
    outfile <<  opt.worst_allowed_q << " WorstAllowedQ\n";
    outfile <<  opt.best_allowed_q << " BestAllowedQ\n";
    outfile <<  opt.allow_spatial_resampling << " AllowSpatialResampling\n";
    outfile <<  opt.resample_down_water_mark << " ResampleDownWaterMark\n";
    outfile <<  opt.resample_up_water_mark << " ResampleUpWaterMark\n";
    outfile <<  opt.allow_df << " AllowDF\n";
    outfile <<  opt.drop_frames_water_mark << " DropFramesWaterMark\n";
    outfile <<  opt.allow_lag << " AllowLag\n";
    outfile <<  opt.play_alternate << " PlayAlternate\n";
    outfile <<  opt.alt_q << " AltQ\n";
    outfile <<  opt.alt_freq << " AltFreq\n";
    outfile <<  opt.gold_q << " GoldQ\n";
    outfile <<  opt.key_q << " KeyQ\n";
    outfile <<  opt.Version << " Version\n";
    outfile <<  opt.lag_in_frames << " LagInFrames\n";
    outfile <<  opt.two_pass_vbrbias << " TwoPassVBRBias\n";
    outfile <<  opt.two_pass_vbrmin_section << " TwoPassVBRMinSection\n";
    outfile <<  opt.two_pass_vbrmax_section << " TwoPassVBRMaxSection\n";
    outfile <<  opt.encode_breakout << " EncodeBreakout\n";
    outfile <<  opt.token_partitions  << " TokenPartitions\n";
    outfile <<  opt.multi_threaded << " MultiThreaded\n";
    outfile <<  opt.error_resilient_mode << " ErrorResilientMode\n";

    //not included in default settings file
    if (ParVersionNum == 1)
    {
        std::string DummyFPFFile = outputFile;
        DummyFPFFile.append(".fpf");
        outfile << "Test" << " FirstPassFile\n";
    }

    outfile << opt.Height << " Height\n";
    outfile << opt.Width << " Width\n";
    outfile << opt.cq_level << " CQLevel\n";

    outfile.close();
    return 0;
}
int vpxt_output_settings_api(const char *outputFile, vpx_codec_enc_cfg_t cfg)
{
    //Saves all vpx_codec_enc_cfg_t settings to a settings file

    std::ofstream outfile(outputFile);

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

int vpxt_input_settings_api(const char *inputFile, vpx_codec_enc_cfg_t &cfg)
{
    //Saves all vpx_codec_enc_cfg_t settings to a settings file

    std::ifstream infile(inputFile);
    std::string Garbage;
    int IntValue;

    infile >> cfg.g_usage;
    infile >> Garbage;
    infile >> cfg.g_threads;
    infile >> Garbage;
    infile >> cfg.g_profile;
    infile >> Garbage;
    infile >> cfg.g_w;
    infile >> Garbage;
    infile >> cfg.g_h;
    infile >> Garbage;
    infile >> cfg.g_timebase.num;
    infile >> Garbage;
    infile >> cfg.g_timebase.den;
    infile >> Garbage;
    infile >> cfg.g_error_resilient;
    infile >> Garbage;
    infile >> IntValue;

    if (IntValue == 0)
        cfg.g_pass = VPX_RC_ONE_PASS;

    if (IntValue == 1)
        cfg.g_pass = VPX_RC_FIRST_PASS;

    if (IntValue == 2)
        cfg.g_pass = VPX_RC_LAST_PASS;

    infile >> Garbage;
    infile >> cfg.g_lag_in_frames;
    infile >> Garbage;
    infile >> cfg.rc_dropframe_thresh;
    infile >> Garbage;
    infile >> cfg.rc_resize_allowed;
    infile >> Garbage;
    infile >> cfg.rc_resize_up_thresh;
    infile >> Garbage;
    infile >> cfg.rc_resize_down_thresh;
    infile >> Garbage;
    infile >> IntValue;

    if (IntValue == 0)
        cfg.rc_end_usage = VPX_VBR;

    if (IntValue == 1)
        cfg.rc_end_usage = VPX_CBR;

    infile >> Garbage;
    infile >> cfg.rc_target_bitrate;
    infile >> Garbage;
    infile >> cfg.rc_min_quantizer;
    infile >> Garbage;
    infile >> cfg.rc_max_quantizer;
    infile >> Garbage;
    infile >> cfg.rc_undershoot_pct;
    infile >> Garbage;
    infile >> cfg.rc_overshoot_pct;
    infile >> Garbage;
    infile >> cfg.rc_buf_sz;
    infile >> Garbage;
    infile >> cfg.rc_buf_initial_sz;
    infile >> Garbage;
    infile >> cfg.rc_buf_optimal_sz;
    infile >> Garbage;
    infile >> cfg.rc_2pass_vbr_bias_pct;
    infile >> Garbage;
    infile >> cfg.rc_2pass_vbr_minsection_pct;
    infile >> Garbage;
    infile >> cfg.rc_2pass_vbr_maxsection_pct;
    infile >> Garbage;
    infile >> IntValue;

    if (IntValue == 0)
        cfg.kf_mode = VPX_KF_FIXED;

    if (IntValue == 1)
        cfg.kf_mode = VPX_KF_AUTO;

    infile >> Garbage;
    infile >> cfg.kf_min_dist;
    infile >> Garbage;
    infile >> cfg.kf_max_dist;
    infile >> Garbage;

    infile.close();
    return 0;
}
int vpxt_output_settings_ivfenc(const char *outputFile, VP8_CONFIG opt)
{
    //Saves all VP8_CONFIG settings to a settings file readable by InputSettings

    std::ofstream outfile(outputFile);

    outfile << "TargetBandwidth " << opt.target_bandwidth << "\n";
    outfile << "NoiseSensitivity " << opt.noise_sensitivity <<  "\n";
    outfile << "Sharpness " << opt.Sharpness <<  "\n";
    outfile << "CpuUsed " << opt.cpu_used <<  "\n";
    outfile << "Mode " << opt.Mode <<  "\n";
    outfile << "AutoKey " << opt.auto_key <<  "\n";
    outfile << "KeyFreq " << opt.key_freq <<  "\n";
    outfile << "EndUsage " << opt.end_usage <<  "\n";
    outfile << "UnderShootPct " << opt.under_shoot_pct <<  "\n";
    outfile << "StartingBufferLevel " << opt.starting_buffer_level <<  "\n";
    outfile << "OptimalBufferLevel " << opt.optimal_buffer_level <<  "\n";
    outfile << "MaximumBufferSize " << opt.maximum_buffer_size <<  "\n";
    outfile << " FixedQ " << opt.fixed_q <<  "\n";
    outfile << "WorstAllowedQ " << opt.worst_allowed_q <<  "\n";
    outfile << "BestAllowedQ " << opt.best_allowed_q <<  "\n";
    outfile << " llowSpatialResampling " << opt.allow_spatial_resampling <<  "\n";
    outfile << "ResampleDownWaterMark " << opt.resample_down_water_mark <<  "\n";
    outfile << "ResampleUpWaterMark " << opt.resample_up_water_mark <<  "\n";
    outfile << "AllowDF " << opt.allow_df << "\n";
    outfile << "DropFramesWaterMark " << opt.drop_frames_water_mark <<  "\n";
    outfile << "TwoPassVBRBias " << opt.two_pass_vbrbias <<  "\n";
    outfile << "TwoPassVBRMinSection " << opt.two_pass_vbrmin_section <<  "\n";
    outfile << "TwoPassVBRMaxSection " << opt.two_pass_vbrmax_section <<  "\n";
    outfile << "AllowLag " << opt.allow_lag <<  "\n";
    outfile << "LagInFrames " << opt.lag_in_frames <<  "\n";
    outfile << "AltFreq " << opt.alt_freq <<  "\n";
    outfile << "KeyQ " << opt.key_q <<  "\n";
    outfile << "AltQ " << opt.alt_q <<  "\n";
    outfile << "GoldQ " << opt.gold_q <<  "\n";
    outfile << "PlayAlternate " << opt.play_alternate <<  "\n";
    outfile << "Version " << opt.Version <<  "\n";
    outfile << "EncodeBreakout " << opt.encode_breakout <<  "\n";

    outfile.close();
    return 0;
}
int vpxt_convert_par_file_to_ivfenc(const char *input, const char *output)
{

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt = vpxt_input_settings(input);
    vpxt_output_settings_ivfenc(output, opt);

    return 0;
}
int vpxt_convert_par_file_to_vpxenc(const char *input_core, const char *input_api)
{
    VP8_CONFIG opt;
    opt = vpxt_input_settings(input_core);

    vpx_codec_enc_cfg_t    cfg;
    const struct codec_item  *codec = codecs;
    vpx_codec_enc_config_default(codec->iface, &cfg, 0);
    vpxt_input_settings_api(input_api, cfg);

    //--debug                                                                           //Debug mode (makes output deterministic)
    //--output=<arg>                                                                    //Output filename
    //--codec=<arg>                                                                     //Codec to use
    if (opt.Mode > 2)
        tprintf(PRINT_STD, "--passes=2 ");                                          //Number of passes (1/2)

    //--pass=<arg>                                                                      //Pass to execute (1/2)
    //--fpf=<arg>                                                                       //First pass statistics file name
    //--limit=<arg>                                                                     //Stop encoding after n input frames
    //--deadline=<arg>                                                                  //Deadline per frame (usec)
    if (opt.Mode == 2 || opt.Mode == 5)
        tprintf(PRINT_STD, "--best ");                                              //Use Best Quality Deadline

    if (opt.Mode == 1 || opt.Mode == 4)
        tprintf(PRINT_STD, "--good ");                                              //Use Good Quality Deadline

    if (opt.Mode == 0)
        tprintf(PRINT_STD, "--rt ");                                                //Use Realtime Quality Deadline

    tprintf(PRINT_STD, "--verbose ");                                                   //Show encoder parameters
    //--psnr                                                                            //Show PSNR in status line
    tprintf(PRINT_STD, "--ivf ");                                                       //Output IVF (default is WebM)

//Encoder Global Options:
    //--yv12                                                                            //Input file is YV12
    //--i420                                                                            //Input file is I420 (default)
    tprintf(PRINT_STD, "--usage=%i ", cfg.g_usage);                                     //Usage profile number to use
    tprintf(PRINT_STD, "--threads=%i ", opt.multi_threaded);                            //Max number of threads to use
    tprintf(PRINT_STD, "--profile=%i ", opt.Version);                                   //Bitstream profile number to use
    tprintf(PRINT_STD, "--width=%i ", opt.Width);                                       //Frame width
    tprintf(PRINT_STD, "--height=%i ", opt.Height);                                     //Frame height
    tprintf(PRINT_STD, "--timebase=%i/%i ", cfg.g_timebase.num, cfg.g_timebase.den);    //Stream timebase (frame duration)
    tprintf(PRINT_STD, "--fps=%i/%i ", cfg.g_timebase.den / 2, cfg.g_timebase.num);      //Stream frame rate (rate/scale)
    tprintf(PRINT_STD, "--error-resilient=%i ", opt.error_resilient_mode);              //Enable error resiliency features

    if (opt.allow_lag == 0)
        tprintf(PRINT_STD, "--lag-in-frames=%i ", 0);                                   //Max number of frames to lag
    else
        tprintf(PRINT_STD, "--lag-in-frames=%i ", opt.lag_in_frames);                   //Max number of frames to lag

//Rate Control Options:
    if (opt.allow_df == 0)
        tprintf(PRINT_STD, "--drop-frame=%i ", 0);                                      //Temporal resampling threshold (buf %)
    else
        tprintf(PRINT_STD, "--drop-frame=%i ", opt.drop_frames_water_mark);             //Temporal resampling threshold (buf %)

    tprintf(PRINT_STD, "--resize-allowed=%i ", opt.allow_spatial_resampling);           //Spatial resampling enabled (bool)
    tprintf(PRINT_STD, "--resize-up=%i ", opt.resample_up_water_mark);                  //Upscale threshold (buf %)
    tprintf(PRINT_STD, "--resize-down=%i ", opt.resample_down_water_mark);              //Downscale threshold (buf %)

    if (opt.end_usage == USAGE_LOCAL_FILE_PLAYBACK)
        tprintf(PRINT_STD, "--end-usage=%i ", VPX_VBR);                                 //VBR=0 | CBR=1

    if (opt.end_usage == USAGE_STREAM_FROM_SERVER)
        tprintf(PRINT_STD, "--end-usage=%i ", VPX_CBR);                                 //VBR=0 | CBR=1

    if (opt.end_usage == USAGE_CONSTRAINED_QUALITY)
        tprintf(PRINT_STD, "--end-usage=%i ", VPX_CQ);                                  //VBR=0 | CBR=1

    tprintf(PRINT_STD, "--target-bitrate=%i ", opt.target_bandwidth);                   //Bitrate (kbps)
    tprintf(PRINT_STD, "--min-q=%i ", opt.best_allowed_q);                              //Minimum (best) quantizer
    tprintf(PRINT_STD, "--max-q=%i ", opt.worst_allowed_q);                             //Maximum (worst) quantizer

    if (opt.fixed_q != -1)
    {
        tprintf(PRINT_STD, "--min-q=%i ", opt.fixed_q);                                 //Minimum (best) quantizer
        tprintf(PRINT_STD, "--max-q=%i ", opt.fixed_q);                                 //Maximum (worst) quantizer
    }

    tprintf(PRINT_STD, "--undershoot-pct=%i ", opt.under_shoot_pct);                    //Datarate undershoot (min) target (%)
    tprintf(PRINT_STD, "--overshoot-pct=%i ", cfg.rc_overshoot_pct);                    //Datarate overshoot (max) target (%)
    tprintf(PRINT_STD, "--buf-sz=%i ", opt.maximum_buffer_size * 1000);                 //Client buffer size (ms)
    tprintf(PRINT_STD, "--buf-initial-sz=%i ", opt.starting_buffer_level * 1000);       //Client initial buffer size (ms)
    tprintf(PRINT_STD, "--buf-optimal-sz=%i ", opt.optimal_buffer_level * 1000);        //Client optimal buffer size (ms)

//Twopass Rate Control Options:
    //--bias-pct=<arg>                                                                 //CBR/VBR bias (0=CBR, 100=VBR)
    //--minsection-pct=<arg>                                                           //GOP min bitrate (% of target)
    //--maxsection-pct=<arg>                                                           //GOP max bitrate (% of target)

//Keyframe Placement Options:
    tprintf(PRINT_STD, "--kf-min-dist=%i ", cfg.kf_min_dist);                           //Minimum keyframe interval (frames)
    tprintf(PRINT_STD, "--kf-max-dist=%i ", opt.key_freq);                              //Maximum keyframe interval (frames)

    if (opt.auto_key == 0)
        tprintf(PRINT_STD, "--disable-kf ");                                            //Disable keyframe placement

//VP8 Specific Options:
    tprintf(PRINT_STD, "--cpu-used=%i ", opt.cpu_used);                                 //CPU Used (-16..16)
    tprintf(PRINT_STD, "--auto-alt-ref=%i ", opt.play_alternate);                       //Enable automatic alt reference frames
    tprintf(PRINT_STD, "--noise-sensitivity=%i ", opt.noise_sensitivity);               //Noise sensitivity (frames to blur)
    tprintf(PRINT_STD, "--sharpness=%i ", opt.Sharpness);                               //Filter sharpness (0-7)
    tprintf(PRINT_STD, "--static-thresh=%i ", opt.encode_breakout);                     //Motion detection threshold
    tprintf(PRINT_STD, "--token-parts=%i ", opt.token_partitions);                      //Number of token partitions to use, log2
    tprintf(PRINT_STD, "--arnr-maxframes=%i ", opt.arnr_max_frames);                   //AltRef Max Frames
    tprintf(PRINT_STD, "--arnr-strength=%i ", opt.arnr_strength);                    //AltRef Strength
    tprintf(PRINT_STD, "--arnr-type=%i ", opt.arnr_type);                        //AltRef Type
    //--tune=<arg>                                                                      //Material to favor - psnr, ssim
    tprintf(PRINT_STD, "--cq-level=%i ", opt.cq_level);

    return 0;
}
//---------------------------------------------------IVF Header Data------------------------------------------------------------------
int vpxt_print_ivf_file_header(IVF_HEADER ivf)
{
    tprintf(PRINT_STD, "IVF FIle Header Data\n\n"
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
            , ivf.version, ivf.headersize, ivf.four_cc, ivf.width, ivf.height, ivf.rate
            , ivf.scale, ivf.length, ivf.unused);
    return 0;
}
int vpxt_format_ivf_header_read(IVF_HEADER *ivf)
{
#ifdef __POWERPC__
    //std::cout << "\n\n\n\n\nPPC DEFINED\n\n\n\n\n";
    // For big endian systems need to swap bytes on height and width
    ivf->width  = ((ivf->width & 0xff) << 8)  | ((ivf->width >> 8) & 0xff);
    ivf->height = ((ivf->height & 0xff) << 8) | ((ivf->height >> 8) & 0xff);
    ivf->rate = (((ivf->rate & 0xff)) << 24)    | (((ivf->rate & 0xff00)) << 8) |   \
                (((ivf->rate & 0xff0000)) >> 8) | (((ivf->rate & 0xff000000)) >> 24);
    ivf->scale = (((ivf->scale & 0xff)) << 24)    | (((ivf->scale & 0xff00)) << 8) |    \
                 (((ivf->scale & 0xff0000)) >> 8) | (((ivf->scale & 0xff000000)) >> 24);
    ivf->length = (((ivf->length & 0xff)) << 24)    | (((ivf->length & 0xff00)) << 8) | \
                  (((ivf->length & 0xff0000)) >> 8) | (((ivf->length & 0xff000000)) >> 24);
#endif

    return 0;
}
int vpxt_format_ivf_header_write(IVF_HEADER &ivf)
{
    ivf.version = 0;
    ivf.headersize = make_endian_16(32);

    ivf.width      = make_endian_16(ivf.width);
    ivf.height     = make_endian_16(ivf.height);
    ivf.scale      = make_endian_32(ivf.scale);
    ivf.rate       = make_endian_32(ivf.rate);
    ivf.length     = make_endian_32(ivf.length);

    if (ivf.four_cc != 842094169 && ivf.four_cc != 808596553)
    {
        //tprintf(  PRINT_STD, "\nUtil_FourCC: %i",ivf.four_cc);
        //tprintf(  PRINT_STD, "\nini vp8 fourcc\n");
        ivf.four_cc     = MAKEFOURCC('V', 'P', '8', '0');
    }

    return 0;
}
int vpxt_format_frame_header_read(IVF_FRAME_HEADER &ivf_fh)
{
#ifdef __POWERPC__
    ivf_fh.frameSize = ((ivf_fh.frameSize & 0xff) << 24) |
                       ((ivf_fh.frameSize & 0xff00) << 8) |
                       ((ivf_fh.frameSize & 0xff0000) >> 8) |
                       ((ivf_fh.frameSize & 0xff000000) >> 24);
    ivf_fh.timeStamp = ((ivf_fh.timeStamp & 0xff) << 56             |   \
                        ((ivf_fh.timeStamp >>  8) & 0xff) << 48 |   \
                        ((ivf_fh.timeStamp >> 16) & 0xff) << 40 |   \
                        ((ivf_fh.timeStamp >> 24) & 0xff) << 32 |   \
                        ((ivf_fh.timeStamp >> 32) & 0xff) << 24 |   \
                        ((ivf_fh.timeStamp >> 40) & 0xff) << 16 |   \
                        ((ivf_fh.timeStamp >> 48) & 0xff) <<  8 |   \
                        ((ivf_fh.timeStamp >> 56) & 0xff));

    //std::cout << "POWERPC-Read\n";
#endif

    return 0;
}
int vpxt_format_frame_header_write(IVF_FRAME_HEADER &ivf_fh)
{
#ifdef __POWERPC__
    ivf_fh.timeStamp = make_endian_64(ivf_fh.timeStamp);
    ivf_fh.frameSize = make_endian_32(ivf_fh.frameSize);
#endif

    return 0;
}
//---------------------------------------------------File Management------------------------------------------------------------------
long vpxt_file_size(const char *inFile, int printbool)
{
    //finds and returns the size of a file with output.
    char FileNameinFile[256];
    vpxt_file_name(inFile, FileNameinFile, 0);

    if (printbool)
        tprintf(PRINT_BTH, "Size of %s: ", FileNameinFile);

    long pos;
    long end;

    FILE *f;
    f = fopen(inFile , "r");

    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    fseek(f, pos, SEEK_SET);

    if (printbool)
        tprintf(PRINT_BTH, "%i bytes", end - pos);

    fclose(f);

    return end - pos;
}
void vpxt_file_name(const char *input, char *FileName, int removeExt)
{
    //Extracts only the files name from its full path.

    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;

    while (input[parser] != '\0')
    {
        if (input[parser] == slashChar())
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

        if (input[parser] == slashChar())
        {
            slashcount2++;
        }

        parser++;
    }

    if (removeExt == 0)FileName[parser2] = '\0';

    if (removeExt == 1)FileName[parser2-4] = '\0';

    return;
}


void vpxt_folder_name(const char *input, char *output)
{
    //Gets the full name of the folder a file is in and returns it.

    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;
    const char *Dir = input;

    while (Dir[parser] != '\0')
    {
        if (Dir[parser] == slashChar())
        {
            slashcount++;
        }

        parser++;
    }

    parser = 0;

    while (slashcount2 < slashcount)
    {
        if (Dir[parser] == slashChar())
        {
            slashcount2++;
        }

        output[parser] = Dir[parser];
        parser++;
    }

    output[parser] = '\0';

    return;
}

void vpxt_remove_file_extension(const char *DirIn, char *DirOut)
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

std::string vpxt_extract_date_time(const std::string InputStr)
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
        if (input[parser] == slashChar())
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

        if (input[parser] == slashChar())
        {
            slashcount2++;
        }

        parser++;
    }

    FileName[parser2] = '\0';
    std::string FileNameStr = FileName;
    return FileNameStr;
}
int vpxt_timestamp_compare(const std::string TimeStampNow, const std::string TimeStampPrevious)
{
    int i = 0;

    while (i < 24)
    {
        if (TimeStampNow[i] != TimeStampPrevious[i])
            return 0;

        i++;
    }

    return 1;
}
int vpxt_identify_test(const char *test_char)
{
    // parse through possible options
    //  if input is number return number
    //  if input is string check for number string coresponds to return that
    //  on error/cant find number string coresponds to return -1

    if (atoi(test_char) != 0)
        return atoi(test_char);
    else
    {
        char test_char_no_space[255];
        vpxt_remove_char_spaces(test_char, test_char_no_space, 255);//make sure no white spaces
        std::string id_test_str = test_char_no_space;
        vpxt_lower_case_string(id_test_str);

        if (id_test_str.substr(0, 1).compare("+") == 0)
            id_test_str.erase(0, 1);

        if (id_test_str.compare("test_allow_drop_frames") == 0)
            return AlWDFNUM;

        if (id_test_str.compare("test_allow_lag") == 0)
            return ALWLGNUM;

        if (id_test_str.compare("test_allow_spatial_resampling") == 0)
            return ALWSRNUM;

        if (id_test_str.compare("test_arnr") == 0)
            return ARNRTNUM;

        if (id_test_str.compare("test_auto_key_frame") == 0)
            return AUTKFNUM;

        if (id_test_str.compare("test_buffer_level") == 0)
            return BUFLVNUM;

        if (id_test_str.compare("test_change_cpu_dec") == 0)
            return CPUDENUM;

        if (id_test_str.compare("test_change_cpu_enc") == 0)
            return CPUENNUM;

        if (id_test_str.compare("test_constrained_quality") == 0)
            return CONQUNUM;

        if (id_test_str.compare("test_data_rate") == 0)
            return DTARTNUM;

        if (id_test_str.compare("test_debug_matches_release") == 0)
            return DBMRLNUM;

        if (id_test_str.compare("test_drop_frame_watermark") == 0)
            return DFWMWNUM;

        if (id_test_str.compare("test_encoder_break_out") == 0)
            return ENCBONUM;

        if (id_test_str.compare("test_error_resolution") == 0)
            return ERRMWNUM;

        if (id_test_str.compare("test_extra_file") == 0)
            return EXTFINUM;

        if (id_test_str.compare("test_fixed_quantizer") == 0)
            return FIXDQNUM;

        if (id_test_str.compare("test_force_key_frame") == 0)
            return FKEFRNUM;

        if (id_test_str.compare("test_frame_size") == 0)
            return FRSZTNUM;

        if (id_test_str.compare("test_good_vs_best") == 0)
            return GQVBQNUM;

        if (id_test_str.compare("test_lag_in_frames") == 0)
            return LGIFRNUM;

        if (id_test_str.compare("test_max_quantizer") == 0)
            return MAXQUNUM;

        if (id_test_str.compare("test_mem_leak") == 0)
            return MEML1NUM;

        if (id_test_str.compare("test_mem_leak2") == 0)
            return MEML2NUM;

        if (id_test_str.compare("test_min_quantizer") == 0)
            return MINQUNUM;

        if (id_test_str.compare("test_multithreaded") == 0)
            return MULTTNUM;

        if (id_test_str.compare("test_new_vs_old_psnr") == 0)
            return NVOPSNUM;

        if (id_test_str.compare("test_new_vs_old_enc_cpu_tick") == 0)
            return NVOECPTK;

        if (id_test_str.compare("test_noise_sensitivity") == 0)
            return NOISENUM;

        if (id_test_str.compare("test_one_pass_vs_two_pass") == 0)
            return OV2PSNUM;

        if (id_test_str.compare("test_play_alternate") == 0)
            return PLYALNUM;

        if (id_test_str.compare("test_post_processor") == 0)
            return POSTPNUM;

        if (id_test_str.compare("test_reconstruct_buffer") == 0)
            return RECBFNUM;

        if (id_test_str.compare("test_resample_down_watermark") == 0)
            return RSDWMNUM;

        if (id_test_str.compare("test_speed") == 0)
            return SPEEDNUM;

        if (id_test_str.compare("test_test_vector") == 0)
            return TVECTNUM;

        if (id_test_str.compare("test_two_pass_vs_two_pass_best") == 0)
            return TV2BTNUM;

        if (id_test_str.compare("test_undershoot") == 0)
            return UNDSHNUM;

        if (id_test_str.compare("test_version") == 0)
            return VERSINUM;

        if (id_test_str.compare("test_win_lin_mac_match") == 0)
            return WMLMMNUM;

        if (id_test_str.compare("0") == 0)
            return 0;
    }

    return -1;
}
int vpxt_run_multiple_tests_input_check(const char *input, int MoreInfo)
{
    //function returns number of tests found if input is correct -1 if not correct and -3 if there is an error

    //////////////////////////////Read Number of Tests//////////////////////////////////////////
    std::fstream infile2;
    infile2.open(input);

    if (!infile2.good())
    {
        tprintf(PRINT_STD, "\nInput File does not exist\n");
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

    if (numberoftests > 990)
    {
        tprintf(PRINT_BTH, "\nNumber of test exceeds current capacity please limit external tests to 990\n");
        return -1;
    }

    int linesskippedtracker = 0;

    char buffer[1024];
    char buffer2[1024];

    int Buf1Var = 0;
    int DummyArgvVar = 1;
    int CommentBool = 0;

    int PassFail[999]; //= new int[numberoftests+2];
    int PassFailInt = 0;
    int TestsRun = 0;

    std::string StringAr[999];// = new string[numberoftests+2];
    std::string SelectorAr[999];// = new string[numberoftests+2];
    std::string SelectorAr2[999];// = new string[numberoftests+2];


    int SelectorArInt = 0;
    int y;
    int PassFailReturn = 1;



    std::fstream infile;
    infile.open(input);

    if (!infile.good())
    {
        tprintf(PRINT_STD, "\nInput File does not exist\n");
        infile.close();
        //delete [] PassFail;
        //delete [] StringAr;
        //delete [] SelectorAr;
        //delete [] SelectorAr2;
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

        std::cout << buffer << "\n";

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
                int selector =  vpxt_identify_test(DummyArgv[1]);

                if (selector >= 0 && selector < MAXTENUM)
                {
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

                if (selector == ARNRTNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "Arnr";
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

                /*if (selector == ALTFRNUM)
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
                }*/

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

                if (selector == CPUENNUM)
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

                if (selector == CONQUNUM)
                {
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 6))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "ConstrainQuality";
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

                if (selector == FRSZTNUM)
                {
                    if (!(DummyArgvVar == 8 || DummyArgvVar == 7))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "FrameSizeTest";
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
                    if (!(DummyArgvVar == 7 || DummyArgvVar == 6))
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

                if (selector == NVOECPTK)
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

                if (selector == RECBFNUM)
                {
                    if (!(DummyArgvVar == 6 || DummyArgvVar == 5))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "RECBFNUMfer";
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
                        SelectorAr2[SelectorArInt] = "WinLinMacMatch";
                        PassFail[PassFailInt] = trackthis1;
                        //std::cout << "\n\n\n\n\n" << DummyArgvVar << "\n\n\n\n\n";
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                //Make sure that all tests input are vaild tests by checking the list (make sure to add new tests here!)
                if (selector != RTFFINUM && selector != AlWDFNUM && selector != ALWLGNUM && selector != FRSZTNUM && selector != ARNRTNUM &&
                    selector != AUTKFNUM && selector != BUFLVNUM && selector != CPUDENUM && selector != CPUENNUM && selector != CONQUNUM &&
                    selector != DFWMWNUM && selector != DTARTNUM && selector != DBMRLNUM && selector != ENCBONUM && selector != ERRMWNUM &&
                    selector != EXTFINUM && selector != FIXDQNUM && selector != FKEFRNUM && selector != GQVBQNUM && selector != LGIFRNUM &&
                    selector != MAXQUNUM && selector != MEML1NUM && selector != MEML2NUM && selector != MINQUNUM && selector != MULTTNUM &&
                    selector != NVOPSNUM && selector != NVOECPTK && selector != NOISENUM && selector != OV2PSNUM && selector != PLYALNUM &&
                    selector != POSTPNUM && selector != RSDWMNUM && selector != SPEEDNUM && selector != TVECTNUM && selector != RECBFNUM &&
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
    tprintf(PRINT_STD, "\n");

    while (y < SelectorArInt)
    {
        if (PassFail[y] != -1)
        {
            PassFailReturn = 0;
            tprintf(PRINT_STD, "Line: %4i Test: %-25s - Not properly Formatted\n", PassFail[y], SelectorAr2[y].c_str());
            tprintf(PRINT_STD, "%s\n\n", SelectorAr[y].c_str());
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
            //delete [] PassFail;
            //delete [] StringAr;
            //delete [] SelectorAr;
            //delete [] SelectorAr2;
            //return 0;
            return -1;

        }
        else
        {
            if (PassFailInt == 0)
            {
                tprintf(PRINT_STD, "Test File Specified is empty.");
                //delete [] PassFail;
                //delete [] StringAr;
                //delete [] SelectorAr;
                //delete [] SelectorAr2;
                return 0;
            }
            else
            {

                tprintf(PRINT_STD, "\nAll %i Tests in text file: %s - are properly Formatted\n\n", y, input);

                //return 1;
                //delete [] PassFail;
                //delete [] StringAr;   //will cause error
                //delete [] SelectorAr; //will cause error
                //delete [] SelectorAr2;
                infile.close();
                return SelectorArInt;
            }
        }
    }
    else
    {

        if (PassFailReturn == 0)
        {
            tprintf(PRINT_STD, "\nFile Contains %i Tests\n", SelectorArInt);
            tprintf(PRINT_STD, "\nFile Contains %i Lines\n", trackthis1);
            infile.close();
            //delete [] PassFail;
            //delete [] StringAr;
            //delete [] SelectorAr;
            //delete [] SelectorAr2;
            //return 0;
            return -1;
        }
        else
        {
            tprintf(PRINT_STD, "\nAll %i Tests in text file: %s - are properly Formatted\n\n", y, input);
            tprintf(PRINT_STD, "\nFile Contains %i Tests\n", SelectorArInt);
            tprintf(PRINT_STD, "\nFile Contains %i Lines\n", trackthis1);
            infile.close();
            //return 1;
            //delete [] PassFail;
            //delete [] StringAr;
            //delete [] SelectorAr;
            //delete [] SelectorAr2;
            return SelectorArInt;
        }
    }

    infile.close();
    //return 3;
    //delete [] PassFail;
    //delete [] StringAr;
    //delete [] SelectorAr;
    //delete [] SelectorAr2;
    return -3;
}
int vpxt_file_exists_check(const std::string input)
{

    std::ifstream infile(input.c_str());

    if (infile)
    {
        infile.close();
        return 1;
    }

    infile.close();
    return 0;
}
int vpxt_folder_exist_check(const std::string FolderName)
{
#if defined(_WIN32)

    if (GetFileAttributes(FolderName.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        //tprintf(  PRINT_STD, "\nFolder: %s Does not exist\n", FolderName.c_str());
        return 0;
    }
    else
    {
        //tprintf(  PRINT_STD, "\nFolder: %s Does exist\n", FolderName.c_str());
        return 1;
    }

#else

    if (opendir(FolderName.c_str()) == NULL)
    {
        //tprintf(  PRINT_STD, "\nFolder: %s Does not exist\n", FolderName.c_str());
        return 0;
    }
    else
    {
        //tprintf(  PRINT_STD, "\nFolder: %s Does exist\n", FolderName.c_str());
        return 1;
    }

#endif
    return 0;
}
void vpxt_subfolder_name(const char *input, char *FileName)
{
    //extracts name of last two folders a target file is in and returns it
    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;

    while (input[parser] != '\0')
    {
        if (input[parser] == slashChar())
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

        if (input[parser] == slashChar())
        {
            slashcount2++;
        }

        parser++;
    }

    FileName[parser2] = '\0';

    return;
}
void vpxt_test_name(char *input, char *FileName)
{
    //Gets the directory test name from an input sting
    int parser = 0;
    int slashcount = 0;
    int slashcount2 = 0;
    char OpSlashChar;

    //find out what the default shash char is then define the oposite slash char
    if (slashChar() == '\\')
    {
        OpSlashChar = '/';
    }

    if (slashChar() == '/')
    {
        OpSlashChar = '\\';
    }

    //Parse through stirng replaceing any instance of an oposite slash char with a
    //correct slash char in order to run cross plat.
    while (input[parser] != '\0')
    {
        if (input[parser] == OpSlashChar)
        {
            input[parser] = slashChar();
        }

        parser++;
    }

    //continue with the function after proper initialization.
    parser = 0;

    while (input[parser] != '\0' && input[parser] != slashChar())
    {
        FileName[parser] = input[parser];
        parser++;
    }

    FileName[parser] = '\0';

    return;
}
int vpxt_remove_char_spaces(const char *input, char *output, int maxsize)
{
    int pos = 0;
    int offset = 0;

    while (input[pos] != '\0' && pos < maxsize)
    {
        if (input[pos] != ' ')
            output[pos-offset] = input[pos];
        else
            offset++;

        pos++;
    }

    output[pos-offset] = '\0';

    return 0;
}
//--------------------------------------------------------Math------------------------------------------------------------------------
int vpxt_decimal_places(int InputNumber)
{
    int y = 0;
    int totaltensplaces = 0;

    //find out how many decimal places
    while (y >= 0)
    {
        y = InputNumber - (int)pow(10.0, totaltensplaces);
        //tprintf(  PRINT_STD, "%i = %i - %f\n",y,total_number_of_frames,pow(10.0,totaltensplaces));
        totaltensplaces++;
    }

    //std::cout << "\n" << totaltensplaces << "\n";
    totaltensplaces = totaltensplaces - 1;
    //std::cout << "\n" << totaltensplaces << "\n";

    return totaltensplaces;
}
int vpxt_gcd(int a, int b)
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
int vpxt_abs_int(int input)
{
    if (input < 0)
    {
        input = input * -1;
    }

    return input;
}
float vpxt_abs_float(float input)
{
    if (input < 0.0)
    {
        input = input * -1.0;
    }

    return input;
}
double vpxt_abs_double(double input)
{
    if (input < 0)
    {
        input = input * -1.0;
    }

    return input;
}
int vpxt_solve_quadradic(float X1, float X2, float X3, float Y1, float Y2, float Y3, float &A, float &B, float &C)
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

    tprintf(  PRINT_STD, "Intput X1: ");
    cin >> X1;
    tprintf(  PRINT_STD, "Intput X2: ");
    cin >> X2;
    tprintf(  PRINT_STD, "Intput X3: ");
    cin >> X3;

    tprintf(  PRINT_STD, "Intput Y1: ");
    cin >> Y1;
    tprintf(  PRINT_STD, "Intput Y2: ");
    cin >> Y2;
    tprintf(  PRINT_STD, "Intput Y3: ");
    cin >> Y3;*/


    A = ((Y2 - Y1) * (X1 - X3) + (Y3 - Y1) * (X2 - X1)) / ((X1 - X3) * ((X2 * X2) - (X1 * X1)) + (X2 - X1) * ((X3 * X3) - (X1 * X1)));
    B = ((Y2 - Y1) - A * ((X2 * X2) - (X1 * X1))) / (X2 - X1);
    C = Y1 - A * (X1 * X1) - B * X1;

    //std::cout << "\nA = " << A;
    //std::cout << "\nB = " << B;
    //std::cout << "\nC = " << C << "\n";

    //tprintf(  PRINT_STD, "\n\ny = %.2fx^2 + %.2fx + %.2f\n\n",A,B,C);

    return 0;
}
float vpxt_area_under_quadradic(float A, float B, float C, float X1, float X2)
{
    float Area1 = ((A * X1 * X1 * X1) / 3) + ((B * X1 * X1) / 2) + C * X1;
    float Area2 = ((A * X2 * X2 * X2) / 3) + ((B * X2 * X2) / 2) + C * X2;
    float TotalArea = Area2 - Area1;
    return TotalArea;
}
char *vpxt_itoa_custom(int value, char *result, int base)
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

    std::reverse(result, out);
    *out = 0;

    return result;
}
//----------------------------------------------------Cross Plat----------------------------------------------------------------------
unsigned int vpxt_get_high_res_timer_tick()
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
unsigned int vpxt_get_time_in_micro_sec(unsigned int startTick, unsigned int stopTick)
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
unsigned int vpxt_get_cpu_tick()
{
#if defined(_WIN32)

    FILETIME CreationTime;
    FILETIME ExitTime;
    FILETIME KernelTime;
    FILETIME UserTime;

    LPSYSTEMTIME SystemTime;

    GetProcessTimes(GetCurrentProcess(), &CreationTime, &ExitTime, &KernelTime, &UserTime);

    //UserTime is meaured in groupings of 100 nano seconds, so 10^-9 * 100
    // = 10^-7 so to get to 10^-6 res divide by 10
    return UserTime.dwLowDateTime / 10;

#else
    return clock();
#endif
}
unsigned int vpxt_get_time()
{
    unsigned int Time = 0;
    Time = vpxt_get_high_res_timer_tick();
    return Time;
}
int vpxt_get_cur_dir(std::string &CurrentDir)
{
#if defined(_WIN32)
    TCHAR CurrentDirChar[MAX_PATH] = "";

    if (!::GetCurrentDirectory(sizeof(CurrentDirChar) - 1, CurrentDirChar))
    {
        tprintf(PRINT_BTH, "Could not get current directory");
        return 0;
    }

    CurrentDir = CurrentDirChar;
#else
    char CurrentDirChar[256];
    getcwd(CurrentDirChar, 256);
    CurrentDir = CurrentDirChar;
#endif
    return 0;
}
int vpxt_make_dir(std::string CreateDir)
{
#if defined(_WIN32)
    /////////////////////////////////////
    CreateDir.insert(0, "mkdir \"");
    CreateDir.append("\"");
    system(CreateDir.c_str());
    /////////////////////////////////////
#elif defined(linux)
    CreateDir.insert(0, "mkdir -p \"");
    CreateDir.append("\"");
    system(CreateDir.c_str());
#elif defined(__APPLE__)
    CreateDir.insert(0, "mkdir -p \"");
    CreateDir.append("\"");
    system(CreateDir.c_str());
#elif defined(__POWERPC__)
    CreateDir.insert(0, "mkdir -p \"");
    CreateDir.append("\"");
    system(CreateDir.c_str());
#endif
    return 0;
}
int vpxt_make_dir_vpx(std::string CreateDir2)
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

void vpxt_run_exe(std::string RunExe)
{
    tprintf(PRINT_STD, "\nAtempting to run: %s\n\n", RunExe.c_str());
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
int vpxt_add_dir_files_to_ignore(std::vector<std::string> &IgnoredFiles, std::string Directory)
{

#if defined(_WIN32)
    WIN32_FIND_DATA FileData;
    HANDLE hFind;
    std::string FileName;

    hFind = FindFirstFile(Directory.c_str(), &FileData);

    while (FileName.compare(FileData.cFileName) != 0)
    {
        FileName = FileData.cFileName;
        IgnoredFiles.push_back(FileName);
        FindNextFile(hFind, &FileData);
    }

#else
    //Record all files in the current directory.
    DIR *FileData;
    struct dirent *hFind;
    std::string FileName;

    FileData = opendir(Directory.c_str());
    hFind = readdir(FileData);

    while (FileName.compare(hFind->d_name) != 0)
    {
        FileName = hFind->d_name;
        IgnoredFiles.push_back(FileName);

        hFind = readdir(FileData);

        if (hFind == NULL)
        {
            break;
        }
    }

    closedir(FileData);
#endif

    return 0;
}
int vpxt_find_non_ignored_files_in_dir(std::vector<std::string> IgnoredFiles, std::vector<std::string> &FilesFound, std::string Directory)
{
    //Function returns the number of non ignored files found on success
    //-1 on error

#if defined(_WIN32)
    int Fail = 0;
    std::string FileName;
    WIN32_FIND_DATA FileData;
    HANDLE hFind;

    hFind = FindFirstFile(Directory.c_str(), &FileData);

    while (FileName.compare(FileData.cFileName) != 0)
    {
        FileName = FileData.cFileName;

        int CurVecPos = 0;
        int IgnoreFile = 0;

        while (CurVecPos < IgnoredFiles.size())
        {
            if (IgnoredFiles[CurVecPos].compare(FileName.c_str()) == 0)
            {
                IgnoreFile = 1;
            }

            CurVecPos++;
        }

        if (IgnoreFile == 0)
        {
            FilesFound.push_back(FileName);
        }

        FindNextFile(hFind, &FileData);
    }

#else

    DIR *FileData;
    struct dirent *hFind;

    FileData = opendir(Directory.c_str());
    hFind = readdir(FileData);

    std::string FileName;
    std::vector<std::string> DestFileExtraVector;

    while (FileName.compare(hFind->d_name) != 0)
    {
        FileName = hFind->d_name;

        int CurVecPos = 0;
        int IgnoreFile = 0;

        while (CurVecPos < IgnoredFiles.size())
        {
            if (IgnoredFiles[CurVecPos].compare(FileName.c_str()) == 0)
            {
                IgnoreFile = 1;
            }

            CurVecPos++;
        }

        if (IgnoreFile == 0)
        {
            FilesFound.push_back(FileName);
        }

        hFind = readdir(FileData);

        if (hFind == NULL)
        {
            break;
        }
    }

    closedir(FileData);

#endif

    return FilesFound.size();
}
//---------------------------------------------------------IVF------------------------------------------------------------------------
int image2yuvconfig(const vpx_image_t   *img, YV12_BUFFER_CONFIG  *yv12)
{
    //vpx_codec_err_t        res = ON2_CODEC_OK;
    yv12->buffer_alloc = img->planes[PLANE_Y];
    yv12->y_buffer = img->planes[PLANE_Y];
    yv12->u_buffer = img->planes[PLANE_U];
    yv12->v_buffer = img->planes[PLANE_V];

    yv12->y_width  = img->d_w;
    yv12->y_height = img->d_h;
    yv12->uv_width = (1 + yv12->y_width) / 2;
    yv12->uv_height = (1 + yv12->y_height) / 2;

    yv12->y_stride = img->stride[PLANE_Y];
    yv12->uv_stride = img->stride[PLANE_U];

    yv12->border  = (img->stride[PLANE_Y] - img->w) / 2;

    //int yplane_size = (img->d_h + 2 * yv12->border) * (img->d_w + 2 * yv12->border);
    //int uvplane_size = ((1 + img->d_h) / 2 + yv12->border) * ((1 + img->d_w) / 2 + yv12->border);
    //yv12->frame_size = yplane_size + 2 * uvplane_size;

    //  yv12->clrtype = (/*img->fmt == IMG_FMT_ON2I420 || img->fmt == */IMG_FMT_ON2YV12); //REG_YUV = 0
    return 0;
}
double vpxt_ivf_psnr(const char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, double *SsimOut)
{
    if (frameStats != 3)
    {
        frameStats = 1;//Overide to print individual frames to screen
    }

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
        tprintf(PRINT_BTH, "\nError Opening Raw File: %s\n", inputFile1);
        return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    int buffer_sz = 32;
    unsigned int frameCount = ivfhRaw.length;


    vpx_image_t    raw_img;
    vpx_img_alloc(&raw_img, IMG_FMT_I420, ivfhRaw.width, ivfhRaw.height, 1);

    YV12_BUFFER_CONFIG Raw_YV12;
    Raw_YV12.y_width   = raw_img.d_w;
    Raw_YV12.y_height  = raw_img.d_h;
    Raw_YV12.y_stride  = raw_img.stride[PLANE_Y];
    Raw_YV12.uv_width  = (1 + Raw_YV12.y_width) / 2;
    Raw_YV12.uv_height = (1 + Raw_YV12.y_height) / 2;
    Raw_YV12.uv_stride = raw_img.stride[PLANE_U];
    Raw_YV12.buffer_alloc        = raw_img.img_data;
    Raw_YV12.y_buffer           = raw_img.img_data;
    Raw_YV12.u_buffer = raw_img.planes[PLANE_U];
    Raw_YV12.v_buffer = raw_img.planes[PLANE_V];

    if (RawFrameOffset > 0) //Burn Frames untill Raw frame offset reached - currently disabled by override of RawFrameOffset
    {
        for (int i = 0; i < RawFrameOffset; i++)
        {
        }
    }

    //I420 hex-0x30323449 dec-808596553
    //YV12 hex-0x32315659 dec-842094169

    if (ivfhRaw.four_cc == 842094169)
    {
        forceUVswap = 1;   //if YV12 Do not swap Frames
    }

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.u_buffer;
        Raw_YV12.u_buffer = Raw_YV12.v_buffer;
        Raw_YV12.v_buffer = temp;
    }

    ////////////////////////Initilize Compressed File////////////////////////
    FILE *CompFile = fopen(inputFile2, "rb");

    if (CompFile == NULL)
    {
        tprintf(PRINT_BTH, "\nError Opening Compressed File: %s\n", inputFile2);
        vpx_img_free(&raw_img);
        fclose(RawFile);
        return 0;
    }

    IVF_HEADER ivfhComp;
    InitIVFHeader(&ivfhComp);
    fread(&ivfhComp, 1, sizeof(ivfhComp), CompFile);
    vpxt_format_ivf_header_read(&ivfhComp);

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
        tprintf(PRINT_BTH, "\n\n                        ---------Computing PSNR---------");
    }

    if (printvar == 0)
    {
        tprintf(PRINT_STD, "\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 1)
    {
        tprintf(PRINT_BTH, "\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 5)
    {
        tprintf(PRINT_BTH, "\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            tprintf(PRINT_STD, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
        else
        {
            tprintf(PRINT_BTH, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
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
    vpx_codec_ctx_t       decoder;
    vpx_codec_iface_t       *iface = NULL;
    vpx_codec_iface_t  *ivf_iface = ifaces[0].iface;
    vpx_codec_dec_cfg_t     cfg = {0};
    iface = ivf_iface;

    vp8_postproc_cfg_t ppcfg = {0};

    ppcfg.deblocking_level = deblock_level2;
    ppcfg.noise_level = noise_level2;
    ppcfg.post_proc_flag = flags2;

    if (vpx_codec_dec_init(&decoder, ifaces[0].iface, &cfg, 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(RawFile);
        fclose(CompFile);
        delete timeStamp2;
        delete timeEndStamp2;
        vpx_img_free(&raw_img);
        return EXIT_FAILURE;
    }

    /////////////////////Setup Temp YV12 Buffer to Resize if nessicary/////////////////////
    vp8_scale_machine_specific_config();

    YV12_BUFFER_CONFIG Temp_YV12;
    memset(&Temp_YV12, 0, sizeof(Temp_YV12));
    vp8_yv12_alloc_frame_buffer(&Temp_YV12, ivfhRaw.width, ivfhRaw.height, 0);
    ///////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_control(&decoder, VP8_SET_POSTPROC, &ppcfg);
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
            tprintf(PRINT_BTH, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
            vpx_img_free(&raw_img);
            return 0;
        }

        vpxt_format_frame_header_read(ivf_fhComp);

        bytes2 = ivf_fhComp.frameSize;
        sumBytes2 += bytes2;

        char *CompBuff = new char[ivf_fhComp.frameSize*2];

        if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        {
            tprintf(PRINT_BTH, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] CompBuff;
            vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
            vpx_img_free(&raw_img);
            return 0;
        }

        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;

        if (vpx_codec_decode(&decoder, (uint8_t *) CompBuff, ivf_fhComp.frameSize, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));
        }

        if (img = vpx_codec_get_frame(&decoder, &iter))
        {

            image2yuvconfig(img, &Comp_YV12);

            if (img->d_w != ivfhRaw.width || img->d_h != ivfhRaw.height) //if frame not correct size resize it for psnr
            {
                int GCDInt1 = vpxt_gcd(img->d_w, ivfhRaw.width);
                int GCDInt2 = vpxt_gcd(img->d_h, ivfhRaw.height);

                vp8_yv12_scale_or_center(&Comp_YV12, &Temp_YV12, ivfhRaw.width, ivfhRaw.height, 0, (ivfhRaw.width / GCDInt1), (img->d_w / GCDInt1), (ivfhRaw.height / GCDInt2), (img->d_h / GCDInt2));

                Comp_YV12 = Temp_YV12;
            }

            delete [] CompBuff;
            /////////////////////////////////////////////////////////////////////////////

            //////////////////////Get YV12 Data For Raw File//////////////////////
            IVF_FRAME_HEADER ivf_fhRaw;

            if (!fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), RawFile))
            {
                tprintf(PRINT_BTH, "\nError Computing PSNR\n");
                fclose(RawFile);
                fclose(CompFile);
                delete timeStamp2;
                delete timeEndStamp2;
                vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
                vpx_img_free(&raw_img);
                return 0;
            }

            vpxt_format_frame_header_read(ivf_fhRaw);

            bytes1 = ivf_fhRaw.frameSize;
            sumBytes += bytes1;

            read_frame_enc(RawFile, &raw_img, ivf_fhRaw.frameSize);

            //////////////////////////////////////////////////////////////////////

            ///////////////////////////Preform PSNR Calc///////////////////////////////////
            if (SsimOut)
            {
                double weight;
                double thisSsim = VP8_CalcSSIM_Tester(&Raw_YV12, &Comp_YV12, 1, &weight);
                summedQuality += thisSsim * weight ;
                summedWeights += weight;
            }

            double YPsnr;
            double UPsnr;
            double VPsnr;
            double SqError;
            double thisPsnr = VP8_CalcPSNR_Tester(&Raw_YV12, &Comp_YV12, &YPsnr, &UPsnr, &VPsnr, &SqError);

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
                    tprintf(PRINT_STD, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                            currentVideo1Frame, frameCount
                           );
                }

                if (frameStats == 1)
                {
                    tprintf(PRINT_BTH, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                            currentVideo1Frame,
                            bytes1 * 8.0,
                            bytes2 * 8.0,
                            thisPsnr, 1.0 * YPsnr ,
                            1.0 * UPsnr ,
                            1.0 * VPsnr);
                }

                if (frameStats == 2)
                {
                    tprintf(PRINT_STD, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
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
                    tprintf(PRINT_STD, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
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

    vpx_codec_destroy(&decoder);

    //Over All PSNR Calc
    double samples = 3.0 / 2 * frameCount * Raw_YV12.y_width * Raw_YV12.y_height;
    double avgPsnr = summedPsnr / frameCount;
    double totalPsnr = VP8_Mse2Psnr_Tester(samples, 255.0, sumSqError);

    if (summedWeights < 1.0)
        summedWeights = 1.0;

    double totalSSim = 100 * pow(summedQuality / summedWeights, 8.0);

    ////////Printing////////
    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            tprintf(PRINT_STD, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, ",
                    sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,                   //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,               //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr);
            tprintf(PRINT_STD, SsimOut ? "SSIM: %8.2f\n" : "SSIM: Not run.", totalSSim);

        }
        else
        {
            tprintf(PRINT_BTH, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, ",
                    sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,           //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,       //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr);
            tprintf(PRINT_BTH, SsimOut ? "SSIM: %8.2f\n" : "SSIM: Not run.", totalSSim);
        }
    }

    if (printvar != 0)
    {
        tprintf(PRINT_BTH, "\n                        --------------------------------\n");
    }

    if (SsimOut)
        *SsimOut = totalSSim;

    ////////////////////////
    fclose(RawFile);
    fclose(CompFile);
    delete timeStamp2;
    delete timeEndStamp2;
    vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
    vpx_img_free(&raw_img);

    return totalPsnr;
}
double vpxt_post_proc_ivf_psnr(const char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double *SsimOut)
{
    if (frameStats != 3)
    {
        frameStats = 1;//Overide to print individual frames to screen
    }

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
        tprintf(PRINT_BTH, "\nError Opening Raw File: %s\n", inputFile1);
        return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    int buffer_sz = 32;
    unsigned int frameCount = ivfhRaw.length;


    vpx_image_t    raw_img;
    vpx_img_alloc(&raw_img, IMG_FMT_I420, ivfhRaw.width, ivfhRaw.height, 1);

    YV12_BUFFER_CONFIG Raw_YV12;
    Raw_YV12.y_width   = raw_img.d_w;
    Raw_YV12.y_height  = raw_img.d_h;
    Raw_YV12.y_stride  = raw_img.stride[PLANE_Y];
    Raw_YV12.uv_width  = (1 + Raw_YV12.y_width) / 2;
    Raw_YV12.uv_height = (1 + Raw_YV12.y_height) / 2;
    Raw_YV12.uv_stride = raw_img.stride[PLANE_U];
    Raw_YV12.buffer_alloc        = raw_img.img_data;
    Raw_YV12.y_buffer           = raw_img.img_data;
    Raw_YV12.u_buffer = raw_img.planes[PLANE_U];
    Raw_YV12.v_buffer = raw_img.planes[PLANE_V];

    if (RawFrameOffset > 0) //Burn Frames untill Raw frame offset reached - currently disabled by override of RawFrameOffset
    {
        for (int i = 0; i < RawFrameOffset; i++)
        {
        }
    }

    //I420 hex-0x30323449 dec-808596553
    //YV12 hex-0x32315659 dec-842094169

    if (ivfhRaw.four_cc == 842094169)
    {
        forceUVswap = 1;   //if YV12 Do not swap Frames
    }

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.u_buffer;
        Raw_YV12.u_buffer = Raw_YV12.v_buffer;
        Raw_YV12.v_buffer = temp;
    }

    ////////////////////////Initilize Compressed File////////////////////////
    FILE *CompFile = fopen(inputFile2, "rb");

    if (CompFile == NULL)
    {
        tprintf(PRINT_BTH, "\nError Opening Compressed File: %s\n", inputFile2);
        vpx_img_free(&raw_img);
        fclose(RawFile);
        return 0;
    }

    IVF_HEADER ivfhComp;
    InitIVFHeader(&ivfhComp);
    fread(&ivfhComp, 1, sizeof(ivfhComp), CompFile);
    vpxt_format_ivf_header_read(&ivfhComp);

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
        tprintf(PRINT_BTH, "\n\n                        ---------Computing PSNR---------");
    }

    if (printvar == 0)
    {
        tprintf(PRINT_STD, "\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 1)
    {
        tprintf(PRINT_BTH, "\n\nComparing %s to %s:\n                        \n", inputFile1, inputFile2);
    }

    if (printvar == 5)
    {
        tprintf(PRINT_BTH, "\n\nComparing %s to %s:                        \n\n", inputFile1, inputFile2);
    }

    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            tprintf(PRINT_STD, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
        }
        else
        {
            tprintf(PRINT_BTH, "File Has: %d total frames. \n Frame Offset 1 is 0\n Frame Offset 2 is 0\n Force UV Swap is %d\n Frame Statistics is %d:\n \n", frameCount, forceUVswap, frameStats);
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
    vpx_codec_ctx_t       decoder;
    vpx_codec_iface_t       *iface = NULL;
    vpx_codec_iface_t  *ivf_iface = ifaces[0].iface;
    vpx_codec_dec_cfg_t     cfg = {0};
    iface = ivf_iface;

    vp8_postproc_cfg_t ppcfg = {0};

    //if(noise_level != 0)
    //{
    //  ppcfg.post_proc_flag |= VP8_ADDNOISE;
    //  ppcfg.noise_level = noise_level;

    //}
    //if(deblock_level != 0)
    //{
    //  ppcfg.post_proc_flag |= VP8_DEMACROBLOCK;
    //  ppcfg.deblocking_level = deblock_level;

    //}

    //ppcfg.post_proc_flag |= VP8_DEBLOCK;
    //ppcfg.post_proc_flag = flags;

    ppcfg.post_proc_flag = flags;
    ppcfg.noise_level = noise_level;
    ppcfg.deblocking_level = deblock_level;

    if (vpx_codec_dec_init(&decoder, ifaces[0].iface, &cfg, VPX_CODEC_USE_POSTPROC))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(RawFile);
        fclose(CompFile);
        delete timeStamp2;
        delete timeEndStamp2;
        vpx_img_free(&raw_img);
        return EXIT_FAILURE;
    }

    if (vpx_codec_control(&decoder, VP8_SET_POSTPROC, &ppcfg) != 0)
    {
        tprintf(PRINT_STD, "Failed to update decoder post processor settings\n");
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
            tprintf(PRINT_BTH, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
            vpx_img_free(&raw_img);
            return 0;
        }

        vpxt_format_frame_header_read(ivf_fhComp);

        bytes2 = ivf_fhComp.frameSize;
        sumBytes2 += bytes2;

        char *CompBuff = new char[ivf_fhComp.frameSize * 2];

        if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        {
            tprintf(PRINT_BTH, "\nError Computing PSNR\n");
            fclose(RawFile);
            fclose(CompFile);
            delete timeStamp2;
            delete timeEndStamp2;
            delete [] CompBuff;
            vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
            vpx_img_free(&raw_img);
            return 0;
        }

        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;

        if (vpx_codec_decode(&decoder, (uint8_t *) CompBuff, ivf_fhComp.frameSize, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));
        }

        if (img = vpx_codec_get_frame(&decoder, &iter))
        {

            image2yuvconfig(img, &Comp_YV12);

            if (img->d_w != ivfhRaw.width || img->d_h != ivfhRaw.height) //if frame not correct size resize it for psnr
            {
                int GCDInt1 = vpxt_gcd(img->d_w, ivfhRaw.width);
                int GCDInt2 = vpxt_gcd(img->d_h, ivfhRaw.height);

                vp8_yv12_scale_or_center(&Comp_YV12, &Temp_YV12, ivfhRaw.width, ivfhRaw.height, 0, (ivfhRaw.width / GCDInt1), (img->d_w / GCDInt1), (ivfhRaw.height / GCDInt2), (img->d_h / GCDInt2));

                Comp_YV12 = Temp_YV12;
            }

            delete [] CompBuff;
            /////////////////////////////////////////////////////////////////////////////

            //////////////////////Get YV12 Data For Raw File//////////////////////
            IVF_FRAME_HEADER ivf_fhRaw;

            if (!fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), RawFile))
            {
                tprintf(PRINT_BTH, "\nError Computing PSNR\n");
                fclose(RawFile);
                fclose(CompFile);
                delete timeStamp2;
                delete timeEndStamp2;
                vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
                vpx_img_free(&raw_img);
                return 0;
            }

            vpxt_format_frame_header_read(ivf_fhRaw);

            bytes1 = ivf_fhRaw.frameSize;
            sumBytes += bytes1;






            read_frame_enc(RawFile, &raw_img, ivf_fhRaw.frameSize);

            //////////////////////////////////////////////////////////////////////

            ///////////////////////////Preform PSNR Calc///////////////////////////////////
            if (SsimOut)
            {
                double weight;
                double thisSsim = VP8_CalcSSIM_Tester(&Raw_YV12, &Comp_YV12, 1, &weight);
                summedQuality += thisSsim * weight ;
                summedWeights += weight;
            }

            double YPsnr;
            double UPsnr;
            double VPsnr;
            double SqError;
            double thisPsnr = VP8_CalcPSNR_Tester(&Raw_YV12, &Comp_YV12, &YPsnr, &UPsnr, &VPsnr, &SqError);

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
                    tprintf(PRINT_STD, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
                            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                            currentVideo1Frame, frameCount
                           );
                }

                if (frameStats == 1)
                {
                    tprintf(PRINT_BTH, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
                            currentVideo1Frame,
                            bytes1 * 8.0,
                            bytes2 * 8.0,
                            thisPsnr, 1.0 * YPsnr ,
                            1.0 * UPsnr ,
                            1.0 * VPsnr);
                }

                if (frameStats == 2)
                {
                    tprintf(PRINT_STD, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%7d of %7d  ",
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
                    tprintf(PRINT_STD, "F:%5d, 1:%6.0f 2:%6.0f, Avg :%5.2f, Y:%5.2f, U:%5.2f, V:%5.2f\n",
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

    vpx_codec_destroy(&decoder);

    //Over All PSNR Calc
    double samples = 3.0 / 2 * frameCount * Raw_YV12.y_width * Raw_YV12.y_height;
    double avgPsnr = summedPsnr / frameCount;
    double totalPsnr = VP8_Mse2Psnr_Tester(samples, 255.0, sumSqError);

    if (summedWeights < 1.0)
        summedWeights = 1.0;

    double totalSSim = 100 * pow(summedQuality / summedWeights, 8.0);

    ////////Printing////////
    if (printvar == 1 || printvar == 5 || printvar == 0)
    {
        if (frameStats == 3)
        {
            tprintf(PRINT_STD, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, ",
                    sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,                   //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,               //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr);
            tprintf(PRINT_STD, SsimOut ? "SSIM: %8.2f\n" : "SSIM: Not run.", totalSSim);

        }
        else
        {
            tprintf(PRINT_BTH, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, ",
                    sumBytes * 8.0 / ivfhRaw.length*(ivfhRaw.rate / 2) / ivfhRaw.scale / 1000,           //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / ivfhComp.length*(ivfhComp.rate / 2) / ivfhComp.scale / 1000,       //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr, 1.0 * summedYPsnr / frameCount,
                    1.0 * summedUPsnr / frameCount, 1.0 * summedVPsnr / frameCount,
                    totalPsnr);
            tprintf(PRINT_BTH, SsimOut ? "SSIM: %8.2f\n" : "SSIM: Not run.", totalSSim);
        }
    }

    if (printvar != 0)
    {
        tprintf(PRINT_BTH, "\n                        --------------------------------\n");
    }

    if (SsimOut)
        *SsimOut = totalSSim;

    ////////////////////////
    fclose(RawFile);
    fclose(CompFile);
    delete timeStamp2;
    delete timeEndStamp2;
    vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
    vpx_img_free(&raw_img);

    return totalPsnr;
}
double vpxt_ivf_data_rate(const char *inputFile, int DROuputSel)
{
    if (DROuputSel != 2)
    {
        tprintf(PRINT_BTH, "\n--------Data Rate-------\n");

        char FileNameOnly[256];

        vpxt_file_name(inputFile, FileNameOnly, 0);
        tprintf(PRINT_BTH, "Data Rate for: %s", FileNameOnly);
    }

    ///////////////////////////////////
    long PosSize = vpxt_file_size(inputFile, 0);
    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nCan not open Data Rate Input file");
        return 0;
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);
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
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
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
    std::cout << "\n";

    while (currentVideoFrame < frameCount)
    {
        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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

    tprintf(PRINT_STD, "\nData rate for frames 0..%i\n", frameCount - 1);
    tprintf(PRINT_STD, "Average %*.2f kb/s\n", 10, Avg);
    tprintf(PRINT_STD, "Min     %*.2f kb/s\n", 10, Min);
    tprintf(PRINT_STD, "Max     %*.2f kb/s\n", 10, Max);
    tprintf(PRINT_STD, "File    %*.2f kb/s\n", 10, File);

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
        tprintf(PRINT_BTH, "\n------------------------\n");
    }

    delete [] outputVideoBuffer;

    return Avg;
}

int vpxt_ivf_check_pbm(const char *inputFile, int bitRate, int maxBuffer, int preBuffer)
{
    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);

    IVF_FRAME_HEADER ivf_fhRaw;

    frameCount = ivfhRaw.length;
    int nFrameFail = 0;

    bool checkOverrun = false;
    double secondsperframe = ((double)ivfhRaw.scale / (double)ivfhRaw.rate) * 2;//(the *2 is due to scale being doubled for extra frame padding)
    int bitsAddedPerFrame = ((bitRate * 1000 * secondsperframe)) - .5; //-.5 to cancel out rounding
    int bitsInBuffer = preBuffer * bitRate;
    int maxBitsInBuffer = maxBuffer * bitRate;

    fpos_t position;
    fgetpos(in, &position);

    while (currentVideoFrame < frameCount)
    {
        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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

int vpxt_ivf_check_pbm_threshold(const char *inputFile, double bitRate, int maxBuffer, int preBuffer, int optimalbuffer, int Threshold)
{
    std::string ResizeInStr = inputFile;
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
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);

    IVF_FRAME_HEADER ivf_fhRaw;

    frameCount = ivfhRaw.length;
    int nFrameFail = 0;

    bool checkOverrun = false;
    double secondsperframe = ((double)ivfhRaw.scale / (double)ivfhRaw.rate) * 2;//(the *2 is due to scale being doubled for extra frame padding)
    int bitsAddedPerFrame = ((bitRate * 1000 * secondsperframe)) - .5; //-.5 to cancel out rounding
    int bitsInBuffer = preBuffer * bitRate;
    int maxBitsInBuffer = maxBuffer * bitRate;

    fpos_t position;
    fgetpos(in, &position);

    while (currentVideoFrame < frameCount)
    {
        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        //fprintf(out, "Frame: %4i Bits in Buffer %i = bitsAddedPerFrame %i - frame size %i     - ", currentVideoFrame, bitsInBuffer, bitsAddedPerFrame, ivf_fhRaw.frameSize * 8);

        bitsInBuffer += bitsAddedPerFrame;
        bitsInBuffer -= ivf_fhRaw.frameSize * 8;

        int optimalbufferFloat = optimalbuffer;
        double MaxPercentBuffer = (((Threshold * optimalbufferFloat) / 100) * bitRate);

        if (bitsInBuffer < MaxPercentBuffer)
        {
            fprintf(out, "%i %i\n", currentVideoFrame, 1);
        }
        else
        {
            fprintf(out, "%i %i\n", currentVideoFrame, 0);
        }

        if (bitsInBuffer > maxBitsInBuffer)
        {
            if (checkOverrun)
            {
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

    if (PrintSwitch == 1)
    {
        fclose(out);
    }

    return -11;
}

int vpxt_faux_compress()
{
#ifdef API
    vpx_codec_ctx_t        encoder;
    vpx_codec_iface_t     *iface = &vpx_enc_vp8_algo;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    vpx_codec_enc_init(&encoder, iface, &cfg, 0);
    vpx_codec_destroy(&encoder);
#else
    VP8_CONFIG opt;
    vpxt_default_parameters(opt);
    VP8_Initialize();
    VP8_PTR optr = VP8_CreateCompressor(&opt);
    VP8_RemoveCompressor(&optr);
    VP8_Shutdown();
#endif

    return 1;
}
int vpxt_faux_decompress(const char *inputChar)
{
#ifdef API

    vpx_dec_ctx_t          decoder;
    vpx_codec_iface_t     *iface = &vpx_codec_vp8_dx_algo;
    vpx_codec_dec_cfg_t     cfg;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    int                    width, height;
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
            ivfhRaw.four_cc = 808596553;
            width = ivfhRaw.width;
            height = ivfhRaw.height;
            ///////////////////////////////////////////////////////
        }
    }

    vpx_codec_dec_init(&decoder, iface, &cfg, 0);
    read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf);
    vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0);
    vpx_codec_destroy(&decoder);
    free(buf);
    fclose(infile);

#else
    vp8dx_initialize();
    VP8D_CONFIG oxcf;
    VP8D_PTR optr = vp8dx_create_decompressor(&oxcf);
    vp8dx_remove_decompressor(optr);
    //vp8dx_Shutdown();
#endif

    return 1;
}
//---------------------------------------------------Test Functions-------------------------------------------------------------------
int initialize_test_directory(int argc, const char *const *argv, int TestType, const std::string &WorkingDir, const char *MyDir, std::string &CurTestDirStr, std::string &FileIndexStr, char MainTestDirChar[255], char FileIndexOutputChar[255], std::string FilesAr[])
{
    //Initilizes CurTestDirStr, FileIndexStr, MainTestDirChar, and FileIndexOutputChar to proper values.

    std::string PrefTestOnlyTestMatch = "";
    char CurTestDirChar[255] = "";

    if (TestType == 2 || TestType == 1)
    {
        snprintf(CurTestDirChar, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (CurTestDirChar[v] != '\"')
        {
            MainTestDirChar[v] = CurTestDirChar[v];
            v++;
        }

        MainTestDirChar[v] = slashChar();
        MainTestDirChar[v+1] = '\0';
        CurTestDirStr = MainTestDirChar;
        /////////////////////////////////////////////////////////////////////////////////
        FileIndexStr = MainTestDirChar;
        FileIndexStr.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        CurTestDirStr.append(MyDir);
        CurTestDirStr.append(slashCharStr());
        CurTestDirStr.append(FilesAr[0]);
        CurTestDirStr.erase(CurTestDirStr.length() - 1, 1);

        std::string CreateDir2 = CurTestDirStr;
        CreateDir2.insert(0, "md \"");
        vpxt_make_dir_vpx(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char CurTestDirStr2[255];
            snprintf(CurTestDirStr2, 255, "%s", CurTestDirStr.c_str());
            vpxt_subfolder_name(CurTestDirStr2, FileIndexOutputChar);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting CurTestDirStr to the location of the files we want to examine.
        char buffer[255];

        std::string CurTestDirChar = WorkingDir;

        CurTestDirChar.append(slashCharStr());
        FileIndexStr = CurTestDirChar;
        FileIndexStr.append("FileIndex.txt");

        std::fstream FileStream;
        FileStream.open(FileIndexStr.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char PrefTestOnlyTestMatchChar[255];
        vpxt_test_name(buffer, PrefTestOnlyTestMatchChar);
        PrefTestOnlyTestMatch = PrefTestOnlyTestMatchChar;

        if (PrefTestOnlyTestMatch.compare(MyDir) != 0)
        {
            tprintf(PRINT_STD, "Error: File mismatch ");
            tprintf(PRINT_STD, "PrefTestOnlyTestMatch: %s MyDir: %s", PrefTestOnlyTestMatch.c_str(), MyDir);
            return 11;
        }

        CurTestDirChar.append(buffer);
        CurTestDirStr = CurTestDirChar;
    }

    return 0;
}
void record_test_complete(const std::string MainDirString, const char *FileIndexOutputChar, int TestType)
{
    if (TestType == 2)
    {
        std::fstream FileStream;
        FileStream.open(MainDirString.c_str(), std::fstream::out | std::fstream::app);

        if (!FileStream.good())
        {
            tprintf(PRINT_STD, "\nERROR WRITING FILE: FILEINDEX: %s\n", MainDirString.c_str());
            return;
        }

        FileStream << FileIndexOutputChar << "\n";
        FileStream.close();
    }

    return;
}
int print_version()
{
    std::string arch = "Unknown";
    std::string Platform = "Unknown";

#if ARCH_X86
    arch = "32 bit";
#elif ARCH_X86_64
    arch = "64 bit";
#elif HAVE_ARMV6
    arch = "Arm 6";
#elif HAVE_ARMV7
    arch = "Arm 7";
#endif

#if defined(_WIN32)
    Platform = "Windows";
#elif defined(linux)
    Platform = "Linux";
#elif defined(__APPLE__)
    Platform = "Intel Mac";
#elif defined(__POWERPC__)
    Platform = "PowerPC";
#endif

    tprintf(PRINT_STD, "\n %s - %s %s\n", vpx_codec_iface_name(&vpx_codec_vp8_cx_algo), arch.c_str(), Platform.c_str());
    return 0;
}
void print_header_info()
{
    std::string TestMachineInfo = "                 Test Machine is Running: Unknown Platform\n\n";
    std::string arch = "Unknown";
#if ARCH_X86
    arch = "32 bit";
#else if ARCH_X86_64
    arch = "64 bit";
#endif

#if defined(_WIN32)
    TestMachineInfo = "";
    std::string CodecNameStr = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    int x = 0;

    while (x < 40 - (CodecNameStr.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n");
    std::string Platform = "";
    Platform.append("Test Machine is Running: ");
    Platform.append(arch.c_str());
    Platform.append(" Windows");

    x = 0;

    while (x < 40 - (Platform.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(Platform.c_str());
    TestMachineInfo.append("\n\n");

#endif
#if defined(linux)
    TestMachineInfo = "";
    std::string CodecNameStr = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    int x = 0;

    while (x < 40 - (CodecNameStr.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n");
    std::string Platform = "";
    Platform.append("Test Machine is Running: ");
    Platform.append(arch.c_str());
    Platform.append(" Linux");

    x = 0;

    while (x < 40 - (Platform.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(Platform.c_str());
    TestMachineInfo.append("\n\n");
#endif
#if defined(__APPLE__)
    TestMachineInfo = "";
    std::string CodecNameStr = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    int x = 0;

    while (x < 40 - (CodecNameStr.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n");
    std::string Platform = "";
    Platform.append("Test Machine is Running: ");
    Platform.append(arch.c_str());
    Platform.append(" Intel Mac");

    x = 0;

    while (x < 40 - (Platform.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(Platform.c_str());
    TestMachineInfo.append("\n\n");
#endif
#if defined(__POWERPC__)
    TestMachineInfo = "";
    std::string CodecNameStr = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    int x = 0;

    while (x < 40 - (CodecNameStr.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n");
    std::string Platform = "";
    Platform.append("Test Machine is Running: ");
    Platform.append(arch.c_str());
    Platform.append(" PowerPC");

    x = 0;

    while (x < 40 - (Platform.length() / 2))
    {
        TestMachineInfo.append(" ");
        x++;
    }

    TestMachineInfo.append(Platform.c_str());
    TestMachineInfo.append("\n\n");
#endif

    tprintf(PRINT_BTH, "%s", TestMachineInfo.c_str());
}
void print_header_full_test(int argc, const char *const *argv, std::string WorkingDir3)
{
    //Full Test Header Output
    //Formats workingDir3 string to fit in text box
    //records settings from argv to be written to text file
    std::string PrintWorkingDir3 = WorkingDir3;
    std::string PrintInput = "Input:";
    PrintWorkingDir3.insert(0, "Output: ");

    int y = 0;

    while (y < argc)
    {
        PrintInput.append(" ");
        PrintInput.append(argv[y]);
        y++;
    }

    y = 75;

    while (y < PrintInput.length())
    {
        PrintInput.insert(y, "\n");
        y = y + 75;
    }

    y = 75;

    while (y < PrintWorkingDir3.length())
    {
        PrintWorkingDir3.insert(y, "\n");
        y = y + 75;
    }

    print_header_info();

    tprintf(PRINT_BTH, "\n/////////////////////////////////Full Test/////////////////////////////////\n%s\n\n%s", PrintInput.c_str(), PrintWorkingDir3.c_str());
    tprintf(PRINT_BTH, "\n///////////////////////////////////////////////////////////////////////////\n\n");
}
void print_header_compression_only(int argc, const char *const *argv, std::string WorkingDir3)
{
    //Compression Header
    //Formats workingDir3 string to fit in text box
    //records settings from argv to be written to text file

    std::string PrintWorkingDir3 = WorkingDir3;
    std::string PrintInput = "Input:";
    PrintWorkingDir3.insert(0, "Output: ");

    int y = 0;

    while (y < argc)
    {
        PrintInput.append(" ");
        PrintInput.append(argv[y]);
        y++;
    }

    y = 75;

    while (y < PrintInput.length())
    {
        PrintInput.insert(y, "\n");
        y = y + 75;
    }

    y = 75;

    while (y < PrintWorkingDir3.length())
    {
        PrintWorkingDir3.insert(y, "\n");
        y = y + 75;
    }

    print_header_info();

    tprintf(PRINT_BTH, "\n///////////////////////////////Compress Only///////////////////////////////\n%s\n\n%s", PrintInput.c_str(), PrintWorkingDir3.c_str());
    tprintf(PRINT_BTH, "\n///////////////////////////////////////////////////////////////////////////\n\n");
}
void print_header_test_only(int argc, const char *const *argv, std::string  WorkingDir3)
{
    //Test Only Header
    //Formats workingDir3 string to fit in text box records input
    //location and output location both are the same

    std::string PrintWorkingDir3 = WorkingDir3;
    std::string PrintWorkingDir4 = WorkingDir3;
    std::string PrintInput = "Input:";
    PrintWorkingDir3.insert(0, "Output: ");
    PrintWorkingDir4.insert(0, "Input: ");

    int y = 0;

    while (y < argc)
    {
        PrintInput.append(" ");
        PrintInput.append(argv[y]);
        y++;
    }

    y = 75;

    while (y < PrintWorkingDir4.length())
    {
        PrintWorkingDir4.insert(y, "\n");
        y = y + 75;
    }

    y = 75;

    while (y < PrintWorkingDir3.length())
    {
        PrintWorkingDir3.insert(y, "\n");
        y = y + 75;
    }

    print_header_info();

    tprintf(PRINT_BTH, "\n/////////////////////////Existing Compression Test/////////////////////////\n%s\n\n%s", PrintWorkingDir4.c_str(), PrintWorkingDir3.c_str());
    tprintf(PRINT_BTH, "\n///////////////////////////////////////////////////////////////////////////\n\n");
}
void check_time_stamp(int SelectorArInt, std::string *SelectorAr, std::string *SelectorAr2, std::string TimeStampPrevious, int &identicalFileVar, std::string *TimeStampAr2)
{
    char identicalFileBuffer[3] = "";

    if (SelectorArInt != 0 && SelectorAr[SelectorArInt].compare(SelectorAr[SelectorArInt-1]) == 0)
    {
        if (vpxt_timestamp_compare(TimeStampAr2[0], TimeStampPrevious))
        {
            identicalFileVar++;
            vpxt_itoa_custom(identicalFileVar, identicalFileBuffer, 10);
            TimeStampAr2[0].erase(TimeStampAr2[0].end() - 1);
            TimeStampAr2[0].append("_");
            TimeStampAr2[0].append(identicalFileBuffer);
            TimeStampAr2[0].append("\"");
        }
        else
        {
            identicalFileVar = 1;
        }
    }
    else
    {
        identicalFileVar = 1;
    }
}
void vpxt_formated_print(int selector, const char *fmt, ...)
{
    char buffer[2048];
    va_list ap;
    va_start(ap, fmt);

    int charswritten = vsnprintf(buffer, sizeof(buffer) - 1, fmt, ap);
    std::string SummaryStr = buffer;

    //selector == HLPPRT -> Summary
    //selector == TOLPRT -> Help
    //selector == FUNPRT -> Function
    //selector == OTRPRT -> Other non formatted output
    //selector == RESPRT -> Individual Pass Fail output

    std::string SummaryStrOutput;
    int EndOfLineLength = 0;

    if (selector == HLPPRT || selector == TOLPRT || selector == FUNPRT) //add padding for formating
    {
        SummaryStrOutput.append("         ");
    }

    if (selector == RESPRT) //add padding for formating
    {
        SummaryStrOutput.append(" * ");
    }

    //determine cut off to keep words whole
    int Cutoff;

    if (selector == HLPPRT || selector == TOLPRT || selector == FUNPRT)
    {
        Cutoff = 66;
    }

    if (selector == OTRPRT)
    {
        Cutoff = 79;
    }

    if (selector == RESPRT)
    {
        Cutoff = 70;
    }

    int x = 0;

    while (x + Cutoff < SummaryStr.length())
    {


        if (SummaryStr.substr(x + Cutoff + 1, 1).compare(" ") == 0 || SummaryStr.substr(x + Cutoff, 1).compare(" ") == 0)
        {
            Cutoff++;
        }
        else
        {
            while (SummaryStr.substr(x + Cutoff, 1).compare(" ") != 0)
            {
                Cutoff--;
            }

            Cutoff++;
        }

        //add the properly formated string to the output string
        SummaryStrOutput.append(SummaryStr.substr(x, Cutoff));


        if (selector == HLPPRT || selector == TOLPRT || selector == FUNPRT) //add padding for formating
        {
            SummaryStrOutput.append("\n         ");
        }

        if (selector == RESPRT) //add padding for formating
        {
            SummaryStrOutput.append("\n   ");
        }

        x = x + Cutoff;

        while (SummaryStr.substr(x, 1).compare(" ") == 0)
        {
            x++;
        }

        if (selector == HLPPRT || selector == TOLPRT || selector == FUNPRT)
        {
            Cutoff = 66;
        }

        if (selector == OTRPRT)
        {
            Cutoff = 79;
        }

        if (selector == RESPRT)
        {
            Cutoff = 70;
        }

    }

    SummaryStrOutput.append(SummaryStr.substr(x, SummaryStr.length() - x));

    if (selector == HLPPRT)
    {
        tprintf(PRINT_STD, "\n\nSummary:\n");
        tprintf(PRINT_STD, "%s\n\n", SummaryStrOutput.c_str());
    }

    if (selector == TOLPRT)
    {
        tprintf(PRINT_STD, "\n\n  Help:\n");
        tprintf(PRINT_STD, "%s\n\n", SummaryStrOutput.c_str());
    }

    if (selector == FUNPRT)
    {
        tprintf(PRINT_STD, "\n\nFunction:\n");
        tprintf(PRINT_STD, "%s\n\n", SummaryStrOutput.c_str());
    }

    if (selector == OTRPRT)
    {
        fprintf(stderr, "%s", SummaryStrOutput.c_str());
    }

    if (selector == RESPRT)
    {
        tprintf(PRINT_BTH, "%s", SummaryStrOutput.c_str());
    }

    return;
}
void vpxt_cap_string_print(int selector, const char *fmt, ...)
{
    //This function will capitalize the first letter of any word
    //seperated with either an '_' or a ' ' and print it.

    char buffer[2048];
    char buffer_cap[2048];
    va_list ap;
    va_start(ap, fmt);

    int charswritten = vsnprintf(buffer, sizeof(buffer) - 1, fmt, ap);

    int parse_int = 0;
    int cap_next = 0;

    while (buffer[parse_int] != '\0' && parse_int < sizeof(buffer))
    {
        char add_to_buffer_cap;

        if (cap_next == 1)
        {
            //Capitalize current letter
            add_to_buffer_cap = toupper(buffer[parse_int]);
            cap_next = 0;
        }
        else
        {
            add_to_buffer_cap =  buffer[parse_int];
        }

        if (parse_int == 0)
        {
            //Capitalize current letter
            add_to_buffer_cap = toupper(buffer[parse_int]);
        }

        if (buffer[parse_int] == '_' || buffer[parse_int] == ' ')
        {
            cap_next = 1;
        }

        buffer_cap[parse_int] = add_to_buffer_cap;
        parse_int++;
    }

    buffer_cap[parse_int] = '\0';

    if (selector == PRINT_STD)
        tprintf(PRINT_STD, "%s", buffer_cap);

    if (selector == PRINT_ERR)
        fprintf(stderr, "%s", buffer_cap);

    if (selector == PRINT_BTH)
        tprintf(PRINT_BTH, "%s", buffer_cap);

    return;
}
int  vpxt_lower_case_string(std::string &input)
{
    int pos = 0;

    while (pos < input.length())
    {
        input[pos] = tolower(input[pos]);
        pos = pos + 1;
    }

    return 0;
}
//----------------------------------------------------------IVF API-------------------------------------------------------------------------
#ifdef API
int vpxt_compress_ivf_to_ivf(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck)
{
    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    std::ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        std::string QuantOutStr = outputFile2;
        QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        QuantOutStr.append("_quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_ctx_t        encoder;
    const char             *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    vpx_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    unsigned long            cx_time = 0;

    struct vpx_rational      arg_framerate = {30, 1};

    /* Populate encoder configuration */
    res = vpx_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));

        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return -1;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    vpxt_core_config_to_api_config(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        tprintf(PRINT_BTH, "Input File not found: %s\n", in_fn);

        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    if (ivfhRaw.four_cc == 842094169) //if yv12
    {
        arg_use_i420 = 0;
    }

    int w       = ivfhRaw.width;
    int h       = ivfhRaw.height;
    int fr      = (ivfhRaw.rate / ivfhRaw.scale);

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
    if (codec->iface == &vpx_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        tprintf(PRINT_BTH, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.target_bandwidth, oxcf.worst_allowed_q, oxcf.best_allowed_q, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            tprintf(PRINT_BTH, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            tprintf(PRINT_BTH, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            tprintf(PRINT_BTH, " BestQuality\n\n");
        }

        tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
            fclose(infile);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);

                if (RunQCheck == 1)
                {
                    QuantOutFile.close();
                }

                return -1;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);

                if (RunQCheck == 1)
                {
                    QuantOutFile.close();
                }

                return -1;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? VPX_RC_LAST_PASS : VPX_RC_FIRST_PASS
                 : VPX_RC_ONE_PASS;
#if VPX_ENCODER_ABI_VERSION > (1 + VPX_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = VPX_KF_FIXED;

        vpx_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ///////////Set Encoder Custom Settings/////////////////
        vpx_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.cpu_used);
        vpx_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.encode_breakout);
        vpx_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.play_alternate);
        vpx_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.noise_sensitivity);
        vpx_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        vpx_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (vp8e_token_partitions) oxcf.token_partitions);

        vpx_codec_control(&encoder, VP8E_SET_ARNR_MAXFRAMES, oxcf.arnr_max_frames);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_STRENGTH, oxcf.arnr_strength);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_TYPE, oxcf.arnr_type);
        vpx_codec_control(&encoder, VP8E_SET_CQ_LEVEL, oxcf.cq_level);

        ///////////////////////////////////////////////////////
        if (ctx_exit_on_error_tester(&encoder, "Failed to initialize encoder") == -1)
        {
            fclose(infile);
            fclose(outfile);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            std::string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_paramaters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 20, 20);
            OutputsettingsFile.append("_paramaters_vpx.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings_api(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        while (frame_avail || got_data)
        {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            struct vpx_usec_timer timer;
            int64_t frame_start, next_frame_start;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame_enc(infile, &raw, ivf_fhRaw.frameSize);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");
            }
            else
                frame_avail = 0;

            frame_start = (cfg.g_timebase.den * (int64_t)(frames_in - 1)
                           * arg_framerate.den) / cfg.g_timebase.num / arg_framerate.num;
            next_frame_start = (cfg.g_timebase.den * (int64_t)(frames_in)
                                * arg_framerate.den)
                               / cfg.g_timebase.num / arg_framerate.num;

            vpx_usec_timer_start(&timer);
            vpx_codec_encode(&encoder, frame_avail ? &raw : NULL, frame_start, next_frame_start - frame_start, 0, arg_deadline);
            vpx_usec_timer_mark(&timer);
            cx_time += vpx_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            if (RunQCheck == 1)
            {
                if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
                {
                    int lastQuantizerValue = 0;
                    vpx_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                    QuantOutFile << frames_out << " " << lastQuantizerValue << "\n";
                }
            }

            if (RunQCheck == 2)
            {
                //Print Quantizers
                int lastQuantizerValue = 0;
                vpx_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                tprintf(PRINT_STD, "frame %i Quantizer: %i\n", frames_out, lastQuantizerValue);
            }

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                /*frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                write_ivf_frame_header(outfile, pkt);
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                break;
                case VPX_CODEC_STATS_PKT:
                stats_write(&stats,
                pkt->data.twopass_stats.buf,
                pkt->data.twopass_stats.sz);
                break;
                default:
                break;
                }
                }

                fflush(stdout);*/
                got_data = 1;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    frames_out++;
                    //fprintf(stderr, " %6luF", (unsigned long)pkt->data.frame.sz);
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    nbytes += pkt->data.raw.sz;
                    //tprintf(PRINT_BTH, "Encoded Frame Size: %i\n",pkt->data.frame.sz);
                    break;
                case VPX_CODEC_STATS_PKT:
                    frames_out++;
                    fprintf(stderr, " %6luS",
                            (unsigned long)pkt->data.twopass_stats.sz);
                    stats_write(&stats,
                                pkt->data.twopass_stats.buf,
                                pkt->data.twopass_stats.sz);
                    nbytes += pkt->data.raw.sz;
                    break;
                case VPX_CODEC_PSNR_PKT:

                    if (0)
                    {
                        int i;

                        for (i = 0; i < 4; i++)
                            fprintf(stderr, "%.3lf ", pkt->data.psnr.psnr[i]);
                    }

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
        vpx_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_BTH, "\n");
    }

    vpx_img_free(&raw);

    if (RunQCheck == 1)
    {
        QuantOutFile.close();
    }

    return 0;
}
int vpxt_compress_ivf_to_ivf_no_error_output(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck)
{
    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_ctx_t        encoder;
    const char            *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    vpx_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    double total_cpu_time_used = 0;
    unsigned long          cx_time = 0;

    struct vpx_rational      arg_framerate = {30, 1};

    /* Populate encoder configuration */
    res = vpx_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));
        return -1;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    vpxt_core_config_to_api_config(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        tprintf(PRINT_BTH, "Input File not found: %s\n", in_fn);
        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    if (ivfhRaw.four_cc == 842094169) //if yv12
    {
        arg_use_i420 = 0;
    }

    int w       = ivfhRaw.width;
    int h       = ivfhRaw.height;
    int fr      = (ivfhRaw.rate / ivfhRaw.scale);

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
    if (codec->iface == &vpx_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        tprintf(PRINT_STD, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.target_bandwidth, oxcf.worst_allowed_q, oxcf.best_allowed_q, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            tprintf(PRINT_STD, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            tprintf(PRINT_STD, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            tprintf(PRINT_STD, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            tprintf(PRINT_STD, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            tprintf(PRINT_STD, " BestQuality\n\n");
        }

        tprintf(PRINT_STD, "API - Compressing Raw IVF File to VP8 IVF File: \n");
        //fprintf(stderr, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);
            return -1;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
            fclose(infile);
            return -1;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);
                return -1;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);
                return -1;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? VPX_RC_LAST_PASS : VPX_RC_FIRST_PASS
                 : VPX_RC_ONE_PASS;
#if VPX_ENCODER_ABI_VERSION > (1 + VPX_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = VPX_KF_FIXED;


        vpx_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        vpx_codec_enc_config_set(&encoder, &cfg);
        ///////////Set Encoder Custom Settings/////////////////
        vpx_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.cpu_used);
        vpx_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.encode_breakout);
        vpx_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.play_alternate);
        vpx_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.noise_sensitivity);
        vpx_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        vpx_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (vp8e_token_partitions) oxcf.token_partitions);

        vpx_codec_control(&encoder, VP8E_SET_ARNR_MAXFRAMES, oxcf.arnr_max_frames);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_STRENGTH, oxcf.arnr_strength);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_TYPE, oxcf.arnr_type);
        vpx_codec_control(&encoder, VP8E_SET_CQ_LEVEL, oxcf.cq_level);

        ///////////////////////////////////////////////////////
        if (ctx_exit_on_error_tester(&encoder, "Failed to initialize encoder") == -1)
        {
            fclose(infile);
            fclose(outfile);
            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            std::string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_paramaters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 20, 20);
            OutputsettingsFile.append("_paramaters_vpx.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings_api(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        while (frame_avail || got_data)
        {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            clock_t start = 0, end = 0;
            struct vpx_usec_timer timer;
            int64_t frame_start, next_frame_start;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame_enc(infile, &raw, ivf_fhRaw.frameSize);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    tprintf(PRINT_STD, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_STD, ".");
            }
            else
                frame_avail = 0;

            frame_start = (cfg.g_timebase.den * (int64_t)(frames_in - 1)
                           * arg_framerate.den) / cfg.g_timebase.num / arg_framerate.num;
            next_frame_start = (cfg.g_timebase.den * (int64_t)(frames_in)
                                * arg_framerate.den)
                               / cfg.g_timebase.num / arg_framerate.num;

            vpx_usec_timer_start(&timer);
            vpx_codec_encode(&encoder, frame_avail ? &raw : NULL, frame_start, next_frame_start - frame_start, 0, arg_deadline);
            vpx_usec_timer_mark(&timer);
            cx_time += vpx_usec_timer_elapsed(&timer);

            total_cpu_time_used = total_cpu_time_used + (end - start);

            ctx_exit_on_error_tester(&encoder, "Failed to encode frame");
            got_data = 0;

            //Print Quantizers
            //int lastQuantizerValue = 0;
            //vpx_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
            //tprintf(  PRINT_STD, "frame %i Quantizer: %i\n",frames_out,lastQuantizerValue);

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case VPX_CODEC_STATS_PKT:
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
        vpx_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_STD, "\n");
    }

    vpx_img_free(&raw);
    return 0;
}
unsigned int vpxt_time_compress_ivf_to_ivf(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck, unsigned int &CPUTick)
{
    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_ctx_t        encoder;
    const char            *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    vpx_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;

    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    unsigned long            cx_time = 0;
    unsigned int total_cpu_time_used = 0;
    int framesoutrec = 0;
    struct vpx_rational      arg_framerate = {30, 1};

    /* Populate encoder configuration */
    res = vpx_codec_enc_config_default(codec->iface, &cfg, 0);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));
        return -1;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    vpxt_core_config_to_api_config(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        tprintf(PRINT_BTH, "Input File not found: %s\n", in_fn);
        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    if (ivfhRaw.four_cc == 842094169) //if yv12
    {
        arg_use_i420 = 0;
    }

    int w       = ivfhRaw.width;
    int h       = ivfhRaw.height;
    int fr      = (ivfhRaw.rate / ivfhRaw.scale);

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
    if (codec->iface == &vpx_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        tprintf(PRINT_BTH, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.target_bandwidth, oxcf.worst_allowed_q, oxcf.best_allowed_q, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            tprintf(PRINT_BTH, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            tprintf(PRINT_BTH, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            tprintf(PRINT_BTH, " BestQuality\n\n");
        }

        tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            tprintf(PRINT_STD, "Failed to open input file");
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            tprintf(PRINT_STD, "Failed to open output file");
            fclose(infile);
            return -1;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);
                return -1;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);
                return -1;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? VPX_RC_LAST_PASS : VPX_RC_FIRST_PASS
                 : VPX_RC_ONE_PASS;
#if VPX_ENCODER_ABI_VERSION > (1 + VPX_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = VPX_KF_FIXED;

        vpx_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ///////////Set Encoder Custom Settings/////////////////
        vpx_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.cpu_used);
        vpx_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.encode_breakout);
        vpx_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.play_alternate);
        vpx_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.noise_sensitivity);
        vpx_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        vpx_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (vp8e_token_partitions) oxcf.token_partitions);

        vpx_codec_control(&encoder, VP8E_SET_ARNR_MAXFRAMES, oxcf.arnr_max_frames);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_STRENGTH, oxcf.arnr_strength);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_TYPE, oxcf.arnr_type);
        vpx_codec_control(&encoder, VP8E_SET_CQ_LEVEL, oxcf.cq_level);

        ///////////////////////////////////////////////////////
        if (ctx_exit_on_error_tester(&encoder, "Failed to initialize encoder") == -1)
        {
            fclose(infile);
            fclose(outfile);
            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            std::string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_paramaters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 20, 20);
            OutputsettingsFile.append("_paramaters_vpx.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings_api(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        while (frame_avail || got_data)
        {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            unsigned int start, end;
            struct vpx_usec_timer timer;
            int64_t frame_start, next_frame_start;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame_enc(infile, &raw, ivf_fhRaw.frameSize);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");
            }
            else
                frame_avail = 0;

            frame_start = (cfg.g_timebase.den * (int64_t)(frames_in - 1)
                           * arg_framerate.den) / cfg.g_timebase.num / arg_framerate.num;
            next_frame_start = (cfg.g_timebase.den * (int64_t)(frames_in)
                                * arg_framerate.den)
                               / cfg.g_timebase.num / arg_framerate.num;

            start = vpxt_get_cpu_tick();
            vpx_usec_timer_start(&timer);
            vpx_codec_encode(&encoder, frame_avail ? &raw : NULL, frame_start, next_frame_start - frame_start, 0, arg_deadline);
            vpx_usec_timer_mark(&timer);
            cx_time += vpx_usec_timer_elapsed(&timer);
            end = vpxt_get_cpu_tick();

            ctx_exit_on_error_tester(&encoder, "Failed to encode frame");
            got_data = 0;

            total_cpu_time_used = total_cpu_time_used + (end - start);

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case VPX_CODEC_STATS_PKT:
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
        vpx_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_STD, "\n");
        framesoutrec = frames_out;
    }

    vpx_img_free(&raw);


    tprintf(PRINT_BTH, "\n File completed: Time in Microseconds: %u, Fps: %d \n",
            cx_time, 1000 * framesoutrec / (cx_time / 1000));
    tprintf(PRINT_BTH, " Total CPU Ticks: %u\n", total_cpu_time_used);

    char TextFilechar1[255];
    char TextFilechar2[255];

    vpxt_remove_file_extension(outputFile2, TextFilechar1);
    vpxt_remove_file_extension(outputFile2, TextFilechar2);

    char *FullNameMs = strcat(TextFilechar1, "compression_time.txt");

    std::ofstream FullNameMsFile(FullNameMs);
    FullNameMsFile << cx_time;
    FullNameMsFile.close();

    char *FullNameCpu = strcat(TextFilechar2, "compression_cpu_tick.txt");

    std::ofstream FullNameCpuFile(FullNameCpu);
    FullNameCpuFile << total_cpu_time_used;
    FullNameCpuFile.close();

    CPUTick = total_cpu_time_used;
    return cx_time;
}
int vpxt_compress_ivf_to_ivf_force_key_frame(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck, int forceKeyFrame)
{
    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    std::ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        std::string QuantOutStr = outputFile2;
        QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        QuantOutStr.append("_quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_ctx_t        encoder;
    const char                  *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    vpx_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    double total_cpu_time_used = 0;
    int                  flags = 0;
    unsigned long          cx_time = 0;

    struct vpx_rational      arg_framerate = {30, 1};

    /* Populate encoder configuration */
    res = vpx_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));

        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return -1;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    vpxt_core_config_to_api_config(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        tprintf(PRINT_BTH, "Input File not found: %s\n", in_fn);

        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    if (ivfhRaw.four_cc == 842094169) //if yv12
    {
        arg_use_i420 = 0;
    }

    int w       = ivfhRaw.width;
    int h       = ivfhRaw.height;
    int fr      = (ivfhRaw.rate / ivfhRaw.scale);

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
    if (codec->iface == &vpx_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    for (pass = 0; pass < arg_passes; pass++)
    {
        tprintf(PRINT_BTH, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.target_bandwidth, oxcf.worst_allowed_q, oxcf.best_allowed_q, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            tprintf(PRINT_BTH, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            tprintf(PRINT_BTH, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            tprintf(PRINT_BTH, " BestQuality\n\n");
        }

        tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
            fclose(infile);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);

                if (RunQCheck == 1)
                {
                    QuantOutFile.close();
                }

                return -1;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                fclose(infile);
                fclose(outfile);

                if (RunQCheck == 1)
                {
                    QuantOutFile.close();
                }

                return -1;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? VPX_RC_LAST_PASS : VPX_RC_FIRST_PASS
                 : VPX_RC_ONE_PASS;
#if VPX_ENCODER_ABI_VERSION > (1 + VPX_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = VPX_KF_FIXED;

        vpx_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ///////////Set Encoder Custom Settings/////////////////
        vpx_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.cpu_used);
        vpx_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.encode_breakout);
        vpx_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.play_alternate);
        vpx_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.noise_sensitivity);
        vpx_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        vpx_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (vp8e_token_partitions) oxcf.token_partitions);

        vpx_codec_control(&encoder, VP8E_SET_ARNR_MAXFRAMES, oxcf.arnr_max_frames);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_STRENGTH, oxcf.arnr_strength);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_TYPE, oxcf.arnr_type);
        vpx_codec_control(&encoder, VP8E_SET_CQ_LEVEL, oxcf.cq_level);

        ///////////////////////////////////////////////////////
        if (ctx_exit_on_error_tester(&encoder, "Failed to initialize encoder") == -1)
        {
            fclose(infile);
            fclose(outfile);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            std::string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_paramaters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 20, 20);
            OutputsettingsFile.append("_paramaters_vpx.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings_api(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        int forceKeyFrameTracker = forceKeyFrame;

        while (frame_avail || got_data)
        {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            struct vpx_usec_timer timer;
            int64_t frame_start, next_frame_start;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame_enc(infile, &raw, ivf_fhRaw.frameSize);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");
            }
            else
                frame_avail = 0;

            //std::cout << "\nframes_in: " << frames_in << " forceKeyFrameTracker: " << forceKeyFrameTracker;

            if (frames_in - 1 == forceKeyFrameTracker)
            {
                flags |= VPX_EFLAG_FORCE_KF;
                forceKeyFrameTracker = forceKeyFrameTracker + forceKeyFrame;
                //std::cout << "\nForce Key Frame: " << frames_in << " Next KeyFrame: " << forceKeyFrameTracker <<"\n";
            }
            else
            {
                flags &= ~VPX_EFLAG_FORCE_KF;
            }

            frame_start = (cfg.g_timebase.den * (int64_t)(frames_in - 1)
                           * arg_framerate.den) / cfg.g_timebase.num / arg_framerate.num;
            next_frame_start = (cfg.g_timebase.den * (int64_t)(frames_in)
                                * arg_framerate.den)
                               / cfg.g_timebase.num / arg_framerate.num;

            vpx_usec_timer_start(&timer);
            vpx_codec_encode(&encoder, frame_avail ? &raw : NULL, frame_start, next_frame_start - frame_start, flags, arg_deadline);
            vpx_usec_timer_mark(&timer);
            cx_time += vpx_usec_timer_elapsed(&timer);

            ctx_exit_on_error_tester(&encoder, "Failed to encode frame");
            got_data = 0;

            if (RunQCheck == 1)
            {
                if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
                {
                    int lastQuantizerValue = 0;
                    vpx_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                    QuantOutFile << frames_out << " " << lastQuantizerValue << "\n";
                }
            }

            if (RunQCheck == 2)
            {
                //Print Quantizers
                int lastQuantizerValue = 0;
                vpx_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                tprintf(PRINT_STD, "frame %i Quantizer: %i\n", frames_out, lastQuantizerValue);
            }

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case VPX_CODEC_STATS_PKT:
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
        vpx_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_BTH, "\n");
    }

    vpx_img_free(&raw);

    if (RunQCheck == 1)
    {
        QuantOutFile.close();
    }

    return 0;
}
int vpxt_compress_ivf_to_ivf_recon_buffer_check(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck)
{
    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    std::ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        std::string QuantOutStr = outputFile2;
        QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        QuantOutStr.append("_quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_ctx_t       encoder;
    const char            *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    FILE                  *infile, *outfile, *outfile3;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    vpx_image_t            raw;
    const struct codec_item  *codec = codecs;
    int                    frame_avail, got_data;
    int                      arg_usage = 0, arg_passes = 1, arg_deadline = 0;
    int                      arg_limit = 0;
    static const arg_def_t **ctrl_args = no_args;
    int                      verbose = 0;
    int                      arg_use_i420 = 1;
    double total_cpu_time_used = 0;
    unsigned long          cx_time = 0;

    struct vpx_rational      arg_framerate = {30, 1};

    //outfile = encoded ivf file
    void *out;//all raw preview frames
    void *out2;//all raw decoded frames
    void *out3;//individual raw preview frames
    void *out4;//individual decoded preview frames

    /* Populate encoder configuration */
    res = vpx_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));

        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return -1;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    vpxt_core_config_to_api_config(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");

    if (GetWHinfile == NULL)
    {
        tprintf(PRINT_BTH, "Input File not found: %s\n", in_fn);

        if (RunQCheck == 1)
        {
            QuantOutFile.close();
        }

        return -1;
    }

    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    if (ivfhRaw.four_cc == 842094169) //if yv12
    {
        arg_use_i420 = 0;
    }

    int w       = ivfhRaw.width;
    int h       = ivfhRaw.height;
    int fr      = (ivfhRaw.rate / ivfhRaw.scale);

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
    if (codec->iface == &vpx_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

    vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    std::string out_fn4STRb = out_fn;
    out_fn4STRb.erase(out_fn4STRb.length() - 4, 4);
    out_fn4STRb.append("_DecodeFrame");
    out_fn4STRb.append(slashCharStr().c_str());

    std::string CreateDir3b = out_fn4STRb;
    CreateDir3b.insert(0, "mkdir \"");
    CreateDir3b.append("\"");
    system(CreateDir3b.c_str());

    std::string out_fn3STRb = out_fn;
    out_fn3STRb.erase(out_fn3STRb.length() - 4, 4);
    out_fn3STRb.append("_PreviewFrame");
    out_fn3STRb.append(slashCharStr().c_str());

    std::string CreateDir2b = out_fn3STRb;
    CreateDir2b.insert(0, "mkdir \"");
    CreateDir2b.append("\"");
    system(CreateDir2b.c_str());


    std::ofstream ReconOutFile;
    std::string ReconOutStr = outputFile2;
    ReconOutStr.erase(ReconOutStr.length() - 4, 4);
    ReconOutStr.append("_ReconFrameState.txt");
    char ReconOutChar[255];
    snprintf(ReconOutChar, 255, "%s", ReconOutStr.c_str());

    ReconOutFile.open(ReconOutChar);

    for (pass = 0; pass < arg_passes; pass++)
    {
        tprintf(PRINT_BTH, "\n\n Target Bit Rate: %d \n Max Quantizer: %d \n Min Quantizer %d \n %s: %d \n \n", oxcf.target_bandwidth, oxcf.worst_allowed_q, oxcf.best_allowed_q, CompressString, CompressInt);

        int CharCount = 0;

        if (pass == 0 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nFirst Pass - ");
        }

        if (pass == 1 && arg_passes == 2)
        {
            tprintf(PRINT_BTH, "\nSecond Pass - ");
        }

        if (oxcf.Mode == 0) //Real Time Mode
        {
            tprintf(PRINT_BTH, " RealTime\n\n");
        }

        if (oxcf.Mode == 1 || oxcf.Mode == 4) //One Pass Good
        {
            tprintf(PRINT_BTH, " GoodQuality\n\n");
        }

        if (oxcf.Mode == 2 || oxcf.Mode == 5) //One Pass Best
        {
            tprintf(PRINT_BTH, " BestQuality\n\n");
        }

        tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);
            ReconOutFile.close();

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");
        /////////////////////////DELETEME//////////////////////////////////
        std::string out_fn2STR = out_fn;
        out_fn2STR.append("_Preview.raw");
        out = out_open(out_fn2STR.c_str(), 0);

        std::string out_fn3STR = out_fn;
        out_fn3STR.append("_Decode.raw");
        out2 = out_open(out_fn3STR.c_str(), 0);
        ///////////////////////////////////////////////////////////////////

        if (!outfile)
        {
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
            ReconOutFile.close();
            fclose(infile);
            out_close(out, out_fn2STR.c_str(), 0);
            out_close(out2, out_fn2STR.c_str(), 0);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                ReconOutFile.close();
                fclose(infile);
                fclose(outfile);
                out_close(out, out_fn2STR.c_str(), 0);
                out_close(out2, out_fn2STR.c_str(), 0);

                if (RunQCheck == 1)
                {
                    QuantOutFile.close();
                }

                return -1;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                ReconOutFile.close();
                fclose(infile);
                fclose(outfile);
                out_close(out, out_fn2STR.c_str(), 0);
                out_close(out2, out_fn2STR.c_str(), 0);

                if (RunQCheck == 1)
                {
                    QuantOutFile.close();
                }

                return -1;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? VPX_RC_LAST_PASS : VPX_RC_FIRST_PASS
                 : VPX_RC_ONE_PASS;
#if VPX_ENCODER_ABI_VERSION > (1 + VPX_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = VPX_KF_FIXED;

        //vpx_codec_enc_config_set(&encoder,&cfg);
        vpx_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ///////////Set Encoder Custom Settings/////////////////
        vpx_codec_control(&encoder, VP8E_SET_CPUUSED, oxcf.cpu_used);
        vpx_codec_control(&encoder, VP8E_SET_STATIC_THRESHOLD, oxcf.encode_breakout);
        vpx_codec_control(&encoder, VP8E_SET_ENABLEAUTOALTREF, oxcf.play_alternate);
        vpx_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, oxcf.noise_sensitivity);
        vpx_codec_control(&encoder, VP8E_SET_SHARPNESS, oxcf.Sharpness);
        vpx_codec_control(&encoder, VP8E_SET_TOKEN_PARTITIONS, (vp8e_token_partitions) oxcf.token_partitions);

        vpx_codec_control(&encoder, VP8E_SET_ARNR_MAXFRAMES, oxcf.arnr_max_frames);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_STRENGTH, oxcf.arnr_strength);
        vpx_codec_control(&encoder, VP8E_SET_ARNR_TYPE, oxcf.arnr_type);
        vpx_codec_control(&encoder, VP8E_SET_CQ_LEVEL, oxcf.cq_level);

        ///////////////////////////////////////////////////////
        if (ctx_exit_on_error_tester(&encoder, "Failed to initialize encoder") == -1)
        {
            fclose(infile);
            fclose(outfile);
            ReconOutFile.close();
            out_close(out, out_fn2STR.c_str(), 0);
            out_close(out2, out_fn2STR.c_str(), 0);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            std::string OutputsettingsFile = outputFile2;
            OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            OutputsettingsFile.append("_paramaters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - 20, 20);
            OutputsettingsFile.append("_paramaters_vpx.txt");
            char OutputsettingsFileChar2[255];

            snprintf(OutputsettingsFileChar2, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings_api(OutputsettingsFileChar2,  cfg);
            ///////////////////////////////////////////////////////////////////////////////////////////
        }

        /////////////////////////////////////////////////INI DECODER/////////////////////////////////////////////////
        vpx_codec_ctx_t       decoder;
        vp8_postproc_cfg_t      vp8_pp_cfg = {0};
        vpx_codec_iface_t       *iface = ifaces[0].iface;
        int postproc = 0;
        vpx_codec_dec_cfg_t     cfgdec = {0};

        if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfgdec,
                               postproc ? VPX_CODEC_USE_POSTPROC : 0))
        {
            tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
            ReconOutFile.close();
            fclose(infile);
            fclose(outfile);
            out_close(out, out_fn2STR.c_str(), 0);
            out_close(out2, out_fn2STR.c_str(), 0);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }

        if (vp8_pp_cfg.post_proc_flag
            && vpx_codec_control(&decoder, VP8_SET_POSTPROC, &vp8_pp_cfg))
        {
            fprintf(stderr, "Failed to configure postproc: %s\n", vpx_codec_error(&decoder));
            ReconOutFile.close();
            fclose(infile);
            fclose(outfile);
            out_close(out, out_fn2STR.c_str(), 0);
            out_close(out2, out_fn2STR.c_str(), 0);

            if (RunQCheck == 1)
            {
                QuantOutFile.close();
            }

            return -1;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (frame_avail || got_data)
        {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            struct vpx_usec_timer timer;
            int64_t frame_start, next_frame_start;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame_enc(infile, &raw, ivf_fhRaw.frameSize);

                if (frame_avail)
                    frames_in++;

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");
            }
            else
                frame_avail = 0;

            frame_start = (cfg.g_timebase.den * (int64_t)(frames_in - 1)
                           * arg_framerate.den) / cfg.g_timebase.num / arg_framerate.num;
            next_frame_start = (cfg.g_timebase.den * (int64_t)(frames_in)
                                * arg_framerate.den)
                               / cfg.g_timebase.num / arg_framerate.num;

            vpx_usec_timer_start(&timer);
            vpx_codec_encode(&encoder, frame_avail ? &raw : NULL, frame_start, next_frame_start - frame_start, 0, arg_deadline);
            vpx_usec_timer_mark(&timer);
            cx_time += vpx_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            if (RunQCheck == 1)
            {
                if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
                {
                    int lastQuantizerValue = 0;
                    vpx_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                    QuantOutFile << frames_out << " " << lastQuantizerValue << "\n";
                }
            }

            if (RunQCheck == 2)
            {
                //Print Quantizers
                int lastQuantizerValue = 0;
                vpx_codec_control(&encoder, VP8E_GET_LAST_QUANTIZER_64, &lastQuantizerValue);
                tprintf(PRINT_STD, "frame %i Quantizer: %i\n", frames_out, lastQuantizerValue);
            }

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;
                vpx_codec_iter_t  iterdec = NULL;
                int MemCheckY = 0;
                int MemCheckU = 0;
                int MemCheckV = 0;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);

                    const vpx_image_t    *imgPreview;
                    const vpx_image_t    *imgDecode;

                    imgPreview = vpx_codec_get_preview_frame(&encoder);

                    vpx_codec_decode(&decoder, (const uint8_t *)pkt->data.frame.buf, pkt->data.frame.sz, NULL, 0);
                    imgDecode = vpx_codec_get_frame(&decoder, &iterdec);

                    if (imgPreview && imgDecode)
                    {
                        std::string out_fn3STR = out_fn;
                        out_fn3STR.erase(out_fn3STR.length() - 4, 4);
                        out_fn3STR.append("_PreviewFrame");
                        out_fn3STR.append(slashCharStr().c_str());

                        char intchar[56];
                        vpxt_itoa_custom(frames_out, intchar, 10);
                        out_fn3STR.append(intchar);
                        out_fn3STR.append(".raw");
                        out3 = out_open(out_fn3STR.c_str(), 0);

                        std::string out_fn4STR = out_fn;
                        out_fn4STR.erase(out_fn4STR.length() - 4, 4);
                        out_fn4STR.append("_DecodeFrame");
                        out_fn4STR.append(slashCharStr().c_str());

                        char intchar2[56];
                        vpxt_itoa_custom(frames_out, intchar2, 10);
                        out_fn4STR.append(intchar2);
                        out_fn4STR.append(".raw");
                        out4 = out_open(out_fn4STR.c_str(), 0);

                        unsigned int y;
                        char out_fn[128+24];
                        uint8_t *bufPreview;
                        uint8_t *bufDecode;

                        bufPreview = imgPreview->planes[PLANE_Y];
                        bufDecode = imgDecode->planes[PLANE_Y];

                        for (y = 0; y < imgPreview->d_h; y++)
                        {
                            out_put(out3, bufPreview, imgPreview->d_w, 0);
                            bufPreview += imgPreview->stride[PLANE_Y];

                            out_put(out4, bufDecode, imgDecode->d_w, 0);
                            bufDecode += imgDecode->stride[PLANE_Y];

                            MemCheckY |= memcmp(bufPreview, bufDecode, imgDecode->stride[PLANE_Y]);
                        }

                        bufPreview = imgPreview->planes[PLANE_U];
                        bufDecode = imgDecode->planes[PLANE_U];

                        for (y = 0; y < imgPreview->d_h / 2; y++)
                        {
                            out_put(out3, bufPreview, imgPreview->d_w / 2, 0);
                            bufPreview += imgPreview->stride[PLANE_U];

                            out_put(out4, bufDecode, imgDecode->d_w / 2, 0);
                            bufDecode += imgDecode->stride[PLANE_U];

                            MemCheckU |= memcmp(bufPreview, bufDecode, imgDecode->stride[PLANE_U]);
                        }

                        bufPreview = imgPreview->planes[PLANE_V];
                        bufDecode = imgDecode->planes[PLANE_V];

                        for (y = 0; y < imgPreview->d_h / 2; y++)
                        {
                            out_put(out3, bufPreview, imgPreview->d_w / 2, 0);
                            bufPreview += imgPreview->stride[PLANE_V];

                            out_put(out4, bufDecode, imgDecode->d_w / 2, 0);
                            bufDecode += imgDecode->stride[PLANE_V];

                            MemCheckV |= memcmp(bufPreview, bufDecode, imgDecode->stride[PLANE_V]);
                        }


                        if (MemCheckY != 0)
                        {
                            ReconOutFile << frames_out << " Y " << 0 << "\n";
                        }
                        else
                        {
                            ReconOutFile << frames_out << " Y " << 1 << "\n";
                        }

                        if (MemCheckU != 0)
                        {
                            ReconOutFile << frames_out << " U " << 0 << "\n";
                        }
                        else
                        {
                            ReconOutFile << frames_out << " U " << 1 << "\n";
                        }

                        if (MemCheckV != 0)
                        {
                            ReconOutFile << frames_out << " V " << 0 << "\n";
                        }
                        else
                        {
                            ReconOutFile << frames_out << " V " << 1 << "\n";
                        }

                        out_close(out3, out_fn3STR.c_str(), 0);
                        out_close(out4, out_fn4STR.c_str(), 0);
                    }

                    if (imgPreview)
                    {
                        unsigned int y;
                        char out_fn[128+24];
                        uint8_t *buf;

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
                case VPX_CODEC_STATS_PKT:
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

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);

        fclose(infile);
        fclose(outfile);
        stats_close(&stats, arg_passes - 1);

        out_close(out, out_fn2STR.c_str(), 0);
        out_close(out2, out_fn2STR.c_str(), 0);

        tprintf(PRINT_BTH, "\n");
    }

    vpx_img_free(&raw);
    ReconOutFile.close();

    if (RunQCheck == 1)
    {
        QuantOutFile.close();
    }

    return 0;
}
int vpxt_decompress_ivf_to_ivf(const char *inputchar, const char *outputchar)
{
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    double total_cpu_time_used = 0;
    const char                   *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};
    unsigned long          dx_time = 0;

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
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
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

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

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
int vpxt_decompress_ivf_to_raw(const char *inputchar, const char *outputchar)
{
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    double total_cpu_time_used = 0;
    unsigned int total_cpu_time_used_ms = 0;
    const char             *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    unsigned long           dx_time = 0;

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (vp8_pp_cfg.post_proc_flag
        && vpx_codec_control(&decoder, VP8_SET_POSTPROC, &vp8_pp_cfg))
    {
        fprintf(stderr, "Failed to configure postproc: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            tprintf(PRINT_STD, "decoded frame %d.\n", frame);

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
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
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

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
            //  tprintf(  PRINT_STD, "\n\n NO IMG \n\n");
            //}
        }

        if (stop_after && frame >= stop_after)
            break;
    }

    total_cpu_time_used_ms = (unsigned int)(total_cpu_time_used);

    if (summary)
    {
        tprintf(PRINT_BTH, "\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, total_cpu_time_used_ms, (float)frame * 1000000.0 / (float)total_cpu_time_used_ms);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
int vpxt_decompress_ivf_to_raw_no_error_output(const char *inputchar, const char *outputchar)
{
    vpx_codec_ctx_t       decoder;
    const char                  *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    double total_cpu_time_used = 0;
    unsigned int total_cpu_time_used_ms = 0;
    const char              *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};
    unsigned long           dx_time = 0;

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_STD, "Failed to open input file: %s", fn);
        return -1;
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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            tprintf(PRINT_STD, "decoded frame %d.\n", frame);

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
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
                    tprintf(PRINT_STD, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_STD, ".");

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

    total_cpu_time_used_ms = (unsigned int)(total_cpu_time_used);

    if (summary)
    {
        tprintf(PRINT_BTH, "\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, total_cpu_time_used_ms, (float)frame * 1000000.0 / (float)total_cpu_time_used_ms);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
int vpxt_decompress_ivf_to_ivf_no_output(const char *inputchar, const char *outputchar)
{
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    double total_cpu_time_used = 0;
    unsigned int total_cpu_time_used_ms = 0;
    const char                   *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};
    unsigned long           dx_time = 0;

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_STD, "Failed to open input file: %s", fn);
        return -1;
    }

    tprintf(PRINT_STD, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
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
                    tprintf(PRINT_STD, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_STD, ".");

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

    total_cpu_time_used_ms = (unsigned int)(total_cpu_time_used);

    if (summary && total_cpu_time_used_ms)
    {
        tprintf(PRINT_BTH, "\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, total_cpu_time_used_ms, (float)frame * 1000000.0 / (float)total_cpu_time_used_ms);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
unsigned int vpxt_time_decompress_ivf_to_ivf(const char *inputchar, const char *outputchar, unsigned int &CPUTick)
{
    //Time Decompress is not supposed to save output that is the only difference between it and
    //vpxt_decompress_ivf_to_ivf_time_and_output
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 1, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    const char             *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};
    unsigned int           dx_time = 0;
    unsigned int           total_cpu_time_used = 0;

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;
        unsigned __int64 start, end;

        start = vpxt_get_cpu_tick();
        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        end  = vpxt_get_cpu_tick();
        dx_time += vpx_usec_timer_elapsed(&timer);

        total_cpu_time_used = total_cpu_time_used + (end - start);
        //vpx_usec_timer_mark(&timer);
        //dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (!noblit)
        {
            if (vpx_codec_get_frame(&decoder, &iter))
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
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

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
        tprintf(PRINT_BTH, "\n\n Decoded %d frames in %lu us (%.2f fps)\n", frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
        tprintf(PRINT_BTH, " Total CPU Ticks: %i\n", total_cpu_time_used);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    char TextFilechar1[255];
    char TextFilechar2[255];

    vpxt_remove_file_extension(outputchar, TextFilechar1);
    vpxt_remove_file_extension(outputchar, TextFilechar2);

    char *FullNameMs = strcat(TextFilechar1, "decompression_time.txt");

    std::ofstream FullNameMsFile(FullNameMs);
    FullNameMsFile << dx_time;
    FullNameMsFile.close();

    char *FullNameCpu = strcat(TextFilechar2, "decompression_cpu_tick.txt");

    std::ofstream FullNameCpuFile(FullNameCpu);
    FullNameCpuFile << total_cpu_time_used;
    FullNameCpuFile.close();

    CPUTick = total_cpu_time_used;
    return dx_time;
}
unsigned int vpxt_decompress_ivf_to_ivf_time_and_output(const char *inputchar, const char *outputchar, unsigned int &CPUTick)
{
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    const char             *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t    cfg = {0};
    unsigned int           dx_time = 0;
    unsigned int           total_cpu_time_used = 0;

    int CharCount = 0;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");

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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;
        unsigned __int64 start, end;

        start = vpxt_get_cpu_tick();
        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        end  = vpxt_get_cpu_tick();

        dx_time += vpx_usec_timer_elapsed(&timer);
        total_cpu_time_used = total_cpu_time_used + (end - start);

        ++frame;

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
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
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

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
        //tprintf(  PRINT_BTH, "\n\nDecoded %d frames in %lu us (%.2f fps)\n", frame, total_cpu_time_used_ms, (float)frame * 1000000.0 / (float)total_cpu_time_used_ms);
        tprintf(PRINT_BTH, "\n\n Decoded %d frames in %lu us (%.2f fps)\n", frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
        tprintf(PRINT_BTH, " Total CPU Ticks: %i\n", total_cpu_time_used);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    char TextFilechar1[255];
    char TextFilechar2[255];

    vpxt_remove_file_extension(outputchar, TextFilechar1);
    vpxt_remove_file_extension(outputchar, TextFilechar2);

    char *FullNameMs = strcat(TextFilechar1, "decompression_time.txt");

    std::ofstream FullNameMsFile(FullNameMs);
    FullNameMsFile << dx_time;
    FullNameMsFile.close();

    char *FullNameCpu = strcat(TextFilechar2, "decompression_cpu_tick.txt");

    std::ofstream FullNameCpuFile(FullNameCpu);
    FullNameCpuFile << total_cpu_time_used;
    FullNameCpuFile.close();

    CPUTick = total_cpu_time_used;
    return dx_time;
}
int vpxt_dec_compute_md5(const char *inputchar, const char *outputchar)
{
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    const char             *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};

    int CharCount = 0;

    do_md5 = 1;

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_STD, "Failed to open file");
        return -1;
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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (vp8_pp_cfg.post_proc_flag
        && vpx_codec_control(&decoder, VP8_SET_POSTPROC, &vp8_pp_cfg))
    {
        fprintf(stderr, "Failed to configure postproc: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            tprintf(PRINT_STD, "decoded frame %d.\n", frame);

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
            {
                unsigned int y;
                char out_fn[128+24];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!fn2)
                {
                    out = out_open(out_fn, do_md5);
                }

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
        tprintf(PRINT_STD, "Decoded %d frames in %lu us (%.2f fps)\n",
                frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        if (fn2)
            out_close(out, fn2, do_md5);

        return -1;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);

    return 0;
}
#endif
//--------------------------------------------------------IVF Tools-------------------------------------------------------------------------
int vpxt_cut_ivf(const char *inputFile, const char *outputFile, int StartingFrame, int EndingFrame)
{
    bool verbose = 1;

    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");
        return 0;
    }

    FILE *out = fopen(outputFile, "wb");

    if (out == NULL)
    {
        tprintf(PRINT_STD, "\nOutput file does not exist");
        fclose(in);
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
    fread(&ivfhRaw.four_cc, 1, 4, in);
    fread(&ivfhRaw.width, 1, 2, in);
    fread(&ivfhRaw.height, 1, 2, in);
    fread(&ivfhRaw.rate, 1, 4, in);
    fread(&ivfhRaw.scale, 1, 4, in);
    fread(&ivfhRaw.length, 1, 4, in);
    fread(&storage, 1, 4, in);
    vpxt_format_ivf_header_read(&ivfhRaw);

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
    "Unused                 - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    /*tprintf(  PRINT_STD, "FRAME HEADER 0\n\n"
    "Frame Size            - %i \n"
    "Time Stamp            - %i \n"
    "\n"

    ,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);*/

    IVF_HEADER ivfhCompressed;
    InitIVFHeader(&ivfhCompressed);
    memset(&ivfhCompressed, 0, sizeof(ivfhCompressed));
    ivfhCompressed = ivfhRaw;
    IVF_FRAME_HEADER ivf_fhCompressed;
    vpxt_format_ivf_header_write(ivfhCompressed);
    fwrite((char *)&ivfhCompressed, 1, 32, out);
    vpxt_format_ivf_header_read(&ivfhCompressed);
    tprintf(PRINT_STD, "\nCopy IVF file to IVF file: \n");

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
                vpxt_format_frame_header_read(ivf_fhRaw);
                fread(inputVideoBuffer, 1, ivf_fhRaw.frameSize, in);

                //ivf_fhCompressed.timeStamp = NewTimeStamp;


                /*  if(currentVideoFrame != frameCount)
                {
                tprintf(  PRINT_STD, "FRAME HEADER %i\n\n"
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
            //ivf_fhCompressed.timeStamp = ivf_fhRaw.timeStamp;//0;     // not defined, use rate and scale in header replaced by line: memset(&ivf_fh, 0, sizeof(ivf_fh));

            vpxt_format_frame_header_write(ivf_fhCompressed);
            fwrite(&ivf_fhCompressed.frameSize, 1, sizeof(ivf_fhCompressed.frameSize), out);
            fwrite(&ivf_fhCompressed.timeStamp, 1, sizeof(ivf_fhCompressed.timeStamp), out);
            fwrite(inputVideoBuffer, 1, FrameSizeStorage, out);


            if (CharCount == 79)
            {
                tprintf(PRINT_STD, "\n");
                CharCount = 0;
            }

            tprintf(PRINT_STD, "*");
            ++currentVideoFrame;
            WrittenoutVideoFrames++;
            CharCount++;
            NewTimeStamp = NewTimeStamp + 2;

            ivf_fhRaw.frameSize = 0;
            ivf_fhRaw.timeStamp = 0;

            //memset(inputVideoBuffer,0,sizeof(inputVideoBuffer));

            //if(!feof(in))
            //{
            //  fread(&ivf_fhRaw.frameSize,1,4,in);
            //  fread(&ivf_fhRaw.timeStamp,1,8,in);
            //  vpxt_format_frame_header_read(ivf_fhRaw);
            //  fread(inputVideoBuffer,1,ivf_fhRaw.frameSize,in);

            //  /*  if(currentVideoFrame != frameCount)
            //  {
            //  tprintf(  PRINT_STD, "FRAME HEADER %i\n\n"
            //  "Frame Size            - %i \n"
            //  "Time Stamp            - %i \n"
            //  "\n"

            //  ,currentVideoFrame,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);
            //  }*/
            //}
            //else
            //{
            //  break;
            //}
        }
        else
        {
            if (CharCount == 79)
            {
                tprintf(PRINT_STD, "\n");
                CharCount = 0;
            }

            tprintf(PRINT_STD, ".");

            CharCount++;

            if (!feof(in))
            {
                fread(&ivf_fhRaw.frameSize, 1, 4, in);
                fread(&ivf_fhRaw.timeStamp, 1, 8, in);
                vpxt_format_frame_header_read(ivf_fhRaw);
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
    vpxt_format_ivf_header_write(ivfhCompressed);
    fwrite(&ivfhCompressed, 1, 32, out);

    fclose(in);
    fclose(out);

    delete [] inputVideoBuffer;

    return(0);
}
int vpxt_crop_raw_ivf(const char *inputFile, const char *outputFile, int xoffset, int yoffset, int newFrameWidth, int newFrameHeight, int FileIsIVF, int OutputToFile)
{
    bool verbose = 1;

    if (OutputToFile != 1)
    {
        OutputToFile = 0;
    }


    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");

        if (OutputToFile)
        {
            fprintf(stderr, "\nInput file does not exist");
        }

        return 0;
    }

    FILE *out = fopen(outputFile, "wb");

    if (out == NULL)
    {
        tprintf(PRINT_STD, "\nOutput file does not exist");

        if (OutputToFile)
        {
            fprintf(stderr, "\nOutput file does not exist");
        }

        fclose(in);
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
    fread(&ivfhRaw.four_cc, 1, 4, in);
    fread(&ivfhRaw.width, 1, 2, in);
    fread(&ivfhRaw.height, 1, 2, in);
    fread(&ivfhRaw.rate, 1, 4, in);
    fread(&ivfhRaw.scale, 1, 4, in);
    fread(&ivfhRaw.length, 1, 4, in);
    fread(&storage, 1, 4, in);
    vpxt_format_ivf_header_read(&ivfhRaw);

    img_fmt_t vidFormat = IMG_FMT_NONE;

    int flipuv = 0;


    vidFormat = IMG_FMT_I420;


    if (vidFormat == IMG_FMT_NONE)
    {
        tprintf(PRINT_STD, "\n Video Format not found.  Currently supported: I420 and YV12.\n");

        if (OutputToFile)
        {
            fprintf(stderr, "\n Video Format not found.  Currently supported: I420 and YV12.\n");
        }

        return 0;
    }

    unsigned char *inputVideoBuffer = new unsigned char [ivfhRaw.width * ivfhRaw.height*3/2];

    IVF_FRAME_HEADER ivf_fhRaw;

    IVF_HEADER  ivfhCropped;
    InitIVFHeader(& ivfhCropped);
    memset(& ivfhCropped, 0, sizeof(ivfhCropped));
    ivfhCropped = ivfhRaw;

    IVF_FRAME_HEADER ivf_fhCropped;
    vpxt_format_ivf_header_write(ivfhCropped);

    if (FileIsIVF == 1)
    {
        fwrite((char *)& ivfhCropped, 1, 32, out);
    };

    vpxt_format_ivf_header_read(& ivfhCropped);

    tprintf(PRINT_STD, "\nCrop IVF file to ");

    if (OutputToFile)
    {
        fprintf(stderr, "\nCrop IVF file to ");
    }

    if (FileIsIVF == 0)
    {
        tprintf(PRINT_STD, "RAW file: \n");

        if (OutputToFile)
        {
            fprintf(stderr, "RAW file: \n");
        }
    }

    if (FileIsIVF == 1)
    {
        tprintf(PRINT_STD, "IVF file: \n");

        if (OutputToFile)
        {
            fprintf(stderr, "IVF file: \n");
        }
    }

    int frameCount = ivfhRaw.length;

    currentVideoFrame = 0;

    int WrittenoutVideoFrames = 0;

    int NewTimeStamp = 0;

    while (currentVideoFrame < frameCount)
    {
        memset(inputVideoBuffer, 0, sizeof(inputVideoBuffer));

        vpx_image_t img;
        vpx_img_alloc(&img, IMG_FMT_I420, ivfhRaw.width, ivfhRaw.height, 1);

        if (!feof(in))
        {
            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            vpxt_format_frame_header_read(ivf_fhRaw);
            read_frame_enc(in, &img, ivf_fhRaw.frameSize);
        }
        else
        {
            break;
        }

        if (vpx_img_set_rect(&img, xoffset, yoffset, newFrameWidth, newFrameHeight) != 0)
        {
            tprintf(PRINT_STD, "ERROR: INVALID RESIZE\n");

            if (OutputToFile)
            {
                fprintf(stderr, "ERROR: INVALID RESIZE\n");
            }

            break;
        }

        ivfhCropped.height = newFrameHeight;
        ivfhCropped.width = newFrameWidth;

        ivf_fhRaw.frameSize = newFrameWidth * newFrameHeight + ((newFrameWidth + 1) / 2) * ((newFrameHeight + 1) / 2) + ((newFrameWidth + 1) / 2) * ((newFrameHeight + 1) / 2);

        memset(&ivf_fhCropped, 0, sizeof(ivf_fhCropped));

        unsigned int FrameSizeStorage = ivf_fhRaw.frameSize;
        ivf_fhCropped.frameSize = ivf_fhRaw.frameSize;
        ivf_fhCropped.timeStamp = NewTimeStamp;

        vpxt_format_frame_header_write(ivf_fhCropped);

        unsigned int y;
        char out_fn[128+24];
        uint8_t *buf;
        const char *sfx = 0;
        int do_md5 = 0;

        if (FileIsIVF == 1)
        {
            ivf_write_frameAPI(out, ivf_fhCropped.timeStamp, ivf_fhCropped.frameSize);
        }

        int size = 0;
        int offset = 0;

        /*    size = ftell(out);
        offset = size;
        tprintf(  PRINT_STD, "\nStart = %i\nPredicted Y: %i\nPredicted U: %i\nPredicted V: %i\n", size, newFrameWidth * newFrameHeight, ((newFrameWidth + 1) / 2)*((newFrameHeight + 1) / 2), ((newFrameWidth + 1) / 2)*((newFrameHeight + 1) / 2));*/

        buf = img.planes[PLANE_Y];

        for (y = 0; y < img.d_h; y++)
        {
            out_put(out, buf, img.d_w, do_md5);
            buf += img.stride[PLANE_Y];
        }

        /*size = ftell(out);
        tprintf(  PRINT_STD, "\YPlane = %i\n", size - offset);
        offset = size;*/

        buf = img.planes[flipuv?PLANE_V:PLANE_U];

        for (y = 0; y < (1 + img.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + img.d_w) / 2, do_md5);
            buf += img.stride[PLANE_U];
        }

        /* size = ftell(out);
        tprintf(  PRINT_STD, "UPlane = %i\n", size - offset);
        offset = size;*/

        buf = img.planes[flipuv?PLANE_U:PLANE_V];

        for (y = 0; y < (1 + img.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + img.d_w) / 2, do_md5);
            buf += img.stride[PLANE_V];
        }

        /*size = ftell(out);
        tprintf(  PRINT_STD, "VPlane = %i\n", size - offset);
        offset = size;*/

        if (CharCount == 79)
        {
            tprintf(PRINT_STD, "\n");

            if (OutputToFile)
            {
                fprintf(stderr, "\n");
            }

            CharCount = 0;
        }

        tprintf(PRINT_STD, ".");

        if (OutputToFile)
        {
            fprintf(stderr, ".");
        }

        ++currentVideoFrame;
        WrittenoutVideoFrames++;
        CharCount++;
        NewTimeStamp = NewTimeStamp + 2;

        ivf_fhRaw.frameSize = 0;
        ivf_fhRaw.timeStamp = 0;

        vpx_img_free(&img);
    }

    tprintf(PRINT_STD, "\n");

    if (OutputToFile)
    {
        fprintf(stderr, "\n");
    }

    ivfhCropped.length = WrittenoutVideoFrames;

    if (FileIsIVF == 1)
    {
        fseek(out , 0 , SEEK_SET);
        vpxt_format_ivf_header_write(ivfhCropped);
        fwrite(& ivfhCropped, 1, 32, out);
    }

    fclose(in);
    fclose(out);

    delete [] inputVideoBuffer;

    return(0);
}
int vpxt_paste_ivf(const char *inputFile1, const char *inputFile2, const char *outputFile, int StartingFrame)
{
    bool verbose = 1;


    FILE *in1 = fopen(inputFile1, "rb");

    if (in1 == NULL)
    {
        tprintf(PRINT_STD, "\nInput file 1 does not exist");
        return 0;
    }

    FILE *in2 = fopen(inputFile2, "rb");

    if (in2 == NULL)
    {
        tprintf(PRINT_STD, "\nInput file 2 does not exist");
        fclose(in1);
        return 0;
    }

    FILE *out = fopen(outputFile, "wb");

    if (out == NULL)
    {
        tprintf(PRINT_STD, "\nOutput file does not exist");
        fclose(in1);
        fclose(in2);
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
    fread(&ivfhRaw1.four_cc, 1, 4, in1);
    fread(&ivfhRaw1.width, 1, 2, in1);
    fread(&ivfhRaw1.height, 1, 2, in1);
    fread(&ivfhRaw1.rate, 1, 4, in1);
    fread(&ivfhRaw1.scale, 1, 4, in1);
    fread(&ivfhRaw1.length, 1, 4, in1);
    fread(&storage, 1, 4, in1);
    vpxt_format_ivf_header_read(&ivfhRaw1);

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
    "Unused                 - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/
    IVF_FRAME_HEADER ivf_fhRaw1;
    fread(&ivf_fhRaw1.frameSize, 1, 4, in1);
    fread(&ivf_fhRaw1.timeStamp, 1, 8, in1);
    vpxt_format_frame_header_read(ivf_fhRaw1);
    fread(inputVideoBuffer1, 1, ivf_fhRaw1.frameSize, in1);

    IVF_HEADER ivfhRaw2;

    InitIVFHeader(&ivfhRaw2);
    memset(&ivfhRaw2, 0, sizeof(ivfhRaw2));
    fread(&ivfhRaw2.signature, 1, 4, in2);
    fread(&ivfhRaw2.version, 1, 2, in2);
    fread(&ivfhRaw2.headersize, 1, 2, in2);
    fread(&ivfhRaw2.four_cc, 1, 4, in2);
    fread(&ivfhRaw2.width, 1, 2, in2);
    fread(&ivfhRaw2.height, 1, 2, in2);
    fread(&ivfhRaw2.rate, 1, 4, in2);
    fread(&ivfhRaw2.scale, 1, 4, in2);
    fread(&ivfhRaw2.length, 1, 4, in2);
    fread(&storage, 1, 4, in2);
    vpxt_format_ivf_header_read(&ivfhRaw2);

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
    "Unused                 - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/
    IVF_FRAME_HEADER ivf_fhRaw2;
    fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
    fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
    vpxt_format_frame_header_read(ivf_fhRaw2);
    fread(inputVideoBuffer2, 1, ivf_fhRaw2.frameSize, in2);


    /*tprintf(  PRINT_STD, "FRAME HEADER 0\n\n"
    "Frame Size            - %i \n"
    "Time Stamp            - %i \n"
    "\n"

    ,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);*/

    IVF_HEADER ivfhCompressed;
    InitIVFHeader(&ivfhCompressed);
    memset(&ivfhCompressed, 0, sizeof(ivfhCompressed));
    ivfhCompressed = ivfhRaw1;
    IVF_FRAME_HEADER ivf_fhCompressed;
    vpxt_format_ivf_header_write(ivfhCompressed);
    fwrite((char *)&ivfhCompressed, 1, 32, out);
    vpxt_format_ivf_header_read(&ivfhCompressed);
    tprintf(PRINT_STD, "\nPasting IVF file into IVF file: \n");

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
            ivf_fhCompressed.timeStamp = ivf_fhRaw2.timeStamp;//0;      // not defined, use rate and scale in header replaced by line: memset(&ivf_fh, 0, sizeof(ivf_fh));

            vpxt_format_frame_header_write(ivf_fhCompressed);
            fwrite(&ivf_fhCompressed.frameSize, 1, sizeof(ivf_fhCompressed.frameSize), out);
            fwrite(&ivf_fhCompressed.timeStamp, 1, sizeof(ivf_fhCompressed.timeStamp), out);
            fwrite(inputVideoBuffer2, 1, FrameSizeStorage, out);


            if (CharCount == 79)
            {
                tprintf(PRINT_STD, "\n");
                CharCount = 0;
            }

            tprintf(PRINT_STD, "+");
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
                vpxt_format_frame_header_read(ivf_fhRaw2);
                fread(inputVideoBuffer2, 1, ivf_fhRaw2.frameSize, in2);

                /*  if(currentVideoFrame != frameCount)
                {
                tprintf(  PRINT_STD, "FRAME HEADER %i\n\n"
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
            ivf_fhCompressed.timeStamp = ivf_fhRaw1.timeStamp;//0;      // not defined, use rate and scale in header replaced by line: memset(&ivf_fh, 0, sizeof(ivf_fh));

            vpxt_format_frame_header_write(ivf_fhCompressed);
            fwrite(&ivf_fhCompressed.frameSize, 1, sizeof(ivf_fhCompressed.frameSize), out);
            fwrite(&ivf_fhCompressed.timeStamp, 1, sizeof(ivf_fhCompressed.timeStamp), out);
            fwrite(inputVideoBuffer1, 1, FrameSizeStorage, out);


            if (CharCount == 79)
            {
                tprintf(PRINT_STD, "\n");
                CharCount = 0;
            }

            tprintf(PRINT_STD, ".");
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
                vpxt_format_frame_header_read(ivf_fhRaw1);
                fread(inputVideoBuffer1, 1, ivf_fhRaw1.frameSize, in1);

                /*  if(currentVideoFrame != frameCount)
                {
                tprintf(  PRINT_STD, "FRAME HEADER %i\n\n"
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
    vpxt_format_ivf_header_write(ivfhCompressed);
    fwrite(&ivfhCompressed, 1, 32, out);

    fclose(in1);
    fclose(in2);
    fclose(out);

    delete [] inputVideoBuffer1;
    delete [] inputVideoBuffer2;

    return(0);
}
int vpxt_ivf_to_raw(const std::string inputFile, const std::string outputDir)
{
    FILE *in = fopen(inputFile.c_str(), "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame = 1;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);

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
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.fourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    fread(&ivf_fhRaw.frameSize, 1, 4, in);
    fread(&ivf_fhRaw.timeStamp, 1, 8, in);
    vpxt_format_frame_header_read(ivf_fhRaw);

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

    tprintf(PRINT_STD, "\n");

    std::string OutputDirStrwithQuotes = outputDir;

    char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

    std::string outputDirStr2 = outputDir;
    char outputDirChar2[255];

    snprintf(outputDirChar2, 255, "%s", outputDirStr2.c_str());

    FILE *out2 = fopen(outputDirChar2, "wb");
    ///////////////////////////////////////////////////////////////////////////////////////
    int LastFrameDecPlaces = vpxt_decimal_places(ivfhRaw.length);

    ///////////////////////////////////////////////////////////////////////////////////////
    while (currentVideoFrame <= frameCount)
    {
        /*fprintf(stderr, "FRAME HEADER 0\n"
        "Frame Size            - %i \n"
        "Time Stamp            - %i \n"
        "\n"
        ,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);*/

        tprintf(PRINT_STD, ".");
        memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        std::string outputDirStr = outputDir;

        char currentVideoFrameStr[10];
        char widthchar[10];
        char heightchar[10];
        vpxt_itoa_custom(currentVideoFrame, currentVideoFrameStr, 10);
        vpxt_itoa_custom(ivfhRaw.width, widthchar, 10);
        vpxt_itoa_custom(ivfhRaw.height, heightchar, 10);

        outputDirStr.append(slashCharStr());
        char InputFileName2[255];
        vpxt_file_name(inputFile.c_str(), InputFileName2, 0);
        std::string InputFileNameStr2 = InputFileName2;

        if (InputFileNameStr2.substr(InputFileNameStr2.length() - 4, 1).compare(".") == 0) //if file extension is present remove it
            outputDirStr.append(InputFileNameStr2.substr(0, InputFileNameStr2.length() - 4));
        else
            outputDirStr.append(InputFileName2);

        outputDirStr.append("-");
        outputDirStr.append(widthchar);
        outputDirStr.append("x");
        outputDirStr.append(heightchar);
        outputDirStr.append("-Frame-");
        ///////////////////////////////////////////////////////////////////////////////////////
        int CurNumDecPlaces = vpxt_decimal_places(currentVideoFrame);

        while (CurNumDecPlaces < LastFrameDecPlaces) //add zeros for increasing frames
        {
            //printf("%i < %i ", CurNumDecPlaces,InputDecPlaces);
            outputDirStr.append("0");
            CurNumDecPlaces++;
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        outputDirStr.append(currentVideoFrameStr);
        outputDirStr.append(".raw");

        char outputDirChar[255];
        snprintf(outputDirChar, 255, "%s", outputDirStr.c_str());

        fwrite(inbuff, 1, ivf_fhRaw.frameSize, out2);

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        currentVideoFrame ++;
    }

    fclose(in);
    fclose(out2);

    return 0;
}
int vpxt_ivf_to_raw_frames(const std::string inputFile, const std::string outputDir)
{
    FILE *in = fopen(inputFile.c_str(), "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame = 1;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);

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
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.fourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    fread(&ivf_fhRaw.frameSize, 1, 4, in);
    fread(&ivf_fhRaw.timeStamp, 1, 8, in);
    vpxt_format_frame_header_read(ivf_fhRaw);

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

    tprintf(PRINT_STD, "\n");

    std::string OutputDirStrwithQuotes = outputDir;


    OutputDirStrwithQuotes.append("\"");
    OutputDirStrwithQuotes.insert(0, "md \"");
    vpxt_make_dir_vpx(OutputDirStrwithQuotes);


    char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

    std::string outputDirStr2 = outputDir;
    char outputDirChar2[255];


    outputDirStr2.append(slashCharStr());

    char InputFileName[255];
    vpxt_file_name(inputFile.c_str(), InputFileName, 0);
    std::string InputFileNameStr = InputFileName;

    if (InputFileNameStr.substr(InputFileNameStr.length() - 4, 1).compare(".") == 0) //if file extension is present remove it
        outputDirStr2.append(InputFileNameStr.substr(0, InputFileNameStr.length() - 4));
    else
        outputDirStr2.append(InputFileName);

    outputDirStr2.append(".raw");
    snprintf(outputDirChar2, 255, "%s", outputDirStr2.c_str());


    FILE *out2 = fopen(outputDirChar2, "wb");
    ///////////////////////////////////////////////////////////////////////////////////////
    int LastFrameDecPlaces = vpxt_decimal_places(ivfhRaw.length);

    ///////////////////////////////////////////////////////////////////////////////////////
    while (currentVideoFrame <= frameCount)
    {
        /*fprintf(stderr, "FRAME HEADER 0\n"
        "Frame Size            - %i \n"
        "Time Stamp            - %i \n"
        "\n"
        ,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);*/

        tprintf(PRINT_STD, ".");
        memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        std::string outputDirStr = outputDir;

        char currentVideoFrameStr[10];
        char widthchar[10];
        char heightchar[10];
        vpxt_itoa_custom(currentVideoFrame, currentVideoFrameStr, 10);
        vpxt_itoa_custom(ivfhRaw.width, widthchar, 10);
        vpxt_itoa_custom(ivfhRaw.height, heightchar, 10);

        outputDirStr.append(slashCharStr());
        char InputFileName2[255];
        vpxt_file_name(inputFile.c_str(), InputFileName2, 0);
        std::string InputFileNameStr2 = InputFileName2;

        if (InputFileNameStr2.substr(InputFileNameStr2.length() - 4, 1).compare(".") == 0) //if file extension is present remove it
            outputDirStr.append(InputFileNameStr2.substr(0, InputFileNameStr2.length() - 4));
        else
            outputDirStr.append(InputFileName2);

        outputDirStr.append("-");
        outputDirStr.append(widthchar);
        outputDirStr.append("x");
        outputDirStr.append(heightchar);
        outputDirStr.append("-Frame-");
        ///////////////////////////////////////////////////////////////////////////////////////
        int CurNumDecPlaces = vpxt_decimal_places(currentVideoFrame);

        while (CurNumDecPlaces < LastFrameDecPlaces) //add zeros for increasing frames
        {
            //printf("%i < %i ", CurNumDecPlaces,InputDecPlaces);
            outputDirStr.append("0");
            CurNumDecPlaces++;
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        outputDirStr.append(currentVideoFrameStr);
        outputDirStr.append(".raw");

        char outputDirChar[255];
        snprintf(outputDirChar, 255, "%s", outputDirStr.c_str());

        FILE *out = fopen(outputDirChar, "wb");
        fwrite(inbuff, 1, ivf_fhRaw.frameSize, out);
        fwrite(inbuff, 1, ivf_fhRaw.frameSize, out2);
        fclose(out);

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        currentVideoFrame ++;
    }

    fclose(in);
    fclose(out2);

    return 0;
}
int vpxt_display_ivf_header_info(int argc, const char *argv[])
{
    if (argc < 3)
    {
        tprintf(PRINT_STD, "\n"
                "  DisplayIVFHeaderInfo \n\n"
                "    <Input File>\n"
                "    <Full File Info 1/yes-0/no>\n"
                "    <Optional Outputfile>\n\n"
               );
        return 0;
    }

    char inputFile[256] = "";
    strncpy(inputFile, argv[2], 256);
    int extrafileinfo = 0;

    if (argc > 3)
    {
        extrafileinfo = atoi(argv[3]);
    }


    if (argc > 4)
    {
        char outputFile[256] = "";
        strncpy(outputFile, argv[4], 256);

        FILE *fp;

        if ((fp = freopen(outputFile, "w", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n", outputFile);
            exit(1);
        }

        FILE *in = fopen(inputFile, "rb");

        if (in == NULL)
        {
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

        tprintf(PRINT_BTH, "\n"
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
                , ivfhRaw.version, ivfhRaw.headersize, ivfhRaw.four_cc, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate
                , ivfhRaw.scale, ivfhRaw.length, ivfhRaw.unused);

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);

        tprintf(PRINT_BTH, "FRAME HEADER 0\n"
                "Frame Size            - %i \n"
                "Time Stamp            - %i \n"
                "\n"

                , ivf_fhRaw.frameSize, (int)ivf_fhRaw.timeStamp);

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

            vpxt_format_frame_header_read(ivf_fhRaw);

            fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);

            if (feof(in))break;

            tprintf(PRINT_BTH, "FRAME HEADER %i\n"
                    "Frame Size            - %i \n"
                    "Time Stamp            - %i \n"
                    "\n"

                    , currentVideoFrame, ivf_fhRaw.frameSize, (int)ivf_fhRaw.timeStamp);

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
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);

        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

        tprintf(PRINT_STD, "\n"
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
                , ivfhRaw.version, ivfhRaw.headersize, ivfhRaw.four_cc, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate
                , ivfhRaw.scale, ivfhRaw.length, ivfhRaw.unused);

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        tprintf(PRINT_STD, "FRAME HEADER 0\n"
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

            vpxt_format_frame_header_read(ivf_fhRaw);

            fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);

            if (feof(in))break;

            tprintf(PRINT_STD, "FRAME HEADER %i\n"
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
int vpxt_compare_ivf_header_info(int argc, const char *argv[])
{
    if (argc < 5)
    {
        tprintf(PRINT_STD, "\n"
                "  CompareIVFHeaderInfo \n\n"
                "    <inputfile1>\n"
                "    <inputfile2>\n"
                "    <Full File Info 1/yes-0/no>\n"
                "    <OutputFile>\n"
                "\n ");

        return 0;
    }

    const char *inputFile1 = argv[2];

    const char *inputFile2 = argv[3];

    int extrafileinfo;

    if (argc > 3)
    {
        extrafileinfo = atoi(argv[4]);
    }
    else
    {
        extrafileinfo = 0;
    }

    std::string strinputFile1 = inputFile1;
    std::string strinputFile2 = inputFile2;

    FILE *in = fopen(inputFile1, "rb");


    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file 1 does not exist");
        return 0;
    }

    FILE *in2 = fopen(inputFile2, "rb");

    if (in2 == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file 2 does not exist");
        fclose(in);
        return 0;
    }

    if (argc < 6) //if not output file
    {
        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);

        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

        IVF_HEADER ivfhRaw2;

        InitIVFHeader(&ivfhRaw2);

        fread(&ivfhRaw2, 1, sizeof(ivfhRaw2), in2);
        vpxt_format_ivf_header_read(&ivfhRaw2);

        tprintf(PRINT_STD, "\n"
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
                , 12, ivfhRaw.four_cc, ivfhRaw2.four_cc
                , 12, ivfhRaw.width, ivfhRaw2.width
                , 12, ivfhRaw.height, ivfhRaw2.height
                , 12, ivfhRaw.rate, ivfhRaw2.rate
                , 12, ivfhRaw.scale, ivfhRaw2.scale
                , 12, ivfhRaw.length, ivfhRaw2.length
                , 12, ivfhRaw.unused, ivfhRaw2.unused);

        IVF_FRAME_HEADER ivf_fhRaw;


        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);



        IVF_FRAME_HEADER ivf_fhRaw2;

        fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
        fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
        vpxt_format_frame_header_read(ivf_fhRaw2);

        fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);

        int currentVideoFrame = 0;

        tprintf(PRINT_STD, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
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
                vpxt_format_frame_header_read(ivf_fhRaw);

                fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount2)
            {
                fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
                fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
                vpxt_format_frame_header_read(ivf_fhRaw2);

                fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount1 && currentVideoFrame <= frameCount2)
            {
                tprintf(PRINT_STD, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
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
                    tprintf(PRINT_STD,
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
                    tprintf(PRINT_STD,
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

        const char *outputfile = argv[5];

        FILE *fp;

        if ((fp = freopen(outputfile, "w", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n", outputfile);
            exit(1);
        }

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);

        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

        IVF_HEADER ivfhRaw2;

        InitIVFHeader(&ivfhRaw2);

        fread(&ivfhRaw2, 1, sizeof(ivfhRaw2), in2);
        vpxt_format_ivf_header_read(&ivfhRaw2);

        tprintf(PRINT_STD, "\n"
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
                , 12, ivfhRaw.four_cc, ivfhRaw2.four_cc
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
                , 12, ivfhRaw.four_cc, ivfhRaw2.four_cc
                , 12, ivfhRaw.width, ivfhRaw2.width
                , 12, ivfhRaw.height, ivfhRaw2.height
                , 12, ivfhRaw.rate, ivfhRaw2.rate
                , 12, ivfhRaw.scale, ivfhRaw2.scale
                , 12, ivfhRaw.length, ivfhRaw2.length
                , 12, ivfhRaw.unused, ivfhRaw2.unused);

        IVF_FRAME_HEADER ivf_fhRaw;


        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);



        IVF_FRAME_HEADER ivf_fhRaw2;

        fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
        fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
        vpxt_format_frame_header_read(ivf_fhRaw2);

        fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);

        int currentVideoFrame = 0;

        std::cerr << "\n Time 2 Stamp: " << ivf_fhRaw2.timeStamp << "\n";

        tprintf(PRINT_BTH, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
                "Frame Size            - %-*iFrame Size            - %i\n"
                "Time Stamp            - %-*iTime Stamp            - %-*i\n"
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
                vpxt_format_frame_header_read(ivf_fhRaw);

                fseek(in, ivf_fhRaw.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount2)
            {
                fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
                fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
                vpxt_format_frame_header_read(ivf_fhRaw2);

                fseek(in2, ivf_fhRaw2.frameSize, SEEK_CUR);
            }

            if (currentVideoFrame <= frameCount1 && currentVideoFrame <= frameCount2)
            {
                tprintf(PRINT_BTH, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
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
                    tprintf(PRINT_STD,
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
                    tprintf(PRINT_BTH,
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
int vpxt_compare_ivf(const char *inputFile1, const char *inputFile2)
{
    ////Returns:
    //-1 if files are identical
    //-2 if file 2 ends before File 1
    //-3 if file 1 ends before File 2
    //and >= 0 where the number the function returns is the frame that they differ first on.

    FILE *in1 = fopen(inputFile1, "rb");

    int returnval = -1;

    if (in1 == NULL)
    {
        tprintf(PRINT_BTH, "\nFile 1 does not exist");
        return 0;
    }

    FILE *in2 = fopen(inputFile2, "rb");

    if (in2 == NULL)
    {
        tprintf(PRINT_BTH, "\nFile 2 does not exist");
        fclose(in1);
        return 0;
    }

    int currentVideoFrame = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, 32, in1);
    vpxt_format_ivf_header_read(&ivfhRaw);

    IVF_HEADER ivfhRaw2;

    InitIVFHeader(&ivfhRaw2);
    fread(&ivfhRaw2, 1, 32, in2);
    vpxt_format_ivf_header_read(&ivfhRaw2);

    unsigned char *inputVideoBuffer1 = new unsigned char [ivfhRaw.width * ivfhRaw.height*3];
    unsigned char *inputVideoBuffer2 = new unsigned char [ivfhRaw2.width * ivfhRaw2.height*3];
    memset(inputVideoBuffer1, 0, ivfhRaw.width * ivfhRaw.height * 3);
    memset(inputVideoBuffer2, 0, ivfhRaw2.width * ivfhRaw2.height * 3);

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
    //,12,ivfhRaw.four_cc,ivfhRaw2.four_cc
    //,12,ivfhRaw.width,ivfhRaw2.width
    //,12,ivfhRaw.height,ivfhRaw2.height
    //,12,ivfhRaw.rate,ivfhRaw2.rate
    //,12,ivfhRaw.scale,ivfhRaw2.scale
    //,12,ivfhRaw.length,ivfhRaw2.length
    //,12,ivfhRaw.unused,ivfhRaw2.unused);
    ////////////////////////////////////////////////////////////////////////

    if (ivfhRaw.signature[0] != ivfhRaw2.signature[0])
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.signature[1] != ivfhRaw2.signature[1])
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.signature[2] != ivfhRaw2.signature[2])
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.signature[3] != ivfhRaw2.signature[3])
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.version != ivfhRaw2.version)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.headersize != ivfhRaw2.headersize)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.four_cc != ivfhRaw2.four_cc)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.width != ivfhRaw2.width)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.height != ivfhRaw2.height)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.rate != ivfhRaw2.rate)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.scale != ivfhRaw2.scale)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivfhRaw.length != ivfhRaw2.length)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    IVF_FRAME_HEADER ivf_fhRaw1;

    fread(&ivf_fhRaw1.frameSize, 1, 4, in1);
    fread(&ivf_fhRaw1.timeStamp, 1, 8, in1);
    vpxt_format_frame_header_read(ivf_fhRaw1);

    fread(inputVideoBuffer1, 1, ivf_fhRaw1.frameSize, in1);

    IVF_FRAME_HEADER ivf_fhRaw2;

    fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
    fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
    vpxt_format_frame_header_read(ivf_fhRaw2);

    fread(inputVideoBuffer2, 1, ivf_fhRaw2.frameSize, in2);

    //////////////////////////////////OutPut////////////////////////////////
    //tprintf(  PRINT_STD, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
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
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    if (ivf_fhRaw2.timeStamp != ivf_fhRaw1.timeStamp)
    {
        returnval = -2;
    }

    if (memcmp(inputVideoBuffer1, inputVideoBuffer2, ivf_fhRaw1.frameSize) != 0)
    {
        fclose(in1);
        fclose(in2);
        delete [] inputVideoBuffer1;
        delete [] inputVideoBuffer2;
        return currentVideoFrame;
    }

    while (!feof(in1) || !feof(in2))
    {
        // if one file ends and the other doesnt exit
        if (feof(in1) && !feof(in2))
        {
            fclose(in1);
            fclose(in2);
            delete [] inputVideoBuffer1;
            delete [] inputVideoBuffer2;
            return -2;
        }

        if (!feof(in1) && feof(in2))
        {
            fclose(in1);
            fclose(in2);
            delete [] inputVideoBuffer1;
            delete [] inputVideoBuffer2;
            return -3;
        }

        if (currentVideoFrame == ivfhRaw2.length)
        {
            break;
        }

        memset(inputVideoBuffer1, 0, ivfhRaw.width * ivfhRaw.height * 3);
        memset(inputVideoBuffer2, 0, ivfhRaw2.width * ivfhRaw2.height * 3);

        fread(&ivf_fhRaw1.frameSize, 1, 4, in1);
        fread(&ivf_fhRaw1.timeStamp, 1, 8, in1);
        vpxt_format_frame_header_read(ivf_fhRaw1);
        fread(inputVideoBuffer1, 1, ivf_fhRaw1.frameSize, in1);

        fread(&ivf_fhRaw2.frameSize, 1, 4, in2);
        fread(&ivf_fhRaw2.timeStamp, 1, 8, in2);
        vpxt_format_frame_header_read(ivf_fhRaw2);
        fread(inputVideoBuffer2, 1, ivf_fhRaw2.frameSize, in2);

        //////////////////////////////////OutPut////////////////////////////////
        //tprintf(  PRINT_STD, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
        //"Frame Size            - %-*iFrame Size            - %i\n"
        //"Time Stamp            - %-*iTime Stamp            - %i\n"
        //"\n"

        //,22,currentVideoFrame,currentVideoFrame
        //,12,ivf_fhRaw1.frameSize,ivf_fhRaw2.frameSize
        //,12,ivf_fhRaw1.timeStamp,ivf_fhRaw2.timeStamp);
        ////////////////////////////////////////////////////////////////////////

        if (ivf_fhRaw2.frameSize != ivf_fhRaw1.frameSize)
        {
            fclose(in1);
            fclose(in2);
            //std::cout << "Frame Size not equal";
            delete [] inputVideoBuffer1;
            delete [] inputVideoBuffer2;
            return currentVideoFrame + 1;
        }

        if (ivf_fhRaw2.timeStamp != ivf_fhRaw1.timeStamp)
        {
            returnval = -2;
        }

        if (memcmp(inputVideoBuffer1, inputVideoBuffer2, ivf_fhRaw1.frameSize) != 0)
        {

            fclose(in1);
            fclose(in2);
            delete [] inputVideoBuffer1;
            delete [] inputVideoBuffer2;
            return currentVideoFrame + 1;
        }

        currentVideoFrame++;
    }

    fclose(in1);
    fclose(in2);
    delete [] inputVideoBuffer1;
    delete [] inputVideoBuffer2;

    return returnval;
}
double vpxt_display_droped_frames(const char *inputchar, int PrintSwitch)
{
    int dropedframecount = 0;

    std::string DropedInStr = inputchar;
    DropedInStr.erase(DropedInStr.length() - 4, 4);
    DropedInStr.append("_aprox_droped_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", DropedInStr.c_str());


    FILE *in = fopen(inputchar, "rb");
    FILE *out;

    ///////////////////////////////////
    //long PosSize = vpxt_file_size(inputFile, 0);

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");

        if (out == NULL)
        {
            tprintf(PRINT_BTH, "\nOutput file does not exist");
            fclose(in);
            return 0;
        }

        //std::ofstream outfile(outputFile);
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);

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
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    fread(&ivf_fhRaw.frameSize, 1, 4, in);
    fread(&ivf_fhRaw.timeStamp, 1, 8, in);
    vpxt_format_frame_header_read(ivf_fhRaw);

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
        vpxt_format_frame_header_read(ivf_fhRaw);

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
double vpxt_display_resized_frames(const char *inputchar, int PrintSwitch)
{
    //PrintSwitch == 0 -> Print to screen
    //PrintSwitch == 1 -> Print to file

    std::string ResizeInStr = inputchar;
    ResizeInStr.erase(ResizeInStr.length() - 4, 4);
    ResizeInStr.append("_resized_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", ResizeInStr.c_str());

    FILE *out;

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");
    }

    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    vpx_codec_dec_cfg_t     cfg = {0};
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};

    int CharCount = 0;

    FILE *in = fopen(fn, "rb");
    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);
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

        tprintf(PRINT_STD, "Failed to open file");
        return EXIT_FAILURE;
    }

    is_ivf = file_is_ivf(infile, &fourcc);

    if (is_ivf)
    {
        /* Try to determine the codec from the fourcc. */
        for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
            if ((fourcc & ifaces[i].fourcc_mask) == ifaces[i].fourcc)
            {
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
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
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        if (progress)
            tprintf(PRINT_STD, "decoded frame %d.\n", frame);



        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
            {
                if (img->d_w != width || img->d_h != height) //CheckFrameSize
                {
                    if (PrintSwitch == 0)
                    {
                        tprintf(PRINT_STD, "%i %i %i \n", frame, img->d_w, img->d_h);
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
        tprintf(PRINT_STD, "Decoded %d frames in %lu us (%.2f fps)\n",
                frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
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
double vpxt_display_visible_frames(const char *inputFile, int Selector)
{
    // 0 = just display
    // 1 = write to file
    std::string VisInStr = inputFile;
    VisInStr.erase(VisInStr.length() - 4, 4);
    VisInStr.append("_visible_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", VisInStr.c_str());

    int VisableCount = 0;

    if (Selector == 0)
    {
        FILE *in = fopen(inputFile, "rb");
        ///////////////////////////////////
        long PosSize = vpxt_file_size(inputFile, 0);

        if (in == NULL)
        {
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

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
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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
        std::cout << "\n";

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
            unsigned int showFrame = oz.show_frame;
            unsigned int version = oz.version;

            if (showFrame == 1)
            {
                VisableCount++;
                std::cout << "\n" << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            vpxt_format_frame_header_read(ivf_fhRaw);

            currentVideoFrame ++;
        }

        fclose(in);
        delete[] inbuff;
    }

    if (Selector == 1)
    {
        FILE *in = fopen(inputFile, "rb");
        std::ofstream outfile(outputFile);

        ///////////////////////////////////
        long PosSize = vpxt_file_size(inputFile, 0);

        if (in == NULL)
        {
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

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
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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
        //std::cout << "\n";

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
            unsigned int showFrame = oz.show_frame;
            unsigned int version = oz.version;

            if (showFrame == 1)
            {
                VisableCount++;
                outfile << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            vpxt_format_frame_header_read(ivf_fhRaw);

            currentVideoFrame ++;
        }

        outfile.close();
        fclose(in);
        delete[] inbuff;

    }

    return VisableCount;

}
double vpxt_display_alt_ref_frames(const char *inputFile, int Selector)
{
    // 0 = just display
    // 1 = write to file

    std::string AltRefInStr = inputFile;
    AltRefInStr.erase(AltRefInStr.length() - 4, 4);
    AltRefInStr.append("_alt_ref_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", AltRefInStr.c_str());

    int AltRefCount = 0;

    if (Selector == 0)
    {
        FILE *in = fopen(inputFile, "rb");
        ///////////////////////////////////
        long PosSize = vpxt_file_size(inputFile, 0);

        if (in == NULL)
        {
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

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
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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
        std::cout << "\n";

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
            unsigned int showFrame = oz.show_frame;
            unsigned int version = oz.version;

            if (showFrame == 0)
            {
                AltRefCount++;
                std::cout << "\n" << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            vpxt_format_frame_header_read(ivf_fhRaw);

            currentVideoFrame ++;
        }

        fclose(in);
        delete[] inbuff;
    }

    if (Selector == 1)
    {
        FILE *in = fopen(inputFile, "rb");
        std::ofstream outfile(outputFile);

        ///////////////////////////////////
        long PosSize = vpxt_file_size(inputFile, 0);

        if (in == NULL)
        {
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

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
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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
        //std::cout << "\n";

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
            unsigned int showFrame = oz.show_frame;
            unsigned int version = oz.version;

            if (showFrame == 0)
            {
                AltRefCount++;
                outfile << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            vpxt_format_frame_header_read(ivf_fhRaw);

            currentVideoFrame ++;
        }

        outfile.close();
        fclose(in);
        delete[] inbuff;

    }

    return AltRefCount;

}
double vpxt_display_key_frames(const char *inputFile, int Selector)
{
    int keyframecount = 0;

    std::string KeyInStr = inputFile;
    KeyInStr.erase(KeyInStr.length() - 4, 4);
    KeyInStr.append("_key_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", KeyInStr.c_str());

    if (Selector == 0)
    {
        FILE *in = fopen(inputFile, "rb");
        ///////////////////////////////////
        long PosSize = vpxt_file_size(inputFile, 0);

        if (in == NULL)
        {
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

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
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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
        std::cout << "\n";

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
            unsigned int showFrame = oz.show_frame;
            unsigned int version = oz.version;

            if (type == 0)
            {
                keyframecount++;
                std::cout << "\n" << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            vpxt_format_frame_header_read(ivf_fhRaw);

            currentVideoFrame ++;
        }

        fclose(in);
        delete[] inbuff;
    }

    if (Selector == 1)
    {
        FILE *in = fopen(inputFile, "rb");
        std::ofstream outfile(outputFile);

        ///////////////////////////////////
        long PosSize = vpxt_file_size(inputFile, 0);

        if (in == NULL)
        {
            tprintf(PRINT_BTH, "\nInput file does not exist");
            return 0;
        }

        int currentVideoFrame = 0;
        int frameCount = 0;
        int byteRec = 0;

        IVF_HEADER ivfhRaw;

        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        vpxt_format_ivf_header_read(&ivfhRaw);

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
        ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.four_cc,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
        ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

        IVF_FRAME_HEADER ivf_fhRaw;

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

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
        //std::cout << "\n";

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
            unsigned int showFrame = oz.show_frame;
            unsigned int version = oz.version;

            if (type == 0)
            {
                keyframecount++;
                outfile << currentVideoFrame << "\n";
            }

            fread(&ivf_fhRaw.frameSize, 1, 4, in);
            fread(&ivf_fhRaw.timeStamp, 1, 8, in);
            vpxt_format_frame_header_read(ivf_fhRaw);

            currentVideoFrame ++;
        }

        outfile.close();
        fclose(in);
        delete[] inbuff;

    }

    return keyframecount;
}
int vpxt_lag_in_frames_check(const char *QuantInChar)
{
    std::ifstream Quantinfile(QuantInChar);

    if (!Quantinfile.good())
    {
        tprintf(PRINT_BTH, "\nError: Cannot open file: %s\n", QuantInChar);
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
int vpxt_dfwm_check(const char *InputFile, int printselect)
{
    //return 0 = pass
    //return 1 = fail
    //return -1 = error
    //return -3 = threshold never reached

    std::string CheckPBMThreshStr = InputFile;
    CheckPBMThreshStr.erase(CheckPBMThreshStr.length() - 4, 4);
    CheckPBMThreshStr.append("_CheckPBMThresh.txt");
    char CheckPBMThreshChar[255] = "";
    snprintf(CheckPBMThreshChar, 255, "%s", CheckPBMThreshStr.c_str());

    std::string ResizeFramesStr = InputFile;
    ResizeFramesStr.erase(ResizeFramesStr.length() - 4, 4);
    ResizeFramesStr.append("_resized_frames.txt");
    char ResizeFramesChar[255] = "";
    snprintf(ResizeFramesChar, 255, "%s", ResizeFramesStr.c_str());

    std::string KeyFramesStr = InputFile;
    KeyFramesStr.erase(KeyFramesStr.length() - 4, 4);
    KeyFramesStr.append("_key_frames.txt");
    char KeyFramesChar[255] = "";
    snprintf(KeyFramesChar, 255, "%s", KeyFramesStr.c_str());


    std::ifstream CheckPBMFile(CheckPBMThreshChar);

    if (!CheckPBMFile.good())
    {
        tprintf(PRINT_BTH, "\nError: Cannot open file: %s\n", CheckPBMThreshChar);
        CheckPBMFile.close();
        return -1;
    }

    std::ifstream ResizeFramesFile(ResizeFramesChar);

    if (!ResizeFramesFile.good())
    {
        tprintf(PRINT_BTH, "\nError: Cannot open file: %s\n", ResizeFramesChar);
        ResizeFramesFile.close();
        CheckPBMFile.close();
        return -1;
    }

    std::ifstream KeyFramesFile(KeyFramesChar);

    if (!KeyFramesFile.good())
    {
        tprintf(PRINT_BTH, "\nError: Cannot open file: %s\n", KeyFramesChar);
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
        CheckPBMFile.seekg(std::ios::beg);

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
                    tprintf(PRINT_STD, "For Key Frame %4i; frame %4i under buffer level for first resized frame -Pass\n", curkeyframe, curkeyframe - 1);
                }
            }
            else
            {
                if (printselect == 1)
                {
                    tprintf(PRINT_STD, "For Key Frame %4i; frame %4i not under buffer level for first resized frame -Fail\n", curkeyframe, curkeyframe - 1);
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
                    tprintf(PRINT_STD, "For Key Frame %4i; frame %4i not under buffer level -Pass\n", curkeyframe, curkeyframe - 1);
                }
            }
            else
            {
                if (printselect == 1)
                {
                    tprintf(PRINT_STD, "For Key Frame %4i; frame %4i under buffer level -Fail\n", curkeyframe, curkeyframe - 1);
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
int vpxt_check_min_quantizer(const char *inputFile, int MinQuantizer)
{
    char QuantDispNameChar[255] = "";
    vpxt_file_name(inputFile, QuantDispNameChar, 0);

    tprintf(PRINT_BTH, "Checking %s min quantizer:\n", QuantDispNameChar);

    std::string QuantInStr = inputFile;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    std::ifstream infile(QuantInChar);

    if (!infile.good())
    {
        tprintf(PRINT_BTH, "\nError: Cannot open file: %s\n", QuantInChar);
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
                tprintf(PRINT_BTH, "     Quantizer not >= min for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }
        else
        {
            if (quantizer < MinQuantizer)
            {
                tprintf(PRINT_BTH, "     Quantizer not >= min for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }

        //std::cout << "FRAME: " << frame << " Quantizer: " << quantizer << "\n";

    }

    tprintf(PRINT_STD, "     All quantizers >= min for all frames - Passed");
    fprintf(stderr, "     All quantizers >= min for all frames - Passed");

    return -1;//result > -1 -> fail | result = -1 pass
}
int vpxt_check_max_quantizer(const char *inputFile, int MaxQuantizer)
{
    char QuantDispNameChar[255] = "";
    vpxt_file_name(inputFile, QuantDispNameChar, 0);

    tprintf(PRINT_BTH, "Checking %s max quantizer:\n", QuantDispNameChar);

    std::string QuantInStr = inputFile;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    std::ifstream infile(QuantInChar);

    if (!infile.good())
    {
        tprintf(PRINT_BTH, "\nError: Cannot open file: %s\n", QuantInChar);
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
                tprintf(PRINT_BTH, "     Quantizer not <= max for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }
        else
        {
            if (quantizer > MaxQuantizer)
            {
                tprintf(PRINT_BTH, "     Quantizer not <= max for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }

        //std::cout << "FRAME: " << frame << " Quantizer: " << quantizer << "\n";
    }

    tprintf(PRINT_BTH, " All quantizers <= max for all frames");

    return -1;//result > -1 -> fail | result = -1 pass
}
int vpxt_check_fixed_quantizer(const char *inputFile, int FixedQuantizer)
{
    char QuantDispNameChar[255] = "";
    vpxt_file_name(inputFile, QuantDispNameChar, 0);

    tprintf(PRINT_BTH, "Checking %s fixed quantizer:", QuantDispNameChar);

    std::string QuantInStr = inputFile;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    std::ifstream infile(QuantInChar);

    if (!infile.good())
    {
        tprintf(PRINT_BTH, "\nError: Cannot open file: %s\n", QuantInChar);
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
                tprintf(PRINT_BTH, " not fixed for frame %i q=%i - Failed", frame, quantizer);
                return frame;
            }
        }
        else
        {
            if (quantizer != FixedQuantizer)
            {
                tprintf(PRINT_BTH, " not fixed for frame %i q=%i", frame, quantizer);
                return frame;
            }
        }

        //std::cout << "FRAME: " << frame << " Quantizer: " << quantizer << "\n";

    }

    tprintf(PRINT_BTH, " fixed for all frames");

    return -1;//result > -1 -> fail | result = -1 pass
}
int vpxt_time_return(const char *infile, int FileType)
{
    int speed;

    char TextFilechar1[255];

    vpxt_remove_file_extension(infile, TextFilechar1);

    char *FullName = "";

    if (FileType == 0)
    {
        FullName = strcat(TextFilechar1, "compression_time.txt");
    }

    if (FileType == 1)
    {
        FullName = strcat(TextFilechar1, "decompression_time.txt");
    }

    std::ifstream infile2(FullName);

    if (!infile2.is_open())
    {
        tprintf(PRINT_BTH, "File: %s not opened", FullName);
        return 0;
    }

    infile2 >> speed;
    infile2.close();

    return speed;
}
int vpxt_cpu_tick_return(const char *infile, int FileType)
{
    int speed;

    char TextFilechar1[255];

    vpxt_remove_file_extension(infile, TextFilechar1);

    char *FullName = "";

    if (FileType == 0)
    {
        FullName = strcat(TextFilechar1, "compression_cpu_tick.txt");
    }

    if (FileType == 1)
    {
        FullName = strcat(TextFilechar1, "decompression_cpu_tick.txt");
    }

    std::ifstream infile2(FullName);

    if (!infile2.is_open())
    {
        tprintf(PRINT_BTH, "File: %s not opened", FullName);
        return 0;
    }

    infile2 >> speed;
    infile2.close();

    return speed;
}
int vpxt_get_number_of_frames(const char *inputFile)
{
    FILE *in = fopen(inputFile, "rb");

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);

    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);
    int FrameLength = ivfhRaw.length;

    fclose(in);

    return FrameLength;
}
int vpxt_check_force_key_frames(const char *KeyFrameoutputfile, int ForceKeyFrameInt, const char *ForceKeyFrame)
{
    // return - 1 if file not found
    // return 1 if failed
    // return 0 if passed

    std::ifstream infile(KeyFrameoutputfile);

    if (!infile.good())
    {
        tprintf(PRINT_BTH, "\nKey Frame File Not Present\n");
        //fclose(fp);
        return -1;
    }

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;
    int selector2 = 0;
    int doOnce2 = 0;
    int x2;
    int y2;
    int framecount = 0;

    while (!infile.eof())
    {
        if (selector2 == 1)
        {
            infile >> x2;
            selector2 = 2;
        }
        else
        {
            infile >> y2;
            selector2 = 1;
        }

        if (doOnce2 == 0)
        {
            x2 = y2;
            infile >> y2;
            doOnce2 = 1;
            selector2 = 1;
        }

        if (vpxt_abs_int(y2 - x2) != ForceKeyFrameInt)
        {
            vpxt_formated_print(5, "Key Frames do not occur only when Force Key Frame dictates: %i - Failed", ForceKeyFrameInt);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    int maxKeyFrame = 0;

    if (x2 > y2)
    {
        maxKeyFrame = x2;
    }
    else
    {
        maxKeyFrame = y2;
    }

    int NumberofFrames = vpxt_get_number_of_frames(ForceKeyFrame);

    if (NumberofFrames - 1 >= (maxKeyFrame + ForceKeyFrameInt))
    {
        vpxt_formated_print(RESPRT, "Key Frames do not occur only when Force Key Frame dictates: %i - Failed", ForceKeyFrameInt);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    return fail;
}
int vpxt_check_mem_state(const std::string FileName, std::string &bufferString)
{
    std::ifstream infile4(FileName.c_str());

    if (!infile4.good())
        return -1;

    char buffer4[256];

    infile4.getline(buffer4, 256);
    infile4.getline(buffer4, 256);
    infile4.getline(buffer4, 256);
    infile4.getline(buffer4, 256);

    bufferString = buffer4;

    return 0;
}
int vpxt_print_compare_ivf_results(int lngRC)
{
    //return 1 for files identical
    //retrun -1 for files not identical
    //return 0 on error

    if (lngRC >= 0)
    {
        tprintf(PRINT_BTH, "Files differ at frame: %i\n", lngRC);
        return -1;
    }

    if (lngRC == -1)
    {
        tprintf(PRINT_BTH, "Files are identical\n");
        return 1;
    }

    if (lngRC == -2)
    {
        tprintf(PRINT_BTH, "File 2 ends before File 1\n");
        return -1;
    }

    if (lngRC == -3)
    {
        tprintf(PRINT_BTH, "File 1 ends before File 2\n");
        return -1;
    }

    return 0;
}
double vpxt_get_psnr(const char *compFileName)
{
    //Takes in a compression full path and looks for the psnr file
    //accociated with it.
    std::string TimeTextFileStr = compFileName;
    TimeTextFileStr.erase(TimeTextFileStr.length() - 4, 4);
    TimeTextFileStr.append("_psnr.txt");

    double inputPSNR;
    std::ifstream infile(TimeTextFileStr.c_str());

    if (!infile.good())
    {
        tprintf(PRINT_BTH, "\nFailed to open input file: %s", TimeTextFileStr.c_str());
        return 0;
    }

    infile >> inputPSNR;
    infile.close();

    if (inputPSNR <= 0.0 || inputPSNR >= 60.0)
    {
        tprintf(PRINT_BTH, "\nPSNR value is not vailid: %f\n", inputPSNR);
        return 0;
    }

    return inputPSNR;
}
//-----------------------------------------------------------IVF Enc------------------------------------------------------
int API20Encoder(long width, long height, const char *infilechar, const char *outfilechar)
{
    vpx_codec_ctx_t        encoder;
    const char                  *in_fn = infilechar, *out_fn = outfilechar, *stats_fn = NULL;
    int                    i;
    FILE                  *infile, *outfile;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_err_t        res;
    int                    pass, one_pass_only = 0;
    stats_io_t             stats;
    vpx_image_t            raw;
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
    res = vpx_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    cfg.g_w = width;                                                          //
    cfg.g_h = height;

    /* Now parse the remainder of the parameters. */


    /* Handle codec specific options */
    if (codec->iface == &vpx_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

#define SHOW(field) tprintf(  PRINT_STD, "    %-28s = %d\n", #field, cfg.field)

    if (1)
    {
        tprintf(PRINT_STD, "Source file: %s Format: %s\n", in_fn, arg_use_i420 ? "I420" : "YV12");
        tprintf(PRINT_STD, "Destination file: %s\n", out_fn);
        tprintf(PRINT_STD, "Encoder parameters:\n");

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

    vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);


    memset(&stats, 0, sizeof(stats));

    for (pass = one_pass_only ? one_pass_only - 1 : 0; pass < arg_passes; pass++)
    {
        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            tprintf(PRINT_STD, "Failed to open input file");
            return EXIT_FAILURE;
        }

        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            tprintf(PRINT_STD, "Failed to open output file");
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }


        cfg.g_pass = arg_passes == 2
                     ? pass ? VPX_RC_LAST_PASS : VPX_RC_FIRST_PASS
                 : VPX_RC_ONE_PASS;
#if VPX_ENCODER_ABI_VERSION > (1 + VPX_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = VPX_KF_FIXED;

        vpx_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");

        /* Note that we bypass the vpx_codec_control wrapper macro because
        * we're being clever to store the control IDs in an array. Real
        * applications will want to make use of the enumerations directly
        */
        for (i = 0; i < arg_ctrl_cnt; i++)
        {
            if (vpx_codec_control_(&encoder, arg_ctrls[i][0], arg_ctrls[i][1]))
                tprintf(PRINT_STD, "Error: Tried to set control %d = %d\n",
                        arg_ctrls[i][0], arg_ctrls[i][1]);

            ctx_exit_on_error(&encoder, "Failed to control codec");
        }

        frame_avail = 1;
        got_data = 0;

        while (frame_avail || got_data)
        {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            struct vpx_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                frame_avail = read_frame_enc(infile, &raw, width * height * 3 / 2);

                if (frame_avail)
                    frames_in++;

                tprintf(PRINT_STD, "\rPass %d/%d frame %4d/%-4d %7ldB \033[K", pass + 1,
                        arg_passes, frames_in, frames_out, nbytes);
            }
            else
                frame_avail = 0;

            vpx_usec_timer_start(&timer);
            vpx_codec_encode(&encoder, frame_avail ? &raw : NULL, frames_in - 1,
                             1, 0, arg_deadline);
            vpx_usec_timer_mark(&timer);
            cx_time += vpx_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    tprintf(PRINT_STD, " %6luF",
                            (unsigned long)pkt->data.frame.sz);
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case VPX_CODEC_STATS_PKT:
                    tprintf(PRINT_STD, " %6luS",
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
        vpx_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_STD, "\n");
    }

    vpx_img_free(&raw);
    return 0;
}
int API20EncoderIVF2IVF(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt)
{
    vpx_codec_ctx_t        encoder;
    const char             *in_fn = inputFile, *out_fn = outputFile2, *stats_fn = NULL;
    int                    i;
    FILE                  *infile, *outfile;
    vpx_codec_enc_cfg_t    cfg;
    vpx_codec_err_t        res;
    int                    pass;
    stats_io_t             stats;
    vpx_image_t            raw;
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
    res = vpx_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));
        return EXIT_FAILURE;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    vpxt_core_config_to_api_config(oxcf, &cfg);

    FILE *GetWHinfile = fopen(in_fn, "rb");
    IVF_HEADER ivfhRaw;
    IVF_FRAME_HEADER ivf_fhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    int w       = ivfhRaw.width;
    int h       = ivfhRaw.height;
    int fr      = (ivfhRaw.rate / ivfhRaw.scale);

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
        //          vpx_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_GOOD_QUALITY_ENCODING);
        cfg.g_profile = 1;
    }

    if (oxcf.Mode == 2) //One Pass Best
    {
        //          vpx_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_BEST_QUALITY_ENCODING);
        cfg.g_profile = 2;
    }

    if (oxcf.Mode == 3) //First Pass
    {
        //          vpx_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_GOOD_QUALITY_ENCODING);
        cfg.g_profile = 3;
    }

    if (oxcf.Mode == 4) //Two Pass Good
    {
        //          vpx_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_GOOD_QUALITY_ENCODING);
        cfg.g_profile = 0;
        arg_passes = 2;

    }

    if (oxcf.Mode == 5) //Two Pass Best
    {
        //          vpx_codec_control_(&codec, VP8E_SET_ENCODING_MODE, VP8_BEST_QUALITY_ENCODING);
        cfg.g_profile = 2;
        arg_passes = 2;
    }

    ///////////////////////////////////////////////////////////////////

    /* Handle codec specific options */
    if (codec->iface == &vpx_codec_vp8_cx_algo)
    {
        ctrl_args = vp8_args;
    }

#define SHOW(field) tprintf(  PRINT_STD, "    %-28s = %d\n", #field, cfg.field)

    if (1)
    {
        tprintf(PRINT_STD, "Source file: %s Format: %s\n", in_fn, arg_use_i420 ? "I420" : "YV12");
        tprintf(PRINT_STD, "Destination file: %s\n", out_fn);
        tprintf(PRINT_STD, "Encoder parameters:\n");

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

    vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
                  cfg.g_w, cfg.g_h, 1);

    for (pass = 0; pass < arg_passes; pass++)
    {
        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;

        infile = fopen(in_fn, "rb");

        if (!infile)
        {
            tprintf(PRINT_STD, "Failed to open input file");
            return EXIT_FAILURE;
        }

        //////////////////////read in junk IVFData//////////////////////
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        outfile = fopen(out_fn, "wb");

        if (!outfile)
        {
            tprintf(PRINT_STD, "Failed to open output file");
            return EXIT_FAILURE;
        }

        if (stats_fn)
        {
            if (!stats_open_file(&stats, stats_fn, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (!stats_open_mem(&stats, pass))
            {
                tprintf(PRINT_STD, "Failed to open statistics store\n");
                return EXIT_FAILURE;
            }
        }

        cfg.g_pass = arg_passes == 2
                     ? pass ? VPX_RC_LAST_PASS : VPX_RC_FIRST_PASS
                 : VPX_RC_ONE_PASS;
#if VPX_ENCODER_ABI_VERSION > (1 + VPX_CODEC_ABI_VERSION)

        if (pass)
        {
            cfg.rc_twopass_stats_in = stats_get(&stats);
        }

#endif

        write_ivf_file_header(outfile, &cfg, codec->fourcc, 0);

        /* Construct Encoder Context */
        if (cfg.kf_min_dist == cfg.kf_max_dist)
            cfg.kf_mode = VPX_KF_FIXED;

        vpx_codec_enc_init(&encoder, codec->iface, &cfg, 0);
        ctx_exit_on_error(&encoder, "Failed to initialize encoder");

        /* Note that we bypass the vpx_codec_control wrapper macro because
        * we're being clever to store the control IDs in an array. Real
        * applications will want to make use of the enumerations directly
        */
        for (i = 0; i < arg_ctrl_cnt; i++)
        {
            if (vpx_codec_control_(&encoder, arg_ctrls[i][0], arg_ctrls[i][1]))
                tprintf(PRINT_STD, "Error: Tried to set control %d = %d\n",
                        arg_ctrls[i][0], arg_ctrls[i][1]);

            ctx_exit_on_error(&encoder, "Failed to control codec");
        }

        frame_avail = 1;
        got_data = 0;

        while (frame_avail || got_data)
        {
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            struct vpx_usec_timer timer;

            if (!arg_limit || frames_in < arg_limit)
            {
                //////////////////////read in junk IVFData//////////////////////
                fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame_enc(infile, &raw, ivf_fhRaw.frameSize);

                if (frame_avail)
                    frames_in++;

                tprintf(PRINT_STD, "\rPass %d/%d frame %4d/%-4d %7ldB \033[K", pass + 1,
                        arg_passes, frames_in, frames_out, nbytes);
            }
            else
                frame_avail = 0;

            vpx_usec_timer_start(&timer);
            vpx_codec_encode(&encoder, frame_avail ? &raw : NULL, frames_in - 1,
                             1, 0, arg_deadline);
            vpx_usec_timer_mark(&timer);
            cx_time += vpx_usec_timer_elapsed(&timer);
            ctx_exit_on_error(&encoder, "Failed to encode frame");
            got_data = 0;

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                frames_out++;
                got_data = 1;
                nbytes += pkt->data.raw.sz;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    tprintf(PRINT_STD, " %6luF",
                            (unsigned long)pkt->data.frame.sz);
                    write_ivf_frame_header(outfile, pkt);
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
                    break;
                case VPX_CODEC_STATS_PKT:
                    tprintf(PRINT_STD, " %6luS",
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
        vpx_codec_destroy(&encoder);

        fclose(infile);

        if (!fseek(outfile, 0, SEEK_SET))
            write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_STD, "\n");
    }

    vpx_img_free(&raw);
    return 0;
}
int API20Decoder(const char *inputchar, const char *outputchar)
{
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    char                  *prefix = NULL;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    const char             *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_STD, "Failed to open file");
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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg,
                           postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    tprintf(PRINT_STD, "%s\n", decoder.name);
    std::cout << "\nWidth: " << cfg.w << "\nHeight: " << cfg.h << "\n";

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            tprintf(PRINT_STD, "decoded frame %d.\n", frame);

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
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
        tprintf(PRINT_STD, "Decoded %d frames in %lu us (%.2f fps)\n",
                frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);
    free(prefix);

    return 0;
}
int API20DecoderIVF2IVF(const char *inputchar, const char *outputchar)
{
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    char                  *prefix = NULL;
    int                    i;
    uint8_t               *buf = NULL;
    uint32_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           is_ivf, fourcc;
    unsigned long          dx_time = 0;
    const char             *fn2 = outputchar;
    void *out;
    vpx_codec_dec_cfg_t     cfg = {0};

    /* Open file */
    infile = fopen(fn, "rb");

    if (!infile)
    {
        tprintf(PRINT_STD, "Failed to open file");
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
                vpx_codec_iface_t  *ivf_iface = ifaces[i].iface;

                if (iface && iface != ivf_iface)
                    tprintf(PRINT_STD, "Notice -- IVF header indicates codec: %s\n",
                            ifaces[i].name);
                else
                    iface = ivf_iface;

                break;
            }
    }

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    tprintf(PRINT_STD, "%s\n", decoder.name);

    /* Decode file */
    while (!read_frame(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf))
    {
        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;
        struct vpx_usec_timer timer;

        vpx_usec_timer_start(&timer);

        if (vpx_codec_decode(&decoder, buf, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));

            if (detail)
                tprintf(PRINT_STD, "  Additional information: %s\n", detail);

            goto fail;
        }

        vpx_usec_timer_mark(&timer);
        dx_time += vpx_usec_timer_elapsed(&timer);

        ++frame;

        if (progress)
            tprintf(PRINT_STD, "decoded frame %d.\n", frame);

        if (!noblit)
        {
            if (img = vpx_codec_get_frame(&decoder, &iter))
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
        tprintf(PRINT_STD, "Decoded %d frames in %lu us (%.2f fps)\n",
                frame, dx_time, (float)frame * 1000000.0 / (float)dx_time);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        return EXIT_FAILURE;
    }

    if (fn2)
        out_close(out, fn2, do_md5);

    free(buf);
    fclose(infile);
    free(prefix);

    return 0;
}
