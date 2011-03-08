#include "vpxt_test_declarations.h"

int test_change_cpu_enc(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Arnr Maxframes:";
    char *MyDir = "test_change_cpu_enc";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int ArnrMaxframes = atoi(argv[5]);
    std::string EncForm = argv[6];
    std::string DecForm = argv[7];

    int speed = 0;
    int Fail = 0;
    int ModesRun = 0;

    unsigned int cpu_tick1 = 0;
    unsigned int cpu_tick2 = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string ChangedCPUDec0OutFile = CurTestDirStr;
    ChangedCPUDec0OutFile.append(slashCharStr());
    ChangedCPUDec0OutFile.append("test_change_cpu_enc_compression_none");
    vpxt_enc_format_append(ChangedCPUDec0OutFile, EncForm);

    std::string ChangedCPUDecNOutBase = CurTestDirStr;
    ChangedCPUDecNOutBase.append(slashCharStr());
    ChangedCPUDecNOutBase.append(MyDir);
    ChangedCPUDecNOutBase.append("_compression_");

    std::string OutputStr0 = ChangedCPUDecNOutBase;
    OutputStr0.append("none");
    vpxt_enc_format_append(OutputStr0, EncForm);
    std::string OutputStr1 = ChangedCPUDecNOutBase;
    OutputStr1.append("mmx");
    vpxt_enc_format_append(OutputStr1, EncForm);
    std::string OutputStr2 = ChangedCPUDecNOutBase;
    OutputStr2.append("sse");
    vpxt_enc_format_append(OutputStr2, EncForm);
    std::string OutputStr3 = ChangedCPUDecNOutBase;
    OutputStr3.append("sse2");
    vpxt_enc_format_append(OutputStr3, EncForm);
    std::string OutputStr4 = ChangedCPUDecNOutBase;
    OutputStr4.append("sse3");
    vpxt_enc_format_append(OutputStr4, EncForm);
    std::string OutputStr5 = ChangedCPUDecNOutBase;
    OutputStr5.append("ssse3");
    vpxt_enc_format_append(OutputStr5, EncForm);
    std::string OutputStr6 = ChangedCPUDecNOutBase;
    OutputStr6.append("sse4_1");
    vpxt_enc_format_append(OutputStr6, EncForm);

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
    //opt.Version = VersionNum;
    int CompressInt = ArnrMaxframes;
    opt.Mode = Mode;
    int counter = 0;

    putenv("ON2_SIMD_CAPS=0");

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        std::vector<std::string> CompressonVector;

        if (vpxt_file_exists_check(OutputStr0))
            CompressonVector.push_back(OutputStr0);

        if (vpxt_file_exists_check(OutputStr1))
            CompressonVector.push_back(OutputStr1);

        if (vpxt_file_exists_check(OutputStr2))
            CompressonVector.push_back(OutputStr2);

        if (vpxt_file_exists_check(OutputStr3))
            CompressonVector.push_back(OutputStr3);

        if (vpxt_file_exists_check(OutputStr4))
            CompressonVector.push_back(OutputStr4);

        if (vpxt_file_exists_check(OutputStr5))
            CompressonVector.push_back(OutputStr5);

        if (vpxt_file_exists_check(OutputStr6))
            CompressonVector.push_back(OutputStr6);

        ModesRun = CompressonVector.size();

        cpu_tick1 = vpxt_cpu_tick_return(CompressonVector[0].c_str(), 0);

        int CurrentFile = 0;

        while (CurrentFile < CompressonVector.size())
        {

            cpu_tick2 = vpxt_cpu_tick_return(CompressonVector[CurrentFile].c_str(), 0);

            if (CurrentFile >= 1)
            {
                tprintf(PRINT_BTH, "\n");

                char CompFileIndexOutputChar[255];
                char CompFile2[255];
                vpxt_file_name(CompressonVector[CurrentFile-1].c_str(), CompFileIndexOutputChar, 0);
                vpxt_file_name(CompressonVector[CurrentFile].c_str(), CompFile2, 0);

                tprintf(PRINT_BTH, "\nComparing %s to %s\n", CompFileIndexOutputChar, CompFile2);

                int lngRC = vpxt_compare_enc(CompressonVector[CurrentFile-1].c_str(), CompressonVector[CurrentFile].c_str());

                if (lngRC >= 0)
                {
                    tprintf(PRINT_BTH, "\n * Fail: Files differ at frame: %i", lngRC);
                    Fail = 1;
                }

                if (lngRC == -1)
                {
                    tprintf(PRINT_BTH, " * Files are identical");
                }

                if (lngRC == -2)
                {
                    tprintf(PRINT_BTH, "\n * Fail: File 2 ends before File 1.\n");
                    Fail = 1;
                }

                if (lngRC == -3)
                {
                    tprintf(PRINT_BTH, "\n * Fail: File 1 ends before File 2.\n");
                    Fail = 1;
                }
            }

            CurrentFile++;
        }

        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        std::vector<std::string> CompressonVector;

        putenv("ON2_SIMD_CAPS=0");
        std::string OutputStr = ChangedCPUDecNOutBase;
        OutputStr.append("none");
        vpxt_dec_format_append(OutputStr, DecForm);

        opt.arnr_max_frames = ArnrMaxframes;

        tprintf(PRINT_BTH, "\n\nDetected CPU capability: NONE");
        unsigned int Time1 = vpxt_time_compress(input.c_str(), OutputStr.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, cpu_tick1, EncForm);
        CompressonVector.push_back(OutputStr);

        if (Time1 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_BTH, "\n");

        int counterMax = 64;
        int FileNumber = 1;
        counter++;

        int Simd_Caps = x86_simd_caps();

        while (counter < counterMax)
        {
            int CPUFound = 0;
            int Has_Check = (counter + 1) / 2;

            if ((Simd_Caps & HAS_MMX)    == Has_Check)
                CPUFound = 1;

            if ((Simd_Caps & HAS_SSE)    == Has_Check)
                CPUFound = 1;

            if ((Simd_Caps & HAS_SSE2)   == Has_Check)
                CPUFound = 1;

            if ((Simd_Caps & HAS_SSE3)   == Has_Check)
                CPUFound = 1;

            if ((Simd_Caps & HAS_SSSE3)  == Has_Check)
                CPUFound = 1;

            if ((Simd_Caps & HAS_SSE4_1) == Has_Check)
                CPUFound = 1;

            if (CPUFound == 1)
            {
                std::string CPUStr = "";
                tprintf(PRINT_BTH, "\nDetected CPU capability: ");

                if ((Simd_Caps & HAS_MMX)    == Has_Check)
                    CPUStr = "mmx";

                if ((Simd_Caps & HAS_SSE)    == Has_Check)
                    CPUStr = "sse";

                if ((Simd_Caps & HAS_SSE2)   == Has_Check)
                    CPUStr = "sse2";

                if ((Simd_Caps & HAS_SSE3)   == Has_Check)
                    CPUStr = "sse3";

                if ((Simd_Caps & HAS_SSSE3)  == Has_Check)
                    CPUStr = "ssse3";

                if ((Simd_Caps & HAS_SSE4_1) == Has_Check)
                    CPUStr = "sse4_1";

                tprintf(PRINT_BTH, "%s", CPUStr.c_str());

                ///////////Updating CPU///////////
                std::string CPUIDSTRING = "ON2_SIMD_CAPS=";
                char CounterChar[10];
                vpxt_itoa_custom(counter, CounterChar, 10);
                CPUIDSTRING.append(CounterChar);

                char CPUChar[255];
                snprintf(CPUChar, 255, CPUIDSTRING.c_str());
                putenv(CPUChar);

                ///////Compresion and Time ///////
                std::string ChangedCPUDecNOutCurrent = ChangedCPUDecNOutBase;
                std::string ChangedCPUDecNOutLast = ChangedCPUDecNOutBase;

                char count[20];
                vpxt_itoa_custom(counter, count, 10);
                ChangedCPUDecNOutCurrent.append(CPUStr.c_str());
                vpxt_dec_format_append(ChangedCPUDecNOutCurrent, DecForm);

                opt.Mode = Mode;
                unsigned int Time2 = vpxt_time_compress(input.c_str(), ChangedCPUDecNOutCurrent.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, cpu_tick2, EncForm);
                CompressonVector.push_back(ChangedCPUDecNOutCurrent);

                if (Time2 == -1)
                {
                    fclose(fp);
                    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                    return 2;
                }

                if (TestType != 2 && counter != 0)
                {
                    char CompFileIndexOutputChar[255];
                    vpxt_file_name(CompressonVector[CompressonVector.size()-1].c_str(), CompFileIndexOutputChar, 0);

                    char CompFile2[255];
                    vpxt_file_name(CompressonVector[CompressonVector.size()-2].c_str(), CompFile2, 0);

                    tprintf(PRINT_BTH, "\nComparing %s to %s\n", CompFileIndexOutputChar, CompFile2);

                    int lngRC = vpxt_compare_enc(CompressonVector[CompressonVector.size()-1].c_str(), CompressonVector[CompressonVector.size()-2].c_str());

                    if (lngRC >= 0)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: Files differ at frame: %i on file number %i\n", lngRC, FileNumber);
                        Fail = 1;
                    }

                    if (lngRC == -1)
                    {
                        tprintf(PRINT_BTH, " * Files are identical\n");
                    }

                    if (lngRC == -2)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: File 2 ends before File 1.\n");
                        Fail = 1;
                    }

                    if (lngRC == -3)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: File 1 ends before File 2.\n");
                        Fail = 1;
                    }
                }
            }

            counter = ((counter + 1) * 2) - 1;
            tprintf(PRINT_BTH, "\n");
            FileNumber++;
        }

        ModesRun = CompressonVector.size();
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        putenv("ON2_SIMD_CAPS=");
        return 10;
    }

    int overallfail = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (Fail != 1)
    {
        vpxt_formated_print(RESPRT, "All Files are identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (Fail == 1)
    {
        vpxt_formated_print(RESPRT, "All Files are not identical - Failed");
        tprintf(PRINT_BTH, "\n");
        overallfail = 1;
    }

    if (ModesRun == 7)
    {
        vpxt_formated_print(RESPRT, "All instruction sets run - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (ModesRun != 7)
    {
        vpxt_formated_print(RESPRT, "Not all instruction sets run - MinPassed");
        tprintf(PRINT_BTH, "\n");

        if (overallfail != 1) overallfail = 2;
    }

    if (cpu_tick1 == cpu_tick2)
    {
        vpxt_formated_print(RESPRT, "cpu_tick1: %u == cpu_tick2: %u - Failed", cpu_tick1, cpu_tick2);
        tprintf(PRINT_BTH, "\n");
        overallfail = 1;
    }

    if (cpu_tick1 != cpu_tick2)
    {
        vpxt_formated_print(RESPRT, "cpu_tick1: %u != cpu_tick2: %u - Passed", cpu_tick1, cpu_tick2);
        tprintf(PRINT_BTH, "\n");
    }

    if (overallfail == 2)
    {
        tprintf(PRINT_BTH, "\nMinPassed\n");

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    if (overallfail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, ChangedCPUDec0OutFile.c_str(), OutputStr0.c_str(), OutputStr1.c_str(), OutputStr2.c_str(), OutputStr3.c_str(), OutputStr4.c_str(), OutputStr5.c_str(), OutputStr6.c_str());

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, ChangedCPUDec0OutFile.c_str(), OutputStr0.c_str(), OutputStr1.c_str(), OutputStr2.c_str(), OutputStr3.c_str(), OutputStr4.c_str(), OutputStr5.c_str(), OutputStr6.c_str());

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}