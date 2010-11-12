#include "vpxt_test_declarations.h"

int test_change_cpu_dec(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Version";

    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  CPUDecOnlyWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Version>\n "
            "    <Optional Settings File>\n"
        );
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "CPUDecOnlyWorks";
    string MainDirString = "";
    char File1[255] = "";


    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string CPUDecOnlyWorksOutFile = WorkingDirString;
    string CPUDecOnlyWorksOut_CPU = WorkingDirString;

    CPUDecOnlyWorksOutFile.append(slashCharStr());
    CPUDecOnlyWorksOutFile.append("CPUDecOnlyWorksOutput.ivf");
    CPUDecOnlyWorksOut_CPU.append(slashCharStr());
    CPUDecOnlyWorksOut_CPU.append("CPUDecOnlyWorksOutput_");

    //char CPUDecOnlyWorksOutFile[255];
    //char CPUDecOnlyWorksOut_CPU[255];

    //snprintf(CPUDecOnlyWorksOutFile, 255, "%s", WorkingDir4.c_str());
    //snprintf(CPUDecOnlyWorksOut_CPU, 255, "%s", WorkingDir5.c_str());

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

    tprintf("CPU Dec Only Works Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int VersionNum = atoi(argv[5]);

    int DoOnceSpeedRead = 0;
    unsigned int totalms = 0;
    unsigned int totalms2 = 0;

    int speed = 0;
    int Fail = 0;
    int ModesRun = 0;
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
    opt.Mode = MODE_GOODQUALITY;
    opt.Version = VersionNum;
    int CompressInt = opt.Version;

    int counter = 0;
    int Mode2 = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        vector<string> DecompressonVector;

        string OutputStr0 = CPUDecOnlyWorksOut_CPU;
        OutputStr0.append("None.ivf");
        string OutputStr1 = CPUDecOnlyWorksOut_CPU;
        OutputStr1.append("MMX.ivf");
        string OutputStr2 = CPUDecOnlyWorksOut_CPU;
        OutputStr2.append("SSE.ivf");
        string OutputStr3 = CPUDecOnlyWorksOut_CPU;
        OutputStr3.append("SSE2.ivf");
        string OutputStr4 = CPUDecOnlyWorksOut_CPU;
        OutputStr4.append("SSE3.ivf");
        string OutputStr5 = CPUDecOnlyWorksOut_CPU;
        OutputStr5.append("SSSE3.ivf");
        string OutputStr6 = CPUDecOnlyWorksOut_CPU;
        OutputStr6.append("SSE4_1.ivf");

        if (vpxt_file_exists_check(OutputStr0))DecompressonVector.push_back(OutputStr0);

        if (vpxt_file_exists_check(OutputStr1))DecompressonVector.push_back(OutputStr1);

        if (vpxt_file_exists_check(OutputStr2))DecompressonVector.push_back(OutputStr2);

        if (vpxt_file_exists_check(OutputStr3))DecompressonVector.push_back(OutputStr3);

        if (vpxt_file_exists_check(OutputStr4))DecompressonVector.push_back(OutputStr4);

        if (vpxt_file_exists_check(OutputStr5))DecompressonVector.push_back(OutputStr5);

        if (vpxt_file_exists_check(OutputStr6))DecompressonVector.push_back(OutputStr6);

        ModesRun = DecompressonVector.size();

        totalms = vpxt_time_return(DecompressonVector[0].c_str(), 1);

        int CurrentDecFile = 0;

        while (CurrentDecFile < DecompressonVector.size())
        {

            totalms2 = vpxt_time_return(DecompressonVector[CurrentDecFile].c_str(), 1);

            if (CurrentDecFile >= 1)
            {
                tprintf("\n");

                char CompFile1[255];
                char CompFile2[255];
                vpxt_file_name(DecompressonVector[CurrentDecFile-1].c_str(), CompFile1, 0);
                vpxt_file_name(DecompressonVector[CurrentDecFile].c_str(), CompFile2, 0);

                tprintf("\nComparing %s to %s\n", CompFile1, CompFile2);

                int lngRC = vpxt_compare_ivf(DecompressonVector[CurrentDecFile-1].c_str(), DecompressonVector[CurrentDecFile].c_str());

                if (lngRC >= 0)
                {
                    tprintf("\n * Fail: Files differ at frame: %i", lngRC);
                    Fail = 1;
                }

                if (lngRC == -1)
                {
                    tprintf(" * Files are identical");
                }

                if (lngRC == -2)
                {
                    tprintf("\n * Fail: File 2 ends before File 1.\n");
                    Fail = 1;
                }

                if (lngRC == -3)
                {
                    tprintf("\n * Fail: File 1 ends before File 2.\n");
                    Fail = 1;
                }
            }

            CurrentDecFile++;
        }

        tprintf("\n");
    }
    else
    {
        vector<string> DecompressonVector;
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input, CPUDecOnlyWorksOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        putenv("ON2_SIMD_CAPS=0");

        string Output2Str = CPUDecOnlyWorksOut_CPU;
        Output2Str.append("NONE.ivf");

        tprintf("\n\nDetected CPU capability: NONE");

        totalms = vpxt_decompress_ivf_to_ivf_time_and_output(CPUDecOnlyWorksOutFile.c_str(), Output2Str.c_str());
        DecompressonVector.push_back(Output2Str);

        if (totalms == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        tprintf("\n");

        int counterMax = 64;
        counter++;
        int i = 1;

        int Simd_Caps = x86_simd_caps();

        while (counter < counterMax)
        {
            int CPUFound = 0;
            int Has_Check = (counter + 1) / 2;

            if ((Simd_Caps & HAS_MMX)    == Has_Check) CPUFound = 1;

            if ((Simd_Caps & HAS_SSE)    == Has_Check) CPUFound = 1;

            if ((Simd_Caps & HAS_SSE2)   == Has_Check) CPUFound = 1;

            if ((Simd_Caps & HAS_SSE3)   == Has_Check) CPUFound = 1;

            if ((Simd_Caps & HAS_SSSE3)  == Has_Check) CPUFound = 1;

            if ((Simd_Caps & HAS_SSE4_1) == Has_Check) CPUFound = 1;

            if (CPUFound == 1)
            {
                string CPUStr = "";
                tprintf("\nDetected CPU capability: ");

                if ((Simd_Caps & HAS_MMX)    == Has_Check)
                {
                    CPUStr = "MMX";
                }

                if ((Simd_Caps & HAS_SSE)    == Has_Check)
                {
                    CPUStr = "SSE";
                }

                if ((Simd_Caps & HAS_SSE2)   == Has_Check)
                {
                    CPUStr = "SSE2";
                }

                if ((Simd_Caps & HAS_SSE3)   == Has_Check)
                {
                    CPUStr = "SSE3";
                }

                if ((Simd_Caps & HAS_SSSE3)  == Has_Check)
                {
                    CPUStr = "SSSE3";
                }

                if ((Simd_Caps & HAS_SSE4_1) == Has_Check)
                {
                    CPUStr = "SSE4_1";
                }

                tprintf("%s", CPUStr.c_str());

                string CPUIDSTRING = "ON2_SIMD_CAPS=";
                char CounterChar[10];
                vpx_itoa_custom(counter, CounterChar, 10);
                CPUIDSTRING.append(CounterChar);

                char CPUChar[255];
                snprintf(CPUChar, 255, CPUIDSTRING.c_str());
                putenv(CPUChar);

                //////////////////////////////////
                ///////Compresion and Time ///////

                string ChangedCPUDecOutFileStr1 = CPUDecOnlyWorksOut_CPU;
                string ChangedCPUDecOutFileStr2 = CPUDecOnlyWorksOut_CPU;

                char count[20];
                vpx_itoa_custom(counter, count, 10);
                ChangedCPUDecOutFileStr1.append(CPUStr.c_str());
                ChangedCPUDecOutFileStr1.append(".ivf");

                totalms2 = vpxt_decompress_ivf_to_ivf_time_and_output(CPUDecOnlyWorksOutFile.c_str(), ChangedCPUDecOutFileStr1.c_str());
                DecompressonVector.push_back(ChangedCPUDecOutFileStr1);

                if (totalms2 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                int countOld = (counter - 1);
                vpx_itoa_custom(countOld, count, 10);
                ChangedCPUDecOutFileStr2.append(CPUStr.c_str());
                ChangedCPUDecOutFileStr2.append(".ivf");

                if (TestType != 2)
                {
                    char CompFile1[255];
                    char CompFile2[255];
                    vpxt_file_name(DecompressonVector[DecompressonVector.size()-1].c_str(), CompFile1, 0);
                    vpxt_file_name(DecompressonVector[DecompressonVector.size()-2].c_str(), CompFile2, 0);

                    tprintf("\nComparing %s to %s\n", CompFile1, CompFile2);

                    int lngRC = vpxt_compare_ivf(DecompressonVector[DecompressonVector.size()-1].c_str(), DecompressonVector[DecompressonVector.size()-2].c_str());

                    if (lngRC >= 0)
                    {
                        tprintf("\n * Fail: Files differ at frame: %i on file number %i", lngRC, i);
                        Fail = 1;
                    }

                    if (lngRC == -1)
                    {
                        tprintf(" * Files are identical");
                    }

                    if (lngRC == -2)
                    {
                        tprintf("\n * Fail: File 2 ends before File 1.\n");
                        Fail = 1;
                    }

                    if (lngRC == -3)
                    {
                        tprintf("\n * Fail: File 1 ends before File 2.\n");
                        Fail = 1;
                    }
                }
            }

            counter = ((counter + 1) * 2) - 1;
            tprintf("\n\n");
            i++;
        }

        ModesRun = DecompressonVector.size();
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        putenv("ON2_SIMD_CAPS=");
        return 10;
    }

    int overallfail = 0;
    tprintf("\nResults:\n\n");

    if (Fail == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files Identical - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (Fail == 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files not Identical - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        overallfail = 1;
    }

    if (ModesRun == 7)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All instruction sets run - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (ModesRun != 7)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Not all instruction sets run - MinPassed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        if (overallfail != 1) overallfail = 2;
    }

    if (totalms != totalms2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Decompress times are not equal - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (totalms == totalms2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "CPU changes are not effecting the runtime - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        overallfail = 1;
    }

    if (overallfail == 2)
    {
        tprintf("\nMinPassed\n");
        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    if (overallfail == 0)
    {
        tprintf("\nPassed\n");
        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        tprintf("\nFailed\n");
        fclose(fp);
        putenv("ON2_SIMD_CAPS=");
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}