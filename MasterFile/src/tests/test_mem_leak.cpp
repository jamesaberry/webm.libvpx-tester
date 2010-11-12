#include "vpxt_test_declarations.h"

int test_mem_leak(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    if (!(argc == 6 || argc == 7))
    {
        printf(
            "  MemLeakCheck \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Mem Leak Check Exe>\n" //Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>
            "    <Optional Settings File>\n"
            "\n");
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char MemLeakExe[255];

    string MemLeakExeStr = argv[5];
    snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());

    //#if defined(_WIN32)
    //        {
    //            snprintf(MemLeakExe, 255, "%s", argv[5]);
    //        }
    //#elif defined(linux)
    //        {
    //            string MemLeakExeStr = argv[5];
    //            snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    //        }
    //#elif defined(__APPLE__)
    //        {
    //            string MemLeakExeStr = argv[5];
    //            snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    //        }
    //#elif defined(__POWERPC__)
    //        {
    //            string MemLeakExeStr = argv[5];
    //            snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());
    //        }
    //#endif

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char ExeCharMemLeak[1024] = "";
    char *MyDir = "MemLeakCheck";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    //Get the exe's parent folder From argv[0] Paths for both exes will be the same
    vpxt_folder_name(argv[0], ExeCharMemLeak);
    string ExeCharMemLeakStr = ExeCharMemLeak;

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string WorkingDir7 = WorkingDirString;
    string WorkingDir8 = WorkingDirString;
    string WorkingDir9 = "";
    string WorkingDir10 = "";
    string WorkingDir11 = "";


    WorkingDir4.append(slashCharStr());
    WorkingDir4.append("MemLeakCheck.ivf");

    WorkingDir7.append(slashCharStr());
    WorkingDir7.append("MemLeakCheckOutput.txt");

    WorkingDir8.append(slashCharStr());
    WorkingDir8.append("MemLeakCheckParFile.txt");

#if defined(_WIN32)
    {
        WorkingDir9 = "\"\"";
        WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
        WorkingDir9.append(MemLeakExe);         // Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);              // Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);        // Output
        WorkingDir9.append("\" 8 \"");
        WorkingDir9.append(WorkingDir8);        // Par File
        WorkingDir9.append("\" 4 \"");
        WorkingDir9.append(WorkingDir7);        // Mem Output File
        WorkingDir9.append("\"\"");
        WorkingDir10.append(slashCharStr());
        WorkingDir10.append("MemLeakCheckOutput_FP.txt");
        WorkingDir11 = "\"\"";
        WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
        WorkingDir11.append(MemLeakExe);        // Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);             // Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);       // Output
        WorkingDir11.append("\" 8 \"");
        WorkingDir11.append(WorkingDir8);       // Par File
        WorkingDir11.append("\" 4 \"");
        WorkingDir11.append(WorkingDir10);      // Mem Output File
        WorkingDir11.append("\"\"");
    }
#else
    {
        WorkingDir9 = "\"";
        WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
        WorkingDir9.append(MemLeakExe);         // Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);              // Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);        // Output
        WorkingDir9.append("\" 8 \"");
        WorkingDir9.append(WorkingDir8);        // Par File
        WorkingDir9.append("\" 4 \"");
        WorkingDir9.append(WorkingDir7);        // Mem Output File
        WorkingDir9.append("\"");
        WorkingDir10.append(slashCharStr());
        WorkingDir10.append("MemLeakCheckOutput_FP.txt");
        WorkingDir11 = "\"";
        WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
        WorkingDir11.append(MemLeakExe);            // Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);             // Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);       // Output
        WorkingDir11.append("\" 8 \"");
        WorkingDir11.append(WorkingDir8);       // Par File
        WorkingDir11.append("\" 4 \"");
        WorkingDir11.append(WorkingDir10);      // Mem Output File
        WorkingDir11.append("\"");
    }
#endif

    char MemLeakCheckIVF[255];
    char MemLeakCheckTXT[255];
    char MemLeakCheckTXT_FP[255];
    char MemLeakCheckParFile[255];
    char ProgramMemLeakCheck[2048];
    char ProgramMemLeakCheck_FirstPass[2048];

    snprintf(MemLeakCheckIVF, 255, "%s", WorkingDir4.c_str());
    snprintf(MemLeakCheckTXT, 255, "%s", WorkingDir7.c_str());
    snprintf(MemLeakCheckParFile, 255, "%s", WorkingDir8.c_str());
    snprintf(ProgramMemLeakCheck, 2048, "%s", WorkingDir9.c_str());
    snprintf(MemLeakCheckTXT_FP, 255, "%s", WorkingDir10.c_str());
    snprintf(ProgramMemLeakCheck_FirstPass, 2048, "%s", WorkingDir11.c_str());

    //cout << "\n\n\n" << ProgramMemLeakCheck << "\n\n\n";
    //cout << "\n\n\n" << ProgramMemLeakCheck_FirstPass << "\n\n\n";

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
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


    tprintf("Mem Leak Check Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 7)
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

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf("\nInput encode file %s does not exist\n", argv[2]);
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate ;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
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
            vpxt_output_settings(MemLeakCheckParFile, opt);
            vpxt_run_exe(ProgramMemLeakCheck);
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
            vpxt_output_settings(MemLeakCheckParFile, opt);
            vpxt_run_exe(ProgramMemLeakCheck);
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
            vpxt_output_settings(MemLeakCheckParFile, opt);
            vpxt_run_exe(ProgramMemLeakCheck);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            vpxt_output_settings(MemLeakCheckParFile, opt);
            vpxt_run_exe(ProgramMemLeakCheck);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            vpxt_output_settings(MemLeakCheckParFile, opt);
            vpxt_run_exe(ProgramMemLeakCheck);
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

    int fail = 0;

    tprintf("\n\nResults:\n\n");

    ifstream infile4(MemLeakCheckTXT);

    if (!infile4.good())
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "MemLeakCheckOutput.txt not found - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nFailed\n");

        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
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
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Memory Currently Allocated == 0 - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Memory Currently Allocated != 0 - %s - Failed", bufferString4.c_str());
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf("\nPassed\n");

        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        tprintf("\nFailed\n");

        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 0;
}