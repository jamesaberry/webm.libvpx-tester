#include "vpxt_test_declarations.h"

int test_speed(int argc,
               const char *const *argv,
               const std::string &working_dir,
               std::string files_ar[],
               int test_type,
               int delete_ivf)
{
    char *comp_out_str = "Cpu Used";
    char *test_dir = "test_speed";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int lag_in_frames_input = atoi(argv[5]);
    std::string enc_format = argv[6];

    int speed = 0;
    int fail_1 = 0;
    int fail_2 = 0;
    int fail_3 = 0;
    int fail_4 = 0;

    if (mode != 0 && mode != 1)
        return vpxt_test_help(argv[1], 0);

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return 11;

    std::string speed_test_good_quality_base = cur_test_dir_str + slashCharStr()
        + test_dir + "_compression_cpu_used_";

    std::string speed_test_real_time_base = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_cpu_used_";

    int speed_counter = 0;
    std::string speed_test_real_time_str_arr[34];

    while (speed_counter < 33)
    {
        if (speed_counter < 17)
        {
            char counter_char[4];
            vpxt_itoa_custom(speed_counter, counter_char, 10);

            speed_test_real_time_str_arr[speed_counter] =
                speed_test_real_time_base + counter_char;
            vpxt_enc_format_append(speed_test_real_time_str_arr[speed_counter],
                enc_format);
        }
        else
        {
            int counter = speed_counter - 16;
            char counter_char[4];
            vpxt_itoa_custom(counter, counter_char, 10);

            speed_test_real_time_str_arr[speed_counter] =
                speed_test_real_time_base + "-" + counter_char;
            vpxt_enc_format_append(speed_test_real_time_str_arr[speed_counter],
                enc_format);
        }

        speed_counter++;
    }

    /////////////OutPutfile////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;

    if (test_type == COMP_ONLY || test_type == TEST_AND_COMP)
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

    if (test_type == TEST_AND_COMP)
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
            record_test_complete(file_index_str, file_index_output_char, test_type);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    int compress_int = opt.cpu_used;
    opt.target_bandwidth = bitrate;
    opt.Mode = MODE_GOODQUALITY;
    opt.lag_in_frames = lag_in_frames_input;

    tprintf(PRINT_BTH, "\nLagInFrames: %i\n", lag_in_frames_input);

    unsigned int good_quality_total_cpu_tick[7];
    unsigned int real_time_total_cpu_tick[17];
    unsigned int real_time_total_cpu_tick_pos[18];

    double good_quality_psnr_arr[7];
    double real_time_psnr_arr[17];
    double real_time_psnr_arr_pos[18];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        int counter = 0;

        if (mode == 1)
        {
            while (counter < 6)
            {
                char counter_char[4];
                vpxt_itoa_custom(counter, counter_char, 10);

                std::string speed_test_good_quality_str =
                    speed_test_good_quality_base + counter_char;
                vpxt_enc_format_append(speed_test_good_quality_str, enc_format);

                good_quality_total_cpu_tick[counter] =
                    vpxt_cpu_tick_return(speed_test_good_quality_str.c_str(),0);
                good_quality_psnr_arr[counter] =
                    vpxt_psnr(input.c_str(), speed_test_good_quality_str.c_str()
                    , 1, PRINT_BTH, 1, NULL);
                counter++;
            }
        }

        if (mode == 0)
        {
            counter = -1;
            int counter_2 = 0;

            while (counter > -17)
            {
                char counter_char[4];
                vpxt_itoa_custom(counter, counter_char, 10);

                std::string speed_test_real_time_str = speed_test_real_time_base
                    + counter_char;
                vpxt_enc_format_append(speed_test_real_time_str, enc_format);

                real_time_total_cpu_tick[counter_2] =
                    vpxt_cpu_tick_return(speed_test_real_time_str.c_str(), 0);
                real_time_psnr_arr[counter_2] =
                    vpxt_psnr(input.c_str(), speed_test_real_time_str.c_str(),1,
                    PRINT_BTH, 1, NULL);

                counter--;
                counter_2++;
            }
        }
    }
    else
    {

        int counter = 0;

        if (mode == 1)
        {
            while (counter < 6)
            {
                char counter_char[4];
                vpxt_itoa_custom(counter, counter_char, 10);

                std::string speed_test_good_quality_str =
                    speed_test_good_quality_base + counter_char;
                vpxt_enc_format_append(speed_test_good_quality_str, enc_format);

                opt.cpu_used = counter;
                compress_int = opt.cpu_used;
                opt.Mode = MODE_GOODQUALITY;
                unsigned int time = vpxt_time_compress(input.c_str(),
                    speed_test_good_quality_str.c_str(), speed, bitrate, opt,
                    comp_out_str, compress_int, 0,
                    good_quality_total_cpu_tick[counter], enc_format);

                if (time == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    return 2;
                }

                if (test_type != 2 && test_type != 3)
                {
                    good_quality_psnr_arr[counter] = vpxt_psnr(input.c_str(),
                        speed_test_good_quality_str.c_str(), 1, PRINT_BTH, 1,
                        NULL);
                }

                counter++;
            }
        }

        if (mode == 0)
        {
            counter = -1;
            int counter_2 = 0;

            while (counter > -17)
            {
                char counter_char[4];
                vpxt_itoa_custom(counter, counter_char, 10);

                std::string speed_test_real_time_str = speed_test_real_time_base
                    + counter_char;
                vpxt_enc_format_append(speed_test_real_time_str, enc_format);

                opt.cpu_used = counter;
                compress_int = opt.cpu_used;
                opt.Mode = MODE_REALTIME;
                unsigned int time = vpxt_time_compress(input.c_str(),
                    speed_test_real_time_str.c_str(), speed, bitrate, opt,
                    comp_out_str, compress_int, 0,
                    real_time_total_cpu_tick[counter_2], enc_format);

                if (time == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    return 2;
                }

                if (test_type != 2 && test_type != 3)
                {
                    real_time_psnr_arr[counter_2] = vpxt_psnr(input.c_str(),
                        speed_test_real_time_str.c_str(), 1, PRINT_BTH, 1,
                        NULL);
                }

                counter--;
                counter_2++;
            }
        }
    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 10;
    }

    tprintf(PRINT_BTH, "\n");

    int counter = 1;
    int pass = 1;

    if (mode == 1)
    {
        while (counter < 6)
        {
            if (good_quality_total_cpu_tick[counter] <
                good_quality_total_cpu_tick[counter-1])
            {
                tprintf(PRINT_BTH, "      CpuUsed %*i Encode Tick: %i < CpuUsed"
                    " %*i Encode Tick: %i\n", 2, counter,
                    good_quality_total_cpu_tick[counter], 2, counter - 1,
                    good_quality_total_cpu_tick[counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "Fail: CpuUsed %*i Encode Tick: %i > "
                    "CpuUsed %*i Encode Tick: %i\n", 2, counter,
                    good_quality_total_cpu_tick[counter], 2, counter - 1,
                    good_quality_total_cpu_tick[counter-1]);
                fail_1 = 1;
            }

            float psnr_percent;
            psnr_percent = vpxt_abs_float(good_quality_psnr_arr[counter] -
                good_quality_psnr_arr[counter-1]) /
                good_quality_psnr_arr[counter-1];

            if (psnr_percent < 0.1)
            {
                tprintf(PRINT_BTH, "      CpuUsed %*i PSNR: %4.2f within 10%% "
                    "of CpuUsed %*i PSNR: %4.2f\n", 2, counter,
                    good_quality_psnr_arr[counter], 2, counter - 1,
                    good_quality_psnr_arr[counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "Fail: CpuUsed %*i PSNR: %4.2f not within "
                    "10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter,
                    good_quality_psnr_arr[counter], 2, counter - 1,
                    good_quality_psnr_arr[counter-1]);
                fail_2 = 1;
            }

            counter++;
        }
    }

    if (mode == 0)
    {
        counter = 1;

        while (counter < 16)
        {
            if (real_time_total_cpu_tick[counter] <
                real_time_total_cpu_tick[counter-1])
            {
                tprintf(PRINT_BTH, "      CpuUsed -%*i Encode Tick: %i < "
                    "CpuUsed -%*i Encode Tick: %i\n", 2, counter + 1,
                    real_time_total_cpu_tick[counter], 2, counter,
                    real_time_total_cpu_tick[counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "fail_1: CpuUsed -%*i Encode Tick: %i > "
                    "CpuUsed -%*i Encode Tick: %i\n", 2, counter + 1,
                    real_time_total_cpu_tick[counter], 2, counter,
                    real_time_total_cpu_tick[counter-1]);
                fail_1++;

                float time_percent = (real_time_total_cpu_tick[counter] -
                    real_time_total_cpu_tick[counter-1]);

                if (time_percent < 0)
                    time_percent = time_percent * (-1);

                if (time_percent > (real_time_total_cpu_tick[counter-1] / 10))
                    fail_3++;
            }

            float psnr_percent;
            psnr_percent = vpxt_abs_float(real_time_psnr_arr[counter] -
                real_time_psnr_arr[counter-1]) / real_time_psnr_arr[counter-1];

            if (psnr_percent < 0.1)
            {
                tprintf(PRINT_BTH, "      CpuUsed -%*i PSNR: %4.2f within 10%% "
                    "of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1,
                    real_time_psnr_arr[counter], 2, counter,
                    real_time_psnr_arr[counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "fail_1: CpuUsed -%*i PSNR: %4.2f not within "
                    "10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1,
                    real_time_psnr_arr[counter], 2, counter,
                    real_time_psnr_arr[counter-1]);
                fail_2++;
            }

            counter++;
        }

        tprintf(PRINT_BTH, "\n\n");
    }


    if (mode == 0)
    {
        tprintf(PRINT_BTH, "\n\nResults:\n\n");

        if (fail_1 == 0)
        {
            vpxt_formated_print(RESPRT, "All encode ticks decrease as CpuUsed "
                "increases - Passed");
            tprintf(PRINT_BTH, "\n");
        }

        if (fail_1 < 4 && fail_1 != 0)
        {
            vpxt_formated_print(RESPRT, "Enough encode ticks decrease as "
                "CpuUsed increases - Min Passed");
            tprintf(PRINT_BTH, "\n");
            pass = 2;
        }

        if (fail_1 >= 4)
        {
            vpxt_formated_print(RESPRT, "Not enough encode ticks decrease as "
                "CpuUsed increases - Failed");
            tprintf(PRINT_BTH, "\n");
            pass = 0;
        }

        if (fail_2 == 0)
        {
            vpxt_formated_print(RESPRT, "All PSNRs are within 10%% - Passed");
            tprintf(PRINT_BTH, "\n");
        }

        if (fail_2 < 2 && fail_2 != 0)
        {
            vpxt_formated_print(RESPRT, "Enough PSNRs are within 10%% - "
                "Min Passed");
            tprintf(PRINT_BTH, "\n");
            pass = 2;
        }

        if (fail_2 >= 2)
        {
            vpxt_formated_print(RESPRT, "Not enough PSNRs are within 10%% - "
                "Failed");
            tprintf(PRINT_BTH, "\n");
            pass = 0;
        }

        if (fail_3 != 0)
        {
            vpxt_formated_print(RESPRT, "Not all Encode speeds are within 10%% "
                "- Failed");
            tprintf(PRINT_BTH, "\n");
            pass = 0;
        }
    }

    if (mode == 1)
    {
        tprintf(PRINT_BTH, "\n\nResults:\n\n");

        if (fail_1 == 1)
        {
            vpxt_formated_print(RESPRT, "Not all encode ticks decrease as "
                "CpuUsed increases - Failed");
            tprintf(PRINT_BTH, "\n");
            pass = 0;
        }
        else
        {
            vpxt_formated_print(RESPRT, "All encode ticks decrease as CpuUsed "
                "increases - Passed");
            tprintf(PRINT_BTH, "\n");
        }

        if (fail_2 == 0)
        {
            vpxt_formated_print(RESPRT, "All PSNR values are within 10%% of "
                "eachother - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "Not all PSNR values are within 10%% "
                "of eachother - Failed");
            tprintf(PRINT_BTH, "\n");
            pass = 0;
        }
    }

    if (pass == 0)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
        {
            int z = 0;

            while (z < 33)
            {
                vpxt_delete_files(1, speed_test_real_time_str_arr[z].c_str());
                z++;
            }
        }

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 0;
    }

    if (pass == 1)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
        {
            int z = 0;

            while (z < 33)
            {
                vpxt_delete_files(1, speed_test_real_time_str_arr[z].c_str());
                z++;
            }
        }

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nMin Passed\n");

        if (delete_ivf)
        {
            int z = 0;

            while (z < 33)
            {
                vpxt_delete_files(1, speed_test_real_time_str_arr[z].c_str());
                z++;
            }
        }

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 8;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return 6;
}