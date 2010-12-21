#include "vpxt_test_declarations.h"

int test_new_vs_old_psnr(int argc, char *argv[], const string& WorkingDir, string FilesAr[], int TestType)
{

    char *MyDir = "test_new_vs_old_psnr";

    if (!(argc == 7 || argc == 8))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        tprintf(PRINT_STD,
                "\n\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Exe File To Compare>\n"
                "    <Parameter Version-1=1.0.4|2=2.0.0>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char ExeInput[255];
    snprintf(ExeInput, 255, "%s", argv[5]);
    int ParFileNum = atoi(argv[6]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    string ExeString = "";
    char MainTestDirChar[255] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    char ExeChar[1024];
    char ExeChar2[1024];
    vpxt_folder_name(argv[0], ExeChar2);
    snprintf(ExeChar, 1024, "%s", ExeChar2);

    ExeString = ExeChar;

#if defined(_WIN32)
    ExeString.insert(0, "\"\"");
#else
    ExeString.insert(0, "\'");
#endif

    //////////////////////////////////////////////
    string NewEncFile = CurTestDirStr;
    NewEncFile.append(slashCharStr());
    NewEncFile.append(MyDir);
    NewEncFile.append("_compression_new.ivf");

    string OldEncFile = CurTestDirStr;
    OldEncFile.append(slashCharStr());
    OldEncFile.append(MyDir);
    OldEncFile.append("_compression_old.ivf");

    string ParFile = CurTestDirStr;
    ParFile.append(slashCharStr());
    ParFile.append(MyDir);
    ParFile.append("_parameter_file.txt");

    string Program = ExeString;
    string FPF_Program = "";

#if defined(_WIN32)
    {
        Program.append(ExeInput);
        Program.append("\" compress \"");
        Program.append(input);
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
        Program.append(input);
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
    string TextfileString = CurTestDirStr;
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
    if (argc == 8)
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

    /////////////////Make Sure Exe File Exists///////////////
    if (!vpxt_file_exists_check(argv[5]))
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

            if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, 0, 3, 3) == -1)
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

            vpxt_run_exe(Program);
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;

            if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, 0, 3, 3) == -1)
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

            if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, 0, 3, 3) == -1)
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

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, 0, 3, 3) == -1)
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

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, 0, 3, 3) == -1)
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

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, 0, 3, 3) == -1)
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

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0, 0, 3, 3) == -1)
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

    double PSNRArr[2];

    PSNRArr[0] = vpxt_ivf_psnr(input, NewEncFile.c_str(), 0, 0, 1, NULL);
    PSNRArr[1] = vpxt_get_psnr(OldEncFile.c_str());

    tprintf(PRINT_BTH, "\nNew DataRate");
    vpxt_ivf_data_rate(NewEncFile.c_str(), 1);

    tprintf(PRINT_BTH, "\nOld DataRate");
    vpxt_ivf_data_rate(OldEncFile.c_str(), 1);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (PSNRArr[0] > PSNRArr[1])
    {
        vpxt_formated_print(RESPRT, "New PSNR: %.2f > Old PSNR: %.2f - Passed", PSNRArr[0], PSNRArr[1]);

        tprintf(PRINT_BTH, "\n\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    if (PSNRArr[0] <= PSNRArr[1] + (PSNRArr[1] * 0.005) && PSNRArr[0] >= PSNRArr[1] - (PSNRArr[1] * 0.005))
    {
        vpxt_formated_print(RESPRT, "New PSNR: %.2f is with in 1/2%% of old: %.2f - Min Passed", PSNRArr[0], PSNRArr[1]);

        tprintf(PRINT_BTH, "\n\nMin Passed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 8;
    }
    else
    {
        vpxt_formated_print(RESPRT, "New PSNR: %.2f < Old PSNR: %.2f - Failed", PSNRArr[0], PSNRArr[1]);

        tprintf(PRINT_BTH, "\n\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}