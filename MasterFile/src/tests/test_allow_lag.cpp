#include "vpxt_test_declarations.h"

int test_allow_lag(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Allow Lag";
    char *MyDir = "test_allow_lag";

    if (!(argc == 6 || argc == 5))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        printf(
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

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);;
    char *input2 = argv[5];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    string AllowLagon = CurTestDirStr;
    AllowLagon.append(slashCharStr());
    AllowLagon.append(MyDir);
    AllowLagon.append("_compression_1.ivf");

    string AllowLagoff = CurTestDirStr;
    AllowLagoff.append(slashCharStr());
    AllowLagoff.append(MyDir);
    AllowLagoff.append("_compression_0.ivf");

    /////////////OutPutfile////////////
    string TextfileString = CurTestDirStr;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == COMP_ONLY || TestType == TEST_AND_COMP)
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

    if (TestType == TEST_AND_COMP)
        print_header_full_test(argc, argv, MainTestDirChar);

    if (TestType == COMP_ONLY)
        print_header_compression_only(argc, argv, MainTestDirChar);

    if (TestType == TEST_ONLY)
        print_header_test_only(argc, argv, CurTestDirStr);

    vpxt_cap_string_print(PRINT_BOTH, "%s", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf("\nInput Settings file %s does not exist\n", argv[argc-1]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.lag_in_frames = 10;

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 =Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.allow_lag = 0;

        if (vpxt_compress_ivf_to_ivf(input, AllowLagoff.c_str(), speed, BitRate, opt, CompressString, 0, 1) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.allow_lag = 1;

        if (vpxt_compress_ivf_to_ivf(input, AllowLagon.c_str(), speed, BitRate, opt, CompressString, 1, 1) == -1)
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

    ////////////Make Sure That Frames are lagged////////////
    string QuantInStr = AllowLagon;
    QuantInStr.erase(QuantInStr.length() - 4, 4);
    QuantInStr.append("_quantizers.txt");
    char QuantInChar[255] = "";
    snprintf(QuantInChar, 255, "%s", QuantInStr.c_str());

    int LagInFramesFound = vpxt_lag_in_frames_check(QuantInChar);
    int AllowLagONAltRefCount = vpxt_display_alt_ref_frames(AllowLagon.c_str(), 1);
    int AllowLagOFFAltRefCount = vpxt_display_alt_ref_frames(AllowLagoff.c_str(), 1);
    int VisibleFrameONCount = vpxt_display_visible_frames(AllowLagon.c_str(), 1);
    int VisibleFrameOFFCount = vpxt_display_visible_frames(AllowLagoff.c_str(), 1);

    char AllowLagonFilename[255];
    vpxt_file_name(AllowLagon.c_str(), AllowLagonFilename, 0);
    char AllowLagoffFilename[255];
    vpxt_file_name(AllowLagoff.c_str(), AllowLagoffFilename, 0);

    int lngRC = vpxt_compare_ivf(AllowLagoff.c_str(), AllowLagon.c_str());

    tprintf("\n\nResults:\n\n");

    int fail = 0;

    if (LagInFramesFound == opt.lag_in_frames)
    {
        vpxt_formated_print(RESPRT, "Correct number of LagInFrames (%i) detected in compression for %s - Passed", opt.lag_in_frames, AllowLagonFilename);
        tprintf("\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Correct number of LagInFrames (%i) not detected in compression for %s - Failed", opt.lag_in_frames, AllowLagonFilename);
        tprintf("\n");
        fail = 1;
    }

    if (AllowLagONAltRefCount > 0)
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames exist for %s - Passed", AllowLagonFilename);
        tprintf("\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames do not exist for %s - Failed", AllowLagonFilename);
        tprintf("\n");
        fail = 1;
    }

    if (VisibleFrameONCount == VisibleFrameOFFCount)
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i == Visible Frames for %s = %i - Passed", AllowLagonFilename, VisibleFrameONCount, AllowLagoffFilename, VisibleFrameOFFCount);
        tprintf("\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i != Visible Frames for %s = %i - Failed", AllowLagonFilename, VisibleFrameONCount, AllowLagoffFilename, VisibleFrameOFFCount);
        tprintf("\n");
        fail = 1;
    }

    if (lngRC >= 0)
    {
        vpxt_formated_print(RESPRT, "%s is not identical to %s - Passed", AllowLagonFilename, AllowLagoffFilename);
        tprintf("\n");
    }

    if (lngRC == -1)
    {
        vpxt_formated_print(RESPRT, "%s is identical to %s - Failed", AllowLagonFilename, AllowLagoffFilename);
        tprintf("\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf("\nPassed\n");
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf("\nFailed\n");
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}