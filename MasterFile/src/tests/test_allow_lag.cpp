#include "vpxt_test_declarations.h"

int test_allow_lag(int argc,
                   const char** argv,
                   const std::string &working_dir,
                   const std::string sub_folder_str,
                   int test_type,
                   int delete_ivf)
{
    char *comp_out_str = "Allow Lag";
    char *test_dir = "test_allow_lag";
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

    std::string allow_lag_on_enc = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_1";
    vpxt_enc_format_append(allow_lag_on_enc, enc_format);

    std::string allow_lag_off_enc = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_0";
    vpxt_enc_format_append(allow_lag_off_enc, enc_format);

    ///////////// OutPutfile ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;

    if (test_type == kCompOnly || test_type == kFullTest)
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

    if (test_type == kFullTest)
        print_header_full_test(argc, argv, main_test_dir_char);

    if (test_type == kCompOnly)
        print_header_compression_only(argc, argv, main_test_dir_char);

    if (test_type == kTestOnly)
        print_header_test_only(argc, argv, cur_test_dir_str);

    vpxt_cap_string_print(PRINT_BTH, "%s", test_dir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    /////////////////// Use Custom Settings ///////////////////
    if (input_ver == 2)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n",
                argv[argc-1]);

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = bitrate;
    opt.lag_in_frames = 10;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;
        opt.allow_lag = 0;

        if (vpxt_compress(input.c_str(), allow_lag_off_enc.c_str(), speed,
            bitrate, opt, comp_out_str, 0, 1, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.allow_lag = 1;

        if (vpxt_compress(input.c_str(), allow_lag_on_enc.c_str(), speed,
            bitrate, opt, comp_out_str, 1, 1, enc_format) == -1)
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
        record_test_complete(file_index_str, file_index_output_char,
            test_type);
        return kTestEncCreated;
    }

    //////////// Make Sure That Frames are lagged ////////////
    std::string quant_in_str;
    vpxt_remove_file_extension(allow_lag_on_enc.c_str(), quant_in_str);
    quant_in_str += "quantizers.txt";

    int lag_in_frames_found = vpxt_lag_in_frames_check(quant_in_str.c_str());
    int allow_lag_on_alt_ref_count = vpxt_display_alt_ref_frames(
        allow_lag_on_enc.c_str(), 1);
    int allow_lag_off_alt_ref_count = vpxt_display_alt_ref_frames(
        allow_lag_off_enc.c_str() , 1);
    int visible_frame_on_count = vpxt_display_visible_frames(
        allow_lag_on_enc.c_str(), 1);
    int visible_frame_off_count = vpxt_display_visible_frames(
        allow_lag_off_enc.c_str(), 1);

    char allow_lag_on_file_name[255];
    vpxt_file_name(allow_lag_on_enc.c_str(), allow_lag_on_file_name, 0);
    char allow_lag_off_file_name[255];
    vpxt_file_name(allow_lag_off_enc.c_str(), allow_lag_off_file_name, 0);

    int lng_rc = vpxt_compare_enc(allow_lag_off_enc.c_str(),
        allow_lag_on_enc.c_str(), 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

    if (lag_in_frames_found == opt.lag_in_frames)
    {
        vpxt_formated_print(RESPRT, "Correct number of LagInFrames (%i) "
            "detected in compression for %s - Passed", opt.lag_in_frames,
            allow_lag_on_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Correct number of LagInFrames (%i) not "
            "detected in compression for %s - Failed", opt.lag_in_frames,
            allow_lag_on_file_name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (allow_lag_on_alt_ref_count > 0)
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames exist for %s "
            "- Passed", allow_lag_on_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames do not exist "
            "for %s - Failed", allow_lag_on_file_name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (visible_frame_on_count == visible_frame_off_count)
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i == Visible "
            "Frames for %s = %i - Passed", allow_lag_on_file_name,
            visible_frame_on_count, allow_lag_off_file_name,
            visible_frame_off_count);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i != Visible "
            "Frames for %s = %i - Failed", allow_lag_on_file_name,
            visible_frame_on_count, allow_lag_off_file_name,
            visible_frame_off_count);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (lng_rc >= 0)
    {
        vpxt_formated_print(RESPRT, "%s is not identical to %s - Passed",
            allow_lag_on_file_name, allow_lag_off_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if (lng_rc == -1)
    {
        vpxt_formated_print(RESPRT, "%s is identical to %s - Failed",
            allow_lag_on_file_name, allow_lag_off_file_name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(2, allow_lag_on_enc.c_str(),
            allow_lag_off_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(2, allow_lag_on_enc.c_str(),
            allow_lag_off_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}