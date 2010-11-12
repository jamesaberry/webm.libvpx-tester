#include "vpxt_test_declarations.h"

int test_debug_matches_release(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    // So long as Debug.exe <INPUT FILE> <OUTPUT FILE> <PARAMETER FILE>
    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  DebugMatchesRelease \n\n"
            "    <inputfile>\n"
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
            "\n");
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    char *ExeInputDebug = argv[5];
    char ExeInputRelease[255];

    snprintf(ExeInputRelease, 255, "%s", argv[6]);

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "DebugMatchesRelease";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    char ExeCharDebugRelease[1024];
    vpxt_folder_name(argv[0], ExeCharDebugRelease);
    string ExeCharDebugReleaseString = ExeCharDebugRelease;

    string DebugOutput = WorkingDirString;
    string ReleaseOutput = WorkingDirString;
    string WorkingDir6 = WorkingDirString;
    string ParFileDebug = WorkingDirString;
    string ParFileRelease = WorkingDirString;
    string ProgramDebug = ExeCharDebugReleaseString;
    string ProgramRelease = ExeCharDebugReleaseString;

    string DebugExeLoc = ExeCharDebugReleaseString;
    DebugExeLoc.append(ExeInputDebug);
    string ReleaseExeLoc = ExeCharDebugReleaseString;
    ReleaseExeLoc.append(ExeInputRelease);

    DebugOutput.append(slashCharStr());
    DebugOutput.append("output_Debug.ivf");
    ReleaseOutput.append(slashCharStr());
    ReleaseOutput.append("output_Release.ivf");

    ParFileDebug.append(slashCharStr());
    ParFileDebug.append("ParFileDebug.txt");
    ParFileRelease.append(slashCharStr());
    ParFileRelease.append("ParFileRelease.txt");

#if defined(_WIN32)
    {
        ProgramDebug.append(ExeInputRelease);
        ProgramDebug.append("\" \"");
        ProgramDebug.append(input);
        ProgramDebug.append("\" \"");
        ProgramDebug.append(DebugOutput);
        ProgramDebug.append("\" 8");
        ProgramDebug.append(" \"");
        ProgramDebug.append(ParFileDebug);
        ProgramDebug.append("\"");
        ProgramDebug.append(" 0");
        ProgramDebug.append("\"");
        ProgramRelease.append(ExeInputDebug);
        ProgramRelease.append("\" \"");
        ProgramRelease.append(input);
        ProgramRelease.append("\" \"");
        ProgramRelease.append(ReleaseOutput);
        ProgramRelease.append("\" 8");
        ProgramRelease.append(" \"");
        ProgramRelease.append(ParFileRelease);
        ProgramRelease.append("\"");
        ProgramRelease.append(" 0");
        ProgramRelease.append("\"");

        ProgramDebug.insert(0, "\"\"");
        ProgramRelease.insert(0, "\"\"");
    }
#else
    {
        ProgramDebug.append(ExeInputRelease);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(input);
        ProgramDebug.append("\' \'");
        ProgramDebug.append(DebugOutput);
        ProgramDebug.append("\' 8");
        ProgramDebug.append(" \'");
        ProgramDebug.append(ParFileDebug);
        ProgramDebug.append("\'");
        ProgramDebug.append(" 0");
        ProgramRelease.append(ExeInputDebug);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(input);
        ProgramRelease.append("\' \'");
        ProgramRelease.append(ReleaseOutput);
        ProgramRelease.append("\' 8");
        ProgramRelease.append(" \'");
        ProgramRelease.append(ParFileRelease);
        ProgramRelease.append("\'");
        ProgramRelease.append(" 0");

        ProgramDebug.insert(0, "\'");
        ProgramRelease.insert(0, "\'");
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

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file2.\n");
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

    fclose(fp);

    if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
    {
        printf("Cannot open out put file3.\n");
        exit(1);
    }

    tprintf("Debug Matches Release Test");

    int speed = 0;

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
    if (!vpxt_file_exists_check(argv[6]))
    {
        tprintf("\nInput executable %s does not exist\n", argv[6]);
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
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

    opt.target_bandwidth = BitRate;

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

            vpxt_output_settings(ParFileDebug.c_str(), opt);
            vpxt_run_exe(ProgramDebug);
            vpxt_output_settings(ParFileRelease.c_str(), opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file4.\n");
                exit(1);
            }

            fprintf(stderr, " ");
            vpxt_run_exe(ProgramRelease);
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;

            vpxt_output_settings(ParFileDebug.c_str(), opt);
            vpxt_run_exe(ProgramDebug);
            vpxt_output_settings(ParFileRelease.c_str(), opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file5.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            vpxt_run_exe(ProgramRelease);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            vpxt_output_settings(ParFileDebug.c_str(), opt);
            vpxt_run_exe(ProgramDebug);
            vpxt_output_settings(ParFileRelease.c_str(), opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file6.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            vpxt_run_exe(ProgramRelease);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;

            vpxt_output_settings(ParFileDebug.c_str(), opt);
            vpxt_run_exe(ProgramDebug);
            vpxt_output_settings(ParFileRelease.c_str(), opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file9.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            vpxt_run_exe(ProgramRelease);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            vpxt_output_settings(ParFileDebug.c_str(), opt);
            vpxt_run_exe(ProgramDebug);
            vpxt_output_settings(ParFileRelease.c_str(), opt);

            fclose(fp);

            if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
            {
                printf("Cannot open out put file11.\n");
                exit(1);
            }

            fprintf(stderr, " ");

            vpxt_run_exe(ProgramRelease);

        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    tprintf("\n\nComparing Files: ");

    int lngRC = vpxt_compare_ivf(ReleaseOutput.c_str(), DebugOutput.c_str());

    int fail = 0;

    if (lngRC >= 0)
    {
        tprintf("Files differ at frame: %i\n", lngRC);
        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression not identical to Release Compression - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (lngRC == -1)
    {
        tprintf("Files are identical\n");
        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression identical to Release Compression - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (lngRC == -2)
    {
        tprintf("File 2 ends before File 1\n");
        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression not identical to Release Compression - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (lngRC == -3)
    {
        tprintf("File 1 ends before File 2\n");
        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Debug Compression not identical to Release Compression - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf("\nPassed\n");
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
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