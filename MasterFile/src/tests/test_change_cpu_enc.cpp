#include "vpxt_test_declarations.h"

int test_change_cpu_enc(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "Version:";

    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  ChangeCPUWorks \n\n"
            "    <inputfile>\n"
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
        );
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "ChangeCPUWorks";
    string MainDirString = "";
    char File1[255] = "";


    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string ChangedCPUDec0OutFile = WorkingDirString;
    string ChangedCPUDecNOutBase = WorkingDirString;

    ChangedCPUDec0OutFile.append(slashCharStr());
    ChangedCPUDec0OutFile.append("ChangeCPUWorksOutput_NONE.ivf");
    ChangedCPUDecNOutBase.append(slashCharStr());
    ChangedCPUDecNOutBase.append("ChangeCPUWorksOutput_");

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

    tprintf("Change CPU Works Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int VersionNum = atoi(argv[5]);

    int speed = 0;
    int Fail = 0;
    int ModesRun = 0;

    unsigned int cpu_tick1 = 0;
    unsigned int cpu_tick2 = 0;

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
    opt.Version = VersionNum;
    int CompressInt = opt.Version;
    opt.Mode = Mode;
    int counter = 0;

    putenv("ON2_SIMD_CAPS=0");

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        vector<string> CompressonVector;

        string OutputStr0 = ChangedCPUDecNOutBase;
        OutputStr0.append("None.ivf");
        string OutputStr1 = ChangedCPUDecNOutBase;
        OutputStr1.append("MMX.ivf");
        string OutputStr2 = ChangedCPUDecNOutBase;
        OutputStr2.append("SSE.ivf");
        string OutputStr3 = ChangedCPUDecNOutBase;
        OutputStr3.append("SSE2.ivf");
        string OutputStr4 = ChangedCPUDecNOutBase;
        OutputStr4.append("SSE3.ivf");
        string OutputStr5 = ChangedCPUDecNOutBase;
        OutputStr5.append("SSSE3.ivf");
        string OutputStr6 = ChangedCPUDecNOutBase;
        OutputStr6.append("SSE4_1.ivf");

        if (vpxt_file_exists_check(OutputStr0))CompressonVector.push_back(OutputStr0);

        if (vpxt_file_exists_check(OutputStr1))CompressonVector.push_back(OutputStr1);

        if (vpxt_file_exists_check(OutputStr2))CompressonVector.push_back(OutputStr2);

        if (vpxt_file_exists_check(OutputStr3))CompressonVector.push_back(OutputStr3);

        if (vpxt_file_exists_check(OutputStr4))CompressonVector.push_back(OutputStr4);

        if (vpxt_file_exists_check(OutputStr5))CompressonVector.push_back(OutputStr5);

        if (vpxt_file_exists_check(OutputStr6))CompressonVector.push_back(OutputStr6);

        ModesRun = CompressonVector.size();

        cpu_tick1 = vpxt_cpu_tick_return(CompressonVector[0].c_str(), 0);

        int CurrentFile = 0;

        while (CurrentFile < CompressonVector.size())
        {

            cpu_tick2 = vpxt_cpu_tick_return(CompressonVector[CurrentFile].c_str(), 0);

            if (CurrentFile >= 1)
            {
                tprintf("\n");

                char CompFile1[255];
                char CompFile2[255];
                vpxt_file_name(CompressonVector[CurrentFile-1].c_str(), CompFile1, 0);
                vpxt_file_name(CompressonVector[CurrentFile].c_str(), CompFile2, 0);

                tprintf("\nComparing %s to %s\n", CompFile1, CompFile2);

                int lngRC = vpxt_compare_ivf(CompressonVector[CurrentFile-1].c_str(), CompressonVector[CurrentFile].c_str());

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

            CurrentFile++;
        }

        tprintf("\n");
    }
    else
    {
        vector<string> CompressonVector;

        putenv("ON2_SIMD_CAPS=0");
        string OutputStr = ChangedCPUDecNOutBase;
        OutputStr.append("NONE.ivf");

        tprintf("\n\nDetected CPU capability: NONE");
        unsigned int Time1 = vpxt_time_compress_ivf_to_ivf(input, OutputStr.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, cpu_tick1);
        CompressonVector.push_back(OutputStr);

        if (Time1 == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        tprintf("\n");

        int counterMax = 64;
        int FileNumber = 1;
        counter++;

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

                ///////////Updating CPU///////////
                string CPUIDSTRING = "ON2_SIMD_CAPS=";
                char CounterChar[10];
                vpx_itoa_custom(counter, CounterChar, 10);
                CPUIDSTRING.append(CounterChar);

                char CPUChar[255];
                snprintf(CPUChar, 255, CPUIDSTRING.c_str());
                putenv(CPUChar);

                //tprintf("\n\nCPU:%i", counter);

                ///////Compresion and Time ///////
                string ChangedCPUDecNOutCurrent = ChangedCPUDecNOutBase;
                string ChangedCPUDecNOutLast = ChangedCPUDecNOutBase;

                char count[20];
                vpx_itoa_custom(counter, count, 10);
                ChangedCPUDecNOutCurrent.append(CPUStr.c_str());
                ChangedCPUDecNOutCurrent.append(".ivf");

                opt.Mode = Mode;
                unsigned int Time2 = vpxt_time_compress_ivf_to_ivf(input, ChangedCPUDecNOutCurrent.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, cpu_tick2);
                CompressonVector.push_back(ChangedCPUDecNOutCurrent);

                if (Time2 == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                if (TestType != 2 && counter != 0)
                {
                    char CompFile1[255];
                    char CompFile2[255];
                    vpxt_file_name(CompressonVector[CompressonVector.size()-1].c_str(), CompFile1, 0);
                    vpxt_file_name(CompressonVector[CompressonVector.size()-2].c_str(), CompFile2, 0);

                    tprintf("\nComparing %s to %s\n", CompFile1, CompFile2);

                    int lngRC = vpxt_compare_ivf(CompressonVector[CompressonVector.size()-1].c_str(), CompressonVector[CompressonVector.size()-2].c_str());

                    if (lngRC >= 0)
                    {
                        tprintf("\n * Fail: Files differ at frame: %i on file number %i\n", lngRC, FileNumber);
                        Fail = 1;
                    }

                    if (lngRC == -1)
                    {
                        tprintf(" * Files are identical\n");
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
            tprintf("\n");
            FileNumber++;
        }

        ModesRun = CompressonVector.size();
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        putenv("ON2_SIMD_CAPS=");
        return 10;
    }

    //cpu_tick1 = vpxt_cpu_tick_return(ChangedCPUDec0OutFile.c_str(), 0);
    int overallfail = 0;

    tprintf("\n\nResults:\n\n");

    if (Fail != 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files are identical - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (Fail == 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All Files are not identical - Failed");
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

    if (cpu_tick1 == cpu_tick2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "cpu_tick1: %u == cpu_tick2: %u - Failed", cpu_tick1, cpu_tick2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        overallfail = 1;
    }

    if (cpu_tick1 != cpu_tick2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "cpu_tick1: %u != cpu_tick2: %u - Passed", cpu_tick1, cpu_tick2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
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