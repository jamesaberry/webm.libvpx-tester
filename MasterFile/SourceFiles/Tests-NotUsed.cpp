#define _CRT_SECURE_NO_WARNINGS
//------------------------------------------------------------------------------
//
//  Copyright (c) 1999-2005  On2 Technologies Inc.  All Rights Reserved.
//  The name is deceptive and is a carry over from the vp6 basic testing suite
//  Routine these tests are used for vp8
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
#include <cstdio>
#include <cmath>
#include <vector>
#include <stdlib.h>
/////PSNR/////
#include "yv12config.h"
#include <iostream>
#include <fstream>
using namespace std;
/////CompIVF///////////////
#include "CompIVF.h"
//////////////////////////

#if defined(_WIN32)
#define snprintf _snprintf
#include "on2vpplugin.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdio>

extern char *itoa_custom(int value, char *result, int base);

////////////////////////Global Slash Character Definion for multiplat////////////////////////
extern char slashChar;
extern string slashCharStr;
/////////////////////////////////////////////////////////////////////////////////////////////

//driver
extern string DateString();

//Utilities
extern int absInt(int input);
extern float absFloat(float input);
extern double absDouble(double input);
extern VP8_CONFIG InPutSettings(char *inputFile);
extern int OutPutSettings(char *outputFile, VP8_CONFIG opt);
extern long FileSize(char *inFile);
extern void FolderName(char *input, char *output);
extern void TestName(char *input, char *TestName);
extern void SubFolderName(char *input, char *FileName);
extern unsigned int GetTime();
extern int MakeDirVPX(string CreateDir2);
extern int CompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt);
extern unsigned int TimeCompressIVFtoIVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &opt, char *CompressString, int CompressInt);
extern int DecompressIVFtoIVF(char *inputFile, char *outputFile2);
extern unsigned int DecompressIVFtoIVFTimeAndOutput(char *inputFile, char *outputFile2);
extern double IVFPSNR(char *inputFile1, char *inputFile2, int forceUVswap, int frameStats, int printvar, double &SsimOut);
extern int PSNRSelect(char *inFile, char *outFile);
extern int IVFCheckPBM(char *inputFile, int bitRate, int maxBuffer, int preBuffer);
extern int CompIVF(char *inputFile1, char *inputFile2);
extern int MultiPlatTimeRet(char *infile);
extern double IVFDataRate(char *inputFile, int DROuputSel);
extern int OutPutCompatSettings(char *outputFile, VP8_CONFIG opt, int ParVersionNum);
extern void VP8DefaultParms(VP8_CONFIG &opt);

//tests
extern void RecordTestComplete(string MainDirString, string File1String, int TestType);
extern void PrintHeader1(int argc, char *argv[], string WorkingDir3);
extern void PrintHeader2(int argc, char *argv[], string WorkingDir3);
extern void PrintHeader3(int argc, char *argv[], string WorkingDir3);

