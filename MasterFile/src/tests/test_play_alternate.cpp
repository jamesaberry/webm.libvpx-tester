#include "vpxt_test_declarations.h"

int test_play_alternate(int argc,
                        const char** argv,
                        const std::string &working_dir,
                        const std::string sub_folder_str,
                        int test_type,
                        int delete_ivf,
                        int artifact_detection)
{
    char *comp_out_str = "Play Alternate";
    char *test_dir = "test_play_alternate";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int speed = 0;
    int play_alternate_1_val = 0;
    int play_alternate_2_val = 1;

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    char play_alt_buff[255];

    vpxt_itoa_custom(play_alternate_1_val, play_alt_buff, 10);
    std::string play_alternate_1 = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_" + play_alt_buff;
    vpxt_enc_format_append(play_alternate_1, enc_format);

    vpxt_itoa_custom(play_alternate_2_val, play_alt_buff, 10);
    std::string play_alternate_2 = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_" + play_alt_buff;
    vpxt_enc_format_append(play_alternate_2, enc_format);

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
    opt.allow_lag = 1;
    opt.lag_in_frames = 10;
    opt.end_usage = 1;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;
        opt.play_alternate = play_alternate_1_val;

        if (vpxt_compress(input.c_str(), play_alternate_1.c_str(), speed,
            bitrate, opt, comp_out_str, play_alternate_1_val, 0, enc_format)
            == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.play_alternate = play_alternate_2_val;

        if (vpxt_compress(input.c_str(), play_alternate_2.c_str(), speed,
            bitrate, opt, comp_out_str, play_alternate_2_val, 0, enc_format)
            == -1)
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

    int lng_rc = vpxt_compare_enc(play_alternate_2.c_str(),
        play_alternate_1.c_str(),
        0);

    int test_state = kTestPassed;

    int play_alternate_on_alt_ref_count =
        vpxt_display_alt_ref_frames(play_alternate_2.c_str(), 1);
    int play_alternate_off_alt_ref_count =
        vpxt_display_alt_ref_frames(play_alternate_1.c_str(), 1);
    int visible_frame_on_count =
        vpxt_display_visible_frames(play_alternate_2.c_str(), 1);
    int visible_frame_off_count =
        vpxt_display_visible_frames(play_alternate_1.c_str(), 1);

    char play_alternate_on_file_name[255];
    vpxt_file_name(play_alternate_2.c_str(), play_alternate_on_file_name, 0);
    char play_alternate_off_file_name[255];
    vpxt_file_name(play_alternate_1.c_str(), play_alternate_off_file_name, 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (play_alternate_on_alt_ref_count > 0)
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames exist for %s - "
            "Passed", play_alternate_on_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames do not exist "
            "for %s - Failed", play_alternate_on_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (play_alternate_off_alt_ref_count > 0)
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames exist for %s - "
            "Failed", play_alternate_off_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames do not exist "
            "for %s - Passed", play_alternate_off_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if (visible_frame_on_count == visible_frame_off_count)
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i == Visible "
            "Frames for %s = %i - Passed", play_alternate_on_file_name,
            visible_frame_on_count, play_alternate_off_file_name,
            visible_frame_off_count);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i != Visible "
            "Frames for %s = %i - Failed", play_alternate_on_file_name,
            visible_frame_on_count, play_alternate_off_file_name,
            visible_frame_off_count);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (lng_rc >= 0)
    {
        vpxt_formated_print(RESPRT, "Files are not identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (lng_rc == -1)
    {
        vpxt_formated_print(RESPRT, "Files are identical - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(2, play_alternate_1.c_str(),
        play_alternate_2.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}