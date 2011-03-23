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
#include "vpx_version.h"
#include "vpx_config.h"
#include "tools_common.h"
#include "y4minput.h"
#include "EbmlWriter.h"
#include "EbmlIDs.h"
#include "nestegg.h"
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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef unsigned char BYTE;
typedef off_t EbmlLoc;

#define IVF_FRAME_HDR_SZ (sizeof(uint32_t) + sizeof(uint64_t))
#define RAW_FRAME_HDR_SZ (sizeof(uint32_t))
#define IVF_FILE_HDR_SZ (32)
#define VP8_FOURCC (0x00385056)
#define CONFIG_VP8_ENCODER 1

#if !defined(_MSC_VER)
#define LITERALU64(n) n##LLU
#endif

#if defined(_MSC_VER)
#define fseeko _fseeki64
#define ftello _ftelli64
#define LITERALU64(n) n
#elif defined(_WIN32)
#define fseeko fseeko64
#define ftello ftello64
#endif

/* These pointers are to the start of an element */
#if !CONFIG_OS_SUPPORT
typedef long off_t;
#define fseeko fseek
#define ftello ftell
#endif

#if CONFIG_OS_SUPPORT
#if defined(_WIN32)
#include <io.h>
#define snprintf _snprintf
#define isatty   _isatty
#define fileno   _fileno
#else
#include <unistd.h>
#endif
#endif
#ifndef PATH_MAX
#define PATH_MAX 256
#endif

#if CONFIG_VP8_DECODER
#include "vp8dx.h"
#endif
#if CONFIG_MD5
#include "md5_utils.h"
#endif

#if defined(_WIN32)
#define USE_POSIX_MMAP 0
#include "on2vpplugin.h"
#define snprintf _snprintf
#else
#define USE_POSIX_MMAP 1
#include "dirent.h"
typedef unsigned int  DWORD;
#endif

#if USE_POSIX_MMAP
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifdef _MSC_VER
#define USE_POSIX_MMAP 0
#else
#define USE_POSIX_MMAP 1
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
//////////////////////////ENCODE-START//////////////////////////////////////////////////////////////////////////////////////////////
enum video_file_type
{
    FILE_TYPE_RAW,
    FILE_TYPE_IVF,
    FILE_TYPE_Y4M
};

struct detect_buffer
{
    char buf[4];
    size_t buf_read;
    size_t position;
};

struct cue_entry
{
    unsigned int time;
    uint64_t     loc;
};
struct EbmlGlobal
{
    int debug;
    FILE    *stream;
    int64_t last_pts_ms;
    vpx_rational_t  framerate;
    off_t    position_reference;
    off_t    seek_info_pos;
    off_t    segment_info_pos;
    off_t    track_pos;
    off_t    cue_pos;
    off_t    cluster_pos;
    off_t    track_id_pos;
    EbmlLoc  startSegment;
    EbmlLoc  startCluster;
    uint32_t cluster_timecode;
    int      cluster_open;
    struct cue_entry *cue_list;
    unsigned int      cues;
};
static const char *exec_name;
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
        out = strcmp("-", out_fn) ? fopen(out_fn, "wb") : set_binary_mode(stdout);
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
void ivf_write_frame_header(FILE *outfile, uint64_t timeStamp, uint32_t frameSize)
{
    IVF_FRAME_HEADER ivf_fh;

    ivf_fh.timeStamp = make_endian_64(timeStamp);
    ivf_fh.frameSize = make_endian_32(frameSize);

    fwrite((char *)&ivf_fh, sizeof(ivf_fh), 1, outfile);

}
static const struct codec_item
{
    char const              *name;
    const vpx_codec_iface_t *iface;
    unsigned int             fourcc;
} codecs[] =
{
#if CONFIG_VP8_ENCODER
    {"vp8",  &vpx_codec_vp8_cx_algo, 0x30385056},
#endif
};

static void usage_exit_enc();

void die_enc(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    usage_exit_enc();
}
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

static int read_frame_enc(FILE *f, vpx_image_t *img, unsigned int file_type, y4m_input *y4m, struct detect_buffer *detect)
{
    int plane = 0;
    int shortread = 0;

    if (file_type == FILE_TYPE_Y4M)
    {
        if (y4m_input_fetch_frame(y4m, f, img) < 1)
            return 0;
    }
    else
    {
        if (file_type == FILE_TYPE_IVF)
        {
            char junk[IVF_FRAME_HDR_SZ];

            /* Skip the frame header. We know how big the frame should be. See
            * write_ivf_frame_header() for documentation on the frame header
            * layout.
            */
            if (fread(junk, 1, IVF_FRAME_HDR_SZ, f));
        }

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
                size_t needed = w;
                size_t buf_position = 0;
                const size_t left = detect->buf_read - detect->position;

                if (left > 0)
                {
                    const size_t more = (left < needed) ? left : needed;
                    memcpy(ptr, detect->buf + detect->position, more);
                    buf_position = more;
                    needed -= more;
                    detect->position += more;
                }

                if (needed > 0)
                {
                    shortread |= (fread(ptr + buf_position, 1, needed, f) < needed);
                }

                ptr += img->stride[plane];
            }
        }
    }

    return !shortread;
}
static int skim_frame_enc(FILE *f, vpx_image_t *img, unsigned int file_type, y4m_input *y4m, struct detect_buffer *detect)
{
    int plane = 0;
    int shortread = 0;

    if (file_type == FILE_TYPE_Y4M)
    {
        if (y4m_input_skim_frame(y4m, f, img) < 1)
            return 0;
    }

    if (file_type == FILE_TYPE_IVF)
    {
        IVF_FRAME_HEADER ivf_fhRaw;
        fread(&ivf_fhRaw.frameSize, 1, 4, f);
        fread(&ivf_fhRaw.timeStamp, 1, 8, f);
        vpxt_format_frame_header_read(ivf_fhRaw);
        fseek(f, ivf_fhRaw.frameSize, SEEK_CUR);
        return 0;
    }

    return 1;
}
unsigned int file_is_ivf(FILE *infile, unsigned int *fourcc, unsigned int *width, unsigned int *height, struct detect_buffer *detect, unsigned int *scale, unsigned int *rate)
{
    char raw_hdr[IVF_FILE_HDR_SZ];
    int is_ivf = 0;

    if (memcmp(detect->buf, "DKIF", 4) != 0)
        return 0;

    /* See write_ivf_file_header() for more documentation on the file header
    * layout.
    */
    if (fread(raw_hdr + 4, 1, IVF_FILE_HDR_SZ - 4, infile)
        == IVF_FILE_HDR_SZ - 4)
    {
        {
            is_ivf = 1;

            if (mem_get_le16(raw_hdr + 4) != 0)
                fprintf(stderr, "Error: Unrecognized IVF version! This file may not"
                        " decode properly.");

            *fourcc = mem_get_le32(raw_hdr + 8);
        }
    }

    if (is_ivf)
    {
        *width = mem_get_le16(raw_hdr + 12);
        *height = mem_get_le16(raw_hdr + 14);
        *rate = mem_get_le32(raw_hdr + 16);
        *scale = mem_get_le32(raw_hdr + 20);
        detect->position = 4;
    }

    return is_ivf;
}

unsigned int file_is_y4m(FILE      *infile, y4m_input *y4m, char       detect[4])
{
    if (memcmp(detect, "YUV4", 4) == 0)
    {
        return 1;
    }

    return 0;
}
static void write_ivf_file_header(FILE *outfile, const vpx_codec_enc_cfg_t *cfg, unsigned int fourcc, int frame_cnt)
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

    if (fwrite(header, 1, 32, outfile));
}


static void write_ivf_frame_header(FILE *outfile, const vpx_codec_cx_pkt_t *pkt)
{
    char             header[12];
    vpx_codec_pts_t  pts;

    if (pkt->kind != VPX_CODEC_CX_FRAME_PKT)
        return;

    pts = pkt->data.frame.pts;
    mem_put_le32(header, pkt->data.frame.sz);
    mem_put_le32(header + 4, pts & 0xFFFFFFFF);
    mem_put_le32(header + 8, pts >> 32);

    if (fwrite(header, 1, 12, outfile));
}

void Ebml_Write(EbmlGlobal *glob, const void *buffer_in, unsigned long len)
{
    if (fwrite(buffer_in, 1, len, glob->stream));
}
void Ebml_Serialize(EbmlGlobal *glob, const void *buffer_in, unsigned long len)
{
    const unsigned char *q = (const unsigned char *)buffer_in + len - 1;

    for (; len; len--)
        Ebml_Write(glob, q--, 1);
}
static void Ebml_SerializeUnsigned32(EbmlGlobal *glob, unsigned long class_id, uint64_t ui)
{
    unsigned char sizeSerialized = 4 | 0x80;
    Ebml_WriteID(glob, class_id);
    Ebml_Serialize(glob, &sizeSerialized, 1);
    Ebml_Serialize(glob, &ui, 4);
}
static void Ebml_StartSubElement(EbmlGlobal *glob, EbmlLoc *ebmlLoc, unsigned long class_id)
{
    unsigned long long unknownLen =  LITERALU64(0x01FFFFFFFFFFFFFF);
    Ebml_WriteID(glob, class_id);
    *ebmlLoc = ftello(glob->stream);
    Ebml_Serialize(glob, &unknownLen, 8);
}
static void Ebml_EndSubElement(EbmlGlobal *glob, EbmlLoc *ebmlLoc)
{
    off_t pos;
    uint64_t size;
    pos = ftello(glob->stream);
    size = pos - *ebmlLoc - 8;
    size |=  LITERALU64(0x0100000000000000);
    fseeko(glob->stream, *ebmlLoc, SEEK_SET);
    Ebml_Serialize(glob, &size, 8);
    fseeko(glob->stream, pos, SEEK_SET);
}
static void write_webm_seek_element(EbmlGlobal *ebml, unsigned long id, off_t pos)
{
    uint64_t offset = pos - ebml->position_reference;
    EbmlLoc start;
    Ebml_StartSubElement(ebml, &start, Seek);
    Ebml_SerializeBinary(ebml, SeekID, id);
    Ebml_SerializeUnsigned64(ebml, SeekPosition, offset);
    Ebml_EndSubElement(ebml, &start);
}
static void write_webm_seek_info(EbmlGlobal *ebml)
{
    off_t pos;
    pos = ftello(ebml->stream);

    if (ebml->seek_info_pos)
        fseeko(ebml->stream, ebml->seek_info_pos, SEEK_SET);
    else
        ebml->seek_info_pos = pos;

    {
        EbmlLoc start;
        Ebml_StartSubElement(ebml, &start, SeekHead);
        write_webm_seek_element(ebml, Tracks, ebml->track_pos);
        write_webm_seek_element(ebml, Cues,   ebml->cue_pos);
        write_webm_seek_element(ebml, Info,   ebml->segment_info_pos);
        Ebml_EndSubElement(ebml, &start);
    }
    {
        EbmlLoc startInfo;
        uint64_t frame_time;
        frame_time = (uint64_t)1000 * ebml->framerate.den
                     / ebml->framerate.num;
        ebml->segment_info_pos = ftello(ebml->stream);
        Ebml_StartSubElement(ebml, &startInfo, Info);
        Ebml_SerializeUnsigned(ebml, TimecodeScale, 1000000);
        Ebml_SerializeFloat(ebml, Segment_Duration,
                            ebml->last_pts_ms + frame_time);
        Ebml_SerializeString(ebml, 0x4D80,
                             ebml->debug ? "vpxenc" : "vpxenc" VERSION_STRING);
        Ebml_SerializeString(ebml, 0x5741,
                             ebml->debug ? "vpxenc" : "vpxenc" VERSION_STRING);
        Ebml_EndSubElement(ebml, &startInfo);
    }
}
static void write_webm_file_header(EbmlGlobal *glob, const vpx_codec_enc_cfg_t *cfg, const struct vpx_rational *fps)
{
    {
        EbmlLoc start;
        Ebml_StartSubElement(glob, &start, EBML);
        Ebml_SerializeUnsigned(glob, EBMLVersion, 1);
        Ebml_SerializeUnsigned(glob, EBMLReadVersion, 1); //EBML Read Version
        Ebml_SerializeUnsigned(glob, EBMLMaxIDLength, 4); //EBML Max ID Length
        Ebml_SerializeUnsigned(glob, EBMLMaxSizeLength, 8); //EBML Max Size Length
        Ebml_SerializeString(glob, DocType, "webm"); //Doc Type
        Ebml_SerializeUnsigned(glob, DocTypeVersion, 2); //Doc Type Version
        Ebml_SerializeUnsigned(glob, DocTypeReadVersion, 2); //Doc Type Read Version
        Ebml_EndSubElement(glob, &start);
    }
    {
        Ebml_StartSubElement(glob, &glob->startSegment, Segment); //segment
        glob->position_reference = ftello(glob->stream);
        glob->framerate = *fps;
        write_webm_seek_info(glob);
        {
            EbmlLoc trackStart;
            glob->track_pos = ftello(glob->stream);
            Ebml_StartSubElement(glob, &trackStart, Tracks);
            {
                unsigned int trackNumber = 1;
                uint64_t     trackID = 0;
                EbmlLoc start;
                Ebml_StartSubElement(glob, &start, TrackEntry);
                Ebml_SerializeUnsigned(glob, TrackNumber, trackNumber);
                glob->track_id_pos = ftello(glob->stream);
                Ebml_SerializeUnsigned32(glob, TrackUID, trackID);
                Ebml_SerializeUnsigned(glob, TrackType, 1); //video is always 1
                Ebml_SerializeString(glob, CodecID, "V_VP8");
                {
                    unsigned int pixelWidth = cfg->g_w;
                    unsigned int pixelHeight = cfg->g_h;
                    float        frameRate   = (float)fps->num / (float)fps->den;
                    EbmlLoc videoStart;
                    Ebml_StartSubElement(glob, &videoStart, Video);
                    Ebml_SerializeUnsigned(glob, PixelWidth, pixelWidth);
                    Ebml_SerializeUnsigned(glob, PixelHeight, pixelHeight);
                    Ebml_SerializeFloat(glob, FrameRate, frameRate);
                    Ebml_EndSubElement(glob, &videoStart); //Video
                }
                Ebml_EndSubElement(glob, &start); //Track Entry
            }
            Ebml_EndSubElement(glob, &trackStart);
        }
    }
}
static void write_webm_block(EbmlGlobal *glob, const vpx_codec_enc_cfg_t *cfg, const vpx_codec_cx_pkt_t  *pkt)
{
    unsigned long  block_length;
    unsigned char  track_number;
    unsigned short block_timecode = 0;
    unsigned char  flags;
    int64_t        pts_ms;
    int            start_cluster = 0, is_keyframe;
    pts_ms = pkt->data.frame.pts * 1000
             * (uint64_t)cfg->g_timebase.num / (uint64_t)cfg->g_timebase.den;

    if (pts_ms <= glob->last_pts_ms)
        pts_ms = glob->last_pts_ms + 1;

    glob->last_pts_ms = pts_ms;

    if (pts_ms - glob->cluster_timecode > SHRT_MAX)
        start_cluster = 1;
    else
        block_timecode = pts_ms - glob->cluster_timecode;

    is_keyframe = (pkt->data.frame.flags & VPX_FRAME_IS_KEY);

    if (start_cluster || is_keyframe)
    {
        if (glob->cluster_open)
            Ebml_EndSubElement(glob, &glob->startCluster);

        block_timecode = 0;
        glob->cluster_open = 1;
        glob->cluster_timecode = pts_ms;
        glob->cluster_pos = ftello(glob->stream);
        Ebml_StartSubElement(glob, &glob->startCluster, Cluster); //cluster
        Ebml_SerializeUnsigned(glob, Timecode, glob->cluster_timecode);

        if (is_keyframe)
        {
            struct cue_entry *cue;
            glob->cue_list = (cue_entry *)realloc(glob->cue_list,
                                                  (glob->cues + 1) * sizeof(struct cue_entry));
            cue = &glob->cue_list[glob->cues];
            cue->time = glob->cluster_timecode;
            cue->loc = glob->cluster_pos;
            glob->cues++;
        }
    }

    Ebml_WriteID(glob, SimpleBlock);
    block_length = pkt->data.frame.sz + 4;
    block_length |= 0x10000000;
    Ebml_Serialize(glob, &block_length, 4);
    track_number = 1;
    track_number |= 0x80;
    Ebml_Write(glob, &track_number, 1);
    Ebml_Serialize(glob, &block_timecode, 2);
    flags = 0;

    if (is_keyframe)
        flags |= 0x80;

    if (pkt->data.frame.flags & VPX_FRAME_IS_INVISIBLE)
        flags |= 0x08;

    Ebml_Write(glob, &flags, 1);
    Ebml_Write(glob, pkt->data.frame.buf, pkt->data.frame.sz);
}
static void
write_webm_file_footer(EbmlGlobal *glob, long hash)
{
    if (glob->cluster_open)
        Ebml_EndSubElement(glob, &glob->startCluster);

    {
        EbmlLoc start;
        int i;
        glob->cue_pos = ftello(glob->stream);
        Ebml_StartSubElement(glob, &start, Cues);

        for (i = 0; i < glob->cues; i++)
        {
            struct cue_entry *cue = &glob->cue_list[i];
            EbmlLoc start;
            Ebml_StartSubElement(glob, &start, CuePoint);
            {
                EbmlLoc start;
                Ebml_SerializeUnsigned(glob, CueTime, cue->time);
                Ebml_StartSubElement(glob, &start, CueTrackPositions);
                Ebml_SerializeUnsigned(glob, CueTrack, 1);
                Ebml_SerializeUnsigned64(glob, CueClusterPosition,
                                         cue->loc - glob->position_reference);
                Ebml_EndSubElement(glob, &start);
            }
            Ebml_EndSubElement(glob, &start);
        }

        Ebml_EndSubElement(glob, &start);
    }
    Ebml_EndSubElement(glob, &glob->startSegment);
    write_webm_seek_info(glob);
    fseeko(glob->stream, glob->track_id_pos, SEEK_SET);
    Ebml_SerializeUnsigned32(glob, TrackUID, glob->debug ? 0xDEADBEEF : hash);
    fseeko(glob->stream, 0, SEEK_END);
}
static unsigned int murmur(const void *key, int len, unsigned int seed)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    unsigned int h = seed ^ len;
    const unsigned char *data = (const unsigned char *)key;

    while (len >= 4)
    {
        unsigned int k;
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;

    h *= m;

    h ^= h >> 15;

    return h;
}

static double vp8_mse2psnr(double Samples, double Peak, double Mse)
{
    double psnr;

    if ((double)Mse > 0.0)
        psnr = 10.0 * log10(Peak * Peak * Samples / Mse);
    else
        psnr = 60;      // Limit to prevent / 0

    if (psnr > 60)
        psnr = 60;

    return psnr;
}


static const arg_def_t debugmode = ARG_DEF("D", "debug", 0,
                                   "Debug mode (makes output deterministic)");
static const arg_def_t outputfile = ARG_DEF("o", "output", 1,
                                    "Output filename");
static const arg_def_t use_yv12 = ARG_DEF(NULL, "yv12", 0,
                                  "Input file is YV12 ");
static const arg_def_t use_i420 = ARG_DEF(NULL, "i420", 0,
                                  "Input file is I420 (default)");
static const arg_def_t codecarg = ARG_DEF(NULL, "codec", 1,
                                  "Codec to use");
static const arg_def_t passes           = ARG_DEF("p", "passes", 1,
        "Number of passes (1/2)");
static const arg_def_t pass_arg         = ARG_DEF(NULL, "pass", 1,
        "Pass to execute (1/2)");
static const arg_def_t fpf_name         = ARG_DEF(NULL, "fpf", 1,
        "First pass statistics file name");
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
static const arg_def_t psnrarg          = ARG_DEF(NULL, "psnr", 0,
        "Show PSNR in status line");
static const arg_def_t framerate        = ARG_DEF(NULL, "fps", 1,
        "Stream frame rate (rate/scale)");
static const arg_def_t use_ivf          = ARG_DEF(NULL, "ivf", 0,
        "Output IVF (default is WebM)");
static const arg_def_t *main_args[] =
{
    &debugmode,
    &outputfile, &codecarg, &passes, &pass_arg, &fpf_name, &limit, &deadline,
    &best_dl, &good_dl, &rt_dl,
    &verbosearg, &psnrarg, &use_ivf, &framerate,
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
    &width, &height, &timebase, &framerate, &error_resilient,
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
        "VBR=0 | CBR=1 | CQ=2");
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
static const arg_def_t kf_disabled = ARG_DEF(NULL, "disable-kf", 0,
                                     "Disable keyframe placement");
static const arg_def_t *kf_args[] =
{
    &kf_min_dist, &kf_max_dist, &kf_disabled, NULL
};


#if CONFIG_VP8_ENCODER
static const arg_def_t noise_sens = ARG_DEF(NULL, "noise-sensitivity", 1,
                                    "Noise sensitivity (frames to blur)");
static const arg_def_t sharpness = ARG_DEF(NULL, "sharpness", 1,
                                   "Filter sharpness (0-7)");
static const arg_def_t static_thresh = ARG_DEF(NULL, "static-thresh", 1,
                                       "Motion detection threshold");
#endif

#if CONFIG_VP8_ENCODER
static const arg_def_t cpu_used = ARG_DEF(NULL, "cpu-used", 1,
                                  "CPU Used (-16..16)");
#endif


#if CONFIG_VP8_ENCODER
static const arg_def_t token_parts = ARG_DEF(NULL, "token-parts", 1,
                                     "Number of token partitions to use, log2");
static const arg_def_t auto_altref = ARG_DEF(NULL, "auto-alt-ref", 1,
                                     "Enable automatic alt reference frames");
static const arg_def_t arnr_maxframes = ARG_DEF(NULL, "arnr-maxframes", 1,
                                        "AltRef Max Frames");
static const arg_def_t arnr_strength = ARG_DEF(NULL, "arnr-strength", 1,
                                       "AltRef Strength");
static const arg_def_t arnr_type = ARG_DEF(NULL, "arnr-type", 1,
                                   "AltRef Type");
static const struct arg_enum_list tuning_enum[] =
{
    {"psnr", VP8_TUNE_PSNR},
    {"ssim", VP8_TUNE_SSIM},
    {NULL, 0}
};
static const arg_def_t tune_ssim = ARG_DEF_ENUM(NULL, "tune", 1,
                                   "Material to favor", tuning_enum);
static const arg_def_t cq_level = ARG_DEF(NULL, "cq-level", 1,
                                  "Constrained Quality Level");
static const arg_def_t *vp8_args[] =
{
    &cpu_used, &auto_altref, &noise_sens, &sharpness, &static_thresh,
    &token_parts, &arnr_maxframes, &arnr_strength, &arnr_type,
    &tune_ssim, &cq_level, NULL
};
static const int vp8_arg_ctrl_map[] =
{
    VP8E_SET_CPUUSED, VP8E_SET_ENABLEAUTOALTREF,
    VP8E_SET_NOISE_SENSITIVITY, VP8E_SET_SHARPNESS, VP8E_SET_STATIC_THRESHOLD,
    VP8E_SET_TOKEN_PARTITIONS,
    VP8E_SET_ARNR_MAXFRAMES, VP8E_SET_ARNR_STRENGTH , VP8E_SET_ARNR_TYPE,
    VP8E_SET_TUNING, VP8E_SET_CQ_LEVEL, 0
};
#endif

static const arg_def_t *no_args[] = { NULL };

static void usage_exit_enc()
{
    int i;

    fprintf(stderr, "Usage: %s <options> -o dst_filename src_filename \n",
            exec_name);

    fprintf(stderr, "\nOptions:\n");
    arg_show_usage(stdout, main_args);
    fprintf(stderr, "\nEncoder Global Options:\n");
    arg_show_usage(stdout, global_args);
    fprintf(stderr, "\nRate Control Options:\n");
    arg_show_usage(stdout, rc_args);
    fprintf(stderr, "\nTwopass Rate Control Options:\n");
    arg_show_usage(stdout, rc_twopass_args);
    fprintf(stderr, "\nKeyframe Placement Options:\n");
    arg_show_usage(stdout, kf_args);
#if CONFIG_VP8_ENCODER
    fprintf(stderr, "\nVP8 Specific Options:\n");
    arg_show_usage(stdout, vp8_args);
#endif
    fprintf(stderr, "\n"
            "Included encoders:\n"
            "\n");

    for (i = 0; i < sizeof(codecs) / sizeof(codecs[0]); i++)
        fprintf(stderr, "    %-6s - %s\n",
                codecs[i].name,
                vpx_codec_iface_name(codecs[i].iface));

    exit(EXIT_FAILURE);
}


///////////////////////////ENCODE-END///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////DECODE-START////////////////////////////////////////////////////////////////////////////////////////////////////
static const struct
{
    char const *name;
    const vpx_codec_iface_t *iface;
    unsigned int             fourcc;
    unsigned int             fourcc_mask;
} ifaces[] =
{
#if CONFIG_VP8_DECODER
    {"vp8",  &vpx_codec_vp8_dx_algo,   VP8_FOURCC, 0x00FFFFFF},
#endif
};

static const arg_def_t flipuvarg = ARG_DEF(NULL, "flipuv", 0,
                                   "Flip the chroma planes in the output");
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
static const arg_def_t threadsarg = ARG_DEF("t", "threads", 1,
                                    "Max threads to use");

#if CONFIG_MD5
static const arg_def_t md5arg = ARG_DEF(NULL, "md5", 0,
                                        "Compute the MD5 sum of the decoded frame");
#endif
static const arg_def_t *all_args[] =
{
    &codecarg, &use_yv12, &use_i420, &flipuvarg, &noblitarg,
    &progressarg, &limitarg, &postprocarg, &summaryarg, &outputfile,
    &threadsarg, &verbosearg,
#if CONFIG_MD5
    &md5arg,
#endif
    NULL
};

#if CONFIG_VP8_DECODER
static const arg_def_t addnoise_level = ARG_DEF(NULL, "noise-level", 1,
                                        "Enable VP8 postproc add noise");
static const arg_def_t deblock = ARG_DEF(NULL, "deblock", 0,
                                 "Enable VP8 deblocking");
static const arg_def_t demacroblock_level = ARG_DEF(NULL, "demacroblock-level", 1,
        "Enable VP8 demacroblocking, w/ level");
static const arg_def_t pp_debug_info = ARG_DEF(NULL, "pp-debug-info", 1,
                                       "Enable VP8 visible debug info");
static const arg_def_t pp_disp_ref_frame = ARG_DEF(NULL, "pp-dbg-ref-frame", 1,
        "Display only selected reference frame per macro block");
static const arg_def_t pp_disp_mb_modes = ARG_DEF(NULL, "pp-dbg-mb-modes", 1,
        "Display only selected macro block modes");
static const arg_def_t pp_disp_b_modes = ARG_DEF(NULL, "pp-dbg-b-modes", 1,
        "Display only selected block modes");
static const arg_def_t pp_disp_mvs = ARG_DEF(NULL, "pp-dbg-mvs", 1,
                                     "Draw only selected motion vectors");

static const arg_def_t *vp8_pp_args[] =
{
    &addnoise_level, &deblock, &demacroblock_level, &pp_debug_info,
    &pp_disp_ref_frame, &pp_disp_mb_modes, &pp_disp_b_modes, &pp_disp_mvs,
    NULL
};
#endif
static void usage_exit_dec()
{
    int i;

    fprintf(stderr, "Usage: %s <options> filename\n\n"
            "Options:\n", exec_name);
    arg_show_usage(stderr, all_args);
#if CONFIG_VP8_DECODER
    fprintf(stderr, "\nVP8 Postprocessing Options:\n");
    arg_show_usage(stderr, vp8_pp_args);
#endif
    fprintf(stderr,
            "\nOutput File Patterns:\n\n"
            "  The -o argument specifies the name of the file(s) to "
            "write to. If the\n  argument does not include any escape "
            "characters, the output will be\n  written to a single file. "
            "Otherwise, the filename will be calculated by\n  expanding "
            "the following escape characters:\n"
            "\n\t%%w   - Frame width"
            "\n\t%%h   - Frame height"
            "\n\t%%<n> - Frame number, zero padded to <n> places (1..9)"
            "\n\n  Pattern arguments are only supported in conjunction "
            "with the --yv12 and\n  --i420 options. If the -o option is "
            "not specified, the output will be\n  directed to stdout.\n"
           );
    fprintf(stderr, "\nIncluded decoders:\n\n");

    for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
        fprintf(stderr, "    %-6s - %s\n",
                ifaces[i].name,
                vpx_codec_iface_name(ifaces[i].iface));

    exit(EXIT_FAILURE);
}

void die_dec(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    usage_exit_dec();
}

enum file_kind
{
    RAW_FILE,
    IVF_FILE,
    WEBM_FILE
};
struct input_ctx
{
    enum file_kind  kind;
    FILE           *infile;
    nestegg        *nestegg_ctx;
    nestegg_packet *pkt;
    unsigned int    chunk;
    unsigned int    chunks;
    unsigned int    video_track;
};
static int read_frame_dec(struct input_ctx      *input, uint8_t               **buf, size_t                *buf_sz, size_t                *buf_alloc_sz)
{
    char     raw_hdr[IVF_FRAME_HDR_SZ];
    size_t          new_buf_sz;
    FILE           *infile = input->infile;
    enum file_kind  kind = input->kind;

    if (kind == WEBM_FILE)
    {
        if (input->chunk >= input->chunks)
        {
            unsigned int track;

            do
            {
                if (input->pkt)
                    nestegg_free_packet(input->pkt);

                if (nestegg_read_packet(input->nestegg_ctx, &input->pkt) <= 0
                    || nestegg_packet_track(input->pkt, &track))
                    return 1;
            }
            while (track != input->video_track);

            if (nestegg_packet_count(input->pkt, &input->chunks))
                return 1;

            input->chunk = 0;
        }

        if (nestegg_packet_data(input->pkt, input->chunk, buf, buf_sz))
            return 1;

        input->chunk++;
        return 0;
    }
    /* For both the raw and ivf formats, the frame size is the first 4 bytes
    * of the frame header. We just need to special case on the header
    * size.
    */
    else if (fread(raw_hdr, kind == IVF_FILE
                   ? IVF_FRAME_HDR_SZ : RAW_FRAME_HDR_SZ, 1, infile) != 1)
    {
        if (!feof(infile))
            fprintf(stderr, "Failed to read frame size\n");

        new_buf_sz = 0;
    }
    else
    {
        new_buf_sz = mem_get_le32(raw_hdr);

        if (new_buf_sz > 256 * 1024 * 1024)
        {
            fprintf(stderr, "Error: Read invalid frame size (%u)\n",
                    new_buf_sz);
            new_buf_sz = 0;
        }

        if (kind == RAW_FILE && new_buf_sz > 256 * 1024)
            fprintf(stderr, "Warning: Read invalid frame size (%u)"
                    " - not a raw file?\n", new_buf_sz);

        if (new_buf_sz > *buf_alloc_sz)
        {
            uint8_t *new_buf = (uint8_t *)realloc(*buf, 2 * new_buf_sz);

            if (new_buf)
            {
                *buf = new_buf;
                *buf_alloc_sz = 2 * new_buf_sz;
            }
            else
            {
                fprintf(stderr, "Failed to allocate compressed data buffer\n");
                new_buf_sz = 0;
            }
        }
    }

    *buf_sz = new_buf_sz;

    if (*buf_sz)
    {
        if (fread(*buf, 1, *buf_sz, infile) != *buf_sz)
        {
            fprintf(stderr, "Failed to read full frame\n");
            return 1;
        }

        return 0;
    }

    return 1;
}

