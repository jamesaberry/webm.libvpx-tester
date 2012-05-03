#include "vpxt_test_declarations.h"

int test_lag_in_frames(int argc,
                       const char** argv,
                       const std::string &working_dir,
                       const std::string sub_folder_str,
                       int test_type,
                       int delete_ivf)
{
    char *comp_out_str = "Lag In Frames";
    char *test_dir = "test_lag_in_frames";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);;
    int lag_in_frames_1_val = atoi(argv[5]);
    int lag_in_frames_2_val = atoi(argv[6]);
    std::string enc_format = argv[7];

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

    char lag_in_frames_buff[255];

    std::string lag_in_frames_0 = cur_test_dir_str + slashCharStr() +
        "test_lag_in_frames_compression_0";
    vpxt_enc_format_append(lag_in_frames_0, enc_format);

    vpxt_itoa_custom(lag_in_frames_1_val, lag_in_frames_buff, 10);
    std::string lag_in_frames_1 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_";
    lag_in_frames_1 += lag_in_frames_buff;
    vpxt_enc_format_append(lag_in_frames_1, enc_format);

    vpxt_itoa_custom(lag_in_frames_2_val, lag_in_frames_buff, 10);
    std::string lag_in_frames_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_";
    lag_in_frames_2 += *lag_in_frames_buff;
    vpxt_enc_format_append(lag_in_frames_2, enc_format);

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

    if (lag_in_frames_1_val > 25 || lag_in_frames_2_val > 25 ||
        lag_in_frames_2_val < 0 || lag_in_frames_1_val < 0)
    {
        tprintf(PRINT_BTH, "\nLag in Frames settings must be between 0 and "
            "25.\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
    }

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
        opt.lag_in_frames = lag_in_frames_1_val;

        if (vpxt_compress(input.c_str(), lag_in_frames_0.c_str(), speed,
            bitrate, opt, comp_out_str, 0, 1, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.allow_lag = 1;
        opt.lag_in_frames = lag_in_frames_1_val;

        if (vpxt_compress(input.c_str(), lag_in_frames_1.c_str(), speed,
            bitrate, opt, comp_out_str, lag_in_frames_2_val, 1, enc_format)
            == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.allow_lag = 1;
        opt.lag_in_frames = lag_in_frames_2_val;

        if (vpxt_compress(input.c_str(), lag_in_frames_2.c_str(), speed,
            bitrate, opt, comp_out_str, lag_in_frames_2_val, 1, enc_format)
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

    double lag_in_frames_0_psnr = vpxt_psnr(input.c_str(),
        lag_in_frames_0.c_str(), 0, PRINT_BTH, 1, NULL);
    double lag_in_frames_1_psnr = vpxt_psnr(input.c_str(),
        lag_in_frames_1.c_str(), 0, PRINT_BTH, 1, NULL);
    double lag_in_frames_2_psnr = vpxt_psnr(input.c_str(),
        lag_in_frames_2.c_str(), 0, PRINT_BTH, 1, NULL);

    double ten_per_0 = lag_in_frames_0_psnr / 10;
    double ten_per_1 = lag_in_frames_1_psnr / 10;
    double ten_per_2 = lag_in_frames_2_psnr / 10;

    int lng_rc_1 = vpxt_compare_enc(lag_in_frames_0.c_str(),
        lag_in_frames_1.c_str(),0);
    int lng_rc_2 = vpxt_compare_enc(lag_in_frames_1.c_str(),
        lag_in_frames_2.c_str(),0);

    std::string quant_in_0_str;
    vpxt_remove_file_extension(lag_in_frames_0.c_str(), quant_in_0_str);
    quant_in_0_str += "quantizers.txt";
    int lag_in_frames_found_0 =vpxt_lag_in_frames_check(quant_in_0_str.c_str());

    std::string quant_in_1_str;
    vpxt_remove_file_extension(lag_in_frames_1.c_str(), quant_in_1_str);
    quant_in_1_str += "quantizers.txt";
    int lag_in_frames_found_1 =vpxt_lag_in_frames_check(quant_in_1_str.c_str());

    std::string quant_in_2_str;
    vpxt_remove_file_extension(lag_in_frames_2.c_str(), quant_in_2_str);
    quant_in_2_str += "quantizers.txt";
    int lag_in_frames_found_2 =vpxt_lag_in_frames_check(quant_in_2_str.c_str());

    int fail = 0;

    char lag_in_frame_0_file_name[255] = "";
    char lag_in_frame_1_file_name[255] = "";
    char lag_in_frame_2_file_name[255] = "";

    vpxt_file_name(lag_in_frames_0.c_str(), lag_in_frame_0_file_name, 0);
    vpxt_file_name(lag_in_frames_1.c_str(), lag_in_frame_1_file_name, 0);
    vpxt_file_name(lag_in_frames_2.c_str(), lag_in_frame_2_file_name, 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (lag_in_frames_found_0 == 0)
    {
        vpxt_formated_print(RESPRT, "%s properly lagged %i frames - Passed",
            lag_in_frame_0_file_name, lag_in_frames_found_0);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s improperly lagged frames %i - Failed",
            lag_in_frame_0_file_name, lag_in_frames_found_0);
        tprintf(PRINT_BTH, "\n");
    }

    if (lag_in_frames_found_1 == lag_in_frames_1_val)
    {
        vpxt_formated_print(RESPRT, "%s properly lagged %i frames - Passed",
            lag_in_frame_1_file_name, lag_in_frames_found_1);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s improperly lagged %i frames - Failed",
            lag_in_frame_1_file_name, lag_in_frames_found_1);
        tprintf(PRINT_BTH, "\n");
    }

    if (lag_in_frames_found_2 == lag_in_frames_2_val)
    {
        vpxt_formated_print(RESPRT, "%s properly lagged %i frames - Passed",
            lag_in_frame_2_file_name, lag_in_frames_found_2);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s improperly lagged %i frames - Failed",
            lag_in_frame_2_file_name, lag_in_frames_found_2);
        tprintf(PRINT_BTH, "\n");
    }

    if (lng_rc_1 == -1)
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed",
            lag_in_frame_0_file_name, lag_in_frame_1_file_name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed",
            lag_in_frame_0_file_name, lag_in_frame_1_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if (lng_rc_2 == -1)
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed",
            lag_in_frame_1_file_name, lag_in_frame_2_file_name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed",
            lag_in_frame_1_file_name, lag_in_frame_2_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if (lag_in_frames_1_psnr <= (lag_in_frames_0_psnr + ten_per_0) &&
        lag_in_frames_1_psnr >= (lag_in_frames_0_psnr - ten_per_0))
    {
        vpxt_formated_print(RESPRT, "PSNR for %s is within 10%% of PSNR for %s "
            "- %.2f < %.2f < %.2f - Passed", lag_in_frame_0_file_name,
            lag_in_frame_1_file_name, (lag_in_frames_0_psnr - ten_per_0),
            lag_in_frames_1_psnr, (lag_in_frames_0_psnr + ten_per_0));
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        if (!(lag_in_frames_1_psnr <= (lag_in_frames_0_psnr + ten_per_0)))
        {
            vpxt_formated_print(RESPRT, "PSNR for %s is not within 10%% of "
                "PSNR for %s - %.2f < %.2f - Failed", lag_in_frame_0_file_name,
                lag_in_frame_1_file_name, (lag_in_frames_0_psnr + ten_per_0),
                lag_in_frames_1_psnr);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "PSNR for %s is not within 10%% of "
                "PSNR for %s - %.2f < %.2f - Failed", lag_in_frame_0_file_name,
                lag_in_frame_1_file_name, lag_in_frames_1_psnr,
                (lag_in_frames_0_psnr - ten_per_0));
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (lag_in_frames_2_psnr <= (lag_in_frames_1_psnr + ten_per_1) &&
        lag_in_frames_2_psnr >= (lag_in_frames_1_psnr - ten_per_1))
    {
        vpxt_formated_print(RESPRT, "PSNR for %s is within 10%% of PSNR for %s "
            "- %.2f < %.2f < %.2f - Passed", lag_in_frame_1_file_name,
            lag_in_frame_2_file_name, (lag_in_frames_1_psnr - ten_per_1),
            lag_in_frames_2_psnr, (lag_in_frames_1_psnr + ten_per_1));
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        if (!(lag_in_frames_1_psnr <= (lag_in_frames_0_psnr + ten_per_0)))
        {
            vpxt_formated_print(RESPRT, "PSNR for %s is not within 10%% of "
                "PSNR for %s - %.2f < %.2f - Failed", lag_in_frame_1_file_name,
                lag_in_frame_2_file_name, (lag_in_frames_1_psnr + ten_per_1),
                lag_in_frames_2_psnr);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "PSNR for %s is not within 10%% of "
                "PSNR for %s - %.2f < %.2f - Failed", lag_in_frame_1_file_name,
                lag_in_frame_2_file_name, lag_in_frames_2_psnr,
                (lag_in_frames_1_psnr - ten_per_1));
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(3, lag_in_frames_0.c_str(),
            lag_in_frames_1.c_str(), lag_in_frames_2.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(3, lag_in_frames_0.c_str(),
            lag_in_frames_1.c_str(), lag_in_frames_2.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}