#define _CRT_SECURE_NO_WARNINGS
//------------------------------------------------------------------------------
//
//  Copyright (c) 1999-2005  On2 Technologies Inc.  All Rights Reserved.
//
//------------------------------------------------------------------------------
/////VP8//////////////////
#include "on2-vpx-shim.h"
#include "onyx.h"
#include "onyxd.h"
#include <map>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
//////////////////////////
#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
/////PSNR/////
#include "yv12config.h"
#include <string>
#include <iostream>
#include <fstream>
#include "vpx_mem.h"
#include "ivf.h"
using namespace std;
////////CompIVF////////////
#include "CompIVF.h"
//////////////////////////

//windows only testing//
#if defined(_WIN32)
#define snprintf _snprintf
#include "on2vpplugin.h"
#endif
///////////////////////
#include <cstdio>

#if defined(_WIN32)
#include <windows.h>
#include <tchar.h>
#include <direct.h>
#elif defined(linux)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#elif defined(__APPLE__)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#elif defined(__POWERPC__)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#endif

////////////////////////Global Slash Character Definion for multiplat////////////////////////
extern char slashChar;
extern string slashCharStr;
/////////////////////////////////////////////////////////////////////////////////////////////
extern char *itoa_custom(int value, char *result, int base);

//driver
extern string DateString();

//Utilities
extern int absInt(int input);
extern float absFloat(float input);
extern double absDouble(double input);
extern VP8_CONFIG VP8RandomParms(VP8_CONFIG &opt, char *inputfile, int display);
extern VP8_CONFIG InPutSettings(char *inputFile);
extern int OutPutSettings(char *outputFile, VP8_CONFIG opt);
extern int OutPutCompatSettings(char *outputFile, VP8_CONFIG opt, int ParVersionNum);
extern long FileSize(char *inFile);
extern long FileSize2(char *inFile);
extern void FolderName2(char *DirIn, char *DirOut);
extern void FolderName(char *input, char *output);
extern void FileName(char *input, char *FileName);
extern string ExtractDateTime(string InputStr);
extern void TestName(char *input, char *TestName);
extern int TimeStampCompare(string TimeStampNow, string TimeStampPrevious);
extern void SubFolderName(char *input, char *FileName);
extern int Test0InputTextCheck(char *input, int MoreInfo);
extern int FileExistsCheck(string input);
extern unsigned int GetTime();
extern int MakeDirVPX(string CreateDir2);
extern void RunExe(string RunExe);
extern int CompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt, int RunQCheck);
extern unsigned int TimeCompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt, int RunQCheck);
extern int DecompressIVFtoIVF(char *inputFile, char *outputFile2);
extern unsigned int TimeDecompressIVFtoIVF(char *inputFile, char *outputFile2);
extern unsigned int DecompressIVFtoIVFTimeAndOutput(char *inputFile, char *outputFile2);
//extern int KeyFrameCompressIVFtoIVF(char *inputFile, char *outputFile, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt, char *outputfile);
//extern int KeyFrameInOutCompressIVFtoIVF(char *inputFile, char *outputFile, int speed, int BitRate, VP8_CONFIG opt, char *CompressString, int CompressInt, char *inputfile, char *outputfile, int InOutControl);
extern double IVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, double &SsimOut);
extern double PostProcIVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double &SsimOut);
extern int PSNRSelect(char *inFile, char *outFile);
extern double IVFDataRate(char *inputFile, int DROuputSel);
extern int IVFCheckPBM(char *inputFile, int bitRate, int maxBuffer, int preBuffer);
extern int CompIVF(char *inputFile1, char *inputFile2);
extern double IVFDisplayKeyFrames(char *inputFile, int Selector);
extern int TimeReturn(char *infile);
extern int DecompressIVFtoRaw(char *inputFile, char *outputFile2);
extern int DecompressIVFtoIVFErrorDetection(char *inputFile, char *outputFile2);
extern void VP8DefaultParms(VP8_CONFIG &opt);
extern int DecComputeMD5(char *inputchar, char *outputchar);
extern int CheckMinQ(char *inputFile, int MinQuantizer);
extern int CheckMaxQ(char *inputFile, int MaxQuantizer);
extern int CheckFixedQ(char *inputFile, int FixedQuantizer);
extern double IVFDisplayAltRefFrames(char *inputFile, int Selector);
extern double IVFDisplayVisibleFrames(char *inputFile, int Selector);