static int skim_frame_dec(struct input_ctx      *input, uint8_t               **buf, size_t                *buf_sz, size_t                *buf_alloc_sz)
{
    char     raw_hdr[IVF_FRAME_HDR_SZ];
    size_t          new_buf_sz;
    FILE           *infile = input->infile;
    enum file_kind  kind = input->kind;

    if (kind == WEBM_FILE)
    {
        if (input->chunk >= input->chunks)
        {
            unsigned int track;

            do
            {
                if (input->pkt)
                    nestegg_free_packet(input->pkt);

                if (nestegg_read_packet(input->nestegg_ctx, &input->pkt) <= 0
                    || nestegg_packet_track(input->pkt, &track))
                    return 1;
            }
            while (track != input->video_track);

            if (nestegg_packet_count(input->pkt, &input->chunks))
                return 1;

            input->chunk = 0;
        }

        if (nestegg_packet_data(input->pkt, input->chunk, buf, buf_sz))
            return 1;

        input->chunk++;
        return 0;
    }
    /* For both the raw and ivf formats, the frame size is the first 4 bytes
    * of the frame header. We just need to special case on the header
    * size.
    */
    else if (fread(raw_hdr, kind == IVF_FILE
                   ? IVF_FRAME_HDR_SZ : RAW_FRAME_HDR_SZ, 1, infile) != 1)
    {
        if (!feof(infile))
            fprintf(stderr, "Failed to read frame size\n");

        new_buf_sz = 0;
    }
    else
    {
        new_buf_sz = mem_get_le32(raw_hdr);

        if (new_buf_sz > 256 * 1024 * 1024)
        {
            fprintf(stderr, "Error: Read invalid frame size (%u)\n",
                    new_buf_sz);
            new_buf_sz = 0;
        }

        if (kind == RAW_FILE && new_buf_sz > 256 * 1024)
            fprintf(stderr, "Warning: Read invalid frame size (%u)"
                    " - not a raw file?\n", (unsigned int)new_buf_sz);

        if (new_buf_sz > *buf_alloc_sz)
        {
            uint8_t *new_buf = (uint8_t *)realloc(*buf, 2 * new_buf_sz);

            if (new_buf)
            {
                *buf = new_buf;
                *buf_alloc_sz = 2 * new_buf_sz;
            }
            else
            {
                fprintf(stderr, "Failed to allocate compressed data buffer\n");
                new_buf_sz = 0;
            }
        }
    }

    *buf_sz = new_buf_sz;

    if (*buf_sz)
    {
        /*if (fread(*buf, 1, *buf_sz, infile) != *buf_sz)
        {
        fprintf(stderr, "Failed to read full frame\n");
        return 1;
        }*/
        long size = new_buf_sz;
        fseek(infile, size, SEEK_CUR);

        return 0;
    }

    return 1;
}

unsigned int file_is_ivf_dec(FILE *infile, unsigned int *fourcc, unsigned int *width, unsigned int *height, unsigned int *fps_den, unsigned int *fps_num)
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
                fprintf(stderr, "Error: Unrecognized IVF version! This file may not"
                        " decode properly.");

            *fourcc = mem_get_le32(raw_hdr + 8);
            *width = mem_get_le16(raw_hdr + 12);
            *height = mem_get_le16(raw_hdr + 14);
            *fps_num = mem_get_le32(raw_hdr + 16);
            *fps_den = mem_get_le32(raw_hdr + 20);

            if (*fps_num < 1000)
            {
                if (*fps_num & 1)*fps_den <<= 1;
                else *fps_num >>= 1;
            }
            else
            {
                *fps_num = 30;
                *fps_den = 1;
            }
        }
    }

    if (!is_ivf)
        rewind(infile);

    return is_ivf;
}

unsigned int file_is_raw(FILE *infile, unsigned int *fourcc, unsigned int *width, unsigned int *height, unsigned int *fps_den, unsigned int *fps_num)
{
    unsigned char buf[32];
    int is_raw = 0;
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);

    if (fread(buf, 1, 32, infile) == 32)
    {
        int i;

        if (mem_get_le32(buf) < 256 * 1024 * 1024)
            for (i = 0; i < sizeof(ifaces) / sizeof(ifaces[0]); i++)
                if (!vpx_codec_peek_stream_info(ifaces[i].iface,
                                                buf + 4, 32 - 4, &si))
                {
                    is_raw = 1;
                    *fourcc = ifaces[i].fourcc;
                    *width = si.w;
                    *height = si.h;
                    *fps_num = 30;
                    *fps_den = 1;
                    break;
                }
    }

    rewind(infile);
    return is_raw;
}
static int nestegg_read_cb(void *buffer, size_t length, void *userdata)
{
    FILE *f = (FILE *)userdata;

    if (fread(buffer, 1, length, f) < length)
    {
        if (ferror(f))
            return -1;

        if (feof(f))
            return 0;
    }

    return 1;
}
static int nestegg_seek_cb(int64_t offset, int whence, void *userdata)
{
    switch (whence)
    {
    case NESTEGG_SEEK_SET:
        whence = SEEK_SET;
        break;
    case NESTEGG_SEEK_CUR:
        whence = SEEK_CUR;
        break;
    case NESTEGG_SEEK_END:
        whence = SEEK_END;
        break;
    };

    return fseek((FILE *)userdata, offset, whence) ? -1 : 0;
}
static int64_t nestegg_tell_cb(void *userdata)
{
    return ftell((FILE *)userdata);
}
static void nestegg_log_cb(nestegg *context, unsigned int severity, char const *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}
static int webm_guess_framerate(struct input_ctx *input, unsigned int     *fps_den, unsigned int     *fps_num)
{
    unsigned int i;
    uint64_t     tstamp = 0;

    for (i = 0; tstamp < 1000000000 && i < 50;)
    {
        nestegg_packet *pkt;
        unsigned int track;

        if (nestegg_read_packet(input->nestegg_ctx, &pkt) <= 0)
            break;

        nestegg_packet_track(pkt, &track);

        if (track == input->video_track)
        {
            nestegg_packet_tstamp(pkt, &tstamp);
            i++;
        }

        nestegg_free_packet(pkt);
    }

    if (nestegg_track_seek(input->nestegg_ctx, input->video_track, 0))
        goto fail;

    *fps_num = (i - 1) * 1000000;
    *fps_den = tstamp / 1000;
    return 0;
fail:
    nestegg_destroy(input->nestegg_ctx);
    input->nestegg_ctx = NULL;
    rewind(input->infile);
    return 1;
}
static int file_is_webm(struct input_ctx *input, unsigned int     *fourcc, unsigned int     *width, unsigned int     *height, unsigned int     *fps_den, unsigned int     *fps_num)
{
    unsigned int i, n;
    int          track_type = -1;
    uint64_t     tstamp = 0;
    nestegg_io io = {nestegg_read_cb, nestegg_seek_cb, nestegg_tell_cb,
                     input->infile
                    };
    nestegg_video_params params;
    nestegg_packet *pkt;

    if (nestegg_init(&input->nestegg_ctx, io, NULL))
        goto fail;

    if (nestegg_track_count(input->nestegg_ctx, &n))
        goto fail;

    for (i = 0; i < n; i++)
    {
        track_type = nestegg_track_type(input->nestegg_ctx, i);

        if (track_type == NESTEGG_TRACK_VIDEO)
            break;
        else if (track_type < 0)
            goto fail;
    }

    if (nestegg_track_codec_id(input->nestegg_ctx, i) != NESTEGG_CODEC_VP8)
    {
        fprintf(stderr, "Not VP8 video, quitting.\n");
        exit(1);
    }

    input->video_track = i;

    if (nestegg_track_video_params(input->nestegg_ctx, i, &params))
        goto fail;

    *fps_den = 0;
    *fps_num = 0;
    *fourcc = VP8_FOURCC;
    *width = params.width;
    *height = params.height;
    return 1;
fail:
    input->nestegg_ctx = NULL;
    rewind(input->infile);
    return 0;
}
void show_progress(int frame_in, int frame_out, unsigned long dx_time)
{
    fprintf(stderr, "%d decoded frames/%d showed frames in %lu us (%.2f fps)\r",
            frame_in, frame_out, dx_time,
            (float)frame_out * 1000000.0 / (float)dx_time);
}
void generate_filename(const char *pattern, char *out, size_t q_len, unsigned int d_w, unsigned int d_h, unsigned int frame_in)
{
    const char *p = pattern;
    char *q = out;

    do
    {
        char *next_pat = strchr((char *)p, '%');

        if (p == next_pat)
        {
            size_t pat_len;
            q[q_len - 1] = '\0';

            switch (p[1])
            {
            case 'w':
                snprintf(q, q_len - 1, "%d", d_w);
                break;
            case 'h':
                snprintf(q, q_len - 1, "%d", d_h);
                break;
            case '1':
                snprintf(q, q_len - 1, "%d", frame_in);
                break;
            case '2':
                snprintf(q, q_len - 1, "%02d", frame_in);
                break;
            case '3':
                snprintf(q, q_len - 1, "%03d", frame_in);
                break;
            case '4':
                snprintf(q, q_len - 1, "%04d", frame_in);
                break;
            case '5':
                snprintf(q, q_len - 1, "%05d", frame_in);
                break;
            case '6':
                snprintf(q, q_len - 1, "%06d", frame_in);
                break;
            case '7':
                snprintf(q, q_len - 1, "%07d", frame_in);
                break;
            case '8':
                snprintf(q, q_len - 1, "%08d", frame_in);
                break;
            case '9':
                snprintf(q, q_len - 1, "%09d", frame_in);
                break;
            default:
                die_dec("Unrecognized pattern %%%c\n", p[1]);
            }

            pat_len = strlen(q);

            if (pat_len >= q_len - 1)
                die_dec("Output filename too long.\n");

            q += pat_len;
            p += 2;
            q_len -= pat_len;
        }
        else
        {
            size_t copy_len;

            if (!next_pat)
                copy_len = strlen(p);
            else
                copy_len = next_pat - p;

            if (copy_len >= q_len - 1)
                die_dec("Output filename too long.\n");

            memcpy(q, p, copy_len);
            q[copy_len] = '\0';
            q += copy_len;
            p += copy_len;
            q_len -= copy_len;
        }
    }
    while (*p);
}
///////////////////////////DECODE-END/////////////////////////////////////////////////////////////////////////////////////////////////////
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
    int length = 1;


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

        if (length == 0)
            length = 1;

        if (fr == 0)
            fr = 1;

        if (h == 0)
            h = 1;

        if (w == 0)
            w = 1;

        fclose(GetWHinfile);
    }

    opt.Width = w;
    opt.Height = h;

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

        while (opt.best_allowed_q > opt.worst_allowed_q)
        {
            opt.best_allowed_q = rand() % 64;
            opt.worst_allowed_q   = rand() % 64;
        }
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

    float TBUpperBoundFloat = ((w * h) / (320 * 240) * 2048);
    int TBUpperBound = TBUpperBoundFloat;

    if (TBUpperBound == 0)
        TBUpperBound = 1;

    opt.target_bandwidth = rand() %  TBUpperBound + 1;//valid Range: No Max so based on resolution
    opt.key_freq = rand() % length + 2; //valid Range: No Max so based on number of frames

    opt.arnr_max_frames = rand() % 16; //valid Range: 0 to 15
    opt.arnr_strength = rand() % 7; //valid Range: 6
    opt.arnr_type = rand() % 3 + 1; //valid Range: 1 to 3

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

    if (display != 2)
    {
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
    }

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
    outfile << opt.arnr_max_frames << " ArnrMaxFrames\n";
    outfile << opt.arnr_strength << " ArnrStr\n";
    outfile << opt.arnr_type << " ArnrType\n";

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
int vpxt_convert_par_file_to_vpxenc(const char *input_core, const char *input_api, char *vpxenc_parameters, int vpxenc_parameters_sz)
{
    VP8_CONFIG opt;
    opt = vpxt_input_settings(input_core);

    vpx_codec_enc_cfg_t    cfg;
    const struct codec_item  *codec = codecs;
    vpx_codec_enc_config_default(codec->iface, &cfg, 0);
    vpxt_input_settings_api(input_api, cfg);

    int endofstr = 0;

    //--debug                                                                           //Debug mode (makes output deterministic)
    //--output=<arg>                                                                    //Output filename
    //--codec=<arg>                                                                     //Codec to use
    if (opt.Mode > 2)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--passes=2 ");                                          //Number of passes (1/2)

    //--pass=<arg>                                                                      //Pass to execute (1/2)
    //--fpf=<arg>                                                                       //First pass statistics file name
    //--limit=<arg>                                                                     //Stop encoding after n input frames
    //--deadline=<arg>                                                                  //Deadline per frame (usec)
    if (opt.Mode == 2 || opt.Mode == 5)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--best ");                                              //Use Best Quality Deadline

    if (opt.Mode == 1 || opt.Mode == 4)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--good ");                                              //Use Good Quality Deadline

    if (opt.Mode == 0)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--rt ");                                                //Use Realtime Quality Deadline

    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--verbose ");                                                   //Show encoder parameters
    //--psnr                                                                            //Show PSNR in status line                                                    //Output IVF (default is WebM)

    //Encoder Global Options:
    //--yv12                                                                            //Input file is YV12
    //--i420                                                                            //Input file is I420 (default)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--usage=%i ", cfg.g_usage);                                     //Usage profile number to use
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--threads=%i ", opt.multi_threaded);                            //Max number of threads to use
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--profile=%i ", opt.Version);                                   //Bitstream profile number to use
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--width=%i ", opt.Width);                                       //Frame width
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--height=%i ", opt.Height);                                     //Frame height
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--timebase=%i/%i ", cfg.g_timebase.num, cfg.g_timebase.den);    //Stream timebase (frame duration)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--fps=%i/%i ", cfg.g_timebase.den / 2, cfg.g_timebase.num);      //Stream frame rate (rate/scale)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--error-resilient=%i ", opt.error_resilient_mode);              //Enable error resiliency features

    if (opt.allow_lag == 0)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--lag-in-frames=%i ", 0);                                   //Max number of frames to lag
    else
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--lag-in-frames=%i ", opt.lag_in_frames);                   //Max number of frames to lag

    //Rate Control Options:
    if (opt.allow_df == 0)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--drop-frame=%i ", 0);                                      //Temporal resampling threshold (buf %)
    else
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--drop-frame=%i ", opt.drop_frames_water_mark);             //Temporal resampling threshold (buf %)

    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--resize-allowed=%i ", opt.allow_spatial_resampling);           //Spatial resampling enabled (bool)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--resize-up=%i ", opt.resample_up_water_mark);                  //Upscale threshold (buf %)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--resize-down=%i ", opt.resample_down_water_mark);              //Downscale threshold (buf %)

    if (opt.end_usage == USAGE_LOCAL_FILE_PLAYBACK)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--end-usage=%i ", VPX_VBR);                                 //VBR=0 | CBR=1

    if (opt.end_usage == USAGE_STREAM_FROM_SERVER)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--end-usage=%i ", VPX_CBR);                                 //VBR=0 | CBR=1

    if (opt.end_usage == USAGE_CONSTRAINED_QUALITY)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--end-usage=%i ", VPX_CQ);                                  //VBR=0 | CBR=1

    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--target-bitrate=%i ", opt.target_bandwidth);                   //Bitrate (kbps)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--min-q=%i ", opt.best_allowed_q);                              //Minimum (best) quantizer
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--max-q=%i ", opt.worst_allowed_q);                             //Maximum (worst) quantizer

    if (opt.fixed_q != -1)
    {
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--min-q=%i ", opt.fixed_q);                                 //Minimum (best) quantizer
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--max-q=%i ", opt.fixed_q);                                 //Maximum (worst) quantizer
    }

    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--undershoot-pct=%i ", opt.under_shoot_pct);                    //Datarate undershoot (min) target (%)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--overshoot-pct=%i ", cfg.rc_overshoot_pct);                    //Datarate overshoot (max) target (%)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--buf-sz=%i ", opt.maximum_buffer_size * 1000);                 //Client buffer size (ms)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--buf-initial-sz=%i ", opt.starting_buffer_level * 1000);       //Client initial buffer size (ms)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--buf-optimal-sz=%i ", opt.optimal_buffer_level * 1000);        //Client optimal buffer size (ms)

    //Twopass Rate Control Options:
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--bias-pct=%i ", opt.two_pass_vbrbias);                        //CBR/VBR bias (0=CBR, 100=VBR)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--minsection-pct=%i ", opt.two_pass_vbrmin_section);            //GOP min bitrate (% of target)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--maxsection-pct=%i ", opt.two_pass_vbrmax_section);            //GOP max bitrate (% of target)

    //Keyframe Placement Options:
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--kf-min-dist=%i ", cfg.kf_min_dist);                           //Minimum keyframe interval (frames)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--kf-max-dist=%i ", opt.key_freq);                              //Maximum keyframe interval (frames)

    if (opt.auto_key == 0)
        endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--disable-kf ");                                            //Disable keyframe placement

    //VP8 Specific Options:
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--cpu-used=%i ", opt.cpu_used);                                 //CPU Used (-16..16)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--auto-alt-ref=%i ", opt.play_alternate);                       //Enable automatic alt reference frames
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--noise-sensitivity=%i ", opt.noise_sensitivity);               //Noise sensitivity (frames to blur)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--sharpness=%i ", opt.Sharpness);                               //Filter sharpness (0-7)
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--static-thresh=%i ", opt.encode_breakout);                     //Motion detection threshold
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--token-parts=%i ", opt.token_partitions);                      //Number of token partitions to use, log2
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--arnr-maxframes=%i ", opt.arnr_max_frames);                   //AltRef Max Frames
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--arnr-strength=%i ", opt.arnr_strength);                    //AltRef Strength
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--arnr-type=%i ", opt.arnr_type);                        //AltRef Type
    //--tune=<arg>                                                                      //Material to favor - psnr, ssim
    endofstr += snprintf(vpxenc_parameters + endofstr, vpxenc_parameters_sz, "--cq-level=%i ", opt.cq_level);

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

int  vpxt_get_number_of_frames(const char *inputFile)
{
    //Get frame count from y4m ivf and or webm return -1 on error

    int length = 0;
    int use_y4m = 1;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;

    unsigned int           fourcc;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;

    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    input.infile = infile;

    if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;

    if (input.kind == WEBM_FILE)
    {
        if (webm_guess_framerate(&input, &fps_den, &fps_num))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

        while (!skim_frame_dec(&input, &buf, (size_t *) &buf_sz, (size_t *) &buf_alloc_sz))
            length++;

        //printf("\nWebm Length: %i\n",length);
        fclose(infile);

        if (input.nestegg_ctx)
            nestegg_destroy(input.nestegg_ctx);

        if (input.kind != WEBM_FILE)
            free(buf);

        return length;
    }

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    y4m_input y4m;
    unsigned int file_type;
    unsigned long nbytes = 0;
    struct detect_buffer detect;

    unsigned int rate;
    unsigned int scale;

    detect.buf_read = fread(detect.buf, 1, 4, infile);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(infile, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, infile, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            width = y4m.pic_w;
            height = y4m.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate = y4m.fps_n;
                scale = y4m.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 && file_is_ivf(infile, &fourcc, &width, &height, &detect, &scale, &rate))
    {
        file_type = FILE_TYPE_IVF;
    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(infile);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile);

        //printf("\nIVF Length: %i\n",ivfhRaw.length);

        fclose(infile);
        return ivfhRaw.length;
    }

    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1);

    if (file_type == FILE_TYPE_Y4M)
    {

        int frame_avail = 1;

        while (frame_avail)
        {
            frame_avail = skim_frame_enc(infile, &raw, file_type, &y4m, &detect);

            if (!frame_avail)
                break;

            length++;
        }

        //printf("\nY4M Length: %i\n",length);
        fclose(infile);

        vpx_img_free(&raw);
        return length;
    }

    fclose(infile);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    vpx_img_free(&raw);

    return -1;
}
int  vpxt_raw_file_size(const char *inputFile)
{
    //Get frame count from y4m ivf and or webm return -1 on error

    int size = 0;
    int use_y4m = 1;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;

    unsigned int           fourcc;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;

    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    input.infile = infile;

    if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;

    if (input.kind == WEBM_FILE)
    {
        if (webm_guess_framerate(&input, &fps_den, &fps_num))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

        while (!skim_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
            size = size + buf_sz;

        //printf("\nWebm size: %i\n",size);
        fclose(infile);
        return size;
    }

    y4m_input y4m;
    unsigned int file_type;
    unsigned long nbytes = 0;
    struct detect_buffer detect;

    unsigned int rate;
    unsigned int scale;

    detect.buf_read = fread(detect.buf, 1, 4, infile);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(infile, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, infile, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            width = y4m.pic_w;
            height = y4m.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate = y4m.fps_n;
                scale = y4m.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 && file_is_ivf(infile, &fourcc, &width, &height, &detect, &scale, &rate))
    {
        file_type = FILE_TYPE_IVF;
    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(infile);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile);

        //printf("\nIVF Length: %i\n",ivfhRaw.length);

        while (1)
        {
            IVF_FRAME_HEADER ivf_fhRaw;
            fread(&ivf_fhRaw.frameSize, 1, 4, infile);

            if (feof(infile))
                break;

            fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
            vpxt_format_frame_header_read(ivf_fhRaw);
            fseek(infile, ivf_fhRaw.frameSize, SEEK_CUR);
            size = size + ivf_fhRaw.frameSize;
        }

        //printf("\nIVF Size: %i\n",size);

        fclose(infile);
        return size;
    }

    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1);

    if (file_type == FILE_TYPE_Y4M)
    {
        int frame_avail = 1;

        while (frame_avail)
        {
            frame_avail = skim_frame_enc(infile, &raw, file_type, &y4m, &detect);

            if (!frame_avail)
                break;

            size = size + (width * height * 3 / 2);
        }

        //printf("\nY4M Size: %i\n",size);
        fclose(infile);
        vpx_img_free(&raw);
        y4m_input_close(&y4m);

        return size;
    }

    fclose(infile);
    vpx_img_free(&raw);

    if (file_type == FILE_TYPE_Y4M)
        y4m_input_close(&y4m);

    return -1;
}
int vpxt_remove_file_extension(const char *In, std::string &Out)
{
    //Takes in the full name of a file and writes the directory and file name (without its extention) to the second input.
    //return extension length on success
    int parser = 0;
    int lastDot = 0;

    while (In[parser] != '\0')
    {
        if (In[parser] == '.')
            lastDot = parser;

        parser++;
    }

    Out = In;

    if (lastDot > 0)
        Out.erase(lastDot, parser - lastDot);

    Out.append("_");

    //printf("\nOutput: %s\n",Out.c_str());

    return parser - lastDot;
}

int vpxt_get_file_extension(const char *In, std::string &Out)
{
    //Takes in the full name of a file and write the extention
    //to out return value is size of ext
    int parser = 0;
    int lastDot = 0;

    while (In[parser] != '\0')
    {
        if (In[parser] == '.')
            lastDot = parser;

        parser++;
    }

    Out = In;

    if (lastDot > 0)
        Out.erase(0, lastDot);

    return Out.length();
}

