/****************************************************************************
 *
 *   Module Title : ssim.c
 *
 *   Description  : functions for calculating structural similarity2 between
 *                  two frames
 *
 *   Copyright (c) 1999 - 2005  On2 Technologies Inc. All Rights Reserved.
 *
 ***************************************************************************/
#include "yv12config.h"

#define C1 (float)(64 * 64 * 0.01*255*0.01*255)
#define C2 (float)(64 * 64 * 0.03*255*0.03*255)

static int widthY;
static int heightY;
static int heightUV;
static int widthUV;
static int strideUV;
static int nbFrames;
static int stride;
static const char *fname;
static const char *fname2;
static int lumimask;
static int luminance;
static int scaled;
static double planeSummedWeights = 0;

#ifdef ARM
static short img12_sum_block[8*864*864] ;

static short img1_sum[16*864];
static short img2_sum[16*864];
static int   img1_sq_sum[16*864];
static int   img2_sq_sum[16*864];
static int   img12_mul_sum[16*864];
#else
static short img12_sum_block[8*2048*2048] ;

static short img1_sum[16*2048];
static short img2_sum[16*2048];
static int   img1_sq_sum[16*2048];
static int   img2_sq_sum[16*2048];
static int   img12_mul_sum[16*2048];

#endif


double similarity2
(
    int muX,
    int muY,
    int preMuX2,
    int preMuY2,
    int preMuXY2
)
{
    int muX2, muY2, muXY, thetaX2, thetaY2, thetaXY;

    muX2 = muX * muX;
    muY2 = muY * muY;
    muXY = muX * muY;

    thetaX2 = 64 * preMuX2 - muX2;
    thetaY2 = 64 * preMuY2 - muY2;
    thetaXY = 64 * preMuXY2 - muXY;

    return (2 * muXY + C1) * (2 * thetaXY + C2) / ((muX2 + muY2 + C1) * (thetaX2 + thetaY2 + C2));
}

