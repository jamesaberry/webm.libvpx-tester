#include "vpxt_test_declarations.h"

int test_mem_leak(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    //Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>
    char *MyDir = "test_mem_leak";

    if (!(argc == 6 || argc == 7))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        printf(
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
            "    <Mem Leak Check Exe>\n"
            "    <Optional Settings File>\n"
            "\n"
        );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char MemLeakExe[255];
    string MemLeakExeStr = argv[5];
    snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    char ExeCharMemLeak[1024] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    //Get the exe's parent folder From argv[0] Paths for both exes will be the same
    vpxt_folder_name(argv[0], ExeCharMemLeak);

    string MemLeakCheckIVFStr = CurTestDirStr;
    MemLeakCheckIVFStr.append(slashCharStr());
    MemLeakCheckIVFStr.append(MyDir);
    MemLeakCheckIVFStr.append("_compression.ivf");

    string MemLeakCheckTxtStr = CurTestDirStr;
    MemLeakCheckTxtStr.append(slashCharStr());
    MemLeakCheckTxtStr.append(MyDir);
    MemLeakCheckTxtStr.append("_compression_output.txt");

    string MemLeakCheckParFileStr = CurTestDirStr;
    MemLeakCheckParFileStr.append(slashCharStr());
    MemLeakCheckParFileStr.append(MyDir);
    MemLeakCheckParFileStr.append("_compression_parameter_file.txt");

    string ExeCharMemLeakStr = ExeCharMemLeak;
    string ProgramMemLeakCheckStr = "";
    string MemLeakCheckTXT_FPStr = "";
    string ProgramMemLeakCheck_FirstPassStr = "";

#if defined(_WIN32)
    {
        ProgramMemLeakCheckStr = "\"\"";
        ProgramMemLeakCheckStr.append(ExeCharMemLeakStr.c_str());  // Exe Path
        ProgramMemLeakCheckStr.append(MemLeakExe);         // Exe Name
        ProgramMemLeakCheckStr.append("\" \"");
        ProgramMemLeakCheckStr.append(input);              // Input
        ProgramMemLeakCheckStr.append("\" \"");
        ProgramMemLeakCheckStr.append(MemLeakCheckIVFStr.c_str());        // Output
        ProgramMemLeakCheckStr.append("\" 8 \"");
        ProgramMemLeakCheckStr.append(MemLeakCheckParFileStr);        // Par File
        ProgramMemLeakCheckStr.append("\" 4 \"");
        ProgramMemLeakCheckStr.append(MemLeakCheckTxtStr.c_str());        // Mem Output File
        ProgramMemLeakCheckStr.append("\"\"");
        MemLeakCheckTXT_FPStr.append(slashCharStr());
        MemLeakCheckTXT_FPStr.append("MemLeakCheckOutput_FP.txt");
        ProgramMemLeakCheck_FirstPassStr = "\"\"";
        ProgramMemLeakCheck_FirstPassStr.append(ExeCharMemLeakStr); // Exe Path
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakExe);        // Exe Name
        ProgramMemLeakCheck_FirstPassStr.append("\" \"");
        ProgramMemLeakCheck_FirstPassStr.append(input);             // Input
        ProgramMemLeakCheck_FirstPassStr.append("\" \"");
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakCheckIVFStr.c_str());       // Output
        ProgramMemLeakCheck_FirstPassStr.append("\" 8 \"");
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakCheckParFileStr);       // Par File
        ProgramMemLeakCheck_FirstPassStr.append("\" 4 \"");
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakCheckTXT_FPStr.c_str());      // Mem Output File
        ProgramMemLeakCheck_FirstPassStr.append("\"\"");
    }
#else
    {
        ProgramMemLeakCheckStr = "\"";
        ProgramMemLeakCheckStr.append(ExeCharMemLeakStr.c_str());  // Exe Path
        ProgramMemLeakCheckStr.append(MemLeakExe);         // Exe Name
        ProgramMemLeakCheckStr.append("\" \"");
        ProgramMemLeakCheckStr.append(input);              // Input
        ProgramMemLeakCheckStr.append("\" \"");
        ProgramMemLeakCheckStr.append(MemLeakCheckIVFStr.c_str());        // Output
        ProgramMemLeakCheckStr.append("\" 8 \"");
        ProgramMemLeakCheckStr.append(MemLeakCheckParFileStr);        // Par File
        ProgramMemLeakCheckStr.append("\" 4 \"");
        ProgramMemLeakCheckStr.append(MemLeakCheckTxtStr.c_str());        // Mem Output File
        ProgramMemLeakCheckStr.append("\"");
        MemLeakCheckTXT_FPStr.append(slashCharStr());
        MemLeakCheckTXT_FPStr.append("MemLeakCheckOutput_FP.txt");
        ProgramMemLeakCheck_FirstPassStr = "\"";
        ProgramMemLeakCheck_FirstPassStr.append(ExeCharMemLeakStr); // Exe Path
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakExe);            // Exe Name
        ProgramMemLeakCheck_FirstPassStr.append("\" \"");
        ProgramMemLeakCheck_FirstPassStr.append(input);             // Input
        ProgramMemLeakCheck_FirstPassStr.append("\" \"");
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakCheckIVFStr.c_str());       // Output
        ProgramMemLeakCheck_FirstPassStr.append("\" 8 \"");
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakCheckParFileStr);       // Par File
        ProgramMemLeakCheck_FirstPassStr.append("\" 4 \"");
        ProgramMemLeakCheck_FirstPassStr.append(MemLeakCheckTXT_FPStr.c_str());      // Mem Output File
        ProgramMemLeakCheck_FirstPassStr.append("\"");
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
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
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

    vpxt_cap_string_print(PRINT_BOTH, "%s", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 7)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf("\nInput Settings file %s does not exist\n", argv[argc-1]);

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
        tprintf("\nInput executable %s does not exist\n", argv[5]);

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf("\nInput encode file %s does not exist\n", argv[2]);

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate ;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        if (Mode == 0)
        {
            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            opt.Mode = MODE_REALTIME;
            vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
            vpxt_run_exe(ProgramMemLeakCheckStr.c_str());
        }

        if (Mode == 1)
        {
            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            opt.Mode = MODE_GOODQUALITY;
            vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
            vpxt_run_exe(ProgramMemLeakCheckStr.c_str());
        }

        if (Mode == 2)
        {
            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            opt.Mode = MODE_BESTQUALITY;
            vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
            vpxt_run_exe(ProgramMemLeakCheckStr.c_str());
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
            vpxt_run_exe(ProgramMemLeakCheckStr.c_str());
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
            vpxt_run_exe(ProgramMemLeakCheckStr.c_str());
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

    int fail = 0;

    tprintf("\n\nResults:\n\n");

    ifstream infile4(MemLeakCheckTxtStr.c_str());

    if (!infile4.good())
    {
        vpxt_formated_print(RESPRT, "MemLeakCheckOutput.txt not found - Failed");
        tprintf("\n");

        tprintf("\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    char buffer4[256];

    infile4.getline(buffer4, 256);
    infile4.getline(buffer4, 256);
    infile4.getline(buffer4, 256);
    infile4.getline(buffer4, 256);

    string bufferString4 = buffer4;

    if (bufferString4.compare(0, 24, "_currently Allocated= 0;") == 0)
    {
        vpxt_formated_print(RESPRT, "Memory Currently Allocated == 0 - Passed");
        tprintf("\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Memory Currently Allocated != 0 - %s - Failed", bufferString4.c_str());
        tprintf("\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf("\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf("\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 0;
}