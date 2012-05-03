#include "vpxt_test_declarations.h"

int test_debug_matches_release(int argc,
                               const char *const *argv,
                               const std::string &working_dir,
                               std::string files_ar[],
                               int test_type,
                               int delete_ivf)
{
    //Needs Debug.exe and Release.exe
    char *test_dir = "test_debug_matches_release";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string exe_input_debug = argv[5];
    std::string exe_input_release = argv[6];
    std::string enc_format = argv[7];
    std::string dec_format = argv[8];

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return 11;

    std::string exe_debug_release_str;
    vpxt_folder_name(argv[0], &exe_debug_release_str);

    std::string debug_exe_loc = exe_debug_release_str + exe_input_debug;
    std::string release_exe_loc = exe_debug_release_str + exe_input_release;

    std::string debug_output_enc = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_debug";
    vpxt_enc_format_append(debug_output_enc, enc_format);

    std::string release_output_enc = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_release";
    vpxt_enc_format_append(release_output_enc, enc_format);

    std::string debug_output_dec = cur_test_dir_str + slashCharStr() + test_dir
        + "_decompression_debug";
    vpxt_dec_format_append(debug_output_dec, dec_format);

    std::string release_output_dec = cur_test_dir_str + slashCharStr() +
        test_dir + "_decompression_release";
    vpxt_dec_format_append(release_output_dec, dec_format);

    std::string par_file_debug = cur_test_dir_str + slashCharStr() + test_dir +
        "_parameter_file_debug.txt";

    std::string par_file_release = cur_test_dir_str + slashCharStr() + test_dir
        + "_parameter_file_release.txt";

    std::string debug_enc_cmd = exe_debug_release_str;
    std::string release_enc_cmd = exe_debug_release_str;
    std::string debug_dec_cmd = exe_debug_release_str;
    std::string release_dec_cmd = exe_debug_release_str;

#if defined(_WIN32)
    {
        debug_enc_cmd.insert(0, "\"\"");
        debug_enc_cmd += exe_input_release + "\" compress \"" + input
            + "\" \"" + debug_output_enc + "\" 8 \"" + par_file_debug +
            "\" 0\"";

        debug_dec_cmd.insert(0, "\"\"");
        debug_dec_cmd += exe_input_release + "\" decompress \"" +
            debug_output_enc + "\" \"" + debug_output_dec + "\"\" ";

        release_enc_cmd.insert(0, "\"\"");
        release_enc_cmd + exe_input_debug + "\" compress \"" + input +
            "\" \"" + release_output_enc + "\" 8 \"" + par_file_release +
            "\" 0\"";

        release_dec_cmd.insert(0, "\"\"");
        release_dec_cmd += exe_input_debug + "\" decompress \"" +
            release_output_enc + "\" \"" + release_output_dec + "\"\" ";
    }
#else
    {
        debug_enc_cmd.insert(0, "\'");
        debug_enc_cmd += exe_input_release + "\' compress \'" + input + "\' \'"
            + debug_output_enc + "\' 8 \'" + par_file_debug + "\' 0";

        debug_dec_cmd.insert(0, "\'");
        debug_dec_cmd += exe_input_release + "\' decompress \'" +
            debug_output_enc + "\' \'" + debug_output_dec + "\' ";

        release_enc_cmd.insert(0, "\'");
        release_enc_cmd += exe_input_debug + "\' compress \'" + input + "\' \'"
            + release_output_enc + "\' 8 \'" + par_file_release + "\' 0";

        release_dec_cmd.insert(0, "\'");
        release_dec_cmd += exe_input_debug + "\' decompress \'" +
            release_output_enc + "\' \'" + release_output_dec + "\'";
    }
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
        tprintf(PRINT_STD, "Cannot open out put file2.\n");
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

    vpxt_cap_string_print(PRINT_BTH, "%s\n", test_dir);

    fclose(fp);

    if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file3.\n");
        exit(1);
    }

    fprintf(stderr, " ");

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
    if (!vpxt_file_exists_check(argv[6]))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", argv[6]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 2;
    }

    /////////////////Make Sure Exe File Exists///////////////
    if (!vpxt_file_exists_check(argv[5]))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", argv[5]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 2;
    }

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf(PRINT_BTH, "\nInput encode file %s does not exist\n", argv[2]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 2;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = bitrate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;
        vpxt_determinate_parameters(opt);

        vpxt_output_settings(par_file_debug.c_str(), opt);
        vpxt_run_exe(debug_enc_cmd);
        vpxt_run_exe(debug_dec_cmd);
        vpxt_output_settings(par_file_release.c_str(), opt);

        fclose(fp);

        if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put FileIndexOutputChar1.\n");
            exit(1);
        }

        fprintf(stderr, " ");
        vpxt_run_exe(release_enc_cmd);
        vpxt_run_exe(release_dec_cmd);
    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 10;
    }

    tprintf(PRINT_BTH, "\n\nComparing Compression Files: ");
    int lng_rc_enc = vpxt_compare_enc(release_output_enc.c_str(),
        debug_output_enc.c_str(), 0);
    int enc_match = vpxt_print_compare_ivf_results(lng_rc_enc, 0);

    tprintf(PRINT_BTH, "Comparing Decompression Files: ");
    int lng_rc_dec = vpxt_compare_dec(release_output_dec.c_str(),
        debug_output_dec.c_str());
    int dec_match = vpxt_print_compare_ivf_results(lng_rc_dec, 0);

    int fail = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (enc_match == 1)
    {
        vpxt_formated_print(RESPRT, "Debug Compression identical to Release "
            "Compression - Passed\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Debug Compression not identical to "
            "Release Compression - Failed");
        fail = 1;
    }

    if (dec_match == 1)
    {
        vpxt_formated_print(RESPRT, "Debug Decompression identical to Release "
            "Decompression - Passed\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Debug Decmpression not identical to "
            "Release Decompression - Failed");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(4, debug_output_enc.c_str(),
            release_output_enc.c_str(), debug_output_dec.c_str(),
            release_output_dec.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(4, debug_output_enc.c_str(),
            release_output_enc.c_str(), debug_output_dec.c_str(),
            release_output_dec.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 0;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return 6;
}
