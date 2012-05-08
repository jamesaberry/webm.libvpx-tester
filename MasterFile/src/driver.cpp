#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_test_definitions.h"
#include "vpxt_test_declarations.h"
#include "vpxt_test_list.h"
#include "vpxt_utilities.h"
#include "vpxt_driver.h"
#include "vpxt_tools.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>

#if defined(_WIN32)
#define snprintf _snprintf
#else
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#endif

int create_working_folder(int argc, const char** argv, char *working_dir_char)
{
    ///////////////////////////////// Create Working folder ////////////////////

    // Get Date and time info and convert it to a string removing colons in time
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    std::string date_and_time = asctime(time_info);

    // remove colons in time string
    char date_and_time_char_arr[255];
    int w = 0;

    replace_substring( ":", "_", &date_and_time);
    replace_substring( " ", "_", &date_and_time);
    date_and_time.erase(date_and_time.length() - 1, date_and_time.length());

    std::string folder;
    vpxt_folder_name(argv[0], &folder);

    // add Date and time
    folder += date_and_time;
    std::string folder_check = folder;

    // Make sure folder doesnt already exist
    int number = 0;
    while (vpxt_folder_exist_check(folder_check))
    {
        number = number + 1;
        char num_char[255];
        vpxt_itoa_custom(number, num_char, 10);

        folder_check = folder + "_" + num_char;
    }

    // append sub number to end of folder name
    if (number != 0)
    {
        char num_char[255];
        vpxt_itoa_custom(number + 1, num_char, 10);

        folder += "_";
        folder += num_char;
    }

    folder += "\"";
    snprintf(working_dir_char, 255, "%s", folder.c_str());

    return 0;
}

std::string date_string()
{
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    char *date_and_time = asctime(time_info);

    // remove colons in time string
    char date_and_time_char_arr[255];
    int w = 0;

    while (date_and_time[w] != '\n')
    {
        if (date_and_time[w] == ':' || date_and_time[w] == ' ')
            date_and_time_char_arr[w] = '_';
        else
            date_and_time_char_arr[w] = date_and_time[w];

        w = w + 1;
    }

    date_and_time_char_arr[w] = '\"';
    date_and_time_char_arr[w+1] = '\0';

    return date_and_time_char_arr;
}

void vpxt_on_error_output()
{
    tprintf(PRINT_STD, "\n");
    tprintf(PRINT_STD, "Usage: Tester.exe <options> \n");
    tprintf(PRINT_STD, "\n");
    tprintf(PRINT_STD, "  <test options>                           "
        "<tool options>\n");
    tprintf(PRINT_STD, "                                            "
        "\n");
    tprintf(PRINT_STD, "   (0) run_multiple_tests                   "
        "VPXEnc\n");
    tprintf(PRINT_STD, "   (1) test_allow_drop_frames               "
        "VPXDec\n");
    tprintf(PRINT_STD, "   (2) test_allow_lag                       "
        "VPXTEnc\n");
    tprintf(PRINT_STD, "   (3) test_allow_spatial_resampling        "
        "VPXTDec\n");
    tprintf(PRINT_STD, "   (4) test_arnr                            "
        "\n");
    tprintf(PRINT_STD, "   (5) test_auto_key_frame                  "
        "VPXTempScaleEnc\n");
    tprintf(PRINT_STD, "   (6) test_buffer_level                    "
        "VPXTTempScaleEnc\n");
    tprintf(PRINT_STD, "   (7) test_change_cpu_dec                  "
        "VPXMultiResEnc\n");
    tprintf(PRINT_STD, "   (8) test_change_cpu_enc                  "
        "VPXTMultiResEnc\n");
    tprintf(PRINT_STD, "   (9) test_constrained_quality             "
        "VPXTDecRESZ\n");
    tprintf(PRINT_STD, "  (10) test_copy_set_reference              "
        "VPXTDecPARD\n");
    tprintf(PRINT_STD, "  (11) test_data_rate                       "
        "VPXTDec2Raw\n");
    tprintf(PRINT_STD, "  (12) test_debug_matches_release           "
        "\n");
    tprintf(PRINT_STD, "  (13) test_drop_frame_watermark            "
        "VPXTDataRate\n");
    tprintf(PRINT_STD, "  (14) test_encoder_break_out               "
        "VPXTPSNR\n");
    tprintf(PRINT_STD, "  (15) test_error_concealment               "
        "VPXTPSNRDEC\n");
    tprintf(PRINT_STD, "  (16) test_error_resolution                "
        "VPXTCheckPBM\n");
    tprintf(PRINT_STD, "  (17) test_extra_file                      "
        "\n");
    tprintf(PRINT_STD, "  (18) test_fixed_quantizer                 "
        "Raw2Formatted\n");
    tprintf(PRINT_STD, "  (19) test_force_key_frame                 "
        "Formatted2Raw\n");
    tprintf(PRINT_STD, "  (20) test_frame_size                      "
        "Formatted2RawFrames\n");
    tprintf(PRINT_STD, "  (21) test_good_vs_best                    "
        "CombineIndvFrames\n");
    tprintf(PRINT_STD, "  (22) test_lag_in_frames                   "
        "\n");
    tprintf(PRINT_STD, "  (23) test_max_quantizer                   "
        "CompareEnc\n");
    tprintf(PRINT_STD, "  (24) test_mem_leak                        "
        "CompareHeader\n");
    tprintf(PRINT_STD, "  (25) test_mem_leak2                       "
        "DisplayHeader\n");
    tprintf(PRINT_STD, "  (26) test_min_quantizer                   "
        "\n");
    tprintf(PRINT_STD, "  (27) test_multiple_resolution_encode      "
        "DispKeyFrames\n");
    tprintf(PRINT_STD, "  (28) test_multithreaded_dec               "
        "DispResizedFrames\n");
    tprintf(PRINT_STD, "  (29) test_multithreaded_enc               "
        "DispVisibleFrames\n");
    tprintf(PRINT_STD, "  (30) test_new_vs_old_enc_cpu_tick         "
        "DispAltRefFrames\n");
    tprintf(PRINT_STD, "  (31) test_new_vs_old_psnr                 "
        "\n");
    tprintf(PRINT_STD, "  (32) test_new_vs_old_temp_scale           "
        "CropRawClip\n");
    tprintf(PRINT_STD, "  (33) test_noise_sensitivity               "
        "PadRawClip\n");
    tprintf(PRINT_STD, "  (34) test_one_pass_vs_two_pass            "
        "CutClip\n");
    tprintf(PRINT_STD, "  (35) test_play_alternate                  "
        "PasteClip\n");
    tprintf(PRINT_STD, "  (36) test_post_processor                  "
        "\n");
    tprintf(PRINT_STD, "  (37) test_post_processor_mfqe             "
        "PlayDecIVF\n");
    tprintf(PRINT_STD, "  (38) test_reconstruct_buffer              "
        "PlayCompIVF\n");
    tprintf(PRINT_STD, "  (39) test_resample_down_watermark         "
        "\n");
    tprintf(PRINT_STD, "  (40) test_speed                           "
        "CreateSampleTextFiles\n");
    tprintf(PRINT_STD, "  (41) test_temporal_scalability            "
        "PrintVersion\n");
    tprintf(PRINT_STD, "  (42) test_test_vector                     "
        "\n");
    tprintf(PRINT_STD, "  (43) test_thirtytwo_vs_sixtyfour          "
        "VPXEncPar\n");
    tprintf(PRINT_STD, "  (44) test_two_pass_vs_two_pass_best       "
        "RandParFile\n");
    tprintf(PRINT_STD, "  (45) test_undershoot                      "
        "RandCompress\n");
    tprintf(PRINT_STD, "  (46) test_version                         "
        "GraphPSNR\n");
    tprintf(PRINT_STD, "  (47) test_vpx_matches_int                 "
        "Help\n");
    tprintf(PRINT_STD, "  (48) test_win_lin_mac_match               "
        "\n");
#if !defined(_WIN32)
    tprintf(PRINT_STD, "\n");
#endif

    return;
}

int show_hidden_cmds()
{
    tprintf(PRINT_STD, "\n\n"
        "    Hidden Commands \n"
        "\n"
        "Thres\n"
        "DispFrameData\n"
        "DispDropedFrames\n"
        "GetFrameCount\n"
        "\n"
        "TestFileInputCheck\n"
        "RandomStressTest\n"
        "FormatSummary\n"
        "\n"
        "Quad\n"
        "CompareDec\n"
        "PrintCpuInfo\n"
        "\n"
        "CopyAllTxtFiles\n"
        "\n"
        "CombineIndvFrames\n"
        "WriteIvfFileHeader\n"
        "WriteIvfFrameHeader\n"
        "WriteFrameData\n"
        "\n");
    return 0;

}

void write_32bit_quick_test(const std::string& working_dir)
{
    std::string text_file_str;
    vpxt_folder_name(working_dir.c_str(), &text_file_str);
    text_file_str += "QuickTest_32Bit.txt";

    FILE *fp;

    if ((fp = fopen(text_file_str.c_str(), "w")) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            text_file_str.c_str());
        exit(1);
    }

    std::string OSStr;

#if defined(_WIN32)
    OSStr = "Win";
#elif defined(__APPLE__)
    OSStr = "Mac";
#elif defined(linux)
    OSStr = "Lin";
