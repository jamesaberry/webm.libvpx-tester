#include "vpxt_test_declarations.h"

int test_good_vs_best(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "Allow DF";
    char *input = argv[2];

    if (!(argc == 4 || argc == 5))
    {
        printf(
            "  GoodQvBestQ \n\n"
            "    <inputfile>\n"
            "    <Target Bit Rate>\n ");

        return 0;
    }

    /////////////////Might want to see if you can make a function to do this so its a bit cleaner/////////////////
    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "GoodQvBestQ";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;


    string GvBgOutFile1 = WorkingDirString;
    string GvBbOutFile1 = WorkingDirString;
    string GvBgOutFile2 = WorkingDirString;
    string GvBbOutFile2 = WorkingDirString;
    string GvBgOutFile3 = WorkingDirString;
    string GvBbOutFile3 = WorkingDirString;

    GvBgOutFile1.append(slashCharStr());
    GvBgOutFile1.append("GoodVsBestGood1.ivf");
    GvBbOutFile1.append(slashCharStr());
    GvBbOutFile1.append("GoodVsBestBest1.ivf");
    GvBgOutFile2.append(slashCharStr());
    GvBgOutFile2.append("GoodVsBestGood2.ivf");
    GvBbOutFile2.append(slashCharStr());
    GvBbOutFile2.append("GoodVsBestBest2.ivf");
    GvBgOutFile3.append(slashCharStr());
    GvBgOutFile3.append("GoodVsBestGood3.ivf");
    GvBbOutFile3.append(slashCharStr());
    GvBbOutFile3.append("GoodVsBestBest3.ivf");

    //char GvBgOutFile1[255];
    //char GvBgOutFile2[255];
    //char GvBgOutFile3[255];
    //char GvBbOutFile1[255];
    //char GvBbOutFile2[255];
    //char GvBbOutFile3[255];

    //snprintf(GvBgOutFile1, 255, "%s", WorkingDir4.c_str());
    //snprintf(GvBbOutFile1, 255, "%s", WorkingDir5.c_str());
    //snprintf(GvBgOutFile2, 255, "%s", WorkingDir4b.c_str());
    //snprintf(GvBbOutFile2, 255, "%s", WorkingDir5b.c_str());
    //snprintf(GvBgOutFile3, 255, "%s", WorkingDir4c.c_str());
    //snprintf(GvBbOutFile3, 255, "%s", WorkingDir5c.c_str());

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
    int BitRate = atoi(argv[3]);

    tprintf("Good Quality vs Best Quality Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 5)
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
    int CompressInt = opt.allow_df;

    int BitRate1 = BitRate - (BitRate * 0.3);
    int BitRate2 = BitRate;
    int BitRate3 = BitRate + (BitRate * 0.3);

    //opt.target_bandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.target_bandwidth = BitRate1;
        opt.Mode = MODE_GOODQUALITY;

        if (vpxt_compress_ivf_to_ivf(input, GvBgOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate2;

        if (vpxt_compress_ivf_to_ivf(input, GvBgOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate3;

        if (vpxt_compress_ivf_to_ivf(input, GvBgOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate1;
        opt.Mode = MODE_BESTQUALITY;

        if (vpxt_compress_ivf_to_ivf(input, GvBbOutFile1.c_str(), speed, BitRate1, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate2;

        if (vpxt_compress_ivf_to_ivf(input, GvBbOutFile2.c_str(), speed, BitRate2, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.target_bandwidth = BitRate3;

        if (vpxt_compress_ivf_to_ivf(input, GvBbOutFile3.c_str(), speed, BitRate3, opt, CompressString, CompressInt, 0) == -1)
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

    tprintf("\n");

    float GoodSize1 = vpxt_ivf_data_rate(GvBgOutFile1.c_str(), 1);
    float BestSize1 = vpxt_ivf_data_rate(GvBbOutFile1.c_str(), 1);
    float GoodSize2 = vpxt_ivf_data_rate(GvBgOutFile2.c_str(), 1);
    float BestSize2 = vpxt_ivf_data_rate(GvBbOutFile2.c_str(), 1);
    float GoodSize3 = vpxt_ivf_data_rate(GvBgOutFile3.c_str(), 1);
    float BestSize3 = vpxt_ivf_data_rate(GvBbOutFile3.c_str(), 1);

    double PSNRG1;
    double PSNRB1;
    double PSNRG2;
    double PSNRB2;
    double PSNRG3;
    double PSNRB3;

    PSNRG1 = vpxt_ivf_psnr(input, GvBgOutFile1.c_str(), 1, 0, 1, NULL);
    PSNRB1 = vpxt_ivf_psnr(input, GvBbOutFile1.c_str(), 1, 0, 1, NULL);
    PSNRG2 = vpxt_ivf_psnr(input, GvBgOutFile2.c_str(), 1, 0, 1, NULL);
    PSNRB2 = vpxt_ivf_psnr(input, GvBbOutFile2.c_str(), 1, 0, 1, NULL);
    PSNRG3 = vpxt_ivf_psnr(input, GvBgOutFile3.c_str(), 1, 0, 1, NULL);
    PSNRB3 = vpxt_ivf_psnr(input, GvBbOutFile3.c_str(), 1, 0, 1, NULL);

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

    tprintf("\n\n"
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

    tprintf("Good Quality Curve: y = %fx^2 + %fx + %f\n", GoodA, GoodB, GoodC);
    tprintf("Best Quality Curve: y = %fx^2 + %fx + %f\n", BestA, BestB, BestC);
    tprintf("\nGood Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, GoodAreaVal);
    tprintf("Best Quality area under curve for interval %.2f - %.2f = %.2f\n", minCommon, maxCommon, BestAreaVal);

    int Pass = 0;

    tprintf("\n\nResults:\n\n");

    if (GoodAreaVal == BestAreaVal)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Best Quality area under curve: %.2f == Good Quality area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (BestAreaVal > GoodAreaVal)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Best Quality area under curve: %.2f > Good Quality area under curve: %.2f - Passed", BestAreaVal, GoodAreaVal);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 1;
    }

    if (BestAreaVal < GoodAreaVal)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Best Quality area under curve: %.2f < Good Quality area under curve: %.2f - Failed", BestAreaVal, GoodAreaVal);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (Pass == 1)
    {
        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        tprintf("\nFailed\n");

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