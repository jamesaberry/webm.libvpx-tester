#include "vpxt_test_declarations.h"

int test_new_vs_old_enc_cpu_tick(int argc,
                                 const char** argv,
                                 const std::string &working_dir,
                                 const std::string sub_folder_str,
                                 int test_type,
                                 int delete_ivf,
                                 int artifact_detection)
{
    char *test_dir = "test_new_vs_old_enc_cpu_tick";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string old_file_name = argv[5];
    int par_file_num = atoi(argv[6]);
    std::string enc_format = argv[7];

    int speed = 0;
    unsigned int cpu_tick_new = 0;
    unsigned int cpu_tick_old = 0;

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string exe_dir_str;
    vpxt_folder_name(argv[0], &exe_dir_str);
    std::string exe_str = exe_dir_str;

    char old_exe_full_path[256];
    snprintf(old_exe_full_path, 255, "%s%s", exe_dir_str.c_str(), argv[5]);

    std::string new_enc_file = cur_test_dir_str;
    new_enc_file += slashCharStr();
    new_enc_file += test_dir;
    new_enc_file += "_compression_new";
    vpxt_enc_format_append(new_enc_file, enc_format);

    std::string old_enc_file = cur_test_dir_str;
    old_enc_file += slashCharStr();
    old_enc_file += test_dir;
    old_enc_file += "_compression_old";
    vpxt_enc_format_append(old_enc_file, enc_format);

    std::string par_file = cur_test_dir_str;
    par_file += slashCharStr();
    par_file += test_dir;
    par_file += "_parameter_file.txt";

    std::string comand_line_str = exe_str;
    comand_line_str += old_file_name;

#if defined(_WIN32)
    {
        comand_line_str.insert(0, "\"");
        comand_line_str.insert(0, "\"");
        comand_line_str += "\" compress \"";
        comand_line_str += input.c_str();
        comand_line_str += "\" \"";
        comand_line_str += old_enc_file;
        comand_line_str += "\" 8";
        comand_line_str += " \"";
        comand_line_str += par_file;
        comand_line_str += "\"";
        comand_line_str += " 2";
        comand_line_str += "\"";
    }
#else
    {
        comand_line_str.insert(0, "\'./");
        comand_line_str += "\' compress \'";
        comand_line_str += input.c_str();
        comand_line_str += "\' \'";
        comand_line_str += old_enc_file;
        comand_line_str += "\' 8";
        comand_line_str += " \'";
        comand_line_str += par_file;
        comand_line_str += "\'";
        comand_line_str += " 2";
    }
#endif

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

    int indeterminate = 0;
    int test_state = kTestPassed;

    char git_log_input[256];
    char test_log_input[256];
    char test_log_output[256];

    snprintf(test_log_input, 255, "%s%s", exe_dir_str.c_str(),
        "test_new_vs_old_enc_cpu_tick-log.txt");
    snprintf(git_log_input, 255, "%s%s", exe_dir_str.c_str(),
        "libvpx-git-log.txt");
    snprintf(test_log_output, 255, "%s%s", exe_dir_str.c_str(),
        "test_new_vs_old_enc_cpu_tick-log-sync.txt");

    // check to see if git-log.txt and new-vs-old-time-log exist.
    // If so use new method else use old.
    if (vpxt_file_exists_check(git_log_input) &&
        vpxt_file_exists_check(test_log_input))
    {
        tprintf(PRINT_BTH, "\n Using Log\n");

        // Make New Compression get time.
        opt.target_bandwidth = bitrate;
        opt.auto_key = 1;
        opt.cpu_used = -4;

        // Run Test only (Runs Test, Sets up test to be run, or skips compresion
        // of files)
        if (test_type == kTestOnly)
        {
            cpu_tick_new = vpxt_cpu_tick_return(new_enc_file.c_str(), 0);
            cpu_tick_old = vpxt_cpu_tick_return(old_enc_file.c_str(), 0);

            tprintf(PRINT_BTH, "\ncpu_tick_new: %i\n", cpu_tick_new);
            tprintf(PRINT_BTH, "\ncpu_tick_old: %i\n", cpu_tick_old);
        }
        else
        {
            opt.Mode = mode;
            unsigned int time = vpxt_time_compress(input.c_str(),
                new_enc_file.c_str(), speed, bitrate, opt, "VP8", 0, 0,
                cpu_tick_new, enc_format);

            if (time == -1 || time == 0)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }
        }

        char cpu_tick_new_char[256];
        sprintf(cpu_tick_new_char, "%u", cpu_tick_new);

        std::string time_result_str;
        time_result_str += cpu_tick_new_char;
        time_result_str += " ms";

        int arg_parse = 1;
        std::string command_line_input_str;

        // assemble command line input string
        while (arg_parse < argc){
            if (arg_parse != 1)
                command_line_input_str += " ";
            command_line_input_str += argv[arg_parse];
            ++arg_parse;
        }

        if (command_line_input_str.substr(0, 1).compare(" ") == 0)
            command_line_input_str.erase(command_line_input_str.begin(),
            command_line_input_str.begin() + 1);

        printf("\n\nARG STR:%s\n\n", command_line_input_str.c_str());

        if (vpxt_init_new_vs_old_log(test_log_input, command_line_input_str)
            != 1)
        {
            tprintf(PRINT_BTH, "\nNUMBER OF UNIQUE IDS NOT EQUAL 1: TEST "
                "ABORTED\n");
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        std::vector<double> raw_data_list;

        int sync_fail = vpxt_sync_new_vs_old_log(test_log_input, git_log_input,
            test_log_output, time_result_str.c_str(), command_line_input_str,
            "test_new_vs_old_enc_cpu_tick");

        if(!sync_fail)
        {
            tprintf(PRINT_BTH, "\n\n---------------------COMMIT-TIME-LOG-------"
                "--------------\n\n");
            vpxt_eval_new_vs_old_log(test_log_output, command_line_input_str, 1,
                raw_data_list
                , "test_new_vs_old_enc_cpu_tick");
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "------------\n");
        }
        else
        {
            tprintf(PRINT_BTH, "\nFAILED TO SYNC NEW VS OLD LOG - TEST ABORTED"
                "\n");
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        tprintf(PRINT_BTH, "\n\n\nResults:\n\n");

        if (raw_data_list.size() < 2)
        {
            vpxt_formated_print(RESPRT, "New speed: %.2u Old speed: Not Found -"
            " Indeterminate", raw_data_list[0]);
            indeterminate = 1;
        }
        else
        {
            if(raw_data_list[0] < raw_data_list[1])
                vpxt_formated_print(RESPRT, "New: %.0f is faster than Old: %.0f"
                " - Passed", raw_data_list[0], raw_data_list[1]);
            else if(raw_data_list[0] == raw_data_list[1])
                vpxt_formated_print(RESPRT, "Files took the same amount of time"
                " - Passed", raw_data_list[0], raw_data_list[1]);
            else if(raw_data_list[0] < (raw_data_list[1] +
                (raw_data_list[1] * .05)))
                vpxt_formated_print(RESPRT, "New: %.0f is with in five percent"
                " of Old: %.0f - Passed", raw_data_list[0], raw_data_list[1]);
            else{
                vpxt_formated_print(RESPRT, "New: %.0f is slower than Old: %.0f"
                " - Failed",raw_data_list[0], raw_data_list[1]);
                test_state = kTestFailed;
            }
        }

        vpxt_delete_files_quiet(1, test_log_input);
        vpxt_copy_file(test_log_output, test_log_input);
        vpxt_delete_files_quiet(1, test_log_output);
    }
    else
    {

        ///////////////// Make Sure Exe File Exists ///////////////
        if (!vpxt_file_exists_check(old_exe_full_path))
        {
            tprintf(PRINT_BTH, "\nInput executable %s does not exist\n",
                argv[5]);

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        /////////////////////////////////////////////////////////

        tprintf(PRINT_BTH, "\n Using Exe\n");

        opt.target_bandwidth = bitrate;
        opt.auto_key = 1;
        opt.cpu_used = -4;

        // Run Test only (Runs Test, Sets up test to be run, or skips compresion
        // of files)
        if (test_type == kTestOnly)
        {
            cpu_tick_new = vpxt_cpu_tick_return(new_enc_file.c_str(), 0);
            cpu_tick_old = vpxt_cpu_tick_return(old_enc_file.c_str(), 0);

            tprintf(PRINT_BTH, "\ncpu_tick_new: %i\n", cpu_tick_new);
            tprintf(PRINT_BTH, "\ncpu_tick_old: %i\n", cpu_tick_old);
        }
        else
        {
            opt.Mode = mode;
            unsigned int time = vpxt_time_compress(input.c_str(),
                new_enc_file.c_str(), speed, bitrate, opt, "VP8", 0, 0,
                cpu_tick_new, enc_format);

            if (time == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }

            vpxt_output_compatable_settings(par_file.c_str(), opt,par_file_num);
            tprintf(PRINT_STD, "\nCompressing Old File\n");

            fclose(fp);

            if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) == NULL)
            {
                tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                    text_file_str.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            vpxt_run_exe(comand_line_str);
            unsigned int time2 = vpxt_time_return(old_enc_file.c_str(), 0);
            cpu_tick_old = vpxt_cpu_tick_return(old_enc_file.c_str(), 0);

            tprintf(PRINT_BTH, "\n\nFile completed: time in Microseconds: %u",
                time2);
            tprintf(PRINT_BTH, "\n Total CPU Ticks: %u\n", cpu_tick_old);
        }

        tprintf(PRINT_BTH, "\n\n\nResults:\n\n");

        if (cpu_tick_new < cpu_tick_old)
            vpxt_formated_print(RESPRT, "New: %u is Faster than Old: %u - "
                "Passed", cpu_tick_new, cpu_tick_old);
        else if (cpu_tick_new < (cpu_tick_old + (cpu_tick_old * .05)))
            vpxt_formated_print(RESPRT, "New: %u is with in five percent of Old"
            ": %u - Passed", cpu_tick_new, cpu_tick_old);
        else if (cpu_tick_new == cpu_tick_old)
            vpxt_formated_print(RESPRT, "Files Took the same amount of time - "
                "Passed");
        else if (cpu_tick_new > cpu_tick_old){
            vpxt_formated_print(RESPRT, "Old: %u is Faster than New: %u -"
                " Failed",cpu_tick_old, cpu_tick_new);

            test_state = kTestFailed;
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

    if (indeterminate)
        test_state = kTestIndeterminate;

    if (test_state == kTestIndeterminate)
        tprintf(PRINT_BTH, "Indeterminate\n");
    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "Passed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "Failed\n");

    if (delete_ivf)
        vpxt_delete_files(2, new_enc_file.c_str(), old_enc_file.c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
