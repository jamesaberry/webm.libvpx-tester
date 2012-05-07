#include "vpxt_test_declarations.h"

int test_encoder_break_out(int argc,
                           const char** argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf,
                           int artifact_detection)
{
    char *comp_out_str = "Encoder Break Out";
    char *test_dir = "test_encoder_break_out";
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

    int enc_break_out_0_art_det = artifact_detection;
    int enc_break_out_100_art_det = artifact_detection;
    int enc_break_out_500_art_det = artifact_detection;
    int enc_break_out_1000_art_det = artifact_detection;

    std::string enc_break_out_0 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_0";
    vpxt_enc_format_append(enc_break_out_0, enc_format);

    std::string enc_break_out_100 = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_100";
    vpxt_enc_format_append(enc_break_out_100, enc_format);

    std::string enc_break_out_500 = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression_500";
    vpxt_enc_format_append(enc_break_out_500, enc_format);

    std::string enc_break_out_1000 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_1000";
    vpxt_enc_format_append(enc_break_out_1000, enc_format);

    std::string enc_break_out_0_dec = cur_test_dir_str + slashCharStr() +
        test_dir + "_decompression_0";
    vpxt_dec_format_append(enc_break_out_0_dec, dec_format);

    std::string enc_break_out_100_dec = cur_test_dir_str + slashCharStr() +
        test_dir + "_decompression_100";
    vpxt_dec_format_append(enc_break_out_100_dec, dec_format);

    std::string enc_break_out_500_dec = cur_test_dir_str + slashCharStr() +
        test_dir + "_decompression_500";
    vpxt_dec_format_append(enc_break_out_500_dec, dec_format);

    std::string enc_break_out_1000_dec = cur_test_dir_str + slashCharStr() +
        test_dir + "_decompression_1000";
    vpxt_dec_format_append(enc_break_out_1000_dec, dec_format);

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

        opt.encode_breakout = 0;

        if (vpxt_compress(input.c_str(), enc_break_out_0.c_str(), speed, bitrate
            , opt, comp_out_str, 0, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.encode_breakout = 100;

        if (vpxt_compress(input.c_str(), enc_break_out_100.c_str(), speed,
            bitrate, opt, comp_out_str, 100, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.encode_breakout = 500;

        if (vpxt_compress(input.c_str(), enc_break_out_500.c_str(), speed,
            bitrate,  opt, comp_out_str, 500, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.encode_breakout = 1000;

        if (vpxt_compress(input.c_str(), enc_break_out_1000.c_str(), speed,
            bitrate, opt, comp_out_str, 1000, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_BTH, "\nDecoding enc_break_out_0");

        if (vpxt_decompress(enc_break_out_0.c_str(), enc_break_out_0_dec.c_str()
            , dec_format, 1) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_BTH, "\n\nDecoding enc_break_out_100");

        if (vpxt_decompress(enc_break_out_100.c_str(),
            enc_break_out_100_dec.c_str(),
            dec_format, 1) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_BTH, "\n\nDecoding enc_break_out_500");

        if (vpxt_decompress(enc_break_out_500.c_str(),
            enc_break_out_500_dec.c_str(), dec_format, 1) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_BTH, "\n\nDecoding enc_break_out_1000");

        if (vpxt_decompress(enc_break_out_1000.c_str(),
            enc_break_out_1000_dec.c_str(), dec_format, 1) == -1)
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

    tprintf(PRINT_BTH, "\n\n");
    long source_file_size = vpxt_raw_file_size(input.c_str());

    tprintf(PRINT_BTH, "\n");
    long enc_break_out_0_dec_file_size =
        vpxt_raw_file_size(enc_break_out_0_dec.c_str());
    tprintf(PRINT_BTH, "\n");
    long enc_break_out_100_dec_file_size =
        vpxt_raw_file_size(enc_break_out_100_dec.c_str());
    tprintf(PRINT_BTH, "\n");
    long enc_break_out_500_dec_file_size =
        vpxt_raw_file_size(enc_break_out_500_dec.c_str());
    tprintf(PRINT_BTH, "\n");
    long enc_break_out_1000_dec_file_size =
        vpxt_raw_file_size(enc_break_out_1000_dec.c_str());
    tprintf(PRINT_BTH, "\n");

    double psnr_0 = vpxt_psnr(input.c_str(), enc_break_out_0.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, enc_break_out_0_art_det);
    double psnr_100 = vpxt_psnr(input.c_str(), enc_break_out_100.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, enc_break_out_100_art_det);
    double psnr_500 = vpxt_psnr(input.c_str(), enc_break_out_500.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, enc_break_out_500_art_det);
    double psnr_1000 = vpxt_psnr(input.c_str(), enc_break_out_1000.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, enc_break_out_1000_art_det);

    double dB1 = vpxt_abs_double(psnr_0 - psnr_100);
    double dB2 = vpxt_abs_double(psnr_100 - psnr_500);
    double dB3 = vpxt_abs_double(psnr_500 - psnr_1000);

    tprintf(PRINT_BTH, "\n");
    tprintf(PRINT_BTH, "psnr_0   : %f \n", psnr_0);
    tprintf(PRINT_BTH, "psnr_100 : %f \n", psnr_100);
    tprintf(PRINT_BTH, "psnr_500 : %f \n", psnr_500);
    tprintf(PRINT_BTH, "psnr_1000: %f \n", psnr_1000);

    tprintf(PRINT_BTH, "\n");
    tprintf(PRINT_BTH, "Raw Files Size Source  : %i \n", source_file_size);
    tprintf(PRINT_BTH, "Raw Files Size psnr_0   : %i \n",
        enc_break_out_0_dec_file_size);
    tprintf(PRINT_BTH, "Raw Files Size psnr_100 : %i \n",
        enc_break_out_100_dec_file_size);
    tprintf(PRINT_BTH, "Raw Files Size psnr_500 : %i \n",
        enc_break_out_500_dec_file_size);
    tprintf(PRINT_BTH, "Raw Files Size psnr_1000: %i \n",
        enc_break_out_1000_dec_file_size);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int indt_count = 0;

    if (source_file_size != enc_break_out_0_dec_file_size)
    {
        vpxt_formated_print(RESPRT,"enc_break_out_0 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "enc_break_out_0 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (source_file_size != enc_break_out_100_dec_file_size)
    {
        vpxt_formated_print(RESPRT,
            "enc_break_out_100 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "enc_break_out_100 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (source_file_size != enc_break_out_500_dec_file_size)
    {
        vpxt_formated_print(RESPRT,
            "enc_break_out_500 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "enc_break_out_500 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (source_file_size != enc_break_out_1000_dec_file_size)
    {
        vpxt_formated_print(RESPRT, "enc_break_out_1000 was not properly "
            "decoded.");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "enc_break_out_1000 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (dB1 <= 2)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 0 PSNR and 100 PSNR are "
            "within 2 dB - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (dB1 >= 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 0 PSNR and 100 PSNR not "
            "within 5 dB - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (dB1 > 2 && dB1 < 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 0 PSNR and 100 PSNR "
            "greater than 2 dB but less than 5 dB - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        indt_count++;
    }

    if (dB2 <= 2)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 100 PSNR and 500 PSNR "
            "are within 2 dB - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (dB2 >= 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 100 PSNR and 500 PSNR "
            "not within 5 dB - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (dB2 > 2 && dB2 < 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 100 PSNR and 500 PSNR "
            "greater than 2 dB but less than 5 dB - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        indt_count++;
    }

    if (dB3 <= 2)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 500 PSNR and 1000 PSNR "
            "are within 2 dB - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (dB3 >= 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 500 PSNR and 1000 PSNR "
            "not within 5 dB - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (dB3 > 2 && dB3 < 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 500 PSNR and 1000 PSNR "
            "greater than 2 dB but less than 5 dB: - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        indt_count++;
    }

    if (indt_count != 0)
        test_state = kTestIndeterminate;

    // handle possible artifact
    if(enc_break_out_0_art_det == kPossibleArtifactFound ||
        enc_break_out_100_art_det == kPossibleArtifactFound ||
        enc_break_out_500_art_det == kPossibleArtifactFound ||
        enc_break_out_1000_art_det == kPossibleArtifactFound)
    {
        tprintf(PRINT_BTH, "\nPossible Artifact\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPossibleArtifact;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestIndeterminate)
        tprintf(PRINT_BTH, "\nIndeterminate\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(8, enc_break_out_0.c_str(),
        enc_break_out_100.c_str(), enc_break_out_500.c_str(),
        enc_break_out_1000.c_str(), enc_break_out_0_dec.c_str(),
        enc_break_out_100_dec.c_str(), enc_break_out_500_dec.c_str(),
        enc_break_out_1000_dec.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}