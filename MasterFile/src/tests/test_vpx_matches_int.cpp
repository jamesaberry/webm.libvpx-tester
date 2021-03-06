#include "vpxt_test_declarations.h"

int test_vpx_matches_int(int argc,
                         const char** argv,
                         const std::string &working_dir,
                         const std::string sub_folder_str,
                         int test_type,
                         int delete_ivf,
                         int artifact_detection)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_vpx_matches_int";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string vpxencPath = argv[5];
    std::string vpxdecPath = argv[6];
    std::string enc_format = argv[7];
    std::string dec_format = argv[8];

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

    std::string internal_comp = cur_test_dir_str + slashCharStr() + test_dir +
        "_internal_compression";
    vpxt_enc_format_append(internal_comp, enc_format);

    std::string vpxenc_comp = cur_test_dir_str + slashCharStr() + test_dir +
        "_vpxenc_compression";
    vpxt_enc_format_append(vpxenc_comp, enc_format);

    std::string internal_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_internal_decompression";
    vpxt_dec_format_append(internal_dec, dec_format);

    std::string vpxdec_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_vpxenc_decompression";
    vpxt_dec_format_append(vpxdec_dec, dec_format);

    std::string vpxenc_cmd_str = vpxencPath + " " + input + " -o " +
        vpxenc_comp + " ";

    std::string vpxdec_cmd_str = vpxdecPath + " " + vpxenc_comp + " -o " +
        vpxdec_dec;

#if !defined(_WIN32)
    vpxenc_cmd_str.insert(0, "./");
    vpxdec_cmd_str.insert(0, "./");
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
    if(vpxt_use_custom_settings(argv, argc, input_ver, fp, file_index_str,
        file_index_output_char, test_type, opt, bitrate)
        == kTestIndeterminate)
        return kTestIndeterminate;

    ///////////////// Make Sure Exe File Exists ///////////////
    if (!vpxt_file_exists_check(vpxencPath.c_str()))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n",
            vpxencPath.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
    }

    ///////////////// Make Sure Exe File Exists ///////////////
    if (!vpxt_file_exists_check(vpxdecPath.c_str()))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n",
            vpxdecPath.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
    }

    ///////////////// Make Sure ivf File Exists ///////////////
    if (!vpxt_file_exists_check(input.c_str()))
    {
        tprintf(PRINT_BTH, "\nInput encode file %s does not exist\n",
            input.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
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
        vpxt_determinate_parameters(opt);

        if (vpxt_compress(input.c_str(), internal_comp.c_str(), speed, bitrate,
            opt, comp_out_str, 0, 0, enc_format, kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        // Get internal parameter file strings
        std::string internal_comp_par_1;
        int extLength = vpxt_remove_file_extension(internal_comp.c_str(),
            internal_comp_par_1);
        internal_comp_par_1 += "parameters_core.txt";
        std::string internal_comp_par_2 = internal_comp_par_1;
        internal_comp_par_2.erase(internal_comp_par_2.length()-(15 + extLength),
            15 + extLength);
        internal_comp_par_2 += "_parameters_vpx.txt";

        // convert internal parameters to vpxenc
        char vpxenc_parameters[1024];
        vpxt_convert_par_file_to_vpxenc(internal_comp_par_1.c_str(),
            internal_comp_par_2.c_str(), vpxenc_parameters, 1024);
        vpxenc_cmd_str += vpxenc_parameters;

        if (enc_format.compare("ivf") == 0)
            vpxenc_cmd_str += "--ivf";

        if(vpxt_file_is_yv12(input.c_str()))
            vpxenc_cmd_str += " --yv12";

        // run vpxenc comp
        tprintf(PRINT_ERR, "\nAttempting to run: %s\n\n",
            vpxenc_cmd_str.c_str());

        fclose(fp);

        vpxt_run_exe(vpxenc_cmd_str);
        vpxt_run_exe(vpxdec_cmd_str);

        if ((fp = freopen(text_file_str.c_str(), "a", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                text_file_str.c_str());
            exit(1);
        }

        tprintf(PRINT_ERR, "\nAttempting to run: %s\n\n",
            vpxdec_cmd_str.c_str());
        vpxt_decompress(vpxenc_comp.c_str(), internal_dec.c_str(), dec_format,
            1);
    }

    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    tprintf(PRINT_BTH, "\n\nComparing Compression Files: ");
    int lngRCEnc = vpxt_compare_enc(internal_comp.c_str(), vpxenc_comp.c_str(),
        0);
    int EncMatch = vpxt_print_compare_ivf_results(lngRCEnc, 1);

    tprintf(PRINT_BTH, "Comparing Decompression Files: ");
    int lngRCDec = vpxt_compare_dec(vpxdec_dec.c_str(), internal_dec.c_str());
    int DecMatch = vpxt_print_compare_ivf_results(lngRCDec, 0);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (EncMatch == 1)
    {
        vpxt_formated_print(RESPRT, "Internal Compression identical to Vpxenc "
            "Compression - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Internal Compression not identical to "
            "Vpxenc Compression - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (DecMatch == 1)
    {
        vpxt_formated_print(RESPRT, "Internal Decompression identical to "
            "Vpxenc Decompression - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Internal Decompression not identical to "
            "Vpxenc Decompression - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(4, internal_comp.c_str(), vpxenc_comp.c_str(),
        vpxdec_dec.c_str(), internal_dec.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;

}