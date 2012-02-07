#include "vpxt_test_declarations.h"

int test_new_vs_old_psnr(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{

    char *MyDir = "test_new_vs_old_psnr";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char ExeInput[255];
    snprintf(ExeInput, 255, "%s", argv[5]);
    int ParFileNum = atoi(argv[6]);
    std::string EncForm = argv[7];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    std::string ExeString = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    char ExeChar[1024];
    char ExeChar2[1024];
    vpxt_folder_name(argv[0], ExeChar2);
    snprintf(ExeChar, 1024, "%s", ExeChar2);

    ExeString = ExeChar;

    char oldexefullpath[256];
    snprintf(oldexefullpath, 255, "%s%s", ExeChar, argv[5]);

#if defined(_WIN32)
    ExeString.insert(0, "\"\"");
#else
    ExeString.insert(0, "\'");
#endif

    //////////////////////////////////////////////
    std::string NewEncFile = CurTestDirStr;
    NewEncFile.append(slashCharStr());
    NewEncFile.append(MyDir);
    NewEncFile.append("_compression_new");
    vpxt_enc_format_append(NewEncFile, EncForm);

    std::string OldEncFile = CurTestDirStr;
    OldEncFile.append(slashCharStr());
    OldEncFile.append(MyDir);
    OldEncFile.append("_compression_old");
    vpxt_enc_format_append(OldEncFile, EncForm);

    std::string ParFile = CurTestDirStr;
    ParFile.append(slashCharStr());
    ParFile.append(MyDir);
    ParFile.append("_parameter_file.txt");

    std::string Program = ExeString;
    std::string FPF_Program = "";

#if defined(_WIN32)
    {
        Program.append(ExeInput);
        Program.append("\" compress \"");
        Program.append(input.c_str());
        Program.append("\" \"");
        Program.append(OldEncFile);
        Program.append("\" 8");
        Program.append(" \"");
        Program.append(ParFile);
        Program.append("\"");
        FPF_Program = Program;
        FPF_Program.append(" 0");
        FPF_Program.append("\"");
        Program.append(" 3");
        Program.append("\"");
    }
#else
    {
        Program.append(ExeInput);
        Program.append("\' compress \'");
        Program.append(input.c_str());
        Program.append("\' \'");
        Program.append(OldEncFile);
        Program.append("\' 8");
        Program.append(" \'");
        Program.append(ParFile);
        Program.append("\'");
        FPF_Program = Program;
        FPF_Program.append(" 0");
        Program.append(" 3");
    }
#endif

    /////////////OutPutfile////////////
    std::string TextfileString = CurTestDirStr;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == COMP_ONLY || TestType == TEST_AND_COMP)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (TestType == TEST_AND_COMP)
        print_header_full_test(argc, argv, MainTestDirChar);

    if (TestType == COMP_ONLY)
        print_header_compression_only(argc, argv, MainTestDirChar);

    if (TestType == TEST_ONLY)
        print_header_test_only(argc, argv, CurTestDirStr);

    vpxt_cap_string_print(PRINT_BTH, "%s", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (inputCheck == 2)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n", argv[argc-1]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    double PSNRArr[2];
    int Indeterminate = 0;

    char InputTestLog[256];
    char InputGitLog[256];
    char OutputTestLog[256];

    snprintf(InputTestLog, 255, "%s%s", ExeChar, "test_new_vs_old_psnr-log.txt");
    snprintf(InputGitLog, 255, "%s%s", ExeChar, "libvpx-git-log.txt");
    snprintf(OutputTestLog, 255, "%s%s", ExeChar, "test_new_vs_old_psnr-log-sync.txt");

    //check to see if git-log.txt and new-vs-old-psnr-log exist.  If so use new method else use old.
    if (vpxt_file_exists_check(InputGitLog) && vpxt_file_exists_check(InputTestLog))
    {
        if (TestType == TEST_ONLY)
        {
            //no prep required
        }
        else
        {
            opt.Mode = Mode;

            if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }
        }

        double NewPSNR = vpxt_psnr(input.c_str(), NewEncFile.c_str(), 0, PRINT_BTH, 1, NULL);

        double NewDR = vpxt_data_rate(NewEncFile.c_str(), 1);

        char NewPSNR_char[256];
        snprintf(NewPSNR_char, 256, "%g", NewPSNR);

        char NewDR_char[256];
        snprintf(NewDR_char, 256, "%g", NewDR);

        std::string UpdateString = "";
        UpdateString.append(NewPSNR_char);
        UpdateString.append(" psnr ");
        UpdateString.append(NewDR_char);
        UpdateString.append(" kb/s");

        int argParse = 1;
        std::string ArgumentString;

        while (argParse < argc)
        {
            if (argParse != 1)
                ArgumentString.append(" ");

            ArgumentString.append(argv[argParse]);
            argParse = argParse + 1;
        }

        if (ArgumentString.substr(0, 1).compare(" ") == 0)
            ArgumentString.erase(ArgumentString.begin(), ArgumentString.begin() + 1);

        if (vpxt_init_new_vs_old_log(InputTestLog, ArgumentString) != 1)
        {
            tprintf(PRINT_BTH, "\nNUMBER OF UNIQUE IDS NOT EQUAL 1: TEST ABORTED\n");
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        std::vector<double> ValueList;

        int sync_state = vpxt_sync_new_vs_old_log(InputTestLog, InputGitLog, OutputTestLog, UpdateString.c_str(), ArgumentString, "test_new_vs_old_psnr");

        if (sync_state == -1)
            ValueList.push_back(NewPSNR);
        else
        {
            tprintf(PRINT_BTH, "\n\n-------------------------COMMIT-PSNR-LOG-------------------------\n\n");
            vpxt_eval_new_vs_old_log(OutputTestLog, ArgumentString, 1, ValueList, "test_new_vs_old_psnr");
            tprintf(PRINT_BTH, "\n-----------------------------------------------------------------\n");
        }

        if (ValueList.size() < 2)
        {
            PSNRArr[0] = ValueList[0];
            PSNRArr[1] = 0.0;
            Indeterminate = 1;
        }
        else
        {
            PSNRArr[0] = ValueList[0];
            PSNRArr[1] = ValueList[1];
        }

        vpxt_delete_files_quiet(1, InputTestLog);
        vpxt_copy_file(OutputTestLog, InputTestLog);
        vpxt_delete_files_quiet(1, OutputTestLog);
    }
    else
    {
        /////////////////Make Sure Exe File Exists///////////////
        if (!vpxt_file_exists_check(oldexefullpath))
        {
            tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", argv[5]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        /////////////////////////////////////////////////////////

        opt.target_bandwidth = BitRate;
        opt.auto_key = 1;

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == TEST_ONLY)
        {
            //This test requires no preperation before a Test Only Run
        }
        else
        {
            if (Mode == 0)
            {
                opt.Mode = MODE_REALTIME;

                if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
                {
                    fclose(fp);
                    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                    return 2;
                }

                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");
                fprintf(stderr, "\nAttempt to run:\n%s\n\n", Program.c_str());

                vpxt_run_exe(Program);
            }

            if (Mode == 1)
            {
                opt.Mode = MODE_GOODQUALITY;

                if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
                {
                    fclose(fp);
                    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                    return 2;
                }

                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                //cout << "\n\n" << Program << "\n\n";

                vpxt_run_exe(Program);
            }

            if (Mode == 2)
            {
                opt.Mode = MODE_BESTQUALITY;

                if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
                {
                    fclose(fp);
                    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                    return 2;
                }

                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);
                vpxt_run_exe(Program);
            }

            if (Mode == 3)
            {
            }

            if (Mode == 4)
            {
                if (ParFileNum == 1) //The old encoding method for two pass required for the encoder to be called twice once to run the first pass then again for the second
                {
                    opt.Mode = MODE_SECONDPASS;
                    opt.lag_in_frames = 10;

                    if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
                    {
                        fclose(fp);
                        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                        return 2;
                    }

                    opt.Mode = MODE_FIRSTPASS;
                    vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(FPF_Program);//this needs to be run to make this test compatable with old versions of vp8 that required the first pass to be called diffrently

                    opt.Mode = MODE_SECONDPASS;
                    vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(Program);
                }
                else
                {
                    opt.Mode = MODE_SECONDPASS;

                    if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
                    {
                        fclose(fp);
                        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                        return 2;
                    }

                    opt.Mode = MODE_SECONDPASS;
                    vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(Program);
                }
            }

            if (Mode == 5)
            {
                if (ParFileNum == 1) //The old encoding method for two pass required for the encoder to be called twice once to run the first pass then again for the second
                {
                    opt.Mode = MODE_SECONDPASS_BEST;
                    opt.lag_in_frames = 10;

                    if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
                    {
                        fclose(fp);
                        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                        return 2;
                    }

                    opt.Mode = 3;
                    vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(FPF_Program);//this needs to be run to make this test compatable with old versions of vp8 that required the first pass to be called diffrently

                    opt.Mode = MODE_SECONDPASS_BEST;
                    vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(Program);
                }
                else
                {
                    opt.Mode = MODE_SECONDPASS_BEST;

                    if (vpxt_compress(input.c_str(), NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, EncForm) == -1)
                    {
                        fclose(fp);
                        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                        return 2;
                    }

                    opt.Mode = MODE_SECONDPASS_BEST;
                    vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                    fclose(fp);

                    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                    {
                        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
                        exit(1);
                    }

                    fprintf(stderr, " ");

                    vpxt_run_exe(Program);
                }
            }
        }

        //Create Compression only stop test short.
        if (TestType == COMP_ONLY)
        {
            //Compression only run
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 10;
        }

        if (vpxt_file_size(OldEncFile.c_str(), 0) == 0)
        {
            tprintf(PRINT_BTH, "\nError - Old File Incorrect\n");

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 0;
        }

        PSNRArr[0] = vpxt_psnr(input.c_str(), NewEncFile.c_str(), 0, PRINT_BTH, 1, NULL);
        PSNRArr[1] = vpxt_get_psnr(OldEncFile.c_str());

        tprintf(PRINT_BTH, "\nNew DataRate");
        vpxt_data_rate(NewEncFile.c_str(), 1);

        tprintf(PRINT_BTH, "\nOld DataRate");
        vpxt_data_rate(OldEncFile.c_str(), 1);
    }

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (Indeterminate == 1)
    {
        vpxt_formated_print(RESPRT, "New PSNR: %.2f Old PSNR: Not Found - Indeterminate", PSNRArr[0]);

        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        if (DeleteIVF)
            vpxt_delete_files(2, NewEncFile.c_str(), OldEncFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    if (PSNRArr[0] >= PSNRArr[1])
    {
        vpxt_formated_print(RESPRT, "New PSNR: %.2f >= Old PSNR: %.2f - Passed", PSNRArr[0], PSNRArr[1]);

        tprintf(PRINT_BTH, "\n\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, NewEncFile.c_str(), OldEncFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "New PSNR: %.2f < Old PSNR: %.2f - Failed", PSNRArr[0], PSNRArr[1]);

        tprintf(PRINT_BTH, "\n\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, NewEncFile.c_str(), OldEncFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}