#include "vpxt_test_declarations.h"

int test_mem_leak2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    // So long as Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>

    if (!(argc == 4 || argc == 5))
    {
        printf(
            "  MemLeakCheck2 \n\n"
            "    <Mem Leak Check Exe>\n"
            "    <Decode Test File>\n"
            "\n");
        return 0;
    }

    char *input = "Blank";
    int Mode = 1;
    int BitRate = 128;
    char MemLeakExe[255];
    char DecInFile[255];

    snprintf(MemLeakExe, 255, "%s", argv[2]);
    snprintf(DecInFile, 255, "%s", argv[3]);

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char ExeCharMemLeak[1024] = "";
    char *MyDir = "MemLeakCheck2";
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
    string WorkingDir9;

    WorkingDir4.append(slashCharStr());
    WorkingDir4.append("MemLeakCheck.ivf");

    WorkingDir7.append(slashCharStr());
    WorkingDir7.append("MemLeakCheckOutput");

    WorkingDir8.append(slashCharStr());
    WorkingDir8.append("MemLeakCheckParFile");

#if defined(_WIN32)
    {
        WorkingDir9 = "\"\"";
        WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
        WorkingDir9.append(MemLeakExe);         // Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);              // Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);        // Output
        WorkingDir9.append("\" 0 \"");
        WorkingDir9.append(WorkingDir8);        // Par File
        WorkingDir9.append("\" 5 \"");
        WorkingDir9.append(WorkingDir7);        // Mem Output File
        WorkingDir9.append("\"\"");
    }

#elif defined(linux)
    {
        WorkingDir9 = "\'";
        WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
        WorkingDir9.append(MemLeakExe);         // Exe Name
        WorkingDir9.append("\' \'");
        WorkingDir9.append(input);              // Input
        WorkingDir9.append("\' \'");
        WorkingDir9.append(WorkingDir4);        // Output
        WorkingDir9.append("\' 0 \'");
        WorkingDir9.append(WorkingDir8);        // Par File
        WorkingDir9.append("\' 5 \'");
        WorkingDir9.append(WorkingDir7);        // Mem Output File
        WorkingDir9.append("\'");
    }
#elif defined(__APPLE__)
    {
        WorkingDir9 = "\"";
        WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
        WorkingDir9.append(MemLeakExe);         // Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);              // Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);        // Output
        WorkingDir9.append("\" 0 \"");
        WorkingDir9.append(WorkingDir8);        // Par File
        WorkingDir9.append("\" 5 \"");
        WorkingDir9.append(WorkingDir7);        // Mem Output File
        WorkingDir9.append("\"");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir9 = "\"";
        WorkingDir9.append(ExeCharMemLeakStr);  // Exe Path
        WorkingDir9.append(MemLeakExe);         // Exe Name
        WorkingDir9.append("\" \"");
        WorkingDir9.append(input);              // Input
        WorkingDir9.append("\" \"");
        WorkingDir9.append(WorkingDir4);        // Output
        WorkingDir9.append("\" 0 \"");
        WorkingDir9.append(WorkingDir8);        // Par File
        WorkingDir9.append("\" 5 \"");
        WorkingDir9.append(WorkingDir7);        // Mem Output File
        WorkingDir9.append("\"");
    }
#endif


    string WorkingDir10 = WorkingDirString;
    string WorkingDir11;
    string WorkingDir12;

    WorkingDir10.append(slashCharStr());
    WorkingDir10.append("MemLeakCheckOutput");

    string MemLeakCheckResultfile1 = WorkingDir10;
    string MemLeakCheckResultfile2 = WorkingDir10;
    MemLeakCheckResultfile1.append("_Encode.txt");
    MemLeakCheckResultfile2.append("_Decode.txt");

    char MemLeakCheckTXT1[255];
    char MemLeakCheckTXT2[255];
    snprintf(MemLeakCheckTXT1, 255, "%s", MemLeakCheckResultfile1.c_str());
    snprintf(MemLeakCheckTXT2, 255, "%s", MemLeakCheckResultfile2.c_str());

#if defined(_WIN32)
    {
        WorkingDir11 = "\"\"";
        WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
        WorkingDir11.append(MemLeakExe);            // Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);             // Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);       // Output
        WorkingDir11.append("\" 0 \"");
        WorkingDir11.append(WorkingDir8);       // Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\" 5 \"");
        WorkingDir12.append("\" 6 \"");
        WorkingDir11.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append("\" \"");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\"");
        WorkingDir12.append("\"");
    }
#elif defined(linux)
    {
        WorkingDir11 = "\'";
        WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
        WorkingDir11.append(MemLeakExe);            // Exe Name
        WorkingDir11.append("\' \'");
        WorkingDir11.append(input);             // Input
        WorkingDir11.append("\' \'");
        WorkingDir11.append(WorkingDir4);       // Output
        WorkingDir11.append("\' 0 \'");
        WorkingDir11.append(WorkingDir8);       // Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\' 5 \'");
        WorkingDir12.append("\' 6 \'");
        WorkingDir11.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append("\' \'");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\'");
        WorkingDir12.append("\'");
    }