#endif

    fprintf(fp, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp, "%%              \n");
    fprintf(fp, "%%              QuickTest %s32Bit\n", OSStr.c_str());
    fprintf(fp, "%%              \n");
    fprintf(fp, "%%              \n");
    fprintf(fp, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp, "%%%%%%%%%%%%%%%%%%Verify Encoders and Decoders%%%%%%%%%%%%%%%"
        "%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@webm@y4m\n", "test_extra_file",
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%"
        "%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_good_vs_best", slashChar(), slashChar());
    fprintf(fp, "%s@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@..%c"
        "TestClips%cMemLeakCheck2_Compression.ivf\n", "test_mem_leak2",
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_multiple_resolution_encode", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_one_pass_vs_two_pass", slashChar(), slashChar());
    fprintf(fp, "%s@%i@..%cTestClips%cTestVectors\n",
        "test_test_vector", 1, slashChar(), slashChar());
    fprintf(fp, "%s@%i@..%cTestClips%cTestVectors\n",
        "test_test_vector", 2, slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_two_pass_vs_two_pass_best", slashChar(), slashChar());
    fprintf(fp, "%%%s@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA@webm@y4m\n",
        "test_win_lin_mac_match", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@400@600@0@0@0@webm"
        "\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@400@600@0@0@0@webm"
        "\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@3@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@6@400@600@1000@1500@"
        "2000@webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@7@400@600@0@0@0@webm"
        "\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@8@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@400@600@0@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@400@600@0@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@3@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@6@400@600@1000@1500@"
        "2000@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@7@400@600@0@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@8@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%"
        "%%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@10@webm@y4m\n",
        "test_speed", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@10@webm@y4m\n",
        "test_speed", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%%%%%MultiThreaded%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n",
        "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n",
        "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n",
        "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n",
        "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%"
        "%%%%%%%%%%%\n");
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode0%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@0@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@6@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@VP8vNewest_Plug"
        "In_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@10@200@webm@y4m\n",
        "test_post_processor_mfqe", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode1%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128@webm@y4m\n",
        "test_allow_drop_frames", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c1-test_change_cpu_enc-cpuused-4.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@1@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@VP8vNewest_Plug"
        "In_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-file"
        "s%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@10@200@webm@y4m\n",
        "test_post_processor_mfqe", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode2%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@2@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@VP8vNewest_Plug"
        "In_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-file"
        "s%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@10@200@webm@y4m\n",
        "test_post_processor_mfqe", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode4%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_allow_lag", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_arnr", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@4@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m@..%cTestCl"
        "ips%cspecial-case-par-files%c5-test_frame_size_settings.txt\n",
        "test_frame_size", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@10@24@webm@y4m\n",
        "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@VP8vNewest_Plug"
        "In_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_play_alternate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cInternal_gNYZH9kuaYM_640x360_2000F.ivf@4@51"
        "2@webm@ivf@..%cTestClips%cspecial-case-par-files%c4-test_reconstruct_b"
        "uffer_settings.txt\n", "test_reconstruct_buffer", slashChar(),
        slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode5%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_allow_lag", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_arnr",
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@5@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@10@24@webm@y4m\n",
        "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@VP8vNewest_Plug"
        "In_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_play_alternate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    tprintf(PRINT_STD, "\n\nQuick Test file created:\n%s\n\n",
        text_file_str.c_str());
    fclose(fp);
}
void write_64bit_quick_test(const std::string& working_dir)
{
    std::string text_file_str;
    vpxt_folder_name(working_dir.c_str(), &text_file_str);
    text_file_str += "QuickTest_64Bit.txt";

    FILE *fp;

    if ((fp = fopen(text_file_str.c_str(), "w")) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            text_file_str.c_str());
        exit(1);
    }

    std::string OSStr;

#if defined(_WIN32)
    OSStr = "Win";
#elif defined(__APPLE__)
    OSStr = "Mac";
#elif defined(linux)
    OSStr = "Lin";
#endif

    fprintf(fp, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp, "%%              \n");
    fprintf(fp, "%%              QuickTest %s64Bit\n", OSStr.c_str());
    fprintf(fp, "%%              \n");
    fprintf(fp, "%%              \n");
    fprintf(fp, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@webm@y4m\n", "test_extra_file",
        slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%%%%%MultiThreaded%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n",
        "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n",
        "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n",
        "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n",
        "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%%%%%Verify Encoders and Decoders%%%%%%%%%%%%%%%"
        "%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@vpxenc.exe@vpxdec.exe@webm@"
        "y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%"
        "%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_good_vs_best", slashChar(), slashChar());
    fprintf(fp, "%s@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@..%cTestClips%cMemL"
        "eakCheck2_Compression.ivf\n", "test_mem_leak2", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_multiple_resolution_encode", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_one_pass_vs_two_pass", slashChar(), slashChar());
    fprintf(fp, "%s@%i@..%cTestClips%cTestVectors\n",
        "test_test_vector", 1, slashChar(), slashChar());
    fprintf(fp, "%s@%i@..%cTestClips%cTestVectors\n",
        "test_test_vector", 2, slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n",
        "test_two_pass_vs_two_pass_best", slashChar(), slashChar());
    fprintf(fp, "%%%i@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA\n",
        "test_win_lin_mac_match", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@400@600@0@0@0@webm"
        "\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@400@600@0@0@0@webm"
        "\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@3@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@6@400@600@1000@1500@"
        "2000@webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@7@400@600@0@0@0@webm"
        "\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@8@400@600@1000@0@0@"
        "webm\n", "test_temporal_scalability", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@400@600@0@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@400@600@0@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@3@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@6@400@600@1000@1500@"
        "2000@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@7@400@600@0@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@8@400@600@1000@0@0@"
        "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@webm\n",
        "test_new_vs_old_temp_scale", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%"
        "%%%%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@10@webm@y4m\n",
        "test_speed", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@10@webm@y4m\n",
        "test_speed", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%"
        "%%%%%%%%%%%\n");
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode0%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@0@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@6@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@VP8vNewest_Plug"
        "In_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@10@200@webm@y4m\n",
        "test_post_processor_mfqe", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode1%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128@webm@y4m\n",
        "test_allow_drop_frames", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c1-test_change_cpu_enc-cpuused-4.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@2@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@VP8vNewest_Plug"
        "In_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@10@200@webm@y4m\n",
        "test_post_processor_mfqe", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode2%%%%\n");
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@2@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@VP8vNewest_Plug"
        "In_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@10@200@webm@y4m\n",
        "test_post_processor_mfqe", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode4%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_allow_lag", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_arnr", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@4@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m@..%cTestCl"
        "ips%cspecial-case-par-files%c5-test_frame_size_settings.txt\n",
        "test_frame_size", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@10@24@webm@y4m\n",
        "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@VP8vNewest_Plug"
        "In_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_play_alternate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cInternal_gNYZH9kuaYM_640x360_2000F.ivf@4@"
        "512@webm@y4m@..%cTestClips%cspecial-case-par-files%c4-test_reconstruct"
        "_buffer_settings.txt\n", "test_reconstruct_buffer", slashChar(),
        slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Mode5%%%%\n");
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_allow_lag", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n",
        "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_arnr", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@6@webm@y4m\n",
        "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_buffer_level", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@0@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@1@webm@y4m\n",
        "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@256@0@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc"
        , slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@256@1@webm@y4m@..%cTestClips%cs"
        "pecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n",
        "test_change_cpu_enc", slashChar(), slashChar(), slashChar(),
        slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@256@10@webm@y4m\n",
        "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@120@webm@y4m\n",
        "test_copy_set_reference", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@288@webm@y4m\n",
        "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csam-1920x1080.ivf@5@1024@webm@y4m\n",
        "test_data_rate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n",
        "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n",
        "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_error_concealment", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_error_resolution", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@20@60@webm@y4m\n",
        "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@24@webm@y4m\n",
        "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@176@144@webm@y4m\n",
        "test_frame_size", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@10@24@webm@y4m\n",
        "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMod"
        "e_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@VP8vNewest_Plug"
        "In_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files"
        "%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n",
        "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMod"
        "e_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(),
        slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_play_alternate", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_post_processor", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_undershoot", slashChar(), slashChar());
    fprintf(fp, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n",
        "test_version", slashChar(), slashChar());
    fprintf(fp, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    tprintf(PRINT_STD, "\n\nQuick Test file created:\n%s\n\n",
        text_file_str.c_str());
    fclose(fp);
}

int print_quick_test_files(const std::string& working_dir)
{
    // 32BitQuickRun
    write_32bit_quick_test(working_dir);
    // 64BitQuickRun
    write_64bit_quick_test(working_dir);

    return 0;
}

int vpxt_test_help(const char *input_char, int printSummary)
{
    std::string input_str = input_char;
    int selector = vpxt_identify_test(input_char);
    std::string print_base_opts = "\n"
        "    <Input File>\n"
        "    <Mode>\n"
        "          (0)Realtime/Live Encoding\n"
        "          (1)Good Quality Fast Encoding\n"
        "          (2)One Pass Best Quality\n"
        "          (3)Two Pass - First Pass\n"
        "          (4)Two Pass\n"
        "          (5)Two Pass Best Quality\n"
        "    <Target Bit Rate>\n";

    get_test_name(selector, input_str);

    if (selector == kTestMultiRun)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD,
            "\n"
            "    <Test Type>\n"
            "         <1 - Create Files and Run Tests>\n"
            "         <2 - Create Files only>\n"
            "         <3 - Run Tests For Existing Files - Requires Input "
            "Test directory>\n"
            "    <Input Text File>\n"
            "    <Optional - Input Test directory>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test runs tests from an input file"
            " and summarizes the results. The test can be run in four modes"
            ": Mode 1 = Create compressions and run tests, Mode 2 = Create "
            "compressions only, Mode 3 = Run tests on pre-existing "
            "compressions, and Mode 4 = Resume tests in progress. To create"
            " a template driver text file use the command "
            "`CreateSampleTextFiles`.");

#if !defined(_WIN32)
        tprintf(PRINT_STD, "\n");
