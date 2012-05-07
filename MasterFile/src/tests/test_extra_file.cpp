#include "vpxt_test_declarations.h"

int test_extra_file(int argc,
                    const char** argv,
                    const std::string &working_dir,
                    const std::string sub_folder_str,
                    int test_type,
                    int delete_ivf,
                    int artifact_detection)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_extra_file";
    int bitrate = 128;
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    std::string enc_format = argv[3];

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    //////// Compression Directory ////////
    std::string compression_dir = cur_test_dir_str + slashCharStr();
    std::string compression_dir_all_files = compression_dir;
#if defined(_WIN32)
    compression_dir_all_files += "*";
#endif

    //////// Tester Directory ////////
    std::string base_dir;
    vpxt_folder_name(argv[0], &base_dir);
    std::string tester_dir_all_files = base_dir;
#if defined(_WIN32)
    tester_dir_all_files += "*";
#endif

    //////// Current Directory ////////
    std::string current_dir;
    vpxt_get_cur_dir(current_dir);
    std::string current_dir_all_files = current_dir + slashCharStr();
#if defined(_WIN32)
    current_dir_all_files += "*";
#endif

    ////////////////////////////////////////////////////////////////

    std::string extra_file_check_str = cur_test_dir_str + slashCharStr() +
        test_dir + "_compression";
    vpxt_enc_format_append(extra_file_check_str, enc_format);

    // This is to record state for compression only run
    std::string extra_file_check_results_str = cur_test_dir_str + slashCharStr()
        + test_dir + "_result.txt";

    ///////////// Open Output File and Print Header ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;
    FILE *fp;

    vpxt_open_output_file(test_type, text_file_str, fp);
    vpxt_print_header(argc, argv, main_test_dir_char, cur_test_dir_str,
        test_dir, test_type);
    tprintf(PRINT_BTH, "\n\n---------------------------Checking for OPSNR Files"
        "----------------------------\n\n");

    ///////////////////////// Delete opsnr.stt /////////////////////////////////

    // check Current directory
    std::string current_dir_opsnr = current_dir + slashCharStr();

    tprintf(PRINT_BTH, "Checking: %s For opsnr.stt - ",
        current_dir_opsnr.c_str());
    current_dir_opsnr += "opsnr.stt";

    FILE *file_current_dir_opsnr = fopen(current_dir_opsnr.c_str(), "rb");

    if (file_current_dir_opsnr == NULL)
        tprintf(PRINT_BTH, "opsnr.stt File not found. \n\n");
    else
    {
        fclose(file_current_dir_opsnr);
        remove(current_dir_opsnr.c_str());
        tprintf(PRINT_BTH, "opsnr.stt File found and deleted. \n");
    }

    // check Tester Dirctory
    std::string tester_dir_opsnr = base_dir;
    tprintf(PRINT_BTH, "Checking: %s For opsnr.stt - ",
        tester_dir_opsnr.c_str());
    tester_dir_opsnr += "opsnr.stt";

    FILE *file_tester_opsnr = fopen(tester_dir_opsnr.c_str(), "rb");

    if (file_tester_opsnr == NULL)
        tprintf(PRINT_BTH, "opsnr.stt File not found. \n\n");
    else
    {
        fclose(file_tester_opsnr);
        remove(tester_dir_opsnr.c_str());
        tprintf(PRINT_BTH, "opsnr.stt File found and deleted. \n");
    }

    // check Compression directory
    std::string comp_dir_opsnr = compression_dir;
    tprintf(PRINT_BTH, "Checking: %s For opsnr.stt - ", comp_dir_opsnr.c_str());
    comp_dir_opsnr += "opsnr.stt";

    FILE *file_comp_dir_opsnr = fopen(comp_dir_opsnr.c_str(), "rb");

    if (file_comp_dir_opsnr == NULL)
        tprintf(PRINT_BTH, "opsnr.stt File not found. \n\n");
    else
    {
        fclose(file_comp_dir_opsnr);
        remove(comp_dir_opsnr.c_str());
        tprintf(PRINT_BTH, "opsnr.stt File found and deleted. \n");
    }

    tprintf(PRINT_BTH, "-------------------------------------------------------"
        "-----------------------\n");
    ////////////////////////////////////////////////////////////////

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    /////////////////// Use Custom Settings ///////////////////
    if(vpxt_use_custom_settings(argv, argc, input_ver, fp, file_index_str,
        file_index_output_char, test_type, opt, bitrate)
        == kTestIndeterminate)
        return kTestIndeterminate;

    int compress_int = opt.allow_df;
    bitrate = opt.target_bandwidth;
    int test_state = kTestPassed;

    // Record all files in the executable directory
    std::vector<std::string> ignore_tester_dir;
    vpxt_add_dir_files_to_ignore(ignore_tester_dir, tester_dir_all_files);

    // Record all files in the current directory.
    std::vector<std::string> ignore_current_dir;
    vpxt_add_dir_files_to_ignore(ignore_current_dir, current_dir_all_files);

    // There are no files in comp dir but we need to ignore compression files
    std::vector<std::string> ignore_compression_dir;
    ignore_compression_dir.push_back(".");
    ignore_compression_dir.push_back("..");
    ignore_compression_dir.push_back("test_extra_file.txt");
    ignore_compression_dir.push_back("test_extra_file_compression.ivf");
    ignore_compression_dir.push_back("test_extra_file_compression.webm");
    ignore_compression_dir.push_back(
        "test_extra_file_compression_parameters_core.txt");
    ignore_compression_dir.push_back(
        "test_extra_file_compression_parameters_vpx.txt");

    if (test_type == kTestOnly)
    {
        tprintf(PRINT_BTH, "\nExtraFileCheck Test Run Previously Retreiveing "
            "Result\n");

        std::ifstream infile(extra_file_check_results_str.c_str());

        if (!infile.is_open())
        {
            tprintf(PRINT_BTH, "File: %s not opened",
                extra_file_check_results_str.c_str());
            return kTestFailed;
        }

        infile >> test_state;
        infile.close();

        if (test_state == kTestFailed)
            tprintf(PRINT_BTH, "\n\nExtra files found - Test Failed\n");
        if (test_state == kTestPassed)
            tprintf(PRINT_BTH, "\n\n No extra files found - Test Passed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char,
            test_type);
        return test_state;
    }
    else
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (vpxt_compress(input.c_str(), extra_file_check_str.c_str(),
            opt.multi_threaded, bitrate, opt, comp_out_str, 1, 0, enc_format) ==
            -1)
        {
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }
    }

    tprintf(PRINT_BTH, "\n\n---------------------------Checking for Extra Files"
        "----------------------------\n\n");
    tprintf(PRINT_BTH, "Checking: %s", compression_dir_all_files.c_str());
    tprintf(PRINT_BTH, "\n\nChecking: %s", tester_dir_all_files.c_str());
    tprintf(PRINT_BTH, "\n\nChecking: %s\n\n", current_dir_all_files.c_str());

    std::vector<std::string> found_tester_dir;
    std::vector<std::string> found_current_dir;
    std::vector<std::string> found_compression_dir;

    int cur_dir_fail = vpxt_find_non_ignored_files_in_dir(ignore_current_dir,
        found_current_dir, current_dir_all_files);
    int comp_dir_fail = vpxt_find_non_ignored_files_in_dir(
        ignore_compression_dir, found_compression_dir,
        compression_dir_all_files);
    int tester_dir_fail = vpxt_find_non_ignored_files_in_dir(ignore_tester_dir,
        found_tester_dir, tester_dir_all_files);

    tprintf(PRINT_BTH, "-------------------------------------------------------"
        "-----------------------\n");
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (test_type == kCompOnly)
    {
        std::ofstream outfile(extra_file_check_results_str.c_str());

        if (cur_dir_fail == 0 && comp_dir_fail == 0 && tester_dir_fail == 0)
        {
            tprintf(PRINT_BTH, "\nNo extra files found\n");
            outfile << 0;
        }
        else
        {
            tprintf(PRINT_BTH, "\nExtra files found\n");
            outfile << 1;
        }

        outfile.close();
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    if (tester_dir_fail == 0)
    {
        vpxt_formated_print(RESPRT, "No extra files found in tester directory "
            "- Passed\n");
    }
    else
    {
        unsigned int vecpos = 0;

        while (vecpos < found_tester_dir.size())
        {
            vpxt_formated_print(RESPRT, "Tester directory extra file found: %s "
                "- Failed\n", found_tester_dir[vecpos].c_str());
            vecpos++;
        }

        test_state = kTestFailed;
    }

    if (cur_dir_fail == 0)
    {
        vpxt_formated_print(RESPRT, "No extra files found in current directory "
            "- Passed\n");
    }
    else
    {
        unsigned int vecpos = 0;

        while (vecpos < found_current_dir.size())
        {
            vpxt_formated_print(RESPRT, "Current directory extra file found: "
                "%s - Failed\n", found_current_dir[vecpos].c_str());
            vecpos++;
        }

        test_state = kTestFailed;
    }

    if (comp_dir_fail == 0)
    {
        vpxt_formated_print(RESPRT, "No extra files found in compression "
            "directory - Passed\n");
    }
    else
    {
        unsigned int vecpos = 0;

        while (vecpos < found_compression_dir.size())
        {
            vpxt_formated_print(RESPRT, "Compression directory extra file "
                "found: %s - Failed\n", found_compression_dir[vecpos].c_str());
            vecpos++;
        }

        test_state = kTestFailed;
    }

    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");

    if (delete_ivf)
        vpxt_delete_files(1, extra_file_check_str.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;

}