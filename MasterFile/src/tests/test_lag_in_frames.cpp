#include "vpxt_test_declarations.h"

int test_lag_in_frames(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Lag In Frames";

    char *input = argv[2];

    if (!(argc == 7 || argc == 8))
    {
        printf(
            "  LagInFrames \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Lag in Frames 1>\n"
            "    <Lag in Frames 2>\n"
            "    <Optional Settings File>\n"
        );

        return 0;
    }

    ////////////Formatting Test Specific Directory////////////

    string WorkingDirString = ""; // <- All Options need to set a value for this

    string MainDirString = "";
    char *MyDir = "LagInFrames";

    char WorkingDir3[255] = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string LagInFrames0 = WorkingDirString;
    string LagInFrames1 = WorkingDirString;
    string LagInFrames2 = WorkingDirString;

    LagInFrames0.append(slashCharStr());
    LagInFrames0.append("LagInFrames0Output.ivf");
    LagInFrames1.append(slashCharStr());
    LagInFrames1.append("LagInFrames1Output.ivf");
    LagInFrames2.append(slashCharStr());
    LagInFrames2.append("LagInFrames2Output.ivf");

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
    int BitRate = atoi(argv[4]);;

    int Mode = atoi(argv[3]);

    int LagInFrames1Val = atoi(argv[5]);
    int LagInFrames2Val = atoi(argv[6]);

    tprintf("Lag In Frames Test");

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

    if (LagInFrames1Val > 25 || LagInFrames2Val > 25 || LagInFrames2Val < 0 || LagInFrames1Val < 0)
    {
        tprintf("\nLag in Frames settings must be between 0 and 25.\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 =Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.allow_lag = 0;
        opt.lag_in_frames = LagInFrames1Val;

        if (vpxt_compress_ivf_to_ivf(input, LagInFrames0.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.allow_lag = 1;
        opt.lag_in_frames = LagInFrames1Val;

        if (vpxt_compress_ivf_to_ivf(input, LagInFrames1.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.allow_lag = 1;
        opt.lag_in_frames = LagInFrames2Val;

        if (vpxt_compress_ivf_to_ivf(input, LagInFrames2.c_str(), speed, BitRate, opt, CompressString, LagInFrames2Val, 1) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    double LagInFrames0PSNR = vpxt_ivf_psnr(input, LagInFrames0.c_str(), 0, 0, 1, NULL);
    double LagInFrames1PSNR = vpxt_ivf_psnr(input, LagInFrames1.c_str(), 0, 0, 1, NULL);
    double LagInFrames2PSNR = vpxt_ivf_psnr(input, LagInFrames2.c_str(), 0, 0, 1, NULL);

    double TenPer0 = LagInFrames0PSNR / 10;
    double TenPer1 = LagInFrames1PSNR / 10;
    double TenPer2 = LagInFrames2PSNR / 10;

    int lngRC1 = vpxt_compare_ivf(LagInFrames0.c_str(), LagInFrames1.c_str());
    int lngRC2 = vpxt_compare_ivf(LagInFrames1.c_str(), LagInFrames2.c_str());

    string QuantInStr0 = LagInFrames0;
    QuantInStr0.erase(QuantInStr0.length() - 4, 4);
    QuantInStr0.append("_Quantizers.txt");
    int LagInFramesFound0 = vpxt_lag_in_frames_check(QuantInStr0.c_str());

    string QuantInStr1 = LagInFrames1;
    QuantInStr1.erase(QuantInStr1.length() - 4, 4);
    QuantInStr1.append("_Quantizers.txt");
    int LagInFramesFound1 = vpxt_lag_in_frames_check(QuantInStr1.c_str());

    string QuantInStr2 = LagInFrames2;
    QuantInStr2.erase(QuantInStr2.length() - 4, 4);
    QuantInStr2.append("_Quantizers.txt");
    int LagInFramesFound2 = vpxt_lag_in_frames_check(QuantInStr2.c_str());

    int PSNRTally = 0;
    int fail = 0;

    char LagInFrames0FileName[255] = "";
    char LagInFrames1FileName[255] = "";
    char LagInFrames2FileName[255] = "";

    vpxt_file_name(LagInFrames0.c_str(), LagInFrames0FileName, 0);
    vpxt_file_name(LagInFrames1.c_str(), LagInFrames1FileName, 0);
    vpxt_file_name(LagInFrames2.c_str(), LagInFrames2FileName, 0);

    tprintf("\n\nResults:\n\n");

    if (LagInFramesFound0 == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s properly lagged %i frames - Passed", LagInFrames0FileName, LagInFramesFound0);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s improperly lagged frames %i - Failed", LagInFrames0FileName, LagInFramesFound0);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (LagInFramesFound1 == LagInFrames1Val)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s properly lagged %i frames - Passed", LagInFrames1FileName, LagInFramesFound1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s improperly lagged %i frames - Failed", LagInFrames1FileName, LagInFramesFound1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (LagInFramesFound2 == LagInFrames2Val)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s properly lagged %i frames - Passed", LagInFrames2FileName, LagInFramesFound2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s improperly lagged %i frames - Failed", LagInFrames2FileName, LagInFramesFound2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (lngRC1 == -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s identical to %s - Failed", LagInFrames0FileName, LagInFrames1FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s not identical to %s - Passed", LagInFrames0FileName, LagInFrames1FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (lngRC2 == -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s identical to %s - Failed", LagInFrames1FileName, LagInFrames2FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "%s not identical to %s - Passed", LagInFrames1FileName, LagInFrames2FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (LagInFrames1PSNR <= (LagInFrames0PSNR + TenPer0) && LagInFrames1PSNR >= (LagInFrames0PSNR - TenPer0))
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "PSNR for %s is within 10%% of PSNR for %s - %.2f < %.2f < %.2f - Passed", LagInFrames0FileName, LagInFrames1FileName, (LagInFrames0PSNR - TenPer0), LagInFrames1PSNR, (LagInFrames0PSNR + TenPer0));
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        if (!(LagInFrames1PSNR <= (LagInFrames0PSNR + TenPer0)))
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames0FileName, LagInFrames1FileName, (LagInFrames0PSNR + TenPer0), LagInFrames1PSNR);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames0FileName, LagInFrames1FileName, LagInFrames1PSNR, (LagInFrames0PSNR - TenPer0));
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
    }

    if (LagInFrames2PSNR <= (LagInFrames1PSNR + TenPer1) && LagInFrames2PSNR >= (LagInFrames1PSNR - TenPer1))
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "PSNR for %s is within 10%% of PSNR for %s - %.2f < %.2f < %.2f - Passed", LagInFrames1FileName, LagInFrames2FileName, (LagInFrames1PSNR - TenPer1), LagInFrames2PSNR, (LagInFrames1PSNR + TenPer1));
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        if (!(LagInFrames1PSNR <= (LagInFrames0PSNR + TenPer0)))
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames1FileName, LagInFrames2FileName, (LagInFrames1PSNR + TenPer1), LagInFrames2PSNR);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "PSNR for %s is not within 10%% of PSNR for %s - %.2f < %.2f - Failed", LagInFrames1FileName, LagInFrames2FileName, LagInFrames2PSNR, (LagInFrames1PSNR - TenPer1));
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;
        }
    }

    if (fail == 0)
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