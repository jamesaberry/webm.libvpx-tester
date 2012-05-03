#include "vpxt_test_declarations.h"

int test_constrained_quality(int argc,
                             const char *const *argv,
                             const std::string &working_dir,
                             const std::string sub_folder_str,
                             int test_type,
                             int delete_ivf)
{
    char *comp_out_str = "Constrained Quality";
    char *test_dir = "test_constrained_quality";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int constrained_q = atoi(argv[5]);
    std::string enc_format = argv[6];

    int speed = 0;
    unsigned int time_1 = 0;
    unsigned int time_2 = 0;

    ////////////Formatting Test Specific directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return TEST_ERRFM;

    std::string constrained_q_on_enc = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_1";
    vpxt_enc_format_append(constrained_q_on_enc, enc_format);

    std::string constrained_q_off_enc = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_0";
    vpxt_enc_format_append(constrained_q_off_enc, enc_format);

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

    opt.target_bandwidth = bitrate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;
        opt.end_usage = 1;
        unsigned int cpu_tick1_conq_off = 0;

        time_1 = vpxt_time_compress(input.c_str(), constrained_q_off_enc.c_str()
            , speed, bitrate, opt, comp_out_str, 0, 1, cpu_tick1_conq_off,
            enc_format);

        if (time_1 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.end_usage = 2;
        opt.cq_level = constrained_q;
        unsigned int cpu_tick1_conq_on = 0;

        time_2 = vpxt_time_compress(input.c_str(), constrained_q_on_enc.c_str(),
            speed, bitrate, opt, comp_out_str, 1, 1, cpu_tick1_conq_on,
            enc_format);

        if (time_2 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }
    }

    if (test_type == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
    }

    double constrained_q_on_psnr = vpxt_psnr(input.c_str(),
        constrained_q_on_enc.c_str(),
        0, PRINT_BTH, 1, NULL);
    double constrained_q_off_psnr = vpxt_psnr(input.c_str(),
        constrained_q_off_enc.c_str(), 0, PRINT_BTH, 1, NULL);

    double constrained_q_on_data_rate = vpxt_data_rate(
        constrained_q_on_enc.c_str(), 1);
    double constrained_q_off_data_rate = vpxt_data_rate(
        constrained_q_off_enc.c_str(), 1);

    vpxt_display_alt_ref_frames(constrained_q_on_enc.c_str(), 1);
    vpxt_display_alt_ref_frames(constrained_q_off_enc.c_str(), 1);
    vpxt_display_key_frames(constrained_q_on_enc.c_str(), 1);
    vpxt_display_key_frames(constrained_q_off_enc.c_str(), 1);

    tprintf(PRINT_BTH, "\nCQ on  PSNR: %.2f CQ on  Data Rate: %.2f CQ on  "
        "linearized PSNR: %.2f\n", constrained_q_on_psnr,
        constrained_q_on_data_rate, pow(10.0, (constrained_q_on_psnr / 10.0)));
    tprintf(PRINT_BTH, "CQ off PSNR: %.2f CQ off Data Rate: %.2f CQ off "
        "linearized PSNR: %.2f\n", constrained_q_off_psnr,
        constrained_q_off_data_rate,pow(10.0, (constrained_q_off_psnr / 10.0)));
    tprintf(PRINT_BTH, "\nConstrained Q on  time: %i microseconds\n", time_2);
    tprintf(PRINT_BTH, "Constrained Q off time: %i microseconds\n", time_1);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

    if (pow(10.0, (constrained_q_on_psnr / 10.0)) / constrained_q_on_data_rate >
        pow(10.0, (constrained_q_off_psnr / 10.0)) /constrained_q_off_data_rate)
    {
        vpxt_formated_print(RESPRT, "CQ on lineraized psnr / data rate: %f > "
            "CQ off lineraized psnr / data rate: %f - Passed",
            pow(10.0, (constrained_q_on_psnr / 10.0)) /
            constrained_q_on_data_rate, pow(10.0, (constrained_q_off_psnr /
            10.0)) / constrained_q_off_data_rate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "CQ on lineraized psnr / data rate: %f <= "
            "CQ off lineraized psnr / data rate: %f - Failed",
            pow(10.0, (constrained_q_on_psnr / 10.0)) /
            constrained_q_on_data_rate, pow(10.0, (constrained_q_off_psnr /
            10.0)) / constrained_q_off_data_rate);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (constrained_q_on_psnr > 25.0)
    {
        vpxt_formated_print(RESPRT, "Constrained quality PSNR: %f > 25 - "
            "Passed", constrained_q_on_psnr);
        tprintf(PRINT_BTH, "\n");
    }

    if (constrained_q_on_psnr <= 25.0)
    {
        vpxt_formated_print(RESPRT, "Constrained quality PSNR: %f <= 25 - "
            "Failed", constrained_q_on_psnr);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(2, constrained_q_on_enc.c_str(),
            constrained_q_off_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(2, constrained_q_on_enc.c_str(),
            constrained_q_off_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }

    fclose(fp);

    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}
