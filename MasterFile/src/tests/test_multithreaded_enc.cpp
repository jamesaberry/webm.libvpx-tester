#include "vpxt_test_declarations.h"

int test_multithreaded_enc(int argc,
                           const char** argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf,
                           int artifact_detection)
{
    char *comp_out_str = "Multithreaded";
    char *test_dir = "test_multithreaded_enc";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    unsigned int core_count = atoi(argv[5]);
    std::string enc_format = argv[6];

    int multithreaded = 0;
    unsigned int time_1;
    unsigned int time_2;

    if (mode != 0 && mode != 1)
        return vpxt_test_help(argv[1], 0);

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string multithreaded_on_comp = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_1";
    vpxt_enc_format_append(multithreaded_on_comp, enc_format);

    std::string multithreaded_off_comp = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_0";
    vpxt_enc_format_append(multithreaded_off_comp, enc_format);

    ///////////// Open Output File and Print Header ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;
    FILE *fp;

    vpxt_open_output_file(test_type, text_file_str, fp);
    vpxt_print_header(argc, argv, main_test_dir_char, cur_test_dir_str,
        test_dir, test_type);

    if (!(core_count > 1))
    {
        tprintf(PRINT_STD, "\nMultiple Cores not used Test aborted: %i\n",
            core_count);
        fclose(fp);
        return kTestFailed;
    }

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    /////////////////// Use Custom Settings ///////////////////
    if(vpxt_use_custom_settings(argv, argc, input_ver, fp, file_index_str,
        file_index_output_char, test_type, opt, bitrate)
        == kTestIndeterminate)
        return kTestIndeterminate;

    opt.target_bandwidth = bitrate;
    opt.end_usage = 0;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        time_1 = vpxt_time_return(multithreaded_on_comp.c_str(), 0);
        time_2 = vpxt_time_return(multithreaded_off_comp.c_str(), 0);
    }
    else
    {
        if (mode == kRealTime)
        {
            opt.Mode = MODE_REALTIME;
            opt.multi_threaded = core_count;
            opt.cpu_used = -1;
            unsigned int cpu_tick_1 = 0;
            time_1 = vpxt_time_compress(input.c_str(),
                multithreaded_on_comp.c_str(), multithreaded, bitrate, opt,
                comp_out_str, opt.multi_threaded, 0, cpu_tick_1, enc_format);

            if (time_1 == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }

            opt.Mode = MODE_REALTIME;
            opt.multi_threaded = 0;
            unsigned int cpu_tick_2 = 0;
            time_2 = vpxt_time_compress(input.c_str(),
                multithreaded_off_comp.c_str(), multithreaded, bitrate, opt,
                comp_out_str, opt.multi_threaded, 0, cpu_tick_2, enc_format);

            if (time_2 == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }
        }

        if (mode == kOnePassGoodQuality)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.multi_threaded = core_count;
            unsigned int cpu_tick_1 = 0;
            time_1 = vpxt_time_compress(input.c_str(),
                multithreaded_on_comp.c_str(), multithreaded, bitrate, opt,
                comp_out_str, opt.multi_threaded, 0, cpu_tick_1, enc_format);

            if (time_1 == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.multi_threaded = 0;
            unsigned int cpu_tick_2 = 0;
            time_2 = vpxt_time_compress(input.c_str(),
                multithreaded_off_comp.c_str(), multithreaded, bitrate, opt,
                comp_out_str, opt.multi_threaded, 0, cpu_tick_2, enc_format);

            if (time_2 == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }
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

    char time_1_file_name[255] = "";
    char time_2_file_name[255] = "";

    vpxt_file_name(multithreaded_on_comp.c_str(), time_1_file_name, 0);
    vpxt_file_name(multithreaded_off_comp.c_str(), time_2_file_name, 0);

    int test_state = kTestFailed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (time_1 == 0 || time_2 == 0)
    {
        if (time_1 == 0)
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed",
            time_1_file_name, time_1);

        if (time_2 == 0)
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed",
            time_2_file_name, time_2);
    }

    if (time_1 < time_2 && !(time_1 == 0 || time_2 == 0))
    {
        vpxt_formated_print(RESPRT, "%s time: %u < %s time2: %u - Passed",
            time_1_file_name, time_1, time_2_file_name, time_2);

        test_state = kTestPassed;
    }

    if (time_1 == time_2 && !(time_1 == 0 || time_2 == 0))
    {
        vpxt_formated_print(RESPRT, "%s time: %u == %s time: %u - Indeterminate"
            , time_1_file_name, time_1, time_2_file_name, time_2);

        test_state = kTestIndeterminate;
    }

    if (time_1 > time_2 && !(time_1 == 0 || time_2 == 0))
    {
        vpxt_formated_print(RESPRT, "%s time: %u > %s time: %u - Failed",
            time_1_file_name, time_1, time_2_file_name, time_2);
    }

    if(test_state == kTestPassed)
        tprintf(PRINT_BTH, "\n\nPassed\n");
    if(test_state == kTestFailed)
        tprintf(PRINT_BTH, "\n\nFailed\n");
    if(test_state == kTestIndeterminate)
        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

    if (delete_ivf)
        vpxt_delete_files(2, multithreaded_on_comp.c_str(),
        multithreaded_off_comp.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}