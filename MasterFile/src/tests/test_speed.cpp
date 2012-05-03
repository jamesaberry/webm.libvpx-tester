#include "vpxt_test_declarations.h"

#define max_real_time_value 16
#define max_good_quality_value 6

int test_speed(int argc,
               const char** argv,
               const std::string &working_dir,
               const std::string sub_folder_str,
               int test_type,
               int delete_ivf,
               int artifact_detection)
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

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    int speed_test_real_time_art_det[max_real_time_value] = {0};
    int speed_test_good_quality_art_det[max_good_quality_value] = {0};

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string speed_test_base = cur_test_dir_str + slashCharStr()
        + test_dir + "_compression_cpu_used_";

    std::string speed_test_real_time_str_arr[max_real_time_value];
    std::string speed_test_good_quality_str_arr[max_good_quality_value];

    int speed_counter = 0;
    int counter = 0;
    while (speed_counter < max_good_quality_value)
    {
        char counter_char[4];
        vpxt_itoa_custom(speed_counter, counter_char, 10);

        speed_test_good_quality_str_arr[speed_counter] = speed_test_base +
            counter_char;
        vpxt_enc_format_append(speed_test_good_quality_str_arr[speed_counter],
            enc_format);

        speed_test_good_quality_art_det[speed_counter] = artifact_detection;

        speed_counter++;
    }
    speed_counter = 0;
    while (speed_counter < max_real_time_value)
    {
        char counter_char[4];
        vpxt_itoa_custom(speed_counter + 1, counter_char, 10);

        speed_test_real_time_str_arr[speed_counter] = speed_test_base + "-" +
            counter_char;
        vpxt_enc_format_append(speed_test_real_time_str_arr[speed_counter],
            enc_format);

        speed_test_real_time_art_det[speed_counter] = artifact_detection;

        speed_counter++;
    }

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
    int compress_int = opt.cpu_used;
    opt.target_bandwidth = bitrate;
    opt.Mode = MODE_GOODQUALITY;
    opt.lag_in_frames = lag_in_frames_input;

    tprintf(PRINT_BTH, "\nLagInFrames: %i\n", lag_in_frames_input);

    unsigned int good_quality_total_cpu_tick[max_good_quality_value];
    unsigned int real_time_total_cpu_tick[max_real_time_value];

    double good_quality_psnr_arr[max_good_quality_value];
    double real_time_psnr_arr[max_real_time_value];

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        speed_counter = 0; // array counter same as speed counter

        if (mode == kOnePassGoodQuality)
        {
            while (speed_counter < max_good_quality_value)
            {
                good_quality_total_cpu_tick[speed_counter] =
                    vpxt_cpu_tick_return(
                    speed_test_good_quality_str_arr[speed_counter].c_str(),0);
                good_quality_psnr_arr[speed_counter] =
                    vpxt_psnr(input.c_str(),
                    speed_test_good_quality_str_arr[speed_counter].c_str() , 1,
                    PRINT_BTH, 1, 0, 0, 0, NULL,
                    speed_test_good_quality_art_det[speed_counter]);

                speed_counter++;
            }
        }

        if (mode == kRealTime)
        {
            counter = 0; // array counter

            while (counter > -1 * max_real_time_value)
            {
                real_time_total_cpu_tick[counter] =
                    vpxt_cpu_tick_return(
                    speed_test_real_time_str_arr[counter].c_str(), 0);
                real_time_psnr_arr[counter] =
                    vpxt_psnr(input.c_str(),
                    speed_test_real_time_str_arr[counter].c_str(),1, PRINT_BTH,
                    1, 0, 0, 0, NULL, speed_test_real_time_art_det[counter]);

                counter++;
            }
        }
    }
    else
    {
        speed_counter = 0; // array counter same as speed counter

        if (mode == kOnePassGoodQuality)
        {
            while (speed_counter < max_good_quality_value)
            {
                opt.cpu_used = speed_counter;
                compress_int = opt.cpu_used;
                opt.Mode = MODE_GOODQUALITY;
                unsigned int time = vpxt_time_compress(input.c_str(),
                    speed_test_good_quality_str_arr[speed_counter].c_str(),
                    speed, bitrate, opt, comp_out_str, compress_int, 0,
                    good_quality_total_cpu_tick[speed_counter], enc_format);

                if (time == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    return kTestIndeterminate;
                }

                if (test_type != 2 && test_type != 3)
                {
                    good_quality_psnr_arr[speed_counter] = vpxt_psnr(
                        input.c_str(),
                        speed_test_good_quality_str_arr[speed_counter].c_str(),
                        1, PRINT_BTH, 1, 0, 0, 0, NULL,
                        speed_test_good_quality_art_det[speed_counter]);
                }

                speed_counter++;
            }
        }

        if (mode == kRealTime)
        {
            speed_counter = -1; // speed setting counter
            counter = 0;        // array counter

            while (speed_counter >= -1 * max_real_time_value)
            {
                opt.cpu_used = speed_counter;
                compress_int = opt.cpu_used;
                opt.Mode = MODE_REALTIME;
                unsigned int time = vpxt_time_compress(input.c_str(),
                    speed_test_real_time_str_arr[counter].c_str(), speed,
                    bitrate, opt, comp_out_str, compress_int, 0,
                    real_time_total_cpu_tick[counter], enc_format);

                if (time == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    return kTestIndeterminate;
                }

                if (test_type != 2 && test_type != 3)
                {
                    real_time_psnr_arr[counter] = vpxt_psnr(input.c_str(),
                        speed_test_real_time_str_arr[counter].c_str(), 1,
                        PRINT_BTH, 1, 0, 0, 0, NULL,
                        speed_test_real_time_art_det[counter]);
                }

                speed_counter--;
                counter++;
            }
        }
    }

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    tprintf(PRINT_BTH, "\n");
    speed_counter = 1;
    int test_state = kTestPassed;

    if (mode == kOnePassGoodQuality)
    {
        while (speed_counter < max_good_quality_value)
        {
            if (good_quality_total_cpu_tick[speed_counter] <
                good_quality_total_cpu_tick[speed_counter-1])
            {
                tprintf(PRINT_BTH, "      CpuUsed %*i Encode Tick: %i < CpuUsed"
                    " %*i Encode Tick: %i\n", 2, speed_counter,
                    good_quality_total_cpu_tick[speed_counter], 2, speed_counter
                    - 1, good_quality_total_cpu_tick[speed_counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "Fail: CpuUsed %*i Encode Tick: %i > "
                    "CpuUsed %*i Encode Tick: %i\n", 2, speed_counter,
                    good_quality_total_cpu_tick[speed_counter], 2, speed_counter
                    - 1, good_quality_total_cpu_tick[speed_counter-1]);
                fail_1 = 1;
            }

            double psnr_percent;
            psnr_percent = vpxt_abs_double(good_quality_psnr_arr[speed_counter]
            - good_quality_psnr_arr[speed_counter-1]) /
                good_quality_psnr_arr[speed_counter-1];

            if (psnr_percent < 0.1)
            {
                tprintf(PRINT_BTH, "      CpuUsed %*i PSNR: %4.2f within 10%% "
                    "of CpuUsed %*i PSNR: %4.2f\n", 2, speed_counter,
                    good_quality_psnr_arr[speed_counter], 2, speed_counter - 1,
                    good_quality_psnr_arr[speed_counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "Fail: CpuUsed %*i PSNR: %4.2f not within "
                    "10%% of CpuUsed %*i PSNR: %4.2f\n", 2, speed_counter,
                    good_quality_psnr_arr[speed_counter], 2, speed_counter - 1,
                    good_quality_psnr_arr[speed_counter-1]);
                fail_2 = 1;
            }

            speed_counter++;
        }
    }

    if (mode == kRealTime)
    {
        while (speed_counter < max_real_time_value)
        {
            if (real_time_total_cpu_tick[speed_counter] <
                real_time_total_cpu_tick[speed_counter-1])
            {
                tprintf(PRINT_BTH, "      CpuUsed -%*i Encode Tick: %i < "
                    "CpuUsed -%*i Encode Tick: %i\n", 2, speed_counter + 1,
                    real_time_total_cpu_tick[speed_counter], 2, speed_counter,
                    real_time_total_cpu_tick[speed_counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "FAIL: CpuUsed -%*i Encode Tick: %i > "
                    "CpuUsed -%*i Encode Tick: %i\n", 2, speed_counter + 1,
                    real_time_total_cpu_tick[speed_counter], 2, speed_counter,
                    real_time_total_cpu_tick[speed_counter-1]);
                fail_1++;

                double time_percent = (real_time_total_cpu_tick[speed_counter] -
                    real_time_total_cpu_tick[speed_counter-1]);

                if (time_percent < 0)
                    time_percent = time_percent * (-1);

                if (time_percent > (real_time_total_cpu_tick[speed_counter-1] /
                    10))
                    fail_3++;
            }

            double psnr_percent;
            psnr_percent = vpxt_abs_double(real_time_psnr_arr[speed_counter] -
                real_time_psnr_arr[speed_counter-1]) /
                real_time_psnr_arr[speed_counter-1];

            if (psnr_percent < 0.1)
            {
                tprintf(PRINT_BTH, "      CpuUsed -%*i PSNR: %4.2f within 10%% "
                    "of CpuUsed -%*i PSNR: %4.2f\n", 2, speed_counter + 1,
                    real_time_psnr_arr[speed_counter], 2, speed_counter,
                    real_time_psnr_arr[speed_counter-1]);
            }
            else
            {
                tprintf(PRINT_BTH, "FAIL: CpuUsed -%*i PSNR: %4.2f not within "
                    "10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, speed_counter + 1,
                    real_time_psnr_arr[speed_counter], 2, speed_counter,
                    real_time_psnr_arr[speed_counter-1]);
                fail_2++;
            }

            speed_counter++;
        }

        tprintf(PRINT_BTH, "\n\n");
    }

    if (mode == kRealTime)
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
            test_state = kTestMinPassed;
        }

        if (fail_1 >= 4)
        {
            vpxt_formated_print(RESPRT, "Not enough encode ticks decrease as "
                "CpuUsed increases - Failed");
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
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
            test_state = kTestMinPassed;
        }

        if (fail_2 >= 2)
        {
            vpxt_formated_print(RESPRT, "Not enough PSNRs are within 10%% - "
                "Failed");
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }

        if (fail_3 != 0)
        {
            vpxt_formated_print(RESPRT, "Not all Encode speeds are within 10%% "
                "- Failed");
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    if (mode == kOnePassGoodQuality)
    {
        tprintf(PRINT_BTH, "\n\nResults:\n\n");

        if (fail_1 == 1)
        {
            vpxt_formated_print(RESPRT, "Not all encode ticks decrease as "
                "CpuUsed increases - Failed");
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
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
            test_state = kTestFailed;
        }
    }

    counter = 0;
    // handle possible artifact
    if(mode == kRealTime)
    {
        while(counter < max_real_time_value)
        {
            if(speed_test_good_quality_art_det[counter] ==
                kPossibleArtifactFound)
            {
                tprintf(PRINT_BTH, "\nPossible Artifact\n");

                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestPossibleArtifact;
            }
            ++counter;
        }
    }
    else
    {
        while(counter < max_good_quality_value)
        {
            if(speed_test_real_time_art_det[counter] == kPossibleArtifactFound)
            {
                tprintf(PRINT_BTH, "\nPossible Artifact\n");

                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestPossibleArtifact;
            }
            ++counter;
        }
    }

    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestMinPassed)
        tprintf(PRINT_BTH, "\nMin Passed\n");

    if (delete_ivf)
        if(mode == kRealTime)
            for (counter = 0; counter < max_real_time_value; ++counter)
                vpxt_delete_files(1,
                speed_test_real_time_str_arr[counter].c_str());
        else
            for (counter = 0; counter < max_good_quality_value; ++counter)
                vpxt_delete_files(1,
                speed_test_good_quality_str_arr[counter].c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}