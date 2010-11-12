#include "vpxt_test_declarations.h"

int test_fixed_quantizer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "FixedQ";
    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  FixedQ \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <FixedQ 1>\n "
            "    <FixedQ 2>\n "
            "    <Optional Settings File>\n"
        );
        return 0;
    }


    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "FixedQ";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string FixedQ1 = WorkingDirString;
    string FixedQ2 = WorkingDirString;

    FixedQ1.append(slashCharStr());
    FixedQ1.append("FixedQ_");
    FixedQ2.append(slashCharStr());
    FixedQ2.append("FixedQ_");

    FixedQ1.append(argv[5]);
    FixedQ2.append(argv[6]);

    FixedQ1.append(".ivf");
    FixedQ2.append(".ivf");

    //char FixedQ1[255];
    //char FixedQ2[255];

    //snprintf(FixedQ1, 255, "%s", WorkingDir4.c_str());
    //snprintf(FixedQ2, 255, "%s", WorkingDir5.c_str());

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

    int speed = 0;
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int FixedQ1Int = atoi(argv[5]);
    int FixedQ2Int = atoi(argv[6]);

    tprintf("Fixed Q Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
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
    opt.fixed_q = 1;
    int FixedQCheck1 = FixedQ1Int;
    int FixedQCheck2 = FixedQ2Int;

    if (FixedQ1Int < 0 || FixedQ1Int > 63 || FixedQ2Int < 0 || FixedQ2Int > 63)
    {
        tprintf("\n\nInvaild Qunatizer Range, Vaild range = 0-63 - Indeterminate\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 8;
    }

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.fixed_q = FixedQ1Int;

        if (vpxt_compress_ivf_to_ivf(input, FixedQ1.c_str(), speed, BitRate, opt, CompressString, FixedQ1Int, 1) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.fixed_q = FixedQ2Int;

        if (vpxt_compress_ivf_to_ivf(input, FixedQ2.c_str(), speed, BitRate, opt, CompressString, FixedQ2Int, 1) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
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

    tprintf("\n");

    int fail = 0;

    tprintf("\n");
    int FixedQCheckVal1 = vpxt_check_fixed_quantizer(FixedQ1.c_str(), FixedQ1Int);
    tprintf("\n");
    int FixedQCheckVal2 = vpxt_check_fixed_quantizer(FixedQ2.c_str(), FixedQ2Int);
    tprintf("\n");

    tprintf("\n");
    int FixedQSize1 = vpxt_file_size(FixedQ1.c_str(), 1);
    tprintf("\n");
    int FixedQSize2 = vpxt_file_size(FixedQ2.c_str(), 1);
    tprintf("\n");

    char FixedQ1FileName[255] = "";
    char FixedQ2FileName[255] = "";

    vpxt_file_name(FixedQ1.c_str(), FixedQ1FileName, 0);
    vpxt_file_name(FixedQ2.c_str(), FixedQ2FileName, 0);

    tprintf("\n\nResults:\n\n");

    if (FixedQCheckVal1 != -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Quantizer not fixed for %s for frame %i - Failed", FixedQ1FileName, FixedQCheckVal1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Quantizer fixed at %i for all frames for %s - Passed", FixedQ1Int, FixedQ1FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (FixedQCheckVal2 != -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Quantizer not fixed for %s for frame %i - Failed", FixedQ2FileName, FixedQCheckVal2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Quantizer fixed at %i for all frames for %s - Passed", FixedQ2Int, FixedQ2FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (FixedQ1Int < FixedQ2Int) //make sure that lower fixed q has higher datarate
    {

        if (FixedQSize1 <= FixedQSize2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s file size: %i >= %s file size: %i - Failed", FixedQ2FileName, FixedQSize2, FixedQ1FileName, FixedQSize1);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s file size: %i > %s file size: %i - Passed", FixedQ1FileName, FixedQSize1, FixedQ2FileName, FixedQSize2);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
    }
    else
    {
        if (FixedQSize2 <= FixedQSize1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s file size: %i >= %s file size: %i - Failed", FixedQ1FileName, FixedQSize1, FixedQ2FileName, FixedQSize2);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "%s file size: %i > %s file size: %i - Failed", FixedQ2FileName, FixedQSize2, FixedQ1FileName, FixedQSize1);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
    }

    if (fail == 1)
    {
        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }
    else
    {
        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}