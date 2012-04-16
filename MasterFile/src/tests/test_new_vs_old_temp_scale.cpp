#include "vpxt_test_declarations.h"

int test_new_vs_old_temp_scale(int argc,
                               const char *const *argv,
                               const std::string &working_dir,
                               std::string files_ar[],
                               int test_type,
                               int delete_ivf)
{
    char *test_dir = "test_new_vs_old_temp_scale";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int layer_mode = atoi(argv[3]);
    int temp_scale_bitrate_0 = atoi(argv[4]);
    int temp_scale_bitrate_1 = atoi(argv[5]);
    int temp_scale_bitrate_2 = atoi(argv[6]);
    int temp_scale_bitrate_3 = atoi(argv[7]);
    int temp_scale_bitrate_4 = atoi(argv[8]);
    std::string old_file_name = argv[9];
    std::string enc_format = argv[10];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, files_ar) == 11)
        return 11;

    int number_of_encodes = 0;
    std::string temp_scale_out_base = cur_test_dir_str;
    temp_scale_out_base += slashCharStr();
    temp_scale_out_base += test_dir;
    temp_scale_out_base += "_new_comp";

    std::vector<std::string> new_vs_old_temp_scale_new_fn_vec;
    std::vector<std::string> new_vs_old_temp_scale_old_fn_vec;
    std::vector<std::string>::iterator str_it;

    std::vector<double> new_vs_old_temp_scale_psnr_new;
    std::vector<double> new_vs_old_temp_scale_psnr_old;
    std::vector<double> new_vs_old_temp_scale_data_rate_new;
    std::vector<double> new_vs_old_temp_scale_data_rate_old;
    std::vector<double> raw_data_list;

    std::vector<double>::iterator double_it;
    std::vector<double>::iterator double_it2;

    if((layer_mode >= 0 && layer_mode < 2) || layer_mode == 7)
        number_of_encodes = 2;
    if((layer_mode >= 2 && layer_mode < 6) || layer_mode == 8)
        number_of_encodes = 3;
    if(layer_mode == 6)
        number_of_encodes = 5;

    //file names for new
    for(int i = 0; i < number_of_encodes; ++i){
        char i_char[8];
        vpxt_itoa_custom(i, i_char, 10);

        std::string temp_scale_str = cur_test_dir_str;
        temp_scale_str += slashCharStr();
        temp_scale_str += test_dir;
        temp_scale_str += "_new_comp_";
        temp_scale_str += i_char;
        vpxt_enc_format_append(temp_scale_str, enc_format);
        new_vs_old_temp_scale_new_fn_vec.push_back(temp_scale_str);
    }

    //file names for old
    for(int i = 0; i < number_of_encodes; ++i){
        char i_char[8];
        vpxt_itoa_custom(i, i_char, 10);

        std::string temp_scale_str = cur_test_dir_str;
        temp_scale_str += slashCharStr();
        temp_scale_str += test_dir;
        temp_scale_str += "_old_comp_";
        temp_scale_str += i_char;
        vpxt_enc_format_append(temp_scale_str, enc_format);
        new_vs_old_temp_scale_old_fn_vec.push_back(temp_scale_str);
    }

    std::string exe_dir_str;
    vpxt_folder_name(argv[0], &exe_dir_str);
    std::string command_line_str = exe_dir_str;

    char old_exe_full_path[256];
    snprintf(old_exe_full_path, 255, "%s%s", exe_dir_str.c_str(), argv[9]);

    ///////////Set up old exe command line//////////////////
    std::string old_enc_file = cur_test_dir_str;
    old_enc_file += slashCharStr();
    old_enc_file += test_dir;
    old_enc_file += "_old_comp";

    std::string par_file = cur_test_dir_str;
    par_file += slashCharStr();
    par_file += test_dir;
    par_file += "_parameter_file.txt";

    char layer_mode_char[256];
    char temp_scale_bitrate_0_char[256];
    char temp_scale_bitrate_1_char[256];
    char temp_scale_bitrate_2_char[256];
    char temp_scale_bitrate_3_char[256];
    char temp_scale_bitrate_4_char[256];

    vpxt_itoa_custom(layer_mode, layer_mode_char, 10);
    vpxt_itoa_custom(temp_scale_bitrate_0, temp_scale_bitrate_0_char, 10);
    vpxt_itoa_custom(temp_scale_bitrate_1, temp_scale_bitrate_1_char, 10);
    vpxt_itoa_custom(temp_scale_bitrate_2, temp_scale_bitrate_2_char, 10);
    vpxt_itoa_custom(temp_scale_bitrate_3, temp_scale_bitrate_3_char, 10);
    vpxt_itoa_custom(temp_scale_bitrate_4, temp_scale_bitrate_4_char, 10);

    char* quote;
