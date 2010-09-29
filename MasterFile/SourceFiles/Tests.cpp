#define _CRT_SECURE_NO_WARNINGS
#include "test-definitions.h"
#include "onyx.h"
#include "onyxd.h"
#include "yv12config.h"
#include "vpx_mem.h"
#include "ivf.h"
#include "CompIVF.h"
#include <map>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

#if defined(_WIN32)
#include "on2vpplugin.h"
#include <windows.h>
#include <tchar.h>
#include <direct.h>
#define snprintf _snprintf
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

extern "C"
{
#include "vpx_timer.h"
    extern vpx_codec_iface_t vpx_codec_vp8_cx_algo;
}

//driver
extern string DateString();
extern void FormatedPrint(string SummaryStr, int selector);

//Utilities
extern void VP8DefaultParms(VP8_CONFIG &opt);
extern VP8_CONFIG VP8RandomParms(VP8_CONFIG &opt, char *inputfile, int display);
extern VP8_CONFIG InPutSettings(char *inputFile);
extern int OutPutSettings(const char *outputFile, VP8_CONFIG opt);
extern int OutPutCompatSettings(const char *outputFile, VP8_CONFIG opt, int ParVersionNum);

extern long FileSize(const char *inFile);
extern long FileSize2(const char *inFile);
extern void FileName(const char *input, char *FileName, int removeExt);
extern void FolderName(const char *input, char *output);
extern void FolderName2(const char *DirIn, char *DirOut);
extern string ExtractDateTime(string InputStr);
extern int TimeStampCompare(string TimeStampNow, string TimeStampPrevious);
extern int Test0InputTextCheck(char *input, int MoreInfo);
extern int FileExistsCheck(string input);
extern void SubFolderName(char *input, char *FileName);
extern void TestName(char *input, char *TestName);

extern int absInt(int input);
extern float absFloat(float input);
extern double absDouble(double input);
extern int SolveQuadradic(float X1, float X2, float X3, float Y1, float Y2, float Y3, float &A, float &B, float &C);
extern float AreaUnderQuadradic(float A, float B, float C, float X1, float X2);
extern char *itoa_custom(int value, char *result, int base);

extern unsigned int GetTimeInMicroSec(unsigned int startTick, unsigned int stopTick);
extern unsigned int VPX_GetProcCoreCount();
extern unsigned int GetTime();
extern int MakeDirVPX(string CreateDir2);
extern void RunExe(string RunExe);

extern double IVFPSNR(const char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, double *SsimOut);
extern double PostProcIVFPSNR(char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double *SsimOut);
extern double IVFDataRate(const char *inputFile, int DROuputSel);
extern int IVFCheckPBM(const char *inputFile, int bitRate, int maxBuffer, int preBuffer);
extern int IVFCheckPBMThreshold(const char *inputFile, double bitRate, int maxBuffer, int preBuffer, int optimalbuffer, int Threshold);

extern int CompressIVFtoIVF(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt, int RunQCheck);
extern unsigned int TimeCompressIVFtoIVF(char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt, int RunQCheck);
extern int CompressIVFtoIVFForceKeyFrame(char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, int forceKeyFrame);
extern int CompressIVFtoIVFReconBufferCheck(char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck);
extern int DecompressIVFtoIVF(const char *inputFile, const char *outputFile2);
extern int DecompressIVFtoRaw(const char *inputFile, const char *outputFile2);
extern unsigned int TimeDecompressIVFtoIVF(const char *inputFile, const char *outputFile2);
extern unsigned int DecompressIVFtoIVFTimeAndOutput(const char *inputFile, const char *outputFile2);
extern int DecComputeMD5(const char *inputchar, const char *outputchar);

extern int CropRawIVF(char *inputFile, const char *outputFile, int xoffset, int yoffset, int newFrameWidth, int newFrameHeight, int FileIsIVF, int OutputToFile);
extern int CompIVF(const char *inputFile1, const char *inputFile2);
extern double IVFDisplayResizedFrames(const char *inputchar, int PrintSwitch);
extern double IVFDisplayVisibleFrames(const char *inputFile, int Selector);
extern double IVFDisplayAltRefFrames(const char *inputFile, int Selector);
extern double IVFDisplayKeyFrames(const char *inputFile, int Selector);
extern int IVFLagInFramesCheck(const char *QuantInChar);
extern int IVFDFWMCheck(const char *InputFile, int printselect);
extern int CheckMinQ(char *inputFile, int MinQuantizer);
extern int CheckMaxQ(const char *inputFile, int MaxQuantizer);
extern int CheckFixedQ(const char *inputFile, int FixedQuantizer);
extern int TimeReturn(const char *infile, int FileType);
extern int GetNumberofFrames(const char *inputFile);

