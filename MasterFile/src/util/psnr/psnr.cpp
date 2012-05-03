#include "yv12config.h"
#include "vpxt_utilities.h"
#include <cmath>

#define MAX_PSNR 60


double vp8_mse_2_psnr_tester(double samples, double peak, double mse)
{
    double psnr;

    if (mse > 0.0)
        psnr = 10.0 * log10(peak * peak * samples / mse);
    else
        psnr = MAX_PSNR;      // Limit to prevent / 0

    if (psnr > MAX_PSNR)
        psnr = MAX_PSNR;

    return psnr;
}

double vp8_calcpsnr_tester(YV12_BUFFER_CONFIG *source, YV12_BUFFER_CONFIG *dest,
                           double *ypsnr, double *upsnr, double *vpsnr,
                           double *sq_error, int print_out,
                           int& possible_artifact)
{
    int i, j;
    int diff;
    double frame_psnr;
    double total;
    double grand_total;
    unsigned char *src = source->y_buffer;
    unsigned char *dst = dest->y_buffer;

    double sub_frame_ypsnr[16][16] = {0}; // break the frame into 16 by 16
    double sub_frame_total[16][16] = {0}; // hold 16 by 16 frame total data

    // try to keep at least 64 pixel segments
    int width_segments = source->y_width / 64;
    int height_segments = width_segments;

    if(height_segments > 16)
        height_segments = 16;
    if(width_segments > 16)
        width_segments = 16;

    total = 0.0;
    grand_total = 0.0;

    // Loop throught the Y plane raw and reconstruction data summing
    // (square differences)
    for (i = 0; i < source->y_height; i++)
    {
        for (j = 0; j < source->y_width; j++)
        {
            diff        = (int)(src[j]) - (int)(dst[j]);
            total      += diff * diff;

            // gather totals for internal segments
            if(possible_artifact == kRunArtifactDetection)
                sub_frame_total[i / ((source->y_height / height_segments == 0)
                ? 1 : ((height_segments - 1) + source->y_height) /
                height_segments)] [j / ((source->y_width/width_segments ==0 )
                ? 1 : ((width_segments - 1 ) + source->y_width) /
                width_segments)] += diff * diff;
        }

        src += source->y_stride;
        dst += dest->y_stride;
    }

    // Work out Y PSNR
    *ypsnr = vp8_mse_2_psnr_tester(source->y_height * source->y_width, 255.0,
        total);

    double max_psnr_1 = 0;
    double max_psnr_2 = 0;
    double max_psnr_3 = 0;
    double min_psnr = 61;

    if(possible_artifact == kRunArtifactDetection)
    {
        // Work out Y PSNRs for internal segments and find min and max
        for(i=0; i < height_segments; i++){
            for(j=0; j < width_segments; j++){

                int sub_frame_height = 1;
                if(i == (height_segments - 1))
                    sub_frame_height = source->y_height - ((height_segments-1)
                    * (((height_segments - 1) + source->y_height)
                    / height_segments));
                else
                    sub_frame_height = ((height_segments - 1) + source->y_height
                    ) / height_segments;

                int sub_frame_width = 1;
                if(j == (width_segments - 1))
                    sub_frame_width = source->y_width - ((width_segments - 1) *
                    (((width_segments - 1) + source->y_width) /
                    width_segments));
                else
                    sub_frame_width = (15 + source->y_width) / width_segments;

                sub_frame_ypsnr[i][j] = vp8_mse_2_psnr_tester(sub_frame_height *
                    sub_frame_width, 255.0, sub_frame_total[i][j]);

                // Get min and top three max sub psnrs
                if(sub_frame_ypsnr[i][j] != 60 && sub_frame_ypsnr[i][j] >
                    max_psnr_1){
                    max_psnr_2 = max_psnr_1;
                    max_psnr_3 = max_psnr_2;
                    max_psnr_1 = sub_frame_ypsnr[i][j];
                }
                else if(sub_frame_ypsnr[i][j] != 60 && sub_frame_ypsnr[i][j] >
                    max_psnr_2){
                    max_psnr_3 = max_psnr_2;
                    max_psnr_2 = sub_frame_ypsnr[i][j];
                }
                else if(sub_frame_ypsnr[i][j] != 60 && sub_frame_ypsnr[i][j] >
                    max_psnr_3){
                    max_psnr_3 = sub_frame_ypsnr[i][j];
                }
                if(sub_frame_ypsnr[i][j] < min_psnr)
                    min_psnr = sub_frame_ypsnr[i][j];
            }
        }

        // if min sub psnr is not within ~57% of top three psnr
        // average then flag as potential artifact.
        if((max_psnr_1 + max_psnr_2 + max_psnr_3) / 7 >=  min_psnr)
            possible_artifact = kPossibleArtifactFound;
        else
            possible_artifact = kNoArtifactFound;

        if(possible_artifact == kPossibleArtifactFound && print_out)
        {
            tprintf(print_out, "min: %.0f Max: %.0f %.0f %.0f", min_psnr,
                max_psnr_1, max_psnr_2, max_psnr_3);

            for(i=0; i < height_segments; i++){
                tprintf(print_out, "\n");
                for(int z = 0; z < (width_segments*3)+1; z++){
                    tprintf(print_out, "-");
                }

                tprintf(print_out, "\n|");
                for(j=0; j < width_segments; j++){
                    tprintf(print_out, "%.0f|",sub_frame_ypsnr[i][j]);
                }
            }
            tprintf(print_out, "\n");
            for(int z = 0; z < (width_segments*3)+1; z++){
                tprintf(print_out, "-");
            }
            tprintf(print_out, "\n");
        }
    }

    grand_total += total;
    total = 0;

    // Loop through the U plane
    src = source->u_buffer;
    dst = dest->u_buffer;

    for (i = 0; i < source->uv_height; i++)
    {
        for (j = 0; j < source->uv_width; j++)
        {
            diff        = (int)(src[j]) - (int)(dst[j]);
            total      += diff * diff;
        }

        src += source->uv_stride;
        dst += dest->uv_stride;
    }

    // Work out U PSNR
    *upsnr = vp8_mse_2_psnr_tester(source->uv_height * source->uv_width, 255.0,
        total);
    grand_total += total;
    total = 0;

    // V PSNR
    src = source->v_buffer;
    dst = dest->v_buffer;

    for (i = 0; i < source->uv_height; i++)
    {
        for (j = 0; j < source->uv_width; j++)
        {
            diff        = (int)(src[j]) - (int)(dst[j]);
            total      += diff * diff;
        }

        src += source->uv_stride;
        dst += dest->uv_stride;
    }

    // Work out UV PSNR
    *vpsnr = vp8_mse_2_psnr_tester(source->uv_height * source->uv_width, 255.0,
        total);
    grand_total += total;
    total = 0;

    // Work out total PSNR
    frame_psnr = vp8_mse_2_psnr_tester(source->y_height * source->y_width *
        3 / 2 , 255.0, grand_total);

    *sq_error = 1.0 * grand_total;

    return frame_psnr;
}