//DataRate
extern double DataRate(int argc, char *argv);

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

    unsigned int ON2_GetHighResTimerTick();
    unsigned int ON2_GetTimeInMicroSec(unsigned int startTick, unsigned int stopTick);
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

    printf("Buffer Level Works Test\n");
    fprintf(stderr, "Buffer Level Works Test\n");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;
    int StartingBufferLvl = 4;
    int MaximumBufferLevel = 6;

    int constInt = 128;

    int CalcBufferSize = BitRate * MaximumBufferLevel * constInt;
    int CalcPrebuffer = BitRate * StartingBufferLvl * constInt;

    char CharCalcBufferSize[32];
    char CharCalcPrebuffer[32];
    char CharBitRate[32];

    itoa_custom(CalcBufferSize, CharCalcBufferSize, 10);
    itoa_custom(CalcPrebuffer, CharCalcPrebuffer, 10);
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

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, BufferLevelWorksOut, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

    int PassFail = IVFCheckPBM(BufferLevelWorksOut, BitRate, CalcBufferSize, CalcPrebuffer);

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

    printf("CPU Dec Only Works\n");
    fprintf(stderr, "CPU Dec Only Works\n");

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
        putenv("ON2_SIMD_CAPS=0");

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
            putenv((char *)CPUIDSTRING.c_str());

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

            putenv("ON2_SIMD_CAPS=0");

            printf("\n");
            fprintf(stderr, "\n");

            int countOld = (counter - 1);
            itoa_custom(countOld, count, 10);

            Output3.append(count);

            if (DoOnceSpeedRead == 0)
            {
                char GetTimeChar[255];
                snprintf(GetTimeChar, 255, "%s", Output.c_str());

                totalms = MultiPlatTimeRet(GetTimeChar);
                DoOnceSpeedRead = 1;
            }
            else
            {
                char GetTimeChar[255];
                snprintf(GetTimeChar, 255, "%s", Output.c_str());

                totalms2 = MultiPlatTimeRet(GetTimeChar);
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, CPUDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

    if (Fail == 0)
    {
        if (totalms == totalms2)
        {
            printf("Fail: CPU changes are not effecting the runtime\n");
            fprintf(stderr, "Fail: CPU changes are not effecting the runtime\n");
            fclose(fp);
            putenv("ON2_SIMD_CAPS=");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (totalms != totalms2)
        {
            printf("\nAll files are identical and decompress times are not equal: Pass\n\n");
            fprintf(stderr, "\nAll files are identical and their decompress times are not equal: Pass\n\n");
            fclose(fp);
            putenv("ON2_SIMD_CAPS=");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }

    if (Fail == 1)
    {
        printf("\nFail: All Files not Identical.\n");
        fprintf(stderr, "\nFail: All Files not Identical.\n");
        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    putenv("ON2_SIMD_CAPS=");
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}

int ChangeCPUDec(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "Version";

    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  ChangeCPUDec \n\n"
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
    char *MyDir = "ChangeCPUDec";
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
    WorkingDir4.append("ChangedCPUDecOutput.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("ChangedCPUDec_CPU");

    char ChangedCPUDecOutFile[255];
    char ChangedCPUDec_CPUBase[255];

    snprintf(ChangedCPUDecOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(ChangedCPUDec_CPUBase, 255, "%s", WorkingDir5.c_str());

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

    printf("Change CPU Dec Test");
    fprintf(stderr, "Change CPU Dec Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int VersionNum = atoi(argv[5]);

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

    unsigned int totalms1 = 0;
    unsigned int totalms = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        printf("\n\n");
        fprintf(stderr, "\n\n");
        int counter = 0;
        ////////////////////////////Start Decode and recording time



        string Output2 = ChangedCPUDec_CPUBase;
        Output2.append("0.ivf");
        char Output2Char[1024];
        snprintf(Output2Char, 1024, "%s", Output2.c_str());

        putenv("ON2_SIMD_CAPS=0");
        counter = counter + 1;

        totalms = MultiPlatTimeRet(Output2Char);

        int counterMax = 12;
        int i = 1;
        string OutputLast;

        while (counter < counterMax)
        {
            int nameselect = 0;

            printf("\n\n");
            fprintf(stderr, "\n\n");

            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);
            putenv((char *)CPUIDSTRING.c_str());

            //Name strings
            string Output = ChangedCPUDec_CPUBase;
            string Output3 = ChangedCPUDec_CPUBase;
            char count[20];
            itoa_custom(counter, count, 10);

            Output.append(count);
            Output.append(".ivf");

            char ChangedCPUDecOutFileChar[255];
            snprintf(ChangedCPUDecOutFileChar, 255, "%s", Output.c_str());

            int countOld = (counter - 1) / 2;
            itoa_custom(countOld, count, 10);

            Output3.append(count);
            Output3.append(".ivf");


            printf("\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);
            fprintf(stderr, "\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);

            char Comp1[255];
            char Comp2[255];

            snprintf(Comp1, 255, "%s", Output.c_str());
            snprintf(Comp2, 255, "%s", Output3.c_str());

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

            totalms1 = MultiPlatTimeRet(Comp2);

            counter = (counter * 2) + 1;
            i++;
        }
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, ChangedCPUDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangedCPUDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangedCPUDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangedCPUDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangedCPUDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        printf("\n\n");
        fprintf(stderr, "\n\n");
        int counter = 0;

        ////////////////////////////Start Decode and recording time

        string Output2 = ChangedCPUDec_CPUBase;
        Output2.append("0.ivf");
        char Output2Char[1024];
        snprintf(Output2Char, 1024, "%s", Output2.c_str());

        putenv("ON2_SIMD_CAPS=0");
        printf("CPU:%i\n", 0);
        fprintf(stderr, "CPU:%i\n", 0);

        totalms1 = DecompressIVFtoIVFTimeAndOutput(ChangedCPUDecOutFile, Output2Char);
        printf("Decompression Time in Microseconds: %i", totalms1);
        fprintf(stderr, "Decompression Time in Microseconds: %i", totalms1);
        counter = counter + 1;

        int counterMax = 12;
        int i = 1;
        string OutputLast;

        while (counter < counterMax)
        {
            int nameselect = 0;

            printf("\n\n");
            fprintf(stderr, "\n\n");

            string CPUIDSTRING = "ON2_SIMD_CAPS=";
            char CounterChar[10];
            itoa_custom(counter, CounterChar, 10);
            CPUIDSTRING.append(CounterChar);
            putenv((char *)CPUIDSTRING.c_str());

            printf("CPU:%i\n", counter);
            fprintf(stderr, "CPU:%i\n", counter);

            //Name strings
            string Output = ChangedCPUDec_CPUBase;
            string Output3 = ChangedCPUDec_CPUBase;
            char count[20];
            itoa_custom(counter, count, 10);

            Output.append(count);
            Output.append(".ivf");

            char ChangedCPUDecOutFileChar[255];
            snprintf(ChangedCPUDecOutFileChar, 255, "%s", Output.c_str());

            totalms = DecompressIVFtoIVFTimeAndOutput(ChangedCPUDecOutFile, ChangedCPUDecOutFileChar);

            printf("Decompression Time in Microseconds: %i", totalms);
            fprintf(stderr, "Decompression Time in Microseconds: %i", totalms);

            if (counter == 1)
            {
                Output3.append("0.ivf");
            }
            else
            {
                int countOld = (counter - 1) / 2;
                itoa_custom(countOld, count, 10);

                Output3.append(count);
                Output3.append(".ivf");
            }

            if (TestType != 2)
            {
                printf("\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);
                fprintf(stderr, "\ncomparing CPU:%i to CPU:%i", (counter - 1) / 2, counter);

                char Comp1[255];
                char Comp2[255];

                snprintf(Comp1, 255, "%s", Output.c_str());
                snprintf(Comp2, 255, "%s", Output3.c_str());

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
        putenv("ON2_SIMD_CAPS=");
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 10;
    }

    cout << "\n";
    cerr << "\n";

    if (Fail != 1)
    {
        if (totalms == totalms1)
        {
            printf("Fail: CPU changes are not effecting the runtime");
            fprintf(stderr, "Fail: CPU changes are not effecting the runtime");

            fclose(fp);
            putenv("ON2_SIMD_CAPS=");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (totalms != totalms1)
        {
            printf("\n\nAll files are identical and Times are not all equal: Pass.\n\n");
            fprintf(stderr, "\n\nAll files are identical and Times are not all equal: Pass.\n\n");

            fclose(fp);
            putenv("ON2_SIMD_CAPS=");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }

    if (Fail == 1)
    {
        printf("Fail: Files are not identical.\n\n");
        fprintf(stderr, "Fail: Files are not identical.\n\n");

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    putenv("ON2_SIMD_CAPS=");
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
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

    printf("Change CPU Works\n");
    fprintf(stderr, "Change CPU Works\n");

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
        Time1 = MultiPlatTimeRet(ChangedCPUDec0OutFile);

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

            Time2 = MultiPlatTimeRet(Comp1);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangedCPUDec0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt);

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
                Time2 = TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt);

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
                Time2 = Time2 + TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt);

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
                Time2 = Time2 + TimeCompressIVFtoIVF(input, ChangedCPUDecNOutFileFinal, speed, BitRate, opt, CompressString, CompressInt);

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


    if (Fail != 1)
    {
        if (Time1 == Time2)
        {
            printf("\nFail: Time1: %i Time2: %i - CPU changes are not effecting the runtime\n", Time1, Time2);
            fprintf(stderr, "\nFail:  Time1: %i Time2: %i - CPU changes are not effecting the runtime\n", Time1, Time2);
            fclose(fp);
            putenv("ON2_SIMD_CAPS=");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Time1 != Time2)
        {
            printf("\n\nAll Files Identical and Time 1: %i Not = Time 2: %i - Pass\n\n", Time1, Time2);
            fprintf(stderr, "\n\nAll Files Identical and Time 1: %i Not = Time 2: %i - Pass\n\n", Time1, Time2);
            fclose(fp);
            putenv("ON2_SIMD_CAPS=");
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 1;
        }
    }

    if (Fail == 1)
    {
        printf("\n\n\nFail: All Files not identical.\n");
        fprintf(stderr, "\n\n\nFail: All Files not identical.\n");
        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    putenv("ON2_SIMD_CAPS=");
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;
}


int NewVsOldRealTimeSpeed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
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

    printf("New Vs Old Real Time Speed Test\n");
    fprintf(stderr, "New Vs Old Real Time Speed Test\n");

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
        Time1 = MultiPlatTimeRet(outputVP8New);
        Time2 = MultiPlatTimeRet(outputVP8Old2);

        cout << "\nTime1: " << Time1 << "\n";
        cout << "\nTime2: " << Time2 << "\n";
    }
    else
    {

        opt.Mode = MODE_REALTIME;
        Time1 = TimeCompressIVFtoIVF(input, outputVP8New, speed, BitRate, opt, "VP8", 0);

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
        Time2 = MultiPlatTimeRet(outputVP8Old2);

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

    printf("\n\n Files Took the same amount of time. \n");
    fprintf(stderr, "\n\n Files Took the same amount of time. \n");

    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 2;
}



int OnePassVsTwoPass(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
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

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("TwoPassOutput1.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("OnePassOutput2.ivf");

    char TwoPassOutFile[255];
    char OnePassOutFile[255];

    snprintf(TwoPassOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(OnePassOutFile, 255, "%s", WorkingDir5.c_str());

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

    printf("One Pass Vs Two Pass Test\n");
    fprintf(stderr, "One Pass Vs Two Pass Test\n");

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

    opt.TargetBandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        opt.Mode = 5;

        if (CompressIVFtoIVF(input, TwoPassOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.Mode = 2;

        if (CompressIVFtoIVF(input, OnePassOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

    float Size2 = IVFDataRate(TwoPassOutFile, 1);
    float Size1 = IVFDataRate(OnePassOutFile, 1);

    int PSNRToggle = PSNRSelect(input, TwoPassOutFile);

    double PSNR1;
    double PSNR2;

    double ssim = 0;
    PSNR2 = IVFPSNR(input, TwoPassOutFile, PSNRToggle, 0, 1, ssim);
    PSNR1 = IVFPSNR(input, OnePassOutFile, PSNRToggle, 0, 1, ssim);

    double PSRNPerc = absDouble(((PSNR2 - PSNR1) / PSNR1) * 100.00);
    double BRPerc = absDouble(((Size2 - Size1) / Size1) * 100.00);
    int Pass = 0;

    if (PSNR1 == PSNR2)
    {
        printf("\n\n No Difference Between One Pass and Two Pass Good Quality PSNRs");
        fprintf(stderr, "\n\n No Difference Between One Pass and Two Pass Good Quality PSNRs");
    }

    if (PSNR1 < PSNR2)
    {
        printf("\n\n Two Pass Good Quality returned a %f%% higher PSNR", PSRNPerc);
        fprintf(stderr, "\n\n Two Pass Good Quality returned a %f%% higher PSNR", PSRNPerc);
        Pass = 1;
    }

    if (PSNR1 > PSNR2)
    {
        printf("\n\n Two Pass Good Quality returned a %f% lower PSNR", PSRNPerc);
        fprintf(stderr, "\n\n Two Pass Good Quality returned a %f%% lower PSNR", PSRNPerc);
    }

    if (Size2 == Size1)
    {
        printf("\n No Difference Between One Pass and Two Pass Good Quality Bitrates\n");
        fprintf(stderr, "\n No Difference Between One Pass and Two Pass Good Quality Bitrates\n");
    }

    if (Size2 > Size1)
    {
        printf("\n Two Pass Good Quality returned a %f%% higher Bitrate \n ", BRPerc);
        fprintf(stderr, "\n Two Good Quality Best returned a %f%% higher Bitrate \n ", BRPerc);
    }

    if (Size2 < Size1)
    {
        printf("\n Two Pass Good Quality returned a %f%% lower Bitrate\n ", BRPerc);
        fprintf(stderr, "\n Two Pass Good Quality returned a %f%% lower Bitrate\n ", BRPerc);
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


int ResampleDownWaterMark(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
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

    printf("ResampleDownWaterMark");
    fprintf(stderr, "ResampleDownWaterMark");

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
    opt.AllowSpatialResampling = 1;
    opt.EndUsage = 0;
    opt.TargetBandwidth = BitRate;

    int n = 0;
    double PSNRArr[2];

    opt.ResampleUpWaterMark = 100;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {

    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.ResampleDownWaterMark = 90;

            if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.ResampleDownWaterMark = 10;

            if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
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
            opt.ResampleDownWaterMark = 90;

            if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.ResampleDownWaterMark = 10;

            if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
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
            opt.ResampleDownWaterMark = 90;

            if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_BESTQUALITY;
            opt.ResampleDownWaterMark = 10;

            if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
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
            opt.ResampleDownWaterMark = 90;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.ResampleDownWaterMark = 10;
            opt.Mode = MODE_SECONDPASS;

            if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 5)
        {
            opt.ResampleDownWaterMark = 90;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, DownWaterSamp90OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                RecordTestComplete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.ResampleDownWaterMark = 10;
            opt.Mode = MODE_SECONDPASS_BEST;

            if (CompressIVFtoIVF(input, DownWaterSamp10OutFile, speed, BitRate, opt, CompressString, opt.ResampleDownWaterMark) == -1)
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

    double ssim = 0;
    PSNRArr[0] = IVFPSNR(input, DownWaterSamp90OutFile, PSNRToggle, 0, 1, ssim);
    PSNRArr[1] = IVFPSNR(input, DownWaterSamp10OutFile, PSNRToggle, 0, 1, ssim);



    if (PSNRArr[0] < PSNRArr[1])
    {
        printf("\n Passed - Down Water Mark Sample 10: %f PSNR is greater than 90: %f \n", PSNRArr[1], PSNRArr[0]);
        fprintf(stderr, "\n Passed - Down Water Mark Sample 10: %f PSNR is greater than 90: %f \n", PSNRArr[1], PSNRArr[0]);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (PSNRArr[0] == PSNRArr[1])
    {
        printf("\n Unknown: Resample-Down-Watermark has no effect, try different parameters \n"/*, PSNRArr[0], PSNRArr[1]*/);
        fprintf(stderr, "\n Unknown: Resample-Down-Watermark has no effect, try different parameters \n"/*, PSNRArr[0], PSNRArr[1]*/);
        fclose(fp);
        string File1Str = File1;
        RecordTestComplete(MainDirString, File1Str, TestType);
        return 2;
    }


    printf("\n Failed - Down Water Mark Sample 90: %f PSNR is greater than 10: %f \n", PSNRArr[0], PSNRArr[1]);
    fprintf(stderr, "\n Failed - Down Water Mark Sample 90: %f PSNR is greater than 10: %f \n", PSNRArr[0], PSNRArr[1]);
    fclose(fp);
    string File1Str = File1;
    RecordTestComplete(MainDirString, File1Str, TestType);
    return 0;

}
//----------------------------------------Tests No Longer Used----------------------------------------------------------------
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
        totalms = MultiPlatTimeRet(ChangeIDCTDecOutputDEC0);
        totalms2 = MultiPlatTimeRet(ChangeIDCTDecOutputDEC1);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, ChangeIDCTDecOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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
        Time1 = MultiPlatTimeRet(ChangeIDCT1OutFile);
        Time2 = MultiPlatTimeRet(ChangeIDCT0OutFile);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time1 = TimeCompressIVFtoIVF(input, ChangeIDCT0OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
            Time2 = TimeCompressIVFtoIVF(input, ChangeIDCT1OutFile, speed, BitRate, opt, CompressString, CompressInt);

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
        totalms = MultiPlatTimeRet(IDCTDecOnlyWorksOutputDecodedIDCT0);
        totalms2 = MultiPlatTimeRet(IDCTDecOnlyWorksOutputDecodedIDCT1);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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

            if (CompressIVFtoIVF(input, IDCTDecOnlyWorksOutFile, speed, BitRate, opt, CompressString, CompressInt) == -1)
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


int VP61vVP62(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " VP61vVP62 - Removed as of 03 29 2010 - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;

    // So long as Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>
    if (argc != 6)
    {
        printf(
            "  VP61vVP62 \n\n"
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
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char *ExeInput = argv[5];

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "VP61vVP62";
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
    char ExeChar[1024];
    FolderName(argv[0], ExeChar);
    string ExeString = ExeChar;
    CreateDir2.insert(0, "md \"");
    ExeString.insert(0, "\"\"");
    MakeDirVPX(CreateDir2.c_str());

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = ExeString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("VP62.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("VP61.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("VP61.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("ParFile.txt");
    WorkingDir8.append(ExeInput);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(input);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(WorkingDir5);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(WorkingDir7);
    WorkingDir8.append("\"\"");

    char VP62[255];
    char VP61[255];
    char VP61b[255];
    char ParFile[255];
    char Program[1024];

    snprintf(VP62, 255, "%s", WorkingDir4.c_str());
    snprintf(VP61, 255, "%s", WorkingDir5.c_str());
    snprintf(VP61b, 255, "%s", WorkingDir6.c_str());
    snprintf(ParFile, 255, "%s", WorkingDir7.c_str());
    snprintf(Program, 1024, "%s", WorkingDir8.c_str());

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


    printf("VP61vVP62\n");
    fprintf(stderr, "VP61vVP62\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    opt.TargetBandwidth = BitRate;
    opt.AutoKey = 1;

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;

        if (CompressIVFtoIVF(input, VP62, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;

        if (CompressIVFtoIVF(input, VP62, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    if (Mode == 2)
    {
        opt.Mode = MODE_BESTQUALITY;

        if (CompressIVFtoIVF(input, VP62, speed, BitRate, opt, "VP8", 718) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        OutPutSettings(ParFile, opt);
        system(Program);
    }

    if (Mode == 3)
    {
    }

    if (Mode == 4)
    {
        opt.Mode = MODE_SECONDPASS;

        if (CompressIVFtoIVF(input, VP62, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    if (Mode == 5)
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (CompressIVFtoIVF(input, VP62, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    int lngRC = CompIVF(VP62, VP61);

    if (lngRC >= 0)
    {
        printf("\n\nFail: Files differ at frame: %i", lngRC);
        fprintf(stderr, "\n\nFail: Files differ at frame: %i", lngRC);
        return 0;
    }

    if (lngRC == -1)
    {
        printf("\nFiles are identical");
        fprintf(stderr, "\nFiles are identical");
        return 1;
    }

    if (lngRC == -2)
    {
        printf("\n\nFail: File 2 ends before File 1.\n"/*, lngRC*/);
        fprintf(stderr, "\n\nFail: File 2 ends before File 1.\n"/*, lngRC*/);
        return 0;
    }

    if (lngRC == -3)
    {
        printf("\n\nFail: File 1 ends before File 2.\n"/*, lngRC*/);
        fprintf(stderr, "\n\nFail: File 1 ends before File 2.\n"/*, lngR*/);
        return 0;
    }

    fclose(fp);
    return 0;
}
int VP6MvVP60(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    printf(
        "\n"
        "\n"
        " ******************************************************************************\n"
        " VP6MvVP60 - Removed as of 03 29 2010 - This Is not a vaild VP8 Test\n"
        " ******************************************************************************\n"
        "\n"
        "\n"
    );
    return 12;

    // So long as Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>
    if (argc != 6)
    {
        printf(
            "  VP6MvVP60 \n\n"
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
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char *ExeInput = argv[5];

    ///////////////////////////////////////////////Formatting Test Specific Directory////////////////////////////
    char WorkingDir2[255];
    char WorkingDir3[255];
    char *MyDir = "VP6MvVP60";
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
    char ExeChar[1024];
    FolderName(argv[0], ExeChar);
    string ExeString = ExeChar;
    CreateDir2.insert(0, "md \"");
    ExeString.insert(0, "\"\"");
    MakeDirVPX(CreateDir2.c_str());

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = ExeString;

    WorkingDir4.append(slashCharStr);
    WorkingDir4.append("VP6M.ivf");
    WorkingDir5.append(slashCharStr);
    WorkingDir5.append("VP60.ivf");
    WorkingDir6.append(slashCharStr);
    WorkingDir6.append("VP60.ivf");
    WorkingDir7.append(slashCharStr);
    WorkingDir7.append("ParFile.txt");
    WorkingDir8.append(ExeInput);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(input);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(WorkingDir5);
    WorkingDir8.append("\" \"");
    WorkingDir8.append(WorkingDir7);
    WorkingDir8.append("\"\"");

    char VP6M[255];
    char VP60[255];
    char VP60v[255];
    char ParFile[255];
    char Program[1024];

    snprintf(VP6M, 255, "%s", WorkingDir4.c_str());
    snprintf(VP60, 255, "%s", WorkingDir5.c_str());
    snprintf(VP60v, 255, "%s", WorkingDir6.c_str());
    snprintf(ParFile, 255, "%s", WorkingDir7.c_str());
    snprintf(Program, 255, "%s", WorkingDir8.c_str());

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


    printf("VP6MvVP60\n");
    fprintf(stderr, "VP6MvVP60\n");

    VP8_CONFIG opt;
    VP8DefaultParms(opt);

    opt.TargetBandwidth = BitRate;
    opt.AutoKey = 1;

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;

        if (CompressIVFtoIVF(input, VP6M, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;

        if (CompressIVFtoIVF(input, VP6M, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    if (Mode == 2)
    {
        opt.Mode = MODE_BESTQUALITY;

        if (CompressIVFtoIVF(input, VP6M, speed, BitRate, opt, "VP8", 718) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            RecordTestComplete(MainDirString, File1Str, TestType);
            return 2;
        }

        OutPutSettings(ParFile, opt);
        system(Program);
    }

    if (Mode == 3)
    {
    }

    if (Mode == 4)
    {
        opt.Mode = MODE_SECONDPASS;

        if (CompressIVFtoIVF(input, VP6M, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    if (Mode == 5)
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (CompressIVFtoIVF(input, VP6M, speed, BitRate, opt, "VP8", 718) == -1)
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

        system(Program);
    }

    int lngRC = CompIVF(VP6M, VP60);

    if (lngRC >= 0)
    {
        printf("\n\nFail: Files differ at frame: %i", lngRC);
        fprintf(stderr, "\n\nFail: Files differ at frame: %i", lngRC);
        return 0;
    }

    if (lngRC == -1)
    {
        printf("\nFiles are identical");
        fprintf(stderr, "\nFiles are identical");
        return 1;
    }

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

    fclose(fp);
    return 0;
}