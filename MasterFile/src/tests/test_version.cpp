#include "vpxt_test_declarations.h"

int test_version(int argc,
                 const char** argv,
                 const std::string &working_dir,
                 const std::string sub_folder_str,
                 int test_type,
                 int delete_ivf,
                 int artifact_detection)
{
    char *comp_out_str = "Version";
    char *test_dir = "test_version";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];
    std::string dec_format = argv[6];

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

    int version_0_art_det = artifact_detection;
    int version_1_art_det = artifact_detection;
    int version_2_art_det = artifact_detection;
    int version_3_art_det = artifact_detection;

    std::string version_0 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_0";
    vpxt_enc_format_append(version_0, enc_format);

    std::string version_1 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_1";
    vpxt_enc_format_append(version_1, enc_format);

    std::string version_2 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_2";
    vpxt_enc_format_append(version_2, enc_format);

    std::string version_3 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_3";
    vpxt_enc_format_append(version_3, enc_format);

    std::string version_0_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_decompression_0";
    vpxt_dec_format_append(version_0_dec, dec_format);

    std::string version_1_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_decompression_1";
    vpxt_dec_format_append(version_1_dec, dec_format);

    std::string version_2_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_decompression_2";
    vpxt_dec_format_append(version_2_dec, dec_format);

    std::string version_3_dec = cur_test_dir_str + slashCharStr() + test_dir +
        "_decompression_3";
    vpxt_dec_format_append(version_3_dec, dec_format);

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

    opt.target_bandwidth = bitrate;

    double psnr_arr[4];
    unsigned int dec_cpu_tick[4];

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        dec_cpu_tick[0] = vpxt_cpu_tick_return(version_0_dec.c_str(), 1);
        dec_cpu_tick[1] = vpxt_cpu_tick_return(version_1_dec.c_str(), 1);
        dec_cpu_tick[2] = vpxt_cpu_tick_return(version_2_dec.c_str(), 1);
        dec_cpu_tick[3] = vpxt_cpu_tick_return(version_3_dec.c_str(), 1);
    }
    else
    {
        opt.Mode = mode;

        opt.Version = 0;

        if (vpxt_compress(input.c_str(), version_0.c_str(), speed, bitrate, opt,
            comp_out_str, 0, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.Version = 1;

        if (vpxt_compress(input.c_str(), version_1.c_str(), speed, bitrate, opt,
            comp_out_str, 1, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.Version = 2;

        if (vpxt_compress(input.c_str(), version_2.c_str(), speed, bitrate, opt,
            comp_out_str, 2, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.Version = 3;

        if (vpxt_compress(input.c_str(), version_3.c_str(), speed, bitrate, opt,
            comp_out_str, 3, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_STD, "\n\n");
        unsigned int time_1 = vpxt_time_decompress(version_0.c_str(),
            version_0_dec.c_str(), dec_cpu_tick[0], dec_format, 1);

        if (time_1 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_STD, "\n");
        unsigned int time_2 = vpxt_time_decompress(version_1.c_str(),
            version_1_dec.c_str(), dec_cpu_tick[1], dec_format, 1);

        if (time_2 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_STD, "\n");
        unsigned int time_3 = vpxt_time_decompress(version_2.c_str(),
            version_2_dec.c_str(), dec_cpu_tick[2], dec_format, 1);

        if (time_3 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_STD, "\n");
        unsigned int time_4 = vpxt_time_decompress(version_3.c_str(),
            version_3_dec.c_str(), dec_cpu_tick[3], dec_format, 1);

        if (time_4 == -1)
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

    psnr_arr[0] = vpxt_psnr(input.c_str(), version_0.c_str(), 0, PRINT_BTH, 1,
        0, 0, 0, NULL, version_0_art_det);
    psnr_arr[1] = vpxt_psnr(input.c_str(), version_1.c_str(), 0, PRINT_BTH, 1,
        0, 0, 0, NULL, version_1_art_det);
    psnr_arr[2] = vpxt_psnr(input.c_str(), version_2.c_str(), 0, PRINT_BTH, 1,
        0, 0, 0, NULL, version_2_art_det);
    psnr_arr[3] = vpxt_psnr(input.c_str(), version_3.c_str(), 0, PRINT_BTH, 1,
        0, 0, 0, NULL, version_3_art_det);

    tprintf(PRINT_BTH, "\n");

    int psnr_fail = 0;
    int time_fail = 0;
    int i = 0;

    while (i < 4)
    {
        int t = i + 1;
        while (t < 4)
        {
            // i should always have Higher PSNR than t
            // i should always have a higher dec_cpu_tick as well.
            if (psnr_arr[i] < psnr_arr[t])
            {
                if (dec_cpu_tick[i] < dec_cpu_tick[t])
                {
                    tprintf(PRINT_BTH, "\nFailed Version %i Decode Tick: %d "
                        ">= Version %i Decode Tick: %d\n"
                            "Failed Version %i PSNR: %f <= Version %i PSNR: "
                            "%f\n", i, dec_cpu_tick[i], t, dec_cpu_tick[t], i,
                            psnr_arr[i], t, psnr_arr[t]);
                    time_fail++;
                    psnr_fail++;
                }
                else
                {
                    tprintf(PRINT_BTH, "\n       Version %i Decode Tick: %d "
                        ">= Version %i Decode Tick: %d\n"
                            "Failed Version %i PSNR: %f <= Version %i PSNR: "
                            "%f\n", i, dec_cpu_tick[i], t, dec_cpu_tick[t], i,
                            psnr_arr[i], t, psnr_arr[t]);
                    psnr_fail++;
                }
            }
            else
            {
                if (dec_cpu_tick[i] < dec_cpu_tick[t])
                {
                    tprintf(PRINT_BTH, "\nFailed Version %i Decode Tick: %d <= "
                        "Version %i Decode Tick: %d\n"
                            "       Version %i PSNR: %f >= Version %i PSNR: "
                            "%f\n", i, dec_cpu_tick[i], t, dec_cpu_tick[t], i,
                            psnr_arr[i], t, psnr_arr[t]);
                    time_fail++;
                }
                else
                {
                    tprintf(PRINT_BTH, "\n       Version %i Decode Tick: %d >= "
                        "Version %i Decode Tick: %d\n"
                            "       Version %i PSNR: %f >= Version %i PSNR: "
                            "%f\n", i, dec_cpu_tick[i], t, dec_cpu_tick[t], i,
                            psnr_arr[i], t, psnr_arr[t]);
                }
            }
            t++;
        }
        i++;
    }

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (psnr_fail == 0)
    {
        vpxt_formated_print(RESPRT, "All PSNRs decrease as version numbers "
            "increase - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (psnr_fail < 2 && psnr_fail != 0)
    {
        vpxt_formated_print(RESPRT, "All but one PSNR Decreases as version "
            "numbers increase - Min Passed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestMinPassed;
    }

    if (psnr_fail >= 2)
    {
        vpxt_formated_print(RESPRT, "Not all PSNRs decrease as version numbers "
            "increase - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (time_fail == 0)
    {
        vpxt_formated_print(RESPRT, "All Decode ticks decrease as version "
            "numbers increase - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (time_fail < 2 && time_fail != 0)
    {
        vpxt_formated_print(RESPRT, "All but one Decode ticks decrease as "
            "version numbers increase - Min Passed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestMinPassed;
    }

    if (time_fail >= 2)
    {
        vpxt_formated_print(RESPRT, "Not all Decode ticks increase as "
            "version numbers increase - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    // handle possible artifact
    if(version_0_art_det == kPossibleArtifactFound ||
        version_1_art_det == kPossibleArtifactFound ||
        version_2_art_det == kPossibleArtifactFound ||
        version_3_art_det == kPossibleArtifactFound)
    {
        tprintf(PRINT_BTH, "\nPossible Artifact\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPossibleArtifact;
    }

    if (test_state == kTestMinPassed)
        tprintf(PRINT_BTH, "\nMin Passed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");

    if (delete_ivf)
        vpxt_delete_files(8, version_0.c_str(), version_1.c_str(),
        version_2.c_str(), version_3.c_str(), version_0_dec.c_str(),
        version_1_dec.c_str(), version_2_dec.c_str(),version_3_dec.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}