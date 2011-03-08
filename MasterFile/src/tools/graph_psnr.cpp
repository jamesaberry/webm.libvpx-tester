#include "vpxt_test_declarations.h"

int tool_graph_psnr(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType)
{
    if (argc < 6 || argc > 8)
    {
        tprintf(PRINT_STD, "\n"
                "  GraphPSNR\n\n"
                "    <Inputfile 1>\n"
                "    <Starting Bit Rate>\n"
                "    <Ending Bit Rate>\n"
                "    <Bit Rate Step>\n"
                "    <Encode Format - webm/ivf>\n"
                "    <Decode Format - y4m/ivf>\n"
                "     <Optional - Par Input>\n"

                "\n");
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "GraphPSNR";
    std::string MainDirString = "";
    char File1[255] = "";

    snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

    int v = 0;

    while (WorkingDir2[v] != '\"')
    {
        WorkingDir3[v] = WorkingDir2[v];
        v++;
    }

    WorkingDir3[v] = slashChar();
    WorkingDir3[v+1] = '\0';
    std::string WorkingDirString = WorkingDir3;
    WorkingDirString.append(MyDir);
    WorkingDirString.append(slashCharStr());
    WorkingDirString.append(FilesAr[0]);
    WorkingDirString.erase(WorkingDirString.length() - 1, 1);

    std::string CreateDir2 = WorkingDirString;
    CreateDir2.insert(0, "md \"");
    vpxt_make_dir_vpx(CreateDir2.c_str());

    /////////////OutPutfile////////////
    std::string TextfileString = WorkingDirString;

    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);
    std::string OutPutStr = TextfileString;
    TextfileString.append(".txt");

    OutPutStr.append("_");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        print_header_full_test(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    tprintf(PRINT_BTH, "Graph PSNR\n");

    std::string input = argv[2];
    int FirstBitRate = atoi(argv[3]);
    int LastBitRate = atoi(argv[4]);
    int BitRateStep = atoi(argv[5]);
    std::string EncForm = argv[6];
    std::string DecForm = argv[7];
    std::string ParFile = argv[8];

    int speed = 0;

    char  FileNameChar[255];
    vpxt_file_name(input.c_str(), FileNameChar, 0);
    std::string InputName = FileNameChar;
    InputName.resize(InputName.length() - 4, ' ');
    OutPutStr.append(InputName.c_str());
    std::string ParFileOut = OutPutStr;
    OutPutStr.append("_TBR");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    double PSNRArr[100];
    double SSIMArr[100];
    double DataRateArr[100];
    unsigned int EncTimeArr[100];
    unsigned int DecTimeArr[100];


    char *CompressString = "Graph PSNR";

    if (argc > 6)
    {
        opt = vpxt_input_settings(ParFile.c_str());
    }

    opt.target_bandwidth = FirstBitRate;

    ///////////output Par file////////////////////
    ParFileOut.append("_ParameterFile.txt");
    char ParFileOutChar[255];
    snprintf(ParFileOutChar, 255, "%s", ParFileOut.c_str());
    vpxt_output_settings(ParFileOutChar, opt);
    ///////////////////////////////////////////////

    int x = 0;
    int DoONce = 0;

    while (opt.target_bandwidth <= LastBitRate)
    {
        if (opt.target_bandwidth == LastBitRate && DoONce == 0)
        {
            DoONce = 1;
        }

        std::string OutPutStr2 = OutPutStr;
        char TBChar[8];
        vpxt_itoa_custom(opt.target_bandwidth, TBChar, 10);
        OutPutStr2.append(TBChar);
        std::string OutPutStr3 = OutPutStr2;
        OutPutStr3.append("_Dec");
        vpxt_enc_format_append(OutPutStr2, EncForm);
        vpxt_dec_format_append(OutPutStr3, DecForm);

        char outputChar[255];
        snprintf(outputChar, 255, "%s", OutPutStr2.c_str());

        char outputChar2[255];
        snprintf(outputChar2, 255, "%s", OutPutStr3.c_str());

        unsigned int cpu_tick1 = 0;
        EncTimeArr[x] = vpxt_time_compress(input.c_str(), outputChar , speed, opt.target_bandwidth, opt, CompressString, 0, 0, cpu_tick1, EncForm);

        if (EncTimeArr[x] == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, TestType);
            return 2;
        }

        unsigned int cpu_tick2 = 0;
        DecTimeArr[x] = vpxt_time_decompress(outputChar, outputChar2, cpu_tick2, DecForm);

        if (DecTimeArr[x] == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, TestType);
            return 2;
        }

        double ssimnumber = 0;
        PSNRArr[x] = vpxt_psnr(input.c_str(), outputChar, 0, 2, 1, &ssimnumber);
        SSIMArr[x] = ssimnumber;
        DataRateArr[x] = vpxt_data_rate(outputChar, 1);
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

    tprintf(PRINT_BTH, "\n\n");

    while (x < ArrSize)
    {
        tprintf(PRINT_BTH, " DataRate: %.2f PSNR: %.2f SSIM: %.2f EncodeTime: %i \n DecodeTime: %i\n", DataRateArr[x], PSNRArr[x], SSIMArr[x], EncTimeArr[x], DecTimeArr[x]);
        x++;
    }

    x = 0;

    tprintf(PRINT_BTH, "\n\nDataRate\n\n");

    while (x < ArrSize)
    {
        tprintf(PRINT_BTH, "%.2f\n", DataRateArr[x]);
        x++;
    }

    x = 0;

    tprintf(PRINT_BTH, "\nPSNR\n\n");

    while (x < ArrSize)
    {
        tprintf(PRINT_BTH, "%.2f\n", PSNRArr[x]);
        x++;
    }

    x = 0;

    tprintf(PRINT_BTH, "\nSSIM\n\n");

    while (x < ArrSize)
    {
        tprintf(PRINT_BTH, "%.2f\n", SSIMArr[x]);
        x++;
    }

    x = 0;

    tprintf(PRINT_BTH, "\nEncodeTime\n\n");

    while (x < ArrSize)
    {
        tprintf(PRINT_BTH, "%i\n", EncTimeArr[x]);
        x++;
    }

    x = 0;

    tprintf(PRINT_BTH, "\nDecodeTime\n\n");

    while (x < ArrSize)
    {
        tprintf(PRINT_BTH, "%i\n", DecTimeArr[x]);
        x++;
    }

    fclose(fp);
    return 4;

}