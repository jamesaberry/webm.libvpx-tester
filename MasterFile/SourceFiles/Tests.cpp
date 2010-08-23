#define _CRT_SECURE_NO_WARNINGS
//------------------------------------------------------------------------------
//
//  Copyright (c) 1999-2005  On2 Technologies Inc.  All Rights Reserved.
//
//------------------------------------------------------------------------------
/////VP8//////////////////
#include "on2-vpx-shim.h"
#include "test-definitions.h"
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
extern double IVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, double *SsimOut);
extern double PostProcIVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double &SsimOut);
extern int PSNRSelect(char *inFile, char *outFile);
extern double IVFDataRate(char *inputFile, int DROuputSel);
extern int IVFCheckPBM(char *inputFile, int bitRate, int maxBuffer, int preBuffer);
extern int CompIVF(char *inputFile1, char *inputFile2);
extern double IVFDisplayKeyFrames(char *inputFile, int Selector);
extern int TimeReturn(char *infile, int FileType);
extern int DecompressIVFtoRaw(char *inputFile, char *outputFile2);
extern int DecompressIVFtoIVFErrorDetection(char *inputFile, char *outputFile2);
extern void VP8DefaultParms(VP8_CONFIG &opt);
extern int DecComputeMD5(char *inputchar, char *outputchar);
extern int CheckMinQ(char *inputFile, int MinQuantizer);
extern int CheckMaxQ(char *inputFile, int MaxQuantizer);
extern int CheckFixedQ(char *inputFile, int FixedQuantizer);
extern double IVFDisplayAltRefFrames(char *inputFile, int Selector);
extern double IVFDisplayVisibleFrames(char *inputFile, int Selector);
extern double IVFDisplayResizedFrames(char *inputchar, int PrintSwitch);
extern int SolveQuadradic(float X1, float X2, float X3, float Y1, float Y2, float Y3, float &A, float &B, float &C);
extern float AreaUnderQuadradic(float A, float B, float C, float X1, float X2);
extern int IVFLagInFramesCheck(char *QuantInChar);
extern int CompressIVFtoIVFForceKeyFrame(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, int forceKeyFrame);
extern int GetNumberofFrames(char *inputFile);
extern int IVFDFWMCheck(char *InputFile, int printselect);
extern int IVFCheckPBMThreshold(char *inputFile, double bitRate, int maxBuffer, int preBuffer, int optimalbuffer, int Threshold);
extern int CompressIVFtoIVFReconBufferCheck(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck);

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
extern int ReconBuffer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
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

    printf("%s", TestMachineInfo.c_str());
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

    cout << TestMachineInfo;
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

    cout << TestMachineInfo;
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

                if (selector == AlWDFNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowDropFrames";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AllowDF(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == ALWLGNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowLagTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AllowLagTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == ALWSRNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowSpatialResampling";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AllowSpatialResamplingTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == AUTKFNUM)
                {
                    SelectorAr[SelectorArInt] = "AutoKeyFramingWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = AutoKeyFramingWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == BUFLVNUM)
                {
                    SelectorAr[SelectorArInt] = "BufferLevelWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = BufferLevelWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == CPUDENUM)
                {
                    SelectorAr[SelectorArInt] = "CPUDecOnlyWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = CPUDecOnlyWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == CHGWRNUM)
                {
                    SelectorAr[SelectorArInt] = "ChangeCPUWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ChangeCPUWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == DFWMWNUM)
                {
                    SelectorAr[SelectorArInt] = "DropFramesWaterMarkWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = DFWM(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == DTARTNUM)
                {
                    SelectorAr[SelectorArInt] = "DataRateTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = DataRateTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == DBMRLNUM)
                {
                    SelectorAr[SelectorArInt] = "DebugMatchesRelease";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = DebugMatchesRelease(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == ENCBONUM)
                {
                    SelectorAr[SelectorArInt] = "EncoderBreakOut";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = EncoderBreakOut(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == ERRMWNUM)
                {
                    SelectorAr[SelectorArInt] = "ErrorResilientModeWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ErrorRes(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == EXTFINUM)
                {
                    SelectorAr[SelectorArInt] = "ExtraFileCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ExtraFileCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == FIXDQNUM)
                {
                    SelectorAr[SelectorArInt] = "FixedQ";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = FixedQ(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == FKEFRNUM)
                {
                    SelectorAr[SelectorArInt] = "ForceKeyFrameWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ForceKeyFrameWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == GQVBQNUM)
                {
                    SelectorAr[SelectorArInt] = "GoodQualityVsBestQuality";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = GoodQvBestQ(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == LGIFRNUM)
                {
                    SelectorAr[SelectorArInt] = "LagInFramesTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = LagInFramesTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == MAXQUNUM)
                {
                    SelectorAr[SelectorArInt] = "MaxQuantizerTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MaxQTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == MEML1NUM)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MemLeakCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == MEML2NUM)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck2";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MemLeakCheck2(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == MINQUNUM)
                {
                    SelectorAr[SelectorArInt] = "MinQuantizerTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MinQTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == MULTTNUM)
                {
                    SelectorAr[SelectorArInt] = "MultiThreadedTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = MultiThreadedTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == NVOPSNUM)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldPSNR";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = NewVsOldPSNR(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == NVORTNUM)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldRealTimeSpeed";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = NewVsOldRealTimeSpeed(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == NOISENUM)
                {
                    SelectorAr[SelectorArInt] = "NoiseSensitivityWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = NoiseSensitivityWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == OV2PSNUM)
                {
                    SelectorAr[SelectorArInt] = "OnePassVsTwoPass";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = OnePassVsTwoPass(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == PLYALNUM)
                {
                    SelectorAr[SelectorArInt] = "PlayAlternate";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = PlayAlternate(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == POSTPNUM)
                {
                    SelectorAr[SelectorArInt] = "PostProcessorWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = PostProcessorWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == RECONBUF)
                {
                    SelectorAr[SelectorArInt] = "ReconBuffer";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ReconBuffer(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == RSDWMNUM)
                {
                    SelectorAr[SelectorArInt] = "ResampleDownWaterMark";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = ResampleDownWaterMark(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == SPEEDNUM)
                {
                    SelectorAr[SelectorArInt] = "SpeedTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = SpeedTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == TVECTNUM)
                {
                    SelectorAr[SelectorArInt] = "TestVectorCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = TestVectorCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == TV2BTNUM)
                {
                    SelectorAr[SelectorArInt] = "TwoPassVsTwoPassBest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = TwoPassVsTwoPassBest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == UNDSHNUM)
                {
                    SelectorAr[SelectorArInt] = "UnderShoot";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = UnderShoot(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == VERSINUM)
                {
                    SelectorAr[SelectorArInt] = "Version";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];
                    PassFail[PassFailInt] = Version(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);
                    TestsRun++;
                }

                if (selector == WMLMMNUM)
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
//-------------------------------------------VP8 Tests----------------------------------------------------------------
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

        EncTimeArr[x] = TimeCompressIVFtoIVF(input, outputChar , speed, opt.TargetBandwidth, opt, CompressString, 0, 0);

        if (EncTimeArr[x] == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        DecTimeArr[x] = TimeDecompressIVFtoIVF(outputChar, outputChar2);

        double ssimnumber = 0;
        PSNRArr[x] = IVFPSNR(input, outputChar, 0, 2, 1, &ssimnumber);
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

        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1, 0) == -1)
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
        if (CompressIVFtoIVF(input, RandComp, speed, BitRate, opt, CompressString, 1, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    return 5;
}
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

    printf("Allow Drop Frames Test");
    fprintf(stderr, "Allow Drop Frames Test");

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
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.AllowDF = 0;

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowDF = 1;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.AllowDF = 0;

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AllowDF = 1;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowDF = 1;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowDF = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, AllowDFoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowDF = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowDFon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    int AllowDFonFrames = IVFDisplayVisibleFrames(AllowDFon, 1);
    int AllowDFoffFrames = IVFDisplayVisibleFrames(AllowDFoff, 1);

    /*printf("\n\n");
    fprintf(stderr, "\n\n");
    long File1bytes = FileSize(AllowDFon);
    printf("\n");
    fprintf(stderr, "\n");
    long File2bytes = FileSize(AllowDFoff);
    printf("\n");
    fprintf(stderr, "\n");*/

    /*char OutputChar1[255];
    snprintf(OutputChar1, 255, "",);
    string OutputChar1str = OutputChar1;
    FormatedPrint(OutputChar1str, 5);
    printf("\n");fprintf(stderr, "\n");*/

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    int fail = 0;

    if (AllowDFonFrames < AllowDFoffFrames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DF on frames = %i < DF off frames = %i - Passed", AllowDFonFrames, AllowDFoffFrames);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (AllowDFoffFrames == AllowDFonFrames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DF on frames = %i == DF off frames = %i No effect - Failed", AllowDFonFrames, AllowDFoffFrames);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (AllowDFonFrames > AllowDFoffFrames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DF on frames = %i > DF off frames = %i - Failed", AllowDFonFrames, AllowDFoffFrames);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
}
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

    printf("Allow Lag Test");
    fprintf(stderr, "Allow Lag Test");

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
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.AllowLag = 0;

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowLag = 1;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1, 1) == -1)
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

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AllowLag = 1;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1, 1) == -1)
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

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowLag = 1;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1, 1) == -1)
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

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1, 1) == -1)
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

            if (CompressIVFtoIVF(input, AllowLagoff, speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowLag = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowLagon, speed, BitRate, opt, CompressString, 1, 1) == -1)
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

    int CorrectLagFail = 0;
    ////////////Make Sure That Frames are lagged////////////
    string QuantInStr = AllowLagon;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_Quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    int LagInFramesFound = IVFLagInFramesCheck(QuantInChar);

    int AllowLagONAltRefCount = IVFDisplayAltRefFrames(AllowLagon, 1);
    int AllowLagOFFAltRefCount = IVFDisplayAltRefFrames(AllowLagoff, 1);
    int VisibleFrameONCount = IVFDisplayVisibleFrames(AllowLagon, 1);
    int VisibleFrameOFFCount = IVFDisplayVisibleFrames(AllowLagoff, 1);

    //cout << "AllowLagONAltRefCount: " << AllowLagONAltRefCount << "\n";
    //cout << "AllowLagOFFAltRefCount: " << AllowLagOFFAltRefCount << "\n";
    //cout << "VisibleFrameONCount: " << VisibleFrameONCount << "\n";
    //cout << "VisibleFrameOFFCount: " << VisibleFrameOFFCount << "\n";

    char AllowLagonFilename[255];
    FileName(AllowLagon, AllowLagonFilename);
    char AllowLagoffFilename[255];
    FileName(AllowLagoff, AllowLagoffFilename);

    int lngRC = CompIVF(AllowLagoff, AllowLagon);

    /*char OutputChar1[255];
    snprintf(OutputChar1, 255, "",);
    string OutputChar1str = OutputChar1;
    FormatedPrint(OutputChar1str, 5);
    printf("\n");fprintf(stderr, "\n");*/

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    int fail = 0;

    if (LagInFramesFound == opt.LagInFrames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Correct number of LagInFrames (%i) detected in compression for %s - Passed", opt.LagInFrames, AllowLagonFilename);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Correct number of LagInFrames (%i) not detected in compression for %s - Failed", opt.LagInFrames, AllowLagonFilename);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (AllowLagONAltRefCount > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Alternate reference frames exist for %s - Passed", AllowLagonFilename);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Alternate reference frames do not exist for %s - Failed", AllowLagonFilename);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (VisibleFrameONCount == VisibleFrameOFFCount)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Visible Frames for %s = %i == Visible Frames for %s = %i - Passed", AllowLagonFilename, VisibleFrameONCount, AllowLagoffFilename, VisibleFrameOFFCount);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Visible Frames for %s = %i != Visible Frames for %s = %i - Failed", AllowLagonFilename, VisibleFrameONCount, AllowLagoffFilename, VisibleFrameOFFCount);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (lngRC >= 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s is not identical to %s - Passed", AllowLagonFilename, AllowLagoffFilename);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (lngRC == -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s is identical to %s - Failed", AllowLagonFilename, AllowLagoffFilename);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
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

    printf("Allow Spatial Resampling Test");
    fprintf(stderr, "Allow Spatial Resampling Test");

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
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.AllowSpatialResampling = 0;

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.AllowSpatialResampling = 1;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowSpatialResampling = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, Spatialoff, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.AllowSpatialResampling = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Spatialon, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    double SpatialResampPSNR = IVFPSNR(input, Spatialon, 0, 0, 1, NULL);

    char SpatialonFileName[255];
    FileName(Spatialon, SpatialonFileName);
    char SpatialoffFileName[255];
    FileName(Spatialoff, SpatialoffFileName);

    printf("\nChecking: %s for resized frames\n", SpatialonFileName);
    fprintf(stderr, "\nChecking: %s for resized frames\n", SpatialonFileName);
    int AllowSpatResampleONFramesResized = IVFDisplayResizedFrames(Spatialon, 1);

    printf("Checking: %s for resized frames\n", SpatialoffFileName);
    fprintf(stderr, "Checking: %s for resized frames\n", SpatialoffFileName);
    int AllowSpatResampleOFFFramesResized = IVFDisplayResizedFrames(Spatialoff, 1);

    int fail = 0;
    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    if (AllowSpatResampleONFramesResized > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOn Frames Resized %i > 0 - Passed", AllowSpatResampleONFramesResized);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOn Frames Resized %i <= 0 - Failed", AllowSpatResampleONFramesResized);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (AllowSpatResampleOFFFramesResized == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOff Frames Resized %i == 0 - Passed", AllowSpatResampleOFFFramesResized);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOff Frames Resized %i != 0 - Failed", AllowSpatResampleOFFFramesResized);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (SpatialResampPSNR > 15.0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResample PSNR: %f > 15.00 - Passed", SpatialResampPSNR);;
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResample On PSNR: %f < 15.00 - Failed", SpatialResampPSNR);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    return 0;
}

int AutoKeyFramingWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "opt.AutoKeyFrame";

    if (!(argc == 7 || argc == 6))
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
            "    <Key Frame Frequency>\n"
            "	 <Optional Settings File>\n"
            "\n");
        return 0;
    }

    int speed = 0;
    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int AutoKeyFramingInt = atoi(argv[5]);

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
    WorkingDir4.append("AutoKeyFramingWorksOutput1_KeyFrames.txt");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("AutoKeyFramingWorksOutput1.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("AutoKeyFramingWorksOutput2_KeyFrames.txt");
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

    printf("Auto Key Framing Works Test");
    fprintf(stderr, "Auto Key Framing Works Test");

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
    opt.key_freq = AutoKeyFramingInt;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2, speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

    IVFDisplayKeyFrames(AutoKeyFramingWorks1, 1);
    IVFDisplayKeyFrames(AutoKeyFramingWorks2, 1);

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

    int fail = 0;
    int x;
    int y;

    while (!infile1.eof() && !infile2.eof())
    {
        infile1 >> x;
        infile2 >> y;

        if (x != y)
        {
            fail = 1;
        }

    }

    if (!infile1.eof())
    {
        fail = 1;
    }

    if (!infile2.eof())
    {
        fail = 1;
    }

    infile1.close();
    infile2.close();

    char AutoKeyFramingWorks1FileName[255];
    FileName(AutoKeyFramingWorks1, AutoKeyFramingWorks1FileName);
    char AutoKeyFramingWorks2FileName[255];
    FileName(AutoKeyFramingWorks2, AutoKeyFramingWorks2FileName);

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    //////////////////////////////////////////////////////////////////////////////////
    int fail2 = 0;
    ifstream infile(outputfile1);

    if (!infile.good())
    {
        printf("\nKey Frame File Not Present - Failed");
        fprintf(stderr, "\nKey Frame File Not Present - Failed");
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

        if (absInt(y2 - x2) > AutoKeyFramingInt)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Key Frames do not occur at least as frequently as Auto Key Frame dictates: %i No key frames between %i and %i - Failed", AutoKeyFramingInt, x2, y2);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail2 = 1;
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

    int NumberofFrames = GetNumberofFrames(AutoKeyFramingWorks1);

    if (NumberofFrames - 1 >= (maxKeyFrame + AutoKeyFramingInt))
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames do not occur at least as frequently as Auto Key Frame dictates: %i No key frames between %i and %i - Failed", AutoKeyFramingInt, maxKeyFrame, NumberofFrames - 1);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    ///////////////////////////////////////////////////////////////////////////
    if (fail2 == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames occur at least as frequently as Auto Key Frame dictates: %i - Passed", AutoKeyFramingInt);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (fail == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames occur at the same locations for %s and %s - Passed", AutoKeyFramingWorks1FileName, AutoKeyFramingWorks2FileName);
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
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames do not occur at the same locations for %s and %s - Failed", AutoKeyFramingWorks1FileName, AutoKeyFramingWorks2FileName);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");

        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }


}
int BufferLevelWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "AllowDF";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  BufferLevelWorks \n\n"
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
    char *MyDir = "BufferLevelWorks";
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
    WorkingDir4.append("BufferLevelWorksOutput.ivf");

    char BufferLevelWorksOut[255];
    snprintf(BufferLevelWorksOut, 255, "%s", WorkingDir4.c_str());

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

    printf("Buffer Level Works Test");
    fprintf(stderr, "Buffer Level Works Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;
    int StartingBufferLvl = 4;
    int MaximumBufferLevel = 6;

    //int constInt = 128;

    //int CalcBufferSize = BitRate * MaximumBufferLevel * constInt;
    //int CalcPrebuffer = BitRate * StartingBufferLvl * constInt;

    char CharCalcBufferSize[32];
    char CharCalcPrebuffer[32];
    char CharBitRate[32];

    itoa_custom(MaximumBufferLevel * 1000, CharCalcBufferSize, 10);
    itoa_custom(StartingBufferLvl * 100, CharCalcPrebuffer, 10);
    itoa_custom(BitRate, CharBitRate, 10);

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
    opt.AllowDF = 1;
    int CompressInt = opt.AllowDF;

    opt.TargetBandwidth = BitRate;
    opt.StartingBufferLevel = StartingBufferLvl;
    opt.MaximumBufferSize = MaximumBufferLevel;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }
    }

    if (TestType == 2) //Create Compression only stop test short.
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    int PassFail = IVFCheckPBM(BufferLevelWorksOut, BitRate, MaximumBufferLevel * 1000, StartingBufferLvl * 1000);

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    if (PassFail == -11)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "No buffer under run detected - Passed");
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
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Buffer under run at frame: %i - Failed", PassFail);
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
}
int CPUDecOnlyWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Version";

    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  CPUDecOnlyWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Version>\n "
            "	  <Optional Settings File>\n"
        );
        return 0;
    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "CPUDecOnlyWorks";
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
    WorkingDir4.append("CPUDecOnlyWorksOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("CPUDecOnlyWorksOutput_CPU");

    char CPUDecOnlyWorksOutFile[255];
    char CPUDecOnlyWorksOut_CPU[255];

    snprintf(CPUDecOnlyWorksOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(CPUDecOnlyWorksOut_CPU, 255, "%s", WorkingDir5.c_str());

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

    printf("CPU Dec Only Works Test");
    fprintf(stderr, "CPU Dec Only Works Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int VersionNum = atoi(argv[5]);

    int DoOnceSpeedRead = 0;
    unsigned int totalms = 0;
    unsigned int totalms2 = 0;

    int speed = 0;
    int Fail = 0;
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
    opt.Mode = MODE_GOODQUALITY;
    opt.Version = VersionNum;
    int CompressInt = opt.Version;

    int counter = 0;
    int Mode2 = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //putenv("ON2_SIMD_CAPS=0");

        string Output2 = CPUDecOnlyWorksOut_CPU;
        Output2.append("0.ivf");
        char Output2Char[1024];
        snprintf(Output2Char, 1024, "%s", Output2.c_str());

        int counterMax = 12;
        counter++;
        int i = 1;

        while (counter < counterMax)
        {

            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);
            //putenv((char *)CPUIDSTRING.c_str());

            //////////////////////////////////
            ///////Compresion and Time ///////

            string Output = CPUDecOnlyWorksOut_CPU;
            string Output3 = CPUDecOnlyWorksOut_CPU;
            char count[20];
            itoa_custom(counter, count, 10);

            Output.append(count);
            Output.append(".ivf");

            char ChangedCPUDecOutFileChar[255];
            snprintf(ChangedCPUDecOutFileChar, 255, "%s", Output.c_str());

            //putenv("ON2_SIMD_CAPS=0");

            printf("\n");
            fprintf(stderr, "\n");

            int countOld = (counter - 1);
            itoa_custom(countOld, count, 10);

            Output3.append(count);

            if (DoOnceSpeedRead == 0)
            {
                char GetTimeChar[255];
                snprintf(GetTimeChar, 255, "%s", Output.c_str());

                totalms = TimeReturn(GetTimeChar, 1);
                DoOnceSpeedRead = 1;
            }
            else
            {
                char GetTimeChar[255];
                snprintf(GetTimeChar, 255, "%s", Output.c_str());

                totalms2 = TimeReturn(GetTimeChar, 1);
            }

            Output3.append(".ivf");


            char Comp1[255];
            char Comp2[255];

            snprintf(Comp1, 255, "%s", Output.c_str());
            snprintf(Comp2, 255, "%s", Output3.c_str());

            cout << "\ncomparing " << Comp1 << " \n\nto\n\n " << Comp2 << "\n\n";
            fprintf(stderr, "\ncomparing CPU:%i to CPU:%i", counter - 1, counter);

            int lngRC = CompIVF(Comp1, Comp2);

            if (lngRC >= 0)
            {
                printf("\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                fprintf(stderr, "\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                Fail = 1;
            }

            if (lngRC == -1)
            {
                printf("\nFiles are identical");
                fprintf(stderr, "\nFiles are identical");
            }

            if (lngRC == -2)
            {
                printf("\n\nFail: File 2 ends before File 1.\n"/*, lngRC*/);
                fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n"/*, lngRC*/);
                Fail = 1;
            }

            if (lngRC == -3)
            {
                printf("\n\nFail: File 1 ends before File 2.\n"/*, lngRC*/);
                fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n"/*, lngRC*/);
                Fail = 1;
            }

            counter = counter + 1;
            printf("\n\n");
            i++;
            fprintf(stderr, "\n\n");
        }

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        putenv("ON2_SIMD_CAPS=0");

        string Output2 = CPUDecOnlyWorksOut_CPU;
        Output2.append("0.ivf");
        char Output2Char[1024];
        snprintf(Output2Char, 1024, "%s", Output2.c_str());

        printf("\n\n");
        fprintf(stderr, "\n\n");

        printf("CPU:%i\n", 0);
        fprintf(stderr, "CPU:%i\n", 0);

        totalms = DecompressIVFtoIVFTimeAndOutput(CPUDecOnlyWorksOutFile, Output2Char);

        printf("\n\n");
        fprintf(stderr, "\n\n");

        int counterMax = 12;
        counter++;
        int i = 1;

        while (counter < counterMax)
        {
            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);
            putenv((char *)CPUIDSTRING.c_str());

            printf("CPU:%i", counter);
            fprintf(stderr, "CPU:%i", counter);

            //////////////////////////////////
            ///////Compresion and Time ///////

            string Output = CPUDecOnlyWorksOut_CPU;
            string Output3 = CPUDecOnlyWorksOut_CPU;
            char count[20];
            itoa_custom(counter, count, 10);

            Output.append(count);
            Output.append(".ivf");

            char ChangedCPUDecOutFileChar[255];
            snprintf(ChangedCPUDecOutFileChar, 255, "%s", Output.c_str());

            printf("\n");
            fprintf(stderr, "\n");

            totalms2 = DecompressIVFtoIVFTimeAndOutput(CPUDecOnlyWorksOutFile, ChangedCPUDecOutFileChar);

            int countOld = (counter - 1);
            itoa_custom(countOld, count, 10);

            Output3.append(count);
            Output3.append(".ivf");

            if (TestType != 2)
            {
                char Comp1[255];
                char Comp2[255];

                snprintf(Comp1, 255, "%s", Output.c_str());
                snprintf(Comp2, 255, "%s", Output3.c_str());

                cout << "\ncomparing " << Comp1 << " \n\nto\n\n " << Comp2 << "\n\n";
                fprintf(stderr, "\ncomparing CPU:%i to CPU:%i", counter - 1, counter);

                int lngRC = CompIVF(Comp1, Comp2);

                if (lngRC >= 0)
                {
                    printf("\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                    fprintf(stderr, "\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                    Fail = 1;
                }

                if (lngRC == -1)
                {
                    printf("\nFiles are identical");
                    fprintf(stderr, "\nFiles are identical");
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
            }

            counter = counter + 1;
            printf("\n\n");
            i++;
            fprintf(stderr, "\n\n");
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        putenv("ON2_SIMD_CAPS=");
        return 10;
    }

    int overallfail = 0;
    printf("\nResults:\n\n");
    fprintf(stderr, "\nResults:\n\n");


    if (Fail == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files Identical - Passed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (Fail == 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files not Identical - Failed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        overallfail = 1;
    }

    if (totalms != totalms2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Decompress times are not equal - Passed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (totalms == totalms2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "CPU changes are not effecting the runtime - Failed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        overallfail = 1;
    }

    if (overallfail == 0)
    {
        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");
        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
}

int ChangeCPUWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "Version:";

    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  ChangeCPUWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Version>\n"
            "	  <Optional Settings File>\n"
        );
        return 0;
    }



    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    string WorkingDirString = "";
    string Mode3TestMatch = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "ChangeCPUWorks";
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
    WorkingDir4.append("ChangedCPUNOutputCPU0.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("ChangedCPUNOutputCPU");

    char ChangedCPUDec0OutFile[255];
    char ChangedCPUDecNOutFile[255];

    snprintf(ChangedCPUDec0OutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(ChangedCPUDecNOutFile, 255, "%s", WorkingDir5.c_str());

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

    printf("Change CPU Works Test");
    fprintf(stderr, "Change CPU Works Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int VersionNum = atoi(argv[5]);

    unsigned int totalms1 = 0;
    unsigned int totalms2 = 0;
    unsigned int totalms3 = 0;

    unsigned int totalms1b = 0;
    unsigned int totalms2b = 0;
    unsigned int totalms3b = 0;

    int speed = 0;
    int Fail = 0;

    unsigned int Time1;
    unsigned int Time2;

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
    opt.Version = VersionNum;
    int CompressInt = opt.Version;
    opt.Mode = MODE_GOODQUALITY;
    int counter = 0;

    putenv("ON2_SIMD_CAPS=0");

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        int counterMax = 16;
        counter++;
        int i = 1;
        Time1 = TimeReturn(ChangedCPUDec0OutFile, 0);

        while (counter < counterMax)
        {
            ///updating CPU

            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);
            putenv((char *)CPUIDSTRING.c_str());

            //////////////////////////////////
            ///////Compresion and Time ///////

            string Output = ChangedCPUDecNOutFile;
            string Output3 = ChangedCPUDecNOutFile;
            char count[20];
            itoa_custom(counter, count, 10);

            Output.append(count);
            Output.append(".ivf");

            char ChangedCPUDecNOutFileFinal[255];
            snprintf(ChangedCPUDecNOutFileFinal, 255, "%s", Output.c_str());

            int countOld = (counter - 1) / 2;
            itoa_custom(countOld, count, 10);

            Output3.append(count);
            Output3.append(".ivf");

            char Comp1[255];
            snprintf(Comp1, 255, "%s", Output.c_str());
            char Comp2[255];
            snprintf(Comp2, 255, "%s", Output3.c_str());

            printf("\n\ncomparing\n\n %s \n\n to \n\n%s\n\n", Comp1, Comp2);
            fprintf(stderr, "\n\ncomparing\n\n %s \n\n to \n\n%s\n\n", Comp1, Comp2);

            int lngRC = CompIVF(Comp1, Comp2);

            if (lngRC >= 0)
            {
                printf("\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                fprintf(stderr, "\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                Fail = 1;
            }

            if (lngRC == -1)
            {
                printf("\nFiles are identical");
                fprintf(stderr, "\nFiles are identical");
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

            Time2 = TimeReturn(Comp1, 0);

            counter = (counter * 2) + 1;
            i++;
        }
    }
    else
    {

        printf("\n\nCPU:%i", 0);
        fprintf(stderr, "\n\nCPU:%i", 0);

        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        int counterMax = 16;
        counter++;
        int i = 1;

        while (counter < counterMax)
        {
            ///updating CPU

            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);
            putenv((char *)CPUIDSTRING.c_str());
            printf("\n\nCPU:%i", counter);
            fprintf(stderr, "\n\nCPU:%i", counter);

            //////////////////////////////////
            ///////Compresion and Time ///////

            string Output = ChangedCPUDecNOutFile;
            string Output3 = ChangedCPUDecNOutFile;
            char count[20];
            itoa_custom(counter, count, 10);

            Output.append(count);
            Output.append(".ivf");

            char ChangedCPUDecNOutFileFinal[255];
            snprintf(ChangedCPUDecNOutFileFinal, 255, "%s", Output.c_str());

            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = Time2 + TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = Time2 + TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt, 0);

                if (Time2 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }


            //compavi
            int countOld = (counter - 1) / 2;
            itoa_custom(countOld, count, 10);

            Output3.append(count);
            Output3.append(".ivf");

            char Comp1[255];
            snprintf(Comp1, 255, "%s", Output.c_str());
            char Comp2[255];
            snprintf(Comp2, 255, "%s", Output3.c_str());

            if (TestType != 2)
            {
                printf("\n\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);
                fprintf(stderr, "\n\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);

                int lngRC = CompIVF(Comp1, Comp2);

                if (lngRC >= 0)
                {
                    printf("\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                    fprintf(stderr, "\n\nFail: Files differ at frame: %i on file number %i", lngRC, i);
                    Fail = 1;
                }

                if (lngRC == -1)
                {
                    printf("\nFiles are identical");
                    fprintf(stderr, "\nFiles are identical");
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
            }

            counter = (counter * 2) + 1;
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
        putenv("ON2_SIMD_CAPS=");
        return 10;
    }

    int overallfail = 0;
    printf("\n\n\nResults:\n\n");
    fprintf(stderr, "\n\n\nResults:\n\n");

    if (Fail != 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files are identical - Passed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (Fail == 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files are not identical - Failed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        overallfail = 1;
    }

    if (Time1 == Time2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Time1: %i == Time2: %i - Failed", Time1, Time2);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        overallfail = 1;
    }

    if (Time1 != Time2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Time 1: %i != Time 2: %i - Passed", Time1, Time2);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (overallfail == 0)
    {
        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
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


    printf("Drop Frame Watermark Works Test");
    fprintf(stderr, "Drop Frame Watermark Works Test");


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
    opt.end_usage = 0;
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

            printf("\n");
            fprintf(stderr, "\n");
            DMFW[i] = FileSize(DFWMOutFile);
            printf("\n");
            fprintf(stderr, "\n");

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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile, speed, BitRate, opt, CompressString, n, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (TestType != 2)
            {
                printf("\n");
                fprintf(stderr, "\n");
                DMFW[i] = FileSize(DFWMOutFile);
                printf("\n");
                fprintf(stderr, "\n");
            }

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

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    while (n < 5)
    {
        if (DMFW[n+1] == DMFW[n])
        {
            EqualBool++;

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %i = DFWM%4i: %i - Indeterminate", i - 20, DMFW[n+1], i, DMFW[n]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[n+1] > DMFW[n])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %i > DFWM%4i: %i - Passed", i - 20, DMFW[n+1], i, DMFW[n]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[n+1] < DMFW[n])
        {
            testBool = 0;

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %i < DFWM%4i: %i - Fail", i - 20, DMFW[n+1], i, DMFW[n]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        n++;
        i = i - 20;
    }

    if (testBool == 0)
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
    else
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
        else
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }
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

    printf("Data Rate Test");
    fprintf(stderr, "Data Rate Test");


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

    int TargetDataRatePercentage = 30;

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
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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


    printf("\n");
    fprintf(stderr, "\n");
    double FileDataRate = IVFDataRate(TargetBitRate1, 1);
    double DataRateProx = absDouble(100 - absDouble(((FileDataRate * 100) / BitRate)));

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    if (DataRateProx < TargetDataRatePercentage)
    {
        if (FileDataRate < BitRate)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% lower than Target, DataRate is within %i%% of: %4.2f - Passed", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% greater than Target, DataRate is within %i%% of: %4.2f - Passed", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {

        if (FileDataRate < BitRate)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% less than Target, DataRate not within %i%% of: %4.2f - Failed \n", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% greater than Target, DataRate not within %i%% of: %4.2f - Failed \n", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
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

    string DebugExeLoc = ExeCharDebugReleaseString;
    DebugExeLoc.append(ExeInputDebug);
    string ReleaseExeLoc = ExeCharDebugReleaseString;
    ReleaseExeLoc.append(ExeInputRelease);


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

    printf("Debug Matches Release Test");
    fprintf(stderr, "Debug Matches Release Test");

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

    FILE *ReleaseFileCheck = fopen(ReleaseExeLoc.c_str(), "rb");

    if (ReleaseFileCheck == NULL)
    {
        printf("\nRelease EXE %s not Found\n", ReleaseExeLoc.c_str());
        fprintf(stderr, "\nRelease EXE %s not Found\n", ReleaseExeLoc.c_str());
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

    fclose(ReleaseFileCheck);
    FILE *DebugFileCheck = fopen(DebugExeLoc.c_str(), "rb");

    if (DebugFileCheck == NULL)
    {
        printf("\nDebug EXE %s not Found\n", DebugExeLoc.c_str());
        fprintf(stderr, "\nDebug EXE %s not Found\n", DebugExeLoc.c_str());
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }

    fclose(ReleaseFileCheck);

    /////////////////////////////////////////////////////////

    opt.TargetBandwidth = BitRate;

    //cout << "\n\n" << ProgramDebug << "\n\n";
    //cout << "\n\n" << ParFileRelease << "\n\n";

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
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

    int fail = 0;

    if (lngRC >= 0)
    {
        printf("Files differ at frame: %i\n", lngRC);
        fprintf(stderr, "Files differ at frame: %i\n", lngRC);

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression not identical to Release Compression - Failed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (lngRC == -1)
    {
        printf("Files are identical\n");
        fprintf(stderr, "Files are identical\n");

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression identical to Release Compression - Passed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (lngRC == -2)
    {
        printf("File 2 ends before File 1\n");
        fprintf(stderr, "File 2 ends before File 1\n");

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression not identical to Release Compression - Failed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (lngRC == -3)
    {
        printf("File 1 ends before File 2\n");
        fprintf(stderr, "File 1 ends before File 2\n");

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression not identical to Release Compression - Failed");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
}
int EncoderBreakOut(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "EncoderBreakOut";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  EncoderBreakOut \n\n"
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


    printf("Encoder Break Out Test");
    fprintf(stderr, "Encoder Break Out Test");

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
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.EncodeBreakout = 0;

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.EncodeBreakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.EncodeBreakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.EncodeBreakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000, 0) == -1)
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

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.EncodeBreakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.EncodeBreakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.EncodeBreakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000, 0) == -1)
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

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.EncodeBreakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.EncodeBreakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.EncodeBreakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000, 0) == -1)
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

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 100;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 500;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 1000;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000, 0) == -1)
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

            if (CompressIVFtoIVF(input, EncBreakOut0, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 100;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut100, speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 500;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut500, speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.EncodeBreakout = 1000;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut1000, speed, BitRate, opt, CompressString, 1000, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
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
    }

    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    long SourceFileSize = 0;

    long EncBreakOut0_Dec_FileSize = 0;
    long EncBreakOut100_Dec_FileSize = 0;
    long EncBreakOut500_Dec_FileSize = 0;
    long EncBreakOut1000_Dec_FileSize = 0;

    printf("\n\n");
    fprintf(stderr, "\n\n");
    SourceFileSize = FileSize(input);
    printf("\n");
    fprintf(stderr, "\n");
    EncBreakOut0_Dec_FileSize = FileSize(EncBreakOut0_Dec);
    printf("\n");
    fprintf(stderr, "\n");
    EncBreakOut100_Dec_FileSize = FileSize(EncBreakOut100_Dec);
    printf("\n");
    fprintf(stderr, "\n");
    EncBreakOut500_Dec_FileSize = FileSize(EncBreakOut500_Dec);
    printf("\n");
    fprintf(stderr, "\n");
    EncBreakOut1000_Dec_FileSize = FileSize(EncBreakOut1000_Dec);
    printf("\n");
    fprintf(stderr, "\n");

    double PSNR0;
    double PSNR100;
    double PSNR500;
    double PSNR1000;

    PSNR0 = IVFPSNR(input, EncBreakOut0, 0, 0, 1, NULL);
    PSNR100 = IVFPSNR(input, EncBreakOut100, 0, 0, 1, NULL);
    PSNR500 = IVFPSNR(input, EncBreakOut500, 0, 0, 1, NULL);
    PSNR1000 = IVFPSNR(input, EncBreakOut1000, 0, 0, 1, NULL);

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
    cout << "dB3: " << dB3 << "\n";

    cerr << "\ndB1: " << dB1 << "\n";
    cerr << "dB2: " << dB2 << "\n";
    cerr << "dB3: " << dB3 << "\n";

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    int Pass = 1;
    int IndCount = 0;

    if (SourceFileSize != EncBreakOut0_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut0 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut0 was properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (SourceFileSize != EncBreakOut100_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut100 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut100 was properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (SourceFileSize != EncBreakOut500_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut500 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut500 was properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (SourceFileSize != EncBreakOut1000_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut1000 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut1000 was properly decoded.");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (dB1 <= 2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 0 PSNR and 100 PSNR are within 2 dB: %4f dB - Passed", dB1);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (dB1 >= 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 0 PSNR and 100 PSNR not within 5 dB: %4f dB - Failed", dB1);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        Pass = 0;
    }

    if (dB1 > 2 && dB1 < 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 0 PSNR and 100 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB1);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        IndCount++;
    }

    if (dB2 <= 2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 100 PSNR and 500 PSNR are within 2 dB: %4f dB - Passed", dB2);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (dB2 >= 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 100 PSNR and 500 PSNR not within 5 dB: %4f dB - Failed", dB2);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        Pass = 0;
    }

    if (dB2 > 2 && dB2 < 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 100 PSNR and 500 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB2);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        IndCount++;
    }

    if (dB3 <= 2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 500 PSNR and 1000 PSNR are within 2 dB: %4f dB - Passed", dB3);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }

    if (dB3 >= 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 500 PSNR and 1000 PSNR not within 5 dB: %4f dB - Failed", dB3);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        Pass = 0;
    }

    if (dB3 > 2 && dB3 < 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 500 PSNR and 1000 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB3);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        IndCount++;
    }

    if (Pass == 1)
    {
        if (IndCount == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nIndeterminate\n");
            fprintf(stderr, "\nIndeterminate\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }
    }
    else
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }
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
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.ErrorResilientMode = 1;

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.ErrorResilientMode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.ErrorResilientMode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.ErrorResilientMode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.ErrorResilientMode = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

            if (CompressIVFtoIVF(input, ErrorOnOutFile, speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.ErrorResilientMode = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ErrorOffOutFile, speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    PSNRon = IVFPSNR(input, ErrorOnOutFile, PSNRToggle, 0, 1, NULL);
    PSNRoff = IVFPSNR(input, ErrorOffOutFile, PSNRToggle, 0, 1, NULL);

    float PSRNPerc = 100 * absFloat((PSNRon - PSNRoff) / PSNRoff);

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    if (PSRNPerc < 10.00)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "ErrorRes on PSNR is within 10%% of Error Res off PSNR: %.2f%% - Passed", PSRNPerc);
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
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "ErrorRes on PSNR is not within 10%% of Error Res off PSNR: %.2f%% - Failed", PSRNPerc);
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
}



int ExtraFileCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
#if defined(linux)

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

        if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 0, 0/*opt.DeleteFirstPassFile*/) == -1)
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

        if (TestType == 2)
        {
            cout << "\nExtra files found\n";
            cerr << "\nExtra files found\n";

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 1;
            outfile.close();

            return 10;
        }

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Extra Files Found - Failed");
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

    if (Fail == 0)
    {
        printf("-------------------------------------------------------------------------------\n");
        fprintf(stderr, "-------------------------------------------------------------------------------\n");

        if (TestType == 2)
        {
            cout << "\nNo extra files found\n";
            cerr << "\nNo extra files found\n";

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 0;
            outfile.close();

            return 10;
        }

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "No extra files found - Passed");
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

#endif
#if defined(__APPLE__)

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

        if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 0, 0) == -1)
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

        if (TestType == 2)
        {
            cout << "\nExtra files found\n";
            cerr << "\nExtra files found\n";

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 1;
            outfile.close();

            return 10;
        }

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Extra Files Found - Failed");
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

    if (Fail == 0)
    {
        printf("-------------------------------------------------------------------------------\n");
        fprintf(stderr, "-------------------------------------------------------------------------------\n");

        if (TestType == 2)
        {
            cout << "\nNo extra files found\n";
            cerr << "\nNo extra files found\n";

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 0;
            outfile.close();

            return 10;
        }

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "No extra files found - Passed");
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

            if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 0, 0, 0/*opt.DeleteFirstPassFile*/) == -1)
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

            if (TestType == 2)
            {
                cout << "\nExtra files found\n";
                cerr << "\nExtra files found\n";

                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 1;
                outfile.close();

                return 10;
            }

            printf("\n\nResults:\n\n");
            fprintf(stderr, "\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Extra Files Found - Failed");
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

        if (Fail == 0)
        {
            printf("-------------------------------------------------------------------------------\n");
            fprintf(stderr, "-------------------------------------------------------------------------------\n");

            if (TestType == 2)
            {
                cout << "\nNo extra files found\n";
                cerr << "\nNo extra files found\n";

                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 0;
                outfile.close();

                return 10;
            }

            printf("\n\nResults:\n\n");
            fprintf(stderr, "\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "No extra files found - Passed");
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

            if (CompressIVFtoIVF(input, ExtraFileCheck, opt.MultiThreaded, BitRate, opt, CompressString, 1, 0/*opt.DeleteFirstPassFile*/) == -1)
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

            if (TestType == 2)
            {
                cout << "\nExtra files found\n";
                cerr << "\nExtra files found\n";

                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 1;
                outfile.close();

                return 10;
            }

            printf("\n\nResults:\n\n");
            fprintf(stderr, "\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Extra Files Found - Failed");
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

        if (Fail == 0)
        {
            printf("-------------------------------------------------------------------------------\n");
            fprintf(stderr, "-------------------------------------------------------------------------------\n");

            if (TestType == 2)
            {
                cout << "\nNo extra files found\n";
                cerr << "\nNo extra files found\n";

                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 0;
                outfile.close();

                return 10;
            }

            printf("\n\nResults:\n\n");
            fprintf(stderr, "\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "No extra files found - Passed");
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

#endif

        return 0;

    }
    int FixedQ(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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
        fprintf(stderr, "Fixed Q Test");

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
        int FixedQCheck1 = FixedQ1Int;
        int FixedQCheck2 = FixedQ2Int;

        if (FixedQ1Int < 0 || FixedQ1Int > 63 || FixedQ2Int < 0 || FixedQ2Int > 63)
        {
            printf("\n\nInvaild Qunatizer Range, Vaild range = 0-63 - Indeterminate\n");
            fprintf(stderr, "\n\nInvaild Qunatizer Range, Vaild range = 0-63 - Indeterminate\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }

        int n = 0;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.FixedQ = FixedQ1Int;

                if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.FixedQ = FixedQ2Int;

                if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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

                if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.FixedQ = FixedQ2Int;

                if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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

                if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.FixedQ = FixedQ2Int;

                if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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

                if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.FixedQ = FixedQ2Int;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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

                if (CompressIVFtoIVF(input, FixedQ1, speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.FixedQ = FixedQ2Int;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, FixedQ2, speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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

        int fail = 0;

        printf("\n");
        fprintf(stderr, "\n");
        int FixedQCheckVal1 = CheckFixedQ(FixedQ1, FixedQ1Int);
        printf("\n");
        fprintf(stderr, "\n");
        int FixedQCheckVal2 = CheckFixedQ(FixedQ2, FixedQ2Int);
        printf("\n");
        fprintf(stderr, "\n");

        printf("\n");
        fprintf(stderr, "\n");
        int FixedQSize1 = FileSize(FixedQ1);
        printf("\n");
        fprintf(stderr, "\n");
        int FixedQSize2 = FileSize(FixedQ2);
        printf("\n");
        fprintf(stderr, "\n");

        char FixedQ1FileName[255] = "";
        char FixedQ2FileName[255] = "";

        FileName(FixedQ1, FixedQ1FileName);
        FileName(FixedQ2, FixedQ2FileName);

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (FixedQCheckVal1 != -1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Quantizer not fixed for %s for frame %i - Failed", FixedQ1FileName, FixedQCheckVal1);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Quantizer fixed at %i for all frames for %s - Passed", FixedQ1Int, FixedQ1FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (FixedQCheckVal2 != -1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Quantizer not fixed for %s for frame %i - Failed", FixedQ2FileName, FixedQCheckVal2);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Quantizer fixed at %i for all frames for %s - Passed", FixedQ2Int, FixedQ2FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (FixedQ1Int < FixedQ2Int) //make sure that lower fixed q has higher datarate
        {

            if (FixedQSize1 <= FixedQSize2)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "%s file size: %i >= %s file size: %i - Failed", FixedQ2FileName, FixedQSize2, FixedQ1FileName, FixedQSize1);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "%s file size: %i > %s file size: %i - Passed", FixedQ1FileName, FixedQSize1, FixedQ2FileName, FixedQSize2);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }
        }
        else
        {
            if (FixedQSize2 <= FixedQSize1)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "%s file size: %i >= %s file size: %i - Failed", FixedQ1FileName, FixedQSize1, FixedQ2FileName, FixedQSize2);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "%s file size: %i > %s file size: %i - Failed", FixedQ2FileName, FixedQSize2, FixedQ1FileName, FixedQSize1);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }
        }

        if (fail == 1)
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
        else
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }
    int ForceKeyFrameWorks(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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
        WorkingDir4.append("ForceKeyFrameOutput_KeyFrames.txt");
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

        printf("Force Key Frame Works Test");
        fprintf(stderr, "Force Key Frame Works Test");

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
        opt.AutoKey = 0;//1;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.KeyFreq = 0;//ForceKeyFrameInt;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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
                opt.KeyFreq = 0;//ForceKeyFrameInt;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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
                opt.KeyFreq = 0;//ForceKeyFrameInt;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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
                opt.KeyFreq = 0;//ForceKeyFrameInt;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.KeyFreq = 0;//ForceKeyFrameInt;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame, speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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

        IVFDisplayKeyFrames(ForceKeyFrame, 1);
        //////////////////////////////////////////////////////////////////////////////////
        ifstream infile(outputfile);

        if (!infile.good())
        {
            printf("\nKey Frame File Not Present\n");
            fprintf(stderr, "\nKey Frame File Not Present\n");
            fclose(fp);
            return 0;
        }

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

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

            if (absInt(y2 - x2) != ForceKeyFrameInt)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Key Frames do not occur only when Force Key Frame dictates: %i - Failed", ForceKeyFrameInt);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
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

        int NumberofFrames = GetNumberofFrames(ForceKeyFrame);

        if (NumberofFrames - 1 >= (maxKeyFrame + ForceKeyFrameInt))
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Key Frames do not occur only when Force Key Frame dictates: %i - Failed", ForceKeyFrameInt);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        ///////////////////////////////////////////////////////////////////////////

        if (fail == 1)
        {
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
            snprintf(OutputChar1, 255, "Key Frames occur only when Force Key Frame dictates: %i - Passed", ForceKeyFrameInt);
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


    }
    int GoodQvBestQ(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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
        string WorkingDir4b = WorkingDirString;
        string WorkingDir5b = WorkingDirString;
        string WorkingDir4c = WorkingDirString;
        string WorkingDir5c = WorkingDirString;

        WorkingDir4.append(slashCharStr);
        WorkingDir4.append("GoodVsBestGood1.ivf");
        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("GoodVsBestBest1.ivf");
        WorkingDir4b.append(slashCharStr);
        WorkingDir4b.append("GoodVsBestGood2.ivf");
        WorkingDir5b.append(slashCharStr);
        WorkingDir5b.append("GoodVsBestBest2.ivf");
        WorkingDir4c.append(slashCharStr);
        WorkingDir4c.append("GoodVsBestGood3.ivf");
        WorkingDir5c.append(slashCharStr);
        WorkingDir5c.append("GoodVsBestBest3.ivf");

        char GvBgOutFile1[255];
        char GvBgOutFile2[255];
        char GvBgOutFile3[255];
        char GvBbOutFile1[255];
        char GvBbOutFile2[255];
        char GvBbOutFile3[255];

        snprintf(GvBgOutFile1, 255, "%s", WorkingDir4.c_str());
        snprintf(GvBbOutFile1, 255, "%s", WorkingDir5.c_str());
        snprintf(GvBgOutFile2, 255, "%s", WorkingDir4b.c_str());
        snprintf(GvBbOutFile2, 255, "%s", WorkingDir5b.c_str());
        snprintf(GvBgOutFile3, 255, "%s", WorkingDir4c.c_str());
        snprintf(GvBbOutFile3, 255, "%s", WorkingDir5c.c_str());

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

        printf("Good Quality vs Best Quality Test");
        fprintf(stderr, "Good Quality vs Best Quality Test");

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

        int BitRate1 = BitRate - (BitRate * 0.3);
        int BitRate2 = BitRate;
        int BitRate3 = BitRate + (BitRate * 0.3);

        //opt.TargetBandwidth = BitRate;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            opt.TargetBandwidth = BitRate1;
            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, GvBgOutFile1, speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate2;

            if (CompressIVFtoIVF(input, GvBgOutFile2, speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate3;

            if (CompressIVFtoIVF(input, GvBgOutFile3, speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate1;
            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, GvBbOutFile1, speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate2;

            if (CompressIVFtoIVF(input, GvBbOutFile2, speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate3;

            if (CompressIVFtoIVF(input, GvBbOutFile3, speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

        float GoodSize1 = IVFDataRate(GvBgOutFile1, 1);
        float BestSize1 = IVFDataRate(GvBbOutFile1, 1);
        float GoodSize2 = IVFDataRate(GvBgOutFile2, 1);
        float BestSize2 = IVFDataRate(GvBbOutFile2, 1);
        float GoodSize3 = IVFDataRate(GvBgOutFile3, 1);
        float BestSize3 = IVFDataRate(GvBbOutFile3, 1);

        //int PSNRToggle = PSNRSelect(input, GvBgOutFile);

        double PSNRG1;
        double PSNRB1;
        double PSNRG2;
        double PSNRB2;
        double PSNRG3;
        double PSNRB3;

        PSNRG1 = IVFPSNR(input, GvBgOutFile1, 1, 0, 1, NULL);
        PSNRB1 = IVFPSNR(input, GvBbOutFile1, 1, 0, 1, NULL);
        PSNRG2 = IVFPSNR(input, GvBgOutFile2, 1, 0, 1, NULL);
        PSNRB2 = IVFPSNR(input, GvBbOutFile2, 1, 0, 1, NULL);
        PSNRG3 = IVFPSNR(input, GvBgOutFile3, 1, 0, 1, NULL);
        PSNRB3 = IVFPSNR(input, GvBbOutFile3, 1, 0, 1, NULL);

        float GoodA = 0;
        float GoodB = 0;
        float GoodC = 0;

        float BestA = 0;
        float BestB = 0;
        float BestC = 0;

        float minCommon = 0;
        float maxCommon = 0;

        if (GoodSize1 > BestSize1) //take area over same range we have decent data for.
        {
            minCommon = GoodSize1;
        }
        else
        {
            minCommon = BestSize1;
        }

        if (GoodSize3 > BestSize3)
        {
            maxCommon = BestSize3;
        }
        else
        {
            maxCommon = GoodSize3;
        }

        SolveQuadradic(GoodSize1, GoodSize2, GoodSize3, PSNRG1, PSNRG2, PSNRG3, GoodA, GoodB, GoodC);
        float GoodAreaVal = AreaUnderQuadradic(GoodA, GoodB, GoodC, minCommon, maxCommon);

        //cout << "\n\nGoodAreaVal: " << GoodAreaVal << "\n\n";

        SolveQuadradic(BestSize1, BestSize2, BestSize3, PSNRB1, PSNRB2, PSNRB3, BestA, BestB, BestC);
        float BestAreaVal = AreaUnderQuadradic(BestA, BestB, BestC, minCommon, maxCommon);

        //cout << "\n\nBestAreaVal: " << BestAreaVal << "\n\n";

        /*cout << GoodA << "\n";
        cout << GoodB << "\n";
        cout << GoodC << "\n";

        cout << BestA << "\n";
        cout << BestB << "\n";;
        cout << BestC << "\n";*/

        //float PSRNPerc = absFloat((PSNRB1 - PSNRG1) / PSNRG1) * 100.00;
        //float BRPerc = (absFloat(BestSize1 - GoodSize1) / GoodSize1) * 100.00;

        printf("\n\n"
               "Data Points:\n"
               "\n"
               " Good Q\n"
               "\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "\n"
               " Best Q\n"
               "\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "\n"
               "\n"
               , GoodSize1, PSNRG1
               , GoodSize2, PSNRG2
               , GoodSize3, PSNRG3
               , BestSize1, PSNRB1
               , BestSize2, PSNRB2
               , BestSize3, PSNRB3
              );

        fprintf(stderr, "\n\n"
                "Data Points:\n"
                "\n"
                " Good Q\n"
                "\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "\n"
                " Best Q\n"
                "\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "\n"
                "\n"
                , GoodSize1, PSNRG1
                , GoodSize2, PSNRG2
                , GoodSize3, PSNRG3
                , BestSize1, PSNRB1
                , BestSize2, PSNRB2
                , BestSize3, PSNRB3
               );

        printf("Good Quality Curve: y = %fx^2 + %fx + %f\n", GoodA, GoodB, GoodC);
        printf("Best Quality Curve: y = %fx^2 + %fx + %f\n", BestA, BestB, BestC);

        fprintf(stderr, "Good Quality Curve: y = %fx^2 + %fx + %f\n", GoodA, GoodB, GoodC);
        fprintf(stderr, "Best Quality Curve: y = %fx^2 + %fx + %f\n", BestA, BestB, BestC);

        printf("\nGood Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, GoodAreaVal);
        printf("Best Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, BestAreaVal);

        fprintf(stderr, "\nGood Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, GoodAreaVal);
        fprintf(stderr, "Best Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, BestAreaVal);

        int Pass = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (GoodAreaVal == BestAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Best Quality area under curve: %.2f == Good Quality area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (BestAreaVal > GoodAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Best Quality area under curve: %.2f > Good Quality area under curve: %.2f - Passed", BestAreaVal, GoodAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            Pass = 1;
        }

        if (BestAreaVal < GoodAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Best Quality area under curve: %.2f < Good Quality area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (Pass == 1)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int LagInFramesTest(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

        printf("Lag In Frames Test");
        fprintf(stderr, "Lag In Frames Test");

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
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.AllowLag = 0;
                opt.LagInFrames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames2Val;

                if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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

                if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }


                opt.Mode = MODE_GOODQUALITY;
                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames2Val;

                if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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

                if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames2Val;

                if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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

                if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames1Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames2Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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

                if (CompressIVFtoIVF(input, LagInFrames0, speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames1Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, LagInFrames1, speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.AllowLag = 1;
                opt.LagInFrames = LagInFrames2Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, LagInFrames2, speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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

        double LagInFrames0PSNR = IVFPSNR(input, LagInFrames0, 0, 0, 1, NULL);
        double LagInFrames1PSNR = IVFPSNR(input, LagInFrames1, 0, 0, 1, NULL);
        double LagInFrames2PSNR = IVFPSNR(input, LagInFrames2, 0, 0, 1, NULL);

        double TenPer0 = LagInFrames0PSNR / 10;
        double TenPer1 = LagInFrames1PSNR / 10;
        double TenPer2 = LagInFrames2PSNR / 10;

        int lngRC1 = CompIVF(LagInFrames0, LagInFrames1);
        int lngRC2 = CompIVF(LagInFrames1, LagInFrames2);

        string QuantInStr0 = LagInFrames0;
        QuantInStr0.erase(QuantInStr0.length() - 4, 4);
        QuantInStr0.append("_Quantizers.txt");
        char QuantInChar0[255] = "";
        snprintf(QuantInChar0, 255, "%s", QuantInStr0.c_str());
        int LagInFramesFound0 = IVFLagInFramesCheck(QuantInChar0);

        string QuantInStr1 = LagInFrames1;
        QuantInStr1.erase(QuantInStr1.length() - 4, 4);
        QuantInStr1.append("_Quantizers.txt");
        char QuantInChar1[255] = "";
        snprintf(QuantInChar1, 255, "%s", QuantInStr1.c_str());
        int LagInFramesFound1 = IVFLagInFramesCheck(QuantInChar1);

        string QuantInStr2 = LagInFrames2;
        QuantInStr2.erase(QuantInStr2.length() - 4, 4);
        QuantInStr2.append("_Quantizers.txt");
        char QuantInChar2[255] = "";
        snprintf(QuantInChar2, 255, "%s", QuantInStr2.c_str());
        int LagInFramesFound2 = IVFLagInFramesCheck(QuantInChar2);

        int PSNRTally = 0;
        int fail = 0;

        char LagInFrames0FileName[255] = "";
        char LagInFrames1FileName[255] = "";
        char LagInFrames2FileName[255] = "";

        FileName(LagInFrames0, LagInFrames0FileName);
        FileName(LagInFrames1, LagInFrames1FileName);
        FileName(LagInFrames2, LagInFrames2FileName);

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (LagInFramesFound0 == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s properly lagged %i frames - Passed", LagInFrames0FileName, LagInFramesFound0);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s improperly lagged frames %i - Failed", LagInFrames0FileName, LagInFramesFound0);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (LagInFramesFound1 == LagInFrames1Val)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s properly lagged %i frames - Passed", LagInFrames1FileName, LagInFramesFound1);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s improperly lagged %i frames - Failed", LagInFrames1FileName, LagInFramesFound1);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (LagInFramesFound2 == LagInFrames2Val)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s properly lagged %i frames - Passed", LagInFrames2FileName, LagInFramesFound2);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s improperly lagged %i frames - Failed", LagInFrames2FileName, LagInFramesFound2);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (lngRC1 == -1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s identical to %s - Failed", LagInFrames0FileName, LagInFrames1FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s not identical to %s - Passed", LagInFrames0FileName, LagInFrames1FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (lngRC2 == -1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s identical to %s - Failed", LagInFrames1FileName, LagInFrames2FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s not identical to %s - Passed", LagInFrames1FileName, LagInFrames2FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (LagInFrames1PSNR <= (LagInFrames0PSNR + TenPer0) && LagInFrames1PSNR >= (LagInFrames0PSNR - TenPer0))
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "PSNR for %s is within 10%% of PSNR for %s - %.2f < %.2f < %.2f - Passed", LagInFrames0FileName, LagInFrames1FileName, (LagInFrames0PSNR - TenPer0), LagInFrames1PSNR, (LagInFrames0PSNR + TenPer0));
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            if (!(LagInFrames1PSNR <= (LagInFrames0PSNR + TenPer0)))
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames0FileName, LagInFrames1FileName, (LagInFrames0PSNR + TenPer0), LagInFrames1PSNR);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames0FileName, LagInFrames1FileName, LagInFrames1PSNR, (LagInFrames0PSNR - TenPer0));
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
        }

        if (LagInFrames2PSNR <= (LagInFrames1PSNR + TenPer1) && LagInFrames2PSNR >= (LagInFrames1PSNR - TenPer1))
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "PSNR for %s is within 10%% of PSNR for %s - %.2f < %.2f < %.2f - Passed", LagInFrames1FileName, LagInFrames2FileName, (LagInFrames1PSNR - TenPer1), LagInFrames2PSNR, (LagInFrames1PSNR + TenPer1));
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            if (!(LagInFrames1PSNR <= (LagInFrames0PSNR + TenPer0)))
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames1FileName, LagInFrames2FileName, (LagInFrames1PSNR + TenPer1), LagInFrames2PSNR);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames1FileName, LagInFrames2FileName, LagInFrames2PSNR, (LagInFrames1PSNR - TenPer1));
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
        }

        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int MaxQTest(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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


        printf("Max Quantizer Test");
        fprintf(stderr, "Max Quantizer Test");

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
        int MaxQArr[10];
        int i = 0;

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
                    PSNRArr[i] = IVFPSNR(input, QuantOutFile, PSNRToggle, 0, 1, NULL);
                    printf("\n");
                    fprintf(stderr, "\n");
                    MaxQArr[i] = CheckMaxQ(QuantOutFile, n);
                    printf("\n");
                    fprintf(stderr, "\n");
                    //PSNRArr[i] = IVFPSNR(input, QuantOutFile, PSNRToggle, 0, 1, NULL);
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

                    if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile, speed, BitRate, opt, CompressString, n, 1) == -1)
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
                    PSNRArr[i] = IVFPSNR(input, QuantOutFile, PSNRToggle, 0, 1, NULL);
                    printf("\n");
                    fprintf(stderr, "\n");
                    MaxQArr[i] = CheckMaxQ(QuantOutFile, n);
                    printf("\n");
                    fprintf(stderr, "\n");
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

        printf("\n");
        fprintf(stderr, "\n");

        i = 0;
        int MaxQDisplayValue = 3;
        int fail = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        while (i < 7)
        {
            if (!(PSNRArr[i+1] <= PSNRArr[i]))
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "MaxQ %2i %.2f > %.2f MaxQ %2i - Failed", MaxQDisplayValue + 8, PSNRArr[i+1], PSNRArr[i], MaxQDisplayValue);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "MaxQ %2i %.2f <= %.2f MaxQ %2i - Passed", MaxQDisplayValue + 8, PSNRArr[i+1], PSNRArr[i], MaxQDisplayValue);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }

            i++;
            MaxQDisplayValue = MaxQDisplayValue + 8;
        }

        i = 0;
        MaxQDisplayValue = 3;

        while (i < 8)
        {
            if (MaxQArr[i] != -1)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "MaxQ value exceded for MaxQ %2i - frame %i - Failed", MaxQDisplayValue, MaxQArr[i]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "MaxQ value not exceded for MaxQ %2i - Passed", MaxQDisplayValue, MaxQArr[i]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }

            MaxQDisplayValue = MaxQDisplayValue + 8;
            i++;
        }

        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int MemLeakCheck(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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


        printf("Mem Leak Check Test");
        fprintf(stderr, "Mem Leak Check Test");

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
            //This test requires no preperation before a Test Only Run
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

        int fail = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        ifstream infile4(MemLeakCheckTXT);

        if (!infile4.good())
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "MemLeakCheckOutput.txt not found - Failed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

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
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Memory Currently Allocated == 0 - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Memory Currently Allocated != 0 - %s - Failed", bufferString4.c_str());
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }



        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;

    }
    int MemLeakCheck2(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

        //cout << "\n\n\n" << ProgramMemLeakCheckEnc << "\n\n\n";
        //cout << "\n\n\n" << ProgramMemLeakCheckDec << "\n\n\n";

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


        printf("Mem Leak Check 2 Test");
        fprintf(stderr, "Mem Leak Check 2 Test");

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
            //This test requires no preperation before a Test Only Run
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

        int fail = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        FILE *infile4 = fopen(MemLeakCheckTXT1, "rb");

        if (infile4 == NULL)
        {
            char OutputChar1[255];
            char MemLeakCheckTXTFileName[200];
            FileName(MemLeakCheckTXT1, MemLeakCheckTXTFileName);
            snprintf(OutputChar1, 255, "File not found: %s - Failed", MemLeakCheckTXTFileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char buffer4[256];

            fgets(buffer4 , 256 , infile4);
            fgets(buffer4 , 256 , infile4);
            fgets(buffer4 , 256 , infile4);
            fgets(buffer4 , 256 , infile4);

            string bufferString4 = buffer4;

            if (bufferString4.compare(0, 24, "_currently Allocated= 0;") == 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Encode Memory Currently Allocated == 0 - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Encode Memory Currently Allocated != 0 - %s - Failed", bufferString4.c_str());
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
        }

        FILE *infile5 = fopen(MemLeakCheckTXT2, "rb");

        if (infile5 == NULL)
        {
            char OutputChar1[255];
            char MemLeakCheckTXTFileName[200];
            FileName(MemLeakCheckTXT2, MemLeakCheckTXTFileName);
            snprintf(OutputChar1, 255, "File not found: %s - Failed", MemLeakCheckTXTFileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {

            char buffer5[256];

            fgets(buffer5 , 256 , infile5);
            fgets(buffer5 , 256 , infile5);
            fgets(buffer5 , 256 , infile5);
            fgets(buffer5 , 256 , infile5);

            string bufferString5 = buffer5;

            if (bufferString5.compare(0, 24, "_currently Allocated= 0;") == 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Decode Memory Currently Allocated == 0 - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Decode Memory Currently Allocated != 0 - %s - Failed", bufferString5.c_str());
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
        }

        if (infile4 != NULL)fclose(infile4);

        if (infile5 != NULL)fclose(infile5);

        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);

            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;


    }
    int MinQTest(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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


        printf("Min Quantizer Test");
        fprintf(stderr, "Min Quantizer Test");

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
        opt.worst_allowed_q = 63;

        int n = 0;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.BestAllowedQ = 10;

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.BestAllowedQ = 60;

                if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60, 1) == -1)
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

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.BestAllowedQ = 60;

                if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60, 1) == -1)
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

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.BestAllowedQ = 60;

                if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60, 1) == -1)
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

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.BestAllowedQ = 60;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60, 1) == -1)
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

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.BestAllowedQ = 60;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60, 1) == -1)
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

        PSNRArr[0] = IVFPSNR(input, Min10QuantOutFile, PSNRToggle, 0, 1, NULL);
        PSNRArr[1] = IVFPSNR(input, Min60QuantOutFile, PSNRToggle, 0, 1, NULL);

        printf("\n");
        fprintf(stderr, "\n");
        int Min10Q = CheckMinQ(Min10QuantOutFile, 10);
        printf("\n\n");
        fprintf(stderr, "\n\n");
        int Min60Q = CheckMinQ(Min60QuantOutFile, 60);
        printf("\n\n");
        fprintf(stderr, "\n\n");

        char Min10FileName[255] = "";
        char Min60FileName[255] = "";

        FileName(Min10QuantOutFile, Min10FileName);
        FileName(Min60QuantOutFile, Min60FileName);

        int fail = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (Min10Q != -1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all %s quantizers above MinQ - Failed", Min10FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All %s quantizers above MinQ - Passed", Min10FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (Min60Q != -1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all %s quantizers above MinQ - Failed", Min60FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All %s quantizers above MinQ - Passed", Min60FileName);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (PSNRArr[0] <= PSNRArr[1])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "MinQ 10 PSNR: %2.2f <= MinQ 60 PSNR: %2.2f - Failed", PSNRArr[0], PSNRArr[1]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "MinQ 10 PSNR: %2.2f > MinQ 60 PSNR: %2.2f - Passed", PSNRArr[0], PSNRArr[1]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (fail == 1)
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
        else
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }

    int MultiThreadedTest(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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
        WorkingDir4.append("MultiThreadedOnOutput.ivf");
        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("MultiThreadedOffOutput.ivf");

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

        printf("MultiThreaded Test");
        fprintf(stderr, "MultiThreaded Test");

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
            Time1 = TimeReturn(MultiThreaded14OutFile, 0);
            Time2 = TimeReturn(MultiThreaded00OutFile, 0);
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.MultiThreaded = CoreCount;
                opt.CpuUsed = -1;
                Time1 = TimeCompressIVFtoIVF(input, MultiThreaded14OutFile, MultiThreaded, BitRate, opt, CompressString, CoreCount, 0);

                if (Time1 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.MultiThreaded = 0;
                Time2 = TimeCompressIVFtoIVF(input, MultiThreaded00OutFile, MultiThreaded, BitRate, opt, CompressString, 0, 0);

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
                Time1 = TimeCompressIVFtoIVF(input, MultiThreaded14OutFile, MultiThreaded, BitRate, opt, CompressString, CoreCount, 0);

                if (Time1 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.MultiThreaded = 0;
                Time2 = TimeCompressIVFtoIVF(input, MultiThreaded00OutFile, MultiThreaded, BitRate, opt, CompressString, 0, 0);

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

        char Time1FileName[255] = "";
        char Time2FileName[255] = "";

        FileName(MultiThreaded14OutFile, Time1FileName);
        FileName(MultiThreaded00OutFile, Time2FileName);

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (Time1 < Time2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s time: %i < %s time2: %i - Passed", Time1FileName, Time1, Time2FileName, Time2);
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

        if (Time1 == Time2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s time: %i == %s time: %i - Indeterminate", Time1FileName, Time1, Time2FileName, Time2);
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

        if (Time1 < Time2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s time: %i > %s time: %i - Failed", Time1FileName, Time1, Time2FileName, Time2);
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
    }
    int NewVsOldPSNR(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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


        printf("New Vs Old PSNR Test");
        fprintf(stderr, "New Vs Old PSNR Test");

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
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;

                if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0) == -1)
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

                if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0) == -1)
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

                if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0) == -1)
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

                    if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0) == -1)
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

                    if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0) == -1)
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

                    if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0) == -1)
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

                    if (CompressIVFtoIVF(input, outputVP7New, speed, BitRate, opt, "VP8", 0, 0) == -1)
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

        PSNRArr[0] = IVFPSNR(input, outputVP7New, PSNRToggle, 0, 1, NULL);
        PSNRArr[1] = PSNROLD;

        printf("\nNew DataRate");
        fprintf(stderr, "\nNew DataRate\n");
        IVFDataRate(outputVP7New, 1);

        printf("\nOld DataRate");
        fprintf(stderr, "\nOld DataRate\n");
        IVFDataRate(outputVP7Old2, 1);

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (PSNRArr[0] > PSNRArr[1])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "New PSNR: %.2f > Old PSNR: %.2f - Passed", PSNRArr[0], PSNRArr[1]);
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

        if (PSNRArr[0] <= PSNRArr[1] + (PSNRArr[1] * 0.005) && PSNRArr[0] >= PSNRArr[1] - (PSNRArr[1] * 0.005))
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "New PSNR: %.2f is with in 1/2%% of old: %.2f - Min Passed", PSNRArr[0], PSNRArr[1]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            printf("\nMin Passed\n");
            fprintf(stderr, "\nMin Passed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "New PSNR: %.2f < Old PSNR: %.2f - Failed", PSNRArr[0], PSNRArr[1]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            printf("\n New PSNR: %.2f lower than Old PSNR: %.2f - Failed\n", PSNRArr[0], PSNRArr[1]);
            fprintf(stderr, "\n New PSNR: %.2f lower than Old PSNR: %.2f - Failed\n", PSNRArr[0], PSNRArr[1]);

            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int NewVsOldRealTimeSpeed(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
    {
        if (!(argc == 7 || argc == 6))
        {
            printf(
                "  NewVsOldRealTimeSpeed \n\n"
                "    <inputfile>\n"
                "    <Target Bit Rate>\n"
                "    <Exe File To Compare>\n"
                "    <Parameter Version-1=1.0.4|2=2.0.0>\n"
                "	 <Optional Settings File>\n"
                "\n"
            );
            return 0;
        }

        char *input = argv[2];
        int BitRate = atoi(argv[3]);
        int ParFileNum = atoi(argv[5]);

        char ExeInput[255];
        snprintf(ExeInput, 255, "%s", argv[4]);

        //#if defined(_WIN32)
        //	{
        //		snprintf(ExeInput,255,"%s",argv[4]);
        //	}
        //#elif defined(linux)
        //	{
        //		string ExeInputStr = argv[4];
        //		snprintf(ExeInput,255,"%s",ExeInputStr.c_str());
        //	}
        //#elif defined(__APPLE__)
        //	{
        //		string ExeInputStr = argv[4];
        //		snprintf(ExeInput,255,"%s",ExeInputStr.c_str());
        //	}
        //#elif defined(__POWERPC__)
        //	{
        //		string ExeInputStr = argv[4];
        //		snprintf(ExeInput,255,"%s",ExeInputStr.c_str());
        //	}
        //#endif

        ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
        string WorkingDirString = "";
        string Mode3TestMatch = "";
        string ExeString = "";
        char WorkingDir2[255] = "";
        char WorkingDir3[255] = "";
        char *MyDir = "NewVsOldRealTimeSpeed";
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
            char ExeChar[1024];
            char ExeChar2[1024];
            FolderName(argv[0], ExeChar2);
            snprintf(ExeChar, 1024, "%s", ExeChar2);
            ExeString = ExeChar;
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
        string WorkingDir7 = WorkingDirString;
        string WorkingDir8 = ExeString;

        WorkingDir4.append(slashCharStr);
        WorkingDir4.append("outputVP8New.ivf");
        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("outputVP8Old.ivf");
        WorkingDir6.append(slashCharStr);
        WorkingDir6.append("outputVP8Old.ivf");
        WorkingDir7.append(slashCharStr);
        WorkingDir7.append("ParFile.txt");
        WorkingDir8.append(ExeInput);

#if defined(_WIN32)
        {
            WorkingDir8.insert(0, "\"");
            WorkingDir8.insert(0, "\"");
            WorkingDir8.append("\" \"");
            WorkingDir8.append(input);
            WorkingDir8.append("\" \"");
            WorkingDir8.append(WorkingDir5);
            WorkingDir8.append("\" 8");
            WorkingDir8.append(" \"");
            WorkingDir8.append(WorkingDir7);
            WorkingDir8.append("\"");
            WorkingDir8.append(" 2");
            WorkingDir8.append("\"");
        }
#elif defined(linux)
        {
            //WorkingDir8.insert(0,"\"");
            WorkingDir8.insert(0, "\'");
            WorkingDir8.append("\' \'");
            WorkingDir8.append(input);
            WorkingDir8.append("\' \'");
            WorkingDir8.append(WorkingDir5);
            WorkingDir8.append("\' 8");
            WorkingDir8.append(" \'");
            WorkingDir8.append(WorkingDir7);
            WorkingDir8.append("\'");
            WorkingDir8.append(" 2");
            //WorkingDir8.append("\"");
        }
#elif defined(__APPLE__)
        {
            //WorkingDir8.insert(0,"\"");
            WorkingDir8.insert(0, "\'");
            WorkingDir8.append("\' \'");
            WorkingDir8.append(input);
            WorkingDir8.append("\' \'");
            WorkingDir8.append(WorkingDir5);
            WorkingDir8.append("\' 8");
            WorkingDir8.append(" \'");
            WorkingDir8.append(WorkingDir7);
            WorkingDir8.append("\'");
            WorkingDir8.append(" 2");
            //WorkingDir8.append("\"");
        }
#elif defined(__POWERPC__)
        {
            //WorkingDir8.insert(0,"\"");
            WorkingDir8.insert(0, "\'");
            WorkingDir8.append("\' \'");
            WorkingDir8.append(input);
            WorkingDir8.append("\' \'");
            WorkingDir8.append(WorkingDir5);
            WorkingDir8.append("\' 8");
            WorkingDir8.append(" \'");
            WorkingDir8.append(WorkingDir7);
            WorkingDir8.append("\'");
            WorkingDir8.append(" 2");
            //WorkingDir8.append("\"");
        }
#endif

        char outputVP8New[255];
        char outputVP8Old[255];
        char outputVP8Old2[255];
        char ParFile[255];
        char Program[1024];

        snprintf(outputVP8New, 255, "%s", WorkingDir4.c_str());
        snprintf(outputVP8Old, 255, "%s", WorkingDir5.c_str());
        snprintf(outputVP8Old2, 255, "%s", WorkingDir6.c_str());
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

        int speed = 0;


        unsigned int Time1;
        unsigned int Time2 = 0;
        unsigned int Time3 = 0;

        printf("New Vs Old Real Time Speed Test");
        fprintf(stderr, "New Vs Old Real Time Speed Test");

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
        opt.CpuUsed = -1;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            Time1 = TimeReturn(outputVP8New, 0);
            Time2 = TimeReturn(outputVP8Old2, 0);

            cout << "\nTime1: " << Time1 << "\n";
            cout << "\nTime2: " << Time2 << "\n";
        }
        else
        {

            opt.Mode = MODE_REALTIME;
            Time1 = TimeCompressIVFtoIVF(input, outputVP8New, speed, BitRate, opt, "VP8", 0, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutCompatSettings(ParFile, opt, ParFileNum);
            printf("\nCompressing Old File\n");

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            system(Program);
            Time2 = TimeReturn(outputVP8Old2, 0);

            printf("\n\nFile completed: Time in Microseconds: %i", Time2);
            fprintf(stderr, "\n\nFile completed: Time in Microseconds: %i", Time2);
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

        printf("\n\n\nResults:\n\n");
        fprintf(stderr, "\n\n\nResults:\n\n");

        if (Time1 > Time2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Old: %i is Faster than New: %i - Failed", Time2, Time1);
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

        if (Time1 < Time2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "New: %i is Faster than Old: %i - Passed", Time1, Time2);
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
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Files Took the same amount of time - Indeterminate");
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
    int NoiseSensitivityWorks(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
    {
        //This test looks a lot like NoiseSensitivityWorks but has a different purpose.  This test ensures
        //That different noise sensitivities have an effect for each possible noise sensitivity value and
        //passes only if that is the case.
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
        WorkingDir4.append("NoiseSenseOut");

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

                PSNRArr[Noise] = IVFPSNR(input, NoiseSenseOut, PSNRToggle, 0, 1, NULL);
                printf("\n");
                fprintf(stderr, "\n");
                File2bytes[Noise] = FileSize(NoiseSenseOut);
                printf("\n");
                fprintf(stderr, "\n");

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

                    if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise, 0) == -1)
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

                    if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise, 0) == -1)
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

                    if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise, 0) == -1)
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

                    if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise, 0) == -1)
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

                    if (CompressIVFtoIVF(input, NoiseSenseOut, speed, BitRate, opt, CompressString, Noise, 0) == -1)
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

                    PSNRArr[Noise] = IVFPSNR(input, NoiseSenseOut, PSNRToggle, 0, 1, NULL);
                    printf("\n");
                    fprintf(stderr, "\n");
                    File2bytes[Noise] = FileSize(NoiseSenseOut);
                    printf("\n");
                    fprintf(stderr, "\n");
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
        int fail = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        while (n != 6)
        {
            if (PSNRArr[n] == PSNRArr[n+1] && File2bytes[n] == File2bytes[n+1])
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Noise %i PSNR %.2f == Noise %i PSNR %.2f - Failed", n, PSNRArr[n], n + 1, PSNRArr[n+1]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;

            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Noise %i PSNR %.2f != Noise %i PSNR %.2f - Passed", n, PSNRArr[n], n + 1, PSNRArr[n+1]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }

            n++;
        }

        if (PSNRArr[0] <= PSNRArr[6])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Noise 0 PSNR: %.2f <= Noise 6 PSNR: %.2f - Failed", PSNRArr[0], PSNRArr[6]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Noise 0 PSNR: %.2f > Noise 6 PSNR: %.2f - Passed", PSNRArr[0], PSNRArr[6]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int OnePassVsTwoPass(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
    {
        char *CompressString = "AllowDF";

        if (!(argc == 4 || argc == 5))
        {
            printf(
                "  OnePassVsTwoPass \n\n"
                "    <inputfile>\n"
                "    <Target Bit Rate>\n ");
            return 0;
        }

        ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
        string WorkingDirString = "";
        string Mode3TestMatch = "";
        char WorkingDir2[255] = "";
        char WorkingDir3[255] = "";
        char *MyDir = "OnePassVsTwoPass";
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
        string WorkingDir4b = WorkingDirString;
        string WorkingDir5b = WorkingDirString;
        string WorkingDir4c = WorkingDirString;
        string WorkingDir5c = WorkingDirString;

        WorkingDir4.append(slashCharStr);
        WorkingDir4.append("TwoPassOutput1.ivf");
        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("OnePassOutput1.ivf");
        WorkingDir4b.append(slashCharStr);
        WorkingDir4b.append("TwoPassOutput2.ivf");
        WorkingDir5b.append(slashCharStr);
        WorkingDir5b.append("OnePassOutput2.ivf");
        WorkingDir4c.append(slashCharStr);
        WorkingDir4c.append("TwoPassOutput3.ivf");
        WorkingDir5c.append(slashCharStr);
        WorkingDir5c.append("OnePassOutput3.ivf");

        char TwoPassOutFile1[255];
        char OnePassOutFile1[255];
        char TwoPassOutFile2[255];
        char OnePassOutFile2[255];
        char TwoPassOutFile3[255];
        char OnePassOutFile3[255];

        snprintf(TwoPassOutFile1, 255, "%s", WorkingDir4.c_str());
        snprintf(OnePassOutFile1, 255, "%s", WorkingDir5.c_str());
        snprintf(TwoPassOutFile2, 255, "%s", WorkingDir4b.c_str());
        snprintf(OnePassOutFile2, 255, "%s", WorkingDir5b.c_str());
        snprintf(TwoPassOutFile3, 255, "%s", WorkingDir4c.c_str());
        snprintf(OnePassOutFile3, 255, "%s", WorkingDir5c.c_str());

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

        printf("One Pass Vs Two Pass Test");
        fprintf(stderr, "One Pass Vs Two Pass Test");

        char *input = argv[2];
        int BitRate = atoi(argv[3]);

        int speed = 0;

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

        int BitRate1 = BitRate - (BitRate * 0.3);
        int BitRate2 = BitRate;
        int BitRate3 = BitRate + (BitRate * 0.3);


        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            opt.Mode = 5;
            opt.TargetBandwidth = BitRate1;

            if (CompressIVFtoIVF(input, TwoPassOutFile1, speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate2;

            if (CompressIVFtoIVF(input, TwoPassOutFile2, speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate3;

            if (CompressIVFtoIVF(input, TwoPassOutFile3, speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = 2;
            opt.TargetBandwidth = BitRate1;

            if (CompressIVFtoIVF(input, OnePassOutFile1, speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate2;

            if (CompressIVFtoIVF(input, OnePassOutFile2, speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate3;

            if (CompressIVFtoIVF(input, OnePassOutFile3, speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

        float SizeTwoPass1 = IVFDataRate(TwoPassOutFile1, 1);
        float SizeOnePass1 = IVFDataRate(OnePassOutFile1, 1);
        float SizeTwoPass2 = IVFDataRate(TwoPassOutFile2, 1);
        float SizeOnePass2 = IVFDataRate(OnePassOutFile2, 1);
        float SizeTwoPass3 = IVFDataRate(TwoPassOutFile3, 1);
        float SizeOnePass3 = IVFDataRate(OnePassOutFile3, 1);

        //int PSNRToggle = PSNRSelect(input, TwoPassOutFile);

        double PSNRTwoPass1;
        double PSNRTwoPass2;
        double PSNRTwoPass3;

        double PSNROnePass1;
        double PSNROnePass2;
        double PSNROnePass3;

        PSNRTwoPass1 = IVFPSNR(input, TwoPassOutFile1, 1, 0, 1, NULL);
        PSNRTwoPass2 = IVFPSNR(input, TwoPassOutFile2, 1, 0, 1, NULL);
        PSNRTwoPass3 = IVFPSNR(input, TwoPassOutFile3, 1, 0, 1, NULL);
        PSNROnePass1 = IVFPSNR(input, OnePassOutFile1, 1, 0, 1, NULL);
        PSNROnePass2 = IVFPSNR(input, OnePassOutFile2, 1, 0, 1, NULL);
        PSNROnePass3 = IVFPSNR(input, OnePassOutFile3, 1, 0, 1, NULL);

        //    double PSRNPerc = absDouble(((PSNR2 - PSNR1) / PSNR1) * 100.00);
        //    double BRPerc = absDouble(((Size2 - Size1) / Size1) * 100.00);
        int Pass = 0;

        float TwoPassA = 0;
        float TwoPassB = 0;
        float TwoPassC = 0;

        float OnePassA = 0;
        float OnePassB = 0;
        float OnePassC = 0;

        float minCommon = 0;
        float maxCommon = 0;

        if (SizeTwoPass1 > SizeOnePass1) //take area over same range we have decent data for.
        {
            minCommon = SizeTwoPass1;
        }
        else
        {
            minCommon = SizeOnePass1;
        }

        if (SizeTwoPass3 > SizeOnePass3)
        {
            maxCommon = SizeOnePass3;
        }
        else
        {
            maxCommon = SizeTwoPass3;
        }

        SolveQuadradic(SizeTwoPass1, SizeTwoPass2, SizeTwoPass3, PSNRTwoPass1, PSNRTwoPass2, PSNRTwoPass3, TwoPassA, TwoPassB, TwoPassC);
        float TwoPassAreaVal = AreaUnderQuadradic(TwoPassA, TwoPassB, TwoPassC, minCommon, maxCommon);

        //cout << "\n\nGoodAreaVal: " << GoodAreaVal << "\n\n";

        SolveQuadradic(SizeOnePass1, SizeOnePass2, SizeOnePass3, PSNROnePass1, PSNROnePass2, PSNROnePass3, OnePassA, OnePassB, OnePassC);
        float OnePassAreaVal = AreaUnderQuadradic(OnePassA, OnePassB, OnePassC, minCommon, maxCommon);

        //cout << "\n\nBestAreaVal: " << BestAreaVal << "\n\n";

        /*cout << GoodA << "\n";
        cout << GoodB << "\n";
        cout << GoodC << "\n";

        cout << BestA << "\n";
        cout << BestB << "\n";;
        cout << BestC << "\n";*/

        //float PSRNPerc = absFloat((PSNRB1 - PSNRG1) / PSNRG1) * 100.00;
        //float BRPerc = (absFloat(BestSize1 - GoodSize1) / GoodSize1) * 100.00;

        printf("\n\n"
               "Data Points:\n"
               "\n"
               " Two Pass\n"
               "\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "\n"
               " One Pass\n"
               "\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "\n"
               "\n"
               , SizeTwoPass1, PSNRTwoPass1
               , SizeTwoPass2, PSNRTwoPass2
               , SizeTwoPass3, PSNRTwoPass3
               , SizeOnePass1, PSNROnePass1
               , SizeOnePass2, PSNROnePass2
               , SizeOnePass3, PSNROnePass3
              );

        fprintf(stderr, "\n\n"
                "Data Points:\n"
                "\n"
                " Two Pass\n"
                "\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "\n"
                " One Pass\n"
                "\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "\n"
                "\n"
                , SizeTwoPass1, PSNRTwoPass1
                , SizeTwoPass2, PSNRTwoPass2
                , SizeTwoPass3, PSNRTwoPass3
                , SizeOnePass1, PSNROnePass1
                , SizeOnePass2, PSNROnePass2
                , SizeOnePass3, PSNROnePass3
               );

        printf("Two Pass Curve: y = %fx^2 + %fx + %f\n", TwoPassA, TwoPassB, TwoPassC);
        printf("One Pass Curve: y = %fx^2 + %fx + %f\n", OnePassA, OnePassB, OnePassC);

        fprintf(stderr, "Good Quality Curve: y = %fx^2 + %fx + %f\n", TwoPassA, TwoPassB, TwoPassC);
        fprintf(stderr, "Best Quality Curve: y = %fx^2 + %fx + %f\n", OnePassA, OnePassB, OnePassC);

        printf("\nGood Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, TwoPassAreaVal);
        printf("Best Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, OnePassAreaVal);

        fprintf(stderr, "\nGood Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, TwoPassAreaVal);
        fprintf(stderr, "Best Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, OnePassAreaVal);

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (TwoPassAreaVal == OnePassAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Two Pass area under curve: %.2f == One Pass area under curve: %.2f - Failed", TwoPassAreaVal, OnePassAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (OnePassAreaVal < TwoPassAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Two Pass area under curve: %.2f > One Pass area under curve: %.2f - Passed", TwoPassAreaVal, OnePassAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            Pass = 1;
        }

        if (OnePassAreaVal > TwoPassAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Two Pass  area under curve: %.2f < One Pass area under curve: %.2f - Failed", TwoPassAreaVal, OnePassAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (Pass == 1)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }


    int PlayAlternate(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

        printf("Play Alternate Test");
        fprintf(stderr, "Play Alternate Test");

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
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.PlayAlternate = PlayAlternate1Val;

                if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.PlayAlternate = PlayAlternate2Val;

                if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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

                if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.PlayAlternate = PlayAlternate2Val;

                if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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

                if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.PlayAlternate = PlayAlternate2Val;

                if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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

                if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.PlayAlternate = PlayAlternate2Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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

                if (CompressIVFtoIVF(input, PlayAlternate1, speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.PlayAlternate = PlayAlternate2Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, PlayAlternate2, speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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

        int fail = 0;

        int PlayAlternateOnAltRefCount = IVFDisplayAltRefFrames(PlayAlternate2, 1);
        int PlayAlternateOffAltRefCount = IVFDisplayAltRefFrames(PlayAlternate1, 1);
        int VisibleFrameONCount = IVFDisplayVisibleFrames(PlayAlternate2, 1);
        int VisibleFrameOFFCount = IVFDisplayVisibleFrames(PlayAlternate1, 1);

        char PlayAlternateOnFilename[255];
        FileName(PlayAlternate2, PlayAlternateOnFilename);
        char PlayAlternateOffFilename[255];
        FileName(PlayAlternate1, PlayAlternateOffFilename);

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (PlayAlternateOnAltRefCount > 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Alternate reference frames exist for %s - Passed", PlayAlternateOnFilename);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Alternate reference frames do not exist for %s - Failed", PlayAlternateOnFilename);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        if (PlayAlternateOffAltRefCount > 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Alternate reference frames exist for %s - Failed", PlayAlternateOffFilename);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Alternate reference frames do not exist for %s - Passed", PlayAlternateOffFilename);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (VisibleFrameONCount == VisibleFrameOFFCount)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Visible Frames for %s = %i == Visible Frames for %s = %i - Passed", PlayAlternateOnFilename, VisibleFrameONCount, PlayAlternateOffFilename, VisibleFrameOFFCount);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Visible Frames for %s = %i != Visible Frames for %s = %i - Failed", PlayAlternateOnFilename, VisibleFrameONCount, PlayAlternateOffFilename, VisibleFrameOFFCount);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        if (lngRC >= 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Files are not identical - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (lngRC == -1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Files are identical - Failed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }

    int PostProcessorWorks(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

        printf("Post Processor Works Test");
        fprintf(stderr, "Post Processor Works Test");

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
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;

                if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile, speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

        printf("\nCaculating PSNR: NOFILTERING DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        fprintf(stderr, "\nCaculating PSNR: ONYXD_NOFILTERING DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile, PSNRToggle, 0, 1, deblock_level, 0, flags, ssim);
        countme++;

        flags++;
        printf("\nCaculating PSNR: DEBLOCK DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        fprintf(stderr, "\nCaculating PSNR: ONYXD_DEBLOCK DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
        PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile, PSNRToggle, 0, 1, deblock_level, noise_level, flags, ssim);
        countme++;
        flags++;

        while (deblock_level != 16)
        {
            printf("\nCaculating PSNR: DEMACROBLOCK DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
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
            printf("\nCaculating PSNR: ADDNOISE DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
            fprintf(stderr, "\nCaculating PSNR: ADDNOISE DeblockLevel %i NoiseLevel %i \n", deblock_level, noise_level);
            PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile, PSNRToggle, 0, 1, deblock_level, noise_level, flags, ssim);
            countme++;

            noise_level++;
        }

        int x = 0;
        cout << "\n";

        int TestFail = 0;
        int TenPercent = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (PSNRArr[0] != PSNRArr[1])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", PSNRArr[1], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            //printf("DEBLOCK PSNR: %4.2f != NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);
            //fprintf(stderr, "ONYXD_DEBLOCK PSNR: %f != NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Failed", PSNRArr[1], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            //printf("DEBLOCK PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
            //fprintf(stderr, "DEBLOCK PSNR: %4.2f  == NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
            TestFail = 1;
        }

        printf("\n");
        fprintf(stderr, "\n");

        x = 2;

        while (x < 18)
        {
            if (PSNRArr[0] != PSNRArr[x])
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "DeblockLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", 2, x - 2, PSNRArr[x], PSNRArr[0]);;
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);

            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "DeblockLevel %*i PSNR: %4.2f += NOFILTERING PSNR: %4.2f - Failed", 2, x - 2, PSNRArr[x], PSNRArr[0]);;
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
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
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "NoiseLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE NoiseLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE NoiseLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);

            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "NoiseLevel %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Failed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE NoiseLevel %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE NoiseLevel %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
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
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f within 10%% of %4.2f - Passed", PSNRArr[1], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            //printf("DEBLOCK PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);
            //fprintf(stderr, "DEBLOCK PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", PSNRArr[1], PSNRArr[0]);

        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f not within 10%% of %4.2f - Failed", PSNRArr[1], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            //printf("DEBLOCK PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
            //fprintf(stderr, "DEBLOCK PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f - Fail\n", PSNRArr[1], PSNRArr[0]);
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
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "DeblockLevel %*i PSNR: %4.2f within 10%% of %4.2f - Passed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "DEMACROBLOCK DeblockLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "DeblockLvl %*i PSNR: %4.2f not within 10%% of %4.2f - Failed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("DEMACROBLOCK DeblockLvl %*i PSNR: %4.2f not within 10%% of NOFILTER PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "DEMACROBLOCK DeblockLvl %*i PSNR: %4.2f not within 10%% of NOFILTER PSNR: %4.2f\n", 2, x - 2, PSNRArr[x], PSNRArr[0]);
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
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "NoiseLevel %*i PSNR: %4.2f within 10%% of %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE NoiseLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE NoiseLevel %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);

            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "NoiseLevel %*i PSNR: %4.2f not within 10%% of %4.2f - Indeterminate", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE NoiseLevel %*i PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE NoiseLevel %*i PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                TenPercent = 1;
            }

            x++;
        }

        if (TestFail == 0)
        {
            if (TenPercent == 0)
            {
                //printf("\n All Post Processing Values Return Different PSNRs and are within 10%% of original PSNR - Passed\n");
                //fprintf(stderr, "\n All Post Processing Values Return Different PSNRs and are within 10%% of original PSNR - Passed\n");

                printf("\nPassed\n");
                fprintf(stderr, "\nPassed\n");

                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 1;
            }
            else
            {
                //printf("\n All Post Processing Values Return Different PSNRs but are not all within 10%% of original PSNR - Passed\n");
                //fprintf(stderr, "\n All Post Processing Values Return Different PSNRs but are not all within 10%% of original PSNR - Passed\n");

                printf("\nMin Passed\n");
                fprintf(stderr, "\nMin Passed\n");

                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 8;
            }


        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int ReconBuffer(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
    {

        char *CompressString = "Allow Drop Frames";

        char *input = argv[2];

        if (!(argc == 6 || argc == 5))
        {
            printf(
                "  ReconBuffer \n\n"
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
        char *MyDir = "ReconBuffer";
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
        WorkingDir4.append("ReconBuffer.ivf");
        //WorkingDir5.append(slashCharStr);
        //WorkingDir5.append("AllowDFOffOutput.ivf");

        char ReconBuffer[255];
        //char AllowDFoff[255];

        snprintf(ReconBuffer, 255, "%s", WorkingDir4.c_str());
        //snprintf(AllowDFoff, 255, "%s", WorkingDir5.c_str());

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

        printf("Recon Buffer Test");
        fprintf(stderr, "Recon Buffer Test");

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
        opt.lag_in_frames = 0;
        opt.play_alternate = 0;
        //opt.lag_in_frames = 18; //Test works when lag in frames = 0 but does not work if lag in
        //frames greater than 0 so for temp fix we just use lag in frames
        // = 0.

        //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
        //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
        //Test Type 3 = Mode 2 = Run Test Compressions

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer, speed, BitRate, opt, CompressString, 0, 0) == -1)
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
                opt.AllowDF = 0;

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer, speed, BitRate, opt, CompressString, 0, 0) == -1)
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

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer, speed, BitRate, opt, CompressString, 0, 0) == -1)
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

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer, speed, BitRate, opt, CompressString, 0, 0) == -1)
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

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer, speed, BitRate, opt, CompressString, 0, 0) == -1)
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

        //int AllowDFonFrames = IVFDisplayVisibleFrames(AllowDFon, 1);
        //int AllowDFoffFrames = IVFDisplayVisibleFrames(AllowDFoff, 1);

        /*printf("\n\n");
        fprintf(stderr, "\n\n");
        long File1bytes = FileSize(AllowDFon);
        printf("\n");
        fprintf(stderr, "\n");
        long File2bytes = FileSize(AllowDFoff);
        printf("\n");
        fprintf(stderr, "\n");*/

        /*char OutputChar1[255];
        snprintf(OutputChar1, 255, "",);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");fprintf(stderr, "\n");*/

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        int fail = 0;

        //int NumberofFramesInFile = GetNumberofFrames(ReconBuffer);

        ifstream ReconOutFile;
        string ReconOutStr = ReconBuffer;
        ReconOutStr.erase(ReconOutStr.length() - 4, 4);
        ReconOutStr.append("_ReconFrameState.txt");
        char ReconOutChar[255];
        snprintf(ReconOutChar, 255, "%s", ReconOutStr.c_str());

        ReconOutFile.open(ReconOutChar);

        int Frame;
        char BufferLetter;
        int State;

        while (!ReconOutFile.eof())
        {
            ReconOutFile >> Frame;
            ReconOutFile >> BufferLetter;
            ReconOutFile >> State;

            if (State == 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Frame: %i Buffer: %c - Preview not identical to Decoded - Failed", Frame, BufferLetter);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 1;
            }
        }

        ReconOutFile.close();

        if (fail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All preview frames are identical to decoded frames - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int ResampleDownWaterMark(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
    {
        char *CompressString = "ResampleDownWaterMark";

        if (!(argc == 6 || argc == 5))
        {
            printf(
                "  ResampleDownWaterMark \n\n"
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
        char *MyDir = "ResampleDownWaterMark";
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
        WorkingDir4.append("DownWaterSamp90Output.ivf");
        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("DownWaterSamp10Output.ivf");

        char DownWaterSamp90OutFile[255];
        char DownWaterSamp10OutFile[255];

        snprintf(DownWaterSamp90OutFile, 255, "%s", WorkingDir4.c_str());
        snprintf(DownWaterSamp10OutFile, 255, "%s", WorkingDir5.c_str());

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

        printf("Resample Down WaterMark Test");
        fprintf(stderr, "Resample Down WaterMark Test");

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
        //opt.auto_key = 0;
        opt.allow_lag = 0;
        opt.AllowSpatialResampling = 1;
        opt.EndUsage = 0;
        opt.TargetBandwidth = BitRate;

        int ResampleDownWaterMarkHigh = 90;
        int ResampleDownWaterMarkLow = 20;

        int n = 0;
        double PSNRArr[2];

        opt.ResampleUpWaterMark = 100;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.ResampleDownWaterMark = ResampleDownWaterMarkHigh;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.ResampleDownWaterMark = ResampleDownWaterMarkLow;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
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
                opt.ResampleDownWaterMark = ResampleDownWaterMarkHigh;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.ResampleDownWaterMark = ResampleDownWaterMarkLow;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
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
                opt.ResampleDownWaterMark = ResampleDownWaterMarkHigh;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.ResampleDownWaterMark = ResampleDownWaterMarkLow;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
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
                opt.ResampleDownWaterMark = ResampleDownWaterMarkHigh;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.ResampleDownWaterMark = ResampleDownWaterMarkLow;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.ResampleDownWaterMark = ResampleDownWaterMarkHigh;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.ResampleDownWaterMark = ResampleDownWaterMarkLow;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark, 0) == -1)
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

        int PSNRToggle = PSNRSelect(input, DownWaterSamp90OutFile);

        char DownWaterSamp10Filename[255];
        FileName(DownWaterSamp10OutFile, DownWaterSamp10Filename);
        char DownWaterSamp90Filename[255];
        FileName(DownWaterSamp90OutFile, DownWaterSamp90Filename);

        int DispKeyFrames10int = IVFDisplayKeyFrames(DownWaterSamp10OutFile, 1);
        int DispResized10int = IVFDisplayResizedFrames(DownWaterSamp10OutFile, 1);
        int DispCheckPBMThr10int = IVFCheckPBMThreshold(DownWaterSamp10OutFile, opt.target_bandwidth, opt.maximum_buffer_size * 1000, opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000, ResampleDownWaterMarkLow);
        int RDWMCheck10int = 0;

        if (DispResized10int > 0)
        {
            printf("\nChecking %s buffer for correct resize frame placement:\n\n", DownWaterSamp10Filename);
            RDWMCheck10int = IVFDFWMCheck(DownWaterSamp10OutFile, 1);
        }
        else
        {
            RDWMCheck10int = -2;
        }

        int DispKeyFrames90int = IVFDisplayKeyFrames(DownWaterSamp90OutFile, 1);
        int DispResized90int = IVFDisplayResizedFrames(DownWaterSamp90OutFile, 1);
        int DispCheckPBMThr90int = IVFCheckPBMThreshold(DownWaterSamp90OutFile, opt.target_bandwidth, opt.maximum_buffer_size * 1000, opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000, ResampleDownWaterMarkHigh);
        int RDWMCheck90int = -3;

        if (DispResized90int > 0)
        {
            printf("\nChecking %s buffer for correct resize frame placement:\n\n", DownWaterSamp90Filename);
            RDWMCheck90int = IVFDFWMCheck(DownWaterSamp90OutFile, 1);
        }
        else
        {
            RDWMCheck90int = -2;
        }

        /*cout << "\n";
        cout << "DispKeyFrames90int: " <<  DispKeyFrames90int << "\n";
        cout << "DispResized90int: " <<  DispResized90int << "\n";
        cout << "DispCheckPBMThr90int: " <<  DispCheckPBMThr90int << "\n";
        cout << "RDWMCheck90int: " << RDWMCheck90int << "\n";
        cout << "\n";
        cout << "DispKeyFrames10int: " <<  DispKeyFrames10int << "\n";
        cout << "DispResized10int: " <<  DispResized10int << "\n";
        cout << "DispCheckPBMThr10int: " <<  DispCheckPBMThr10int << "\n";
        cout << "RDWMCheck90int: " << RDWMCheck90int << "\n";*/

        int fail = 0; //1 = failed // 2 = indt // 3 = track resize for 10 // track resize for 90
        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (DispResized10int > 0 && DispResized90int > 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Both DWMS 10 and 90 returned resized frames - Indeterminate");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (DispResized10int == 0 && DispResized90int > 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 10 returned no resized frames; DWMS 90 returned resized frames - Indeterminate");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 4;
            //indt
        }

        if (DispResized10int > 0 && DispResized90int == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 90 returned no resized frames; DWMS 10 returned resized frames - Failed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
            //fail
        }

        if (DispResized10int == 0 && DispResized90int == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Both DWMS 10 and 90 returned  no resized frames - Indeterminate");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 2;
        }

        if (RDWMCheck10int == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 10 resizes first frame at correct buffer location - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            //fail
        }

        if (RDWMCheck10int == 1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 10 does not resize first frame at correct buffer location - Failed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
            //fail
        }

        if (RDWMCheck10int == -3)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 10 buffer threshold never reached - Indeterminate");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 2;
            //fail
        }

        if (RDWMCheck90int == 0)
        {
            if (fail == 4)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "DWMS 90 resizes first frame at correct buffer location - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                fail = 3;
                //fail
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "DWMS 90 resizes first frame at correct buffer location - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                //fail
            }
        }

        if (RDWMCheck90int == 1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 90 does not resize first frame at correct buffer location - Failed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 2;
            //fail
        }

        if (RDWMCheck90int == -3)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 90 buffer threshold never reached - Indeterminate");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 2;
            //fail
        }

        /*if (PSNRArr[0] < PSNRArr[1])
        {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Down Water Mark Sample 10 PSNR: %.2f > 90 PSNR: %.2f - Passed", PSNRArr[1], PSNRArr[0]);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        }
        if (PSNRArr[0] == PSNRArr[1])
        {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Resample-Down-Watermark has no effect - Indeterminate");
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        }
        if(PSNRArr[0] > PSNRArr[1])
        {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Down Water Mark Sample 90 PSNR: %f > 10 PSNR : %f \n", PSNRArr[0], PSNRArr[1]);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        fail = 1;
        }*/
        if (fail == 0)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }

        if (fail == 3)
        {
            printf("\nMin Passed\n");
            fprintf(stderr, "\nMin Passed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }

        if (fail == 2)
        {
            printf("\nIndeterminate\n");
            fprintf(stderr, "\nIndeterminate\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }
    int SpeedTest(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

                    GoodTotalms[counter] = TimeReturn(SpeedTestGoodQ, 0);
                    GoodPSNRArr[counter] = IVFPSNR(input, SpeedTestGoodQ, 1, 0, 1, NULL);
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

                    RealTotalms[counter2] = TimeReturn(SpeedTestRealTime, 0);
                    RealPSNRArr[counter2] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, NULL);
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

                    RealTotalmsPos[counter] = TimeReturn(SpeedTestRealTime, 0);
                    RealPSNRArrPos[counter] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, NULL);
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
                    GoodTotalms[counter] = TimeCompressIVFtoIVF(input, SpeedTestGoodQ, speed, BitRate, opt, CompressString, CompressInt, 0);

                    if (GoodTotalms[counter] == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    if (TestType != 2 && TestType != 3)
                    {
                        GoodPSNRArr[counter] = IVFPSNR(input, SpeedTestGoodQ, 1, 0, 1, NULL);
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
                    RealTotalms[counter2] = TimeCompressIVFtoIVF(input, SpeedTestRealTime, speed, BitRate, opt, CompressString, CompressInt, 0);

                    if (TestType != 2 && TestType != 3)
                    {
                        RealPSNRArr[counter2] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, NULL);
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
                    RealTotalmsPos[counter] = TimeCompressIVFtoIVF(input, SpeedTestRealTime, speed, BitRate, opt, CompressString, CompressInt, 0);

                    if (RealTotalmsPos[counter] == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    if (TestType != 2 && TestType != 3)
                    {
                        RealPSNRArrPos[counter] = IVFPSNR(input, SpeedTestRealTime, 1, 0, 1, NULL);
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
        int pass = 1;

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


        if (Mode == 0)
        {
            printf("\n\nResults:\n\n");
            fprintf(stderr, "\n\nResults:\n\n");

            if (Fail == 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "All encode times decrease as CpuUsed increases - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("All encode times decrease as CpuUsed increases - Passed");
            }

            if (Fail < 4 && Fail != 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Enough encode times decrease as CpuUsed increases - Min Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                pass = 2;
            }

            if (Fail >= 4)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Not enough encode times decrease as CpuUsed increases - Failed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                pass = 0;
            }

            if (Fail2 == 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "All PSNRs are within 10%% - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
            }

            if (Fail2 < 2 && Fail2 != 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Enough PSNRs are within 10%% - Min Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                pass = 2;
            }

            if (Fail2 >= 2)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Not enough PSNRs are within 10%% - Failed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                pass = 0;
            }

            if (Failb != 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Not all Encode speeds are within 10%% - Failed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");
                pass = 0;

                //printf("\nFail - Not all Encode speeds are within 10%%\n");
                //fprintf(stderr, "\nFail - Not all Encode speeds are within 10%%\n");
                //pass = 0;
            }
        }



        if (Mode == 1)
        {
            printf("\n\nResults:\n\n");
            fprintf(stderr, "\n\nResults:\n\n");

            if (Fail == 1)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Not all encode times decrease as CpuUsed increases - Failed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("Not all encode times decrease as CpuUsed increases - Failed");
                pass = 0;
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "All encode times decrease as CpuUsed increases - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("All encode times decrease as CpuUsed increases - Passed");
            }

            if (Fail2 == 0)
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "All PSNR values are within 10%% of eachother - Passed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("All PSNR values are within 10% of eachother - Passed");
            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "Not all PSNR values are within 10%% of eachother - Failed");
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("Not all PSNR values are within 10% of eachother - Failed");
                pass = 0;
            }
        }

        if (pass == 0)
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (pass == 1)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nMin Passed\n");
            fprintf(stderr, "\nMin Passed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }


    }
    int TestVectorCheck(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

        printf("Test Vector Check Test");
        fprintf(stderr, "Test Vector Check Test");

        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
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

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (Fail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All decoded test vector MD5 checksum's match expected checksum's - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            //printf("\n\n All Decoded Test Vector Check Sums Match Expected Check Sums: Passed \n\n");
            //fprintf(stderr, "\n\n  All Decoded Test Vector Check Sums Match Expected Check Sums: Passed \n\n");

            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            string FailStr = "Not all decoded test vectors match expected MD5 checksum's.";

            //printf("\n   Test Vectors that FAIL: ");
            //fprintf(stderr, "\n   Test Vectors that FAIL: ");

            int q = 0;

            if (FailVector.size() == 1)
            {
                FailStr.append(" Test Vector ");
            }
            else
            {
                FailStr.append(" Test Vectors ");
            }

            while (q < FailVector.size())
            {


                if (q != 0)
                {
                    if (q + 1 != FailVector.size())
                    {
                        FailStr.append(", ");
                        //printf(", ");
                        //fprintf(stderr, ", ");
                    }
                    else
                    {
                        FailStr.append(" and ");
                    }
                }

                char TVFailNumChar[32] = "";
                itoa_custom(FailVector[q], TVFailNumChar, 10);
                FailStr.append(TVFailNumChar);
                //printf("%i", FailVector[q]);
                //fprintf(stderr, "%i", FailVector[q]);
                q++;
            }

            if (FailVector.size() == 1)
            {
                FailStr.append(" does not match");
            }
            else
            {
                FailStr.append(" do not match");
            }

            FailStr.append(" - Failed");
            //printf("\n");
            //fprintf(stderr, "\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, FailStr.c_str());
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
    }
    int TwoPassVsTwoPassBest(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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
        string WorkingDir4b = WorkingDirString;
        string WorkingDir5b = WorkingDirString;
        string WorkingDir4c = WorkingDirString;
        string WorkingDir5c = WorkingDirString;

        WorkingDir4.append(slashCharStr);
        WorkingDir4.append("TwoPassOutput1.ivf");
        WorkingDir5.append(slashCharStr);
        WorkingDir5.append("TwoPassBestOutput1.ivf");
        WorkingDir4b.append(slashCharStr);
        WorkingDir4b.append("TwoPassOutput2.ivf");
        WorkingDir5b.append(slashCharStr);
        WorkingDir5b.append("TwoPassBestOutput2.ivf");
        WorkingDir4c.append(slashCharStr);
        WorkingDir4c.append("TwoPassOutput3.ivf");
        WorkingDir5c.append(slashCharStr);
        WorkingDir5c.append("TwoPassBestOutput3.ivf");

        char TwoPassOutFile1[255];
        char TwoPassBestOutFile1[255];
        char TwoPassOutFile2[255];
        char TwoPassBestOutFile2[255];
        char TwoPassOutFile3[255];
        char TwoPassBestOutFile3[255];

        snprintf(TwoPassOutFile1, 255, "%s", WorkingDir4.c_str());
        snprintf(TwoPassBestOutFile1, 255, "%s", WorkingDir5.c_str());
        snprintf(TwoPassOutFile2, 255, "%s", WorkingDir4b.c_str());
        snprintf(TwoPassBestOutFile2, 255, "%s", WorkingDir5b.c_str());
        snprintf(TwoPassOutFile3, 255, "%s", WorkingDir4c.c_str());
        snprintf(TwoPassBestOutFile3, 255, "%s", WorkingDir5c.c_str());

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

        printf("Two Pass Vs Two Pass Best Test");
        fprintf(stderr, "TwoPassVsTwoPassBest Test");

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

        int BitRate1 = BitRate - (BitRate * 0.3);
        int BitRate2 = BitRate;
        int BitRate3 = BitRate + (BitRate * 0.3);


        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {

            opt.Mode = MODE_SECONDPASS;
            opt.TargetBandwidth = BitRate1;

            if (CompressIVFtoIVF(input, TwoPassOutFile1, speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate2;

            if (CompressIVFtoIVF(input, TwoPassOutFile2, speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate3;

            if (CompressIVFtoIVF(input, TwoPassOutFile3, speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS_BEST;
            opt.TargetBandwidth = BitRate1;

            if (CompressIVFtoIVF(input, TwoPassBestOutFile1, speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate2;

            if (CompressIVFtoIVF(input, TwoPassBestOutFile2, speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.TargetBandwidth = BitRate3;

            if (CompressIVFtoIVF(input, TwoPassBestOutFile3, speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

        float GoodSize1 = IVFDataRate(TwoPassOutFile1, 1);
        float BestSize1 = IVFDataRate(TwoPassBestOutFile1, 1);
        float GoodSize2 = IVFDataRate(TwoPassOutFile2, 1);
        float BestSize2 = IVFDataRate(TwoPassBestOutFile2, 1);
        float GoodSize3 = IVFDataRate(TwoPassOutFile3, 1);
        float BestSize3 = IVFDataRate(TwoPassBestOutFile3, 1);

        //int PSNRToggle = PSNRSelect(input, TwoPassOutFile);

        double PSNRG1;
        double PSNRB1;
        double PSNRG2;
        double PSNRB2;
        double PSNRG3;
        double PSNRB3;

        PSNRG1 = IVFPSNR(input, TwoPassOutFile1, 1, 0, 1, NULL);
        PSNRB1 = IVFPSNR(input, TwoPassBestOutFile1, 1, 0, 1, NULL);
        PSNRG2 = IVFPSNR(input, TwoPassOutFile2, 1, 0, 1, NULL);
        PSNRB2 = IVFPSNR(input, TwoPassBestOutFile2, 1, 0, 1, NULL);
        PSNRG3 = IVFPSNR(input, TwoPassOutFile3, 1, 0, 1, NULL);
        PSNRB3 = IVFPSNR(input, TwoPassBestOutFile3, 1, 0, 1, NULL);

        float GoodA = 0;
        float GoodB = 0;
        float GoodC = 0;

        float BestA = 0;
        float BestB = 0;
        float BestC = 0;

        float minCommon = 0;
        float maxCommon = 0;

        if (GoodSize1 > BestSize1) //take area over same range we have decent data for.
        {
            minCommon = GoodSize1;
        }
        else
        {
            minCommon = BestSize1;
        }

        if (GoodSize3 > BestSize3)
        {
            maxCommon = BestSize3;
        }
        else
        {
            maxCommon = GoodSize3;
        }

        SolveQuadradic(GoodSize1, GoodSize2, GoodSize3, PSNRG1, PSNRG2, PSNRG3, GoodA, GoodB, GoodC);
        float GoodAreaVal = AreaUnderQuadradic(GoodA, GoodB, GoodC, minCommon, maxCommon);

        //cout << "\n\nGoodAreaVal: " << GoodAreaVal << "\n\n";

        SolveQuadradic(BestSize1, BestSize2, BestSize3, PSNRB1, PSNRB2, PSNRB3, BestA, BestB, BestC);
        float BestAreaVal = AreaUnderQuadradic(BestA, BestB, BestC, minCommon, maxCommon);

        //float PSRNPerc = absFloat((PSNRB - PSNRG) / PSNRG) * 100.00;
        // float BRPerc = absFloat((GoodSize - BestSize) / BestSize) * 100.00;
        printf("\n\n"
               "Data Points:\n"
               "\n"
               " Good Q\n"
               "\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "\n"
               " Best Q\n"
               "\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "(%.2f,%2.2f)\n"
               "\n"
               "\n"
               , GoodSize1, PSNRG1
               , GoodSize2, PSNRG2
               , GoodSize3, PSNRG3
               , BestSize1, PSNRB1
               , BestSize2, PSNRB2
               , BestSize3, PSNRB3
              );

        fprintf(stderr, "\n\n"
                "Data Points:\n"
                "\n"
                " Two Pass\n"
                "\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "\n"
                " Two Pass Best\n"
                "\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "(%.2f,%2.2f)\n"
                "\n"
                "\n"
                , GoodSize1, PSNRG1
                , GoodSize2, PSNRG2
                , GoodSize3, PSNRG3
                , BestSize1, PSNRB1
                , BestSize2, PSNRB2
                , BestSize3, PSNRB3
               );

        printf("Two Pass Curve: y = %fx^2 + %fx + %f\n", GoodA, GoodB, GoodC);
        printf("Two Pass Best Curve: y = %fx^2 + %fx + %f\n", BestA, BestB, BestC);

        fprintf(stderr, "Two Pass Curve: y = %fx^2 + %fx + %f\n", GoodA, GoodB, GoodC);
        fprintf(stderr, "Two Pass Best Curve: y = %fx^2 + %fx + %f\n", BestA, BestB, BestC);

        printf("\nTwo Pass area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, GoodAreaVal);
        printf("Two Pass Best area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, BestAreaVal);

        fprintf(stderr, "\nTwo Pass area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, GoodAreaVal);
        fprintf(stderr, "Two Pass Best area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, BestAreaVal);

        int Pass = 0;

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (GoodAreaVal == BestAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Two Pass Best area under curve: %.2f == Two Pass area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (BestAreaVal > GoodAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Two Pass Best area under curve: %.2f > Two Pass area under curve: %.2f - Passed", BestAreaVal, GoodAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            Pass = 1;
        }

        if (BestAreaVal < GoodAreaVal)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Two Pass Best area under curve: %.2f < Two Pass area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (Pass == 1)
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }

    int UnderShoot(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

        printf("Undershoot Test");
        fprintf(stderr, "Undershoot Test");

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
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.UnderShootPct = 10;

                if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.UnderShootPct = 100;

                if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100, 0) == -1)
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

                if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.UnderShootPct = 100;

                if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100, 0) == -1)
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

                if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.UnderShootPct = 100;

                if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100, 0) == -1)
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

                if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.UnderShootPct = 100;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100, 0) == -1)
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

                if (CompressIVFtoIVF(input, UnderShoot10, speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.UnderShootPct = 100;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, UnderShoot100, speed, BitRate, opt, CompressString, 100, 0) == -1)
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

        printf("\n");
        fprintf(stderr, "\n");
        long File1bytes = FileSize(UnderShoot10);
        printf("\n");
        fprintf(stderr, "\n");
        long File2bytes = FileSize(UnderShoot100);
        printf("\n");
        fprintf(stderr, "\n");

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (File1bytes < File2bytes)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "File size 1:%i < File size 2:%i - Passed", File1bytes , File2bytes);;
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            //printf("\n File size 1:%i < File size 2:%i : Passed \n", File1bytes , File2bytes);
            //fprintf(stderr, "\n File size 1:%i < File size 2:%i : Passed \n", File1bytes , File2bytes);

            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }

        if (File1bytes == File2bytes)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "No effect try different file - Indeterminate");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            //printf("No effect try different file \n");
            //fprintf(stderr, "No effect try different file \n");

            printf("\nIndeterminate\n");
            fprintf(stderr, "\nIndeterminate\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "File size 1:%i > File size 2:%i - Failed", File1bytes , File2bytes);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");

            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            //printf("\n File size 1:%i > File size 2:%i : Failed\n", File1bytes , File2bytes);
            //fprintf(stderr, "\n File size 1:%i > File size 2:%i : Failed \n", File1bytes , File2bytes);

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
    }

    int Version(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

        printf("Version Test");
        fprintf(stderr, "Version Test");

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

        float PSNRArr[4];
        unsigned int DecTime[4];

        //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
        //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
        //Test Type 3 = Mode 2 =Run Test Compressions

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            DecTime[0] = TimeReturn(Version0_Dec, 1);
            DecTime[1] = TimeReturn(Version1_Dec, 1);
            DecTime[2] = TimeReturn(Version2_Dec, 1);
            DecTime[3] = TimeReturn(Version3_Dec, 1);
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.Version = 0;

                if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.Version = 1;

                if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.Version = 2;

                if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.Version = 3;

                if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.Version = 1;

                if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.Version = 2;

                if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.Version = 3;

                if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.Version = 1;

                if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.Version = 2;

                if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.Version = 3;

                if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 1;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 2;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 3;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0, speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 1;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Version1, speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 2;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Version2, speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 3;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Version3, speed, BitRate, opt, CompressString, 3, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

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

        }

        //Create Compression only stop test short.
        if (TestType == 2)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 10;
        }

        int PSNRToggle = PSNRSelect(input, Version0);

        PSNRArr[0] = IVFPSNR(input, Version0, PSNRToggle, 0, 1, NULL);
        PSNRArr[1] = IVFPSNR(input, Version1, PSNRToggle, 0, 1, NULL);
        PSNRArr[2] = IVFPSNR(input, Version2, PSNRToggle, 0, 1, NULL);
        PSNRArr[3] = IVFPSNR(input, Version3, PSNRToggle, 0, 1, NULL);

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

        int fail = 0;
        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        if (PSNRFail == 0)// && TIMEFail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All PSNRs decrease as version numbers increase - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (PSNRFail < 2 && PSNRFail != 0)// && TIMEFail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All but one PSNR Decreases as version numbers increase - Min Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 2;
        }

        if (PSNRFail >= 2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all PSNRs decrease as version numbers increase - Failed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        if (TIMEFail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All decode times decrease as version numbers increase - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (TIMEFail < 2 && TIMEFail != 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All but one decode times decrease as version numbers increase - Min Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 2;
        }

        if (TIMEFail >= 2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all decode times increase as version numbers increase - Failed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        if (fail == 2)
        {
            printf("\nIndeterminate\n");
            fprintf(stderr, "\nIndeterminate\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 8;
        }

        if (fail == 1)
        {
            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }
        else
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }
    int WindowsMatchesLinux(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
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

                if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode, 0) == -1)
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

                if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode, 0) == -1)
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

                if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode, 0) == -1)
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

                if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode, 0) == -1)
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

                if (CompressIVFtoIVF(input, EncOutput, speed, BitRate, opt, "Mode", Mode, 0) == -1)
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
    //----------------------------------------Code Coverage----------------------------------------------------------------
    int CodeCoverage(int argc, char * argv[], string WorkingDir, string FilesAr[])
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
                CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
            }

            if (Mode == 1)
            {
                opt.Mode = MODE_GOODQUALITY;
                CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
            }

            if (Mode == 2)
            {
                opt.Mode = MODE_BESTQUALITY;

                CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
            }

            if (Mode == 3)
            {
            }

            if (Mode == 4)
            {
                opt.Mode = MODE_SECONDPASS;
                CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
            }

            if (Mode == 5)
            {
                opt.Mode = MODE_SECONDPASS_BEST;
                CompressIVFtoIVF(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
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

