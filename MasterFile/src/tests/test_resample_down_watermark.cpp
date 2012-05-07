#include "vpxt_test_declarations.h"

int test_resample_down_watermark(int argc,
                                 const char** argv,
                                 const std::string &working_dir,
                                 const std::string sub_folder_str,
                                 int test_type,
                                 int delete_ivf,
                                 int artifact_detection)
{
    char *comp_out_str = "Resample Down Watermark";
    char *test_dir = "test_resample_down_watermark";
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

    std::string down_water_sample_90_comp = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_90";
    vpxt_enc_format_append(down_water_sample_90_comp, enc_format);

    std::string down_water_sample_20_comp = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_20";
    vpxt_enc_format_append(down_water_sample_20_comp, enc_format);

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
    opt.allow_lag = 0;
    opt.allow_spatial_resampling = 1;
    opt.end_usage = 0;
    opt.target_bandwidth = bitrate;

    int resample_down_water_high = 90;
    int resample_down_water_low = 20;
    int n = 0;

    opt.resample_up_water_mark = 100;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        opt.resample_down_water_mark = resample_down_water_high;

        if (vpxt_compress(input.c_str(), down_water_sample_90_comp.c_str(),
            speed, bitrate, opt, comp_out_str, opt.resample_down_water_mark, 0,
            enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.resample_down_water_mark = resample_down_water_low;

        if (vpxt_compress(input.c_str(), down_water_sample_20_comp.c_str(),
            speed, bitrate, opt, comp_out_str, opt.resample_down_water_mark, 0,
            enc_format) == -1)
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

    char down_water_sample_20_file_name[255];
    vpxt_file_name(down_water_sample_20_comp.c_str(),
        down_water_sample_20_file_name, 0);
    char down_water_sample_90_file_name[255];
    vpxt_file_name(down_water_sample_90_comp.c_str(),
        down_water_sample_90_file_name, 0);

    int disp_key_frames_20 =
        vpxt_display_key_frames(down_water_sample_20_comp.c_str(), 1);
    int disp_resized_frames_20 =
        vpxt_display_resized_frames(down_water_sample_20_comp.c_str(), 1);
    int disp_check_pbm_20 =
        vpxt_check_pbm_threshold(down_water_sample_20_comp.c_str(),
        opt.target_bandwidth, opt.maximum_buffer_size * 1000,
        opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000,
        resample_down_water_low);
    int rdwm_check_20 = 0;

    if (disp_resized_frames_20 > 0)
    {
        tprintf(PRINT_STD, "\nChecking %s buffer for correct resize frame "
            "placement:\n\n", down_water_sample_20_file_name);
        rdwm_check_20 = vpxt_dfwm_check(down_water_sample_20_comp.c_str(), 1);
    }
    else
        rdwm_check_20 = -2;

    int disp_key_frames_90 =
        vpxt_display_key_frames(down_water_sample_90_comp.c_str(), 1);
    int disp_resized_frames_90 =
        vpxt_display_resized_frames(down_water_sample_90_comp.c_str(), 1);
    int disp_check_pbm_90 =
        vpxt_check_pbm_threshold(down_water_sample_90_comp.c_str(),
        opt.target_bandwidth, opt.maximum_buffer_size * 1000,
        opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000,
        resample_down_water_high);
    int rdwm_check_90 = -3;

    if (disp_resized_frames_90 > 0)
    {
        tprintf(PRINT_STD, "\nChecking %s buffer for correct resize frame "
            "placement:\n\n", down_water_sample_90_file_name);
        rdwm_check_90 = vpxt_dfwm_check(down_water_sample_90_comp.c_str(), 1);
    }
    else
        rdwm_check_90 = -2;

    // 1 = failed // 2 = indt // 3 = track resize for 10 // track resize for 90
    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (disp_resized_frames_20 > 0 && disp_resized_frames_90 > 0)
    {
        vpxt_formated_print(RESPRT, "Both DWMS 10 and 90 returned resized "
            "frames - Indeterminate");
        tprintf(PRINT_BTH, "\n");
    }

    if (disp_resized_frames_20 == 0 && disp_resized_frames_90 > 0)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 returned no resized frames; DWMS "
            "90 returned resized frames - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestMinPassed;
    }

    if (disp_resized_frames_20 > 0 && disp_resized_frames_90 == 0)
    {
        vpxt_formated_print(RESPRT, "DWMS 90 returned no resized frames; DWMS "
            "10 returned resized frames - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (disp_resized_frames_20 == 0 && disp_resized_frames_90 == 0)
    {
        vpxt_formated_print(RESPRT, "Both DWMS 10 and 90 returned  no resized "
            "frames - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestIndeterminate;
    }

    if (rdwm_check_20 == 0)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 resizes first frame at correct "
            "buffer location - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (rdwm_check_20 == 1)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 does not resize first frame at "
            "correct buffer location - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (rdwm_check_20 == -3)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 buffer threshold never reached - "
            "Indeterminate");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestIndeterminate;
    }

    if (rdwm_check_90 == 0)
    {
        if (test_state == kTestMinPassed)
        {
            vpxt_formated_print(RESPRT, "DWMS 90 resizes first frame at "
                "correct buffer location - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DWMS 90 resizes first frame at "
                "correct buffer location - Passed");
            tprintf(PRINT_BTH, "\n");
        }
    }

    if (rdwm_check_90 == 1)
    {
        vpxt_formated_print(RESPRT, "DWMS 90 does not resize first frame at "
            "correct buffer location - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (rdwm_check_90 == -3)
    {
        vpxt_formated_print(RESPRT, "DWMS 90 buffer threshold never reached - "
            "Indeterminate");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestIndeterminate;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestMinPassed)
        tprintf(PRINT_BTH, "\nMin Passed\n");
    if (test_state == kTestIndeterminate)
        tprintf(PRINT_BTH, "\nIndeterminate\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(2, down_water_sample_90_comp.c_str(),
        down_water_sample_20_comp.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}