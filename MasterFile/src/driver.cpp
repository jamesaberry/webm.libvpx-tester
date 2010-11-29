#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_test_definitions.h"
#include "vpxt_test_list.h"
#include "utilities.h"
#include "driver.h"
#include "tools.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#if defined(_WIN32)
#include <windows.h>
#include <tchar.h>
#include <direct.h>
#define snprintf _snprintf
#else
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#endif

//CodeCoverage
extern int vpxt_array_coverage(int argc, char *argv[], string WorkingDir, string FilesAr[]);

int create_working_folder(int argc, char *argv[], char *WorkingDirChar)
{
    ///////////////////////////////////////////Create Working Folder////////////////////////////////////

    //Get Date and time info and convert it to a string removing colons in time
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    string DateAndTime = asctime(timeinfo);

    //remove colons in time string
    char DateAndTimeCharArray[255];
    int w = 0;

    while (DateAndTime.c_str()[w] != '\n')
    {
        if (DateAndTime.c_str()[w] == ':' || DateAndTime.c_str()[w] == ' ')
        {
            DateAndTimeCharArray[w] = '_';
        }
        else
        {
            DateAndTimeCharArray[w] = DateAndTime.c_str()[w];
        }

        w++;
    }

    DateAndTimeCharArray[w] = '\0';
    string DateAndTime3 = DateAndTimeCharArray;

    //Get Dir Folder
    char Folder3[255];
    char Folder2[255];

    vpxt_folder_name(argv[0], Folder3);
    snprintf(Folder2, 255, "%s", Folder3);
    string Folder = Folder2;

    //add Date and Time
    int number = 0;

    Folder.append(DateAndTime3);
    string FolderCheck = Folder;

    while (vpxt_folder_exist_check(FolderCheck)) //Make sure folder doesnt already exist
    {
        number++;
        char numberChar[255];
        vpxt_itoa_custom(number, numberChar, 10);

        FolderCheck = Folder;
        FolderCheck.append("_");
        FolderCheck.append(numberChar);
    }

    if (number != 0) //append sub number to end of folder name
    {
        char numberChar[255];
        vpxt_itoa_custom(number + 1, numberChar, 10);

        Folder.append("_");
        Folder.append(numberChar);
    }

    Folder.append("\"");
    Folder.append("\0");
    snprintf(WorkingDirChar, 255, "%s", Folder.c_str());

    return 0;
}
string date_string()
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    string DateAndTime = asctime(timeinfo);

    //remove colons in time string
    char DateAndTimeCharArray[255];
    int w = 0;

    while (DateAndTime.c_str()[w] != '\n')
    {
        if (DateAndTime.c_str()[w] == ':' || DateAndTime.c_str()[w] == ' ')
        {
            DateAndTimeCharArray[w] = '_';
        }
        else
        {
            DateAndTimeCharArray[w] = DateAndTime.c_str()[w];
        }

        w++;
    }

    DateAndTimeCharArray[w] = '\"';
    DateAndTimeCharArray[w+1] = '\0';
    string DateAndTime3(DateAndTimeCharArray);

    return DateAndTime3;

}
void vpxt_on_error_output()
{
    /*printf("\n"
           "Usage: Tester.exe <options> \n"
           "\n"
           "  <tests options>         \n"
           "                          \n"
           "   (0) run_multiple_tests <test type> <input file>\n"
           "   (1) test_allow_drop_frames <input file> <compression mode> <target bit rate>\n"
           "   (2) test_allow_lag  <input file> <compression mode> <target bit rate>\n"
           "   (3) test_allow_spatial_resampling <input file> <compression mode> <target bit rate>\n"
           "   (4) test_auto_key_frame <input file> <compression mode> <target bit rate> <key frame frequency>\n"
           "   (5) test_buffer_level <input file> <compression mode> <target bit rate>\n"
           "   (6) test_change_cpu_dec <input file> <compression mode> <target bit rate> <version>\n"
           "   (7) test_change_cpu_enc <input file> <compression mode> <target bit rate> <version>\n"
           "   (8) test_data_rate <input file> <compression mode> <target bit rate>\n"
           "   (9) test_debug_matches_release <input file> <compression mode> <target bit rate> <debug.exe> <release.exe>\n"
           "  (10) test_drop_frame_watermark <input file> <compression mode> <target bit rate>\n"
           "  (11) test_encoder_break_out <input file> <compression mode> <target bit rate>\n"
           "  (12) test_error_resolution <input file> <compression mode> <target bit rate>\n"
           "  (13) test_extra_file <input file>\n"
           "  (14) test_fixed_quantizer <input file> <compression mode> <target bit rate> <fixed q1> <fixed q2>\n"
           "  (15) test_force_key_frame <input file> <compression mode> <target bit rate> <force key frame>\n"
           "  (16) test_frame_size <input file> <compression mode> <target bit rate> <starting width> <starting height>\n"
           "  (17) test_good_vs_best <input file> <target bit rate>\n"
           "  (18) test_lag_in_frames <input file> <compression mode> <target bit rate> <lag in frames 1> <lag in frames 2>\n"
           "  (19) test_max_quantizer <input file> <compression mode> <target bit rate>\n"
           "  (20) test_mem_leak <input file> <compression mode> <target bit rate> <debug.exe>\n"
           "  (21) test_mem_leak2 <debug.exe> <decode test files>\n"
           "  (22) test_min_quantizer <input file> <compression mode> <target bit rate>\n"
           "  (23) test_multithreaded <input file> <compression mode> <target bit rate> <number of cores to use>\n"
           "  (24) test_new_vs_old_psnr <input file> <compression mode> <target bit rate> <old_release.exe> <par version>\n"
           "  (25) test_new_vs_old_real_time_speed <input file> <target bit rate> <old_release.exe> <par version>\n"
           "  (26) test_noise_sensitivity <input file> <compression mode> <target bit rate>\n"
           "  (27) test_one_pass_vs_two_pass <input file> <target bit rate>\n"
           "  (28) test_play_alternate <input file> <compression mode> <target bit rate>\n"
           "  (29) test_post_processor <input file> <compression mode> <target bit rate>\n"
           "  (30) test_reconstruct_buffer <input file> <compression mode> <target bit rate>\n"
           "  (31) test_resample_down_watermark <input file> <compression mode> <target bit rate>\n"
           "  (32) test_speed <input file> <compression mode> <target bit rate> <lag in frames>\n"
           "  (33) test_test_vector <input directory>\n"
           "  (34) test_two_pass_vs_two_pass_best <input file> <target bit rate>\n"
           "  (35) test_undershoot <input file> <compression mode> <target bit rate>\n"
           "  (36) test_version <input file> <compression mode> <target bit rate>\n"
           "  (37) test_win_lin_mac_match <input file> <compression mode> <target bit rate> <common folder> <common settings>\n"
           "                                           \n"
          );*/

    printf("\n"
           "Usage: Tester.exe <options> \n"
           "\n"
           "  <tests options>                          <tool options>\n"
           "                                            \n"
           "   (0) run_multiple_tests                   IVFEnc\n"
           "   (1) test_allow_drop_frames               IVFDec\n"
           "   (2) test_allow_lag                       IVF2IVFCompr\n"
           "   (3) test_allow_spatial_resampling        IVF2IVFDec\n"
           "   (4) test_auto_key_frame                  IVF2RawDec\n"
           "   (5) test_buffer_level                    \n"
           "   (6) test_change_cpu_dec                  IVFDataRate\n"
           "   (7) test_change_cpu_enc                  IVFPSNR\n"
           "   (8) test_data_rate                       IVFCheckPBM\n"
           "   (9) test_debug_matches_release           \n"
           "  (10) test_drop_frame_watermark            Raw2IVF\n"
           "  (11) test_encoder_break_out               IVF2Raw\n"
           "  (12) test_error_resolution                IVF2RawFrames\n"
           "  (13) test_extra_file                      CombineIndvFrames\n"
           "  (14) test_fixed_quantizer                 \n"
           "  (15) test_force_key_frame                 CompareIVF\n"
           "  (16) test_frame_size                      CompIVFHeader\n"
           "  (17) test_good_vs_best                    DispIVFHeader\n"
           "  (18) test_lag_in_frames                   \n"
           "  (19) test_max_quantizer                   DispKeyFrames\n"
           "  (20) test_mem_leak                        DispResizedFrames\n"
           "  (21) test_mem_leak2                       DispVisibleFrames\n"
           "  (22) test_min_quantizer                   DispAltRefFrames\n"
           "  (23) test_multithreaded                   \n"
           "  (24) test_new_vs_old_psnr                 CropRawIVF\n"
           "  (25) test_new_vs_old_real_time_speed      CutIVF\n"
           "  (26) test_noise_sensitivity               PasteIVF\n"
           "  (27) test_one_pass_vs_two_pass            \n"
           "  (28) test_play_alternate                  PlayDecIVF\n"
           "  (29) test_post_processor                  PlayCompIVF\n"
           "  (30) test_reconstruct_buffer              \n"
           "  (31) test_resample_down_watermark         CreateSampleTextFiles\n"
           "  (32) test_speed                           PrintVersion\n"
           "  (33) test_test_vector                     \n"
           "  (34) test_two_pass_vs_two_pass_best       RandParFile\n"
           "  (35) test_undershoot                      RandIVFComp\n"
           "  (36) test_version                         GraphPSNR\n"
           "  (37) test_win_lin_mac_match               Help\n"
           "                                           \n"
          );
}
void write_32bit_quick_test(string WorkingDir)
{
    char FolderNameChar[255];
    vpxt_folder_name(WorkingDir.c_str(), FolderNameChar);
    string TextfileString5 = FolderNameChar;
    TextfileString5.append("QuickTest_32Bit.txt");

    FILE *fp5;

    if ((fp5 = fopen(TextfileString5.c_str(), "w")) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString5.c_str());
        exit(1);
    }

    string OSStr = "";

#if defined(_WIN32)
    OSStr = "Win";
#elif defined(__APPLE__)
    OSStr = "Mac";
#elif defined(linux)
    OSStr = "Lin";
#endif

    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%%              \n");
    fprintf(fp5, "%%              QuickTest %s32Bit\n", OSStr.c_str());
    fprintf(fp5, "%%              \n");
    fprintf(fp5, "%%              \n");
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf\n", "test_extra_file", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128\n", "test_good_vs_best", slashChar(), slashChar());
    fprintf(fp5, "%s@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@..%cTestClips%cMemLeakCheck2_Compression.ivf\n", "test_mem_leak2", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", "test_new_vs_old_real_time_speed", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128\n", "test_one_pass_vs_two_pass", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cTestVectors\n", "test_test_vector", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128\n", "test_two_pass_vs_two_pass_best", slashChar(), slashChar());
    fprintf(fp5, "%%%i@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA\n", "test_win_lin_mac_match", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2\n", "test_multithreaded", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2\n", "test_multithreaded", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@10\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@10\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode0%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@6\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode1%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128\n", "test_allow_drop_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1@..%cTestClips%cSpecialCaseParameterFiles%c1ChangeCPUWorksCPUUsage4.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode2%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode4%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@10@24\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode5%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@10@24\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    printf("\n\nQuick Test file created:\n%s\n\n", TextfileString5.c_str());
    fclose(fp5);
}
void write_64bit_quick_test(string WorkingDir)
{
    char FolderNameChar[255];
    vpxt_folder_name(WorkingDir.c_str(), FolderNameChar);
    string TextfileString5 = FolderNameChar;
    TextfileString5.append("QuickTest_64Bit.txt");

    FILE *fp5;

    if ((fp5 = fopen(TextfileString5.c_str(), "w")) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString5.c_str());
        exit(1);
    }

    string OSStr = "";

