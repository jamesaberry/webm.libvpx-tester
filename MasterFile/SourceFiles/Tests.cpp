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
extern int CompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt);
extern unsigned int TimeCompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt);
extern int DecompressIVFtoIVF(char *inputFile, char *outputFile2);
extern unsigned int TimeDecompressIVFtoIVF(char *inputFile, char *outputFile2);
extern int KeyFrameCompressIVFtoIVF(char *inputFile, char *outputFile, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt, char *outputfile);
extern int KeyFrameInOutCompressIVFtoIVF(char *inputFile, char *outputFile, int speed, int BitRate, VP8_CONFIG opt, char *CompressString, int CompressInt, char *inputfile, char *outputfile, int InOutControl);
extern double IVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, double &SsimOut);
extern double PostProcIVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double &SsimOut);
extern int PSNRSelect(char *inFile, char *outFile);
extern double IVFDataRate(char *inputFile, int DROuputSel);
extern int IVFCheckPBM(char *inputFile, int bitRate, int maxBuffer, int preBuffer);
extern int CompIVF(char *inputFile1, char *inputFile2);
extern double IVFDisplayKeyFrames(char *inputFile, int Selector, char *outputFile);
extern int MultiPlatTimeRet(char *infile);
extern int DecompressIVFtoRaw(char *inputFile, char *outputFile2);
extern int DecompressIVFtoIVFErrorDetection(char *inputFile, char *outputFile2);
extern void VP8DefaultParms(VP8_CONFIG &opt);
extern int DecComputeMD5(char *inputchar, char *outputchar);

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
extern int EncoderBreakOutTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
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

void RecordTestComplete(string MainDirString, string File1String, int TestType)
{
    if (TestType == 2)
    {
        fstream FileStream;
        FileStream.open(MainDirString.c_str(), fstream::out | fstream::app);

        if (!FileStream.good())
        {
            printf("\nERROR WRITING FILE: FILEINDEX: %s\n", MainDirString.c_str());
            return;
        }

        FileStream << File1String << "\n";
        FileStream.close();
    }

    return;
}
int PrintVersion()
{
    printf("\n%s\n", on2_codec_iface_name(&on2_codec_vp8_cx_algo));
    return 0;
}
void PrintHeader1(int argc, char *argv[], string WorkingDir3)
{
    //Full Test Header Output
    //Formats workingDir3 string to fit in text box
    //records settings from argv to be written to text file
    string PrintWorkingDir3 = WorkingDir3;
    string PrintInput = "Input:";
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

    string TestMachineInfo = "                 Test Machine is Running: Unknown Platform\n\n";
#if defined(_WIN32)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));
    TestMachineInfo.append("\n                     Test Machine is Running: Windows\n\n");
#endif
#if defined(linux)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Linux\n\n");
#endif
#if defined(__APPLE__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Mac\n\n");
#endif
#if defined(__POWERPC__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                      Test Machine is Running: PowerPC\n\n");
#endif

    printf("\n/////////////////////////////////Full Test/////////////////////////////////\n%s\n\n%s", PrintInput.c_str(), PrintWorkingDir3.c_str());
    printf("\n///////////////////////////////////////////////////////////////////////////\n\n");
    fprintf(stderr, "%s", TestMachineInfo.c_str());
    fprintf(stderr, "\n/////////////////////////////////Full Test/////////////////////////////////\n%s\n\n%s", PrintInput.c_str(), PrintWorkingDir3.c_str());
    fprintf(stderr, "\n///////////////////////////////////////////////////////////////////////////\n\n");
}
void PrintHeader2(int argc, char *argv[], string WorkingDir3)
{
    //Compression Header
    //Formats workingDir3 string to fit in text box
    //records settings from argv to be written to text file

    string PrintWorkingDir3 = WorkingDir3;
    string PrintInput = "Input:";
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

    string TestMachineInfo = "                 Test Machine is Running: Unknown Platform\n\n";

#if defined(_WIN32)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                      Test Machine is Running: Windows\n\n");
#endif
#if defined(linux)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Linux\n\n");
#endif
#if defined(__APPLE__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                        Test Machine is Running: Mac\n\n");
#endif
#if defined(__POWERPC__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                      Test Machine is Running: PowerPC\n\n");
#endif

    printf("\n///////////////////////////////Compress Only///////////////////////////////\n");
    cout << PrintInput;
    cout << "\n\n";
    cout << PrintWorkingDir3;
    printf("\n///////////////////////////////////////////////////////////////////////////\n\n");

    cerr << TestMachineInfo;
    fprintf(stderr, "\n///////////////////////////////Compress Only///////////////////////////////\n");
    cerr << PrintInput;
    cerr << "\n\n";
    cerr << PrintWorkingDir3;
    fprintf(stderr, "\n///////////////////////////////////////////////////////////////////////////\n\n");
}
void PrintHeader3(int argc, char *argv[], string  WorkingDir3)
{
    //Test Only Header
    //Formats workingDir3 string to fit in text box records input
    //location and output location both are the same

    string PrintWorkingDir3 = WorkingDir3;
    string PrintWorkingDir4 = WorkingDir3;
    string PrintInput = "Input:";
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

    string TestMachineInfo = "                 Test Machine is Running: Unknown Platform\n\n";

#if defined(_WIN32)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                     Test Machine is Running: Windows\n\n");
#endif
#if defined(linux)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Linux\n\n");
#endif
#if defined(__APPLE__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                        Test Machine is Running: Mac\n\n");
#endif
#if defined(__POWERPC__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(on2_codec_iface_name(&on2_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                      Test Machine is Running: PowerPC\n\n");
#endif

    printf("\n/////////////////////////Existing Compression Test/////////////////////////\n");
    cout << PrintWorkingDir4;
    cout << "\n\n";
    cout << PrintWorkingDir3;
    printf("\n///////////////////////////////////////////////////////////////////////////\n\n");

    cerr << TestMachineInfo;
    fprintf(stderr, "\n/////////////////////////Existing Compression Test/////////////////////////\n");
    cerr << PrintWorkingDir4;
    cerr << "\n\n";
    cerr << PrintWorkingDir3;
    fprintf(stderr, "\n///////////////////////////////////////////////////////////////////////////\n\n");
}
void CheckTimeStamp(int SelectorArInt, string *SelectorAr, string *SelectorAr2, string TimeStampPrevious, int &identicalFileVar, string *TimeStampAr2)
{
    char identicalFileBuffer[3] = "";

    if (SelectorArInt != 0 && SelectorAr[SelectorArInt].compare(SelectorAr[SelectorArInt-1]) == 0)
    {
        if (TimeStampCompare(TimeStampAr2[0], TimeStampPrevious))
        {
            identicalFileVar++;
            itoa_custom(identicalFileVar, identicalFileBuffer, 10);
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
int ExternalTestRunner(int argc, char *argv[], string WorkingDir,  int NumberofTests)
{
    if (argc < 5)
    {
        if (argc < 4)
        {
            printf(
                "  ExternalTestRunner \n\n"
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
    }

    if (WorkingDir.length() > 175)
    {
        printf("\nError: Folder Path Length too long.\n");
        fprintf(stderr, "\nError: Folder Path Length too long.\n");
        return 0;
    }

    char buffer[512] = "";
    char buffer2[512] = "";
    char identicalFileBuffer[3] = "";

    //int PassFail[9999] = {0};
    int *PassFail = new int[NumberofTests+2];

    string SummCompAndTest = "";
    string SummComp = "";
    string SummTest = "";

    char *input = argv[3];

    string StringAr[20];
    string TimeStampAr2[2];
    string SelectorAr[999];
    string SelectorAr2[999];

    int y = 0;
    int Buf1Var = 0;
    int TestsRun = 0;
    int PrintMe = 1;
    int DummyArgvVar = 1;
    int trackthis1 = 0;
    int trackthis2 = 0;
    int CommentBool = 0;
    int PassFailInt = 0;
    int SelectorArInt = 0;
    int TestsRunCatch = 0;
    int MakeNewTestrun = 1;
    int identicalFileVar = 1;
    int NumberOfTestsRun = 1;
    int TestType = atoi(argv[2]);
    int linesskippedtracker = 0;

    long FilePositionTracker = 0;
    long FilePositionTracker2 = 0;

    string TimeStampPrevious = "";

    string WorkDirFileStr = "";
    string WorkingTextFilestr = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "Summary";


////////////////////Sets Stage for Resume Mode//////////////////////
    if (TestType == 4)
    {
        // Read in whats been done already from the summary updating all variables needed
        // Start running the test from TestRuns.txt file since it is auto commented it will resume with the last
        // Test not completed.  So long as all arrays are updated properly and variables are as well it should work
        // Seemlessly.

        WorkingDir = argv[3];											//location of main dir in as workingdir

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr);
        WorkingTextFilestr.append("TestsRun.txt");						//TestsRun.txt original file ided

        //Determines which test is in the process of being run and sets the correct text file as input file
        SummCompAndTest = WorkingDir;
        SummCompAndTest.append(slashCharStr);
        SummCompAndTest.append("Mode1Results.txt");		// Mode 1
        //SummCompAndTest.append("Summary_CompressionsANDTests.txt");		// Mode 1
        SummComp = WorkingDir;
        SummComp.append(slashCharStr);
        SummComp.append("Mode2Results.txt");				// Mode 2
        //SummComp.append("Summary_CompressionsOnly.txt");				// Mode 2
        SummTest = WorkingDir;
        SummTest.append(slashCharStr);
        SummTest.append("Mode3Results.txt");						// Mode 3
        //SummTest.append("Summary_TestsOnly.txt");						// Mode 3

        int TestRunning = 0;

        fstream SummCompAndTestFile;
        SummCompAndTestFile.open(SummCompAndTest.c_str(), fstream::in);

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

        //exits with error if it cant find a summary file
        if (TestRunning == 0)
        {
            printf("\nError: No Summary File found\n");
            delete [] PassFail;
            return 0;
        }

        ///Sets Correct input file
        string PriorResultInput;

        if (TestRunning == 1)
        {
            PriorResultInput = SummCompAndTest;
        }

        if (TestRunning == 2)
        {
            PriorResultInput =  SummComp;
        }

        if (TestRunning == 3)
        {
            PriorResultInput = SummTest;
        }

        fstream PriorResultInputFile;
        PriorResultInputFile.open(PriorResultInput.c_str());

        int TestsrunCount = 0;
        int VarToInput = 0;
        int p = 0;

        //read in and throw away header
        while (p < 7)
        {

            PriorResultInputFile.getline(buffer, 1024);
            p++;
        }

        //read in data
        while (!PriorResultInputFile.eof())
        {
            PriorResultInputFile.getline(buffer, 1024);
            string bufferStr = buffer;
            int u = 0;

            //0 number not read yet|1 TestType not read yet|2 TestFolder not read yet|3 Status not read yet
            while (buffer[u] != '\0' && buffer[u] != '\r' && u <= bufferStr.length())
            {
                if (buffer[u] != ' ')
                {
                    if (VarToInput == 5)
                    {
                        VarToInput = 0;
                    }

                    if (VarToInput == 3)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != '\0' && buffer[u] != '\r')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        buffer[u] = '\0';
                        string PassFailStr = buffer3;

                        if (PassFailStr.compare("Passed") == 0)
                        {
                            PassFail[TestsrunCount] = 1;
                        }

                        if (PassFailStr.compare("Failed") == 0)
                        {
                            PassFail[TestsrunCount] = 0;
                        }

                        if (PassFailStr.compare("Indeterminate") == 0)
                        {
                            PassFail[TestsrunCount] = 2;
                        }

                        if (PassFailStr.compare("SeeComboRunLog") == 0)
                        {
                            PassFail[TestsrunCount] = 3;
                        }

                        if (PassFailStr.compare("SeePSNRLog") == 0)
                        {
                            PassFail[TestsrunCount] = 4;
                        }

                        if (PassFailStr.compare("RandomTestCompleted") == 0)
                        {
                            PassFail[TestsrunCount] = 5;
                        }

                        if (PassFailStr.compare("MinTestPassed") == 0)
                        {
                            PassFail[TestsrunCount] = 8;
                        }

                        if (PassFailStr.compare("CompressionMade") == 0)
                        {
                            PassFail[TestsrunCount] = 10;
                        }

                        if (PassFailStr.compare("ErrorFileMismatch") == 0)
                        {
                            PassFail[TestsrunCount] = 11;
                        }

                        if (PassFailStr.compare("TestNotSupported") == 0)
                        {
                            PassFail[TestsrunCount] = 12;
                        }

                        VarToInput = 4;

                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 2)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (a < 24)
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        SelectorAr2[TestsrunCount] = buffer3;
                        SelectorAr2[TestsrunCount].append("\"");

                        VarToInput = 3;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 1)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != ' ')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        SelectorAr[TestsrunCount] = buffer3;
                        VarToInput = 2;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 0)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != ' ')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;
                        }

                        VarToInput = 1;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    //resets var at end
                    if (VarToInput == 4)
                    {
                        int a = 0;

                        while (buffer[u] != '\0' && buffer[u] != '\r')
                        {
                            u++;
                            a++;
                        }

                        VarToInput = 0;
                    }
                }

                u++;
            }


            if (buffer[0] != '\0' && buffer[u] != '\r')
            {
                // if the first character of a line after the initial input is read happens to be a dash
                // it signifies the end of Tests being run and the start of the summary, meaning that all
                // operations have already been run and the tests should teminate.
                if (buffer[0] == '-')
                {
                    if (TestRunning == 1)
                    {
                        printf("\nAll tests and compressions have already been created and run.\n");
                        PriorResultInputFile.close();
                        exit(0);
                    }

                    if (TestRunning == 2)
                    {
                        cout << "\nAll compressions have already been created.\n";
                        PriorResultInputFile.close();
                        exit(0);
                    }

                    if (TestRunning == 3)
                    {
                        cout << "\nAll tests on compressions have already been run.\n";
                        PriorResultInputFile.close();
                        exit(0);
                    }
                }

                TestsrunCount++;
            }

        }

        TestType = TestRunning;
        SelectorArInt = TestsrunCount;
        NumberOfTestsRun = TestsrunCount;
        PassFailInt = TestsrunCount;
        TestsRun = TestsrunCount;
        TestsRunCatch = TestsrunCount;

        if (TestType == 3)
        {
            NumberOfTestsRun++;
        }

        PriorResultInputFile.close();

        WorkingDir.append("\"");

        PrintMe = 0;
        MakeNewTestrun = 0;
    }

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////Format Output///////////////////////////


    if (TestType == 1)
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

        WorkDirFileStr = WorkingDir3;
        //WorkDirFileStr.append(MyDir);
        //WorkDirFileStr.append("_CompressionsANDTests.txt");
        WorkDirFileStr.append("Mode1Results.txt");

    }

    if (TestType == 2)
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

        WorkDirFileStr = WorkingDir3;
        //WorkDirFileStr.append(MyDir);
        //WorkDirFileStr.append("_CompressionsOnly.txt");
        WorkDirFileStr.append("Mode2Results.txt");
    }

    if (TestType == 3)
    {
        WorkDirFileStr = argv[3];
        WorkDirFileStr.append(slashCharStr);
        //WorkDirFileStr.append(MyDir);
        //WorkDirFileStr.append("_TestsOnly.txt");
        WorkDirFileStr.append("Mode3Results.txt");
    }


    if (TestType != 4)
    {
        ////////////Formatting New Input TextFile///////////
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';

        WorkingTextFilestr = WorkingDir3;
        WorkingTextFilestr.append("TestsRun.txt");
        ///////////////////////////////////////////////////

    }

////////////////////////////////////////////////////////////////////
/////////////////Makes New Working Text File From Input/////////////
    if (TestType != 3 && MakeNewTestrun == 1)
    {
        //This copies original driving text file over to a new formated one not used for Mode 4 or 3
        fstream infile;
        infile.open(input);

        if (!infile.good())
        {
            printf("\nInput File does not exist\n");
            delete [] PassFail;
            return 0;
        }

        string CreateDir2(WorkingDir);
        CreateDir2.insert(0, "md \"");
        CreateDir2.erase(CreateDir2.end() - 1);
        MakeDirVPX(CreateDir2);

        ofstream outfile(WorkingTextFilestr.c_str());

        while (!infile.eof())
        {
            infile.getline(buffer, 512);

            if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' || buffer[0] == '\r')
            {
                outfile << buffer << "\n";
            }
            else
            {
                if (buffer[0] != ' ')
                {
                    outfile << " " << buffer << "\n";
                }
                else
                {
                    outfile << buffer << "\n";
                }
            }
        }

        infile.close();
        outfile.close();
    }

    if (TestType == 3)
    {


        WorkingDir = argv[3];								//location of main dir in as workingdir

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr);
        WorkingTextFilestr.append("TestsRun.txt");			//TestsRun.txt original file ided

        WorkingDir.append("\"");

        //Need to go through and replace all '+' from input file with '-' so that test can keep track of "Tests Only" run
        fstream WorkingTextFilePrep;
        WorkingTextFilePrep.open(WorkingTextFilestr.c_str());

        while (!WorkingTextFilePrep.eof())
        {
            FilePositionTracker = WorkingTextFilePrep.tellg();
            WorkingTextFilePrep.getline(buffer, 1024);
            FilePositionTracker2 = WorkingTextFilePrep.tellg();

            string bufferstring = buffer;
            bufferstring.erase(0);

            if (buffer[0] == '+')
            {
                bufferstring.insert(0, " ");
                WorkingTextFilePrep.seekg(FilePositionTracker);
                WorkingTextFilePrep << bufferstring.c_str() ;
                WorkingTextFilePrep.seekg(FilePositionTracker2);
            }
        }


        WorkingTextFilePrep.close();

        string SummComp = argv[3];
        SummComp.append(slashCharStr);
        SummComp.append("Mode2Results.txt");
        //SummComp.append("Summary_CompressionsOnly.txt");

        fstream PriorResultInputFile2;
        PriorResultInputFile2.open(SummComp.c_str());

        if (!PriorResultInputFile2.good())
        {
            printf("\nError opening file: %s\n", SummComp.c_str());
            delete [] PassFail;
            return 0;
        }

        int TestsrunCount = 0;
        int VarToInput = 0;
        int p = 0;

        //read in and throw away header
        while (p < 7)
        {
            PriorResultInputFile2.getline(buffer, 1024);
            p++;
        }

        //read in data
        int runonce = 1;
        char buffer3[255];

        while (!PriorResultInputFile2.eof())
        {
            PriorResultInputFile2.getline(buffer, 1024);
            runonce = 1;
            int u = 0;
            //0 number not read yet|1 TestType not read yet|2 TestFolder not read yet|3 Status not read yet

            int Q = 35;

            while (Q < 35 + 24)
            {
                buffer3[u] = buffer[Q];
                Q++;
                u++;
            }

            SelectorAr2[TestsrunCount] = buffer3;
            SelectorAr2[TestsrunCount].append("\"");

            if (buffer[0] != '\0' && buffer[u] != '\r')
            {
                TestsrunCount++;
            }

            memset(buffer3, 0, sizeof(buffer3));
        }

        PriorResultInputFile2.close();
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fstream WorkingTextFile;
    WorkingTextFile.open(WorkingTextFilestr.c_str());

    if (!WorkingTextFile.good())
    {
        printf("\nInput File does not exist\n");
        delete [] PassFail;
        return 0;
    }

/////////////////////////////////////////Files are actualy processed and run here//////////////////////////////////////////////
    while (!WorkingTextFile.eof())
    {
        memset(buffer, 0, sizeof(buffer));

        const char *DummyArgv[999];
        DummyArgv[0] = argv[0];
        DummyArgvVar = 1;
        FilePositionTracker = WorkingTextFile.tellg();
        WorkingTextFile.getline(buffer, 1024);
        FilePositionTracker2 = WorkingTextFile.tellg();

        string bufferString = buffer;

        trackthis1++;

        //skips over any line starting with a % in the input file to allow for comenting
        if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' || buffer[0] == '-' || buffer[0] == '\r')
        {
            linesskippedtracker++;
        }
        else
        {
            Buf1Var = 0;

#if defined(_WIN32)

            while (buffer[Buf1Var] != '\0')
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\0')
                {
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\0')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar].clear();
                    StringAr[DummyArgvVar].insert(0, buffer2);
                    DummyArgvVar++;
                }
            }

#elif defined(linux)

            while (buffer[Buf1Var] != '\0')
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\0')
                {
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\0')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

#elif defined(__APPLE__)

            while (buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
                {
                    //cout << buffer[Buf1Var] << "\n";
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\r')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

#elif defined(__POWERPC__)

            while (buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
                {
                    //cout << buffer[Buf1Var] << "\n";
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\r')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

#endif

            y = 1;

            while (y < DummyArgvVar)
            {
                DummyArgv[y] = StringAr[y].c_str();
                y++;
            }

            char NumberOfTestsRunChar[4];
            DummyArgv[y] = itoa_custom(NumberOfTestsRun, NumberOfTestsRunChar, 10);

            if (CommentBool == 0)
            {
                printf("\n");
                int selector = atoi(DummyArgv[1]);

                if (selector >= 0 && selector < 53)
                {
                    if (selector != 11 && selector != 33 && selector != 35 && selector != 0)
                        NumberOfTestsRun++;
                }

                TimeStampPrevious = TimeStampAr2[0];
                TimeStampAr2[0] = DateString();

                //set the correct tester type dir string input
                string TestDir = WorkingDir;

                if (TestType == 3)
                {
                    TestDir = argv[3];
                }

                if (selector == 0)
                {
                    if (TestType == 1)
                    {
                        //ExternalTestRunner(DummyArgvVar, (char **)DummyArgv, WorkingDir,);
                        printf("Not Implemented Yet.\n");
                    }
                }

                if (selector == 1)
                {
                    SelectorAr[SelectorArInt] = "AllowDropFrames";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AllowDF(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 2)
                {
                    SelectorAr[SelectorArInt] = "AllowLagTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AllowLagTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 3)
                {
                    SelectorAr[SelectorArInt] = "AllowSpatialResampling";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AllowSpatialResamplingTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 4)
                {
                    SelectorAr[SelectorArInt] = "AllowSpatialResamplingTest2";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AllowSpatialResamplingTest2(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 5)
                {
                    SelectorAr[SelectorArInt] = "AltFreqTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AltFreqTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 6)
                {
                    SelectorAr[SelectorArInt] = "AutoKeyFramingWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AutoKeyFramingWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 7)
                {
                    SelectorAr[SelectorArInt] = "BufferLevelWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = BufferLevelWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 8)
                {
                    SelectorAr[SelectorArInt] = "CPUDecOnlyWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = CPUDecOnlyWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 9)
                {
                    SelectorAr[SelectorArInt] = "ChangeCPUDec";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ChangeCPUDec(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 10)
                {
                    SelectorAr[SelectorArInt] = "ChangeCPUWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ChangeCPUWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 11)
                {
                    SelectorAr[SelectorArInt] = "DropFramesWaterMarkWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = DFWM(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 12)
                {
                    SelectorAr[SelectorArInt] = "DataRateTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = DataRateTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 13)
                {
                    SelectorAr[SelectorArInt] = "DebugMatchesRelease";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = DebugMatchesRelease(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 14)
                {
                    SelectorAr[SelectorArInt] = "EncoderBreakOut";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = EncoderBreakOutTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 15)
                {
                    SelectorAr[SelectorArInt] = "ErrorResilientModeWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ErrorRes(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 16)
                {
                    SelectorAr[SelectorArInt] = "ExtraFileCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ExtraFileCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 17)
                {
                    SelectorAr[SelectorArInt] = "FixedQ";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = FixedQ(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 18)
                {
                    SelectorAr[SelectorArInt] = "ForceKeyFrameWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ForceKeyFrameWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 19)
                {
                    SelectorAr[SelectorArInt] = "GoodQualityVsBestQuality";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = GoodQvBestQ(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 20)
                {
                    SelectorAr[SelectorArInt] = "LagInFramesTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = LagInFramesTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 21)
                {
                    SelectorAr[SelectorArInt] = "MaxQuantizerTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MaxQTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 22)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MemLeakCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 23)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck2";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MemLeakCheck2(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 24)
                {
                    SelectorAr[SelectorArInt] = "MinQuantizerTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MinQTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 25)
                {
                    SelectorAr[SelectorArInt] = "MultiThreadedTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MultiThreadedTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 26)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldPSNR";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = NewVsOldPSNR(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 27)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldRealTimeSpeed";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = NewVsOldRealTimeSpeed(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 28)
                {
                    SelectorAr[SelectorArInt] = "NoiseSensitivityWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = NoiseSensitivityWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 29)
                {
                    SelectorAr[SelectorArInt] = "OnePassVsTwoPass";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = OnePassVsTwoPass(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 30)
                {
                    SelectorAr[SelectorArInt] = "PlayAlternate";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = PlayAlternate(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 31)
                {
                    SelectorAr[SelectorArInt] = "PostProcessorWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = PostProcessorWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 32)
                {
                    SelectorAr[SelectorArInt] = "PreProcessorWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = PreProcessorWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 33)
                {
                    SelectorAr[SelectorArInt] = "ResampleDownWaterMark";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ResampleDownWaterMark(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 34)
                {
                    SelectorAr[SelectorArInt] = "SpeedTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = SpeedTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 35)
                {
                    SelectorAr[SelectorArInt] = "TestVectorCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = TestVectorCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 36)
                {
                    SelectorAr[SelectorArInt] = "TwoPassVsTwoPassBest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = TwoPassVsTwoPassBest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 37)
                {
                    SelectorAr[SelectorArInt] = "UnderShoot";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = UnderShoot(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 38)
                {
                    SelectorAr[SelectorArInt] = "Version";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = Version(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == 39)
                {
                    SelectorAr[SelectorArInt] = "WindowsMatchesLinux";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = WindowsMatchesLinux(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }




                ////////////////////////////////////On Fly out put/////////////////////
                FILE *fp;

                if ((fp = freopen(WorkDirFileStr.c_str(), "a", stderr)) == NULL)
                {
                    printf("Cannot open summary out put file.\n");
                    delete [] PassFail;
                    return 0;
                }

                if (PrintMe == 1)
                {
                    if (TestType == 1)
                    {
                        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                                "                                  Test Results \n");
                    }

                    if (TestType == 2)
                    {
                        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                                "                                Test Compressions \n");
                    }

                    if (TestType == 3)
                    {

                        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                                "                                Test Only Results \n");
                    }

                    fprintf(stderr, "\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");

                    PrintMe = 0;
                }



                SelectorAr2[SelectorArInt].erase(SelectorAr2[SelectorArInt].end() - 1);
                fprintf(stderr, "%4i %-30s%-28s", SelectorArInt, SelectorAr[SelectorArInt].c_str(), SelectorAr2[SelectorArInt].c_str());

                SelectorAr2[SelectorArInt].append("\"");

                if (PassFail[SelectorArInt] == 1)
                {
                    fprintf(stderr, "Passed\n");
                }

                if (PassFail[SelectorArInt] == 0)
                {
                    fprintf(stderr, "Failed\n");
                }

                if (PassFail[SelectorArInt] == 2)
                {
                    fprintf(stderr, "Indeterminate\n");
                }

                if (PassFail[SelectorArInt] == 3)
                {
                    fprintf(stderr, "SeeComboRunLog\n");
                }

                if (PassFail[SelectorArInt] == 4)
                {
                    fprintf(stderr, "SeePSNRLog\n");
                }

                if (PassFail[SelectorArInt] == 5)
                {
                    fprintf(stderr, "RandomTestCompleted\n");
                }

                if (PassFail[SelectorArInt] == 8)
                {
                    fprintf(stderr, "MinTestPassed\n");
                }

                if (PassFail[SelectorArInt] == 10)
                {
                    fprintf(stderr, "CompressionMade\n");
                }

                if (PassFail[SelectorArInt] == 11)
                {
                    fprintf(stderr, "ErrorFileMismatch\n");
                }

                if (PassFail[SelectorArInt] == 12)
                {
                    fprintf(stderr, "TestNotSupported\n");
                }

                fclose(fp);



                WorkingTextFile.seekg(FilePositionTracker);
                string bufferstring = buffer;
                bufferstring.erase(0);

                if (TestType == 3)
                {
                    bufferstring.insert(0, "-");
                }
                else
                {
                    bufferstring.insert(0, "+");
                }

                WorkingTextFile << bufferstring.c_str() ;

                WorkingTextFile.seekg(FilePositionTracker2);
                ////////////////////////////////////////////////////////////////////////////////


                PassFailInt++;
                SelectorArInt++;
            }

            ////////////////////////////////////////////////////////////////////////////////
            CommentBool = 0;
        }
    }


    WorkingTextFile.close();
    //outputs a summary of the test results

    if (TestsRun == 0)
    {
        cout << "Test file specified contains no tests.\n";
        delete [] PassFail;
        return 0;
    }

    if (TestsRun == TestsRunCatch)
    {
        if (TestType == 1)
        {
            printf("\nAll tests and compressions have already been created and run.\n");
            delete [] PassFail;
            return 0;
        }

        if (TestType == 2)
        {
            printf("\nAll compressions have already been created.\n");
            delete [] PassFail;
            return 0;
        }

        if (TestType == 3)
        {
            printf("\nAll tests on compressions have already been run.\n");
            delete [] PassFail;
            return 0;
        }
    }

    FILE *fp;

    if ((fp = freopen(WorkDirFileStr.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open summary out put file.\n");
        delete [] PassFail;
        return 0;
    }

    if (TestType == 1)
    {
        printf("\n--------------------------------------------------------------------------------\n"
               "                                  Test Results \n");

        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                "                                  Test Results \n");
    }

    if (TestType == 2)
    {
        printf("\n--------------------------------------------------------------------------------\n"
               "                                Test Compressions \n");

        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                "                                Test Compressions \n");
    }

    if (TestType == 3)
    {
        printf("\n--------------------------------------------------------------------------------\n"
               "                                Test Only Results \n");

        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                "                                Test Only Results \n");
    }

    y = 0;

    printf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
    fprintf(stderr, "\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");


    while (y < TestsRun)
    {
        SelectorAr2[y].erase(SelectorAr2[y].end() - 1);
        printf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
        fprintf(stderr, "%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());

        if (PassFail[y] == 1)
        {
            printf("Passed\n");
            fprintf(stderr, "Passed\n");
        }

        if (PassFail[y] == 0)
        {
            printf("Failed\n");
            fprintf(stderr, "Failed\n");
        }

        if (PassFail[y] == 2)
        {
            printf("Indeterminate\n");
            fprintf(stderr, "Indeterminate\n");
        }

        if (PassFail[y] == 3)
        {
            printf("SeeComboRunLog\n");
            fprintf(stderr, "SeeComboRunLog\n");
        }

        if (PassFail[y] == 4)
        {
            printf("SeePSNRLog\n");
            fprintf(stderr, "SeePSNRLog\n");
        }

        if (PassFail[y] == 5)
        {
            printf("RandomTestCompleted\n");
            fprintf(stderr, "RandomTestCompleted.\n");
        }

        if (PassFail[y] == 8)
        {
            printf("MinTestPassed\n");
            fprintf(stderr, "MinTestPassed\n");
        }

        if (PassFail[y] == 10)
        {
            printf("CompressionMade\n");
            fprintf(stderr, "Compression Made\n");
        }

        if (PassFail[y] == 11)
        {
            printf("ErrorFileMismatch\n");
            fprintf(stderr, "ErrorFileMismatch\n");
        }

        if (PassFail[y] == 12)
        {
            printf("TestNotSupported\n");
            fprintf(stderr, "TestNotSupported\n");
        }

        y++;
    }

    if (TestType == 1)
    {
        printf("\n--------------------------------------------------------------------------------\n"
               "                                  Test Results - Passed \n");

        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                "                                  Test Results - Passed \n");
    }

    if (TestType == 3)
    {
        printf("\n--------------------------------------------------------------------------------\n"
               "                                Test Only Results - Passed \n");

        fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                "                                Test Only Results - Passed \n");
    }

    printf("\n\n");
    fprintf(stderr, "\n\n");
    int TestIndicator = 0;

    if (TestType == 3 || TestType == 1)
    {
        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 1)
            {
                if (TestIndicator == 0)
                {
                    printf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                    fprintf(stderr, "\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                printf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                fprintf(stderr, "%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                printf("Passed\n");
                fprintf(stderr, "Passed\n");
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            printf("NONE\n\n");
            fprintf(stderr, "NONE\n\n");
        }

        if (TestType == 1)
        {
            printf("\n--------------------------------------------------------------------------------\n"
                   "                                  Test Results - Failed \n");

            fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                    "                                  Test Results - Failed \n");
        }

        if (TestType == 3)
        {
            printf("\n--------------------------------------------------------------------------------\n"
                   "                                Test Only Results - Failed \n");

            fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                    "                                Test Only Results - Failed \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 0)
            {
                if (TestIndicator == 0)
                {
                    printf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                    fprintf(stderr, "\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                printf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                fprintf(stderr, "%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                printf("Failed\n");
                fprintf(stderr, "Failed\n");
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            printf("NONE\n\n");
            fprintf(stderr, "NONE\n\n");
        }

        if (TestType == 1)
        {
            printf("\n--------------------------------------------------------------------------------\n"
                   "                                  Test Results - Other \n");

            fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                    "                                  Test Results - Other \n");
        }

        if (TestType == 3)
        {
            printf("\n--------------------------------------------------------------------------------\n"
                   "                                Test Only Results - Other \n");

            fprintf(stderr, "\n--------------------------------------------------------------------------------\n"
                    "                                Test Only Results - Other \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] != 0 && PassFail[y] != 1)
            {
                if (TestIndicator == 0)
                {
                    printf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                    fprintf(stderr, "\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                printf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                fprintf(stderr, "%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());

                if (PassFail[y] == 2)
                {
                    printf("Indeterminate\n");
                    fprintf(stderr, "Indeterminate\n");
                }

                if (PassFail[y] == 3)
                {
                    printf("SeeComboRunLog\n");
                    fprintf(stderr, "SeeComboRunLog\n");
                }

                if (PassFail[y] == 4)
                {
                    printf("SeePSNRLog\n");
                    fprintf(stderr, "SeePSNRLog\n");
                }

                if (PassFail[y] == 5)
                {
                    printf("RandomTestCompleted\n");
                    fprintf(stderr, "RandomTestCompleted\n");
                }

                if (PassFail[y] == 8)
                {
                    printf("MinTestPassed\n");
                    fprintf(stderr, "MinTestPassed\n");
                }

                if (PassFail[y] == 10)
                {
                    printf("CompressionMade.\n");
                    fprintf(stderr, "CompressionMade\n");
                }

                if (PassFail[y] == 11)
                {
                    printf("ErrorFileMismatch\n");
                    fprintf(stderr, "ErrorFileMismatch\n");
                }

                if (PassFail[y] == 12)
                {
                    printf("TestNotSupported\n");
                    fprintf(stderr, "TestNotSupported\n");
                }

            }

            y++;
        }

        if (TestIndicator == 0)
        {
            printf("NONE\n\n");
            fprintf(stderr, "NONE\n\n");
        }
    }

    fclose(fp);
    delete [] PassFail;
    return 0;
}
//-------------------------------------------Orig VP8 Tests----------------------------------------------------------------
int AllowDF(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "Allow Drop Frames";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  AllowDF \n\n"
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

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "AllowDF";
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
    WorkingDir4.append("AllowDFOnOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("AllowDFOffOutput.ivf");

    char AllowDFon[255];
    char AllowDFoff[255];

    snprintf(AllowDFon, 255, "%s", WorkingDir4.c_str());
    snprintf(AllowDFoff, 255, "%s", WorkingDir5.c_str());

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


    printf("Allow DF test\n");
    fprintf(stderr, "Allow DF test\n");

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

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 = Run Test Compressions

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
            opt.AllowDF = 0;

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowDF = 1;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            cout << "test\n";
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.AllowDF = 0;

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AllowDF = 1;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.AllowDF = 0;

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowDF = 1;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 4)
        {
            opt.AllowDF = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowDF = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.AllowDF = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowDF = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1) == -1)
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

    long File1bytes = FileSize(AllowDFon);
    long File2bytes = FileSize(AllowDFoff);

    if (File1bytes < File2bytes)
    {
        printf("\n\n DF on file size:%i < DF off file size:%i : Passed \n", File1bytes, File2bytes);
        fprintf(stderr, "\n\n DF on file size:%i < DF off file size:%i : Passed \n", File1bytes, File2bytes);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (File1bytes == File2bytes)
    {
        printf("\n\n DF on file size:%i = DF off file size:%i : No effect  \n", File1bytes, File2bytes);
        fprintf(stderr, "\n\n DF on file size:%i = DF off file size:%i : No effect  \n", File1bytes, File2bytes);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (File1bytes > File2bytes)
    {
        printf("\n\n DF on file size:%i > DF off file size:%i : Failed \n", File1bytes, File2bytes);
        fprintf(stderr, "\n\n DF on file size:%i > DF off file size:%i : Failed\n", File1bytes, File2bytes);
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int AllowSpatialResamplingTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "AllowSpatialResampling";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  AllowSpatialResampling \n\n"
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
    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    char *MyDir = "AllowSpatialResampling";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
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
    WorkingDir4.append("SpatialOnOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("SpatialOffOutput.ivf");

    char Spatialon[255];
    char Spatialoff[255];

    snprintf(Spatialon, 255, "%s", WorkingDir4.c_str());
    snprintf(Spatialoff, 255, "%s", WorkingDir5.c_str());

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

    char *input = argv[2];

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);

    printf("Spatial Resample test");
    fprintf(stderr, "Spatial Resample test");

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
    opt.ResampleDownWaterMark = 60;
    opt.ResampleUpWaterMark = 80;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.AllowSpatialResampling = 0;

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.AllowSpatialResampling = 0;

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.AllowSpatialResampling = 0;

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.AllowSpatialResampling = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowSpatialResampling = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.AllowSpatialResampling = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowSpatialResampling = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1) == -1)
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


    long File1bytes = FileSize(Spatialon);
    long File2bytes = FileSize(Spatialoff);

    if (File1bytes < File2bytes)
    {
        printf("\n\n Spacial Resampling on: %i is Smaller than Spacial Resampling off: %i: Passed \n", File1bytes, File2bytes);
        fprintf(stderr, "\n\n Spacial Resampling on: %i is Smaller than Spacial Resampling off: %i: Passed \n", File1bytes, File2bytes);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (File1bytes == File2bytes)
    {
        printf("\n\nFile 1: %i = File 2: %i - No effect try different file \n", File1bytes, File2bytes);
        fprintf(stderr, "\n\nFile 1: %i = File 2: %i - No effect try different file \n", File1bytes, File2bytes);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    printf("\n\n Spacial Resampling off: %i is Smaller than Spacial Resampling on: %i: Failed \n", File1bytes, File2bytes);
    fprintf(stderr, "\n\n Spacial Resampling off: %i is Smaller than Spacial Resampling on: %i: Failed \n", File1bytes, File2bytes);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
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

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1) == -1)
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

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1) == -1)
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

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1) == -1)
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

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1) == -1)
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

            if (CompressIVFtoIVF(input, SpatialResamp2OutFile, speed, BitRate, opt, CompressString, 1) == -1)
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

int AutoKeyFramingWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "opt.AutoKeyFrame";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  AutoKeyFramingWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "	 <Optional Settings File>\n"
            "\n");
        return 0;
    }

    int speed = 0;
    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int AutoKeyFramingInt = 1;

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "AutoKeyFramingWorks";
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
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("AutoKeyFramingWorksOutPut1.txt");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("AutoKeyFramingWorksOutput1.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("AutoKeyFramingWorksWorksOutPut2.txt");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("AutoKeyFramingWorksOutput2.ivf");

    char outputfile1[255];
    char outputfile2[255];
    char AutoKeyFramingWorks1[255];
    char AutoKeyFramingWorks2[255];

    snprintf(outputfile1, 255, "%s", WorkingDir4.c_str());
    snprintf(outputfile2, 255, "%s", WorkingDir6.c_str());
    snprintf(AutoKeyFramingWorks1, 255, "%s", WorkingDir5.c_str());
    snprintf(AutoKeyFramingWorks2, 255, "%s", WorkingDir7.c_str());

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

    printf("AutoKeyFramingWorks test\n");
    fprintf(stderr, "AutoKeyFramingWorks test\n");

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
    opt.AutoKey = 1;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt) == -1)
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

    IVFDisplayKeyFrames(AutoKeyFramingWorks1, 1, outputfile1);
    IVFDisplayKeyFrames(AutoKeyFramingWorks2, 1, outputfile2);

    ifstream infile1(outputfile1);
    ifstream infile2(outputfile2);

    if (!infile1.good())
    {
        printf("\nKey Frame File Not Present\n");
        fprintf(stderr, "\nKey Frame File Not Present\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (!infile2.good())
    {
        printf("\nKey Frame File Not Present\n");
        fprintf(stderr, "\nKey Frame File Not Present\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }


    int x;
    int y;

    while (!infile1.eof() && !infile2.eof())
    {
        infile1 >> x;
        infile2 >> y;

        if (x != y)
        {
            printf("\n\n Auto Key Frame Files not identical: Failed \n");
            fprintf(stderr, "\n\n Auto Key Frame Files not identical: Failed \n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

    }


    if (!infile1.eof())
    {
        printf("\n\n Auto Key Frame Files not identical: Failed \n");
        fprintf(stderr, "\n\n Auto Key Frame Files not identical: Failed \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (!infile2.eof())
    {
        printf("\n\n Auto Key Frame Files not identical: Failed \n");
        fprintf(stderr, "\n\n Auto Key Frame Files not identical: Failed \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    printf("\n\n Auto Key Frame Files are identical: Passed \n");
    fprintf(stderr, "\n\n Auto Key Frame Files are identical: Passed \n");

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 1;
}
int DFWM(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "DropFramesWaterMark";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  DFWMTest \n\n"
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
    char *MyDir = "DropFramesWaterMark";
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
    WorkingDir4.append("DFWMOutput");

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


    printf("DMFW Test\n");
    fprintf(stderr, "DMFW Test\n");


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
    /////Newly added//////
    opt.WorstAllowedQ = 15;
    opt.AllowDF = 1;
    opt.AllowSpatialResampling = 0;
    //////////////////////

    int n = 100;
    int PSNRToggle = 0;

    int i = 0;
    long DMFW[6];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        while (n >= 0)
        {
            opt.DropFramesWaterMark = n;
            //update file name
            char num[20];
            itoa_custom(n, num, 10);

            string WorkingDir5 = WorkingDir4;
            WorkingDir5.append(num);
            WorkingDir5.append(".ivf");

            char DFWMOutFile[255];
            snprintf(DFWMOutFile, 255, "%s", WorkingDir5.c_str());

            DMFW[i] = FileSize(DFWMOutFile);

            i++;
            n = n - 20;

            WorkingDir5.clear();
        }
    }
    else
    {
        while (n >= 0)
        {
            opt.DropFramesWaterMark = n;
            //update file name
            char num[20];
            itoa_custom(n, num, 10);

            string WorkingDir5 = WorkingDir4;
            WorkingDir5.append(num);
            WorkingDir5.append(".ivf");

            char DFWMOutFile[255];
            snprintf(DFWMOutFile, 255, "%s", WorkingDir5.c_str());

            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (TestType != 2)
                DMFW[i] = FileSize(DFWMOutFile);

            i++;
            n = n - 20;

            WorkingDir5.clear();
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

    n = 0;
    i = 100;
    int EqualBool = 0;
    int testBool = 1;
    cout << "\n";
    cerr << "\n";

    while (n < 6)
    {
        cout << "DFWM" << i << " Size " << DMFW[n] << "\n";
        cerr << "DFWM" << i << " Size " << DMFW[n] << "\n";
        n++;
        i = i - 20;
    }

    i = 100;
    n = 0;
    cout << "\n";
    cerr << "\n";

    while (n < 5)
    {
        if (DMFW[n+1] == DMFW[n])
        {
            EqualBool++;

            if (i == 100)
            {
                cout << "DFWM" << i - 20 << ": " << DMFW[n+1] << " = " << "DFWM" << i << ": " << DMFW[n] << "\n";
                cerr << "DFWM" << i - 20 << ": " << DMFW[n+1] << " = " << "DFWM" << i << ": " << DMFW[n] << "\n";
            }
            else
            {
                cout << "DFWM" << i - 20 << ": " << DMFW[n+1] << " =  " << "DFWM" << i << ": " << DMFW[n] << "\n";
                cerr << "DFWM" << i - 20 << ": " << DMFW[n+1] << " =  " << "DFWM" << i << ": " << DMFW[n] << "\n";
            }
        }

        if (DMFW[n+1] > DMFW[n])
        {
            if (i == 100)
            {
                cout << "DFWM" << i - 20 << ": " << DMFW[n+1] << " > " << "DFWM" << i << ": " << DMFW[n] << "\n";
                cerr << "DFWM" << i - 20 << ": " << DMFW[n+1] << " > " << "DFWM" << i << ": " << DMFW[n] << "\n";
            }
            else
            {
                cout << "DFWM" << i - 20 << ": " << DMFW[n+1] << " >  " << "DFWM" << i << ": " << DMFW[n] << "\n";
                cerr << "DFWM" << i - 20 << ": " << DMFW[n+1] << " >  " << "DFWM" << i << ": " << DMFW[n] << "\n";
            }
        }

        if (DMFW[n+1] < DMFW[n])
        {
            testBool = 0;

            if (i == 100)
            {
                cout << "DFWM" << i - 20 << ": " << DMFW[n+1] << " < " << "DFWM" << i << ": " << DMFW[n] << " - Fail \n";
                cerr << "DFWM" << i - 20 << ": " << DMFW[n+1] << " < " << "DFWM" << i << ": " << DMFW[n] << " - Fail \n";
            }
            else
            {
                cout << "DFWM" << i - 20 << ": " << DMFW[n+1] << " <  " << "DFWM" << i << ": " << DMFW[n] << " - Fail \n";
                cerr << "DFWM" << i - 20 << ": " << DMFW[n+1] << " <  " << "DFWM" << i << ": " << DMFW[n] << " - Fail \n";
            }

        }

        n++;
        i = i - 20;
    }


    if (testBool == 0)
    {
        printf("\n\nTest Failed \n");
        fprintf(stderr, "\n\nTest Failed \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (testBool == 1)
    {
        if (EqualBool == 5)
        {
            printf("\n\nUnknown: Drop-Frames-Watermark has no effect, try different parameters \n");
            fprintf(stderr, "\n\nUnknown: Drop-Frames-Watermark has no effect, try different parameters \n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        printf("\n\nPassed \n");
        fprintf(stderr, "\n\nPassed \n");
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 1;
}

int DataRateTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "AllowDF";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  DataRateTest \n\n"
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
    char *MyDir = "DataRateTest";
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
    WorkingDir4.append("TargetBitRateOutput.ivf");

    char TargetBitRate1[255];

    snprintf(TargetBitRate1, 255, "%s", WorkingDir4.c_str());

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

    printf("Data Rate Test\n");
    fprintf(stderr, "Data Rate Test\n");


    int speed = 0;

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    double BitRate = atoi(argv[4]);

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

    ////////////////Data Rate Specific Settings////////////////
    opt.BestAllowedQ = 0;
    opt.WorstAllowedQ = 63;
    opt.AllowDF = 1;
    ///////////////////////////////////////////////////////////

    int CompressInt = opt.AllowDF;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

    //need to clean this up
    int num = 3;
    char *in2;
    in2 = &TargetBitRate1[0];


    printf("\n\n");
    fprintf(stderr, "\n\n");
    double FileDataRate = IVFDataRate(TargetBitRate1, 1);
    double DataRateProx = absDouble(100 - absDouble(((FileDataRate * 100) / BitRate)));

    if (DataRateProx < 30)
    {
        if (FileDataRate < BitRate)
        {
            printf("\nDataRate: %4.2f is %4.2f%% lower than Target\n\nDataRate is within 30%% of: %4.2f:Passed \n", FileDataRate, DataRateProx, BitRate);
            fprintf(stderr, "\nDataRate: %4.2f is %4.2f%% lower than Target\n\nDataRate is within 30%% of: %4.2f:Passed \n", FileDataRate, DataRateProx, BitRate);
        }
        else
        {
            printf("\nDataRate: %4.2f is %4.2f%% greater than Target\n\nDataRate is within 30%% of: %4.2f:Passed \n", FileDataRate, DataRateProx, BitRate);
            fprintf(stderr, "\nDataRate: %4.2f is %4.2f%% greater than Target\n\nDataRate is within 30%% of: %4.2f:Passed \n", FileDataRate, DataRateProx, BitRate);
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (FileDataRate < BitRate)
    {
        printf("\nDataRate: %4.2f is %4.2f%% less than Target\n\nDataRate not within 30%% of: %4.2f:Failed \n", FileDataRate, DataRateProx, BitRate);
        fprintf(stderr, "\nDataRate: %4.2f is %4.2f%% less than Target\n\nDataRate not within 30%% of: %4.2f:Failed \n", FileDataRate, DataRateProx, BitRate);
    }
    else
    {
        printf("\nDataRate: %4.2f is %4.2f%% greater than Target\n\nDataRate not within 30%% of: %4.2f:Failed \n", FileDataRate, DataRateProx, BitRate);
        fprintf(stderr, "\nDataRate: %4.2f is %4.2f%% greater than Target\n\nDataRate not within 30%% of: %4.2f:Failed \n", FileDataRate, DataRateProx, BitRate);
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

int DebugMatchesRelease(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    // So long as Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>
    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  DebugMatchesRelease \n\n"
            "    <inputfile>\n"
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
            "	 <Optional Settings File>\n"
            "\n");
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char *ExeInputDebug = argv[5];
    char ExeInputRelease[255];

#if defined(_WIN32)
    {
        snprintf(ExeInputRelease, 255, "%s", argv[6]);
    }
#elif defined(linux)
    {
        string ExeInputReleaseStr = argv[6];
        snprintf(ExeInputRelease, 255, "%s", ExeInputReleaseStr.c_str());
    }
#elif defined(__APPLE__)
    {
        string ExeInputReleaseStr = argv[6];
        snprintf(ExeInputRelease, 255, "%s", ExeInputReleaseStr.c_str());
    }
#elif defined(__POWERPC__)
    {
        string ExeInputReleaseStr = argv[6];
        snprintf(ExeInputRelease, 255, "%s", ExeInputReleaseStr.c_str());
    }
#endif
    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "DebugMatchesRelease";
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

    char ExeCharDebugRelease[1024];
    FolderName(argv[0], ExeCharDebugRelease);
    string ExeCharDebugReleaseString = ExeCharDebugRelease;

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = WorkingDirString;
    string WorkingDir9 = ExeCharDebugReleaseString;
    string WorkingDir10 = ExeCharDebugReleaseString;


    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("output_Debug.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("output_Release.ivf");

    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("ParFileDebug.txt");
    WorkingDir8.append(slashCharStr);
    WorkingDir8.append("ParFileRelease.txt");

#if defined(_WIN32)
    {
        WorkingDir9.append(ExeInputRelease);
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);
        WorkingDir9.append("\" 8");
        WorkingDir9.append(" \"");
        WorkingDir9.append(WorkingDir7);
        WorkingDir9.append("\"");
        WorkingDir9.append(" 0");
        WorkingDir9.append("\"");
        WorkingDir10.append(ExeInputDebug);
        WorkingDir10.append("\" \"");
        WorkingDir10.append(input);
        WorkingDir10.append("\" \"");
        WorkingDir10.append(WorkingDir5);
        WorkingDir10.append("\" 8");
        WorkingDir10.append(" \"");
        WorkingDir10.append(WorkingDir8);
        WorkingDir10.append("\"");
        WorkingDir10.append(" 0");
        WorkingDir10.append("\"");
    }
#elif defined(linux)
    {
        WorkingDir9.append(ExeInputRelease);
        WorkingDir9.append("\' \'");
        WorkingDir9.append(input);
        WorkingDir9.append("\' \'");
        WorkingDir9.append(WorkingDir4);
        WorkingDir9.append("\' 8");
        WorkingDir9.append(" \'");
        WorkingDir9.append(WorkingDir7);
        WorkingDir9.append("\'");
        WorkingDir9.append(" 0");
        WorkingDir10.append(ExeInputDebug);
        WorkingDir10.append("\' \'");
        WorkingDir10.append(input);
        WorkingDir10.append("\' \'");
        WorkingDir10.append(WorkingDir5);
        WorkingDir10.append("\' 8");
        WorkingDir10.append(" \'");
        WorkingDir10.append(WorkingDir8);
        WorkingDir10.append("\'");
        WorkingDir10.append(" 0");

    }
#elif defined(__APPLE__)
    {
        WorkingDir9.append(ExeInputRelease);
        WorkingDir9.append("\' \'");
        WorkingDir9.append(input);
        WorkingDir9.append("\' \'");
        WorkingDir9.append(WorkingDir4);
        WorkingDir9.append("\' 8");
        WorkingDir9.append(" \'");
        WorkingDir9.append(WorkingDir7);
        WorkingDir9.append("\'");
        WorkingDir9.append(" 0");
        WorkingDir10.append(ExeInputDebug);
        WorkingDir10.append("\' \'");
        WorkingDir10.append(input);
        WorkingDir10.append("\' \'");
        WorkingDir10.append(WorkingDir5);
        WorkingDir10.append("\' 8");
        WorkingDir10.append(" \'");
        WorkingDir10.append(WorkingDir8);
        WorkingDir10.append("\'");
        WorkingDir10.append(" 0");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir9.append(ExeInputRelease);
        WorkingDir9.append("\' \'");
        WorkingDir9.append(input);
        WorkingDir9.append("\' \'");
        WorkingDir9.append(WorkingDir4);
        WorkingDir9.append("\' 8");
        WorkingDir9.append(" \'");
        WorkingDir9.append(WorkingDir7);
        WorkingDir9.append("\'");
        WorkingDir9.append(" 0");
        WorkingDir10.append(ExeInputDebug);
        WorkingDir10.append("\' \'");
        WorkingDir10.append(input);
        WorkingDir10.append("\' \'");
        WorkingDir10.append(WorkingDir5);
        WorkingDir10.append("\' 8");
        WorkingDir10.append(" \'");
        WorkingDir10.append(WorkingDir8);
        WorkingDir10.append("\'");
        WorkingDir10.append(" 0");
    }
#endif

    char outputVP7Debug[255];
    char outputVP7Release[255];

    char ParFileDebug[255];
    char ParFileRelease[255];

    char ProgramDebug[1024];
    char ProgramRelease[1024];

    snprintf(outputVP7Debug, 255, "%s", WorkingDir4.c_str());
    snprintf(outputVP7Release, 255, "%s", WorkingDir5.c_str());

    snprintf(ParFileDebug, 255, "%s", WorkingDir7.c_str());
    snprintf(ParFileRelease, 255, "%s", WorkingDir8.c_str());

#if defined(_WIN32)
    {
        WorkingDir9.insert(0, "\"\"");
        WorkingDir10.insert(0, "\"\"");
    }
#elif defined(linux)
    {
        WorkingDir9.insert(0, "\'");
        WorkingDir10.insert(0, "\'");
    }
#elif defined(__APPLE__)
    {
        WorkingDir9.insert(0, "\'");
        WorkingDir10.insert(0, "\'");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir9.insert(0, "\'");
        WorkingDir10.insert(0, "\'");
    }
#endif

    snprintf(ProgramDebug, 1024, "%s", WorkingDir9.c_str());
    snprintf(ProgramRelease, 1024, "%s", WorkingDir10.c_str());

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
        printf("Cannot open out put file2.\n");
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

    fclose(fp);

    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
    {
        printf("Cannot open out put file3.\n");
        exit(1);
    }

    printf("DebugMatchesRelease\n");
    fprintf(stderr, "DebugMatchesRelease\n");

    int speed = 0;

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

    cout << "\n\n" << ProgramDebug << "\n\n";
    cout << "\n\n" << ParFileRelease << "\n\n";

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            OutPutSettings(ParFileDebug, opt);

            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");
            RunExe(ProgramRelease);
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            OutPutSettings(ParFileDebug, opt);

            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file5.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(ProgramRelease);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            OutPutSettings(ParFileDebug, opt);

            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file6.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(ProgramRelease);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            OutPutSettings(ParFileDebug, opt);
            RunExe(ProgramDebug);

            opt.Mode = MODE_SECONDPASS;
            OutPutSettings(ParFileRelease, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file9.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(ProgramRelease);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            OutPutSettings(ParFileDebug, opt);
            RunExe(ProgramDebug);

            opt.Mode = MODE_SECONDPASS_BEST;
            OutPutSettings(ParFileRelease, opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file11.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(ProgramRelease);

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

    printf("\n\nComparing Files: ");
    fprintf(stderr, "\n\nComparing Files: ");

    int lngRC = CompIVF(outputVP7Release, outputVP7Debug);

    if (lngRC >= 0)
    {
        printf("Files differ at frame: %i - Fail", lngRC);
        fprintf(stderr, "Files differ at frame: %i - Fail", lngRC);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (lngRC == -1)
    {
        printf("Files are identical - Pass\n");
        fprintf(stderr, "Files are identical - Pass\n");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (lngRC == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        return 0;
    }

    if (lngRC == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

int ErrorRes(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "ErrorResilientMode";
    char *input = argv[2];

    if (argc != 5)
    {
        printf(
            "  ErrorRes \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n ");

        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "ErrorRes";
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
    WorkingDir4.append("ErrorOnOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("ErrorOffOutput.ivf");

    char ErrorOnOutFile[255];
    char ErrorOffOutFile[255];

    snprintf(ErrorOnOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(ErrorOffOutFile, 255, "%s", WorkingDir5.c_str());

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


    printf("Error Ressiliancy Test\n");
    fprintf(stderr, "Error Ressiliancy Test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    opt.TargetBandwidth = BitRate;

    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.ErrorResilientMode = 1;

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.ErrorResilientMode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.ErrorResilientMode = 1;

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.ErrorResilientMode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.ErrorResilientMode = 1;

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.ErrorResilientMode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.ErrorResilientMode = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.ErrorResilientMode = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.ErrorResilientMode = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.ErrorResilientMode = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1) == -1)
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

    int PSNRToggle = PSNRSelect(input, ErrorOnOutFile);

    printf("\n");
    fprintf(stderr, "\n");

    double PSNRon;
    double PSNRoff;

    double ssim = 0;
    PSNRon = IVFPSNR(input, ErrorOnOutFile, PSNRToggle, 0, 1, ssim);
    PSNRoff = IVFPSNR(input, ErrorOffOutFile, PSNRToggle, 0, 1, ssim);

    float PSRNPerc = 100 * absFloat((PSNRon - PSNRoff) / PSNRoff);

    if (PSRNPerc < 10.00)
    {
        printf("\nPSNR of ErrorRes on is with in 10%% of PSNR Error Res off: %f%% - Passed\n", PSRNPerc);
        fprintf(stderr, "\nPSNR of ErrorRes on is with in 10%% of PSNR Error Res off: %f%% - Passed\n", PSRNPerc);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    printf("\nPSNR of ErrorRes on is not with in 10%% of PSNR Error Res off: %f%% - Failed\n", PSRNPerc);
    fprintf(stderr, "\nPSNR of ErrorRes on is not with in 10%% of PSNR Error Res off: %f%% - Failed\n", PSRNPerc);
    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);

    return 0;
}



int ExtraFileCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
#if defined(linux)
    {
        char *CompressString = "opt.DeleteFirstPassFile";

        if (!(argc == 3 || argc == 4))
        {
            printf(
                "  ExtraFileCheck \n\n"
                "    <inputfile>\n"
                "	 <Optional Settings File>\n"
                "\n"
            );
            return 0;
        }

        ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
        string WorkingDirString = "";
        char WorkingDir2[255] = "";
        char WorkingDir3[255] = "";
        char *MyDir = "Extra File Check Test";
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

            WorkingDir2.append(buffer);
            WorkingDirString = WorkingDir2;
        }

        string ExtraFileDir = WorkingDirString;
        ////////////////////OutPut Directory location////////////////////
        char EF_DestChar[255];
        snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
        string EF_DestString = EF_DestChar;
        //////////////////////////////////////////////////////////////////
        //////////////////Program Location///////////////////////////////
        char EF_SourChar[255];
        char EF_SourChar2[255];
        FolderName(argv[0], EF_SourChar2);
        snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
        string EF_SourString1 = EF_SourChar;
        EF_SourString1.erase(EF_SourString1.length() - 1, 1);
        ////////////////////////////////////////////////////////////////
        //////////////////Current Dir Locatoin//////////////////////////
        char EF_SourChar3[256];
        getcwd(EF_SourChar3, 256);

        string EF_SourString2 = EF_SourChar3;
        ////////////////////////////////////////////////////////////////

        string WorkingDir4 = WorkingDirString;
        string WorkingDir5 = WorkingDirString;

        WorkingDir4.append(slashCharStr);
        WorkingDir4.append("ExtraFileCheck.ivf");

        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("ExtraFileCheckResult.txt");

        char ExtraFileCheck[255];
        char ExtraFileCheckResult[255];

        snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
        snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

        printf("Extra File Check\n");
        fprintf(stderr, "Extra File Check\n");

        printf("---------------------------Checking for OPSNR Files----------------------------\n");
        fprintf(stderr, "---------------------------Checking for OPSNR Files----------------------------\n");

        /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
        char CurrentPath[260];
        getcwd(CurrentPath, 260);

        string CurrentPathStr = CurrentPath;

        cout << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";

        CurrentPathStr.append(slashCharStr);
        CurrentPathStr.append("opsnr.stt");

        FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

        if (opsnrExists == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";

        }
        else
        {
            fclose(opsnrExists);
            remove(CurrentPathStr.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        string EF_DestString2 = EF_DestChar;

        cout << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";

        EF_DestString2.append(slashCharStr);
        EF_DestString2.append("opsnr.stt");

        FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

        if (opsnrExists2 == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";
        }
        else
        {
            fclose(opsnrExists2);
            remove(EF_DestString2.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        printf("-------------------------------------------------------------------------------\n");
        fprintf(stderr, "-------------------------------------------------------------------------------\n");
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        char *input = argv[2];

        VP8_CONFIG opt;
        VP8DefaultParms(opt);

        ///////////////////Use Custom Settings///////////////////
        if (argc == 4)
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
        }

        /////////////////////////////////////////////////////////

        int CompressInt = opt.AllowDF;
        int BitRate = opt.TargetBandwidth;
        int Fail = 0;

        ///////////////////////////Record all files in the executable + Current directory///////////////////////////
        //Record all files in the executable directory.
        DIR *FindFileDataA;
        struct dirent *hFindA;
        string FileNameA;
        vector<string> SourceFileVectorA;

        FindFileDataA = opendir(EF_SourString1.c_str());
        hFindA = readdir(FindFileDataA);

        while (FileNameA.compare(hFindA->d_name) != 0)
        {
            FileNameA = hFindA->d_name;
            SourceFileVectorA.push_back(FileNameA);
            hFindA = readdir(FindFileDataA);

            if (hFindA == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataA);

        //Record all files in the current directory.
        DIR *FindFileDataB;
        struct dirent *hFindB;
        string FileNameB;
        vector<string> SourceFileVectorB;

        FindFileDataB = opendir(EF_SourString2.c_str());
        hFindB = readdir(FindFileDataB);

        while (FileNameB.compare(hFindB->d_name) != 0)
        {
            FileNameB = hFindB->d_name;
            SourceFileVectorB.push_back(FileNameB);
            hFindB = readdir(FindFileDataB);

            if (hFindB == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataB);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            printf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");
            fprintf(stderr, "\nExtraFileCheck Test Run Previously Retreiveing Result\n");

            ifstream infile(ExtraFileCheckResult);

            if (!infile.is_open())
            {
                printf("File: %s not opened", ExtraFileCheckResult);
                fprintf(stderr, "File: %s not opened", ExtraFileCheckResult);
                return 0;
            }

            infile >> Fail;
            infile.close();

            if (Fail == 1)
            {
                cout << "\n\nExtra files found - Test Failed\n";
                cerr << "\n\nExtra files found - Test Failed\n";
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 0;
            }

            if (Fail == 0)
            {
                cout << "\n\n No extra files found - Test Passed\n";
                cerr << "\n\n No extra files found - Test Passed\n";
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 1;
            }
        }
        else
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 0/*opt.DeleteFirstPassFile*/) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
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

        printf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");
        fprintf(stderr, "\n\n---------------------------Checking for Extra Files----------------------------\n\n");

        cout << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";
        cerr << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";

        cout << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";
        cerr << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";

        ///////////////////////////Check all files in the executable + Current directory///////////////////////////
        //Check all files in the output directory.
        DIR *FindFileData2;
        struct dirent *hFind2;

        FindFileData2 = opendir(EF_DestString.c_str());
        hFind2 = readdir(FindFileData2);

        string FileName2;
        vector<string> DestFileExtraVector;

        while (FileName2.compare(hFind2->d_name) != 0)
        {
            FileName2 = hFind2->d_name;

            if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
                FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("Extra File Check Test.txt")
                && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
                && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            if (FileName2.compare("opsnr.stt") == 0)
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            hFind2 = readdir(FindFileData2);

            if (hFind2 == NULL)
            {
                break;
            }
        }

        closedir(FindFileData2);

        //Check through all files in the executable directory.
        DIR *FindFileData3;
        struct dirent *hFind3;
        string FileName3;

        int filenum = 0;
        vector<int> FileFoundVect;
        vector<string> PostCompressionFileListVect;

        FindFileData3 = opendir(EF_SourString1.c_str());
        hFind3 = readdir(FindFileData3);

        while (FileName3.compare(hFind3->d_name) != 0)
        {
            FileName3 = hFind3->d_name;

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorA.begin();

            while (it < SourceFileVectorA.end())
            {
                //checks Exe directory for opsnr.stt file
                if (FileName3.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName3.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVect.push_back(FileFound);
            PostCompressionFileListVect.push_back(FileName3);

            hFind3 = readdir(FindFileData3);

            if (hFind3 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2;
        it2 = FileFoundVect.begin();
        int vectorposition = 0;

        while (it2 < FileFoundVect.end())
        {
            if (FileFoundVect[vectorposition] == 0)
            {
                cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
                cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2++;
            vectorposition++;
        }

        closedir(FindFileData3);

        //Check through all files in the current directory.
        DIR *FindFileData4;
        struct dirent *hFind4;
        string FileName4;

        int filenumB = 0;
        vector<int> FileFoundVectB;
        vector<string> PostCompressionFileListVectB;

        FindFileData4 = opendir(EF_SourString2.c_str());
        hFind4 = readdir(FindFileData4);

        while (FileName4.compare(hFind4->d_name) != 0)
        {
            FileName4 = hFind4->d_name;
            //puts(hFind4->d_name);

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorB.begin();

            while (it < SourceFileVectorB.end())
            {
                //checks current directory for opsnr.stt file
                if (FileName4.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName4.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVectB.push_back(FileFound);
            PostCompressionFileListVectB.push_back(FileName4);

            hFind4 = readdir(FindFileData4);

            if (hFind4 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2B;
        it2B = FileFoundVectB.begin();
        int vectorpositionB = 0;


        cout << "\n";
        cerr << "\n";

        while (it2B < FileFoundVectB.end())
        {
            if (FileFoundVectB[vectorpositionB] == 0)
            {
                cout << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];
                cerr << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2B++;
            vectorpositionB++;
        }

        closedir(FindFileData4);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        cout << "\n";
        cerr << "\n";

        if (Fail == 2)
        {
            vector<string>::iterator it3;
            it3 = DestFileExtraVector.begin();
            int Varloc = 0;

            while (it3 < DestFileExtraVector.end())
            {
                cout << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                cerr << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                Varloc++;
                it3++;
            }

            Fail = 1;
        }

        if (Fail == 1)
        {
            printf("\n-------------------------------------------------------------------------------\n");
            fprintf(stderr, "\n-------------------------------------------------------------------------------\n");

            cout << "\nExtra files found - Test Failed\n";
            cerr << "\nExtra files found - Test Failed\n";

            if (TestType == 2)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 1;
                outfile.close();

                return 10;
            }

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            printf("-------------------------------------------------------------------------------\n");
            fprintf(stderr, "-------------------------------------------------------------------------------\n");

            cout << "\n No extra files found - Test Passed\n";
            cerr << "\n No extra files found - Test Passed\n";

            if (TestType == 2)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 0;
                outfile.close();

                return 10;
            }

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
    }
#endif
#if defined(__APPLE__)
    {
        char *CompressString = "opt.DeleteFirstPassFile";

        if (!(argc == 3 || argc == 4))
        {
            printf(
                "  ExtraFileCheck \n\n"
                "    <inputfile>\n"
                "	 <Optional Settings File>\n"
                "\n"
            );
            return 0;
        }

        ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
        string WorkingDirString = "";
        char WorkingDir2[255] = "";
        char WorkingDir3[255] = "";
        char *MyDir = "Extra File Check Test";
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

            WorkingDir2.append(buffer);
            WorkingDirString = WorkingDir2;
        }

        string ExtraFileDir = WorkingDirString;
        ////////////////////OutPut Directory location////////////////////
        char EF_DestChar[255];
        snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
        string EF_DestString = EF_DestChar;
        //////////////////////////////////////////////////////////////////
        //////////////////Program Location///////////////////////////////
        char EF_SourChar[255];
        char EF_SourChar2[255];
        FolderName(argv[0], EF_SourChar2);
        snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
        string EF_SourString1 = EF_SourChar;
        EF_SourString1.erase(EF_SourString1.length() - 1, 1);
        ////////////////////////////////////////////////////////////////
        //////////////////Current Dir Locatoin//////////////////////////
        char EF_SourChar3[256];
        getcwd(EF_SourChar3, 256);

        string EF_SourString2 = EF_SourChar3;
        ////////////////////////////////////////////////////////////////

        string WorkingDir4 = WorkingDirString;
        string WorkingDir5 = WorkingDirString;

        WorkingDir4.append(slashCharStr);
        WorkingDir4.append("ExtraFileCheck.ivf");

        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("ExtraFileCheckResult.txt");

        char ExtraFileCheck[255];
        char ExtraFileCheckResult[255];

        snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
        snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

        printf("Extra File Check\n");
        fprintf(stderr, "Extra File Check\n");

        printf("---------------------------Checking for OPSNR Files----------------------------\n");
        fprintf(stderr, "---------------------------Checking for OPSNR Files----------------------------\n");

        /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
        char CurrentPath[260];
        getcwd(CurrentPath, 260);

        string CurrentPathStr = CurrentPath;

        cout << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";

        CurrentPathStr.append(slashCharStr);
        CurrentPathStr.append("opsnr.stt");

        FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

        if (opsnrExists == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";

        }
        else
        {
            fclose(opsnrExists);
            remove(CurrentPathStr.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        string EF_DestString2 = EF_DestChar;

        cout << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";

        EF_DestString2.append(slashCharStr);
        EF_DestString2.append("opsnr.stt");

        FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

        if (opsnrExists2 == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";
        }
        else
        {
            fclose(opsnrExists2);
            remove(EF_DestString2.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        printf("-------------------------------------------------------------------------------\n");
        fprintf(stderr, "-------------------------------------------------------------------------------\n");
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        char *input = argv[2];

        VP8_CONFIG opt;
        VP8DefaultParms(opt);

        ///////////////////Use Custom Settings///////////////////
        if (argc == 4)
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
        }

        /////////////////////////////////////////////////////////

        int CompressInt = opt.AllowDF;
        int BitRate = opt.TargetBandwidth;
        int Fail = 0;

        ///////////////////////////Record all files in the executable + Current directory///////////////////////////
        //Record all files in the executable directory.
        DIR *FindFileDataA;
        struct dirent *hFindA;
        string FileNameA;
        vector<string> SourceFileVectorA;

        FindFileDataA = opendir(EF_SourString1.c_str());
        hFindA = readdir(FindFileDataA);

        while (FileNameA.compare(hFindA->d_name) != 0)
        {
            FileNameA = hFindA->d_name;
            SourceFileVectorA.push_back(FileNameA);
            hFindA = readdir(FindFileDataA);

            if (hFindA == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataA);

        //Record all files in the current directory.
        DIR *FindFileDataB;
        struct dirent *hFindB;
        string FileNameB;
        vector<string> SourceFileVectorB;

        FindFileDataB = opendir(EF_SourString2.c_str());
        hFindB = readdir(FindFileDataB);

        while (FileNameB.compare(hFindB->d_name) != 0)
        {
            FileNameB = hFindB->d_name;
            SourceFileVectorB.push_back(FileNameB);
            hFindB = readdir(FindFileDataB);

            if (hFindB == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataB);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            printf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");
            fprintf(stderr, "\nExtraFileCheck Test Run Previously Retreiveing Result\n");

            ifstream infile(ExtraFileCheckResult);

            if (!infile.is_open())
            {
                printf("File: %s not opened", ExtraFileCheckResult);
                fprintf(stderr, "File: %s not opened", ExtraFileCheckResult);
                return 0;
            }

            infile >> Fail;
            infile.close();

            if (Fail == 1)
            {
                cout << "\n\nExtra files found - Test Failed\n";
                cerr << "\n\nExtra files found - Test Failed\n";
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 0;
            }

            if (Fail == 0)
            {
                cout << "\n\n No extra files found - Test Passed\n";
                cerr << "\n\n No extra files found - Test Passed\n";
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 1;
            }
        }
        else
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 0/*opt.DeleteFirstPassFile*/) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
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

        printf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");
        fprintf(stderr, "\n\n---------------------------Checking for Extra Files----------------------------\n\n");

        cout << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";
        cerr << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";

        cout << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";
        cerr << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";

        ///////////////////////////Check all files in the executable + Current directory///////////////////////////
        //Check all files in the output directory.
        DIR *FindFileData2;
        struct dirent *hFind2;

        FindFileData2 = opendir(EF_DestString.c_str());
        hFind2 = readdir(FindFileData2);

        string FileName2;
        vector<string> DestFileExtraVector;

        while (FileName2.compare(hFind2->d_name) != 0)
        {
            FileName2 = hFind2->d_name;

            if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
                FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("Extra File Check Test.txt")
                && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
                && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            if (FileName2.compare("opsnr.stt") == 0)
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            hFind2 = readdir(FindFileData2);

            if (hFind2 == NULL)
            {
                break;
            }
        }

        closedir(FindFileData2);

        //Check through all files in the executable directory.
        DIR *FindFileData3;
        struct dirent *hFind3;
        string FileName3;

        int filenum = 0;
        vector<int> FileFoundVect;
        vector<string> PostCompressionFileListVect;

        FindFileData3 = opendir(EF_SourString1.c_str());
        hFind3 = readdir(FindFileData3);

        while (FileName3.compare(hFind3->d_name) != 0)
        {
            FileName3 = hFind3->d_name;

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorA.begin();

            while (it < SourceFileVectorA.end())
            {
                //checks Exe directory for opsnr.stt file
                if (FileName3.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName3.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVect.push_back(FileFound);
            PostCompressionFileListVect.push_back(FileName3);

            hFind3 = readdir(FindFileData3);

            if (hFind3 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2;
        it2 = FileFoundVect.begin();
        int vectorposition = 0;

        while (it2 < FileFoundVect.end())
        {
            if (FileFoundVect[vectorposition] == 0)
            {
                cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
                cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2++;
            vectorposition++;
        }

        closedir(FindFileData3);

        //Check through all files in the current directory.
        DIR *FindFileData4;
        struct dirent *hFind4;
        string FileName4;

        int filenumB = 0;
        vector<int> FileFoundVectB;
        vector<string> PostCompressionFileListVectB;

        FindFileData4 = opendir(EF_SourString2.c_str());
        hFind4 = readdir(FindFileData4);

        while (FileName4.compare(hFind4->d_name) != 0)
        {
            FileName4 = hFind4->d_name;
            //puts(hFind4->d_name);

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorB.begin();

            while (it < SourceFileVectorB.end())
            {
                //checks current directory for opsnr.stt file
                if (FileName4.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName4.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVectB.push_back(FileFound);
            PostCompressionFileListVectB.push_back(FileName4);

            hFind4 = readdir(FindFileData4);

            if (hFind4 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2B;
        it2B = FileFoundVectB.begin();
        int vectorpositionB = 0;


        cout << "\n";
        cerr << "\n";

        while (it2B < FileFoundVectB.end())
        {
            if (FileFoundVectB[vectorpositionB] == 0)
            {
                cout << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];
                cerr << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2B++;
            vectorpositionB++;
        }

        closedir(FindFileData4);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        cout << "\n";
        cerr << "\n";

        if (Fail == 2)
        {
            vector<string>::iterator it3;
            it3 = DestFileExtraVector.begin();
            int Varloc = 0;

            while (it3 < DestFileExtraVector.end())
            {
                cout << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                cerr << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                Varloc++;
                it3++;
            }

            Fail = 1;
        }

        if (Fail == 1)
        {
            printf("\n-------------------------------------------------------------------------------\n");
            fprintf(stderr, "\n-------------------------------------------------------------------------------\n");

            cout << "\nExtra files found - Test Failed\n";
            cerr << "\nExtra files found - Test Failed\n";

            if (TestType == 2)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 1;
                outfile.close();

                return 10;
            }

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            printf("-------------------------------------------------------------------------------\n");
            fprintf(stderr, "-------------------------------------------------------------------------------\n");

            cout << "\n No extra files found - Test Passed\n";
            cerr << "\n No extra files found - Test Passed\n";

            if (TestType == 2)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 0;
                outfile.close();

                return 10;
            }

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
    }
#endif
#if defined(__POWERPC__)
    {
        char *CompressString = "opt.DeleteFirstPassFile";

        if (!(argc == 3 || argc == 4))
        {
            printf(
                "  ExtraFileCheck \n\n"
                "    <inputfile>\n"
                "	 <Optional Settings File>\n"
                "\n"
            );
            return 0;
        }

        ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
        string WorkingDirString = "";
        char WorkingDir2[255] = "";
        char WorkingDir3[255] = "";
        char *MyDir = "Extra File Check Test";
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

            WorkingDir2.append(buffer);
            WorkingDirString = WorkingDir2;
        }

        string ExtraFileDir = WorkingDirString;
        ////////////////////OutPut Directory location////////////////////
        char EF_DestChar[255];
        snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
        string EF_DestString = EF_DestChar;
        //////////////////////////////////////////////////////////////////
        //////////////////Program Location///////////////////////////////
        char EF_SourChar[255];
        char EF_SourChar2[255];
        FolderName(argv[0], EF_SourChar2);
        snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
        string EF_SourString1 = EF_SourChar;
        EF_SourString1.erase(EF_SourString1.length() - 1, 1);
        ////////////////////////////////////////////////////////////////
        //////////////////Current Dir Locatoin//////////////////////////
        char EF_SourChar3[256];
        getcwd(EF_SourChar3, 256);

        string EF_SourString2 = EF_SourChar3;
        ////////////////////////////////////////////////////////////////

        string WorkingDir4 = WorkingDirString;
        string WorkingDir5 = WorkingDirString;

        WorkingDir4.append(slashCharStr);
        WorkingDir4.append("ExtraFileCheck.ivf");

        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("ExtraFileCheckResult.txt");

        char ExtraFileCheck[255];
        char ExtraFileCheckResult[255];

        snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
        snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

        printf("Extra File Check\n");
        fprintf(stderr, "Extra File Check\n");

        printf("---------------------------Checking for OPSNR Files----------------------------\n");
        fprintf(stderr, "---------------------------Checking for OPSNR Files----------------------------\n");

        /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
        char CurrentPath[260];
        getcwd(CurrentPath, 260);

        string CurrentPathStr = CurrentPath;

        cout << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";

        CurrentPathStr.append(slashCharStr);
        CurrentPathStr.append("opsnr.stt");

        FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

        if (opsnrExists == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";

        }
        else
        {
            fclose(opsnrExists);
            remove(CurrentPathStr.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        string EF_DestString2 = EF_DestChar;

        cout << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";

        EF_DestString2.append(slashCharStr);
        EF_DestString2.append("opsnr.stt");

        FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

        if (opsnrExists2 == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";
        }
        else
        {
            fclose(opsnrExists2);
            remove(EF_DestString2.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        printf("-------------------------------------------------------------------------------\n");
        fprintf(stderr, "-------------------------------------------------------------------------------\n");
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        char *input = argv[2];

        VP8_CONFIG opt;
        VP8DefaultParms(opt);

        ///////////////////Use Custom Settings///////////////////
        if (argc == 4)
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
        }

        /////////////////////////////////////////////////////////

        int CompressInt = opt.AllowDF;
        int BitRate = opt.TargetBandwidth;
        int Fail = 0;

        ///////////////////////////Record all files in the executable + Current directory///////////////////////////
        //Record all files in the executable directory.
        DIR *FindFileDataA;
        struct dirent *hFindA;
        string FileNameA;
        vector<string> SourceFileVectorA;

        FindFileDataA = opendir(EF_SourString1.c_str());
        hFindA = readdir(FindFileDataA);

        while (FileNameA.compare(hFindA->d_name) != 0)
        {
            FileNameA = hFindA->d_name;
            SourceFileVectorA.push_back(FileNameA);
            hFindA = readdir(FindFileDataA);

            if (hFindA == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataA);

        //Record all files in the current directory.
        DIR *FindFileDataB;
        struct dirent *hFindB;
        string FileNameB;
        vector<string> SourceFileVectorB;

        FindFileDataB = opendir(EF_SourString2.c_str());
        hFindB = readdir(FindFileDataB);

        while (FileNameB.compare(hFindB->d_name) != 0)
        {
            FileNameB = hFindB->d_name;
            SourceFileVectorB.push_back(FileNameB);
            hFindB = readdir(FindFileDataB);

            if (hFindB == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataB);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            printf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");
            fprintf(stderr, "\nExtraFileCheck Test Run Previously Retreiveing Result\n");

            ifstream infile(ExtraFileCheckResult);

            if (!infile.is_open())
            {
                printf("File: %s not opened", ExtraFileCheckResult);
                fprintf(stderr, "File: %s not opened", ExtraFileCheckResult);
                return 0;
            }

            infile >> Fail;
            infile.close();

            if (Fail == 1)
            {
                cout << "\n\nExtra files found - Test Failed\n";
                cerr << "\n\nExtra files found - Test Failed\n";
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 0;
            }

            if (Fail == 0)
            {
                cout << "\n\n No extra files found - Test Passed\n";
                cerr << "\n\n No extra files found - Test Passed\n";
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 1;
            }
        }
        else
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 0/*opt.DeleteFirstPassFile*/) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
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

        printf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");
        fprintf(stderr, "\n\n---------------------------Checking for Extra Files----------------------------\n\n");

        cout << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";
        cerr << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";

        cout << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";
        cerr << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";

        ///////////////////////////Check all files in the executable + Current directory///////////////////////////
        //Check all files in the output directory.
        DIR *FindFileData2;
        struct dirent *hFind2;

        FindFileData2 = opendir(EF_DestString.c_str());
        hFind2 = readdir(FindFileData2);

        string FileName2;
        vector<string> DestFileExtraVector;

        while (FileName2.compare(hFind2->d_name) != 0)
        {
            FileName2 = hFind2->d_name;

            if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
                FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("Extra File Check Test.txt")
                && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
                && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            if (FileName2.compare("opsnr.stt") == 0)
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            hFind2 = readdir(FindFileData2);

            if (hFind2 == NULL)
            {
                break;
            }
        }

        closedir(FindFileData2);

        //Check through all files in the executable directory.
        DIR *FindFileData3;
        struct dirent *hFind3;
        string FileName3;

        int filenum = 0;
        vector<int> FileFoundVect;
        vector<string> PostCompressionFileListVect;

        FindFileData3 = opendir(EF_SourString1.c_str());
        hFind3 = readdir(FindFileData3);

        while (FileName3.compare(hFind3->d_name) != 0)
        {
            FileName3 = hFind3->d_name;

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorA.begin();

            while (it < SourceFileVectorA.end())
            {
                //checks Exe directory for opsnr.stt file
                if (FileName3.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName3.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVect.push_back(FileFound);
            PostCompressionFileListVect.push_back(FileName3);

            hFind3 = readdir(FindFileData3);

            if (hFind3 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2;
        it2 = FileFoundVect.begin();
        int vectorposition = 0;

        while (it2 < FileFoundVect.end())
        {
            if (FileFoundVect[vectorposition] == 0)
            {
                cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
                cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2++;
            vectorposition++;
        }

        closedir(FindFileData3);

        //Check through all files in the current directory.
        DIR *FindFileData4;
        struct dirent *hFind4;
        string FileName4;

        int filenumB = 0;
        vector<int> FileFoundVectB;
        vector<string> PostCompressionFileListVectB;

        FindFileData4 = opendir(EF_SourString2.c_str());
        hFind4 = readdir(FindFileData4);

        while (FileName4.compare(hFind4->d_name) != 0)
        {
            FileName4 = hFind4->d_name;
            //puts(hFind4->d_name);

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorB.begin();

            while (it < SourceFileVectorB.end())
            {
                //checks current directory for opsnr.stt file
                if (FileName4.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName4.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVectB.push_back(FileFound);
            PostCompressionFileListVectB.push_back(FileName4);

            hFind4 = readdir(FindFileData4);

            if (hFind4 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2B;
        it2B = FileFoundVectB.begin();
        int vectorpositionB = 0;


        cout << "\n";
        cerr << "\n";

        while (it2B < FileFoundVectB.end())
        {
            if (FileFoundVectB[vectorpositionB] == 0)
            {
                cout << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];
                cerr << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2B++;
            vectorpositionB++;
        }

        closedir(FindFileData4);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        cout << "\n";
        cerr << "\n";

        if (Fail == 2)
        {
            vector<string>::iterator it3;
            it3 = DestFileExtraVector.begin();
            int Varloc = 0;

            while (it3 < DestFileExtraVector.end())
            {
                cout << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                cerr << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                Varloc++;
                it3++;
            }

            Fail = 1;
        }

        if (Fail == 1)
        {
            printf("\n-------------------------------------------------------------------------------\n");
            fprintf(stderr, "\n-------------------------------------------------------------------------------\n");

            cout << "\nExtra files found - Test Failed\n";
            cerr << "\nExtra files found - Test Failed\n";

            if (TestType == 2)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 1;
                outfile.close();

                return 10;
            }

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            printf("-------------------------------------------------------------------------------\n");
            fprintf(stderr, "-------------------------------------------------------------------------------\n");

            cout << "\n No extra files found - Test Passed\n";
            cerr << "\n No extra files found - Test Passed\n";

            if (TestType == 2)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 0;
                outfile.close();

                return 10;
            }

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
    }
#endif
#if defined(_WIN32)
    char *CompressString = "opt.DeleteFirstPassFile";

    if (!(argc == 3 || argc == 4))
    {
        printf(
            "  ExtraFileCheck \n\n"
            "    <inputfile>\n"
            "	 <Optional Settings File>\n"
            "\n"
        );
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "Extra File Check Test";
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

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string ExtraFileDir = WorkingDirString;
    ExtraFileDir.push_back(slashChar);
    ////////////////////OutPut Directory location////////////////////
    char EF_DestChar[255];
    snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
    string EF_DestString = EF_DestChar;
    EF_DestString.append("*");
    //////////////////////////////////////////////////////////////////
    //////////////////Program Location///////////////////////////////
    char EF_SourChar[255];
    char EF_SourChar2[255];
    FolderName(argv[0], EF_SourChar2);
    snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
    string EF_SourString1 = EF_SourChar;
    EF_SourString1.append("*");
    ////////////////////////////////////////////////////////////////
    //////////////////Current Dir Locatoin//////////////////////////
    TCHAR szDirectory[MAX_PATH] = "";

    if (!::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory))
    {
        cout << "Could not get current directory";
        cerr << "Could not get current directory";
        return 0;
    }

    string EF_SourString2 = szDirectory;
    EF_SourString2.append("\\");
    EF_SourString2.append("*");
    ////////////////////////////////////////////////////////////////

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append("\\ExtraFileCheck.ivf");
    WorkingDir5.append("\\ExtraFileCheckResult.txt");

    char ExtraFileCheck[255];
    char ExtraFileCheckResult[255];

    snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
    snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

    printf("Extra File Check\n\n");
    fprintf(stderr, "Extra File Check\n\n");

    printf("---------------------------Checking for OPSNR Files----------------------------\n");
    fprintf(stderr, "---------------------------Checking for OPSNR Files----------------------------\n");

    /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
    char CurrentPath[260];
    _getcwd(CurrentPath, 260);

    string CurrentPathStr = CurrentPath;

    cout << "Checking: " << CurrentPathStr.c_str() << " For opsnr.stt - ";
    cerr << "Checking: " << CurrentPathStr.c_str() << " For opsnr.stt - ";

    CurrentPathStr.append(slashCharStr);
    CurrentPathStr.append("opsnr.stt");

    FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

    if (opsnrExists == NULL)
    {
        cout << "opsnr.stt File not found. \n\n";
        cerr << "opsnr.stt File not found. \n\n";

    }
    else
    {
        fclose(opsnrExists);
        remove(CurrentPathStr.c_str());
        cout << "opsnr.stt File found and deleted. \n";
        cerr << "opsnr.stt File found and deleted. \n";
    }

    string EF_DestString2 = EF_DestChar;

    cout << "Checking: " << EF_DestString2.c_str() << " For opsnr.stt - ";
    cerr << "Checking: " << EF_DestString2.c_str() << " For opsnr.stt - ";

    EF_DestString2.append(slashCharStr);
    EF_DestString2.append("opsnr.stt");

    FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

    if (opsnrExists2 == NULL)
    {
        cout << "opsnr.stt File not found. \n";
        cerr << "opsnr.stt File not found. \n";
    }
    else
    {
        fclose(opsnrExists2);
        remove(EF_DestString2.c_str());
        cout << "opsnr.stt File found and deleted. \n";
        cerr << "opsnr.stt File found and deleted. \n";
    }

    printf("-------------------------------------------------------------------------------\n");
    fprintf(stderr, "-------------------------------------------------------------------------------\n");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////


    char *input = argv[2];

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 4)
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
    }

    /////////////////////////////////////////////////////////

    int CompressInt = opt.AllowDF;
    int BitRate = opt.TargetBandwidth;
    int Fail = 0;

    //Record all files in the executable directory
    WIN32_FIND_DATA FindFileDataA;
    HANDLE hFindA;
    string FileNameA;
    vector<string> SourceFileVectorA;

    hFindA = FindFirstFile(EF_SourString1.c_str(), &FindFileDataA);

    while (FileNameA.compare(FindFileDataA.cFileName) != 0)
    {
        FileNameA = FindFileDataA.cFileName;
        SourceFileVectorA.push_back(FileNameA);
        FindNextFile(hFindA, &FindFileDataA);
    }

    //Record all files in the current directory.
    WIN32_FIND_DATA FindFileDataB;
    HANDLE hFindB;
    string FileNameB;
    vector<string> SourceFileVectorB;

    hFindB = FindFirstFile(EF_SourString2.c_str(), &FindFileDataB);

    while (FileNameB.compare(FindFileDataB.cFileName) != 0)
    {
        FileNameB = FindFileDataB.cFileName;
        SourceFileVectorB.push_back(FileNameB);
        FindNextFile(hFindB, &FindFileDataB);
    }

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        printf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");
        fprintf(stderr, "\nExtraFileCheck Test Run Previously Retreiveing Result\n");

        ifstream infile(ExtraFileCheckResult);

        if (!infile.is_open())
        {
            printf("File: %s not opened", ExtraFileCheckResult);
            fprintf(stderr, "File: %s not opened", ExtraFileCheckResult);
            return 0;
        }

        infile >> Fail;
        infile.close();

        if (Fail == 1)
        {
            cout << "\n\nExtra files found - Test Failed\n";
            cerr << "\n\nExtra files found - Test Failed\n";
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            cout << "\n\n No extra files found - Test Passed\n";
            cerr << "\n\n No extra files found - Test Passed\n";
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }
    else
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 1/*opt.DeleteFirstPassFile*/) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    printf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");
    fprintf(stderr, "\n\n---------------------------Checking for Extra Files----------------------------\n\n");

    cout << "Checking: " << EF_DestString << "\n\nChecking: " << EF_SourString1;
    cerr << "Checking: " << EF_DestString << "\n\nChecking: " << EF_SourString1;

    cout << "\n\nChecking: " << EF_SourString2.c_str() << "\n\n";
    cerr << "\n\nChecking: " << EF_SourString2.c_str() << "\n\n";

    ////////////////////////////////////////////////Windows Specific///////////////////////////////////
    WIN32_FIND_DATA FindFileData2;
    HANDLE hFind2;

    hFind2 = FindFirstFile(EF_DestString.c_str(), &FindFileData2);

    string FileName2;
    vector<string> DestFileExtraVector;

    while (FileName2.compare(FindFileData2.cFileName) != 0)
    {
        FileName2 = FindFileData2.cFileName;

        //Add File names here that you want the file checker to ignore follow format in if
        if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
            FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("Extra File Check Test.txt")
            && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
            && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
        {
            DestFileExtraVector.push_back(FileName2);
            Fail = 2;
        }

        if (FileName2.compare("opsnr.stt") == 0)
        {
            DestFileExtraVector.push_back(FileName2);
            Fail = 2;
        }

        FindNextFile(hFind2, &FindFileData2);
    }

    //Check through source for files other than the originals or a .fpf file.
    WIN32_FIND_DATA FindFileData3;
    HANDLE hFind3;
    string FileName3;

    int filenum = 0;
    vector<int> FileFoundVect;
    vector<string> PostCompressionFileListVect;

    hFind3 = FindFirstFile(EF_SourString1.c_str(), &FindFileData3);

    while (FileName3.compare(FindFileData3.cFileName) != 0)
    {
        FileName3 = FindFileData3.cFileName;

        int vectloc = 0;
        int FileFound = 0;
        vector<string>::iterator it;
        it = SourceFileVectorA.begin();

        while (it < SourceFileVectorA.end())
        {
            //checks Exe directory for opsnr.stt file
            if (FileName3.compare("opsnr.stt") == 0)
            {
                FileFound = 0;
                break;
            }

            if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
            {
                FileFound = 1;
                break;
            }

            if (FileName3.find(".fpf") != string::npos)
            {
                FileFound = 1;
                break;
            }

            vectloc++;
            it++;
        }

        FileFoundVect.push_back(FileFound);
        PostCompressionFileListVect.push_back(FileName3);

        FindNextFile(hFind3, &FindFileData3);
    }

    vector<int>::iterator it2;
    it2 = FileFoundVect.begin();
    int vectorposition = 0;

    while (it2 < FileFoundVect.end())
    {
        if (FileFoundVect[vectorposition] == 0)
        {
            cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
            cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

            if (Fail != 2)
            {
                Fail = 1;
            }
        }

        it2++;
        vectorposition++;
    }

    //Check through current directory a .fpf file.
    WIN32_FIND_DATA FindFileData4;
    HANDLE hFind4;
    string FileName4;

    int filenumB = 0;
    vector<int> FileFoundVectB;
    vector<string> PostCompressionFileListVectB;

    hFind4 = FindFirstFile(EF_SourString2.c_str(), &FindFileData4);

    while (FileName4.compare(FindFileData4.cFileName) != 0)
    {
        FileName4 = FindFileData4.cFileName;

        int vectloc = 0;
        int FileFound = 0;
        vector<string>::iterator it;
        it = SourceFileVectorB.begin();

        while (it < SourceFileVectorB.end())
        {
            //checks current directory for opsnr.stt file
            if (FileName4.compare("opsnr.stt") == 0)
            {
                FileFound = 0;
                break;
            }

            if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
            {
                FileFound = 1;
                break;
            }

            if (FileName4.find(".fpf") != string::npos)
            {
                FileFound = 1;
                break;
            }

            vectloc++;
            it++;
        }

        FileFoundVectB.push_back(FileFound);
        PostCompressionFileListVectB.push_back(FileName4);

        FindNextFile(hFind4, &FindFileData4);
    }

    vector<int>::iterator it2B;
    it2B = FileFoundVectB.begin();
    int vectorpositionB = 0;

    while (it2B < FileFoundVectB.end())
    {
        if (FileFoundVectB[vectorpositionB] == 0)
        {
            cout << "Extra File Found: " << PostCompressionFileListVectB[vectorpositionB] << "\n";
            cerr << "Extra File Found: " << PostCompressionFileListVectB[vectorpositionB] << "\n";

            if (Fail != 2)
            {
                Fail = 1;
            }
        }

        it2B++;
        vectorpositionB++;
    }

    if (Fail == 2)
    {
        vector<string>::iterator it3;
        it3 = DestFileExtraVector.begin();
        int Varloc = 0;

        while (it3 < DestFileExtraVector.end())
        {
            cout << "\n" << "Extra File Found: " << DestFileExtraVector[Varloc];
            cerr << "\n" << "Extra File Found: " << DestFileExtraVector[Varloc];
            Varloc++;
            it3++;
        }

        Fail = 1;
    }

    if (Fail == 1)
    {
        printf("\n-------------------------------------------------------------------------------\n");
        fprintf(stderr, "\n-------------------------------------------------------------------------------\n");

        cout << "\nExtra files found - Test Failed\n";
        cerr << "\nExtra files found - Test Failed\n";

        if (TestType == 2)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 1;
            outfile.close();

            return 10;
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (Fail == 0)
    {
        printf("-------------------------------------------------------------------------------\n");
        fprintf(stderr, "-------------------------------------------------------------------------------\n");

        cout << "\nNo extra files found - Test Passed\n";
        cerr << "\nNo extra files found - Test Passed\n";



        if (TestType == 2)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 0;
            outfile.close();

            return 10;
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);

#endif
    return 0;
}
int ForceKeyFrameWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "opt.KeyFreq";
    char *input = argv[2];

    if (!(argc == 6 || argc == 7))
    {
        printf(
            "  ForceKeyFrameWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <ForceKeyFrame>\n"
            "	 <Optional Settings File>\n"
            "\n"
        );
        return 0;
    }



    int speed = 0;
    int BitRate = atoi(argv[4]);
    int ForceKeyFrameInt = atoi(argv[5]);

    int Mode = atoi(argv[3]);

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "ForceKeyFrameWorks";
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
    WorkingDir4.append("ForceKeyFrameWorksOutPut.txt");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("ForceKeyFrameOutput.ivf");

    char outputfile[255];
    char ForceKeyFrame[255];

    snprintf(outputfile, 255, "%s", WorkingDir4.c_str());
    snprintf(ForceKeyFrame, 255, "%s", WorkingDir5.c_str());

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

    printf("ForceKeyFrameWorks test\n");
    fprintf(stderr, "ForceKeyFrameWorks test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 7)
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
    opt.AutoKey = 1;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.KeyFreq = ForceKeyFrameInt;

            if (CompressIVFtoIVF(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt) == -1)
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
            opt.KeyFreq = ForceKeyFrameInt;

            if (CompressIVFtoIVF(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt) == -1)
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
            opt.KeyFreq = ForceKeyFrameInt;

            if (CompressIVFtoIVF(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt) == -1)
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
            opt.KeyFreq = ForceKeyFrameInt;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.KeyFreq = ForceKeyFrameInt;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt) == -1)
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

    IVFDisplayKeyFrames(ForceKeyFrame, 1, outputfile);
    //////////////////////////////////////////////////////////////////////////////////
    ifstream infile(outputfile);

    if (!infile.good())
    {
        printf("\nKey Frame File Not Present\n");
        fprintf(stderr, "\nKey Frame File Not Present\n");
        fclose(fp);
        return 0;
    }

    int selector2 = 0;
    int doOnce2 = 0;
    int x2;
    int y2;

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
            x2 = 0;
            infile >> y2;
            doOnce2 = 1;
            selector2 = 1;
        }

        if (absInt(y2 - x2) > ForceKeyFrameInt)
        {
            printf("\n\nForce Key Frame not working : Failed \n") ;
            fprintf(stderr, "\n\nForce Key Frame not working : Failed \n") ;
            fclose(fp);
            return 0;
        }
    }

    cout << "\n";
    ///////////////////////////////////////////////////////////////////////////

    printf("\n\n Passed: Key Frames occur at least as frequently as Force Key Frame dictates: %i\n", ForceKeyFrameInt);
    fprintf(stderr, "\n\nPassed: Key Frames occur at least as frequently as Force Key Frame dictates: %i\n", ForceKeyFrameInt);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 1;
}
int GoodQvBestQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "Allow DF";
    char *input = argv[2];

    if (!(argc == 4 || argc == 5))
    {
        printf(
            "  GoodQvBestQ \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n ");

        return 0;
    }

    /////////////////Might want to see if you can make a function to do this so its a bit cleaner/////////////////
    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "GoodQvBestQ";
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
    WorkingDir4.append("GoodVsBestOutput1.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("GoodVsBestOutput2.ivf");

    char GvBgOutFile[255];
    char GvBbOutFile[255];

    snprintf(GvBgOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(GvBbOutFile, 255, "%s", WorkingDir5.c_str());

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
    int BitRate = atoi(argv[3]);

    printf("GoodQvBestQ Test\n");
    fprintf(stderr, "GoodQvBestQ Test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 5)
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

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        opt.Mode = MODE_GOODQUALITY;

        if (CompressIVFtoIVF(input, GvBgOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.Mode = MODE_BESTQUALITY;

        if (CompressIVFtoIVF(input, GvBbOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
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

    printf("\n");
    fprintf(stderr, "\n");

    float GoodSize = IVFDataRate(GvBgOutFile, 1);
    float BestSize = IVFDataRate(GvBbOutFile, 1);

    int PSNRToggle = PSNRSelect(input, GvBgOutFile);

    double PSNRG;
    double PSNRB;

    double ssim = 0;
    PSNRG = IVFPSNR(input, GvBgOutFile, PSNRToggle, 0, 1, ssim);
    PSNRB = IVFPSNR(input, GvBbOutFile, PSNRToggle, 0, 1, ssim);

    float PSRNPerc = absFloat((PSNRB - PSNRG) / PSNRG) * 100.00;
    float BRPerc = (absFloat(BestSize - GoodSize) / GoodSize) * 100.00;

    int Pass = 0;

    if (PSNRB == PSNRG)
    {
        printf("\n No Difference Between Good Quality and Best Quality PSNRs\n");
        fprintf(stderr, "\n No Difference Between Good Quality and Best Quality PSNRs\n");
        Pass = 1;
    }

    if (PSNRB > PSNRG)
    {
        printf("\n Best Quality returned a %f%% higher PSNR \n", PSRNPerc);
        fprintf(stderr, "\n Best Quality returned a %f%% higher PSNR \n", PSRNPerc);
        Pass = 1;
    }

    if (PSNRB < PSNRG)
    {
        printf("\n Best Quality returned a %f%% lower PSNR \n", PSRNPerc);
        fprintf(stderr, "\n Best Quality returned a %f%% lower PSNR \n", PSRNPerc);
    }

    if (BestSize == GoodSize)
    {
        printf("\n No Difference Between Good Quality and Best Quality Data Rates\n");
        fprintf(stderr, "\n No Difference Between Good Quality and Best Quality Data Rates\n");
    }

    if (BestSize > GoodSize)
    {
        printf("\n Best Quality returned a %f%% higher Data Rate \n ", BRPerc);
        fprintf(stderr, "\n Best Quality returned a %f%% higher Data Rate \n ", BRPerc);
    }

    if (BestSize < GoodSize)
    {
        printf("\n Best Quality returned a %f%% lower Data Rate\n ", BRPerc);
        fprintf(stderr, "\n Best Quality returned a %f%% lower Data Rate\n ", BRPerc);
        Pass = 1;
    }

    if (Pass == 1)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int MaxQTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "QuantizerMax";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  MaxQTest \n\n"
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
    char *MyDir = "MaxQTest";
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
    string WorkingDir6 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("QuantOutput");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("QuantOutput.ivf");

    char QuantOutFile2[255];

    snprintf(QuantOutFile2, 255, "%s", WorkingDir6.c_str());

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


    printf("Max Q Test");
    fprintf(stderr, "Max Q Test");

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
    int PSNRToggle = 0;

    int n = 3;
    double PSNRArr[10];
    int i = 0;
    double ssim = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        while (n < 63)
        {
            opt.WorstAllowedQ = n;

            char num[20];
            itoa_custom(n, num, 10);

            string WorkingDir5 = WorkingDir4;
            WorkingDir5.append(num);
            WorkingDir5.append(".ivf");

            char QuantOutFile[255];
            snprintf(QuantOutFile, 255, "%s", WorkingDir5.c_str());

            printf("\n");
            fprintf(stderr, "\n");

            if (TestType != 2)
            {
                PSNRArr[i] = IVFPSNR(input, QuantOutFile, PSNRToggle, 0, 1, ssim);
            }

            n = n + 8;
            i++;
        }
    }
    else
    {
        while (n < 63)
        {

            opt.WorstAllowedQ = n;

            char num[20];
            itoa_custom(n, num, 10);

            string WorkingDir5 = WorkingDir4;
            WorkingDir5.append(num);
            WorkingDir5.append(".ivf");

            char QuantOutFile[255];
            snprintf(QuantOutFile, 255, "%s", WorkingDir5.c_str());

            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;

                if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n) == -1)
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

                if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            printf("\n");
            fprintf(stderr, "\n");

            if (TestType != 2)
            {
                PSNRArr[i] = IVFPSNR(input, QuantOutFile, PSNRToggle, 0, 1, ssim);
            }

            n = n + 8;
            i++;
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

    i = 0;

    while (i < 7)
    {
        printf("\n %f <= %f", PSNRArr[i+1], PSNRArr[i]);
        fprintf(stderr, "\n %f <= %f", PSNRArr[i+1], PSNRArr[i]);

        if (!(PSNRArr[i+1] <= PSNRArr[i]))
        {
            printf("\n Failed: Not all PSNRs Increase as Max Q Decreases %f > %f \n", PSNRArr[i+1], PSNRArr[i]);
            fprintf(stderr, "\n Failed: Not all PSNRs Increase as Max Q Decreases %f > %f \n", PSNRArr[i+1], PSNRArr[i]);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        i++;
    }

    printf("\n\n Passed: All PSNRs Increase as Max Q Decreases\n");
    fprintf(stderr, "\n\n Passed: All PSNRs Increase as Max Q Decreases\n");

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 1;
}
int MemLeakCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    // So long as Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>

    if (!(argc == 6 || argc == 7))
    {
        printf(
            "  MemLeakCheck \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Mem Leak Check Exe>\n"
            "	 <Optional Settings File>\n"
            "\n");
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char MemLeakExe[255];

#if defined(_WIN32)
    {
        snprintf(MemLeakExe, 255, "%s", argv[5]);
    }
#elif defined(linux)
    {
        string MemLeakExeStr = argv[5];
        snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    }
#elif defined(__APPLE__)
    {
        string MemLeakExeStr = argv[5];
        snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    }
#elif defined(__POWERPC__)
    {
        string MemLeakExeStr = argv[5];
        snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    }
#endif

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char ExeCharMemLeak[1024] = "";
    char *MyDir = "MemLeakCheck";
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

    //Get the exe's parent folder From argv[0] Paths for both exes will be the same
    FolderName(argv[0], ExeCharMemLeak);
    string ExeCharMemLeakStr = ExeCharMemLeak;


    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = WorkingDirString;
    string WorkingDir9;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("MemLeakCheck.ivf");

    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("MemLeakCheckOutput.txt");

    WorkingDir8.append(slashCharStr);
    WorkingDir8.append("MemLeackCheckParFile.txt");

#if defined(_WIN32)
    {
        WorkingDir9 = "\"\"";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\" 8 \"");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\" 4 \"");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\"\"");
    }

#elif defined(linux)
    {
        WorkingDir9 = "\'";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\' \'");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\' \'");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\' 8 \'");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\' 4 \'");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\'");
    }
#elif defined(__APPLE__)
    {
        WorkingDir9 = "\"";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\" 8 \"");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\" 4 \"");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\"");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir9 = "\"";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\" 8 \"");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\" 4 \"");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\"");
    }
#endif


    string WorkingDir10 = WorkingDirString;
    string WorkingDir11;

    WorkingDir10.append(slashCharStr);
    WorkingDir10.append("MemLeakCheckOutput_FP.txt");

#if defined(_WIN32)
    {
        WorkingDir11 = "\"\"";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);		// Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\" 8 \"");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir11.append("\" 4 \"");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir11.append("\"\"");
    }
#elif defined(linux)
    {
        WorkingDir11 = "\'";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);			// Exe Name
        WorkingDir11.append("\' \'");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\' \'");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\' 8 \'");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir11.append("\' 4 \'");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir11.append("\'");
    }
#elif defined(__APPLE__)
    {
        WorkingDir11 = "\"";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);			// Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\" 8 \"");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir11.append("\" 4 \"");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir11.append("\"");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir11 = "\"";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);			// Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\" 8 \"");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir11.append("\" 4 \"");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir11.append("\"");
    }
#endif


    char MemLeakCheckIVF[255];
    char MemLeakCheckTXT[255];
    char MemLeakCheckTXT_FP[255];
    char MemLeakCheckParFile[255];
    char ProgramMemLeakCheck[2048];
    char ProgramMemLeakCheck_FirstPass[2048];

    snprintf(MemLeakCheckIVF, 255, "%s", WorkingDir4.c_str());
    snprintf(MemLeakCheckTXT, 255, "%s", WorkingDir7.c_str());
    snprintf(MemLeakCheckParFile, 255, "%s", WorkingDir8.c_str());
    snprintf(ProgramMemLeakCheck, 2048, "%s", WorkingDir9.c_str());
    snprintf(MemLeakCheckTXT_FP, 255, "%s", WorkingDir10.c_str());
    snprintf(ProgramMemLeakCheck_FirstPass, 2048, "%s", WorkingDir11.c_str());

    //cout << "\n\n\n" << ProgramMemLeakCheck << "\n\n\n";
    //cout << "\n\n\n" << ProgramMemLeakCheck_FirstPass << "\n\n\n";

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


    printf("MemLeakCheck\n");
    fprintf(stderr, "MemLeakCheck\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 7)
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

    opt.TargetBandwidth = BitRate ;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
    }
    else
    {
        if (Mode == 0)
        {
            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            opt.Mode = MODE_REALTIME;
            OutPutSettings(MemLeakCheckParFile, opt);
            RunExe(ProgramMemLeakCheck);
        }

        if (Mode == 1)
        {
            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            opt.Mode = MODE_GOODQUALITY;
            OutPutSettings(MemLeakCheckParFile, opt);
            RunExe(ProgramMemLeakCheck);
        }

        if (Mode == 2)
        {
            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            opt.Mode = MODE_BESTQUALITY;
            OutPutSettings(MemLeakCheckParFile, opt);
            RunExe(ProgramMemLeakCheck);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            OutPutSettings(MemLeakCheckParFile, opt);
            RunExe(ProgramMemLeakCheck);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            OutPutSettings(MemLeakCheckParFile, opt);
            RunExe(ProgramMemLeakCheck);
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



    if (1)
    {
        cout << "\nMode " << Mode << "\n";
        cout << "\nMemLeakCheckTXT " << MemLeakCheckTXT << "\n";

        ifstream infile4(MemLeakCheckTXT);

        if (!infile4.good())
        {
            printf("MemLeakCheckOutput.txt not found\n");
            fprintf(stderr, "MemLeakCheckOutput.txt not found\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        char buffer4[256];

        infile4.getline(buffer4, 256);
        infile4.getline(buffer4, 256);
        infile4.getline(buffer4, 256);
        infile4.getline(buffer4, 256);

        string bufferString4 = buffer4;

        if (bufferString4.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            printf("\n\nMemory Leak Check Passed\n");
            fprintf(stderr, "\n\nMemory Leak Check Passed\n");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nMemory Leack Check Failed\n");
            fprintf(stderr, "\nMemory Leack Check Failed\n");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    else
    {

        ifstream infile1(MemLeakCheckTXT_FP);

        if (!infile1.good())
        {
            printf("MemLeakCheckOutput_FP.txt not found\n");
            fprintf(stderr, "MemLeakCheckOutput_FP.txt not found\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        char buffer1[256];

        infile1.getline(buffer1, 256);
        infile1.getline(buffer1, 256);
        infile1.getline(buffer1, 256);
        infile1.getline(buffer1, 256);

        string bufferString1 = buffer1;

        int Fail = 0;

        if (bufferString1.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            cout << "\nFirst Pass File MemLeak Check Pass. \n";
            cerr << "\nFirst Pass File MemLeak Check Pass. \n";
        }
        else
        {
            cout << "\nFirst Pass File MemLeak Check Fail. \n";
            cerr << "\nFirst Pass File MemLeak Check Fail. \n";
            Fail = 1;
        }

        ////second file check////

        ifstream infile2(MemLeakCheckTXT);

        if (!infile2.good())
        {
            printf("MemLeakCheckOutput.txt not found\n");
            fprintf(stderr, "MemLeakCheckOutput.txt not found\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        char buffer2[256];

        infile2.getline(buffer2, 256);
        infile2.getline(buffer2, 256);
        infile2.getline(buffer2, 256);
        infile2.getline(buffer2, 256);

        string bufferString2 = buffer2;

        if (bufferString2.compare(0, 24, "_currently Allocated= 0;") == 0 && Fail == 0)
        {
            printf("\n\nSecond Pass Memory Leak Check Passed\n");
            fprintf(stderr, "\n\nSecond Pass Memory Leak Check Passed\n");

            if (Fail == 1)
            {
                printf("\n\nFailed: First Pass Memory Leak Check File Failed\n");
                fprintf(stderr, "\n\nFailed: First Pass Memory Leak Check File Failed\n");
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 0;
            }

            if (Fail == 0)
            {
                printf("\n\nPassed: Both First and Second Pass Memory Leak Check Files Passed\n");
                fprintf(stderr, "\n\nPassed: Both First and Second Pass Memory Leak Check Files Passed\n");
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 1;
            }


        }
        else
        {
            printf("\nSecond Pass Memory Leack Check Failed\n");
            fprintf(stderr, "\nSecond Pass Memory Leack Check Failed\n");

            if (Fail == 1)
            {
                printf("\n\nFailed: First and Second Pass Memory Leak Check File Failed\n");
                fprintf(stderr, "\n\nFailed: Second Pass Memory Leak Check File Failed\n");
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 0;
            }

            if (Fail == 0)
            {
                printf("\n\nFailed: First and Second Pass Memory Leak Check File Failed\n");
                fprintf(stderr, "\n\nFailed: Second Pass Memory Leak Check File Failed\n");
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 0;
            }
        }
    }



    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;

}
int MemLeakCheck2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    // So long as Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>

    if (!(argc == 4 || argc == 5))
    {
        printf(
            "  MemLeakCheck2 \n\n"
            "    <Mem Leak Check Exe>\n"
            "    <Decode Test File>\n"
            "\n");
        return 0;
    }

    char *input = "Blank";
    int Mode = 1;
    int BitRate = 128;
    char MemLeakExe[255];
    char DecInFile[255];

#if defined(_WIN32)
    {
        snprintf(MemLeakExe, 255, "%s", argv[2]);
        snprintf(DecInFile, 255, "%s", argv[3]);
    }
#elif defined(linux)
    {
        string MemLeakExeStr = argv[2];
        snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    }
#elif defined(__APPLE__)
    {
        string MemLeakExeStr = argv[2];
        snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    }
#elif defined(__POWERPC__)
    {
        string MemLeakExeStr = argv[2];
        snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    }
#endif

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char ExeCharMemLeak[1024] = "";
    char *MyDir = "MemLeakCheck2";
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

    //Get the exe's parent folder From argv[0] Paths for both exes will be the same
    FolderName(argv[0], ExeCharMemLeak);
    string ExeCharMemLeakStr = ExeCharMemLeak;


    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = WorkingDirString;
    string WorkingDir9;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("MemLeakCheck.ivf");

    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("MemLeakCheckOutput");

    WorkingDir8.append(slashCharStr);
    WorkingDir8.append("MemLeackCheckParFile");

#if defined(_WIN32)
    {
        WorkingDir9 = "\"\"";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\" 0 \"");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\" 5 \"");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\"\"");
    }

#elif defined(linux)
    {
        WorkingDir9 = "\'";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\' \'");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\' \'");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\' 0 \'");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\' 5 \'");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\'");
    }
#elif defined(__APPLE__)
    {
        WorkingDir9 = "\"";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\" 0 \"");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\" 5 \"");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\"");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir9 = "\"";
        WorkingDir9.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir9.append(MemLeakExe);			// Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);				// Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);		// Output
        WorkingDir9.append("\" 0 \"");
        WorkingDir9.append(WorkingDir8);		// Par File
        WorkingDir9.append("\" 5 \"");
        WorkingDir9.append(WorkingDir7);		// Mem Output File
        WorkingDir9.append("\"");
    }
#endif


    string WorkingDir10 = WorkingDirString;
    string WorkingDir11;
    string WorkingDir12;

    WorkingDir10.append(slashCharStr);
    WorkingDir10.append("MemLeakCheckOutput");

    string MemLeakCheckResultfile1 = WorkingDir10;
    string MemLeakCheckResultfile2 = WorkingDir10;
    MemLeakCheckResultfile1.append("_Encode.txt");
    MemLeakCheckResultfile2.append("_Decode.txt");

    char MemLeakCheckTXT1[255];
    char MemLeakCheckTXT2[255];
    snprintf(MemLeakCheckTXT1, 255, "%s", MemLeakCheckResultfile1.c_str());
    snprintf(MemLeakCheckTXT2, 255, "%s", MemLeakCheckResultfile2.c_str());

#if defined(_WIN32)
    {
        WorkingDir11 = "\"\"";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);			// Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\" 0 \"");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\" 5 \"");
        WorkingDir12.append("\" 6 \"");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append("\" \"");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\"\"");
        WorkingDir12.append("\"\"");
    }
#elif defined(linux)
    {
        WorkingDir11 = "\'";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);			// Exe Name
        WorkingDir11.append("\' \'");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\' \'");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\' 0 \'");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\' 5 \'");
        WorkingDir12.append("\' 6 \'");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append("\' \'");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\'\'");
        WorkingDir12.append("\'\'");
    }
#elif defined(__APPLE__)
    {
        WorkingDir11 = "\"";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);			// Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\" 0 \"");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\" 5 \"");
        WorkingDir12.append("\" 6 \"");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append("\" \"");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\"\"");
        WorkingDir12.append("\"\"");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir11 = "\"";
        WorkingDir11.append(ExeCharMemLeakStr);	// Exe Path
        WorkingDir11.append(MemLeakExe);			// Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);				// Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);		// Output
        WorkingDir11.append("\" 0 \"");
        WorkingDir11.append(WorkingDir8);		// Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\" 5 \"");
        WorkingDir12.append("\" 6 \"");
        WorkingDir11.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append(WorkingDir10);		// Mem Output File
        WorkingDir12.append("\" \"");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\"\"");
        WorkingDir12.append("\"\"");
    }
#endif


    char MemLeakCheckIVF[255];
    char MemLeakCheckTXT[255];
    char MemLeakCheckParFile[255];
    char ProgramMemLeakCheckEnc[2048];
    char ProgramMemLeakCheckDec[2048];

    snprintf(MemLeakCheckIVF, 255, "%s", WorkingDir4.c_str());
    snprintf(MemLeakCheckTXT, 255, "%s", WorkingDir7.c_str());
    snprintf(MemLeakCheckParFile, 255, "%s", WorkingDir8.c_str());
    snprintf(ProgramMemLeakCheckEnc, 2048, "%s", WorkingDir9.c_str());
    snprintf(ProgramMemLeakCheckDec, 2048, "%s", WorkingDir12.c_str());

    cout << "\n\n\n" << ProgramMemLeakCheckEnc << "\n\n\n";
    cout << "\n\n\n" << ProgramMemLeakCheckDec << "\n\n\n";

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


    printf("MemLeakCheck 2\n");
    fprintf(stderr, "MemLeakCheck 2\n");

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

    opt.TargetBandwidth = BitRate ;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
    }
    else
    {
        fclose(fp);

        if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
        {
            printf("Cannot open out put file4.\n");
            exit(1);
        }

        fprintf(stderr, " ");

        opt.Mode = MODE_GOODQUALITY;
        OutPutSettings(MemLeakCheckParFile, opt);
        RunExe(ProgramMemLeakCheckEnc);
        RunExe(ProgramMemLeakCheckDec);
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

    ifstream infile4a(MemLeakCheckTXT1);

    if (!infile4a.good())
    {
        printf("%s not found\n", infile4a);
        fprintf(stderr, "%s not found\n", infile4a);

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    char buffer4a[256];

    infile4a.getline(buffer4a, 256);
    infile4a.getline(buffer4a, 256);
    infile4a.getline(buffer4a, 256);
    infile4a.getline(buffer4a, 256);

    string bufferString4a = buffer4a;

    ifstream infile4b(MemLeakCheckTXT2);

    if (!infile4b.good())
    {
        printf("%s not found\n", infile4b);
        fprintf(stderr, "%s not found\n", infile4b);

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    char buffer4b[256];

    infile4b.getline(buffer4b, 256);
    infile4b.getline(buffer4b, 256);
    infile4b.getline(buffer4b, 256);
    infile4b.getline(buffer4b, 256);

    string bufferString4b = buffer4b;

    if (bufferString4a.compare(0, 23, "Currently Allocated= 0;") == 0 && bufferString4b.compare(0, 23, "Currently Allocated= 0;") == 0)
    {
        printf("\n\nMemory Leak Check 2 Passed\n");
        fprintf(stderr, "\n\nMemory Leak 2 Check Passed\n");
        infile4a.close();
        infile4b.close();
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        printf("\nMemory Leack Check 2 Failed\n");
        fprintf(stderr, "\nMemory Leack Check 2 Failed\n");
        infile4a.close();
        infile4b.close();
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;

}
int MinQTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "BestAllowedQ";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  MinQTest \n\n"
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
    char *MyDir = "MinQTest";
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
    WorkingDir4.append("Min10QuantOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("Min60QuantOutput.ivf");

    char Min10QuantOutFile[255];
    char Min60QuantOutFile[255];

    snprintf(Min10QuantOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(Min60QuantOutFile, 255, "%s", WorkingDir5.c_str());

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


    printf("Min Q Test");
    fprintf(stderr, "Min Q Test");

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
    double PSNRArr[2];
    opt.TargetBandwidth = BitRate;

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
            opt.BestAllowedQ = 10;

            if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.BestAllowedQ = 60;

            if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60) == -1)
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
            opt.BestAllowedQ = 10;

            if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.BestAllowedQ = 60;

            if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60) == -1)
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
            opt.BestAllowedQ = 10;

            if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.BestAllowedQ = 60;

            if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60) == -1)
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
            opt.BestAllowedQ = 10;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.BestAllowedQ = 60;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.BestAllowedQ = 10;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.BestAllowedQ = 60;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60) == -1)
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

    int PSNRToggle = PSNRSelect(input, Min10QuantOutFile);

    double ssim = 0;
    PSNRArr[0] = IVFPSNR(input, Min10QuantOutFile, PSNRToggle, 0, 1, ssim);
    PSNRArr[1] = IVFPSNR(input, Min60QuantOutFile, PSNRToggle, 0, 1, ssim);


    if (PSNRArr[0] == PSNRArr[1])
    {
        printf("\n Unknown: QuantizerMin has no effect, try different parameters \n");
        fprintf(stderr, "\n Unknown: QuantizerMin has no effect, try different parameters \n");

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

    if (PSNRArr[0] > PSNRArr[1])
    {
        printf("\n Min Q 10: %f returns a greater PSNR than Min Q 60: %f - Passed \n", PSNRArr[0], PSNRArr[1]);
        fprintf(stderr, "\n Min Q 10: %f returns a greater PSNR than Min Q 60: %f - Passed \n", PSNRArr[0], PSNRArr[1]);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }


    printf("\n Min Q 10: %f returns a lower PSNR than Min Q 60: %f - Failed\n", PSNRArr[0], PSNRArr[1]);
    fprintf(stderr, "\n Min Q 10: %f returns a lower PSNR than Min Q 60: %f - Failed\n", PSNRArr[0], PSNRArr[1]);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

int NewVsOldPSNR(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    if (!(argc == 7 || argc == 8))
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
            "    <Exe File To Compare>\n"
            "    <Parameter Version-1=1.0.4|2=2.0.0>\n"
            "	 <Optional Settings File>\n"
            "\n"

        );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int ParFileNum = atoi(argv[6]);
    char ExeInput[255];

#if defined(_WIN32)
    {
        snprintf(ExeInput, 255, "%s", argv[5]);
    }
#elif defined(linux)
    {
        string ExeInputStr = argv[5];
        snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
    }
#elif defined(__APPLE__)
    {
        string ExeInputStr = argv[5];
        snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
    }
#elif defined(__POWERPC__)
    {
        string ExeInputStr = argv[5];
        snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
    }
#endif
    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    string ExeString = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "NewVsOldPSNR";
    string MainDirString = "";
    char File1[255] = "";


    //////////////////////////////////////////////////////////////////////
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
        char ExeChar2[1024];
        FolderName(argv[0], ExeChar2);
        snprintf(ExeChar, 1024, "%s", ExeChar2);

        ExeString = ExeChar;
#if defined(_WIN32)
        {
            ExeString.insert(0, "\"\"");
        }
#elif defined(linux)
        {
            ExeString.insert(0, "\'");
        }
#elif defined(__POWERPC__)
        {
            ExeString.insert(0, "\'");
        }
#elif defined(__APPLE__)
        {
            ExeString.insert(0, "\'");
        }
#endif
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

    //////////////////////////////////////////////
    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = ExeString;
    string WorkingDir9 = "";

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("outputVP8New.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("outputVP8Old.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("outputVP8Old.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("ParFile.txt");

#if defined(_WIN32)
    {
        WorkingDir8.append(ExeInput);
        WorkingDir8.append("\" \"");
        WorkingDir8.append(input);
        WorkingDir8.append("\" \"");
        WorkingDir8.append(WorkingDir5);
        WorkingDir8.append("\" 8");
        WorkingDir8.append(" \"");
        WorkingDir8.append(WorkingDir7);
        WorkingDir8.append("\"");
        WorkingDir9 = WorkingDir8;
        WorkingDir9.append(" 0");
        WorkingDir9.append("\"");
        WorkingDir8.append(" 3");
        WorkingDir8.append("\"");
    }

#elif defined(linux)
    {
        WorkingDir8.append(ExeInput);
        WorkingDir8.append("\' \'");
        WorkingDir8.append(input);
        WorkingDir8.append("\' \'");
        WorkingDir8.append(WorkingDir5);
        WorkingDir8.append("\' 8");
        WorkingDir8.append(" \'");
        WorkingDir8.append(WorkingDir7);
        WorkingDir8.append("\'");
        WorkingDir9 = WorkingDir8;
        WorkingDir9.append(" 0");
        WorkingDir8.append(" 3");
    }
#elif defined(__APPLE__)
    {
        WorkingDir8.append(ExeInput);
        WorkingDir8.append("\' \'");
        WorkingDir8.append(input);
        WorkingDir8.append("\' \'");
        WorkingDir8.append(WorkingDir5);
        WorkingDir8.append("\' 8");
        WorkingDir8.append(" \'");
        WorkingDir8.append(WorkingDir7);
        WorkingDir8.append("\'");
        WorkingDir9 = WorkingDir8;
        WorkingDir9.append(" 0");
        WorkingDir8.append(" 3");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir8.append(ExeInput);
        WorkingDir8.append("\' \'");
        WorkingDir8.append(input);
        WorkingDir8.append("\' \'");
        WorkingDir8.append(WorkingDir5);
        WorkingDir8.append("\' 8");
        WorkingDir8.append(" \'");
        WorkingDir8.append(WorkingDir7);
        WorkingDir8.append("\'");
        WorkingDir9 = WorkingDir8;
        WorkingDir9.append(" 0");
        WorkingDir8.append(" 3");
    }
#endif

    char outputVP7New[255];
    char outputVP7Old[255];
    char outputVP7Old2[255];
    char ParFile[255];
    char Program[1024];
    char Program2[1024];

    snprintf(outputVP7New, 255, "%s", WorkingDir4.c_str());
    snprintf(outputVP7Old, 255, "%s", WorkingDir5.c_str());
    snprintf(outputVP7Old2, 255, "%s", WorkingDir6.c_str());
    snprintf(ParFile, 255, "%s", WorkingDir7.c_str());
    snprintf(Program, 1024, "%s", WorkingDir8.c_str());
    snprintf(Program2, 1024, "%s", WorkingDir9.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    cout << "TextfileString: " << TextfileString.c_str() << "\n";

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


    printf("New Vs Old PSNR Test\n");
    fprintf(stderr, "New Vs Old PSNR Test\n");

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
    opt.AutoKey = 1;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutCompatSettings(ParFile, opt, ParFileNum);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(Program);
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutCompatSettings(ParFile, opt, ParFileNum);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            cout << "\n\n" << Program << "\n\n";

            RunExe(Program);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutCompatSettings(ParFile, opt, ParFileNum);
            RunExe(Program);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            if (ParFileNum == 1) //The old encoding method for two pass required for the encoder to be called twice once to run the first pass then again for the second
            {
                opt.Mode = MODE_SECONDPASS;
                opt.LagInFrames = 10;

                if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_FIRSTPASS;
                OutPutCompatSettings(ParFile, opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                RunExe(Program2);

                opt.Mode = MODE_SECONDPASS;
                OutPutCompatSettings(ParFile, opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                RunExe(Program);
            }
            else
            {
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_SECONDPASS;
                OutPutCompatSettings(ParFile, opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                RunExe(Program);
            }
        }

        if (Mode == 5)
        {
            if (ParFileNum == 1) //The old encoding method for two pass required for the encoder to be called twice once to run the first pass then again for the second
            {
                opt.Mode = MODE_SECONDPASS_BEST;
                opt.LagInFrames = 10;

                if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = 3;
                OutPutCompatSettings(ParFile, opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                RunExe(Program2);

                opt.Mode = MODE_SECONDPASS_BEST;
                OutPutCompatSettings(ParFile, opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                RunExe(Program);
            }
            else
            {
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_SECONDPASS_BEST;
                OutPutCompatSettings(ParFile, opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                RunExe(Program);
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

    if (FileSize2(outputVP7Old2) == 0)
    {
        printf("\nError - Old File Incorrect\n");
        fprintf(stderr, "\nError - Old File Incorrect\n");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    //////////////////////////////////////////////Input OLD PSNR//////////////////////////////////////////////
    char TimeTextFile[256];

    FolderName(outputVP7New, TimeTextFile);
    string TimeTextFileStr = TimeTextFile;

    TimeTextFileStr.append("outputVP8Old_OLD_PSNR.txt");

    float PSNROLD;
    ifstream infile(TimeTextFileStr.c_str());
    infile >> PSNROLD;
    infile.close();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    int PSNRToggle = PSNRSelect(input, outputVP7New);
    double PSNRArr[2];

    double ssim = 0;
    PSNRArr[0] = IVFPSNR(input, outputVP7New, PSNRToggle, 0, 1, ssim);
    PSNRArr[1] = PSNROLD;

    printf("\nNew DataRate");
    fprintf(stderr, "\nNew DataRate\n");
    IVFDataRate(outputVP7New, 1);

    printf("\nOld DataRate");
    fprintf(stderr, "\nOld DataRate\n");
    IVFDataRate(outputVP7Old2, 1);

    if (PSNRArr[0] > PSNRArr[1])
    {
        printf("\n New PSNR: %f Old PSNR: %f - New PSNR is Greater than old - Passed\n", PSNRArr[0], PSNRArr[1]);
        fprintf(stderr, "\n New PSNR: %f Old PSNR: %f - New PSNR is Greater than old - Passed\n", PSNRArr[0], PSNRArr[1]);

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (PSNRArr[0] * 1.01 > PSNRArr[1])
    {
        printf("\n New PSNR: %f is with in 1%% of old: %f - Passed\n", PSNRArr[0], PSNRArr[1]);
        fprintf(stderr, "\n New PSNR: %f is with in 1%% of old: %f - Passed\n", PSNRArr[0], PSNRArr[1]);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    printf("\n New PSNR: %f lower than Old PSNR: %f - Failed\n", PSNRArr[0], PSNRArr[1]);
    fprintf(stderr, "\n New PSNR: %f lower than Old PSNR: %f - Failed\n", PSNRArr[0], PSNRArr[1]);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int NoiseSensitivityWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
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


    printf("Noise Sensitivity Works\n");
    fprintf(stderr, "Noise Sensitivity Works\n");

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

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.NoiseSensitivity = 6;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6) == -1)
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

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.NoiseSensitivity = 6;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6) == -1)
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

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.NoiseSensitivity = 6;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6) == -1)
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

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.NoiseSensitivity = 6;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6) == -1)
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

            if (CompressIVFtoIVF(input, NoiseSens0OutFile, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.NoiseSensitivity = 6;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, NoiseSens6OutFile, speed, BitRate, opt, CompressString, 6) == -1)
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

    if (PSNR6 < PSNR0)
    {
        printf("\n Noise 6 PSNR: %f < Noise 0 PSNR: %f - Passed\n", PSNR6, PSNR0);
        fprintf(stderr, "\n Noise 6 PSNR: %f < Noise 0 PSNR: %f - Passed\n", PSNR6, PSNR0);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (PSNR6 > PSNR0)
    {
        printf("\n Noise 0 PSNR: %f > Noise 6 PSNR: %f - Failed\n", PSNR0, PSNR6);
        fprintf(stderr, "\n Noise 0 PSNR: %f > Noise 6 PSNR: %f - Failed\n", PSNR0, PSNR6);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (PSNR6 == PSNR0)
    {
        printf("\n Unknown: Noise-Sensitivity has no effect, try different parameters \n");
        fprintf(stderr, "\n Unknown: Noise-Sensitivity has no effect, try different parameters \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int GraphPSNR(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    if (argc < 6 || argc > 8)
    {
        printf("\n"
               "  GraphPSNR\n\n"
               "    <Inputfile 1>\n"
               "    <Starting Bit Rate>\n"
               "    <Ending Bit Rate>\n"
               "    <Bit Rate Step>\n"
               "    <Optional - Par Input>\n"

               "\n");
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "GraphPSNR";
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

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;

    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);
    string OutPutStr = TextfileString;
    TextfileString.append(".txt");

    OutPutStr.append("_");

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


    printf("Graph PSNR\n");
    fprintf(stderr, "Graph PSNR\n");

    char *input = argv[2];
    int FirstBitRate = atoi(argv[3]);
    int LastBitRate = atoi(argv[4]);
    int BitRateStep = atoi(argv[5]);
    char *ParFile = argv[6];

    int speed = 0;

    char  FileNameChar[255];
    FileName(input, FileNameChar);
    string InputName = FileNameChar;
    InputName.resize(InputName.length() - 4, ' ');
    OutPutStr.append(InputName.c_str());
    string ParFileOut = OutPutStr;
    OutPutStr.append("_TBR");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    double PSNRArr[100];
    double SSIMArr[100];
    double DataRateArr[100];
    unsigned int EncTimeArr[100];
    unsigned int DecTimeArr[100];


    char *CompressString = "Graph PSNR";

    if (argc > 6)
    {
        opt = InPutSettings(ParFile);
    }

    opt.TargetBandwidth = FirstBitRate;

    ///////////output Par file////////////////////
    ParFileOut.append("_ParameterFile.txt");
    char ParFileOutChar[255];
    snprintf(ParFileOutChar, 255, "%s", ParFileOut.c_str());
    OutPutSettings(ParFileOutChar, opt);
    ///////////////////////////////////////////////

    int x = 0;
    int DoONce = 0;

    while (opt.TargetBandwidth <= LastBitRate)
    {
        if (opt.TargetBandwidth == LastBitRate && DoONce == 0)
        {
            DoONce = 1;
        }

        string OutPutStr2 = OutPutStr;
        char TBChar[8];
        itoa_custom(opt.TargetBandwidth, TBChar, 10);
        OutPutStr2.append(TBChar);
        string OutPutStr3 = OutPutStr2;
        OutPutStr3.append("_Dec");
        OutPutStr2.append(".ivf");
        OutPutStr3.append(".ivf");

        char outputChar[255];
        snprintf(outputChar, 255, "%s", OutPutStr2.c_str());

        char outputChar2[255];
        snprintf(outputChar2, 255, "%s", OutPutStr3.c_str());

        EncTimeArr[x] = TimeCompressIVFtoIVF(input, outputChar , speed, opt.TargetBandwidth, opt, CompressString, 0);

        if (EncTimeArr[x] == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        DecTimeArr[x] = TimeDecompressIVFtoIVF(outputChar, outputChar2);

        double ssimnumber = 0;
        PSNRArr[x] = IVFPSNR(input, outputChar, 0, 2, 1, ssimnumber);
        SSIMArr[x] = ssimnumber;
        DataRateArr[x] = IVFDataRate(outputChar, 1);
        x++;



        opt.TargetBandwidth = opt.TargetBandwidth + BitRateStep;

        if (opt.TargetBandwidth > LastBitRate && DoONce == 0)
        {
            opt.TargetBandwidth = LastBitRate;
            DoONce = 1;
        }

    }

    int ArrSize = x;
    x = 0;

    printf("\n\n");
    fprintf(stderr, "\n\n");

    while (x < ArrSize)
    {
        printf(" DataRate: %.2f PSNR: %.2f SSIM: %.2f EncodeTime: %i \n DecodeTime: %i\n", DataRateArr[x], PSNRArr[x], SSIMArr[x], EncTimeArr[x], DecTimeArr[x]);
        fprintf(stderr, " DataRate: %.2f PSNR: %.2f SSIM: %.2f EncodeTime: %i DecodeTime: %i\n", DataRateArr[x], PSNRArr[x], SSIMArr[x], EncTimeArr[x], DecTimeArr[x]);
        x++;
    }

    x = 0;

    printf("\n\nDataRate\n\n");
    fprintf(stderr, "\n\nDataRate\n\n");

    while (x < ArrSize)
    {
        printf("%.2f\n", DataRateArr[x]);
        fprintf(stderr, " %.2f\n", DataRateArr[x]);
        x++;
    }

    x = 0;

    printf("\nPSNR\n\n");
    fprintf(stderr, "\nPSNR\n\n");

    while (x < ArrSize)
    {
        printf("%.2f\n", PSNRArr[x]);
        fprintf(stderr, " %.2f\n", PSNRArr[x]);
        x++;
    }

    x = 0;

    printf("\nSSIM\n\n");
    fprintf(stderr, "\nSSIM\n\n");

    while (x < ArrSize)
    {
        printf("%.2f\n", SSIMArr[x]);
        fprintf(stderr, " %.2f\n", SSIMArr[x]);
        x++;
    }

    x = 0;

    printf("\nEncodeTime\n\n");
    fprintf(stderr, "\nEncodeTime\n\n");

    while (x < ArrSize)
    {
        printf("%i\n", EncTimeArr[x]);
        fprintf(stderr, " %i\n", EncTimeArr[x]);
        x++;
    }

    x = 0;

    printf("\nDecodeTime\n\n");
    fprintf(stderr, "\nDecodeTime\n\n");

    while (x < ArrSize)
    {
        printf("%i\n", DecTimeArr[x]);
        fprintf(stderr, " %i\n", DecTimeArr[x]);
        x++;
    }

    fclose(fp);
    return 4;

}
int PostProcessorWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "AllowDF";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  PostProcessorWorks \n\n"
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

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);
    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "PostProcessorWorks";
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
    WorkingDir4.append("PostProcOutput.ivf");

    char PostProcOutFile[255];

    snprintf(PostProcOutFile, 255, "%s", WorkingDir4.c_str());

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

    printf("Post Processor Works\n");
    fprintf(stderr, "Post Processor Works\n");

    printf("\n");
    fprintf(stderr, "\n");

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

    int n = 0;
    int i = 0;
    double PSNRArr[50];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

    int PSNRToggle = PSNRSelect(input, PostProcOutFile);

    int deblock_level = 0;
    int flags = 0;
    int noise_level = 0;
    int countme = 0;
    double ssim = 0;

    printf("\nCaculating PSNR: ONYXD_NOFILTERING DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
    fprintf(stderr, "\nCaculating PSNR: ONYXD_NOFILTERING DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
    PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile, PSNRToggle, 0, 1, deblock_level, 0, flags, ssim);
    countme++;

    flags++;
    printf("\nCaculating PSNR: ONYXD_DEBLOCK DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
    fprintf(stderr, "\nCaculating PSNR: ONYXD_DEBLOCK DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
    PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile, PSNRToggle, 0, 1, deblock_level, noise_level, flags, ssim);
    countme++;
    flags++;

    while (deblock_level != 16)
    {
        printf("\nCaculating PSNR: ONYXD_DEMACROBLOCK DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        fprintf(stderr, "\nCaculating PSNR: ONYXD_DEMACROBLOCK DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile, PSNRToggle, 0, 1, deblock_level, 0, flags, ssim);
        countme++;
        deblock_level++;
    }

    flags = flags + 2;
    deblock_level = 0;
    noise_level = 0;

    while (noise_level != 8)
    {
        printf("\nCaculating PSNR: ONYXD_ADDNOISE DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        fprintf(stderr, "\nCaculating PSNR: ONYXD_ADDNOISE DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile, PSNRToggle, 0, 1, deblock_level, noise_level, flags, ssim);
        countme++;

        noise_level++;
    }

    int x = 0;
    cout << "\n";

    int TestFail = 0;
    int TenPercent = 0;

    if (PSNRArr[0] != PSNRArr[1])
    {
        printf("ONYXD_DEBLOCK PSNR: %4.2f != ONYXD_NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);
        fprintf(stderr, "ONYXD_DEBLOCK PSNR: %f != ONYXD_NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);

    }
    else
    {
        printf("ONYXD_DEBLOCK PSNR: %4.2f == ONYXD_NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
        fprintf(stderr, "ONYXD_DEBLOCK PSNR: %4.2f  == ONYXD_NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
        TestFail = 1;
    }

    printf("\n");
    fprintf(stderr, "\n");

    x = 2;

    while (x < 18)
    {
        if (PSNRArr[0] != PSNRArr[x])
        {
            printf("ONYXD_DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f != ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "ONYXD_DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f != ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);

        }
        else
        {
            printf("ONYXD_DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f == ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "ONYXD_DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f == ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            TestFail = 1;
        }

        x++;
    }

    printf("\n");
    fprintf(stderr, "\n");

    while (x < 26)
    {
        if (PSNRArr[0] != PSNRArr[x])
        {
            printf("ONYXD_ADDNOISE NoiseLevel %*i PSNR: %4.2f != ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "ONYXD_ADDNOISE NoiseLevel %*i PSNR: %4.2f != ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);

        }
        else
        {
            printf("ONYXD_ADDNOISE NoiseLevel %*i PSNR: %4.2f == ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "ONYXD_ADDNOISE NoiseLevel %*i PSNR: %4.2f == ONYXD_NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            TestFail = 1;
        }

        x++;
    }

    /////////////////////////////

    printf("\n");
    fprintf(stderr, "\n");

    float PSNRPercent = absFloat(PSNRArr[1] - PSNRArr[0]) / PSNRArr[0];

    if (PSNRPercent < 0.1)
    {
        printf("DEBLOCK PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);
        fprintf(stderr, "DEBLOCK PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);

    }
    else
    {
        printf("DEBLOCK PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
        fprintf(stderr, "DEBLOCK PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
        TenPercent = 1;
    }

    printf("\n");
    fprintf(stderr, "\n");

    x = 2;

    while (x < 18)
    {
        PSNRPercent = absFloat(PSNRArr[x] - PSNRArr[0]) / PSNRArr[0];

        if (PSNRPercent < 0.1)
        {
            printf("DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);

        }
        else
        {
            printf("DEMACROBLOCK DeblockLvl %*i PSNR: %4.2f not within 10%% of NOFILTER PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "DEMACROBLOCK DeblockLvl %*i PSNR: %4.2f not within 10%% of NOFILTER PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            TenPercent = 1;
        }

        x++;
    }

    printf("\n");
    fprintf(stderr, "\n");

    while (x < 26)
    {
        PSNRPercent = absFloat(PSNRArr[x] - PSNRArr[0]) / PSNRArr[0];

        if (PSNRPercent < 0.1)
        {
            printf("ADDNOISE NoiseLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "ADDNOISE NoiseLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);

        }
        else
        {
            printf("ADDNOISE NoiseLevel %*i PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            fprintf(stderr, "ADDNOISE NoiseLevel %*i PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            TenPercent = 1;
        }

        x++;
    }

    if (TestFail == 0)
    {
        if (TenPercent == 0)
        {
            printf("\n All Post Processing Values Return Different PSNRs and are within 10%% of original PSNR - Passed\n");
            fprintf(stderr, "\n All Post Processing Values Return Different PSNRs and are within 10%% of original PSNR - Passed\n");
        }
        else
        {
            printf("\n All Post Processing Values Return Different PSNRs but are not all within 10%% of original PSNR - Passed\n");
            fprintf(stderr, "\n All Post Processing Values Return Different PSNRs but are not all within 10%% of original PSNR - Passed\n");
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    printf("\n Not All Post Processing Values Return Different PSNRs - Failed\n");
    fprintf(stderr, "\n Not All Post Processing Values Return Different PSNRs - Failed\n");

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int PreProcessorWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "NoiseSensitivity";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  PreProcessorWorks \n\n"
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

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "PreProcessorWorks";
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
    WorkingDir4.append("NoiseSenseOut1");

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

    printf("Pre Processor Works test\n");
    fprintf(stderr, "Pre Processor Works test\n");

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

    int Noise = 0;
    long File2bytes[7];
    double PSNRArr[7];
    int doOnce = 1;
    int PSNRToggle;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        while (Noise != 7)
        {
            char num[20];
            itoa_custom(Noise, num, 10);

            string WorkingDir5 = WorkingDir4;
            WorkingDir5.append(num);
            WorkingDir5.append(".ivf");

            char NoiseSenseOut[255];
            snprintf(NoiseSenseOut, 255, "%s", WorkingDir5.c_str());

            if (doOnce == 1)
            {
                PSNRToggle = PSNRSelect(input, NoiseSenseOut);
                doOnce = 0;
            }

            printf("\n");
            fprintf(stderr, "\n");

            double ssim = 0;
            PSNRArr[Noise] = IVFPSNR(input, NoiseSenseOut, PSNRToggle, 0, 1, ssim);
            File2bytes[Noise] = FileSize(NoiseSenseOut);

            Noise++;
        }
    }
    else
    {
        while (Noise != 7)
        {

            char num[20];
            itoa_custom(Noise, num, 10);

            string WorkingDir5 = WorkingDir4;
            WorkingDir5.append(num);
            WorkingDir5.append(".ivf");

            char NoiseSenseOut[255];
            snprintf(NoiseSenseOut, 255, "%s", WorkingDir5.c_str());

            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.NoiseSensitivity = Noise;

                if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise) == -1)
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
                opt.NoiseSensitivity = Noise;

                if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise) == -1)
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
                opt.NoiseSensitivity = Noise;

                if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise) == -1)
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
                opt.NoiseSensitivity = Noise;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.NoiseSensitivity = Noise;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (TestType != 2)
            {
                if (doOnce == 1)
                {
                    PSNRToggle = PSNRSelect(input, NoiseSenseOut);
                    doOnce = 0;
                }

                printf("\n");
                fprintf(stderr, "\n");

                double ssim = 0;
                PSNRArr[Noise] = IVFPSNR(input, NoiseSenseOut, PSNRToggle, 0, 1, ssim);
                File2bytes[Noise] = FileSize(NoiseSenseOut);
            }

            Noise++;
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

    //checks 0v1 | 1v2 | 2v3 | 3v4 | 4v5 | 5v6 Could be modified to check all against eachother if wanted.
    int n = 0;

    while (n != 6)
    {
        if (PSNRArr[n] == PSNRArr[n+1] && File2bytes[n] == File2bytes[n+1])
        {
            printf("/n Noise %i PSNR %f == Noise %i PSNR %f and Noise %i File Size: %i == Noise %i File Size: %i - Failed\n", n, PSNRArr[n], n + 1, PSNRArr[n+1], n, File2bytes[n], n + 1, File2bytes[n+1]);
            fprintf(stderr, "/n Noise %i PSNR %f == Noise %i PSNR %f and Noise %i File Size: %i == Noise %i File Size: %i - Failed\n", n, PSNRArr[n], n + 1, PSNRArr[n+1], n, File2bytes[n], n + 1, File2bytes[n+1]);

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        n++;
    }

    printf("\n PSNRs have Differnt Values - Passed\n");
    fprintf(stderr, "\n PSNRs have Differnt Values - Passed\n");
    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 1;
}
int RandComp(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "RandomInput";

    if (argc != 3)
    {
        printf("\n"
               "  RandComp\n\n"
               "    <Inputfile>\n\n"
              );
        return 0;
    }

    char *input = argv[2];
    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "RandComp";
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

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);

    char InputFileName[255];
    FileName(input, InputFileName);
    string InputFileNameStr = InputFileName;

    if (InputFileNameStr.substr(InputFileNameStr.length() - 4, 4).compare(".ivf") == 0) //if file extension is present remove it
        WorkingDir4.append(InputFileNameStr.substr(0, InputFileNameStr.length() - 4));
    else
        WorkingDir4.append(InputFileName);

    WorkingDir4.append("-RandComp.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("RandCompOutPut.txt");

    char RandComp[255];
    char RandCompParOut[255];

    snprintf(RandComp, 255, "%s", WorkingDir4.c_str());
    snprintf(RandCompParOut, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr);
    TextfileString.append(MyDir);
    TextfileString.append(".txt");

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



    VP8_CONFIG opt = VP8RandomParms(opt, input, 1);
    int Mode = opt.Mode;

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;
        //OutPutSettings(RandCompParOut,opt);
        int speed = opt.MultiThreaded;
        int BitRate = opt.TargetBandwidth;

        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1) == -1)
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
        //OutPutSettings(RandCompParOut,opt);
        int speed = opt.MultiThreaded;
        int BitRate = opt.TargetBandwidth;

        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1) == -1)
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
        //OutPutSettings(RandCompParOut,opt);
        int speed = opt.MultiThreaded;
        int BitRate = opt.TargetBandwidth;

        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1) == -1)
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
        int speed = opt.MultiThreaded;
        int BitRate = opt.TargetBandwidth;
        opt.Mode = MODE_SECONDPASS_BEST;

        //OutPutSettings(RandCompParOut,opt);
        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    return 5;
}
int TwoPassVsTwoPassBest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "AllowDF";
    char *input = argv[2];

    if (!(argc == 4 || argc == 5))
    {
        printf(
            "  TwoPassVsTwoPassBest \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n");
        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "TwoPassVsTwoPassBest";
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
    WorkingDir4.append("TwoPassOutput1.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("TwoPassBestOutput2.ivf");

    char TwoPassOutFile[255];
    char TwoPassBestOutFile[255];

    snprintf(TwoPassOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(TwoPassBestOutFile, 255, "%s", WorkingDir5.c_str());

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
    int BitRate = atoi(argv[3]);

    printf("Two Pass Vs Two Pass Best Test\n");
    fprintf(stderr, "TwoPassVsTwoPassBest Test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 5)
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

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {

        opt.Mode = MODE_SECONDPASS;

        if (CompressIVFtoIVF(input, TwoPassOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.Mode = MODE_SECONDPASS_BEST;

        if (CompressIVFtoIVF(input, TwoPassBestOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
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

    cout << "\n";

    float GoodSize = IVFDataRate(TwoPassOutFile, 1);
    float BestSize = IVFDataRate(TwoPassBestOutFile, 1);

    int PSNRToggle = PSNRSelect(input, TwoPassOutFile);

    double PSNRG;
    double PSNRB;

    double ssim = 0;
    PSNRG = IVFPSNR(input, TwoPassOutFile, PSNRToggle, 0, 1, ssim);
    PSNRB = IVFPSNR(input, TwoPassBestOutFile, PSNRToggle, 0, 1, ssim);

    float PSRNPerc = absFloat((PSNRB - PSNRG) / PSNRG) * 100.00;
    float BRPerc = absFloat((GoodSize - BestSize) / BestSize) * 100.00;
    int Pass = 0;

    if (PSNRB == PSNRG)
    {
        printf("\n\n No Difference Between Two Pass and Two Pass Best PSNRs");
        fprintf(stderr, "\n\n No Difference Between Two Pass and Two Pass Best PSNRs");

    }

    if (PSNRB > PSNRG)
    {
        printf("\n\n Two Pass Best returned a %f%% higher PSNR", PSRNPerc);
        fprintf(stderr, "\n\n Two Pass Best returned a %f%% higher PSNR", PSRNPerc);
        Pass = 1;
    }

    if (PSNRB < PSNRG)
    {
        printf("\n\n Two Pass Best returned a %f%% lower PSNR", PSRNPerc);
        fprintf(stderr, "\n\n Two Pass Best returned a %f%% lower PSNR", PSRNPerc);
    }

    if (BestSize == GoodSize)
    {
        printf("\n\n No Difference Between Two Pass and Two Pass Best Data Rates");
        fprintf(stderr, "\n\n No Difference Between Two Pass and Two Pass Best Data Rates");
    }

    if (BestSize > GoodSize)
    {
        printf("\n Two Pass Best returned a %f%% higher Data Rate \n ", BRPerc);
        fprintf(stderr, "\n Two Pass Best returned a %f%% higher Data Rate \n ", BRPerc);
    }

    if (BestSize < GoodSize)
    {
        printf("\n Two Pass Best returned a %f%% lower Data Rate\n ", BRPerc);
        fprintf(stderr, "\n Two Pass Best returned a %f%% lower Data Rate\n ", BRPerc);
        Pass = 1;
    }

    if (Pass == 1)
    {
        printf("\n Passed\n ");
        fprintf(stderr, "\n Passed\n ");

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    printf("\n Failed\n ");
    fprintf(stderr, "\n Failed\n ");

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

int UnderShoot(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "UnderShootPct";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  UnderShoot \n\n"
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
    char *MyDir = "UnderShoot";
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
    WorkingDir4.append("UnderShoot10Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("UnderShoot100Output.ivf");

    char UnderShoot10[255];
    char UnderShoot100[255];

    snprintf(UnderShoot10, 255, "%s", WorkingDir4.c_str());
    snprintf(UnderShoot100, 255, "%s", WorkingDir5.c_str());

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

    printf("Undershoot test\n");
    fprintf(stderr, "Undershoot test\n");

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
            opt.UnderShootPct = 10;

            if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.UnderShootPct = 100;

            if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100) == -1)
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
            opt.UnderShootPct = 10;

            if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.UnderShootPct = 100;

            if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100) == -1)
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
            opt.UnderShootPct = 10;

            if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.UnderShootPct = 100;

            if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100) == -1)
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
            opt.UnderShootPct = 10;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.UnderShootPct = 100;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.UnderShootPct = 10;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.UnderShootPct = 100;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100) == -1)
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

    long File1bytes = FileSize(UnderShoot10);
    long File2bytes = FileSize(UnderShoot100);

    if (File1bytes < File2bytes)
    {
        printf("\n File size 1:%i < File size 2:%i : Passed \n", File1bytes , File2bytes);
        fprintf(stderr, "\n File size 1:%i < File size 2:%i : Passed \n", File1bytes , File2bytes);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (File1bytes == File2bytes)
    {
        printf("No effect try different file \n");
        fprintf(stderr, "No effect try different file \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

    printf("\n File size 1:%i > File size 2:%i : Failed\n", File1bytes , File2bytes);
    fprintf(stderr, "\n File size 1:%i > File size 2:%i : Failed \n", File1bytes , File2bytes);

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

//--------------------------------------------New VP8 Tests----------------------------------------------------------------
int AllowLagTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "Allow Lag";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  AllowLag \n\n"
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

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "AllowLag";
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

        FileStream.close();
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("AllowLagOnOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("AllowLagOffOutput.ivf");

    char AllowLagon[255];
    char AllowLagoff[255];

    snprintf(AllowLagon, 255, "%s", WorkingDir4.c_str());
    snprintf(AllowLagoff, 255, "%s", WorkingDir5.c_str());

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

    char *input2 = argv[5];

    printf("Allow Lag test\n");
    fprintf(stderr, "Allow Lag test\n");

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
    opt.LagInFrames = 10;

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
            opt.AllowLag = 0;

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowLag = 1;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.AllowLag = 0;

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AllowLag = 1;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.AllowLag = 0;

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowLag = 1;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1) == -1)
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
            opt.AllowLag = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.AllowLag = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1) == -1)
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

    int lngRC = CompIVF(AllowLagoff, AllowLagon);

    if (lngRC >= 0)
    {
        printf("\n\nPass: Allow Lag has an effect - Files differ at frame: %i\n", lngRC);
        fprintf(stderr, "\n\nPass: Allow Lag has an effect - Files differ at frame: %i\n", lngRC);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (lngRC == -1)
    {
        printf("\nFail: Allow Lag has no effect - Files are identical\n");
        fprintf(stderr, "\nFail: Allow Lag has no effect - Files are identical\n");
    }


    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int AltFreqTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

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
            "	 <Optional Settings File>\n"
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

    printf("Alt Freq Test\n");
    fprintf(stderr, "Alt Freq Test\n");

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

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AltFreq = AltFreq2Val;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val) == -1)
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

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AltFreq = AltFreq2Val;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val) == -1)
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

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AltFreq = AltFreq2Val;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val) == -1)
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

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltFreq = AltFreq2Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val) == -1)
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

            if (CompressIVFtoIVF(input, AltFreq1, speed, BitRate, opt, CompressString, AltFreq1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltFreq = AltFreq2Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AltFreq2, speed, BitRate, opt, CompressString, AltFreq2Val) == -1)
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
int EncoderBreakOutTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "EncoderBreakOut";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  EncoderBreakOutTest \n\n"
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

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "EncoderBreakOut";
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
        //cout << "\n" << CreateDir2.c_str() << "\n";
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
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;

    string WorkingDir8 = WorkingDirString;
    string WorkingDir9 = WorkingDirString;
    string WorkingDir10 = WorkingDirString;
    string WorkingDir11 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("EncBreakOut0.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("EncBreakOut100.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("EncBreakOut500.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("EncBreakOut1000.ivf");

    WorkingDir8.append(slashCharStr);
    WorkingDir8.append("EncBreakOut0_Dec.ivf");
    WorkingDir9.append(slashCharStr);
    WorkingDir9.append("EncBreakOut100_Dec.ivf");
    WorkingDir10.append(slashCharStr);
    WorkingDir10.append("EncBreakOut500_Dec.ivf");
    WorkingDir11.append(slashCharStr);
    WorkingDir11.append("EncBreakOut1000_Dec.ivf");

    char EncBreakOut0[255];
    char EncBreakOut100[255];
    char EncBreakOut500[255];
    char EncBreakOut1000[255];

    char EncBreakOut0_Dec[255];
    char EncBreakOut100_Dec[255];
    char EncBreakOut500_Dec[255];
    char EncBreakOut1000_Dec[255];

    snprintf(EncBreakOut0, 255, "%s", WorkingDir4.c_str());
    snprintf(EncBreakOut100, 255, "%s", WorkingDir5.c_str());
    snprintf(EncBreakOut500, 255, "%s", WorkingDir6.c_str());
    snprintf(EncBreakOut1000, 255, "%s", WorkingDir7.c_str());

    snprintf(EncBreakOut0_Dec, 255, "%s", WorkingDir8.c_str());
    snprintf(EncBreakOut100_Dec, 255, "%s", WorkingDir9.c_str());
    snprintf(EncBreakOut500_Dec, 255, "%s", WorkingDir10.c_str());
    snprintf(EncBreakOut1000_Dec, 255, "%s", WorkingDir11.c_str());

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


    printf("Encoder Break Out test\n");
    fprintf(stderr, "Encoder Break Out test\n");

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
            opt.EncodeBreakout = 0;

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.EncodeBreakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.EncodeBreakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.EncodeBreakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000) == -1)
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
            opt.EncodeBreakout = 0;

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.EncodeBreakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.EncodeBreakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.EncodeBreakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000) == -1)
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
            opt.EncodeBreakout = 0;

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.EncodeBreakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.EncodeBreakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.EncodeBreakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000) == -1)
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
            opt.EncodeBreakout = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 100;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 500;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 1000;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.EncodeBreakout = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 100;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 500;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 1000;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000) == -1)
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

    printf("\nDecoding EncBreakOut0");
    fprintf(stderr, "\nDecoding EncBreakOut0");
    DecompressIVFtoIVF(EncBreakOut0, EncBreakOut0_Dec);
    printf("\n\nDecoding EncBreakOut100");
    fprintf(stderr, "\n\nDecoding EncBreakOut100");
    DecompressIVFtoIVF(EncBreakOut100, EncBreakOut100_Dec);
    printf("\n\nDecoding EncBreakOut500");
    fprintf(stderr, "\n\nDecoding EncBreakOut500");
    DecompressIVFtoIVF(EncBreakOut500, EncBreakOut500_Dec);
    printf("\n\nDecoding EncBreakOut1000");
    fprintf(stderr, "\n\nDecoding EncBreakOut1000");
    DecompressIVFtoIVF(EncBreakOut1000, EncBreakOut1000_Dec);

    double PSNR0;
    double PSNR100;
    double PSNR500;
    double PSNR1000;
    double ssim = 0;

    PSNR0 = IVFPSNR(input, EncBreakOut0, 0, 0, 1, ssim);
    PSNR100 = IVFPSNR(input, EncBreakOut100, 0, 0, 1, ssim);
    PSNR500 = IVFPSNR(input, EncBreakOut500, 0, 0, 1, ssim);
    PSNR1000 = IVFPSNR(input, EncBreakOut1000, 0, 0, 1, ssim);

#if defined(_WIN32)
    double dB1 = absDouble(10 * log(PSNR0 / PSNR100));
    double dB2 = absDouble(10 * log(PSNR100 / PSNR500));
    double dB3 = absDouble(10 * log(PSNR500 / PSNR1000));
#elif defined(linux)
    double dB1 = absDouble(10 * log(PSNR0 / PSNR100));
    double dB2 = absDouble(10 * log(PSNR100 / PSNR500));
    double dB3 = absDouble(10 * log(PSNR500 / PSNR1000));
#elif defined(__APPLE__)
    double dB1 = absDouble(10 * log(PSNR0 / PSNR100));
    double dB2 = absDouble(10 * log(PSNR100 / PSNR500));
    double dB3 = absDouble(10 * log(PSNR500 / PSNR1000));
#elif defined(__POWERPC__)
    double dB1 = absDouble(10 * log(PSNR0 / PSNR100));
    double dB2 = absDouble(10 * log(PSNR100 / PSNR500));
    double dB3 = absDouble(10 * log(PSNR500 / PSNR1000));
#endif

    cout << "\ndB1: " << dB1 << "\n";
    cout << "dB2: " << dB2 << "\n";
    cout << "dB3: " << dB3 << "\n\n";

    cerr << "\ndB1: " << dB1 << "\n";
    cerr << "dB2: " << dB2 << "\n";
    cerr << "dB3: " << dB3 << "\n\n";

    int Pass = 1;
    int IndCount = 0;

    if (dB1 <= 2)
    {
        printf("\nPass: Encoder Breakout 0 PSNR and Encoder Breakout 100 PSNR are within 2 dB: %4f dB\n", dB1);
        fprintf(stderr, "\nPass: Encoder Breakout 0 PSNR and Encoder Breakout 100 PSNR are within 2 dB: %4f dB\n", dB1);
    }

    if (dB1 >= 5)
    {
        printf("\nFail: Encoder Breakout 0 PSNR and Encoder Breakout 100 PSNR not within 5 dB: %4f dB\n", dB1);
        fprintf(stderr, "\nFail: Encoder Breakout 0 PSNR and Encoder Breakout 100 PSNR not within 5 dB: %4f dB\n", dB1);
        Pass = 0;
    }

    if (dB1 > 2 && dB1 < 5)
    {
        printf("\nIndeterminate: Encoder Breakout 0 PSNR and Encoder Breakout 100 PSNR greater than 2 dB but less than 5 dB: %4f dB\n", dB1);
        fprintf(stderr, "\nIndeterminate: Encoder Breakout 0 PSNR and Encoder Breakout 100 PSNR greater than 2 dB but less than 5 dB: %4f dB\n", dB1);
        IndCount++;
    }

    if (dB2 <= 2)
    {
        printf("\nPass: Encoder Breakout 100 PSNR and Encoder Breakout 500 PSNR are within 2 dB: %4f dB\n", dB2);
        fprintf(stderr, "\nPass: Encoder Breakout 100 PSNR and Encoder Breakout 500 PSNR are within 2 dB: %4f dB\n", dB2);
    }

    if (dB2 >= 5)
    {
        printf("\nFail: Encoder Breakout 100 PSNR and Encoder Breakout 500 PSNR not within 5 dB: %4f dB\n", dB2);
        fprintf(stderr, "\nFail: Encoder Breakout 100 PSNR and Encoder Breakout 500 PSNR not within 5 dB: %4f dB\n", dB2);
        Pass = 0;
    }

    if (dB2 > 2 && dB2 < 5)
    {
        printf("\nIndeterminate: Encoder Breakout 100 PSNR and Encoder Breakout 500 PSNR greater than 2 dB but less than 5 dB: %4f dB\n", dB2);
        fprintf(stderr, "\nIndeterminate: Encoder Breakout 100 PSNR and Encoder Breakout 500 PSNR greater than 2 dB but less than 5 dB: %4f dB\n", dB2);
        IndCount++;
    }

    if (dB3 <= 2)
    {
        printf("\nPass: Encoder Breakout 500 PSNR and Encoder Breakout 1000 PSNR are within 2 dB: %4f dB\n", dB3);
        fprintf(stderr, "\nPass: Encoder Breakout 500 PSNR and Encoder Breakout 1000 PSNR are within 2 dB: %4f dB\n", dB3);
    }

    if (dB3 >= 5)
    {
        printf("\nFail: Encoder Breakout 500 PSNR and Encoder Breakout 1000 PSNR not within 5 dB: %4f dB\n", dB3);
        fprintf(stderr, "\nFail: Encoder Breakout 500 PSNR and Encoder Breakout 1000 PSNR not within 5 dB:\n %4f dB\n", dB3);
        Pass = 0;
    }

    if (dB3 > 2 && dB3 < 5)
    {
        printf("\nIndeterminate: Encoder Breakout 500 PSNR and Encoder Breakout 1000 PSNR greater than 2 dB but less than 5 dB: %4f dB\n", dB3);
        fprintf(stderr, "\nIndeterminate: Encoder Breakout 500 PSNR and Encoder Breakout 1000 PSNR greater than 2 dB but less than 5 dB: %4f dB\n", dB3);
        IndCount++;
    }

    printf("\n");
    fprintf(stderr, "\n");

    if (Pass == 1)
    {
        if (IndCount == 0)
        {
            printf("\nPass: All Encoder Breakout PSNRs are within 2 dB of one another\n");
            fprintf(stderr, "\nPass: All Encoder Breakout PSNRs are within 2 dB of one another\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nIndeterminate: Not all Encoder Breakout PSNRs are with in 2 dB of one another but all are less than 5 dB of one another\n");
            fprintf(stderr, "\nIndeterminate: Not all Encoder Breakout PSNRs are with in 2 dB of one another but all are less than 5 dB of one another\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    printf("\nFail: Not all Encoder Breakout PSNRs are less than 5 dB of one another\n");
    fprintf(stderr, "\nFail: Not all Encoder Breakout PSNRs are less than 5 dB of one another\n");
    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int FixedQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "FixedQ";
    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  FixedQ \n\n"
            "    <inputfile>\n"
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
            "	 <Optional Settings File>\n"
        );
        return 0;
    }


    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "FixedQ";
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
    WorkingDir4.append("FixedQ_");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("FixedQ_");

    WorkingDir4.append(argv[5]);
    WorkingDir5.append(argv[6]);

    WorkingDir4.append(".ivf");
    WorkingDir5.append(".ivf");

    char FixedQ1[255];
    char FixedQ2[255];

    snprintf(FixedQ1, 255, "%s", WorkingDir4.c_str());
    snprintf(FixedQ2, 255, "%s", WorkingDir5.c_str());

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
    int FixedQ1Int = atoi(argv[5]);
    int FixedQ2Int = atoi(argv[6]);

    printf("Fixed Q Test");
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
            opt.FixedQ = FixedQ1Int;

            if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.FixedQ = FixedQ2Int;

            if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int) == -1)
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
            opt.FixedQ = FixedQ1Int;

            if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.FixedQ = FixedQ2Int;

            if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int) == -1)
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
            opt.FixedQ = FixedQ1Int;

            if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.FixedQ = FixedQ2Int;

            if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int) == -1)
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
            opt.FixedQ = FixedQ1Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.FixedQ = FixedQ2Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.FixedQ = FixedQ1Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.FixedQ = FixedQ2Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int) == -1)
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

    printf("\nComparing Files: \n");
    fprintf(stderr, "\nComparing Files: \n");

    int CompCheck = CompIVF(FixedQ1, FixedQ2);

    if (CompCheck == - 1)
    {
        printf("\nFiles are identical Fixed Q has no effect. : Failed \n");
        fprintf(stderr, "\nFiles are identical Fixed Q has no effect. : Failed \n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    printf("\nFiles are not identical Fixed Q Test: Passed \n");
    fprintf(stderr, "\nFiles are not identical Fixed Q Test: Passed \n");
    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 1;
}

int LagInFramesTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Lag In Frames";

    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  LagInFrames \n\n"
            "    <inputfile>\n"
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
            "	 <Optional Settings File>\n"
        );

        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "LagInFrames";
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

        FileStream.close();
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;


    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("LagInFrames1Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("LagInFrames2Output.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("LagInFrames0Output.ivf");

    char LagInFrames0[255];
    char LagInFrames1[255];
    char LagInFrames2[255];

    snprintf(LagInFrames0, 255, "%s", WorkingDir6.c_str());
    snprintf(LagInFrames1, 255, "%s", WorkingDir4.c_str());
    snprintf(LagInFrames2, 255, "%s", WorkingDir5.c_str());

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

    int LagInFrames1Val = atoi(argv[5]);
    int LagInFrames2Val = atoi(argv[6]);

    printf("Lag In Frames test\n");
    fprintf(stderr, "Lag In Frames test\n");

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

    if (LagInFrames1Val > 25 || LagInFrames2Val > 25 || LagInFrames2Val < 0 || LagInFrames1Val < 0)
    {
        printf("\nLag in Frames settings must be between 0 and 25.\n");
        fprintf(stderr, "\nLag in Frames settings must be between 0 and 25.\n");


        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

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
            opt.AllowLag = 0;
            opt.LagInFrames = LagInFrames1Val;

            if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames1Val;

            if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames2Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames2Val;

            if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val) == -1)
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
            opt.AllowLag = 0;
            opt.LagInFrames = LagInFrames1Val;

            if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }


            opt.Mode = MODE_GOODQUALITY;
            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames1Val;

            if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames2Val;

            if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val) == -1)
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
            opt.AllowLag = 0;
            opt.LagInFrames = LagInFrames1Val;

            if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames1Val;

            if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames2Val;

            if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val) == -1)
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
            opt.AllowLag = 0;
            opt.LagInFrames = LagInFrames1Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames1Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames2Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.AllowLag = 0;
            opt.LagInFrames = LagInFrames1Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames1Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.LagInFrames = LagInFrames2Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val) == -1)
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

    double ssim = 0;
    double LagInFrames0PSNR = IVFPSNR(input, LagInFrames0, 0, 0, 1, ssim);
    double LagInFrames1PSNR = IVFPSNR(input, LagInFrames1, 0, 0, 1, ssim);
    double LagInFrames2PSNR = IVFPSNR(input, LagInFrames2, 0, 0, 1, ssim);

    double TenPer0 = LagInFrames0PSNR / 10;
    double TenPer1 = LagInFrames1PSNR / 10;
    double TenPer2 = LagInFrames2PSNR / 10;

    int lngRC1 = CompIVF(LagInFrames0, LagInFrames1);
    int lngRC2 = CompIVF(LagInFrames1, LagInFrames2);

    int PSNRTally = 0;

    if (lngRC1 >= 0 && lngRC2 >= 0)
    {
        if (LagInFrames1PSNR <= (LagInFrames0PSNR + TenPer0) && LagInFrames1PSNR >= (LagInFrames0PSNR - TenPer0))
        {
            PSNRTally = 1;
            printf("\nPSNR for Lag in Frames 1 is with in 0.1 of PSNR for Lag in Frames 0\n %f < %f < %f\n", (LagInFrames0PSNR - TenPer0), LagInFrames1PSNR, (LagInFrames0PSNR + TenPer0));
            fprintf(stderr, "\nPSNR for Lag in Frames 1 is with in 0.1 of PSNR for Lag in Frames 0\n %f < %f < %f\n", (LagInFrames0PSNR - TenPer0), LagInFrames1PSNR, (LagInFrames0PSNR + TenPer0));
        }

        if (LagInFrames2PSNR <= (LagInFrames1PSNR + TenPer1) && LagInFrames2PSNR >= (LagInFrames1PSNR - TenPer1))
        {
            PSNRTally = PSNRTally + 2;
            printf("\nPSNR for Lag in Frames 2 is within 0.1 of PSNR for Lag in Frames 1\n %f < %f < %f\n", (LagInFrames1PSNR - TenPer1), LagInFrames2PSNR, (LagInFrames1PSNR + TenPer1));
            fprintf(stderr, "\nPSNR for Lag in Frames 2 is within 0.1 of PSNR for Lag in Frames 1\n %f < %f < %f\n", (LagInFrames1PSNR - TenPer1), LagInFrames2PSNR, (LagInFrames1PSNR + TenPer1));
        }

        if (PSNRTally == 1)
        {
            PSNRTally = PSNRTally + 2;
            printf("\nFail: PSNR for Lag in Frames 2 is not within 0.1 of PSNR for Lag in Frames 1\n %f < %f < %f\n", (LagInFrames1PSNR - TenPer1), LagInFrames2PSNR, (LagInFrames1PSNR + TenPer1));
            fprintf(stderr, "Fail: \nPSNR for Lag in Frames 2 is not within 0.1 of PSNR for Lag in Frames 1\n %f < %f < %f\n", (LagInFrames1PSNR - TenPer1), LagInFrames2PSNR, (LagInFrames1PSNR + TenPer1));

        }

        if (PSNRTally == 2)
        {
            printf("\nFail: PSNR for Lag in Frames 1 is not within 0.1 of PSNR for Lag in Frames 0\n %f < %f < %f\n", (LagInFrames0PSNR - TenPer0), LagInFrames1PSNR, (LagInFrames0PSNR + TenPer0));
            fprintf(stderr, "\nFail: PSNR for Lag in Frames 1 is not within 0.1 of PSNR for Lag in Frames 0\n %f < %f < %f\n", (LagInFrames0PSNR - TenPer0), LagInFrames1PSNR, (LagInFrames0PSNR + TenPer0));

        }

        if (PSNRTally == 3)
        {
            printf("\n\nPass: Differing Lag in Frames produce three different files with PSNRs falling within 0.1 of eachother.\n\n");
            fprintf(stderr, "\n\nPass: Differing Lag in Frames produce three different files with PSNRs falling within 0.1 of eachother.\n\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }

    if (lngRC1 == -1 && lngRC2 != -1)
    {
        printf("\nFail: First and Second Differing Lag in Frames Files are Identical\n");
        fprintf(stderr, "\nFail: First and Second Differing Lag in Frames Files are Identical\n");
    }

    if (lngRC2 == -1 && lngRC1 != -1)
    {
        printf("\nFail: Second and Third Differing Lag in Frame Files are Identical\n");
        fprintf(stderr, "\nFail: Second and Third Differing Lag in Frame Files are Identical\n");
    }

    if (lngRC2 == -1 && lngRC1 == -1)
    {
        printf("\nFail: First Second and Third Differing Lag in Frame Files are Identical\n");
        fprintf(stderr, "\nFail: First Second and Third Differing Lag in Frame Files are Identical\n");
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int MultiThreadedTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "MultiThreaded";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  MultiThreadedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
        );
        return 0;
    }

    char *input = argv[2];

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "MultiThreadedTest";
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
    WorkingDir4.append("MultiThreaded14Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("MultiThreaded00Output.ivf");

    char MultiThreaded14OutFile[255];
    char MultiThreaded00OutFile[255];

    snprintf(MultiThreaded14OutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(MultiThreaded00OutFile, 255, "%s", WorkingDir5.c_str());

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

    int MultiThreaded = 0;
    int BitRate = atoi(argv[4]);

    unsigned int Time1;
    unsigned int Time2;

    unsigned int CoreCount = ON2_GetProcCoreCount();

    if (!(CoreCount > 1))
    {
        printf("\nMulti Core not found: Test Aborted: %i\n", ON2_GetProcCoreCount());
        fclose(fp);
        return 0;
    }

    int Mode = atoi(argv[3]);

    if (Mode != 0 && Mode != 1)
    {
        printf(
            "  MultiThreadedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
        );
        fclose(fp);
        return 0;
    }

    printf("MultiThreaded Test\n");
    fprintf(stderr, "MultiThreaded Test\n");

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
        Time1 = MultiPlatTimeRet(MultiThreaded14OutFile);
        Time2 = MultiPlatTimeRet(MultiThreaded00OutFile);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.MultiThreaded = CoreCount;
            opt.CpuUsed = -1;
            Time1 = TimeCompressIVFtoIVF(input, MultiThreaded14OutFile, MultiThreaded, BitRate, opt, CompressString, CoreCount);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.MultiThreaded = 0;
            Time2 = TimeCompressIVFtoIVF(input, MultiThreaded00OutFile, MultiThreaded, BitRate, opt, CompressString, 0);

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
            opt.MultiThreaded = CoreCount;
            Time1 = TimeCompressIVFtoIVF(input, MultiThreaded14OutFile, MultiThreaded, BitRate, opt, CompressString, CoreCount);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.MultiThreaded = 0;
            Time2 = TimeCompressIVFtoIVF(input, MultiThreaded00OutFile, MultiThreaded, BitRate, opt, CompressString, 0);

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
        printf("\n Unknown: MultiThreaded has no effect, try different parameters \n");
        fprintf(stderr, "\n Unknown: MultiThreaded has no effect, try different parameters \n");
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
int PlayAlternate(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "PlayAlternate";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  PlayAlternate \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "	 <Optional Settings File>\n"
        );

        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "PlayAlternate";
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

        FileStream.close();
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("PlayAlternate1Output.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("PlayAlternate2Output.ivf");

    char PlayAlternate1[255];
    char PlayAlternate2[255];

    snprintf(PlayAlternate1, 255, "%s", WorkingDir4.c_str());
    snprintf(PlayAlternate2, 255, "%s", WorkingDir5.c_str());

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

    int PlayAlternate1Val = 0;
    int PlayAlternate2Val = 1;

    printf("Play Alternate test\n");
    fprintf(stderr, "Play Alternate test\n");

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
    opt.AllowLag = 1;
    opt.LagInFrames = 10;
    opt.EndUsage = 1;

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
            opt.PlayAlternate = PlayAlternate1Val;

            if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.PlayAlternate = PlayAlternate2Val;

            if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val) == -1)
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
            opt.PlayAlternate = PlayAlternate1Val;

            if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.PlayAlternate = PlayAlternate2Val;

            if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val) == -1)
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
            opt.PlayAlternate = PlayAlternate1Val;

            if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.PlayAlternate = PlayAlternate2Val;

            if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val) == -1)
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
            opt.PlayAlternate = PlayAlternate1Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.PlayAlternate = PlayAlternate2Val;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.PlayAlternate = PlayAlternate1Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.PlayAlternate = PlayAlternate2Val;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val) == -1)
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

    int lngRC = CompIVF(PlayAlternate2, PlayAlternate1);

    if (lngRC >= 0)
    {
        printf("\n\nPass: Differing Play Alternate values have an effect - Files differ at frame: %i\n", lngRC);
        fprintf(stderr, "\n\nPass: Differing Play Alternate values have an effect - Files differ at frame: %i\n", lngRC);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (lngRC == -1)
    {
        printf("\nFail: Differing Play Alternate values have no effect - Files are identical\n");
        fprintf(stderr, "\nFail: Differing Play Alternate values have no effect - Files are identical\n");
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

int SpeedTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "CpuUsed";

    if (!(argc == 6 || argc == 7))
    {
        printf(
            "  SpeedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Lag In Frames>\n"
            "	 <Optional Settings File>\n"
        );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int LagInFramesInput = atoi(argv[5]);

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "SpeedTest";
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
    WorkingDir4.append("SpeedTestGoodQ_CpuUsed");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("SpeedTestRealTime_CpuUsed");

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

    printf("Speed Test");
    fprintf(stderr, "Speed Test");

    if (Mode != 0 && Mode != 1)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    int speed = 0;
    int Fail = 0;
    int Fail2 = 0;
    int Failb = 0;
    int Fail2b = 0;
    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 7)
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
    int CompressInt = opt.CpuUsed;
    opt.TargetBandwidth = BitRate;
    opt.Mode = MODE_GOODQUALITY;
    opt.LagInFrames = LagInFramesInput;

    printf("\nLagInFrames: %i\n", LagInFramesInput);
    fprintf(stderr, "\nLagInFrames: %i\n", LagInFramesInput);

    unsigned int GoodTotalms[7];
    unsigned int RealTotalms[17];
    unsigned int RealTotalmsPos[18];

    double GoodPSNRArr[7];
    double RealPSNRArr[17];
    double RealPSNRArrPos[18];

    double ssim = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        int counter = 0;

        if (Mode == 1)
        {
            while (counter < 6)
            {
                char CounterChar[4];
                itoa_custom(counter, CounterChar, 10);

                string OutPutFile1 = WorkingDir4;
                OutPutFile1.append(CounterChar);
                OutPutFile1.append(".ivf");

                char SpeedTestGoodQ[255];
                snprintf(SpeedTestGoodQ, 255, "%s", OutPutFile1.c_str());

                GoodTotalms[counter] = MultiPlatTimeRet(SpeedTestGoodQ);
                GoodPSNRArr[counter] = IVFPSNR(input, SpeedTestGoodQ, 1, 0, 1, ssim);
                counter++;
            }
        }

        if (Mode == 0)
        {
            counter = -1;
            int counter2 = 0;

            while (counter > -17)
            {
                char CounterChar[4];
                itoa_custom(counter, CounterChar, 10);

                string OutPutFile2 = WorkingDir5;
                OutPutFile2.append(CounterChar);
                OutPutFile2.append(".ivf");

                char SpeedTestRealTime[255];
                snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                RealTotalms[counter2] = MultiPlatTimeRet(SpeedTestRealTime);
                RealPSNRArr[counter2] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, ssim);
                counter--;
                counter2++;
            }

            counter = 0;

            while (counter < 17)
            {
                char CounterChar[4];
                itoa_custom(counter, CounterChar, 10);

                string OutPutFile2 = WorkingDir5;
                OutPutFile2.append(CounterChar);
                OutPutFile2.append(".ivf");

                char SpeedTestRealTime[255];
                snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                RealTotalmsPos[counter] = MultiPlatTimeRet(SpeedTestRealTime);
                RealPSNRArrPos[counter] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, ssim);
                counter++;
            }
        }
    }
    else
    {

        int counter = 0;

        if (Mode == 1)
        {
            while (counter < 6)
            {
                char CounterChar[4];
                itoa_custom(counter, CounterChar, 10);

                string OutPutFile1 = WorkingDir4;
                OutPutFile1.append(CounterChar);
                OutPutFile1.append(".ivf");

                char SpeedTestGoodQ[255];
                snprintf(SpeedTestGoodQ, 255, "%s", OutPutFile1.c_str());

                opt.CpuUsed = counter;
                CompressInt = opt.CpuUsed;
                opt.Mode = MODE_GOODQUALITY;
                GoodTotalms[counter] = TimeCompressIVFtoIVF(input, SpeedTestGoodQ, speed, BitRate, opt, CompressString, CompressInt);

                if (GoodTotalms[counter] == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                if (TestType != 2 && TestType != 3)
                {
                    GoodPSNRArr[counter] = IVFPSNR(input, SpeedTestGoodQ, 1, 0, 1, ssim);
                }

                counter++;
            }
        }

        if (Mode == 0)
        {
            counter = -1;
            int counter2 = 0;

            while (counter > -17)
            {
                char CounterChar[4];
                itoa_custom(counter, CounterChar, 10);

                string OutPutFile2 = WorkingDir5;
                OutPutFile2.append(CounterChar);
                OutPutFile2.append(".ivf");

                char SpeedTestRealTime[255];
                snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                opt.CpuUsed = counter;
                CompressInt = opt.CpuUsed;
                opt.Mode = MODE_REALTIME;
                RealTotalms[counter2] = TimeCompressIVFtoIVF(input, SpeedTestRealTime, speed, BitRate, opt, CompressString, CompressInt);

                if (TestType != 2 && TestType != 3)
                {
                    RealPSNRArr[counter2] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, ssim);
                }

                counter--;
                counter2++;
            }

            counter = 0;

            while (counter < 17)
            {
                char CounterChar[4];
                itoa_custom(counter, CounterChar, 10);

                string OutPutFile2 = WorkingDir5;
                OutPutFile2.append(CounterChar);
                OutPutFile2.append(".ivf");

                char SpeedTestRealTime[255];
                snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                opt.CpuUsed = counter;
                CompressInt = opt.CpuUsed;
                opt.Mode = MODE_REALTIME;
                RealTotalmsPos[counter] = TimeCompressIVFtoIVF(input, SpeedTestRealTime, speed, BitRate, opt, CompressString, CompressInt);

                if (RealTotalmsPos[counter] == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                if (TestType != 2 && TestType != 3)
                {
                    RealPSNRArrPos[counter] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, ssim);
                }

                counter++;
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

    cout << "\n";
    cerr << "\n";

    int counter = 1;

    if (Mode == 1)
    {
        while (counter < 6)
        {
            if (GoodTotalms[counter] < GoodTotalms[counter-1])
            {
                printf("      CpuUsed %*i Encode Time: %i < CpuUsed %*i Encode Time: %i\n", 2, counter, GoodTotalms[counter], 2, counter - 1, GoodTotalms[counter-1]);
                fprintf(stderr, "      CpuUsed %*i Encode Time: %i < CpuUsed %*i Encode Time: %i\n", 2, counter, GoodTotalms[counter], 2, counter - 1, GoodTotalms[counter-1]);
            }
            else
            {
                printf("Fail: CpuUsed %*i Encode Time: %i > CpuUsed %*i Encode Time: %i\n", 2, counter, GoodTotalms[counter], 2, counter - 1, GoodTotalms[counter-1]);
                fprintf(stderr, "Fail: CpuUsed %*i Encode Time: %i > CpuUsed %*i Encode Time: %i\n", 2, counter, GoodTotalms[counter], 2, counter - 1, GoodTotalms[counter-1]);
                Fail = 1;
            }

            float PSNRPercent;
            PSNRPercent = absFloat(GoodPSNRArr[counter] - GoodPSNRArr[counter-1]) / GoodPSNRArr[counter-1];

            if (PSNRPercent < 0.1)
            {
                printf("      CpuUsed %*i PSNR: %4.2f within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, GoodPSNRArr[counter], 2, counter - 1, GoodPSNRArr[counter-1]);
                fprintf(stderr, "      CpuUsed %*i PSNR: %4.2f within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, GoodPSNRArr[counter], 2, counter - 1, GoodPSNRArr[counter-1]);
            }
            else
            {
                printf("Fail: CpuUsed %*i PSNR: %4.2f not within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, GoodPSNRArr[counter], 2, counter - 1, GoodPSNRArr[counter-1]);
                fprintf(stderr, "Fail: CpuUsed %*i PSNR: %4.2f not within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, GoodPSNRArr[counter], 2, counter - 1, GoodPSNRArr[counter-1]);
                Fail2 = 1;
            }

            counter++;
        }
    }

    if (Mode == 0)
    {
        counter = 1;

        while (counter < 16)
        {
            if (RealTotalms[counter] < RealTotalms[counter-1])
            {
                printf("      CpuUsed -%*i Encode Time: %i < CpuUsed -%*i Encode Time: %i\n", 2, counter + 1, RealTotalms[counter], 2, counter, RealTotalms[counter-1]);
                fprintf(stderr, "      CpuUsed -%*i Encode Time: %i < CpuUsed -%*i Encode Time: %i\n", 2, counter + 1, RealTotalms[counter], 2, counter, RealTotalms[counter-1]);
            }
            else
            {
                printf("Fail: CpuUsed -%*i Encode Time: %i > CpuUsed -%*i Encode Time: %i\n", 2, counter + 1, RealTotalms[counter], 2, counter, RealTotalms[counter-1]);
                fprintf(stderr, "Fail: CpuUsed -%*i Encode Time: %i > CpuUsed -%*i Encode Time: %i\n", 2, counter + 1, RealTotalms[counter], 2, counter, RealTotalms[counter-1]);
                Fail++;

                float TimePercent = (RealTotalms[counter] - RealTotalms[counter-1]);

                if (TimePercent < 0)
                {
                    TimePercent = TimePercent * (-1);
                }

                if (TimePercent > (RealTotalms[counter-1] / 10))
                {
                    Failb++;
                }
                else
                {
                }
            }

            float PSNRPercent;
            PSNRPercent = absFloat(RealPSNRArr[counter] - RealPSNRArr[counter-1]) / RealPSNRArr[counter-1];

            if (PSNRPercent < 0.1)
            {
                printf("      CpuUsed -%*i PSNR: %4.2f within 10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1, RealPSNRArr[counter], 2, counter, RealPSNRArr[counter-1]);
                fprintf(stderr, "      CpuUsed -%*i PSNR: %4.2f within 10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1, RealPSNRArr[counter], 2, counter, RealPSNRArr[counter-1]);
            }
            else
            {
                printf("Fail: CpuUsed -%*i PSNR: %4.2f not within 10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1, RealPSNRArr[counter], 2, counter, RealPSNRArr[counter-1]);
                fprintf(stderr, "Fail: CpuUsed -%*i PSNR: %4.2f not within 10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1, RealPSNRArr[counter], 2, counter, RealPSNRArr[counter-1]);
                Fail2++;
            }

            counter++;
        }

        cout << "\n\n";
        cerr << "\n\n";

        counter = 1;

        while (counter < 17)
        {
            if (RealTotalmsPos[counter] < RealTotalmsPos[counter-1])
            {
                printf("      CpuUsed %*i Encode Time: %i < CpuUsed %*i Encode Time: %i\n", 2, counter, RealTotalmsPos[counter], 2, counter - 1, RealTotalmsPos[counter-1]);
                fprintf(stderr, "      CpuUsed %*i Encode Time: %i < CpuUsed %*i Encode Time: %i\n", 2, counter, RealTotalmsPos[counter], 2, counter - 1, RealTotalmsPos[counter-1]);
            }
            else
            {

                printf("Fail: CpuUsed %*i Encode Time: %i > CpuUsed %*i Encode Time: %i\n", 2, counter, RealTotalmsPos[counter], 2, counter - 1, RealTotalmsPos[counter-1]);
                fprintf(stderr, "Fail: CpuUsed %*i Encode Time: %i > CpuUsed %*i Encode Time: %i\n", 2, counter, RealTotalmsPos[counter], 2, counter - 1, RealTotalmsPos[counter-1]);
                Fail++;

                float TimePercent = (RealTotalmsPos[counter] - RealTotalmsPos[counter-1]);

                if (TimePercent < 0)
                {
                    TimePercent = TimePercent * (-1);
                }

                if (TimePercent > (RealTotalmsPos[counter-1] / 10))
                {
                    Failb++;
                }
                else
                {
                }
            }

            float PSNRPercent;
            PSNRPercent = absFloat(RealPSNRArrPos[counter] - RealPSNRArrPos[counter-1]) / RealPSNRArrPos[counter-1];

            if (PSNRPercent < 0.1)
            {
                printf("      CpuUsed %*i PSNR: %4.2f within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, RealPSNRArrPos[counter], 2, counter - 1, RealPSNRArrPos[counter-1]);
                fprintf(stderr, "      CpuUsed %*i PSNR: %4.2f within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, RealPSNRArrPos[counter], 2, counter - 1, RealPSNRArrPos[counter-1]);
            }
            else
            {
                printf("Fail: CpuUsed %*i PSNR: %4.2f not within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, RealPSNRArrPos[counter], 2, counter - 1, RealPSNRArrPos[counter-1]);
                fprintf(stderr, "Fail: CpuUsed %*i PSNR: %4.2f not within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, RealPSNRArrPos[counter], 2, counter - 1, RealPSNRArrPos[counter-1]);
                Fail2++;
            }

            counter++;
        }
    }

    if (Fail == 0 && Fail2 == 0)
    {
        printf("\nPass - All Speeds increase properly and all PSNRs are within 10%%\n");
        fprintf(stderr, "\nPass - All Speeds increase properly and all PSNRs are within 10%%\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }


    if (Mode == 0)
    {

        if (Failb != 0)
        {
            printf("\nFail - Not all Encode speeds are within 10%%\n");
            fprintf(stderr, "\nFail - Not all Encode speeds are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0 && Fail2 < 2)
        {
            printf("\nMin Test Passed - All Speeds increase properly and enough PSNRs are within 10%%\n");
            fprintf(stderr, "\nMin Test Passed - All Speeds increase properly and enough PSNRs are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }

        if (Fail < 4 && Fail2 == 0)
        {
            printf("\nMin Test Passed - Enough Speeds increase properly and All PSNRs are within 10%%\n");
            fprintf(stderr, "\nMin Test Passed - Enough Speeds increase properly and All PSNRs are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }

        if (Fail < 4 && Fail2 < 2)
        {
            printf("\nMin Test Passed - Enough Speeds increase properly and enough PSNRs are within 10%%\n");
            fprintf(stderr, "\nMin Test Passed - Enough Speeds increase properly and enough PSNRs are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }
        else
        {
            printf("\nFail - Not enough Speeds increase properly and not enough PSNRs are within 10%%\n");
            fprintf(stderr, "\nFail - Not enough Speeds increase properly and not enough PSNRs are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }


    if (Mode == 1)
    {
        if (Fail > 0 && Fail2 > 0)
        {
            printf("\nFail - Not all Speeds increase properly and all PSNRs are within 10%%\n");
            fprintf(stderr, "\nFail - Not all Speeds increase properly and all PSNRs are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail > 0 && Fail2 == 0)
        {
            printf("\nFail - Not all Speeds increase properly but all PSNRs are within 10%%\n");
            fprintf(stderr, "\nFail - Not all Speeds increase properly but all PSNRs are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0 && Fail2 > 0)
        {
            printf("\nFail - All Speeds increase properly but not all PSNRs are within 10%%\n");
            fprintf(stderr, "\nFail - All Speeds increase properly but not all PSNRs are within 10%%\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int Version(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Version";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  Version \n\n"
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

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "Version";
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

        FileStream.close();
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;

    string WorkingDir8 = WorkingDirString;
    string WorkingDir9 = WorkingDirString;
    string WorkingDir10 = WorkingDirString;
    string WorkingDir11 = WorkingDirString;

    string WorkingDir12 = WorkingDirString;
    string WorkingDir13 = WorkingDirString;
    string WorkingDir14 = WorkingDirString;
    string WorkingDir15 = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("Version0.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("Version1.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("Version2.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("Version3.ivf");

    WorkingDir8.append(slashCharStr);
    WorkingDir8.append("Version0_Dec.ivf");
    WorkingDir9.append(slashCharStr);
    WorkingDir9.append("Version1_Dec.ivf");
    WorkingDir10.append(slashCharStr);
    WorkingDir10.append("Version2_Dec.ivf");
    WorkingDir11.append(slashCharStr);
    WorkingDir11.append("Version3_Dec.ivf");

    WorkingDir12.append(slashCharStr);
    WorkingDir12.append("Version0_Dec_CompressionTime.txt");
    WorkingDir13.append(slashCharStr);
    WorkingDir13.append("Version1_Dec_CompressionTime.txt");
    WorkingDir14.append(slashCharStr);
    WorkingDir14.append("Version2_Dec_CompressionTime.txt");
    WorkingDir15.append(slashCharStr);
    WorkingDir15.append("Version3_Dec_CompressionTime.txt");

    char Version0[255];
    char Version1[255];
    char Version2[255];
    char Version3[255];

    char Version0_Dec[255];
    char Version1_Dec[255];
    char Version2_Dec[255];
    char Version3_Dec[255];

    char Version0_Dectime[255];
    char Version1_Dectime[255];
    char Version2_Dectime[255];
    char Version3_Dectime[255];

    snprintf(Version0, 255, "%s", WorkingDir4.c_str());
    snprintf(Version1, 255, "%s", WorkingDir5.c_str());
    snprintf(Version2, 255, "%s", WorkingDir6.c_str());
    snprintf(Version3, 255, "%s", WorkingDir7.c_str());

    snprintf(Version0_Dec, 255, "%s", WorkingDir8.c_str());
    snprintf(Version1_Dec, 255, "%s", WorkingDir9.c_str());
    snprintf(Version2_Dec, 255, "%s", WorkingDir10.c_str());
    snprintf(Version3_Dec, 255, "%s", WorkingDir11.c_str());

    snprintf(Version0_Dectime, 255, "%s", WorkingDir12.c_str());
    snprintf(Version1_Dectime, 255, "%s", WorkingDir13.c_str());
    snprintf(Version2_Dectime, 255, "%s", WorkingDir14.c_str());
    snprintf(Version3_Dectime, 255, "%s", WorkingDir15.c_str());

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

    char *input2 = argv[5];

    printf("Version test\n");
    fprintf(stderr, "Version test\n");

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

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 =Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        unsigned int DecTime[4];

        DecTime[0] = MultiPlatTimeRet(Version0_Dec);
        DecTime[1] = MultiPlatTimeRet(Version1_Dec);
        DecTime[2] = MultiPlatTimeRet(Version2_Dec);
        DecTime[3] = MultiPlatTimeRet(Version3_Dec);

        int PSNRToggle = PSNRSelect(input, Version0);

        float PSNRArr[4];

        double ssim = 0;
        PSNRArr[0] = IVFPSNR(input, Version0, PSNRToggle, 0, 1, ssim);
        PSNRArr[1] = IVFPSNR(input, Version1, PSNRToggle, 0, 1, ssim);
        PSNRArr[2] = IVFPSNR(input, Version2, PSNRToggle, 0, 1, ssim);
        PSNRArr[3] = IVFPSNR(input, Version3, PSNRToggle, 0, 1, ssim);

        int Fail = 0;

        int i = 0;

        while (i < 4)
        {

            int t = i + 1;

            while (t < 4)
            {
                //i should always have Higher PSNR than t
                //i should always have a higher DecTime as well.
                if (PSNRArr[i] < PSNRArr[t])
                {
                    if (DecTime[i] < DecTime[t])
                    {
                        printf("\nFailed Version%i Decode Time: %d >= Version%i Decode Time: %d\n"
                               "       Version%i PSNR: %f <= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                        fprintf(stderr, "\nFailed Version%i Decode Time: %d >= Version%d Decode Time: %i\n"
                                "       Version%i PSNR: %f <= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                        Fail = 1;
                    }
                    else
                    {
                        printf("\nFailed Version%i Decode Time: %d <= Version%i Decode Time: %d\n"
                               "       Version%i PSNR: %f <= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                        fprintf(stderr, "\nFailed Version%i Decode Time: %d <= Version%d Decode Time: %i\n"
                                "       Version%i PSNR: %f <= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                        Fail = 1;
                    }
                }
                else
                {
                    if (DecTime[i] < DecTime[t])
                    {
                        printf("\nFailed Version%i Decode Time: %d <= Version%i Decode Time: %d\n"
                               "       Version%i PSNR: %f >= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                        fprintf(stderr, "\nFailed Version%i Decode Time: %d <= Version%d Decode Time: %i\n"
                                "       Version%i PSNR: %f >= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                        Fail = 1;
                    }
                    else
                    {
                        printf("\nPassed Version%i Decode Time: %d >= Version%i Decode Time: %d\n"
                               "       Version%i PSNR: %f >= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                        fprintf(stderr, "\nPassed Version%i Decode Time: %d >= Version%d Decode Time: %i\n"
                                "       Version%i PSNR: %f >= Version%i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    }
                }

                t++;
            }

            i++;
        }




        if (Fail == 0)
        {
            printf("\n     Passed: All PSNRs Decrease or remain constant and Decode times increase or remain constant as version numbers increase\n");
            fprintf(stderr, "\n     Passed: All PSNRs Decrease or remain constant and Decode times increase or remain constant as version numbers increase\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }

        printf("\n     Failed: Not all PSNRs Decrease or remain constant and not all Decode times increase or remain constant as version numbers increase\n");
        fprintf(stderr, "\n     Failed: Not all PSNRs Decrease and not all Decode times increase or remain constant as version numbers increase\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.Version = 0;

            if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.Version = 1;

            if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.Version = 2;

            if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.Version = 3;

            if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3) == -1)
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
            opt.Version = 0;

            if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.Version = 1;

            if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.Version = 2;

            if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.Version = 3;

            if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3) == -1)
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
            opt.Version = 0;

            if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.Version = 1;

            if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.Version = 2;

            if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.Version = 3;

            if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3) == -1)
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
            opt.Version = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Version = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Version = 2;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Version = 3;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.Version = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Version = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Version = 2;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Version = 3;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    unsigned int DecTime[4];

    printf("\n\n");
    fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");
    DecTime[0] = TimeDecompressIVFtoIVF(Version0, Version0_Dec);
    printf("\n");
    fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
    DecTime[1] = TimeDecompressIVFtoIVF(Version1, Version1_Dec);
    printf("\n");
    fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
    DecTime[2] = TimeDecompressIVFtoIVF(Version2, Version2_Dec);
    printf("\n");
    fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
    DecTime[3] = TimeDecompressIVFtoIVF(Version3, Version3_Dec);


    //Create Compression only stop test short.
    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    int PSNRToggle = PSNRSelect(input, Version0);

    float PSNRArr[4];

    double ssim = 0;
    PSNRArr[0] = IVFPSNR(input, Version0, PSNRToggle, 0, 1, ssim);
    PSNRArr[1] = IVFPSNR(input, Version1, PSNRToggle, 0, 1, ssim);
    PSNRArr[2] = IVFPSNR(input, Version2, PSNRToggle, 0, 1, ssim);
    PSNRArr[3] = IVFPSNR(input, Version3, PSNRToggle, 0, 1, ssim);

    cout << "\n";
    cerr << "\n";

    int PSNRFail = 0;
    int TIMEFail = 0;
    int i = 0;

    while (i < 4)
    {

        int t = i + 1;

        while (t < 4)
        {
            //i should always have Higher PSNR than t
            //i should always have a higher DecTime as well.
            if (PSNRArr[i] < PSNRArr[t])
            {
                if (DecTime[i] < DecTime[t])
                {
                    printf("\nFailed Version %i Decode Time: %d >= Version %i Decode Time: %d\n"
                           "Failed Version %i PSNR: %f <= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    fprintf(stderr, "\nFailed Version %i Decode Time: %d >= Version %i Decode Time: %d\n"
                            "Failed Version %i PSNR: %f <= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    TIMEFail++;
                    PSNRFail++;
                }
                else
                {
                    printf("\n       Version %i Decode Time: %d >= Version %i Decode Time: %d\n"
                           "Failed Version %i PSNR: %f <= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    fprintf(stderr, "\n       Version %i Decode Time: %d <= Version %d Decode Time: %i\n"
                            "Failed Version %i PSNR: %f <= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    PSNRFail++;
                }
            }
            else
            {
                if (DecTime[i] < DecTime[t])
                {
                    printf("\nFailed Version %i Decode Time: %d <= Version %i Decode Time: %d\n"
                           "       Version %i PSNR: %f >= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    fprintf(stderr, "\nFailed Version%i Decode Time: %d <= Version%d Decode Time: %i\n"
                            "       Version %i PSNR: %f >= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    TIMEFail++;
                }
                else
                {
                    printf("\n       Version %i Decode Time: %d >= Version %i Decode Time: %d\n"
                           "       Version %i PSNR: %f >= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                    fprintf(stderr, "\n       Version %i Decode Time: %d >= Version %i Decode Time: %d\n"
                            "       Version %i PSNR: %f >= Version %i PSNR: %f\n", i, DecTime[i], t, DecTime[t], i, PSNRArr[i], t, PSNRArr[t]);
                }
            }

            t++;
        }

        i++;
    }

    //Fail tracks PSNR Fails
    //Fail2 tracks Time Fails
    if (PSNRFail == 0 && TIMEFail == 0)
    {
        printf("\nPassed: All PSNRs Decrease and Decode times increase as version numbers increase\n");
        fprintf(stderr, "\nPassed: All PSNRs Decrease and Decode times increase as version numbers increase\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (PSNRFail < 2 && TIMEFail == 0)
    {
        printf("\nMin Test Passed: All but one PSNR Decreases and All Decode times increase as version numbers increase\n");
        fprintf(stderr, "\nMin Test Passed: All but one PSNR Decreases and All Decode times increase as version numbers increase\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 8;
    }

    if (PSNRFail == 0 && TIMEFail < 2)
    {
        printf("\nMin Test Passed: All PSNRs Decrease and all but one Decode time increases as version numbers increase\n");
        fprintf(stderr, "\nMin Test Passed: All PSNRs Decrease and all but one Decode time increases as version numbers increase\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 8;
    }

    if (PSNRFail < 2 && TIMEFail < 2)
    {
        printf("\nMin Test Passed: All but one PSNR Decreases and all but one Decode time increases as version numbers increase\n");
        fprintf(stderr, "\nMin Test Passed: All but one PSNR Decreases and all but one Decode time increases as version numbers increase\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 8;
    }

    printf("\nFailed: Not all PSNRs Decrease and Decode times increase as version numbers increase\n");
    fprintf(stderr, "\nFailed: Not all PSNRs Decrease and Decode times increase as version numbers increase\n");
    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
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

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AltQ = AltQ2Int;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AltQ = AltQ2Int;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AltQ = AltQ2Int;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltQ = AltQ2Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, AltQ1, speed, BitRate, opt, CompressString, AltQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AltQ = AltQ2Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AltQ2, speed, BitRate, opt, CompressString, AltQ2Int) == -1)
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

    int FileSize1 = FileSize(AltQ1);
    int FileSize2 = FileSize(AltQ2);

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

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.KeyQ = KeyQ2Int;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.KeyQ = KeyQ2Int;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.KeyQ = KeyQ2Int;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.KeyQ = KeyQ2Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, KeyQ1, speed, BitRate, opt, CompressString, KeyQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.KeyQ = KeyQ2Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, KeyQ2, speed, BitRate, opt, CompressString, KeyQ2Int) == -1)
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

    int FileSize1 = FileSize(KeyQ1);
    int FileSize2 = FileSize(KeyQ2);

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

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.GoldQ = GoldQ2Int;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.GoldQ = GoldQ2Int;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.GoldQ = GoldQ2Int;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.GoldQ = GoldQ2Int;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int) == -1)
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

            if (CompressIVFtoIVF(input, GoldQ1, speed, BitRate, opt, CompressString, GoldQ1Int) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.GoldQ = GoldQ2Int;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, GoldQ2, speed, BitRate, opt, CompressString, GoldQ2Int) == -1)
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

    int FileSize1 = FileSize(GoldQ1);
    int FileSize2 = FileSize(GoldQ2);

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


int WindowsMatchesLinux(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "WindowsMatchesLinux";
    char *input = argv[2];

    if (!(argc == 9 || argc == 10))
    {
#if defined(linux)
        printf(
            "  WindowsMatchesLinux \n\n"
            "    <inputfile>\n"
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
            "    <Win Input Folder - enter N/A if none>\n"
            "    <Mac Input Folder - enter N/A if none>\n"
            "    <PPC Input Folder - enter N/A if none>\n"
            "    <Optional Settings File>\n"
        );
        return 0;
#elif defined(_WIN32)
        printf(
            "  WindowsMatchesLinux \n\n"
            "    <inputfile>\n"
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
            "    <Lin Input Folder - enter N/A if none>\n"
            "    <Mac Input Folder - enter N/A if none>\n"
            "    <PPC Input Folder - enter N/A if none>\n"
            "    <Optional Settings File>\n"
        );
        return 0;
#elif defined(__POWERPC__)
        printf(
            "  WindowsMatchesLinux \n\n"
            "    <inputfile>\n"
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
            "    <Lin Input Folder - enter N/A if none>\n"
            "    <Win Input Folder - enter N/A if none>\n"
            "    <Mac Input Folder - enter N/A if none>\n"
            "    <Optional Settings File>\n"
        );
        return 0;
#elif defined(__APPLE__)
        printf(
            "  WindowsMatchesLinux \n\n"
            "    <inputfile>\n"
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
            "    <Lin Input Folder - enter N/A if none>\n"
            "    <Win Input Folder - enter N/A if none>\n"
            "    <PPC Input Folder - enter N/A if none>\n"
            "    <Optional Settings File>\n"
        );
        return 0;
#endif
    }


    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "WindowsMatchesLinux";
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

    string WinBase;
    string LinBase;
    string MacBase;
    string PPCBase;

#if defined(linux)
    LinBase = "N/A";
    WinBase = argv[6];
    MacBase = argv[7];
    PPCBase = argv[8];
#elif defined(_WIN32)
    LinBase = argv[6];
    WinBase = "N/A";
    MacBase = argv[7];
    PPCBase = argv[8];
#elif defined(__APPLE__)
    LinBase = argv[6];
    WinBase = argv[7];
    MacBase = "N/A";
    PPCBase = argv[8];
#elif defined(__POWERPC__)
    LinBase = argv[6];
    WinBase = argv[7];
    MacBase = argv[8];
    PPCBase = "N/A";
#endif

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir4b = WorkingDirString;
    string WorkingDir5b = WorkingDirString;
    string WorkingDir6b = WorkingDirString;
    string WorkingDir7b = WorkingDirString;

    string WorkingDir8 = WinBase;
    string WorkingDir9 = LinBase;
    string WorkingDir10 = WinBase;
    string WorkingDir11 = WinBase;
    string WorkingDir12 = LinBase;
    string WorkingDir13 = LinBase;

    string WorkingDir8b = MacBase;
    string WorkingDir9b = PPCBase;
    string WorkingDir10b = MacBase;
    string WorkingDir11b = MacBase;
    string WorkingDir12b = PPCBase;
    string WorkingDir13b = PPCBase;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("Lin_Enc.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("Lin_Dec.ivf");

    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("Win_Enc.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("Win_Dec.ivf");

    WorkingDir4b.append(slashCharStr);
    WorkingDir4b.append("Mac_Enc.ivf");
    WorkingDir5b.append(slashCharStr);
    WorkingDir5b.append("Mac_Dec.ivf");

    WorkingDir6b.append(slashCharStr);
    WorkingDir6b.append("PPC_Enc.ivf");
    WorkingDir7b.append(slashCharStr);
    WorkingDir7b.append("PPC_Dec.ivf");


    WorkingDir8.append(slashCharStr);
    WorkingDir8.append("Win_Enc_Paramaters.txt");
    WorkingDir9.append(slashCharStr);
    WorkingDir9.append("Lin_Enc_Paramaters.txt");

    WorkingDir8b.append(slashCharStr);
    WorkingDir8b.append("Mac_Enc_Paramaters.txt");
    WorkingDir9b.append(slashCharStr);
    WorkingDir9b.append("PPC_Enc_Paramaters.txt");

    WorkingDir10.append(slashCharStr);
    WorkingDir10.append("Win_Enc.ivf");
    WorkingDir11.append(slashCharStr);
    WorkingDir11.append("Win_Dec.ivf");

    WorkingDir12.append(slashCharStr);
    WorkingDir12.append("Lin_Enc.ivf");
    WorkingDir13.append(slashCharStr);
    WorkingDir13.append("Lin_Dec.ivf");

    WorkingDir10b.append(slashCharStr);
    WorkingDir10b.append("Mac_Enc.ivf");
    WorkingDir11b.append(slashCharStr);
    WorkingDir11b.append("Mac_Dec.ivf");

    WorkingDir12b.append(slashCharStr);
    WorkingDir12b.append("PPC_Enc.ivf");
    WorkingDir13b.append(slashCharStr);
    WorkingDir13b.append("PPC_Dec.ivf");


    char LinEnc[255];
    char LinDec[255];
    char WinEnc[255];
    char WinDec[255];
    char MacEnc[255];
    char MacDec[255];
    char PPCEnc[255];
    char PPCDec[255];

    char LinEncParm[255];
    char WinEncParm[255];
    char MacEncParm[255];
    char PPCEncParm[255];

    char WinPrevEnc[255];
    char WinPrevDec[255];
    char LinPrevEnc[255];
    char LinPrevDec[255];
    char MacPrevEnc[255];
    char MacPrevDec[255];
    char PPCPrevEnc[255];
    char PPCPrevDec[255];

    snprintf(LinEnc, 255, "%s", WorkingDir4.c_str());
    snprintf(LinDec, 255, "%s", WorkingDir5.c_str());
    snprintf(WinEnc, 255, "%s", WorkingDir6.c_str());
    snprintf(WinDec, 255, "%s", WorkingDir7.c_str());

    snprintf(WinEncParm, 255, "%s", WorkingDir8.c_str());
    snprintf(LinEncParm, 255, "%s", WorkingDir9.c_str());

    snprintf(WinPrevEnc, 255, "%s", WorkingDir10.c_str());
    snprintf(WinPrevDec, 255, "%s", WorkingDir11.c_str());
    snprintf(LinPrevEnc, 255, "%s", WorkingDir12.c_str());
    snprintf(LinPrevDec, 255, "%s", WorkingDir13.c_str());

    snprintf(MacEnc, 255, "%s", WorkingDir4b.c_str());
    snprintf(MacDec, 255, "%s", WorkingDir5b.c_str());
    snprintf(PPCEnc, 255, "%s", WorkingDir6b.c_str());
    snprintf(PPCDec, 255, "%s", WorkingDir7b.c_str());

    snprintf(MacEncParm, 255, "%s", WorkingDir8b.c_str());
    snprintf(PPCEncParm, 255, "%s", WorkingDir9b.c_str());

    snprintf(MacPrevEnc, 255, "%s", WorkingDir10b.c_str());
    snprintf(MacPrevDec, 255, "%s", WorkingDir11b.c_str());
    snprintf(PPCPrevEnc, 255, "%s", WorkingDir12b.c_str());
    snprintf(PPCPrevDec, 255, "%s", WorkingDir13b.c_str());

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
    int TestMode = atoi(argv[5]);
    char *InputFolder = argv[6];

    printf("Linux Matches Windows");
    fprintf(stderr, "Linux Matches Windows");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);
    ///////////////////Use Custom Settings///////////////////
    //If Mode == 0 (Create Files) Then if input settings is listed use it
    //If Mode == 1 (run Test) Then input settings from test file

    char ParmFile[255];
    char EncOutput[255];
    char DecOutput[255];
    char PrevEncOutput[255];
    char PrevDecOutput[255];

    char PrevEncOutput2[255];
    char PrevDecOutput2[255];
    char PrevEncOutput3[255];
    char PrevDecOutput3[255];

    string CopyEncBaseStr;
    string CopyDecBaseStr;
    string CopyCMD;

#if defined(linux)
    snprintf(ParmFile, 255, "%s", WorkingDir8.c_str());
    snprintf(DecOutput, 255, "%s", LinDec);

    snprintf(PrevEncOutput2, 255, "%s", WorkingDir10b.c_str());
    snprintf(PrevDecOutput2, 255, "%s", WorkingDir11b.c_str());

    snprintf(PrevEncOutput3, 255, "%s", WorkingDir12b.c_str());
    snprintf(PrevDecOutput3, 255, "%s", WorkingDir13b.c_str());

    CopyEncBaseStr = "Lin_Enc.ivf";
    CopyDecBaseStr = "Lin_Dec.ivf";
    CopyCMD = "cp ";
#elif defined(__POWERPC__)
    cout << "\n\nPPC DEFINED\n\n";
    snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

    snprintf(EncOutput, 255, "%s", PPCEnc);
    snprintf(DecOutput, 255, "%s", PPCDec);

    snprintf(PrevEncOutput, 255, "%s", WorkingDir12.c_str());
    snprintf(PrevDecOutput, 255, "%s", WorkingDir13.c_str());

    snprintf(PrevEncOutput2, 255, "%s", WorkingDir10.c_str());
    snprintf(PrevDecOutput2, 255, "%s", WorkingDir11.c_str());

    snprintf(PrevEncOutput3, 255, "%s", WorkingDir10b.c_str());
    snprintf(PrevDecOutput3, 255, "%s", WorkingDir11b.c_str());

    CopyEncBaseStr = "PPC_Enc.ivf";
    CopyDecBaseStr = "PPC_Dec.ivf";
    CopyCMD = "cp ";
#elif defined(__APPLE__)

    cout << "\n\nAPPLE DEFINED\n\n";
    snprintf(ParmFile, 255, "%s", WorkingDir8b.c_str());

    snprintf(EncOutput, 255, "%s", MacEnc);
    snprintf(DecOutput, 255, "%s", MacDec);

    snprintf(PrevEncOutput, 255, "%s", WorkingDir12.c_str());
    snprintf(PrevDecOutput, 255, "%s", WorkingDir13.c_str());

    snprintf(PrevEncOutput2, 255, "%s", WorkingDir10.c_str());
    snprintf(PrevDecOutput2, 255, "%s", WorkingDir11.c_str());

    snprintf(PrevEncOutput3, 255, "%s", WorkingDir12b.c_str());
    snprintf(PrevDecOutput3, 255, "%s", WorkingDir13b.c_str());

    CopyEncBaseStr = "Mac_Enc.ivf";
    CopyDecBaseStr = "Mac_Dec.ivf";
    CopyCMD = "cp ";
#elif defined(_WIN32)
    snprintf(ParmFile, 255, "%s", WorkingDir9.c_str());

    snprintf(EncOutput, 255, "%s", WinEnc);
    snprintf(DecOutput, 255, "%s", WinDec);

    snprintf(PrevEncOutput, 255, "%s", WorkingDir12.c_str());
    snprintf(PrevDecOutput, 255, "%s", WorkingDir13.c_str());

    snprintf(PrevEncOutput2, 255, "%s", WorkingDir10b.c_str());
    snprintf(PrevDecOutput2, 255, "%s", WorkingDir11b.c_str());

    snprintf(PrevEncOutput3, 255, "%s", WorkingDir12b.c_str());
    snprintf(PrevDecOutput3, 255, "%s", WorkingDir13b.c_str());

    CopyEncBaseStr = "Win_Enc.ivf";
    CopyDecBaseStr = "Win_Dec.ivf";
    CopyCMD = "copy ";
#endif

    if (TestMode == 0)
    {
        if (argc == 10)
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
    }

    if (TestMode == 1)
    {
        FILE *InputCheckb = fopen(PrevEncOutput, "rb");

        if (InputCheckb == NULL)
        {
            printf("\nEncoded file %s does not exist\n", PrevEncOutput);
            fprintf(stderr, "\nEncoded file %s does not exist\n", PrevEncOutput);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheckb);
        FILE *InputCheckc = fopen(PrevDecOutput, "rb");

        if (InputCheckc == NULL)
        {
            printf("\nDecoded file %s does not exist\n", PrevDecOutput);
            fprintf(stderr, "\nDecoded file %s does not exist\n", PrevDecOutput);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheckc);

        FILE *InputCheckd = fopen(PrevEncOutput2, "rb");

        if (InputCheckd == NULL)
        {
            printf("\nEncoded file %s does not exist\n", PrevEncOutput2);
            fprintf(stderr, "\nEncoded file %s does not exist\n", PrevEncOutput2);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheckd);
        FILE *InputChecke = fopen(PrevDecOutput2, "rb");

        if (InputChecke == NULL)
        {
            printf("\nDecoded file %s does not exist\n", PrevDecOutput2);
            fprintf(stderr, "\nDecoded file %s does not exist\n", PrevDecOutput2);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputChecke);

        FILE *InputCheckf = fopen(PrevEncOutput3, "rb");

        if (InputCheckf == NULL)
        {
            printf("\nEncoded file %s does not exist\n", PrevEncOutput3);
            fprintf(stderr, "\nEncoded file %s does not exist\n", PrevEncOutput3);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheckf);
        FILE *InputCheckg = fopen(PrevDecOutput3, "rb");

        if (InputCheckg == NULL)
        {
            printf("\nDecoded file %s does not exist\n", PrevDecOutput3);
            fprintf(stderr, "\nDecoded file %s does not exist\n", PrevDecOutput3);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheckg);


        FILE *InputCheck = fopen(ParmFile, "rb");

        if (InputCheck == NULL)
        {
            printf("\nInput Settings file %s does not exist\n", ParmFile);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", ParmFile);
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(InputCheck);
        opt = InPutSettings(ParmFile);
        BitRate = opt.TargetBandwidth;
    }

    /////////////////////////////////////////////////////////
    BitRate = opt.TargetBandwidth;


    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode) == -1)
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

            if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode) == -1)
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

            if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode) == -1)
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

            if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode) == -1)
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

            if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

        }

        printf("\n");
        fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");

        DecompressIVFtoIVF(EncOutput, DecOutput);

        ////////////////////////Copy Resulting files over to same directory exe is located in////////////////////////
        printf("\n\n");
        fprintf(stderr, "\n\n");



        char CopyEnc[255];
        char CopyEnc2[255];

        FolderName(argv[0], CopyEnc);
        snprintf(CopyEnc2, 255, "%s", CopyEnc);
        string CopyEncStr = CopyEnc2;
        string CopyDecStr = CopyEnc2;
        string CopyParmStr = CopyEnc2;

        CopyEncStr.append(CopyEncBaseStr);
        CopyDecStr.append(CopyDecBaseStr);

        string FullCopyString1 = CopyCMD;
        FullCopyString1.append(EncOutput);
        FullCopyString1.append(" ");
        FullCopyString1.append(CopyEncStr);

        string FullCopyString2 = CopyCMD;
        FullCopyString2.append(DecOutput);
        FullCopyString2.append(" ");
        FullCopyString2.append(CopyDecStr);

        string OutputsettingsFile = EncOutput;
        OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
        OutputsettingsFile.append("_Paramaters.txt");

        string OutputsettingsFile2 = CopyEncStr;
        OutputsettingsFile2.erase(OutputsettingsFile2.length() - 4, 4);
        OutputsettingsFile2.append("_Paramaters.txt");

        string FullCopyString3 = CopyCMD;
        FullCopyString3.append(OutputsettingsFile);
        FullCopyString3.append(" ");
        FullCopyString3.append(OutputsettingsFile2);

        system(FullCopyString1.c_str());
        system(FullCopyString2.c_str());
        system(FullCopyString3.c_str());
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    if (TestMode == 0)
    {
        printf("\n\nTest files created.\n");
        fprintf(stderr, "\n\nTest files created.\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

    int ENCFAIL = 0;
    int DECFAIL = 0;
    printf("\n\nComparing %s and %s", EncOutput, PrevEncOutput);
    fprintf(stderr, "\n\nComparing %s and %s", EncOutput, PrevEncOutput);
    int EncResult = CompIVF(EncOutput, PrevEncOutput);

    if (EncResult == -1)
    {
        printf("\n\nFiles are Identical\n");
        fprintf(stderr, "\n\nFiles are Identical\n");
    }

    if (EncResult >= 0)
    {
        printf("\n\nFail: Encoded files differ at frame: %i", EncResult);
        fprintf(stderr, "\n\nFail: Encoded files differ at frame: %i", EncResult);
        ENCFAIL++;
    }

    if (EncResult == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (EncResult == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    printf("\n\nComparing %s and %s", EncOutput, PrevEncOutput2);
    fprintf(stderr, "\n\nComparing %s and %s", EncOutput, PrevEncOutput2);
    int EncResult2 = CompIVF(EncOutput, PrevEncOutput2);

    if (EncResult2 == -1)
    {
        printf("\n\nFiles are Identical\n");
        fprintf(stderr, "\n\nFiles are Identical\n");
    }

    if (EncResult2 >= 0)
    {
        printf("\n\nFail: Encoded files differ at frame: %i", EncResult);
        fprintf(stderr, "\n\nFail: Encoded files differ at frame: %i", EncResult);
        ENCFAIL++;
    }

    if (EncResult2 == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (EncResult2 == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    printf("\n\nComparing %s and %s", EncOutput, PrevEncOutput3);
    fprintf(stderr, "\n\nComparing %s and %s", EncOutput, PrevEncOutput3);
    int EncResult3 = CompIVF(EncOutput, PrevEncOutput3);

    if (EncResult3 == -1)
    {
        printf("\n\nFiles are Identical\n");
        fprintf(stderr, "\n\nFiles are Identical\n");
    }

    if (EncResult3 >= 0)
    {
        printf("\n\nFail: Encoded files differ at frame: %i", EncResult);
        fprintf(stderr, "\n\nFail: Encoded files differ at frame: %i", EncResult);
        ENCFAIL++;
    }

    if (EncResult3 == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (EncResult3 == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    printf("\n\nComparing %s and %s", DecOutput, PrevDecOutput);
    fprintf(stderr, "\n\nComparing %s and %s", DecOutput, PrevDecOutput);
    int DecResult = CompIVF(DecOutput, PrevDecOutput);

    if (DecResult == -1)
    {
        printf("\n\nFiles are Identical\n");
        fprintf(stderr, "\n\nFiles are Identical\n");
    }

    if (DecResult >= 0)
    {
        printf("\n\nFail: Decoded files differ at frame: %i", DecResult);
        fprintf(stderr, "\n\nFail: Decoded files differ at frame: %i", DecResult);
        DECFAIL++;
    }

    if (DecResult == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (DecResult == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    printf("\n\nComparing %s and %s", DecOutput, PrevDecOutput2);
    fprintf(stderr, "\n\nComparing %s and %s", DecOutput, PrevDecOutput2);
    int DecResult2 = CompIVF(DecOutput, PrevDecOutput2);

    if (DecResult2 == -1)
    {
        printf("\n\nFiles are Identical\n");
        fprintf(stderr, "\n\nFiles are Identical\n");
    }

    if (DecResult2 >= 0)
    {
        printf("\n\nFail: Decoded files differ at frame: %i", DecResult);
        fprintf(stderr, "\n\nFail: Decoded files differ at frame: %i", DecResult);
        DECFAIL++;
    }

    if (DecResult2 == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (DecResult2 == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    printf("\n\nComparing %s and %s", DecOutput, PrevDecOutput3);
    fprintf(stderr, "\n\nComparing %s and %s", DecOutput, PrevDecOutput3);
    int DecResult3 = CompIVF(DecOutput, PrevDecOutput3);

    if (DecResult3 == -1)
    {
        printf("\n\nFiles are Identical\n");
        fprintf(stderr, "\n\nFiles are Identical\n");
    }

    if (DecResult3 >= 0)
    {
        printf("\n\nFail: Decoded files differ at frame: %i", DecResult);
        fprintf(stderr, "\n\nFail: Decoded files differ at frame: %i", DecResult);
        DECFAIL++;
    }

    if (DecResult3 == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n");
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (DecResult3 == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n");
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    if (DECFAIL == 0 && ENCFAIL == 0)
    {
        printf("\n\nAll Encoded and Decoded files are identical: Pass\n\n");
        fprintf(stderr, "\n\nAll Encoded and Decoded files are identical: Pass\n\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (DECFAIL > 0 && ENCFAIL > 0)
    {
        printf("\n\nNot All Encoded and Decoded files are identical: Fail\n\n");
        fprintf(stderr, "\n\nNot All Encoded and Decoded files are identical: Fail\n\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (DECFAIL > 0)
    {
        printf("\n\nAll Encoded Files are Identical But Not All Decoded files are identical: Fail\n\n");
        fprintf(stderr, "\n\nAll Encoded Files are Identical But Not All Decoded files are identical: Fail\n\n");
    }

    if (ENCFAIL > 0)
    {
        printf("\n\nAll Decoded Files are Identical But Not All Encoded files are identical: Fail\n\n");
        fprintf(stderr, "\n\nAll Decoded Files are Identical But Not All Encoded files are identical: Fail\n\n");
    }

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}
int TestVectorCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Test Vector Check";

    char *input = argv[2];

    if (!(argc == 3))
    {
        printf(
            "  TestVectorCheck \n\n"
            "    <Input Directory>\n"
            "\n"
        );

        return 0;
    }

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////

    string WorkingDirString = ""; // <- All Options need to set a value for this
    string Mode3TestMatch = "";
    string MainDirString = "";
    char *MyDir = "TestVectorCheck";
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

    string SourceDir4(argv[2]);
    string SourceDir5(argv[2]);
    string SourceDir6(argv[2]);
    string SourceDir7(argv[2]);
    string SourceDir8(argv[2]);
    string SourceDir9(argv[2]);
    string SourceDir10(argv[2]);
    string SourceDir11(argv[2]);
    string SourceDir12(argv[2]);
    string SourceDir13(argv[2]);
    string SourceDir14(argv[2]);
    string SourceDir15(argv[2]);
    string SourceDir16(argv[2]);
    string SourceDir17(argv[2]);
    string SourceDir18(argv[2]);
    string SourceDir19(argv[2]);
    string SourceDir20(argv[2]);
    string SourceDir21(argv[2]);
    string SourceDir22(argv[2]);
    string SourceDir23(argv[2]);
    string SourceDir24(argv[2]);
    string SourceDir25(argv[2]);
    string SourceDir26(argv[2]);
    string SourceDir27(argv[2]);

    string TestVecText4(WorkingDirString.c_str());
    string TestVecText5(WorkingDirString.c_str());
    string TestVecText6(WorkingDirString.c_str());
    string TestVecText7(WorkingDirString.c_str());
    string TestVecText8(WorkingDirString.c_str());
    string TestVecText9(WorkingDirString.c_str());
    string TestVecText10(WorkingDirString.c_str());
    string TestVecText11(WorkingDirString.c_str());
    string TestVecText12(WorkingDirString.c_str());
    string TestVecText13(WorkingDirString.c_str());
    string TestVecText14(WorkingDirString.c_str());
    string TestVecText15(WorkingDirString.c_str());
    string TestVecText16(WorkingDirString.c_str());
    string TestVecText17(WorkingDirString.c_str());
    string TestVecText18(WorkingDirString.c_str());
    string TestVecText19(WorkingDirString.c_str());
    string TestVecText20(WorkingDirString.c_str());
    string TestVecText21(WorkingDirString.c_str());
    string TestVecText22(WorkingDirString.c_str());
    string TestVecText23(WorkingDirString.c_str());
    string TestVecText24(WorkingDirString.c_str());
    string TestVecText25(WorkingDirString.c_str());
    string TestVecText26(WorkingDirString.c_str());
    string TestVecText27(WorkingDirString.c_str());

    string WorkingDir4(WorkingDirString.c_str());
    string WorkingDir5(WorkingDirString.c_str());
    string WorkingDir6(WorkingDirString.c_str());
    string WorkingDir7(WorkingDirString.c_str());
    string WorkingDir8(WorkingDirString.c_str());
    string WorkingDir9(WorkingDirString.c_str());
    string WorkingDir10(WorkingDirString.c_str());
    string WorkingDir11(WorkingDirString.c_str());
    string WorkingDir12(WorkingDirString.c_str());
    string WorkingDir13(WorkingDirString.c_str());
    string WorkingDir14(WorkingDirString.c_str());
    string WorkingDir15(WorkingDirString.c_str());
    string WorkingDir16(WorkingDirString.c_str());
    string WorkingDir17(WorkingDirString.c_str());
    string WorkingDir18(WorkingDirString.c_str());
    string WorkingDir19(WorkingDirString.c_str());
    string WorkingDir20(WorkingDirString.c_str());
    string WorkingDir21(WorkingDirString.c_str());
    string WorkingDir22(WorkingDirString.c_str());
    string WorkingDir23(WorkingDirString.c_str());
    string WorkingDir24(WorkingDirString.c_str());
    string WorkingDir25(WorkingDirString.c_str());
    string WorkingDir26(WorkingDirString.c_str());
    string WorkingDir27(WorkingDirString.c_str());

    string WorkingDir32(WorkingDirString.c_str());
    string WorkingDir33(WorkingDirString.c_str());
    string WorkingDir34(WorkingDirString.c_str());
    string WorkingDir35(WorkingDirString.c_str());
    string WorkingDir36(WorkingDirString.c_str());
    string WorkingDir37(WorkingDirString.c_str());
    string WorkingDir38(WorkingDirString.c_str());
    string WorkingDir39(WorkingDirString.c_str());
    string WorkingDir40(WorkingDirString.c_str());
    string WorkingDir41(WorkingDirString.c_str());
    string WorkingDir42(WorkingDirString.c_str());
    string WorkingDir43(WorkingDirString.c_str());
    string WorkingDir44(WorkingDirString.c_str());
    string WorkingDir45(WorkingDirString.c_str());
    string WorkingDir46(WorkingDirString.c_str());
    string WorkingDir47(WorkingDirString.c_str());
    string WorkingDir48(WorkingDirString.c_str());
    string WorkingDir49(WorkingDirString.c_str());
    string WorkingDir50(WorkingDirString.c_str());
    string WorkingDir51(WorkingDirString.c_str());
    string WorkingDir52(WorkingDirString.c_str());
    string WorkingDir53(WorkingDirString.c_str());
    string WorkingDir54(WorkingDirString.c_str());
    string WorkingDir55(WorkingDirString.c_str());

    SourceDir4.append(slashCharStr);
    SourceDir4.append("vp80-00-comprehensive-001.ivf");
    SourceDir5.append(slashCharStr);
    SourceDir5.append("vp80-00-comprehensive-002.ivf");
    SourceDir6.append(slashCharStr);
    SourceDir6.append("vp80-00-comprehensive-003.ivf");
    SourceDir7.append(slashCharStr);
    SourceDir7.append("vp80-00-comprehensive-004.ivf");
    SourceDir8.append(slashCharStr);
    SourceDir8.append("vp80-00-comprehensive-005.ivf");
    SourceDir9.append(slashCharStr);
    SourceDir9.append("vp80-00-comprehensive-006.ivf");
    SourceDir10.append(slashCharStr);
    SourceDir10.append("vp80-00-comprehensive-007.ivf");
    SourceDir11.append(slashCharStr);
    SourceDir11.append("vp80-00-comprehensive-008.ivf");
    SourceDir12.append(slashCharStr);
    SourceDir12.append("vp80-00-comprehensive-009.ivf");
    SourceDir13.append(slashCharStr);
    SourceDir13.append("vp80-00-comprehensive-010.ivf");
    SourceDir14.append(slashCharStr);
    SourceDir14.append("vp80-00-comprehensive-011.ivf");
    SourceDir15.append(slashCharStr);
    SourceDir15.append("vp80-00-comprehensive-012.ivf");
    SourceDir16.append(slashCharStr);
    SourceDir16.append("vp80-00-comprehensive-013.ivf");
    SourceDir17.append(slashCharStr);
    SourceDir17.append("vp80-00-comprehensive-014.ivf");
    SourceDir18.append(slashCharStr);
    SourceDir18.append("vp80-00-comprehensive-015.ivf");
    SourceDir19.append(slashCharStr);
    SourceDir19.append("vp80-00-comprehensive-016.ivf");
    SourceDir20.append(slashCharStr);
    SourceDir20.append("vp80-00-comprehensive-017.ivf");
    SourceDir21.append(slashCharStr);

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("vp80-00-comprehensive-001Dec.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("vp80-00-comprehensive-002Dec.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("vp80-00-comprehensive-003Dec.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("vp80-00-comprehensive-004Dec.ivf");
    WorkingDir8.append(slashCharStr);
    WorkingDir8.append("vp80-00-comprehensive-005Dec.ivf");
    WorkingDir9.append(slashCharStr);
    WorkingDir9.append("vp80-00-comprehensive-006Dec.ivf");
    WorkingDir10.append(slashCharStr);
    WorkingDir10.append("vp80-00-comprehensive-007Dec.ivf");
    WorkingDir11.append(slashCharStr);
    WorkingDir11.append("vp80-00-comprehensive-008Dec.ivf");
    WorkingDir12.append(slashCharStr);
    WorkingDir12.append("vp80-00-comprehensive-009Dec.ivf");
    WorkingDir13.append(slashCharStr);
    WorkingDir13.append("vp80-00-comprehensive-010Dec.ivf");
    WorkingDir14.append(slashCharStr);
    WorkingDir14.append("vp80-00-comprehensive-011Dec.ivf");
    WorkingDir15.append(slashCharStr);
    WorkingDir15.append("vp80-00-comprehensive-012Dec.ivf");
    WorkingDir16.append(slashCharStr);
    WorkingDir16.append("vp80-00-comprehensive-013Dec.ivf");
    WorkingDir17.append(slashCharStr);
    WorkingDir17.append("vp80-00-comprehensive-014Dec.ivf");
    WorkingDir18.append(slashCharStr);
    WorkingDir18.append("vp80-00-comprehensive-015Dec.ivf");
    WorkingDir19.append(slashCharStr);
    WorkingDir19.append("vp80-00-comprehensive-016Dec.ivf");
    WorkingDir20.append(slashCharStr);
    WorkingDir20.append("vp80-00-comprehensive-017Dec.ivf");
    WorkingDir21.append(slashCharStr);

    WorkingDir32.append(slashCharStr);
    WorkingDir32.append("vp80-00-comprehensive-001.raw");
    WorkingDir33.append(slashCharStr);
    WorkingDir33.append("vp80-00-comprehensive-002.raw");
    WorkingDir34.append(slashCharStr);
    WorkingDir34.append("vp80-00-comprehensive-003.raw");
    WorkingDir35.append(slashCharStr);
    WorkingDir35.append("vp80-00-comprehensive-004.raw");
    WorkingDir36.append(slashCharStr);
    WorkingDir36.append("vp80-00-comprehensive-005.raw");
    WorkingDir37.append(slashCharStr);
    WorkingDir37.append("vp80-00-comprehensive-006.raw");
    WorkingDir38.append(slashCharStr);
    WorkingDir38.append("vp80-00-comprehensive-007.raw");
    WorkingDir39.append(slashCharStr);
    WorkingDir39.append("vp80-00-comprehensive-008.raw");
    WorkingDir40.append(slashCharStr);
    WorkingDir40.append("vp80-00-comprehensive-009.raw");
    WorkingDir41.append(slashCharStr);
    WorkingDir41.append("vp80-00-comprehensive-010.raw");
    WorkingDir42.append(slashCharStr);
    WorkingDir42.append("vp80-00-comprehensive-011.raw");
    WorkingDir43.append(slashCharStr);
    WorkingDir43.append("vp80-00-comprehensive-012.raw");
    WorkingDir44.append(slashCharStr);
    WorkingDir44.append("vp80-00-comprehensive-013.raw");
    WorkingDir45.append(slashCharStr);
    WorkingDir45.append("vp80-00-comprehensive-014.raw");
    WorkingDir46.append(slashCharStr);
    WorkingDir46.append("vp80-00-comprehensive-015.raw");
    WorkingDir47.append(slashCharStr);
    WorkingDir47.append("vp80-00-comprehensive-016.raw");
    WorkingDir48.append(slashCharStr);
    WorkingDir48.append("vp80-00-comprehensive-017.raw");
    WorkingDir49.append(slashCharStr);

    TestVecText4.append(slashCharStr);
    TestVecText4.append("vp80-00-comprehensive-001_MD5.txt");
    TestVecText5.append(slashCharStr);
    TestVecText5.append("vp80-00-comprehensive-002_MD5.txt");
    TestVecText6.append(slashCharStr);
    TestVecText6.append("vp80-00-comprehensive-003_MD5.txt");
    TestVecText7.append(slashCharStr);
    TestVecText7.append("vp80-00-comprehensive-004_MD5.txt");
    TestVecText8.append(slashCharStr);
    TestVecText8.append("vp80-00-comprehensive-005_MD5.txt");
    TestVecText9.append(slashCharStr);
    TestVecText9.append("vp80-00-comprehensive-006_MD5.txt");
    TestVecText10.append(slashCharStr);
    TestVecText10.append("vp80-00-comprehensive-007_MD5.txt");
    TestVecText11.append(slashCharStr);
    TestVecText11.append("vp80-00-comprehensive-008_MD5.txt");
    TestVecText12.append(slashCharStr);
    TestVecText12.append("vp80-00-comprehensive-009_MD5.txt");
    TestVecText13.append(slashCharStr);
    TestVecText13.append("vp80-00-comprehensive-010_MD5.txt");
    TestVecText14.append(slashCharStr);
    TestVecText14.append("vp80-00-comprehensive-011_MD5.txt");
    TestVecText15.append(slashCharStr);
    TestVecText15.append("vp80-00-comprehensive-012_MD5.txt");
    TestVecText16.append(slashCharStr);
    TestVecText16.append("vp80-00-comprehensive-013_MD5.txt");
    TestVecText17.append(slashCharStr);
    TestVecText17.append("vp80-00-comprehensive-014_MD5.txt");
    TestVecText18.append(slashCharStr);
    TestVecText18.append("vp80-00-comprehensive-015_MD5.txt");
    TestVecText19.append(slashCharStr);
    TestVecText19.append("vp80-00-comprehensive-016_MD5.txt");
    TestVecText20.append(slashCharStr);
    TestVecText20.append("vp80-00-comprehensive-017_MD5.txt");
    TestVecText21.append(slashCharStr);

    char TestVector1[255];
    char TestVector2[255];
    char TestVector3[255];
    char TestVector4[255];
    char TestVector5[255];
    char TestVector6[255];
    char TestVector7[255];
    char TestVector8[255];
    char TestVector9[255];
    char TestVector10[255];
    char TestVector11[255];
    char TestVector12[255];
    char TestVector13[255];
    char TestVector14[255];
    char TestVector15[255];
    char TestVector16[255];
    char TestVector17[255];

    char TestVector1Dec[255];
    char TestVector2Dec[255];
    char TestVector3Dec[255];
    char TestVector4Dec[255];
    char TestVector5Dec[255];
    char TestVector6Dec[255];
    char TestVector7Dec[255];
    char TestVector8Dec[255];
    char TestVector9Dec[255];
    char TestVector10Dec[255];
    char TestVector11Dec[255];
    char TestVector12Dec[255];
    char TestVector13Dec[255];
    char TestVector14Dec[255];
    char TestVector15Dec[255];
    char TestVector16Dec[255];
    char TestVector17Dec[255];

    char TestVector1Raw[255];
    char TestVector2Raw[255];
    char TestVector3Raw[255];
    char TestVector4Raw[255];
    char TestVector5Raw[255];
    char TestVector6Raw[255];
    char TestVector7Raw[255];
    char TestVector8Raw[255];
    char TestVector9Raw[255];
    char TestVector10Raw[255];
    char TestVector11Raw[255];
    char TestVector12Raw[255];
    char TestVector13Raw[255];
    char TestVector14Raw[255];
    char TestVector15Raw[255];
    char TestVector16Raw[255];
    char TestVector17Raw[255];

    char TestVector_Text1[255];
    char TestVector_Text2[255];
    char TestVector_Text3[255];
    char TestVector_Text4[255];
    char TestVector_Text5[255];
    char TestVector_Text6[255];
    char TestVector_Text7[255];
    char TestVector_Text8[255];
    char TestVector_Text9[255];
    char TestVector_Text10[255];
    char TestVector_Text11[255];
    char TestVector_Text12[255];
    char TestVector_Text13[255];
    char TestVector_Text14[255];
    char TestVector_Text15[255];
    char TestVector_Text16[255];
    char TestVector_Text17[255];

    snprintf(TestVector1, 255, "%s", SourceDir4.c_str());
    snprintf(TestVector2, 255, "%s", SourceDir5.c_str());
    snprintf(TestVector3, 255, "%s", SourceDir6.c_str());
    snprintf(TestVector4, 255, "%s", SourceDir7.c_str());
    snprintf(TestVector5, 255, "%s", SourceDir8.c_str());
    snprintf(TestVector6, 255, "%s", SourceDir9.c_str());
    snprintf(TestVector7, 255, "%s", SourceDir10.c_str());
    snprintf(TestVector8, 255, "%s", SourceDir11.c_str());
    snprintf(TestVector9, 255, "%s", SourceDir12.c_str());
    snprintf(TestVector10, 255, "%s", SourceDir13.c_str());
    snprintf(TestVector11, 255, "%s", SourceDir14.c_str());
    snprintf(TestVector12, 255, "%s", SourceDir15.c_str());
    snprintf(TestVector13, 255, "%s", SourceDir16.c_str());
    snprintf(TestVector14, 255, "%s", SourceDir17.c_str());
    snprintf(TestVector15, 255, "%s", SourceDir18.c_str());
    snprintf(TestVector16, 255, "%s", SourceDir19.c_str());
    snprintf(TestVector17, 255, "%s", SourceDir20.c_str());

    snprintf(TestVector1Dec, 255, "%s", WorkingDir4.c_str());
    snprintf(TestVector2Dec, 255, "%s", WorkingDir5.c_str());
    snprintf(TestVector3Dec, 255, "%s", WorkingDir6.c_str());
    snprintf(TestVector4Dec, 255, "%s", WorkingDir7.c_str());
    snprintf(TestVector5Dec, 255, "%s", WorkingDir8.c_str());
    snprintf(TestVector6Dec, 255, "%s", WorkingDir9.c_str());
    snprintf(TestVector7Dec, 255, "%s", WorkingDir10.c_str());
    snprintf(TestVector8Dec, 255, "%s", WorkingDir11.c_str());
    snprintf(TestVector9Dec, 255, "%s", WorkingDir12.c_str());
    snprintf(TestVector10Dec, 255, "%s", WorkingDir13.c_str());
    snprintf(TestVector11Dec, 255, "%s", WorkingDir14.c_str());
    snprintf(TestVector12Dec, 255, "%s", WorkingDir15.c_str());
    snprintf(TestVector13Dec, 255, "%s", WorkingDir16.c_str());
    snprintf(TestVector14Dec, 255, "%s", WorkingDir17.c_str());
    snprintf(TestVector15Dec, 255, "%s", WorkingDir18.c_str());
    snprintf(TestVector16Dec, 255, "%s", WorkingDir19.c_str());
    snprintf(TestVector17Dec, 255, "%s", WorkingDir20.c_str());

    snprintf(TestVector1Raw, 255, "%s", WorkingDir32.c_str());
    snprintf(TestVector2Raw, 255, "%s", WorkingDir33.c_str());
    snprintf(TestVector3Raw, 255, "%s", WorkingDir34.c_str());
    snprintf(TestVector4Raw, 255, "%s", WorkingDir35.c_str());
    snprintf(TestVector5Raw, 255, "%s", WorkingDir36.c_str());
    snprintf(TestVector6Raw, 255, "%s", WorkingDir37.c_str());
    snprintf(TestVector7Raw, 255, "%s", WorkingDir38.c_str());
    snprintf(TestVector8Raw, 255, "%s", WorkingDir39.c_str());
    snprintf(TestVector9Raw, 255, "%s", WorkingDir40.c_str());
    snprintf(TestVector10Raw, 255, "%s", WorkingDir41.c_str());
    snprintf(TestVector11Raw, 255, "%s", WorkingDir42.c_str());
    snprintf(TestVector12Raw, 255, "%s", WorkingDir43.c_str());
    snprintf(TestVector13Raw, 255, "%s", WorkingDir44.c_str());
    snprintf(TestVector14Raw, 255, "%s", WorkingDir45.c_str());
    snprintf(TestVector15Raw, 255, "%s", WorkingDir46.c_str());
    snprintf(TestVector16Raw, 255, "%s", WorkingDir47.c_str());
    snprintf(TestVector17Raw, 255, "%s", WorkingDir48.c_str());

    snprintf(TestVector_Text1, 255, "%s", TestVecText4.c_str());
    snprintf(TestVector_Text2, 255, "%s", TestVecText5.c_str());
    snprintf(TestVector_Text3, 255, "%s", TestVecText6.c_str());
    snprintf(TestVector_Text4, 255, "%s", TestVecText7.c_str());
    snprintf(TestVector_Text5, 255, "%s", TestVecText8.c_str());
    snprintf(TestVector_Text6, 255, "%s", TestVecText9.c_str());
    snprintf(TestVector_Text7, 255, "%s", TestVecText10.c_str());
    snprintf(TestVector_Text8, 255, "%s", TestVecText11.c_str());
    snprintf(TestVector_Text9, 255, "%s", TestVecText12.c_str());
    snprintf(TestVector_Text10, 255, "%s", TestVecText13.c_str());
    snprintf(TestVector_Text11, 255, "%s", TestVecText14.c_str());
    snprintf(TestVector_Text12, 255, "%s", TestVecText15.c_str());
    snprintf(TestVector_Text13, 255, "%s", TestVecText16.c_str());
    snprintf(TestVector_Text14, 255, "%s", TestVecText17.c_str());
    snprintf(TestVector_Text15, 255, "%s", TestVecText18.c_str());
    snprintf(TestVector_Text16, 255, "%s", TestVecText19.c_str());
    snprintf(TestVector_Text17, 255, "%s", TestVecText20.c_str());

    string MD5Key[24];

    //New Test Vectors 2.0.0 MD5 Checksums
    MD5Key[0].assign("fad126074e1bd5363d43b9d1cadddb71");
    MD5Key[1].assign("182f03dd264ebac04e24c7c9499d7cdb");
    MD5Key[2].assign("e5fe668b033900022c3eb0ba76a44bd1");
    MD5Key[3].assign("95097ce9808c1d47e03f99c48ad111ec");
    MD5Key[4].assign("0f469e4fd1dea533e5580688b2d242ff");
    MD5Key[5].assign("2d5fa3ec2f88404ae7b305c1074036f4");
    MD5Key[6].assign("92526913d89b6a9b00f2d602def08bce");
    MD5Key[7].assign("bd4d46a9d14fe5a7fc9cfc8deac2d34c");
    MD5Key[8].assign("19201a2d535bd82f41c1a5658def5379");
    MD5Key[9].assign("61d05919a9883d9f215eb3f2db63eb13");
    MD5Key[10].assign("1a0afe5e70512a03323a8f1176bcf022");
    MD5Key[11].assign("4ea997c80dc2087e6deec81f1ecf6668");
    MD5Key[12].assign("93169305d3054327be3cc074f0773a75");
    MD5Key[13].assign("7280a64c51dfa557c1b9552dc1e1fbed");
    MD5Key[14].assign("23b9cc582e344726e76cda092b416bcf");
    MD5Key[15].assign("55e889d22f99718cf6936d55f8ade12b");
    MD5Key[16].assign("95a68ffb228d1d8c6ee54f16a10fb9eb");

    int printKeyTracker = 0;
    int DeleteTV = 0; // if = 1 will delete decompressed tesetvectors if 0 will not

    /////////////OutPutfile////////////
    string TextfileString(WorkingDirString.c_str());
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

    printf("Test Vector Check\n\n");
    fprintf(stderr, "Test Vector Check\n");

    if (TestType == 3)
    {
        //here to be added to if needed later.
    }
    else
    {
        printf("\n\nTestVector 1\n");
        fprintf(stderr, "\n\nTestVector 1\n");

        DecompressIVFtoRaw(TestVector1, TestVector1Raw);
        ////////////////////////////////////////////////////
        printf("\n\nTestVector 2\n");
        fprintf(stderr, "\n\nTestVector 2\n");

        DecompressIVFtoRaw(TestVector2, TestVector2Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 3\n");
        fprintf(stderr, "\n\nTestVector 3\n");

        DecompressIVFtoRaw(TestVector3, TestVector3Raw);
        ////////////////////////////////////////////////////
        printf("\n\nTestVector 4\n");
        fprintf(stderr, "\n\nTestVector 4\n");

        DecompressIVFtoRaw(TestVector4, TestVector4Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 5\n");
        fprintf(stderr, "\n\nTestVector 5\n");

        DecompressIVFtoRaw(TestVector5, TestVector5Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 6\n");
        fprintf(stderr, "\n\nTestVector 6\n");

        DecompressIVFtoRaw(TestVector6, TestVector6Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 7\n");
        fprintf(stderr, "\n\nTestVector 7\n");

        DecompressIVFtoRaw(TestVector7, TestVector7Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 8\n");
        fprintf(stderr, "\n\nTestVector 8\n");

        DecompressIVFtoRaw(TestVector8, TestVector8Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 9\n");
        fprintf(stderr, "\n\nTestVector 9\n");

        DecompressIVFtoRaw(TestVector9, TestVector9Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 10\n");
        fprintf(stderr, "\n\nTestVector 10\n");

        DecompressIVFtoRaw(TestVector10, TestVector10Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 11\n");
        fprintf(stderr, "\n\nTestVector 11\n");

        DecompressIVFtoRaw(TestVector11, TestVector11Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 12\n");
        fprintf(stderr, "\n\nTestVector 12\n");

        DecompressIVFtoRaw(TestVector12, TestVector12Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 13\n");
        fprintf(stderr, "\n\nTestVector 13\n");

        DecompressIVFtoRaw(TestVector13, TestVector13Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 14\n");
        fprintf(stderr, "\n\nTestVector 14\n");

        DecompressIVFtoRaw(TestVector14, TestVector14Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 15\n");
        fprintf(stderr, "\n\nTestVector 15\n");

        DecompressIVFtoRaw(TestVector15, TestVector15Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 16\n");
        fprintf(stderr, "\n\nTestVector 16\n");

        DecompressIVFtoRaw(TestVector16, TestVector16Raw);
        ///////////////////////////////////////////////////////
        printf("\n\nTestVector 17\n");
        fprintf(stderr, "\n\nTestVector 17\n");

        DecompressIVFtoRaw(TestVector17, TestVector17Raw);
        /////////////////////////////////////////////////////////
    }

    vector<int> FailVector;

    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    int u = 1;

    printf("\n\nComputing MD5 for TestVector %i", u);
    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    u++;
    DecComputeMD5(TestVector1, TestVector_Text1);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector2, TestVector_Text2);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector3, TestVector_Text3);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector4, TestVector_Text4);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector5, TestVector_Text5);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector6, TestVector_Text6);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector7, TestVector_Text7);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector8, TestVector_Text8);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector9, TestVector_Text9);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector10, TestVector_Text10);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector11, TestVector_Text11);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector12, TestVector_Text12);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector13, TestVector_Text13);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector14, TestVector_Text14);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector15, TestVector_Text15);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector16, TestVector_Text16);

    fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", u);
    printf("\n\nComputing MD5 for TestVector %i", u);
    u++;
    DecComputeMD5(TestVector17, TestVector_Text17);

    if (DeleteTV == 1)
    {
        if (remove(TestVector1Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector1Raw);
        }

        if (remove(TestVector2Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector2Raw);
        }

        if (remove(TestVector3Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector3Raw);
        }

        if (remove(TestVector4Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector4Raw);
        }

        if (remove(TestVector5Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector5Raw);
        }

        if (remove(TestVector6Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector6Raw);
        }

        if (remove(TestVector7Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector7Raw);
        }

        if (remove(TestVector8Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector8Raw);
        }

        if (remove(TestVector9Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector9Raw);
        }

        if (remove(TestVector10Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector10Raw);
        }

        if (remove(TestVector11Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector11Raw);
        }

        if (remove(TestVector12Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector12Raw);
        }

        if (remove(TestVector13Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector13Raw);
        }

        if (remove(TestVector14Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector14Raw);
        }

        if (remove(TestVector15Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector15Raw);
        }

        if (remove(TestVector16Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector16Raw);
        }

        if (remove(TestVector17Raw) == 0)
        {
            /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
        }
        else
        {
            printf("Error: %s Not Deleted\n\n", TestVector17Raw);
        }
    }


    int x = 0;
    int Fail = 0;

    while (x < 17)
    {
        char TextInput[255];

        if (x == 0)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text1);
        }

        if (x == 1)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text2);
        }

        if (x == 2)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text3);
        }

        if (x == 3)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text4);
        }

        if (x == 4)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text5);
        }

        if (x == 5)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text6);
        }

        if (x == 6)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text7);
        }

        if (x == 7)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text8);
        }

        if (x == 8)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text9);
        }

        if (x == 9)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text10);
        }

        if (x == 10)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text11);
        }

        if (x == 11)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text12);
        }

        if (x == 12)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text13);
        }

        if (x == 13)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text14);
        }

        if (x == 14)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text15);
        }

        if (x == 15)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text16);
        }

        if (x == 16)
        {
            snprintf(TextInput, 255, "%s", TestVector_Text17);
        }

        char buffer[1024];
        fstream infile;
        infile.open(TextInput);
        infile.getline(buffer, 1024);
        infile.close();

        printf("\n\n");
        fprintf(stderr, "\n\n");

        if (MD5Key[x].compare(buffer) == 0)
        {
            printf("     VP8 Test Vector %i \n %s \n %s \n - Pass \n\n", x + 1, buffer, MD5Key[x].c_str());
            fprintf(stderr, "     VP8 Test Vector %i \n %s \n %s \n - Pass \n\n", x + 1, buffer, MD5Key[x].c_str());
        }
        else
        {
            printf("     VP8 Test Vector %i \n %s \n %s \n - Fail \n\n", x + 1, buffer, MD5Key[x].c_str());
            fprintf(stderr, "     VP8 Test Vector %i \n %s \n %s \n - Fail \n\n", x + 1, buffer, MD5Key[x].c_str());
            FailVector.push_back(x + 1);
            Fail = 1;
        }

        x++;
    }

    if (Fail == 0)
    {
        printf("\n\n All Decoded Test Vector Check Sums Match Expected Check Sums: Passed \n\n");
        fprintf(stderr, "\n\n  All Decoded Test Vector Check Sums Match Expected Check Sums: Passed \n\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (Fail == 1)
    {
        printf("\n   Test Vectors that FAIL: ");
        fprintf(stderr, "\n   Test Vectors that FAIL: ");

        int q = 0;

        while (q < FailVector.size())
        {
            if (q != 0)
            {
                printf(", ");
                fprintf(stderr, ", ");
            }

            printf("%i", FailVector[q]);
            fprintf(stderr, "%i", FailVector[q]);
            q++;
        }

        printf("\n");
        fprintf(stderr, "\n");

        printf("\n\n   Not All Decoded Test Vector Check Sums Match Expected Check Sums: Failed \n\n");
        fprintf(stderr, "\n\n Not All Decoded Test Vector Check Sums Match Expected Check Sums: Failed  \n\n");
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
//----------------------------------------Tests No Longer Used----------------------------------------------------------------
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
        Time1 = MultiPlatTimeRet(outputVP7New);
        Time2 = MultiPlatTimeRet(outputVP7Old2);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0);

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
            Time2 = Time2 + MultiPlatTimeRet(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0);

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
            Time2 = Time2 + MultiPlatTimeRet(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

            fclose(fp);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0);

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
            Time2 = Time2 + MultiPlatTimeRet(outputVP7Old);
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
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0);

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
            Time2 = MultiPlatTimeRet(outputVP7Old);
            Time2 = Time2 * 1000;

            printf("\n\n File completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\n File completed: Time in Microseconds: %i", Time2);

            fclose(fp);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            Time1 = TimeCompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0);

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
            Time2 = Time2 + MultiPlatTimeRet(outputVP7Old);
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
        Time1 = MultiPlatTimeRet(Speed14OutFile);
        Time2 = MultiPlatTimeRet(Speed00OutFile);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.MultiThreaded = 14;
            Time1 = TimeCompressIVFtoIVF(input, Speed14OutFile, speed, BitRate, opt, CompressString, 14);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.MultiThreaded = 0;
            Time2 = TimeCompressIVFtoIVF(input, Speed00OutFile, speed, BitRate, opt, CompressString, 0);

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
            Time1 = TimeCompressIVFtoIVF(input, Speed14OutFile, speed, BitRate, opt, CompressString, 14);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.MultiThreaded = 0;
            Time2 = TimeCompressIVFtoIVF(input, Speed00OutFile, speed, BitRate, opt, CompressString, 0);

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

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, DataRateMaxWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

                if (CompressIVFtoIVF(input, ComboRunAVI, opt.MultiThreaded, BitRate, opt, CompressString, TestCounter) == -1)
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
                BitRateAr[TestCounter-1] = FileSize(ComboRunAVI);
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

                if (CompressIVFtoIVF(input2, ComboRunAVI, opt.MultiThreaded, BitRate, opt, CompressString, TestCounter) == -1)
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
                BitRateAr[TestCounter-1] = FileSize(ComboRunAVI);
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

        if (CompressIVFtoIVF(input, ComboRunAVI2, opt.MultiThreaded, BitRate, opt, CompressString2, TestCounter) == -1)
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
        BitRateAr[TestCounter-1] = FileSize(ComboRunAVI2);
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

        if (CompressIVFtoIVF(input2, ComboRunAVI2, opt.MultiThreaded, BitRate, opt, CompressString2, TestCounter) == -1)
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
        BitRateAr[TestCounter-1] = FileSize(ComboRunAVI2);
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
//----------------------------------------Temporarily Back In----------------------------------------------------------------
//----------------------------------------Code Coverage----------------------------------------------------------------
int CodeCoverage(int argc, char *argv[], string WorkingDir, string FilesAr[])
{

    //DATA COVERED AS OF 05-23-2010
    //vp8dx_bitreader_norm
    //vp8_default_zig_zag1d
    //vp8_coef_bands_x
    //vp8_block2left
    //vp8_block2above
    //vp8_block2type
    //vp8_BLOCK2CONTEXT
    //vp8_coef_bands
    //vp8_prev_token_class
    //vp8_default_zig_zag_mask
    //vp8_mb_feature_data_bits
    //vp8_coef_tree
    //Pcat1
    //Pcat2
    //Pcat3
    //Pcat4
    //Pcat5
    //Pcat6
    //cat1
    //cat2
    //cat3
    //cat4
    //cat5
    //cat6
    //kf_y_mode_cts
    //y_mode_cts
    //uv_mode_cts
    //kf_uv_mode_cts
    //bmode_cts
    //sub_mv_ref_prob
    //vp8_mbsplits
    //vp8_mbsplit_count
    //vp8_mbsplit_probs
    //vp8_bmode_tree
    //vp8_ymode_tree
    //vp8_kf_ymode_tree
    //vp8_uv_mode_tree
    //vp8_mbsplit_tree
    //vp8_mv_ref_tree
    //vp8_sub_mv_ref_tree
    //vp8_small_mvtree
    //vp8_mv_update_probs
    //vp8_default_mv_context
    //bilinear_filters
    //sub_pel_filters
    //vp8_mode_contexts
    //kernel5
    //vp8_rv
    //dc_qlookup
    //ac_qlookup
    //bbb
    //vp8_kf_default_bmode_counts
    //vp8d_token_extra_bits2
    //vp8_CoefEncodings
    //vp8_BmodeEncodings
    //vp8_MBsplitEncodings
    //vp8_SmallMVencodings
    //vp8_MVrefEncodingArray
    //vp8_subMVrefEncodingArray
    //vp8_kfYmodeEncodings
    //vp8_UVmodeEncodings
#if defined(DATA_COV)
    char *CompressString = "Code Coverage";

    char *input = argv[2];

    if (argc < 7)
    {
        printf(
            "  CodeCoverage \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <Compress Input File or Decode>\n "
            "          (NoCompress)Decode Input File\n"
            "          (MakeCompression)Compress Input File\n"
            "    <Array To Test 1 - 3 >\n "
            "    <First Varriable in Array To Test>\n "
            "    <Second Varriable in Array To Test - For 2 Dimensional Arrays>\n "

        );

        return 0;

    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////

    string WorkingDirString; // <- All Options need to set a value for this
    string Mode3TestMatch;
    string MainDirString;
    char *MyDir = "CodeCoverage";
    char WorkingDir2[255];
    char WorkingDir3[255];
    char File1[255];

    int TestType = 1;

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        //strcpy(WorkingDir2,WorkingDir.c_str());
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
            printf("Mode3TestMatch: %s MyDir: %s", Mode3TestMatch, MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir5b = WorkingDirString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("CodeCoverageDecCorrectMD5.txt");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("CodeCoverageDecModifiedMD5.txt");
    WorkingDir5b.append(slashCharStr);
    WorkingDir5b.append("CodeCoverageCompression.ivf");

    char CodeCoverageDecCorrect[255];
    char CodeCoverageDecModified[255];
    char CodeCoverageCompression[255];

    snprintf(CodeCoverageDecCorrect, 255, "%s", WorkingDir4.c_str());
    snprintf(CodeCoverageDecModified, 255, "%s", WorkingDir5.c_str());
    snprintf(CodeCoverageCompression, 255, "%s", WorkingDir5b.c_str());

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

    char *input2 = argv[5];

    printf("Code Coverage Test\n");
    fprintf(stderr, "Code Coverage Test\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    opt.TargetBandwidth = BitRate;
    string CompressControl = argv[5];

    if (CompressControl.compare("MakeCompression") == 0)
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0);
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0);
        }
    }

    else
    {
        snprintf(CodeCoverageCompression, 255, "%s", argv[2]);
    }

    int x = 0;
    int y = 0;
    int z = 0;

    if (argc < 8)
    {
        cout << "\nMust Input At least one array variable to check\n";
        return 0;
    }

    x = atoi(argv[7]);

    if (argc > 8)
    {
        y = atoi(argv[8]);
    }

    if (argc > 9)
    {
        z = atoi(argv[9]);
    }

    int StorageInt;
    int StorageInt2;
    char StorageChar;
    short StorageShort;

    cout << "\n";
    cerr << "\n";
    string arrayname = argv[6];
    int StringFound = 0;


    if (arrayname.compare("vp8dx_bitreader_norm") == 0)
    {
        if (x > 255)
        {
            cout << "\nMax Array Value = 255\n";
            return 0;
        }

        cout << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        cerr << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        StringFound = 1;
    }

    /*if(arrayname.compare("vp8_mv_cont_count")==0)
    {
    	if(x > 4)
    	{
    		cout << "\nMax x Array Value = 4\n";
    		return 0;
    	}
    	if(y > 3)
    	{
    		cout << "\nMax y Array Value = 3\n";
    		return 0;
    	}
    	cout << "vp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    	cerr << "vp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    	StringFound = 1;
    }*/
    if (arrayname.compare("vp8_default_zig_zag1d") == 0)
    {
        if (x > 15)
        {
            cout << "\nMax Array Value = 15\n";
            return 0;
        }

        cout << "vp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        cerr << "vp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_coef_bands_x") == 0)
    {
        if (x > 15)
        {
            cout << "\nMax Array Value = 15\n";
            return 0;
        }

        cout << "vp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        cerr << "vp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        StringFound = 1;
    }

    //if(arrayname.compare("vp8_block2context")==0)
    //{
    //	if(x > 74)
    //	{
    //		cout << "\nMax Array Value = 74\n";
    //		return 0;
    //	}
    //	cout << "vp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	cerr << "vp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	StringFound = 1;
    //}

    if (arrayname.compare("vp8_block2left") == 0)
    {
        if (x > 24)
        {
            cout << "\nMax Array Value = 24\n";
            return 0;
        }

        cout << "vp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        cerr << "vp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_block2above") == 0)
    {
        if (x > 24)
        {
            cout << "\nMax Array Value = 24\n";
            return 0;
        }

        cout << "vp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        cerr << "vp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_block2type") == 0)
    {
        if (x > 24)
        {
            cout << "\nMax Array Value = 24\n";
            return 0;
        }

        cout << "vp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        cerr << "vp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_BLOCK2CONTEXT") == 0)
    {
        if (x > 24)
        {
            cout << "\nMax Array Value = 24\n";
            return 0;
        }

        cout << "vp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        cerr << "vp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_coef_bands") == 0) //Char
    {
        if (x > 15)
        {
            cout << "\nMax Array Value = 15\n";
            return 0;
        }

        cout << "vp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        cerr << "vp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_prev_token_class") == 0) //Char
    {
        if (x > 11)
        {
            cout << "\nMax Array Value = 11\n";
            return 0;
        }

        cout << "vp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        cerr << "vp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_default_zig_zag_mask") == 0) //short
    {
        if (x > 15)
        {
            cout << "\nMax Array Value = 15\n";
            return 0;
        }

        cout << "vp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        cerr << "vp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mb_feature_data_bits") == 0)
    {
        if (x > 24)
        {
            cout << "\nMax Array Value = 24\n";
            return 0;
        }

        cout << "vp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        cerr << "vp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_coef_tree") == 0) //Char
    {
        if (x > 21)
        {
            cout << "\nMax Array Value = 21\n";
            return 0;
        }

        cout << "vp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        cerr << "vp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat1") == 0) //Char
    {
        if (x > 0)
        {
            cout << "\nMax Array Value = 0\n";
            return 0;
        }

        cout << "Pcat1[" << x << "] = " << Pcat1[x] << "\n";
        cerr << "Pcat1[" << x << "] = " << Pcat1[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat2") == 0) //Char
    {
        if (x > 1)
        {
            cout << "\nMax Array Value = 1\n";
            return 0;
        }

        cout << "Pcat2[" << x << "] = " << Pcat2[x] << "\n";
        cerr << "Pcat2[" << x << "] = " << Pcat2[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat3") == 0) //Char
    {
        if (x > 2)
        {
            cout << "\nMax Array Value = 2\n";
            return 0;
        }

        cout << "Pcat3[" << x << "] = " << Pcat3[x] << "\n";
        cerr << "Pcat3[" << x << "] = " << Pcat3[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat4") == 0) //Char
    {
        if (x > 3)
        {
            cout << "\nMax Array Value = 3\n";
            return 0;
        }

        cout << "Pcat4[" << x << "] = " << Pcat4[x] << "\n";
        cerr << "Pcat4[" << x << "] = " << Pcat4[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat5") == 0) //Char
    {
        if (x > 4)
        {
            cout << "\nMax Array Value = 4\n";
            return 0;
        }

        cout << "Pcat5[" << x << "] = " << Pcat5[x] << "\n";
        cerr << "Pcat5[" << x << "] = " << Pcat5[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat6") == 0) //Char
    {
        if (x > 10)
        {
            cout << "\nMax Array Value = 10\n";
            return 0;
        }

        cout << "Pcat6[" << x << "] = " << Pcat6[x] << "\n";
        cerr << "Pcat6[" << x << "] = " << Pcat6[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat1") == 0) //Char
    {
        if (x > 1)
        {
            cout << "\nMax Array Value = 1\n";
            return 0;
        }

        cout << "cat1[" << x << "] = " << cat1[x] << "\n";
        cerr << "cat1[" << x << "] = " << cat1[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat2") == 0) //Char
    {
        if (x > 3)
        {
            cout << "\nMax Array Value = 3\n";
            return 0;
        }

        cout << "cat2[" << x << "] = " << cat2[x] << "\n";
        cerr << "cat2[" << x << "] = " << cat2[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat3") == 0) //Char
    {
        if (x > 5)
        {
            cout << "\nMax Array Value = 5\n";
            return 0;
        }

        cout << "cat3[" << x << "] = " << cat3[x] << "\n";
        cerr << "cat3[" << x << "] = " << cat3[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat4") == 0) //Char
    {
        if (x > 7)
        {
            cout << "\nMax Array Value = 7\n";
            return 0;
        }

        cout << "cat4[" << x << "] = " << cat4[x] << "\n";
        cerr << "cat4[" << x << "] = " << cat4[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat5") == 0) //Char
    {
        if (x > 9)
        {
            cout << "\nMax Array Value = 9\n";
            return 0;
        }

        cout << "cat5[" << x << "] = " << cat5[x] << "\n";
        cerr << "cat5[" << x << "] = " << cat5[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat6") == 0) //Char
    {
        if (x > 21)
        {
            cout << "\nMax Array Value = 21\n";
            return 0;
        }

        cout << "cat6[" << x << "] = " << cat6[x] << "\n";
        cerr << "cat6[" << x << "] = " << cat6[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("kf_y_mode_cts") == 0)
    {
        if (x > 4)
        {
            cout << "\nMax Array Value = 4\n";
            return 0;
        }

        cout << "kf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        cerr << "kf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("y_mode_cts") == 0)
    {
        if (x > 4)
        {
            cout << "\nMax Array Value = 4\n";
            return 0;
        }

        cout << "y_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        cerr << "y_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("uv_mode_cts") == 0)
    {
        if (x > 3)
        {
            cout << "\nMax Array Value = 3\n";
            return 0;
        }

        cout << "uv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        cerr << "uv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("kf_uv_mode_cts") == 0)
    {
        if (x > 3)
        {
            cout << "\nMax Array Value = 3\n";
            return 0;
        }

        cout << "kf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        cerr << "kf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("bmode_cts") == 0)
    {
        if (x > 9)
        {
            cout << "\nMax Array Value = 9\n";
            return 0;
        }

        cout << "bmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        cerr << "bmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("sub_mv_ref_prob") == 0)
    {
        if (x > 2)
        {
            cout << "\nMax Array Value = 2\n";
            return 0;
        }

        cout << "sub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        cerr << "sub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        StringFound = 1;
    }

    /*if(arrayname.compare("vp8_sub_mv_ref_prob2")==0)
    {
    	if(x > 4)
    	{
    		cout << "\nMax x Array Value = 4\n";
    		return 0;
    	}
    	if(y > 3)
    	{
    		cout << "\nMax y Array Value = 3\n";
    		return 0;
    	}
    	cout << "vp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    	cerr << "vp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    	StringFound = 1;
    }*/
    if (arrayname.compare("vp8_mbsplits") == 0)
    {
        if (x > 3)
        {
            cout << "\nMax x Array Value = 3\n";
            return 0;
        }

        if (y > 15)
        {
            cout << "\nMax y Array Value = 15\n";
            return 0;
        }

        cout << "vp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        cerr << "vp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mbsplit_count") == 0)
    {
        if (x > 3)
        {
            cout << "\nMax Array Value = 3\n";
            return 0;
        }

        cout << "vp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        cerr << "vp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mbsplit_probs") == 0) //Char
    {
        if (x > 2)
        {
            cout << "\nMax Array Value = 2\n";
            return 0;
        }

        cout << "vp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        cerr << "vp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_bmode_tree") == 0) //Char
    {
        if (x > 17)
        {
            cout << "\nMax Array Value = 17\n";
            return 0;
        }

        cout << "vp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        cerr << "vp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_ymode_tree") == 0) //Char
    {
        if (x > 7)
        {
            cout << "\nMax Array Value = 7\n";
            return 0;
        }

        cout << "vp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        cerr << "vp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_kf_ymode_tree") == 0) //Char
    {
        if (x > 7)
        {
            cout << "\nMax Array Value = 7\n";
            return 0;
        }

        cout << "vp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        cerr << "vp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_uv_mode_tree") == 0) //Char
    {
        if (x > 5)
        {
            cout << "\nMax Array Value = 5\n";
            return 0;
        }

        cout << "vp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        cerr << "vp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mbsplit_tree") == 0) //Char
    {
        if (x > 5)
        {
            cout << "\nMax Array Value = 5\n";
            return 0;
        }

        cout << "vp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        cerr << "vp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mv_ref_tree") == 0) //Char
    {
        if (x > 7)
        {
            cout << "\nMax Array Value = 7\n";
            return 0;
        }

        cout << "vp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        cerr << "vp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_sub_mv_ref_tree") == 0) //Char
    {
        if (x > 5)
        {
            cout << "\nMax Array Value = 5\n";
            return 0;
        }

        cout << "vp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        cerr << "vp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_small_mvtree") == 0) //Char
    {
        if (x > 13)
        {
            cout << "\nMax Array Value = 13\n";
            return 0;
        }

        cout << "vp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        cerr << "vp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        StringFound = 1;
    }

    //if(arrayname.compare("vp8_LowMVtree")==0)//Char
    //{
    //	if(x > 5)
    //	{
    //		cout << "\nMax Array Value = 5\n";
    //		return 0;
    //	}
    //	cout << "vp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	cerr << "vp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	StringFound = 1;
    //}
    //if(arrayname.compare("vp8_HighMVtree")==0)//Char
    //{
    //	if(x > 7)
    //	{
    //		cout << "\nMax Array Value = 7\n";
    //		return 0;
    //	}
    //	cout << "vp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	cerr << "vp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	StringFound = 1;
    //}
    //if(arrayname.compare("vp8_Reverse3bits")==0)
    //{
    //	if(x > 7)
    //	{
    //		cout << "\nMax Array Value = 7\n";
    //		return 0;
    //	}
    //	cout << "vp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	cerr << "vp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	StringFound = 1;
    //}
    //if(arrayname.compare("vp8_Reverse4bits")==0)
    //{
    //	if(x > 15)
    //	{
    //		cout << "\nMax Array Value = 15\n";
    //		return 0;
    //	}
    //	cout << "vp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	cerr << "vp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	StringFound = 1;
    //}
    if (arrayname.compare("vp8_mv_update_probs") == 0)
    {
        if (x > 1)
        {
            cout << "\nMax x Array Value = 1\n";
            return 0;
        }

        if (y > 18)
        {
            cout << "\nMax y Array Value = 18\n";
            return 0;
        }

        cout << "vp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        cerr << "vp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_default_mv_context") == 0)
    {
        if (x > 1)
        {
            cout << "\nMax x Array Value = 1\n";
            return 0;
        }

        if (y > 18)
        {
            cout << "\nMax y Array Value = 18\n";
            return 0;
        }

        cout << "vp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        cerr << "vp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("bilinear_filters") == 0)
    {
        if (x > 7)
        {
            cout << "\nMax x Array Value = 7\n";
            return 0;
        }

        if (y > 1)
        {
            cout << "\nMax y Array Value = 1\n";
            return 0;
        }

        cout << "bilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        cerr << "bilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("sub_pel_filters") == 0)
    {
        if (x > 7)
        {
            cout << "\nMax x Array Value = 7\n";
            return 0;
        }

        if (y > 5)
        {
            cout << "\nMax y Array Value = 5\n";
            return 0;
        }

        cout << "sub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        cerr << "sub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        StringFound = 1;
    }

    //if(arrayname.compare("idctMatrix1")==0)
    //{
    //	if(x > 3)
    //	{
    //		cout << "\nMax x Array Value = 3\n";
    //		return 0;
    //	}
    //	if(y > 3)
    //	{
    //		cout << "\nMax y Array Value = 3\n";
    //		return 0;
    //	}
    //	cout << "idctMatrix1[" << x << "][" << y << "] = " << idctMatrix1[x][y] << "\n";
    //	cerr << "idctMatrix1[" << x << "][" << y << "] = " << idctMatrix1[x][y] << "\n";
    //	StringFound = 1;
    //}
    //if(arrayname.compare("idctMatrix2")==0)
    //{
    //	if(x > 3)
    //	{
    //		cout << "\nMax x Array Value = 3\n";
    //		return 0;
    //	}
    //	if(y > 3)
    //	{
    //		cout << "\nMax y Array Value = 3\n";
    //		return 0;
    //	}
    //	cout << "idctMatrix2[" << x << "][" << y << "] = " << idctMatrix2[x][y] << "\n";
    //	cerr << "idctMatrix2[" << x << "][" << y << "] = " << idctMatrix2[x][y] << "\n";
    //	StringFound = 1;
    //}
    if (arrayname.compare("vp8_mode_contexts") == 0)
    {
        if (x > 5)
        {
            cout << "\nMax x Array Value = 5\n";
            return 0;
        }

        if (y > 3)
        {
            cout << "\nMax y Array Value = 3\n";
            return 0;
        }

        cout << "vp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        cerr << "vp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("kernel5") == 0) //short
    {
        if (x > 4)
        {
            cout << "\nMax Array Value = 4\n";
            return 0;
        }

        cout << "kernel5[" << x << "] = " << kernel5[x] << "\n";
        cerr << "kernel5[" << x << "] = " << kernel5[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_rv") == 0) //short
    {
        if (x > 439)
        {
            cout << "\nMax Array Value = 439\n";
            return 0;
        }

        cout << "vp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        cerr << "vp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("dc_qlookup") == 0)
    {
        if (x > 127)
        {
            cout << "\nMax Array Value = 127\n";
            return 0;
        }

        cout << "dc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        cerr << "dc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("ac_qlookup") == 0)
    {
        if (x > 127)
        {
            cout << "\nMax Array Value = 127\n";
            return 0;
        }

        cout << "ac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        cerr << "ac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("bbb") == 0)
    {
        if (x > 3)
        {
            cout << "\nMax Array Value = 3\n";
            return 0;
        }

        cout << "bbb[" << x << "] = " << bbb[x] << "\n";
        cerr << "bbb[" << x << "] = " << bbb[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_kf_default_bmode_counts") == 0)
    {
        if (x > 9)
        {
            cout << "\nMax x Array Value = 9\n";
            return 0;
        }

        if (y > 9)
        {
            cout << "\nMax y Array Value = 9\n";
            return 0;
        }

        if (z > 9)
        {
            cout << "\nMax z Array Value = 9\n";
            return 0;
        }

        cout << "vp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        cerr << "vp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8d_token_extra_bits2") == 0)
    {
        if (x > 12)
        {
            cout << "\nMax x Array Value = 2\n";
            return 0;
        }

        if (y > 2)
        {
            cout << "\nMax y Array Value = 9\n";
            return 0;
        }

        if (z > 12)
        {
            cout << "\nMax z Array Value = 12\n";
            return 0;
        }

        cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        //				vp8d_token_extra_bits2[0].Probs[1];
        //				vp8d_token_extra_bits2[0].Probs[1];

        StringFound = 1;
    }

    //if(arrayname.compare("nearB")==0)
    //{
    //	if(x > 2)
    //	{
    //		cout << "\nMax x Array Value = 2\n";
    //		return 0;
    //	}
    //	if(y > 3)
    //	{
    //		cout << "\nMax y Array Value = 3\n";
    //		return 0;
    //	}

    //	cout << "nearB[" << x << "].";

    //	if(y == 0)
    //	{
    //		cout << "row = " << nearB[x].row << "\n";
    //	}
    //	if(y == 1)
    //	{
    //		cout << "col = " << nearB[x].col << "\n";
    //	}
    //	if(y == 2)
    //	{
    //		cout << "weight = " << nearB[x].weight << "\n";
    //	}
    //	if(y == 3)
    //	{
    //		cout << "block = " << nearB[x].block << "\n";
    //	}

    //	cerr << "nearB[" << x << "].";

    //	if(y == 0)
    //	{
    //		cerr << "row = " << nearB[x].row << "\n";
    //	}
    //	if(y == 1)
    //	{
    //		cerr << "col = " << nearB[x].col << "\n";
    //	}
    //	if(y == 2)
    //	{
    //		cerr << "weight = " << nearB[x].weight << "\n";
    //	}
    //	if(y == 3)
    //	{
    //		cerr << "block = " << nearB[x].block << "\n";
    //	}

    //	StringFound = 1;
    //}
    if (arrayname.compare("vp8_CoefEncodings") == 0)
    {
        if (x > 11)
        {
            cout << "\nMax Array Value = 11\n";
            return 0;
        }

        cout << "vp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        cerr << "vp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_BmodeEncodings") == 0)
    {
        if (x > 10)
        {
            cout << "\nMax Array Value = 10\n";
            return 0;
        }

        cout << "vp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        cerr << "vp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_MBsplitEncodings") == 0)
    {
        if (x > 4)
        {
            cout << "\nMax Array Value = 4\n";
            return 0;
        }

        cout << "vp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        cerr << "vp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_SmallMVencodings") == 0)
    {
        if (x > 8)
        {
            cout << "\nMax Array Value = 8\n";
            return 0;
        }

        cout << "vp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        cerr << "vp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_MVrefEncodingArray") == 0)
    {
        if (x > 5)
        {
            cout << "\nMax Array Value = 5\n";
            return 0;
        }

        cout << "vp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        cerr << "vp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_subMVrefEncodingArray") == 0)
    {
        if (x > 4)
        {
            cout << "\nMax Array Value = 4\n";
            return 0;
        }

        cout << "vp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        cerr << "vp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_kfYmodeEncodings") == 0)
    {
        if (x > 5)
        {
            cout << "\nMax Array Value = 5\n";
            return 0;
        }

        cout << "vp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        cerr << "vp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_UVmodeEncodings") == 0)
    {
        if (x > 4)
        {
            cout << "\nMax Array Value = 4\n";
            return 0;
        }

        cout << "vp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        cerr << "vp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        StringFound = 1;
    }





    if (StringFound == 0)
    {
        cout << "Array: " << arrayname << " Not Found - Valid Selections Include:\n";
        cout << " vp8dx_bitreader_norm[128]" << "\n";
        //cout << " vp8_mv_cont_count[5][4]" << "\n";
        cout << " vp8_default_zig_zag1d[16]" << "\n";
        cout << " vp8_coef_bands_x[16]" << "\n";
        //cout << " vp8_block2context[25*3]" << "\n";
        cout << " vp8_block2left[25]" << "\n";
        cout << " vp8_block2above[25]" << "\n";
        cout << " vp8_block2type[25]" << "\n";
        cout << " vp8_BLOCK2CONTEXT[25]" << "\n";
        cout << " vp8_coef_bands[16]" << "\n";
        cout << " vp8_prev_token_class[12]" << "\n";
        cout << " vp8_default_zig_zag_mask[16]" << "\n";
        cout << " vp8_mb_feature_data_bits[2]" << "\n";
        cout << " vp8_coef_tree[22]" << "\n";
        cout << " Pcat1[1]" << "\n";
        cout << " Pcat2[2]" << "\n";
        cout << " Pcat3[3]" << "\n";
        cout << " Pcat4[4]" << "\n";
        cout << " Pcat5[5]" << "\n";
        cout << " Pcat6[11]" << "\n";
        cout << " cat1[2]" << "\n";
        cout << " cat2[4]" << "\n";
        cout << " cat3[6]" << "\n";
        cout << " cat4[8]" << "\n";
        cout << " cat5[10]" << "\n";
        cout << " cat6[22]" << "\n";
        cout << " kf_y_mode_cts[5]" << "\n";
        cout << " y_mode_cts[5]" << "\n";
        cout << " uv_mode_cts[4]" << "\n";
        cout << " kf_uv_mode_cts[4]" << "\n";
        cout << " bmode_cts[10]" << "\n";
        cout << " sub_mv_ref_prob [3]" << "\n";
        //cout << " vp8_sub_mv_ref_prob2[5][3]" << "\n";
        cout << " vp8_mbsplits [4]" << "\n";
        cout << " int vp8_mbsplit_count[4]" << "\n";
        cout << " VP8_Prob vp8_mbsplit_probs[3]" << "\n";
        cout << " vp8_bmode_tree[18]" << "\n";
        cout << " vp8_ymode_tree[8]" << "\n";
        cout << " vp8_kf_ymode_tree[8]" << "\n";
        cout << " vp8_uv_mode_tree[6]" << "\n";
        cout << " vp8_mbsplit_tree[6]" << "\n";
        cout << " vp8_mv_ref_tree[8]" << "\n";
        cout << " vp8_sub_mv_ref_tree[6]" << "\n";
        cout << " vp8_small_mvtree [14]" << "\n";
        //cout << " vp8_LowMVtree [6]" << "\n";
        //cout << " vp8_HighMVtree [8]" << "\n";
        //cout << " vp8_Reverse3bits[8]" << "\n";
        //cout << " vp8_Reverse4bits[16]" << "\n";
        cout << " vp8_mv_update_probs[2]" << "\n";
        cout << " vp8_default_mv_context[2]" << "\n";
        cout << " bilinear_filters[8][2]" << "\n";
        cout << " sub_pel_filters[8][6]" << "\n";
        cout << " idctMatrix1[4][4]" << "\n";
        cout << " idctMatrix2[4][4]" << "\n";
        cout << " vp8_mode_contexts[6][4]" << "\n";
        cout << " vp8_kf_default_bmode_counts [10] [10] [10]" << "\n";
        cout << " kernel5[5]" << "\n";
        cout << " vp8_rv[440]" << "\n";
        cout << " dc_qlookup[128]" << "\n";
        cout << " ac_qlookup[128]" << "\n";
        cout << " bbb[4]" << "\n";
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //DecompressIVFtoIVF(CodeCoverageCompression, CodeCoverageDecCorrect);
    DecComputeMD5(CodeCoverageCompression, CodeCoverageDecCorrect);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (arrayname.compare("vp8dx_bitreader_norm") == 0)
    {
        cout << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        cerr << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8dx_bitreader_norm[x];
        vp8dx_bitreader_norm[x] = vp8dx_bitreader_norm[x] + 1;
        cout << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        cerr << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
    }

    //if(arrayname.compare("vp8_mv_cont_count")==0)
    //{
    //	cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //	cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //	cout << "\n\n";
    //	cerr << "\n\n";
    //	StorageInt = vp8_mv_cont_count[x][y];
    //	vp8_mv_cont_count[x][y] = vp8_mv_cont_count[x][y]+1;
    //	cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //	cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //}
    if (arrayname.compare("vp8_default_zig_zag1d") == 0)
    {
        cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_default_zig_zag1d[x];
        vp8_default_zig_zag1d[x] = vp8_default_zig_zag1d[x]  + 20;
        cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands_x") == 0)
    {
        cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_coef_bands_x[x];
        vp8_coef_bands_x[x] = -1;
        //vp8_coef_bands_x[x] = vp8_coef_bands_x[x]  + 20;
        cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
    }

    //if(arrayname.compare("vp8_block2context")==0)
    //{
    //	cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	cout << "\n\n";
    //	cerr << "\n\n";
    //	StorageInt = vp8_block2context[x];
    //	vp8_block2context[x] = vp8_block2context[x]  + 20;
    //	cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //}
    if (arrayname.compare("vp8_block2left") == 0)
    {
        cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        StorageInt = vp8_block2left[x];
        vp8_block2left[x] = vp8_block2left[x]  + 20;
        cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
    }

    if (arrayname.compare("vp8_block2above") == 0)
    {
        cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_block2above[x];
        vp8_block2above[x] = vp8_block2above[x]  + 20;
        cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
    }

    if (arrayname.compare("vp8_block2type") == 0)
    {
        cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_block2type[x];
        vp8_block2type[x] = vp8_block2type[x]  + 20;
        cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
    }

    if (arrayname.compare("vp8_BLOCK2CONTEXT") == 0)
    {
        cout << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        cerr << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_BLOCK2CONTEXT[x];
        vp8_BLOCK2CONTEXT[x] = vp8_BLOCK2CONTEXT[x]  + 20;
        cout << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        cerr << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands") == 0) //Char
    {
        cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_coef_bands[x];
        vp8_coef_bands[x] = vp8_coef_bands[x]  + 20;
        //unsigned char nine = '9';
        //cout << nine << "\n";
        //vp8_coef_bands[x] = nine;
        cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
    }

    if (arrayname.compare("vp8_prev_token_class") == 0) //Char
    {
        cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_prev_token_class[x];
        vp8_prev_token_class[x] = vp8_prev_token_class[x]  + 20;
        //unsigned char nine = '9';
        //cout << nine << "\n";
        //vp8_coef_bands[x] = nine;
        cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
    }

    if (arrayname.compare("vp8_default_zig_zag_mask") == 0) //short
    {
        cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageShort = vp8_default_zig_zag_mask[x];
        vp8_default_zig_zag_mask[x] = vp8_default_zig_zag_mask[x]  + 20;
        cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
    }

    if (arrayname.compare("vp8_mb_feature_data_bits") == 0)
    {
        cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_mb_feature_data_bits[x];
        vp8_mb_feature_data_bits[x] = vp8_mb_feature_data_bits[x]  + 20;
        cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_tree") == 0) //Char
    {
        cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_coef_tree[x];
        vp8_coef_tree[x] = vp8_coef_tree[x]  + 20;
        cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
    }

    if (arrayname.compare("Pcat1") == 0) //Char
    {
        cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = Pcat1[x];
        //Pcat1[x] = Pcat1[x]  + 20;
        Pcat1[x] = 2000;
        cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
    }

    if (arrayname.compare("Pcat2") == 0) //Char
    {
        cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = Pcat2[x];
        //Pcat2[x] = Pcat2[x]  + 20;
        Pcat2[x] = -1;
        cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
    }

    if (arrayname.compare("Pcat3") == 0) //Char
    {
        cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = Pcat3[x];
        //Pcat3[x] = Pcat3[x]  + 20;
        Pcat3[x] = -1;
        cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
    }

    if (arrayname.compare("Pcat4") == 0) //Char
    {
        cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = Pcat4[x];
        //Pcat1[x] = Pcat4[x]  + 20;
        Pcat4[x] = -1;
        cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
    }

    if (arrayname.compare("Pcat5") == 0) //Char
    {
        cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = Pcat5[x];
        //Pcat5[x] = Pcat5[x]  + 20;
        Pcat5[x] = -1;
        cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
    }

    if (arrayname.compare("Pcat6") == 0) //Char
    {
        cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = Pcat6[x];
        //Pcat6[x] = Pcat6[x]  + 20;
        Pcat6[x] = -1;
        cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
    }

    if (arrayname.compare("cat1") == 0) //Char
    {
        cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = cat1[x];
        //cat1[x] = cat1[x]  + 20;
        cat1[x] = -1;
        cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
    }

    if (arrayname.compare("cat2") == 0) //Char
    {
        cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = cat2[x];
        //cat2[x] = cat2[x]  + 20;
        cat2[x] = -1;
        cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
    }

    if (arrayname.compare("cat3") == 0) //Char
    {
        cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = cat3[x];
        //cat3[x] = cat3[x]  + 20;
        cat3[x] = -1;
        cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
    }

    if (arrayname.compare("cat4") == 0) //Char
    {
        cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = cat4[x];
        //cat4[x] = cat4[x]  + 20;
        cat4[x] = -1;
        cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
    }

    if (arrayname.compare("cat5") == 0) //Char
    {
        cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = cat5[x];
        //cat5[x] = cat5[x]  + 20;
        cat5[x] = -1;
        cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
    }

    if (arrayname.compare("cat6") == 0) //Char
    {
        cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = cat6[x];
        //cat6[x] = cat6[x]  + 20;
        cat6[x] = -1;
        cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
    }

    if (arrayname.compare("kf_y_mode_cts") == 0)
    {
        cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = kf_y_mode_cts[x];
        kf_y_mode_cts[x] = kf_y_mode_cts[x]  + 20;
        cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("y_mode_cts") == 0)
    {
        cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = y_mode_cts[x];
        y_mode_cts[x] = y_mode_cts[x]  + 20;
        cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("uv_mode_cts") == 0)
    {
        cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = uv_mode_cts[x];
        uv_mode_cts[x] = uv_mode_cts[x]  + 20;
        cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("kf_uv_mode_cts") == 0)
    {
        cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = kf_uv_mode_cts[x];
        kf_uv_mode_cts[x] = kf_uv_mode_cts[x]  + 20;
        cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("bmode_cts") == 0)
    {
        cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = bmode_cts[x];
        //bmode_cts[x] = bmode_cts[x]  + 2000;
        bmode_cts[x] = 0;
        cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
    }

    if (arrayname.compare("sub_mv_ref_prob") == 0)
    {
        cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = sub_mv_ref_prob[x];
        sub_mv_ref_prob[x] = sub_mv_ref_prob[x]  + 20;
        cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
    }

    /*if(arrayname.compare("vp8_sub_mv_ref_prob2")==0)
    {
    	cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    	cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    	cout << "\n\n";
    	cerr << "\n\n";
    	StorageInt = vp8_sub_mv_ref_prob2[x][y];
    	vp8_sub_mv_ref_prob2[x][y] = vp8_sub_mv_ref_prob2[x][y]+1;
    	cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    	cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    }*/
    if (arrayname.compare("vp8_mbsplits") == 0)
    {
        cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_mbsplits[x][y];
        vp8_mbsplits[x][y] = vp8_mbsplits[x][y] + 1;
        cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_count") == 0)
    {
        cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_mbsplit_count[x];
        vp8_mbsplit_count[x] = vp8_mbsplit_count[x]  + 20;
        cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_probs") == 0) //Char
    {
        cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_mbsplit_probs[x];
        vp8_mbsplit_probs[x] = vp8_mbsplit_probs[x]  + 20;
        cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
    }

    if (arrayname.compare("vp8_bmode_tree") == 0) //Char
    {
        cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_mbsplit_probs[x];
        vp8_mbsplit_probs[x] = vp8_mbsplit_probs[x]  + 20;
        cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_ymode_tree") == 0) //Char
    {
        cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_ymode_tree[x];
        vp8_ymode_tree[x] = vp8_ymode_tree[x]  + 20;
        cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_ymode_tree") == 0) //Char
    {
        cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_kf_ymode_tree[x];
        vp8_kf_ymode_tree[x] = vp8_kf_ymode_tree[x]  + 20;
        cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_uv_mode_tree") == 0) //Char
    {
        cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_uv_mode_tree[x];
        vp8_uv_mode_tree[x] = vp8_uv_mode_tree[x]  + 20;
        cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_tree") == 0) //Char
    {
        cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_mbsplit_tree[x];
        vp8_mbsplit_tree[x] = vp8_mbsplit_tree[x]  + 20;
        cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mv_ref_tree") == 0) //Char
    {
        cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_mv_ref_tree[x];
        vp8_mv_ref_tree[x] = vp8_mv_ref_tree[x]  + 20;
        cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_sub_mv_ref_tree") == 0) //Char
    {
        cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_sub_mv_ref_tree[x];
        vp8_sub_mv_ref_tree[x] = vp8_sub_mv_ref_tree[x]  + 20;
        cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_small_mvtree") == 0) //Char
    {
        cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_small_mvtree[x];
        vp8_small_mvtree[x] = vp8_small_mvtree[x]  + 20;
        cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
    }

    //if(arrayname.compare("vp8_LowMVtree")==0)//Char
    //{
    //	cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	cout << "\n\n";
    //	cerr << "\n\n";
    //	StorageChar = vp8_LowMVtree[x];
    //	vp8_LowMVtree[x] = vp8_LowMVtree[x]  + 20;
    //	cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_HighMVtree")==0)//Char
    //{
    //	cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	cout << "\n\n";
    //	cerr << "\n\n";
    //	StorageChar = vp8_HighMVtree[x];
    //	vp8_HighMVtree[x] = vp8_HighMVtree[x]  + 20;
    //	cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse3bits")==0)
    //{
    //	cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	cout << "\n\n";
    //	cerr << "\n\n";
    //	StorageInt = vp8_Reverse3bits[x];
    //	vp8_Reverse3bits[x] = vp8_Reverse3bits[x]  + 20;
    //	cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse4bits")==0)
    //{
    //	cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	cout << "\n\n";
    //	cerr << "\n\n";
    //	StorageInt = vp8_Reverse4bits[x];
    //	vp8_Reverse4bits[x] = vp8_Reverse4bits[x]  + 20;
    //	cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //}
    if (arrayname.compare("vp8_mv_update_probs") == 0)
    {
        cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_mv_update_probs[x][y];
        vp8_mv_update_probs[x][y] = vp8_mv_update_probs[x][y] + 1;
        cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
    }

    if (arrayname.compare("vp8_default_mv_context") == 0)
    {
        cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageChar = vp8_default_mv_context[x][y];
        vp8_default_mv_context[x][y] = vp8_default_mv_context[x][y] + 1;
        cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
    }

    if (arrayname.compare("bilinear_filters") == 0)
    {
        cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = bilinear_filters[x][y];
        bilinear_filters[x][y] = bilinear_filters[x][y] + 1;
        cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
    }

    if (arrayname.compare("sub_pel_filters") == 0)
    {
        cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageShort = sub_pel_filters[x][y];
        sub_pel_filters[x][y] = sub_pel_filters[x][y] + 1;
        cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mode_contexts") == 0)
    {
        cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_mode_contexts[x][y];
        vp8_mode_contexts[x][y] = vp8_mode_contexts[x][y] + 1;
        cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
    }

    if (arrayname.compare("kernel5") == 0) //short
    {
        cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageShort = kernel5[x];
        kernel5[x] = kernel5[x]  + 20;
        cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
    }

    if (arrayname.compare("vp8_rv") == 0) //short
    {
        cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageShort = vp8_rv[x];
        vp8_rv[x] = vp8_rv[x]  + 20;
        cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
    }

    if (arrayname.compare("dc_qlookup") == 0)
    {
        cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        cerr << "\nrdc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = dc_qlookup[x];
        dc_qlookup[x] = dc_qlookup[x]  + 2000;
        //dc_qlookup[x] = dc_qlookup[x]  + 20;
        //dc_qlookup[x] = - 100;
        cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        cerr << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
    }

    if (arrayname.compare("ac_qlookup") == 0)
    {
        cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = ac_qlookup[x];
        //ac_qlookup[x] = ac_qlookup[x]  + 2000;
        //ac_qlookup[x] = ac_qlookup[x]  + 20;
        ac_qlookup[x] = -100;
        cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
    }

    if (arrayname.compare("bbb") == 0)
    {
        cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = bbb[x];
        bbb[x] = bbb[x]  + 20;
        cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_default_bmode_counts") == 0)
    {
        cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_kf_default_bmode_counts[x][y][z];
        vp8_kf_default_bmode_counts[x][y][z] = vp8_kf_default_bmode_counts[x][y][z] + 20;
        cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
    }

    if (arrayname.compare("vp8d_token_extra_bits2") == 0)
    {
        cout << "\n";
        cerr << "\n";

        cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        cout << "\n\n";
        cerr << "\n\n";

        if (y == 0)
        {
            StorageInt = vp8d_token_extra_bits2[x].MinVal;
            vp8d_token_extra_bits2[x].MinVal = vp8d_token_extra_bits2[x].MinVal + 20;
        }

        if (y == 1)
        {
            StorageInt = vp8d_token_extra_bits2[x].Length;
            vp8d_token_extra_bits2[x].Length = vp8d_token_extra_bits2[x].Length + 20;
        }

        if (y == 2)
        {
            StorageInt = vp8d_token_extra_bits2[x].Probs[z];
            vp8d_token_extra_bits2[x].Probs[z] = vp8d_token_extra_bits2[x].Probs[z] + 20;
        }


        cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }
    }

    /*if(arrayname.compare("nearB")==0)
    {
    	cout << "\n";
    	cerr << "\n";

    	cout << "nearB[" << x << "].";

    	if(y == 0)
    	{
    		cout << "row = " << nearB[x].row << "\n";
    	}
    	if(y == 1)
    	{
    		cout << "col = " << nearB[x].col << "\n";
    	}
    	if(y == 2)
    	{
    		cout << "weight = " << nearB[x].weight << "\n";
    	}
    	if(y == 3)
    	{
    		cout << "block = " << nearB[x].block << "\n";
    	}

    	cerr << "nearB[" << x << "].";

    	if(y == 0)
    	{
    		cerr << "row = " << nearB[x].row << "\n";
    	}
    	if(y == 1)
    	{
    		cerr << "col = " << nearB[x].col << "\n";
    	}
    	if(y == 2)
    	{
    		cerr << "weight = " << nearB[x].weight << "\n";
    	}
    	if(y == 3)
    	{
    		cerr << "block = " << nearB[x].block << "\n";
    	}

    	cout << "\n\n";
    	cerr << "\n\n";

    	if(y == 0)
    	{
    		StorageInt = nearB[x].row;
    		nearB[x].row = nearB[x].row+20;
    	}
    	if(y == 1)
    	{
    		StorageInt = nearB[x].col;
    		nearB[x].col = nearB[x].col+20;
    	}
    	if(y == 2)
    	{
    		StorageInt = nearB[x].weight;
    		nearB[x].weight = nearB[x].weight+20;
    	}
    	if(y == 3)
    	{
    		StorageInt = nearB[x].block;
    		nearB[x].block = nearB[x].block+20;
    	}

    	cout << "nearB[" << x << "].";

    	if(y == 0)
    	{
    		cout << "row = " << nearB[x].row << "\n";
    	}
    	if(y == 1)
    	{
    		cout << "col = " << nearB[x].col << "\n";
    	}
    	if(y == 2)
    	{
    		cout << "weight = " << nearB[x].weight << "\n";
    	}
    	if(y == 3)
    	{
    		cout << "block = " << nearB[x].block << "\n";
    	}

    	cerr << "nearB[" << x << "].";

    	if(y == 0)
    	{
    		cerr << "row = " << nearB[x].row << "\n";
    	}
    	if(y == 1)
    	{
    		cerr << "col = " << nearB[x].col << "\n";
    	}
    	if(y == 2)
    	{
    		cerr << "weight = " << nearB[x].weight << "\n";
    	}
    	if(y == 3)
    	{
    		cerr << "block = " << nearB[x].block << "\n";
    	}
    }*/

    if (arrayname.compare("vp8_CoefEncodings") == 0)
    {
        cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_CoefEncodings[x].value;
        StorageInt2 = vp8_CoefEncodings[x].Len;
        vp8_CoefEncodings[x].value = vp8_CoefEncodings[x].value  + 2000;
        vp8_CoefEncodings[x].Len = vp8_CoefEncodings[x].Len  + 2000;
        cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_BmodeEncodings") == 0)
    {
        cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_BmodeEncodings[x].value;
        StorageInt2 = vp8_BmodeEncodings[x].Len;
        vp8_BmodeEncodings[x].value = vp8_BmodeEncodings[x].value  + 2000;
        vp8_BmodeEncodings[x].Len = vp8_BmodeEncodings[x].Len  + 2000;
        cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MBsplitEncodings") == 0)
    {
        cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_MBsplitEncodings[x].value;
        StorageInt2 = vp8_MBsplitEncodings[x].Len;
        vp8_MBsplitEncodings[x].value = vp8_MBsplitEncodings[x].value  + 2000;
        vp8_MBsplitEncodings[x].Len = vp8_MBsplitEncodings[x].Len  + 2000;
        cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_SmallMVencodings") == 0)
    {
        cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_SmallMVencodings[x].value;
        StorageInt2 = vp8_SmallMVencodings[x].Len;
        vp8_SmallMVencodings[x].value = vp8_SmallMVencodings[x].value  + 2000;
        vp8_SmallMVencodings[x].Len = vp8_SmallMVencodings[x].Len + 2000;
        cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MVrefEncodingArray") == 0)
    {
        cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_MVrefEncodingArray[x].value;
        StorageInt2 = vp8_MVrefEncodingArray[x].Len;
        vp8_MVrefEncodingArray[x].value = vp8_MVrefEncodingArray[x].value  + 2000;
        vp8_MVrefEncodingArray[x].Len = vp8_MVrefEncodingArray[x].Len  + 2000;
        cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_subMVrefEncodingArray") == 0)
    {
        cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_subMVrefEncodingArray[x].value;
        StorageInt2 = vp8_subMVrefEncodingArray[x].Len;
        vp8_subMVrefEncodingArray[x].value = vp8_subMVrefEncodingArray[x].value  + 2000;
        vp8_subMVrefEncodingArray[x].Len = vp8_subMVrefEncodingArray[x].Len  + 2000;
        cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_kfYmodeEncodings") == 0)
    {
        cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_kfYmodeEncodings[x].value;
        StorageInt2 = vp8_kfYmodeEncodings[x].Len;
        vp8_kfYmodeEncodings[x].value = vp8_kfYmodeEncodings[x].value  + 2000;
        vp8_kfYmodeEncodings[x].Len = vp8_kfYmodeEncodings[x].Len  + 2000;
        cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_UVmodeEncodings") == 0)
    {
        cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        cout << "\n\n";
        cerr << "\n\n";
        StorageInt = vp8_UVmodeEncodings[x].value;
        StorageInt2 = vp8_UVmodeEncodings[x].Len;
        vp8_UVmodeEncodings[x].value = vp8_UVmodeEncodings[x].value  + 2000;
        vp8_UVmodeEncodings[x].Len = vp8_UVmodeEncodings[x].Len + 2000;
        cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
    }



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //DecompressIVFtoIVF(CodeCoverageCompression, CodeCoverageDecModified);
    DecComputeMD5(CodeCoverageCompression, CodeCoverageDecModified);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (arrayname.compare("vp8dx_bitreader_norm") == 0)
    {
        cout << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        cerr << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        vp8dx_bitreader_norm[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        cerr << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
    }

    //if(arrayname.compare("vp8_mv_cont_count")==0)
    //{
    //	cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //	cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //	vp8_mv_cont_count[x][y]=StorageInt;
    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";
    //	cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //	cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //}
    if (arrayname.compare("vp8_default_zig_zag1d") == 0)
    {
        cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        vp8_default_zig_zag1d[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands_x") == 0)
    {
        cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        vp8_coef_bands_x[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
    }

    //if(arrayname.compare("vp8_block2context")==0)
    //{
    //	cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	vp8_block2context[x] = StorageInt;
    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";
    //	cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //	cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //}
    if (arrayname.compare("vp8_block2left") == 0)
    {
        cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        vp8_block2left[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
    }

    if (arrayname.compare("vp8_block2above") == 0)
    {
        cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        vp8_block2above[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
    }

    if (arrayname.compare("vp8_block2type") == 0)
    {
        cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        vp8_block2type[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
    }

    if (arrayname.compare("vp8_BLOCK2CONTEXT") == 0)
    {
        cout << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        cerr << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        vp8_BLOCK2CONTEXT[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        cerr << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands") == 0) //Char
    {
        cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        vp8_coef_bands[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
    }

    if (arrayname.compare("vp8_prev_token_class") == 0) //Char
    {
        cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        vp8_prev_token_class[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
    }

    if (arrayname.compare("vp8_default_zig_zag_mask") == 0) //short
    {
        cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        vp8_default_zig_zag_mask[x] = StorageShort;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
    }

    if (arrayname.compare("vp8_mb_feature_data_bits") == 0)
    {
        cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        vp8_mb_feature_data_bits[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_tree") == 0) //Char
    {
        cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        vp8_coef_tree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
    }

    if (arrayname.compare("Pcat1") == 0) //Char
    {
        cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        Pcat1[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
    }

    if (arrayname.compare("Pcat2") == 0) //Char
    {
        cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        Pcat2[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
    }

    if (arrayname.compare("Pcat3") == 0) //Char
    {
        cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        Pcat3[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
    }

    if (arrayname.compare("Pcat4") == 0) //Char
    {
        cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        Pcat4[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
    }

    if (arrayname.compare("Pcat5") == 0) //Char
    {
        cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        Pcat5[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
    }

    if (arrayname.compare("Pcat6") == 0) //Char
    {
        cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        Pcat6[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
    }

    if (arrayname.compare("cat1") == 0) //Char
    {
        cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        cat1[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
    }

    if (arrayname.compare("cat2") == 0) //Char
    {
        cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        cat2[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
    }

    if (arrayname.compare("cat3") == 0) //Char
    {
        cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        cat3[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
    }

    if (arrayname.compare("cat4") == 0) //Char
    {
        cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        Pcat4[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
    }

    if (arrayname.compare("cat5") == 0) //Char
    {
        cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        Pcat5[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
    }

    if (arrayname.compare("cat6") == 0) //Char
    {
        cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        cat6[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
    }

    if (arrayname.compare("kf_y_mode_cts") == 0)
    {
        cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        kf_y_mode_cts[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("y_mode_cts") == 0)
    {
        cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        y_mode_cts[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("uv_mode_cts") == 0)
    {
        cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        uv_mode_cts[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("kf_uv_mode_cts") == 0)
    {
        cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        kf_uv_mode_cts[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("bmode_cts") == 0)
    {
        cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        bmode_cts[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
    }

    if (arrayname.compare("sub_mv_ref_prob") == 0)
    {
        cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        sub_mv_ref_prob[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
    }

    //if(arrayname.compare("vp8_sub_mv_ref_prob2")==0)
    //{
    //	cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //	cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //	vp8_sub_mv_ref_prob2[x][y]=StorageInt;
    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";
    //	cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //	cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //}
    if (arrayname.compare("vp8_mbsplits") == 0)
    {
        cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        vp8_mbsplits[x][y] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_count") == 0)
    {
        cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        vp8_mbsplit_count[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_probs") == 0) //Char
    {
        cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        vp8_mbsplit_probs[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
    }

    if (arrayname.compare("vp8_bmode_tree") == 0) //Char
    {
        cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        vp8_mbsplit_probs[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_ymode_tree") == 0) //Char
    {
        cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        vp8_ymode_tree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_ymode_tree") == 0) //Char
    {
        cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        vp8_kf_ymode_tree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_uv_mode_tree") == 0) //Char
    {
        cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        vp8_uv_mode_tree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_tree") == 0) //Char
    {
        cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        vp8_mbsplit_tree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mv_ref_tree") == 0) //Char
    {
        cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        vp8_mv_ref_tree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_sub_mv_ref_tree") == 0) //Char
    {
        cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        vp8_sub_mv_ref_tree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_small_mvtree") == 0) //Char
    {
        cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        vp8_small_mvtree[x] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
    }

    //if(arrayname.compare("vp8_LowMVtree")==0)//Char
    //{
    //	cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	vp8_LowMVtree[x] = StorageChar;
    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";
    //	cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //	cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_HighMVtree")==0)//Char
    //{
    //	cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	vp8_HighMVtree[x] = StorageChar;
    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";
    //	cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //	cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse3bits")==0)
    //{
    //	cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	vp8_Reverse3bits[x] = StorageInt;
    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";
    //	cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //	cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse4bits")==0)
    //{
    //	cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	vp8_Reverse4bits[x] = StorageInt;
    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";
    //	cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //	cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //}
    if (arrayname.compare("vp8_mv_update_probs") == 0)
    {
        cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        vp8_mv_update_probs[x][y] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
    }

    if (arrayname.compare("vp8_default_mv_context") == 0)
    {
        cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        vp8_default_mv_context[x][y] = StorageChar;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
    }

    if (arrayname.compare("bilinear_filters") == 0)
    {
        cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        bilinear_filters[x][y] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
    }

    if (arrayname.compare("sub_pel_filters") == 0)
    {
        cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        sub_pel_filters[x][y] = StorageShort;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mode_contexts") == 0)
    {
        cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        vp8_mode_contexts[x][y] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
    }

    if (arrayname.compare("kernel5") == 0) //short
    {
        cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        kernel5[x] = StorageShort;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
    }

    if (arrayname.compare("vp8_rv") == 0) //short
    {
        cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        vp8_rv[x] = StorageShort;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
    }

    if (arrayname.compare("dc_qlookup") == 0) //short
    {
        cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        cerr << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        dc_qlookup[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        cerr << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
    }

    if (arrayname.compare("ac_qlookup") == 0)
    {
        cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        ac_qlookup[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
    }

    if (arrayname.compare("bbb") == 0)
    {
        cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        bbb[x] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_default_bmode_counts") == 0)
    {
        cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        vp8_kf_default_bmode_counts[x][y][z] = StorageInt;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
    }

    if (arrayname.compare("vp8d_token_extra_bits2") == 0)
    {
        cout << "\n";
        cerr << "\n";

        cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        if (y == 0)
        {
            vp8d_token_extra_bits2[x].MinVal = StorageInt;
        }

        if (y == 1)
        {
            vp8d_token_extra_bits2[x].Length = StorageInt;
        }

        if (y == 2)
        {
            vp8d_token_extra_bits2[x].Probs[z] = StorageInt;
        }

        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";

        cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }
    }

    //if(arrayname.compare("nearB")==0)
    //{
    //	cout << "\n";
    //	cerr << "\n";

    //	cout << "nearB[" << x << "].";

    //	if(y == 0)
    //	{
    //		cout << "row = " << nearB[x].row << "\n";
    //	}
    //	if(y == 1)
    //	{
    //		cout << "col = " << nearB[x].col << "\n";
    //	}
    //	if(y == 2)
    //	{
    //		cout << "weight = " << nearB[x].weight << "\n";
    //	}
    //	if(y == 3)
    //	{
    //		cout << "block = " << nearB[x].block << "\n";
    //	}

    //	cerr << "nearB[" << x << "].";

    //	if(y == 0)
    //	{
    //		cerr << "row = " << nearB[x].row << "\n";
    //	}
    //	if(y == 1)
    //	{
    //		cerr << "col = " << nearB[x].col << "\n";
    //	}
    //	if(y == 2)
    //	{
    //		cerr << "weight = " << nearB[x].weight << "\n";
    //	}
    //	if(y == 3)
    //	{
    //		cerr << "block = " << nearB[x].block << "\n";
    //	}

    //	//vp8_kf_default_bmode_counts[x][y][z]=StorageInt;
    //	if(y == 0)
    //	{
    //		nearB[x].row = StorageInt;
    //	}
    //	if(y == 1)
    //	{
    //		nearB[x].col = StorageInt;
    //	}
    //	if(y == 2)
    //	{
    //		nearB[x].weight = StorageInt;
    //	}
    //	if(y == 3)
    //	{
    //		nearB[x].block = StorageInt;
    //	}

    //	cout << "\nArray Var Reset to:";
    //	cerr << "\nArray Var Reset to:";

    //	cout << "nearB[" << x << "].";

    //	if(y == 0)
    //	{
    //		cout << "row = " << nearB[x].row << "\n";
    //	}
    //	if(y == 1)
    //	{
    //		cout << "col = " << nearB[x].col << "\n";
    //	}
    //	if(y == 2)
    //	{
    //		cout << "weight = " << nearB[x].weight << "\n";
    //	}
    //	if(y == 3)
    //	{
    //		cout << "block = " << nearB[x].block << "\n";
    //	}

    //	cerr << "nearB[" << x << "].";

    //	if(y == 0)
    //	{
    //		cerr << "row = " << nearB[x].row << "\n";
    //	}
    //	if(y == 1)
    //	{
    //		cerr << "col = " << nearB[x].col << "\n";
    //	}
    //	if(y == 2)
    //	{
    //		cerr << "weight = " << nearB[x].weight << "\n";
    //	}
    //	if(y == 3)
    //	{
    //		cerr << "block = " << nearB[x].block << "\n";
    //	}
    //}
    if (arrayname.compare("vp8_CoefEncodings") == 0)
    {
        cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        vp8_CoefEncodings[x].value = StorageInt;
        vp8_CoefEncodings[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_BmodeEncodings") == 0)
    {
        cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        vp8_BmodeEncodings[x].value = StorageInt;
        vp8_BmodeEncodings[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MBsplitEncodings") == 0)
    {
        cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        vp8_MBsplitEncodings[x].value = StorageInt;
        vp8_MBsplitEncodings[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_SmallMVencodings") == 0)
    {
        cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        vp8_SmallMVencodings[x].value = StorageInt;
        vp8_SmallMVencodings[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MVrefEncodingArray") == 0)
    {
        cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        vp8_MVrefEncodingArray[x].value = StorageInt;
        vp8_MVrefEncodingArray[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_subMVrefEncodingArray") == 0)
    {
        cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        vp8_subMVrefEncodingArray[x].value = StorageInt;
        vp8_subMVrefEncodingArray[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_kfYmodeEncodings") == 0)
    {
        cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        vp8_kfYmodeEncodings[x].value = StorageInt;
        vp8_kfYmodeEncodings[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_UVmodeEncodings") == 0)
    {
        cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        vp8_UVmodeEncodings[x].value = StorageInt;
        vp8_UVmodeEncodings[x].Len = StorageInt2;
        cout << "\nArray Var Reset to:";
        cerr << "\nArray Var Reset to:";
        cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
    }


    cout << "\n";
    cerr << "\n";

    char bufferCorrect[1024];
    fstream infileCorrect;
    infileCorrect.open(CodeCoverageDecCorrect);
    infileCorrect.getline(bufferCorrect, 1024);
    infileCorrect.close();

    char bufferModified[1024];
    fstream infileModified;
    infileModified.open(CodeCoverageDecModified);
    infileModified.getline(bufferModified, 1024);
    infileModified.close();

    string bufferCorrectSTR = bufferCorrect;
    //snprintf(bufferCorrectSTR,1024,"%s",bufferCorrect);

    //if(CompIVF(CodeCoverageDecCorrect,CodeCoverageDecModified) == -1)
    if (bufferCorrectSTR.compare(bufferModified) == 0)
    {
        cout << "\nMD5 Checksums Identical for variable number: " << x << " - Fail\n";
        cerr << "\nMD5 Checksums Identical for variable number: " << x << " - Fail\n";

        char rootDir[255];
        FolderName(argv[0], rootDir);
        string CoverageSummaryTextFile = rootDir;
        CoverageSummaryTextFile.append("CoverageSummary.txt");

        char FullName[255];

        snprintf(FullName, 255, "%s", CoverageSummaryTextFile.c_str());

        ofstream outfile2(FullName, ios_base::app);

        if (argc == 8)
        {
            outfile2 << "\nMD5 Checksums Identical for: " << arrayname << "[" << x << "] - Fail";
        }

        if (argc == 9)
        {
            outfile2 << "\nMD5 Checksums Identical for: " << arrayname << "[" << x << "][" << y << "] - Fail";
        }

        if (argc == 10)
        {
            outfile2 << "\nMD5 Checksums Identical for: " << arrayname << "[" << x << "][" << y << "][" << z << "] - Fail";
        }

        outfile2.close();
    }
    else
    {
        cout << "\nFiles Not Identical - Pass\n";
        cerr << "\nFiles Not Identical - Pass\n";
    }

#else

    cout << "\nDecoder Array Coverage Tests Are Not Available In This Build.\n";

#endif


    return 0;
}
