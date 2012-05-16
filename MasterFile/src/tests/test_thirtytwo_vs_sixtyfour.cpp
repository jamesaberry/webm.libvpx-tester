#include "vpxt_test_declarations.h"

int test_thirtytwo_vs_sixtyfour(int argc,
                                const char** argv,
                                const std::string &working_dir,
                                const std::string sub_folder_str,
                                int test_type,
                                int delete_ivf,
                                int artifact_detection)
{
    char *comp_out_str = "ThirtyTwoVsSixtyFour";
    char *test_dir = "test_thirtytwo_vs_sixtyfour";
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

    base_folder += slashCharStr();
    std::string version_string = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    size_t version_pos = version_string.find("v");
    base_folder += version_string.substr(version_pos) + "-32v64" + slashCharStr() +
        "Mode" + argv[3] + slashCharStr();
    vpxt_make_dir(base_folder);

    int test_mode = 0;
    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string file_to_enc;
    std::string file_to_dec;

    std::string enc_32 = base_folder + test_dir + "_compression_thirty_two";
    vpxt_enc_format_append(enc_32, enc_format);

    std::string enc_64 = base_folder + test_dir + "_compression_sixty_four";
    vpxt_enc_format_append(enc_64, enc_format);

    std::string dec_32 = base_folder + test_dir + "_decompression_thirty_two";
    vpxt_dec_format_append(dec_32, dec_format);

    std::string dec_64 = base_folder + test_dir + "_decompression_sixty_four";
    vpxt_dec_format_append(dec_64, dec_format);

#if ARCH_X86
    file_to_enc = enc_32;
    file_to_dec = dec_32;

    if (vpxt_file_exists_check(enc_64) && vpxt_file_exists_check(dec_64))
        test_mode = 1;

#else if ARCH_X86_64
    file_to_enc = enc_64;
    file_to_dec = dec_64;

    if (vpxt_file_exists_check(enc_32) && vpxt_file_exists_check(dec_32))
        test_mode = 1;
#endif

    ///////////// Open Output File and Print Header ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;
    FILE *fp;

    vpxt_open_output_file(test_type, text_file_str, fp);
    vpxt_print_header(argc, argv, main_test_dir_char, cur_test_dir_str,
        test_dir, test_type);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);
    /////////////////// Use Custom Settings ///////////////////

    bitrate = opt.target_bandwidth;

    if (test_type == kTestOnly)
    {

    }
    else
    {
        opt.Mode = mode;
        vpxt_determinate_parameters(opt);

        if (vpxt_compress(input.c_str(), file_to_enc.c_str(), speed, bitrate,
            opt, "Mode", mode, 0, enc_format, kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_STD, "\n");
        fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");

        if (vpxt_decompress(file_to_enc.c_str(), file_to_dec.c_str(), dec_format
            , 1) == -1)
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

    if (test_mode == kRealTime)
    {
        tprintf(PRINT_BTH, "\n\nResults:\n\n");
        vpxt_formated_print(RESPRT, "Test files created.");
        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
    }

    int enc_fail = 0;
    int dec_fail = 0;

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", enc_32.c_str(),
        enc_64.c_str());
    int enc_32_vs_enc_64 = vpxt_compare_enc(enc_32.c_str(), enc_64.c_str(), 0);

    if (enc_32_vs_enc_64 == -1)
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");

    if (enc_32_vs_enc_64 >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i",
            enc_32_vs_enc_64);
        enc_fail++;
    }

    if (enc_32_vs_enc_64 == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        enc_fail++;
    }

    if (enc_32_vs_enc_64 == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        enc_fail++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", dec_32.c_str(),
        dec_64.c_str());
    int dec_32_vs_dec_64 = vpxt_compare_dec(dec_32.c_str(), dec_64.c_str());

    if (dec_32_vs_dec_64 == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (dec_32_vs_dec_64 >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i",
            dec_32_vs_dec_64);
        dec_fail++;
    }

    if (dec_32_vs_dec_64 == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        dec_fail++;
    }

    if (dec_32_vs_dec_64 == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        dec_fail++;
    }

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

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
        test_state = kTestFailed;
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
        test_state = kTestFailed;
    }

    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