int vpxt_enc_format_append(std::string &InputString, std::string EncFormat)
{
    vpxt_lower_case_string(EncFormat);

    if (EncFormat.compare("ivf") == 0)
        InputString.append(".ivf");
    else
        InputString.append(".webm");

    return 0;
}
int vpxt_dec_format_append(std::string &InputString, std::string DecFormat)
{
    vpxt_lower_case_string(DecFormat);

    if (DecFormat.compare("ivf") == 0)
        InputString.append(".ivf");
    else
        InputString.append(".y4m");

    return 0;
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
int get_test_name(int TestNumber, std::string &TestName)
{
    if (TestNumber == RTFFINUM) TestName = "RunTestsFromFile";

    if (TestNumber == AlWDFNUM) TestName = "test_allow_drop_frames";

    if (TestNumber == ALWLGNUM) TestName = "test_allow_lag";

    if (TestNumber == ALWSRNUM) TestName = "test_allow_spatial_resampling";

    if (TestNumber == ARNRTNUM) TestName = "test_arnr";

    if (TestNumber == AUTKFNUM) TestName = "test_auto_key_frame";

    if (TestNumber == BUFLVNUM) TestName = "test_buffer_level";

    if (TestNumber == CPUDENUM) TestName = "test_change_cpu_dec";

    if (TestNumber == CPUENNUM) TestName = "test_change_cpu_enc";

    if (TestNumber == CONQUNUM) TestName = "test_constrained_quality";

    if (TestNumber == DTARTNUM) TestName = "test_data_rate";

    if (TestNumber == DBMRLNUM) TestName = "test_debug_matches_release";

    if (TestNumber == DFWMWNUM) TestName = "test_drop_frame_watermark";

    if (TestNumber == ENCBONUM) TestName = "test_encoder_break_out";

    if (TestNumber == ERRMWNUM) TestName = "test_error_resolution";

    if (TestNumber == EXTFINUM) TestName = "test_extra_file";

    if (TestNumber == FIXDQNUM) TestName = "test_fixed_quantizer";

    if (TestNumber == FKEFRNUM) TestName = "test_force_key_frame";

    if (TestNumber == FRSZTNUM) TestName = "test_frame_size";

    if (TestNumber == GQVBQNUM) TestName = "test_good_vs_best";

    if (TestNumber == LGIFRNUM) TestName = "test_lag_in_frames";

    if (TestNumber == MAXQUNUM) TestName = "test_max_quantizer";

    if (TestNumber == MEML1NUM) TestName = "test_mem_leak";

    if (TestNumber == MEML2NUM) TestName = "test_mem_leak2";

    if (TestNumber == MINQUNUM) TestName = "test_min_quantizer";

    if (TestNumber == MULTTNUM) TestName = "test_multithreaded";

    if (TestNumber == NVOECPTK) TestName = "test_new_vs_old_enc_cpu_tick";

    if (TestNumber == NVOPSNUM) TestName = "test_new_vs_old_psnr";

    if (TestNumber == NOISENUM) TestName = "test_noise_sensitivity";

    if (TestNumber == OV2PSNUM) TestName = "test_one_pass_vs_two_pass";

    if (TestNumber == PLYALNUM) TestName = "test_play_alternate";

    if (TestNumber == POSTPNUM) TestName = "test_post_processor";

    if (TestNumber == RECBFNUM) TestName = "test_reconstruct_buffer";

    if (TestNumber == RSDWMNUM) TestName = "test_resample_down_watermark";

    if (TestNumber == SPEEDNUM) TestName = "test_speed";

    if (TestNumber == TVECTNUM) TestName = "test_test_vector";

    if (TestNumber == TV2BTNUM) TestName = "test_two_pass_vs_two_pass_best";

    if (TestNumber == UNDSHNUM) TestName = "test_undershoot";

    if (TestNumber == VERSINUM) TestName = "test_version";

    if (TestNumber == VPXMINUM) TestName = "test_vpx_matches_int";

    if (TestNumber == WMLMMNUM) TestName = "test_win_lin_mac_match";

    return 0;
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

        if (id_test_str.compare("test_vpx_matches_int") == 0)
            return VPXMINUM;

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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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

                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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

                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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

                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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

                if (selector == VPXMINUM)
                {
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
                    {
                        SelectorAr[SelectorArInt].append(buffer);
                        SelectorAr2[SelectorArInt] = "VpxencMatchesIntComp";
                        PassFail[PassFailInt] = trackthis1;
                        //std::cout << "\n\n\n\n\n" << DummyArgvVar << "\n\n\n\n\n";
                    }
                    else
                    {

                        PassFail[PassFailInt] = -1;
                    }
                }

                if (selector == WMLMMNUM)
                {
                    if (!vpxt_check_arg_input(DummyArgv[1], DummyArgvVar))
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
                    selector != TV2BTNUM && selector != UNDSHNUM && selector != VERSINUM && selector != WMLMMNUM && selector != ALWSRNUM &&
                    selector != VPXMINUM)
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
int  vpxt_check_arg_input(const char *testName, int argNum)
{
    //return 1 if correct number of inputs 2 if par file input and - 1 if fail
    int selector = vpxt_identify_test(testName);

    //test_allow_drop_frames
    if (selector == AlWDFNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_allow_lag
    if (selector == ALWLGNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_allow_spatial_resampling
    if (selector == ALWSRNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_arnr
    if (selector == ARNRTNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_auto_key_frame
    if (selector == AUTKFNUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_buffer_level
    if (selector == BUFLVNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_change_cpu_dec
    if (selector == CPUDENUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_change_cpu_enc
    if (selector == CPUENNUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_constrained_quality
    if (selector == CONQUNUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_data_rate
    if (selector == DFWMWNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_debug_matches_release
    if (selector == DTARTNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_drop_frame_watermark
    if (selector == DBMRLNUM)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    //test_encoder_break_out
    if (selector == ENCBONUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_error_resolution
    if (selector == ERRMWNUM)//error res check this one
        if (argNum == 7)
            return 1;

    //test_extra_file
    if (selector == EXTFINUM)
    {
        if (argNum == 5)
            return 1;

        if (argNum == 6)
            return 2;
    }

    //test_fixed_quantizer
    if (selector == FIXDQNUM)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    //test_force_key_frame
    if (selector == FKEFRNUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_frame_size
    if (selector == FRSZTNUM)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    //test_good_vs_best
    if (selector == GQVBQNUM)
    {
        if (argNum == 6)
            return 1;

        if (argNum == 7)
            return 2;
    }

    //test_lag_in_frames
    if (selector == LGIFRNUM)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    //test_max_quantizer
    if (selector == MAXQUNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_mem_leak
    if (selector == MEML1NUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_mem_leak2
    if (selector == MEML2NUM)
    {
        if (argNum == 3)
            return 1;

        if (argNum == 4)
            return 2;
    }

    //test_min_quantizer
    if (selector == MINQUNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_multithreaded
    if (selector == MULTTNUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_new_vs_old_enc_cpu_tick
    if (selector == NVOPSNUM)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    //test_new_vs_old_psnr
    if (selector == NVOECPTK)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    //test_noise_sensitivity
    if (selector == NOISENUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_one_pass_vs_two_pass
    if (selector == OV2PSNUM)
    {
        if (argNum == 6)
            return 1;

        if (argNum == 7)
            return 2;
    }

    //test_play_alternate
    if (selector == PLYALNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_post_processor
    if (selector == POSTPNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_reconstruct_buffer
    if (selector == RECBFNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_resample_down_watermark
    if (selector == RSDWMNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_speed
    if (selector == SPEEDNUM)
    {
        if (argNum == 8)
            return 1;

        if (argNum == 9)
            return 2;
    }

    //test_test_vector
    if (selector == TVECTNUM)
        if ((argNum == 3))
            return 1;

    //test_two_pass_vs_two_pass_best
    if (selector == TV2BTNUM)
    {
        if (argNum == 6)
            return 1;

        if (argNum == 7)
            return 2;
    }

    //test_undershoot
    if (selector == UNDSHNUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_version
    if (selector == VERSINUM)
    {
        if (argNum == 7)
            return 1;

        if (argNum == 8)
            return 2;
    }

    //test_win_lin_mac_match
    if (selector == VPXMINUM)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    //test_win_lin_mac_match
    if (selector == WMLMMNUM)
    {
        if (argNum == 9)
            return 1;

        if (argNum == 10)
            return 2;
    }

    return -1;
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
void vpxt_delete_files(int argcount, ...)
{
    va_list vl;
    va_start(vl, argcount);
    tprintf(PRINT_BTH, "\n");
    int i;

    for (i = 0; i < argcount; ++i)
    {
        std::string FileToDelete = va_arg(vl, const char *);

        if (remove(FileToDelete.c_str()) == 0)
            tprintf(PRINT_BTH, "* %s - Successfully Deleted\n\n", FileToDelete.c_str());
        else
            tprintf(PRINT_BTH, "- Error: %s - Not Deleted\n\n", FileToDelete.c_str());
    }

    va_end(vl);
}
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
int vpxt_list_files_in_dir(std::vector<std::string> &FileNameVector, std::string Directory)
{
#if defined(_WIN32)
    std::string RawDirectory = Directory;
    RawDirectory.append("\\");
    Directory.append("\\*");
    WIN32_FIND_DATA FileData;
    HANDLE hFind;
    std::string FileName;
    hFind = FindFirstFile(Directory.c_str(), &FileData);

    while (FileName.compare(FileData.cFileName) != 0)
    {
        std::string FullFileName = RawDirectory;
        FileName = FileData.cFileName;
        FullFileName.append(FileName);
        FileNameVector.push_back(FullFileName);
        FindNextFile(hFind, &FileData);
    }

#else
    DIR *FileData;
    struct dirent *hFind;
    std::string FileName;
    FileData = opendir(Directory.c_str());
    hFind = readdir(FileData);

    while (FileName.compare(hFind->d_name) != 0)
    {
        FileName = hFind->d_name;
        FileNameVector.push_back(FileName);
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
//---------------------------------------------------------Evaluation------------------------------------------------------------------------
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
double vpxt_psnr(const char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, double *SsimOut)
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

    /*FILE *RawFile = fopen(inputFile1, "rb");

    if (RawFile == NULL)
    {
    tprintf(PRINT_BTH, "\nError Opening Raw File: %s\n", inputFile1);
    return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    int buffer_sz = 32;*/
    unsigned int frameCount = 0;//ivfhRaw.length;

    unsigned int             file_type, fourcc;
    struct detect_buffer detect;
    unsigned int RawWidth = 0;
    unsigned int RawHeight = 0;
    unsigned int    RawRate = 0;
    unsigned int    RawScale = 0;
    y4m_input                y4m;
    int                      arg_have_framerate = 0;
    struct vpx_rational      arg_framerate = {30, 1};
    int                      arg_use_i420 = 1;

    FILE *RawFile = strcmp(inputFile1, "-") ? fopen(inputFile1, "rb") : set_binary_mode(stdin);

    if (!RawFile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile1);
        return -1;
    }

    detect.buf_read = fread(detect.buf, 1, 4, RawFile);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(RawFile, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, RawFile, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            RawWidth = y4m.pic_w;
            RawHeight = y4m.pic_h;
            RawRate = y4m.fps_n;
            RawScale = y4m.fps_d;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (!arg_have_framerate)
            {
                arg_framerate.num = y4m.fps_n;
                arg_framerate.den = y4m.fps_d;
            }

            arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 &&
             file_is_ivf(RawFile, &fourcc, &RawWidth, &RawHeight, &detect, &RawScale, &RawRate))
    {
        file_type = FILE_TYPE_IVF;

        switch (fourcc)
        {
        case 0x32315659:
            arg_use_i420 = 0;
            break;
        case 0x30323449:
            arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type = FILE_TYPE_RAW;
    }

    if (!RawWidth || !RawHeight)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    vpx_image_t    raw_img;
    vpx_img_alloc(&raw_img, IMG_FMT_I420, RawWidth, RawHeight, 1);

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

    if (fourcc == 842094169)
        forceUVswap = 1;   //if YV12 Do not swap Frames

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.u_buffer;
        Raw_YV12.u_buffer = Raw_YV12.v_buffer;
        Raw_YV12.v_buffer = temp;
    }

    ////////////////////////Initilize Compressed File////////////////////////
    /*FILE *CompFile = fopen(inputFile2, "rb");

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
    vpxt_format_ivf_header_read(&ivfhComp);*/

    /* Open file */
    FILE *CompFile = strcmp(inputFile2, "-") ? fopen(inputFile2, "rb") : set_binary_mode(stdin);

    if (!CompFile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile2);
        return -1;
    }

    unsigned int            CompFourcc;
    unsigned int            CompWidth;
    unsigned int            CompHeight;
    unsigned int            CompScale;
    unsigned int            CompRate;
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = CompFile;

    if (file_is_ivf_dec(CompFile, &CompFourcc, &CompWidth, &CompHeight, &CompScale, &CompRate))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &CompFourcc, &CompWidth, &CompHeight, &CompScale, &CompRate))
        input.kind = WEBM_FILE;
    else if (file_is_raw(CompFile, &CompFourcc, &CompWidth, &CompHeight, &CompScale, &CompRate))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &CompScale, &CompRate))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    //if(input.kind == WEBM_FILE)
    //   if(webm_guess_framerate(&input, &CompScale, &CompRate))
    //   {
    //       fprintf(stderr, "Failed to guess framerate -- error parsing "
    //                       "webm file?\n");
    //       return EXIT_FAILURE;
    //   }

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
    const struct codec_item  *codec = codecs;
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
    vp8_yv12_alloc_frame_buffer(&Temp_YV12, RawWidth, RawHeight, 0);
    ///////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_control(&decoder, VP8_SET_POSTPROC, &ppcfg);
    //////////////////////////////////////////New//////////////////////////////////////////

    uint8_t               *CompBuff = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;

    //while (currentVideo1Frame <= frameCount - 1)
    //while(read_frame_enc(RawFile, &raw_img, file_type, &y4m, &detect))
    while (!read_frame_dec(&input, &CompBuff, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
    {
        if (file_type == 2)
        {
            raw_img.img_data = y4m.dst_buf;
            Raw_YV12.buffer_alloc        = raw_img.img_data;
            Raw_YV12.y_buffer           = raw_img.img_data;
            Raw_YV12.u_buffer = raw_img.planes[PLANE_U];
            Raw_YV12.v_buffer = raw_img.planes[PLANE_V];
        }

        unsigned long lpdwFlags = 0;
        unsigned long lpckid = 0;
        long bytes1;
        long bytes2;

        bytes2 = buf_sz;
        sumBytes2 += bytes2;

        //////////////////////Get YV12 Data For Compressed File//////////////////////
        /*IVF_FRAME_HEADER ivf_fhComp;

        if (!fread(&ivf_fhComp, 1, sizeof(ivf_fhComp), CompFile))
        {
        tprintf(PRINT_BTH, "\nError Computing PSNR 1\n");
        fclose(RawFile);
        fclose(CompFile);
        delete timeStamp2;
        delete timeEndStamp2;
        vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
        vpx_img_free(&raw_img);
        return 0;
        }

        vpxt_format_frame_header_read(ivf_fhComp);*/



        //char *CompBuff = new char[ivf_fhComp.frameSize*2];
        //read_frame_dec(&input, &CompBuff, (size_t*)&buf_sz, (size_t*)&buf_alloc_sz);
        //if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        //{
        //    tprintf(PRINT_BTH, "\nError Computing PSNR 2\n");
        //    fclose(RawFile);
        //    fclose(CompFile);
        //    delete timeStamp2;
        //    delete timeEndStamp2;
        //    delete [] CompBuff;
        //    vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
        //    vpx_img_free(&raw_img);
        //    return 0;
        //}

        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;

        if (vpx_codec_decode(&decoder, (uint8_t *) CompBuff, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));
        }

        if (img = vpx_codec_get_frame(&decoder, &iter))
        {
            ++currentVideo1Frame;

            image2yuvconfig(img, &Comp_YV12);

            if (img->d_w != RawWidth || img->d_h != RawHeight) //if frame not correct size resize it for psnr
            {
                int GCDInt1 = vpxt_gcd(img->d_w, RawWidth);
                int GCDInt2 = vpxt_gcd(img->d_h, RawHeight);

                vp8_yv12_scale_or_center(&Comp_YV12, &Temp_YV12, RawWidth, RawHeight, 0, (RawWidth / GCDInt1), (img->d_w / GCDInt1), (RawHeight / GCDInt2), (img->d_h / GCDInt2));

                Comp_YV12 = Temp_YV12;
            }

            //////////////////////Get YV12 Data For Raw File//////////////////////
            read_frame_enc(RawFile, &raw_img, file_type, &y4m, &detect);
            bytes1 = (RawWidth * RawHeight * 3) / 2; //ivf_fhRaw.frameSize;
            sumBytes += bytes1;

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
        }
        else
        {
            //delete [] CompBuff;
        }
    }

    vpx_codec_destroy(&decoder);

    //Over All PSNR Calc
    double samples = 3.0 / 2 * currentVideo1Frame * Raw_YV12.y_width * Raw_YV12.y_height;
    double avgPsnr = summedPsnr / currentVideo1Frame;
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
                    sumBytes * 8.0 / currentVideo1Frame*(RawRate / 2) / RawScale / 1000,                  //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / currentVideo1Frame*(CompRate / 2) / CompScale / 1000,               //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr,
                    1.0 * summedYPsnr / currentVideo1Frame,
                    1.0 * summedUPsnr / currentVideo1Frame,
                    1.0 * summedVPsnr / currentVideo1Frame,
                    totalPsnr);
            tprintf(PRINT_STD, SsimOut ? "SSIM: %8.2f\n" : "SSIM: Not run.", totalSSim);

        }
        else
        {
            tprintf(PRINT_BTH, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, ",
                    sumBytes * 8.0 / currentVideo1Frame*(RawRate / 2) / RawScale / 1000,          //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / currentVideo1Frame*(CompRate / 2) / CompScale / 1000,       //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr,
                    1.0 * summedYPsnr / currentVideo1Frame,
                    1.0 * summedUPsnr / currentVideo1Frame,
                    1.0 * summedVPsnr / currentVideo1Frame,
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

    vpx_codec_destroy(&decoder);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(CompBuff);

    return totalPsnr;
}
double vpxt_post_proc_psnr(const char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double *SsimOut)
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

    /*FILE *RawFile = fopen(inputFile1, "rb");

    if (RawFile == NULL)
    {
    tprintf(PRINT_BTH, "\nError Opening Raw File: %s\n", inputFile1);
    return 0;
    }

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), RawFile);
    vpxt_format_ivf_header_read(&ivfhRaw);

    int buffer_sz = 32;*/
    unsigned int frameCount = 0;//ivfhRaw.length;

    unsigned int             file_type, fourcc;
    struct detect_buffer detect;
    unsigned int RawWidth = 0;
    unsigned int RawHeight = 0;
    unsigned int    RawRate = 0;
    unsigned int    RawScale = 0;
    y4m_input                y4m;
    int                      arg_have_framerate = 0;
    struct vpx_rational      arg_framerate = {30, 1};
    int                      arg_use_i420 = 1;

    FILE *RawFile = strcmp(inputFile1, "-") ? fopen(inputFile1, "rb") : set_binary_mode(stdin);

    if (!RawFile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile1);
        return -1;
    }

    detect.buf_read = fread(detect.buf, 1, 4, RawFile);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(RawFile, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, RawFile, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            RawWidth = y4m.pic_w;
            RawHeight = y4m.pic_h;
            RawRate = y4m.fps_n;
            RawScale = y4m.fps_d;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (!arg_have_framerate)
            {
                arg_framerate.num = y4m.fps_n;
                arg_framerate.den = y4m.fps_d;
            }

            arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 &&
             file_is_ivf(RawFile, &fourcc, &RawWidth, &RawHeight, &detect, &RawScale, &RawRate))
    {
        file_type = FILE_TYPE_IVF;

        switch (fourcc)
        {
        case 0x32315659:
            arg_use_i420 = 0;
            break;
        case 0x30323449:
            arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type = FILE_TYPE_RAW;
    }

    if (!RawWidth || !RawHeight)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    vpx_image_t    raw_img;
    vpx_img_alloc(&raw_img, IMG_FMT_I420, RawWidth, RawHeight, 1);

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

    if (fourcc == 842094169)
        forceUVswap = 1;   //if YV12 Do not swap Frames

    if (forceUVswap == 1)
    {
        unsigned char *temp = Raw_YV12.u_buffer;
        Raw_YV12.u_buffer = Raw_YV12.v_buffer;
        Raw_YV12.v_buffer = temp;
    }

    ////////////////////////Initilize Compressed File////////////////////////
    /*FILE *CompFile = fopen(inputFile2, "rb");

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
    vpxt_format_ivf_header_read(&ivfhComp);*/

    /* Open file */
    FILE *CompFile = strcmp(inputFile2, "-") ? fopen(inputFile2, "rb") : set_binary_mode(stdin);

    if (!CompFile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile2);
        return -1;
    }

    unsigned int            CompFourcc;
    unsigned int            CompWidth;
    unsigned int            CompHeight;
    unsigned int            CompScale;
    unsigned int            CompRate;
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = CompFile;

    if (file_is_ivf_dec(CompFile, &CompFourcc, &CompWidth, &CompHeight, &CompScale, &CompRate))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &CompFourcc, &CompWidth, &CompHeight, &CompScale, &CompRate))
        input.kind = WEBM_FILE;
    else if (file_is_raw(CompFile, &CompFourcc, &CompWidth, &CompHeight, &CompScale, &CompRate))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &CompScale, &CompRate))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    //if(input.kind == WEBM_FILE)
    //   if(webm_guess_framerate(&input, &CompScale, &CompRate))
    //   {
    //       fprintf(stderr, "Failed to guess framerate -- error parsing "
    //                       "webm file?\n");
    //       return EXIT_FAILURE;
    //   }

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
    const struct codec_item  *codec = codecs;
    vpx_codec_iface_t  *ivf_iface = ifaces[0].iface;
    vpx_codec_dec_cfg_t     cfg = {0};
    iface = ivf_iface;

    vp8_postproc_cfg_t ppcfg = {0};

    ppcfg.deblocking_level = deblock_level2;
    ppcfg.noise_level = noise_level2;
    ppcfg.post_proc_flag = flags2;

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
    vp8_yv12_alloc_frame_buffer(&Temp_YV12, RawWidth, RawHeight, 0);
    ///////////////////////////////////////////////////////////////////////////////////////

    vpx_codec_control(&decoder, VP8_SET_POSTPROC, &ppcfg);
    //////////////////////////////////////////New//////////////////////////////////////////

    uint8_t               *CompBuff = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;

    //while (currentVideo1Frame <= frameCount - 1)
    //while(read_frame_enc(RawFile, &raw_img, file_type, &y4m, &detect))
    while (!read_frame_dec(&input, &CompBuff, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
    {
        if (file_type == 2)
        {
            raw_img.img_data = y4m.dst_buf;
            Raw_YV12.buffer_alloc        = raw_img.img_data;
            Raw_YV12.y_buffer           = raw_img.img_data;
            Raw_YV12.u_buffer = raw_img.planes[PLANE_U];
            Raw_YV12.v_buffer = raw_img.planes[PLANE_V];
        }

        unsigned long lpdwFlags = 0;
        unsigned long lpckid = 0;
        long bytes1;
        long bytes2;
        bytes2 = buf_sz;
        sumBytes2 += bytes2;

        //////////////////////Get YV12 Data For Compressed File//////////////////////
        /*IVF_FRAME_HEADER ivf_fhComp;

        if (!fread(&ivf_fhComp, 1, sizeof(ivf_fhComp), CompFile))
        {
        tprintf(PRINT_BTH, "\nError Computing PSNR 1\n");
        fclose(RawFile);
        fclose(CompFile);
        delete timeStamp2;
        delete timeEndStamp2;
        vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
        vpx_img_free(&raw_img);
        return 0;
        }

        vpxt_format_frame_header_read(ivf_fhComp);*/



        //char *CompBuff = new char[ivf_fhComp.frameSize*2];

        //read_frame_dec(&input, &CompBuff, (size_t*)&buf_sz, (size_t*)&buf_alloc_sz);

        //if (!fread(CompBuff, 1, ivf_fhComp.frameSize, CompFile))
        //{
        //    tprintf(PRINT_BTH, "\nError Computing PSNR 2\n");
        //    fclose(RawFile);
        //    fclose(CompFile);
        //    delete timeStamp2;
        //    delete timeEndStamp2;
        //    delete [] CompBuff;
        //    vp8_yv12_de_alloc_frame_buffer(&Temp_YV12);
        //    vpx_img_free(&raw_img);
        //    return 0;
        //}

        vpx_codec_iter_t  iter = NULL;
        vpx_image_t    *img;

        if (vpx_codec_decode(&decoder, (uint8_t *) CompBuff, buf_sz, NULL, 0))
        {
            const char *detail = vpx_codec_error_detail(&decoder);
            tprintf(PRINT_STD, "Failed to decode frame: %s\n", vpx_codec_error(&decoder));
        }

        if (img = vpx_codec_get_frame(&decoder, &iter))
        {
            ++currentVideo1Frame;

            image2yuvconfig(img, &Comp_YV12);

            if (img->d_w != RawWidth || img->d_h != RawHeight) //if frame not correct size resize it for psnr
            {
                int GCDInt1 = vpxt_gcd(img->d_w, RawWidth);
                int GCDInt2 = vpxt_gcd(img->d_h, RawHeight);

                vp8_yv12_scale_or_center(&Comp_YV12, &Temp_YV12, RawWidth, RawHeight, 0, (RawWidth / GCDInt1), (img->d_w / GCDInt1), (RawHeight / GCDInt2), (img->d_h / GCDInt2));

                Comp_YV12 = Temp_YV12;
            }


            //////////////////////Get YV12 Data For Raw File//////////////////////
            read_frame_enc(RawFile, &raw_img, file_type, &y4m, &detect);
            bytes1 = (RawWidth * RawHeight * 3) / 2; //ivf_fhRaw.frameSize;
            sumBytes += bytes1;

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

        }
        else
        {
            //delete [] CompBuff;
        }
    }

    vpx_codec_destroy(&decoder);

    //Over All PSNR Calc
    double samples = 3.0 / 2 * currentVideo1Frame * Raw_YV12.y_width * Raw_YV12.y_height;
    double avgPsnr = summedPsnr / currentVideo1Frame;
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
                    sumBytes * 8.0 / currentVideo1Frame*(RawRate / 2) / RawScale / 1000,                  //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / currentVideo1Frame*(CompRate / 2) / CompScale / 1000,               //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr,
                    1.0 * summedYPsnr / currentVideo1Frame,
                    1.0 * summedUPsnr / currentVideo1Frame,
                    1.0 * summedVPsnr / currentVideo1Frame,
                    totalPsnr);
            tprintf(PRINT_STD, SsimOut ? "SSIM: %8.2f\n" : "SSIM: Not run.", totalSSim);

        }
        else
        {
            tprintf(PRINT_BTH, "\nDr1:%8.2f Dr2:%8.2f, Avg: %5.2f, Avg Y: %5.2f, Avg U: %5.2f, Avg V: %5.2f, Ov PSNR: %8.2f, ",
                    sumBytes * 8.0 / currentVideo1Frame*(RawRate / 2) / RawScale / 1000,          //divided by two added when rate doubled to handle doubling of timestamp
                    sumBytes2 * 8.0 / currentVideo1Frame*(CompRate / 2) / CompScale / 1000,       //divided by two added when rate doubled to handle doubling of timestamp
                    avgPsnr,
                    1.0 * summedYPsnr / currentVideo1Frame,
                    1.0 * summedUPsnr / currentVideo1Frame,
                    1.0 * summedVPsnr / currentVideo1Frame,
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

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(CompBuff);

    return totalPsnr;
}
double vpxt_data_rate(const char *inputFile, int DROuputSel)
{
    if (DROuputSel != 2)
    {
        tprintf(PRINT_BTH, "\n--------Data Rate-------\n");

        char FileNameOnly[256];

        vpxt_file_name(inputFile, FileNameOnly, 0);
        tprintf(PRINT_BTH, "Data Rate for: %s", FileNameOnly);
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    long PosSize = vpxt_file_size(inputFile, 0);
    ///////////////////////////////////
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    unsigned int           fourcc;

    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;

    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    FILE *infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &fps_den, &fps_num))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    ///////////////////////////////////
    //frameCount = ivfhRaw.length;



    //fpos_t position;
    //fgetpos(in, &position);
    tprintf(PRINT_STD, "\n");

    long nBytes = 0;
    long nBytesMin = 999999;
    long nBytesMax = 0;

    while (!skim_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
    {
        //fread(&ivf_fhRaw.frameSize, 1, 4, in);
        //fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        //vpxt_format_frame_header_read(ivf_fhRaw);

        //nBytes = nBytes + ivf_fhRaw.frameSize;
        nBytes = nBytes + buf_sz;

        if (buf_sz < nBytesMin)
        {
            nBytesMin = buf_sz;
        }

        if (buf_sz > nBytesMax)
        {
            nBytesMax = buf_sz;
        }

        //fseek(in , ivf_fhRaw.frameSize , SEEK_CUR);

        currentVideoFrame ++;
    }

    long nSamples = currentVideoFrame;
    long lRateNum = fps_num; //divided by two added when rate doubled to handle doubling of timestamp
    long lRateDenom = fps_den;

    long nSamplesPerBlock = 1;

    double dRateFactor = static_cast<double>(lRateNum) / static_cast<double>(lRateDenom) * static_cast<double>(8) / static_cast<double>(1000);

    double Avg = (double)nBytes * (double)dRateFactor / (double)nSamples;
    double Min = (double)nBytesMin * (double)dRateFactor / (double)nSamplesPerBlock;
    double Max = (double)nBytesMax * (double)dRateFactor / (double)nSamplesPerBlock;
    double File = (double)PosSize * (double)dRateFactor / (double)nSamples;

    tprintf(PRINT_STD, "\nData rate for frames 0..%i\n", currentVideoFrame - 1);
    tprintf(PRINT_STD, "Average %*.2f kb/s\n", 10, Avg);
    tprintf(PRINT_STD, "Min     %*.2f kb/s\n", 10, Min);
    tprintf(PRINT_STD, "Max     %*.2f kb/s\n", 10, Max);
    tprintf(PRINT_STD, "File    %*.2f kb/s\n", 10, File);

    if (DROuputSel == 1)
    {
        fprintf(stderr, "\nData rate for frames 0..%i\n", currentVideoFrame - 1);
        fprintf(stderr, "Average %*.2f kb/s\n", 10, Avg);
        fprintf(stderr, "Min     %*.2f kb/s\n", 10, Min);
        fprintf(stderr, "Max     %*.2f kb/s\n", 10, Max);
        fprintf(stderr, "File    %*.2f kb/s\n", 10, File);
    }

    //fclose(in);

    if (DROuputSel != 2)
    {
        tprintf(PRINT_BTH, "\n------------------------\n");
    }

    //   delete [] outputVideoBuffer;
    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    return Avg;
}

int vpxt_check_pbm(const char *inputFile, int bitRate, int maxBuffer, int preBuffer)
{

    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    unsigned int            fourcc;

    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;

    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    FILE *infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &fps_den, &fps_num))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    //frameCount = ivfhRaw.length;
    int nFrameFail = 0;

    bool checkOverrun = false;
    double secondsperframe = ((double)fps_den / (double)fps_num);//(the *2 is due to scale being doubled for extra frame padding)
    int bitsAddedPerFrame = ((bitRate * 1000 * secondsperframe)) - .5; //-.5 to cancel out rounding
    int bitsInBuffer = preBuffer * bitRate;
    int maxBitsInBuffer = maxBuffer * bitRate;

    while (!skim_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
    {
        bitsInBuffer += bitsAddedPerFrame;
        bitsInBuffer -= buf_sz * 8;

        if (bitsInBuffer < 0.)
        {
            if (input.nestegg_ctx)
                nestegg_destroy(input.nestegg_ctx);

            if (input.kind != WEBM_FILE)
                free(buf);

            fclose(infile);
            return currentVideoFrame;
        }

        if (bitsInBuffer > maxBitsInBuffer)
        {
            if (checkOverrun)
            {
                if (input.nestegg_ctx)
                    nestegg_destroy(input.nestegg_ctx);

                if (input.kind != WEBM_FILE)
                    free(buf);

                fclose(infile);
                return currentVideoFrame;
            }
            else
            {
                bitsInBuffer = maxBitsInBuffer;
            }
        }

        currentVideoFrame ++;
    }

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);
    return -11;
}
int vpxt_check_pbm_threshold(const char *inputFile, double bitRate, int maxBuffer, int preBuffer, int optimalbuffer, int Threshold)
{
    //std::string ResizeInStr = inputFile;
    //ResizeInStr.erase(ResizeInStr.length() - 4, 4);
    std::string ResizeInStr;
    vpxt_remove_file_extension(inputFile, ResizeInStr);
    ResizeInStr.append("CheckPBMThresh.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", ResizeInStr.c_str());

    FILE *out;

    int PrintSwitch = 1;

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");
    }

    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    unsigned int            fourcc;

    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;

    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    FILE *infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &fps_den, &fps_num))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    //frameCount = ivfhRaw.length;
    int nFrameFail = 0;

    bool checkOverrun = false;
    double secondsperframe = ((double)fps_den / (double)fps_num);//(the *2 is due to scale being doubled for extra frame padding)
    int bitsAddedPerFrame = ((bitRate * 1000 * secondsperframe)) - .5; //-.5 to cancel out rounding
    int bitsInBuffer = preBuffer * bitRate;
    int maxBitsInBuffer = maxBuffer * bitRate;

    while (!skim_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
    {
        bitsInBuffer += bitsAddedPerFrame;
        bitsInBuffer -= buf_sz * 8;

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

        currentVideoFrame ++;
    }

    fclose(infile);

    if (PrintSwitch == 1)
    {
        fclose(out);
    }

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

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
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    unsigned int           fourcc;
    const char            *fn = inputChar;

    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;

    struct input_ctx        input;
    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    //FILE *infile = fopen(inputChar, "rb");

    //char raw_hdr[32];
    //int is_ivf = 0;

    //if (fread(raw_hdr, 1, 32, infile) == 32)
    //{
    //  if (raw_hdr[0] == 'D' && raw_hdr[1] == 'K'
    //      && raw_hdr[2] == 'I' && raw_hdr[3] == 'F')
    //  {
    //      is_ivf = 1;
    //      ///////////////////Write Header Info///////////////////
    //      IVF_HEADER ivfhRaw;
    //      memcpy(&ivfhRaw, raw_hdr, 32);
    //      ivfhRaw.four_cc = 808596553;
    //      width = ivfhRaw.width;
    //      height = ivfhRaw.height;
    //      ///////////////////////////////////////////////////////
    //  }
    //}

    vpx_codec_dec_init(&decoder, iface, &cfg, 0);
    read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz);
    //read_frame_dec(infile, &buf, &buf_sz, &buf_alloc_sz, is_ivf);
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
void print_header_info_to_file(const char *FileName)
{
    FILE *outfile;
    outfile = fopen(FileName, "a");

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

    fprintf(outfile, "%s", TestMachineInfo.c_str());

    fclose(outfile);
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
//----------------------------------------------------------Enc/Dec-------------------------------------------------------------------------
#ifdef API
int vpxt_compress(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck, std::string EncFormat)
{
    int                      write_webm = 1;
    vpxt_lower_case_string(EncFormat);

    if (EncFormat.compare("ivf") == 0)
        write_webm = 0;

    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    std::ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        //std::string QuantOutStr = outputFile2;
        //QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        std::string QuantOutStr;
        vpxt_remove_file_extension(outputFile2, QuantOutStr);
        QuantOutStr.append("quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        if (RunQCheck == 1)
            QuantOutFile.close();

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    //new
    unsigned int             file_type, fourcc;
    y4m_input                y4m;
    int                      arg_have_framerate = 0;
    EbmlGlobal               ebml = {0};
    uint32_t                 hash = 0;


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
            QuantOutFile.close();

        return -1;
    }

    //////////////////////Update CFG Settings Here////////////////////
    // if mode == 4 or 5 arg_passes = 2
    vpxt_core_config_to_api_config(oxcf, &cfg);

    //FILE *GetWHinfile = fopen(in_fn, "rb");

    //if (GetWHinfile == NULL)
    //{
    //    tprintf(PRINT_BTH, "Input File not found: %s\n", in_fn);

    //    if (RunQCheck == 1)
    //        QuantOutFile.close();

    //    return -1;
    //}

    //IVF_HEADER ivfhRaw;
    //IVF_FRAME_HEADER ivf_fhRaw;
    //InitIVFHeader(&ivfhRaw);
    //fread(&ivfhRaw, 1, sizeof(ivfhRaw), GetWHinfile);
    //vpxt_format_ivf_header_read(&ivfhRaw);

    //if (ivfhRaw.four_cc == 842094169) //if yv12
    //{
    //    arg_use_i420 = 0;
    //}

    //int w       = ivfhRaw.width;
    //int h       = ivfhRaw.height;
    //int fr      = (ivfhRaw.rate / ivfhRaw.scale);

    //oxcf.Width = w;
    //oxcf.Height = h;

    //cfg.g_w = w;                                                          //
    //cfg.g_h = h;
    //cfg.g_timebase.num = 1000;
    //cfg.g_timebase.den = fr * 1000;
    //fclose(GetWHinfile);

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

    /*vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
    cfg.g_w, cfg.g_h, 1);*/

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

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;
        struct detect_buffer detect;

        infile = strcmp(in_fn, "-") ? fopen(in_fn, "rb") : set_binary_mode(stdin);

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        detect.buf_read = fread(detect.buf, 1, 4, infile);
        detect.position = 0;

        unsigned int rate;
        unsigned int scale;

        if (detect.buf_read == 4 && file_is_y4m(infile, &y4m, detect.buf))
        {
            if (y4m_input_open(&y4m, infile, detect.buf, 4) >= 0)
            {
                file_type = FILE_TYPE_Y4M;
                cfg.g_w = y4m.pic_w;
                cfg.g_h = y4m.pic_h;

                /* Use the frame rate from the file only if none was specified
                * on the command-line.
                */
                if (!arg_have_framerate)
                {
                    arg_framerate.num = y4m.fps_n;
                    arg_framerate.den = y4m.fps_d;
                }

                arg_use_i420 = 0;
            }
            else
            {
                fprintf(stderr, "Unsupported Y4M stream.\n");
                return EXIT_FAILURE;
            }
        }
        else if (detect.buf_read == 4 && file_is_ivf(infile, &fourcc, &cfg.g_w, &cfg.g_h, &detect, &scale, &rate))
        {
            file_type = FILE_TYPE_IVF;

            switch (fourcc)
            {
            case 0x32315659:
                arg_use_i420 = 0;
                break;
            case 0x30323449:
                arg_use_i420 = 1;
                break;
            default:
                fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
                return EXIT_FAILURE;
            }
        }
        else
        {
            file_type = FILE_TYPE_RAW;
        }

        if (!cfg.g_w || !cfg.g_h)
        {
            fprintf(stderr, "Specify stream dimensions with --width (-w) "
                    " and --height (-h).\n");
            return EXIT_FAILURE;
        }

        std::string inputformat;
        std::string outputformat;

        if (write_webm == 1)
            outputformat = "Webm";

        if (write_webm == 0)
            outputformat = "IVF";

        if (file_type == FILE_TYPE_RAW)
            inputformat = "Raw";

        if (file_type == FILE_TYPE_Y4M)
            inputformat = "Y4M";

        if (file_type == FILE_TYPE_IVF)
            inputformat = "IVF";

        //tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");
        tprintf(PRINT_BTH, "API - Compressing Raw %s File to VP8 %s File: \n", inputformat.c_str(), outputformat.c_str());

        //////////////////////read in junk IVFData//////////////////////
        //fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile); //Read In File Header
        ////////////////////////////////////////////////////////////////
        //outfile = fopen(out_fn, "wb");

        if (pass == (one_pass_only ? one_pass_only - 1 : 0))
        {
            if (file_type == FILE_TYPE_Y4M)
                /*The Y4M reader does its own allocation.
                Just initialize this here to avoid problems if we never read any
                frames.*/
                memset(&raw, 0, sizeof(raw));
            else
                vpx_img_alloc(&raw, arg_use_i420 ? VPX_IMG_FMT_I420 : VPX_IMG_FMT_YV12,
                              cfg.g_w, cfg.g_h, 1);

            // This was added so that ivfenc will create monotically increasing
            // timestamps.  Since we create new timestamps for alt-reference frames
            // we need to make room in the series of timestamps.  Since there can
            // only be 1 alt-ref frame ( current bitstream) multiplying by 2
            // gives us enough room.
        }

        outfile = strcmp(out_fn, "-") ? fopen(out_fn, "wb") : set_binary_mode(stdout);

        if (!outfile)
        {
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
            fclose(infile);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        if (write_webm && fseek(outfile, 0, SEEK_CUR))
        {
            fprintf(stderr, "WebM output to pipes not supported.\n");
            fclose(infile);
            fclose(outfile);

            if (RunQCheck == 1)
                QuantOutFile.close();

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
                    QuantOutFile.close();

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
                    QuantOutFile.close();

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

        if (write_webm)
        {
            ebml.stream = outfile;
            write_webm_file_header(&ebml, &cfg, &arg_framerate);
        }
        else
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
                QuantOutFile.close();

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            //std::string OutputsettingsFile = outputFile2;
            //OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            std::string OutputsettingsFile;
            int extLength = vpxt_remove_file_extension(outputFile2, OutputsettingsFile);
            OutputsettingsFile.append("parameters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - (15 + extLength), 15 + extLength);
            OutputsettingsFile.append("_parameters_vpx.txt");
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
                //fread(&ivf_fhRaw.frameSize, 1, 4, infile);
                //fread(&ivf_fhRaw.timeStamp, 1, 8, infile);
                ////////////////////////////////////////////////////////////////
                frame_avail = read_frame_enc(infile, &raw, file_type, &y4m, &detect);

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
                got_data = 1;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    frames_out++;

                    //fprintf(stderr, " %6luF", (unsigned long)pkt->data.frame.sz);
                    if (write_webm)
                    {
                        if (!ebml.debug)
                            hash = murmur(pkt->data.frame.buf,
                                          pkt->data.frame.sz, hash);

                        write_webm_block(&ebml, &cfg, pkt);
                    }
                    else
                    {
                        write_ivf_frame_header(outfile, pkt);

                        if (fwrite(pkt->data.frame.buf, 1,
                                   pkt->data.frame.sz, outfile));
                    }

                    nbytes += pkt->data.raw.sz;
                    //tprintf(PRINT_BTH, "Encoded Frame Size: %i\n",pkt->data.frame.sz);
                    break;
                case VPX_CODEC_STATS_PKT:
                    frames_out++;
                    /*fprintf(stderr, " %6luS",
                    (unsigned long)pkt->data.twopass_stats.sz);*/
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

        if (write_webm)
        {
            write_webm_file_footer(&ebml, hash);
        }
        else
        {
            if (!fseek(outfile, 0, SEEK_SET))
                write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);
        }

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_BTH, "\n");
    }

    vpx_img_free(&raw);
    free(ebml.cue_list);

    if (RunQCheck == 1)
        QuantOutFile.close();

    return 0;
}
int vpxt_compress_no_error_output(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck, std::string EncFormat)
{
    int                      write_webm = 1;
    vpxt_lower_case_string(EncFormat);

    if (EncFormat.compare("ivf") == 0)
        write_webm = 0;

    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    std::ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        //std::string QuantOutStr = outputFile2;
        //QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        std::string QuantOutStr;
        vpxt_remove_file_extension(outputFile2, QuantOutStr);
        QuantOutStr.append("quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());
        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        if (RunQCheck == 1)
            QuantOutFile.close();

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //new
    unsigned int             file_type, fourcc;
    y4m_input                y4m;
    int                      arg_have_framerate = 0;
    EbmlGlobal               ebml = {0};
    uint32_t                 hash = 0;

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

        if (RunQCheck == 1)
            QuantOutFile.close();

        return -1;
    }

    vpxt_core_config_to_api_config(oxcf, &cfg);

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

    /*vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
    cfg.g_w, cfg.g_h, 1);*/

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

        //tprintf(PRINT_STD, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;
        struct detect_buffer detect;

        infile = strcmp(in_fn, "-") ? fopen(in_fn, "rb") : set_binary_mode(stdin);

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        detect.buf_read = fread(detect.buf, 1, 4, infile);
        detect.position = 0;
        unsigned int rate;
        unsigned int scale;

        if (detect.buf_read == 4 && file_is_y4m(infile, &y4m, detect.buf))
        {
            if (y4m_input_open(&y4m, infile, detect.buf, 4) >= 0)
            {
                file_type = FILE_TYPE_Y4M;
                cfg.g_w = y4m.pic_w;
                cfg.g_h = y4m.pic_h;

                /* Use the frame rate from the file only if none was specified
                * on the command-line.
                */
                if (!arg_have_framerate)
                {
                    arg_framerate.num = y4m.fps_n;
                    arg_framerate.den = y4m.fps_d;
                }

                arg_use_i420 = 0;
            }
            else
            {
                fprintf(stderr, "Unsupported Y4M stream.\n");
                return EXIT_FAILURE;
            }
        }
        else if (detect.buf_read == 4 &&
                 file_is_ivf(infile, &fourcc, &cfg.g_w, &cfg.g_h, &detect, &scale, &rate))
        {
            file_type = FILE_TYPE_IVF;

            switch (fourcc)
            {
            case 0x32315659:
                arg_use_i420 = 0;
                break;
            case 0x30323449:
                arg_use_i420 = 1;
                break;
            default:
                fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
                return EXIT_FAILURE;
            }
        }
        else
        {
            file_type = FILE_TYPE_RAW;
        }

        if (!cfg.g_w || !cfg.g_h)
        {
            fprintf(stderr, "Specify stream dimensions with --width (-w) "
                    " and --height (-h).\n");
            return EXIT_FAILURE;
        }

        std::string inputformat;
        std::string outputformat;

        if (write_webm == 1)
            outputformat = "Webm";

        if (write_webm == 0)
            outputformat = "IVF";

        if (file_type == FILE_TYPE_RAW)
            inputformat = "Raw";

        if (file_type == FILE_TYPE_Y4M)
            inputformat = "Y4M";

        if (file_type == FILE_TYPE_IVF)
            inputformat = "IVF";

        tprintf(PRINT_STD, "API - Compressing Raw %s File to VP8 %s File: \n", inputformat.c_str(), outputformat.c_str());

        if (pass == (one_pass_only ? one_pass_only - 1 : 0))
        {
            if (file_type == FILE_TYPE_Y4M)
                /*The Y4M reader does its own allocation.
                Just initialize this here to avoid problems if we never read any
                frames.*/
                memset(&raw, 0, sizeof(raw));
            else
                vpx_img_alloc(&raw, arg_use_i420 ? VPX_IMG_FMT_I420 : VPX_IMG_FMT_YV12,
                              cfg.g_w, cfg.g_h, 1);

            // This was added so that ivfenc will create monotically increasing
            // timestamps.  Since we create new timestamps for alt-reference frames
            // we need to make room in the series of timestamps.  Since there can
            // only be 1 alt-ref frame ( current bitstream) multiplying by 2
            // gives us enough room.
        }

        outfile = strcmp(out_fn, "-") ? fopen(out_fn, "wb") : set_binary_mode(stdout);

        if (!outfile)
        {
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
            fclose(infile);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        if (write_webm && fseek(outfile, 0, SEEK_CUR))
        {
            fprintf(stderr, "WebM output to pipes not supported.\n");
            fclose(infile);
            fclose(outfile);

            if (RunQCheck == 1)
                QuantOutFile.close();

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
                    QuantOutFile.close();

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
                    QuantOutFile.close();

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

        if (write_webm)
        {
            ebml.stream = outfile;
            write_webm_file_header(&ebml, &cfg, &arg_framerate);
        }
        else
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
                QuantOutFile.close();

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            //std::string OutputsettingsFile = outputFile2;
            //OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            std::string OutputsettingsFile;
            int extLength = vpxt_remove_file_extension(outputFile2, OutputsettingsFile);
            OutputsettingsFile.append("parameters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - (15 + extLength), 15 + extLength);
            OutputsettingsFile.append("_parameters_vpx.txt");
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

                frame_avail = read_frame_enc(infile, &raw, file_type, &y4m,
                                             &detect);

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
                got_data = 1;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    frames_out++;

                    //fprintf(stderr, " %6luF", (unsigned long)pkt->data.frame.sz);
                    if (write_webm)
                    {
                        if (!ebml.debug)
                            hash = murmur(pkt->data.frame.buf,
                                          pkt->data.frame.sz, hash);

                        write_webm_block(&ebml, &cfg, pkt);
                    }
                    else
                    {
                        write_ivf_frame_header(outfile, pkt);

                        if (fwrite(pkt->data.frame.buf, 1,
                                   pkt->data.frame.sz, outfile));
                    }

                    nbytes += pkt->data.raw.sz;
                    //tprintf(PRINT_BTH, "Encoded Frame Size: %i\n",pkt->data.frame.sz);
                    break;
                case VPX_CODEC_STATS_PKT:
                    frames_out++;
                    /*fprintf(stderr, " %6luS",
                    (unsigned long)pkt->data.twopass_stats.sz);*/
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
            //uint64_t temp= (uint64_t)frames_in * 1000000;
        }
        vpx_codec_destroy(&encoder);

        fclose(infile);

        if (write_webm)
        {
            write_webm_file_footer(&ebml, hash);
        }
        else
        {
            if (!fseek(outfile, 0, SEEK_SET))
                write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);
        }

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_STD, "\n");
    }

    vpx_img_free(&raw);
    free(ebml.cue_list);

    if (RunQCheck == 1)
        QuantOutFile.close();

    return 0;
}
unsigned int vpxt_time_compress(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck, unsigned int &CPUTick, std::string EncFormat)
{
    int                      write_webm = 1;
    vpxt_lower_case_string(EncFormat);

    if (EncFormat.compare("ivf") == 0)
        write_webm = 0;

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
            QuantOutFile.close();

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //new
    unsigned int             file_type, fourcc;
    y4m_input                y4m;
    int                      arg_have_framerate = 0;
    EbmlGlobal               ebml = {0};
    uint32_t                 hash = 0;

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
    res = vpx_codec_enc_config_default(codec->iface, &cfg, arg_usage);

    if (res)
    {
        tprintf(PRINT_STD, "Failed to get config: %s\n", vpx_codec_err_to_string(res));

        if (RunQCheck == 1)
            QuantOutFile.close();

        return -1;
    }

    vpxt_core_config_to_api_config(oxcf, &cfg);

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

    /*vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
    cfg.g_w, cfg.g_h, 1);*/

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

        //tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;
        struct detect_buffer detect;

        infile = strcmp(in_fn, "-") ? fopen(in_fn, "rb") : set_binary_mode(stdin);

        if (!infile)
        {
            tprintf(PRINT_STD, "Failed to open input file");

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        detect.buf_read = fread(detect.buf, 1, 4, infile);
        detect.position = 0;

        unsigned int rate;
        unsigned int scale;

        if (detect.buf_read == 4 && file_is_y4m(infile, &y4m, detect.buf))
        {
            if (y4m_input_open(&y4m, infile, detect.buf, 4) >= 0)
            {
                file_type = FILE_TYPE_Y4M;
                cfg.g_w = y4m.pic_w;
                cfg.g_h = y4m.pic_h;

                /* Use the frame rate from the file only if none was specified
                * on the command-line.
                */
                if (!arg_have_framerate)
                {
                    arg_framerate.num = y4m.fps_n;
                    arg_framerate.den = y4m.fps_d;
                }

                arg_use_i420 = 0;
            }
            else
            {
                fprintf(stderr, "Unsupported Y4M stream.\n");
                return EXIT_FAILURE;
            }
        }
        else if (detect.buf_read == 4 &&
                 file_is_ivf(infile, &fourcc, &cfg.g_w, &cfg.g_h, &detect, &scale, &rate))
        {
            file_type = FILE_TYPE_IVF;

            switch (fourcc)
            {
            case 0x32315659:
                arg_use_i420 = 0;
                break;
            case 0x30323449:
                arg_use_i420 = 1;
                break;
            default:
                fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
                return EXIT_FAILURE;
            }
        }
        else
        {
            file_type = FILE_TYPE_RAW;
        }

        if (!cfg.g_w || !cfg.g_h)
        {
            fprintf(stderr, "Specify stream dimensions with --width (-w) "
                    " and --height (-h).\n");
            return EXIT_FAILURE;
        }

        std::string inputformat;
        std::string outputformat;

        if (write_webm == 1)
            outputformat = "Webm";

        if (write_webm == 0)
            outputformat = "IVF";

        if (file_type == FILE_TYPE_RAW)
            inputformat = "Raw";

        if (file_type == FILE_TYPE_Y4M)
            inputformat = "Y4M";

        if (file_type == FILE_TYPE_IVF)
            inputformat = "IVF";

        tprintf(PRINT_BTH, "API - Compressing Raw %s File to VP8 %s File: \n", inputformat.c_str(), outputformat.c_str());

        if (pass == (one_pass_only ? one_pass_only - 1 : 0))
        {
            if (file_type == FILE_TYPE_Y4M)
                /*The Y4M reader does its own allocation.
                Just initialize this here to avoid problems if we never read any
                frames.*/
                memset(&raw, 0, sizeof(raw));
            else
                vpx_img_alloc(&raw, arg_use_i420 ? VPX_IMG_FMT_I420 : VPX_IMG_FMT_YV12,
                              cfg.g_w, cfg.g_h, 1);

            // This was added so that ivfenc will create monotically increasing
            // timestamps.  Since we create new timestamps for alt-reference frames
            // we need to make room in the series of timestamps.  Since there can
            // only be 1 alt-ref frame ( current bitstream) multiplying by 2
            // gives us enough room.
        }

        outfile = strcmp(out_fn, "-") ? fopen(out_fn, "wb") : set_binary_mode(stdout);

        if (!outfile)
        {
            tprintf(PRINT_STD, "Failed to open output file");
            fclose(infile);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        if (write_webm && fseek(outfile, 0, SEEK_CUR))
        {
            fprintf(stderr, "WebM output to pipes not supported.\n");
            fclose(infile);
            fclose(outfile);

            if (RunQCheck == 1)
                QuantOutFile.close();

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
                    QuantOutFile.close();

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
                    QuantOutFile.close();

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

        if (write_webm)
        {
            ebml.stream = outfile;
            write_webm_file_header(&ebml, &cfg, &arg_framerate);
        }
        else
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
                QuantOutFile.close();

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            //std::string OutputsettingsFile = outputFile2;
            //OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            std::string OutputsettingsFile;
            int extLength = vpxt_remove_file_extension(outputFile2, OutputsettingsFile);
            OutputsettingsFile.append("parameters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - (15 + extLength), 15 + extLength);
            OutputsettingsFile.append("_parameters_vpx.txt");
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

                frame_avail = read_frame_enc(infile, &raw, file_type, &y4m,
                                             &detect);

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

            total_cpu_time_used = total_cpu_time_used + (end - start);

            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)))
            {
                got_data = 1;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    frames_out++;

                    //fprintf(stderr, " %6luF", (unsigned long)pkt->data.frame.sz);
                    if (write_webm)
                    {
                        if (!ebml.debug)
                            hash = murmur(pkt->data.frame.buf,
                                          pkt->data.frame.sz, hash);

                        write_webm_block(&ebml, &cfg, pkt);
                    }
                    else
                    {
                        write_ivf_frame_header(outfile, pkt);

                        if (fwrite(pkt->data.frame.buf, 1,
                                   pkt->data.frame.sz, outfile));
                    }

                    nbytes += pkt->data.raw.sz;
                    //tprintf(PRINT_BTH, "Encoded Frame Size: %i\n",pkt->data.frame.sz);
                    break;
                case VPX_CODEC_STATS_PKT:
                    frames_out++;
                    /*fprintf(stderr, " %6luS",
                    (unsigned long)pkt->data.twopass_stats.sz);*/
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

        if (write_webm)
        {
            write_webm_file_footer(&ebml, hash);
        }
        else
        {
            if (!fseek(outfile, 0, SEEK_SET))
                write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);
        }

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_STD, "\n");
    }

    vpx_img_free(&raw);
    free(ebml.cue_list);

    tprintf(PRINT_BTH, "\n File completed: Time in Microseconds: %u, Fps: %d \n",
            cx_time, 1000 * framesoutrec / (cx_time / 1000));
    tprintf(PRINT_BTH, " Total CPU Ticks: %u\n", total_cpu_time_used);

    //char TextFilechar1[255];
    //char TextFilechar2[255];
    std::string FullNameMs;
    std::string FullNameCpu;

    vpxt_remove_file_extension(outputFile2, FullNameMs);
    vpxt_remove_file_extension(outputFile2, FullNameCpu);

    //char *FullNameMs = strcat(TextFilechar1, "compression_time.txt");
    FullNameMs.append("compression_time.txt");

    std::ofstream FullNameMsFile(FullNameMs.c_str());
    FullNameMsFile << cx_time;
    FullNameMsFile.close();

    //char *FullNameCpu = strcat(TextFilechar2, "compression_cpu_tick.txt");
    FullNameCpu.append("compression_cpu_tick.txt");

    std::ofstream FullNameCpuFile(FullNameCpu.c_str());
    FullNameCpuFile << total_cpu_time_used;
    FullNameCpuFile.close();

    CPUTick = total_cpu_time_used;

    if (RunQCheck == 1)
        QuantOutFile.close();

    return cx_time;
}
int vpxt_compress_force_key_frame(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck, int forceKeyFrame, std::string EncFormat)
{
    int                      write_webm = 1;
    vpxt_lower_case_string(EncFormat);

    if (EncFormat.compare("ivf") == 0)
        write_webm = 0;

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
            QuantOutFile.close();

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //new
    unsigned int             file_type, fourcc;
    y4m_input                y4m;
    int                      arg_have_framerate = 0;
    EbmlGlobal               ebml = {0};
    uint32_t                 hash = 0;

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
            QuantOutFile.close();

        return -1;
    }

    vpxt_core_config_to_api_config(oxcf, &cfg);

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

    /*vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
    cfg.g_w, cfg.g_h, 1);*/

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

        //tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;
        struct detect_buffer detect;

        infile = strcmp(in_fn, "-") ? fopen(in_fn, "rb") : set_binary_mode(stdin);

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        detect.buf_read = fread(detect.buf, 1, 4, infile);
        detect.position = 0;

        unsigned int rate;
        unsigned int scale;

        if (detect.buf_read == 4 && file_is_y4m(infile, &y4m, detect.buf))
        {
            if (y4m_input_open(&y4m, infile, detect.buf, 4) >= 0)
            {
                file_type = FILE_TYPE_Y4M;
                cfg.g_w = y4m.pic_w;
                cfg.g_h = y4m.pic_h;

                /* Use the frame rate from the file only if none was specified
                * on the command-line.
                */
                if (!arg_have_framerate)
                {
                    arg_framerate.num = y4m.fps_n;
                    arg_framerate.den = y4m.fps_d;
                }

                arg_use_i420 = 0;
            }
            else
            {
                fprintf(stderr, "Unsupported Y4M stream.\n");
                return EXIT_FAILURE;
            }
        }
        else if (detect.buf_read == 4 &&
                 file_is_ivf(infile, &fourcc, &cfg.g_w, &cfg.g_h, &detect, &scale, &rate))
        {
            file_type = FILE_TYPE_IVF;

            switch (fourcc)
            {
            case 0x32315659:
                arg_use_i420 = 0;
                break;
            case 0x30323449:
                arg_use_i420 = 1;
                break;
            default:
                fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
                return EXIT_FAILURE;
            }
        }
        else
        {
            file_type = FILE_TYPE_RAW;
        }

        if (!cfg.g_w || !cfg.g_h)
        {
            fprintf(stderr, "Specify stream dimensions with --width (-w) "
                    " and --height (-h).\n");
            return EXIT_FAILURE;
        }

        std::string inputformat;
        std::string outputformat;

        if (write_webm == 1)
            outputformat = "Webm";

        if (write_webm == 0)
            outputformat = "IVF";

        if (file_type == FILE_TYPE_RAW)
            inputformat = "Raw";

        if (file_type == FILE_TYPE_Y4M)
            inputformat = "Y4M";

        if (file_type == FILE_TYPE_IVF)
            inputformat = "IVF";

        tprintf(PRINT_BTH, "API - Compressing Raw %s File to VP8 %s File: \n", inputformat.c_str(), outputformat.c_str());

        if (pass == (one_pass_only ? one_pass_only - 1 : 0))
        {
            if (file_type == FILE_TYPE_Y4M)
                /*The Y4M reader does its own allocation.
                Just initialize this here to avoid problems if we never read any
                frames.*/
                memset(&raw, 0, sizeof(raw));
            else
                vpx_img_alloc(&raw, arg_use_i420 ? VPX_IMG_FMT_I420 : VPX_IMG_FMT_YV12,
                              cfg.g_w, cfg.g_h, 1);

            // This was added so that ivfenc will create monotically increasing
            // timestamps.  Since we create new timestamps for alt-reference frames
            // we need to make room in the series of timestamps.  Since there can
            // only be 1 alt-ref frame ( current bitstream) multiplying by 2
            // gives us enough room.
        }

        outfile = strcmp(out_fn, "-") ? fopen(out_fn, "wb") : set_binary_mode(stdout);

        if (!outfile)
        {
            tprintf(PRINT_BTH, "Failed to open output file: %s", out_fn);
            fclose(infile);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        if (write_webm && fseek(outfile, 0, SEEK_CUR))
        {
            fprintf(stderr, "WebM output to pipes not supported.\n");
            fclose(infile);
            fclose(outfile);

            if (RunQCheck == 1)
                QuantOutFile.close();

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
                    QuantOutFile.close();

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
                    QuantOutFile.close();

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

        if (write_webm)
        {
            ebml.stream = outfile;
            write_webm_file_header(&ebml, &cfg, &arg_framerate);
        }
        else
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
                QuantOutFile.close();

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            //std::string OutputsettingsFile = outputFile2;
            //OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            std::string OutputsettingsFile;
            int extLength = vpxt_remove_file_extension(outputFile2, OutputsettingsFile);
            OutputsettingsFile.append("parameters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - (15 + extLength), 15 + extLength);
            OutputsettingsFile.append("_parameters_vpx.txt");
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

                frame_avail = read_frame_enc(infile, &raw, file_type, &y4m,
                                             &detect);

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
                got_data = 1;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    frames_out++;

                    //fprintf(stderr, " %6luF", (unsigned long)pkt->data.frame.sz);
                    if (write_webm)
                    {
                        if (!ebml.debug)
                            hash = murmur(pkt->data.frame.buf,
                                          pkt->data.frame.sz, hash);

                        write_webm_block(&ebml, &cfg, pkt);
                    }
                    else
                    {
                        write_ivf_frame_header(outfile, pkt);

                        if (fwrite(pkt->data.frame.buf, 1,
                                   pkt->data.frame.sz, outfile));
                    }

                    nbytes += pkt->data.raw.sz;
                    //tprintf(PRINT_BTH, "Encoded Frame Size: %i\n",pkt->data.frame.sz);
                    break;
                case VPX_CODEC_STATS_PKT:
                    frames_out++;
                    /*fprintf(stderr, " %6luS",
                    (unsigned long)pkt->data.twopass_stats.sz);*/
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

        if (write_webm)
        {
            write_webm_file_footer(&ebml, hash);
        }
        else
        {
            if (!fseek(outfile, 0, SEEK_SET))
                write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);
        }

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);
        tprintf(PRINT_BTH, "\n");
    }

    vpx_img_free(&raw);
    free(ebml.cue_list);

    if (RunQCheck == 1)
        QuantOutFile.close();

    return 0;
}
int vpxt_compress_recon_buffer_check(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, const char *CompressString, int CompressInt, int RunQCheck, int OutputRaw, std::string EncFormat)
{
    std::string outputFile2Str = outputFile2;
    int                      write_webm = 1;
    vpxt_lower_case_string(EncFormat);

    if (EncFormat.compare("ivf") == 0)
        write_webm = 0;

    if (write_webm)
        outputFile2Str.append(".webm");
    else
        outputFile2Str.append(".ivf");

    //RunQCheck - Signifies if the quantizers should be check to make sure theyre working properly during an encode
    //RunQCheck = 0 = Do not save q values
    //RunQCheck = 1 = Save q values
    //RunQCheck = 2 = display q values only
    std::ofstream QuantOutFile;

    if (RunQCheck == 1)
    {
        //std::string QuantOutStr = outputFile2;
        //QuantOutStr.erase(QuantOutStr.length() - 4, 4);
        std::string QuantOutStr;
        vpxt_remove_file_extension(outputFile2, QuantOutStr);
        QuantOutStr.append("_quantizers.txt");
        char QuantOutChar[255];
        snprintf(QuantOutChar, 255, "%s", QuantOutStr.c_str());

        QuantOutFile.open(QuantOutChar);
    }

    //////////////////////////////////////////DELETE ME TEMP MEASURE//////////////////////////////////////////
    if (oxcf.Mode == 3) //Real Time Mode
    {
        if (RunQCheck == 1)
            QuantOutFile.close();

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    //new
    unsigned int             file_type, fourcc;
    y4m_input                y4m;
    int                      arg_have_framerate = 0;
    EbmlGlobal               ebml = {0};
    uint32_t                 hash = 0;

    vpx_codec_ctx_t       encoder;
    const char            *in_fn = inputFile, *out_fn = outputFile2Str.c_str(), *stats_fn = NULL;
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
            QuantOutFile.close();

        return -1;
    }

    vpxt_core_config_to_api_config(oxcf, &cfg);


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

    /*vpx_img_alloc(&raw, arg_use_i420 ? IMG_FMT_I420 : IMG_FMT_YV12,
    cfg.g_w, cfg.g_h, 1);*/

    cfg.g_timebase.den *= 2;
    memset(&stats, 0, sizeof(stats));

    //std::string out_fn4STRb = out_fn;
    //out_fn4STRb.erase(out_fn4STRb.length() - 4, 4);
    std::string out_fn4STRb;
    vpxt_remove_file_extension(out_fn, out_fn4STRb);
    out_fn4STRb.append("DecodeFrame");
    out_fn4STRb.append(slashCharStr().c_str());

    std::string CreateDir3b = out_fn4STRb;
    CreateDir3b.insert(0, "mkdir \"");
    CreateDir3b.append("\"");
    system(CreateDir3b.c_str());

    //std::string out_fn3STRb = out_fn;
    //out_fn3STRb.erase(out_fn3STRb.length() - 4, 4);
    std::string out_fn3STRb;
    vpxt_remove_file_extension(out_fn, out_fn3STRb);
    out_fn3STRb.append("PreviewFrame");
    out_fn3STRb.append(slashCharStr().c_str());

    std::string CreateDir2b = out_fn3STRb;
    CreateDir2b.insert(0, "mkdir \"");
    CreateDir2b.append("\"");
    system(CreateDir2b.c_str());


    std::ofstream ReconOutFile;
    //std::string ReconOutStr = outputFile2;
    //ReconOutStr.erase(ReconOutStr.length() - 4, 4);
    std::string ReconOutStr;
    vpxt_remove_file_extension(outputFile2, ReconOutStr);
    ReconOutStr.append("ReconFrameState.txt");
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

        //tprintf(PRINT_BTH, "API - Compressing Raw IVF File to VP8 IVF File: \n");

        int frames_in = 0, frames_out = 0;
        unsigned long nbytes = 0;
        struct detect_buffer detect;

        infile = strcmp(in_fn, "-") ? fopen(in_fn, "rb") : set_binary_mode(stdin);

        if (!infile)
        {
            tprintf(PRINT_BTH, "Failed to open input file: %s", in_fn);
            ReconOutFile.close();

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        detect.buf_read = fread(detect.buf, 1, 4, infile);
        detect.position = 0;

        unsigned int rate;
        unsigned int scale;

        if (detect.buf_read == 4 && file_is_y4m(infile, &y4m, detect.buf))
        {
            if (y4m_input_open(&y4m, infile, detect.buf, 4) >= 0)
            {
                file_type = FILE_TYPE_Y4M;
                cfg.g_w = y4m.pic_w;
                cfg.g_h = y4m.pic_h;

                /* Use the frame rate from the file only if none was specified
                * on the command-line.
                */
                if (!arg_have_framerate)
                {
                    arg_framerate.num = y4m.fps_n;
                    arg_framerate.den = y4m.fps_d;
                }

                arg_use_i420 = 0;
            }
            else
            {
                fprintf(stderr, "Unsupported Y4M stream.\n");
                return EXIT_FAILURE;
            }
        }
        else if (detect.buf_read == 4 &&
                 file_is_ivf(infile, &fourcc, &cfg.g_w, &cfg.g_h, &detect, &scale, &rate))
        {
            file_type = FILE_TYPE_IVF;

            switch (fourcc)
            {
            case 0x32315659:
                arg_use_i420 = 0;
                break;
            case 0x30323449:
                arg_use_i420 = 1;
                break;
            default:
                fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
                return EXIT_FAILURE;
            }
        }
        else
        {
            file_type = FILE_TYPE_RAW;
        }

        if (!cfg.g_w || !cfg.g_h)
        {
            fprintf(stderr, "Specify stream dimensions with --width (-w) "
                    " and --height (-h).\n");
            return EXIT_FAILURE;
        }

        std::string inputformat;
        std::string outputformat;

        if (write_webm == 1)
            outputformat = "Webm";

        if (write_webm == 0)
            outputformat = "IVF";

        if (file_type == FILE_TYPE_RAW)
            inputformat = "Raw";

        if (file_type == FILE_TYPE_Y4M)
            inputformat = "Y4M";

        if (file_type == FILE_TYPE_IVF)
            inputformat = "IVF";

        tprintf(PRINT_BTH, "API - Compressing Raw %s File to VP8 %s File: \n", inputformat.c_str(), outputformat.c_str());

        if (pass == (one_pass_only ? one_pass_only - 1 : 0))
        {
            if (file_type == FILE_TYPE_Y4M)
                /*The Y4M reader does its own allocation.
                Just initialize this here to avoid problems if we never read any
                frames.*/
                memset(&raw, 0, sizeof(raw));
            else
                vpx_img_alloc(&raw, arg_use_i420 ? VPX_IMG_FMT_I420 : VPX_IMG_FMT_YV12,
                              cfg.g_w, cfg.g_h, 1);

            // This was added so that ivfenc will create monotically increasing
            // timestamps.  Since we create new timestamps for alt-reference frames
            // we need to make room in the series of timestamps.  Since there can
            // only be 1 alt-ref frame ( current bitstream) multiplying by 2
            // gives us enough room.
        }

        outfile = strcmp(out_fn, "-") ? fopen(out_fn, "wb") : set_binary_mode(stdout);

        std::string out_fn2STR = out_fn;
        out_fn2STR.append("_Preview.raw");

        if (!OutputRaw)
            out = out_open(out_fn2STR.c_str(), 0);

        std::string out_fn3STR = out_fn;
        out_fn3STR.append("_Decode.raw");

        if (!OutputRaw)
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
                QuantOutFile.close();

            return -1;
        }

        if (write_webm && fseek(outfile, 0, SEEK_CUR))
        {
            fprintf(stderr, "WebM output to pipes not supported.\n");
            fclose(infile);
            fclose(outfile);

            if (RunQCheck == 1)
                QuantOutFile.close();

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
                    QuantOutFile.close();

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
                    QuantOutFile.close();

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

        if (write_webm)
        {
            ebml.stream = outfile;
            write_webm_file_header(&ebml, &cfg, &arg_framerate);
        }
        else
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
            ReconOutFile.close();
            out_close(out, out_fn2STR.c_str(), 0);
            out_close(out2, out_fn2STR.c_str(), 0);

            if (RunQCheck == 1)
                QuantOutFile.close();

            return -1;
        }

        frame_avail = 1;
        got_data = 0;

        if ((arg_passes == 2 && pass == 1) || arg_passes == 1)
        {
            /////////////////////////////////////OUTPUT PARAMATERS/////////////////////////////////////
            //std::string OutputsettingsFile = outputFile2;
            //OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
            std::string OutputsettingsFile;
            int extLength = vpxt_remove_file_extension(outputFile2, OutputsettingsFile);
            OutputsettingsFile.append("parameters_core.txt");
            char OutputsettingsFileChar[255];

            snprintf(OutputsettingsFileChar, 255, "%s", OutputsettingsFile.c_str());
            vpxt_output_settings(OutputsettingsFileChar,  oxcf);
            ///////////////////////////////////OUTPUT PARAMATERS API///////////////////////////////////
            OutputsettingsFile.erase(OutputsettingsFile.length() - (15 + extLength), 15 + extLength);
            OutputsettingsFile.append("_parameters_vpx.txt");
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
                QuantOutFile.close();

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

                frame_avail = read_frame_enc(infile, &raw, file_type, &y4m,
                                             &detect);

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
                got_data = 1;
                vpx_codec_iter_t  iterdec = NULL;

                int MemCheckY = 0;
                int MemCheckU = 0;
                int MemCheckV = 0;

                switch (pkt->kind)
                {
                case VPX_CODEC_CX_FRAME_PKT:
                    frames_out++;

                    //fprintf(stderr, " %6luF", (unsigned long)pkt->data.frame.sz);
                    if (write_webm)
                    {
                        if (!ebml.debug)
                            hash = murmur(pkt->data.frame.buf,
                                          pkt->data.frame.sz, hash);

                        write_webm_block(&ebml, &cfg, pkt);
                    }
                    else
                    {
                        write_ivf_frame_header(outfile, pkt);

                        if (fwrite(pkt->data.frame.buf, 1,
                                   pkt->data.frame.sz, outfile));
                    }

                    nbytes += pkt->data.raw.sz;
                    const vpx_image_t    *imgPreview;
                    const vpx_image_t    *imgDecode;

                    imgPreview = vpx_codec_get_preview_frame(&encoder);

                    vpx_codec_decode(&decoder, (const uint8_t *)pkt->data.frame.buf, pkt->data.frame.sz, NULL, 0);
                    imgDecode = vpx_codec_get_frame(&decoder, &iterdec);

                    if (imgPreview && imgDecode)
                    {
                        //std::string out_fn3STR = out_fn;
                        //out_fn3STR.erase(out_fn3STR.length() - 4, 4);
                        std::string out_fn3STR;
                        vpxt_remove_file_extension(out_fn, out_fn3STR);
                        out_fn3STR.append("PreviewFrame");
                        out_fn3STR.append(slashCharStr().c_str());

                        char intchar[56];
                        vpxt_itoa_custom(frames_out, intchar, 10);
                        out_fn3STR.append(intchar);
                        out_fn3STR.append(".raw");

                        if (!OutputRaw)
                            out3 = out_open(out_fn3STR.c_str(), 0);

                        //std::string out_fn4STR = out_fn;
                        //out_fn4STR.erase(out_fn4STR.length() - 4, 4);
                        std::string out_fn4STR;
                        vpxt_remove_file_extension(out_fn, out_fn4STR);
                        out_fn4STR.append("DecodeFrame");
                        out_fn4STR.append(slashCharStr().c_str());

                        char intchar2[56];
                        vpxt_itoa_custom(frames_out, intchar2, 10);
                        out_fn4STR.append(intchar2);
                        out_fn4STR.append(".raw");

                        if (!OutputRaw)
                            out4 = out_open(out_fn4STR.c_str(), 0);

                        unsigned int y;
                        char out_fn[128+24];
                        uint8_t *bufPreview;
                        uint8_t *bufDecode;

                        bufPreview = imgPreview->planes[PLANE_Y];
                        bufDecode = imgDecode->planes[PLANE_Y];

                        for (y = 0; y < imgDecode->d_h; y++)
                        {
                            if (!OutputRaw)
                                out_put(out3, bufPreview, imgDecode->d_w, 0);

                            bufPreview += imgPreview->stride[PLANE_Y];

                            if (!OutputRaw)
                                out_put(out4, bufDecode, imgDecode->d_w, 0);

                            bufDecode += imgDecode->stride[PLANE_Y];

                            MemCheckY |= memcmp(bufPreview, bufDecode, imgDecode->stride[PLANE_Y]);
                        }

                        bufPreview = imgPreview->planes[PLANE_U];
                        bufDecode = imgDecode->planes[PLANE_U];

                        for (y = 0; y < imgDecode->d_h / 2; y++)
                        {
                            if (!OutputRaw)
                                out_put(out3, bufPreview, imgDecode->d_w / 2, 0);

                            bufPreview += imgPreview->stride[PLANE_U];

                            if (!OutputRaw)
                                out_put(out4, bufDecode, imgDecode->d_w / 2, 0);

                            bufDecode += imgDecode->stride[PLANE_U];

                            MemCheckU |= memcmp(bufPreview, bufDecode, imgDecode->stride[PLANE_U]);
                        }

                        bufPreview = imgPreview->planes[PLANE_V];
                        bufDecode = imgDecode->planes[PLANE_V];

                        for (y = 0; y < imgDecode->d_h / 2; y++)
                        {
                            if (!OutputRaw)
                                out_put(out3, bufPreview, imgDecode->d_w / 2, 0);

                            bufPreview += imgPreview->stride[PLANE_V];

                            if (!OutputRaw)
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

                        if (!OutputRaw)
                        {
                            out_close(out3, out_fn3STR.c_str(), 0);
                            out_close(out4, out_fn4STR.c_str(), 0);
                        }
                    }

                    if (imgPreview && !OutputRaw)
                    {
                        unsigned int y;
                        char out_fn[128+24];
                        uint8_t *buf;

                        buf = imgPreview->planes[PLANE_Y];

                        for (y = 0; y < imgDecode->d_h; y++)
                        {
                            out_put(out, buf, imgDecode->d_w, 0);
                            buf += imgPreview->stride[PLANE_Y];
                        }

                        buf = imgPreview->planes[PLANE_U];

                        for (y = 0; y < imgDecode->d_h / 2; y++)
                        {
                            out_put(out, buf, imgDecode->d_w / 2, 0);
                            buf += imgPreview->stride[PLANE_U];
                        }

                        buf = imgPreview->planes[PLANE_V];

                        for (y = 0; y < imgDecode->d_h / 2; y++)
                        {
                            out_put(out, buf, imgDecode->d_w / 2, 0);
                            buf += imgPreview->stride[PLANE_V];
                        }
                    }

                    if (imgDecode && !OutputRaw)
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
                    frames_out++;
                    /*fprintf(stderr, " %6luS",
                    (unsigned long)pkt->data.twopass_stats.sz);*/
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
        vpx_codec_destroy(&decoder);

        fclose(infile);

        if (write_webm)
        {
            write_webm_file_footer(&ebml, hash);
        }
        else
        {
            if (!fseek(outfile, 0, SEEK_SET))
                write_ivf_file_header(outfile, &cfg, codec->fourcc, frames_out);
        }

        fclose(outfile);
        stats_close(&stats, arg_passes - 1);

        if (!OutputRaw)
        {
            out_close(out, out_fn2STR.c_str(), 0);
            out_close(out2, out_fn2STR.c_str(), 0);
        }

        tprintf(PRINT_BTH, "\n");
    }

    vpx_img_free(&raw);
    free(ebml.cue_list);

    ReconOutFile.close();

    if (RunQCheck == 1)
        QuantOutFile.close();

    return 0;
}
int vpxt_decompress(const char *inputchar, const char *outputchar, std::string DecFormat)
{
    int                     use_y4m = 1;
    vpxt_lower_case_string(DecFormat);

    if (DecFormat.compare("ivf") == 0)
        use_y4m = 2;

    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    void                   *out = NULL;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    std::string compformat;
    std::string decformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (use_y4m == 0)
        decformat = "Raw";

    if (use_y4m == 1)
        decformat = "Y4M";

    if (use_y4m == 2)
        decformat = "IVF";

    tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 %s File to Raw %s File: \n", compformat.c_str(), decformat.c_str());

    outfile_pattern = outfile_pattern ? outfile_pattern : "-";
    single_file = 1;
    {
        const char *p = outfile_pattern;

        do
        {
            p = strchr(p, '%');

            if (p && p[1] >= '1' && p[1] <= '9')
            {
                single_file = 0;
                break;
            }

            if (p)
                p++;
        }
        while (p);
    }

    if (single_file && !noblit)
    {
        generate_filename(outfile_pattern, outfile, sizeof(outfile) - 1,
                          width, height, 0);
        strncpy(outfile, outputchar, PATH_MAX);
        //outfile = outputchar;
        out = out_open(outfile, do_md5);
    }

    if (use_y4m && !noblit)
    {
        char buffer[128];

        if (!single_file)
        {
            fprintf(stderr, "YUV4MPEG2 not supported with output patterns,"
                    " try --i420 or --yv12.\n");
            return EXIT_FAILURE;
        }

        if (input.kind == WEBM_FILE)
            if (webm_guess_framerate(&input, &fps_den, &fps_num))
            {
                fprintf(stderr, "Failed to guess framerate -- error parsing "
                        "webm file?\n");
                return EXIT_FAILURE;
            }

        /*Note: We can't output an aspect ratio here because IVF doesn't
        store one, and neither does VP8.
        That will have to wait until these tools support WebM natively.*/
        sprintf(buffer, "YUV4MPEG2 C%s W%u H%u F%u:%u I%c\n",
                "420jpeg", width, height, fps_num, fps_den, 'p');
        out_put(out, (unsigned char *)buffer, strlen(buffer), do_md5);
    }

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    /* Decode file */
    while (!read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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

        ++frame_in;

        if ((img = vpx_codec_get_frame(&decoder, &iter)))
            ++frame_out;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img)
            {
                unsigned int y;
                char out_fn[PATH_MAX];
                uint8_t *buf;

                if (!single_file)
                {
                    size_t len = sizeof(out_fn) - 1;
                    out_fn[len] = '\0';
                    generate_filename(outfile_pattern, out_fn, len - 1,
                                      img->d_w, img->d_h, frame_in);
                    out = out_open(out_fn, do_md5);
                }
                else if (use_y4m)
                    out_put(out, (unsigned char *)"FRAME\n", 6, do_md5);

                if (!use_y4m)
                    ivf_write_frame_header((FILE *)out, 0, img->d_h * img->d_w);

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

                buf = img->planes[VPX_PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[VPX_PLANE_Y];
                }

                buf = img->planes[flipuv?VPX_PLANE_V:VPX_PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_U];
                }

                buf = img->planes[flipuv?VPX_PLANE_U:VPX_PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_V];
                }

                if (!single_file)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary || progress)
    {
        show_progress(frame_in, frame_out, dx_time);
        fprintf(stderr, "\n");
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (single_file && !noblit)
        out_close(out, outfile, do_md5);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    return 0;
}
int vpxt_decompress_to_raw(const char *inputchar, const char *outputchar)
{
    int                     use_y4m = 1;

    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    void                   *out = NULL;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    std::string compformat;
    std::string decformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (use_y4m == 0)
        decformat = "Raw";

    if (use_y4m == 1)
        decformat = "Y4M";

    if (use_y4m == 2)
        decformat = "IVF";

    tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 %s File to Raw File: \n", compformat.c_str());

    outfile_pattern = outfile_pattern ? outfile_pattern : "-";
    single_file = 1;
    {
        const char *p = outfile_pattern;

        do
        {
            p = strchr(p, '%');

            if (p && p[1] >= '1' && p[1] <= '9')
            {
                single_file = 0;
                break;
            }

            if (p)
                p++;
        }
        while (p);
    }

    if (single_file && !noblit)
    {
        generate_filename(outfile_pattern, outfile, sizeof(outfile) - 1,
                          width, height, 0);
        strncpy(outfile, outputchar, PATH_MAX);
        //outfile = outputchar;
        out = out_open(outfile, do_md5);
    }

    if (use_y4m && !noblit)
    {
        char buffer[128];

        if (!single_file)
        {
            fprintf(stderr, "YUV4MPEG2 not supported with output patterns,"
                    " try --i420 or --yv12.\n");
            return EXIT_FAILURE;
        }

        if (input.kind == WEBM_FILE)
            if (webm_guess_framerate(&input, &fps_den, &fps_num))
            {
                fprintf(stderr, "Failed to guess framerate -- error parsing "
                        "webm file?\n");
                return EXIT_FAILURE;
            }

        /*Note: We can't output an aspect ratio here because IVF doesn't
        store one, and neither does VP8.
        That will have to wait until these tools support WebM natively.*/
        sprintf(buffer, "YUV4MPEG2 C%s W%u H%u F%u:%u I%c\n",
                "420jpeg", width, height, fps_num, fps_den, 'p');
        out_put(out, (unsigned char *)buffer, strlen(buffer), do_md5);
    }

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    /* Decode file */
    while (!read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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

        ++frame_in;

        if ((img = vpx_codec_get_frame(&decoder, &iter)))
            ++frame_out;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img)
            {
                unsigned int y;
                char out_fn[PATH_MAX];
                uint8_t *buf;

                if (!single_file)
                {
                    size_t len = sizeof(out_fn) - 1;
                    out_fn[len] = '\0';
                    generate_filename(outfile_pattern, out_fn, len - 1,
                                      img->d_w, img->d_h, frame_in);
                    out = out_open(out_fn, do_md5);
                }
                else if (use_y4m)
                    out_put(out, (unsigned char *)"FRAME\n", 6, do_md5);

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

                buf = img->planes[VPX_PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[VPX_PLANE_Y];
                }

                buf = img->planes[flipuv?VPX_PLANE_V:VPX_PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_U];
                }

                buf = img->planes[flipuv?VPX_PLANE_U:VPX_PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_V];
                }

                if (!single_file)
                    out_close(out, out_fn, do_md5);
            }

            //vpx_img_free(img);
        }

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary || progress)
    {
        show_progress(frame_in, frame_out, dx_time);
        fprintf(stderr, "\n");
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (single_file && !noblit)
        out_close(out, outfile, do_md5);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);



    return 0;
}
int vpxt_decompress_to_raw_no_error_output(const char *inputchar, const char *outputchar)
{
    int                     use_y4m = 1;

    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    void                   *out = NULL;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_STD, "Failed to open input file: %s", fn);
        return -1;
    }


    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    //tprintf(PRINT_STD, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    std::string compformat;
    std::string decformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (use_y4m == 0)
        decformat = "Raw";

    if (use_y4m == 1)
        decformat = "Y4M";

    if (use_y4m == 2)
        decformat = "IVF";

    tprintf(PRINT_STD, "\nAPI - Decompressing VP8 %s File to Raw File: \n", compformat.c_str());

    outfile_pattern = outfile_pattern ? outfile_pattern : "-";
    single_file = 1;
    {
        const char *p = outfile_pattern;

        do
        {
            p = strchr(p, '%');

            if (p && p[1] >= '1' && p[1] <= '9')
            {
                single_file = 0;
                break;
            }

            if (p)
                p++;
        }
        while (p);
    }

    if (single_file && !noblit)
    {
        generate_filename(outfile_pattern, outfile, sizeof(outfile) - 1,
                          width, height, 0);
        strncpy(outfile, outputchar, PATH_MAX);
        //outfile = outputchar;
        out = out_open(outfile, do_md5);
    }

    if (use_y4m && !noblit)
    {
        char buffer[128];

        if (!single_file)
        {
            fprintf(stderr, "YUV4MPEG2 not supported with output patterns,"
                    " try --i420 or --yv12.\n");
            return EXIT_FAILURE;
        }

        if (input.kind == WEBM_FILE)
            if (webm_guess_framerate(&input, &fps_den, &fps_num))
            {
                fprintf(stderr, "Failed to guess framerate -- error parsing "
                        "webm file?\n");
                return EXIT_FAILURE;
            }

        /*Note: We can't output an aspect ratio here because IVF doesn't
        store one, and neither does VP8.
        That will have to wait until these tools support WebM natively.*/
        sprintf(buffer, "YUV4MPEG2 C%s W%u H%u F%u:%u I%c\n",
                "420jpeg", width, height, fps_num, fps_den, 'p');
        out_put(out, (unsigned char *)buffer, strlen(buffer), do_md5);
    }

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    /* Decode file */
    while (!read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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

        ++frame_in;

        if ((img = vpx_codec_get_frame(&decoder, &iter)))
            ++frame_out;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img)
            {
                unsigned int y;
                char out_fn[PATH_MAX];
                uint8_t *buf;

                if (!single_file)
                {
                    size_t len = sizeof(out_fn) - 1;
                    out_fn[len] = '\0';
                    generate_filename(outfile_pattern, out_fn, len - 1,
                                      img->d_w, img->d_h, frame_in);
                    out = out_open(out_fn, do_md5);
                }
                else if (use_y4m)
                    out_put(out, (unsigned char *)"FRAME\n", 6, do_md5);

                if (!use_y4m)
                    ivf_write_frame_header((FILE *)out, 0, img->d_h * img->d_w);

                if (CharCount == 79)
                {
                    tprintf(PRINT_STD, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_STD, ".");

                buf = img->planes[VPX_PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[VPX_PLANE_Y];
                }

                buf = img->planes[flipuv?VPX_PLANE_V:VPX_PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_U];
                }

                buf = img->planes[flipuv?VPX_PLANE_U:VPX_PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_V];
                }

                if (!single_file)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary || progress)
    {
        show_progress(frame_in, frame_out, dx_time);
        fprintf(stderr, "\n");
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (single_file && !noblit)
        out_close(out, outfile, do_md5);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    return 0;
}
int vpxt_decompress_no_output(const char *inputchar, const char *outputchar, std::string DecFormat)
{
    int                     use_y4m = 1;
    vpxt_lower_case_string(DecFormat);

    if (DecFormat.compare("ivf") == 0)
        use_y4m = 2;

    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    void                   *out = NULL;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_STD, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_STD, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    std::string compformat;
    std::string decformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (use_y4m == 0)
        decformat = "Raw";

    if (use_y4m == 1)
        decformat = "Y4M";

    if (use_y4m == 2)
        decformat = "IVF";

    tprintf(PRINT_STD, "\nAPI - Decompressing VP8 %s File to Raw %s File: \n", compformat.c_str(), decformat.c_str());

    outfile_pattern = outfile_pattern ? outfile_pattern : "-";
    single_file = 1;
    {
        const char *p = outfile_pattern;

        do
        {
            p = strchr(p, '%');

            if (p && p[1] >= '1' && p[1] <= '9')
            {
                single_file = 0;
                break;
            }

            if (p)
                p++;
        }
        while (p);
    }

    if (single_file && !noblit)
    {
        generate_filename(outfile_pattern, outfile, sizeof(outfile) - 1,
                          width, height, 0);
        strncpy(outfile, outputchar, PATH_MAX);
        //outfile = outputchar;
        out = out_open(outfile, do_md5);
    }

    if (use_y4m && !noblit)
    {
        char buffer[128];

        if (!single_file)
        {
            fprintf(stderr, "YUV4MPEG2 not supported with output patterns,"
                    " try --i420 or --yv12.\n");
            return EXIT_FAILURE;
        }

        if (input.kind == WEBM_FILE)
            if (webm_guess_framerate(&input, &fps_den, &fps_num))
            {
                fprintf(stderr, "Failed to guess framerate -- error parsing "
                        "webm file?\n");
                return EXIT_FAILURE;
            }

        /*Note: We can't output an aspect ratio here because IVF doesn't
        store one, and neither does VP8.
        That will have to wait until these tools support WebM natively.*/
        sprintf(buffer, "YUV4MPEG2 C%s W%u H%u F%u:%u I%c\n",
                "420jpeg", width, height, fps_num, fps_den, 'p');
        out_put(out, (unsigned char *)buffer, strlen(buffer), do_md5);
    }

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    /* Decode file */
    while (!read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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

        ++frame_in;

        if ((img = vpx_codec_get_frame(&decoder, &iter)))
            ++frame_out;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img)
            {
                unsigned int y;
                char out_fn[PATH_MAX];
                uint8_t *buf;

                if (!single_file)
                {
                    size_t len = sizeof(out_fn) - 1;
                    out_fn[len] = '\0';
                    generate_filename(outfile_pattern, out_fn, len - 1,
                                      img->d_w, img->d_h, frame_in);
                    out = out_open(out_fn, do_md5);
                }
                else if (use_y4m)
                    out_put(out, (unsigned char *)"FRAME\n", 6, do_md5);

                if (!use_y4m)
                    ivf_write_frame_header((FILE *)out, 0, img->d_h * img->d_w);

                if (CharCount == 79)
                {
                    tprintf(PRINT_STD, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_STD, ".");

                buf = img->planes[VPX_PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[VPX_PLANE_Y];
                }

                buf = img->planes[flipuv?VPX_PLANE_V:VPX_PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_U];
                }

                buf = img->planes[flipuv?VPX_PLANE_U:VPX_PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_V];
                }

                if (!single_file)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary || progress)
    {
        show_progress(frame_in, frame_out, dx_time);
        fprintf(stderr, "\n");
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (single_file && !noblit)
        out_close(out, outfile, do_md5);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    return 0;
}
unsigned int vpxt_time_decompress(const char *inputchar, const char *outputchar, unsigned int &CPUTick, std::string DecFormat)
{
    int                     use_y4m = 1;
    vpxt_lower_case_string(DecFormat);

    if (DecFormat.compare("ivf") == 0)
        use_y4m = 2;

    //Time Decompress is not supposed to save output that is the only difference between it and
    //vpxt_decompress_ivf_to_ivf_time_and_output
    vpx_codec_ctx_t       decoder;
    unsigned int           total_cpu_time_used = 0;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 1, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    void                   *out = NULL;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    std::string compformat;
    std::string decformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (use_y4m == 0)
        decformat = "Raw";

    if (use_y4m == 1)
        decformat = "Y4M";

    if (use_y4m == 2)
        decformat = "IVF";

    tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 %s \n", compformat.c_str());

    outfile_pattern = outfile_pattern ? outfile_pattern : "-";
    single_file = 1;
    {
        const char *p = outfile_pattern;

        do
        {
            p = strchr(p, '%');

            if (p && p[1] >= '1' && p[1] <= '9')
            {
                single_file = 0;
                break;
            }

            if (p)
                p++;
        }
        while (p);
    }

    if (single_file && !noblit)
    {
        generate_filename(outfile_pattern, outfile, sizeof(outfile) - 1,
                          width, height, 0);
        strncpy(outfile, outputchar, PATH_MAX);
        //outfile = outputchar;
        out = out_open(outfile, do_md5);
    }

    if (use_y4m && !noblit)
    {
        char buffer[128];

        if (!single_file)
        {
            fprintf(stderr, "YUV4MPEG2 not supported with output patterns,"
                    " try --i420 or --yv12.\n");
            return EXIT_FAILURE;
        }

        if (input.kind == WEBM_FILE)
            if (webm_guess_framerate(&input, &fps_den, &fps_num))
            {
                fprintf(stderr, "Failed to guess framerate -- error parsing "
                        "webm file?\n");
                return EXIT_FAILURE;
            }

        /*Note: We can't output an aspect ratio here because IVF doesn't
        store one, and neither does VP8.
        That will have to wait until these tools support WebM natively.*/
        sprintf(buffer, "YUV4MPEG2 C%s W%u H%u F%u:%u I%c\n",
                "420jpeg", width, height, fps_num, fps_den, 'p');
        out_put(out, (unsigned char *)buffer, strlen(buffer), do_md5);
    }

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    /* Decode file */
    while (!read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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
        ++frame_in;

        if (CharCount == 79)
        {
            tprintf(PRINT_BTH, "\n");
            CharCount = 0;
        }

        CharCount++;
        tprintf(PRINT_BTH, ".");

        if ((img = vpx_codec_get_frame(&decoder, &iter)))
            ++frame_out;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img)
            {
                unsigned int y;
                char out_fn[PATH_MAX];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!single_file)
                {
                    size_t len = sizeof(out_fn) - 1;
                    out_fn[len] = '\0';
                    generate_filename(outfile_pattern, out_fn, len - 1,
                                      img->d_w, img->d_h, frame_in);
                    out = out_open(out_fn, do_md5);
                }
                else if (use_y4m)
                    out_put(out, (unsigned char *)"FRAME\n", 6, do_md5);

                if (!use_y4m)
                    ivf_write_frame_header((FILE *)out, 0, img->d_h * img->d_w);

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

                buf = img->planes[VPX_PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[VPX_PLANE_Y];
                }

                buf = img->planes[flipuv?VPX_PLANE_V:VPX_PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_U];
                }

                buf = img->planes[flipuv?VPX_PLANE_U:VPX_PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_V];
                }

                if (!single_file)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary)
    {
        tprintf(PRINT_BTH, "\n\n Decoded %d frames in %lu us (%.2f fps)\n", frame_in, dx_time, (float)frame_in * 1000000.0 / (float)dx_time);
        tprintf(PRINT_BTH, " Total CPU Ticks: %i\n", total_cpu_time_used);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (single_file && !noblit)
        out_close(out, outfile, do_md5);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    //char TextFilechar1[255];
    //char TextFilechar2[255];

    std::string FullNameMs;
    std::string FullNameCpu;

    vpxt_remove_file_extension(outputchar, FullNameMs);
    vpxt_remove_file_extension(outputchar, FullNameCpu);

    //char *FullNameMs = strcat(TextFilechar1, "decompression_time.txt");
    FullNameMs.append("decompression_time.txt");

    std::ofstream FullNameMsFile(FullNameMs.c_str());
    FullNameMsFile << dx_time;
    FullNameMsFile.close();

    //char *FullNameCpu = strcat(TextFilechar2, "decompression_cpu_tick.txt");
    FullNameCpu.append("decompression_cpu_tick.txt");

    std::ofstream FullNameCpuFile(FullNameCpu.c_str());
    FullNameCpuFile << total_cpu_time_used;
    FullNameCpuFile.close();

    CPUTick = total_cpu_time_used;
    return dx_time;
}
unsigned int vpxt_decompress_time_and_output(const char *inputchar, const char *outputchar, unsigned int &CPUTick, std::string DecFormat)
{
    int                     use_y4m = 1;
    vpxt_lower_case_string(DecFormat);

    if (DecFormat.compare("ivf") == 0)
        use_y4m = 2;

    vpx_codec_ctx_t       decoder;
    unsigned int           total_cpu_time_used = 0;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    void                   *out = NULL;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    std::string compformat;
    std::string decformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (use_y4m == 0)
        decformat = "Raw";

    if (use_y4m == 1)
        decformat = "Y4M";

    if (use_y4m == 2)
        decformat = "IVF";

    tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 %s File to Raw %s File: \n", compformat.c_str(), decformat.c_str());

    outfile_pattern = outfile_pattern ? outfile_pattern : "-";
    single_file = 1;
    {
        const char *p = outfile_pattern;

        do
        {
            p = strchr(p, '%');

            if (p && p[1] >= '1' && p[1] <= '9')
            {
                single_file = 0;
                break;
            }

            if (p)
                p++;
        }
        while (p);
    }

    if (single_file && !noblit)
    {
        generate_filename(outfile_pattern, outfile, sizeof(outfile) - 1,
                          width, height, 0);
        strncpy(outfile, outputchar, PATH_MAX);
        //outfile = outputchar;
        out = out_open(outfile, do_md5);
    }

    if (use_y4m && !noblit)
    {
        char buffer[128];

        if (!single_file)
        {
            fprintf(stderr, "YUV4MPEG2 not supported with output patterns,"
                    " try --i420 or --yv12.\n");
            return EXIT_FAILURE;
        }

        if (input.kind == WEBM_FILE)
            if (webm_guess_framerate(&input, &fps_den, &fps_num))
            {
                fprintf(stderr, "Failed to guess framerate -- error parsing "
                        "webm file?\n");
                return EXIT_FAILURE;
            }

        /*Note: We can't output an aspect ratio here because IVF doesn't
        store one, and neither does VP8.
        That will have to wait until these tools support WebM natively.*/
        sprintf(buffer, "YUV4MPEG2 C%s W%u H%u F%u:%u I%c\n",
                "420jpeg", width, height, fps_num, fps_den, 'p');
        out_put(out, (unsigned char *)buffer, strlen(buffer), do_md5);
    }

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    /* Decode file */
    while (!read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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
        ++frame_in;

        if ((img = vpx_codec_get_frame(&decoder, &iter)))
            ++frame_out;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img)
            {
                unsigned int y;
                char out_fn[PATH_MAX];
                uint8_t *buf;
                const char *sfx = flipuv ? "yv12" : "i420";

                if (!single_file)
                {
                    size_t len = sizeof(out_fn) - 1;
                    out_fn[len] = '\0';
                    generate_filename(outfile_pattern, out_fn, len - 1,
                                      img->d_w, img->d_h, frame_in);
                    out = out_open(out_fn, do_md5);
                }
                else if (use_y4m)
                    out_put(out, (unsigned char *)"FRAME\n", 6, do_md5);

                if (!use_y4m)
                    ivf_write_frame_header((FILE *)out, 0, img->d_h * img->d_w);

                if (CharCount == 79)
                {
                    tprintf(PRINT_BTH, "\n");
                    CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");

                buf = img->planes[VPX_PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[VPX_PLANE_Y];
                }

                buf = img->planes[flipuv?VPX_PLANE_V:VPX_PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_U];
                }

                buf = img->planes[flipuv?VPX_PLANE_U:VPX_PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_V];
                }

                if (!single_file)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary)
    {
        tprintf(PRINT_BTH, "\n\n Decoded %d frames in %lu us (%.2f fps)\n", frame_in, dx_time, (float)frame_in * 1000000.0 / (float)dx_time);
        tprintf(PRINT_BTH, " Total CPU Ticks: %i\n", total_cpu_time_used);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (single_file && !noblit)
        out_close(out, outfile, do_md5);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    //char TextFilechar1[255];
    //char TextFilechar2[255];
    std::string FullNameMs;
    std::string FullNameCpu;

    vpxt_remove_file_extension(outputchar, FullNameMs);
    vpxt_remove_file_extension(outputchar, FullNameCpu);

    //char *FullNameMs = strcat(TextFilechar1, "decompression_time.txt");
    FullNameMs.append("decompression_time.txt");

    std::ofstream FullNameMsFile(FullNameMs.c_str());
    FullNameMsFile << dx_time;
    FullNameMsFile.close();

    //char *FullNameCpu = strcat(TextFilechar2, "decompression_cpu_tick.txt");
    FullNameCpu.append("decompression_cpu_tick.txt");

    std::ofstream FullNameCpuFile(FullNameCpu.c_str());
    FullNameCpuFile << total_cpu_time_used;
    FullNameCpuFile.close();

    CPUTick = total_cpu_time_used;
    return dx_time;
}
int vpxt_dec_compute_md5(const char *inputchar, const char *outputchar)
{
    int                     use_y4m = 0;

    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 0, do_md5 = 1, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    void                   *out = NULL;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    outfile_pattern = outfile_pattern ? outfile_pattern : "-";
    single_file = 1;
    {
        const char *p = outfile_pattern;

        do
        {
            p = strchr(p, '%');

            if (p && p[1] >= '1' && p[1] <= '9')
            {
                single_file = 0;
                break;
            }

            if (p)
                p++;
        }
        while (p);
    }

    if (single_file && !noblit)
    {
        generate_filename(outfile_pattern, outfile, sizeof(outfile) - 1, width, height, 0);
        strncpy(outfile, outputchar, PATH_MAX);
        //outfile = outputchar;
        out = out_open(outfile, do_md5);
    }

    if (use_y4m && !noblit)
    {
        char buffer[128];

        if (!single_file)
        {
            fprintf(stderr, "YUV4MPEG2 not supported with output patterns,"
                    " try --i420 or --yv12.\n");
            return EXIT_FAILURE;
        }

        if (input.kind == WEBM_FILE)
            if (webm_guess_framerate(&input, &fps_den, &fps_num))
            {
                fprintf(stderr, "Failed to guess framerate -- error parsing "
                        "webm file?\n");
                return EXIT_FAILURE;
            }

        /*Note: We can't output an aspect ratio here because IVF doesn't
        store one, and neither does VP8.
        That will have to wait until these tools support WebM natively.*/
        sprintf(buffer, "YUV4MPEG2 C%s W%u H%u F%u:%u I%c\n",
                "420jpeg", width, height, fps_num, fps_den, 'p');
        out_put(out, (unsigned char *)buffer, strlen(buffer), do_md5);
    }

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    /* Decode file */
    while (!read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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

        ++frame_in;

        if ((img = vpx_codec_get_frame(&decoder, &iter)))
            ++frame_out;

        if (progress)
        {
        }

        if (!noblit)
        {
            if (img)
            {
                unsigned int y;
                char out_fn[PATH_MAX];
                uint8_t *buf;

                if (!single_file)
                {
                    size_t len = sizeof(out_fn) - 1;
                    out_fn[len] = '\0';
                    generate_filename(outfile_pattern, out_fn, len - 1,
                                      img->d_w, img->d_h, frame_in);
                    out = out_open(out_fn, do_md5);
                }
                else if (use_y4m)
                    out_put(out, (unsigned char *)"FRAME\n", 6, do_md5);

                /*if (CharCount == 79)
                {
                tprintf(PRINT_BTH, "\n");
                CharCount = 0;
                }

                CharCount++;
                tprintf(PRINT_BTH, ".");*/

                buf = img->planes[VPX_PLANE_Y];

                for (y = 0; y < img->d_h; y++)
                {
                    out_put(out, buf, img->d_w, do_md5);
                    buf += img->stride[VPX_PLANE_Y];
                }

                buf = img->planes[flipuv?VPX_PLANE_V:VPX_PLANE_U];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_U];
                }

                buf = img->planes[flipuv?VPX_PLANE_U:VPX_PLANE_V];

                for (y = 0; y < (1 + img->d_h) / 2; y++)
                {
                    out_put(out, buf, (1 + img->d_w) / 2, do_md5);
                    buf += img->stride[VPX_PLANE_V];
                }

                if (!single_file)
                    out_close(out, out_fn, do_md5);
            }
        }

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary || progress)
    {
        tprintf(PRINT_STD, "Decoded %d frames in %lu us (%.2f fps)\n",
                frame_in, dx_time, (float)frame_in * 1000000.0 / (float)dx_time);
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (single_file && !noblit)
        out_close(out, outfile, do_md5);

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    return 0;
}
#endif
//--------------------------------------------------------Tools-------------------------------------------------------------------------
int vpxt_cut_clip(const char *inputFile, const char *outputFile, int StartingFrame, int EndingFrame)
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

    y4m_input                y4m;
    unsigned int file_type, fourcc;
    unsigned long nbytes = 0;
    struct detect_buffer detect;

    unsigned int width;
    unsigned int height;
    unsigned int rate;
    unsigned int scale;
    //int arg_use_i420 = 1;

    detect.buf_read = fread(detect.buf, 1, 4, in);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(in, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, in, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            width = y4m.pic_w;
            height = y4m.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate = y4m.fps_n;
                scale = y4m.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 && file_is_ivf(in, &fourcc, &width, &height, &detect, &scale, &rate))
    {
        file_type = FILE_TYPE_IVF;

        switch (fourcc)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type = FILE_TYPE_RAW;
    }

    if (!width || !height)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    std::string inputformat;

    if (file_type == FILE_TYPE_RAW)
        inputformat = "Raw";

    if (file_type == FILE_TYPE_Y4M)
        inputformat = "Y4M";

    if (file_type == FILE_TYPE_IVF)
        inputformat = "IVF";

    //printf("\nwidth: %i height: %i rate: %i scale: %i \n",width, height, rate, scale);
    tprintf(PRINT_STD, "\nCut %s file to %s file: \n", inputformat.c_str(), inputformat.c_str());

    if (file_type == FILE_TYPE_Y4M)
    {
        //copy and output y4m header
        fpos_t position;
        fgetpos(in, &position);
        rewind(in);

        char buffer[128];
        fread(buffer, 1, 128, in);

        std::string bufferStr = buffer;
        size_t HeaderEnd;
        std::string bufferStr2 = bufferStr.substr(0, bufferStr.find("FRAME"));
        char buffer2[128];
        strncpy(buffer2, bufferStr2.c_str(), 128);
        out_put(out, (unsigned char *)buffer2, strlen(buffer2), 0);
        fsetpos(in, &position);
    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(in);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    currentVideoFrame = 1;

    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1);
    int frame_avail = 1;
    int framesWritten = 0;

    while (frame_avail)
    {

        if (currentVideoFrame >= StartingFrame && currentVideoFrame <= EndingFrame)
        {
            if (CharCount == 79)
            {
                tprintf(PRINT_STD, "\n");
                CharCount = 0;
            }

            tprintf(PRINT_STD, "*");
            CharCount++;

            frame_avail = read_frame_enc(in, &raw, file_type, &y4m, &detect);

            if (!frame_avail)
                break;

            if (file_type == FILE_TYPE_Y4M)
            {
                out_put(out, (unsigned char *)"FRAME\n", 6, 0);
            }

            if (file_type == FILE_TYPE_IVF)
            {
                ivf_write_frame_header(out, 0, width * height * 3 / 2);
            }

            unsigned int y;
            uint8_t *buf;

            buf = raw.planes[VPX_PLANE_Y];

            for (y = 0; y < raw.d_h; y++)
            {
                out_put(out, buf, raw.d_w, 0);
                buf += raw.stride[VPX_PLANE_Y];
            }

            buf = raw.planes[VPX_PLANE_U];

            for (y = 0; y < (1 + raw.d_h) / 2; y++)
            {
                out_put(out, buf, (1 + raw.d_w) / 2, 0);
                buf += raw.stride[VPX_PLANE_U];
            }

            buf = raw.planes[VPX_PLANE_V];

            for (y = 0; y < (1 + raw.d_h) / 2; y++)
            {
                out_put(out, buf, (1 + raw.d_w) / 2, 0);
                buf += raw.stride[VPX_PLANE_V];
            }

            framesWritten++;
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

            frame_avail = read_frame_enc(in, &raw, file_type, &y4m, &detect);
        }

        currentVideoFrame++;
    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(in);
        rewind(out);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        ivfhRaw.length = framesWritten;
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    fclose(in);
    fclose(out);
    vpx_img_free(&raw);

    return(0);
}
int vpxt_crop_raw_clip(const char *inputFile, const char *outputFile, int xoffset, int yoffset, int newFrameWidth, int newFrameHeight, int FileIsIVF, int OutputToFile)
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

    y4m_input                y4m;
    unsigned int file_type, fourcc;
    unsigned long nbytes = 0;
    struct detect_buffer detect;

    unsigned int width;
    unsigned int height;
    unsigned int rate;
    unsigned int scale;
    //int arg_use_i420 = 1;

    detect.buf_read = fread(detect.buf, 1, 4, in);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(in, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, in, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            width = y4m.pic_w;
            height = y4m.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate = y4m.fps_n;
                scale = y4m.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 && file_is_ivf(in, &fourcc, &width, &height, &detect, &scale, &rate))
    {
        file_type = FILE_TYPE_IVF;

        switch (fourcc)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type = FILE_TYPE_RAW;
    }

    if (!width || !height)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    std::string inputformat;

    if (file_type == FILE_TYPE_RAW)
        inputformat = "Raw";

    if (file_type == FILE_TYPE_Y4M)
        inputformat = "Y4M";

    if (file_type == FILE_TYPE_IVF)
        inputformat = "IVF";

    //printf("\nwidth: %i height: %i rate: %i scale: %i \n",width, height, rate, scale);
    tprintf(PRINT_STD, "\nCrop %s file to %s file: \n", inputformat.c_str(), inputformat.c_str());

    if (file_type == FILE_TYPE_Y4M)
    {
        //copy and output y4m header
        fpos_t position;
        fgetpos(in, &position);
        rewind(in);

        char buffer[128];
        fread(buffer, 1, 128, in);

        std::string bufferStr = buffer;
        size_t WidthLoc = 0;
        size_t WidthLocEnd = 0;
        size_t HeightLoc = 0;
        size_t HeightLocEnd = 0;
        size_t FrameLoc = 0;

        char widthChar[32];
        char heightChar[32];
        vpxt_itoa_custom(newFrameWidth, widthChar, 10);
        vpxt_itoa_custom(newFrameHeight, heightChar, 10);

        WidthLoc = bufferStr.find("W");
        WidthLocEnd = bufferStr.find(" ", WidthLoc + 1);
        HeightLoc = bufferStr.find("H");
        HeightLocEnd = bufferStr.find(" ", HeightLoc + 1);
        FrameLoc = bufferStr.find("FRAME");

        std::string bufferStr2 = bufferStr.substr(0, WidthLoc);
        bufferStr2.append("W");
        bufferStr2.append(widthChar);
        bufferStr2.append(" ");
        bufferStr2.append("H");
        bufferStr2.append(heightChar);
        bufferStr2.append(bufferStr.substr(HeightLocEnd, FrameLoc - HeightLocEnd));

        char buffer2[128];
        strncpy(buffer2, bufferStr2.c_str(), 128);
        out_put(out, (unsigned char *)buffer2, strlen(buffer2), 0);
        fsetpos(in, &position);

    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(in);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    currentVideoFrame = 1;

    int frame_avail = 1;
    int framesWritten = 0;

    while (frame_avail)
    {
        vpx_image_t raw;
        vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1);

        if (CharCount == 79)
        {
            tprintf(PRINT_STD, "\n");
            CharCount = 0;
        }

        tprintf(PRINT_STD, ".");
        CharCount++;

        frame_avail = read_frame_enc(in, &raw, file_type, &y4m, &detect);

        if (!frame_avail)
        {
            vpx_img_free(&raw);
            break;
        }

        if (vpx_img_set_rect(&raw, xoffset, yoffset, newFrameWidth, newFrameHeight) != 0)
        {
            tprintf(PRINT_STD, "ERROR: INVALID RESIZE\n");

            if (OutputToFile)
            {
                fprintf(stderr, "ERROR: INVALID RESIZE\n");
            }

            vpx_img_free(&raw);
            break;
        }

        if (!frame_avail)
        {
            vpx_img_free(&raw);
            break;
        }

        if (file_type == FILE_TYPE_Y4M)
        {
            out_put(out, (unsigned char *)"FRAME\n", 6, 0);
        }

        if (file_type == FILE_TYPE_IVF)
        {
            ivf_write_frame_header(out, 0, newFrameWidth * newFrameHeight + ((newFrameWidth + 1) / 2) *((newFrameHeight + 1) / 2) + ((newFrameWidth + 1) / 2) *((newFrameHeight + 1) / 2));
        }

        unsigned int y;
        uint8_t *buf;

        buf = raw.planes[VPX_PLANE_Y];

        for (y = 0; y < raw.d_h; y++)
        {
            out_put(out, buf, raw.d_w, 0);
            buf += raw.stride[VPX_PLANE_Y];
        }

        buf = raw.planes[VPX_PLANE_U];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_U];
        }

        buf = raw.planes[VPX_PLANE_V];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_V];
        }

        framesWritten++;
        currentVideoFrame++;

        vpx_img_free(&raw);
    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(in);
        rewind(out);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        ivfhRaw.length = framesWritten;
        ivfhRaw.width = newFrameWidth;
        ivfhRaw.height = newFrameHeight;
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    fclose(in);
    fclose(out);

    return(0);
}
int vpxt_pad_raw_clip(const char *inputFile, const char *outputFile, int newFrameWidth, int newFrameHeight, int FileIsIVF, int OutputToFile)
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

    y4m_input                y4m;
    unsigned int file_type, fourcc;
    unsigned long nbytes = 0;
    struct detect_buffer detect;

    unsigned int width;
    unsigned int height;
    unsigned int rate;
    unsigned int scale;
    //int arg_use_i420 = 1;

    detect.buf_read = fread(detect.buf, 1, 4, in);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(in, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, in, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            width = y4m.pic_w;
            height = y4m.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate = y4m.fps_n;
                scale = y4m.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 && file_is_ivf(in, &fourcc, &width, &height, &detect, &scale, &rate))
    {
        file_type = FILE_TYPE_IVF;

        switch (fourcc)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type = FILE_TYPE_RAW;
    }

    if (!width || !height)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    std::string inputformat;

    if (file_type == FILE_TYPE_RAW)
        inputformat = "Raw";

    if (file_type == FILE_TYPE_Y4M)
        inputformat = "Y4M";

    if (file_type == FILE_TYPE_IVF)
        inputformat = "IVF";

    //printf("\nwidth: %i height: %i rate: %i scale: %i \n",width, height, rate, scale);
    tprintf(PRINT_STD, "\nCrop %s file to %s file: \n", inputformat.c_str(), inputformat.c_str());

    if (file_type == FILE_TYPE_Y4M)
    {
        //copy and output y4m header
        fpos_t position;
        fgetpos(in, &position);
        rewind(in);

        char buffer[128];
        fread(buffer, 1, 128, in);

        std::string bufferStr = buffer;
        size_t WidthLoc = 0;
        size_t WidthLocEnd = 0;
        size_t HeightLoc = 0;
        size_t HeightLocEnd = 0;
        size_t FrameLoc = 0;

        char widthChar[32];
        char heightChar[32];
        vpxt_itoa_custom(newFrameWidth, widthChar, 10);
        vpxt_itoa_custom(newFrameHeight, heightChar, 10);

        WidthLoc = bufferStr.find("W");
        WidthLocEnd = bufferStr.find(" ", WidthLoc + 1);
        HeightLoc = bufferStr.find("H");
        HeightLocEnd = bufferStr.find(" ", HeightLoc + 1);
        FrameLoc = bufferStr.find("FRAME");

        std::string bufferStr2 = bufferStr.substr(0, WidthLoc);
        bufferStr2.append("W");
        bufferStr2.append(widthChar);
        bufferStr2.append(" ");
        bufferStr2.append("H");
        bufferStr2.append(heightChar);
        bufferStr2.append(bufferStr.substr(HeightLocEnd, FrameLoc - HeightLocEnd));

        char buffer2[128];
        strncpy(buffer2, bufferStr2.c_str(), 128);
        out_put(out, (unsigned char *)buffer2, strlen(buffer2), 0);
        fsetpos(in, &position);

    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(in);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    currentVideoFrame = 1;

    int frame_avail = 1;
    int framesWritten = 0;

    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1);

    while (frame_avail)
    {
        if (CharCount == 79)
        {
            tprintf(PRINT_STD, "\n");
            CharCount = 0;
        }

        tprintf(PRINT_STD, ".");
        CharCount++;

        frame_avail = read_frame_enc(in, &raw, file_type, &y4m, &detect);

        if (!frame_avail)
            break;

        if (file_type == FILE_TYPE_Y4M)
        {
            out_put(out, (unsigned char *)"FRAME\n", 6, 0);
        }

        if (file_type == FILE_TYPE_IVF)
        {
            ivf_write_frame_header(out, 0, newFrameWidth * newFrameHeight + ((newFrameWidth + 1) / 2) *((newFrameHeight + 1) / 2) + ((newFrameWidth + 1) / 2) *((newFrameHeight + 1) / 2));
        }

        unsigned int y;
        unsigned int z;
        uint8_t *buf;

        int size = 0;
        int offset = 0;

        int FrameSizeTrack = 0;

        buf = raw.planes[PLANE_Y];

        for (y = 0; y < raw.d_h; y++)
        {
            out_put(out, buf, raw.d_w, 0);
            buf += raw.stride[PLANE_Y];

            for (z = raw.d_w; z < newFrameWidth; z++)
            {
                char padchar = 'a';
                uint8_t padbuf = padchar;
                out_put(out, &padbuf, 1, 0);
            }
        }

        for (y = raw.d_h; y < newFrameHeight; y++)
        {
            char padchar = 'a';
            uint8_t padbuf = padchar;

            for (z = 0; z < newFrameWidth; z++)
            {
                char padchar = 'a';
                uint8_t padbuf = padchar;
                out_put(out, &padbuf, 1, 0);
            }
        }

        buf = raw.planes[PLANE_U];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[PLANE_U];

            for (z = (1 + raw.d_w) / 2; z < (1 + newFrameWidth) / 2; z++)
            {
                char padchar = 'a';
                uint8_t padbuf = padchar;
                out_put(out, &padbuf, 1, 0);
            }
        }

        for (y = (1 + raw.d_h) / 2; y < (1 + newFrameHeight) / 2; y++)
        {
            char padchar = 'a';
            uint8_t padbuf = padchar;

            for (z = 0 / 2; z < (1 + newFrameWidth) / 2; z++)
            {
                char padchar = 'a';
                uint8_t padbuf = padchar;
                out_put(out, &padbuf, 1, 0);
            }
        }

        buf = raw.planes[PLANE_V];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[PLANE_V];

            for (z = (1 + raw.d_w) / 2; z < (1 + newFrameWidth) / 2; z++)
            {
                char padchar = 'a';
                uint8_t padbuf = padchar;
                out_put(out, &padbuf, 1, 0);
            }
        }

        for (y = (1 + raw.d_h) / 2; y < (1 + newFrameHeight) / 2; y++)
        {
            char padchar = 'a';
            uint8_t padbuf = padchar;

            for (z = 0; z < (1 + newFrameWidth) / 2; z++)
            {
                char padchar = 'a';
                uint8_t padbuf = padchar;
                out_put(out, &padbuf, 1, 0);
            }
        }

        framesWritten++;
        currentVideoFrame++;
    }

    if (file_type == FILE_TYPE_IVF)
    {
        rewind(in);
        rewind(out);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
        ivfhRaw.length = framesWritten;
        ivfhRaw.width = newFrameWidth;
        ivfhRaw.height = newFrameHeight;
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    fclose(in);
    fclose(out);
    vpx_img_free(&raw);


    return(0);
}
int vpxt_paste_clip(const char *inputFile1, const char *inputFile2, const char *outputFile, int StartingFrame)
{
    bool verbose = 1;

    FILE *in1 = fopen(inputFile1, "rb");

    if (in1 == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");
        return 0;
    }

    FILE *in2 = fopen(inputFile2, "rb");

    if (in2 == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");
        return 0;
    }

    FILE *out = fopen(outputFile, "wb");

    if (out == NULL)
    {
        tprintf(PRINT_STD, "\nOutput file does not exist");
        fclose(in1);
        return 0;
    }

    int currentVideoFrame = 0;
    int CharCount = 0;

    y4m_input                y4m1;
    unsigned int file_type1, fourcc1;
    unsigned long nbytes1 = 0;
    struct detect_buffer detect1;

    unsigned int width1;
    unsigned int height1;
    unsigned int rate1;
    unsigned int scale1;
    //int arg_use_i420 = 1;

    detect1.buf_read = fread(detect1.buf, 1, 4, in1);
    detect1.position = 0;

    if (detect1.buf_read == 4 && file_is_y4m(in1, &y4m1, detect1.buf))
    {
        if (y4m_input_open(&y4m1, in1, detect1.buf, 4) >= 0)
        {
            file_type1 = FILE_TYPE_Y4M;
            width1 = y4m1.pic_w;
            height1 = y4m1.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate1 = y4m1.fps_n;
                scale1 = y4m1.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect1.buf_read == 4 && file_is_ivf(in1, &fourcc1, &width1, &height1, &detect1, &scale1, &rate1))
    {
        file_type1 = FILE_TYPE_IVF;

        switch (fourcc1)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc1);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type1 = FILE_TYPE_RAW;
    }

    if (!width1 || !height1)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    y4m_input                y4m2;
    unsigned int file_type2, fourcc2;
    unsigned long nbytes2 = 0;
    struct detect_buffer detect2;

    unsigned int width2;
    unsigned int height2;
    unsigned int rate2;
    unsigned int scale2;
    //int arg_use_i420 = 1;

    detect2.buf_read = fread(detect2.buf, 1, 4, in2);
    detect2.position = 0;

    if (detect2.buf_read == 4 && file_is_y4m(in2, &y4m2, detect2.buf))
    {
        if (y4m_input_open(&y4m2, in2, detect2.buf, 4) >= 0)
        {
            file_type2 = FILE_TYPE_Y4M;
            width2 = y4m2.pic_w;
            height2 = y4m2.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate2 = y4m2.fps_n;
                scale2 = y4m2.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect2.buf_read == 4 && file_is_ivf(in2, &fourcc2, &width2, &height2, &detect2, &scale2, &rate2))
    {
        file_type2 = FILE_TYPE_IVF;

        switch (fourcc2)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc2);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type2 = FILE_TYPE_RAW;
    }

    if (!width2 || !height2)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    if (width1 != width2)
    {
        printf("width1 %i != width2 %i\n", width1, width2);
        return 0;
    }

    if (height1 != height2)
    {
        printf("height1 %i != height2 %i\n", height1, height2);
        return 0;
    }

    if (file_type1 != file_type2)
    {
        printf("file_type1 %i != file_type2 %i\n", file_type1, file_type2);
        return 0;
    }

    /*if(fourcc1 != fourcc2){
    printf("fourcc1 %i != fourcc2 %i\n",fourcc1,fourcc2);
    return 0;
    }*/

    std::string inputformat1;
    std::string inputformat2;

    if (file_type1 == FILE_TYPE_RAW)
        inputformat1 = "Raw";

    if (file_type1 == FILE_TYPE_Y4M)
        inputformat1 = "Y4M";

    if (file_type1 == FILE_TYPE_IVF)
        inputformat1 = "IVF";

    if (file_type2 == FILE_TYPE_RAW)
        inputformat2 = "Raw";

    if (file_type2 == FILE_TYPE_Y4M)
        inputformat2 = "Y4M";

    if (file_type2 == FILE_TYPE_IVF)
        inputformat2 = "IVF";

    //printf("\nwidth: %i height: %i rate: %i scale: %i \n",width, height, rate, scale);
    tprintf(PRINT_STD, "\nPaste %s file into %s file: \n", inputformat2.c_str(), inputformat1.c_str());

    if (file_type1 == FILE_TYPE_Y4M)
    {
        //copy and output y4m header
        fpos_t position;
        fgetpos(in1, &position);
        rewind(in1);

        char buffer[128];
        fread(buffer, 1, 128, in1);

        std::string bufferStr = buffer;
        size_t HeaderEnd;
        std::string bufferStr2 = bufferStr.substr(0, bufferStr.find("FRAME"));
        char buffer2[128];
        strncpy(buffer2, bufferStr2.c_str(), 128);
        out_put(out, (unsigned char *)buffer2, strlen(buffer2), 0);
        fsetpos(in1, &position);
    }

    if (file_type1 == FILE_TYPE_IVF)
    {
        rewind(in1);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in1);
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width1, height1, 1);
    int frame_avail1 = 1;
    int frame_avail2 = 1;
    int framesWritten = 0;

    while (frame_avail1)
    {
        if (CharCount == 79)
        {
            tprintf(PRINT_STD, "\n");
            CharCount = 0;
        }

        CharCount++;

        if (framesWritten >= StartingFrame && frame_avail2)
        {
            tprintf(PRINT_STD, "*");
            frame_avail2 = read_frame_enc(in2, &raw, file_type1, &y4m1, &detect1);
        }
        else
        {
            tprintf(PRINT_STD, ".");
            frame_avail1 = read_frame_enc(in1, &raw, file_type1, &y4m1, &detect1);
        }

        if (!frame_avail2)
            frame_avail1 = read_frame_enc(in1, &raw, file_type1, &y4m1, &detect1);

        if (!frame_avail1)
            break;

        if (file_type1 == FILE_TYPE_Y4M)
        {
            out_put(out, (unsigned char *)"FRAME\n", 6, 0);
        }

        if (file_type1 == FILE_TYPE_IVF)
        {
            ivf_write_frame_header(out, 0, width1 * height1 * 3 / 2);
        }

        unsigned int y;
        uint8_t *buf;

        buf = raw.planes[VPX_PLANE_Y];

        for (y = 0; y < raw.d_h; y++)
        {
            out_put(out, buf, raw.d_w, 0);
            buf += raw.stride[VPX_PLANE_Y];
        }

        buf = raw.planes[VPX_PLANE_U];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_U];
        }

        buf = raw.planes[VPX_PLANE_V];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_V];
        }

        framesWritten++;
    }

    if (file_type1 == FILE_TYPE_IVF)
    {
        rewind(in1);
        rewind(out);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), in1);
        ivfhRaw.length = framesWritten;
        fwrite((char *)& ivfhRaw, 1, sizeof(ivfhRaw), out);
    }

    fclose(in1);
    fclose(in2);
    fclose(out);
    vpx_img_free(&raw);

    return(0);
}
int vpxt_formatted_to_raw(const std::string inputFile, const std::string outputFile)
{
    bool verbose = 1;

    FILE *in = fopen(inputFile.c_str(), "rb");

    if (in == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");
        return 0;
    }

    FILE *out = fopen(outputFile.c_str(), "wb");

    if (out == NULL)
    {
        tprintf(PRINT_STD, "\nOutput file does not exist");
        fclose(in);
        return 0;
    }

    int currentVideoFrame = 0;
    int CharCount = 0;

    y4m_input                y4m;
    unsigned int file_type, fourcc;
    unsigned long nbytes = 0;
    struct detect_buffer detect;

    unsigned int width;
    unsigned int height;
    unsigned int rate;
    unsigned int scale;
    //int arg_use_i420 = 1;

    detect.buf_read = fread(detect.buf, 1, 4, in);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(in, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, in, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            width = y4m.pic_w;
            height = y4m.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate = y4m.fps_n;
                scale = y4m.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 && file_is_ivf(in, &fourcc, &width, &height, &detect, &scale, &rate))
    {
        file_type = FILE_TYPE_IVF;

        switch (fourcc)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type = FILE_TYPE_RAW;
    }

    if (!width || !height)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    std::string inputformat;

    if (file_type == FILE_TYPE_RAW)
        inputformat = "Raw";

    if (file_type == FILE_TYPE_Y4M)
        inputformat = "Y4M";

    if (file_type == FILE_TYPE_IVF)
        inputformat = "IVF";

    currentVideoFrame = 1;

    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1);
    int frame_avail = 1;
    int framesWritten = 0;

    tprintf(PRINT_STD, "\n");

    while (frame_avail)
    {
        if (CharCount == 79)
        {
            tprintf(PRINT_STD, "\n");
            CharCount = 0;
        }

        tprintf(PRINT_STD, ".");
        CharCount++;

        frame_avail = read_frame_enc(in, &raw, file_type, &y4m, &detect);

        if (!frame_avail)
            break;

        unsigned int y;
        uint8_t *buf;

        buf = raw.planes[VPX_PLANE_Y];

        for (y = 0; y < raw.d_h; y++)
        {
            out_put(out, buf, raw.d_w, 0);
            buf += raw.stride[VPX_PLANE_Y];
        }

        buf = raw.planes[VPX_PLANE_U];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_U];
        }

        buf = raw.planes[VPX_PLANE_V];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_V];
        }

        framesWritten++;
        currentVideoFrame++;
    }

    fclose(in);
    fclose(out);
    vpx_img_free(&raw);

    return(0);
}
int vpxt_formatted_to_raw_frames(const std::string inputFile, const std::string outputFile)
{
    int frameLength = vpxt_get_number_of_frames(inputFile.c_str());
    int LastFrameDecPlaces = vpxt_decimal_places(frameLength);

    bool verbose = 1;

    FILE *in = fopen(inputFile.c_str(), "rb");

    if (in == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame = 0;
    int CharCount = 0;

    y4m_input                y4m;
    unsigned int file_type, fourcc;
    unsigned long nbytes = 0;

    struct detect_buffer detect;

    unsigned int width;
    unsigned int height;
    unsigned int rate;
    unsigned int scale;
    //int arg_use_i420 = 1;

    detect.buf_read = fread(detect.buf, 1, 4, in);
    detect.position = 0;

    if (detect.buf_read == 4 && file_is_y4m(in, &y4m, detect.buf))
    {
        if (y4m_input_open(&y4m, in, detect.buf, 4) >= 0)
        {
            file_type = FILE_TYPE_Y4M;
            width = y4m.pic_w;
            height = y4m.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate = y4m.fps_n;
                scale = y4m.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect.buf_read == 4 && file_is_ivf(in, &fourcc, &width, &height, &detect, &scale, &rate))
    {
        file_type = FILE_TYPE_IVF;

        switch (fourcc)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type = FILE_TYPE_RAW;
    }

    if (!width || !height)
    {
        fprintf(stderr, "\nunsupported file type.");
        return EXIT_FAILURE;
    }

    std::string inputformat;

    if (file_type == FILE_TYPE_RAW)
        inputformat = "Raw";

    if (file_type == FILE_TYPE_Y4M)
        inputformat = "Y4M";

    if (file_type == FILE_TYPE_IVF)
        inputformat = "IVF";

    currentVideoFrame = 1;

    vpx_image_t raw;
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1);
    int frame_avail = 1;
    int framesWritten = 0;

    tprintf(PRINT_STD, "\n");

    while (frame_avail)
    {
        char currentVideoFrameStr[10];
        char widthchar[10];
        char heightchar[10];
        vpxt_itoa_custom(currentVideoFrame, currentVideoFrameStr, 10);
        vpxt_itoa_custom(width, widthchar, 10);
        vpxt_itoa_custom(height, heightchar, 10);

        std::string FrameFileName;
        vpxt_remove_file_extension(outputFile.c_str(), FrameFileName);
        FrameFileName.erase(FrameFileName.length() - 1, 1);
        FrameFileName.append("-");
        FrameFileName.append(widthchar);
        FrameFileName.append("x");
        FrameFileName.append(heightchar);
        FrameFileName.append("-Frame-");

        int CurNumDecPlaces = vpxt_decimal_places(currentVideoFrame);

        while (CurNumDecPlaces < LastFrameDecPlaces) //add zeros for increasing frames
        {
            //printf("%i < %i ", CurNumDecPlaces,InputDecPlaces);
            FrameFileName.append("0");
            CurNumDecPlaces++;
        }

        FrameFileName.append(currentVideoFrameStr);
        FrameFileName.append(".raw");

        //printf("Name: %s\n", FrameFileName.c_str());

        FILE *out = fopen(FrameFileName.c_str(), "wb");

        if (out == NULL)
        {
            tprintf(PRINT_STD, "\nOutput file does not exist");
            fclose(in);
            return 0;
        }

        if (CharCount == 79)
        {
            tprintf(PRINT_STD, "\n");
            CharCount = 0;
        }

        tprintf(PRINT_STD, ".");
        CharCount++;

        frame_avail = read_frame_enc(in, &raw, file_type, &y4m, &detect);

        if (!frame_avail)
            break;

        unsigned int y;
        uint8_t *buf;

        buf = raw.planes[VPX_PLANE_Y];

        for (y = 0; y < raw.d_h; y++)
        {
            out_put(out, buf, raw.d_w, 0);
            buf += raw.stride[VPX_PLANE_Y];
        }

        buf = raw.planes[VPX_PLANE_U];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_U];
        }

        buf = raw.planes[VPX_PLANE_V];

        for (y = 0; y < (1 + raw.d_h) / 2; y++)
        {
            out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf += raw.stride[VPX_PLANE_V];
        }

        fclose(out);
        currentVideoFrame++;
    }

    fclose(in);
    vpx_img_free(&raw);

    return(0);
}
int vpxt_display_header_info(int argc, const char *const *argv)
{
    char inputFile[256] = "";
    strncpy(inputFile, argv[2], 256);
    int extrafileinfo = 0;

    FILE *fp;

    if (argc > 3)
        extrafileinfo = atoi(argv[3]);

    if (argc > 4)
    {
        char outputFile[256] = "";
        strncpy(outputFile, argv[4], 256);

        if ((fp = freopen(outputFile, "w", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n", outputFile);
            exit(1);
        }
    }

    unsigned char  signature[4]; //='DKIF';
    unsigned short version = 0;     // -
    unsigned short headersize = 0;  // -
    unsigned int fourcc = 0;        //good
    unsigned int width = 0;         //good
    unsigned int height = 0;        //good
    unsigned int rate = 0;          //good
    unsigned int scale = 0;         //good
    unsigned int length = 0;        //other measure
    unsigned char unused[4];        // -

    signature[0] = ' ';
    signature[1] = ' ';
    signature[2] = ' ';
    signature[3] = ' ';

    std::vector<unsigned int>     frameSize;
    std::vector<unsigned __int64> timeStamp;

    FILE *infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    struct input_ctx        input;

    input.chunk = 0;

    input.chunks = 0;

    input.infile = NULL;

    input.kind = RAW_FILE;

    input.nestegg_ctx = 0;

    input.pkt = 0;

    input.video_track = 0;

    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &scale, &rate))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &scale, &rate))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    std::string compformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (input.kind == IVF_FILE)
    {
        rewind(infile);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile);

        version = ivfhRaw.version;
        headersize = ivfhRaw.headersize;
        fourcc = ivfhRaw.four_cc;
        width = ivfhRaw.width;
        height = ivfhRaw.height;
        rate = ivfhRaw.rate;
        scale = ivfhRaw.scale;
        length = ivfhRaw.length;

        signature[0] = ivfhRaw.signature[0];
        signature[1] = ivfhRaw.signature[1];
        signature[2] = ivfhRaw.signature[2];
        signature[3] = ivfhRaw.signature[3];
    }

    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    int currentVideoFrame = 0;
    int frame_avail = 0;

    while (!frame_avail)
    {
        frame_avail = skim_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz);

        if (!frame_avail && (extrafileinfo == 1 || currentVideoFrame == 0))
        {
            frameSize.push_back(buf_sz);
            timeStamp.push_back(0);
            //timeStamp.push_back(input.pkt->timecode);
        }
        else
            frame_avail = 1;

        currentVideoFrame++;
    }

    if (input.kind == WEBM_FILE)
        length = currentVideoFrame;

    tprintf(PRINT_STD, "\nFILE TYPE IS: %s\n", compformat.c_str());
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
            , signature[0], signature[1], signature[2], signature[3]
            , version, headersize, fourcc, width, height, rate, scale, length, unused);

    if (argc > 4)
    {
        tprintf(PRINT_ERR, "\nFILE TYPE IS: %s\n", compformat.c_str());
        tprintf(PRINT_ERR, "\n"
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
                , signature[0], signature[1], signature[2], signature[3]
                , version, headersize, fourcc, width, height, rate, scale, length, unused);
    }

    currentVideoFrame = 0;

    while (currentVideoFrame < frameSize.size())
    {
        tprintf(PRINT_STD, "FRAME HEADER %i\n"
                "Frame Size            - %i \n"
                "Time Stamp            - %i \n"
                "\n"

                , currentVideoFrame, frameSize[currentVideoFrame], (int)timeStamp[currentVideoFrame]);

        if (argc > 4)
        {
            tprintf(PRINT_ERR, "FRAME HEADER %i\n"
                    "Frame Size            - %i \n"
                    "Time Stamp            - %i \n"
                    "\n"

                    , currentVideoFrame, frameSize[currentVideoFrame], (int)timeStamp[currentVideoFrame]);
        }

        currentVideoFrame++;
    }

    fclose(infile);

    if (argc > 4)
        fclose(fp);

    return 0;
}
int vpxt_compare_header_info(int argc, const char *const *argv)
{
    const char *inputFile1 = argv[2];
    const char *inputFile2 = argv[3];

    const char *outputfile = argv[5];

    FILE *fp;

    if (argc == 6)
    {
        if ((fp = freopen(outputfile, "w", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n", outputfile);
            exit(1);
        }
    }

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

    int returnval = -1;

    unsigned char  signature_1[4]; //='DKIF';
    unsigned short version_1 = 0;       // -
    unsigned short headersize_1 = 0;    // -
    unsigned int fourcc_1 = 0;      //good
    unsigned int width_1 = 0;           //good
    unsigned int height_1 = 0;      //good
    unsigned int rate_1 = 0;            //good
    unsigned int scale_1 = 0;           //good
    unsigned int length_1 = 0;      //other measure
    unsigned char unused_1[4];      // -

    unsigned char  signature_2[4]; //='DKIF';
    unsigned short version_2 = 0;       // -
    unsigned short headersize_2 = 0;    // -
    unsigned int fourcc_2 = 0;      //good
    unsigned int width_2 = 0;           //good
    unsigned int height_2 = 0;      //good
    unsigned int rate_2 = 0;            //good
    unsigned int scale_2 = 0;           //good
    unsigned int length_2 = 0;      //other measure
    unsigned char unused_2[4];      // -

    signature_1[0] = ' ';
    signature_1[1] = ' ';
    signature_1[2] = ' ';
    signature_1[3] = ' ';

    signature_2[0] = ' ';
    signature_2[1] = ' ';
    signature_2[2] = ' ';
    signature_2[3] = ' ';

    FILE *infile_1 = strcmp(inputFile1, "-") ? fopen(inputFile1, "rb") : set_binary_mode(stdin);
    FILE *infile_2 = strcmp(inputFile2, "-") ? fopen(inputFile2, "rb") : set_binary_mode(stdin);

    if (!infile_1)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile1);
        return -1;
    }

    struct input_ctx        input_1;

    struct input_ctx        input_2;

    input_1.chunk = 0;

    input_1.chunks = 0;

    input_1.infile = NULL;

    input_1.kind = RAW_FILE;

    input_1.nestegg_ctx = 0;

    input_1.pkt = 0;

    input_1.video_track = 0;

    input_2.chunk = 0;

    input_2.chunks = 0;

    input_2.infile = NULL;

    input_2.kind = RAW_FILE;

    input_2.nestegg_ctx = 0;

    input_2.pkt = 0;

    input_2.video_track = 0;

    input_1.infile = infile_1;

    if (file_is_ivf_dec(infile_1, &fourcc_1, &width_1, &height_1, &scale_1, &rate_1))
        input_1.kind = IVF_FILE;
    else if (file_is_webm(&input_1, &fourcc_1, &width_1, &height_1, &scale_1, &rate_1))
        input_1.kind = WEBM_FILE;
    else if (file_is_raw(infile_1, &fourcc_1, &width_1, &height_1, &scale_1, &rate_1))
        input_1.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input_1.kind == WEBM_FILE)
        if (webm_guess_framerate(&input_1, &scale_1, &rate_1))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    input_2.infile = infile_2;

    if (file_is_ivf_dec(infile_2, &fourcc_2, &width_2, &height_2, &scale_2, &rate_2))
        input_2.kind = IVF_FILE;
    else if (file_is_webm(&input_2, &fourcc_2, &width_2, &height_2, &scale_2, &rate_2))
        input_2.kind = WEBM_FILE;
    else if (file_is_raw(infile_2, &fourcc_2, &width_2, &height_2, &scale_2, &rate_2))
        input_2.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input_2.kind == WEBM_FILE)
        if (webm_guess_framerate(&input_2, &scale_2, &rate_2))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    std::string compformat_1;
    std::string compformat_2;

    if (input_1.kind == IVF_FILE)
        compformat_1 = "IVF";

    if (input_1.kind == WEBM_FILE)
        compformat_1 = "Webm";

    if (input_1.kind == RAW_FILE)
        compformat_1 = "Raw";

    if (input_2.kind == IVF_FILE)
        compformat_2 = "IVF";

    if (input_2.kind == WEBM_FILE)
        compformat_2 = "Webm";

    if (input_2.kind == RAW_FILE)
        compformat_2 = "Raw";

    if (input_1.kind == IVF_FILE)
    {
        rewind(infile_1);
        IVF_HEADER ivfhRaw_1;
        InitIVFHeader(&ivfhRaw_1);
        fread(&ivfhRaw_1, 1, sizeof(ivfhRaw_1), infile_1);

        version_1 = ivfhRaw_1.version;
        headersize_1 = ivfhRaw_1.headersize;
        fourcc_1 = ivfhRaw_1.four_cc;
        width_1 = ivfhRaw_1.width;
        height_1 = ivfhRaw_1.height;
        rate_1 = ivfhRaw_1.rate;
        scale_1 = ivfhRaw_1.scale;
        length_1 = ivfhRaw_1.length;

        signature_1[0] = ivfhRaw_1.signature[0];
        signature_1[1] = ivfhRaw_1.signature[1];
        signature_1[2] = ivfhRaw_1.signature[2];
        signature_1[3] = ivfhRaw_1.signature[3];
    }

    if (input_2.kind == IVF_FILE)
    {
        rewind(infile_2);
        IVF_HEADER ivfhRaw_2;
        InitIVFHeader(&ivfhRaw_2);
        fread(&ivfhRaw_2, 1, sizeof(ivfhRaw_2), infile_2);

        version_2 = ivfhRaw_2.version;
        headersize_2 = ivfhRaw_2.headersize;
        fourcc_2 = ivfhRaw_2.four_cc;
        width_2 = ivfhRaw_2.width;
        height_2 = ivfhRaw_2.height;
        rate_2 = ivfhRaw_2.rate;
        scale_2 = ivfhRaw_2.scale;
        length_2 = ivfhRaw_2.length;

        signature_2[0] = ivfhRaw_2.signature[0];
        signature_2[1] = ivfhRaw_2.signature[1];
        signature_2[2] = ivfhRaw_2.signature[2];
        signature_2[3] = ivfhRaw_2.signature[3];
    }

    uint8_t               *buf_1 = NULL;
    uint8_t               *buf_2 = NULL;
    uint32_t               buf_sz_1 = 0, buf_alloc_sz_1 = 0;
    uint32_t               buf_sz_2 = 0, buf_alloc_sz_2 = 0;
    int currentVideoFrame = 0;
    int frame_avail_1 = 0;
    int frame_avail_2 = 0;

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
            , signature_1[0], signature_1[1], signature_1[2], 9, signature_1[3]
            , signature_2, signature_2[1], signature_2[2], signature_2[3]
            , 12, version_1, version_2
            , 12, headersize_1, headersize_2
            , 12, fourcc_1, fourcc_2
            , 12, width_1, width_2
            , 12, height_1, height_2
            , 12, rate_1, rate_2
            , 12, scale_1, scale_2
            , 12, length_1, length_2
            , 12, unused_1, unused_2);

    if (argc == 6)
    {
        tprintf(PRINT_ERR, "\n"
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
                , signature_1[0], signature_1[1], signature_1[2], 9, signature_1[3]
                , signature_2, signature_2[1], signature_2[2], signature_2[3]
                , 12, version_1, version_2
                , 12, headersize_1, headersize_2
                , 12, fourcc_1, fourcc_2
                , 12, width_1, width_2
                , 12, height_1, height_2
                , 12, rate_1, rate_2
                , 12, scale_1, scale_2
                , 12, length_1, length_2
                , 12, unused_1, unused_2);
    }

    while (!frame_avail_1 && !frame_avail_2)
    {
        frame_avail_1 = skim_frame_dec(&input_1, &buf_1, (size_t *)&buf_sz_1, (size_t *)&buf_alloc_sz_1);
        frame_avail_2 = skim_frame_dec(&input_2, &buf_2, (size_t *)&buf_sz_2, (size_t *)&buf_alloc_sz_2);

        if (frame_avail_1 && frame_avail_2)
        {
            break;
        }

        if (!frame_avail_1 && frame_avail_2)
        {
            tprintf(PRINT_STD,
                    "FRAME HEADER1 %-*i\n"
                    "Frame Size            - %-*i\n"
                    "Time Stamp            - %-*i\n"
                    "\n"

                    , 22, currentVideoFrame
                    , 12, buf_sz_1
                    , 12, 0);

            if (argc == 6)
            {
                tprintf(PRINT_ERR,
                        "FRAME HEADER1 %-*i\n"
                        "Frame Size            - %-*i\n"
                        "Time Stamp            - %-*i\n"
                        "\n"

                        , 22, currentVideoFrame
                        , 12, buf_sz_1
                        , 12, 0);
            }
        }

        if (!frame_avail_2 && frame_avail_1)
        {
            tprintf(PRINT_STD,
                    "                                    FRAME HEADER2 %i\n"
                    "                                    Frame Size            - %i\n"
                    "                                    Time Stamp            - %i\n"
                    "\n"

                    , currentVideoFrame
                    , buf_sz_2
                    , 0);

            if (argc == 6)
            {
                tprintf(PRINT_ERR,
                        "                                    FRAME HEADER2 %i\n"
                        "                                    Frame Size            - %i\n"
                        "                                    Time Stamp            - %i\n"
                        "\n"

                        , currentVideoFrame
                        , buf_sz_2
                        , 0);
            }
        }

        if (!frame_avail_1 && !frame_avail_2)
        {
            tprintf(PRINT_STD, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
                    "Frame Size            - %-*iFrame Size            - %i\n"
                    "Time Stamp            - %-*iTime Stamp            - %i\n"
                    "\n"

                    , 22, currentVideoFrame, currentVideoFrame
                    , 12, buf_sz_1, buf_sz_2
                    , 12, 0, 0);

            if (argc == 6)
            {
                tprintf(PRINT_ERR, "FRAME HEADER1 %-*iFRAME HEADER2 %i\n"
                        "Frame Size            - %-*iFrame Size            - %i\n"
                        "Time Stamp            - %-*iTime Stamp            - %i\n"
                        "\n"

                        , 22, currentVideoFrame, currentVideoFrame
                        , 12, buf_sz_1, buf_sz_2
                        , 12, 0, 0);
            }
        }

        currentVideoFrame++;
    }

    if (input_1.nestegg_ctx)
        nestegg_destroy(input_1.nestegg_ctx);

    if (input_2.nestegg_ctx)
        nestegg_destroy(input_2.nestegg_ctx);

    if (input_1.kind != WEBM_FILE)
        free(buf_1);

    if (input_2.kind != WEBM_FILE)
        free(buf_2);

    fclose(infile_1);
    fclose(infile_2);

    if (argc == 6)
        fclose(fp);

    return returnval;
}
int vpxt_compare_dec(const char *inputFile1, const char *inputFile2)
{
    ////Returns:
    //-1 if files are identical
    //-2 if file 2 ends before File 1
    //-3 if file 1 ends before File 2
    //and >= 0 where the number the function returns is the frame that they differ first on.

    int returnval = -1;

    FILE *in_1 = fopen(inputFile1, "rb");

    if (in_1 == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");
        return 0;
    }

    FILE *in_2 = fopen(inputFile2, "rb");

    if (in_2 == NULL)
    {
        tprintf(PRINT_STD, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame_1 = 0;
    int CharCount_1 = 0;

    y4m_input                y4m_1;
    unsigned int file_type_1, fourcc_1;
    unsigned long nbytes_1 = 0;
    struct detect_buffer detect_1;

    y4m_input                y4m_2;
    unsigned int file_type_2, fourcc_2;
    unsigned long nbytes_2 = 0;
    struct detect_buffer detect_2;

    unsigned int width_1;
    unsigned int height_1;
    unsigned int rate_1;
    unsigned int scale_1;

    unsigned int width_2;
    unsigned int height_2;
    unsigned int rate_2;
    unsigned int scale_2;

    detect_1.buf_read = fread(detect_1.buf, 1, 4, in_1);
    detect_1.position = 0;

    detect_2.buf_read = fread(detect_2.buf, 1, 4, in_2);
    detect_2.position = 0;

    if (detect_1.buf_read == 4 && file_is_y4m(in_1, &y4m_1, detect_1.buf))
    {
        if (y4m_input_open(&y4m_1, in_1, detect_1.buf, 4) >= 0)
        {
            file_type_1 = FILE_TYPE_Y4M;
            width_1 = y4m_1.pic_w;
            height_1 = y4m_1.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate_1 = y4m_1.fps_n;
                scale_1 = y4m_1.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect_1.buf_read == 4 && file_is_ivf(in_1, &fourcc_1, &width_1, &height_1, &detect_1, &scale_1, &rate_1))
    {
        file_type_1 = FILE_TYPE_IVF;

        switch (fourcc_1)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc_1);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type_1 = FILE_TYPE_RAW;
    }

    if (!width_1 || !height_1)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    if (detect_2.buf_read == 4 && file_is_y4m(in_2, &y4m_2, detect_2.buf))
    {
        if (y4m_input_open(&y4m_2, in_2, detect_2.buf, 4) >= 0)
        {
            file_type_2 = FILE_TYPE_Y4M;
            width_2 = y4m_2.pic_w;
            height_2 = y4m_2.pic_h;

            /* Use the frame rate from the file only if none was specified
            * on the command-line.
            */
            if (1)
            {
                rate_2 = y4m_2.fps_n;
                scale_2 = y4m_2.fps_d;
            }

            //arg_use_i420 = 0;
        }
        else
        {
            fprintf(stderr, "Unsupported Y4M stream.\n");
            return EXIT_FAILURE;
        }
    }
    else if (detect_2.buf_read == 4 && file_is_ivf(in_2, &fourcc_2, &width_2, &height_2, &detect_2, &scale_2, &rate_2))
    {
        file_type_2 = FILE_TYPE_IVF;

        switch (fourcc_2)
        {
        case 0x32315659:
            //arg_use_i420 = 0;
            break;
        case 0x30323449:
            //arg_use_i420 = 1;
            break;
        default:
            fprintf(stderr, "Unsupported fourcc (%08x) in IVF\n", fourcc_2);
            return EXIT_FAILURE;
        }
    }
    else
    {
        file_type_2 = FILE_TYPE_RAW;
    }

    if (!width_2 || !height_2)
    {
        fprintf(stderr, "Specify stream dimensions with --width (-w) "
                " and --height (-h).\n");
        return EXIT_FAILURE;
    }

    std::string inputformat_1;
    std::string inputformat_2;

    if (file_type_1 == FILE_TYPE_RAW)
        inputformat_1 = "Raw";

    if (file_type_1 == FILE_TYPE_Y4M)
        inputformat_1 = "Y4M";

    if (file_type_1 == FILE_TYPE_IVF)
        inputformat_1 = "IVF";

    if (file_type_2 == FILE_TYPE_RAW)
        inputformat_2 = "Raw";

    if (file_type_2 == FILE_TYPE_Y4M)
        inputformat_2 = "Y4M";

    if (file_type_2 == FILE_TYPE_IVF)
        inputformat_2 = "IVF";

    int currentVideoFrame = 1;

    int frame_avail_1 = 1;
    int frame_avail_2 = 1;
    vpx_image_t raw_2;
    vpx_image_t raw_1;

    if (file_type_1 == FILE_TYPE_Y4M)
        /*The Y4M reader does its own allocation.
        Just initialize this here to avoid problems if we never read any
        frames.*/
        memset(&raw_1, 0, sizeof(raw_1));
    else
        vpx_img_alloc(&raw_1, VPX_IMG_FMT_I420, width_1, height_1, 1);

    if (file_type_2 == FILE_TYPE_Y4M)
        /*The Y4M reader does its own allocation.
        Just initialize this here to avoid problems if we never read any
        frames.*/
        memset(&raw_2, 0, sizeof(raw_2));
    else
        vpx_img_alloc(&raw_2, VPX_IMG_FMT_I420, width_2, height_2, 1);

    while (frame_avail_1 && frame_avail_2)
    {
        frame_avail_1 = read_frame_enc(in_1, &raw_1, file_type_1, &y4m_1, &detect_1);
        frame_avail_2 = read_frame_enc(in_2, &raw_2, file_type_2, &y4m_2, &detect_2);

        if (!frame_avail_1 || !frame_avail_2)
        {
            if (file_type_1 == FILE_TYPE_IVF)
                vpx_img_free(&raw_1);

            if (file_type_1 == FILE_TYPE_IVF)
                vpx_img_free(&raw_2);

            fclose(in_1);
            fclose(in_2);

            if (file_type_1 == FILE_TYPE_Y4M)
                y4m_input_close(&y4m_1);

            if (file_type_2 == FILE_TYPE_Y4M)
                y4m_input_close(&y4m_2);

            if (frame_avail_1 == frame_avail_2)
                return -1;
            else
                return currentVideoFrame + 1;
        }

        if (raw_1.d_w != raw_2.d_w || raw_1.d_h != raw_2.d_h)
        {
            if (file_type_1 == FILE_TYPE_IVF)
                vpx_img_free(&raw_1);

            if (file_type_1 == FILE_TYPE_IVF)
                vpx_img_free(&raw_2);

            fclose(in_1);
            fclose(in_2);

            if (file_type_1 == FILE_TYPE_Y4M)
                y4m_input_close(&y4m_1);

            if (file_type_2 == FILE_TYPE_Y4M)
                y4m_input_close(&y4m_2);

            return currentVideoFrame + 1;
        }

        unsigned int y;
        uint8_t *buf_1;
        uint8_t *buf_2;

        buf_1 = raw_1.planes[VPX_PLANE_Y];
        buf_2 = raw_2.planes[VPX_PLANE_Y];

        for (y = 0; y < raw_1.d_h; y++)
        {
            if (memcmp(buf_1, buf_2, raw_1.d_w) != 0)
            {
                if (file_type_1 == FILE_TYPE_IVF)
                    vpx_img_free(&raw_1);

                if (file_type_1 == FILE_TYPE_IVF)
                    vpx_img_free(&raw_2);

                fclose(in_1);
                fclose(in_2);

                if (file_type_1 == FILE_TYPE_Y4M)
                    y4m_input_close(&y4m_1);

                if (file_type_2 == FILE_TYPE_Y4M)
                    y4m_input_close(&y4m_2);

                return currentVideoFrame + 1;
            }

            //out_put(out, buf, raw.d_w, 0);
            buf_1 += raw_1.stride[VPX_PLANE_Y];
            buf_2 += raw_2.stride[VPX_PLANE_Y];
        }

        buf_1 = raw_1.planes[VPX_PLANE_U];
        buf_2 = raw_2.planes[VPX_PLANE_U];

        for (y = 0; y < (1 + raw_1.d_h) / 2; y++)
        {
            if (memcmp(buf_1, buf_2, (1 + raw_1.d_w) / 2) != 0)
            {
                if (file_type_1 == FILE_TYPE_IVF)
                    vpx_img_free(&raw_1);

                if (file_type_1 == FILE_TYPE_IVF)
                    vpx_img_free(&raw_2);

                fclose(in_1);
                fclose(in_2);

                if (file_type_1 == FILE_TYPE_Y4M)
                    y4m_input_close(&y4m_1);

                if (file_type_2 == FILE_TYPE_Y4M)
                    y4m_input_close(&y4m_2);

                return currentVideoFrame + 1;
            }

            //out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf_1 += raw_1.stride[VPX_PLANE_U];
            buf_2 += raw_2.stride[VPX_PLANE_U];
        }

        buf_1 = raw_1.planes[VPX_PLANE_V];
        buf_2 = raw_2.planes[VPX_PLANE_V];

        for (y = 0; y < (1 + raw_1.d_h) / 2; y++)
        {
            if (memcmp(buf_1, buf_2, (1 + raw_1.d_w) / 2) != 0)
            {
                if (file_type_1 == FILE_TYPE_IVF)
                    vpx_img_free(&raw_1);

                if (file_type_1 == FILE_TYPE_IVF)
                    vpx_img_free(&raw_2);

                fclose(in_1);
                fclose(in_2);

                if (file_type_1 == FILE_TYPE_Y4M)
                    y4m_input_close(&y4m_1);

                if (file_type_2 == FILE_TYPE_Y4M)
                    y4m_input_close(&y4m_2);

                return currentVideoFrame + 1;
            }

            //out_put(out, buf, (1 + raw.d_w) / 2, 0);
            buf_1 += raw_1.stride[VPX_PLANE_V];
            buf_2 += raw_2.stride[VPX_PLANE_V];
        }

        //free(buf_1);
        //free(buf_2);
        //vpx_img_free(&raw_1);
        //vpx_img_free(&raw_2);
        currentVideoFrame++;
    }


    if (file_type_1 == FILE_TYPE_IVF)
        vpx_img_free(&raw_1);

    if (file_type_1 == FILE_TYPE_IVF)
        vpx_img_free(&raw_2);

    fclose(in_1);
    fclose(in_2);

    if (file_type_1 == FILE_TYPE_Y4M)
        y4m_input_close(&y4m_1);

    if (file_type_2 == FILE_TYPE_Y4M)
        y4m_input_close(&y4m_2);

    return returnval;
}
int vpxt_compare_enc(const char *inputFile1, const char *inputFile2)
{
    ////Returns:
    //-1 if files are identical
    //-2 if file 2 ends before File 1
    //-3 if file 1 ends before File 2
    //and >= 0 where the number the function returns is the frame that they differ first on.

    int returnval = -1;

    unsigned char  signature_1[4]; //='DKIF';
    unsigned short version_1 = 0;       // -
    unsigned short headersize_1 = 0;    // -
    unsigned int fourcc_1 = 0;      //good
    unsigned int width_1 = 0;           //good
    unsigned int height_1 = 0;      //good
    unsigned int rate_1 = 0;            //good
    unsigned int scale_1 = 0;           //good
    unsigned int length_1 = 0;      //other measure
    unsigned char unused_1[4];      // -

    unsigned char  signature_2[4]; //='DKIF';
    unsigned short version_2 = 0;       // -
    unsigned short headersize_2 = 0;    // -
    unsigned int fourcc_2 = 0;      //good
    unsigned int width_2 = 0;           //good
    unsigned int height_2 = 0;      //good
    unsigned int rate_2 = 0;            //good
    unsigned int scale_2 = 0;           //good
    unsigned int length_2 = 0;      //other measure
    unsigned char unused_2[4];      // -

    signature_1[0] = ' ';
    signature_1[1] = ' ';
    signature_1[2] = ' ';
    signature_1[3] = ' ';

    signature_2[0] = ' ';
    signature_2[1] = ' ';
    signature_2[2] = ' ';
    signature_2[3] = ' ';

    FILE *infile_1 = strcmp(inputFile1, "-") ? fopen(inputFile1, "rb") : set_binary_mode(stdin);
    FILE *infile_2 = strcmp(inputFile2, "-") ? fopen(inputFile2, "rb") : set_binary_mode(stdin);

    if (!infile_1)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile1);
        return -1;
    }

    struct input_ctx        input_1;

    struct input_ctx        input_2;

    input_1.chunk = 0;

    input_1.chunks = 0;

    input_1.infile = NULL;

    input_1.kind = RAW_FILE;

    input_1.nestegg_ctx = 0;

    input_1.pkt = 0;

    input_1.video_track = 0;

    input_2.chunk = 0;

    input_2.chunks = 0;

    input_2.infile = NULL;

    input_2.kind = RAW_FILE;

    input_2.nestegg_ctx = 0;

    input_2.pkt = 0;

    input_2.video_track = 0;

    input_1.infile = infile_1;

    if (file_is_ivf_dec(infile_1, &fourcc_1, &width_1, &height_1, &scale_1, &rate_1))
        input_1.kind = IVF_FILE;
    else if (file_is_webm(&input_1, &fourcc_1, &width_1, &height_1, &scale_1, &rate_1))
        input_1.kind = WEBM_FILE;
    else if (file_is_raw(infile_1, &fourcc_1, &width_1, &height_1, &scale_1, &rate_1))
        input_1.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return -5;
    }

    if (input_1.kind == WEBM_FILE)
        if (webm_guess_framerate(&input_1, &scale_1, &rate_1))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    input_2.infile = infile_2;

    if (file_is_ivf_dec(infile_2, &fourcc_2, &width_2, &height_2, &scale_2, &rate_2))
        input_2.kind = IVF_FILE;
    else if (file_is_webm(&input_2, &fourcc_2, &width_2, &height_2, &scale_2, &rate_2))
        input_2.kind = WEBM_FILE;
    else if (file_is_raw(infile_2, &fourcc_2, &width_2, &height_2, &scale_2, &rate_2))
        input_2.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return -5;
    }

    if (input_2.kind == WEBM_FILE)
        if (webm_guess_framerate(&input_2, &scale_2, &rate_2))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    std::string compformat_1;
    std::string compformat_2;

    if (input_1.kind == IVF_FILE)
        compformat_1 = "IVF";

    if (input_1.kind == WEBM_FILE)
        compformat_1 = "Webm";

    if (input_1.kind == RAW_FILE)
        compformat_1 = "Raw";

    if (input_2.kind == IVF_FILE)
        compformat_2 = "IVF";

    if (input_2.kind == WEBM_FILE)
        compformat_2 = "Webm";

    if (input_2.kind == RAW_FILE)
        compformat_2 = "Raw";

    if (input_1.kind == IVF_FILE)
    {
        rewind(infile_1);
        IVF_HEADER ivfhRaw_1;
        InitIVFHeader(&ivfhRaw_1);
        fread(&ivfhRaw_1, 1, sizeof(ivfhRaw_1), infile_1);

        version_1 = ivfhRaw_1.version;
        headersize_1 = ivfhRaw_1.headersize;
        fourcc_1 = ivfhRaw_1.four_cc;
        width_1 = ivfhRaw_1.width;
        height_1 = ivfhRaw_1.height;
        rate_1 = ivfhRaw_1.rate;
        scale_1 = ivfhRaw_1.scale;
        length_1 = ivfhRaw_1.length;

        signature_1[0] = ivfhRaw_1.signature[0];
        signature_1[1] = ivfhRaw_1.signature[1];
        signature_1[2] = ivfhRaw_1.signature[2];
        signature_1[3] = ivfhRaw_1.signature[3];
    }

    if (input_2.kind == IVF_FILE)
    {
        rewind(infile_2);
        IVF_HEADER ivfhRaw_2;
        InitIVFHeader(&ivfhRaw_2);
        fread(&ivfhRaw_2, 1, sizeof(ivfhRaw_2), infile_2);

        version_2 = ivfhRaw_2.version;
        headersize_2 = ivfhRaw_2.headersize;
        fourcc_2 = ivfhRaw_2.four_cc;
        width_2 = ivfhRaw_2.width;
        height_2 = ivfhRaw_2.height;
        rate_2 = ivfhRaw_2.rate;
        scale_2 = ivfhRaw_2.scale;
        length_2 = ivfhRaw_2.length;

        signature_2[0] = ivfhRaw_2.signature[0];
        signature_2[1] = ivfhRaw_2.signature[1];
        signature_2[2] = ivfhRaw_2.signature[2];
        signature_2[3] = ivfhRaw_2.signature[3];
    }

    uint8_t               *buf_1 = NULL;
    uint8_t               *buf_2 = NULL;
    uint32_t               buf_sz_1 = 0, buf_alloc_sz_1 = 0;
    uint32_t               buf_sz_2 = 0, buf_alloc_sz_2 = 0;
    int currentVideoFrame = 0;
    int frame_avail_1 = 0;
    int frame_avail_2 = 0;

    while (!frame_avail_1 && !frame_avail_2)
    {
        frame_avail_1 = read_frame_dec(&input_1, &buf_1, (size_t *)&buf_sz_1, (size_t *)&buf_alloc_sz_1);
        frame_avail_2 = read_frame_dec(&input_2, &buf_2, (size_t *)&buf_sz_2, (size_t *)&buf_alloc_sz_2);

        if (frame_avail_1 || frame_avail_2)
        {
            if (input_1.nestegg_ctx)
                nestegg_destroy(input_1.nestegg_ctx);

            if (input_2.nestegg_ctx)
                nestegg_destroy(input_2.nestegg_ctx);

            if (input_1.kind != WEBM_FILE)
                free(buf_1);

            if (input_2.kind != WEBM_FILE)
                free(buf_2);

            fclose(infile_1);
            fclose(infile_2);

            if (frame_avail_1 == frame_avail_2)
                return -1;
            else
                return currentVideoFrame + 1;
        }

        if (memcmp(buf_1, buf_2, buf_sz_1) != 0)
        {
            if (input_1.nestegg_ctx)
                nestegg_destroy(input_1.nestegg_ctx);

            if (input_2.nestegg_ctx)
                nestegg_destroy(input_2.nestegg_ctx);

            if (input_1.kind != WEBM_FILE)
                free(buf_1);

            if (input_2.kind != WEBM_FILE)
                free(buf_2);

            fclose(infile_1);
            fclose(infile_2);
            return currentVideoFrame + 1;
        }

        currentVideoFrame++;
    }

    if (input_1.nestegg_ctx)
        nestegg_destroy(input_1.nestegg_ctx);

    if (input_2.nestegg_ctx)
        nestegg_destroy(input_2.nestegg_ctx);

    if (input_1.kind != WEBM_FILE)
        free(buf_1);

    if (input_2.kind != WEBM_FILE)
        free(buf_2);

    fclose(infile_1);
    fclose(infile_2);

    return returnval;
}
double vpxt_display_droped_frames(const char *inputchar, int PrintSwitch)
{
    int dropedframecount = 0;

    //std::string DropedInStr = inputchar;
    //DropedInStr.erase(DropedInStr.length() - 4, 4);
    std::string DropedInStr;
    vpxt_remove_file_extension(inputchar, DropedInStr);
    DropedInStr.append("aprox_droped_frames.txt");
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
                fprintf(out, "%i\n", currentVideoFrame);

            priorTimeStamp = priorTimeStamp + 2;
            dropedframecount++;
        }

        priorTimeStamp = ivf_fhRaw.timeStamp;

        currentVideoFrame ++;
    }

    if (PrintSwitch == 1)
        fclose(out);

    fclose(in);

    return dropedframecount;
}
double vpxt_display_resized_frames(const char *inputchar, int PrintSwitch)
{
    //std::string ResizeInStr = inputchar;
    //ResizeInStr.erase(ResizeInStr.length() - 4, 4);
    std::string ResizeInStr;
    vpxt_remove_file_extension(inputchar, ResizeInStr);
    ResizeInStr.append("resized_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", ResizeInStr.c_str());

    FILE *out;

    if (PrintSwitch == 1)
    {
        out = fopen(outputFile, "w");
    }

    int                     use_y4m = 1;
    vpx_codec_ctx_t       decoder;
    const char            *fn = inputchar;
    int                    i;
    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    FILE                  *infile;
    int                    frame_in = 0, frame_out = 0, flipuv = 0, noblit = 0, do_md5 = 0, progress = 0;
    int                    stop_after = 0, postproc = 0, summary = 0, quiet = 1;
    vpx_codec_iface_t       *iface = NULL;
    unsigned int           fourcc;
    unsigned long          dx_time = 0;
    struct arg               arg;
    char                   **argv, **argi, **argj;
    const char             *outfile_pattern = 0;
    char                    outfile[PATH_MAX];
    int                     single_file;
    unsigned int            width;
    unsigned int            height;
    unsigned int            fps_den;
    unsigned int            fps_num;
    vpx_codec_dec_cfg_t     cfg = {0};
#if CONFIG_VP8_DECODER
    vp8_postproc_cfg_t      vp8_pp_cfg = {0};
    int                     vp8_dbg_color_ref_frame = 0;
    int                     vp8_dbg_color_mb_modes = 0;
    int                     vp8_dbg_color_b_modes = 0;
    int                     vp8_dbg_display_mv = 0;
#endif
    struct input_ctx        input;

    input.chunk = 0;
    input.chunks = 0;
    input.infile = NULL;
    input.kind = RAW_FILE;
    input.nestegg_ctx = 0;
    input.pkt = 0;
    input.video_track = 0;

    int CharCount = 0;

    /* Open file */
    infile = strcmp(fn, "-") ? fopen(fn, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", fn);
        return -1;
    }

    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 IVF File to Raw IVF File: \n");
    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &fps_den, &fps_num))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    std::string compformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";


    //tprintf(PRINT_BTH, "\nAPI - Decompressing VP8 %s File to Raw %s File: \n", compformat.c_str(), decformat.c_str());

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

    unsigned int FrameSize = (width * height * 3) / 2;
    unsigned __int64 TimeStamp = 0;

    if (vpx_codec_dec_init(&decoder, iface ? iface :  ifaces[0].iface, &cfg, postproc ? VPX_CODEC_USE_POSTPROC : 0))
    {
        tprintf(PRINT_STD, "Failed to initialize decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    int resizedIMGCount = 0;
    int frame = 0;

    /* Decode file */
    while (!skim_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz))
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

        ++frame_in;

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

                ++frame_out;
            }
        }

        ++frame;

        if (stop_after && frame_in >= stop_after)
            break;
    }

    if (summary || progress)
    {
        show_progress(frame_in, frame_out, dx_time);
        fprintf(stderr, "\n");
    }

