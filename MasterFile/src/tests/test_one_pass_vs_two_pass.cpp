#include "vpxt_test_declarations.h"

int test_one_pass_vs_two_pass(int argc,
                              const char *const *argv,
                              const std::string &working_dir,
                              const std::string sub_folder_str,
                              int test_type,
                              int delete_ivf)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_one_pass_vs_two_pass";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int bitrate = atoi(argv[3]);
    std::string enc_format = argv[4];

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

    std::string one_pass_out_1 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_one_pass_1";
    vpxt_enc_format_append(one_pass_out_1, enc_format);

    std::string one_pass_out_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_one_pass_2";
    vpxt_enc_format_append(one_pass_out_2, enc_format);

    std::string one_pass_out_3 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_one_pass_3";
    vpxt_enc_format_append(one_pass_out_3, enc_format);

    std::string two_pass_out_1 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_two_pass_1";
    vpxt_enc_format_append(two_pass_out_1, enc_format);

    std::string two_pass_out_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_two_pass_2";
    vpxt_enc_format_append(two_pass_out_2, enc_format);

    std::string two_pass_out_3 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_two_pass_3";
    vpxt_enc_format_append(two_pass_out_3, enc_format);

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
    int compress_int = opt.allow_df;
    int bitrate_1 = bitrate - (bitrate * 0.3);
    int bitrate_2 = bitrate;
    int bitrate_3 = bitrate + (bitrate * 0.3);


    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = 5;
        opt.target_bandwidth = bitrate_1;

        if (vpxt_compress(input.c_str(), two_pass_out_1.c_str(), speed,
            bitrate_1, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.target_bandwidth = bitrate_2;

        if (vpxt_compress(input.c_str(), two_pass_out_2.c_str(), speed,
            bitrate_2, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.target_bandwidth = bitrate_3;

        if (vpxt_compress(input.c_str(), two_pass_out_3.c_str(), speed,
            bitrate_3, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.Mode = 2;
        opt.target_bandwidth = bitrate_1;

        if (vpxt_compress(input.c_str(), one_pass_out_1.c_str(), speed,
            bitrate_1, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.target_bandwidth = bitrate_2;

        if (vpxt_compress(input.c_str(), one_pass_out_2.c_str(), speed,
            bitrate_2, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.target_bandwidth = bitrate_3;

        if (vpxt_compress(input.c_str(), one_pass_out_3.c_str(), speed,
            bitrate_3, opt, comp_out_str, compress_int, 0, enc_format) == -1)
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

    tprintf(PRINT_STD, "\n");

    double size_two_pass_1 = vpxt_data_rate(two_pass_out_1.c_str(), 1);
    double size_one_pass_1 = vpxt_data_rate(one_pass_out_1.c_str(), 1);
    double size_two_pass_2 = vpxt_data_rate(two_pass_out_2.c_str(), 1);
    double size_one_pass_2 = vpxt_data_rate(one_pass_out_2.c_str(), 1);
    double size_two_pass_3 = vpxt_data_rate(two_pass_out_3.c_str(), 1);
    double size_one_pass_3 = vpxt_data_rate(one_pass_out_3.c_str(), 1);

    double psnr_two_pass_1 = vpxt_psnr(input.c_str(), two_pass_out_1.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_two_pass_2 = vpxt_psnr(input.c_str(), two_pass_out_2.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_two_pass_3 = vpxt_psnr(input.c_str(), two_pass_out_3.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_one_pass_1 = vpxt_psnr(input.c_str(), one_pass_out_1.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_one_pass_2 = vpxt_psnr(input.c_str(), one_pass_out_2.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_one_pass_3 = vpxt_psnr(input.c_str(), one_pass_out_3.c_str(), 1,
        PRINT_BTH, 1, NULL);

    int pass = 0;

    //data rates not always in order so find smallest observed data rate
    double size_two_pass_min = size_two_pass_1;
    double size_one_pass_min = size_one_pass_1;

    if(size_two_pass_2 < size_two_pass_min)
        size_two_pass_min = size_two_pass_2;

    if(size_two_pass_3 < size_two_pass_min)
        size_two_pass_min = size_two_pass_3;

    if(size_one_pass_2 < size_one_pass_min)
        size_one_pass_min = size_one_pass_2;

    if(size_one_pass_3 < size_one_pass_min)
        size_one_pass_min = size_one_pass_3;

    //data rates not always in order so find largest observed data rate
    double size_two_pass_max = size_two_pass_3;
    double size_one_pass_max = size_one_pass_3;

    if(size_two_pass_2 > size_two_pass_max)
        size_two_pass_max = size_two_pass_2;

    if(size_two_pass_1 > size_two_pass_max)
        size_two_pass_max = size_two_pass_1;

    if(size_one_pass_2 > size_one_pass_max)
        size_one_pass_max = size_one_pass_2;

    if(size_one_pass_1 > size_one_pass_max)
        size_one_pass_max = size_one_pass_1;

    double two_pass_a = 0;
    double two_pass_b = 0;
    double two_pass_c = 0;

    double one_pass_a = 0;
    double one_pass_b = 0;
    double one_pass_c = 0;

    double min_common = 0;
    double max_common = 0;

    //take area over same range we have decent data for.
    if (size_two_pass_min > size_one_pass_min)
        min_common = size_two_pass_min;
    else
        min_common = size_one_pass_min;

    if (size_two_pass_max > size_one_pass_max)
        max_common = size_one_pass_max;
    else
        max_common = size_two_pass_max;

    vpxt_solve_quadratic(size_two_pass_1, size_two_pass_2, size_two_pass_3,
        psnr_two_pass_1, psnr_two_pass_2, psnr_two_pass_3, two_pass_a,
        two_pass_b, two_pass_c);
    double two_pass_area = vpxt_area_under_quadratic(two_pass_a, two_pass_b,
        two_pass_c, min_common, max_common);

    vpxt_solve_quadratic(size_one_pass_1, size_one_pass_2, size_one_pass_3,
        psnr_one_pass_1, psnr_one_pass_2, psnr_one_pass_3, one_pass_a,
        one_pass_b, one_pass_c);
    double one_pass_area = vpxt_area_under_quadratic(one_pass_a, one_pass_b,
        one_pass_c, min_common, max_common);

    tprintf(PRINT_BTH, "\n\n"
            "Data Points:\n"
            "\n"
            " Two Pass\n"
            "\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "\n"
            " One Pass\n"
            "\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "\n"
            "\n"
            , size_two_pass_1, psnr_two_pass_1
            , size_two_pass_2, psnr_two_pass_2
            , size_two_pass_3, psnr_two_pass_3
            , size_one_pass_1, psnr_one_pass_1
            , size_one_pass_2, psnr_one_pass_2
            , size_one_pass_3, psnr_one_pass_3
           );

    tprintf(PRINT_BTH, "Two Pass Curve: y = %fx^2 + %fx + %f\n", two_pass_a,
        two_pass_b, two_pass_c);
    tprintf(PRINT_BTH, "One Pass Curve: y = %fx^2 + %fx + %f\n", one_pass_a,
        one_pass_b, one_pass_c);
    tprintf(PRINT_BTH, "\nGood Quality area under curve for interval %.2f - "
        "%.2f = %.2f\n", min_common, max_common, two_pass_area);
    tprintf(PRINT_BTH, "Best Quality area under curve for interval %.2f - "
        "%.2f = %.2f\n", min_common, max_common, one_pass_area);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (two_pass_area == one_pass_area)
    {
        vpxt_formated_print(RESPRT, "Two Pass area under curve: %.2f == One "
            "Pass area under curve: %.2f - Failed", two_pass_area,
            one_pass_area);
        tprintf(PRINT_BTH, "\n");
    }

    if (one_pass_area < two_pass_area)
    {
        vpxt_formated_print(RESPRT, "Two Pass area under curve: %.2f > One "
            "Pass area under curve: %.2f - Passed", two_pass_area,
            one_pass_area);
        tprintf(PRINT_BTH, "\n");
        pass = 1;
    }

    if (one_pass_area > two_pass_area)
    {
        vpxt_formated_print(RESPRT, "Two Pass  area under curve: %.2f < One "
            "Pass area under curve: %.2f - Failed", two_pass_area,
            one_pass_area);
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 1)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(6, one_pass_out_1.c_str(),
            one_pass_out_2.c_str(), one_pass_out_3.c_str(),
            two_pass_out_1.c_str(), two_pass_out_2.c_str(),
            two_pass_out_3.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(6, one_pass_out_1.c_str(),
            one_pass_out_2.c_str(), one_pass_out_3.c_str(),
            two_pass_out_1.c_str(), two_pass_out_2.c_str(),
            two_pass_out_3.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}