#elif defined(__APPLE__)
    {
        WorkingDir11 = "\"";
        WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
        WorkingDir11.append(MemLeakExe);            // Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);             // Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);       // Output
        WorkingDir11.append("\" 0 \"");
        WorkingDir11.append(WorkingDir8);       // Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\" 5 \"");
        WorkingDir12.append("\" 6 \"");
        WorkingDir11.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append("\" \"");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\"");
        WorkingDir12.append("\"");
    }
#elif defined(__POWERPC__)
    {
        WorkingDir11 = "\"";
        WorkingDir11.append(ExeCharMemLeakStr); // Exe Path
        WorkingDir11.append(MemLeakExe);            // Exe Name
        WorkingDir11.append("\" \"");
        WorkingDir11.append(input);             // Input
        WorkingDir11.append("\" \"");
        WorkingDir11.append(WorkingDir4);       // Output
        WorkingDir11.append("\" 0 \"");
        WorkingDir11.append(WorkingDir8);       // Par File
        WorkingDir12 = WorkingDir11;
        WorkingDir11.append("\" 5 \"");
        WorkingDir12.append("\" 6 \"");
        WorkingDir11.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append(WorkingDir10);      // Mem Output File
        WorkingDir12.append("\" \"");
        WorkingDir12.append(DecInFile);
        WorkingDir11.append("\"");
        WorkingDir12.append("\"");
    }
#endif


    char MemLeakCheckIVF[255];
    char MemLeakCheckTXT[255];
    char MemLeakCheckParFile[255];
    char ProgramMemLeakCheckEnc[2048];
    char ProgramMemLeakCheckDec[2048];

    snprintf(MemLeakCheckIVF, 255, "%s", WorkingDir4.c_str());
    snprintf(MemLeakCheckTXT, 255, "%s", WorkingDir7.c_str());
    snprintf(MemLeakCheckParFile, 255, "%s", WorkingDir8.c_str());
    snprintf(ProgramMemLeakCheckEnc, 2048, "%s", WorkingDir9.c_str());
    snprintf(ProgramMemLeakCheckDec, 2048, "%s", WorkingDir12.c_str());

    //cout << "\n\n\n" << ProgramMemLeakCheckEnc << "\n\n\n";
    //cout << "\n\n\n" << ProgramMemLeakCheckDec << "\n\n\n";

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


    tprintf("Mem Leak Check 2 Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
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
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf("\nInput executable %s does not exist\n", argv[2]);
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(argv[3]))
    {
        tprintf("\nInput decode file %s does not exist\n", argv[3]);
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
        fclose(fp);

        if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
        {
            printf("Cannot open out put file4.\n");
            exit(1);
        }

        fprintf(stderr, " ");

        opt.Mode = MODE_GOODQUALITY;
        vpxt_output_settings(MemLeakCheckParFile, opt);
        vpxt_run_exe(ProgramMemLeakCheckEnc);
        vpxt_run_exe(ProgramMemLeakCheckDec);
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

    FILE *infile4 = fopen(MemLeakCheckTXT1, "rb");

    if (infile4 == NULL)
    {
        char OutputChar1[255];
        char MemLeakCheckTXTFileName[200];
        vpxt_file_name(MemLeakCheckTXT1, MemLeakCheckTXTFileName, 0);
        snprintf(OutputChar1, 255, "File not found: %s - Failed", MemLeakCheckTXTFileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char buffer4[256];

        fgets(buffer4 , 256 , infile4);
        fgets(buffer4 , 256 , infile4);
        fgets(buffer4 , 256 , infile4);
        fgets(buffer4 , 256 , infile4);

        string bufferString4 = buffer4;

        if (bufferString4.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Encode Memory Currently Allocated == 0 - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Encode Memory Currently Allocated != 0 - %s - Failed", bufferString4.c_str());
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
    }

    FILE *infile5 = fopen(MemLeakCheckTXT2, "rb");

    if (infile5 == NULL)
    {
        char OutputChar1[255];
        char MemLeakCheckTXTFileName[200];
        vpxt_file_name(MemLeakCheckTXT2, MemLeakCheckTXTFileName, 0);
        snprintf(OutputChar1, 255, "File not found: %s - Failed", MemLeakCheckTXTFileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {

        char buffer5[256];

        fgets(buffer5 , 256 , infile5);
        fgets(buffer5 , 256 , infile5);
        fgets(buffer5 , 256 , infile5);
        fgets(buffer5 , 256 , infile5);

        string bufferString5 = buffer5;

        if (bufferString5.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Decode Memory Currently Allocated == 0 - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Decode Memory Currently Allocated != 0 - %s - Failed", bufferString5.c_str());
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
    }

    if (infile4 != NULL)fclose(infile4);

    if (infile5 != NULL)fclose(infile5);

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