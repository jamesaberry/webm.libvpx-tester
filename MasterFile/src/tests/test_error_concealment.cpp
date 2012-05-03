#include "vpxt_test_declarations.h"

int test_error_concealment(int argc,
                           const char** argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf,
                           int artifact_detection)
{
    char *comp_out_str = "Error Resilient Mode";
    char *test_dir = "test_error_concealment";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];
    std::string dec_format = argv[6];

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

    std::string error_con_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression";
    vpxt_enc_format_append(error_con_enc, enc_format);

    std::string error_con_enc_wpfd;
    vpxt_remove_file_extension(error_con_enc.c_str(), error_con_enc_wpfd);
    error_con_enc_wpfd += "with_part_frame_drops.ivf";

    std::string error_con_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_decompression";
    vpxt_dec_format_append(error_con_dec, dec_format);

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

    opt.target_bandwidth = bitrate;

    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        opt.error_resilient_mode = 1;

        if (vpxt_compress(input.c_str(), error_con_enc.c_str(), speed, bitrate,
            opt, comp_out_str, opt.error_resilient_mode, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        if (vpxt_decompress_partial_drops(error_con_enc.c_str(),
            error_con_dec.c_str(), dec_format, 1, 3, 5, 2, PRINT_BTH, 1) == -1)
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

    tprintf(PRINT_BTH, "\n");

    double psnr_clean;
    double psnr_drops;

    psnr_clean = vpxt_psnr(input.c_str(), error_con_enc.c_str(), 0, PRINT_BTH, 1
        , 0, 0, 0, NULL, artifact_detection);
    psnr_drops = vpxt_psnr_dec(input.c_str(), error_con_dec.c_str(), 0,
        PRINT_BTH, 1, NULL, 0, 0);

    double psnr_perc = 100 * vpxt_abs_double((psnr_clean - psnr_drops) /
        psnr_drops);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (psnr_perc < 50.00)
    {
        vpxt_formated_print(RESPRT, "No Partial Drop PSNR: %.2f is within 50%% "
            "of Partial Drop PSNR: %.2f - %.2f%% - Passed", psnr_clean,
            psnr_drops, psnr_perc);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "No Partial Drop PSNR: %.2f is not within "
            "50%% of Partial Drop PSNR: %.2f - %.2f%% - Failed", psnr_clean,
            psnr_drops, psnr_perc);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (psnr_drops > 20.00)
    {
        vpxt_formated_print(RESPRT, "Partial Drop PSNR is greater than 20.00: "
            "%.2f - Passed", psnr_drops);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Partial Drop PSNR is not greater than "
            "20.00: %.2f - Failed", psnr_drops);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(3, error_con_enc.c_str(), error_con_dec.c_str(),
        error_con_enc_wpfd.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}