/////Tests
extern int ExternalTestRunner(int argc, char *argv[], string WorkingDir,  int NumberofTests);
extern int RandComp(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int GraphPSNR(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AllowDF(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AllowLagTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AllowSpatialResamplingTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AutoKeyFramingWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int BufferLevelWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int CPUDecOnlyWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ChangeCPUWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int DFWM(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int DataRateTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int DebugMatchesRelease(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int EncoderBreakOut(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ErrorRes(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ExtraFileCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int FixedQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ForceKeyFrameWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int FrameSizeTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int GoodQvBestQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int LagInFramesTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MaxQTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MemLeakCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MemLeakCheck2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MinQTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MultiThreadedTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int NewVsOldPSNR(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int NewVsOldRealTimeSpeed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int NoiseSensitivityWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int OnePassVsTwoPass(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int PlayAlternate(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int PostProcessorWorks(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ReconBuffer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int ResampleDownWaterMark(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int SpeedTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int TestVectorCheck(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int TwoPassVsTwoPassBest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int UnderShoot(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int Version(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int WindowsMatchesLinux(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);

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
    INT16         MinVal;
    INT16         Length;
    UINT8 Probs[12];
} TOKENEXTRABITS;

extern "C" TOKENEXTRABITS vp8d_token_extra_bits2[12];

//extern "C" struct
//{
//  int row;
//  int col;
//  int weight;  /*  was 5 - (abs(nearB[i].row) + abs(nearB[i].col)) */
//  int block;
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
    printf("\n%s\n", vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
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
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
    TestMachineInfo.append("\n                     Test Machine is Running: Windows\n\n");
#endif
#if defined(linux)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Linux\n\n");
#endif
#if defined(__APPLE__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Mac\n\n");
#endif
#if defined(__POWERPC__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

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
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                      Test Machine is Running: Windows\n\n");
#endif
#if defined(linux)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Linux\n\n");
#endif
#if defined(__APPLE__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                        Test Machine is Running: Mac\n\n");
#endif
#if defined(__POWERPC__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

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
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                     Test Machine is Running: Windows\n\n");
#endif
#if defined(linux)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                       Test Machine is Running: Linux\n\n");
#endif
#if defined(__APPLE__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    TestMachineInfo.append("\n                        Test Machine is Running: Mac\n\n");
#endif
#if defined(__POWERPC__)
    TestMachineInfo = "                      ";
    TestMachineInfo.append(vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

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

    unsigned long RunTimeRecAr[999];
    /////////////////////////////Record Time it took to run test/////////////////////////////
    int RecordRunTimes = 0; //If set to one will record run times of tests
    unsigned int RunTime1 = 0;
    unsigned int RunTime2 = 0;

    /////////////////////////////////////////////////////////////////////////////////////////

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

        WorkingDir = argv[3];                                           //location of main dir in as workingdir

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr);
        WorkingTextFilestr.append("TestsRun.txt");                      //TestsRun.txt original file ided

        //Determines which test is in the process of being run and sets the correct text file as input file
        SummCompAndTest = WorkingDir;
        SummCompAndTest.append(slashCharStr);
        SummCompAndTest.append("Mode1Results.txt");     // Mode 1
        //SummCompAndTest.append("Summary_CompressionsANDTests.txt");       // Mode 1
        SummComp = WorkingDir;
        SummComp.append(slashCharStr);
        SummComp.append("Mode2Results.txt");                // Mode 2
        //SummComp.append("Summary_CompressionsOnly.txt");              // Mode 2
        SummTest = WorkingDir;
        SummTest.append(slashCharStr);
        SummTest.append("Mode3Results.txt");                        // Mode 3
        //SummTest.append("Summary_TestsOnly.txt");                     // Mode 3

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


        WorkingDir = argv[3];                               //location of main dir in as workingdir

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr);
        WorkingTextFilestr.append("TestsRun.txt");          //TestsRun.txt original file ided

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

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = AllowDF(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ALWLGNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowLagTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = AllowLagTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ALWSRNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowSpatialResampling";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = AllowSpatialResamplingTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == AUTKFNUM)
                {
                    SelectorAr[SelectorArInt] = "AutoKeyFramingWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = AutoKeyFramingWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == BUFLVNUM)
                {
                    SelectorAr[SelectorArInt] = "BufferLevelWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = BufferLevelWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == CPUDENUM)
                {
                    SelectorAr[SelectorArInt] = "CPUDecOnlyWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = CPUDecOnlyWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == CHGWRNUM)
                {
                    SelectorAr[SelectorArInt] = "ChangeCPUWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = ChangeCPUWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == DFWMWNUM)
                {
                    SelectorAr[SelectorArInt] = "DropFramesWaterMarkWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = DFWM(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == DTARTNUM)
                {
                    SelectorAr[SelectorArInt] = "DataRateTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = DataRateTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == DBMRLNUM)
                {
                    SelectorAr[SelectorArInt] = "DebugMatchesRelease";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = DebugMatchesRelease(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ENCBONUM)
                {
                    SelectorAr[SelectorArInt] = "EncoderBreakOut";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = EncoderBreakOut(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ERRMWNUM)
                {
                    SelectorAr[SelectorArInt] = "ErrorResilientModeWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = ErrorRes(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == EXTFINUM)
                {
                    SelectorAr[SelectorArInt] = "ExtraFileCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = ExtraFileCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == FIXDQNUM)
                {
                    SelectorAr[SelectorArInt] = "FixedQ";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = FixedQ(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == FKEFRNUM)
                {
                    SelectorAr[SelectorArInt] = "ForceKeyFrameWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = ForceKeyFrameWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == FRSZTNUM)
                {
                    SelectorAr[SelectorArInt] = "FrameSizeTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = FrameSizeTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == GQVBQNUM)
                {
                    SelectorAr[SelectorArInt] = "GoodQualityVsBestQuality";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = GoodQvBestQ(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == LGIFRNUM)
                {
                    SelectorAr[SelectorArInt] = "LagInFramesTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = LagInFramesTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MAXQUNUM)
                {
                    SelectorAr[SelectorArInt] = "MaxQuantizerTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = MaxQTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MEML1NUM)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = MemLeakCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MEML2NUM)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck2";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = MemLeakCheck2(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MINQUNUM)
                {
                    SelectorAr[SelectorArInt] = "MinQuantizerTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = MinQTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MULTTNUM)
                {
                    SelectorAr[SelectorArInt] = "MultiThreadedTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = MultiThreadedTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == NVOPSNUM)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldPSNR";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = NewVsOldPSNR(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == NVORTNUM)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldRealTimeSpeed";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = NewVsOldRealTimeSpeed(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == NOISENUM)
                {
                    SelectorAr[SelectorArInt] = "NoiseSensitivityWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = NoiseSensitivityWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == OV2PSNUM)
                {
                    SelectorAr[SelectorArInt] = "OnePassVsTwoPass";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = OnePassVsTwoPass(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == PLYALNUM)
                {
                    SelectorAr[SelectorArInt] = "PlayAlternate";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = PlayAlternate(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == POSTPNUM)
                {
                    SelectorAr[SelectorArInt] = "PostProcessorWorks";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = PostProcessorWorks(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == RECONBUF)
                {
                    SelectorAr[SelectorArInt] = "ReconBuffer";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = ReconBuffer(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == RSDWMNUM)
                {
                    SelectorAr[SelectorArInt] = "ResampleDownWaterMark";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = ResampleDownWaterMark(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == SPEEDNUM)
                {
                    SelectorAr[SelectorArInt] = "SpeedTest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = SpeedTest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == TVECTNUM)
                {
                    SelectorAr[SelectorArInt] = "TestVectorCheck";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = TestVectorCheck(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == TV2BTNUM)
                {
                    SelectorAr[SelectorArInt] = "TwoPassVsTwoPassBest";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = TwoPassVsTwoPassBest(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == UNDSHNUM)
                {
                    SelectorAr[SelectorArInt] = "UnderShoot";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = UnderShoot(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == VERSINUM)
                {
                    SelectorAr[SelectorArInt] = "Version";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = Version(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == WMLMMNUM)
                {
                    SelectorAr[SelectorArInt] = "WindowsMatchesLinux";
                    CheckTimeStamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = GetTime();
                    }

                    PassFail[PassFailInt] = WindowsMatchesLinux(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = GetTime();
                        RunTimeRecAr[SelectorArInt] = GetTimeInMicroSec(RunTime1, RunTime2);
                    }

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

        /////////////////////////////Output Time it took to run test/////////////////////////////
        if (RecordRunTimes == 1)
        {
            string TimeoutputStr = WorkDirFileStr;
            TimeoutputStr.erase(TimeoutputStr.length() - 4, 4);
            TimeoutputStr.append("_Times.txt");

            FILE *outputTime;
            outputTime = fopen(TimeoutputStr.c_str() , "w");

            printf("\n--------------------------------------------------------------------------------\n"
                   "                       Time to Run Tests\n");

            fprintf(outputTime, "\n--------------------------------------------------------------------------------\n"
                    "                      Time to Run Tests\n");

            printf("\n\n%4s %-28s%s\n\n", "", " Test Name", "            Time");
            fprintf(outputTime, "\n\n%4s %-28s%s\n\n", "", " Test Name", "            Time");

            y = 0;

            while (y < TestsRun)
            {
                printf("   %s", SelectorAr[y].c_str());
                fprintf(outputTime, "   %s", SelectorAr[y].c_str());
                int z = SelectorAr[y].size();

                while (z <= 25)
                {
                    printf(" ");
                    fprintf(outputTime, " ");
                    z++;
                }

                printf("%20u ms\n", RunTimeRecAr[y]);
                fprintf(outputTime, "%20u ms\n", RunTimeRecAr[y]);
                y++;
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////


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
    FileName(input, FileNameChar, 0);
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

    opt.target_bandwidth = FirstBitRate;

    ///////////output Par file////////////////////
    ParFileOut.append("_ParameterFile.txt");
    char ParFileOutChar[255];
    snprintf(ParFileOutChar, 255, "%s", ParFileOut.c_str());
    OutPutSettings(ParFileOutChar, opt);
    ///////////////////////////////////////////////

    int x = 0;
    int DoONce = 0;

    while (opt.target_bandwidth <= LastBitRate)
    {
        if (opt.target_bandwidth == LastBitRate && DoONce == 0)
        {
            DoONce = 1;
        }

        string OutPutStr2 = OutPutStr;
        char TBChar[8];
        itoa_custom(opt.target_bandwidth, TBChar, 10);
        OutPutStr2.append(TBChar);
        string OutPutStr3 = OutPutStr2;
        OutPutStr3.append("_Dec");
        OutPutStr2.append(".ivf");
        OutPutStr3.append(".ivf");

        char outputChar[255];
        snprintf(outputChar, 255, "%s", OutPutStr2.c_str());

        char outputChar2[255];
        snprintf(outputChar2, 255, "%s", OutPutStr3.c_str());

        EncTimeArr[x] = TimeCompressIVFtoIVF(input, outputChar , speed, opt.target_bandwidth, opt, CompressString, 0, 0);

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



        opt.target_bandwidth = opt.target_bandwidth + BitRateStep;

        if (opt.target_bandwidth > LastBitRate && DoONce == 0)
        {
            opt.target_bandwidth = LastBitRate;
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
    FileName(input, InputFileName, 0);
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
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;

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
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;

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
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;

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
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;
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

    string AllowDFon = WorkingDirString;
    string AllowDFoff = WorkingDirString;

    AllowDFon.append(slashCharStr);
    AllowDFon.append("AllowDFOnOutput.ivf");
    AllowDFoff.append(slashCharStr);
    AllowDFoff.append("AllowDFOffOutput.ivf");

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;

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
            opt.allow_df = 0;

            if (CompressIVFtoIVF(input, AllowDFoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.allow_df = 1;

            if (CompressIVFtoIVF(input, AllowDFon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.allow_df = 0;

            if (CompressIVFtoIVF(input, AllowDFoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.allow_df = 1;

            if (CompressIVFtoIVF(input, AllowDFon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.allow_df = 0;

            if (CompressIVFtoIVF(input, AllowDFoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.allow_df = 1;

            if (CompressIVFtoIVF(input, AllowDFon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 4)
        {
            opt.allow_df = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowDFoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.allow_df = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowDFon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.allow_df = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowDFoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.allow_df = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowDFon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    int AllowDFonFrames = IVFDisplayVisibleFrames(AllowDFon.c_str(), 1);
    int AllowDFoffFrames = IVFDisplayVisibleFrames(AllowDFoff.c_str(), 1);

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

    string AllowLagon = WorkingDirString;
    string AllowLagoff = WorkingDirString;

    AllowLagon.append(slashCharStr);
    AllowLagon.append("AllowLagOnOutput.ivf");
    AllowLagoff.append(slashCharStr);
    AllowLagoff.append("AllowLagOffOutput.ivf");

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.lag_in_frames = 10;

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
            opt.allow_lag = 0;

            if (CompressIVFtoIVF(input, AllowLagoff.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.allow_lag = 1;

            if (CompressIVFtoIVF(input, AllowLagon.c_str(), speed, BitRate, opt, CompressString, 1, 1) == -1)
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
            opt.allow_lag = 0;

            if (CompressIVFtoIVF(input, AllowLagoff.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.allow_lag = 1;

            if (CompressIVFtoIVF(input, AllowLagon.c_str(), speed, BitRate, opt, CompressString, 1, 1) == -1)
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
            opt.allow_lag = 0;

            if (CompressIVFtoIVF(input, AllowLagoff.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.allow_lag = 1;

            if (CompressIVFtoIVF(input, AllowLagon.c_str(), speed, BitRate, opt, CompressString, 1, 1) == -1)
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
            opt.allow_lag = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowLagoff.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.allow_lag = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AllowLagon.c_str(), speed, BitRate, opt, CompressString, 1, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.allow_lag = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowLagoff.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.allow_lag = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AllowLagon.c_str(), speed, BitRate, opt, CompressString, 1, 1) == -1)
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

    int AllowLagONAltRefCount = IVFDisplayAltRefFrames(AllowLagon.c_str(), 1);
    int AllowLagOFFAltRefCount = IVFDisplayAltRefFrames(AllowLagoff.c_str(), 1);
    int VisibleFrameONCount = IVFDisplayVisibleFrames(AllowLagon.c_str(), 1);
    int VisibleFrameOFFCount = IVFDisplayVisibleFrames(AllowLagoff.c_str(), 1);

    char AllowLagonFilename[255];
    FileName(AllowLagon.c_str(), AllowLagonFilename, 0);
    char AllowLagoffFilename[255];
    FileName(AllowLagoff.c_str(), AllowLagoffFilename, 0);

    int lngRC = CompIVF(AllowLagoff.c_str(), AllowLagon.c_str());

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    int fail = 0;

    if (LagInFramesFound == opt.lag_in_frames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Correct number of LagInFrames (%i) detected in compression for %s - Passed", opt.lag_in_frames, AllowLagonFilename);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Correct number of LagInFrames (%i) not detected in compression for %s - Failed", opt.lag_in_frames, AllowLagonFilename);
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

    string Spatialon = WorkingDirString;
    string Spatialoff = WorkingDirString;

    Spatialon.append(slashCharStr);
    Spatialon.append("SpatialOnOutput.ivf");
    Spatialoff.append(slashCharStr);
    Spatialoff.append("SpatialOffOutput.ivf");

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.end_usage = 0;
    opt.resample_down_water_mark = 60;
    opt.resample_up_water_mark = 80;

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
            opt.allow_spatial_resampling = 0;

            if (CompressIVFtoIVF(input, Spatialoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.allow_spatial_resampling = 1;

            if (CompressIVFtoIVF(input, Spatialon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.allow_spatial_resampling = 0;

            if (CompressIVFtoIVF(input, Spatialoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.allow_spatial_resampling = 1;

            if (CompressIVFtoIVF(input, Spatialon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.allow_spatial_resampling = 0;

            if (CompressIVFtoIVF(input, Spatialoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.allow_spatial_resampling = 1;

            if (CompressIVFtoIVF(input, Spatialon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.allow_spatial_resampling = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Spatialoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.allow_spatial_resampling = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, Spatialon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.allow_spatial_resampling = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Spatialoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.allow_spatial_resampling = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, Spatialon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    double SpatialResampPSNR = IVFPSNR(input, Spatialon.c_str(), 0, 0, 1, NULL);

    char SpatialonFileName[255];
    FileName(Spatialon.c_str(), SpatialonFileName, 0);
    char SpatialoffFileName[255];
    FileName(Spatialoff.c_str(), SpatialoffFileName, 0);

    printf("\nChecking: %s for resized frames\n", SpatialonFileName);
    fprintf(stderr, "\nChecking: %s for resized frames\n", SpatialonFileName);
    int AllowSpatResampleONFramesResized = IVFDisplayResizedFrames(Spatialon.c_str(), 1);

    printf("Checking: %s for resized frames\n", SpatialoffFileName);
    fprintf(stderr, "Checking: %s for resized frames\n", SpatialoffFileName);
    int AllowSpatResampleOFFFramesResized = IVFDisplayResizedFrames(Spatialoff.c_str(), 1);

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
    char *CompressString = "opt.auto_keyFrame";

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


    string KeyFrameTxtOut1 = WorkingDirString;
    string KeyFrameTxtOut2 = WorkingDirString;
    string AutoKeyFramingWorks1 = WorkingDirString;
    string AutoKeyFramingWorks2 = WorkingDirString;

    KeyFrameTxtOut1.append(slashCharStr);
    KeyFrameTxtOut1.append("AutoKeyFramingWorksOutput1_KeyFrames.txt");
    AutoKeyFramingWorks1.append(slashCharStr);
    AutoKeyFramingWorks1.append("AutoKeyFramingWorksOutput1.ivf");
    KeyFrameTxtOut2.append(slashCharStr);
    KeyFrameTxtOut2.append("AutoKeyFramingWorksOutput2_KeyFrames.txt");
    AutoKeyFramingWorks2.append(slashCharStr);
    AutoKeyFramingWorks2.append("AutoKeyFramingWorksOutput2.ivf");


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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.auto_key = 1;
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks1.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, AutoKeyFramingWorks2.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

    IVFDisplayKeyFrames(AutoKeyFramingWorks1.c_str(), 1);
    IVFDisplayKeyFrames(AutoKeyFramingWorks2.c_str(), 1);

    ifstream infile1(KeyFrameTxtOut1.c_str());
    ifstream infile2(KeyFrameTxtOut2.c_str());

    if (!infile1.good())
    {
        printf("\nKey Frame File 1 Not Found: %s\n", KeyFrameTxtOut1.c_str());
        fprintf(stderr, "\nKey Frame File 1 Not Found: %s\n", KeyFrameTxtOut1.c_str());
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (!infile2.good())
    {
        printf("\nKey Frame File 2 Not Found: %s\n", KeyFrameTxtOut2.c_str());
        fprintf(stderr, "\nKey Frame File 2 Not Found: %s\n", KeyFrameTxtOut2.c_str());
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
    FileName(AutoKeyFramingWorks1.c_str(), AutoKeyFramingWorks1FileName, 0);
    char AutoKeyFramingWorks2FileName[255];
    FileName(AutoKeyFramingWorks2.c_str(), AutoKeyFramingWorks2FileName, 0);

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    //////////////////////////////////////////////////////////////////////////////////
    int fail2 = 0;
    ifstream infile(KeyFrameTxtOut1.c_str());

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

    int NumberofFrames = GetNumberofFrames(AutoKeyFramingWorks1.c_str());

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

        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
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


    string BufferLevelWorksOut = WorkingDirString;
    //string WorkingDir5 = WorkingDirString;

    BufferLevelWorksOut.append(slashCharStr);
    BufferLevelWorksOut.append("BufferLevelWorksOutput.ivf");

    //char BufferLevelWorksOut[255];
    //snprintf(BufferLevelWorksOut, 255, "%s", WorkingDir4.c_str());

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    opt.allow_df = 1;
    int CompressInt = opt.allow_df;

    opt.target_bandwidth = BitRate;
    opt.starting_buffer_level = StartingBufferLvl;
    opt.maximum_buffer_size = MaximumBufferLevel;

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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

    int PassFail = IVFCheckPBM(BufferLevelWorksOut.c_str(), BitRate, MaximumBufferLevel * 1000, StartingBufferLvl * 1000);

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

    string CPUDecOnlyWorksOutFile = WorkingDirString;
    string CPUDecOnlyWorksOut_CPU = WorkingDirString;

    CPUDecOnlyWorksOutFile.append(slashCharStr);
    CPUDecOnlyWorksOutFile.append("CPUDecOnlyWorksOutput.ivf");
    CPUDecOnlyWorksOut_CPU.append(slashCharStr);
    CPUDecOnlyWorksOut_CPU.append("CPUDecOnlyWorksOutput_CPU");

    //char CPUDecOnlyWorksOutFile[255];
    //char CPUDecOnlyWorksOut_CPU[255];

    //snprintf(CPUDecOnlyWorksOutFile, 255, "%s", WorkingDir4.c_str());
    //snprintf(CPUDecOnlyWorksOut_CPU, 255, "%s", WorkingDir5.c_str());

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.Mode = MODE_GOODQUALITY;
    opt.Version = VersionNum;
    int CompressInt = opt.Version;

    int counter = 0;
    int Mode2 = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        putenv("ON2_SIMD_CAPS=0");

        string Output2Str = CPUDecOnlyWorksOut_CPU;
        Output2Str.append("0.ivf");

        printf("\n\nCPU:%i\n", 0);
        fprintf(stderr, "\n\nCPU:%i\n", 0);

        totalms = DecompressIVFtoIVFTimeAndOutput(CPUDecOnlyWorksOutFile.c_str(), Output2Str.c_str());

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

            char CPUChar[255];
            snprintf(CPUChar, 255, CPUIDSTRING.c_str());
            putenv(CPUChar);

            printf("CPU:%i", counter);
            fprintf(stderr, "CPU:%i", counter);

            //////////////////////////////////
            ///////Compresion and Time ///////

            string ChangedCPUDecOutFileStr1 = CPUDecOnlyWorksOut_CPU;
            string ChangedCPUDecOutFileStr2 = CPUDecOnlyWorksOut_CPU;

            char count[20];
            itoa_custom(counter, count, 10);
            ChangedCPUDecOutFileStr1.append(count);
            ChangedCPUDecOutFileStr1.append(".ivf");

            printf("\n");
            fprintf(stderr, "\n");

            totalms2 = DecompressIVFtoIVFTimeAndOutput(CPUDecOnlyWorksOutFile.c_str(), ChangedCPUDecOutFileStr1.c_str());

            int countOld = (counter - 1);
            itoa_custom(countOld, count, 10);
            ChangedCPUDecOutFileStr2.append(count);
            ChangedCPUDecOutFileStr2.append(".ivf");

            if (TestType != 2)
            {
                printf("comparing CPU:%i to CPU:%i", counter - 1, counter);
                fprintf(stderr, "\ncomparing CPU:%i to CPU:%i", counter - 1, counter);

                int lngRC = CompIVF(ChangedCPUDecOutFileStr1.c_str(), ChangedCPUDecOutFileStr2.c_str());

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

    string ChangedCPUDec0OutFile = WorkingDirString;
    string ChangedCPUDecNOutBase = WorkingDirString;

    ChangedCPUDec0OutFile.append(slashCharStr);
    ChangedCPUDec0OutFile.append("ChangedCPUNOutputCPU0.ivf");
    ChangedCPUDecNOutBase.append(slashCharStr);
    ChangedCPUDecNOutBase.append("ChangedCPUNOutputCPU");

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

    //unsigned int totalms1 = 0;
    //unsigned int totalms2 = 0;
    //unsigned int totalms3 = 0;

    //unsigned int totalms1b = 0;
    //unsigned int totalms2b = 0;
    //unsigned int totalms3b = 0;

    int speed = 0;
    int Fail = 0;

    unsigned int Time1 = 0;
    unsigned int Time2 = 0;

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////


    opt.target_bandwidth = BitRate;
    opt.Version = VersionNum;
    int CompressInt = opt.Version;
    opt.Mode = MODE_GOODQUALITY;
    int counter = 0;

    putenv("ON2_SIMD_CAPS=0");

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        int counterMax = 16;
        counter = 1;
        int FileNumber = 1;

        while (counter < counterMax)
        {
            ///////////Updating CPU///////////
            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);

            char CPUChar[255];
            snprintf(CPUChar, 255, CPUIDSTRING.c_str());
            putenv(CPUChar);

            ///////Compresion and Time ///////
            string ChangedCPUDecNOutCurrent = ChangedCPUDecNOutBase;
            string ChangedCPUDecNOutLast = ChangedCPUDecNOutBase;

            char count[20];
            itoa_custom(counter, count, 10);
            ChangedCPUDecNOutCurrent.append(count);
            ChangedCPUDecNOutCurrent.append(".ivf");

            int countOld = (counter - 1) / 2;
            itoa_custom(countOld, count, 10);
            ChangedCPUDecNOutLast.append(count);
            ChangedCPUDecNOutLast.append(".ivf");

            printf("\n\ncomparing\n\n %s \n\n to \n\n%s\n\n", ChangedCPUDecNOutCurrent.c_str(), ChangedCPUDecNOutLast.c_str());
            fprintf(stderr, "\n\ncomparing\n\n %s \n\n to \n\n%s\n\n", ChangedCPUDecNOutCurrent.c_str(), ChangedCPUDecNOutLast.c_str());

            int lngRC = CompIVF(ChangedCPUDecNOutCurrent.c_str(), ChangedCPUDecNOutLast.c_str());

            if (lngRC >= 0)
            {
                printf("\n\nFail: Files differ at frame: %i on file number %i", lngRC, FileNumber);
                fprintf(stderr, "\n\nFail: Files differ at frame: %i on file number %i", lngRC, FileNumber);
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

            Time2 = TimeReturn(ChangedCPUDecNOutCurrent.c_str(), 0);

            counter = (counter * 2) + 1;
            FileNumber++;
        }
    }
    else
    {

        int counterMax = 16;
        int FileNumber = 1;

        while (counter < counterMax)
        {
            ///////////Updating CPU///////////
            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);

            char CPUChar[255];
            snprintf(CPUChar, 255, CPUIDSTRING.c_str());
            putenv(CPUChar);

            printf("\n\nCPU:%i", counter);
            fprintf(stderr, "\n\nCPU:%i", counter);

            ///////Compresion and Time ///////
            string ChangedCPUDecNOutCurrent = ChangedCPUDecNOutBase;
            string ChangedCPUDecNOutLast = ChangedCPUDecNOutBase;

            char count[20];
            itoa_custom(counter, count, 10);
            ChangedCPUDecNOutCurrent.append(count);
            ChangedCPUDecNOutCurrent.append(".ivf");

            int countOld = (counter - 1) / 2;
            itoa_custom(countOld, count, 10);
            ChangedCPUDecNOutLast.append(count);
            ChangedCPUDecNOutLast.append(".ivf");

            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutCurrent.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutCurrent.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutCurrent.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutCurrent.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutCurrent.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

                if (Time2 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (TestType != 2 && counter != 0)
            {
                printf("\n\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);
                fprintf(stderr, "\n\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);

                int lngRC = CompIVF(ChangedCPUDecNOutCurrent.c_str(), ChangedCPUDecNOutLast.c_str());

                if (lngRC >= 0)
                {
                    printf("\n\nFail: Files differ at frame: %i on file number %i", lngRC, FileNumber);
                    fprintf(stderr, "\n\nFail: Files differ at frame: %i on file number %i", lngRC, FileNumber);
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
            FileNumber++;
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        putenv("ON2_SIMD_CAPS=");
        return 10;
    }

    Time1 = TimeReturn(ChangedCPUDec0OutFile.c_str(), 0);
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
        snprintf(OutputChar1, 255, "Time1: %u == Time2: %u - Failed", Time1, Time2);
        string OutputChar1str = OutputChar1;
        FormatedPrint(OutputChar1str, 5);
        printf("\n");
        fprintf(stderr, "\n");
        overallfail = 1;
    }

    if (Time1 != Time2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Time 1: %u != Time 2: %u - Passed", Time1, Time2);
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

    string DFWMOutFileBase = WorkingDirString;

    DFWMOutFileBase.append(slashCharStr);
    DFWMOutFileBase.append("DFWMOutput");

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    /////Newly added//////
    opt.worst_allowed_q = 15;
    opt.allow_df = 1;
    opt.allow_spatial_resampling = 0;
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
            ///////Update File Name///////
            char num[20];
            itoa_custom(n, num, 10);
            string DFWMOutFile = DFWMOutFileBase;
            DFWMOutFile.append(num);
            DFWMOutFile.append(".ivf");

            printf("\n");
            fprintf(stderr, "\n");
            DMFW[i] = FileSize(DFWMOutFile.c_str());
            printf("\n");
            fprintf(stderr, "\n");

            i++;
            n = n - 20;

            DFWMOutFile.clear();
        }
    }
    else
    {
        while (n >= 0)
        {
            opt.drop_frames_water_mark = n;

            ///////Update File Name///////
            char num[20];
            itoa_custom(n, num, 10);
            string DFWMOutFile = DFWMOutFileBase;
            DFWMOutFile.append(num);
            DFWMOutFile.append(".ivf");

            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;

                if (CompressIVFtoIVF(input, DFWMOutFile.c_str(), speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile.c_str(), speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile.c_str(), speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile.c_str(), speed, BitRate, opt, CompressString, n, 0) == -1)
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

                if (CompressIVFtoIVF(input, DFWMOutFile.c_str(), speed, BitRate, opt, CompressString, n, 0) == -1)
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
                DMFW[i] = FileSize(DFWMOutFile.c_str());
                printf("\n");
                fprintf(stderr, "\n");
            }

            i++;
            n = n - 20;

            DFWMOutFile.clear();
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

    i = 0;
    n = 100;
    int EqualBool = 0;
    int testBool = 1;

    printf("\n\n");
    fprintf(stderr, "\n\n");

    while (i < 6)
    {
        printf("DFWM%4i Size: %8i bytes\n", n, DMFW[i]);//cout << "DFWM" << n << " Size " << DMFW[i] << "\n";
        fprintf(stderr, "DFWM%4i Size: %8i bytes\n", n, DMFW[i]);//cerr << "DFWM" << n << " Size " << DMFW[i] << "\n";
        i++;
        n = n - 20;
    }

    n = 100;
    i = 0;

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    while (i < 5)
    {
        if (DMFW[i+1] == DMFW[i])
        {
            EqualBool++;

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %i = DFWM%4i: %i - Indeterminate", n - 20, DMFW[i+1], n, DMFW[i]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[i+1] > DMFW[i])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %i > DFWM%4i: %i - Passed", n - 20, DMFW[i+1], n, DMFW[i]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[i+1] < DMFW[i])
        {
            testBool = 0;

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %i < DFWM%4i: %i - Failed", n - 20, DMFW[i+1], n, DMFW[i]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        i++;
        n = n - 20;
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

    string TargetBitRate1 = WorkingDirString;
    //string WorkingDir5 = WorkingDirString;

    TargetBitRate1.append(slashCharStr);
    TargetBitRate1.append("TargetBitRateOutput.ivf");

    //char TargetBitRate1[255];
    //snprintf(TargetBitRate1, 255, "%s", WorkingDir4.c_str());

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    int TargetDataRatePercentage = 30;

    opt.target_bandwidth = BitRate;

    ////////////////Data Rate Specific Settings////////////////
    opt.best_allowed_q = 0;
    opt.worst_allowed_q = 63;
    opt.allow_df = 1;
    ///////////////////////////////////////////////////////////

    int CompressInt = opt.allow_df;

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

            if (CompressIVFtoIVF(input, TargetBitRate1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

            if (CompressIVFtoIVF(input, TargetBitRate1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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
    double FileDataRate = IVFDataRate(TargetBitRate1.c_str(), 1);
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

    snprintf(ExeInputRelease, 255, "%s", argv[6]);

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

    string DebugOutput = WorkingDirString;
    string ReleaseOutput = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string ParFileDebug = WorkingDirString;
    string ParFileRelease = WorkingDirString;
    string ProgramDebug = ExeCharDebugReleaseString;
    string ProgramRelease = ExeCharDebugReleaseString;

    string DebugExeLoc = ExeCharDebugReleaseString;
    DebugExeLoc.append(ExeInputDebug);
    string ReleaseExeLoc = ExeCharDebugReleaseString;
    ReleaseExeLoc.append(ExeInputRelease);

    DebugOutput.append(slashCharStr);
    DebugOutput.append("output_Debug.ivf");
    ReleaseOutput.append(slashCharStr);
    ReleaseOutput.append("output_Release.ivf");

    ParFileDebug.append(slashCharStr);
    ParFileDebug.append("ParFileDebug.txt");
    ParFileRelease.append(slashCharStr);
    ParFileRelease.append("ParFileRelease.txt");

#if defined(_WIN32)
    {
        ProgramDebug.append(ExeInputRelease);
        ProgramDebug.append("\" \"");
        ProgramDebug.append(input);
        ProgramDebug.append("\" \"");
        ProgramDebug.append(DebugOutput);
        ProgramDebug.append("\" 8");
        ProgramDebug.append(" \"");
        ProgramDebug.append(ParFileDebug);
        ProgramDebug.append("\"");
        ProgramDebug.append(" 0");
        ProgramDebug.append("\"");
        ProgramRelease.append(ExeInputDebug);
        ProgramRelease.append("\" \"");
        ProgramRelease.append(input);
        ProgramRelease.append("\" \"");
        ProgramRelease.append(ReleaseOutput);
        ProgramRelease.append("\" 8");
        ProgramRelease.append(" \"");
        ProgramRelease.append(ParFileRelease);
        ProgramRelease.append("\"");
        ProgramRelease.append(" 0");
        ProgramRelease.append("\"");
    }
#elif defined(linux)
    {
        ProgramDebug.append(ExeInputRelease);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(input);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(DebugOutput);
        ProgramDebug.append("\' 8");
        ProgramDebug.append(" \'");
        ProgramDebug.append(ParFileDebug);
        ProgramDebug.append("\'");
        ProgramDebug.append(" 0");
        ProgramRelease.append(ExeInputDebug);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(input);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(ReleaseOutput);
        ProgramRelease.append("\' 8");
        ProgramRelease.append(" \'");
        ProgramRelease.append(ParFileRelease);
        ProgramRelease.append("\'");
        ProgramRelease.append(" 0");

    }
#elif defined(__APPLE__)
    {
        ProgramDebug.append(ExeInputRelease);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(input);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(DebugOutput);
        ProgramDebug.append("\' 8");
        ProgramDebug.append(" \'");
        ProgramDebug.append(ParFileDebug);
        ProgramDebug.append("\'");
        ProgramDebug.append(" 0");
        ProgramRelease.append(ExeInputDebug);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(input);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(ReleaseOutput);
        ProgramRelease.append("\' 8");
        ProgramRelease.append(" \'");
        ProgramRelease.append(ParFileRelease);
        ProgramRelease.append("\'");
        ProgramRelease.append(" 0");
    }
#elif defined(__POWERPC__)
    {
        ProgramDebug.append(ExeInputRelease);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(input);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(DebugOutput);
        ProgramDebug.append("\' 8");
        ProgramDebug.append(" \'");
        ProgramDebug.append(ParFileDebug);
        ProgramDebug.append("\'");
        ProgramDebug.append(" 0");
        ProgramRelease.append(ExeInputDebug);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(input);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(ReleaseOutput);
        ProgramRelease.append("\' 8");
        ProgramRelease.append(" \'");
        ProgramRelease.append(ParFileRelease);
        ProgramRelease.append("\'");
        ProgramRelease.append(" 0");
    }
#endif
#if defined(_WIN32)
    {
        ProgramDebug.insert(0, "\"\"");
        ProgramRelease.insert(0, "\"\"");
    }
#else
    {
        ProgramDebug.insert(0, "\'");
        ProgramRelease.insert(0, "\'");
    }
#endif


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
        BitRate = opt.target_bandwidth;
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

    opt.target_bandwidth = BitRate;

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

            OutPutSettings(ParFileDebug.c_str(), opt);
            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease.c_str(), opt);

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

            OutPutSettings(ParFileDebug.c_str(), opt);
            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease.c_str(), opt);

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

            OutPutSettings(ParFileDebug.c_str(), opt);
            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease.c_str(), opt);

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

            OutPutSettings(ParFileDebug.c_str(), opt);
            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease.c_str(), opt);

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

            OutPutSettings(ParFileDebug.c_str(), opt);
            RunExe(ProgramDebug);
            OutPutSettings(ParFileRelease.c_str(), opt);

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

    int lngRC = CompIVF(ReleaseOutput.c_str(), DebugOutput.c_str());

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

    string EncBreakOut0 = WorkingDirString;
    string EncBreakOut100 = WorkingDirString;
    string EncBreakOut500 = WorkingDirString;
    string EncBreakOut1000 = WorkingDirString;

    string EncBreakOut0_Dec = WorkingDirString;
    string EncBreakOut100_Dec = WorkingDirString;
    string EncBreakOut500_Dec = WorkingDirString;
    string EncBreakOut1000_Dec = WorkingDirString;

    EncBreakOut0.append(slashCharStr);
    EncBreakOut0.append("EncBreakOut0.ivf");
    EncBreakOut100.append(slashCharStr);
    EncBreakOut100.append("EncBreakOut100.ivf");
    EncBreakOut500.append(slashCharStr);
    EncBreakOut500.append("EncBreakOut500.ivf");
    EncBreakOut1000.append(slashCharStr);
    EncBreakOut1000.append("EncBreakOut1000.ivf");

    EncBreakOut0_Dec.append(slashCharStr);
    EncBreakOut0_Dec.append("EncBreakOut0_Dec.ivf");
    EncBreakOut100_Dec.append(slashCharStr);
    EncBreakOut100_Dec.append("EncBreakOut100_Dec.ivf");
    EncBreakOut500_Dec.append(slashCharStr);
    EncBreakOut500_Dec.append("EncBreakOut500_Dec.ivf");
    EncBreakOut1000_Dec.append(slashCharStr);
    EncBreakOut1000_Dec.append("EncBreakOut1000_Dec.ivf");

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;

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
            opt.encode_breakout = 0;

            if (CompressIVFtoIVF(input, EncBreakOut0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.encode_breakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.encode_breakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500.c_str(), speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.encode_breakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000.c_str(), speed, BitRate, opt, CompressString, 1000, 0) == -1)
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
            opt.encode_breakout = 0;

            if (CompressIVFtoIVF(input, EncBreakOut0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.encode_breakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.encode_breakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500.c_str(), speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.encode_breakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000.c_str(), speed, BitRate, opt, CompressString, 1000, 0) == -1)
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
            opt.encode_breakout = 0;

            if (CompressIVFtoIVF(input, EncBreakOut0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.encode_breakout = 100;

            if (CompressIVFtoIVF(input, EncBreakOut100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.encode_breakout = 500;

            if (CompressIVFtoIVF(input, EncBreakOut500.c_str(), speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.encode_breakout = 1000;

            if (CompressIVFtoIVF(input, EncBreakOut1000.c_str(), speed, BitRate, opt, CompressString, 1000, 0) == -1)
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
            opt.encode_breakout = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.encode_breakout = 100;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.encode_breakout = 500;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut500.c_str(), speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.encode_breakout = 1000;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, EncBreakOut1000.c_str(), speed, BitRate, opt, CompressString, 1000, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.encode_breakout = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.encode_breakout = 100;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.encode_breakout = 500;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut500.c_str(), speed, BitRate, opt, CompressString, 500, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.encode_breakout = 1000;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, EncBreakOut1000.c_str(), speed, BitRate, opt, CompressString, 1000, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        printf("\nDecoding EncBreakOut0");
        fprintf(stderr, "\nDecoding EncBreakOut0");
        DecompressIVFtoIVF(EncBreakOut0.c_str(), EncBreakOut0_Dec.c_str());

        printf("\n\nDecoding EncBreakOut100");
        fprintf(stderr, "\n\nDecoding EncBreakOut100");
        DecompressIVFtoIVF(EncBreakOut100.c_str(), EncBreakOut100_Dec.c_str());

        printf("\n\nDecoding EncBreakOut500");
        fprintf(stderr, "\n\nDecoding EncBreakOut500");
        DecompressIVFtoIVF(EncBreakOut500.c_str(), EncBreakOut500_Dec.c_str());

        printf("\n\nDecoding EncBreakOut1000");
        fprintf(stderr, "\n\nDecoding EncBreakOut1000");
        DecompressIVFtoIVF(EncBreakOut1000.c_str(), EncBreakOut1000_Dec.c_str());
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
    EncBreakOut0_Dec_FileSize = FileSize(EncBreakOut0_Dec.c_str());

    printf("\n");
    fprintf(stderr, "\n");
    EncBreakOut100_Dec_FileSize = FileSize(EncBreakOut100_Dec.c_str());

    printf("\n");
    fprintf(stderr, "\n");
    EncBreakOut500_Dec_FileSize = FileSize(EncBreakOut500_Dec.c_str());

    printf("\n");
    fprintf(stderr, "\n");
    EncBreakOut1000_Dec_FileSize = FileSize(EncBreakOut1000_Dec.c_str());

    printf("\n");
    fprintf(stderr, "\n");

    double PSNR0;
    double PSNR100;
    double PSNR500;
    double PSNR1000;

    PSNR0 = IVFPSNR(input, EncBreakOut0.c_str(), 0, 0, 1, NULL);
    PSNR100 = IVFPSNR(input, EncBreakOut100.c_str(), 0, 0, 1, NULL);
    PSNR500 = IVFPSNR(input, EncBreakOut500.c_str(), 0, 0, 1, NULL);
    PSNR1000 = IVFPSNR(input, EncBreakOut1000.c_str(), 0, 0, 1, NULL);

    double dB1 = absDouble(10 * log(PSNR0 / PSNR100));
    double dB2 = absDouble(10 * log(PSNR100 / PSNR500));
    double dB3 = absDouble(10 * log(PSNR500 / PSNR1000));

    printf("\ndB1: %f \n", dB1);
    fprintf(stderr, "\ndB1: %f \n", dB1);

    printf("dB2: %f \n", dB1);
    fprintf(stderr, "dB2: %f \n", dB1);

    printf("dB3: %f \n", dB1);
    fprintf(stderr, "dB3: %f \n", dB1);

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

    string ErrorOnOutFile = WorkingDirString;
    string ErrorOffOutFile = WorkingDirString;

    ErrorOnOutFile.append(slashCharStr);
    ErrorOnOutFile.append("ErrorOnOutput.ivf");
    ErrorOffOutFile.append(slashCharStr);
    ErrorOffOutFile.append("ErrorOffOutput.ivf");

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

    opt.target_bandwidth = BitRate;

    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.error_resilient_mode = 1;

            if (CompressIVFtoIVF(input, ErrorOnOutFile.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.error_resilient_mode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.error_resilient_mode = 1;

            if (CompressIVFtoIVF(input, ErrorOnOutFile.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.error_resilient_mode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.error_resilient_mode = 1;

            if (CompressIVFtoIVF(input, ErrorOnOutFile.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.error_resilient_mode = 0;

            if (CompressIVFtoIVF(input, ErrorOffOutFile.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
            opt.error_resilient_mode = 1;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, ErrorOnOutFile.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.error_resilient_mode = 0;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, ErrorOffOutFile.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.error_resilient_mode = 1;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ErrorOnOutFile.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.error_resilient_mode = 0;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, ErrorOffOutFile.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    printf("\n");
    fprintf(stderr, "\n");

    double PSNRon;
    double PSNRoff;

    PSNRon = IVFPSNR(input, ErrorOnOutFile.c_str(), 0, 0, 1, NULL);
    PSNRoff = IVFPSNR(input, ErrorOffOutFile.c_str(), 0, 0, 1, NULL);

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

    int CompressInt = opt.allow_df;
    int BitRate = opt.target_bandwidth;
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

        if (CompressIVFtoIVF(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 0, 0/*opt.DeleteFirstPassFile*/) == -1)
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

    int CompressInt = opt.allow_df;
    int BitRate = opt.target_bandwidth;
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

        if (CompressIVFtoIVF(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 0, 0) == -1)
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

        int CompressInt = opt.allow_df;
        int BitRate = opt.target_bandwidth;
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

            if (CompressIVFtoIVF(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 0, 0, 0/*opt.DeleteFirstPassFile*/) == -1)
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

        int CompressInt = opt.allow_df;
        int BitRate = opt.target_bandwidth;
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

            if (CompressIVFtoIVF(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 1, 0/*opt.DeleteFirstPassFile*/) == -1)
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

        string FixedQ1 = WorkingDirString;
        string FixedQ2 = WorkingDirString;

        FixedQ1.append(slashCharStr);
        FixedQ1.append("FixedQ_");
        FixedQ2.append(slashCharStr);
        FixedQ2.append("FixedQ_");

        FixedQ1.append(argv[5]);
        FixedQ2.append(argv[6]);

        FixedQ1.append(".ivf");
        FixedQ2.append(".ivf");

        //char FixedQ1[255];
        //char FixedQ2[255];

        //snprintf(FixedQ1, 255, "%s", WorkingDir4.c_str());
        //snprintf(FixedQ2, 255, "%s", WorkingDir5.c_str());

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        opt.target_bandwidth = BitRate;
        opt.fixed_q = 1;
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
                opt.fixed_q = FixedQ1Int;

                if (CompressIVFtoIVF(input, FixedQ1.c_str(), speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.fixed_q = FixedQ2Int;

                if (CompressIVFtoIVF(input, FixedQ2.c_str(), speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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
                opt.fixed_q = FixedQ1Int;

                if (CompressIVFtoIVF(input, FixedQ1.c_str(), speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.fixed_q = FixedQ2Int;

                if (CompressIVFtoIVF(input, FixedQ2.c_str(), speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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
                opt.fixed_q = FixedQ1Int;

                if (CompressIVFtoIVF(input, FixedQ1.c_str(), speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.fixed_q = FixedQ2Int;

                if (CompressIVFtoIVF(input, FixedQ2.c_str(), speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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
                opt.fixed_q = FixedQ1Int;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, FixedQ1.c_str(), speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.fixed_q = FixedQ2Int;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, FixedQ2.c_str(), speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.fixed_q = FixedQ1Int;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, FixedQ1.c_str(), speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.fixed_q = FixedQ2Int;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, FixedQ2.c_str(), speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
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
        int FixedQCheckVal1 = CheckFixedQ(FixedQ1.c_str(), FixedQ1Int);
        printf("\n");
        fprintf(stderr, "\n");
        int FixedQCheckVal2 = CheckFixedQ(FixedQ2.c_str(), FixedQ2Int);
        printf("\n");
        fprintf(stderr, "\n");

        printf("\n");
        fprintf(stderr, "\n");
        int FixedQSize1 = FileSize(FixedQ1.c_str());
        printf("\n");
        fprintf(stderr, "\n");
        int FixedQSize2 = FileSize(FixedQ2.c_str());
        printf("\n");
        fprintf(stderr, "\n");

        char FixedQ1FileName[255] = "";
        char FixedQ2FileName[255] = "";

        FileName(FixedQ1.c_str(), FixedQ1FileName, 0);
        FileName(FixedQ2.c_str(), FixedQ2FileName, 0);

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

        char *CompressString = "opt.key_freq";
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

        string KeyFrameoutputfile = WorkingDirString;
        string ForceKeyFrame = WorkingDirString;

        KeyFrameoutputfile.append(slashCharStr);
        KeyFrameoutputfile.append("ForceKeyFrameOutput_KeyFrames.txt");
        ForceKeyFrame.append(slashCharStr);
        ForceKeyFrame.append("ForceKeyFrameOutput.ivf");

        //char KeyFrameoutputfile[255];
        //char ForceKeyFrame[255];

        //snprintf(outputfile, 255, "%s", WorkingDir4.c_str());
        //snprintf(ForceKeyFrame, 255, "%s", WorkingDir5.c_str());

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
        opt.auto_key = 0;//1;

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
                opt.key_freq = 0;//ForceKeyFrameInt;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame.c_str(), speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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
                opt.key_freq = 0;//ForceKeyFrameInt;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame.c_str(), speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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
                opt.key_freq = 0;//ForceKeyFrameInt;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame.c_str(), speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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
                opt.key_freq = 0;//ForceKeyFrameInt;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame.c_str(), speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.key_freq = 0;//ForceKeyFrameInt;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVFForceKeyFrame(input, ForceKeyFrame.c_str(), speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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

        IVFDisplayKeyFrames(ForceKeyFrame.c_str(), 1);
        //////////////////////////////////////////////////////////////////////////////////
        ifstream infile(KeyFrameoutputfile.c_str());

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

        int NumberofFrames = GetNumberofFrames(ForceKeyFrame.c_str());

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
    int FrameSizeTest(int argc, char * argv[], string WorkingDir, string FilesAr[], int TestType)
    {
        char *CompressString = "Frame Size";

        int StartingWidth = 0;
        int StartingHeight = 0;

        if (!(argc == 8 || argc == 7))
        {
            printf(
                "  FrameSizeTest \n\n"
                "    <inputfile>\n"
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
                "	 <Optional Settings File>\n"
            );

            return 0;
        }

        StartingWidth = atoi(argv[5]);
        StartingHeight = atoi(argv[6]);



        char *input = argv[2];

        ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////

        string WorkingDirString = ""; // <- All Options need to set a value for this
        string Mode3TestMatch = "";
        string MainDirString = "";
        char *MyDir = "FrameSizeTest";
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

        char InputFileName[256] = "";
        FileName(input, InputFileName, 1);

        string WorkingDir4 = WorkingDirString;
        WorkingDir4.append(slashCharStr);
        WorkingDir4.append(InputFileName);

        char NewWidth[20];
        char NewHeight[20];
        string RawCrop[47];

        //height
        int counter = 0;
        int FileNum = 1;

        while (counter < 16)
        {
            RawCrop[FileNum] = WorkingDir4;
            RawCrop[FileNum].append("_");
            itoa_custom(StartingWidth, NewWidth, 10); //width
            RawCrop[FileNum].append(NewWidth);
            RawCrop[FileNum].append("x");
            itoa_custom(StartingHeight - (counter), NewHeight, 10); //height
            RawCrop[FileNum].append(NewHeight);
            RawCrop[FileNum].append("_raw.ivf");

            counter++;
            FileNum++;
        }

        //width
        counter = 1;

        while (counter < 16)
        {
            RawCrop[FileNum] = WorkingDir4;
            RawCrop[FileNum].append("_");
            itoa_custom(StartingWidth - (counter), NewWidth, 10); //width
            RawCrop[FileNum].append(NewWidth);
            RawCrop[FileNum].append("x");
            itoa_custom(StartingHeight, NewHeight, 10); //height
            RawCrop[FileNum].append(NewHeight);
            RawCrop[FileNum].append("_raw.ivf");

            counter++;
            FileNum++;
        }

        //width and height
        counter = 1;

        while (counter < 16)
        {
            RawCrop[FileNum] = WorkingDir4;
            RawCrop[FileNum].append("_");
            itoa_custom(StartingWidth - (counter), NewWidth, 10); //width
            RawCrop[FileNum].append(NewWidth);
            RawCrop[FileNum].append("x");
            itoa_custom(StartingHeight - (counter), NewHeight, 10); //height
            RawCrop[FileNum].append(NewHeight);
            RawCrop[FileNum].append("_raw.ivf");

            counter++;
            FileNum++;
        }

        //encoded file names
        FileNum = 1;
        string EncCrop[47];

        while (FileNum < 47)
        {
            EncCrop[FileNum] = RawCrop[FileNum];
            EncCrop[FileNum].erase(EncCrop[FileNum].end() - 7, EncCrop[FileNum].end());
            EncCrop[FileNum].append("enc.ivf");
            FileNum++;
        }

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

        //Make sure starting width and height are mults of 16
        if ((StartingWidth % 16 != 0) && (StartingHeight % 16 != 0))
        {
            printf("\nError: Starting width and height are not multiples of 16\n");
            fprintf(stderr, "\nError: Starting width and height are not multiples of 16\n");

            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (StartingHeight % 16 != 0)
        {
            printf("\nError: Starting height is not a multiple of 16\n");
            fprintf(stderr, "\nError: Starting height is not a multiple of 16\n");

            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (StartingWidth % 16 != 0)
        {
            printf("\nError: Starting width is not a multiple of 16\n");
            fprintf(stderr, "\nError: Starting width is not a multiple of 16\n");

            printf("\nFailed\n");
            fprintf(stderr, "\nFailed\n");
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        int speed = 0;
        int BitRate = atoi(argv[4]);

        int Mode = atoi(argv[3]);

        printf("Frame Size Test");
        fprintf(stderr, "Frame Size Test");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;

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

            //Create Raw Crops
            int x = 0;
            int RawCropNum = 1;

            while (x < 16)
            {
                printf("\nCroping to %i %i", StartingWidth, StartingHeight - x);
                fprintf(stderr, "\nCroping to %i %i", StartingWidth, StartingHeight - x);
                CropRawIVF(input, RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth, StartingHeight - x, 1, 1);
                x++;
                RawCropNum++;
                //return 0;
            }

            x = 1;

            while (x < 16)
            {
                printf("\nCroping to %i %i", StartingWidth - x, StartingHeight);
                fprintf(stderr, "\nCroping to %i %i", StartingWidth - x, StartingHeight);
                CropRawIVF(input, RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth - x, StartingHeight, 1, 1);
                x++;
                RawCropNum++;
            }

            x = 1;

            while (x < 16)
            {
                printf("\nCroping to %i %i", StartingWidth - x, StartingHeight - x);
                fprintf(stderr, "\nCroping to %i %i", StartingWidth - x, StartingHeight - x);
                CropRawIVF(input, RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth - x, StartingHeight - x, 1, 1);
                x++;
                RawCropNum++;
            }

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

            if (Mode == 4)
            {
                opt.Mode = MODE_SECONDPASS;
            }

            if (Mode == 5)
            {
                opt.Mode = MODE_SECONDPASS_BEST;
            }

            //Create Compressions
            RawCropNum = 1;

            while (RawCropNum < 47)
            {
                char FileNameChar[256];
                char FileNameChar2[256];
                snprintf(FileNameChar, 256, RawCrop[RawCropNum].c_str());
                FileName(FileNameChar, FileNameChar2, 1);

                printf("\nCompressing %s", FileNameChar2);
                fprintf(stderr, "\nCompressing %s", FileNameChar2);

                if (CompressIVFtoIVF(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                RawCropNum++;
            }
        }

        if (TestType == 2)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 10;
        }

        int PercentFail = 0;
        int MinPSNRFail = 0;
        double PSNRAr[46];

        int RawCropNum = 1;

        while (RawCropNum < 47)
        {
            PSNRAr[RawCropNum-1] = IVFPSNR(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), 0, 0, 1, NULL);
            RawCropNum++;
        }

        RawCropNum = 1;
        double ThreePercentPSNR = (3 * PSNRAr[0]) / 100;

        while (RawCropNum < 47)
        {
            char FileNameChar[256];
            char FileNameChar2[256];
            snprintf(FileNameChar, 256, EncCrop[RawCropNum].c_str());
            FileName(FileNameChar, FileNameChar2, 0);

            if (RawCropNum == 1)
            {
                printf("\n\n PSNR %s: %.2f", FileNameChar2, PSNRAr[RawCropNum-1]);
            }
            else if (PSNRAr[RawCropNum-1] <  PSNRAr[0] + ThreePercentPSNR && PSNRAr[RawCropNum-1] >  PSNRAr[0] - ThreePercentPSNR)
            {
                printf("\n PSNR %s: %.2f within 3%% of %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
                fprintf(stderr, "\n PSNR %s: %.2f within 3%% of %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
            }
            else
            {
                printf("\n PSNR %s: %.2f not within 3%% of %.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
                fprintf(stderr, "\n PSNR %s: %.2f not within 3%% of %.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
                PercentFail = 1;
            }

            RawCropNum++;
        }

        printf("\n\n");
        fprintf(stderr, "\n\n");

        RawCropNum = 1;

        while (RawCropNum < 47)
        {
            char FileNameChar[256];
            char FileNameChar2[256];
            snprintf(FileNameChar, 256, EncCrop[RawCropNum].c_str());
            FileName(FileNameChar, FileNameChar2, 0);

            if (PSNRAr[RawCropNum-1] > 25.0)
            {
                printf("\n PSNR %s: %.2f > %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
                fprintf(stderr, "\n PSNR %s: %.2f > %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
            }
            else
            {
                printf("\n PSNR %s: %.2f < %.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
                fprintf(stderr, "\n PSNR %s: %.2f <%.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
                MinPSNRFail = 1;
            }

            RawCropNum++;
        }


        printf("\n");

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        int fail = 0;

        if (PercentFail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All PSNRs are within 3%% of %.2f - Passed", PSNRAr[0]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all PSNRs are within 3%% of %.2f - Failed", PSNRAr[0]);
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
            fail = 1;
        }

        if (MinPSNRFail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All PSNRs are greater than 25.0 - Passed");
            string OutputChar1str = OutputChar1;
            FormatedPrint(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all PSNRs are greater than 25.0 - Failed");
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


        string GvBgOutFile1 = WorkingDirString;
        string GvBbOutFile1 = WorkingDirString;
        string GvBgOutFile2 = WorkingDirString;
        string GvBbOutFile2 = WorkingDirString;
        string GvBgOutFile3 = WorkingDirString;
        string GvBbOutFile3 = WorkingDirString;

        GvBgOutFile1.append(slashCharStr);
        GvBgOutFile1.append("GoodVsBestGood1.ivf");
        GvBbOutFile1.append(slashCharStr);
        GvBbOutFile1.append("GoodVsBestBest1.ivf");
        GvBgOutFile2.append(slashCharStr);
        GvBgOutFile2.append("GoodVsBestGood2.ivf");
        GvBbOutFile2.append(slashCharStr);
        GvBbOutFile2.append("GoodVsBestBest2.ivf");
        GvBgOutFile3.append(slashCharStr);
        GvBgOutFile3.append("GoodVsBestGood3.ivf");
        GvBbOutFile3.append(slashCharStr);
        GvBbOutFile3.append("GoodVsBestBest3.ivf");

        //char GvBgOutFile1[255];
        //char GvBgOutFile2[255];
        //char GvBgOutFile3[255];
        //char GvBbOutFile1[255];
        //char GvBbOutFile2[255];
        //char GvBbOutFile3[255];

        //snprintf(GvBgOutFile1, 255, "%s", WorkingDir4.c_str());
        //snprintf(GvBbOutFile1, 255, "%s", WorkingDir5.c_str());
        //snprintf(GvBgOutFile2, 255, "%s", WorkingDir4b.c_str());
        //snprintf(GvBbOutFile2, 255, "%s", WorkingDir5b.c_str());
        //snprintf(GvBgOutFile3, 255, "%s", WorkingDir4c.c_str());
        //snprintf(GvBbOutFile3, 255, "%s", WorkingDir5c.c_str());

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        int CompressInt = opt.allow_df;

        int BitRate1 = BitRate - (BitRate * 0.3);
        int BitRate2 = BitRate;
        int BitRate3 = BitRate + (BitRate * 0.3);

        //opt.target_bandwidth = BitRate;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            opt.target_bandwidth = BitRate1;
            opt.Mode = MODE_GOODQUALITY;

            if (CompressIVFtoIVF(input, GvBgOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate2;

            if (CompressIVFtoIVF(input, GvBgOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate3;

            if (CompressIVFtoIVF(input, GvBgOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate1;
            opt.Mode = MODE_BESTQUALITY;

            if (CompressIVFtoIVF(input, GvBbOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate2;

            if (CompressIVFtoIVF(input, GvBbOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate3;

            if (CompressIVFtoIVF(input, GvBbOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

        float GoodSize1 = IVFDataRate(GvBgOutFile1.c_str(), 1);
        float BestSize1 = IVFDataRate(GvBbOutFile1.c_str(), 1);
        float GoodSize2 = IVFDataRate(GvBgOutFile2.c_str(), 1);
        float BestSize2 = IVFDataRate(GvBbOutFile2.c_str(), 1);
        float GoodSize3 = IVFDataRate(GvBgOutFile3.c_str(), 1);
        float BestSize3 = IVFDataRate(GvBbOutFile3.c_str(), 1);

        double PSNRG1;
        double PSNRB1;
        double PSNRG2;
        double PSNRB2;
        double PSNRG3;
        double PSNRB3;

        PSNRG1 = IVFPSNR(input, GvBgOutFile1.c_str(), 1, 0, 1, NULL);
        PSNRB1 = IVFPSNR(input, GvBbOutFile1.c_str(), 1, 0, 1, NULL);
        PSNRG2 = IVFPSNR(input, GvBgOutFile2.c_str(), 1, 0, 1, NULL);
        PSNRB2 = IVFPSNR(input, GvBbOutFile2.c_str(), 1, 0, 1, NULL);
        PSNRG3 = IVFPSNR(input, GvBgOutFile3.c_str(), 1, 0, 1, NULL);
        PSNRB3 = IVFPSNR(input, GvBbOutFile3.c_str(), 1, 0, 1, NULL);

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

        SolveQuadradic(BestSize1, BestSize2, BestSize3, PSNRB1, PSNRB2, PSNRB3, BestA, BestB, BestC);
        float BestAreaVal = AreaUnderQuadradic(BestA, BestB, BestC, minCommon, maxCommon);

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

        string LagInFrames0 = WorkingDirString;
        string LagInFrames1 = WorkingDirString;
        string LagInFrames2 = WorkingDirString;

        LagInFrames0.append(slashCharStr);
        LagInFrames0.append("LagInFrames0Output.ivf");
        LagInFrames1.append(slashCharStr);
        LagInFrames1.append("LagInFrames1Output.ivf");
        LagInFrames2.append(slashCharStr);
        LagInFrames2.append("LagInFrames2Output.ivf");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;

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
                opt.allow_lag = 0;
                opt.lag_in_frames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames0.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames1.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames2Val;

                if (CompressIVFtoIVF(input, LagInFrames2.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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
                opt.allow_lag = 0;
                opt.lag_in_frames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames0.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }


                opt.Mode = MODE_GOODQUALITY;
                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames1.c_str(), speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames2Val;

                if (CompressIVFtoIVF(input, LagInFrames2.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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
                opt.allow_lag = 0;
                opt.lag_in_frames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames0.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames1Val;

                if (CompressIVFtoIVF(input, LagInFrames1.c_str(), speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames2Val;

                if (CompressIVFtoIVF(input, LagInFrames2.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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
                opt.allow_lag = 0;
                opt.lag_in_frames = LagInFrames1Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, LagInFrames0.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames1Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, LagInFrames1.c_str(), speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames2Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, LagInFrames2.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.allow_lag = 0;
                opt.lag_in_frames = LagInFrames1Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, LagInFrames0.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames1Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, LagInFrames1.c_str(), speed, BitRate, opt, CompressString, LagInFrames1Val, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.allow_lag = 1;
                opt.lag_in_frames = LagInFrames2Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, LagInFrames2.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
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

        double LagInFrames0PSNR = IVFPSNR(input, LagInFrames0.c_str(), 0, 0, 1, NULL);
        double LagInFrames1PSNR = IVFPSNR(input, LagInFrames1.c_str(), 0, 0, 1, NULL);
        double LagInFrames2PSNR = IVFPSNR(input, LagInFrames2.c_str(), 0, 0, 1, NULL);

        double TenPer0 = LagInFrames0PSNR / 10;
        double TenPer1 = LagInFrames1PSNR / 10;
        double TenPer2 = LagInFrames2PSNR / 10;

        int lngRC1 = CompIVF(LagInFrames0.c_str(), LagInFrames1.c_str());
        int lngRC2 = CompIVF(LagInFrames1.c_str(), LagInFrames2.c_str());

        string QuantInStr0 = LagInFrames0;
        QuantInStr0.erase(QuantInStr0.length() - 4, 4);
        QuantInStr0.append("_Quantizers.txt");
        int LagInFramesFound0 = IVFLagInFramesCheck(QuantInStr0.c_str());

        string QuantInStr1 = LagInFrames1;
        QuantInStr1.erase(QuantInStr1.length() - 4, 4);
        QuantInStr1.append("_Quantizers.txt");
        int LagInFramesFound1 = IVFLagInFramesCheck(QuantInStr1.c_str());

        string QuantInStr2 = LagInFrames2;
        QuantInStr2.erase(QuantInStr2.length() - 4, 4);
        QuantInStr2.append("_Quantizers.txt");
        int LagInFramesFound2 = IVFLagInFramesCheck(QuantInStr2.c_str());

        int PSNRTally = 0;
        int fail = 0;

        char LagInFrames0FileName[255] = "";
        char LagInFrames1FileName[255] = "";
        char LagInFrames2FileName[255] = "";

        FileName(LagInFrames0.c_str(), LagInFrames0FileName, 0);
        FileName(LagInFrames1.c_str(), LagInFrames1FileName, 0);
        FileName(LagInFrames2.c_str(), LagInFrames2FileName, 0);

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

        string QuantOutBase = WorkingDirString;

        QuantOutBase.append(slashCharStr);
        QuantOutBase.append("QuantOutput");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
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
                char num[20];
                itoa_custom(n, num, 10);

                string QuantOutFile = QuantOutBase;
                QuantOutFile.append(num);
                QuantOutFile.append(".ivf");

                printf("\n");
                fprintf(stderr, "\n");

                if (TestType != 2)
                {
                    PSNRArr[i] = IVFPSNR(input, QuantOutFile.c_str(), PSNRToggle, 0, 1, NULL);
                    printf("\n");
                    fprintf(stderr, "\n");
                    MaxQArr[i] = CheckMaxQ(QuantOutFile.c_str(), n);
                    printf("\n");
                    fprintf(stderr, "\n");
                }

                n = n + 8;
                i++;
            }
        }
        else
        {
            while (n < 63)
            {

                opt.worst_allowed_q = n;

                char num[20];
                itoa_custom(n, num, 10);

                string QuantOutFile = QuantOutBase;
                QuantOutFile.append(num);
                QuantOutFile.append(".ivf");

                if (Mode == 0)
                {
                    opt.Mode = MODE_REALTIME;

                    if (CompressIVFtoIVF(input, QuantOutFile.c_str(), speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile.c_str(), speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile.c_str(), speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile.c_str(), speed, BitRate, opt, CompressString, n, 1) == -1)
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

                    if (CompressIVFtoIVF(input, QuantOutFile.c_str(), speed, BitRate, opt, CompressString, n, 1) == -1)
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
                    PSNRArr[i] = IVFPSNR(input, QuantOutFile.c_str(), PSNRToggle, 0, 1, NULL);
                    printf("\n");
                    fprintf(stderr, "\n");
                    MaxQArr[i] = CheckMaxQ(QuantOutFile.c_str(), n);
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
        WorkingDir8.append("MemLeakCheckParFile.txt");

#if defined(_WIN32)
        {
            WorkingDir9 = "\"\"";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\" \"");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\" \"");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\" 8 \"");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\" 4 \"");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
            WorkingDir9.append("\"\"");
        }

#elif defined(linux)
        {
            WorkingDir9 = "\'";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\' \'");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\' \'");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\' 8 \'");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\' 4 \'");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
            WorkingDir9.append("\'");
        }
#elif defined(__APPLE__)
        {
            WorkingDir9 = "\"";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\" \"");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\" \"");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\" 8 \"");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\" 4 \"");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
            WorkingDir9.append("\"");
        }
#elif defined(__POWERPC__)
        {
            WorkingDir9 = "\"";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\" \"");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\" \"");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\" 8 \"");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\" 4 \"");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
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
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);        // Exe Name
            WorkingDir11.append("\" \"");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\" \"");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\" 8 \"");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir11.append("\" 4 \"");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
            WorkingDir11.append("\"\"");
        }
#elif defined(linux)
        {
            WorkingDir11 = "\'";
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);            // Exe Name
            WorkingDir11.append("\' \'");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\' \'");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\' 8 \'");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir11.append("\' 4 \'");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
            WorkingDir11.append("\'");
        }
#elif defined(__APPLE__)
        {
            WorkingDir11 = "\"";
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);            // Exe Name
            WorkingDir11.append("\" \"");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\" \"");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\" 8 \"");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir11.append("\" 4 \"");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
            WorkingDir11.append("\"");
        }
#elif defined(__POWERPC__)
        {
            WorkingDir11 = "\"";
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);            // Exe Name
            WorkingDir11.append("\" \"");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\" \"");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\" 8 \"");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir11.append("\" 4 \"");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate ;

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

        snprintf(MemLeakExe, 255, "%s", argv[2]);
        snprintf(DecInFile, 255, "%s", argv[3]);

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
        WorkingDir8.append("MemLeakCheckParFile");

#if defined(_WIN32)
        {
            WorkingDir9 = "\"\"";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\" \"");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\" \"");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\" 0 \"");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\" 5 \"");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
            WorkingDir9.append("\"\"");
        }

#elif defined(linux)
        {
            WorkingDir9 = "\'";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\' \'");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\' \'");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\' 0 \'");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\' 5 \'");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
            WorkingDir9.append("\'");
        }
#elif defined(__APPLE__)
        {
            WorkingDir9 = "\"";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\" \"");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\" \"");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\" 0 \"");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\" 5 \"");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
            WorkingDir9.append("\"");
        }
#elif defined(__POWERPC__)
        {
            WorkingDir9 = "\"";
            WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
            WorkingDir9.append(MemLeakExe);         // Exe Name
            WorkingDir9.append("\" \"");
            WorkingDir9.append(input);              // Input
            WorkingDir9.append("\" \"");
            WorkingDir9.append(WorkingDir4);        // Output
            WorkingDir9.append("\" 0 \"");
            WorkingDir9.append(WorkingDir8);        // Par File
            WorkingDir9.append("\" 5 \"");
            WorkingDir9.append(WorkingDir7);        // Mem Output File
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
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);            // Exe Name
            WorkingDir11.append("\" \"");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\" \"");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\" 0 \"");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir12 = WorkingDir11;
            WorkingDir11.append("\" 5 \"");
            WorkingDir12.append("\" 6 \"");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append("\" \"");
            WorkingDir12.append(DecInFile);
            WorkingDir11.append("\"");
            WorkingDir12.append("\"");
        }
#elif defined(linux)
        {
            WorkingDir11 = "\'";
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);            // Exe Name
            WorkingDir11.append("\' \'");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\' \'");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\' 0 \'");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir12 = WorkingDir11;
            WorkingDir11.append("\' 5 \'");
            WorkingDir12.append("\' 6 \'");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append("\' \'");
            WorkingDir12.append(DecInFile);
            WorkingDir11.append("\'");
            WorkingDir12.append("\'");
        }
#elif defined(__APPLE__)
        {
            WorkingDir11 = "\"";
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);            // Exe Name
            WorkingDir11.append("\" \"");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\" \"");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\" 0 \"");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir12 = WorkingDir11;
            WorkingDir11.append("\" 5 \"");
            WorkingDir12.append("\" 6 \"");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append("\" \"");
            WorkingDir12.append(DecInFile);
            WorkingDir11.append("\"");
            WorkingDir12.append("\"");
        }
#elif defined(__POWERPC__)
        {
            WorkingDir11 = "\"";
            WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
            WorkingDir11.append(MemLeakExe);            // Exe Name
            WorkingDir11.append("\" \"");
            WorkingDir11.append(input);             // Input
            WorkingDir11.append("\" \"");
            WorkingDir11.append(WorkingDir4);       // Output
            WorkingDir11.append("\" 0 \"");
            WorkingDir11.append(WorkingDir8);       // Par File
            WorkingDir12 = WorkingDir11;
            WorkingDir11.append("\" 5 \"");
            WorkingDir12.append("\" 6 \"");
            WorkingDir11.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append(WorkingDir10);      // Mem Output File
            WorkingDir12.append("\" \"");
            WorkingDir12.append(DecInFile);
            WorkingDir11.append("\"");
            WorkingDir12.append("\"");
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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate ;

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
            FileName(MemLeakCheckTXT1, MemLeakCheckTXTFileName, 0);
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
            FileName(MemLeakCheckTXT2, MemLeakCheckTXTFileName, 0);
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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        double PSNRArr[2];
        opt.target_bandwidth = BitRate;
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
                opt.best_allowed_q = 10;

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.best_allowed_q = 60;

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
                opt.best_allowed_q = 10;

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.best_allowed_q = 60;

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
                opt.best_allowed_q = 10;

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.best_allowed_q = 60;

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
                opt.best_allowed_q = 10;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.best_allowed_q = 60;
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
                opt.best_allowed_q = 10;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.best_allowed_q = 60;
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

        PSNRArr[0] = IVFPSNR(input, Min10QuantOutFile, 0, 0, 1, NULL);
        PSNRArr[1] = IVFPSNR(input, Min60QuantOutFile, 0, 0, 1, NULL);

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

        FileName(Min10QuantOutFile, Min10FileName, 0);
        FileName(Min60QuantOutFile, Min60FileName, 0);

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


        string MultiThreadedOnOutFile = WorkingDirString;
        string MultiThreadedOffOutFile = WorkingDirString;

        MultiThreadedOnOutFile.append(slashCharStr);
        MultiThreadedOnOutFile.append("MultiThreadedOnOutput.ivf");
        MultiThreadedOffOutFile.append(slashCharStr);
        MultiThreadedOffOutFile.append("MultiThreadedOffOutput.ivf");

        //char MultiThreadedOnOutFile[255];
        //char MultiThreadedOffOutFile[255];

        //snprintf(MultiThreaded14OutFile, 255, "%s", WorkingDir4.c_str());
        //snprintf(MultiThreaded00OutFile, 255, "%s", WorkingDir5.c_str());

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

        unsigned int CoreCount = VPX_GetProcCoreCount();

        if (!(CoreCount > 1))
        {
            printf("\nMulti Core not found: Test Aborted: %i\n", VPX_GetProcCoreCount());
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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
        opt.end_usage = 0;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            Time1 = TimeReturn(MultiThreadedOnOutFile.c_str(), 0);
            Time2 = TimeReturn(MultiThreadedOffOutFile.c_str(), 0);
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.multi_threaded = CoreCount;
                opt.cpu_used = -1;
                Time1 = TimeCompressIVFtoIVF(input, MultiThreadedOnOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, CoreCount, 0);

                if (Time1 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.multi_threaded = 0;
                Time2 = TimeCompressIVFtoIVF(input, MultiThreadedOffOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, 0, 0);

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
                opt.multi_threaded = CoreCount;
                Time1 = TimeCompressIVFtoIVF(input, MultiThreadedOnOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, CoreCount, 0);

                if (Time1 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.multi_threaded = 0;
                Time2 = TimeCompressIVFtoIVF(input, MultiThreadedOffOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, 0, 0);

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

        FileName(MultiThreadedOnOutFile.c_str(), Time1FileName, 0);
        FileName(MultiThreadedOffOutFile.c_str(), Time2FileName, 0);

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

//#if defined(_WIN32)
//        {
        snprintf(ExeInput, 255, "%s", argv[5]);
//        }
//#elif defined(linux)
//        {
//            string ExeInputStr = argv[5];
//            snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
//        }
//#elif defined(__APPLE__)
//        {
//            string ExeInputStr = argv[5];
//            snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
//        }
//#elif defined(__POWERPC__)
//        {
//            string ExeInputStr = argv[5];
//            snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
//        }
//#endif
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
        string NewEncFile = WorkingDirString;
        string OldEncFile = WorkingDirString;
        string ParFile = WorkingDirString;
        string Program = ExeString;
        string FPF_Program = "";

        NewEncFile.append(slashCharStr);
        NewEncFile.append("outputVP8New.ivf");
        OldEncFile.append(slashCharStr);
        OldEncFile.append("outputVP8Old.ivf");
        ParFile.append(slashCharStr);
        ParFile.append("ParFile.txt");

#if defined(_WIN32)
        {
            Program.append(ExeInput);
            Program.append("\" \"");
            Program.append(input);
            Program.append("\" \"");
            Program.append(OldEncFile);
            Program.append("\" 8");
            Program.append(" \"");
            Program.append(ParFile);
            Program.append("\"");
            FPF_Program = Program;
            FPF_Program.append(" 0");
            FPF_Program.append("\"");
            Program.append(" 3");
            Program.append("\"");
        }
#else
        {
            Program.append(ExeInput);
            Program.append("\' \'");
            Program.append(input);
            Program.append("\' \'");
            Program.append(OldEncFile);
            Program.append("\' 8");
            Program.append(" \'");
            Program.append(ParFile);
            Program.append("\'");
            FPF_Program = Program;
            FPF_Program.append(" 0");
            Program.append(" 3");
        }
#endif

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
        opt.auto_key = 1;

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

                if (CompressIVFtoIVF(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

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

                if (CompressIVFtoIVF(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

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

                if (CompressIVFtoIVF(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);
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
                    opt.lag_in_frames = 10;

                    if (CompressIVFtoIVF(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    opt.Mode = MODE_FIRSTPASS;
                    OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        printf("Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    RunExe(FPF_Program);//this needs to be run to make this test compatable with old versions of vp8 that required the first pass to be called diffrently

                    opt.Mode = MODE_SECONDPASS;
                    OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

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

                    if (CompressIVFtoIVF(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    opt.Mode = MODE_SECONDPASS;
                    OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

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
                    opt.lag_in_frames = 10;

                    if (CompressIVFtoIVF(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    opt.Mode = 3;
                    OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        printf("Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    RunExe(FPF_Program);//this needs to be run to make this test compatable with old versions of vp8 that required the first pass to be called diffrently

                    opt.Mode = MODE_SECONDPASS_BEST;
                    OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

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

                    if (CompressIVFtoIVF(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    opt.Mode = MODE_SECONDPASS_BEST;
                    OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);

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

        if (FileSize2(OldEncFile.c_str()) == 0)
        {
            printf("\nError - Old File Incorrect\n");
            fprintf(stderr, "\nError - Old File Incorrect\n");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        //////////////////////////////////////////////Input OLD PSNR//////////////////////////////////////////////
        char TimeTextFile[256];

        FolderName(NewEncFile.c_str(), TimeTextFile);
        string TimeTextFileStr = TimeTextFile;

        TimeTextFileStr.append("outputVP8Old_OLD_PSNR.txt");

        float PSNROLD;
        ifstream infile(TimeTextFileStr.c_str());
        infile >> PSNROLD;
        infile.close();
        //////////////////////////////////////////////////////////////////////////////////////////////////////////

        double PSNRArr[2];

        PSNRArr[0] = IVFPSNR(input, NewEncFile.c_str(), 0, 0, 1, NULL);
        PSNRArr[1] = PSNROLD;

        printf("\nNew DataRate");
        fprintf(stderr, "\nNew DataRate\n");
        IVFDataRate(NewEncFile.c_str(), 1);

        printf("\nOld DataRate");
        fprintf(stderr, "\nOld DataRate\n");
        IVFDataRate(OldEncFile.c_str(), 1);

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

        string outputVP8New = WorkingDirString;
        string outputVP8Old = WorkingDirString;
        string outputVP8Old2 = WorkingDirString;
        string ParFile = WorkingDirString;
        string Program = ExeString;

        outputVP8New.append(slashCharStr);
        outputVP8New.append("outputVP8New.ivf");
        outputVP8Old.append(slashCharStr);
        outputVP8Old.append("outputVP8Old.ivf");
        outputVP8Old2.append(slashCharStr);
        outputVP8Old2.append("outputVP8Old.ivf");
        ParFile.append(slashCharStr);
        ParFile.append("ParFile.txt");
        Program.append(ExeInput);

#if defined(_WIN32)
        {
            Program.insert(0, "\"");
            Program.insert(0, "\"");
            Program.append("\" \"");
            Program.append(input);
            Program.append("\" \"");
            Program.append(outputVP8Old);
            Program.append("\" 8");
            Program.append(" \"");
            Program.append(ParFile);
            Program.append("\"");
            Program.append(" 2");
            Program.append("\"");
        }
#else
        {
            Program.insert(0, "\'");
            Program.append("\' \'");
            Program.append(input);
            Program.append("\' \'");
            Program.append(outputVP8Old);
            Program.append("\' 8");
            Program.append(" \'");
            Program.append(ParFile);
            Program.append("\'");
            Program.append(" 2");
        }
#endif

        //char outputVP8New[255];
        //char outputVP8Old[255];
        //char outputVP8Old2[255];
        //char ParFile[255];
        //char Program[1024];

        //snprintf(outputVP8New, 255, "%s", WorkingDir4.c_str());
        //snprintf(outputVP8Old, 255, "%s", WorkingDir5.c_str());
        //snprintf(outputVP8Old2, 255, "%s", WorkingDir6.c_str());
        //snprintf(ParFile, 255, "%s", WorkingDir7.c_str());
        //snprintf(Program, 1024, "%s", WorkingDir8.c_str());

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


        unsigned int Time1 = 0;
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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
        opt.auto_key = 1;
        opt.cpu_used = -1;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            Time1 = TimeReturn(outputVP8New.c_str(), 0);
            Time2 = TimeReturn(outputVP8Old2.c_str(), 0);

            printf("\nTime1: %i\n", Time1);
            printf("\nTime2: %i\n", Time2);
            fprintf(stderr, "\nTime1: %i\n", Time1);
            fprintf(stderr, "\nTime2: %i\n", Time2);
        }
        else
        {

            opt.Mode = MODE_REALTIME;
            Time1 = TimeCompressIVFtoIVF(input, outputVP8New.c_str(), speed, BitRate, opt, "VP8", 0, 0);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            OutPutCompatSettings(ParFile.c_str(), opt, ParFileNum);
            printf("\nCompressing Old File\n");

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            RunExe(Program);
            Time2 = TimeReturn(outputVP8Old2.c_str(), 0);

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

        string NoiseSenseBase = WorkingDirString;
        NoiseSenseBase.append(slashCharStr);
        NoiseSenseBase.append("NoiseSenseOut");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;

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

                string NoiseSenseOut = NoiseSenseBase;
                NoiseSenseOut.append(num);
                NoiseSenseOut.append(".ivf");

                //char NoiseSenseOut[255];
                //snprintf(NoiseSenseOut, 255, "%s", WorkingDir5.c_str());

                if (doOnce == 1)
                {
                    doOnce = 0;
                }

                printf("\n");
                fprintf(stderr, "\n");

                PSNRArr[Noise] = IVFPSNR(input, NoiseSenseOut.c_str(), 0, 0, 1, NULL);
                printf("\n");
                fprintf(stderr, "\n");
                File2bytes[Noise] = FileSize(NoiseSenseOut.c_str());
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

                string NoiseSenseOut = NoiseSenseBase;
                NoiseSenseOut.append(num);
                NoiseSenseOut.append(".ivf");

                //char NoiseSenseOut[255];
                //snprintf(NoiseSenseOut, 255, "%s", WorkingDir5.c_str());

                if (Mode == 0)
                {
                    opt.Mode = MODE_REALTIME;
                    opt.noise_sensitivity = Noise;

                    if (CompressIVFtoIVF(input, NoiseSenseOut.c_str(), speed, BitRate, opt, CompressString, Noise, 0) == -1)
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
                    opt.noise_sensitivity = Noise;

                    if (CompressIVFtoIVF(input, NoiseSenseOut.c_str(), speed, BitRate, opt, CompressString, Noise, 0) == -1)
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
                    opt.noise_sensitivity = Noise;

                    if (CompressIVFtoIVF(input, NoiseSenseOut.c_str(), speed, BitRate, opt, CompressString, Noise, 0) == -1)
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
                    opt.noise_sensitivity = Noise;
                    opt.Mode = MODE_SECONDPASS;

                    if (CompressIVFtoIVF(input, NoiseSenseOut.c_str(), speed, BitRate, opt, CompressString, Noise, 0) == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }
                }

                if (Mode == 5)
                {
                    opt.noise_sensitivity = Noise;
                    opt.Mode = MODE_SECONDPASS_BEST;

                    if (CompressIVFtoIVF(input, NoiseSenseOut.c_str(), speed, BitRate, opt, CompressString, Noise, 0) == -1)
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

                    PSNRArr[Noise] = IVFPSNR(input, NoiseSenseOut.c_str(), 0, 0, 1, NULL);
                    printf("\n");
                    fprintf(stderr, "\n");
                    File2bytes[Noise] = FileSize(NoiseSenseOut.c_str());
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


        string TwoPassOutFile1 = WorkingDirString;
        string OnePassOutFile1 = WorkingDirString;
        string TwoPassOutFile2 = WorkingDirString;
        string OnePassOutFile2 = WorkingDirString;
        string TwoPassOutFile3 = WorkingDirString;
        string OnePassOutFile3 = WorkingDirString;

        TwoPassOutFile1.append(slashCharStr);
        TwoPassOutFile1.append("TwoPassOutput1.ivf");
        OnePassOutFile1.append(slashCharStr);
        OnePassOutFile1.append("OnePassOutput1.ivf");
        TwoPassOutFile2.append(slashCharStr);
        TwoPassOutFile2.append("TwoPassOutput2.ivf");
        OnePassOutFile2.append(slashCharStr);
        OnePassOutFile2.append("OnePassOutput2.ivf");
        TwoPassOutFile3.append(slashCharStr);
        TwoPassOutFile3.append("TwoPassOutput3.ivf");
        OnePassOutFile3.append(slashCharStr);
        OnePassOutFile3.append("OnePassOutput3.ivf");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        int CompressInt = opt.allow_df;

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
            opt.target_bandwidth = BitRate1;

            if (CompressIVFtoIVF(input, TwoPassOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate2;

            if (CompressIVFtoIVF(input, TwoPassOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate3;

            if (CompressIVFtoIVF(input, TwoPassOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = 2;
            opt.target_bandwidth = BitRate1;

            if (CompressIVFtoIVF(input, OnePassOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate2;

            if (CompressIVFtoIVF(input, OnePassOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate3;

            if (CompressIVFtoIVF(input, OnePassOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

        float SizeTwoPass1 = IVFDataRate(TwoPassOutFile1.c_str(), 1);
        float SizeOnePass1 = IVFDataRate(OnePassOutFile1.c_str(), 1);
        float SizeTwoPass2 = IVFDataRate(TwoPassOutFile2.c_str(), 1);
        float SizeOnePass2 = IVFDataRate(OnePassOutFile2.c_str(), 1);
        float SizeTwoPass3 = IVFDataRate(TwoPassOutFile3.c_str(), 1);
        float SizeOnePass3 = IVFDataRate(OnePassOutFile3.c_str(), 1);

        double PSNRTwoPass1;
        double PSNRTwoPass2;
        double PSNRTwoPass3;

        double PSNROnePass1;
        double PSNROnePass2;
        double PSNROnePass3;

        PSNRTwoPass1 = IVFPSNR(input, TwoPassOutFile1.c_str(), 1, 0, 1, NULL);
        PSNRTwoPass2 = IVFPSNR(input, TwoPassOutFile2.c_str(), 1, 0, 1, NULL);
        PSNRTwoPass3 = IVFPSNR(input, TwoPassOutFile3.c_str(), 1, 0, 1, NULL);
        PSNROnePass1 = IVFPSNR(input, OnePassOutFile1.c_str(), 1, 0, 1, NULL);
        PSNROnePass2 = IVFPSNR(input, OnePassOutFile2.c_str(), 1, 0, 1, NULL);
        PSNROnePass3 = IVFPSNR(input, OnePassOutFile3.c_str(), 1, 0, 1, NULL);

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

        string PlayAlternate1 = WorkingDirString;
        string PlayAlternate2 = WorkingDirString;

        PlayAlternate1.append(slashCharStr);
        PlayAlternate1.append("PlayAlternate1Output.ivf");
        PlayAlternate2.append(slashCharStr);
        PlayAlternate2.append("PlayAlternate2Output.ivf");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
        opt.allow_lag = 1;
        opt.lag_in_frames = 10;
        opt.end_usage = 1;

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
                opt.play_alternate = PlayAlternate1Val;

                if (CompressIVFtoIVF(input, PlayAlternate1.c_str(), speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.play_alternate = PlayAlternate2Val;

                if (CompressIVFtoIVF(input, PlayAlternate2.c_str(), speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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
                opt.play_alternate = PlayAlternate1Val;

                if (CompressIVFtoIVF(input, PlayAlternate1.c_str(), speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.play_alternate = PlayAlternate2Val;

                if (CompressIVFtoIVF(input, PlayAlternate2.c_str(), speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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
                opt.play_alternate = PlayAlternate1Val;

                if (CompressIVFtoIVF(input, PlayAlternate1.c_str(), speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.play_alternate = PlayAlternate2Val;

                if (CompressIVFtoIVF(input, PlayAlternate2.c_str(), speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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
                opt.play_alternate = PlayAlternate1Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, PlayAlternate1.c_str(), speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.play_alternate = PlayAlternate2Val;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, PlayAlternate2.c_str(), speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.play_alternate = PlayAlternate1Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, PlayAlternate1.c_str(), speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.play_alternate = PlayAlternate2Val;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, PlayAlternate2.c_str(), speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
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

        int lngRC = CompIVF(PlayAlternate2.c_str(), PlayAlternate1.c_str());

        int fail = 0;

        int PlayAlternateOnAltRefCount = IVFDisplayAltRefFrames(PlayAlternate2.c_str(), 1);
        int PlayAlternateOffAltRefCount = IVFDisplayAltRefFrames(PlayAlternate1.c_str(), 1);
        int VisibleFrameONCount = IVFDisplayVisibleFrames(PlayAlternate2.c_str(), 1);
        int VisibleFrameOFFCount = IVFDisplayVisibleFrames(PlayAlternate1.c_str(), 1);

        char PlayAlternateOnFilename[255];
        FileName(PlayAlternate2.c_str(), PlayAlternateOnFilename, 0);
        char PlayAlternateOffFilename[255];
        FileName(PlayAlternate1.c_str(), PlayAlternateOffFilename, 0);

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

        string PostProcOutFile = WorkingDirString;

        PostProcOutFile.append(slashCharStr);
        PostProcOutFile.append("PostProcOutput.ivf");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        int CompressInt = opt.allow_df;

        opt.target_bandwidth = BitRate;

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

                if (CompressIVFtoIVF(input, PostProcOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

                if (CompressIVFtoIVF(input, PostProcOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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

        int deblock_level = 0;
        int flags = 0;
        int noise_level = 0;
        int countme = 0;
        double ssim = 0;

        printf("\nCaculating PSNR: NOFILTERING DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        fprintf(stderr, "\nCaculating PSNR: ONYXD_NOFILTERING DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, 0, flags, &ssim);
        countme++;

        flags++;
        printf("\nCaculating PSNR: DEBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        fprintf(stderr, "\nCaculating PSNR: ONYXD_DEBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, noise_level, flags, &ssim);
        countme++;
        flags++;

        while (deblock_level != 16)
        {
            printf("\nCaculating PSNR: DEMACROBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
            fprintf(stderr, "\nCaculating PSNR: ONYXD_DEMACROBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
            PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, 0, flags, &ssim);
            countme++;
            deblock_level++;
        }

        flags = flags + 2;
        deblock_level = 0;
        noise_level = 0;

        while (noise_level != 8)
        {
            printf("\nCaculating PSNR: ADDNOISE DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
            fprintf(stderr, "\nCaculating PSNR: ADDNOISE DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
            PSNRArr[countme] = PostProcIVFPSNR(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, noise_level, flags, &ssim);
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
                snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE noise_level %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE noise_level %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);

            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Failed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE noise_level %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE noise_level %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
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
                snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f within 10%% of %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE noise_level %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE noise_level %*i PSNR: %4.2f within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);

            }
            else
            {
                char OutputChar1[255];
                snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f not within 10%% of %4.2f - Indeterminate", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                string OutputChar1str = OutputChar1;
                FormatedPrint(OutputChar1str, 5);
                printf("\n");
                fprintf(stderr, "\n");

                //printf("ADDNOISE noise_level %*i PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
                //fprintf(stderr, "ADDNOISE noise_level %*i PSNR: %4.2f not within 10%% of NOFILTERING PSNR: %4.2f\n", 2, x - 18, PSNRArr[x], PSNRArr[0]);
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

        string ReconBuffer = WorkingDirString;

        ReconBuffer.append(slashCharStr);
        ReconBuffer.append("ReconBuffer.ivf");


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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
        opt.lag_in_frames = 0;
        opt.play_alternate = 0;

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

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
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
                opt.allow_df = 0;

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
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
                opt.allow_df = 0;

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 4)
            {
                opt.allow_df = 0;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.allow_df = 0;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVFReconBufferCheck(input, ReconBuffer.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
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

        printf("\n\nResults:\n\n");
        fprintf(stderr, "\n\nResults:\n\n");

        int fail = 0;

        ifstream ReconOutFile;
        string ReconOutStr = ReconBuffer;
        ReconOutStr.erase(ReconOutStr.length() - 4, 4);
        ReconOutStr.append("_ReconFrameState.txt");

        ReconOutFile.open(ReconOutStr.c_str());

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

        string DownWaterSamp90OutFile = WorkingDirString;
        string DownWaterSamp10OutFile = WorkingDirString;

        DownWaterSamp90OutFile.append(slashCharStr);
        DownWaterSamp90OutFile.append("DownWaterSamp90Output.ivf");
        DownWaterSamp10OutFile.append(slashCharStr);
        DownWaterSamp10OutFile.append("DownWaterSamp10Output.ivf");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        opt.allow_lag = 0;
        opt.allow_spatial_resampling = 1;
        opt.end_usage = 0;
        opt.target_bandwidth = BitRate;

        int ResampleDownWaterMarkHigh = 90;
        int ResampleDownWaterMarkLow = 20;

        int n = 0;
        double PSNRArr[2];

        opt.resample_up_water_mark = 100;

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
                opt.resample_down_water_mark = ResampleDownWaterMarkHigh;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.resample_down_water_mark = ResampleDownWaterMarkLow;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
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
                opt.resample_down_water_mark = ResampleDownWaterMarkHigh;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.resample_down_water_mark = ResampleDownWaterMarkLow;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
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
                opt.resample_down_water_mark = ResampleDownWaterMarkHigh;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.resample_down_water_mark = ResampleDownWaterMarkLow;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
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
                opt.resample_down_water_mark = ResampleDownWaterMarkHigh;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.resample_down_water_mark = ResampleDownWaterMarkLow;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.resample_down_water_mark = ResampleDownWaterMarkHigh;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, DownWaterSamp90OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.resample_down_water_mark = ResampleDownWaterMarkLow;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, DownWaterSamp10OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
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

        char DownWaterSamp10Filename[255];
        FileName(DownWaterSamp10OutFile.c_str(), DownWaterSamp10Filename, 0);
        char DownWaterSamp90Filename[255];
        FileName(DownWaterSamp90OutFile.c_str(), DownWaterSamp90Filename, 0);

        int DispKeyFrames10int = IVFDisplayKeyFrames(DownWaterSamp10OutFile.c_str(), 1);
        int DispResized10int = IVFDisplayResizedFrames(DownWaterSamp10OutFile.c_str(), 1);
        int DispCheckPBMThr10int = IVFCheckPBMThreshold(DownWaterSamp10OutFile.c_str(), opt.target_bandwidth, opt.maximum_buffer_size * 1000, opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000, ResampleDownWaterMarkLow);
        int RDWMCheck10int = 0;

        if (DispResized10int > 0)
        {
            printf("\nChecking %s buffer for correct resize frame placement:\n\n", DownWaterSamp10Filename);
            RDWMCheck10int = IVFDFWMCheck(DownWaterSamp10OutFile.c_str(), 1);
        }
        else
        {
            RDWMCheck10int = -2;
        }

        int DispKeyFrames90int = IVFDisplayKeyFrames(DownWaterSamp90OutFile.c_str(), 1);
        int DispResized90int = IVFDisplayResizedFrames(DownWaterSamp90OutFile.c_str(), 1);
        int DispCheckPBMThr90int = IVFCheckPBMThreshold(DownWaterSamp90OutFile.c_str(), opt.target_bandwidth, opt.maximum_buffer_size * 1000, opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000, ResampleDownWaterMarkHigh);
        int RDWMCheck90int = -3;

        if (DispResized90int > 0)
        {
            printf("\nChecking %s buffer for correct resize frame placement:\n\n", DownWaterSamp90Filename);
            RDWMCheck90int = IVFDFWMCheck(DownWaterSamp90OutFile.c_str(), 1);
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

        string SpeedTestGoodQBase = WorkingDirString;
        string SpeedTestRealTimeBase = WorkingDirString;

        SpeedTestGoodQBase.append(slashCharStr);
        SpeedTestGoodQBase.append("SpeedTestGoodQ_CpuUsed");
        SpeedTestRealTimeBase.append(slashCharStr);
        SpeedTestRealTimeBase.append("SpeedTestRealTime_CpuUsed");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        int CompressInt = opt.cpu_used;
        opt.target_bandwidth = BitRate;
        opt.Mode = MODE_GOODQUALITY;
        opt.lag_in_frames = LagInFramesInput;

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

                    string SpeedTestGoodQ = SpeedTestGoodQBase;
                    SpeedTestGoodQ.append(CounterChar);
                    SpeedTestGoodQ.append(".ivf");

                    //char SpeedTestGoodQ[255];
                    //snprintf(SpeedTestGoodQ, 255, "%s", OutPutFile1.c_str());

                    GoodTotalms[counter] = TimeReturn(SpeedTestGoodQ.c_str(), 0);
                    GoodPSNRArr[counter] = IVFPSNR(input, SpeedTestGoodQ.c_str(), 1, 0, 1, NULL);
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

                    string SpeedTestRealTime = SpeedTestRealTimeBase;
                    SpeedTestRealTime.append(CounterChar);
                    SpeedTestRealTime.append(".ivf");

                    //char SpeedTestRealTime[255];
                    //snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                    RealTotalms[counter2] = TimeReturn(SpeedTestRealTime.c_str(), 0);
                    RealPSNRArr[counter2] = IVFPSNR(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
                    counter--;
                    counter2++;
                }

                counter = 0;

                while (counter < 17)
                {
                    char CounterChar[4];
                    itoa_custom(counter, CounterChar, 10);

                    string SpeedTestRealTime = SpeedTestRealTimeBase;
                    SpeedTestRealTime.append(CounterChar);
                    SpeedTestRealTime.append(".ivf");

                    //char SpeedTestRealTime[255];
                    //snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                    RealTotalmsPos[counter] = TimeReturn(SpeedTestRealTime.c_str(), 0);
                    RealPSNRArrPos[counter] = IVFPSNR(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
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

                    string SpeedTestGoodQ = SpeedTestGoodQBase;
                    SpeedTestGoodQ.append(CounterChar);
                    SpeedTestGoodQ.append(".ivf");

                    //char SpeedTestGoodQ[255];
                    //snprintf(SpeedTestGoodQ, 255, "%s", OutPutFile1.c_str());

                    opt.cpu_used = counter;
                    CompressInt = opt.cpu_used;
                    opt.Mode = MODE_GOODQUALITY;
                    GoodTotalms[counter] = TimeCompressIVFtoIVF(input, SpeedTestGoodQ.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

                    if (GoodTotalms[counter] == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    if (TestType != 2 && TestType != 3)
                    {
                        GoodPSNRArr[counter] = IVFPSNR(input, SpeedTestGoodQ.c_str(), 1, 0, 1, NULL);
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

                    string SpeedTestRealTime = SpeedTestRealTimeBase;
                    SpeedTestRealTime.append(CounterChar);
                    SpeedTestRealTime.append(".ivf");

                    //char SpeedTestRealTime[255];
                    //snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                    opt.cpu_used = counter;
                    CompressInt = opt.cpu_used;
                    opt.Mode = MODE_REALTIME;
                    RealTotalms[counter2] = TimeCompressIVFtoIVF(input, SpeedTestRealTime.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

                    if (TestType != 2 && TestType != 3)
                    {
                        RealPSNRArr[counter2] = IVFPSNR(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
                    }

                    counter--;
                    counter2++;
                }

                counter = 0;

                while (counter < 17)
                {
                    char CounterChar[4];
                    itoa_custom(counter, CounterChar, 10);

                    string SpeedTestRealTime = SpeedTestRealTimeBase;
                    SpeedTestRealTime.append(CounterChar);
                    SpeedTestRealTime.append(".ivf");

                    //char SpeedTestRealTime[255];
                    //snprintf(SpeedTestRealTime, 255, "%s", OutPutFile2.c_str());

                    opt.cpu_used = counter;
                    CompressInt = opt.cpu_used;
                    opt.Mode = MODE_REALTIME;
                    RealTotalmsPos[counter] = TimeCompressIVFtoIVF(input, SpeedTestRealTime.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0);

                    if (RealTotalmsPos[counter] == -1)
                    {
                        fclose(fp);
                        string File1Str = File1;
                        RecordTestComplete(MainDirString, File1Str, TestType);
                        return 2;
                    }

                    if (TestType != 2 && TestType != 3)
                    {
                        RealPSNRArrPos[counter] = IVFPSNR(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
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

            printf("\n\n");
            fprintf(stderr, "\n\n");

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
        int CurTestVector = 1;
        int LastTestVector = 102;

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

        string TestVectorFolder = argv[2];
        string TestVectorOutFolder = WorkingDirString.c_str();
        //TestVectorOutFolder.erase(TestVectorOutFolder.end() - 1);

        int TestVectorNum = 0;
        string TestVector[102];

        while (TestVectorNum < 102)
        {
            TestVector[TestVectorNum] = TestVectorFolder;
            TestVectorNum++;
        }

        TestVectorNum = 0;
        string TestVector_Raw[102];

        while (TestVectorNum < 102)
        {
            TestVector_Raw[TestVectorNum] = (TestVectorOutFolder.c_str());
            TestVectorNum++;
        }

        TestVector[1].append(slashCharStr);
        TestVector[1].append("vp80-00-comprehensive-001.ivf");
        TestVector[2].append(slashCharStr);
        TestVector[2].append("vp80-00-comprehensive-002.ivf");
        TestVector[3].append(slashCharStr);
        TestVector[3].append("vp80-00-comprehensive-003.ivf");
        TestVector[4].append(slashCharStr);
        TestVector[4].append("vp80-00-comprehensive-004.ivf");
        TestVector[5].append(slashCharStr);
        TestVector[5].append("vp80-00-comprehensive-005.ivf");
        TestVector[6].append(slashCharStr);
        TestVector[6].append("vp80-00-comprehensive-006.ivf");
        TestVector[7].append(slashCharStr);
        TestVector[7].append("vp80-00-comprehensive-007.ivf");
        TestVector[8].append(slashCharStr);
        TestVector[8].append("vp80-00-comprehensive-008.ivf");
        TestVector[9].append(slashCharStr);
        TestVector[9].append("vp80-00-comprehensive-009.ivf");
        TestVector[10].append(slashCharStr);
        TestVector[10].append("vp80-00-comprehensive-010.ivf");
        TestVector[11].append(slashCharStr);
        TestVector[11].append("vp80-00-comprehensive-011.ivf");
        TestVector[12].append(slashCharStr);
        TestVector[12].append("vp80-00-comprehensive-012.ivf");
        TestVector[13].append(slashCharStr);
        TestVector[13].append("vp80-00-comprehensive-013.ivf");
        TestVector[14].append(slashCharStr);
        TestVector[14].append("vp80-00-comprehensive-014.ivf");
        TestVector[15].append(slashCharStr);
        TestVector[15].append("vp80-00-comprehensive-015.ivf");
        TestVector[16].append(slashCharStr);
        TestVector[16].append("vp80-00-comprehensive-016.ivf");
        TestVector[17].append(slashCharStr);
        TestVector[17].append("vp80-00-comprehensive-017.ivf");
        TestVector[18].append(slashCharStr);
        TestVector[18].append("vp80-01-intra-1400.ivf");
        TestVector[19].append(slashCharStr);
        TestVector[19].append("vp80-01-intra-1411.ivf");
        TestVector[20].append(slashCharStr);
        TestVector[20].append("vp80-01-intra-1416.ivf");
        TestVector[21].append(slashCharStr);
        TestVector[21].append("vp80-01-intra-1417.ivf");
        TestVector[22].append(slashCharStr);
        TestVector[22].append("vp80-02-inter-1402.ivf");
        TestVector[23].append(slashCharStr);
        TestVector[23].append("vp80-02-inter-1412.ivf");
        TestVector[24].append(slashCharStr);
        TestVector[24].append("vp80-02-inter-1418.ivf");
        TestVector[25].append(slashCharStr);
        TestVector[25].append("vp80-02-inter-1424.ivf");
        TestVector[26].append(slashCharStr);
        TestVector[26].append("vp80-03-segmentation-1401.ivf");
        TestVector[27].append(slashCharStr);
        TestVector[27].append("vp80-03-segmentation-1403.ivf");
        TestVector[28].append(slashCharStr);
        TestVector[28].append("vp80-03-segmentation-1407.ivf");
        TestVector[29].append(slashCharStr);
        TestVector[29].append("vp80-03-segmentation-1408.ivf");
        TestVector[30].append(slashCharStr);
        TestVector[30].append("vp80-03-segmentation-1409.ivf");
        TestVector[31].append(slashCharStr);
        TestVector[31].append("vp80-03-segmentation-1410.ivf");
        TestVector[32].append(slashCharStr);
        TestVector[32].append("vp80-03-segmentation-1413.ivf");
        TestVector[33].append(slashCharStr);
        TestVector[33].append("vp80-03-segmentation-1414.ivf");
        TestVector[34].append(slashCharStr);
        TestVector[34].append("vp80-03-segmentation-1415.ivf");
        TestVector[35].append(slashCharStr);
        TestVector[35].append("vp80-03-segmentation-1425.ivf");
        TestVector[36].append(slashCharStr);
        TestVector[36].append("vp80-03-segmentation-1426.ivf");
        TestVector[37].append(slashCharStr);
        TestVector[37].append("vp80-03-segmentation-1427.ivf");
        TestVector[38].append(slashCharStr);
        TestVector[38].append("vp80-03-segmentation-1432.ivf");
        TestVector[39].append(slashCharStr);
        TestVector[39].append("vp80-03-segmentation-1435.ivf");
        TestVector[40].append(slashCharStr);
        TestVector[40].append("vp80-03-segmentation-1436.ivf");
        TestVector[41].append(slashCharStr);
        TestVector[41].append("vp80-03-segmentation-1437.ivf");
        TestVector[42].append(slashCharStr);
        TestVector[42].append("vp80-03-segmentation-1441.ivf");
        TestVector[43].append(slashCharStr);
        TestVector[43].append("vp80-03-segmentation-1442.ivf");
        TestVector[44].append(slashCharStr);
        TestVector[44].append("vp80-04-partitions-1404.ivf");
        TestVector[45].append(slashCharStr);
        TestVector[45].append("vp80-04-partitions-1405.ivf");
        TestVector[46].append(slashCharStr);
        TestVector[46].append("vp80-04-partitions-1406.ivf");
        TestVector[47].append(slashCharStr);
        TestVector[47].append("vp80-05-sharpness-1428.ivf");
        TestVector[48].append(slashCharStr);
        TestVector[48].append("vp80-05-sharpness-1429.ivf");
        TestVector[49].append(slashCharStr);
        TestVector[49].append("vp80-05-sharpness-1430.ivf");
        TestVector[50].append(slashCharStr);
        TestVector[50].append("vp80-05-sharpness-1431.ivf");
        TestVector[51].append(slashCharStr);
        TestVector[51].append("vp80-05-sharpness-1433.ivf");
        TestVector[52].append(slashCharStr);
        TestVector[52].append("vp80-05-sharpness-1434.ivf");
        TestVector[53].append(slashCharStr);
        TestVector[53].append("vp80-05-sharpness-1438.ivf");
        TestVector[54].append(slashCharStr);
        TestVector[54].append("vp80-05-sharpness-1439.ivf");
        TestVector[55].append(slashCharStr);
        TestVector[55].append("vp80-05-sharpness-1440.ivf");
        TestVector[56].append(slashCharStr);
        TestVector[56].append("vp80-05-sharpness-1443.ivf");
        TestVector[57].append(slashCharStr);
        TestVector[57].append("vp80-06-cropping-001.ivf");
        TestVector[58].append(slashCharStr);
        TestVector[58].append("vp80-06-cropping-002.ivf");
        TestVector[59].append(slashCharStr);
        TestVector[59].append("vp80-06-cropping-003.ivf");
        TestVector[60].append(slashCharStr);
        TestVector[60].append("vp80-06-cropping-004.ivf");
        TestVector[61].append(slashCharStr);
        TestVector[61].append("vp80-06-cropping-005.ivf");
        TestVector[62].append(slashCharStr);
        TestVector[62].append("vp80-06-cropping-006.ivf");
        TestVector[63].append(slashCharStr);
        TestVector[63].append("vp80-06-cropping-007.ivf");
        TestVector[64].append(slashCharStr);
        TestVector[64].append("vp80-06-cropping-008.ivf");
        TestVector[65].append(slashCharStr);
        TestVector[65].append("vp80-06-cropping-009.ivf");
        TestVector[66].append(slashCharStr);
        TestVector[66].append("vp80-06-cropping-010.ivf");
        TestVector[67].append(slashCharStr);
        TestVector[67].append("vp80-06-cropping-011.ivf");
        TestVector[68].append(slashCharStr);
        TestVector[68].append("vp80-06-cropping-012.ivf");
        TestVector[69].append(slashCharStr);
        TestVector[69].append("vp80-06-cropping-013.ivf");
        TestVector[70].append(slashCharStr);
        TestVector[70].append("vp80-06-cropping-014.ivf");
        TestVector[71].append(slashCharStr);
        TestVector[71].append("vp80-06-cropping-015.ivf");
        TestVector[72].append(slashCharStr);
        TestVector[72].append("vp80-06-cropping-016.ivf");
        TestVector[73].append(slashCharStr);
        TestVector[73].append("vp80-06-cropping-017.ivf");
        TestVector[74].append(slashCharStr);
        TestVector[74].append("vp80-06-cropping-018.ivf");
        TestVector[75].append(slashCharStr);
        TestVector[75].append("vp80-06-cropping-019.ivf");
        TestVector[76].append(slashCharStr);
        TestVector[76].append("vp80-06-cropping-020.ivf");
        TestVector[77].append(slashCharStr);
        TestVector[77].append("vp80-06-cropping-021.ivf");
        TestVector[78].append(slashCharStr);
        TestVector[78].append("vp80-06-cropping-022.ivf");
        TestVector[79].append(slashCharStr);
        TestVector[79].append("vp80-06-cropping-023.ivf");
        TestVector[80].append(slashCharStr);
        TestVector[80].append("vp80-06-cropping-024.ivf");
        TestVector[81].append(slashCharStr);
        TestVector[81].append("vp80-06-cropping-025.ivf");
        TestVector[82].append(slashCharStr);
        TestVector[82].append("vp80-06-cropping-026.ivf");
        TestVector[83].append(slashCharStr);
        TestVector[83].append("vp80-06-cropping-027.ivf");
        TestVector[84].append(slashCharStr);
        TestVector[84].append("vp80-06-cropping-028.ivf");
        TestVector[85].append(slashCharStr);
        TestVector[85].append("vp80-06-cropping-029.ivf");
        TestVector[86].append(slashCharStr);
        TestVector[86].append("vp80-06-cropping-030.ivf");
        TestVector[87].append(slashCharStr);
        TestVector[87].append("vp80-06-cropping-031.ivf");
        TestVector[88].append(slashCharStr);
        TestVector[88].append("vp80-06-cropping-032.ivf");
        TestVector[89].append(slashCharStr);
        TestVector[89].append("vp80-06-cropping-033.ivf");
        TestVector[90].append(slashCharStr);
        TestVector[90].append("vp80-06-cropping-034.ivf");
        TestVector[91].append(slashCharStr);
        TestVector[91].append("vp80-06-cropping-035.ivf");
        TestVector[92].append(slashCharStr);
        TestVector[92].append("vp80-06-cropping-036.ivf");
        TestVector[93].append(slashCharStr);
        TestVector[93].append("vp80-06-cropping-037.ivf");
        TestVector[94].append(slashCharStr);
        TestVector[94].append("vp80-06-cropping-038.ivf");
        TestVector[95].append(slashCharStr);
        TestVector[95].append("vp80-06-cropping-039.ivf");
        TestVector[96].append(slashCharStr);
        TestVector[96].append("vp80-06-cropping-040.ivf");
        TestVector[97].append(slashCharStr);
        TestVector[97].append("vp80-06-cropping-041.ivf");
        TestVector[98].append(slashCharStr);
        TestVector[98].append("vp80-06-cropping-042.ivf");
        TestVector[99].append(slashCharStr);
        TestVector[99].append("vp80-06-cropping-043.ivf");
        TestVector[100].append(slashCharStr);
        TestVector[100].append("vp80-06-cropping-044.ivf");
        TestVector[101].append(slashCharStr);
        TestVector[101].append("vp80-06-cropping-045.ivf");

        TestVector_Raw[1].append(slashCharStr);
        TestVector_Raw[1].append("vp80-00-comprehensive-001.raw");
        TestVector_Raw[2].append(slashCharStr);
        TestVector_Raw[2].append("vp80-00-comprehensive-002.raw");
        TestVector_Raw[3].append(slashCharStr);
        TestVector_Raw[3].append("vp80-00-comprehensive-003.raw");
        TestVector_Raw[4].append(slashCharStr);
        TestVector_Raw[4].append("vp80-00-comprehensive-004.raw");
        TestVector_Raw[5].append(slashCharStr);
        TestVector_Raw[5].append("vp80-00-comprehensive-005.raw");
        TestVector_Raw[6].append(slashCharStr);
        TestVector_Raw[6].append("vp80-00-comprehensive-006.raw");
        TestVector_Raw[7].append(slashCharStr);
        TestVector_Raw[7].append("vp80-00-comprehensive-007.raw");
        TestVector_Raw[8].append(slashCharStr);
        TestVector_Raw[8].append("vp80-00-comprehensive-008.raw");
        TestVector_Raw[9].append(slashCharStr);
        TestVector_Raw[9].append("vp80-00-comprehensive-009.raw");
        TestVector_Raw[10].append(slashCharStr);
        TestVector_Raw[10].append("vp80-00-comprehensive-010.raw");
        TestVector_Raw[11].append(slashCharStr);
        TestVector_Raw[11].append("vp80-00-comprehensive-011.raw");
        TestVector_Raw[12].append(slashCharStr);
        TestVector_Raw[12].append("vp80-00-comprehensive-012.raw");
        TestVector_Raw[13].append(slashCharStr);
        TestVector_Raw[13].append("vp80-00-comprehensive-013.raw");
        TestVector_Raw[14].append(slashCharStr);
        TestVector_Raw[14].append("vp80-00-comprehensive-014.raw");
        TestVector_Raw[15].append(slashCharStr);
        TestVector_Raw[15].append("vp80-00-comprehensive-015.raw");
        TestVector_Raw[16].append(slashCharStr);
        TestVector_Raw[16].append("vp80-00-comprehensive-016.raw");
        TestVector_Raw[17].append(slashCharStr);
        TestVector_Raw[17].append("vp80-00-comprehensive-017.raw");
        TestVector_Raw[18].append(slashCharStr);
        TestVector_Raw[18].append("vp80-01-intra-1400.raw");
        TestVector_Raw[19].append(slashCharStr);
        TestVector_Raw[19].append("vp80-01-intra-1411.raw");
        TestVector_Raw[20].append(slashCharStr);
        TestVector_Raw[20].append("vp80-01-intra-1416.raw");
        TestVector_Raw[21].append(slashCharStr);
        TestVector_Raw[21].append("vp80-01-intra-1417.raw");
        TestVector_Raw[22].append(slashCharStr);
        TestVector_Raw[22].append("vp80-02-inter-1402.raw");
        TestVector_Raw[23].append(slashCharStr);
        TestVector_Raw[23].append("vp80-02-inter-1412.raw");
        TestVector_Raw[24].append(slashCharStr);
        TestVector_Raw[24].append("vp80-02-inter-1418.raw");
        TestVector_Raw[25].append(slashCharStr);
        TestVector_Raw[25].append("vp80-02-inter-1424.raw");
        TestVector_Raw[26].append(slashCharStr);
        TestVector_Raw[26].append("vp80-03-segmentation-1401.raw");
        TestVector_Raw[27].append(slashCharStr);
        TestVector_Raw[27].append("vp80-03-segmentation-1403.raw");
        TestVector_Raw[28].append(slashCharStr);
        TestVector_Raw[28].append("vp80-03-segmentation-1407.raw");
        TestVector_Raw[29].append(slashCharStr);
        TestVector_Raw[29].append("vp80-03-segmentation-1408.raw");
        TestVector_Raw[30].append(slashCharStr);
        TestVector_Raw[30].append("vp80-03-segmentation-1409.raw");
        TestVector_Raw[31].append(slashCharStr);
        TestVector_Raw[31].append("vp80-03-segmentation-1410.raw");
        TestVector_Raw[32].append(slashCharStr);
        TestVector_Raw[32].append("vp80-03-segmentation-1413.raw");
        TestVector_Raw[33].append(slashCharStr);
        TestVector_Raw[33].append("vp80-03-segmentation-1414.raw");
        TestVector_Raw[34].append(slashCharStr);
        TestVector_Raw[34].append("vp80-03-segmentation-1415.raw");
        TestVector_Raw[35].append(slashCharStr);
        TestVector_Raw[35].append("vp80-03-segmentation-1425.raw");
        TestVector_Raw[36].append(slashCharStr);
        TestVector_Raw[36].append("vp80-03-segmentation-1426.raw");
        TestVector_Raw[37].append(slashCharStr);
        TestVector_Raw[37].append("vp80-03-segmentation-1427.raw");
        TestVector_Raw[38].append(slashCharStr);
        TestVector_Raw[38].append("vp80-03-segmentation-1432.raw");
        TestVector_Raw[39].append(slashCharStr);
        TestVector_Raw[39].append("vp80-03-segmentation-1435.raw");
        TestVector_Raw[40].append(slashCharStr);
        TestVector_Raw[40].append("vp80-03-segmentation-1436.raw");
        TestVector_Raw[41].append(slashCharStr);
        TestVector_Raw[41].append("vp80-03-segmentation-1437.raw");
        TestVector_Raw[42].append(slashCharStr);
        TestVector_Raw[42].append("vp80-03-segmentation-1441.raw");
        TestVector_Raw[43].append(slashCharStr);
        TestVector_Raw[43].append("vp80-03-segmentation-1442.raw");
        TestVector_Raw[44].append(slashCharStr);
        TestVector_Raw[44].append("vp80-04-partitions-1404.raw");
        TestVector_Raw[45].append(slashCharStr);
        TestVector_Raw[45].append("vp80-04-partitions-1405.raw");
        TestVector_Raw[46].append(slashCharStr);
        TestVector_Raw[46].append("vp80-04-partitions-1406.raw");
        TestVector_Raw[47].append(slashCharStr);
        TestVector_Raw[47].append("vp80-05-sharpness-1428.raw");
        TestVector_Raw[48].append(slashCharStr);
        TestVector_Raw[48].append("vp80-05-sharpness-1429.raw");
        TestVector_Raw[49].append(slashCharStr);
        TestVector_Raw[49].append("vp80-05-sharpness-1430.raw");
        TestVector_Raw[50].append(slashCharStr);
        TestVector_Raw[50].append("vp80-05-sharpness-1431.raw");
        TestVector_Raw[51].append(slashCharStr);
        TestVector_Raw[51].append("vp80-05-sharpness-1433.raw");
        TestVector_Raw[52].append(slashCharStr);
        TestVector_Raw[52].append("vp80-05-sharpness-1434.raw");
        TestVector_Raw[53].append(slashCharStr);
        TestVector_Raw[53].append("vp80-05-sharpness-1438.raw");
        TestVector_Raw[54].append(slashCharStr);
        TestVector_Raw[54].append("vp80-05-sharpness-1439.raw");
        TestVector_Raw[55].append(slashCharStr);
        TestVector_Raw[55].append("vp80-05-sharpness-1440.raw");
        TestVector_Raw[56].append(slashCharStr);
        TestVector_Raw[56].append("vp80-05-sharpness-1443.raw");
        TestVector_Raw[57].append(slashCharStr);
        TestVector_Raw[57].append("vp80-06-cropping-001.raw");
        TestVector_Raw[58].append(slashCharStr);
        TestVector_Raw[58].append("vp80-06-cropping-002.raw");
        TestVector_Raw[59].append(slashCharStr);
        TestVector_Raw[59].append("vp80-06-cropping-003.raw");
        TestVector_Raw[60].append(slashCharStr);
        TestVector_Raw[60].append("vp80-06-cropping-004.raw");
        TestVector_Raw[61].append(slashCharStr);
        TestVector_Raw[61].append("vp80-06-cropping-005.raw");
        TestVector_Raw[62].append(slashCharStr);
        TestVector_Raw[62].append("vp80-06-cropping-006.raw");
        TestVector_Raw[63].append(slashCharStr);
        TestVector_Raw[63].append("vp80-06-cropping-007.raw");
        TestVector_Raw[64].append(slashCharStr);
        TestVector_Raw[64].append("vp80-06-cropping-008.raw");
        TestVector_Raw[65].append(slashCharStr);
        TestVector_Raw[65].append("vp80-06-cropping-009.raw");
        TestVector_Raw[66].append(slashCharStr);
        TestVector_Raw[66].append("vp80-06-cropping-010.raw");
        TestVector_Raw[67].append(slashCharStr);
        TestVector_Raw[67].append("vp80-06-cropping-011.raw");
        TestVector_Raw[68].append(slashCharStr);
        TestVector_Raw[68].append("vp80-06-cropping-012.raw");
        TestVector_Raw[69].append(slashCharStr);
        TestVector_Raw[69].append("vp80-06-cropping-013.raw");
        TestVector_Raw[70].append(slashCharStr);
        TestVector_Raw[70].append("vp80-06-cropping-014.raw");
        TestVector_Raw[71].append(slashCharStr);
        TestVector_Raw[71].append("vp80-06-cropping-015.raw");
        TestVector_Raw[72].append(slashCharStr);
        TestVector_Raw[72].append("vp80-06-cropping-016.raw");
        TestVector_Raw[73].append(slashCharStr);
        TestVector_Raw[73].append("vp80-06-cropping-017.raw");
        TestVector_Raw[74].append(slashCharStr);
        TestVector_Raw[74].append("vp80-06-cropping-018.raw");
        TestVector_Raw[75].append(slashCharStr);
        TestVector_Raw[75].append("vp80-06-cropping-019.raw");
        TestVector_Raw[76].append(slashCharStr);
        TestVector_Raw[76].append("vp80-06-cropping-020.raw");
        TestVector_Raw[77].append(slashCharStr);
        TestVector_Raw[77].append("vp80-06-cropping-021.raw");
        TestVector_Raw[78].append(slashCharStr);
        TestVector_Raw[78].append("vp80-06-cropping-022.raw");
        TestVector_Raw[79].append(slashCharStr);
        TestVector_Raw[79].append("vp80-06-cropping-023.raw");
        TestVector_Raw[80].append(slashCharStr);
        TestVector_Raw[80].append("vp80-06-cropping-024.raw");
        TestVector_Raw[81].append(slashCharStr);
        TestVector_Raw[81].append("vp80-06-cropping-025.raw");
        TestVector_Raw[82].append(slashCharStr);
        TestVector_Raw[82].append("vp80-06-cropping-026.raw");
        TestVector_Raw[83].append(slashCharStr);
        TestVector_Raw[83].append("vp80-06-cropping-027.raw");
        TestVector_Raw[84].append(slashCharStr);
        TestVector_Raw[84].append("vp80-06-cropping-028.raw");
        TestVector_Raw[85].append(slashCharStr);
        TestVector_Raw[85].append("vp80-06-cropping-029.raw");
        TestVector_Raw[86].append(slashCharStr);
        TestVector_Raw[86].append("vp80-06-cropping-030.raw");
        TestVector_Raw[87].append(slashCharStr);
        TestVector_Raw[87].append("vp80-06-cropping-031.raw");
        TestVector_Raw[88].append(slashCharStr);
        TestVector_Raw[88].append("vp80-06-cropping-032.raw");
        TestVector_Raw[89].append(slashCharStr);
        TestVector_Raw[89].append("vp80-06-cropping-033.raw");
        TestVector_Raw[90].append(slashCharStr);
        TestVector_Raw[90].append("vp80-06-cropping-034.raw");
        TestVector_Raw[91].append(slashCharStr);
        TestVector_Raw[91].append("vp80-06-cropping-035.raw");
        TestVector_Raw[92].append(slashCharStr);
        TestVector_Raw[92].append("vp80-06-cropping-036.raw");
        TestVector_Raw[93].append(slashCharStr);
        TestVector_Raw[93].append("vp80-06-cropping-037.raw");
        TestVector_Raw[94].append(slashCharStr);
        TestVector_Raw[94].append("vp80-06-cropping-038.raw");
        TestVector_Raw[95].append(slashCharStr);
        TestVector_Raw[95].append("vp80-06-cropping-039.raw");
        TestVector_Raw[96].append(slashCharStr);
        TestVector_Raw[96].append("vp80-06-cropping-040.raw");
        TestVector_Raw[97].append(slashCharStr);
        TestVector_Raw[97].append("vp80-06-cropping-041.raw");
        TestVector_Raw[98].append(slashCharStr);
        TestVector_Raw[98].append("vp80-06-cropping-042.raw");
        TestVector_Raw[99].append(slashCharStr);
        TestVector_Raw[99].append("vp80-06-cropping-043.raw");
        TestVector_Raw[100].append(slashCharStr);
        TestVector_Raw[100].append("vp80-06-cropping-044.raw");
        TestVector_Raw[101].append(slashCharStr);
        TestVector_Raw[101].append("vp80-06-cropping-045.raw");

        TestVectorNum = 0;
        string TestVector_Text[102];

        while (TestVectorNum < 102)
        {
            TestVector_Text[TestVectorNum] = TestVector_Raw[TestVectorNum];
            TestVector_Text[TestVectorNum].erase(TestVector_Text[TestVectorNum].end() - 4, TestVector_Text[TestVectorNum].end());
            TestVector_Text[TestVectorNum].append("_MD5.txt");
            TestVectorNum++;
        }

        ////print out the names above
        //CurTestVector = 1;
        //while(CurTestVector < LastTestVector)
        //{
        //  //printf("\n%s",TestVector[CurTestVector].c_str());
        //  CurTestVector++;
        //}

        //CurTestVector = 1;
        //while(CurTestVector < LastTestVector)
        //{
        //  //printf("\n%s",TestVector_Raw[CurTestVector].c_str());
        //  CurTestVector++;
        //}

        //CurTestVector = 1;
        //while(CurTestVector < LastTestVector)
        //{
        //  //printf("\n%s",TestVector_Text[CurTestVector].c_str());
        //  CurTestVector++;
        //}

        string MD5Key[101];

        //New Test Vectors 2.0.0 MD5 Checksums
        MD5Key[0].assign("fad126074e1bd5363d43b9d1cadddb71");   /*vp80-00-comprehensive-001.raw*/
        MD5Key[1].assign("182f03dd264ebac04e24c7c9499d7cdb");   /*vp80-00-comprehensive-002.raw*/
        MD5Key[2].assign("e5fe668b033900022c3eb0ba76a44bd1");   /*vp80-00-comprehensive-003.raw*/
        MD5Key[3].assign("95097ce9808c1d47e03f99c48ad111ec");   /*vp80-00-comprehensive-004.raw*/
        MD5Key[4].assign("0f469e4fd1dea533e5580688b2d242ff");   /*vp80-00-comprehensive-005.raw*/
        MD5Key[5].assign("2d5fa3ec2f88404ae7b305c1074036f4");   /*vp80-00-comprehensive-006.raw*/
        MD5Key[6].assign("92526913d89b6a9b00f2d602def08bce");   /*vp80-00-comprehensive-007.raw*/
        MD5Key[7].assign("bd4d46a9d14fe5a7fc9cfc8deac2d34c");   /*vp80-00-comprehensive-008.raw*/
        MD5Key[8].assign("19201a2d535bd82f41c1a5658def5379");   /*vp80-00-comprehensive-009.raw*/
        MD5Key[9].assign("61d05919a9883d9f215eb3f2db63eb13");   /*vp80-00-comprehensive-010.raw*/
        MD5Key[10].assign("1a0afe5e70512a03323a8f1176bcf022");  /*vp80-00-comprehensive-011.raw*/
        MD5Key[11].assign("4ea997c80dc2087e6deec81f1ecf6668");  /*vp80-00-comprehensive-012.raw*/
        MD5Key[12].assign("93169305d3054327be3cc074f0773a75");  /*vp80-00-comprehensive-013.raw*/
        MD5Key[13].assign("7280a64c51dfa557c1b9552dc1e1fbed");  /*vp80-00-comprehensive-014.raw*/
        MD5Key[14].assign("23b9cc582e344726e76cda092b416bcf");  /*vp80-00-comprehensive-015.raw*/
        MD5Key[15].assign("55e889d22f99718cf6936d55f8ade12b");  /*vp80-00-comprehensive-016.raw*/
        MD5Key[16].assign("95a68ffb228d1d8c6ee54f16a10fb9eb");  /*vp80-00-comprehensive-017.raw*/
        MD5Key[17].assign("53b08ac91398a5dd948434e41b31b47e");  /*vp80-01-intra-1400.raw*/
        MD5Key[18].assign("8fa1762329e65c97245393a933cd0f00");  /*vp80-01-intra-1411.raw*/
        MD5Key[19].assign("cffd1299fa7a0330264cb411d9482bb0");  /*vp80-01-intra-1416.raw*/
        MD5Key[20].assign("0e6c13a78a203d95fe12d206a432f642");  /*vp80-01-intra-1417.raw*/
        MD5Key[21].assign("184ee9c5cd6e32f2fe7b2f5a463d37b3");  /*vp80-02-inter-1402.raw*/
        MD5Key[22].assign("6928dc8e7886914b0ba5825518e54bd7");  /*vp80-02-inter-1412.raw*/
        MD5Key[23].assign("45302aa645ff5c139fe580ac30482245");  /*vp80-02-inter-1418.raw*/
        MD5Key[24].assign("4816cb607488b930ceadeb2cdb034c49");  /*vp80-02-inter-1424.raw*/
        MD5Key[25].assign("f7acb74e99528568714129e2994ceca5");  /*vp80-03-segmentation-1401.raw*/
        MD5Key[26].assign("4e651f545a21863e97547185f93dbd7b");  /*vp80-03-segmentation-1403.raw*/
        MD5Key[27].assign("fa76612d673cbfcb8ec58eda08400388");  /*vp80-03-segmentation-1407.raw*/
        MD5Key[28].assign("886f15167bbdd5ea6c099e8b74452c7c");  /*vp80-03-segmentation-1408.raw*/
        MD5Key[29].assign("780cc4d060eecec04c2746c98065ec6f");  /*vp80-03-segmentation-1409.raw*/
        MD5Key[30].assign("f3468778cd11642f095b4e5dcb19fbda");  /*vp80-03-segmentation-1410.raw*/
        MD5Key[31].assign("6a0564ccc1a655d929390a72ff558db9");  /*vp80-03-segmentation-1413.raw*/
        MD5Key[32].assign("0f887b4bc1bb0aae670c50c9b7f0142f");  /*vp80-03-segmentation-1414.raw*/
        MD5Key[33].assign("8b83e0a3ca0da9e8d7f47a06dc08e18b");  /*vp80-03-segmentation-1415.raw*/
        MD5Key[34].assign("96ffacf0c3eae59b58252be24a60e9b2");  /*vp80-03-segmentation-1425.raw*/
        MD5Key[35].assign("ab1062e4e45e6802d80313da52028af2");  /*vp80-03-segmentation-1426.raw*/
        MD5Key[36].assign("761c3d8e23314516592a1f876865c22a");  /*vp80-03-segmentation-1427.raw*/
        MD5Key[37].assign("c5a7776fdfe8908fcc64e58c317c8cf3");  /*vp80-03-segmentation-1432.raw*/
        MD5Key[38].assign("36a77df963d0d8c3bc098827be403bdb");  /*vp80-03-segmentation-1435.raw*/
        MD5Key[39].assign("bfd17a557ee1ba347c755a18ce5a64a6");  /*vp80-03-segmentation-1436.raw*/
        MD5Key[40].assign("876e7c782ee4dd23866498b794856fd1");  /*vp80-03-segmentation-1437.raw*/
        MD5Key[41].assign("d7a1e99d0ec80ac2b95cc7277bf4db3c");  /*vp80-03-segmentation-1441.raw*/
        MD5Key[42].assign("1a23409897f51ad2920d5ddb999eac71");  /*vp80-03-segmentation-1442.raw*/
        MD5Key[43].assign("27837df047de5b5ae2dc8f2e9d318cb3");  /*vp80-04-partitions-1404.raw*/
        MD5Key[44].assign("12fb1d187ee70738265d8f3a0a70ef26");  /*vp80-04-partitions-1405.raw*/
        MD5Key[45].assign("2da53f93a051dcb8290f884a55272dd9");  /*vp80-04-partitions-1406.raw*/
        MD5Key[46].assign("14b537dae40c6013079fd3d25cb16e7a");  /*vp80-05-sharpness-1428.raw*/
        MD5Key[47].assign("e836423126f8a7de2c6c9777e0a84214");  /*vp80-05-sharpness-1429.raw*/
        MD5Key[48].assign("51503f7a786032d2cbed84ed11430ff3");  /*vp80-05-sharpness-1430.raw*/
        MD5Key[49].assign("7f7f534b2d6e28002e119ed269c8f282");  /*vp80-05-sharpness-1431.raw*/
        MD5Key[50].assign("a2ff07ccbb019f48e020507ca5f5ee90");  /*vp80-05-sharpness-1433.raw*/
        MD5Key[51].assign("110a65e1729fc54e0a25dbf9cc367ccf");  /*vp80-05-sharpness-1434.raw*/
        MD5Key[52].assign("cc468ac5ce042c85f04d62a8e09c97ff");  /*vp80-05-sharpness-1438.raw*/
        MD5Key[53].assign("ebfc41ade751e96220e74491bffda736");  /*vp80-05-sharpness-1439.raw*/
        MD5Key[54].assign("903a267bcc69ad5f8d886db6478d997a");  /*vp80-05-sharpness-1440.raw*/
        MD5Key[55].assign("3c5c3c66cad414d6b79de77e977f115b");  /*vp80-05-sharpness-1443.raw*/
        MD5Key[56].assign("9f8e73e634493ab3e56aad63321f8e11");  /*vp80-06-cropping-001.raw*/
        MD5Key[57].assign("aef0cfaf282d07cd7ac360fb4f7a1f6b");  /*vp80-06-cropping-002.raw*/
        MD5Key[58].assign("609e845f662e3455daf57c68b859a4f6");  /*vp80-06-cropping-003.raw*/
        MD5Key[59].assign("c37c1c11bf3f5adcfa62a39b35cf3d07");  /*vp80-06-cropping-004.raw*/
        MD5Key[60].assign("82641ff61fe1d6bb7ba8accfa5b399ad");  /*vp80-06-cropping-005.raw*/
        MD5Key[61].assign("b8fa425191154685444164157bfbf53d");  /*vp80-06-cropping-006.raw*/
        MD5Key[62].assign("ee19c6b8947c3521b810b66ae080368a");  /*vp80-06-cropping-007.raw*/
        MD5Key[63].assign("1d99ee17a5ba002dbb185a00ea517427");  /*vp80-06-cropping-008.raw*/
        MD5Key[64].assign("017e2e8066397e6f4fdf9522cc2d3428");  /*vp80-06-cropping-009.raw*/
        MD5Key[65].assign("2e66d33de5a4550b168524b569799858");  /*vp80-06-cropping-010.raw*/
        MD5Key[66].assign("3afdb776a6260ab3a3722d5ed0a51ea6");  /*vp80-06-cropping-011.raw*/
        MD5Key[67].assign("ee370c45dd534c994e83454962f05e4a");  /*vp80-06-cropping-012.raw*/
        MD5Key[68].assign("de95d19355f402c2ce9a75562ef45a88");  /*vp80-06-cropping-013.raw*/
        MD5Key[69].assign("79d1c72208f9a40a770a065b48d7c9b6");  /*vp80-06-cropping-014.raw*/
        MD5Key[70].assign("eac91c3f679b57fb0d41965a58515c58");  /*vp80-06-cropping-015.raw*/
        MD5Key[71].assign("1ade1e1d87d045a3df99c1c4431c3770");  /*vp80-06-cropping-016.raw*/
        MD5Key[72].assign("a92acb085407d9f38d6b86a25dcd31b6");  /*vp80-06-cropping-017.raw*/
        MD5Key[73].assign("633ab126a1f74babedef090b7a9f79f3");  /*vp80-06-cropping-018.raw*/
        MD5Key[74].assign("336b86b25f320eb63eb0476ab84229cd");  /*vp80-06-cropping-019.raw*/
        MD5Key[75].assign("d4bf42f6671d06dbda405c105984f4a6");  /*vp80-06-cropping-020.raw*/
        MD5Key[76].assign("a857394e5ad0b52ffcffd61d0be108ed");  /*vp80-06-cropping-021.raw*/
        MD5Key[77].assign("2f985c573f020c5bac55d48fa5e24489");  /*vp80-06-cropping-022.raw*/
        MD5Key[78].assign("8a56b8c71b671503018727917fcdb145");  /*vp80-06-cropping-023.raw*/
        MD5Key[79].assign("2369693e71dcb565327b49c31fdc513e");  /*vp80-06-cropping-024.raw*/
        MD5Key[80].assign("9be67b80d50f157bbae891e2a465909e");  /*vp80-06-cropping-025.raw*/
        MD5Key[81].assign("20ed8b808ad8f3be05a9c80e0328f0e5");  /*vp80-06-cropping-026.raw*/
        MD5Key[82].assign("d4178440ab559d89df2d036c6606cda6");  /*vp80-06-cropping-027.raw*/
        MD5Key[83].assign("c65520731d9caae321de26fa41e77bf2");  /*vp80-06-cropping-028.raw*/
        MD5Key[84].assign("097c3f56e8917306fa5f1a60e63c95b9");  /*vp80-06-cropping-029.raw*/
        MD5Key[85].assign("f52c5fc26a9439aa8226d533bcdada98");  /*vp80-06-cropping-030.raw*/
        MD5Key[86].assign("5df73922d1ed6680e4d2ac35d86b061a");  /*vp80-06-cropping-031.raw*/
        MD5Key[87].assign("9d9a31179bfb0daf3a884625e7f1c116");  /*vp80-06-cropping-032.raw*/
        MD5Key[88].assign("bbafe2fdf7b927ff0b403f91646ed80b");  /*vp80-06-cropping-033.raw*/
        MD5Key[89].assign("b2d27a6f08e37e1a26bfd31e229684d8");  /*vp80-06-cropping-034.raw*/
        MD5Key[90].assign("dd5138a4831475284d285a6e821387d6");  /*vp80-06-cropping-035.raw*/
        MD5Key[91].assign("8fd17178a7d163a8eb9b439d9fa42797");  /*vp80-06-cropping-036.raw*/
        MD5Key[92].assign("0e7960d1d71e1aa5a97d22f152c10a8f");  /*vp80-06-cropping-037.raw*/
        MD5Key[93].assign("84b6553703153de46aefb09bc479bca5");  /*vp80-06-cropping-038.raw*/
        MD5Key[94].assign("971f42513a93493c4c7bce8177a07da4");  /*vp80-06-cropping-039.raw*/
        MD5Key[95].assign("98e3e9253baa99f5bf40316eb54d9e36");  /*vp80-06-cropping-040.raw*/
        MD5Key[96].assign("1e57bb03f8872d07d64a3177d2a31ceb");  /*vp80-06-cropping-041.raw*/
        MD5Key[97].assign("1a2360cc18b06016777fb9614ac4de61");  /*vp80-06-cropping-042.raw*/
        MD5Key[98].assign("18924afc1f5acb60d3933184ff1f93c8");  /*vp80-06-cropping-043.raw*/
        MD5Key[99].assign("62f5de3feb4346072c94d643d6946fb4");  /*vp80-06-cropping-044.raw*/
        MD5Key[100].assign("8319b0f71a76787f894f2927b7923897");  /*vp80-06-cropping-045.raw*/

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
            CurTestVector = 1;

            while (CurTestVector < LastTestVector)
            {
                printf("\n\nTestVector %i\nAPI - Decompressing VP8 IVF File to Raw File: \n", CurTestVector);
                fprintf(stderr, "\n\nTestVector %i\nAPI - Decompressing VP8 IVF File to Raw File: \n", CurTestVector);

                DecompressIVFtoRaw(TestVector[CurTestVector].c_str(), TestVector_Raw[CurTestVector].c_str());
                CurTestVector++;
            }
        }

        vector<int> FailVector;

        if (TestType == 2)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 10;
        }

        //Compute MD5 CheckSums
        CurTestVector = 1;

        while (CurTestVector < LastTestVector)
        {
            printf("\n\nComputing MD5 for TestVector %i", CurTestVector);
            fprintf(stderr, "\n\nComputing MD5 for TestVector %i\n", CurTestVector);

            DecComputeMD5(TestVector[CurTestVector].c_str(), TestVector_Text[CurTestVector].c_str());
            CurTestVector++;
        }

        if (DeleteTV == 1)
        {
            CurTestVector = 1;

            while (CurTestVector < LastTestVector)
            {
                if (remove(TestVector_Raw[CurTestVector].c_str()) == 0)
                {
                    /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
                }
                else
                {
                    printf("Error: %s Not Deleted\n\n", TestVector_Raw[CurTestVector].c_str());
                }

                CurTestVector++;
            }
        }

        int x = 0;
        int Fail = 0;

        CurTestVector = 1;

        while (CurTestVector < LastTestVector)
        {
            char TextInput[255];
            char TestVectFileName[255];
            snprintf(TextInput, 255, "%s", TestVector_Text[CurTestVector].c_str());
            FileName(TestVector[CurTestVector].c_str(), TestVectFileName, 1);

            char buffer[1024];
            fstream infile;
            infile.open(TextInput);
            infile.getline(buffer, 1024);
            infile.close();

            printf("\n\n");
            fprintf(stderr, "\n\n");

            if (MD5Key[x].compare(buffer) == 0)
            {
                printf(" VP8 Test Vector %i: %s \n"
                       " Observed: %s \n"
                       " Expected: %s \n"
                       " -Pass \n\n"
                       , x + 1, TestVectFileName, buffer, MD5Key[x].c_str());

                fprintf(stderr, " VP8 Test Vector %i: %s \n"
                        " Observed: %s \n"
                        " Expected: %s \n"
                        " -Pass \n\n"
                        , x + 1, TestVectFileName, buffer, MD5Key[x].c_str());
            }
            else
            {
                printf(" VP8 Test Vector %i: %s \n"
                       " Observed: %s \n"
                       " Expected: %s \n"
                       " -Fail \n\n"
                       , x + 1, TestVectFileName, buffer, MD5Key[x].c_str());

                fprintf(stderr, " VP8 Test Vector %i: %s \n"
                        " Observed: %s \n"
                        " Expected: %s \n"
                        " -Fail \n\n"
                        , x + 1, TestVectFileName, buffer, MD5Key[x].c_str());

                FailVector.push_back(x + 1);
                Fail = 1;
            }

            CurTestVector++;
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
                    }
                    else
                    {
                        FailStr.append(" and ");
                    }
                }

                char TVFailNumChar[32] = "";
                itoa_custom(FailVector[q], TVFailNumChar, 10);
                FailStr.append(TVFailNumChar);
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

        string TwoPassOutFile1 = WorkingDirString;
        string TwoPassBestOutFile1 = WorkingDirString;
        string TwoPassOutFile2 = WorkingDirString;
        string TwoPassBestOutFile2 = WorkingDirString;
        string TwoPassOutFile3 = WorkingDirString;
        string TwoPassBestOutFile3 = WorkingDirString;

        TwoPassOutFile1.append(slashCharStr);
        TwoPassOutFile1.append("TwoPassOutput1.ivf");
        TwoPassBestOutFile1.append(slashCharStr);
        TwoPassBestOutFile1.append("TwoPassBestOutput1.ivf");
        TwoPassOutFile2.append(slashCharStr);
        TwoPassOutFile2.append("TwoPassOutput2.ivf");
        TwoPassBestOutFile2.append(slashCharStr);
        TwoPassBestOutFile2.append("TwoPassBestOutput2.ivf");
        TwoPassOutFile3.append(slashCharStr);
        TwoPassOutFile3.append("TwoPassOutput3.ivf");
        TwoPassBestOutFile3.append(slashCharStr);
        TwoPassBestOutFile3.append("TwoPassBestOutput3.ivf");

        ////char TwoPassOutFile1[255];
        //char TwoPassBestOutFile1[255];
        //char TwoPassOutFile2[255];
        //char TwoPassBestOutFile2[255];
        //char TwoPassOutFile3[255];
        //char TwoPassBestOutFile3[255];

        //snprintf(TwoPassOutFile1, 255, "%s", WorkingDir4.c_str());
        //snprintf(TwoPassBestOutFile1, 255, "%s", WorkingDir5.c_str());
        //snprintf(TwoPassOutFile2, 255, "%s", WorkingDir4b.c_str());
        //snprintf(TwoPassBestOutFile2, 255, "%s", WorkingDir5b.c_str());
        //snprintf(TwoPassOutFile3, 255, "%s", WorkingDir4c.c_str());
        //snprintf(TwoPassBestOutFile3, 255, "%s", WorkingDir5c.c_str());

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        int CompressInt = opt.allow_df;

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
            opt.target_bandwidth = BitRate1;

            if (CompressIVFtoIVF(input, TwoPassOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate2;

            if (CompressIVFtoIVF(input, TwoPassOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate3;

            if (CompressIVFtoIVF(input, TwoPassOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_SECONDPASS_BEST;
            opt.target_bandwidth = BitRate1;

            if (CompressIVFtoIVF(input, TwoPassBestOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate2;

            if (CompressIVFtoIVF(input, TwoPassBestOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.target_bandwidth = BitRate3;

            if (CompressIVFtoIVF(input, TwoPassBestOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

        float GoodSize1 = IVFDataRate(TwoPassOutFile1.c_str(), 1);
        float BestSize1 = IVFDataRate(TwoPassBestOutFile1.c_str(), 1);
        float GoodSize2 = IVFDataRate(TwoPassOutFile2.c_str(), 1);
        float BestSize2 = IVFDataRate(TwoPassBestOutFile2.c_str(), 1);
        float GoodSize3 = IVFDataRate(TwoPassOutFile3.c_str(), 1);
        float BestSize3 = IVFDataRate(TwoPassBestOutFile3.c_str(), 1);

        double PSNRG1;
        double PSNRB1;
        double PSNRG2;
        double PSNRB2;
        double PSNRG3;
        double PSNRB3;

        PSNRG1 = IVFPSNR(input, TwoPassOutFile1.c_str(), 1, 0, 1, NULL);
        PSNRB1 = IVFPSNR(input, TwoPassBestOutFile1.c_str(), 1, 0, 1, NULL);
        PSNRG2 = IVFPSNR(input, TwoPassOutFile2.c_str(), 1, 0, 1, NULL);
        PSNRB2 = IVFPSNR(input, TwoPassBestOutFile2.c_str(), 1, 0, 1, NULL);
        PSNRG3 = IVFPSNR(input, TwoPassOutFile3.c_str(), 1, 0, 1, NULL);
        PSNRB3 = IVFPSNR(input, TwoPassBestOutFile3.c_str(), 1, 0, 1, NULL);

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

        string UnderShoot10 = WorkingDirString;
        string UnderShoot100 = WorkingDirString;

        UnderShoot10.append(slashCharStr);
        UnderShoot10.append("UnderShoot10Output.ivf");
        UnderShoot100.append(slashCharStr);
        UnderShoot100.append("UnderShoot100Output.ivf");

        //char UnderShoot10[255];
        //char UnderShoot100[255];

        //snprintf(UnderShoot10, 255, "%s", WorkingDir4.c_str());
        //snprintf(UnderShoot100, 255, "%s", WorkingDir5.c_str());

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;

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
                opt.under_shoot_pct = 10;

                if (CompressIVFtoIVF(input, UnderShoot10.c_str(), speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.under_shoot_pct = 100;

                if (CompressIVFtoIVF(input, UnderShoot100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
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
                opt.under_shoot_pct = 10;

                if (CompressIVFtoIVF(input, UnderShoot10.c_str(), speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.under_shoot_pct = 100;

                if (CompressIVFtoIVF(input, UnderShoot100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
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
                opt.under_shoot_pct = 10;

                if (CompressIVFtoIVF(input, UnderShoot10.c_str(), speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.under_shoot_pct = 100;

                if (CompressIVFtoIVF(input, UnderShoot100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
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
                opt.under_shoot_pct = 10;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, UnderShoot10.c_str(), speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.under_shoot_pct = 100;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, UnderShoot100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            if (Mode == 5)
            {
                opt.under_shoot_pct = 10;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, UnderShoot10.c_str(), speed, BitRate, opt, CompressString, 10, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.under_shoot_pct = 100;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, UnderShoot100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
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
        long File1bytes = FileSize(UnderShoot10.c_str());
        printf("\n");
        fprintf(stderr, "\n");
        long File2bytes = FileSize(UnderShoot100.c_str());
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

        string Version0 = WorkingDirString;
        string Version1 = WorkingDirString;
        string Version2 = WorkingDirString;
        string Version3 = WorkingDirString;

        string Version0_Dec = WorkingDirString;
        string Version1_Dec = WorkingDirString;
        string Version2_Dec = WorkingDirString;
        string Version3_Dec = WorkingDirString;

        string Version0_Dectime = WorkingDirString;
        string Version1_Dectime = WorkingDirString;
        string Version2_Dectime = WorkingDirString;
        string Version3_Dectime = WorkingDirString;

        Version0.append(slashCharStr);
        Version0.append("Version0.ivf");
        Version1.append(slashCharStr);
        Version1.append("Version1.ivf");
        Version2.append(slashCharStr);
        Version2.append("Version2.ivf");
        Version3.append(slashCharStr);
        Version3.append("Version3.ivf");

        Version0_Dec.append(slashCharStr);
        Version0_Dec.append("Version0_Dec.ivf");
        Version1_Dec.append(slashCharStr);
        Version1_Dec.append("Version1_Dec.ivf");
        Version2_Dec.append(slashCharStr);
        Version2_Dec.append("Version2_Dec.ivf");
        Version3_Dec.append(slashCharStr);
        Version3_Dec.append("Version3_Dec.ivf");

        Version0_Dectime.append(slashCharStr);
        Version0_Dectime.append("Version0_Dec_CompressionTime.txt");
        Version1_Dectime.append(slashCharStr);
        Version1_Dectime.append("Version1_Dec_CompressionTime.txt");
        Version2_Dectime.append(slashCharStr);
        Version2_Dectime.append("Version2_Dec_CompressionTime.txt");
        Version3_Dectime.append(slashCharStr);
        Version3_Dectime.append("Version3_Dec_CompressionTime.txt");

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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;

        float PSNRArr[4];
        unsigned int DecTime[4];

        //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
        //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
        //Test Type 3 = Mode 2 =Run Test Compressions

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            DecTime[0] = TimeReturn(Version0_Dec.c_str(), 1);
            DecTime[1] = TimeReturn(Version1_Dec.c_str(), 1);
            DecTime[2] = TimeReturn(Version2_Dec.c_str(), 1);
            DecTime[3] = TimeReturn(Version3_Dec.c_str(), 1);
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;
                opt.Version = 0;

                if (CompressIVFtoIVF(input, Version0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.Version = 1;

                if (CompressIVFtoIVF(input, Version1.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.Version = 2;

                if (CompressIVFtoIVF(input, Version2.c_str(), speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_REALTIME;
                opt.Version = 3;

                if (CompressIVFtoIVF(input, Version3.c_str(), speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.Version = 1;

                if (CompressIVFtoIVF(input, Version1.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.Version = 2;

                if (CompressIVFtoIVF(input, Version2.c_str(), speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_GOODQUALITY;
                opt.Version = 3;

                if (CompressIVFtoIVF(input, Version3.c_str(), speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.Version = 1;

                if (CompressIVFtoIVF(input, Version1.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.Version = 2;

                if (CompressIVFtoIVF(input, Version2.c_str(), speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_BESTQUALITY;
                opt.Version = 3;

                if (CompressIVFtoIVF(input, Version3.c_str(), speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 1;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Version1.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 2;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Version2.c_str(), speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 3;
                opt.Mode = MODE_SECONDPASS;

                if (CompressIVFtoIVF(input, Version3.c_str(), speed, BitRate, opt, CompressString, 3, 0) == -1)
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

                if (CompressIVFtoIVF(input, Version0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 1;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Version1.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 2;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Version2.c_str(), speed, BitRate, opt, CompressString, 2, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Version = 3;
                opt.Mode = MODE_SECONDPASS_BEST;

                if (CompressIVFtoIVF(input, Version3.c_str(), speed, BitRate, opt, CompressString, 3, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    RecordTestComplete(MainDirString, File1Str, TestType);
                    return 2;
                }
            }

            printf("\n\n");
            fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");
            DecTime[0] = TimeDecompressIVFtoIVF(Version0.c_str(), Version0_Dec.c_str());
            printf("\n");
            fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
            DecTime[1] = TimeDecompressIVFtoIVF(Version1.c_str(), Version1_Dec.c_str());
            printf("\n");
            fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
            DecTime[2] = TimeDecompressIVFtoIVF(Version2.c_str(), Version2_Dec.c_str());
            printf("\n");
            fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
            DecTime[3] = TimeDecompressIVFtoIVF(Version3.c_str(), Version3_Dec.c_str());

        }

        //Create Compression only stop test short.
        if (TestType == 2)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 10;
        }

        PSNRArr[0] = IVFPSNR(input, Version0.c_str(), 0, 0, 1, NULL);
        PSNRArr[1] = IVFPSNR(input, Version1.c_str(), 0, 0, 1, NULL);
        PSNRArr[2] = IVFPSNR(input, Version2.c_str(), 0, 0, 1, NULL);
        PSNRArr[3] = IVFPSNR(input, Version3.c_str(), 0, 0, 1, NULL);

        printf("\n");
        fprintf(stderr, "\n");

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
            printf("\nMin Passed\n");
            fprintf(stderr, "\nMin Passed\n");

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
                BitRate = opt.target_bandwidth;
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
            BitRate = opt.target_bandwidth;
        }

        /////////////////////////////////////////////////////////
        BitRate = opt.target_bandwidth;


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

        opt.target_bandwidth = BitRate;
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
        //  if(x > 74)
        //  {
        //      cout << "\nMax Array Value = 74\n";
        //      return 0;
        //  }
        //  cout << "vp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  cerr << "vp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  StringFound = 1;
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
        //  if(x > 5)
        //  {
        //      cout << "\nMax Array Value = 5\n";
        //      return 0;
        //  }
        //  cout << "vp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  cerr << "vp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  StringFound = 1;
        //}
        //if(arrayname.compare("vp8_HighMVtree")==0)//Char
        //{
        //  if(x > 7)
        //  {
        //      cout << "\nMax Array Value = 7\n";
        //      return 0;
        //  }
        //  cout << "vp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  cerr << "vp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  StringFound = 1;
        //}
        //if(arrayname.compare("vp8_Reverse3bits")==0)
        //{
        //  if(x > 7)
        //  {
        //      cout << "\nMax Array Value = 7\n";
        //      return 0;
        //  }
        //  cout << "vp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  cerr << "vp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  StringFound = 1;
        //}
        //if(arrayname.compare("vp8_Reverse4bits")==0)
        //{
        //  if(x > 15)
        //  {
        //      cout << "\nMax Array Value = 15\n";
        //      return 0;
        //  }
        //  cout << "vp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  cerr << "vp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  StringFound = 1;
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
        //  if(x > 3)
        //  {
        //      cout << "\nMax x Array Value = 3\n";
        //      return 0;
        //  }
        //  if(y > 3)
        //  {
        //      cout << "\nMax y Array Value = 3\n";
        //      return 0;
        //  }
        //  cout << "idctMatrix1[" << x << "][" << y << "] = " << idctMatrix1[x][y] << "\n";
        //  cerr << "idctMatrix1[" << x << "][" << y << "] = " << idctMatrix1[x][y] << "\n";
        //  StringFound = 1;
        //}
        //if(arrayname.compare("idctMatrix2")==0)
        //{
        //  if(x > 3)
        //  {
        //      cout << "\nMax x Array Value = 3\n";
        //      return 0;
        //  }
        //  if(y > 3)
        //  {
        //      cout << "\nMax y Array Value = 3\n";
        //      return 0;
        //  }
        //  cout << "idctMatrix2[" << x << "][" << y << "] = " << idctMatrix2[x][y] << "\n";
        //  cerr << "idctMatrix2[" << x << "][" << y << "] = " << idctMatrix2[x][y] << "\n";
        //  StringFound = 1;
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

            //              vp8d_token_extra_bits2[0].Probs[1];
            //              vp8d_token_extra_bits2[0].Probs[1];

            StringFound = 1;
        }

        //if(arrayname.compare("nearB")==0)
        //{
        //  if(x > 2)
        //  {
        //      cout << "\nMax x Array Value = 2\n";
        //      return 0;
        //  }
        //  if(y > 3)
        //  {
        //      cout << "\nMax y Array Value = 3\n";
        //      return 0;
        //  }

        //  cout << "nearB[" << x << "].";

        //  if(y == 0)
        //  {
        //      cout << "row = " << nearB[x].row << "\n";
        //  }
        //  if(y == 1)
        //  {
        //      cout << "col = " << nearB[x].col << "\n";
        //  }
        //  if(y == 2)
        //  {
        //      cout << "weight = " << nearB[x].weight << "\n";
        //  }
        //  if(y == 3)
        //  {
        //      cout << "block = " << nearB[x].block << "\n";
        //  }

        //  cerr << "nearB[" << x << "].";

        //  if(y == 0)
        //  {
        //      cerr << "row = " << nearB[x].row << "\n";
        //  }
        //  if(y == 1)
        //  {
        //      cerr << "col = " << nearB[x].col << "\n";
        //  }
        //  if(y == 2)
        //  {
        //      cerr << "weight = " << nearB[x].weight << "\n";
        //  }
        //  if(y == 3)
        //  {
        //      cerr << "block = " << nearB[x].block << "\n";
        //  }

        //  StringFound = 1;
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
        //  cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
        //  cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
        //  cout << "\n\n";
        //  cerr << "\n\n";
        //  StorageInt = vp8_mv_cont_count[x][y];
        //  vp8_mv_cont_count[x][y] = vp8_mv_cont_count[x][y]+1;
        //  cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
        //  cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
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
        //  cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  cout << "\n\n";
        //  cerr << "\n\n";
        //  StorageInt = vp8_block2context[x];
        //  vp8_block2context[x] = vp8_block2context[x]  + 20;
        //  cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
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
        //  cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  cout << "\n\n";
        //  cerr << "\n\n";
        //  StorageChar = vp8_LowMVtree[x];
        //  vp8_LowMVtree[x] = vp8_LowMVtree[x]  + 20;
        //  cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //}
        //if(arrayname.compare("vp8_HighMVtree")==0)//Char
        //{
        //  cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  cout << "\n\n";
        //  cerr << "\n\n";
        //  StorageChar = vp8_HighMVtree[x];
        //  vp8_HighMVtree[x] = vp8_HighMVtree[x]  + 20;
        //  cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //}
        //if(arrayname.compare("vp8_Reverse3bits")==0)
        //{
        //  cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  cout << "\n\n";
        //  cerr << "\n\n";
        //  StorageInt = vp8_Reverse3bits[x];
        //  vp8_Reverse3bits[x] = vp8_Reverse3bits[x]  + 20;
        //  cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //}
        //if(arrayname.compare("vp8_Reverse4bits")==0)
        //{
        //  cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  cout << "\n\n";
        //  cerr << "\n\n";
        //  StorageInt = vp8_Reverse4bits[x];
        //  vp8_Reverse4bits[x] = vp8_Reverse4bits[x]  + 20;
        //  cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
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
        //  cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
        //  cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
        //  vp8_mv_cont_count[x][y]=StorageInt;
        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";
        //  cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
        //  cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
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
        //  cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  vp8_block2context[x] = StorageInt;
        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";
        //  cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
        //  cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
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
        //  cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
        //  cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
        //  vp8_sub_mv_ref_prob2[x][y]=StorageInt;
        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";
        //  cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
        //  cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
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
        //  cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  vp8_LowMVtree[x] = StorageChar;
        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";
        //  cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //  cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
        //}
        //if(arrayname.compare("vp8_HighMVtree")==0)//Char
        //{
        //  cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  vp8_HighMVtree[x] = StorageChar;
        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";
        //  cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //  cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
        //}
        //if(arrayname.compare("vp8_Reverse3bits")==0)
        //{
        //  cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  vp8_Reverse3bits[x] = StorageInt;
        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";
        //  cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //  cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
        //}
        //if(arrayname.compare("vp8_Reverse4bits")==0)
        //{
        //  cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  vp8_Reverse4bits[x] = StorageInt;
        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";
        //  cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
        //  cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
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
        //  cout << "\n";
        //  cerr << "\n";

        //  cout << "nearB[" << x << "].";

        //  if(y == 0)
        //  {
        //      cout << "row = " << nearB[x].row << "\n";
        //  }
        //  if(y == 1)
        //  {
        //      cout << "col = " << nearB[x].col << "\n";
        //  }
        //  if(y == 2)
        //  {
        //      cout << "weight = " << nearB[x].weight << "\n";
        //  }
        //  if(y == 3)
        //  {
        //      cout << "block = " << nearB[x].block << "\n";
        //  }

        //  cerr << "nearB[" << x << "].";

        //  if(y == 0)
        //  {
        //      cerr << "row = " << nearB[x].row << "\n";
        //  }
        //  if(y == 1)
        //  {
        //      cerr << "col = " << nearB[x].col << "\n";
        //  }
        //  if(y == 2)
        //  {
        //      cerr << "weight = " << nearB[x].weight << "\n";
        //  }
        //  if(y == 3)
        //  {
        //      cerr << "block = " << nearB[x].block << "\n";
        //  }

        //  //vp8_kf_default_bmode_counts[x][y][z]=StorageInt;
        //  if(y == 0)
        //  {
        //      nearB[x].row = StorageInt;
        //  }
        //  if(y == 1)
        //  {
        //      nearB[x].col = StorageInt;
        //  }
        //  if(y == 2)
        //  {
        //      nearB[x].weight = StorageInt;
        //  }
        //  if(y == 3)
        //  {
        //      nearB[x].block = StorageInt;
        //  }

        //  cout << "\nArray Var Reset to:";
        //  cerr << "\nArray Var Reset to:";

        //  cout << "nearB[" << x << "].";

        //  if(y == 0)
        //  {
        //      cout << "row = " << nearB[x].row << "\n";
        //  }
        //  if(y == 1)
        //  {
        //      cout << "col = " << nearB[x].col << "\n";
        //  }
        //  if(y == 2)
        //  {
        //      cout << "weight = " << nearB[x].weight << "\n";
        //  }
        //  if(y == 3)
        //  {
        //      cout << "block = " << nearB[x].block << "\n";
        //  }

        //  cerr << "nearB[" << x << "].";

        //  if(y == 0)
        //  {
        //      cerr << "row = " << nearB[x].row << "\n";
        //  }
        //  if(y == 1)
        //  {
        //      cerr << "col = " << nearB[x].col << "\n";
        //  }
        //  if(y == 2)
        //  {
        //      cerr << "weight = " << nearB[x].weight << "\n";
        //  }
        //  if(y == 3)
        //  {
        //      cerr << "block = " << nearB[x].block << "\n";
        //  }
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