fail:

    if (vpx_codec_destroy(&decoder))
    {
        tprintf(PRINT_STD, "Failed to destroy decoder: %s\n", vpx_codec_error(&decoder));
        fclose(infile);

        return -1;
    }

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    if (PrintSwitch == 1)
    {
        fclose(out);
    }

    return resizedIMGCount;
}
double vpxt_display_visible_frames(const char *inputFile, int Selector)
{
    // 0 = just display
    // 1 = write to file
    //std::string VisInStr = inputFile;
    //VisInStr.erase(VisInStr.length() - 4, 4);
    std::string VisInStr;
    vpxt_remove_file_extension(inputFile, VisInStr);
    VisInStr.append("visible_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", VisInStr.c_str());

    int VisableCount = 0;
    //int currentVideoFrame = 0;

    std::ofstream outfile;

    if (Selector == 1)
    {
        outfile.open(outputFile);

        if (!outfile.good())
        {
            tprintf(PRINT_BTH, "\nERROR: Could not open output file: %s\n", outputFile);
            return 0;
        }
    }

    unsigned char  signature[4]; //='DKIF';
    unsigned short version = 0;     // -
    unsigned short headersize = 0;  // -
    unsigned int fourcc = 0;        //good
    unsigned int width = 0;         //good
    unsigned int height = 0;        //good
    unsigned int rate = 0;          //good
    unsigned int scale = 0;         //good
    unsigned int length = 0;        //other measure
    unsigned char unused[4];        // -

    signature[0] = ' ';
    signature[1] = ' ';
    signature[2] = ' ';
    signature[3] = ' ';

    std::vector<unsigned int>     frameSize;
    std::vector<unsigned __int64> timeStamp;

    FILE *infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    struct input_ctx        input;

    input.chunk = 0;

    input.chunks = 0;

    input.infile = NULL;

    input.kind = RAW_FILE;

    input.nestegg_ctx = 0;

    input.pkt = 0;

    input.video_track = 0;

    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &scale, &rate))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &scale, &rate))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    std::string compformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (input.kind == IVF_FILE)
    {
        rewind(infile);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile);

        version = ivfhRaw.version;
        headersize = ivfhRaw.headersize;
        fourcc = ivfhRaw.four_cc;
        width = ivfhRaw.width;
        height = ivfhRaw.height;
        rate = ivfhRaw.rate;
        scale = ivfhRaw.scale;
        length = ivfhRaw.length;

        signature[0] = ivfhRaw.signature[0];
        signature[1] = ivfhRaw.signature[1];
        signature[2] = ivfhRaw.signature[2];
        signature[3] = ivfhRaw.signature[3];
    }

    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    int currentVideoFrame = 0;
    int frame_avail = 0;

    while (!frame_avail)
    {
        frame_avail = read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz);

        if (frame_avail)
            break;

        //check to see if visible frame
        VP8_HEADER oz;

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
        memcpy(&oz, buf, 3); // copy 3 bytes;
