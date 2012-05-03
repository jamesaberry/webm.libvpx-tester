#include "vpxt_test_declarations.h"

int test_min_quantizer(int argc,
                       const char** argv,
                       const std::string &working_dir,
                       const std::string sub_folder_str,
                       int test_type,
                       int delete_ivf,
                       int artifact_detection)
{
    char *comp_out_str = "Min Quantizer";
    char *test_dir = "test_min_quantizer";
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

    int min_10_quant_out_file_art_det = artifact_detection;
    int min_60_quant_out_file_art_det = artifact_detection;

    std::string min_10_quant_out_file = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_10";
    vpxt_enc_format_append(min_10_quant_out_file, enc_format);

    std::string min_60_quant_out_file = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_60";
    vpxt_enc_format_append(min_60_quant_out_file, enc_format);

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

    vpxt_cap_string_print(PRINT_BTH, "%s", test_dir);

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
            record_test_complete(file_index_str, file_index_output_char, test_type);
            return kTestIndeterminate;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    double psnr_arr[2];
    opt.target_bandwidth = bitrate;
    opt.worst_allowed_q = 63;

    // make sure constrained quantizer mode is off
    if(opt.end_usage == 2)
        opt.end_usage = 1;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        opt.best_allowed_q = 10;
        // make sure max q is greater than min q
        while(opt.worst_allowed_q < opt.best_allowed_q)
                opt.worst_allowed_q = rand() % 64;

        if (vpxt_compress(input.c_str(), min_10_quant_out_file.c_str(), speed,
            bitrate, opt, comp_out_str, 10, 1, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.best_allowed_q = 60;
        // make sure max q is greater than min q
        while(opt.worst_allowed_q < opt.best_allowed_q)
                opt.worst_allowed_q = rand() % 64;

        if (vpxt_compress(input.c_str(), min_60_quant_out_file.c_str(), speed,
            bitrate, opt, comp_out_str, 60, 1, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char, test_type);
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

    psnr_arr[0] = vpxt_psnr(input.c_str(), min_10_quant_out_file.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, min_10_quant_out_file_art_det);
    psnr_arr[1] = vpxt_psnr(input.c_str(), min_60_quant_out_file.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, min_60_quant_out_file_art_det);

    tprintf(PRINT_BTH, "\n");
    int min_10_q = vpxt_check_min_quantizer(min_10_quant_out_file.c_str(), 10);
    tprintf(PRINT_BTH, "\n\n");
    int min_60_q = vpxt_check_min_quantizer(min_60_quant_out_file.c_str(), 60);
    tprintf(PRINT_BTH, "\n\n");

    char min_10_file_name[255] = "";
    char min_60_file_name[255] = "";

    vpxt_file_name(min_10_quant_out_file.c_str(), min_10_file_name, 0);
    vpxt_file_name(min_60_quant_out_file.c_str(), min_60_file_name, 0);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (min_10_q != -1)
    {
        vpxt_formated_print(RESPRT, "Not all %s quantizers above MinQ - Failed",
            min_10_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "All %s quantizers above MinQ - Passed",
            min_10_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if (min_60_q != -1)
    {
        vpxt_formated_print(RESPRT, "Not all %s quantizers above MinQ - Failed",
            min_60_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "All %s quantizers above MinQ - Passed",
            min_60_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if (psnr_arr[0] <= psnr_arr[1])
    {
        vpxt_formated_print(RESPRT, "MinQ 10 PSNR: %2.2f <= MinQ 60 PSNR: "
            "%2.2f - Failed", psnr_arr[0], psnr_arr[1]);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "MinQ 10 PSNR: %2.2f > MinQ 60 PSNR: %2.2f "
            "- Passed", psnr_arr[0], psnr_arr[1]);
        tprintf(PRINT_BTH, "\n");
    }

    // handle possible artifact
    if(min_10_quant_out_file_art_det == kPossibleArtifactFound ||
        min_60_quant_out_file_art_det == kPossibleArtifactFound)
    {
        tprintf(PRINT_BTH, "\nPossible Artifact\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPossibleArtifact;
    }

    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");

    if (delete_ivf)
        vpxt_delete_files(2, min_10_quant_out_file.c_str(),
        min_60_quant_out_file.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}