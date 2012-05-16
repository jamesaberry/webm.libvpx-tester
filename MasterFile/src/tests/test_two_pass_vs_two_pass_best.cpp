#include "vpxt_test_declarations.h"

int test_two_pass_vs_two_pass_best(int argc,
                                   const char** argv,
                                   const std::string &working_dir,
                                   const std::string sub_folder_str,
                                   int test_type,
                                   int delete_ivf,
                                   int artifact_detection)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_two_pass_vs_two_pass_best";
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

    int two_pass_out_1_art_det = artifact_detection;
    int two_pass_out_2_art_det = artifact_detection;
    int two_pass_out_3_art_det = artifact_detection;
    int two_pass_best_out_1_art_det = artifact_detection;
    int two_pass_best_out_2_art_det = artifact_detection;
    int two_pass_best_out_3_art_det = artifact_detection;

    std::string two_pass_out_1 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_two_pass_1";
    vpxt_enc_format_append(two_pass_out_1, enc_format);

    std::string two_pass_out_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_two_pass_2";
    vpxt_enc_format_append(two_pass_out_2, enc_format);

    std::string two_pass_out_3 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_two_pass_3";
    vpxt_enc_format_append(two_pass_out_3, enc_format);

    std::string two_pass_best_out_1 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_two_pass_best_1";
    vpxt_enc_format_append(two_pass_best_out_1, enc_format);

    std::string two_pass_best_out_2 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_two_pass_best_2";
    vpxt_enc_format_append(two_pass_best_out_2, enc_format);

    std::string two_pass_best_out_3 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_two_pass_best_3";
    vpxt_enc_format_append(two_pass_best_out_3, enc_format);

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
        opt.Mode = MODE_SECONDPASS;
        opt.target_bandwidth = bitrate_1;

        if (vpxt_compress(input.c_str(), two_pass_out_1.c_str(), speed,
            bitrate_1, opt, comp_out_str, compress_int, 0, enc_format,
            kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_2;

        if (vpxt_compress(input.c_str(), two_pass_out_2.c_str(), speed,
            bitrate_2, opt, comp_out_str, compress_int, 0, enc_format,
            kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_3;

        if (vpxt_compress(input.c_str(), two_pass_out_3.c_str(), speed,
            bitrate_3, opt, comp_out_str, compress_int, 0, enc_format,
            kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.Mode = MODE_SECONDPASS_BEST;
        opt.target_bandwidth = bitrate_1;

        if (vpxt_compress(input.c_str(), two_pass_best_out_1.c_str(), speed,
            bitrate_1, opt, comp_out_str, compress_int, 0, enc_format,
            kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_2;

        if (vpxt_compress(input.c_str(), two_pass_best_out_2.c_str(), speed,
            bitrate_2, opt, comp_out_str, compress_int, 0, enc_format,
            kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.target_bandwidth = bitrate_3;

        if (vpxt_compress(input.c_str(), two_pass_best_out_3.c_str(), speed,
            bitrate_3, opt, comp_out_str, compress_int, 0, enc_format,
            kSetConfigOff) == -1)
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

    tprintf(PRINT_STD, "\n");

    double good_size_1 = vpxt_data_rate(two_pass_out_1.c_str(), 1);
    double best_size_1 = vpxt_data_rate(two_pass_best_out_1.c_str(), 1);
    double good_size_2 = vpxt_data_rate(two_pass_out_2.c_str(), 1);
    double best_size_2 = vpxt_data_rate(two_pass_best_out_2.c_str(), 1);
    double good_size_3 = vpxt_data_rate(two_pass_out_3.c_str(), 1);
    double best_size_3 = vpxt_data_rate(two_pass_best_out_3.c_str(), 1);

    double psnr_good_1 = vpxt_psnr(input.c_str(), two_pass_out_1.c_str(), 1,
        PRINT_BTH, 1, 0, 0, 0, NULL, two_pass_out_1_art_det);
    double psnr_best_1 = vpxt_psnr(input.c_str(), two_pass_best_out_1.c_str(),
        1, PRINT_BTH, 1, 0, 0, 0, NULL, two_pass_best_out_1_art_det);
    double psnr_good_2 = vpxt_psnr(input.c_str(), two_pass_out_2.c_str(), 1,
        PRINT_BTH, 1, 0, 0, 0, NULL, two_pass_out_2_art_det);
    double psnr_best_2 = vpxt_psnr(input.c_str(), two_pass_best_out_2.c_str(),
        1, PRINT_BTH, 1, 0, 0, 0, NULL, two_pass_best_out_2_art_det);
    double psnr_good_3 = vpxt_psnr(input.c_str(), two_pass_out_3.c_str(), 1,
        PRINT_BTH, 1, 0, 0, 0, NULL, two_pass_out_3_art_det);
    double psnr_best_3 = vpxt_psnr(input.c_str(), two_pass_best_out_3.c_str(),
        1, PRINT_BTH, 1, 0, 0, 0, NULL, two_pass_best_out_3_art_det);

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

    tprintf(PRINT_BTH, "Two Pass Curve: y = %fx^2 + %fx + %f\n", good_a, good_b,
        good_c);
    tprintf(PRINT_BTH, "Two Pass Best Curve: y = %fx^2 + %fx + %f\n", best_a,
        best_b, best_c);
    tprintf(PRINT_BTH, "\nTwo Pass area under curve for interval %.2f - %.2f = "
        "%.2f\n", min_common, max_common, good_area);
    tprintf(PRINT_BTH, "Two Pass Best area under curve for interval %.2f - "
        "%.2f = %.2f\n", min_common, max_common, best_area);

    int test_state = kTestFailed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (good_area == best_area)
    {
        vpxt_formated_print(RESPRT, "Two Pass Best area under curve: %.2f == "
            "Two Pass area under curve: %.2f - Failed", best_area,
            good_area);
        tprintf(PRINT_BTH, "\n");
    }

    if (best_area > good_area)
    {
        vpxt_formated_print(RESPRT, "Two Pass Best area under curve: %.2f > "
            "Two Pass area under curve: %.2f - Passed", best_area,
            good_area);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestPassed;
    }

    if (best_area < good_area)
    {
        vpxt_formated_print(RESPRT, "Two Pass Best area under curve: %.2f < "
            "Two Pass area under curve: %.2f - Failed", best_area,
            good_area);
        tprintf(PRINT_BTH, "\n");
    }

    // handle possible artifact
    if(two_pass_out_1_art_det == kPossibleArtifactFound ||
        two_pass_out_2_art_det == kPossibleArtifactFound ||
        two_pass_out_3_art_det == kPossibleArtifactFound ||
        two_pass_best_out_1_art_det == kPossibleArtifactFound ||
        two_pass_best_out_2_art_det == kPossibleArtifactFound ||
        two_pass_best_out_3_art_det == kPossibleArtifactFound)
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
        vpxt_delete_files(6, two_pass_out_1.c_str(), two_pass_out_2.c_str(),
        two_pass_out_3.c_str(), two_pass_best_out_1.c_str(),
        two_pass_best_out_2.c_str(), two_pass_best_out_3.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}