#endif
        return 0;
    }

    if (selector == kTestAllowDropFrames)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with Allow Drop Frames on, the second with Allow "
            "Drop Frames off. It then records and compares the number of "
            "frames each compression has. If Allow Drop Frames on has fewer"
            " frames than Allow Drop Frames off, the test passes.");
    }

    if (selector == kTestAllowLag)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with Allow Lag equal to 0, the second with Allow "
            "Lag equal to 1. If the correct number of lagged frames are "
            "detected via quantizer output, alternate reference frames "
            "exist for Allow Lag on, Allow Lag on has the same number of "
            "visible frames as Allow Lag off, and Allow Lag on is not "
            "identical to Allow Lag off, the test passes.");
    }

    if (selector == kTestAllowSpatialResampling)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with Allow Spatial Resampling off, the second with "
            "Allow Spatial Resampling on. The test then records the number "
            "of resized frames for each and computes the PSNR for Allow "
            "Spatial Resampling on. If Allow Spatial Resampling on has "
            "resized frames, Allow Spatial Resampling off does not have "
            "resized frames and Allow Spatial Resampling on has a PSNR "
            "greater than 15, the test passes.");
    }

    if (selector == kTestArnr)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "    <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates four compressions: "
            "the first with Alternate Reference Frames off, the second "
            "with Alternate Reference Frames on and an ARNR Type equal to "
            "one, the third with Alternate Reference Frames on and an ARNR "
            "Type equal to two, the fourth with Alternate Reference Frames "
            "on and an ARNR Type equal to three.  The test then computes "
            "PSNR values and Data Rates for all files.  If all compressions"
            " are not identical, all PSNR values increase as ARNR Type "
            "increases or are within five percent of the Alternate "
            "Reference Frames off compression, and Data Rates decrease as "
            "ARNR Type increases or are within five percent of the "
            "Alternate Reference Frames off compression, the test passes.");
    }

    if (selector == kTestAutoKeyFrame)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions "
            "with identical parameters, using user input Auto Key Frames. "
            "The test then records the placement of each compression's key "
            "frames. If both compressions' key frames occur in identical "
            "locations and at least as frequently as Auto Key Frame "
            "dictates, the test passes.");

    }

    if (selector == kTestBufferLevel)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression and "
            "runs CheckPBM on it. If no buffer underrun is detected, the "
            "test passes.");

    }

    if (selector == kTestChangeCpuDec)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Version>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression of the "
            "user input version (0-3) and then decompresses it using mmx, "
            "sse, sse2, sse3, ssse3 and sse4_1 instruction sets. The test "
            "then compares them against one another. If all compressions "
            "are identical and the times to decompress them are not, the "
            "test passes.");

    }

    if (selector == kTestChangeCpuEnc)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Version>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates compressions of the "
            "user input version (0-3) using mmx, sse, sse2, sse3, ssse3 and"
            " sse4_1 instruction sets. If all compressions are identical "
            "and compression times are not, the test passes.");
    }

    if (selector == kTestConstrainedQuality)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Constrained Quantizer>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with constrained quality off, the second with user "
            "input constrained quality.  The test then records their PSNRs "
            "and Data Rates.  If linear PSNR per Data Rate is greater for "
            "user input Constrained Quality, the test passes.");
    }

    if (selector == kTestCopySetReference)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <First Copy Frame>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression with "
            "error resilient on and then decompresses it using "
            "vpxt_decompress_copy_set.  At the First Copy Frame the decoder"
            " is copied to decoder_copy and run syncronusly with the "
            "original.  So long as the First Copy Frame is reached and all "
            "decoded original and copy frames after the First Copy Frame "
            "are identical the test passes.");
    }

    if (selector == kTestDataRate)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression and "
            "records its data rate. If the compression's data rate is "
            "within 30%% of the input target bandwidth, the test passes.");

    }

    if (selector == kTestDebugMatchesRelease)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Debug Executable - Must take <INPUT FILE> <OUTPUT FILE> "
            "<PARAMETER FILE>\n"
            "    <Release Executable-Must take <INPUT FILE> <OUTPUT FILE> "
            "<PARAMETER FILE>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first using an executable built using the newest release "
            "library, the second using an executable built using the newest"
            " debug library. If the two compressions are identical, the "
            "test passes.");

    }

    if (selector == kTestDropFrameWaterMark)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates 6 compressions with "
            "DFWM values of 100, 80, 60, 40, 20 and 0, and records their "
            "sizes. If each successively lower DFWM compression has an "
            "equal or larger size than the previous, the test passes.");

    }

    if (selector == kTestEncoderBreakout)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates four compressions: "
            "the first has an EncodeBreakout of 0, the second "
            "EncodeBreakout of 100, the third EncodeBreakout of 500 and the"
            " fourth EncodeBreakout of 1000. Decompressions of the encoded"
            " files are then carried out and PSNR values are calculated. If"
            " the decompressions run successfully and the PSNR values of "
            "each successive EncodeBreakout trial are within 2 dB, the test"
            "passes. If the PSNRs are greater than 2 dB but less than 5 dB"
            " the test is inconclusive; if the PSNRs have greater than a 5"
            " dB difference the test fails.");
    }

    if (selector == kTestErrorConcealment)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression with "
            "error resilient mode on and then decompresses it with partial "
            "frame drops.  Psnrs are calculated for both the decoded file "
            "and encoded file.  If the psnr of the decoded file is with in "
            "50% of the psnr of the encoded file, the test passes.");

    }

    if (selector == kTestErrorResolution)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with Error Resilient Mode off, the second with Error"
            " Resilient Mode on. The test then records their PSNR values. "
            "If their PSNRs are within 10%% of each other, the test "
            "passes.");

    }

    if (selector == kTestExtraFile)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a two-pass "
            "compression and checks for extra files in the current "
            "directory, the directory the executable is located in, and the"
            " directory the output file is written to. If no extra files "
            "are found, the test passes.");
    }

    if (selector == kTestFixedQuantizer)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <FixedQ 1>\n"
            "    <FixedQ 2>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions "
            "each with user input Fixed Quantizers, and records the "
            "quantizers used to encode each frame. The test then records "
            "the compressions' sizes. If all quantizers for each "
            "compression match the input Fixed Quantizer, and the smaller "
            "quantizer's compression has a larger file size, the test "
            "passes.");
    }

    if (selector == kTestForcedKeyFrame)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <ForceKeyFrame>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression using "
            "a user-input value as a Force Key Frame Interval. The Test "
            "then records the placement of all key frames in the "
            "compression. If key frames occur only when Force Key Frame "
            "dictates, the test passes.");

    }

    if (selector == kTestFrameSize)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Starting Width-must be a mult of 16>\n"
            "    <Starting Height-must be a mult of 16>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates forty-six cropped "
            "raw ivf or y4m files from a user inputfile, starting height, "
            "and starting width (where starting height and starting width "
            "must be multiples of 16).  The test then creates compressions "
            "for each raw file and computes its PSNR.  If all compressions "
            "PSNRs are within five percent of the starting width/height "
            "file's PSNR and all PSNRs are greater than 25 the test "
            "passes.");
    }

    if (selector == kTestGoodVsBest)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Target Bit Rate>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates six compressions: "
            "the first and fourth compressions for 30%% less than the input"
            " target bandwidth at good quality and best quality, the second"
            " and fifth compressions for the input target bandwidth at good"
            " quality and best quality, and the third and sixth at 30%% "
            "more than the input target bandwidth at good quality and best "
            "quality. The test then records each file's data rate and PSNR,"
            " and computes the area under the curve for the common interval"
            " between the good quality curve and best quality curve. If the"
            " area under the best quality curve is greater than the area "
            "under the good quality curve, the test passes.");
    }

    if (selector == kTestLagInFrames)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Lag in Frames 1>\n"
            "    <Lag in Frames 2>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates three compressions: "
            "the first with Allow Lag set to 0, the second and third with "
            "Allow Lag set to 1. The second compression uses the first "
            "user-input Lag in Frames value for its Lag in frames and the "
            "third uses the second user-input value for its Lag in Frames. "
            "The test outputs each file's quantizer values for each encoded"
            " frame. If none of the files are identical, the PSNRs of each "
            "successive file are within 10%% of the last, and the quantizer"
            " output shows that the proper number of frames were lagged, "
            "the test passes.");
    }

    if (selector == kTestMaxQuantizer)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates nine compressions, "
            "the first with a Max Quantizer equal to 4 and each subsequent "
            "file with a Max Quantizer 8 greater than the last until 63. "
            "The test records the individual quantizer values for each "
            "encoded frame. If the PSNRs of each Max Quantizer compression "
            "from 4 to 63 increase as Max Quantizer decreases (or are "
            "within 1 %% of eachother) and the recorded quantizers for each"
            " file do not exceed their corresponding Max Quantizer for all "
            "compressions, the test passes.");

    }

    if (selector == kTestMemLeak)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Newest Debug executable>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression using "
            "the debug executable to check memory usage and records the "
            "results to an output file. If no memory leaks are found, the "
            "test passes.");
    }

    if (selector == kTestMemLeak2)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Mem Leak Check Exe>\n"
            "\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test uses the debug executable to "
            "open and close 10,000 instances of the encoder and 10,000 "
            "instances of the decoder, and then checks for no memory leaks."
            " If there are no leaks, the test passes.");
    }

    if (selector == kTestMinQuantizer)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with a Min Quantizer equal to 10, the second with a "
            "Min Quantizer equal to 60. It records the quantizer used for "
            "each frame of the compression. If the first file has a higher "
            "PSNR than the second file and every quantizer for both files "
            "is above the corresponding Min Quantizer, the test passes.");
    }

    if (selector == kTestMultiResolutionEncode)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Target Bit Rate>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a user input number "
            "of multiple resolution encoded files.  Then psnr calculations "
            "are done on the resultant files.  If all psnr values are above"
            " the minimum threshold, and increase as frame sizes increase, "
            "the test passes.");
    }

    if (selector == kTestMultiThreadedDec)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Number of Cores to Use>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression with a "
            "user input number of token partitions.  It then preforms two "
            "decompressions on this compression.  The first decompression "
            "using 0 threads,  The second decompression using a user input "
            "number of threads.  If the user input number of threads takes "
            "less time to decompress and the two decompressions are "
            "identical, the test passes.");
    }

    if (selector == kTestMultiThreadedEnc)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Number of Cores to Use>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first using a MultiThreaded equal to 2, the second using a"
            " MultiThreaded equal to 0. The test then compares the times to"
            " compress each. If MultiThreaded 2 is faster than 0, the test "
            "passes.");
    }

    if (selector == kTestNewVsOldEncCpuTick)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Target Bit Rate>\n"
            "    <Exe File To Compare>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression using "
            "the newest version of VP8.  It then compares the encode time "
            "for this compression against an old encode time obtained "
            "either from a log file or by running another identical encode "
            "using an old executable. If the new compression's encode time "
            "is faster than the old compressions encode time, the test "
            "passes.");
    }

    if (selector == kTestNewVsOldPsnr)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Exe File To Compare>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression using "
            "the newest version of VP8. It then compares the psnr values "
            "for this compression against an old PSNR value obtained either"
            " from a log file or by running another identical encode using "
            "an old executable.  If the new compression's PSNR is greater "
            "than the old compressions PSNR (or within 1%%), the test "
            "passes.");
    }

    if (selector == kTestNewVsOldTempScale)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Layer Mode 0-6>\n"
            "    <bitrate 0 (Modes 0-8)>\n"
            "    <bitrate 1 (Modes 0-8)>\n"
            "    <bitrate 2 (Modes 2-6&8, else 0)>\n"
            "    <bitrate 3 (Mode 6, else 0)>\n"
            "    <bitrate 4 (Mode 6, else 0)>\n"
            "    <Exe File To Compare>\n"
            "    <Encode Format - webm/ivf>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a user input number "
            "of temporally scaled encodes.  It then compares the PSNR "
            "values and encode times for these compressions against old "
            "psnr values and encode times obtain either from a log file or "
            "by running another set of identical encodes using an old "
            "executable.  If the new compression's PSNR is greater than "
            "the old compressions PSNR (or within 1%%), and the new "
            "compressions encode time is faster than the old, the test "
            "passes.");
    }

    if (selector == kTestNoiseSensitivity)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates seven compressions "
            "with Noise Sensitivity values from 0 to 6 and computes PSNR "
            "values for each. If all compressions have differing PSNR "
            "values and Noise Sensitivity 0 has a higher PSNR than Noise "
            "Sensitivity 6, the test passes.");
    }

    if (selector == kTestOnePassVsTwoPass)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Target Bit Rate>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates six compressions: "
            "the first and fourth compressions for 30%% less than the input"
            " target bandwidth at one-pass good quality and two-pass good "
            "quality, the second and fifth compressions for the input "
            "target bandwidth at one-pass good quality and two-pass good "
            "quality, and the third and sixth at 30%% more than the input "
            "target bandwidth at one-pass good quality and two-pass good "
            "quality. The test then records each file's data rate and PSNR "
            "and computes the area under the curve for the common interval "
            "between the one-pass good quality curve and the two-pass good "
            "quality curve. If the area under the two-pass good quality "
            "curve is greater than the area under the one-pass good quality"
            " curve, the test passes.");
    }

    if (selector == kTestPlayAlternate)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with Play Alternate equal to 0, the second with Play"
            " Alternate equal to 1. The test then records the placement of "
            "alternate reference frames and visible frames for both "
            "compressions. If alternate reference frames exist for "
            "Play Alternate = 1 and not for Play Alternate = 0, visible "
            "frames for Play Alternate 1 and Play Alternate 2 are equal, "
            "and the files are not identical, the test passes.");
    }

    if (selector == kTestPostProcessor)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression, then "
            "creates a No Filtering decompression, and decompressions for "
            "Deblock and Noise levels ranging from 0 to 15. If all Deblock "
            "and Noise decompressions return a different PSNR than the No "
            "Filtering Decompression but are within 10%%, the test "
            "passes.");
    }

    if (selector == kTestPostProcessorMfqe)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Key Freq>\n"
            "    <Max Intra Bitrate Pct>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression, then "
            "then preforms psnr calculations using the mfqe post processing"
            " flag and no post processing.  If the mfqe psnr is greater "
            "than the no post processing psnr, the test passes.");
    }

    if (selector == kTestReconstructBuffer)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());


        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression and "
            "internally compares the compressor's preview frames to the "
            "decoded output produced by decompressing the compressor's "
            "encoded frame. The state of each frame is recorded to a text "
            "file. If the contents of all preview frames are identical to "
            "the content of all decoded frames, the test passes.");
    }

    if (selector == kTestResampleDownWatermark)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with resample-down-watermark set to 90, the second "
            "with resample-down-watermark set to 10. The test then records "
            "the frames at which the file buffer reaches the designated "
            "thresholds, the location of key frames, and location of "
            "resized frames for both files. If the first resized frame "
            "occurs on the first instance where the frame prior to a key "
            "frame reaches the correct buffer saturation for both "
            "compressions, the test passes.");
    }

    if (selector == kTestSpeed)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Lag in Frames>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test works for RealTime Mode and "
            "Good Quality Mode. For RealTime Mode the test creates "
            "compressions for CpuUsed Values from -1 to -16 and 0 to 16. "
            "For Good Quality Mode the test creates compressions for "
            "CpuUsed Values from 0 to 5. If compression speed increases as "
            "CpuUsed increases and all PSNRs are within 10%% of the "
            "previous, the test passes.");
    }

    if (selector == kTestTemporalScalability)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Layer Mode 0-6>\n"
            "    <Bitrate 0 (Modes 0-8)>\n"
            "    <Bitrate 1 (Modes 0-8)>\n"
            "    <Bitrate 2 (Modes 2-6&8, else 0)>\n"
            "    <Bitrate 3 (Mode 6, else 0)>\n"
            "    <Bitrate 4 (Mode 6, else 0)>\n"
            "    <Encode Format - webm/ivf>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a user input number "
            "of temporally scaled encodes along with corresponding normal "
            "encodes.  The test then records frame statistics for the "
            "resultant temporally scaled encode along with psnr values for "
            "the both the temporally scaled encodes and normal encodes.  If"
            " the correct psnr relationships obtain, frame decimation "
            "occurs correctly, and the temporally scaled encodes time is "
            "within or less than a given range, the test passes.");

    }

    if (selector == kTestTestVector)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Threads>\n"
            "    <Input directory>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test decodes each VP8 Test Vector "
            "using the number of threads input and Checks its MD5 checksum "
            "against the expected value. If all Test Vectors decode "
            "properly and all MD5 checksums match their expected values, "
            "the test passes.");
    }

    if (selector == kTestThirtytwoVsSixtyfour)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Output directory>\n"
            "    <Settings File>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test is run in one of two modes. "
            "The first creates either a thirty two or sixty four bit "
            "compression and decompression to be tested by the second mode."
            "The second mode creates either a thirty two or sixty four bit "
            "compression and decompression, and then compares them to "
            "previously encoded and decoded files. If the files are "
            "identical, the test passes.");
    }

    if (selector == kTestTwoPassVsTwoPassBest)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Target Bit Rate>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates six compressions: "
            "the first and fourth compressions for 30%% less than the input"
            " target bandwidth at two-pass good quality and two-pass best "
            "quality, the second and fifth compressions for the input "
            "target bandwidth at two-pass good quality and two-pass best "
            "quality, and the third and sixth at 30%% more than the input "
            "target bandwidth at two-pass good quality and two-pass best "
            "quality. The test then records each file's data rate and PSNR "
            "and computes the area under the curve for the common interval "
            "between the two-pass good quality curve and the two-pass best "
            "quality curve. If the area under the two-pass best quality "
            "curve is greater than the area under the two-pass good quality"
            " curve, the test passes.");
    }

    if (selector == kTestUndershoot)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: "
            "the first with an undershoot equal to 10, the second with an "
            "undershoot equal to 100. If the Undershoot 100 compression's "
            "file size is greater than the Undershoot 10 compression's file"
            " size, the test passes.");
    }

    if (selector == kTestVersion)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates four compressions: "
            "the first with Version equal to 0, the second with Version "
            "equal to 1, the third with Version equal to 2, and the fourth "
            "with Version equal to 3. The test then decodes each and "
            "records the time it took to do so. If each successive Version "
            "takes less time than the prior to decode and has a lower PSNR,"
            " the test passes.");
    }

    if (selector == kTestVpxMatchesInt)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "%s"
            "    <Vpxenc Executable>\n"
            "    <Vpxdec Executable>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n"
            "     <Optional Settings File>\n", print_base_opts.c_str());

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test takes a raw input file along "
            "with vpxenc and vpxdec paths and produces two encode files and"
            " two decode files.  The first encode file is produced using "
            "the testers internal compressor.  The second encode file is "
            "produced using the passed in vpxenc path.  The first decode "
            "file is produced using the passed in vpxdec path.  The second "
            "decode file is produced using the testers internal decoder.  "
            "The test then compares the encoded and decoded files.  If all "
            "decoded and encoded files are identical the test passes.");
    }

    if (selector == kTestWinLinMacMatch)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", input_str.c_str());

        tprintf(PRINT_STD, "\n"
            "    <Input File>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Output directory>\n"
            "    <Settings File>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Decode Format - y4m/ivf>\n");

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test can be run in two test "
            "modes. The first, Mode 0, creates platform-specific "
            "compressions and decompressions to be tested on another "
            "platform. The second mode, Mode 1, creates platform-specific "
            "compressions and decompressions, and then compares them to "
            "previously encoded and decoded files created by Mode 0. If "
            "the files are identical, the test passes.");
    }

