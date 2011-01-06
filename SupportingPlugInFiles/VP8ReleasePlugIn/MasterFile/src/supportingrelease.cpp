#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_utilities.h"
#include "vp8cx.h"
#include "vp8dx.h"
#include "onyx.h"
#include "ivf.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdio>

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
        tprintf(PRINT_BTH, "\nInput file does not exist");
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
    std::cout << "\n";

    std::string OutputDirStrwithQuotes = outputDir;

    if (WriteIndFrames != 5)
    {
        OutputDirStrwithQuotes.append("\"");
        OutputDirStrwithQuotes.insert(0, "md \"");
        vpxt_make_dir_vpx(OutputDirStrwithQuotes);
    }

    char *inbuff = new char[ivfhRaw.width * ivfhRaw.height * 3/2];

    std::string outputDirStr2 = outputDir;
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

    std::cout << "\n\nConverting to Raw\n";

    while (currentVideoFrame < frameCount)
    {
        std::cout << ".";
        memset(inbuff, 0, ivfhRaw.width * ivfhRaw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        std::string outputDirStr = outputDir;
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

    std::cout << "\n";

    return 0;
}
void supportingReleaseOnError()
{
    tprintf(PRINT_STD, "\n"
            "  IVF VP8 Release\n\n"
            "    <Mode>\n"
            "      <compress>\n"
            "      <decompress>\n"
            "\n"
            "  Compress Release                                 Decompress Release\n"
            "\n"
            "    <Inputfile>                                      <Inputfile>\n"
            "    <Outputfile>                                     <Outputfile>\n"
            "    <Par File Origin 7 VP7 8 VP8>\n"
            "    <Par File>\n"
            "    <Extra Commands>\n"
            "      <0 No Extra Commands>\n"
            "      <1 Run PSNR only>\n"
            "      <2 Record Compression Time only>\n"
            "      <3 Record Compression Time and Run PSNR>\n"
            "\n"
            "\n"
            "   Release Exe using: %s\n", vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
}
int supportingFileRunPSNR(char *inputFile, char *outputFile)
{
    double totalPsnr;
    std::cout << "\n\n";
    double ssimDummyVar = 0;
    totalPsnr = vpxt_ivf_psnr(inputFile, outputFile, 0, 0, 1, NULL);

    char TextFilechar1[255];
    vpxt_remove_file_extension(outputFile, TextFilechar1);

    char *FullName = strcat(TextFilechar1, "psnr.txt");

    std::ofstream outfile2(FullName);
    outfile2 << totalPsnr;
    outfile2.close();

    return 0;
}
int main(int argc, char *argv[])
{
    std::string Compress = "compress";
    std::string Decompress = "decompress";

    if (argc < 3)
    {
        supportingReleaseOnError();
        return 0;
    }

    if (Compress.compare(argv[1]) == 0)
    {
        if (argc < 7)
        {
            supportingReleaseOnError();
            return 0;
        }

        char *inputFile = argv[2];
        char *outputFile = argv[3];
        int ParVer = atoi(argv[4]);
        char *parfile = argv[5];
        int ExtraCommand = atoi(argv[6]);

        VP8_CONFIG opt;
        vpxt_default_parameters(opt);
        unsigned int CPUTick = 0;

        if (ParVer == 7)
            std::cout << "\n\nNot Yet Supported\n\n";

        if (ParVer == 8)
            opt = vpxt_input_settings(parfile);

        tprintf(PRINT_BTH, "\nRelease Exe using: %s\n", vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
        vpxt_time_compress_ivf_to_ivf(inputFile, outputFile, 0, opt.target_bandwidth, opt, "VP8 Release", 0, 0, CPUTick, 0, 3, 3);

        if (ExtraCommand == 1 || ExtraCommand == 3)
            supportingFileRunPSNR(inputFile, outputFile);

        return 0;
    }

    if (Decompress.compare(argv[1]) == 0)
    {
        if (argc < 3)
        {
            supportingReleaseOnError();
            return 0;
        }

        tprintf(PRINT_BTH, "\nRelease Exe using: %s\n", vpx_codec_iface_name(&vpx_codec_vp8_dx_algo));

        char *inputFile = argv[2];
        char *outputFile = argv[3];
        unsigned int CPUTick = 0;

        vpxt_decompress_ivf_to_ivf_time_and_output(inputFile, outputFile, CPUTick);

        return 0;
    }

    supportingReleaseOnError();
    return 0;
}