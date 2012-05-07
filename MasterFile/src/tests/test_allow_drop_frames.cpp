#include "vpxt_test_declarations.h"

int test_allow_drop_frames(int argc,
                           const char** argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf,
                           int artifact_detection)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_allow_drop_frames";
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

    std::string allow_df_on_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_1";
    vpxt_enc_format_append(allow_df_on_enc, enc_format);

    std::string allow_df_off_enc = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_0";
    vpxt_enc_format_append(allow_df_off_enc, enc_format);

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
        opt.allow_df = 0;

        if (vpxt_compress(input.c_str(), allow_df_off_enc.c_str(), speed,
            bitrate, opt, comp_out_str, 0, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.allow_df = 1;

        if (vpxt_compress(input.c_str(), allow_df_on_enc.c_str(), speed, bitrate
            , opt, comp_out_str, 1, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }
    }

    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    int allow_df_on_frames = vpxt_display_visible_frames(allow_df_on_enc.c_str()
        , 1);
    int allow_df_off_frames = vpxt_display_visible_frames(
        allow_df_off_enc.c_str(), 1);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (allow_df_on_frames < allow_df_off_frames)
    {
        vpxt_formated_print(RESPRT, "DF on frames = %i < DF off frames = %i - "
            "Passed", allow_df_on_frames, allow_df_off_frames);
        tprintf(PRINT_BTH, "\n");
    }

    if (allow_df_off_frames == allow_df_on_frames)
    {
        vpxt_formated_print(RESPRT, "DF on frames = %i == DF off frames = %i "
            "No effect - Failed", allow_df_on_frames, allow_df_off_frames);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (allow_df_on_frames > allow_df_off_frames)
    {
        vpxt_formated_print(RESPRT, "DF on frames = %i > DF off frames = %i - "
            "Failed", allow_df_on_frames, allow_df_off_frames);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(2, allow_df_on_enc.c_str(), allow_df_off_enc.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
