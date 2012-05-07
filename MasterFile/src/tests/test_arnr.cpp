#include "vpxt_test_declarations.h"

int test_arnr(int argc,
              const char** argv,
              const std::string &working_dir,
              const std::string sub_folder_str,
              int test_type,
              int delete_ivf,
              int artifact_detection)
{
    char *comp_out_str = "Arnr Type";
    char *test_dir = "test_arnr";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int speed = 0;
    unsigned int time_0 = 0;
    unsigned int time_1 = 0;
    unsigned int time_2 = 0;
    unsigned int time_3 = 0;

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    int arnr_0_enc_art_det = artifact_detection;
    int arnr_1_enc_art_det = artifact_detection;
    int arnr_2_enc_art_det = artifact_detection;
    int arnr_3_enc_art_det = artifact_detection;

    std::string arnr_0_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_1_altref_0";
    vpxt_enc_format_append(arnr_0_enc, enc_format);

    std::string arnr_1_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_1";
    vpxt_enc_format_append(arnr_1_enc, enc_format);

    std::string arnr_2_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_2";
    vpxt_enc_format_append(arnr_2_enc, enc_format);

    std::string arnr_3_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_3";
    vpxt_enc_format_append(arnr_3_enc, enc_format);

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
        opt.arnr_max_frames = 10;
        opt.allow_lag = 1;
        opt.lag_in_frames = 20;

        opt.play_alternate = 0;
        opt.arnr_type = 1;
        unsigned int cpu_tick_0 = 0;

        time_0 = vpxt_time_compress(input.c_str(), arnr_0_enc.c_str(),
            speed, bitrate, opt, "Play Alternate", opt.arnr_type, 0, cpu_tick_0,
            enc_format);

        if (time_1 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.play_alternate = 1;
        opt.arnr_type = 1;
        unsigned int cpu_tick_1 = 0;

        time_1 = vpxt_time_compress(input.c_str(), arnr_1_enc.c_str(), speed,
            bitrate, opt, comp_out_str, opt.arnr_type, 0, cpu_tick_1,
            enc_format);

        if (time_1 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.arnr_type = 2;
        unsigned int cpu_tick_2 = 0;

        time_2 = vpxt_time_compress(input.c_str(), arnr_2_enc.c_str(), speed,
            bitrate, opt, comp_out_str, opt.arnr_type, 0, cpu_tick_2,
            enc_format);

        if (time_2 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.arnr_type = 3;
        unsigned int cpu_tick3 = 0;

        time_3 = vpxt_time_compress(input.c_str(), arnr_3_enc.c_str(), speed,
            bitrate, opt, comp_out_str, opt.arnr_type, 0, cpu_tick3,enc_format);

        if (time_3 == -1)
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

    double arnr_enc_0_psnr = vpxt_psnr(input.c_str(), arnr_0_enc.c_str(),
        0, PRINT_BTH, 1, 0, 0, 0, NULL, arnr_0_enc_art_det);
    double arnr_enc_1_psnr = vpxt_psnr(input.c_str(), arnr_1_enc.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, arnr_1_enc_art_det);
    double arnr_enc_2_psnr = vpxt_psnr(input.c_str(), arnr_2_enc.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, arnr_2_enc_art_det);
    double arnr_enc_3_psnr = vpxt_psnr(input.c_str(), arnr_3_enc.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, arnr_3_enc_art_det);

    double arnr_enc_0_data_rate = vpxt_data_rate(arnr_0_enc.c_str(), 1);
    double arnr_enc_1_data_rate = vpxt_data_rate(arnr_1_enc.c_str(), 1);
    double arnr_enc_2_data_rate = vpxt_data_rate(arnr_2_enc.c_str(), 1);
    double arnr_enc_3_data_rate = vpxt_data_rate(arnr_3_enc.c_str(), 1);

    char arnr_enc_0_file_name[256];
    char arnr_enc_1_file_name[256];
    char arnr_enc_2_file_name[256];
    char arnr_enc_3_file_name[256];

    vpxt_file_name(arnr_0_enc.c_str(), arnr_enc_0_file_name, 0);
    vpxt_file_name(arnr_1_enc.c_str(), arnr_enc_1_file_name, 0);
    vpxt_file_name(arnr_2_enc.c_str(), arnr_enc_2_file_name, 0);
    vpxt_file_name(arnr_3_enc.c_str(), arnr_enc_3_file_name, 0);

    tprintf(PRINT_BTH, "\nComparing %s to %s\n", arnr_enc_0_file_name,
        arnr_enc_1_file_name);
    int compare_1 = vpxt_compare_enc(arnr_0_enc.c_str(), arnr_1_enc.c_str(),
        0);

    if (compare_1 >= 0)
        tprintf(PRINT_BTH, "\n Files differ at frame: %i\n", compare_1);
    else
        tprintf(PRINT_BTH, "\n Files are identical\n");

    tprintf(PRINT_BTH, "\nComparing %s to %s\n", arnr_enc_1_file_name,
        arnr_enc_2_file_name);
    int compare_2 = vpxt_compare_enc(arnr_1_enc.c_str(), arnr_2_enc.c_str(), 0);

    if (compare_2 >= 0)
        tprintf(PRINT_BTH, "\n Files differ at frame: %i\n", compare_2);
    else
        tprintf(PRINT_BTH, "\n Files are identical\n");

    tprintf(PRINT_BTH, "\nComparing %s to %s\n", arnr_enc_2_file_name,
        arnr_enc_3_file_name);
    int compare_3 = vpxt_compare_enc(arnr_2_enc.c_str(), arnr_3_enc.c_str(), 0);

    if (compare_3 >= 0)
        tprintf(PRINT_BTH, "\n Files differ at frame: %i\n", compare_3);
    else
        tprintf(PRINT_BTH, "\n Files are identical\n");

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (compare_1 >= 0)
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed",
            arnr_enc_0_file_name, arnr_enc_1_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed",
            arnr_enc_0_file_name, arnr_enc_1_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (compare_2 >= 0)
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed",
            arnr_enc_1_file_name, arnr_enc_2_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed",
            arnr_enc_1_file_name, arnr_enc_2_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (compare_3 >= 0)
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed",
            arnr_enc_2_file_name, arnr_enc_3_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed",
            arnr_enc_2_file_name, arnr_enc_3_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    // if greater than - pass
    if (arnr_enc_1_psnr > arnr_enc_0_psnr)
    {
        vpxt_formated_print(RESPRT, "Arnr 1 PSNR: %.2f > Alt Ref 0 PSNR: %.2f "
            "- Passed", arnr_enc_1_psnr, arnr_enc_0_psnr);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        // if less than but within 5% - pass
        if (arnr_enc_1_psnr >= (arnr_enc_0_psnr - (arnr_enc_0_psnr * 0.05)))
        {
            vpxt_formated_print(RESPRT, "Arnr 1 PSNR: %.2f is with in 5%% of "
                "Alt Ref 0 PSNR: %.2f - Passed", arnr_enc_1_psnr,
                arnr_enc_0_psnr);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 1 PSNR: %.2f not within 5%% of "
                "Alt Ref 0 PSNR: %.2f - Failed", arnr_enc_1_psnr,
                arnr_enc_0_psnr);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    if (arnr_enc_2_psnr > arnr_enc_0_psnr) // if greater than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 2 PSNR: %.2f > Alt Ref 0 PSNR: %.2f "
            "- Passed", arnr_enc_2_psnr, arnr_enc_0_psnr);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        // if less than but within 5% - pass
        if (arnr_enc_2_psnr >= (arnr_enc_0_psnr - (arnr_enc_0_psnr * 0.05)))
        {
            vpxt_formated_print(RESPRT, "Arnr 2 PSNR: %.2f is with in 5%% of "
                "Alt Ref 0 PSNR: %.2f - Passed", arnr_enc_2_psnr,
                arnr_enc_0_psnr);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 2 PSNR: %.2f not within 5%% of "
                "Alt Ref 0 PSNR: %.2f - Failed", arnr_enc_2_psnr,
                arnr_enc_0_psnr);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    if (arnr_enc_3_psnr > arnr_enc_0_psnr) // if greater than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 3 PSNR: %.2f > Alt Ref 0 PSNR: %.2f "
            "- Passed", arnr_enc_3_psnr, arnr_enc_0_psnr);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        // if less than but within 5% - pass
        if (arnr_enc_3_psnr >= (arnr_enc_0_psnr - (arnr_enc_0_psnr * 0.05)))
        {
            vpxt_formated_print(RESPRT, "Arnr 3 PSNR: %.2f is with in 5%% of "
                "Alt Ref 0 PSNR: %.2f - Passed", arnr_enc_3_psnr,
                arnr_enc_0_psnr);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 3 PSNR: %.2f not within 5%% of "
                "Alt Ref 0 PSNR: %.2f - Failed", arnr_enc_3_psnr,
                arnr_enc_0_psnr);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    if (arnr_enc_1_data_rate < arnr_enc_0_data_rate) // if less than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 1 Data Rate: %.2f < Alt Ref 0 Data "
            "Rate: %.2f - Passed", arnr_enc_1_data_rate, arnr_enc_0_data_rate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        // if greater than but within 5% - pass
        if (arnr_enc_1_psnr <= (arnr_enc_0_psnr + (arnr_enc_0_psnr * 0.05)))
        {
            vpxt_formated_print(RESPRT, "Arnr 1 Data Rate: %.2f is with in 5%% "
                "of Alt Ref 0 Data Rate: %.2f - Passed", arnr_enc_1_data_rate,
                arnr_enc_0_data_rate);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 1 Data Rate: %.2f not within 5%% "
                "of Alt Ref 0 Data Rate: %.2f - Failed", arnr_enc_1_data_rate,
                arnr_enc_0_data_rate);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    if (arnr_enc_2_data_rate < arnr_enc_0_data_rate) // if less than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 2 Data Rate: %.2f < Alt Ref 0 Data "
            "Rate: %.2f - Passed", arnr_enc_2_data_rate, arnr_enc_0_data_rate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        // if greater than but within 5% - pass
        if (arnr_enc_2_psnr <= (arnr_enc_0_psnr + (arnr_enc_0_psnr * 0.05)))
        {
            vpxt_formated_print(RESPRT, "Arnr 2 Data Rate: %.2f is with in 5%% "
                "of Alt Ref 0 Data Rate: %.2f - Passed", arnr_enc_2_data_rate,
                arnr_enc_0_data_rate);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 2 Data Rate: %.2f not within 5%% "
                "of Alt Ref 0 Data Rate: %.2f - Failed", arnr_enc_2_data_rate,
                arnr_enc_0_data_rate);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    if (arnr_enc_3_data_rate < arnr_enc_0_data_rate) // if less than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 3 Data Rate: %.2f < Alt Ref 0 Data "
            "Rate: %.2f - Passed", arnr_enc_3_data_rate, arnr_enc_0_data_rate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        // if greater than but within 5% - pass
        if (arnr_enc_3_psnr <= (arnr_enc_0_psnr + (arnr_enc_0_psnr * 0.05)))
        {
            vpxt_formated_print(RESPRT, "Arnr 3 Data Rate: %.2f is with in 5%% "
                "of Alt Ref 0 Data Rate: %.2f - Passed", arnr_enc_3_data_rate,
                arnr_enc_0_data_rate);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 3 Data Rate: %.2f not within 5%% "
                "of Alt Ref 0 Data Rate: %.2f - Failed", arnr_enc_3_data_rate,
                arnr_enc_0_data_rate);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    // handle possible artifact
    if(arnr_0_enc_art_det == kPossibleArtifactFound || arnr_1_enc_art_det ==
        kPossibleArtifactFound || arnr_2_enc_art_det == kPossibleArtifactFound
        || arnr_3_enc_art_det == kPossibleArtifactFound)
    {
        tprintf(PRINT_BTH, "\nPossible Artifact\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPossibleArtifact;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(4, arnr_0_enc.c_str(), arnr_1_enc.c_str(),
        arnr_2_enc.c_str(), arnr_3_enc.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
