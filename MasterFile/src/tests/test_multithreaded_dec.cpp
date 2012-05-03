#include "vpxt_test_declarations.h"

int test_multithreaded_dec(int argc,
                           const char *const *argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf)
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

    ////////////Formatting Test Specific directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return TEST_ERRFM;

    std::string multitread_comp_file = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(multitread_comp_file, enc_format);

    std::string multithreaded_dec_0 = cur_test_dir_str +
        slashCharStr() + test_dir + "_dec_thread_0";
    vpxt_enc_format_append(multithreaded_dec_0, dec_format);

    std::string multithreaded_dec_n = cur_test_dir_str +
        slashCharStr() + test_dir + "_dec_thread_" + argv[5];
    vpxt_enc_format_append(multithreaded_dec_n, dec_format);

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

    if (!(core_count > 1))
    {
        tprintf(PRINT_STD, "\nMultiple Cores not used Test aborted: %i\n",
            core_count);
        fclose(fp);
        return TEST_FAILED;
    }

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
    opt.end_usage = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        time_0_dec = vpxt_time_return(multithreaded_dec_0.c_str(),
            0);
        time_n_dec = vpxt_time_return(multithreaded_dec_n.c_str(),
            0);
    }
    else
    {
        if (mode == 0)
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
                return TEST_INDT;
            }
        }

        if (mode == 1)
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
                return TEST_INDT;
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

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
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

    int fail = 0;
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

        fail = 1;
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

        if (fail != 1)
            fail = 2;
    }

    if (time_n_dec > time_0_dec)
    {
        vpxt_formated_print(RESPRT, "%s time: %u > %s time: %u - Failed",
            time_n_dec_file_name, time_n_dec, time_0_dec_file_name, time_0_dec);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    //compare decodes
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
        fail = 1;
    }


    //final evaluate
    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(3, multitread_comp_file.c_str(),
            multithreaded_dec_0.c_str(),
            multithreaded_dec_n.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(3, multitread_comp_file.c_str(),
            multithreaded_dec_0.c_str(),
            multithreaded_dec_n.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }

    if (fail == 3)
    {
        tprintf(PRINT_BTH, "\nIndeterminate\n");

        if (delete_ivf)
            vpxt_delete_files(3, multitread_comp_file.c_str(),
            multithreaded_dec_0.c_str(),
            multithreaded_dec_n.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_INDT;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}