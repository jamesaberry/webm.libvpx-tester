#include "vpxt_test_declarations.h"

// See vp8.h vp8_postproc_cfg description for valid ranges.
#define max_deblock_level 16
#define max_noise_level 16
#define test_post_processor_psnr_ar_size 2 + max_deblock_level + max_noise_level

int test_post_processor(int argc,
                        const char** argv,
                        const std::string &working_dir,
                        const std::string sub_folder_str,
                        int test_type,
                        int delete_ivf,
                        int artifact_detection)
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

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    int post_proc_encode_art_det = artifact_detection;
    int post_proc_decode_art_det = kDontRunArtifactDetection;

    std::string post_proc_encode = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression";
    vpxt_enc_format_append(post_proc_encode, enc_format);

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

        if (vpxt_compress(input.c_str(), post_proc_encode.c_str(), speed,
            bitrate, opt, comp_out_str, opt.allow_df, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }
    }

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        // Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    int n = 0;
    double ssim = 0;

    double psnr_arr[test_post_processor_psnr_ar_size];

    // No Filtering PSNR calculation
    int deblock_level = 0;
    int noise_level = 0;
    int flags = VP8_NOFILTERING;

    tprintf(PRINT_BTH, "\nCaculating PSNR: NOFILTERING DeblockLevel %i "
        "noise_level %i \n", deblock_level, noise_level);

    psnr_arr[n] = vpxt_psnr(input.c_str(), post_proc_encode.c_str(), 0,
        PRINT_BTH, 1, deblock_level, 0, flags, &ssim, post_proc_encode_art_det);

    ++n;

    // Deblock PSNR calculation
    flags = VP8_DEBLOCK;

    tprintf(PRINT_BTH, "\nCaculating PSNR: DEBLOCK DeblockLevel %i noise_level "
        "%i \n", deblock_level, noise_level);

    psnr_arr[n] = vpxt_psnr(input.c_str(), post_proc_encode.c_str(), 0,
        PRINT_BTH, 1, deblock_level, noise_level, flags, &ssim,
        post_proc_decode_art_det);

    ++n;

    // Demackroblock PSNR calculation
    flags = VP8_DEMACROBLOCK;

    while (deblock_level < max_deblock_level && n <
        test_post_processor_psnr_ar_size) {
        tprintf(PRINT_BTH, "\nCaculating PSNR: DEMACROBLOCK DeblockLevel %i "
            "noise_level %i \n", deblock_level, noise_level);

        psnr_arr[n] = vpxt_psnr(input.c_str(), post_proc_encode.c_str(), 0,
            PRINT_BTH, 1, deblock_level, 0, flags, &ssim,
            post_proc_decode_art_det);

        ++deblock_level;
        ++n;
    }

    // Add noise PSNR calculation
    deblock_level = 0;
    noise_level = 0;
    flags = VP8_ADDNOISE;

    while (noise_level < max_noise_level && n <
        test_post_processor_psnr_ar_size) {
        tprintf(PRINT_BTH, "\nCaculating PSNR: ADDNOISE DeblockLevel %i "
            "noise_level %i \n", deblock_level, noise_level);

            psnr_arr[n] = vpxt_psnr(input.c_str(), post_proc_encode.c_str(), 0,
                PRINT_BTH, 1, deblock_level, noise_level, flags, &ssim,
                post_proc_decode_art_det);

        ++noise_level;
        ++n;
    }

    tprintf(PRINT_BTH, "\n\n\nResults:\n\n");

    int test_state = kTestPassed;
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

        test_state = kTestFailed;
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

            test_state = kTestFailed;
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

            test_state = kTestFailed;
        }

        ++n;;
    }

    tprintf(PRINT_BTH, "\n");

    double psnr_percent = vpxt_abs_double(psnr_arr[1] - psnr_arr[0]) / psnr_arr[0];

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
        psnr_percent = vpxt_abs_double(psnr_arr[n] - psnr_arr[0]) / psnr_arr[0];

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
        psnr_percent = vpxt_abs_double(psnr_arr[n] - psnr_arr[0]) / psnr_arr[0];

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

    if (ten_percent != 0)
        test_state = kTestMinPassed;

    // handle possible artifact
    if(post_proc_encode_art_det == kPossibleArtifactFound)
    {
        tprintf(PRINT_BTH, "\nPossible Artifact\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPossibleArtifact;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestMinPassed)
        tprintf(PRINT_BTH, "\nMin Passed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(1, post_proc_encode.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}