#if !defined(_WIN32)

    if (!printSummary)
        tprintf(PRINT_STD, "\n");

#endif

    return 0;
}
int vpxt_tool_help(const char *input_char, int printSummary)
{
    // return 1 if string found return 0 if string not found if string not
    // found TestHelp will be run through.
    std::string input_str = input_char;
    vpxt_lower_case_string(input_str);

    if (input_str.compare("vpxenc") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  VPXEnc\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "VPXEnc.exe clone");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxdec") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXDec\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "VPXDec.exe clone");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtenc") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT Compress\n\n"
            "    <(1)Normal Compress |(2)TimeCompress>\n"
            "    <Input File>\n"
            "    <outputfile>\n"
            "    <Bit Rate>\n"
            "    <Mode>\n"
            "    <Encode Format - webm/ivf>\n"
            "    <Optional - Parameter File>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw IVF or "
            "Y4M file and produce an encoded IVF or WEBM file using the "
            "given mode and bitrate. Default encode settings can be "
            "overridden by specifying a parameter file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtdec") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT Decompress\n\n"
            "    <Input File>\n"
            "    <Output File>\n"
            "    <Decode Format - y4m/ivf>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an encoded IVF "
            "or WEBM file and output a decoded IVF or Y4M file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtempscaleenc") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  VPX Temp Scale Enc\n\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "vp8_scalable_patterns.exe clone");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxttempscaleenc") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  VPXT Temp Scale Enc\n\n"
            "    <Input File>\n"
            "    <Output Base>\n"
            "    <Layer Mode 0-6>\n"
            "    <Bitrate 0 (Modes 0-6)>\n"
            "    <Bitrate 1 (Modes 0-6)>\n"
            "    <Bitrate 2 (Modes 2-6, else 0)>\n"
            "    <Bitrate 3 (Mode 6, else 0)>\n"
            "    <Bitrate 4 (Mode 6, else 0)>\n"
            "    <Encode Format - webm/ivf>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxmultiresenc") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  VPX Multi Resolution Encoder\n\n"
            "    <Width>\n"
            "    <Height>\n"
            "    <Input File>\n"
            "    <Output 1>\n"
            "    <Output 2>\n"
            "    <Output 3>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtmultiresenc") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  VPXT Multi Resolution Encoder\n\n"
            "    <Input File>\n"
            "    <Output>\n"
            "    <Bit Rate>\n"
            "    <Encode Format - webm/ivf>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtdecresz") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT Decompress Resize\n\n"
            "    <Input File>\n"
            "    <Output File>\n"
            "    <Decode Format - y4m/ivf>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an encoded IVF "
            "or WEBM file and output a decoded IVF or Y4M file making sure "
            "to correctly scale up any resized encoded frames.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtdecpard") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT Decompress Partial Drops\n\n"
            "    <Input File>\n"
            "    <Output File>\n"
            "    <Decode Format - y4m/ivf>\n"
            "    <Parameter 1>\n"
            "    <Parameter 2>\n"
            "    <Mode (0-2)>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an encoded IVF "
            "or WEBM file and output a decoded IVF or Y4M file with user "
            "input partial drops.  The first parameter signifies the "
            "numerator of the ratio of frames to drop for mode 0, the "
            "starting frame for mode 1 and the percentage to randomly drop "
            "for mode 2.  The second parameter signifies the denominator of"
            " the ratio of frames to drop for mode 0, the last frame to "
            "drop for mode 1 and the random seed for mode 2.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtdec2raw") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT Decompress to Raw \n\n"
            "    <Input File>\n"
            "    <outputfile>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an encoded IVF "
            "or WEBM file and output a decoded raw file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtdatarate") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT DataRate \n\n"
            "    <Input File>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an IVF or WEBM "
            "file and compute its average, min, max, and file data rates.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtpsnr") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT PSNR\n\n"
            "     <Source File>\n"
            "     <Compressed File>\n"
            "     <Run SSIM - 0|1>\n"
            "     <Artifact Detection - 0|1>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will compute an encoded IVF "
            "or WEBM file's PSNR using the encoded file's IVF or Y4M source"
            " file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtpsnrdec") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT PSNR DEC\n\n"
            "     <Source File>\n"
            "     <Decoded Compressed File>\n"
            "     <Run SSIM - 0|1>\n"
            "     <Width    - optional>\n"
            "     <Height   - optional>\n"
            "     <SWAP UV  - optional>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will compute a decoded IVF "
            "or WEBM file's PSNR using the encoded file's IVF or Y4M source"
            " file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxtcheckpbm") == 0)
    {
        tprintf(PRINT_STD,
            "\n  VPXT Check PBM\n\n"
            "     <Input File>\n"
            "     <Bitrate         (kbps)>\n"
            "     <Max Buffer Size (ms)>\n"
            "     <Prebuffer Size  (ms)>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will run CheckPBM to ensure "
            "a buffer underrun won't occur.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("raw2formatted") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Raw To Formatted\n\n"
            "    <Input File>\n"
            "    <OutPutDir>\n"
            "    <IVF/Y4M>\n"
            "    <Width>\n"
            "    <Height>\n"
            "    <FrameRate>\n"
            "    <FourCC>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw file and "
            "produce a raw IVF or Y4M file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("formatted2raw") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Formatted To Raw \n\n"
            "    <Input File>\n"
            "    <Output File>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an IVF or Y4M "
            "file and produce a raw file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("formatted2rawframes") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Formatted To RawFrames\n\n"
            "    <Input File>\n"
            "    <OutPutDir>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an IVF or Y4M "
            "file and produce individual raw frames for each frame that "
            "exists in a user-specified directory.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("combineindvframes") == 0)
    {
        tprintf(PRINT_STD, "\n  Combine Indv Frames");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will combine all individual "
            "decoded frames in a directory into a single raw file in "
            "numerical order.");

        tprintf(PRINT_STD,
            "\n"
            "   Usage:\n"
            "\n"
            "    <Input directory>\n"
            "    <Name Base>\n"
            "    <File Extension - include .>\n"
            "    <First Frame - Include preceding zeros>\n"
            "    <Last  Frame - Include preceding zeros>\n"
            "    <Output File>\n"
            "\n"
            "\n"
            "     Example: \n"
            "\n"
            "       First File Name 1: img-720x480-0003.i420\n"
            "       Last File Name 2: img-720x480-0223.i420\n"
            "\n"
            "       Input directory = directory files are located in\n"
            "       Name Base = img-720x480-\n"
            "       File Extension = .i420\n"
            "       First Frame = 0003\n"
            "       Last Frame = 0223\n"
            "       Output File = Path to desired output file\n");

#if !defined(_WIN32)
        tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("compareenc") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Compare Enc\n\n"
            "     <inputfile1>\n"
            "     <inputfile2>\n"
            "     <full 1/0>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will compare the video "
            "content of two IVF or WEBM files and will display if they are "
            "identical or, if they differ, the first frame they differ "
            "at.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("compareheader") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Compare Header Info \n\n"
            "    <inputfile1>\n"
            "    <inputfile2>\n"
            "    <Full File Info 1/yes-0/no>\n"
            "    <OutputFile>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will compare the file and "
            "frame headers of two IVF or WEBM files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("displayheader") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Display Header Info \n\n"
            "    <Input File>\n"
            "    <Full File Info 1/yes-0/no>\n"
            "    <Optional Outputfile>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the file and "
            "frame headers of an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("dispkeyframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Display Key Frames \n\n"
            "    <Input IVF File>\n"
            "    <Write to file 0 | 1 Print to screen>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location "
            "of key frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("dispresizedframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Display Resized Frames \n\n"
            "    <Input IVF File>\n"
            "    <Write to file 0 | 1 Print to screen>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location "
            "of resized frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("dispvisibleframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Display Visible Frames \n\n"
            "    <Input IVF File>\n"
            "    <Write to file 0 | 1 Print to screen>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location "
            "of visible frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("dispaltrefframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Display AltRef Frames \n\n"
            "    <Input IVF File>\n"
            "    <Write to file 0 | 1 Print to screen>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location "
            "of alternate reference frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("croprawclip") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Crop Raw Clip\n\n"
            "    <Input File>\n"
            "    <outputfile>\n"
            "    <xoffset>\n"
            "    <yoffset>\n"
            "    <New Frame Width>\n"
            "    <New Frame Height>\n"
            "    <Raw/IVF 0-Raw 1-IVF>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw IVF or "
            "Y4M file and produce a cropped raw IVF or Y4M file using the "
            "input size parameters.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("padrawclip") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Pad Raw Clip\n\n"
            "    <Input File>\n"
            "    <outputfile>\n"
            "    <New Frame Width>\n"
            "    <New Frame Height>\n"
            "    <Raw/IVF 0-Raw 1-IVF>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw IVF or "
            "Y4M file and produce a padded raw IVF or Y4M file using the "
            "input size parameters.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("cutclip") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Cut Clip\n\n"
            "    <Input File>\n"
            "    <outputfile>\n"
            "    <Starting Frame>\n"
            "    <Ending Frame>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will cut a portion of an "
            "IVF or Y4M file starting at Starting Frame and ending at "
            "Ending Frame to a new output file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("pasteclip") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Paste Clip\n\n"
            "    <Inputfile1>\n"
            "    <Inputfile2>\n"
            "    <Outputfile>\n"
            "    <First Paste Frame>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will paste the contents of "
            "Inputfile2 into Inputfile1 starting at Inputfile1's First "
            "Paste Frame to a new output file for raw IVF and Y4M files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("playdecivf") == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD,
            "\n  Play Dec IVF\n\n"
            "    <Input File>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will convert an "
            "uncompressed IVF or Y4M file to a raw YUV file and play it "
            "using tmnplay or mplayer.  Please be sure to put a copy of "
            "tmnplay in your C:\\bin directory.  Tmnplay can be found in "
            "the TestClips directory.");

#else
        tprintf(PRINT_STD, "\n  PlayDecIVF");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This Tool will convert an "
            "uncompressed ivf file to a raw yuv file and play it using "
            "mplayer.  To install mplayer open a command console and "
            "type:");

        tprintf(PRINT_STD, "\n    svn checkout svn://svn.mplayerhq.hu/mplayer/"
            "trunk mplayer\n"
            "    cd mplayer\n"
            "    ./configure\n"
            "    make\n"
            "    make install\n");

        tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("playcompivf") == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD,
            "\n  Play Comp IVF\n\n"
            "    <Input File>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will convert a compressed "
            "IVF or WEBM file to a raw YUV file and play it using tmnplay "
            "or mplayer.  Please be sure to put a copy of tmnplay in your "
            "C:\\bin directory.  Tmnplay can be found in the TestClips "
            "directory.");

