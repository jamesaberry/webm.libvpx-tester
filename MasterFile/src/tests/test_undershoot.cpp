#include "vpxt_test_declarations.h"

int test_undershoot(int argc,
                    const char** argv,
                    const std::string &working_dir,
                    const std::string sub_folder_str,
                    int test_type,
                    int delete_ivf,
                    int artifact_detection)
{
    char *comp_out_str = "Undershoot";
    char *test_dir = "test_undershoot";
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

    std::string under_shoot_10 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_10";
    vpxt_enc_format_append(under_shoot_10, enc_format);

    std::string under_shoot_100 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_100";
    vpxt_enc_format_append(under_shoot_100, enc_format);

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

    opt.target_bandwidth = bitrate;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        opt.under_shoot_pct = 10;

        if (vpxt_compress(input.c_str(), under_shoot_10.c_str(), speed, bitrate,
            opt, comp_out_str, 10, 0, enc_format, kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.under_shoot_pct = 100;

        if (vpxt_compress(input.c_str(), under_shoot_100.c_str(), speed, bitrate
            , opt, comp_out_str, 100, 0, enc_format, kSetConfigOff) == -1)
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
        // Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    tprintf(PRINT_BTH, "\n\n");
    long under_shoot_10_file_sz = vpxt_file_size(under_shoot_10.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    long under_shoot_100_file_sz = vpxt_file_size(under_shoot_100.c_str(), 1);
    tprintf(PRINT_BTH, "\n");

    int test_state = kTestFailed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (under_shoot_10_file_sz < under_shoot_100_file_sz)
    {
        vpxt_formated_print(RESPRT, "File size 1:%i < File size 2:%i - Passed",
            under_shoot_10_file_sz , under_shoot_100_file_sz);

        test_state = kTestPassed;
    }
    else if (under_shoot_10_file_sz == under_shoot_100_file_sz)
    {
        vpxt_formated_print(RESPRT, "No effect try different file - "
            "Indeterminate");
        test_state = kTestIndeterminate;
    }
    else
    {
        vpxt_formated_print(RESPRT, "File size 1:%i > File size 2:%i - Failed",
            under_shoot_10_file_sz , under_shoot_100_file_sz);
    }

    if(test_state == kTestPassed)
        tprintf(PRINT_BTH, "\n\nPassed\n");
    if(test_state == kTestFailed)
        tprintf(PRINT_BTH, "\n\nFailed\n");
    if(test_state == kTestIndeterminate)
        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

    if (delete_ivf)
            vpxt_delete_files(2, under_shoot_10.c_str(),
            under_shoot_100.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
