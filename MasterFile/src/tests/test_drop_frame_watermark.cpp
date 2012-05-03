#include "vpxt_test_declarations.h"

int test_drop_frame_watermark(int argc,
                              const char *const *argv,
                              const std::string &working_dir,
                              const std::string sub_folder_str,
                              int test_type,
                              int delete_ivf)
{
    char *comp_out_str = "Drop Frames Watermark";
    char *test_dir = "test_drop_frame_watermark";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int speed = 0;

    ////////////Formatting Test Specific directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return TEST_ERRFM;

    std::string dfwm_out_file_base = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_";

    std::string dfwm_out_file_0   = dfwm_out_file_base + "0";
    vpxt_enc_format_append(dfwm_out_file_0, enc_format);
    std::string dfwm_out_file_20  = dfwm_out_file_base + "20";
    vpxt_enc_format_append(dfwm_out_file_20, enc_format);
    std::string dfwm_out_file_40  = dfwm_out_file_base + "40";
    vpxt_enc_format_append(dfwm_out_file_40, enc_format);
    std::string dfwm_out_file_60  = dfwm_out_file_base + "60";
    vpxt_enc_format_append(dfwm_out_file_60, enc_format);
    std::string dfwm_out_file_80  = dfwm_out_file_base + "80";
    vpxt_enc_format_append(dfwm_out_file_80, enc_format);
    std::string dfwm_out_file_100 = dfwm_out_file_base + "100";
    vpxt_enc_format_append(dfwm_out_file_100, enc_format);

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
            record_test_complete(file_index_str, file_index_output_char, test_type);
            return TEST_INDT;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = bitrate;
    opt.worst_allowed_q = 15;
    opt.allow_df = 1;
    opt.allow_spatial_resampling = 0;
    opt.end_usage = 0;

    int n = 100;
    int i = 0;
    long dfwm_arr[6];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        while (n >= 0)
        {
            ///////Update File Name///////
            char num[20];
            vpxt_itoa_custom(n, num, 10);
            std::string dfwm_out_file = dfwm_out_file_base;
            dfwm_out_file += num;
            vpxt_enc_format_append(dfwm_out_file, enc_format);

            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
            dfwm_arr[i] = vpxt_file_size(dfwm_out_file.c_str(), 1);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");

            i++;
            n = n - 20;

            dfwm_out_file.clear();
        }
    }
    else
    {
        while (n >= 0)
        {
            opt.drop_frames_water_mark = n;

            ///////Update File Name///////
            char num[20];
            vpxt_itoa_custom(n, num, 10);
            std::string dfwm_out_file = dfwm_out_file_base;
            dfwm_out_file += num;
            vpxt_enc_format_append(dfwm_out_file, enc_format);

            opt.Mode = mode;

            if (vpxt_compress(input.c_str(), dfwm_out_file.c_str(), speed,
                bitrate, opt, comp_out_str, n, 0, enc_format) == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return TEST_INDT;
            }

            if (test_type != 2)
            {
                tprintf(PRINT_STD, "\n");
                fprintf(stderr, "\n");
                dfwm_arr[i] = vpxt_display_visible_frames(dfwm_out_file.c_str(), 1);
                tprintf(PRINT_STD, "\n");
                fprintf(stderr, "\n");
            }

            i++;
            n = n - 20;
            dfwm_out_file.clear();
        }
    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
    }

    i = 0;
    n = 100;
    int equal_count = 0;
    int pass = 1;

    tprintf(PRINT_STD, "\n\n");
    fprintf(stderr, "\n\n");

    while (i < 6)
    {
        tprintf(PRINT_STD, "DFWM%4i Visible Frames: %4i\n", n, dfwm_arr[i]);
        fprintf(stderr, "DFWM%4i Visible Frames: %4i\n", n, dfwm_arr[i]);
        i++;
        n = n - 20;
    }

    n = 100;
    i = 0;

    tprintf(PRINT_STD, "\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    while (i < 5)
    {
        if (dfwm_arr[i+1] == dfwm_arr[i])
        {
            equal_count++;

            vpxt_formated_print(RESPRT, "DFWM%4i: %4i = DFWM%4i: %4i - "
                "Indeterminate", n - 20, dfwm_arr[i+1], n, dfwm_arr[i]);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
        }

        if (dfwm_arr[i+1] > dfwm_arr[i])
        {
            vpxt_formated_print(RESPRT, "DFWM%4i: %4i > DFWM%4i: %4i - "
                "Passed", n - 20, dfwm_arr[i+1], n, dfwm_arr[i]);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
        }

        if (dfwm_arr[i+1] < dfwm_arr[i])
        {
            pass = 0;

            vpxt_formated_print(RESPRT, "DFWM%4i: %4i < DFWM%4i: %4i - "
                "Failed", n - 20, dfwm_arr[i+1], n, dfwm_arr[i]);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
        }

        i++;
        n = n - 20;
    }

    if (pass == 0)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(6, dfwm_out_file_0.c_str(), dfwm_out_file_20.c_str(),
            dfwm_out_file_40.c_str(), dfwm_out_file_60.c_str(), dfwm_out_file_80.c_str(),
            dfwm_out_file_100.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }
    else
    {
        if (equal_count == 5)
        {
            tprintf(PRINT_BTH, "\n\nUnknown: Drop-Frames-Watermark has no "
                "effect, try different parameters \n");

            if (delete_ivf)
                vpxt_delete_files(6, dfwm_out_file_0.c_str(),
                dfwm_out_file_20.c_str(), dfwm_out_file_40.c_str(),
                dfwm_out_file_60.c_str(), dfwm_out_file_80.c_str(),
                dfwm_out_file_100.c_str());

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char, test_type);
            return TEST_INDT;
        }
        else
        {
            tprintf(PRINT_BTH, "\nPassed\n");

            if (delete_ivf)
                vpxt_delete_files(6, dfwm_out_file_0.c_str(),
                dfwm_out_file_20.c_str(), dfwm_out_file_40.c_str(),
                dfwm_out_file_60.c_str(), dfwm_out_file_80.c_str(),
                dfwm_out_file_100.c_str());

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char, test_type);
            return TEST_PASSED;
        }
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}