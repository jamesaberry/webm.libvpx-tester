#include "vpxt_test_declarations.h"

int test_data_rate(int argc,
                   const char** argv,
                   const std::string &working_dir,
                   const std::string sub_folder_str,
                   int test_type,
                   int delete_ivf,
                   int artifact_detection)
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

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string target_bitrate_enc = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(target_bitrate_enc, enc_format);

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

    int target_data_rate_percent = 30;
    opt.target_bandwidth = bitrate;

    //////////////// Data Rate Specific Settings ////////////////
    opt.best_allowed_q = 0;
    opt.worst_allowed_q = 63;
    opt.allow_df = 1;
    ///////////////////////////////////////////////////////////

    int compress_int = opt.allow_df;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
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
            return kTestIndeterminate;
        }
    }

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    tprintf(PRINT_BTH, "\n");

    double file_data_rate = vpxt_data_rate(target_bitrate_enc.c_str(), 1);
    double data_rate_prox = vpxt_abs_double(100 - vpxt_abs_double(
        ((file_data_rate * 100) / bitrate)));

    int test_state = kTestFailed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (data_rate_prox < target_data_rate_percent)
    {
        if (file_data_rate < bitrate)
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% lower than "
                "Target, DataRate is within %i%% of: %i - Passed",
                file_data_rate, data_rate_prox, target_data_rate_percent,
                bitrate);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% greater "
                "than Target, DataRate is within %i%% of: %i - Passed",
                file_data_rate, data_rate_prox, target_data_rate_percent,
                bitrate);
            tprintf(PRINT_BTH, "\n");
        }

        test_state = kTestPassed;
    }
    else
    {
        if (file_data_rate < bitrate)
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% less than "
                "Target, DataRate not within %i%% of: %i - Failed \n",
                file_data_rate, data_rate_prox, target_data_rate_percent,
                bitrate);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% greater "
                "than Target, DataRate not within %i%% of: %i - Failed \n",
                file_data_rate, data_rate_prox, target_data_rate_percent,
                bitrate);
            tprintf(PRINT_BTH, "\n");
        }
    }

    if(test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if(test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(1, target_bitrate_enc.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}