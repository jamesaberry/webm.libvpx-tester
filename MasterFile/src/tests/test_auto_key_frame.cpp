#include "vpxt_test_declarations.h"

int test_auto_key_frame(int argc,
                        const char** argv,
                        const std::string &working_dir,
                        const std::string sub_folder_str,
                        int test_type,
                        int delete_ivf,
                        int artifact_detection)
{
    char *comp_out_str = "Auto Key Frame";
    char *test_dir = "test_auto_key_frame";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    int auto_key_frame = atoi(argv[5]);
    std::string enc_format = argv[6];

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

    std::string auto_key_frame_works_enc_1 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_1";
    vpxt_enc_format_append(auto_key_frame_works_enc_1, enc_format);

    std::string auto_key_frame_works_enc_2 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_2";
    vpxt_enc_format_append(auto_key_frame_works_enc_2, enc_format);

    std::string key_frame_txt_out_1 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_1_key_frames.txt";

    std::string key_frame_txt_out_2 = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression_2_key_frames.txt";

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

    opt.target_bandwidth = bitrate;
    opt.auto_key = 1;
    opt.key_freq = auto_key_frame;

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = mode;

        if (vpxt_compress(input.c_str(), auto_key_frame_works_enc_1.c_str(),
            speed, bitrate, opt, comp_out_str, auto_key_frame, 0, enc_format)
            == -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        if (vpxt_compress(input.c_str(), auto_key_frame_works_enc_2.c_str(),
            speed, bitrate, opt, comp_out_str, auto_key_frame, 0, enc_format)
            == -1)
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
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    vpxt_display_key_frames(auto_key_frame_works_enc_1.c_str(), 1);
    vpxt_display_key_frames(auto_key_frame_works_enc_2.c_str(), 1);

    std::ifstream key_frame_1_infile(key_frame_txt_out_1.c_str());
    std::ifstream key_frame_2_infile(key_frame_txt_out_2.c_str());

    if (!key_frame_1_infile.good())
    {
        tprintf(PRINT_BTH, "\nKey Frame File 1 Not Found: %s\n",
            key_frame_txt_out_1.c_str());
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    if (!key_frame_2_infile.good())
    {
        tprintf(PRINT_BTH, "\nKey Frame File 2 Not Found: %s\n",
            key_frame_txt_out_2.c_str());
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    int key_frame_pos_state = kTestPassed;
    int key_frame_state = kTestPassed;
    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int x;
    int y;

    while (!key_frame_1_infile.eof() && !key_frame_2_infile.eof())
    {
        key_frame_1_infile >> x;
        key_frame_2_infile >> y;

        if (x != y)
            key_frame_pos_state = kTestFailed;
    }

    if (!key_frame_1_infile.eof())
        key_frame_pos_state = kTestFailed;

    if (!key_frame_2_infile.eof())
        key_frame_pos_state = kTestFailed;

    key_frame_1_infile.close();
    key_frame_2_infile.close();

    char auto_key_frame_works_1_file_name[255];
    vpxt_file_name(auto_key_frame_works_enc_1.c_str(),
        auto_key_frame_works_1_file_name, 0);
    char auto_key_frame_works_2_file_name[255];
    vpxt_file_name(auto_key_frame_works_enc_2.c_str(),
        auto_key_frame_works_2_file_name, 0);

    ////////////////////////////////////////////////////////////////////////////

    std::ifstream infile(key_frame_txt_out_1.c_str());

    if (!infile.good())
    {
        tprintf(PRINT_BTH, "\nKey Frame File Not Present - Failed");
        fclose(fp);
        return kTestFailed;
    }

    int selector = 0;
    int do_once = 0;
    int x2;
    int y2;

    while (!infile.eof())
    {
        if (selector == 1)
        {
            infile >> x2;
            selector = 2;
        }
        else
        {
            infile >> y2;
            selector = 1;
        }

        if (do_once == 0)
        {
            x2 = 0;
            infile >> y2;
            do_once = 1;
            selector = 1;
        }

        if (vpxt_abs_int(y2 - x2) > auto_key_frame)
        {
            vpxt_formated_print(RESPRT, "Key Frames do not occur at least as "
                "frequently as Auto Key Frame dictates: %i No key frames "
                "between %i and %i - Failed", auto_key_frame, x2, y2);
            tprintf(PRINT_BTH, "\n");
            key_frame_state = kTestFailed;
        }
    }

    int max_key_frame = 0;

    if (x2 > y2)
        max_key_frame = x2;
    else
        max_key_frame = y2;

    int number_of_frames = vpxt_get_number_of_frames(
        auto_key_frame_works_enc_1.c_str());

    if (number_of_frames - 1 >= (max_key_frame + auto_key_frame))
    {
        vpxt_formated_print(RESPRT, "Key Frames do not occur at least as "
            "frequently as Auto Key Frame dictates: %i No key frames between "
            "%i and %i - Failed", auto_key_frame, max_key_frame,
            number_of_frames - 1);
        tprintf(PRINT_BTH, "\n");
        key_frame_pos_state = kTestFailed;
    }

    ///////////////////////////////////////////////////////////////////////////
    if (key_frame_state == kTestPassed)
    {
        vpxt_formated_print(RESPRT, "Key Frames occur at least as frequently as"
            " Auto Key Frame dictates: %i - Passed", auto_key_frame);
        tprintf(PRINT_BTH, "\n");
    }

    if (key_frame_pos_state == kTestPassed)
    {
        vpxt_formated_print(RESPRT, "Key Frames occur at the same locations "
            "for %s and %s - Passed", auto_key_frame_works_1_file_name,
            auto_key_frame_works_2_file_name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Key Frames do not occur at the same "
            "locations for %s and %s - Failed", auto_key_frame_works_1_file_name
            , auto_key_frame_works_2_file_name);
        tprintf(PRINT_BTH, "\n");
    }

    if(key_frame_pos_state == kTestPassed && key_frame_state == kTestPassed)
        test_state = kTestPassed;
    else
        test_state = kTestFailed;

    if(test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if(test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}