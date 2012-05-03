#include "vpxt_test_declarations.h"

int test_change_cpu_enc(int argc,
                        const char** argv,
                        const std::string &working_dir,
                        const std::string sub_folder_str,
                        int test_type,
                        int delete_ivf,
                        int artifact_detection)
{
#if defined(ARM)
    printf("\nTEST NOT SUPPORTED FOR ARM.\n");
    return kTestFailed;
#endif
#if defined(_PPC)
    printf("\nTEST NOT SUPPORTED FOR PPC.\n");
    return kTestFailed;
#else
    char *comp_out_str = "Arnr Maxframes:";
    char *test_dir = "test_change_cpu_enc";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int arnr_max_frames = atoi(argv[5]);
    std::string enc_format = argv[6];
    std::string dec_format = argv[7];

    int speed = 0;
    int fail = 0;
    int modes_run = 0;

    unsigned int cpu_tick_1 = 0;
    unsigned int cpu_tick_2 = 0;

    int simd_caps = x86_simd_caps();
    char simd_caps_orig_char[10];
    vpxt_itoa_custom(simd_caps, simd_caps_orig_char, 10);

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string change_cpu_dec_0_enc = cur_test_dir_str + slashCharStr() +
        "test_change_cpu_enc_compression_none";
    vpxt_enc_format_append(change_cpu_dec_0_enc, enc_format);

    std::string change_cpu_dec_n_base = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_";

    std::string change_cpu_enc_none = change_cpu_dec_n_base + "none";
    vpxt_enc_format_append(change_cpu_enc_none, enc_format);
    std::string change_cpu_enc_mmx = change_cpu_dec_n_base + "mmx";
    vpxt_enc_format_append(change_cpu_enc_mmx, enc_format);
    std::string change_cpu_enc_sse = change_cpu_dec_n_base + "sse";
    vpxt_enc_format_append(change_cpu_enc_sse, enc_format);
    std::string change_cpu_enc_sse2 = change_cpu_dec_n_base + "sse2";
    vpxt_enc_format_append(change_cpu_enc_sse2, enc_format);
    std::string change_cpu_enc_sse3 = change_cpu_dec_n_base + "sse3";
    vpxt_enc_format_append(change_cpu_enc_sse3, enc_format);
    std::string change_cpu_enc_ssse3 = change_cpu_dec_n_base + "ssse3";
    vpxt_enc_format_append(change_cpu_enc_ssse3, enc_format);
    std::string change_cpu_enc_sse4_1 = change_cpu_dec_n_base + "sse4_1";
    vpxt_enc_format_append(change_cpu_enc_sse4_1, enc_format);

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
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = bitrate;
    int compress_int = arnr_max_frames;
    opt.Mode = mode;
    vpxt_determinate_parameters(opt);

    int counter = 0;

    putenv("VPX_SIMD_CAPS=0");

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        std::vector<std::string> compression_vector;

        if (vpxt_file_exists_check(change_cpu_enc_none))
            compression_vector.push_back(change_cpu_enc_none);

        if (vpxt_file_exists_check(change_cpu_enc_mmx))
            compression_vector.push_back(change_cpu_enc_mmx);

        if (vpxt_file_exists_check(change_cpu_enc_sse))
            compression_vector.push_back(change_cpu_enc_sse);

        if (vpxt_file_exists_check(change_cpu_enc_sse2))
            compression_vector.push_back(change_cpu_enc_sse2);

        if (vpxt_file_exists_check(change_cpu_enc_sse3))
            compression_vector.push_back(change_cpu_enc_sse3);

        if (vpxt_file_exists_check(change_cpu_enc_ssse3))
            compression_vector.push_back(change_cpu_enc_ssse3);

        if (vpxt_file_exists_check(change_cpu_enc_sse4_1))
            compression_vector.push_back(change_cpu_enc_sse4_1);

        modes_run = compression_vector.size();

        cpu_tick_1 = vpxt_cpu_tick_return(compression_vector[0].c_str(), 0);

        unsigned int current_file = 0;

        while (current_file < compression_vector.size())
        {
            cpu_tick_2 =
                vpxt_cpu_tick_return(compression_vector[current_file].c_str(),
                0);

            if (current_file >= 1)
            {
                tprintf(PRINT_BTH, "\n");

                char comp_file_index_output_char[255];
                char comp_file_2[255];
                vpxt_file_name(compression_vector[current_file-1].c_str(),
                    comp_file_index_output_char, 0);
                vpxt_file_name(compression_vector[current_file].c_str(),
                    comp_file_2, 0);

                tprintf(PRINT_BTH, "\nComparing %s to %s\n",
                    comp_file_index_output_char, comp_file_2);

                int lng_rc =
                    vpxt_compare_enc(compression_vector[current_file-1].c_str(),
                    compression_vector[current_file].c_str(), 0);

                if (lng_rc >= 0)
                {
                    tprintf(PRINT_BTH, "\n * Fail: Files differ at frame: "
                        "%i", lng_rc);
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

            current_file++;
        }

        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        std::vector<std::string> compression_vector;

        putenv("VPX_SIMD_CAPS=0");
        std::string output_str = change_cpu_dec_n_base + "none";
        vpxt_enc_format_append(output_str, enc_format);

        opt.arnr_max_frames = arnr_max_frames;

        tprintf(PRINT_BTH, "\n\nDetected CPU capability: NONE");
        unsigned int time_1 = vpxt_time_compress(input.c_str(),
            output_str.c_str(), speed, bitrate, opt, comp_out_str, compress_int,
            0, cpu_tick_1, enc_format);
        compression_vector.push_back(output_str);

        if (time_1 == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            std::string simd_caps_str = "VPX_SIMD_CAPS=";
            simd_caps_str += simd_caps_orig_char;
            putenv((char*)simd_caps_str.c_str());
            return kTestIndeterminate;
        }

        tprintf(PRINT_BTH, "\n");

        int counter_max = 64;
        int file_number = 1;
        counter++;

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
                    cpu_str = "mmx";

                if ((simd_caps & HAS_SSE)    == has_check)
                    cpu_str = "sse";

                if ((simd_caps & HAS_SSE2)   == has_check)
                    cpu_str = "sse2";

                if ((simd_caps & HAS_SSE3)   == has_check)
                    cpu_str = "sse3";

                if ((simd_caps & HAS_SSSE3)  == has_check)
                    cpu_str = "ssse3";

                if ((simd_caps & HAS_SSE4_1) == has_check)
                    cpu_str = "sse4_1";

                tprintf(PRINT_BTH, "%s", cpu_str.c_str());

                ///////////Updating CPU///////////
                std::string cpu_id_string = "VPX_SIMD_CAPS=";
                char counter_char[10];
                vpxt_itoa_custom(counter, counter_char, 10);
                cpu_id_string += counter_char;

                char cpu_char[255];
                snprintf(cpu_char, 255, cpu_id_string.c_str());
                putenv(cpu_char);

                /////// Compresion and time ///////
                std::string changed_cpu_dec_n_out_current =
                    change_cpu_dec_n_base;
                std::string changed_cpu_dec_n_out_last = change_cpu_dec_n_base;

                char count[20];
                vpxt_itoa_custom(counter, count, 10);
                changed_cpu_dec_n_out_current += cpu_str.c_str();
                vpxt_enc_format_append(changed_cpu_dec_n_out_current,
                    enc_format);

                opt.Mode = mode;
                unsigned int time_2 = vpxt_time_compress(input.c_str(),
                    changed_cpu_dec_n_out_current.c_str(), speed, bitrate, opt,
                    comp_out_str, compress_int, 0, cpu_tick_2, enc_format);
                compression_vector.push_back(changed_cpu_dec_n_out_current);

                if (time_2 == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    std::string simd_caps_str = "VPX_SIMD_CAPS=" +
                        *simd_caps_orig_char;
                    putenv((char*)simd_caps_str.c_str());
                    return kTestIndeterminate;
                }

                if (test_type != 2 && counter != 0)
                {
                    char comp_file_index_output_char[255];
                    vpxt_file_name(
                        compression_vector[compression_vector.size()-1].c_str(),
                        comp_file_index_output_char, 0);

                    char comp_file_2[255];
                    vpxt_file_name(
                        compression_vector[compression_vector.size()-2].c_str(),
                        comp_file_2, 0);

                    tprintf(PRINT_BTH, "\nComparing %s to %s\n",
                        comp_file_index_output_char, comp_file_2);

                    int lng_rc =
                        vpxt_compare_enc(
                        compression_vector[compression_vector.size()-1].c_str(),
                        compression_vector[compression_vector.size()-2].c_str(),
                        0);

                    if (lng_rc >= 0)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: Files differ at frame: "
                            "%i on file number %i\n", lng_rc, file_number);
                        fail = 1;
                    }

                    if (lng_rc == -1)
                        tprintf(PRINT_BTH, " * Files are identical\n");

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
            tprintf(PRINT_BTH, "\n");
            file_number++;
        }

        modes_run = compression_vector.size();
    }

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        std::string simd_caps_str = "VPX_SIMD_CAPS=";
        simd_caps_str += simd_caps_orig_char;
        putenv((char*)simd_caps_str.c_str());
        return kTestEncCreated;
    }

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (fail != 1)
    {
        vpxt_formated_print(RESPRT, "All Files are identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (fail == 1)
    {
        vpxt_formated_print(RESPRT, "All Files are not identical - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
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

        if (test_state != kTestFailed)
            test_state = kTestMinPassed;
    }

    if (cpu_tick_1 == cpu_tick_2)
    {
        vpxt_formated_print(RESPRT, "cpu_tick_1: %u == cpu_tick_2: %u - "
            "MinPassed" , cpu_tick_1, cpu_tick_2);
        tprintf(PRINT_BTH, "\n");
        test_state = kTestMinPassed;
    }

    if (cpu_tick_1 != cpu_tick_2)
    {
        vpxt_formated_print(RESPRT, "cpu_tick_1: %u != cpu_tick_2: %u - Passed",
            cpu_tick_1, cpu_tick_2);
        tprintf(PRINT_BTH, "\n");
    }

    if (test_state == kTestMinPassed)
        tprintf(PRINT_BTH, "\nMinPassed\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(8, change_cpu_dec_0_enc.c_str(),
        change_cpu_enc_none.c_str(), change_cpu_enc_mmx.c_str(),
        change_cpu_enc_sse.c_str(), change_cpu_enc_sse2.c_str(),
        change_cpu_enc_sse3.c_str(), change_cpu_enc_ssse3.c_str(),
        change_cpu_enc_sse4_1.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    std::string simd_caps_str = "VPX_SIMD_CAPS=";
    simd_caps_str += simd_caps_orig_char;
    putenv((char*)simd_caps_str.c_str());
    return test_state;
#endif
}