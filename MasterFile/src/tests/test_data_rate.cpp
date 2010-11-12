#include "vpxt_test_declarations.h"

int test_data_rate(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "AllowDF";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  DataRateTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <Optional Settings File>\n");
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "DataRateTest";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string TargetBitRate1 = WorkingDirString;
    //string WorkingDir5 = WorkingDirString;

    TargetBitRate1.append(slashCharStr());
    TargetBitRate1.append("TargetBitRateOutput.ivf");

    //char TargetBitRate1[255];
    //snprintf(TargetBitRate1, 255, "%s", WorkingDir4.c_str());

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

    printf("Data Rate Test");
    fprintf(stderr, "Data Rate Test");


    int speed = 0;

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    double BitRate = atoi(argv[4]);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    int TargetDataRatePercentage = 30;

    opt.target_bandwidth = BitRate;

    ////////////////Data Rate Specific Settings////////////////
    opt.best_allowed_q = 0;
    opt.worst_allowed_q = 63;
    opt.allow_df = 1;
    ///////////////////////////////////////////////////////////

    int CompressInt = opt.allow_df;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input, TargetBitRate1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
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
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    //need to clean this up
    int num = 3;
    char *in2;
    in2 = &TargetBitRate1[0];


    printf("\n");
    fprintf(stderr, "\n");
    double FileDataRate = vpxt_ivf_data_rate(TargetBitRate1.c_str(), 1);
    double DataRateProx = vpxt_abs_double(100 - vpxt_abs_double(((FileDataRate * 100) / BitRate)));

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    if (DataRateProx < TargetDataRatePercentage)
    {
        if (FileDataRate < BitRate)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% lower than Target, DataRate is within %i%% of: %4.2f - Passed", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% greater than Target, DataRate is within %i%% of: %4.2f - Passed", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        printf("\nPassed\n");
        fprintf(stderr, "\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {

        if (FileDataRate < BitRate)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% less than Target, DataRate not within %i%% of: %4.2f - Failed \n", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DataRate: %4.2f is %4.2f%% greater than Target, DataRate not within %i%% of: %4.2f - Failed \n", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");

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