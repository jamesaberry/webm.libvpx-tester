#include "yv12config.h"
#include <math.h>

#define MAX_PSNR 60


double VP8_Mse2Psnr(double Samples, double Peak, double Mse)
{
    double psnr;

    if ((double)Mse > 0.0)
        psnr = 10.0 * log10(Peak * Peak * Samples / Mse);
    else
        psnr = MAX_PSNR;      // Limit to prevent / 0

    if (psnr > MAX_PSNR)
        psnr = MAX_PSNR;

    return psnr;
}

double VP8_CalcPSNR(YV12_BUFFER_CONFIG *source, YV12_BUFFER_CONFIG *dest, double *YPsnr, double *UPsnr, double *VPsnr, double *SqError)
{
    int i, j;
    int Diff;
    double FramePsnr;
    double Total;
    double GrandTotal;
    unsigned char *src = source->YBuffer;
    unsigned char *dst = dest->YBuffer;

    Total = 0.0;
    GrandTotal = 0.0;

    // Loop throught the Y plane raw and reconstruction data summing (square differences)
    for (i = 0; i < source->YHeight; i++)
    {

        for (j = 0; j < source->YWidth; j++)
        {
            Diff        = (int)(src[j]) - (int)(dst[j]);
            Total      += Diff * Diff;
        }

        src += source->YStride;
        dst += dest->YStride;
    }

    // Work out Y PSNR
    *YPsnr = VP8_Mse2Psnr(source->YHeight * source->YWidth, 255.0, Total);
    GrandTotal += Total;
    Total = 0;


    // Loop through the U plane
    src = source->UBuffer;
    dst = dest->UBuffer;

    for (i = 0; i < source->UVHeight; i++)
    {

        for (j = 0; j < source->UVWidth; j++)
        {
            Diff        = (int)(src[j]) - (int)(dst[j]);
            Total      += Diff * Diff;
        }

        src += source->UVStride;
        dst += dest->UVStride;
    }

    // Work out U PSNR
    *UPsnr = VP8_Mse2Psnr(source->UVHeight * source->UVWidth, 255.0, Total);
    GrandTotal += Total;
    Total = 0;


    // V PSNR
    src = source->VBuffer;
    dst = dest->VBuffer;

    for (i = 0; i < source->UVHeight; i++)
    {

        for (j = 0; j < source->UVWidth; j++)
        {
            Diff        = (int)(src[j]) - (int)(dst[j]);
            Total      += Diff * Diff;
        }

        src += source->UVStride;
        dst += dest->UVStride;
    }

    // Work out UV PSNR
    *VPsnr = VP8_Mse2Psnr(source->UVHeight * source->UVWidth, 255.0, Total);
    GrandTotal += Total;
    Total = 0;

    // Work out total PSNR
    FramePsnr = VP8_Mse2Psnr(source->YHeight * source->YWidth * 3 / 2 , 255.0, GrandTotal);

    *SqError = 1.0 * GrandTotal;

    return FramePsnr;
}
