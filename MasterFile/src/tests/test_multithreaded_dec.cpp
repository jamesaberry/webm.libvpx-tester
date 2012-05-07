#include "vpxt_test_declarations.h"

int test_multithreaded_dec(int argc,
                           const char** argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf,
                           int artifact_detection)
{
    char *comp_out_str = "TokenPart";
    char *test_dir = "test_multithreaded_dec";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    unsigned int core_count = atoi(argv[5]);
    std::string enc_format = argv[6];
    std::string dec_format = argv[7];

    int multithreaded = 0;
    unsigned int time_0_dec;
    unsigned int time_n_dec;

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

    std::string multitread_comp_file = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(multitread_comp_file, enc_format);

    std::string multithreaded_dec_0 = cur_test_dir_str +
        slashCharStr() + test_dir + "_dec_thread_0";
    vpxt_enc_format_append(multithreaded_dec_0, dec_format);

    std::string multithreaded_dec_n = cur_test_dir_str +
        slashCharStr() + test_dir + "_dec_thread_" + argv[5];
    vpxt_enc_format_append(multithreaded_dec_n, dec_format);

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
        time_0_dec = vpxt_time_return(multithreaded_dec_0.c_str(),
            0);
        time_n_dec = vpxt_time_return(multithreaded_dec_n.c_str(),
            0);
    }
    else
    {
        if (mode == kRealTime)
        {
            if (core_count < 4)
                opt.token_partitions = core_count;
            else
                opt.token_partitions = 3;

            opt.Mode = MODE_REALTIME;
            opt.cpu_used = -1;
            unsigned int cpu_tick_1 = 0;

            if (vpxt_compress(input.c_str(), multitread_comp_file.c_str(),
                multithreaded, bitrate, opt, comp_out_str,
                opt.token_partitions, 0, enc_format) == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }
        }

        if (mode == kOnePassGoodQuality)
        {
            if (core_count < 4)
                opt.token_partitions = core_count;
            else
                opt.token_partitions = 3;

            opt.Mode = MODE_GOODQUALITY;
            unsigned int cpu_tick_1 = 0;

            if (vpxt_compress(input.c_str(), multitread_comp_file.c_str(),
                multithreaded, bitrate, opt, comp_out_str,
                opt.token_partitions, 0, enc_format) == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }
        }
    }

    unsigned int dec_cpu_tick_1 = 0;
    tprintf(PRINT_BTH, "\n\nDec Threads: 0");
    time_0_dec = vpxt_decompress_time_and_output(multitread_comp_file.c_str(),
        multithreaded_dec_0.c_str(), dec_cpu_tick_1, dec_format, 0);
    tprintf(PRINT_BTH, "\nDec time in ms: %i\n", time_0_dec);
    unsigned int DecCPUTick2 = 0;
    tprintf(PRINT_BTH, "\n\nDec Threads: %i", core_count);
    time_n_dec = vpxt_decompress_time_and_output(multitread_comp_file.c_str(),
        multithreaded_dec_n.c_str(), DecCPUTick2, dec_format,
        core_count);
    tprintf(PRINT_BTH, "\nDec time in ms: %i\n", time_n_dec);

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        // Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    tprintf(PRINT_BTH, "\n\nComparing Decompression Files: ");
    int compare_dec_output =
        vpxt_compare_dec(multithreaded_dec_0.c_str(),
        multithreaded_dec_n.c_str());
    int compare_dec = vpxt_print_compare_ivf_results(compare_dec_output, 1);

    char time_0_dec_file_name[255] = "";
    char time_n_dec_file_name[255] = "";

    vpxt_file_name(multithreaded_dec_0.c_str(), time_0_dec_file_name, 0);
    vpxt_file_name(multithreaded_dec_n.c_str(), time_n_dec_file_name, 0);

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (time_0_dec == 0 || time_n_dec == 0)
    {
        if (time_0_dec == 0)
        {
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed",
                time_0_dec_file_name, time_0_dec);
            tprintf(PRINT_BTH, "\n");
        }

        if (time_n_dec == 0)
        {
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed",
                time_n_dec_file_name, time_n_dec);
        }

        test_state = kTestFailed;
    }

    if (time_n_dec < time_0_dec)
    {
        vpxt_formated_print(RESPRT, "%s time: %u < %s time: %u - Passed",
            time_n_dec_file_name, time_n_dec, time_0_dec_file_name, time_0_dec);
        tprintf(PRINT_BTH, "\n");
    }

    if (time_0_dec == time_n_dec)
    {
        vpxt_formated_print(RESPRT, "%s time: %u == %s time: %u - Indeterminate"
            , time_0_dec_file_name, time_0_dec, time_n_dec_file_name,
            time_n_dec);
        tprintf(PRINT_BTH, "\n");

        if (test_state != kTestFailed)
            test_state = kTestIndeterminate;
    }

    if (time_n_dec > time_0_dec)
    {
        vpxt_formated_print(RESPRT, "%s time: %u > %s time: %u - Failed",
            time_n_dec_file_name, time_n_dec, time_0_dec_file_name, time_0_dec);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    // compare decodes
    if (compare_dec == 1)
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Passed",
            time_0_dec_file_name, time_n_dec_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Failed",
            time_0_dec_file_name, time_n_dec_file_name);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestIndeterminate)
        tprintf(PRINT_BTH, "\nIndeterminate\n");

    if (delete_ivf)
        vpxt_delete_files(3, multitread_comp_file.c_str(),
        multithreaded_dec_0.c_str(), multithreaded_dec_n.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}