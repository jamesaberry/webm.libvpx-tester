#include "vpxt_test_declarations.h"

int tool_graph_psnr(int argc,
                    const char** argv,
                    const std::string &working_dir,
                    const std::string sub_folder_str,
                    int test_type)
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

    //////////// Formatting Test Specific directory ////////////
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *test_dir = "GraphPSNR";
    std::string MainDirString;
    char File1[255] = "";

    snprintf(WorkingDir2, 255, "%s", working_dir.c_str());

    int v = 0;

    while (WorkingDir2[v] != '\"')
    {
        WorkingDir3[v] = WorkingDir2[v];
        v++;
    }

    WorkingDir3[v] = slashChar();
    WorkingDir3[v+1] = '\0';
    std::string WorkingDirString = WorkingDir3;
    WorkingDirString += test_dir;
    WorkingDirString += slashCharStr() + sub_folder_str;
    WorkingDirString.erase(WorkingDirString.length() - 1, 1);

    std::string create_dir_2 = WorkingDirString;
    create_dir_2.insert(0, "md \"");
    vpxt_make_dir_vpx(create_dir_2.c_str());

    ///////////// OutPutfile ////////////
    std::string text_file_str = WorkingDirString + slashCharStr() + test_dir;

    std::string OutPutStr = text_file_str;
    text_file_str += ".txt";
    OutPutStr += "_";

    FILE *fp;

    if ((fp = freopen(text_file_str.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            text_file_str.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (test_type == 1)
    {
        print_header_full_test(argc, argv, WorkingDir3);
    }

    if (test_type == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (test_type == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    tprintf(PRINT_BTH, "Graph PSNR\n");

    std::string input = argv[2];
    int FirstBitRate = atoi(argv[3]);
    int LastBitRate = atoi(argv[4]);
    int BitRateStep = atoi(argv[5]);
    std::string enc_format = argv[6];
    std::string dec_format = argv[7];
    std::string ParFile = argv[8];

    int speed = 0;

    char  file_name_char[255];
    vpxt_file_name(input.c_str(), file_name_char, 0);
    std::string InputName = file_name_char;
    InputName.resize(InputName.length() - 4, ' ');
    OutPutStr += InputName;
    std::string ParFileOut = OutPutStr;
    OutPutStr += "_TBR";

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    double psnr_arr[100];
    double SSIMArr[100];
    double DataRateArr[100];
    unsigned int EncTimeArr[100];
    unsigned int DecTimeArr[100];


    char *comp_out_str = "Graph PSNR";

    if (argc > 6)
    {
        opt = vpxt_input_settings(ParFile.c_str());
    }

    opt.target_bandwidth = FirstBitRate;

    /////////// Output Par File ////////////////////
    ParFileOut += "_ParameterFile.txt";
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
        OutPutStr2 += TBChar;
        std::string OutPutStr3 = OutPutStr2;
        OutPutStr3 += "_Dec";
        vpxt_enc_format_append(OutPutStr2, enc_format);
        vpxt_dec_format_append(OutPutStr3, dec_format);

        char outputChar[255];
        snprintf(outputChar, 255, "%s", OutPutStr2.c_str());

        char outputChar2[255];
        snprintf(outputChar2, 255, "%s", OutPutStr3.c_str());

        unsigned int cpu_tick_1 = 0;
        EncTimeArr[x] = vpxt_time_compress(input.c_str(), outputChar, speed,
            opt.target_bandwidth, opt, comp_out_str, 0, 0, cpu_tick_1,
            enc_format);

        if (EncTimeArr[x] == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, test_type);
            return 2;
        }

        unsigned int cpu_tick_2 = 0;
        DecTimeArr[x] = vpxt_time_decompress(outputChar, outputChar2, cpu_tick_2
            , dec_format, 1);

        if (DecTimeArr[x] == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, test_type);
            return 2;
        }

        double ssimnumber = 0;
        psnr_arr[x] = vpxt_psnr(input.c_str(), outputChar, 0, PRINT_BTH, 1,
            &ssimnumber);
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
        tprintf(PRINT_BTH, " DataRate: %.2f PSNR: %.2f SSIM: %.2f EncodeTime: "
            "%i \n DecodeTime: %i\n", DataRateArr[x], psnr_arr[x], SSIMArr[x],
            EncTimeArr[x], DecTimeArr[x]);
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
        tprintf(PRINT_BTH, "%.2f\n", psnr_arr[x]);
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