#define _CRT_SECURE_NO_WARNINGS
#include "test-definitions.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

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
///////////////////////////////////////////////////////////////////////////////////////

//This is temporary need better solution
#ifdef DSHOW
char TesterExePath[256];
#endif

//Utilities
extern void FolderName(const char *input, char *output);
extern int Test0InputTextCheck(char *input, int MoreInfo);
extern int IVFCheckPBMThreshold(const char *inputFile, double bitRate, int maxBuffer, int preBuffer, int optimalbuffer, int Threshold);
extern int DisplayIVFHeaderInfo(int argc, char *argv[]);
extern int CompareIVFHeaderInfo(int argc, char *argv[]);
extern double IVFDisplayDropedFrames(char *inputchar, int PrintSwitch);
extern double IVFDisplayResizedFrames(const char *inputchar, int PrintSwitch);
extern double IVFDisplayVisibleFrames(const char *inputchar, int PrintSwitch);
extern double IVFDisplayAltRefFrames(const char *inputFile, int Selector);
extern double IVFDisplayKeyFrames(const char *inputFile, int Selector);

//Tools
extern int ComprIVF2IVF(int argc, char *argv[], string WorkingDir);
extern int DecIVF2IVF(int argc, char *argv[]);
extern int DecIVF2Raw(int argc, char *argv[]);
extern int IVFDataRateTool(int argc, char *argv[]);
extern int IVFPSNRrun(int argc, char *argv[]);
extern int IVFCheckPBMrun(int argc, char *argv[]);
extern int compareIVF(int argc, char *argv[]);
extern int WriteIndividualFramesOut(int argc, char *argv[]);
extern int IVF2Raw(char *inputFile, char *outputDir);
extern int Raw2IVF(int argc, char *argv[]);
extern int CutIVFTool(int argc, char *argv[]);
extern int CropRawIVFTool(int argc, char *argv[]);
extern int PasteIVF(int argc, char *argv[]);
extern int CombineIndvFrames(int argc, char *argv[]);
extern int PlayCompIVF(int argc, char *argv[]);
extern int PlayDecIVF(int argc, char *argv[]);
extern int DeleteAllIVFFiles(int argc, char *argv[]);
extern int CopyAllTxtFiles(int argc, char *argv[]);
extern int SideBySideText(int argc, char *argv[]);
extern int CompressionEquiv(int argc, char *argv[], string WorkingDir);
extern int ConvertParmFileToIVFenc(char *input, char *output);
extern int CompMatchesIVFenc(int argc, char *argv[]);
extern int CompareCodeCoverage(int argc, char *argv[]);
extern int FormatCodeCoverageFile(int argc, char *argv[]);
extern int TestVectorIndex(int argc, char *argv[]);
extern int APICOMPRESS(int argc, char *argv[]);
extern int APIDECOMPRESS(int argc, char *argv[]);
extern int IVFDECTestVectorCheck(int argc, char *argv[]);
extern int ArrayCovFailListToFullList(int argc, char *argv[]);
extern int ArrayCovSummaryFile(int argc, char *argv[]);
extern int WinMemMonFormat(int argc, char *argv[]);
extern int WinMemMonGraph(int argc, char *argv[]);
extern int CreateRandParFile(int argc, char *argv[]);
extern int RunIVFDec(int argc, char *argv[]);
extern int RunIVFEnc(int argc, char *argv[]);
extern int SolveQuad();

/////Tests
extern int PrintVersion();
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
extern int WinLinMacMatch(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);

