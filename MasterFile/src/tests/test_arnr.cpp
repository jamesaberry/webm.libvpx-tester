#include "vpxt_test_declarations.h"

int test_arnr(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Arnr Type";
    char *MyDir = "test_arnr";

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
    int BitRate = atoi(argv[4]);
    //int Arnr = atoi(argv[5]);

    int speed = 0;
    unsigned int Time0 = 0;
    unsigned int Time1 = 0;
    unsigned int Time2 = 0;
    unsigned int Time3 = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string CompArnr1AltRef0 = CurTestDirStr;
    CompArnr1AltRef0.append(slashCharStr());
    CompArnr1AltRef0.append(MyDir);
    CompArnr1AltRef0.append("_compression_1_altref_0.ivf");

    std::string CompArnr1 = CurTestDirStr;
    CompArnr1.append(slashCharStr());
    CompArnr1.append(MyDir);
    CompArnr1.append("_compression_1.ivf");

    std::string CompArnr2 = CurTestDirStr;
    CompArnr2.append(slashCharStr());
    CompArnr2.append(MyDir);
    CompArnr2.append("_compression_2.ivf");

    std::string CompArnr3 = CurTestDirStr;
    CompArnr3.append(slashCharStr());
    CompArnr3.append(MyDir);
    CompArnr3.append("_compression_3.ivf");

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

    opt.target_bandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.arnr_max_frames = 10;
        opt.allow_lag = 1;
        opt.lag_in_frames = 20;

        opt.play_alternate = 0;
        opt.arnr_type = 1;
        unsigned int cpu_tick0 = 0;

        Time0 = vpxt_time_compress_ivf_to_ivf(input.c_str(), CompArnr1AltRef0.c_str(), speed, BitRate, opt, "Play Alternate", opt.arnr_type, 0, cpu_tick0);

        if (Time1 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.play_alternate = 1;
        opt.arnr_type = 1;
        unsigned int cpu_tick1 = 0;

        Time1 = vpxt_time_compress_ivf_to_ivf(input.c_str(), CompArnr1.c_str(), speed, BitRate, opt, CompressString, opt.arnr_type, 0, cpu_tick1);

        if (Time1 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.arnr_type = 2;
        unsigned int cpu_tick2 = 0;

        Time2 = vpxt_time_compress_ivf_to_ivf(input.c_str(), CompArnr2.c_str(), speed, BitRate, opt, CompressString, opt.arnr_type, 0, cpu_tick2);

        if (Time2 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.arnr_type = 3;
        unsigned int cpu_tick3 = 0;

        Time3 = vpxt_time_compress_ivf_to_ivf(input.c_str(), CompArnr3.c_str(), speed, BitRate, opt, CompressString, opt.arnr_type, 0, cpu_tick3);

        if (Time3 == -1)
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

    double CompArnr0PSNR = vpxt_ivf_psnr(input.c_str(), CompArnr1AltRef0.c_str(), 0, 0, 1, NULL);
    double CompArnr1PSNR = vpxt_ivf_psnr(input.c_str(), CompArnr1.c_str(), 0, 0, 1, NULL);
    double CompArnr2PSNR = vpxt_ivf_psnr(input.c_str(), CompArnr2.c_str(), 0, 0, 1, NULL);
    double CompArnr3PSNR = vpxt_ivf_psnr(input.c_str(), CompArnr3.c_str(), 0, 0, 1, NULL);

    double CompArnr0DataRate = vpxt_ivf_data_rate(CompArnr1AltRef0.c_str(), 1);
    double CompArnr1DataRate = vpxt_ivf_data_rate(CompArnr1.c_str(), 1);
    double CompArnr2DataRate = vpxt_ivf_data_rate(CompArnr2.c_str(), 1);
    double CompArnr3DataRate = vpxt_ivf_data_rate(CompArnr3.c_str(), 1);

    char CompArnr1AltRef0Name[256];
    char CompArnr1Name[256];
    char CompArnr2Name[256];
    char CompArnr3Name[256];

    vpxt_file_name(CompArnr1AltRef0.c_str(), CompArnr1AltRef0Name, 0);
    vpxt_file_name(CompArnr1.c_str(), CompArnr1Name, 0);
    vpxt_file_name(CompArnr2.c_str(), CompArnr2Name, 0);
    vpxt_file_name(CompArnr3.c_str(), CompArnr3Name, 0);

    tprintf(PRINT_BTH, "\nComparing %s to %s\n", CompArnr1AltRef0Name, CompArnr1Name);
    int Compare1 = vpxt_compare_ivf(CompArnr1AltRef0.c_str(), CompArnr1.c_str());

    if (Compare1 >= 0)
        tprintf(PRINT_BTH, "\n Files differ at frame: %i\n", Compare1);
    else
        tprintf(PRINT_BTH, "\n Files are identical\n");

    tprintf(PRINT_BTH, "\nComparing %s to %s\n", CompArnr1Name, CompArnr2Name);
    int Compare2 = vpxt_compare_ivf(CompArnr1.c_str(), CompArnr2.c_str());

    if (Compare2 >= 0)
        tprintf(PRINT_BTH, "\n Files differ at frame: %i\n", Compare2);
    else
        tprintf(PRINT_BTH, "\n Files are identical\n");

    tprintf(PRINT_BTH, "\nComparing %s to %s\n", CompArnr2Name, CompArnr3Name);
    int Compare3 = vpxt_compare_ivf(CompArnr2.c_str(), CompArnr3.c_str());

    if (Compare3 >= 0)
        tprintf(PRINT_BTH, "\n Files differ at frame: %i\n", Compare3);
    else
        tprintf(PRINT_BTH, "\n Files are identical\n");

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

    if (Compare1 >= 0)
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed", CompArnr1AltRef0Name, CompArnr1Name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed", CompArnr1AltRef0Name, CompArnr1Name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (Compare2 >= 0)
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed", CompArnr1Name, CompArnr2Name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed", CompArnr1Name, CompArnr2Name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (Compare3 >= 0)
    {
        vpxt_formated_print(RESPRT, "%s not identical to %s - Passed", CompArnr2Name, CompArnr3Name);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s identical to %s - Failed", CompArnr2Name, CompArnr3Name);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (CompArnr1PSNR > CompArnr0PSNR) // if greater than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 1 PSNR: %.2f > Alt Ref 0 PSNR: %.2f - Passed", CompArnr1PSNR, CompArnr0PSNR);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {

        if (CompArnr1PSNR >= (CompArnr0PSNR - (CompArnr0PSNR * 0.05))) // if less than but within 5% - pass
        {
            vpxt_formated_print(RESPRT, "Arnr 1 PSNR: %.2f is with in 5%% of Alt Ref 0 PSNR: %.2f - Passed", CompArnr1PSNR, CompArnr0PSNR);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 1 PSNR: %.2f not within 5%% of Alt Ref 0 PSNR: %.2f - Failed", CompArnr1PSNR, CompArnr0PSNR);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (CompArnr2PSNR > CompArnr0PSNR) // if greater than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 2 PSNR: %.2f > Alt Ref 0 PSNR: %.2f - Passed", CompArnr2PSNR, CompArnr0PSNR);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {

        if (CompArnr2PSNR >= (CompArnr0PSNR - (CompArnr0PSNR * 0.05))) // if less than but within 5% - pass
        {
            vpxt_formated_print(RESPRT, "Arnr 2 PSNR: %.2f is with in 5%% of Alt Ref 0 PSNR: %.2f - Passed", CompArnr2PSNR, CompArnr0PSNR);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 2 PSNR: %.2f not within 5%% of Alt Ref 0 PSNR: %.2f - Failed", CompArnr2PSNR, CompArnr0PSNR);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (CompArnr3PSNR > CompArnr0PSNR) // if greater than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 3 PSNR: %.2f > Alt Ref 0 PSNR: %.2f - Passed", CompArnr3PSNR, CompArnr0PSNR);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {

        if (CompArnr3PSNR >= (CompArnr0PSNR - (CompArnr0PSNR * 0.05))) // if less than but within 5% - pass
        {
            vpxt_formated_print(RESPRT, "Arnr 3 PSNR: %.2f is with in 5%% of Alt Ref 0 PSNR: %.2f - Passed", CompArnr3PSNR, CompArnr0PSNR);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 3 PSNR: %.2f not within 5%% of Alt Ref 0 PSNR: %.2f - Failed", CompArnr3PSNR, CompArnr0PSNR);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (CompArnr1DataRate < CompArnr0DataRate) // if less than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 1 Data Rate: %.2f < Alt Ref 0 Data Rate: %.2f - Passed", CompArnr1DataRate, CompArnr0DataRate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {

        if (CompArnr1PSNR <= (CompArnr0PSNR + (CompArnr0PSNR * 0.05))) // if greater than but within 5% - pass
        {
            vpxt_formated_print(RESPRT, "Arnr 1 Data Rate: %.2f is with in 5%% of Alt Ref 0 Data Rate: %.2f - Passed", CompArnr1DataRate, CompArnr0DataRate);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 1 Data Rate: %.2f not within 5%% of Alt Ref 0 Data Rate: %.2f - Failed", CompArnr1DataRate, CompArnr0DataRate);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (CompArnr2DataRate < CompArnr0DataRate) // if less than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 2 Data Rate: %.2f < Alt Ref 0 Data Rate: %.2f - Passed", CompArnr2DataRate, CompArnr0DataRate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {

        if (CompArnr2PSNR <= (CompArnr0PSNR + (CompArnr0PSNR * 0.05))) // if greater than but within 5% - pass
        {
            vpxt_formated_print(RESPRT, "Arnr 2 Data Rate: %.2f is with in 5%% of Alt Ref 0 Data Rate: %.2f - Passed", CompArnr2DataRate, CompArnr0DataRate);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 2 Data Rate: %.2f not within 5%% of Alt Ref 0 Data Rate: %.2f - Failed", CompArnr2DataRate, CompArnr0DataRate);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (CompArnr3DataRate < CompArnr0DataRate) // if less than - pass
    {
        vpxt_formated_print(RESPRT, "Arnr 3 Data Rate: %.2f < Alt Ref 0 Data Rate: %.2f - Passed", CompArnr3DataRate, CompArnr0DataRate);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {

        if (CompArnr3PSNR <= (CompArnr0PSNR + (CompArnr0PSNR * 0.05))) // if greater than but within 5% - pass
        {
            vpxt_formated_print(RESPRT, "Arnr 3 Data Rate: %.2f is with in 5%% of Alt Ref 0 Data Rate: %.2f - Passed", CompArnr3DataRate, CompArnr0DataRate);
            tprintf(PRINT_BTH, "\n");
        }
        else // if less than and not with in 5% - fail
        {
            vpxt_formated_print(RESPRT, "Arnr 3 Data Rate: %.2f not within 5%% of Alt Ref 0 Data Rate: %.2f - Failed", CompArnr3DataRate, CompArnr0DataRate);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(4, CompArnr1AltRef0.c_str(), CompArnr1.c_str(), CompArnr2.c_str(), CompArnr3.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(4, CompArnr1AltRef0.c_str(), CompArnr1.c_str(), CompArnr2.c_str(), CompArnr3.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);

    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
