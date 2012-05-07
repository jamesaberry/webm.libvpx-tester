#include "vpxt_test_declarations.h"

int test_new_vs_old_psnr(int argc,
                         const char** argv,
                         const std::string &working_dir,
                         const std::string sub_folder_str,
                         int test_type,
                         int delete_ivf,
                         int artifact_detection)
{
    char *test_dir = "test_new_vs_old_psnr";
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

#if defined(_WIN32)
    exe_str.insert(0, "\"\"");
#else
    exe_str.insert(0, "\'");
#endif

    int new_enc_file_art_det = artifact_detection;

    //////////////////////////////////////////////
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
    std::string fpf_comand_line_str;

#if defined(_WIN32)
    {
        comand_line_str += old_file_name.c_str();
        comand_line_str += "\" compress \"";
        comand_line_str += input.c_str();
        comand_line_str += "\" \"";
        comand_line_str += old_enc_file;
        comand_line_str += "\" 8";
        comand_line_str += " \"";
        comand_line_str += par_file;
        comand_line_str += "\"";
        fpf_comand_line_str = comand_line_str;
        fpf_comand_line_str += " 0";
        fpf_comand_line_str += "\"";
        comand_line_str += " 3";
        comand_line_str += "\"";
    }
#else
    {
        comand_line_str += old_file_name.c_str();
        comand_line_str += "\' compress \'";
        comand_line_str += input.c_str();
        comand_line_str += "\' \'";
        comand_line_str += old_enc_file;
        comand_line_str += "\' 8";
        comand_line_str += " \'";
        comand_line_str += par_file;
        comand_line_str += "\'";
        fpf_comand_line_str = comand_line_str;
        fpf_comand_line_str += " 0";
        comand_line_str += " 3";
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
        "test_new_vs_old_psnr-log.txt");
    snprintf(git_log_input, 255, "%s%s", exe_dir_str.c_str(),
        "libvpx-git-log.txt");
    snprintf(test_log_output, 255, "%s%s", exe_dir_str.c_str(),
        "test_new_vs_old_psnr-log-sync.txt");

    // check to see if git-log.txt and new-vs-old-psnr-log exist.
    // If so use new method else use old.
    if (vpxt_file_exists_check(git_log_input) &&
        vpxt_file_exists_check(test_log_input))
    {
        if (test_type == kTestOnly)
        {
            // no prep required
        }
        else
        {
            opt.Mode = mode;

            if (vpxt_compress(input.c_str(), new_enc_file.c_str(),
                speed, bitrate, opt, "VP8", 0, 0, enc_format) == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }
        }

        // run psnr for new compression
        double new_psnr = vpxt_psnr(input.c_str(), new_enc_file.c_str(), 0,
            PRINT_BTH, 1, 0, 0, 0, NULL, new_enc_file_art_det);

        double new_data_rate = vpxt_data_rate(new_enc_file.c_str(), 1);

        char new_psnr_char[256];
        snprintf(new_psnr_char, 256, "%g", new_psnr);
        char new_data_rate_char[256];
        snprintf(new_data_rate_char, 256, "%g", new_data_rate);

        // assemble psnr/data rate results string
        std::string psnr_data_rate_result_str;
        psnr_data_rate_result_str += new_psnr_char;
        psnr_data_rate_result_str += " psnr ";
        psnr_data_rate_result_str += new_data_rate_char;
        psnr_data_rate_result_str += " kb/s";

        int arg_parse = 1;
        std::string command_line_input_str;

        // assemble command line input string
        while (arg_parse < argc){
            if (arg_parse != 1)
                command_line_input_str += " ";
            command_line_input_str += argv[arg_parse];
            ++arg_parse;
        }

        // make sure first char is not a space
        if (command_line_input_str.substr(0, 1).compare(" ") == 0)
            command_line_input_str.erase(command_line_input_str.begin(),
            command_line_input_str.begin() + 1);

        // make sure that only one log instance of current input settings exist
        // if no log instances of current input settings exist create one new
        // instance and move on.
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

        // sync/update the new git (libvpx-git-log.txt) log vs the new_vs_old
        // git log (test_new_vs_old_temp_scale-log)
        int sync_fail = vpxt_sync_new_vs_old_log(test_log_input,
            git_log_input, test_log_output, psnr_data_rate_result_str.c_str(),
            command_line_input_str, "test_new_vs_old_psnr");

        if(!sync_fail)
        {
            tprintf(PRINT_BTH, "\n\n-------------------------COMMIT-PSNR-LOG---"
                "----------------------\n\n");
            vpxt_eval_new_vs_old_log(test_log_output, command_line_input_str, 1,
                raw_data_list, "test_new_vs_old_psnr");
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
            return kTestIndeterminate;
        }

        tprintf(PRINT_BTH, "\n\nResults:\n\n");

        if (raw_data_list.size() < 4)
        {
            vpxt_formated_print(RESPRT, "New PSNR: %.4f - Old PSNR: Not found",
                raw_data_list[0]);
            indeterminate = 1;
        }
        else
        {
            if(raw_data_list[0] >= raw_data_list[2])
                vpxt_formated_print(RESPRT, "New PSNR: %.4f >= Old "
                "PSNR: %.4f - Passed", raw_data_list[0], raw_data_list[2]);
            else{
                vpxt_formated_print(RESPRT, "New PSNR: %.4f <  Old "
                    "PSNR: %.4f - Failed", raw_data_list[0],raw_data_list[2]);
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

        double psnr_ar[2];
        opt.target_bandwidth = bitrate;
        opt.auto_key = 1;

        // Run Test only (Runs Test, Sets up test to be run, or skips compresion
        // of files)
        if (test_type == kTestOnly)
        {
            // This test requires no preperation before a Test Only Run
        }
        else
        {
            if (mode == kRealTime)
            {
                opt.Mode = MODE_REALTIME;

                if (vpxt_compress(input.c_str(), new_enc_file.c_str(), speed,
                    bitrate, opt, "VP8", 0, 0, enc_format) == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    return kTestIndeterminate;
                }

                vpxt_output_compatable_settings(par_file.c_str(), opt,
                    par_file_num);

                fclose(fp);

                if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                    NULL)
                {
                    tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                        text_file_str.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");
                fprintf(stderr, "\nAttempt to run:\n%s\n\n",
                    comand_line_str.c_str());

                vpxt_run_exe(comand_line_str);
            }

            if (mode == kOnePassGoodQuality)
            {
                opt.Mode = MODE_GOODQUALITY;

                if (vpxt_compress(input.c_str(), new_enc_file.c_str(), speed,
                    bitrate, opt, "VP8", 0, 0, enc_format) == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    return kTestIndeterminate;
                }

                vpxt_output_compatable_settings(par_file.c_str(), opt,
                    par_file_num);

                fclose(fp);

                if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                    NULL)
                {
                    tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                        text_file_str.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                vpxt_run_exe(comand_line_str);
            }

            if (mode == kOnePassBestQuality)
            {
                opt.Mode = MODE_BESTQUALITY;

                if (vpxt_compress(input.c_str(), new_enc_file.c_str(), speed,
                    bitrate, opt, "VP8", 0, 0, enc_format) == -1)
                {
                    fclose(fp);
                    record_test_complete(file_index_str, file_index_output_char,
                        test_type);
                    return kTestIndeterminate;
                }

                vpxt_output_compatable_settings(par_file.c_str(), opt,
                    par_file_num);
                vpxt_run_exe(comand_line_str);
            }

            if (mode == 3)
            {
            }

            if (mode == kTwoPassGoodQuality)
            {
                // The old encoding method for two pass required for the encoder
                // to be called twice once to run the first pass then again for
                // the second
                if (par_file_num == 1)
                {
                    opt.Mode = MODE_SECONDPASS;
                    opt.lag_in_frames = 10;

                    if (vpxt_compress(input.c_str(), new_enc_file.c_str(),speed,
                        bitrate, opt, "VP8", 0, 0, enc_format) == -1)
                    {
                        fclose(fp);
                        record_test_complete(file_index_str,
                            file_index_output_char, test_type);
                        return kTestIndeterminate;
                    }

                    opt.Mode = MODE_FIRSTPASS;
                    vpxt_output_compatable_settings(par_file.c_str(), opt,
                        par_file_num);

                    fclose(fp);

                    if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                        NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                            text_file_str.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");
                    // this needs to be run to make this test compatable with
                    // old versions of vp8 that required the first pass to be
                    // called diffrently
                    vpxt_run_exe(fpf_comand_line_str);

                    opt.Mode = MODE_SECONDPASS;
                    vpxt_output_compatable_settings(par_file.c_str(), opt,
                        par_file_num);

                    fclose(fp);

                    if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                        NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                            text_file_str.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(comand_line_str);
                }
                else
                {
                    opt.Mode = MODE_SECONDPASS;

                    if (vpxt_compress(input.c_str(), new_enc_file.c_str(),speed,
                        bitrate, opt, "VP8", 0, 0, enc_format) == -1)
                    {
                        fclose(fp);
                        record_test_complete(file_index_str,
                            file_index_output_char, test_type);
                        return kTestIndeterminate;
                    }

                    opt.Mode = MODE_SECONDPASS;
                    vpxt_output_compatable_settings(par_file.c_str(), opt,
                        par_file_num);

                    fclose(fp);

                    if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                        NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                            text_file_str.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(comand_line_str);
                }
            }

            if (mode == kTwoPassBestQuality)
            {
                // The old encoding method for two pass required for the encoder
                // to be called twice once to run the first pass then again for
                // the second
                if (par_file_num == 1)
                {
                    opt.Mode = MODE_SECONDPASS_BEST;
                    opt.lag_in_frames = 10;

                    if (vpxt_compress(input.c_str(), new_enc_file.c_str(),speed,
                        bitrate, opt, "VP8", 0, 0, enc_format) == -1)
                    {
                        fclose(fp);
                        record_test_complete(file_index_str,
                            file_index_output_char, test_type);
                        return kTestIndeterminate;
                    }

                    opt.Mode = 3;
                    vpxt_output_compatable_settings(par_file.c_str(), opt,
                        par_file_num);

                    fclose(fp);

                    if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                        NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                            text_file_str.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    // this needs to be run to make this test compatable with
                    // old versions of vp8 that required the first pass to be
                    // called diffrently
                    vpxt_run_exe(fpf_comand_line_str);

                    opt.Mode = MODE_SECONDPASS_BEST;
                    vpxt_output_compatable_settings(par_file.c_str(), opt,
                        par_file_num);

                    fclose(fp);

                    if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                        NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                            text_file_str.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(comand_line_str);
                }
                else
                {
                    opt.Mode = MODE_SECONDPASS_BEST;

                    if (vpxt_compress(input.c_str(), new_enc_file.c_str(),speed,
                        bitrate, opt, "VP8", 0, 0, enc_format) == -1)
                    {
                        fclose(fp);
                        record_test_complete(file_index_str,
                            file_index_output_char, test_type);
                        return kTestIndeterminate;
                    }

                    opt.Mode = MODE_SECONDPASS_BEST;
                    vpxt_output_compatable_settings(par_file.c_str(), opt,
                        par_file_num);

                    fclose(fp);

                    if ((fp = freopen(text_file_str.c_str(), "a+", stderr)) ==
                        NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
                            text_file_str.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(comand_line_str);
                }
            }
        }

        // Create Compression only stop test short.
        if (test_type == kCompOnly)
        {
            // Compression only run
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestEncCreated;
        }

        if (vpxt_file_size(old_enc_file.c_str(), 0) == 0)
        {
            tprintf(PRINT_BTH, "\nError - Old File Incorrect\n");

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestFailed;
        }

        psnr_ar[0] = vpxt_psnr(input.c_str(), new_enc_file.c_str(), 0,PRINT_BTH,
            1, 0, 0, 0, NULL, new_enc_file_art_det);
        psnr_ar[1] = vpxt_get_psnr(old_enc_file.c_str());

        tprintf(PRINT_BTH, "\nNew DataRate");
        vpxt_data_rate(new_enc_file.c_str(), 1);

        tprintf(PRINT_BTH, "\nOld DataRate");
        vpxt_data_rate(old_enc_file.c_str(), 1);


        if(psnr_ar[0] >= psnr_ar[1])
            vpxt_formated_print(RESPRT, "New PSNR: %.4f >= Old "
            "PSNR: %.4f - Passed", psnr_ar[0], psnr_ar[1]);
        else{
            vpxt_formated_print(RESPRT, "New PSNR: %.4f <  Old "
                "PSNR: %.4f - Failed", psnr_ar[0], psnr_ar[1]);
            test_state = kTestFailed;
        }
    }

    if (indeterminate == 1)
        test_state = kTestIndeterminate;

    // handle possible artifact
    if(new_enc_file_art_det == kPossibleArtifactFound)
    {
        tprintf(PRINT_BTH, "\nPossible Artifact\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPossibleArtifact;
    }

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
