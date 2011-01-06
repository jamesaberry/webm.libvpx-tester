#include "vpxt_test_declarations.h"

int test_data_rate(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_data_rate";

    if (!(argc == 6 || argc == 5))
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
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    double BitRate = atoi(argv[4]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string TargetBitRate1 = CurTestDirStr;
    TargetBitRate1.append(slashCharStr());
    TargetBitRate1.append(MyDir);
    TargetBitRate1.append("_compression.ivf");

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
    if (argc == 6)
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

    int TargetDataRatePercentage = 30;
    opt.target_bandwidth = BitRate;

    ////////////////Data Rate Specific Settings////////////////
    opt.best_allowed_q = 0;
    opt.worst_allowed_q = 63;
    opt.allow_df = 1;
    ///////////////////////////////////////////////////////////

    int CompressInt = opt.allow_df;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), TargetBitRate1.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n");

    double FileDataRate = vpxt_ivf_data_rate(TargetBitRate1.c_str(), 1);
    double DataRateProx = vpxt_abs_double(100 - vpxt_abs_double(((FileDataRate * 100) / BitRate)));

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (DataRateProx < TargetDataRatePercentage)
    {
        if (FileDataRate < BitRate)
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% lower than Target, DataRate is within %i%% of: %4.2f - Passed", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% greater than Target, DataRate is within %i%% of: %4.2f - Passed", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            tprintf(PRINT_BTH, "\n");
        }

        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {

        if (FileDataRate < BitRate)
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% less than Target, DataRate not within %i%% of: %4.2f - Failed \n", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DataRate: %4.2f is %4.2f%% greater than Target, DataRate not within %i%% of: %4.2f - Failed \n", FileDataRate, DataRateProx, TargetDataRatePercentage, BitRate);
            tprintf(PRINT_BTH, "\n");
        }

        tprintf(PRINT_STD, "\nFailed\n");
        fprintf(stderr, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}