#endif
        unsigned int type = oz.type;
        unsigned int showFrame = oz.show_frame;
        unsigned int version = oz.version;

        if (showFrame == 1)
        {
            VisableCount++;

            if (Selector == 0)
                tprintf(PRINT_STD, "\n%i\n", currentVideoFrame);

            if (Selector == 1)
                outfile << currentVideoFrame << "\n";
        }

        currentVideoFrame ++;
    }


fail:

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    if (Selector == 1)
        outfile.close();

    return VisableCount;

}
double vpxt_display_alt_ref_frames(const char *inputFile, int Selector)
{
    // 0 = just display
    // 1 = write to file

    //std::string AltRefInStr = inputFile;
    //AltRefInStr.erase(AltRefInStr.length() - 4, 4);
    std::string AltRefInStr;
    vpxt_remove_file_extension(inputFile, AltRefInStr);
    AltRefInStr.append("alt_ref_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", AltRefInStr.c_str());

    int AltRefCount = 0;

    std::ofstream outfile;

    if (Selector == 1)
    {
        outfile.open(outputFile);

        if (!outfile.good())
        {
            tprintf(PRINT_BTH, "\nERROR: Could not open output file: %s\n", outputFile);
            return 0;
        }
    }

    unsigned char  signature[4]; //='DKIF';
    unsigned short version = 0;     // -
    unsigned short headersize = 0;  // -
    unsigned int fourcc = 0;        //good
    unsigned int width = 0;         //good
    unsigned int height = 0;        //good
    unsigned int rate = 0;          //good
    unsigned int scale = 0;         //good
    unsigned int length = 0;        //other measure
    unsigned char unused[4];        // -

    signature[0] = ' ';
    signature[1] = ' ';
    signature[2] = ' ';
    signature[3] = ' ';

    std::vector<unsigned int>     frameSize;
    std::vector<unsigned __int64> timeStamp;

    FILE *infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    struct input_ctx        input;

    input.chunk = 0;

    input.chunks = 0;

    input.infile = NULL;

    input.kind = RAW_FILE;

    input.nestegg_ctx = 0;

    input.pkt = 0;

    input.video_track = 0;

    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &scale, &rate))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &scale, &rate))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    std::string compformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (input.kind == IVF_FILE)
    {
        rewind(infile);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile);

        version = ivfhRaw.version;
        headersize = ivfhRaw.headersize;
        fourcc = ivfhRaw.four_cc;
        width = ivfhRaw.width;
        height = ivfhRaw.height;
        rate = ivfhRaw.rate;
        scale = ivfhRaw.scale;
        length = ivfhRaw.length;

        signature[0] = ivfhRaw.signature[0];
        signature[1] = ivfhRaw.signature[1];
        signature[2] = ivfhRaw.signature[2];
        signature[3] = ivfhRaw.signature[3];
    }

    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    int currentVideoFrame = 0;
    int frame_avail = 0;

    while (!frame_avail)
    {
        frame_avail = read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz);

        if (frame_avail)
            break;

        //check to see if visible frame
        VP8_HEADER oz;

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
        memcpy(&oz, buf, 3); // copy 3 bytes;
