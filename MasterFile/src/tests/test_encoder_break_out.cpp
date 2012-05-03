#include "vpxt_test_declarations.h"

int test_encoder_break_out(int argc,
                           const char *const *argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf)
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

    ////////////Formatting Test Specific directory////////////
    std::string cur_test_dir_str;

    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return TEST_ERRFM;

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

        opt.encode_breakout = 0;

        if (vpxt_compress(input.c_str(), enc_break_out_0.c_str(), speed, bitrate
            , opt, comp_out_str, 0, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.encode_breakout = 100;

        if (vpxt_compress(input.c_str(), enc_break_out_100.c_str(), speed,
            bitrate, opt, comp_out_str, 100, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.encode_breakout = 500;

        if (vpxt_compress(input.c_str(), enc_break_out_500.c_str(), speed,
            bitrate,  opt, comp_out_str, 500, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.encode_breakout = 1000;

        if (vpxt_compress(input.c_str(), enc_break_out_1000.c_str(), speed,
            bitrate, opt, comp_out_str, 1000, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        tprintf(PRINT_BTH, "\nDecoding enc_break_out_0");

        if (vpxt_decompress(enc_break_out_0.c_str(), enc_break_out_0_dec.c_str()
            , dec_format, 1) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        tprintf(PRINT_BTH, "\n\nDecoding enc_break_out_100");

        if (vpxt_decompress(enc_break_out_100.c_str(),
            enc_break_out_100_dec.c_str(),
            dec_format, 1) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        tprintf(PRINT_BTH, "\n\nDecoding enc_break_out_500");

        if (vpxt_decompress(enc_break_out_500.c_str(),
            enc_break_out_500_dec.c_str(), dec_format, 1) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        tprintf(PRINT_BTH, "\n\nDecoding enc_break_out_1000");

        if (vpxt_decompress(enc_break_out_1000.c_str(),
            enc_break_out_1000_dec.c_str(), dec_format, 1) == -1)
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
        PRINT_BTH, 1, NULL);
    double psnr_100 = vpxt_psnr(input.c_str(), enc_break_out_100.c_str(), 0,
        PRINT_BTH, 1, NULL);
    double psnr_500 = vpxt_psnr(input.c_str(), enc_break_out_500.c_str(), 0,
        PRINT_BTH, 1, NULL);
    double psnr_1000 = vpxt_psnr(input.c_str(), enc_break_out_1000.c_str(), 0,
        PRINT_BTH, 1, NULL);

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

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int pass = 1;
    int indt_count = 0;

    if (source_file_size != enc_break_out_0_dec_file_size)
    {
        vpxt_formated_print(RESPRT,"enc_break_out_0 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        pass = 0;
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
        pass = 0;
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
        pass = 0;
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
        pass = 0;
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
        pass = 0;
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
        pass = 0;
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
        pass = 0;
    }

    if (dB3 > 2 && dB3 < 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 500 PSNR and 1000 PSNR "
            "greater than 2 dB but less than 5 dB: - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        indt_count++;
    }

    if (pass == 1)
    {
        if (indt_count == 0)
        {
            tprintf(PRINT_BTH, "\nPassed\n");

            if (delete_ivf)
                vpxt_delete_files(8, enc_break_out_0.c_str(),
                enc_break_out_100.c_str(), enc_break_out_500.c_str(),
                enc_break_out_1000.c_str(), enc_break_out_0_dec.c_str(),
                enc_break_out_100_dec.c_str(), enc_break_out_500_dec.c_str(),
                enc_break_out_1000_dec.c_str());


            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_PASSED;
        }
        else
        {
            tprintf(PRINT_BTH, "\nIndeterminate\n");

            if (delete_ivf)
                vpxt_delete_files(8, enc_break_out_0.c_str(),
                enc_break_out_100.c_str(), enc_break_out_500.c_str(),
                enc_break_out_1000.c_str(), enc_break_out_0_dec.c_str(),
                enc_break_out_100_dec.c_str(), enc_break_out_500_dec.c_str(),
                enc_break_out_1000_dec.c_str());


            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(8, enc_break_out_0.c_str(),
            enc_break_out_100.c_str(), enc_break_out_500.c_str(),
            enc_break_out_1000.c_str(), enc_break_out_0_dec.c_str(),
            enc_break_out_100_dec.c_str(), enc_break_out_500_dec.c_str(),
            enc_break_out_1000_dec.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}