#include "vpxt_test_declarations.h"

//See vp8.h vp8_postproc_cfg description for valid ranges.
#define max_deblock_level 16
#define max_noise_level 16
#define test_post_processor_psnr_ar_size 2 + max_deblock_level + max_noise_level

int test_post_processor(int argc,
                        const char *const *argv,
                        const std::string &working_dir,
                        std::string files_ar[],
                        int test_type,
                        int delete_ivf)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_post_processor";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return 11;

    std::string post_proc_encode = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression";
    vpxt_enc_format_append(post_proc_encode, enc_format);

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

    vpxt_cap_string_print(PRINT_BTH, "%s\n", test_dir);

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
            return 2;
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

        if (vpxt_compress(input.c_str(), post_proc_encode.c_str(), speed,
            bitrate, opt, comp_out_str, opt.allow_df, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return 2;
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

    int n = 0;
    double ssim = 0;

    double psnr_arr[test_post_processor_psnr_ar_size];

    //No Filtering PSNR calculation
    int deblock_level = 0;
    int noise_level = 0;
    int flags = VP8_NOFILTERING;

    tprintf(PRINT_BTH, "\nCaculating PSNR: NOFILTERING DeblockLevel %i "
        "noise_level %i \n", deblock_level, noise_level);

    psnr_arr[n] = vpxt_post_proc_psnr(input.c_str(),
        post_proc_encode.c_str(), 0, PRINT_BTH, 1, deblock_level, 0, flags,
        &ssim);

    ++n;

    //Deblock PSNR calculation
    flags = VP8_DEBLOCK;

    tprintf(PRINT_BTH, "\nCaculating PSNR: DEBLOCK DeblockLevel %i noise_level "
        "%i \n", deblock_level, noise_level);

    psnr_arr[n] = vpxt_post_proc_psnr(input.c_str(),
        post_proc_encode.c_str(), 0, PRINT_BTH, 1, deblock_level, noise_level,
        flags, &ssim);

    ++n;

    //Demackroblock PSNR calculation
    flags = VP8_DEMACROBLOCK;

    while (deblock_level < max_deblock_level && n <
        test_post_processor_psnr_ar_size) {
        tprintf(PRINT_BTH, "\nCaculating PSNR: DEMACROBLOCK DeblockLevel %i "
            "noise_level %i \n", deblock_level, noise_level);

        psnr_arr[n] = vpxt_post_proc_psnr(input.c_str(),
            post_proc_encode.c_str(), 0, PRINT_BTH, 1, deblock_level, 0,
            flags, &ssim);

        ++deblock_level;
        ++n;
    }

    //Add noise PSNR calculation
    deblock_level = 0;
    noise_level = 0;
    flags = VP8_ADDNOISE;

    while (noise_level < max_noise_level && n <
        test_post_processor_psnr_ar_size) {
        tprintf(PRINT_BTH, "\nCaculating PSNR: ADDNOISE DeblockLevel %i "
            "noise_level %i \n", deblock_level, noise_level);

            psnr_arr[n] = vpxt_post_proc_psnr(input.c_str(),
            post_proc_encode.c_str(), 0, PRINT_BTH, 1, deblock_level,
            noise_level, flags, &ssim);

        ++noise_level;
        ++n;
    }

    tprintf(PRINT_BTH, "\n\n\nResults:\n\n");

    int test_fail = 0;
    int ten_percent = 0;
    n = 0;

    if (psnr_arr[0] != psnr_arr[1])
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f != NOFILTERING PSNR: "
            "%4.2f - Passed", psnr_arr[1], psnr_arr[0]);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f == NOFILTERING PSNR: "
            "%4.2f - Failed", psnr_arr[1], psnr_arr[0]);
        tprintf(PRINT_BTH, "\n");

        test_fail = 1;
    }

    tprintf(PRINT_BTH, "\n");

    n = 2;
    while (n < max_deblock_level + 2)
    {
        if (psnr_arr[0] != psnr_arr[n])
        {
            vpxt_formated_print(RESPRT, "DeblockLevel %*i PSNR: %4.2f != "
                "NOFILTERING PSNR: %4.2f - Passed", 2, n - 2, psnr_arr[n],
                psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DeblockLevel %*i PSNR: %4.2f += "
                "NOFILTERING PSNR: %4.2f - Failed", 2, n - 2, psnr_arr[n],
                psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");

            test_fail = 1;
        }

        ++n;;
    }

    tprintf(PRINT_BTH, "\n");

    while (n < max_noise_level + max_deblock_level + 2)
    {
        if (psnr_arr[0] != psnr_arr[n])
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f != "
                "NOFILTERING PSNR: %4.2f - Passed", 2, n - 18, psnr_arr[n],
                psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f == "
                "NOFILTERING PSNR: %4.2f - Failed", 2, n - 18, psnr_arr[n],
                psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");

            test_fail = 1;
        }

        ++n;;
    }

    tprintf(PRINT_BTH, "\n");

    float psnr_percent = vpxt_abs_float(psnr_arr[1] - psnr_arr[0]) / psnr_arr[0];

    if (psnr_percent < 0.1)
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f within 10%% of %4.2f "
            "- Passed", psnr_arr[1], psnr_arr[0]);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f not within 10%% of "
            "%4.2f - Failed", psnr_arr[1], psnr_arr[0]);
        tprintf(PRINT_BTH, "\n");

        ten_percent = 1;
    }

    tprintf(PRINT_BTH, "\n");

    n = 2;

    while (n < max_deblock_level + 2)
    {
        psnr_percent = vpxt_abs_float(psnr_arr[n] - psnr_arr[0]) / psnr_arr[0];

        if (psnr_percent < 0.1)
        {
            vpxt_formated_print(RESPRT, "DeblockLevel %*i PSNR: %4.2f within "
                "10%% of %4.2f - Passed", 2, n - 2, psnr_arr[n], psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DeblockLvl %*i PSNR: %4.2f not within "
                "10%% of %4.2f - Failed", 2, n - 2, psnr_arr[n], psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");

            ten_percent = 1;
        }

        ++n;;
    }

    tprintf(PRINT_BTH, "\n");

    while (n < max_noise_level + max_deblock_level + 2)
    {
        psnr_percent = vpxt_abs_float(psnr_arr[n] - psnr_arr[0]) / psnr_arr[0];

        if (psnr_percent < 0.1)
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f within "
                "10%% of %4.2f - Passed", 2, n - 18, psnr_arr[n], psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f not "
                "within 10%% of %4.2f - Indeterminate", 2, n - 18, psnr_arr[n],
                psnr_arr[0]);
            tprintf(PRINT_BTH, "\n");

            ten_percent = 1;
        }

        ++n;;
    }

    if (delete_ivf)
        vpxt_delete_files(1, post_proc_encode.c_str());

    if (test_fail == 0)
    {
        if (ten_percent == 0)
        {
            tprintf(PRINT_BTH, "\nPassed\n");

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return 1;
        }
        else
        {
            tprintf(PRINT_BTH, "\nMin Passed\n");

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return 8;
        }
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 0;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return 6;
}