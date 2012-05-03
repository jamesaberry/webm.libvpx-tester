#include "vpxt_test_declarations.h"

int test_multiple_resolution_encode(int argc,
                                    const char *const *argv,
                                    const std::string &working_dir,
                                    const std::string sub_folder_str,
                                    int test_type,
                                    int delete_ivf)
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

    ////////////Formatting Test Specific directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return TEST_ERRFM;

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

    std::string multi_res_enc_1 = multi_res_enc + "-" + width_1_char + "x" +
        height_1_char;
    vpxt_enc_format_append(multi_res_enc_1, enc_format);

    std::string multi_res_enc_2 = multi_res_enc + "-" + width_2_char + "x" +
        height_2_char;
    vpxt_enc_format_append(multi_res_enc_2, enc_format);

    std::string multi_res_enc_3 = multi_res_enc + "-" + width_3_char + "x" +
        height_3_char;
    vpxt_enc_format_append(multi_res_enc_3, enc_format);

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
            record_test_complete(file_index_str, file_index_output_char, test_type);
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
        if (vpxt_compress_multi_resolution(input.c_str(), multi_res_enc.c_str(),
            speed, bitrate, opt, comp_out_str, 1, 0, enc_format) == -1)
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
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
    }

    double psnr_1 = vpxt_psnr(input.c_str(), multi_res_enc_1.c_str(), 0,
        PRINT_BTH, 1, NULL);
    double psnr_2 = vpxt_psnr(input.c_str(), multi_res_enc_2.c_str(), 0,
        PRINT_BTH, 1, NULL);
    double psnr_3 = vpxt_psnr(input.c_str(), multi_res_enc_3.c_str(), 0,
        PRINT_BTH, 1, NULL);

    char multi_res_enc_1_file_name[256];
    char multi_res_enc_2_file_name[256];
    char multi_res_enc_3_file_name[256];

    vpxt_file_name(multi_res_enc_1.c_str(), multi_res_enc_1_file_name, 0);
    vpxt_file_name(multi_res_enc_2.c_str(), multi_res_enc_2_file_name, 0);
    vpxt_file_name(multi_res_enc_3.c_str(), multi_res_enc_3_file_name, 0);

    int fail = 0;

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
        fail = 1;
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
        fail = 1;
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
        fail = 1;
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
        
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(3, multi_res_enc_1.c_str(),
            multi_res_enc_2.c_str(), multi_res_enc_3.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(3, multi_res_enc_1.c_str(),
            multi_res_enc_2.c_str(), multi_res_enc_3.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}
