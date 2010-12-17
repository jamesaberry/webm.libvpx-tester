#define _CRT_SECURE_NO_WARNINGS
#include "utilities.h"
#include "driver.h"
#include "comp_ivf.h"
#include "onyx.h"
#include "ivf.h"
#include "x86.h"
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

#if defined(_WIN32)
#include "on2vpplugin.h"
#define snprintf _snprintf
#endif
#if defined(linux)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif
#if defined(__APPLE__)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif
#if defined(__POWERPC__)
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

//ivfdec ivfenc
extern int ivfdec(int argc, const char **argv_);
extern int ivfenc(int argc, const char **argv_);

int ComprIVF2IVF(int argc, char *argv[], string WorkingDir)
{
    char *CompressString = "Allow DF";

    if (argc < 7 || argc > 8)
    {
        tprintf(PRINT_STD,
                "\n  Compress IVF to IVF \n\n"
                "    <(1)Normal Compress |(2)TimeCompress>\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <Bit Rate>\n"
                "    <Mode>\n"
                "    <Optional - Parameter File>\n"
                "\n");

        return 0;
    }

    int CompressionType = atoi(argv[2]);
    char *input = argv[3];
    char *output = argv[4];
    int BitRate = atoi(argv[5]);
    int Mode = atoi(argv[6]);

    int speed = 0;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;

    if (argc == 8)
    {
        opt = vpxt_input_settings(argv[7]);
    }

    int CompressInt = opt.allow_df;

    ////////////Track Mem Usage//////////
    //string MemLeakCheckTXT2Str = output;
    //MemLeakCheckTXT2Str.append("_MemOut.txt");
    //char MemLeakCheckTXT2[255];
    //snprintf(MemLeakCheckTXT2, 255, "%s", MemLeakCheckTXT2Str.c_str());
    //on2_MemoryTrackerSetLogType(0, MemLeakCheckTXT2);
    //cout << "\nMemory Tracking to file: " << MemLeakCheckTXT2 << "\n";
    /////////////////////////////////////

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;

        if (CompressionType == 1)
        {
            vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        }

        if (CompressionType == 2)
        {
            unsigned int CPUTick = 0;
            vpxt_time_compress_ivf_to_ivf(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
        }
    }

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;

        if (CompressionType == 1)
        {
            vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        }

        if (CompressionType == 2)
        {
            unsigned int CPUTick = 0;
            vpxt_time_compress_ivf_to_ivf(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
        }
    }

    if (Mode == 2)
    {
        opt.Mode = MODE_BESTQUALITY;

        if (CompressionType == 1)
        {
            vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        }

        if (CompressionType == 2)
        {
            unsigned int CPUTick = 0;
            vpxt_time_compress_ivf_to_ivf(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
        }
    }

    if (Mode == 3)
    {
    }

    if (Mode == 4)
    {
        opt.Mode = MODE_SECONDPASS;

        if (CompressionType == 1)
        {
            vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        }

        if (CompressionType == 2)
        {
            unsigned int CPUTick = 0;
            vpxt_time_compress_ivf_to_ivf(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
        }
    }

    if (Mode == 5)
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (CompressionType == 1)
        {
            vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        }

        if (CompressionType == 2)
        {
            unsigned int CPUTick = 0;
            vpxt_time_compress_ivf_to_ivf(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
        }
    }

    ////////////Track Mem Usage//////////
    //on2_MemoryTrackerDump();
    /////////////////////////////////////
    return 0;
}


int CopyIVF2IVF(int argc, char *argv[], string WorkingDir)
{
    char *CompressString = "Allow DF";

    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  Copy IVF to IVF \n\n"
                "    <Input File>\n"
                "    <outputfile>\n ");

        return 0;
    }

    char *input = argv[2];
    char *output = argv[3];

    int speed = 0;
    int BitRate = 128;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);
    int CompressInt = opt.allow_df;

    opt.target_bandwidth = BitRate;

    opt.Mode = MODE_GOODQUALITY;

    return 0;

}
int DecIVF2IVF(int argc, char *argv[])
{
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  Decompress IVF to IVF \n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "\n");

        return 0;
    }

    char *inputFile = argv[2];
    char *outputFile = argv[3];

    vpxt_decompress_ivf_to_ivf_no_output(inputFile, outputFile);

    return 0;
}
int DecIVF2Raw(int argc, char *argv[])
{
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  Decompress IVF to Raw \n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "\n");

        return 0;
    }

    char *inputFile = argv[2];
    char *outputFile = argv[3];

    vpxt_decompress_ivf_to_raw(inputFile, outputFile);

    return 0;
}
int IVFDataRateTool(int argc, char *argv[])
{
    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  IVF DataRate \n\n"
                "    <Input File>\n"
                "\n");
        return 0;
    }

    vpxt_ivf_data_rate(argv[2], 2);

    return 1;
}
int IVFPSNRrun(int argc, char *argv[])
{
    if (argc < 5)
    {
        tprintf(PRINT_STD,
                "\n  IVFPSNR\n\n"
                "     <Raw IVF File>\n"
                "     <Comp IVF File>\n"
                "     <Run SSIM 0 no 1 yes>\n"
                "\n");
        exit(0);
    }

    double runssim = 1;
    char *Raw = argv[2];
    char *Comp = argv[3];

    if (atoi(argv[4]) == 1)
    {
        vpxt_ivf_psnr(Raw, Comp, 0, 3, 0, &runssim);
    }
    else
    {
        vpxt_ivf_psnr(Raw, Comp, 0, 3, 0, NULL);
    }

    tprintf(PRINT_STD, "\n");

    return 0;
}
int IVFCheckPBMrun(int argc, char *argv[])
{
    if (argc < 6)
    {
        tprintf(PRINT_STD,
                "\n  IVFCheckPBM\n\n"
                "     <input>\n"
                "     <bitrate>\n"
                "     <bufferSize>\n"
                "     <prebuffer>\n"
                "\n");
        exit(0);
    }

    char *inputFile = argv[2];
    int bitRate = atoi(argv[3]);
    int maxBuffer = atoi(argv[4]);
    int preBuffer = atoi(argv[5]);
    int outputme;

    outputme = vpxt_ivf_check_pbm(inputFile, bitRate, maxBuffer, preBuffer);

    if (outputme == -11)
    {
        tprintf(PRINT_STD, "CheckPBM Passed\n");
    }
    else
    {
        tprintf(PRINT_STD, "CheckPBM failed at: %i\n", outputme);
    }

    return 0;
}
int compareIVF(int argc, char *argv[])
{
    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  CompareIVF\n\n"
                "     <inputfile1>\n"
                "     <inputfile2>\n"
               );
        return 0;
    }

    char *Output3 = argv[2];
    char *Output = argv[3];

    tprintf(PRINT_STD, "\nComparing Files:\n\n");
    cout << Output3 << "\n";
    cout << Output << "\n";

    char *CompIVFAr[5];
    CompIVFAr[1] = (char *)Output;
    CompIVFAr[2] = (char *)Output3;

    int CompIVFOutput = vpxt_compare_ivf(Output3, Output);

    if (CompIVFOutput == -1)
    {
        cout << "\n Files Contain Identical Video Data\n";
    }

    if (CompIVFOutput == -2)
    {
        tprintf(PRINT_STD, "\n\nFail: File 2 ends before File 1.\n");
    }

    if (CompIVFOutput == -3)
    {
        tprintf(PRINT_STD, "\n\nFail: File 1 ends before File 2.\n");
    }

    if (CompIVFOutput != -1 && CompIVFOutput != -2 && CompIVFOutput != -3)
    {
        cout << "\nFILES DIFFER AT FRAME: " << CompIVFOutput << "\n";
    }

    return 0;
}
int RawDataIVF(char *input, char *output)
{
    FILE *in = fopen(input, "rb");
    FILE *out = fopen(output, "wb");

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file: - does not exist");
        return 0;
    }

    if (out == NULL)
    {
        tprintf(PRINT_BTH, "\nOutput file: - does not exist");
        return 0;
    }

    VP8_CONFIG oxcf;
    vpxt_default_parameters(oxcf);

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);

    oxcf.Width      = ivfhRaw.width;
    oxcf.Height     = ivfhRaw.height;

    IVF_FRAME_HEADER ivf_fhRaw;

    int frameSize = oxcf.Width * oxcf.Height * 3 / 2;
    char *outbuff = new char[frameSize];
    unsigned char *inbuff = new unsigned char[oxcf.Width * oxcf.Height * 3/2];

    int Framecount = 0;

    while (!feof(in))
    {
        if (!fread(&ivf_fhRaw.frameSize, 1, 4, in))
            break;

        if (!fread(&ivf_fhRaw.timeStamp, 1, 8, in))
            break;

        if (!fread((char *)inbuff, 1, ivf_fhRaw.frameSize, in))
            break;

        fwrite(inbuff, 1, frameSize, out);
        Framecount ++;
    }

    fclose(in);
    fclose(out);

    cout << "\nFrame Count>: " << Framecount << "\n";

    return 0;
}
int WriteIndividualFramesOut(int argc, char *argv[])
{
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  IVF2RawFrames\n\n"
                "    <Input File>\n"
                "    <OutPutDir>\n"
                "    <WriteOutAllFrames-1=yes|0=no>\n"
               );
        return 0;
    }

    char *inputFile = argv[2];
    char *outputDir = argv[3];
    int WriteIndFrames = atoi(argv[4]);

    FILE *in = fopen(inputFile, "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame = 1;
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
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.fourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
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
        vpxt_make_dir_vpx(OutputDirStrwithQuotes);
    }

    char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

    string outputDirStr2 = outputDir;
    char outputDirChar2[255];

    if (WriteIndFrames != 5)
    {
        outputDirStr2.append(slashCharStr());

        char InputFileName[255];
        vpxt_file_name(inputFile, InputFileName, 0);
        string InputFileNameStr = InputFileName;

        if (InputFileNameStr.substr(InputFileNameStr.length() - 4, 1).compare(".") == 0) //if file extension is present remove it
            outputDirStr2.append(InputFileNameStr.substr(0, InputFileNameStr.length() - 4));
        else
            outputDirStr2.append(InputFileName);

        //outputDirStr2.append("AllFrames.raw");
        outputDirStr2.append(".raw");
        snprintf(outputDirChar2, 255, "%s", outputDirStr2.c_str());
    }
    else
    {
        snprintf(outputDirChar2, 255, "%s", outputDirStr2.c_str());
    }

    FILE *out2 = fopen(outputDirChar2, "wb");
    ///////////////////////////////////////////////////////////////////////////////////////
    int LastFrameDecPlaces = vpxt_decimal_places(ivfhRaw.length);

    ///////////////////////////////////////////////////////////////////////////////////////
    while (currentVideoFrame <= frameCount)
    {
        /*fprintf(stderr, "FRAME HEADER 0\n"
        "Frame Size            - %i \n"
        "Time Stamp            - %i \n"
        "\n"
        ,ivf_fhRaw.frameSize,ivf_fhRaw.timeStamp);*/

        cout << ".";
        memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        string outputDirStr = outputDir;

        char currentVideoFrameStr[10];
        char widthchar[10];
        char heightchar[10];
        vpxt_itoa_custom(currentVideoFrame, currentVideoFrameStr, 10);
        vpxt_itoa_custom(ivfhRaw.width, widthchar, 10);
        vpxt_itoa_custom(ivfhRaw.height, heightchar, 10);

        outputDirStr.append(slashCharStr());
        char InputFileName2[255];
        vpxt_file_name(inputFile, InputFileName2, 0);
        string InputFileNameStr2 = InputFileName2;

        if (InputFileNameStr2.substr(InputFileNameStr2.length() - 4, 1).compare(".") == 0) //if file extension is present remove it
            outputDirStr.append(InputFileNameStr2.substr(0, InputFileNameStr2.length() - 4));
        else
            outputDirStr.append(InputFileName2);

        outputDirStr.append("-");
        outputDirStr.append(widthchar);
        outputDirStr.append("x");
        outputDirStr.append(heightchar);
        outputDirStr.append("-Frame-");
        ///////////////////////////////////////////////////////////////////////////////////////
        int CurNumDecPlaces = vpxt_decimal_places(currentVideoFrame);

        while (CurNumDecPlaces < LastFrameDecPlaces) //add zeros for increasing frames
        {
            //printf("%i < %i ", CurNumDecPlaces,InputDecPlaces);
            outputDirStr.append("0");
            CurNumDecPlaces++;
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        outputDirStr.append(currentVideoFrameStr);
        outputDirStr.append(".raw");

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

    return 0;
}
int IVF2Raw(char *inputFile, char *outputDir)
{
    int WriteIndFrames = 5;

    FILE *in = fopen(inputFile, "rb");
    ///////////////////////////////////

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
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
    ,ivfhRaw.version,ivfhRaw.headersize,ivfhRaw.fourCC,ivfhRaw.width,ivfhRaw.height,ivfhRaw.rate
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
        vpxt_make_dir_vpx(OutputDirStrwithQuotes);
    }

    char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 2];


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
        memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 2);
        //memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        string outputDirStr = outputDir;
        char currentVideoFrameStr[10];
        vpxt_itoa_custom(currentVideoFrame, currentVideoFrameStr, 10);
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
int Raw2IVF(int argc, char *argv[])
{
    if (argc < 8)
    {
        tprintf(PRINT_STD,
                "\n  Raw2IVF\n\n"
                "    <Input File>\n"
                "    <OutPutDir>\n"
                "    <Width>\n"
                "    <Height>\n"
                "    <FrameRate>\n"
                "    <FourCC>\n"
               );
        return 0;
    }

    char *inputFile = argv[2];
    char *outputDir = argv[3];
    int Width = atoi(argv[4]);
    int Height = atoi(argv[5]);
    int FrameRate = atoi(argv[6]);
    string FourCC = argv[7];

    FILE *in = fopen(inputFile, "rb");
    FILE *out2 = fopen(outputDir, "wb");
    ///////////////////////////////////

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    int currentVideoFrame = 0;
    int frameCount = 0;
    int byteRec = 0;

    IVF_HEADER ivfhRaw;
    InitIVFHeader(&ivfhRaw);

    if (FourCC.compare("I420") == 0)
    {
        ivfhRaw.four_cc    = MAKEFOURCC('I', '4', '2', '0');
    }

    if (FourCC.compare("YV12") == 0)
    {
        ivfhRaw.four_cc    = MAKEFOURCC('Y', 'V', '1', '2');

    }

    ivfhRaw.headersize  = 32;
    ivfhRaw.height      = Height;
    ivfhRaw.length      = 0;
    ivfhRaw.rate        = FrameRate;
    ivfhRaw.scale       = 1;
    ivfhRaw.version     = 0;
    ivfhRaw.width       = Width;

    vpxt_format_ivf_header_write(ivfhRaw);
    fwrite(&ivfhRaw, 1, 32, out2);

    tprintf(PRINT_STD, "IVF DataRate\n\n"
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
            , ivfhRaw.signature[0], ivfhRaw.signature[1], ivfhRaw.signature[2], ivfhRaw.signature[3]
            , ivfhRaw.version, ivfhRaw.headersize, ivfhRaw.four_cc, ivfhRaw.width, ivfhRaw.height, ivfhRaw.rate
            , ivfhRaw.scale, ivfhRaw.length, ivfhRaw.unused);

    IVF_FRAME_HEADER ivf_fhRaw;
    ivf_fhRaw.frameSize = ivfhRaw.width * ivfhRaw.height * 3 / 2;
    ivf_fhRaw.timeStamp = 0;


    long nSamples = frameCount;
    long lRateNum = ivfhRaw.rate;
    long lRateDenom = ivfhRaw.scale;

    long nSamplesPerBlock = 1;

    long nBytes = 0;
    long nBytesMin = 999999;
    long nBytesMax = 0;

    cout << "\n Convereting to IVF.\n";

    char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];
    int CharCount = 0;

    while (!feof(in))
    {
        if (CharCount == 79)
        {
            cout << "\n";
            CharCount = 0;
        }

        cout << ".";
        memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        if (feof(in))
            break;

        vpxt_format_frame_header_write(ivf_fhRaw);
        fwrite(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), out2);
        fwrite(inbuff, 1, ivf_fhRaw.frameSize, out2);
        currentVideoFrame ++;
    }

    ivfhRaw.length = currentVideoFrame;
    fseek(out2 , 0 , SEEK_SET);
    vpxt_format_ivf_header_write(ivfhRaw);
    fwrite(&ivfhRaw, 1, 32, out2);

    fclose(in);
    fclose(out2);

    return 0;
}
int CutIVFTool(int argc, char *argv[])
{
    if (argc < 5)
    {
        tprintf(PRINT_STD,
                "\n  CutIVF\n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <Starting Frame>\n"
                "    <Ending Frame>\n"
               );
        return 0;
    }

    char *inputFile = argv[2];
    char *outputFile = argv[3];
    int StartingFrame = atoi(argv[4]);
    int EndingFrame = atoi(argv[5]);

    vpxt_cut_ivf(inputFile, outputFile, StartingFrame, EndingFrame);

    return 0;
}
int CropRawIVFTool(int argc, char *argv[])
{
    if (argc < 8)
    {
        tprintf(PRINT_STD,
                "\n  CropRawIVF\n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <xoffset>\n"
                "    <yoffset>\n"
                "    <New Frame Width>\n"
                "    <New Frame Height>\n"
                "    <Raw/IVF 0-Raw 1-IVF>\n"
               );
        return 0;
    }

    char *inputFile = argv[2];
    char *outputFile = argv[3];
    int xoffset = atoi(argv[4]);
    int yoffset = atoi(argv[5]);
    int newFrameWidth = atoi(argv[6]);
    int newFrameHeight = atoi(argv[7]);
    int FileIsIVF  = atoi(argv[8]);

    vpxt_crop_raw_ivf(inputFile, outputFile, xoffset, yoffset, newFrameWidth, newFrameHeight, FileIsIVF, 0);
    return 0;
}
int vpxt_paste_ivf(int argc, char *argv[])
{
    if (argc < 5)
    {
        tprintf(PRINT_STD,
                "\n  PasteIVF\n\n"
                "    <Inputfile1>\n"
                "    <Inputfile2>\n"
                "    <Outputfile>\n"
                "    <First Paste Frame>\n"
               );
        return 0;
    }

    char *inputFile1 = argv[2];
    char *inputFile2 = argv[3];
    char *outputFile = argv[4];
    int StartingFrame = atoi(argv[5]);
    //int EndingFrame = atoi(argv[5]);

    vpxt_paste_ivf(inputFile1, inputFile2, outputFile, StartingFrame);

    return 0;
}
int CombineIndvFrames(int argc, char *argv[])
{
    if (argc != 8)
    {
        tprintf(PRINT_STD,
                "\n  MakeRawFromIndvFrames\n\n"
                "    <Input Director>\n"
                "    <Namebase>\n"
                "    <File Extension - include .>\n"
                "    <First Frame - Include preceding zeros>\n"
                "    <Last  Frame - Include preceding zeros>\n"
                "    <Output File>\n"
               );
        return 0;
    }

    string inputDir = argv[2];
    string namebase = argv[3];
    string extension = argv[4];
    int FirstFrame = atoi(argv[5]);
    int LastFrame = atoi(argv[6]);
    string outputfile = argv[7];


    int x = 0;
    int StaticZeroCount = 0;

    while (argv[6][x] != NULL)
    {
        char ZeroCheck[2];
        ZeroCheck[0] = argv[6][x];
        ZeroCheck[1] = '\0';

        //cout << "\n" << argv[6][x] << "\n";
        if (atoi(ZeroCheck) == 0)
        {
            StaticZeroCount++;
            //cout << "zero++";
        }
        else
        {
            break;
        }

        x++;
    }

    string CurIndividualFrameFileName = "";
    int InputDecPlaces = vpxt_decimal_places(LastFrame);//find out how many dec places due to increasing frames
    int CurrentFrame = FirstFrame;

    FILE *out = fopen(outputfile.c_str(), "wb");

    if (out == NULL)
    {
        tprintf(PRINT_STD, "\nOutput file does not exist");
        return 0;
    }

    cout << "\n\n";

    while (CurrentFrame <= LastFrame)
    {
        //printf("%i ",CurrentFrame);
        CurIndividualFrameFileName = inputDir;
        CurIndividualFrameFileName.append(slashCharStr());
        CurIndividualFrameFileName.append(namebase);

        int AddedStaticZeros = 0;

        while (AddedStaticZeros < StaticZeroCount) //add static zeros
        {
            CurIndividualFrameFileName.append("0");
            AddedStaticZeros++;
        }

        int CurNumDecPlaces = vpxt_decimal_places(CurrentFrame);

        while (CurNumDecPlaces < InputDecPlaces) //add zeros for increasing frames
        {
            //printf("%i < %i ", CurNumDecPlaces,InputDecPlaces);
            CurIndividualFrameFileName.append("0");
            CurNumDecPlaces++;
        }

        char CurrentFrameChar[512];
        vpxt_itoa_custom(CurrentFrame, CurrentFrameChar, 10);
        CurIndividualFrameFileName.append(CurrentFrameChar);
        CurIndividualFrameFileName.append(extension);



        char CurIndividualFrameFileNameChar[255];
        char CurIndividualFrameFileNameOnly[255];
        snprintf(CurIndividualFrameFileNameChar, 255, "%s", CurIndividualFrameFileName.c_str());
        vpxt_file_name(CurIndividualFrameFileNameChar, CurIndividualFrameFileNameOnly, 0);

        int fileSize = vpxt_file_size(CurIndividualFrameFileNameChar, 0);

        FILE *in = fopen(CurIndividualFrameFileName.c_str(), "rb");

        if (in != NULL && fileSize > 0)
        {

            tprintf(PRINT_STD, "%s\n", CurIndividualFrameFileNameOnly);
            char *inputBuffer = new char[fileSize*2];
            fread(inputBuffer, 1, fileSize, in);
            fwrite(inputBuffer, 1, fileSize, out);
            delete [] inputBuffer;
            fclose(in);
        }
        else
        {
            tprintf(PRINT_STD, "%s - Skiped\n", CurIndividualFrameFileNameOnly);
        }

        CurrentFrame++;
    }

    fclose(out);
    return 0;
}
int Playvpxt_compare_ivf(int argc, char *argv[])
{
    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  PlayCompIVF\n\n"
                "    <Input File>\n"
               );
        return 0;
    }

#if defined(__APPLE__)
    {
        //printf("\nError - Not Yet Implemented for Mac.\n");
        //return 0;
    }
#endif
#if defined(__POWERPC__)
    {
        //printf("\nError - Not Yet Implemented for Mac.\n");
        //return 0;
    }
#endif
#if defined(linux)
    {
        //if(!vpxt_file_exists_check("/usr/bin/mplayer"))
        //{
        // tprintf(PRINT_STD, "\nError - c:\\bin\\tmnplay.exe not found.\n");
        //  return 0;
        //}
    }
#endif
#if defined(_WIN32)
    {
        if (!vpxt_file_exists_check("c:\\bin\\tmnplay.exe"))
        {
            tprintf(PRINT_STD, "\nError - c:\\bin\\tmnplay.exe not found.\n");
            return 0;
        }
    }
#endif

    char *input = argv[2];

    string inputStr = input;
    inputStr.append("_DEC.ivf");

    char output[255];
    snprintf(output, 255, "%s", inputStr.c_str());

    cout << "\n";

    //vpxt_decompress_ivf_to_ivf_no_output(input,output);



    char output2[256];
    char *dummyargv1[6];
    inputStr.append(".raw");
    snprintf(output2, 255, "%s", inputStr.c_str());

    tprintf(PRINT_STD, "\nAPI - Decompressing VP8 IVF File to Raw File: \n");
    vpxt_decompress_ivf_to_raw_no_error_output(input, output2);

    char FiveChar[256];
    vpxt_itoa_custom(5, FiveChar, 10);

    dummyargv1[2] = output;
    dummyargv1[3] = output2;
    dummyargv1[4] = FiveChar;

    //cout << "\n\nConverting to IVF.";
    //WriteIndividualFramesOut(4,dummyargv1);

    /////////////////////Read In Data From IVF File/////////////////////
    FILE *in = fopen(input, "rb");

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);

    char WidthChar[256];
    char HeightChar[256];
    char FrameRateChar[256];
    char YUVChar[256];

    int Width = ivfhRaw.width;
    int Height = ivfhRaw.height;
    int FrameRate = (ivfhRaw.rate / ivfhRaw.scale);
    int YUV = 0;

    vpxt_itoa_custom(Width, WidthChar, 10);
    vpxt_itoa_custom(Height, HeightChar, 10);
    vpxt_itoa_custom(FrameRate, FrameRateChar, 10);
    vpxt_itoa_custom(YUV, YUVChar, 10);
    fclose(in);
    ////////////////////////////////////////////////////////////////////

    string Program;

#if defined(_WIN32)
    Program = "\"c:\\bin\\tmnplay.exe";
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" ");
    Program.append(WidthChar);
    Program.append(" ");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(FrameRateChar);
    Program.append(" \"");
    Program.append(YUVChar);
#elif defined(linux)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program.append(WidthChar);
    Program.append(":h=");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" -loop 1000");
#elif defined(__APPLE__)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program.append(WidthChar);
    Program.append(":h=");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" -loop 1000");
#elif defined(__POWERPC__)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program.append(WidthChar);
    Program.append(":h=");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" -loop 1000");
#endif

    cout << "\n\n";

    int SysRet = system(Program.c_str());

    if (!SysRet == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    Make sure tmnplay is located in your c:\\bin directory.\n"
                "    tmnplay can be found in the TestClips directory.\n"
               );
#else
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    make sure that mplayer is installed correctly.  mplayer\n"
                "    can be installed by typing:\n"
                "\n"
                "    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
                "    cd mplayer\n"
                "    ./configure\n"
                "    make\n"
                "    make install\n"
                "\n"
                "    from a command Prompt."
               );
#endif
    }


    if (remove(output2) != 0)
        cout << "\nError deleting file: " << output2 << "\n";
    else
        cout << "\n\nFile successfully deleted: " << output2 << "\n";;

    return 0;

}
int PlayDecIVF(int argc, char *argv[])
{
    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  PlayDecIVF\n\n"
                "    <Input File>\n"
               );
        return 0;
    }

#if defined(linux)
    {
        if (!vpxt_file_exists_check("/usr/bin/mplayer"))
        {
            //printf("\nError - /usr/bin/mplayer not found.\n");
            //return 0;
        }
    }
#endif
#if defined(__APPLE__)
    {
        //printf("\nError - Not Yet Implemented for Mac\n");
        //return 0;
    }
#endif
#if defined(__POWERPC__)
    {
        //printf("\nError - Not Yet Implemented for Mac\n");
        //return 0;
    }
#endif
#if defined(_WIN32)
    {
        if (!vpxt_file_exists_check("c:\\bin\\tmnplay.exe"))
        {
            tprintf(PRINT_STD, "\nError - c:\\bin\\tmnplay.exe not found.\n");
            return 0;
        }
    }
#endif


    char *input = argv[2];

    string inputStr = input;

    char output2[255];
    char *dummyargv1[6];
    inputStr.append(".raw");
    snprintf(output2, 255, "%s", inputStr.c_str());

    char FiveChar[256];
    vpxt_itoa_custom(5, FiveChar, 10);

    dummyargv1[2] = input;
    dummyargv1[3] = output2;
    dummyargv1[4] = FiveChar;

    cout << "\n\nConvereting to Raw.";
    WriteIndividualFramesOut(4, dummyargv1);

    /////////////////////Read In Data From IVF File/////////////////////
    FILE *in = fopen(input, "rb");

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    IVF_HEADER ivfhRaw;

    InitIVFHeader(&ivfhRaw);
    fread(&ivfhRaw, 1, sizeof(ivfhRaw), in);

    char WidthChar[256];
    char HeightChar[256];
    char FrameRateChar[256];
    char YUVChar[256];

    int Width = ivfhRaw.width;
    int Height = ivfhRaw.height;
    int FrameRate = (ivfhRaw.rate / ivfhRaw.scale);
    int YUV = 1;

    if (ivfhRaw.four_cc == 808596553)//I420
    {
        YUV = 0;
    }

    vpxt_itoa_custom(Width, WidthChar, 10);
    vpxt_itoa_custom(Height, HeightChar, 10);
    vpxt_itoa_custom(FrameRate, FrameRateChar, 10);
    vpxt_itoa_custom(YUV, YUVChar, 10);
    fclose(in);
    ////////////////////////////////////////////////////////////////////

    char output3[256];
    inputStr.append("\\AllFrames.raw");
    snprintf(output3, 255, "%s", inputStr.c_str());

    char *dummyargv2[6];
    dummyargv2[1] = output3;
    dummyargv2[2] = WidthChar;
    dummyargv2[3] = HeightChar;
    dummyargv2[4] = FrameRateChar;
    dummyargv2[5] = YUVChar;

    string Program;

#if defined(_WIN32)
    Program = "\"c:\\bin\\tmnplay.exe";
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" ");
    Program.append(WidthChar);
    Program.append(" ");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(FrameRateChar);
    Program.append(" \"");
    Program.append(YUVChar);
#elif defined(linux)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program.append(WidthChar);
    Program.append(":h=");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" -loop 1000");
#elif defined(__APPLE__)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program.append(WidthChar);
    Program.append(":h=");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" -loop 1000");
#elif defined(__POWERPC__)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program.append(WidthChar);
    Program.append(":h=");
    Program.append(HeightChar);
    Program.append(" ");
    Program.append(" \"");
    Program.append(output2);
    Program.append("\"");
    Program.append(" -loop 1000");
#endif

    cout << "\n\n";

    int SysRet = system(Program.c_str());

    if (!SysRet == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    Make sure tmnplay is located in your c:\\bin directory.\n"
                "    tmnplay can be found in the TestClips directory.\n"
               );
#else
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    make sure that mplayer is installed correctly.  mplayer\n"
                "    can be installed by typing:\n"
                "\n"
                "    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
                "    cd mplayer\n"
                "    ./configure\n"
                "    make\n"
                "    make install\n"
                "\n"
                "    from a command Prompt."
               );
#endif
    }

    if (remove(output2) != 0)
        cout << "\nError deleting file: " << output2 << "\n";
    else
        cout << "\n\nFile successfully deleted: " << output2 << "\n";;

    return 0;
}
int DecoderCheck(int argc, char *argv[])
{
    char ExeLoc[255];
    vpxt_folder_name(argv[0], ExeLoc);

    string IVFSourceFile = ExeLoc;
    IVFSourceFile.append("DecTestMaterial\\");

    string DecodeInputStr = IVFSourceFile;
    DecodeInputStr.append("I420_bondA_FileToDec.ivf");

    ///////////////////////////////////////////////
    string BeforeCompressionDecOutput1 = IVFSourceFile;
    string BeforeCompressionDecOutput2 = IVFSourceFile;
    string BeforeCompressionDecOutput3 = IVFSourceFile;

    BeforeCompressionDecOutput1.append("BeforeCompressionOut");
    BeforeCompressionDecOutput2.append("BeforeCompressionOut");
    BeforeCompressionDecOutput3.append("BeforeCompressionOut");

    string MakeDirBeforeCompressionDecOutput1 = BeforeCompressionDecOutput1;
    MakeDirBeforeCompressionDecOutput1.insert(0, "\"");
    MakeDirBeforeCompressionDecOutput1.insert(0, "md ");
    MakeDirBeforeCompressionDecOutput1.insert(0, "\"");
    MakeDirBeforeCompressionDecOutput1.append("\"");
    vpxt_make_dir_vpx(MakeDirBeforeCompressionDecOutput1);

    BeforeCompressionDecOutput1.append("\\BeforeCompressionDecOutput1.ivf");
    BeforeCompressionDecOutput2.append("\\BeforeCompressionDecOutput2.ivf");
    BeforeCompressionDecOutput3.append("\\BeforeCompressionDecOutput3.ivf");
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    string AfterCompressionDecOutput1 = IVFSourceFile;
    string AfterCompressionDecOutput2 = IVFSourceFile;
    string AfterCompressionDecOutput3 = IVFSourceFile;

    AfterCompressionDecOutput1.append("AfterCompressionOut");
    AfterCompressionDecOutput2.append("AfterCompressionOut");
    AfterCompressionDecOutput3.append("AfterCompressionOut");

    string MakeDirAfterCompressionDecOutput1 = AfterCompressionDecOutput1;
    MakeDirAfterCompressionDecOutput1.insert(0, "\"");
    MakeDirAfterCompressionDecOutput1.insert(0, "md ");
    MakeDirAfterCompressionDecOutput1.insert(0, "\"");
    MakeDirAfterCompressionDecOutput1.append("\"");
    vpxt_make_dir_vpx(MakeDirAfterCompressionDecOutput1);

    AfterCompressionDecOutput1.append("\\AfterCompressionDecOutput1.ivf");
    AfterCompressionDecOutput2.append("\\AfterCompressionDecOutput2.ivf");
    AfterCompressionDecOutput3.append("\\AfterCompressionDecOutput3.ivf");
    ///////////////////////////////////////////////
    string CompressionInputStr = IVFSourceFile;
    string CompressionOutputStr = IVFSourceFile;

    CompressionInputStr.append("src16_FileToCompress.ivf");
    CompressionOutputStr.append("NewCompressionOut");

    string MakeDirCompressionOutputStr = CompressionOutputStr;
    MakeDirCompressionOutputStr.insert(0, "\"");
    MakeDirCompressionOutputStr.insert(0, "md ");
    MakeDirCompressionOutputStr.insert(0, "\"");
    MakeDirCompressionOutputStr.append("\"");
    vpxt_make_dir_vpx(MakeDirCompressionOutputStr);

    CompressionOutputStr.append("\\NewCompression.ivf");
    ///////////////////////////////////////////////

    char BeforeCompressionDecOutput1Char[255];
    char BeforeCompressionDecOutput2Char[255];
    char BeforeCompressionDecOutput3Char[255];

    char AfterCompressionDecOutput1Char[255];
    char AfterCompressionDecOutput2Char[255];
    char AfterCompressionDecOutput3Char[255];

    char DecodeInput[255];

    char CompressionInput[255];
    char CompressionOutput[255];

    snprintf(BeforeCompressionDecOutput1Char, 255, "%s", BeforeCompressionDecOutput1.c_str());
    snprintf(BeforeCompressionDecOutput2Char, 255, "%s", BeforeCompressionDecOutput2.c_str());
    snprintf(BeforeCompressionDecOutput3Char, 255, "%s", BeforeCompressionDecOutput3.c_str());

    snprintf(AfterCompressionDecOutput1Char, 255, "%s", AfterCompressionDecOutput1.c_str());
    snprintf(AfterCompressionDecOutput2Char, 255, "%s", AfterCompressionDecOutput2.c_str());
    snprintf(AfterCompressionDecOutput3Char, 255, "%s", AfterCompressionDecOutput3.c_str());

    snprintf(DecodeInput, 255, "%s", DecodeInputStr.c_str());

    snprintf(CompressionInput, 255, "%s", CompressionInputStr.c_str());
    snprintf(CompressionOutput, 255, "%s", CompressionOutputStr.c_str());

    tprintf(PRINT_STD, "\n\nDecompressing %s to %s\n", DecodeInput, BeforeCompressionDecOutput1Char);
    unsigned int CPUTick1 = 0;
    vpxt_decompress_ivf_to_ivf_time_and_output(DecodeInput, BeforeCompressionDecOutput1Char, CPUTick1);
    tprintf(PRINT_STD, "\n\nDecompressing %s to %s\n", DecodeInput, BeforeCompressionDecOutput2Char);
    unsigned int CPUTick2 = 0;
    vpxt_decompress_ivf_to_ivf_time_and_output(DecodeInput, BeforeCompressionDecOutput2Char, CPUTick2);
    tprintf(PRINT_STD, "\n\nDecompressing %s to %s\n", DecodeInput, BeforeCompressionDecOutput3Char);
    unsigned int CPUTick3 = 0;
    vpxt_decompress_ivf_to_ivf_time_and_output(DecodeInput, BeforeCompressionDecOutput3Char, CPUTick3);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int speed = 0;
    int BitRate = 128;

    int Mode = 1;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;
        tprintf(PRINT_STD, "\n\nCompressing %s to %s\n", CompressionInput, CompressionOutput);
        vpxt_compress_ivf_to_ivf_no_error_output(CompressionInput, CompressionOutput, speed, BitRate, opt, "", 0, 0, 0, 3, 3);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    tprintf(PRINT_STD, "\n\nDecompressing %s to %s\n", DecodeInput, AfterCompressionDecOutput1Char);
    unsigned int CPUTick4 = 0;
    vpxt_decompress_ivf_to_ivf_time_and_output(DecodeInput, AfterCompressionDecOutput1Char, CPUTick4);
    tprintf(PRINT_STD, "\n\nDecompressing %s to %s\n", DecodeInput, AfterCompressionDecOutput2Char);
    unsigned int CPUTick5 = 0;
    vpxt_decompress_ivf_to_ivf_time_and_output(DecodeInput, AfterCompressionDecOutput2Char, CPUTick5);
    tprintf(PRINT_STD, "\n\nDecompressing %s to %s\n", DecodeInput, AfterCompressionDecOutput3Char);
    unsigned int CPUTick6 = 0;
    vpxt_decompress_ivf_to_ivf_time_and_output(DecodeInput, AfterCompressionDecOutput3Char, CPUTick6);

    cout << "\n\n";

    if (vpxt_compare_ivf(BeforeCompressionDecOutput1Char, BeforeCompressionDecOutput2Char) == -1)
    {
        cout << "\nBefore Compression Files 1 and 2 are identical";
    }
    else
    {
        cout << "\nBefore Compression Files 1 and 2 are not identical: ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(BeforeCompressionDecOutput1Char, BeforeCompressionDecOutput2Char);
    }

    if (vpxt_compare_ivf(BeforeCompressionDecOutput1Char, BeforeCompressionDecOutput3Char) == -1)
    {
        cout << "\nBefore Compression Files 1 and 3 are identical";
    }
    else
    {
        cout << "\nBefore Compression Files 1 and 3 are not identical: ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(BeforeCompressionDecOutput1Char, BeforeCompressionDecOutput3Char);
    }

    if (vpxt_compare_ivf(BeforeCompressionDecOutput2Char, BeforeCompressionDecOutput3Char) == -1)
    {
        cout << "\nBefore Compression Files 2 and 3 are identical";
    }
    else
    {
        cout << "\nBefore Compression Files 2 and 3 are not identical: ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(BeforeCompressionDecOutput2Char, BeforeCompressionDecOutput3Char);
    }

    //////////////////////////After//////////////////////////

    if (vpxt_compare_ivf(AfterCompressionDecOutput1Char, AfterCompressionDecOutput2Char) == -1)
    {
        cout << "\nAfter Compression Files 1 and 2 are identical";
    }
    else
    {
        cout << "\nAfter Compression Files 1 and 2 are not identical: ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(AfterCompressionDecOutput1Char, AfterCompressionDecOutput2Char);
    }

    if (vpxt_compare_ivf(AfterCompressionDecOutput1Char, AfterCompressionDecOutput3Char) == -1)
    {
        cout << "\nAfter Compression Files 1 and 3 are identical";
    }
    else
    {
        cout << "\nAfter Compression Files 1 and 3 are not identical: ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(AfterCompressionDecOutput1Char, AfterCompressionDecOutput3Char);
    }

    if (vpxt_compare_ivf(AfterCompressionDecOutput2Char, AfterCompressionDecOutput3Char) == -1)
    {
        cout << "\nAfter Compression Files 2 and 3 are identical ";
    }
    else
    {
        cout << "\nAfter Compression Files 2 and 3 are not identical: ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(AfterCompressionDecOutput2Char, AfterCompressionDecOutput3Char);
    }


    if (vpxt_compare_ivf(BeforeCompressionDecOutput1Char, AfterCompressionDecOutput1Char) == -1)
    {
        cout << "\nBefore Compression File 1 and After Compression File 1 are identical ";
    }
    else
    {
        cout << "\nBefore Compression File 1 and After Compression File 1 are not identical : ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(BeforeCompressionDecOutput1Char, AfterCompressionDecOutput1Char);
    }

    if (vpxt_compare_ivf(BeforeCompressionDecOutput2Char, AfterCompressionDecOutput2Char) == -1)
    {
        cout << "\nBefore Compression File 2 and After Compression File 2 are identical ";
    }
    else
    {
        cout << "\nBefore Compression File 2 and After Compression File 2 are not identical : ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(BeforeCompressionDecOutput2Char, AfterCompressionDecOutput2Char);
    }

    if (vpxt_compare_ivf(BeforeCompressionDecOutput3Char, AfterCompressionDecOutput3Char) == -1)
    {
        cout << "\nBefore Compression File 3 and After Compression File 3 are identical ";
    }
    else
    {
        cout << "\nBefore Compression File 3 and After Compression File 3 are not identical : ";
        cout << "Files Differ at Frame: " << vpxt_compare_ivf(BeforeCompressionDecOutput3Char, AfterCompressionDecOutput3Char);
    }

    return 0;
}
int IVFParseandDelete(char *DirName)
{

#if defined(linux)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    string InputStr = DirName;
    char GetFirstFileChar[255];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());
    //strcpy(GetFirstFileChar,InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;
    vector<string> SourceFileVectorA;

    FindFileDataA = opendir(DirName);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = DirName;
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                //strcpy(FullPathChar,FullPathName.c_str());
                IVFParseandDelete(FullPathChar);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = DirName;
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(outputString.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                    }
                }
            }


        }
    }

    return 0;

#endif

#if defined(_WIN32)
    string DirNameStr = DirName;
    DirNameStr.append("\\*");

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    string doubledot = "..";
    string singledot = ".";

    hFind = FindFirstFile(DirNameStr.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        tprintf(PRINT_STD, "FindFirstFile failed (%d)\n", GetLastError());
        return 0;
    }
    else
    {
        if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
        {
            string FullPathName = DirName;
            FullPathName.append("\\");
            FullPathName.append(FindFileData.cFileName);
            char FullPathChar[255];
            snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
            IVFParseandDelete(FullPathChar);
        }
        else
        {
            if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string outputString = DirName;
                outputString.append("\\");
                outputString.append(FindFileData.cFileName);

                string FileName = FindFileData.cFileName;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(outputString.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                    }
                }
            }
        }

        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string FullPathName = DirName;
                FullPathName.append("\\");
                FullPathName.append(FindFileData.cFileName);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                IVFParseandDelete(FullPathChar);
            }
            else
            {
                if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
                {
                    string outputString = DirName;
                    outputString.append("\\");
                    outputString.append(FindFileData.cFileName);

                    string FileName = FindFileData.cFileName;
                    string extention = FileName.substr(FileName.length() - 4, 4);

                    if (extention.compare(".ivf") == 0)
                    {
                        if (remove(outputString.c_str()) == 0)
                        {
                            tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                        }
                        else
                        {
                            tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                        }
                    }
                }
            }
        }

        FindClose(hFind);
    }

    return 0;
#endif
    return 0;
}
int DeleteAllIVFFiles(int argc, char *argv[])
{
#if defined(linux)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
               );
        return 0;
    }

    string continueSTR;
    string DoNotLetAllIVFsBeDeleted = argv[2];

    if (DoNotLetAllIVFsBeDeleted.length() == 0)
    {
        cout << "\nEmpty input directory detected function aborted\n";
        return 0;
    }

    tprintf(PRINT_STD, "\nWARNING: This will permenently remove all .ivf files from the input directory:\n %s  Do you wish to continue - (YES)(NO): ", argv[2]);
    cin >> continueSTR;

    if (continueSTR.compare("YES") != 0)
    {
        cout << "\nFunction Aborted\n";
        return 0;
    }

    cout << "\n";

    string InputStr = argv[2];
    char GetFirstFileChar[255];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());
    //strcpy(GetFirstFileChar,InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;
    vector<string> SourceFileVectorA;

    FindFileDataA = opendir(argv[2]);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                //strcpy(FullPathChar,FullPathName.c_str());
                IVFParseandDelete(FullPathChar);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = argv[2];
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(outputString.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                    }
                }
            }


        }
    }

    return 0;

#elif defined(__POWERPC__)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
               );
        return 0;
    }

    string continueSTR;
    string DoNotLetAllIVFsBeDeleted = argv[2];

    if (DoNotLetAllIVFsBeDeleted.length() == 0)
    {
        cout << "\nEmpty input directory detected function aborted\n";
        return 0;
    }

    tprintf(PRINT_STD, "\nWARNING: This will permenently remove all .ivf files from the input directory:\n %s  Do you wish to continue - (YES)(NO): ", argv[2]);
    cin >> continueSTR;

    if (continueSTR.compare("YES") != 0)
    {
        cout << "\nFunction Aborted\n";
        return 0;
    }

    cout << "\n";

    string InputStr = argv[2];
    char GetFirstFileChar[255];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;
    vector<string> SourceFileVectorA;

    FindFileDataA = opendir(argv[2]);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                IVFParseandDelete(FullPathChar);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = argv[2];
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(outputString.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                    }
                }
            }


        }
    }

    return 0;

#elif defined(__APPLE__)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
               );
        return 0;
    }

    string continueSTR;
    string DoNotLetAllIVFsBeDeleted = argv[2];

    if (DoNotLetAllIVFsBeDeleted.length() == 0)
    {
        cout << "\nEmpty input directory detected function aborted\n";
        return 0;
    }

    tprintf(PRINT_STD, "\nWARNING: This will permenently remove all .ivf files from the input directory:\n %s  Do you wish to continue - (YES)(NO): ", argv[2]);
    cin >> continueSTR;

    if (continueSTR.compare("YES") != 0)
    {
        cout << "\nFunction Aborted\n";
        return 0;
    }

    cout << "\n";

    string InputStr = argv[2];
    char GetFirstFileChar[256];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;
    vector<string> SourceFileVectorA;

    FindFileDataA = opendir(argv[2]);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[256];
                IVFParseandDelete(FullPathChar);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = argv[2];
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(outputString.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                    }
                }
            }


        }
    }

    return 0;

#elif defined(_WIN32)

    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
               );
        return 0;
    }

    string continueSTR;
    string DoNotLetAllIVFsBeDeleted = argv[2];

    if (DoNotLetAllIVFsBeDeleted.length() == 0)
    {
        cout << "\nEmpty input directory detected function aborted\n";
        return 0;
    }

    tprintf(PRINT_STD, "\nWARNING: This will permenently remove all .ivf files from the input directory:\n %s  Do you wish to continue - (YES)(NO): ", argv[2]);
    cin >> continueSTR;

    if (continueSTR.compare("YES") != 0)
    {
        cout << "\nFunction Aborted\n";
        return 0;
    }

    cout << "\n";

    string InputStr = argv[2];
    InputStr.append("\\*");
    char GetFirstFileChar[255];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    hFind = FindFirstFile(GetFirstFileChar, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        tprintf(PRINT_STD, "\nFindFirstFile failed (%d)\n", GetLastError());
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {
        if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
        {
            string FullPathName = argv[2];
            FullPathName.append("\\");
            FullPathName.append(FindFileData.cFileName);
            char FullPathChar[255];
            snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
            IVFParseandDelete(FullPathChar);
        }
        else
        {
            if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string outputString = argv[2];
                outputString.append("\\");
                outputString.append(FindFileData.cFileName);

                string FileName = FindFileData.cFileName;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(outputString.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                    }
                }
            }
        }

        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("\\");
                FullPathName.append(FindFileData.cFileName);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                IVFParseandDelete(FullPathChar);
            }
            else
            {
                if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
                {
                    string outputString = argv[2];
                    outputString.append("\\");
                    outputString.append(FindFileData.cFileName);

                    string FileName = FindFileData.cFileName;
                    string extention = FileName.substr(FileName.length() - 4, 4);

                    if (extention.compare(".ivf") == 0)
                    {
                        if (remove(outputString.c_str()) == 0)
                        {
                            tprintf(PRINT_STD, "%s Successfully Deleted\n\n", outputString.c_str());
                        }
                        else
                        {
                            tprintf(PRINT_STD, "Error: %s Not Deleted\n\n", outputString.c_str());
                        }
                    }
                }
            }
        }

        FindClose(hFind);
    }

    return 0;
#endif
    return 0;
}
int TxtParseandCopy(char *DirName, char *BaseOutputDir, int BaseInputStrLength)
{
#if defined(linux)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    string InputStr = DirName;

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;

    FindFileDataA = opendir(DirName);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = DirName;
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                TxtParseandCopy(FullPathChar, BaseOutputDir, BaseInputStrLength);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = DirName;
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = BaseOutputDir;
                    FileNamePart1.append("/");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "cp \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }


        }
    }

    return 0;

#elif defined(__POWERPC__)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    string InputStr = DirName;

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;

    FindFileDataA = opendir(DirName);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = DirName;
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                TxtParseandCopy(FullPathChar, BaseOutputDir, BaseInputStrLength);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = DirName;
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = BaseOutputDir;
                    FileNamePart1.append("/");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "cp \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }


        }
    }

    return 0;

#elif defined(__APPLE__)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    string InputStr = DirName;

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;

    FindFileDataA = opendir(DirName);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = DirName;
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                TxtParseandCopy(FullPathChar, BaseOutputDir, BaseInputStrLength);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = DirName;
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = BaseOutputDir;
                    FileNamePart1.append("/");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "cp \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }


        }
    }

    return 0;

#elif defined(_WIN32)
    string DirNameStr = DirName;
    DirNameStr.append("\\*");

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    string doubledot = "..";
    string singledot = ".";

    hFind = FindFirstFile(DirNameStr.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        tprintf(PRINT_STD, "FindFirstFile failed (%d)\n", GetLastError());
        return 0;
    }
    else
    {
        if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
        {
            string FullPathName = DirName;
            FullPathName.append("\\");
            FullPathName.append(FindFileData.cFileName);
            char FullPathChar[255];
            snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
            TxtParseandCopy(FullPathChar, BaseOutputDir, BaseInputStrLength);
        }
        else
        {
            if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string outputString = DirName;
                outputString.append("\\");
                outputString.append(FindFileData.cFileName);

                string FileName = FindFileData.cFileName;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = BaseOutputDir;
                    FileNamePart1.append("\\");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "copy \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }
        }

        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string FullPathName = DirName;
                FullPathName.append("\\");
                FullPathName.append(FindFileData.cFileName);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                TxtParseandCopy(FullPathChar, BaseOutputDir, BaseInputStrLength);
            }
            else
            {
                if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
                {
                    string outputString = DirName;
                    outputString.append("\\");
                    outputString.append(FindFileData.cFileName);

                    string FileName = FindFileData.cFileName;
                    string extention = FileName.substr(FileName.length() - 4, 4);

                    if (extention.compare(".txt") == 0)
                    {
                        //if extention matches txt
                        string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                        string FileNamePart1 = BaseOutputDir;
                        FileNamePart1.append("\\");
                        FileNamePart1.append(FileNamePart2);
                        string CopyCmdString = "copy \"";
                        CopyCmdString.append(outputString);
                        CopyCmdString.append("\" \"");
                        CopyCmdString.append(FileNamePart1);
                        CopyCmdString.append("\"");

                        string MkDirStr = "mkdir \"";
                        string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                        MkDirStr.append(DirName);
                        MkDirStr.append("\"");

                        system(MkDirStr.c_str());
                        system(CopyCmdString.c_str());

                    }
                }
            }
        }

        FindClose(hFind);
    }

    return 0;
#endif
    return 0;
}
int CopyAllTxtFiles(int argc, char *argv[])
{
#if defined(linux)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
                "    <Output Directory>\n"
               );
        return 0;
    }


    string InputStr = argv[2];
    int BaseInputStrLength = InputStr.length();
    char GetFirstFileChar[255];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());
    //strcpy(GetFirstFileChar,InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;

    FindFileDataA = opendir(argv[2]);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                //strcpy(FullPathChar,FullPathName.c_str());
                TxtParseandCopy(FullPathChar, argv[3], BaseInputStrLength);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = argv[2];
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = argv[3];
                    FileNamePart1.append("/");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "cp \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }


        }
    }

    return 0;

#elif defined(__POWERPC__)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
                "    <Output Directory>\n"
               );
        return 0;
    }


    string InputStr = argv[2];
    int BaseInputStrLength = InputStr.length();
    char GetFirstFileChar[256];
    strcpy(GetFirstFileChar, InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;

    FindFileDataA = opendir(argv[2]);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {
            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                //strcpy(FullPathChar,FullPathName.c_str());
                TxtParseandCopy(FullPathChar, argv[3], BaseInputStrLength);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = argv[2];
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = argv[3];
                    FileNamePart1.append("/");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "cp \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }


        }
    }

    return 0;

#elif defined(__APPLE__)

    unsigned char isFile = 0x8;
    unsigned char isFolder = 0x4;

    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
                "    <Output Directory>\n"
               );
        return 0;
    }


    string InputStr = argv[2];
    int BaseInputStrLength = InputStr.length();
    char GetFirstFileChar[255];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;

    FindFileDataA = opendir(argv[2]);

    if (FindFileDataA == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (hFindA = readdir(FindFileDataA))
        {

            if (hFindA->d_type == isFolder && doubledot.compare(hFindA->d_name) != 0 && singledot.compare(hFindA->d_name) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("/");
                FullPathName.append(hFindA->d_name);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                TxtParseandCopy(FullPathChar, argv[3], BaseInputStrLength);

            }

            if (hFindA->d_type == isFile)
            {
                string outputString = argv[2];
                outputString.append("/");
                outputString.append(hFindA->d_name);

                string FileName = hFindA->d_name;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = argv[3];
                    FileNamePart1.append("/");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "cp \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }


        }
    }

    return 0;

#elif defined(_WIN32)

    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input Directory>\n"
                "    <Output Directory>\n"
               );
        return 0;
    }

    cout << "\n";

    string InputStr = argv[2];
    int BaseInputStrLength = InputStr.length();
    InputStr.append("\\*");
    char GetFirstFileChar[255];
    snprintf(GetFirstFileChar, 255, "%s", InputStr.c_str());

    string doubledot = "..";
    string singledot = ".";

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    hFind = FindFirstFile(GetFirstFileChar, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        tprintf(PRINT_STD, "\nFindFirstFile failed (%d)\n", GetLastError());
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {
        if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
        {
            string FullPathName = argv[2];
            FullPathName.append("\\");
            FullPathName.append(FindFileData.cFileName);
            char FullPathChar[255];
            snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
            TxtParseandCopy(FullPathChar, argv[3], BaseInputStrLength);
        }
        else
        {
            if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string outputString = argv[2];
                outputString.append("\\");
                outputString.append(FindFileData.cFileName);

                string FileName = FindFileData.cFileName;
                string extention = FileName.substr(FileName.length() - 4, 4);

                if (extention.compare(".txt") == 0)
                {
                    //if extention matches txt
                    string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                    string FileNamePart1 = argv[3];
                    FileNamePart1.append("\\");
                    FileNamePart1.append(FileNamePart2);
                    string CopyCmdString = "copy \"";
                    CopyCmdString.append(outputString);
                    CopyCmdString.append("\" \"");
                    CopyCmdString.append(FileNamePart1);
                    CopyCmdString.append("\"");

                    string MkDirStr = "mkdir \"";
                    string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                    MkDirStr.append(DirName);
                    MkDirStr.append("\"");

                    system(MkDirStr.c_str());
                    system(CopyCmdString.c_str());
                }
            }
        }

        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
            {
                string FullPathName = argv[2];
                FullPathName.append("\\");
                FullPathName.append(FindFileData.cFileName);
                char FullPathChar[255];
                snprintf(FullPathChar, 255, "%s", FullPathName.c_str());
                TxtParseandCopy(FullPathChar, argv[3], BaseInputStrLength);
            }
            else
            {
                if (doubledot.compare(FindFileData.cFileName) != 0 && singledot.compare(FindFileData.cFileName) != 0)
                {
                    string outputString = argv[2];
                    outputString.append("\\");
                    outputString.append(FindFileData.cFileName);

                    string FileName = FindFileData.cFileName;
                    string extention = FileName.substr(FileName.length() - 4, 4);

                    if (extention.compare(".txt") == 0)
                    {
                        //if extention matches txt
                        string FileNamePart2 = outputString.substr(BaseInputStrLength + 1, outputString.length() - BaseInputStrLength - 1);
                        string FileNamePart1 = argv[3];
                        FileNamePart1.append("\\");
                        FileNamePart1.append(FileNamePart2);
                        string CopyCmdString = "copy \"";
                        CopyCmdString.append(outputString);
                        CopyCmdString.append("\" \"");
                        CopyCmdString.append(FileNamePart1);
                        CopyCmdString.append("\"");

                        string MkDirStr = "mkdir \"";
                        string DirName = FileNamePart1.substr(0, FileNamePart1.length() - FileName.length());
                        MkDirStr.append(DirName);
                        MkDirStr.append("\"");

                        system(MkDirStr.c_str());
                        system(CopyCmdString.c_str());

                    }
                }
            }
        }

        FindClose(hFind);
    }

    return 0;
#endif
    return 0;
}
int SideBySideText(int argc, char *argv[])
{
    if (argc < 5)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input File 1>\n"
                "    <Input File 2>\n"
                "    <Output File>\n"
               );
        return 0;
    }

    char InputCharFormatChar[9999];
    char InputChar1[9999];
    char InputChar2[9999];
    string InputCharFormatStr;
    string InputCharStr1;
    string InputCharStr2;
    int MaxLength = 0;

    string InputStr1 = argv[2];
    string InputStr2 = argv[3];
    string OutputStr = argv[4];

    ifstream InfileFormat(InputStr1.c_str());

    while (!InfileFormat.eof())
    {
        InfileFormat.getline(InputCharFormatChar, 9999);
        InputCharFormatStr = InputCharFormatChar;

        if (InputCharFormatStr.length() > MaxLength)
        {
            MaxLength = InputCharFormatStr.length();
        }
    }

    cout << "MaxLength: " << MaxLength << "\n";

    InfileFormat.close();
    ifstream Infile1(InputStr1.c_str());
    ifstream Infile2(InputStr2.c_str());
    fstream  Outfile;
    Outfile.open(OutputStr.c_str(), fstream::in | fstream::out | fstream::app);

    while (!(Infile1.eof() && Infile2.eof()))
    {
        if (!(Infile1.eof()))
        {
            Infile1.getline(InputChar1, 9999);
            InputCharStr1 = InputChar1;
        }
        else
        {
            InputCharStr1 = "";
        }

        if (!(Infile2.eof()))
        {
            Infile2.getline(InputChar2, 9999);
            InputCharStr2 = InputChar2;
        }
        else
        {
            InputCharStr2 = "";
        }

        Outfile << InputCharStr1;

        int n = 0;

        while (InputCharStr1.length() + n < MaxLength + 10)
        {
            Outfile << " ";
            n++;
        }

        Outfile << InputCharStr2 << "\n";

    }

    Infile1.close();
    Infile2.close();
    Outfile.close();

    return 0;

}
int CompressionEquiv(int argc, char *argv[], string WorkingDir)
{

    char *CompressString = "Allow DF";

    if (argc < 6 || argc > 7)
    {
        tprintf(PRINT_STD,
                "\n  Compress IVF to IVF \n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <Bit Rate>\n"
                "    <Mode>\n"
                "    <Optional - Parameter File>\n"
                "\n");

        return 0;
    }

    string input = argv[2];
    string output = argv[3];
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[5]);

    int speed = 0;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;

    if (argc == 7)
    {
        opt = vpxt_input_settings(argv[6]);
    }

    string output1 = output;
    string output2 = output;
    string output3 = output;

    output1.append("_CompressIVFtoIVF.ivf");
    output2.append("_CompressIVFtoIVFNoErrorOutput.ivf");
    output3.append("_TimeCompressIVFtoIVF.ivf");

    int CompressInt = opt.allow_df;

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;
        vpxt_compress_ivf_to_ivf((char *)input.c_str(), (char *)output1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        vpxt_compress_ivf_to_ivf_no_error_output((char *)input.c_str(), (char *)output2.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        unsigned int CPUTick = 0;
        vpxt_time_compress_ivf_to_ivf((char *)input.c_str(), (char *)output3.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
    }

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;

        vpxt_compress_ivf_to_ivf((char *)input.c_str(), (char *)output1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        vpxt_compress_ivf_to_ivf_no_error_output((char *)input.c_str(), (char *)output2.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        unsigned int CPUTick = 0;
        vpxt_time_compress_ivf_to_ivf((char *)input.c_str(), (char *) output3.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);

    }

    if (Mode == 2)
    {
        opt.Mode = MODE_BESTQUALITY;

        vpxt_compress_ivf_to_ivf((char *)input.c_str(), (char *) output1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        vpxt_compress_ivf_to_ivf_no_error_output((char *)input.c_str(), (char *) output2.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
        unsigned int CPUTick = 0;
        vpxt_time_compress_ivf_to_ivf((char *)input.c_str(), (char *) output3.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
    }

    if (Mode == 3)
    {

    }

    if (Mode == 4)
    {
        opt.Mode = MODE_SECONDPASS;
        vpxt_compress_ivf_to_ivf((char *)input.c_str(), (char *) output1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);

        opt.Mode = MODE_SECONDPASS;
        vpxt_compress_ivf_to_ivf_no_error_output((char *)input.c_str(), (char *) output2.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);

        opt.Mode = MODE_SECONDPASS;
        unsigned int CPUTick = 0;
        vpxt_time_compress_ivf_to_ivf((char *)input.c_str(), (char *) output3.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
    }

    if (Mode == 5)
    {
        opt.Mode = MODE_SECONDPASS_BEST;
        vpxt_compress_ivf_to_ivf((char *)input.c_str(), (char *) output1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);

        opt.Mode = MODE_SECONDPASS_BEST;
        vpxt_compress_ivf_to_ivf_no_error_output((char *)input.c_str(), (char *) output2.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);

        opt.Mode = MODE_SECONDPASS_BEST;
        unsigned int CPUTick = 0;
        vpxt_time_compress_ivf_to_ivf((char *)input.c_str(), (char *) output3.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, CPUTick, 0, 3, 3);
    }

    if (vpxt_compare_ivf((char *)output1.c_str(), (char *)output2.c_str()) == -1)
    {
        cout << "Pass - No Error Output and standard Do match \n";

        if (vpxt_compare_ivf((char *)output2.c_str(), (char *)output3.c_str()) == -1)
        {
            cout << "Pass - Time Compress and No Error Output match\n\n";
            cout << "\nAll compressions are equal. - Pass\n\n";
        }
        else
        {
            cout << "Fail - Time Compress Does not match No Error Output\n";
            return 0;
        }
    }
    else
    {
        cout << "Fail - No Error Output and standard Do not match \n";
        return 0;
    }

    string output1DEC = output;
    string output2DEC = output;
    string output3DEC = output;
    string output4DEC = output;
    string output5DEC = output;

    output1DEC.append("_DecompressIVFtoIVF.ivf");
    output2DEC.append("_DecompressIVFtoIVFNoOutput.ivf");
    output3DEC.append("_TimeDecompressIVFtoIVF.ivf");
    output4DEC.append("_DecompressIVFtoIVFTimeAndOutput.ivf");

    cout << "DecompressIVFtoIVF\n";
    vpxt_decompress_ivf_to_ivf((char *)output1.c_str(), (char *) output1DEC.c_str());
    cout << "\nDecompressIVFtoIVFNoOutput\n";
    vpxt_decompress_ivf_to_ivf_no_output((char *)output1.c_str(), (char *) output2DEC.c_str());
    cout << "\nTimeDecompressIVFtoIVF\n";
    unsigned int CPUTick1 = 0;
    vpxt_time_decompress_ivf_to_ivf((char *)output1.c_str(), (char *) output3DEC.c_str(), CPUTick1);
    cout << "\nDecompressIVFtoIVFTimeAndOutput\n";
    unsigned int CPUTick2 = 0;
    vpxt_decompress_ivf_to_ivf_time_and_output((char *)output1.c_str(), (char *)output4DEC.c_str(), CPUTick2);
    cout << "\n\n";

    if (vpxt_compare_ivf((char *) output1DEC.c_str(), (char *) output2DEC.c_str()) == -1)
    {
        cout << "Pass DecompressIVFtoIVF == DecompressIVFtoIVFNoOutput\n";
    }
    else
    {
        cout << "Fail DecompressIVFtoIVF != DecompressIVFtoIVFNoOutput\n";
        return 0;
    }

    return 0;
}
int ConvertParmFileToIVFenc(char *input, char *output)
{

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt = vpxt_input_settings(input);
    vpxt_output_settings_ivfenc(output, opt);

    return 0;
}
int CompMatchesIVFenc(int argc, char *argv[])
{
    char *CompressString = "Allow DF";

    if (argc < 6 || argc > 7)
    {
        tprintf(PRINT_STD,
                "\n  Compress IVF to IVF \n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <Bit Rate>\n"
                "    <Mode>\n"
                "    <Optional - Parameter File>\n"
                "\n");

        return 0;
    }

    char *input = argv[2];
    char *output = argv[3];
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[5]);

    char FileNameChar[256];
    vpxt_file_name(input, FileNameChar, 0);

    /////////////////////Tester Par File//////////////////
    string OutputsettingsFile = output;
    OutputsettingsFile.erase(OutputsettingsFile.length() - 4, 4);
    string OutputsettingsFile2 = OutputsettingsFile;
    OutputsettingsFile.append("_paramaters_core.txt");
    /////////////////////IVFenc Par File//////////////////
    OutputsettingsFile2.append("_IVFEnc_Paramaters.txt");
    /////////////////////Tester IVF Comp//////////////////
    /////////////////////IVFENC IVF Comp//////////////////
    string IVFEncOutput1STR = input;
    string IVFEncOutput2STR = input;
    IVFEncOutput1STR.erase(IVFEncOutput1STR.length() - 4, 4);
    IVFEncOutput2STR.erase(IVFEncOutput2STR.length() - 4, 4);
    IVFEncOutput1STR.append("_IVFENC.ivf");
    IVFEncOutput2STR.append("_IVFENC.yuv");
    /////////////////////IVF Source to Raw///////////////
    string RawInput = input;
    string RawInputNameOnly = FileNameChar;

    RawInput.erase(RawInput.length() - 4, 4);
    RawInput.append("_Raw.yuv");
    RawInputNameOnly.append("_Raw.yuv");
    //////////////////////////////////////////////////////


    char ParameterFileTesterFP[255];    // Full path of tester parm file
    char ParameterFileIVFEncFP[255];    // Full path of ivfenc parm file
    char RawInputFP[255];               // Full path of raw ivfenc file
    char IVFEncOutput1FP[255];          // Full Path of IVFENC ivf Output
    char IVFEncOutput2FP[255];          // Full Path of IVFENC yuv Output
    char RawInputNO[255];               // Name only part of raw input file for ivfenc
    char ParameterFileIVFEncNO[255];    // Name only of IVFENC Par file
    char IVFEncOutput1NO[255];          // Name only of IVFENC ivf Output
    char IVFEncOutput2NO[255];          // Name only of IVFENC yuv Output


    snprintf(ParameterFileTesterFP, 255, "%s", OutputsettingsFile.c_str());
    snprintf(ParameterFileIVFEncFP, 255, "%s", OutputsettingsFile2.c_str());
    snprintf(RawInputNO, 255, "%s", RawInputNameOnly.c_str());
    snprintf(RawInputFP, 255, "%s", RawInput.c_str());
    snprintf(IVFEncOutput1FP, 255, "%s", IVFEncOutput1STR.c_str());
    snprintf(IVFEncOutput2FP, 255, "%s", IVFEncOutput2STR.c_str());

    vpxt_file_name(ParameterFileIVFEncFP, ParameterFileIVFEncNO, 0); // name only part of parm file for ivfenc
    vpxt_file_name(IVFEncOutput1FP, IVFEncOutput1NO, 0);
    vpxt_file_name(IVFEncOutput2FP, IVFEncOutput2NO, 0);

    cout << "\n\n";
    cout << "ParameterFileTesterFP: " << ParameterFileTesterFP << "\n";
    cout << "ParameterFileIVFEncFP: " << ParameterFileIVFEncFP << "\n";
    cout << "RawInputFP: " << RawInputFP << "\n";
    cout << "IVFEncOutput1FP: " << IVFEncOutput1FP << "\n";
    cout << "IVFEncOutput2FP: " << IVFEncOutput2FP << "\n";
    cout << "RawInputNO: " << RawInputNO << "\n";
    cout << "ParameterFileIVFEncNO: " << ParameterFileIVFEncNO << "\n";
    cout << "IVFEncOutput1NO: " << IVFEncOutput1NO << "\n";
    cout << "IVFEncOutput2NO: " << IVFEncOutput2NO << "\n";

    // name only part of parm file for ivfenc - ParameterFileIVFEncNameOnly
    // name only part of raw input file for ivfenc - RawInputNameOnlyChar

    int speed = 0;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;

    if (argc == 7)
    {
        opt = vpxt_input_settings(argv[6]);
    }

    int CompressInt = opt.allow_df;

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;
        vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
    }

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;
        vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
    }

    if (Mode == 2)
    {
        opt.Mode = MODE_BESTQUALITY;
        vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
    }

    if (Mode == 3)
    {
    }

    if (Mode == 4)
    {
        opt.Mode = MODE_SECONDPASS;
        vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
    }

    if (Mode == 5)
    {
        opt.Mode = MODE_SECONDPASS_BEST;
        vpxt_compress_ivf_to_ivf_no_error_output(input, output, speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3);
    }

    ConvertParmFileToIVFenc(ParameterFileTesterFP, ParameterFileIVFEncFP);  //Make IVFENC Parameter file from Tester Parameter File
    IVF2Raw(input, RawInputFP);                                             //Make Raw YUV File from IVF Input

    int Width = opt.Width;
    int Height = opt.Height;
    int FrameRate = opt.frame_rate;
    char ConversionHolder[256];

    //Make System Exe string for IVFENC to Create Compression
    string Program = "ivfenc ";
    Program.append(RawInputNO);
    Program.append(" ");
    memset(ConversionHolder, 0, sizeof(ConversionHolder));
    Program.append(vpxt_itoa_custom(Width, ConversionHolder, 10));
    Program.append(" ");
    memset(ConversionHolder, 0, sizeof(ConversionHolder));
    Program.append(vpxt_itoa_custom(Height, ConversionHolder, 10));
    Program.append(" ");
    memset(ConversionHolder, 0, sizeof(ConversionHolder));
    Program.append(vpxt_itoa_custom(FrameRate, ConversionHolder, 10));
    Program.append(" ");
    Program.append(IVFEncOutput1NO);
    Program.append(" ");
    Program.append(IVFEncOutput2NO);
    Program.append(" ");
    Program.append(ParameterFileIVFEncNO);
    Program.append(" \" \" ");

    cout << "\n" << Program << "\n\n";

    system(Program.c_str());

    int CompResult = vpxt_compare_ivf(IVFEncOutput1FP, output);

    if (CompResult == -1)
    {
        cout << "\n\n FILES ARE IDENTICAL - PASSED\n";
        return 1;
    }

    cout << "\n\n FILES ARE NOT IDENTICAL - FAILED\n";


    return 0;
}
int compare_code_coverage(int argc, char *argv[])
{
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n Compare Code Coverage\n\n"
                "    <Current Code Coverage Dir>\n"
                "    <New Code Coverage Dir>\n"
                "    <Updated Code Coverage Dir>\n"
                "\n");

        return 0;
    }

    tprintf(PRINT_STD, "\nCurrent Directory:\n%s\nNew Directory: \n%s\nUpdated Directory: \n%s\n\n", argv[2], argv[3] , argv[4]);

    string currentFile = "";
    int x = 1;

    while (x <= 30)
    {
        if (x == 1)
        {
            currentFile = "\\alloccommon.c.gcov.txt";
        }

        if (x == 2)
        {
            currentFile = "\\blockd.c.gcov.txt";
        }

        if (x == 3)
        {
            currentFile = "\\dboolhuff.c.gcov.txt";
        }

        if (x == 4)
        {
            currentFile = "\\decodemv.c.gcov.txt";
        }

        if (x == 5)
        {
            currentFile = "\\decodframe.c.gcov.txt";
        }

        if (x == 6)
        {
            currentFile = "\\demode.c.gcov.txt";
        }

        if (x == 7)
        {
            currentFile = "\\dequantize.c.gcov.txt";
        }

        if (x == 8)
        {
            currentFile = "\\detokenize.c.gcov.txt";
        }

        if (x == 9)
        {
            currentFile = "\\dsystemdependent.c.gcov.txt";
        }

        if (x == 10)
        {
            currentFile = "\\entropy.c.gcov.txt";
        }

        if (x == 11)
        {
            currentFile = "\\entropymode.c.gcov.txt";
        }

        if (x == 12)
        {
            currentFile = "\\extend.c.gcov.txt";
        }

        if (x == 13)
        {
            currentFile = "\\filter_c.c.gcov.txt";
        }

        if (x == 14)
        {
            currentFile = "\\findnearmv.c.gcov.txt";
        }

        //if( x == 15){currentFile = "\\getproc.c.gcov.txt";}
        if (x == 15)
        {
            currentFile = "\\idctllm.c.gcov.txt";
        }

        if (x == 16)
        {
            currentFile = "\\loopfilter.c.gcov.txt";
        }

        if (x == 17)
        {
            currentFile = "\\mbpitch.c.gcov.txt";
        }

        if (x == 18)
        {
            currentFile = "\\onyxd_if.c.gcov.txt";
        }

        if (x == 19)
        {
            currentFile = "\\postproc.c.gcov.txt";
        }

        if (x == 20)
        {
            currentFile = "\\quant_common.c.gcov.txt";
        }

        if (x == 21)
        {
            currentFile = "\\recon.c.gcov.txt";
        }

        if (x == 22)
        {
            currentFile = "\\reconinter.c.gcov.txt";
        }

        if (x == 23)
        {
            currentFile = "\\reconintra.c.gcov.txt";
        }

        if (x == 24)
        {
            currentFile = "\\reconintra4x4.c.gcov.txt";
        }

        if (x == 25)
        {
            currentFile = "\\Segmentation_common.c.gcov.txt";
        }

        if (x == 26)
        {
            currentFile = "\\swapyv12buffer.c.gcov.txt";
        }

        if (x == 27)
        {
            currentFile = "\\systemdependent.c.gcov.txt";
        }

        if (x == 28)
        {
            currentFile = "\\textblit.c.gcov.txt";
        }

        if (x == 39)
        {
            currentFile = "\\threading.c.gcov.txt";
        }

        if (x == 30)
        {
            currentFile = "\\treecoder.c.gcov.txt";
        }

        //if( x == 32){currentFile = "\\treereader.c.gcov.txt";}
        //if( x == 33){currentFile = "\\yv12config.c.gcov.txt";}
        //if( x == 34){currentFile = "\\yv12extend.c.gcov.txt";}

        string CurCC = argv[2];
        string NewCC = argv[3];
        string UpdCC = argv[4];
        string UniqueCC = argv[3];

        CurCC.append(currentFile);
        NewCC.append(currentFile);
        UpdCC.append(currentFile);

        UniqueCC.append("\\UniqueActivations\\");
        string SysStr = "md \"";
        SysStr.append(UniqueCC);
        SysStr.append("\"");
        system(SysStr.c_str());

        UniqueCC.append(currentFile);
        UniqueCC.erase(UniqueCC.length() - 4, 4);
        UniqueCC.append("_UniqueActivations.txt");

        ifstream CurCCFile(CurCC.c_str());
        ifstream NewCCFile(NewCC.c_str());
        ofstream UpdCCFile(UpdCC.c_str());
        ofstream UnqCCFile(UniqueCC.c_str());

        if (!CurCCFile.good())
        {
            cout << "\nCannot open " << CurCC.c_str() << "\n";
            CurCCFile.close();
            NewCCFile.close();
            UpdCCFile.close();
            return 0;
        }

        if (!NewCCFile.good())
        {
            cout << "\nCannot open " << NewCC.c_str() << "\n";
            CurCCFile.close();
            NewCCFile.close();
            UpdCCFile.close();
            return 0;
        }

        tprintf(PRINT_STD, "\nChecking: %s", currentFile.c_str());
        int line = 0;

        while (!CurCCFile.eof())
        {
            if (line != 0)
            {
                UpdCCFile << "\n";
            }

            line++;
            char CurCCBuffer[9999];
            CurCCFile.getline(CurCCBuffer, 9999);

            char NewCCBuffer[9999];
            NewCCFile.getline(NewCCBuffer, 9999);

            string CurCCStr = CurCCBuffer;
            string NewCCStr = NewCCBuffer;

            if (CurCCStr.compare(0, 10, "    #####:") == 0)
            {
                if (NewCCStr.compare(0, 10, "    #####:") != 0)
                {
                    tprintf(PRINT_STD, "\nNew Activation: %s\n     %s", NewCC.c_str(), NewCCStr.c_str());
                    UpdCCFile << NewCCStr.c_str();
                    UnqCCFile << NewCCStr.c_str() << "\n";
                }
                else
                {
                    UpdCCFile << CurCCStr.c_str();
                }
            }
            else
            {
                UpdCCFile << CurCCStr.c_str();
            }
        }

        CurCCFile.close();
        NewCCFile.close();
        UpdCCFile.close();
        UnqCCFile.close();

        char UniqueCCChar[255];
        snprintf(UniqueCCChar, 255, "%s", UniqueCC.c_str());

        if (vpxt_file_size(UniqueCCChar, 0) == 0)
        {
            string SysStr2 = "del \"";
            SysStr2.append(UniqueCC);
            SysStr2.append("\"");
            system(SysStr2.c_str());
        }

        x++;
    }

    return 0;
}
int FormatCodeCoverageFile(int argc, char *argv[])
{
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n Format Code Coverage Files\n\n"
                "    <Current Code Coverage Dir>\n"
                "    <New Code Coverage Dir>\n"
                "\n");

        return 0;
    }

    tprintf(PRINT_STD, "\nCurrent Directory:\n%s\nNew Directory: \n%s\nUpdated Directory: \n%s\n\n", argv[2], argv[3] , argv[4]);

    string currentFile = "";
    int x = 1;

    while (x <= 34)
    {
        if (x == 1)
        {
            currentFile = "\\alloccommon.c.gcov.txt";
        }

        if (x == 2)
        {
            currentFile = "\\blockd.c.gcov.txt";
        }

        if (x == 3)
        {
            currentFile = "\\dboolhuff.c.gcov.txt";
        }

        if (x == 4)
        {
            currentFile = "\\decodemv.c.gcov.txt";
        }

        if (x == 5)
        {
            currentFile = "\\decodframe.c.gcov.txt";
        }

        if (x == 6)
        {
            currentFile = "\\demode.c.gcov.txt";
        }

        if (x == 7)
        {
            currentFile = "\\dequantize.c.gcov.txt";
        }

        if (x == 8)
        {
            currentFile = "\\detokenize.c.gcov.txt";
        }

        if (x == 9)
        {
            currentFile = "\\dsystemdependent.c.gcov.txt";
        }

        if (x == 10)
        {
            currentFile = "\\entropy.c.gcov.txt";
        }

        if (x == 11)
        {
            currentFile = "\\entropymode.c.gcov.txt";
        }

        if (x == 12)
        {
            currentFile = "\\extend.c.gcov.txt";
        }

        if (x == 13)
        {
            currentFile = "\\filter_c.c.gcov.txt";
        }

        if (x == 14)
        {
            currentFile = "\\findnearmv.c.gcov.txt";
        }

        if (x == 15)
        {
            currentFile = "\\getproc.c.gcov.txt";
        }

        if (x == 16)
        {
            currentFile = "\\idctllm.c.gcov.txt";
        }

        if (x == 17)
        {
            currentFile = "\\loopfilter.c.gcov.txt";
        }

        if (x == 18)
        {
            currentFile = "\\mbpitch.c.gcov.txt";
        }

        if (x == 19)
        {
            currentFile = "\\onyxd_if.c.gcov.txt";
        }

        if (x == 20)
        {
            currentFile = "\\postproc.c.gcov.txt";
        }

        if (x == 21)
        {
            currentFile = "\\quant_common.c.gcov.txt";
        }

        if (x == 22)
        {
            currentFile = "\\recon.c.gcov.txt";
        }

        if (x == 23)
        {
            currentFile = "\\reconinter.c.gcov.txt";
        }

        if (x == 24)
        {
            currentFile = "\\reconintra.c.gcov.txt";
        }

        if (x == 25)
        {
            currentFile = "\\reconintra4x4.c.gcov.txt";
        }

        if (x == 26)
        {
            currentFile = "\\Segmentation_common.c.gcov.txt";
        }

        if (x == 27)
        {
            currentFile = "\\swapyv12buffer.c.gcov.txt";
        }

        if (x == 28)
        {
            currentFile = "\\systemdependent.c.gcov.txt";
        }

        if (x == 29)
        {
            currentFile = "\\textblit.c.gcov.txt";
        }

        if (x == 30)
        {
            currentFile = "\\threading.c.gcov.txt";
        }

        if (x == 31)
        {
            currentFile = "\\treecoder.c.gcov.txt";
        }

        if (x == 32)
        {
            currentFile = "\\treereader.c.gcov.txt";
        }

        if (x == 33)
        {
            currentFile = "\\yv12config.c.gcov.txt";
        }

        if (x == 34)
        {
            currentFile = "\\yv12extend.c.gcov.txt";
        }

        string CurCC = argv[2];
        string NewCC = argv[3];

        CurCC.append(currentFile);
        NewCC.append(currentFile);

        ifstream CurCCFile(CurCC.c_str());
        ofstream NewCCFile(NewCC.c_str());


        if (!CurCCFile.good())
        {
            cout << "\nCannot open " << CurCC.c_str() << "\n";
            CurCCFile.close();
            NewCCFile.close();
            return 0;
        }

        tprintf(PRINT_STD, "\nFormatting: %s", currentFile.c_str());
        int line = 0;
        int Function = 0;

        while (!CurCCFile.eof())
        {
            if (line != 0 && Function == 0)
            {
                NewCCFile << "\n";   //keeps from adding an endline to the last line of file or where a "function" occurs
            }

            line++;
            char CurCCBuffer[9999];
            CurCCFile.getline(CurCCBuffer, 9999);

            string CurCCStr = CurCCBuffer;

            if (CurCCStr.compare(0, 8, "function") == 0)
            {
                Function = 1;
            }
            else
            {
                NewCCFile << CurCCStr.c_str();
                Function = 0;
            }
        }

        CurCCFile.close();
        NewCCFile.close();

        x++;
    }

    return 0;
}
int TestVectorIndex(int argc, char *argv[])
{
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n Test Vector Index\n\n"
                "    <Current Code Coverage Dir>\n"
                "    <File to Check>\n"
                "    <Line Number to Check>\n"
                "\n");

        return 0;
    }

    tprintf(PRINT_STD, "\n Searching for %s Line %i Coverage.\n\n", argv[3], atoi(argv[4]));

    string currentFile = "";
    int x = 1;
    int y = 1;

    while (x < 6)
    {
        while (y < 15)
        {

            string CurCC = argv[2];
            string currentFile = argv[3];

            char Charx[32];
            vpxt_itoa_custom(x, Charx, 10);
            char Chary[32];
            vpxt_itoa_custom(y, Chary, 10);

            if (x == 1 && y == 1)
            {
                CurCC.append("\\VP8REF");
                CurCC.append(Charx);
                CurCC.append("\\");
                CurCC.append(Chary);
                CurCC.append("\\summary");
                CurCC.append("\\");
                CurCC.append(currentFile);
                CurCC.append(".c.gcov.txt");
            }
            else
            {
                CurCC.append("\\VP8REF");
                CurCC.append(Charx);
                CurCC.append("\\");
                CurCC.append(Chary);
                CurCC.append("\\summary");
                CurCC.append("\\UniqueActivations\\");
                CurCC.append(currentFile);
                CurCC.append(".c.gcov_UniqueActivations.txt");
            }

            ifstream CurCCFile(CurCC.c_str());

            if (CurCCFile.good())
            {
                int line = 0;

                while (!CurCCFile.eof())
                {
                    line++;
                    char CurCCBuffer[9999];
                    CurCCFile.getline(CurCCBuffer, 9999); //Get Line
                    string CurCCStr = CurCCBuffer;       //Turn into string

                    int Colfound1 = 0;
                    int Colfound2 = 0;
                    Colfound1 = CurCCStr.find(":");
                    Colfound2 = CurCCStr.find(":", Colfound1 + 1);

                    if (Colfound2 > 0)
                    {
                        string LineNumChar = CurCCStr.substr(Colfound1, Colfound2 - Colfound1);

                        int LastSPaceFound = LineNumChar.find_last_of(" ");
                        string LineNumCharNoSpace = LineNumChar.substr(LastSPaceFound + 1);

                        if (atoi(LineNumCharNoSpace.c_str()) == atoi(argv[4]))
                        {

                            if (CurCCStr.substr(Colfound1 - 5, 5).compare("#####") != 0)
                            {
                                cout << "          " << CurCCStr << "\n";
                                cout << "  Found\n";
                                tprintf(PRINT_STD, "          Line Covered by Test Vector %i ", y);

                                if (x == 1)
                                {
                                    tprintf(PRINT_STD, "Under Normal Conditions\n");
                                }

                                if (x == 2)
                                {
                                    tprintf(PRINT_STD, "When run using Multiple Cores\n");
                                }

                                if (x == 3)
                                {
                                    tprintf(PRINT_STD, "When run using Add Noise\n");
                                }

                                if (x == 4)
                                {
                                    tprintf(PRINT_STD, "When run using Deblock\n");
                                }

                                if (x == 5)
                                {
                                    tprintf(PRINT_STD, "When run using Demarcoblock\n");
                                }

                                return 0;
                            }
                        }
                    }
                }
            }

            CurCCFile.close();
            y++;
        }

        x++;
        y = 1;
    }

    cout << "\n  Line Coverage Not Found.\n";
    return 0;
}

int APICOMPRESS(int argc, char *argv[])
{
#ifdef API

    if (argc != 7)
    {
        tprintf(PRINT_STD,
                "\n  APICompress \n\n"
                "    <Input File>\n"
                "    <outputfile>\n"
                "    <width>\n"
                "    <height>\n"
                "    <frame rate>\n"
               );

        return 0;
    }

    char *input = argv[2];
    char *output = argv[3];
    int width = atoi(argv[4]);
    int height = atoi(argv[5]);
    double framerate = (double)atoi(argv[6]);

    VP8_CONFIG testname;
    vpxt_default_parameters(testname);

    testname = vpxt_input_settings("C:\\Users\\jberry\\Desktop\\AllowDFOnOutput_Paramaters.txt");
    API20Encoder(width, height, input, output);

#else
    tprintf(PRINT_STD, "\n Function Not supported in current build \n");
#endif

    return 0;
}
int APIDECOMPRESS(int argc, char *argv[])
{

#ifdef API

    if (argc != 5)
    {
        tprintf(PRINT_STD,
                "\n  APIDecompress \n\n"
                "    <Input File>\n"
                "    <Codec - vp8>\n"
                "    <outputfile>\n"
               );
        return 0;
    }

    char *input = argv[2];
    char *codec = argv[3];
    char *output = argv[4];

    API20Decoder(input , output);
#else
    tprintf(PRINT_STD, "\n Function Not supported in current build \n");
#endif

    return 0;
}

int IVFDECtest_vector_test(int argc, char *argv[])
{
    return 0;
}
int ArrayCovFailListToFullList(int argc, char *argv[])
{
    //This Function Takes an input file from a code coverage run that summarizes failure
    //and formats it to display Passes and Fails.

    if (!(argc == 4))
    {
        tprintf(PRINT_STD, "\n"
                "  ArrayCovFailListToFullList \n\n"
                "    <Input File>\n"
                "    <Output File>\n"
                "\n"
                "  Note: The input file must have one line of white space at the top of the file\n"
                "\n"
               );

        return 0;
    }

    string FullPossibleRes[2785];

    FullPossibleRes[0] = "MD5 Checksums Identical for: ac_qlookup[0] - Fail";
    FullPossibleRes[1] = "MD5 Checksums Identical for: ac_qlookup[1] - Fail";
    FullPossibleRes[2] = "MD5 Checksums Identical for: ac_qlookup[2] - Fail";
    FullPossibleRes[3] = "MD5 Checksums Identical for: ac_qlookup[3] - Fail";
    FullPossibleRes[4] = "MD5 Checksums Identical for: ac_qlookup[4] - Fail";
    FullPossibleRes[5] = "MD5 Checksums Identical for: ac_qlookup[5] - Fail";
    FullPossibleRes[6] = "MD5 Checksums Identical for: ac_qlookup[6] - Fail";
    FullPossibleRes[7] = "MD5 Checksums Identical for: ac_qlookup[7] - Fail";
    FullPossibleRes[8] = "MD5 Checksums Identical for: ac_qlookup[8] - Fail";
    FullPossibleRes[9] = "MD5 Checksums Identical for: ac_qlookup[9] - Fail";
    FullPossibleRes[10] = "MD5 Checksums Identical for: ac_qlookup[10] - Fail";
    FullPossibleRes[11] = "MD5 Checksums Identical for: ac_qlookup[11] - Fail";
    FullPossibleRes[12] = "MD5 Checksums Identical for: ac_qlookup[12] - Fail";
    FullPossibleRes[13] = "MD5 Checksums Identical for: ac_qlookup[13] - Fail";
    FullPossibleRes[14] = "MD5 Checksums Identical for: ac_qlookup[14] - Fail";
    FullPossibleRes[15] = "MD5 Checksums Identical for: ac_qlookup[15] - Fail";
    FullPossibleRes[16] = "MD5 Checksums Identical for: ac_qlookup[16] - Fail";
    FullPossibleRes[17] = "MD5 Checksums Identical for: ac_qlookup[17] - Fail";
    FullPossibleRes[18] = "MD5 Checksums Identical for: ac_qlookup[18] - Fail";
    FullPossibleRes[19] = "MD5 Checksums Identical for: ac_qlookup[19] - Fail";
    FullPossibleRes[20] = "MD5 Checksums Identical for: ac_qlookup[20] - Fail";
    FullPossibleRes[21] = "MD5 Checksums Identical for: ac_qlookup[21] - Fail";
    FullPossibleRes[22] = "MD5 Checksums Identical for: ac_qlookup[22] - Fail";
    FullPossibleRes[23] = "MD5 Checksums Identical for: ac_qlookup[23] - Fail";
    FullPossibleRes[24] = "MD5 Checksums Identical for: ac_qlookup[24] - Fail";
    FullPossibleRes[25] = "MD5 Checksums Identical for: ac_qlookup[25] - Fail";
    FullPossibleRes[26] = "MD5 Checksums Identical for: ac_qlookup[26] - Fail";
    FullPossibleRes[27] = "MD5 Checksums Identical for: ac_qlookup[27] - Fail";
    FullPossibleRes[28] = "MD5 Checksums Identical for: ac_qlookup[28] - Fail";
    FullPossibleRes[29] = "MD5 Checksums Identical for: ac_qlookup[29] - Fail";
    FullPossibleRes[30] = "MD5 Checksums Identical for: ac_qlookup[30] - Fail";
    FullPossibleRes[31] = "MD5 Checksums Identical for: ac_qlookup[31] - Fail";
    FullPossibleRes[32] = "MD5 Checksums Identical for: ac_qlookup[32] - Fail";
    FullPossibleRes[33] = "MD5 Checksums Identical for: ac_qlookup[33] - Fail";
    FullPossibleRes[34] = "MD5 Checksums Identical for: ac_qlookup[34] - Fail";
    FullPossibleRes[35] = "MD5 Checksums Identical for: ac_qlookup[35] - Fail";
    FullPossibleRes[36] = "MD5 Checksums Identical for: ac_qlookup[36] - Fail";
    FullPossibleRes[37] = "MD5 Checksums Identical for: ac_qlookup[37] - Fail";
    FullPossibleRes[38] = "MD5 Checksums Identical for: ac_qlookup[38] - Fail";
    FullPossibleRes[39] = "MD5 Checksums Identical for: ac_qlookup[39] - Fail";
    FullPossibleRes[40] = "MD5 Checksums Identical for: ac_qlookup[40] - Fail";
    FullPossibleRes[41] = "MD5 Checksums Identical for: ac_qlookup[41] - Fail";
    FullPossibleRes[42] = "MD5 Checksums Identical for: ac_qlookup[42] - Fail";
    FullPossibleRes[43] = "MD5 Checksums Identical for: ac_qlookup[43] - Fail";
    FullPossibleRes[44] = "MD5 Checksums Identical for: ac_qlookup[44] - Fail";
    FullPossibleRes[45] = "MD5 Checksums Identical for: ac_qlookup[45] - Fail";
    FullPossibleRes[46] = "MD5 Checksums Identical for: ac_qlookup[46] - Fail";
    FullPossibleRes[47] = "MD5 Checksums Identical for: ac_qlookup[47] - Fail";
    FullPossibleRes[48] = "MD5 Checksums Identical for: ac_qlookup[48] - Fail";
    FullPossibleRes[49] = "MD5 Checksums Identical for: ac_qlookup[49] - Fail";
    FullPossibleRes[50] = "MD5 Checksums Identical for: ac_qlookup[50] - Fail";
    FullPossibleRes[51] = "MD5 Checksums Identical for: ac_qlookup[51] - Fail";
    FullPossibleRes[52] = "MD5 Checksums Identical for: ac_qlookup[52] - Fail";
    FullPossibleRes[53] = "MD5 Checksums Identical for: ac_qlookup[53] - Fail";
    FullPossibleRes[54] = "MD5 Checksums Identical for: ac_qlookup[54] - Fail";
    FullPossibleRes[55] = "MD5 Checksums Identical for: ac_qlookup[55] - Fail";
    FullPossibleRes[56] = "MD5 Checksums Identical for: ac_qlookup[56] - Fail";
    FullPossibleRes[57] = "MD5 Checksums Identical for: ac_qlookup[57] - Fail";
    FullPossibleRes[58] = "MD5 Checksums Identical for: ac_qlookup[58] - Fail";
    FullPossibleRes[59] = "MD5 Checksums Identical for: ac_qlookup[59] - Fail";
    FullPossibleRes[60] = "MD5 Checksums Identical for: ac_qlookup[60] - Fail";
    FullPossibleRes[61] = "MD5 Checksums Identical for: ac_qlookup[61] - Fail";
    FullPossibleRes[62] = "MD5 Checksums Identical for: ac_qlookup[62] - Fail";
    FullPossibleRes[63] = "MD5 Checksums Identical for: ac_qlookup[63] - Fail";
    FullPossibleRes[64] = "MD5 Checksums Identical for: ac_qlookup[64] - Fail";
    FullPossibleRes[65] = "MD5 Checksums Identical for: ac_qlookup[65] - Fail";
    FullPossibleRes[66] = "MD5 Checksums Identical for: ac_qlookup[66] - Fail";
    FullPossibleRes[67] = "MD5 Checksums Identical for: ac_qlookup[67] - Fail";
    FullPossibleRes[68] = "MD5 Checksums Identical for: ac_qlookup[68] - Fail";
    FullPossibleRes[69] = "MD5 Checksums Identical for: ac_qlookup[69] - Fail";
    FullPossibleRes[70] = "MD5 Checksums Identical for: ac_qlookup[70] - Fail";
    FullPossibleRes[71] = "MD5 Checksums Identical for: ac_qlookup[71] - Fail";
    FullPossibleRes[72] = "MD5 Checksums Identical for: ac_qlookup[72] - Fail";
    FullPossibleRes[73] = "MD5 Checksums Identical for: ac_qlookup[73] - Fail";
    FullPossibleRes[74] = "MD5 Checksums Identical for: ac_qlookup[74] - Fail";
    FullPossibleRes[75] = "MD5 Checksums Identical for: ac_qlookup[75] - Fail";
    FullPossibleRes[76] = "MD5 Checksums Identical for: ac_qlookup[76] - Fail";
    FullPossibleRes[77] = "MD5 Checksums Identical for: ac_qlookup[77] - Fail";
    FullPossibleRes[78] = "MD5 Checksums Identical for: ac_qlookup[78] - Fail";
    FullPossibleRes[79] = "MD5 Checksums Identical for: ac_qlookup[79] - Fail";
    FullPossibleRes[80] = "MD5 Checksums Identical for: ac_qlookup[80] - Fail";
    FullPossibleRes[81] = "MD5 Checksums Identical for: ac_qlookup[81] - Fail";
    FullPossibleRes[82] = "MD5 Checksums Identical for: ac_qlookup[82] - Fail";
    FullPossibleRes[83] = "MD5 Checksums Identical for: ac_qlookup[83] - Fail";
    FullPossibleRes[84] = "MD5 Checksums Identical for: ac_qlookup[84] - Fail";
    FullPossibleRes[85] = "MD5 Checksums Identical for: ac_qlookup[85] - Fail";
    FullPossibleRes[86] = "MD5 Checksums Identical for: ac_qlookup[86] - Fail";
    FullPossibleRes[87] = "MD5 Checksums Identical for: ac_qlookup[87] - Fail";
    FullPossibleRes[88] = "MD5 Checksums Identical for: ac_qlookup[88] - Fail";
    FullPossibleRes[89] = "MD5 Checksums Identical for: ac_qlookup[89] - Fail";
    FullPossibleRes[90] = "MD5 Checksums Identical for: ac_qlookup[90] - Fail";
    FullPossibleRes[91] = "MD5 Checksums Identical for: ac_qlookup[91] - Fail";
    FullPossibleRes[92] = "MD5 Checksums Identical for: ac_qlookup[92] - Fail";
    FullPossibleRes[93] = "MD5 Checksums Identical for: ac_qlookup[93] - Fail";
    FullPossibleRes[94] = "MD5 Checksums Identical for: ac_qlookup[94] - Fail";
    FullPossibleRes[95] = "MD5 Checksums Identical for: ac_qlookup[95] - Fail";
    FullPossibleRes[96] = "MD5 Checksums Identical for: ac_qlookup[96] - Fail";
    FullPossibleRes[97] = "MD5 Checksums Identical for: ac_qlookup[97] - Fail";
    FullPossibleRes[98] = "MD5 Checksums Identical for: ac_qlookup[98] - Fail";
    FullPossibleRes[99] = "MD5 Checksums Identical for: ac_qlookup[99] - Fail";
    FullPossibleRes[100] = "MD5 Checksums Identical for: ac_qlookup[100] - Fail";
    FullPossibleRes[101] = "MD5 Checksums Identical for: ac_qlookup[101] - Fail";
    FullPossibleRes[102] = "MD5 Checksums Identical for: ac_qlookup[102] - Fail";
    FullPossibleRes[103] = "MD5 Checksums Identical for: ac_qlookup[103] - Fail";
    FullPossibleRes[104] = "MD5 Checksums Identical for: ac_qlookup[104] - Fail";
    FullPossibleRes[105] = "MD5 Checksums Identical for: ac_qlookup[105] - Fail";
    FullPossibleRes[106] = "MD5 Checksums Identical for: ac_qlookup[106] - Fail";
    FullPossibleRes[107] = "MD5 Checksums Identical for: ac_qlookup[107] - Fail";
    FullPossibleRes[108] = "MD5 Checksums Identical for: ac_qlookup[108] - Fail";
    FullPossibleRes[109] = "MD5 Checksums Identical for: ac_qlookup[109] - Fail";
    FullPossibleRes[110] = "MD5 Checksums Identical for: ac_qlookup[110] - Fail";
    FullPossibleRes[111] = "MD5 Checksums Identical for: ac_qlookup[111] - Fail";
    FullPossibleRes[112] = "MD5 Checksums Identical for: ac_qlookup[112] - Fail";
    FullPossibleRes[113] = "MD5 Checksums Identical for: ac_qlookup[113] - Fail";
    FullPossibleRes[114] = "MD5 Checksums Identical for: ac_qlookup[114] - Fail";
    FullPossibleRes[115] = "MD5 Checksums Identical for: ac_qlookup[115] - Fail";
    FullPossibleRes[116] = "MD5 Checksums Identical for: ac_qlookup[116] - Fail";
    FullPossibleRes[117] = "MD5 Checksums Identical for: ac_qlookup[117] - Fail";
    FullPossibleRes[118] = "MD5 Checksums Identical for: ac_qlookup[118] - Fail";
    FullPossibleRes[119] = "MD5 Checksums Identical for: ac_qlookup[119] - Fail";
    FullPossibleRes[120] = "MD5 Checksums Identical for: ac_qlookup[120] - Fail";
    FullPossibleRes[121] = "MD5 Checksums Identical for: ac_qlookup[121] - Fail";
    FullPossibleRes[122] = "MD5 Checksums Identical for: ac_qlookup[122] - Fail";
    FullPossibleRes[123] = "MD5 Checksums Identical for: ac_qlookup[123] - Fail";
    FullPossibleRes[124] = "MD5 Checksums Identical for: ac_qlookup[124] - Fail";
    FullPossibleRes[125] = "MD5 Checksums Identical for: ac_qlookup[125] - Fail";
    FullPossibleRes[126] = "MD5 Checksums Identical for: ac_qlookup[126] - Fail";
    FullPossibleRes[127] = "MD5 Checksums Identical for: ac_qlookup[127] - Fail";
    FullPossibleRes[128] = "MD5 Checksums Identical for: bbb[0] - Fail";
    FullPossibleRes[129] = "MD5 Checksums Identical for: bbb[1] - Fail";
    FullPossibleRes[130] = "MD5 Checksums Identical for: bbb[2] - Fail";
    FullPossibleRes[131] = "MD5 Checksums Identical for: bbb[3] - Fail";
    FullPossibleRes[132] = "MD5 Checksums Identical for: bilinear_filters[0][0] - Fail";
    FullPossibleRes[133] = "MD5 Checksums Identical for: bilinear_filters[1][0] - Fail";
    FullPossibleRes[134] = "MD5 Checksums Identical for: bilinear_filters[2][0] - Fail";
    FullPossibleRes[135] = "MD5 Checksums Identical for: bilinear_filters[3][0] - Fail";
    FullPossibleRes[136] = "MD5 Checksums Identical for: bilinear_filters[4][0] - Fail";
    FullPossibleRes[137] = "MD5 Checksums Identical for: bilinear_filters[5][0] - Fail";
    FullPossibleRes[138] = "MD5 Checksums Identical for: bilinear_filters[6][0] - Fail";
    FullPossibleRes[139] = "MD5 Checksums Identical for: bilinear_filters[7][0] - Fail";
    FullPossibleRes[140] = "MD5 Checksums Identical for: bilinear_filters[0][1] - Fail";
    FullPossibleRes[141] = "MD5 Checksums Identical for: bilinear_filters[1][1] - Fail";
    FullPossibleRes[142] = "MD5 Checksums Identical for: bilinear_filters[2][1] - Fail";
    FullPossibleRes[143] = "MD5 Checksums Identical for: bilinear_filters[3][1] - Fail";
    FullPossibleRes[144] = "MD5 Checksums Identical for: bilinear_filters[4][1] - Fail";
    FullPossibleRes[145] = "MD5 Checksums Identical for: bilinear_filters[5][1] - Fail";
    FullPossibleRes[146] = "MD5 Checksums Identical for: bilinear_filters[6][1] - Fail";
    FullPossibleRes[147] = "MD5 Checksums Identical for: bilinear_filters[7][1] - Fail";
    FullPossibleRes[148] = "MD5 Checksums Identical for: bmode_cts[0] - Fail";
    FullPossibleRes[149] = "MD5 Checksums Identical for: bmode_cts[1] - Fail";
    FullPossibleRes[150] = "MD5 Checksums Identical for: bmode_cts[2] - Fail";
    FullPossibleRes[151] = "MD5 Checksums Identical for: bmode_cts[3] - Fail";
    FullPossibleRes[152] = "MD5 Checksums Identical for: bmode_cts[4] - Fail";
    FullPossibleRes[153] = "MD5 Checksums Identical for: bmode_cts[5] - Fail";
    FullPossibleRes[154] = "MD5 Checksums Identical for: bmode_cts[6] - Fail";
    FullPossibleRes[155] = "MD5 Checksums Identical for: bmode_cts[7] - Fail";
    FullPossibleRes[156] = "MD5 Checksums Identical for: bmode_cts[8] - Fail";
    FullPossibleRes[157] = "MD5 Checksums Identical for: bmode_cts[9] - Fail";
    FullPossibleRes[158] = "MD5 Checksums Identical for: cat1[0] - Fail";
    FullPossibleRes[159] = "MD5 Checksums Identical for: cat1[1] - Fail";
    FullPossibleRes[160] = "MD5 Checksums Identical for: cat2[0] - Fail";
    FullPossibleRes[161] = "MD5 Checksums Identical for: cat2[1] - Fail";
    FullPossibleRes[162] = "MD5 Checksums Identical for: cat2[2] - Fail";
    FullPossibleRes[163] = "MD5 Checksums Identical for: cat2[3] - Fail";
    FullPossibleRes[164] = "MD5 Checksums Identical for: cat3[0] - Fail";
    FullPossibleRes[165] = "MD5 Checksums Identical for: cat3[1] - Fail";
    FullPossibleRes[166] = "MD5 Checksums Identical for: cat3[2] - Fail";
    FullPossibleRes[167] = "MD5 Checksums Identical for: cat3[3] - Fail";
    FullPossibleRes[168] = "MD5 Checksums Identical for: cat3[4] - Fail";
    FullPossibleRes[169] = "MD5 Checksums Identical for: cat3[5] - Fail";
    FullPossibleRes[170] = "MD5 Checksums Identical for: cat4[0] - Fail";
    FullPossibleRes[171] = "MD5 Checksums Identical for: cat4[1] - Fail";
    FullPossibleRes[172] = "MD5 Checksums Identical for: cat4[2] - Fail";
    FullPossibleRes[173] = "MD5 Checksums Identical for: cat4[3] - Fail";
    FullPossibleRes[174] = "MD5 Checksums Identical for: cat4[4] - Fail";
    FullPossibleRes[175] = "MD5 Checksums Identical for: cat4[5] - Fail";
    FullPossibleRes[176] = "MD5 Checksums Identical for: cat4[6] - Fail";
    FullPossibleRes[177] = "MD5 Checksums Identical for: cat4[7] - Fail";
    FullPossibleRes[178] = "MD5 Checksums Identical for: cat5[0] - Fail";
    FullPossibleRes[179] = "MD5 Checksums Identical for: cat5[1] - Fail";
    FullPossibleRes[180] = "MD5 Checksums Identical for: cat5[2] - Fail";
    FullPossibleRes[181] = "MD5 Checksums Identical for: cat5[3] - Fail";
    FullPossibleRes[182] = "MD5 Checksums Identical for: cat5[4] - Fail";
    FullPossibleRes[183] = "MD5 Checksums Identical for: cat5[5] - Fail";
    FullPossibleRes[184] = "MD5 Checksums Identical for: cat5[6] - Fail";
    FullPossibleRes[185] = "MD5 Checksums Identical for: cat5[7] - Fail";
    FullPossibleRes[186] = "MD5 Checksums Identical for: cat5[8] - Fail";
    FullPossibleRes[187] = "MD5 Checksums Identical for: cat5[9] - Fail";
    FullPossibleRes[188] = "MD5 Checksums Identical for: cat6[0] - Fail";
    FullPossibleRes[189] = "MD5 Checksums Identical for: cat6[1] - Fail";
    FullPossibleRes[190] = "MD5 Checksums Identical for: cat6[2] - Fail";
    FullPossibleRes[191] = "MD5 Checksums Identical for: cat6[3] - Fail";
    FullPossibleRes[192] = "MD5 Checksums Identical for: cat6[4] - Fail";
    FullPossibleRes[193] = "MD5 Checksums Identical for: cat6[5] - Fail";
    FullPossibleRes[194] = "MD5 Checksums Identical for: cat6[6] - Fail";
    FullPossibleRes[195] = "MD5 Checksums Identical for: cat6[7] - Fail";
    FullPossibleRes[196] = "MD5 Checksums Identical for: cat6[8] - Fail";
    FullPossibleRes[197] = "MD5 Checksums Identical for: cat6[9] - Fail";
    FullPossibleRes[198] = "MD5 Checksums Identical for: cat6[10] - Fail";
    FullPossibleRes[199] = "MD5 Checksums Identical for: cat6[11] - Fail";
    FullPossibleRes[200] = "MD5 Checksums Identical for: cat6[12] - Fail";
    FullPossibleRes[201] = "MD5 Checksums Identical for: cat6[13] - Fail";
    FullPossibleRes[202] = "MD5 Checksums Identical for: cat6[14] - Fail";
    FullPossibleRes[203] = "MD5 Checksums Identical for: cat6[15] - Fail";
    FullPossibleRes[204] = "MD5 Checksums Identical for: cat6[16] - Fail";
    FullPossibleRes[205] = "MD5 Checksums Identical for: cat6[17] - Fail";
    FullPossibleRes[206] = "MD5 Checksums Identical for: cat6[18] - Fail";
    FullPossibleRes[207] = "MD5 Checksums Identical for: cat6[19] - Fail";
    FullPossibleRes[208] = "MD5 Checksums Identical for: cat6[20] - Fail";
    FullPossibleRes[209] = "MD5 Checksums Identical for: cat6[21] - Fail";
    FullPossibleRes[210] = "MD5 Checksums Identical for: dc_qlookup[0] - Fail";
    FullPossibleRes[211] = "MD5 Checksums Identical for: dc_qlookup[1] - Fail";
    FullPossibleRes[212] = "MD5 Checksums Identical for: dc_qlookup[2] - Fail";
    FullPossibleRes[213] = "MD5 Checksums Identical for: dc_qlookup[3] - Fail";
    FullPossibleRes[214] = "MD5 Checksums Identical for: dc_qlookup[4] - Fail";
    FullPossibleRes[215] = "MD5 Checksums Identical for: dc_qlookup[5] - Fail";
    FullPossibleRes[216] = "MD5 Checksums Identical for: dc_qlookup[6] - Fail";
    FullPossibleRes[217] = "MD5 Checksums Identical for: dc_qlookup[7] - Fail";
    FullPossibleRes[218] = "MD5 Checksums Identical for: dc_qlookup[8] - Fail";
    FullPossibleRes[219] = "MD5 Checksums Identical for: dc_qlookup[9] - Fail";
    FullPossibleRes[220] = "MD5 Checksums Identical for: dc_qlookup[10] - Fail";
    FullPossibleRes[221] = "MD5 Checksums Identical for: dc_qlookup[11] - Fail";
    FullPossibleRes[222] = "MD5 Checksums Identical for: dc_qlookup[12] - Fail";
    FullPossibleRes[223] = "MD5 Checksums Identical for: dc_qlookup[13] - Fail";
    FullPossibleRes[224] = "MD5 Checksums Identical for: dc_qlookup[14] - Fail";
    FullPossibleRes[225] = "MD5 Checksums Identical for: dc_qlookup[15] - Fail";
    FullPossibleRes[226] = "MD5 Checksums Identical for: dc_qlookup[16] - Fail";
    FullPossibleRes[227] = "MD5 Checksums Identical for: dc_qlookup[17] - Fail";
    FullPossibleRes[228] = "MD5 Checksums Identical for: dc_qlookup[18] - Fail";
    FullPossibleRes[229] = "MD5 Checksums Identical for: dc_qlookup[19] - Fail";
    FullPossibleRes[230] = "MD5 Checksums Identical for: dc_qlookup[20] - Fail";
    FullPossibleRes[231] = "MD5 Checksums Identical for: dc_qlookup[21] - Fail";
    FullPossibleRes[232] = "MD5 Checksums Identical for: dc_qlookup[22] - Fail";
    FullPossibleRes[233] = "MD5 Checksums Identical for: dc_qlookup[23] - Fail";
    FullPossibleRes[234] = "MD5 Checksums Identical for: dc_qlookup[24] - Fail";
    FullPossibleRes[235] = "MD5 Checksums Identical for: dc_qlookup[25] - Fail";
    FullPossibleRes[236] = "MD5 Checksums Identical for: dc_qlookup[26] - Fail";
    FullPossibleRes[237] = "MD5 Checksums Identical for: dc_qlookup[27] - Fail";
    FullPossibleRes[238] = "MD5 Checksums Identical for: dc_qlookup[28] - Fail";
    FullPossibleRes[239] = "MD5 Checksums Identical for: dc_qlookup[29] - Fail";
    FullPossibleRes[240] = "MD5 Checksums Identical for: dc_qlookup[30] - Fail";
    FullPossibleRes[241] = "MD5 Checksums Identical for: dc_qlookup[31] - Fail";
    FullPossibleRes[242] = "MD5 Checksums Identical for: dc_qlookup[32] - Fail";
    FullPossibleRes[243] = "MD5 Checksums Identical for: dc_qlookup[33] - Fail";
    FullPossibleRes[244] = "MD5 Checksums Identical for: dc_qlookup[34] - Fail";
    FullPossibleRes[245] = "MD5 Checksums Identical for: dc_qlookup[35] - Fail";
    FullPossibleRes[246] = "MD5 Checksums Identical for: dc_qlookup[36] - Fail";
    FullPossibleRes[247] = "MD5 Checksums Identical for: dc_qlookup[37] - Fail";
    FullPossibleRes[248] = "MD5 Checksums Identical for: dc_qlookup[38] - Fail";
    FullPossibleRes[249] = "MD5 Checksums Identical for: dc_qlookup[39] - Fail";
    FullPossibleRes[250] = "MD5 Checksums Identical for: dc_qlookup[40] - Fail";
    FullPossibleRes[251] = "MD5 Checksums Identical for: dc_qlookup[41] - Fail";
    FullPossibleRes[252] = "MD5 Checksums Identical for: dc_qlookup[42] - Fail";
    FullPossibleRes[253] = "MD5 Checksums Identical for: dc_qlookup[43] - Fail";
    FullPossibleRes[254] = "MD5 Checksums Identical for: dc_qlookup[44] - Fail";
    FullPossibleRes[255] = "MD5 Checksums Identical for: dc_qlookup[45] - Fail";
    FullPossibleRes[256] = "MD5 Checksums Identical for: dc_qlookup[46] - Fail";
    FullPossibleRes[257] = "MD5 Checksums Identical for: dc_qlookup[47] - Fail";
    FullPossibleRes[258] = "MD5 Checksums Identical for: dc_qlookup[48] - Fail";
    FullPossibleRes[259] = "MD5 Checksums Identical for: dc_qlookup[49] - Fail";
    FullPossibleRes[260] = "MD5 Checksums Identical for: dc_qlookup[50] - Fail";
    FullPossibleRes[261] = "MD5 Checksums Identical for: dc_qlookup[51] - Fail";
    FullPossibleRes[262] = "MD5 Checksums Identical for: dc_qlookup[52] - Fail";
    FullPossibleRes[263] = "MD5 Checksums Identical for: dc_qlookup[53] - Fail";
    FullPossibleRes[264] = "MD5 Checksums Identical for: dc_qlookup[54] - Fail";
    FullPossibleRes[265] = "MD5 Checksums Identical for: dc_qlookup[55] - Fail";
    FullPossibleRes[266] = "MD5 Checksums Identical for: dc_qlookup[56] - Fail";
    FullPossibleRes[267] = "MD5 Checksums Identical for: dc_qlookup[57] - Fail";
    FullPossibleRes[268] = "MD5 Checksums Identical for: dc_qlookup[58] - Fail";
    FullPossibleRes[269] = "MD5 Checksums Identical for: dc_qlookup[59] - Fail";
    FullPossibleRes[270] = "MD5 Checksums Identical for: dc_qlookup[60] - Fail";
    FullPossibleRes[271] = "MD5 Checksums Identical for: dc_qlookup[61] - Fail";
    FullPossibleRes[272] = "MD5 Checksums Identical for: dc_qlookup[62] - Fail";
    FullPossibleRes[273] = "MD5 Checksums Identical for: dc_qlookup[63] - Fail";
    FullPossibleRes[274] = "MD5 Checksums Identical for: dc_qlookup[64] - Fail";
    FullPossibleRes[275] = "MD5 Checksums Identical for: dc_qlookup[65] - Fail";
    FullPossibleRes[276] = "MD5 Checksums Identical for: dc_qlookup[66] - Fail";
    FullPossibleRes[277] = "MD5 Checksums Identical for: dc_qlookup[67] - Fail";
    FullPossibleRes[278] = "MD5 Checksums Identical for: dc_qlookup[68] - Fail";
    FullPossibleRes[279] = "MD5 Checksums Identical for: dc_qlookup[69] - Fail";
    FullPossibleRes[280] = "MD5 Checksums Identical for: dc_qlookup[70] - Fail";
    FullPossibleRes[281] = "MD5 Checksums Identical for: dc_qlookup[71] - Fail";
    FullPossibleRes[282] = "MD5 Checksums Identical for: dc_qlookup[72] - Fail";
    FullPossibleRes[283] = "MD5 Checksums Identical for: dc_qlookup[73] - Fail";
    FullPossibleRes[284] = "MD5 Checksums Identical for: dc_qlookup[74] - Fail";
    FullPossibleRes[285] = "MD5 Checksums Identical for: dc_qlookup[75] - Fail";
    FullPossibleRes[286] = "MD5 Checksums Identical for: dc_qlookup[76] - Fail";
    FullPossibleRes[287] = "MD5 Checksums Identical for: dc_qlookup[77] - Fail";
    FullPossibleRes[288] = "MD5 Checksums Identical for: dc_qlookup[78] - Fail";
    FullPossibleRes[289] = "MD5 Checksums Identical for: dc_qlookup[79] - Fail";
    FullPossibleRes[290] = "MD5 Checksums Identical for: dc_qlookup[80] - Fail";
    FullPossibleRes[291] = "MD5 Checksums Identical for: dc_qlookup[81] - Fail";
    FullPossibleRes[292] = "MD5 Checksums Identical for: dc_qlookup[82] - Fail";
    FullPossibleRes[293] = "MD5 Checksums Identical for: dc_qlookup[83] - Fail";
    FullPossibleRes[294] = "MD5 Checksums Identical for: dc_qlookup[84] - Fail";
    FullPossibleRes[295] = "MD5 Checksums Identical for: dc_qlookup[85] - Fail";
    FullPossibleRes[296] = "MD5 Checksums Identical for: dc_qlookup[86] - Fail";
    FullPossibleRes[297] = "MD5 Checksums Identical for: dc_qlookup[87] - Fail";
    FullPossibleRes[298] = "MD5 Checksums Identical for: dc_qlookup[88] - Fail";
    FullPossibleRes[299] = "MD5 Checksums Identical for: dc_qlookup[89] - Fail";
    FullPossibleRes[300] = "MD5 Checksums Identical for: dc_qlookup[90] - Fail";
    FullPossibleRes[301] = "MD5 Checksums Identical for: dc_qlookup[91] - Fail";
    FullPossibleRes[302] = "MD5 Checksums Identical for: dc_qlookup[92] - Fail";
    FullPossibleRes[303] = "MD5 Checksums Identical for: dc_qlookup[93] - Fail";
    FullPossibleRes[304] = "MD5 Checksums Identical for: dc_qlookup[94] - Fail";
    FullPossibleRes[305] = "MD5 Checksums Identical for: dc_qlookup[95] - Fail";
    FullPossibleRes[306] = "MD5 Checksums Identical for: dc_qlookup[96] - Fail";
    FullPossibleRes[307] = "MD5 Checksums Identical for: dc_qlookup[97] - Fail";
    FullPossibleRes[308] = "MD5 Checksums Identical for: dc_qlookup[98] - Fail";
    FullPossibleRes[309] = "MD5 Checksums Identical for: dc_qlookup[99] - Fail";
    FullPossibleRes[310] = "MD5 Checksums Identical for: dc_qlookup[100] - Fail";
    FullPossibleRes[311] = "MD5 Checksums Identical for: dc_qlookup[101] - Fail";
    FullPossibleRes[312] = "MD5 Checksums Identical for: dc_qlookup[102] - Fail";
    FullPossibleRes[313] = "MD5 Checksums Identical for: dc_qlookup[103] - Fail";
    FullPossibleRes[314] = "MD5 Checksums Identical for: dc_qlookup[104] - Fail";
    FullPossibleRes[315] = "MD5 Checksums Identical for: dc_qlookup[105] - Fail";
    FullPossibleRes[316] = "MD5 Checksums Identical for: dc_qlookup[106] - Fail";
    FullPossibleRes[317] = "MD5 Checksums Identical for: dc_qlookup[107] - Fail";
    FullPossibleRes[318] = "MD5 Checksums Identical for: dc_qlookup[108] - Fail";
    FullPossibleRes[319] = "MD5 Checksums Identical for: dc_qlookup[109] - Fail";
    FullPossibleRes[320] = "MD5 Checksums Identical for: dc_qlookup[110] - Fail";
    FullPossibleRes[321] = "MD5 Checksums Identical for: dc_qlookup[111] - Fail";
    FullPossibleRes[322] = "MD5 Checksums Identical for: dc_qlookup[112] - Fail";
    FullPossibleRes[323] = "MD5 Checksums Identical for: dc_qlookup[113] - Fail";
    FullPossibleRes[324] = "MD5 Checksums Identical for: dc_qlookup[114] - Fail";
    FullPossibleRes[325] = "MD5 Checksums Identical for: dc_qlookup[115] - Fail";
    FullPossibleRes[326] = "MD5 Checksums Identical for: dc_qlookup[116] - Fail";
    FullPossibleRes[327] = "MD5 Checksums Identical for: dc_qlookup[117] - Fail";
    FullPossibleRes[328] = "MD5 Checksums Identical for: dc_qlookup[118] - Fail";
    FullPossibleRes[329] = "MD5 Checksums Identical for: dc_qlookup[119] - Fail";
    FullPossibleRes[330] = "MD5 Checksums Identical for: dc_qlookup[120] - Fail";
    FullPossibleRes[331] = "MD5 Checksums Identical for: dc_qlookup[121] - Fail";
    FullPossibleRes[332] = "MD5 Checksums Identical for: dc_qlookup[122] - Fail";
    FullPossibleRes[333] = "MD5 Checksums Identical for: dc_qlookup[123] - Fail";
    FullPossibleRes[334] = "MD5 Checksums Identical for: dc_qlookup[124] - Fail";
    FullPossibleRes[335] = "MD5 Checksums Identical for: dc_qlookup[125] - Fail";
    FullPossibleRes[336] = "MD5 Checksums Identical for: dc_qlookup[126] - Fail";
    FullPossibleRes[337] = "MD5 Checksums Identical for: dc_qlookup[127] - Fail";
    FullPossibleRes[338] = "MD5 Checksums Identical for: vp8_default_mv_context[0] - Fail";
    FullPossibleRes[339] = "MD5 Checksums Identical for: vp8_default_mv_context[1] - Fail";
    FullPossibleRes[340] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[0] - Fail";
    FullPossibleRes[341] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[1] - Fail";
    FullPossibleRes[342] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[2] - Fail";
    FullPossibleRes[343] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[3] - Fail";
    FullPossibleRes[344] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[4] - Fail";
    FullPossibleRes[345] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[5] - Fail";
    FullPossibleRes[346] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[6] - Fail";
    FullPossibleRes[347] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[7] - Fail";
    FullPossibleRes[348] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[8] - Fail";
    FullPossibleRes[349] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[9] - Fail";
    FullPossibleRes[350] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[10] - Fail";
    FullPossibleRes[351] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[11] - Fail";
    FullPossibleRes[352] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[12] - Fail";
    FullPossibleRes[353] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[13] - Fail";
    FullPossibleRes[354] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[14] - Fail";
    FullPossibleRes[355] = "MD5 Checksums Identical for: vp8_default_zig_zag1d[15] - Fail";
    FullPossibleRes[356] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[0] - Fail";
    FullPossibleRes[357] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[1] - Fail";
    FullPossibleRes[358] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[2] - Fail";
    FullPossibleRes[359] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[3] - Fail";
    FullPossibleRes[360] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[4] - Fail";
    FullPossibleRes[361] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[5] - Fail";
    FullPossibleRes[362] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[6] - Fail";
    FullPossibleRes[363] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[7] - Fail";
    FullPossibleRes[364] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[8] - Fail";
    FullPossibleRes[365] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[9] - Fail";
    FullPossibleRes[366] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[10] - Fail";
    FullPossibleRes[367] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[11] - Fail";
    FullPossibleRes[368] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[12] - Fail";
    FullPossibleRes[369] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[13] - Fail";
    FullPossibleRes[370] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[14] - Fail";
    FullPossibleRes[371] = "MD5 Checksums Identical for: vp8_default_zig_zag_mask[15] - Fail";
    FullPossibleRes[372] = "MD5 Checksums Identical for: kernel5[0] - Fail";
    FullPossibleRes[373] = "MD5 Checksums Identical for: kernel5[1] - Fail";
    FullPossibleRes[374] = "MD5 Checksums Identical for: kernel5[2] - Fail";
    FullPossibleRes[375] = "MD5 Checksums Identical for: kernel5[3] - Fail";
    FullPossibleRes[376] = "MD5 Checksums Identical for: kernel5[4] - Fail";
    FullPossibleRes[377] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][0] - Fail";
    FullPossibleRes[378] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][0] - Fail";
    FullPossibleRes[379] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][0] - Fail";
    FullPossibleRes[380] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][0] - Fail";
    FullPossibleRes[381] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][0] - Fail";
    FullPossibleRes[382] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][0] - Fail";
    FullPossibleRes[383] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][0] - Fail";
    FullPossibleRes[384] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][0] - Fail";
    FullPossibleRes[385] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][0] - Fail";
    FullPossibleRes[386] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][0] - Fail";
    FullPossibleRes[387] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][0] - Fail";
    FullPossibleRes[388] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][0] - Fail";
    FullPossibleRes[389] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][0] - Fail";
    FullPossibleRes[390] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][0] - Fail";
    FullPossibleRes[391] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][0] - Fail";
    FullPossibleRes[392] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][0] - Fail";
    FullPossibleRes[393] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][0] - Fail";
    FullPossibleRes[394] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][0] - Fail";
    FullPossibleRes[395] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][0] - Fail";
    FullPossibleRes[396] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][0] - Fail";
    FullPossibleRes[397] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][0] - Fail";
    FullPossibleRes[398] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][0] - Fail";
    FullPossibleRes[399] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][0] - Fail";
    FullPossibleRes[400] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][0] - Fail";
    FullPossibleRes[401] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][0] - Fail";
    FullPossibleRes[402] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][0] - Fail";
    FullPossibleRes[403] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][0] - Fail";
    FullPossibleRes[404] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][0] - Fail";
    FullPossibleRes[405] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][0] - Fail";
    FullPossibleRes[406] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][0] - Fail";
    FullPossibleRes[407] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][0] - Fail";
    FullPossibleRes[408] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][0] - Fail";
    FullPossibleRes[409] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][0] - Fail";
    FullPossibleRes[410] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][0] - Fail";
    FullPossibleRes[411] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][0] - Fail";
    FullPossibleRes[412] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][0] - Fail";
    FullPossibleRes[413] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][0] - Fail";
    FullPossibleRes[414] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][0] - Fail";
    FullPossibleRes[415] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][0] - Fail";
    FullPossibleRes[416] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][0] - Fail";
    FullPossibleRes[417] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][0] - Fail";
    FullPossibleRes[418] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][0] - Fail";
    FullPossibleRes[419] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][0] - Fail";
    FullPossibleRes[420] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][0] - Fail";
    FullPossibleRes[421] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][0] - Fail";
    FullPossibleRes[422] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][0] - Fail";
    FullPossibleRes[423] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][0] - Fail";
    FullPossibleRes[424] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][0] - Fail";
    FullPossibleRes[425] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][0] - Fail";
    FullPossibleRes[426] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][0] - Fail";
    FullPossibleRes[427] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][0] - Fail";
    FullPossibleRes[428] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][0] - Fail";
    FullPossibleRes[429] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][0] - Fail";
    FullPossibleRes[430] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][0] - Fail";
    FullPossibleRes[431] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][0] - Fail";
    FullPossibleRes[432] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][0] - Fail";
    FullPossibleRes[433] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][0] - Fail";
    FullPossibleRes[434] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][0] - Fail";
    FullPossibleRes[435] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][0] - Fail";
    FullPossibleRes[436] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][0] - Fail";
    FullPossibleRes[437] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][0] - Fail";
    FullPossibleRes[438] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][0] - Fail";
    FullPossibleRes[439] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][0] - Fail";
    FullPossibleRes[440] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][0] - Fail";
    FullPossibleRes[441] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][0] - Fail";
    FullPossibleRes[442] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][0] - Fail";
    FullPossibleRes[443] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][0] - Fail";
    FullPossibleRes[444] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][0] - Fail";
    FullPossibleRes[445] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][0] - Fail";
    FullPossibleRes[446] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][0] - Fail";
    FullPossibleRes[447] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][0] - Fail";
    FullPossibleRes[448] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][0] - Fail";
    FullPossibleRes[449] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][0] - Fail";
    FullPossibleRes[450] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][0] - Fail";
    FullPossibleRes[451] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][0] - Fail";
    FullPossibleRes[452] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][0] - Fail";
    FullPossibleRes[453] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][0] - Fail";
    FullPossibleRes[454] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][0] - Fail";
    FullPossibleRes[455] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][0] - Fail";
    FullPossibleRes[456] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][0] - Fail";
    FullPossibleRes[457] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][0] - Fail";
    FullPossibleRes[458] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][0] - Fail";
    FullPossibleRes[459] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][0] - Fail";
    FullPossibleRes[460] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][0] - Fail";
    FullPossibleRes[461] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][0] - Fail";
    FullPossibleRes[462] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][0] - Fail";
    FullPossibleRes[463] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][0] - Fail";
    FullPossibleRes[464] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][0] - Fail";
    FullPossibleRes[465] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][0] - Fail";
    FullPossibleRes[466] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][0] - Fail";
    FullPossibleRes[467] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][0] - Fail";
    FullPossibleRes[468] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][0] - Fail";
    FullPossibleRes[469] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][0] - Fail";
    FullPossibleRes[470] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][0] - Fail";
    FullPossibleRes[471] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][0] - Fail";
    FullPossibleRes[472] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][0] - Fail";
    FullPossibleRes[473] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][0] - Fail";
    FullPossibleRes[474] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][0] - Fail";
    FullPossibleRes[475] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][0] - Fail";
    FullPossibleRes[476] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][0] - Fail";
    FullPossibleRes[477] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][1] - Fail";
    FullPossibleRes[478] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][1] - Fail";
    FullPossibleRes[479] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][1] - Fail";
    FullPossibleRes[480] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][1] - Fail";
    FullPossibleRes[481] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][1] - Fail";
    FullPossibleRes[482] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][1] - Fail";
    FullPossibleRes[483] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][1] - Fail";
    FullPossibleRes[484] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][1] - Fail";
    FullPossibleRes[485] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][1] - Fail";
    FullPossibleRes[486] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][1] - Fail";
    FullPossibleRes[487] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][1] - Fail";
    FullPossibleRes[488] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][1] - Fail";
    FullPossibleRes[489] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][1] - Fail";
    FullPossibleRes[490] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][1] - Fail";
    FullPossibleRes[491] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][1] - Fail";
    FullPossibleRes[492] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][1] - Fail";
    FullPossibleRes[493] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][1] - Fail";
    FullPossibleRes[494] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][1] - Fail";
    FullPossibleRes[495] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][1] - Fail";
    FullPossibleRes[496] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][1] - Fail";
    FullPossibleRes[497] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][1] - Fail";
    FullPossibleRes[498] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][1] - Fail";
    FullPossibleRes[499] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][1] - Fail";
    FullPossibleRes[500] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][1] - Fail";
    FullPossibleRes[501] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][1] - Fail";
    FullPossibleRes[502] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][1] - Fail";
    FullPossibleRes[503] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][1] - Fail";
    FullPossibleRes[504] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][1] - Fail";
    FullPossibleRes[505] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][1] - Fail";
    FullPossibleRes[506] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][1] - Fail";
    FullPossibleRes[507] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][1] - Fail";
    FullPossibleRes[508] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][1] - Fail";
    FullPossibleRes[509] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][1] - Fail";
    FullPossibleRes[510] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][1] - Fail";
    FullPossibleRes[511] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][1] - Fail";
    FullPossibleRes[512] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][1] - Fail";
    FullPossibleRes[513] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][1] - Fail";
    FullPossibleRes[514] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][1] - Fail";
    FullPossibleRes[515] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][1] - Fail";
    FullPossibleRes[516] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][1] - Fail";
    FullPossibleRes[517] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][1] - Fail";
    FullPossibleRes[518] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][1] - Fail";
    FullPossibleRes[519] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][1] - Fail";
    FullPossibleRes[520] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][1] - Fail";
    FullPossibleRes[521] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][1] - Fail";
    FullPossibleRes[522] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][1] - Fail";
    FullPossibleRes[523] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][1] - Fail";
    FullPossibleRes[524] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][1] - Fail";
    FullPossibleRes[525] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][1] - Fail";
    FullPossibleRes[526] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][1] - Fail";
    FullPossibleRes[527] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][1] - Fail";
    FullPossibleRes[528] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][1] - Fail";
    FullPossibleRes[529] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][1] - Fail";
    FullPossibleRes[530] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][1] - Fail";
    FullPossibleRes[531] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][1] - Fail";
    FullPossibleRes[532] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][1] - Fail";
    FullPossibleRes[533] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][1] - Fail";
    FullPossibleRes[534] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][1] - Fail";
    FullPossibleRes[535] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][1] - Fail";
    FullPossibleRes[536] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][1] - Fail";
    FullPossibleRes[537] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][1] - Fail";
    FullPossibleRes[538] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][1] - Fail";
    FullPossibleRes[539] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][1] - Fail";
    FullPossibleRes[540] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][1] - Fail";
    FullPossibleRes[541] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][1] - Fail";
    FullPossibleRes[542] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][1] - Fail";
    FullPossibleRes[543] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][1] - Fail";
    FullPossibleRes[544] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][1] - Fail";
    FullPossibleRes[545] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][1] - Fail";
    FullPossibleRes[546] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][1] - Fail";
    FullPossibleRes[547] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][1] - Fail";
    FullPossibleRes[548] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][1] - Fail";
    FullPossibleRes[549] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][1] - Fail";
    FullPossibleRes[550] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][1] - Fail";
    FullPossibleRes[551] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][1] - Fail";
    FullPossibleRes[552] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][1] - Fail";
    FullPossibleRes[553] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][1] - Fail";
    FullPossibleRes[554] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][1] - Fail";
    FullPossibleRes[555] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][1] - Fail";
    FullPossibleRes[556] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][1] - Fail";
    FullPossibleRes[557] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][1] - Fail";
    FullPossibleRes[558] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][1] - Fail";
    FullPossibleRes[559] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][1] - Fail";
    FullPossibleRes[560] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][1] - Fail";
    FullPossibleRes[561] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][1] - Fail";
    FullPossibleRes[562] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][1] - Fail";
    FullPossibleRes[563] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][1] - Fail";
    FullPossibleRes[564] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][1] - Fail";
    FullPossibleRes[565] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][1] - Fail";
    FullPossibleRes[566] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][1] - Fail";
    FullPossibleRes[567] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][1] - Fail";
    FullPossibleRes[568] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][1] - Fail";
    FullPossibleRes[569] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][1] - Fail";
    FullPossibleRes[570] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][1] - Fail";
    FullPossibleRes[571] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][1] - Fail";
    FullPossibleRes[572] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][1] - Fail";
    FullPossibleRes[573] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][1] - Fail";
    FullPossibleRes[574] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][1] - Fail";
    FullPossibleRes[575] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][1] - Fail";
    FullPossibleRes[576] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][1] - Fail";
    FullPossibleRes[577] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][2] - Fail";
    FullPossibleRes[578] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][2] - Fail";
    FullPossibleRes[579] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][2] - Fail";
    FullPossibleRes[580] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][2] - Fail";
    FullPossibleRes[581] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][2] - Fail";
    FullPossibleRes[582] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][2] - Fail";
    FullPossibleRes[583] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][2] - Fail";
    FullPossibleRes[584] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][2] - Fail";
    FullPossibleRes[585] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][2] - Fail";
    FullPossibleRes[586] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][2] - Fail";
    FullPossibleRes[587] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][2] - Fail";
    FullPossibleRes[588] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][2] - Fail";
    FullPossibleRes[589] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][2] - Fail";
    FullPossibleRes[590] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][2] - Fail";
    FullPossibleRes[591] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][2] - Fail";
    FullPossibleRes[592] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][2] - Fail";
    FullPossibleRes[593] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][2] - Fail";
    FullPossibleRes[594] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][2] - Fail";
    FullPossibleRes[595] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][2] - Fail";
    FullPossibleRes[596] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][2] - Fail";
    FullPossibleRes[597] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][2] - Fail";
    FullPossibleRes[598] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][2] - Fail";
    FullPossibleRes[599] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][2] - Fail";
    FullPossibleRes[600] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][2] - Fail";
    FullPossibleRes[601] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][2] - Fail";
    FullPossibleRes[602] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][2] - Fail";
    FullPossibleRes[603] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][2] - Fail";
    FullPossibleRes[604] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][2] - Fail";
    FullPossibleRes[605] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][2] - Fail";
    FullPossibleRes[606] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][2] - Fail";
    FullPossibleRes[607] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][2] - Fail";
    FullPossibleRes[608] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][2] - Fail";
    FullPossibleRes[609] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][2] - Fail";
    FullPossibleRes[610] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][2] - Fail";
    FullPossibleRes[611] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][2] - Fail";
    FullPossibleRes[612] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][2] - Fail";
    FullPossibleRes[613] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][2] - Fail";
    FullPossibleRes[614] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][2] - Fail";
    FullPossibleRes[615] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][2] - Fail";
    FullPossibleRes[616] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][2] - Fail";
    FullPossibleRes[617] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][2] - Fail";
    FullPossibleRes[618] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][2] - Fail";
    FullPossibleRes[619] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][2] - Fail";
    FullPossibleRes[620] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][2] - Fail";
    FullPossibleRes[621] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][2] - Fail";
    FullPossibleRes[622] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][2] - Fail";
    FullPossibleRes[623] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][2] - Fail";
    FullPossibleRes[624] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][2] - Fail";
    FullPossibleRes[625] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][2] - Fail";
    FullPossibleRes[626] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][2] - Fail";
    FullPossibleRes[627] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][2] - Fail";
    FullPossibleRes[628] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][2] - Fail";
    FullPossibleRes[629] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][2] - Fail";
    FullPossibleRes[630] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][2] - Fail";
    FullPossibleRes[631] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][2] - Fail";
    FullPossibleRes[632] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][2] - Fail";
    FullPossibleRes[633] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][2] - Fail";
    FullPossibleRes[634] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][2] - Fail";
    FullPossibleRes[635] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][2] - Fail";
    FullPossibleRes[636] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][2] - Fail";
    FullPossibleRes[637] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][2] - Fail";
    FullPossibleRes[638] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][2] - Fail";
    FullPossibleRes[639] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][2] - Fail";
    FullPossibleRes[640] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][2] - Fail";
    FullPossibleRes[641] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][2] - Fail";
    FullPossibleRes[642] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][2] - Fail";
    FullPossibleRes[643] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][2] - Fail";
    FullPossibleRes[644] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][2] - Fail";
    FullPossibleRes[645] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][2] - Fail";
    FullPossibleRes[646] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][2] - Fail";
    FullPossibleRes[647] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][2] - Fail";
    FullPossibleRes[648] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][2] - Fail";
    FullPossibleRes[649] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][2] - Fail";
    FullPossibleRes[650] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][2] - Fail";
    FullPossibleRes[651] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][2] - Fail";
    FullPossibleRes[652] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][2] - Fail";
    FullPossibleRes[653] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][2] - Fail";
    FullPossibleRes[654] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][2] - Fail";
    FullPossibleRes[655] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][2] - Fail";
    FullPossibleRes[656] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][2] - Fail";
    FullPossibleRes[657] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][2] - Fail";
    FullPossibleRes[658] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][2] - Fail";
    FullPossibleRes[659] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][2] - Fail";
    FullPossibleRes[660] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][2] - Fail";
    FullPossibleRes[661] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][2] - Fail";
    FullPossibleRes[662] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][2] - Fail";
    FullPossibleRes[663] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][2] - Fail";
    FullPossibleRes[664] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][2] - Fail";
    FullPossibleRes[665] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][2] - Fail";
    FullPossibleRes[666] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][2] - Fail";
    FullPossibleRes[667] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][2] - Fail";
    FullPossibleRes[668] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][2] - Fail";
    FullPossibleRes[669] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][2] - Fail";
    FullPossibleRes[670] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][2] - Fail";
    FullPossibleRes[671] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][2] - Fail";
    FullPossibleRes[672] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][2] - Fail";
    FullPossibleRes[673] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][2] - Fail";
    FullPossibleRes[674] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][2] - Fail";
    FullPossibleRes[675] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][2] - Fail";
    FullPossibleRes[676] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][2] - Fail";
    FullPossibleRes[677] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][3] - Fail";
    FullPossibleRes[678] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][3] - Fail";
    FullPossibleRes[679] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][3] - Fail";
    FullPossibleRes[680] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][3] - Fail";
    FullPossibleRes[681] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][3] - Fail";
    FullPossibleRes[682] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][3] - Fail";
    FullPossibleRes[683] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][3] - Fail";
    FullPossibleRes[684] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][3] - Fail";
    FullPossibleRes[685] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][3] - Fail";
    FullPossibleRes[686] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][3] - Fail";
    FullPossibleRes[687] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][3] - Fail";
    FullPossibleRes[688] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][3] - Fail";
    FullPossibleRes[689] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][3] - Fail";
    FullPossibleRes[690] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][3] - Fail";
    FullPossibleRes[691] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][3] - Fail";
    FullPossibleRes[692] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][3] - Fail";
    FullPossibleRes[693] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][3] - Fail";
    FullPossibleRes[694] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][3] - Fail";
    FullPossibleRes[695] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][3] - Fail";
    FullPossibleRes[696] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][3] - Fail";
    FullPossibleRes[697] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][3] - Fail";
    FullPossibleRes[698] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][3] - Fail";
    FullPossibleRes[699] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][3] - Fail";
    FullPossibleRes[700] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][3] - Fail";
    FullPossibleRes[701] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][3] - Fail";
    FullPossibleRes[702] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][3] - Fail";
    FullPossibleRes[703] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][3] - Fail";
    FullPossibleRes[704] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][3] - Fail";
    FullPossibleRes[705] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][3] - Fail";
    FullPossibleRes[706] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][3] - Fail";
    FullPossibleRes[707] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][3] - Fail";
    FullPossibleRes[708] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][3] - Fail";
    FullPossibleRes[709] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][3] - Fail";
    FullPossibleRes[710] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][3] - Fail";
    FullPossibleRes[711] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][3] - Fail";
    FullPossibleRes[712] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][3] - Fail";
    FullPossibleRes[713] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][3] - Fail";
    FullPossibleRes[714] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][3] - Fail";
    FullPossibleRes[715] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][3] - Fail";
    FullPossibleRes[716] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][3] - Fail";
    FullPossibleRes[717] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][3] - Fail";
    FullPossibleRes[718] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][3] - Fail";
    FullPossibleRes[719] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][3] - Fail";
    FullPossibleRes[720] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][3] - Fail";
    FullPossibleRes[721] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][3] - Fail";
    FullPossibleRes[722] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][3] - Fail";
    FullPossibleRes[723] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][3] - Fail";
    FullPossibleRes[724] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][3] - Fail";
    FullPossibleRes[725] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][3] - Fail";
    FullPossibleRes[726] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][3] - Fail";
    FullPossibleRes[727] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][3] - Fail";
    FullPossibleRes[728] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][3] - Fail";
    FullPossibleRes[729] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][3] - Fail";
    FullPossibleRes[730] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][3] - Fail";
    FullPossibleRes[731] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][3] - Fail";
    FullPossibleRes[732] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][3] - Fail";
    FullPossibleRes[733] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][3] - Fail";
    FullPossibleRes[734] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][3] - Fail";
    FullPossibleRes[735] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][3] - Fail";
    FullPossibleRes[736] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][3] - Fail";
    FullPossibleRes[737] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][3] - Fail";
    FullPossibleRes[738] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][3] - Fail";
    FullPossibleRes[739] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][3] - Fail";
    FullPossibleRes[740] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][3] - Fail";
    FullPossibleRes[741] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][3] - Fail";
    FullPossibleRes[742] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][3] - Fail";
    FullPossibleRes[743] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][3] - Fail";
    FullPossibleRes[744] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][3] - Fail";
    FullPossibleRes[745] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][3] - Fail";
    FullPossibleRes[746] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][3] - Fail";
    FullPossibleRes[747] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][3] - Fail";
    FullPossibleRes[748] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][3] - Fail";
    FullPossibleRes[749] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][3] - Fail";
    FullPossibleRes[750] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][3] - Fail";
    FullPossibleRes[751] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][3] - Fail";
    FullPossibleRes[752] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][3] - Fail";
    FullPossibleRes[753] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][3] - Fail";
    FullPossibleRes[754] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][3] - Fail";
    FullPossibleRes[755] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][3] - Fail";
    FullPossibleRes[756] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][3] - Fail";
    FullPossibleRes[757] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][3] - Fail";
    FullPossibleRes[758] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][3] - Fail";
    FullPossibleRes[759] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][3] - Fail";
    FullPossibleRes[760] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][3] - Fail";
    FullPossibleRes[761] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][3] - Fail";
    FullPossibleRes[762] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][3] - Fail";
    FullPossibleRes[763] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][3] - Fail";
    FullPossibleRes[764] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][3] - Fail";
    FullPossibleRes[765] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][3] - Fail";
    FullPossibleRes[766] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][3] - Fail";
    FullPossibleRes[767] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][3] - Fail";
    FullPossibleRes[768] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][3] - Fail";
    FullPossibleRes[769] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][3] - Fail";
    FullPossibleRes[770] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][3] - Fail";
    FullPossibleRes[771] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][3] - Fail";
    FullPossibleRes[772] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][3] - Fail";
    FullPossibleRes[773] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][3] - Fail";
    FullPossibleRes[774] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][3] - Fail";
    FullPossibleRes[775] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][3] - Fail";
    FullPossibleRes[776] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][3] - Fail";
    FullPossibleRes[777] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][4] - Fail";
    FullPossibleRes[778] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][4] - Fail";
    FullPossibleRes[779] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][4] - Fail";
    FullPossibleRes[780] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][4] - Fail";
    FullPossibleRes[781] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][4] - Fail";
    FullPossibleRes[782] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][4] - Fail";
    FullPossibleRes[783] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][4] - Fail";
    FullPossibleRes[784] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][4] - Fail";
    FullPossibleRes[785] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][4] - Fail";
    FullPossibleRes[786] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][4] - Fail";
    FullPossibleRes[787] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][4] - Fail";
    FullPossibleRes[788] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][4] - Fail";
    FullPossibleRes[789] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][4] - Fail";
    FullPossibleRes[790] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][4] - Fail";
    FullPossibleRes[791] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][4] - Fail";
    FullPossibleRes[792] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][4] - Fail";
    FullPossibleRes[793] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][4] - Fail";
    FullPossibleRes[794] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][4] - Fail";
    FullPossibleRes[795] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][4] - Fail";
    FullPossibleRes[796] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][4] - Fail";
    FullPossibleRes[797] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][4] - Fail";
    FullPossibleRes[798] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][4] - Fail";
    FullPossibleRes[799] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][4] - Fail";
    FullPossibleRes[800] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][4] - Fail";
    FullPossibleRes[801] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][4] - Fail";
    FullPossibleRes[802] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][4] - Fail";
    FullPossibleRes[803] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][4] - Fail";
    FullPossibleRes[804] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][4] - Fail";
    FullPossibleRes[805] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][4] - Fail";
    FullPossibleRes[806] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][4] - Fail";
    FullPossibleRes[807] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][4] - Fail";
    FullPossibleRes[808] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][4] - Fail";
    FullPossibleRes[809] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][4] - Fail";
    FullPossibleRes[810] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][4] - Fail";
    FullPossibleRes[811] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][4] - Fail";
    FullPossibleRes[812] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][4] - Fail";
    FullPossibleRes[813] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][4] - Fail";
    FullPossibleRes[814] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][4] - Fail";
    FullPossibleRes[815] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][4] - Fail";
    FullPossibleRes[816] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][4] - Fail";
    FullPossibleRes[817] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][4] - Fail";
    FullPossibleRes[818] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][4] - Fail";
    FullPossibleRes[819] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][4] - Fail";
    FullPossibleRes[820] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][4] - Fail";
    FullPossibleRes[821] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][4] - Fail";
    FullPossibleRes[822] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][4] - Fail";
    FullPossibleRes[823] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][4] - Fail";
    FullPossibleRes[824] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][4] - Fail";
    FullPossibleRes[825] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][4] - Fail";
    FullPossibleRes[826] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][4] - Fail";
    FullPossibleRes[827] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][4] - Fail";
    FullPossibleRes[828] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][4] - Fail";
    FullPossibleRes[829] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][4] - Fail";
    FullPossibleRes[830] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][4] - Fail";
    FullPossibleRes[831] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][4] - Fail";
    FullPossibleRes[832] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][4] - Fail";
    FullPossibleRes[833] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][4] - Fail";
    FullPossibleRes[834] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][4] - Fail";
    FullPossibleRes[835] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][4] - Fail";
    FullPossibleRes[836] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][4] - Fail";
    FullPossibleRes[837] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][4] - Fail";
    FullPossibleRes[838] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][4] - Fail";
    FullPossibleRes[839] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][4] - Fail";
    FullPossibleRes[840] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][4] - Fail";
    FullPossibleRes[841] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][4] - Fail";
    FullPossibleRes[842] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][4] - Fail";
    FullPossibleRes[843] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][4] - Fail";
    FullPossibleRes[844] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][4] - Fail";
    FullPossibleRes[845] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][4] - Fail";
    FullPossibleRes[846] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][4] - Fail";
    FullPossibleRes[847] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][4] - Fail";
    FullPossibleRes[848] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][4] - Fail";
    FullPossibleRes[849] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][4] - Fail";
    FullPossibleRes[850] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][4] - Fail";
    FullPossibleRes[851] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][4] - Fail";
    FullPossibleRes[852] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][4] - Fail";
    FullPossibleRes[853] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][4] - Fail";
    FullPossibleRes[854] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][4] - Fail";
    FullPossibleRes[855] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][4] - Fail";
    FullPossibleRes[856] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][4] - Fail";
    FullPossibleRes[857] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][4] - Fail";
    FullPossibleRes[858] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][4] - Fail";
    FullPossibleRes[859] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][4] - Fail";
    FullPossibleRes[860] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][4] - Fail";
    FullPossibleRes[861] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][4] - Fail";
    FullPossibleRes[862] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][4] - Fail";
    FullPossibleRes[863] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][4] - Fail";
    FullPossibleRes[864] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][4] - Fail";
    FullPossibleRes[865] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][4] - Fail";
    FullPossibleRes[866] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][4] - Fail";
    FullPossibleRes[867] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][4] - Fail";
    FullPossibleRes[868] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][4] - Fail";
    FullPossibleRes[869] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][4] - Fail";
    FullPossibleRes[870] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][4] - Fail";
    FullPossibleRes[871] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][4] - Fail";
    FullPossibleRes[872] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][4] - Fail";
    FullPossibleRes[873] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][4] - Fail";
    FullPossibleRes[874] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][4] - Fail";
    FullPossibleRes[875] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][4] - Fail";
    FullPossibleRes[876] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][4] - Fail";
    FullPossibleRes[877] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][5] - Fail";
    FullPossibleRes[878] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][5] - Fail";
    FullPossibleRes[879] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][5] - Fail";
    FullPossibleRes[880] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][5] - Fail";
    FullPossibleRes[881] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][5] - Fail";
    FullPossibleRes[882] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][5] - Fail";
    FullPossibleRes[883] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][5] - Fail";
    FullPossibleRes[884] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][5] - Fail";
    FullPossibleRes[885] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][5] - Fail";
    FullPossibleRes[886] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][5] - Fail";
    FullPossibleRes[887] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][5] - Fail";
    FullPossibleRes[888] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][5] - Fail";
    FullPossibleRes[889] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][5] - Fail";
    FullPossibleRes[890] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][5] - Fail";
    FullPossibleRes[891] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][5] - Fail";
    FullPossibleRes[892] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][5] - Fail";
    FullPossibleRes[893] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][5] - Fail";
    FullPossibleRes[894] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][5] - Fail";
    FullPossibleRes[895] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][5] - Fail";
    FullPossibleRes[896] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][5] - Fail";
    FullPossibleRes[897] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][5] - Fail";
    FullPossibleRes[898] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][5] - Fail";
    FullPossibleRes[899] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][5] - Fail";
    FullPossibleRes[900] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][5] - Fail";
    FullPossibleRes[901] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][5] - Fail";
    FullPossibleRes[902] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][5] - Fail";
    FullPossibleRes[903] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][5] - Fail";
    FullPossibleRes[904] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][5] - Fail";
    FullPossibleRes[905] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][5] - Fail";
    FullPossibleRes[906] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][5] - Fail";
    FullPossibleRes[907] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][5] - Fail";
    FullPossibleRes[908] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][5] - Fail";
    FullPossibleRes[909] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][5] - Fail";
    FullPossibleRes[910] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][5] - Fail";
    FullPossibleRes[911] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][5] - Fail";
    FullPossibleRes[912] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][5] - Fail";
    FullPossibleRes[913] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][5] - Fail";
    FullPossibleRes[914] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][5] - Fail";
    FullPossibleRes[915] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][5] - Fail";
    FullPossibleRes[916] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][5] - Fail";
    FullPossibleRes[917] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][5] - Fail";
    FullPossibleRes[918] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][5] - Fail";
    FullPossibleRes[919] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][5] - Fail";
    FullPossibleRes[920] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][5] - Fail";
    FullPossibleRes[921] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][5] - Fail";
    FullPossibleRes[922] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][5] - Fail";
    FullPossibleRes[923] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][5] - Fail";
    FullPossibleRes[924] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][5] - Fail";
    FullPossibleRes[925] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][5] - Fail";
    FullPossibleRes[926] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][5] - Fail";
    FullPossibleRes[927] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][5] - Fail";
    FullPossibleRes[928] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][5] - Fail";
    FullPossibleRes[929] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][5] - Fail";
    FullPossibleRes[930] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][5] - Fail";
    FullPossibleRes[931] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][5] - Fail";
    FullPossibleRes[932] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][5] - Fail";
    FullPossibleRes[933] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][5] - Fail";
    FullPossibleRes[934] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][5] - Fail";
    FullPossibleRes[935] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][5] - Fail";
    FullPossibleRes[936] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][5] - Fail";
    FullPossibleRes[937] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][5] - Fail";
    FullPossibleRes[938] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][5] - Fail";
    FullPossibleRes[939] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][5] - Fail";
    FullPossibleRes[940] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][5] - Fail";
    FullPossibleRes[941] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][5] - Fail";
    FullPossibleRes[942] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][5] - Fail";
    FullPossibleRes[943] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][5] - Fail";
    FullPossibleRes[944] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][5] - Fail";
    FullPossibleRes[945] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][5] - Fail";
    FullPossibleRes[946] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][5] - Fail";
    FullPossibleRes[947] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][5] - Fail";
    FullPossibleRes[948] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][5] - Fail";
    FullPossibleRes[949] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][5] - Fail";
    FullPossibleRes[950] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][5] - Fail";
    FullPossibleRes[951] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][5] - Fail";
    FullPossibleRes[952] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][5] - Fail";
    FullPossibleRes[953] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][5] - Fail";
    FullPossibleRes[954] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][5] - Fail";
    FullPossibleRes[955] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][5] - Fail";
    FullPossibleRes[956] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][5] - Fail";
    FullPossibleRes[957] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][5] - Fail";
    FullPossibleRes[958] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][5] - Fail";
    FullPossibleRes[959] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][5] - Fail";
    FullPossibleRes[960] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][5] - Fail";
    FullPossibleRes[961] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][5] - Fail";
    FullPossibleRes[962] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][5] - Fail";
    FullPossibleRes[963] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][5] - Fail";
    FullPossibleRes[964] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][5] - Fail";
    FullPossibleRes[965] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][5] - Fail";
    FullPossibleRes[966] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][5] - Fail";
    FullPossibleRes[967] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][5] - Fail";
    FullPossibleRes[968] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][5] - Fail";
    FullPossibleRes[969] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][5] - Fail";
    FullPossibleRes[970] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][5] - Fail";
    FullPossibleRes[971] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][5] - Fail";
    FullPossibleRes[972] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][5] - Fail";
    FullPossibleRes[973] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][5] - Fail";
    FullPossibleRes[974] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][5] - Fail";
    FullPossibleRes[975] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][5] - Fail";
    FullPossibleRes[976] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][5] - Fail";
    FullPossibleRes[977] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][6] - Fail";
    FullPossibleRes[978] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][6] - Fail";
    FullPossibleRes[979] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][6] - Fail";
    FullPossibleRes[980] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][6] - Fail";
    FullPossibleRes[981] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][6] - Fail";
    FullPossibleRes[982] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][6] - Fail";
    FullPossibleRes[983] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][6] - Fail";
    FullPossibleRes[984] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][6] - Fail";
    FullPossibleRes[985] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][6] - Fail";
    FullPossibleRes[986] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][6] - Fail";
    FullPossibleRes[987] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][6] - Fail";
    FullPossibleRes[988] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][6] - Fail";
    FullPossibleRes[989] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][6] - Fail";
    FullPossibleRes[990] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][6] - Fail";
    FullPossibleRes[991] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][6] - Fail";
    FullPossibleRes[992] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][6] - Fail";
    FullPossibleRes[993] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][6] - Fail";
    FullPossibleRes[994] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][6] - Fail";
    FullPossibleRes[995] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][6] - Fail";
    FullPossibleRes[996] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][6] - Fail";
    FullPossibleRes[997] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][6] - Fail";
    FullPossibleRes[998] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][6] - Fail";
    FullPossibleRes[999] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][6] - Fail";
    FullPossibleRes[1000] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][6] - Fail";
    FullPossibleRes[1001] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][6] - Fail";
    FullPossibleRes[1002] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][6] - Fail";
    FullPossibleRes[1003] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][6] - Fail";
    FullPossibleRes[1004] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][6] - Fail";
    FullPossibleRes[1005] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][6] - Fail";
    FullPossibleRes[1006] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][6] - Fail";
    FullPossibleRes[1007] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][6] - Fail";
    FullPossibleRes[1008] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][6] - Fail";
    FullPossibleRes[1009] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][6] - Fail";
    FullPossibleRes[1010] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][6] - Fail";
    FullPossibleRes[1011] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][6] - Fail";
    FullPossibleRes[1012] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][6] - Fail";
    FullPossibleRes[1013] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][6] - Fail";
    FullPossibleRes[1014] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][6] - Fail";
    FullPossibleRes[1015] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][6] - Fail";
    FullPossibleRes[1016] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][6] - Fail";
    FullPossibleRes[1017] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][6] - Fail";
    FullPossibleRes[1018] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][6] - Fail";
    FullPossibleRes[1019] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][6] - Fail";
    FullPossibleRes[1020] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][6] - Fail";
    FullPossibleRes[1021] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][6] - Fail";
    FullPossibleRes[1022] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][6] - Fail";
    FullPossibleRes[1023] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][6] - Fail";
    FullPossibleRes[1024] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][6] - Fail";
    FullPossibleRes[1025] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][6] - Fail";
    FullPossibleRes[1026] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][6] - Fail";
    FullPossibleRes[1027] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][6] - Fail";
    FullPossibleRes[1028] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][6] - Fail";
    FullPossibleRes[1029] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][6] - Fail";
    FullPossibleRes[1030] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][6] - Fail";
    FullPossibleRes[1031] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][6] - Fail";
    FullPossibleRes[1032] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][6] - Fail";
    FullPossibleRes[1033] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][6] - Fail";
    FullPossibleRes[1034] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][6] - Fail";
    FullPossibleRes[1035] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][6] - Fail";
    FullPossibleRes[1036] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][6] - Fail";
    FullPossibleRes[1037] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][6] - Fail";
    FullPossibleRes[1038] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][6] - Fail";
    FullPossibleRes[1039] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][6] - Fail";
    FullPossibleRes[1040] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][6] - Fail";
    FullPossibleRes[1041] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][6] - Fail";
    FullPossibleRes[1042] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][6] - Fail";
    FullPossibleRes[1043] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][6] - Fail";
    FullPossibleRes[1044] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][6] - Fail";
    FullPossibleRes[1045] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][6] - Fail";
    FullPossibleRes[1046] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][6] - Fail";
    FullPossibleRes[1047] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][6] - Fail";
    FullPossibleRes[1048] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][6] - Fail";
    FullPossibleRes[1049] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][6] - Fail";
    FullPossibleRes[1050] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][6] - Fail";
    FullPossibleRes[1051] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][6] - Fail";
    FullPossibleRes[1052] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][6] - Fail";
    FullPossibleRes[1053] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][6] - Fail";
    FullPossibleRes[1054] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][6] - Fail";
    FullPossibleRes[1055] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][6] - Fail";
    FullPossibleRes[1056] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][6] - Fail";
    FullPossibleRes[1057] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][6] - Fail";
    FullPossibleRes[1058] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][6] - Fail";
    FullPossibleRes[1059] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][6] - Fail";
    FullPossibleRes[1060] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][6] - Fail";
    FullPossibleRes[1061] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][6] - Fail";
    FullPossibleRes[1062] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][6] - Fail";
    FullPossibleRes[1063] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][6] - Fail";
    FullPossibleRes[1064] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][6] - Fail";
    FullPossibleRes[1065] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][6] - Fail";
    FullPossibleRes[1066] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][6] - Fail";
    FullPossibleRes[1067] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][6] - Fail";
    FullPossibleRes[1068] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][6] - Fail";
    FullPossibleRes[1069] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][6] - Fail";
    FullPossibleRes[1070] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][6] - Fail";
    FullPossibleRes[1071] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][6] - Fail";
    FullPossibleRes[1072] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][6] - Fail";
    FullPossibleRes[1073] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][6] - Fail";
    FullPossibleRes[1074] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][6] - Fail";
    FullPossibleRes[1075] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][6] - Fail";
    FullPossibleRes[1076] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][6] - Fail";
    FullPossibleRes[1077] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][7] - Fail";
    FullPossibleRes[1078] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][7] - Fail";
    FullPossibleRes[1079] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][7] - Fail";
    FullPossibleRes[1080] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][7] - Fail";
    FullPossibleRes[1081] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][7] - Fail";
    FullPossibleRes[1082] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][7] - Fail";
    FullPossibleRes[1083] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][7] - Fail";
    FullPossibleRes[1084] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][7] - Fail";
    FullPossibleRes[1085] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][7] - Fail";
    FullPossibleRes[1086] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][7] - Fail";
    FullPossibleRes[1087] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][7] - Fail";
    FullPossibleRes[1088] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][7] - Fail";
    FullPossibleRes[1089] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][7] - Fail";
    FullPossibleRes[1090] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][7] - Fail";
    FullPossibleRes[1091] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][7] - Fail";
    FullPossibleRes[1092] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][7] - Fail";
    FullPossibleRes[1093] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][7] - Fail";
    FullPossibleRes[1094] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][7] - Fail";
    FullPossibleRes[1095] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][7] - Fail";
    FullPossibleRes[1096] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][7] - Fail";
    FullPossibleRes[1097] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][7] - Fail";
    FullPossibleRes[1098] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][7] - Fail";
    FullPossibleRes[1099] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][7] - Fail";
    FullPossibleRes[1100] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][7] - Fail";
    FullPossibleRes[1101] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][7] - Fail";
    FullPossibleRes[1102] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][7] - Fail";
    FullPossibleRes[1103] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][7] - Fail";
    FullPossibleRes[1104] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][7] - Fail";
    FullPossibleRes[1105] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][7] - Fail";
    FullPossibleRes[1106] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][7] - Fail";
    FullPossibleRes[1107] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][7] - Fail";
    FullPossibleRes[1108] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][7] - Fail";
    FullPossibleRes[1109] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][7] - Fail";
    FullPossibleRes[1110] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][7] - Fail";
    FullPossibleRes[1111] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][7] - Fail";
    FullPossibleRes[1112] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][7] - Fail";
    FullPossibleRes[1113] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][7] - Fail";
    FullPossibleRes[1114] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][7] - Fail";
    FullPossibleRes[1115] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][7] - Fail";
    FullPossibleRes[1116] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][7] - Fail";
    FullPossibleRes[1117] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][7] - Fail";
    FullPossibleRes[1118] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][7] - Fail";
    FullPossibleRes[1119] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][7] - Fail";
    FullPossibleRes[1120] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][7] - Fail";
    FullPossibleRes[1121] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][7] - Fail";
    FullPossibleRes[1122] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][7] - Fail";
    FullPossibleRes[1123] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][7] - Fail";
    FullPossibleRes[1124] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][7] - Fail";
    FullPossibleRes[1125] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][7] - Fail";
    FullPossibleRes[1126] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][7] - Fail";
    FullPossibleRes[1127] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][7] - Fail";
    FullPossibleRes[1128] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][7] - Fail";
    FullPossibleRes[1129] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][7] - Fail";
    FullPossibleRes[1130] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][7] - Fail";
    FullPossibleRes[1131] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][7] - Fail";
    FullPossibleRes[1132] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][7] - Fail";
    FullPossibleRes[1133] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][7] - Fail";
    FullPossibleRes[1134] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][7] - Fail";
    FullPossibleRes[1135] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][7] - Fail";
    FullPossibleRes[1136] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][7] - Fail";
    FullPossibleRes[1137] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][7] - Fail";
    FullPossibleRes[1138] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][7] - Fail";
    FullPossibleRes[1139] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][7] - Fail";
    FullPossibleRes[1140] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][7] - Fail";
    FullPossibleRes[1141] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][7] - Fail";
    FullPossibleRes[1142] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][7] - Fail";
    FullPossibleRes[1143] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][7] - Fail";
    FullPossibleRes[1144] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][7] - Fail";
    FullPossibleRes[1145] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][7] - Fail";
    FullPossibleRes[1146] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][7] - Fail";
    FullPossibleRes[1147] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][7] - Fail";
    FullPossibleRes[1148] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][7] - Fail";
    FullPossibleRes[1149] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][7] - Fail";
    FullPossibleRes[1150] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][7] - Fail";
    FullPossibleRes[1151] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][7] - Fail";
    FullPossibleRes[1152] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][7] - Fail";
    FullPossibleRes[1153] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][7] - Fail";
    FullPossibleRes[1154] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][7] - Fail";
    FullPossibleRes[1155] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][7] - Fail";
    FullPossibleRes[1156] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][7] - Fail";
    FullPossibleRes[1157] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][7] - Fail";
    FullPossibleRes[1158] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][7] - Fail";
    FullPossibleRes[1159] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][7] - Fail";
    FullPossibleRes[1160] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][7] - Fail";
    FullPossibleRes[1161] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][7] - Fail";
    FullPossibleRes[1162] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][7] - Fail";
    FullPossibleRes[1163] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][7] - Fail";
    FullPossibleRes[1164] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][7] - Fail";
    FullPossibleRes[1165] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][7] - Fail";
    FullPossibleRes[1166] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][7] - Fail";
    FullPossibleRes[1167] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][7] - Fail";
    FullPossibleRes[1168] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][7] - Fail";
    FullPossibleRes[1169] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][7] - Fail";
    FullPossibleRes[1170] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][7] - Fail";
    FullPossibleRes[1171] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][7] - Fail";
    FullPossibleRes[1172] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][7] - Fail";
    FullPossibleRes[1173] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][7] - Fail";
    FullPossibleRes[1174] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][7] - Fail";
    FullPossibleRes[1175] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][7] - Fail";
    FullPossibleRes[1176] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][7] - Fail";
    FullPossibleRes[1177] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][8] - Fail";
    FullPossibleRes[1178] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][8] - Fail";
    FullPossibleRes[1179] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][8] - Fail";
    FullPossibleRes[1180] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][8] - Fail";
    FullPossibleRes[1181] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][8] - Fail";
    FullPossibleRes[1182] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][8] - Fail";
    FullPossibleRes[1183] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][8] - Fail";
    FullPossibleRes[1184] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][8] - Fail";
    FullPossibleRes[1185] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][8] - Fail";
    FullPossibleRes[1186] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][8] - Fail";
    FullPossibleRes[1187] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][8] - Fail";
    FullPossibleRes[1188] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][8] - Fail";
    FullPossibleRes[1189] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][8] - Fail";
    FullPossibleRes[1190] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][8] - Fail";
    FullPossibleRes[1191] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][8] - Fail";
    FullPossibleRes[1192] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][8] - Fail";
    FullPossibleRes[1193] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][8] - Fail";
    FullPossibleRes[1194] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][8] - Fail";
    FullPossibleRes[1195] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][8] - Fail";
    FullPossibleRes[1196] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][8] - Fail";
    FullPossibleRes[1197] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][8] - Fail";
    FullPossibleRes[1198] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][8] - Fail";
    FullPossibleRes[1199] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][8] - Fail";
    FullPossibleRes[1200] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][8] - Fail";
    FullPossibleRes[1201] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][8] - Fail";
    FullPossibleRes[1202] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][8] - Fail";
    FullPossibleRes[1203] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][8] - Fail";
    FullPossibleRes[1204] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][8] - Fail";
    FullPossibleRes[1205] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][8] - Fail";
    FullPossibleRes[1206] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][8] - Fail";
    FullPossibleRes[1207] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][8] - Fail";
    FullPossibleRes[1208] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][8] - Fail";
    FullPossibleRes[1209] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][8] - Fail";
    FullPossibleRes[1210] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][8] - Fail";
    FullPossibleRes[1211] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][8] - Fail";
    FullPossibleRes[1212] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][8] - Fail";
    FullPossibleRes[1213] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][8] - Fail";
    FullPossibleRes[1214] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][8] - Fail";
    FullPossibleRes[1215] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][8] - Fail";
    FullPossibleRes[1216] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][8] - Fail";
    FullPossibleRes[1217] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][8] - Fail";
    FullPossibleRes[1218] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][8] - Fail";
    FullPossibleRes[1219] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][8] - Fail";
    FullPossibleRes[1220] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][8] - Fail";
    FullPossibleRes[1221] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][8] - Fail";
    FullPossibleRes[1222] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][8] - Fail";
    FullPossibleRes[1223] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][8] - Fail";
    FullPossibleRes[1224] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][8] - Fail";
    FullPossibleRes[1225] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][8] - Fail";
    FullPossibleRes[1226] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][8] - Fail";
    FullPossibleRes[1227] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][8] - Fail";
    FullPossibleRes[1228] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][8] - Fail";
    FullPossibleRes[1229] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][8] - Fail";
    FullPossibleRes[1230] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][8] - Fail";
    FullPossibleRes[1231] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][8] - Fail";
    FullPossibleRes[1232] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][8] - Fail";
    FullPossibleRes[1233] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][8] - Fail";
    FullPossibleRes[1234] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][8] - Fail";
    FullPossibleRes[1235] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][8] - Fail";
    FullPossibleRes[1236] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][8] - Fail";
    FullPossibleRes[1237] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][8] - Fail";
    FullPossibleRes[1238] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][8] - Fail";
    FullPossibleRes[1239] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][8] - Fail";
    FullPossibleRes[1240] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][8] - Fail";
    FullPossibleRes[1241] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][8] - Fail";
    FullPossibleRes[1242] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][8] - Fail";
    FullPossibleRes[1243] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][8] - Fail";
    FullPossibleRes[1244] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][8] - Fail";
    FullPossibleRes[1245] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][8] - Fail";
    FullPossibleRes[1246] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][8] - Fail";
    FullPossibleRes[1247] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][8] - Fail";
    FullPossibleRes[1248] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][8] - Fail";
    FullPossibleRes[1249] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][8] - Fail";
    FullPossibleRes[1250] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][8] - Fail";
    FullPossibleRes[1251] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][8] - Fail";
    FullPossibleRes[1252] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][8] - Fail";
    FullPossibleRes[1253] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][8] - Fail";
    FullPossibleRes[1254] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][8] - Fail";
    FullPossibleRes[1255] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][8] - Fail";
    FullPossibleRes[1256] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][8] - Fail";
    FullPossibleRes[1257] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][8] - Fail";
    FullPossibleRes[1258] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][8] - Fail";
    FullPossibleRes[1259] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][8] - Fail";
    FullPossibleRes[1260] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][8] - Fail";
    FullPossibleRes[1261] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][8] - Fail";
    FullPossibleRes[1262] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][8] - Fail";
    FullPossibleRes[1263] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][8] - Fail";
    FullPossibleRes[1264] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][8] - Fail";
    FullPossibleRes[1265] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][8] - Fail";
    FullPossibleRes[1266] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][8] - Fail";
    FullPossibleRes[1267] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][8] - Fail";
    FullPossibleRes[1268] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][8] - Fail";
    FullPossibleRes[1269] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][8] - Fail";
    FullPossibleRes[1270] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][8] - Fail";
    FullPossibleRes[1271] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][8] - Fail";
    FullPossibleRes[1272] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][8] - Fail";
    FullPossibleRes[1273] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][8] - Fail";
    FullPossibleRes[1274] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][8] - Fail";
    FullPossibleRes[1275] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][8] - Fail";
    FullPossibleRes[1276] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][8] - Fail";
    FullPossibleRes[1277] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][0][9] - Fail";
    FullPossibleRes[1278] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][0][9] - Fail";
    FullPossibleRes[1279] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][0][9] - Fail";
    FullPossibleRes[1280] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][0][9] - Fail";
    FullPossibleRes[1281] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][0][9] - Fail";
    FullPossibleRes[1282] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][0][9] - Fail";
    FullPossibleRes[1283] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][0][9] - Fail";
    FullPossibleRes[1284] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][0][9] - Fail";
    FullPossibleRes[1285] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][0][9] - Fail";
    FullPossibleRes[1286] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][0][9] - Fail";
    FullPossibleRes[1287] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][1][9] - Fail";
    FullPossibleRes[1288] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][1][9] - Fail";
    FullPossibleRes[1289] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][1][9] - Fail";
    FullPossibleRes[1290] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][1][9] - Fail";
    FullPossibleRes[1291] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][1][9] - Fail";
    FullPossibleRes[1292] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][1][9] - Fail";
    FullPossibleRes[1293] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][1][9] - Fail";
    FullPossibleRes[1294] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][1][9] - Fail";
    FullPossibleRes[1295] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][1][9] - Fail";
    FullPossibleRes[1296] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][1][9] - Fail";
    FullPossibleRes[1297] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][2][9] - Fail";
    FullPossibleRes[1298] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][2][9] - Fail";
    FullPossibleRes[1299] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][2][9] - Fail";
    FullPossibleRes[1300] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][2][9] - Fail";
    FullPossibleRes[1301] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][2][9] - Fail";
    FullPossibleRes[1302] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][2][9] - Fail";
    FullPossibleRes[1303] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][2][9] - Fail";
    FullPossibleRes[1304] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][2][9] - Fail";
    FullPossibleRes[1305] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][2][9] - Fail";
    FullPossibleRes[1306] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][2][9] - Fail";
    FullPossibleRes[1307] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][3][9] - Fail";
    FullPossibleRes[1308] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][3][9] - Fail";
    FullPossibleRes[1309] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][3][9] - Fail";
    FullPossibleRes[1310] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][3][9] - Fail";
    FullPossibleRes[1311] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][3][9] - Fail";
    FullPossibleRes[1312] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][3][9] - Fail";
    FullPossibleRes[1313] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][3][9] - Fail";
    FullPossibleRes[1314] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][3][9] - Fail";
    FullPossibleRes[1315] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][3][9] - Fail";
    FullPossibleRes[1316] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][3][9] - Fail";
    FullPossibleRes[1317] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][4][9] - Fail";
    FullPossibleRes[1318] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][4][9] - Fail";
    FullPossibleRes[1319] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][4][9] - Fail";
    FullPossibleRes[1320] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][4][9] - Fail";
    FullPossibleRes[1321] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][4][9] - Fail";
    FullPossibleRes[1322] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][4][9] - Fail";
    FullPossibleRes[1323] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][4][9] - Fail";
    FullPossibleRes[1324] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][4][9] - Fail";
    FullPossibleRes[1325] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][4][9] - Fail";
    FullPossibleRes[1326] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][4][9] - Fail";
    FullPossibleRes[1327] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][5][9] - Fail";
    FullPossibleRes[1328] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][5][9] - Fail";
    FullPossibleRes[1329] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][5][9] - Fail";
    FullPossibleRes[1330] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][5][9] - Fail";
    FullPossibleRes[1331] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][5][9] - Fail";
    FullPossibleRes[1332] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][5][9] - Fail";
    FullPossibleRes[1333] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][5][9] - Fail";
    FullPossibleRes[1334] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][5][9] - Fail";
    FullPossibleRes[1335] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][5][9] - Fail";
    FullPossibleRes[1336] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][5][9] - Fail";
    FullPossibleRes[1337] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][6][9] - Fail";
    FullPossibleRes[1338] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][6][9] - Fail";
    FullPossibleRes[1339] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][6][9] - Fail";
    FullPossibleRes[1340] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][6][9] - Fail";
    FullPossibleRes[1341] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][6][9] - Fail";
    FullPossibleRes[1342] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][6][9] - Fail";
    FullPossibleRes[1343] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][6][9] - Fail";
    FullPossibleRes[1344] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][6][9] - Fail";
    FullPossibleRes[1345] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][6][9] - Fail";
    FullPossibleRes[1346] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][6][9] - Fail";
    FullPossibleRes[1347] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][7][9] - Fail";
    FullPossibleRes[1348] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][7][9] - Fail";
    FullPossibleRes[1349] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][7][9] - Fail";
    FullPossibleRes[1350] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][7][9] - Fail";
    FullPossibleRes[1351] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][7][9] - Fail";
    FullPossibleRes[1352] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][7][9] - Fail";
    FullPossibleRes[1353] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][7][9] - Fail";
    FullPossibleRes[1354] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][7][9] - Fail";
    FullPossibleRes[1355] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][7][9] - Fail";
    FullPossibleRes[1356] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][7][9] - Fail";
    FullPossibleRes[1357] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][8][9] - Fail";
    FullPossibleRes[1358] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][8][9] - Fail";
    FullPossibleRes[1359] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][8][9] - Fail";
    FullPossibleRes[1360] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][8][9] - Fail";
    FullPossibleRes[1361] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][8][9] - Fail";
    FullPossibleRes[1362] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][8][9] - Fail";
    FullPossibleRes[1363] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][8][9] - Fail";
    FullPossibleRes[1364] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][8][9] - Fail";
    FullPossibleRes[1365] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][8][9] - Fail";
    FullPossibleRes[1366] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][8][9] - Fail";
    FullPossibleRes[1367] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[0][9][9] - Fail";
    FullPossibleRes[1368] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[1][9][9] - Fail";
    FullPossibleRes[1369] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[2][9][9] - Fail";
    FullPossibleRes[1370] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[3][9][9] - Fail";
    FullPossibleRes[1371] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[4][9][9] - Fail";
    FullPossibleRes[1372] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[5][9][9] - Fail";
    FullPossibleRes[1373] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[6][9][9] - Fail";
    FullPossibleRes[1374] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[7][9][9] - Fail";
    FullPossibleRes[1375] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[8][9][9] - Fail";
    FullPossibleRes[1376] = "MD5 Checksums Identical for: vp8_kf_default_bmode_counts[9][9][9] - Fail";
    FullPossibleRes[1377] = "MD5 Checksums Identical for: kf_uv_mode_cts[0] - Fail";
    FullPossibleRes[1378] = "MD5 Checksums Identical for: kf_uv_mode_cts[1] - Fail";
    FullPossibleRes[1379] = "MD5 Checksums Identical for: kf_uv_mode_cts[2] - Fail";
    FullPossibleRes[1380] = "MD5 Checksums Identical for: kf_uv_mode_cts[3] - Fail";
    FullPossibleRes[1381] = "MD5 Checksums Identical for: kf_y_mode_cts[0] - Fail";
    FullPossibleRes[1382] = "MD5 Checksums Identical for: kf_y_mode_cts[1] - Fail";
    FullPossibleRes[1383] = "MD5 Checksums Identical for: kf_y_mode_cts[2] - Fail";
    FullPossibleRes[1384] = "MD5 Checksums Identical for: kf_y_mode_cts[3] - Fail";
    FullPossibleRes[1385] = "MD5 Checksums Identical for: kf_y_mode_cts[4] - Fail";
    FullPossibleRes[1386] = "MD5 Checksums Identical for: vp8_mb_feature_data_bits[0] - Fail";
    FullPossibleRes[1387] = "MD5 Checksums Identical for: vp8_mb_feature_data_bits[1] - Fail";
    FullPossibleRes[1388] = "MD5 Checksums Identical for: vp8_mode_contexts[0][0] - Fail";
    FullPossibleRes[1389] = "MD5 Checksums Identical for: vp8_mode_contexts[1][0] - Fail";
    FullPossibleRes[1390] = "MD5 Checksums Identical for: vp8_mode_contexts[2][0] - Fail";
    FullPossibleRes[1391] = "MD5 Checksums Identical for: vp8_mode_contexts[3][0] - Fail";
    FullPossibleRes[1392] = "MD5 Checksums Identical for: vp8_mode_contexts[4][0] - Fail";
    FullPossibleRes[1393] = "MD5 Checksums Identical for: vp8_mode_contexts[5][0] - Fail";
    FullPossibleRes[1394] = "MD5 Checksums Identical for: vp8_mode_contexts[0][1] - Fail";
    FullPossibleRes[1395] = "MD5 Checksums Identical for: vp8_mode_contexts[1][1] - Fail";
    FullPossibleRes[1396] = "MD5 Checksums Identical for: vp8_mode_contexts[2][1] - Fail";
    FullPossibleRes[1397] = "MD5 Checksums Identical for: vp8_mode_contexts[3][1] - Fail";
    FullPossibleRes[1398] = "MD5 Checksums Identical for: vp8_mode_contexts[4][1] - Fail";
    FullPossibleRes[1399] = "MD5 Checksums Identical for: vp8_mode_contexts[5][1] - Fail";
    FullPossibleRes[1400] = "MD5 Checksums Identical for: vp8_mode_contexts[0][2] - Fail";
    FullPossibleRes[1401] = "MD5 Checksums Identical for: vp8_mode_contexts[1][2] - Fail";
    FullPossibleRes[1402] = "MD5 Checksums Identical for: vp8_mode_contexts[2][2] - Fail";
    FullPossibleRes[1403] = "MD5 Checksums Identical for: vp8_mode_contexts[3][2] - Fail";
    FullPossibleRes[1404] = "MD5 Checksums Identical for: vp8_mode_contexts[4][2] - Fail";
    FullPossibleRes[1405] = "MD5 Checksums Identical for: vp8_mode_contexts[5][2] - Fail";
    FullPossibleRes[1406] = "MD5 Checksums Identical for: vp8_mode_contexts[0][3] - Fail";
    FullPossibleRes[1407] = "MD5 Checksums Identical for: vp8_mode_contexts[1][3] - Fail";
    FullPossibleRes[1408] = "MD5 Checksums Identical for: vp8_mode_contexts[2][3] - Fail";
    FullPossibleRes[1409] = "MD5 Checksums Identical for: vp8_mode_contexts[3][3] - Fail";
    FullPossibleRes[1410] = "MD5 Checksums Identical for: vp8_mode_contexts[4][3] - Fail";
    FullPossibleRes[1411] = "MD5 Checksums Identical for: vp8_mode_contexts[5][3] - Fail";
    FullPossibleRes[1412] = "MD5 Checksums Identical for: vp8_mv_cont_count[0][0] - Fail"; // Removed 2.0
    FullPossibleRes[1413] = "MD5 Checksums Identical for: vp8_mv_cont_count[1][0] - Fail";
    FullPossibleRes[1414] = "MD5 Checksums Identical for: vp8_mv_cont_count[2][0] - Fail";
    FullPossibleRes[1415] = "MD5 Checksums Identical for: vp8_mv_cont_count[3][0] - Fail";
    FullPossibleRes[1416] = "MD5 Checksums Identical for: vp8_mv_cont_count[4][0] - Fail";
    FullPossibleRes[1417] = "MD5 Checksums Identical for: vp8_mv_cont_count[0][1] - Fail";
    FullPossibleRes[1418] = "MD5 Checksums Identical for: vp8_mv_cont_count[1][1] - Fail";
    FullPossibleRes[1419] = "MD5 Checksums Identical for: vp8_mv_cont_count[2][1] - Fail";
    FullPossibleRes[1420] = "MD5 Checksums Identical for: vp8_mv_cont_count[3][1] - Fail";
    FullPossibleRes[1421] = "MD5 Checksums Identical for: vp8_mv_cont_count[4][1] - Fail";
    FullPossibleRes[1422] = "MD5 Checksums Identical for: vp8_mv_cont_count[0][2] - Fail";
    FullPossibleRes[1423] = "MD5 Checksums Identical for: vp8_mv_cont_count[1][2] - Fail";
    FullPossibleRes[1424] = "MD5 Checksums Identical for: vp8_mv_cont_count[2][2] - Fail";
    FullPossibleRes[1425] = "MD5 Checksums Identical for: vp8_mv_cont_count[3][2] - Fail";
    FullPossibleRes[1426] = "MD5 Checksums Identical for: vp8_mv_cont_count[4][2] - Fail";
    FullPossibleRes[1427] = "MD5 Checksums Identical for: vp8_mv_cont_count[0][3] - Fail";
    FullPossibleRes[1428] = "MD5 Checksums Identical for: vp8_mv_cont_count[1][3] - Fail";
    FullPossibleRes[1429] = "MD5 Checksums Identical for: vp8_mv_cont_count[2][3] - Fail";
    FullPossibleRes[1430] = "MD5 Checksums Identical for: vp8_mv_cont_count[3][3] - Fail";
    FullPossibleRes[1431] = "MD5 Checksums Identical for: vp8_mv_cont_count[4][3] - Fail";  // Removed 2.0
    FullPossibleRes[1432] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[0] - Fail";
    FullPossibleRes[1433] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[1] - Fail";
    FullPossibleRes[1434] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[2] - Fail";
    FullPossibleRes[1435] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[3] - Fail";
    FullPossibleRes[1436] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[4] - Fail";
    FullPossibleRes[1437] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[5] - Fail";
    FullPossibleRes[1438] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[6] - Fail";
    FullPossibleRes[1439] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[7] - Fail";
    FullPossibleRes[1440] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[8] - Fail";
    FullPossibleRes[1441] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[9] - Fail";
    FullPossibleRes[1442] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[10] - Fail";
    FullPossibleRes[1443] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[11] - Fail";
    FullPossibleRes[1444] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[12] - Fail";
    FullPossibleRes[1445] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[13] - Fail";
    FullPossibleRes[1446] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[14] - Fail";
    FullPossibleRes[1447] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[15] - Fail";
    FullPossibleRes[1448] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[16] - Fail";
    FullPossibleRes[1449] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[17] - Fail";
    FullPossibleRes[1450] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[18] - Fail";
    FullPossibleRes[1451] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[19] - Fail";
    FullPossibleRes[1452] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[20] - Fail";
    FullPossibleRes[1453] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[21] - Fail";
    FullPossibleRes[1454] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[22] - Fail";
    FullPossibleRes[1455] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[23] - Fail";
    FullPossibleRes[1456] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[24] - Fail";
    FullPossibleRes[1457] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[25] - Fail";
    FullPossibleRes[1458] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[26] - Fail";
    FullPossibleRes[1459] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[27] - Fail";
    FullPossibleRes[1460] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[28] - Fail";
    FullPossibleRes[1461] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[29] - Fail";
    FullPossibleRes[1462] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[30] - Fail";
    FullPossibleRes[1463] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[31] - Fail";
    FullPossibleRes[1464] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[32] - Fail";
    FullPossibleRes[1465] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[33] - Fail";
    FullPossibleRes[1466] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[34] - Fail";
    FullPossibleRes[1467] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[35] - Fail";
    FullPossibleRes[1468] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[36] - Fail";
    FullPossibleRes[1469] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[37] - Fail";
    FullPossibleRes[1470] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[38] - Fail";
    FullPossibleRes[1471] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[39] - Fail";
    FullPossibleRes[1472] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[40] - Fail";
    FullPossibleRes[1473] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[41] - Fail";
    FullPossibleRes[1474] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[42] - Fail";
    FullPossibleRes[1475] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[43] - Fail";
    FullPossibleRes[1476] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[44] - Fail";
    FullPossibleRes[1477] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[45] - Fail";
    FullPossibleRes[1478] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[46] - Fail";
    FullPossibleRes[1479] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[47] - Fail";
    FullPossibleRes[1480] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[48] - Fail";
    FullPossibleRes[1481] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[49] - Fail";
    FullPossibleRes[1482] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[50] - Fail";
    FullPossibleRes[1483] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[51] - Fail";
    FullPossibleRes[1484] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[52] - Fail";
    FullPossibleRes[1485] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[53] - Fail";
    FullPossibleRes[1486] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[54] - Fail";
    FullPossibleRes[1487] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[55] - Fail";
    FullPossibleRes[1488] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[56] - Fail";
    FullPossibleRes[1489] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[57] - Fail";
    FullPossibleRes[1490] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[58] - Fail";
    FullPossibleRes[1491] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[59] - Fail";
    FullPossibleRes[1492] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[60] - Fail";
    FullPossibleRes[1493] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[61] - Fail";
    FullPossibleRes[1494] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[62] - Fail";
    FullPossibleRes[1495] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[63] - Fail";
    FullPossibleRes[1496] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[64] - Fail";
    FullPossibleRes[1497] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[65] - Fail";
    FullPossibleRes[1498] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[66] - Fail";
    FullPossibleRes[1499] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[67] - Fail";
    FullPossibleRes[1500] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[68] - Fail";
    FullPossibleRes[1501] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[69] - Fail";
    FullPossibleRes[1502] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[70] - Fail";
    FullPossibleRes[1503] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[71] - Fail";
    FullPossibleRes[1504] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[72] - Fail";
    FullPossibleRes[1505] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[73] - Fail";
    FullPossibleRes[1506] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[74] - Fail";
    FullPossibleRes[1507] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[75] - Fail";
    FullPossibleRes[1508] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[76] - Fail";
    FullPossibleRes[1509] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[77] - Fail";
    FullPossibleRes[1510] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[78] - Fail";
    FullPossibleRes[1511] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[79] - Fail";
    FullPossibleRes[1512] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[80] - Fail";
    FullPossibleRes[1513] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[81] - Fail";
    FullPossibleRes[1514] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[82] - Fail";
    FullPossibleRes[1515] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[83] - Fail";
    FullPossibleRes[1516] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[84] - Fail";
    FullPossibleRes[1517] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[85] - Fail";
    FullPossibleRes[1518] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[86] - Fail";
    FullPossibleRes[1519] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[87] - Fail";
    FullPossibleRes[1520] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[88] - Fail";
    FullPossibleRes[1521] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[89] - Fail";
    FullPossibleRes[1522] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[90] - Fail";
    FullPossibleRes[1523] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[91] - Fail";
    FullPossibleRes[1524] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[92] - Fail";
    FullPossibleRes[1525] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[93] - Fail";
    FullPossibleRes[1526] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[94] - Fail";
    FullPossibleRes[1527] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[95] - Fail";
    FullPossibleRes[1528] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[96] - Fail";
    FullPossibleRes[1529] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[97] - Fail";
    FullPossibleRes[1530] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[98] - Fail";
    FullPossibleRes[1531] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[99] - Fail";
    FullPossibleRes[1532] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[100] - Fail";
    FullPossibleRes[1533] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[101] - Fail";
    FullPossibleRes[1534] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[102] - Fail";
    FullPossibleRes[1535] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[103] - Fail";
    FullPossibleRes[1536] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[104] - Fail";
    FullPossibleRes[1537] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[105] - Fail";
    FullPossibleRes[1538] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[106] - Fail";
    FullPossibleRes[1539] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[107] - Fail";
    FullPossibleRes[1540] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[108] - Fail";
    FullPossibleRes[1541] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[109] - Fail";
    FullPossibleRes[1542] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[110] - Fail";
    FullPossibleRes[1543] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[111] - Fail";
    FullPossibleRes[1544] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[112] - Fail";
    FullPossibleRes[1545] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[113] - Fail";
    FullPossibleRes[1546] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[114] - Fail";
    FullPossibleRes[1547] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[115] - Fail";
    FullPossibleRes[1548] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[116] - Fail";
    FullPossibleRes[1549] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[117] - Fail";
    FullPossibleRes[1550] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[118] - Fail";
    FullPossibleRes[1551] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[119] - Fail";
    FullPossibleRes[1552] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[120] - Fail";
    FullPossibleRes[1553] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[121] - Fail";
    FullPossibleRes[1554] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[122] - Fail";
    FullPossibleRes[1555] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[123] - Fail";
    FullPossibleRes[1556] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[124] - Fail";
    FullPossibleRes[1557] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[125] - Fail";
    FullPossibleRes[1558] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[126] - Fail";
    FullPossibleRes[1559] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[127] - Fail";
    FullPossibleRes[1560] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[128] - Fail";
    FullPossibleRes[1561] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[129] - Fail";
    FullPossibleRes[1562] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[130] - Fail";
    FullPossibleRes[1563] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[131] - Fail";
    FullPossibleRes[1564] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[132] - Fail";
    FullPossibleRes[1565] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[133] - Fail";
    FullPossibleRes[1566] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[134] - Fail";
    FullPossibleRes[1567] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[135] - Fail";
    FullPossibleRes[1568] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[136] - Fail";
    FullPossibleRes[1569] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[137] - Fail";
    FullPossibleRes[1570] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[138] - Fail";
    FullPossibleRes[1571] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[139] - Fail";
    FullPossibleRes[1572] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[140] - Fail";
    FullPossibleRes[1573] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[141] - Fail";
    FullPossibleRes[1574] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[142] - Fail";
    FullPossibleRes[1575] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[143] - Fail";
    FullPossibleRes[1576] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[144] - Fail";
    FullPossibleRes[1577] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[145] - Fail";
    FullPossibleRes[1578] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[146] - Fail";
    FullPossibleRes[1579] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[147] - Fail";
    FullPossibleRes[1580] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[148] - Fail";
    FullPossibleRes[1581] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[149] - Fail";
    FullPossibleRes[1582] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[150] - Fail";
    FullPossibleRes[1583] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[151] - Fail";
    FullPossibleRes[1584] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[152] - Fail";
    FullPossibleRes[1585] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[153] - Fail";
    FullPossibleRes[1586] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[154] - Fail";
    FullPossibleRes[1587] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[155] - Fail";
    FullPossibleRes[1588] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[156] - Fail";
    FullPossibleRes[1589] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[157] - Fail";
    FullPossibleRes[1590] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[158] - Fail";
    FullPossibleRes[1591] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[159] - Fail";
    FullPossibleRes[1592] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[160] - Fail";
    FullPossibleRes[1593] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[161] - Fail";
    FullPossibleRes[1594] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[162] - Fail";
    FullPossibleRes[1595] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[163] - Fail";
    FullPossibleRes[1596] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[164] - Fail";
    FullPossibleRes[1597] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[165] - Fail";
    FullPossibleRes[1598] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[166] - Fail";
    FullPossibleRes[1599] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[167] - Fail";
    FullPossibleRes[1600] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[168] - Fail";
    FullPossibleRes[1601] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[169] - Fail";
    FullPossibleRes[1602] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[170] - Fail";
    FullPossibleRes[1603] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[171] - Fail";
    FullPossibleRes[1604] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[172] - Fail";
    FullPossibleRes[1605] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[173] - Fail";
    FullPossibleRes[1606] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[174] - Fail";
    FullPossibleRes[1607] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[175] - Fail";
    FullPossibleRes[1608] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[176] - Fail";
    FullPossibleRes[1609] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[177] - Fail";
    FullPossibleRes[1610] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[178] - Fail";
    FullPossibleRes[1611] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[179] - Fail";
    FullPossibleRes[1612] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[180] - Fail";
    FullPossibleRes[1613] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[181] - Fail";
    FullPossibleRes[1614] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[182] - Fail";
    FullPossibleRes[1615] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[183] - Fail";
    FullPossibleRes[1616] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[184] - Fail";
    FullPossibleRes[1617] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[185] - Fail";
    FullPossibleRes[1618] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[186] - Fail";
    FullPossibleRes[1619] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[187] - Fail";
    FullPossibleRes[1620] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[188] - Fail";
    FullPossibleRes[1621] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[189] - Fail";
    FullPossibleRes[1622] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[190] - Fail";
    FullPossibleRes[1623] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[191] - Fail";
    FullPossibleRes[1624] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[192] - Fail";
    FullPossibleRes[1625] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[193] - Fail";
    FullPossibleRes[1626] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[194] - Fail";
    FullPossibleRes[1627] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[195] - Fail";
    FullPossibleRes[1628] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[196] - Fail";
    FullPossibleRes[1629] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[197] - Fail";
    FullPossibleRes[1630] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[198] - Fail";
    FullPossibleRes[1631] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[199] - Fail";
    FullPossibleRes[1632] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[200] - Fail";
    FullPossibleRes[1633] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[201] - Fail";
    FullPossibleRes[1634] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[202] - Fail";
    FullPossibleRes[1635] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[203] - Fail";
    FullPossibleRes[1636] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[204] - Fail";
    FullPossibleRes[1637] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[205] - Fail";
    FullPossibleRes[1638] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[206] - Fail";
    FullPossibleRes[1639] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[207] - Fail";
    FullPossibleRes[1640] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[208] - Fail";
    FullPossibleRes[1641] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[209] - Fail";
    FullPossibleRes[1642] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[210] - Fail";
    FullPossibleRes[1643] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[211] - Fail";
    FullPossibleRes[1644] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[212] - Fail";
    FullPossibleRes[1645] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[213] - Fail";
    FullPossibleRes[1646] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[214] - Fail";
    FullPossibleRes[1647] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[215] - Fail";
    FullPossibleRes[1648] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[216] - Fail";
    FullPossibleRes[1649] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[217] - Fail";
    FullPossibleRes[1650] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[218] - Fail";
    FullPossibleRes[1651] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[219] - Fail";
    FullPossibleRes[1652] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[220] - Fail";
    FullPossibleRes[1653] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[221] - Fail";
    FullPossibleRes[1654] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[222] - Fail";
    FullPossibleRes[1655] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[223] - Fail";
    FullPossibleRes[1656] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[224] - Fail";
    FullPossibleRes[1657] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[225] - Fail";
    FullPossibleRes[1658] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[226] - Fail";
    FullPossibleRes[1659] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[227] - Fail";
    FullPossibleRes[1660] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[228] - Fail";
    FullPossibleRes[1661] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[229] - Fail";
    FullPossibleRes[1662] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[230] - Fail";
    FullPossibleRes[1663] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[231] - Fail";
    FullPossibleRes[1664] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[232] - Fail";
    FullPossibleRes[1665] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[233] - Fail";
    FullPossibleRes[1666] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[234] - Fail";
    FullPossibleRes[1667] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[235] - Fail";
    FullPossibleRes[1668] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[236] - Fail";
    FullPossibleRes[1669] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[237] - Fail";
    FullPossibleRes[1670] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[238] - Fail";
    FullPossibleRes[1671] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[239] - Fail";
    FullPossibleRes[1672] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[240] - Fail";
    FullPossibleRes[1673] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[241] - Fail";
    FullPossibleRes[1674] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[242] - Fail";
    FullPossibleRes[1675] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[243] - Fail";
    FullPossibleRes[1676] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[244] - Fail";
    FullPossibleRes[1677] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[245] - Fail";
    FullPossibleRes[1678] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[246] - Fail";
    FullPossibleRes[1679] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[247] - Fail";
    FullPossibleRes[1680] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[248] - Fail";
    FullPossibleRes[1681] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[249] - Fail";
    FullPossibleRes[1682] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[250] - Fail";
    FullPossibleRes[1683] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[251] - Fail";
    FullPossibleRes[1684] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[252] - Fail";
    FullPossibleRes[1685] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[253] - Fail";
    FullPossibleRes[1686] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[254] - Fail";
    FullPossibleRes[1687] = "MD5 Checksums Identical for: vp8dx_bitreader_norm[255] - Fail";
    FullPossibleRes[1688] = "MD5 Checksums Identical for: vp8_bmode_tree[0] - Fail";
    FullPossibleRes[1689] = "MD5 Checksums Identical for: vp8_bmode_tree[1] - Fail";
    FullPossibleRes[1690] = "MD5 Checksums Identical for: vp8_bmode_tree[2] - Fail";
    FullPossibleRes[1691] = "MD5 Checksums Identical for: vp8_bmode_tree[3] - Fail";
    FullPossibleRes[1692] = "MD5 Checksums Identical for: vp8_bmode_tree[4] - Fail";
    FullPossibleRes[1693] = "MD5 Checksums Identical for: vp8_bmode_tree[5] - Fail";
    FullPossibleRes[1694] = "MD5 Checksums Identical for: vp8_bmode_tree[6] - Fail";
    FullPossibleRes[1695] = "MD5 Checksums Identical for: vp8_bmode_tree[7] - Fail";
    FullPossibleRes[1696] = "MD5 Checksums Identical for: vp8_bmode_tree[8] - Fail";
    FullPossibleRes[1697] = "MD5 Checksums Identical for: vp8_bmode_tree[9] - Fail";
    FullPossibleRes[1698] = "MD5 Checksums Identical for: vp8_bmode_tree[10] - Fail";
    FullPossibleRes[1699] = "MD5 Checksums Identical for: vp8_bmode_tree[11] - Fail";
    FullPossibleRes[1700] = "MD5 Checksums Identical for: vp8_bmode_tree[12] - Fail";
    FullPossibleRes[1701] = "MD5 Checksums Identical for: vp8_bmode_tree[13] - Fail";
    FullPossibleRes[1702] = "MD5 Checksums Identical for: vp8_bmode_tree[14] - Fail";
    FullPossibleRes[1703] = "MD5 Checksums Identical for: vp8_bmode_tree[15] - Fail";
    FullPossibleRes[1704] = "MD5 Checksums Identical for: vp8_bmode_tree[16] - Fail";
    FullPossibleRes[1705] = "MD5 Checksums Identical for: vp8_bmode_tree[17] - Fail";
    FullPossibleRes[1706] = "MD5 Checksums Identical for: vp8_coef_tree[0] - Fail";
    FullPossibleRes[1707] = "MD5 Checksums Identical for: vp8_coef_tree[1] - Fail";
    FullPossibleRes[1708] = "MD5 Checksums Identical for: vp8_coef_tree[2] - Fail";
    FullPossibleRes[1709] = "MD5 Checksums Identical for: vp8_coef_tree[3] - Fail";
    FullPossibleRes[1710] = "MD5 Checksums Identical for: vp8_coef_tree[4] - Fail";
    FullPossibleRes[1711] = "MD5 Checksums Identical for: vp8_coef_tree[5] - Fail";
    FullPossibleRes[1712] = "MD5 Checksums Identical for: vp8_coef_tree[6] - Fail";
    FullPossibleRes[1713] = "MD5 Checksums Identical for: vp8_coef_tree[7] - Fail";
    FullPossibleRes[1714] = "MD5 Checksums Identical for: vp8_coef_tree[8] - Fail";
    FullPossibleRes[1715] = "MD5 Checksums Identical for: vp8_coef_tree[9] - Fail";
    FullPossibleRes[1716] = "MD5 Checksums Identical for: vp8_coef_tree[10] - Fail";
    FullPossibleRes[1717] = "MD5 Checksums Identical for: vp8_coef_tree[11] - Fail";
    FullPossibleRes[1718] = "MD5 Checksums Identical for: vp8_coef_tree[12] - Fail";
    FullPossibleRes[1719] = "MD5 Checksums Identical for: vp8_coef_tree[13] - Fail";
    FullPossibleRes[1720] = "MD5 Checksums Identical for: vp8_coef_tree[14] - Fail";
    FullPossibleRes[1721] = "MD5 Checksums Identical for: vp8_coef_tree[15] - Fail";
    FullPossibleRes[1722] = "MD5 Checksums Identical for: vp8_coef_tree[16] - Fail";
    FullPossibleRes[1723] = "MD5 Checksums Identical for: vp8_coef_tree[17] - Fail";
    FullPossibleRes[1724] = "MD5 Checksums Identical for: vp8_coef_tree[18] - Fail";
    FullPossibleRes[1725] = "MD5 Checksums Identical for: vp8_coef_tree[19] - Fail";
    FullPossibleRes[1726] = "MD5 Checksums Identical for: vp8_coef_tree[20] - Fail";
    FullPossibleRes[1727] = "MD5 Checksums Identical for: vp8_coef_tree[21] - Fail";
    FullPossibleRes[1728] = "MD5 Checksums Identical for: VP8_HighMVtree[0] - Fail"; //Removed 2.0
    FullPossibleRes[1729] = "MD5 Checksums Identical for: VP8_HighMVtree[1] - Fail";
    FullPossibleRes[1730] = "MD5 Checksums Identical for: VP8_HighMVtree[2] - Fail";
    FullPossibleRes[1731] = "MD5 Checksums Identical for: VP8_HighMVtree[3] - Fail";
    FullPossibleRes[1732] = "MD5 Checksums Identical for: VP8_HighMVtree[4] - Fail";
    FullPossibleRes[1733] = "MD5 Checksums Identical for: VP8_HighMVtree[5] - Fail";
    FullPossibleRes[1734] = "MD5 Checksums Identical for: VP8_HighMVtree[6] - Fail";
    FullPossibleRes[1735] = "MD5 Checksums Identical for: VP8_HighMVtree[7] - Fail"; //Removed 2.0
    FullPossibleRes[1736] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[0] - Fail";
    FullPossibleRes[1737] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[1] - Fail";
    FullPossibleRes[1738] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[2] - Fail";
    FullPossibleRes[1739] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[3] - Fail";
    FullPossibleRes[1740] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[4] - Fail";
    FullPossibleRes[1741] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[5] - Fail";
    FullPossibleRes[1742] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[6] - Fail";
    FullPossibleRes[1743] = "MD5 Checksums Identical for: vp8_kf_ymode_tree[7] - Fail";
    FullPossibleRes[1744] = "MD5 Checksums Identical for: VP8_LowMVtree[0] - Fail"; //Removed 2.0
    FullPossibleRes[1745] = "MD5 Checksums Identical for: VP8_LowMVtree[1] - Fail";
    FullPossibleRes[1746] = "MD5 Checksums Identical for: VP8_LowMVtree[2] - Fail";
    FullPossibleRes[1747] = "MD5 Checksums Identical for: VP8_LowMVtree[3] - Fail";
    FullPossibleRes[1748] = "MD5 Checksums Identical for: VP8_LowMVtree[4] - Fail";
    FullPossibleRes[1749] = "MD5 Checksums Identical for: VP8_LowMVtree[5] - Fail"; //Removed 2.0
    FullPossibleRes[1750] = "MD5 Checksums Identical for: vp8_mbsplit_count[0] - Fail";
    FullPossibleRes[1751] = "MD5 Checksums Identical for: vp8_mbsplit_count[1] - Fail";
    FullPossibleRes[1752] = "MD5 Checksums Identical for: vp8_mbsplit_count[2] - Fail";
    FullPossibleRes[1753] = "MD5 Checksums Identical for: vp8_mbsplit_count[3] - Fail";
    FullPossibleRes[1754] = "MD5 Checksums Identical for: vp8_mbsplit_probs[0] - Fail";
    FullPossibleRes[1755] = "MD5 Checksums Identical for: vp8_mbsplit_probs[1] - Fail";
    FullPossibleRes[1756] = "MD5 Checksums Identical for: vp8_mbsplit_probs[2] - Fail";
    FullPossibleRes[1757] = "MD5 Checksums Identical for: vp8_mbsplits[0] - Fail";
    FullPossibleRes[1758] = "MD5 Checksums Identical for: vp8_mbsplits[1] - Fail";
    FullPossibleRes[1759] = "MD5 Checksums Identical for: vp8_mbsplits[2] - Fail";
    FullPossibleRes[1760] = "MD5 Checksums Identical for: vp8_mbsplits[3] - Fail";
    FullPossibleRes[1761] = "MD5 Checksums Identical for: vp8_mbsplit_tree[0] - Fail";
    FullPossibleRes[1762] = "MD5 Checksums Identical for: vp8_mbsplit_tree[1] - Fail";
    FullPossibleRes[1763] = "MD5 Checksums Identical for: vp8_mbsplit_tree[2] - Fail";
    FullPossibleRes[1764] = "MD5 Checksums Identical for: vp8_mbsplit_tree[3] - Fail";
    FullPossibleRes[1765] = "MD5 Checksums Identical for: vp8_mbsplit_tree[4] - Fail";
    FullPossibleRes[1766] = "MD5 Checksums Identical for: vp8_mbsplit_tree[5] - Fail";
    FullPossibleRes[1767] = "MD5 Checksums Identical for: vp8_mv_ref_tree[0] - Fail";
    FullPossibleRes[1768] = "MD5 Checksums Identical for: vp8_mv_ref_tree[1] - Fail";
    FullPossibleRes[1769] = "MD5 Checksums Identical for: vp8_mv_ref_tree[2] - Fail";
    FullPossibleRes[1770] = "MD5 Checksums Identical for: vp8_mv_ref_tree[3] - Fail";
    FullPossibleRes[1771] = "MD5 Checksums Identical for: vp8_mv_ref_tree[4] - Fail";
    FullPossibleRes[1772] = "MD5 Checksums Identical for: vp8_mv_ref_tree[5] - Fail";
    FullPossibleRes[1773] = "MD5 Checksums Identical for: vp8_mv_ref_tree[6] - Fail";
    FullPossibleRes[1774] = "MD5 Checksums Identical for: vp8_mv_ref_tree[7] - Fail";
    FullPossibleRes[1775] = "MD5 Checksums Identical for: vp8_mv_update_probs[0] - Fail";
    FullPossibleRes[1776] = "MD5 Checksums Identical for: vp8_mv_update_probs[1] - Fail";
    FullPossibleRes[1777] = "MD5 Checksums Identical for: VP8_Reverse3bits[0] - Fail"; //Removed 2.0
    FullPossibleRes[1778] = "MD5 Checksums Identical for: VP8_Reverse3bits[1] - Fail";
    FullPossibleRes[1779] = "MD5 Checksums Identical for: VP8_Reverse3bits[2] - Fail";
    FullPossibleRes[1780] = "MD5 Checksums Identical for: VP8_Reverse3bits[3] - Fail";
    FullPossibleRes[1781] = "MD5 Checksums Identical for: VP8_Reverse3bits[4] - Fail";
    FullPossibleRes[1782] = "MD5 Checksums Identical for: VP8_Reverse3bits[5] - Fail";
    FullPossibleRes[1783] = "MD5 Checksums Identical for: VP8_Reverse3bits[6] - Fail";
    FullPossibleRes[1784] = "MD5 Checksums Identical for: VP8_Reverse3bits[7] - Fail";
    FullPossibleRes[1785] = "MD5 Checksums Identical for: VP8_Reverse4bits[0] - Fail";
    FullPossibleRes[1786] = "MD5 Checksums Identical for: VP8_Reverse4bits[1] - Fail";
    FullPossibleRes[1787] = "MD5 Checksums Identical for: VP8_Reverse4bits[2] - Fail";
    FullPossibleRes[1788] = "MD5 Checksums Identical for: VP8_Reverse4bits[3] - Fail";
    FullPossibleRes[1789] = "MD5 Checksums Identical for: VP8_Reverse4bits[4] - Fail";
    FullPossibleRes[1790] = "MD5 Checksums Identical for: VP8_Reverse4bits[5] - Fail";
    FullPossibleRes[1791] = "MD5 Checksums Identical for: VP8_Reverse4bits[6] - Fail";
    FullPossibleRes[1792] = "MD5 Checksums Identical for: VP8_Reverse4bits[7] - Fail";
    FullPossibleRes[1793] = "MD5 Checksums Identical for: VP8_Reverse4bits[8] - Fail";
    FullPossibleRes[1794] = "MD5 Checksums Identical for: VP8_Reverse4bits[9] - Fail";
    FullPossibleRes[1795] = "MD5 Checksums Identical for: VP8_Reverse4bits[10] - Fail";
    FullPossibleRes[1796] = "MD5 Checksums Identical for: VP8_Reverse4bits[11] - Fail";
    FullPossibleRes[1797] = "MD5 Checksums Identical for: VP8_Reverse4bits[12] - Fail";
    FullPossibleRes[1798] = "MD5 Checksums Identical for: VP8_Reverse4bits[13] - Fail";
    FullPossibleRes[1799] = "MD5 Checksums Identical for: VP8_Reverse4bits[14] - Fail";
    FullPossibleRes[1800] = "MD5 Checksums Identical for: VP8_Reverse4bits[15] - Fail"; //Removed 2.0
    FullPossibleRes[1801] = "MD5 Checksums Identical for: vp8_small_mvtree[0] - Fail";
    FullPossibleRes[1802] = "MD5 Checksums Identical for: vp8_small_mvtree[1] - Fail";
    FullPossibleRes[1803] = "MD5 Checksums Identical for: vp8_small_mvtree[2] - Fail";
    FullPossibleRes[1804] = "MD5 Checksums Identical for: vp8_small_mvtree[3] - Fail";
    FullPossibleRes[1805] = "MD5 Checksums Identical for: vp8_small_mvtree[4] - Fail";
    FullPossibleRes[1806] = "MD5 Checksums Identical for: vp8_small_mvtree[5] - Fail";
    FullPossibleRes[1807] = "MD5 Checksums Identical for: vp8_small_mvtree[6] - Fail";
    FullPossibleRes[1808] = "MD5 Checksums Identical for: vp8_small_mvtree[7] - Fail";
    FullPossibleRes[1809] = "MD5 Checksums Identical for: vp8_small_mvtree[8] - Fail";
    FullPossibleRes[1810] = "MD5 Checksums Identical for: vp8_small_mvtree[9] - Fail";
    FullPossibleRes[1811] = "MD5 Checksums Identical for: vp8_small_mvtree[10] - Fail";
    FullPossibleRes[1812] = "MD5 Checksums Identical for: vp8_small_mvtree[11] - Fail";
    FullPossibleRes[1813] = "MD5 Checksums Identical for: vp8_small_mvtree[12] - Fail";
    FullPossibleRes[1814] = "MD5 Checksums Identical for: vp8_small_mvtree[13] - Fail";
    FullPossibleRes[1815] = "MD5 Checksums Identical for: vp8_sub_mv_ref_tree[0] - Fail";
    FullPossibleRes[1816] = "MD5 Checksums Identical for: vp8_sub_mv_ref_tree[1] - Fail";
    FullPossibleRes[1817] = "MD5 Checksums Identical for: vp8_sub_mv_ref_tree[2] - Fail";
    FullPossibleRes[1818] = "MD5 Checksums Identical for: vp8_sub_mv_ref_tree[3] - Fail";
    FullPossibleRes[1819] = "MD5 Checksums Identical for: vp8_sub_mv_ref_tree[4] - Fail";
    FullPossibleRes[1820] = "MD5 Checksums Identical for: vp8_sub_mv_ref_tree[5] - Fail";
    FullPossibleRes[1821] = "MD5 Checksums Identical for: vp8_uv_mode_tree[0] - Fail";
    FullPossibleRes[1822] = "MD5 Checksums Identical for: vp8_uv_mode_tree[1] - Fail";
    FullPossibleRes[1823] = "MD5 Checksums Identical for: vp8_uv_mode_tree[2] - Fail";
    FullPossibleRes[1824] = "MD5 Checksums Identical for: vp8_uv_mode_tree[3] - Fail";
    FullPossibleRes[1825] = "MD5 Checksums Identical for: vp8_uv_mode_tree[4] - Fail";
    FullPossibleRes[1826] = "MD5 Checksums Identical for: vp8_uv_mode_tree[5] - Fail";
    FullPossibleRes[1827] = "MD5 Checksums Identical for: vp8_ymode_tree[0] - Fail";
    FullPossibleRes[1828] = "MD5 Checksums Identical for: vp8_ymode_tree[1] - Fail";
    FullPossibleRes[1829] = "MD5 Checksums Identical for: vp8_ymode_tree[2] - Fail";
    FullPossibleRes[1830] = "MD5 Checksums Identical for: vp8_ymode_tree[3] - Fail";
    FullPossibleRes[1831] = "MD5 Checksums Identical for: vp8_ymode_tree[4] - Fail";
    FullPossibleRes[1832] = "MD5 Checksums Identical for: vp8_ymode_tree[5] - Fail";
    FullPossibleRes[1833] = "MD5 Checksums Identical for: vp8_ymode_tree[6] - Fail";
    FullPossibleRes[1834] = "MD5 Checksums Identical for: vp8_ymode_tree[7] - Fail";
    FullPossibleRes[1835] = "MD5 Checksums Identical for: vp8_block2above[0] - Fail";
    FullPossibleRes[1836] = "MD5 Checksums Identical for: vp8_block2above[1] - Fail";
    FullPossibleRes[1837] = "MD5 Checksums Identical for: vp8_block2above[2] - Fail";
    FullPossibleRes[1838] = "MD5 Checksums Identical for: vp8_block2above[3] - Fail";
    FullPossibleRes[1839] = "MD5 Checksums Identical for: vp8_block2above[4] - Fail";
    FullPossibleRes[1840] = "MD5 Checksums Identical for: vp8_block2above[5] - Fail";
    FullPossibleRes[1841] = "MD5 Checksums Identical for: vp8_block2above[6] - Fail";
    FullPossibleRes[1842] = "MD5 Checksums Identical for: vp8_block2above[7] - Fail";
    FullPossibleRes[1843] = "MD5 Checksums Identical for: vp8_block2above[8] - Fail";
    FullPossibleRes[1844] = "MD5 Checksums Identical for: vp8_block2above[9] - Fail";
    FullPossibleRes[1845] = "MD5 Checksums Identical for: vp8_block2above[10] - Fail";
    FullPossibleRes[1846] = "MD5 Checksums Identical for: vp8_block2above[11] - Fail";
    FullPossibleRes[1847] = "MD5 Checksums Identical for: vp8_block2above[12] - Fail";
    FullPossibleRes[1848] = "MD5 Checksums Identical for: vp8_block2above[13] - Fail";
    FullPossibleRes[1849] = "MD5 Checksums Identical for: vp8_block2above[14] - Fail";
    FullPossibleRes[1850] = "MD5 Checksums Identical for: vp8_block2above[15] - Fail";
    FullPossibleRes[1851] = "MD5 Checksums Identical for: vp8_block2above[16] - Fail";
    FullPossibleRes[1852] = "MD5 Checksums Identical for: vp8_block2above[17] - Fail";
    FullPossibleRes[1853] = "MD5 Checksums Identical for: vp8_block2above[18] - Fail";
    FullPossibleRes[1854] = "MD5 Checksums Identical for: vp8_block2above[19] - Fail";
    FullPossibleRes[1855] = "MD5 Checksums Identical for: vp8_block2above[20] - Fail";
    FullPossibleRes[1856] = "MD5 Checksums Identical for: vp8_block2above[21] - Fail";
    FullPossibleRes[1857] = "MD5 Checksums Identical for: vp8_block2above[22] - Fail";
    FullPossibleRes[1858] = "MD5 Checksums Identical for: vp8_block2above[23] - Fail";
    FullPossibleRes[1859] = "MD5 Checksums Identical for: vp8_block2above[24] - Fail";
    FullPossibleRes[1860] = "MD5 Checksums Identical for: vp8_block2context[0] - Fail";
    FullPossibleRes[1861] = "MD5 Checksums Identical for: vp8_block2context[1] - Fail";
    FullPossibleRes[1862] = "MD5 Checksums Identical for: vp8_block2context[2] - Fail";
    FullPossibleRes[1863] = "MD5 Checksums Identical for: vp8_block2context[3] - Fail";
    FullPossibleRes[1864] = "MD5 Checksums Identical for: vp8_block2context[4] - Fail";
    FullPossibleRes[1865] = "MD5 Checksums Identical for: vp8_block2context[5] - Fail";
    FullPossibleRes[1866] = "MD5 Checksums Identical for: vp8_block2context[6] - Fail";
    FullPossibleRes[1867] = "MD5 Checksums Identical for: vp8_block2context[7] - Fail";
    FullPossibleRes[1868] = "MD5 Checksums Identical for: vp8_block2context[8] - Fail";
    FullPossibleRes[1869] = "MD5 Checksums Identical for: vp8_block2context9] - Fail";
    FullPossibleRes[1870] = "MD5 Checksums Identical for: vp8_block2context[10] - Fail";
    FullPossibleRes[1871] = "MD5 Checksums Identical for: vp8_block2context[11] - Fail";
    FullPossibleRes[1872] = "MD5 Checksums Identical for: vp8_block2context[12] - Fail";
    FullPossibleRes[1873] = "MD5 Checksums Identical for: vp8_block2context[13] - Fail";
    FullPossibleRes[1874] = "MD5 Checksums Identical for: vp8_block2context[14] - Fail";
    FullPossibleRes[1875] = "MD5 Checksums Identical for: vp8_block2context[15] - Fail";
    FullPossibleRes[1876] = "MD5 Checksums Identical for: vp8_block2context[16] - Fail";
    FullPossibleRes[1877] = "MD5 Checksums Identical for: vp8_block2context[17] - Fail";
    FullPossibleRes[1878] = "MD5 Checksums Identical for: vp8_block2context[18] - Fail";
    FullPossibleRes[1879] = "MD5 Checksums Identical for: vp8_block2context[19] - Fail";
    FullPossibleRes[1880] = "MD5 Checksums Identical for: vp8_block2context[20] - Fail";
    FullPossibleRes[1881] = "MD5 Checksums Identical for: vp8_block2context[21] - Fail";
    FullPossibleRes[1882] = "MD5 Checksums Identical for: vp8_block2context[22] - Fail";
    FullPossibleRes[1883] = "MD5 Checksums Identical for: vp8_block2context[23] - Fail";
    FullPossibleRes[1884] = "MD5 Checksums Identical for: vp8_block2context[24] - Fail";
    FullPossibleRes[1885] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[0] - Fail"; //Removed 2.0
    FullPossibleRes[1886] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[1] - Fail";
    FullPossibleRes[1887] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[2] - Fail";
    FullPossibleRes[1888] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[3] - Fail";
    FullPossibleRes[1889] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[4] - Fail";
    FullPossibleRes[1890] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[5] - Fail";
    FullPossibleRes[1891] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[6] - Fail";
    FullPossibleRes[1892] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[7] - Fail";
    FullPossibleRes[1893] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[8] - Fail";
    FullPossibleRes[1894] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[9] - Fail";
    FullPossibleRes[1895] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[10] - Fail";
    FullPossibleRes[1896] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[11] - Fail";
    FullPossibleRes[1897] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[12] - Fail";
    FullPossibleRes[1898] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[13] - Fail";
    FullPossibleRes[1899] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[14] - Fail";
    FullPossibleRes[1900] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[15] - Fail";
    FullPossibleRes[1901] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[16] - Fail";
    FullPossibleRes[1902] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[17] - Fail";
    FullPossibleRes[1903] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[18] - Fail";
    FullPossibleRes[1904] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[19] - Fail";
    FullPossibleRes[1905] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[20] - Fail";
    FullPossibleRes[1906] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[21] - Fail";
    FullPossibleRes[1907] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[22] - Fail";
    FullPossibleRes[1908] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[23] - Fail";
    FullPossibleRes[1909] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[24] - Fail";
    FullPossibleRes[1910] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[25] - Fail";
    FullPossibleRes[1911] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[26] - Fail";
    FullPossibleRes[1912] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[27] - Fail";
    FullPossibleRes[1913] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[28] - Fail";
    FullPossibleRes[1914] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[29] - Fail";
    FullPossibleRes[1915] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[30] - Fail";
    FullPossibleRes[1916] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[31] - Fail";
    FullPossibleRes[1917] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[32] - Fail";
    FullPossibleRes[1918] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[33] - Fail";
    FullPossibleRes[1919] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[34] - Fail";
    FullPossibleRes[1920] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[35] - Fail";
    FullPossibleRes[1921] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[36] - Fail";
    FullPossibleRes[1922] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[37] - Fail";
    FullPossibleRes[1923] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[38] - Fail";
    FullPossibleRes[1924] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[39] - Fail";
    FullPossibleRes[1925] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[40] - Fail";
    FullPossibleRes[1926] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[41] - Fail";
    FullPossibleRes[1927] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[42] - Fail";
    FullPossibleRes[1928] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[43] - Fail";
    FullPossibleRes[1929] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[44] - Fail";
    FullPossibleRes[1930] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[45] - Fail";
    FullPossibleRes[1931] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[46] - Fail";
    FullPossibleRes[1932] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[47] - Fail";
    FullPossibleRes[1933] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[48] - Fail";
    FullPossibleRes[1934] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[49] - Fail";
    FullPossibleRes[1935] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[50] - Fail";
    FullPossibleRes[1936] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[51] - Fail";
    FullPossibleRes[1937] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[52] - Fail";
    FullPossibleRes[1938] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[53] - Fail";
    FullPossibleRes[1939] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[54] - Fail";
    FullPossibleRes[1940] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[55] - Fail";
    FullPossibleRes[1941] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[56] - Fail";
    FullPossibleRes[1942] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[57] - Fail";
    FullPossibleRes[1943] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[58] - Fail";
    FullPossibleRes[1944] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[59] - Fail";
    FullPossibleRes[1945] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[60] - Fail";
    FullPossibleRes[1946] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[61] - Fail";
    FullPossibleRes[1947] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[62] - Fail";
    FullPossibleRes[1948] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[63] - Fail";
    FullPossibleRes[1949] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[64] - Fail";
    FullPossibleRes[1950] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[65] - Fail";
    FullPossibleRes[1951] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[66] - Fail";
    FullPossibleRes[1952] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[67] - Fail";
    FullPossibleRes[1953] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[68] - Fail";
    FullPossibleRes[1954] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[69] - Fail";
    FullPossibleRes[1955] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[70] - Fail";
    FullPossibleRes[1956] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[71] - Fail";
    FullPossibleRes[1957] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[72] - Fail";
    FullPossibleRes[1958] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[73] - Fail";
    FullPossibleRes[1959] = "MD5 Checksums Identical for: vp8_block2context_LEFTABOVE[74] - Fail"; //Removed 2.0
    FullPossibleRes[1960] = "MD5 Checksums Identical for: vp8_block2left[0] - Fail";
    FullPossibleRes[1961] = "MD5 Checksums Identical for: vp8_block2left[1] - Fail";
    FullPossibleRes[1962] = "MD5 Checksums Identical for: vp8_block2left[2] - Fail";
    FullPossibleRes[1963] = "MD5 Checksums Identical for: vp8_block2left[3] - Fail";
    FullPossibleRes[1964] = "MD5 Checksums Identical for: vp8_block2left[4] - Fail";
    FullPossibleRes[1965] = "MD5 Checksums Identical for: vp8_block2left[5] - Fail";
    FullPossibleRes[1966] = "MD5 Checksums Identical for: vp8_block2left[6] - Fail";
    FullPossibleRes[1967] = "MD5 Checksums Identical for: vp8_block2left[7] - Fail";
    FullPossibleRes[1968] = "MD5 Checksums Identical for: vp8_block2left[8] - Fail";
    FullPossibleRes[1969] = "MD5 Checksums Identical for: vp8_block2left[9] - Fail";
    FullPossibleRes[1970] = "MD5 Checksums Identical for: vp8_block2left[10] - Fail";
    FullPossibleRes[1971] = "MD5 Checksums Identical for: vp8_block2left[11] - Fail";
    FullPossibleRes[1972] = "MD5 Checksums Identical for: vp8_block2left[12] - Fail";
    FullPossibleRes[1973] = "MD5 Checksums Identical for: vp8_block2left[13] - Fail";
    FullPossibleRes[1974] = "MD5 Checksums Identical for: vp8_block2left[14] - Fail";
    FullPossibleRes[1975] = "MD5 Checksums Identical for: vp8_block2left[15] - Fail";
    FullPossibleRes[1976] = "MD5 Checksums Identical for: vp8_block2left[16] - Fail";
    FullPossibleRes[1977] = "MD5 Checksums Identical for: vp8_block2left[17] - Fail";
    FullPossibleRes[1978] = "MD5 Checksums Identical for: vp8_block2left[18] - Fail";
    FullPossibleRes[1979] = "MD5 Checksums Identical for: vp8_block2left[19] - Fail";
    FullPossibleRes[1980] = "MD5 Checksums Identical for: vp8_block2left[20] - Fail";
    FullPossibleRes[1981] = "MD5 Checksums Identical for: vp8_block2left[21] - Fail";
    FullPossibleRes[1982] = "MD5 Checksums Identical for: vp8_block2left[22] - Fail";
    FullPossibleRes[1983] = "MD5 Checksums Identical for: vp8_block2left[23] - Fail";
    FullPossibleRes[1984] = "MD5 Checksums Identical for: vp8_block2left[24] - Fail";
    FullPossibleRes[1985] = "MD5 Checksums Identical for: vp8_block2type[0] - Fail";
    FullPossibleRes[1986] = "MD5 Checksums Identical for: vp8_block2type[1] - Fail";
    FullPossibleRes[1987] = "MD5 Checksums Identical for: vp8_block2type[2] - Fail";
    FullPossibleRes[1988] = "MD5 Checksums Identical for: vp8_block2type[3] - Fail";
    FullPossibleRes[1989] = "MD5 Checksums Identical for: vp8_block2type[4] - Fail";
    FullPossibleRes[1990] = "MD5 Checksums Identical for: vp8_block2type[5] - Fail";
    FullPossibleRes[1991] = "MD5 Checksums Identical for: vp8_block2type[6] - Fail";
    FullPossibleRes[1992] = "MD5 Checksums Identical for: vp8_block2type[7] - Fail";
    FullPossibleRes[1993] = "MD5 Checksums Identical for: vp8_block2type[8] - Fail";
    FullPossibleRes[1994] = "MD5 Checksums Identical for: vp8_block2type[9] - Fail";
    FullPossibleRes[1995] = "MD5 Checksums Identical for: vp8_block2type[10] - Fail";
    FullPossibleRes[1996] = "MD5 Checksums Identical for: vp8_block2type[11] - Fail";
    FullPossibleRes[1997] = "MD5 Checksums Identical for: vp8_block2type[12] - Fail";
    FullPossibleRes[1998] = "MD5 Checksums Identical for: vp8_block2type[13] - Fail";
    FullPossibleRes[1999] = "MD5 Checksums Identical for: vp8_block2type[14] - Fail";
    FullPossibleRes[2000] = "MD5 Checksums Identical for: vp8_block2type[15] - Fail";
    FullPossibleRes[2001] = "MD5 Checksums Identical for: vp8_block2type[16] - Fail";
    FullPossibleRes[2002] = "MD5 Checksums Identical for: vp8_block2type[17] - Fail";
    FullPossibleRes[2003] = "MD5 Checksums Identical for: vp8_block2type[18] - Fail";
    FullPossibleRes[2004] = "MD5 Checksums Identical for: vp8_block2type[19] - Fail";
    FullPossibleRes[2005] = "MD5 Checksums Identical for: vp8_block2type[20] - Fail";
    FullPossibleRes[2006] = "MD5 Checksums Identical for: vp8_block2type[21] - Fail";
    FullPossibleRes[2007] = "MD5 Checksums Identical for: vp8_block2type[22] - Fail";
    FullPossibleRes[2008] = "MD5 Checksums Identical for: vp8_block2type[23] - Fail";
    FullPossibleRes[2009] = "MD5 Checksums Identical for: vp8_block2type[24] - Fail";
    FullPossibleRes[2010] = "MD5 Checksums Identical for: vp8_coef_bands[0] - Fail";
    FullPossibleRes[2011] = "MD5 Checksums Identical for: vp8_coef_bands[1] - Fail";
    FullPossibleRes[2012] = "MD5 Checksums Identical for: vp8_coef_bands[2] - Fail";
    FullPossibleRes[2013] = "MD5 Checksums Identical for: vp8_coef_bands[3] - Fail";
    FullPossibleRes[2014] = "MD5 Checksums Identical for: vp8_coef_bands[4] - Fail";
    FullPossibleRes[2015] = "MD5 Checksums Identical for: vp8_coef_bands[5] - Fail";
    FullPossibleRes[2016] = "MD5 Checksums Identical for: vp8_coef_bands[6] - Fail";
    FullPossibleRes[2017] = "MD5 Checksums Identical for: vp8_coef_bands[7] - Fail";
    FullPossibleRes[2018] = "MD5 Checksums Identical for: vp8_coef_bands[8] - Fail";
    FullPossibleRes[2019] = "MD5 Checksums Identical for: vp8_coef_bands[9] - Fail";
    FullPossibleRes[2020] = "MD5 Checksums Identical for: vp8_coef_bands[10] - Fail";
    FullPossibleRes[2021] = "MD5 Checksums Identical for: vp8_coef_bands[11] - Fail";
    FullPossibleRes[2022] = "MD5 Checksums Identical for: vp8_coef_bands[12] - Fail";
    FullPossibleRes[2023] = "MD5 Checksums Identical for: vp8_coef_bands[13] - Fail";
    FullPossibleRes[2024] = "MD5 Checksums Identical for: vp8_coef_bands[14] - Fail";
    FullPossibleRes[2025] = "MD5 Checksums Identical for: vp8_coef_bands[15] - Fail";
    FullPossibleRes[2026] = "MD5 Checksums Identical for: vp8_coef_bands_x[0] - Fail";
    FullPossibleRes[2027] = "MD5 Checksums Identical for: vp8_coef_bands_x[1] - Fail";
    FullPossibleRes[2028] = "MD5 Checksums Identical for: vp8_coef_bands_x[2] - Fail";
    FullPossibleRes[2029] = "MD5 Checksums Identical for: vp8_coef_bands_x[3] - Fail";
    FullPossibleRes[2030] = "MD5 Checksums Identical for: vp8_coef_bands_x[4] - Fail";
    FullPossibleRes[2031] = "MD5 Checksums Identical for: vp8_coef_bands_x[5] - Fail";
    FullPossibleRes[2032] = "MD5 Checksums Identical for: vp8_coef_bands_x[6] - Fail";
    FullPossibleRes[2033] = "MD5 Checksums Identical for: vp8_coef_bands_x[7] - Fail";
    FullPossibleRes[2034] = "MD5 Checksums Identical for: vp8_coef_bands_x[8] - Fail";
    FullPossibleRes[2035] = "MD5 Checksums Identical for: vp8_coef_bands_x[9] - Fail";
    FullPossibleRes[2036] = "MD5 Checksums Identical for: vp8_coef_bands_x[10] - Fail";
    FullPossibleRes[2037] = "MD5 Checksums Identical for: vp8_coef_bands_x[11] - Fail";
    FullPossibleRes[2038] = "MD5 Checksums Identical for: vp8_coef_bands_x[12] - Fail";
    FullPossibleRes[2039] = "MD5 Checksums Identical for: vp8_coef_bands_x[13] - Fail";
    FullPossibleRes[2040] = "MD5 Checksums Identical for: vp8_coef_bands_x[14] - Fail";
    FullPossibleRes[2041] = "MD5 Checksums Identical for: vp8_coef_bands_x[15] - Fail";
    FullPossibleRes[2042] = "MD5 Checksums Identical for: Pcat1[0] - Fail";
    FullPossibleRes[2043] = "MD5 Checksums Identical for: Pcat2[0] - Fail";
    FullPossibleRes[2044] = "MD5 Checksums Identical for: Pcat2[1] - Fail";
    FullPossibleRes[2045] = "MD5 Checksums Identical for: Pcat3[0] - Fail";
    FullPossibleRes[2046] = "MD5 Checksums Identical for: Pcat3[1] - Fail";
    FullPossibleRes[2047] = "MD5 Checksums Identical for: Pcat3[2] - Fail";
    FullPossibleRes[2048] = "MD5 Checksums Identical for: Pcat4[0] - Fail";
    FullPossibleRes[2049] = "MD5 Checksums Identical for: Pcat4[1] - Fail";
    FullPossibleRes[2050] = "MD5 Checksums Identical for: Pcat4[2] - Fail";
    FullPossibleRes[2051] = "MD5 Checksums Identical for: Pcat4[3] - Fail";
    FullPossibleRes[2052] = "MD5 Checksums Identical for: Pcat5[0] - Fail";
    FullPossibleRes[2053] = "MD5 Checksums Identical for: Pcat5[1] - Fail";
    FullPossibleRes[2054] = "MD5 Checksums Identical for: Pcat5[2] - Fail";
    FullPossibleRes[2055] = "MD5 Checksums Identical for: Pcat5[3] - Fail";
    FullPossibleRes[2056] = "MD5 Checksums Identical for: Pcat5[4] - Fail";
    FullPossibleRes[2057] = "MD5 Checksums Identical for: Pcat6[0] - Fail";
    FullPossibleRes[2058] = "MD5 Checksums Identical for: Pcat6[1] - Fail";
    FullPossibleRes[2059] = "MD5 Checksums Identical for: Pcat6[2] - Fail";
    FullPossibleRes[2060] = "MD5 Checksums Identical for: Pcat6[3] - Fail";
    FullPossibleRes[2061] = "MD5 Checksums Identical for: Pcat6[4] - Fail";
    FullPossibleRes[2062] = "MD5 Checksums Identical for: Pcat6[5] - Fail";
    FullPossibleRes[2063] = "MD5 Checksums Identical for: Pcat6[6] - Fail";
    FullPossibleRes[2064] = "MD5 Checksums Identical for: Pcat6[7] - Fail";
    FullPossibleRes[2065] = "MD5 Checksums Identical for: Pcat6[8] - Fail";
    FullPossibleRes[2066] = "MD5 Checksums Identical for: Pcat6[9] - Fail";
    FullPossibleRes[2067] = "MD5 Checksums Identical for: Pcat6[10] - Fail";
    FullPossibleRes[2068] = "MD5 Checksums Identical for: vp8_prev_token_class[0] - Fail";
    FullPossibleRes[2069] = "MD5 Checksums Identical for: vp8_prev_token_class[1] - Fail";
    FullPossibleRes[2070] = "MD5 Checksums Identical for: vp8_prev_token_class[2] - Fail";
    FullPossibleRes[2071] = "MD5 Checksums Identical for: vp8_prev_token_class[3] - Fail";
    FullPossibleRes[2072] = "MD5 Checksums Identical for: vp8_prev_token_class[4] - Fail";
    FullPossibleRes[2073] = "MD5 Checksums Identical for: vp8_prev_token_class[5] - Fail";
    FullPossibleRes[2074] = "MD5 Checksums Identical for: vp8_prev_token_class[6] - Fail";
    FullPossibleRes[2075] = "MD5 Checksums Identical for: vp8_prev_token_class[7] - Fail";
    FullPossibleRes[2076] = "MD5 Checksums Identical for: vp8_prev_token_class[8] - Fail";
    FullPossibleRes[2077] = "MD5 Checksums Identical for: vp8_prev_token_class[9] - Fail";
    FullPossibleRes[2078] = "MD5 Checksums Identical for: vp8_prev_token_class[10] - Fail";
    FullPossibleRes[2079] = "MD5 Checksums Identical for: vp8_prev_token_class[11] - Fail";
    FullPossibleRes[2080] = "MD5 Checksums Identical for: vp8_rv[0] - Fail";
    FullPossibleRes[2081] = "MD5 Checksums Identical for: vp8_rv[1] - Fail";
    FullPossibleRes[2082] = "MD5 Checksums Identical for: vp8_rv[2] - Fail";
    FullPossibleRes[2083] = "MD5 Checksums Identical for: vp8_rv[3] - Fail";
    FullPossibleRes[2084] = "MD5 Checksums Identical for: vp8_rv[4] - Fail";
    FullPossibleRes[2085] = "MD5 Checksums Identical for: vp8_rv[5] - Fail";
    FullPossibleRes[2086] = "MD5 Checksums Identical for: vp8_rv[6] - Fail";
    FullPossibleRes[2087] = "MD5 Checksums Identical for: vp8_rv[7] - Fail";
    FullPossibleRes[2088] = "MD5 Checksums Identical for: vp8_rv[8] - Fail";
    FullPossibleRes[2089] = "MD5 Checksums Identical for: vp8_rv[9] - Fail";
    FullPossibleRes[2090] = "MD5 Checksums Identical for: vp8_rv[10] - Fail";
    FullPossibleRes[2091] = "MD5 Checksums Identical for: vp8_rv[11] - Fail";
    FullPossibleRes[2092] = "MD5 Checksums Identical for: vp8_rv[12] - Fail";
    FullPossibleRes[2093] = "MD5 Checksums Identical for: vp8_rv[13] - Fail";
    FullPossibleRes[2094] = "MD5 Checksums Identical for: vp8_rv[14] - Fail";
    FullPossibleRes[2095] = "MD5 Checksums Identical for: vp8_rv[15] - Fail";
    FullPossibleRes[2096] = "MD5 Checksums Identical for: vp8_rv[16] - Fail";
    FullPossibleRes[2097] = "MD5 Checksums Identical for: vp8_rv[17] - Fail";
    FullPossibleRes[2098] = "MD5 Checksums Identical for: vp8_rv[18] - Fail";
    FullPossibleRes[2099] = "MD5 Checksums Identical for: vp8_rv[19] - Fail";
    FullPossibleRes[2100] = "MD5 Checksums Identical for: vp8_rv[20] - Fail";
    FullPossibleRes[2101] = "MD5 Checksums Identical for: vp8_rv[21] - Fail";
    FullPossibleRes[2102] = "MD5 Checksums Identical for: vp8_rv[22] - Fail";
    FullPossibleRes[2103] = "MD5 Checksums Identical for: vp8_rv[23] - Fail";
    FullPossibleRes[2104] = "MD5 Checksums Identical for: vp8_rv[24] - Fail";
    FullPossibleRes[2105] = "MD5 Checksums Identical for: vp8_rv[25] - Fail";
    FullPossibleRes[2106] = "MD5 Checksums Identical for: vp8_rv[26] - Fail";
    FullPossibleRes[2107] = "MD5 Checksums Identical for: vp8_rv[27] - Fail";
    FullPossibleRes[2108] = "MD5 Checksums Identical for: vp8_rv[28] - Fail";
    FullPossibleRes[2109] = "MD5 Checksums Identical for: vp8_rv[29] - Fail";
    FullPossibleRes[2110] = "MD5 Checksums Identical for: vp8_rv[20] - Fail";
    FullPossibleRes[2111] = "MD5 Checksums Identical for: vp8_rv[21] - Fail";
    FullPossibleRes[2112] = "MD5 Checksums Identical for: vp8_rv[22] - Fail";
    FullPossibleRes[2113] = "MD5 Checksums Identical for: vp8_rv[23] - Fail";
    FullPossibleRes[2114] = "MD5 Checksums Identical for: vp8_rv[24] - Fail";
    FullPossibleRes[2115] = "MD5 Checksums Identical for: vp8_rv[25] - Fail";
    FullPossibleRes[2116] = "MD5 Checksums Identical for: vp8_rv[26] - Fail";
    FullPossibleRes[2117] = "MD5 Checksums Identical for: vp8_rv[27] - Fail";
    FullPossibleRes[2118] = "MD5 Checksums Identical for: vp8_rv[28] - Fail";
    FullPossibleRes[2119] = "MD5 Checksums Identical for: vp8_rv[29] - Fail";
    FullPossibleRes[2120] = "MD5 Checksums Identical for: vp8_rv[30] - Fail";
    FullPossibleRes[2121] = "MD5 Checksums Identical for: vp8_rv[31] - Fail";
    FullPossibleRes[2122] = "MD5 Checksums Identical for: vp8_rv[32] - Fail";
    FullPossibleRes[2123] = "MD5 Checksums Identical for: vp8_rv[33] - Fail";
    FullPossibleRes[2124] = "MD5 Checksums Identical for: vp8_rv[34] - Fail";
    FullPossibleRes[2125] = "MD5 Checksums Identical for: vp8_rv[35] - Fail";
    FullPossibleRes[2126] = "MD5 Checksums Identical for: vp8_rv[36] - Fail";
    FullPossibleRes[2127] = "MD5 Checksums Identical for: vp8_rv[37] - Fail";
    FullPossibleRes[2128] = "MD5 Checksums Identical for: vp8_rv[38] - Fail";
    FullPossibleRes[2129] = "MD5 Checksums Identical for: vp8_rv[39] - Fail";
    FullPossibleRes[2130] = "MD5 Checksums Identical for: vp8_rv[40] - Fail";
    FullPossibleRes[2131] = "MD5 Checksums Identical for: vp8_rv[41] - Fail";
    FullPossibleRes[2132] = "MD5 Checksums Identical for: vp8_rv[42] - Fail";
    FullPossibleRes[2133] = "MD5 Checksums Identical for: vp8_rv[43] - Fail";
    FullPossibleRes[2134] = "MD5 Checksums Identical for: vp8_rv[44] - Fail";
    FullPossibleRes[2135] = "MD5 Checksums Identical for: vp8_rv[45] - Fail";
    FullPossibleRes[2136] = "MD5 Checksums Identical for: vp8_rv[46] - Fail";
    FullPossibleRes[2137] = "MD5 Checksums Identical for: vp8_rv[47] - Fail";
    FullPossibleRes[2138] = "MD5 Checksums Identical for: vp8_rv[48] - Fail";
    FullPossibleRes[2139] = "MD5 Checksums Identical for: vp8_rv[49] - Fail";
    FullPossibleRes[2140] = "MD5 Checksums Identical for: vp8_rv[50] - Fail";
    FullPossibleRes[2141] = "MD5 Checksums Identical for: vp8_rv[51] - Fail";
    FullPossibleRes[2142] = "MD5 Checksums Identical for: vp8_rv[52] - Fail";
    FullPossibleRes[2143] = "MD5 Checksums Identical for: vp8_rv[53] - Fail";
    FullPossibleRes[2144] = "MD5 Checksums Identical for: vp8_rv[54] - Fail";
    FullPossibleRes[2145] = "MD5 Checksums Identical for: vp8_rv[55] - Fail";
    FullPossibleRes[2146] = "MD5 Checksums Identical for: vp8_rv[56] - Fail";
    FullPossibleRes[2147] = "MD5 Checksums Identical for: vp8_rv[57] - Fail";
    FullPossibleRes[2148] = "MD5 Checksums Identical for: vp8_rv[58] - Fail";
    FullPossibleRes[2149] = "MD5 Checksums Identical for: vp8_rv[59] - Fail";
    FullPossibleRes[2150] = "MD5 Checksums Identical for: vp8_rv[60] - Fail";
    FullPossibleRes[2151] = "MD5 Checksums Identical for: vp8_rv[61] - Fail";
    FullPossibleRes[2152] = "MD5 Checksums Identical for: vp8_rv[62] - Fail";
    FullPossibleRes[2153] = "MD5 Checksums Identical for: vp8_rv[63] - Fail";
    FullPossibleRes[2154] = "MD5 Checksums Identical for: vp8_rv[64] - Fail";
    FullPossibleRes[2155] = "MD5 Checksums Identical for: vp8_rv[65] - Fail";
    FullPossibleRes[2156] = "MD5 Checksums Identical for: vp8_rv[66] - Fail";
    FullPossibleRes[2157] = "MD5 Checksums Identical for: vp8_rv[67] - Fail";
    FullPossibleRes[2158] = "MD5 Checksums Identical for: vp8_rv[68] - Fail";
    FullPossibleRes[2159] = "MD5 Checksums Identical for: vp8_rv[69] - Fail";
    FullPossibleRes[2160] = "MD5 Checksums Identical for: vp8_rv[70] - Fail";
    FullPossibleRes[2161] = "MD5 Checksums Identical for: vp8_rv[71] - Fail";
    FullPossibleRes[2162] = "MD5 Checksums Identical for: vp8_rv[72] - Fail";
    FullPossibleRes[2163] = "MD5 Checksums Identical for: vp8_rv[73] - Fail";
    FullPossibleRes[2164] = "MD5 Checksums Identical for: vp8_rv[74] - Fail";
    FullPossibleRes[2165] = "MD5 Checksums Identical for: vp8_rv[75] - Fail";
    FullPossibleRes[2166] = "MD5 Checksums Identical for: vp8_rv[76] - Fail";
    FullPossibleRes[2167] = "MD5 Checksums Identical for: vp8_rv[77] - Fail";
    FullPossibleRes[2168] = "MD5 Checksums Identical for: vp8_rv[78] - Fail";
    FullPossibleRes[2169] = "MD5 Checksums Identical for: vp8_rv[79] - Fail";
    FullPossibleRes[2170] = "MD5 Checksums Identical for: vp8_rv[80] - Fail";
    FullPossibleRes[2171] = "MD5 Checksums Identical for: vp8_rv[81] - Fail";
    FullPossibleRes[2172] = "MD5 Checksums Identical for: vp8_rv[82] - Fail";
    FullPossibleRes[2173] = "MD5 Checksums Identical for: vp8_rv[83] - Fail";
    FullPossibleRes[2174] = "MD5 Checksums Identical for: vp8_rv[84] - Fail";
    FullPossibleRes[2175] = "MD5 Checksums Identical for: vp8_rv[85] - Fail";
    FullPossibleRes[2176] = "MD5 Checksums Identical for: vp8_rv[86] - Fail";
    FullPossibleRes[2177] = "MD5 Checksums Identical for: vp8_rv[87] - Fail";
    FullPossibleRes[2178] = "MD5 Checksums Identical for: vp8_rv[88] - Fail";
    FullPossibleRes[2179] = "MD5 Checksums Identical for: vp8_rv[89] - Fail";
    FullPossibleRes[2180] = "MD5 Checksums Identical for: vp8_rv[90] - Fail";
    FullPossibleRes[2181] = "MD5 Checksums Identical for: vp8_rv[91] - Fail";
    FullPossibleRes[2182] = "MD5 Checksums Identical for: vp8_rv[92] - Fail";
    FullPossibleRes[2183] = "MD5 Checksums Identical for: vp8_rv[93] - Fail";
    FullPossibleRes[2184] = "MD5 Checksums Identical for: vp8_rv[94] - Fail";
    FullPossibleRes[2185] = "MD5 Checksums Identical for: vp8_rv[95] - Fail";
    FullPossibleRes[2186] = "MD5 Checksums Identical for: vp8_rv[96] - Fail";
    FullPossibleRes[2187] = "MD5 Checksums Identical for: vp8_rv[97] - Fail";
    FullPossibleRes[2188] = "MD5 Checksums Identical for: vp8_rv[98] - Fail";
    FullPossibleRes[2189] = "MD5 Checksums Identical for: vp8_rv[99] - Fail";
    FullPossibleRes[2190] = "MD5 Checksums Identical for: vp8_rv[100] - Fail";
    FullPossibleRes[2191] = "MD5 Checksums Identical for: vp8_rv[101] - Fail";
    FullPossibleRes[2192] = "MD5 Checksums Identical for: vp8_rv[102] - Fail";
    FullPossibleRes[2193] = "MD5 Checksums Identical for: vp8_rv[103] - Fail";
    FullPossibleRes[2194] = "MD5 Checksums Identical for: vp8_rv[104] - Fail";
    FullPossibleRes[2195] = "MD5 Checksums Identical for: vp8_rv[105] - Fail";
    FullPossibleRes[2196] = "MD5 Checksums Identical for: vp8_rv[106] - Fail";
    FullPossibleRes[2197] = "MD5 Checksums Identical for: vp8_rv[107] - Fail";
    FullPossibleRes[2198] = "MD5 Checksums Identical for: vp8_rv[108] - Fail";
    FullPossibleRes[2199] = "MD5 Checksums Identical for: vp8_rv[109] - Fail";
    FullPossibleRes[2200] = "MD5 Checksums Identical for: vp8_rv[110] - Fail";
    FullPossibleRes[2201] = "MD5 Checksums Identical for: vp8_rv[111] - Fail";
    FullPossibleRes[2202] = "MD5 Checksums Identical for: vp8_rv[112] - Fail";
    FullPossibleRes[2203] = "MD5 Checksums Identical for: vp8_rv[113] - Fail";
    FullPossibleRes[2204] = "MD5 Checksums Identical for: vp8_rv[114] - Fail";
    FullPossibleRes[2205] = "MD5 Checksums Identical for: vp8_rv[115] - Fail";
    FullPossibleRes[2206] = "MD5 Checksums Identical for: vp8_rv[116] - Fail";
    FullPossibleRes[2207] = "MD5 Checksums Identical for: vp8_rv[117] - Fail";
    FullPossibleRes[2208] = "MD5 Checksums Identical for: vp8_rv[118] - Fail";
    FullPossibleRes[2209] = "MD5 Checksums Identical for: vp8_rv[119] - Fail";
    FullPossibleRes[2210] = "MD5 Checksums Identical for: vp8_rv[120] - Fail";
    FullPossibleRes[2211] = "MD5 Checksums Identical for: vp8_rv[121] - Fail";
    FullPossibleRes[2212] = "MD5 Checksums Identical for: vp8_rv[122] - Fail";
    FullPossibleRes[2213] = "MD5 Checksums Identical for: vp8_rv[123] - Fail";
    FullPossibleRes[2214] = "MD5 Checksums Identical for: vp8_rv[124] - Fail";
    FullPossibleRes[2215] = "MD5 Checksums Identical for: vp8_rv[125] - Fail";
    FullPossibleRes[2216] = "MD5 Checksums Identical for: vp8_rv[126] - Fail";
    FullPossibleRes[2217] = "MD5 Checksums Identical for: vp8_rv[127] - Fail";
    FullPossibleRes[2218] = "MD5 Checksums Identical for: vp8_rv[128] - Fail";
    FullPossibleRes[2219] = "MD5 Checksums Identical for: vp8_rv[129] - Fail";
    FullPossibleRes[2220] = "MD5 Checksums Identical for: vp8_rv[130] - Fail";
    FullPossibleRes[2221] = "MD5 Checksums Identical for: vp8_rv[131] - Fail";
    FullPossibleRes[2222] = "MD5 Checksums Identical for: vp8_rv[132] - Fail";
    FullPossibleRes[2223] = "MD5 Checksums Identical for: vp8_rv[133] - Fail";
    FullPossibleRes[2224] = "MD5 Checksums Identical for: vp8_rv[134] - Fail";
    FullPossibleRes[2225] = "MD5 Checksums Identical for: vp8_rv[135] - Fail";
    FullPossibleRes[2226] = "MD5 Checksums Identical for: vp8_rv[136] - Fail";
    FullPossibleRes[2227] = "MD5 Checksums Identical for: vp8_rv[137] - Fail";
    FullPossibleRes[2228] = "MD5 Checksums Identical for: vp8_rv[138] - Fail";
    FullPossibleRes[2229] = "MD5 Checksums Identical for: vp8_rv[139] - Fail";
    FullPossibleRes[2230] = "MD5 Checksums Identical for: vp8_rv[140] - Fail";
    FullPossibleRes[2231] = "MD5 Checksums Identical for: vp8_rv[141] - Fail";
    FullPossibleRes[2232] = "MD5 Checksums Identical for: vp8_rv[142] - Fail";
    FullPossibleRes[2233] = "MD5 Checksums Identical for: vp8_rv[143] - Fail";
    FullPossibleRes[2234] = "MD5 Checksums Identical for: vp8_rv[144] - Fail";
    FullPossibleRes[2235] = "MD5 Checksums Identical for: vp8_rv[145] - Fail";
    FullPossibleRes[2236] = "MD5 Checksums Identical for: vp8_rv[146] - Fail";
    FullPossibleRes[2237] = "MD5 Checksums Identical for: vp8_rv[147] - Fail";
    FullPossibleRes[2238] = "MD5 Checksums Identical for: vp8_rv[148] - Fail";
    FullPossibleRes[2239] = "MD5 Checksums Identical for: vp8_rv[149] - Fail";
    FullPossibleRes[2240] = "MD5 Checksums Identical for: vp8_rv[150] - Fail";
    FullPossibleRes[2241] = "MD5 Checksums Identical for: vp8_rv[151] - Fail";
    FullPossibleRes[2242] = "MD5 Checksums Identical for: vp8_rv[152] - Fail";
    FullPossibleRes[2243] = "MD5 Checksums Identical for: vp8_rv[153] - Fail";
    FullPossibleRes[2244] = "MD5 Checksums Identical for: vp8_rv[154] - Fail";
    FullPossibleRes[2245] = "MD5 Checksums Identical for: vp8_rv[155] - Fail";
    FullPossibleRes[2246] = "MD5 Checksums Identical for: vp8_rv[156] - Fail";
    FullPossibleRes[2247] = "MD5 Checksums Identical for: vp8_rv[157] - Fail";
    FullPossibleRes[2248] = "MD5 Checksums Identical for: vp8_rv[158] - Fail";
    FullPossibleRes[2249] = "MD5 Checksums Identical for: vp8_rv[159] - Fail";
    FullPossibleRes[2250] = "MD5 Checksums Identical for: vp8_rv[160] - Fail";
    FullPossibleRes[2251] = "MD5 Checksums Identical for: vp8_rv[161] - Fail";
    FullPossibleRes[2252] = "MD5 Checksums Identical for: vp8_rv[162] - Fail";
    FullPossibleRes[2253] = "MD5 Checksums Identical for: vp8_rv[163] - Fail";
    FullPossibleRes[2254] = "MD5 Checksums Identical for: vp8_rv[164] - Fail";
    FullPossibleRes[2255] = "MD5 Checksums Identical for: vp8_rv[165] - Fail";
    FullPossibleRes[2256] = "MD5 Checksums Identical for: vp8_rv[166] - Fail";
    FullPossibleRes[2257] = "MD5 Checksums Identical for: vp8_rv[167] - Fail";
    FullPossibleRes[2258] = "MD5 Checksums Identical for: vp8_rv[168] - Fail";
    FullPossibleRes[2259] = "MD5 Checksums Identical for: vp8_rv[169] - Fail";
    FullPossibleRes[2260] = "MD5 Checksums Identical for: vp8_rv[170] - Fail";
    FullPossibleRes[2261] = "MD5 Checksums Identical for: vp8_rv[171] - Fail";
    FullPossibleRes[2262] = "MD5 Checksums Identical for: vp8_rv[172] - Fail";
    FullPossibleRes[2263] = "MD5 Checksums Identical for: vp8_rv[173] - Fail";
    FullPossibleRes[2264] = "MD5 Checksums Identical for: vp8_rv[174] - Fail";
    FullPossibleRes[2265] = "MD5 Checksums Identical for: vp8_rv[175] - Fail";
    FullPossibleRes[2266] = "MD5 Checksums Identical for: vp8_rv[176] - Fail";
    FullPossibleRes[2267] = "MD5 Checksums Identical for: vp8_rv[177] - Fail";
    FullPossibleRes[2268] = "MD5 Checksums Identical for: vp8_rv[178] - Fail";
    FullPossibleRes[2269] = "MD5 Checksums Identical for: vp8_rv[179] - Fail";
    FullPossibleRes[2270] = "MD5 Checksums Identical for: vp8_rv[180] - Fail";
    FullPossibleRes[2271] = "MD5 Checksums Identical for: vp8_rv[181] - Fail";
    FullPossibleRes[2272] = "MD5 Checksums Identical for: vp8_rv[182] - Fail";
    FullPossibleRes[2273] = "MD5 Checksums Identical for: vp8_rv[183] - Fail";
    FullPossibleRes[2274] = "MD5 Checksums Identical for: vp8_rv[184] - Fail";
    FullPossibleRes[2275] = "MD5 Checksums Identical for: vp8_rv[185] - Fail";
    FullPossibleRes[2276] = "MD5 Checksums Identical for: vp8_rv[186] - Fail";
    FullPossibleRes[2277] = "MD5 Checksums Identical for: vp8_rv[187] - Fail";
    FullPossibleRes[2278] = "MD5 Checksums Identical for: vp8_rv[188] - Fail";
    FullPossibleRes[2279] = "MD5 Checksums Identical for: vp8_rv[189] - Fail";
    FullPossibleRes[2280] = "MD5 Checksums Identical for: vp8_rv[190] - Fail";
    FullPossibleRes[2281] = "MD5 Checksums Identical for: vp8_rv[191] - Fail";
    FullPossibleRes[2282] = "MD5 Checksums Identical for: vp8_rv[192] - Fail";
    FullPossibleRes[2283] = "MD5 Checksums Identical for: vp8_rv[193] - Fail";
    FullPossibleRes[2284] = "MD5 Checksums Identical for: vp8_rv[194] - Fail";
    FullPossibleRes[2285] = "MD5 Checksums Identical for: vp8_rv[195] - Fail";
    FullPossibleRes[2286] = "MD5 Checksums Identical for: vp8_rv[196] - Fail";
    FullPossibleRes[2287] = "MD5 Checksums Identical for: vp8_rv[197] - Fail";
    FullPossibleRes[2288] = "MD5 Checksums Identical for: vp8_rv[198] - Fail";
    FullPossibleRes[2289] = "MD5 Checksums Identical for: vp8_rv[199] - Fail";
    FullPossibleRes[2290] = "MD5 Checksums Identical for: vp8_rv[200] - Fail";
    FullPossibleRes[2291] = "MD5 Checksums Identical for: vp8_rv[201] - Fail";
    FullPossibleRes[2292] = "MD5 Checksums Identical for: vp8_rv[202] - Fail";
    FullPossibleRes[2293] = "MD5 Checksums Identical for: vp8_rv[203] - Fail";
    FullPossibleRes[2294] = "MD5 Checksums Identical for: vp8_rv[204] - Fail";
    FullPossibleRes[2295] = "MD5 Checksums Identical for: vp8_rv[205] - Fail";
    FullPossibleRes[2296] = "MD5 Checksums Identical for: vp8_rv[206] - Fail";
    FullPossibleRes[2297] = "MD5 Checksums Identical for: vp8_rv[207] - Fail";
    FullPossibleRes[2298] = "MD5 Checksums Identical for: vp8_rv[208] - Fail";
    FullPossibleRes[2299] = "MD5 Checksums Identical for: vp8_rv[209] - Fail";
    FullPossibleRes[2300] = "MD5 Checksums Identical for: vp8_rv[210] - Fail";
    FullPossibleRes[2301] = "MD5 Checksums Identical for: vp8_rv[211] - Fail";
    FullPossibleRes[2302] = "MD5 Checksums Identical for: vp8_rv[212] - Fail";
    FullPossibleRes[2303] = "MD5 Checksums Identical for: vp8_rv[213] - Fail";
    FullPossibleRes[2304] = "MD5 Checksums Identical for: vp8_rv[214] - Fail";
    FullPossibleRes[2305] = "MD5 Checksums Identical for: vp8_rv[215] - Fail";
    FullPossibleRes[2306] = "MD5 Checksums Identical for: vp8_rv[216] - Fail";
    FullPossibleRes[2307] = "MD5 Checksums Identical for: vp8_rv[217] - Fail";
    FullPossibleRes[2308] = "MD5 Checksums Identical for: vp8_rv[218] - Fail";
    FullPossibleRes[2309] = "MD5 Checksums Identical for: vp8_rv[219] - Fail";
    FullPossibleRes[2310] = "MD5 Checksums Identical for: vp8_rv[220] - Fail";
    FullPossibleRes[2311] = "MD5 Checksums Identical for: vp8_rv[221] - Fail";
    FullPossibleRes[2312] = "MD5 Checksums Identical for: vp8_rv[222] - Fail";
    FullPossibleRes[2313] = "MD5 Checksums Identical for: vp8_rv[223] - Fail";
    FullPossibleRes[2314] = "MD5 Checksums Identical for: vp8_rv[224] - Fail";
    FullPossibleRes[2315] = "MD5 Checksums Identical for: vp8_rv[225] - Fail";
    FullPossibleRes[2316] = "MD5 Checksums Identical for: vp8_rv[226] - Fail";
    FullPossibleRes[2317] = "MD5 Checksums Identical for: vp8_rv[227] - Fail";
    FullPossibleRes[2318] = "MD5 Checksums Identical for: vp8_rv[228] - Fail";
    FullPossibleRes[2319] = "MD5 Checksums Identical for: vp8_rv[229] - Fail";
    FullPossibleRes[2320] = "MD5 Checksums Identical for: vp8_rv[230] - Fail";
    FullPossibleRes[2321] = "MD5 Checksums Identical for: vp8_rv[231] - Fail";
    FullPossibleRes[2322] = "MD5 Checksums Identical for: vp8_rv[232] - Fail";
    FullPossibleRes[2323] = "MD5 Checksums Identical for: vp8_rv[233] - Fail";
    FullPossibleRes[2324] = "MD5 Checksums Identical for: vp8_rv[234] - Fail";
    FullPossibleRes[2325] = "MD5 Checksums Identical for: vp8_rv[235] - Fail";
    FullPossibleRes[2326] = "MD5 Checksums Identical for: vp8_rv[236] - Fail";
    FullPossibleRes[2327] = "MD5 Checksums Identical for: vp8_rv[237] - Fail";
    FullPossibleRes[2328] = "MD5 Checksums Identical for: vp8_rv[238] - Fail";
    FullPossibleRes[2329] = "MD5 Checksums Identical for: vp8_rv[239] - Fail";
    FullPossibleRes[2330] = "MD5 Checksums Identical for: vp8_rv[240] - Fail";
    FullPossibleRes[2331] = "MD5 Checksums Identical for: vp8_rv[241] - Fail";
    FullPossibleRes[2332] = "MD5 Checksums Identical for: vp8_rv[242] - Fail";
    FullPossibleRes[2333] = "MD5 Checksums Identical for: vp8_rv[243] - Fail";
    FullPossibleRes[2334] = "MD5 Checksums Identical for: vp8_rv[244] - Fail";
    FullPossibleRes[2335] = "MD5 Checksums Identical for: vp8_rv[245] - Fail";
    FullPossibleRes[2336] = "MD5 Checksums Identical for: vp8_rv[246] - Fail";
    FullPossibleRes[2337] = "MD5 Checksums Identical for: vp8_rv[247] - Fail";
    FullPossibleRes[2338] = "MD5 Checksums Identical for: vp8_rv[248] - Fail";
    FullPossibleRes[2339] = "MD5 Checksums Identical for: vp8_rv[249] - Fail";
    FullPossibleRes[2340] = "MD5 Checksums Identical for: vp8_rv[250] - Fail";
    FullPossibleRes[2341] = "MD5 Checksums Identical for: vp8_rv[251] - Fail";
    FullPossibleRes[2342] = "MD5 Checksums Identical for: vp8_rv[252] - Fail";
    FullPossibleRes[2343] = "MD5 Checksums Identical for: vp8_rv[253] - Fail";
    FullPossibleRes[2344] = "MD5 Checksums Identical for: vp8_rv[254] - Fail";
    FullPossibleRes[2345] = "MD5 Checksums Identical for: vp8_rv[255] - Fail";
    FullPossibleRes[2346] = "MD5 Checksums Identical for: vp8_rv[256] - Fail";
    FullPossibleRes[2347] = "MD5 Checksums Identical for: vp8_rv[257] - Fail";
    FullPossibleRes[2348] = "MD5 Checksums Identical for: vp8_rv[258] - Fail";
    FullPossibleRes[2349] = "MD5 Checksums Identical for: vp8_rv[259] - Fail";
    FullPossibleRes[2350] = "MD5 Checksums Identical for: vp8_rv[260] - Fail";
    FullPossibleRes[2351] = "MD5 Checksums Identical for: vp8_rv[261] - Fail";
    FullPossibleRes[2352] = "MD5 Checksums Identical for: vp8_rv[262] - Fail";
    FullPossibleRes[2353] = "MD5 Checksums Identical for: vp8_rv[263] - Fail";
    FullPossibleRes[2354] = "MD5 Checksums Identical for: vp8_rv[264] - Fail";
    FullPossibleRes[2355] = "MD5 Checksums Identical for: vp8_rv[265] - Fail";
    FullPossibleRes[2356] = "MD5 Checksums Identical for: vp8_rv[266] - Fail";
    FullPossibleRes[2357] = "MD5 Checksums Identical for: vp8_rv[267] - Fail";
    FullPossibleRes[2358] = "MD5 Checksums Identical for: vp8_rv[268] - Fail";
    FullPossibleRes[2359] = "MD5 Checksums Identical for: vp8_rv[269] - Fail";
    FullPossibleRes[2360] = "MD5 Checksums Identical for: vp8_rv[270] - Fail";
    FullPossibleRes[2361] = "MD5 Checksums Identical for: vp8_rv[271] - Fail";
    FullPossibleRes[2362] = "MD5 Checksums Identical for: vp8_rv[272] - Fail";
    FullPossibleRes[2363] = "MD5 Checksums Identical for: vp8_rv[273] - Fail";
    FullPossibleRes[2364] = "MD5 Checksums Identical for: vp8_rv[274] - Fail";
    FullPossibleRes[2365] = "MD5 Checksums Identical for: vp8_rv[275] - Fail";
    FullPossibleRes[2366] = "MD5 Checksums Identical for: vp8_rv[276] - Fail";
    FullPossibleRes[2367] = "MD5 Checksums Identical for: vp8_rv[277] - Fail";
    FullPossibleRes[2368] = "MD5 Checksums Identical for: vp8_rv[278] - Fail";
    FullPossibleRes[2369] = "MD5 Checksums Identical for: vp8_rv[279] - Fail";
    FullPossibleRes[2370] = "MD5 Checksums Identical for: vp8_rv[280] - Fail";
    FullPossibleRes[2371] = "MD5 Checksums Identical for: vp8_rv[281] - Fail";
    FullPossibleRes[2372] = "MD5 Checksums Identical for: vp8_rv[282] - Fail";
    FullPossibleRes[2373] = "MD5 Checksums Identical for: vp8_rv[283] - Fail";
    FullPossibleRes[2374] = "MD5 Checksums Identical for: vp8_rv[284] - Fail";
    FullPossibleRes[2375] = "MD5 Checksums Identical for: vp8_rv[285] - Fail";
    FullPossibleRes[2376] = "MD5 Checksums Identical for: vp8_rv[286] - Fail";
    FullPossibleRes[2377] = "MD5 Checksums Identical for: vp8_rv[287] - Fail";
    FullPossibleRes[2378] = "MD5 Checksums Identical for: vp8_rv[288] - Fail";
    FullPossibleRes[2379] = "MD5 Checksums Identical for: vp8_rv[289] - Fail";
    FullPossibleRes[2380] = "MD5 Checksums Identical for: vp8_rv[290] - Fail";
    FullPossibleRes[2381] = "MD5 Checksums Identical for: vp8_rv[291] - Fail";
    FullPossibleRes[2382] = "MD5 Checksums Identical for: vp8_rv[292] - Fail";
    FullPossibleRes[2383] = "MD5 Checksums Identical for: vp8_rv[293] - Fail";
    FullPossibleRes[2384] = "MD5 Checksums Identical for: vp8_rv[294] - Fail";
    FullPossibleRes[2385] = "MD5 Checksums Identical for: vp8_rv[295] - Fail";
    FullPossibleRes[2386] = "MD5 Checksums Identical for: vp8_rv[296] - Fail";
    FullPossibleRes[2387] = "MD5 Checksums Identical for: vp8_rv[297] - Fail";
    FullPossibleRes[2388] = "MD5 Checksums Identical for: vp8_rv[298] - Fail";
    FullPossibleRes[2389] = "MD5 Checksums Identical for: vp8_rv[299] - Fail";
    FullPossibleRes[2390] = "MD5 Checksums Identical for: vp8_rv[300] - Fail";
    FullPossibleRes[2391] = "MD5 Checksums Identical for: vp8_rv[301] - Fail";
    FullPossibleRes[2392] = "MD5 Checksums Identical for: vp8_rv[302] - Fail";
    FullPossibleRes[2393] = "MD5 Checksums Identical for: vp8_rv[303] - Fail";
    FullPossibleRes[2394] = "MD5 Checksums Identical for: vp8_rv[304] - Fail";
    FullPossibleRes[2395] = "MD5 Checksums Identical for: vp8_rv[305] - Fail";
    FullPossibleRes[2396] = "MD5 Checksums Identical for: vp8_rv[306] - Fail";
    FullPossibleRes[2397] = "MD5 Checksums Identical for: vp8_rv[307] - Fail";
    FullPossibleRes[2398] = "MD5 Checksums Identical for: vp8_rv[308] - Fail";
    FullPossibleRes[2399] = "MD5 Checksums Identical for: vp8_rv[309] - Fail";
    FullPossibleRes[2400] = "MD5 Checksums Identical for: vp8_rv[310] - Fail";
    FullPossibleRes[2401] = "MD5 Checksums Identical for: vp8_rv[311] - Fail";
    FullPossibleRes[2402] = "MD5 Checksums Identical for: vp8_rv[312] - Fail";
    FullPossibleRes[2403] = "MD5 Checksums Identical for: vp8_rv[313] - Fail";
    FullPossibleRes[2404] = "MD5 Checksums Identical for: vp8_rv[314] - Fail";
    FullPossibleRes[2405] = "MD5 Checksums Identical for: vp8_rv[315] - Fail";
    FullPossibleRes[2406] = "MD5 Checksums Identical for: vp8_rv[316] - Fail";
    FullPossibleRes[2407] = "MD5 Checksums Identical for: vp8_rv[317] - Fail";
    FullPossibleRes[2408] = "MD5 Checksums Identical for: vp8_rv[318] - Fail";
    FullPossibleRes[2409] = "MD5 Checksums Identical for: vp8_rv[319] - Fail";
    FullPossibleRes[2410] = "MD5 Checksums Identical for: vp8_rv[320] - Fail";
    FullPossibleRes[2411] = "MD5 Checksums Identical for: vp8_rv[321] - Fail";
    FullPossibleRes[2412] = "MD5 Checksums Identical for: vp8_rv[322] - Fail";
    FullPossibleRes[2413] = "MD5 Checksums Identical for: vp8_rv[323] - Fail";
    FullPossibleRes[2414] = "MD5 Checksums Identical for: vp8_rv[324] - Fail";
    FullPossibleRes[2415] = "MD5 Checksums Identical for: vp8_rv[325] - Fail";
    FullPossibleRes[2416] = "MD5 Checksums Identical for: vp8_rv[326] - Fail";
    FullPossibleRes[2417] = "MD5 Checksums Identical for: vp8_rv[327] - Fail";
    FullPossibleRes[2418] = "MD5 Checksums Identical for: vp8_rv[328] - Fail";
    FullPossibleRes[2419] = "MD5 Checksums Identical for: vp8_rv[329] - Fail";
    FullPossibleRes[2420] = "MD5 Checksums Identical for: vp8_rv[330] - Fail";
    FullPossibleRes[2421] = "MD5 Checksums Identical for: vp8_rv[331] - Fail";
    FullPossibleRes[2422] = "MD5 Checksums Identical for: vp8_rv[332] - Fail";
    FullPossibleRes[2423] = "MD5 Checksums Identical for: vp8_rv[333] - Fail";
    FullPossibleRes[2424] = "MD5 Checksums Identical for: vp8_rv[334] - Fail";
    FullPossibleRes[2425] = "MD5 Checksums Identical for: vp8_rv[335] - Fail";
    FullPossibleRes[2426] = "MD5 Checksums Identical for: vp8_rv[336] - Fail";
    FullPossibleRes[2427] = "MD5 Checksums Identical for: vp8_rv[337] - Fail";
    FullPossibleRes[2428] = "MD5 Checksums Identical for: vp8_rv[338] - Fail";
    FullPossibleRes[2429] = "MD5 Checksums Identical for: vp8_rv[339] - Fail";
    FullPossibleRes[2430] = "MD5 Checksums Identical for: vp8_rv[340] - Fail";
    FullPossibleRes[2431] = "MD5 Checksums Identical for: vp8_rv[341] - Fail";
    FullPossibleRes[2432] = "MD5 Checksums Identical for: vp8_rv[342] - Fail";
    FullPossibleRes[2433] = "MD5 Checksums Identical for: vp8_rv[343] - Fail";
    FullPossibleRes[2434] = "MD5 Checksums Identical for: vp8_rv[344] - Fail";
    FullPossibleRes[2435] = "MD5 Checksums Identical for: vp8_rv[345] - Fail";
    FullPossibleRes[2436] = "MD5 Checksums Identical for: vp8_rv[346] - Fail";
    FullPossibleRes[2437] = "MD5 Checksums Identical for: vp8_rv[347] - Fail";
    FullPossibleRes[2438] = "MD5 Checksums Identical for: vp8_rv[348] - Fail";
    FullPossibleRes[2439] = "MD5 Checksums Identical for: vp8_rv[349] - Fail";
    FullPossibleRes[2440] = "MD5 Checksums Identical for: vp8_rv[350] - Fail";
    FullPossibleRes[2441] = "MD5 Checksums Identical for: vp8_rv[351] - Fail";
    FullPossibleRes[2442] = "MD5 Checksums Identical for: vp8_rv[352] - Fail";
    FullPossibleRes[2443] = "MD5 Checksums Identical for: vp8_rv[353] - Fail";
    FullPossibleRes[2444] = "MD5 Checksums Identical for: vp8_rv[354] - Fail";
    FullPossibleRes[2445] = "MD5 Checksums Identical for: vp8_rv[355] - Fail";
    FullPossibleRes[2446] = "MD5 Checksums Identical for: vp8_rv[356] - Fail";
    FullPossibleRes[2447] = "MD5 Checksums Identical for: vp8_rv[357] - Fail";
    FullPossibleRes[2448] = "MD5 Checksums Identical for: vp8_rv[358] - Fail";
    FullPossibleRes[2449] = "MD5 Checksums Identical for: vp8_rv[359] - Fail";
    FullPossibleRes[2450] = "MD5 Checksums Identical for: vp8_rv[360] - Fail";
    FullPossibleRes[2451] = "MD5 Checksums Identical for: vp8_rv[361] - Fail";
    FullPossibleRes[2452] = "MD5 Checksums Identical for: vp8_rv[362] - Fail";
    FullPossibleRes[2453] = "MD5 Checksums Identical for: vp8_rv[363] - Fail";
    FullPossibleRes[2454] = "MD5 Checksums Identical for: vp8_rv[364] - Fail";
    FullPossibleRes[2455] = "MD5 Checksums Identical for: vp8_rv[365] - Fail";
    FullPossibleRes[2456] = "MD5 Checksums Identical for: vp8_rv[366] - Fail";
    FullPossibleRes[2457] = "MD5 Checksums Identical for: vp8_rv[367] - Fail";
    FullPossibleRes[2458] = "MD5 Checksums Identical for: vp8_rv[368] - Fail";
    FullPossibleRes[2459] = "MD5 Checksums Identical for: vp8_rv[369] - Fail";
    FullPossibleRes[2460] = "MD5 Checksums Identical for: vp8_rv[370] - Fail";
    FullPossibleRes[2461] = "MD5 Checksums Identical for: vp8_rv[371] - Fail";
    FullPossibleRes[2462] = "MD5 Checksums Identical for: vp8_rv[372] - Fail";
    FullPossibleRes[2463] = "MD5 Checksums Identical for: vp8_rv[373] - Fail";
    FullPossibleRes[2464] = "MD5 Checksums Identical for: vp8_rv[374] - Fail";
    FullPossibleRes[2465] = "MD5 Checksums Identical for: vp8_rv[375] - Fail";
    FullPossibleRes[2466] = "MD5 Checksums Identical for: vp8_rv[376] - Fail";
    FullPossibleRes[2467] = "MD5 Checksums Identical for: vp8_rv[377] - Fail";
    FullPossibleRes[2468] = "MD5 Checksums Identical for: vp8_rv[378] - Fail";
    FullPossibleRes[2469] = "MD5 Checksums Identical for: vp8_rv[379] - Fail";
    FullPossibleRes[2470] = "MD5 Checksums Identical for: vp8_rv[380] - Fail";
    FullPossibleRes[2471] = "MD5 Checksums Identical for: vp8_rv[381] - Fail";
    FullPossibleRes[2472] = "MD5 Checksums Identical for: vp8_rv[382] - Fail";
    FullPossibleRes[2473] = "MD5 Checksums Identical for: vp8_rv[383] - Fail";
    FullPossibleRes[2474] = "MD5 Checksums Identical for: vp8_rv[384] - Fail";
    FullPossibleRes[2475] = "MD5 Checksums Identical for: vp8_rv[385] - Fail";
    FullPossibleRes[2476] = "MD5 Checksums Identical for: vp8_rv[386] - Fail";
    FullPossibleRes[2477] = "MD5 Checksums Identical for: vp8_rv[387] - Fail";
    FullPossibleRes[2478] = "MD5 Checksums Identical for: vp8_rv[388] - Fail";
    FullPossibleRes[2479] = "MD5 Checksums Identical for: vp8_rv[389] - Fail";
    FullPossibleRes[2480] = "MD5 Checksums Identical for: vp8_rv[390] - Fail";
    FullPossibleRes[2481] = "MD5 Checksums Identical for: vp8_rv[391] - Fail";
    FullPossibleRes[2482] = "MD5 Checksums Identical for: vp8_rv[392] - Fail";
    FullPossibleRes[2483] = "MD5 Checksums Identical for: vp8_rv[393] - Fail";
    FullPossibleRes[2484] = "MD5 Checksums Identical for: vp8_rv[394] - Fail";
    FullPossibleRes[2485] = "MD5 Checksums Identical for: vp8_rv[395] - Fail";
    FullPossibleRes[2486] = "MD5 Checksums Identical for: vp8_rv[396] - Fail";
    FullPossibleRes[2487] = "MD5 Checksums Identical for: vp8_rv[397] - Fail";
    FullPossibleRes[2488] = "MD5 Checksums Identical for: vp8_rv[398] - Fail";
    FullPossibleRes[2489] = "MD5 Checksums Identical for: vp8_rv[399] - Fail";
    FullPossibleRes[2490] = "MD5 Checksums Identical for: vp8_rv[400] - Fail";
    FullPossibleRes[2491] = "MD5 Checksums Identical for: vp8_rv[401] - Fail";
    FullPossibleRes[2492] = "MD5 Checksums Identical for: vp8_rv[402] - Fail";
    FullPossibleRes[2493] = "MD5 Checksums Identical for: vp8_rv[403] - Fail";
    FullPossibleRes[2494] = "MD5 Checksums Identical for: vp8_rv[404] - Fail";
    FullPossibleRes[2495] = "MD5 Checksums Identical for: vp8_rv[405] - Fail";
    FullPossibleRes[2496] = "MD5 Checksums Identical for: vp8_rv[406] - Fail";
    FullPossibleRes[2497] = "MD5 Checksums Identical for: vp8_rv[407] - Fail";
    FullPossibleRes[2498] = "MD5 Checksums Identical for: vp8_rv[408] - Fail";
    FullPossibleRes[2499] = "MD5 Checksums Identical for: vp8_rv[409] - Fail";
    FullPossibleRes[2500] = "MD5 Checksums Identical for: vp8_rv[410] - Fail";
    FullPossibleRes[2501] = "MD5 Checksums Identical for: vp8_rv[411] - Fail";
    FullPossibleRes[2502] = "MD5 Checksums Identical for: vp8_rv[412] - Fail";
    FullPossibleRes[2503] = "MD5 Checksums Identical for: vp8_rv[413] - Fail";
    FullPossibleRes[2504] = "MD5 Checksums Identical for: vp8_rv[414] - Fail";
    FullPossibleRes[2505] = "MD5 Checksums Identical for: vp8_rv[415] - Fail";
    FullPossibleRes[2506] = "MD5 Checksums Identical for: vp8_rv[416] - Fail";
    FullPossibleRes[2507] = "MD5 Checksums Identical for: vp8_rv[417] - Fail";
    FullPossibleRes[2508] = "MD5 Checksums Identical for: vp8_rv[418] - Fail";
    FullPossibleRes[2509] = "MD5 Checksums Identical for: vp8_rv[419] - Fail";
    FullPossibleRes[2510] = "MD5 Checksums Identical for: vp8_rv[420] - Fail";
    FullPossibleRes[2511] = "MD5 Checksums Identical for: vp8_rv[421] - Fail";
    FullPossibleRes[2512] = "MD5 Checksums Identical for: vp8_rv[422] - Fail";
    FullPossibleRes[2513] = "MD5 Checksums Identical for: vp8_rv[423] - Fail";
    FullPossibleRes[2514] = "MD5 Checksums Identical for: vp8_rv[424] - Fail";
    FullPossibleRes[2515] = "MD5 Checksums Identical for: vp8_rv[425] - Fail";
    FullPossibleRes[2516] = "MD5 Checksums Identical for: vp8_rv[426] - Fail";
    FullPossibleRes[2517] = "MD5 Checksums Identical for: vp8_rv[427] - Fail";
    FullPossibleRes[2518] = "MD5 Checksums Identical for: vp8_rv[428] - Fail";
    FullPossibleRes[2519] = "MD5 Checksums Identical for: vp8_rv[429] - Fail";
    FullPossibleRes[2520] = "MD5 Checksums Identical for: vp8_rv[430] - Fail";
    FullPossibleRes[2521] = "MD5 Checksums Identical for: vp8_rv[431] - Fail";
    FullPossibleRes[2522] = "MD5 Checksums Identical for: vp8_rv[432] - Fail";
    FullPossibleRes[2523] = "MD5 Checksums Identical for: vp8_rv[433] - Fail";
    FullPossibleRes[2524] = "MD5 Checksums Identical for: vp8_rv[434] - Fail";
    FullPossibleRes[2525] = "MD5 Checksums Identical for: vp8_rv[435] - Fail";
    FullPossibleRes[2526] = "MD5 Checksums Identical for: vp8_rv[436] - Fail";
    FullPossibleRes[2527] = "MD5 Checksums Identical for: vp8_rv[437] - Fail";
    FullPossibleRes[2528] = "MD5 Checksums Identical for: vp8_rv[438] - Fail";
    FullPossibleRes[2529] = "MD5 Checksums Identical for: vp8_rv[439] - Fail";
    FullPossibleRes[2530] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[0][0] - Fail"; //Removed 2.0
    FullPossibleRes[2531] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[1][0] - Fail";
    FullPossibleRes[2532] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[2][0] - Fail";
    FullPossibleRes[2533] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[3][0] - Fail";
    FullPossibleRes[2534] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[4][0] - Fail";
    FullPossibleRes[2535] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[0][1] - Fail";
    FullPossibleRes[2536] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[1][1] - Fail";
    FullPossibleRes[2537] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[2][1] - Fail";
    FullPossibleRes[2538] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[3][1] - Fail";
    FullPossibleRes[2539] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[4][1] - Fail";
    FullPossibleRes[2540] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[0][2] - Fail";
    FullPossibleRes[2541] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[1][2] - Fail";
    FullPossibleRes[2542] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[2][2] - Fail";
    FullPossibleRes[2543] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[3][2] - Fail";
    FullPossibleRes[2544] = "MD5 Checksums Identical for: vp8_sub_mv_ref_prob2[4][2] - Fail"; //Removed 2.0
    FullPossibleRes[2545] = "MD5 Checksums Identical for: sub_mv_ref_prob[0] - Fail";
    FullPossibleRes[2546] = "MD5 Checksums Identical for: sub_mv_ref_prob[1] - Fail";
    FullPossibleRes[2547] = "MD5 Checksums Identical for: sub_mv_ref_prob[2] - Fail";
    FullPossibleRes[2548] = "MD5 Checksums Identical for: sub_pel_filters[0][0] - Fail";
    FullPossibleRes[2549] = "MD5 Checksums Identical for: sub_pel_filters[1][0] - Fail";
    FullPossibleRes[2550] = "MD5 Checksums Identical for: sub_pel_filters[2][0] - Fail";
    FullPossibleRes[2551] = "MD5 Checksums Identical for: sub_pel_filters[3][0] - Fail";
    FullPossibleRes[2552] = "MD5 Checksums Identical for: sub_pel_filters[4][0] - Fail";
    FullPossibleRes[2553] = "MD5 Checksums Identical for: sub_pel_filters[5][0] - Fail";
    FullPossibleRes[2554] = "MD5 Checksums Identical for: sub_pel_filters[6][0] - Fail";
    FullPossibleRes[2555] = "MD5 Checksums Identical for: sub_pel_filters[7][0] - Fail";
    FullPossibleRes[2556] = "MD5 Checksums Identical for: sub_pel_filters[0][1] - Fail";
    FullPossibleRes[2557] = "MD5 Checksums Identical for: sub_pel_filters[1][1] - Fail";
    FullPossibleRes[2558] = "MD5 Checksums Identical for: sub_pel_filters[2][1] - Fail";
    FullPossibleRes[2559] = "MD5 Checksums Identical for: sub_pel_filters[3][1] - Fail";
    FullPossibleRes[2560] = "MD5 Checksums Identical for: sub_pel_filters[4][1] - Fail";
    FullPossibleRes[2561] = "MD5 Checksums Identical for: sub_pel_filters[5][1] - Fail";
    FullPossibleRes[2562] = "MD5 Checksums Identical for: sub_pel_filters[6][1] - Fail";
    FullPossibleRes[2563] = "MD5 Checksums Identical for: sub_pel_filters[7][1] - Fail";
    FullPossibleRes[2564] = "MD5 Checksums Identical for: sub_pel_filters[0][2] - Fail";
    FullPossibleRes[2565] = "MD5 Checksums Identical for: sub_pel_filters[1][2] - Fail";
    FullPossibleRes[2566] = "MD5 Checksums Identical for: sub_pel_filters[2][2] - Fail";
    FullPossibleRes[2567] = "MD5 Checksums Identical for: sub_pel_filters[3][2] - Fail";
    FullPossibleRes[2568] = "MD5 Checksums Identical for: sub_pel_filters[4][2] - Fail";
    FullPossibleRes[2569] = "MD5 Checksums Identical for: sub_pel_filters[5][2] - Fail";
    FullPossibleRes[2570] = "MD5 Checksums Identical for: sub_pel_filters[6][2] - Fail";
    FullPossibleRes[2571] = "MD5 Checksums Identical for: sub_pel_filters[7][2] - Fail";
    FullPossibleRes[2572] = "MD5 Checksums Identical for: sub_pel_filters[0][3] - Fail";
    FullPossibleRes[2573] = "MD5 Checksums Identical for: sub_pel_filters[1][3] - Fail";
    FullPossibleRes[2574] = "MD5 Checksums Identical for: sub_pel_filters[2][3] - Fail";
    FullPossibleRes[2575] = "MD5 Checksums Identical for: sub_pel_filters[3][3] - Fail";
    FullPossibleRes[2576] = "MD5 Checksums Identical for: sub_pel_filters[4][3] - Fail";
    FullPossibleRes[2577] = "MD5 Checksums Identical for: sub_pel_filters[5][3] - Fail";
    FullPossibleRes[2578] = "MD5 Checksums Identical for: sub_pel_filters[6][3] - Fail";
    FullPossibleRes[2579] = "MD5 Checksums Identical for: sub_pel_filters[7][3] - Fail";
    FullPossibleRes[2580] = "MD5 Checksums Identical for: sub_pel_filters[0][4] - Fail";
    FullPossibleRes[2581] = "MD5 Checksums Identical for: sub_pel_filters[1][4] - Fail";
    FullPossibleRes[2582] = "MD5 Checksums Identical for: sub_pel_filters[2][4] - Fail";
    FullPossibleRes[2583] = "MD5 Checksums Identical for: sub_pel_filters[3][4] - Fail";
    FullPossibleRes[2584] = "MD5 Checksums Identical for: sub_pel_filters[4][4] - Fail";
    FullPossibleRes[2585] = "MD5 Checksums Identical for: sub_pel_filters[5][4] - Fail";
    FullPossibleRes[2586] = "MD5 Checksums Identical for: sub_pel_filters[6][4] - Fail";
    FullPossibleRes[2587] = "MD5 Checksums Identical for: sub_pel_filters[7][4] - Fail";
    FullPossibleRes[2588] = "MD5 Checksums Identical for: sub_pel_filters[0][5] - Fail";
    FullPossibleRes[2589] = "MD5 Checksums Identical for: sub_pel_filters[1][5] - Fail";
    FullPossibleRes[2590] = "MD5 Checksums Identical for: sub_pel_filters[2][5] - Fail";
    FullPossibleRes[2591] = "MD5 Checksums Identical for: sub_pel_filters[3][5] - Fail";
    FullPossibleRes[2592] = "MD5 Checksums Identical for: sub_pel_filters[4][5] - Fail";
    FullPossibleRes[2593] = "MD5 Checksums Identical for: sub_pel_filters[5][5] - Fail";
    FullPossibleRes[2594] = "MD5 Checksums Identical for: sub_pel_filters[6][5] - Fail";
    FullPossibleRes[2595] = "MD5 Checksums Identical for: sub_pel_filters[7][5] - Fail";
    FullPossibleRes[2596] = "MD5 Checksums Identical for: uv_mode_cts[0] - Fail";
    FullPossibleRes[2597] = "MD5 Checksums Identical for: uv_mode_cts[1] - Fail";
    FullPossibleRes[2598] = "MD5 Checksums Identical for: uv_mode_cts[2] - Fail";
    FullPossibleRes[2599] = "MD5 Checksums Identical for: uv_mode_cts[3] - Fail";
    FullPossibleRes[2600] = "MD5 Checksums Identical for: y_mode_cts[0] - Fail";
    FullPossibleRes[2601] = "MD5 Checksums Identical for: y_mode_cts[1] - Fail";
    FullPossibleRes[2602] = "MD5 Checksums Identical for: y_mode_cts[2] - Fail";
    FullPossibleRes[2603] = "MD5 Checksums Identical for: y_mode_cts[3] - Fail";
    FullPossibleRes[2604] = "MD5 Checksums Identical for: y_mode_cts[4] - Fail";
    FullPossibleRes[2605] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][0] - Fail";
    FullPossibleRes[2606] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][1] - Fail";
    FullPossibleRes[2607] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][0] - Fail";
    FullPossibleRes[2608] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][1] - Fail";
    FullPossibleRes[2609] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][2] - Fail";
    FullPossibleRes[2610] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][3] - Fail";
    FullPossibleRes[2611] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][4] - Fail";
    FullPossibleRes[2612] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][5] - Fail";
    FullPossibleRes[2613] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][6] - Fail";
    FullPossibleRes[2614] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][7] - Fail";
    FullPossibleRes[2615] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][8] - Fail";
    FullPossibleRes[2616] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][9] - Fail";
    FullPossibleRes[2617] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][10] - Fail";
    FullPossibleRes[2618] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[0][2][11] - Fail";
    FullPossibleRes[2619] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][0] - Fail";
    FullPossibleRes[2620] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][1] - Fail";
    FullPossibleRes[2621] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][0] - Fail";
    FullPossibleRes[2622] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][1] - Fail";
    FullPossibleRes[2623] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][2] - Fail";
    FullPossibleRes[2624] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][3] - Fail";
    FullPossibleRes[2625] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][4] - Fail";
    FullPossibleRes[2626] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][5] - Fail";
    FullPossibleRes[2627] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][6] - Fail";
    FullPossibleRes[2628] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][7] - Fail";
    FullPossibleRes[2629] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][8] - Fail";
    FullPossibleRes[2630] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][9] - Fail";
    FullPossibleRes[2631] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][10] - Fail";
    FullPossibleRes[2632] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[1][2][11] - Fail";
    FullPossibleRes[2633] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][0] - Fail";
    FullPossibleRes[2634] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][1] - Fail";
    FullPossibleRes[2635] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][0] - Fail";
    FullPossibleRes[2636] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][1] - Fail";
    FullPossibleRes[2637] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][2] - Fail";
    FullPossibleRes[2638] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][3] - Fail";
    FullPossibleRes[2639] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][4] - Fail";
    FullPossibleRes[2640] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][5] - Fail";
    FullPossibleRes[2641] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][6] - Fail";
    FullPossibleRes[2642] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][7] - Fail";
    FullPossibleRes[2643] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][8] - Fail";
    FullPossibleRes[2644] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][9] - Fail";
    FullPossibleRes[2645] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][10] - Fail";
    FullPossibleRes[2646] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[2][2][11] - Fail";
    FullPossibleRes[2647] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][0] - Fail";
    FullPossibleRes[2648] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][1] - Fail";
    FullPossibleRes[2649] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][0] - Fail";
    FullPossibleRes[2650] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][1] - Fail";
    FullPossibleRes[2651] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][2] - Fail";
    FullPossibleRes[2652] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][3] - Fail";
    FullPossibleRes[2653] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][4] - Fail";
    FullPossibleRes[2654] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][5] - Fail";
    FullPossibleRes[2655] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][6] - Fail";
    FullPossibleRes[2656] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][7] - Fail";
    FullPossibleRes[2657] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][8] - Fail";
    FullPossibleRes[2658] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][9] - Fail";
    FullPossibleRes[2659] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][10] - Fail";
    FullPossibleRes[2660] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[3][2][11] - Fail";
    FullPossibleRes[2661] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][0] - Fail";
    FullPossibleRes[2662] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][1] - Fail";
    FullPossibleRes[2663] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][0] - Fail";
    FullPossibleRes[2664] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][1] - Fail";
    FullPossibleRes[2665] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][2] - Fail";
    FullPossibleRes[2666] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][3] - Fail";
    FullPossibleRes[2667] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][4] - Fail";
    FullPossibleRes[2668] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][5] - Fail";
    FullPossibleRes[2669] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][6] - Fail";
    FullPossibleRes[2670] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][7] - Fail";
    FullPossibleRes[2671] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][8] - Fail";
    FullPossibleRes[2672] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][9] - Fail";
    FullPossibleRes[2673] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][10] - Fail";
    FullPossibleRes[2674] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[4][2][11] - Fail";
    FullPossibleRes[2675] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][0] - Fail";
    FullPossibleRes[2676] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][1] - Fail";
    FullPossibleRes[2677] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][0] - Fail";
    FullPossibleRes[2678] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][1] - Fail";
    FullPossibleRes[2679] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][2] - Fail";
    FullPossibleRes[2680] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][3] - Fail";
    FullPossibleRes[2681] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][4] - Fail";
    FullPossibleRes[2682] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][5] - Fail";
    FullPossibleRes[2683] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][6] - Fail";
    FullPossibleRes[2684] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][7] - Fail";
    FullPossibleRes[2685] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][8] - Fail";
    FullPossibleRes[2686] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][9] - Fail";
    FullPossibleRes[2687] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][10] - Fail";
    FullPossibleRes[2688] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[5][2][11] - Fail";
    FullPossibleRes[2689] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][0] - Fail";
    FullPossibleRes[2690] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][1] - Fail";
    FullPossibleRes[2691] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][0] - Fail";
    FullPossibleRes[2692] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][1] - Fail";
    FullPossibleRes[2693] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][2] - Fail";
    FullPossibleRes[2694] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][3] - Fail";
    FullPossibleRes[2695] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][4] - Fail";
    FullPossibleRes[2696] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][5] - Fail";
    FullPossibleRes[2697] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][6] - Fail";
    FullPossibleRes[2698] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][7] - Fail";
    FullPossibleRes[2699] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][8] - Fail";
    FullPossibleRes[2700] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][9] - Fail";
    FullPossibleRes[2701] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][10] - Fail";
    FullPossibleRes[2702] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[6][2][11] - Fail";
    FullPossibleRes[2703] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][0] - Fail";
    FullPossibleRes[2704] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][1] - Fail";
    FullPossibleRes[2705] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][0] - Fail";
    FullPossibleRes[2706] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][1] - Fail";
    FullPossibleRes[2707] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][2] - Fail";
    FullPossibleRes[2708] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][3] - Fail";
    FullPossibleRes[2709] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][4] - Fail";
    FullPossibleRes[2710] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][5] - Fail";
    FullPossibleRes[2711] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][6] - Fail";
    FullPossibleRes[2712] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][7] - Fail";
    FullPossibleRes[2713] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][8] - Fail";
    FullPossibleRes[2714] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][9] - Fail";
    FullPossibleRes[2715] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][10] - Fail";
    FullPossibleRes[2716] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[7][2][11] - Fail";
    FullPossibleRes[2717] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][0] - Fail";
    FullPossibleRes[2718] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][1] - Fail";
    FullPossibleRes[2719] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][0] - Fail";
    FullPossibleRes[2720] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][1] - Fail";
    FullPossibleRes[2721] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][2] - Fail";
    FullPossibleRes[2722] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][3] - Fail";
    FullPossibleRes[2723] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][4] - Fail";
    FullPossibleRes[2724] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][5] - Fail";
    FullPossibleRes[2725] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][6] - Fail";
    FullPossibleRes[2726] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][7] - Fail";
    FullPossibleRes[2727] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][8] - Fail";
    FullPossibleRes[2728] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][9] - Fail";
    FullPossibleRes[2729] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][10] - Fail";
    FullPossibleRes[2730] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[8][2][11] - Fail";
    FullPossibleRes[2731] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][0] - Fail";
    FullPossibleRes[2732] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][1] - Fail";
    FullPossibleRes[2733] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][0] - Fail";
    FullPossibleRes[2734] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][1] - Fail";
    FullPossibleRes[2735] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][2] - Fail";
    FullPossibleRes[2736] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][3] - Fail";
    FullPossibleRes[2737] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][4] - Fail";
    FullPossibleRes[2738] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][5] - Fail";
    FullPossibleRes[2739] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][6] - Fail";
    FullPossibleRes[2740] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][7] - Fail";
    FullPossibleRes[2741] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][8] - Fail";
    FullPossibleRes[2742] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][9] - Fail";
    FullPossibleRes[2743] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][10] - Fail";
    FullPossibleRes[2744] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[9][2][11] - Fail";
    FullPossibleRes[2745] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][0] - Fail";
    FullPossibleRes[2746] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][1] - Fail";
    FullPossibleRes[2747] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][0] - Fail";
    FullPossibleRes[2748] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][1] - Fail";
    FullPossibleRes[2749] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][2] - Fail";
    FullPossibleRes[2750] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][3] - Fail";
    FullPossibleRes[2751] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][4] - Fail";
    FullPossibleRes[2752] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][5] - Fail";
    FullPossibleRes[2753] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][6] - Fail";
    FullPossibleRes[2754] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][7] - Fail";
    FullPossibleRes[2755] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][8] - Fail";
    FullPossibleRes[2756] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][9] - Fail";
    FullPossibleRes[2757] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][10] - Fail";
    FullPossibleRes[2758] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[10][2][11] - Fail";
    FullPossibleRes[2759] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][0] - Fail";
    FullPossibleRes[2760] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][1] - Fail";
    FullPossibleRes[2761] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][0] - Fail";
    FullPossibleRes[2762] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][1] - Fail";
    FullPossibleRes[2763] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][2] - Fail";
    FullPossibleRes[2764] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][3] - Fail";
    FullPossibleRes[2765] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][4] - Fail";
    FullPossibleRes[2766] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][5] - Fail";
    FullPossibleRes[2767] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][6] - Fail";
    FullPossibleRes[2768] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][7] - Fail";
    FullPossibleRes[2769] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][8] - Fail";
    FullPossibleRes[2770] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][9] - Fail";
    FullPossibleRes[2771] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][10] - Fail";
    FullPossibleRes[2772] = "MD5 Checksums Identical for: vp8d_token_extra_bits2[11][2][11] - Fail";
    FullPossibleRes[2773] = "MD5 Checksums Identical for: nearB[0][0] - Fail"; //Removed 2.0
    FullPossibleRes[2774] = "MD5 Checksums Identical for: nearB[0][1] - Fail";
    FullPossibleRes[2775] = "MD5 Checksums Identical for: nearB[0][2] - Fail";
    FullPossibleRes[2776] = "MD5 Checksums Identical for: nearB[0][3] - Fail";
    FullPossibleRes[2777] = "MD5 Checksums Identical for: nearB[1][0] - Fail";
    FullPossibleRes[2778] = "MD5 Checksums Identical for: nearB[1][1] - Fail";
    FullPossibleRes[2779] = "MD5 Checksums Identical for: nearB[1][2] - Fail";
    FullPossibleRes[2780] = "MD5 Checksums Identical for: nearB[1][3] - Fail";
    FullPossibleRes[2781] = "MD5 Checksums Identical for: nearB[2][0] - Fail";
    FullPossibleRes[2782] = "MD5 Checksums Identical for: nearB[2][1] - Fail";
    FullPossibleRes[2783] = "MD5 Checksums Identical for: nearB[2][2] - Fail";
    FullPossibleRes[2784] = "MD5 Checksums Identical for: nearB[2][3] - Fail"; //Removed 2.0

    ifstream Infile1(argv[2]);
    fstream  Outfile;
    Outfile.open(argv[3], fstream::in | fstream::out | fstream::app);

    char InputChar1[9999];

    int x = 0;
    int y = 0;
    int z = 0;

    Infile1.getline(InputChar1, 9999);
    Infile1.getline(InputChar1, 9999);
    string InputChar1Str = InputChar1;


    cout << "\nCreating Text File" << "\n";

    while (x < 2785)
    {

        if (FullPossibleRes[x].compare(InputChar1) == 0) //if they are the same - Fail
        {
            string OutputStr = FullPossibleRes[x].substr(28, FullPossibleRes[x].length() - 28);
            Outfile << OutputStr << "\n";

            if (Infile1.eof())
            {
                InputChar1Str = "";
                snprintf(InputChar1, 9999, "%s", InputChar1Str.c_str());
                //strcpy(InputChar1,InputChar1Str.c_str());
            }
            else
            {
                Infile1.getline(InputChar1, 9999);
                InputChar1Str = InputChar1;
            }
        }
        else//if they are not the same - Pass
        {
            if (!(x >= 1412 && x <= 1431) && !(x >= 1728 && x <= 1735) && !(x >= 1744 && x <= 1749) && !(x >= 1778 && x <= 1800) && !(x >= 1885 && x <= 1959) && !(x >= 2530 && x <= 2544) && !(x >= 2773 && x <= 2784))
            {
                string TempBuffer = FullPossibleRes[x];
                TempBuffer.erase(TempBuffer.length() - 4, 4);
                TempBuffer.append("Pass");
                string OutputStr = TempBuffer.substr(28, TempBuffer.length() - 28);
                Outfile << OutputStr << "\n";
            }
        }

        x++;
    }

    cout << "Text File Created" << "\n";

    Infile1.close();
    Outfile.close();
    return 0;
}
int ArrayCovSummaryFile(int argc, char *argv[])
{
    if (!(argc == 6))
    {
        tprintf(PRINT_STD, "\n"
                "  ArrayCovSummaryFile \n\n"
                "    <Current Summary>\n"
                "    <TestVector Number>\n"
                "    <TestVector Summary>\n"
                "    <Output File>\n"
                "\n"
               );

        return 0;
    }

    char InputChar1[9999];
    char InputChar2[9999];

    string InputStr1 = argv[2];
    string InputStr2 = argv[4];
    string OutputStr = argv[5];

    string InputCharStr1;
    string InputCharStr2;



    ifstream Infile1(InputStr1.c_str());
    ifstream Infile2(InputStr2.c_str());
    fstream  Outfile;
    Outfile.open(OutputStr.c_str(), fstream::in | fstream::out | fstream::app);

    cout << "\n";

    int z = 0;

    while (z < 2785)
    {
        Infile1.getline(InputChar1, 9999);
        InputCharStr1 = InputChar1;

        Infile2.getline(InputChar2, 9999);
        InputCharStr2 = InputChar2;

        string InputCharStr1PassFail = InputCharStr1.substr(InputCharStr1.length() - 4, 4); //grab last four chars of line 1
        string InputCharStr2PassFail = InputCharStr2.substr(InputCharStr2.length() - 4, 4); //grab last four chars of line 2

        if (InputCharStr1PassFail.compare("Pass") == 0)
        {
            Outfile << InputChar1 << "\n";
        }
        else
        {
            if (InputCharStr2PassFail.compare("Pass") == 0)
            {
                Outfile << "TestVector" << argv[3] << InputChar2 << "\n";
                cout << "TestVector" << argv[3] << InputChar2 << "\n";
            }
            else
            {
                Outfile << InputChar1 << "\n";
            }

        }

        z++;
    }

    Infile1.close();
    Infile2.close();
    Outfile.close();

    return 0;
}
int WinMemMonFormat(int argc, char *argv[])
{
    //Useful for formatting data output by Memmonitor for windows piped to a text file
    //Use in combination with WinMemMonGraph to get data in a format condusive to graphing
    if (argc < 4)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input File 1>\n"
                "    <Output File>\n"
               );
        return 0;
    }

    char InputCharFormatChar[9999];
    char InputChar1[999999];
    char InputChar2[9999];
    string InputCharFormatStr;
    string InputCharStr1;
    string InputCharStr2;
    int MaxLength = 0;

    string InputStr1 = argv[2];
    string OutputStr = argv[3];

    ifstream Infile1(InputStr1.c_str());
    fstream  Outfile;
    Outfile.open(OutputStr.c_str(), fstream::in | fstream::out | fstream::app);

    while (!Infile1.eof())
    {
        Infile1.getline(InputChar1, 999999);
        InputCharStr1 = InputChar1;

        //parse through string find "memmon" if there isnt a return char before it put one there
        int MemMonPosCur = 0;
        int MemMonPosLast = 0;

        MemMonPosCur = InputCharStr1.find("MemMon", MemMonPosCur + 1); //ignore the first instance

        MemMonPosLast = MemMonPosCur;
        MemMonPosCur = InputCharStr1.find("MemMon", MemMonPosCur + 1);

        cout << "First Pos " << MemMonPosCur << "\n";

        if (MemMonPosCur != -1)
        {
            Outfile << InputCharStr1.substr(MemMonPosLast, MemMonPosCur - MemMonPosLast).c_str() << "\n";
            cout << MemMonPosCur << "\n";
            cout << InputCharStr1.substr(MemMonPosLast, MemMonPosCur - MemMonPosLast).c_str() << "\n";

            while (MemMonPosCur != -1)
            {
                MemMonPosLast = MemMonPosCur;
                MemMonPosCur = InputCharStr1.find("MemMon", MemMonPosCur + 1);
                Outfile << InputCharStr1.substr(MemMonPosLast, MemMonPosCur - MemMonPosLast).c_str() << "\n";
                cout << MemMonPosCur << "\n";
                cout << InputCharStr1.substr(MemMonPosLast, MemMonPosCur - MemMonPosLast).c_str() << "\n";
            }

            Outfile << InputCharStr1.substr(MemMonPosLast).c_str() << "\n";
            cout << InputCharStr1.substr(MemMonPosLast).c_str() << "\n";

        }
        else
        {
            Outfile << InputCharStr1 << "\n";
            cout << InputCharStr1 << "\n";
        }

    }

    Infile1.close();
    Outfile.close();

    return 0;

}
int WinMemMonGraph(int argc, char *argv[])
{
    //Use on WinMemMonFormat formatted material to obtain a minute to memusage graph
    //of a programs usage for memmonitor gathered data
    if (argc < 5)
    {
        tprintf(PRINT_STD,
                "\n  Delete IVF Files\n\n"
                "    <Input File 1>\n"
                "    <Output File>\n"
                "    <Time Interval In Seconds>\n"
               );
        return 0;
    }

    char InputCharFormatChar[9999];
    char InputChar1[9999];
    char InputChar2[9999];
    string InputCharFormatStr;
    string InputCharStr1;
    string InputCharStr2;
    int MaxLength = 0;

    string InputStr1 = argv[2];
    string OutputStr = argv[3];
    int OutPutTime = atoi(argv[4]);

    ifstream Infile1(InputStr1.c_str());
    fstream  Outfile;
    Outfile.open(OutputStr.c_str(), fstream::in | fstream::out | fstream::app);

    int MinCount = 0;
    int OutputBool = 0;
    int TimeCount = 0;

    while (!Infile1.eof())
    {
        Infile1.getline(InputChar1, 9999);
        InputCharStr1 = InputChar1;

        char MemUsage[999];

        if (InputCharStr1.find("MemMon") != -1)
        {
            MinCount ++;
            TimeCount++;

            if (TimeCount == OutPutTime)
            {
                TimeCount = 0;
                OutputBool = 1;
            }
        }

        if (InputCharStr1.find("WorkingSetSize:") != -1)
        {
            int SetSizePos = InputCharStr1.find("WorkingSetSize:");
            strncpy(MemUsage, InputCharStr1.substr(SetSizePos + 15, InputCharStr1.length() - 15 - SetSizePos - 1).c_str(), 999);
            cout << InputCharStr1.substr(SetSizePos + 15, InputCharStr1.length() - 15 - SetSizePos - 1).c_str() << "\n";
        }

        if (OutputBool == 1)
        {
            Outfile << MinCount / 60 << "\t" << MemUsage << "\n";
            OutputBool = 0;
        }

    }

    Infile1.close();
    Outfile.close();

    return 0;

}
int CreateRandParFile(int argc, char *argv[])
{
    if (argc < 3)
    {
        tprintf(PRINT_STD,
                "\n  RandomParFile\n\n"
                "    <Output Par File>\n"
               );
        return 0;
    }

    char *output = argv[2];

    VP8_CONFIG opt = vpxt_random_parameters(opt, "", 0);
    vpxt_output_settings(output, opt);

    tprintf(PRINT_STD, "\nSaved to: %s\n", output);

    return 0;
}
int RunIVFDec(int argc, char *argv[])
{
    int dummyargc = argc - 1;

    const char *DummyArgv[999];
    DummyArgv[0] = argv[0];

    int i = 2;

    //cout << "\n";
    while (i < argc)
    {

        //cout << "DummyArgv[" << i-1 << "] = argv[" << i << "] = " << argv[i] << "\n";
        DummyArgv[i-1] = argv[i];
        i++;
    }

    //cout << "\n";
    ivfdec(dummyargc, (const char **) DummyArgv);

    return 0;
}
int RunIVFEnc(int argc, char *argv[])
{
    int dummyargc = argc - 1;

    const char *DummyArgv[999];
    DummyArgv[0] = argv[0];

    int i = 2;

    //cout << "\n";
    while (i < argc)
    {

        //cout << "DummyArgv[" << i-1 << "] = argv[" << i << "] = " << argv[i] << "\n";
        DummyArgv[i-1] = argv[i];
        i++;
    }

    //cout << "\n";
    ivfenc(dummyargc, (const char **) DummyArgv);

    return 0;
}
int SolveQuad()
{
    float X1 = 0;
    float X2 = 0;
    float X3 = 0;

    float Y1 = 0;
    float Y2 = 0;
    float Y3 = 0;

    float A = 0;
    float B = 0;
    float C = 0;

    tprintf(PRINT_STD, "\nIntput X1: ");
    cin >> X1;
    tprintf(PRINT_STD, "Intput X2: ");
    cin >> X2;
    tprintf(PRINT_STD, "Intput X3: ");
    cin >> X3;

    tprintf(PRINT_STD, "Intput Y1: ");
    cin >> Y1;
    tprintf(PRINT_STD, "Intput Y2: ");
    cin >> Y2;
    tprintf(PRINT_STD, "Intput Y3: ");
    cin >> Y3;


    A = ((Y2 - Y1) * (X1 - X3) + (Y3 - Y1) * (X2 - X1)) / ((X1 - X3) * ((X2 * X2) - (X1 * X1)) + (X2 - X1) * ((X3 * X3) - (X1 * X1)));
    B = ((Y2 - Y1) - A * ((X2 * X2) - (X1 * X1))) / (X2 - X1);
    C = Y1 - A * (X1 * X1) - B * X1;

    cout << "\nA = " << A;
    cout << "\nB = " << B;
    cout << "\nC = " << C;

    tprintf(PRINT_STD, "\n\ny = %.2fx^2 + %.2fx + %.2f\n\n", A, B, C);

    return 0;
}

int PrintCPUInfo()
{
    int Simd_Caps = x86_simd_caps();

    //printf("\nSimd_Caps = %i\n",Simd_Caps);

    tprintf(PRINT_STD, "\nDetected CPU capabilities:\n");

    if ((Simd_Caps & HAS_MMX)    == HAS_MMX)   tprintf(PRINT_STD, "\n     MMX    Detected");

    if ((Simd_Caps & HAS_SSE)    == HAS_SSE)   tprintf(PRINT_STD, "\n     SSE    Detected");

    if ((Simd_Caps & HAS_SSE2)   == HAS_SSE2)  tprintf(PRINT_STD, "\n     SSE2   Detected");

    if ((Simd_Caps & HAS_SSE3)   == HAS_SSE3)  tprintf(PRINT_STD, "\n     SSE3   Detected");

    if ((Simd_Caps & HAS_SSSE3)  == HAS_SSSE3) tprintf(PRINT_STD, "\n     SSSE3  Detected");

    if ((Simd_Caps & HAS_SSE4_1) == HAS_SSE4_1)tprintf(PRINT_STD, "\n     SSE4_1 Detected");

    tprintf(PRINT_STD, "\n");

    return 0;
}