/////Tests
extern int ExternalTestRunner(int argc, char *argv[], string WorkingDir,  int NumberofTests);
extern int AllowDF(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AllowSpatialResamplingTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AllowSpatialResamplingTest2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AutoKeyFramingWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ComboCheckRun(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int DFWM(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int DataRateMaxWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int DataRateTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int DebugMatchesRelease(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ExtraFileCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ForceKeyFrameWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int GoodQvBestQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int InterlaceWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int KeyInKeyOutWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MaxQTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MemLeakCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MemLeakCheck2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MinQTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int NewVsOldPSNR(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int NoiseSensitivityWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int GraphPSNR(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int PostProcessorWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int PreProcessorWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int RandComp(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int SpeedTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int TwoPassVsTwoPassBest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int UnderShoot(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
/////NewVP8Tests
extern int AllowLagTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AltFreqTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int FixedQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int LagInFramesTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MultiThreadedTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int PlayAlternate(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int Version(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AltQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int KeyQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int GoldQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int WindowsMatchesLinux(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int EncoderBreakOut(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int TestVectorCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
//TestsNoLongerUsed
extern int NewVsOldSpeed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int SpeedTest2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ErrorRes(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);


/////TestsNotUsed
extern int BufferLevelWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int CPUDecOnlyWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ChangeCPUDec(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ChangeCPUWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ChangeIDCTDecWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ChangeIDCTWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int IDCTDecOnlyWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int NewVsOldRealTimeSpeed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int OnePassVsTwoPass(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ResampleDownWaterMark(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int VP61vVP62(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int VP6MvVP60(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);

//Tools
extern int WriteIndividualFramesOut(int argc, char *argv[]);
extern "C" on2_codec_iface_t on2_codec_vp8_cx_algo;

extern void FormatedPrint(string SummaryStr, int selector);
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

    extern const char *on2_codec_build_config(void);
}
extern unsigned int ON2_GetProcCoreCount();

///////////////////////////////External Arrays to Test/////////////////////////////

//#define DATA_COV /* If DataCov Definition is commented out Array Code Coverage Tests are not inclued if it is not they are */
#if defined(DATA_COV)

extern "C" unsigned int vp8dx_bitreader_norm[256];
extern "C" unsigned int vp8_mv_cont_count[5][4];
extern "C" int vp8_default_zig_zag1d[16];
extern "C" UINT16 vp8_coef_bands_x[16];
//extern "C" UINT16 vp8_block2context[25*3];
extern "C" int vp8_block2left[25];
extern "C" int vp8_block2above[25];
extern "C" int vp8_block2type[25];
extern "C" int vp8_BLOCK2CONTEXT[25];

#define uchar unsigned char
typedef uchar cuchar;

extern "C" cuchar vp8_coef_bands[16];
extern "C" cuchar vp8_prev_token_class[12];
extern "C" short vp8_default_zig_zag_mask[16];
extern "C" int vp8_mb_feature_data_bits[2];

typedef signed char VP8_TreeIndex;

extern "C" VP8_TreeIndex vp8_coef_tree[22];

typedef unsigned char VP8_Prob;
typedef VP8_Prob Prob;

extern "C" Prob Pcat1[1];
extern "C" Prob Pcat2[2];
extern "C" Prob Pcat3[3];
extern "C" Prob Pcat4[4];
extern "C" Prob Pcat5[5];
extern "C" Prob Pcat6[11];

extern "C" VP8_TreeIndex cat1[2];
extern "C" VP8_TreeIndex cat2[4];
extern "C" VP8_TreeIndex cat3[6];
extern "C" VP8_TreeIndex cat4[8];
extern "C" VP8_TreeIndex cat5[10];
extern "C" VP8_TreeIndex cat6[22];

extern "C" int kf_y_mode_cts[5];
extern "C" int y_mode_cts[5];
extern "C" int uv_mode_cts[4];
extern "C" int kf_uv_mode_cts[4];
extern "C" int bmode_cts[10];

extern "C" VP8_Prob sub_mv_ref_prob [3];
extern "C" VP8_Prob vp8_sub_mv_ref_prob2[5][3];

typedef int VP8_MBsplit[16];

extern "C" VP8_MBsplit vp8_mbsplits [4];
extern "C" int vp8_mbsplit_count[4];
extern "C" VP8_Prob vp8_mbsplit_probs[3];

extern "C" VP8_TreeIndex vp8_bmode_tree[18];
extern "C" VP8_TreeIndex vp8_ymode_tree[8];
extern "C" VP8_TreeIndex vp8_kf_ymode_tree[8];
extern "C" VP8_TreeIndex vp8_uv_mode_tree[6];
extern "C" VP8_TreeIndex vp8_mbsplit_tree[6];
extern "C" VP8_TreeIndex vp8_mv_ref_tree[8];
extern "C" VP8_TreeIndex vp8_sub_mv_ref_tree[6];

extern "C" VP8_TreeIndex vp8_small_mvtree [14];
//extern "C" VP8_TreeIndex vp8_LowMVtree [6];
//extern "C" VP8_TreeIndex vp8_HighMVtree [8];

//extern "C" int vp8_Reverse3bits[8];
//extern "C" int vp8_Reverse4bits[16];

typedef VP8_Prob MV_CONTEXT [19];

extern "C" MV_CONTEXT vp8_mv_update_probs[2];
extern "C" MV_CONTEXT vp8_default_mv_context[2];

extern "C" int bilinear_filters[8][2];
extern "C" short sub_pel_filters[8][6];

//extern "C" short idctMatrix1[4][4];
//extern "C" short idctMatrix2[4][4];

extern "C" int vp8_mode_contexts[6][4];

extern "C" unsigned int vp8_kf_default_bmode_counts [10] [10] [10];

extern "C" short kernel5[5];
extern "C" short vp8_rv[440];

extern "C" int dc_qlookup[128];
extern "C" int ac_qlookup[128];
extern "C" int bbb[4];

//////////////////////////Testing//////////////////////////

extern struct VP8_TokenStruct
{
    int value;
    int Len;
};

extern "C" struct VP8_TokenStruct vp8_CoefEncodings[12];

extern "C" struct VP8_TokenStruct vp8_BmodeEncodings[10];
extern "C" struct VP8_TokenStruct VP8_YmodeEncodings[5];
extern "C" struct VP8_TokenStruct vp8_MBsplitEncodings[4];
extern "C" struct VP8_TokenStruct vp8_SmallMVencodings[8];

extern "C" struct VP8_TokenStruct vp8_MVrefEncodingArray[5];
extern "C" struct VP8_TokenStruct vp8_subMVrefEncodingArray[4];
extern "C" struct VP8_TokenStruct vp8_kfYmodeEncodings[5];
extern "C" struct VP8_TokenStruct vp8_UVmodeEncodings[4];

////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    INT16		  MinVal;
    INT16         Length;
    UINT8 Probs[12];
} TOKENEXTRABITS;

extern "C" TOKENEXTRABITS vp8d_token_extra_bits2[12];

//extern "C" struct
//{
//	int row;
//	int col;
//	int weight;	 /*	 was 5 - (abs(nearB[i].row) + abs(nearB[i].col)) */
//	int block;
//} nearB[3];
/////////////////////////////////////////////////////////////////////////////////
#endif

extern void PrintHeader1(int argc, char *argv[], string WorkingDir3);
extern void PrintHeader2(int argc, char *argv[], string WorkingDir3);
extern void PrintHeader3(int argc, char *argv[], string WorkingDir3);
extern void RecordTestComplete(string MainDirString, string File1String, int TestType);
//----------------------------------------Tests No Longer Used----------------------------------------------------------------
int AltFreqTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    //RemoveME:This test should test alt_freq sort of like key frame frequency but im seeing notes in the
    //         source code that says this setting will not be used in the final build but i am seeing
    //         the setting used in onyx_if.c line 1544 and 1823 to set baseline_gf_interval but it also
    //         seems to use DEFAULT_GF_INTERVAL if no value for alt_freq exists so not sure if test should
    //         be removed or modified currently.

    char *CompressString = "AltFreq";

    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  AltFreqTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Alt Freq 1>\n"
            "    <Alt Freq 2>\n"
            "    <Optional Settings File>\n"
        );

        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "AltFreq";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char File1[255] = "";

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            //strcpy(WorkingDirString2, WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch 1");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;

        FileStream.close();
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("AltFreq1Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("AltFreq2Output.ivf");

    char AltFreq1[255];
    char AltFreq2[255];

    snprintf(AltFreq1, 255, "%s", WorkingDir4.c_str());
    snprintf(AltFreq2, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);;

    int Mode = atoi(argv[3]);

    int AltFreq1Val = atoi(argv[5]);
    int AltFreq2Val = atoi(argv[6]);

    printf("Alt Freq Test");
    fprintf(stderr, "Alt Freq Test");

    //Test is not valid for API Build so if API Build abort test.
#ifdef API
    printf("\nNot a valid VP8 API Test - TestNotSupported\n");
    fprintf(stderr, "\nNot a valid VP8 API Test - TestNotSupported\n");
    fclose(fp);
    string File2Str = File1;
    RecordTestComplete(MainDirString, File2Str, TestType);
    return 12;
#endif

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.TargetBandwidth = BitRate;
    opt.PlayAlternate = 1;
    opt.FixedQ = 1;

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 =Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //here to be added to if needed later.
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.AltFreq = AltFreq1Val;

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AltFreq = AltFreq2Val;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.AltFreq = AltFreq1Val;

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AltFreq = AltFreq2Val;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            opt.AltFreq = AltFreq1Val;

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AltFreq = AltFreq2Val;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.AltFreq = AltFreq1Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltFreq = AltFreq2Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.AltFreq = AltFreq1Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltFreq = AltFreq2Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    int lngRC = CompIVF(AltFreq2, AltFreq1);

    if (lngRC >= 0)
    {
        printf("\n\nPass: Differing Alternate Frequencies have an effect - Files differ at frame: %i\n", lngRC);
        fprintf(stderr, "\n\nPass: Differing  Alternate Frequencies have an effect - Files differ at frame: %i\n", lngRC);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (lngRC == -1)
    {
        printf("\nFail: Differing  Alternate Frequencies have no effect - Files are identical\n");
        fprintf(stderr, "\nFail: Differing  Alternate Frequencies have no effect - Files are identical\n");
    }


    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int ChangeIDCTDecWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " ChangeIDCTDecWorks - Removed as of 11 16 2009 - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;


    char *CompressString = "AllowDF";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  ChangeIDCTDecWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString;
    string Mode3TestMatch;
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "ChangeIDCTDecWorks";
    string MainDirString;
    char File1[255];


    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        //Makes sure log file and test txt file match up
        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMismatch");
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("ChangeIDCTDecOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("ChangeIDCTDecOutputDEC0.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("ChangeIDCTDecOutputDEC1.ivf");

    char ChangeIDCTDecOutFile[255];
    char ChangeIDCTDecOutputDEC0[255];
    char ChangeIDCTDecOutputDEC1[255];

    snprintf(ChangeIDCTDecOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(ChangeIDCTDecOutputDEC0, 255, "%s", WorkingDir5.c_str());
    snprintf(ChangeIDCTDecOutputDEC1, 255, "%s", WorkingDir6.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");


    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDirString);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    printf("Change IDCT Dec Works\n");
    fprintf(stderr, "Change IDCT Dec Works\n");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;
    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////
    int CompressInt = opt.AllowDF;

    unsigned int totalms = 0;
    unsigned int totalms2 = 0;

    int lngRC = 0;

    opt.TargetBandwidth = BitRate;

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        totalms = TimeReturn(ChangeIDCTDecOutputDEC0);
        totalms2 = TimeReturn(ChangeIDCTDecOutputDEC1);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        printf("\n\nIDTC=0\n");
        fprintf(stderr, "\n\nIDTC=0\n");

        totalms = DecompressIVFtoIVFTimeAndOutput(ChangeIDCTDecOutFile, ChangeIDCTDecOutputDEC0);

        printf("\n\nIDTC=1\n");
        fprintf(stderr, "\n\nIDTC=1\n");

        totalms2 = 0;

        totalms2 = DecompressIVFtoIVFTimeAndOutput(ChangeIDCTDecOutFile, ChangeIDCTDecOutputDEC1);

    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    printf("\ncomparing IDCT:0 to IDCT:1");
    fprintf(stderr, "\ncomparing IDCT:0 to IDCT:1");

    lngRC = CompIVF(ChangeIDCTDecOutputDEC0, ChangeIDCTDecOutputDEC1);


    if (lngRC == -1)
    {
        if (totalms != totalms2)
        {
            printf("\n\nFiles are identical and Time 1:%i != Time2:%i - Pass\n\n", totalms, totalms2);
            fprintf(stderr, "n\nFiles are identical and Time 1:%i != Time2:%i - Pass\n\n", totalms, totalms2);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }

        printf("\n\n Failed - Files are identical but times are equal. - Time 1:%i != Time2:%i \n\n", totalms, totalms2);
        fprintf(stderr, "\n\n Failed - Files are identical but times are equal. - Time 1:%i == Time2:%i \n", totalms, totalms2);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;

    }

    printf("\n\n Failed - Files are not Identical. \n\n");
    fprintf(stderr, "\n\n Failed - Files are not Identical. \n");

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int ChangeIDCTWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " ChangeIDCTWorks - Removed as of 11 16 2009 - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;


    char *CompressString = "AllowDF";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  ChangeIDCTDecWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString;
    string Mode3TestMatch;
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "ChangeIDCTWorks";
    string MainDirString;
    char File1[255];


    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            //strcpy(WorkingDirString2, WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        //Makes sure log file and test txt file match up
        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMismatch");
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("ChangeIDCT0Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("ChangeIDCT1Output.ivf");

    char ChangeIDCT0OutFile[255];
    char ChangeIDCT1OutFile[255];

    snprintf(ChangeIDCT0OutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(ChangeIDCT1OutFile, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDirString);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    printf("Change IDCT Works\n");
    fprintf(stderr, "Change IDCT Works\n");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    printf("\n\nIDTC=0");
    fprintf(stderr, "\n\nIDTC=0");

    int speed = 0;
    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////
    int CompressInt = opt.AllowDF;

    opt.TargetBandwidth = BitRate;

    unsigned int Time1;
    unsigned int Time2;

    int lngRC;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        Time1 = TimeReturn(ChangeIDCT1OutFile);
        Time2 = TimeReturn(ChangeIDCT0OutFile);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        printf("\n\nIDTC=1");
        fprintf(stderr, "\n\nIDTC=1");


        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    printf("\ncomparing IDCT:0 to IDCT:1");
    fprintf(stderr, "\ncomparing IDCT:0 to IDCT:1");

    lngRC = CompIVF(ChangeIDCT1OutFile, ChangeIDCT0OutFile);

    if (lngRC == -1)
    {
        if (Time1 != Time2)
        {
            printf("\nFiles are identical and Time 1:%i != Time 2:%i :Pass\n\n", Time1, Time2);
            fprintf(stderr, "\nFiles are identical and Time 1:%i != Time 2:%i :Pass\n\n", Time1, Time2);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            if (lngRC == -2)
            {
                printf("\n\nFail: File 2 ends before File 1.\n"/*, lngRC*/);
                fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n"/*, lngRC*/);
                return 0;
            }

            if (lngRC == -3)
            {
                printf("\n\nFail: File 1 ends before File 2.\n"/*, lngRC*/);
                fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n"/*, lngRC*/);
                return 0;
            }

            printf("\n Failed - Times are the same. Time 1:%i == Time 2:%i\n\n", Time1, Time2);
            fprintf(stderr, "\n Failed - Times are the same. Time 1:%i == Time 2:%i\n\n", Time1, Time2);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

    }
    else
    {
        printf("\n\nFail: Files differ at frame: %i\n\n", lngRC);
        fprintf(stderr, "\n\nFail: Files differ at frame: %i\n\n", lngRC);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);

    return 0;
}
int IDCTDecOnlyWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " IDCTDecOnlyWorks - Removed as of 11 16 2009 - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;


    char *CompressString = "AllowDF";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  IDCTDecOnlyWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString;
    string Mode3TestMatch;
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "IDCTDecOnlyWorks";
    string MainDirString;
    char File1[255];

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);
        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            //strcpy(WorkingDirString2, WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        //Makes sure log file and test txt file match up
        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMismatch");
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }


    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("IDCTDecOnlyWorksOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("IDCTDecOnlyWorksOutputDecodedIDCT0.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("IDCTDecOnlyWorksOutputDecodedIDCT1.ivf");

    char IDCTDecOnlyWorksOutFile[255];
    char IDCTDecOnlyWorksOutputDecodedIDCT0[255];
    char IDCTDecOnlyWorksOutputDecodedIDCT1[255];

    snprintf(IDCTDecOnlyWorksOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(IDCTDecOnlyWorksOutputDecodedIDCT0, 255, "%s", WorkingDir5.c_str());
    snprintf(IDCTDecOnlyWorksOutputDecodedIDCT1, 255, "%s", WorkingDir6.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDirString);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    printf("IDCT Dec Only Works\n");
    fprintf(stderr, "IDCT Dec Only Works\n");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    unsigned int totalms = 0;
    unsigned int totalms2 = 0;

    int speed = 0;
    int Fail = 0;
    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////
    int CompressInt = opt.AllowDF;
    opt.TargetBandwidth = BitRate;
    opt.Mode = MODE_GOODQUALITY;

    int counter = 0;
    int Mode2 = 0;

    int lngRC;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        totalms = TimeReturn(IDCTDecOnlyWorksOutputDecodedIDCT0);
        totalms2 = TimeReturn(IDCTDecOnlyWorksOutputDecodedIDCT1);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        printf("\n\nIDTC=0\n");
        fprintf(stderr, "\n\nIDTC=0\n");

        totalms = DecompressIVFtoIVFTimeAndOutput(IDCTDecOnlyWorksOutFile, IDCTDecOnlyWorksOutputDecodedIDCT0);

        printf("\n\nIDTC=1\n");
        fprintf(stderr, "\n\nIDTC=1\n");

        totalms2 = DecompressIVFtoIVFTimeAndOutput(IDCTDecOnlyWorksOutFile, IDCTDecOnlyWorksOutputDecodedIDCT1);

    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    printf("\ncomparing IDCT:0 to IDCT:1 - ");
    fprintf(stderr, "\ncomparing IDCT:0 to IDCT:1 - ");

    lngRC = CompIVF(IDCTDecOnlyWorksOutputDecodedIDCT0, IDCTDecOnlyWorksOutputDecodedIDCT1);

    if (lngRC >= 0)
    {
        printf("\n\nFail: Files differ at frame: %i\n", lngRC);
        fprintf(stderr, "\n\nFail: Files differ at frame: %i\n", lngRC);
        Fail = 1;
    }

    if (lngRC == -1)
    {
        printf("\nFiles are identical\n");
        fprintf(stderr, "\nFiles are identical\n");
    }

    if (lngRC == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        Fail = 1;
    }

    if (lngRC == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        Fail = 1;
    }

    printf("\n");
    fprintf(stderr, "\n");

    if (Fail == 0)
    {
        if (totalms == totalms2)
        {
            printf("Fail: Files are Identical but IDCT changes are not effecting the runtime Time 1:%i == Time 2:%i\n\n", totalms, totalms2);
            fprintf(stderr, "Fail: Files are Identical but IDCT changes are not effecting the runtime Time 1:%i == Time 2:%i\n\n", totalms, totalms2);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (totalms != totalms2)
        {
            printf("Pass - Files are Identical with differing run times.  Time 1:%i != Time 2:%i\n\n", totalms, totalms2);
            fprintf(stderr, "Pass - Files are Identical with differing run times. Time 1:%i != Time 2:%i\n\n", totalms, totalms2);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }
    else
    {
        printf("\nFail: Files are not Identical.\n");
        fprintf(stderr, "\nFail: Files are not Identical.\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}


int NewVsOldSpeed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " NewVsOldSpeed - Removed as of ## ## #### - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;

    if (argc != 6)
    {
        printf(
            "  NewVsOldPSRN \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Exe File To Compare>\n\n"

        );
        return 0;
    }




    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char *ExeInput = argv[5];

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString;
    string Mode3TestMatch;
    string ExeString;
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "NewVsOldSpeed";
    string MainDirString;
    char File1[255];

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        char ExeChar[1024];
        FolderName(argv[0], ExeChar);
        ExeString = ExeChar;
        ExeString.insert(0, "\"\"");
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[6]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    ////////////////////////////

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = ExeString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("outputVP7New.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("outputVP7Old.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("outputVP7Old.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("ParFile.txt");
    WorkingDir8.append(ExeInput);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(input);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(WorkingDir5);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(WorkingDir7);
    WorkingDir8.append("\"");
    WorkingDir8.append(" 0");
    WorkingDir8.append("\"");

    char outputVP7New[255];
    char outputVP7Old[255];
    char outputVP7Old2[255];
    char ParFile[255];
    char Program[1024];

    snprintf(outputVP7New, 255, "%s", WorkingDir4.c_str());
    snprintf(outputVP7Old, 255, "%s", WorkingDir5.c_str());
    snprintf(outputVP7Old2, 255, "%s", WorkingDir6.c_str());
    snprintf(ParFile, 255, "%s", WorkingDir7.c_str());
    snprintf(Program, 1024, "%s", WorkingDir8.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;

    unsigned int Time1 = 0;
    unsigned int Time2 = 0;
    unsigned int Time3 = 0;

    printf("New Vs Old Speed Test\n");
    fprintf(stderr, "New Vs Old Speed Test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    opt.TargetBandwidth = BitRate;
    opt.AutoKey = 1;


    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        Time1 = TimeReturn(outputVP7New);
        Time2 = TimeReturn(outputVP7Old2);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutSettings(ParFile, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(Program);
            Time2 = Time2 + TimeReturn(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutSettings(ParFile, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");



            RunExe(Program);
            Time2 = Time2 + TimeReturn(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

            fclose(fp);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutSettings(ParFile, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");


            RunExe(Program);
            Time2 = Time2 + TimeReturn(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

            fclose(fp);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {

            opt.Mode = MODE_SECONDPASS;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS;
            OutPutSettings(ParFile, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(Program);
            Time2 = TimeReturn(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

            fclose(fp);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS_BEST;
            OutPutSettings(ParFile, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(Program);
            Time2 = Time2 + TimeReturn(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

            fclose(fp);
        }

        if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
        {
            printf("Cannot open out put file: %s\n", TextfileString.c_str());
            exit(1);
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    if (Time1 > Time2)
    {
        printf("\n\n Old: %i is Faster than New: %i - Failed \n", Time2, Time1);
        fprintf(stderr, "\n\n Old: %i is Faster than New: %i - Failed \n", Time2, Time1);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (Time1 < Time2)
    {
        printf("\n\n New: %i is Faster than Old: %i - Passed \n", Time1, Time2);
        fprintf(stderr, "\n\n New: %i is Faster than Old: %i - Passed \n", Time1, Time2);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    printf("\n\n Files Took the same amount of time: %i \n", Time1);
    fprintf(stderr, "\n\n Files Took the same amount of time: %i \n", Time1);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 2;
}


int SpeedTest2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " SpeedTest2 - Removed as of ## ## #### - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;

    char *CompressString = "Speed";

    if (argc != 5)
    {
        printf(
            "  SpeedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n ");
        return 0;
    }


    if (!(ON2_GetProcCoreCount() > 1))
    {
        printf("\nMulti Core not found: Test Aborted\n");
        return 0;
    }
    else
    {
        printf("\nMulti Core found: %i\n", ON2_GetProcCoreCount());
    }

    char *input = argv[2];

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString;
    string Mode3TestMatch;
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "SpeedTest";
    string MainDirString;
    char File1[255];

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[5]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }


    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("Speed14Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("Speed00Output.ivf");

    char Speed14OutFile[255];
    char Speed00OutFile[255];

    snprintf(Speed14OutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(Speed00OutFile, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);

    unsigned int Time1;
    unsigned int Time2;

    int Mode = atoi(argv[3]);

    if (Mode != 0 && Mode != 1)
    {
        printf(
            "  SpeedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n ");
        fclose(fp);
        return 0;
    }

    printf("Speed Test");
    fprintf(stderr, "Speed Test");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    opt.TargetBandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        Time1 = TimeReturn(Speed14OutFile);
        Time2 = TimeReturn(Speed00OutFile);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.MultiThreaded = 14;
            Time1 = TimeCompressIVFtoIVF(input, Speed14OutFile, speed, BitRate, opt, CompressString, 14, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.MultiThreaded = 0;
            Time2 = TimeCompressIVFtoIVF(input, Speed00OutFile, speed, BitRate, opt, CompressString, 0, 0);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.MultiThreaded = 14;
            Time1 = TimeCompressIVFtoIVF(input, Speed14OutFile, speed, BitRate, opt, CompressString, 14, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.MultiThreaded = 0;
            Time2 = TimeCompressIVFtoIVF(input, Speed00OutFile, speed, BitRate, opt, CompressString, 0, 0);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    if (Time1 < Time2)
    {
        printf("\nTime1: %i < Time2: %i ", Time1, Time2);
        fprintf(stderr, "\nTime1: %i < Time2: %i ", Time1, Time2);
        printf("- Passed\n");
        fprintf(stderr, "- Passed \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (Time1 == Time2)
    {
        printf("\n Unknown: Speed has no effect, try different parameters \n");
        fprintf(stderr, "\n Unknown: Speed has no effect, try different parameters \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

    printf("\nTime1: %i > Time2: %i ", Time1, Time2);
    fprintf(stderr, "\nTime1: %i > Time2: %i ", Time1, Time2);
    printf("\n Failed \n");
    fprintf(stderr, "\n Failed \n");


    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int DataRateMaxWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    //Removed 03 25 2009
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " DataRateMaxWorks - Removed as of 03 25 2009 - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;

    char *CompressString = "AllowDF";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  DataRateMaxWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n");
        return 0;
    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString;
    string Mode3TestMatch;
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "DataRateMaxWorks";
    string MainDirString;
    char File1[255];

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("DataRateMaxWorksOutput.ivf");

    char DataRateMaxWorksOut[255];

    snprintf(DataRateMaxWorksOut, 255, "%s", WorkingDir4.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;

    int BitRate = atoi(argv[4]);
    int StartingBufferLvl = 3;
    int MaximumBufferLevel = 3;

    int constInt = 128;
    int CalcBufferSize = BitRate * MaximumBufferLevel * constInt;
    int CalcPrebuffer = BitRate * StartingBufferLvl * constInt;

    int Mode = atoi(argv[3]);

    printf("Data Rate Max Works Test\n");
    fprintf(stderr, "Data Rate Max Works Test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////

    int CompressInt = opt.AllowDF;

    ////////////////Data Rate Specific Settings////////////////
    opt.BestAllowedQ = 0;
    opt.WorstAllowedQ = 63;
    opt.AllowDF = 1;
    ///////////////////////////////////////////////////////////

    opt.TargetBandwidth = BitRate;
    opt.StartingBufferLevel = StartingBufferLvl;
    opt.MaximumBufferSize = MaximumBufferLevel;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    int PassFail = IVFCheckPBM(DataRateMaxWorksOut, BitRate, CalcBufferSize, CalcPrebuffer);

    if (PassFail == -11)
    {
        printf("\n\nCheckPBM Returned -11 - Passed\n");
        fprintf(stderr, "\n\nCheckPBM Returned -11 - Passed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    printf("\n Failed - Underrun at frame: %i \n", PassFail);
    fprintf(stderr, "\n Failed - Underrun at frame: %i \n", PassFail);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}


int InterlaceWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " InterlaceWorks - Removed as of ## ## #### - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;

    char *CompressString = "Interlaced";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  InterlaceWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n");
        return 0;
    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString;
    string Mode3TestMatch;
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "InterlaceWorks";
    string MainDirString;
    char File1[255];

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("InterlaceOnOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("InterlaceOffOutput.ivf");

    char Interlaceon[255];
    char Interlaceoff[255];

    snprintf(Interlaceon, 255, "%s", WorkingDir4.c_str());
    snprintf(Interlaceoff, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);


    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");



    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);

    printf("Interlace Works Test\n");
    fprintf(stderr, "Interlace Works Test\n");

    printf("\nInterlace Works Test Not Relevent for VP8.\n");
    fprintf(stderr, "\nInterlace Works Test Not Relevent for VP8.\n");

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}


int KeyInKeyOutWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " KeyInKeyOutWorks - Removed as of ## ## #### - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;
}
int ComboCheckRun(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    // This will Input Test Settings from a properly formatted text file and run Compresions of the file
    char *CompressString = "opt.KeyFreq";

    if (argc != 4)
    {
        printf(
            "  ComboCheckRun \n\n"
            "    <Settings Inputfile>\n"
            "    <Primary Avi Inputfile>\n\n"
        );
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "ComboCheckRun";
    string MainDirString = "";
    char File1[255] = "";

    snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

    int v = 0;

    while (WorkingDir2[v] != '\"')
    {
        WorkingDir3[v] = WorkingDir2[v];
        v++;
    }

    WorkingDir3[v] = slashChar;
    WorkingDir3[v+1] = '\0';
    string WorkingDirString = WorkingDir3;
    WorkingDirString.append(MyDir);
    WorkingDirString.append(slashCharStr);
    WorkingDirString.append(FilesAr[0]);
    WorkingDirString.erase(WorkingDirString.length() - 1, 1);

    string CreateDir2 = WorkingDirString;
    CreateDir2.insert(0, "md \"");
    MakeDirVPX(CreateDir2.c_str());


    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = WorkingDirString;

    string WorkingDir9 = WorkingDirString;
    string WorkingDir10 = WorkingDirString;

    printf("\n\n %s \n\n", WorkingDir3);

    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("ComboRun.txt");
    char outputfile[255];
    snprintf(outputfile, 255, "%s", WorkingDir6.c_str());

    /////////////OutPutfile////////////
    ///Log output File/////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);
    ///Summary output File/////
    string TextfileString2 = TextfileString;
    TextfileString.append("Raw.txt");
    TextfileString2.append("Summary.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    char *input = argv[3];

    char *input2;
    char *inputfile = argv[2];

    printf("Combo Check Run Test\n");
    fprintf(stderr, "Combo Check Run Test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    //creates

    int Testnum = 1;
    //get the # of tests to run
    ifstream infile(inputfile);

    if (!infile.good())
    {
        printf("\nCannot Find Input File\n");
        fprintf(stderr, "\nCannot Find Input File\n");
        fclose(fp);
        infile.close();
        return 3;
    }

    int TxtLines = 1;

    while (!infile.eof())
    {
        char input3[256];
        infile.getline(input3, 256);
        string input4 = input3;

        if (strncmp(input4.c_str(), "Test", 4) == 0)
        {
            Testnum++;
        }

        TxtLines++;
    }

    infile.close();

    double PSNRAr[256];
    double DataRateAr[256];
    long BitRateAr[256];
    int PBMAr[256];
    int testworked[256];

    ifstream infile2(inputfile);

    int TestCounter = 0;
    int x = 0;
    int State = 0;

    char buffer[256];
    string buffer2;
    string StorageBuffer;
    string StorageBuffer2;
    int BitRate = 0;
    int comment = 0;
    int InputToggle = 0;
    char *TestNameAr[256];
    char *InputFileAr[256];
    vector<string> InputVec;


    int y = 0;

    while (y != TxtLines)
    {

        //State = 0 Initializing searching for first "Test" when first "Test" found state set to 1
        //State = 1 Inputting Settings for current Compresion when New "Test" found state set to 2
        //State = 2 New "Test" Declaration found running compresion and tests then setting state to 1
        y++;
        infile2.getline(buffer, 256);
        buffer2 = buffer;

        if (State == 0)
        {
            if (strncmp(buffer2.c_str(), "Test", 4) == 0)
            {
                State = 1;
                //save Test Name
                StorageBuffer = buffer;
                infile2.getline(buffer, 256);
                buffer2 = buffer;
            }

        }

        if (State == 1)
        {

            if (buffer[0] == '%')
            {
                comment = 1;
            }
            else
            {
                comment = 0;
            }

            if (comment != 1)
            {
                if (buffer2.find("File=") != string::npos)
                {
                    char InputCharArr[256];

                    if (strcmp(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str(), "") != 0)
                    {
                        snprintf(InputCharArr, 255, "%s", buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                    }
                    else
                    {
                        snprintf(InputCharArr, 255, "%s", "No Input File entered");
                    }

                    input2 = InputCharArr;
                    InputToggle = 1;
                }

                if (buffer2.find("AutoKey=") != string::npos)
                {
                    opt.AutoKey = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("KeyFreq=") != string::npos)
                {
                    opt.KeyFreq = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("NoiseSensitivity=") != string::npos)
                {
                    opt.NoiseSensitivity = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("Sharpness=") != string::npos)
                {
                    opt.Sharpness = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("Mode=") != string::npos)
                {
                    int Mode = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());

                    if (Mode == 0)
                    {
                        opt.Mode = MODE_REALTIME;
                    }

                    if (Mode == 1)
                    {
                        opt.Mode = MODE_GOODQUALITY;
                    }

                    if (Mode == 2)
                    {
                        opt.Mode = MODE_BESTQUALITY;
                    }

                    if (Mode == 3)
                    {
                        opt.Mode = MODE_FIRSTPASS;
                    }

                    if (Mode == 4)
                    {
                        opt.Mode = MODE_SECONDPASS;
                    }

                    if (Mode == 5)
                    {
                        opt.Mode = MODE_SECONDPASS_BEST;
                    }
                }

                if (buffer2.find("MultiThreaded=") != string::npos)
                {
                    opt.MultiThreaded = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("TargetBandwidth=") != string::npos)
                {
                    opt.TargetBandwidth = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("UnderShootPct=") != string::npos)
                {
                    opt.UnderShootPct = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("FixedQ=") != string::npos)
                {
                    opt.FixedQ = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("BestAllowedQ=") != string::npos)
                {
                    opt.BestAllowedQ = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("WorstAllowedQ=") != string::npos)
                {
                    opt.WorstAllowedQ = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("StartingBufferLevel=") != string::npos)
                {
                    opt.StartingBufferLevel = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("OptimalBufferLevel=") != string::npos)
                {
                    opt.OptimalBufferLevel = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("MaximumBufferSize=") != string::npos)
                {
                    opt.MaximumBufferSize = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("AllowDF=") != string::npos)
                {
                    opt.AllowDF = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("DropFramesWaterMark=") != string::npos)
                {
                    opt.DropFramesWaterMark = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("AllowSpatialResampling=") != string::npos)
                {
                    opt.AllowSpatialResampling = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("ResampleDownWaterMark=") != string::npos)
                {
                    opt.ResampleDownWaterMark = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("ResampleUpWaterMark=") != string::npos)
                {
                    opt.ResampleUpWaterMark = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("TwoPassVBRBias=") != string::npos)
                {
                    opt.TwoPassVBRBias = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("TwoPassVBRMinSection=") != string::npos)
                {
                    opt.TwoPassVBRMinSection = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("TwoPassVBRMaxSection=") != string::npos)
                {
                    opt.TwoPassVBRMaxSection = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("SettingsFile=") != string::npos)
                {

                }

                if (buffer2.find("RootDirectory=") != string::npos)
                {

                }

                if (buffer2.find("EncodeBreakout=") != string::npos)
                {
                    opt.EncodeBreakout = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("CpuUsed=") != string::npos)
                {
                    opt.CpuUsed = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("EndUsage=") != string::npos)
                {
                    opt.EndUsage = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("Version=") != string::npos)
                {
                    opt.Version = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("FrameRate=") != string::npos)
                {
                    opt.FrameRate = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("AllowLag=") != string::npos)
                {
                    opt.AllowLag = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("LagInFrames=") != string::npos)
                {
                    opt.LagInFrames = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("PlayAlternate=") != string::npos)
                {
                    opt.PlayAlternate = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("AltFreq=") != string::npos)
                {
                    opt.AltFreq = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("AltQ=") != string::npos)
                {
                    opt.AltQ = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("KeyQ=") != string::npos)
                {
                    opt.KeyQ = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (buffer2.find("GoldQ=") != string::npos)
                {
                    opt.GoldQ = atoi(buffer2.substr(buffer2.rfind("=") + 1, buffer2.length()).c_str());
                }

                if (strncmp(buffer2.c_str(), "Test", 4) == 0)
                {
                    StorageBuffer2 = StorageBuffer;
                    StorageBuffer = buffer;
                    State = 2;
                }
            }

        }

        if (State == 2)
        {
            char *CompressString = "ComboRunTest";

            TestCounter++;
            char TestCountChar[32];
            itoa_custom(TestCounter, TestCountChar, 10);

            string WorkingDir9 = WorkingDir7;
            string WorkingDir10 = WorkingDir8;

            WorkingDir9.append(slashCharStr);
            WorkingDir9.append("ComboRun_");
            WorkingDir9.append(StorageBuffer2);
            WorkingDir9.append(".ivf");
            char ComboRunAVI[255];
            snprintf(ComboRunAVI, 255, "%s", WorkingDir9.c_str());

            WorkingDir10.append(slashCharStr);
            WorkingDir10.append("ComboRun_Par_");
            WorkingDir10.append(StorageBuffer2);
            WorkingDir10.append(".txt");
            char ComboRunPar[255];
            snprintf(ComboRunPar, 255, "%s", WorkingDir10.c_str());

            BitRate = opt.TargetBandwidth;

            if (InputToggle == 0)
            {
                if (FileExistsCheck(input))
                {
                    printf("\n================================================================================\n");
                    fprintf(stderr, "\n================================================================================\n");
                    printf("Test %i - Input File Not Found Test Aborted\n", TestCounter);
                    fprintf(stderr, "Test %i - Input File Not Found Test Aborted\n", TestCounter);
                    testworked[TestCounter-1] = 0;
                    InputToggle = 2;
                }
            }

            if (InputToggle == 1)
            {
                if (FileExistsCheck(input2))
                {
                    printf("\n================================================================================\n");
                    fprintf(stderr, "\n================================================================================\n");
                    printf("Test %i - Input File Not Found Test Aborted\n", TestCounter);
                    fprintf(stderr, "Test %i - Input File Not Found Test Aborted\n", TestCounter);
                    testworked[TestCounter-1] = 0;
                    InputToggle = 3;
                }
            }

            if (InputToggle == 0)
            {
                InputVec.push_back(input);
                testworked[TestCounter-1] = 1;

                printf("\n================================================================================\n");
                fprintf(stderr, "\n================================================================================\n");
                printf("Test %i - Input File: %s\n", TestCounter, input);
                fprintf(stderr, "Test %i - Input File: %s\n", TestCounter, input);

                if (CompressIVFtoIVF(input, ComboRunAVI, opt.MultiThreaded, BitRate, opt, CompressString, TestCounter, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                OutPutSettings(ComboRunPar, opt);

                //PSNR Run
                printf("\n\n--------------------------------------PSNR--------------------------------------\n");
                fprintf(stderr, "\n\n--------------------------------------PSNR--------------------------------------\n");
                int PSNRVar = PSNRSelect(input, ComboRunAVI);
                double ssim = 0;
                PSNRAr[TestCounter-1] = IVFPSNR(input, ComboRunAVI, PSNRVar, 2, 1, ssim);

                // DataRate Run
                printf("\n------------------------------------DataRate------------------------------------\n");
                fprintf(stderr, "\n------------------------------------DataRate------------------------------------\n");
                DataRateAr[TestCounter-1] = IVFDataRate(ComboRunAVI, 1);

                //BitRate Run
                printf("\n-------------------------------------BitRate------------------------------------");
                fprintf(stderr, "\n-------------------------------------BitRate------------------------------------");
                printf("\n");
                fprintf(stderr, "\n");
                BitRateAr[TestCounter-1] = FileSize(ComboRunAVI);
                printf("\n");
                fprintf(stderr, "\n");
                printf("\nFile Size: %i \n", BitRateAr[TestCounter-1]);
                fprintf(stderr, "\nFile Size: %i \n", BitRateAr[TestCounter-1]);

                //PBM Run
                printf("\n---------------------------------------PBM--------------------------------------\n");
                fprintf(stderr, "\n---------------------------------------PBM--------------------------------------\n");
                int constInt = 128;
                int CalcBufferSize = opt.TargetBandwidth * opt.MaximumBufferSize * constInt;
                int CalcPrebuffer = opt.TargetBandwidth * opt.StartingBufferLevel * constInt;

                PBMAr[TestCounter-1] = IVFCheckPBM(ComboRunAVI, opt.TargetBandwidth, CalcBufferSize, CalcPrebuffer);

                if (PBMAr[TestCounter-1] == -11)
                {
                    cout << "PBM Passed";
                    cerr << "PBM Passed";
                }

                if (PBMAr[TestCounter-1] != -11)
                {
                    cout << "PBM Failed";
                    cerr << "PBM Failed";
                }

                cerr << "\n";
            }

            if (InputToggle == 1)
            {
                char storage[256];
                int u = 0;

                while (input2[u] != '\0' && input2[u] != '\r')
                {
                    storage[u] = input2[u];
                    u++;
                }

                storage[u] = '\0';

                InputVec.push_back(storage);
                testworked[TestCounter-1] = 1;

                printf("\n================================================================================\n");
                fprintf(stderr, "\n================================================================================\n");
                printf("Test %i - Input File: %s\n", TestCounter, input2);
                fprintf(stderr, "Test %i - Input File: %s\n", TestCounter, input2);

                if (CompressIVFtoIVF(input2, ComboRunAVI, opt.MultiThreaded, BitRate, opt, CompressString, TestCounter, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                OutPutSettings(ComboRunPar, opt);

                //PSNR Run
                printf("\n\n--------------------------------------PSNR--------------------------------------\n");
                fprintf(stderr, "\n\n--------------------------------------PSNR--------------------------------------\n");
                int PSNRVar = PSNRSelect(input2, ComboRunAVI);
                double ssim = 0;
                PSNRAr[TestCounter-1] = IVFPSNR(input2, ComboRunAVI, PSNRVar, 2, 1, ssim);


                // DataRate Run
                printf("\n------------------------------------DataRate------------------------------------\n");
                fprintf(stderr, "\n------------------------------------DataRate------------------------------------\n");
                DataRateAr[TestCounter-1] = IVFDataRate(ComboRunAVI, 1);

                //BitRate Run
                printf("\n-------------------------------------BitRate------------------------------------");
                fprintf(stderr, "\n-------------------------------------BitRate------------------------------------");
                printf("\n");
                fprintf(stderr, "\n");
                BitRateAr[TestCounter-1] = FileSize(ComboRunAVI);
                printf("\n");
                fprintf(stderr, "\n");
                printf("\nFile Size: %i \n", BitRateAr[TestCounter-1]);
                fprintf(stderr, "\nFile Size: %i \n", BitRateAr[TestCounter-1]);

                //PBM Run
                printf("\n---------------------------------------PBM--------------------------------------\n");
                fprintf(stderr, "\n---------------------------------------PBM--------------------------------------\n");
                int constInt = 128;
                int CalcBufferSize = opt.TargetBandwidth * opt.MaximumBufferSize * constInt;
                int CalcPrebuffer = opt.TargetBandwidth * opt.StartingBufferLevel * constInt;

                PBMAr[TestCounter-1] = IVFCheckPBM(ComboRunAVI, opt.TargetBandwidth, CalcBufferSize, CalcPrebuffer);

                if (PBMAr[TestCounter-1] == -11)
                {
                    cout << "PBM Passed";
                    cerr << "PBM Passed";
                }

                if (PBMAr[TestCounter-1] != -11)
                {
                    cout << "PBM Failed";
                    cerr << "PBM Failed";
                }

                cerr << "\n";
            }

            if (InputToggle == 2)
            {
                char storage[256];
                int u = 0;

                while (input[u] != '\0' && input[u] != '\r')
                {
                    storage[u] = input[u];
                    u++;
                }

                storage[u] = '\0';
                InputVec.push_back(storage);
            }

            if (InputToggle == 3)
            {
                char storage[256];
                int u = 0;

                while (input2[u] != '\0' && input2[u] != '\0')
                {
                    storage[u] = input2[u];
                    u++;
                }

                storage[u] = '\0';
                InputVec.push_back(storage);
            }

            InputToggle = 0;

            cout << "\n";
            State = 1;
            VP8DefaultParms(opt);
        }
    }


    //run last test
    StorageBuffer2 = StorageBuffer;
    char  NamChar[255];
    snprintf(NamChar, 255, "%s", StorageBuffer2.c_str());
    TestNameAr[TestCounter] = NamChar;
    char *CompressString2 = "ComboRunTest";

    TestCounter++;
    char TestCountChar2[32];
    itoa_custom(TestCounter, TestCountChar2, 10);

    string WorkingDir11 = WorkingDir9;
    string WorkingDir12 = WorkingDir10;

    WorkingDir11.append(slashCharStr);
    WorkingDir11.append("ComboRun_");
    WorkingDir11.append(StorageBuffer2);
    WorkingDir11.append(".ivf");
    char ComboRunAVI2[255];
    snprintf(ComboRunAVI2, 255, "%s", WorkingDir11.c_str());

    WorkingDir12.append(slashCharStr);
    WorkingDir12.append("ComboRun_Par_");
    WorkingDir12.append(StorageBuffer2);
    WorkingDir12.append(".txt");
    char ComboRunPar2[255];
    snprintf(ComboRunPar2, 255, "%s", WorkingDir12.c_str());

    BitRate = opt.TargetBandwidth;

    //checks to make sure file exists if InputToggle = 0 it checks Defualt input (input)
    //if InputToggle = 1 It checks custom input (input2) if it doesnt find the file it aborts the test and moves on
    if (InputToggle == 0)
    {
        if (FileExistsCheck(input))
        {
            printf("\n================================================================================\n");
            fprintf(stderr, "\n================================================================================\n");
            printf("Test %i - Input File Not Found Test Aborted\n", TestCounter);
            fprintf(stderr, "Test %i - Input File Not Found Test Aborted\n", TestCounter);
            testworked[TestCounter-1] = 0;
            InputToggle = 2;
        }
    }

    if (InputToggle == 1)
    {
        if (FileExistsCheck(input2))
        {
            printf("\n================================================================================\n");
            fprintf(stderr, "\n================================================================================\n");
            printf("\nTest %i - Input File Not Found Test Aborted\n", TestCounter);
            fprintf(stderr, "\nTest %i - Input File Not Found Test Aborted\n", TestCounter);
            testworked[TestCounter-1] = 0;
            InputToggle = 3;
        }
    }

    //Runs the compresion and tests on the Default or Custom input once its been verified the file exists
    if (InputToggle == 0)
    {
        InputVec.push_back(input);
        testworked[TestCounter-1] = 1;
        printf("\n================================================================================\n");
        fprintf(stderr, "\n================================================================================\n");
        printf("Test %i - Input File: %s\n", TestCounter, input);
        fprintf(stderr, "Test %i - Input File: %s\n", TestCounter, input);

        if (CompressIVFtoIVF(input, ComboRunAVI2, opt.MultiThreaded, BitRate, opt, CompressString2, TestCounter, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        OutPutSettings(ComboRunPar2, opt);

        //PSNR Run
        printf("\n\n--------------------------------------PSNR--------------------------------------\n");
        fprintf(stderr, "\n\n--------------------------------------PSNR--------------------------------------\n");
        int PSNRVar = PSNRSelect(input, ComboRunAVI2);
        double ssim = 0;
        PSNRAr[TestCounter-1] = IVFPSNR(input, ComboRunAVI2, PSNRVar, 2, 1, ssim);

        // DataRate Run
        printf("\n------------------------------------DataRate------------------------------------\n");
        fprintf(stderr, "\n------------------------------------DataRate------------------------------------\n");
        DataRateAr[TestCounter-1] = IVFDataRate(ComboRunAVI2, 1);

        //BitRate Run
        printf("\n-------------------------------------BitRate------------------------------------");
        fprintf(stderr, "\n-------------------------------------BitRate------------------------------------");
        printf("\n");
        fprintf(stderr, "\n");
        BitRateAr[TestCounter-1] = FileSize(ComboRunAVI2);
        printf("\n");
        fprintf(stderr, "\n");
        printf("\nFile Size: %i \n", BitRateAr[TestCounter-1]);
        fprintf(stderr, "\nFile Size: %i \n", BitRateAr[TestCounter-1]);

        //PBM Run
        printf("\n---------------------------------------PBM--------------------------------------\n");
        fprintf(stderr, "\n---------------------------------------PBM--------------------------------------\n");
        int constInt = 128;
        int CalcBufferSize = opt.TargetBandwidth * opt.MaximumBufferSize * constInt;
        int CalcPrebuffer = opt.TargetBandwidth * opt.StartingBufferLevel * constInt;

        PBMAr[TestCounter-1] = IVFCheckPBM(ComboRunAVI2, opt.TargetBandwidth, CalcBufferSize, CalcPrebuffer);

        if (PBMAr[TestCounter-1] == -11)
        {
            cout << "PBM Passed";
            cerr << "PBM Passed";
        }

        if (PBMAr[TestCounter-1] != -11)
        {
            cout << "PBM Failed";
            cerr << "PBM Failed";
        }

        cerr << "\n";

    }

    if (InputToggle == 1)
    {
        InputFileAr[TestCounter-1] = input2;
        testworked[TestCounter-1] = 1;

        printf("\n================================================================================\n");
        fprintf(stderr, "\n================================================================================\n");
        printf("Test %i - Input File: %s\n", TestCounter, input2);
        fprintf(stderr, "Test %i - Input File: %s\n", TestCounter, input2);

        if (CompressIVFtoIVF(input2, ComboRunAVI2, opt.MultiThreaded, BitRate, opt, CompressString2, TestCounter, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        OutPutSettings(ComboRunPar2, opt);

        char storage[256];
        int u = 0;

        while (input2[u] != '\0' && input2[u] != '\r')
        {
            storage[u] = input2[u];
            u++;
        }

        storage[u] = '\0';

        InputVec.push_back(storage);

        //PSNR Run
        printf("\n\n--------------------------------------PSNR--------------------------------------\n");
        fprintf(stderr, "\n\n--------------------------------------PSNR--------------------------------------\n");
        int PSNRVar = PSNRSelect(input2, ComboRunAVI2);
        double ssim = 0;
        PSNRAr[TestCounter-1] = IVFPSNR(input2, ComboRunAVI2, PSNRVar, 2, 1, ssim);

        // DataRate Run
        printf("\n------------------------------------DataRate------------------------------------\n");
        fprintf(stderr, "\n------------------------------------DataRate------------------------------------\n");
        DataRateAr[TestCounter-1] = IVFDataRate(ComboRunAVI2, 1);

        //BitRate Run
        printf("\n-------------------------------------BitRate------------------------------------");
        fprintf(stderr, "\n-------------------------------------BitRate------------------------------------");
        printf("\n");
        fprintf(stderr, "\n");
        BitRateAr[TestCounter-1] = FileSize(ComboRunAVI2);
        printf("\n");
        fprintf(stderr, "\n");
        printf("\nFile Size: %i \n", BitRateAr[TestCounter-1]);
        fprintf(stderr, "\nFile Size: %i \n", BitRateAr[TestCounter-1]);

        // PBM Run
        printf("\n---------------------------------------PBM--------------------------------------\n");
        fprintf(stderr, "\n---------------------------------------PBM--------------------------------------\n");
        int constInt = 128;
        int CalcBufferSize = opt.TargetBandwidth * opt.MaximumBufferSize * constInt;
        int CalcPrebuffer = opt.TargetBandwidth * opt.StartingBufferLevel * constInt;

        PBMAr[TestCounter-1] = IVFCheckPBM(ComboRunAVI2, opt.TargetBandwidth, CalcBufferSize, CalcPrebuffer);

        if (PBMAr[TestCounter-1] == -11)
        {
            cout << "PBM Passed";
            cerr << "PBM Passed";
        }

        if (PBMAr[TestCounter-1] != -11)
        {
            cout << "PBM Failed";
            cerr << "PBM Failed";
        }

        cerr << "\n";
    }

    if (InputToggle == 2)
    {
        char storage[256];
        int u = 0;

        while (input[u] != '\0' && input[u] != '\r')
        {
            storage[u] = input[u];
            u++;
        }

        storage[u] = '\0';
        cout << "\n" << storage << "\n";
        InputVec.push_back(storage);
    }

    if (InputToggle == 3)
    {
        char storage[256];
        int u = 0;

        while (input2[u] != '\0' && input2[u] != '\r')
        {
            storage[u] = input2[u];
            u++;
        }

        storage[u] = '\0';
        cout << "\n" << storage << "\n";
        InputVec.push_back(storage);
    }

    InputToggle = 0;

    cout << "\n";
    VP8DefaultParms(opt);
    //Close Log Output
    fclose(fp);

    //Open summary Output
    if ((fp = freopen(TextfileString2.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString2.c_str());
        exit(1);
    }

    int f = 0;
    printf("\n\n");

    int e = 0;
    int OutLength = 0;

    //get the largest inputfile string length
    while (e < TestCounter)
    {
        if (FileExistsCheck(InputVec[e].c_str()))
        {
            if (OutLength < InputVec[e].length())
            {
                OutLength = InputVec[e].length();
            }
        }

        e++;
    }

    if (OutLength < 38)
    {
        OutLength = 38;
    }

    //Output to screen
    cout << "\n*************************************Summary************************************\n\n";
    cout.width(5);
    cout << "Test#";
    cout.width(40);
    cout << "Input File";
    cout.width(10);
    cout.precision(4);
    cout << "PSNR";
    cout.width(10);
    cout.precision(4);
    cout << "DataRate";
    cout.width(10);
    cout.precision(2);
    cout << "BitRate";
    cout.width(4);
    cout.precision(2);
    cout << "PBM";
    cout << "\n";

    //OutPut to file

    cerr << "ComboTestRunSummary\n\n\n";
    cerr << "\n*************************************Summary************************************\n\n";
    cerr.width(5);
    cerr << "Test#";
    cerr.width(OutLength + 2);
    cerr << "Input File";
    cerr.width(10);
    cerr.precision(2);
    cerr << "PSNR";
    cerr.width(10);
    cerr.precision(2);
    cerr << "DataRate";
    cerr.width(10);
    cerr.precision(2);
    cerr << "BitRate";
    cerr.width(4);
    cerr.precision(2);
    cerr << "PBM";
    cerr << "\n";



    while (f < TestCounter)
    {
        if (testworked[f] == 1)
        {
            //Output to screen
            cout.width(5);
            cout << f + 1;
            cout.width(40);

            if (InputVec[f].length() <= 38)
            {
                cout << InputVec[f].c_str();
            }
            else
            {
                char PrintBuffer[40];
                char ResizeInputBufferAr[255];
                snprintf(ResizeInputBufferAr, 255, "%s", InputVec[f].c_str());
                PrintBuffer[0] = ResizeInputBufferAr[0];
                PrintBuffer[1] = ResizeInputBufferAr[1];
                PrintBuffer[2] = ResizeInputBufferAr[2];
                PrintBuffer[3] = '.';
                PrintBuffer[4] = '.';
                int t = 5;

                while (t < 38)
                {
                    int s;
                    s = 38 - t;
                    PrintBuffer[t] = ResizeInputBufferAr[InputVec[f].length() - s];
                    t++;
                }

                PrintBuffer[t] = '\0';
                cout << PrintBuffer;
            }

            cout.width(10);
            cout.precision(4);
            cout << PSNRAr[f];
            cout.width(10);
            cout.precision(5);
            cout << DataRateAr[f];
            cout.width(10);
            cout.precision(5);
            cout << BitRateAr[f];
            cout.width(4);
            cout.precision(2);
            cout << PBMAr[f];
            cout << "\n";

            //OutPut to file
            cerr.width(5);
            cerr << f + 1;
            cerr.width(OutLength + 2);
            cerr << InputVec[f].c_str();
            cerr.width(10);
            cerr.precision(4);
            cerr << PSNRAr[f];
            cerr.width(10);
            cerr.precision(5);
            cerr << DataRateAr[f];
            cerr.width(10);
            cerr.precision(2);
            cerr << BitRateAr[f];
            cerr.width(4);
            cerr.precision(2);
            cerr << PBMAr[f];
            cerr << "\n";
        }
        else
        {
            cout.width(5);
            cout << f + 1;
            cout.width(40);

            if (InputVec[f].length() <= 38)
            {
                cout << InputVec[f].c_str();
            }
            else
            {
                char PrintBuffer[40];
                char ResizeInputBufferAr[255];
                snprintf(ResizeInputBufferAr, 255, "%s", InputVec[f].c_str());
                PrintBuffer[0] = ResizeInputBufferAr[0];
                PrintBuffer[1] = ResizeInputBufferAr[1];
                PrintBuffer[2] = ResizeInputBufferAr[2];
                PrintBuffer[3] = '.';
                PrintBuffer[4] = '.';
                int t = 5;

                while (t < 38)
                {
                    int s;
                    s = 38 - t;
                    PrintBuffer[t] = ResizeInputBufferAr[InputVec[f].length() - s];
                    t++;
                }

                PrintBuffer[t] = '\0';
                cout << PrintBuffer;
            }

            cout << "\n";


            cerr.width(5);
            cerr << f + 1;
            cerr.width(OutLength + 2);
            cerr << InputVec[f].c_str();
            cerr << "\n";

        }

        f++;
    }

    fclose(fp);
    return 3;
}
int AltQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "AltQ";
    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  AltQ \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <AltQ 1>\n "
            "    <AltQ 2>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "AltQ";
    string MainDirString = "";
    char File1[255] = "";

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("AltQ_");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("AltQ_");

    WorkingDir4.append(argv[5]);
    WorkingDir5.append(argv[6]);

    WorkingDir4.append(".ivf");
    WorkingDir5.append(".ivf");

    char AltQ1[255];
    char AltQ2[255];

    snprintf(AltQ1, 255, "%s", WorkingDir4.c_str());
    snprintf(AltQ2, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int AltQ1Int = atoi(argv[5]);
    int AltQ2Int = atoi(argv[6]);


    printf("Alt Q Test");
    fprintf(stderr, "Alt Q Test");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////
    double PSNRArr[2];
    opt.TargetBandwidth = BitRate;
    opt.FixedQ = 1;

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.AltQ = AltQ1Int;

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AltQ = AltQ2Int;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.AltQ = AltQ1Int;

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AltQ = AltQ2Int;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            opt.AltQ = AltQ1Int;

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AltQ = AltQ2Int;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.AltQ = AltQ1Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltQ = AltQ2Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.AltQ = AltQ1Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltQ = AltQ2Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    printf("\n");
    fprintf(stderr, "\n");
    int FileSize1 = FileSize(AltQ1);
    printf("\n");
    fprintf(stderr, "\n");
    int FileSize2 = FileSize(AltQ2);
    printf("\n");
    fprintf(stderr, "\n");

    int PSNRToggle = PSNRSelect(input, AltQ1);

    double ssim = 0;
    PSNRArr[0] = IVFPSNR(input, AltQ1, PSNRToggle, 0, 1, ssim);
    PSNRArr[1] = IVFPSNR(input, AltQ2, PSNRToggle, 0, 1, ssim);

    printf("\n AltQ %i: %f - AltQ %i: %f \n"
           " AltQ %i: %i - AltQ %i: %i \n"
           , AltQ1Int, PSNRArr[0], AltQ2Int, PSNRArr[1],
           AltQ1Int, FileSize1,
           AltQ2Int, FileSize2);
    fprintf(stderr, "\n AltQ %i: %f - AltQ %i: %f \n"
            " AltQ %i: %i - AltQ %i: %i \n"
            , AltQ1Int, PSNRArr[0], AltQ2Int, PSNRArr[1],
            AltQ1Int, FileSize1,
            AltQ2Int, FileSize2);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 2;
}
int KeyQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "KeyQ";
    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  KeyQ \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <KeyQ 1>\n "
            "    <KeyQ 2>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }


    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "KeyQ";
    string MainDirString = "";
    char File1[255] = "";

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("KeyQ_");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("KeyQ_");

    WorkingDir4.append(argv[5]);
    WorkingDir5.append(argv[6]);

    WorkingDir4.append(".ivf");
    WorkingDir5.append(".ivf");

    char KeyQ1[255];
    char KeyQ2[255];

    snprintf(KeyQ1, 255, "%s", WorkingDir4.c_str());
    snprintf(KeyQ2, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int KeyQ1Int = atoi(argv[5]);
    int KeyQ2Int = atoi(argv[6]);

    printf("Key Q Test");
    fprintf(stderr, "Key Q Test");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////
    double PSNRArr[2];
    opt.TargetBandwidth = BitRate;
    opt.FixedQ = 1;

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.KeyQ = KeyQ1Int;

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.KeyQ = KeyQ2Int;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.KeyQ = KeyQ1Int;

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.KeyQ = KeyQ2Int;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            opt.KeyQ = KeyQ1Int;

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.KeyQ = KeyQ2Int;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.KeyQ = KeyQ1Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.KeyQ = KeyQ2Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.KeyQ = KeyQ1Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.KeyQ = KeyQ2Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    printf("\n");
    fprintf(stderr, "\n");
    int FileSize1 = FileSize(KeyQ1);
    printf("\n");
    fprintf(stderr, "\n");
    int FileSize2 = FileSize(KeyQ2);
    printf("\n");
    fprintf(stderr, "\n");

    int PSNRToggle = PSNRSelect(input, KeyQ1);

    double ssim = 0;
    PSNRArr[0] = IVFPSNR(input, KeyQ1, PSNRToggle, 0, 1, ssim);
    PSNRArr[1] = IVFPSNR(input, KeyQ2, PSNRToggle, 0, 1, ssim);

    printf("\n KeyQ %i: %f - KeyQ %i: %f \n"
           " KeyQ %i: %i - KeyQ %i: %i \n"
           , KeyQ1Int, PSNRArr[0], KeyQ2Int, PSNRArr[1],
           KeyQ1Int, FileSize1,
           KeyQ2Int, FileSize2);
    fprintf(stderr, "\n KeyQ %i: %f - KeyQ %i: %f \n"
            " KeyQ %i: %i - KeyQ %i: %i \n"
            , KeyQ1Int, PSNRArr[0], KeyQ2Int, PSNRArr[1],
            KeyQ1Int, FileSize1,
            KeyQ2Int, FileSize2);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 2;
}
int GoldQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "GoldQ";
    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  GoldQ \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <GoldQ 1>\n "
            "    <GoldQ 2>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "GoldQ";
    string MainDirString = "";
    char File1[255] = "";

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("GoldQ_");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("GoldQ_");

    WorkingDir4.append(argv[5]);
    WorkingDir5.append(argv[6]);

    WorkingDir4.append(".ivf");
    WorkingDir5.append(".ivf");

    char GoldQ1[255];
    char GoldQ2[255];

    snprintf(GoldQ1, 255, "%s", WorkingDir4.c_str());
    snprintf(GoldQ2, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int GoldQ1Int = atoi(argv[5]);
    int GoldQ2Int = atoi(argv[6]);

    printf("Gold Q Test");
    fprintf(stderr, "Gold Q Test");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////
    double PSNRArr[2];
    opt.TargetBandwidth = BitRate;
    opt.FixedQ = 1;

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.GoldQ = GoldQ1Int;

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.GoldQ = GoldQ2Int;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.GoldQ = GoldQ1Int;

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.GoldQ = GoldQ2Int;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            opt.GoldQ = GoldQ1Int;

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.GoldQ = GoldQ2Int;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.GoldQ = GoldQ1Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.GoldQ = GoldQ2Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.GoldQ = GoldQ1Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.GoldQ = GoldQ2Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    printf("\n");
    fprintf(stderr, "\n");
    int FileSize1 = FileSize(GoldQ1);
    printf("\n");
    fprintf(stderr, "\n");
    int FileSize2 = FileSize(GoldQ2);
    printf("\n");
    fprintf(stderr, "\n");

    int PSNRToggle = PSNRSelect(input, GoldQ1);

    double ssim = 0;
    PSNRArr[0] = IVFPSNR(input, GoldQ1, PSNRToggle, 0, 1, ssim);
    PSNRArr[1] = IVFPSNR(input, GoldQ2, PSNRToggle, 0, 1, ssim);

    printf("\n GoldQ %i: %f - GoldQ %i: %f \n"
           " GoldQ %i: %i - GoldQ %i: %i \n"
           , GoldQ1Int, PSNRArr[0], GoldQ2Int, PSNRArr[1],
           GoldQ1Int, FileSize1,
           GoldQ2Int, FileSize2);
    fprintf(stderr, "\n GoldQ %i: %f - GoldQ %i: %f \n"
            " GoldQ %i: %i - GoldQ %i: %i \n"
            , GoldQ1Int, PSNRArr[0], GoldQ2Int, PSNRArr[1],
            GoldQ1Int, FileSize1,
            GoldQ2Int, FileSize2);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 2;
}


int AllowSpatialResamplingTest2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "AllowSpatialResampling";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  AllowSpatialResampling2 \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n");
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "AllowSpatialResamplingTest2";
    string MainDirString = "";
    char File1[255] = "";

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        //
        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }


    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("AllowSpatialResampling2Output.ivf");

    char SpatialResamp2OutFile[255];

    snprintf(SpatialResamp2OutFile, 255, "%s", WorkingDir4.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);


    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);


    printf("AllowSpatialResamplingTest2\n");
    fprintf(stderr, "AllowSpatialResamplingTest2\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.TargetBandwidth = BitRate;
    opt.EndUsage = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            printf("\n\n");
            fprintf(stderr, "\n\n");
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            printf("\n\n");
            fprintf(stderr, "\n\n");
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            printf("\n\n");
            fprintf(stderr, "\n\n");
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.AllowSpatialResampling = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            printf("\n\n");
            fprintf(stderr, "\n\n");
        }

        if (Mode == 5)
        {
            opt.AllowSpatialResampling = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            printf("\n\n");
            fprintf(stderr, "\n\n");
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    double PeakSNRon;

    double ssim = 0;
    PeakSNRon = IVFPSNR(input, SpatialResamp2OutFile, 0, 0, 1, ssim);

    if (PeakSNRon > 15)
    {
        printf("\n Allow Spatial Resample On - PSNR: %f > 15 - Passed\n", PeakSNRon);
        fprintf(stderr, "\n Allow Spatial Resample On - PSNR: %f > 15 - Passed\n", PeakSNRon);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    printf("\n Allow Spatial Resample On - PeakSNR: %f < 15 - Failed\n", PeakSNRon);
    fprintf(stderr, "\n Allow Spatial Resample On - PeakSNR: %f < 15 - Failed\n", PeakSNRon);
    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

int NoiseSensitivityWorksORIGINAL(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "NoiseSensitivity";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  NoiseSensitivityWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "NoiseSensitivityWorks";
    string MainDirString = "";
    char File1[255] = "";

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr);
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");
        MakeDirVPX(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            SubFolderName(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr);
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::in | fstream::out | fstream::app);

        int n = 0;

        while (n < atoi(argv[argc]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        TestName(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            printf("ErrorFileMisMatch ");
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch.c_str(), MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }


    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("NoiseSens0Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("NoiseSens6Output.ivf");

    char NoiseSens0OutFile[255];
    char NoiseSens6OutFile[255];

    snprintf(NoiseSens0OutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(NoiseSens6OutFile, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        PrintHeader1(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        PrintHeader2(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        PrintHeader3(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);


    printf("Noise Sensitivity Works Test");
    fprintf(stderr, "Noise Sensitivity Works Test");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        FILE *InputCheck = fopen(argv[argc-1], "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(argv[argc-1]);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.TargetBandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.NoiseSensitivity = 0;

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.NoiseSensitivity = 6;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.NoiseSensitivity = 0;

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.NoiseSensitivity = 6;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            opt.NoiseSensitivity = 0;

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.NoiseSensitivity = 6;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.NoiseSensitivity = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.NoiseSensitivity = 6;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.NoiseSensitivity = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.NoiseSensitivity = 6;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    int PSNRToggle = PSNRSelect(input, NoiseSens0OutFile);

    printf("\n");
    fprintf(stderr, "\n");

    double PSNR0;
    double PSNR6;
    double ssim = 0;

    PSNR0 = IVFPSNR(input, NoiseSens0OutFile, PSNRToggle, 0, 1, ssim);
    PSNR6 = IVFPSNR(input, NoiseSens6OutFile, PSNRToggle, 0, 1, ssim);

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    if (PSNR6 < PSNR0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Noise 6 PSNR: %.2f < Noise 0 PSNR: %.2f - Passed", PSNR6, PSNR0);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");

        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (PSNR6 > PSNR0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Noise 0 PSNR: %.2f > Noise 6 PSNR: %.2f - Failed", PSNR0, PSNR6);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");

        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Noise 0 PSNR: %.2f == Noise 6 PSNR: %.2f - Indeterminate", PSNR0, PSNR6);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");

        printf("\nIndeterminate\n");
        fprintf(stderr, "\nIndeterminate\n");

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

}