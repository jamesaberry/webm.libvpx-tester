#include "vpxt_test_declarations.h"

int test_multithreaded_dec(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "TokenPart";
    char *MyDir = "test_multithreaded_dec";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    unsigned int CoreCount = atoi(argv[5]);
    std::string EncForm = argv[6];
    std::string DecForm = argv[7];

    int MultiThreaded = 0;
    unsigned int Time0Dec;
    unsigned int TimeNDec;

    if (Mode != 0 && Mode != 1)
        return vpxt_test_help(argv[1], 0);

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string MultiThreadCompFile = CurTestDirStr;
    MultiThreadCompFile.append(slashCharStr());
    MultiThreadCompFile.append(MyDir);
    MultiThreadCompFile.append("_compression");
    vpxt_enc_format_append(MultiThreadCompFile, EncForm);

    std::string MultiThreadedOffOutFile_DecThread0 = CurTestDirStr;
    MultiThreadedOffOutFile_DecThread0.append(slashCharStr());
    MultiThreadedOffOutFile_DecThread0.append(MyDir);
    MultiThreadedOffOutFile_DecThread0.append("_dec_thread_0");
    vpxt_enc_format_append(MultiThreadedOffOutFile_DecThread0, DecForm);

    std::string MultiThreadedOffOutFile_DecThreadN = CurTestDirStr;
    MultiThreadedOffOutFile_DecThreadN.append(slashCharStr());
    MultiThreadedOffOutFile_DecThreadN.append(MyDir);
    MultiThreadedOffOutFile_DecThreadN.append("_dec_thread_");
    MultiThreadedOffOutFile_DecThreadN.append(argv[5]);
    vpxt_enc_format_append(MultiThreadedOffOutFile_DecThreadN, DecForm);

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

    if (!(CoreCount > 1))
    {
        tprintf(PRINT_STD, "\nMultiple Cores not used Test aborted: %i\n", CoreCount);
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
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n", argv[argc-1]);

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

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        Time0Dec = vpxt_time_return(MultiThreadedOffOutFile_DecThread0.c_str(), 0);
        TimeNDec = vpxt_time_return(MultiThreadedOffOutFile_DecThreadN.c_str(), 0);
    }
    else
    {
        if (Mode == 0)
        {
            if (CoreCount < 4)
                opt.token_partitions = CoreCount;
            else
                opt.token_partitions = 3;

            opt.Mode = MODE_REALTIME;
            opt.cpu_used = -1;
            unsigned int cpu_tick1 = 0;

            if (vpxt_compress(input.c_str(), MultiThreadCompFile.c_str(), MultiThreaded, BitRate, opt, CompressString, opt.token_partitions, 0, EncForm) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }
        }

        if (Mode == 1)
        {
            if (CoreCount < 4)
                opt.token_partitions = CoreCount;
            else
                opt.token_partitions = 3;

            opt.Mode = MODE_GOODQUALITY;
            unsigned int cpu_tick1 = 0;

            if (vpxt_compress(input.c_str(), MultiThreadCompFile.c_str(), MultiThreaded, BitRate, opt, CompressString, opt.token_partitions, 0, EncForm) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }
        }
    }

    unsigned int DecCPUTick1 = 0;
    tprintf(PRINT_BTH, "\n\nDec Threads: 0");
    Time0Dec = vpxt_decompress_time_and_output(MultiThreadCompFile.c_str(), MultiThreadedOffOutFile_DecThread0.c_str(), DecCPUTick1, DecForm, 0);
    tprintf(PRINT_BTH, "\nDec Time in ms: %i\n", Time0Dec);
    unsigned int DecCPUTick2 = 0;
    tprintf(PRINT_BTH, "\n\nDec Threads: %i", CoreCount);
    TimeNDec = vpxt_decompress_time_and_output(MultiThreadCompFile.c_str(), MultiThreadedOffOutFile_DecThreadN.c_str(), DecCPUTick2, DecForm, CoreCount);
    tprintf(PRINT_BTH, "\nDec Time in ms: %i\n", TimeNDec);

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n\nComparing Decompression Files: ");
    int CompareDecOutput = vpxt_compare_dec(MultiThreadedOffOutFile_DecThread0.c_str(), MultiThreadedOffOutFile_DecThreadN.c_str());
    int CompareDec = vpxt_print_compare_ivf_results(CompareDecOutput, 1);

    char Time0FileNameDec[255] = "";
    char TimeNFileNameDec[255] = "";

    vpxt_file_name(MultiThreadedOffOutFile_DecThread0.c_str(), Time0FileNameDec, 0);
    vpxt_file_name(MultiThreadedOffOutFile_DecThreadN.c_str(), TimeNFileNameDec, 0);

    int fail = 0;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (Time0Dec == 0 || TimeNDec == 0)
    {
        if (Time0Dec == 0)
        {
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed", Time0FileNameDec, Time0Dec);
            tprintf(PRINT_BTH, "\n");
        }

        if (TimeNDec == 0)
        {
            vpxt_formated_print(RESPRT, "%s time: %u = 0 - Failed", TimeNFileNameDec, TimeNDec);
        }

        fail = 1;
    }

    if (TimeNDec < Time0Dec)
    {
        vpxt_formated_print(RESPRT, "%s time: %u < %s time: %u - Passed", TimeNFileNameDec, TimeNDec, Time0FileNameDec, Time0Dec);
        tprintf(PRINT_BTH, "\n");
    }

    if (Time0Dec == TimeNDec)
    {
        vpxt_formated_print(RESPRT, "%s time: %u == %s time: %u - Indeterminate", Time0FileNameDec, Time0Dec, TimeNFileNameDec, TimeNDec);
        tprintf(PRINT_BTH, "\n");

        if (fail != 1)
            fail = 2;
    }

    if (TimeNDec > Time0Dec)
    {
        vpxt_formated_print(RESPRT, "%s time: %u > %s time: %u - Failed", TimeNFileNameDec, TimeNDec, Time0FileNameDec, Time0Dec);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    //compare decodes
    if (CompareDec == 1)
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Passed", Time0FileNameDec, TimeNFileNameDec);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Failed", Time0FileNameDec, TimeNFileNameDec);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }


    //final evaluate
    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(3, MultiThreadCompFile.c_str(), MultiThreadedOffOutFile_DecThread0.c_str(), MultiThreadedOffOutFile_DecThreadN.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(3, MultiThreadCompFile.c_str(), MultiThreadedOffOutFile_DecThread0.c_str(), MultiThreadedOffOutFile_DecThreadN.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    if (fail == 3)
    {
        tprintf(PRINT_BTH, "\nIndeterminate\n");

        if (DeleteIVF)
            vpxt_delete_files(3, MultiThreadCompFile.c_str(), MultiThreadedOffOutFile_DecThread0.c_str(), MultiThreadedOffOutFile_DecThreadN.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}