#include "vpxt_test_declarations.h"

int test_constrained_quality(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType)
{
    char *CompressString = "Constrained Quality";
    char *MyDir = "test_constrained_quality";

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
                "    <Constrained Quantizer>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int ConstrainedQ = atoi(argv[5]);

    int speed = 0;
    unsigned int Time1 = 0;
    unsigned int Time2 = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string ConstrainedQon = CurTestDirStr;
    ConstrainedQon.append(slashCharStr());
    ConstrainedQon.append(MyDir);
    ConstrainedQon.append("_compression_1.ivf");

    std::string ConstrainedQoff = CurTestDirStr;
    ConstrainedQoff.append(slashCharStr());
    ConstrainedQoff.append(MyDir);
    ConstrainedQoff.append("_compression_0.ivf");

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

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.end_usage = 1;
        unsigned int cpu_tick1_conq_off = 0;

        Time1 = vpxt_time_compress_ivf_to_ivf(input.c_str(), ConstrainedQoff.c_str(), speed, BitRate, opt, CompressString, 0, 1, cpu_tick1_conq_off);

        if (Time1 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.end_usage = 2;
        opt.cq_level = ConstrainedQ;
        unsigned int cpu_tick1_conq_on = 0;

        Time2 = vpxt_time_compress_ivf_to_ivf(input.c_str(), ConstrainedQon.c_str(), speed, BitRate, opt, CompressString, 1, 1, cpu_tick1_conq_on);

        if (Time2 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    double ConstrainedQonPSNR = vpxt_ivf_psnr(input.c_str(), ConstrainedQon.c_str(), 0, 0, 1, NULL);
    double ConstrainedQoffPSNR = vpxt_ivf_psnr(input.c_str(), ConstrainedQoff.c_str(), 0, 0, 1, NULL);

    double ConstrainedQonDataRate = vpxt_ivf_data_rate(ConstrainedQon.c_str(), 1);
    double ConstrainedQoffDataRate = vpxt_ivf_data_rate(ConstrainedQoff.c_str(), 1);

    vpxt_display_alt_ref_frames(ConstrainedQon.c_str(), 1);
    vpxt_display_alt_ref_frames(ConstrainedQoff.c_str(), 1);
    vpxt_display_key_frames(ConstrainedQon.c_str(), 1);
    vpxt_display_key_frames(ConstrainedQoff.c_str(), 1);

    tprintf(PRINT_BTH, "\nCQ on  PSNR: %.2f CQ on  Data Rate: %.2f CQ on  linearized PSNR: %.2f\n", ConstrainedQonPSNR, ConstrainedQonDataRate, pow(10.0, (ConstrainedQonPSNR / 10.0)));
    tprintf(PRINT_BTH, "CQ off PSNR: %.2f CQ off Data Rate: %.2f CQ off linearized PSNR: %.2f\n", ConstrainedQoffPSNR, ConstrainedQoffDataRate, pow(10.0, (ConstrainedQoffPSNR / 10.0)));
    tprintf(PRINT_BTH, "\nConstrained Q on  Time: %i microseconds\n", Time2);
    tprintf(PRINT_BTH, "Constrained Q off Time: %i microseconds\n", Time1);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

    if (pow(10.0, (ConstrainedQonPSNR / 10.0)) / ConstrainedQonDataRate > pow(10.0, (ConstrainedQoffPSNR / 10.0)) / ConstrainedQoffDataRate)
    {
        vpxt_formated_print(RESPRT, "CQ on lineraized psnr / data rate: %f > CQ off lineraized psnr / data rate: %f - Passed", pow(10.0, (ConstrainedQonPSNR / 10.0)) / ConstrainedQonDataRate, pow(10.0, (ConstrainedQoffPSNR / 10.0)) / ConstrainedQoffDataRate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "CQ on lineraized psnr / data rate: %f <= CQ off lineraized psnr / data rate: %f - Failed", pow(10.0, (ConstrainedQonPSNR / 10.0)) / ConstrainedQonDataRate, pow(10.0, (ConstrainedQoffPSNR / 10.0)) / ConstrainedQoffDataRate);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (ConstrainedQonPSNR > 25.0)
    {
        vpxt_formated_print(RESPRT, "Constrained quality PSNR: %f > 25 - Passed", ConstrainedQonPSNR);
        tprintf(PRINT_BTH, "\n");
    }

    if (ConstrainedQonPSNR <= 25.0)
    {
        vpxt_formated_print(RESPRT, "Constrained quality PSNR: %f <= 25 - Failed", ConstrainedQonPSNR);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);

    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
