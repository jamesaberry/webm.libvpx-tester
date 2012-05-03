#include "vpxt_test_declarations.h"

int test_reconstruct_buffer(int argc,
                            const char** argv,
                            const std::string &working_dir,
                            const std::string sub_folder_str,
                            int test_type,
                            int delete_ivf,
                            int artifact_detection)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_reconstruct_buffer";
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

    std::string recon_buffer_comp = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(recon_buffer_comp, enc_format);

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
    opt.lag_in_frames = 0;
    opt.play_alternate = 0;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        if (vpxt_compress_recon_buffer_check(input.c_str(),
            recon_buffer_comp.c_str(), speed, bitrate, opt,
            comp_out_str, 0, 0, delete_ivf, enc_format) == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }
    }

    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    std::ifstream recon_out_file;
    std::string recon_out_str;
    vpxt_remove_file_extension(recon_buffer_comp.c_str(), recon_out_str);
    recon_out_str += "ReconFrameState.txt";
    recon_out_file.open(recon_out_str.c_str());

    int frame;
    char buffer_letter;
    int state;

    while (!recon_out_file.eof())
    {
        recon_out_file >> frame;
        recon_out_file >> buffer_letter;
        recon_out_file >> state;

        if (state == 0)
        {
            vpxt_formated_print(RESPRT, "frame: %i Buffer: %c - Preview not "
                "identical to Decoded - Failed", frame, buffer_letter);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
    }

    recon_out_file.close();

    if (test_state == kTestPassed)
    {
        vpxt_formated_print(RESPRT, "All preview frames are identical to "
            "decoded frames - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        vpxt_delete_files(1, recon_buffer_comp.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}