#include "vpxt_test_declarations.h"

int tool_rand_comp(int argc,
                   const char** argv,
                   const std::string &working_dir,
                   const std::string sub_folder_str,
                   int test_type)
{
    char *comp_out_str = "RandomInput";

    if (argc != 3)
    {
        tprintf(PRINT_STD, "\n"
                "  RandComp\n\n"
                "    <Input File>\n"
                "    <webm/ivf>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    std::string enc_format = argv[3];

    //////////// Formatting Test Specific directory ////////////
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *test_dir = "RandComp";
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

    std::string WorkingDir4 = WorkingDirString + slashCharStr();
    std::string WorkingDir5 = WorkingDirString;

    char input_file_name[255];
    vpxt_file_name(input.c_str(), input_file_name, 0);
    std::string InputFileNameStr = input_file_name;

    if (InputFileNameStr.substr(InputFileNameStr.length() - 4, 4).compare(".ivf"
        ) == 0) // if file extension is present remove it
        WorkingDir4 += InputFileNameStr.substr(0,
        InputFileNameStr.length() - 4);
    else
        WorkingDir4 += input_file_name;

    WorkingDir4 += "-RandComp";
    vpxt_enc_format_append(WorkingDir4, enc_format);
    WorkingDir5 += slashCharStr();
    WorkingDir5 += "RandCompOutPut.txt";

    char RandComp[255];
    char RandCompParOut[255];

    snprintf(RandComp, 255, "%s", WorkingDir4.c_str());
    snprintf(RandCompParOut, 255, "%s", WorkingDir5.c_str());

    ///////////// OutPutfile ////////////
    std::string text_file_str = WorkingDirString = slashCharStr() + test_dir +
        ".txt";

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



    VP8_CONFIG opt = vpxt_random_parameters(opt, input.c_str(), 1);
    int Mode = opt.Mode;

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;
        // vpxt_output_settings(RandCompParOut,opt);
        int speed = opt.multi_threaded;
        int bitrate = opt.target_bandwidth;

        if (vpxt_compress(input.c_str(), RandComp, speed, bitrate, opt,
            comp_out_str, 1, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, test_type);
            return 2;
        }
    }

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;
        int speed = opt.multi_threaded;
        int bitrate = opt.target_bandwidth;

        if (vpxt_compress(input.c_str(), RandComp, speed, bitrate, opt,
            comp_out_str, 1, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, test_type);
            return 2;
        }
    }

    if (Mode == 2)
    {
        opt.Mode = MODE_BESTQUALITY;
        int speed = opt.multi_threaded;
        int bitrate = opt.target_bandwidth;

        if (vpxt_compress(input.c_str(), RandComp, speed, bitrate, opt,
            comp_out_str, 1, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, test_type);
            return 2;
        }
    }

    if (Mode == 3)
    {
    }

    if (Mode == 4)
    {
        int speed = opt.multi_threaded;
        int bitrate = opt.target_bandwidth;
        opt.Mode = MODE_SECONDPASS_BEST;

        if (vpxt_compress(input.c_str(), RandComp, speed, bitrate, opt,
            comp_out_str, 1, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, test_type);
            return 2;
        }
    }

    return 5;
}