#include "vpxt_test_declarations.h"

int test_mem_leak2(int argc,
                   const char** argv,
                   const std::string &working_dir,
                   const std::string sub_folder_str,
                   int test_type,
                   int delete_ivf,
                   int artifact_detection)
{
    // Needs Debug.exe
    char *test_dir = "test_mem_leak2";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    char mem_leak_exe[255];
    char dec_in_file[255];
    snprintf(mem_leak_exe, 255, "%s", argv[2]);
    snprintf(dec_in_file, 255, "%s", argv[3]);

    std::string input = "Blank";
    int mode = 1;
    int bitrate = 128;

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string exe_char_mem_leak_str;
    vpxt_folder_name(argv[0], &exe_char_mem_leak_str);

    std::string mem_leak_check_txt_base = cur_test_dir_str + slashCharStr() +
        test_dir;

    std::string mem_leak_check_txt_1_str = mem_leak_check_txt_base +
        "_compression_memory_summary.txt";

    std::string mem_leak_check_txt_2_str = mem_leak_check_txt_base +
        "_decompression_memory_summary.txt";

    std::string mem_leak_check_enc_cmd_str;
    std::string mem_leak_check_dec_cmd_str;

#if defined(_WIN32)
    // Faux Compress
    mem_leak_check_enc_cmd_str = "\"\"" + exe_char_mem_leak_str + mem_leak_exe +
        "\" fauxcompress \"" + mem_leak_check_txt_1_str + "\"\"";

    // Faux Decompress
    mem_leak_check_dec_cmd_str = "\"\"" + exe_char_mem_leak_str + mem_leak_exe +
        "\" fauxdecompress \"" + mem_leak_check_txt_2_str + "\" \"" +
        dec_in_file + "\"\"";
#else
    // Faux Compress
    mem_leak_check_enc_cmd_str = "\'" + exe_char_mem_leak_str + mem_leak_exe +
        "\' fauxcompress \'" + mem_leak_check_txt_1_str + "\'";

    // Faux Decompress
    mem_leak_check_dec_cmd_str = "\'" + exe_char_mem_leak_str + mem_leak_exe +
        "\' fauxdecompress \'" + mem_leak_check_txt_2_str + "\' \'" +
        dec_in_file + "\'";
#endif

    ///////////// Open Output File and Print Header ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;
    FILE *fp;

    vpxt_open_output_file(test_type, text_file_str, fp);
    vpxt_print_header(argc, argv, main_test_dir_char, cur_test_dir_str,
        test_dir, test_type);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    /////////////////// Use Custom Settings ///////////////////
    if(vpxt_use_custom_settings(argv, argc, input_ver, fp, file_index_str,
        file_index_output_char, test_type, opt, bitrate)
        == kTestIndeterminate)
        return kTestIndeterminate;

    ///////////////// Make Sure Exe File Exists ///////////////
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", argv[2]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
    }

    ///////////////// Make Sure ivf File Exists ///////////////
    if (!vpxt_file_exists_check(argv[3]))
    {
        tprintf(PRINT_BTH, "\nInput decode file %s does not exist\n", argv[3]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = bitrate ;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        fclose(fp);

        if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file4.\n");
            exit(1);
        }

        fprintf(stderr, " ");

        // opt.Mode = MODE_GOODQUALITY;
        // vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
        vpxt_run_exe(mem_leak_check_enc_cmd_str.c_str());
        vpxt_run_exe(mem_leak_check_dec_cmd_str.c_str());
    }

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        // Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    FILE *infile = fopen(mem_leak_check_txt_1_str.c_str(), "rb");

    if (infile == NULL)
    {
        char MemLeakCheckTXTFileName[200];
        vpxt_file_name(mem_leak_check_txt_1_str.c_str(),
            MemLeakCheckTXTFileName, 0);
        vpxt_formated_print(RESPRT, "File not found: %s - Failed",
            MemLeakCheckTXTFileName);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        char buffer[256];

        fgets(buffer , 256 , infile);
        fgets(buffer , 256 , infile);
        fgets(buffer , 256 , infile);
        fgets(buffer , 256 , infile);

        std::string buffer_str = buffer;

        if (buffer_str.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            vpxt_formated_print(RESPRT, "Encode Memory Currently Allocated == "
                "0 - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "Encode Memory Currently Allocated != "
                "0 - %s - Failed", buffer_str.c_str());
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    FILE *infile_2 = fopen(mem_leak_check_txt_2_str.c_str(), "rb");

    if (infile_2 == NULL)
    {
        char MemLeakCheckTXTFileName[200];
        vpxt_file_name(mem_leak_check_txt_2_str.c_str(),
            MemLeakCheckTXTFileName, 0);
        vpxt_formated_print(RESPRT, "File not found: %s - Failed",
            MemLeakCheckTXTFileName);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        char buffer_2[256];

        fgets(buffer_2 , 256 , infile_2);
        fgets(buffer_2 , 256 , infile_2);
        fgets(buffer_2 , 256 , infile_2);
        fgets(buffer_2 , 256 , infile_2);

        std::string buffer_str_2 = buffer_2;

        if (buffer_str_2.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            vpxt_formated_print(RESPRT, "Decode Memory Currently Allocated == "
                "0 - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "Decode Memory Currently Allocated != "
                "0 - %s - Failed", buffer_str_2.c_str());
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    if (infile != NULL)fclose(infile);
    if (infile_2 != NULL)fclose(infile_2);

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;

}