#else
        tprintf(PRINT_STD, "\n  PlayCompIVF");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This Tool will convert a compressed "
            "ivf file to a raw yuv file and play it using mplayer.  To "
            "install mplayer open a command console and type:");

        tprintf(PRINT_STD, "\n    svn checkout svn://svn.mplayerhq.hu/mplayer/"
            "trunk mplayer\n"
            "    cd mplayer\n"
            "    ./configure\n"
            "    make\n"
            "    make install\n");

        tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("createsampletextfiles") == 0)
    {
        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will create sample text "
            "files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("printversion") == 0)
    {
        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will print the version of "
            "VP8 being used by the tester.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("vpxencpar") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Convert Parameter File to vpxenc settings\n\n"
            "     <input core file>\n"
            "     <input api file>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will convert tester "
            "parameters into vpxenc parameters.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("randparfile") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Random Par File\n\n"
            "    <Output Par File>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will create a valid VP8 "
            "random parameter file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("randcompress") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Random Compress\n\n"
            "    <Input File>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will create a compression "
            "using random parameters for an input IVF or Y4M file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (input_str.compare("graphpsnr") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Graph PSNR\n\n"
            "    <Inputfile 1>\n"
            "    <Starting Bit Rate>\n"
            "    <Ending Bit Rate>\n"
            "    <Bit Rate Step>\n"
            "    <Optional - Par Input>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool creates a series of "
            "compressions from user-input values for starting bit rate, "
            "ending bit rate, and an increment. Output files are created, "
            "and the utility computes and outputs the data rates and PSNRs "
            "of the files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    /////////////////////////// Hidden Commands ////////////////////////////////

    if (input_str.compare("copyalltxtfiles") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Copy All Txt Files\n\n"
            "    <Input directory>\n"
            "    <Output directory>\n"
            );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }
    if (input_str.compare("dispdropedframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Display Dropped Frames \n\n"
            "    <Input IVF File>\n"
            "    <Write to file 1 | 0 Print to screen>\n"
            "\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }
    if (input_str.compare("dispframedata") == 0)
    {
        tprintf(PRINT_STD, "\n"
            "  Display Frame Data \n\n"
            "    <Input IVF File>\n"
            "\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }

    if (input_str.compare("writeivffileheader") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Write IVF File Header\n\n"
            "     <Output File>\n"
            "     <Width>\n"
            "     <Height>\n"
            "     <Scale>\n"
            "     <Rate>\n"
            "     <Length>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }

    if (input_str.compare("writeivfframeheader") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Write IVF Frame Header\n\n"
            "     <Output File>\n"
            "     <Time Stamp>\n"
            "     <Frame Size>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }

    if (input_str.compare("writeframedata") == 0)
    {
        tprintf(PRINT_STD,
            "\n  Write IVF Frame Data\n\n"
            "     <Input File>\n"
            "     <Output File>\n"
            "     <Frame Size>\n");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }

    return 0;
}

void format_summary(const char *ptr_input_file_name)
{
    tprintf(PRINT_STD, "\n Formating Summary file.\n");

    std::string input_file_name = ptr_input_file_name;

    std::string sum_by_test_output;
    vpxt_remove_file_extension(input_file_name.c_str(), sum_by_test_output);
    sum_by_test_output += "expanded.txt";

    std::string tests_run;
    vpxt_folder_name(ptr_input_file_name, &tests_run);
    tests_run += "tests_run.txt";

    FILE *fp;

    if ((fp = freopen(sum_by_test_output.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open summary out put file: %s\n",
            sum_by_test_output.c_str());
        return;
    }

    std::string line_buf;
    std::string test_track_name;
    std::string test_track_result;

    int test_track = 1;
    int result_track = 0;

    // loop through all existing tests alphabeticaly
    while (test_track <= MAXTENUM)
    {
        result_track = 0;

        bool print_test_header_1 = 1;

        // This iterates through all possible tests run using the test tracker
        // names to specify which was run the number is only to drive
        // The iteration through and does not represent test number
        if (test_track == kTestAllowDropFrames)
            test_track_name = "Test_Allow_Drop_Frames";

        if (test_track == kTestAllowLag)
            test_track_name = "Test_Allow_Lag";

        if (test_track == kTestAllowSpatialResampling)
            test_track_name = "Test_Allow_Spatial_Resampling";

        if (test_track == kTestArnr)
            test_track_name = "Test_Arnr";

        if (test_track == kTestAutoKeyFrame)
            test_track_name = "Test_Auto_Key_Frame";

        if (test_track == kTestBufferLevel)
            test_track_name = "Test_Buffer_Level";

        if (test_track == kTestChangeCpuDec)
            test_track_name = "Test_Change_Cpu_Dec";

        if (test_track == kTestChangeCpuEnc)
            test_track_name = "Test_Change_Cpu_Enc";

        if (test_track == kTestConstrainedQuality)
            test_track_name = "Test_Constrained_Quality";

        if (test_track == kTestCopySetReference)
            test_track_name = "Test_Copy_Set_Reference";

        if (test_track == kTestDataRate)
            test_track_name = "Test_Data_Rate";

        if (test_track == kTestDebugMatchesRelease)
            test_track_name = "Test_Debug_Matches_Release";

        if (test_track == kTestDropFrameWaterMark)
            test_track_name = "Test_Drop_Frame_Watermark";

        if (test_track == kTestEncoderBreakout)
            test_track_name = "Test_Encoder_Break_Out";

        if (test_track == kTestErrorConcealment)
            test_track_name = "Test_Error_Concealment";

        if (test_track == kTestErrorResolution)
            test_track_name = "Test_Error_Resolution";

        if (test_track == kTestExtraFile)
            test_track_name = "Test_Extra_File";

        if (test_track == kTestFixedQuantizer)
            test_track_name = "Test_Fixed_Quantizer";

        if (test_track == kTestForcedKeyFrame)
            test_track_name = "Test_Force_Key_frame";

        if (test_track == kTestFrameSize)
            test_track_name = "Test_Frame_Size";

        if (test_track == kTestGoodVsBest)
            test_track_name = "Test_Good_Vs_Best";

        if (test_track == kTestLagInFrames)
            test_track_name = "Test_Lag_In_Frames";

        if (test_track == kTestMaxQuantizer)
            test_track_name = "Test_Max_Quantizer";

        if (test_track == kTestMemLeak)
            test_track_name = "Test_Mem_Leak";

        if (test_track == kTestMemLeak2)
            test_track_name = "Test_Mem_Leak2";

        if (test_track == kTestMinQuantizer)
            test_track_name = "Test_Min_Quantizer";

        if (test_track == kTestMultiResolutionEncode)
            test_track_name = "Test_Multiple_Resolution_Encode";

        if (test_track == kTestMultiThreadedDec)
            test_track_name = "Test_Multithreaded_Dec";

        if (test_track == kTestMultiThreadedEnc)
            test_track_name = "Test_Multithreaded_Enc";

        if (test_track == kTestNewVsOldPsnr)
            test_track_name = "Test_New_Vs_Old_Psnr";

        if (test_track == kTestNewVsOldTempScale)
            test_track_name = "Test_New_Vs_Old_Temp_Scale";

        if (test_track == kTestNewVsOldEncCpuTick)
            test_track_name = "Test_New_Vs_Old_Enc_Cpu_Tick";

        if (test_track == kTestNoiseSensitivity)
            test_track_name = "Test_Noise_Sensitivity";

        if (test_track == kTestOnePassVsTwoPass)
            test_track_name = "Test_One_Pass_Vs_Two_Pass";

        if (test_track == kTestPlayAlternate)
            test_track_name = "Test_Play_Alternate";

        if (test_track == kTestPostProcessor)
            test_track_name = "Test_Post_Processor";

        if (test_track == kTestReconstructBuffer)
            test_track_name = "Test_Reconstruct_Buffer";

        if (test_track == kTestResampleDownWatermark)
            test_track_name = "Test_Resample_Down_Watermark";

        if (test_track == kTestSpeed)
            test_track_name = "Test_Speed";

        if (test_track == kTestTemporalScalability)
            test_track_name = "Test_Test_Vector";

        if (test_track == kTestTestVector)
            test_track_name = "Test_Temporal_Scalability";

        if (test_track == kTestThirtytwoVsSixtyfour)
            test_track_name = "Test_Thirtytwo_Vs_Sixtyfour";

        if (test_track == kTestTwoPassVsTwoPassBest)
            test_track_name = "Test_Two_Pass_Vs_Two_Pass_Best";

        if (test_track == kTestUndershoot)
            test_track_name = "Test_Undershoot";

        if (test_track == kTestVersion)
            test_track_name = "Test_Version";

        if (test_track == kTestVersion)
            test_track_name = "Test_Vpx_Matches_Int";

        if (test_track == kTestWinLinMacMatch)
            test_track_name = "Test_Win_Lin_Mac_Match";

        int track_passed = 0;
        int track_failed = 0;
        int track_indter = 0;
        int track_combo_run = 0;
        int track_psnr_log = 0;
        int track_rand_test = 0;
        int track_comp_made = 0;
        int track_err_file_mismatch = 0;
        int track_test_not_supported = 0;
        int track_min_pass = 0;

        // loop though all possible results
        while (result_track < 13)
        {
            if (result_track == kTestPassed)
                test_track_result = "Passed";

            if (result_track == kTestFailed)
                test_track_result = "Failed";

            if (result_track == kTestIndeterminate)
                test_track_result = "Indeterminate";

            if (result_track == kTestComboLog)
                test_track_result = "SeeComboRunLog";

            if (result_track == kTestPsnrLog)
                test_track_result = "SeePSNRLog";

            if (result_track == kTestRandomComplete)
                test_track_result = "RandomTestCompleted";

            if (result_track == kTestError)
                test_track_result = "";

            if (result_track == kTestMinPassed)
                test_track_result = "track_min_pass";

            if (result_track == kTestEncCreated)
                test_track_result = "CompressionMade";

            if (result_track == kTestErrFileMismatch)
                test_track_result = "ErrorFileMismatch";

            if (result_track == kTestNotSupported)
                test_track_result = "TestNotSupported";

            std::fstream format_summary_by_test_file;
            format_summary_by_test_file.open(input_file_name.c_str());

            if (!format_summary_by_test_file)
            {
                tprintf(PRINT_STD, "Cannot open input file.");
                return;
            }

            int p = 0;
            bool print_test_header_2 = 1;

            // read in and throw away header
            while (p < 11)
            {
                // format_summary_by_test_file.getline(buffer, 1024);
                getline(format_summary_by_test_file, line_buf);
                p = p + 1;
            }

            while (!format_summary_by_test_file.eof() && line_buf[0] != '-')
            {
                getline(format_summary_by_test_file, line_buf);

                if (line_buf.length() > 60)
                {
                    unsigned int n = 5;

                    while (n < 39 && line_buf[n] != ' ')
                        n = n + 1;

                    std::string test_read_string = line_buf.substr(5, n - 5);

                    n = 62;

                    while (n < 194 && line_buf[n] != ' ' != 0 && n <
                        line_buf.length())
                        n = n + 1;

                    std::string test_result_string = line_buf.substr(62, n -
                        62);

                    // check to see if names and states match up
                    if (test_read_string.compare(test_track_name) == 0 &&
                        test_result_string.compare(test_track_result) == 0)
                    {
                        if (result_track == 1)
                            track_passed = track_passed + 1;

                        if (result_track == 0)
                            track_failed = track_failed + 1;

                        if (result_track == 2)
                            track_indter = track_indter + 1;

                        if (result_track == 3)
                            track_combo_run = track_combo_run + 1;

                        if (result_track == 4)
                            track_psnr_log = track_psnr_log + 1;

                        if (result_track == 8)
                            track_min_pass = track_min_pass + 1;

                        if (result_track == 10)
                            track_comp_made = track_comp_made + 1;

                        if (result_track == 11)
                            track_err_file_mismatch = track_err_file_mismatch +
                            1;

                        if (result_track == 12)
                            track_test_not_supported = track_test_not_supported
                            + 1;

                        if (print_test_header_1)
                        {
                            // print test header
                            int left_line_size = 75 - (test_read_string.size() /
                                2);
                            int counter = 0;

                            fprintf(stderr, "\n\n");

                            while (counter < left_line_size)
                            {
                                fprintf(stderr, "=");
                                counter = counter + 1;
                            }

                            fprintf(stderr, "%s", test_read_string.c_str());
                            counter = counter + test_read_string.size();

                            while (counter < 150)
                            {
                                fprintf(stderr, "=");
                                counter = counter + 1;
                            }

                            fprintf(stderr, "\n");
                            print_test_header_1 = 0;
                        }

                        if (print_test_header_2)
                        {
                            fprintf(stderr, "\n %s:\n\n",
                                test_result_string.c_str());
                            fprintf(stderr, "%4s %-32s%-25s%s  "
                                "Test Settings\n\n", "#", "            Test "
                                "Name" , "     Date and time", "Status");
                            print_test_header_2 = 0;
                        }

                        fprintf(stderr, "%s ", line_buf.c_str());

                        //// Get the correct Test Input settings and output ////
                        //// them to summary ///////////////////////////////////
                        std::fstream tests_run_file;
                        tests_run_file.open(tests_run.c_str(),
                            std::fstream::in);

                        if (!tests_run_file)
                        {
                            tests_run_file.close();
                            format_summary_by_test_file.close();
                            fclose(fp);
                            tprintf(PRINT_BTH, "\nFile creation Canceled: "
                                "Cannot find %s\n", tests_run.c_str());
                            return;
                        }

                        int test_run_num_int = 0;
                        std::stringstream TestRunNum(line_buf.substr(0, 4));
                        TestRunNum >> test_run_num_int;

                        int test_num_cur = -1;

                        while (!tests_run_file.eof() && test_num_cur !=
                            test_run_num_int)
                        {
                            std::string tests_run_string;
                            getline(tests_run_file, tests_run_string);
                            std::string test = tests_run_string.substr(0, 1);

                            if (tests_run_string[0] == '+')
                                test_num_cur = test_num_cur + 1;

                            if (test_num_cur == test_run_num_int)
                            {
                                tests_run_string[0] = ' ';
                                unsigned int t = 0;

                                while (t < tests_run_string.length())
                                {
                                    if (tests_run_string[t] == '@')
                                        tests_run_string[t] = ' ';

                                    t = t + 1;
                                }

                                fprintf(stderr, "%s\n", tests_run_string.c_str());
                            }
                        }

                        tests_run_file.close();
                        ////////////////////////////////////////////////////////
                    }
                }
            }

            format_summary_by_test_file.close();
            result_track = result_track + 1;
        }

        int pass_or_fail_exist = 0;
        int compress_exist = 0;
        int print_output = 0;

        if (track_passed != 0)
        {
            pass_or_fail_exist = 1;
            print_output = 1;
        }

        if (track_failed != 0)
        {
            pass_or_fail_exist = 1;
            print_output = 1;
        }

        if (track_indter != 0)
        {
            pass_or_fail_exist = 1;
            print_output = 1;
        }

        if (track_combo_run != 0)
            print_output = 1;

        if (track_psnr_log != 0)
            print_output = 1;

        if (track_rand_test != 0)
            print_output = 1;

        if (track_comp_made != 0)
        {
            compress_exist = 1;
            print_output = 1;
        }

        if (track_err_file_mismatch != 0)
            print_output = 1;

        if (track_test_not_supported != 0)
            print_output = 1;

        if (track_min_pass != 0)
            print_output = 1;

        if (print_output == 1)
        {
            if (!compress_exist == 1)
            {
                fprintf(stderr, "\n\n\n%s Summary:\n\n",
                    test_track_name.c_str());
                fprintf(stderr, "Passed: %i\n", track_passed);
                fprintf(stderr, "Failed: %i\n", track_failed);
                fprintf(stderr, "Indeterminate: %i\n", track_indter);

                if (track_combo_run != 0)
                    fprintf(stderr, "track_combo_run: %i\n",
                    track_combo_run);

                if (track_psnr_log != 0)
                    fprintf(stderr, "track_psnr_log: %i\n", track_psnr_log);

                if (track_rand_test != 0)
                    fprintf(stderr, "track_rand_test: %i\n",
                    track_rand_test);

                if (track_comp_made != 0)
                    fprintf(stderr, "track_comp_made: %i\n",
                    track_comp_made);

                if (track_err_file_mismatch != 0)
                    fprintf(stderr, "track_err_file_mismatch: %i\n",
                    track_err_file_mismatch);

                if (track_test_not_supported != 0)
                    fprintf(stderr, "track_test_not_supported: %i\n",
                    track_test_not_supported);

                if (track_min_pass != 0)
                    fprintf(stderr, "track_min_pass: %i\n", track_min_pass);
            }
            else
            {
                fprintf(stderr, "\n\n\n%s Summary:\n\n",
                    test_track_name.c_str());

                if (track_passed != 0)
                    fprintf(stderr, "Passed: %i\n", track_passed);

                if (track_failed != 0)
                    fprintf(stderr, "Failed: %i\n", track_failed);

                if (track_indter != 0)
                    fprintf(stderr, "Indeterminate: %i\n", track_indter);

                if (track_combo_run != 0)
                    fprintf(stderr, "track_combo_run: %i\n",
                    track_combo_run);

                if (track_psnr_log != 0)
                    fprintf(stderr, "track_psnr_log: %i\n", track_psnr_log);

                if (track_rand_test != 0)
                    fprintf(stderr, "track_rand_test: %i\n",
                    track_rand_test);

                if (track_comp_made != 0)
                    fprintf(stderr, "track_comp_made: %i\n",
                    track_comp_made);

                if (track_err_file_mismatch != 0)
                    fprintf(stderr, "track_err_file_mismatch: %i\n",
                    track_err_file_mismatch);

                if (track_test_not_supported != 0)
                    fprintf(stderr, "track_test_not_supported: %i\n",
                    track_test_not_supported);

                if (track_min_pass != 0)
                    fprintf(stderr, "track_min_pass: %i\n", track_min_pass);
            }

            fprintf(stderr, "\n\n");
        }

        test_track = test_track + 1;
    }

    fclose(fp);
    return;
}

int vpxt_run_multi(int argc, const char** argv, std::string working_dir)
{
    if (argc < 4)
        return vpxt_test_help(argv[1], 0);

    int valid_input = 0;

    if (atoi(argv[2]) != kResumeTest && atoi(argv[2]) != kTestOnly)
    {
        valid_input = vpxt_run_multiple_tests_input_check(argv[3], 0);

        if (valid_input <= 0)
        {
            tprintf(PRINT_STD, "\nExternal Test Runner Aborted\n\n");
            return 0;
        }
    }
    else
    {
        std::string input_ver;
        input_ver += argv[3] + slashCharStr() + "tests_run.txt";
        valid_input = vpxt_run_multiple_tests_input_check(input_ver.c_str(), 0);

        if (valid_input <= 0)
        {
            tprintf(PRINT_STD, "\nExternal Test Runner Aborted\n\n");
            return 0;
        }
    }

    // check to see if we should run a lean multi run
    int delete_ivf_files = 0;
    int artifact_detection = kDontRunArtifactDetection;

    int n = 4;
    while (n < argc)
    {
        std::string option_check = argv[n];
        vpxt_lower_case_string(option_check);

        if (option_check.compare("lean") == 0)
            delete_ivf_files = 1;

        if (option_check.compare("artifact_detect") == 0)
            artifact_detection = kRunArtifactDetection;

        ++n;
    }

    run_multiple_tests(argc, argv, working_dir.c_str(), valid_input,
        delete_ivf_files, artifact_detection);

    std::string summary_file = working_dir;

    if (atoi(argv[2]) == kFullTest)
    {
        char summary_file_char[255] = "";
        summary_file.erase(summary_file.end() - 1);
        summary_file += slashCharStr() + "test_results.txt";
        snprintf(summary_file_char, 255, "%s", summary_file.c_str());
        format_summary(summary_file_char);
        return 0;
    }

    if (atoi(argv[2]) == kTestOnly)
    {
        char summary_file_char[255] = "";
        summary_file.erase(summary_file.end() - 1);
        summary_file += slashCharStr() += "compression_results.txt";
        snprintf(summary_file_char, 255, "%s", summary_file.c_str());
        format_summary(summary_file_char);
        return 0;
    }

    if (atoi(argv[2]) == kResumeTest)
    {
        std::string sum_comp_and_test;
        std::string sum_comp;
        std::string sum_test;

        // Mode 1
        sum_comp_and_test = argv[3] + slashCharStr() + "test_results.txt";
        // Mode 2
        sum_comp = argv[3] + slashCharStr() + "compression_results.txt";
        // Mode 3
        sum_test = argv[3] + slashCharStr() + "compression_test_results.txt";

        int test_running = kNoTest;

        // if sum comp and test file exists then full test
        std::fstream sum_comp_and_testFile;
        sum_comp_and_testFile.open(sum_comp_and_test.c_str());

        if (sum_comp_and_testFile)
            test_running = kFullTest;

        // if sum test file exists then test only
        std::fstream sum_testFile;
        sum_testFile.open(sum_test.c_str());

        if (sum_testFile)
            test_running = kTestOnly;

        // if sum comp file exits and not test only then comp only
        std::fstream sum_compFile;
        sum_compFile.open(sum_comp.c_str());

        if (sum_compFile)
            if (test_running != kTestOnly)
                test_running = kCompOnly;

        sum_comp_and_testFile.close();
        sum_testFile.close();
        sum_compFile.close();

        summary_file = argv[3];

        if (test_running == kFullTest)
        {
            summary_file += slashCharStr() + "test_results.txt";
            format_summary(summary_file.c_str());
            return 0;
        }

        if (test_running == kTestOnly)
        {
            summary_file += slashCharStr() + "compression_test_results.txt";
            format_summary(summary_file.c_str());
        }
    }

    return 0;
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        vpxt_on_error_output();
        return 0;
    }

    const std::string sub_folder_str;
    char working_dir_char[256]  = "";
    create_working_folder(argc, argv, working_dir_char);
    std::string working_dir = working_dir_char;
    std::string input_1_str(argv[1]);
    std::string input_2_str;

    if (argc > 2)
        input_2_str =  argv[2];

    vpxt_lower_case_string(input_1_str);
    vpxt_lower_case_string(input_2_str);

    if (input_1_str.compare("0") == 0)
    {
        if (working_dir.length() > 175)
        {
            tprintf(PRINT_BTH, "\nError: folder Path Length too long.\n");
            return 0;
        }
    }

    /////////////////////////// Help ///////////////////////////////////////////

    if (input_1_str.compare("help") == 0)
    {
        vpxt_on_error_output();
        vpxt_formated_print(TOLPRT, "To run tests input the number to the left "
            "of the test name along with test specific input.  To obtain more "
            "information about a specific test or utility input the number of "
            "the test or the utility name you wish to inquire about followed "
            "by \"help\".");
        return 0;
    }

    if (input_2_str.compare("help") == 0)
    {
        int ToolHelpCheck = vpxt_tool_help(input_1_str.c_str(), 1);
        if (!ToolHelpCheck)
            vpxt_test_help(argv[1], 1);

        return 0;
    }

    ////////////////////Public Commands/////////////////////////
    // vpxenc clone
    if (input_1_str.compare("vpxenc") == 0)
        return tool_run_ivfenc(argc, argv);

    // vpxdec clone
    if (input_1_str.compare("vpxdec") == 0)
        return tool_run_ivfdec(argc, argv);

    // Compresses an IVF Raw File to an IVF Compressed file
    if (input_1_str.compare("vpxtenc") == 0)
        return tool_vpxt_enc(argc, argv, working_dir);

    // Decompresses an IVF Compressed file to an IVF Raw File
    if (input_1_str.compare("vpxtdec") == 0)
        return tool_vpxt_dec(argc, argv);

    // Vp8 Scalable Patterns clone
    if (input_1_str.compare("vpxtempscaleenc") == 0)
        return tool_vp8_scalable_patterns(argc, argv);

    // vpxt Multi Resolution Encode
    if (input_1_str.compare("vpxttempscaleenc") == 0)
        return tool_vpxt_temp_scale_enc(argc, argv);

    // Multi Resolution Encode
    if (input_1_str.compare("vpxmultiresenc") == 0)
        return tool_multi_res_enc(argc, argv);

    // vpxt Multi Resolution Encode
    if (input_1_str.compare("vpxtmultiresenc") == 0)
        return tool_vpxt_multi_res_enc(argc, argv);

    // Decompresses an IVF Compressed file to an IVF Raw File WITH RESIZE
    if (input_1_str.compare("vpxtdecresz") == 0)
        return tool_vpxt_dec_resize(argc, argv);

    // Decompresses an IVF Compressed file to an IVF Raw File ERROR CONCEALMENT
    if (input_1_str.compare("vpxtdecpard") == 0)
        return tool_vpxt_dec_part_drop(argc, argv);

    // Decodes IVF File to Raw File
    if (input_1_str.compare("vpxtdec2raw") == 0)
        return tool_vpxt_dec_to_raw(argc, argv);

    // Computes Data Rate for an input IVF File
    if (input_1_str.compare("vpxtdatarate") == 0)
        return tool_vpxt_data_rate(argc, argv);

    // Computes PSNR for two input files
    if (input_1_str.compare("vpxtpsnr") == 0)
        return tool_vpxt_psnr_run(argc, argv);

    // Computes Decoded PSNR for two input files
    if (input_1_str.compare("vpxtpsnrdec") == 0)
        return tool_vpxt_psnr_run_dec(argc, argv);

    //Computes PBM For an input file
    if (input_1_str.compare("vpxtcheckpbm") == 0)
        return tool_vpxt_check_pbm_run(argc, argv);

    // Converts an IVF File to a Raw File
    if (input_1_str.compare("raw2formatted") == 0)
        return tool_raw_to_formatted(argc, argv);

    // Converts an ivf file to a raw file
    if (input_1_str.compare("formatted2raw") == 0)
        return tool_formatted_to_raw(argc, argv);

    // Converts an ivf file to a raw file
    if (input_1_str.compare("formatted2rawframes") == 0)
        return tool_formatted_to_raw_frames(argc, argv);

    // Compares two ivf files
    if (input_1_str.compare("compareenc") == 0)
        return tool_compare_enc(argc, argv);

    // Compares the Headers of two ivf files
    if (input_1_str.compare("compareheader") == 0)
        return tool_compare_header_info(argc, argv);

    // Displays the header contents of an ivf file
    if (input_1_str.compare("displayheader") == 0)
        return tool_display_header_info(argc, argv);

    // Displays which frames are key frames for an input compressed ivf file
    if (input_1_str.compare("dispkeyframes") == 0)
        return tool_disp_key_frames(argc, argv);

    // Compresses an IVF Raw File to an IVF Compressed file
    if (input_1_str.compare("dispresizedframes") == 0)
        return tool_disp_resized_frames(argc, argv);

    // Compresses an IVF Raw File to an IVF Compressed file
    if (input_1_str.compare("dispvisibleframes") == 0)
        return tool_disp_visible_frames(argc, argv);

    // Compresses an IVF Raw File to an IVF Compressed file
    if (input_1_str.compare("dispaltrefframes") == 0)
        return tool_disp_alt_ref_frames(argc, argv);

    // Modifies an ivf file croping part of the clip
    if (input_1_str.compare("croprawclip") == 0)
        return tool_crop_raw_clip(argc, argv);

    // Modifies an ivf file by padding it
    if (input_1_str.compare("padrawclip") == 0)
        return tool_pad_raw_clip(argc, argv);

    // Modifies an ivf file cutting out a portion of the clip and saving it
    if (input_1_str.compare("cutclip") == 0)
        return tool_cut_ivf(argc, argv);

    // Modifies an ivf file by pasting a clip into it
    if (input_1_str.compare("pasteclip") == 0)
        return tool_paste_clip(argc, argv);

    // Plays a Decoded ivf file (Converts the file to a Raw file and uses
    // tmnplay.exe to play the file)
    if (input_1_str.compare("playdecivf") == 0)
        return tool_play_dec_ivf(argc, argv);

    // Plays a compressed ivf file (Decodes the file to an ivf then converts
    // that ivf to a raw then used tnmplay.exe to play that file.)
    if (input_1_str.compare("playcompivf") == 0)
        return tool_play_comp_ivf(argc, argv);

    // Creates sample text files that include quick test and other templates
    if (input_1_str.compare("createsampletextfiles") == 0)
        return print_quick_test_files(working_dir);

    // Prints the internal version number - make sure all libs built in same
    // multithreadedmode - properties  C++ CodeGen RuntimeLibrary
    if (input_1_str.compare("printversion") == 0)
        return print_version();

    // converts Tester Parameter file to a vpxenc ParFile
    if (input_1_str.compare("vpxencpar") == 0)
        return tool_convert_par_file_to_vpxenc(argc, argv);

    // creates a random valid parameter file
    if (input_1_str.compare("randparfile") == 0)
        return tool_create_rand_par_file(argc, argv);

    // runs a compression using random configuration
    if (input_1_str.compare("randcompress") == 0)
        return tool_rand_comp(argc, argv, working_dir, sub_folder_str, 1);

    // creates a set of psnr to data rate data points
    if (input_1_str.compare("graphpsnr") == 0)
        return tool_graph_psnr(argc, argv, working_dir, sub_folder_str, 1);

    //////////////////// Hidden Commands /////////////////////////
    // Shows Hidden Commands
    if (input_1_str.compare("showhidden") == 0)
        return show_hidden_cmds();

    // runs threshold check (this needs to be looked at)
    if (input_1_str.compare("thresh") == 0)
        return tool_run_thresh(argc, argv);

    // Displays which frames are key frames for an input compressed ivf file
    if (input_1_str.compare("dispframedata") == 0)
        return tool_disp_frame_data(argc, argv);

    // displays dropped frames
    if (input_1_str.compare("dispdropedframes") == 0)
        return tool_disp_droped_frames(argc, argv);

    // gets the number of frames a file has
    if (input_1_str.compare("getframecount") == 0)
        return vpxt_get_number_of_frames(argv[2]);

    // Checks to make sure a Multi Run Input file has the correct format
    if (input_1_str.compare("testfileinputcheck") == 0)
        return vpxt_run_multiple_tests_input_check(argv[2], 1);

    // creates a set of random tests
    if (input_1_str.compare("randomstresstest") == 0)
        return tool_random_stress_test(argc, argv);

    // Formats a test results output file by test and result
    if (input_1_str.compare("formatsummary") == 0)
        return tool_format_summary(argc, argv);

    // Solves Quadradic
    if (input_1_str.compare("quad") == 0)
        return tool_solve_quad();

    // Compares two dec ivf files
    if (input_1_str.compare("comparedec") == 0)
        return tool_compare_dec(argc, argv);

    // print which instruction sets are available on machine
    if (input_1_str.compare("printcpuinfo") == 0)
        return tool_print_cpu_info();

    // Copies all text files in a directory to a new directory preserving file
    // structure
    if (input_1_str.compare("copyalltxtfiles") == 0)
        return tool_copy_all_txt_files(argc, argv);

    // combines as set of frames to a raw file
    if (input_1_str.compare("combineindvframes") == 0)
        return tool_combine_indv_frames(argc, argv);

    // write ivf file header data to a file
    if (input_1_str.compare("writeivffileheader") == 0)
        return tool_vpxt_write_ivf_file_header(argc, argv);

    // write ivf frame header data to a file
    if (input_1_str.compare("writeivfframeheader") == 0)
        return tool_vpxt_write_ivf_frame_header(argc, argv);

    // write frame data to a file
    if (input_1_str.compare("writeframedata") == 0)
        return tool_vpxt_write_frame_data(argc, argv);

    tprintf(PRINT_STD, "\n");

    int selector = vpxt_identify_test(argv[1]);
    if (selector > MAXTENUM || selector < 0)
    {
        vpxt_on_error_output();
        return 0;
    }

    if (selector == kTestMultiRun)
        return vpxt_run_multi(argc, argv, working_dir);

    if (selector == kTestAllowDropFrames)
        return test_allow_drop_frames(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestAllowLag)
        return test_allow_lag(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestAllowSpatialResampling)
        return test_allow_spatial_resampling(argc, argv, working_dir,
        sub_folder_str, 1 , KEEP_IVF, NO_ART_DET);

    if (selector == kTestArnr)
        return test_arnr(argc, argv, working_dir, sub_folder_str, 1, KEEP_IVF,
        NO_ART_DET);

    if (selector == kTestAutoKeyFrame)
        return test_auto_key_frame(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestBufferLevel)
        return test_buffer_level(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestChangeCpuDec)
        return test_change_cpu_dec(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestChangeCpuEnc)
        return test_change_cpu_enc(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestConstrainedQuality)
        return test_constrained_quality(argc, argv, working_dir, sub_folder_str
        , 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestCopySetReference)
        return test_copy_set_reference(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestDropFrameWaterMark)
        return test_drop_frame_watermark(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestDataRate)
        return test_data_rate(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestDebugMatchesRelease)
        return test_debug_matches_release(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestEncoderBreakout)
        return test_encoder_break_out(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestErrorConcealment)
        return test_error_concealment(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestErrorResolution)
        return test_error_resolution(argc, argv, working_dir, sub_folder_str, 1
        , KEEP_IVF, NO_ART_DET);

    if (selector == kTestExtraFile)
        return test_extra_file(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestFixedQuantizer)
        return test_fixed_quantizer(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestForcedKeyFrame)
        return test_force_key_frame(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestFrameSize)
        return test_frame_size(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestGoodVsBest)
        return test_good_vs_best(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestLagInFrames)
        return test_lag_in_frames(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestMaxQuantizer)
        return test_max_quantizer(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestMemLeak)
        return test_mem_leak(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestMemLeak2)
        return test_mem_leak2(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestMinQuantizer)
        return test_min_quantizer(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestMultiResolutionEncode)
        return test_multiple_resolution_encode(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestMultiThreadedDec)
        return test_multithreaded_dec(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestMultiThreadedEnc)
        return test_multithreaded_enc(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestNewVsOldPsnr)
        return test_new_vs_old_psnr(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestNewVsOldTempScale)
        return test_new_vs_old_temp_scale(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestNewVsOldEncCpuTick)
        return test_new_vs_old_enc_cpu_tick(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestNoiseSensitivity)
        return test_noise_sensitivity(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestOnePassVsTwoPass)
        return test_one_pass_vs_two_pass(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestPlayAlternate)
        return test_play_alternate(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestPostProcessor)
        return test_post_processor(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestPostProcessorMfqe)
        return test_post_processor_mfqe(argc, argv, working_dir, sub_folder_str
        , 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestReconstructBuffer)
        return test_reconstruct_buffer(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestResampleDownWatermark)
        return test_resample_down_watermark(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestSpeed)
        return test_speed(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestTemporalScalability)
        return test_temporal_scalability(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestTestVector)
        return test_test_vector(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestThirtytwoVsSixtyfour)
        return test_thirtytwo_vs_sixtyfour(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestTwoPassVsTwoPassBest)
        return test_two_pass_vs_two_pass_best(argc, argv, working_dir,
        sub_folder_str, 1, KEEP_IVF, NO_ART_DET);

    if (selector == kTestUndershoot)
        return test_undershoot(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestVersion)
        return test_version(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestVpxMatchesInt)
        return test_vpx_matches_int(argc, argv, working_dir, sub_folder_str, 1,
        KEEP_IVF, NO_ART_DET);

    if (selector == kTestWinLinMacMatch)
        return test_win_lin_mac_match(argc, argv, working_dir, sub_folder_str,
        1, KEEP_IVF, NO_ART_DET);

    vpxt_on_error_output();
    return 0;
}