#include "vpxt_test_declarations.h"

int test_good_vs_best(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_good_vs_best";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int BitRate = atoi(argv[3]);
    std::string EncForm = argv[4];

    int speed = 0;

    /////////////////Might want to see if you can make a function to do this so its a bit cleaner/////////////////
    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string GoodOutFile1 = CurTestDirStr;
    GoodOutFile1.append(slashCharStr());
    GoodOutFile1.append(MyDir);
    GoodOutFile1.append("_compression_good_1");
    vpxt_enc_format_append(GoodOutFile1, EncForm);

    std::string GoodOutFile2 = CurTestDirStr;
    GoodOutFile2.append(slashCharStr());
    GoodOutFile2.append(MyDir);
    GoodOutFile2.append("_compression_good_2");
    vpxt_enc_format_append(GoodOutFile2, EncForm);

    std::string GoodOutFile3 = CurTestDirStr;
    GoodOutFile3.append(slashCharStr());
    GoodOutFile3.append(MyDir);
    GoodOutFile3.append("_compression_good_3");
    vpxt_enc_format_append(GoodOutFile3, EncForm);

    std::string BestOutFile1 = CurTestDirStr;
    BestOutFile1.append(slashCharStr());
    BestOutFile1.append(MyDir);
    BestOutFile1.append("_compression_best_1");
    vpxt_enc_format_append(BestOutFile1, EncForm);

    std::string BestOutFile2 = CurTestDirStr;
    BestOutFile2.append(slashCharStr());
    BestOutFile2.append(MyDir);
    BestOutFile2.append("_compression_best_2");
    vpxt_enc_format_append(BestOutFile2, EncForm);

    std::string BestOutFile3 = CurTestDirStr;
    BestOutFile3.append(slashCharStr());
    BestOutFile3.append(MyDir);
    BestOutFile3.append("_compression_best_3");
    vpxt_enc_format_append(BestOutFile3, EncForm);

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
    if (inputCheck == 2)
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
        opt.target_bandwidth = BitRate1;
        opt.Mode = MODE_GOODQUALITY;

        if (vpxt_compress(input.c_str(), GoodOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate2;

        if (vpxt_compress(input.c_str(), GoodOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate3;

        if (vpxt_compress(input.c_str(), GoodOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate1;
        opt.Mode = MODE_BESTQUALITY;

        if (vpxt_compress(input.c_str(), BestOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate2;

        if (vpxt_compress(input.c_str(), BestOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate3;

        if (vpxt_compress(input.c_str(), BestOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0, EncForm) == -1)
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

    float GoodSize1 = vpxt_data_rate(GoodOutFile1.c_str(), 1);
    float BestSize1 = vpxt_data_rate(BestOutFile1.c_str(), 1);
    float GoodSize2 = vpxt_data_rate(GoodOutFile2.c_str(), 1);
    float BestSize2 = vpxt_data_rate(BestOutFile2.c_str(), 1);
    float GoodSize3 = vpxt_data_rate(GoodOutFile3.c_str(), 1);
    float BestSize3 = vpxt_data_rate(BestOutFile3.c_str(), 1);

    double PSNRG1;
    double PSNRB1;
    double PSNRG2;
    double PSNRB2;
    double PSNRG3;
    double PSNRB3;

    PSNRG1 = vpxt_psnr(input.c_str(), GoodOutFile1.c_str(), 1, 0, 1, NULL);
    PSNRB1 = vpxt_psnr(input.c_str(), BestOutFile1.c_str(), 1, 0, 1, NULL);
    PSNRG2 = vpxt_psnr(input.c_str(), GoodOutFile2.c_str(), 1, 0, 1, NULL);
    PSNRB2 = vpxt_psnr(input.c_str(), BestOutFile2.c_str(), 1, 0, 1, NULL);
    PSNRG3 = vpxt_psnr(input.c_str(), GoodOutFile3.c_str(), 1, 0, 1, NULL);
    PSNRB3 = vpxt_psnr(input.c_str(), BestOutFile3.c_str(), 1, 0, 1, NULL);

    //data rates not always in order so find smallest observed data rate
    float GoodSizeMin = GoodSize1;
    float BestSizeMin = BestSize1;

    if(GoodSize2 < GoodSizeMin)
        GoodSizeMin = GoodSize2;

    if(GoodSize3 < GoodSizeMin)
        GoodSizeMin = GoodSize3;

    if(BestSize2 < BestSizeMin)
        BestSizeMin = BestSize2;

    if(BestSize3 < BestSizeMin)
        BestSizeMin = BestSize3;

    //data rates not always in order so find largest observed data rate
    float GoodSizeMax = GoodSize3;
    float BestSizeMax = BestSize3;

    if(GoodSize2 > GoodSizeMax)
        GoodSizeMax = GoodSize2;

    if(GoodSize1 > GoodSizeMax)
        GoodSizeMax = GoodSize1;

    if(BestSize2 > BestSizeMax)
        BestSizeMax = BestSize2;

    if(BestSize1 > BestSizeMax)
        BestSizeMax = BestSize1;

    float GoodA = 0;
    float GoodB = 0;
    float GoodC = 0;

    float BestA = 0;
    float BestB = 0;
    float BestC = 0;

    float minCommon = 0;
    float maxCommon = 0;

    //take area over same range we have decent data for.
    if (GoodSizeMin > BestSizeMin)
        minCommon = GoodSizeMin;
    else
        minCommon = BestSizeMin;

    if (GoodSizeMax > BestSizeMax)
        maxCommon = BestSizeMax;
    else
        maxCommon = GoodSizeMax;

    vpxt_solve_quadratic(GoodSize1, GoodSize2, GoodSize3, PSNRG1, PSNRG2, PSNRG3, GoodA, GoodB, GoodC);
    float GoodAreaVal = vpxt_area_under_quadratic(GoodA, GoodB, GoodC, minCommon, maxCommon);

    vpxt_solve_quadratic(BestSize1, BestSize2, BestSize3, PSNRB1, PSNRB2, PSNRB3, BestA, BestB, BestC);
    float BestAreaVal = vpxt_area_under_quadratic(BestA, BestB, BestC, minCommon, maxCommon);

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

    tprintf(PRINT_BTH, "Good Quality Curve: y = %fx^2 + %fx + %f\n", GoodA, GoodB, GoodC);
    tprintf(PRINT_BTH, "Best Quality Curve: y = %fx^2 + %fx + %f\n", BestA, BestB, BestC);
    tprintf(PRINT_BTH, "\nGood Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, GoodAreaVal);
    tprintf(PRINT_BTH, "Best Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, BestAreaVal);

    int Pass = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (GoodAreaVal == BestAreaVal)
    {
        vpxt_formated_print(RESPRT, "Best Quality area under curve: %.2f == Good Quality area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
        tprintf(PRINT_BTH, "\n");
    }

    if (BestAreaVal > GoodAreaVal)
    {
        vpxt_formated_print(RESPRT, "Best Quality area under curve: %.2f > Good Quality area under curve: %.2f - Passed", BestAreaVal, GoodAreaVal);
        tprintf(PRINT_BTH, "\n");
        Pass = 1;
    }

    if (BestAreaVal < GoodAreaVal)
    {
        vpxt_formated_print(RESPRT, "Best Quality area under curve: %.2f < Good Quality area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
        tprintf(PRINT_BTH, "\n");
    }

    if (Pass == 1)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(6, GoodOutFile2.c_str(), GoodOutFile3.c_str(), GoodOutFile1.c_str(), BestOutFile1.c_str(), BestOutFile2.c_str(), BestOutFile3.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(6, GoodOutFile2.c_str(), GoodOutFile3.c_str(), GoodOutFile1.c_str(), BestOutFile1.c_str(), BestOutFile2.c_str(), BestOutFile3.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}