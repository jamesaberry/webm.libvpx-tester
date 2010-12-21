#include "vpxt_test_declarations.h"

int test_debug_matches_release(int argc, char *argv[], const string& WorkingDir, string FilesAr[], int TestType)
{
    //Needs Debug.exe and Release.exe
    char *MyDir = "test_debug_matches_release";

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
                "    <Debug Executable - Must take <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>\n"
                "    <Release Executable-Must take <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char *ExeInputDebug = argv[5];
    char ExeInputRelease[255];
    snprintf(ExeInputRelease, 255, "%s", argv[6]);

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    char ExeCharDebugRelease[1024];
    vpxt_folder_name(argv[0], ExeCharDebugRelease);
    string ExeCharDebugReleaseString = ExeCharDebugRelease;

    string DebugExeLoc = ExeCharDebugReleaseString;
    DebugExeLoc.append(ExeInputDebug);

    string ReleaseExeLoc = ExeCharDebugReleaseString;
    ReleaseExeLoc.append(ExeInputRelease);

    string DebugOutputEnc = CurTestDirStr;
    DebugOutputEnc.append(slashCharStr());
    DebugOutputEnc.append(MyDir);
    DebugOutputEnc.append("_compression_debug.ivf");

    string ReleaseOutputEnc = CurTestDirStr;
    ReleaseOutputEnc.append(slashCharStr());
    ReleaseOutputEnc.append(MyDir);
    ReleaseOutputEnc.append("_compression_release.ivf");

    string DebugOutputDec = CurTestDirStr;
    DebugOutputDec.append(slashCharStr());
    DebugOutputDec.append(MyDir);
    DebugOutputDec.append("_decompression_debug.ivf");

    string ReleaseOutputDec = CurTestDirStr;
    ReleaseOutputDec.append(slashCharStr());
    ReleaseOutputDec.append(MyDir);
    ReleaseOutputDec.append("_decompression_release.ivf");

    string ParFileDebug = CurTestDirStr;
    ParFileDebug.append(slashCharStr());
    ParFileDebug.append(MyDir);
    ParFileDebug.append("_parameter_file_debug.txt");

    string ParFileRelease = CurTestDirStr;
    ParFileRelease.append(slashCharStr());
    ParFileRelease.append(MyDir);
    ParFileRelease.append("_parameter_file_release.txt");

    string ProgramDebugEnc = ExeCharDebugReleaseString;
    string ProgramReleaseEnc = ExeCharDebugReleaseString;
    string ProgramDebugDec = ExeCharDebugReleaseString;
    string ProgramReleaseDec = ExeCharDebugReleaseString;

#if defined(_WIN32)
    {
        ProgramDebugEnc.insert(0, "\"\"");
        ProgramDebugEnc.append(ExeInputRelease);
        ProgramDebugEnc.append("\" compress \"");
        ProgramDebugEnc.append(input);
        ProgramDebugEnc.append("\" \"");
        ProgramDebugEnc.append(DebugOutputEnc);
        ProgramDebugEnc.append("\" 8");
        ProgramDebugEnc.append(" \"");
        ProgramDebugEnc.append(ParFileDebug);
        ProgramDebugEnc.append("\"");
        ProgramDebugEnc.append(" 0");
        ProgramDebugEnc.append("\"");

        ProgramDebugDec.insert(0, "\"\"");
        ProgramDebugDec.append(ExeInputRelease);
        ProgramDebugDec.append("\" decompress \"");
        ProgramDebugDec.append(DebugOutputEnc);
        ProgramDebugDec.append("\" \"");
        ProgramDebugDec.append(DebugOutputDec);
        ProgramDebugDec.append("\"\" ");

        ProgramReleaseEnc.insert(0, "\"\"");
        ProgramReleaseEnc.append(ExeInputDebug);
        ProgramReleaseEnc.append("\" compress \"");
        ProgramReleaseEnc.append(input);
        ProgramReleaseEnc.append("\" \"");
        ProgramReleaseEnc.append(ReleaseOutputEnc);
        ProgramReleaseEnc.append("\" 8");
        ProgramReleaseEnc.append(" \"");
        ProgramReleaseEnc.append(ParFileRelease);
        ProgramReleaseEnc.append("\"");
        ProgramReleaseEnc.append(" 0");
        ProgramReleaseEnc.append("\"");

        ProgramReleaseDec.insert(0, "\"\"");
        ProgramReleaseDec.append(ExeInputDebug);
        ProgramReleaseDec.append("\" decompress \"");
        ProgramReleaseDec.append(ReleaseOutputEnc);
        ProgramReleaseDec.append("\" \"");
        ProgramReleaseDec.append(ReleaseOutputDec);
        ProgramReleaseDec.append("\"\" ");
    }
#else
    {
        ProgramDebugEnc.insert(0, "\'");
        ProgramDebugEnc.append(ExeInputRelease);
        ProgramDebugEnc.append("\' compress \'");
        ProgramDebugEnc.append(input);
        ProgramDebugEnc.append("\' \'");
        ProgramDebugEnc.append(DebugOutputEnc);
        ProgramDebugEnc.append("\' 8");
        ProgramDebugEnc.append(" \'");
        ProgramDebugEnc.append(ParFileDebug);
        ProgramDebugEnc.append("\'");
        ProgramDebugEnc.append(" 0");

        ProgramDebugDec.insert(0, "\'");
        ProgramDebugDec.append(ExeInputRelease);
        ProgramDebugDec.append("\' decompress \'");
        ProgramDebugDec.append(DebugOutputEnc);
        ProgramDebugDec.append("\' \'");
        ProgramDebugDec.append(DebugOutputDec);
        ProgramDebugDec.append("\' ");

        ProgramReleaseEnc.insert(0, "\'");
        ProgramReleaseEnc.append(ExeInputDebug);
        ProgramReleaseEnc.append("\' compress \'");
        ProgramReleaseEnc.append(input);
        ProgramReleaseEnc.append("\' \'");
        ProgramReleaseEnc.append(ReleaseOutputEnc);
        ProgramReleaseEnc.append("\' 8");
        ProgramReleaseEnc.append(" \'");
        ProgramReleaseEnc.append(ParFileRelease);
        ProgramReleaseEnc.append("\'");
        ProgramReleaseEnc.append(" 0");

        ProgramReleaseDec.insert(0, "\'");
        ProgramReleaseDec.append(ExeInputDebug);
        ProgramReleaseDec.append("\' decompress \'");
        ProgramReleaseDec.append(ReleaseOutputEnc);
        ProgramReleaseDec.append("\' \'");
        ProgramReleaseDec.append(ReleaseOutputDec);
        ProgramReleaseDec.append("\'");
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
        tprintf(PRINT_STD, "Cannot open out put file2.\n");
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

    vpxt_cap_string_print(PRINT_BTH, "%s\n", MyDir);

    fclose(fp);

    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file3.\n");
        exit(1);
    }

    fprintf(stderr, " ");

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
    if (!vpxt_file_exists_check(argv[6]))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", argv[6]);

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
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

    opt.target_bandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        vpxt_output_settings(ParFileDebug.c_str(), opt);
        vpxt_run_exe(ProgramDebugEnc);
        vpxt_run_exe(ProgramDebugDec);
        vpxt_output_settings(ParFileRelease.c_str(), opt);

        fclose(fp);

        if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put FileIndexOutputChar1.\n");
            exit(1);
        }

        fprintf(stderr, " ");
        vpxt_run_exe(ProgramReleaseEnc);
        vpxt_run_exe(ProgramReleaseDec);
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n\nComparing Compression Files: ");
    int lngRCEnc = vpxt_compare_ivf(ReleaseOutputEnc.c_str(), DebugOutputEnc.c_str());
    int EncMatch = vpxt_print_compare_ivf_results(lngRCEnc);

    tprintf(PRINT_BTH, "Comparing Decompression Files: ");
    int lngRCDec = vpxt_compare_ivf(ReleaseOutputDec.c_str(), DebugOutputDec.c_str());
    int DecMatch = vpxt_print_compare_ivf_results(lngRCDec);

    int fail = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (EncMatch == 1)
    {
        vpxt_formated_print(RESPRT, "Debug Compression identical to Release Compression - Passed\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Debug Compression not identical to Release Compression - Failed");
        fail = 1;
    }

    if (DecMatch == 1)
    {
        vpxt_formated_print(RESPRT, "Debug Decompression identical to Release Decompression - Passed\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Debug Decmpression not identical to Release Decompression - Failed");
        fail = 1;
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
    return 6;
}