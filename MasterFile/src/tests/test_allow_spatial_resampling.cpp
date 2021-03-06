#include "vpxt_test_declarations.h"

int test_allow_spatial_resampling(int argc,
                                  const char** argv,
                                  const std::string &working_dir,
                                  const std::string sub_folder_str,
                                  int test_type,
                                  int delete_ivf,
                                  int artifact_detection)
{
    char *comp_out_str = "Allow Spatial Resampling";
    char *test_dir = "test_allow_spatial_resampling";
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

    int spatial_on_enc_art_det = artifact_detection;

    std::string spatial_on_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_1";
    vpxt_enc_format_append(spatial_on_enc, enc_format);

    std::string spatial_off_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_0";
    vpxt_enc_format_append(spatial_off_enc, enc_format);

    std::string spatial_on_enc_set_cfg = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_1_set_cfg" ;
    vpxt_enc_format_append(spatial_on_enc_set_cfg, enc_format);

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
    opt.end_usage = 0;
    opt.resample_down_water_mark = 60;
    opt.resample_up_water_mark = 80;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;
        opt.allow_spatial_resampling = 0;

        if (vpxt_compress(input.c_str(), spatial_off_enc.c_str(), speed, bitrate
            , opt, comp_out_str, 0, 0, enc_format, kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.allow_spatial_resampling = 1;

        if (vpxt_compress(input.c_str(), spatial_on_enc.c_str(), speed, bitrate,
            opt, comp_out_str, 1, 0, enc_format, kSetConfigOff) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        if (vpxt_compress(input.c_str(), spatial_on_enc_set_cfg.c_str(), speed,
            bitrate, opt, comp_out_str, 1, 0, enc_format, kSetConfigOn) == -1)
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

    double spatial_resample_psnr = vpxt_psnr(input.c_str(),
        spatial_on_enc.c_str(), 0, PRINT_BTH, 1, 0, 0, 0, NULL,
        spatial_on_enc_art_det);

    char spatial_on_file_name[255];
    vpxt_file_name(spatial_on_enc.c_str(), spatial_on_file_name, 0);
    char spatial_off_file_name[255];
    vpxt_file_name(spatial_off_enc.c_str(), spatial_off_file_name, 0);

    tprintf(PRINT_BTH, "\nChecking: %s for resized frames\n",
        spatial_on_file_name);
    int spatial_on_frames_resized =
        vpxt_display_resized_frames(spatial_on_enc.c_str(), 1);

    tprintf(PRINT_BTH, "Checking: %s for resized frames\n",
        spatial_off_file_name);
    int spatial_off_frames_resized =
        vpxt_display_resized_frames(spatial_off_enc.c_str(), 1);

    char spatial_on_enc_fn[255];
    vpxt_file_name(spatial_on_enc.c_str(), spatial_on_enc_fn, 0);
    char spatial_on_enc_set_cfg_fn[255];
    vpxt_file_name(spatial_on_enc_set_cfg.c_str(), spatial_on_enc_set_cfg_fn,
        0);

    tprintf(PRINT_BTH, "Comparing: %s to %s\n", spatial_on_enc_fn,
        spatial_on_enc_set_cfg_fn);
    int lng_rc = vpxt_compare_enc(spatial_on_enc.c_str(),
        spatial_on_enc_set_cfg.c_str(), 0);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (spatial_on_frames_resized > 0)
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOn Frames Resized %i "
            "> 0 - Passed", spatial_on_frames_resized);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOn Frames Resized %i "
            "<= 0 - Failed", spatial_on_frames_resized);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (spatial_off_frames_resized == 0)
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOff Frames Resized "
            "%i == 0 - Passed", spatial_off_frames_resized);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOff Frames Resized "
            "%i != 0 - Failed", spatial_off_frames_resized);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (spatial_resample_psnr > 15.0)
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResample PSNR: %f > 15.00 - "
            "Passed", spatial_resample_psnr);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResample On PSNR: %f < 15.00 -"
            " Failed", spatial_resample_psnr);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (lng_rc >= -1)
    {
        vpxt_formated_print(RESPRT, "%s is identical to %s - Passed",
            spatial_on_enc_fn, spatial_on_enc_set_cfg_fn);
        tprintf(PRINT_BTH, "\n");
    }

    if (lng_rc == 0)
    {
        vpxt_formated_print(RESPRT, "%s is not identical to %s - Failed",
            spatial_on_enc_fn, spatial_on_enc_set_cfg_fn);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    // handle possible artifact
    if(spatial_on_enc_art_det == kPossibleArtifactFound)
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
        vpxt_delete_files(2, spatial_on_enc.c_str(), spatial_off_enc.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
