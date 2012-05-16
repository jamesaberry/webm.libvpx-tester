#include "vpxt_test_declarations.h"

int test_buffer_level(int argc,
                      const char** argv,
                      const std::string &working_dir,
                      const std::string sub_folder_str,
                      int test_type,
                      int delete_ivf,
                      int artifact_detection)
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

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string buffer_lvl_works_enc = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(buffer_lvl_works_enc, enc_format);

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
    opt.allow_df = 1;
    int compress_int = opt.allow_df;

    opt.target_bandwidth = bitrate;
    starting_buffer_lvl = opt.starting_buffer_level;
    maximum_buffer_lvl = opt.maximum_buffer_size;

    vpxt_itoa_custom(maximum_buffer_lvl, char_calc_buff_sz, 10);
    vpxt_itoa_custom(starting_buffer_lvl, char_calc_pre_buff, 10);
    vpxt_itoa_custom(bitrate, char_bit_rate, 10);

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        if (vpxt_compress(input.c_str(), buffer_lvl_works_enc.c_str(), speed,
            bitrate, opt, comp_out_str, compress_int, 0, enc_format,
            kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }
    }

    if (test_type == kCompOnly) // Create Compression only stop test short.
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    int check_pbm_result = vpxt_check_pbm(buffer_lvl_works_enc.c_str(), bitrate,
        maximum_buffer_lvl, starting_buffer_lvl);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (check_pbm_result == -11)
    {
        vpxt_formated_print(RESPRT, "No buffer under run detected - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Buffer under run at frame: %i - Failed",
            check_pbm_result);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if(test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if(test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(1, buffer_lvl_works_enc.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}