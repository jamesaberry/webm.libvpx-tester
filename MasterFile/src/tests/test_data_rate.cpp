#include "vpxt_test_declarations.h"

int test_data_rate(int argc,
                   const char *const *argv,
                   const std::string &working_dir,
                   const std::string sub_folder_str,
                   int test_type,
                   int delete_ivf)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_data_rate";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int speed = 0;

    ////////////Formatting Test Specific directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return TEST_ERRFM;

    std::string target_bitrate_enc = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(target_bitrate_enc, enc_format);

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

    int target_data_rate_percent = 30;
    opt.target_bandwidth = bitrate;

    ////////////////Data Rate Specific Settings////////////////
    opt.best_allowed_q = 0;
    opt.worst_allowed_q = 63;
    opt.allow_df = 1;
    ///////////////////////////////////////////////////////////

    int compress_int = opt.allow_df;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        if (vpxt_compress(input.c_str(), target_bitrate_enc.c_str(), speed,
            bitrate, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }
    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
    }

    tprintf(PRINT_BTH, "\n");

    double file_data_rate = vpxt_data_rate(target_bitrate_enc.c_str(), 1);
    double data_rate_prox = vpxt_abs_double(100 - vpxt_abs_double(
        ((file_data_rate * 100) / bitrate)));

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (data_rate_prox < target_data_rate_percent)
    {
        if (file_data_rate < bitrate)
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% lower than "
                "Target, DataRate is within %i%% of: %4.2f - Passed",
                file_data_rate, data_rate_prox, target_data_rate_percent,
                bitrate);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% greater "
                "than Target, DataRate is within %i%% of: %4.2f - Passed",
                file_data_rate, data_rate_prox, target_data_rate_percent, bitrate);
            tprintf(PRINT_BTH, "\n");
        }

        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(1, target_bitrate_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }
    else
    {
        if (file_data_rate < bitrate)
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% less than "
                "Target, DataRate not within %i%% of: %4.2f - Failed \n",
                file_data_rate, data_rate_prox, target_data_rate_percent, bitrate);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% greater "
                "than Target, DataRate not within %i%% of: %4.2f - Failed \n",
                file_data_rate, data_rate_prox, target_data_rate_percent, bitrate);
            tprintf(PRINT_BTH, "\n");
        }

        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(1, target_bitrate_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}