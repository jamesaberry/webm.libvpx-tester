#include "vpxt_test_declarations.h"

int test_new_vs_old_psnr(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  NewVsOldPSRN \n\n"
            "    <inputfile>\n"
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
    int ParFileNum = atoi(argv[6]);
    char ExeInput[255];

    //#if defined(_WIN32)
    //        {
    snprintf(ExeInput, 255, "%s", argv[5]);
    //        }
    //#elif defined(linux)
    //        {
    //            string ExeInputStr = argv[5];
    //            snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
    //        }
    //#elif defined(__APPLE__)
    //        {
    //            string ExeInputStr = argv[5];
    //            snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
    //        }
    //#elif defined(__POWERPC__)
    //        {
    //            string ExeInputStr = argv[5];
    //            snprintf(ExeInput, 255, "%s", ExeInputStr.c_str());
    //        }
    //#endif
    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";

    string ExeString = "";

    char WorkingDir3[255] = "";
    char *MyDir = "NewVsOldPSNR";
    string MainDirString = "";
    char File1[255] = "";


    //////////////////////////////////////////////////////////////////////
    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
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
    string NewEncFile = WorkingDirString;
    string OldEncFile = WorkingDirString;
    string ParFile = WorkingDirString;
    string Program = ExeString;
    string FPF_Program = "";

    NewEncFile.append(slashCharStr());
    NewEncFile.append("outputVP8New.ivf");
    OldEncFile.append(slashCharStr());
    OldEncFile.append("outputVP8Old.ivf");
    ParFile.append(slashCharStr());
    ParFile.append("ParFile.txt");

#if defined(_WIN32)
    {
        Program.append(ExeInput);
        Program.append("\" \"");
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
        Program.append("\' \'");
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
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    cout << "TextfileString: " << TextfileString.c_str() << "\n";

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        print_header_full_test(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    int speed = 0;


    tprintf("New Vs Old PSNR Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    /////////////////Make Sure Exe File Exists///////////////
    if (!vpxt_file_exists_check(argv[5]))
    {
        tprintf("\nInput executable %s does not exist\n", argv[5]);
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.auto_key = 1;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;

            if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }

            vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            vpxt_run_exe(Program);
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;

            if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }

            vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file: %s\n", TextfileString.c_str());
                exit(1);
            }

            fprintf(stderr, " ");

            //cout << "\n\n" << Program << "\n\n";

            vpxt_run_exe(Program);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
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

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_FIRSTPASS;
                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                vpxt_run_exe(FPF_Program);//this needs to be run to make this test compatable with old versions of vp8 that required the first pass to be called diffrently

                opt.Mode = MODE_SECONDPASS;
                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                vpxt_run_exe(Program);
            }
            else
            {
                opt.Mode = MODE_SECONDPASS;

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_SECONDPASS;
                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
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

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = 3;
                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                vpxt_run_exe(FPF_Program);//this needs to be run to make this test compatable with old versions of vp8 that required the first pass to be called diffrently

                opt.Mode = MODE_SECONDPASS_BEST;
                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                vpxt_run_exe(Program);
            }
            else
            {
                opt.Mode = MODE_SECONDPASS_BEST;

                if (vpxt_compress_ivf_to_ivf(input, NewEncFile.c_str(), speed, BitRate, opt, "VP8", 0, 0) == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                opt.Mode = MODE_SECONDPASS_BEST;
                vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);

                fclose(fp);

                if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
                {
                    printf("Cannot open out put file: %s\n", TextfileString.c_str());
                    exit(1);
                }

                fprintf(stderr, " ");

                vpxt_run_exe(Program);
            }
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    if (vpxt_file_size(OldEncFile.c_str(), 0) == 0)
    {
        tprintf("\nError - Old File Incorrect\n");
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    //////////////////////////////////////////////Input OLD PSNR//////////////////////////////////////////////
    char TimeTextFile[256];

    vpxt_folder_name(NewEncFile.c_str(), TimeTextFile);
    string TimeTextFileStr = TimeTextFile;

    TimeTextFileStr.append("outputVP8Old_OLD_PSNR.txt");

    float PSNROLD;
    ifstream infile(TimeTextFileStr.c_str());
    infile >> PSNROLD;
    infile.close();
    /////////////////////////////////////////////////////////////

    double PSNRArr[2];

    PSNRArr[0] = vpxt_ivf_psnr(input, NewEncFile.c_str(), 0, 0, 1, NULL);
    PSNRArr[1] = PSNROLD;

    tprintf("\nNew DataRate");
    vpxt_ivf_data_rate(NewEncFile.c_str(), 1);

    tprintf("\nOld DataRate");
    vpxt_ivf_data_rate(OldEncFile.c_str(), 1);

    tprintf("\n\nResults:\n\n");

    if (PSNRArr[0] > PSNRArr[1])
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "New PSNR: %.2f > Old PSNR: %.2f - Passed", PSNRArr[0], PSNRArr[1]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (PSNRArr[0] <= PSNRArr[1] + (PSNRArr[1] * 0.005) && PSNRArr[0] >= PSNRArr[1] - (PSNRArr[1] * 0.005))
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "New PSNR: %.2f is with in 1/2%% of old: %.2f - Min Passed", PSNRArr[0], PSNRArr[1]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\nMin Passed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 8;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "New PSNR: %.2f < Old PSNR: %.2f - Failed", PSNRArr[0], PSNRArr[1]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\n New PSNR: %.2f lower than Old PSNR: %.2f - Failed\n", PSNRArr[0], PSNRArr[1]);
        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}