#include "vpxt_test_declarations.h"

int test_copy_set_reference(int argc,
                            const char *const *argv,
                            const std::string &working_dir,
                            std::string files_ar[],
                            int test_type,
                            int delete_ivf)
{
    char *comp_out_str = "Error Resilient";
    char *test_dir = "test_copy_set_reference";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int first_copy_frame = atoi(argv[5]);
    std::string enc_format = argv[6];
    std::string dec_format = argv[7];

    int speed = 0;
    int pass = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return 11;

    std::string copy_set_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression";
    vpxt_enc_format_append(copy_set_enc, enc_format);

    std::string copy_set_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_decompression";
    vpxt_dec_format_append(copy_set_dec, dec_format);

    std::string copy_set_dec_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_decompression_clone";
    vpxt_dec_format_append(copy_set_dec_2, dec_format);

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
        opt.error_resilient_mode = 1;

        if (vpxt_compress(input.c_str(), copy_set_enc.c_str(), speed, bitrate,
            opt, comp_out_str, opt.error_resilient_mode, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return 2;
        }

        pass = vpxt_decompress_copy_set(copy_set_enc.c_str(),
            copy_set_dec.c_str(), copy_set_dec_2.c_str(), dec_format, 1,
            first_copy_frame, PRINT_BTH);
    }

    if (test_type == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 10;
    }

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (pass == 0)
    {
        vpxt_formated_print(RESPRT, "Not all cloned decompression frames "
            "identical to non cloned decompression frames - Failed");
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 1)
    {
        vpxt_formated_print(RESPRT, "All cloned decompression frames identical "
            "to non cloned decompression frames - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 2)
    {
        vpxt_formated_print(RESPRT, "First clone frame not reached - "
            "Indeterminate");
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 1)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(3, copy_set_enc.c_str(), copy_set_dec.c_str(),
            copy_set_dec_2.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 1;
    }

    if (pass == 2)
    {
        tprintf(PRINT_BTH, "\nIndeterminate\n");

        if (delete_ivf)
            vpxt_delete_files(3, copy_set_enc.c_str(), copy_set_dec.c_str(),
            copy_set_dec_2.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 2;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(3, copy_set_enc.c_str(), copy_set_dec.c_str(),
            copy_set_dec_2.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 0;
    }

    fclose(fp);

    record_test_complete(file_index_str, file_index_output_char, test_type);
    return 6;
}
