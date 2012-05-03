#include "vpxt_test_declarations.h"

int test_fixed_quantizer(int argc,
                         const char *const *argv,
                         const std::string &working_dir,
                         std::string files_ar[],
                         int test_type,
                         int delete_ivf)
{
    char *comp_out_str = "Fixed Quantizer";
    char *test_dir = "test_fixed_quantizer";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int fixed_q_1 = atoi(argv[5]);
    int fixed_q_2 = atoi(argv[6]);
    std::string enc_format = argv[7];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return TEST_ERRFM;

    std::string fixed_q_1_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_" + argv[5];
    vpxt_enc_format_append(fixed_q_1_enc, enc_format);

    std::string fixed_q_2_enc = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_" + argv[6];
    vpxt_enc_format_append(fixed_q_2_enc, enc_format);

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
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    opt.target_bandwidth = bitrate;
    opt.fixed_q = 1;

    if (fixed_q_1 < 0 || fixed_q_1 > 63 || fixed_q_2 < 0 || fixed_q_2 > 63)
    {
        tprintf(PRINT_BTH, "\n\nInvaild Qunatizer Range, Vaild range = 0-63 - "
            "Indeterminate\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_MINPA;
    }

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (test_type == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        opt.fixed_q = fixed_q_1;

        if (vpxt_compress(input.c_str(), fixed_q_1_enc.c_str(), speed, bitrate,
            opt, comp_out_str, fixed_q_1, 1, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }

        opt.fixed_q = fixed_q_2;

        if (vpxt_compress(input.c_str(), fixed_q_2_enc.c_str(), speed, bitrate,
            opt, comp_out_str, fixed_q_2, 1, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return TEST_INDT;
        }
    }

    //Create Compression only stop test short.
    if (test_type == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_COMPM;
    }

    tprintf(PRINT_BTH, "\n");

    int fail = 0;

    tprintf(PRINT_BTH, "\n");
    int fixed_q_check_val_1 = vpxt_check_fixed_quantizer(fixed_q_1_enc.c_str(),
        fixed_q_1);
    tprintf(PRINT_BTH, "\n");
    int fixed_q_check_val_2 = vpxt_check_fixed_quantizer(fixed_q_2_enc.c_str(),
        fixed_q_2);
    tprintf(PRINT_BTH, "\n");

    tprintf(PRINT_BTH, "\n");
    int fixed_q_size_1 = vpxt_file_size(fixed_q_1_enc.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    int fixed_q_size_2 = vpxt_file_size(fixed_q_2_enc.c_str(), 1);
    tprintf(PRINT_BTH, "\n");

    char fixed_q_1_file_name[255] = "";
    char fixed_q_2_file_name[255] = "";

    vpxt_file_name(fixed_q_1_enc.c_str(), fixed_q_1_file_name, 0);
    vpxt_file_name(fixed_q_2_enc.c_str(), fixed_q_2_file_name, 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (fixed_q_check_val_1 != -1)
    {
        vpxt_formated_print(RESPRT, "Quantizer not fixed for %s for frame %i -"
            " Failed", fixed_q_1_file_name, fixed_q_check_val_1);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Quantizer fixed at %i for all frames for "
            "%s - Passed", fixed_q_1, fixed_q_1_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if (fixed_q_check_val_2 != -1)
    {
        vpxt_formated_print(RESPRT, "Quantizer not fixed for %s for frame %i - "
            "Failed", fixed_q_2_file_name, fixed_q_check_val_2);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Quantizer fixed at %i for all frames for "
            "%s - Passed", fixed_q_2, fixed_q_2_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    //make sure that lower fixed q has higher datarate
    if (fixed_q_1 < fixed_q_2)
    {

        if (fixed_q_size_1 <= fixed_q_size_2)
        {
            vpxt_formated_print(RESPRT, "%s file size: %i >= %s file size: %i "
                "- Failed", fixed_q_2_file_name, fixed_q_size_2,
                fixed_q_1_file_name, fixed_q_size_1);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "%s file size: %i > %s file size: %i "
                "- Passed", fixed_q_1_file_name, fixed_q_size_1,
                fixed_q_2_file_name, fixed_q_size_2);
            tprintf(PRINT_BTH, "\n");
        }
    }
    else
    {
        if (fixed_q_size_2 <= fixed_q_size_1)
        {
            vpxt_formated_print(RESPRT, "%s file size: %i >= %s file size: %i "
                "- Failed", fixed_q_1_file_name, fixed_q_size_1,
                fixed_q_2_file_name, fixed_q_size_2);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "%s file size: %i > %s file size: %i "
                "- Failed", fixed_q_2_file_name, fixed_q_size_2,
                fixed_q_1_file_name, fixed_q_size_1);
            tprintf(PRINT_BTH, "\n");
        }
    }

    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(2, fixed_q_1_enc.c_str(), fixed_q_2_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_FAILED;
    }
    else
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(2, fixed_q_1_enc.c_str(), fixed_q_2_enc.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return TEST_PASSED;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return TEST_ERROR;
}