#if defined(_WIN32)
    quote = "\"";
    command_line_str.insert(0, "\"\"");
#else
    quote = "\'";
    command_line_str.insert(0, "\'");
#endif

    command_line_str += old_file_name;
    command_line_str += quote;
    command_line_str += " temp_scale_compress ";
    command_line_str += quote;
    command_line_str += input.c_str();
    command_line_str += quote;
    command_line_str += " ";
    command_line_str += quote;
    command_line_str += old_enc_file;
    command_line_str += quote;
    command_line_str += " ";
    command_line_str += layer_mode_char;
    command_line_str += " ";
    command_line_str += temp_scale_bitrate_0_char;
    command_line_str += " ";
    command_line_str += temp_scale_bitrate_1_char;
    command_line_str += " ";
    command_line_str += temp_scale_bitrate_2_char;
    command_line_str += " ";
    command_line_str += temp_scale_bitrate_3_char;
    command_line_str += " ";
    command_line_str += temp_scale_bitrate_4_char;
    command_line_str += " ";
    command_line_str += enc_format;
    command_line_str += " 8 ";
    command_line_str += quote;
    command_line_str += par_file;
    command_line_str += quote;
    command_line_str += " 3";

#if defined(_WIN32)
    command_line_str += quote;
#endif

    /////////////OutPutfile////////////
    std::string text_file_str = cur_test_dir_str;
    text_file_str += slashCharStr();
    text_file_str += test_dir;

    if (test_type == COMP_ONLY || test_type == TEST_AND_COMP)
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

    if (test_type == TEST_AND_COMP)
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
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
    }

    int n = 0;
    int use_log = 0;
    int failed = 0;
    int indeterminate = 0;

    unsigned int new_scale_compress_time = 0;
    unsigned int old_scale_compress_time = 0;

    char git_log_input[256];
    char test_log_input[256];
    char test_log_output[256];

    snprintf(test_log_input, 255, "%s%s", exe_dir_str.c_str(),
        "test_new_vs_old_temp_scale-log.txt");
    snprintf(git_log_input, 255, "%s%s", exe_dir_str.c_str(),
        "libvpx-git-log.txt");
    snprintf(test_log_output, 255, "%s%s", exe_dir_str.c_str(),
        "test_new_vs_old_temp_scale-log-sync.txt");

    //check to see if git-log.txt and test_new_vs_old_temp_scale-log exist.
    //If so use new method else use old if cant find either abort.
    if (vpxt_file_exists_check(git_log_input) &&
        vpxt_file_exists_check(test_log_input))
        use_log = 1;
    else if (!vpxt_file_exists_check(old_exe_full_path))
    {
        tprintf(PRINT_BTH, "\nCould not find log files and input executable %s "
            "does not exist - Test Aborted\n", argv[9]);

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char,
            test_type);
        return 2;
    }

    //if means to preform test found create new compressions
    opt.Mode = 0;
    new_scale_compress_time = vpxt_compress_scalable_patterns(
        input.c_str(), temp_scale_out_base.c_str(), speed, opt, "VP8",
        0, 0, enc_format, layer_mode, temp_scale_bitrate_0,
        temp_scale_bitrate_1, temp_scale_bitrate_2,
        temp_scale_bitrate_3, temp_scale_bitrate_4);

    if (new_scale_compress_time == -1){
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char,
            test_type);
        return 2;
    }

    //run psnrs for new temp scale compressions
    for(str_it = new_vs_old_temp_scale_new_fn_vec.begin(); str_it <
        new_vs_old_temp_scale_new_fn_vec.end(); ++str_it)
        new_vs_old_temp_scale_psnr_new.push_back(vpxt_psnr(input.c_str(),
        (*str_it).c_str(), 0, PRINT_BTH, 1, NULL));
    //run data rates for new temp scale compressions
    for(str_it = new_vs_old_temp_scale_new_fn_vec.begin(); str_it <
        new_vs_old_temp_scale_new_fn_vec.end(); ++str_it)
        new_vs_old_temp_scale_data_rate_new.push_back(vpxt_data_rate(
        (*str_it).c_str(), 1));

    //if log files found
    if(use_log)
    {
        //assemble psnr/data rate results string
        std::string psnr_data_rate_result_str;
        double_it2 = new_vs_old_temp_scale_data_rate_new.begin();
        for(double_it = new_vs_old_temp_scale_psnr_new.begin(); double_it <
            new_vs_old_temp_scale_psnr_new.end(); ++double_it)
        {
            char psnr_char[256];
            snprintf(psnr_char, 256, "%g", *double_it);
            char data_rate_char[256];
            snprintf(data_rate_char, 256, "%g", *double_it2);

            psnr_data_rate_result_str += psnr_char;
            psnr_data_rate_result_str += " psnr ";
            psnr_data_rate_result_str += data_rate_char;
            psnr_data_rate_result_str += " kb/s ";

            ++double_it2;
        }

        char new_time_char[256];
        vpxt_itoa_custom(new_scale_compress_time, new_time_char, 10);
        psnr_data_rate_result_str += new_time_char;
        psnr_data_rate_result_str += " ms";

        int arg_parse = 1;
        std::string command_line_input_str;

        //assemble command line input string
        while (arg_parse < argc){
            if (arg_parse != 1)
                command_line_input_str += " ";
            command_line_input_str += argv[arg_parse];
            ++arg_parse;
        }

        //make sure first char is not a space
        if (command_line_input_str.substr(0, 1).compare(" ") == 0)
            command_line_input_str.erase(command_line_input_str.begin(),
            command_line_input_str.begin() + 1);

        //make sure that only one log instance of current input settings exist
        //if no log instances of current input settings exist create one new
        //instance and move on.
        if (vpxt_init_new_vs_old_log(test_log_input, command_line_input_str)
            != 1)
        {
            tprintf(PRINT_BTH, "\nNUMBER OF UNIQUE IDS NOT EQUAL 1: TEST "
                "ABORTED\n");
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return 2;
        }

        //sync/update the new git (libvpx-git-log.txt) log vs the new_vs_old git
        //log (test_new_vs_old_temp_scale-log)
        int sync_fail = vpxt_sync_new_vs_old_log(test_log_input, git_log_input,
            test_log_output, psnr_data_rate_result_str.c_str(),
            command_line_input_str, "test_new_vs_old_temp_scale");

        if(!sync_fail)
        {
            tprintf(PRINT_BTH, "\n\n-------------------------COMMIT-PSNR-LOG---"
                "----------------------\n\n");

            vpxt_eval_new_vs_old_log(test_log_output, command_line_input_str, 1,
                raw_data_list, "test_new_vs_old_temp_scale");

            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "--------------------\n");
        }
        else
        {
            tprintf(PRINT_BTH, "\nFAILED TO SYNC NEW VS OLD LOG - TEST ABORTED"
                "\n");
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return 2;
        }

        //sort through raw data
        //make sure we have correct number of new and old data points
        if (raw_data_list.size() < (number_of_encodes * 4) + 1)
        {
            //if we dont have old data handle indeterminate here.
            tprintf(PRINT_BTH, "\n\nResults:\n\n");

            for(double_it = raw_data_list.begin();
                double_it < raw_data_list.end(); ++double_it)
            {   //skip over data rate values
                if(n % 2 == 0)
                    vpxt_formated_print(RESPRT, "New PSNR: %i %.4f - Old PSNR: "
                    "Not found - Indeterminate\n", n/2, *double_it);
                ++n;
            }

            indeterminate = 1;
        }
        else
        {   //parse through data and get psnr information
            for(double_it = raw_data_list.begin() + (number_of_encodes * 2) + 1;
                double_it < raw_data_list.begin() + (number_of_encodes * 4) + 1;
                ++double_it)
            {
                //skip over data rate values
                if(n % 2 == 0)
                    new_vs_old_temp_scale_psnr_old.push_back(*double_it);
                ++n;
            }

            //get old time
            old_scale_compress_time = raw_data_list[(number_of_encodes * 4) +1];
        }

        //update test_new_vs_old_temp_scale-log.txt
        vpxt_delete_files_quiet(1, test_log_input);
        vpxt_copy_file(test_log_output, test_log_input);
        vpxt_delete_files_quiet(1, test_log_output);
    }
    else
    {
        //if old executable found
        vpxt_output_compatable_settings(par_file.c_str(), opt, 2);

        fclose(fp);

        if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
            NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                text_file_str.c_str());
            exit(1);
        }

        fprintf(stderr, " ");

        vpxt_run_exe(command_line_str);

        //run psnrs for new temp scale compressions
        for(str_it = new_vs_old_temp_scale_old_fn_vec.begin(); str_it <
            new_vs_old_temp_scale_old_fn_vec.end(); ++str_it)
            new_vs_old_temp_scale_psnr_old.push_back(vpxt_psnr(input.c_str(),
            (*str_it).c_str(), 0, PRINT_BTH, 1, NULL));
        //run data rates for new temp scale compressions
        for(str_it = new_vs_old_temp_scale_old_fn_vec.begin(); str_it <
            new_vs_old_temp_scale_old_fn_vec.end(); ++str_it)
            new_vs_old_temp_scale_data_rate_old.push_back(vpxt_data_rate(
            (*str_it).c_str(), 1));

        //get old time
        old_scale_compress_time = vpxt_cpu_tick_return(old_enc_file.c_str(), 0);
    }

    //indeterminate special case is handled above.
    if(!indeterminate)
    {
        tprintf(PRINT_BTH, "\n\nResults:\n\n");

        //evaluate new vs old psnrs
        n = 1;
        double_it2 = new_vs_old_temp_scale_psnr_old.begin();
        for(double_it = new_vs_old_temp_scale_psnr_new.begin(); double_it <
            new_vs_old_temp_scale_psnr_new.end(); ++double_it)
        {
            if(*double_it >= *double_it2)
                vpxt_formated_print(RESPRT, "New PSNR: %i %.4f >= Old "
                "PSNR: %i %.4f - Passed\n", n, *double_it, n, *double_it2);
            else{
                vpxt_formated_print(RESPRT, "New PSNR: %i %.4f <  Old "
                    "PSNR: %i %.4f - Failed\n", n, *double_it, n, *double_it2);
                failed = 1;
            }

            ++double_it2;
            ++n;
        }

        //evaluate new vs old times
        if(new_scale_compress_time <= old_scale_compress_time)
            vpxt_formated_print(RESPRT, "New time: %i <= Old "
            "time: %i - Passed\n", new_scale_compress_time,
            old_scale_compress_time);
        else{
            vpxt_formated_print(RESPRT, "New time: %i > Old "
                "time: %i - Failed\n", new_scale_compress_time,
                old_scale_compress_time);
            failed = 1;
        }

        tprintf(PRINT_BTH, "\n");
    }

    //delete encode files if flagged
    if (delete_ivf){
        for(str_it = new_vs_old_temp_scale_new_fn_vec.begin(); str_it <
            new_vs_old_temp_scale_new_fn_vec.end(); ++str_it)
            vpxt_delete_files(1, (*str_it).c_str());
        for(str_it = new_vs_old_temp_scale_old_fn_vec.begin(); str_it <
            new_vs_old_temp_scale_old_fn_vec.end(); ++str_it)
            vpxt_delete_files(1, (*str_it).c_str());
    }

    if (indeterminate)
    {
        tprintf(PRINT_BTH, "Indeterminate\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 2;
    }

    if (!failed)
    {
        tprintf(PRINT_BTH, "Passed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "Failed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return 0;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return 6;
}
