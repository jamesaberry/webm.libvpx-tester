#include "vpxt_test_declarations.h"

int test_post_processor_mfqe(int argc,
                            const char** argv,
                            const std::string &working_dir,
                            const std::string sub_folder_str,
                            int test_type,
                            int delete_ivf,
                            int artifact_detection)
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

    int post_proc_encode_no_mfqe_art_det = artifact_detection;
    int post_proc_encode_mfqe_art_det = artifact_detection;

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
        opt.key_freq = key_freq;
        opt.rc_max_intra_bitrate_pct = max_intra_bitrate_pct;

        if (vpxt_compress(input.c_str(), post_proc_encode.c_str(), speed,
            bitrate, opt, comp_out_str, opt.allow_df, 0, enc_format,
            kSetConfigOff) == -1)
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
        PRINT_BTH, 1, 0, 0, 0, &ssim, post_proc_encode_no_mfqe_art_det);

    // MFQE PSNR calculation
    flags = VP8_MFQE;
    psnr_arr_mfqe_filter = vpxt_psnr(input.c_str(), post_proc_encode.c_str(), 0,
        PRINT_BTH, 1, deblock_level, 0, flags, &ssim,
        post_proc_encode_mfqe_art_det);

    tprintf(PRINT_BTH, "\n\n\nResults:\n\n");

    int test_state = kTestPassed;

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

        test_state = kTestFailed;
    }

    // handle possible artifact
    if(post_proc_encode_no_mfqe_art_det == kPossibleArtifactFound ||
        post_proc_encode_mfqe_art_det == kPossibleArtifactFound)
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
        vpxt_delete_files(1, post_proc_encode.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}