//CodeCoverage
extern int CodeCoverage(int argc, char *argv[], string WorkingDir, string FilesAr[]);

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
           "   (0)RunTestsFromFile                     IVFEnc\n"
           "   (1)AllowDropFrames                      IVFDec\n"
           "   (2)AllowLagTest                         IVF2IVFCompr\n"
           "   (3)AllowSpatialResampling               IVF2IVFDec\n"
           "   (4)AutoKeyFramingWorks                  IVF2RawDec\n"
           "   (5)BufferLevelWorks                     \n"
           "   (6)CPUDecOnlyWorks                      IVFDataRate\n"
           "   (7)ChangeCPUWorks                       IVFPSNR\n"
           "   (8)DropFramesWaterMarkWorks             IVFCheckPBM\n"
           "   (9)DataRateTest                         \n"
           "  (10)DebugMatchesRelease                  Raw2IVF\n"
           "  (11)EncoderBreakOut                      IVF2Raw\n"
           "  (12)ErrorResilientModeWorks              IVF2RawFrames\n"
           "  (13)ExtraFileCheck                       CombineIndvFrames\n"
           "  (14)FixedQ                               \n"
           "  (15)ForceKeyFrameWorks                   CompareIVF\n"
           "  (16)FrameSizeTest                        CompIVFHeader\n"
           "  (17)GoodQualityVsBestQuality             DispIVFHeader\n"
           "  (18)LagInFramesTest                      \n"
           "  (19)MaxQuantizerTest                     DispKeyFrames\n"
           "  (20)MemLeakCheck                         DispResizedFrames\n"
           "  (21)MemLeakCheck2                        DispVisibleFrames\n"
           "  (22)MinQuantizerTest                     DispAltRefFrames\n"
           "  (23)MultiThreadedTest                    \n"
           "  (24)NewVsOldPSNR                         CropRawIVF\n"
           "  (25)NewVsOldRealTimeSpeed                CutIVF\n"
           "  (26)NoiseSensitivityWorks                PasteIVF\n"
           "  (27)OnePassVsTwoPass                     \n"
           "  (28)PlayAlternate                        PlayDecIVF\n"
           "  (29)PostProcessorWorks                   PlayCompIVF\n"
           "  (30)ReconBuffer                          \n"
           "  (31)ResampleDownWaterMark                CreateSampleTextFiles\n"
           "  (32)SpeedTest                            PrintVersion\n"
           "  (33)TestVectorCheck                      \n"
           "  (34)TwoPassVsTwoPassBest                 RandParFile\n"
           "  (35)UnderShoot                           RandIVFComp\n"
           "  (36)Version                              GraphPSNR\n"
           "  (37)WinLinMacMatch                       Help\n"
           "                                           \n"
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
            "49  WinLinMacMatch \n\n"
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
            "50  EncoderBreakOut \n\n"
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
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf\n", EXTFINUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128\n", GQVBQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@..%cTestClips%cMemLeakCheck2_Compression.ivf\n", MEML2NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", NVORTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128\n", OV2PSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cTestVectors\n", TVECTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128\n", TV2BTNUM, slashChar, slashChar);
    fprintf(fp5, "%%%i@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA\n", WMLMMNUM, slashChar, slashChar);
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2\n", MULTTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2\n", MULTTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@10\n", SPEEDNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@10\n", SPEEDNUM, slashChar, slashChar);
    fprintf(fp5, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode0%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@6\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode1%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128\n", AlWDFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@1@..%cTestClips%cSpecialCaseParameterFiles%c1ChangeCPUWorksCPUUsage4.txt\n", CHGWRNUM, slashChar, slashChar, slashChar, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode2%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode4%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", ALWLGNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@10@24\n", LGIFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", PLYALNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode5%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", ALWLGNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe@VP8vNewest_PlugIn_RLib_RMode_32Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@10@24\n", LGIFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_32Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_32Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", PLYALNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    printf("\n\nQuick Test file created:\n%s\n\n", TextfileString5.c_str());
    fclose(fp5);
}
void Print6(string WorkingDir)
{
    string TextfileString5 = WorkingDir;
    TextfileString5.erase(TextfileString5.end() - 25, TextfileString5.end());
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
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf\n", EXTFINUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128\n", GQVBQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@..%cTestClips%cMemLeakCheck2_Compression.ivf\n", MEML2NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", NVORTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128\n", OV2PSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cTestVectors\n", TVECTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@128\n", TV2BTNUM, slashChar, slashChar);
    fprintf(fp5, "%%%i@..%cTestClips%csrc16.ivf@1@128@0@NA@NA@NA\n", WMLMMNUM, slashChar, slashChar);
    fprintf(fp5, "%%%%%%%%%%%%%%Tests That Run Twice Per input File%%%%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@0@128@2\n", MULTTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@1@128@2\n", MULTTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@10\n", SPEEDNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@10\n", SPEEDNUM, slashChar, slashChar);
    fprintf(fp5, "%%%%%%%%%%%%Tests That For Multiple Modes Per input File%%%%%%%%%%%%%%%%\n");
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode0%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@0@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@6\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@0@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode1%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_1280x720_1500F.ivf@1@128\n", AlWDFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@1@..%cTestClips%cSpecialCaseParameterFiles%c1ChangeCPUWorksCPUUsage4.txt\n", CHGWRNUM, slashChar, slashChar, slashChar, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_1280x720_1500F.ivf@1@2048\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@1@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@1@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode2%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@2@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@2@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode4%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", ALWLGNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@4@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@10@24\n", LGIFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", PLYALNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@4@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "\n");
    fprintf(fp5, "%%%%Mode5%%%%\n");
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", ALWLGNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", ALWSRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@6\n", AUTKFNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", BUFLVNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@0\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@1\n", CPUDENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@0\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@1\n", CHGWRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", DFWMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", DTARTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe@VP8vNewest_PlugIn_RLib_RMode_64Bit.exe\n", DBMRLNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%cBBB_720x480_2000F.ivf@5@128\n", ENCBONUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", ERRMWNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@20@60\n", FIXDQNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@24\n", FKEFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@176@144\n", FRSZTNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@10@24\n", LGIFRNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", MAXQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@VP8vNewest_PlugIn_DLib_DMode_64Bit.exe\n", MEML1NUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", MINQUNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128@VP8vOldest_PlugIn_RLib_RMode_64Bit.exe@2\n", NVOPSNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", NOISENUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", PLYALNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", POSTPNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", RECONBUF, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", RSDWMNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", UNDSHNUM, slashChar, slashChar);
    fprintf(fp5, "%i@..%cTestClips%csrc16.ivf@5@128\n", VERSINUM, slashChar, slashChar);
    fprintf(fp5, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");



    printf("\n\nQuick Test file created:\n%s\n\n", TextfileString5.c_str());
    fclose(fp5);
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
    //selector == 5 -> Individual Pass Fail output

    string SummaryStrOutput;
    int EndOfLineLength = 0;

    if (selector == 1 || selector == 2 || selector == 3) //add padding for formating
    {
        SummaryStrOutput.append("         ");
    }

    if (selector == 5) //add padding for formating
    {
        SummaryStrOutput.append(" * ");
    }

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

    if (selector == 5)
    {
        Cutoff = 70;
    }

    int x = 0;

    while (x + Cutoff < SummaryStr.length())
    {


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

        if (selector == 5) //add padding for formating
        {
            SummaryStrOutput.append("\n   ");
        }

        x = x + Cutoff;

        while (SummaryStr.substr(x, 1).compare(" ") == 0)
        {
            x++;
        }

        if (selector == 1 || selector == 2 || selector == 3)
        {
            Cutoff = 66;
        }

        if (selector == 4)
        {
            Cutoff = 79;
        }

        if (selector == 5)
        {
            Cutoff = 70;
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

    if (selector == 5)
    {
        printf("%s", SummaryStrOutput.c_str());
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

        FormatedPrint("The test runs tests from an input file and summarizes the results. The test can be run in four modes: Mode 1 - Create compressions and run tests, Mode 2 - Create compressions only, Mode 3 - Run tests on preexisting compressions, and Mode 4 - Resume tests in progress. To create a template driver text file use the command: CreateSampleTextFiles.", 1);
    }

    if (selector == AlWDFNUM)
    {
        printf("\nUse:\n\n"
               "%2i AllowDF \n\n"
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
               , AlWDFNUM);

        FormatedPrint("The test creates two files; the first with Drop Frames on, the second with Drop Frames off. It then records and compares the number of frames each file has.  If Drop Frames on has fewer frames than Drop Frames off; the test passes.", 1);

    }

    if (selector == ALWLGNUM)
    {
        printf("\nUse:\n\n"
               "%2i AllowLag \n\n"
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
               , ALWLGNUM);

        FormatedPrint("The test creates two compressions; the first with Allow Lag equal to 0, the second with Allow Lag equal to 1.  If the correct number of lagged frames are detected via quantizer output, alternate reference frames exist for Allow Lag on, Allow Lag on has the same number of visible frames as Allow Lag off, and Allow Lag on is not identical to Allow Lag off; the test passes.  (Test Pass/Fail only valid for modes 4 and 5.)", 1);
    }

    if (selector == ALWSRNUM)
    {
        printf("\nUse:\n\n"
               "%2i AllowSpatialResampling \n\n"
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
               , ALWSRNUM);

        FormatedPrint("The test creates two files the first with Spatial Resampling off the second with Spatial Resampling on. The test then records the number of resized frames for each and computes the PSNR for Spatial Resampling on.  If the number of resized frames for Spatial Resampling on is greater than 0, the number of resized frames for Spatial Resampling off equals 0, and the PSNR calculated for Spatial Resampling on is greater than 15; the test passes.", 1);
    }

    if (selector == AUTKFNUM)
    {
        printf("\nUse:\n\n"
               "%2i AutoKeyFramingWorks \n\n"
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
               , AUTKFNUM);

        FormatedPrint("The test creates two files with identical parameters setting Auto Key Frame equal to 6. The test then records the placement of each files key frames.  If both files key frames occur in identical locations and at least as frequently as Auto Key Frame dictates; the test passes.", 1);

    }

    if (selector == BUFLVNUM)
    {
        printf("\nUse:\n\n"
               "%2i BufferLevelWorks \n\n"
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
               , BUFLVNUM);

        FormatedPrint("The test creates a compression and runs CheckPBM on it.  If no buffer under run is detected; the test passes.", 1);

    }

    if (selector == CPUDENUM)
    {
        printf("\nUse:\n\n"
               "%2i CPUDecOnlyWorks \n\n"
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
               , CPUDENUM);

        FormatedPrint("The test creates a compression of the user input version (0-3) and then decompresses it for ON2_SIMD_CAPS values ranging from 0 to 11. The test then compares them against one another.  If all compressions are identical and the times to decompress them are not; the test passes.", 1);

    }

    if (selector == CHGWRNUM)
    {
        printf("\nUse:\n\n"
               "%2i ChangeCPUWorks \n\n"
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
               , CHGWRNUM);

        FormatedPrint("The test creates compressions of the user input version (0-3) with ON2_SIMD_CAPS values 0 1 3 7 and 15.  If all compressions are identical and compression times are not the test passes.", 1);

    }

    if (selector == DFWMWNUM)
    {
        printf("\nUse:\n\n"
               "%2i DFWMTest \n\n"
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
               , DFWMWNUM);

        FormatedPrint("The test creates 6 compressions with DFWM values of 100 80 60 40 20 and 0 and records their sizes.  If each successively lower DFWM compression has a equal or larger size than the previous the test passes.", 1);

    }

    if (selector == DTARTNUM)
    {
        printf("\nUse:\n\n"
               "%2i DataRateTest \n\n"
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
               , DTARTNUM);

        FormatedPrint("The test creates a compression and records it data rate.  If the compressions data rate is within 30% of the input target bandwidth; the test passes.", 1);

    }

    if (selector == DBMRLNUM)
    {
        printf("\nUse:\n\n"
               "%2i DebugMatchesRelease \n\n"
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
               , DBMRLNUM);

        FormatedPrint("The test creates two compressions the first using an executable built using the newest release library, the second using an executable built using the newest debug library.  If the two compressions are identical; the test passes.", 1);

    }

    if (selector == ENCBONUM)
    {
        printf("\nUse:\n\n"
               "%2i EncoderBreakOut \n\n"
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
               , ENCBONUM);

        FormatedPrint("The test creates four compressions. The first with an EncodeBreakout of 0, the second with an EncodeBreakout of 100, the thrid with an EncodeBreakout of 500 and the fourth with an EncodeBreakout of 1000. Decompressions of the encoded files are then carried out and PSNR values are calculated.  If the decompressions run successfully and the PSNR values of each successive EncodeBreakout trial are with in 2 dB the test passes. If the PSNRs are greater than 2 dB but less than 5 dB the test is inconclusive and if the PSNRs have greater than a 5 dB difference the test fails.", 1);
    }

    if (selector == ERRMWNUM)
    {
        printf("\nUse:\n\n"
               "%2i ErrorRes \n\n"
               "    <inputfile>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "          (2)One Pass Best Quality\n"
               "          (3)Two Pass - First Pass\n"
               "          (4)Two Pass\n"
               "          (5)Two Pass Best Quality\n"
               "    <Target Bit Rate>\n "
               , ERRMWNUM);

        FormatedPrint("The test creates two compressions the first with Error Resilient Mode off the second on. The test then records their PSNR values.  If their PSNRs are with in 10% of one another the test passes.", 1);

    }

    if (selector == EXTFINUM)
    {
        printf("\nUse:\n\n"
               "%2i ExtraFileCheck \n\n"
               "    <Inputfile>\n"
               "	 <Optional Settings File>\n"
               , EXTFINUM);

        FormatedPrint("The test creates a two pass compression and checks the current directory, the directory the executable is located in and the directory the output file is written to for extra files.  If no extra files are found the test passes.", 1);
    }

    if (selector == FIXDQNUM)
    {
        printf("\nUse:\n\n"
               "%2i FixedQ \n\n"
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
               , FIXDQNUM);

        FormatedPrint("The test creates two compressions each with user input Fixed Quantizers and records the quantizers used to encode each frame. The test then records the compressions sizes.  If all quantizers for each compression match the input Fixed Quantizer and the smaller quantizers compression has a larger file size; the test passes.", 1);
    }

    if (selector == FKEFRNUM)
    {
        printf("\nUse:\n\n"
               "%2i ForceKeyFrameWorks \n\n"
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
               , FKEFRNUM);

        FormatedPrint("The test creates a compression using a user input value for a Force Key Frame Interval. The compressor forces a key frame for every Force Key Frame Invervalith frame. The Test then records the placement of all key frames in the compression.  If key frames occur only when Force Key Frame dictates; the test passes.", 1);

    }

    if (selector == FRSZTNUM)
    {
        printf("\nUse:\n\n"
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
               "     <Optional Settings File>\n"
              );
        FormatedPrint("The test creates fourty-six cropped raw ivf files from a user input file, starting height and starting width (where starting height and starting width must be multiples of 16).  The test then creates compressions for each raw file and computes its psnr.  If all compressions psnrs within five percent of the starting width height files psnr and all psnrs are greater than 25 the test passes.", 1);
    }

    if (selector == GQVBQNUM)
    {
        printf("\nUse:\n\n"
               "%2i GoodQvBestQ \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n"
               "	 <Optional Settings File>\n"
               , GQVBQNUM);

        FormatedPrint("The test creates six compressions. The first and fourth compressions for 30% less than the input target bandwidth at good quality and best quality, the second and fifth compressions for the input target bandwidth at good quality and best quality, and the thrid and sixth at 30% more than the input target bandwidth at good quality and best quality. The test then records each files data rate and PSNR and computes the area under the curve for the common interval between the good quality curve and best quality curve.  If the area under the best quality curve is greater than the area under the good quality curve; the test passes.", 1);
    }

    if (selector == LGIFRNUM)
    {
        printf("\nUse:\n\n"
               "%2i LagInFrames \n\n"
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
               , LGIFRNUM);

        FormatedPrint("The test creates three compressions one with Allow Lag set to 0 the second and third with Allow Lag set to 1. The second compression uses the first user input Lag in Frames value for its Lag in frames and the third uses the second user input value for its Lag in Frames. The test outputs each files quantizer values for each encoded frame.  If none of the files are identical, the PSNRs of each successive file are within 10% of the last and the quantizer output shows that the proper number of frames were lagged; the test passes.  (Test Pass/Fail only valid for modes 4 and 5.)", 1);
    }

    if (selector == MAXQUNUM)
    {
        printf("\nUse:\n\n"
               "%2i MaxQTest \n\n"
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
               , MAXQUNUM);

        FormatedPrint("The test creates nine files the first with a WorstAllowedQ equal to 3 and each subsequent file with a WorstAllowedQ eight greater than the last until 63. The test records the individual quantizer values for each encoded frame.  If the PSNRs of each WorstAllowedQ compression from 3 to 63 increase as Worst AllowedQ decreases and the recorded quantizers for each file do not exceed their corresponding WorstAllowedQ for all compressions; the test passes.", 1);

    }

    if (selector == MEML1NUM)
    {
        printf("\nUse:\n\n"
               "%2i MemLeakCheck \n\n"
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
               , MEML1NUM);

        FormatedPrint("The Test creates a compression using the debug executable to check memory usage and records the results to an output file.  If no memory leaks are found the test passes.", 1);
    }

    if (selector == MEML2NUM)
    {
        printf("\nUse:\n\n"
               "%2i MemLeakCheck2 \n\n"
               "    <Mem Leak Check Exe>\n"
               "\n"
               , MEML2NUM);

        FormatedPrint("The test uses the debug executable to open and close 10,000 instances of the encoder and open and close 10,000 instance the decoder and then checks to make sure there are no memory leaks.  If there are no leaks the test passes.", 1);
    }

    if (selector == MINQUNUM)
    {
        printf("\nUse:\n\n"
               "%2i MinQTest \n\n"
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
               , MINQUNUM);

        FormatedPrint("The test creates two files the first with a MinQ equal to 10 the second with a MinQ equal to 60 and records the quantizer used for each compressions frames.  If the first file has a higher PSNR than the second file and every quantizer for both files is above the corresponding MinQ; the test passes.", 1);
    }

    if (selector == MULTTNUM)
    {
        printf("\nUse:\n\n"
               "%2i MultiThreadedTest \n\n"
               "    <inputfile>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "    <Target Bit Rate>\n "
               "    <Number of Cores to Use>\n"
               "     <Optional Settings File>\n"
               , MULTTNUM);

        FormatedPrint("The test creates two compressions the first using a MultiThreaded equal to 2 the second using a MultiThreaded equal to 0. The test then compares the times to compress each.  If MultiThreaded 2 is faster than 0; the test passes.", 1);
    }

    if (selector == NVOPSNUM)
    {
        printf("\nUse:\n\n"
               "%2i NewVsOldPSRN \n\n"
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
               , NVOPSNUM);

        FormatedPrint("The test creates two compressions the first using the newest version of VP8 and the second using a separate executable built using an older version. It then computes and records PSNR values for each.  If new PSNR is greater than olds PSNR or is at least within 1% of the old; the test passes.", 1);
    }

    if (selector == NVORTNUM)
    {
        printf("\nUse:\n\n"
               "%2i NewVsOldRealTimeSpeed \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n"
               "    <Exe File To Compare>\n"
               "	 <Optional Settings File>\n"
               , NVORTNUM);

        FormatedPrint("The test creates two compressions the first using the newest version of VP8 and the second using a separate executable built using an older version. The test records the time that each compression took.  If the new compressions time is at least 10% faster than the old compressions time; the test passes.", 1);
    }

    if (selector == NOISENUM)
    {
        printf("\nUse:\n\n"
               "%2i NoiseSensitivityWorks \n\n"
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
               , NOISENUM);

        FormatedPrint("The test compresses seven files with Noise Sensitivity values from 0 to 6 and computes PSNR values for each.  If all compressions have differing PSNR values and Noise Sensitivity 0 has a higher PSNR than Noise Sensitivity 6; the test passes.", 1);
    }

    if (selector == OV2PSNUM)
    {
        printf("\nUse:\n\n"
               "%2i OnePassVsTwoPass \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n "
               "	 <Optional Settings File>\n"
               , OV2PSNUM);

        FormatedPrint("The test creates six compressions. The first and fourth compressions for 30% less than the input target bandwidth at one pass good quality and two pass good quality, the second and fifth compressions for the input target bandwidth at one pass good quality and two pass good quality, and the thrid and sixth at 30% more than the input target bandwidth at one pass good quality and two pass good quality. The test then records each files data rate and PSNR and computes the area under the curve for the common interval between the one pass good quality curve and the two pass good quality curve.  If the area under the two pass good quality curve is greater than the area under the one pass good quality curve; the test passes.", 1);
    }

    if (selector == PLYALNUM)
    {
        printf("\nUse:\n\n"
               "%2i PlayAlternate \n\n"
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
               , PLYALNUM);

        FormatedPrint("The test creates two compressions the first with Play Alternate equal to 0 the second with Play Alternate equal to 1. The test then records the placement of alternate reference frames and visible frames for both compressions.  If alternate reference frames exist for Play Alternate = 1 and not for Play Alternate = 0, visible frames for Play Alternate 1 and Play Alternate 2 are equal, and the files are not identical; the test passes.  (Test Pass/Fail only valid for modes 4 and 5.)", 1);
    }

    if (selector == POSTPNUM)
    {
        printf("\nUse:\n\n"
               "%2i PostProcessorWorks \n\n"
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
               , POSTPNUM);

        FormatedPrint("The test creates a compression then creates a No Filtering decompression, decompressions for Deblock and Noise levels ranging from 0 to 15.  If all Deblock and Noise decompressions return a different PSNR than the No Filtering Decompression but are within 10%; the test passes.", 1);
    }

    if (selector == RECONBUF)
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
            , RECONBUF);


        FormatedPrint("The test creates a compression and internally compares the compressor's preview frames to the decoded output produced by decompressing the compressor�s en-coded frame. The state of each frame is recorded to a text file.  If the contents of all preview frames are identical to the content of all decoded frames; the test passes.", 1);
    }

    if (selector == RSDWMNUM)
    {
        printf("\nUse:\n\n"
               "%2i ResampleDownWaterMark \n\n"
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
               , RSDWMNUM);

        FormatedPrint("The test creates two files the first with resample-down-watermark set to 90 the second with resample-down-watermark set to 10. The test then records the frames at which the file buffer reaches the designated thresholds, the location of key frames and location of resized frames for both files.  If the first resized frame occurs on the first instance where the frame prior to a key frame reaches the correct buffer saturation for both compressions; the test passes.", 1);
    }

    if (selector == SPEEDNUM)
    {
        printf("\nUse:\n\n"
               "%2i SpeedTest \n\n"
               "    <inputfile>\n"
               "    <Mode>\n"
               "          (0)Realtime/Live Encoding\n"
               "          (1)Good Quality Fast Encoding\n"
               "    <Target Bit Rate>\n "
               "	 <Optional Settings File>\n"
               , SPEEDNUM);

        FormatedPrint("The test works for RealTime Mode and Good Quality Mode. For Real Time Mode the test creates compressions for CpuUsed Values from -1 to -16 and 0 to 16. For Good Quality Mode the test creates compressions for CpuUsed Values from 0 to 5.  If compression speed increases as CpuUsed increases and all PSNRs are within 10% of the previous; the test passes.", 1);
    }

    if (selector == TV2BTNUM)
    {
        printf("\nUse:\n\n"
               "%2i TestVectorCheck \n\n"
               "    <Input Directory>\n"
               "\n"
               , TV2BTNUM);

        FormatedPrint("This test decodes each VP8 Test Vector and Checks its MD5 checksum against the expected value.  If all Test Vectors decode properly and all MD5 checksums match their expected values; the test passes.", 1);

    }

    if (selector == TV2BTNUM)
    {
        printf("\nUse:\n\n"
               "%2i TwoPassVsTwoPassBest \n\n"
               "    <inputfile>\n"
               "    <Target Bit Rate>\n "
               "	 <Optional Settings File>\n"
               , TV2BTNUM);

        FormatedPrint("The test creates six compressions. The first and fourth compressions for 30% less than the input target bandwidth at two pass good quality and two pass best quality, the second and fifth compressions for the input target bandwidth at two pass good quality and two pass best quality, and the third and sixth at 30% more than the input target bandwidth at two pass good quality and two pass best quality. The test then records each files data rate and PSNR and computes the area under the curve for the common interval between the two pass good quality curve and the two pass best quality curve.  If the area under the two pass best quality curve is greater than the area under the two pass good quality curve; the test passes.", 1);


    }

    if (selector == UNDSHNUM)
    {
        printf("\nUse:\n\n"
               "%2i UnderShoot \n\n"
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
               , UNDSHNUM);

        FormatedPrint("The test creates two files the first with an undershoot equal to 10 the second with an undershoot equal to 100.  If the Undershoot 100 compressions file size is greater than the Undershoot 10 compressions file size; the test passes.", 1);
    }

    if (selector == VERSINUM)
    {
        printf("\nUse:\n\n"
               "%2i Version \n\n"
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
               , VERSINUM);

        FormatedPrint("The test creates four compressions the first with Version equal to 0 the second with version equal to 1 the third with version equal to 2 the fourth with version equal to 3. The test then decodes each and records the time it took to do so.  If each successive version takes less time than the prior to decode and has a lower PSNR; the test passes.", 1);
    }

    if (selector == WMLMMNUM)
    {
        printf("\nUse:\n\n"
               "%2i WinLinMacMatch \n\n"
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
               , WMLMMNUM);

        FormatedPrint("The test can be run in two test modes. The first Mode, 0 creates platform specific compressions and decompressions to be tested on another platform. The second Mode creates platform specific compressions and decompressions and then compares them to previously encoded and decoded files created by test mode 0.  If the files are identical the test passes.", 1);
    }

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
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        FormatedPrint("This utility will display the location of key frames within an ivf file to the screen or an output file.", 2);
        return 1;
    }

    if (InputString.compare("DispResizedFrames") == 0)
    {
        printf("\n"
               "  DisplayResizedFrames \n\n"
               "    <Input IVF File>\n"
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        FormatedPrint("This utility will display the location of resized frames within an ivf file to the screen or an output file.", 2);
        return 1;
    }

    if (InputString.compare("DispVisibleFrames") == 0)
    {
        printf("\n"
               "  DisplayVisibleFrames \n\n"
               "    <Input IVF File>\n"
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        FormatedPrint("This utility will display the location of visible frames within an ivf file to the screen or an output file.", 2);
        return 1;
    }

    if (InputString.compare("DispAltRefFrames") == 0)
    {
        printf("\n"
               "  DisplayAltRefFrames \n\n"
               "    <Input IVF File>\n"
               "    <Write to file 0 | 1 Print to screen>\n"
               "\n");

        FormatedPrint("This utility will display the location of alternate reference frames within an ivf file to the screen or an output file.", 2);
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

    if (InputString.compare("IVFEnc") == 0)
    {
        printf(
            "\n  IVFEnc\n"
            "\n"
        );

        FormatedPrint("This utility is a clone of the standard VP8 ivfenc utility.", 2);
        return 1;
    }

    if (InputString.compare("IVFDec") == 0)
    {
        printf(
            "\n  IVFDec\n"
            "\n"
        );

        FormatedPrint("This utility is a clone of the standard VP8 ivfdec utility.", 2);
        return 1;
    }

    if (InputString.compare("CropRawIVF") == 0)
    {
        printf(
            "\n  CropRawIVF\n\n"
            "    <inputfile>\n"
            "    <outputfile>\n"
            "    <xoffset>\n"
            "    <yoffset>\n"
            "    <New Frame Width>\n"
            "    <New Frame Height>\n"
            "    <Raw/IVF 0-Raw 1-IVF>\n"
        );

        FormatedPrint("This utility will take in a raw ivf file and produce a croped raw ivf file using the input size parameters.", 2);
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

    while (TestTracker < 40)
    {
        if (TestTracker == AlWDFNUM)
            TestTrackerName = "AllowDropFrames";

        if (TestTracker == ALWLGNUM)
            TestTrackerName = "AllowLagTest";

        if (TestTracker == ALWSRNUM)
            TestTrackerName = "AllowSpatialResampling";

        if (TestTracker == AUTKFNUM)
            TestTrackerName = "AutoKeyFramingWorks";

        if (TestTracker == BUFLVNUM)
            TestTrackerName = "BufferLevelWorks";

        if (TestTracker == CPUDENUM)
            TestTrackerName = "CPUDecOnlyWorks";

        if (TestTracker == CHGWRNUM)
            TestTrackerName = "ChangeCPUWorks";

        if (TestTracker == DFWMWNUM)
            TestTrackerName = "DropFramesWaterMarkWorks";

        if (TestTracker == DTARTNUM)
            TestTrackerName = "DataRateTest";

        if (TestTracker == DBMRLNUM)
            TestTrackerName = "DebugMatchesRelease";

        if (TestTracker == ENCBONUM)
            TestTrackerName = "EncoderBreakOut";

        if (TestTracker == ERRMWNUM)
            TestTrackerName = "ErrorResilientModeWorks";

        if (TestTracker == EXTFINUM)
            TestTrackerName = "ExtraFileCheck";

        if (TestTracker == FIXDQNUM)
            TestTrackerName = "FixedQ";

        if (TestTracker == FKEFRNUM)
            TestTrackerName = "ForceKeyFrameWorks";

        if (TestTracker == FRSZTNUM)
            TestTrackerName = "FrameSizeTest";

        if (TestTracker == GQVBQNUM)
            TestTrackerName = "GoodQualityVsBestQuality";

        if (TestTracker == LGIFRNUM)
            TestTrackerName = "LagInFramesTest";

        if (TestTracker == MAXQUNUM)
            TestTrackerName = "MaxQuantizerTest";

        if (TestTracker == MEML1NUM)
            TestTrackerName = "MemLeakCheck";

        if (TestTracker == MEML2NUM)
            TestTrackerName = "MemLeakCheck2";

        if (TestTracker == MINQUNUM)
            TestTrackerName = "MinQuantizerTest";

        if (TestTracker == MULTTNUM)
            TestTrackerName = "MultiThreadedTest";

        if (TestTracker == NVOPSNUM)
            TestTrackerName = "NewVsOldPSNR";

        if (TestTracker == NVORTNUM)
            TestTrackerName = "NewVsOldRealTimeSpeed";

        if (TestTracker == NOISENUM)
            TestTrackerName = "NoiseSensitivityWorks";

        if (TestTracker == OV2PSNUM)
            TestTrackerName = "OnePassVsTwoPass";

        if (TestTracker == PLYALNUM)
            TestTrackerName = "PlayAlternate";

        if (TestTracker == POSTPNUM)
            TestTrackerName = "PostProcessorWorks";

        if (TestTracker == RECONBUF)
            TestTrackerName = "ReconBuffer";

        if (TestTracker == RSDWMNUM)
            TestTrackerName = "ResampleDownWaterMark";

        if (TestTracker == SPEEDNUM)
            TestTrackerName = "SpeedTest";

        if (TestTracker == TVECTNUM)
            TestTrackerName = "TestVectorCheck";

        if (TestTracker == TV2BTNUM)
            TestTrackerName = "TwoPassVsTwoPassBest";

        if (TestTracker == UNDSHNUM)
            TestTrackerName = "UnderShoot";

        if (TestTracker == VERSINUM)
            TestTrackerName = "Version";

        if (TestTracker == WMLMMNUM)
            TestTrackerName = "WinLinMacMatch";


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


    while (TestTracker < 40)
    {
        ResultTracker = 0;

        int DoOnce1 = 1;

        //This iterates through all possible tests run using the test tracker names to specify which was run the number is only to drive
        //The iteration through and does not represent test number
        if (TestTracker == AlWDFNUM)
            TestTrackerName = "AllowDropFrames";

        if (TestTracker == ALWLGNUM)
            TestTrackerName = "AllowLagTest";

        if (TestTracker == ALWSRNUM)
            TestTrackerName = "AllowSpatialResampling";

        if (TestTracker == AUTKFNUM)
            TestTrackerName = "AutoKeyFramingWorks";

        if (TestTracker == BUFLVNUM)
            TestTrackerName = "BufferLevelWorks";

        if (TestTracker == CPUDENUM)
            TestTrackerName = "CPUDecOnlyWorks";

        if (TestTracker == CHGWRNUM)
            TestTrackerName = "ChangeCPUWorks";

        if (TestTracker == DFWMWNUM)
            TestTrackerName = "DropFramesWaterMarkWorks";

        if (TestTracker == DTARTNUM)
            TestTrackerName = "DataRateTest";

        if (TestTracker == DBMRLNUM)
            TestTrackerName = "DebugMatchesRelease";

        if (TestTracker == ENCBONUM)
            TestTrackerName = "EncoderBreakOut";

        if (TestTracker == ERRMWNUM)
            TestTrackerName = "ErrorResilientModeWorks";

        if (TestTracker == EXTFINUM)
            TestTrackerName = "ExtraFileCheck";

        if (TestTracker == FIXDQNUM)
            TestTrackerName = "FixedQ";

        if (TestTracker == FKEFRNUM)
            TestTrackerName = "ForceKeyFrameWorks";

        if (TestTracker == FRSZTNUM)
            TestTrackerName = "FrameSizeTest";

        if (TestTracker == GQVBQNUM)
            TestTrackerName = "GoodQualityVsBestQuality";

        if (TestTracker == LGIFRNUM)
            TestTrackerName = "LagInFramesTest";

        if (TestTracker == MAXQUNUM)
            TestTrackerName = "MaxQuantizerTest";

        if (TestTracker == MEML1NUM)
            TestTrackerName = "MemLeakCheck";

        if (TestTracker == MEML2NUM)
            TestTrackerName = "MemLeakCheck2";

        if (TestTracker == MINQUNUM)
            TestTrackerName = "MinQuantizerTest";

        if (TestTracker == MULTTNUM)
            TestTrackerName = "MultiThreadedTest";

        if (TestTracker == NVOPSNUM)
            TestTrackerName = "NewVsOldPSNR";

        if (TestTracker == NVORTNUM)
            TestTrackerName = "NewVsOldRealTimeSpeed";

        if (TestTracker == NOISENUM)
            TestTrackerName = "NoiseSensitivityWorks";

        if (TestTracker == OV2PSNUM)
            TestTrackerName = "OnePassVsTwoPass";

        if (TestTracker == PLYALNUM)
            TestTrackerName = "PlayAlternate";

        if (TestTracker == POSTPNUM)
            TestTrackerName = "PostProcessorWorks";

        if (TestTracker == RECONBUF)
            TestTrackerName = "ReconBuffer";

        if (TestTracker == RSDWMNUM)
            TestTrackerName = "ResampleDownWaterMark";

        if (TestTracker == SPEEDNUM)
            TestTrackerName = "SpeedTest";

        if (TestTracker == TVECTNUM)
            TestTrackerName = "TestVectorCheck";

        if (TestTracker == TV2BTNUM)
            TestTrackerName = "TwoPassVsTwoPassBest";

        if (TestTracker == UNDSHNUM)
            TestTrackerName = "UnderShoot";

        if (TestTracker == VERSINUM)
            TestTrackerName = "Version";

        if (TestTracker == WMLMMNUM)
            TestTrackerName = "WinLinMacMatch";


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
                        printf("\nFile creation Canceled: Cannot find %s\n", TestsRun.c_str());
                        fprintf(stderr, "\nFile creation Canceled: Cannot find %s\n", TestsRun.c_str());
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
    if (TestInputString.compare("TestPrint") == 0)                  //Compresses an IVF Raw File to an IVF Compressed file
    {
        cout << "\n";
        FormatedPrint("This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.", 1);
        cout << "\n";
        FormatedPrint("This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.", 2);
        cout << "\n";
        FormatedPrint("This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.", 3);
        cout << "\n";
        FormatedPrint("This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.", 4);
        cout << "\n";
        cout << "\n";
        FormatedPrint("This is some text i wanted to try to see how it would look if formated via the standard formatting that exists currently with the tester.  I wonder how it will look though i dont think it will server my purposes i think i can make a modified version that may.", 5);
        cout << "\n";
        cout << "\n";
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

        IVFCheckPBMThreshold(argv[2], atof(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

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
        CompareIVFHeaderInfo(argc, argv);
        return 0;
    }

    if (TestInputString.compare("DispIVFHeader") == 0)                  //Displays the header contents of an ivf file
    {
        DisplayIVFHeaderInfo(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CompareIVF") == 0)                     //Compares two ivf files
    {
        compareIVF(argc, argv);
        return 0;
    }

    if (TestInputString.compare("CreateSampleTextFiles") == 0)          //Creates sample text files that include quick test and other templates
    {
        PrintTxtFiles(WorkingDir);
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
        PasteIVF(argc, argv);
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

        int resizedframes = IVFDisplayResizedFrames(argv[2], atoi(argv[3]));
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

        int droppedframes = IVFDisplayDropedFrames(argv[2], atoi(argv[3]));
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

        int visframenum = IVFDisplayVisibleFrames(argv[2], atoi(argv[3]));
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

        int altrefframes = IVFDisplayAltRefFrames(argv[2], atoi(argv[3]));
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

        int keyframecount = IVFDisplayKeyFrames(argv[2], atoi(argv[3]));
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

        IVFDisplayAltRefFrames(argv[2], 0);
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
        FormatSummaryByTest(argv[2], argv3int);*/


        FormatSummaryByTest(argv[2], atoi(argv[3]));
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

        FormatSummaryByTestandResult(argv[2], atoi(argv[3]));
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
        PlayCompIVF(argc, argv);
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
        ShowHiddenCmds();
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
        CodeCoverage(argc, argv, WorkingDir, EmptyAr);
        return 0;
    }

    if (TestInputString.compare("CompareCodeCoverage") == 0)                //Compares New Code Coverage Results to Old Code Coverage Results and merges the files to an updated version
    {
        CompareCodeCoverage(argc, argv);
        return 0;
    }

    if (TestInputString.compare("FormatCodeCoverageFile") == 0)         //Formats Cygwin Code Coverage files to the same format as regular linux
    {
        FormatCodeCoverageFile(argc, argv);
        return 0;
    }

    if (TestInputString.compare("IVFDECTestVectorCheck") == 0)          //
    {
        IVFDECTestVectorCheck(argc, argv);
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
        PrintVersion();
        return 0;
    }

    if (TestInputString.compare("Test0InputTextCheck") == 0)                //Checks to make sure a Multi Run Input file has the correct format
    {
        Test0InputTextCheck(argv[2], 1);
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
                "    <inputfile>\n"
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
        GraphPSNR(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (TestInputString.compare("RandIVFComp") == 0)                //
    {
        RandComp(argc, argv, WorkingDir, EmptyAr, 1);
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
            SummCompAndTest.append("Mode1Results.txt");         // Mode 1
            SummComp = argv[3];
            SummComp.append(slashCharStr);
            SummComp.append("Mode2Results.txt");                    // Mode 2
            SummTest = argv[3];
            SummTest.append(slashCharStr);
            SummTest.append("Mode3Results.txt");                            // Mode 3

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

    if (selector == AlWDFNUM)
    {
        AllowDF(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == ALWLGNUM)
    {
        AllowLagTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == ALWSRNUM)
    {
        AllowSpatialResamplingTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == AUTKFNUM)
    {
        AutoKeyFramingWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == BUFLVNUM)
    {
        BufferLevelWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == CPUDENUM)
    {
        CPUDecOnlyWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == CHGWRNUM)
    {
        ChangeCPUWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == DFWMWNUM)
    {
        DFWM(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == DTARTNUM)
    {
        DataRateTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == DBMRLNUM)
    {
        DebugMatchesRelease(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == ENCBONUM)
    {
        EncoderBreakOut(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == ERRMWNUM)
    {
        ErrorRes(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == EXTFINUM)
    {
        ExtraFileCheck(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == FIXDQNUM)
    {
        FixedQ(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == FKEFRNUM)
    {
        ForceKeyFrameWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == FRSZTNUM)
    {
        FrameSizeTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == GQVBQNUM)
    {
        GoodQvBestQ(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == LGIFRNUM)
    {
        LagInFramesTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == MAXQUNUM)
    {
        MaxQTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == MEML1NUM)
    {
        MemLeakCheck(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == MEML2NUM)
    {
        MemLeakCheck2(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == MINQUNUM)
    {
        MinQTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == MULTTNUM)
    {
        MultiThreadedTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == NVOPSNUM)
    {
        NewVsOldPSNR(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == NVORTNUM)
    {
        NewVsOldRealTimeSpeed(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == NOISENUM)
    {
        NoiseSensitivityWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == OV2PSNUM)
    {
        OnePassVsTwoPass(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == PLYALNUM)
    {
        PlayAlternate(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == POSTPNUM)
    {
        PostProcessorWorks(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == RECONBUF)
    {
        ReconBuffer(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == RSDWMNUM)
    {
        ResampleDownWaterMark(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == SPEEDNUM)
    {
        SpeedTest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == TVECTNUM)
    {
        TestVectorCheck(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == TV2BTNUM)
    {
        TwoPassVsTwoPassBest(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == UNDSHNUM)
    {
        UnderShoot(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == VERSINUM)
    {
        Version(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    if (selector == WMLMMNUM)
    {
        WinLinMacMatch(argc, argv, WorkingDir, EmptyAr, 1);
        return 0;
    }

    OnErrorOutPut();
    return 0;

}
