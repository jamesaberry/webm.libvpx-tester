#include "vpxt_test_declarations.h"

int test_multiple_resolution_encode(int argc,
                                    const char** argv,
                                    const std::string &working_dir,
                                    const std::string sub_folder_str,
                                    int test_type,
                                    int delete_ivf,
                                    int artifact_detection)
{
    char *comp_out_str = "Allow Spatial Resampling";
    char *test_dir = "test_multiple_resolution_encode";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int bitrate = atoi(argv[3]);
    std::string enc_format = argv[4];

    unsigned int width_1;
    unsigned int height_1;
    unsigned int width_2;
    unsigned int height_2;
    unsigned int width_3;
    unsigned int height_3;

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

    char width_1_char[10];
    char width_2_char[10];
    char width_3_char[10];

    char height_1_char[10];
    char height_2_char[10];
    char height_3_char[10];

    vpxt_get_multi_res_width_height(input.c_str(), 1, width_1, height_1);
    vpxt_get_multi_res_width_height(input.c_str(), 2, width_2, height_2);
    vpxt_get_multi_res_width_height(input.c_str(), 3, width_3, height_3);

    vpxt_itoa_custom(width_1, width_1_char, 10);
    vpxt_itoa_custom(height_1, height_1_char, 10);
    vpxt_itoa_custom(width_2, width_2_char, 10);
    vpxt_itoa_custom(height_2, height_2_char, 10);
    vpxt_itoa_custom(width_3, width_3_char, 10);
    vpxt_itoa_custom(height_3, height_3_char, 10);

    std::string multi_res_enc = cur_test_dir_str + slashCharStr() + test_dir;

    int multi_res_enc_1_art_det = artifact_detection;
    int multi_res_enc_2_art_det = artifact_detection;
    int multi_res_enc_3_art_det = artifact_detection;

    std::string multi_res_enc_1 = multi_res_enc + "-" + width_1_char + "x" +
        height_1_char;
    vpxt_enc_format_append(multi_res_enc_1, enc_format);

    std::string multi_res_enc_2 = multi_res_enc + "-" + width_2_char + "x" +
        height_2_char;
    vpxt_enc_format_append(multi_res_enc_2, enc_format);

    std::string multi_res_enc_3 = multi_res_enc + "-" + width_3_char + "x" +
        height_3_char;
    vpxt_enc_format_append(multi_res_enc_3, enc_format);

    ///////////// OutPutfile ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;
    FILE *fp;

    vpxt_open_output_file(test_type, text_file_str, fp);

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

    opt.target_bandwidth = bitrate;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        if (vpxt_compress_multi_resolution(input.c_str(), multi_res_enc.c_str(),
            speed, bitrate, opt, comp_out_str, 1, 0, enc_format) == -1)
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
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    double psnr_1 = vpxt_psnr(input.c_str(), multi_res_enc_1.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, multi_res_enc_1_art_det);
    double psnr_2 = vpxt_psnr(input.c_str(), multi_res_enc_2.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, multi_res_enc_2_art_det);
    double psnr_3 = vpxt_psnr(input.c_str(), multi_res_enc_3.c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, multi_res_enc_3_art_det);

    char multi_res_enc_1_file_name[256];
    char multi_res_enc_2_file_name[256];
    char multi_res_enc_3_file_name[256];

    vpxt_file_name(multi_res_enc_1.c_str(), multi_res_enc_1_file_name, 0);
    vpxt_file_name(multi_res_enc_2.c_str(), multi_res_enc_2_file_name, 0);
    vpxt_file_name(multi_res_enc_3.c_str(), multi_res_enc_3_file_name, 0);

    int test_state = kTestPassed;

    if (psnr_1 > 25.0)
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f > 25 - Passed",
            multi_res_enc_1_file_name, psnr_1);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 25 - Failed",
            multi_res_enc_1_file_name, psnr_1);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    if (psnr_2 > 25.0)
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f > 25 - Passed",
            multi_res_enc_2_file_name, psnr_2);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 25 - Failed",
            multi_res_enc_2_file_name, psnr_2);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }
    if (psnr_3 > 25.0)
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f > 25 - Passed",
            multi_res_enc_3_file_name, psnr_3);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 25 - Failed",
            multi_res_enc_3_file_name, psnr_3);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (psnr_1 >= psnr_2 && psnr_2 >= psnr_3)
    {
        vpxt_formated_print(RESPRT, "%s: %.2f >= %s: %.2f >= %s: %.2f - Passed"
            , multi_res_enc_1_file_name, psnr_1, multi_res_enc_2_file_name,
            psnr_2, multi_res_enc_3_file_name, psnr_3);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        if(psnr_1 < psnr_2 &&  psnr_2 < psnr_3)
        {
            vpxt_formated_print(RESPRT, "%s: %.2f < %s: %.2f < %s: %.2f - "
                "Failed", multi_res_enc_1_file_name, psnr_1,
                multi_res_enc_2_file_name, psnr_2, multi_res_enc_3_file_name,
                psnr_3);
            tprintf(PRINT_BTH, "\n");
        }
        else if(psnr_1 < psnr_2)
        {
            vpxt_formated_print(RESPRT, "%s: %.2f < %s: %.2f >= %s: %.2f - "
               "Failed", multi_res_enc_1_file_name, psnr_1,
               multi_res_enc_2_file_name, psnr_2, multi_res_enc_3_file_name,
               psnr_3);
            tprintf(PRINT_BTH, "\n");
        }
        else if(psnr_2 < psnr_3)
        {
            vpxt_formated_print(RESPRT, "%s: %.2f >= %s: %.2f < %s: %.2f - "
                "Failed", multi_res_enc_1_file_name, psnr_1,
                multi_res_enc_2_file_name, psnr_2, multi_res_enc_3_file_name,
                psnr_3);
            tprintf(PRINT_BTH, "\n");
        }
        
        test_state = kTestFailed;
    }

    // handle possible artifact
    if(multi_res_enc_1_art_det == kPossibleArtifactFound ||
        multi_res_enc_2_art_det == kPossibleArtifactFound ||
        multi_res_enc_3_art_det == kPossibleArtifactFound)
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
        vpxt_delete_files(3, multi_res_enc_1.c_str(), multi_res_enc_2.c_str(),
        multi_res_enc_3.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
