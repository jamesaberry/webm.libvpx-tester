#include "vpxt_test_declarations.h"

int test_change_cpu_dec(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Version";
    char *MyDir = "test_change_cpu_dec";

    if (!(argc == 7 || argc == 6))
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
                "    <Version>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int VersionNum = atoi(argv[5]);

    int DoOnceSpeedRead = 0;
    unsigned int totalms = 0;
    unsigned int totalms2 = 0;

    int speed = 0;
    int Fail = 0;
    int ModesRun = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string CPUDecOnlyWorksOutFile = CurTestDirStr;
    CPUDecOnlyWorksOutFile.append(slashCharStr());
    CPUDecOnlyWorksOutFile.append(MyDir);
    CPUDecOnlyWorksOutFile.append("_compression.ivf");

    std::string CPUDecOnlyWorksOut_CPU = CurTestDirStr;
    CPUDecOnlyWorksOut_CPU.append(slashCharStr());
    CPUDecOnlyWorksOut_CPU.append(MyDir);
    CPUDecOnlyWorksOut_CPU.append("_decompression");

    std::string OutputStr0 = CPUDecOnlyWorksOut_CPU;
    OutputStr0.append("none.ivf");
    std::string OutputStr1 = CPUDecOnlyWorksOut_CPU;
    OutputStr1.append("mmx.ivf");
    std::string OutputStr2 = CPUDecOnlyWorksOut_CPU;
    OutputStr2.append("sse.ivf");
    std::string OutputStr3 = CPUDecOnlyWorksOut_CPU;
    OutputStr3.append("sse2.ivf");
    std::string OutputStr4 = CPUDecOnlyWorksOut_CPU;
    OutputStr4.append("sse3.ivf");
    std::string OutputStr5 = CPUDecOnlyWorksOut_CPU;
    OutputStr5.append("ssse3.ivf");
    std::string OutputStr6 = CPUDecOnlyWorksOut_CPU;
    OutputStr6.append("sse4_1.ivf");

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

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.Mode = MODE_GOODQUALITY;
    opt.Version = VersionNum;
    int CompressInt = opt.Version;

    int counter = 0;
    int Mode2 = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        std::vector<std::string> DecompressonVector;

        if (vpxt_file_exists_check(OutputStr0))
            DecompressonVector.push_back(OutputStr0);

        if (vpxt_file_exists_check(OutputStr1))
            DecompressonVector.push_back(OutputStr1);

        if (vpxt_file_exists_check(OutputStr2))
            DecompressonVector.push_back(OutputStr2);

        if (vpxt_file_exists_check(OutputStr3))
            DecompressonVector.push_back(OutputStr3);

        if (vpxt_file_exists_check(OutputStr4))
            DecompressonVector.push_back(OutputStr4);

        if (vpxt_file_exists_check(OutputStr5))
            DecompressonVector.push_back(OutputStr5);

        if (vpxt_file_exists_check(OutputStr6))
            DecompressonVector.push_back(OutputStr6);

        ModesRun = DecompressonVector.size();

        totalms = vpxt_time_return(DecompressonVector[0].c_str(), 1);

        int CurrentDecFile = 0;

        while (CurrentDecFile < DecompressonVector.size())
        {

            totalms2 = vpxt_time_return(DecompressonVector[CurrentDecFile].c_str(), 1);

            if (CurrentDecFile >= 1)
            {
                tprintf(PRINT_BTH, "\n");

                char CompFileIndexOutputChar[255];
                char CompFile2[255];
                vpxt_file_name(DecompressonVector[CurrentDecFile-1].c_str(), CompFileIndexOutputChar, 0);
                vpxt_file_name(DecompressonVector[CurrentDecFile].c_str(), CompFile2, 0);

                tprintf(PRINT_BTH, "\nComparing %s to %s\n", CompFileIndexOutputChar, CompFile2);

                int lngRC = vpxt_compare_ivf(DecompressonVector[CurrentDecFile-1].c_str(), DecompressonVector[CurrentDecFile].c_str());

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

            CurrentDecFile++;
        }

        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        std::vector<std::string> DecompressonVector;
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), CPUDecOnlyWorksOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        putenv("ON2_SIMD_CAPS=0");

        std::string Output2Str = CPUDecOnlyWorksOut_CPU;
        Output2Str.append("NONE.ivf");

        tprintf(PRINT_BTH, "\n\nDetected CPU capability: NONE");
        unsigned int CPUTick1 = 0;
        totalms = vpxt_decompress_ivf_to_ivf_time_and_output(CPUDecOnlyWorksOutFile.c_str(), Output2Str.c_str(), CPUTick1);
        DecompressonVector.push_back(Output2Str);

        if (totalms == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_BTH, "\n");

        int counterMax = 64;
        counter++;
        int i = 1;

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

                std::string CPUIDSTRING = "ON2_SIMD_CAPS=";
                char CounterChar[10];
                vpxt_itoa_custom(counter, CounterChar, 10);
                CPUIDSTRING.append(CounterChar);

                char CPUChar[255];
                snprintf(CPUChar, 255, CPUIDSTRING.c_str());
                putenv(CPUChar);

                //////////////////////////////////
                ///////Compresion and Time ///////

                std::string ChangedCPUDecOutFileStr1 = CPUDecOnlyWorksOut_CPU;
                std::string ChangedCPUDecOutFileStr2 = CPUDecOnlyWorksOut_CPU;

                char count[20];
                vpxt_itoa_custom(counter, count, 10);
                ChangedCPUDecOutFileStr1.append(CPUStr.c_str());
                ChangedCPUDecOutFileStr1.append(".ivf");

                unsigned int CPUTick2 = 0;
                totalms2 = vpxt_decompress_ivf_to_ivf_time_and_output(CPUDecOnlyWorksOutFile.c_str(), ChangedCPUDecOutFileStr1.c_str(), CPUTick2);
                DecompressonVector.push_back(ChangedCPUDecOutFileStr1);

                if (totalms2 == -1)
                {
                    fclose(fp);
                    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                    return 2;
                }

                int countOld = (counter - 1);
                vpxt_itoa_custom(countOld, count, 10);
                ChangedCPUDecOutFileStr2.append(CPUStr.c_str());
                ChangedCPUDecOutFileStr2.append(".ivf");

                if (TestType != 2)
                {
                    char CompFileIndexOutputChar[255];
                    char CompFile2[255];
                    vpxt_file_name(DecompressonVector[DecompressonVector.size()-1].c_str(), CompFileIndexOutputChar, 0);
                    vpxt_file_name(DecompressonVector[DecompressonVector.size()-2].c_str(), CompFile2, 0);

                    tprintf(PRINT_BTH, "\nComparing %s to %s\n", CompFileIndexOutputChar, CompFile2);

                    int lngRC = vpxt_compare_ivf(DecompressonVector[DecompressonVector.size()-1].c_str(), DecompressonVector[DecompressonVector.size()-2].c_str());

                    if (lngRC >= 0)
                    {
                        tprintf(PRINT_BTH, "\n * Fail: Files differ at frame: %i on file number %i", lngRC, i);
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
            }

            counter = ((counter + 1) * 2) - 1;
            tprintf(PRINT_BTH, "\n\n");
            i++;
        }

        ModesRun = DecompressonVector.size();
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        putenv("ON2_SIMD_CAPS=");
        return 10;
    }

    int overallfail = 0;
    tprintf(PRINT_BTH, "\nResults:\n\n");

    if (Fail == 0)
    {
        vpxt_formated_print(RESPRT, "All Files Identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (Fail == 1)
    {
        vpxt_formated_print(RESPRT, "All Files not Identical - Failed");
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

    if (totalms != totalms2)
    {
        vpxt_formated_print(RESPRT, "Decompress times are not equal - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (totalms == totalms2)
    {
        vpxt_formated_print(RESPRT, "CPU changes are not effecting the runtime - Failed");
        tprintf(PRINT_BTH, "\n");
        overallfail = 1;
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
            vpxt_delete_files(8, CPUDecOnlyWorksOutFile.c_str(), OutputStr0.c_str(), OutputStr1.c_str(), OutputStr2.c_str(), OutputStr3.c_str(), OutputStr4.c_str(), OutputStr5.c_str(), OutputStr6.c_str());

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, CPUDecOnlyWorksOutFile.c_str(), OutputStr0.c_str(), OutputStr1.c_str(), OutputStr2.c_str(), OutputStr3.c_str(), OutputStr4.c_str(), OutputStr5.c_str(), OutputStr6.c_str());

        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}