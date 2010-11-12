#include "vpxt_test_declarations.h"

int test_multithreaded(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "MultiThreaded";

    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  MultiThreadedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Number of Cores to Use>\n"
            "     <Optional Settings File>\n"
        );
        return 0;
    }

    char *input = argv[2];

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "MultiThreadedTest";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string MultiThreadedOnOutFile = WorkingDirString;
    string MultiThreadedOffOutFile = WorkingDirString;

    MultiThreadedOnOutFile.append(slashCharStr());
    MultiThreadedOnOutFile.append("MultiThreadedOnOutput.ivf");
    MultiThreadedOffOutFile.append(slashCharStr());
    MultiThreadedOffOutFile.append("MultiThreadedOffOutput.ivf");

    //char MultiThreadedOnOutFile[255];
    //char MultiThreadedOffOutFile[255];

    //snprintf(MultiThreaded14OutFile, 255, "%s", WorkingDir4.c_str());
    //snprintf(MultiThreaded00OutFile, 255, "%s", WorkingDir5.c_str());

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

    int MultiThreaded = 0;
    int BitRate = atoi(argv[4]);
    unsigned int CoreCount = atoi(argv[5]);//VPX_GetProcCoreCount();

    unsigned int Time1;
    unsigned int Time2;

    if (!(CoreCount > 1))
    {
        printf("\nMultiple Cores not used Test aborted: %i\n", CoreCount);
        fclose(fp);
        return 0;
    }

    int Mode = atoi(argv[3]);

    if (Mode != 0 && Mode != 1)
    {
        printf(
            "  MultiThreadedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Number of Cores to Use>\n"
            "     <Optional Settings File>\n"
        );
        fclose(fp);
        return 0;
    }

    tprintf("MultiThreaded Test");

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

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.end_usage = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        Time1 = vpxt_time_return(MultiThreadedOnOutFile.c_str(), 0);
        Time2 = vpxt_time_return(MultiThreadedOffOutFile.c_str(), 0);
    }
    else
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            opt.multi_threaded = CoreCount;
            opt.cpu_used = -1;
            unsigned int cpu_tick1 = 0;
            Time1 = vpxt_time_compress_ivf_to_ivf(input, MultiThreadedOnOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, opt.multi_threaded, 0, cpu_tick1);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.multi_threaded = 0;
            unsigned int cpu_tick2 = 0;
            Time2 = vpxt_time_compress_ivf_to_ivf(input, MultiThreadedOffOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, opt.multi_threaded, 0, cpu_tick2);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.multi_threaded = CoreCount;
            unsigned int cpu_tick1 = 0;
            Time1 = vpxt_time_compress_ivf_to_ivf(input, MultiThreadedOnOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, opt.multi_threaded, 0, cpu_tick1);

            if (Time1 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.multi_threaded = 0;
            unsigned int cpu_tick2 = 0;
            Time2 = vpxt_time_compress_ivf_to_ivf(input, MultiThreadedOffOutFile.c_str(), MultiThreaded, BitRate, opt, CompressString, opt.multi_threaded, 0, cpu_tick2);

            if (Time2 == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
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

    char Time1FileName[255] = "";
    char Time2FileName[255] = "";

    vpxt_file_name(MultiThreadedOnOutFile.c_str(), Time1FileName, 0);
    vpxt_file_name(MultiThreadedOffOutFile.c_str(), Time2FileName, 0);

    tprintf("\n\nResults:\n\n");

    if (Time1 < Time2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s time: %u < %s time2: %u - Passed", Time1FileName, Time1, Time2FileName, Time2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (Time1 == Time2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s time: %u == %s time: %u - Indeterminate", Time1FileName, Time1, Time2FileName, Time2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nIndeterminate\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    if (Time1 > Time2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s time: %u > %s time: %u - Failed", Time1FileName, Time1, Time2FileName, Time2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

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