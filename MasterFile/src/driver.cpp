#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_test_definitions.h"
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
//#include <dirent.h>
#endif

//CodeCoverage
extern int tool_array_coverage(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[]);

int create_working_folder(int argc, const char *argv[], char *WorkingDirChar)
{
    ///////////////////////////////////////////Create Working Folder////////////////////////////////////

    //Get Date and time info and convert it to a string removing colons in time
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::string DateAndTime = asctime(timeinfo);

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

        w = w + 1;
    }

    DateAndTimeCharArray[w] = '\0';
    std::string DateAndTime3 = DateAndTimeCharArray;

    //Get Dir Folder
    char Folder3[255];
    char Folder2[255];

    vpxt_folder_name(argv[0], Folder3);
    snprintf(Folder2, 255, "%s", Folder3);
    std::string Folder = Folder2;

    //add Date and Time
    int number = 0;

    Folder.append(DateAndTime3);
    std::string FolderCheck = Folder;

    while (vpxt_folder_exist_check(FolderCheck)) //Make sure folder doesnt already exist
    {
        number = number + 1;
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
std::string date_string()
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *DateAndTime = asctime(timeinfo);

    //remove colons in time string
    char DateAndTimeCharArray[255];
    int w = 0;

    while (DateAndTime[w] != '\n')
    {
        if (DateAndTime[w] == ':' || DateAndTime[w] == ' ')
            DateAndTimeCharArray[w] = '_';
        else
            DateAndTimeCharArray[w] = DateAndTime[w];

        w = w + 1;
    }

    DateAndTimeCharArray[w] = '\"';
    DateAndTimeCharArray[w+1] = '\0';

    return DateAndTimeCharArray;

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
    "  (23) test_multithreaded_enc <input file> <compression mode> <target bit rate> <number of cores to use>\n"
    "  (24) test_new_vs_old_psnr <input file> <compression mode> <target bit rate> <old_release.exe> <par version>\n"
    "  (25) test_new_vs_old_enc_cpu_tick <input file> <target bit rate> <old_release.exe> <par version>\n"
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

    tprintf(PRINT_STD, "\n");
    tprintf(PRINT_STD, "Usage: Tester.exe <options> \n");
    tprintf(PRINT_STD, "\n");
    tprintf(PRINT_STD, "  <test options>                           <tool options>\n");
    tprintf(PRINT_STD, "                                            \n");
    tprintf(PRINT_STD, "   (0) run_multiple_tests                   VPXEnc\n");
    tprintf(PRINT_STD, "   (1) test_allow_drop_frames               VPXDec\n");
    tprintf(PRINT_STD, "   (2) test_allow_lag                       VPXTEnc\n");
    tprintf(PRINT_STD, "   (3) test_allow_spatial_resampling        VPXTDec\n");
    tprintf(PRINT_STD, "   (4) test_arnr                            VPXTDec2Raw\n");
    tprintf(PRINT_STD, "   (5) test_auto_key_frame                  \n");
    tprintf(PRINT_STD, "   (6) test_buffer_level                    VPXTDataRate\n");
    tprintf(PRINT_STD, "   (7) test_change_cpu_dec                  VPXTPSNR\n");
    tprintf(PRINT_STD, "   (8) test_change_cpu_enc                  VPXTCheckPBM\n");
    tprintf(PRINT_STD, "   (9) test_constrained_quality             \n");
    tprintf(PRINT_STD, "  (10) test_data_rate                       Raw2Formatted\n");
    tprintf(PRINT_STD, "  (11) test_debug_matches_release           Formatted2Raw\n");
    tprintf(PRINT_STD, "  (12) test_drop_frame_watermark            Formatted2RawFrames\n");
    tprintf(PRINT_STD, "  (13) test_encoder_break_out               CombineIndvFrames\n");
    tprintf(PRINT_STD, "  (14) test_error_resolution                \n");
    tprintf(PRINT_STD, "  (15) test_extra_file                      CompareEnc\n");
    tprintf(PRINT_STD, "  (16) test_fixed_quantizer                 CompareHeader\n");
    tprintf(PRINT_STD, "  (17) test_force_key_frame                 DisplayHeader\n");
    tprintf(PRINT_STD, "  (18) test_frame_size                      \n");
    tprintf(PRINT_STD, "  (19) test_good_vs_best                    DispKeyFrames\n");
    tprintf(PRINT_STD, "  (20) test_lag_in_frames                   DispResizedFrames\n");
    tprintf(PRINT_STD, "  (21) test_max_quantizer                   DispVisibleFrames\n");
    tprintf(PRINT_STD, "  (22) test_mem_leak                        DispAltRefFrames\n");
    tprintf(PRINT_STD, "  (23) test_mem_leak2                       \n");
    tprintf(PRINT_STD, "  (24) test_min_quantizer                   CropRawClip\n");
    tprintf(PRINT_STD, "  (25) test_multithreaded_dec               PadRawClip\n");
    tprintf(PRINT_STD, "  (26) test_multithreaded_enc               CutClip\n");
    tprintf(PRINT_STD, "  (27) test_new_vs_old_enc_cpu_tick         PasteClip\n");
    tprintf(PRINT_STD, "  (28) test_new_vs_old_psnr                 \n");
    tprintf(PRINT_STD, "  (29) test_noise_sensitivity               PlayDecIVF\n");
    tprintf(PRINT_STD, "  (30) test_one_pass_vs_two_pass            PlayCompIVF\n");
    tprintf(PRINT_STD, "  (31) test_play_alternate                  \n");
    tprintf(PRINT_STD, "  (32) test_post_processor                  CreateSampleTextFiles\n");
    tprintf(PRINT_STD, "  (33) test_reconstruct_buffer              PrintVersion\n");
    tprintf(PRINT_STD, "  (34) test_resample_down_watermark         \n");
    tprintf(PRINT_STD, "  (35) test_speed                           VPXEncPar\n");
    tprintf(PRINT_STD, "  (36) test_test_vector                     RandParFile\n");
    tprintf(PRINT_STD, "  (37) test_thirtytwo_vs_sixtyfour          RandCompress\n");
    tprintf(PRINT_STD, "  (38) test_two_pass_vs_two_pass_best       GraphPSNR\n");
    tprintf(PRINT_STD, "  (39) test_undershoot                      Help\n");
    tprintf(PRINT_STD, "  (40) test_version                         \n");
    tprintf(PRINT_STD, "  (41) test_vpx_matches_int                 \n");
    tprintf(PRINT_STD, "  (42) test_win_lin_mac_match               \n");
#if !defined(_WIN32)
    tprintf(PRINT_STD, "\n");
#endif

    return;
}
void write_32bit_quick_test(const std::string WorkingDir)
{
    char FolderNameChar[255];
    vpxt_folder_name(WorkingDir.c_str(), FolderNameChar);
    std::string TextfileString5 = FolderNameChar;
    TextfileString5.append("QuickTest_32Bit.txt");

    FILE *fp5;

    if ((fp5 = fopen(TextfileString5.c_str(), "w")) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString5.c_str());
        exit(1);
    }

    std::string OSStr = "";

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
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%Verify Encoders and Decoders%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@webm@y4m\n", "test_extra_file", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n", "test_good_vs_best", slashChar(), slashChar());
    fprintf(fp5, "%s@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@..%cTestClips%cMemLeakCheck2_Compression.ivf\n", "test_mem_leak2", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n", "test_one_pass_vs_two_pass", slashChar(), slashChar());
    fprintf(fp5, "%s@%i@..%cTestClips%cTestVectors\n", "test_test_vector", 1, slashChar(), slashChar());
    fprintf(fp5, "%s@%i@..%cTestClips%cTestVectors\n", "test_test_vector", 2, slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n", "test_two_pass_vs_two_pass_best", slashChar(), slashChar());
    fprintf(fp5, "%%%i@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA@webm@y4m\n", "test_win_lin_mac_match", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n", "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n", "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n", "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n", "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@10@webm@y4m\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@10@webm@y4m\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode0%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@6@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode1%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128@webm@y4m\n", "test_allow_drop_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c1-test_change_cpu_enc-cpuused-4.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode2%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode4%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_arnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m@..%cTestClips%cspecial-case-par-files%c5-test_frame_size_settings.txt\n", "test_frame_size", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@10@24@webm@y4m\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cInternal_gNYZH9kuaYM_640x360_2000F.ivf@4@512@webm@ivf@..%cTestClips%cspecial-case-par-files%c4-test_reconstruct_buffer_settings.txt\n", "test_reconstruct_buffer", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode5%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_arnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@10@24@webm@y4m\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    tprintf(PRINT_STD, "\n\nQuick Test file created:\n%s\n\n", TextfileString5.c_str());
    fclose(fp5);
}
void write_64bit_quick_test(const std::string WorkingDir)
{
    char FolderNameChar[255];
    vpxt_folder_name(WorkingDir.c_str(), FolderNameChar);
    std::string TextfileString5 = FolderNameChar;
    TextfileString5.append("QuickTest_64Bit.txt");

    FILE *fp5;

    if ((fp5 = fopen(TextfileString5.c_str(), "w")) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString5.c_str());
        exit(1);
    }

    std::string OSStr = "";

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
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%Verify Encoders and Decoders%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@webm@y4m\n", "test_extra_file", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@vpxenc.exe@vpxdec.exe@webm@y4m\n", "test_vpx_matches_int", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n", "test_good_vs_best", slashChar(), slashChar());
    fprintf(fp5, "%s@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@..%cTestClips%cMemLeakCheck2_Compression.ivf\n", "test_mem_leak2", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n", "test_one_pass_vs_two_pass", slashChar(), slashChar());
    fprintf(fp5, "%s@%i@..%cTestClips%cTestVectors\n", "test_test_vector", 1, slashChar(), slashChar());
    fprintf(fp5, "%s@%i@..%cTestClips%cTestVectors\n", "test_test_vector", 2, slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@128@webm@y4m\n", "test_two_pass_vs_two_pass_best", slashChar(), slashChar());
    fprintf(fp5, "%%%i@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA\n", "test_win_lin_mac_match", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n", "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n", "test_multithreaded_enc", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2@webm@y4m\n", "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2@webm@y4m\n", "test_multithreaded_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@10@webm@y4m\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@10@webm@y4m\n", "test_speed", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode0%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@6@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@0@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode1%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128@webm@y4m\n", "test_allow_drop_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c1-test_change_cpu_enc-cpuused-4.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@1@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode2%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@2@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode4%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_arnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@176@144@webm@y4m@..%cTestClips%cspecial-case-par-files%c5-test_frame_size_settings.txt\n", "test_frame_size", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@10@24@webm@y4m\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cInternal_gNYZH9kuaYM_640x360_2000F.ivf@4@512@webm@y4m@..%cTestClips%cspecial-case-par-files%c4-test_reconstruct_buffer_settings.txt\n", "test_reconstruct_buffer", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@4@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode5%%%%\n");
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_allow_lag", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n", "test_allow_spatial_resampling", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_arnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@6@webm@y4m\n", "test_auto_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_buffer_level", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@0@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@1@webm@y4m\n", "test_change_cpu_dec", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@256@0@webm@y4m@..%cTestClips%cspecial-case-par-files%c0-default-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@256@1@webm@y4m@..%cTestClips%cspecial-case-par-files%c3-test_change_cpu_enc-coverage-settings.txt\n", "test_change_cpu_enc", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@256@10@webm@y4m\n", "test_constrained_quality", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@288@webm@y4m\n", "test_drop_frame_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_data_rate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe@webm@y4m\n", "test_debug_matches_release", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@webm@y4m\n", "test_encoder_break_out", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_error_resolution", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@20@60@webm@y4m\n", "test_fixed_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@24@webm@y4m\n", "test_force_key_frame", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@176@144@webm@y4m\n", "test_frame_size", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@10@24@webm@y4m\n", "test_lag_in_frames", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_max_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m\n", "test_mem_leak", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%cBBB_720x480_2000F.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@webm@y4m@..%cTestClips%cspecial-case-par-files%c2-test_mem_leak_check-allow-spatial-resampling-1.txt\n", "test_mem_leak", slashChar(), slashChar(), slashChar(), slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_min_quantizer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_enc_cpu_tick", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2@webm@y4m\n", "test_new_vs_old_psnr", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_noise_sensitivity", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_play_alternate", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_post_processor", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_reconstruct_buffer", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_resample_down_watermark", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_undershoot", slashChar(), slashChar());
    fprintf(fp5, "%s@..%cTestClips%csrc16.ivf@5@128@webm@y4m\n", "test_version", slashChar(), slashChar());
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    tprintf(PRINT_STD, "\n\nQuick Test file created:\n%s\n\n", TextfileString5.c_str());
    fclose(fp5);
}
int  print_quick_test_files(const std::string WorkingDir)
{
    write_32bit_quick_test(WorkingDir);//32BitQuickRun
    write_64bit_quick_test(WorkingDir);//64BitQuickRun

    return 0;
}
int  vpxt_test_help(const char *InputChar, int printSummary)
{
    std::string TestInputString = InputChar;
    int selector = vpxt_identify_test(InputChar);

    if (TestInputString.compare("0") == 0)
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", InputChar);
        tprintf(PRINT_STD,
                "\nUse:\n\n"
                " 0 run_multipule_tests \n\n"
                "    <Test Type>\n"
                "         <1 - Create Files and Run Tests>\n"
                "         <2 - Create Files only>\n"
                "         <3 - Run Tests For Existing Files - Requires Input Test Directory>\n"
                "    <Input Text File>\n"
                "    <Optional - Input Test Directory>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test runs tests from an input file and summarizes the results. The test can be run in four modes: Mode 1 = Create compressions and run tests, Mode 2 = Create compressions only, Mode 3 = Run tests on pre-existing compressions, and Mode 4 = Resume tests in progress. To create a template driver text file use the command `CreateSampleTextFiles`.");

#if !defined(_WIN32)
        tprintf(PRINT_STD, "\n");
#endif
        return 0;
    }

    get_test_name(selector, TestInputString);

    if (selector == AlWDFNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with Allow Drop Frames on, the second with Allow Drop Frames off. It then records and compares the number of frames each compression has. If Allow Drop Frames on has fewer frames than Allow Drop Frames off, the test passes.");

    }

    if (selector == ALWLGNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with Allow Lag equal to 0, the second with Allow Lag equal to 1. If the correct number of lagged frames are detected via quantizer output, alternate reference frames exist for Allow Lag on, Allow Lag on has the same number of visible frames as Allow Lag off, and Allow Lag on is not identical to Allow Lag off, the test passes.");
    }

    if (selector == ALWSRNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with Allow Spatial Resampling off, the second with Allow Spatial Resampling on. The test then records the number of resized frames for each and computes the PSNR for Allow Spatial Resampling on. If Allow Spatial Resampling on has resized frames, Allow Spatial Resampling off does not have resized frames and Allow Spatial Resampling on has a PSNR greater than 15, the test passes.");
    }

    if (selector == ARNRTNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "    <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates four compressions: the first with Alternate Reference Frames off, the second with Alternate Reference Frames on and an ARNR Type equal to one, the third with Alternate Reference Frames on and an ARNR Type equal to two, the fourth with Alternate Reference Frames on and an ARNR Type equal to three.  The test then computes PSNR values and Data Rates for all files.  If all compressions are not identical, all PSNR values increase as ARNR Type increases or are within five percent of the Alternate Reference Frames off compression, and Data Rates decrease as ARNR Type increases or are within five percent of the Alternate Reference Frames off compression, the test passes.");
    }

    if (selector == AUTKFNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions with identical parameters, using user input Auto Key Frames. The test then records the placement of each compression's key frames. If both compressions' key frames occur in identical locations and at least as frequently as Auto Key Frame dictates, the test passes.");

    }

    if (selector == BUFLVNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression and runs CheckPBM on it. If no buffer underrun is detected, the test passes.");

    }

    if (selector == CPUDENUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression of the user input version (0-3) and then decompresses it using mmx, sse, sse2, sse3, ssse3 and sse4_1 instruction sets. The test then compares them against one another. If all compressions are identical and the times to decompress them are not, the test passes.");

    }

    if (selector == CPUENNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates compressions of the user input version (0-3) using mmx, sse, sse2, sse3, ssse3 and sse4_1 instruction sets. If all compressions are identical and compression times are not, the test passes.");
    }

    if (selector == CONQUNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Constrained Quantizer>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
                "\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with constrained quality off, the second with user input constrained quality.  The test then records their PSNRs and Data Rates.  If linear PSNR per Data Rate is greater for user input Constrained Quality, the test passes.");
    }

    if (selector == DTARTNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression and records its data rate. If the compression's data rate is within 30%% of the input target bandwidth, the test passes.");

    }

    if (selector == DBMRLNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first using an executable built using the newest release library, the second using an executable built using the newest debug library. If the two compressions are identical, the test passes.");

    }

    if (selector == DFWMWNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates 6 compressions with DFWM values of 100, 80, 60, 40, 20 and 0, and records their sizes. If each successively lower DFWM compression has an equal or larger size than the previous, the test passes.");

    }

    if (selector == ENCBONUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates four compressions: the first has an EncodeBreakout of 0, the second EncodeBreakout of 100, the third EncodeBreakout of 500 and the fourth EncodeBreakout of 1000. Decompressions of the encoded files are then carried out and PSNR values are calculated. If the decompressions run successfully and the PSNR values of each successive EncodeBreakout trial are within 2 dB, the test passes. If the PSNRs are greater than 2 dB but less than 5 dB the test is inconclusive; if the PSNRs have greater than a 5 dB difference the test fails.");
    }

    if (selector == ERRMWNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with Error Resilient Mode off, the second with Error Resilient Mode on. The test then records their PSNR values. If their PSNRs are within 10%% of each other, the test passes.");

    }

    if (selector == EXTFINUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a two-pass compression and checks for extra files in the current directory, the directory the executable is located in, and the directory the output file is written to. If no extra files are found, the test passes.");
    }

    if (selector == FIXDQNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <FixedQ 1>\n"
                "    <FixedQ 2>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions each with user input Fixed Quantizers, and records the quantizers used to encode each frame. The test then records the compressions' sizes. If all quantizers for each compression match the input Fixed Quantizer, and the smaller quantizer's compression has a larger file size, the test passes.");
    }

    if (selector == FKEFRNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression using a user-input value as a Force Key Frame Interval. The Test then records the placement of all key frames in the compression. If key frames occur only when Force Key Frame dictates, the test passes.");

    }

    if (selector == FRSZTNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates forty-six cropped raw ivf or y4m files from a user inputfile, starting height, and starting width (where starting height and starting width must be multiples of 16).  The test then creates compressions for each raw file and computes its PSNR.  If all compressions PSNRs are within five percent of the starting width/height file's PSNR and all PSNRs are greater than 25 the test passes.");
    }

    if (selector == GQVBQNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates six compressions: the first and fourth compressions for 30%% less than the input target bandwidth at good quality and best quality, the second and fifth compressions for the input target bandwidth at good quality and best quality, and the third and sixth at 30%% more than the input target bandwidth at good quality and best quality. The test then records each file's data rate and PSNR, and computes the area under the curve for the common interval between the good quality curve and best quality curve. If the area under the best quality curve is greater than the area under the good quality curve, the test passes.");
    }

    if (selector == LGIFRNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates three compressions: the first with Allow Lag set to 0, the second and third with Allow Lag set to 1. The second compression uses the first user-input Lag in Frames value for its Lag in frames and the third uses the second user-input value for its Lag in Frames. The test outputs each file's quantizer values for each encoded frame. If none of the files are identical, the PSNRs of each successive file are within 10%% of the last, and the quantizer output shows that the proper number of frames were lagged, the test passes.");
    }

    if (selector == MAXQUNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates nine compressions, the first with a Max Quantizer equal to 4 and each subsequent file with a Max Quantizer 8 greater than the last until 63. The test records the individual quantizer values for each encoded frame. If the PSNRs of each Max Quantizer compression from 4 to 63 increase as Max Quantizer decreases and the recorded quantizers for each file do not exceed their corresponding Max Quantizer for all compressions, the test passes.");

    }

    if (selector == MEML1NUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression using the debug executable to check memory usage and records the results to an output file. If no memory leaks are found, the test passes.");
    }

    if (selector == MEML2NUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Mem Leak Check Exe>\n"
                "\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test uses the debug executable to open and close 10,000 instances of the encoder and 10,000 instances of the decoder, and then checks for no memory leaks. If there are no leaks, the test passes.");
    }

    if (selector == MINQUNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with a Min Quantizer equal to 10, the second with a Min Quantizer equal to 60. It records the quantizer used for each frame of the compression. If the first file has a higher PSNR than the second file and every quantizer for both files is above the corresponding Min Quantizer, the test passes.");
    }

    if (selector == MULTDNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "    <Target Bit Rate>\n"
                "    <Number of Cores to Use>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression with a user input number of token partitions.  It then preforms two decompressions on this compression.  The first decompression using 0 threads,  The second decompression using a user input number of threads.  If the user input number of threads takes less time to decompress and the two decompressions are identical, the test passes.");
    }

    if (selector == MULTENUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "    <Target Bit Rate>\n"
                "    <Number of Cores to Use>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first using a MultiThreaded equal to 2, the second using a MultiThreaded equal to 0. The test then compares the times to compress each. If MultiThreaded 2 is faster than 0, the test passes.");
    }

    if (selector == NVOECPTK)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Target Bit Rate>\n"
                "    <Exe File To Compare>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first using the newest version of VP8, the second using a separate executable built using an older version. The test records the time that each compression took. If the new compression's time is at least 10%% faster than the old compression's time, the test passes.");
    }

    if (selector == NVOPSNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
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
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
                "\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first using the newest version of VP8, the second using a separate executable built using an older version. The test then computes and records PSNR values for each. If the new compressions PSNR is greater than the old compressions PSNR (or within 1%%), the test passes.");
    }

    if (selector == NOISENUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates seven compressions with Noise Sensitivity values from 0 to 6 and computes PSNR values for each. If all compressions have differing PSNR values and Noise Sensitivity 0 has a higher PSNR than Noise Sensitivity 6, the test passes.");
    }

    if (selector == OV2PSNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates six compressions: the first and fourth compressions for 30%% less than the input target bandwidth at one-pass good quality and two-pass good quality, the second and fifth compressions for the input target bandwidth at one-pass good quality and two-pass good quality, and the third and sixth at 30%% more than the input target bandwidth at one-pass good quality and two-pass good quality. The test then records each file's data rate and PSNR and computes the area under the curve for the common interval between the one-pass good quality curve and the two-pass good quality curve. If the area under the two-pass good quality curve is greater than the area under the one-pass good quality curve, the test passes.");
    }

    if (selector == PLYALNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with Play Alternate equal to 0, the second with Play Alternate equal to 1. The test then records the placement of alternate reference frames and visible frames for both compressions. If alternate reference frames exist for Play Alternate = 1 and not for Play Alternate = 0, visible frames for Play Alternate 1 and Play Alternate 2 are equal, and the files are not identical, the test passes.");
    }

    if (selector == POSTPNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression, then creates a No Filtering decompression, and decompressions for Deblock and Noise levels ranging from 0 to 15. If all Deblock and Noise decompressions return a different PSNR than the No Filtering Decompression but are within 10%%, the test passes.");
    }

    if (selector == RECBFNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );


        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates a compression and internally compares the compressor's preview frames to the decoded output produced by decompressing the compressor's encoded frame. The state of each frame is recorded to a text file. If the contents of all preview frames are identical to the content of all decoded frames, the test passes.");
    }

    if (selector == RSDWMNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with resample-down-watermark set to 90, the second with resample-down-watermark set to 10. The test then records the frames at which the file buffer reaches the designated thresholds, the location of key frames, and location of resized frames for both files. If the first resized frame occurs on the first instance where the frame prior to a key frame reaches the correct buffer saturation for both compressions, the test passes.");
    }

    if (selector == SPEEDNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "    <Target Bit Rate>\n"
                "    <Lag in Frames>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test works for RealTime Mode and Good Quality Mode. For RealTime Mode the test creates compressions for CpuUsed Values from -1 to -16 and 0 to 16. For Good Quality Mode the test creates compressions for CpuUsed Values from 0 to 5. If compression speed increases as CpuUsed increases and all PSNRs are within 10%% of the previous, the test passes.");
    }

    if (selector == TVECTNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Threads>\n"
                "    <Input Directory>\n"
                "\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test decodes each VP8 Test Vector using the number of threads input and Checks its MD5 checksum against the expected value. If all Test Vectors decode properly and all MD5 checksums match their expected values, the test passes.");

    }

    if (selector == TTVSFNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "    <Target Bit Rate>\n"
                "    <Output Directory>\n"
                "    <Settings File>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "");

    }

    if (selector == TV2BTNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates six compressions: the first and fourth compressions for 30%% less than the input target bandwidth at two-pass good quality and two-pass best quality, the second and fifth compressions for the input target bandwidth at two-pass good quality and two-pass best quality, and the third and sixth at 30%% more than the input target bandwidth at two-pass good quality and two-pass best quality. The test then records each file's data rate and PSNR and computes the area under the curve for the common interval between the two-pass good quality curve and the two-pass best quality curve. If the area under the two-pass best quality curve is greater than the area under the two-pass good quality curve, the test passes.");
    }

    if (selector == UNDSHNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates two compressions: the first with an undershoot equal to 10, the second with an undershoot equal to 100. If the Undershoot 100 compression's file size is greater than the Undershoot 10 compression's file size, the test passes.");
    }

    if (selector == VERSINUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test creates four compressions: the first with Version equal to 0, the second with Version equal to 1, the third with Version equal to 2, and the fourth with Version equal to 3. The test then decodes each and records the time it took to do so. If each successive Version takes less time than the prior to decode and has a lower PSNR, the test passes.");
    }

    if (selector == VPXMINUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Vpxenc Executable>\n"
                "    <Vpxdec Executable>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional Settings File>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test takes a raw input file along with vpxenc and vpxdec paths and produces two encode files and two decode files.  The first encode file is produced using the testers internal compressor.  The second encode file is produced using the passed in vpxenc path.  The first decode file is produced using the passed in vpxdec path.  The second decode file is produced using the testers internal decoder.  The test then compares the encoded and decoded files.  If all decoded and encoded files are identical the test passes.");
    }

    if (selector == WMLMMNUM)
    {
        if (printSummary)
            tprintf(PRINT_STD, "\nUse:\n\n%2i", selector);
        else
            tprintf(PRINT_STD, " ");

        vpxt_cap_string_print(PRINT_STD, " %s\n", TestInputString.c_str());

        tprintf(PRINT_STD, "\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "    <Target Bit Rate>\n"
                "    <Output Directory>\n"
                "    <Settings File>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
               );

        if (printSummary)
            vpxt_formated_print(HLPPRT, "The test can be run in two test modes. The first, Mode 0, creates platform-specific compressions and decompressions to be tested on another platform. The second mode, Mode 1, creates platform-specific compressions and decompressions, and then compares them to previously encoded and decoded files created by Mode 0. If the files are identical, the test passes.");
    }

