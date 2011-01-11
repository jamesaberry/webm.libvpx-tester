#include "vpxt_test_declarations.h"

int test_two_pass_vs_two_pass_best(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_two_pass_vs_two_pass_best";

    if (!(argc == 4 || argc == 5))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        tprintf(PRINT_STD,
                "\n\n"
                "    <Input File>\n"
                "    <Target Bit Rate>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int BitRate = atoi(argv[3]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string TwoPassOutFile1 = CurTestDirStr;
    TwoPassOutFile1.append(slashCharStr());
    TwoPassOutFile1.append(MyDir);
    TwoPassOutFile1.append("_compression_two_pass_1.ivf");

    std::string TwoPassOutFile2 = CurTestDirStr;
    TwoPassOutFile2.append(slashCharStr());
    TwoPassOutFile2.append(MyDir);
    TwoPassOutFile2.append("_compression_two_pass_2.ivf");

    std::string TwoPassOutFile3 = CurTestDirStr;
    TwoPassOutFile3.append(slashCharStr());
    TwoPassOutFile3.append(MyDir);
    TwoPassOutFile3.append("_compression_two_pass_3.ivf");

    std::string TwoPassBestOutFile1 = CurTestDirStr;
    TwoPassBestOutFile1.append(slashCharStr());
    TwoPassBestOutFile1.append(MyDir);
    TwoPassBestOutFile1.append("_compression_two_pass_best_1.ivf");

    std::string TwoPassBestOutFile2 = CurTestDirStr;
    TwoPassBestOutFile2.append(slashCharStr());
    TwoPassBestOutFile2.append(MyDir);
    TwoPassBestOutFile2.append("_compression_two_pass_best_2.ivf");

    std::string TwoPassBestOutFile3 = CurTestDirStr;
    TwoPassBestOutFile3.append(slashCharStr());
    TwoPassBestOutFile3.append(MyDir);
    TwoPassBestOutFile3.append("_compression_two_pass_best_3.ivf");

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
    if (argc == 5)
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
    int CompressInt = opt.allow_df;

    int BitRate1 = BitRate - (BitRate * 0.3);
    int BitRate2 = BitRate;
    int BitRate3 = BitRate + (BitRate * 0.3);

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = MODE_SECONDPASS;
        opt.target_bandwidth = BitRate1;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), TwoPassOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate2;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), TwoPassOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate3;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), TwoPassOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.Mode = MODE_SECONDPASS_BEST;
        opt.target_bandwidth = BitRate1;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), TwoPassBestOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate2;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), TwoPassBestOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate3;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), TwoPassBestOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

    tprintf(PRINT_STD, "\n");

    float GoodSize1 = vpxt_ivf_data_rate(TwoPassOutFile1.c_str(), 1);
    float BestSize1 = vpxt_ivf_data_rate(TwoPassBestOutFile1.c_str(), 1);
    float GoodSize2 = vpxt_ivf_data_rate(TwoPassOutFile2.c_str(), 1);
    float BestSize2 = vpxt_ivf_data_rate(TwoPassBestOutFile2.c_str(), 1);
    float GoodSize3 = vpxt_ivf_data_rate(TwoPassOutFile3.c_str(), 1);
    float BestSize3 = vpxt_ivf_data_rate(TwoPassBestOutFile3.c_str(), 1);

    double PSNRG1;
    double PSNRB1;
    double PSNRG2;
    double PSNRB2;
    double PSNRG3;
    double PSNRB3;

    PSNRG1 = vpxt_ivf_psnr(input.c_str(), TwoPassOutFile1.c_str(), 1, 0, 1, NULL);
    PSNRB1 = vpxt_ivf_psnr(input.c_str(), TwoPassBestOutFile1.c_str(), 1, 0, 1, NULL);
    PSNRG2 = vpxt_ivf_psnr(input.c_str(), TwoPassOutFile2.c_str(), 1, 0, 1, NULL);
    PSNRB2 = vpxt_ivf_psnr(input.c_str(), TwoPassBestOutFile2.c_str(), 1, 0, 1, NULL);
    PSNRG3 = vpxt_ivf_psnr(input.c_str(), TwoPassOutFile3.c_str(), 1, 0, 1, NULL);
    PSNRB3 = vpxt_ivf_psnr(input.c_str(), TwoPassBestOutFile3.c_str(), 1, 0, 1, NULL);

    float GoodA = 0;
    float GoodB = 0;
    float GoodC = 0;

    float BestA = 0;
    float BestB = 0;
    float BestC = 0;

    float minCommon = 0;
    float maxCommon = 0;

    if (GoodSize1 > BestSize1) //take area over same range we have decent data for.
    {
        minCommon = GoodSize1;
    }
    else
    {
        minCommon = BestSize1;
    }

    if (GoodSize3 > BestSize3)
    {
        maxCommon = BestSize3;
    }
    else
    {
        maxCommon = GoodSize3;
    }

    vpxt_solve_quadradic(GoodSize1, GoodSize2, GoodSize3, PSNRG1, PSNRG2, PSNRG3, GoodA, GoodB, GoodC);
    float GoodAreaVal = vpxt_area_under_quadradic(GoodA, GoodB, GoodC, minCommon, maxCommon);

    vpxt_solve_quadradic(BestSize1, BestSize2, BestSize3, PSNRB1, PSNRB2, PSNRB3, BestA, BestB, BestC);
    float BestAreaVal = vpxt_area_under_quadradic(BestA, BestB, BestC, minCommon, maxCommon);


    tprintf(PRINT_BTH, "\n\n"
            "Data Points:\n"
            "\n"
            " Good Q\n"
            "\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "\n"
            " Best Q\n"
            "\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "(%.2f,%2.2f)\n"
            "\n"
            "\n"
            , GoodSize1, PSNRG1
            , GoodSize2, PSNRG2
            , GoodSize3, PSNRG3
            , BestSize1, PSNRB1
            , BestSize2, PSNRB2
            , BestSize3, PSNRB3
           );

    tprintf(PRINT_BTH, "Two Pass Curve: y = %fx^2 + %fx + %f\n", GoodA, GoodB, GoodC);
    tprintf(PRINT_BTH, "Two Pass Best Curve: y = %fx^2 + %fx + %f\n", BestA, BestB, BestC);
    tprintf(PRINT_BTH, "\nTwo Pass area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, GoodAreaVal);
    tprintf(PRINT_BTH, "Two Pass Best area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, BestAreaVal);

    int Pass = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (GoodAreaVal == BestAreaVal)
    {
        vpxt_formated_print(RESPRT, "Two Pass Best area under curve: %.2f == Two Pass area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
        tprintf(PRINT_BTH, "\n");
    }

    if (BestAreaVal > GoodAreaVal)
    {
        vpxt_formated_print(RESPRT, "Two Pass Best area under curve: %.2f > Two Pass area under curve: %.2f - Passed", BestAreaVal, GoodAreaVal);
        tprintf(PRINT_BTH, "\n");
        Pass = 1;
    }

    if (BestAreaVal < GoodAreaVal)
    {
        vpxt_formated_print(RESPRT, "Two Pass Best area under curve: %.2f < Two Pass area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
        tprintf(PRINT_BTH, "\n");
    }

    if (Pass == 1)
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