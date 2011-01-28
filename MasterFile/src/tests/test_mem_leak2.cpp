#include "vpxt_test_declarations.h"

int test_mem_leak2(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    //Needs Debug.exe
    char *MyDir = "test_mem_leak2";

    if (!(argc == 4 || argc == 5))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        tprintf(PRINT_STD,
                "\n\n"
                "    <Mem Leak Check Exe>\n"
                "    <Decode Test File>\n"
                "\n"
               );
        return 0;
    }

    char MemLeakExe[255];
    char DecInFile[255];
    snprintf(MemLeakExe, 255, "%s", argv[2]);
    snprintf(DecInFile, 255, "%s", argv[3]);

    std::string input = "Blank";
    int Mode = 1;
    int BitRate = 128;

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
    std::string ExeCharMemLeakStr = ExeCharMemLeak;

    std::string MemLeakCheckTXTBase = CurTestDirStr;
    MemLeakCheckTXTBase.append(slashCharStr());
    MemLeakCheckTXTBase.append(MyDir);

    std::string MemLeakCheckTXT1Str = MemLeakCheckTXTBase;
    MemLeakCheckTXT1Str.append("_compression_memory_summary.txt");

    std::string MemLeakCheckTXT2Str = MemLeakCheckTXTBase;
    MemLeakCheckTXT2Str.append("_decompression_memory_summary.txt");

    std::string ProgramMemLeakCheckEncStr;
    std::string ProgramMemLeakCheckDecStr;

#if defined(_WIN32)
    //Faux Compress
    ProgramMemLeakCheckEncStr = "\"\"";
    ProgramMemLeakCheckEncStr.append(ExeCharMemLeakStr);            // Exe Path
    ProgramMemLeakCheckEncStr.append(MemLeakExe);                   // Exe Name
    ProgramMemLeakCheckEncStr.append("\" fauxcompress \"");
    ProgramMemLeakCheckEncStr.append(MemLeakCheckTXT1Str.c_str());  // Output txt file
    ProgramMemLeakCheckEncStr.append("\"\"");
    //Faux Decompress
    ProgramMemLeakCheckDecStr = "\"\"";
    ProgramMemLeakCheckDecStr.append(ExeCharMemLeakStr);            // Exe Path
    ProgramMemLeakCheckDecStr.append(MemLeakExe);                   // Exe Name
    ProgramMemLeakCheckDecStr.append("\" fauxdecompress \"");
    ProgramMemLeakCheckDecStr.append(MemLeakCheckTXT2Str.c_str());  // Output txt file
    ProgramMemLeakCheckDecStr.append("\" \"");
    ProgramMemLeakCheckDecStr.append(DecInFile);                    // Input faux dec file
    ProgramMemLeakCheckDecStr.append("\"\"");
#else
    //Faux Compress
    ProgramMemLeakCheckEncStr = "\'";
    ProgramMemLeakCheckEncStr.append(ExeCharMemLeakStr);            // Exe Path
    ProgramMemLeakCheckEncStr.append(MemLeakExe);                   // Exe Name
    ProgramMemLeakCheckEncStr.append("\' fauxcompress \'");
    ProgramMemLeakCheckEncStr.append(MemLeakCheckTXT1Str.c_str());  // Output txt file
    ProgramMemLeakCheckEncStr.append("\'");
    //Faux Decompress
    ProgramMemLeakCheckDecStr = "\'";
    ProgramMemLeakCheckDecStr.append(ExeCharMemLeakStr);            // Exe Path
    ProgramMemLeakCheckDecStr.append(MemLeakExe);                   // Exe Name
    ProgramMemLeakCheckDecStr.append("\' fauxdecompress \'");
    ProgramMemLeakCheckDecStr.append(MemLeakCheckTXT2Str.c_str());  // Output txt file
    ProgramMemLeakCheckDecStr.append("\' \'");
    ProgramMemLeakCheckDecStr.append(DecInFile);                    // Input faux dec file
    ProgramMemLeakCheckDecStr.append("\'");
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
    if (argc == 6)
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
    if (!vpxt_file_exists_check(argv[2]))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", argv[2]);

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(argv[3]))
    {
        tprintf(PRINT_BTH, "\nInput decode file %s does not exist\n", argv[3]);

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

        //opt.Mode = MODE_GOODQUALITY;
        //vpxt_output_settings(MemLeakCheckParFileStr.c_str(), opt);
        vpxt_run_exe(ProgramMemLeakCheckEncStr.c_str());
        vpxt_run_exe(ProgramMemLeakCheckDecStr.c_str());
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

    FILE *infile4 = fopen(MemLeakCheckTXT1Str.c_str(), "rb");

    if (infile4 == NULL)
    {
        char MemLeakCheckTXTFileName[200];
        vpxt_file_name(MemLeakCheckTXT1Str.c_str(), MemLeakCheckTXTFileName, 0);
        vpxt_formated_print(RESPRT, "File not found: %s - Failed", MemLeakCheckTXTFileName);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        char buffer4[256];

        fgets(buffer4 , 256 , infile4);
        fgets(buffer4 , 256 , infile4);
        fgets(buffer4 , 256 , infile4);
        fgets(buffer4 , 256 , infile4);

        std::string bufferString4 = buffer4;

        if (bufferString4.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            vpxt_formated_print(RESPRT, "Encode Memory Currently Allocated == 0 - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "Encode Memory Currently Allocated != 0 - %s - Failed", bufferString4.c_str());
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    FILE *infile5 = fopen(MemLeakCheckTXT2Str.c_str(), "rb");

    if (infile5 == NULL)
    {
        char MemLeakCheckTXTFileName[200];
        vpxt_file_name(MemLeakCheckTXT2Str.c_str(), MemLeakCheckTXTFileName, 0);
        vpxt_formated_print(RESPRT, "File not found: %s - Failed", MemLeakCheckTXTFileName);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        char buffer5[256];

        fgets(buffer5 , 256 , infile5);
        fgets(buffer5 , 256 , infile5);
        fgets(buffer5 , 256 , infile5);
        fgets(buffer5 , 256 , infile5);

        std::string bufferString5 = buffer5;

        if (bufferString5.compare(0, 24, "_currently Allocated= 0;") == 0)
        {
            vpxt_formated_print(RESPRT, "Decode Memory Currently Allocated == 0 - Passed");
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "Decode Memory Currently Allocated != 0 - %s - Failed", bufferString5.c_str());
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (infile4 != NULL)fclose(infile4);

    if (infile5 != NULL)fclose(infile5);

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