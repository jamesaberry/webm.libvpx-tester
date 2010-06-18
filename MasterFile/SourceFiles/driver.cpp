#define _CRT_SECURE_NO_WARNINGS
//////////////////////////
#include "on2-vpx-shim.h"
#include "yv12config.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <time.h>
using namespace std;
//////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
////////////////////////Slash Character Definion for multiplat////////////////////////
#if defined(_WIN32)
char slashChar = '\\';
string slashCharStr = "\\";

#include <windows.h>
#include <tchar.h>
#include <direct.h>
#define snprintf _snprintf


#elif defined(linux)
char slashChar = '/';
string slashCharStr = "/";

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#elif defined(__APPLE__)
char slashChar = '/';
string slashCharStr = "/";

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#elif defined(__POWERPC__)
char slashChar = '/';
string slashCharStr = "/";

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#endif
#include <cstdio>
///////////////////////////////////////////////////////////////////////////////////////

//This is temporary need better solution
#ifdef DSHOW
char TesterExePath[256];
#endif

//Utilities
extern long FileSize(char *inFile);
extern void FolderName(char *DirIn, char *DirOut);
extern int Test0InputTextCheck(char *input, int MoreInfo);
extern int CompareIVFHeaderInfo(int argc, char *argv[]);
extern int DisplayIVFHeaderInfo(int argc, char *argv[]);
extern int CompIVF(char *inputFile1, char *inputFile2);
extern double IVFDisplayKeyFrames(char *inputFile, int Selector, char *outputFile);
extern int API20Encoder(long width, long height, char *infilechar, char *outfilechar);
extern int API20Decoder(char *inputchar, char *outputchar);
extern int MakeDirVPX(string CreateDir2);

//Tools
extern int ComprAVI2IVF(int argc, char *argv[], string WorkingDir);
extern int ComprIVF2IVF(int argc, char *argv[], string WorkingDir);
extern int CopyIVF2IVF(int argc, char *argv[], string WorkingDir);
extern int DecIVF2IVF(int argc, char *argv[]);
extern int IVFDataRateTool(int argc, char *argv[]);
extern int IVFPSNRrun(int argc, char *argv[]);
extern int IVFCheckPBMrun(int argc, char *argv[]);
extern int compareIVF(int argc, char *argv[]);
extern int ChangeCPUDecIVF(int argc, char *argv[], string WorkingDir);
extern int RawDataIVF(char *input, char *output);
extern int WriteIndividualFramesOut(int argc, char *argv[]);
extern int CutIVF(int argc, char *argv[]);
extern int PlayCompIVF(int argc, char *argv[]);
extern int PlayDecIVF(int argc, char *argv[]);
extern int DeleteAllIVFFiles(int argc, char *argv[]);
extern int CopyAllTxtFiles(int argc, char *argv[]);
extern int SideBySideText(int argc, char *argv[]);
extern int CompressionEquiv(int argc, char *argv[], string WorkingDir);
extern int Raw2IVF(int argc, char *argv[]);
extern int ConvertParmFileToIVFenc(char *input, char *output);
extern int CompMatchesIVFenc(int argc, char *argv[]);
extern int CompareCodeCoverage(int argc, char *argv[]);
extern int DecIVF2Raw(int argc, char *argv[]);
extern int FormatCodeCoverageFile(int argc, char *argv[]);
extern int TestVectorIndex(int argc, char *argv[]);
extern int APICOMPRESS(int argc, char *argv[]);
extern int APIDECOMPRESS(int argc, char *argv[]);
extern int IVFDECTestVectorCheck(int argc, char *argv[]);
extern int ArrayCovFailListToFullList(int argc, char *argv[]);
extern int IVF2Raw(char *inputFile, char *outputDir);
extern int ArrayCovSummaryFile(int argc, char *argv[]);
extern int WinMemMonFormat(int argc, char *argv[]);
extern int WinMemMonGraph(int argc, char *argv[]);
extern int PasteIVF(int argc, char *argv[]);
extern int PrintVersion();
extern int CombineIndvFrames(int argc, char *argv[]);
extern int CreateRandParFile(int argc, char *argv[]);