#if !defined(_WIN32)

    if (!printSummary)
        tprintf(PRINT_STD, "\n");

#endif

    return 0;
}
int  vpxt_tool_help(const char *InputChar, int printSummary)//return 1 if string found return 0 if string not found if string not found TestHelp will be run through.
{
    std::string InputString = InputChar;
    vpxt_lower_case_string(InputString);

    if (InputString.compare("vpxtenc") == 0)
    {
        tprintf(PRINT_STD,
                "\n  VPXT Compress\n\n"
                "    <(1)Normal Compress |(2)TimeCompress>\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <Bit Rate>\n"
                "    <Mode>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Optional - Parameter File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw IVF or Y4M file and produce an encoded IVF or WEBM file using the given mode and bitrate. Default encode settings can be overridden by specifying a parameter file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxtdec") == 0)
    {
        tprintf(PRINT_STD,
                "\n  VPXT Decompress\n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <Decode Format - y4m/ivf>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an encoded IVF or WEBM file and output a decoded IVF or Y4M file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxtdec2raw") == 0)
    {
        tprintf(PRINT_STD,
                "\n  VPXT Decompress to Raw \n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an encoded IVF or WEBM file and output a decoded raw file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxtdatarate") == 0)
    {
        tprintf(PRINT_STD,
                "\n  VPXT DataRate \n\n"
                "    <Input File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an IVF or WEBM file and compute its average, min, max, and file data rates.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxtpsnr") == 0)
    {
        tprintf(PRINT_STD,
                "\n  VPXT PSNR\n\n"
                "     <Raw IVF File>\n"
                "     <Comp IVF File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will compute an encoded IVF or WEBM file's PSNR using the encoded file's IVF or Y4M source file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxtcheckpbm") == 0)
    {
        tprintf(PRINT_STD,
                "\n  VPXT Check PBM\n\n"
                "     <input>\n"
                "     <bitrate>\n"
                "     <bufferSize>\n"
                "     <prebuffer>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will run CheckPBM to make sure a buffer underrun won't occur.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("raw2formatted") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Raw To Formatted\n\n"
                "    <Input File>\n"
                "    <OutPutDir>\n"
                "    <IVF/Y4M>\n"
                "    <Width>\n"
                "    <Height>\n"
                "    <FrameRate>\n"
                "    <FourCC>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw file and produce a raw IVF or Y4M file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("formatted2raw") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Formatted To Raw \n\n"
                "    <Input File>\n"
                "    <Output File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an IVF or Y4M file and produce a raw file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("formatted2rawframes") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Formatted To RawFrames\n\n"
                "    <Input File>\n"
                "    <OutPutDir>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in an IVF or Y4M file and produce individual raw frames for each frame that exists in a user-specified directory.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("combineindvframes") == 0)
    {
        tprintf(PRINT_STD, "\n  Combine Indv Frames");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will combine all individual decoded frames in a directory into a single raw file in numerical order.");

        tprintf(PRINT_STD,
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
                "       Output File = Path to desired output file\n"
               );

#if !defined(_WIN32)
        tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (InputString.compare("compareenc") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Compare Enc\n\n"
                "     <inputfile1>\n"
                "     <inputfile2>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will compare the video content of two IVF or WEBM files and will display if they are identical or, if they differ, the first frame they differ at.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("compareheader") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Compare Header Info \n\n"
                "    <inputfile1>\n"
                "    <inputfile2>\n"
                "    <Full File Info 1/yes-0/no>\n"
                "    <OutputFile>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will compare the file and frame headers of two IVF or WEBM files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("displayheader") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Display Header Info \n\n"
                "    <Input File>\n"
                "    <Full File Info 1/yes-0/no>\n"
                "    <Optional Outputfile>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the file and frame headers of an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("dispkeyframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Display Key Frames \n\n"
                "    <Input IVF File>\n"
                "    <Write to file 0 | 1 Print to screen>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location of key frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("dispresizedframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Display Resized Frames \n\n"
                "    <Input IVF File>\n"
                "    <Write to file 0 | 1 Print to screen>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location of resized frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("dispvisibleframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Display Visible Frames \n\n"
                "    <Input IVF File>\n"
                "    <Write to file 0 | 1 Print to screen>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location of visible frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("dispaltrefframes") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Display AltRef Frames \n\n"
                "    <Input IVF File>\n"
                "    <Write to file 0 | 1 Print to screen>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will display the location of alternate reference frames within an IVF or WEBM file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }


    if (InputString.compare("cutclip") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Cut Clip\n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <Starting Frame>\n"
                "    <Ending Frame>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will cut a portion of an IVF or Y4M file starting at Starting Frame and ending at Ending Frame to a new output file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("pasteclip") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Paste Clip\n\n"
                "    <Inputfile1>\n"
                "    <Inputfile2>\n"
                "    <Outputfile>\n"
                "    <First Paste Frame>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will paste the contents of Inputfile2 into Inputfile1 starting at Inputfile1's First Paste Frame to a new output file for raw IVF and Y4M files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("playdecivf") == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD,
                "\n  Play Dec IVF\n\n"
                "    <Input File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will convert an uncompressed IVF or Y4M file to a raw YUV file and play it using tmnplay or mplayer.  Please be sure to put a copy of tmnplay in your C:\\bin directory.  Tmnplay can be found in the TestClips directory.");

#else
        tprintf(PRINT_STD, "\n  PlayDecIVF");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This Tool will convert an uncompressed ivf file to a raw yuv file and play it using mplayer.  To install mplayer open a command console and type:");

        tprintf(PRINT_STD, "\n    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
                "    cd mplayer\n"
                "    ./configure\n"
                "    make\n"
                "    make install\n"
               );

        tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (InputString.compare("playcompivf") == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD,
                "\n  Play Comp IVF\n\n"
                "    <Input File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will convert a compressed IVF or WEBM file to a raw YUV file and play it using tmnplay or mplayer.  Please be sure to put a copy of tmnplay in your C:\\bin directory.  Tmnplay can be found in the TestClips directory.");

#else
        tprintf(PRINT_STD, "\n  PlayCompIVF");

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This Tool will convert a compressed ivf file to a raw yuv file and play it using mplayer.  To install mplayer open a command console and type:");

        tprintf(PRINT_STD, "\n    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
                "    cd mplayer\n"
                "    ./configure\n"
                "    make\n"
                "    make install\n"
               );

        tprintf(PRINT_STD, "\n");
#endif
        return 1;
    }

    if (InputString.compare("createsampletextfiles") == 0)
    {
        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will create sample text files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("printversion") == 0)
    {
        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will print the version of VP8 being used by the tester.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("randparfile") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Random Par File\n\n"
                "    <Output Par File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will create a valid VP8 random parameter file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("randcompress") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Random Compress\n\n"
                "    <Input File>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will create a compression using random parameters for an input IVF or Y4M file.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("graphpsnr") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  Graph PSNR\n\n"
                "    <Inputfile 1>\n"
                "    <Starting Bit Rate>\n"
                "    <Ending Bit Rate>\n"
                "    <Bit Rate Step>\n"
                "    <Optional - Par Input>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool creates a series of compressions from user-input values for starting bit rate, ending bit rate, and an increment. Output files are created, and the utility computes and outputs the data rates and PSNRs of the files.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
    }

    if (InputString.compare("testvectorindex") == 0)
    {
        tprintf(PRINT_STD, "\n  TestVectorIndex\n");

        if (printSummary)
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
#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxenc") == 0)
    {
        tprintf(PRINT_STD, "\n"
                "  VPXEnc\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "VPXEnc.exe clone");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxdec") == 0)
    {
        tprintf(PRINT_STD,
                "\n  VPXDec\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "VPXDec.exe clone");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("croprawclip") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Crop Raw Clip\n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <xoffset>\n"
                "    <yoffset>\n"
                "    <New Frame Width>\n"
                "    <New Frame Height>\n"
                "    <Raw/IVF 0-Raw 1-IVF>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw IVF or Y4M file and produce a cropped raw IVF or Y4M file using the input size parameters.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("padrawclip") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Pad Raw Clip\n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <New Frame Width>\n"
                "    <New Frame Height>\n"
                "    <Raw/IVF 0-Raw 1-IVF>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will take in a raw IVF or Y4M file and produce a padded raw IVF or Y4M file using the input size parameters.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("vpxencpar") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Convert Parameter File to vpxenc settings\n\n"
                "     <input core file>\n"
                "     <input api file>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "This tool will convert tester parameters into vpxenc parameters.");

#if !defined(_WIN32)
        else
            tprintf(PRINT_STD, "\n");

#endif
        return 1;
    }

    if (InputString.compare("writeivffileheader") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Write IVF File Header\n\n"
                "     <Output File>\n"
                "     <Width>\n"
                "     <Height>\n"
                "     <Scale>\n"
                "     <Rate>\n"
                "     <Length>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }

    if (InputString.compare("writeivfframeheader") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Write IVF Frame Header\n\n"
                "     <Output File>\n"
                "     <Time Stamp>\n"
                "     <Frame Size>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }

    if (InputString.compare("writeframedata") == 0)
    {
        tprintf(PRINT_STD,
                "\n  Write IVF Frame Data\n\n"
                "     <Input File>\n"
                "     <Output File>\n"
                "     <Frame Size>\n"
               );

        if (printSummary)
            vpxt_formated_print(TOLPRT, "");

        return 1;
    }

    return 0;
}
void format_summary(const char *InputFileNameCharAr)
{
    tprintf(PRINT_STD, "\n Formating Summary file.\n");

    std::string InputFileName = InputFileNameCharAr;

    //std::string SummaryByTestOutput = InputFileName;
    //SummaryByTestOutput.erase(SummaryByTestOutput.end() - 4, SummaryByTestOutput.end());
    std::string SummaryByTestOutput;
    vpxt_remove_file_extension(InputFileName.c_str(), SummaryByTestOutput);
    SummaryByTestOutput.append("expanded.txt");

    char TestsRunChar[255];
    vpxt_folder_name(InputFileNameCharAr, TestsRunChar);
    std::string TestsRun = TestsRunChar;
    TestsRun.append("tests_run.txt");

    FILE *fp;

    if ((fp = freopen(SummaryByTestOutput.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open summary out put file: %s\n", SummaryByTestOutput.c_str());
        return;
    }

    std::string LineBuffer = "";
    std::string TestTrackerName = "";
    std::string TestTrackerResult = "";

    int TestTracker = 1;
    int ResultTracker = 0;

    //loop through all existing tests alphabeticaly
    while (TestTracker <= MAXTENUM)
    {
        ResultTracker = 0;

        bool PrintTestHeader1 = 1;

        //This iterates through all possible tests run using the test tracker names to specify which was run the number is only to drive
        //The iteration through and does not represent test number
        if (TestTracker == AlWDFNUM)
            TestTrackerName = "Test_Allow_Drop_Frames";

        if (TestTracker == ALWLGNUM)
            TestTrackerName = "Test_Allow_Lag";

        if (TestTracker == ALWSRNUM)
            TestTrackerName = "Test_Allow_Spatial_Resampling";

        if (TestTracker == ARNRTNUM)
            TestTrackerName = "Test_Arnr";

        if (TestTracker == AUTKFNUM)
            TestTrackerName = "Test_Auto_Key_Frame";

        if (TestTracker == BUFLVNUM)
            TestTrackerName = "Test_Buffer_Level";

        if (TestTracker == CPUDENUM)
            TestTrackerName = "Test_Change_Cpu_Dec";

        if (TestTracker == CPUENNUM)
            TestTrackerName = "Test_Change_Cpu_Enc";

        if (TestTracker == CONQUNUM)
            TestTrackerName = "Test_Constrained_Quality";

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

        if (TestTracker == MULTDNUM)
            TestTrackerName = "Test_Multithreaded_Dec";

        if (TestTracker == MULTENUM)
            TestTrackerName = "Test_Multithreaded_Enc";

        if (TestTracker == NVOPSNUM)
            TestTrackerName = "Test_New_Vs_Old_Psnr";

        if (TestTracker == NVOECPTK)
            TestTrackerName = "Test_New_Vs_Old_Enc_Cpu_Tick";

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
            TestTrackerName = "Test_Speed";

        if (TestTracker == TVECTNUM)
            TestTrackerName = "Test_Test_Vector";

        if (TestTracker == TTVSFNUM)
            TestTrackerName = "Test_Thirtytwo_Vs_Sixtyfour";

        if (TestTracker == TV2BTNUM)
            TestTrackerName = "Test_Two_Pass_Vs_Two_Pass_Best";

        if (TestTracker == UNDSHNUM)
            TestTrackerName = "Test_Undershoot";

        if (TestTracker == VERSINUM)
            TestTrackerName = "Test_Version";

        if (TestTracker == VERSINUM)
            TestTrackerName = "Test_Vpx_Matches_Int";

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

            std::fstream FormatSummaryByTestFile;
            FormatSummaryByTestFile.open(InputFileName.c_str());

            if (!FormatSummaryByTestFile)
            {
                tprintf(PRINT_STD, "Cannot open input file.");
                return;
            }

            int p = 0;
            bool PrintTestHeader2 = 1;

            //read in and throw away header
            while (p < 11)
            {
                //FormatSummaryByTestFile.getline(buffer, 1024);
                getline(FormatSummaryByTestFile, LineBuffer);
                p = p + 1;
            }

            while (!FormatSummaryByTestFile.eof() && LineBuffer[0] != '-')
            {
                getline(FormatSummaryByTestFile, LineBuffer);

                if (LineBuffer.length() > 60)
                {
                    int n = 5;

                    while (n < 39 && LineBuffer[n] != ' ')
                        n = n + 1;

                    std::string TestReadString = LineBuffer.substr(5, n - 5);

                    n = 62;

                    while (n < 194 && LineBuffer[n] != ' ' != 0 && n < LineBuffer.length())
                        n = n + 1;

                    std::string TestResultString = LineBuffer.substr(62, n - 62);

                    //check to see if names and states match up
                    if (TestReadString.compare(TestTrackerName) == 0 && TestResultString.compare(TestTrackerResult) == 0)
                    {
                        if (ResultTracker == 1)
                            PassedTrack = PassedTrack + 1;

                        if (ResultTracker == 0)
                            FailedTrack = FailedTrack + 1;

                        if (ResultTracker == 2)
                            IndeterminateTrack = IndeterminateTrack + 1;

                        if (ResultTracker == 3)
                            SeeComboRunLogTrack = SeeComboRunLogTrack + 1;

                        if (ResultTracker == 4)
                            SeePSNRLogTrack = SeePSNRLogTrack + 1;

                        if (ResultTracker == 8)
                            MinTestPassed = MinTestPassed + 1;

                        if (ResultTracker == 10)
                            CompressionMadeTrack = CompressionMadeTrack + 1;

                        if (ResultTracker == 11)
                            ErrorFileMismatchTrack = ErrorFileMismatchTrack + 1;

                        if (ResultTracker == 12)
                            TestNotSupportedTrack = TestNotSupportedTrack + 1;

                        if (PrintTestHeader1)
                        {
                            //print test header
                            int leftLineSize = 75 - (TestReadString.size() / 2);
                            int counter = 0;

                            fprintf(stderr, "\n\n");

                            while (counter < leftLineSize)
                            {
                                fprintf(stderr, "=");
                                counter = counter + 1;
                            }

                            fprintf(stderr, "%s", TestReadString.c_str());
                            counter = counter + TestReadString.size();

                            while (counter < 150)
                            {
                                fprintf(stderr, "=");
                                counter = counter + 1;
                            }

                            fprintf(stderr, "\n");
                            PrintTestHeader1 = 0;
                        }

                        if (PrintTestHeader2)
                        {
                            fprintf(stderr, "\n %s:\n\n", TestResultString.c_str());
                            fprintf(stderr, "%4s %-32s%-25s%s  Test Settings\n\n", "#", "            Test Name" , "     Date and Time", "Status");
                            PrintTestHeader2 = 0;
                        }

                        fprintf(stderr, "%s ", LineBuffer.c_str());

                        ///////////////////Get the correct Test Input settings and output them to summary.//////////////
                        std::fstream TestsRunFile;
                        TestsRunFile.open(TestsRun.c_str(), std::fstream::in);

                        if (!TestsRunFile)
                        {
                            TestsRunFile.close();
                            FormatSummaryByTestFile.close();
                            fclose(fp);
                            tprintf(PRINT_BTH, "\nFile creation Canceled: Cannot find %s\n", TestsRun.c_str());
                            return;
                        }

                        int TestRunNumInt = 0;
                        std::stringstream TestRunNum(LineBuffer.substr(0, 4));
                        TestRunNum >> TestRunNumInt;

                        int TestNumberCur = -1;

                        while (!TestsRunFile.eof() && TestNumberCur != TestRunNumInt)
                        {
                            std::string TestsRunString = "";
                            getline(TestsRunFile, TestsRunString);
                            std::string test = TestsRunString.substr(0, 1);

                            if (TestsRunString[0] == '+')
                                TestNumberCur = TestNumberCur + 1;

                            if (TestNumberCur == TestRunNumInt)
                            {
                                TestsRunString[0] = ' ';
                                int t = 0;

                                while (t < TestsRunString.length())
                                {
                                    if (TestsRunString[t] == '@')
                                        TestsRunString[t] = ' ';

                                    t = t + 1;
                                }

                                fprintf(stderr, "%s\n", TestsRunString.c_str());
                            }
                        }

                        TestsRunFile.close();
                        //////////////////////////////////////////////////////////////////////////////////////////////
                    }
                }
            }

            FormatSummaryByTestFile.close();
            ResultTracker = ResultTracker + 1;
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

        TestTracker = TestTracker + 1;
    }

    fclose(fp);
    return;
}
int  show_hidden_cmds()
{
    tprintf(PRINT_STD, "\n\n"
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
            "         VPXTDec2Raw               -Decode an IVF file to a RAW File\n"
            "         DeleteAllIVFFiles        -Deletes all IVF Files in a dir DANGEROUS!!!\n"
            "         Raw2Formatted                  -Converts a RAW File to an IVF File\n"
            "         SideBySideText           -Creates a txt file with two input coloums\n"
            "         Formatted2RawFrames            -Writes out individual Frame data to a dir\n"

            "\n");
    return 0;

}
int  vpxt_run_multi(int argc, const char *argv[], std::string WorkingDir)
{
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "  run_multipule_tests \n\n"
                "    <Test Type>\n"
                "      <1 - Create Files and Run Tests>\n"
                "        <Input Text File>\n"
                "        <Delete IVF Files| 0=no 1=yes>\n"
                "      <2 - Create Files only>\n"
                "        <Input Text File>\n"
                "      <3 - Run Tests For Existing Files>\n"
                "        <Input Test Directory>\n"
                "        <Delete IVF Files| 0=no 1=yes>\n"
                "      <4 - Resume Test>\n"
                "        <Input Test Directory>\n"
                "        <Delete IVF Files| 0=no 1=yes>\n"
                "\n");

        return 0;
    }

    int VaildInput = 0;

    if (atoi(argv[2]) != 4 && atoi(argv[2]) != 3)
    {
        VaildInput = vpxt_run_multiple_tests_input_check(argv[3], 0);

        if (VaildInput <= 0)
        {
            tprintf(PRINT_STD, "\nExternal Test Runner Aborted\n\n");
            return 0;
        }
    }
    else
    {
        std::string inputCheck = "";
        inputCheck.append(argv[3]);
        inputCheck.append(slashCharStr().c_str());
        inputCheck.append("tests_run.txt");
        VaildInput = vpxt_run_multiple_tests_input_check(inputCheck.c_str(), 0);

        if (VaildInput <= 0)
        {
            tprintf(PRINT_STD, "\nExternal Test Runner Aborted\n\n");
            return 0;
        }
    }

    //check to see if we should run a lean multi run
    int DeleteIVFFiles = 0;

    if (argc > 4)
    {
        std::string LeanCheck = argv[4];
        vpxt_lower_case_string(LeanCheck);

        if (LeanCheck.compare("lean") == 0)
            DeleteIVFFiles = 1;
    }

    run_multiple_tests(argc, argv, WorkingDir.c_str(), VaildInput, DeleteIVFFiles);

    std::string SummaryFile = WorkingDir;

    if (atoi(argv[2]) == 1)
    {
        char SummaryFileChar[255] = "";
        SummaryFile.erase(SummaryFile.end() - 1);
        SummaryFile.append(slashCharStr());
        SummaryFile.append("test_results.txt");
        snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
        format_summary(SummaryFileChar);
        return 0;
    }

    if (atoi(argv[2]) == 3)
    {

        char SummaryFileChar[255] = "";
        SummaryFile.erase(SummaryFile.end() - 1);
        SummaryFile.append(slashCharStr());
        SummaryFile.append("compression_results.txt");
        snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
        format_summary(SummaryFileChar);
        return 0;
    }

    if (atoi(argv[2]) == 4)
    {
        std::string SummCompAndTest = "";
        std::string SummComp = "";
        std::string SummTest = "";

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
        std::fstream SummCompAndTestFile;
        SummCompAndTestFile.open(SummCompAndTest.c_str());

        if (SummCompAndTestFile)
        {
            TestRunning = 1;
        }

        std::fstream SummTestFile;
        SummTestFile.open(SummTest.c_str());

        if (SummTestFile)
        {
            TestRunning = 3;
        }

        std::fstream SummCompFile;
        SummCompFile.open(SummComp.c_str());

        if (SummCompFile)
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
            format_summary(SummaryFileChar);
            return 0;
        }

        if (TestRunning == 3)
        {

            char SummaryFileChar[255] = "";
            SummaryFile.append(slashCharStr());
            SummaryFile.append("compression_test_results.txt");
            snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
            format_summary(SummaryFileChar);
        }
    }

    return 0;
}
int  main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        vpxt_on_error_output();
        return 0;
    }

    std::string EmptyAr[1];
    char WorkingDirChar[256]  = "";
    create_working_folder(argc, argv, WorkingDirChar);
    std::string WorkingDir = "";
    WorkingDir.append(WorkingDirChar);
    std::string TestInputString(argv[1]);
    std::string TestInputString2 = "";

    if (argc > 2)
        TestInputString2 =  argv[2];

    vpxt_lower_case_string(TestInputString);
    vpxt_lower_case_string(TestInputString2);

    if (TestInputString.compare("0") == 0)
    {
        if (WorkingDir.length() > 175)
        {
            tprintf(PRINT_BTH, "\nError: Folder Path Length too long.\n");
            return 0;
        }
    }

    /////////////////////////////////////////////////////////////Help//////////////////////////////////////////////////////////

    if (TestInputString.compare("help") == 0)
    {
        vpxt_on_error_output();
        vpxt_formated_print(TOLPRT, "To run tests input the number to the left of the test name along with test specific input.  To obtain more information about a specific test or utility input the number of the test or the utility name you wish to inquire about followed by \"help\".");
        return 0;
    }

    if (TestInputString2.compare("help") == 0)
    {
        int ToolHelpCheck = vpxt_tool_help(TestInputString.c_str(), 1);

        if (!ToolHelpCheck)
        {
            vpxt_test_help(argv[1], 1);
        }

        return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////Tools//////////////////////////////////////////////////////////
    ////////////////////Public Commands/////////////////////////

    if (TestInputString.compare("testprint") == 0)                          //Prints sample text in several formats
        return tool_test_print();

    if (TestInputString.compare("quad") == 0)                               //Solves Quadradic
        return tool_solve_quad();

    if (TestInputString.compare("vpxtenc") == 0)                            //Compresses an IVF Raw File to an IVF Compressed file
        return tool_vpxt_enc(argc, argv, WorkingDir);

    if (TestInputString.compare("thresh") == 0)                             //runs checkpbm (this needs to be looked at)
        return tool_run_thresh(argc, argv);

    if (TestInputString.compare("vpxtdec") == 0)                            //Decompresses and IVF Compressed file to an IVF Raw File
        return tool_vpxt_dec(argc, argv);

    if (TestInputString.compare("vpxtdatarate") == 0)                        //Computes Data Rate for an input IVF File
        return tool_vpxt_data_rate(argc, argv);

    if (TestInputString.compare("vpxtpsnr") == 0)                            //Computes PSNR for two input files
        return tool_vpxt_psnr_run(argc, argv);

    if (TestInputString.compare("vpxtcheckpbm") == 0)                        //Computes PBM For an input file
        return tool_vpxt_check_pbm_run(argc, argv);

    if (TestInputString.compare("compareheader") == 0)                      //Compares the Headers of two ivf files
        return tool_compare_header_info(argc, argv);

    if (TestInputString.compare("displayheader") == 0)                      //Displays the header contents of an ivf file
        return tool_display_header_info(argc, argv);

    if (TestInputString.compare("compareenc") == 0)                         //Compares two ivf files
        return tool_compare_enc(argc, argv);

    if (TestInputString.compare("comparedec") == 0)                         //Compares two ivf files
        return tool_compare_dec(argc, argv);

    if (TestInputString.compare("createsampletextfiles") == 0)              //Creates sample text files that include quick test and other templates
        return print_quick_test_files(WorkingDir);

    if (TestInputString.compare("cutclip") == 0)                             //Modifies an ivf files size
        return tool_cut_ivf(argc, argv);

    if (TestInputString.compare("croprawclip") == 0)                         //Modifies an ivf files size
        return tool_crop_raw_clip(argc, argv);

    if (TestInputString.compare("padrawclip") == 0)                         //Modifies an ivf files size
        return tool_pad_raw_clip(argc, argv);

    if (TestInputString.compare("pasteclip") == 0)                           //Modifies an ivf files size
        return tool_paste_clip(argc, argv);

    if (TestInputString.compare("dispresizedframes") == 0)                  //Compresses an IVF Raw File to an IVF Compressed file
        return tool_disp_resized_frames(argc, argv);

    if (TestInputString.compare("dispdropedframes") == 0)                   //Compresses an IVF Raw File to an IVF Compressed file
        return tool_disp_droped_frames(argc, argv);

    if (TestInputString.compare("dispvisibleframes") == 0)                  //Compresses an IVF Raw File to an IVF Compressed file
        return tool_disp_visible_frames(argc, argv);

    if (TestInputString.compare("dispaltrefframes") == 0)                   //Compresses an IVF Raw File to an IVF Compressed file
        return tool_disp_alt_ref_frames(argc, argv);

    if (TestInputString.compare("dispkeyframes") == 0)                      //Displays which frames are key frames for an input compressed ivf file
        return tool_disp_key_frames(argc, argv);

    if (TestInputString.compare("dispframedata") == 0)                      //Displays which frames are key frames for an input compressed ivf file
        return tool_disp_frame_data(argc, argv);

    if (TestInputString.compare("formatsummary") == 0)                      //Formats a test results output file by test and result
        return tool_format_summary(argc, argv);

    if (TestInputString.compare("formatted2raw") == 0)                            //Converts an ivf file to a raw file
        return tool_formatted_to_raw(argc, argv);

    if (TestInputString.compare("playcompivf") == 0)                        //Plays a compressed ivf file (Decodes the file to an ivf then converts that ivf to a raw then used tnmplay.exe to play that file.)
        return tool_play_comp_ivf(argc, argv);

    if (TestInputString.compare("playdecivf") == 0)                         // Plays a Decoded ivf file (Converts the file to a Raw file and uses tmnplay.exe to play the file)
        return tool_play_dec_ivf(argc, argv);

    if (TestInputString.compare("vpxencpar") == 0)             //converts Tester Parameter file to a vpxenc ParFile
        return tool_convert_par_file_to_vpxenc(argc, argv);

    ////////////////////Hidden Commands/////////////////////////
    if (TestInputString.compare("showhidden") == 0)                         //Shows Hidden Commands
        return show_hidden_cmds();

    if (TestInputString.compare("codecoverage") == 0)                       //Preforms Code Coverage Tests for data coverage if enabled
        return tool_array_coverage(argc, argv, WorkingDir, EmptyAr);

    if (TestInputString.compare("comparecodecoverage") == 0)                //Compares New Code Coverage Results to Old Code Coverage Results and merges the files to an updated version
        return tool_compare_code_coverage(argc, argv);

    if (TestInputString.compare("formatcodecoveragefile") == 0)             //Formats Cygwin Code Coverage files to the same format as regular linux
        return tool_format_code_coverage_file(argc, argv);

    if (TestInputString.compare("ivfdectestvectorcheck") == 0)              //
        return tool_ivf_dec_test_vector_check(argc, argv);

    if (TestInputString.compare("testvectorindex") == 0)                    //Will search a data base and return the Test Vector that covers an input File and Line Number
        return tool_test_vector_index(argc, argv);

    if (TestInputString.compare("compmatchesivfenc") == 0)                  //Makes sure that the Tester compressor matches ivfenc compressor
        return tool_comp_matches_ivfenc(argc, argv);

    if (TestInputString.compare("compressionequiv") == 0)                   //makes sure that all variations of the compressor produce the same output
        return tool_compression_equiv(argc, argv, WorkingDir);

    if (TestInputString.compare("printversion") == 0)                       //Prints the internal version number - make sure all libs built in same multithreadedmode - properties  C++ CodeGen RuntimeLibrary
        return print_version();

    if (TestInputString.compare("test0inputcheck") == 0)                    //Checks to make sure a Multi Run Input file has the correct format
        return vpxt_run_multiple_tests_input_check(argv[2], 1);

    if (TestInputString.compare("convertparfiletoivfenc") == 0)             //converts Tester Parameter file to a IVFenc ParFile - obsolete
        return tool_convert_par_file_to_ivfenc(argc, argv);

    if (TestInputString.compare("copyalltxtfiles") == 0)                    //Copies all text files in a directory to a new directory preserving file structure
        return tool_copy_all_txt_files(argc, argv);

    if (TestInputString.compare("vpxtdec2raw") == 0)                         //Decodes IVF File to Raw File
        return tool_vpxt_dec_to_raw(argc, argv);

    if (TestInputString.compare("deleteallivffiles") == 0)                  //Deletes all IVF files in a directory be very care with this function!
        return tool_delete_all_ivf_files(argc, argv);

    if (TestInputString.compare("raw2formatted") == 0)                            //Converts an IVF File to a Raw File
        return tool_raw_to_formatted(argc, argv);

    if (TestInputString.compare("sidebysidetext") == 0)                     //Prints out the input from two text files side by side as coloums to an output txt file
        return tool_side_by_side_text(argc, argv);

    if (TestInputString.compare("formatted2rawframes") == 0)                      //Converts an ivf file to a raw file
        return tool_formatted_to_raw_frames(argc, argv);

    if (TestInputString.compare("arraycovfaillisttofull") == 0)             //
        return tool_array_cov_fail_list_to_full_list(argc, argv);

    if (TestInputString.compare("arraycovsummaryfile") == 0)                //
        return tool_array_cov_summary_file(argc, argv);

    if (TestInputString.compare("winmemmonformat") == 0)                    //
        return tool_win_mem_mon_format(argc, argv);

    if (TestInputString.compare("winmemmongraph") == 0)                     //
        return tool_win_mem_mon_graph(argc, argv);

    if (TestInputString.compare("combineindvframes") == 0)                  //
        return tool_combine_indv_frames(argc, argv);

    if (TestInputString.compare("graphpsnr") == 0)                          //
        return tool_graph_psnr(argc, argv, WorkingDir, EmptyAr, 1);

    if (TestInputString.compare("randcompress") == 0)                        //
        return tool_rand_comp(argc, argv, WorkingDir, EmptyAr, 1);

    if (TestInputString.compare("randparfile") == 0)                        //
        return tool_create_rand_par_file(argc, argv);

    if (TestInputString.compare("vpxdec") == 0)                             //
        return tool_run_ivfdec(argc, argv);

    if (TestInputString.compare("vpxenc") == 0)                             //
        return tool_run_ivfenc(argc, argv);

    if (TestInputString.compare("printcpuinfo") == 0)                       //
        return tool_print_cpu_info();

    if (TestInputString.compare("randommultitest") == 0)
        return tool_random_multi_test(argc, argv);

    if (TestInputString.compare("getframecount") == 0)
        return vpxt_get_number_of_frames(argv[2]);

    if (TestInputString.compare("writeivffileheader") == 0)
        return tool_vpxt_write_ivf_file_header(argc, argv);

    if (TestInputString.compare("writeivfframeheader") == 0)
        return tool_vpxt_write_ivf_frame_header(argc, argv);

    if (TestInputString.compare("writeframedata") == 0)
        return tool_vpxt_write_frame_data(argc, argv);

    tprintf(PRINT_STD, "\n");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int selector = vpxt_identify_test(argv[1]);

    if (selector > MAXTENUM || selector < 0)
    {
        vpxt_on_error_output();
        return 0;
    }

    if (selector == 0)
        return vpxt_run_multi(argc, argv, WorkingDir);

    if (selector == AlWDFNUM)
        return test_allow_drop_frames(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == ALWLGNUM)
        return test_allow_lag(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == ALWSRNUM)
        return test_allow_spatial_resampling(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == ARNRTNUM)
        return test_arnr(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == AUTKFNUM)
        return test_auto_key_frame(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == BUFLVNUM)
        return test_buffer_level(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == CPUDENUM)
        return test_change_cpu_dec(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == CPUENNUM)
        return test_change_cpu_enc(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == CONQUNUM)
        return test_constrained_quality(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == DFWMWNUM)
        return test_drop_frame_watermark(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == DTARTNUM)
        return test_data_rate(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == DBMRLNUM)
        return test_debug_matches_release(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == ENCBONUM)
        return test_encoder_break_out(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == ERRMWNUM)
        return test_error_resolution(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == EXTFINUM)
        return test_extra_file(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == FIXDQNUM)
        return test_fixed_quantizer(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == FKEFRNUM)
        return test_force_key_frame(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == FRSZTNUM)
        return test_frame_size(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == GQVBQNUM)
        return test_good_vs_best(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == LGIFRNUM)
        return test_lag_in_frames(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == MAXQUNUM)
        return test_max_quantizer(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == MEML1NUM)
        return test_mem_leak(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == MEML2NUM)
        return test_mem_leak2(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == MINQUNUM)
        return test_min_quantizer(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == MULTDNUM)
        return test_multithreaded_dec(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == MULTENUM)
        return test_multithreaded_enc(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == NVOPSNUM)
        return test_new_vs_old_psnr(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == NVOECPTK)
        return test_new_vs_old_enc_cpu_tick(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == NOISENUM)
        return test_noise_sensitivity(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == OV2PSNUM)
        return test_one_pass_vs_two_pass(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == PLYALNUM)
        return test_play_alternate(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == POSTPNUM)
        return test_post_processor(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == RECBFNUM)
        return test_reconstruct_buffer(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == RSDWMNUM)
        return test_resample_down_watermark(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == SPEEDNUM)
        return test_speed(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == TVECTNUM)
        return test_test_vector(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == TTVSFNUM)
        return test_thirtytwo_vs_sixtyfour(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == TV2BTNUM)
        return test_two_pass_vs_two_pass_best(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == UNDSHNUM)
        return test_undershoot(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == VERSINUM)
        return test_version(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == VPXMINUM)
        return test_vpx_matches_int(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    if (selector == WMLMMNUM)
        return test_win_lin_mac_match(argc, argv, WorkingDir, EmptyAr, 1, KEEP_IVF);

    vpxt_on_error_output();
    return 0;
}
