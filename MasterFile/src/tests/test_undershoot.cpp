#include "vpxt_test_declarations.h"

int test_undershoot(int argc,
                    const char** argv,
                    const std::string &working_dir,
                    const std::string sub_folder_str,
                    int test_type,
                    int delete_ivf)
{
    char *comp_out_str = "Undershoot";
    char *test_dir = "test_undershoot";
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

    std::string under_shoot_10 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_10";
    vpxt_enc_format_append(under_shoot_10, enc_format);

    std::string under_shoot_100 = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_100";
    vpxt_enc_format_append(under_shoot_100, enc_format);

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

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        opt.under_shoot_pct = 10;

        if (vpxt_compress(input.c_str(), under_shoot_10.c_str(), speed, bitrate,
            opt, comp_out_str, 10, 0, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt.under_shoot_pct = 100;

        if (vpxt_compress(input.c_str(), under_shoot_100.c_str(), speed, bitrate
            , opt, comp_out_str, 100, 0, enc_format) == -1)
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

    tprintf(PRINT_BTH, "\n\n");
    long under_shoot_10_file_sz = vpxt_file_size(under_shoot_10.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    long under_shoot_100_file_sz = vpxt_file_size(under_shoot_100.c_str(), 1);
    tprintf(PRINT_BTH, "\n");

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (under_shoot_10_file_sz < under_shoot_100_file_sz)
    {
        vpxt_formated_print(RESPRT, "File size 1:%i < File size 2:%i - Passed",
            under_shoot_10_file_sz , under_shoot_100_file_sz);

        tprintf(PRINT_BTH, "\n\nPassed\n");

        if (delete_ivf)
            vpxt_delete_files(2, under_shoot_10.c_str(),
            under_shoot_100.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }

    if (under_shoot_10_file_sz == under_shoot_100_file_sz)
    {
        vpxt_formated_print(RESPRT, "No effect try different file - "
            "Indeterminate");

        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        if (delete_ivf)
            vpxt_delete_files(2, under_shoot_10.c_str(),
            under_shoot_100.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestIndeterminate;
    }
    else
    {
        vpxt_formated_print(RESPRT, "File size 1:%i > File size 2:%i - Failed",
            under_shoot_10_file_sz , under_shoot_100_file_sz);

        tprintf(PRINT_BTH, "\n\nFailed\n");

        if (delete_ivf)
            vpxt_delete_files(2, under_shoot_10.c_str(),
            under_shoot_100.c_str());

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}