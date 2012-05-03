#include "vpxt_test_declarations.h"

int test_arnr(int argc,
              const char** argv,
              const std::string &working_dir,
              const std::string sub_folder_str,
              int test_type,
              int delete_ivf)
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
        0, PRINT_BTH, 1, NULL);
    double arnr_enc_1_psnr = vpxt_psnr(input.c_str(), arnr_1_enc.c_str(), 0,
        PRINT_BTH, 1, NULL);
    double arnr_enc_2_psnr = vpxt_psnr(input.c_str(), arnr_2_enc.c_str(), 0,
        PRINT_BTH, 1, NULL);
    double arnr_enc_3_psnr = vpxt_psnr(input.c_str(), arnr_3_enc.c_str(), 0,
        PRINT_BTH, 1, NULL);

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

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

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
        fail = 1;
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
        fail = 1;
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
        fail = 1;
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
            fail = 1;
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
            fail = 1;
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
            fail = 1;
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
            fail = 1;
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
            fail = 1;
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
            fail = 1;
        }
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(4, arnr_0_enc.c_str(), arnr_1_enc.c_str(),
            arnr_2_enc.c_str(), arnr_3_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(4, arnr_0_enc.c_str(), arnr_1_enc.c_str(),
            arnr_2_enc.c_str(), arnr_3_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);

    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}
