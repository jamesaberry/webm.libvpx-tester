#include "yv12config.h"
#include <cmath>

#define MAX_PSNR 60


double VP8_Mse2Psnr_Tester(double Samples, double Peak, double Mse)
{
    double psnr;

    if (Mse > 0.0)
        psnr = 10.0 * log10(Peak * Peak * Samples / Mse);
    else
        psnr = MAX_PSNR;      // Limit to prevent / 0

    if (psnr > MAX_PSNR)
        psnr = MAX_PSNR;

    return psnr;
}

double VP8_CalcPSNR_Tester(YV12_BUFFER_CONFIG *source, YV12_BUFFER_CONFIG *dest, double *YPsnr, double *UPsnr, double *VPsnr, double *SqError)
{
    int i, j;
    int Diff;
    double FramePsnr;
    double Total;
    double GrandTotal;
    unsigned char *src = source->y_buffer;
    unsigned char *dst = dest->y_buffer;

    Total = 0.0;
    GrandTotal = 0.0;

    // Loop throught the Y plane raw and reconstruction data summing
    // (square differences)
    for (i = 0; i < source->y_height; i++)
    {

        for (j = 0; j < source->y_width; j++)
        {
            Diff        = (int)(src[j]) - (int)(dst[j]);
            Total      += Diff * Diff;
        }

        src += source->y_stride;
        dst += dest->y_stride;
    }

    // Work out Y PSNR
    *YPsnr = VP8_Mse2Psnr_Tester(source->y_height * source->y_width, 255.0, Total);
    GrandTotal += Total;
    Total = 0;


    // Loop through the U plane
    src = source->u_buffer;
    dst = dest->u_buffer;

    for (i = 0; i < source->uv_height; i++)
    {

        for (j = 0; j < source->uv_width; j++)
        {
            Diff        = (int)(src[j]) - (int)(dst[j]);
            Total      += Diff * Diff;
        }

        src += source->uv_stride;
        dst += dest->uv_stride;
    }

    // Work out U PSNR
    *UPsnr = VP8_Mse2Psnr_Tester(source->uv_height * source->uv_width, 255.0, Total);
    GrandTotal += Total;
    Total = 0;


    // V PSNR
    src = source->v_buffer;
    dst = dest->v_buffer;

    for (i = 0; i < source->uv_height; i++)
    {

        for (j = 0; j < source->uv_width; j++)
        {
            Diff        = (int)(src[j]) - (int)(dst[j]);
            Total      += Diff * Diff;
        }

        src += source->uv_stride;
        dst += dest->uv_stride;
    }

    // Work out UV PSNR
    *VPsnr = VP8_Mse2Psnr_Tester(source->uv_height * source->uv_width, 255.0, Total);
    GrandTotal += Total;
    Total = 0;

    // Work out total PSNR
    FramePsnr = VP8_Mse2Psnr_Tester(source->y_height * source->y_width * 3 / 2 , 255.0, GrandTotal);

    *SqError = 1.0 * GrandTotal;

    return FramePsnr;
}
