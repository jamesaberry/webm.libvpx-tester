#include "vpxt_test_declarations.h"

int test_fixed_quantizer(int argc, char *argv[], const string &WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Fixed Quantizer";
    char *MyDir = "test_fixed_quantizer";

    if (!(argc == 7 || argc == 8))
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
                "    <FixedQ 1>\n"
                "    <FixedQ 2>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int FixedQ1Int = atoi(argv[5]);
    int FixedQ2Int = atoi(argv[6]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    string FixedQ1 = CurTestDirStr;
    FixedQ1.append(slashCharStr());
    FixedQ1.append(MyDir);
    FixedQ1.append("_compression_");
    FixedQ1.append(argv[5]);
    FixedQ1.append(".ivf");

    string FixedQ2 = CurTestDirStr;
    FixedQ2.append(slashCharStr());
    FixedQ2.append(MyDir);
    FixedQ2.append("_compression_");
    FixedQ2.append(argv[6]);
    FixedQ2.append(".ivf");

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

    /////////////////////////////////////////////////////////
    opt.target_bandwidth = BitRate;
    opt.fixed_q = 1;
    int FixedQCheck1 = FixedQ1Int;
    int FixedQCheck2 = FixedQ2Int;

    if (FixedQ1Int < 0 || FixedQ1Int > 63 || FixedQ2Int < 0 || FixedQ2Int > 63)
    {
        tprintf(PRINT_BTH, "\n\nInvaild Qunatizer Range, Vaild range = 0-63 - Indeterminate\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 8;
    }

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.fixed_q = FixedQ1Int;

        if (vpxt_compress_ivf_to_ivf(input, FixedQ1.c_str(), speed, BitRate, opt, CompressString, FixedQ1Int, 1, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.fixed_q = FixedQ2Int;

        if (vpxt_compress_ivf_to_ivf(input, FixedQ2.c_str(), speed, BitRate, opt, CompressString, FixedQ2Int, 1, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
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

    tprintf(PRINT_BTH, "\n");

    int fail = 0;

    tprintf(PRINT_BTH, "\n");
    int FixedQCheckVal1 = vpxt_check_fixed_quantizer(FixedQ1.c_str(), FixedQ1Int);
    tprintf(PRINT_BTH, "\n");
    int FixedQCheckVal2 = vpxt_check_fixed_quantizer(FixedQ2.c_str(), FixedQ2Int);
    tprintf(PRINT_BTH, "\n");

    tprintf(PRINT_BTH, "\n");
    int FixedQSize1 = vpxt_file_size(FixedQ1.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    int FixedQSize2 = vpxt_file_size(FixedQ2.c_str(), 1);
    tprintf(PRINT_BTH, "\n");

    char FixedQ1FileName[255] = "";
    char FixedQ2FileName[255] = "";

    vpxt_file_name(FixedQ1.c_str(), FixedQ1FileName, 0);
    vpxt_file_name(FixedQ2.c_str(), FixedQ2FileName, 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (FixedQCheckVal1 != -1)
    {
        vpxt_formated_print(RESPRT, "Quantizer not fixed for %s for frame %i - Failed", FixedQ1FileName, FixedQCheckVal1);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Quantizer fixed at %i for all frames for %s - Passed", FixedQ1Int, FixedQ1FileName);
        tprintf(PRINT_BTH, "\n");
    }

    if (FixedQCheckVal2 != -1)
    {
        vpxt_formated_print(RESPRT, "Quantizer not fixed for %s for frame %i - Failed", FixedQ2FileName, FixedQCheckVal2);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Quantizer fixed at %i for all frames for %s - Passed", FixedQ2Int, FixedQ2FileName);
        tprintf(PRINT_BTH, "\n");
    }

    if (FixedQ1Int < FixedQ2Int) //make sure that lower fixed q has higher datarate
    {

        if (FixedQSize1 <= FixedQSize2)
        {
            vpxt_formated_print(RESPRT, "%s file size: %i >= %s file size: %i - Failed", FixedQ2FileName, FixedQSize2, FixedQ1FileName, FixedQSize1);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "%s file size: %i > %s file size: %i - Passed", FixedQ1FileName, FixedQSize1, FixedQ2FileName, FixedQSize2);
            tprintf(PRINT_BTH, "\n");
        }
    }
    else
    {
        if (FixedQSize2 <= FixedQSize1)
        {
            vpxt_formated_print(RESPRT, "%s file size: %i >= %s file size: %i - Failed", FixedQ1FileName, FixedQSize1, FixedQ2FileName, FixedQSize2);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "%s file size: %i > %s file size: %i - Failed", FixedQ2FileName, FixedQSize2, FixedQ1FileName, FixedQSize1);
            tprintf(PRINT_BTH, "\n");
        }
    }

    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }
    else
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}