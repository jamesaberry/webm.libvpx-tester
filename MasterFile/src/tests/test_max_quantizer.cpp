#include "vpxt_test_declarations.h"

int test_max_quantizer(int argc,
                       const char** argv,
                       const std::string &working_dir,
                       const std::string sub_folder_str,
                       int test_type,
                       int delete_ivf)
{
    char *comp_out_str = "Max Quantizer";
    char *test_dir = "test_max_quantizer";
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

    std::string quant_out_base = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_";

    std::string quant_out_3 = quant_out_base + "4";
    vpxt_enc_format_append(quant_out_3, enc_format);
    std::string quant_out_11 = quant_out_base + "12";
    vpxt_enc_format_append(quant_out_11, enc_format);
    std::string quant_out_19 = quant_out_base = "20";
    vpxt_enc_format_append(quant_out_19, enc_format);
    std::string quant_out_27 = quant_out_base + "28";
    vpxt_enc_format_append(quant_out_27, enc_format);
    std::string quant_out_35 = quant_out_base + "36";
    vpxt_enc_format_append(quant_out_35, enc_format);
    std::string quant_out_43 = quant_out_base + "44";
    vpxt_enc_format_append(quant_out_43, enc_format);
    std::string quant_out_51 = quant_out_base + "52";
    vpxt_enc_format_append(quant_out_51, enc_format);
    std::string quant_out_59 = quant_out_base + "60";
    vpxt_enc_format_append(quant_out_59, enc_format);

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
    // make sure constrained quantizer mode is off
    if(opt.end_usage == 2)
        opt.end_usage = 1;
    opt.target_bandwidth = bitrate;

    int n = 4;
    double psnr_arr[10];
    int max_q_arr[10];
    int i = 0;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        while (n < 63)
        {
            char num[20];
            vpxt_itoa_custom(n, num, 10);

            std::string quant_out_file = quant_out_base;
            quant_out_file += *num;
            vpxt_enc_format_append(quant_out_file, enc_format);

            tprintf(PRINT_BTH, "\n");

            if (test_type != 2)
            {
                psnr_arr[i] = vpxt_psnr(input.c_str(), quant_out_file.c_str(),
                    0, PRINT_BTH, 1, NULL);
                tprintf(PRINT_BTH, "\n");
                max_q_arr[i] = vpxt_check_max_quantizer(quant_out_file.c_str(), n);
                tprintf(PRINT_BTH, "\n");
            }

            n = n + 8;
            i++;
        }
    }
    else
    {
        while (n < 63)
        {
            opt.worst_allowed_q = n;
            // make sure min q is less than max
            while(opt.best_allowed_q > n)
                opt.best_allowed_q = rand() % 64;

            char num[20];
            vpxt_itoa_custom(n, num, 10);

            std::string quant_out_file = quant_out_base;
            quant_out_file += num;
            vpxt_enc_format_append(quant_out_file, enc_format);

            opt.Mode = mode;

            if (vpxt_compress(input.c_str(), quant_out_file.c_str(), speed,
                bitrate, opt, comp_out_str, n, 1, enc_format) == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }

            tprintf(PRINT_BTH, "\n");

            if (test_type != 2)
            {
                psnr_arr[i] = vpxt_psnr(input.c_str(), quant_out_file.c_str(),
                    0, PRINT_BTH, 1, NULL);
                tprintf(PRINT_BTH, "\n");
                max_q_arr[i] = vpxt_check_max_quantizer(quant_out_file.c_str(),
                    n);
                tprintf(PRINT_BTH, "\n");
            }

            n = n + 8;
            i++;
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

    tprintf(PRINT_BTH, "\n");

    i = 0;
    int max_q_display_value = 4;
    int fail = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    while (i < 7)
    {
        if (!(psnr_arr[i+1] <= psnr_arr[i]))
        {
            // if psnr_arr[i+1] greater than but within 1% - min pass
            if (psnr_arr[i+1] <= (psnr_arr[i] + (psnr_arr[i] * 0.01)))
            {
                vpxt_formated_print(RESPRT, "MaxQ %2i PSNR %.2f within 1%% of "
                    "MaxQ %2i PSNR %.2f - MinPassed", max_q_display_value + 8,
                    psnr_arr[i+1], max_q_display_value, psnr_arr[i]);
                tprintf(PRINT_BTH, "\n");
                fail = 2;
            }
            else
            {
                vpxt_formated_print(RESPRT, "MaxQ %2i %.2f > %.2f MaxQ %2i - "
                    "Failed", max_q_display_value + 8, psnr_arr[i+1],
                    psnr_arr[i], max_q_display_value);
                tprintf(PRINT_BTH, "\n");
                fail = 1;
            }
        }
        else
        {
            vpxt_formated_print(RESPRT, "MaxQ %2i %.2f <= %.2f MaxQ %2i - "
                "Passed", max_q_display_value + 8, psnr_arr[i+1], psnr_arr[i],
                max_q_display_value);
            tprintf(PRINT_BTH, "\n");
        }

        i++;
        max_q_display_value = max_q_display_value + 8;
    }

    i = 0;
    max_q_display_value = 4;

    while (i < 8)
    {
        if (max_q_arr[i] != -1)
        {
            vpxt_formated_print(RESPRT, "MaxQ value exceded for MaxQ %2i - "
                "frame %i - Failed", max_q_display_value, max_q_arr[i]);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "MaxQ value not exceded for MaxQ %2i "
                "- Passed", max_q_display_value, max_q_arr[i]);
            tprintf(PRINT_BTH, "\n");
        }

        max_q_display_value = max_q_display_value + 8;
        i++;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(8, quant_out_3.c_str(), quant_out_11.c_str(),
            quant_out_19.c_str(), quant_out_27.c_str(), quant_out_35.c_str(),
            quant_out_43.c_str(), quant_out_51.c_str(), quant_out_59.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }
    else if (fail == 2)
    {
        tprintf(PRINT_BTH, "\nMin Passed\n");

        if (delete_ivf)
            vpxt_delete_files(8, quant_out_3.c_str(), quant_out_11.c_str(),
            quant_out_19.c_str(), quant_out_27.c_str(), quant_out_35.c_str(),
            quant_out_43.c_str(), quant_out_51.c_str(), quant_out_59.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestMinPassed;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(8, quant_out_3.c_str(), quant_out_11.c_str(),
            quant_out_19.c_str(), quant_out_27.c_str(), quant_out_35.c_str(),
            quant_out_43.c_str(), quant_out_51.c_str(), quant_out_59.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}