#if defined(_WIN32)
    OSStr = "Win";
#elif defined(__APPLE__)
    OSStr = "Mac";
#elif defined(linux)
    OSStr = "Lin";
#endif

    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%%              \n");
    fprintf(fp5, "%%              QuickTest %s64Bit\n", OSStr.c_str());
    fprintf(fp5, "%%              \n");
    fprintf(fp5, "%%              \n");
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf\n", "test_extra_file", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128\n", "test_good_vs_best", slashChar(), slashChar());
    fprintf(fp5, "%s@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@..%cTestClips%cMemLeakCheck2_Compression.ivf\n", "test_mem_leak2", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", "test_new_vs_old_real_time_speed", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128\n", "test_one_pass_vs_two_pass", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cTestVectors\n", "test_test_vector", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128\n", "test_two_pass_vs_two_pass_best", slashChar(), slashChar());
    fprintf(fp5, "%%%i@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA\n", "test_win_lin_mac_match", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2\n", "test_multithreaded", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2\n", "test_multithreaded", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@10\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@10\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode0%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@6\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode1%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128\n", "test_allow_drop_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1@..%cTestClips%cSpecialCaseParameterFiles%c1ChangeCPUWorksCPUUsage4.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode2%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode4%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@10@24\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode5%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@6\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@0\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@1\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@0\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@1\n", "test_change_cpu_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@288\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@20@60\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@24\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@176@144\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@10@24\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    printf("\n\nQuick Test file created:\n%s\n\n", TextfileString5.c_str());
    fclose(fp5);
}
void print_quick_test_files(string WorkingDir)
{
    write_32bit_quick_test(WorkingDir);//32BitQuickRun
    write_64bit_quick_test(WorkingDir);//64BitQuickRun

    return;
}
void vpxt_test_help(int argc, char *argv[], string WorkingDir)
{
    string TestInputString = argv[1];
    int selector = vpxt_identify_test(argv[1]);

    if (TestInputString.compare("0") == 0)
    {
        printf(
            "\nUse:\n\n"
            " 0 run_multipule_tests \n\n"
            "    <Test Type>\n"
            "         <1 - Create Files and Run Tests>\n"
            "         <2 - Create Files only>\n"
            "         <3 - Run Tests For Existing Files - Requires Input Test Directory>\n"
            "    <Input Text File>\n"
            "    <Optional - Input Test Directory>\n");

        vpxt_formated_print(HLPPRT, "The test runs tests from an input file and summarizes the results. The test can be run in four modes: Mode 1 - Create compressions and run tests, Mode 2 - Create compressions only, Mode 3 - Run tests on preexisting compressions, and Mode 4 - Resume tests in progress. To create a template driver text file use the command: CreateSampleTextFiles.");
    }

    if (selector == AlWDFNUM)
    {
        printf("\nUse:\n\n"
               "%2i AllowDF \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , AlWDFNUM);

        vpxt_formated_print(HLPPRT, "The test creates two files; the first with Drop Frames on, the second with Drop Frames off. It then records and compares the number of frames each file has.  If Drop Frames on has fewer frames than Drop Frames off; the test passes.");

    }

    if (selector == ALWLGNUM)
    {
        printf("\nUse:\n\n"
               "%2i AllowLag \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , ALWLGNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions; the first with Allow Lag equal to 0, the second with Allow Lag equal to 1.  If the correct number of lagged frames are detected via quantizer output, alternate reference frames exist for Allow Lag on, Allow Lag on has the same number of visible frames as Allow Lag off, and Allow Lag on is not identical to Allow Lag off; the test passes.  (Test Pass/Fail only valid for modes 4 and 5.)");
    }

    if (selector == ALWSRNUM)
    {
        printf("\nUse:\n\n"
               "%2i AllowSpatialResampling \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , ALWSRNUM);

        vpxt_formated_print(HLPPRT, "The test creates two files the first with Spatial Resampling off the second with Spatial Resampling on. The test then records the number of resized frames for each and computes the PSNR for Spatial Resampling on.  If the number of resized frames for Spatial Resampling on is greater than 0, the number of resized frames for Spatial Resampling off equals 0, and the PSNR calculated for Spatial Resampling on is greater than 15; the test passes.");
    }

    if (selector == AUTKFNUM)
    {
        printf("\nUse:\n\n"
               "%2i AutoKeyFramingWorks \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "     <Optional Settings File>\n"
               , AUTKFNUM);

        vpxt_formated_print(HLPPRT, "The test creates two files with identical parameters setting Auto Key Frame equal to 6. The test then records the placement of each files key frames.  If both files key frames occur in identical locations and at least as frequently as Auto Key Frame dictates; the test passes.");

    }

    if (selector == BUFLVNUM)
    {
        printf("\nUse:\n\n"
               "%2i BufferLevelWorks \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , BUFLVNUM);

        vpxt_formated_print(HLPPRT, "The test creates a compression and runs CheckPBM on it.  If no buffer under run is detected; the test passes.");

    }

    if (selector == CPUDENUM)
    {
        printf("\nUse:\n\n"
               "%2i CPUDecOnlyWorks \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Version>\n"
               "     <Optional Settings File>\n"
               , CPUDENUM);

        vpxt_formated_print(HLPPRT, "The test creates a compression of the user input version (0-3) and then decompresses it for ON2_SIMD_CAPS values ranging from 0 to 63 in ((x+1)*2)-1 increments for all supported instruction sets (NONE, MMX, SSE, SSE2, SSE3, SSSE3, SSE4_1). The test then compares the decompressions against one another.  If all decompressions are identical and the times to decompress them are not and all instruction sets are run; the test passes.");

    }

    if (selector == CHGWRNUM)
    {
        printf("\nUse:\n\n"
               "%2i ChangeCPUWorks \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Version>\n"
               "     <Optional Settings File>\n"
               , CHGWRNUM);

        vpxt_formated_print(HLPPRT, "The test creates compressions of the user input version (0-3) for ON2_SIMD_CAPS values ranging from 0 to 63 in ((x+1)*2)-1 increments for all supported instruction sets (NONE, MMX, SSE, SSE2, SSE3, SSSE3, SSE4_1).  If all compressions are identical, compression times are not, and all instruction sets are run; the test passes.");
    }

    if (selector == DFWMWNUM)
    {
        printf("\nUse:\n\n"
               "%2i DFWMTest \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , DFWMWNUM);

        vpxt_formated_print(HLPPRT, "The test creates 6 compressions with DFWM values of 100 80 60 40 20 and 0 and records their sizes.  If each successively lower DFWM compression has a equal or larger size than the previous the test passes.");

    }

    if (selector == DTARTNUM)
    {
        printf("\nUse:\n\n"
               "%2i DataRateTest \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , DTARTNUM);

        vpxt_formated_print(HLPPRT, "The test creates a compression and records it data rate.  If the compressions data rate is within 30% of the input target bandwidth; the test passes.");

    }

    if (selector == DBMRLNUM)
    {
        printf("\nUse:\n\n"
               "%2i DebugMatchesRelease \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Debug Executable - Must take <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>\n"
               "    <Release Executable-Must take <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>\n"
               "     <Optional Settings File>\n"
               , DBMRLNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions the first using an executable built using the newest release library, the second using an executable built using the newest debug library.  If the two compressions are identical; the test passes.");

    }

    if (selector == ENCBONUM)
    {
        printf("\nUse:\n\n"
               "%2i EncoderBreakOut \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , ENCBONUM);

        vpxt_formated_print(HLPPRT, "The test creates four compressions. The first with an EncodeBreakout of 0, the second with an EncodeBreakout of 100, the thrid with an EncodeBreakout of 500 and the fourth with an EncodeBreakout of 1000. Decompressions of the encoded files are then carried out and PSNR values are calculated.  If the decompressions run successfully and the PSNR values of each successive EncodeBreakout trial are with in 2 dB the test passes. If the PSNRs are greater than 2 dB but less than 5 dB the test is inconclusive and if the PSNRs have greater than a 5 dB difference the test fails.");
    }

    if (selector == ERRMWNUM)
    {
        printf("\nUse:\n\n"
               "%2i ErrorRes \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               , ERRMWNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions the first with Error Resilient Mode off the second on. The test then records their PSNR values.  If their PSNRs are with in 10% of one another the test passes.");

    }

    if (selector == EXTFINUM)
    {
        printf("\nUse:\n\n"
               "%2i ExtraFileCheck \n\n"
               "    <Input File>\n"
               "     <Optional Settings File>\n"
               , EXTFINUM);

        vpxt_formated_print(HLPPRT, "The test creates a two pass compression and checks the current directory, the directory the executable is located in and the directory the output file is written to for extra files.  If no extra files are found the test passes.");
    }

    if (selector == FIXDQNUM)
    {
        printf("\nUse:\n\n"
               "%2i FixedQ \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "    <FixedQ 1>\n "
               "    <FixedQ 2>\n "
               "     <Optional Settings File>\n"
               , FIXDQNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions each with user input Fixed Quantizers and records the quantizers used to encode each frame. The test then records the compressions sizes.  If all quantizers for each compression match the input Fixed Quantizer and the smaller quantizers compression has a larger file size; the test passes.");
    }

    if (selector == FKEFRNUM)
    {
        printf("\nUse:\n\n"
               "%2i ForceKeyFrameWorks \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <ForceKeyFrame>\n"
               "     <Optional Settings File>\n"
               , FKEFRNUM);

        vpxt_formated_print(HLPPRT, "The test creates a compression using a user input value for a Force Key Frame Interval. The compressor forces a key frame for every Force Key Frame Invervalith frame. The Test then records the placement of all key frames in the compression.  If key frames occur only when Force Key Frame dictates; the test passes.");

    }

    if (selector == FRSZTNUM)
    {
        printf("\nUse:\n\n"
               "  FrameSizeTest \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Starting Width-must be a mult of 16>\n"
               "    <Starting Height-must be a mult of 16>\n"
               "     <Optional Settings File>\n"
              );
        vpxt_formated_print(HLPPRT, "The test creates fourty-six cropped raw ivf files from a user input file, starting height and starting width (where starting height and starting width must be multiples of 16).  The test then creates compressions for each raw file and computes its psnr.  If all compressions psnrs within five percent of the starting width height files psnr and all psnrs are greater than 25 the test passes.");
    }

    if (selector == GQVBQNUM)
    {
        printf("\nUse:\n\n"
               "%2i GoodQvBestQ \n\n"
               "    <Input File>\n"
               "    <Target Bit Rate>\n"
               "     <Optional Settings File>\n"
               , GQVBQNUM);

        vpxt_formated_print(HLPPRT, "The test creates six compressions. The first and fourth compressions for 30% less than the input target bandwidth at good quality and best quality, the second and fifth compressions for the input target bandwidth at good quality and best quality, and the thrid and sixth at 30% more than the input target bandwidth at good quality and best quality. The test then records each files data rate and PSNR and computes the area under the curve for the common interval between the good quality curve and best quality curve.  If the area under the best quality curve is greater than the area under the good quality curve; the test passes.");
    }

    if (selector == LGIFRNUM)
    {
        printf("\nUse:\n\n"
               "%2i LagInFrames \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Lag in Frames 1>\n"
               "    <Lag in Frames 2>\n"
               "     <Optional Settings File>\n"
               , LGIFRNUM);

        vpxt_formated_print(HLPPRT, "The test creates three compressions one with Allow Lag set to 0 the second and third with Allow Lag set to 1. The second compression uses the first user input Lag in Frames value for its Lag in frames and the third uses the second user input value for its Lag in Frames. The test outputs each files quantizer values for each encoded frame.  If none of the files are identical, the PSNRs of each successive file are within 10% of the last and the quantizer output shows that the proper number of frames were lagged; the test passes.  (Test Pass/Fail only valid for modes 4 and 5.)");
    }

    if (selector == MAXQUNUM)
    {
        printf("\nUse:\n\n"
               "%2i MaxQTest \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , MAXQUNUM);

        vpxt_formated_print(HLPPRT, "The test creates nine files the first with a WorstAllowedQ equal to 3 and each subsequent file with a WorstAllowedQ eight greater than the last until 63. The test records the individual quantizer values for each encoded frame.  If the PSNRs of each WorstAllowedQ compression from 3 to 63 increase as Worst AllowedQ decreases and the recorded quantizers for each file do not exceed their corresponding WorstAllowedQ for all compressions; the test passes.");

    }

    if (selector == MEML1NUM)
    {
        printf("\nUse:\n\n"
               "%2i MemLeakCheck \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Newest Debug executable>\n"
               "     <Optional Settings File>\n"
               , MEML1NUM);

        vpxt_formated_print(HLPPRT, "The Test creates a compression using the debug executable to check memory usage and records the results to an output file.  If no memory leaks are found the test passes.");
    }

    if (selector == MEML2NUM)
    {
        printf("\nUse:\n\n"
               "%2i MemLeakCheck2 \n\n"
               "    <Mem Leak Check Exe>\n"
               "\n"
               , MEML2NUM);

        vpxt_formated_print(HLPPRT, "The test uses the debug executable to open and close 10,000 instances of the encoder and open and close 10,000 instance the decoder and then checks to make sure there are no memory leaks.  If there are no leaks the test passes.");
    }

    if (selector == MINQUNUM)
    {
        printf("\nUse:\n\n"
               "%2i MinQTest \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , MINQUNUM);

        vpxt_formated_print(HLPPRT, "The test creates two files the first with a MinQ equal to 10 the second with a MinQ equal to 60 and records the quantizer used for each compressions frames.  If the first file has a higher PSNR than the second file and every quantizer for both files is above the corresponding MinQ; the test passes.");
    }

    if (selector == MULTTNUM)
    {
        printf("\nUse:\n\n"
               "%2i MultiThreadedTest \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "    <Target Bit Rate>\n "
               "    <Number of Cores to Use>\n"
               "     <Optional Settings File>\n"
               , MULTTNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions the first using a MultiThreaded equal to 2 the second using a MultiThreaded equal to 0. The test then compares the times to compress each.  If MultiThreaded 2 is faster than 0; the test passes.");
    }

    if (selector == NVOPSNUM)
    {
        printf("\nUse:\n\n"
               "%2i NewVsOldPSRN \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Exe File To Compare>\n"
               "     <Optional Settings File>\n"
               "\n"
               , NVOPSNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions the first using the newest version of VP8 and the second using a separate executable built using an older version. It then computes and records PSNR values for each.  If new PSNR is greater than olds PSNR or is at least within 1% of the old; the test passes.");
    }

    if (selector == NVORTNUM)
    {
        printf("\nUse:\n\n"
               "%2i NewVsOldRealTimeSpeed \n\n"
               "    <Input File>\n"
               "    <Target Bit Rate>\n"
               "    <Exe File To Compare>\n"
               "     <Optional Settings File>\n"
               , NVORTNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions the first using the newest version of VP8 and the second using a separate executable built using an older version. The test records the time that each compression took.  If the new compressions time is at least 10% faster than the old compressions time; the test passes.");
    }

    if (selector == NOISENUM)
    {
        printf("\nUse:\n\n"
               "%2i NoiseSensitivityWorks \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , NOISENUM);

        vpxt_formated_print(HLPPRT, "The test compresses seven files with Noise Sensitivity values from 0 to 6 and computes PSNR values for each.  If all compressions have differing PSNR values and Noise Sensitivity 0 has a higher PSNR than Noise Sensitivity 6; the test passes.");
    }

    if (selector == OV2PSNUM)
    {
        printf("\nUse:\n\n"
               "%2i OnePassVsTwoPass \n\n"
               "    <Input File>\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , OV2PSNUM);

        vpxt_formated_print(HLPPRT, "The test creates six compressions. The first and fourth compressions for 30% less than the input target bandwidth at one pass good quality and two pass good quality, the second and fifth compressions for the input target bandwidth at one pass good quality and two pass good quality, and the thrid and sixth at 30% more than the input target bandwidth at one pass good quality and two pass good quality. The test then records each files data rate and PSNR and computes the area under the curve for the common interval between the one pass good quality curve and the two pass good quality curve.  If the area under the two pass good quality curve is greater than the area under the one pass good quality curve; the test passes.");
    }

    if (selector == PLYALNUM)
    {
        printf("\nUse:\n\n"
               "%2i PlayAlternate \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "     <Optional Settings File>\n"
               , PLYALNUM);

        vpxt_formated_print(HLPPRT, "The test creates two compressions the first with Play Alternate equal to 0 the second with Play Alternate equal to 1. The test then records the placement of alternate reference frames and visible frames for both compressions.  If alternate reference frames exist for Play Alternate = 1 and not for Play Alternate = 0, visible frames for Play Alternate 1 and Play Alternate 2 are equal, and the files are not identical; the test passes.  (Test Pass/Fail only valid for modes 4 and 5.)");
    }

    if (selector == POSTPNUM)
    {
        printf("\nUse:\n\n"
               "%2i PostProcessorWorks \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , POSTPNUM);

        vpxt_formated_print(HLPPRT, "The test creates a compression then creates a No Filtering decompression, decompressions for Deblock and Noise levels ranging from 0 to 15.  If all Deblock and Noise decompressions return a different PSNR than the No Filtering Decompression but are within 10%; the test passes.");
    }

    if (selector == RECBFNUM)
    {
        printf(
            "  RECBFNUMfer \n\n"
            "    <Input File>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "     <Optional Settings File>\n"
            , RECBFNUM);


        vpxt_formated_print(HLPPRT, "The test creates a compression and internally compares the compressor's preview frames to the decoded output produced by decompressing the compressor�s en-coded frame. The state of each frame is recorded to a text file.  If the contents of all preview frames are identical to the content of all decoded frames; the test passes.");
    }

    if (selector == RSDWMNUM)
    {
        printf("\nUse:\n\n"
               "%2i ResampleDownWaterMark \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , RSDWMNUM);

        vpxt_formated_print(HLPPRT, "The test creates two files the first with resample-down-watermark set to 90 the second with resample-down-watermark set to 10. The test then records the frames at which the file buffer reaches the designated thresholds, the location of key frames and location of resized frames for both files.  If the first resized frame occurs on the first instance where the frame prior to a key frame reaches the correct buffer saturation for both compressions; the test passes.");
    }

    if (selector == SPEEDNUM)
    {
        printf("\nUse:\n\n"
               "%2i SpeedTest \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , SPEEDNUM);

        vpxt_formated_print(HLPPRT, "The test works for RealTime Mode and Good Quality Mode. For Real Time Mode the test creates compressions for CpuUsed Values from -1 to -16 and 0 to 16. For Good Quality Mode the test creates compressions for CpuUsed Values from 0 to 5.  If compression speed increases as CpuUsed increases and all PSNRs are within 10% of the previous; the test passes.");
    }

    if (selector == TV2BTNUM)
    {
        printf("\nUse:\n\n"
               "%2i TestVectorCheck \n\n"
               "    <Input Directory>\n"
               "\n"
               , TV2BTNUM);

        vpxt_formated_print(HLPPRT, "This test decodes each VP8 Test Vector and Checks its MD5 checksum against the expected value.  If all Test Vectors decode properly and all MD5 checksums match their expected values; the test passes.");

    }

    if (selector == TV2BTNUM)
    {
        printf("\nUse:\n\n"
               "%2i TwoPassVsTwoPassBest \n\n"
               "    <Input File>\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , TV2BTNUM);

        vpxt_formated_print(HLPPRT, "The test creates six compressions. The first and fourth compressions for 30% less than the input target bandwidth at two pass good quality and two pass best quality, the second and fifth compressions for the input target bandwidth at two pass good quality and two pass best quality, and the third and sixth at 30% more than the input target bandwidth at two pass good quality and two pass best quality. The test then records each files data rate and PSNR and computes the area under the curve for the common interval between the two pass good quality curve and the two pass best quality curve.  If the area under the two pass best quality curve is greater than the area under the two pass good quality curve; the test passes.");


    }

    if (selector == UNDSHNUM)
    {
        printf("\nUse:\n\n"
               "%2i UnderShoot \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , UNDSHNUM);

        vpxt_formated_print(HLPPRT, "The test creates two files the first with an undershoot equal to 10 the second with an undershoot equal to 100.  If the Undershoot 100 compressions file size is greater than the Undershoot 10 compressions file size; the test passes.");
    }

    if (selector == VERSINUM)
    {
        printf("\nUse:\n\n"
               "%2i Version \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "     <Optional Settings File>\n"
               , VERSINUM);

        vpxt_formated_print(HLPPRT, "The test creates four compressions the first with Version equal to 0 the second with version equal to 1 the third with version equal to 2 the fourth with version equal to 3. The test then decodes each and records the time it took to do so.  If each successive version takes less time than the prior to decode and has a lower PSNR; the test passes.");
    }

    if (selector == WMLMMNUM)
    {
        printf("\nUse:\n\n"
               "%2i WinLinMacMatch \n\n"
               "    <Input File>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               "    <Test Mode>\n"
               "          (0)Create Test Files\n"
               "          (1)Preform Test\n"
               "    <Input Folder - enter N/A if none>\n "
               "    <Optional Settings File>\n"
               , WMLMMNUM);

        vpxt_formated_print(HLPPRT, "The test can be run in two test modes. The first Mode, 0 creates platform specific compressions and decompressions to be tested on another platform. The second Mode creates platform specific compressions and decompressions and then compares them to previously encoded and decoded files created by test mode 0.  If the files are identical the test passes.");
    }

    return;
}
int  vpxt_tool_help(string InputString)//return 1 if string found return 0 if string not found if string not found TestHelp will be run through.
{
    if (InputString.compare("IVF2IVFCompr") == 0)
    {
        printf(
            "\n  Compress IVF to IVF \n\n"
            "    <(1)Normal Compress |(2)TimeCompress>\n"
            "    <Input File>\n"
            "    <outputfile>\n"
            "    <Bit Rate>\n"
            "    <Mode>\n"
            "    <Optional - Parameter File>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will take in a raw ivf file and produce an encoded ivf file using the given mode and bitrate.  Default encode settings can be overridden by specifying a parameter file.");
        return 1;
    }

    if (InputString.compare("IVF2IVFDec") == 0)
    {
        printf(
            "\n  Decompress IVF to IVF \n\n"
            "    <Input File>\n"
            "    <outputfile>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will take in an encoded ivf file and output a decoded ivf file.");
        return 1;
    }

    if (InputString.compare("IVF2RawDec") == 0)
    {
        printf(
            "\n  Decompress IVF to Raw \n\n"
            "    <Input File>\n"
            "    <outputfile>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will take in an encoded ivf file and output a decoded raw file.");
        return 1;
    }

    if (InputString.compare("IVFDataRate") == 0)
    {
        printf(
            "\n  IVF DataRate \n\n"
            "    <Input File>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will take in an ivf file and compute its average, min, max, and file data rates. ");
        return 1;
    }

    if (InputString.compare("IVFPSNR") == 0)
    {
        printf(
            "\n  IVFPSNR\n\n"
            "     <Raw IVF File>\n"
            "     <Comp IVF File>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will compute an encoded files psnr using the encoded file's ivf source file.");
        return 1;
    }

    if (InputString.compare("IVFCheckPBM") == 0)
    {
        printf(
            "\n  IVFCheckPBM\n\n"
            "     <input>\n"
            "     <bitrate>\n"
            "     <bufferSize>\n"
            "     <prebuffer>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will run CheckPBM to make sure a buffer under run wont occur.");
        return 1;
    }

    if (InputString.compare("Raw2IVF") == 0)
    {
        printf(
            "\n  Raw2IVF\n\n"
            "    <Input File>\n"
            "    <OutPutDir>\n"
            "    <Width>\n"
            "    <Height>\n"
            "    <FrameRate>\n"
            "    <FourCC>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will take in a raw file and produce a raw ivf file.");
        return 1;
    }

    if (InputString.compare("IVF2Raw") == 0)
    {
        printf("\n"
               "  IVF2Raw \n\n"
               "    <Input File>\n"
               "    <Output File>\n"
              );

        vpxt_formated_print(TOLPRT, "This utility will take in an ivf file and produce a raw file.");
        return 1;
    }

    if (InputString.compare("IVF2RawFrames") == 0)
    {
        printf(
            "\n  IVF2RawFrames\n\n"
            "    <Input File>\n"
            "    <OutPutDir>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will take in an ivf file and produce individual raw frames for each frame that exists in the user specified directory.");
        return 1;
    }

    if (InputString.compare("CombineIndvFrames") == 0)
    {
        printf("\n  CombineIndvFrames");
        vpxt_formated_print(TOLPRT, "This utility will combine all individual decoded frames in a folder into a single raw file in numerical order.");
        printf(
            "\n"
            "   Usage:\n"
            "\n"
            "    <Input Directory>\n"
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
            "       Input Directory = Directory files are located in\n"
            "       Name Base = img-720x480-\n"
            "       File Extension = .i420\n"
            "       First Frame = 0003\n"
            "       Last Frame = 0223\n"
            "       Output File = Path to desired output file"
            "\n"
            "\n"
        );

        return 1;
    }

    if (InputString.compare("CompareIVF") == 0)
    {
        printf(
            "\n  CompareIVF\n\n"
            "     <inputfile1>\n"
            "     <inputfile2>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will compare the video content of two ivf files and will display if they are identical, or if they differ the first frame they differ at.");
        return 1;
    }

    if (InputString.compare("CompIVFHeader") == 0)
    {
        printf("\n"
               "  CompareIVFHeaderInfo \n\n"
               "    <inputfile1>\n"
               "    <inputfile2>\n"
               "    <Full File Info 1/yes-0/no>\n"
               "    <OutputFile>\n"
              );

        vpxt_formated_print(TOLPRT, "This utility will compare the file and frame headers of two ivf files.");
        return 1;
    }

    if (InputString.compare("DispIVFHeader") == 0)
    {
        printf("\n"
               "  DisplayIVFHeaderInfo \n\n"
               "    <Input File>\n"
               "    <Full File Info 1/yes-0/no>\n"
               "    <Optional Outputfile>\n"
              );

        vpxt_formated_print(TOLPRT, "This utility will display the file and frame headers of an ivf file.");
        return 1;
    }

    if (InputString.compare("DispKeyFrames") == 0)
    {
        printf("\n"
               "  DispKeyFrames \n\n"
               "    <Input IVF File>\n"
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        vpxt_formated_print(TOLPRT, "This utility will display the location of key frames within an ivf file to the screen or an output file.");
        return 1;
    }

    if (InputString.compare("DispResizedFrames") == 0)
    {
        printf("\n"
               "  DisplayResizedFrames \n\n"
               "    <Input IVF File>\n"
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        vpxt_formated_print(TOLPRT, "This utility will display the location of resized frames within an ivf file to the screen or an output file.");
        return 1;
    }

    if (InputString.compare("DispVisibleFrames") == 0)
    {
        printf("\n"
               "  DisplayVisibleFrames \n\n"
               "    <Input IVF File>\n"
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        vpxt_formated_print(TOLPRT, "This utility will display the location of visible frames within an ivf file to the screen or an output file.");
        return 1;
    }

    if (InputString.compare("DispAltRefFrames") == 0)
    {
        printf("\n"
               "  DisplayAltRefFrames \n\n"
               "    <Input IVF File>\n"
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        vpxt_formated_print(TOLPRT, "This utility will display the location of alternate reference frames within an ivf file to the screen or an output file.");
        return 1;
    }


    if (InputString.compare("CutIVF") == 0)
    {
        printf(
            "\n  CutIVF\n\n"
            "    <Input File>\n"
            "    <outputfile>\n"
            "    <Starting Frame>\n"
            "    <Ending Frame>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will cut a portion of an ivf file starting at Starting Frame and ending at Ending Frame to a new output file");
        return 1;
    }

    if (InputString.compare("PasteIVF") == 0)
    {
        printf(
            "\n  PasteIVF\n\n"
            "    <Inputfile1>\n"
            "    <Inputfile2>\n"
            "    <Outputfile>\n"
            "    <First Paste Frame>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will paste the contents of Inputfile2 into Inputfile1 starting at Inputfile1's First Paste Frame to a new Outputfile.");
        return 1;
    }

    if (InputString.compare("PlayDecIVF") == 0)
    {
#if defined(_WIN32)
        printf(
            "\n  PlayDecIVF\n\n"
            "    <Input File>\n"
        );
        vpxt_formated_print(TOLPRT, "This Tool will convert an uncompressed ivf file to a raw yuv file and play it using tmnplay.  Please be sure to put a copy of tmnplay in your C:\\bin directory.  Tmnplay can be found in the TestClips directory.");
#else
        printf("\n  PlayDecIVF");
        vpxt_formated_print(TOLPRT, "This Tool will convert an uncompressed ivf file to a raw yuv file and play it using mplayer.  To install mplayer open a command console and type:");
        printf("    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
               "    cd mplayer\n"
               "    ./configure\n"
               "    make\n"
               "    make install\n"
               "\n"
              );
#endif
        return 1;
    }

    if (InputString.compare("PlayCompIVF") == 0)
    {
#if defined(_WIN32)
        printf(
            "\n  PlayCompIVF\n\n"
            "    <Input File>\n"
        );
        vpxt_formated_print(TOLPRT, "This Tool will convert a compressed ivf file to a raw yuv file and play it using tmnplay.  Please be sure to put a copy of tmnplay in your C:\\bin directory.  Tmnplay can be found in the TestClips directory.");
#else
        printf("\n  PlayCompIVF");
        vpxt_formated_print(TOLPRT, "This Tool will convert a compressed ivf file to a raw yuv file and play it using mplayer.  To install mplayer open a command console and type:");
        printf("    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
               "    cd mplayer\n"
               "    ./configure\n"
               "    make\n"
               "    make install\n"
               "\n"
              );
#endif
        return 1;
    }

    if (InputString.compare("CreateSampleTextFiles") == 0)
    {
        vpxt_formated_print(TOLPRT, "This utility will create sample text files.");
        return 1;
    }

    if (InputString.compare("PrintVersion") == 0)
    {
        vpxt_formated_print(TOLPRT, "This utility will print the version of vp8 being used by the tester.");
        return 1;
    }

    if (InputString.compare("RandParFile") == 0)
    {
        printf(
            "\n  RandomParFile\n\n"
            "    <Output Par File>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will create a valid vp8 random parameter file.");
        return 1;
    }

    if (InputString.compare("RandIVFComp") == 0)
    {
        printf("\n"
               "  RandComp\n\n"
               "    <Input File>\n"
              );

        vpxt_formated_print(TOLPRT, "This utility will create a compression using random parameters for an input ivf file.");
        return 1;
    }

    if (InputString.compare("GraphPSNR") == 0)
    {
        printf("\n"
               "  GraphPSNR\n\n"
               "    <Inputfile 1>\n"
               "    <Starting Bit Rate>\n"
               "    <Ending Bit Rate>\n"
               "    <Bit Rate Step>\n"
               "    <Optional - Par Input>\n"
              );

        vpxt_formated_print(TOLPRT, "The utility creates compressions from user input starting to user input ending bit rates at user input steps.  The utility then computes and outputs the data rates and PSNRs of the resultant files.");
    }

    if (InputString.compare("TestVectorIndex") == 0)
    {
        printf("\n  TestVectorIndex\n");
        vpxt_formated_print(TOLPRT, "This Utility makes use of the TestVectorsIndividualRunsResults Directory to locate the first report of a line being covered by a test vector and reports relevant information about it.");
        /*printf("\n  Valid Files to check include:\n"
                "     alloccommon\n"
                "     blockd\n"
                "     dboolhuff\n"
                "     decodemv\n"
                "     decodframe\n"
                "     demode\n"
                "     dequantize\n"
                "     detokenize\n"
                "     dsystemdependent\n"
                "     entropy\n"
                "     entropymode\n"
                "     extend\n"
                "     filter_c\n"
                "     findnearmv\n"
                "     getproc\n"
                "     idctllm\n"
                "     loopfilter\n"
                "     mbpitch\n"
                "     onyxd_if\n"
                "     postproc\n"
                "     quant_common\n"
                "     recon\n"
                "     reconinter\n"
                "     reconintra\n"
                "     reconintra4x4\n"
                "     Segmentation_common\n"
                "     swapyv12buffer\n"
                "     systemdependent\n"
                "     textblit\n"
                "     threading\n"
                "     treecoder\n"
                "     treereader\n"
                "     yv12config\n"
                "     yv12extend\n\n");*/
        return 1;
    }

    if (InputString.compare("IVFEnc") == 0)
    {
        printf(
            "\n  IVFEnc\n"
            "\n"
        );

        vpxt_formated_print(TOLPRT, "This utility is a clone of the standard VP8 ivfenc utility.");
        return 1;
    }

    if (InputString.compare("IVFDec") == 0)
    {
        printf(
            "\n  IVFDec\n"
            "\n"
        );

        vpxt_formated_print(TOLPRT, "This utility is a clone of the standard VP8 ivfdec utility.");
        return 1;
    }

    if (InputString.compare("CropRawIVF") == 0)
    {
        printf(
            "\n  CropRawIVF\n\n"
            "    <Input File>\n"
            "    <outputfile>\n"
            "    <xoffset>\n"
            "    <yoffset>\n"
            "    <New Frame Width>\n"
            "    <New Frame Height>\n"
            "    <Raw/IVF 0-Raw 1-IVF>\n"
        );

        vpxt_formated_print(TOLPRT, "This utility will take in a raw ivf file and produce a croped raw ivf file using the input size parameters.");
        return 1;
    }

    return 0;
}
void format_summary_by_test(char *InputFileNameCharAr, int DeleteOldFile)
{
    printf("\n Formating Summary file.\n");
    string InputFileName = InputFileNameCharAr;
    string SummaryByTestOutput = InputFileName;
    SummaryByTestOutput.erase(SummaryByTestOutput.end() - 4, SummaryByTestOutput.end());

    SummaryByTestOutput.append("_SortBy_Test.txt");

    //Deletes old file if user chooses to.
    if (DeleteOldFile == 1)
    {
        fstream FormatSummaryByTestFileOutDelete;
        FormatSummaryByTestFileOutDelete.open(SummaryByTestOutput.c_str(), fstream::in | fstream::out | fstream::app);

        if (FormatSummaryByTestFileOutDelete.good())
        {
            FormatSummaryByTestFileOutDelete.close();
        }
        else
        {
            FormatSummaryByTestFileOutDelete.close();
        }
    }

    char buffer[1024];
    char buffer2[36];

    string TestTrackerName;
    int TestTracker = 1;

    while (TestTracker <= MAXTENUM)
    {
        if (TestTracker == AlWDFNUM)
            TestTrackerName = "Test_Allow_Drop_Frames";

        if (TestTracker == ALWLGNUM)
            TestTrackerName = "Test_Allow_Lag";

        if (TestTracker == ALWSRNUM)
            TestTrackerName = "Test_Allow_Spatial_Resampling";

        if (TestTracker == AUTKFNUM)
            TestTrackerName = "Test_Auto_Key_Frame";

        if (TestTracker == BUFLVNUM)
            TestTrackerName = "Test_Buffer_Level";

        if (TestTracker == CPUDENUM)
            TestTrackerName = "Test_Change_Cpu_Dec";

        if (TestTracker == CHGWRNUM)
            TestTrackerName = "Test_Change_Cpu_Enc";

        if (TestTracker == DTARTNUM)
            TestTrackerName = "Test_Data_Rate";

        if (TestTracker == DBMRLNUM)
            TestTrackerName = "Test_Debug_Matches_Release";

        if (TestTracker == DFWMWNUM)
            TestTrackerName = "Test_Drop_Frame_Watermark";

        if (TestTracker == ENCBONUM)
            TestTrackerName = "Test_Encoder_Break_Out";

        if (TestTracker == ERRMWNUM)
            TestTrackerName = "Test_Error_Resolution";

        if (TestTracker == EXTFINUM)
            TestTrackerName = "Test_Extra_File";

        if (TestTracker == FIXDQNUM)
            TestTrackerName = "Test_Fixed_Quantizer";

        if (TestTracker == FKEFRNUM)
            TestTrackerName = "Test_Force_Key_frame";

        if (TestTracker == FRSZTNUM)
            TestTrackerName = "Test_Frame_Size";

        if (TestTracker == GQVBQNUM)
            TestTrackerName = "Test_Good_Vs_Best";

        if (TestTracker == LGIFRNUM)
            TestTrackerName = "Test_Lag_In_Frames";

        if (TestTracker == MAXQUNUM)
            TestTrackerName = "Test_Max_Quantizer";

        if (TestTracker == MEML1NUM)
            TestTrackerName = "Test_Mem_Leak";

        if (TestTracker == MEML2NUM)
            TestTrackerName = "Test_Mem_Leak2";

        if (TestTracker == MINQUNUM)
            TestTrackerName = "Test_Min_Quantizer";

        if (TestTracker == MULTTNUM)
            TestTrackerName = "Test_Multithreaded";

        if (TestTracker == NVOPSNUM)
            TestTrackerName = "Test_New_Vs_Old_Psnr";

        if (TestTracker == NVORTNUM)
            TestTrackerName = "Test_New_Vs_Old_Real_Time_Speed";

        if (TestTracker == NOISENUM)
            TestTrackerName = "Test_Noise_Sensitivity";

        if (TestTracker == OV2PSNUM)
            TestTrackerName = "Test_One_Pass_Vs_Two_Pass";

        if (TestTracker == PLYALNUM)
            TestTrackerName = "Test_Play_Alternate";

        if (TestTracker == POSTPNUM)
            TestTrackerName = "Test_Post_Processor";

        if (TestTracker == RECBFNUM)
            TestTrackerName = "Test_Reconstruct_Buffer";

        if (TestTracker == RSDWMNUM)
            TestTrackerName = "Test_Resample_Down_Watermark";

        if (TestTracker == SPEEDNUM)
            TestTrackerName = "Sest_Speed";

        if (TestTracker == TVECTNUM)
            TestTrackerName = "Test_Test_Vector";

        if (TestTracker == TV2BTNUM)
            TestTrackerName = "Test_Two_Pass_Vs_Two_Pass_Best";

        if (TestTracker == UNDSHNUM)
            TestTrackerName = "Test_Undershoot";

        if (TestTracker == VERSINUM)
            TestTrackerName = "Test_Version";

        if (TestTracker == WMLMMNUM)
            TestTrackerName = "Test_Win_Lin_Mac_Match";

        fstream FormatSummaryByTestFile;
        FormatSummaryByTestFile.open(InputFileName.c_str());

        if (!FormatSummaryByTestFile.good())
        {
            cout << "Cannot open input file.";
            return;
        }

        int p = 0;
        int DoOnce = 1;

        //read in and throw away header
        while (p < 7)
        {
            FormatSummaryByTestFile.getline(buffer, 1024);
            p++;
        }

        while (!FormatSummaryByTestFile.eof() && buffer[0] != '-')
        {
            memset(buffer, 0, sizeof(buffer));
            memset(buffer2, 0, sizeof(buffer2));
            FormatSummaryByTestFile.getline(buffer, 1024);

            int n = 5;
            int i = 0;

            while (n < 39 && buffer[n] != ' ')
            {
                buffer2[i] = buffer[n];
                n++;
                i++;
            }

            string TestReadString = buffer2;

            if (TestReadString.compare(TestTrackerName) == 0)
            {
                fstream FormatSummaryByTestFileOut;
                FormatSummaryByTestFileOut.open(SummaryByTestOutput.c_str(), fstream::in | fstream::out | fstream::app);

                if (DoOnce)
                {
                    FormatSummaryByTestFileOut << "\n\n ===================================" << buffer2 << "=================================== \n\n";
                    DoOnce = 0;
                }

                FormatSummaryByTestFileOut << buffer << "\n";
                FormatSummaryByTestFileOut.close();

            }

        }

        FormatSummaryByTestFile.close();

        TestTracker++;
    }

    return;

}
void format_summary_by_test_and_result(char *InputFileNameCharAr, int DeleteOldFile)
{
    printf("\n Formating Summary file.\n");

    string InputFileName = InputFileNameCharAr;
    string SummaryByTestOutput = InputFileName;
    SummaryByTestOutput.erase(SummaryByTestOutput.end() - 4, SummaryByTestOutput.end());

    SummaryByTestOutput.append("_expanded.txt");

    char TestsRunChar[255];
    vpxt_folder_name(InputFileNameCharAr, TestsRunChar);
    string TestsRun = TestsRunChar;
    TestsRun.append("tests_run.txt");

    if (DeleteOldFile == 1)
    {
        fstream FormatSummaryByTestFileOutDelete;
        FormatSummaryByTestFileOutDelete.open(SummaryByTestOutput.c_str(), fstream::in | fstream::out | fstream::app);

        if (FormatSummaryByTestFileOutDelete.good())
        {
            FormatSummaryByTestFileOutDelete.close();
        }
        else
        {
            FormatSummaryByTestFileOutDelete.close();
        }
    }

    FILE *fp;

    if ((fp = freopen(SummaryByTestOutput.c_str(), "a", stderr)) == NULL)
    {
        printf("Cannot open summary out put file: %s\n", SummaryByTestOutput.c_str());
        return;
    }

    //Deletes old file if user chooses to.

    char buffer[1024];
    char buffer2[36];
    char buffer3[128];
    char buffer4[1024];

    string TestTrackerName;
    string TestTrackerResult;

    int TestTracker = 1;
    int ResultTracker = 0;

    //loop through all existing tests alphabeticaly
    while (TestTracker <= MAXTENUM)
    {
        ResultTracker = 0;

        int DoOnce1 = 1;

        //This iterates through all possible tests run using the test tracker names to specify which was run the number is only to drive
        //The iteration through and does not represent test number
        if (TestTracker == AlWDFNUM)
            TestTrackerName = "Test_Allow_Drop_Frames";

        if (TestTracker == ALWLGNUM)
            TestTrackerName = "Test_Allow_Lag";

        if (TestTracker == ALWSRNUM)
            TestTrackerName = "Test_Allow_Spatial_Resampling";

        if (TestTracker == AUTKFNUM)
            TestTrackerName = "Test_Auto_Key_Frame";

        if (TestTracker == BUFLVNUM)
            TestTrackerName = "Test_Buffer_Level";

        if (TestTracker == CPUDENUM)
            TestTrackerName = "Test_Change_Cpu_Dec";

        if (TestTracker == CHGWRNUM)
            TestTrackerName = "Test_Change_Cpu_Enc";

        if (TestTracker == DTARTNUM)
            TestTrackerName = "Test_Data_Rate";

        if (TestTracker == DBMRLNUM)
            TestTrackerName = "Test_Debug_Matches_Release";

        if (TestTracker == DFWMWNUM)
            TestTrackerName = "Test_Drop_Frame_Watermark";

        if (TestTracker == ENCBONUM)
            TestTrackerName = "Test_Encoder_Break_Out";

        if (TestTracker == ERRMWNUM)
            TestTrackerName = "Test_Error_Resolution";

        if (TestTracker == EXTFINUM)
            TestTrackerName = "Test_Extra_File";

        if (TestTracker == FIXDQNUM)
            TestTrackerName = "Test_Fixed_Quantizer";

        if (TestTracker == FKEFRNUM)
            TestTrackerName = "Test_Force_Key_frame";

        if (TestTracker == FRSZTNUM)
            TestTrackerName = "Test_Frame_Size";

        if (TestTracker == GQVBQNUM)
            TestTrackerName = "Test_Good_Vs_Best";

        if (TestTracker == LGIFRNUM)
            TestTrackerName = "Test_Lag_In_Frames";

        if (TestTracker == MAXQUNUM)
            TestTrackerName = "Test_Max_Quantizer";

        if (TestTracker == MEML1NUM)
            TestTrackerName = "Test_Mem_Leak";

        if (TestTracker == MEML2NUM)
            TestTrackerName = "Test_Mem_Leak2";

        if (TestTracker == MINQUNUM)
            TestTrackerName = "Test_Min_Quantizer";

        if (TestTracker == MULTTNUM)
            TestTrackerName = "Test_Multithreaded";

        if (TestTracker == NVOPSNUM)
            TestTrackerName = "Test_New_Vs_Old_Psnr";

        if (TestTracker == NVORTNUM)
            TestTrackerName = "Test_New_Vs_Old_Real_Time_Speed";

        if (TestTracker == NOISENUM)
            TestTrackerName = "Test_Noise_Sensitivity";

        if (TestTracker == OV2PSNUM)
            TestTrackerName = "Test_One_Pass_Vs_Two_Pass";

        if (TestTracker == PLYALNUM)
            TestTrackerName = "Test_Play_Alternate";

        if (TestTracker == POSTPNUM)
            TestTrackerName = "Test_Post_Processor";

        if (TestTracker == RECBFNUM)
            TestTrackerName = "Test_Reconstruct_Buffer";

        if (TestTracker == RSDWMNUM)
            TestTrackerName = "Test_Resample_Down_Watermark";

        if (TestTracker == SPEEDNUM)
            TestTrackerName = "Sest_Speed";

        if (TestTracker == TVECTNUM)
            TestTrackerName = "Test_Test_Vector";

        if (TestTracker == TV2BTNUM)
            TestTrackerName = "Test_Two_Pass_Vs_Two_Pass_Best";

        if (TestTracker == UNDSHNUM)
            TestTrackerName = "Test_Undershoot";

        if (TestTracker == VERSINUM)
            TestTrackerName = "Test_Version";

        if (TestTracker == WMLMMNUM)
            TestTrackerName = "Test_Win_Lin_Mac_Match";

        int PassedTrack = 0;
        int FailedTrack = 0;
        int IndeterminateTrack = 0;
        int SeeComboRunLogTrack = 0;
        int SeePSNRLogTrack = 0;
        int RandomTestCompletedTrack = 0;
        int CompressionMadeTrack = 0;
        int ErrorFileMismatchTrack = 0;
        int TestNotSupportedTrack = 0;
        int MinTestPassed = 0;

        //loop though all possible results
        while (ResultTracker < 13)
        {
            if (ResultTracker == 1)
                TestTrackerResult = "Passed";

            if (ResultTracker == 0)
                TestTrackerResult = "Failed";

            if (ResultTracker == 2)
                TestTrackerResult = "Indeterminate";

            if (ResultTracker == 3)
                TestTrackerResult = "SeeComboRunLog";

            if (ResultTracker == 4)
                TestTrackerResult = "SeePSNRLog";

            if (ResultTracker == 5)
                TestTrackerResult = "RandomTestCompleted";

            if (ResultTracker == 6)
                TestTrackerResult = "";

            if (ResultTracker == 7)
                TestTrackerResult = "";

            if (ResultTracker == 8)
                TestTrackerResult = "MinTestPassed";

            if (ResultTracker == 9)
                TestTrackerResult = "";

            if (ResultTracker == 10)
                TestTrackerResult = "CompressionMade";

            if (ResultTracker == 11)
                TestTrackerResult = "ErrorFileMismatch";

            if (ResultTracker == 12)
                TestTrackerResult = "TestNotSupported";

            fstream FormatSummaryByTestFile;
            FormatSummaryByTestFile.open(InputFileName.c_str());

            if (!FormatSummaryByTestFile.good())
            {
                cout << "Cannot open input file.";
                return;
            }

            int p = 0;
            int DoOnce2 = 1;

            //read in and throw away header
            while (p < 7)
            {
                FormatSummaryByTestFile.getline(buffer, 1024);
                p++;
            }

            while (!FormatSummaryByTestFile.eof() && buffer[0] != '-')
            {
                memset(buffer, 0, sizeof(buffer));
                memset(buffer2, 0, sizeof(buffer2));
                memset(buffer3, 0, sizeof(buffer2));
                memset(buffer4, 0, sizeof(buffer2));

                FormatSummaryByTestFile.getline(buffer, 1024);

                int n = 5;
                int i = 0;

                while (n < 39 && buffer[n] != ' ')
                {
                    buffer2[i] = buffer[n];
                    n++;
                    i++;
                }

                n = 62;
                i = 0;

                while (n < 194 && buffer[n] != ' ')
                {
                    buffer3[i] = buffer[n];
                    n++;
                    i++;
                }

                string TestReadString = buffer2;
                string TestResultString = buffer3;

                //check to see if names and states match up
                if (TestReadString.compare(TestTrackerName) == 0 && TestResultString.compare(TestTrackerResult) == 0)
                {
                    if (ResultTracker == 1)
                        PassedTrack++;

                    if (ResultTracker == 0)
                        FailedTrack++;

                    if (ResultTracker == 2)
                        IndeterminateTrack++;

                    if (ResultTracker == 3)
                        SeeComboRunLogTrack++;

                    if (ResultTracker == 4)
                        SeePSNRLogTrack++;

                    if (ResultTracker == 8)
                        MinTestPassed++;

                    if (ResultTracker == 10)
                        CompressionMadeTrack++;

                    if (ResultTracker == 11)
                        ErrorFileMismatchTrack++;

                    if (ResultTracker == 12)
                        TestNotSupportedTrack++;

                    if (DoOnce1)
                    {
                        //print test header
                        int leftLineSize = 75 - (TestReadString.size() / 2);
                        int counter = 0;

                        fprintf(stderr, "\n\n");

                        while (counter < leftLineSize)
                        {
                            fprintf(stderr, "=");
                            counter++;
                        }

                        fprintf(stderr, "%s", TestReadString.c_str());
                        counter = counter + TestReadString.size();

                        while (counter < 150)
                        {
                            fprintf(stderr, "=");
                            counter++;
                        }

                        fprintf(stderr, "\n");
                        DoOnce1 = 0;
                    }

                    if (DoOnce2)
                    {
                        fprintf(stderr, "\n %s:\n\n", buffer3);
                        fprintf(stderr, "%4s %-32s%-25s%s  Test Settings\n\n", "#", "            Test Name" , "     Date and Time", "Status");
                        DoOnce2 = 0;
                    }

                    fprintf(stderr, "%s ", buffer);

                    ///////////////////Get the correct Test Input settings and output them to summary.//////////////
                    fstream TestsRunFile;
                    TestsRunFile.open(TestsRun.c_str(), fstream::in);

                    if (!TestsRunFile.good())
                    {
                        TestsRunFile.close();
                        FormatSummaryByTestFile.close();
                        fclose(fp);
                        tprintf("\nFile creation Canceled: Cannot find %s\n", TestsRun.c_str());
                        return;
                    }

                    char TestRunNum[4] = "";

                    int v = 0;

                    while (v < 4)
                    {
                        TestRunNum[v] = buffer[v];
                        v++;
                    }

                    string TestRunNumStr = TestRunNum;
                    int TestRunNumInt = atoi(TestRunNumStr.c_str());

                    int TestNumberCur = -1;

                    while (!TestsRunFile.eof() && TestNumberCur != TestRunNumInt)
                    {
                        TestsRunFile.getline(buffer4, 1024);

                        if (buffer4[0] == '+')
                        {
                            TestNumberCur++;
                        }

                        if (TestNumberCur == TestRunNumInt)
                        {
                            buffer4[0] = ' ';
                            int t = 0;

                            while (buffer4[t] != '\0')
                            {
                                if (buffer4[t] == '@')
                                {
                                    buffer4[t] = ' ';
                                }

                                t++;
                            }

                            printf("%s\n", buffer4);
                        }
                    }

                    TestsRunFile.close();
                    //////////////////////////////////////////////////////////////////////////////////////////////
                }
            }

            FormatSummaryByTestFile.close();
            ResultTracker++;
        }

        int PassorFailExist = 0;
        int CompressExist = 0;
        int PrintOutput = 0;

        if (PassedTrack != 0)
        {
            PassorFailExist = 1;
            PrintOutput = 1;
        }

        if (FailedTrack != 0)
        {
            PassorFailExist = 1;
            PrintOutput = 1;
        }

        if (IndeterminateTrack != 0)
        {
            PassorFailExist = 1;
            PrintOutput = 1;
        }

        if (SeeComboRunLogTrack != 0)
            PrintOutput = 1;

        if (SeePSNRLogTrack != 0)
            PrintOutput = 1;

        if (RandomTestCompletedTrack != 0)
            PrintOutput = 1;

        if (CompressionMadeTrack != 0)
        {
            CompressExist = 1;
            PrintOutput = 1;
        }

        if (ErrorFileMismatchTrack != 0)
            PrintOutput = 1;

        if (TestNotSupportedTrack != 0)
            PrintOutput = 1;

        if (MinTestPassed != 0)
            PrintOutput = 1;

        if (PrintOutput == 1)
        {
            if (!CompressExist == 1)
            {
                fprintf(stderr, "\n\n\n%s Summary:\n\n", TestTrackerName.c_str());
                fprintf(stderr, "Passed: %i\n", PassedTrack);
                fprintf(stderr, "Failed: %i\n", FailedTrack);
                fprintf(stderr, "Indeterminate: %i\n", IndeterminateTrack);

                if (SeeComboRunLogTrack != 0)
                    fprintf(stderr, "SeeComboRunLogTrack: %i\n", SeeComboRunLogTrack);

                if (SeePSNRLogTrack != 0)
                    fprintf(stderr, "SeePSNRLogTrack: %i\n", SeePSNRLogTrack);

                if (RandomTestCompletedTrack != 0)
                    fprintf(stderr, "RandomTestCompletedTrack: %i\n", RandomTestCompletedTrack);

                if (CompressionMadeTrack != 0)
                    fprintf(stderr, "CompressionMadeTrack: %i\n", CompressionMadeTrack);

                if (ErrorFileMismatchTrack != 0)
                    fprintf(stderr, "ErrorFileMismatchTrack: %i\n", ErrorFileMismatchTrack);

                if (TestNotSupportedTrack != 0)
                    fprintf(stderr, "TestNotSupportedTrack: %i\n", TestNotSupportedTrack);

                if (MinTestPassed != 0)
                    fprintf(stderr, "MinTestPassed: %i\n", MinTestPassed);
            }
            else
            {
                fprintf(stderr, "\n\n\n%s Summary:\n\n", TestTrackerName.c_str());

                if (PassedTrack != 0)
                    fprintf(stderr, "Passed: %i\n", PassedTrack);

                if (FailedTrack != 0)
                    fprintf(stderr, "Failed: %i\n", FailedTrack);

                if (IndeterminateTrack != 0)
                    fprintf(stderr, "Indeterminate: %i\n", IndeterminateTrack);

                if (SeeComboRunLogTrack != 0)
                    fprintf(stderr, "SeeComboRunLogTrack: %i\n", SeeComboRunLogTrack);

                if (SeePSNRLogTrack != 0)
                    fprintf(stderr, "SeePSNRLogTrack: %i\n", SeePSNRLogTrack);

                if (RandomTestCompletedTrack != 0)
                    fprintf(stderr, "RandomTestCompletedTrack: %i\n", RandomTestCompletedTrack);

                if (CompressionMadeTrack != 0)
                    fprintf(stderr, "CompressionMadeTrack: %i\n", CompressionMadeTrack);

                if (ErrorFileMismatchTrack != 0)
                    fprintf(stderr, "ErrorFileMismatchTrack: %i\n", ErrorFileMismatchTrack);

                if (TestNotSupportedTrack != 0)
                    fprintf(stderr, "TestNotSupportedTrack: %i\n", TestNotSupportedTrack);

                if (MinTestPassed != 0)
                    fprintf(stderr, "MinTestPassed: %i\n", MinTestPassed);
            }

            fprintf(stderr, "\n\n");
        }

        TestTracker++;
    }

    fclose(fp);
    return;

}
void show_hidden_cmds()
{
    printf("\n\n"
           "    Hidden Commands \n"
           "\n"
           "      <Compression/Decompression>"
           "\n"
           "         APICOMPRESS              -IVFEnc Clone using API 2.0\n"
           "         APIDECOMPRESS            -IVFDec Clone using API 2.0\n"
           "\n"
           "      <Test Vector Tools>"
           "\n"
           "         CodeCoverage             -Array Coverage Tests\n"
           "         CompareCodeCoverage      -Compares New to Old Code Coverage Results\n"
           "         FormatCodeCoverageFile   -Formats Code Coverage to Uniform Format\n"
           "         IVFDECTestVectorCheck    -Checks Checksums for Dir of Dec Test Vectors\n"
           "         TestVectorIndex          -Returns Test Vector that covers File & Line\n"
           "         ArrayCovFailListToFull   -Converts Fail List to Full Pass and Fail\n"
           "         ArrayCovSummaryFile      -Checks and records New Array Activations\n"
           "\n"
           "      <Verification Tools>"
           "\n"
           "         CompMatchesIVFenc        -Checks that IVFEnc matches Tester Encoder\n"
           "         CompressionEquiv         -Checks that each Tester Encoder is the same\n"
           "         PrintVersion             -Prints Internal VP8 Version Number\n"
           "         Test0InputTextCheck      -Tests a Mode 0 input to make sure its valid\n"
           "\n"
           "      <Misc>"
           "\n"
           "         ConvertParmFileToIVFenc  -Converts Tester Par File to IVFEnc Par File\n"
           "         CopyAllTxtFiles          -Copies all Txt Files in a Dir with file str\n"
           "         IVF2RawDec               -Decode an IVF file to a RAW File\n"
           "         DeleteAllIVFFiles        -Deletes all IVF Files in a dir DANGEROUS!!!\n"
           "         Raw2IVF                  -Converts a RAW File to an IVF File\n"
           "         SideBySideText           -Creates a txt file with two input coloums\n"
           "         IVF2RawFrames            -Writes out individual Frame data to a dir\n"

           "\n");
    return;

}
int  main(int argc, char *argv[])
{
    if (argc < 2)
    {
        vpxt_on_error_output();
        return 0;
    }

    string EmptyAr[1];
    char WorkingDirChar[256]  = "";
    create_working_folder(argc, argv, WorkingDirChar);
    string WorkingDir = "";
    WorkingDir.append(WorkingDirChar);
    string TestInputString(argv[1]);
    string TestInputString1(argv[1]);
    string TestInputString2;

    if (argc > 2)
    {
        TestInputString2 =  argv[2];
    }

    if (TestInputString.compare("0") == 0)
    {
        if (WorkingDir.length() > 175)
        {
            tprintf("\nError: Folder Path Length too long.\n");
            return 0;
        }
    }

    /////////////////////////////////////////////////////////////Help//////////////////////////////////////////////////////////

    if (TestInputString1.compare("Help") == 0 || TestInputString1.compare("HELP") == 0  || TestInputString1.compare("help") == 0)
    {
        vpxt_on_error_output();
        vpxt_formated_print(TOLPRT, "To run tests input the number to the left of the test name along with test specific input.  To obtain more information about a specific test or utility input the number of the test or the utility name you wish to inquire about followed by \"help\".");
        return 0;
    }

    if (TestInputString2.compare("Help") == 0 || TestInputString2.compare("HELP") == 0  || TestInputString2.compare("help") == 0)
    {
        int ToolHelpCheck = vpxt_tool_help(TestInputString1);

        if (!ToolHelpCheck)
        {
            vpxt_test_help(argc, argv, WorkingDir);
        }

        return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////Tools//////////////////////////////////////////////////////////
    ////////////////////Public Commands/////////////////////////
    if (TestInputString.compare("DirectorCheck") == 0)//delete this
    {
        vector<string> ignore;
        vector<string> list;
        string dirctory = argv[2];

        ignore.push_back(".");
        ignore.push_back("..");
        //ignore.push_back("New Text Document.txt");
        //ignore.push_back("ExtraFileCheckTest.txt");
        //ignore.push_back("Extra File Check Test_TestOnly.txt");
        //ignore.push_back("ExtraFileCheck_Paramaters.txt");
        //ignore.push_back("ExtraFileCheck_APIParamaters.txt");

        int INTEGER = vpxt_find_non_ignored_files_in_dir(ignore, list, dirctory);

        int i = 0;

        while (i < list.size())
        {
            cout << i << " " << list[i].c_str() << "\n";
            i++;
        }

        cout << "\nReturn value: " << INTEGER << "\n";

        return 0;
    }

    if (TestInputString.compare("TestPrint") == 0)                  //Compresses an IVF Raw File to an IVF Compressed file
    {
        cout << "\n";
        vpxt_formated_print(HLPPRT, "This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.");
        cout << "\n";
        vpxt_formated_print(TOLPRT, "This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.");
        cout << "\n";
        vpxt_formated_print(FUNPRT, "This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.");
        cout << "\n";
        vpxt_formated_print(OTRPRT, "This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.");
        cout << "\n\n";
        vpxt_formated_print(RESPRT, "This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.");
        cout << "\n\n";
        return 0;
    }

    if (TestInputString.compare("Quad") == 0)                   //Compresses an IVF Raw File to an IVF Compressed file
    {
        SolveQuad();
        return 0;
    }

    if (TestInputString.compare("IVF2IVFCompr") == 0)                   //Compresses an IVF Raw File to an IVF Compressed file
    {
        ComprIVF2IVF(argc, argv, WorkingDir);
        return 0;
    }

    if (TestInputString.compare("Thresh") == 0)                 //Compresses an IVF Raw File to an IVF Compressed file
    {
        if (argc < 8)
        {
            printf("\n"
                   "  DisplayResizedFrames \n\n"
                   "    <Input IVF File>\n"
                   "    <bitRate>\n"
                   "    <maxBuffer>\n"
                   "    <preBuffer>\n"
                   "    <optBuffer>\n"
                   "    <Threshold>\n"
                   "\n");
            return 0;
        }

        vpxt_ivf_check_pbm_threshold(argv[2], atof(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

        return 0;
    }

    if (TestInputString.compare("IVF2IVFDec") == 0)                     //Decompresses and IVF Compressed file to an IVF Raw File
    {
        DecIVF2IVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFDataRate") == 0)                        //Computes Data Rate for an input IVF File
    {
        IVFDataRateTool(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFPSNR") == 0)                            //Computes PSNR for two input files
    {
        IVFPSNRrun(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFCheckPBM") == 0)                        //Computes PBM For an input file
    {
        IVFCheckPBMrun(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompIVFHeader") == 0)                  //Compares the Headers of two ivf files
    {
        vpxt_compare_ivf_header_info(argc, argv);
        return 0;
    }

    if (TestInputString.compare("DispIVFHeader") == 0)                  //Displays the header contents of an ivf file
    {
        vpxt_display_ivf_header_info(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompareIVF") == 0)                     //Compares two ivf files
    {
        compareIVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CreateSampleTextFiles") == 0)          //Creates sample text files that include quick test and other templates
    {
        print_quick_test_files(WorkingDir);
        return 0;
    }

    if (TestInputString.compare("CutIVF") == 0)                         //Modifies an ivf files size
    {
        CutIVFTool(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CropRawIVF") == 0)                         //Modifies an ivf files size
    {
        CropRawIVFTool(argc, argv);
        return 0;
    }

    if (TestInputString.compare("PasteIVF") == 0)                           //Modifies an ivf files size
    {
        vpxt_paste_ivf(argc, argv);
        return 0;
    }

    if (TestInputString.compare("DispResizedFrames") == 0)                  //Compresses an IVF Raw File to an IVF Compressed file
    {
        if (argc < 4)
        {
            printf("\n"
                   "  DisplayResizedFrames \n\n"
                   "    <Input IVF File>\n"
                   "    <Write to file 1 | 0 Print to screen>\n"
                   "\n");
            return 0;
        }

        int resizedframes = vpxt_display_resized_frames(argv[2], atoi(argv[3]));
        printf("\nResized Frames Found: %i\n", resizedframes);
        return 0;
    }

    if (TestInputString.compare("DispDroppedFrames") == 0)                  //Compresses an IVF Raw File to an IVF Compressed file
    {
        if (argc < 4)
        {
            printf("\n"
                   "  DisplayDroppedFrames \n\n"
                   "    <Input IVF File>\n"
                   "    <Write to file 1 | 0 Print to screen>\n"
                   "\n");
            return 0;
        }

        int droppedframes = vpxt_display_droped_frames(argv[2], atoi(argv[3]));
        printf("\nDropped Frames Counted: %i \n\n (Num is aprox as any frame droped after last encoded frame cannot be counted)\n", droppedframes);
        return 0;
    }

    if (TestInputString.compare("DispVisibleFrames") == 0)                  //Compresses an IVF Raw File to an IVF Compressed file
    {
        if (argc < 4)
        {
            printf("\n"
                   "  DisplayVisibleFrames \n\n"
                   "    <Input IVF File>\n"
                   "    <Write to file 0 | 1 Print to screen>\n"
                   "\n");
            return 0;
        }

        int visframenum = vpxt_display_visible_frames(argv[2], atoi(argv[3]));
        printf("\nVisible Frames Found: %i\n", visframenum);

        return 0;
    }

    if (TestInputString.compare("DispAltRefFrames") == 0)                   //Compresses an IVF Raw File to an IVF Compressed file
    {
        if (argc < 4)
        {
            printf("\n"
                   "  DisplayAltRefFrames \n\n"
                   "    <Input IVF File>\n"
                   "    <Write to file 0 | 1 Print to screen>\n"
                   "\n");
            return 0;
        }

        int altrefframes = vpxt_display_alt_ref_frames(argv[2], atoi(argv[3]));
        printf("\nAlternate Reference Frames Found: %i\n", altrefframes);
        return 0;
    }

    if (TestInputString.compare("DispKeyFrames") == 0)                  //Displays which frames are key frames for an input compressed ivf file
    {
        if (argc < 3)
        {
            printf("\n"
                   "  DispKeyFrames \n\n"
                   "    <Input IVF File>\n"
                   "    <Write to file 0 | 1 Print to screen>\n"
                   "\n");
            return 0;
        }

        int keyframecount = vpxt_display_key_frames(argv[2], atoi(argv[3]));
        printf("\nKey Frames Found: %i\n", keyframecount);
        return 0;
    }

    if (TestInputString.compare("DispAltRefFrames") == 0)                   //Displays which frames are key frames for an input compressed ivf file
    {
        if (argc < 3)
        {
            printf("\n"
                   "  DispFrameData \n\n"
                   "    <Input IVF File>\n"
                   "\n");
            return 0;
        }

        vpxt_display_alt_ref_frames(argv[2], 0);
        return 0;
    }

    if (TestInputString.compare("FormatSummaryByTest") == 0)                //Formats a Test Results output file by test
    {
        if (argc < 4)
        {
            printf(
                "  FormatSummaryByTestandResult \n\n"
                "    <Text File to Format>\n"
                "    <Overwrite old Test and Result | 1 Yes 0 No  >\n"
                "\n");
            return 0;
        }

        /*argv3int = 0;
        safe_strto32(argv[3], &argv3int);
        format_summary_by_test(argv[2], argv3int);*/


        format_summary_by_test(argv[2], atoi(argv[3]));
        return 0;
    }

    if (TestInputString.compare("FormatSummaryByTestandResult") == 0)   //Formats a test results output file by test and result
    {
        if (argc < 4)
        {
            printf("\n"
                   "  FormatSummaryByTestandResult \n\n"
                   "    <Text File to Format>\n"
                   "    <Overwrite old Test and Result | 1 Yes 0 No  >\n"
                   "\n");

            return 0;
        }

        format_summary_by_test_and_result(argv[2], atoi(argv[3]));
        return 0;
    }

    if (TestInputString.compare("IVF2Raw") == 0)                            //Converts an ivf file to a raw file
    {
        if (argc < 4)
        {
            printf("\n"
                   "  IVF2Raw \n\n"
                   "    <Input File>\n"
                   "    <Output File>\n"
                   "\n");

            return 0;
        }

        IVF2Raw(argv[2], argv[3]);
        return 0;
    }

    if (TestInputString.compare("PlayCompIVF") == 0)                        //Plays a compressed ivf file (Decodes the file to an ivf then converts that ivf to a raw then used tnmplay.exe to play that file.)
    {
        Playvpxt_compare_ivf(argc, argv);
        return 0;
    }

    if (TestInputString.compare("PlayDecIVF") == 0)                     // Plays a Decoded ivf file (Converts the file to a Raw file and uses tmnplay.exe to play the file)
    {
        PlayDecIVF(argc, argv);
        return 0;
    }

    ////////////////////Hidden Commands/////////////////////////
    if (TestInputString.compare("ShowHidden") == 0)                     //Shows Hidden Commands
    {
        show_hidden_cmds();
        return 0;
    }

    if (TestInputString.compare("APICOMPRESS") == 0)                        //API Compresion Clone
    {
        APICOMPRESS(argc, argv);
        return 0;
    }

    if (TestInputString.compare("APIDECOMPRESS") == 0)                  //API Decompression Clone
    {
        APIDECOMPRESS(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CodeCoverage") == 0)                   //Preforms Code Coverage Tests for data coverage if enabled
    {
        vpxt_array_coverage(argc, argv, WorkingDir, EmptyAr);
        return 0;
    }

    if (TestInputString.compare("CompareCodeCoverage") == 0)                //Compares New Code Coverage Results to Old Code Coverage Results and merges the files to an updated version
    {
        compare_code_coverage(argc, argv);
        return 0;
    }

    if (TestInputString.compare("FormatCodeCoverageFile") == 0)         //Formats Cygwin Code Coverage files to the same format as regular linux
    {
        FormatCodeCoverageFile(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFDECTestVectorCheck") == 0)          //
    {
        IVFDECtest_vector_test(argc, argv);
        return 0;
    }

    if (TestInputString.compare("TestVectorIndex") == 0)                    //Will search a data base and return the Test Vector that covers an input File and Line Number
    {
        TestVectorIndex(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompMatchesIVFenc") == 0)              //Makes sure that the Tester compressor matches ivfenc compressor
    {
        CompMatchesIVFenc(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompressionEquiv") == 0)               //makes sure that all variations of the compressor produce the same output
    {
        CompressionEquiv(argc, argv, WorkingDir);
        return 0;
    }

    if (TestInputString.compare("PrintVersion") == 0)                   //Prints the internal version number - make sure all libs built in same multithreadedmode - properties  C++ CodeGen RuntimeLibrary
    {
        print_version();
        return 0;
    }

    if (TestInputString.compare("Test0InputTextCheck") == 0)                //Checks to make sure a Multi Run Input file has the correct format
    {
        vpxt_run_multiple_tests_input_check(argv[2], 1);
        return 0;
    }

    if (TestInputString.compare("ConvertParFileToIVFEnc") == 0)         //converts Tester Parameter file to a IVFenc ParFile - obsolete
    {
        if (argc < 4)
        {
            printf(
                "\n  Convert Parameter File\n\n"
                "     <input Par File>\n"
                "     <output Par File>\n"
                "\n");
            exit(0);
        }

        ConvertParmFileToIVFenc(argv[2], argv[3]);
        return 0;
    }

    if (TestInputString.compare("CopyAllTxtFiles") == 0)                    //Copies all text files in a directory to a new directory preserving file structure
    {
        CopyAllTxtFiles(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVF2RawDec") == 0)                     //Decodes IVF File to Raw File
    {
        DecIVF2Raw(argc, argv);
        return 0;
    }

    if (TestInputString.compare("DeleteAllIVFFiles") == 0)              //Deletes all IVF files in a directory be very care with this function!
    {
        DeleteAllIVFFiles(argc, argv);
        return 0;
    }

    if (TestInputString.compare("Raw2IVF") == 0)                            //Converts an IVF File to a Raw File
    {
        Raw2IVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("SideBySideText") == 0)                 //Prints out the input from two text files side by side as coloums to an output txt file
    {
        SideBySideText(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVF2RawFrames") == 0)                  //Converts an ivf file to a raw file
    {
        if (argc < 3)
        {
            printf(
                "\n  IVF2RawFrames\n\n"
                "    <Input File>\n"
                "    <OutPutDir>\n"
            );
            return 0;
        }

        argv[4] = "1";
        WriteIndividualFramesOut(argc, argv);
        return 0;
    }

    if (TestInputString.compare("ArrayCovFailListToFull") == 0)         //
    {
        ArrayCovFailListToFullList(argc, argv);
        return 0;
    }

    if (TestInputString.compare("ArrayCovSummaryFile") == 0)                //
    {
        ArrayCovSummaryFile(argc, argv);
        return 0;
    }

    if (TestInputString.compare("WinMemMonFormat") == 0)                //
    {
        WinMemMonFormat(argc, argv);
        return 0;
    }

    if (TestInputString.compare("WinMemMonGraph") == 0)             //
    {
        WinMemMonGraph(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CombineIndvFrames") == 0)              //
    {
        CombineIndvFrames(argc, argv);
        return 0;
    }

    if (TestInputString.compare("GraphPSNR") == 0)              //
    {
        graph_psnr(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (TestInputString.compare("RandIVFComp") == 0)                //
    {
        rand_comp(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (TestInputString.compare("RandParFile") == 0)                //
    {
        CreateRandParFile(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFDec") == 0)             //
    {
        RunIVFDec(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFEnc") == 0)             //
    {
        RunIVFEnc(argc, argv);
        return 0;
    }

    if (TestInputString.compare("PrintCPUInfo") == 0)             //
    {
        PrintCPUInfo();
        return 0;
    }


    printf("\n");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //int selector = atoi(argv[1]);
    int selector = vpxt_identify_test(argv[1]);


    if (selector > MAXTENUM || selector < 0)
    {
        vpxt_on_error_output();
        return 0;
    }

    if (selector == 0)
    {
        if (argc < 4)
        {
            printf(
                "  run_multipule_tests \n\n"
                "    <Test Type>\n"
                "      <1 - Create Files and Run Tests>\n"
                "        <Input Text File>\n"
                "      <2 - Create Files only>\n"
                "        <Input Text File>\n"
                "      <3 - Run Tests For Existing Files>\n"
                "        <Input Test Directory>\n"
                "      <4 - Resume Test>\n"
                "        <Input Test Directory>\n"
                "\n");

            return 0;
        }

        int VaildInput = 0;

        if (atoi(argv[2]) != 4 && atoi(argv[2]) != 3)
        {
            VaildInput = vpxt_run_multiple_tests_input_check(argv[3], 0);

            if (VaildInput <= 0)
            {
                printf("\nExternal Test Runner Aborted\n\n");
                return 0;
            }
        }
        else
        {
            string inputCheck = "";
            inputCheck.append(argv[3]);
            inputCheck.append(slashCharStr().c_str());
            inputCheck.append("tests_run.txt");
            VaildInput = vpxt_run_multiple_tests_input_check(inputCheck.c_str(), 0);

            if (VaildInput <= 0)
            {
                printf("\nExternal Test Runner Aborted\n\n");
                return 0;
            }
        }

        run_multiple_tests(argc, argv, WorkingDir.c_str(), VaildInput);

        string SummaryFile = WorkingDir;

        if (atoi(argv[2]) == 1)
        {
            char SummaryFileChar[255] = "";
            SummaryFile.erase(SummaryFile.end() - 1);
            SummaryFile.append(slashCharStr());
            SummaryFile.append("test_results.txt");
            snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
            format_summary_by_test_and_result(SummaryFileChar, 0);
            return 0;
        }

        if (atoi(argv[2]) == 3)
        {

            char SummaryFileChar[255] = "";
            SummaryFile.erase(SummaryFile.end() - 1);
            SummaryFile.append(slashCharStr());
            SummaryFile.append("compression_results.txt");
            snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
            format_summary_by_test_and_result(SummaryFileChar, 0);
            return 0;
        }

        if (atoi(argv[2]) == 4)
        {
            string SummCompAndTest = "";
            string SummComp = "";
            string SummTest = "";

            SummCompAndTest = argv[3];
            SummCompAndTest.append(slashCharStr());
            SummCompAndTest.append("test_results.txt");     // Mode 1
            SummComp = argv[3];
            SummComp.append(slashCharStr());
            SummComp.append("compression_results.txt");     // Mode 2
            SummTest = argv[3];
            SummTest.append(slashCharStr());
            SummTest.append("compression_test_results.txt");// Mode 3

            int TestRunning = 0;
            fstream SummCompAndTestFile;
            SummCompAndTestFile.open(SummCompAndTest.c_str());

            if (SummCompAndTestFile.good())
            {
                TestRunning = 1;
            }

            fstream SummTestFile;
            SummTestFile.open(SummTest.c_str());

            if (SummTestFile.good())
            {
                TestRunning = 3;
            }

            fstream SummCompFile;
            SummCompFile.open(SummComp.c_str());

            if (SummCompFile.good())
            {
                if (TestRunning != 3)
                {
                    TestRunning = 2;
                }

            }

            SummCompAndTestFile.close();
            SummTestFile.close();
            SummCompFile.close();

            SummaryFile = argv[3];

            if (TestRunning == 1)
            {
                char SummaryFileChar[255] = "";
                SummaryFile.append(slashCharStr());
                SummaryFile.append("test_results.txt");
                snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
                format_summary_by_test_and_result(SummaryFileChar, 0);
                return 0;
            }

            if (TestRunning == 3)
            {

                char SummaryFileChar[255] = "";
                SummaryFile.append(slashCharStr());
                SummaryFile.append("compression_test_results.txt");
                snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
                format_summary_by_test_and_result(SummaryFileChar, 0);
            }
        }

        return 0;
    }

    if (selector == AlWDFNUM)
    {
        return test_allow_drop_frames(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == ALWLGNUM)
    {
        return test_allow_lag(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == ALWSRNUM)
    {
        return test_allow_spatial_resampling(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == AUTKFNUM)
    {
        return test_auto_key_frame(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == BUFLVNUM)
    {
        return test_buffer_level(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == CPUDENUM)
    {
        return test_change_cpu_dec(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == CHGWRNUM)
    {
        return test_change_cpu_enc(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == DFWMWNUM)
    {
        return test_drop_frame_watermark(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == DTARTNUM)
    {
        return test_data_rate(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == DBMRLNUM)
    {
        return test_debug_matches_release(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == ENCBONUM)
    {
        return test_encoder_break_out(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == ERRMWNUM)
    {
        return test_error_resolution(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == EXTFINUM)
    {
        return test_extra_file(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == FIXDQNUM)
    {
        return test_fixed_quantizer(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == FKEFRNUM)
    {
        return test_force_key_frame(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == FRSZTNUM)
    {
        return test_frame_size(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == GQVBQNUM)
    {
        return test_good_vs_best(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == LGIFRNUM)
    {
        return test_lag_in_frames(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == MAXQUNUM)
    {
        return test_max_quantizer(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == MEML1NUM)
    {
        return test_mem_leak(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == MEML2NUM)
    {
        return test_mem_leak2(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == MINQUNUM)
    {
        return test_min_quantizer(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == MULTTNUM)
    {
        return test_multithreaded(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == NVOPSNUM)
    {
        test_new_vs_old_psnr(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == NVORTNUM)
    {
        return test_new_vs_old_real_time_speed(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == NOISENUM)
    {
        return test_noise_sensitivity(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == OV2PSNUM)
    {
        return test_one_pass_vs_two_pass(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == PLYALNUM)
    {
        return test_play_alternate(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == POSTPNUM)
    {
        return test_post_processor(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == RECBFNUM)
    {
        return test_reconstruct_buffer(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == RSDWMNUM)
    {
        return test_resample_down_watermark(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == SPEEDNUM)
    {
        return test_speed(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == TVECTNUM)
    {
        return test_vector_test(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == TV2BTNUM)
    {
        return test_two_pass_vs_two_pass_best(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == UNDSHNUM)
    {
        return test_undershoot(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == VERSINUM)
    {
        return test_version(argc, argv, WorkingDir, EmptyAr, 1);
    }

    if (selector == WMLMMNUM)
    {
        return test_win_lin_mac_match(argc, argv, WorkingDir, EmptyAr, 1);
    }

    vpxt_on_error_output();
    return 0;

}
