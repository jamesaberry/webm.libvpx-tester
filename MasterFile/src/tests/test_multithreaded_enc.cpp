#include "vpxt_test_declarations.h"

int test_multithreaded_enc(int argc,
                           const char *const *argv,
                           const std::string &WorkingDir,
                           std::string FilesAr[],
                           int TestType,
                           int DeleteIVF)
{
    char *CompressString = "Multithreaded";
    char *MyDir = "test_multithreaded_enc";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    unsigned int CoreCount = atoi(argv[5]);
    std::string EncForm = argv[6];

    int MultiThreaded = 0;
    unsigned int Time1;
    unsigned int Time2;

    if (Mode != 0 && Mode != 1)
        return vpxt_test_help(argv[1], 0);

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir,
        CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar,
        FilesAr) == 11)
        return 11;

    std::string MultiThreadedOnOutFile = CurTestDirStr;
    MultiThreadedOnOutFile.append(slashCharStr());
    MultiThreadedOnOutFile.append(MyDir);
    MultiThreadedOnOutFile.append("_compression_1");
    vpxt_enc_format_append(MultiThreadedOnOutFile, EncForm);

    std::string MultiThreadedOffOutFile = CurTestDirStr;
    MultiThreadedOffOutFile.append(slashCharStr());
    MultiThreadedOffOutFile.append(MyDir);
    MultiThreadedOffOutFile.append("_compression_0");
    vpxt_enc_format_append(MultiThreadedOffOutFile, EncForm);

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
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            TextfileString.c_str());
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

    if (!(CoreCount > 1))
    {
        tprintf(PRINT_STD, "\nMultiple Cores not used Test aborted: %i\n",
            CoreCount);
        fclose(fp);
        return 0;
    }

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (inputCheck == 2)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n",
                argv[argc-1]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.end_usage = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (TestType == TEST_ONLY)
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
            Time1 = vpxt_time_compress(input.c_str(),
                MultiThreadedOnOutFile.c_str(), MultiThreaded, BitRate, opt,
                CompressString, opt.multi_threaded, 0, cpu_tick1, EncForm);

            if (Time1 == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar,
                    TestType);
                return 2;
            }

            opt.Mode = MODE_REALTIME;
            opt.multi_threaded = 0;
            unsigned int cpu_tick2 = 0;
            Time2 = vpxt_time_compress(input.c_str(),
                MultiThreadedOffOutFile.c_str(), MultiThreaded, BitRate, opt,
                CompressString, opt.multi_threaded, 0, cpu_tick2, EncForm);

            if (Time2 == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar,
                    TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            opt.multi_threaded = CoreCount;
            unsigned int cpu_tick1 = 0;
            Time1 = vpxt_time_compress(input.c_str(),
                MultiThreadedOnOutFile.c_str(), MultiThreaded, BitRate, opt,
                CompressString, opt.multi_threaded, 0, cpu_tick1, EncForm);

            if (Time1 == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar,
                    TestType);
                return 2;
            }

            opt.Mode = MODE_GOODQUALITY;
            opt.multi_threaded = 0;
            unsigned int cpu_tick2 = 0;
            Time2 = vpxt_time_compress(input.c_str(),
                MultiThreadedOffOutFile.c_str(), MultiThreaded, BitRate, opt,
                CompressString, opt.multi_threaded, 0, cpu_tick2, EncForm);

            if (Time2 == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar,
                    TestType);
                return 2;
            }
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

    char Time1FileName[255] = "";
    char Time2FileName[255] = "";

    vpxt_file_name(MultiThreadedOnOutFile.c_str(), Time1FileName, 0);
    vpxt_file_name(MultiThreadedOffOutFile.c_str(), Time2FileName, 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (Time1 == 0 || Time2 == 0)
    {
        if (Time1 == 0)
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed",
            Time1FileName, Time1);

        if (Time2 == 0)
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed",
            Time2FileName, Time2);

        tprintf(PRINT_BTH, "\n\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, MultiThreadedOnOutFile.c_str(),
            MultiThreadedOffOutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;

    }

    if (Time1 < Time2)
    {
        vpxt_formated_print(RESPRT, "%s time: %u < %s time2: %u - Passed",
            Time1FileName, Time1, Time2FileName, Time2);

        tprintf(PRINT_BTH, "\n\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, MultiThreadedOnOutFile.c_str(),
            MultiThreadedOffOutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    if (Time1 == Time2)
    {
        vpxt_formated_print(RESPRT, "%s time: %u == %s time: %u - Indeterminate"
            , Time1FileName, Time1, Time2FileName, Time2);

        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        if (DeleteIVF)
            vpxt_delete_files(2, MultiThreadedOnOutFile.c_str(),
            MultiThreadedOffOutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    if (Time1 > Time2)
    {
        vpxt_formated_print(RESPRT, "%s time: %u > %s time: %u - Failed",
            Time1FileName, Time1, Time2FileName, Time2);

        tprintf(PRINT_BTH, "\n\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, MultiThreadedOnOutFile.c_str(),
            MultiThreadedOffOutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}