#endif
        unsigned int type = oz.type;
        unsigned int showFrame = oz.show_frame;
        unsigned int version = oz.version;

        if (showFrame == 0)
        {
            AltRefCount++;

            if (Selector == 0)
                tprintf(PRINT_STD, "\n%i\n", currentVideoFrame);

            if (Selector == 1)
                outfile << currentVideoFrame << "\n";
        }

        currentVideoFrame ++;
    }


fail:

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    if (Selector == 1)
        outfile.close();

    return AltRefCount;

}
double vpxt_display_key_frames(const char *inputFile, int Selector)
{
    int keyframecount = 0;

    //std::string KeyInStr = inputFile;
    //KeyInStr.erase(KeyInStr.length() - 4, 4);
    std::string KeyInStr;
    vpxt_remove_file_extension(inputFile, KeyInStr);
    KeyInStr.append("key_frames.txt");
    char outputFile[255] = "";
    snprintf(outputFile, 255, "%s", KeyInStr.c_str());

    std::ofstream outfile;

    if (Selector == 1)
    {
        outfile.open(outputFile);

        if (!outfile.good())
        {
            tprintf(PRINT_BTH, "\nERROR: Could not open output file: %s\n", outputFile);
            return 0;
        }
    }

    unsigned char  signature[4]; //='DKIF';
    unsigned short version = 0;     // -
    unsigned short headersize = 0;  // -
    unsigned int fourcc = 0;        //good
    unsigned int width = 0;         //good
    unsigned int height = 0;        //good
    unsigned int rate = 0;          //good
    unsigned int scale = 0;         //good
    unsigned int length = 0;        //other measure
    unsigned char unused[4];        // -

    signature[0] = ' ';
    signature[1] = ' ';
    signature[2] = ' ';
    signature[3] = ' ';

    FILE *infile = strcmp(inputFile, "-") ? fopen(inputFile, "rb") : set_binary_mode(stdin);

    if (!infile)
    {
        tprintf(PRINT_BTH, "Failed to open input file: %s", inputFile);
        return -1;
    }

    struct input_ctx        input;

    input.chunk = 0;

    input.chunks = 0;

    input.infile = NULL;

    input.kind = RAW_FILE;

    input.nestegg_ctx = 0;

    input.pkt = 0;

    input.video_track = 0;

    input.infile = infile;

    if (file_is_ivf_dec(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = IVF_FILE;
    else if (file_is_webm(&input, &fourcc, &width, &height, &scale, &rate))
        input.kind = WEBM_FILE;
    else if (file_is_raw(infile, &fourcc, &width, &height, &scale, &rate))
        input.kind = RAW_FILE;
    else
    {
        fprintf(stderr, "Unrecognized input file type.\n");
        return EXIT_FAILURE;
    }

    if (input.kind == WEBM_FILE)
        if (webm_guess_framerate(&input, &scale, &rate))
        {
            fprintf(stderr, "Failed to guess framerate -- error parsing "
                    "webm file?\n");
            return EXIT_FAILURE;
        }

    std::string compformat;

    if (input.kind == IVF_FILE)
        compformat = "IVF";

    if (input.kind == WEBM_FILE)
        compformat = "Webm";

    if (input.kind == RAW_FILE)
        compformat = "Raw";

    if (input.kind == IVF_FILE)
    {
        rewind(infile);
        IVF_HEADER ivfhRaw;
        InitIVFHeader(&ivfhRaw);
        fread(&ivfhRaw, 1, sizeof(ivfhRaw), infile);

        version = ivfhRaw.version;
        headersize = ivfhRaw.headersize;
        fourcc = ivfhRaw.four_cc;
        width = ivfhRaw.width;
        height = ivfhRaw.height;
        rate = ivfhRaw.rate;
        scale = ivfhRaw.scale;
        length = ivfhRaw.length;

        signature[0] = ivfhRaw.signature[0];
        signature[1] = ivfhRaw.signature[1];
        signature[2] = ivfhRaw.signature[2];
        signature[3] = ivfhRaw.signature[3];
    }

    uint8_t               *buf = NULL;
    size_t               buf_sz = 0, buf_alloc_sz = 0;
    int currentVideoFrame = 0;
    int frame_avail = 0;

    while (!frame_avail)
    {
        frame_avail = read_frame_dec(&input, &buf, (size_t *)&buf_sz, (size_t *)&buf_alloc_sz);

        if (frame_avail)
            break;

        //check to see if visible frame
        VP8_HEADER oz;

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
        memcpy(&oz, buf, 3); // copy 3 bytes;
#endif
        unsigned int type = oz.type;
        unsigned int showFrame = oz.show_frame;
        unsigned int version = oz.version;

        if (type == 0)
        {
            keyframecount++;

            if (Selector == 0)
                tprintf(PRINT_STD, "\n%i\n", currentVideoFrame);

            if (Selector == 1)
                outfile << currentVideoFrame << "\n";
        }

        currentVideoFrame ++;
    }

fail:

    if (input.nestegg_ctx)
        nestegg_destroy(input.nestegg_ctx);

    if (input.kind != WEBM_FILE)
        free(buf);

    fclose(infile);

    if (Selector == 1)
        outfile.close();

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

    //std::string CheckPBMThreshStr = InputFile;
    //CheckPBMThreshStr.erase(CheckPBMThreshStr.length() - 4, 4);
    std::string CheckPBMThreshStr;
    vpxt_remove_file_extension(InputFile, CheckPBMThreshStr);
    CheckPBMThreshStr.append("CheckPBMThresh.txt");
    char CheckPBMThreshChar[255] = "";
    snprintf(CheckPBMThreshChar, 255, "%s", CheckPBMThreshStr.c_str());

    //std::string ResizeFramesStr = InputFile;
    //ResizeFramesStr.erase(ResizeFramesStr.length() - 4, 4);
    std::string ResizeFramesStr;
    vpxt_remove_file_extension(InputFile, ResizeFramesStr);
    ResizeFramesStr.append("resized_frames.txt");
    char ResizeFramesChar[255] = "";
    snprintf(ResizeFramesChar, 255, "%s", ResizeFramesStr.c_str());

    //std::string KeyFramesStr = InputFile;
    //KeyFramesStr.erase(KeyFramesStr.length() - 4, 4);
    std::string KeyFramesStr;
    vpxt_remove_file_extension(InputFile, KeyFramesStr);
    KeyFramesStr.append("key_frames.txt");
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

    //std::string QuantInStr = inputFile;
    //QuantInStr.erase(QuantInStr.length() - 4, 4);
    std::string QuantInStr;
    vpxt_remove_file_extension(inputFile, QuantInStr);
    QuantInStr.append("quantizers.txt");
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

    //std::string QuantInStr = inputFile;
    //QuantInStr.erase(QuantInStr.length() - 4, 4);
    std::string QuantInStr;
    vpxt_remove_file_extension(inputFile, QuantInStr);
    QuantInStr.append("quantizers.txt");
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

    //std::string QuantInStr = inputFile;
    //QuantInStr.erase(QuantInStr.length() - 4, 4);
    std::string QuantInStr;
    vpxt_remove_file_extension(inputFile, QuantInStr);
    QuantInStr.append("quantizers.txt");
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

    //char TextFilechar1[255];
    std::string FullName;

    vpxt_remove_file_extension(infile, FullName);

    //char *FullName = "";

    if (FileType == 0)
    {
        //FullName = strcat(TextFilechar1, "compression_time.txt");
        FullName.append("compression_time.txt");
    }

    if (FileType == 1)
    {
        //FullName = strcat(TextFilechar1, "decompression_time.txt");
        FullName.append("decompression_time.txt");
    }

    std::ifstream infile2(FullName.c_str());

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

    //char TextFilechar1[255];
    std::string FullName;

    vpxt_remove_file_extension(infile, FullName);

    //char *FullName = "";

    if (FileType == 0)
    {
        //FullName = strcat(TextFilechar1, "compression_cpu_tick.txt");
        FullName.append("compression_cpu_tick.txt");
    }

    if (FileType == 1)
    {
        //FullName = strcat(TextFilechar1, "decompression_cpu_tick.txt");
        FullName.append("decompression_cpu_tick.txt");
    }

    std::ifstream infile2(FullName.c_str());

    if (!infile2.is_open())
    {
        tprintf(PRINT_BTH, "File: %s not opened", FullName);
        return 0;
    }

    infile2 >> speed;
    infile2.close();

    return speed;
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
int vpxt_print_compare_ivf_results(int lngRC, int printErr)
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
    //std::string TimeTextFileStr = compFileName;
    //TimeTextFileStr.erase(TimeTextFileStr.length() - 4, 4);
    std::string TimeTextFileStr;
    vpxt_remove_file_extension(compFileName, TimeTextFileStr);
    TimeTextFileStr.append("psnr.txt");

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
