#include "vpxt_test_declarations.h"

int test_buffer_level(int argc,
                      const char *const *argv,
                      const std::string &working_dir,
                      std::string files_ar[],
                      int test_type,
                      int delete_ivf)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_buffer_level";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int speed = 0;
    int64_t starting_buffer_lvl;
    int64_t maximum_buffer_lvl;

    char char_calc_buff_sz[32];
    char char_calc_pre_buff[32];
    char char_bit_rate[32];

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return TEST_ERRFM;

    std::string buffer_lvl_works_enc = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(buffer_lvl_works_enc, enc_format);

    /////////////OutPutfile////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;

    if (test_type == COMP_ONLY || test_type == FULL_TEST)
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

    if (test_type == FULL_TEST)
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
            return TEST_INDT;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    opt.allow_df = 1;
    int compress_int = opt.allow_df;

    opt.target_bandwidth = bitrate;
    starting_buffer_lvl = opt.starting_buffer_level;
    maximum_buffer_lvl = opt.maximum_buffer_size;

    vpxt_itoa_custom(maximum_buffer_lvl, char_calc_buff_sz, 10);
    vpxt_itoa_custom(starting_buffer_lvl, char_calc_pre_buff, 10);
    vpxt_itoa_custom(bitrate, char_bit_rate, 10);

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        if (vpxt_compress(input.c_str(), buffer_lvl_works_enc.c_str(), speed,
            bitrate, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }
    }

    if (test_type == COMP_ONLY) //Create Compression only stop test short.
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
    }

    int check_pbm_result = vpxt_check_pbm(buffer_lvl_works_enc.c_str(), bitrate,
        maximum_buffer_lvl, starting_buffer_lvl);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (check_pbm_result == -11)
    {
        vpxt_formated_print(RESPRT, "No buffer under run detected - Passed");
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(1, buffer_lvl_works_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Buffer under run at frame: %i - Failed",
            check_pbm_result);
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(1, buffer_lvl_works_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}