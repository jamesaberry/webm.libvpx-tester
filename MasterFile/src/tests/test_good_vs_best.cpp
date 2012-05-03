#include "vpxt_test_declarations.h"

int test_good_vs_best(int argc,
                      const char** argv,
                      const std::string &working_dir,
                      const std::string sub_folder_str,
                      int test_type,
                      int delete_ivf)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_good_vs_best";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int bitrate = atoi(argv[3]);
    std::string enc_format = argv[4];

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

    std::string good_out_file_1 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_good_1";
    vpxt_enc_format_append(good_out_file_1, enc_format);

    std::string good_out_file_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_good_2";
    vpxt_enc_format_append(good_out_file_2, enc_format);

    std::string good_out_file_3 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_good_3";
    vpxt_enc_format_append(good_out_file_3, enc_format);

    std::string best_out_file_1 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_best_1";
    vpxt_enc_format_append(best_out_file_1, enc_format);

    std::string best_out_file_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_best_2";
    vpxt_enc_format_append(best_out_file_2, enc_format);

    std::string best_out_file_3 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_best_3";
    vpxt_enc_format_append(best_out_file_3, enc_format);

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
    int compress_int = opt.allow_df;
    int bitrate_1 = bitrate - (bitrate * 0.3);
    int bitrate_2 = bitrate;
    int bitrate_3 = bitrate + (bitrate * 0.3);

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.target_bandwidth = bitrate_1;
        opt.Mode = MODE_GOODQUALITY;

        if (vpxt_compress(input.c_str(), good_out_file_1.c_str(), speed,
            bitrate_1, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_2;

        if (vpxt_compress(input.c_str(), good_out_file_2.c_str(), speed,
            bitrate_2, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_3;

        if (vpxt_compress(input.c_str(), good_out_file_3.c_str(), speed,
            bitrate_3, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_1;
        opt.Mode = MODE_BESTQUALITY;

        if (vpxt_compress(input.c_str(), best_out_file_1.c_str(), speed,
            bitrate_1, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_2;

        if (vpxt_compress(input.c_str(), best_out_file_2.c_str(), speed,
            bitrate_2, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_3;

        if (vpxt_compress(input.c_str(), best_out_file_3.c_str(), speed,
            bitrate_3, opt, comp_out_str, compress_int, 0, enc_format) == -1)
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

    tprintf(PRINT_BTH, "\n");

    double good_size_1 = vpxt_data_rate(good_out_file_1.c_str(), 1);
    double best_size_1 = vpxt_data_rate(best_out_file_1.c_str(), 1);
    double good_size_2 = vpxt_data_rate(good_out_file_2.c_str(), 1);
    double best_size_2 = vpxt_data_rate(best_out_file_2.c_str(), 1);
    double good_size_3 = vpxt_data_rate(good_out_file_3.c_str(), 1);
    double best_size_3 = vpxt_data_rate(best_out_file_3.c_str(), 1);

    double psnr_good_1 = vpxt_psnr(input.c_str(), good_out_file_1.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_best_1 = vpxt_psnr(input.c_str(), best_out_file_1.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_good_2 = vpxt_psnr(input.c_str(), good_out_file_2.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_best_2 = vpxt_psnr(input.c_str(), best_out_file_2.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_good_3 = vpxt_psnr(input.c_str(), good_out_file_3.c_str(), 1,
        PRINT_BTH, 1, NULL);
    double psnr_best_3 = vpxt_psnr(input.c_str(), best_out_file_3.c_str(), 1,
        PRINT_BTH, 1, NULL);

    // data rates not always in order so find smallest observed data rate
    double good_size_min = good_size_1;
    double best_size_min = best_size_1;

    if(good_size_2 < good_size_min)
        good_size_min = good_size_2;

    if(good_size_3 < good_size_min)
        good_size_min = good_size_3;

    if(best_size_2 < best_size_min)
        best_size_min = best_size_2;

    if(best_size_3 < best_size_min)
        best_size_min = best_size_3;

    // data rates not always in order so find largest observed data rate
    double good_size_max = good_size_3;
    double best_size_max = best_size_3;

    if(good_size_2 > good_size_max)
        good_size_max = good_size_2;

    if(good_size_1 > good_size_max)
        good_size_max = good_size_1;

    if(best_size_2 > best_size_max)
        best_size_max = best_size_2;

    if(best_size_1 > best_size_max)
        best_size_max = best_size_1;

    double good_a = 0;
    double good_b = 0;
    double good_c = 0;

    double best_a = 0;
    double best_b = 0;
    double best_c = 0;

    double min_common = 0;
    double max_common = 0;

    // take area over same range we have decent data for.
    if (good_size_min > best_size_min)
        min_common = good_size_min;
    else
        min_common = best_size_min;

    if (good_size_max > best_size_max)
        max_common = best_size_max;
    else
        max_common = good_size_max;

    vpxt_solve_quadratic(good_size_1, good_size_2, good_size_3, psnr_good_1,
        psnr_good_2, psnr_good_3, good_a, good_b, good_c);
    double good_area = vpxt_area_under_quadratic(good_a, good_b, good_c,
        min_common, max_common);

    vpxt_solve_quadratic(best_size_1, best_size_2, best_size_3, psnr_best_1,
        psnr_best_2, psnr_best_3, best_a, best_b, best_c);
    double best_area = vpxt_area_under_quadratic(best_a, best_b, best_c,
        min_common, max_common);

    tprintf(PRINT_BTH, "\n\n"
            "Data Points:\n"
            "\n"
            " Good Q\n"
            "\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "\n"
            " Best Q\n"
            "\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "\n"
            "\n"
            , good_size_1, psnr_good_1
            , good_size_2, psnr_good_2
            , good_size_3, psnr_good_3
            , best_size_1, psnr_best_1
            , best_size_2, psnr_best_2
            , best_size_3, psnr_best_3
           );

    tprintf(PRINT_BTH, "Good Quality Curve: y = %fx^2 + %fx + %f\n", good_a,
        good_b, good_c);
    tprintf(PRINT_BTH, "Best Quality Curve: y = %fx^2 + %fx + %f\n", best_a,
        best_b, best_c);
    tprintf(PRINT_BTH, "\nGood Quality area under curve for interval %.2f - "
        "%.2f = %.2f\n", min_common, max_common, good_area);
    tprintf(PRINT_BTH, "Best Quality area under curve for interval %.2f - %.2f "
        "= %.2f\n", min_common, max_common, best_area);

    int pass = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (good_area == best_area)
    {
        vpxt_formated_print(RESPRT, "Best Quality area under curve: %.2f == "
            "Good Quality area under curve: %.2f - Failed", best_area,
            good_area);
        tprintf(PRINT_BTH, "\n");
    }

    if (best_area > good_area)
    {
        vpxt_formated_print(RESPRT, "Best Quality area under curve: %.2f > "
            "Good Quality area under curve: %.2f - Passed", best_area,
            good_area);
        tprintf(PRINT_BTH, "\n");
        pass = 1;
    }

    if (best_area < good_area)
    {
        vpxt_formated_print(RESPRT, "Best Quality area under curve: %.2f < "
            "Good Quality area under curve: %.2f - Failed", best_area,
            good_area);
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 1)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(6, good_out_file_2.c_str(),
            good_out_file_3.c_str(), good_out_file_1.c_str(),
            best_out_file_1.c_str(), best_out_file_2.c_str(),
            best_out_file_3.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(6, good_out_file_2.c_str(),
            good_out_file_3.c_str(),
            good_out_file_1.c_str(), best_out_file_1.c_str(),
            best_out_file_2.c_str(), best_out_file_3.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}