double ssim2
(
    const unsigned char *img1,
    const unsigned char *img2,
    int stride_img1,
    int stride_img2,
    int width,
    int height
)
{
    int x, y, x2, y2, img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block, temp;

    double planeQuality, weight, mean;

    short *img1_sum_ptr1, *img1_sum_ptr2;
    short *img2_sum_ptr1, *img2_sum_ptr2;
    int *img1_sq_sum_ptr1, *img1_sq_sum_ptr2;
    int *img2_sq_sum_ptr1, *img2_sq_sum_ptr2;
    int *img12_mul_sum_ptr1, *img12_mul_sum_ptr2;

    planeQuality = 0;

    if (lumimask)
        planeSummedWeights = 0.0f;
    else
        planeSummedWeights = (height - 7) * (width - 7);

    //some prologue for the main loop
    temp = 8 * width;

    img1_sum_ptr1      = img1_sum + temp;
    img2_sum_ptr1      = img2_sum + temp;
    img1_sq_sum_ptr1   = img1_sq_sum + temp;
    img2_sq_sum_ptr1   = img2_sq_sum + temp;
    img12_mul_sum_ptr1 = img12_mul_sum + temp;

    for (x = 0; x < width; x++)
    {
        img1_sum[x]      = img1[x];
        img2_sum[x]      = img2[x];
        img1_sq_sum[x]   = img1[x] * img1[x];
        img2_sq_sum[x]   = img2[x] * img2[x];
        img12_mul_sum[x] = img1[x] * img2[x];

        img1_sum_ptr1[x]      = 0;
        img2_sum_ptr1[x]      = 0;
        img1_sq_sum_ptr1[x]   = 0;
        img2_sq_sum_ptr1[x]   = 0;
        img12_mul_sum_ptr1[x] = 0;
    }

    //the main loop
    for (y = 1; y < height; y++)
    {
        img1 += stride_img1;
        img2 += stride_img2;

        temp = (y - 1) % 9 * width;

        img1_sum_ptr1      = img1_sum + temp;
        img2_sum_ptr1      = img2_sum + temp;
        img1_sq_sum_ptr1   = img1_sq_sum + temp;
        img2_sq_sum_ptr1   = img2_sq_sum + temp;
        img12_mul_sum_ptr1 = img12_mul_sum + temp;

        temp = y % 9 * width;

        img1_sum_ptr2      = img1_sum + temp;
        img2_sum_ptr2      = img2_sum + temp;
        img1_sq_sum_ptr2   = img1_sq_sum + temp;
        img2_sq_sum_ptr2   = img2_sq_sum + temp;
        img12_mul_sum_ptr2 = img12_mul_sum + temp;

        for (x = 0; x < width; x++)
        {
            img1_sum_ptr2[x]      = img1_sum_ptr1[x] + img1[x];
            img2_sum_ptr2[x]      = img2_sum_ptr1[x] + img2[x];
            img1_sq_sum_ptr2[x]   = img1_sq_sum_ptr1[x] + img1[x] * img1[x];
            img2_sq_sum_ptr2[x]   = img2_sq_sum_ptr1[x] + img2[x] * img2[x];
            img12_mul_sum_ptr2[x] = img12_mul_sum_ptr1[x] + img1[x] * img2[x];
        }

        if (y > 6)
        {
            //calculate the sum of the last 8 lines by subtracting the total sum of 8 lines back from the present sum
            temp = (y + 1) % 9 * width;

            img1_sum_ptr1      = img1_sum + temp;
            img2_sum_ptr1      = img2_sum + temp;
            img1_sq_sum_ptr1   = img1_sq_sum + temp;
            img2_sq_sum_ptr1   = img2_sq_sum + temp;
            img12_mul_sum_ptr1 = img12_mul_sum + temp;

            for (x = 0; x < width; x++)
            {
                img1_sum_ptr1[x]      = img1_sum_ptr2[x] - img1_sum_ptr1[x];
                img2_sum_ptr1[x]      = img2_sum_ptr2[x] - img2_sum_ptr1[x];
                img1_sq_sum_ptr1[x]   = img1_sq_sum_ptr2[x] - img1_sq_sum_ptr1[x];
                img2_sq_sum_ptr1[x]   = img2_sq_sum_ptr2[x] - img2_sq_sum_ptr1[x];
                img12_mul_sum_ptr1[x] = img12_mul_sum_ptr2[x] - img12_mul_sum_ptr1[x];
            }

            //here we calculate the sum over the 8x8 block of pixels
            //this is done by sliding a window across the column sums for the last 8 lines
            //each time adding the new column sum, and subtracting the one which fell out of the window
            img1_block      = 0;
            img2_block      = 0;
            img1_sq_block   = 0;
            img2_sq_block   = 0;
            img12_mul_block = 0;

            //prologue, and calculation of simularity measure from the first 8 column sums
            for (x = 0; x < 8; x++)
            {
                img1_block      += img1_sum_ptr1[x];
                img2_block      += img2_sum_ptr1[x];
                img1_sq_block   += img1_sq_sum_ptr1[x];
                img2_sq_block   += img2_sq_sum_ptr1[x];
                img12_mul_block += img12_mul_sum_ptr1[x];
            }

            if (lumimask)
            {
                y2 = y - 7;
                x2 = 0;

                if (luminance)
                {
                    mean = (img2_block + img1_block) / 128.0f;

                    if (!(y2 % 2 || x2 % 2))
                        *(img12_sum_block + y2 / 2 * widthUV + x2 / 2) = img2_block + img1_block;
                }
                else
                {
                    mean = *(img12_sum_block + y2 * widthUV + x2);
                    mean += *(img12_sum_block + y2 * widthUV + x2 + 4);
                    mean += *(img12_sum_block + (y2 + 4) * widthUV + x2);
                    mean += *(img12_sum_block + (y2 + 4) * widthUV + x2 + 4);

                    mean /= 512.0f;
                }

                weight = mean < 40 ? 0.0f :
                         (mean < 50 ? (mean - 40.0f) / 10.0f : 1.0f);
                planeSummedWeights += weight;

                planeQuality += weight * similarity2(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);
            }
            else
                planeQuality += similarity2(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);

            //and for the rest
            for (x = 8; x < width; x++)
            {
                img1_block      = img1_block + img1_sum_ptr1[x] - img1_sum_ptr1[x - 8];
                img2_block      = img2_block + img2_sum_ptr1[x] - img2_sum_ptr1[x - 8];
                img1_sq_block   = img1_sq_block + img1_sq_sum_ptr1[x] - img1_sq_sum_ptr1[x - 8];
                img2_sq_block   = img2_sq_block + img2_sq_sum_ptr1[x] - img2_sq_sum_ptr1[x - 8];
                img12_mul_block = img12_mul_block + img12_mul_sum_ptr1[x] - img12_mul_sum_ptr1[x - 8];

                if (lumimask)
                {
                    y2 = y - 7;
                    x2 = x - 7;

                    if (luminance)
                    {
                        mean = (img2_block + img1_block) / 128.0f;

                        if (!(y2 % 2 || x2 % 2))
                            *(img12_sum_block + y2 / 2 * widthUV + x2 / 2) = img2_block + img1_block;
                    }
                    else
                    {
                        mean = *(img12_sum_block + y2 * widthUV + x2);
                        mean += *(img12_sum_block + y2 * widthUV + x2 + 4);
                        mean += *(img12_sum_block + (y2 + 4) * widthUV + x2);
                        mean += *(img12_sum_block + (y2 + 4) * widthUV + x2 + 4);

                        mean /= 512.0f;
                    }

                    weight = mean < 40 ? 0.0f :
                             (mean < 50 ? (mean - 40.0f) / 10.0f : 1.0f);
                    planeSummedWeights += weight;

                    planeQuality += weight * similarity2(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);
                }
                else
                    planeQuality += similarity2(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);
            }
        }
    }

    if (planeSummedWeights == 0)
        return 1.0f;
    else
        return planeQuality / planeSummedWeights;
}

double VP8_CalcSSIM2(YV12_BUFFER_CONFIG *source, YV12_BUFFER_CONFIG *dest, int lumamask, double *weight)
{
    double a, b, c;
    double frameWeight;
    double ssimv = 0;

    widthY = source->YWidth;
    heightY = source->YHeight;
    heightUV = source->UVHeight;
    widthUV = source->UVWidth;
    strideUV = dest->UVStride;
    stride = dest->YStride;

    lumimask = lumamask;

    luminance = 1;
    a = ssim2(source->YBuffer, dest->YBuffer,
              source->YStride, dest->YStride, source->YWidth, source->YHeight);
    luminance = 0;

    frameWeight = planeSummedWeights / ((widthY - 7) * (heightY - 7));

    if (frameWeight == 0)
        a = b = c = 1.0f;
    else
    {
        b = ssim2(source->UBuffer, dest->UBuffer,
                  source->UVStride, dest->UVStride, source->UVWidth, source->UVHeight);

        c = ssim2(source->VBuffer, dest->VBuffer,
                  source->UVStride, dest->UVStride, source->UVWidth, source->UVHeight);
    }

    ssimv = a * .8 + .1 * (b + c);

    *weight = frameWeight;

    return ssimv;
}