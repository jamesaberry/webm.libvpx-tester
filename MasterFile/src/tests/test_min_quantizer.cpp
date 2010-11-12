#include "vpxt_test_declarations.h"

int test_min_quantizer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "BestAllowedQ";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  MinQTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <Optional Settings File>\n"
        );
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "MinQTest";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr());
    WorkingDir4.append("Min10QuantOutput.ivf");
    WorkingDir5.append(slashCharStr());
    WorkingDir5.append("Min60QuantOutput.ivf");

    char Min10QuantOutFile[255];
    char Min60QuantOutFile[255];

    snprintf(Min10QuantOutFile, 255, "%s", WorkingDir4.c_str());
    snprintf(Min60QuantOutFile, 255, "%s", WorkingDir5.c_str());

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
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);


    tprintf("Min Quantizer Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
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
    double PSNRArr[2];
    opt.target_bandwidth = BitRate;
    opt.worst_allowed_q = 63;

    int n = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.best_allowed_q = 10;

        if (vpxt_compress_ivf_to_ivf(input, Min10QuantOutFile, speed, BitRate, opt, CompressString, 10, 1) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.best_allowed_q = 60;

        if (vpxt_compress_ivf_to_ivf(input, Min60QuantOutFile, speed, BitRate, opt, CompressString, 60, 1) == -1)
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

    PSNRArr[0] = vpxt_ivf_psnr(input, Min10QuantOutFile, 0, 0, 1, NULL);
    PSNRArr[1] = vpxt_ivf_psnr(input, Min60QuantOutFile, 0, 0, 1, NULL);

    tprintf("\n");
    int Min10Q = vpxt_check_min_quantizer(Min10QuantOutFile, 10);
    tprintf("\n\n");
    int Min60Q = vpxt_check_min_quantizer(Min60QuantOutFile, 60);
    tprintf("\n\n");

    char Min10FileName[255] = "";
    char Min60FileName[255] = "";

    vpxt_file_name(Min10QuantOutFile, Min10FileName, 0);
    vpxt_file_name(Min60QuantOutFile, Min60FileName, 0);

    int fail = 0;

    tprintf("\n\nResults:\n\n");

    if (Min10Q != -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Not all %s quantizers above MinQ - Failed", Min10FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All %s quantizers above MinQ - Passed", Min10FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (Min60Q != -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Not all %s quantizers above MinQ - Failed", Min60FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All %s quantizers above MinQ - Passed", Min60FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (PSNRArr[0] <= PSNRArr[1])
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "MinQ 10 PSNR: %2.2f <= MinQ 60 PSNR: %2.2f - Failed", PSNRArr[0], PSNRArr[1]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "MinQ 10 PSNR: %2.2f > MinQ 60 PSNR: %2.2f - Passed", PSNRArr[0], PSNRArr[1]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
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