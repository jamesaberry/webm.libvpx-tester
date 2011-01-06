#include "vpxt_test_declarations.h"

int test_mem_leak(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType)
{
    //Needs Debug.exe
    char *MyDir = "test_mem_leak";

    if (!(argc == 6 || argc == 7))
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
                "    <Mem Leak Check Exe>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char MemLeakExe[255];
    std::string MemLeakExeStr = argv[5];
    snprintf(MemLeakExe, 255, "%s", MemLeakExeStr.c_str());

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    char ExeCharMemLeak[1024] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    //Get the exe's parent folder From argv[0] Paths for both exes will be the same
    vpxt_folder_name(argv[0], ExeCharMemLeak);

    /////////////////////////////////////////////////
    std::string MemLeakCheckIVFDECStr = CurTestDirStr;
    MemLeakCheckIVFDECStr.append(slashCharStr());
    MemLeakCheckIVFDECStr.append(MyDir);
    MemLeakCheckIVFDECStr.append("_decompression.ivf");
    /////////////////////////////////////////////////

    std::string MemLeakCheckIVFStr = CurTestDirStr;
    MemLeakCheckIVFStr.append(slashCharStr());
    MemLeakCheckIVFStr.append(MyDir);
    MemLeakCheckIVFStr.append("_compression.ivf");

    std::string EncMemLeakCheckTxtStr = CurTestDirStr;
    EncMemLeakCheckTxtStr.append(slashCharStr());
    EncMemLeakCheckTxtStr.append(MyDir);
    EncMemLeakCheckTxtStr.append("_compression_memory_summary.txt");

    std::string DecMemLeakCheckTxtStr = CurTestDirStr;
    DecMemLeakCheckTxtStr.append(slashCharStr());
    DecMemLeakCheckTxtStr.append(MyDir);
    DecMemLeakCheckTxtStr.append("_decompression_memory_summary.txt");

    std::string MemLeakCheckParFileStr = CurTestDirStr;
    MemLeakCheckParFileStr.append(slashCharStr());
    MemLeakCheckParFileStr.append(MyDir);
    MemLeakCheckParFileStr.append("_compression_parameter_file.txt");

    std::string ProgramEncMemLeakCheckStr = "";
    std::string ProgramDecMemLeakCheckStr = "";

#if defined(_WIN32)
    {
        //compression
        ProgramEncMemLeakCheckStr = "\"\"";
        ProgramEncMemLeakCheckStr.append(ExeCharMemLeak);  // Exe Path
        ProgramEncMemLeakCheckStr.append(MemLeakExe);         // Exe Name
        ProgramEncMemLeakCheckStr.append("\" memcompress \"");
        ProgramEncMemLeakCheckStr.append(input.c_str());              // Input
        ProgramEncMemLeakCheckStr.append("\" \"");
        ProgramEncMemLeakCheckStr.append(MemLeakCheckIVFStr.c_str());        // Output
        ProgramEncMemLeakCheckStr.append("\" 8 \"");
        ProgramEncMemLeakCheckStr.append(MemLeakCheckParFileStr.c_str());        // Par File
        ProgramEncMemLeakCheckStr.append("\" 0 \"");
        ProgramEncMemLeakCheckStr.append(EncMemLeakCheckTxtStr.c_str());        // Mem Output File
        ProgramEncMemLeakCheckStr.append("\"\"");
        //decompression
        ProgramDecMemLeakCheckStr = "\"\"";
        ProgramDecMemLeakCheckStr.append(ExeCharMemLeak);  // Exe Path
        ProgramDecMemLeakCheckStr.append(MemLeakExe);         // Exe Name
        ProgramDecMemLeakCheckStr.append("\" memdecompress \"");
        ProgramDecMemLeakCheckStr.append(MemLeakCheckIVFStr.c_str());              // Input
        ProgramDecMemLeakCheckStr.append("\" \"");
        ProgramDecMemLeakCheckStr.append(MemLeakCheckIVFDECStr.c_str());        // Output
        ProgramDecMemLeakCheckStr.append("\" \"");
        ProgramDecMemLeakCheckStr.append(DecMemLeakCheckTxtStr.c_str());        // Par File
        ProgramDecMemLeakCheckStr.append("\"\"");
    }
#else
    {
        //compression
        ProgramEncMemLeakCheckStr = "\"";
        ProgramEncMemLeakCheckStr.append(ExeCharMemLeak);  // Exe Path
        ProgramEncMemLeakCheckStr.append(MemLeakExe);         // Exe Name
        ProgramEncMemLeakCheckStr.append("\" memcompress \"");
        ProgramEncMemLeakCheckStr.append(input.c_str());              // Input
        ProgramEncMemLeakCheckStr.append("\" \"");
        ProgramEncMemLeakCheckStr.append(MemLeakCheckIVFStr.c_str());        // Output
        ProgramEncMemLeakCheckStr.append("\" 8 \"");
        ProgramEncMemLeakCheckStr.append(MemLeakCheckParFileStr.c_str());        // Par File
        ProgramEncMemLeakCheckStr.append("\" 0 \"");
        ProgramEncMemLeakCheckStr.append(EncMemLeakCheckTxtStr.c_str());        // Mem Output File
        ProgramEncMemLeakCheckStr.append("\"");
        //decompression
        ProgramDecMemLeakCheckStr = "\"";
        ProgramDecMemLeakCheckStr.append(ExeCharMemLeak);  // Exe Path
        ProgramDecMemLeakCheckStr.append(MemLeakExe);         // Exe Name
        ProgramDecMemLeakCheckStr.append("\" memdecompress \"");
        ProgramDecMemLeakCheckStr.append(MemLeakCheckIVFStr.c_str());              // Input
        ProgramDecMemLeakCheckStr.append("\" \"");
        ProgramDecMemLeakCheckStr.append(MemLeakCheckIVFDECStr.c_str());        // Output
        ProgramDecMemLeakCheckStr.append("\" \"");
        ProgramDecMemLeakCheckStr.append(DecMemLeakCheckTxtStr.c_str());        // Par File
        ProgramDecMemLeakCheckStr.append("\"");
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
    if (argc == 7)
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

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf(PRINT_BTH, "\nInput encode file %s does not exist\n", argv[2]);

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
        fclose(fp);

        if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file4.\n");
            exit(1);
        }

        fprintf(stderr, " ");

        opt.Mode = Mode;
        vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
        vpxt_run_exe(ProgramEncMemLeakCheckStr.c_str());
        vpxt_run_exe(ProgramDecMemLeakCheckStr.c_str());
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

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    std::string MemCompressResults = "";
    std::string MemDecompressResults = "";

    if (vpxt_check_mem_state(EncMemLeakCheckTxtStr, MemCompressResults) == -1)
    {
        vpxt_formated_print(RESPRT, "%s not found - Failed", EncMemLeakCheckTxtStr);
        fail = 1;
    }
    else
    {
        if (MemCompressResults.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            vpxt_formated_print(RESPRT, "Compression Memory Currently Allocated == 0 - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "Compression Memory Currently Allocated != 0 - %s - Failed", MemCompressResults.c_str());
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (vpxt_check_mem_state(DecMemLeakCheckTxtStr, MemDecompressResults) == -1)
    {
        vpxt_formated_print(RESPRT, "%s not found - Failed", EncMemLeakCheckTxtStr);
        fail = 1;
    }
    else
    {
        if (MemDecompressResults.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            vpxt_formated_print(RESPRT, "Decompression Memory Currently Allocated == 0 - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "Decompression Memory Currently Allocated != 0 - %s - Failed", MemDecompressResults.c_str());
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 0;
}