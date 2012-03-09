#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_utilities.h"
#include "vpx_config.h"
#include "vpx_mem.h"
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
        vpxt_make_dir(OutputDirStrwithQuotes);
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
void supportingDebugOnError()
{
    tprintf(PRINT_STD, "\n"
            "  IVF VP8 Debug\n\n"
            "    <Mode>\n"
            "      <compress>\n"
            "      <decompress>\n"
            "      <memcompress>\n"
            "      <memdecompress>\n"
            "      <fauxcompress>\n"
            "      <fauxdecompress>\n"
            "\n"
            "  Compress Debug                                  Decompress Debug"
			"\n"
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
            "  Compress Mem Leak Check                    Decompress Mem Leak "
			"Check\n"
            "\n"
            "    <Inputfile>                                      <Inputfile>\n"
            "    <Outputfile>                                     <Outputfile>"
			"\n"
            "    <Par File Origin 7 VP7 8 VP8>                    <Memory "
			"Output File>\n"
            "    <Par File>\n"
            "    <Extra Commands>\n"
            "      <0 No Extra Commands>\n"
            "      <1 Run PSNR only>\n"
            "      <2 Record Compression Time only>\n"
            "      <3 Record Compression Time and Run PSNR>\n"
            "    <Memory Output File>\n"
            "\n"
            "  Faux Compress                                    Faux Decompres"
			"s\n"
            "\n"
			"    <mem output file>                                <mem output "
			"file>\n"
            "\n"
            "\n"
            "   Debug Exe using: %s\n",
			vpx_codec_iface_name(&vpx_codec_vp8_cx_algo)
           );
}
int supportingFileRunPSNR(char *inputFile, char *outputFile)
{
    double totalPsnr;
    std::cout << "\n\n";
    double ssimDummyVar = 0;
    totalPsnr = vpxt_psnr(inputFile, outputFile, 0, PRINT_BTH, 1, NULL);

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
    std::string Compress = "compress";
    std::string memCompress = "memcompress";
    std::string fauxCompress = "fauxcompress";

    std::string Decompress = "decompress";
    std::string memDecompress = "memdecompress";
    std::string fauxDecompress = "fauxdecompress";

    if (argc < 2)
    {
        supportingDebugOnError();
        return 0;
    }

    //If Faux Compress
    if (fauxCompress.compare(argv[1]) == 0)
    {
        if (argc < 2)
        {
            supportingDebugOnError();
            return 0;
        }

        //This tests faux compress
        std::string MemLeakCheckTXTStr1 = argv[2];
        char MemLeakCheckTXT1[255];
        snprintf(MemLeakCheckTXT1, 255, "%s", MemLeakCheckTXTStr1.c_str());

        vpx_memory_tracker_set_log_type(0, MemLeakCheckTXT1);
        int x = 0;
        int n = 0;

        tprintf(PRINT_BTH, "\nAPI - Testing Faux Compressions:\n");

        while (x < 10000)
        {
            vpxt_faux_compress();

            if (n == 125)
            {
                tprintf(PRINT_BTH, ".");
                n = 0;
            }

            x++;
            n++;
        }

        vpx_memory_tracker_dump();
        return 0;
    }

    //If Faux Decompress
    if (fauxDecompress.compare(argv[1]) == 0)
    {
        if (argc < 3)
        {
            supportingDebugOnError();
            return 0;
        }

        //This tests faux decompress
        std::string MemLeakCheckTXTStr2 = argv[2];
        char *DecinputChar = argv[3];
        char MemLeakCheckTXT2[255];
        snprintf(MemLeakCheckTXT2, 255, "%s", MemLeakCheckTXTStr2.c_str());

        vpx_memory_tracker_set_log_type(0, MemLeakCheckTXT2);
        int x = 0;
        int n = 0;

        tprintf(PRINT_BTH, "\n\nAPI - Testing Faux Decompressions:\n");

        while (x < 10000)
        {
            vpxt_faux_decompress(DecinputChar);

            if (n == 125)
            {
                tprintf(PRINT_BTH, ".");
                n = 0;
            }

            x++;
            n++;
        }

        vpx_memory_tracker_dump();

        return 0;
    }

    //If Compress or Compress Mem Leak Check
    if (Compress.compare(argv[1]) == 0 || memCompress.compare(argv[1]) == 0)
    {
        if (argc < 7)
        {
            supportingDebugOnError();
            return 0;
        }

        tprintf(PRINT_BTH, "\nDebug Exe using: %s\n",
			vpx_codec_iface_name(&vpx_codec_vp8_cx_algo));

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

        //If Mem Leak Check
        if (memCompress.compare(argv[1]) == 0)
        {
            //This handles the Mem Leak Check Test.
            if (argc < 8)
            {
                supportingDebugOnError();
                return 0;
            }

            char *MemLeakCheckTXT = argv[7];
            //printf("\n Outputfile: %s\n",MemLeakCheckTXT);
            vpx_memory_tracker_set_log_type(0, MemLeakCheckTXT);
            vpxt_time_compress(inputFile, outputFile, opt.multi_threaded,
				opt.target_bandwidth, opt, "VP8 Debug", 0, 0, CPUTick, EncExt);
            vpx_memory_tracker_dump();
        }

        //If Compress
        if (Compress.compare(argv[1]) == 0)
            vpxt_time_compress(inputFile, outputFile, 0, opt.target_bandwidth,
			opt, "VP8 Release", 0, 0, CPUTick, EncExt);

        if (ExtraCommand == 1 || ExtraCommand == 3)
            supportingFileRunPSNR(inputFile, outputFile);

        return 0;
    }

    //If Decompress or Decomress Mem Leak Check
    if (Decompress.compare(argv[1]) == 0 || memDecompress.compare(argv[1]) == 0)
    {
        if (argc < 3)
        {
            supportingDebugOnError();
            return 0;
        }

        tprintf(PRINT_BTH, "\nDebug Exe using: %s\n",
			vpx_codec_iface_name(&vpx_codec_vp8_dx_algo));

        char *inputFile = argv[2];
        char *outputFile = argv[3];
        unsigned int CPUTick = 0;

        //check output type
        std::string DecExt;
        vpxt_get_file_extension(outputFile, DecExt);
        DecExt.erase(0, 1); //remove period

        if (memDecompress.compare(argv[1]) == 0)
        {
            //This handles the Mem Leak Check Test.
            if (argc < 4)
            {
                supportingDebugOnError();
                return 0;
            }

            char *MemLeakCheckTXT = argv[4];
            printf("\n Outputfile: %s\n", MemLeakCheckTXT);
            vpx_memory_tracker_set_log_type(0, MemLeakCheckTXT);
            vpxt_decompress_time_and_output(inputFile, outputFile, CPUTick,
				DecExt, 1);
            vpx_memory_tracker_dump();
        }

        if (Decompress.compare(argv[1]) == 0)
            vpxt_decompress_time_and_output(inputFile, outputFile, CPUTick,
			DecExt, 1);

        return 0;
    }

    supportingDebugOnError();
    return 0;
}