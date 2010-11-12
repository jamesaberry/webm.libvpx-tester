#define _CRT_SECURE_NO_WARNINGS
#include "onyx.h"
#include "ivf.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vpx_config.h"
#include "vpx_mem.h"
#include "vp8cx.h"
#include "utilities.h"
#include <cstdio>
using namespace std;

#if defined(_WIN32)
#define snprintf _snprintf
#endif

int IVF2Raw(char *inputFile, char *outputDir)
{
    int WriteIndFrames = 5;//atoi(argv[4]);

    FILE *in = fopen(inputFile, "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        printf("\nInput file does not exist");
        fprintf(stderr, "\nInput file does not exist");
        fclose(in);
        return 0;
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);
    vpxt_format_ivf_header_read(&ivfhRaw);

    /*printf( "IVF DataRate\n\n"
    "FILE HEADER \n\n"
    "File Header            - %c%c%c%c \n"
    "File Format Version    - %i \n"
    "File Header Size       - %i \n"
    "Video Data FourCC      - %i \n"
    "Video Image Width      - %i \n"
    "Video Image Height     - %i \n"
    "Frame Rate Rate        - %i \n"
    "Frame Rate Scale       - %i \n"
    "Video Length in Frames - %i \n"
    "Unused                 - %c \n"
    "\n\n"
    ,ivfhRaw.signature[0],ivfhRaw.signature[1],ivfhRaw.signature[2],ivfhRaw.signature[3]
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.FourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
    ,ivfhRaw.scale,ivfhRaw.length,ivfhRaw.unused);*/

    IVF_FRAME_HEADER ivf_fhRaw;

    fread(&ivf_fhRaw.frameSize, 1, 4, in);
    fread(&ivf_fhRaw.timeStamp, 1, 8, in);
    vpxt_format_frame_header_read(ivf_fhRaw);

    frameCount = ivfhRaw.length;

    long nSamples = frameCount;
    long lRateNum = ivfhRaw.rate;
    long lRateDenom = ivfhRaw.scale;

    long nSamplesPerBlock = 1;

    long nBytes = 0;
    long nBytesMin = 999999;
    long nBytesMax = 0;

    fpos_t position;
    fgetpos(in, &position);
    cout << "\n";

    string OutputDirStrwithQuotes = outputDir;

    if (WriteIndFrames != 5)
    {
        OutputDirStrwithQuotes.append("\"");
        OutputDirStrwithQuotes.insert(0, "md \"");
        vpxt_make_dir(OutputDirStrwithQuotes);
    }

    char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

    string outputDirStr2 = outputDir;
    char outputDirChar2[255];

    if (WriteIndFrames != 5)
    {
        outputDirStr2.append(slashCharStr());
        outputDirStr2.append("AllFrames.raw");
        snprintf(outputDirChar2, 255, "%s", outputDirStr2.c_str());
    }
    else
    {
        snprintf(outputDirChar2, 255, "%s", outputDirStr2.c_str());
    }

    FILE *out2 = fopen(outputDirChar2, "wb");

    cout << "\n\nConverting to Raw\n";

    while (currentVideoFrame < frameCount)
    {
        cout << ".";
        memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        string outputDirStr = outputDir;
        char currentVideoFrameStr[10];
        vpx_itoa_custom(currentVideoFrame, currentVideoFrameStr, 10);
        outputDirStr.append(slashCharStr());
        outputDirStr.append("Frame_");
        outputDirStr.append(currentVideoFrameStr);

        char outputDirChar[255];
        snprintf(outputDirChar, 255, "%s", outputDirStr.c_str());

        if (WriteIndFrames == 0 || WriteIndFrames == 5)
        {
            fwrite(inbuff, 1, ivf_fhRaw.frameSize, out2);
        }

        if (WriteIndFrames == 1)
        {
            FILE *out = fopen(outputDirChar, "wb");
            fwrite(inbuff, 1, ivf_fhRaw.frameSize, out);
            fwrite(inbuff, 1, ivf_fhRaw.frameSize, out2);
            fclose(out);
        }

        fread(&ivf_fhRaw.frameSize, 1, 4, in);
        fread(&ivf_fhRaw.timeStamp, 1, 8, in);
        vpxt_format_frame_header_read(ivf_fhRaw);

        currentVideoFrame ++;
    }

    fclose(in);
    fclose(out2);

    cout << "\n";

    return 0;
}
int main(int argc, char *argv[])
{
    int FrameStats = 0;
    int forceUVswap2 = 0;
    int frameStats2 = 0;
    int PSNRRun = 0;
    int RunNTimes = 0;

    if (argc < 6)
    {
        printf("\n"
               "  IVF VP8 Compress Release\n\n"
               "    <Inputfile>\n"
               "    <Outputfile>\n"
               "    <Par File Origin 7 VP7 8 VP8>\n"
               "    <Par File>\n"
               "    <Extra Commands>\n"
               "      <0 No Extra Commands>\n"
               "      <1 Run PSNR only>\n"
               "      <2 Record Compression Time only>\n"
               "      <3 Record Compression Time and Run PSNR>\n"
               "\n"
               "\n"
               "   Debug Exe using: %s\n", vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
        return 0;
    }

    printf("\nDebug Exe using: %s\n", vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
    fprintf(stderr, "\nDebug Exe using: %s\n", vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    int ParVer = atoi(argv[3]);
    char *parfile = argv[4];
    int ExtraCommand = atoi(argv[5]);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);


    if (ParVer == 7)
    {
        cout << "\n\nNot Yet Supported\n\n";
    }

    if (ParVer == 8)
    {
        opt = vpxt_input_settings(parfile);
    }

    if (ExtraCommand == 4)
    {
        //This handles the Mem Leak Check Test.

        char *MemLeakCheckTXT = argv[6];

        vpx_memory_tracker_set_log_type(0, MemLeakCheckTXT);
        unsigned int CPUTick = 0;
        vpxt_time_compress_ivf_to_ivf(inputFile, outputFile, opt.multi_threaded, opt.target_bandwidth, opt, "VP8 Debug", 0, 0, CPUTick);
        vpx_memory_tracker_dump();

        return 0;
    }

    if (ExtraCommand == 5)
    {
        string MemLeakCheckTXTStr1 = argv[6];
        MemLeakCheckTXTStr1.append("_Encode.txt");
        char MemLeakCheckTXT1[255];
        snprintf(MemLeakCheckTXT1, 255, "%s", MemLeakCheckTXTStr1.c_str());

        vpx_memory_tracker_set_log_type(0, MemLeakCheckTXT1);
        int x = 0;
        int n = 0;
#ifdef API
        printf("\nAPI - Testing Faux Compressions:\n");
        fprintf(stderr, "\nAPI - Testing Faux Compressions:\n");
#else
        printf("\nTesting Faux Compressions:\n");
        fprintf(stderr, "\nTesting Faux Compressions:\n");
#endif

        while (x < 10000)
        {
            vpxt_faux_compress();

            if (n == 125)
            {
                printf(".");
                fprintf(stderr, ".");
                n = 0;
            }

            x++;
            n++;
        }

        vpx_memory_tracker_dump();
        return 0;
    }

    if (ExtraCommand == 6)
    {
        string MemLeakCheckTXTStr2 = argv[6];
        char *DecinputChar = argv[7];
        MemLeakCheckTXTStr2.append("_Decode.txt");
        char MemLeakCheckTXT2[255];
        snprintf(MemLeakCheckTXT2, 255, "%s", MemLeakCheckTXTStr2.c_str());

        vpx_memory_tracker_set_log_type(0, MemLeakCheckTXT2);
        int x = 0;
        int n = 0;
#ifdef API
        printf("\n\nAPI - Testing Faux Decompressions:\n");
        fprintf(stderr, "\n\nAPI - Testing Faux Decompressions:\n");
#else
        printf("\n\nTesting Faux Decompressions:\n");
        fprintf(stderr, "\n\nTesting Faux Decompressions:\n");
#endif

        while (x < 10000)
        {
            vpxt_faux_decompress(DecinputChar);

            if (n == 125)
            {
                printf(".");
                fprintf(stderr, ".");
                n = 0;
            }

            x++;
            n++;
        }

        vpx_memory_tracker_dump();

        return 0;
    }

    cout << "inputFile: " << inputFile << "\n";
    cout << "outputFile: " << outputFile << "\n";
    cout << "parfile: " << parfile << "\n";
    cerr << "inputFile: " << inputFile << "\n";
    cerr << "outputFile: " << outputFile << "\n";
    cerr << "parfile: " << parfile << "\n";

    string outputFile2 = outputFile;
    outputFile2.append("DEC.ivf");

    char outputFile2Char [256];

    snprintf(outputFile2Char, 255, "%s", outputFile2.c_str());
    unsigned int CPUTick = 0;
    vpxt_time_compress_ivf_to_ivf(inputFile, outputFile, 0, opt.target_bandwidth, opt, "VP8 Release", 0, 0, CPUTick);

    double totalPsnr;

    if (ExtraCommand == 1 || ExtraCommand == 3)
    {
        cout << "\n\n";
        double ssimDummyVar = 0;
        totalPsnr = vpxt_ivf_psnr(inputFile, outputFile, 0, 0, 1, NULL);

        char TextFilechar1[255];

        vpxt_remove_file_extension(outputFile, TextFilechar1);

        char *FullName = strcat(TextFilechar1, "OLD_PSNR.txt");

        ofstream outfile2(FullName);
        outfile2 << totalPsnr;
        outfile2.close();
    }

    return 0;
}