/////Tests
extern int ExternalTestRunner(int argc, char *argv[], string WorkingDir, int NumberofTests);
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
extern int TokenPartitionTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int Version(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int AltQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int KeyQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int GoldQ(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int WindowsMatchesLinux(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int EncoderBreakOutTest(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int MemLeakCheck2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
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

//CodeCoverage
extern int CodeCoverage(int argc, char *argv[], string WorkingDir, string FilesAr[]);

//Misc Tools
extern int DecoderCheck(int argc, char *argv[]);
extern "C" const char *on2_codec_build_config(void);

int CreateWorkingFolder(int argc, char *argv[], char *WorkingDirChar)
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

    DateAndTimeCharArray[w] = '\"';
    DateAndTimeCharArray[w+1] = '\0';
    string DateAndTime3 = DateAndTimeCharArray;

    //Get Dir Folder
    char Folder3[255];
    char Folder2[255];

    FolderName(argv[0], Folder3);
    snprintf(Folder2, 255, "%s", Folder3);
    string Folder = Folder2;

    //add Date and Time
    Folder.append(DateAndTime3);
    snprintf(WorkingDirChar, 255, "%s", Folder.c_str());

    return 0;

}
string DateString()
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
void OnErrorOutPut()
{
    printf("\n"
           "Instructions for Use\n"
           "\n"
           " VP8 Test Program\n"
           "\n"
           "  <Test Number>                          Tools\n"
           "                                           \n"
           "   (0)RunTestsFromFile                     IVF2IVFCompr\n"
           "   (1)AllowDropFrames                      IVF2IVFDec\n"
           "   (2)AllowLagTest                         IVF2RawDec\n"
           "   (3)AllowSpatialResampling               IVFDataRate\n"
           "   (4)AllowSpatialResampling2              IVFPSNR\n"
           "   (5)AltFreqTest                          IVFCheckPBM\n"
           "   (6)AutoKeyFramingWorks                  \n"
           "   (7)BufferLevelWorks                     Raw2IVF\n"
           "   (8)CPUDecOnlyWorks                      IVF2Raw\n"
           "   (9)ChangeCPUDec                         IVF2RawFrames\n"
           "  (10)ChangeCPUWorks                       CombineIndvFrames\n"
           "  (11)DropFramesWaterMarkWorks             \n"
           "  (12)DataRateTest                         CompIVFHeader\n"
           "  (13)DebugMatchesRelease                  DispIVFHeader\n"
           "  (14)EncoderBreakOutTest                  DispKeyFrames\n"
           "  (15)ErrorResilientModeWorks              CompareIVF\n"
           "  (16)ExtraFileCheck                       \n"
           "  (17)FixedQ                               CutIVF\n"
           "  (18)ForceKeyFrameWorks                   PasteIVF\n"
           "  (19)GoodQualityVsBestQuality        	   \n"
           "  (20)LagInFramesTest                      PlayDecIVF\n"
           "  (21)MaxQuantizerTest                     PlayCompIVF\n"
           "  (22)MemLeakCheck                         \n"
           "  (23)MemLeakCheck2                        CreateSampleTextFiles\n"
           "  (24)MinQuantizerTest                     PrintVersion\n"
           "  (25)MultiThreadedTest                    \n"
           "  (26)NewVsOldPSNR                         RandParFile\n"
           "  (27)NewVsOldRealTimeSpeed                RandIVFComp\n"
           "  (28)NoiseSensitivityWorks                GraphPSNR\n"
           "  (29)OnePassVsTwoPassm                    Help\n"
           "  (30)PlayAlternate                        \n"
           "  (31)PostProcessorWorks                   \n"
           "  (32)PreProcessorWorks                    \n"
           "  (33)ResampleDownWaterMark                \n"
           "  (34)SpeedTest                            \n"
           "  (35)TestVectorCheck                      \n"
           "  (36)TwoPassVsTwoPassBest                 \n"
           "  (37)UnderShoot							\n"
           "  (38)Version                              \n"
           "  (39)WindowsMatchesLinux                  \n"
           "\n"
          );
}
void Print1(string WorkingDir)
{
    string TextfileString = WorkingDir;
    TextfileString.erase(TextfileString.end() - 25, TextfileString.end());
    //TextfileString.append(slashCharStr);
    TextfileString.append("PrintTxtFiles.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    fprintf(stderr,
            " 0 ExternalTestRunner \n\n"
            "    <Test Type>\n"
            "         <1 - Create Files and Run Tests>\n"
            "         <2 - Create Files only>\n"
            "         <3 - Run Tests For Existing Files - Requires Input Test Directory>\n"
            "    <Input Text File>\n"
            "    <Optional - Input Test Directory>\n");

    fprintf(stderr, "\n\n\n"
            " 1 AllowDF \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 2 AllowSpatialResampling \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 3 AllowSpatialResampling2 \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 4 AutoKeyFramingWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 5 BufferLevelWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 6 CPUDecOnlyWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 7 ChangeCPUDec \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 8 ChangeCPUWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            " 9 ChangeIDCTDecWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            "10 ChangeIDCTDecWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            "11 ComboCheckRun \n\n"
            "    <Settings Inputfile>\n"
            "    <Primary Avi Inputfile>\n"
           );

    fprintf(stderr, "\n\n\n"
            "12 DFWMTest \n\n"
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

    fprintf(stderr, "\n\n\n"
            "13 DataRateMaxWorks \n\n"
            "    This Test is not a valid VP8 Test.\n");

    fprintf(stderr, "\n\n\n"
            "14 DataRateTest \n\n"
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

    fprintf(stderr, "\n\n\n"
            "15 DebugMatchesRelease \n\n"
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
           );

    fprintf(stderr, "\n\n\n"
            "16  ErrorRes \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n ");

    fprintf(stderr, "\n\n\n"
            "17 ExtraFileCheck \n\n"
            "    <Inputfile>\n"
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "18 ForceKeyFrameWorks \n\n"
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
           );

    fprintf(stderr, "\n\n\n"
            "19 GoodQvBestQ \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n"
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "20 IDCTDecOnlyWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            "21 InterlaceWorks \n\n"
            "    This Test is not a valid VP8 Test.\n");

    fprintf(stderr, "\n\n\n"
            "22 KeyInKeyOutWorks \n\n"
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
           );
    fprintf(stderr, "\n\n\n"
            "23 MaxQTest \n\n"
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

    fprintf(stderr, "\n\n\n"
            "24 MemLeakCheck \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Newest Debug executable>\n"
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "25 MinQTest \n\n"
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

    fprintf(stderr, "\n\n\n"
            "26 NewVsOldPSRN \n\n"
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
            "	 <Optional Settings File>\n"
            "\n"
           );

    fprintf(stderr, "\n\n\n"
            "27 NewVsOldRealTimeSpeed \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n"
            "    <Exe File To Compare>\n"
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "28 NewVsOldSpeed \n\n"
            "    This Test is not a valid VP8 Test.\n");

    fprintf(stderr, "\n\n\n"
            "29 NoiseSensitivityWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            "30 OnePassVsTwoPass \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "31 PostProcessorWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            "32 PreProcessorWorks \n\n"
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

    fprintf(stderr, "\n\n\n"
            "33 GraphPSNR\n\n"
            "    <Inputfile 1>\n"
            "    <Inputfile 2>\n"
            "    <UV Swap off:0 on:1>\n"
            "    <Frame Stats off:0 on:1>\n"
            "\n");

    fprintf(stderr, "\n\n\n"
            "34 ResampleDownWaterMark \n\n"
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

    fprintf(stderr, "\n\n\n"
            "35 RandComp\n\n"
            "    <Inputfile>\n"
           );
    fprintf(stderr, "\n\n\n"
            "36  \n\n"
            "    This Test is not a valid VP8 Test.\n");

    fprintf(stderr, "\n\n\n"
            "37 TwoPassVsTwoPassBest \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "38 UnderShoot \n\n"
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

    fprintf(stderr, "\n\n\n"
            "39 VP61vVP62 \n\n"
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
           );

    fprintf(stderr, "\n\n\n"
            "40 VP6MvVP60 \n\n"
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
            "\n");

    fprintf(stderr, "\n\n\n"
            "41  AllowLag \n\n"
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

    fprintf(stderr, "\n\n\n"
            "42  AltFreqTest \n\n"
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

    fprintf(stderr, "\n\n\n"
            "43  FixedQ \n\n"
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

    fprintf(stderr, "\n\n\n"
            "44  LagInFrames \n\n"
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

    fprintf(stderr, "\n\n\n"
            "45  MultiThreadedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "46  PlayAlternate \n\n"
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

    fprintf(stderr, "\n\n\n"
            "47  SpeedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Lag In Frames>\n "
            "	 <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "48  Version \n\n"
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

    fprintf(stderr, "\n\n\n"
            "49  WindowsMatchesLinux \n\n"
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
            "    <Input Folder - enter N/A if none>\n "
            "    <Optional Settings File>\n"
           );

    fprintf(stderr, "\n\n\n"
            "50  EncoderBreakOutTest \n\n"
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
    fprintf(stderr, "\n\n\n"
            "51  MemLeakCheck2 \n\n"
            "    <Mem Leak Check Exe>\n"
           );
    fprintf(stderr, "\n\n\n"
            "52  TestVectorCheck \n\n"
            "      <Input Directory>\n"
            "\n"
           );

    printf("\n\nDisplay Test Inputs text file created:\n%s\n\n", TextfileString.c_str());
    fclose(fp);

    return;
}
void Print2(string WorkingDir)
{


    string TextfileString2 = WorkingDir;
    TextfileString2.erase(TextfileString2.end() - 25, TextfileString2.end());
    //TextfileString2.append(slashCharStr);
    TextfileString2.append("ComboTestSampleInput.txt");

    FILE *fp2;

    if ((fp2 = freopen(TextfileString2.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString2.c_str());
        exit(1);
    }

    fprintf(stderr,
            "Anything above Test 1 is effectively commented out.  Anything below that that does not start with \"Test\" or\n"
            "one of the strings below with out the \'%%\' will set the coresponding setting to the value selected.\n"
            "The \'%%\' sign will comment out anything on the line that follows regardless of what it is.\n"
            "\n"
            "%%File=\n"
            "%%CpuUsed=\n"
            "%%AutoKey=\n"
            "%%KeyFreq=\n"
            "%%NoiseSensitivity=\n"
            "%%Sharpness=\n"
            "%%Mode=\n"
            "%%RateNum=\n"
            "%%RateDenom=\n"
            "%%TargetBandwidth=\n"
            //"%%MaxAllowedDatarate=\n"
            "%%FixedQ=\n"
            "%%BestAllowedQ=\n"
            "%%QuantizerMax=\n"
            "%%StartingBufferLevel=\n"
            "%%OptimalBufferLevel=\n"
            "%%MaximumBufferSize=\n"
            "%%AllowDF=\n"
            "%%DropFramesWaterMark=\n"
            "%%AllowSpatialResampling=\n"
            "%%ResampleDownWaterMark=\n"
            "%%ResampleUpWaterMark=\n"
            "%%TwoPassVBRBias=\n"
            "%%TwoPassVBRMinSection=\n"
            "%%TwoPassVBRMaxSection=\n"
            "%%DeleteFirstPassFile=\n"
            "%%FirstPassFile=\n"
            "%%TwoPassVBRMinSection=\n"
            "%%EncodeBreakout=\n"
            "\n"
            "%%VP8Vars\n"
            "%%AllowLag=\n"
            "%%LagInFrames=\n"
            "%%PlayAlternate=\n"
            "%%AltFreq=\n"
            "%%AltQ=\n"
            "%%KeyQ=\n"
            "%%GoldQ=\n"
            "%%Version=\n"
            "\n"
            "%%----------Test Information------------\n"
            "\n"
            "________________________________________________\n"
            "\n"
            "Test1\n"
            "File=C:\\I420 Video\\ivf\\I420_+spidey_noAudio_352x176_15fps.ivf\n"
            "TargetBandwidth=128\n"
            "BestAllowedQ=20\n"
            "StartingBufferLevel=3\n"
            //"MaxAllowedDatarate=200\n"
            "MaximumBufferSize=3\n"
            "\n"
            "________________________________________________\n"
            "\n"
            "Test2\n"
            "File=C:\\I420 Video\\ivf\\I420_+spidey_noAudio_352x176_15fps.ivf\n"
            "NoiseSensitivity=1\n"
            "KeyFreq=20\n"
            "TargetBandwidth=100\n"
            "BestAllowedQ=35\n"
            "________________________________________________\n"
            "\n"
            "Test3\n"
            "File=C:\\I420 Video\\ivf\\I420_+spidey_noAudio.ivf\n"
            "TargetBandwidth=25\n"
            "BestAllowedQ=20\n"
            "_______________________________________________\n"
            "\n"
            "Test4\n"
            "File=C:\\I420 Video\\ivf\\I420_+spidey_noAudio_352x176_15fps.ivf\n"
            "AllowDF=1\n"
            "TargetBandwidth=128\n"
            //"MaxAllowedDatarate=22\n"
            "QuantizerMax=45\n"
            "________________________________________________\n"
            "\n"
            "Test5\n"
            "File=C:\\I420 Video\\ivf\\I420_+spidey_noAudio_352x176_15fps.ivf\n"
            "AllowDF=0\n"
            "TargetBandwidth=128\n"
            //"MaxAllowedDatarate=22\n"
            "QuantizerMax=45\n"
            "________________________________________________\n"
            "Test6\n"
            "File=C:\\I420 Video\\ivf\\I420_+spidey_noAudio_352x176_15fps.ivf\n"
            "AllowDF=1\n"
            "TargetBandwidth=128\n"
            //"%MaxAllowedDatarate=22\n"
            "QuantizerMax=45\n"
            "_______________________________________________\n"
            "Test7\n"
            "File=C:\\I420 Video\\ivf\\I420_+spidey_noAudio.ivf\n"
            "AllowDF=1\n"
            "TargetBandwidth=128\n"
            //"MaxAllowedDatarate=22\n"
            "QuantizerMax=45\n"
            "________________________________________________\n"
            "\n"
            "________________________________________________\n"
            "%%Test9\n"
            "________________________________________________\n"
            "%%Test10\n"
            "________________________________________________\n"
            "%%Test11\n"
            "________________________________________________\n"
           );

    printf("\n\nSample Combo Test Run text file created:\n%s\n\n", TextfileString2.c_str());
    fclose(fp2);

    return;

}
void Print3(string WorkingDir)
{
    string TextfileString3 = WorkingDir;
    TextfileString3.erase(TextfileString3.end() - 25, TextfileString3.end());
    //TextfileString3.append(slashCharStr);
    TextfileString3.append("MultiRunSampleInput.txt");
    //cout << TextfileString3.c_str() << endl;

    FILE *fp3;

    if ((fp3 = freopen(TextfileString3.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString3.c_str());
        exit(1);
    }

    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "1@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "3@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "4@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "6@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "7@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "8@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "9@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "10@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "9@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "10@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "%%11 DNW - Does Not Work In Multi Run\n"
            "11@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "13@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "12@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "13@Full_IVF_Path_And_FileName@Mode@BitRate@Name_of_Newest_Debug_PlugIn@Name_of_Newest_Release_PlugIn\n"
            "15@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "16@Full_IVF_Path_And_FileName\n"
            "18@Full_IVF_Path_And_FileName@Mode@BitRate@ForceKeyFrame\n"
            "19@Full_IVF_Path_And_FileName@BitRate\n"
            "20@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "%%21 DNE - Does Not Exist in VP8\n"
            "22@Full_IVF_Path_And_FileName@Mode@BitRate@ForceKeyFrame\n"
            "21@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "22@Full_IVF_Path_And_FileName@Mode@BitRate@Name_of_Newest_Debug_PlugIn\n"
            "24@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "26@Full_IVF_Path_And_FileName@Mode@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "27@Full_IVF_Path_And_FileName@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "%%28 DNE - Does Not Exist in VP8\n"
            "28@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "29@Full_IVF_Path_And_FileName@BitRate\n"
            "31@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "32@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "%%33 DNW - Does Not Work In Multi Run\n"
            "33@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "%%35 DNW - Does Not Work In Multi Run\n"
            "%%36 DNE - Does Not Exist in VP8\n"
            "36@Full_IVF_Path_And_FileName@BitRate\n"
            "37@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "%%39 Padding\n"
            "%%40 Padding\n"
            "2@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "5@Full_IVF_Path_And_FileName@Mode@BitRate@Alt_Freq_1@Alt_Freq_2\n"
            "17@Full_IVF_Path_And_FileName@Mode@BitRate@FixedQ_1@FixedQ_2\n"
            "20@Full_IVF_Path_And_FileName@Mode@BitRate@Lag_in_Frames_1@Lag_in_Frames_2\n"
            "25@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "30@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "34@Full_IVF_Path_And_FileName@Mode@BitRate@Lag_in_Frames_3\n"
            "38@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "14@Full_IVF_Path_And_FileName@Mode@BitRate\n"
            "23@Name_of_Newest_Debug_PlugIn\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
           );

    printf("\n\nSample Multi Run text file created:\n%s\n\n", TextfileString3.c_str());
    fclose(fp3);

    return;
}
void Print4(string WorkingDir)
{
    string TextfileString4 = WorkingDir;
    TextfileString4.erase(TextfileString4.end() - 25, TextfileString4.end());
    //TextfileString4.append(slashCharStr);
    TextfileString4.append("FullModeMultiRunSampleInput.txt");

    FILE *fp4;

    if ((fp4 = freopen(TextfileString4.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString4.c_str());
        exit(1);
    }

    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%Instructions: Replace the following: Full_IVF_Path_And_FileName, BitRate, Name_of_Oldest_Release_\n"
            "%%              PlugIn, ForceKeyFrame, Name_of_Newest_Debug_PlugIn, Name_of_Newest_Release_PlugIn, \n"
            "%%              Lag_in_Frames_1, Lag_in_Frames_2, Lag_in_Frames_2, Alt_Freq_1, and Alt_Freq_2 with \n"
            "%%              the settings you wish to test.\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "16@Full_IVF_Path_And_FileName\n"
            "19@Full_IVF_Path_And_FileName@BitRate\n"
            "27@Full_IVF_Path_And_FileName@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "29@Full_IVF_Path_And_FileName@BitRate\n"
            "36@Full_IVF_Path_And_FileName@BitRate\n"
            "23@Name_of_Newest_Debug_PlugIn\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "25@Full_IVF_Path_And_FileName@0@BitRate\n"
            "25@Full_IVF_Path_And_FileName@1@BitRate\n"
            "34@Full_IVF_Path_And_FileName@0@BitRate@Lag_in_Frames_3\n"
            "34@Full_IVF_Path_And_FileName@1@BitRate@Lag_in_Frames_3\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n"

            "%%%%%%%%%%%%%%Mode0%%%%%%%%%%%%%%\n"
            "%%1 DNW - Does Not Work For Mode 0\n"
            "%%2 DNW - Does Not Work For Mode 0\n"
            "4@Full_IVF_Path_And_FileName@0@BitRate\n"
            "%%4 DNW - Does Not Work For Mode 0\n"
            "7@Full_IVF_Path_And_FileName@0@BitRate\n"
            "%%6 DNW - Does Not Work For Mode 0\n"
            "%%7 DNW - Does Not Work For Mode 0\n"
            "%%8 DNW - Does Not Work For Mode 0\n"
            "%%9 DNW - Does Not Work For Mode 0\n"
            "%%10 DNW - Does Not Work For Mode 0\n"
            "%%11 DNW - Does Not Work In Multi Run\n"
            "%%12 DNW - Does Not Work For Mode 0\n"
            "13@Full_IVF_Path_And_FileName@0@BitRate\n"
            "12@Full_IVF_Path_And_FileName@0@BitRate\n"
            "%%15 DNW - Does Not Work For Mode 0\n"
            "15@Full_IVF_Path_And_FileName@0@BitRate\n"
            "18@Full_IVF_Path_And_FileName@0@BitRate@ForceKeyFrame\n"
            "%%20 DNW - Does Not Work For Mode 0\n"
            "%%21 DNE - Does Not Exist in VP8\n"
            "%%22 DNE - Does Not Exist in VP8\n"
            "21@Full_IVF_Path_And_FileName@0@BitRate\n"
            "22@Full_IVF_Path_And_FileName@0@BitRate@Name_of_Newest_Debug_PlugIn\n"
            "24@Full_IVF_Path_And_FileName@0@BitRate\n"
            "26@Full_IVF_Path_And_FileName@0@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "%%28 DNE - Does Not Exist in VP8\n"
            "28@Full_IVF_Path_And_FileName@0@BitRate\n"
            "31@Full_IVF_Path_And_FileName@0@BitRate\n"
            "%%32 DNW - Does Not Work For Mode 0\n"
            "%%33 DNW - Does Not Work In Multi Run\n"
            "33@Full_IVF_Path_And_FileName@0@BitRate\n"
            "%%35 DNW - Does Not Work In Multi Run\n"
            "%%36 DNE - Does Not Exist in VP8\n"
            "%%38 DNW - Does Not Work For Mode 0\n"
            "%%39 Padding\n"
            "%%40 Padding\n"
            "%%41 DNW - Does Not Work For Mode 0\n"
            "%%42 DNW - Does Not Work For Mode 0\n"
            "%%43 DNW - Does Not Work For Mode 0\n"
            "20@Full_IVF_Path_And_FileName@0@BitRate@Lag_in_Frames_1@Lag_in_Frames_2\n"
            "%%46 DNW - Does Not Work For Mode 0\n"
            "38@Full_IVF_Path_And_FileName@0@BitRate\n"
            "14@Full_IVF_Path_And_FileName@4@BitRate\n\n"

            "%%%%%%%%%%%%%%Mode1%%%%%%%%%%%%%%\n"
            "1@Full_IVF_Path_And_FileName@1@BitRate\n"
            "4@Full_IVF_Path_And_FileName@1@BitRate\n"
            "4@Full_IVF_Path_And_FileName@1@BitRate\n"
            "6@Full_IVF_Path_And_FileName@1@BitRate\n"
            "7@Full_IVF_Path_And_FileName@1@BitRate\n"
            "8@Full_IVF_Path_And_FileName@1@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@1@BitRate@Version\n"
            "10@Full_IVF_Path_And_FileName@1@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@1@BitRate\n"
            "10@Full_IVF_Path_And_FileName@1@BitRate\n"
            "%%11 DNW - Does Not Work In Multi Run\n"
            "11@Full_IVF_Path_And_FileName@1@BitRate\n"
            "13@Full_IVF_Path_And_FileName@1@BitRate\n"
            "12@Full_IVF_Path_And_FileName@1@BitRate\n"
            "13@Full_IVF_Path_And_FileName@1@BitRate@Name_of_Newest_Debug_PlugIn@Name_of_Newest_Release_PlugIn\n"
            "15@Full_IVF_Path_And_FileName@1@BitRate\n"
            "18@Full_IVF_Path_And_FileName@1@BitRate@ForceKeyFrame\n"
            "20@Full_IVF_Path_And_FileName@1@BitRate\n"
            "%%21 DNE - Does Not Exist in VP8\n"
            "%%22 DNE - Does Not Exist in VP8\n"
            "21@Full_IVF_Path_And_FileName@1@BitRate\n"
            "22@Full_IVF_Path_And_FileName@1@BitRate@Name_of_Newest_Debug_PlugIn\n"
            "24@Full_IVF_Path_And_FileName@1@BitRate\n"
            "26@Full_IVF_Path_And_FileName@1@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "%%28 DNE - Does Not Exist in VP8\n"
            "28@Full_IVF_Path_And_FileName@1@BitRate\n"
            "31@Full_IVF_Path_And_FileName@1@BitRate\n"
            "32@Full_IVF_Path_And_FileName@1@BitRate\n"
            "%%33 DNW - Does Not Work In Multi Run\n"
            "33@Full_IVF_Path_And_FileName@1@BitRate\n"
            "%%35 DNW - Does Not Work In Multi Run\n"
            "%%36 DNE - Does Not Exist in VP8\n"
            "37@Full_IVF_Path_And_FileName@1@BitRate\n"
            "%%39 Padding\n"
            "%%40 Padding\n"
            "2@Full_IVF_Path_And_FileName@1@BitRate\n"
            "5@Full_IVF_Path_And_FileName@1@BitRate@Alt_Freq_1@Alt_Freq_2\n"
            "17@Full_IVF_Path_And_FileName@1@BitRate@FixedQ_1@FixedQ_2\n"
            "20@Full_IVF_Path_And_FileName@1@BitRate@Lag_in_Frames_1@Lag_in_Frames_2\n"
            "30@Full_IVF_Path_And_FileName@1@BitRate\n"
            "38@Full_IVF_Path_And_FileName@1@BitRate\n"
            "14@Full_IVF_Path_And_FileName@1@BitRate\n\n"

            "%%%%%%%%%%%%%%Mode2%%%%%%%%%%%%%%\n"
            "1@Full_IVF_Path_And_FileName@2@BitRate\n"
            "4@Full_IVF_Path_And_FileName@2@BitRate\n"
            "4@Full_IVF_Path_And_FileName@2@BitRate\n"
            "6@Full_IVF_Path_And_FileName@2@BitRate\n"
            "7@Full_IVF_Path_And_FileName@2@BitRate\n"
            "8@Full_IVF_Path_And_FileName@2@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@2@BitRate@Version\n"
            "10@Full_IVF_Path_And_FileName@2@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@2@BitRate\n"
            "10@Full_IVF_Path_And_FileName@2@BitRate\n"
            "%%11 DNW - Does Not Work In Multi Run\n"
            "11@Full_IVF_Path_And_FileName@2@BitRate\n"
            "13@Full_IVF_Path_And_FileName@2@BitRate\n"
            "12@Full_IVF_Path_And_FileName@2@BitRate\n"
            "13@Full_IVF_Path_And_FileName@2@BitRate@Name_of_Newest_Debug_PlugIn@Name_of_Newest_Release_PlugIn\n"
            "15@Full_IVF_Path_And_FileName@2@BitRate\n"
            "18@Full_IVF_Path_And_FileName@2@BitRate@ForceKeyFrame\n"
            "20@Full_IVF_Path_And_FileName@2@BitRate\n"
            "%%21 DNE - Does Not Exist in VP8\n"
            "%%22 DNE - Does Not Exist in VP8\n"
            "21@Full_IVF_Path_And_FileName@2@BitRate\n"
            "22@Full_IVF_Path_And_FileName@2@BitRate@Name_of_Newest_Debug_PlugIn\n"
            "24@Full_IVF_Path_And_FileName@2@BitRate\n"
            "26@Full_IVF_Path_And_FileName@2@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "%%28 DNE - Does Not Exist in VP8\n"
            "28@Full_IVF_Path_And_FileName@2@BitRate\n"
            "31@Full_IVF_Path_And_FileName@2@BitRate\n"
            "32@Full_IVF_Path_And_FileName@2@BitRate\n"
            "%%33 DNW - Does Not Work In Multi Run\n"
            "33@Full_IVF_Path_And_FileName@2@BitRate\n"
            "%%35 DNW - Does Not Work In Multi Run\n"
            "%%36 DNE - Does Not Exist in VP8\n"
            "37@Full_IVF_Path_And_FileName@2@BitRate\n"
            "%%39 Padding\n"
            "%%40 Padding\n"
            "2@Full_IVF_Path_And_FileName@2@BitRate\n"
            "5@Full_IVF_Path_And_FileName@2@BitRate@Alt_Freq_1@Alt_Freq_2\n"
            "17@Full_IVF_Path_And_FileName@2@BitRate@FixedQ_1@FixedQ_2\n"
            "20@Full_IVF_Path_And_FileName@2@BitRate@Lag_in_Frames_1@Lag_in_Frames_2\n"
            "30@Full_IVF_Path_And_FileName@2@BitRate\n"
            "38@Full_IVF_Path_And_FileName@2@BitRate\n"
            "14@Full_IVF_Path_And_FileName@2@BitRate\n\n"

            "%%%%%%%%%%%%%%Mode4%%%%%%%%%%%%%%\n"
            "1@Full_IVF_Path_And_FileName@4@BitRate\n"
            "4@Full_IVF_Path_And_FileName@4@BitRate\n"
            "4@Full_IVF_Path_And_FileName@4@BitRate\n"
            "6@Full_IVF_Path_And_FileName@4@BitRate\n"
            "7@Full_IVF_Path_And_FileName@4@BitRate\n"
            "8@Full_IVF_Path_And_FileName@4@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@4@BitRate@Version\n"
            "10@Full_IVF_Path_And_FileName@4@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@4@BitRate\n"
            "10@Full_IVF_Path_And_FileName@4@BitRate\n"
            "%%11 DNW - Does Not Work In Multi Run\n"
            "11@Full_IVF_Path_And_FileName@4@BitRate\n"
            "13@Full_IVF_Path_And_FileName@4@BitRate\n"
            "12@Full_IVF_Path_And_FileName@4@BitRate\n"
            "13@Full_IVF_Path_And_FileName@4@BitRate@Name_of_Newest_Debug_PlugIn@Name_of_Newest_Release_PlugIn\n"
            "15@Full_IVF_Path_And_FileName@4@BitRate\n"
            "18@Full_IVF_Path_And_FileName@4@BitRate@ForceKeyFrame\n"
            "20@Full_IVF_Path_And_FileName@4@BitRate\n"
            "%%21 DNE - Does Not Exist in VP8\n"
            "%%22 DNE - Does Not Exist in VP8\n"
            "21@Full_IVF_Path_And_FileName@4@BitRate\n"
            "22@Full_IVF_Path_And_FileName@4@BitRate@Name_of_Newest_Debug_PlugIn\n"
            "24@Full_IVF_Path_And_FileName@4@BitRate\n"
            "26@Full_IVF_Path_And_FileName@4@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "%%28 DNE - Does Not Exist in VP8\n"
            "28@Full_IVF_Path_And_FileName@4@BitRate\n"
            "31@Full_IVF_Path_And_FileName@4@BitRate\n"
            "32@Full_IVF_Path_And_FileName@4@BitRate\n"
            "%%33 DNW - Does Not Work In Multi Run\n"
            "33@Full_IVF_Path_And_FileName@4@BitRate\n"
            "%%35 DNW - Does Not Work In Multi Run\n"
            "%%36 DNE - Does Not Exist in VP8\n"
            "37@Full_IVF_Path_And_FileName@4@BitRate\n"
            "%%39 Padding\n"
            "%%40 Padding\n"
            "2@Full_IVF_Path_And_FileName@4@BitRate\n"
            "5@Full_IVF_Path_And_FileName@4@BitRate@Alt_Freq_1@Alt_Freq_2\n"
            "17@Full_IVF_Path_And_FileName@4@BitRate@FixedQ_1@FixedQ_2\n"
            "20@Full_IVF_Path_And_FileName@4@BitRate@Lag_in_Frames_1@Lag_in_Frames_2\n"
            "30@Full_IVF_Path_And_FileName@4@BitRate\n"
            "38@Full_IVF_Path_And_FileName@4@BitRate\n"
            "14@Full_IVF_Path_And_FileName@4@BitRate\n\n"

            "%%%%%%%%%%%%%%Mode5%%%%%%%%%%%%%%\n"
            "1@Full_IVF_Path_And_FileName@5@BitRate\n"
            "4@Full_IVF_Path_And_FileName@5@BitRate\n"
            "4@Full_IVF_Path_And_FileName@5@BitRate\n"
            "6@Full_IVF_Path_And_FileName@5@BitRate\n"
            "7@Full_IVF_Path_And_FileName@5@BitRate\n"
            "8@Full_IVF_Path_And_FileName@5@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@5@BitRate@Version\n"
            "10@Full_IVF_Path_And_FileName@5@BitRate@Version\n"
            "9@Full_IVF_Path_And_FileName@5@BitRate\n"
            "10@Full_IVF_Path_And_FileName@5@BitRate\n"
            "%%11 DNW - Does Not Work In Multi Run\n"
            "11@Full_IVF_Path_And_FileName@5@BitRate\n"
            "13@Full_IVF_Path_And_FileName@5@BitRate\n"
            "12@Full_IVF_Path_And_FileName@5@BitRate\n"
            "13@Full_IVF_Path_And_FileName@5@BitRate@Name_of_Newest_Debug_PlugIn@Name_of_Newest_Release_PlugIn\n"
            "15@Full_IVF_Path_And_FileName@5@BitRate\n"
            "18@Full_IVF_Path_And_FileName@5@BitRate@ForceKeyFrame\n"
            "20@Full_IVF_Path_And_FileName@5@BitRate\n"
            "%%21 DNE - Does Not Exist in VP8\n"
            "%%22 DNE - Does Not Exist in VP8\n"
            "21@Full_IVF_Path_And_FileName@5@BitRate\n"
            "22@Full_IVF_Path_And_FileName@5@BitRate@Name_of_Newest_Debug_PlugIn\n"
            "24@Full_IVF_Path_And_FileName@5@BitRate\n"
            "26@Full_IVF_Path_And_FileName@5@BitRate@Name_of_Oldest_Release_PlugIn\n"
            "%%28 DNE - Does Not Exist in VP8\n"
            "28@Full_IVF_Path_And_FileName@5@BitRate\n"
            "31@Full_IVF_Path_And_FileName@5@BitRate\n"
            "32@Full_IVF_Path_And_FileName@5@BitRate\n"
            "%%33 DNW - Does Not Work In Multi Run\n"
            "33@Full_IVF_Path_And_FileName@5@BitRate\n"
            "%%35 DNW - Does Not Work In Multi Run\n"
            "%%36 DNE - Does Not Exist in VP8\n"
            "37@Full_IVF_Path_And_FileName@5@BitRate\n"
            "%%39 Padding\n"
            "%%40 Padding\n"
            "2@Full_IVF_Path_And_FileName@5@BitRate\n"
            "5@Full_IVF_Path_And_FileName@5@BitRate@Alt_Freq_1@Alt_Freq_2\n"
            "17@Full_IVF_Path_And_FileName@5@BitRate@FixedQ_1@FixedQ_2\n"
            "20@Full_IVF_Path_And_FileName@5@BitRate@Lag_in_Frames_1@Lag_in_Frames_2\n"
            "30@Full_IVF_Path_And_FileName@5@BitRate\n"
            "38@Full_IVF_Path_And_FileName@5@BitRate\n"
            "14@Full_IVF_Path_And_FileName@5@BitRate\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
           );

    printf("\n\nSample Multi Run text file created:\n%s\n\n", TextfileString4.c_str());
    fclose(fp4);
}
void Print5(string WorkingDir)
{
    string TextfileString5 = WorkingDir;
    TextfileString5.erase(TextfileString5.end() - 25, TextfileString5.end());
    //TextfileString5.append(slashCharStr);
    TextfileString5.append("QuickTest_32Bit.txt");

    FILE *fp5;

    if ((fp5 = freopen(TextfileString5.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString5.c_str());
        exit(1);
    }

#if defined(_WIN32)
    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%Instructions: Replace the following: /home/jberry/CodecSDKTestVP8/MasterFile/SampleTestMaterial/src16.ivf , 128, Name_of_Oldest_Release_\n"
            "%%              PlugIn, 6, VP8vNewest_PlugIn_DLib_DMode_API.exe, VP8vNewest_PlugIn_RLib_RMode_32Bit.exe, \n"
            "%%              10, 100, 100, 20, and 100 with \n"
            "%%              the settings you wish to test.\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%\n"
            "16@..\\TestClips\\src16.ivf\n"
            "%%49@..\\TestClips\\src16.ivf@1@128@0@NA@NA@NA\n"
            "19@..\\TestClips\\src16.ivf@128\n"
            "27@..\\TestClips\\src16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "29@..\\TestClips\\src16.ivf@128\n"
            "36@..\\TestClips\\src16.ivf@128\n"
            "%%51@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "35@..\\TestClips\\TestVectors\n"
            "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n"
            "25@..\\TestClips\\BBB_720x480_2000F.ivf@0@128\n"
            "25@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "34@..\\TestClips\\src16.ivf@0@128@10\n"
            "34@..\\TestClips\\src16.ivf@1@128@10\n"
            "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n"
            "\n"
            "%%%%Mode0%%%%\n"
            "\n"
            "7@..\\TestClips\\src16.ivf@0@128\n"
            "12@..\\TestClips\\src16.ivf@0@128\n"
            "15@..\\TestClips\\src16.ivf@0@128\n"
            "18@..\\TestClips\\src16.ivf@0@128@6\n"
            "21@..\\TestClips\\src16.ivf@0@128\n"
            "22@..\\TestClips\\src16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@0@128\n"
            "26@..\\TestClips\\src16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@0@128\n"
            "31@..\\TestClips\\src16.ivf@0@128\n"
            "%%34@..\\TestClips\\src16.ivf@0@128\n"
            "38@..\\TestClips\\src16.ivf@0@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@0@128\n"
            "\n"
            "%%%%Mode1%%%%\n"
            "1@..\\TestClips\\BBB_1280x720_1500F.ivf@1@128\n"
            "4@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "%%3@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "6@..\\TestClips\\src16.ivf@1@128\n"
            "7@..\\TestClips\\src16.ivf@1@128\n"
            "8@..\\TestClips\\src16.ivf@1@128@0\n"
            "8@..\\TestClips\\src16.ivf@1@128@1\n"
            "9@..\\TestClips\\src16.ivf@1@128@0\n"
            "9@..\\TestClips\\src16.ivf@1@128@1\n"
            "10@..\\TestClips\\src16.ivf@1@128@0\n"
            "10@..\\TestClips\\src16.ivf@1@128@1\n"
            "10@..\\TestClips\\src16.ivf@1@128@1@..\\TestClips\\SpecialCaseParameterFiles\\1ChangeCPUWorksCPUUsage4.txt\n"
            "11@..\\TestClips\\src16.ivf@1@128\n"
            "12@..\\TestClips\\src16.ivf@1@128\n"
            "12@..\\TestClips\\BBB_1280x720_1500F.ivf@1@2048\n"
            "13@..\\TestClips\\src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@1@128\n"
            "18@..\\TestClips\\src16.ivf@1@128@6\n"
            "21@..\\TestClips\\src16.ivf@1@128\n"
            "22@..\\TestClips\\src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@1@128\n"
            "26@..\\TestClips\\src16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@1@128\n"
            "31@..\\TestClips\\src16.ivf@1@128\n"
            "32@..\\TestClips\\src16.ivf@1@128\n"
            "%%34@..\\TestClips\\src16.ivf@1@128\n"
            "37@..\\TestClips\\src16.ivf@1@128\n"
            "%%42@..\\TestClips\\src16.ivf@1@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@1@128@-1@100\n"
            "38@..\\TestClips\\src16.ivf@1@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "\n"
            "%%%%Mode2%%%%\n"
            "%%3@..\\TestClips\\src16.ivf@2@128\n"
            "6@..\\TestClips\\src16.ivf@2@128\n"
            "7@..\\TestClips\\src16.ivf@2@128\n"
            "8@..\\TestClips\\src16.ivf@2@128@0\n"
            "8@..\\TestClips\\src16.ivf@2@128@1\n"
            "9@..\\TestClips\\src16.ivf@2@128@0\n"
            "9@..\\TestClips\\src16.ivf@2@128@1\n"
            "10@..\\TestClips\\src16.ivf@2@128@0\n"
            "10@..\\TestClips\\src16.ivf@2@128@1\n"
            "11@..\\TestClips\\src16.ivf@2@128\n"
            "12@..\\TestClips\\src16.ivf@2@128\n"
            "13@..\\TestClips\\src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@2@128\n"
            "18@..\\TestClips\\src16.ivf@2@128@6\n"
            "21@..\\TestClips\\src16.ivf@2@128\n"
            "22@..\\TestClips\\src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@2@128\n"
            "26@..\\TestClips\\src16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@2@128\n"
            "31@..\\TestClips\\src16.ivf@2@128\n"
            "32@..\\TestClips\\src16.ivf@2@128\n"
            "%%34@..\\TestClips\\src16.ivf@2@128\n"
            "37@..\\TestClips\\src16.ivf@2@128\n"
            "%%42@..\\TestClips\\src16.ivf@2@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@2@128@-1@100\n"
            "38@..\\TestClips\\src16.ivf@2@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@2@128\n"

            "%%%%Mode4%%%%\n"
            "%%3@..\\TestClips\\src16.ivf@4@128\n"
            "6@..\\TestClips\\src16.ivf@4@128\n"
            "7@..\\TestClips\\src16.ivf@4@128\n"
            "8@..\\TestClips\\src16.ivf@4@128@0\n"
            "8@..\\TestClips\\src16.ivf@4@128@1\n"
            "9@..\\TestClips\\src16.ivf@4@128@0\n"
            "9@..\\TestClips\\src16.ivf@4@128@1\n"
            "10@..\\TestClips\\src16.ivf@4@128@0\n"
            "10@..\\TestClips\\src16.ivf@4@128@1\n"
            "11@..\\TestClips\\src16.ivf@4@128\n"
            "12@..\\TestClips\\src16.ivf@4@128\n"
            "13@..\\TestClips\\src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@4@128\n"
            "18@..\\TestClips\\src16.ivf@4@128@6\n"
            "21@..\\TestClips\\src16.ivf@4@128\n"
            "22@..\\TestClips\\src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@4@128\n"
            "26@..\\TestClips\\src16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@4@128\n"
            "31@..\\TestClips\\src16.ivf@4@128\n"
            "32@..\\TestClips\\src16.ivf@4@128\n"
            "%%34@..\\TestClips\\src16.ivf@4@128\n"
            "37@..\\TestClips\\src16.ivf@4@128\n"
            "2@..\\TestClips\\src16.ivf@4@128\n"
            "%%42@..\\TestClips\\src16.ivf@4@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@4@128@-1@100\n"
            "20@..\\TestClips\\src16.ivf@4@128@10@24\n"
            "30@..\\TestClips\\src16.ivf@4@128\n"
            "38@..\\TestClips\\src16.ivf@4@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@4@128\n"
            "\n"
            "%%%%Mode5%%%%\n"
            "%%3@..\\TestClips\\src16.ivf@5@128\n"
            "6@..\\TestClips\\src16.ivf@5@128\n"
            "7@..\\TestClips\\src16.ivf@5@128\n"
            "8@..\\TestClips\\src16.ivf@5@128@0\n"
            "8@..\\TestClips\\src16.ivf@5@128@1\n"
            "9@..\\TestClips\\src16.ivf@5@128@0\n"
            "9@..\\TestClips\\src16.ivf@5@128@1\n"
            "10@..\\TestClips\\src16.ivf@5@128@0\n"
            "10@..\\TestClips\\src16.ivf@5@128@1\n"
            "11@..\\TestClips\\src16.ivf@5@128\n"
            "12@..\\TestClips\\src16.ivf@5@128\n"
            "13@..\\TestClips\\src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@5@128\n"
            "18@..\\TestClips\\src16.ivf@5@128@6\n"
            "21@..\\TestClips\\src16.ivf@5@128\n"
            "22@..\\TestClips\\src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@5@128\n"
            "26@..\\TestClips\\src16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@5@128\n"
            "31@..\\TestClips\\src16.ivf@5@128\n"
            "32@..\\TestClips\\src16.ivf@5@128\n"
            "%%34@..\\TestClips\\src16.ivf@5@128\n"
            "37@..\\TestClips\\src16.ivf@5@128\n"
            "2@..\\TestClips\\src16.ivf@5@128\n"
            "%%42@..\\TestClips\\src16.ivf@5@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@5@128@-1@100\n"
            "20@..\\TestClips\\src16.ivf@5@128@10@24\n"
            "30@..\\TestClips\\src16.ivf@5@128\n"
            "38@..\\TestClips\\src16.ivf@5@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@5@128\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
           );
#elif defined(__APPLE__)
    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%Instructions: Replace the following: /home/jberry/CodecSDKTestVP8/MasterFile/SampleTestMaterial/src16.ivf , 128, Name_of_Oldest_Release_\n"
            "%%             PlugIn, 6, VP8vNewest_PlugIn_DLib_DMode_API.exe, VP8vNewest_PlugIn_RLib_RMode_32Bit.exe, \n"
            "%%              10, 100, 100, 20, and 100 with \n"
            "%%              the settings you wish to test.\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "16@../TestClips/src16.ivf\n"
            "%%49@../TestClips/src16.ivf@1@128@0@NA@NA@NA\n"
            "19@../TestClips/src16.ivf@128\n"
            "27@../TestClips/src16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "29@../TestClips/src16.ivf@128\n"
            "36@../TestClips/src16.ivf@128\n"
            "%%51@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "35@../TestClips/TestVectors\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "34@../TestClips/src16.ivf@0@128@10\n"
            "34@../TestClips/src16.ivf@1@128@10\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "\n"
            "%%%%%%%%Mode0%%%%%%%%\n"
            "\n"
            "7@../TestClips/src16.ivf@0@128\n"
            "12@../TestClips/src16.ivf@0@128\n"
            "15@../TestClips/src16.ivf@0@128\n"
            "18@../TestClips/src16.ivf@0@128@6\n"
            "21@../TestClips/src16.ivf@0@128\n"
            "22@../TestClips/src16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@0@128\n"
            "26@../TestClips/src16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@0@128\n"
            "31@../TestClips/src16.ivf@0@128\n"
            "%%34@../TestClips/src16.ivf@0@128\n"
            "38@../TestClips/src16.ivf@0@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "\n"
            "%%%%%%%%Mode1%%%%%%%%\n"
            "1@../TestClips/BBB_1280x720_1500F.ivf@1@128\n"
            "4@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "%%3@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "6@../TestClips/src16.ivf@1@128\n"
            "7@../TestClips/src16.ivf@1@128\n"
            "8@../TestClips/src16.ivf@1@128@0\n"
            "8@../TestClips/src16.ivf@1@128@1\n"
            "9@../TestClips/src16.ivf@1@128@0\n"
            "9@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@0\n"
            "10@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@1@../TestClips/SpecialCaseParameterFiles/1ChangeCPUWorksCPUUsage4.txt\n"
            "11@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/BBB_1280x720_1500F.ivf@1@2048\n"
            "13@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@1@128\n"
            "18@../TestClips/src16.ivf@1@128@6\n"
            "21@../TestClips/src16.ivf@1@128\n"
            "22@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@1@128\n"
            "26@../TestClips/src16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@1@128\n"
            "31@../TestClips/src16.ivf@1@128\n"
            "32@../TestClips/src16.ivf@1@128\n"
            "%%34@../TestClips/src16.ivf@1@128\n"
            "37@../TestClips/src16.ivf@1@128\n"
            "%%%%42@../TestClips/src16.ivf@1@128@20@100\n"
            "17@../TestClips/src16.ivf@1@128@-1@100\n"
            "38@../TestClips/src16.ivf@1@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "\n"
            "%%%%%%%%Mode2%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@2@128\n"
            "6@../TestClips/src16.ivf@2@128\n"
            "7@../TestClips/src16.ivf@2@128\n"
            "8@../TestClips/src16.ivf@2@128@0\n"
            "8@../TestClips/src16.ivf@2@128@1\n"
            "9@../TestClips/src16.ivf@2@128@0\n"
            "9@../TestClips/src16.ivf@2@128@1\n"
            "10@../TestClips/src16.ivf@2@128@0\n"
            "10@../TestClips/src16.ivf@2@128@1\n"
            "11@../TestClips/src16.ivf@2@128\n"
            "12@../TestClips/src16.ivf@2@128\n"
            "13@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@2@128\n"
            "18@../TestClips/src16.ivf@2@128@6\n"
            "21@../TestClips/src16.ivf@2@128\n"
            "22@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@2@128\n"
            "26@../TestClips/src16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@2@128\n"
            "31@../TestClips/src16.ivf@2@128\n"
            "32@../TestClips/src16.ivf@2@128\n"
            "%%34@../TestClips/src16.ivf@2@128\n"
            "37@../TestClips/src16.ivf@2@128\n"
            "%%%%42@../TestClips/src16.ivf@2@128@20@100\n"
            "17@../TestClips/src16.ivf@2@128@-1@100\n"
            "38@../TestClips/src16.ivf@2@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@2@128\n"

            "%%%%%%%%Mode4%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@4@128\n"
            "6@../TestClips/src16.ivf@4@128\n"
            "7@../TestClips/src16.ivf@4@128\n"
            "8@../TestClips/src16.ivf@4@128@0\n"
            "8@../TestClips/src16.ivf@4@128@1\n"
            "9@../TestClips/src16.ivf@4@128@0\n"
            "9@../TestClips/src16.ivf@4@128@1\n"
            "10@../TestClips/src16.ivf@4@128@0\n"
            "10@../TestClips/src16.ivf@4@128@1\n"
            "11@../TestClips/src16.ivf@4@128\n"
            "12@../TestClips/src16.ivf@4@128\n"
            "13@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@4@128\n"
            "18@../TestClips/src16.ivf@4@128@6\n"
            "21@../TestClips/src16.ivf@4@128\n"
            "22@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@4@128\n"
            "26@../TestClips/src16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@4@128\n"
            "31@../TestClips/src16.ivf@4@128\n"
            "32@../TestClips/src16.ivf@4@128\n"
            "%%34@../TestClips/src16.ivf@4@128\n"
            "37@../TestClips/src16.ivf@4@128\n"
            "2@../TestClips/src16.ivf@4@128\n"
            "%%%%42@../TestClips/src16.ivf@4@128@20@100\n"
            "17@../TestClips/src16.ivf@4@128@-1@100\n"
            "20@../TestClips/src16.ivf@4@128@10@24\n"
            "30@../TestClips/src16.ivf@4@128\n"
            "38@../TestClips/src16.ivf@4@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@4@128\n"
            "\n"
            "%%%%%%%%Mode5%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@5@128\n"
            "6@../TestClips/src16.ivf@5@128\n"
            "7@../TestClips/src16.ivf@5@128\n"
            "8@../TestClips/src16.ivf@5@128@0\n"
            "8@../TestClips/src16.ivf@5@128@1\n"
            "9@../TestClips/src16.ivf@5@128@0\n"
            "9@../TestClips/src16.ivf@5@128@1\n"
            "10@../TestClips/src16.ivf@5@128@0\n"
            "10@../TestClips/src16.ivf@5@128@1\n"
            "11@../TestClips/src16.ivf@5@128\n"
            "12@../TestClips/src16.ivf@5@128\n"
            "13@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@5@128\n"
            "18@../TestClips/src16.ivf@5@128@6\n"
            "21@../TestClips/src16.ivf@5@128\n"
            "22@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@5@128\n"
            "26@../TestClips/src16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@5@128\n"
            "31@../TestClips/src16.ivf@5@128\n"
            "32@../TestClips/src16.ivf@5@128\n"
            "%%34@../TestClips/src16.ivf@5@128\n"
            "37@../TestClips/src16.ivf@5@128\n"
            "2@../TestClips/src16.ivf@5@128\n"
            "%%%%42@../TestClips/src16.ivf@5@128@20@100\n"
            "17@../TestClips/src16.ivf@5@128@-1@100\n"
            "20@../TestClips/src16.ivf@5@128@10@24\n"
            "30@../TestClips/src16.ivf@5@128\n"
            "38@../TestClips/src16.ivf@5@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@5@128\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
           );
#elif defined(linux)
    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%Instructions: Replace the following: /home/jberry/CodecSDKTestVP8/MasterFile/SampleTestMaterial/src16.ivf , 128, Name_of_Oldest_Release_\n"
            "%%             PlugIn, 6, VP8vNewest_PlugIn_DLib_DMode_API.exe, VP8vNewest_PlugIn_RLib_RMode_32Bit.exe, \n"
            "%%              10, 100, 100, 20, and 100 with \n"
            "%%              the settings you wish to test.\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "16@../TestClips/src16.ivf\n"
            "%%49@../TestClips/src16.ivf@1@128@0@NA@NA@NA\n"
            "19@../TestClips/src16.ivf@128\n"
            "27@../TestClips/src16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "29@../TestClips/src16.ivf@128\n"
            "36@../TestClips/src16.ivf@128\n"
            "%%51@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "35@../TestClips/TestVectors\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "34@../TestClips/src16.ivf@0@128@10\n"
            "34@../TestClips/src16.ivf@1@128@10\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "\n"
            "%%%%%%%%Mode0%%%%%%%%\n"
            "\n"
            "7@../TestClips/src16.ivf@0@128\n"
            "12@../TestClips/src16.ivf@0@128\n"
            "15@../TestClips/src16.ivf@0@128\n"
            "18@../TestClips/src16.ivf@0@128@6\n"
            "21@../TestClips/src16.ivf@0@128\n"
            "22@../TestClips/src16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@0@128\n"
            "26@../TestClips/src16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@0@128\n"
            "31@../TestClips/src16.ivf@0@128\n"
            "%%34@../TestClips/src16.ivf@0@128\n"
            "38@../TestClips/src16.ivf@0@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "\n"
            "%%%%%%%%Mode1%%%%%%%%\n"
            "1@../TestClips/BBB_1280x720_1500F.ivf@1@128\n"
            "4@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "%%3@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "6@../TestClips/src16.ivf@1@128\n"
            "7@../TestClips/src16.ivf@1@128\n"
            "8@../TestClips/src16.ivf@1@128@0\n"
            "8@../TestClips/src16.ivf@1@128@1\n"
            "9@../TestClips/src16.ivf@1@128@0\n"
            "9@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@0\n"
            "10@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@1@../TestClips/SpecialCaseParameterFiles/1ChangeCPUWorksCPUUsage4.txt\n"
            "11@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/BBB_1280x720_1500F.ivf@1@2048\n"
            "13@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@1@128\n"
            "18@../TestClips/src16.ivf@1@128@6\n"
            "21@../TestClips/src16.ivf@1@128\n"
            "22@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@1@128\n"
            "26@../TestClips/src16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@1@128\n"
            "31@../TestClips/src16.ivf@1@128\n"
            "32@../TestClips/src16.ivf@1@128\n"
            "%%34@../TestClips/src16.ivf@1@128\n"
            "37@../TestClips/src16.ivf@1@128\n"
            "%%%%42@../TestClips/src16.ivf@1@128@20@100\n"
            "17@../TestClips/src16.ivf@1@128@-1@100\n"
            "38@../TestClips/src16.ivf@1@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "\n"
            "%%%%%%%%Mode2%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@2@128\n"
            "6@../TestClips/src16.ivf@2@128\n"
            "7@../TestClips/src16.ivf@2@128\n"
            "8@../TestClips/src16.ivf@2@128@0\n"
            "8@../TestClips/src16.ivf@2@128@1\n"
            "9@../TestClips/src16.ivf@2@128@0\n"
            "9@../TestClips/src16.ivf@2@128@1\n"
            "10@../TestClips/src16.ivf@2@128@0\n"
            "10@../TestClips/src16.ivf@2@128@1\n"
            "11@../TestClips/src16.ivf@2@128\n"
            "12@../TestClips/src16.ivf@2@128\n"
            "13@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@2@128\n"
            "18@../TestClips/src16.ivf@2@128@6\n"
            "21@../TestClips/src16.ivf@2@128\n"
            "22@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@2@128\n"
            "26@../TestClips/src16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@2@128\n"
            "31@../TestClips/src16.ivf@2@128\n"
            "32@../TestClips/src16.ivf@2@128\n"
            "%%34@../TestClips/src16.ivf@2@128\n"
            "37@../TestClips/src16.ivf@2@128\n"
            "%%%%42@../TestClips/src16.ivf@2@128@20@100\n"
            "17@../TestClips/src16.ivf@2@128@-1@100\n"
            "38@../TestClips/src16.ivf@2@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@2@128\n"

            "%%%%%%%%Mode4%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@4@128\n"
            "6@../TestClips/src16.ivf@4@128\n"
            "7@../TestClips/src16.ivf@4@128\n"
            "8@../TestClips/src16.ivf@4@128@0\n"
            "8@../TestClips/src16.ivf@4@128@1\n"
            "9@../TestClips/src16.ivf@4@128@0\n"
            "9@../TestClips/src16.ivf@4@128@1\n"
            "10@../TestClips/src16.ivf@4@128@0\n"
            "10@../TestClips/src16.ivf@4@128@1\n"
            "11@../TestClips/src16.ivf@4@128\n"
            "12@../TestClips/src16.ivf@4@128\n"
            "13@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@4@128\n"
            "18@../TestClips/src16.ivf@4@128@6\n"
            "21@../TestClips/src16.ivf@4@128\n"
            "22@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@4@128\n"
            "26@../TestClips/src16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@4@128\n"
            "31@../TestClips/src16.ivf@4@128\n"
            "32@../TestClips/src16.ivf@4@128\n"
            "%%34@../TestClips/src16.ivf@4@128\n"
            "37@../TestClips/src16.ivf@4@128\n"
            "2@../TestClips/src16.ivf@4@128\n"
            "%%%%42@../TestClips/src16.ivf@4@128@20@100\n"
            "17@../TestClips/src16.ivf@4@128@-1@100\n"
            "20@../TestClips/src16.ivf@4@128@10@24\n"
            "30@../TestClips/src16.ivf@4@128\n"
            "38@../TestClips/src16.ivf@4@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@4@128\n"
            "\n"
            "%%%%%%%%Mode5%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@5@128\n"
            "6@../TestClips/src16.ivf@5@128\n"
            "7@../TestClips/src16.ivf@5@128\n"
            "8@../TestClips/src16.ivf@5@128@0\n"
            "8@../TestClips/src16.ivf@5@128@1\n"
            "9@../TestClips/src16.ivf@5@128@0\n"
            "9@../TestClips/src16.ivf@5@128@1\n"
            "10@../TestClips/src16.ivf@5@128@0\n"
            "10@../TestClips/src16.ivf@5@128@1\n"
            "11@../TestClips/src16.ivf@5@128\n"
            "12@../TestClips/src16.ivf@5@128\n"
            "13@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n"
            "15@../TestClips/src16.ivf@5@128\n"
            "18@../TestClips/src16.ivf@5@128@6\n"
            "21@../TestClips/src16.ivf@5@128\n"
            "22@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n"
            "24@../TestClips/src16.ivf@5@128\n"
            "26@../TestClips/src16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n"
            "28@../TestClips/src16.ivf@5@128\n"
            "31@../TestClips/src16.ivf@5@128\n"
            "32@../TestClips/src16.ivf@5@128\n"
            "%%34@../TestClips/src16.ivf@5@128\n"
            "37@../TestClips/src16.ivf@5@128\n"
            "2@../TestClips/src16.ivf@5@128\n"
            "%%%%42@../TestClips/src16.ivf@5@128@20@100\n"
            "17@../TestClips/src16.ivf@5@128@-1@100\n"
            "20@../TestClips/src16.ivf@5@128@10@24\n"
            "30@../TestClips/src16.ivf@5@128\n"
            "38@../TestClips/src16.ivf@5@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@5@128\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
           );
#endif

    printf("\n\nQuick Test file created:\n%s\n\n", TextfileString5.c_str());
    fclose(fp5);
}
void Print6(string WorkingDir)
{
    string TextfileString6 = WorkingDir;
    TextfileString6.erase(TextfileString6.end() - 25, TextfileString6.end());
    //TextfileString6.append(slashCharStr);
    TextfileString6.append("QuickTest_64Bit.txt");

    FILE *fp6;

    if ((fp6 = freopen(TextfileString6.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString6.c_str());
        exit(1);
    }

#if defined(_WIN32)
    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%Instructions: Replace the following: /home/jberry/CodecSDKTestVP8/MasterFile/SampleTestMaterial/src16.ivf , 128, Name_of_Oldest_Release_\n"
            "%%              PlugIn, 6, VP8vNewest_PlugIn_DLib_DMode_API.exe, VP8vNewest_PlugIn_RLib_RMode_64Bit.exe, \n"
            "%%              10, 100, 100, 20, and 100 with \n"
            "%%              the settings you wish to test.\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%\n"
            "16@..\\TestClips\\src16.ivf\n"
            "%%49@..\\TestClips\\src16.ivf@1@128@0@NA@NA@NA\n"
            "19@..\\TestClips\\src16.ivf@128\n"
            "27@..\\TestClips\\src16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "29@..\\TestClips\\src16.ivf@128\n"
            "36@..\\TestClips\\src16.ivf@128\n"
            "%%51@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "35@..\\TestClips\\TestVectors\n"
            "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n"
            "25@..\\TestClips\\BBB_720x480_2000F.ivf@0@128\n"
            "25@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "34@..\\TestClips\\src16.ivf@0@128@10\n"
            "34@..\\TestClips\\src16.ivf@1@128@10\n"
            "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n"
            "\n"
            "%%%%Mode0%%%%\n"
            "\n"
            "7@..\\TestClips\\src16.ivf@0@128\n"
            "12@..\\TestClips\\src16.ivf@0@128\n"
            "15@..\\TestClips\\src16.ivf@0@128\n"
            "18@..\\TestClips\\src16.ivf@0@128@6\n"
            "21@..\\TestClips\\src16.ivf@0@128\n"
            "22@..\\TestClips\\src16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@0@128\n"
            "26@..\\TestClips\\src16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@0@128\n"
            "31@..\\TestClips\\src16.ivf@0@128\n"
            "%%34@..\\TestClips\\src16.ivf@0@128\n"
            "38@..\\TestClips\\src16.ivf@0@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@0@128\n"
            "\n"
            "%%%%Mode1%%%%\n"
            "1@..\\TestClips\\BBB_1280x720_1500F.ivf@1@128\n"
            "4@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "%%3@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "6@..\\TestClips\\src16.ivf@1@128\n"
            "7@..\\TestClips\\src16.ivf@1@128\n"
            "8@..\\TestClips\\src16.ivf@1@128@0\n"
            "8@..\\TestClips\\src16.ivf@1@128@1\n"
            "9@..\\TestClips\\src16.ivf@1@128@0\n"
            "9@..\\TestClips\\src16.ivf@1@128@1\n"
            "10@..\\TestClips\\src16.ivf@1@128@0\n"
            "10@..\\TestClips\\src16.ivf@1@128@1\n"
            "10@..\\TestClips\\src16.ivf@1@128@1@..\\TestClips\\SpecialCaseParameterFiles\\1ChangeCPUWorksCPUUsage4.txt\n"
            "11@..\\TestClips\\src16.ivf@1@128\n"
            "12@..\\TestClips\\src16.ivf@1@128\n"
            "12@..\\TestClips\\BBB_1280x720_1500F.ivf@1@2048\n"
            "13@..\\TestClips\\src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@1@128\n"
            "18@..\\TestClips\\src16.ivf@1@128@6\n"
            "21@..\\TestClips\\src16.ivf@1@128\n"
            "22@..\\TestClips\\src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@1@128\n"
            "26@..\\TestClips\\src16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@1@128\n"
            "31@..\\TestClips\\src16.ivf@1@128\n"
            "32@..\\TestClips\\src16.ivf@1@128\n"
            "%%34@..\\TestClips\\src16.ivf@1@128\n"
            "37@..\\TestClips\\src16.ivf@1@128\n"
            "%%42@..\\TestClips\\src16.ivf@1@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@1@128@-1@100\n"
            "38@..\\TestClips\\src16.ivf@1@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@1@128\n"
            "\n"
            "%%%%Mode2%%%%\n"
            "%%3@..\\TestClips\\src16.ivf@2@128\n"
            "6@..\\TestClips\\src16.ivf@2@128\n"
            "7@..\\TestClips\\src16.ivf@2@128\n"
            "8@..\\TestClips\\src16.ivf@2@128@0\n"
            "8@..\\TestClips\\src16.ivf@2@128@1\n"
            "9@..\\TestClips\\src16.ivf@2@128@0\n"
            "9@..\\TestClips\\src16.ivf@2@128@1\n"
            "10@..\\TestClips\\src16.ivf@2@128@0\n"
            "10@..\\TestClips\\src16.ivf@2@128@1\n"
            "11@..\\TestClips\\src16.ivf@2@128\n"
            "12@..\\TestClips\\src16.ivf@2@128\n"
            "13@..\\TestClips\\src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@2@128\n"
            "18@..\\TestClips\\src16.ivf@2@128@6\n"
            "21@..\\TestClips\\src16.ivf@2@128\n"
            "22@..\\TestClips\\src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@2@128\n"
            "26@..\\TestClips\\src16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@2@128\n"
            "31@..\\TestClips\\src16.ivf@2@128\n"
            "32@..\\TestClips\\src16.ivf@2@128\n"
            "%%34@..\\TestClips\\src16.ivf@2@128\n"
            "37@..\\TestClips\\src16.ivf@2@128\n"
            "%%42@..\\TestClips\\src16.ivf@2@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@2@128@-1@100\n"
            "38@..\\TestClips\\src16.ivf@2@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@2@128\n"

            "%%%%Mode4%%%%\n"
            "%%3@..\\TestClips\\src16.ivf@4@128\n"
            "6@..\\TestClips\\src16.ivf@4@128\n"
            "7@..\\TestClips\\src16.ivf@4@128\n"
            "8@..\\TestClips\\src16.ivf@4@128@0\n"
            "8@..\\TestClips\\src16.ivf@4@128@1\n"
            "9@..\\TestClips\\src16.ivf@4@128@0\n"
            "9@..\\TestClips\\src16.ivf@4@128@1\n"
            "10@..\\TestClips\\src16.ivf@4@128@0\n"
            "10@..\\TestClips\\src16.ivf@4@128@1\n"
            "11@..\\TestClips\\src16.ivf@4@128\n"
            "12@..\\TestClips\\src16.ivf@4@128\n"
            "13@..\\TestClips\\src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@4@128\n"
            "18@..\\TestClips\\src16.ivf@4@128@6\n"
            "21@..\\TestClips\\src16.ivf@4@128\n"
            "22@..\\TestClips\\src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@4@128\n"
            "26@..\\TestClips\\src16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@4@128\n"
            "31@..\\TestClips\\src16.ivf@4@128\n"
            "32@..\\TestClips\\src16.ivf@4@128\n"
            "%%34@..\\TestClips\\src16.ivf@4@128\n"
            "37@..\\TestClips\\src16.ivf@4@128\n"
            "2@..\\TestClips\\src16.ivf@4@128\n"
            "%%42@..\\TestClips\\src16.ivf@4@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@4@128@-1@100\n"
            "20@..\\TestClips\\src16.ivf@4@128@10@24\n"
            "30@..\\TestClips\\src16.ivf@4@128\n"
            "38@..\\TestClips\\src16.ivf@4@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@4@128\n"
            "\n"
            "%%%%Mode5%%%%\n"
            "%%3@..\\TestClips\\src16.ivf@5@128\n"
            "6@..\\TestClips\\src16.ivf@5@128\n"
            "7@..\\TestClips\\src16.ivf@5@128\n"
            "8@..\\TestClips\\src16.ivf@5@128@0\n"
            "8@..\\TestClips\\src16.ivf@5@128@1\n"
            "9@..\\TestClips\\src16.ivf@5@128@0\n"
            "9@..\\TestClips\\src16.ivf@5@128@1\n"
            "10@..\\TestClips\\src16.ivf@5@128@0\n"
            "10@..\\TestClips\\src16.ivf@5@128@1\n"
            "11@..\\TestClips\\src16.ivf@5@128\n"
            "12@..\\TestClips\\src16.ivf@5@128\n"
            "13@..\\TestClips\\src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@..\\TestClips\\src16.ivf@5@128\n"
            "18@..\\TestClips\\src16.ivf@5@128@6\n"
            "21@..\\TestClips\\src16.ivf@5@128\n"
            "22@..\\TestClips\\src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@..\\TestClips\\src16.ivf@5@128\n"
            "26@..\\TestClips\\src16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@..\\TestClips\\src16.ivf@5@128\n"
            "31@..\\TestClips\\src16.ivf@5@128\n"
            "32@..\\TestClips\\src16.ivf@5@128\n"
            "%%34@..\\TestClips\\src16.ivf@5@128\n"
            "37@..\\TestClips\\src16.ivf@5@128\n"
            "2@..\\TestClips\\src16.ivf@5@128\n"
            "%%42@..\\TestClips\\src16.ivf@5@128@20@100\n"
            "17@..\\TestClips\\src16.ivf@5@128@-1@100\n"
            "20@..\\TestClips\\src16.ivf@5@128@10@24\n"
            "30@..\\TestClips\\src16.ivf@5@128\n"
            "38@..\\TestClips\\src16.ivf@5@128\n"
            "14@..\\TestClips\\BBB_720x480_2000F.ivf@5@128\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
           );
#elif defined(__APPLE__)
    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%Instructions: Replace the following: /home/jberry/CodecSDKTestVP8/MasterFile/SampleTestMaterial/src16.ivf , 128, Name_of_Oldest_Release_\n"
            "%%             PlugIn, 6, VP8vNewest_PlugIn_DLib_DMode_API.exe, VP8vNewest_PlugIn_RLib_RMode_64Bit.exe, \n"
            "%%              10, 100, 100, 20, and 100 with \n"
            "%%              the settings you wish to test.\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "16@../TestClips/src16.ivf\n"
            "%%49@../TestClips/src16.ivf@1@128@0@NA@NA@NA\n"
            "19@../TestClips/src16.ivf@128\n"
            "27@../TestClips/src16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "29@../TestClips/src16.ivf@128\n"
            "36@../TestClips/src16.ivf@128\n"
            "%%51@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "35@../TestClips/TestVectors\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "34@../TestClips/src16.ivf@0@128@10\n"
            "34@../TestClips/src16.ivf@1@128@10\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "\n"
            "%%%%%%%%Mode0%%%%%%%%\n"
            "\n"
            "7@../TestClips/src16.ivf@0@128\n"
            "12@../TestClips/src16.ivf@0@128\n"
            "15@../TestClips/src16.ivf@0@128\n"
            "18@../TestClips/src16.ivf@0@128@6\n"
            "21@../TestClips/src16.ivf@0@128\n"
            "22@../TestClips/src16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@0@128\n"
            "26@../TestClips/src16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@0@128\n"
            "31@../TestClips/src16.ivf@0@128\n"
            "%%34@../TestClips/src16.ivf@0@128\n"
            "38@../TestClips/src16.ivf@0@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "\n"
            "%%%%%%%%Mode1%%%%%%%%\n"
            "1@../TestClips/BBB_1280x720_1500F.ivf@1@128\n"
            "4@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "%%3@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "6@../TestClips/src16.ivf@1@128\n"
            "7@../TestClips/src16.ivf@1@128\n"
            "8@../TestClips/src16.ivf@1@128@0\n"
            "8@../TestClips/src16.ivf@1@128@1\n"
            "9@../TestClips/src16.ivf@1@128@0\n"
            "9@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@0\n"
            "10@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@1@../TestClips/SpecialCaseParameterFiles/1ChangeCPUWorksCPUUsage4.txt\n"
            "11@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/BBB_1280x720_1500F.ivf@1@2048\n"
            "13@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@1@128\n"
            "18@../TestClips/src16.ivf@1@128@6\n"
            "21@../TestClips/src16.ivf@1@128\n"
            "22@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@1@128\n"
            "26@../TestClips/src16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@1@128\n"
            "31@../TestClips/src16.ivf@1@128\n"
            "32@../TestClips/src16.ivf@1@128\n"
            "%%34@../TestClips/src16.ivf@1@128\n"
            "37@../TestClips/src16.ivf@1@128\n"
            "%%%%42@../TestClips/src16.ivf@1@128@20@100\n"
            "17@../TestClips/src16.ivf@1@128@-1@100\n"
            "38@../TestClips/src16.ivf@1@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "\n"
            "%%%%%%%%Mode2%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@2@128\n"
            "6@../TestClips/src16.ivf@2@128\n"
            "7@../TestClips/src16.ivf@2@128\n"
            "8@../TestClips/src16.ivf@2@128@0\n"
            "8@../TestClips/src16.ivf@2@128@1\n"
            "9@../TestClips/src16.ivf@2@128@0\n"
            "9@../TestClips/src16.ivf@2@128@1\n"
            "10@../TestClips/src16.ivf@2@128@0\n"
            "10@../TestClips/src16.ivf@2@128@1\n"
            "11@../TestClips/src16.ivf@2@128\n"
            "12@../TestClips/src16.ivf@2@128\n"
            "13@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@2@128\n"
            "18@../TestClips/src16.ivf@2@128@6\n"
            "21@../TestClips/src16.ivf@2@128\n"
            "22@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@2@128\n"
            "26@../TestClips/src16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@2@128\n"
            "31@../TestClips/src16.ivf@2@128\n"
            "32@../TestClips/src16.ivf@2@128\n"
            "%%34@../TestClips/src16.ivf@2@128\n"
            "37@../TestClips/src16.ivf@2@128\n"
            "%%%%42@../TestClips/src16.ivf@2@128@20@100\n"
            "17@../TestClips/src16.ivf@2@128@-1@100\n"
            "38@../TestClips/src16.ivf@2@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@2@128\n"

            "%%%%%%%%Mode4%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@4@128\n"
            "6@../TestClips/src16.ivf@4@128\n"
            "7@../TestClips/src16.ivf@4@128\n"
            "8@../TestClips/src16.ivf@4@128@0\n"
            "8@../TestClips/src16.ivf@4@128@1\n"
            "9@../TestClips/src16.ivf@4@128@0\n"
            "9@../TestClips/src16.ivf@4@128@1\n"
            "10@../TestClips/src16.ivf@4@128@0\n"
            "10@../TestClips/src16.ivf@4@128@1\n"
            "11@../TestClips/src16.ivf@4@128\n"
            "12@../TestClips/src16.ivf@4@128\n"
            "13@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@4@128\n"
            "18@../TestClips/src16.ivf@4@128@6\n"
            "21@../TestClips/src16.ivf@4@128\n"
            "22@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@4@128\n"
            "26@../TestClips/src16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@4@128\n"
            "31@../TestClips/src16.ivf@4@128\n"
            "32@../TestClips/src16.ivf@4@128\n"
            "%%34@../TestClips/src16.ivf@4@128\n"
            "37@../TestClips/src16.ivf@4@128\n"
            "2@../TestClips/src16.ivf@4@128\n"
            "%%%%42@../TestClips/src16.ivf@4@128@20@100\n"
            "17@../TestClips/src16.ivf@4@128@-1@100\n"
            "20@../TestClips/src16.ivf@4@128@10@24\n"
            "30@../TestClips/src16.ivf@4@128\n"
            "38@../TestClips/src16.ivf@4@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@4@128\n"
            "\n"
            "%%%%%%%%Mode5%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@5@128\n"
            "6@../TestClips/src16.ivf@5@128\n"
            "7@../TestClips/src16.ivf@5@128\n"
            "8@../TestClips/src16.ivf@5@128@0\n"
            "8@../TestClips/src16.ivf@5@128@1\n"
            "9@../TestClips/src16.ivf@5@128@0\n"
            "9@../TestClips/src16.ivf@5@128@1\n"
            "10@../TestClips/src16.ivf@5@128@0\n"
            "10@../TestClips/src16.ivf@5@128@1\n"
            "11@../TestClips/src16.ivf@5@128\n"
            "12@../TestClips/src16.ivf@5@128\n"
            "13@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@5@128\n"
            "18@../TestClips/src16.ivf@5@128@6\n"
            "21@../TestClips/src16.ivf@5@128\n"
            "22@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@5@128\n"
            "26@../TestClips/src16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@5@128\n"
            "31@../TestClips/src16.ivf@5@128\n"
            "32@../TestClips/src16.ivf@5@128\n"
            "%%34@../TestClips/src16.ivf@5@128\n"
            "37@../TestClips/src16.ivf@5@128\n"
            "2@../TestClips/src16.ivf@5@128\n"
            "%%%%42@../TestClips/src16.ivf@5@128@20@100\n"
            "17@../TestClips/src16.ivf@5@128@-1@100\n"
            "20@../TestClips/src16.ivf@5@128@10@24\n"
            "30@../TestClips/src16.ivf@5@128\n"
            "38@../TestClips/src16.ivf@5@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@5@128\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
           );
#elif defined(linux)
    fprintf(stderr,
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%Instructions: Replace the following: /home/jberry/CodecSDKTestVP8/MasterFile/SampleTestMaterial/src16.ivf , 128, Name_of_Oldest_Release_\n"
            "%%             PlugIn, 6, VP8vNewest_PlugIn_DLib_DMode_API.exe, VP8vNewest_PlugIn_RLib_RMode_64Bit.exe, \n"
            "%%              10, 100, 100, 20, and 100 with \n"
            "%%              the settings you wish to test.\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Once Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "16@../TestClips/src16.ivf\n"
            "%%49@../TestClips/src16.ivf@1@128@0@NA@NA@NA\n"
            "19@../TestClips/src16.ivf@128\n"
            "27@../TestClips/src16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "29@../TestClips/src16.ivf@128\n"
            "36@../TestClips/src16.ivf@128\n"
            "%%51@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "35@../TestClips/TestVectors\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "25@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "34@../TestClips/src16.ivf@0@128@10\n"
            "34@../TestClips/src16.ivf@1@128@10\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "\n"
            "%%%%%%%%Mode0%%%%%%%%\n"
            "\n"
            "7@../TestClips/src16.ivf@0@128\n"
            "12@../TestClips/src16.ivf@0@128\n"
            "15@../TestClips/src16.ivf@0@128\n"
            "18@../TestClips/src16.ivf@0@128@6\n"
            "21@../TestClips/src16.ivf@0@128\n"
            "22@../TestClips/src16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@0@128\n"
            "26@../TestClips/src16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@0@128\n"
            "31@../TestClips/src16.ivf@0@128\n"
            "%%34@../TestClips/src16.ivf@0@128\n"
            "38@../TestClips/src16.ivf@0@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@0@128\n"
            "\n"
            "%%%%%%%%Mode1%%%%%%%%\n"
            "1@../TestClips/BBB_1280x720_1500F.ivf@1@128\n"
            "4@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "%%3@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "6@../TestClips/src16.ivf@1@128\n"
            "7@../TestClips/src16.ivf@1@128\n"
            "8@../TestClips/src16.ivf@1@128@0\n"
            "8@../TestClips/src16.ivf@1@128@1\n"
            "9@../TestClips/src16.ivf@1@128@0\n"
            "9@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@0\n"
            "10@../TestClips/src16.ivf@1@128@1\n"
            "10@../TestClips/src16.ivf@1@128@1@../TestClips/SpecialCaseParameterFiles/1ChangeCPUWorksCPUUsage4.txt\n"
            "11@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/src16.ivf@1@128\n"
            "12@../TestClips/BBB_1280x720_1500F.ivf@1@2048\n"
            "13@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@1@128\n"
            "18@../TestClips/src16.ivf@1@128@6\n"
            "21@../TestClips/src16.ivf@1@128\n"
            "22@../TestClips/src16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@1@128\n"
            "26@../TestClips/src16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@1@128\n"
            "31@../TestClips/src16.ivf@1@128\n"
            "32@../TestClips/src16.ivf@1@128\n"
            "%%34@../TestClips/src16.ivf@1@128\n"
            "37@../TestClips/src16.ivf@1@128\n"
            "%%%%42@../TestClips/src16.ivf@1@128@20@100\n"
            "17@../TestClips/src16.ivf@1@128@-1@100\n"
            "38@../TestClips/src16.ivf@1@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@1@128\n"
            "\n"
            "%%%%%%%%Mode2%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@2@128\n"
            "6@../TestClips/src16.ivf@2@128\n"
            "7@../TestClips/src16.ivf@2@128\n"
            "8@../TestClips/src16.ivf@2@128@0\n"
            "8@../TestClips/src16.ivf@2@128@1\n"
            "9@../TestClips/src16.ivf@2@128@0\n"
            "9@../TestClips/src16.ivf@2@128@1\n"
            "10@../TestClips/src16.ivf@2@128@0\n"
            "10@../TestClips/src16.ivf@2@128@1\n"
            "11@../TestClips/src16.ivf@2@128\n"
            "12@../TestClips/src16.ivf@2@128\n"
            "13@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@2@128\n"
            "18@../TestClips/src16.ivf@2@128@6\n"
            "21@../TestClips/src16.ivf@2@128\n"
            "22@../TestClips/src16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@2@128\n"
            "26@../TestClips/src16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@2@128\n"
            "31@../TestClips/src16.ivf@2@128\n"
            "32@../TestClips/src16.ivf@2@128\n"
            "%%34@../TestClips/src16.ivf@2@128\n"
            "37@../TestClips/src16.ivf@2@128\n"
            "%%%%42@../TestClips/src16.ivf@2@128@20@100\n"
            "17@../TestClips/src16.ivf@2@128@-1@100\n"
            "38@../TestClips/src16.ivf@2@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@2@128\n"

            "%%%%%%%%Mode4%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@4@128\n"
            "6@../TestClips/src16.ivf@4@128\n"
            "7@../TestClips/src16.ivf@4@128\n"
            "8@../TestClips/src16.ivf@4@128@0\n"
            "8@../TestClips/src16.ivf@4@128@1\n"
            "9@../TestClips/src16.ivf@4@128@0\n"
            "9@../TestClips/src16.ivf@4@128@1\n"
            "10@../TestClips/src16.ivf@4@128@0\n"
            "10@../TestClips/src16.ivf@4@128@1\n"
            "11@../TestClips/src16.ivf@4@128\n"
            "12@../TestClips/src16.ivf@4@128\n"
            "13@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@4@128\n"
            "18@../TestClips/src16.ivf@4@128@6\n"
            "21@../TestClips/src16.ivf@4@128\n"
            "22@../TestClips/src16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@4@128\n"
            "26@../TestClips/src16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@4@128\n"
            "31@../TestClips/src16.ivf@4@128\n"
            "32@../TestClips/src16.ivf@4@128\n"
            "%%34@../TestClips/src16.ivf@4@128\n"
            "37@../TestClips/src16.ivf@4@128\n"
            "2@../TestClips/src16.ivf@4@128\n"
            "%%%%42@../TestClips/src16.ivf@4@128@20@100\n"
            "17@../TestClips/src16.ivf@4@128@-1@100\n"
            "20@../TestClips/src16.ivf@4@128@10@24\n"
            "30@../TestClips/src16.ivf@4@128\n"
            "38@../TestClips/src16.ivf@4@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@4@128\n"
            "\n"
            "%%%%%%%%Mode5%%%%%%%%\n"
            "%%3@../TestClips/src16.ivf@5@128\n"
            "6@../TestClips/src16.ivf@5@128\n"
            "7@../TestClips/src16.ivf@5@128\n"
            "8@../TestClips/src16.ivf@5@128@0\n"
            "8@../TestClips/src16.ivf@5@128@1\n"
            "9@../TestClips/src16.ivf@5@128@0\n"
            "9@../TestClips/src16.ivf@5@128@1\n"
            "10@../TestClips/src16.ivf@5@128@0\n"
            "10@../TestClips/src16.ivf@5@128@1\n"
            "11@../TestClips/src16.ivf@5@128\n"
            "12@../TestClips/src16.ivf@5@128\n"
            "13@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n"
            "15@../TestClips/src16.ivf@5@128\n"
            "18@../TestClips/src16.ivf@5@128@6\n"
            "21@../TestClips/src16.ivf@5@128\n"
            "22@../TestClips/src16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n"
            "24@../TestClips/src16.ivf@5@128\n"
            "26@../TestClips/src16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n"
            "28@../TestClips/src16.ivf@5@128\n"
            "31@../TestClips/src16.ivf@5@128\n"
            "32@../TestClips/src16.ivf@5@128\n"
            "%%34@../TestClips/src16.ivf@5@128\n"
            "37@../TestClips/src16.ivf@5@128\n"
            "2@../TestClips/src16.ivf@5@128\n"
            "%%%%42@../TestClips/src16.ivf@5@128@20@100\n"
            "17@../TestClips/src16.ivf@5@128@-1@100\n"
            "20@../TestClips/src16.ivf@5@128@10@24\n"
            "30@../TestClips/src16.ivf@5@128\n"
            "38@../TestClips/src16.ivf@5@128\n"
            "14@../TestClips/BBB_720x480_2000F.ivf@5@128\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
           );
#endif

    printf("\n\nQuick Test file created:\n%s\n\n", TextfileString6.c_str());
    fclose(fp6);
}
void PrintTxtFiles(string WorkingDir)
{
    Print5(WorkingDir);//32BitQuickRun
    Print6(WorkingDir);//64BitQuickRun

    return;
}
void FormatedPrint(string SummaryStr, int selector)
{

    //selector == 1 -> Summary
    //selector == 2 -> Help
    //selector == 3 -> Function
    //selector == 4 -> Other non formatted output
    string SummaryStrOutput;

    if (selector == 1 || selector == 2 || selector == 3) //add padding for formating
    {
        SummaryStrOutput.append("         ");
    }

    int x = 0;

    while (x + 66 < SummaryStr.length())
    {
        //determine cut off to keep words whole
        int Cutoff;

        if (selector == 1 || selector == 2 || selector == 3)
        {
            Cutoff = 66;
        }

        if (selector == 4)
        {
            Cutoff = 79;
        }

        if (SummaryStr.substr(x + Cutoff + 1, 1).compare(" ") == 0 || SummaryStr.substr(x + Cutoff, 1).compare(" ") == 0)
        {
            Cutoff++;
        }
        else
        {
            while (SummaryStr.substr(x + Cutoff, 1).compare(" ") != 0)
            {
                Cutoff--;
            }

            Cutoff++;
        }

        //add the properly formated string to the output string
        SummaryStrOutput.append(SummaryStr.substr(x, Cutoff));

        if (selector == 1 || selector == 2 || selector == 3) //add padding for formating
        {
            SummaryStrOutput.append("\n         ");
        }

        x = x + Cutoff;

        while (SummaryStr.substr(x, 1).compare(" ") == 0)
        {
            x++;
        }
    }

    SummaryStrOutput.append(SummaryStr.substr(x, SummaryStr.length() - x));

    if (selector == 1)
    {
        printf("\n\nSummary:\n");
        printf("%s\n\n", SummaryStrOutput.c_str());
    }

    if (selector == 2)
    {
        printf("\n\n  Help:\n");
        printf("%s\n\n", SummaryStrOutput.c_str());
    }

    if (selector == 3)
    {
        printf("\n\nFunction:\n");
        printf("%s\n\n", SummaryStrOutput.c_str());
    }

    if (selector == 4)
    {
        fprintf(stderr, "%s", SummaryStrOutput.c_str());
    }

    return;
}
void TestHelp(int argc, char *argv[], string WorkingDir)
{

    string TestInputString = argv[1];
    int selector = atoi(argv[1]);

    if (TestInputString.compare("0") == 0)
    {
        printf(
            "\nUse:\n\n"
            " 0 ExternalTestRunner \n\n"
            "    <Test Type>\n"
            "         <1 - Create Files and Run Tests>\n"
            "         <2 - Create Files only>\n"
            "         <3 - Run Tests For Existing Files - Requires Input Test Directory>\n"
            "    <Input Text File>\n"
            "    <Optional - Input Test Directory>\n");

        FormatedPrint("Runs functions from an input file and summarizes results.  Test can be run in four modes.  Mode 1: Create Compressions and run tests.  Mode 2: Create Compressions Only.  Mode 3: Run Tests on Pre-existing Compressions and Mode 4 resume Tests in progress.  To create a template driver text file use the command: CreateSampleTextFiles", 1);
    }

    if (selector == 1)
    {
        printf("\nUse:\n\n"
               " 1 AllowDF \n\n"
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

        FormatedPrint("The test creates two files the first with Drop Frames on the second with Drop Frames off then tests them and compares sizes. If Drop Frames on is smaller than Drop Frames off the Test passes", 1);

    }

    if (selector == 2)
    {
        printf("\nUse:\n\n"
               "2  AllowLag \n\n"
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

        FormatedPrint("The Test creates two compressions the first with Allow Lag equal to 0 the second with Allow Lag equal to 1.  If the two files are not identical the test passes.", 1);
    }

    if (selector == 3)
    {
        printf("\nUse:\n\n"
               " 3 AllowSpatialResampling \n\n"
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

        FormatedPrint("The Test creates two files the first with  Spatial Resampling off the second with  Spatial Resampling on then tests them and compares sizes. If  Spatial Resampling on is smaller than Spatial Resampling off the Test passes", 1);
    }

    if (selector == 4)
    {
        printf("\nUse:\n\n"
               " 4 AllowSpatialResampling2 \n\n"
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

        FormatedPrint("The Test creates a file with AllowSpatialResampling on and Tests its PSNR. If its PSNR is greater than 15 the test passes.", 1);

    }

    if (selector == 5)
    {
        printf("\nUse:\n\n"
               "5  AltFreqTest \n\n"
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

        FormatedPrint("The Test creates two compressions each with user input Alternate Frequencies.  It then compares the compressions.  If the files are not identical the test passes.", 1);
    }

    if (selector == 6)
    {
        printf("\nUse:\n\n"
               " 6 AutoKeyFramingWorks \n\n"
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

        FormatedPrint("The Test Creates two files with identical parameters setting auto key frame equal to 1.  The test then checks to make sure that the key frames for both files occur in identical locations if so the Test passes. ", 1);

    }

    if (selector == 7)
    {
        printf("\nUse:\n\n"
               " 7 BufferLevelWorks \n\n"
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

        FormatedPrint("The Test creates a Compression  and runs CheckPBM on it if CheckPBM Returns -11 Test Passes.", 1);

    }

    if (selector == 8)
    {
        printf("\nUse:\n\n"
               " 8 CPUDecOnlyWorks \n\n"
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

        FormatedPrint("The Test creates a compression of the user input version and then decompresses it for On2_CPUID values ranging from 0 to 11. The Test then compares them against one another.  If all compressions are identical and the times to decompress them are not the test passes.", 1);

    }

    if (selector == 9)
    {
        printf("\nUse:\n\n"
               " 9 ChangeCPUDec \n\n"
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

        FormatedPrint("The Test creates a compression of the user input version and then decompresses it for On2_CPUID values 0 1 3 and 7.  The Test then compares them against one another.  If all compressions are identical and the times to compress them are not the test passes.", 1);

    }

    if (selector == 10)
    {
        printf("\nUse:\n\n"
               " 10 ChangeCPUWorks \n\n"
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

        FormatedPrint("The Test creates compressions of the user input version with On2_CPUID's 0 1 3 7 and 15.  If all compressions are identical and compression times are not the test passes.", 1);

    }

    if (selector == 11)
    {
        printf("\nUse:\n\n"
               "11 DFWMTest \n\n"
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

        FormatedPrint("The Test creates 6 Compressions with DFWM values of 100 80 60 40 20 and 0 and records their sizes.  If each successively lower DFWM compression has a equal or larger size than the previous the test passes.", 1);

    }

    if (selector == 12)
    {
        printf("\nUse:\n\n"
               "12 DataRateTest \n\n"
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

        FormatedPrint("The Test creates a compression and tests to make sure that it is with in 10%% of its target.  If it is it passes.", 1);

    }

    if (selector == 13)
    {
        printf("\nUse:\n\n"
               "13 DebugMatchesRelease \n\n"
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
              );

        FormatedPrint("The Test creates two files the first using an executable built using the release libraries the second built using an executable built using the debug libraries.  If the two files are identical the test is passed.", 1);

    }

    if (selector == 14)
    {
        printf("\nUse:\n\n"
               "14  EncoderBreakOutTest \n\n"
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

        FormatedPrint("The Test creates four compressions.  The first with an EncodeBreakout of 0, the second with an EncodeBreakout of 100, the thrid with an EncodeBreakout of 500 and the fourth with an EncodeBreakout of 1000.  Decompressions of the Encoded files are then carried out and PSNR values are calculated.  If the Decompressions run and the PSNR values of each succesive EncodeBreakout trial are with in 2 dB the test passes.  If the PSNRs are greater than 2 dB but less than 5 dB the test is inconclusive and if the PSNRs have greater than a 5 dB difference the test fails.", 1);
    }

    if (selector == 15)
    {
        printf("\nUse:\n\n"
               "15  ErrorRes \n\n"
               "    <inputfile>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n ");

        FormatedPrint("The Test Creates two compressions the first with Error Resilient Mode off the second on.  If their PSNRs are with in 10%% of one another the test passes.", 1);

    }

    if (selector == 16)
    {
        printf("\nUse:\n\n"
               "16 ExtraFileCheck \n\n"
               "    <Inputfile>\n"
               "	 <Optional Settings File>\n"
              );

        FormatedPrint("The Test Creates a two pass compression and checks the current directory, the directory the executable is located in and the directory the output file is written to for extra files.  If no extra files are found the test passes.", 1);
    }

    if (selector == 17)
    {
        printf("\nUse:\n\n"
               "17  FixedQ \n\n"
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

        FormatedPrint("The Test creates Two compressions each with user input Fixed Quantizers.  The Test then compares the compressions.  If the files are not identical the test passes.", 1);
    }

    if (selector == 18)
    {
        printf("\nUse:\n\n"
               "18 ForceKeyFrameWorks \n\n"
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
              );

        FormatedPrint("The test creates a compression using a user input value for Key Frame Frequency.  The resulting compression's Key Frames are then checked to makes sure that its key frames occur at least as frequently as Key Frame Frequency dictates.", 1);

    }

    if (selector == 19)
    {
        printf("\nUse:\n\n"
               "19 GoodQvBestQ \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n"
               "	 <Optional Settings File>\n"
              );

        FormatedPrint("The Test creates a compression in Good Quality and another in Best Quality Mode the Test then tests and Records their respective PSNRs and Data Rates.", 1);
    }

    if (selector == 20)
    {
        printf("\nUse:\n\n"
               "20  LagInFrames \n\n"
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

        FormatedPrint("The Test creates three compressions one with Allow Lag set to 0 the second and third with Allow Lag set to 1.  The second compression uses the first user in put Lag in Frames value for its Lag in frames and the third uses the second user input value for its Lag in Frames.  If none of the files are identical and the PSNRs of each successive file are with in 10%% of the last the test passes.", 1);
    }

    if (selector == 21)
    {
        printf("\nUse:\n\n"
               "21 MaxQTest \n\n"
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

        FormatedPrint("The Test Creates nine files the first with a WorstAllowedQ equal to 3 and each subsequent file with a WorstAllowedQ eight greater than the last until 63.  If the PSNRs of each WorstAllowedQ compression from 3 to 63 increase as Worst AllowedQ decreases the Test passes.", 1);

    }

    if (selector == 22)
    {
        printf("\nUse:\n\n"
               "22 MemLeakCheck \n\n"
               "    <inputfile>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n"
               "    <Newest Debug executable>\n"
               "	 <Optional Settings File>\n"
              );

        FormatedPrint("The Test creates a compression using the debug executable to check memory usage and record the results to an output file.  If no memory leaks are found the test passes.", 1);
    }

    if (selector == 23)
    {
        printf(
            "23  MemLeakCheck2 \n\n"
            "    <Mem Leak Check Exe>\n"
            "\n");

        FormatedPrint("The Test Opens and closes 10,000 instances of the Encoder and opens and closes 10,000 instance the Decoder and then checks to make sure there are no memory leaks.  If there are no Leaks the test passes.", 1);
    }

    if (selector == 24)
    {
        printf("\nUse:\n\n"
               "24 MinQTest \n\n"
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

        FormatedPrint("The Test creates two files the first with a MinQ equal to 10 the second with a  MinQ equal to 60.  If the first file has a higher PSNR than the second file the Test passes.", 1);
    }

    if (selector == 25)
    {
        printf("\nUse:\n\n"
               "25  MultiThreadedTest \n\n"
               "    <inputfile>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "    <Target Bit Rate>\n "
               "	 <Optional Settings File>\n"
              );

        FormatedPrint("The Test creates two compressions the first using a MultiThreaded equal to 14 the second using a MultiThreaded equal to 0.  The Test then compares the times to compress each.  If MultiThreaded 14 is faster than 0 the test passes.", 1);
    }

    if (selector == 26)
    {
        printf("\nUse:\n\n"
               "26 NewVsOldPSRN \n\n"
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
               "	 <Optional Settings File>\n"
               "\n"
              );

        FormatedPrint("The test creates two files the first using the newest version of VP8 and the second using a separate executable built using an older version.  It then tests the new vs the old and passes if the new beats the olds PSNR Number or is at least with in 1%% of the old.", 1);
    }

    if (selector == 27)
    {
        printf("\nUse:\n\n"
               "27 NewVsOldRealTimeSpeed \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n"
               "    <Exe File To Compare>\n"
               "	 <Optional Settings File>\n"
              );

        FormatedPrint("The test creates two files the first using the newest version of VP8 and the second using a separate executable built using an older version. The test records the time that each compression took and if the first file compresses at least 10%% faster than the second the test passes.", 1);
    }

    if (selector == 28)
    {
        printf("\nUse:\n\n"
               "28 NoiseSensitivityWorks \n\n"
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

        FormatedPrint("The Test compresses two files the first with Noise Sensitivity equal to 0 the second with Noise Sensitivity equal to 6.  If the first file has a higher PSNR than the second file the test passes.", 1);
    }

    if (selector == 29)
    {
        printf("\nUse:\n\n"
               "29 OnePassVsTwoPass \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n "
               "	 <Optional Settings File>\n"
              );

        FormatedPrint("The Test compresses two files the first using Second Pass Good Quality Mode the second using Best Quality Mode.  The test then records and evaluates the first and second files PSNR numbers and Data Rates.  If Second Pass Good Quality Mode Produces a lower bit rate and or a Higher PSNR the test Passes", 1);
    }

    if (selector == 30)
    {
        printf("\nUse:\n\n"
               "30  PlayAlternate \n\n"
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

        FormatedPrint("The Test creates two compressions the first with Play Alternate equal to 0 the second with Play Alternate equal to 1.  The Test then compares the resulting compressions if they are not equal the test passes.", 1);
    }

    if (selector == 31)
    {
        printf("\nUse:\n\n"
               "31 PostProcessorWorks \n\n"
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

        FormatedPrint("The Test creates a compression then creates a No Filtering decompression, decompressions for Deblock and Noise levels ranging from 0 to 15.  So long as all Deblock and Noise decompressions return a different PSNR than the No Filtering Decompression but are with in 10%% the test passes.", 1);
    }

    if (selector == 32)
    {
        printf("\nUse:\n\n"
               "32 PreProcessorWorks \n\n"
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

        FormatedPrint("The Test creates seven compressions using noise sensitivity values from 0 to 6  and records their PSNRs.  If none of the compressions PSNRs are equal the test passes.", 1);
    }

    if (selector == 33)
    {
        printf("\nUse:\n\n"
               "33 ResampleDownWaterMark \n\n"
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

        FormatedPrint("The Test creates two files the first with resample-down-watermark set to 90 the second with  resample-down-watermark set to 10. If the PSNR of the first compression is less than the PSNR of the second the test passes.", 1);
    }

    if (selector == 34)
    {
        printf(
            "34  SpeedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n "
            "	 <Optional Settings File>\n");

        FormatedPrint("The Test works for RealTime Mode and Good Quality Mode.  For Real Time Mode the test creates compresssions for CpuUsed Values from - 1 to -16 and 0 to 16.  If compression speed increases as CpuUsed increases and all PSNRs are within 10%% of the previous the Test Passes.  For Good Quality Mode the test creates compresssions for CpuUsed Values from 0 to 5.  If compression speed increases as CpuUsed increases and all PSNRs are within 10%% of the previous the Test Passes. ", 1);
    }

    if (selector == 35)
    {
        printf(
            "  TestVectorCheck \n\n"
            "    <Input Directory>\n"
            "\n"
        );

        FormatedPrint("This Test Decodes each VP8 Test Vector and Checks its SHA1 Checksum against the expected value.  If all Test Vectors decode properly the test passes.", 1);

    }

    if (selector == 36)
    {
        printf("\nUse:\n\n"
               "36 TwoPassVsTwoPassBest \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n "
               "	 <Optional Settings File>\n"
              );

        FormatedPrint("The Test creates two files the first using Two Pass Mode the Second Two Pass Best Mode.  The test then records and evaluates the first and second files PSNR numbers and Data Rates.  If Second Pass Best Quality Mode Produces a lower bit rate and or a Higher PSNR the test Passes", 1);


    }

    if (selector == 37)
    {
        printf("\nUse:\n\n"
               "37 UnderShoot \n\n"
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

        FormatedPrint("The Test creates two files the first with an undershoot equal to 10 the second with an undershoot equal to 100. If the second file's Bit Rate is greater than the first's the Test passes.", 1);
    }

    if (selector == 38)
    {
        printf("\nUse:\n\n"
               "38  Version \n\n"
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

        FormatedPrint("The Test creates four compressions the first with Version equal to 0 the second with version equal to 1 the third with version equal to 2 the fourth with version equal to 3.  The Test then Decodes each and records the time it took to do so.  If each successive version takes less time than the prior to decode and has a Lower PSNR the test passes.", 1);
    }

    if (selector == 39)
    {
        printf("\nUse:\n\n"
               "39  WindowsMatchesLinux \n\n"
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
               "    <Input Folder - enter N/A if none>\n "
               "    <Optional Settings File>\n"
              );

        FormatedPrint("The Test Can be run in two test modes.  The First Mode, 0 creates platform specific compressions and decompressions to be tested on another platform.  The second Mode creates platform specific compressions and decompressions and then compares them to previously encoded and decoded files created by test mode 0.  If the files are identical the test passes.", 1);
    }

    //if(selector == 20)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"20 IDCTDecOnlyWorks \n\n"
    //	"    <inputfile>\n"
    //	"    <Mode>\n"
    //	"          (0)Realtime/Live Encoding\n"
    //	"          (1)Good Quality Fast Encoding\n"
    //	"          (2)One Pass Best Quality\n"
    //	"          (3)Two Pass - First Pass\n"
    //	"          (4)Two Pass\n"
    //	"          (5)Two Pass Best Quality\n"
    //	"    <Target Bit Rate>\n "
    //	"	 <Optional Settings File>\n"
    //	);
    //
    //	FormatedPrint("The Test creates a compression then Creates two decompressions one with On2_fastIDCTDisabled = 0 the other 1 if the two decompressions are identical and their times to create them differ the test passes. ",1);
    //}
    //if(selector == 21)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"21 InterlaceWorks \n\n"
    //	"    This Test is not a valid VP8 Test.\n");

    //	FormatedPrint("This Test is not a valid VP8 Test.",1);
//
    //}
    //if(selector == 22)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"22 KeyInKeyOutWorks \n\n"
    //	"    This Test is not a valid VP8 Test.\n");

    //	FormatedPrint("This Test is not a valid VP8 Test.",1);
    //}
    //if(selector == 35)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"35 RandComp\n\n"
    //	"    <Inputfile>\n"
    //	);
    //
    //	FormatedPrint("Randomizes VP8 Parameters",1);
    //}
    //if(selector == 36)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"36 SpeedTest \n\n"
    //	"    This Test is not a valid VP8 Test.\n");

    //	FormatedPrint("This Test is not a valid VP8 Test.",1);
    //}
    //if(selector == 33)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"33 GraphPSNR\n\n"
    //	"    <Inputfile 1>\n"
    //	"    <Inputfile 2>\n"
    //	"    <UV Swap off:0 on:1>\n"
    //	"    <Frame Stats off:0 on:1>\n"
    //	"\n");

    //	FormatedPrint("The Test generates and creates compressions from user input starting and ending bit rates and steps.  The Test then computes and outputs the data rates and PSNRs in multiple formats.",1);
    //}
    //if(selector == 28)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"28 NewVsOldSpeed \n\n"
    //	"    This Test is not a valid VP8 Test.\n");

    //	FormatedPrint("This Test is not a valid VP8 Test.",1);
    //}
    //if(selector == 13)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"13 DataRateMaxWorks \n\n"
    //	"    This Test is not a valid VP8 Test.\n");

    //	FormatedPrint("This Test is not a valid VP8 Test. - Removed 03 25 2009",1);
    //}
    //if(selector == 9) //Removed-06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	" 9 ChangeIDCTDecWorks \n\n"
    //	"    <inputfile>\n"
    //	"    <Mode>\n"
    //	"          (0)Realtime/Live Encoding\n"
    //	"          (1)Good Quality Fast Encoding\n"
    //	"          (2)One Pass Best Quality\n"
    //	"          (3)Two Pass - First Pass\n"
    //	"          (4)Two Pass\n"
    //	"          (5)Two Pass Best Quality\n"
    //	"    <Target Bit Rate>\n "
    //	"	 <Optional Settings File>\n"
    //	);

    //	FormatedPrint("The Test creates a Compression and then two decompressions one with On2_fastIDCTDisabled = 0 the other = 1.  If the decompressions are identical and the times to complete them are not the test passes.",1);

    //}
    //if(selector == 10) //Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"10 ChangeIDCTDecWorks \n\n"
    //	"    <inputfile>\n"
    //	"    <Mode>\n"
    //	"          (0)Realtime/Live Encoding\n"
    //	"          (1)Good Quality Fast Encoding\n"
    //	"          (2)One Pass Best Quality\n"
    //	"          (3)Two Pass - First Pass\n"
    //	"          (4)Two Pass\n"
    //	"          (5)Two Pass Best Quality\n"
    //	"    <Target Bit Rate>\n "
    //	"	 <Optional Settings File>\n"
    //	);

    //	FormatedPrint("The Test creates two compressions one with On2_fastIDCTDisabled = 0 the other 1.  If the compressions are identical but the times to create them are not the test passes.",1);
    //}
    //if(selector == 11)//Removed 06-07-2010
    //{
    //	printf( "\nUse:\n\n"
    //	"11 ComboCheckRun \n\n"
    //	"    <Settings Inputfile>\n"
    //	"    <Primary Avi Inputfile>\n"
    //	);

    //	FormatedPrint("Performs a user specified number of tests driven by a text file.  The User may define files and custom settings to run compressions and tests on.  Tests run include PSNR CheckPBM and DataRate.  To create a template driver text file for this test, use the command: CreateSampleTextFiles",1);

    //}
    return;
}
int  ToolHelp(string InputString)//return 1 if string found return 0 if string not found if string not found TestHelp will be run through.
{
    if (InputString.compare("IVF2IVFCompr") == 0)
    {
        printf(
            "\n  Compress IVF to IVF \n\n"
            "    <(1)Normal Compress |(2)TimeCompress>\n"
            "    <inputfile>\n"
            "    <outputfile>\n"
            "    <Bit Rate>\n"
            "    <Mode>\n"
            "    <Optional - Parameter File>\n"
        );

        FormatedPrint("This utility will take in a raw ivf file and produce an encoded ivf file using the given mode and bitrate.  Default encode settings can be overridden by specifying a parameter file.", 2);
        return 1;
    }

    if (InputString.compare("IVF2IVFDec") == 0)
    {
        printf(
            "\n  Decompress IVF to IVF \n\n"
            "    <inputfile>\n"
            "    <outputfile>\n"
        );

        FormatedPrint("This utility will take in an encoded ivf file and output a decoded ivf file.", 2);
        return 1;
    }

    if (InputString.compare("IVF2RawDec") == 0)
    {
        printf(
            "\n  Decompress IVF to Raw \n\n"
            "    <inputfile>\n"
            "    <outputfile>\n"
        );

        FormatedPrint("This utility will take in an encoded ivf file and output a decoded raw file.", 2);
        return 1;
    }

    if (InputString.compare("IVFDataRate") == 0)
    {
        printf(
            "\n  IVF DataRate \n\n"
            "    <inputfile>\n"
        );

        FormatedPrint("This utility will take in an ivf file and compute its average, min, max, and file data rates. ", 2);
        return 1;
    }

    if (InputString.compare("IVFPSNR") == 0)
    {
        printf(
            "\n  IVFPSNR\n\n"
            "     <Raw IVF File>\n"
            "     <Comp IVF File>\n"
        );

        FormatedPrint("This utility will compute an encoded files psnr using the encoded file's ivf source file.", 2);
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

        FormatedPrint("This utility will run CheckPBM to make sure a buffer under run wont occur.", 2);
        return 1;
    }

    if (InputString.compare("Raw2IVF") == 0)
    {
        printf(
            "\n  Raw2IVF\n\n"
            "    <inputfile>\n"
            "    <OutPutDir>\n"
            "    <Width>\n"
            "    <Height>\n"
            "    <FrameRate>\n"
            "    <FourCC>\n"
        );

        FormatedPrint("This utility will take in a raw file and produce a raw ivf file.", 2);
        return 1;
    }

    if (InputString.compare("IVF2Raw") == 0)
    {
        printf("\n"
               "  IVF2Raw \n\n"
               "    <Input File>\n"
               "    <Output File>\n"
              );

        FormatedPrint("This utility will take in an ivf file and produce a raw file.", 2);
        return 1;
    }

    if (InputString.compare("IVF2RawFrames") == 0)
    {
        printf(
            "\n  IVF2RawFrames\n\n"
            "    <inputfile>\n"
            "    <OutPutDir>\n"
        );

        FormatedPrint("This utility will take in an ivf file and produce individual raw frames for each frame that exists in the user specified directory.", 2);
        return 1;
    }

    if (InputString.compare("CombineIndvFrames") == 0)
    {
        printf("\n  CombineIndvFrames");
        FormatedPrint("This utility will combine all individual decoded frames in a folder into a single raw file in numerical order.", 2);
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

    if (InputString.compare("CompIVFHeader") == 0)
    {
        printf("\n"
               "  CompareIVFHeaderInfo \n\n"
               "    <inputfile1>\n"
               "    <inputfile2>\n"
               "    <Full File Info 1/yes-0/no>\n"
               "    <OutputFile>\n"
              );

        FormatedPrint("This utility will compare the file and frame headers of two ivf files.", 2);
        return 1;
    }

    if (InputString.compare("DispIVFHeader") == 0)
    {
        printf("\n"
               "  DisplayIVFHeaderInfo \n\n"
               "    <Inputfile>\n"
               "    <Full File Info 1/yes-0/no>\n"
               "    <Optional Outputfile>\n"
              );

        FormatedPrint("This utility will display the file and frame headers of an ivf file.", 2);
        return 1;
    }

    if (InputString.compare("DispKeyFrames") == 0)
    {
        printf("\n"
               "  DispKeyFrames \n\n"
               "    <Input IVF File>\n"
              );

        FormatedPrint("This utility will display the location of key frames within an ivf file.", 2);
        return 1;
    }

    if (InputString.compare("CompareIVF") == 0)
    {
        printf(
            "\n  CompareIVF\n\n"
            "     <inputfile1>\n"
            "     <inputfile2>\n"
        );

        FormatedPrint("This utility will compare the video content of two ivf files and will display if they are identical, or if they differ the first frame they differ at.", 2);
        return 1;
    }

    if (InputString.compare("CutIVF") == 0)
    {
        printf(
            "\n  CutIVF\n\n"
            "    <inputfile>\n"
            "    <outputfile>\n"
            "    <Starting Frame>\n"
            "    <Ending Frame>\n"
        );

        FormatedPrint("This utility will cut a portion of an ivf file starting at Starting Frame and ending at Ending Frame to a new output file", 2);
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

        FormatedPrint("This utility will paste the contents of Inputfile2 into Inputfile1 starting at Inputfile1's First Paste Frame to a new Outputfile.", 2);
        return 1;
    }

    if (InputString.compare("PlayDecIVF") == 0)
    {
#if defined(_WIN32)
        printf(
            "\n  PlayDecIVF\n\n"
            "    <inputfile>\n"
        );
        FormatedPrint("This Tool will convert an uncompressed ivf file to a raw yuv file and play it using tmnplay.  Please be sure to put a copy of tmnplay in your C:\\bin directory.  Tmnplay can be found in the TestClips directory.", 2);
#else
        printf("\n  PlayDecIVF");
        FormatedPrint("This Tool will convert an uncompressed ivf file to a raw yuv file and play it using mplayer.  To install mplayer open a command console and type:", 2);
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
            "    <inputfile>\n"
        );
        FormatedPrint("This Tool will convert a compressed ivf file to a raw yuv file and play it using tmnplay.  Please be sure to put a copy of tmnplay in your C:\\bin directory.  Tmnplay can be found in the TestClips directory.", 2);
#else
        printf("\n  PlayCompIVF");
        FormatedPrint("This Tool will convert a compressed ivf file to a raw yuv file and play it using mplayer.  To install mplayer open a command console and type:", 2);
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
        FormatedPrint("This utility will create sample text files.", 2);
        return 1;
    }

    if (InputString.compare("PrintVersion") == 0)
    {
        FormatedPrint("This utility will print the version of vp8 being used by the tester.", 2);
        return 1;
    }

    if (InputString.compare("RandParFile") == 0)
    {
        printf(
            "\n  RandomParFile\n\n"
            "    <Output Par File>\n"
        );

        FormatedPrint("This utility will create a valid vp8 random parameter file.", 2);
        return 1;
    }

    if (InputString.compare("RandIVFComp") == 0)
    {
        printf("\n"
               "  RandComp\n\n"
               "    <Inputfile>\n"
              );

        FormatedPrint("This utility will create a compression using random parameters for an input ivf file.", 2);
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

        FormatedPrint("The utility creates compressions from user input starting to user input ending bit rates at user input steps.  The utility then computes and outputs the data rates and PSNRs of the resultant files.", 2);
    }

    if (InputString.compare("TestVectorIndex") == 0)
    {
        printf("\n  TestVectorIndex\n");
        FormatedPrint("This Utility makes use of the TestVectorsIndividualRunsResults Directory to locate the first report of a line being covered by a test vector and reports relevant information about it.", 2);
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

    return 0;
}
void FormatSummaryByTest(char *InputFileNameCharAr, int DeleteOldFile)
{
    cout << "\n Formating Summary file.\n";
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

    while (TestTracker < 48)
    {
        if (TestTracker == 1)
            TestTrackerName = "AllowDropFrames";

        if (TestTracker == 2)
            TestTrackerName = "AllowSpatialResampling";

        if (TestTracker == 3)
            TestTrackerName = "AllowSpatialResamplingTest2";

        if (TestTracker == 4)
            TestTrackerName = "AutoKeyFramingWorks";

        if (TestTracker == 5)
            TestTrackerName = "BufferLevelWorks";

        if (TestTracker == 6)
            TestTrackerName = "CPUDecOnlyWorks";

        if (TestTracker == 7)
            TestTrackerName = "ChangeCPUDec";

        if (TestTracker == 8)
            TestTrackerName = "ChangeCPUWorks";

        if (TestTracker == 9)
            TestTrackerName = "ChangeIDCTDecWorks";

        if (TestTracker == 10)
            TestTrackerName = "ChangeIDCTWorks";

        if (TestTracker == 11)
            TestTrackerName = "ComboTestRun";

        if (TestTracker == 12)
            TestTrackerName = "DropFramesWaterMarkWorks";

        if (TestTracker == 13)
            TestTrackerName = "DataRateMaxWorks";

        if (TestTracker == 14)
            TestTrackerName = "DataRateTest";

        if (TestTracker == 15)
            TestTrackerName = "DebugMatchesRelease";

        if (TestTracker == 16)
            TestTrackerName = "ErrorResilientModeWorks";

        if (TestTracker == 17)
            TestTrackerName = "ExtraFileCheck";

        if (TestTracker == 18)
            TestTrackerName = "ForceKeyFrameWorks";

        if (TestTracker == 19)
            TestTrackerName = "GoodQualityVsBestQuality";

        if (TestTracker == 20)
            TestTrackerName = "IDCTDecOnlyWorks";

        if (TestTracker == 21)
            TestTrackerName = "InterlaceWorks";

        if (TestTracker == 22)
            TestTrackerName = "KeyInKeyOutWorks";

        if (TestTracker == 23)
            TestTrackerName = "MaxQuantizerTest";

        if (TestTracker == 24)
            TestTrackerName = "MemLeakCheck";

        if (TestTracker == 25)
            TestTrackerName = "MinQuantizerTest";

        if (TestTracker == 26)
            TestTrackerName = "NewVsOldPSNR";

        if (TestTracker == 27)
            TestTrackerName = "NewVsOldRealTimeSpeed";

        if (TestTracker == 28)
            TestTrackerName = "NewVsOldSpeed";

        if (TestTracker == 29)
            TestTrackerName = "NoiseSensitivityWorks";

        if (TestTracker == 30)
            TestTrackerName = "OnePassVsTwoPass";

        if (TestTracker == 31)
            TestTrackerName = "PostProcessorWorks";

        if (TestTracker == 32)
            TestTrackerName = "PreProcessorWorks";

        if (TestTracker == 33)
            TestTrackerName = "GraphPSNR";

        if (TestTracker == 34)
            TestTrackerName = "ResampleDownWaterMark";

        if (TestTracker == 35)
            TestTrackerName = "RandComp";

        if (TestTracker == 36)
            TestTrackerName = "SpeedTest";

        if (TestTracker == 37)
            TestTrackerName = "TwoPassVsTwoPassBest";

        if (TestTracker == 38)
            TestTrackerName = "UnderShoot";

        if (TestTracker == 39)
            TestTrackerName = "VP61vVP62";

        if (TestTracker == 40)
            TestTrackerName = "VP6MvVP60";

        if (TestTracker == 41)
            TestTrackerName = "AllowLagTest";

        if (TestTracker == 42)
            TestTrackerName = "AltFreqTest";

        if (TestTracker == 43)
            TestTrackerName = "FixedQ";

        if (TestTracker == 44)
            TestTrackerName = "LagInFramesTest";

        if (TestTracker == 45)
            TestTrackerName = "MultiThreadedTest";

        if (TestTracker == 46)
            TestTrackerName = "PlayAlternate";

        if (TestTracker == 47)
            TestTrackerName = "Version";


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

            while (n < 35 && buffer[n] != ' ')
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
void FormatSummaryByTestandResult(char *InputFileNameCharAr, int DeleteOldFile)
{
    cout << "\n Formating Summary file.\n";

    string InputFileName = InputFileNameCharAr;
    string SummaryByTestOutput = InputFileName;
    SummaryByTestOutput.erase(SummaryByTestOutput.end() - 4, SummaryByTestOutput.end());

    SummaryByTestOutput.append("_Expanded.txt");

    char TestsRunChar[255];
    FolderName(InputFileNameCharAr, TestsRunChar);
    string TestsRun = TestsRunChar;
    TestsRun.append("TestsRun.txt");



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


    while (TestTracker < 53)
    {
        ResultTracker = 0;

        int DoOnce1 = 1;

        //This iterates through all possible tests run using the test tracker names to specify which was run the number is only to drive
        //The iteration through and does not represent test number
        if (TestTracker == 1)
            TestTrackerName = "AllowDropFrames";

        if (TestTracker == 2)
            TestTrackerName = "AllowSpatialResampling";

        if (TestTracker == 3)
            TestTrackerName = "AllowSpatialResamplingTest2";

        if (TestTracker == 4)
            TestTrackerName = "AutoKeyFramingWorks";

        if (TestTracker == 5)
            TestTrackerName = "BufferLevelWorks";

        if (TestTracker == 6)
            TestTrackerName = "CPUDecOnlyWorks";

        if (TestTracker == 7)
            TestTrackerName = "ChangeCPUDec";

        if (TestTracker == 8)
            TestTrackerName = "ChangeCPUWorks";

        if (TestTracker == 9)
            TestTrackerName = "ChangeIDCTDecWorks";

        if (TestTracker == 10)
            TestTrackerName = "ChangeIDCTWorks";

        if (TestTracker == 11)
            TestTrackerName = "ComboTestRun";

        if (TestTracker == 12)
            TestTrackerName = "DropFramesWaterMarkWorks";

        if (TestTracker == 13)
            TestTrackerName = "DataRateMaxWorks";

        if (TestTracker == 14)
            TestTrackerName = "DataRateTest";

        if (TestTracker == 15)
            TestTrackerName = "DebugMatchesRelease";

        if (TestTracker == 16)
            TestTrackerName = "ErrorResilientModeWorks";

        if (TestTracker == 17)
            TestTrackerName = "ExtraFileCheck";

        if (TestTracker == 18)
            TestTrackerName = "ForceKeyFrameWorks";

        if (TestTracker == 19)
            TestTrackerName = "GoodQualityVsBestQuality";

        if (TestTracker == 20)
            TestTrackerName = "IDCTDecOnlyWorks";

        if (TestTracker == 21)
            TestTrackerName = "InterlaceWorks";

        if (TestTracker == 22)
            TestTrackerName = "KeyInKeyOutWorks";

        if (TestTracker == 23)
            TestTrackerName = "MaxQuantizerTest";

        if (TestTracker == 24)
            TestTrackerName = "MemLeakCheck";

        if (TestTracker == 25)
            TestTrackerName = "MinQuantizerTest";

        if (TestTracker == 26)
            TestTrackerName = "NewVsOldPSNR";

        if (TestTracker == 27)
            TestTrackerName = "NewVsOldRealTimeSpeed";

        if (TestTracker == 28)
            TestTrackerName = "NewVsOldSpeed";

        if (TestTracker == 29)
            TestTrackerName = "NoiseSensitivityWorks";

        if (TestTracker == 30)
            TestTrackerName = "OnePassVsTwoPass";

        if (TestTracker == 31)
            TestTrackerName = "PostProcessorWorks";

        if (TestTracker == 32)
            TestTrackerName = "PreProcessorWorks";

        if (TestTracker == 33)
            TestTrackerName = "GraphPSNR";

        if (TestTracker == 34)
            TestTrackerName = "ResampleDownWaterMark";

        if (TestTracker == 35)
            TestTrackerName = "RandComp";

        if (TestTracker == 36)
            TestTrackerName = "SpeedTest";

        if (TestTracker == 37)
            TestTrackerName = "TwoPassVsTwoPassBest";

        if (TestTracker == 38)
            TestTrackerName = "UnderShoot";

        if (TestTracker == 39)
            TestTrackerName = "VP61vVP62";

        if (TestTracker == 40)
            TestTrackerName = "VP6MvVP60";

        if (TestTracker == 41)
            TestTrackerName = "AllowLagTest";

        if (TestTracker == 42)
            TestTrackerName = "AltFreqTest";

        if (TestTracker == 43)
            TestTrackerName = "FixedQ";

        if (TestTracker == 44)
            TestTrackerName = "LagInFramesTest";

        if (TestTracker == 45)
            TestTrackerName = "MultiThreadedTest";

        if (TestTracker == 46)
            TestTrackerName = "PlayAlternate";

        if (TestTracker == 47)
            TestTrackerName = "SpeedTest";

        if (TestTracker == 48)
            TestTrackerName = "Version";

        if (TestTracker == 49)
            TestTrackerName = "WindowsMatchesLinux";

        if (TestTracker == 50)
            TestTrackerName = "EncoderBreakOut";

        if (TestTracker == 51)
            TestTrackerName = "MemLeakCheck2";

        if (TestTracker == 52)
            TestTrackerName = "TestVectorCheck";


        int	PassedTrack = 0;
        int FailedTrack = 0;
        int IndeterminateTrack = 0;
        int SeeComboRunLogTrack = 0;
        int SeePSNRLogTrack = 0;
        int RandomTestCompletedTrack = 0;
        int CompressionMadeTrack = 0;
        int ErrorFileMismatchTrack = 0;
        int TestNotSupportedTrack = 0;
        int MinTestPassed = 0;

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

                while (n < 35 && buffer[n] != ' ')
                {
                    buffer2[i] = buffer[n];
                    n++;
                    i++;
                }

                n = 63;
                i = 0;

                while (n < 190 && buffer[n] != ' ')
                {
                    buffer3[i] = buffer[n];
                    n++;
                    i++;
                }

                string TestReadString = buffer2;
                string TestResultString = buffer3;

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
                        cerr << "\n\n ====================================================================" << buffer2 << "==================================================================== \n";
                        DoOnce1 = 0;
                    }

                    if (DoOnce2)
                    {
                        cerr << "\n " << buffer3 << ":\n\n";
                        fprintf(stderr, "%4s %-30s%-28s%s  Test Settings\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                        DoOnce2 = 0;
                    }

                    cerr << buffer << " ";

                    ///////////////////Get the correct Test Input settings and output them to summary.//////////////
                    fstream TestsRunFile;
                    TestsRunFile.open(TestsRun.c_str(), fstream::in);

                    if (!TestsRunFile.good())
                    {
                        TestsRunFile.close();
                        FormatSummaryByTestFile.close();
                        fclose(fp);
                        printf("\nFile creation Canceled: Cannot find TestsRun.txt\n");
                        fprintf(stderr, "\nFile creation Canceled: Cannot find TestsRun.txt\n");
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

                            cerr << buffer4 << "\n";

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
        {
            PrintOutput = 1;
        }

        if (SeePSNRLogTrack != 0)
        {
            PrintOutput = 1;
        }

        if (RandomTestCompletedTrack != 0)
        {
            PrintOutput = 1;
        }

        if (CompressionMadeTrack != 0)
        {
            CompressExist = 1;
            PrintOutput = 1;
        }

        if (ErrorFileMismatchTrack != 0)
        {
            PrintOutput = 1;
        }

        if (TestNotSupportedTrack != 0)
        {
            PrintOutput = 1;
        }

        if (MinTestPassed != 0)
        {
            PrintOutput = 1;
        }

        if (PrintOutput == 1)
        {
            if (!CompressExist == 1)
            {
                cerr << "\n\n\n" << TestTrackerName.c_str() << " Summary:\n\n";

                cerr << "Passed: " << PassedTrack << "\n";
                cerr << "Failed: " << FailedTrack << "\n";
                cerr << "Indeterminate: " << IndeterminateTrack << "\n";

                if (SeeComboRunLogTrack != 0)
                {
                    cerr << "SeeComboRunLogTrack: " << SeeComboRunLogTrack << "\n";
                }

                if (SeePSNRLogTrack != 0)
                {
                    cerr << "SeePSNRLogTrack: " << SeePSNRLogTrack << "\n";
                }

                if (RandomTestCompletedTrack != 0)
                {
                    cerr << "RandomTestCompletedTrack: " << RandomTestCompletedTrack << "\n";
                }

                if (CompressionMadeTrack != 0)
                {
                    cerr << "CompressionMadeTrack: " << CompressionMadeTrack << "\n";
                }

                if (ErrorFileMismatchTrack != 0)
                {
                    cerr << "ErrorFileMismatchTrack: " << ErrorFileMismatchTrack << "\n";
                }

                if (TestNotSupportedTrack != 0)
                {
                    cerr << "TestNotSupportedTrack: " << TestNotSupportedTrack << "\n";
                }

                if (MinTestPassed != 0)
                {
                    cerr << "MinTestPassed: " << MinTestPassed << "\n";
                }
            }
            else
            {
                cerr << "\n\n\n" << TestTrackerName.c_str() << " Summary:\n\n";

                if (PassedTrack != 0)
                {
                    cerr << "Passed: " << PassedTrack << "\n";

                }

                if (FailedTrack != 0)
                {
                    cerr << "Failed: " << FailedTrack << "\n";
                }

                if (IndeterminateTrack != 0)
                {
                    cerr << "Indeterminate: " << IndeterminateTrack << "\n";
                }

                if (SeeComboRunLogTrack != 0)
                {
                    cerr << "SeeComboRunLogTrack: " << SeeComboRunLogTrack << "\n";
                }

                if (SeePSNRLogTrack != 0)
                {
                    cerr << "SeePSNRLogTrack: " << SeePSNRLogTrack << "\n";
                }

                if (RandomTestCompletedTrack != 0)
                {
                    cerr << "RandomTestCompletedTrack: " << RandomTestCompletedTrack << "\n";
                }

                if (CompressionMadeTrack != 0)
                {
                    cerr << "CompressionMadeTrack: " << CompressionMadeTrack << "\n";
                }

                if (ErrorFileMismatchTrack != 0)
                {
                    cerr << "ErrorFileMismatchTrack: " << ErrorFileMismatchTrack << "\n";
                }

                if (TestNotSupportedTrack != 0)
                {
                    cerr << "TestNotSupportedTrack: " << TestNotSupportedTrack << "\n";
                }

                if (MinTestPassed != 0)
                {
                    cerr << "MinTestPassed: " << MinTestPassed << "\n";
                }
            }

            cerr << "\n\n";
        }

        TestTracker++;
    }

    fclose(fp);
    return;

}
void ShowHiddenCmds()
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
        OnErrorOutPut();
        return 0;
    }

#ifdef DSHOW
    snprintf(TesterExePath, 255, "%s", argv[0]);
    //strcpy(TesterExePath, argv[0]);
#endif

    string EmptyAr[1];
    char WorkingDirChar[256]  = "";
    CreateWorkingFolder(argc, argv, WorkingDirChar);
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
            printf("\nError: Folder Path Length too long.\n");
            fprintf(stderr, "\nError: Folder Path Length too long.\n");
            return 0;
        }
    }

    /////////////////////////////////////////////////////////////Help//////////////////////////////////////////////////////////

    if (TestInputString1.compare("Help") == 0 || TestInputString1.compare("HELP") == 0  || TestInputString1.compare("help") == 0)
    {
        OnErrorOutPut();
        FormatedPrint("To run tests input the number to the left of the test name along with test specific input.  To obtain more information about a specific test or utility input the number of the test or the utility name you wish to inquire about followed by \"help\".", 2);
        return 0;
    }

    if (TestInputString2.compare("Help") == 0 || TestInputString2.compare("HELP") == 0  || TestInputString2.compare("help") == 0)
    {
        int ToolHelpCheck = ToolHelp(TestInputString1);

        if (!ToolHelpCheck)
        {
            TestHelp(argc, argv, WorkingDir);
        }

        return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////Tools//////////////////////////////////////////////////////////
    ////////////////////Public Commands/////////////////////////
    if (TestInputString.compare("IVF2IVFCompr") == 0)					//Compresses an IVF Raw File to an IVF Compressed file
    {
        ComprIVF2IVF(argc, argv, WorkingDir);
        return 0;
    }

    if (TestInputString.compare("IVF2IVFDec") == 0)						//Decompresses and IVF Compressed file to an IVF Raw File
    {
        DecIVF2IVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFDataRate") == 0)						//Computes Data Rate for an input IVF File
    {
        IVFDataRateTool(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFPSNR") == 0)							//Computes PSNR for two input files
    {
        IVFPSNRrun(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFCheckPBM") == 0)						//Computes PBM For an input file
    {
        IVFCheckPBMrun(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompIVFHeader") == 0)					//Compares the Headers of two ivf files
    {
        CompareIVFHeaderInfo(argc, argv);
        return 0;
    }

    if (TestInputString.compare("DispIVFHeader") == 0)					//Displays the header contents of an ivf file
    {
        DisplayIVFHeaderInfo(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompareIVF") == 0)						//Compares two ivf files
    {
        compareIVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CreateSampleTextFiles") == 0)			//Creates sample text files that include quick test and other templates
    {
        PrintTxtFiles(WorkingDir);
        return 0;
    }

    if (TestInputString.compare("CutIVF") == 0)							//Modifies an ivf files size
    {
        CutIVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("PasteIVF") == 0)							//Modifies an ivf files size
    {
        PasteIVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("DispKeyFrames") == 0)					//Displays which frames are key frames for an input compressed ivf file
    {
        if (argc < 3)
        {
            printf("\n"
                   "  DispKeyFrames \n\n"
                   "    <Input IVF File>\n"
                   "\n");
            return 0;
        }

        IVFDisplayKeyFrames(argv[2], 0, "");
        return 0;
    }

    if (TestInputString.compare("FormatSummaryByTest") == 0)				//Formats a Test Results output file by test
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
        FormatSummaryByTest(argv[2], argv3int);*/


        FormatSummaryByTest(argv[2], atoi(argv[3]));
        return 0;
    }

    if (TestInputString.compare("FormatSummaryByTestandResult") == 0)	//Formats a test results output file by test and result
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

        FormatSummaryByTestandResult(argv[2], atoi(argv[3]));
    }

    if (TestInputString.compare("IVF2Raw") == 0)							//Converts an ivf file to a raw file
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

    if (TestInputString.compare("PlayCompIVF") == 0)						//Plays a compressed ivf file (Decodes the file to an ivf then converts that ivf to a raw then used tnmplay.exe to play that file.)
    {
        PlayCompIVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("PlayDecIVF") == 0)						// Plays a Decoded ivf file (Converts the file to a Raw file and uses tmnplay.exe to play the file)
    {
        PlayDecIVF(argc, argv);
        return 0;
    }

    ////////////////////Hidden Commands/////////////////////////
    if (TestInputString.compare("ShowHidden") == 0)						//Shows Hidden Commands
    {
        ShowHiddenCmds();
        return 0;
    }

    if (TestInputString.compare("APICOMPRESS") == 0)						//API Compresion Clone
    {
        APICOMPRESS(argc, argv);
        return 0;
    }

    if (TestInputString.compare("APIDECOMPRESS") == 0)					//API Decompression Clone
    {
        APIDECOMPRESS(argc, argv);
        return 0;
    }

    //if(TestInputString.compare("VP8Compress") == 0)//IVFEnc Clone
    //{
    //	if(VP8CompressIVF(argc, argv) ==1)
    //	{
    //		//FormatedPrint("IVFEnc Clone",3);
    //	}
    //}
    //if(TestInputString.compare("VP8Decompress") == 0)//IVFDec Clone
    //{
    //	if(argc < 3)
    //	{
    //		cout<<"Usage: ivfdec file1.ivf fil2.yuv"<<endl;
    //		cout<<endl;
    //		//FormatedPrint("IVFDec Clone",3);
    //		return 0;
    //	}

    //	VP8Decompress(argv[2], argv[3]);
    //}
    //if(TestInputString.compare("VP8CompressIVF") == 0)//Disabled
    //{
    //	if (argc < 5)
    //	{
    //		cout << "\nUsage: VP8CompressIVF Mode(0-5) input output\n";
    //		return 0;
    //	}

    //	VP8CompressIVF2(atoi(argv[2]), argv[3], argv[4]);
    //}
    //if(TestInputString.compare("VP8DecompressIVF") == 0)//Diabled
    //{
    //	if(argc < 3)
    //	{
    //		cout<<"Usage: ivfdec file1.ivf fil2.yuv"<<endl;
    //		cout<<endl;
    //		return 0;
    //	}

    //	VP8DecompressIVFtoIVF(argv[2], argv[3]);
    //}
    if (TestInputString.compare("CodeCoverage") == 0)					//Preforms Code Coverage Tests for data coverage if enabled
    {
        CodeCoverage(argc, argv, WorkingDir, EmptyAr);
        return 0;
    }

    if (TestInputString.compare("CompareCodeCoverage") == 0)				//Compares New Code Coverage Results to Old Code Coverage Results and merges the files to an updated version
    {
        CompareCodeCoverage(argc, argv);
        return 0;
    }

    if (TestInputString.compare("FormatCodeCoverageFile") == 0)			//Formats Cygwin Code Coverage files to the same format as regular linux
    {
        FormatCodeCoverageFile(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFDECTestVectorCheck") == 0)			//
    {
        IVFDECTestVectorCheck(argc, argv);
        return 0;
    }

    if (TestInputString.compare("TestVectorIndex") == 0)					//Will search a data base and return the Test Vector that covers an input File and Line Number
    {
        TestVectorIndex(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompMatchesIVFenc") == 0)				//Makes sure that the Tester compressor matches ivfenc compressor
    {
        CompMatchesIVFenc(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompressionEquiv") == 0)				//makes sure that all variations of the compressor produce the same output
    {
        CompressionEquiv(argc, argv, WorkingDir);
        return 0;
    }

    if (TestInputString.compare("PrintVersion") == 0)					//Prints the internal version number - make sure all libs built in same multithreadedmode - properties  C++ CodeGen RuntimeLibrary
    {
        PrintVersion();
        return 0;
    }

    if (TestInputString.compare("Test0InputTextCheck") == 0)				//Checks to make sure a Multi Run Input file has the correct format
    {
        Test0InputTextCheck(argv[2], 1);
        return 0;
    }

    if (TestInputString.compare("ConvertParFileToIVFEnc") == 0)			//converts Tester Parameter file to a IVFenc ParFile - obsolete
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

    if (TestInputString.compare("CopyAllTxtFiles") == 0)					//Copies all text files in a directory to a new directory preserving file structure
    {
        CopyAllTxtFiles(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVF2RawDec") == 0)						//Decodes IVF File to Raw File
    {
        DecIVF2Raw(argc, argv);
        return 0;
    }

    if (TestInputString.compare("DeleteAllIVFFiles") == 0)				//Deletes all IVF files in a directory be very care with this function!
    {
        DeleteAllIVFFiles(argc, argv);
        return 0;
    }

    if (TestInputString.compare("Raw2IVF") == 0)							//Converts an IVF File to a Raw File
    {
        Raw2IVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("SideBySideText") == 0)					//Prints out the input from two text files side by side as coloums to an output txt file
    {
        SideBySideText(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVF2RawFrames") == 0)					//Converts an ivf file to a raw file
    {
        if (argc < 3)
        {
            printf(
                "\n  IVF2RawFrames\n\n"
                "    <inputfile>\n"
                "    <OutPutDir>\n"
            );
            return 0;
        }

        argv[4] = "1";
        WriteIndividualFramesOut(argc, argv);
        return 0;
    }

    if (TestInputString.compare("ArrayCovFailListToFull") == 0)			//
    {
        ArrayCovFailListToFullList(argc, argv);
        return 0;
    }

    if (TestInputString.compare("ArrayCovSummaryFile") == 0)				//
    {
        ArrayCovSummaryFile(argc, argv);
        return 0;
    }

    if (TestInputString.compare("WinMemMonFormat") == 0)				//
    {
        WinMemMonFormat(argc, argv);
        return 0;
    }

    if (TestInputString.compare("WinMemMonGraph") == 0)				//
    {
        WinMemMonGraph(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CombineIndvFrames") == 0)				//
    {
        CombineIndvFrames(argc, argv);
        return 0;
    }

    if (TestInputString.compare("GraphPSNR") == 0)				//
    {
        GraphPSNR(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (TestInputString.compare("RandIVFComp") == 0)				//
    {
        RandComp(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (TestInputString.compare("RandParFile") == 0)				//
    {
        CreateRandParFile(argc, argv);
        return 0;
    }


    printf("\n");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int selector = atoi(argv[1]);

    if (selector > 100 || selector < 0)
    {

        OnErrorOutPut();
        return 0;
    }

    if (TestInputString.compare("0") == 0)
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

        int VaildInput = 0;

        if (atoi(argv[2]) != 4 && atoi(argv[2]) != 3)
        {
            VaildInput = Test0InputTextCheck(argv[3], 0);

            if (VaildInput <= 0)
            {
                printf("\nExternal Test Runner Aborted\n\n");
                return 0;
            }
        }

        ExternalTestRunner(argc, argv, WorkingDir.c_str(), VaildInput);

        string SummaryFile = WorkingDir;

        if (atoi(argv[2]) == 1)
        {
            char SummaryFileChar[255] = "";
            SummaryFile.erase(SummaryFile.end() - 1);
            SummaryFile.append(slashCharStr);
            SummaryFile.append("Mode1Results.txt");
            snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
            FormatSummaryByTestandResult(SummaryFileChar, 0);
            return 0;
        }

        if (atoi(argv[2]) == 3)
        {

            char SummaryFileChar[255] = "";
            SummaryFile.erase(SummaryFile.end() - 1);
            SummaryFile.append(slashCharStr);
            SummaryFile.append("Mode2Results.txt");
            snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
            FormatSummaryByTestandResult(SummaryFileChar, 0);
            return 0;
        }

        if (atoi(argv[2]) == 4)
        {
            string SummCompAndTest = "";
            string SummComp = "";
            string SummTest = "";

            SummCompAndTest = argv[3];
            SummCompAndTest.append(slashCharStr);
            SummCompAndTest.append("Mode1Results.txt");			// Mode 1
            SummComp = argv[3];
            SummComp.append(slashCharStr);
            SummComp.append("Mode2Results.txt");					// Mode 2
            SummTest = argv[3];
            SummTest.append(slashCharStr);
            SummTest.append("Mode3Results.txt");							// Mode 3

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
                SummaryFile.append(slashCharStr);
                SummaryFile.append("Mode1Results.txt");
                snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
                FormatSummaryByTestandResult(SummaryFileChar, 0);
                return 0;
            }

            if (TestRunning == 3)
            {

                char SummaryFileChar[255] = "";
                SummaryFile.append(slashCharStr);
                SummaryFile.append("Mode3Results.txt");
                snprintf(SummaryFileChar, 255, "%s", SummaryFile.c_str());
                FormatSummaryByTestandResult(SummaryFileChar, 0);
            }
        }

        return 0;
    }

    if (selector == 1)
    {
        AllowDF(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 2)
    {
        AllowLagTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 3)
    {
        AllowSpatialResamplingTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 4)
    {
        AllowSpatialResamplingTest2(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 5)
    {
        AltFreqTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 6)
    {
        AutoKeyFramingWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 7)
    {
        BufferLevelWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 8)
    {
        CPUDecOnlyWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 9)
    {
        ChangeCPUDec(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 10)
    {
        ChangeCPUWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 11)
    {
        DFWM(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 12)
    {
        DataRateTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 13)
    {
        DebugMatchesRelease(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 14)
    {
        EncoderBreakOutTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 15)
    {
        ErrorRes(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 16)
    {
        ExtraFileCheck(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 17)
    {
        FixedQ(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 18)
    {
        ForceKeyFrameWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 19)
    {
        GoodQvBestQ(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 20)
    {
        LagInFramesTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 21)
    {
        MaxQTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 22)
    {
        MemLeakCheck(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 23)
    {
        MemLeakCheck2(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 24)
    {
        MinQTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 25)
    {
        MultiThreadedTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 26)
    {
        NewVsOldPSNR(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 27)
    {
        NewVsOldRealTimeSpeed(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 28)
    {
        NoiseSensitivityWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 29)
    {
        OnePassVsTwoPass(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 30)
    {
        PlayAlternate(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 31)
    {
        PostProcessorWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 32)
    {
        PreProcessorWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 33)
    {
        ResampleDownWaterMark(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 34)
    {
        SpeedTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 35)
    {
        TestVectorCheck(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 36)
    {
        TwoPassVsTwoPassBest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 37)
    {
        UnderShoot(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 38)
    {
        Version(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == 39)
    {
        WindowsMatchesLinux(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }


////////////Temp DELETE Removed Tests///////////////
//	if(selector == 9)
//	{
//		ChangeIDCTDecWorks(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 10)
//	{
//		ChangeIDCTWorks(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 11)
//	{
//		ComboCheckRun(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 13)
//	{
//		DataRateMaxWorks(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 20)
//	{
//		IDCTDecOnlyWorks(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 21)
//	{
//		InterlaceWorks(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 22)
//	{
//		KeyInKeyOutWorks(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 28)
//	{
//		NewVsOldSpeed(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//if(selector == 33)
//	{
//		GraphPSNR(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//	if(selector == 35)
//	{
//		RandComp(argc, argv, WorkingDir, EmptyAr, 1);
//	}
//////////////////////////////////////////////

    OnErrorOutPut();
    return 0;

}
