#include "vpxt_test_declarations.h"

int test_post_processor_mfqe(int argc,
                            const char** argv,
                            const std::string &working_dir,
                            const std::string sub_folder_str,
                            int test_type,
                            int delete_ivf)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_post_processor_mfqe";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int key_freq = atoi(argv[5]);
    int max_intra_bitrate_pct = atoi(argv[6]);
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

    std::string post_proc_encode = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression";
    vpxt_enc_format_append(post_proc_encode, enc_format);

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

    vpxt_cap_string_print(PRINT_BTH, "%s\n", test_dir);

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
        opt.key_freq = key_freq;
        opt.rc_max_intra_bitrate_pct = max_intra_bitrate_pct;

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

    double ssim = 0;
    double psnr_arr_no_filter = 0;
    double psnr_arr_mfqe_filter = 0;

    // No Filtering PSNR calculation
    int deblock_level = 0;
    int noise_level = 0;
    int flags = VP8_NOFILTERING;
    psnr_arr_no_filter = vpxt_psnr(input.c_str(), post_proc_encode.c_str(), 0,
        PRINT_BTH, 1, &ssim);

    // MFQE PSNR calculation
    flags = VP8_MFQE;
    psnr_arr_mfqe_filter = vpxt_post_proc_psnr(input.c_str(),
        post_proc_encode.c_str(), 0, PRINT_BTH, 1, deblock_level, 0, flags,
        &ssim);

    tprintf(PRINT_BTH, "\n\n\nResults:\n\n");

    int fail = 0;

    if (psnr_arr_mfqe_filter > psnr_arr_no_filter)
    {
    if (psnr_arr_mfqe_filter > psnr_arr_no_filter)
        vpxt_formated_print(RESPRT, "MFQE PSNR: %.2f > Raw PSNR: %.2f - Passed",
            psnr_arr_mfqe_filter, psnr_arr_no_filter);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "MFQE PSNR: %.2f <= Raw PSNR: %.2f - Failed"
            ,psnr_arr_mfqe_filter, psnr_arr_no_filter);
        tprintf(PRINT_BTH, "\n");

        fail = 1;
    }

    if (delete_ivf)
        vpxt_delete_files(1, post_proc_encode.c_str());

    if (!fail)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}