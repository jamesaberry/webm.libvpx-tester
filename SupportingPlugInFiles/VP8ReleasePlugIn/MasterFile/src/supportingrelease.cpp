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
    int WriteIndFrames = 5;

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
            "  Compress Release                                 Decompress "
            "Release\n"
            "\n"
            "    <Inputfile>                                      <Inputfile>\n"
            "    <Outputfile>                                     <Outputfile>"
            "\n"
            "    <Par File Origin 7 VP7 8 VP8>\n"
            "    <Par File>\n"
            "    <Extra Commands>\n"
            "      <0 No Extra Commands>\n"
            "      <1 Run PSNR only>\n"
            "      <2 Record Compression Time only>\n"
            "      <3 Record Compression Time and Run PSNR>\n"
            "\n"
            "\n"
            "   Release Exe using: %s\n",
            vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
}
int supportingFileRunPSNR(char *inputFile, char *outputFile)
{
    double totalPsnr;
    std::cout << "\n\n";
    double ssimDummyVar = 0;
    int potential_artifact = kDontRunArtifactDetection;
    totalPsnr = vpxt_psnr(inputFile, outputFile, 0, PRINT_BTH, 1, 0, 0, 0, NULL,
        potential_artifact);

    std::string TextFilechar1 = "";
    vpxt_remove_file_extension(outputFile, TextFilechar1);
    TextFilechar1.append("psnr.txt");

    std::ofstream outfile2(TextFilechar1.c_str());
    outfile2 << totalPsnr;
    outfile2.close();

    return 0;
}
int main(int argc, char *argv[])
{
    std::string compress = "compress";
    std::string decompress = "decompress";
    std::string temp_scale_compress = "temp_scale_compress";

    if (argc < 3)
    {
        supportingReleaseOnError();
        return 0;
    }

    if (compress.compare(argv[1]) == 0)
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

        //check output type
        std::string EncExt;
        vpxt_get_file_extension(outputFile, EncExt);
        EncExt.erase(0, 1); //remove period

        tprintf(PRINT_BTH, "\nRelease Exe using: %s\n",
            vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));
        vpxt_time_compress(inputFile, outputFile, 0, opt.target_bandwidth, opt,
            "VP8 Release", 0, 0, CPUTick, EncExt);

        if (ExtraCommand == 1 || ExtraCommand == 3)
            supportingFileRunPSNR(inputFile, outputFile);

        return 0;
    }

    if (decompress.compare(argv[1]) == 0)
    {
        if (argc < 3)
        {
            supportingReleaseOnError();
            return 0;
        }

        tprintf(PRINT_BTH, "\nRelease Exe using: %s\n",
            vpx_codec_iface_name(&vpx_codec_vp8_dx_algo));

        char *inputFile = argv[2];
        char *outputFile = argv[3];
        unsigned int CPUTick = 0;

        //check output type
        std::string DecExt;
        vpxt_get_file_extension(outputFile, DecExt);
        DecExt.erase(0, 1); //remove period

        vpxt_decompress_time_and_output(inputFile, outputFile, CPUTick,
            DecExt, 1);

        return 0;
    }

    if (temp_scale_compress.compare(argv[1]) == 0)
    {
        if (argc < 14)
        {
            supportingReleaseOnError();
            return 0;
        }

        char *input_file = argv[2];
        char *output_file_base = argv[3];
        int layer_mode = atoi(argv[4]);
        int temp_scale_bitrate_0 = atoi(argv[5]);
        int temp_scale_bitrate_1 = atoi(argv[6]);
        int temp_scale_bitrate_2 = atoi(argv[7]);
        int temp_scale_bitrate_3 = atoi(argv[8]);
        int temp_scale_bitrate_4 = atoi(argv[9]);
        char * enc_format = argv[10];
        int ParVer = atoi(argv[11]);
        char *parfile = argv[12];
        int ExtraCommand = atoi(argv[13]);

        VP8_CONFIG opt;
        vpxt_default_parameters(opt);
        unsigned int CPUTick = 0;

        if (ParVer == 7)
            std::cout << "\n\nNot Yet Supported\n\n";

        if (ParVer == 8)
            opt = vpxt_input_settings(parfile);

        tprintf(PRINT_BTH, "\nRelease Exe using: %s\n",
            vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

        unsigned int scale_compress_time = vpxt_compress_scalable_patterns(
            input_file, output_file_base, 0, opt, "VP8",
            0, 0, enc_format, layer_mode, temp_scale_bitrate_0,
            temp_scale_bitrate_1, temp_scale_bitrate_2,
            temp_scale_bitrate_3, temp_scale_bitrate_4);


        std::string FullNameCpu = output_file_base;
        FullNameCpu.append("_compression_cpu_tick.txt");

        std::ofstream FullNameCpuFile(FullNameCpu.c_str());
        FullNameCpuFile << scale_compress_time;
        FullNameCpuFile.close();

        return 0;
    }

    supportingReleaseOnError();
    return 0;
}