#include "vpxt_test_declarations.h"

int test_new_vs_old_enc_cpu_tick(int argc, char *argv[], const string &WorkingDir, string FilesAr[], int TestType)
{

    char *MyDir = "test_new_vs_old_enc_cpu_tick";

    if (!(argc == 8 || argc == 7))
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
    unsigned int cpu_tick1 = 0;
    unsigned int cpu_tick2 = 0;
    unsigned int Time3 = 0;

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    string ExeString = "";
    char MainTestDirChar[255] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    string outputVP8New = CurTestDirStr;
    outputVP8New.append(slashCharStr());
    outputVP8New.append(MyDir);
    outputVP8New.append("_compression_new.ivf");

    string outputVP8Old = CurTestDirStr;
    outputVP8Old.append(slashCharStr());
    outputVP8Old.append(MyDir);
    outputVP8Old.append("_compression_old.ivf");

    string outputVP8Old2 = CurTestDirStr;
    outputVP8Old2.append(slashCharStr());
    outputVP8Old2.append(MyDir);
    outputVP8Old2.append("_compression_old.ivf");

    string ParFile = CurTestDirStr;
    ParFile.append(slashCharStr());
    ParFile.append(MyDir);
    ParFile.append("_parameter_file.txt");

    string Program = ExeString;
    Program.append(ExeInput);

#if defined(_WIN32)
    {
        Program.insert(0, "\"");
        Program.insert(0, "\"");
        Program.append("\" compress \"");
        Program.append(input);
        Program.append("\" \"");
        Program.append(outputVP8Old);
        Program.append("\" 8");
        Program.append(" \"");
        Program.append(ParFile);
        Program.append("\"");
        Program.append(" 2");
        Program.append("\"");
    }
#else
    {
        Program.insert(0, "\'./");
        Program.append("\' compress \'");
        Program.append(input);
        Program.append("\' \'");
        Program.append(outputVP8Old);
        Program.append("\' 8");
        Program.append(" \'");
        Program.append(ParFile);
        Program.append("\'");
        Program.append(" 2");
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
    opt.cpu_used = -4;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        cpu_tick1 = vpxt_cpu_tick_return(outputVP8New.c_str(), 0);
        cpu_tick2 = vpxt_cpu_tick_return(outputVP8Old2.c_str(), 0);

        tprintf(PRINT_BTH, "\ncpu_tick1: %i\n", cpu_tick1);
        tprintf(PRINT_BTH, "\ncpu_tick2: %i\n", cpu_tick2);
    }
    else
    {

        opt.Mode = Mode;
        unsigned int Time = vpxt_time_compress_ivf_to_ivf(input, outputVP8New.c_str(), speed, BitRate, opt, "VP8", 0, 0, cpu_tick1, 0, 3, 3);

        if (Time == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);
        tprintf(PRINT_STD, "\nCompressing Old File\n");

        fclose(fp);

        if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
            exit(1);
        }

        fprintf(stderr, " ");

        vpxt_run_exe(Program);
        unsigned int Time2 = vpxt_time_return(outputVP8Old2.c_str(), 0);
        cpu_tick2 = vpxt_cpu_tick_return(outputVP8Old2.c_str(), 0);

        tprintf(PRINT_BTH, "\n\nFile completed: Time in Microseconds: %i", Time2);
        tprintf(PRINT_BTH, "\n Total CPU Ticks: %u\n", cpu_tick2);
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n\n\nResults:\n\n");

    if (cpu_tick1 > cpu_tick2)
    {
        vpxt_formated_print(RESPRT, "Old: %i is Faster than New: %i - Failed", cpu_tick2, cpu_tick1);

        tprintf(PRINT_BTH, "\n\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    if (cpu_tick1 < cpu_tick2)
    {
        vpxt_formated_print(RESPRT, "New: %i is Faster than Old: %i - Passed", cpu_tick1, cpu_tick2);

        tprintf(PRINT_BTH, "\n\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Files Took the same amount of time - Indeterminate");

        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}