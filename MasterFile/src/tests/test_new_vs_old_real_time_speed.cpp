#include "vpxt_test_declarations.h"

int test_new_vs_old_real_time_speed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  NewVsOldRealTimeSpeed \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n"
            "    <Exe File To Compare>\n"
            "    <Parameter Version-1=1.0.4|2=2.0.0>\n"
            "    <Optional Settings File>\n"
            "\n"
        );
        return 0;
    }

    char *input = argv[2];
    int BitRate = atoi(argv[3]);
    int ParFileNum = atoi(argv[5]);

    char ExeInput[255];
    snprintf(ExeInput, 255, "%s", argv[4]);



    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";

    string ExeString = "";

    char WorkingDir3[255] = "";
    char *MyDir = "NewVsOldRealTimeSpeed";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string outputVP8New = WorkingDirString;
    string outputVP8Old = WorkingDirString;
    string outputVP8Old2 = WorkingDirString;
    string ParFile = WorkingDirString;
    string Program = ExeString;

    outputVP8New.append(slashCharStr());
    outputVP8New.append("outputVP8New.ivf");
    outputVP8Old.append(slashCharStr());
    outputVP8Old.append("outputVP8Old.ivf");
    outputVP8Old2.append(slashCharStr());
    outputVP8Old2.append("outputVP8Old.ivf");
    ParFile.append(slashCharStr());
    ParFile.append("ParFile.txt");
    Program.append(ExeInput);

#if defined(_WIN32)
    {
        Program.insert(0, "\"");
        Program.insert(0, "\"");
        Program.append("\" \"");
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
        Program.append("\' \'");
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
        print_header_full_test(argc, argv, WorkingDirString);
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


    unsigned int cpu_tick1 = 0;
    unsigned int cpu_tick2 = 0;
    unsigned int Time3 = 0;

    tprintf("New Vs Old Real Time Speed Test");

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
    if (!vpxt_file_exists_check(argv[4]))
    {
        tprintf("\nInput executable %s does not exist\n", argv[4]);
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.auto_key = 1;
    //opt.cpu_used = -1;
    opt.cpu_used = -4;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        cpu_tick1 = vpxt_cpu_tick_return(outputVP8New.c_str(), 0);
        cpu_tick2 = vpxt_cpu_tick_return(outputVP8Old2.c_str(), 0);

        tprintf("\ncpu_tick1: %i\n", cpu_tick1);
        tprintf("\ncpu_tick2: %i\n", cpu_tick2);
    }
    else
    {

        opt.Mode = MODE_REALTIME;
        unsigned int Time = vpxt_time_compress_ivf_to_ivf(input, outputVP8New.c_str(), speed, BitRate, opt, "VP8", 0, 0, cpu_tick1);

        if (Time == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        vpxt_output_compatable_settings(ParFile.c_str(), opt, ParFileNum);
        printf("\nCompressing Old File\n");

        fclose(fp);

        if ((fp = freopen(TextfileString.c_str(), "a+", stderr)) == NULL)
        {
            printf("Cannot open out put file: %s\n", TextfileString.c_str());
            exit(1);
        }

        fprintf(stderr, " ");

        vpxt_run_exe(Program);
        unsigned int Time2 = vpxt_time_return(outputVP8Old2.c_str(), 0);
        cpu_tick2 = vpxt_cpu_tick_return(outputVP8Old2.c_str(), 0);

        tprintf("\n\nFile completed: Time in Microseconds: %i", Time2);
        tprintf("\n Total CPU Ticks: %u\n", cpu_tick2);
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

    tprintf("\n\n\nResults:\n\n");

    if (cpu_tick1 > cpu_tick2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Old: %i is Faster than New: %i - Failed", cpu_tick2, cpu_tick1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (cpu_tick1 < cpu_tick2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "New: %i is Faster than Old: %i - Passed", cpu_tick1, cpu_tick2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Files Took the same amount of time - Indeterminate");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\nIndeterminate\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}