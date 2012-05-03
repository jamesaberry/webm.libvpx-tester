#include "vpxt_test_declarations.h"

int test_change_cpu_dec(int argc,
                        const char *const *argv,
                        const std::string &working_dir,
                        std::string files_ar[],
                        int test_type,
                        int delete_ivf)
{
#if defined(ARM)
    printf("\nTEST NOT SUPPORTED FOR ARM.\n");
    return TEST_FAILED;
#endif
#if defined(_PPC)
    printf("\nTEST NOT SUPPORTED FOR PPC.\n");
    return TEST_FAILED;
#else
    char *comp_out_str = "Version";
    char *test_dir = "test_change_cpu_dec";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int version_num = atoi(argv[5]);
    std::string enc_format = argv[6];
    std::string dec_format = argv[7];

    unsigned int total_ms = 0;
    unsigned int total_ms_2 = 0;

    int speed = 0;
    int fail = 0;
    int modes_run = 0;

    int simd_caps = x86_simd_caps();
    char simd_caps_orig_char[10];
    vpxt_itoa_custom(simd_caps, simd_caps_orig_char, 10);

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return TEST_ERRFM;

    std::string cpu_dec_only_enc = cur_test_dir_str + slashCharStr() + test_dir
        + "_compression";
    vpxt_enc_format_append(cpu_dec_only_enc, enc_format);

    std::string cpu_dec_only_dec = cur_test_dir_str + slashCharStr() + test_dir
        + "_decompression";

    std::string cpu_dec_only_dec_none = cpu_dec_only_dec + "_none";
    vpxt_dec_format_append(cpu_dec_only_dec_none, dec_format);
    std::string cpu_dec_only_dec_mmx = cpu_dec_only_dec + "_mmx";
    vpxt_dec_format_append(cpu_dec_only_dec_mmx, dec_format);
    std::string cpu_dec_only_dec_sse = cpu_dec_only_dec + "_sse";
    vpxt_dec_format_append(cpu_dec_only_dec_sse, dec_format);
    std::string cpu_dec_only_dec_sse2 = cpu_dec_only_dec + "_sse2";
    vpxt_dec_format_append(cpu_dec_only_dec_sse2, dec_format);
    std::string cpu_dec_only_dec_sse3 = cpu_dec_only_dec + "_sse3";
    vpxt_dec_format_append(cpu_dec_only_dec_sse3, dec_format);
    std::string cpu_dec_only_dec_ssse3 = cpu_dec_only_dec + "_ssse3";
    vpxt_dec_format_append(cpu_dec_only_dec_ssse3, dec_format);
    std::string cpu_dec_only_dec_sse4_1 = cpu_dec_only_dec + "_sse4_1";
    vpxt_dec_format_append(cpu_dec_only_dec_sse4_1, dec_format);

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
    opt.Mode = MODE_GOODQUALITY;
    opt.Version = version_num;
    int compress_int = opt.Version;
    vpxt_determinate_parameters(opt);

    int counter = 0;
    int mode_2 = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        std::vector<std::string> decompression_vector;

        if (vpxt_file_exists_check(cpu_dec_only_dec_none))
            decompression_vector.push_back(cpu_dec_only_dec_none);

        if (vpxt_file_exists_check(cpu_dec_only_dec_mmx))
            decompression_vector.push_back(cpu_dec_only_dec_mmx);

        if (vpxt_file_exists_check(cpu_dec_only_dec_sse))
            decompression_vector.push_back(cpu_dec_only_dec_sse);

        if (vpxt_file_exists_check(cpu_dec_only_dec_sse2))
            decompression_vector.push_back(cpu_dec_only_dec_sse2);

        if (vpxt_file_exists_check(cpu_dec_only_dec_sse3))
            decompression_vector.push_back(cpu_dec_only_dec_sse3);

        if (vpxt_file_exists_check(cpu_dec_only_dec_ssse3))
            decompression_vector.push_back(cpu_dec_only_dec_ssse3);

        if (vpxt_file_exists_check(cpu_dec_only_dec_sse4_1))
            decompression_vector.push_back(cpu_dec_only_dec_sse4_1);

        modes_run = decompression_vector.size();

        total_ms = vpxt_time_return(decompression_vector[0].c_str(), 1);

        unsigned int current_dec_file = 0;

        while (current_dec_file < decompression_vector.size())
        {

            total_ms_2 =
                vpxt_time_return(decompression_vector[current_dec_file].c_str(), 1);

            if (current_dec_file >= 1)
            {
                tprintf(PRINT_BTH, "\n");

                char comp_file_index_output_char[255];
                char comp_file_2[255];
                vpxt_file_name(decompression_vector[current_dec_file-1].c_str(),
                    comp_file_index_output_char, 0);
                vpxt_file_name(decompression_vector[current_dec_file].c_str(),
                    comp_file_2, 0);

                tprintf(PRINT_BTH, "\nComparing %s to %s\n",
                    comp_file_index_output_char, comp_file_2);

                int lng_rc =
                  vpxt_compare_enc(decompression_vector[current_dec_file-1].c_str(),
                  decompression_vector[current_dec_file].c_str(), 0);

                if (lng_rc >= 0)
                {
                    tprintf(PRINT_BTH, "\n * Fail: Files differ at frame: %i",
                        lng_rc);
                    fail = 1;
                }

                if (lng_rc == -1)
                    tprintf(PRINT_BTH, " * Files are identical");

                if (lng_rc == -2)
                {
                    tprintf(PRINT_BTH, "\n * Fail: File 2 ends before File 1."
                        "\n");
                    fail = 1;
                }

                if (lng_rc == -3)
                {
                    tprintf(PRINT_BTH, "\n * Fail: File 1 ends before File 2."
                        "\n");
                    fail = 1;
                }
            }

            current_dec_file++;
        }

        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        std::vector<std::string> decompression_vector;
        opt.Mode = mode;

        if (vpxt_compress(input.c_str(), cpu_dec_only_enc.c_str(), speed,
            bitrate, opt, comp_out_str, compress_int, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char, test_type);
            return TEST_INDT;
        }

        putenv("VPX_SIMD_CAPS=0");

        std::string output_2_str = cpu_dec_only_dec + "_none";
        vpxt_dec_format_append(output_2_str, dec_format);

        tprintf(PRINT_BTH, "\n\nDetected CPU capability: none");
        unsigned int cpu_tick_1 = 0;
        total_ms = vpxt_decompress_time_and_output(cpu_dec_only_enc.c_str()
            , output_2_str.c_str(), cpu_tick_1, dec_format, 1);
        decompression_vector.push_back(output_2_str);

        if (total_ms == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char, test_type);
            std::string simd_caps_str = "VPX_SIMD_CAPS=";
            simd_caps_str += simd_caps_orig_char;
            putenv((char*)simd_caps_str.c_str());
            return TEST_INDT;
        }

        tprintf(PRINT_BTH, "\n");

        int counter_max = 64;
        counter++;
        int i = 1;

        while (counter < counter_max)
        {
            int cpu_found = 0;
            int has_check = (counter + 1) / 2;

            if ((simd_caps & HAS_MMX)    == has_check)
                cpu_found = 1;

            if ((simd_caps & HAS_SSE)    == has_check)
                cpu_found = 1;

            if ((simd_caps & HAS_SSE2)   == has_check)
                cpu_found = 1;

            if ((simd_caps & HAS_SSE3)   == has_check)
                cpu_found = 1;

            if ((simd_caps & HAS_SSSE3)  == has_check)
                cpu_found = 1;

            if ((simd_caps & HAS_SSE4_1) == has_check)
                cpu_found = 1;

            if (cpu_found == 1)
            {
                std::string cpu_str;
                tprintf(PRINT_BTH, "\nDetected CPU capability: ");

                if ((simd_caps & HAS_MMX)    == has_check)
                    cpu_str = "_mmx";

                if ((simd_caps & HAS_SSE)    == has_check)
                    cpu_str = "_sse";

                if ((simd_caps & HAS_SSE2)   == has_check)
                    cpu_str = "_sse2";

                if ((simd_caps & HAS_SSE3)   == has_check)
                    cpu_str = "_sse3";

                if ((simd_caps & HAS_SSSE3)  == has_check)
                    cpu_str = "_ssse3";

                if ((simd_caps & HAS_SSE4_1) == has_check)
                    cpu_str = "_sse4_1";

                tprintf(PRINT_BTH, "%s", cpu_str.c_str());

                std::string cpu_id_str = "VPX_SIMD_CAPS=";
                char counter_char[10];
                vpxt_itoa_custom(counter, counter_char, 10);
                cpu_id_str += counter_char;

                char cpu_char[255];
                snprintf(cpu_char, 255, cpu_id_str.c_str());
                putenv(cpu_char);

                //////////////////////////////////
                ///////Compresion and time ///////

                std::string change_cpu_dec_out_file_1 = cpu_dec_only_dec;
                std::string change_cpu_dec_out_file_2 = cpu_dec_only_dec;

                char count[20];
                vpxt_itoa_custom(counter, count, 10);
                change_cpu_dec_out_file_1 += cpu_str.c_str();
                vpxt_dec_format_append(change_cpu_dec_out_file_1, dec_format);

                unsigned int cpu_tick_2 = 0;
                total_ms_2 =
                 vpxt_decompress_time_and_output(cpu_dec_only_enc.c_str(),
                 change_cpu_dec_out_file_1.c_str(), cpu_tick_2, dec_format, 1);
                decompression_vector.push_back(change_cpu_dec_out_file_1);

                if (total_ms_2 == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    std::string simd_caps_str = "VPX_SIMD_CAPS=" +
                        *simd_caps_orig_char;
                    putenv((char*)simd_caps_str.c_str());
                    return TEST_INDT;
                }

                int count_old = (counter - 1);
                vpxt_itoa_custom(count_old, count, 10);
                change_cpu_dec_out_file_2 += cpu_str.c_str();
                vpxt_dec_format_append(change_cpu_dec_out_file_2, dec_format);

                if (test_type != 2)
                {
                    char comp_file_index_output_char[255];
                    char comp_file_2[255];
                    vpxt_file_name(
                        decompression_vector[decompression_vector.size()-1].c_str(),
                        comp_file_index_output_char, 0);
                    vpxt_file_name(
                        decompression_vector[decompression_vector.size()-2].c_str(),
                        comp_file_2, 0);

                    tprintf(PRINT_BTH, "\nComparing %s to %s\n",
                        comp_file_index_output_char, comp_file_2);

                    int lng_rc =
                        vpxt_compare_dec(
                        decompression_vector[decompression_vector.size()-1].c_str(),
                        decompression_vector[decompression_vector.size()-2].c_str());

                    if (lng_rc >= 0)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: Files differ at frame: "
                            "%i on file number %i", lng_rc, i);
                        fail = 1;
                    }

                    if (lng_rc == -1)
                        tprintf(PRINT_BTH, " * Files are identical");

                    if (lng_rc == -2)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: File 2 ends before File "
                            "1.\n");
                        fail = 1;
                    }

                    if (lng_rc == -3)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: File 1 ends before File "
                            "2.\n");
                        fail = 1;
                    }
                }
            }

            counter = ((counter + 1) * 2) - 1;
            tprintf(PRINT_BTH, "\n\n");
            i++;
        }

        modes_run = decompression_vector.size();
    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        std::string simd_caps_str = "VPX_SIMD_CAPS=";
        simd_caps_str += simd_caps_orig_char;
        putenv((char*)simd_caps_str.c_str());
        return TEST_COMPM;
    }

    int over_all_fail = 0;
    tprintf(PRINT_BTH, "\nResults:\n\n");

    if (fail == 0)
    {
        vpxt_formated_print(RESPRT, "All Files Identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (fail == 1)
    {
        vpxt_formated_print(RESPRT, "All Files not Identical - Failed");
        tprintf(PRINT_BTH, "\n");
        over_all_fail = 1;
    }

    if (modes_run == 7)
    {
        vpxt_formated_print(RESPRT, "All instruction sets run - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (modes_run != 7)
    {
        vpxt_formated_print(RESPRT, "Not all instruction sets run - MinPassed");
        tprintf(PRINT_BTH, "\n");

        if (over_all_fail != 1) over_all_fail = 2;
    }

    if (total_ms != total_ms_2)
    {
        vpxt_formated_print(RESPRT, "Decompress times are not equal - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (total_ms == total_ms_2)
    {
        vpxt_formated_print(RESPRT, "CPU changes are not effecting the runtime "
            "- MinPassed");
        tprintf(PRINT_BTH, "\n");
        over_all_fail = 2;
    }

    if (over_all_fail == 2)
    {
        tprintf(PRINT_BTH, "\nMinPassed\n");
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        std::string simd_caps_str = "VPX_SIMD_CAPS=";
        simd_caps_str += simd_caps_orig_char;
        putenv((char*)simd_caps_str.c_str());
        return TEST_INDT;
    }

    if (over_all_fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(8, cpu_dec_only_enc.c_str(),
            cpu_dec_only_dec_none.c_str(), cpu_dec_only_dec_mmx.c_str(), cpu_dec_only_dec_sse.c_str(),
            cpu_dec_only_dec_sse2.c_str(), cpu_dec_only_dec_sse3.c_str(), cpu_dec_only_dec_ssse3.c_str(),
            cpu_dec_only_dec_sse4_1.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        std::string simd_caps_str = "VPX_SIMD_CAPS=";
        simd_caps_str += simd_caps_orig_char;
        putenv((char*)simd_caps_str.c_str());
        return TEST_PASSED;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(8, cpu_dec_only_enc.c_str(),
            cpu_dec_only_dec_none.c_str(), cpu_dec_only_dec_mmx.c_str(), cpu_dec_only_dec_sse.c_str(),
            cpu_dec_only_dec_sse2.c_str(), cpu_dec_only_dec_sse3.c_str(), cpu_dec_only_dec_ssse3.c_str(),
            cpu_dec_only_dec_sse4_1.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        std::string simd_caps_str = "VPX_SIMD_CAPS=";
        simd_caps_str += simd_caps_orig_char;
        putenv((char*)simd_caps_str.c_str());
        return TEST_FAILED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    std::string simd_caps_str = "VPX_SIMD_CAPS=";
    simd_caps_str += simd_caps_orig_char;
    putenv((char*)simd_caps_str.c_str());
    return TEST_ERROR;
#endif
}