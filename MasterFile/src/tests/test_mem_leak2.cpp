#include "vpxt_test_declarations.h"

int test_mem_leak2(int argc,
                   const char *const *argv,
                   const std::string &working_dir,
                   std::string files_ar[],
                   int test_type,
                   int delete_ivf)
{
    //Needs Debug.exe
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

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return 11;

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
    //Faux Compress
    mem_leak_check_enc_cmd_str = "\"\"" + exe_char_mem_leak_str + mem_leak_exe +
        "\" fauxcompress \"" + mem_leak_check_txt_1_str + "\"\"";

    //Faux Decompress
    mem_leak_check_dec_cmd_str = "\"\"" + exe_char_mem_leak_str + mem_leak_exe +
        "\" fauxdecompress \"" + mem_leak_check_txt_2_str + "\" \"" +
        dec_in_file + "\"\"";
#else
    //Faux Compress
    mem_leak_check_enc_cmd_str = "\'" + exe_char_mem_leak_str + mem_leak_exe +
        "\' fauxcompress \'" + mem_leak_check_txt_1_str + "\'";

    //Faux Decompress
    mem_leak_check_dec_cmd_str = "\'" + exe_char_mem_leak_str + mem_leak_exe +
        "\' fauxdecompress \'" + mem_leak_check_txt_2_str + "\' \'" +
        dec_in_file + "\'";
#endif

    /////////////OutPutfile////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;

    if (test_type == COMP_ONLY || test_type == TEST_AND_COMP)
        text_file_str += ".txt";
    else
        text_file_str += "_TestOnly.txt";

    FILE *fp;

    if ((fp = freopen(text_file_str.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            text_file_str.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (test_type == TEST_AND_COMP)
        print_header_full_test(argc, argv, main_test_dir_char);

    if (test_type == COMP_ONLY)
        print_header_compression_only(argc, argv, main_test_dir_char);

    if (test_type == TEST_ONLY)
        print_header_test_only(argc, argv, cur_test_dir_str);

    vpxt_cap_string_print(PRINT_BTH, "%s", test_dir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (input_ver == 2)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n",
                argv[argc-1]);

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////Make Sure Exe File Exists///////////////
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", argv[2]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 2;
    }

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(argv[3]))
    {
        tprintf(PRINT_BTH, "\nInput decode file %s does not exist\n", argv[3]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 2;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = bitrate ;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
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

        //opt.Mode = MODE_GOODQUALITY;
        //vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
        vpxt_run_exe(mem_leak_check_enc_cmd_str.c_str());
        vpxt_run_exe(mem_leak_check_dec_cmd_str.c_str());
    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 10;
    }

    int fail = 0;

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
        fail = 1;
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
            fail = 1;
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
        fail = 1;
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
            fail = 1;
        }
    }

    if (infile != NULL)fclose(infile);
    if (infile_2 != NULL)fclose(infile_2);

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 0;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return 0;

}