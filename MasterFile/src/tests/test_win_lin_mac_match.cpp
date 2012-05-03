#include "vpxt_test_declarations.h"

int test_win_lin_mac_match(int argc,
                           const char *const *argv,
                           const std::string &working_dir,
                           std::string files_ar[],
                           int test_type,
                           int delete_ivf)
{
    char *comp_out_str = "WinLinMacMatch";
    char *test_dir = "test_win_lin_mac_match";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string base_folder = argv[5];
    std::string parameter_file = argv[6];
    std::string enc_format = argv[7];
    std::string dec_format = argv[8];

    int speed = 0;

    std::string arch = "Unknown";
#if ARCH_X86
    arch = "-32Bit";
#elif ARCH_X86_64
    arch = "-64Bit";
#elif HAVE_ARMV6
    arch = "-Arm6";
#elif HAVE_ARMV7
    arch = "-Arm7";
#endif

    base_folder += slashCharStr();
    std::string version_string = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    size_t version_pos = version_string.find("v");
    version_string.substr(version_pos);
    base_folder  += version_string.substr(version_pos) + arch + slashCharStr() +
        "Mode" + argv[3] + slashCharStr();
    vpxt_make_dir(base_folder);
    int test_mode = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return TEST_ERRFM;

    std::string file_to_enc;
    std::string file_to_dec;

    std::string win_enc = base_folder + test_dir + "_compression_win";
    vpxt_enc_format_append(win_enc, enc_format);

    std::string lin_enc = base_folder + test_dir + "_compression_lin";
    vpxt_enc_format_append(lin_enc, enc_format);

    std::string mac_enc = base_folder + test_dir + "_compression_mac";
    vpxt_enc_format_append(mac_enc, enc_format);

    std::string win_dec = base_folder + test_dir + "_decompression_win";
    vpxt_dec_format_append(win_dec, dec_format);

    std::string lin_dec = base_folder + test_dir + "_decompression_lin";
    vpxt_dec_format_append(lin_dec, dec_format);

    std::string mac_dec = base_folder + test_dir + "_decompression_mac";
    vpxt_dec_format_append(mac_dec, dec_format);

#if defined(linux)
    file_to_enc = lin_enc;
    file_to_dec = lin_dec;

    if (vpxt_file_exists_check(win_enc) && vpxt_file_exists_check(win_dec) &&
        vpxt_file_exists_check(mac_enc) && vpxt_file_exists_check(mac_dec))
    {
        test_mode = 1;
    }

#elif defined(_WIN32)
    file_to_enc = win_enc;
    file_to_dec = win_dec;

    if (vpxt_file_exists_check(lin_enc) && vpxt_file_exists_check(lin_dec) &&
        vpxt_file_exists_check(mac_enc) && vpxt_file_exists_check(mac_dec))
    {
        test_mode = 1;
    }

#elif defined(__APPLE__)
    file_to_enc = mac_enc;
    file_to_dec = mac_dec;

    if (vpxt_file_exists_check(win_enc) && vpxt_file_exists_check(win_dec) &&
        vpxt_file_exists_check(lin_enc) && vpxt_file_exists_check(lin_dec))
    {
        test_mode = 1;
    }

#endif

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

    bitrate = opt.target_bandwidth;

    if (test_type == TEST_ONLY)
    {

    }
    else
    {
        opt.Mode = mode;

        if (vpxt_compress(input.c_str(), file_to_enc.c_str(), speed, bitrate,
            opt, "Mode", mode, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        tprintf(PRINT_STD, "\n");
        fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");

        if (vpxt_decompress(file_to_enc.c_str(), file_to_dec.c_str(),
            dec_format, 1) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
    }

    if (test_mode == 0)
    {
        tprintf(PRINT_BTH, "\n\nResults:\n\n");

        vpxt_formated_print(RESPRT, "Test files created.");

        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_INDT;
    }

    int enc_fail = 0;
    int dec_fail = 0;

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", win_enc.c_str(),
        lin_enc.c_str());
    int win_enc_vs_lin_enc = vpxt_compare_enc(win_enc.c_str(), lin_enc.c_str(),
        0);

    if (win_enc_vs_lin_enc == -1)
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");

    if (win_enc_vs_lin_enc >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i",
            win_enc_vs_lin_enc);
        enc_fail++;
    }

    if (win_enc_vs_lin_enc == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        enc_fail++;
    }

    if (win_enc_vs_lin_enc == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        enc_fail++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", lin_enc.c_str(),
        mac_enc.c_str());
    int lin_enc_vs_mac_enc = vpxt_compare_enc(lin_enc.c_str(), mac_enc.c_str(),
        0);

    if (lin_enc_vs_mac_enc == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (lin_enc_vs_mac_enc >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i",
            lin_enc_vs_mac_enc);
        enc_fail++;
    }

    if (lin_enc_vs_mac_enc == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        enc_fail++;
    }

    if (lin_enc_vs_mac_enc == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        enc_fail++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", win_enc.c_str(),
        mac_enc.c_str());
    int win_enc_vs_mac_enc = vpxt_compare_enc(win_enc.c_str(), mac_enc.c_str(),
        0);

    if (win_enc_vs_mac_enc == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (win_enc_vs_mac_enc >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i",
            win_enc_vs_mac_enc);
        enc_fail++;
    }

    if (win_enc_vs_mac_enc == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        enc_fail++;
    }

    if (win_enc_vs_mac_enc == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        enc_fail++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", win_dec.c_str(),
        lin_dec.c_str());
    int win_dec_vs_lin_dec = vpxt_compare_enc(win_dec.c_str(), lin_dec.c_str(),
        0);

    if (win_dec_vs_lin_dec == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (win_dec_vs_lin_dec >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i",
            win_dec_vs_lin_dec);
        dec_fail++;
    }

    if (win_dec_vs_lin_dec == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        dec_fail++;
    }

    if (win_dec_vs_lin_dec == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        dec_fail++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", lin_dec.c_str(),
        mac_dec.c_str());
    int lin_dec_vs_mac_dec = vpxt_compare_enc(lin_dec.c_str(), mac_dec.c_str(),
        0);

    if (lin_dec_vs_mac_dec == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (lin_dec_vs_mac_dec >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i",
            lin_dec_vs_mac_dec);
        dec_fail++;
    }

    if (lin_dec_vs_mac_dec == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        dec_fail++;
    }

    if (lin_dec_vs_mac_dec == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        dec_fail++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", win_dec.c_str(),
        mac_dec.c_str());
    int win_dec_vs_mac_dec = vpxt_compare_enc(win_dec.c_str(), mac_dec.c_str(),
        0);

    if (win_dec_vs_mac_dec == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (win_dec_vs_mac_dec >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i",
            win_dec_vs_mac_dec);
        dec_fail++;
    }

    if (win_dec_vs_mac_dec == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        dec_fail++;
    }

    if (win_dec_vs_mac_dec == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        dec_fail++;
    }

    tprintf(PRINT_BTH, "\n\nResults:\n\n");
    int fail = 0;

    if (enc_fail == 0)
    {
        vpxt_formated_print(RESPRT, "All encoded files are identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (enc_fail > 0)
    {
        vpxt_formated_print(RESPRT, "Not all encoded files are identical - "
            "Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (dec_fail == 0)
    {
        vpxt_formated_print(RESPRT, "All decoded files are identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (dec_fail > 0)
    {
        vpxt_formated_print(RESPRT, "Not all decoded files are identical - "
            "Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }
    else
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}
